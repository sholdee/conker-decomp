#ifndef CONKER_BLOOD_H
#define CONKER_BLOOD_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "blood": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_F44D0.c
 *   game_161520.c
 *   game_1D43B0.c
 *   game_1DBC60.c
 *   game_1DC980.c
 *   game_1E2180.c
 *   game_1E9A30.c
 */


typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x4];
    f32 unk1C;
    u8 pad20[0xE];
    u8 unk2E;
    u8 pad2F[0x41];
    f32 unk70;
    f32 unk74;
} Struct15134CEC;

/* Motion state of a blood splat particle, kept at BloodSplat::motion. */
typedef struct {
    s16 timer;              /* frames of growth before the splat starts to fall */
    f32 growth;
    Vec2f velocity;
    f32 gravity;
    f32 spread;             /* growth per unit of horizontal speed while falling */
} Blood356D4Motion;

/* A 2D blood splat particle spawned by func_151356D4. */
typedef struct {
    u8 pad0[0x10];
    Vec2f position;
    f32 width;
    f32 height;
    u8 pad20[0x50];
    Blood356D4Motion motion;
} BloodSplat;

/* The particle func_15136698 spawns; its desc is copied to +0x18 and `extra` to growth. */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ u8 pad110[0x18];
    /* 0x128 */ f32 growth;
} GrowingBloodParticle;

/* A bouncing chunk thrown by func_1513264C / func_15132A4C (from a ParticleSpawn desc at +0x10). */
typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    f32 radius;
    f32 bounce;             /* velocity factor kept on each bounce */
    f32 width;
    f32 height;
    u8 pad20[0xC];
    f32 unk2C;
    f32 type;               /* 0, 1 or 2 (no decal), from the blood profile */
    f32 unk34;
    Vec3f position;
    Vec3f velocity;
    Vec3f spin;
    f32 unk5C;
    s32 flags;
    u8 pad64[0x10C];
    s32 unk170;             /* counted down by message 0x45; first byte set by the spawner */
} BloodChunk;

/* A BloodTrail of a debris piece (func_151A6F00), with the size of that piece as its extra data. */
typedef struct DebrisBloodTrail {
    /* 0x00 */ BloodTrail trail;
    /* 0x40 */ f32 size;
} DebrisBloodTrail;

/* Per-particle data of the debris pieces, at 0x170 of the particle. */
typedef struct {
    /* 0x0 */ f32 stopSpeed;   /* landing slower than this (vel.y above it) stops the piece */
    /* 0x4 */ DebrisBloodTrail *trail;
    /* 0x8 */ u8 landPuffs;    /* spawn func_1514AB5C particles on landing */
    /* 0x9 */ u8 landPuffFlag;
    /* 0xA */ s16 landPuffLifeBase;
    /* 0xC */ s16 landPuffLifeRange;
} DebrisData;

/* A debris piece: a ModelParticle with DebrisData. */
typedef struct {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ DebrisData debris;
} BloodDebrisParticle;

/* An effect owned by an actor; func_151BC5A4 handles its messages, func_15134C98 forwards them while unk28 is 1. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 pad29[0x17];
    /* 0x40 */ ActorRef owner;
} OwnedEffect40;

/* One point of a ribbon trail. */
typedef struct {
    Vec3f pos;
    s16 lifetime;
    u8 alpha;
    u8 padF;
    f32 texU; /* texture coordinate along the ribbon */
} RibbonTrailPoint;

/* Trail state that follows the owner actor. */
typedef struct {
    ActorRef owner;
    Vec3f lastPos;      /* 0x08: position of the newest point */
    f32 pending;           /* 0x14: points owed, emitted once it exceeds 1 */
    f32 unk18;
    f32 texU;              /* 0x1C: texture coordinate reached by the owner */
    f32 lastTexU;          /* 0x20: texture coordinate of the newest point */
} RibbonTrailData;

/* Ribbon trail effect: a ring buffer of points from head (oldest) to tail (next write). */
typedef struct {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 buffers[0x10];
    /* 0x94 */ RibbonTrailPoint *points;
    /* 0x98 */ RibbonTrailData *data;
} RibbonTrail;

typedef struct {
    TimedEffect base;
    ActorRef owner;
} OwnedEffect28;

/* BloodStrip spawned by func_151BD2F8, with two bytes of extra data. */
typedef struct {
    BloodStrip strip;
    u8 pad7C[4];
    s16 sampleAge; /* 0x80: frames since the last sample */
} AgedBloodStrip;


/* game_F44D0.c */
void func_150C7020(Actor *owner, s16 duration, u8 arg2, s32 arg3);
void func_150C71C0(f32 *from, f32 *to, s32 arg2, s32 arg3, s32 arg4, EffectHeader *task);

/* game_161520.c */
s32 func_15134070(Actor *actor);
s32 func_1513416C(DefParticleHead *particle);
BloodDrip *func_1513418C(BloodDripDesc *desc, s32 arg1, u8 arg2, s32 arg3);
void func_151342BC(BloodDrip *drip);
s32 func_151346D0(s32 arg0, BloodDrip *drip, s32 arg2);
void func_151346EC(EffectHeader *effect);
void func_1513470C(EffectHeader *effect);
void func_1513472C(BloodDrip *drip);
void func_1513477C(BloodDrip *drip);
void func_151347CC(void *arg0, ActorMsg *msg, u8 message);
void func_151348F0(f32 arg0, f32 arg1, s32 arg2, s32 arg3);
void *func_15134908(BloodTrailDesc *desc, s32 extraSize, u8 arg2, s32 arg3); /* a BloodTrail, then extraSize bytes */
void func_151349D0(BloodTrail *trail);
void func_15134C98(OwnedEffect40 *effect, ActorMsg *msg, u8 msgId);
void func_15134CD4(f32 arg0, f32 arg1, s32 arg2, s32 arg3);
s32 func_15134CEC(Struct15134CEC *particle);
void *func_15134DAC(BloodStripDesc *desc, s32 extraSize); /* a BloodStrip, then extraSize bytes */
void func_15134E48(BloodStrip *arg0);
void func_151352EC(EffectHeader *effect);
void func_1513530C(EffectHeader *effect);
void func_1513532C(BloodStrip *arg0);
void func_151353A8(BloodStrip *arg0);
void func_15135424(void *matrix, Vec3f *inA, Vec3f *inB, Vec3f *outA, Vec3f *outB);
void func_15135480(BloodStrip *strip, ActorMsg *msg, u8 message);
void func_1513555C(BloodStrip *strip, ActorMsg *msg, u8 message);
void func_151355B8(BloodStrip *strip, ActorMsg *msg, u8 message);
s32 func_15135658(BloodStrip *strip);
f32 func_15135670(BloodStrip *strip);
void *func_151356D4(Vec2f *position, f32 size, f32 gravity, f32 spread, u8 alpha, u8 endAlpha, s16 delay, s16 lifetime,
                    s16 duration, s16 period, u8 arg10, s32 arg11);
s32 func_15135BF8(BloodSplat *splat);
void func_15135DD0(Vec3f *from, Vec3f *to, f32 radius, u8 arg3, s32 arg4);
void func_15136404(Actor *actor, f32 radius, u8 arg2, s32 arg3);
GrowingBloodParticle *func_15136698(f32 size, f32 arg1, u8 alpha, u8 arg3, s16 lifetime, CollisionTri *surface,
                                    Vec3f *arg6, u8 arg7, u8 arg8, u8 arg9, s32 arg10);
s32 func_151368A8(GrowingBloodParticle *particle);
void func_15136918(f32 size, u8 arg1, u8 arg2, s16 lifetime, s16 arg4, CollisionTri *surface, f32 *position, u8 arg7,
                   s32 arg8);
s32 func_15136A1C(DefParticleHead *particle);
void func_15136A50(f32 *x, f32 *y, f32 *z, s16 lifetime, s32 arg4, s32 arg5);
void func_15136AE4(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, u8 *arg6);
void func_15136C3C(Actor *actor, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7);
void func_15136F50(Vec3f *from, Vec3f *to, Vec3f *direction, u8 arg3, u8 arg4, s32 arg5);
void func_15137610(Actor *actor, Vec3f *from, Vec3f *to, Vec3f *direction, u8 arg4, s32 arg5);
void func_1513783C(Actor *actor, u8 arg1, s32 arg2);
s32 func_15137C64(Vec3f *start, Vec3f *end, Vec3f *dir, Vec3f *side, Vec3f *up, Vec3f *from, Vec3f *to,
                  Vec3f *direction);
s32 func_15137E10(BloodStrip *strip);
void func_15137E60(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip);
void func_15137F30(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip,
                   Vec3f *start, Vec3f *end, Vec3f *velocity, f32 *arg3, s16 *arg4, u8 *arg5, f32 *arg6);
s32 func_151380B4(Actor *actor, s32 kind, Vec3f *out);
void func_15138120(Actor *actor, s32 kind, u8 arg2);
void func_151382E0(Vec3f *position, s32 kind, void *arg2, u8 arg3, s32 arg4);
void func_15138424(Actor *actor, Vec3f *position, s32 kind, void *arg3, u8 arg4, s32 arg5);
s32 func_151389A8(BloodChunk *chunk, f32 x, f32 y, f32 z, f32 groundY, CollisionTri *surface);
void func_15138BC0(Actor *actor, u8 arg1, s32 arg2);
void func_15138C80(Actor *actor, u8 arg1, s32 arg2);
void func_15138E98(Actor *actor, Vec3f *origin, u8 extraSpray, CollisionHit *hit, u8 arg4, s32 arg5);
s32 func_15139578(BloodChunk *chunk, f32 x, f32 y, f32 z, f32 groundY, CollisionTri *surface);
void func_15139768(Actor *actor, Vec3f *origin, f32 scale, CollisionHit *hit, u8 arg4, s32 arg5);
void func_15139D74(Actor *actor, Vec3f *origin, CollisionHit *hit, u8 arg3, s32 arg4);
void func_1513A24C(Vec3f *position, const s32 *table, const f32 *arg2, s32 count, f32 scale, u8 kind, u8 arg6, u8 *actor,
                   CollisionHit *hit, u8 arg9, s32 argA);
void func_1513A48C(Vec3f *position, u8 arg1, s32 arg2);
void func_1513A594(Actor *actor, Vec3f *origin, CollisionHit *hit, u8 arg3, s32 arg4);
void func_1513A5E0(Vec3f *position, u8 arg1, s32 arg2);
void func_1513A6E0(Actor *actor, Vec3f *origin, CollisionHit *hit, u8 arg3, s32 arg4);
void func_1513ABB8(Actor *actor, Vec3f *origin, u8 variant, CollisionHit *hit, u8 arg4, s32 arg5);
void func_1513B0B8(BloodChunk *chunk, s32 arg1, u8 message);
void func_1513B0F8(Actor *actor, Vec3f *origin, CollisionHit *hit, u8 arg3, s32 arg4);
extern const f32 D_800A3C30[15];
extern const s32 D_800A3C6C[10];
extern const s32 D_800A3C94[8];
extern const s32 D_800A3CB4[8];
extern const s32 D_800A3CD4[8];
extern const s32 D_800A3CF4[8];
extern const s32 D_800A3D14[8];
extern const s32 D_800A3D34[8];
extern const s32 D_800A3D54[10];
extern const s32 D_800A3D7C[8];
extern const s32 D_800A3D9C[8];
extern const s32 D_800A3DBC[12];
extern const s32 D_800A3DEC[8];
extern const s32 D_800A3E0C[8];
extern const s32 D_800A3E2C[7];
extern const s32 D_800A3E48[10];
extern const s32 D_800A3E70[8];
extern const s32 D_800A3E90[9];
extern const s32 D_800A3EB4[8];
extern const s32 D_800A3ED4[7];
extern const s32 D_800A3EF0[9];
extern const s32 D_800A3F14[20];
extern const s32 D_800A3F64[20];
extern const BloodProfile D_800A3FD8[2];
extern const BloodProfile D_800A3FF8[2];
extern const BloodProfile D_800A4018[1];
extern const BloodProfile D_800A4028[1];
extern const BloodProfile D_800A4038[2];
extern const BloodProfile D_800A4058[1];
extern const BloodProfile D_800A4068[6];
extern const BloodProfile D_800A40C8[3];
extern const BloodProfile D_800A40F8[2];
extern const u8 D_800A4160[6];
extern const s32 D_800A4168[4];
extern const s32 D_800A4178[4];
extern const Vec3f D_800A4188[4];
extern const Vec3f D_800A41B8;
extern const u8 D_800A41C4[4];
extern const s32 D_800A41C8[4];
extern const s32 D_800A41D8[4];
extern const Vec3f D_800A41E8[4];
extern const u8 D_800A4218[3];
extern const s32 D_800A421C[3];
extern const s32 D_800A4228[3];
extern const Vec3f D_800A4234[3];
extern const s32 D_800A4260[1];
extern const f32 D_800A4264[1];
extern const s32 D_800A4268[2];
extern const f32 D_800A4270[2];
extern const u8 D_800A4278[6];
extern const s32 D_800A4280[4];
extern const s32 D_800A4290[4];
extern const Vec3f D_800A42A0[4];
extern const u8 D_800A42D0[3];
extern const s32 D_800A42D4[2][2];
extern const s32 D_800A42E4[2][2];
extern const Vec3f D_800A42F4[2][2];
extern const u8 D_800A4324[3];
extern const s32 D_800A4328[2];
extern const s32 D_800A4330[2];
extern const Vec3f D_800A4338[2];

/* game_1D43B0.c */
void func_151A6F00(DebrisEmitter *emitter, u8 mode, u8 arg2, s32 arg3);
s32 func_151A73EC(BloodDebrisParticle *particle);
s32 func_151A743C(BloodDebrisParticle *particle, f32 x, f32 y, f32 z, f32 floorY);
void func_151A7610(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, DebrisBloodTrail *trail);
void func_151A77C0(BillboardParticle *particle);
void func_151A787C(BillboardParticle *particle);
void func_151A7908(BloodDebrisParticle *particle);

/* game_1DBC60.c */
void *func_151AE7B0(Actor *actor, f32 rate, s16 lifetime, u8 arg3, s32 arg4);

/* game_1DC980.c */
void func_151AF4D0(Actor *, u8, u8, s32);

/* game_1E2180.c */
BloodDrip *func_151B4CD0(Actor *actor, u8 arg1, s32 arg2);
void func_151B4E4C(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, BloodDrip *emitter);
void func_151B4EA4(Vec3f *position, f32 dirX, f32 dirY, f32 dirZ, u8 arg4, u8 arg5);
BloodDrip *func_151B4FE0(Actor *actor, u8 arg1, s32 arg2);
void func_151B50A4(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, BloodDrip *emitter);
void func_151B50F4(Vec3f *position, f32 dirX, f32 dirY, f32 dirZ, u8 arg4);

/* game_1E9A30.c */
s32 func_151BC580(OwnedEffect40 *effect);
void func_151BC5A4(OwnedEffect40 *effect, ActorMsg *msg, u8 msgId);
s32 func_151BC64C(RibbonTrail *trail);
s32 func_151BC794(RibbonTrail *trail);
Gfx *func_151BCA90(RibbonTrail *trail, Gfx *gfx, s16 viewport);
void func_151BD21C(RibbonTrail *trail, ActorMsg *msg, u8 msgId);
s32 func_151BD2BC(OwnedEffect28 *effect);
void func_151BD2F8(Actor *actor, s32 arg1, s32 arg2);
s32 func_151BD42C(AgedBloodStrip *strip);
void func_151BD43C(Vec3f *pointA, Vec3f *pointB, Vec3f *velA, Vec3f *velB, f32 time, BloodStrip *strip);
f32 func_151BD750(AgedBloodStrip *strip);
void func_151BD79C(SpriteParticle *particle, Vec3f *prevPos);
void func_151BD7F4(Actor *actor);
void func_151BD828(Actor *actor, u8 arg1, s32 arg2);
void func_151BDD8C(BurstEffect *effect);
void func_151BE0AC(s32 effect, s32 msg, u8 msgId);
void func_151BE138(BurstEffect *effect);
void func_151BE1B8(BurstEffect *effect);
void func_151BE1E4(BurstEffect *effect);

#endif
