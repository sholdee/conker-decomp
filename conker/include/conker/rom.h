#ifndef CONKER_ROM_H
#define CONKER_ROM_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "rom": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init_4470.c
 *   game_57FA0.c
 */

/* One 8-byte entry of the ROM asset table at assets_offsets_table_ROM_START (game_57FA0.c). */
typedef struct {
    s32 offset; /* added to the table base to reach the next level */
    s32 info;   /* bits 0-27: size (0 = missing), bits 28-30: compression (1 = compressed),
                 * bit 31: last entry (tables loaded into RAM, see func_1502B4A8) */
} RomTableEntry;

/* init_4470.c */
void func_10004470(void);
void func_10004514(s32 devAddr, void *dramAddr, u32 size, s32 blocking);
void func_10004674(void);
void func_100046E4(s32 devAddr, void *dramAddr, u32 size);
void func_1000480C(u32 romAddr, void *dest, u32 size);

/* game_57FA0.c */
void func_1502AAF0(void);
void func_1502AAF8(s32 arg0);
void func_1502AB04(s32 count, RomTableEntry *entries, s32 tick, u32 romAddr);
s32 func_1502AC88(u8 *table, s32 index, s32 *info);
RomTableEntry *func_1502AF04(u8 *table, u8 *buf, s32 index, s32 count);
u8 *func_1502B020(s32 *size, s32 depth, ...);
RomTableEntry *func_1502B110(u8 *table, s32 count, u8 *buf, u32 depth, ...);
s32 func_1502B224(u8 *romAddr, void *dst, u32 info, u32 maxSize);
void *func_1502B350(u8 *romAddr, s32 info, u32 *size);
s32 func_1502B4A8(RomTableEntry *table, s32 count);
void *func_1502B5C8(u32 *size, s32 depth, ...); /* loads ROM table data */
void *func_1502B6BC(u32 *size, s32 count, s32 *countOut, s32 depth, ...);
u32 func_1502B7F0(void *data, s32 depth, ...); /* data: where to store the loaded block's address */
s32 func_1502B8E0(void *dst, u32 maxSize, s32 depth, ...);
s32 func_1502B9B4(s32 depth, ...);

#endif
