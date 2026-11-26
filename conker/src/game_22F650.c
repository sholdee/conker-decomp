#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A data-only unit after game_179F30 in the link. Its .rodata (0x24A920) follows game_179F30's literals
 * and jump tables, and it is front-end data (byte lists, const records), which IDO would place before
 * the literals of one unit; its last record ends at 0x24AA94, padded to 0x24AAA0. Its .data (0x22F650)
 * starts on a 16-byte boundary after game_179F30's handler tables and their padding, and ends at
 * 0x22F718, padded to 0x22F720. It holds the attachment tables of game_179F30.c: the spawn-callback
 * slot lists and the attachment type tree.
 */

/* Spawn-callback slot lists (indices into D_8008AA70 of game_179F30), one per level slot, indexed
   by the object's unique slot (see func_1514D15C). */
static const u8 D_800A5E60[] = { 0 };
static const u8 D_800A5E64[] = { 1, 2, 8, 11 };
static const u8 D_800A5E68[] = { 3, 7, 7, 7, 7, 4, 5, 6, 9 };
static const u8 D_800A5E74[] = { 0 };
static const u8 D_800A5E78[] = { 12 };
static const u8 D_800A5E7C[] = { 16 };
static const u8 D_800A5E80[] = { 14 };
static const u8 D_800A5E84[] = { 15, 31 };
static const u8 D_800A5E88[] = { 0 };
static const u8 D_800A5E8C[] = { 21 };
static const u8 D_800A5E90[] = { 24 };
static const u8 D_800A5E94[] = { 25 };
static const u8 D_800A5E98[] = { 26 };
static const u8 D_800A5E9C[] = { 27 };
static const u8 D_800A5EA0[] = { 32 };
static const u8 D_800A5EA4[] = { 33 };
static const u8 D_800A5EA8[] = { 34 };
static const u8 D_800A5EAC[] = { 35 };
static const u8 D_800A5EB0[] = { 37 };
static const u8 D_800A5EB4[] = { 40 };
static const u8 D_800A5EB8[] = { 44 };
static const u8 D_800A5EBC[] = { 57 };

/* Spawn callback by level slot and by object id slot; 0 is none (see func_1514D15C). */
const u8 D_800A5EC0[24] = { 0 };
const u8 D_800A5ED8[28] = {
    0x0A, 0x0D, 0x11, 0x12, 0x13, 0x14, 0x16, 0x1C, 0x1D, 0x1E, 0x24, 0x26, 0x27, 0x29,
    0x2A, 0x2B, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
};

/* Attachment types, one per D_8008ABE8 entry: message handler, destroy handler (indices into
   D_8008AB64 and D_8008AB58 of game_179F30, -1 none), first child and next sibling (-1 none). */
static const AttachmentType D_800A5EF4 = { 7, 0, -1, -1 }; /* type 27 */
static const AttachmentType D_800A5EFC = { 7, 0, -1, 27 }; /* type 26 */
static const AttachmentType D_800A5F04 = { 6, 0, -1, 26 }; /* type 25 */
static const AttachmentType D_800A5F0C = { 0, 2, -1, 25 }; /* type 24 */
static const AttachmentType D_800A5F14 = { 0, 0, -1, -1 }; /* type 23 */
static const AttachmentType D_800A5F1C = { 0, 0, -1, 23 }; /* type 22 */
static const AttachmentType D_800A5F24 = { 0, 0, -1, 22 }; /* type 21 */
static const AttachmentType D_800A5F2C = { 1, 0, 9, -1 }; /* type 8 */
static const AttachmentType D_800A5F34 = { 1, 0, -1, -1 }; /* type 9 */
static const AttachmentType D_800A5F3C = { 1, 0, -1, 10 }; /* type 10 */
static const AttachmentType D_800A5F44 = { 0, 0, -1, 19 }; /* type 18 */
static const AttachmentType D_800A5F4C = { 3, 1, 12, 16 }; /* type 11 */
static const AttachmentType D_800A5F54 = { 0, 0, 17, 18 }; /* type 16 */
static const AttachmentType D_800A5F5C = { 0, 0, -1, -1 }; /* type 17 */
static const AttachmentType D_800A5F64 = { 0, 0, -1, 20 }; /* type 19 */
static const AttachmentType D_800A5F6C = { 5, 0, -1, 21 }; /* type 20 */
static const AttachmentType D_800A5F74 = { 3, 0, -1, -1 }; /* type 15 */
static const AttachmentType D_800A5F7C = { 3, 1, 14, 15 }; /* type 12 */
static const AttachmentType D_800A5F84 = { 4, 1, -1, -1 }; /* type 13 */
static const AttachmentType D_800A5F8C = { 3, 1, -1, 13 }; /* type 14 */
static const AttachmentType D_800A5F94 = { 1, 0, -1, 8 }; /* type 7 */
static const AttachmentType D_800A5F9C = { 2, 0, -1, 7 }; /* type 6 */
static const AttachmentType D_800A5FA4 = { 1, 0, -1, -1 }; /* type 4 */
static const AttachmentType D_800A5FAC = { 1, 0, -1, 4 }; /* type 3 */
static const AttachmentType D_800A5FB4 = { 1, 0, 6, -1 }; /* type 5 */
static const AttachmentType D_800A5FBC = { 1, 0, 3, 5 }; /* type 2 */
static const AttachmentType D_800A5FC4 = { 1, 0, 2, 11 }; /* type 1 */
static const AttachmentType D_800A5FCC = { 0, -1, 1, -1 }; /* type 0 */

/* The slot list of each level slot. */
const u8 *D_8008AB90[] = {
    D_800A5E64, D_800A5E68, D_800A5E74, D_800A5E60, D_800A5E78, D_800A5E7C,
    D_800A5E80, D_800A5E84, D_800A5E88, D_800A5E8C, D_800A5E90, D_800A5E94,
    D_800A5E98, D_800A5E9C, D_800A5EA0, D_800A5EA4, D_800A5EA8, D_800A5EAC,
    D_800A5EB0, D_800A5EB4, D_800A5EB8, D_800A5EBC,
};

/* The attachment type tree, by type. */
const AttachmentType *D_8008ABE8[] = {
    &D_800A5FCC, &D_800A5FC4, &D_800A5FBC, &D_800A5FAC, &D_800A5FA4, &D_800A5FB4,
    &D_800A5F9C, &D_800A5F94, &D_800A5F2C, &D_800A5F34, &D_800A5F3C, &D_800A5F4C,
    &D_800A5F7C, &D_800A5F84, &D_800A5F8C, &D_800A5F74, &D_800A5F54, &D_800A5F5C,
    &D_800A5F44, &D_800A5F64, &D_800A5F6C, &D_800A5F24, &D_800A5F1C, &D_800A5F14,
    &D_800A5F0C, &D_800A5F04, &D_800A5EFC, &D_800A5EF4,
};
