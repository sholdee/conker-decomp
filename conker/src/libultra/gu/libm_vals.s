.include "macro.inc"

/* libm constants (ultralib gu/libm_vals.s): the float quiet NaN that sinf and cosf return. */
.section .rodata

glabel __libm_qnan_f
    .word 0x7F810000
    .balign 16
