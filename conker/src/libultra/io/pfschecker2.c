#include <ultra64.h>
#include "controller.h"

s32 osPfsChecker2(OSPfs *pfs) {
    int j;
    s32 ret;
    __OSInodeUnit next_page;
    __OSInode checked_inode;
    __OSInode tmp_inode;
    __OSDir tmp_dir;
    __OSInodeUnit file_next_node[16];
    __OSInodeCache cache;
    int fixed = 0;
    u8 bank;
    s32 cc;
    s32 cl;
    int offset;

    ret = __osCheckId(pfs);
    if (ret == PFS_ERR_NEW_PACK) {
        ret = __osGetId(pfs);
    }
    if (ret != 0) {
        return ret;
    }
    ERRCK(corrupted_init(pfs, &cache));
    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir));
        if (tmp_dir.company_code != 0 && tmp_dir.game_code != 0) {
            next_page = tmp_dir.start_page;
            cc = 0;
            cl = 0;
            bank = 255;
            while (next_page.ipage >= pfs->inode_start_page && next_page.inode_t.bank < pfs->banks &&
                   next_page.inode_t.page > 0 && next_page.inode_t.page < 128) {
                if (bank != next_page.inode_t.bank) {
                    bank = next_page.inode_t.bank;
                    ret = __osPfsRWInode(pfs, &tmp_inode, OS_READ, bank);
                    if (ret != 0 && ret != PFS_ERR_INCONSISTENT) {
                        return ret;
                    }
                }
                if ((cc = corrupted(pfs, next_page, &cache) - cl) != 0) {
                    break;
                }
                cl = 1;
                next_page = tmp_inode.inode_page[next_page.inode_t.page];
            }
            if (cc == 0 && next_page.ipage == 1) {
                continue;
            }
            tmp_dir.company_code = 0;
            tmp_dir.game_code = 0;
            tmp_dir.start_page.ipage = 0;
            tmp_dir.status = 0;
            tmp_dir.data_sum = 0;
            SET_ACTIVEBANK_TO_ZERO;
            ERRCK(__osContRamWrite(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir, FALSE));
            fixed++;
        } else if (tmp_dir.company_code != 0 || tmp_dir.game_code != 0) {
            tmp_dir.company_code = 0;
            tmp_dir.game_code = 0;
            tmp_dir.start_page.ipage = 0;
            tmp_dir.status = 0;
            tmp_dir.data_sum = 0;
            SET_ACTIVEBANK_TO_ZERO;
            ERRCK(__osContRamWrite(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir, FALSE));
            fixed++;
        }
    }
    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir));
        if (tmp_dir.company_code != 0 && tmp_dir.game_code != 0 &&
            tmp_dir.start_page.ipage >= (u16)pfs->inode_start_page) {
            file_next_node[j].ipage = tmp_dir.start_page.ipage;
        } else {
            file_next_node[j].ipage = 0;
        }
    }
    for (bank = 0; bank < pfs->banks; bank++) {
        ret = __osPfsRWInode(pfs, &tmp_inode, OS_READ, bank);
        if (ret != 0 && ret != PFS_ERR_INCONSISTENT) {
            return ret;
        }
        if (bank > 0) {
            offset = 1;
        } else {
            offset = pfs->inode_start_page;
        }
        for (j = 0; j < offset; j++) {
            checked_inode.inode_page[j].ipage = tmp_inode.inode_page[j].ipage;
        }
        for (; j < 128; j++) {
            checked_inode.inode_page[j].ipage = 3;
        }
        for (j = 0; j < pfs->dir_size; j++) {
            while (file_next_node[j].inode_t.bank == bank &&
                   file_next_node[j].ipage >= (u16)pfs->inode_start_page) {
                u8 pp = file_next_node[j].inode_t.page;
                file_next_node[j] = checked_inode.inode_page[pp] = tmp_inode.inode_page[pp];
            }
        }
        ERRCK(__osPfsRWInode(pfs, &checked_inode, OS_WRITE, bank));
    }
    if (fixed) {
        pfs->status |= PFS_CORRUPTED;
    } else {
        pfs->status &= ~PFS_CORRUPTED;
    }
    return 0;
}

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
