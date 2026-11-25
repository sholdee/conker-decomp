#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Extra bits func_16000B14 ORs into the held-button mask for the analog stick. */
#define STICK_LEFT  0x10000
#define STICK_RIGHT 0x20000
#define STICK_UP    0x40000
#define STICK_DOWN  0x80000

/* Result bits of a screen's input handler (see func_16000B14's loops). */
#define DEBUGGER_REDRAW 1 /* draw the current screen again */
#define DEBUGGER_CLEAR  2 /* clear the framebuffer before drawing */
#define DEBUGGER_EXIT   4 /* leave the debugger */

/* Screens (D_16003AF4) indexing the draw (D_16003AF8) and input (D_16003B08) tables. */
#define SCREEN_PRESS_BUTTON 0
#define SCREEN_MAIN_MENU    1
#define SCREEN_REGISTERS    2
#define SCREEN_STACK        3

/* Two-character register name, a space and a terminator, filled in from D_160037F0. */
typedef struct {
    u8 text[4];
} DebuggerLabel;

extern u8 D_160037F0[];
extern DebuggerLabel D_16003B48;

void func_160006CC(OSThread *thread);
void func_16000590(OSThread *thread);
void func_16001044(s32 position, s32 mode, u32 value);

/* SCREEN_PRESS_BUTTON: draw "PRESS BUTTON". */
void func_16000000(void) {
    func_160012B0(278, D_160046AC);
}

/* SCREEN_PRESS_BUTTON input: A or B opens the main menu. */
s32 func_16000028(void) {
    if (D_16003890 & (A_BUTTON | B_BUTTON)) {
        D_16003AF4 = SCREEN_MAIN_MENU;
        return DEBUGGER_REDRAW | DEBUGGER_CLEAR;
    }
    return 0;
}

/* SCREEN_MAIN_MENU: draw the menu with the selected entry (D_16003B1C) in red, then the build info. */
void func_16000058(void) {
    s32 i;
    s32 position;

    func_16001338(255, 255, 255);
    func_160012B0(44, D_160046D0); // "MAIN MENU"
    position = 108;
    for (i = 0; i < 2; i++) {
        if (D_16003B1C == i) {
            func_16001338(255, 0, 0);
        } else {
            func_16001338(255, 255, 255);
        }
        func_160012B0(position, (u8 *)D_16003B20[i]); // "REGISTERS", "STACK"
        position += 32;
    }
    if (D_16003B1C == 2) {
        func_16001338(255, 0, 0);
    } else {
        func_16001338(255, 255, 255);
    }
    if (D_16003AF0 != 0) {
        if ((D_1600389C->unk120 == EXC_SYSCALL) && (D_160038A4 == 0)) {
            func_160012B0(position, D_160046DC); // "CONTINUE"
        } else if (D_16003AF0 != 0) {
            func_160012B0(position, D_160046E8); // "HOSTDEBUG"
        }
    } else {
        func_160012B0(position, D_160046F4); // "RETRY CODE"
    }
    func_16001338(255, 255, 255);
    func_160012B0(611, D_16004700); // "Version"
    func_16001044(619, 1, 163);
    func_160012B0(643, D_16004708); // "Built"
    func_160012B0(651, D_16004710); // "Dec 19 2000"
    func_160012B0(663, D_1600471C); // "09:57:42"
}

/* SCREEN_MAIN_MENU input: stick up/down moves the selection, A picks it. */
s32 func_16000224(void) {
    s32 result = 0;

    if (D_16003890 & STICK_UP) {
        D_16003B1C--;
        result = DEBUGGER_REDRAW;
    }
    if (D_16003890 & STICK_DOWN) {
        D_16003B1C++;
        result = DEBUGGER_REDRAW;
    }
    if (D_16003B1C >= 3) {
        D_16003B1C = 0;
    }
    if (D_16003B1C < 0) {
        D_16003B1C = 2;
    }
    if (D_16003890 & A_BUTTON) {
        switch (D_16003B1C) {
            case 0:
                result = DEBUGGER_REDRAW | DEBUGGER_CLEAR;
                D_16003AF4 = SCREEN_REGISTERS;
                break;
            case 1:
                result = DEBUGGER_REDRAW | DEBUGGER_CLEAR;
                D_16003AF4 = SCREEN_STACK;
                break;
            case 2:
                result = DEBUGGER_EXIT;
                break;
        }
    }
    return result;
}

void func_16000304(void) {
}

void func_1600030C(void) {
}

/* SCREEN_REGISTERS: page 0 shows the CPU registers, pages 1 and 2 the FPU registers. */
void func_16000314(void) {
    switch (D_16003B28) {
        case 0:
            func_16000424(D_1600389C);
            func_160006CC((OSThread *)D_1600389C);
            break;
        case 1:
        case 2:
            func_16000590((OSThread *)D_1600389C);
            break;
    }
}

/* SCREEN_REGISTERS input: C buttons flip through the pages, B returns to the main menu. */
s32 func_16000384(void) {
    s32 result = 0;

    if (D_16003890 & (R_CBUTTONS | D_CBUTTONS)) {
        D_16003B28++;
        if (D_16003B28 >= 3) {
            D_16003B28 = 0;
        }
        return DEBUGGER_REDRAW | DEBUGGER_CLEAR;
    }
    if (D_16003890 & (L_CBUTTONS | U_CBUTTONS)) {
        D_16003B28--;
        if (D_16003B28 < 0) {
            D_16003B28 = 2;
        }
        return DEBUGGER_REDRAW | DEBUGGER_CLEAR;
    }
    if (D_16003890 & B_BUTTON) {
        D_16003AF4 = SCREEN_MAIN_MENU;
        result = DEBUGGER_REDRAW | DEBUGGER_CLEAR;
    }
    return result;
}

/* Register page 0, top half: pc, cause (with its exception name), sr, badvaddr and thread id. */
void func_16000424(struct118 *arg0) {
    u32 cause;
    u32 exception;
    OSThread *thread = (OSThread *)arg0;

    func_16001338(255, 255, 255);
    func_160012B0(35, D_16004728); // "Pc"
    func_16001044(43, 0, thread->context.pc);
    func_160012B0(67, D_1600472C); // "Cause"
    func_16001044(75, 0, thread->context.cause);
    func_16001338(128, 128, 255);
    cause = thread->context.cause;
    exception = (cause >> 2) & 0xF;

    func_160012B0(107, (u8 *)D_16003848[exception]);
    if (exception == 11) { // coprocessor unusable: show which one
        func_16001044(111, 1, (cause >> 28) & 3);
    }
    func_16001338(255, 255, 255);
    func_160012B0(131, D_16004734); // "Sr"
    func_16001044(139, 0, thread->context.sr);
    func_160012B0(163, D_16004738); // "Badvadr"
    func_16001044(171, 0, thread->context.badvaddr);
    func_160012B0(195, D_16004740); // "Thread"
    func_16001044(203, 1, thread->id);
    if (D_160038A4 != 0) {
        func_160012B0(52, D_16004748); // "Lockup_Now"
    }
}

extern u8 *D_16003B30[6];
extern u8 D_160047A4[];
extern u8 D_160047AC[];

/* Register pages 1 and 2: the FPCSR with its raised exception causes, then 16 FPU registers. */
void func_16000590(OSThread *thread) {
    s32 position;
    s32 i;
    s32 word;
    u32 bits;
    u32 page = 0;
    u32 *words = (u32 *)thread;

    bits = thread->context.fpcsr;
    func_160012B0(3, D_160047A4); // "Fpcsr:"
    func_16001044(10, 0, bits);
    bits >>= 12; // cause bits: inexact, underflow, overflow, div by zero, invalid, unimplemented
    position = 44;
    for (i = 0; i < 6; i++, bits >>= 1) {
        if (bits & 1) {
            func_160012B0(position, D_16003B30[i]);
            position += 32;
        }
    }
    i = 0;
    position = 195;
    if (D_16003B28 == 1) {
        word = 0x4C; // context.fp0
    } else {
        word = 0x6C; // 16 registers past context.fp0
        page = 16;
    }
    do {
        func_160012B0(position, D_160047AC); // "fp"
        func_16001044(position + 2, 1, i + page);
        func_16001044(position + 5, 2, words[word + 1]); // f_even, the register's low word
        word += 2;
        position += 32;
        i++;
    } while (i < 16);
}

/*
 * Register page 0, bottom half: each D_160037F0 entry is a two-character name followed by
 * the u32 index of the register's (u64) slot in the OSThread; a zero name ends the table.
 */
void func_160006CC(OSThread *thread) {
    DebuggerLabel label = D_16003B48;
    s32 position = 291;
    u8 *entry = D_160037F0;
    s32 index;

    func_16001338(192, 192, 255);
    do {
        label.text[0] = entry[0];
        label.text[1] = entry[1];
        func_160012B0(position, label.text);
        position += 3;
        index = entry[2];
        func_16001044(position, 0, ((u32 *)thread)[index + 1]); // low word
        position += 13;
        entry += 3;
    } while (entry[0] != 0);
}
void func_160012B0(s32 arg0, u8 *arg1);
void func_16001338(u8 arg0, u8 arg1, u8 arg2);
extern s32 D_8002D4B0;
extern s32 *D_8002D8B0;
extern s32 D_16003B4C;
extern struct118 *D_1600389C;
extern u8 D_160047B0[];
extern u8 D_160047BC[];
extern u8 D_160047C0[];
void func_1600078C(void)
{
  s32 temp_s0;
  s32 temp_s0_2;
  s32 y;
  s32 i;
  u32 *addr;
  u32 word;
  u32 dataStart;
  u32 prefix;
  temp_s0 = ((s32 *) D_1600389C)[0x3D];
  func_16001338(0, 255, 0);
  func_160012B0(0xB, D_160047B0);
  addr = (u32 *) (temp_s0 + (D_16003B4C * 4));
  if ((((((((u32) addr) & 3) & 0xFFFFu) & 0xFFFFu) == 0) && (((u32) addr) >= 0x80000000U)) && (((u32) addr) < 0x80800001U))
  {
    dataStart = (u32) (&D_8002D4B0);
    if (D_16003B4C == 0)
    {
      func_16001338(255, 0, 0);
    }
    else
      if ((((u32) addr) >= dataStart) && (((u32) addr) < (dataStart + 0x400)))
    {
      func_16001338(128, 128, 255);
    }
    else
      if ((((u32) addr) >= ((u32) (&D_8002D8B0))) && (((u32) addr) < (((u32) (&D_8002D8B0)) + 0x4000)))
    {
      func_16001338(255, 128, 128);
    }
    else
    {
      func_16001338(255, 255, 255);
    }
    y = 0x61;
    i = 0;
    do
    {
      func_16001044(y, 0, addr);
      func_160012B0(y + 8, D_160047BC);
      word = *addr;
      prefix = (word >> 24) & 0xFF;
      if (prefix == 0x80)
      {
        func_16001338(128, 128, 255);
      }
      else
        if (prefix == 0x15)
      {
        func_16001338(255, 0, 0);
      }
      else
        if (prefix == 0x16)
      {
        func_16001338(128, 255, 128);
      }
      else
        if (prefix == 0x10)
      {
        func_16001338(255, 0, 0);
      }
      else
      {
        func_16001338(255, 255, 255);
      }
      func_16001044(y + 0xC, 0, word);
      temp_s0_2 = y + 0x16;
      func_160012B0(temp_s0_2, D_160047C0);
      func_16001338(255, 255, 255);
      func_16001044(temp_s0_2, 1, word);
      if ((((u32) addr) >= dataStart) && (((u32) addr) < (dataStart + 0x400)))
      {
        func_16001338(128, 128, 255);
      }
      else
        if ((((u32) addr) >= ((u32) (&D_8002D8B0))) && (((u32) addr) < (((u32) (&D_8002D8B0)) + 0x4000)))
      {
        func_16001338(255, 128, 128);
      }
      else
      {
        func_16001338(255, 255, 255);
      }
      i += 1;
      y += 0x20;
      addr += 1;
    }
    while (i != 0x16);
  }
}

void func_160012B0(s32 arg0, u8 *arg1);
void func_16001338(u8 arg0, u8 arg1, u8 arg2);

// NON-MATCHING: close but still some stuff to figure out
// void func_1600078C(void) {
//     s32 temp_s0;
//     u8 range_prefix;
//     u32 temp_s2;
//     s32 temp_s5;
//     s32 phi_s1;
//     u32 *phi_s2;
//     u32 phi_s5;
//     s32 i;
//
//     temp_s0 = D_1600389C->unkF4;
//     func_16001338(0, 255, 0); // green
//     func_160012B0(11, &D_160047B0);
//     temp_s2 = (D_16003B4C * 4) + temp_s0;
//     if (((temp_s2 & 3) == 0) && (temp_s2 >= 0x80000000U) && (temp_s2 < 0x80800001U)) {
//         if (D_16003B4C == 0) {
//             func_16001338(255, 0, 0); // red
//             phi_s5 = &D_8002D4B0;
//         } else {
//             temp_s5 = &D_8002D4B0 - 0x2B50; // 11088 ?
//             if ((temp_s2 >= temp_s5) && (temp_s2 < (temp_s5 + 0x400))) {
//                 func_16001338(128, 128, 255); // purple
//                 phi_s5 = temp_s5;
//             } else if ((temp_s2 >= (u32) &D_8002D8B0) && (temp_s2 < ((u32)&D_8002D8B0 + 0x4000))) {
//                 func_16001338(255, 128, 128); // light red
//                 phi_s5 = temp_s5;
//             } else {
//                 func_16001338(255, 255, 255);
//                 phi_s5 = temp_s5;
//             }
//         }
//         phi_s1 = 97; // "a"
//         phi_s2 = temp_s2;
//         for (i = 0; i < 22; i++) {
//             func_16001044(phi_s1, 0, phi_s2);
//             func_160012B0(phi_s1 + 8, &D_160047BC);
//             range_prefix = (*phi_s2 >> 24);
//             if (range_prefix == 0x80) {                   // 0x80000000
//                 func_16001338(128, 128, 255);
//             } else if (range_prefix == 0x15) {            // 0x15000000
//                 func_16001338(255, 0, 0);
//             } else if (range_prefix == 0x16) {            // 0x16000000
//                 func_16001338(128, 255, 128);
//             } else if (range_prefix == 0x10) {            // 0x10000000
//                 func_16001338(255, 0, 0);
//             } else {
//                 func_16001338(255, 255, 255);
//             }
//             func_16001044(phi_s1 + 0xC, 0, *phi_s2);
//             func_160012B0(phi_s1 + 0x16, &D_160047C0);
//             func_16001338(255, 255, 255);
//             func_16001044(phi_s1 + 0x16, 1, *phi_s2);
//             if (((u32) phi_s2 >= phi_s5) && ((u32) phi_s2 < (phi_s5 + 0x400))) {
//                 func_16001338(128, 128, 0xFF);
//             } else if (((u32) phi_s2 >= (u32) &D_8002D8B0) && ((u32) phi_s2 < ((u32)&D_8002D8B0 + 0x4000))) {
//                 func_16001338(255, 128, 128);
//             } else {
//                 func_16001338(255, 255, 255);
//             }
//             phi_s1 += 0x20;
//             phi_s2 += 4;
//         }
//     }
// }

s32 func_16000A5C(void) {
    s32 res = 0;

    if ((D_160036F3 >= 41) || (D_16003890 & U_CBUTTONS)) {
        D_16003B4C -= 1;
        if (D_16003B4C < 0) {
            D_16003B4C = 0;
        } else {
            res = 3;
        }
    } else if ((D_160036F3 < -40) || (D_16003890 & D_CBUTTONS)) {
        D_16003B4C += 1;
        if (D_16003B4C >= 201) {
            D_16003B4C = 200;
        }
        res = 3;
    }
    if ((D_16003890 & B_BUTTON)) {
        D_16003AF4 = 1;
        res = 3;
    }
    return res;
}

// called from func_10007DAC
typedef struct {
    u16 state;
    u16 retraceCount;
    void *framep;
} DebuggerViContext;
extern DebuggerViContext *D_8002BDE0;
extern u32 D_8003C8E8[4];
extern u32 D_160038AC[32];
extern u32 D_1600392C[32];
extern u32 D_160039AC[32];
extern u32 D_16003A2C[32];
extern u32 D_160039E8;
extern u32 D_16003A68;
extern s32 D_16003894;
extern s32 D_16003898;
extern void (*D_16003AF8[4])(void);
extern s32 (*D_16003B08[4])(void);
extern OSContPad D_160036F0[4];
void func_16003650(void);
void func_16001678(void);
s32 func_16001700(void);
void func_16001830(OSContPad *data);

s32 func_16000B14(struct OSThread_s *thread) {
    u32 *context;
    u32 *frame;
    u32 entryLo;
    s32 first;
    s32 state;
    s32 stick;
    u32 pc;
    u32 page;
    s32 odd;
    u32 flags;
    s32 offset;
    s32 *entry;
    u32 *saved;
    void (*draw)(void);
    s32 (*input)(void);

    state = 0;
    first = 1;
    if (D_8002AC5C != 0) {
        return 0;
    }
    D_16003888 = 0;
    if ((D_8002AAE8[0] == 0) || (D_8002AAE8[1] == 0)) {
        D_8002AAE8[0] = 0x80350000;
        D_8002AAE8[1] = 0x80350000;
        return 0;
    }
    func_16003650();
    context = D_8003C8E8;
    saved = context;
    D_160038AC[15] = saved[0];
    D_1600392C[15] = saved[1];
    D_160039E8 = saved[2];
    D_16003A68 = saved[3];
    if ((((u32 *)thread)[0x11C / 4] & 0xFF000000) != 0x15000000) {
        D_16003AF0 = 1;
    } else {
        page = ((u32 *)thread)[0x11C / 4] & ~0xFFF;
        odd = page & 0x1000;
        pc = page & ~0x1000;
        page = pc;
        D_16003AF0 = 0;
        for (offset = 0; offset < 32; offset++) {
            if (page == D_160039AC[offset]) {
                entryLo = odd ? D_1600392C[offset] : D_160038AC[offset];
                flags = entryLo;
                if (flags & 2) {
                    D_16003AF0 = 1;
                }
            }
        }
    }
    if ((((u32)D_8003C8E0 >> 24) & 0xFF) == 0xC) {
        thread = &D_80031AE0;
    }
    frame = (u32 *)D_8002BDE0->framep;
    if ((saved = frame) == (u32 *)D_8002AAE8[1]) {
        *(s8 *)&D_16003888 = 1;
    }
    D_1600389C = (struct118 *)thread;
    D_160038A4 = 0;
    if ((((s32 *)thread)[0x120 / 4] == 0x20) &&
        ((u32)func_150AD770 == ((u32 *)thread)[0x11C / 4])) {
        *(s8 *)&D_160038A4 = 1;
    }
    do {
        if ((first == 0) && (state & 2)) {
            func_16001678();
        }
        draw = D_16003AF8[D_16003AF4];
        if (draw != 0) {
            draw();
        }
        osWritebackDCacheAll();
        do {
            state = 0;
            D_16003898 = D_16003894;
            func_16001700();
            func_16001830(D_160036F0);
            if (first != 0) {
                D_16003898 = D_16003894;
            }
            D_16003894 = D_160036F0[0].button;
            stick = D_160036F0[0].stick_x;
            if (stick >= 0x33) {
                D_16003894 |= 0x20000;
            }
            if (stick < -0x32) {
                D_16003894 |= 0x10000;
            }
            stick = D_160036F0[0].stick_y;
            if (stick >= 0x33) {
                D_16003894 |= 0x40000;
            }
            if (stick < -0x32) {
                D_16003894 |= 0x80000;
            }
            D_16003890 = (D_16003894 ^ D_16003898) & D_16003894;
            input = D_16003B08[D_16003AF4];
            if (input != 0) {
                state = input();
            }
        } while (!(state & 5));
        first = 0;
    } while (!(state & 4));
    if (D_16003AF0 == 0) {
        ((s16 *)thread)[0x10 / 2] = 4;
        ((s16 *)thread)[0x12 / 2] = 0;
        return 1;
    }
    if ((((s32 *)thread)[0x120 / 4] == 0x20) && (D_160038A4 == 0)) {
        ((s32 *)thread)[0x11C / 4] += 4;
        return 1;
    }
    return 0;
}
s32 func_16001B34(u8 *buffer, const u8 *format, ...);

void func_16000F8C(s32 position, f32 value) {
    f32 copy;
    u8 buffer[44];
    u32 exponent;

    if ((position >= (D_160038A0 << 5)) && (position < 0x341)) {
        copy = value;
        exponent = (u32)(*(s32 *)&copy & 0x7F800000) >> 23;
        if (((exponent == 0) || (exponent >= 0xFF)) &&
            ((exponent = *(s32 *)&copy * 2) != 0)) {
            func_160012B0(position, D_160047D0);
            return;
        }
        func_16001B34(buffer, D_160047D4, D_160047DC, D_160047E0,
                      (f64)value);
        func_160012B0(position, buffer);
    }
}
// NON-MATCHING: lots to figure out
// void func_16000F8C(s32 arg0, f32 arg1) {
//     struct165 tmp;
//     s32 temp_v1;
//     u32 temp_t9;
//
//     if ((arg0 >= (D_160038A0 << 5)) && (arg0 < 833)) {
//         tmp.unk18 = arg1;
//         temp_v1 = *(s32*)&tmp.unk18;
//         temp_t9 = (u32) (temp_v1 & 0x7F800000) >> 0x17;
//         if ((temp_t9 == 0) || (temp_t9 >= 0x255U)) {
//             if ((temp_v1 * 2) != 0) {
//                 func_160012B0(arg1, &D_160047D0); // arg0,
//                 return;
//             }
//         }
//         func_16001B34(&arg0, &tmp.unk0, &D_160047D4, &D_160047DC, &D_160047E0); // , (f64) arg1
//         func_160012B0(arg0, &tmp.unk0);
//     }
// }

u8 *func_1600160C();
u8 *func_160014F0(u8 *destination, u8 character);
s32 func_16001B34(u8 *buffer, const u8 *format, ...);

typedef struct DebuggerDecimalPowers {
    s32 power[10];
} DebuggerDecimalPowers;
extern DebuggerDecimalPowers D_16003B50;
extern const u8 D_160047E4[];
extern const u8 D_160047E8[];
extern const u8 D_160047F0[];
extern const u8 D_160047F4[];

void func_16001044(s32 position, s32 mode, u32 value) {
    s32 index;
    s32 started;
    s32 decimal_index;
    s32 digit;
    s32 exponent;
    u8 character;
    DebuggerDecimalPowers powers;
    s32 power;
    u32 mantissa;
    f32 copy;
    u8 buffer[36];
    u8 *destination;

    powers = D_16003B50;
    if ((position >= (D_160038A0 << 5)) && (position < 0x341)) {
        destination = func_1600160C(position);
        switch (mode) {
        case 0:
            destination += 0x70;
            index = 0;
            do {
                character = value & 0xF;
                if (character >= 10) {
                    character += 7;
                }
                character += '0';
                func_160014F0(destination, character);
                value = (s32)value >> 4;
                index++;
                destination -= 0x10;
            } while (index != 8);
            break;
        case 1:
            if ((s32)value < 0) {
                destination = func_160014F0(destination, '-');
                value = -value;
            }
            started = 0;
            for (decimal_index = 9; decimal_index >= 0; decimal_index--) {
                power = powers.power[decimal_index];
                digit = (s32)value / power;
                value = (s32)value % power;
                if ((digit > 0) || started || (decimal_index == 0)) {
                    destination = func_160014F0(destination, digit + '0');
                    started = 1;
                }
            }
            break;
        case 2:
            exponent = (s32)(value & 0x7F800000) >> 23;
            mantissa = value << 9;
            if (((exponent <= 0) || (exponent >= 0xFF)) &&
                ((exponent != 0) || mantissa)) {
                func_160012B0(position, D_160047E4);
                return;
            }
            *(s32 *)&copy = value;
            func_16001B34(buffer, D_160047E8, D_160047F0, D_160047F4,
                          (f64)copy);
            func_160012B0(position, buffer);
            break;
        }
    }
}

void func_160012B0(s32 arg0, u8 *arg1) {
    if (arg1 && (arg0 >= (D_160038A0 << 5)) && (arg0 < 833)) {
        s32 fb = func_1600160C();
        while (*arg1 != 0) {
            fb = func_160014F0(fb, *arg1);
            *arg1++;
        }
    }
}
// convert rgb to rgba16
void func_16001338(u8 arg0, u8 arg1, u8 arg2) {
    D_1600388C = ((arg0 & 0xF8) << 8) | ((arg1 & 0xF8) << 3) | ((arg2 & 0xF8) >> 2) | 1;
}

extern u16 D_1600388C;
extern s32 D_160038A8;
void func_16001390(s16 arg0, s16 arg1, register s16 arg2, s16 arg3)
{
  s32 count;
  u16 *ptr;
  if (arg2 < arg0)
  {
    return;
  }
  if (arg3 < arg1)
  {
    return;
  }
  if (arg0 < 0)
  {
    return;
  }
  if (arg1 < 0)
  {
    return;
  }
  arg2++;
  arg3++;
  ptr = (u16 *) func_1600160C(0);
  ptr += arg0 + (arg1 * D_160038A8);
  arg2 -= arg0;
  arg3 -= arg1;
  if (arg3 > 0)
  {
    do
    {
      count = arg2;
      if (arg2 > 0)
      {
        do
        {
          *(ptr++) = D_1600388C;
          count--;
        }
        while (count != 0);
      }
      arg3--;
      ptr += D_160038A8 - arg2;
    }
    while (arg3 > 0);
  }
}

extern const u8 D_16003CE0[];

u8 *func_160014F0(u8 *destination, u8 character) {
    u16 *pixel;
    u16 color;
    u32 column;
    u16 bits;
    u32 row;
    const u8 *glyph;
    s32 code;
    u16 value;

    pixel = (u16 *)destination;
    color = D_1600388C;
    column = character;
    bits = column;
    code = bits;
    if (code < 0x20) {
        code = 0x20;
    }
    row = 0, glyph = D_16003CE0 + ((code - 0x20) << 3);
    for (; row < 8; row++, glyph++) {
        for (column = 0, bits = *glyph; column < 8; column++) {
            value = (bits & 0x80) ? color : 1;
            bits <<= 1;
            *pixel++ = value;
        }
        pixel += D_160038A8 - 8;
    }
    return destination + 0x10;
}

// splat into framebuffer
u8 *func_1600160C(s32 position) {
    s32 row = position & 0xFFE0;
    s32 offset = row;

    if (D_160038A8 != 0x124) {
        offset = (row >> 2) + row;
    }
    offset = (offset >> 2) * (D_160038A8 << 1);
    offset += (position & 0x1F) << 4;
    offset += D_160038A8 << 2;
    offset += 0x10;
    return (u8 *)D_8002AAE8[D_16003888] + offset;
}

// contains delay slot
void func_16001678(void) {
    u32 *ptr;
    u32 *end;
    s32 height;
    s32 width;

    ptr = (u32 *) D_8002AAE8[D_16003888];
    height = D_160038A8;
    if (height == 0x124) {
        width = 0xD7;
    } else {
        width = 0x108;
    }

    end = ptr + ((height >> 1) * width);
    while (ptr < end) {
        ptr += 4;
        ptr[-4] = 0x10001;
        ptr[-3] = 0x10001;
        ptr[-2] = 0x10001;
        ptr[-1] = 0x10001;
    }
}

s32 func_160016F4(s32 arg0) {
    return arg0;
}
