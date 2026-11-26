#ifndef CONKER_HEAP_H
#define CONKER_HEAP_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "heap": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init_3920.c
 *   init_3930.c
 *   init_3BD0.c
 *   init_3C40.c
 */

/* init_3920.c */
void heap_clearExpansionPak(void);
extern u8 gExpansionPak; // an Expansion Pak is present (8MB of RDRAM)

/* init_3930.c */
void heap_initMemoryLayout(void);
extern u32 gRdpOutputBuffer; // RDP output buffer address (u64 *), stored as an integer
extern u32 gRdpOutputBufferEnd; // its end address
extern u32 gHeapEnd;
extern u32 D_8003809C;

/* init_3BD0.c */
void heap_init(void);

/* init_3C40.c */
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

#endif
