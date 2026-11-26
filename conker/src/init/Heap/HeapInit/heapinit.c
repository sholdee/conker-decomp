#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void heap_init(void) {
    AllocatorFreeBlock *heap;

    // the whole heap starts as one free block
    gHeapFirstBlock = (AllocatorBlock *)D_800E9D10;
    gHeapFirstBlock->next = NULL;
    gHeapFirstBlock->prev = NULL;
    gHeapFirstBlock->taggedSize = gHeapEnd - (u32)D_800E9D10 - sizeof(AllocatorFreeBlock);
    ((AllocatorFreeBlock *)gHeapFirstBlock)->nextFree = NULL;
    ((AllocatorFreeBlock *)gHeapFirstBlock)->prevFree = NULL;
    heap = (AllocatorFreeBlock *)gHeapFirstBlock;
    gHeapFreeTail = heap;
    gHeapFreeHead = gHeapFreeTail;
    gHeapLargestFree = heap;
}
