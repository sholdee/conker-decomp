#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare the globals: IDO lays out .bss in the order it first
 * sees each name. */
AllocatorFreeBlock *gHeapLargestFree; // largest free heap block
AllocatorBlock *gHeapFirstBlock;     // first heap block (all blocks, address order)
AllocatorFreeBlock *gHeapFreeHead; // free list head (address order)
AllocatorFreeBlock *gHeapFreeTail; // free list tail
static s32 gHeapLastAllocSize;
static s32 gHeapLastAllocTag;
static s32 gHeapLastAllocAlign;
static s32 gHeapLastAllocFromEnd;
static s32 gHeapLastAllocMode;

#include "functions.h"
#include "variables.h"

/*
 * Heap globals:
 *   gHeapFirstBlock  first block of the heap (all blocks, address order)
 *   gHeapFreeHead  head of the free list (address order)
 *   gHeapFreeTail  tail of the free list
 *   gHeapLargestFree  largest free block, gHeapLargestFreeSize its size
 *   gHeapLastAllocSize..gHeapLastAllocMode  arguments of the last heap_alloc call
 */

static s32 gHeapLargestFreeSize = 0x10000000; // size of the largest free block

/* Allocates size bytes from the front of the heap; see heap_alloc. */
void *allocate_memory(s32 size, s32 tag, s32 align, s32 mode) {
    return heap_alloc(size, tag, align, FALSE, mode);
}

/*
 * Allocates a block with a size-byte payload and the given tag, aligned by class
 * align. With fromEnd FALSE the first fitting free block is split at its start;
 * otherwise the last fitting one is split at its end. mode 1 refuses when the
 * largest free block is under 0x7800 bytes; mode 0 treats running out of memory
 * as fatal. Returns the payload address, or 0.
 */
void *heap_alloc(s32 size, s32 tag, s32 align, s32 fromEnd, s32 mode) {
    AllocatorBlock *block;
    AllocatorFreeBlock *freeBlock;
    u32 pad; // matching: unused stack slot in the original frame
    u32 freeSize;
    AllocatorBlock *following;
    u32 remainder;
    AllocatorBlock *prev;
    AllocatorFreeBlock *nextFree;
    AllocatorFreeBlock *prevFree;
    s32 alignOffset;
    s32 alignMask;
    // per alignment class: the payload offset to round from, and the rounding mask
    s16 offsets[5] = { 3, 7, 15, 63, 0x1FFF };
    s16 masks[5] = { ~3, ~7, ~15, ~63, ~0x1FFF };
    OSIntMask intMask;

    gHeapLastAllocSize = size;
    gHeapLastAllocTag = tag;
    gHeapLastAllocAlign = align;
    gHeapLastAllocFromEnd = fromEnd;
    gHeapLastAllocMode = mode;
    if (align == 2) {
        size = (size + 15) & ~15;
    }
    if (size < 8) {
        size = 8;
    }
    size = (size + 3) & ~3;
    alignOffset = offsets[align];
    alignMask = masks[align];
    if (mode == 1 && gHeapLargestFreeSize < 0x7800) {
        return NULL;
    }

    intMask = osSetIntMask(1);
    if (!fromEnd) {
        freeBlock = gHeapFreeHead;
    } else {
        freeBlock = gHeapFreeTail;
    }
    // Find a free block that still fits size bytes once its payload is aligned.
    while (TRUE) {
        if (freeBlock == NULL) {
            osSetIntMask(intMask);
            if (mode == 0) {
                D_8003C8E0 = 0x0C000042;
                func_150AD770();
            }
            return NULL;
        }
        block = (AllocatorBlock *)((u32)((u8 *)freeBlock + alignOffset + sizeof(AllocatorBlock)) & alignMask);
        freeSize = freeBlock->header.taggedSize;
        if ((u32)((u8 *)freeBlock + freeSize + sizeof(AllocatorBlock)) >= (u32)block + size) {
            break;
        }
        if (!fromEnd) {
            freeBlock = freeBlock->nextFree;
        } else {
            freeBlock = freeBlock->prevFree;
        }
    }

    // block is the aligned payload so far; step back to its header.
    if (!fromEnd) {
        block--;
        following = (AllocatorBlock *)((u32)block + size + sizeof(AllocatorBlock));
        remainder = ((u32)freeBlock + freeSize) - ((u32)block + size);
    } else {
        block = (AllocatorBlock *)(((((u32)freeBlock + freeSize) - size + sizeof(AllocatorBlock)) & alignMask) -
                                   sizeof(AllocatorBlock));
        following = freeBlock->header.next;
        remainder = (u32)block - (u32)freeBlock;
    }
    prev = freeBlock->header.prev;
    nextFree = freeBlock->nextFree;
    prevFree = freeBlock->prevFree;

    if (!fromEnd) {
        if (remainder >= sizeof(AllocatorFreeBlock)) {
            // The tail of the free block stays free: link it in its place.
            following->next = freeBlock->header.next;
            following->taggedSize = remainder - sizeof(AllocatorBlock);
            if (following->next != NULL) {
                following->next->prev = following;
            }
            remainder = 0;
            ((AllocatorFreeBlock *)following)->nextFree = nextFree;
            ((AllocatorFreeBlock *)following)->prevFree = prevFree;
            if (nextFree != NULL) {
                nextFree->prevFree = (AllocatorFreeBlock *)following;
            } else {
                gHeapFreeTail = (AllocatorFreeBlock *)following;
            }
            if (prevFree != NULL) {
                prevFree->nextFree = (AllocatorFreeBlock *)following;
            } else {
                gHeapFreeHead = (AllocatorFreeBlock *)following;
            }
        } else {
            // Too small to keep: the whole free block is used.
            following = freeBlock->header.next;
            if (nextFree != NULL) {
                nextFree->prevFree = prevFree;
            } else {
                gHeapFreeTail = prevFree;
            }
            if (prevFree != NULL) {
                prevFree->nextFree = nextFree;
            } else {
                gHeapFreeHead = nextFree;
            }
        }
        // Any alignment gap before block goes to the previous block.
        if (prev == NULL) {
            gHeapFirstBlock = block;
        } else {
            prev->next = block;
            prev->taggedSize = (prev->taggedSize & 0xFF000000) | ((u32)block - (u32)prev - sizeof(AllocatorBlock));
        }
        block->next = following;
        block->prev = prev;
        block->taggedSize = (tag << 24) | (remainder + size);
        if (following != NULL) {
            following->prev = block;
        }
    } else if (remainder >= sizeof(AllocatorFreeBlock)) {
        // Carve block off the end; the free block keeps the front.
        block->next = freeBlock->header.next;
        block->prev = &freeBlock->header;
        block->taggedSize =
            (tag << 24) | ((u32)((u8 *)freeBlock + freeSize + sizeof(AllocatorBlock)) - (u32)block - sizeof(AllocatorBlock));
        if (block->next != NULL) {
            block->next->prev = block;
        }
        freeBlock->header.next = block;
        freeBlock->header.taggedSize = remainder - sizeof(AllocatorBlock);
    } else {
        // Too small to keep: the whole free block becomes block.
        block = (AllocatorBlock *)((((u32)freeBlock + alignOffset + sizeof(AllocatorBlock)) & alignMask) -
                                   sizeof(AllocatorBlock));
        block->next = following;
        block->prev = prev;
        block->taggedSize =
            (tag << 24) | ((u32)((u8 *)freeBlock + freeSize + sizeof(AllocatorBlock)) - (u32)block - sizeof(AllocatorBlock));
        if (nextFree != NULL) {
            nextFree->prevFree = prevFree;
        } else {
            gHeapFreeTail = prevFree;
        }
        if (prevFree != NULL) {
            prevFree->nextFree = nextFree;
        } else {
            gHeapFreeHead = nextFree;
        }
        if (prev == NULL) {
            gHeapFirstBlock = block;
        } else {
            prev->next = block;
            prev->taggedSize = (prev->taggedSize & 0xFF000000) | ((u32)block - (u32)prev - sizeof(AllocatorBlock));
        }
        if (block->next != NULL) {
            block->next->prev = block;
        }
        if (block->prev != NULL) {
            block->prev->next = block;
        }
    }

    if (freeBlock == gHeapLargestFree) {
        heap_findLargestFree();
    }
    osSetIntMask(intMask);
    return block + 1;
}

/* Frees the block whose payload is ptr, merging it with free neighbours. */
void heap_free(void *ptr) {
    AllocatorFreeBlock *block;
    AllocatorFreeBlock *neighbor;
    AllocatorFreeBlock *cursor;
    s32 merged;
    volatile OSIntMask mask; // matching: IDO keeps mask in a register otherwise; the target spills it to the stack

    if (ptr != NULL) {
        cursor = (AllocatorFreeBlock *)((AllocatorBlock *)ptr - 1);
        block = cursor;
        merged = FALSE;
        mask = osSetIntMask(1);
        neighbor = (AllocatorFreeBlock *)cursor->header.prev;
        // matching: clears only the tag byte (sb); a masked u32 store emits lw/and/sw
        ((u8 *)&cursor->header.taggedSize)[0] = 0;
        if (neighbor != NULL && (neighbor->header.taggedSize >> 24) == 0) {
            neighbor->header.next = cursor->header.next;
            neighbor->header.taggedSize += cursor->header.taggedSize + sizeof(AllocatorBlock);
            if (neighbor->header.next != NULL) {
                neighbor->header.next->prev = &neighbor->header;
            }
            block = neighbor;
            merged = TRUE;
        }
        neighbor = (AllocatorFreeBlock *)block->header.next;
        if (neighbor != NULL && (neighbor->header.taggedSize >> 24) == 0) {
            block->header.next = neighbor->header.next;
            block->header.taggedSize += neighbor->header.taggedSize + sizeof(AllocatorBlock);
            if (block->header.next != NULL) {
                block->header.next->prev = &block->header;
            }
            block->nextFree = neighbor->nextFree;
            if (block->nextFree != NULL) {
                block->nextFree->prevFree = block;
            }
            cursor = neighbor->prevFree;
            if (cursor == NULL) {
                gHeapFreeHead = block;
                block->prevFree = NULL;
            } else if (cursor != block) {
                block->prevFree = cursor;
                if (block->prevFree != NULL) {
                    block->prevFree->nextFree = block;
                }
            }
            merged = TRUE;
        }
        if (!merged) {
            // Insert into the address-ordered free list.
            cursor = gHeapFreeHead;
            if (cursor == NULL) {
                block->nextFree = NULL;
                block->prevFree = NULL;
                gHeapFreeHead = block;
            } else if ((u32)block < (u32)cursor) {
                block->nextFree = cursor;
                block->prevFree = NULL;
                cursor->prevFree = block;
                gHeapFreeHead = block;
            } else {
                for (;;) {
                    if (cursor->nextFree == NULL) {
                        block->nextFree = NULL;
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
        if (block->nextFree == NULL) {
            gHeapFreeTail = block;
        }
        if ((u32)gHeapLargestFreeSize < block->header.taggedSize) {
            gHeapLargestFreeSize = block->header.taggedSize;
            gHeapLargestFree = block;
        }
        osSetIntMask(mask);
    }
}

/* Frees every tag-2 block and counts tags 3 and 4 down by one (so they expire on later passes). */
void heap_expireBlocks(void) {
    u32 tag;
    AllocatorBlock *block;
    OSIntMask mask;

    mask = osSetIntMask(1);
    for (block = gHeapFirstBlock; block != NULL; block = block->next) {
        tag = block->taggedSize >> 24;
        if (tag == 2) {
            heap_free(block + 1);
        } else if (tag == 3 || tag == 4) {
            block->taggedSize = ((tag - 1) << 24) | (block->taggedSize & 0xFFFFFF);
        }
    }
    osSetIntMask(mask);
}

/* Frees every block tagged 1-4. */
void heap_freeTaggedBlocks(void) {
    u32 tag;
    AllocatorBlock *block;
    OSIntMask mask;

    mask = osSetIntMask(1);
    block = gHeapFirstBlock;
    text_clearQueue();
    for (; block != NULL; block = block->next) {
        tag = block->taggedSize >> 24;
        if (tag == 1 || tag == 2 || tag == 3 || tag == 4) {
            heap_free(block + 1);
        }
    }
    osSetIntMask(mask);
}

/* Replaces the tag of the allocated block whose payload is ptr. */
void heap_setTag(void *ptr, u32 tag) {
    OSIntMask mask;
    AllocatorBlock *block;

    mask = osSetIntMask(1);
    // matching: integer subtraction; pointer arithmetic folds the -0xC into the load/store offsets
    block = (AllocatorBlock *)((u32)ptr - sizeof(AllocatorBlock));
    block->taggedSize = (block->taggedSize & 0xFFFFFF) | (tag << 24);
    osSetIntMask(mask);
}

/* Rescans the free list for the largest free block, caching it and its size. */
void heap_findLargestFree(void) {
    AllocatorFreeBlock *block;
    AllocatorFreeBlock *largest;
    s32 largestSize;

    for (block = gHeapFreeHead, largestSize = 0; block != NULL; block = block->nextFree) {
        if (largestSize < (s32)block->header.taggedSize) {
            largestSize = block->header.taggedSize;
            largest = block;
        }
    }

    gHeapLargestFree = largest;
    gHeapLargestFreeSize = largestSize;
}
