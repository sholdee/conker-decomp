#ifndef CONKER_DEBUGGER_H
#define CONKER_DEBUGGER_H

#include <ultra64.h>
#include "structs.h"
#include "libc/stdarg.h"

/*
 * Subsystem "debugger": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   debugger_256F80.c
 *   debugger_257330.c
 *   debugger_257350.c
 *   debugger/debugger.c
 *   debugger_258ED0.s
 */

/* Conversion state for one printf field (libultra _Pft): n0/n1/n2 are the lengths of the
 * prefix, digits and suffix pieces, nz0/nz1/nz2 the zero padding after each. */
typedef struct {
    union {
        long long ll;
        double d;
    } v;
    u8 *s;
    s32 n0;
    s32 nz0;
    s32 n1;
    s32 nz1;
    s32 n2;
    s32 nz2;
    s32 prec;
    s32 width;
    u32 nchar;
    u32 flags;
    u8 qual;
} DebuggerPft;

/* debugger_256F80.c */
s32 func_16001700(void);
void func_16001830(OSContPad *data);
void func_160018BC(void);
s32 func_16001984(void);
s32 func_160019A8(s32 direction, void *dramAddr);
void func_16001A64(void);
s32 func_16001A6C(f32 arg0);

/* debugger_257330.c */
void func_16001AB0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* debugger_257350.c */
void *func_16001AD0(void *dst, const void *src, u32 count);
s32 func_16001B00(u8 *str);
s32 func_16001B34(u8 *buf, const u8 *fmt, ...);
u8 *func_16001B8C(u8 *dst, const u8 *src, u32 count);
s32 func_16001BB4(u8 *(*pfn)(u8 *, const u8 *, u32), u8 *arg, const u8 *fmt, va_list args);
void func_160021FC(DebuggerPft *px, va_list *pap, u8 code, u8 *ac);
void func_1600288C(void *arg0, unsigned char code);
s16 func_16002D2C(s16 *pex, u16 *ps);
void func_16002DE4(DebuggerPft *px, u8 code, u8 *p, s16 nsig, s16 xexp);
void func_160033A8(DebuggerPft *px, u8 code);

/* debugger/debugger.c */
void func_16000000(void);
s32 func_16000028(void);
void func_16000058(void);
s32 func_16000224(void);
void func_16000304(void);
void func_1600030C(void);
void func_16000314(void);
s32 func_16000384(void);
void func_16000424(OSThread *faulted);
void func_16000590(OSThread *thread);
void func_160006CC(OSThread *thread);
void func_1600078C(void);
s32 func_16000A5C(void);
s32 func_16000B14(OSThread *thread);
void func_16000F8C(s32 position, f32 value);
void func_16001044(s32 position, s32 mode, u32 value);
void func_160012B0(s32 position, const u8 *text);
void func_16001338(u8 red, u8 green, u8 blue);
void func_16001390(s16 left, s16 top, s16 right, s16 bottom);
u8 *func_160014F0(u8 *destination, u8 character);
u8 *func_1600160C(s32 position);
void func_16001678(void);
s32 func_160016F4(s32 value);

/* debugger_258ED0.s: takes the TLB snapshot */
void func_16003650(void);

#endif
