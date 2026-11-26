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
void func_10003920(void);
extern u8 D_80038080; // an Expansion Pak is present (8MB of RDRAM)

/* init_3930.c */
void func_10003930(void);
extern u32 D_80038090; // RDP output buffer address (u64 *), stored as an integer
extern u32 D_80038094; // its end address
extern u32 D_80038098;
extern u32 D_8003809C;

/* init_3BD0.c */
void func_10003BD0(void);

/* init_3C40.c */
void *allocate_memory(s32 size, s32 tag, s32 align, s32 mode);
void *func_10003C6C(s32 size, s32 tag, s32 align, s32 fromEnd, s32 mode);
void func_10004074(void *ptr);
void func_10004250(void);
void func_10004308(void);
void func_100043B4(void *ptr, u32 tag);
void func_1000440C(void);
extern AllocatorFreeBlock *D_800380B0; // largest free heap block
extern AllocatorBlock *D_800380B4;     // first heap block (all blocks, address order)
extern AllocatorFreeBlock *D_800380B8; // free list head (address order)
extern AllocatorFreeBlock *D_800380BC; // free list tail

#endif
