#!/usr/bin/env python

import sys
import struct

from uf2utils.file import UF2File
from uf2utils.constants import Flags
from uf2utils.family import Family

def checksum(prog):
    check = 0
    for i in range(len(prog)//4):
        check ^= struct.unpack("<I", prog[i*4:(i+1)*4])[0]
    return check

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: mkuf2.py input.bin output.uf2")
        sys.exit(1)

    rv4028_prog_filename = sys.argv[1]
    rv4028_prog = open(rv4028_prog_filename, 'rb').read()
    rv4028_len = len(rv4028_prog)
    payload = struct.pack("<IIII", 0x52563238, rv4028_len, checksum(rv4028_prog), 0x40284028)
    payload += rv4028_prog
    payload += bytearray(256 - (len(payload) % 256))

    uf2 = UF2File()
    uf2.header.flags = Flags.FamilyIDPresent
    uf2.header.family = Family.byName('rp2350-riscv')
    uf2.append_payload(payload, 0x10200000)
    uf2.to_file(sys.argv[2])
