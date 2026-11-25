#ifndef CONKER_LEVEL_H
#define CONKER_LEVEL_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "level": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_2DDF0.c
 *   game_305D0.c
 *   game_30E90.c
 *   game_360A0.c
 *   game_3BA70.c
 *   game/done/game_3BFD0.c
 *   game/done/game_3BFE0.c
 *   game/done/game_3C230.c
 *   game/done/game_3C3D0.c
 *   game/done/game_3C6C0.c
 *   game_3C740.c
 *   game_3CE80.c
 *   game_3D2E0.c
 *   game_3D5C0.c
 *   game/done/game_3D6F0.c
 *   game/done/game_3D770.c
 *   game/done/game_3D780.c
 *   game_3D9A0.c
 *   game/done/game_3DB30.c
 *   game/done/game_3DB50.c
 *   game/done/game_3DB60.c
 *   game/done/game_3DB80.c
 *   game_3DC30.c
 *   game_3DF10.c
 *   game/done/game_3E460.c
 *   game/done/game_3E5A0.c
 *   game/done/game_3E620.c
 *   game/done/game_3E7E0.c
 *   game/done/game_3E810.c
 *   game_3E890.c
 *   game/done/game_3EFB0.c
 *   game/done/game_3F0F0.c
 *   game/done/game_3F120.c
 *   game_3F1F0.c
 *   game/done/game_3F3D0.c
 *   game/done/game_3F450.c
 *   game/done/game_3F4D0.c
 *   game/done/game_3F560.c
 *   game_3F670.c
 *   game/done/game_3FBD0.c
 *   game/done/game_3FC20.c
 *   game/done/game_3FC30.c
 *   game_40490.c
 *   game_43D00.c
 *   game_FDD70.c
 *   game_15D6E0.c
 *   game/done/game_1A6300.c
 */

/* game_30E90.c */
typedef void (*ObjectFunc)();

/* A level object placement as stored in the level's object lists (0x44 bytes each). */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 rotX;
    /* 0x08 */ s16 rotY;
    /* 0x0A */ s16 rotZ;
    /* 0x0C */ Gfx *displayList; // NULL: load model `modelId`; 1 or 2: use level model `modelId`
    /* 0x10 */ s32 modelId;
    /* 0x14 */ union {
        u32 index;         // into D_80088C90 when below 0xFFFF
        ObjectFunc func;   // otherwise the update function itself
    } behavior;
    /* 0x18 */ void *data;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ f32 scaleX;
    /* 0x24 */ f32 scaleY;
    /* 0x28 */ f32 scaleZ;
    /* 0x2C */ u8 unk4C;
    /* 0x2D */ u8 shadowSize;
    /* 0x2E */ u16 cullRadiusXZ;
    /* 0x30 */ u16 cullRadiusY;
    /* 0x32 */ u8 flags;
    /* 0x33 */ u8 id;            // 0: assign a free one
    /* 0x34 */ u8 inactive;
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ s32 material;
    /* 0x3C */ u8 state;
    /* 0x3D */ u8 pad3D;
    /* 0x3E */ u16 effectFlags;
    /* 0x40 */ u16 drawDistance;
    /* 0x42 */ u8 pad42[2];
} ObjectSpawn; // size 0x44

#endif
