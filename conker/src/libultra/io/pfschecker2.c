#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "controller.h"


#pragma GLOBAL_ASM("asm/nonmatchings/libultra/io/pfschecker2/osPfsChecker2.s")

s32 corrupted_init2(OSPfs *pfs, __OSInodeCache *cache) {
    int i;
    int n;
    int offset;
    u8 bank;
    __OSInodeUnit tpage;
    __OSInode tmp_inode;
    int ret;

    for (i = 0; i < 256; i++) {
        cache->map[i] = 0;
    }
    cache->bank = -1;
    for (bank = 0; bank < pfs->banks; bank++) {
        if (bank > 0) {
            offset = 1;
        } else {
            offset = pfs->inode_start_page;
        }
        ret = __osPfsRWInode(pfs, &tmp_inode, OS_READ, bank);
        if (ret != 0 && ret != PFS_ERR_INCONSISTENT) {
            return ret;
        }
        for (i = offset; i < 128; i++) {
            tpage = tmp_inode.inode_page[i];
            if (tpage.ipage >= pfs->inode_start_page && tpage.inode_t.bank != bank) {
                n = (tpage.inode_t.page / 4) + ((tpage.inode_t.bank % 8) * 32);
                cache->map[n] |= (1 << (bank % 8));
            }
        }
    }
    return 0;
}

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
