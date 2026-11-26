#ifndef CONKER_EFFECT_H
#define CONKER_EFFECT_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "effect": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_35CF0.c
 *   game_35D20.c
 *   game/done/game_36040.c
 *   game_DBA60.c
 *   game_DDB60.c
 *   game_DDF10.c
 *   game_E0F60.c
 *   game_E2880.c
 *   game_E2DA0.c
 *   game_E4FF0.c
 *   game_E8090.c
 *   game_E8C10.c
 *   game_EABF0.c
 *   game_EB340.c
 *   game_EB600.c
 *   game_EBE60.c
 *   game_EEE70.c
 *   game_F15D0.c
 *   game_F3BA0.c
 *   game_F4B20.c
 *   game_F5800.c
 *   game_F5D80.c
 *   game_FC410.c
 *   game_FC5F0.c
 *   game_FC9E0.c
 *   game_FD1D0.c
 *   game_FDBE0.c
 *   game_FE850.c
 *   game_FF0E0.c
 *   game_1045D0.c
 *   game_1048D0.c
 *   game_108E90.c
 *   game_109220.c
 *   game_10B380.c
 *   game/done/game_10B7C0.c
 *   game_10B7D0.c
 *   game/done/game_10C140.c
 *   game_10C170.c
 *   game_10CD70.c
 *   game_10DD20.c
 *   game_10E240.c
 *   game_1104D0.c
 *   game_111670.c
 *   game_111A00.c
 *   game_112F90.c
 *   game_117490.c
 *   game_117FC0.c
 *   game_118400.c
 *   game_11F780.c
 *   game/done/game_122650.c
 *   game_1227F0.c
 *   game_124920.c
 *   game_125A50.c
 *   game_127060.c
 *   game_128970.s
 *   game_129DE0.c
 *   game_129E80.c
 *   game_129EE0.c
 *   game_12B1C0.c
 *   game_12B7D0.c
 *   game_12D630.c
 *   game/done/game_12D7E0.c
 *   game_12D7F0.c
 *   game_12F400.c
 *   game_12FDD0.c
 *   game_130B40.c
 *   game_1312F0.c
 *   game_1355D0.c
 *   game_168A90.c
 *   game_16EE20.c
 *   game_173D40.c
 *   game_174BF0.c
 *   game/done/game_1765E0.c
 *   game_179F30.c
 *   game_182C30.c
 *   game_1844C0.c
 *   game_185560.c
 *   game_193130.c
 *   game/done/game_193420.c
 *   game_193430.c
 *   game_1944C0.c
 *   game_19E040.c
 *   game_1A1E50.c
 *   game_1A6360.c
 *   game_1B4A90.c
 *   game_1B5370.c
 *   game_1B7870.c
 *   game_1B7EC0.c
 *   game_1BD8B0.c
 *   game_1BDA00.c
 *   game_1BDB90.c
 *   game_1BFCB0.c
 *   game_1C0840.c
 *   game_1C1150.c
 *   game/done/game_1CC420.c
 *   game_1D3420.c
 *   game_1D3800.c
 *   game_1D3FC0.c
 *   game_1D4140.c
 *   game_1D6E80.c
 *   game_1D92F0.c
 *   game_1DBB60.c
 *   game_1DC150.c
 *   game_1DD500.c
 *   game_1E3050.c
 *   game_1E5820.c
 *   game_1E73B0.c
 *   game_1EB6C0.c
 *   game_1EBD00.c
 *   game_1EBFD0.c
 *   game_1FF760.c
 *   game_1FFF60.c
 */

/* Two texture-id tables of five animation frames each (D_80088810 in game_F5800.c, D_80088900 in game_1048D0.c). */
typedef struct TextureFrameTables {
    /* 0x0 */ s32 *frames[2];
} TextureFrameTables; /* size 0x8 */

/* Level effect spawners (game_EB340.c, game_EB600.c). */
/* The 8-byte payload func_150BDE90 copies to +0x28 of the object it creates. */
typedef struct {
    /* 0x00 */ Camera *owner;
    /* 0x04 */ s16 timer; // countdown to the next emitter spawn
    /* 0x06 */ s16 unk6;
} EB340Spawn;

/* The object func_150BDE90 creates; func_150BDF0C is its per-frame update. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EB340Spawn spawn;
} EB340Obj;

/* Copied to +0x70 of the emitter object func_1515548C returns. */
typedef struct {
    /* 0x00 */ Camera *owner;
    /* 0x04 */ u8   pad4[0x8];
    /* 0x0C */ u8   unkC;
    /* 0x0D */ u8   unkD;
    /* 0x0E */ u8   padE[0x2];
    /* 0x10 */ f32  unk10;
    /* 0x14 */ u8   pad14[0x44];
} EB340Payload;

typedef struct {
    /* 0x00 */ u8  pad0[0x14];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8  pad18[0x68];
    /* 0x80 */ f32 unk80;
} EB340Rising;

/* Behaviour scratch state (WorldObject.touch) of func_150BE210. */
typedef struct {
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ f32 angle; // turns at spinY per frame until past 360
} TurnerWork;

#define TURNER_WORK(obj) ((TurnerWork *)&(obj)->touch)

/* Behaviour scratch state (WorldObject.touch) of func_150BE2E8 (game_EB600.c): the end points of its path. */
typedef struct {
    /* 0x7C */ s16 from[3];
    /* 0x82 */ s16 to[3];
} PathMoverWork;

#define PATH_MOVER_WORK(obj) ((PathMoverWork *)&(obj)->touch)

/* Effect kind 4 of game_193430.c (0xA8 bytes from func_15167A68): one textured quad, double-buffered by
 * D_800BE9C0. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Vtx quads[2][4];
    /* 0x90 */ s8 dirX;       // -cos(direction) * 127, mode 1 only
    /* 0x91 */ s8 dirZ;       //  sin(direction) * 127, mode 1 only
    /* 0x92 */ u8 life;       // frames left; fades out over the last 3/4 of lifeMax
    /* 0x93 */ u8 lifeMax;
    /* 0x94 */ u8 unk94;
    /* 0x95 */ u8 mode;       // 0: flat in XZ, 1: upright facing dirX/dirZ, 2: rotated by angleA/angleB
    /* 0x96 */ s16 growthRate;
    /* 0x98 */ s16 x;
    /* 0x9A */ s16 y;
    /* 0x9C */ s16 z;
    /* 0x9E */ s16 size;      // half the quad's edge length
    /* 0xA0 */ u8 angleA;
    /* 0xA1 */ u8 angleB;
    /* 0xA2 */ u8 padA2[0x6];
} QuadEffect;

/* game/done/game_35CF0.c */
void func_15008840(void);

/* game_35D20.c */
void func_15008870(s32 mode);
void func_15008930(s32 mode);

/* game/done/game_36040.c */
void func_15008B90(void);
void func_15008BB0(void);
void func_15008BE0(void);

/* game_DBA60.c */
typedef struct Particle150AED9C Particle150AED9C;
typedef struct AttachedEffect AttachedEffect;
void func_150AE5B0(Camera *arg0);
void func_150AE790(AttachedEffect *effect);
void func_150AEB9C(Actor *actor);
void func_150AECCC(struct42 *arg0);
void func_150AED4C(BillboardParticle *p);
s32 func_150AED9C(Particle150AED9C *particle);
s32 func_150AEDD8(QuadSprite *sprite);
void func_150AEDF8(AttachedEffect *effect, ActorMsg *msg, u8 type);

/* game_DDB60.c */
void func_150B06B0(Actor *actor, Actor *other, u8 arg2, s32 arg3);
extern const s32 D_8009F810[1];
extern const f32 D_8009F814[1];
extern const s32 D_8009F818[1];
extern const f32 D_8009F81C[1];

/* game_DDF10.c */
void func_150B0A60(Actor *owner, Actor *player, f32 *forward, f32 *right, Vec3f *outPos, s16 *outAngle);
void func_150B0C34(Actor *owner);
void func_150B0C58(Actor *owner, u8 arg1, s32 arg2);
void func_150B0CE0(DDB60Effect *effect, s32 arg1, u8 arg2);
void func_150B0D20(DDB60Effect *effect);
extern const f32 D_8009F860[2];
extern const f32 D_8009F868[2];

/* game_E0F60.c */
void func_150B3AB0(Actor *obj, u8 arg1);
s32 func_150B3C0C(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 pitch, s32 yaw, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 owner);

/* game_E2880.c */
typedef struct OwnerSparkEffect OwnerSparkEffect;
OwnerSparkEffect *func_150B53D0(Actor *owner, s16 duration, u8 arg2, s32 arg3);
void func_150B54A8(OwnerSparkEffect *effect);
void func_150B57C4(OwnerSparkEffect *effect);
void func_150B57E8(OwnerSparkEffect *effect);
void func_150B5814(OwnerSparkEffect *effect);
void func_150B5840(OwnerSparkEffect *effect, ActorMsg *event, u8 eventId);

/* game_E2DA0.c */
typedef struct ObjBurstEffect ObjBurstEffect;
typedef struct BoltRingEffect BoltRingEffect;
typedef struct Arg150B66DC Arg150B66DC;
GameCommand *func_150B58F0(GameCommand *cmd, s32 objIndex);
void func_150B5950(ObjBurstEffect *effect);
void func_150B5A3C(Vec3f *pos, u8 owner, s32 kind);
void func_150B5C38(Vec3f *pos, u8 owner, s32 kind);
void func_150B5E34(Vec3f *pos, u8 owner, s32 kind);
void func_150B6000(Vec3f *pos, u8 owner, s32 kind);
void func_150B60E0(Actor *obj, Vec3f *out);
void func_150B6110(BoltRingEffect *effect);
void func_150B6450(EffectHeader *effect, s32 arg1, u8 msg);
void func_150B648C(u8 variant);
s32 func_150B66DC(Arg150B66DC *ctx);
void func_150B6754(u8 owner, s32 kind);

/* game_E4FF0.c */
void func_150B7B40(Actor *obj, u8 arg1, s32 arg2);
void func_150B82D0(Vec3f *center, f32 *height, f32 *radius, f32 *yScale, u8 arg4, s32 arg5);
void func_150B85C0(Vec3f *center, f32 *height, f32 *radius, u8 arg3, s32 arg4);
s32 func_150B879C(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle, s32 arg9, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);

/* game_E8090.c */
typedef struct Particle_150BAFEC Particle_150BAFEC;
void func_150BABE0(Actor *obj, u8 side, u8 arg2);
s32 func_150BAFEC(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw, s32 arg9, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);
s32 func_150BB260(Particle_150BAFEC *particle, s32 arg1);
void func_150BB408(Particle_150BAFEC *particle);
s32 func_150BB450(Particle_150BAFEC *particle);
s32 func_150BB498(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 arg14);

/* game_E8C10.c */
struct Obj150BCFB8;
s32 func_150BB760(Actor *actor);
s32 func_150BBB5C(Actor *actor);
s32 func_150BC488(Actor *actor);
s32 func_150BCBBC(Actor *actor);
s32 func_150BCFB8(struct Obj150BCFB8 *obj, s32 arg1, s32 arg2, s32 arg3, f32 floorY);
void func_150BD070(u8 arg0, s32 arg1);

/* game_EABF0.c */
struct Obj150BDB3C;
void func_150BD740(Actor *actor, u8 arg1, s32 arg2);
s32 func_150BD954(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, f32 *dir, u8 arg14);
s32 func_150BDB3C(struct Obj150BDB3C *obj);
extern const Vec3f D_8009FF40[4];
extern const Vec3f D_8009FF70[4];
extern const Vec3f D_8009FFA0[4];

/* game_EB340.c */
void func_150BDE90(Camera *owner, u8 arg1, s32 arg2);
void func_150BDF0C(EB340Obj *obj);

/* game_EB600.c */
void func_150BE150(EB340Obj *obj, void **msg, u8 event);
s32 func_150BE1C4(EB340Rising *arg0);
void func_150BE210(WorldObject *obj);
void func_150BE2E8(WorldObject *mover);
GameCommand *func_150BE438(GameCommand *cmd, s32 id);
void func_150BE494(Actor *obj, u8 arg1, s32 arg2);
extern const s32 D_800A0010[10];
extern const f32 D_800A0038[10];
extern const s32 D_800A0060[1];
extern const f32 D_800A0064[1];
extern const f32 D_800A0068[1];

/* game_EBE60.c */
s32 func_150BE9B0(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side);
s32 func_150BEAA8(s32 arg0, s32 arg1);
void func_150BEACC(s32 actor, s32 side, s32 strength);
s32 func_150BEC30(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side);
s32 func_150BED28(s32 arg0, s32 arg1);
void func_150BED4C(s32 actor, s32 side, s32 strength);
void func_150BEEB0(Camera *arg0);

/* game_EEE70.c */
s32 func_150C19C0(Vec3f *pos, Actor *actor, u8 side);
s32 func_150C1A2C(s32 arg0, s32 arg1);
void func_150C1A40(Actor *actor, s32 side, s32 arg2);
s32 func_150C1E34(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, f32 fromX, f32 fromY, f32 fromZ, s32 arg8, s32 arg9,
                  s32 argA, s32 argB, f32 size);

/* game_F15D0.c */
typedef struct PulseEffect PulseEffect;
void func_150C4120(Actor *owner, s16 duration, u8 arg2, s32 arg3);
void func_150C44A4(PulseEffect *effect);
void func_150C4AD8(PulseEffect *effect);
void func_150C4B08(PulseEffect *effect);
void func_150C4B34(PulseEffect *effect);
void func_150C4B60(PulseEffect *effect, s32 arg1, u8 event);
extern const Vec3f D_800A0350[3];
extern const u8 D_800A0374[4];
extern const Vec3f D_800A0378[4];
extern const u8 D_800A03A8[4];

/* game_F3BA0.c */
typedef struct Watcher Watcher;
struct CeilingProbe;
void func_150C66F0(Watcher *watcher);
s32 func_150C673C(FlickerSprite *sprite);
s32 func_150C682C(FlickerSprite *sprite);
void func_150C6870(Watcher *watcher);
WatcherEmitter *func_150C68C4(Actor *character, Watcher *watcher);
void func_150C6A08(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, WatcherEmitter *emitter);
s32 func_150C6D1C(WatcherEmitter *emitter);
void func_150C6D40(WatcherEmitter *emitter);
void func_150C6D68(WatcherEmitter *emitter);
FlickerSprite *func_150C6D90(Watcher *watcher);
extern const struct CeilingProbe D_800A0450;

/* game_F4B20.c */
typedef struct Obj150C7670 Obj150C7670;
s32 func_150C7670(Obj150C7670 *arg0);
s32 func_150C773C(void *arg0, s32 arg1);

/* game_F5800.c */
void func_150C8350(void);
void func_150C84F4(s32 arg0);
void func_150C851C(s32 scale);
Gfx *func_150C8600(Gfx *gdl);

/* game_F5D80.c */
typedef struct RadialEffect RadialEffect;
typedef struct ParticleObject ParticleObject;
typedef struct BurstParticle BurstParticle;
RadialEffect *func_150C88D0(RadialEffectDef *def, s32 extraSize, u8 arg2, s32 arg3);
void func_150C8A68(RadialEffect *effect);
Gfx *func_150C8DB8(Gfx *gdl, RadialEffect *effect, s32 arg2);
void func_150C99B4(RadialEffect *effect, Vec3f *pos, u8 heading, f32 cosine, f32 sine);
void func_150C9BDC(RadialEffect *effect, f32 oldRadius);
void func_150C9DC4(RadialEffect *effect, Vec3f *pos, u8 heading, f32 cosine, f32 sine);
s32 func_150CA07C(BurstParticle *particle);
void func_150CA150(Actor *actor);
void func_150CA930(Vec3f *pos);
s32 func_150CA9D0(QuadSprite *arg0);
s32 func_150CAA04(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 heading, s32 arg9,
                  s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14);
s32 func_150CAC28(ParticleObject *obj, s32 arg1);
s32 func_150CADD0(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 heading, s32 arg9,
                  s32 arg10, s32 arg11, s32 arg12, f32 *arg13, u8 arg14);
s32 func_150CB008(BurstParticle *particle);

/* game_FC410.c */
void func_150CEF60(s32 arg0);
f32 func_150CF040(s32 arg0, s32 arg1);
void func_150CF080(s32 *arg0);

/* game_FC5F0.c */
typedef struct ActorEmitter ActorEmitter;
void func_150CF140(ActorEmitter *emitter);
void func_150CF484(ActorEmitter *emitter, ActorMsg *msg, u8 type);

/* game_FC9E0.c */
typedef struct HopSprite HopSprite;
void func_150CF530(s32 arg0);
void func_150CF578(s32 arg0);
Gfx *func_150CF5E8(Gfx *gfx);
void func_150CF680(s16 unk12, u8 unkC, s32 unk1);
s32 func_150CF800(HopSprite *hopper);
void func_150CFBEC(HopSprite *hopper, f32 *targetX, u8 type);
void func_150CFC38(f32 targetX);
s32 func_150CFC60(HopSprite *particle);

/* game_FD1D0.c */
u8 *func_150CFD20(u8 *str);
u8 *func_150CFD5C(u8 *str);
s32 func_150CFD84(u8 *str, u8 **lineEnd);
s32 func_150CFDB8(u8 *str);
void func_150CFE3C(TextObj *obj);
void func_150CFE98(TextObj *obj);
TextObj *func_150CFF10(u8 id, u8 *str, s16 duration, s32 styleSize, s8 updateFn, u8 msgFn, u8 arg6, s32 arg7);
s32 func_150D0034(s32 arg0, TextObj *obj, s32 arg2);
void func_150D00C0(TextObj *obj, u8 *payload, u8 type);
void func_150D0134(u8 id, u8 *str, s16 duration, u8 arg3, s32 arg4);
s32 func_150D01A0(TextObj *obj);
void func_150D02B4(u8 id, u8 *str, s16 duration, u8 arg3, s32 arg4);
s32 func_150D032C(TextObj *obj);
void func_150D04C4(u8 id, u8 *str, s16 duration, u8 arg3, s32 arg4);
s32 func_150D0534(TextObj *obj);

/* game_FDBE0.c */
void func_150D0730(s32 arg0);

/* game_FE850.c */
typedef struct Obj150D149C Obj150D149C;
void func_150D13A0(s32 arg0);
void func_150D1410(s32 arg0);
void func_150D146C(s32 arg0);
void func_150D149C(Obj150D149C *arg0);

/* game_FF0E0.c */
typedef struct FF0E0Object FF0E0Object;
void func_150D1C30(FF0E0Object *obj);
void func_150D1F6C(FF0E0Object *obj, s32 arg, u8 msg);
void func_150D2054(FF0E0Object *obj);
void func_150D20B0(FF0E0Object *obj);
void func_150D20DC(FF0E0Object *obj);

/* game_1045D0.c */
void func_150D7120(Vec3f *pos, u8 fxOwner, s32 fxList);
void func_150D728C(CollisionTri *surface, Vec3f *pos, Vec3f *dir, u8 fxOwner, s32 fxList);

/* game_1048D0.c */
typedef struct TintedObject TintedObject;
void func_150D7420(TintedObject *obj, u8 *state, u8 msg);
void func_150D74DC(TintedObject *obj, u8 *state, u8 msg);
void func_150D758C(TintedObject *obj, u8 *state, u8 msg);
void func_150D7630(s32 arg0);
Gfx *func_150D765C(Gfx *gdl);
void func_150D7790(Actor *obj, s32 action);
void func_150D7928(Actor *obj);

/* game_108E90.c */
typedef struct PuffTimer PuffTimer;
void func_150DB9E0(Actor *owner, s32 type, u8 sceneTag);
void func_150DBC60(PuffTimer *timer);

/* game_109220.c */
typedef struct StreamActor StreamActor;
s32 func_150DBD70(u8 kind);
s32 func_150DC558(StreamActor *actor);

/* game_10B380.c */
typedef struct TrapEffect TrapEffect;
typedef struct EffectSpawnSource EffectSpawnSource;
void func_150DDED0(WorldObject *trap);
s32 func_150DDF88(TrapEffect *obj);
s32 func_150DDFAC(EffectSpawnSource *source, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface);
s32 func_150DE12C(EffectSpawnSource *source, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface);
s32 func_150DE2A4(QuadSprite *sprite);
void func_150DE2C4(BillboardParticle *particle);

/* game/done/game_10B7C0.c */
void func_150DE310(s32 arg0);

/* game_10B7D0.c */
typedef struct ZonePairEffect ZonePairEffect;
typedef struct TravelParticle TravelParticle;
typedef struct TexturePair TexturePair;
void func_150DE320(s32 arg0);
void func_150DE32C(s32 arg0, s32 arg1, s32 arg2);
void func_150DE458(WorldObject *prop);
void func_150DE6D8(WorldObject *prop);
void func_150DE7C0(ZonePairEffect *emitter);
s32 func_150DEACC(TravelParticle *particle);
s32 func_150DEB58(TravelParticle *particle, s16 arg1);
void func_150DEBE0(s32 arg0);
void func_150DEC28(u8 index, u8 arg1);
extern const TexturePair D_800A0D00;

/* game/done/game_10C140.c */
void func_150DEC90(s32 arg0);

/* game_10C170.c */
typedef struct PointEffect PointEffect;
void func_150DECC0(Vec3f *pos, u8 owner, s32 kind);
void func_150DF334(PointEffect *effect);
void func_150DF4B8(Vec3f *pos, s32 arg1, s32 arg2, u8 owner, s32 kind);

/* game_10CD70.c */
typedef struct SwingObject SwingObject;
typedef struct BoltEmitter BoltEmitter;
s32 func_150DF8C0(s32 index);
void func_150DF920(s32 arg0);
Gfx *func_150DFBD0(Gfx *gdl);
void func_150DFCA8(SwingObject *obj);
void func_150DFDA4(Actor *actor);
s32 func_150DFDD0(ModelAttachment *att, Actor *owner);
void func_150DFEFC(BoltEmitter *emitter);
void func_150E02C0(BoltEmitter *emitter, s32 arg1, u8 arg2);
void func_150E0300(void);
void func_150E0348(Actor *actor, u8 arg1, s32 arg2);
void func_150E03F8(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect);
void func_150E05F8(WorldObject *obj);
void func_150E06D8(Actor *actor, u8 slot, u8 arg2, s32 arg3);

/* game_10DD20.c */
void func_150E0870(Actor *obj, u8 arg1, s32 arg2);
void func_150E0BE0(Actor *obj);

/* game_10E240.c */
typedef struct PropEmitterEffect PropEmitterEffect;
void func_150E0D90(Camera *camera);
TimedEffect *func_150E1060(u8 index, u8 owner, s32 kind);
void func_150E114C(PropEmitterEffect *effect);

/* game_1104D0.c */
typedef struct Obj4174 Obj4174;
typedef struct Obj4010 Obj4010;
typedef struct HomingEffect HomingEffect;
HomingEffect *func_150E3020(s32 x, s32 y, s32 z, s32 targetX, s32 targetY, s32 targetZ, s32 arg6, f32 speed,
                            Actor *targetObj, f32 originX, f32 originY, f32 originZ, f32 *follow, s16 lifetime);
s32 func_150E3208(s32 x, s32 y, s32 z, s32 targetX, s32 targetY, s32 targetZ, s32 arg6, s32 frames);
s32 func_150E32D0(s32 x, s32 y, s32 z, Actor *targetObj, s32 arg4, f32 speed);
void func_150E3340(f32 *origin, s32 *to, f32 *follow, s16 lifetime);
s32 func_150E33CC(s32 arg0, s32 arg1, void **arg2, s32 arg3);
s32 func_150E3414(s32 originX, s32 originY, s32 originZ, s32 x, s32 y, s32 z, Actor *targetObj, s32 arg7, f32 speed);
void func_150E3514(HomingEffect *effect);
void func_150E35DC(s32 handle);
void func_150E36BC(s32 handle, s32 *x, s32 *y, s32 *z);
void func_150E3738(HomingEffect *arg0);
void func_150E4010(Obj4010 *obj);
void func_150E411C(struct Game1EF500Trail *trail);
s32 func_150E4174(Obj4174 *obj);

/* game_111670.c */
void func_150E41C0(void);
void func_150E42F8(s32 count);
void func_150E4514(s32 amount);

/* game_111A00.c */
typedef struct Struct800A1070 Struct800A1070;
typedef struct Struct150E5810 Struct150E5810;
typedef struct Struct150E4CBC Struct150E4CBC;
void func_150E4550(f32 x, f32 y, f32 z, s32 size, s32 material, s16 **tri, s32 slot);
void func_150E4928(WorldObject *obj);
void func_150E4CBC(Struct150E4CBC *fx);
void func_150E4E04(f32 x, f32 y, f32 z, s32 kind, s32 size, s16 **tri, s32 slot);
void func_150E5558(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, s32 kind, s32 size);
s32 func_150E5810(Struct150E5810 *obj, s32 arg1, s32 arg2, s32 arg3, f32 groundY, s16 (*verts)[3]);
extern const Struct800A1070 D_800A1070[];
extern const u16 D_800A1110[][2];
extern const f32 D_800A111C[];

/* game_112F90.c */
void func_150E5AE0(void);

/* game_117490.c */
typedef struct struct_EA11C_arg5 struct_EA11C_arg5;
typedef struct OscillatorObject OscillatorObject;
OscillatorObject *func_150E9FE0(Actor *owner, s16 arg1, u8 arg2, s32 arg3);
s32 func_150EA10C(OscillatorObject *obj);
void func_150EA11C(Vec3f *from, Vec3f *to, s32 arg2, s32 arg3, f32 t, struct_EA11C_arg5 *arg5);
f32 func_150EA490(OscillatorObject *obj);
void *func_150EA500(Actor *owner, s16 lifetime, u8 arg2, s32 arg3);
void func_150EA5CC(f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz);
void func_150EA8E0(s32 arg0);
void func_150EA904(s32 arg0, s32 index);
void func_150EA944(WorldObject *entry);

/* game_117FC0.c */
typedef struct struct_EAB10 struct_EAB10;
typedef struct FlashObject FlashObject;
void func_150EAB10(struct_EAB10 *actor);
s32 func_150EAE24(FlashObject *obj);

/* game_118400.c */
s32 func_150EAF50(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side);
s32 func_150EB030(s32 kind, Actor *actor);
void func_150EB090(s32 actor, s32 side, s32 strength);

/* game_11F780.c */
typedef struct SwayObject SwayObject;
s32 func_150F22D0(Vec3f *out, Actor *owner, u8 slot);
s32 func_150F237C(s32 arg0, s32 arg1);
void func_150F2390(Actor *owner, s32 slot, s32 arg2);
s32 func_150F23E0(SwayObject *obj);
s32 func_150F2480(DynModel *model);
s32 func_150F2518(DynModel *model, s32 arg1);
s32 func_150F25A0(LightEffect *effect);
void func_150F26A0(Camera *actor);
void func_150F2994(Camera *actor, s32 flag);

/* game/done/game_122650.c */
void func_150F51A0(s32 arg0, s32 arg1);
s32 func_150F51BC(void);
void func_150F51E8(s32 arg0);

/* game_1227F0.c */
void func_150F5340(void);

/* game_124920.c */
typedef struct TrailPuff TrailPuff;
typedef struct ProjectileParticle ProjectileParticle;
void func_150F7470(Vec3f *pos, Vec3f *aim, Vec3f *altOrigin, Vec3f *target, u8 targetIsDir, RayHit *knownHit, f32 speed, f32 arg7,
                   f32 arg8, Actor *owner, u8 arg10, u8 arg11, u8 arg12, u8 arg13, s32 arg14, s8 arg15, s32 sound,
                   u8 arg17, u8 arg18, s32 arg19);
s8 func_150F78B4(ProjectileParticle *obj);
s32 func_150F7E20(TrailPuff *obj);
void func_150F7F58(TrailPuff *obj, s16 arg1);
void func_150F7F8C(Vec3f *pos, u8 arg1, s32 arg2);
void func_150F802C(f32 *target, s16 arg1, u8 arg2, s32 arg3);
s32 func_150F81BC(ProjectileParticle *obj);

/* game_125A50.c */
typedef struct StageObj StageObj;
typedef struct StageMsg StageMsg;
void func_150F85A0(s32 arg0, Actor *obj, s32 arg2);
void func_150F884C(s32 arg0, s32 arg1);
void func_150F887C(StageObj *obj, StageMsg *msg, u8 type);
void func_150F892C(StageObj *obj);
void func_150F9720(u8 index);
void func_150F9788(TimedEffect *arg0);
void func_150F9794(TimedEffect *arg0);
void func_150F97C0(TimedEffect *arg0);
void func_150F97EC(Actor *obj, u8 arg1, s32 arg2);
void func_150F9950(Actor *obj, u8 arg1, s32 arg2);
extern const u8 D_800A1C00[8][8];
extern const u8 D_800A1C40[4][2];
extern const Vec3f D_800A1C48;
extern const Vec3f D_800A1C54;

/* game_127060.c */
typedef struct Obj150FADC8 Obj150FADC8;
typedef struct LineEmitter LineEmitter;
typedef struct Func150FB188State Func150FB188State;
typedef struct FadeInSprite FadeInSprite;
typedef struct FadeInEffect FadeInEffect;
void func_150F9BB0(Actor *actor, u8 owner, s32 kind);
void func_150FA1B8(BurstEffect *effect);
void func_150FA468(TimedEffect *effect, s32 arg1, u8 event);
void func_150FA520(Actor *actor, u8 point, u8 owner, s32 kind);
void func_150FAA40(u8 owner, s32 kind);
void func_150FAAEC(LineEmitter *effect);
void func_150FACE4(EffectHeader *obj, s32 arg1, u8 event);
void func_150FAD28(void);
void func_150FAD78(void);
void func_150FADC8(Obj150FADC8 *obj, s32 arg1, u8 event);
void func_150FAE18(s16 duration, u8 owner, s32 kind);
s32 func_150FB188(Func150FB188State *obj);
void func_150FB1E8(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync, u8 *proceed);
void func_150FB240(u8 *alpha, s16 timer, s16 duration, s16 fadeLength, s16 fadeRate);
s32 func_150FB29C(FadeInEffect *effect);
s32 func_150FB2E0(FadeInSprite *sprite);
void func_150FB324(Actor *actor, u8 arg1, s32 arg2);

/* game_128970.s */
Gfx *func_150FB4C0(Gfx *gfx, MtxF viewMtx);

/* game_129DE0.c */
typedef struct Effect_129DE0 Effect_129DE0;
/* An effect that holds a EffectHeader at 0x38 (see func_150FC974). */
struct Effect_129DE0 {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ u8 pad28[0x10];
    /* 0x38 */ EffectHeader *unk38;
};
s32 func_150FC930(Actor *obj, s32 arg1, s32 arg2);
void func_150FC974(Effect_129DE0 *effect);
void func_150FC9A4(Effect_129DE0 *effect);

/* game_129E80.c */
void func_150FC9D0(Effect_129DE0 *obj);

/* game_129EE0.c */
typedef struct Actor150FD514 Actor150FD514;
void func_150FCA30(void);
void func_150FCBC0(u8 index);
f32 func_150FCF1C(void *arg0);
void func_150FCFB0(s32 arg0);
void func_150FCFD4(Actor *obj, u8 arg1, s32 arg2);
s32 func_150FD514(Actor150FD514 *particle);
void func_150FDB0C(Actor150FD514 *actor, ActorMsg *msg, u8 eventId);
Vtx *func_150FDBA0(Actor150FD514 *actor, s16 viewport);
void func_150FDC2C(Actor150FD514 *actor);
void func_150FDCAC(Actor150FD514 *actor);
void func_150FDCD8(Actor150FD514 *actor);

/* game_12B1C0.c */
void func_150FDD10(s32 arg0);

/* game_12B7D0.c */
struct Game1EF500Trail *func_150FE320(Actor *obj, u8 arg1, s32 arg2);
void func_150FE49C(Actor *obj, u8 arg1, s32 arg2, Vec3f *outPosition, Vec3f *outOrigin);
void func_150FE604(Actor *obj, Vec3f *position, u8 arg2, s32 arg3);
void func_150FE794(Actor *obj, s32 arg1, Vec3f *out);
void func_150FE7D4(Actor *obj);

/* game_12D630.c */
void func_15100180(Actor *owner);
void func_151001B4(Actor *owner);
void func_15100230(OwnerEffect *effect, ActorRef *msg, u8 msgId);
void func_151002BC(OwnerEffect *effect);

/* game/done/game_12D7E0.c */
void func_15100330(s32 arg0);

/* game_12D7F0.c */
void func_15100340(s32 arg0);
Gfx *func_15100464(Gfx *gdl);

/* game_12F400.c */
typedef struct Struct15101F50 Struct15101F50;
typedef struct Owner1510281C Owner1510281C;
typedef struct Owner151025F4 Owner151025F4;
typedef struct Owner151022AC Owner151022AC;
typedef struct Obj15102884 Obj15102884;
Vtx *func_15101F50(Struct15101F50 *particle, s16 viewport);
s32 func_151022AC(Owner151022AC *obj);
s32 func_151025F4(Owner151025F4 *obj, s32 arg1);
struct EffectParticle *func_151026BC(Actor *obj, s16 arg1, u8 kind, u8 arg3, u8 arg4, s32 arg5);
void func_151027E8(Actor *actor);
s32 func_1510281C(Owner1510281C *owner, s16 key);
s32 func_15102884(Obj15102884 *obj, s32 arg1);
s32 func_151028AC(Owner1510281C *owner, s16 key);

/* game_12FDD0.c */
typedef struct BoneBeamParticle BoneBeamParticle;
QuadSprite *func_15102920(f32 size, u8 arg1, CollisionTri *surface, Vec3f *position, s16 duration, u8 arg5, u8 arg6,
                          s32 surfaceFlags, u8 arg8, s32 arg9);
BoneBeamParticle *func_15102B38(Actor *target, u8 bone, const Vec3f *start, const Vec3f *end, ParticleSize *size,
                                s16 lifetime, u8 alpha, f32 flashSize, Vec3f *flashPos, u8 viewMask, s32 extraSize, s16 node,
                                u8 arg12, s32 arg13);
void func_15102D50(BoneBeamParticle *particle, s32 arg1, u8 arg2);
s32 func_15102D90(BoneBeamParticle *particle);
Vtx *func_15102EB8(BoneBeamParticle *particle, s16 viewport);
void func_15103254(s16 lifetime, u8 alpha, f32 size, Vec3f *pos, u8 views, u8 arg5, s32 arg6);

/* game_130B40.c */
void func_15103690(s32 arg0);
void func_151036B4(Vec3f *origin, u8 arg1, s32 arg2);
struct SoundRecord;
/* A sound record callback (SoundRecordCallback, conker/sound.h) that never stops its record. */
s32 func_151037DC(struct SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);

/* game_1312F0.c */
void func_15103E40(Actor *owner, Actor *obj, Vec3f *pos, u8 variant, s32 arg4, u8 arg5, s32 arg6);

/* game_1355D0.c */
void func_15108120(Actor *obj, u8 arg1, s32 arg2);

/* game_168A90.c */
DynModel *func_1513B5E0(DynModelDesc *desc, u8 arg1, s32 userDataSize, u8 arg3, s32 arg4);
void func_1513B798(DynModel *obj);
Gfx *func_1513B83C(Gfx *gfx, DynModel *obj, s16 viewport);
s32 func_1513B968(DynModel *obj, s16 viewport);
void func_1513B9A8(DynModel *obj);
void func_1513B9DC(DynModel *obj);
void func_1513BA10(DynModel *obj);
void func_1513BA44(DynModel *obj);
void func_1513BA78(DynModel *obj, s32 arg1, u8 arg2);
s32 func_1513BAD4(DynModel *obj, s16 viewport);
DynModel *func_1513BAE8(void);
s32 func_1513BBFC(DynModel *obj, s16 viewport);
s32 func_1513BEB0(DynModel *obj);

/* game_16EE20.c */
typedef struct Msg151419D0 Msg151419D0;
typedef struct LinkedObject151419D0 LinkedObject151419D0;
typedef struct Args151420F8 Args151420F8;
void func_15141970(LinkedObject151419D0 *obj);
void func_15141990(void *arg0);
void func_151419B0(void *arg0);
void func_151419D0(LinkedObject151419D0 *obj, Msg151419D0 *msg, u8 msgType);
void func_15141A7C(Actor *actor, s32 arg1);
s32 func_15141C0C(Actor *actor);
s32 func_15141CC0(u32 type);
void func_15141DA4(void *actor, s32 selector, s32 handler);
void func_15141E38(Actor *actor, s32 handler);
void func_15141F78(u8 kind, CollisionHit *src, f32 scale, u8 arg3, Vec3f *pos, u8 arg5);
s32 func_151420F8(Actor *actor);
void func_15142180(u8 kind, Vec3f *pos, s32 arg2, f32 scaleA, f32 scaleB);
extern const Args151420F8 D_800A5200;

/* game_16F770.c */
s32 func_151422C0(s32 id, const char *name, s32 min, s32 max, const char *file, s32 line);
s32 func_151422DC(s32 id, const char *name, s32 min, s32 max, s32 value, const char *file, s32 line);
s32 func_151422F8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
/* (void *bones, s32 bone, Vec3f *out); declared without a prototype: game_1D6E80.c also passes the actor */
void func_15142314();
f32 func_151423D8(u8 angle);
Actor *func_15142444(u8 uniqueId, Actor *self);
void func_151424F4(Mtx *out, f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 sx, f32 sy, f32 sz, f32 x,
                   f32 y, f32 z);
void func_15142600(Mtx *out, f32 scaleXZ, f32 scaleY, f32 sx, f32 sy, f32 sz, f32 x, f32 y, f32 z, f32 atX, f32 atY,
                   f32 atZ);
void func_15142838(Mtx *out, f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z);
void func_15142914(MtxF mf, f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z);
void func_151429E0(u8 palette, u8 *r, u8 *g, u8 *b);
s32 func_15142A5C(Actor *actor);
f32 func_15142A80(f32 t);
f32 func_15142AC0(f32 t);
f32 func_15142B04(f32 t);
f32 func_15142B44(f32 t);
Gfx *func_15142B7C(Gfx *gfx, s32 setBits, s32 clearBits);
Gfx *func_15142C10(Gfx *gfx, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *func_15142CF0(Gfx *gfx, s32 minLevel, s32 lodFrac, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *func_15142E24(Gfx *gfx, TextureDesc *desc, s32 frame, s32 cm, s32 s, s32 t, s32 texture, u8 mode,
                   SpriteParams *sprite, u8 *needSync, s32 assetArg);
Gfx *func_15142FBC(Gfx *gfx, s32 modeH, s32 modeL, u8 *needSync);
s16 func_15143044(u8 arg0, s32 arg1);
s32 func_1514306C(TextureDesc *arg0, s32 arg1, s32 arg2, u8 mode);
void func_15143134(const Vec3f *offset, Vec3f *out, void *mtx);
void func_151432BC(TriggerZone *region, f32 *x, f32 *z, f32 *top, f32 *bottom);
void func_151436B4(f32 yaw, f32 pitch, f32 length, Vec3f *out);
void func_1514373C(f32 angle, f32 length, f32 *x, f32 *z);
void func_15143794(s16 yaw, s16 pitch, f32 length, Vec3f *out); /* (yaw, pitch, length) -> vector */
void func_15143834(s16 yaw, s16 pitch, f32 length, Vec3f *out);
void func_15143874(s16 angle, f32 length, f32 *x, f32 *z);
TriggerZone *func_151438D8(s32 start, s32 end, u16 fields, TriggerZone *key);
void func_15143D18(s32 *start, s32 *end, s32 lo, s32 hi);
s32 func_15143DA8(s32 *value, s32 lo, s32 hi);
s32 func_15143E08(Actor *actor);
s32 func_15143E24(Actor *actor);
f32 func_15143E64(const Vec3f *v);
u8 func_15143E94(s32 arg0, s32 arg1);
s32 func_1514401C(u8 id, u32 *speed, s32 *pos, u8 flags);
void func_151441A4(s16 *outR, s16 *outG, s16 *outB, s16 *outA, u8 r0, u8 g0, u8 b0, s32 a0, u8 r1, u8 g1, u8 b1, u8 a1,
                   u8 weight, u8 mode);
void func_151442FC(s16 *outR, s16 *outG, s16 *outB, s16 *outA, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, u8 a1,
                   u8 weight, u8 mode);
s32 func_151444DC(s32 value, s32 hi, s32 lo);
f32 func_15144528(f32 value, f32 hi, f32 lo);
f32 func_15144598(TriggerZone *region);
f32 func_1514462C(TriggerZone *region);
void func_1514470C(TriggerZone *region, Vec3f *out);
f32 func_15144A74(Vec3f *a, Vec3f *b); /* dot product */
/* (s32 index); declared without a prototype: game_1ED0F0.c passes a second argument */
f32 func_15144AA8();
Vec3f *func_15144B34(s32 index);
f32 func_15144B68(f32 angle);
f32 func_15144BC8(f32 angle); /* wraps an angle into [0, 360] degrees */
s32 func_15144C2C(s16 value);
f32 func_15144C8C(f32 a, f32 b);
/* (f32 *pos, f32 *screen, f32 *outA, f32 *outDepth, f32 *outInvDepth, u8 viewport); declared without a prototype:
 * func_151408A4 passes its s16 view without narrowing it */
s32 func_15144CEC();
s32 func_15144E80(CollisionTri *tri, Vec3f *binormal, Vec3f *edgeOut, Vec3f *normal); /* triangle edges + normal */
void func_151450B4(Vec3f *a, Vec3f *b, Vec3f *out);
s32 func_15145128(Vec3f *v, Vec3f *out, f32 *length, f32 *invLength); /* normalize */
s32 func_151451F0(Vec3f *origin, Vec3f *dir, Vec3f *center, f32 radius, f32 maxDist, Vec3f *hitNear, Vec3f *hitFar,
                  f32 *tNear, f32 *tFar);
s32 func_151452C4(Vec3f *origin, Vec3f *dir, Vec3f *center, f32 radius, Vec3f *hitNear, Vec3f *hitFar, f32 *tNear,
                  f32 *tFar);
s32 func_151454BC(u8 index, f32 radius, Vec3f *point);
void func_15145548(Vec3f *start, Vec3f *dir, Vec3f *point, Vec3f *out, f32 *t);
s32 func_1514563C(Vec3f *start, Vec3f *dir, Vec3f *point, Vec3f *out, f32 *t);
void func_15145740(Actor *actor, Vec3f *target, Vec3f *dir, Vec3f *offsetDir, f32 pitchOffset);
void func_15145974(Vec3f *v, f32 *heading, f32 *elevation);
f32 func_15145A0C(f32 arg0, f32 scale, f32 arg2);
void func_15145A50(Actor *actor);
s32 func_15145AD8(Vec3f *start, Vec3f *dir, Actor *actor, Vec3f *hitNear, Vec3f *hitFar, f32 *radius, f32 *arg6,
                  Vec3f *center);
u8 func_15145C90(s32 index);
void func_15145CD0(WorldObject *placement, f32 **in, f32 **out, s32 count);
void func_15145DB4(WorldObject *placement, Vec3f *in, Vec3f *out, s32 count);
void func_15145EA4(const Vec3f **offsets, Vec3f **results, void *mtx, s32 count);
s32 func_15146078(Vec3f *n, Vec3f *u, Vec3f *v); /* two axes perpendicular to a direction */
void *func_151462C8(void *gfx, ModelLink *link, u8 mode, Actor *actor, u8 uniqueId, s16 slot, Vec3f *pos, u8 flags,
                    s32 arg8);
u8 func_151464B8(ZoneNode *zone);
void func_15146508(Actor *a, Actor *b);
s32 func_1514654C(Actor *actor, ModelAttachment *point, s32 index, const Vec3f **offsets, Vec3f **results,
                  s32 count);
s32 func_1514672C(Vec3f *pos);
void func_151467A4(f32 *timer, f32 maxTime, f32 *target, f32 lo, f32 mid, f32 hi, f32 rate, f32 *value);
extern const Vec3f D_800A5480; /* zero vector */
extern const f32 D_800A548C[102];

/* game_173D40.c */
s32 func_15146890(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side);
s32 func_15146970(s32 kind, Actor *actor);
void func_15146A98(s32 actor, s32 side, s32 strength);
void func_15146BB8(s32 actor, s32 side, s32 strength);
void func_15146BD8(s32 actor, s32 side, s32 strength);
void func_15146BF8(s32 actor, s32 side, s32 strength, s32 alt);
void func_15146D60(s32 actor, s32 side, s32 strength);
void func_15146E84(s32 actor, s32 side, s32 strength);
void func_15147040(s32 actor, s32 side, s32 strength);
s32 func_151470FC(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);
void func_15147318(s32 actor, s32 side, s32 strength);
void func_15147478(s32 actor, s32 side, s32 strength);
void func_151475D8(s32 actor, s32 side, s32 strength);

/* game_174BF0.c */
void func_15147740(EffectObj *obj);
void func_151478D0(EffectObj *obj);
/* func_151478F4 and func_15147928 take any view of a func_15147A80 object. */
void func_151478F4(void *obj);
void func_15147928(void *obj);
void func_1514795C(EffectObj *obj);
void func_151479E0(EffectObj *obj);
void func_15147A30(EffectObj *obj);
/* Returns the new object; each caller keeps it in its own view (TrackerTrail, MotionTrail, ...). */
void *func_15147A80(EffectTemplate *header, s32 dataSize, s32 itemSize, s32 updateKind, s32 updateKind2,
                    s32 drawKind, s32 arg6, s32 arg7, CollisionHit *hit, u8 arg9, s32 argA);
void *func_15147C4C(void *gfx, EffectObj *obj, s16 arg2);
void func_15147D1C(EffectObj *obj, s32 arg1, u8 arg2);
void func_15147D64(s32 arg0, u8 arg1);

/* game/done/game_1765E0.c */
/* Both return a TimedEffect followed by dataSize bytes of its kind's data (the kind's own type). */
void *func_15149130(s16 timer, s8 expireCallback, s8 tickCallback, s8 drawCallback, u8 flags, u8 callbackSet,
                    s32 dataSize, u8 owner, s32 list);
void *func_151491F4(s16 timer, s8 expireCallback, s8 tickCallback, u8 flags, u8 callbackSet, s32 dataSize,
                    u8 owner, s32 list);
void func_15149264(TimedEffect *object);
void func_15149318(TimedEffect *object);
void func_1514933C(TimedEffect *object);
void func_15149368(TimedEffect *object);
void func_15149394(TimedEffect *object);
void func_151493E4(TimedEffect *object);
void func_15149434(TimedEffect *object, s32 arg1, u8 arg2);
s32 func_15149490(s32 arg0, TimedEffect *object, s16 arg2);
void func_151494E0(s32 arg0, u8 arg1);
void func_15149514(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj);

/* game_179F30.c */
typedef struct ToggleTarget ToggleTarget;
s32 func_1514CA80(Actor *obj, s32 *uniqueSlot, s32 *globalSlot, s32 *idSlot);
s32 func_1514D15C(Actor *obj);
s32 func_1514D310(Actor *owner);
s32 func_1514D3B0(Actor *owner, s16 type, s32 msg, s32 arg);
s32 func_1514D4B8(s16 root, s16 type);
void func_1514D564(Actor *obj);
void func_1514D64C(Actor *obj);
void func_1514D96C(Actor *obj);
void func_1514D978(Actor *obj);
void func_1514D9F4(Actor *obj);
void func_1514DA38(Actor *obj);
void func_1514DAA4(Actor *obj);
void func_1514DB18(Actor *obj);
void func_1514DB58(Actor *obj);
void func_1514DB64(Actor *obj);
void func_1514DB98(Actor *obj);
void func_1514DBB8(Actor *obj);
void func_1514DC38(Actor *obj);
void func_1514DC5C(Actor *obj);
void func_1514DC84(Actor *obj);
void func_1514DC98(Actor *obj);
void func_1514DCAC(Actor *obj);
void func_1514DCF4(Actor *obj);
void func_1514DD2C(Actor *obj);
void func_1514DD4C(Actor *obj);
void func_1514DD6C(Actor *obj);
void func_1514DD8C(Actor *obj);
void func_1514DDAC(Actor *obj);
void func_1514DDCC(Actor *obj);
void func_1514DDEC(Actor *obj);
void func_1514DE0C(Actor *obj);
void func_1514DE2C(Actor *obj);
void func_1514DE38(Actor *obj);
void func_1514DE44(Actor *obj);
void func_1514DE50(Actor *obj);
void func_1514DE88(Actor *obj);
void func_1514DE94(Actor *obj);
void func_1514DFD0(Actor *obj);
void func_1514E00C(Actor *obj);
void func_1514E194(Actor *obj);
void func_1514E31C(Actor *obj);
void func_1514E508(Actor *obj);
void func_1514E5B8(Actor *obj);
void func_1514E668(Actor *obj);
void func_1514E718(Actor *obj);
void func_1514E7C8(Actor *obj);
void func_1514E824(s32 handle);
void func_1514E830(EffectHeader *handle);
void func_1514E850(void *handle);
void func_1514E87C(void *handle);
s32 func_1514E89C(ToggleTarget *target, s32 msg, s32 arg);
s32 func_1514E920(s32 handle, s32 msg, s32 arg);
void func_1514E9DC(s32 handle, s32 msg, s32 arg);
void func_1514E9FC(ToggleTarget *target, s32 msg, s32 arg);
s32 func_1514EA1C(ActorEmitterProcess *target, s32 msg, s32 arg);
void func_1514EB6C(ToggleTarget *target, s32 msg, s32 arg);
s32 func_1514EB8C(s32 handle, s32 msg, s32 arg);
Attachment *func_1514EBA4(void *handle, s16 type, s32 extraSize);
/* Actor attachment lists (Actor.attachments): `handle` is the attached effect or object. */
Attachment *func_1514EC1C(void *handle, Actor *owner, s16 type);
s32 func_1514ECE0(Attachment *list, s16 type, Attachment **out);
s32 func_1514ED3C(Attachment *list, void *handle, Attachment **out);
void *func_1514ED8C(Attachment *node, Actor *owner);
void func_1514EDF0(void *handle, Actor *owner);
void func_1514EE70(Actor *obj);
void func_1514EECC(Actor *obj);
void func_1514F110(Actor *obj);
s32 func_1514F130(LightEffect *target, s32 msg, s32 arg);
void func_1514F194(Actor *obj);
void func_1514F308(Actor *obj);
void func_1514F3CC(Actor *obj);
void func_1514F44C(Actor *obj);
void func_1514F4A8(Actor *obj);
void func_1514F4D8(Actor *obj);
void func_1514F508(Actor *obj);
void func_1514F538(Actor *obj);
void func_1514F568(Actor *obj);
void func_1514F590(Actor *obj);
void func_1514F5CC(Actor *obj);
extern const Vec3f D_800A58A0;
extern const Vec3f D_800A58AC;
extern const Vec3f D_800A58B8;
extern const Vec3f D_800A58C4;
extern const Vec3f D_800A5934;
extern const Vec3f D_800A5940;
extern const Vec3f D_800A594C;
extern const Vec3f D_800A5958;
extern const Vec3f D_800A5964;

/* game_182C30.c */
/*
 * Type 0x50 object: a marker over player movement_obj[playerId], made of a white point light and a sprite
 * (*D_800E03E0) that bob 160 units above the player.
 */
typedef struct PlayerMarker {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 playerId;
    /* 0x11 */ u8 state;
    /* 0x12 */ u8 bobPhase;
    /* 0x13 */ u8 pad13;
    /* 0x14 */ PointLight *light;
    /* 0x18 */ Mtx mtx[2];
    /* 0x98 */ f32 heightOffset;
} PlayerMarker;

PlayerMarker *func_15155780(s32 playerId, s32 owner);
void func_151557FC(s32 playerId, s32 duration, f32 heightOffset);
void func_1515589C(PlayerMarker *marker);
Gfx *func_15155CFC(Gfx *gfx, PlayerMarker *marker, s16 view);
void func_15155EF8(PlayerMarker *arg0);
void func_15155F3C(void);
void func_15155F90(void);
PlayerMarker *func_15155FD4(s32 playerId);
void func_15156028(PlayerMarker *marker, Actor **actors, u8 eventType);

/* game_1844C0.c */
ModelEffect *func_15157010(EffectParams *params, s32 arg1, f32 arg2, s32 attachArg, s32 arg4, s32 extraSize, u8 owner,
                           s32 list);
void func_151571C4(ModelEffect *fx);
void func_15157248(ModelEffect *fx);
void func_1515728C(ModelEffect *fx);
void func_151572D0(ModelEffect *fx);
Gfx *func_15157420(Gfx *gfx, ModelEffect *fx, s16 view);
s32 func_15157860(ModelEffect *fx);
ModelEffect *func_15157898(EffectParams *params, EffectMotion *motion, s32 arg2, f32 arg3, s32 attachArg, s32 arg5,
                           s32 extraSize, u8 owner, s32 list);
s32 func_15157918(ModelEffect *fx);
s32 func_15157AA8(ModelEffect *fx);
void func_15157D88(ModelEffect *fx, s32 event, u8 eventType);
s32 func_15157DC8(ModelEffect *fx);
void func_15157DEC(ModelEffect *fx, f32 *xform);
Gfx *func_15157F80(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync, u8 *proceed);
Gfx *func_15157FE8(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync);
void func_15158078(s32 event, u8 eventType);

/* game_185560.c */
typedef struct StateRule StateRule;
typedef struct SpawnerTemplate SpawnerTemplate;
typedef struct SpawnerSource SpawnerSource;
typedef struct Spawner Spawner;
typedef struct RiseEmitterObject RiseEmitterObject;
typedef struct ParticleUpdateArg ParticleUpdateArg;
typedef struct EffectMotionParticle EffectMotionParticle;
typedef struct DiskEmitterObject DiskEmitterObject;
EffectActor *func_151580B0(EffectActorTemplate *template, s32 attachment, s32 arg2, u8 large, s32 extraSize, u8 arg5,
                           s32 arg6);
EffectActor *func_151581D8(EffectActorTemplate *template, u8 large, s32 extraSize, u8 arg3, s32 arg4);
void func_15158224(EffectActor *actor);
Gfx *func_151582C8(Gfx *gfx, EffectActor *actor, s16 arg2);
s32 func_1515858C(Mtx *mtx, EffectActor *actor);
s32 func_15158684(EffectActor *actor);
s32 func_15158920(Mtx *mtx, EffectActor *actor);
void func_15158A20(EffectActor *actor);
void func_15158AA4(EffectActor *actor);
void func_15158AD0(EffectActor *actor);
s32 func_15158AFC(EffectActor *actor);
void func_15158B3C(EffectActor *actor, ActorMsg *msg, u8 msgId);
CharacterWatcher *func_15158BD0(Actor *chr, u8 enableActions, s32 extraSize);
void func_15158CD4(CharacterWatcher *watcher);
void func_15158D00(CharacterWatcher *watcher);
void func_15158D2C(CharacterWatcher *watcher);
void func_15158FA4(void *self, ActorMsg *msg, u8 msgId);
s32 func_15159084(Actor *chr, u8 mode);
s32 func_15159120(Actor *chr, u8 mode);
s32 func_15159184(Actor *chr, u8 mode);
s32 func_15159230(Actor *chr, Vec3f *lastPosition, u8 prevState);
s32 func_151592B8(Actor *chr, u8 mode);
s32 func_15159370(Actor *chr, u8 *mode);
u8 func_1515942C(Actor *chr, s16 state0, s16 state1, s16 state2, s16 moveState, s16 surfaceState);
u8 func_15159594(EffectMotionParticle *particle, ParticleUpdateArg *arg1);
s32 func_151596BC(EffectMotionParticle *particle, ParticleUpdateArg *arg1);
EffectMotionParticle *func_15159890(Vec3f *pos, Vec3f *velocity, s32 arg2, s32 arg3);
void func_15159BB0(f32 x, f32 y, f32 z, s32 arg3, s32 arg4, s32 arg5, EffectHeader *obj);
void func_15159C08(DiskEmitterObject *obj);
s32 func_1515A11C(EffectMotionParticle *particle, ParticleUpdateArg *arg1);
EffectMotionParticle *func_1515A238(Vec3f *pos, Vec3f *velocity, f32 arg2, f32 damping, CollisionHit *ground, f32 size,
                                    s16 lifetime, u8 alpha, u8 arg8, u8 arg9, s32 argA);
s32 func_1515A60C(EffectMotionParticle *particle, ParticleUpdateArg *arg1);
u8 func_1515A78C(EffectMotionParticle *particle, ParticleUpdateArg *arg1);
s32 func_1515A920(Actor *chr, CollisionHit **out);
void func_1515A974(SpawnerSource *volatile src, s32 arg1);
void func_1515AA84(Spawner *spawner, Vec3f *pos, f32 ceiling, s32 arg3);
void func_1515AB88(RiseEmitterObject *obj);
extern const Vtx D_800A6080[5];
extern const Vtx D_800A60D0[4];
extern const Vtx D_800A6110[6];
extern const Vtx D_800A6170[5];
extern const Vtx D_800A61C0[4];
extern const StateRule D_800A6200[28];
extern const s8 D_800A62E0[28];
extern const s8 D_800A62FC[28];
extern const s8 D_800A6318[28];
extern const s8 D_800A6334[28];
extern const s8 D_800A6350[28];
extern const SpawnerTemplate D_800A636C[1];

/* game_193130.c */
void func_15165C80(f32 *angle, Vtx *verts, f32 radius, f32 *velocity, f32 maxAngle, f32 minAngle, u8 *vtxIndices,
                   u8 count);

/* game/done/game_193420.c */
void func_15165F70(void);

/* game_193430.c */
void func_15165F80(s32 direction, s32 x, s32 y, s32 z, s32 growthRate, s32 life, s32 arg6, u8 arg7, s32 arg8);
void func_15166118(s32 angleA, s32 angleB, s32 x, s32 y, s32 z, s32 growthRate, s32 life, s32 arg7, u8 arg8, s32 arg9);
void func_15166204(QuadEffect *effect);
Gfx *func_15166268(Gfx *gdl, QuadEffect *effect, s32 arg2);
Gfx *func_151668B8(Gfx *gdl, s32 arg1, s32 arg2, s32 arg3);

/* game_1944C0.c */
typedef struct SpriteFx SpriteFx;
typedef struct ModelFxData ModelFxData;
/*
 * Data of a type 0x10 model effect (ModelFx in game_1944C0.c); func_15168BE4 copies a template of it into
 * the object. The callback-2 update, func_15169C70, reads it as Debris in game_197120.c.
 */
struct ModelFxData {
    /* 0x00 */ f32 position[3];
    /* 0x0C */ f32 rotation[3];
    /* 0x18 */ f32 velocity[3];
    /* 0x24 */ f32 spin[3];
    /* 0x30 */ f32 scale[3];
    /* 0x3C */ s32 life;
    /* 0x40 */ Gfx *displayList;
    /* 0x44 */ u32 pad44;
    /* 0x48 */ f32 friction;
    /* 0x4C */ f32 bounce;
    /* 0x50 */ f32 spinDamp;
    /* 0x54 */ u8 callback;     /* index into D_8008CA20 */
    /* 0x55 */ u8 primA;
    /* 0x56 */ u8 tileUlt;
    /* 0x57 */ u8 phase;
    /* 0x58 */ u8 flags;
    /* 0x59 */ u8 floorOffset;
    /* 0x5A */ u8 modelId;      /* the func_1518C900 resource displayList came from */
    /* 0x5B */ u8 kind;
    /* 0x5C */ u8 renderMode;
    /* 0x5D */ u8 combineMode;
    /* 0x5E */ u8 pad5E[2];
};
typedef struct ModelFx ModelFx;
typedef struct DLCommand DLCommand;
void func_15167010(void);
void func_1516706C(void);
void func_151670C0(void);
void func_151671E8(void);
void func_15167310(void);
Gfx *func_151674F8(Gfx *gfx, s32 group, s16 arg2, s32 list);
void *func_15167A68(s32 type, s32 list, s32 size, s32 arg3, s32 owner, s32 arg5);
/* The 0x18-byte template func_15167AD8 copies into a new SpriteFx from its anim field on. */
typedef struct SpriteFxTemplate {
    /* 0x00 */ TextureDesc *anim;
    /* 0x04 */ s16 frame;
    /* 0x06 */ s16 frameStep;
    /* 0x08 */ s16 x;
    /* 0x0A */ s16 y;
    /* 0x0C */ s16 z;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s8 fadeStep;
    /* 0x13 */ u8 pad13;        /* alpha, set by func_15167AD8 */
    /* 0x14 */ s8 callback;     /* index into D_8008CA20 */
    /* 0x15 */ u8 unk15;
} SpriteFxTemplate;
void func_15167AD8(SpriteFxTemplate *src, u8 owner, s32 list);
void func_15167B44(SpriteFx *fx);
Gfx *func_15167C58(Gfx *gdl, SpriteFx *fx, u8 arg2);
BillboardParticle *func_15167D84(BillboardTemplate *src, s32 altType, s32 extra, s8 onDeath, u8 owner, s32 list);
void func_15167E0C(BillboardParticle *p);
s32 func_15168118(Gfx *gfx, BillboardParticle *p, s16 viewIndex);
void func_1516865C(QuadFxData *q, u8 r, u8 g, u8 b, u8 a);
QuadFx *func_15168800(QuadFxData *src, s32 owner, s32 list);
void func_15168870(QuadFx *fx);
void func_15168A2C(EffectHeader *obj);
void func_15168A4C(EffectHeader *obj, s32 type);
void func_15168A9C(EffectHeader *obj);
void func_15168B10(EffectHeader *obj, s32 type);
void func_15168B44(void *arg0);
void func_15168BAC(ModelFx *fx);
void func_15168BE4(ModelFxData *src, u8 owner, s32 list);
Gfx *func_15168C4C(Gfx *gfx, ModelFx *fx, s32 arg2);
void func_15168E34(s32 *addr, s32 offset);
void func_15168E54(Gfx *dl, s32 offset);
void func_15168F08(DLCommand *dl, s32 base);
void func_15168F84(s32 group, s32 *first, s32 *last);
/* void func_15169040(s32 event, u8 eventType) is declared in the files that call it:
   game_16F770.c calls it with two more arguments. */
void func_15169070(s32 first, s32 last, s32 event, u8 eventType);
void func_15169260(const s32 *types, s32 count, s32 event, u8 eventType);
void func_1516944C(s32 type, s32 event, u8 eventType);
void func_151695F0(Actor *actor, u8 eventType);
void func_1516962C(s32 type, Actor *actor, u8 eventType);
s32 func_15169668(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_1516968C(EffectHeader *obj, u8 *event, u8 eventType);
void func_151696DC(EffectHeader *obj);
/* These take any object of the object lists. Each object type has its own struct. The list code uses
 * only the bytes 0x0-0xC of an object (type, list, prev, next, owner), which EffectHeader describes
 * together with flags and state at 0xD-0xF. A struct of only those bytes cannot be a member (its size
 * rounds up to 0x10), and many types (LightEffect, RumbleObj, TextBox, StyleParticle, ...) keep their
 * own fields at 0xD-0xF. */
void func_1516972C(void *obj);
void func_1516979C(void *obj);
void func_15169804(void *obj);
void func_15169824(void *obj);
void func_15169850(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj);

/* game_19E040.c */
typedef struct ModelBurstPiece ModelBurstPiece;
void func_15170B90(WorldObject *actor, s32 modelId, s32 kind, s32 mode, u8 owner, s32 list);
void func_15170EC4(WorldObject *actor, u8 owner, s32 list);
void func_15170F4C(WorldObject *actor, u8 owner, s32 list);
void func_151711C4(Actor *obj);
void func_15171200(s32 count, s32 modelId, f32 x, f32 y, f32 z, f32 speed, f32 minScale, f32 maxScale, f32 radius,
                   s32 kind, s32 life, f32 bounce, u8 owner, s32 list);
void func_15171600(WorldObject *actor, u8 owner, s32 list);
void func_151717FC(WorldObject *actor, f32 halfX, f32 halfZ, f32 offsetX, f32 offsetY, f32 offsetZ, s32 type, s32 style,
                   s32 countA, s32 countB, f32 scale, u8 owner, s32 list);
void func_151718F0(f32 x, f32 y, f32 z, f32 radius, s32 baseSize, f32 sizeScale, s32 color, s32 count, u8 owner,
                   s32 list);
void func_15171BF4(Actor *obj, u8 owner);
extern const f32 D_800A6E90[6][3];
extern const ModelBurstPiece D_800A6ED8[6];

/* game_1A1E50.c */
void func_151749A0(s32 arg0, s32 arg1);
void func_151749F8(s32 arg0, s32 arg1);
Gfx *func_15174AA4(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15174B48(Gfx *gfx, s32 arg1, s32 arg2);

/* game_1A6360.c */
void func_15178EB0(void);
void func_15178EFC(s32 state);
void func_15179008(s32 player);
void func_151794C8(void);
void func_15179600(s32 scatterY, s32 dropFlags);
void func_151797B0(QuadFx *drop);
void func_15179AB8(void);
void func_15179B14(s32 count);
void func_15179CB0(QuadFx *splash);

/* game_1B4A90.c */
typedef struct FadeBurstEffect FadeBurstEffect;
void func_151875E0(f32 x, f32 y, f32 z, s32 speed, s32 duration, s32 durationJitter, f32 scaleYZ, f32 scaleX);
void func_15187978(FadeBurstEffect *burst);
Gfx *func_15187A98(Gfx *gdl, FadeBurstEffect *burst, s32 arg2);
void func_15187D6C(MtxF mtx, f32 rx, f32 ry, f32 rz);

/* game_1B5370.c */
s32 func_15187EC0(s32 type, f32 rate, u8 fromR, u8 fromG, u8 fromB, u8 toR, u8 toG, u8 toB);
void func_15187F90(void);
void func_15187FC0(s32 index, s32 *rgb);
void func_15188010(s32 index, f32 *blend);
void func_1518804C(s32 index, f32 blend);
void func_151880C0(void);

/* game_1B7870.c */
EffectActor *func_1518A3C0(Vec3f *position, Vec3f *angle, f32 scale, Vec3f *velocity, Vec3f *angularVelocity,
                           f32 gravity, f32 damping, u8 flags, s16 lifetime, u8 altDisplayList, s32 extraSize, u8 arg11,
                           s32 arg12);
s32 func_1518A5F4(EffectActor *actor);
s32 func_1518A914(Mtx *mtx, EffectActor *actor);

/* game_1B7EC0.c */
typedef struct QueueNode QueueNode;
typedef struct Queue Queue;
void func_1518AA10(Queue *arg);
Queue *func_1518AADC(s32 capacity, s16 lifetime, u8 flags);
QueueNode *func_1518AB60(void *value, u8 releaseFunc);
s32 func_1518ABD0(Queue *queue, void *value, u8 releaseFunc);

/* game_1BD8B0.c */
typedef struct BoundEffect BoundEffect;
void func_15190400(BoundEffect *effect, ActorRef *msg, u8 type);
void func_15190454(LightEffect *obj);
void func_15190464(LightEffect *obj);

/* game_1BDA00.c */
typedef struct PeriodicObject PeriodicObject;
void func_15190550(AnchorEmitter *obj, union ActorMsg *msg, u8 event);
s32 func_151905BC(PeriodicObject *obj);
void func_151906C4(PeriodicObject *obj);

/* game_1BDB90.c */
struct TimedSoundObject;
typedef struct ObjectPair ObjectPair;
void func_151906E0(struct TimedSoundObject *self, ObjectPair *pair, u8 msg);
void func_1519072C(Actor *self);

/* game_1BFCB0.c */
void func_15192800(Actor *obj, s32 arg1);
s32 func_151928B0(Actor *obj, s32 *group);

/* game_1C0840.c */
typedef struct FallingObject FallingObject;
void func_15193390(ObjectEmitter *volatile emitter);
void func_151934B4(FallingObject *obj);

/* game_1C1150.c */
void func_15193CA0(Actor *obj, u8 arg1, u8 arg2, s32 arg3);
void func_151942B0(Actor *self, Actor *other, u8 *arg2);
/* (Actor *self, Actor *other, u8 *arg2); declared without a prototype: game_981E0.c passes two arguments */
void func_151942E8();
void func_15194320(Actor *self, Actor *other, u8 *arg2);
void func_15194394(Actor *self, Actor *other, u8 *arg2);
void func_15194408(Actor *self, Actor *other);
void func_15194588(Actor *self, Actor *other, u8 *arg2);
void func_151945CC(Actor *self, Actor *other, u8 *arg2);
void func_15194794(Actor *self, Actor *other, u8 *arg2);
void func_15194810(Actor *self, Actor *other, u8 *arg2);
void func_151949E0(Actor *self, Actor *other, u8 *arg2);
void func_151949F4(s32 arg0, Actor *self, Actor *other);
void func_15194A68(Actor *self, Actor *other, u8 *arg2);
void func_15194AB4(Actor *obj);
void func_15194B1C(Actor *obj);
void func_15194B94(Actor *self, Actor *other, u8 *arg2);
void func_15194BF0(Actor *self, Actor *other, u8 *arg2);
void func_15194D80(Actor *self, Actor *other, u8 *arg2);
void func_15194DA4(Actor *self, Actor *other, u8 *arg2);
void func_15194DC8(Actor *self, Actor *other, u8 *arg2);
void func_15194E54(Actor *self, Actor *other, u8 *arg2);
void func_15194EA8(Actor *self, Actor *other, u8 *arg2);
void func_15194F48(Actor *self, Actor *other, u8 *arg2);
void func_15194FF4(Actor *self, Actor *other, s32 arg2);
void func_151951E0(Actor *obj);
void func_15195528(Actor *self, Actor *other, u8 *arg2);
void func_15195650(Actor *self, Actor *other, u8 *arg2);
void func_15195698(Actor *self, Actor *other, u8 *arg2);
void func_15195738(Actor *self, Actor *other, u8 *arg2);

/* game/done/game_1CC420.c */
void func_1519EF70(Actor *arg0, s32 arg1, s32 arg2);

/* game_1D3420.c */
typedef struct Game1D3420Listener Game1D3420Listener;
typedef struct Game1D3420Emitter Game1D3420Emitter;
typedef struct Game1D2B10QuadVertex Game1D2B10QuadVertex;
typedef struct Game1D2B10QuadOwner Game1D2B10QuadOwner;
void func_151A5F70(Game1D3420Listener *listener, void *data, u8 type);
void func_151A6068(Game1D3420Emitter *emitter, s32 arg1, u8 playSound, u8 arg3, u8 arg4, s32 arg5);
Game1D2B10QuadVertex *func_151A6350(Game1D2B10QuadOwner *owner, s16 index);

/* game_1D3FC0.c */
struct ZoneEmitter; /* include/conker/particle.h */
void func_151A6B10(struct ZoneEmitter *obj);
void func_151A6B3C(struct ZoneEmitter *obj);
void func_151A6B68(Actor *target, s32 owner);
ListObj *func_151A6BD8(s32 owner);

/* game_1D4140.c */
void func_151A6C90(Actor *obj, u8 arg1, s32 arg2);

/* game_1D6E80.c */
typedef struct Task151AA30C Task151AA30C;
typedef struct Task151AA17C Task151AA17C;
typedef struct PulseFx PulseFx;
typedef struct Obj151AB3A4 Obj151AB3A4;
typedef struct Obj151AADBC Obj151AADBC;
typedef struct Obj151AA6D8 Obj151AA6D8;
typedef struct LoopSoundFx LoopSoundFx;
typedef struct EmitterTask EmitterTask;
typedef struct ActorTask ActorTask;
void func_151A99D0(ActorTask *task);
void func_151A9AA4(ActorTask *task);
void func_151A9BA0(ActorTask *task);
void func_151A9CA0(ActorTask *task);
void func_151A9DC0(ActorTask *task);
void func_151A9EC0(ActorTask *task);
void func_151A9FC8(ActorTask *task);
void func_151AA09C(ActorTask *task);
void func_151AA170(s32 arg0);
void func_151AA17C(Task151AA17C *task);
void func_151AA1D0(void);
void func_151AA1F0(void);
void func_151AA210(Task151AA17C *task);
void func_151AA264(Actor *actor, CollisionHit *hit);
void func_151AA30C(f32 x, f32 y, f32 z, f32 arg3, f32 scale, f32 arg5, Task151AA30C *task);
PulseFx *func_151AA48C(Actor *actor, ActorTask *task);
s32 func_151AA5A4(PulseFx *fx);
void func_151AA6D8(Vec3f *from, Vec3f *to, s32 arg2, s32 arg3, f32 t, Obj151AA6D8 *obj);
f32 func_151AAA4C(PulseFx *fx);
void func_151AAABC(ActorTask *task);
void func_151AAB50(PulseFx *fx);
void func_151AAB78(PulseFx *fx);
s32 func_151AABA0(PulseFx *fx);
void func_151AABC4(Actor *actor, u8 index);
s32 func_151AADBC(Obj151AADBC *obj);
void func_151AADF8(ActorTask *task);
s32 func_151AB090(FlickerSprite *fx);
s32 func_151AB180(FlickerSprite *fx);
void func_151AB1C4(ActorTask *task);
LoopSoundFx *func_151AB2C4(Actor *actor, ActorTask *task);
void func_151AB3A4(f32 x, f32 y, f32 z, f32 dx, f32 arg4, f32 dz, Obj151AB3A4 *obj);
s32 func_151AB6B8(LoopSoundFx *fx);
void func_151AB788(LoopSoundFx *fx);
void func_151AB7D8(LoopSoundFx *fx);
void func_151AB828(ActorTask *task);
void func_151AB854(ActorTask *task);
void func_151AB920(Actor *actor, s32 arg1);
void func_151AB930(Actor *actor);
void func_151AB9C8(EmitterTask *task);
void func_151ABD54(EmitterTask *task, ActorMsg *msg, u8 msgId);
void func_151ABE00(Actor *actor);

/* game_1D92F0.c */
typedef struct FxSpawnPayload FxSpawnPayload;
typedef struct FxEmitter FxEmitter;
void func_151ABE40(Vec3f *pos, CollisionHit *hit, s32 type, u8 arg3, s32 arg4);
s32 func_151AC078(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw, s32 arg9, s32 argA,
                  s32 type, s32 argC, FxSpawnPayload *payload, u8 argE);
s32 func_151AC3CC(FxEmitter *emitter);
s32 func_151AC408(FxEmitter *emitter, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface);

/* game_1DBB60.c */
void func_151AE6B0(CharacterWatcher *task);

/* game_1DC150.c */
void func_151AECA0(Actor *obj, u8 arg1, s32 arg2);

/* game_1DD500.c */
typedef struct Game1DD500Task Game1DD500Task;
typedef struct Game1DD500Particle Game1DD500Particle;
s32 func_151B0050(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 pitch, s16 yaw, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);
void func_151B01B8(Actor *actor, Actor *other);
void func_151B03B8(Actor *actor, u8 arg1, s32 arg2);
Game1DD500Task *func_151B09BC(Actor *actor, Actor *other, s16 lifetime, u8 arg3, s32 arg4);
void func_151B0B88(Game1DD500Task *task);
s32 func_151B118C(Game1DD500Particle *particle);
s32 func_151B1478(Game1DD500Particle *particle);
Vtx *func_151B14AC(Game1DD500Particle *particle, s16 view);
s32 func_151B1828(Game1DD500Particle *arg0);
void func_151B1918(Game1DD500Task *task);
void func_151B19A4(Game1DD500Task *task, void *msg, u8 type);
void func_151B1A58(Game1DD500Task *task);
void func_151B1A84(Game1DD500Task *task);

/* game_1E3050.c */
/* (Actor *spawner, Actor *spawned); declared without a prototype: game_981E0.c also passes an angle */
void func_151B5BA0();

/* game_1E5820.c */
typedef struct Effect1E5820 Effect1E5820;
void func_151B8370(Effect1E5820 *effect);
void func_151B83A0(Effect1E5820 *effect);
void func_151B83CC(Effect1E5820 *effect);

/* game_1E73B0.c */
typedef struct SoundChannel SoundChannel;
void *func_151B9F00(Actor *owner, u8 arg1, s32 arg2);
s32 func_151BA084(DynModel *effect, s16 viewport);
void func_151BA468(DynModel *effect, ActorMsg *msg, u8 msgType);
void func_151BA518(Actor *obj, u8 arg1, s32 arg2);
void func_151BB044(Actor *obj);
/* sound update callback */
s32 func_151BB0E0(SoundChannel *chan, s32 arg1, u32 *volume, s32 arg3, s32 *speedOut, s32 arg5, u16 *soundId);
void func_151BB61C(Actor *obj, Vec3f *from, Vec3f *to, u8 arg3, s32 arg4);
void func_151BBA9C(Actor *obj, f32 *a, f32 *b, u8 arg3, s32 arg4);
void func_151BBEE4(Vec3f *pos, u8 arg1, s32 arg2);
void func_151BBFBC(CollisionTri *surf, Vec3f *pos, s32 arg2, s32 arg3);
void func_151BC074(Actor *obj);
void func_151BC104(Actor *obj, u8 arg1, s32 arg2);

/* game_1EB6C0.c */
typedef struct OwnedParticle OwnedParticle;
s32 func_151BE210(BurstSpark *spark);
Vtx *func_151BE4B8(BurstSpark *spark, s16 viewport);
void func_151BE558(OwnedParticle *particle, ActorMsg *msg, u8 msgId);
void func_151BE604(BurstSpark *spark, s32 msg, u8 msgId);
void func_151BE644(BurstSpark *spark);
void func_151BE674(BurstSpark *spark);
void func_151BE6A0(BurstSpark *spark);
s32 func_151BE6CC(BurstGlow *glow);
void func_151BE788(BurstGlow *glow, s32 msg, u8 msgId);
void func_151BE7C8(BurstGlow *glow);
void func_151BE7F8(BurstGlow *glow);
void func_151BE824(BurstGlow *glow);

/* game_1EBD00.c */
void *func_151BE850(Vec3f *pos, f32 angle, u8 count, u8 arg3, u8 variant, u8 owner, s32 list);

/* game_1EBFD0.c */
struct S151BEB20;
typedef struct S151BECB8 S151BECB8;
s32 func_151BEB20(struct S151BEB20 *arg0);
s32 func_151BEC94(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4);
s32 func_151BECB8(S151BECB8 *arg0);
s32 func_151BEE94(Actor *arg0);

/* game_1FF760.c */
void func_151D22B0(Actor *actor, u8 arg1, s32 arg2);

/* game_1FFF60.c */
typedef struct BoneFollower BoneFollower;
typedef struct ActorFollowerGroup ActorFollowerGroup;
void func_151D2AB0(s32 index);
void func_151D2B4C(s32 index);
BoneFollower *func_151D2BA4(void *params, Vec3f *position, s32 extraSize, u8 arg3, s32 arg4);
void func_151D2C40(BoneFollower *follower);
void func_151D2DAC(BoneFollower *follower);
void func_151D2DCC(BoneFollower *arg0);
void func_151D2E14(BoneFollower *arg0);
void func_151D2E5C(BoneFollower *follower, ActorMsg *msg, u8 type);
ActorFollowerGroup *func_151D2F00(void *params, s32 extraSize, u8 arg2, s32 arg3);
void func_151D2F90(ActorFollowerGroup *group);
void func_151D3130(ActorFollowerGroup *group);
void func_151D31F4(ActorFollowerGroup *group);
void func_151D3220(ActorFollowerGroup *group);
void func_151D324C(ActorFollowerGroup *group, ActorMsg *msg, u8 type);
void func_151D3308(BoneFollower *follower);
void func_151D3354(BoneFollower *follower);
void func_151D33B8(ActorFollowerGroup *group);
void func_151D33FC(ActorFollowerGroup *group, ActorMsg *msg);
void func_151D343C(s32 arg0, u8 arg1);

#endif
