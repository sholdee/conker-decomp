#ifndef CONKER_LIBC_H
#define CONKER_LIBC_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "libc": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init_1420.c
 *   init_2070.c
 *   game_21D420.c
 */

/* printf conversion state (libultra xstdio.h), shared by the printf back ends in init_1420.c (_Ldtob,
 * _Genld) and init_2070.c (_Litob, the argument fetcher). IDO has no long double, so ld is a double. qual is
 * unsigned char: libultra's char is unsigned, but the game is built with -signed. */
typedef struct {
    /* 0x00 */ union {
        /* 0x00 */ long long ll;
        /* 0x00 */ double ld;
    } v;
    /* 0x08 */ unsigned char *s;
    /* 0x0C */ int n0;
    /* 0x10 */ int nz0;
    /* 0x14 */ int n1;
    /* 0x18 */ int nz1;
    /* 0x1C */ int n2;
    /* 0x20 */ int nz2;
    /* 0x24 */ int prec;
    /* 0x28 */ int width;
    /* 0x2C */ unsigned int nchar;
    /* 0x30 */ unsigned int flags;
    /* 0x34 */ unsigned char qual;
} _Pft; /* size 0x38 */

#endif
