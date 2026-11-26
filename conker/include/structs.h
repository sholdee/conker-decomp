#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct SndState SndState;
typedef struct Actor Actor;
typedef struct Camera Camera;
struct Rope;
struct BoneChain;
typedef struct ObjScriptCmd ObjScriptCmd;
typedef struct BlockDebris BlockDebris;
/* Effect objects (include/effects.h), named here for the prototypes in functions.h. */
struct DefParticleHead;
struct ParticleDesc;
struct QuadSprite;
struct BillboardParticle;
union ActorMsg;

/*
  union convention
  ================
  b  -> (signed) byte (s8)
  ub -> unsigned byte (u8)
  h  -> (signed) short (s16)
  uh -> unsigned short (u16)
  w  -> (signed) word (s16)
  uw -> unsigned word (u16)
*/

/* Three-float vector: positions, directions, velocities, scales. */
typedef struct Vec3f {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
} Vec3f; // size 0xC

/* A triangle's three vertex positions, passed by value to the triangle tests of game_76710.c
 * (func_15049260, func_15049350). */
typedef struct Tri3f {
    /* 0x00 */ Vec3f pts[3];
} Tri3f; // size 0x24

/* 4x4 float matrix (row-major, the guMtxF2L layout) for bone and camera transforms. */
typedef f32 MtxF[4][4];

/* Three-short vector: packed positions, angles. */
typedef struct Vec3s {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
} Vec3s; // size 0x6

/* Two-float vector: screen or texture positions and velocities. */
typedef struct Vec2f {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
} Vec2f; // size 0x8

/* Two-float vector on the ground plane (world x and z). */
typedef struct Vec2fXZ {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 z;
} Vec2fXZ; // size 0x8

/* Three-int vector. */
typedef struct Vec3i {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 z;
} Vec3i; // size 0xC

// size 0x44

/* Actor effect D_8008A0B4[handler] (game_16EE20.c): its handler runs once, or every frame while it is
 * attached to the actor as a timed effect of `duration` frames. */
typedef struct ActorEffectDef {
    /* 0x0 */ void (*handler)(struct Actor *actor, s32 arg1, s32 timer);
    /* 0x4 */ s32 duration;  // > 0: attached as a timed effect
} ActorEffectDef;

/* A sound player voice record: the SndState of src/libultra/audio/init_15550.c (the audio library's sound state), as seen by game code
 * that cannot include n_libaudio.h; the sound slots of init_EB00.c (D_800425E0) point at one. */
struct SndState {
    /* 0x00 */ struct SndState *next;
    /* 0x04 */ struct SndState *prev;
    /* 0x08 */ u8 pad8[0x4];
    /* 0x0C */ ALSound *snd;
    /* 0x10 */ u8 voice[0x20];       /* N_ALVoice */
    /* 0x30 */ f32 ratio;            /* key-map cents ratio */
    /* 0x34 */ f32 pitch;            /* pitch requested by the client */
    /* 0x38 */ struct SndState **owner;
    /* 0x3C */ ALBank *bank;
    /* 0x40 */ s32 retries;
    /* 0x44 */ s16 volume;           /* client volume */
    /* 0x46 */ s16 envVolume;        /* envelope volume */
    /* 0x48 */ s32 segEnd;
    /* 0x4C */ s16 soundIndex;       /* within the instrument */
    /* 0x4E */ u8 priority;
    /* 0x4F */ u8 pan;
    /* 0x50 */ u8 fxMix;
    /* 0x51 */ u8 fxBus;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 flags;             /* bit 0: allocated, bit 1: looping, bit 2: stopping; 0x10/0x20 from keyMax */
    /* 0x54 */ u8 state;             /* play state */
    /* 0x55 */ u8 pad55[3];
}; /* size 0x58 */

typedef struct {
    s16 unk0;   // used
    s16 unk2;   // used
    s32 unk4;
    u16 *unk8; // used
    s32 unkC;
    s32 unk10;
    s32 unk14; // used
    s32 unk18; // used
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    void *unk28;
    s32 unk2C;
    s32 unk30; // used
    s32 unk34; // used
    s32 unk38;
    s32 unk3C;
    s32 unk40; // used
    s32 unk44; // used
    s32 unk48; // used
    f32 unk4C; // used
    s32 unk50; // used
    f32 unk54; // used
    s32 unk58; // used
    s32 unk5C; // used
    u16 unk60; // used
    u16 unk62; // used
    u16 unk64; // used
    u16 unk66; // used
    s16 unk68; // used
    s16 unk6A; // used
    u16 unk6C; // used
    u16 unk6E; // used
    u16 unk70; // used
    u16 unk72;
    u16 unk74;
    u16 unk76; // used
    s32 unk78; // used
    s32 unk7C; // used
    s32 unk80; // used
    s32 unk84; // used
    s32 unk88; // used
    s32 unk8C; // used
    s32 unk90;
    s16 unk94;
    s16 unk96;
    u8  unk98;
    u8  unk99; // used
    u16 unk9A;
    s16 unk9C;
    s16 unk9E;
    u16 unkA0; // used
    u16 unkA2; // used
    s16 unkA4;
    s16 unkA6;
    s32 unkA8;
    s32 unkAC;
    s32 unkB0;
    s32 unkB4;
    s32 *unkB8; // used
    s32 unkBC;
    s32 unkC0;
    s32 unkC4;
    s32 unkC8; // used
    s32 unkCC; // used
} struct42;

#define OBJ_TYPE_COUNT 104 /* object types: D_8008B4A8 entries, lists per D_800DCE50 row */

/* Common head of every object in the object lists D_800DCE50[list][type] (game_1944C0.c): effects, particles,
 * sprites, timed effects. func_15167A68 allocates one and links it in (func_15168A4C); func_1516972C destroys it
 * (the type's destroy hook, by default a move to type 1) and func_1516979C frees it. */
typedef struct EffectHeader {
    /* 0x00 */ u8 type;     /* index into D_8008B4A8 (ObjType); 1 = pending free */
    /* 0x01 */ u8 kind;     /* the object list (0 or 1) it is linked into */
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ struct EffectHeader *prev;
    /* 0x08 */ struct EffectHeader *next;
    /* 0x0C */ u8 owner;    /* events 0xF / 0x10 naming this id destroy the object (func_1516968C) */
    /* 0x0D */ u8 flags;    /* TimedEffect: bit 0 = state counts down, bit 1 = allocated as type 95 (else 35) */
    /* 0x0E */ s16 state;   /* -1 frees the object; a TimedEffect's timer (it expires below 0) */
} EffectHeader; /* size 0x10 */

/* Timed effect object (type 35, or 95 with flag 2), allocated by func_15149130 with dataSize bytes of
 * per-effect data after it; game_1765E0.c runs its callbacks. */
typedef struct TimedEffect {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s8 expireCallback; /* D_8008A4C0 index, run once the timer goes negative; -1 = none */
    /* 0x11 */ s8 tickCallback;   /* D_8008A4E8 index, run every frame; -1 = none */
    /* 0x12 */ s8 drawCallback;   /* D_8008A670 index; -1 = none */
    /* 0x13 */ u8 callbackSet;    /* index into D_8008A688 / D_8008A7B0 / D_8008A8D8 (74 sets) */
    /* 0x14 */ Vtx *buffers[4];   /* per-view vertex buffers (func_151D5D60), released by func_151D5E30 */
    /* 0x24 */ u8 pad24[4];
} TimedEffect; /* size 0x28; the effect's data follows */

/* Position data of an object emitter (a TimedEffect from func_151491F4 whose expire callback
 * func_15193390, game_1C0840.c, spawns object type 0x45 there). */
typedef struct EmitterPos {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 floorY; // -10000 = no floor
    /* 0x08 */ u8 unk8;
} EmitterPos;

typedef struct ObjectEmitter {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EmitterPos pos;
} ObjectEmitter;

/* Music settings of a level (LevelHeader.music), applied by func_150000B0: up to two songs, each with
 * two parameters (func_1000E40C / func_1000E46C). */
typedef struct LevelMusic {
    /* 0x0 */ s16 song;     // -1 = none
    /* 0x2 */ u16 unk2;
    /* 0x4 */ u16 unk4;
    /* 0x6 */ s16 song2;    // -1 = none
    /* 0x8 */ u16 unk8;
    /* 0xA */ u16 unkA;
    /* 0xC */ u8  mode;
    /* 0xD */ u8  unkD;
} LevelMusic; // size 0xE

/* The current level's settings resource, D_800B0DF0 (loaded by func_15000F04 in game_2DF70). The
 * one-byte callback fields index handler tables; 0 means none. */
typedef struct LevelHeader {
    /* 0x00 */ u16 unk0;            // fog (D_8008CC70.unkE), D_800D9B1C
    /* 0x02 */ u16 unk2;            // fog (D_8008CC70.unkC), D_800D9B20, RenderView.unk67C
    /* 0x04 */ u8  unk4;
    /* 0x05 */ u8  unk5;            // background colour (D_800DBEA8)
    /* 0x06 */ u8  unk6;
    /* 0x07 */ u8  unk7;
    /* 0x08 */ u8  unk8;            // sky mode (D_800891D8); 4 = textured sky
    /* 0x09 */ u8  unk9;            // sky texture set (D_800891BC)
    /* 0x0A */ u8  unkA;            // callback, D_80082D90
    /* 0x0B */ u8  updateCallback;  // D_8008CD04
    /* 0x0C */ u8  drawCallback1;   // D_8008CD74
    /* 0x0D */ u8  drawCallback2;   // D_8008CD7C
    /* 0x0E */ u8  unkE;            // callback, D_80082B40
    /* 0x0F */ u8  unkF;            // callback, D_80089670
    /* 0x10 */ u8  unk10;           // sun position, index into D_8008CFFC
    /* 0x11 */ s8  unk11;           // game_34F20 adds it to 0xEB
    /* 0x12 */ u8  unk12;
    /* 0x13 */ u8  unk13;           // landing step (game_83300)
    /* 0x14 */ u8  unk14;
    /* 0x15 */ u8  pad15;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u16 maxParticles;    // level-triangle particles (game_36680)
    /* 0x1A */ u16 unk1A;           // D_800C3E7C, a count of 0x40-byte entries
    /* 0x1C */ u16 gfxCount;        // display list length in Gfx (func_15015E80 adds 400)
    /* 0x1E */ u16 unk1E;           // game_1B1600
    /* 0x20 */ u8  pad20;
    /* 0x21 */ u8  vtxColorFlags;   // game_1A0E60
    /* 0x22 */ u8  pad22[7];
    /* 0x29 */ u8  unk29;
    /* 0x2A */ u8  flags;           // bit 0 checked by init_11FA0
    /* 0x2B */ u8  ucode;           // game_49D30
    /* 0x2C */ LevelMusic music;
    /* 0x3A */ s16 unk3A;           // fog (D_8008CC70.unk4..unkA)
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;           // nonzero: level fog on objects
    /* 0x40 */ s16 unk40;
    /* 0x42 */ u8  fogColour[4];
    /* 0x46 */ s8  fogPreset;       // fog preset (func_151733E4), -1 = none
    /* 0x47 */ u8  pad47[2];
    /* 0x49 */ u8  unk49;           // callback, D_80086004
    /* 0x4A */ u8  unk4A;           // callback, D_8008600C
    /* 0x4B */ u8  assetList;       // index into D_80082B20, 0 = none
} LevelHeader; // size 0x4C

/* The level settings resource as returned by func_1502B6BC (table 21, func_15002FB4): the
 * LevelHeader and the id list published as D_800D2F40. */
typedef struct LevelSettingsAsset {
    /* 0x00 */ LevelHeader *header;
    /* 0x04 */ s32 unk4;     // low 24 bits nonzero when present
    /* 0x08 */ u16 *idList;  // D_800D2F40, LevelHeader.unk12 entries
} LevelSettingsAsset;

/* A ring buffer of length-prefixed messages (game_70200.c: func_15043A00 / func_15043BB8 / func_15043CA4). */
typedef struct MsgRing {
    /* 0x0 */ u8 *buffer;
    /* 0x4 */ s32 capacity;
    /* 0x8 */ s32 readOffset;
    /* 0xC */ s32 writeOffset;  /* equal to readOffset when empty */
} MsgRing;

/* One simulated bone of an object broken into parts (game_6B320; size 0x68). */
typedef struct ObjPart {
    /* 0x00 */ f32 initPos[3];
    /* 0x0C */ f32 initRot[3];   /* degrees */
    /* 0x18 */ f32 initScale[3];
    /* 0x24 */ f32 pos[3];
    /* 0x30 */ f32 rot[3];
    /* 0x3C */ f32 scale[3];
    /* 0x48 */ f32 vel[3];
    /* 0x54 */ f32 rotVel[3];
    /* 0x60 */ f32 gravity;
    /* 0x64 */ u8 active;
    /* 0x65 */ u8 pad65[3];
} ObjPart;

/* Per-object parts state (D_800C6660[objIndex]; game_6B320). */
typedef struct ObjPartsState {
    /* 0x00 */ ObjPart *parts;
    /* 0x04 */ s32 partMaskLo;   /* bit i selects part i, parts 0..31 */
    /* 0x08 */ s32 partMaskHi;   /* parts 32..63 (shift count taken mod 32) */
    /* 0x0C */ s16 timer;
    /* 0x0E */ u8 kind;          /* index into the D_80084430.. per-kind tables */
    /* 0x0F */ u8 state;         /* PARTS_STATE_* */
} ObjPartsState;

/* Object type descriptor, D_8008B4A8[type] (game_1944C0). */
typedef struct ObjType {
    /* 0x00 */ void (*unk0)(EffectHeader *);
    /* 0x04 */ void (*unk4)(EffectHeader *);
    /* 0x08 */ Gfx *(*draw)(Gfx *gfx, EffectHeader *obj, s16 arg2);
    /* 0x0C */ void (*unkC)(EffectHeader *);
    /* 0x10 */ Gfx *drawBeginDL;
    /* 0x14 */ Gfx *(*drawBegin)(Gfx *gfx, s32 type, s32 list, s16 arg3);
    /* 0x18 */ void (*unk18)(void);
    /* 0x1C */ void (*handleEvent)(EffectHeader *obj, s32 event, u8 eventType);
    /* 0x20 */ Gfx *(*drawEnd)(Gfx *gfx, s32 type, s16 arg2);
    /* 0x24 */ Gfx *drawEndDL;
    /* 0x28 */ void (*destroy)(EffectHeader *);
    /* 0x2C */ void (*free)(EffectHeader *);
    /* 0x30 */ s8 drawMode;
    /* 0x31 */ u8 pad31[3];
} ObjType; // size 0x34

/* A positional sound slot of init_EB00.c (D_800425E0[16]); a handle's low 4 bits are its slot. */
typedef struct {
    /* 0x0 */ u16 handle;
    /* 0x2 */ u16 nextHandle;  /* handed out next, then advanced by 16 */
    /* 0x4 */ u16 sound;       /* bit 15: reserved */
    /* 0x6 */ u16 unk6;
    /* 0x8 */ SndState *voice;
} SoundSlot;

struct SHORTS {
    s16 unk0;
    s16 unk2;
};

struct WORD {
    s32 unk0;
};

/* Per model-type definition, D_800D1C90[id] (loaded by func_1503D774; freed by func_15084558 with its
 * allocation starting 0x10 bytes earlier). An ExpressionTableHeader (game_AB760) sits 8 bytes before it.
 * Only the fields the code reads are typed; the full size is unknown (at least 0x40). */
typedef struct ModelDef {
    /* 0x00 */ s16 aimHeight;       // aim top point, above the aim bone (func_1503A08C)
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8  subModelCount;
    /* 0x05 */ u8  subModelList;    // index into D_80086CAC
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8  texture[2][3];   // texture per eye slot (Actor.unk6C) and state
    /* 0x0E */ u16 bodyRadius;
    /* 0x10 */ s16 bodyHeight;
    /* 0x12 */ u8  unk12;
    /* 0x13 */ u8  pad13;
    /* 0x14 */ s16 headHeight;
    /* 0x16 */ u8  pad16;
    /* 0x17 */ u8  viewAngle;       // width of the spotting cone, in 1/256 turns
    /* 0x18 */ u8  unk18;
    /* 0x19 */ u8  unk19;
    /* 0x1A */ s16 unk1A;           // body cylinder (Actor.unkE4/E6/E8)
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;           // hit cylinder (Actor.unkD2/D4/D6)
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u8  aimBone;
    /* 0x27 */ u8  headBone;
    /* 0x28 */ u8  flags;
    /* 0x29 */ u8  unk29;
    /* 0x2A */ u16 unk2A;
    /* 0x2C */ s8  headYaw[2];      // head turn range: yaw one way / the other (func_1503A830)
    /* 0x2E */ s8  headPitch[2];    // pitch up / down
    /* 0x30 */ s8  aimReach;        // aim point along the aim bone's z axis
    /* 0x31 */ u8  pad31[2];
    /* 0x33 */ s8  unk33;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8  extraModelCount;
    /* 0x39 */ u8  unk39;
    /* 0x3A */ u8  aimHalfWidth;    // aim side points along the aim bone's x axis
    /* 0x3B */ u8  eyeDefault[2];   // default Actor.unk68[0..1]
    /* 0x3D */ u8  pad3D;
    /* 0x3E */ u16 cullDistance;    // 0 = Actor.unkE4, 1 = never culled, else scaled by scale_xz
} ModelDef;

/* Idle animation state of an ActorCtl (ActorCtl.idle, 0x58). While an actor stands still it plays
 * short "sequences" of frames, picked at random while avoiding the ones played most recently. */
typedef struct {
    /* 0x0 */ u8  active;
    /* 0x1 */ u8  holdFrames;  // frames left before the next sequence frame
    /* 0x2 */ u16 timer;       // time spent idle in the current phase
    /* 0x4 */ u8  sequence;    // index into D_80086BA0, 0 = none
    /* 0x5 */ u8  frame;       // position within the sequence
    /* 0x6 */ u8  frameFlags;  // flags of the frame applied last
    /* 0x7 */ u8  recent[5];   // most recently played sequences, newest first
    /* 0xC */ u8  phase;
    /* 0xD */ u8  phaseCount;  // sequences played in the current phase
    /* 0xE */ s16 delay;
} IdleState; // size 0x10

/* A triangle of three s16 vertices: an actor's floor triangle, collision hits (game_71820). */
typedef struct CollisionTri {
    /* 0x00 */ Vec3s v[3];
} CollisionTri; // size 0x12

/* Result of a ray cast: func_15081690 (level mesh, then the actors' collision cylinders or models). */
typedef struct RayHit {
    /* 0x00 */ Actor *obj;                  // actor hit (hitType 2/3), else NULL
    /* 0x04 */ f32 dist;                    // distance from origin to pos; -1.0f: no hit
    /* 0x08 */ Vec3f pos;                   // hit point (entry point on an actor)
    /* 0x14 */ Vec3f centre;                // model hit (hitType 3): centre of the part hit
    /* 0x20 */ Vec3f far;                   // exit point on an actor; = pos on a level-mesh hit
    /* 0x2C */ Vec3f origin;                // the ray
    /* 0x38 */ Vec3f dir;
    /* 0x44 */ CollisionTri tri;            // level triangle hit (hitType 1)
    /* 0x56 */ u16 bone;                    // model hit: bone of the part hit
    /* 0x58 */ u8 part;                     // model hit: part hit
    /* 0x59 */ u8 hitType;                  // 0 none, 1 level mesh, 2 actor cylinder, 3 actor model
    /* 0x5A */ u8 index;                    // model hit: index from func_1508114C
    /* 0x5B */ u8 pad5B;
    /* 0x5C */ struct WorldObject *surface; // level-mesh hit on an object surface, else NULL
    /* 0x60 */ s32 material;                // level-mesh hit: surface material
} RayHit; // size 0x64

/* Per-character control block (Actor.mv, 0x1C0 bytes, allocated for player-controlled and
 * scripted characters): movement state, controller input, rope riding, idle animation and the
 * first-person aim state.
 * original: mv_struct (rom_debug_struct.c, reached through movement_move "->mv"); the Oct 2000
 * layout equals US. Field names in Rare's spelling come from that viewer. */
typedef struct {
    /* 0x000 */ f32 actual_speed;
    /* 0x004 */ s32 duck_tilt;       // nonzero suppresses the walk lean (game_79F10.c)
    /* 0x008 */ s16 pissed_up;
    /* 0x00A */ u8  pissed_amount;   // not accessed by US code
    /* 0x00B */ u8  pissed_cycle;
    /* 0x00C */ u8  pissed_stumble;
    /* 0x00D */ u8  pssd_is_pissing;
    /* 0x00E */ u16 pissed_walk;
    /* 0x010 */ s16 pissed_angle;   // original viewer reads it lhu; s16 matches the float->int stores
    /* 0x012 */ s16 pssd_waist_tilt;
    /* 0x014 */ u16 pssd_waist_wobble;
    /* 0x016 */ u8  pissed_hangover;
    /* 0x017 */ u8  env_suit;
    /* 0x018 */ s16 unk18;
    /* 0x01A */ u8  injured;
    /* 0x01B */ u8  ind_cnt;         // original label "ind.cnt."
    /* 0x01C */ f32 water_up;
    /* 0x020 */ u8  water_attribs;
    /* 0x021 */ u8  unk21;
    /* 0x022 */ s8  unk22;
    /* 0x023 */ s8  door_walkf_time;
    /* 0x024 */ s16 gun_before_reload;
    /* 0x026 */ u8  break_stun;
    /* 0x027 */ u8  jump_on_char;
    /* 0x028 */ f32 swing_xvel;
    /* 0x02C */ f32 swing_zvel;
    /* 0x030 */ s8  gpoint;
    /* 0x031 */ u8  prop;
    /* 0x032 */ u16 walk_attrib;
    /* 0x034 */ u16 unk34;
    /* 0x036 */ u8  over_edge;
    /* 0x037 */ u8  dwalk;
    /* 0x038 */ s32 old_keys;
    /* 0x03C */ u8  turn;
    /* 0x03D */ u8  pad3D;
    /* 0x03E */ u16 xslopeangle;
    /* 0x040 */ f32 lean_last;
    /* 0x044 */ s8  camjolt;
    /* 0x045 */ u8  pad45;
    /* 0x046 */ s16 bitten;
    /* 0x048 */ s16 lockout;
    /* 0x04A */ s8  chased;
    /* 0x04B */ s8  chasing;
    /* 0x04C */ u16 press_angle;
    /* 0x04E */ u8  transport;
    /* 0x04F */ u8  transport_surface;
    /* 0x050 */ s8  unk50;
    /* 0x051 */ u8  land_floor;
    /* 0x052 */ u8  use_cam_butt;
    /* 0x053 */ u8  hang_ledge;
    /* 0x054 */ s8  side;
    /* 0x055 */ s8  save_done;
    /* 0x056 */ u8  unk56;
    /* 0x057 */ u8  unk57;
    /* 0x058 */ IdleState idle;
    /* 0x068 */ u16 unk68;
    /* 0x06A */ u8  pad6A;
    /* 0x06B */ u8  unk6B;      // grab state
    /* 0x06C */ s16 holdX;      // held position (s16 world units)
    /* 0x06E */ s16 holdY;
    /* 0x070 */ s16 holdZ;
    /* 0x072 */ u16 context_exclude_time;
    /* 0x074 */ u8  context_a;
    /* 0x075 */ u8  context_b;
    /* 0x076 */ u8  context_z;
    /* 0x077 */ u8  rt_context_a;
    /* 0x078 */ u8  rt_context_b;
    /* 0x079 */ u8  rt_context_z;
    /* 0x07A */ u8  cont_zone_ind;
    /* 0x07B */ u8  inst_context;
    /* 0x07C */ u8  unk7C;
    /* 0x07D */ u8  matrix_physics;
    /* 0x07E */ u16 unk7E;
    /* 0x080 */ s16 unk80;
    /* 0x082 */ s16 unk82;
    /* 0x084 */ u8  bot;
    /* 0x085 */ s8  stick_x;    // this frame's controller input
    /* 0x086 */ s8  stick_y;
    /* 0x087 */ u8  pad87;
    /* 0x088 */ u16 prevButtons;  // buttons of the previous frame
    /* 0x08A */ u16 buttons;
    /* 0x08C */ u16 pressedButtons;  // bot/scripted newly pressed buttons (copied to D_800CC288)
    /* 0x08E */ s8  buttonsTimer;    // frames until a bot's scripted `buttons` are released
    /* 0x08F */ s8  pressedButtonsTimer; // frames until pressedButtons is cleared
    /* 0x090 */ f32 unk90;
    /* 0x094 */ u8  unk94;
    /* 0x095 */ u8  ladder;
    /* 0x096 */ u8  unk96;
    /* 0x097 */ u8  ladderFromTop;   // the ladder was grabbed from its top (game_CDE80.c)
    /* 0x098 */ u8  rope;            // rope index (func_151AE590), 0 = none
    /* 0x099 */ u8  pad99[3];
    /* 0x09C */ struct Rope *ropePtr;     // rope being ridden, or NULL
    /* 0x0A0 */ f32 ropeProgress;      // 0..1 along the rope
    /* 0x0A4 */ u8  padA4[4];
    /* 0x0A8 */ f32 ropeSwingForce;
    /* 0x0AC */ u8  ropeSwingAngle;
    /* 0x0AD */ u8  ropeSwingTimer;
    /* 0x0AE */ u8  ropeNode;
    /* 0x0AF */ u8  unkAF;
    /* 0x0B0 */ RayHit rayHit;    // last aim or shot ray cast (func_15081690)
    /* 0x114 */ s16 height;      // cached size from func_1507C3E0
    /* 0x116 */ s16 width;
    /* 0x118 */ s16 depth;
    /* 0x11A */ u8  weapon_status;
    /* 0x11B */ u8  spawnIndex;
    /* 0x11C */ struct BoneChain *boneChain;
    /* 0x120 */ u8  death_State;
    /* 0x121 */ u8  unk121;
    /* 0x122 */ u8  respawnReady; // multiplayer: the dead player may respawn now (A pressed, a bot after 181 ticks, ...)
    /* 0x123 */ u8  unk123;
    /* 0x124 */ s16 deathTimer;      // countdown for death_State 2 and 7 (game_A9D90.c)
    /* 0x126 */ s16 deathTime;       // ticks since death_State left 0
    /* 0x128 */ u8  unk128;
    /* 0x129 */ s8  unk129;
    /* 0x12A */ s8  unk12A;
    /* 0x12B */ u8  pad12B;
    /* 0x12C */ u16 unk12C;
    /* 0x12E */ u8  pad12E[2];
    /* 0x130 */ Vec3f aimDir;
    /* 0x13C */ Vec3f aimEye;
    /* 0x148 */ Vec3f aimTarget;
    /* 0x154 */ u8  pad154[0xC];
    /* 0x160 */ Vec3f unk160;
    /* 0x16C */ f32 aimYaw;
    /* 0x170 */ f32 aimPitch;
    /* 0x174 */ f32 aimYawTarget;
    /* 0x178 */ f32 aimPitchTarget;
    /* 0x17C */ f32 aimYawVel;
    /* 0x180 */ f32 aimPitchVel;
    /* 0x184 */ f32 aimZoom;
    /* 0x188 */ f32 aimZoomY;
    /* 0x18C */ f32 unk18C;
    /* 0x190 */ s32 aimZone;
    /* 0x194 */ u8  aimReset;
    /* 0x195 */ s8  being_sniped;
    /* 0x196 */ s8  being_s_char;
    /* 0x197 */ u8  SniperState;      // nonzero while aiming in first person (our earlier name: aimState)
    /* 0x198 */ u8  AimState;        // copied from Camera.DoingAim (game_14FF90.c)
    /* 0x199 */ u8  shotCount;
    /* 0x19A */ u8  numGrenades;
    /* 0x19B */ u8  GrenadeThrowState;
    /* 0x19C */ u16 GrenadeTimer;
    /* 0x19E */ u16 ReloadTimer;
    /* 0x1A0 */ u16 unk1A0;
    /* 0x1A2 */ u16 gunRecoil;       // >> 8 = recoil pitch kick (Camera.LastGunRecoil, walk lean)
    /* 0x1A4 */ u16 gunRecoilVel;    // added to gunRecoil each tick while firing
    /* 0x1A6 */ u16 unk1A6;
    /* 0x1A8 */ u8  unk1A8;
    /* 0x1A9 */ u8  unk1A9;
    /* 0x1AA */ s16 shotsfired;
    /* 0x1AC */ u8  on_fire;
    /* 0x1AD */ u8  unk1AD;
    /* 0x1AE */ s8  ammo_count;
    /* 0x1AF */ s8  unk1AF;
    /* 0x1B0 */ s8  lastSpawnIndex;   // last multiplayer spawn point used
    /* 0x1B1 */ u8  unk1B1;
    /* 0x1B2 */ u8  unk1B2;
    /* 0x1B3 */ u8  unk1B3;
    /* 0x1B4 */ u8  deferredActions; // requests run, then cleared, by the per-frame loop in game_90840.c: 1 func_15063628, 2 fire (by rt_context_b), 4/8 func_151D57F8(0/1)
    /* 0x1B5 */ u8  pad1B5[3];
    /* 0x1B8 */ f32 gunPitch;        // tracked vehicle gun elevation, degrees (game_11A680.c)
    /* 0x1BC */ f32 gunYaw;          // tracked vehicle gun heading, degrees in [0, 360)
} ActorCtl; // size 0x1C0

/* One 0x30-byte entry of the level's spawn table D_800D20FC; Actor.spawn points at the one it came from. */
typedef struct SpawnRecord {
    /* 0x00 */ u16 flags;
    /* 0x02 */ u8  unk2;
    /* 0x03 */ u8  interaction_state;
    /* 0x04 */ u8  type;         // model id, 0xFF for none
    /* 0x05 */ u8  unk5;
    /* 0x06 */ s16 x;
    /* 0x08 */ s16 y;
    /* 0x0A */ s16 z;
    /* 0x0C */ u8  angle;        // high byte of the facing angle
    /* 0x0D */ u8  unkD;
    /* 0x0E */ u8  unkE;
    /* 0x0F */ u8  unkF;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  unk11;
    /* 0x12 */ u8  unk12;
    /* 0x13 */ u8  unk13;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8  pad16[0x2];
    /* 0x18 */ s32 objFlags;     // copied to the object's unkF8
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  pad1E[0x2];
    /* 0x20 */ f32 xz_scale;
    /* 0x24 */ f32 y_scale;
    /* 0x28 */ u8  unique_id;
    /* 0x29 */ u8  partSet;      // 1-based index into D_80086CC4, 0 for none
    /* 0x2A */ u16 unk2A;        // cull radius, 0 = always drawn (func_150623F4)
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u8  keepTime;     // seconds (x30 frames) to keep the model's D_800D2040 counter; 0xFF = never set
    /* 0x2F */ u8  unk2F;
} SpawnRecord; // size 0x30

/* One point of a level path (8 bytes). D_800D2104[n] points at path n: a centre point followed by
 * D_800D2108[n] - 1 waypoints. */
typedef struct PathPoint {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u16 param; /* point 0: radius / 8 of the path's area; later points: speed */
} PathPoint;

/* D_800D2138 (0x20C bytes, cleared by func_15017300): match state. D_800D213C (per-player stats) and
 * D_800D2340 (= unk208) are separately named views into it. */
typedef struct MatchState {
    /* 0x000 */ s32 unk0;      /* indexes D_80087280; 1 = two views */
    /* 0x004 */ u8  pad4[0x204];
    /* 0x208 */ u16 unk208;    /* bit per player */
    /* 0x20A */ u8  pad20A[2];
} MatchState; // size 0x20C

/* Text resource loaded by func_1502B6BC: the text, then the sound effects voiced one per word. */
typedef struct TextResource {
    /* 0x0 */ char *text;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ u16 *sounds;
    /* 0xC */ u32 soundsSize; /* in bytes */
} TextResource;

/* Text box setup: a message-table entry, or one supplied by the caller (see func_1516A7B0). */
typedef struct TextBoxConfig {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 text;            /* message table index, or the string itself when supplied */
    /* 0x08 */ u8 numPages;         /* strings in the text, separated by NULs */
    /* 0x09 */ u8 objIndex;         /* movement_obj object the box belongs to */
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 padB;
    /* 0x0C */ f32 worldX;          /* projected to screen when not TEXTBOX_SCREEN_POS */
    /* 0x10 */ f32 worldY;
    /* 0x14 */ f32 worldZ;
    /* 0x18 */ f32 *screenXPercent; /* TEXTBOX_SCREEN_POS placement */
    /* 0x1C */ f32 *screenYPercent;
    /* 0x20 */ f32 *anglePercent;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u16 duration;
    /* 0x28 */ u8 r;
    /* 0x29 */ u8 g;
    /* 0x2A */ u8 b;
    /* 0x2B */ u8 unk2B;            /* box is removed unless D_800C35EA == 1 */
    /* 0x2C */ void (*callback)(struct TextBoxConfig *config, s32 page, s32 state);
} TextBoxConfig;

/* Object type OBJTYPE_TEXTBOX, 0x74 bytes. */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad1[0xC];
    /* 0x0D */ u8 source;           /* 0: D_800DD254 text, 1: D_800DD258 text, 2: caller string */
    /* 0x0E */ u8 visible;
    /* 0x0F */ u8 unkF;
    /* 0x10 */ TextBoxConfig *config;
    /* 0x14 */ u8 unk14;            /* set for caller-supplied configs; disables button input */
    /* 0x15 */ u8 state;            /* TEXTBOX_STATE_* */
    /* 0x16 */ s16 timer;
    /* 0x18 */ s16 fadeTimer;
    /* 0x1A */ u8 textAlpha;
    /* 0x1B */ u8 bubbleAlpha[3];
    /* 0x1E */ u8 bubbleIndex;
    /* 0x1F */ u8 placed;           /* screen position taken from the world point */
    /* 0x20 */ s8 page;
    /* 0x21 */ u8 pad21[3];
    /* 0x24 */ u8 *text;            /* current page */
    /* 0x28 */ f32 scale;
    /* 0x2C */ f32 scaleSpeed;
    /* 0x30 */ f32 halfTextWidth;
    /* 0x34 */ f32 halfTextHeight;
    /* 0x38 */ f32 width;
    /* 0x3C */ f32 height;
    /* 0x40 */ f32 x;               /* anchor (tip of the tail) */
    /* 0x44 */ f32 y;
    /* 0x48 */ f32 bodyX;           /* scaled body position */
    /* 0x4C */ f32 bodyY;
    /* 0x50 */ f32 centerX;         /* unscaled body position: the text origin */
    /* 0x54 */ f32 centerY;
    /* 0x58 */ f32 angle;           /* direction from the anchor to the body, radians */
    /* 0x5C */ f32 anchorX;
    /* 0x60 */ f32 anchorY;
    /* 0x64 */ f32 baseX;
    /* 0x68 */ f32 baseY;
    /* 0x6C */ f32 yaw;             /* object heading relative to the camera, degrees */
    /* 0x70 */ f32 bubbleX;
} TextBox;

/* Animation playback state (Actor.unk2D0, ModelAttachment.anim). Most fields come in pairs: the
 * current animation and the previous one, kept for blending. Actors allocate only the first 0x3E0
 * bytes (game_AEB40); animated models (game_6C960, used by ModelAttachment) also use the fields
 * past that (size 0x3F8).
 * File-local view of the same object: ObjAnim (58F80), which indexes the current/previous pairs. */
typedef struct AnimInstance {
    /* 0x000 */ void (*callback)(void); // called once when playback reaches the last frame
    /* 0x004 */ u16 anim;           // current animation; bit 15: hold on the last frame
    /* 0x006 */ u16 prevAnim;
    /* 0x008 */ f32 frame;
    /* 0x00C */ f32 prevFrame;
    /* 0x010 */ f32 speed;
    /* 0x014 */ f32 prevSpeed;
    /* 0x018 */ f32 numFrames;      // playback end
    /* 0x01C */ f32 prevNumFrames;
    /* 0x020 */ f32 start;          // playback start (loop point)
    /* 0x024 */ f32 prevStart;
    /* 0x028 */ s32 animId;         // changes whenever a new animation is started
    /* 0x02C */ s32 prevAnimId;
    /* 0x030 */ s32 unk30;
    /* 0x034 */ s32 unk34;
    /* 0x038 */ s8  unk38;
    /* 0x039 */ s8  unk39;
    /* 0x03A */ s16 unk3A;          // blend step
    /* 0x03C */ s16 unk3C;          // blend countdown
    /* 0x03E */ u8  pad3E[2];
    /* 0x040 */ u8  header[0x1D0];  // the animation's data header: [3] frame count, [5] frame size
    /* 0x210 */ u8  prevHeader[0x1D0];
    /* 0x3E0 */ f32 (*matrices[2])[4]; // model transform, one per frame buffer; set by the owner
    /* 0x3E8 */ Mtx *partMtx[2];    // numParts matrices, one array per frame buffer
    /* 0x3F0 */ s32 unk3F0;
    /* 0x3F4 */ u8  numParts;
    /* 0x3F5 */ u8  bank;           // animation bank passed to func_1505E0C4
    /* 0x3F6 */ u8  unk3F6;         // set once the part matrices have been built
} AnimInstance; // size 0x3F8

/* One animation of a model's animation table (D_800D1588[id]); unk0 == 999 ends the table. */
typedef struct ActorAnim {
    /* 0x0 */ u16 unk0;
    /* 0x4 */ s32 offset; // relative to the table until func_1503D484 relocates it
} ActorAnim; // size 0x8

/* The 16 bytes before a model's animation table (loaded as offsets, made absolute by func_1503D660). */
typedef struct AnimHeader {
    /* 0x0 */ struct ScriptEntry *scripts;            // A28B0: script id -> script
    /* 0x4 */ struct HitReaction *hits;             // 83300: hit-reaction table (func_1505C1A4)
    /* 0x8 */ struct Game83300AnimEntry *reactions;   // 83300: reaction entries
    /* 0xC */ u32 reactionsSize;                      // byte size of reactions
} AnimHeader; // size 0x10

/* A path keyframe: a point the track's actor moves through (D_800C3868[scene][track][key]). */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u16 linear;  // nonzero: straight segment to the next key, else Catmull-Rom curve
} SeqPathKey; // size 0x8

/* What a scene track drives (D_800C35F0[scene][track]). */
typedef struct SeqTrackInfo {
    /* 0x0 */ u16 type;     // 1 = camera, 2 = an object (func_15083E90), 3 = an actor (func_151149AC)
    /* 0x2 */ u8 targetId;  // object / actor id for types 2 and 3
    /* 0x3 */ u8 unk3;
    /* 0x4 */ s16 yOffset;
    /* 0x6 */ s16 unk6;
} SeqTrackInfo; // size 0x8

/* The header in front of an animation table from D_800D1588. */
#define ANIM_HEADER(anims) ((AnimHeader *)(anims) - 1)

/* Position/rotation offset of a ModelAttachment from its owner's joint. */
typedef struct AttachmentOffset {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 rotX;
    /* 0x10 */ f32 rotY;
    /* 0x14 */ f32 rotZ;
} AttachmentOffset;

/* A model attached to a game object; doubly linked list headed by D_800C3EE0 (game_5D2C0). */
typedef struct ModelAttachment {
    /* 0x00 */ u8 ownerId;            // unique_id of the owning Actor
    /* 0x01 */ u8 model;              // model id; also selects the per-model hooks
    /* 0x02 */ u8 bone;               // owner joint matrix the model follows
    /* 0x03 */ u8 alpha;
    /* 0x04 */ u8 flags;
    /* 0x05 */ u8 updated;            // set by the update pass; not drawn before that
    /* 0x06 */ u8 setId;              // attachment set id (see func_15083568)
    /* 0x07 */ u8 handler;            // index into the D_800841C0 per-frame callbacks
    /* 0x08 */ u8 ownsVtx;            // vtx[] are private copies that must be freed
    /* 0x09 */ u8 loaded;
    /* 0x0A */ u16 ownerStates[3];    // if set, kept only while owner->unk84 is one of these
    /* 0x10 */ u8 pad10[3];
    /* 0x13 */ u8 hiddenDLists;       // bit i set: display list i is not drawn
    /* 0x14 */ u8 numDLists;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ s8 unk17;              // -1, or the animation set passed to func_1503F62C
    /* 0x18 */ u16 seg6Id;            // resolved by func_1510D0EC into segment 6
    /* 0x1A */ u16 seg7Id;            // resolved by func_1510D0EC into segment 7
    /* 0x1C */ u16 timer;             // frames left to live; 0xFFFF = forever
    /* 0x1E */ u16 parentSet;         // nonzero: follows a matrix of another attachment set
    /* 0x20 */ u16 parentMtx;
    /* 0x22 */ s16 rotZ;
    /* 0x24 */ Gfx **dlists;
    /* 0x28 */ void *vtx[2];          // segment 1 data, one per frame buffer
    /* 0x30 */ s32 unk30;
    /* 0x34 */ Mtx *mtx;              // two matrices, one per frame buffer
    /* 0x38 */ s32 unk38;             // per-model hook state (F4890 flash: frames left)
    /* 0x3C */ s32 unk3C;             // per-model hook state (F4890 flash: brightness 0..255)
    /* 0x40 */ f32 scale;
    /* 0x44 */ AttachmentOffset *offset;
    /* 0x48 */ AnimInstance *anim;
    /* 0x4C */ void *reflMtx[2];      // mirrored matrices, one per frame buffer (game_623D0)
    /* 0x54 */ struct ModelAttachment *next;
    /* 0x58 */ struct ModelAttachment *prev;
} ModelAttachment; // size 0x5C

/* Node of the attachment list an actor keeps at 0x2F4 (game_179F30.c: func_1514EC1C attaches,
 * func_1514ECE0 finds by type, func_1514ED3C finds by handle). */
typedef struct Attachment {
    /* 0x00 */ EffectHeader header;         // object-list node (func_15167A68)
    /* 0x10 */ void *handle;                // the attached thing; what it is depends on `type`
    /* 0x14 */ struct Attachment *next;
    /* 0x18 */ struct Attachment *prev;
    /* 0x1C */ s16 type;                    // index into D_8008ABE8
} Attachment;

/* Attachment types form a tree (first child / next sibling), so a message sent to a type
 * also reaches attachments of its subtypes. D_8008ABE8 holds one per type. */
typedef struct AttachmentType {
    /* 0x0 */ s16 messageHandler;           // index into D_8008AB64, or -1
    /* 0x2 */ s16 destroyHandler;           // index into D_8008AB58, or -1
    /* 0x4 */ s16 firstChild;               // -1 terminates
    /* 0x6 */ s16 nextSibling;              // -1 terminates
} AttachmentType;

/* Defined by the files that use them. */
struct CamData;
struct TimedEvent;

/* Aim and trail points an actor keeps at 0x1A4 (filled in by func_1503A08C). */
typedef struct ActorAimPoints {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 topY;
    /* 0x08 */ s16 side[2][3]; // points pushed out to either side (trails are emitted from them)
    /* 0x14 */ s8  dir[3];     // facing direction in 1/256 units
} ActorAimPoints; // size 0x18

/* Result of a floor/ceiling probe: func_15046C80 (probe down from a point) / func_1504715C (an actor's floor). */
typedef struct CollisionHit {
    /* 0x00 */ f32 height;            // y of the surface hit
    /* 0x04 */ CollisionTri tri;      // the triangle hit
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 material;
    /* 0x1C */ u8 flags;              // 1: solid (level mesh, or object surface with viewFlags 0x80), 2: height within the probe's limit, 4: tri is valid
    /* 0x1D */ u8 source;             // collision set that produced the hit: 1 level mesh, 2-4 object surface sets (3: particles splash)
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ struct WorldObject *surface; // object surface hit (source 2-4), else NULL
} CollisionHit; // size 0x24

/* Per-actor vertex morph state (Actor.morph): blends from keyframe `prev` to keyframe `cur`. */
typedef struct VtxMorphState {
    /* 0x0 */ f32 blend; // 0.0f (prev) .. 1.0f (cur)
    /* 0x4 */ f32 rate;  // blend increase per tick
    /* 0x8 */ u8  prev;
    /* 0x9 */ u8  cur;
    /* 0xA */ u8  hold;  // counts down once the blend has reached 1.0f
} VtxMorphState; // size 0xC

/* The game actor: the 0x32C-byte entries of movement_obj (D_800CC2D0). original: movement_move (rom_debug_struct.c);
 * Oct 2000 had it 4 bytes shorter (the insertion is somewhere in 0x174-0x1C3). */
struct Actor {
    /* 0x000 */ s32 type;
    /* 0x004 */ u8  character_type;
    /* 0x005 */ u8  drawtype;
    /* 0x006 */ u8  unk6;
    /* 0x007 */ u8  unk7;
    /* 0x008 */ u8  unk8;
    /* 0x009 */ u8  unk9;
    /* 0x00A */ u8  unkA;
    /* 0x00B */ u8  unkB[4];  // one per player
    /* 0x00F */ u8  unkF;     // per-player bits (<< 4: seen by player i)
    /* 0x010 */ u16 cullDistance; // from the model definition, or the spawn record override (game_AEB40)
    /* 0x012 */ u16 unk12;    // frame counter
    /* 0x014 */ f32 xpos;
    /* 0x018 */ f32 ypos;
    /* 0x01C */ f32 zpos;
    /* 0x020 */ f32 yspd;
    /* 0x024 */ f32 ygrav;
    /* 0x028 */ f32 shad;
    /* 0x02C */ f32 old_xpos;
    /* 0x030 */ f32 old_ypos;
    /* 0x034 */ f32 old_zpos;
    /* 0x038 */ s16 unk38;
    /* 0x03A */ s8  unk3A;
    /* 0x03B */ u8  unique_id;
    /* 0x03C */ f32 speed;
    /* 0x040 */ f32 theta;
    /* 0x044 */ f32 stickspeed;
    /* 0x048 */ f32 speedup;
    /* 0x04C */ f32 animsp;
    /* 0x050 */ f32 wantanimsp;   // not accessed in US code
    /* 0x054 */ f32 unk54;
    /* 0x058 */ s32 unk58;
    /* 0x05C */ s32 basemtx;
    /* 0x060 */ s16 texScroll;
    /* 0x062 */ s16 texScrollSpeed;
    /* 0x064 */ s8  unk64;
    /* 0x065 */ u8  attach_char;
    /* 0x066 */ u8  unk66;      // flags
    /* 0x067 */ u8  pad67;
    /* 0x068 */ u8  unk68[2];    // texture frame of each animated part (eyes), from eyeCode
    /* 0x06A */ u8  eyeClosed[2]; // per eye, toggled by blinking
    /* 0x06C */ u8  eyeCode[2];   // >= 10: set by the current expression
    /* 0x06E */ u8  blinkTimer;
    /* 0x06F */ u8  expression;   // current expression index
    /* 0x070 */ u8  defaultExpression;
    /* 0x071 */ u8  expressionPriority;
    /* 0x072 */ u16 expressionTimer; // 0xFFFF = no timeout, 0xFFFE = frozen
    /* 0x074 */ u8  unk74;
    /* 0x075 */ s8  pad75;
    /* 0x076 */ u16 angle;  // moving_angle
    /* 0x078 */ u16 fangle;
    /* 0x07A */ u16 langle;  // facing angle
    /* 0x07C */ u16 mod_angle;
    /* 0x07E */ u8  unk7E;
    /* 0x07F */ u8  count1;  // counter
    /* 0x080 */ u8  count2;  // counter
    /* 0x081 */ u8  count3;  // counter
    /* 0x082 */ u8  count4;  // counter
    /* 0x083 */ u8  count5;
    /* 0x084 */ union {
        u16 uh;
        u8  ub[2];
    }   animseq;
    /* 0x086 */ u8  unk86;
    /* 0x087 */ u8  unk87;
    /* 0x088 */ u8  unk88;
    /* 0x089 */ u8  norun;
    /* 0x08A */ u8  nojump;
    /* 0x08B */ u8  pad8B;
    /* 0x08C */ u16 unk8C;
    /* 0x08E */ u16 unk8E;
    /* 0x090 */ s16 unk90;    // saved unkE4
    /* 0x092 */ u16 unk92;
    /* 0x094 */ s32 unk94;
    /* 0x098 */ s32 unk98;     // blacked-out display-list mask
    /* 0x09C */ u32 unk9C;
    /* 0x0A0 */ u32 fogColor;  // packed ARGB; in fog mode 1 a pointer to an f32 percentage
    /* 0x0A4 */ u8  fogMode;
    /* 0x0A5 */ u8  fogLevel;  // fade level, or pulse phase in modes 4/5
    /* 0x0A6 */ u8  fogRate;
    /* 0x0A7 */ u8  fogFade;   // pulse fade-out, 0xFF = full
    /* 0x0A8 */ u8  unkA8;
    /* 0x0A9 */ u8  unkA9;
    /* 0x0AA */ u8  unkAA;
    /* 0x0AB */ u8  unkAB;
    /* 0x0AC */ u8  unkAC;
    /* 0x0AD */ u8  inwater;
    /* 0x0AE */ u8  unkAE;
    /* 0x0AF */ u8  padAF;
    /* 0x0B0 */ s8  unkB0;
    /* 0x0B1 */ u8  padB1;
    /* 0x0B2 */ u16 unkB2;  // (oxygen?)
    /* 0x0B4 */ f32 lastEventTime; // animation time of the last timed event (func_1506AD30)
    /* 0x0B8 */ f32 vtilt;
    /* 0x0BC */ f32 unkBC;
    /* 0x0C0 */ f32 wantspeed;
    /* 0x0C4 */ f32 lean_angle;
    /* 0x0C8 */ f32 unkC8;
    /* 0x0CC */ s16 unkCC;
    /* 0x0CE */ s16 unkCE;
    /* 0x0D0 */ u8  doing_hit;
    /* 0x0D1 */ s8  padD1;
    /* 0x0D2 */ s16 unkD2;    // first set of bounds (func_1515C1A0): radius,
    /* 0x0D4 */ s16 unkD4;    // ...
    /* 0x0D6 */ s16 unkD6;    // ... and centre height above the position
    /* 0x0D8 */ s16 unkD8;
    /* 0x0DA */ s16 unkDA;
    /* 0x0DC */ f32 unkDC;
    /* 0x0E0 */ f32 unkE0;
    /* 0x0E4 */ s16 bodyRadius;  // body cylinder (func_15062AC4, collisions in game_83300)
    /* 0x0E6 */ s16 bodyHeight;
    /* 0x0E8 */ s16 bodyOffsetY; // its centre's height above the position (scaled by scale_y)
    /* 0x0EA */ u16 unkEA;
    /* 0x0EC */ f32 unkEC;
    /* 0x0F0 */ f32 unkF0;
    /* 0x0F4 */ s32 mvmt_control;
    /* 0x0F8 */ s32 noshove;
    /* 0x0FC */ s32 unkFC;
    /* 0x100 */ u8  unk100;
    /* 0x101 */ u8  unk101;
    /* 0x102 */ u8  unk102;
    /* 0x103 */ u8  unk103;
    /* 0x104 */ u8  stunned;
    /* 0x105 */ u8  stunnednum;
    /* 0x106 */ u8  stunnedseq;
    /* 0x107 */ u8  ivehitit;
    /* 0x108 */ u8  unk108;  // attacker slot | 0x80
    /* 0x109 */ u8  unk109;
    /* 0x10A */ u8  unk10A;  // victim/attacker slot | 0x80
    /* 0x10B */ u8  unk10B;
    /* 0x10C */ s16 unk10C;
    /* 0x10E */ u8  unk10E;
    /* 0x10F */ u8  unk10F;
    /* 0x110 */ u8  unk110;
    /* 0x111 */ u8  pad111;
    /* 0x112 */ u16 unk112;
    /* 0x114 */ f32 unk114;
    /* 0x118 */ f32 unk118;
    /* 0x11C */ f32 unk11C;
    /* 0x120 */ s16 unk120;
    /* 0x122 */ u8  unk122;
    /* 0x123 */ u8  unk123;
    /* 0x124 */ u8  owner;
    /* 0x125 */ u8  imune;
    /* 0x126 */ u8  unk126;  // owner index into movement_obj
    /* 0x127 */ u8  playernumber;
    /* 0x128 */ u8  team;
    /* 0x129 */ u8  unk129;
    /* 0x12A */ u8  unk12A;
    /* 0x12B */ u8  pad12B;
    /* 0x12C */ s16 unk12C;
    /* 0x12E */ u16 pad12E;
    /* 0x130 */ u32 pad130;
    /* 0x134 */ u8  unk134;
    /* 0x135 */ u8  morphDuration;
    /* 0x136 */ u8  unk136;  // sprite texture
    /* 0x137 */ u8  unk137;
    /* 0x138 */ u8  speccnt;
    /* 0x139 */ u8  unk139;  // requested sequence
    /* 0x13A */ u8  unk13A;  // its countdown
    /* 0x13B */ u8  unk13B;
    /* 0x13C */ u8  lifting;
    /* 0x13D */ u8  carried;
    /* 0x13E */ u8  unk13E;
    /* 0x13F */ u8  listpos; // index of its spawn record in D_800D20FC, 0xFF = none
    /* 0x140 */ u8  unk140;
    /* 0x141 */ u8  pad141[0x3];
    /* 0x144 */ SpawnRecord *listpos_ptr; // the spawn record this actor came from
    /* 0x148 */ f32 unk148;
    /* 0x14C */ f32 scale_xz;
    /* 0x150 */ f32 scale_y;
    /* 0x154 */ f32 unk154;
    /* 0x158 */ f32 unk158;
    /* 0x15C */ f32 unk15C;
    /* 0x160 */ u16 shadow_size;  // shadow size, scaled by scale_xz (game_13AE20)
    /* 0x162 */ u8  unk162;
    /* 0x163 */ u8  pad163;
    /* 0x164 */ f32 ob_slopex;
    /* 0x168 */ f32 ob_slopey;
    /* 0x16C */ f32 ob_overslopex;
    /* 0x170 */ f32 ob_overslopey;
    /* 0x174 */ f32 unk174;
    /* 0x178 */ f32 unk178;
    /* 0x17C */ f32 ceilingY;
    /* 0x180 */ f32 floorY;
    /* 0x184 */ u32 floorMaterial;  // from the floor probe: low 5 bits surface kind, bits 5+ shade
    /* 0x188 */ void *floorTriVerts; // floor triangle found by the floor probe: its three vertex pointers (Vec3s *[3], game_13AE20)
    /* 0x18C */ CollisionTri floorTri; // the floor triangle under the actor
    /* 0x19E */ u16 floorTriIndex;
    /* 0x1A0 */ u16 propNum;   // db_Object_array index + 1 of the prop stood on, 0 = none
    /* 0x1A2 */ s16 floorType;
    /* 0x1A4 */ ActorAimPoints aim;
    /* 0x1BC */ s16 unk1BC[3];   // a bone position (game_83300)
    /* 0x1C2 */ u8  pad1C2[0x2];
    /* 0x1C4 */ struct TimedEvent *events; // timed event list
    /* 0x1C8 */ u8  current_lod;       // current level of detail
    /* 0x1C9 */ u8  forced_lod; // requested LOD + 1, 0xFF = reset to 0
    /* 0x1CA */ u8  energy;
    /* 0x1CB */ u8  unk1CB;
    /* 0x1CC */ f32 unk1CC;
    /* 0x1D0 */ s8  unk1D0;
    /* 0x1D1 */ s8  unk1D1;
    /* 0x1D2 */ u16 pad1D2;
    /* 0x1D4 */ MtxF *ob_matrix_ptr;   // this frame's bone matrices
    /* 0x1D8 */ MtxF *prevBones; // last frame's copy (platforms)
    /* 0x1DC */ u8  unk1DC;
    /* 0x1DD */ u8  unk1DD[3];
    /* 0x1E0 */ u8  unk1E0[3];
    /* 0x1E3 */ u8  unk1E3;
    /* 0x1E4 */ u8  family;
    /* 0x1E5 */ u8  unk1E5;
    /* 0x1E6 */ u8  unk1E6;
    /* 0x1E7 */ u8  unk1E7;
    /* 0x1E8 */ u8  unk1E8;
    /* 0x1E9 */ u8  pad1E9;
    /* 0x1EA */ u16 unk1EA;
    /* 0x1EC */ u16 unk1EC;
    /* 0x1EE */ u8  pad1EE[0x2];
    /* 0x1F0 */ u32 pad1F0;
    /* 0x1F4 */ f32 unk1F4;
    /* 0x1F8 */ f32 unk1F8;
    /* 0x1FC */ u8  unk1FC;
    /* 0x1FD */ u8  unk1FD;
    /* 0x1FE */ u8  level;       // 0..255, moved by levelStep each update (func_15034860)
    /* 0x1FF */ u8  levelMode;   // 0: fall, 2: rise, else random walk (3: until levelTimer runs out)
    /* 0x200 */ u8  levelTimer;
    /* 0x201 */ u8  unk201;
    /* 0x202 */ s8  unk202;
    /* 0x203 */ s8  levelStep;
    /* 0x204 */ s16 level2;      // -255..255 (direction set by the sign of unk208)
    /* 0x206 */ u8  level2Mode;  // same encoding as levelMode
    /* 0x207 */ u8  level2Timer;
    /* 0x208 */ s8  unk208;
    /* 0x209 */ s8  level2Step;
    /* 0x20A */ u8  unk20A;
    /* 0x20B */ u8  unk20B[4];   // script variables (command 0x59)
    /* 0x20F */ u8  unk20F;
    /* 0x210 */ u8  unk210;
    /* 0x211 */ u8  unk211;
    /* 0x212 */ u8  unk212[0x2];
    /* 0x214 */ s16 unk214;
    /* 0x216 */ s16 unk216;      // distance bias (game_C4DC0)
    /* 0x218 */ ObjScriptCmd *scriptCmd; // current script command
    /* 0x21C */ u16 unk21C;
    /* 0x21E */ u8  unk21E;
    /* 0x21F */ u8  unk21F;
    /* 0x220 */ u8  unk220;
    /* 0x221 */ s8  unk221;
    /* 0x222 */ u8  unk222;
    /* 0x223 */ u8  unk223;
    /* 0x224 */ s16 unk224;
    /* 0x226 */ u8  unk226;
    /* 0x227 */ u8  pad227;
    /* 0x228 */ u8  unk228;
    /* 0x229 */ u8  unk229;
    /* 0x22A */ u8  unk22A;
    /* 0x22B */ u8  unk22B;
    /* 0x22C */ u16 unk22C;
    /* 0x22E */ u16 unk22E;   // cooldown
    /* 0x230 */ u8  pad230;
    /* 0x231 */ u8  unk231;
    /* 0x232 */ u8  intelwalk;
    /* 0x233 */ u8  unk233;
    /* 0x234 */ u8  unk234;
    /* 0x235 */ u8  unk235;
    /* 0x236 */ u8  unk236;
    /* 0x237 */ u8  unk237;
    /* 0x238 */ u8  unk238;
    /* 0x239 */ s8  intelhitroute;
    /* 0x23A */ u8  unk23A;
    /* 0x23B */ u8  unk23B;
    /* 0x23C */ u8  unk23C;
    /* 0x23D */ u8  unk23D;
    /* 0x23E */ u8  unk23E;
    /* 0x23F */ u8  unk23F;
    /* 0x240 */ u8  unk240;
    /* 0x241 */ u8  unk241;
    /* 0x242 */ u8  unk242;
    /* 0x243 */ u8  pad243;
    /* 0x244 */ s16 intel_animseq;
    /* 0x246 */ u8  intel_animspd;
    /* 0x247 */ u8  intel_accel;
    /* 0x248 */ u8  intel_decel;
    /* 0x249 */ u8  unk249;
    /* 0x24A */ u8  unk24A;
    /* 0x24B */ u8  pad24B;
    /* 0x24C */ u8  unk24C;
    /* 0x24D */ u8  unk24D;
    /* 0x24E */ u8  unk24E;
    /* 0x24F */ s8  unk24F;
    /* 0x250 */ u8  unk250;
    /* 0x251 */ u8  unk251;
    /* 0x252 */ u8  unk252;
    /* 0x253 */ u8  unk253;
    /* 0x254 */ u8  unk254;
    /* 0x255 */ u8  unk255;
    /* 0x256 */ u8  unk256;
    /* 0x257 */ u8  unk257;
    /* 0x258 */ u8  unk258;
    /* 0x259 */ u8  unk259[3];
    /* 0x25C */ s32 task_attrib;
    /* 0x260 */ VtxMorphState *morph; // vertex morph (game_1A8060), NULL when the model has none
    /* 0x264 */ s32 hit_vtx_array;      // allocations freed with the actor (func_15060F28)
    /* 0x268 */ s32 hit_box_array;
    /* 0x26C */ s32 hit_ylimit_array;
    /* 0x270 */ f32 cull_radius_xz;
    /* 0x274 */ u8  stand_on_char; // movement_obj index + 1 of the object stood on, 0 = none
    /* 0x275 */ u8  touching_char;  // movement_obj index + 1
    /* 0x276 */ u8  headEffect;      // head-turn effect kind, 0-6 (func_1503D9B4)
    /* 0x277 */ u8  pad277;
    /* 0x278 */ u16 headEffectState; // effect state
    /* 0x27A */ s16 headYaw[2];   // yaw of head parts 1 and 0
    /* 0x27E */ s16 headPitch[2]; // pitch of head parts 1 and 0
    /* 0x282 */ u16 headEffectTimer; // effect frames left, 0xFFFF = endless
    /* 0x284 */ s8  unk284;      // effect offset for headYaw[0]
    /* 0x285 */ s8  unk285;      // ... for headPitch[0]
    /* 0x286 */ s8  unk286;      // ... for headYaw[1]
    /* 0x287 */ s8  unk287;      // ... for headPitch[1]
    /* 0x288 */ u8  unk288;
    /* 0x289 */ u8  pad289[0x3];
    /* 0x28C */ u8 *modelData[7][2]; // per model in modelIds, per frame buffer
    /* 0x2C4 */ u8 *modelIds;
    /* 0x2C8 */ u8  lodCount;   // models in modelIds that are LODs
    /* 0x2C9 */ u8  modelCount; // lodCount + extra sub-models
    /* 0x2CA */ s8  unk2CA;
    /* 0x2CB */ s8  unk2CB;
    /* 0x2CC */ s32 unk2CC;
    /* 0x2D0 */ AnimInstance *unk2D0;
    /* 0x2D4 */ EffectHeader *unk2D4;
    /* 0x2D8 */ f32 unk2D8[3];   // launch velocity (y, gravity, xz) for func_15071FDC, set by script command 0x22;
                                 // [0]/[1] are also a fade timer/rate (func_150DFDD0, game_F15D0)
    /* 0x2E4 */ s32 taskint[3]; // per-type state; game_EB340 saves [0]/[1] as commands 0xE/0x68;
                                // [2] is nonzero while the unk2D8 fade runs
    /* 0x2F0 */ u32 pad2F0;
    /* 0x2F4 */ Attachment *attachments; // head of the attachment list
    /* 0x2F8 */ u16 unk2F8; // wait animation?
    /* 0x2FA */ u8  unk2FA;
    /* 0x2FB */ u8  unk2FB;
    /* 0x2FC */ u8  unk2FC;
    /* 0x2FD */ u8  unk2FD;
    /* 0x2FE */ u8  unk2FE;
    /* 0x2FF */ u8  unk2FF;
    /* 0x300 */ u8  unk300;
    /* 0x301 */ u8  unk301;
    /* 0x302 */ u8  unk302;
    /* 0x303 */ u8  unk303;
    /* 0x304 */ struct PointLightGfx *unk304[4]; // one per view
    /* 0x314 */ Ambient *unk314; // one per frame buffer (func_1515D440)
    /* 0x318 */ Camera *CameraPtr;
    /* 0x31C */ ActorCtl *mv;
    /* 0x320 */ u32 unk320;
    /* 0x324 */ struct CamData *camData; // aim easing state, one entry per aim slot
    /* 0x328 */ s8  unk328;
    /* 0x329 */ u8  pad329[0x3];
}; // size 0x32C => 812 bytes game_object

/*
 * A world object: one 0xA0-byte entry of the level's object array db_Object_array (gWorldObjectCount entries),
 * built by func_150039E0 (game_30E90.c) from the level's placement lists. Owner type 1 for effects and trails.
 * original: db_Object_struct (rom_debug_struct.c)
 */
typedef struct Shadow Shadow; // blob shadow under an object, see game_13AE20.c

/* WorldObject.task_flag (original state names from the debugger's db_Object viewer) */
enum TaskFlag {
    TASK_FLAG_OFF = 0,
    TASK_FLAG_GOING_OFF = 1,
    TASK_FLAG_GOING_ON = 2,
    TASK_FLAG_ON = 3,
    TASK_FLAG_STATE = 3,   // mask of the state bits
    TASK_FLAG_LOCKED = 4   // the viewer shows "locked" / "unlocked"
};

typedef struct WorldObject WorldObject;
struct WorldObject {
    /* 0x00 */ f32 t;                // x rotation, degrees
    /* 0x04 */ f32 u;                // y rotation
    /* 0x08 */ f32 v;                // z rotation
    /* 0x0C */ void *model;          // loaded model asset, if any
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u16 numVertices;
    /* 0x18 */ f32 yOffset;
    /* 0x1C */ Gfx *displayList;
    /* 0x20 */ Vtx *vertices[2];     // per frame buffer (D_800BE9C0); both srcVertices unless state bit 0
    /* 0x28 */ Vtx *srcVertices;     // the model's own vertices
    /* 0x2C */ f32 xScale;
    /* 0x30 */ f32 yScale;
    /* 0x34 */ f32 zScale;
    /* 0x38 */ void (*moveProc)(WorldObject *obj); // per-frame behaviour
    /* 0x3C */ union {
        void *data;                  // per-kind data, e.g. a Turret15112520Cfg
        s32 word;
        struct {
            s16 hi;
            s16 lo;
        } half;
        struct {
            u16 hi;
            u16 lo;
        } uhalf;
        u8 bytes[4];
    } moveData;                      // the placement's parameter word
    /* 0x40 */ s32 floorHitAttrib;   // collision material, used when materials is NULL
    /* 0x44 */ s32 *materials;       // per-triangle materials, from firstTri on
    /* 0x48 */ u8 pad48[4];
    /* 0x4C */ u8 unk4C;            // 0: never drawn (func_15113C88)
    /* 0x4D */ u8 shadowSize;         // blob shadow radius in 4-unit steps, times xScale (game_13AE20.c)
    /* 0x4E */ u8 attach;            // < 3: own transform, 3: none, 101..124: rides actor movement_obj[attach - 100]
    /* 0x4F */ u8 object_status;
    /* 0x50 */ u16 cullRadiusXZ;     // bounding radius: view culling width and placement scatter, see func_15004AAC
    /* 0x52 */ u16 cullRadiusY;      // view culling height
    /* 0x54 */ u16 modelId;          // | 0x8000 for objects without a loaded model
    /* 0x56 */ u16 numTris;          // collision triangles, see func_15003668
    /* 0x58 */ u16 firstTri;
    /* 0x5A */ s16 moveX;            // translation this frame
    /* 0x5C */ s16 moveY;
    /* 0x5E */ s16 moveZ;
    /* 0x60 */ f32 spinX;            // rotation this frame (degrees)
    /* 0x64 */ f32 spinY;
    /* 0x68 */ f32 spinZ;
    /* 0x6C */ u16 effectFlags;
    /* 0x6E */ u8 inactive;          // 1: not updated (finished or removed), no floor probe
    /* 0x6F */ u8 viewFlags;         // bits 0-3: visible in view N, 0x40: update this frame, 0x80: hidden
    /* 0x70 */ u8 state;             // 1: vertices transformed into per-frame copies, 2: drawn by func_150A50C0,
                                     // 8: removed, high nibble: objects following in its group
    /* 0x71 */ u8 object_num;        // own index in db_Object_array
    /* 0x72 */ u8 unique_id;         // found by func_151149AC
    /* 0x73 */ u8 task_flag;         // enum TaskFlag: bits 0-1 state, bit 2 locked
    /* 0x74 */ u16 sound_id;         // handle from func_15114D24
    /* 0x76 */ u8 pad76[2];
    /* 0x78 */ void (*onContact)(WorldObject *obj, Actor *actor);
    /* 0x7C */ union {
        struct {
            /* 0x7C */ s32 a;
            /* 0x80 */ s32 b;
            /* 0x84 */ s32 c;
        } i;                         // original: touch.i.a/b/c
        struct {
            /* 0x7C */ f32 a;
            /* 0x80 */ f32 b;
            /* 0x84 */ f32 c;
        } f;                         // float view (not named by the debugger)
        struct {
            /* 0x7C */ s32 unk7C;
            /* 0x80 */ void *tri;    // floor probe results, see func_150045C4
            /* 0x84 */ f32 y;
        } floor;
    } touch;                         // per-behaviour scratch state
    /* 0x88 */ u16 fadeDistance;     // fades out when the camera is closer than this
    /* 0x8A */ u8 alpha;
    /* 0x8B */ u8 dst_fade_alpha[4]; // per view (the debugger labels them [1]..[4])
    /* 0x8F */ u8 pad8F;
    /* 0x90 */ u16 drawDistance;     // 0: default (D_800D9B1C), 1: 40000
    /* 0x92 */ u8 contactNonPlayers;  // nonzero: onContact also runs for actors that are not players (ObjectBehavior)
    /* 0x93 */ u8 pad93;
    /* 0x94 */ Shadow *shadow;       // created and freed by func_15113C88
    /* 0x98 */ s32 unk98;            // with unk9C, passed by address to func_1503F800
    /* 0x9C */ s32 unk9C;
}; // size 0xA0

typedef struct {
    u8 pad[0x1b];
    u8 unk1B;
} struct133;

/* Camera command playback state (Camera.cmdState, at 0x6CC). */
typedef struct CamCmdState {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 cmdId;                            /* id (cmd.id) of the command last started */
    /* 0x04 */ s16 lastType;                         /* cmd.type of the last command run */
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 blending;                         /* still easing back (blend < 1) after a type 1 command */
    /* 0x0A */ u8 padA[0x6];
    /* 0x10 */ f32 blend;                            /* 0..1 easing factor */
    /* 0x14 */ f32 pushRate;                         /* 0..0.04, scales the minimum-distance push */
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 minHeight;
    /* 0x28 */ f32 maxHeight;
    /* 0x2C */ f32 maxDistance;
} CamCmdState; // size 0x30

/* Current camera command (Camera.cmd, at 0x6FC). Type 15 is a rail (see CamRail). */
typedef struct CamCmd {
    /* 0x00 */ u32 type;
    /* 0x04 */ s16 id;
    /* 0x06 */ u8 pad6[0x2];
    /* 0x08 */ u32 unk8;                             /* type 7: path options (copied to Camera.RailFlags); type 15: rail height offset */
    /* 0x0C */ u8 padC[0x4];
    /* 0x10 */ s16 x;                                /* type 15: rail midpoint */
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 halfLength;                       /* type 15: half the rail length */
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u32 unk1C;                            /* type 15: rail offset mode (copied to rail.offsetMode) */
    /* 0x20 */ u32 unk20;                            /* type 15: rail offset (copied to rail.offset) */
    /* 0x24 */ f32 unk24;                            /* type 15: unk24/unk28 split halfLength into horizontal/vertical parts, */
    /* 0x28 */ f32 unk28;                            /* unk2C/unk30 split the horizontal part into x/z */
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 pad36[0x2];
} CamCmd; // size 0x38

/* Four-byte entry of a game command stream (an opcode and its value), as written by the bone command
 * builders (game_61490.c, func_150344A0, func_15034860) and their per-actor helpers (func_150B58F0,
 * func_150F1B48). An opcode of 0x1000 ends the stream. */
typedef struct GameCommand {
    /* 0x0 */ s16 opcode;
    /* 0x2 */ s16 value;
} GameCommand; /* size 0x4 */

/* Aim (first-person swing) state (Camera.aim, at 0x740). */
typedef struct CamAim {
    /* 0x00 */ Vec3f orbitPos;                      /* eye position on the orbit around the target */
    /* 0x0C */ Vec3f offset;                        /* sideways shift of the aim ray (x and z only) */
    /* 0x18 */ f32 yawVel;
    /* 0x1C */ f32 pitchVel;
    /* 0x20 */ f32 savedYaw;                         /* view to return to on exit */
    /* 0x24 */ f32 savedTargetDistance;
    /* 0x28 */ f32 savedHeight;
    /* 0x2C */ f32 savedTightness;
    /* 0x30 */ f32 savedUnk354;
    /* 0x34 */ f32 targetYaw;
    /* 0x38 */ f32 targetPitch;
    /* 0x3C */ f32 pitch;
    /* 0x40 */ f32 signedPitch;                      /* pitch in -180..180 */
    /* 0x44 */ f32 distance;
    /* 0x48 */ f32 turnDistance;                     /* swing round once Camera.TripodMagnitude passes this */
    /* 0x4C */ u8 useOverride;                       /* take pitch, yaw offset and distance from the fields below */
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ s8 approachDir;                       /* -1: distance grows towards turnDistance, 1: it shrinks */
    /* 0x4F */ u8 pad4F[0x1];
    /* 0x50 */ f32 overridePitch;
    /* 0x54 */ f32 overrideYawOffset;
    /* 0x58 */ f32 overrideDistance;
    /* 0x5C */ f32 offsetXVel;
    /* 0x60 */ f32 offsetZVel;
    /* 0x64 */ s32 stickLock;                        /* bit 0: stick_x held at <= 0, bit 1: at >= 0 */
} CamAim; // size 0x68

/* Smooth transition of the camera to a new target/yaw (func_15128030; Camera.transition, at 0x7F4). */
typedef struct CamTransition {
    /* 0x00 */ u16 state;
    /* 0x02 */ u8 pad2[0x2];
    /* 0x04 */ Vec3f startTarget;
    /* 0x10 */ Vec3f endTarget;
    /* 0x1C */ f32 startDistance;
    /* 0x20 */ f32 startYaw;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 endDistance;
    /* 0x2C */ f32 endYaw;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 endYawAngle;
    /* 0x38 */ f32 endAPitchAngle;
    /* 0x3C */ f32 progress;
    /* 0x40 */ u16 duration;
    /* 0x42 */ u8 pad42[0x2];
    /* 0x44 */ f32 LerpHeightTarget;
    /* 0x48 */ f32 LerpHeightStarting;
    /* 0x4C */ s16 LerpCurrentFrames;
    /* 0x4E */ u8 pad4E[0x2];
} CamTransition; // size 0x50

/* Camera rail: a line segment the eye slides along (Camera.rail, at 0x870; set up by a type 15 command). */
typedef struct CamRail {
    /* 0x00 */ u8 offsetMode;                        /* which side of the rail the eye sits on (0..3) */
    /* 0x01 */ u8 pad1[0x3];
    /* 0x04 */ Vec3f start;
    /* 0x10 */ Vec3f dir;                           /* unit direction from start to end */
    /* 0x1C */ f32 invLength;
    /* 0x20 */ f32 offset;                           /* horizontal distance of the eye from the rail */
    /* 0x24 */ f32 heightOffset;
    /* 0x28 */ f32 eyeEaseA;                         /* easing parameters for the eye, eased toward 10.0 */
    /* 0x2C */ f32 eyeEaseB;
    /* 0x30 */ f32 eyeEaseAVel;
    /* 0x34 */ f32 eyeEaseBVel;
    /* 0x38 */ Vec3f eyeGoal;
} CamRail; // size 0x44

/* Camera mode cache slots: the index into Camera.MCache[] / FCache[] and the other per-mode caches (the
   `slot` argument of func_15123934 / func_151239CC). Original names from the Oct 2000 debugger
   (MCache[NORMAL] ...); slot 18 is not shown there. */
typedef enum CameraMode {
    /*  0 */ CAMERA_MODE_NORMAL,
    /*  1 */ CAMERA_MODE_AIM,
    /*  2 */ CAMERA_MODE_ZONES,
    /*  3 */ CAMERA_MODE_TRANSPORT,
    /*  4 */ CAMERA_MODE_ANIM,
    /*  5 */ CAMERA_MODE_PAUSE,
    /*  6 */ CAMERA_MODE_RAIL,
    /*  7 */ CAMERA_MODE_ZONE_FLAGS,
    /*  8 */ CAMERA_MODE_PICKED_UP,
    /*  9 */ CAMERA_MODE_DOOR,
    /* 10 */ CAMERA_MODE_CUTSCENE,
    /* 11 */ CAMERA_MODE_DEBUG,
    /* 12 */ CAMERA_MODE_FIXED,
    /* 13 */ CAMERA_MODE_SFLY,
    /* 14 */ CAMERA_MODE_GUIDE_RAIL,
    /* 15 */ CAMERA_MODE_BEHIND_BACK,
    /* 16 */ CAMERA_MODE_BOMB,
    /* 17 */ CAMERA_MODE_TASK,
    /* 18 */ CAMERA_MODE_UNK18,
    /* 19 */ CAMERA_MODE_ROB_MODES,
    /* 20 */ CAMERA_MODE_ON_LADDER,
    /* 21 */ CAMERA_MODE_COUNT
} CameraMode;

/* One player camera (Actor.camera; array at camera_global_Cameras). original: Camera struct (rom_debug_struct.c).
   0x000..0x236: the current mode state, each value followed by its per-mode cache (indexed by CameraMode);
   func_15123934 saves it to a slot and func_151239CC restores it. */
struct Camera {
    /* 0x000 */ u16 ModeIndex;                        /* bit index of mode */
    /* 0x002 */ u16 savedModeIndex[CAMERA_MODE_COUNT];
    /* 0x02C */ s32 Mode;                             /* one-hot mode bit; 0x100 is the R-trigger camera */
    /* 0x030 */ s32 MCache[CAMERA_MODE_COUNT];
    /* 0x084 */ s32 AvailableModes;
    /* 0x088 */ s32 savedAvailableModes[CAMERA_MODE_COUNT];
    /* 0x0DC */ s32 Motion;
    /* 0x0E0 */ s32 savedMotion[CAMERA_MODE_COUNT];
    /* 0x134 */ s32 Tightness;
    /* 0x138 */ s32 savedTightness[CAMERA_MODE_COUNT];
    /* 0x18C */ f32 YOffset;                          /* vertical offset of eye and target */
    /* 0x190 */ f32 YOffsetTget;
    /* 0x194 */ f32 XZOffset;                         /* sideways offset of eye and target */
    /* 0x198 */ f32 XZOffsetTget;
    /* 0x19C */ f32 FOVX;
    /* 0x1A0 */ f32 FOVY;
    /* 0x1A4 */ f32 TgetFOVX;
    /* 0x1A8 */ f32 TgetFOVY;
    /* 0x1AC */ f32 LastFOVX;                         /* not accessed by US code */
    /* 0x1B0 */ f32 LastFOVY;                         /* not accessed by US code */
    /* 0x1B4 */ s16 DistIndex;                        /* row of D_800A34B0 (distance, height) */
    /* 0x1B6 */ s16 savedDistIndex[CAMERA_MODE_COUNT];
    /* 0x1E0 */ s16 DistMask;                         /* presets reachable with the C buttons */
    /* 0x1E2 */ s16 savedDistMask[CAMERA_MODE_COUNT];
    /* 0x20C */ s16 FCache[CAMERA_MODE_COUNT];
    /* 0x236 */ u8 pad236[0x2];
    /* 0x238 */ s32 LastFrame;
    /* 0x23C */ u8 Snap;                              /* nonzero: jump to targets instead of easing (counts down) */
    /* 0x23D */ u8 CNumber;                           /* this camera's slot in the per-camera tables */
    /* 0x23E */ u8 CharContext;                       /* copy of mv->cameraZone */
    /* 0x23F */ u8 unk23F;
    /* 0x240 */ s32 SpecialMove;
    /* 0x244 */ s16 InterestCharIndex;                /* second object kept in frame (0 = none) */
    /* 0x246 */ u8 pad246[0x2];
    /* 0x248 */ Actor *InterestCharPtr;
    /* 0x24C */ f32 DistSpeedXZ;
    /* 0x250 */ f32 DistSpeedY;
    /* 0x254 */ f32 DistSpeedXZVel;                   /* not accessed by US code */
    /* 0x258 */ f32 XZVel;
    /* 0x25C */ f32 YVel;
    /* 0x260 */ f32 LOSVel;                           /* not accessed by US code */
    /* 0x264 */ f32 RollVel;
    /* 0x268 */ f32 ShortToMidt;                      /* not accessed by US code */
    /* 0x26C */ f32 SlackenK;
    /* 0x270 */ u8 pad270[0x2];
    /* 0x272 */ s16 FCameraIndex;
    /* 0x274 */ s16 FToFromStatus;
    /* 0x276 */ u8 pad276[0x2];
    /* 0x278 */ f32 FTgetRadius;
    /* 0x27C */ Vec3f unk27C;
    /* 0x288 */ Vec3f unk288;
    /* 0x294 */ f32 FixedVel;                         /* not accessed by US code */
    /* 0x298 */ s16 CharMoved;
    /* 0x29A */ u8 pad29A[0x2];
    /* 0x29C */ f32 DrunkRoll;
    /* 0x2A0 */ f32 LOSYVel;
    /* 0x2A4 */ Vec3f focus;                          /* point followed on the player */
    /* 0x2B0 */ Vec3f lastPlayerPos;
    /* 0x2BC */ Vec3f target;                         /* look-at point */
    /* 0x2C8 */ Vec3f prevTarget;
    /* 0x2D4 */ u8 pad2D4[0xC];
    /* 0x2E0 */ Vec3f savedTarget;
    /* 0x2EC */ Vec3f savedEye;
    /* 0x2F8 */ Vec3f eye;                            /* camera position */
    /* 0x304 */ Vec3f prevEye;
    /* 0x310 */ u8 pad310[0x34];
    /* 0x344 */ f32 unk344;
    /* 0x348 */ f32 height;
    /* 0x34C */ f32 unk34C;
    /* 0x350 */ u8 pad350[0x4];
    /* 0x354 */ f32 unk354;
    /* 0x358 */ f32 unk358;
    /* 0x35C */ f32 unk35C;
    /* 0x360 */ f32 waterY;                           /* water surface below the eye, -10000.0f = none; eye below it = underwater */
    /* 0x364 */ f32 zoneWaterY;                       /* water surface set by a zone (top of its box), -10000.0f = none */
    /* 0x368 */ s16 PadNumber;
    /* 0x36A */ u16 CameraButtons;
    /* 0x36C */ OSContPad *PadPtr;
    /* 0x370 */ f32 TripodMagnitude;
    /* 0x374 */ f32 IdealTripodMagnitude;
    /* 0x378 */ f32 IdealTripodMagLatch;              /* not accessed by US code */
    /* 0x37C */ f32 CameraAngle;                      /* degrees */
    /* 0x380 */ f32 TrueCameraAngle;
    /* 0x384 */ f32 LockedCameraAngle;
    /* 0x388 */ f32 CameraPitch;                      /* degrees */
    /* 0x38C */ f32 AddOnPitch;
    /* 0x390 */ f32 CharRelAngle;
    /* 0x394 */ f32 FCameraAngle;
    /* 0x398 */ f32 CameraPitchRad;
    /* 0x39C */ f32 CameraAngleRad;
    /* 0x3A0 */ f32 TrueCameraAngleRad;               /* TrueCameraAngle in radians */
    /* 0x3A4 */ f32 CutbackAngle;
    /* 0x3A8 */ f32 APitchAngle;
    /* 0x3AC */ f32 APitchUpperBound;                 /* not accessed by US code */
    /* 0x3B0 */ f32 APitchLowerBound;                 /* not accessed by US code */
    /* 0x3B4 */ f32 AccelA;
    /* 0x3B8 */ f32 AccelB;
    /* 0x3BC */ s32 AccelLevel;                       /* not accessed by US code */
    /* 0x3C0 */ f32 CurrentVelX;                      /* easing velocity of eye */
    /* 0x3C4 */ f32 CurrentVelY;
    /* 0x3C8 */ f32 CurrentVelZ;
    /* 0x3CC */ s16 CharIndex;
    /* 0x3CE */ u8 pad3CE[0x2];
    /* 0x3D0 */ Actor *CharPtr;                       /* the object the camera follows */
    /* 0x3D4 */ ActorCtl *mv;                         /* CharPtr->mv */
    /* 0x3D8 */ f32 LOSAdjustFactor;                  /* not accessed by US code */
    /* 0x3DC */ f32 SwimLockedRy;                     /* orbit yaw of the eye, radians */
    /* 0x3E0 */ f32 FZoneAdjustAngleRad;
    /* 0x3E4 */ f32 FZoneEnteredLatch;                /* not accessed by US code */
    /* 0x3E8 */ u8 TonyHitsClear;
    /* 0x3E9 */ u8 pad3E9[0x3];
    /* 0x3EC */ f32 SideHitsRxz;
    /* 0x3F0 */ s16 SHitsPIPOutTimer;
    /* 0x3F2 */ u8 pad3F2[0x2];
    /* 0x3F4 */ f32 SHitsPIPOutXZ;
    /* 0x3F8 */ f32 SHitsPIPOutY;
    /* 0x3FC */ f32 HitVelX;
    /* 0x400 */ f32 HitVelY;
    /* 0x404 */ f32 HitVelZ;
    /* 0x408 */ u8 pad408[0x1C4];
    /* 0x5CC */ f32 LockedCamAngleK;
    /* 0x5D0 */ f32 ClampYAngle;
    /* 0x5D4 */ s32 ClampYTop;
    /* 0x5D8 */ f32 ClampYBottom;
    /* 0x5DC */ f32 PanButtonAddOn;
    /* 0x5E0 */ f32 IdealYaw;                         /* not accessed by US code */
    /* 0x5E4 */ f32 CurrentYaw;                       /* not accessed by US code */
    /* 0x5E8 */ f32 YawAngle;
    /* 0x5EC */ f32 Roll;                             /* roll, degrees */
    /* 0x5F0 */ s32 SpecialFlags;
    /* 0x5F4 */ f32 CliffEdgeDelta;                   /* not accessed by US code */
    /* 0x5F8 */ s16 SideHitsClear;
    /* 0x5FA */ s16 UDHitsCounter;
    /* 0x5FC */ s16 SHitsOffLastFrame;
    /* 0x5FE */ s16 unk5FE;
    /* 0x600 */ u8 blocked;                           /* wall between focus and eye this frame */
    /* 0x601 */ u8 pad601[0x3];
    /* 0x604 */ f32 blockedDistance;
    /* 0x608 */ u8 pad608[0x4];
    /* 0x60C */ f32 CurrentHitsWidth;
    /* 0x610 */ f32 CurrentHitsHeight;
    /* 0x614 */ Vec3f *PivotPtr;
    /* 0x618 */ Vec3f pivotPos;                       /* point PivotPtr can be set to (camera commands) */
    /* 0x624 */ u8 pad624[0x4];
    /* 0x628 */ s32 unk628;
    /* 0x62C */ f32 unk62C;
    /* 0x630 */ f32 AutoRotateAngleVel;               /* yaw velocity while turning toward PivotPtr (func_15122170) */
    /* 0x634 */ s16 CurrentChainIndex;                /* not accessed by US code */
    /* 0x636 */ u8 pad636[0x2];
    /* 0x638 */ s32 ForceAttribute;                   /* not accessed by US code */
    /* 0x63C */ s32 AttributeSet;                     /* not accessed by US code */
    /* 0x640 */ u32 Attributes;
    /* 0x644 */ void *TrianglePtr;                    /* floor triangle under the eye (the floor probe's outTri) */
    /* 0x648 */ CollisionTri floorTri;                /* its vertices */
    /* 0x65A */ u8 pad65A[0x2];
    /* 0x65C */ f32 CentPitchAngleVel;
    /* 0x660 */ f32 CentYawAngleVel;
    /* 0x664 */ Vec3f forward;
    /* 0x670 */ f32 CharTrackingSpeed;
    /* 0x674 */ f32 CharTrackingSpeedTarget;
    /* 0x678 */ f32 CharTrackingSpeedVel;
    /* 0x67C */ f32 NearPlane;
    /* 0x680 */ f32 BigGTargetXZ;                     /* not accessed by US code */
    /* 0x684 */ f32 BigGTargetY;                      /* not accessed by US code */
    /* 0x688 */ f32 BigGMagXZ;                        /* not accessed by US code */
    /* 0x68C */ f32 BigGMagY;                         /* not accessed by US code */
    /* 0x690 */ s32 BigGOnce;                         /* not accessed by US code */
    /* 0x694 */ f32 BigGRecoil;                       /* not accessed by US code */
    /* 0x698 */ s32 unk698;
    /* 0x69C */ u8 pad69C[0x14];
    /* 0x6B0 */ s32 orbitDir;                          /* manual orbit direction, +1 or -1 (func_1512D390) */
    /* 0x6B4 */ u8 pad6B4[0xC];
    /* 0x6C0 */ s32 orbitCounter;
    /* 0x6C4 */ f32 orbitSpeed;                       /* degrees per frame, clamped to +-10 */
    /* 0x6C8 */ s32 cmdRunning;
    /* 0x6CC */ CamCmdState cmdState;
    /* 0x6FC */ CamCmd cmd;
    /* 0x734 */ s32 LastMagsZone;
    /* 0x738 */ s32 LastMagsZoneConsider;
    /* 0x73C */ s16 DoingAim;
    /* 0x73E */ u8 pad73E[0x2];
    /* 0x740 */ CamAim aim;
    /* 0x7A8 */ u8 pad7A8[0x8];
    /* 0x7B0 */ f32 WaterDx;
    /* 0x7B4 */ f32 SpringTime;                       /* frame time scale */
    /* 0x7B8 */ s32 ZoomCounter;
    /* 0x7BC */ f32 CentreBias;
    /* 0x7C0 */ f32 CentreBiasTarget;
    /* 0x7C4 */ f32 CentreBiasVel;                    /* not accessed by US code */
    /* 0x7C8 */ f32 AutoBehindAngleVel;               /* velocity of CameraAngle while func_15049688 eases it (game_14F130.c, game_123FB0.c) */
    /* 0x7CC */ s32 DelayOneFrame;
    /* 0x7D0 */ f32 OffsetXZVel;
    /* 0x7D4 */ f32 OffsetYVel;
    /* 0x7D8 */ f32 WaterEffectVel;
    /* 0x7DC */ f32 WaterEffectTargetAmp;
    /* 0x7E0 */ f32 MatrixEffectDirection;
    /* 0x7E4 */ u8 DoingSpecialContextCase;
    /* 0x7E5 */ u8 pad7E5[0x1];
    /* 0x7E6 */ u16 AlignBehindCounter;
    /* 0x7E8 */ f32 CameraRadius;
    /* 0x7EC */ f32 CameraRadiusAngle;                /* not accessed by US code */
    /* 0x7F0 */ f32 CameraRadiusAngleVel;             /* not accessed by US code */
    /* 0x7F4 */ CamTransition transition;
    /* 0x844 */ f32 LadderAngleVel;
    /* 0x848 */ u8 unk848;
    /* 0x849 */ u8 pad849[0x1];
    /* 0x84A */ s16 AimTimeOuts;                      /* frames left before the aim swing gives up */
    /* 0x84C */ u8 AimTargetOn;                       /* func_151C9BA0 ran; func_151C9ED4 undoes it */
    /* 0x84D */ u8 DoingDeltas;                       /* 0 idle, 1 start requested, 2 playing */
    /* 0x84E */ u8 DeltaCurrentIndex;
    /* 0x84F */ u8 pad84F[0x1];
    /* 0x850 */ s32 DeltaPlaying;                     /* which D_800DC280 table */
    /* 0x854 */ f32 DeltaRoll;
    /* 0x858 */ f32 DeltaPitch;
    /* 0x85C */ f32 DeltaYaw;
    /* 0x860 */ f32 DeltaX;
    /* 0x864 */ f32 DeltaY;
    /* 0x868 */ f32 DeltaZ;
    /* 0x86C */ f32 ForceSwimAngle;                   /* degrees, 999.0f = none */
    /* 0x870 */ CamRail rail;
    /* 0x8B4 */ f32 LastGunRecoil;
    /* 0x8B8 */ u8 HitsZBufferReadFlag;
    /* 0x8B9 */ u8 pad8B9[0x1];
    /* 0x8BA */ s16 HitsZBufferRowIndex;
    /* 0x8BC */ u16 *HitsZBufferHoriDataPtr;
    /* 0x8C0 */ f32 AutoAlignTargetYawVel;            /* yaw velocity for func_15121C00 */
    /* 0x8C4 */ f32 TargetRopeDisp;
    /* 0x8C8 */ f32 RopeDispVel;
    /* 0x8CC */ f32 RopeDisp;
    /* 0x8D0 */ s32 RopeDirection;
    /* 0x8D4 */ f32 PredictSpeedTarget;
    /* 0x8D8 */ f32 PredictSpeedVel;
    /* 0x8DC */ f32 PredictSpeed;                     /* how fast CameraAngle turns toward the target */
    /* 0x8E0 */ f32 AlignBehindAngleVel;
    /* 0x8E4 */ u8 DoingAlignBehind;
    /* 0x8E5 */ u8 pad8E5[0x3];
    /* 0x8E8 */ s32 AlignBehindTimeOut;
    /* 0x8EC */ u8 LockedBehind;
    /* 0x8ED */ u8 DontMoveTilCharacterDoes;
    /* 0x8EE */ u8 pad8EE[0x2];
    /* 0x8F0 */ f32 RailWantedX;
    /* 0x8F4 */ f32 RailWantedXVel;
    /* 0x8F8 */ f32 RailWantedY;
    /* 0x8FC */ f32 RailWantedYVel;
    /* 0x900 */ f32 RailWantedZ;
    /* 0x904 */ f32 RailWantedZVel;
    /* 0x908 */ f32 RailAccelA;
    /* 0x90C */ f32 RailAccelB;
    /* 0x910 */ f32 RailAccelATarget;
    /* 0x914 */ f32 RailAccelBTarget;
    /* 0x918 */ f32 RailAccelAVel;
    /* 0x91C */ f32 RailAccelBVel;
    /* 0x920 */ u16 RailFlags;                        /* type 7 command options (cmd.unk8) */
    /* 0x922 */ u8 pad922[0x2];
    /* 0x924 */ f32 RailMagXZ;
    /* 0x928 */ f32 RailMagY;
    /* 0x92C */ u8 DoingRail;
    /* 0x92D */ u8 pad92D[0x3];
    /* 0x930 */ f32 unk930;
    /* 0x934 */ f32 unk934;
    /* 0x938 */ f32 unk938;
    /* 0x93C */ Vec3f unk93C;
    /* 0x948 */ u8 unk948;
    /* 0x949 */ u8 unk949;
    /* 0x94A */ u8 pad94A[0x2];
    /* 0x94C */ f32 unk94C;
    /* 0x950 */ f32 unk950;
    /* 0x954 */ f32 unk954;
    /* 0x958 */ f32 unk958;
    /* 0x95C */ f32 unk95C;
    /* 0x960 */ f32 unk960;
    /* 0x964 */ f32 unk964;
    /* 0x968 */ f32 unk968;
    /* 0x96C */ f32 speed;                            /* yaw easing speed and limit for func_15049688 (game_14F130.c) */
    /* 0x970 */ f32 limit;
    /* 0x974 */ f32 unk974;
    /* 0x978 */ f32 unk978;
    /* 0x97C */ f32 targetSpeed;
    /* 0x980 */ f32 targetLimit;
    /* 0x984 */ f32 unk984;
    /* 0x988 */ f32 unk988;
    /* 0x98C */ f32 speedVel;
    /* 0x990 */ f32 limitVel;
    /* 0x994 */ f32 sideOffset;
    /* 0x998 */ f32 sideOffsetVel;
    /* 0x99C */ f32 sideOffsetTarget;
}; // size 0x9A0

/* An aim point (gAimPoints[gAimPointCount]): a sphere that objects can aim at, built by aim_loadPoints from an
 * AimPointDef asset. minX/maxX cache the x extent for a quick reject. */
typedef struct AimPoint {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 flags;    /* 2: removed (aim_removePoint) */
    /* 0x08 */ u32 radiusSq;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 maxX;     /* x + radius */
    /* 0x10 */ s16 minX;     /* x - radius */
    /* 0x12 */ s16 unk12;
} AimPoint; // size 0x14

/* One aim point as stored in its level asset (converted to AimPoint on load). */
typedef struct AimPointDef {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} AimPointDef; // size 0x18

/* A cutscene subtitle slot, D_800C3CA0[3] (see func_150234A4). 0x04..0x33 is the TextBoxConfig passed to func_1516A7B0. */
typedef struct {
    /* 0x00 */ s16 active; /* owning track + 1, 0 = free */
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ s32 flags;
    /* 0x08 */ u8 *text;
    /* 0x0C */ u8 isOpen;
    /* 0x0D */ u8 padD[0xF];
    /* 0x1C */ f32 *unk1C;
    /* 0x20 */ f32 *unk20;
    /* 0x24 */ f32 *unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ u8 r;
    /* 0x2D */ u8 g;
    /* 0x2E */ u8 b;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ u8 pad30[4];
    /* 0x34 */ TextBox *handle; /* func_1516A7B0 */
} SeqSubtitle; // size 0x38

/* Summary of one of the three save files the player can pick. */
typedef struct SaveSlotInfo {
    /* 0x00 */ s32 playTime;   /* D_800BE3E8, in seconds */
    /* 0x04 */ u32 unk4;       /* D_800BE3E4 */
    /* 0x08 */ s32 sequence;   /* incremented on every save, -1 when the slot is empty */
    /* 0x0C */ u8 unkC;        /* D_800BE3DF */
    /* 0x0D */ u8 flagCount;   /* number of bits set in the 27 bytes at D_800D2E4C */
    /* 0x0E */ s8 eepromSlot;  /* which of the four EEPROM save files holds it */
    /* 0x0F */ s8 padF;
} SaveSlotInfo; /* size = 0x10 */

/* A texture as the sprite and tile loaders take it (D_800D2C90 in game_C2350.c is the shared one). */
typedef struct Texture {
    /* 0x0 */ u32 addr;  // RAM address, or an asset id below 0x10000000
    /* 0x4 */ u16 width;
    /* 0x6 */ u16 height;
    /* 0x8 */ u8 fmt;    // G_IM_FMT_*; 5 = colour image plus a second 4-bit tile
    /* 0x9 */ u8 siz;    // G_IM_SIZ_*
    /* 0xA */ u8 frames; // animation frames (0 = still)
} Texture; /* size 0xC */

/* A sprite to draw (see func_15095D34, game_C2350.c). func_15095060 points tex at the shared texture;
 * func_15095B08 scales the texture's size by scaleX and scaleY. */
typedef struct SpriteParams {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 scaleX;
    /* 0x08 */ s16 scaleY;
    /* 0x0A */ u8 alpha;
    /* 0x0B */ u8 animSpeed;
    /* 0x0C */ u8 animTimer; // 8 ticks per frame; bit 7 = playing backwards (ping-pong textures)
    /* 0x0D */ u8 flip;      // bit 0: flip s, bit 1: flip t
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ Texture *tex;
} SpriteParams; /* size 0x14 */

/* A texture as stored in the asset tables, converted into a Texture by func_15095060 (game_C2350.c). */
typedef struct TextureDesc {
    /* 0x0 */ u32 addr;  // asset id, or a pointer to a table of per-frame addresses
    /* 0x4 */ u8 frames;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u16 width;
    /* 0x8 */ u16 height;
    /* 0xA */ u8 fmt;
    /* 0xB */ u8 siz;
} TextureDesc; /* size 0xC */

/* The texture coordinates of one vertex (Vtx.v.tc); D_800C6360[id] saves a model's originals. */
typedef struct TexCoord {
    /* 0x0 */ s16 s;
    /* 0x2 */ s16 t;
} TexCoord;

/* The resource picked for one of the 15 particle sprite types (D_800BE4A0[15], game_36680.c). */
typedef struct ParticleResource {
    /* 0x0 */ u16 id; // 0xFF: none
    /* 0x2 */ u16 flags;
} ParticleResource;

/* The texture file of one particle sprite type (D_800DDA90[15]; loaded by game_1A89B0.c). */
typedef struct ParticleTexFile {
    /* 0x0 */ s32 devAddr;
    /* 0x4 */ s32 size;
    /* 0x8 */ void *buf;
    /* 0xC */ u8  used; // set when a sprite of this type was built this frame
    /* 0xD */ u8  padD[3];
} ParticleTexFile; // size 0x10

/* Per-model blood settings, indexed by func_15134070's profile index (the table D_800A3FD8, game_161520.c). */
typedef struct BloodProfile {
    /* 0x0 */ Vec3f offset; // where blood comes from, relative to the actor's bone 12
    /* 0xC */ u16 chunkModel; // model of the gore chunk thrown off the actor (func_1513264C)
    /* 0xE */ u8 unkE; // 2: no blood (sprays and drips are skipped)
    /* 0xF */ u8 padF;
} BloodProfile; /* size 0x10 */

/* A camera message payload (func_1512D560 types 7, 8 and 10, applied by func_15128CB0 in game_156160.c);
 * gCamShot is the shared one that cutscenes and TriggerZones fill in. */
typedef struct CamShot {
    /* 0x00 */ s32 mode;     /* type 7: 0 or 2 = look at at.focus; otherwise aim by at.angles */
    /* 0x04 */ s32 unk4;     /* index into D_800C35EA (cutscene scene) */
    /* 0x08 */ f32 unk8;     /* copied to D_800C3648, the FOV offset, when that scene is active */
    /* 0x0C */ f32 yaw;      /* types 8 and 10: the end yaw; 999.0f = behind the player */
    /* 0x10 */ s32 duration; /* type 10: transition length */
    /* 0x14 */ Vec3f eye;
    /* 0x20 */ union {
        Vec3f focus;
        struct {
            f32 sway;
            f32 pitch;
            f32 yaw;
        } angles;            /* degrees */
    } at;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
} CamShot; // size 0x38 (D_800C3638 follows)

/* Zone types: TriggerZone.kind >> 2. Original names (Oct 2000 debugger table); the US spawn table
 * gZoneSpawnHandlers has the same 26 entries, and the users agree (0 camera, 2 sound, 7 music channel, 8 warp,
 * 17 glow particles, 24 local directional light). */
enum ZoneType {
    ZONE_TYPE_CAMERA,                  /* 0x00 */
    ZONE_TYPE_CHAR_FLAGS,              /* 0x01 */
    ZONE_TYPE_SOUND,                   /* 0x02 */
    ZONE_TYPE_NEUTRAL,                 /* 0x03 */
    ZONE_TYPE_MINI_SHAPE,              /* 0x04 */
    ZONE_TYPE_BADDY_GROUP,             /* 0x05 */
    ZONE_TYPE_REFLECTION,              /* 0x06 */
    ZONE_TYPE_TUNE_CHANNEL,            /* 0x07 */
    ZONE_TYPE_WARP,                    /* 0x08 */
    ZONE_TYPE_LIGHT_SETUP,             /* 0x09 */
    ZONE_TYPE_LIGHT_CONTROL,           /* 0x0A */
    ZONE_TYPE_CONTEXT,                 /* 0x0B */
    ZONE_TYPE_EFFECT,                  /* 0x0C */
    ZONE_TYPE_CUTSCENE,                /* 0x0D */
    ZONE_TYPE_TUNE,                    /* 0x0E */
    ZONE_TYPE_LIGHT_AMBIENT_LOCAL,     /* 0x0F */
    ZONE_TYPE_LIGHT_DIRECTIONAL,       /* 0x10 */
    ZONE_TYPE_POINTGLOW,               /* 0x11 */
    ZONE_TYPE_FIRE,                    /* 0x12 */
    ZONE_TYPE_FLACK,                   /* 0x13 */
    ZONE_TYPE_BULLET,                  /* 0x14 */
    ZONE_TYPE_MISSILE,                 /* 0x15 */
    ZONE_TYPE_DISCO_BALL,              /* 0x16 */
    ZONE_TYPE_LASER_TUBE,              /* 0x17 */
    ZONE_TYPE_LIGHT_DIRECTIONAL_LOCAL, /* 0x18 */
    ZONE_TYPE_AUDIO_CONTROL,           /* 0x19 */
    ZONE_TYPE_COUNT
};

/* Zone area types (original "zone area type"): TriggerZone.kind & 3. Names are ours, from the
 * containment test zone_isOutside. */
enum ZoneAreaType {
    ZONE_AREA_CYLINDER, /* xSize radius, ySize height above the base; tilted by pitch/yaw */
    ZONE_AREA_SPHERE,   /* xSize radius */
    ZONE_AREA_BOX,      /* half extents xSize/zSize, ySize height above the base */
    ZONE_AREA_GATE      /* a rectangle the actor must cross from behind (xSize half width, ySize height) */
};

#define ZONE_TYPE(zone)      ((zone)->kind >> 2)
#define ZONE_AREA_TYPE(zone) ((zone)->kind & 3)

/* A level placement record (TriggerZones proper, gates, markers, light points): zones
 * points at the table. original: struct zone (rom_debug_struct.c) */
typedef struct TriggerZone {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 xSize;   // radius, or the x half extent
    /* 0x08 */ s16 ySize;   // height
    /* 0x0A */ s16 zSize;   // the z half extent
    /* 0x0C */ f32 pitch;   // degrees
    /* 0x10 */ f32 yaw;     // degrees
    /* 0x14 */ u8  inactive;
    /* 0x15 */ u8  kind;    // original "zone type" (bits 7-2, enum ZoneType) and "zone area type" (bits 1-0, enum ZoneAreaType)
    /* 0x16 */ u8  flags;   // bit 0: only triggers near the ground
    /* 0x17 */ u8  subTypeData;
    /* 0x18 */ u32 data1;   // per-type parameters (a colour for light points)
    /* 0x1C */ u32 data2;
    /* 0x20 */ u32 data3;
    /* 0x24 */ f32 sinPitch;
    /* 0x28 */ f32 cosPitch;
    /* 0x2C */ f32 sinYaw;
    /* 0x30 */ f32 cosYaw;
} TriggerZone; // size 0x34

/*
 * A dynamic point light (0x34 bytes). Every allocated light sits in the singly
 * linked list D_800DCD78. A LightEffect (below) drives one through LightEffect.light.
 */
typedef struct PointLight {
    /* 0x00 */ struct PointLight *next;
    /* 0x04 */ u8 unk4;              // only lights with 0 get a position / distance
    /* 0x05 */ u8 color[3];
    /* 0x08 */ u8 unk8;              // matched against the caller's light mask; bit 0 = per-player
    /* 0x09 */ u8 disabled;
    /* 0x0A */ u8 unkA;              // per-player bit: hidden from that player
    /* 0x0B */ u8 group;             // id used by func_1515F170
    /* 0x0C */ u8 unkC;              // per-player bit, cleared every frame by func_1515D69C
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 x;                // x == -0x8000: (y << 16 | z) is a pointer to an f32[3] position
    /* 0x10 */ s16 y;
    /* 0x12 */ s16 z;
    /* 0x14 */ s16 viewPos[4][3];    // position transformed by player i's view matrix
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 radius;
    /* 0x30 */ u8 viewPosValid;      // per-player bit: viewPos[i] is up to date
} PointLight;

/* A point light as the microcode reads it (0x30 bytes, loaded with G_MV_LIGHT). */
typedef struct PointLightGfx {
    /* 0x00 */ u8 col[3];
    /* 0x03 */ u8 pad3;
    /* 0x04 */ u8 colc[3];
    /* 0x07 */ u8 pad7;
    /* 0x08 */ s8 dir[3];
    /* 0x0B */ u8 padB;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x13];
    /* 0x20 */ s16 pos[4];
    /* 0x28 */ s16 pos2[4];
} PointLightGfx;

/* A node of the zone lists D_800DCD80 (ambient colour TriggerZones) and D_800DCD84 (tint TriggerZones), built by func_15009334. */
typedef struct ZoneLink {
    /* 0x0 */ struct ZoneLink *next;
    /* 0x4 */ TriggerZone *zone;
} ZoneLink;

/* A timed node in the D_800CBE00 list, allocated by func_15044964. */
typedef struct TriggerNode {
    /* 0x00 */ struct TriggerNode *next;
    /* 0x04 */ s16 lifetime; // frames left; -1 = never expires
    /* 0x06 */ s16 x;
    /* 0x08 */ s16 y;
    /* 0x0A */ s16 z;
    /* 0x0C */ u8 check;     // index into D_80085E80
    /* 0x0D */ u8 action;    // index into D_80085E8C, run when check passes
    /* 0x0E */ u8 delay;     // frames before check starts running
    /* 0x0F */ u8 padF;
} TriggerNode;

typedef struct {
    u8 r, g, b;
} Color24;

/* A group of vertices whose colors fade together towards their own target colors. */
typedef struct {
    /* 0x0 */ Color24 *colors; // target color of each vertex
    /* 0x4 */ u16 *vertexIndices;
    /* 0x8 */ s32 count;
} VtxColorGroup; // size 0xC

/* D_800CC298 (0x18 bytes): state of the per-type texture animations (func_15061B4C, game_11C2B0.c). */
typedef struct TexAnimState {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ s8  unk8;
    /* 0x09 */ s8  unk9;
    /* 0x0A */ s8  unkA;
    /* 0x0B */ u8  unkB[4];
    /* 0x0F */ u8  unkF;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 tilt;
    /* 0x14 */ s16 pulse;
    /* 0x16 */ u8  pad16[2];
} TexAnimState; // size 0x18

/* One rising bubble sprite (drawn by func_15090630); D_800D2460[6]. */
typedef struct Bubble {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 originX;
    /* 0x6 */ s16 size;
    /* 0x8 */ s16 radius;
    /* 0xA */ s16 timer;
    /* 0xC */ u8  accel;
    /* 0xD */ u8  state;
    /* 0xE */ u8  angle;
    /* 0xF */ u8  phase;
} Bubble; // size 0x10

/* A water-surface vertex (Vtx layout) and its rest position/colour (game_BC510.c). */
typedef struct WaterVtx {
    /* 0x0 */ s16 ob[3];
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];
    /* 0xC */ u8  cn[4];
} WaterVtx; // size 0x10

typedef struct WaterBase {
    /* 0x0 */ s16 ob[3];
    /* 0x6 */ u8  cn[4];
} WaterBase; // size 0xA

/* D_800D24C8: the animated water surface (game_BC510.c). */
typedef struct WaterStage {
    /* 0x00 */ Mtx mtx[2];
    /* 0x80 */ Gfx *dl;
    /* 0x84 */ WaterVtx *vtx[2];
    /* 0x8C */ WaterBase *base;
    /* 0x90 */ WaterBase *target;
    /* 0x94 */ f32 unk94;
    /* 0x98 */ f32 unk98;
    /* 0x9C */ f32 unk9C;
    /* 0xA0 */ f32 unkA0;
    /* 0xA4 */ f32 unkA4;
    /* 0xA8 */ f32 unkA8;
    /* 0xAC */ f32 unkAC;
    /* 0xB0 */ u16 tex[4];
    /* 0xB8 */ s16 timer;
    /* 0xBA */ u8  alpha;
    /* 0xBB */ u8  count;
} WaterStage; // size 0xC0

/* One loaded particle model, shared by every live particle that uses it (D_800DC468 counts them). */
typedef struct ParticleNode {
    /* 0x0 */ s32 *resource;
    /* 0x4 */ struct ParticleNode *next;
    /* 0x8 */ struct ParticleNode *prev;
    /* 0xC */ u16 model;
    /* 0xE */ u8 keepLoaded;
} ParticleNode;

/* A cached keyframe: loaded vertex positions plus ticks left before it is freed. */
typedef struct {
    s16 (*positions)[3];
    u8 timer;
    u8 pad5[3];
} VtxMorphFrame;

/* The two end points of a rope (level data such as D_80096320), higher end first. */
typedef struct RopeEnds {
    /* 0x00 */ Vec3f top;
    /* 0x0C */ Vec3f bottom;
} RopeEnds; /* size 0x18 */

/* Spawn data of a rope; func_151ACBD4 (game_1D9F10.c) copies it into the new Rope from its id field on. */
typedef struct RopeSpawn {
    /* 0x00 */ s8 id;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ RopeEnds ends;
    /* 0x1C */ f32 length;     /* |top.y - bottom.y| */
    /* 0x20 */ s8 unk20;
    /* 0x21 */ u8 pad21[3];
} RopeSpawn; /* size 0x24 */

/* The streaming music player's voice (D_800E0D80; set up by game_43620, run by
 * libultra/audio/game_21FC90). It carries its own copy of the libaudio envmixer parameter block
 * at the offsets N_PVoice uses for em_state onwards (pan at 0x0C, volume at 0x0E, cvolL/cvolR,
 * the two rate pairs, the two targets). The fields outside that block are Rare's. Several fields
 * are also addressed through their own symbols (D_800E0D84..D_800E0DFC) in game_21FC90. */
typedef struct StreamVoice {
    /* 0x00 */ s32  romAddr;        // ROM address of the stream (read at romAddr + D_800E0DE4)
    /* 0x04 */ u8   pad4[0x4];
    /* 0x08 */ void *unk8;          // 0x50-byte heap block
    /* 0x0C */ s16  em_pan;
    /* 0x0E */ s16  em_volume;
    /* 0x10 */ s16  em_cvolL;
    /* 0x12 */ s16  em_cvolR;
    /* 0x14 */ s16  unk14;
    /* 0x16 */ u16  unk16;
    /* 0x18 */ u16  em_lratl;
    /* 0x1A */ s16  em_lratm;
    /* 0x1C */ s16  em_ltgt;
    /* 0x1E */ u16  em_rratl;
    /* 0x20 */ s16  em_rratm;
    /* 0x22 */ s16  em_rtgt;
    /* 0x24 */ u16  unk24;
    /* 0x26 */ u8   pad26[0x2];
    /* 0x28 */ s32  em_delta;
    /* 0x2C */ s32  em_segEnd;
    /* 0x30 */ u16  unk30;          // start of the lowpass state passed to func_1001CF38
    /* 0x32 */ u16  unk32;
    /* 0x34 */ u8   pad34[0x4];
    /* 0x38 */ u8   unk38[0x24];    // DMEM-bound state block (its physical address goes in an acmd)
    /* 0x5C */ void *unk5C;         // 8-byte heap block
    /* 0x60 */ u8   pad60[0x14];
    /* 0x74 */ void *unk74;         // 0x440-byte heap block, cleared on restart
    /* 0x78 */ u8   pad78[0xC];
    /* 0x84 */ s32  unk84;
    /* 0x88 */ s32  unk88;          // target volume
    /* 0x8C */ u8   pad8C[0x4];
    /* 0x90 */ u32  unk90;
    /* 0x94 */ s16  unk94;          // target pan
    /* 0x96 */ s16  unk96;
    /* 0x98 */ u8   pad98[0x2];
    /* 0x9A */ u8   unk9A;
    /* 0x9B */ u8   pad9B[0x5];
} StreamVoice; // size 0xA0

/* A multiplayer spawn point (0x18 bytes) in the D_800D212C table, D_800D2128 entries long. */
typedef struct SpawnPoint {
    /* 0x00 */ s16 pos[3];
    /* 0x06 */ u8  unk6;   /* angle, 256 units per turn */
    /* 0x07 */ u8  facing; /* angle, 256 units per turn */
    /* 0x08 */ Vec3f unk8;  /* the camera eye (func_15005818) */
    /* 0x14 */ u8  unk14;
    /* 0x15 */ s8  unk15;
    /* 0x16 */ s8  link;   /* index of the spawn point to use instead; 0 = none, -1 = invalid */
    /* 0x17 */ u8  pad17;
} SpawnPoint; // size 0x18

/* Objects a bullet trace ignores (func_151C0698 / func_151C1940, game_1ED0F0.c). */
typedef struct IgnoreList {
    /* 0x00 */ s32 count;
    /* 0x04 */ Actor *objs[3];
} IgnoreList; // size 0x10

/* Near/far planes func_1510B458 recorded for a view (gViewClips[view]); near -1.0f marks none. */
typedef struct ViewClip {
    /* 0x0 */ f32 near;
    /* 0x4 */ f32 far;
} ViewClip;

/* The two half-axes that span a particle quad (func_1513D6FC, game_169510.c). */
typedef struct ParticleAxes {
    /* 0x00 */ Vec3f u;
    /* 0x0C */ Vec3f v;
} ParticleAxes; // size 0x18

/* Swarm script commands (game_CB1C0.c). The spawn command (func_1509DD50) for func_15177F90. */
typedef struct SwarmSpawnCmd {
    /* 0x00 */ u8  pad0[0x7];
    /* 0x07 */ u8  memberCount;
    /* 0x08 */ u8  pad8[0x3];
    /* 0x0B */ u8  tag;
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 z;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 playSfx;
} SwarmSpawnCmd;

/* The target command (func_1509DDFC, func_1509DEC4): op 0 follows an object, op 1 heads for a point. */
typedef struct SwarmTargetCmd {
    /* 0x00 */ u8  pad0[0x8];
    /* 0x08 */ s32 target;          /* op 0: object id (bit 13 masked off); op 1: x */
    /* 0x0C */ union {
        struct {
            s16 unk0;
            s16 yOffset;
        } follow;                   /* op 0 */
        s32 y;                      /* op 1 */
    } argC;
    /* 0x10 */ s32 z;               /* op 1 */
    /* 0x14 */ s32 yOffset;         /* op 1 */
} SwarmTargetCmd;

/* One 5-byte command of an object script (Actor.unk218, run by func_1507BC14 in game_A28B0.c): op indexes the
 * command table D_80086730, args are copied to D_800D1890..D_800D1893; an op >= 247 is a 1-byte label. */
struct ObjScriptCmd {
    /* 0x0 */ u8 op;
    /* 0x1 */ u8 args[4];
};

/*
 * Light effect payloads (game_18D770.c, "../Effects/Light/light.c"). Each is stored inline at
 * LightEffect.data; the effect's update callback (LightEffect.update) selects which one.
 */

/* Pulse (func_15163CF8): radius = base + amplitude * sin(phase). */
typedef struct LightPulse {
    /* 0x0 */ f32 base;
    /* 0x4 */ f32 amplitude;
    /* 0x8 */ f32 phase;
    /* 0xC */ f32 speed; // phase advance per frame
} LightPulse; // size 0x10

/* Jitter (func_15163DEC): radius eases towards random targets, mostly between low and mid. */
typedef struct LightJitter {
    /* 0x00 */ f32 mid;
    /* 0x04 */ f32 low;
    /* 0x08 */ f32 high;
    /* 0x0C */ f32 target;
    /* 0x10 */ f32 timer;     // frames until the next target
    /* 0x14 */ f32 interval;  // longest time between targets
    /* 0x18 */ f32 smoothing; // fraction of the distance to the target covered per frame
    /* 0x1C */ f32 level;     // current radius
} LightJitter; // size 0x20

/* Flicker (func_151607A4): bursts of flickers that ramp the light up from dark. */
typedef struct LightFlicker {
    /* 0x00 */ f32 peak;  // radius a flicker ramps up to
    /* 0x04 */ f32 level; // current radius
    /* 0x08 */ f32 rate;  // radius gained per frame
    /* 0x0C */ f32 delay; // frames before the current flicker starts ramping
    /* 0x10 */ s16 count; // flickers left in this burst
    /* 0x12 */ u8  sound; // play a sound on each flicker
} LightFlicker; // size 0x14

/* An effect tied to an actor without following a bone. */
typedef struct LightActorRef {
    /* 0x0 */ Actor *actor;
    /* 0x4 */ u8 actorId; // actor->unique_id when attached
    /* 0x5 */ u8 eventHandler; // index into D_8008B358: handler for other events
} LightActorRef; // size 0x8

/* The light follows an actor, or a point on one of its bones (func_15160B74). */
typedef struct LightAttach {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ u8 actorId;   // actor->unique_id when attached; a change means the actor slot was reused
    /* 0x05 */ u8 bone;      // index of the bone matrix in actor->ob_matrix_ptr
    /* 0x06 */ u8 pad6[0x2];
    /* 0x08 */ Vec3f offset; // position in the bone's frame
    /* 0x14 */ s8 callback;     // index into D_8008B1F8, or -1
    /* 0x15 */ u8 eventHandler; // index into D_8008B374: handler for other events
} LightAttach; // size 0x18

/* A light cast along a beam from an actor's bone, stopping at the first hit (func_15160E30). */
typedef struct LightBeam {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ u8 actorId;  // actor->unique_id when attached
    /* 0x05 */ u8 bone;     // index of the bone matrix in actor->ob_matrix_ptr
    /* 0x06 */ u8 pad6[0x2];
    /* 0x08 */ Vec3f start; // beam start in the bone's frame
    /* 0x14 */ Vec3f end;   // beam end in the bone's frame
    /* 0x20 */ f32 scale;   // hit distance to radius
    /* 0x24 */ u8 flags;        // 1: cast from the bone, 2: flag the hit actor
    /* 0x25 */ u8 eventHandler; // index into D_8008B37C: handler for other events
} LightBeam; // size 0x28

/* Colour pulse: colour = base + amplitude * sin(phase). */
typedef struct LightColourPulse {
    /* 0x00 */ Vec3f base;
    /* 0x0C */ Vec3f amplitude;
    /* 0x18 */ f32 phase;
    /* 0x1C */ f32 speed; // phase advance per frame
} LightColourPulse; // size 0x20

/* Cycle (func_15162B28): low, rising, high, falling, with a callback on entering each state. */
typedef struct LightCycle {
    /* 0x00 */ f32 high;
    /* 0x04 */ f32 low;
    /* 0x08 */ f32 range;     // high - low
    /* 0x0C */ f32 time;      // position in the cycle
    /* 0x10 */ f32 riseStart;
    /* 0x14 */ f32 riseEnd;
    /* 0x18 */ f32 fallStart;
    /* 0x1C */ f32 period;    // also the end of the fall
    /* 0x20 */ f32 invRamp;   // 1 / rise (and fall) duration
    /* 0x24 */ u8  state;     // 0 low, 1 rising, 2 high, 3 falling
    /* 0x25 */ s8  onLow;     // D_8008B364 callback run on entering each state, or -1
    /* 0x26 */ s8  onRise;
    /* 0x27 */ s8  onHigh;
    /* 0x28 */ s8  onFall;
} LightCycle; // size 0x2C; the effect reserves 0x30 bytes for it

/* Lightning (func_151630F4): idle for a random wait, then a burst of decaying flashes with thunder. */
typedef struct LightStorm {
    /* 0x00 */ f32 level;         // current radius
    /* 0x04 */ f32 decay;         // fraction of the level lost per frame
    /* 0x08 */ f32 waitTimer;     // frames until the next burst
    /* 0x0C */ f32 waitMin;
    /* 0x10 */ f32 waitRange;
    /* 0x14 */ s32 flashes;       // flashes left in the current burst
    /* 0x18 */ s32 flashesMin;
    /* 0x1C */ s32 flashesRange;
    /* 0x20 */ f32 flashTimer;    // frames until the next flash
    /* 0x24 */ f32 flashGapMin;
    /* 0x28 */ f32 flashGapRange;
    /* 0x2C */ f32 flashMin;      // radius of a flash
    /* 0x30 */ f32 flashRange;
} LightStorm; // size 0x34

/* The light tracks three f32s owned by someone else (func_15163414). */
typedef struct LightTrack {
    /* 0x0 */ f32 *x;
    /* 0x4 */ f32 *y;
    /* 0x8 */ f32 *z;
    /* 0xC */ s8 callback; // D_8008B36C update callback, or -1
    /* 0xD */ u8 handler;  // D_8008B370 event handler index
} LightTrack; // size 0x10

/* Radius follows the value read by func_15188010 (func_151638E0). */
typedef struct LightFollow {
    /* 0x0 */ s32 source;
    /* 0x4 */ f32 base;
    /* 0x8 */ f32 scale;
} LightFollow; // size 0xC

/* Flash (func_1516429C): decays each frame; event 0x33 relights it. */
typedef struct LightFlash {
    /* 0x0 */ f32 level;
    /* 0x4 */ u8 id;
} LightFlash; // size 0x8

/* A fixed point transformed by a matrix each frame (func_151643F8). */
typedef struct LightTransformed {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ struct WorldObject *placement; // its rotation and position transform the point (func_15145CD0)
} LightTransformed; // size 0x10

/* A light swinging about a pivot with an object's rotation (func_151644A8). */
typedef struct LightSwing {
    /* 0x00 */ f32 offset;          // passed as y to func_150A7960
    /* 0x04 */ f32 scale;           // applied to the object's rotX / rotZ
    /* 0x08 */ WorldObject *object;
    /* 0x0C */ Vec3f pivot;
} LightSwing; // size 0x18

/* LightEffect.data: one payload per kind of light effect. */
typedef union LightEffectData {
    s32 index;            // func_15161A68: entry of D_800C3958[0]
    void *object;         // a plain pointer payload (e.g. the lamp DynModel of game_135D00.c)
    WorldObject *mount;   // func_151606A8: the light hangs below this object
    LightPulse pulse;
    LightJitter jitter;
    LightFlicker flicker;
    LightActorRef actorRef;
    LightAttach attach;
    LightBeam beam;
    LightColourPulse colourPulse;
    LightCycle cycle;
    LightStorm storm;
    LightTrack track;
    LightFollow follow;
    LightFlash flash;
    LightTransformed transformed;
    LightSwing swing;
    struct {
        /* 0x00 */ LightPulse pulse;
        /* 0x10 */ WorldObject *object; // the effect ends once this object is inactive
    } objectPulse;    // func_15163A60
    struct {
        /* 0x00 */ LightAttach attach;
        /* 0x18 */ LightPulse pulse;
    } attachPulse;    // func_15163BE8
    struct {
        /* 0x00 */ LightActorRef ref;
        /* 0x08 */ LightJitter jitter;
        /* 0x28 */ void *user; // e.g. the AnchorEmitter of func_1519021C
    } actorJitter;    // func_151903D0
    struct {
        /* 0x00 */ LightTrack track;
        /* 0x10 */ LightActorRef ref;
    } trackActor;     // func_151BC104 (game_1E73B0.c)
    struct {
        /* 0x00 */ LightCycle cycle;
        /* 0x2C */ u8 pad2C[4];
        /* 0x30 */ u8 objectId; // WorldObject id (func_151149AC) flagged by the cycle callbacks
    } objectCycle;    // func_15009990
} LightEffectData;

/* LightEffect.flags */
#define LIGHT_EFFECT_TIMED      1 // lifetime counts down; the effect ends below 0
#define LIGHT_EFFECT_OWNS_LIGHT 2 // the light is freed with the effect (func_151602C0)

/*
 * A light effect instance (object type 53, allocated by func_1516037C with func_15167A68): a 0x18-byte
 * header, then a payload of the requested size. The first 0xE bytes are the object-list link of
 * EffectHeader. The allocation is 0x18 + payload size, so sizeof(LightEffect) is not used.
 */
typedef struct LightEffect {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 kind;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ struct EffectHeader *prev;
    /* 0x08 */ struct EffectHeader *next;
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 flags;       // LIGHT_EFFECT_*
    /* 0x0F */ s8 update;      // index into D_8008B0F0, or -1
    /* 0x10 */ s16 lifetime;   // frames left when LIGHT_EFFECT_TIMED
    /* 0x12 */ u8 lightType;   // index into the handler tables D_8008B150 / D_8008B208 / D_8008B2B0;
                               // also the id events 0x17-0x22 are addressed to
    /* 0x13 */ u8 pad13;
    /* 0x14 */ PointLight *light;
    /* 0x18 */ LightEffectData data;
} LightEffect;

/* The first fields of a LightEffect (0xE-0x13), passed to func_1516037C and copied into the effect. */
typedef struct LightEffectHeader {
    /* 0x0 */ s8  flags;     // LIGHT_EFFECT_*
    /* 0x1 */ s8  update;    // index into D_8008B0F0, or -1
    /* 0x2 */ s16 lifetime;
    /* 0x4 */ s8  lightType;
    /* 0x5 */ u8  pad5;
} LightEffectHeader; // size 0x6

/* Per-track state of a running cutscene; D_800C3958[scene] points at one per track. */
typedef struct SeqTrackState {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18; // light intensity (func_15161A68)
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24; // light colour r, g, b (func_15161A68)
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 yOffset;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
} SeqTrackState; // size 0x44

/* Payload of the light effect events sent by light_sendEvent (handled by func_151604A0). */
typedef struct LightEventMsg {
    /* 0x0 */ s8  lightType; /* events 0x17-0x22 only reach effects whose lightType matches */
    /* 0x1 */ u8  pad1[0x3];
    /* 0x4 */ void *arg; // event 0x29: an ActorRef; the effects that refer to its actor are dropped
} LightEventMsg; // size 0x8

/* IdRecord.TaskID: bits 9-0 are the task id, bits 15-10 flags. The four original flag names are from
 * the Oct 2000 debugger table; neither PURGE nor SUSPEND set means RUN_BETWEEN_SECTIONS. */
#define TASK_ID_MASK             0x03FF
#define TASK_FLAGS               0xFC00
#define RUN_INIT_ONCE_ONLY       0x0400 /* the init entry runs only when the task is created */
#define DO_NOT_CLEAR_BSS         0x0800 /* a PURGE task keeps its BSS when it is reloaded */
#define PURGE_BETWEEN_SECTIONS   0x1000 /* freed when LoadSection is not the current level */
#define SUSPEND_BETWEEN_SECTIONS 0x2000 /* not run while LoadSection is not the current level */
#define TASK_INIT_PENDING        0x4000 /* the init entry has not run yet */
#define TASK_FINISHED            0x8000 /* the done entry has run; the task no longer runs */

#define TASK_ID(task) ((task)->TaskID & ~TASK_FLAGS)

/* Entry points of a task's script (the `mode` of func_150ADAF0, which interprets it). */
enum TaskEntry {
    TASK_ENTRY_START, /* the script right after the 0x20-byte header, run when the task is created */
    TASK_ENTRY_INIT,  /* InitOffset */
    TASK_ENTRY_RUN,   /* RunOffset, every frame */
    TASK_ENTRY_PURGE, /* PurgeOffset, before the task is freed */
    TASK_ENTRY_DONE   /* DoneOffset, once the task's done flag (D_800D2E4C bit) is set */
};

/* A loaded task (script and its BSS in one allocation), kept in the doubly linked list gTaskWorkingSet
 * (the task working set) sorted by task id (game_C8950). The offsets are from the record's start.
 * original: Task WorkingSet (rom_debug_struct.c) */
typedef struct IdRecord {
    /* 0x00 */ u16 TaskID;       // task id and flags (TASK_ID_MASK, TASK_FLAGS)
    /* 0x02 */ u16 LoadSection;  // the level (level) the task was loaded in
    /* 0x04 */ u16 TaskSize;     // size of the whole allocation
    /* 0x06 */ u16 BSSSize;
    /* 0x08 */ u16 DataOffset;
    /* 0x0A */ u16 BssOffset;
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u16 InitOffset;
    /* 0x10 */ u16 RunOffset;
    /* 0x12 */ u16 PurgeOffset;
    /* 0x14 */ u16 DoneOffset;
    /* 0x16 */ u16 unk16;
    /* 0x18 */ struct IdRecord *NextTaskPtr;
    /* 0x1C */ struct IdRecord *PrevTaskPtr;
} IdRecord; // size 0x20

typedef struct IdRecordList {
    /* 0x00 */ u16 count;
    /* 0x04 */ IdRecord *head;
    /* 0x08 */ IdRecord *tail;
} IdRecordList; // size 0xC

/* The BSS of task 0x83 (func_1509B570(0x83)); only the field read by the money counters is known. */
typedef struct {
    /* 0x00 */ u8 pad0[0x64];
    /* 0x64 */ s32 unk64; /* money amount scale: game_203E20.c and game_215960.c apply it in thirds */
} Struct1509B570;

  // size 0x28

/* A debris particle of game_BC510.c's block stack (a block knocked off the top, func_1508F7BC), in the list
 * D_800D245C; func_1508F0D4 moves it and frees it when its timer runs out. */
struct BlockDebris {
    /* 0x00 */ Mtx mtx[2];                 /* double-buffered by D_800BE9C0 */
    /* 0x80 */ struct BlockDebris *next;
    /* 0x84 */ Vec3f pos;
    /* 0x90 */ Vec3f vel;
    /* 0x9C */ Vec3f rot;
    /* 0xA8 */ Vec3f rotVel;
    /* 0xB4 */ u8 timer;
    /* 0xB5 */ u8 padB5[0x3];
}; /* size 0xB8 */

/*
 * Heap block header. Every block (free or allocated) starts with one; the
 * payload handed out by func_10003C6C starts right after it. The top byte of
 * taggedSize is the block's tag (0 = free), the low 24 bits its payload size.
 * A free block also links into the free list through its first payload words.
 */
typedef struct AllocatorBlock {
    /* 0x0 */ struct AllocatorBlock *next;
    /* 0x4 */ struct AllocatorBlock *prev;
    /* 0x8 */ u32 taggedSize;
} AllocatorBlock;

typedef struct AllocatorFreeBlock {
    /* 0x00 */ AllocatorBlock header;
    /* 0x0C */ struct AllocatorFreeBlock *nextFree;
    /* 0x10 */ struct AllocatorFreeBlock *prevFree;
} AllocatorFreeBlock;

/* A group of linked level entries, D_800D23C0[0..D_80087380) (loaded by func_15017578 in game_447B0).
 * Each member is (kind << 12 | index); kind 2 is an object, indexing db_Object_array. */
typedef struct ObjGroup {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ u16 count;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 members[8];
} ObjGroup; // size 0x18

/* One of the 10 bouncing cursors in D_800BE4E0 (created by func_1500FE30 in game_3D2E0, stepped in
 * game_F5800). */
typedef struct BounceRecord {
    /* 0x0 */ u8  table;        // which D_80088810 table to draw from (0 or 1)
    /* 0x1 */ u8  pad1;
    /* 0x2 */ s8  dir;          // +1 / -1, flips when pos bounces off either end
    /* 0x3 */ u8  pad3;
    /* 0x4 */ s16 pos;          // bounces within [0, 0x500); pos >> 8 picks the table entry
    /* 0x6 */ s16 speed;        // eased towards targetSpeed each tick
    /* 0x8 */ s16 targetSpeed;  // re-randomised by func_150C851C
} BounceRecord; // size 0xA

/* One loaded effect model, D_800DDE80[slot] (game_1B0740 loads and frees them). */
typedef struct EffectModelSlot {
    /* 0x00 */ void *resource;
    /* 0x04 */ Vtx *unk4;          // two vertex buffers (swarm flap frames, wave deformation)
    /* 0x08 */ Vtx *unk8;
    /* 0x0C */ Gfx *displayList;
    /* 0x10 */ s16 *unk10;         // texture ids used by the display list (func_1510CE60's usedOut)
} EffectModelSlot; // size 0x14

/* A camera's message ring buffer, D_800DC2B0[camera index] (game_15AA10; allocated in game_32740).
 * func_1512D560 appends, func_1512D604 takes the next message. */
typedef struct CamMsg {
    /* 0x0 */ s32 type;
    /* 0x4 */ union {
        s32 index;               /* types 5, 6 */
        struct Cfg15128CB0 *cfg; /* type 1 (game_156160.c) */
        CamShot *data;           /* types 7, 8, 10 */
        f32 *value;              /* type 9 */
    } arg;                       /* a pointer or a small integer, depending on the type */
} CamMsg;

typedef struct CamMsgQueue {
    /* 0x00 */ CamMsg msgs[20];
    /* 0xA0 */ u8  padA0[8];
    /* 0xA8 */ s32 head;   // next message to read
    /* 0xAC */ s32 tail;   // next free slot
} CamMsgQueue; // size 0xB0

/* A run of consecutive model vertices loaded under one matrix (bone); D_800C5C08[id] holds
 * D_800C5EF8[id] of them, sorted by address (built by func_150028BC in game_2DF70). */
typedef struct VtxRun {
    /* 0x0 */ Vtx *start;
    /* 0x4 */ s32 count;
    /* 0x8 */ u32 mtxIndex; // the bone the vertices are skinned to
} VtxRun;

/* A model triangle as the offsets of its three vertices in the model's vertex data
 * (D_800C6070[id], built by func_150028BC; the vertex base is D_800D19A0[id]). */
typedef struct VtxTri {
    /* 0x0 */ s32 v[3];
} VtxTri;

/* The part of a WaveDeformer that the func_150A6FA0 family of vertex routines reads. */
typedef struct {
    /* 0x00 */ f32 falloff;   // 1 / (radius / 100)^2
    /* 0x04 */ f32 amplitude;
    /* 0x08 */ u16 radius;
    /* 0x0A */ s16 center[3]; // relative to the object's position
    /* 0x10 */ u16 phase;     // 12-bit; the routines advance it by speed every frame
    /* 0x12 */ s8 speed;
    /* 0x13 */ s8 phaseStep;  // phase offset per hundred model units of distance
    /* 0x14 */ u8 unk14[6];   // only used by mode 1
    /* 0x1A */ u8 pad1A[2];
} WaveParams;

/* Moves an object's vertices in a sine wave (game_1A48C0; D_800DD410 holds pointers to them). */
typedef struct WaveDeformer {
    /* 0x00 */ u8 objectId;
    /* 0x01 */ u8 flags;
    /* 0x02 */ u8 sharesObject; // an earlier deformer already targets this object
    /* 0x03 */ u8 pad3;
    /* 0x04 */ WaveParams params;
    /* 0x20 */ f32 *vertexWeight; // WAVE_WEIGHTED only
    /* 0x24 */ s8 *vertexPhase;
    /* 0x28 */ u8 *vertexAngle;   // mode 2 only; byte angle (256 per turn)
} WaveDeformer; // size 0x2C

/* A lens flare (game_1AB530); D_800DDD64 heads the list. */
typedef struct LensFlare {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ f32 screenX;
    /* 0x10 */ f32 screenY;
    /* 0x14 */ f32 dir[3];
    /* 0x20 */ f32 minFacing;
    /* 0x24 */ struct LensFlare *next;
    /* 0x28 */ s16 brightness;
    /* 0x2A */ u16 zbufSample; /* z-buffer value under (screenX, screenY), read by func_1517E1AC */
    /* 0x2C */ u16 depth;      /* the flare's own z-buffer depth, from func_1517EC1C */
    /* 0x2E */ u8 flags;
    /* 0x2F */ u8 type;
    /* 0x30 */ u8 alpha;
} LensFlare; // size 0x34

/* A trigger zone in a weighted list (built by func_1515F1B0 in game_18A8F0). D_800DCDC4 is a list of
 * spawn regions whose areas add up to D_800DCD90; game_113D60 picks one at random by area. */
typedef struct RegionNode {
    /* 0x00 */ TriggerZone *region;
    /* 0x04 */ f32 volume;
    /* 0x08 */ f32 area;          // the pick weight in D_800DCDC4
    /* 0x0C */ struct RegionNode *next;
} RegionNode;

/* A camera pose of the level's pose table D_800DC020 (D_80089550 entries, loaded by func_15005290 in
 * game_32740); game_15BD90 reads the position, game_C3E20's scripted moves also the angles. */
typedef struct CamPose {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ u8  pad6[6];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} CamPose; // size 0x18

/* Mtx viewed as its s16 integer and fraction halves. */
typedef struct MtxS16 {
    s16 intPart[4][4];
    s16 fracPart[4][4];
} MtxS16;

/* A bone-matrix trail ("afterimage") drawn behind an object (game_1B5CC0); D_800DF7C8[type] lists them. */
typedef struct BoneTrail {
    /* 0x00 */ u8 type;      /* index into the update/free/draw tables */
    /* 0x01 */ u8 numFrames; /* history length */
    /* 0x02 */ u8 numFilled; /* history frames captured so far */
    /* 0x03 */ u8 alpha;
    /* 0x04 */ u8 maxDist;   /* max spacing between frames, see func_15189118 */
    /* 0x06 */ s16 timer;
    /* 0x08 */ MtxS16 *frames;
    /* 0x0C */ struct BoneTrail *next;
    /* 0x10 */ union {
        Actor *object;
        WorldObject *worldObj;
    } owner;
    /* 0x14 */ s32 unk14;    /* type 0 only: swapped into owner.object->unk94 while drawing */
} BoneTrail; // size 0x18 (type 1 trails are allocated without unk14)

/* One 12-byte entry of the world-mesh triangle table D_800DBE3C (built by func_15000F04 in game_2DF70):
 * the triangle's three vertices. Collision code reads the same words as offsets from a vertex base,
 * which is 0 for the static world mesh. */
typedef struct MeshTri {
    /* 0x0 */ Vtx *verts[3];
} MeshTri;

/* A ground-probe candidate hit (func_150A3A70 / func_150A43E0 fill the D_800D3300 array with these). */
typedef struct GroundCandidate {
    /* 0x0 */ s32 height;  // 24.8 fixed point
    /* 0x4 */ s32 *tri;    // collision triangle (a D_800DBE3C entry): three vertex offsets from vtxBase
    /* 0x8 */ s32 vtxBase; // 0 for the static world mesh
    /* 0xC */ s32 unkC;    // group or object index (db_Object_array), depending on the probe type
} GroundCandidate;

/* One on-screen view (a player's region of the framebuffer). D_800BE628 points at an array of these,
 * one per player view (allocated in game_476D0.c). */
typedef struct RenderView {
    /* 0x000 */ u8  unk0;
    /* 0x001 */ u8  onScreen;
    /* 0x002 */ u8  pad2[2];
    /* 0x004 */ f32 width;
    /* 0x008 */ f32 height;
    /* 0x00C */ f32 halfWidth;  // also the screen centre x
    /* 0x010 */ f32 halfHeight; // also the screen centre y
    /* 0x014 */ f32 scaleX;     // sprite x scale (unscaled sprites)
    /* 0x018 */ f32 scaleY;
    /* 0x01C */ f32 unk1C;      // sprite x scale (perspective sprites)
    /* 0x020 */ f32 unk20;
    /* 0x024 */ f32 top;
    /* 0x028 */ f32 bottom;
    /* 0x02C */ f32 left;
    /* 0x030 */ f32 right;
    /* 0x034 */ f32 centerX;
    /* 0x038 */ f32 centerY;
    /* 0x03C */ u8  pad3C[4];
    /* 0x040 */ Vp  viewports[2]; // indexed by the framebuffer index D_800BE9C0
    /* 0x060 */ f32 aspect;
    /* 0x064 */ f32 unk64;
    /* 0x068 */ f32 unk68;
    /* 0x06C */ f32 fovX; // degrees
    /* 0x070 */ f32 fovY; // degrees
    /* 0x074 */ f32 frustumFovX;
    /* 0x078 */ f32 frustumFovY;
    /* 0x07C */ f32 pixelTanX; // tan(fovX / 2) / halfWidth
    /* 0x080 */ f32 pixelTanY; // tan(fovY / 2) / halfHeight
    /* 0x084 */ f32 perspScale; // guPerspectiveF scale (also scales camera shake)
    /* 0x088 */ f32 frustumPlanes[4][3]; // side-plane normals: [0..1] from frustumFovX, [2..3] from frustumFovY
    /* 0x0B8 */ u16 perspNorm;
    /* 0x0BA */ u8  padBA[2];
    /* 0x0BC */ MtxF projection;
    /* 0x0FC */ u8  padFC[4];
    /* 0x100 */ Mtx projectionMtx[2]; // indexed by the framebuffer index D_800BE9C0
} RenderView; // size 0x180

/* A vertical cylinder that confines actors (func_15055E50): x/z centre, base y, height h; r is the radius,
   bit 15 set = keep-out cylinder, clear = the actor must stay inside one of the set's cylinders. */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 z;
    /* 0x4 */ s16 y;
    /* 0x6 */ s16 h;
    /* 0x8 */ u16 r;
} BoundCylinder; /* size 0xA */

/* One entry of the per-level cylinder-set table D_800D18B0 (terminated by cylinders == NULL). */
typedef struct {
    /* 0x0 */ BoundCylinder *cylinders; /* file offset until func_150163D0 relocates it */
    /* 0x4 */ u16 count;
    /* 0x6 */ u16 id;
} BoundCylinderSet; /* size 0x8 */

/* An aim-impact slot (D_800D9960): where a shot from an owner at a given angle lands; tracers snap to it. */
typedef struct {
    /* 0x00 */ s16 unk0;           // compared with the owner's angle
    /* 0x02 */ s8 unk2;            // compared with owner->unk1D1
    /* 0x03 */ u8 ricochetChance;  // percent
    /* 0x04 */ f32 x, y, z;
    /* 0x10 */ f32 normal[3];      // surface normal at the hit
} AimImpact; /* size 0x1C */

/* A four-oscillator wobble driven by an attack/decay/sustain/release envelope, one per view (indexed by Camera.unk23D). */
typedef struct {
    /* 0x00 */ f32 frequency0;     /* oscillator frequencies in Hz (elapsed is in 30 fps frames) */
    /* 0x04 */ f32 frequency1;     // matching: an f32[4] array here changes FP regalloc in func_1512D980
    /* 0x08 */ f32 frequency2;
    /* 0x0C */ f32 frequency3;
    /* 0x10 */ f32 attackTime;
    /* 0x14 */ f32 decayTime;
    /* 0x18 */ f32 sustainTime;
    /* 0x1C */ f32 releaseTime;
    /* 0x20 */ f32 peak;
    /* 0x24 */ f32 sustainLevel;
    /* 0x28 */ f32 elapsed;       /* -1.0f when idle */
    /* 0x2C */ f32 amplitude;
    /* 0x30 */ f32 attackEnd;
    /* 0x34 */ f32 decayEnd;
    /* 0x38 */ f32 sustainEnd;
    /* 0x3C */ f32 releaseEnd;
    /* 0x40 */ f32 attackRate;
    /* 0x44 */ f32 decayRate;
    /* 0x48 */ f32 sustainRate;
    /* 0x4C */ f32 releaseRate;
    /* 0x50 */ s32 stage;         /* 0 = loaded, 1..4 = attack..release, 5 = finished */
    /* 0x54 */ f32 output[4];    /* [0..2] camera translation, [3] roll */
    /* 0x64 */ s32 mode;          /* nonzero: stage advances as the envelope runs */
} ViewEnvelope;

/* A translation plus a rotation (degrees) applied on top of a camera's view matrix. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f rot;
} CamOffset;

/* One of the three exploding pipes (D_8009F920): where it is and which way it faces. */
typedef struct PipeSpot {
    /* 0x0 */ Vec3f pos;
    /* 0xC */ u8 yaw;
} PipeSpot; /* size 0x10 */

/* A random burst point (D_800DCA30): a wall hit traced out from a pipe; all zero when the trace missed. */
typedef struct BurstPoint {
    /* 0x0 */ Vec3f pos;
    /* 0xC */ s32 unkC;
} BurstPoint; /* size 0x10 */

/* z-buffer decompression table entry (D_80089630, indexed by the 3-bit exponent): depth = (mantissa << shift) + base. */
typedef struct {
    /* 0x0 */ s32 shift;
    /* 0x4 */ u32 base;
} ZDecode;

/* A row of the prop sound table D_80089260 (row picked by bits of WorldObject.moveData). Doors play open/close
   when they start moving and opened/closed when they stop; movers play `open` on start and `opened` on stop. */
typedef struct PropSounds {
    /* 0x0 */ s16 open;       /* starts moving forward (opening) */
    /* 0x2 */ s16 close;      /* starts moving back (closing) */
    /* 0x4 */ s16 opened;     /* stops at the far end */
    /* 0x6 */ s16 closed;     /* stops back at the start */
    /* 0x8 */ s16 arrive;     /* movers: played just before reaching the target; selects func_15116BAC */
    /* 0xA */ s16 arriveLead; /* movers: how early (in units of the move rate) to play `arrive` */
} PropSounds; /* size 0xC */

/* Per-behaviour handlers of level objects (D_80088C90, indexed by the placement's behaviour index). */
typedef struct ObjectBehavior {
    /* 0x0 */ void (*moveProc)(WorldObject *obj);
    /* 0x4 */ void (*onContact)(WorldObject *obj, Actor *actor);
    /* 0x8 */ s32 contactNonPlayers; /* copied to WorldObject.contactNonPlayers */
} ObjectBehavior; /* size 0xC */

/* Saved settings of one multiplayer mode (D_80082BBC[mode]); func_151E22BC copies them into D_800E0BE0[]. */
typedef struct ModeSettings {
    /* 0x0 */ s8 unk0;
    /* 0x1 */ s8 unk1; /* -> D_800E0BE0[2], at least 1 */
    /* 0x2 */ s8 unk2; /* -> D_800E0BE0[3] */
    /* 0x3 */ u8 unk3; /* -> D_800E0BE0[16], 0..4 */
    /* 0x4 */ u8 unk4; /* -> D_800E0BE0[17] */
    /* 0x5 */ u8 unk5; /* mode-specific setting (D_800E0BE0[4] / [13] / [18] / [7]) */
    /* 0x6 */ u8 unk6; /* mode-specific setting (D_800E0BE0[14] / [19]) */
    /* 0x7 */ u8 unk7;
    /* 0x8 */ u8 unk8; /* -> D_800E0BE0[22] */
    /* 0x9 */ u8 unk9; /* -> D_800E0BE0[6] */
} ModeSettings; /* size 0xA */

/* A HUD image drawn by func_151ED430: a texture id (split into 4KB chunks of consecutive ids) and its format. */
typedef struct {
    /* 0x00 */ void *texId;
    /* 0x04 */ u16 pad4;
    /* 0x06 */ u16 width;
    /* 0x08 */ u16 height;
    /* 0x0A */ u8 fmt;
    /* 0x0B */ u8 siz;
} HudSprite; /* size 0xC */

/* Axis-aligned bounds of trigger zone i (D_800D309C[i], built by func_15017FA4) plus the set of TriggerZones it overlaps. */
typedef struct {
    /* 0x00 */ u64 overlaps[3]; // bit j: overlaps zone j
    /* 0x18 */ u8 numOverlaps;
    /* 0x1A */ s16 minX;
    /* 0x1C */ s16 maxX;
    /* 0x1E */ s16 minY;
    /* 0x20 */ s16 maxY;
    /* 0x22 */ s16 minZ;
    /* 0x24 */ s16 maxZ;
} ZoneBounds; /* size 0x28 */

/* Per-player match statistics (MenuState.stats). */
typedef struct {
    /* 0x0 */ s16 unk0; /* hits on opponents */
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 unk4; /* hits on opponents with cause 0x2E */
    /* 0x6 */ s16 unk6; /* the player's unk1AA at the end of the match */
    /* 0x8 */ s16 unk8; /* hits taken */
    /* 0xA */ s16 unkA; /* kills of teammates */
} PlayerMatchStats; /* size 0xC */

/* Multiplayer menu / match state (D_8008FDD4, which points at the static instance D_800E0AF0). */
typedef struct {
    /* 0x00 */ f32 unk0;         /* progress of a step between path stops (func_151E6C1C) */
    /* 0x04 */ f32 unk4;         /* its speed */
    /* 0x08 */ f32 unk8;         /* progress along the current stop's path */
    /* 0x0C */ f32 unkC;         /* its speed */
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ u16 input;        /* MENU_* bits pressed this frame on controller 1 */
    /* 0x22 */ u16 padInput[4];  /* MENU_* bits pressed this frame, per controller */
    /* 0x2A */ s8 unk2A;
    /* 0x2B */ s8 unk2B;         /* current path stop */
    /* 0x2C */ s8 unk2C;         /* displayed stop */
    /* 0x2D */ s8 unk2D;
    /* 0x2E */ s8 stickX[4];     /* last stick position, per controller */
    /* 0x32 */ s8 stickY[4];
    /* 0x36 */ s8 repeatX[4];    /* stick auto-repeat timers */
    /* 0x3A */ s8 repeatY[4];
    /* 0x3E */ s8 menu;          /* which of the two menu paths is in use */
    /* 0x3F */ s8 page;
    /* 0x40 */ s8 prevPage;
    /* 0x41 */ s8 cursor;
    /* 0x42 */ s8 selection;     /* e.g. row of the mode table D_800AB68C */
    /* 0x43 */ s8 unk43;
    /* 0x44 */ s8 unk44;
    /* 0x45 */ char pad45;
    /* 0x46 */ s16 score[16];    /* per-player score (checked against the limit D_800E0BE0[6], capped at 9999) */
    /* 0x66 */ PlayerMatchStats stats[4];
} MenuState; /* size 0x98 */

/* One entry of the multiplayer game-mode table D_800AB68C[] (indexed by MenuState.selection). */
typedef struct {
    /* 0x0 */ s16 nameText;
    /* 0x2 */ s16 unk2;  /* mode parameter read when the match is set up */
    /* 0x4 */ s8 mode;   /* index into D_800AB754 / D_800AB77C / D_800AB790 */
    /* 0x5 */ s8 unk5;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ s8 unk7;
    /* 0x8 */ s8 unk8;
    /* 0x9 */ s8 unk9;
} ModeEntry; /* size 0xA */

/* Camera request passed to func_1512D560 (mode 7). */
typedef struct CamParams {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ f32 eyeX;
    /* 0x18 */ f32 eyeY;
    /* 0x1C */ f32 eyeZ;
    /* 0x20 */ f32 atX;
    /* 0x24 */ f32 atY;
    /* 0x28 */ f32 atZ;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
} CamParams;

/* Per-racer state of the race mode (D_800872A0[]), 0x84 bytes. */
typedef struct Racer {
    /* 0x00 */ f32 lateral;       /* 0..1 across the track, see func_15088F30 */
    /* 0x04 */ f32 lateralTarget;
    /* 0x08 */ f32 segmentT;      /* spline parameter between nodes[1] and nodes[2] */
    /* 0x0C */ f32 dirX;
    /* 0x10 */ f32 dirZ;
    /* 0x14 */ f32 speed;
    /* 0x18 */ s32 lapStartTime;
    /* 0x1C */ s32 raceTime;
    /* 0x20 */ s32 lastLapTime;
    /* 0x24 */ s16 nodesPassed;
    /* 0x26 */ s8 lapLength;      /* nodes per lap */
    /* 0x27 */ s8 lap;
    /* 0x28 */ s8 maxLap;
    /* 0x29 */ s8 rank;
    /* 0x2A */ s8 finishOrder;    /* 0x7F while still racing */
    /* 0x2B */ u8 nodes[4];       /* spline control nodes */
    /* 0x2F */ s8 route;
    /* 0x30 */ s8 state;
    /* 0x31 */ s8 actorIndex;     /* index into movement_obj */
    /* 0x32 */ s8 prevTicks;
    /* 0x33 */ s8 ticks;
    /* 0x34 */ f32 camT;
    /* 0x38 */ f32 camX;
    /* 0x3C */ f32 camY;
    /* 0x40 */ f32 camZ;
    /* 0x44 */ u8 camNodes[4];
    /* 0x48 */ u8 started; /* 0 during the pre-race fly-through, then nonzero */
    /* 0x49 */ s8 camState;
    /* 0x4A */ u8 camNode;
    /* 0x4B */ u8 camNextNode;
    /* 0x4C */ CamParams cam;
} Racer; /* size 0x84 */

/* A node of the level's path graph (loaded by func_15085B70), 0x10 bytes. */
typedef struct PathNode {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u8 layer; /* height band, see func_15085DA8 */
    /* 0x7 */ u8 unk7;  /* id looked up by func_15086D48 */
    /* 0x8 */ u8 unk8;
    /* 0x9 */ u8 links[5]; /* neighbouring nodes, 0xFF = none */
    /* 0xE */ u8 type;
    /* 0xF */ u8 param;
} PathNode; /* size 0x10 */

/* Entries of the per-model texture tables D_800C5338[id] (D_800C5628[id] entries each). */
typedef struct ObjTexture {
    /* 0x0 */ u32 addr;   // loaded address (>= 0x10000000 once loaded), or 0x80000000 if the load failed
    /* 0x4 */ s32 id;     // texture id passed to func_1510D0EC
    /* 0x8 */ u16 width;
    /* 0xA */ u16 height;
} ObjTexture; /* size 0xC */

/* One morph group (D_800A7230): which model kind it applies to and how its vertices are split into parts. */
typedef struct {
    /* 0x0 */ u8 kind;
    /* 0x1 */ u8 frameCount;
    /* 0x2 */ u16 partVtxCounts[3];
} VtxMorphGroup; /* size 0x8 */

/* One point of a 40-point spring chain; neighbours pull each other level. */
typedef struct {
    /* 0x0 */ f32 accel;
    /* 0x4 */ f32 vel;
    /* 0x8 */ f32 pos; /* height, in 1/16 world units */
} WaveSample; /* size 0xC */

/* Static per-chain parameters (D_8008D050); D_800DDE60[chain] holds the chain's 40 WaveSamples. */
typedef struct {
    /* 0x00 */ f32 force;    /* constant added to every sample's acceleration */
    /* 0x04 */ f32 damping;  /* velocity multiplier per step */
    /* 0x08 */ f32 scale;    /* sample index = scale * 40 * chain-local distance */
    /* 0x0C */ s16 minZ;     /* chain-local z extent (negative) */
    /* 0x0E */ s16 width;    /* chain-local x extent */
    /* 0x10 */ s16 angle;    /* yaw of the chain */
    /* 0x12 */ s16 objIndex; /* index into db_Object_array for the chain's origin */
    /* 0x14 */ s16 initialPos;
    /* 0x16 */ s8  push;     /* displacement an actor applies to the sample under it */
    /* 0x17 */ u8  soundTimer;
} WavePreset; /* size 0x18 */

/* Metrics of one font glyph, in texels (the FontGlyph of game_6EA90.c). */
typedef struct {
    /* 0x0 */ u8 w;
    /* 0x1 */ u8 h;
    /* 0x2 */ u8 x; /* advance before the glyph */
    /* 0x3 */ u8 y; /* drop below the line top */
} FontGlyph;

/* A list of spawn variants: an id and a scale for each. */
typedef struct {
    /* 0x00 */ u8 count;
    /* 0x04 */ s16 *ids;
    /* 0x08 */ f32 *scales;
} VariantList;

/* One 8-byte cutscene script event (D_800C3688[scene][track][i]). Each track starts with a
   4-event header whose value is the start time (see game_50D80.c). */
typedef struct {
    /* 0x0 */ s8 type;
    /* 0x1 */ u8 delta; /* time step to the next event; 0 chains events at the same time */
    /* 0x2 */ s16 value;
    /* 0x4 */ s8 unk4;
    /* 0x5 */ s8 unk5;
    /* 0x6 */ s16 unk6;
} SeqEvent;

/* Message table entry (D_800DD254 / D_800DD258). */
typedef struct {
    /* 0x0 */ u8 *data;
    /* 0x4 */ s32 unk4;
} MessageEntry;

/* Per-view list of the objects to draw this frame (D_80089240, built in game_13F9D0.c). */
typedef struct DrawEntry {
    /* 0x0 */ u16 index;    // into db_Object_array
    /* 0x2 */ u16 distance; // from func_150A6360
} DrawEntry;

/* Node of the zone tree at D_800DBE48: square regions linked by byte offsets. */
typedef struct ZoneNode {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 unk2;     // bitmask tested by func_1503F800
    /* 0x4 */ s16 next;     // byte offset to the next sibling, 0 = last
    /* 0x6 */ u16 halfSize; // half the region's side length
    /* 0x8 */ s16 x;        // region centre
    /* 0xA */ s16 z;
    /* 0xC */ s16 child;    // byte offset to the first child, 0 = leaf
} ZoneNode;

/* One hit-reaction entry (tables D_8009A9F8, D_8009A6D8 and AnimHeader.hits): what happens to an actor hit by an attack. */
typedef struct HitReaction {
    /* 0x00 */ u8 unk0;  /* bit 0x80: this entry's reaction wins; & 0x7F: reaction anim kind */
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ s32 unk4; /* sound to play */
    /* 0x08 */ u8 unk8;  /* handler index into D_80086694 */
    /* 0x09 */ u8 unk9;  /* hit-volume angle offset (high byte) / half width */
    /* 0x0A */ u8 unkA;  /* hit-volume distance / half depth */
    /* 0x0B */ u8 padB;
    /* 0x0C */ s16 unkC; /* hit-volume height offset */
    /* 0x0E */ u8 unkE;  /* hit-volume radius / side offset */
    /* 0x0F */ u8 unkF;  /* 0x40: keep heading, 0x20: sets unk10B bit 2; masked: damage */
    /* 0x10 */ u8 unk10; /* added to the attacker's heading (high byte) */
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ f32 xzVelocity;
    /* 0x18 */ f32 yVelocity;
    /* 0x1C */ f32 gravity;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ u8 unk22; /* 0x80: sets immune to & 0x3F */
    /* 0x23 */ u8 pad23;
    /* 0x24 */ u16 unk24; /* flags */
    /* 0x26 */ u8 pad26[2];
} HitReaction; /* size 0x28 */

/* File header of an attachment model (func_1502B6BC table 9), cached per model in D_800E0148 with the load
 * reference counts in D_800DF7D0. game_5D2C0 relocates the display-list table; game_1B9DB0 passes the
 * dlists pointer itself on as one display list. */
typedef struct AttachModelHeader {
    /* 0x00 */ Gfx **dlists;    // display list table (entries are relocated on first load)
    /* 0x04 */ u32 dlistsSize;  // size of the table in bytes
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 data[1];      // vertex data, up to the display list table
} AttachModelHeader;

/* A model asset as returned by func_1502B6BC (level models: table 4, D_800B0E50; object models: table 3; dynamic
 * models in game_168A90): a header, the vertices, then the display list. A level model's pointer fields hold
 * offsets from the asset's start until func_150031EC rebases them. */
typedef struct ModelAsset {
    /* 0x00 */ Gfx *displayList; // also marks the end of the vertices
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;         // rebased
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 *materials;   // per-triangle collision materials (rebased)
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 rangeList;    // offset of a RelocRange list, rebased by func_15003120
    /* 0x24 */ s32 unk24;
    /* 0x28 */ Vtx vertices[1];
} ModelAsset;

/* One loaded level model (D_800B0E50[id]). */
typedef struct LevelModelEntry {
    /* 0x0 */ ModelAsset *model;
    /* 0x4 */ s32 loaded;
} LevelModelEntry;

/* Small s32 id tables in rodata (effect-object kinds for func_15169260, texture ids, ...) that functions copy
 * whole onto their stack: the original code used initialized local arrays, whose data the rodata provides. */
typedef struct IdList1 {
    /* 0x0 */ s32 ids[1];
} IdList1;

typedef struct IdList2 {
    /* 0x0 */ s32 ids[2];
} IdList2;

typedef struct IdList3 {
    /* 0x0 */ s32 ids[3];
} IdList3;

#endif
