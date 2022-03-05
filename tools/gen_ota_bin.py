import os
import argparse
import hashlib
import struct
VERSION           = '1.0.0'

COMPULSORY_SIZE   = True
BYTE_ALIGN        = 0x00000004
# JUMP_TABLE
SIZE_PART_1       = 0x00000800
RUN_ADDR_PART_1   = 0x1fff0800
# ER_IROM1
SIZE_PART_2       = 0x00010000
RUN_ADDR_PART_2   = 0x1fff4800

otafile           = 'ota.bin'
part1_flash_addr  = 0
part1_run_addr    = 0
part1_size        = 0
part1_checksum    = 0
part2_flash_addr  = 0
part2_run_addr    = 0
part2_size        = 0
part2_checksum    = 0


def bin_file_merge(bin_file1, bin_file2, bin_merge):
    if (os.path.getsize(bin_file1) == 0 or os.path.getsize(bin_file1) > SIZE_PART_1 or
        os.path.getsize(bin_file2) == 0 or os.path.getsize(bin_file2) > SIZE_PART_2):
        print('input file size error')
        return False

    f1 = open(bin_file1,"rb")
    f2 = open(bin_file2,"rb")
    fmerge = open(bin_merge,"wb")
    
    # Write JUMP_TABLE
    part1_size = os.path.getsize(bin_file1)
    data = f1.read()
    fmerge.write(data)
    if COMPULSORY_SIZE:
        fmerge.seek(SIZE_PART_1, 0)
        part1_size = SIZE_PART_1
    part1_run_addr = RUN_ADDR_PART_1;
    
    # Write APP
    part2_size = os.path.getsize(bin_file2)
    data = f2.read()
    fmerge.write(data)
    if part2_size % BYTE_ALIGN:
        fill_size = BYTE_ALIGN - part2_size % BYTE_ALIGN
        part2_size += fill_size
        fmerge.write(bytes(fill_size))
    part2_flash_addr = part1_size
    part2_run_addr = RUN_ADDR_PART_2
    
    f1.close()
    f2.close()
    fmerge.close()
    
    print('PartN       FlashAddr   RunAddr     Size        Checksum    ');
    print('%-12d%-12x%-12x%-12x%-12x' % (1, part1_flash_addr, part1_run_addr, part1_size, part1_checksum));
    print('%-12d%-12x%-12x%-12x%-12x' % (2, part2_flash_addr, part2_run_addr, part2_size, part2_checksum));
    
    return True


def get_parser():
    parser = argparse.ArgumentParser(description='PHY-62XX Generate OTA File')
    parser.add_argument('bin_file1', metavar='JUMP_TABLE', type=str, nargs=1,
                        help='JUMP_TABLE bin file')
    parser.add_argument('bin_file2', metavar='APP', type=str, nargs=1,
                        help='APP bin file')
    parser.add_argument('bin_merge', metavar='BIN_MERGE', type=str, nargs=1,
                        help='ota bin file')
    return parser


def main():
    print("Aithiner PHY-62XX OTA File Generate Tool version:" + VERSION)
    parser = get_parser()
    args = vars(parser.parse_args())

    bin_file1 = args['bin_file1'][0]
    bin_file2 = args['bin_file2'][0]
    bin_merge = args['bin_merge'][0]

    bin_success = bin_file_merge(bin_file1, bin_file2, bin_merge)
    if bin_success:
        print("Generate OTA File Success");


if __name__=='__main__':
    main()