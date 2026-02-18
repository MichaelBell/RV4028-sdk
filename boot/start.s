.section .boot,"ax"
_boot:
    li gp, 0x8000400
    la sp, __StackTop
    jal __runtime_init
    call main
    bnez a0, .         # Loop forever if main returned non zero
    li a0, 4           # Jump to 0x4
    jalr a0
