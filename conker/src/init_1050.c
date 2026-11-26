#include <ultra64.h>

/* This file's .bss, defined before the headers declare the globals: IDO lays out .bss in the order it first
 * sees each name. It starts the .bss of the program (0x8002D4B0..0x80043B40, which func_10001050 clears). */
u64 D_8002D4B0[0x400 / sizeof(u64)];  // idle thread stack
u64 D_8002D8B0[0x3C00 / sizeof(u64)]; // main thread stack, with D_800314B0 (the debugger checks all 0x4000 bytes)
u64 D_800314B0[0x400 / sizeof(u64)];  // the top of the main thread stack; the boot code starts with sp here
static OSThread D_800318B0;           // idle thread
static u8 D_80031A60[0x80];           // not referenced
OSThread D_80031AE0;                  // main thread
static u8 D_80031C90[0xE88];          // not referenced in C: init_5C2C.s runs on the stack below D_80032B18
u32 D_80032B18[6];                    // [0]: the stack pointer that init_5C2C.s saves before it switches stacks
static u8 D_80032B30[0x800];          // chunk0 offset table (16-byte aligned through D_800354FC)
static u8 D_80033330[0xDB8];          // buffer for init_5C2C.s (16-byte aligned through D_800354F8)
u8 D_800340E8[0x1410];                // init_5C2C.s: work area of the decompressor
u8 *D_800354F8;                       // D_80033330, aligned to 16 bytes
s32 *D_800354FC;                      // chunk0 offsets (D_80032B30, aligned to 16 bytes)

#include "functions.h"
#include "variables.h"
#include "macros.h"

extern u8 debugger_us_ROM_START;
extern u8 game_us_rzip_ROM_START;

/* ROM range of the compressed game code (game_143DE0.c reads it). */
RomFile D_8002AAD0[2] = {
    { (s32)&game_us_rzip_ROM_START, (s32)&debugger_us_ROM_START },
};
u8 D_8002AAE0 = 0;
u8 D_8002AAE4 = 0;
u16 *D_8002AAE8[2] = { NULL, NULL }; // the two framebuffers

/** Boot entry: clear BSS, set up the CPU/FPU and start the idle thread (func_100010F8). */
void func_10001050(void) {
    bzero(&D_8002D4B0, (s32)D_80043B40 - (s32)&D_8002D4B0); // clear BSS
    func_100061F8(1, 31);
    osInitialize();
    __osSetSR(__osGetSR() | SR_CU1 | SR_FR);
    __osSetFpcCsr(FPCSR_FS);
    osCreateThread(&D_800318B0, 1, func_100010F8, NULL, &D_8002D4B0[0x80], 5);
    osStartThread(&D_800318B0);
}

/** Idle thread: start the main thread (func_10001194), then drop to priority 0 and spin. */
void func_100010F8(void *arg) {
    func_10004470(); // create message queues
    osCreateThread(&D_80031AE0, 3, func_10001194, arg, &D_800314B0[0x80], 10);
    if ((D_8002AC5C == 0) && (D_80000310 == 6105)) { // osCicId: CIC-NUS-6105
        osStartThread(&D_80031AE0);
    }
    osSetThreadPri(&D_800318B0, OS_PRIORITY_IDLE);
    for (;;) {
    }
}

extern u8 D_151FA130; // end of the chunk0 code segment (starts at music_init)
/**
 * Main thread: clear the heap, set up framebuffers and the VI manager, load and decompress the
 * boot data from ROM, load the chunk0 offset table, then hand over to the game.
 */
void func_10001194(void *arg) {
    u32 size; // compressed size, later the offset-table entry count
    u32 blockCount;
    s32 compressedRomAddr;
    void *compressed;
    s32 romBase;
    s32 clearSize;
    s32 i;
    u32 framebufferSize;

    func_10005218();
    clearSize = 0x80400000 - (s32)D_80043B40;
    if (D_8000030C == 0) { // osResetType: cold reset
        bzero(D_80043B40, clearSize);
    } else {
        bzero(D_800E9D10, 0x80400000 - (s32)D_800E9D10);
    }
    osInvalICache(D_80043B40, clearSize);
    osInvalDCache(D_80043B40, clearSize);
    func_10003920();
    func_10003930();
    func_10003BD0();
    func_1000709C();
    framebufferSize = 0x1ECC0;
    D_8002AAE8[0] = func_10003C6C(framebufferSize, 0xFF, 3, 1, 0);
    D_8002AAE8[1] = func_10003C6C(framebufferSize, 0xFF, 3, 1, 0);
    osCreateViManager(OS_PRIORITY_VIMGR);

    romBase = (s32)&game_us_rzip_ROM_START;
    func_10004514(romBase, D_80082B20, 16, 1);
    compressedRomAddr = (s32)D_80082B20[0] + romBase; // the segment header, not yet a list pointer
    size = (u32)&debugger_us_ROM_START - compressedRomAddr;
    compressed = allocate_memory(size, 1, 2, 0);
    func_10004514(compressedRomAddr, compressed, size, 1);
    func_10006240(compressed, D_80082B20, D_8003809C);
    func_10004074(compressed);

    D_800354F8 = (u8 *)ALIGN16(&D_80033330);
    blockCount = ((u32)&D_151FA130 - (u32)&music_init + 0xFFF) >> 12;
    D_800354FC = (s32 *)ALIGN16(&D_80032B30);
    size = blockCount + 1;
    // matching: 16-byte round-up written as (x + 0xF | 0xF) ^ 0xF; `& ~0xF` or `* 4` change the code
    func_10004514(romBase + 4, D_800354FC, ((((blockCount + 2) << 2) + 0xF) | 0xF) ^ 0xF, 1);
    for (i = 0; i < size; i++) {
        D_800354FC[i] = (D_800354FC[i] ^ 0x8039CCCA) + romBase;
    }
    D_8003BE74 = 0;
    func_10005B04(0xEB);
    func_10001420();
    func_10005BE0();
    func_15007830();
}
