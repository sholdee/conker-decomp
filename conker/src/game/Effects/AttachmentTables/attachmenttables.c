#include <ultra64.h>
#include "conker/effect.h"

/*
 * A data-only unit after game/Effects/ActorAttachments/actorattachments in the link. Its .rodata (0x24A920) follows game/Effects/ActorAttachments/actorattachments's literals
 * and jump tables, and it is front-end data (byte lists, const records), which IDO would place before
 * the literals of one unit; its last record ends at 0x24AA94, padded to 0x24AAA0. Its .data (0x22F650)
 * starts on a 16-byte boundary after game/Effects/ActorAttachments/actorattachments's handler tables and their padding, and ends at
 * 0x22F718, padded to 0x22F720. It holds the attachment tables of game/Effects/ActorAttachments/actorattachments.c: the spawn-callback
 * slot lists and the attachment type tree.
 */

/* Spawn-callback slot lists (indices into gAttachSpawnFuncs of game/Effects/ActorAttachments/actorattachments), one per level slot, indexed
   by the object's unique slot (see effect_spawnAttachments). */
static const u8 gAttachSpawnSlotList3[] = { 0 };
static const u8 gAttachSpawnSlotList0[] = { 1, 2, 8, 11 };
static const u8 gAttachSpawnSlotList1[] = { 3, 7, 7, 7, 7, 4, 5, 6, 9 };
static const u8 gAttachSpawnSlotList2[] = { 0 };
static const u8 gAttachSpawnSlotList4[] = { 12 };
static const u8 gAttachSpawnSlotList5[] = { 16 };
static const u8 gAttachSpawnSlotList6[] = { 14 };
static const u8 gAttachSpawnSlotList7[] = { 15, 31 };
static const u8 gAttachSpawnSlotList8[] = { 0 };
static const u8 gAttachSpawnSlotList9[] = { 21 };
static const u8 gAttachSpawnSlotList10[] = { 24 };
static const u8 gAttachSpawnSlotList11[] = { 25 };
static const u8 gAttachSpawnSlotList12[] = { 26 };
static const u8 gAttachSpawnSlotList13[] = { 27 };
static const u8 gAttachSpawnSlotList14[] = { 32 };
static const u8 gAttachSpawnSlotList15[] = { 33 };
static const u8 gAttachSpawnSlotList16[] = { 34 };
static const u8 gAttachSpawnSlotList17[] = { 35 };
static const u8 gAttachSpawnSlotList18[] = { 37 };
static const u8 gAttachSpawnSlotList19[] = { 40 };
static const u8 gAttachSpawnSlotList20[] = { 44 };
static const u8 gAttachSpawnSlotList21[] = { 57 };

/* Spawn callback by level slot and by object id slot; 0 is none (see effect_spawnAttachments). */
const u8 gAttachSpawnSlotsByLevel[24] = { 0 };
const u8 gAttachSpawnSlotsById[28] = {
    0x0A, 0x0D, 0x11, 0x12, 0x13, 0x14, 0x16, 0x1C, 0x1D, 0x1E, 0x24, 0x26, 0x27, 0x29,
    0x2A, 0x2B, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
};

/* Attachment types, one per gAttachmentTypes entry: message handler, destroy handler (indices into
   gAttachMessageFuncs and gAttachDestroyFuncs of game/Effects/ActorAttachments/actorattachments, -1 none), first child and next sibling (-1 none). */
static const AttachmentType gAttachmentType27 = { 7, 0, -1, -1 }; /* type 27 */
static const AttachmentType gAttachmentType26 = { 7, 0, -1, 27 }; /* type 26 */
static const AttachmentType gAttachmentType25 = { 6, 0, -1, 26 }; /* type 25 */
static const AttachmentType gAttachmentType24 = { 0, 2, -1, 25 }; /* type 24 */
static const AttachmentType gAttachmentType23 = { 0, 0, -1, -1 }; /* type 23 */
static const AttachmentType gAttachmentType22 = { 0, 0, -1, 23 }; /* type 22 */
static const AttachmentType gAttachmentType21 = { 0, 0, -1, 22 }; /* type 21 */
static const AttachmentType gAttachmentType8 = { 1, 0, 9, -1 }; /* type 8 */
static const AttachmentType gAttachmentType9 = { 1, 0, -1, -1 }; /* type 9 */
static const AttachmentType gAttachmentType10 = { 1, 0, -1, 10 }; /* type 10 */
static const AttachmentType gAttachmentType18 = { 0, 0, -1, 19 }; /* type 18 */
static const AttachmentType gAttachmentType11 = { 3, 1, 12, 16 }; /* type 11 */
static const AttachmentType gAttachmentType16 = { 0, 0, 17, 18 }; /* type 16 */
static const AttachmentType gAttachmentType17 = { 0, 0, -1, -1 }; /* type 17 */
static const AttachmentType gAttachmentType19 = { 0, 0, -1, 20 }; /* type 19 */
static const AttachmentType gAttachmentType20 = { 5, 0, -1, 21 }; /* type 20 */
static const AttachmentType gAttachmentType15 = { 3, 0, -1, -1 }; /* type 15 */
static const AttachmentType gAttachmentType12 = { 3, 1, 14, 15 }; /* type 12 */
static const AttachmentType gAttachmentType13 = { 4, 1, -1, -1 }; /* type 13 */
static const AttachmentType gAttachmentType14 = { 3, 1, -1, 13 }; /* type 14 */
static const AttachmentType gAttachmentType7 = { 1, 0, -1, 8 }; /* type 7 */
static const AttachmentType gAttachmentType6 = { 2, 0, -1, 7 }; /* type 6 */
static const AttachmentType gAttachmentType4 = { 1, 0, -1, -1 }; /* type 4 */
static const AttachmentType gAttachmentType3 = { 1, 0, -1, 4 }; /* type 3 */
static const AttachmentType gAttachmentType5 = { 1, 0, 6, -1 }; /* type 5 */
static const AttachmentType gAttachmentType2 = { 1, 0, 3, 5 }; /* type 2 */
static const AttachmentType gAttachmentType1 = { 1, 0, 2, 11 }; /* type 1 */
static const AttachmentType gAttachmentType0 = { 0, -1, 1, -1 }; /* type 0 */

/* The slot list of each level slot. */
const u8 *gAttachSpawnSlotLists[] = {
    gAttachSpawnSlotList0, gAttachSpawnSlotList1, gAttachSpawnSlotList2, gAttachSpawnSlotList3, gAttachSpawnSlotList4, gAttachSpawnSlotList5,
    gAttachSpawnSlotList6, gAttachSpawnSlotList7, gAttachSpawnSlotList8, gAttachSpawnSlotList9, gAttachSpawnSlotList10, gAttachSpawnSlotList11,
    gAttachSpawnSlotList12, gAttachSpawnSlotList13, gAttachSpawnSlotList14, gAttachSpawnSlotList15, gAttachSpawnSlotList16, gAttachSpawnSlotList17,
    gAttachSpawnSlotList18, gAttachSpawnSlotList19, gAttachSpawnSlotList20, gAttachSpawnSlotList21,
};

/* The attachment type tree, by type. */
const AttachmentType *gAttachmentTypes[] = {
    &gAttachmentType0, &gAttachmentType1, &gAttachmentType2, &gAttachmentType3, &gAttachmentType4, &gAttachmentType5,
    &gAttachmentType6, &gAttachmentType7, &gAttachmentType8, &gAttachmentType9, &gAttachmentType10, &gAttachmentType11,
    &gAttachmentType12, &gAttachmentType13, &gAttachmentType14, &gAttachmentType15, &gAttachmentType16, &gAttachmentType17,
    &gAttachmentType18, &gAttachmentType19, &gAttachmentType20, &gAttachmentType21, &gAttachmentType22, &gAttachmentType23,
    &gAttachmentType24, &gAttachmentType25, &gAttachmentType26, &gAttachmentType27,
};
