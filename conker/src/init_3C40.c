#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/*
 * Heap block header. Every block (free or allocated) starts with one; the
 * payload handed out by func_10003C6C starts right after it. The top byte of
 * taggedSize is the block's tag (0 = free), the low 24 bits its payload size.
 */
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

s32 allocate_memory(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return func_10003C6C(arg0, arg1, arg2, 0, arg3);
}

typedef struct MemBlock {
    struct MemBlock *next;
    struct MemBlock *prev;
    u32 size;
    struct MemBlock *nextFree;
    struct MemBlock *prevFree;
} MemBlock;

extern s32 D_800380D0;
struct S10 { s16 v[5]; };
extern struct S10 D_8002AC34;
extern struct S10 D_8002AC40;

s32 func_10003C6C(s32 size, s32 tag, s32 align, s32 fromEnd, s32 arg4) {
    MemBlock *blk;
    MemBlock *cur;
    u32 pad;
    u32 sz;
    MemBlock *newFree;
    u32 rem;
    MemBlock *prev;
    MemBlock *nf;
    MemBlock *pf;
    s32 off;
    s32 mask;
    struct S10 offs;
    struct S10 masks;
    OSIntMask intMask;

    offs = D_8002AC34;
    masks = D_8002AC40;
    D_800380C0 = size;
    D_800380C4 = tag;
    D_800380C8 = align;
    D_800380CC = fromEnd;
    D_800380D0 = arg4;
    if (align == 2) {
        size = (size + 0xF) & ~0xF;
    }
    if (size < 8) {
        size = 8;
    }
    size = (size + 3) & ~3;
    off = offs.v[align];
    mask = masks.v[align];
    if ((arg4 == 1) && (D_8002AC30 < 0x7800)) {
        return 0;
    }
    intMask = osSetIntMask(1);
    if (fromEnd == 0) {
        cur = (MemBlock *)D_800380B8;
    } else {
        cur = (MemBlock *)D_800380BC;
    }
    while (1) {
        if (cur == NULL) {
            osSetIntMask(intMask);
            if (arg4 == 0) {
                D_8003C8E0 = 0x0C000042;
                func_150AD770();
            }
            return 0;
        }
        blk = (MemBlock *)((u32)((u8 *)cur + off + 0xC) & mask);
        sz = cur->size;
        if ((u32)((u8 *)cur + sz + 0xC) >= (u32)blk + size) {
            break;
        }
        if (fromEnd == 0) {
            cur = cur->nextFree;
        } else {
            cur = cur->prevFree;
        }
    }
    if (fromEnd == 0) {
        blk = (MemBlock *)((u32)blk - 0xC);
        newFree = (MemBlock *)((u32)blk + size + 0xC);
        rem = ((u32)cur + sz) - ((u32)blk + size);
    } else {
        blk = (MemBlock *)(((((u32)cur + sz) - size + 0xC) & mask) - 0xC);
        newFree = cur->next;
        rem = (u32)blk - (u32)cur;
    }
    prev = cur->prev;
    nf = cur->nextFree;
    pf = cur->prevFree;
    if (fromEnd == 0) {
        if (rem >= 0x14) {
            newFree->next = cur->next;
            newFree->size = rem - 0xC;
            if (newFree->next != NULL) {
                newFree->next->prev = newFree;
            }
            rem = 0;
            newFree->nextFree = nf;
            newFree->prevFree = pf;
            if (nf != NULL) {
                nf->prevFree = newFree;
            } else {
                D_800380BC = (s32 *)newFree;
            }
            if (pf != NULL) {
                pf->nextFree = newFree;
            } else {
                D_800380B8 = (struct54 *)newFree;
            }
        } else {
            newFree = cur->next;
            if (nf != NULL) {
                nf->prevFree = pf;
            } else {
                D_800380BC = (s32 *)pf;
            }
            if (pf != NULL) {
                pf->nextFree = nf;
            } else {
                D_800380B8 = (struct54 *)nf;
            }
        }
        if (prev == NULL) {
            D_800380B4 = (struct54 *)blk;
        } else {
            prev->next = blk;
            prev->size = (prev->size & 0xFF000000) | ((u32)blk - (u32)prev - 0xC);
        }
        blk->next = newFree;
        blk->prev = prev;
        blk->size = (tag << 24) | (rem + size);
        if (newFree != NULL) {
            newFree->prev = blk;
        }
    } else if (rem >= 0x14) {
        blk->next = cur->next;
        blk->prev = cur;
        blk->size = (tag << 24) | ((u32)((u8 *)cur + sz + 0xC) - (u32)blk - 0xC);
        if (blk->next != NULL) {
            blk->next->prev = blk;
        }
        cur->next = blk;
        cur->size = rem - 0xC;
    } else {
        blk = (MemBlock *)((((u32)cur + off + 0xC) & mask) - 0xC);
        blk->next = newFree;
        blk->prev = prev;
        blk->size = (tag << 24) | ((u32)((u8 *)cur + sz + 0xC) - (u32)blk - 0xC);
        if (nf != NULL) {
            nf->prevFree = pf;
        } else {
            D_800380BC = (s32 *)pf;
        }
        if (pf != NULL) {
            pf->nextFree = nf;
        } else {
            D_800380B8 = (struct54 *)nf;
        }
        if (prev == NULL) {
            D_800380B4 = (struct54 *)blk;
        } else {
            prev->next = blk;
            prev->size = (prev->size & 0xFF000000) | ((u32)blk - (u32)prev - 0xC);
        }
        if (blk->next != NULL) {
            blk->next->prev = blk;
        }
        if (blk->prev != NULL) {
            blk->prev->next = blk;
        }
    }
    if ((struct54 *)cur == D_800380B0) {
        func_1000440C();
    }
    osSetIntMask(intMask);
    return (s32)blk + 0xC;
}
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

/* Frees every tag-2 block and counts tags 3 and 4 down by one (so they expire on later passes). */
void func_10004250(void) {
    u32 tag;
    AllocatorBlock *block;
    OSIntMask mask;

    mask = osSetIntMask(1);
    for (block = (AllocatorBlock *)D_800380B4; block != NULL; block = block->next) {
        tag = block->taggedSize >> 24;
        if (tag == 2) {
            func_10004074(block + 1);
        } else if (tag == 3 || tag == 4) {
            block->taggedSize = ((tag - 1) << 24) | (block->taggedSize & 0xFFFFFF);
        }
    }
    osSetIntMask(mask);
}

/* Frees every block tagged 1-4. */
void func_10004308(void) {
    u32 tag;
    AllocatorBlock *block;
    OSIntMask mask;

    mask = osSetIntMask(1);
    block = (AllocatorBlock *)D_800380B4;
    func_15042D50();
    for (; block != NULL; block = block->next) {
        tag = block->taggedSize >> 24;
        if (tag == 1 || tag == 2 || tag == 3 || tag == 4) {
            func_10004074(block + 1);
        }
    }
    osSetIntMask(mask);
}

/* Replaces the tag of the allocated block whose payload is ptr. */
void func_100043B4(s32 *ptr, u32 tag) {
    OSIntMask mask;
    AllocatorBlock *block;

    mask = osSetIntMask(1);
    // matching: integer subtraction; pointer arithmetic folds the -0xC into the load/store offsets
    block = (AllocatorBlock *)((u32)ptr - sizeof(AllocatorBlock));
    block->taggedSize = (block->taggedSize & 0xFFFFFF) | (tag << 24);
    osSetIntMask(mask);
}

/* Rescans the free list for the largest free block, caching it and its size. */
void func_1000440C(void) {
    AllocatorFreeBlock *block;
    AllocatorFreeBlock *largest;
    s32 largestSize;

    for (block = (AllocatorFreeBlock *)D_800380B8, largestSize = 0; block != NULL; block = block->nextFree) {
        if (largestSize < (s32)block->header.taggedSize) {
            largestSize = block->header.taggedSize;
            largest = block;
        }
    }

    D_800380B0 = (struct54 *)largest;
    D_8002AC30 = largestSize;
}
