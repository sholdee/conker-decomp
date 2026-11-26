#ifndef CONKER_PROP_H
#define CONKER_PROP_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "prop": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_362B0.c
 *   game_EC420.c
 *   game_F21D0.c
 *   game_F4D20.c
 *   game_FC550.c
 *   game_1188E0.c
 *   game_137ED0.c
 *   game_142560.c
 *   game_142A70.c
 *   game_1435C0.c
 *   game_143870.c
 *   game_143DE0.c
 *   game_144950.c
 *   game_1449A0.c
 *   game_144C70.c
 *   game_147470.c
 *   game_1478C0.c
 *   game_147C70.c
 *   game_1483E0.c
 *   game_14C3F0.c
 *   game_14CE30.c
 *   game/done/game_14D0D0.c
 *   game_1A48C0.c
 *   game_1AFC80.c
 */

/* Behaviour scratch state (WorldObject.touch) of the scenery movers in game_1188E0.c, game_143DE0.c and
 * game_1449A0.c (each file reaches it through its WORK(obj) macro). */
typedef union MoverWork {
    /* 0x7C */ s32 targetPos[3];  /* position to move to */
    /* 0x7C */ f32 targetSpin[3]; /* rotation to turn to */
    struct {
        /* 0x7C */ s32 target; /* animation position to reach, 0..0xFFFF */
        /* 0x80 */ u8 *frames; /* vertex frames loaded from ROM */
        /* 0x84 */ s32 pos;
    } anim;
    struct {
        /* 0x7C */ f32 damping;
        /* 0x80 */ f32 acceleration;
        /* 0x84 */ f32 velocity;
    } swing;
    struct {
        /* 0x7C */ f32 rebound; /* fraction of the velocity kept (reversed) when the target is crossed */
        /* 0x80 */ f32 acceleration; /* per tick */
        /* 0x84 */ f32 velocity;
    } bounce;
} MoverWork;

/* A piece spawned by func_15150400, as the handlers below see it. */
typedef struct {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0xB];
    /* 0x018 */ f32 unk18;
    /* 0x01C */ u8 pad1C[0x1C];
    /* 0x038 */ Vec3f position;
    /* 0x044 */ u8 pad44[0x1C];
    /* 0x060 */ s32 unk60;
    /* 0x064 */ u8 pad64[0x2];
    /* 0x066 */ u16 unk66;
    /* 0x068 */ u8 pad68[0xA8];
    /* 0x110 */ u8 unk110[0x60];
    /* 0x170 */ f32 burstChance; /* func_150C04C0's extraData */
} Piece;

/* An entry whose level ramps toward maxLevel while `rising` is set and back to 0 otherwise. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[0x7];
    /* 0x1F */ u8 level;
    /* 0x20 */ u8 pad20[0x4];
    /* 0x24 */ u8 rising;
    /* 0x25 */ u8 pad25;
    /* 0x26 */ u8 maxLevel;
    /* 0x27 */ u8 riseRate;
    /* 0x28 */ u8 pad28[0x4];
    /* 0x2C */ u8 age;
    /* 0x2D */ s8 unk14Rate;
    /* 0x2E */ s8 unk16Rate;
    /* 0x2F */ u8 fallRate;
} Struct150BF0F4;

typedef struct {
    /* 0x00 */ u8  pad0[0x60];
    /* 0x60 */ s32 flags;
} Struct150C5310;

/*
 * Vtx_t with named coordinates. matching: Vtx_t's ob[] subscripts make IDO schedule the coordinate
 * loads in a different order.
 */
typedef struct PropVtx {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];
    /* 0xC */ u8 cn[4];
} PropVtx;

typedef struct Game1483E0UvVertex {
    s16 x, y, z;
    u8 unknown06[2];
    s16 s, t;
    u32 unknown0C;
} Game1483E0UvVertex;

typedef struct Game1483E0UvPair {
    s16 s, t;
} Game1483E0UvPair;

typedef struct Game1483E0State {
    u8 unknown00[0x10];
    s16 x, y, z;
    u16 count;
    u8 unknown18[8];
    Game1483E0UvVertex *buffers[2];
    Game1483E0UvVertex *source;
    u8 unknown2C[0x13];
    u8 reference_id;
    u8 unknown40[0x3C];
    Game1483E0UvPair *coords;
    Actor *reference;
} Game1483E0State;

typedef struct Game1483E0WaveState {
    Game1483E0UvPair *coords;
    s32 field4;
    s32 field8;
} Game1483E0WaveState;

typedef struct Game1483E0WaveMesh {
    u8 pad0[0x16];
    u16 count;
    u8 pad18[8];
    Game1483E0UvVertex *buffers[2];
    Game1483E0UvVertex *source;
    u8 pad2C[0x10];
    s32 flags; /* bits 0-11: S amplitude, 12-23: T amplitude, 24-31: speed */
    u8 pad40[0x3C];
    Game1483E0WaveState *wave;
    s32 phase;
} Game1483E0WaveMesh;

/* game_362B0.c */
void func_15008E00(void);
void func_15008E10(s32 arg0);

/* game_EC420.c */
void func_150BEF70(s32 arg0);
void func_150BEF7C(s32 arg0);
s32 func_150BF0F4(Struct150BF0F4 *entry);
void func_150BF21C(WorldObject *obj);
void func_150BF760(WorldObject *obj);
void func_150BFA7C(WorldObject *platform);
void func_150BFFE0(Actor *obj);
s32 func_150C01DC(Piece *piece, s32 arg1, s32 arg2, s32 arg3, f32 y);
void func_150C04C0(Vec3f *origin, void *arg1, s32 id, u8 arg3, u8 arg4, s32 arg5);
s32 func_150C0648(Piece *piece, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface);

/* game_F21D0.c */
extern EffectHeader *D_800D98D0[4]; /* level spray effects (game_3C740.c), destroyed by func_150C522C */
void func_150C4D20(WorldObject *pendulum);
void func_150C4E9C(WorldObject *obj);
void func_150C522C(void);
s32 func_150C5280(void);
s32 func_150C52CC(void *fx);
s32 func_150C5310(Struct150C5310 *obj);

/* game_F4D20.c */
void func_150C7870(WorldObject *obj);
void func_150C78E0(WorldObject *obj);
void func_150C7930(WorldObject *obj);
void func_150C7968(WorldObject *obj);
void func_150C79BC(s32 view);
void func_150C7C90(WorldObject *obj);
void func_150C7D7C(WorldObject *obj);

/* game_FC550.c */
void func_150CF0A0(WorldObject *arg0);

/* game_1188E0.c */
void func_150EB430(Vec3f *start, Vec3f *delta, Vec3f *hit);
s32 func_150EB484(Vec3f *start, Vec3f *end, Vec3f *hit);
void func_150EB614(WorldObject *obj);
void func_150EB8C4(void);
void func_150EBC80(TriggerZone *gate, u8 owner, s32 kind);

/* game_137ED0.c */
void func_1510AA20(s32 arg0);
void func_1510AA44(WorldObject *minuteHand);
void func_1510ADD8(WorldObject *obj);
s32 func_1510AEE0(MtxF mtx, f32 x, f32 y, f32 z, f32 nearZ, f32 farZ, f32 slopeX, f32 slopeY, f32 *marginX,
                  f32 *marginY);

/* game_142560.c */
/* (vertex, pivot) index pairs: vertices that follow the rotation of a preceding object (func_1511B7D4). */
typedef struct VertexPivots {
    /* 0x0 */ u8 count;
    /* 0x1 */ u8 pairs[8][2];
} VertexPivots;
typedef struct SoundTriple {
    /* 0x0 */ u16 ids[3];
} SoundTriple;
extern const VertexPivots D_800A2F70;
extern const SoundTriple D_800A2F84;
void func_151150B0(WorldObject *obj);
void func_151150BC(WorldObject *obj);
void func_1511515C(WorldObject *obj);
void func_151151FC(WorldObject *obj);
void func_1511529C(s32 arg0);
void func_151152A8(WorldObject *obj);
void func_15115368(WorldObject *obj);

/* game_142A70.c */
void func_151155C0(WorldObject *obj);
void func_1511575C(WorldObject *obj);
void func_15115E0C(WorldObject *obj, Actor *player);
void func_15115EDC(WorldObject *obj, Actor *player);
void func_15115F68(WorldObject *obj);
void func_15116058(WorldObject *obj);

/* game_1435C0.c */
void func_15116110(WorldObject *obj);
void func_1511617C(WorldObject *obj);
void func_151162D4(WorldObject *obj);

/* game_143870.c */
void func_151163C0(WorldObject *obj);
void func_1511650C(WorldObject *obj, s32 arg1, s32 soundId, f32 pitchScale);
s32 func_15116888(s32 x, s32 z, s32 *unused, Actor *other);
void func_15116924(s32 arg0);

/* game_143DE0.c */
void func_15116930(WorldObject *obj, Actor *actor);
void func_15116984(WorldObject *obj);
void func_151169B4(WorldObject *obj);
void func_15116BAC(WorldObject *obj);
void func_15116D7C(WorldObject *obj);
void func_15116EA4(WorldObject *obj);
f32 func_151172D8(WorldObject *obj, f32 angle);
extern PropSounds D_80089260[16]; // prop sound rows (doors, movers, sliders)

/* game_144950.c */
void func_151174A0(WorldObject *obj);
void func_151174C8(WorldObject *obj);

/* game_1449A0.c */
void func_151174F0(WorldObject *obj);
f32 func_15117518(WorldObject *obj, f32 angle);
void func_15117770(WorldObject *obj);
void func_15117798(WorldObject *obj);

/* game_144C70.c */
void func_151177C0(WorldObject *prop);
void func_151179BC(WorldObject *prop);
void func_15117D3C(WorldObject *prop, Actor *obj);
void func_15117DA4(WorldObject *prop, f32 x, f32 y, f32 z, f32 *out);
void func_15117F3C(WorldObject *prop);
void func_1511896C(WorldObject *prop);
void func_1511898C(WorldObject *prop);
void func_151189AC(WorldObject *prop, s32 arg1);
void func_15118F24(WorldObject *prop);
void func_151193AC(WorldObject *prop, Actor *obj);
void func_151193F4(WorldObject *prop);
void func_151194D4(WorldObject *prop, PropVtx *seed, s32 minDistSq, s32 group);
void func_151196D4(WorldObject *prop);
void func_15119938(WorldObject *prop);

/* game_147470.c */
void func_15119FC0(WorldObject *obj);

/* game_1478C0.c */
s32 func_1511A410(Gfx *dl, s32 *second);
void func_1511A494(WorldObject *obj, s32 *texIndices, s32 *animState);
void func_1511A6FC(WorldObject *obj);
void func_1511A738(WorldObject *obj);

/* game_147C70.c */
void func_1511A7C0(WorldObject *mesh);
void func_1511A838(WorldObject *mesh);

/* game_1483E0.c */
void func_1511AF30(WorldObject *obj);
void func_1511B07C(WorldObject *obj);
void func_1511B51C(WorldObject *obj);
void func_1511B7D4(WorldObject *obj);
void func_1511BA24(WorldObject *obj);
void func_1511BB04(Game1483E0State *obj, f32 refX, f32 refZ, f32 scaleS, f32 scaleT);
void func_1511BDF4(Game1483E0State *obj);
void func_1511BE5C(WorldObject *obj);
void func_1511BEBC(WorldObject *obj);
void func_1511C540(void);
void func_1511C548(WorldObject *obj);
void func_1511C638(WorldObject *obj);
void func_1511CB2C(WorldObject *obj, f32 *originZ);
void func_1511CB44(WorldObject *obj);
void func_1511D394(WorldObject *obj);
void func_1511D7BC(Game1483E0WaveMesh *mesh);
void func_1511D9E4(WorldObject *obj);
void func_1511DBC4(WorldObject *obj);
void func_1511DD98(WorldObject *obj, Actor *character);
void func_1511DF6C(WorldObject *obj);
void func_1511E780(WorldObject *prop);
void func_1511EC50(WorldObject *prop);
void func_1511ED84(WorldObject *prop);

/* game_14C3F0.c */
void func_1511EF40(WorldObject *prop);
void func_1511F31C(WorldObject *prop);
void func_1511F3E8(WorldObject *prop);
void func_1511F4D0(WorldObject *prop);
void func_1511F768(WorldObject *prop, s32 arg1);
void func_1511F788(WorldObject *prop);
void func_1511F92C(WorldObject *prop);

/* game_14CE30.c */
void func_1511F980(void);
void func_1511F990(WorldObject *prop, s32 mode);

/* game/done/game_14D0D0.c */
void func_1511FC20(s32 arg0);
Gfx *func_1511FC2C(Gfx *gfx, f32 x, f32 y, f32 z, f32 rot, s32 view);
Gfx *func_1511FC44(Gfx *gfx, f32 x, f32 y, f32 z, s32 view);

/* game_1A48C0.c */
void func_15177410(u8 flags, u8 objectId, s16 x, s16 y, s16 z, f32 heading, u16 radius, f32 amplitude, s8 speed,
                   s8 phaseStep, u8 argA, u8 argB, u8 argC, u8 argD, u8 argE, u8 argF);
void func_15177A94(void);

/* game_1AFC80.c */
void func_151827D0(void);
void func_15182C5C(WorldObject *prop);
f32 func_15182F58(s32 dist, s32 chain);
s32 func_15182FDC(Actor *actor, s32 sample, s32 chain);

#endif
