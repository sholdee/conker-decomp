#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A data-only unit: the two base spawn tables. They are not data of the debugger. The ROM has them after
 * the part that boot_runFaultDebugger loads (which ends at D_16004960), from the next 16-byte ROM position
 * (D_1A33F0) up to the compressed assets. level_loadSpawnTable (game/Level/SpawnTable/spawntable.c) reads
 * one of them from the ROM into the start of gSpawnRecords, before the level's own records. This project
 * links them at the end of the debugger segment; nothing uses their addresses there.
 *
 * Each record: { flags, unk2, interaction_state, type, unk5, x, y, z, angle, unkD, unkE, unkF, unk10, unk11,
 * unk12, unk13, unk14, pad, objFlags, unk1C, unk1D, pad, xz_scale, y_scale, unique_id, partSet, unk2A, unk2C,
 * keepTime, unk2F }.
 */

/* The base records of most levels: ROM D_1A33F0 .. D_1A34E0. */
const SpawnRecord D_16004968[5] = {
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xB8, 0, 0, 0, 0, { 0 },
      0x44, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 1, 0, 0, 0, 0xFF, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xB8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 2, 0, 0, 0, 0xFF, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xB8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 3, 0, 0, 0, 0xFF, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xB8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 4, 0, 0, 0, 0xFF, 0 },
    { 1, 1, 2, 0x35, 0, 0, 0, 0, 0, 0x9D, 0x83, 0x3B, 0x88, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 5, 0, 0, 0, 0, 0 },
};

/* The base records of levels 26, 36, 43, 45, 48, 51, 52 and 63: ROM D_1A34E0 .. compressed_ROM_START. */
const SpawnRecord D_16004A58[16] = {
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x44, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 1, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 2, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 3, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 4, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 5, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 6, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 7, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 8, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 9, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 10, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 11, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 12, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 13, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 14, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 15, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0xA8, 0, 0, 0, 0, { 0 },
      0x400044, 0xFF, 0xFF, { 0 }, 0.5f, 0.5f, 16, 0, 0, 0, 0, 0 },
};
