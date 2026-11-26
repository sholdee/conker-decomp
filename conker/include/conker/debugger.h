#ifndef CONKER_DEBUGGER_H
#define CONKER_DEBUGGER_H

#include <ultra64.h>
#include "structs.h"
#include "libc/stdarg.h"

/*
 * Subsystem "debugger": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   debugger/Debugger/DebuggerCont/debuggercont.c
 *   debugger/Debugger/DebuggerStub/debuggerstub.c
 *   debugger/Debugger/DebuggerPrintf/debuggerprintf.c
 *   debugger/Debugger/CrashScreen/crashscreen.c
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

/* debugger/Debugger/DebuggerCont/debuggercont.c */
s32 debugger_contStartReadData(void);
void debugger_contGetReadData(OSContPad *data);
void debugger_packReadData(void);
s32 debugger_siDeviceBusy(void);
s32 debugger_siRawStartDma(s32 direction, void *dramAddr);
void func_16001A64(void);
s32 debugger_isBadFloat(f32 arg0);

/* debugger/Debugger/DebuggerStub/debuggerstub.c */
void func_16001AB0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* debugger/Debugger/DebuggerPrintf/debuggerprintf.c */
void *debugger_memcpy(void *dst, const void *src, u32 count);
s32 debugger_strlen(u8 *str);
s32 debugger_sprintf(u8 *buf, const u8 *fmt, ...);
u8 *debugger_sprintfOut(u8 *dst, const u8 *src, u32 count);
s32 debugger_xprintf(u8 *(*pfn)(u8 *, const u8 *, u32), u8 *arg, const u8 *fmt, va_list args);
void debugger_putfld(DebuggerPft *px, va_list *pap, u8 code, u8 *ac);
void debugger_ldtob(void *arg0, unsigned char code);
s16 debugger_ldunscale(s16 *pex, u16 *ps);
void debugger_genld(DebuggerPft *px, u8 code, u8 *p, s16 nsig, s16 xexp);
void debugger_litob(DebuggerPft *px, u8 code);

/* debugger/Debugger/CrashScreen/crashscreen.c */
void debugger_drawPrompt(void);
s32 debugger_inputPrompt(void);
void debugger_drawMainMenu(void);
s32 debugger_inputMainMenu(void);
void func_16000304(void);
void func_1600030C(void);
void debugger_drawRegisters(void);
s32 debugger_inputRegisters(void);
void debugger_drawCpuStatus(OSThread *faulted);
void debugger_drawFpuRegisters(OSThread *thread);
void debugger_drawCpuRegisters(OSThread *thread);
void debugger_drawStack(void);
s32 debugger_inputStack(void);
s32 debugger_run(OSThread *thread);
void debugger_printFloat(s32 position, f32 value);
void debugger_printValue(s32 position, s32 mode, u32 value);
void debugger_printText(s32 position, const char *text);
void debugger_setTextColor(u8 red, u8 green, u8 blue);
void debugger_fillRect(s16 left, s16 top, s16 right, s16 bottom);
u8 *debugger_drawGlyph(u8 *destination, u8 character);
u8 *debugger_getTextAddr(s32 position);
void debugger_clearScreen(void);
s32 debugger_returnArg(s32 value);
extern u32 gTlbEntryLo0[32]; /* the TLB snapshot of debugger_readTlb: EntryLo0, EntryLo1, EntryHi, PageMask */
extern u32 gTlbEntryLo1[32];
extern u32 gTlbEntryHi[32];
extern u32 gTlbPageMask[32];

/* debugger_258ED0.s: takes the TLB snapshot */
void debugger_readTlb(void);

#endif
