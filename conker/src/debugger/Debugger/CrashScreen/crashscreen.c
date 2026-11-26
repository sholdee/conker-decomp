#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Extra bits debugger_run ORs into the held-button mask for the analog stick. */
#define STICK_LEFT  0x10000
#define STICK_RIGHT 0x20000
#define STICK_UP    0x40000
#define STICK_DOWN  0x80000

/* Result bits of a screen's input handler (see debugger_run's loops). */
#define DEBUGGER_REDRAW 1 /* draw the current screen again */
#define DEBUGGER_CLEAR  2 /* clear the framebuffer before drawing */
#define DEBUGGER_EXIT   4 /* leave the debugger */

/* debugger_printValue modes */
#define PRINT_HEX   0
#define PRINT_DEC   1
#define PRINT_FLOAT 2

/* Screens (gDebuggerScreen) indexing the draw (gDebuggerDrawFuncs) and input (gDebuggerInputFuncs) tables. */
#define SCREEN_PRESS_BUTTON 0
#define SCREEN_MAIN_MENU    1
#define SCREEN_REGISTERS    2
#define SCREEN_STACK        3

/* Two-character register name, a space and a terminator, filled in from gDebuggerCpuRegTable. */
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
 * debugger_drawCpuRegisters and debugger_printValue). The debugger has no .bss: its variables are all initialized. */
static OSContPad gDebuggerPads[4] = { 0 }; /* read by debugger_contGetReadData */

/* Lockup reasons (not referenced) */
static char *gDebuggerLockupReasons[] = {
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
static u8 gDebuggerCpuRegTable[] = {
    'v', '0', 10, 'v', '1', 12, 'a', '0', 14, 'a', '1', 16, 'a', '2', 18, 'a', '3', 20,
    't', '0', 22, 't', '1', 24, 't', '2', 26, 't', '3', 28, 't', '4', 30, 't', '5', 32,
    't', '6', 34, 't', '7', 36, 't', '8', 54, 't', '9', 56, 's', '0', 38, 's', '1', 40,
    's', '2', 42, 's', '3', 44, 's', '4', 46, 's', '5', 48, 's', '6', 50, 's', '7', 52,
    's', '8', 62, 's', 'p', 60, 'g', 'p', 58, 'r', 'a', 64, 0,
};

/* Exception names, by the ExcCode field of the cause register */
static char *gDebuggerExceptionNames[16] = {
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

static u8 gDebuggerFbIndex = 0; /* framebuffer index (gFramebuffers) to draw into */
static u16 gDebuggerTextColor = 0xFFFF; /* text colour (RGBA5551) */
static s32 gDebuggerButtonsPressed = 0; /* buttons newly pressed */
static s32 gDebuggerButtonsHeld = 0; /* buttons held now (with the STICK_* bits) */
static s32 gDebuggerButtonsPrev = 0; /* buttons held on the previous poll */
static OSThread *gDebuggerThread = NULL; /* faulted thread */
static s32 gDebuggerFirstRow = 0; /* first text row that may be drawn */
static u8 gDebuggerIsLockup = 0; /* the fault is a deliberate lockup */
static s32 gDebuggerFbWidth = 292; /* framebuffer width in pixels */

/* COP0 TLB snapshot taken by debugger_readTlb: EntryLo0, EntryLo1, EntryHi and PageMask of each entry. */
u32 gTlbEntryLo0[32] = { 0 };
u32 gTlbEntryLo1[32] = { 0 };
u32 gTlbEntryHi[32] = { 0 };
u32 gTlbPageMask[32] = { 0 };
static u32 D_16003AAC[17] = { 0 }; /* not referenced */

static s32 gDebuggerCanResume = 1; /* the faulted thread can be resumed */
static u8 gDebuggerScreen = SCREEN_PRESS_BUTTON; /* current screen */
static void (*gDebuggerDrawFuncs[4])(void) = { debugger_drawPrompt, debugger_drawMainMenu, debugger_drawRegisters, debugger_drawStack };
static s32 (*gDebuggerInputFuncs[4])(void) = { debugger_inputPrompt, debugger_inputMainMenu, debugger_inputRegisters, debugger_inputStack };
static u32 D_16003B18 = 0x80500000; /* not referenced */
static s8 gDebuggerMenuSelection = 0; /* selected main menu entry */

/* SCREEN_PRESS_BUTTON: draw "PRESS BUTTON". */
void debugger_drawPrompt(void) {
    debugger_printText(278, "PRESS BUTTON");
}

static char *gDebuggerMenuEntries[2] = { "REGISTERS", "STACK" };
static s8 gDebuggerRegPage = 0; /* register page */
static s32 D_16003B2C = 0; /* not referenced */

/* SCREEN_PRESS_BUTTON input: A or B opens the main menu. */
s32 debugger_inputPrompt(void) {
    if (gDebuggerButtonsPressed & (A_BUTTON | B_BUTTON)) {
        gDebuggerScreen = SCREEN_MAIN_MENU;
        return DEBUGGER_REDRAW | DEBUGGER_CLEAR;
    }
    return 0;
}

/* SCREEN_MAIN_MENU: draw the menu with the selected entry (gDebuggerMenuSelection) in red, then the build info. */
void debugger_drawMainMenu(void) {
    s32 i;
    s32 position;

    debugger_setTextColor(255, 255, 255);
    debugger_printText(44, "MAIN MENU");
    position = 108;
    for (i = 0; i < 2; i++) {
        if (gDebuggerMenuSelection == i) {
            debugger_setTextColor(255, 0, 0);
        } else {
            debugger_setTextColor(255, 255, 255);
        }
        debugger_printText(position, gDebuggerMenuEntries[i]);
        position += 32;
    }
    if (gDebuggerMenuSelection == 2) {
        debugger_setTextColor(255, 0, 0);
    } else {
        debugger_setTextColor(255, 255, 255);
    }
    if (gDebuggerCanResume != 0) {
        if ((gDebuggerThread->context.cause == EXC_SYSCALL) && (gDebuggerIsLockup == 0)) {
            debugger_printText(position, "CONTINUE");
        } else if (gDebuggerCanResume != 0) {
            debugger_printText(position, "HOSTDEBUG");
        }
    } else {
        debugger_printText(position, "RETRY CODE");
    }
    debugger_setTextColor(255, 255, 255);
    debugger_printText(611, "Version");
    debugger_printValue(619, PRINT_DEC, 163);
    debugger_printText(643, "Built");
    debugger_printText(651, "Dec 19 2000");
    debugger_printText(663, "09:57:42");
}

/* SCREEN_MAIN_MENU input: stick up/down moves the selection, A picks it. */
s32 debugger_inputMainMenu(void) {
    s32 result = 0;

    if (gDebuggerButtonsPressed & STICK_UP) {
        gDebuggerMenuSelection--;
        result = DEBUGGER_REDRAW;
    }
    if (gDebuggerButtonsPressed & STICK_DOWN) {
        gDebuggerMenuSelection++;
        result = DEBUGGER_REDRAW;
    }
    if (gDebuggerMenuSelection >= 3) {
        gDebuggerMenuSelection = 0;
    }
    if (gDebuggerMenuSelection < 0) {
        gDebuggerMenuSelection = 2;
    }
    if (gDebuggerButtonsPressed & A_BUTTON) {
        switch (gDebuggerMenuSelection) {
            case 0:
                result = DEBUGGER_REDRAW | DEBUGGER_CLEAR;
                gDebuggerScreen = SCREEN_REGISTERS;
                break;
            case 1:
                result = DEBUGGER_REDRAW | DEBUGGER_CLEAR;
                gDebuggerScreen = SCREEN_STACK;
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
void debugger_drawRegisters(void) {
    switch (gDebuggerRegPage) {
        case 0:
            debugger_drawCpuStatus(gDebuggerThread);
            debugger_drawCpuRegisters(gDebuggerThread);
            break;
        case 1:
        case 2:
            debugger_drawFpuRegisters(gDebuggerThread);
            break;
    }
}

/* SCREEN_REGISTERS input: C buttons flip through the pages, B returns to the main menu. */
s32 debugger_inputRegisters(void) {
    s32 result = 0;

    if (gDebuggerButtonsPressed & (R_CBUTTONS | D_CBUTTONS)) {
        gDebuggerRegPage++;
        if (gDebuggerRegPage >= 3) {
            gDebuggerRegPage = 0;
        }
        return DEBUGGER_REDRAW | DEBUGGER_CLEAR;
    }
    if (gDebuggerButtonsPressed & (L_CBUTTONS | U_CBUTTONS)) {
        gDebuggerRegPage--;
        if (gDebuggerRegPage < 0) {
            gDebuggerRegPage = 2;
        }
        return DEBUGGER_REDRAW | DEBUGGER_CLEAR;
    }
    if (gDebuggerButtonsPressed & B_BUTTON) {
        gDebuggerScreen = SCREEN_MAIN_MENU;
        result = DEBUGGER_REDRAW | DEBUGGER_CLEAR;
    }
    return result;
}

/* Register page 0, top half: pc, cause (with its exception name), sr, badvaddr and thread id. */
void debugger_drawCpuStatus(OSThread *faulted) {
    u32 cause;
    u32 exception;
    OSThread *thread = faulted; // matching: the extra local keeps IDO's stack-slot layout

    debugger_setTextColor(255, 255, 255);
    debugger_printText(35, "Pc");
    debugger_printValue(43, PRINT_HEX, thread->context.pc);
    debugger_printText(67, "Cause");
    debugger_printValue(75, PRINT_HEX, thread->context.cause);
    debugger_setTextColor(128, 128, 255);
    cause = thread->context.cause;
    exception = (cause >> 2) & 0xF;

    debugger_printText(107, gDebuggerExceptionNames[exception]); // exception name
    if (exception == 11) { // coprocessor unusable: show which one
        debugger_printValue(111, PRINT_DEC, (cause >> 28) & 3);
    }
    debugger_setTextColor(255, 255, 255);
    debugger_printText(131, "Sr");
    debugger_printValue(139, PRINT_HEX, thread->context.sr);
    debugger_printText(163, "Badvadr");
    debugger_printValue(171, PRINT_HEX, thread->context.badvaddr);
    debugger_printText(195, "Thread");
    debugger_printValue(203, PRINT_DEC, thread->id);
    if (gDebuggerIsLockup != 0) {
        debugger_printText(52, "Lockup_Now");
    }
}

/* FPCSR cause names, from bit 12 up */
static char *gFpuExceptionNames[6] = { "Inexact op", "Underflow", "Overflow", "Div by 0", "Invalid op", "Unimplemented op" };

/* Register pages 1 and 2: the FPCSR with its raised exception causes, then 16 FPU registers. */
void debugger_drawFpuRegisters(OSThread *thread) {
    s32 position;
    s32 i;
    s32 word;
    u32 bits;
    u32 page = 0;
    u32 *words = (u32 *)thread;

    bits = thread->context.fpcsr;
    debugger_printText(3, "Fpcsr:");
    debugger_printValue(10, PRINT_HEX, bits);
    bits >>= 12; // cause bits: inexact, underflow, overflow, div by zero, invalid, unimplemented
    position = 44;
    for (i = 0; i < 6; i++, bits >>= 1) {
        if (bits & 1) {
            debugger_printText(position, gFpuExceptionNames[i]);
            position += 32;
        }
    }
    i = 0;
    position = 195;
    if (gDebuggerRegPage == 1) {
        word = 0x4C; // context.fp0
    } else {
        word = 0x6C; // 16 registers past context.fp0, i.e. past the end of the OSThread
        page = 16;
    }
    do {
        debugger_printText(position, "fp");
        debugger_printValue(position + 2, PRINT_DEC, i + page);
        debugger_printValue(position + 5, PRINT_FLOAT, words[word + 1]); // f_even, the register's low word
        word += 2;
        position += 32;
        i++;
    } while (i < 16);
}

/*
 * Register page 0, bottom half: each gDebuggerCpuRegTable entry is a two-character name followed by
 * the u32 index of the register's (u64) slot in the OSThread; a zero name ends the table.
 */
void debugger_drawCpuRegisters(OSThread *thread) {
    DebuggerLabel label = { { 0, 0, ' ', '\0' } };
    s32 position = 291;
    u8 *entry = gDebuggerCpuRegTable;
    s32 index;

    debugger_setTextColor(192, 192, 255);
    do {
        label.text[0] = entry[0];
        label.text[1] = entry[1];
        debugger_printText(position, label.text);
        position += 3;
        index = entry[2];
        debugger_printValue(position, PRINT_HEX, ((u32 *)thread)[index + 1]); // low word
        position += 13;
        entry += 3;
    } while (entry[0] != 0);
}

static s32 gDebuggerStackOffset = 0; /* first stack word shown */

/* True when addr lies in the 0x400 bytes at the start of BSS. */
#define IN_BSS_START(addr) (((u32)(addr) >= (u32)&gIdleThreadStack) && ((u32)(addr) < (u32)&gIdleThreadStack + 0x400))
/* True when addr lies in the 0x4000 bytes at gMainThreadStack. */
#define IN_D_8002D8B0(addr) (((u32)(addr) >= (u32)&gMainThreadStack) && ((u32)(addr) < (u32)&gMainThreadStack + 0x4000))

/*
 * SCREEN_STACK: 22 words of the thread's stack from sp + gDebuggerStackOffset words, each as its address,
 * its hex value (coloured by the segment it points into) and its decimal value.
 */
void debugger_drawStack(void) {
    u32 *stack;
    u32 *address;
    s32 position;
    s32 row;
    s32 decimal_position;
    u32 value;
    u32 segment;

    stack = (u32 *)(u32)gDebuggerThread->context.sp;
    debugger_setTextColor(0, 255, 0);
    debugger_printText(11, "STACK-VIEW");
    address = stack + gDebuggerStackOffset;
    if (!((u32)address & 3) && ((u32)address >= 0x80000000) && ((u32)address < 0x80800001)) {
        if (gDebuggerStackOffset == 0) {
            debugger_setTextColor(255, 0, 0);
        } else if (IN_BSS_START(address)) {
            debugger_setTextColor(128, 128, 255);
        } else if (IN_D_8002D8B0(address)) {
            debugger_setTextColor(255, 128, 128);
        } else {
            debugger_setTextColor(255, 255, 255);
        }
        position = 97;
        for (row = 0; row < 22; row++) {
            debugger_printValue(position, PRINT_HEX, (u32)address);
            debugger_printText(position + 8, ":");
            value = *address;
            segment = (value >> 24) & 0xFF;
            if (segment == 0x80) {
                debugger_setTextColor(128, 128, 255);
            } else if (segment == 0x15) {
                debugger_setTextColor(255, 0, 0);
            } else if (segment == 0x16) {
                debugger_setTextColor(128, 255, 128);
            } else if (segment == 0x10) {
                debugger_setTextColor(255, 0, 0);
            } else {
                debugger_setTextColor(255, 255, 255);
            }
            debugger_printValue(position + 12, PRINT_HEX, value);
            decimal_position = position + 22;
            debugger_printText(decimal_position, "             "); // blank out the previous number
            debugger_setTextColor(255, 255, 255);
            debugger_printValue(decimal_position, PRINT_DEC, value);
            if (IN_BSS_START(address)) {
                debugger_setTextColor(128, 128, 255);
            } else if (IN_D_8002D8B0(address)) {
                debugger_setTextColor(255, 128, 128);
            } else {
                debugger_setTextColor(255, 255, 255);
            }
            position += 32;
            address++;
        }
    }
}

/* SCREEN_STACK input: stick or C up/down scrolls through the stack, B returns to the main menu. */
s32 debugger_inputStack(void) {
    s32 result = 0;

    if ((gDebuggerPads[0].stick_y >= 41) || (gDebuggerButtonsPressed & U_CBUTTONS)) {
        gDebuggerStackOffset--;
        if (gDebuggerStackOffset < 0) {
            gDebuggerStackOffset = 0;
        } else {
            result = DEBUGGER_REDRAW | DEBUGGER_CLEAR;
        }
    } else if ((gDebuggerPads[0].stick_y < -40) || (gDebuggerButtonsPressed & D_CBUTTONS)) {
        gDebuggerStackOffset++;
        if (gDebuggerStackOffset >= 201) {
            gDebuggerStackOffset = 200;
        }
        result = DEBUGGER_REDRAW | DEBUGGER_CLEAR;
    }
    if (gDebuggerButtonsPressed & B_BUTTON) {
        gDebuggerScreen = SCREEN_MAIN_MENU;
        result = DEBUGGER_REDRAW | DEBUGGER_CLEAR;
    }
    return result;
}

/*
 * Debugger entry for a faulted thread (called from func_10007DAC): run the screens until one
 * returns DEBUGGER_EXIT. Returns 1 when the thread should be resumed.
 */
s32 debugger_run(OSThread *thread) {
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
    if (gResetPending != 0) {
        return 0;
    }
    gDebuggerFbIndex = 0;
    if ((gFramebuffers[0] == NULL) || (gFramebuffers[1] == NULL)) {
        gFramebuffers[0] = (u16 *)0x80350000;
        gFramebuffers[1] = (u16 *)0x80350000;
        return 0;
    }
    debugger_readTlb();
    tlb = D_8003C8E8;
    tlbCopy = tlb; // matching: a second pointer keeps IDO's stack slot layout
    gTlbEntryLo0[15] = tlbCopy[0];
    gTlbEntryLo1[15] = tlbCopy[1];
    gTlbEntryHi[15] = tlbCopy[2];
    gTlbPageMask[15] = tlbCopy[3];
    if ((thread->context.pc & 0xFF000000) != 0x15000000) {
        gDebuggerCanResume = 1;
    } else {
        // the pc is in a TLB-mapped segment: look for a valid mapping of its page
        page = thread->context.pc & ~0xFFF;
        odd = page & 0x1000;
        page &= ~0x1000;
        gDebuggerCanResume = 0;
        for (i = 0; i < 32; i++) {
            if (page == gTlbEntryHi[i]) {
                entryLo = odd ? gTlbEntryLo1[i] : gTlbEntryLo0[i];
                if (entryLo & 2) { // valid
                    gDebuggerCanResume = 1;
                }
            }
        }
    }
    if ((((u32)D_8003C8E0 >> 24) & 0xFF) == 0xC) {
        thread = &gMainThread;
    }
    framebuffer = __osViCurr->framep;
    if (framebuffer == (u32 *)gFramebuffers[1]) {
        *(s8 *)&gDebuggerFbIndex = 1; // matching: an s8 store lets IDO reuse the 1 already in a register
    }
    gDebuggerThread = thread;
    gDebuggerIsLockup = 0;
    if ((thread->context.cause == EXC_SYSCALL) && (thread->context.pc == (u32)debug_fatalTrap)) {
        *(s8 *)&gDebuggerIsLockup = 1; // matching: as above
    }
    do {
        if (!first && (result & DEBUGGER_CLEAR)) {
            debugger_clearScreen();
        }
        draw = gDebuggerDrawFuncs[gDebuggerScreen];
        if (draw != NULL) {
            draw();
        }
        osWritebackDCacheAll();
        do {
            result = 0;
            gDebuggerButtonsPrev = gDebuggerButtonsHeld;
            debugger_contStartReadData();
            debugger_contGetReadData(gDebuggerPads);
            if (first) {
                gDebuggerButtonsPrev = gDebuggerButtonsHeld;
            }
            gDebuggerButtonsHeld = gDebuggerPads[0].button;
            stick = gDebuggerPads[0].stick_x;
            if (stick >= 51) {
                gDebuggerButtonsHeld |= STICK_RIGHT;
            }
            if (stick < -50) {
                gDebuggerButtonsHeld |= STICK_LEFT;
            }
            stick = gDebuggerPads[0].stick_y;
            if (stick >= 51) {
                gDebuggerButtonsHeld |= STICK_UP;
            }
            if (stick < -50) {
                gDebuggerButtonsHeld |= STICK_DOWN;
            }
            gDebuggerButtonsPressed = (gDebuggerButtonsHeld ^ gDebuggerButtonsPrev) & gDebuggerButtonsHeld; // newly pressed
            input = gDebuggerInputFuncs[gDebuggerScreen];
            if (input != NULL) {
                result = input();
            }
        } while (!(result & (DEBUGGER_REDRAW | DEBUGGER_EXIT)));
        first = FALSE;
    } while (!(result & DEBUGGER_EXIT));
    if (gDebuggerCanResume == 0) {
        thread->state = OS_STATE_RUNNING;
        thread->flags = 0;
        return 1;
    }
    if ((thread->context.cause == EXC_SYSCALL) && (gDebuggerIsLockup == 0)) {
        thread->context.pc += 4; // step over the syscall
        return 1;
    }
    return 0;
}
/* Print a float at a text position ("NaN" for NaN, infinity and denormals). */
/* (The buffer and the format of debugger_sprintf give warning 709 twice per call: that copy of sprintf in
 * debugger/Debugger/DebuggerPrintf/debuggerprintf.c still takes u8 * where libultra's sprintf takes char *.) */
void debugger_printFloat(s32 position, f32 value) {
    f32 copy;
    char buffer[44];
    u32 exponent;

    if ((position >= (gDebuggerFirstRow << 5)) && (position < 833)) {
        copy = value;
        exponent = (u32)(*(s32 *)&copy & 0x7F800000) >> 23;
        // matching: reusing exponent for the shifted bits keeps them in the same register
        if (((exponent == 0) || (exponent >= 255)) && ((exponent = *(s32 *)&copy << 1) != 0)) {
            debugger_printText(position, "NaN");
            return;
        }
        debugger_sprintf(buffer, "%s%s%f", "", "", value);
        debugger_printText(position, buffer);
    }
}

/* Print value at a text position as 8 hex digits (PRINT_HEX), signed decimal (PRINT_DEC) or as float bits (PRINT_FLOAT). */
void debugger_printValue(s32 position, s32 mode, u32 value) {
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

    if ((position >= (gDebuggerFirstRow << 5)) && (position < 833)) {
        destination = debugger_getTextAddr(position);
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
                debugger_drawGlyph(destination, character);
                value = (s32)value >> 4;
                i++;
                destination -= 16;
            } while (i != 8);
            break;
        case PRINT_DEC:
            if ((s32)value < 0) {
                destination = debugger_drawGlyph(destination, '-');
                value = -value;
            }
            started = FALSE;
            for (place = 9; place >= 0; place--) {
                power = powers.power[place];
                digit = (s32)value / power;
                value = (s32)value % power;
                if ((digit > 0) || started || (place == 0)) {
                    destination = debugger_drawGlyph(destination, digit + '0');
                    started = TRUE;
                }
            }
            break;
        case PRINT_FLOAT:
            exponent = (s32)(value & 0x7F800000) >> 23;
            mantissa = value << 9;
            if (((exponent <= 0) || (exponent >= 255)) && ((exponent != 0) || mantissa)) {
                debugger_printText(position, "NaN");
                return;
            }
            *(u32 *)&copy = value;
            debugger_sprintf(buffer, "%s%s%f", "", "", copy);
            debugger_printText(position, buffer);
            break;
        }
    }
}

/* Print a NUL-terminated string at a text position (row * 32 + column). */
void debugger_printText(s32 position, const char *text) {
    u8 *destination;
    u8 character;

    if ((text != NULL) && (position >= (gDebuggerFirstRow << 5)) && (position < 833)) {
        destination = debugger_getTextAddr(position);
        while ((character = *text) != '\0') {
            destination = debugger_drawGlyph(destination, character);
            text++;
        }
    }
}

/* Set the text colour (gDebuggerTextColor) as an opaque RGBA5551 pixel. */
void debugger_setTextColor(u8 red, u8 green, u8 blue) {
    gDebuggerTextColor = ((red & 0xF8) << 8) | ((green & 0xF8) << 3) | ((blue & 0xF8) >> 2) | 1;
}

/* Fill the pixel rectangle (left, top)..(right, bottom), inclusive, with the text colour. */
void debugger_fillRect(s16 left, s16 top, s16 right, s16 bottom) {
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
    pixel = (u16 *)debugger_getTextAddr(0);
    pixel += left + top * gDebuggerFbWidth;
    right -= left;
    bottom -= top;
    while (bottom > 0) {
        for (count = right; count > 0; count--) {
            *pixel++ = gDebuggerTextColor;
        }
        bottom--;
        pixel += gDebuggerFbWidth - right;
    }
}

/*
 * Draw one 8x8 glyph of the font (D_16003CE0, from ' ' up) at destination in the text colour
 * on the background pixel 1; returns the position of the next character.
 */
u8 *debugger_drawGlyph(u8 *destination, u8 character) {
    u16 *pixel;
    u16 color;
    u32 column;
    u16 bits;
    u32 row;
    const u8 *glyph;
    s32 code;
    u16 pixelValue;

    pixel = (u16 *)destination;
    color = gDebuggerTextColor;
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
        pixel += gDebuggerFbWidth - 8;
    }
    return destination + 16;
}

/*
 * Framebuffer address of a text position (row * 32 + column) in the current framebuffer
 * (gFramebuffers[gDebuggerFbIndex]), whose width in pixels is gDebuggerFbWidth. Text rows are 8 scanlines
 * apart when the width is 292 and 10 otherwise; the text area starts 2 scanlines and
 * 8 pixels in.
 */
u8 *debugger_getTextAddr(s32 position) {
    s32 row = position & 0xFFE0;
    s32 offset = row;

    if (gDebuggerFbWidth != 292) {
        offset = (row >> 2) + row;
    }
    offset = (offset >> 2) * (gDebuggerFbWidth << 1);
    offset += (position & 0x1F) * 16;
    offset += gDebuggerFbWidth * 4;
    offset += 16;
    return (u8 *)gFramebuffers[gDebuggerFbIndex] + offset;
}

/* Clear the current framebuffer to pixel value 1 (215 scanlines at width 292, 264 otherwise). */
void debugger_clearScreen(void) {
    u32 *cursor = (u32 *)gFramebuffers[gDebuggerFbIndex];
    s32 lines;
    u32 *end;

    if (gDebuggerFbWidth == 292) {
        lines = 215;
    } else {
        lines = 264;
    }
    end = (gDebuggerFbWidth >> 1) * lines + cursor;
    while (cursor < end) {
        cursor[0] = 0x10001;
        cursor[1] = 0x10001;
        cursor[2] = 0x10001;
        cursor[3] = 0x10001;
        cursor += 4;
    }
}

s32 debugger_returnArg(s32 value) {
    return value;
}

/* Three sets of VI registers (not referenced): a PAL and an MPAL 320-pixel mode and a 292-pixel mode */
static u32 gDebuggerViRegs[3][20] = {
    { 0x1E000000, 0x140, 0x404233A, 0x271, 0x150C69, 0xC6F0C6E, 0x800300, 0x200, 0, 0x280, 0x400, 0x5F0239, 0x9026B, 2, 0x280, 0x400, 0x5F0239, 0x9026B, 2, 0 },
    { 0x1E000000, 0x140, 0x4651E39, 0x20D, 0x40C11, 0xC190C1A, 0x6C02EC, 0x200, 0, 0x280, 0x400, 0x2501FF, 0xE0204, 2, 0x280, 0x400, 0x2501FF, 0xE0204, 2, 0 },
    { 0x3E000000, 0x124, 0x3E52239, 0x20D, 0xC15, 0xC150C15, 0x8C02D5, 0x200, 0, 0x248, 0x400, 0x3D01E8, 0xE0204, 2, 0x248, 0x400, 0x3D01E8, 0xE0204, 2, 0 },
};
