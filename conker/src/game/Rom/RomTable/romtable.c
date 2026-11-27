#include <ultra64.h>

#include "conker/boot.h"
#include "conker/collision.h"
#include "conker/heap.h"
#include "conker/rom.h"

#include "macros.h"
#include "libc/stdarg.h"

void func_1502AAF0(void) {
}

void func_1502AAF8(s32 arg0) {
}

/* Most-recently-used cache of ROM table entries; the newest is at the end. */
typedef struct {
    u32 romAddr;  /* address of the entry in ROM, with bit 31 set */
    s32 lastUsed; /* value of gRomCacheTick when the entry was last read */
    RomTableEntry entry;
} RomTableCacheLine;

static u32 gRomCacheTick;                   /* bumped on every cache miss */
static RomTableCacheLine gRomTableCache[16]; /* the cache, oldest line first */

/* Drops the oldest `count` cache lines and appends `count` entries read from `romAddr` onwards. */
void rom_fillTableCache(s32 count, RomTableEntry *entries, s32 tick, u32 romAddr) {
    u32 n;
    u32 i;

    n = count; // matching: using the parameter directly allocates registers differently
    if (n != 0) {
        bcopy(&gRomTableCache[n], gRomTableCache, (16 - n) * sizeof(RomTableCacheLine));
    }

    for (i = 16 - n; i < 16; i++) {
        gRomTableCache[i].entry = *entries++;
        gRomTableCache[i].lastUsed = tick;
        gRomTableCache[i].romAddr = romAddr;
        romAddr += sizeof(RomTableEntry);
    }
}

/*
 * Looks up entry `index` of the ROM table at `table` and returns its offset, storing its info word in
 * *info. Hits move to the newest cache line; misses DMA the entry and the one after it into the cache.
 */
s32 rom_getTableEntry(u8 *table, s32 index, s32 *info) {
    u8 buf[0x38];
    u32 dmaAddr;
    u32 dmaSize;
    RomTableEntry *dmaBuf;
    s32 offset;
    RomTableEntry *entries;
    u32 i;
    u32 j;
    RomTableCacheLine line;

    table += index * sizeof(RomTableEntry);
    table = (u8 *)((u32)table | 0x80000000);

    for (i = 0; i < 16; i++) {
        if ((u32)table == gRomTableCache[i].romAddr) {
            line = gRomTableCache[i];
            for (j = i; j < 15; j++) {
                gRomTableCache[j] = gRomTableCache[j + 1];
            }
            gRomTableCache[15] = line;
            gRomTableCache[15].lastUsed = gRomCacheTick;
            *info = gRomTableCache[15].entry.info;
            return gRomTableCache[15].entry.offset;
        }
    }

    gRomCacheTick++;
    dmaAddr = (u32)table & 0x7FFFFFF0;
    dmaSize = (((u32)table & 0xE) + 0x1F) & ~0xF;
    dmaBuf = (RomTableEntry *)((u32)buf & ~0xF);
    rom_read(dmaAddr, dmaBuf, dmaSize, 1);
    entries = (RomTableEntry *)((u8 *)dmaBuf + ((u32)table & 0xF));
    offset = entries[0].offset;
    *info = entries[0].info;
    rom_fillTableCache(2, entries, gRomCacheTick, (u32)table);
    return offset;
}

/*
 * DMAs `count` entries of the ROM table at `table`, starting at entry `index`, into `buf` (16-byte
 * aligned within it) and rebases each entry's offset onto the table. Returns the first entry.
 */
RomTableEntry *rom_loadTableEntries(u8 *table, u8 *buf, s32 index, s32 count) {
    u32 i;
    s32 romAddr;
    u8 *dst;
    RomTableEntry *entries;

    index *= sizeof(RomTableEntry);
    romAddr = (s32)table + index;
    dst = (u8 *)(((s32)buf + 8) & ~0xF);
    rom_read(romAddr & ~0xF, dst, ((romAddr & 0xE) + count * 8 + 0xF) & ~0xF, 1);
    entries = (RomTableEntry *)((u32)dst + (romAddr & 0xF));
    for (i = 0; i < count; i++) {
        entries[i].offset += (u32)table;
    }
    return entries;
}

/*
 * Follows `depth` table indices (the variadic arguments) down from the root ROM table. Returns the
 * ROM address reached, or 0 if an entry is missing, and stores the last entry's size in *size.
 */
u8 *rom_findPath(s32 *size, s32 depth, ...) {
    va_list args;
    u8 *addr;
    s32 index;
    s32 info;

    info = 1;
    addr = assets_offsets_table_ROM_START;
    va_start(args, depth);

    for (; depth != 0; depth--) {
        index = va_arg(args, s32);
        if (info != 0) {
            addr += rom_getTableEntry(addr, index, &info);
        }
        info &= 0xFFFFFFF;
    }

    if (size != NULL) {
        *size = info & 0xFFFFFFF;
    }

    if (info == 0) {
        return NULL;
    }
    return addr;
}

/*
 * Follows `depth - 1` table indices from `table` (the root table if NULL), then loads `count` entries
 * of the table reached, starting at the last index, into `buf` (see rom_loadTableEntries).
 */
RomTableEntry *rom_loadSubTable(u8 *table, s32 count, u8 *buf, u32 depth, ...) {
    va_list args;
    s32 index;
    s32 info;
    RomTableEntry *entries;

    info = 1;
    entries = NULL;
    if (table == NULL) {
        table = assets_offsets_table_ROM_START;
    }

    va_start(args, depth);
    for (; depth > 1; depth--) {
        index = va_arg(args, s32);
        if (info != 0) {
            table += rom_getTableEntry(table, index, &info);
        }
        info &= 0xFFFFFFF;
    }

    index = va_arg(args, s32);
    if (info != 0) {
        entries = rom_loadTableEntries(table, buf, index, count);
    }

    return entries;
}
/*
 * Loads the ROM data at `romAddr` described by the table info word `info` into `dst`, decompressing
 * it if compressed. A nonzero `maxSize` caps the size read. Returns the size loaded (0 if out of memory).
 */
s32 rom_loadInto(u8 *romAddr, void *dst, u32 info, u32 maxSize) {
    u32 size;
    void *packed;
    s32 unpackedSize;

    size = ((info & 0x0FFFFFFF) + 1) & ~1;
    if (maxSize != 0 && maxSize < size) {
        size = maxSize;
    }

    if ((info & 0x70000000) == 0x10000000) {
        packed = allocate_memory(size, 1, 2, 2);
        if (packed == NULL) {
            return 0;
        }

        rom_read((s32)romAddr, packed, (size + 0xF) & ~0xF, 1);
        /* The first word of compressed data is its unpacked size. */
        unpackedSize = *(s32 *)packed & 0x7FFFFFFF;
        if ((size = boot_decompress(packed, dst, gDecompressWork)) != unpackedSize) {
            gDebugStatusCode = 0x0C000036;
            debug_fatalTrap();
        }
        heap_free(packed);
        return size;
    }

    rom_read((s32)romAddr, dst, (size + 0xF) & ~0xF, 1);
    return size;
}
/*
 * Allocates a buffer and loads the ROM data at `romAddr` described by `info` into it, decompressing
 * it into a second buffer if compressed. Stores the size loaded in *size and returns the buffer, or
 * NULL on failure.
 */
void *rom_loadAlloc(u8 *romAddr, s32 info, u32 *size) {
    u32 loadSize;
    void *packed;
    void *data;

    loadSize = ((info & 0x0FFFFFFF) + 1) & ~1;
    data = packed = allocate_memory(loadSize, 1, 2, 2);
    if (packed == NULL) {
        return NULL;
    }
    rom_read((s32)romAddr, packed, (loadSize + 0xF) & ~0xF, 1);
    if ((info & 0x70000000) == 0x10000000) {
        *size = *(s32 *)packed & 0x7FFFFFFF;
        if (*size != 0) {
            data = NULL;
            if (*size < 1000000) {
                data = allocate_memory(*size, 1, 2, 2);
                if (data != NULL) {
                    loadSize = boot_decompress(packed, data, gDecompressWork);
                } else {
                    loadSize = 0;
                }
            } else {
                loadSize = 0;
            }
        } else {
            data = NULL;
            loadSize = 0;
        }
        heap_free(packed);
    }
    *size = loadSize;
    return data;
}
/*
 * Turns a table of `count` entries loaded into RAM into pointers: masks each info word down to its
 * size and rebases each offset onto the table (missing entries become NULL). A `count` of 0 means the
 * table ends at the first entry with bit 31 of its info set. Returns the entry count.
 */
s32 rom_relocateTable(RomTableEntry *table, s32 count) {
    s32 i;

    if (count == 0) {
        while ((table[count++].info & 0x80000000) == 0) {
        }
    }
    for (i = 0; i < count; i++) {
        table[i].info &= 0x0FFFFFFF;
        if (table[i].offset == -1 || table[i].info == 0) {
            table[i].offset = 0;
        } else {
            table[i].offset += (s32)table;
        }
    }
    return count;
}

/*
 * Follows `depth` table indices from the root ROM table and loads the data reached into a new buffer
 * (see rom_loadAlloc). Stores the size loaded in *size if non-NULL; returns the buffer or NULL.
 */
void *rom_loadPath(u32 *size, s32 depth, ...) {
    u32 *found;
    u8 *addr;
    va_list args;
    u32 localSize;
    void *data;
    s32 info;
    s32 index;

    found = &localSize;
    if (size != NULL) {
        found = size;
    }

    *found = 1;
    addr = assets_offsets_table_ROM_START;
    va_start(args, depth);

    for (; depth != 0; depth--) {
        index = va_arg(args, s32);
        if (*found != 0) {
            addr += rom_getTableEntry(addr, index, &info);
        }
        *found = info & 0xFFFFFFF;
    }

    if (*found != 0) {
        data = rom_loadAlloc(addr, info, found);
    } else {
        data = NULL;
    }

    return data;
}
/*
 * Like rom_loadPath, but the data loaded is a table of `count` entries (0 = terminated by bit 31),
 * which is turned into pointers by rom_relocateTable. Stores the entry count in *countOut if non-NULL.
 */
void *rom_loadPathTable(u32 *size, s32 count, s32 *countOut, s32 depth, ...) {
    u32 *found;
    u8 *addr;
    u32 localSize;
    va_list args;
    RomTableEntry *table;
    s32 info;
    s32 index;

    found = &localSize;
    if (size != NULL) {
        found = size;
    }

    *found = 1;
    addr = assets_offsets_table_ROM_START;
    va_start(args, depth);

    for (; depth != 0; depth--) {
        index = va_arg(args, s32);
        if (*found != 0) {
            addr += rom_getTableEntry(addr, index, &info);
        }
        *found = info & 0xFFFFFFF;
    }

    if (*found != 0) {
        table = rom_loadAlloc(addr, info, found);
        if (*found != 0 && table != NULL) {
            count = rom_relocateTable(table, count);
        } else {
            count = 0;
        }
        if (countOut != NULL) {
            *countOut = count;
        }
    } else {
        table = NULL;
    }

    return table;
}
/*
 * Follows `depth` table indices from the root ROM table and loads the data reached into a new buffer
 * (see rom_loadAlloc), stored in *data (NULL if missing). Returns the size loaded.
 */
u32 rom_loadPathTo(void *data, s32 depth, ...) {
    va_list args;
    u8 *addr;
    s32 index;
    u32 size;
    s32 info;

    size = 1;
    addr = assets_offsets_table_ROM_START;
    va_start(args, depth);

    for (; depth != 0; depth--) {
        index = va_arg(args, s32);
        if (size != 0) {
            addr += rom_getTableEntry(addr, index, &info);
        }
        size = info & 0xFFFFFFF;
    }

    if (size != 0) {
        *(void **)data = rom_loadAlloc(addr, info, &size);
    } else {
        *(void **)data = NULL;
    }

    return size;
}

/*
 * Follows `depth` table indices from the root ROM table and loads the data reached into `dst` (see
 * rom_loadInto), at most `maxSize` bytes if nonzero. Returns the size loaded, or 0 if missing.
 */
s32 rom_loadPathInto(void *dst, u32 maxSize, s32 depth, ...) {
    u8 *addr;
    va_list args;
    s32 index;
    s32 size;
    s32 info;

    size = 1;
    addr = assets_offsets_table_ROM_START;
    va_start(args, depth);

    for (; depth != 0; depth--) {
        index = va_arg(args, s32);
        if (size != 0) {
            addr += rom_getTableEntry(addr, index, &info);
        }
        size = info & 0xFFFFFFF;
    }

    if (size != 0) {
        size = rom_loadInto(addr, dst, info, maxSize);
    }

    return size;
}

/*
 * Follows `depth` table indices from the root ROM table and returns the size the data reached takes
 * once loaded: its stored size, or for compressed data the header word read from ROM. 0 if missing.
 */
s32 rom_getPathSize(s32 depth, ...) {
    u8 *addr;
    va_list args;
    s32 size;
    s32 index;
    s32 info;
    u64 buf[3];
    s32 *header;

    size = 1;
    addr = assets_offsets_table_ROM_START;
    va_start(args, depth);

    for (; depth != 0; depth--) {
        index = va_arg(args, s32);
        if (size != 0) {
            addr += rom_getTableEntry(addr, index, &info);
        }
        size = info & 0xFFFFFFF;
    }

    if (size != 0) {
        size = ((info & 0xFFFFFFF) + 1) & ~1;
        if ((info & 0x70000000) == 0x10000000) {
            /* DMA needs a 16-byte aligned buffer. */
            header = (s32 *)&buf[0];
            if ((s32)header & 8) {
                header = (s32 *)&buf[1];
            }
            rom_read((s32)addr, header, 0x10, 1);
            size = header[0];
        }
    }

    return size;
}
