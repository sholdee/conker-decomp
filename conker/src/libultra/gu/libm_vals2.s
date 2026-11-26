.include "macro.inc"

/* The game segment's copy of libm_vals.s (ultralib gu/libm_vals.s): the float quiet NaN. Its
   sinf and cosf return the main copy's __libm_qnan_f; nothing reads this one. */
.section .rodata

glabel D_800ABB10
    .word 0x7F810000
    .balign 16
