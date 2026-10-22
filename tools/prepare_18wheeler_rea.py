#!/usr/bin/env python3
# prepare_18wheeler_rea.py - Extracts IP.BIN & 1ST_READ.BIN from Track 3 and builds 1ST_READ.elf at 0x0C010000

import argparse
import hashlib
import struct
from pathlib import Path

TRACK3_START_LBA = 45000
RAW_SECTOR_SIZE = 2352
USER_DATA_OFFSET = 16
USER_DATA_SIZE = 2048
P0_LOAD_ADDR = 0x0C010000

def read_sector_2048(fp, lba: int) -> bytes:
    idx = lba - TRACK3_START_LBA
    fp.seek(idx * RAW_SECTOR_SIZE + USER_DATA_OFFSET)
    return fp.read(USER_DATA_SIZE)

def extract_from_track3(track3_path: Path, out_dir: Path):
    print(f'[*] Reading GD-ROM High-Density Track 3: {track3_path.name}')
    with track3_path.open('rb') as fp:
        ip_bytes = bytearray()
        for s in range(16):
            ip_bytes.extend(read_sector_2048(fp, TRACK3_START_LBA + s))
        ip_path = out_dir / 'IP.BIN'
        ip_path.write_bytes(ip_bytes)
        print(f'[+] Extracted IP.BIN ({len(ip_bytes)} bytes)')

        pvd = read_sector_2048(fp, TRACK3_START_LBA + 16)
        root_rec = pvd[156:190]
        root_lba = struct.unpack('<I', root_rec[2:6])[0] + 150
        root_len = struct.unpack('<I', root_rec[10:14])[0]

        dir_bytes = bytearray()
        for s in range((root_len + 2047) // 2048):
            dir_bytes.extend(read_sector_2048(fp, root_lba + s))

        pos = 0
        while pos < len(dir_bytes):
            rec_len = dir_bytes[pos]
            if rec_len == 0:
                pos = ((pos // 2048) + 1) * 2048
                continue
            rec = dir_bytes[pos : pos + rec_len]
            extent_lba = struct.unpack('<I', rec[2:6])[0] + 150
            data_len = struct.unpack('<I', rec[10:14])[0]
            name_len = rec[32]
            name = rec[33 : 33 + name_len].decode('ascii', errors='ignore').split(';')[0]
            if name.upper() == '1ST_READ.BIN':
                bin_bytes = bytearray()
                num_sec = (data_len + 2047) // 2048
                for s in range(num_sec):
                    bin_bytes.extend(read_sector_2048(fp, extent_lba + s))
                bin_data = bytes(bin_bytes[:data_len])
                bin_path = out_dir / '1ST_READ.BIN'
                bin_path.write_bytes(bin_data)
                print(f'[+] Extracted 1ST_READ.BIN ({len(bin_data):,} bytes)')
                return ip_path, bin_path
            pos += rec_len
    raise FileNotFoundError('Could not find 1ST_READ.BIN in Track 3 ISO9660 root directory.')

def build_sh4_elf(bin_data: bytes, load_addr: int = P0_LOAD_ADDR) -> bytes:
    ehdr_size, phdr_size, shdr_size, file_offset = 52, 32, 40, 0x1000
    shstrtab = b'\x00.text\x00.shstrtab\x00'
    shstrtab_padded = shstrtab + b'\x00' * ((4 - len(shstrtab) % 4) % 4)
    text_offset = file_offset
    text_size = len(bin_data)
    shstr_offset = text_offset + text_size
    shdr_offset = shstr_offset + len(shstrtab_padded)
    e_ident = b'\x7fELF' + bytes([1, 1, 1, 0, 0]) + b'\x00' * 7
    ehdr = struct.pack('<16sHHIIIIIHHHHHH', e_ident, 2, 42, 1, load_addr, ehdr_size, shdr_offset, 0x09, ehdr_size, phdr_size, 1, shdr_size, 3, 2)
    phdr = struct.pack('<IIIIIIII', 1, text_offset, load_addr, load_addr, text_size, text_size, 7, 0x1000)
    shdr_null = b'\x00' * shdr_size
    shdr_text = struct.pack('<IIIIIIIIII', 1, 1, 6, load_addr, text_offset, text_size, 0, 0, 4, 0)
    shdr_str = struct.pack('<IIIIIIIIII', 7, 3, 0, 0, shstr_offset, len(shstrtab), 0, 0, 1, 0)
    pad_to_text = b'\x00' * (text_offset - (len(ehdr) + len(phdr)))
    return ehdr + phdr + pad_to_text + bin_data + shstrtab_padded + shdr_null + shdr_text + shdr_str

def main():
    parser = argparse.ArgumentParser(description='Prepare 18 Wheeler (Dreamcast) for REA & Ghidra')
    parser.add_argument('--dir', type=Path, default=Path('.'), help='Folder containing Track 3 or 1ST_READ.BIN')
    args = parser.parse_args()
    work_dir = args.dir.resolve()
    bin_path = work_dir / '1ST_READ.BIN'
    ip_path = work_dir / 'IP.BIN'
    if not bin_path.exists():
        track3_candidates = sorted(work_dir.glob('*Track 3*.bin'))
        if not track3_candidates:
            raise SystemExit('[-] Neither 1ST_READ.BIN nor *Track 3*.bin found in directory.')
        ip_path, bin_path = extract_from_track3(track3_candidates[0], work_dir)
    if ip_path.exists():
        ip = ip_path.read_bytes()
        prod_id = ip[0x40:0x4A].decode('ascii', errors='ignore').strip()
        ver = ip[0x4A:0x50].decode('ascii', errors='ignore').strip()
        title = ip[0x80:0x100].decode('ascii', errors='ignore').strip()
        print(f'[+] Disc Header: Title={title} | ProductID={prod_id} | Version={ver}')
    bin_data = bin_path.read_bytes()
    print(f'[+] 1ST_READ.BIN SHA-256: {hashlib.sha256(bin_data).hexdigest()}')
    elf_path = work_dir / '1ST_READ.elf'
    elf_path.write_bytes(build_sh4_elf(bin_data, P0_LOAD_ADDR))
    print(f'[+] Wrote REA-ready SH-4 ELF mapped at 0x{P0_LOAD_ADDR:08X}: {elf_path.name}')

if __name__ == '__main__':
    main()
