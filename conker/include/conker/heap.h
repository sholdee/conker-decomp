#ifndef CONKER_HEAP_H
#define CONKER_HEAP_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "heap": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init/Heap/ExpansionPak/expansionpak.c
 *   init/Heap/MemoryLayout/memorylayout.c
 *   init/Heap/HeapInit/heapinit.c
 *   init/Heap/Allocator/allocator.c
 */

/* init/Heap/ExpansionPak/expansionpak.c */
void heap_clearExpansionPak(void);
extern u8 gExpansionPak; // an Expansion Pak is present (8MB of RDRAM)

/* init/Heap/MemoryLayout/memorylayout.c */
void heap_initMemoryLayout(void);
extern u32 gRdpOutputBuffer; // RDP output buffer address (u64 *), stored as an integer
extern u32 gRdpOutputBufferEnd; // its end address
extern u32 gHeapEnd;
extern u32 gDecompressWork;

/* init/Heap/HeapInit/heapinit.c */
void heap_init(void);

/* init/Heap/Allocator/allocator.c */
void *allocate_memory(s32 size, s32 tag, s32 align, s32 mode);
void *heap_alloc(s32 size, s32 tag, s32 align, s32 fromEnd, s32 mode);
void heap_free(void *ptr);
void heap_expireBlocks(void);
void heap_freeTaggedBlocks(void);
void heap_setTag(void *ptr, u32 tag);
void heap_findLargestFree(void);
extern AllocatorFreeBlock *gHeapLargestFree; // largest free heap block
extern AllocatorBlock *gHeapFirstBlock;     // first heap block (all blocks, address order)
extern AllocatorFreeBlock *gHeapFreeHead; // free list head (address order)
extern AllocatorFreeBlock *gHeapFreeTail; // free list tail

/* past the end of the game .bss (undefined_syms.us.txt) */
extern u8   D_800E9D10[]; // heap arena: from here to 0x80400000

#endif
