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
 *   game_22D750.c (data only, linked after game_137ED0)
 *   game_15D6E0.c
 *   game/done/game_1A6300.c
 */

/* A display list command, viewed as its opcode, the G_MOVEMEM index byte and the address word. */
typedef struct {
    /* 0x0 */ s8 opcode;
    /* 0x1 */ u8 pad1[2];
    /* 0x3 */ u8 index;
    /* 0x4 */ u32 addr;
} DlCommand;

/* game_30E90.c */
typedef void (*ObjectFunc)();

/* Defaults for objects of the second (grouped) list, indexed by model id (D_800A26C0). */
typedef struct {
    /* 0x0 */ u8 unk4C;
    /* 0x1 */ u8 shadowSize;
    /* 0x2 */ s16 cullRadiusY;
    /* 0x4 */ s16 cullRadiusXZ;
    /* 0x6 */ u16 drawDistance;
    /* 0x8 */ u8 flags;
    /* 0x9 */ u8 state;
    /* 0xA */ u16 effectFlags;
} GroupedObjectDefaults;

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

/* game_2DDF0.c */
/* (s32 arg0); declared without a prototype: game_2D4B0.c calls it without the (unused) argument */
void func_15000940();

/* game_305D0.c */
extern s32 *D_80082B20[]; // per level (LevelHeader.assetList): zero-terminated asset id list. The segment
                          // starts here; init_1050 first DMAs its 16-byte header in (word 0: compressed data offset).
extern const u16 D_80091D20[7762]; // per texture: compressed size
extern Gfx *D_800B0E00[4]; // level display lists
extern Vtx *D_800B0E10[4]; // level mesh vertices of the four level models (each display list follows its vertices)
extern VtxColorGroup **D_800B0E30; // groups of each vertex color set
extern u8 *D_800B0E34; // group count of each vertex color set
extern u8 D_800B0E38;
extern s32 D_800B0E40[4];
extern LevelModelEntry *D_800B0E50; // the current level's models
extern s32 D_800B0E58[7762]; // per texture: its address once loaded, or -1
extern u16 D_800B87A0[7762]; // per texture: size
extern s8 D_800BC448[7762]; // per texture: frames left before it is freed (0 = not loaded)
void func_15003120(s32 index, s32 base, s32 list);
void func_150031EC(s32 levelId);
s32 func_150034B4(void);
void func_15003570(void);
void func_15003668(s32 unused);
void func_150038A0(void);
s32 func_1500390C(s32 addr);
void func_150039B0(s32 arg0);
void func_150039BC(s32 addr);

/* game_30E90.c */
void func_150039E0(s32 levelId, s32 arg1);
void func_15004574(void);
void func_150045BC(void);
void func_150045C4(s32 arg0);
void func_150049A4(DlCommand *dl, s32 offset, s32 matrixOffset);
void func_15004A4C(void);
void func_15004AAC(WorldObject *obj, ObjectSpawn *spawn);
s32 func_15004BF0(s32 fromTop);
void func_15004CE0(DlCommand *dl, u32 base);

/* game_360A0.c */
s32 func_15008BF0(TriggerZone *zone);

/* game_3BA70.c */
void func_1500E5C0(void);
void func_1500E70C(s32 id);
void func_1500E738(void);
void func_1500E890(void);
void func_1500E8C0(void);
void func_1500EAA0(void);

/* game_3BFD0.c */
void func_1500EB20(void);

/* game_3BFE0.c */
void func_1500EB30(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_1500EBC4(void);

/* game_3C230.c */
void func_1500ED80(void);
void func_1500EE18(Actor *arg0, s32 arg1, s32 arg2);
void func_1500EE94(TriggerZone *zone);

/* game_3C3D0.c */
void func_1500EF20(void);

/* game_3C6C0.c */
void func_1500F210(void);

/* game_3C740.c */
void *func_1500F290(f32 x, f32 y, f32 z);
void func_1500F378(s32 x, s32 y, s32 z, s32 floorY);
void func_1500F40C(void);

/* game_3CE80.c */
void func_1500F9D0(s32 x, s32 y, s32 z, s32 floorY, s32 arg4);
void func_1500FA64(void);

/* game_3D2E0.c */
extern BounceRecord *D_800BE4E0; /* ten bouncing cursors (func_1500FE30) */
void func_1500FE30(void);
void func_1500FF9C(void);
void func_1500FFCC(void);

/* game_3D5C0.c */
void func_15010110(void);

/* game_3D6F0.c */
void func_15010240(void);

/* game_3D770.c */
void func_150102C0(void);

/* game_3D780.c */
void func_150102D0(void);
void func_150103E0(void);

/* game_3D9A0.c */
void func_150104F0(void);
void func_15010538(Actor *arg0);
void func_15010600(void);

/* game_3DB30.c */
void func_15010680(void);

/* game_3DB50.c */
void func_150106A0(void);

/* game_3DB60.c */
void func_150106B0(void);

/* game_3DB80.c */
s32 func_150106D0(TriggerZone *zone);

/* game_3DC30.c */
void func_15010780(void);
void func_15010880(void);
void func_150109D0(void);

/* game_3DF10.c */
void func_15010A60(void);

/* game_3E460.c */
extern struct TexScroll **D_800BE4F0; /* four texture scrolls (func_15010FB0) */
void func_15010FB0(void);

/* game_3E5A0.c */
void func_150110F0(void);

/* game_3E620.c */
void func_15011170(void);

/* game_3E7E0.c */
void func_15011330(void);

/* game_3E810.c */
void func_15011360(void);

/* game_3E890.c */
void func_150113E0(void);
void func_1501175C(Actor *owner, u8 arg1, s32 arg2);
void func_15011A78(Actor *owner, u8 arg1, s32 arg2);

/* game_3EFB0.c */
void func_15011B00(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_15011B94(void);

/* game_3F0F0.c */
extern u8 D_800BE500[5]; /* five level flags, cleared by func_15011C40 */
void func_15011C40(void);

/* game_3F120.c */
void func_15011C70(void);
void func_15011CA0(void);
void func_15011CC0(void);

/* game_3F1F0.c */
void func_15011D40(void);
s32 func_15011D60(TriggerZone *zone);

/* game_3F3D0.c */
void func_15011F20(s32 arg0);

/* game_3F450.c */
void func_15011FA0(s32 arg0);

/* game_3F4D0.c */
void func_15012020(s32 arg0, s32 arg1);

/* game_3F560.c */
void func_150120B0(s32 arg0);
void func_1501214C(Actor *arg0);

/* game_3F670.c */
void func_150121C0(LightEffect *effect);

/* game_3FBD0.c */
u8 func_15012720(void);

/* game_3FC20.c */
void func_15012770(void);

/* game_3FC30.c */
void func_15012780(void);

/* game_40490.c */
void func_15012FE0(void);
void func_15013000(void);
s32 func_150130B4(TriggerZone *obj);
s32 func_1501370C(TriggerZone *obj);
s32 func_1501374C(TriggerZone *obj);
s32 func_15013778(TriggerZone *obj);
s32 func_1501396C(TriggerZone *obj);
s32 func_150139AC(TriggerZone *obj, s32 index);
s32 func_15013C38(TriggerZone *obj);
s32 func_15013D38(TriggerZone *obj);
s32 func_15013DE8(TriggerZone *obj);
s32 func_15013F9C(TriggerZone *obj);
s32 func_15013FC4(TriggerZone *obj);
s32 func_15014004(TriggerZone *obj);
s32 func_15014040(TriggerZone *obj);
s32 func_1501407C(TriggerZone *obj);
s32 func_15014094(TriggerZone *obj);
s32 func_15014144(TriggerZone *obj);
s32 func_15014220(TriggerZone *obj);
s32 func_150142AC(TriggerZone *obj);
s32 func_150142EC(TriggerZone *obj);
s32 func_150144B8(TriggerZone *obj);
s32 func_1501474C(TriggerZone *obj);
s32 func_15014B60(TriggerZone *obj);
s32 func_15014F6C(TriggerZone *obj);
s32 func_150150A4(void);
s32 func_15015104(TriggerZone *obj);
s32 func_150151D4(TriggerZone *obj);
s32 func_15015300(TriggerZone *obj);
void func_15015354(TriggerZone *obj);
s32 func_15015644(TriggerZone *obj);
s32 func_150156F4(TriggerZone *obj);
extern const s32 D_8009658C[5]; // defined after func_150130B4, which reads them: their .rodata follows its const local
extern const s32 D_800965A0[5];
extern const s32 D_800965B4[8];
extern u16 *D_800BE570; // RGBA16 screen copy, gScreenWidth x gScreenHeight (game_203340)
extern u8 D_800BE574;
extern u8 D_800BE575;

/* game_43D00.c */
void func_15016850(void);
void func_150169A0(s32 levelId);
void func_15017114(s32 levelId);
extern u8 D_800BE590;      // number of distinct spawn model types in D_800BE598
extern u16 D_800BE598[8];  // the distinct spawn model types (< 0xBB) of the level
extern s32 *D_800BE5A8[8]; // per spawn model type: data loaded by func_1502B7F0

/* game_FDD70.c */
void func_150D08C0(Camera *camera);
void func_150D0E90(s32 arg0);
void func_150D10E4(Actor *obj, u8 mirrored);
s32 func_150D11B4(BloodStrip *strip);
void func_150D1204(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip);

/* game_22D750.c (data only, linked after game_137ED0) */
extern ObjectBehavior D_80088C90[89];               // level-object behaviour handlers
extern const GroupedObjectDefaults D_800A26C0[114];

/* game_15D6E0.c */
void func_15130230(void *arg0, s32 arg1);

/* game_1A6300.c */
s32 func_15178E50(s32 arg0);

#endif
