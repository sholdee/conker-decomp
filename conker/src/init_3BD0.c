#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_10003BD0(void) {
    AllocatorFreeBlock *heap;

    // the whole heap starts as one free block
    D_800380B4 = (AllocatorBlock *)D_800E9D10;
    D_800380B4->next = NULL;
    D_800380B4->prev = NULL;
    D_800380B4->taggedSize = D_80038098 - (u32)D_800E9D10 - sizeof(AllocatorFreeBlock);
    ((AllocatorFreeBlock *)D_800380B4)->nextFree = NULL;
    ((AllocatorFreeBlock *)D_800380B4)->prevFree = NULL;
    heap = (AllocatorFreeBlock *)D_800380B4;
    D_800380BC = heap;
    D_800380B8 = D_800380BC;
    D_800380B0 = heap;
}
