#include <ultra64.h>
#include "conker/bone.h"

/*
 * A data-only unit between game/Attach/Attachment/attachment and game/Bone/BoneCommands/bonecommands in the link: its .rodata (0x23C630) and its .data
 * (0x228CF0) each start on a 16-byte unit boundary, and the 12 zero bytes after the last list are the
 * unit's own padding (front-end data and the jump table of game/Bone/BoneCommands/bonecommands would be adjacent in one unit).
 *
 * Per-model bone remap lists: (bone, value) byte pairs terminated by 0xFF. actor_computeBones (game/Actor/ObjectUpdate/objectupdate)
 * copies the list of a model's remap set (1-based: 0 is none) into its per-bone remap buffer.
 */
static const u8 gBoneRemapList13[] = { 5, 2, 0xFF };
static const u8 gBoneRemapList02[] = { 2, 2, 0xFF };
static const u8 gBoneRemapList01[] = { 1, 2, 16, 2, 0xFF };
static const u8 gBoneRemapList03[] = { 12, 2, 13, 2, 0xFF };
static const u8 gBoneRemapList04[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList07[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList08[] = { 1, 4, 4, 5, 14, 4, 18, 4, 22, 4, 0xFF };
static const u8 gBoneRemapList05[] = { 7, 1, 12, 1, 0xFF };
static const u8 gBoneRemapList28[] = { 12, 1, 0xFF };
static const u8 gBoneRemapList06[] = { 1, 3, 0xFF };
static const u8 gBoneRemapList00[] = { 5, 2, 0xFF };
static const u8 gBoneRemapList09[] = { 1, 2, 0xFF };
static const u8 gBoneRemapList10[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList11[] = { 5, 2, 0xFF };
static const u8 gBoneRemapList12[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList14[] = { 2, 2, 3, 2, 0xFF };
static const u8 gBoneRemapList15[] = { 3, 2, 0xFF };
static const u8 gBoneRemapList16[] = { 1, 2, 0xFF };
static const u8 gBoneRemapList17[] = { 43, 2, 44, 2, 0xFF };
static const u8 gBoneRemapList18[] = { 1, 2, 0xFF };
static const u8 gBoneRemapList19[] = { 1, 2, 0xFF };
static const u8 gBoneRemapList20[] = { 13, 2, 0xFF };
static const u8 gBoneRemapList21[] = { 5, 2, 0xFF };
static const u8 gBoneRemapList22[] = { 1, 2, 3, 2, 5, 2, 0xFF };
static const u8 gBoneRemapList23[] = { 29, 2, 0xFF };
static const u8 gBoneRemapList24[] = { 2, 2, 0xFF };
static const u8 gBoneRemapList25[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList26[] = { 3, 2, 0xFF };
static const u8 gBoneRemapList27[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList29[] = { 17, 2, 0xFF };
static const u8 gBoneRemapList30[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList31[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList32[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList33[] = { 3, 2, 5, 2, 0xFF };
static const u8 gBoneRemapList43[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList34[] = { 27, 2, 0xFF };
static const u8 gBoneRemapList35[] = { 25, 2, 0xFF };
static const u8 gBoneRemapList36[] = { 3, 2, 22, 2, 26, 2, 0xFF };
static const u8 gBoneRemapList37[] = { 1, 2, 0xFF };
static const u8 gBoneRemapList38[] = { 4, 2, 5, 2, 0xFF };
static const u8 gBoneRemapList39[] = { 4, 2, 5, 2, 0xFF };
static const u8 gBoneRemapList40[] = { 4, 2, 5, 2, 0xFF };
static const u8 gBoneRemapList41[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList42[] = { 33, 2, 0xFF };
static const u8 gBoneRemapList44[] = { 5, 2, 0xFF };
static const u8 gBoneRemapList45[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList46[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList47[] = { 12, 2, 0xFF };
static const u8 gBoneRemapList48[] = { 4, 2, 5, 2, 0xFF };
static const u8 gBoneRemapList49[] = { 4, 2, 0xFF };
static const u8 gBoneRemapList50[] = { 4, 2, 0xFF };

const u8 *gBoneRemapLists[] = {
    gBoneRemapList00,
    gBoneRemapList01,
    gBoneRemapList02,
    gBoneRemapList03,
    gBoneRemapList04,
    gBoneRemapList05,
    gBoneRemapList06,
    gBoneRemapList07,
    gBoneRemapList08,
    gBoneRemapList09,
    gBoneRemapList10,
    gBoneRemapList11,
    gBoneRemapList12,
    gBoneRemapList13,
    gBoneRemapList14,
    gBoneRemapList15,
    gBoneRemapList16,
    gBoneRemapList17,
    gBoneRemapList18,
    gBoneRemapList19,
    gBoneRemapList20,
    gBoneRemapList21,
    gBoneRemapList22,
    gBoneRemapList23,
    gBoneRemapList24,
    gBoneRemapList25,
    gBoneRemapList26,
    gBoneRemapList27,
    gBoneRemapList28,
    gBoneRemapList29,
    gBoneRemapList30,
    gBoneRemapList31,
    gBoneRemapList32,
    gBoneRemapList33,
    gBoneRemapList34,
    gBoneRemapList35,
    gBoneRemapList36,
    gBoneRemapList37,
    gBoneRemapList38,
    gBoneRemapList39,
    gBoneRemapList40,
    gBoneRemapList41,
    gBoneRemapList42,
    gBoneRemapList43,
    gBoneRemapList44,
    gBoneRemapList45,
    gBoneRemapList46,
    gBoneRemapList47,
    gBoneRemapList48,
    gBoneRemapList49,
    gBoneRemapList50,
};
