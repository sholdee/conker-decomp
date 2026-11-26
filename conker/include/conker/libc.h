#ifndef CONKER_LIBC_H
#define CONKER_LIBC_H

#include <ultra64.h>
#include "structs.h"
#include "libc/stdarg.h"

/*
 * Subsystem "libc": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init/Libc/Xldtob/xldtob.c
 *   init/Libc/Xprintf/xprintf.c
 *   game/Libc/Sprintf/sprintf.c
 */

/* printf conversion state (libultra xstdio.h), shared by the printf back ends in init/Libc/Xldtob/xldtob.c (_Ldtob,
 * _Genld) and init/Libc/Xprintf/xprintf.c (_Litob, the argument fetcher). IDO has no long double, so ld is a double. qual is
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

/* init/Libc/Xldtob/xldtob.c */
void func_10001420(void);
void func_10001444(void);
void func_100014A0(void);
void func_100014C4(s32 arg0);
void func_10001550(void *arg0, unsigned char code);
s16 func_100019F0(s16 *pex, _Pft *px);
void func_10001AA8(_Pft *px, unsigned char code, unsigned char *p, short nsig, short xexp);

/* init/Libc/Xprintf/xprintf.c */
u8 *func_10002070(u8 *arg, u8 *buf, u32 count);
void func_10002088(const char *fmt, ...); /* osSyncPrintf, as os_libc.h declares it */
/* _Printf: the libultra formatter core */
int func_100020D0(u8 *(*pfn)(u8 *, u8 *, u32), u8 *arg, u8 *fmt, va_list args);
void func_10002718(_Pft *px, va_list *pap, unsigned char code, unsigned char *ac);

/* game/Libc/Sprintf/sprintf.c */
u8 *func_151EFF70(u8 *dst, u8 *src, u32 count);
s32 func_151EFF94(u8 *buf, u8 *fmt, ...);

#endif
