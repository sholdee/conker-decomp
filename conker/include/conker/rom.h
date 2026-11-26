#ifndef CONKER_ROM_H
#define CONKER_ROM_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "rom": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init/Rom/RomDma/romdma.c
 *   game/Rom/RomTable/romtable.c
 */

/* One 8-byte entry of the ROM asset table at assets_offsets_table_ROM_START (game/Rom/RomTable/romtable.c). */
typedef struct {
    s32 offset; /* added to the table base to reach the next level */
    s32 info;   /* bits 0-27: size (0 = missing), bits 28-30: compression (1 = compressed),
                 * bit 31: last entry (tables loaded into RAM, see rom_relocateTable) */
} RomTableEntry;

/* init/Rom/RomDma/romdma.c */
void rom_init(void);
void rom_read(s32 devAddr, void *dramAddr, u32 size, s32 blocking);
void rom_waitReads(void);
void rom_readLarge(s32 devAddr, void *dramAddr, u32 size);
void rom_copyUncached(u32 romAddr, void *dest, u32 size);
extern OSIoMesg gRomIoMesg;
extern OSMesgQueue gMessageQueue[3]; // completions of the blocking reads, one per game thread
extern vu8 D_8003A572; // PI interlock flags shared with the PI manager thread (init_2E50)
extern vu8 D_8003A573;
extern vu8 D_8003A575;

/* game/Rom/RomTable/romtable.c */
void func_1502AAF0(void);
void func_1502AAF8(s32 arg0);
void rom_fillTableCache(s32 count, RomTableEntry *entries, s32 tick, u32 romAddr);
s32 rom_getTableEntry(u8 *table, s32 index, s32 *info);
RomTableEntry *rom_loadTableEntries(u8 *table, u8 *buf, s32 index, s32 count);
u8 *rom_findPath(s32 *size, s32 depth, ...);
RomTableEntry *rom_loadSubTable(u8 *table, s32 count, u8 *buf, u32 depth, ...);
s32 rom_loadInto(u8 *romAddr, void *dst, u32 info, u32 maxSize);
void *rom_loadAlloc(u8 *romAddr, s32 info, u32 *size);
s32 rom_relocateTable(RomTableEntry *table, s32 count);
void *rom_loadPath(u32 *size, s32 depth, ...); /* loads ROM table data */
void *rom_loadPathTable(u32 *size, s32 count, s32 *countOut, s32 depth, ...);
u32 rom_loadPathTo(void *data, s32 depth, ...); /* data: where to store the loaded block's address */
s32 rom_loadPathInto(void *dst, u32 maxSize, s32 depth, ...);
s32 rom_getPathSize(s32 depth, ...);

#endif
