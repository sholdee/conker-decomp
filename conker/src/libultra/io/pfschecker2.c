#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "controller.h"


#pragma GLOBAL_ASM("asm/nonmatchings/libultra/io/pfschecker2/osPfsChecker2.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/io/pfschecker2/corrupted_init2.s")

typedef struct {
    /* 0x000 */ __OSInodeUnit inode_page[128];
    /* 0x100 */ u8 bank;
    /* 0x101 */ u8 map[1];
} __OSInodeCache2;

s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);

s32 corrupted2(OSPfs *pfs, __OSInodeUnit fpage, __OSInodeCache2 *cache) {
    int j;
    int offset;
    int cnt = 0;
    u8 bank;
    int start;
    s32 ret = 0;

    offset = fpage.inode_t.page / 4 + (fpage.inode_t.bank % 8) * 32;
    for (bank = 0; bank < pfs->banks; bank++) {
        if (bank > 0) {
            start = 1;
        } else {
            start = pfs->inode_start_page;
        }
        if (bank == fpage.inode_t.bank || (cache->map[offset] & (1 << (bank % 8)))) {
            if (bank != cache->bank) {
                ret = __osPfsRWInode(pfs, (__OSInode *)cache, 0, bank);
                if (ret != 0 && ret != PFS_ERR_INCONSISTENT) {
                    return ret;
                }
                cache->bank = bank;
            }
            for (j = start; cnt < 2 && j < 128; j++) {
                if (cache->inode_page[j].ipage == fpage.ipage) {
                    cnt++;
                }
            }
            if (cnt >= 2) {
                return PFS_CORRUPTED;
            }
        }
    }
    return cnt;
}
