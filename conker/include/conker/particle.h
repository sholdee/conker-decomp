#ifndef CONKER_PARTICLE_H
#define CONKER_PARTICLE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "particle": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_35E80.c
 *   game_39750.c
 *   game_DC6B0.c
 *   game_DF2D0.c
 *   game_DF930.c
 *   game_E5E90.c
 *   game_E6A10.c
 *   game_E7380.c
 *   game_E7970.c
 *   game_E7DE0.c
 *   game_EDE60.c
 *   game_EE710.c
 *   game_EF410.c
 *   game_EFEA0.c
 *   game_F0610.c
 *   game_F3270.c
 *   game_F8590.c
 *   game_F9430.c
 *   game_FA240.c
 *   game_FA360.c
 *   game_FB060.c
 *   game_FFBA0.c
 *   game_1028F0.c
 *   game_113480.c
 *   game_113D60.c
 *   game_116D40.c
 *   game_1199D0.c
 *   game_11FF10.c
 *   game_120950.c
 *   game_121A20.c
 *   game_128D70.c
 *   game_12DAB0.c
 *   game_135780.c
 *   game_1368C0.c
 *   game_15D730.c
 *   game_15F680.c
 *   game_169510.c
 *   game_176A00.c
 *   game_177480.c
 *   game_177B50.c
 *   game_1792B0.c
 *   game_1792D0.c
 *   game_179400.c
 *   game_1797A0.c
 *   game_17CAF0.c
 *   game_183640.c
 *   game_188440.c
 *   game_193E50.c
 *   game_197120.c
 *   game_19A8B0.c
 *   game_19F150.c
 *   game_1A1B40.c
 *   game_1A7260.c
 *   game_1A7490.c
 *   game_1B74A0.c
 *   game_1B81D0.c
 *   game_1B8B60.c
 *   game_1B9F30.c
 *   game_1BA1D0.c
 *   game_1BA670.c
 *   game_1BB950.c
 *   game_1BC650.c
 *   game_1BD940.c
 *   game_1BDC20.c
 *   game_1BF090.c
 *   game_1BFC70.c
 *   game_1BFDD0.c
 *   game_1C0B10.c
 *   game_1CDEC0.c
 *   game_1CE2F0.c
 *   game_1D0600.c
 *   game_1D2B10.c
 *   game_1D3AB0.c
 *   game_1D9A00.c
 *   game_1DBD40.c
 *   game_1DC720.c
 *   game_1DCA70.c
 *   game_1DEF60.c
 *   game_1E30A0.c
 *   game_1E34C0.c
 *   game_1E5FF0.c
 *   game_1E6B40.c
 *   game_1E9820.c
 *   game_1F1CD0.c
 *   game_1FA770.c
 *   game_1FBB80.c
 *   game_200930.c
 *   game_2062D0.c
 *   game_209B50.c
 *   game_20A290.c
 */

/* An RGB colour, one byte per channel (game_F8590.c, game_F9430.c). */
typedef struct Rgb8 {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
} Rgb8; /* size 0x3 */

/* game/done/game_35E80.c */
void func_150089D0(void);

/* game_39750.c */
void func_1500C2A0(void);
void func_1500C2F0(s32 region, f32 density);
void func_1500C8D0(s32 region, f32 density);
void func_1500CE4C(s32 region, f32 density);
void func_1500D3AC(s32 region, f32 density);
void func_1500D8C8(s32 region, f32 density);
void func_1500DE88(s32 region, f32 density);

/* game_DF930.c */
s32 func_150B2480(s32 arg0, s32 arg1);
void func_150B2494(Actor *obj, s32 arg1, s32 arg2);
s32 func_150B2570(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 owner);

/* game_E7DE0.c */
s32 func_150BA930(Vec3f *pos, Actor *obj, CollisionHit *hit, s32 arg3);
s32 func_150BAA00(s32 arg0, s32 arg1);
void func_150BAA14(Actor *obj, s32 arg1, s32 arg2);

/* game_EE710.c */
void func_150C1260(Actor *actor, u8 foot);

/* game_EF410.c */
void func_150C1F60(u8 arg0);
void func_150C2290(u8 arg0);
void func_150C2424(u8 arg0);
s32 func_150C251C(ChannelParticle *particle);
s32 func_150C2558(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);
s32 func_150C2700(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);
void func_150C2804(f32 *x, f32 *y, f32 *z, s16 delay, s32 arg4, s32 arg5);
void func_150C2898(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, Struct150C29F0 *emitter);

/* game_EFEA0.c */
void func_150C29F0(Struct150C29F0 *emitter);
void func_150C2C00(Struct150C29F0 *emitter);
s32 func_150C2FCC(ChannelParticle *particle);
s32 func_150C308C(ChannelParticle *particle);

/* game_F0610.c */
Gfx *func_150C3160(Gfx *gdl, Actor *obj);
void func_150C3230(Actor *obj);
void func_150C3574(Actor *obj);
void func_150C3994(Actor *obj);
s32 func_150C3D48(s32 value);
void func_150C3D5C(Actor *obj);

/* game_FA240.c */
void func_150CCD90(f32 z, f32 *r, f32 *g, f32 *b);

/* game_FB060.c */
void func_150CDBB0(Actor *actor, u8 owner, s32 arg2);
s32 func_150CDCF4(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 arg14);
s32 func_150CDF10(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 arg14);

/* game_120950.c */
f32 func_150F34A0(Actor *obj, f32 pitch);
void func_150F34F4(Actor *actor);
void func_150F43F0(Camera *actor);

/* game_15D730.c */
SpriteParticle *func_15130280(ParticleDesc *desc, u8 kind, CollisionHit *ground, s32 dataSize, u8 arg4, s32 arg5);
SpriteParticle *func_15130374(ParticleDesc *desc, u8 kind, s32 dataSize, u8 arg3, s32 arg4);
SpriteParticle *func_151303BC(ParticleDesc *desc, u8 kind, s32 dataSize);
void func_151303EC(SpriteParticle *p);
Gfx *func_15130A9C(Gfx *gfx, SpriteParticle *p, s16 viewIdx);
void func_1513137C(s16 *r, s16 *g, s16 *b, s16 *a, SpriteParticle *p);
void func_15131514(s16 *r, s16 *g, s16 *b, s16 *a, SpriteParticle *p);
void func_1513164C(s16 *primR, s16 *primG, s16 *primB, s16 *primA, s16 *envR, s16 *envG, s16 *envB, s16 *envA,
                   SpriteParticle *p);
s32 func_151316AC(SpriteParticle *p, Vec3f *prevPos);
s32 func_151316DC(SpriteParticle *p, Vec3f *prevPos);
s32 func_1513170C(SpriteParticle *p, Vec3f *prevPos);
void func_1513173C(SpriteParticle *p);
void func_1513175C(SpriteParticle *p);
void func_1513177C(SpriteParticle *p);
s32 func_151317C8(SpriteParticle *p);
s32 func_15131814(s32 arg0, s32 arg1);
void func_15131828(SpriteParticle *p, f32 *amplitude, u8 *phase, u8 *phaseStep);
void func_151318E8(Vec3f *vel, f32 drag);
void func_15131918(Vec3f *vel, f32 drag);
void func_15131958(Vec3f *vel, f32 drag);
s32 func_151319A4(SpriteParticle *p, Vec3f *prevPos);
s32 func_151319C4(SpriteParticle *p, Vec3f *prevPos, ParticleBounce *bounce);
s32 func_15131AFC(SpriteParticle *p, Vec3f *prevPos);
s32 func_15131B3C(SpriteParticle *p, Vec3f *prevPos);
s32 func_15131B7C(SpriteParticle *p, Vec3f *prevPos, f32 *hit, ParticleBounce *bounce);
void func_15131C2C(SpriteParticle *p, s32 arg1, u8 arg2);
void func_15131C84(u8 *phase, s8 *phaseStep, f32 base, f32 *amplitude, f32 *outX, f32 *outY);
void func_15131D4C(s32 arg0, u8 arg1);
s32 func_15131D9C(SpriteParticle *p, Vec3f *prevPos);
s32 func_15131DEC(SpriteParticle *p, Vec3f *prevPos);
SpriteParticle *func_15131EE4(Vec3f *pos, f32 size, u8 arg2, s32 arg3);

/* game_15F680.c */
void func_151321D0(ModelParticle *particle);
void func_151323AC(ModelParticle *particle);
void func_151323F8(ModelParticle *particle);
void func_15132444(ModelParticle *particle);
void func_15132570(ModelParticle *particle);
void func_1513259C(ModelParticle *particle);
void func_151325C8(ModelParticle *particle);
ModelParticle *func_1513264C(ParticleSpawn *spawn, s32 arg1, s32 arg2, CollisionHit *floor, s32 extraSize, u8 arg5,
                             s32 arg6);
ModelParticle *func_15132A4C(ParticleSpawn *spawn, s32 arg1, s32 arg2, s32 extraSize, u8 arg4, s32 arg5);
void func_15132A88(ModelParticle *particle);
Gfx *func_15132B80(Gfx *gfx, ModelParticle *arg1, s16 arg2);
s32 func_15132DDC(ModelParticle *particle);
s32 func_151332DC(ModelParticle *particle);
s32 func_15133510(Mtx *mtx, ModelParticle *particle);
void func_15133588(ModelParticle *particle, void *data, u8 event);
s32 func_151336A8(s32 model, ParticleNode *node, ModelParticle *particle);
s32 func_15133760(Mtx *mtx, ModelParticle *particle);
s32 func_151337C0(ModelParticle *particle);
s32 func_15133894(ModelParticle *particle);
s32 func_151339D4(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight);
s32 func_15133A50(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight);
s32 func_15133A94(Mtx *mtx, ModelParticle *particle);
s32 func_15133B98(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface);
s32 func_15133C58(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface);
s32 func_15133D20(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface);
void func_15133DE8(ModelParticle *particle, ActorRef *data, u8 event);
void func_15133E3C(s32 arg0, u8 arg1);
Gfx *func_15133E84(Gfx *gfx, ModelParticle *particle, s16 arg2);
Gfx *func_15133EB8(Gfx *gfx, ModelParticle *particle, s16 arg2);
Gfx *func_15133EEC(Gfx *gfx, u16 index, u8 segment, s32 arg3);
Gfx *func_15133FD8(Gfx *gfx, ModelParticle *particle, s16 arg2);

/* game_176A00.c */
ChannelParticle *func_15149550(ParticleEmitterDesc *emitter, u8 arg1, u8 arg2, u8 kind, u8 arg4, s32 arg5);
s32 func_15149838(ChannelParticle *particle);
s32 func_15149A94(ChannelParticle *particle);
s32 func_15149BF4(ChannelParticle *particle);
s32 func_15149C58(ChannelParticle *particle);
Vtx *func_15149D18(ChannelParticle *particle, s16 index);
s32 func_15149EC4(ChannelParticle *particle);

/* game_1792D0.c */
void func_1514BE20(void *arg);

/* game_179400.c */
void func_1514BF50(JitterParticle *p);
void func_1514BF7C(JitterParticle *p);
void func_1514BF9C(JitterParticle *p);
s32 func_1514C258(JitterParticle *p);
s32 func_1514C288(JitterParticle *p);
s32 func_1514C2B8(JitterParticle *p);

/* game_1797A0.c */
void func_1514C2F0(f32 x, f32 y, f32 z, f32 radius, u8 startAngle, s8 angleStep, s16 count, u8 kind, s32 arg8, f32 arg9,
                   s32 arg10, u8 arg11);
void func_1514C470(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 count, u8 kind, s32 arg8, f32 arg9, s32 arg10,
                   u8 arg11);
void func_1514C678(f32 x, f32 y, f32 z, f32 radius, s16 angleA, s16 angleB, s16 count, u8 kind, s32 arg8, f32 arg9,
                   s32 arg10, u8 arg11);
void func_1514C858(f32 x, f32 y, f32 z, f32 radius, s16 yaw, s16 angleA, s16 angleB, s16 count, u8 kind, s32 arg9,
                   f32 arg10, s32 arg11, u8 arg12);

/* game_1A7260.c */
void func_15179DB0(u8 slot);

/* game_1B81D0.c */
s32 func_1518AD20(FloorParticle *particle);
s32 func_1518B1AC(FloorParticle *particle);
s32 func_1518B1D8(FloorParticle *particle);
void func_1518B264(Vec3f *pos, f32 radius, f32 size, s32 count, u8 arg4);
void func_1518B2A8(Vec3f *pos, f32 radius, f32 size, s32 count, u16 model, s32 arg5, u8 arg6);

/* game_1B8B60.c */
void func_1518B6B0(f32 arg0, f32 arg1, f32 arg2, u8 arg3, s32 arg4);

/* game_1BA1D0.c */
void func_1518CD20(WorldObject *placement, u8 arg1, s32 arg2);

/* game_1BB950.c */
void func_1518E4A0(ActorEmitterProcess *emitter);
void func_1518E4CC(ActorEmitterProcess *emitter);
void func_1518E4F8(ActorEmitterProcess *emitter);
void func_1518E524(u8 *texture, u8 *flip, u8 *r, u8 *g, u8 *b, u8 *alpha, s16 *renderModes);
void func_1518E5D8(u8 *texture, u8 *flip, u8 *r, u8 *g, u8 *b, u8 *alpha, s16 *renderModes);
s32 func_1518E66C(ActorEmitterProcess *emitter);
s32 func_1518E6D4(ActorEmitterProcess *emitter);
Vtx *func_1518E73C(ActorEmitterParticle *p, s16 vp);

/* game_1BFC70.c */
void func_151927C0(BillboardParticle *p);

/* game_1D0600.c */
void func_151A3150(FadeEventParticle *particle);

/* game_1D2B10.c */
void func_151A5660(u8 *effect);
void func_151A5CAC(TimedEffect *arg0);
void func_151A5CEC(TimedEffect *arg0);
void func_151A5D2C(u16 soundHandle);
QuadSprite *func_151A5D58(f32 size, u8 arg1, s32 arg2, f32 *pos, s16 duration, u8 arg5, u8 arg6, u8 arg7, s32 arg8);

/* game_1DBD40.c */
void func_151AE890(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, u8 *arg6);
void func_151AE984(f32 *pos, f32 dirX, f32 dirY, f32 dirZ, s32 flag, u8 arg5);
void func_151AEAB4(f32 *pos, s32 flag, u8 arg2);

/* game_1DC720.c */
void *func_151AF270(Actor *actor, u8 arg1, s32 arg2);
void func_151AF338(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect);
void func_151AF388(f32 *position, f32 dx, f32 dy, f32 dz, s32 effectUnkC);

/* game_1E34C0.c */
PulseParticle *func_151B6010(Vec3f *pos, Vec3f *vel, f32 size, Vec3f *rot, Vec3f *rotVel, f32 gravity, s16 lifetime,
                             u8 alpha, s32 unused, f32 elasticity, u8 fadeShift, u8 arg11, u8 arg12, s32 arg13);
void func_151B6254(PulseParticle *particle);

/* game_1E9820.c */
void func_151BC370(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect);

/* game_20A290.c */
void func_151DCDE0(void *arg0, u8 arg1, s32 arg2);

#endif
