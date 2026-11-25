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
 *   game_16F770.c
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

/* game_193430.c */
/* Effect kind 4 (0xA8 bytes from func_15167A68): one textured quad, double-buffered by D_800BE9C0. */
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
void func_150B0A60(Actor *owner, Actor *player, f32 *forward, f32 *right, f32 *outPos, s16 *outAngle);
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
typedef struct TrapTrigger TrapTrigger;
typedef struct TrapEffect TrapEffect;
typedef struct EffectSpawnSource EffectSpawnSource;
void func_150DDED0(TrapTrigger *trap);
s32 func_150DDF88(TrapEffect *obj);
s32 func_150DDFAC(EffectSpawnSource *source, s32 arg1, s32 arg2, s32 arg3, f32 y, s32 arg5);
s32 func_150DE12C(EffectSpawnSource *source, s32 arg1, s32 arg2, s32 arg3, f32 y, s32 arg5);
s32 func_150DE2A4(QuadSprite *sprite);
void func_150DE2C4(BillboardParticle *particle);

/* game/done/game_10B7C0.c */
void func_150DE310(s32 arg0);

#endif
