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

/* func_16001044 modes */
#define PRINT_HEX   0
#define PRINT_DEC   1
#define PRINT_FLOAT 2

/* Screens (D_16003AF4) indexing the draw (D_16003AF8) and input (D_16003B08) tables. */
#define SCREEN_PRESS_BUTTON 0
#define SCREEN_MAIN_MENU    1
#define SCREEN_REGISTERS    2
#define SCREEN_STACK        3

/* Two-character register name, a space and a terminator, filled in from D_160037F0. */
typedef struct {
    char text[4];
} DebuggerLabel;

extern const u8 D_16003CE0[]; // 8x8 font from ' ' (an asset: it stays extracted from the ROM)

/* libultra's __OSViContext; only framep is used here. */
typedef struct {
    u16 state;
    u16 retraceCount;
    void *framep;
} DebuggerViContext;
extern DebuggerViContext *__osViCurr;

/* TLB entry 15 as it was when the exception hit: EntryLo0, EntryLo1, EntryHi, PageMask. */
extern u32 D_8003C8E8[4];

typedef struct DebuggerDecimalPowers {
    s32 power[10];
} DebuggerDecimalPowers;

/* .data, linked at 0x160036F0..0x16003C70 in definition order (with the local initializers of
 * func_160006CC and func_16001044). The debugger has no .bss: its variables are all initialized. */
static OSContPad D_160036F0[4] = { 0 }; /* read by func_16001830 */

/* Lockup reasons (not referenced) */
static char *D_16003708[] = {
    "Non",
    "UPDATE_CHARS",
    "DRAW_CHARS",
    "UPDATE_DBOBJ",
    "DRAW_DBOBJ",
    "TASK",
    "Task Exec",
    "Effects",
    "Update_frontend",
    "Draw_frontend",
    "Main-UpdateOncePerView",
    "Gfx-lockup",
    "Maincode-lockup",
    "Deliberate Lockup (Tm)",
    "Scheduler",
    "Audio timer",
    "SCHEDULER_CPU_LOCKUP",
    "NOT_ENOUGH_CUTSCENE_BITS",
    "DODOWNLOAD_DATA_BIGGER_THAN_MEMORY_BUFFER",
    "DODOWNLOAD_COMPRESSED_DATA_BIGGER_THAN_MEMORY_BUFFER",
    "DODOWNLOAD_SIZE_MISMATCH",
    "ZERO_SIZE_LEVEL_INFO_DOWNLOAD",
    "DB_OBJECT_INIT_NOT_ENOUGH_SPACE",
    "NOT_ENOUGH_MEMORY_FOR_VISIBLE_TRI",
    "MOVE_THINGY_ON_OBJECT",
    "OUT_OF_MEMORY_DOWNLOADING_CHARACTER_TO_HEAP",
    "OUT_OF_MEMORY_DOWNLOADING_ANIMATION_TABLES",
    "OUT_OF_MEMORY_DOWNLOADING_CHARACTER_INFO_TABLES",
    "LIGHTING_HARDWARE_TOO_MANY_LIGHTS",
    "LIGHTING_HARDWARE_WRONG_NUMBER_OF_LIGHTS",
    "TOO_MANY_CHARACTERS_TO_DECOMPRESS",
    "LAST_DISPLAY_LIST_NOT_FREED",
    "HEAP_RAN_OUT_OF_MEMORY",
    "HEAP_BLOCK_ERROR",
    "HEAP_SIZE_WRONG",
    "HEAP_ERROR",
    "INVALID_TEXTURE_NUMBER",
    "TASK_OUT_OF_GLIST",
    "TASK_OUT_OF_SAFETY_BUFFER_GLIST",
    "TASK_TOO_MANY_COMMANDS_FOR_GLIST_SAFETY_BUFFER",
    "DB_HITPOLY_INIT INC MAX TRI SPACE",
    "DB_HITPOLY_INIT INC MAX TREE SPACE",
    "HITS.C OUT OF RANGE APD",
    "HEAP_LOW_PRI_TOO_MUCH",
    "SC STACK OVERFLOW",
    "WETCHARACTER MATRIX INDEX ORA",
    "CHARACTERNODE MATRIX INDEX ORA",
    "CHARACTER FIRE CONKER MATRIX",
    "ABI LIST ERROR (see Mike)",
    "AUDIO CACHE DMA NOT DONE (Mike)",
    "AUDIO CACHE DMA START FAILED (Mike)",
    "EFFECTS STACK OVERFLOW (Andy)",
    "EFFECTS STACK UNDERFLOW (Andy)",
    "AUDIO STACK OVERFLOW (Mike)",
    "MALLOC FREE MISMATCH",
    "CHAREFFECT CORRUPTION",
    "GFX LIST CHECK (Andy)",
    NULL,
};

/* Register page 0: each entry is a two-character name and the u32 index of the register's (u64)
 * slot in the OSThread; a zero name ends the table. */
static u8 D_160037F0[] = {
    'v', '0', 10, 'v', '1', 12, 'a', '0', 14, 'a', '1', 16, 'a', '2', 18, 'a', '3', 20,
    't', '0', 22, 't', '1', 24, 't', '2', 26, 't', '3', 28, 't', '4', 30, 't', '5', 32,
    't', '6', 34, 't', '7', 36, 't', '8', 54, 't', '9', 56, 's', '0', 38, 's', '1', 40,
    's', '2', 42, 's', '3', 44, 's', '4', 46, 's', '5', 48, 's', '6', 50, 's', '7', 52,
    's', '8', 62, 's', 'p', 60, 'g', 'p', 58, 'r', 'a', 64, 0,
};

/* Exception names, by the ExcCode field of the cause register */
static char *D_16003848[16] = {
    "Interupt",
    "TLB Modification",
    "TLB Refill (l/i)",
    "TLB Refill (s)",
    "Addr Error (l/i)",
    "Addr Error (s)",
    "Bus Error (i)",
    "Bus Error (data)",
    "Syscall",
    "Brk Point",
    "Reserved Inst",
    "Cpr     unusable",
    "Arithmetic ovflo",
    "Trap",
    "---",
    "Floating point",
};

static u8 D_16003888 = 0; /* framebuffer index (D_8002AAE8) to draw into */
static u16 D_1600388C = 0xFFFF; /* text colour (RGBA5551) */
static s32 D_16003890 = 0; /* buttons newly pressed */
static s32 D_16003894 = 0; /* buttons held now (with the STICK_* bits) */
static s32 D_16003898 = 0; /* buttons held on the previous poll */
static OSThread *D_1600389C = NULL; /* faulted thread */
static s32 D_160038A0 = 0; /* first text row that may be drawn */
static u8 D_160038A4 = 0; /* the fault is a deliberate lockup */
static s32 D_160038A8 = 292; /* framebuffer width in pixels */

/* COP0 TLB snapshot taken by func_16003650: EntryLo0, EntryLo1, EntryHi and PageMask of each entry. */
u32 D_160038AC[32] = { 0 };
u32 D_1600392C[32] = { 0 };
u32 D_160039AC[32] = { 0 };
u32 D_16003A2C[32] = { 0 };
static u32 D_16003AAC[17] = { 0 }; /* not referenced */

static s32 D_16003AF0 = 1; /* the faulted thread can be resumed */
static u8 D_16003AF4 = SCREEN_PRESS_BUTTON; /* current screen */
static void (*D_16003AF8[4])(void) = { func_16000000, func_16000058, func_16000314, func_1600078C };
static s32 (*D_16003B08[4])(void) = { func_16000028, func_16000224, func_16000384, func_16000A5C };
static u32 D_16003B18 = 0x80500000; /* not referenced */
static s8 D_16003B1C = 0; /* selected main menu entry */

/* SCREEN_PRESS_BUTTON: draw "PRESS BUTTON". */
void func_16000000(void) {
    func_160012B0(278, "PRESS BUTTON");
}

static char *D_16003B20[2] = { "REGISTERS", "STACK" };
static s8 D_16003B28 = 0; /* register page */
static s32 D_16003B2C = 0; /* not referenced */

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
    func_160012B0(44, "MAIN MENU");
    position = 108;
    for (i = 0; i < 2; i++) {
        if (D_16003B1C == i) {
            func_16001338(255, 0, 0);
        } else {
            func_16001338(255, 255, 255);
        }
        func_160012B0(position, D_16003B20[i]);
        position += 32;
    }
    if (D_16003B1C == 2) {
        func_16001338(255, 0, 0);
    } else {
        func_16001338(255, 255, 255);
    }
    if (D_16003AF0 != 0) {
        if ((D_1600389C->context.cause == EXC_SYSCALL) && (D_160038A4 == 0)) {
            func_160012B0(position, "CONTINUE");
        } else if (D_16003AF0 != 0) {
            func_160012B0(position, "HOSTDEBUG");
        }
    } else {
        func_160012B0(position, "RETRY CODE");
    }
    func_16001338(255, 255, 255);
    func_160012B0(611, "Version");
    func_16001044(619, PRINT_DEC, 163);
    func_160012B0(643, "Built");
    func_160012B0(651, "Dec 19 2000");
    func_160012B0(663, "09:57:42");
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
            func_160006CC(D_1600389C);
            break;
        case 1:
        case 2:
            func_16000590(D_1600389C);
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
void func_16000424(OSThread *faulted) {
    u32 cause;
    u32 exception;
    OSThread *thread = faulted; // matching: the extra local keeps IDO's stack-slot layout

    func_16001338(255, 255, 255);
    func_160012B0(35, "Pc");
    func_16001044(43, PRINT_HEX, thread->context.pc);
    func_160012B0(67, "Cause");
    func_16001044(75, PRINT_HEX, thread->context.cause);
    func_16001338(128, 128, 255);
    cause = thread->context.cause;
    exception = (cause >> 2) & 0xF;

    func_160012B0(107, D_16003848[exception]); // exception name
    if (exception == 11) { // coprocessor unusable: show which one
        func_16001044(111, PRINT_DEC, (cause >> 28) & 3);
    }
    func_16001338(255, 255, 255);
    func_160012B0(131, "Sr");
    func_16001044(139, PRINT_HEX, thread->context.sr);
    func_160012B0(163, "Badvadr");
    func_16001044(171, PRINT_HEX, thread->context.badvaddr);
    func_160012B0(195, "Thread");
    func_16001044(203, PRINT_DEC, thread->id);
    if (D_160038A4 != 0) {
        func_160012B0(52, "Lockup_Now");
    }
}

/* FPCSR cause names, from bit 12 up */
static char *D_16003B30[6] = { "Inexact op", "Underflow", "Overflow", "Div by 0", "Invalid op", "Unimplemented op" };

/* Register pages 1 and 2: the FPCSR with its raised exception causes, then 16 FPU registers. */
void func_16000590(OSThread *thread) {
    s32 position;
    s32 i;
    s32 word;
    u32 bits;
    u32 page = 0;
    u32 *words = (u32 *)thread;

    bits = thread->context.fpcsr;
    func_160012B0(3, "Fpcsr:");
    func_16001044(10, PRINT_HEX, bits);
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
        word = 0x6C; // 16 registers past context.fp0, i.e. past the end of the OSThread
        page = 16;
    }
    do {
        func_160012B0(position, "fp");
        func_16001044(position + 2, PRINT_DEC, i + page);
        func_16001044(position + 5, PRINT_FLOAT, words[word + 1]); // f_even, the register's low word
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
    DebuggerLabel label = { { 0, 0, ' ', '\0' } };
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
        func_16001044(position, PRINT_HEX, ((u32 *)thread)[index + 1]); // low word
        position += 13;
        entry += 3;
    } while (entry[0] != 0);
}

static s32 D_16003B4C = 0; /* first stack word shown */

/* True when addr lies in the 0x400 bytes at the start of BSS. */
#define IN_BSS_START(addr) (((u32)(addr) >= (u32)&D_8002D4B0) && ((u32)(addr) < (u32)&D_8002D4B0 + 0x400))
/* True when addr lies in the 0x4000 bytes at D_8002D8B0. */
#define IN_D_8002D8B0(addr) (((u32)(addr) >= (u32)&D_8002D8B0) && ((u32)(addr) < (u32)&D_8002D8B0 + 0x4000))

/*
 * SCREEN_STACK: 22 words of the thread's stack from sp + D_16003B4C words, each as its address,
 * its hex value (coloured by the segment it points into) and its decimal value.
 */
void func_1600078C(void) {
    u32 *stack;
    u32 *address;
    s32 position;
    s32 row;
    s32 decimal_position;
    u32 value;
    u32 segment;

    stack = (u32 *)(u32)D_1600389C->context.sp;
    func_16001338(0, 255, 0);
    func_160012B0(11, "STACK-VIEW");
    address = stack + D_16003B4C;
    if (!((u32)address & 3) && ((u32)address >= 0x80000000) && ((u32)address < 0x80800001)) {
        if (D_16003B4C == 0) {
            func_16001338(255, 0, 0);
        } else if (IN_BSS_START(address)) {
            func_16001338(128, 128, 255);
        } else if (IN_D_8002D8B0(address)) {
            func_16001338(255, 128, 128);
        } else {
            func_16001338(255, 255, 255);
        }
        position = 97;
        for (row = 0; row < 22; row++) {
            func_16001044(position, PRINT_HEX, (u32)address);
            func_160012B0(position + 8, ":");
            value = *address;
            segment = (value >> 24) & 0xFF;
            if (segment == 0x80) {
                func_16001338(128, 128, 255);
            } else if (segment == 0x15) {
                func_16001338(255, 0, 0);
            } else if (segment == 0x16) {
                func_16001338(128, 255, 128);
            } else if (segment == 0x10) {
                func_16001338(255, 0, 0);
            } else {
                func_16001338(255, 255, 255);
            }
            func_16001044(position + 12, PRINT_HEX, value);
            decimal_position = position + 22;
            func_160012B0(decimal_position, "             "); // blank out the previous number
            func_16001338(255, 255, 255);
            func_16001044(decimal_position, PRINT_DEC, value);
            if (IN_BSS_START(address)) {
                func_16001338(128, 128, 255);
            } else if (IN_D_8002D8B0(address)) {
                func_16001338(255, 128, 128);
            } else {
                func_16001338(255, 255, 255);
            }
            position += 32;
            address++;
        }
    }
}

/* SCREEN_STACK input: stick or C up/down scrolls through the stack, B returns to the main menu. */
s32 func_16000A5C(void) {
    s32 result = 0;

    if ((D_160036F0[0].stick_y >= 41) || (D_16003890 & U_CBUTTONS)) {
        D_16003B4C--;
        if (D_16003B4C < 0) {
            D_16003B4C = 0;
        } else {
            result = DEBUGGER_REDRAW | DEBUGGER_CLEAR;
        }
    } else if ((D_160036F0[0].stick_y < -40) || (D_16003890 & D_CBUTTONS)) {
        D_16003B4C++;
        if (D_16003B4C >= 201) {
            D_16003B4C = 200;
        }
        result = DEBUGGER_REDRAW | DEBUGGER_CLEAR;
    }
    if (D_16003890 & B_BUTTON) {
        D_16003AF4 = SCREEN_MAIN_MENU;
        result = DEBUGGER_REDRAW | DEBUGGER_CLEAR;
    }
    return result;
}

/*
 * Debugger entry for a faulted thread (called from func_10007DAC): run the screens until one
 * returns DEBUGGER_EXIT. Returns 1 when the thread should be resumed.
 */
s32 func_16000B14(OSThread *thread) {
    u32 *tlb;
    u32 *framebuffer;
    u32 entryLo;
    s32 first;
    s32 result;
    s32 stick;
    u32 page;
    s32 odd;
    s32 i;
    u32 *tlbCopy;
    void (*draw)(void);
    s32 (*input)(void);

    result = 0;
    first = TRUE;
    if (D_8002AC5C != 0) {
        return 0;
    }
    D_16003888 = 0;
    if ((D_8002AAE8[0] == NULL) || (D_8002AAE8[1] == NULL)) {
        D_8002AAE8[0] = (u16 *)0x80350000;
        D_8002AAE8[1] = (u16 *)0x80350000;
        return 0;
    }
    func_16003650();
    tlb = D_8003C8E8;
    tlbCopy = tlb; // matching: a second pointer keeps IDO's stack slot layout
    D_160038AC[15] = tlbCopy[0];
    D_1600392C[15] = tlbCopy[1];
    D_160039AC[15] = tlbCopy[2];
    D_16003A2C[15] = tlbCopy[3];
    if ((thread->context.pc & 0xFF000000) != 0x15000000) {
        D_16003AF0 = 1;
    } else {
        // the pc is in a TLB-mapped segment: look for a valid mapping of its page
        page = thread->context.pc & ~0xFFF;
        odd = page & 0x1000;
        page &= ~0x1000;
        D_16003AF0 = 0;
        for (i = 0; i < 32; i++) {
            if (page == D_160039AC[i]) {
                entryLo = odd ? D_1600392C[i] : D_160038AC[i];
                if (entryLo & 2) { // valid
                    D_16003AF0 = 1;
                }
            }
        }
    }
    if ((((u32)D_8003C8E0 >> 24) & 0xFF) == 0xC) {
        thread = &D_80031AE0;
    }
    framebuffer = __osViCurr->framep;
    if (framebuffer == (u32 *)D_8002AAE8[1]) {
        *(s8 *)&D_16003888 = 1; // matching: an s8 store lets IDO reuse the 1 already in a register
    }
    D_1600389C = thread;
    D_160038A4 = 0;
    if ((thread->context.cause == EXC_SYSCALL) && (thread->context.pc == (u32)func_150AD770)) {
        *(s8 *)&D_160038A4 = 1; // matching: as above
    }
    do {
        if (!first && (result & DEBUGGER_CLEAR)) {
            func_16001678();
        }
        draw = D_16003AF8[D_16003AF4];
        if (draw != NULL) {
            draw();
        }
        osWritebackDCacheAll();
        do {
            result = 0;
            D_16003898 = D_16003894;
            func_16001700();
            func_16001830(D_160036F0);
            if (first) {
                D_16003898 = D_16003894;
            }
            D_16003894 = D_160036F0[0].button;
            stick = D_160036F0[0].stick_x;
            if (stick >= 51) {
                D_16003894 |= STICK_RIGHT;
            }
            if (stick < -50) {
                D_16003894 |= STICK_LEFT;
            }
            stick = D_160036F0[0].stick_y;
            if (stick >= 51) {
                D_16003894 |= STICK_UP;
            }
            if (stick < -50) {
                D_16003894 |= STICK_DOWN;
            }
            D_16003890 = (D_16003894 ^ D_16003898) & D_16003894; // newly pressed
            input = D_16003B08[D_16003AF4];
            if (input != NULL) {
                result = input();
            }
        } while (!(result & (DEBUGGER_REDRAW | DEBUGGER_EXIT)));
        first = FALSE;
    } while (!(result & DEBUGGER_EXIT));
    if (D_16003AF0 == 0) {
        thread->state = OS_STATE_RUNNING;
        thread->flags = 0;
        return 1;
    }
    if ((thread->context.cause == EXC_SYSCALL) && (D_160038A4 == 0)) {
        thread->context.pc += 4; // step over the syscall
        return 1;
    }
    return 0;
}
/* Print a float at a text position ("NaN" for NaN, infinity and denormals). */
/* (The buffer and the format of func_16001B34 give warning 709 twice per call: that copy of sprintf in
 * debugger_257350.c still takes u8 * where libultra's sprintf takes char *.) */
void func_16000F8C(s32 position, f32 value) {
    f32 copy;
    char buffer[44];
    u32 exponent;

    if ((position >= (D_160038A0 << 5)) && (position < 833)) {
        copy = value;
        exponent = (u32)(*(s32 *)&copy & 0x7F800000) >> 23;
        // matching: reusing exponent for the shifted bits keeps them in the same register
        if (((exponent == 0) || (exponent >= 255)) && ((exponent = *(s32 *)&copy << 1) != 0)) {
            func_160012B0(position, "NaN");
            return;
        }
        func_16001B34(buffer, "%s%s%f", "", "", value);
        func_160012B0(position, buffer);
    }
}

/* Print value at a text position as 8 hex digits (PRINT_HEX), signed decimal (PRINT_DEC) or as float bits (PRINT_FLOAT). */
void func_16001044(s32 position, s32 mode, u32 value) {
    s32 i;
    s32 started;
    s32 place;
    s32 digit;
    s32 exponent;
    u8 character;
    DebuggerDecimalPowers powers = { { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 } };
    s32 power;
    u32 mantissa;
    f32 copy;
    char buffer[36];
    u8 *destination;

    if ((position >= (D_160038A0 << 5)) && (position < 833)) {
        destination = func_1600160C(position);
        switch (mode) {
        case PRINT_HEX:
            destination += 7 * 16; // last digit first
            i = 0;
            do {
                character = value & 0xF;
                if (character >= 10) {
                    character += 'A' - '0' - 10;
                }
                character += '0';
                func_160014F0(destination, character);
                value = (s32)value >> 4;
                i++;
                destination -= 16;
            } while (i != 8);
            break;
        case PRINT_DEC:
            if ((s32)value < 0) {
                destination = func_160014F0(destination, '-');
                value = -value;
            }
            started = FALSE;
            for (place = 9; place >= 0; place--) {
                power = powers.power[place];
                digit = (s32)value / power;
                value = (s32)value % power;
                if ((digit > 0) || started || (place == 0)) {
                    destination = func_160014F0(destination, digit + '0');
                    started = TRUE;
                }
            }
            break;
        case PRINT_FLOAT:
            exponent = (s32)(value & 0x7F800000) >> 23;
            mantissa = value << 9;
            if (((exponent <= 0) || (exponent >= 255)) && ((exponent != 0) || mantissa)) {
                func_160012B0(position, "NaN");
                return;
            }
            *(u32 *)&copy = value;
            func_16001B34(buffer, "%s%s%f", "", "", copy);
            func_160012B0(position, buffer);
            break;
        }
    }
}

/* Print a NUL-terminated string at a text position (row * 32 + column). */
void func_160012B0(s32 position, const char *text) {
    u8 *destination;
    u8 character;

    if ((text != NULL) && (position >= (D_160038A0 << 5)) && (position < 833)) {
        destination = func_1600160C(position);
        while ((character = *text) != '\0') {
            destination = func_160014F0(destination, character);
            text++;
        }
    }
}

/* Set the text colour (D_1600388C) as an opaque RGBA5551 pixel. */
void func_16001338(u8 red, u8 green, u8 blue) {
    D_1600388C = ((red & 0xF8) << 8) | ((green & 0xF8) << 3) | ((blue & 0xF8) >> 2) | 1;
}

/* Fill the pixel rectangle (left, top)..(right, bottom), inclusive, with the text colour. */
void func_16001390(s16 left, s16 top, s16 right, s16 bottom) {
    u16 *pixel;
    s32 count;

    if ((right < left) || (bottom < top)) {
        return;
    }
    if ((left < 0) || (top < 0)) {
        return;
    }
    right++;
    bottom++;
    pixel = (u16 *)func_1600160C(0);
    pixel += left + top * D_160038A8;
    right -= left;
    bottom -= top;
    while (bottom > 0) {
        for (count = right; count > 0; count--) {
            *pixel++ = D_1600388C;
        }
        bottom--;
        pixel += D_160038A8 - right;
    }
}

/*
 * Draw one 8x8 glyph of the font (D_16003CE0, from ' ' up) at destination in the text colour
 * on the background pixel 1; returns the position of the next character.
 */
u8 *func_160014F0(u8 *destination, u8 character) {
    u16 *pixel;
    u16 color;
    u32 column;
    u16 bits;
    u32 row;
    const u8 *glyph;
    s32 code;
    u16 pixelValue;

    pixel = (u16 *)destination;
    color = D_1600388C;
    column = character; // matching: routing the char through column puts both in the same register
    code = column;
    if (code < ' ') {
        code = ' ';
    }
    row = 0, glyph = D_16003CE0 + ((code - ' ') << 3); // matching: own line steers as1's scheduling
    for (; row < 8; row++, glyph++) {
        for (column = 0, bits = *glyph; column < 8; column++) {
            pixelValue = (bits & 0x80) ? color : 1;
            bits <<= 1;
            *pixel++ = pixelValue;
        }
        pixel += D_160038A8 - 8;
    }
    return destination + 16;
}

/*
 * Framebuffer address of a text position (row * 32 + column) in the current framebuffer
 * (D_8002AAE8[D_16003888]), whose width in pixels is D_160038A8. Text rows are 8 scanlines
 * apart when the width is 292 and 10 otherwise; the text area starts 2 scanlines and
 * 8 pixels in.
 */
u8 *func_1600160C(s32 position) {
    s32 row = position & 0xFFE0;
    s32 offset = row;

    if (D_160038A8 != 292) {
        offset = (row >> 2) + row;
    }
    offset = (offset >> 2) * (D_160038A8 << 1);
    offset += (position & 0x1F) * 16;
    offset += D_160038A8 * 4;
    offset += 16;
    return (u8 *)D_8002AAE8[D_16003888] + offset;
}

/* Clear the current framebuffer to pixel value 1 (215 scanlines at width 292, 264 otherwise). */
void func_16001678(void) {
    u32 *cursor = (u32 *)D_8002AAE8[D_16003888];
    s32 lines;
    u32 *end;

    if (D_160038A8 == 292) {
        lines = 215;
    } else {
        lines = 264;
    }
    end = (D_160038A8 >> 1) * lines + cursor;
    while (cursor < end) {
        cursor[0] = 0x10001;
        cursor[1] = 0x10001;
        cursor[2] = 0x10001;
        cursor[3] = 0x10001;
        cursor += 4;
    }
}

s32 func_160016F4(s32 value) {
    return value;
}

/* Three sets of VI registers (not referenced): a PAL and an MPAL 320-pixel mode and a 292-pixel mode */
static u32 D_16003B78[3][20] = {
    { 0x1E000000, 0x140, 0x404233A, 0x271, 0x150C69, 0xC6F0C6E, 0x800300, 0x200, 0, 0x280, 0x400, 0x5F0239, 0x9026B, 2, 0x280, 0x400, 0x5F0239, 0x9026B, 2, 0 },
    { 0x1E000000, 0x140, 0x4651E39, 0x20D, 0x40C11, 0xC190C1A, 0x6C02EC, 0x200, 0, 0x280, 0x400, 0x2501FF, 0xE0204, 2, 0x280, 0x400, 0x2501FF, 0xE0204, 2, 0 },
    { 0x3E000000, 0x124, 0x3E52239, 0x20D, 0xC15, 0xC150C15, 0x8C02D5, 0x200, 0, 0x248, 0x400, 0x3D01E8, 0xE0204, 2, 0x248, 0x400, 0x3D01E8, 0xE0204, 2, 0 },
};
