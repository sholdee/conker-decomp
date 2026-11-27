#include <ultra64.h>

/* This file's .bss, defined before the headers declare the globals: IDO lays out .bss in the order it first
 * sees each name. It starts the .bss of the program (0x8002D4B0..0x80043B40, which boot_entry clears). */
u64 gIdleThreadStack[0x400 / sizeof(u64)];  // idle thread stack
u64 gMainThreadStack[0x3C00 / sizeof(u64)]; // main thread stack, with gMainThreadStackTop (the debugger checks all 0x4000 bytes)
u64 gMainThreadStackTop[0x400 / sizeof(u64)];  // the top of the main thread stack; the boot code starts with sp here
static OSThread gIdleThread;           // idle thread
static u8 D_80031A60[0x80];           // not referenced
OSThread gMainThread;                  // main thread
static u8 gPageFaultStack[0xE88];          // not referenced in C: init/Boot/Paging/pagefault.s runs on the stack below gPageFaultStackTop
u32 gPageFaultStackTop[6];                    // [0]: the stack pointer that init/Boot/Paging/pagefault.s saves before it switches stacks
static u8 gChunk0OffsetsBuf[0x800];          // chunk0 offset table (16-byte aligned through gChunk0Offsets)
static u8 gPageDmaBufferStorage[0xDB8];          // buffer for init/Boot/Paging/pagefault.s (16-byte aligned through gPageDmaBuffer)
u8 gPageInflateWork[0x1410];                // init/Boot/Paging/pagefault.s: work area of the decompressor
u8 *gPageDmaBuffer;                       // gPageDmaBufferStorage, aligned to 16 bytes
s32 *gChunk0Offsets;                      // chunk0 offsets (gChunk0OffsetsBuf, aligned to 16 bytes)

#include "functions.h"
#include "variables.h"
#include "macros.h"

/* ROM range of the compressed game code (game/Prop/PropMover/propmover.c reads it). */
RomFile gRomFiles[2] = {
    { (s32)&game_us_rzip_ROM_START, (s32)&debugger_us_ROM_START },
};
u8 gPageFaultsDisabled = 0;
u8 gPadInitDone = 0;
u16 *gFramebuffers[2] = { NULL, NULL }; // the two framebuffers

/** Boot entry: clear BSS, set up the CPU/FPU and start the idle thread (boot_idleThread). */
void boot_entry(void) {
    bzero(&gIdleThreadStack, (s32)gPageSlots - (s32)&gIdleThreadStack); // clear BSS
    boot_invalidateTlbEntries(1, 31);
    osInitialize();
    __osSetSR(__osGetSR() | SR_CU1 | SR_FR);
    __osSetFpcCsr(FPCSR_FS);
    osCreateThread(&gIdleThread, 1, boot_idleThread, NULL, &gIdleThreadStack[0x80], 5);
    osStartThread(&gIdleThread);
}

/** Idle thread: start the main thread (boot_mainThread), then drop to priority 0 and spin. */
void boot_idleThread(void *arg) {
    rom_init(); // create message queues
    osCreateThread(&gMainThread, 3, boot_mainThread, arg, &gMainThreadStackTop[0x80], 10);
    if ((gResetPending == 0) && (D_80000310 == 6105)) { // osCicId: CIC-NUS-6105
        osStartThread(&gMainThread);
    }
    osSetThreadPri(&gIdleThread, OS_PRIORITY_IDLE);
    for (;;) {
    }
}

/**
 * Main thread: clear the heap, set up framebuffers and the VI manager, load and decompress the
 * boot data from ROM, load the chunk0 offset table, then hand over to the game.
 */
void boot_mainThread(void *arg) {
    u32 size; // compressed size, later the offset-table entry count
    u32 blockCount;
    s32 compressedRomAddr;
    void *compressed;
    s32 romBase;
    s32 clearSize;
    s32 i;
    u32 framebufferSize;

    sched_startPreNmiThread();
    clearSize = 0x80400000 - (s32)gPageSlots;
    if (D_8000030C == 0) { // osResetType: cold reset
        bzero(gPageSlots, clearSize);
    } else {
        bzero(D_800E9D10, 0x80400000 - (s32)D_800E9D10);
    }
    osInvalICache(gPageSlots, clearSize);
    osInvalDCache(gPageSlots, clearSize);
    heap_clearExpansionPak();
    heap_initMemoryLayout();
    heap_init();
    boot_initInflateFixedTables();
    framebufferSize = 0x1ECC0;
    gFramebuffers[0] = heap_alloc(framebufferSize, 0xFF, 3, 1, 0);
    gFramebuffers[1] = heap_alloc(framebufferSize, 0xFF, 3, 1, 0);
    osCreateViManager(OS_PRIORITY_VIMGR);

    romBase = (s32)&game_us_rzip_ROM_START;
    rom_read(romBase, gLevelAssetLists, 16, 1);
    compressedRomAddr = (s32)gLevelAssetLists[0] + romBase; // the segment header, not yet a list pointer
    size = (u32)&debugger_us_ROM_START - compressedRomAddr;
    compressed = allocate_memory(size, 1, 2, 0);
    rom_read(compressedRomAddr, compressed, size, 1);
    boot_decompress(compressed, gLevelAssetLists, D_8003809C);
    heap_free(compressed);

    gPageDmaBuffer = (u8 *)ALIGN16(&gPageDmaBufferStorage);
    blockCount = ((u32)&D_151FA130 - (u32)&music_init + 0xFFF) >> 12;
    gChunk0Offsets = (s32 *)ALIGN16(&gChunk0OffsetsBuf);
    size = blockCount + 1;
    // matching: 16-byte round-up written as (x + 0xF | 0xF) ^ 0xF; `& ~0xF` or `* 4` change the code
    rom_read(romBase + 4, gChunk0Offsets, ((((blockCount + 2) << 2) + 0xF) | 0xF) ^ 0xF, 1);
    for (i = 0; i < size; i++) {
        gChunk0Offsets[i] = (gChunk0Offsets[i] ^ 0x8039CCCA) + romBase;
    }
    gPagePool = 0;
    boot_initPaging(0xEB);
    libc_clearPageSlots();
    boot_resetPageBitmap();
    boot_runGame();
}
