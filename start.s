.section .vectors,"a"
    j _isr_entry
    j _start

.section .early_text,"ax"
.globl _start
_start:
    li gp, 0x400
    la sp, __StackTop
    jal __runtime_init
    call main
    j .

.globl _isr_entry
_isr_entry:
    j .

