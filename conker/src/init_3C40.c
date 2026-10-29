#include <ultra64.h>

#include "functions.h"
#include "variables.h"


s32 allocate_memory(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return func_10003C6C(arg0, arg1, arg2, 0, arg3);
}

#pragma GLOBAL_ASM("asm/nonmatchings/init_3C40/func_10003C6C.s")
typedef struct AllocatorBlock {
    struct AllocatorBlock *next;
    struct AllocatorBlock *prev;
    u32 taggedSize;
} AllocatorBlock;

typedef struct AllocatorFreeBlock {
    AllocatorBlock header;
    struct AllocatorFreeBlock *nextFree;
    struct AllocatorFreeBlock *prevFree;
} AllocatorFreeBlock;

void func_10004074(void *arg0) {
    AllocatorFreeBlock *block;
    AllocatorFreeBlock *neighbor;
    AllocatorFreeBlock *cursor;
    s32 merged;
    volatile s32 mask;

    if (arg0 != 0) {
        cursor = (AllocatorFreeBlock *)((AllocatorBlock *)arg0 - 1);
        block = cursor;
        merged = 0;
        mask = osSetIntMask(1);
        neighbor = (AllocatorFreeBlock *)cursor->header.prev;
        ((u8 *)&cursor->header.taggedSize)[0] = 0;
        if ((neighbor != 0) && ((neighbor->header.taggedSize >> 24) == 0)) {
            neighbor->header.next = cursor->header.next;
            neighbor->header.taggedSize += cursor->header.taggedSize + 0xC;
            if (neighbor->header.next != 0) {
                neighbor->header.next->prev = &neighbor->header;
            }
            block = neighbor;
            merged = 1;
        }
        neighbor = (AllocatorFreeBlock *)block->header.next;
        if ((neighbor != 0) && ((neighbor->header.taggedSize >> 24) == 0)) {
            block->header.next = neighbor->header.next;
            block->header.taggedSize += neighbor->header.taggedSize + 0xC;
            if (block->header.next != 0) {
                block->header.next->prev = &block->header;
            }
            block->nextFree = neighbor->nextFree;
            if (block->nextFree != 0) {
                block->nextFree->prevFree = block;
            }
            cursor = neighbor->prevFree;
            if (cursor == 0) {
                D_800380B8 = (struct54 *)block;
                block->prevFree = 0;
            } else if (cursor != block) {
                block->prevFree = cursor;
                if (block->prevFree != 0) {
                    block->prevFree->nextFree = block;
                }
            }
            merged = 1;
        }
        if (merged == 0) {
            cursor = (AllocatorFreeBlock *)D_800380B8;
            if (cursor == 0) {
                block->nextFree = 0;
                block->prevFree = 0;
                D_800380B8 = (struct54 *)block;
            } else if ((u32)block < (u32)cursor) {
                block->nextFree = cursor;
                block->prevFree = 0;
                cursor->prevFree = block;
                D_800380B8 = (struct54 *)block;
            } else {
                for (;;) {
                    if (cursor->nextFree == 0) {
                        block->nextFree = 0;
                        block->prevFree = cursor;
                        cursor->nextFree = block;
                        break;
                    }
                    if ((u32)block < (u32)cursor->nextFree) {
                        block->nextFree = cursor->nextFree;
                        block->prevFree = cursor;
                        block->nextFree->prevFree = block;
                        cursor->nextFree = block;
                        break;
                    }
                    cursor = cursor->nextFree;
                }
            }
        }
        if (block->nextFree == 0) {
            D_800380BC = (s32 *)block;
        }
        if ((u32)D_8002AC30 < block->header.taggedSize) {
            D_8002AC30 = block->header.taggedSize;
            D_800380B0 = (struct54 *)block;
        }
        osSetIntMask(mask);
    }
}

void func_10004250(void) {
    s32 temp_v0;
    u32 temp_v1;
    OSIntMask mask;
    struct54 *node;

    mask = osSetIntMask(1);

    if (node = D_800380B4) {
        do {
            temp_v1 = node->unk8;
            temp_v0 = temp_v1 >> 0x18;
            if (2 == temp_v0) {
                func_10004074(&node->unkC);
            } else if ((temp_v0 == 3) || (temp_v0 == 4)) {
                node->unk8 = ((temp_v0 - 1) << 0x18) | (temp_v1 & 0xFFFFFF);
            }
        } while(node = node->unk0);
    }
    osSetIntMask(mask);
}

void func_10004308(void) {
    u32 temp_t6;
    struct54 *node;
    OSIntMask mask;

    mask = osSetIntMask(1);
    node = D_800380B4;
    func_15042D50();

    if (node) {
        do {
            temp_t6 = node->unk8 >> 24;
            if (temp_t6 == 1 || temp_t6 == 2 || temp_t6 == 3 || temp_t6 == 4) {
                func_10004074(&node->unkC);
            }
        }
        while (node = node->unk0);
    }
    osSetIntMask(mask);
}

void func_100043B4(s32 *arg0, u32 arg1) {
    OSIntMask mask;
    struct54 *node;

    mask = osSetIntMask(1);
    node = (struct54 *)((u32)arg0 - 0xC);
    node->unk8 = (node->unk8 & 0xFFFFFF) | (arg1 << 24);
    osSetIntMask(mask);
}
// NON-MATCHING: missing an addiu
// void func_100043B4(s32 *arg0, u32 arg1) {
//     OSIntMask mask = osSetIntMask(1);
//     *(arg0 - 1) = (arg1 << 24) | *(arg0 - 1) & 0xFFFFFF;
//     osSetIntMask(mask);
// }

void func_1000440C(void) {
    struct54 *foo;
    struct54 *last_good_foo;
    s32 tmp0;

    for (foo = D_800380B8, tmp0 = NULL; foo != NULL; foo = (struct54 *)foo->unkC) {
        if (tmp0 < (s32) foo->unk8) {
            tmp0 = foo->unk8;
            last_good_foo = foo;
        }
    }

    D_800380B0 = last_good_foo;
    D_8002AC30 = tmp0;
}
