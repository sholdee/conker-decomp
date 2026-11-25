#ifndef CONKER_TRAIL_H
#define CONKER_TRAIL_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "trail": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_DC360.c
 *   game_E1280.c
 *   game_EEB10.c
 *   game_100810.c
 *   game_105FC0.c
 *   game_10A350.c
 *   game_175250.c
 *   game_1897A0.c
 *   game_1B5CC0.c
 *   game_1B6DB0.c
 *   game_1B8F40.c
 *   game_1CA420.c
 *   game_1CC440.c
 *   game_1D0840.c
 *   game_1E37D0.c
 *   game_1FBED0.c
 *   game_1FC0C0.c
 *   game_1FC5D0.c
 *   game_1FC830.c
 *   game_1FEA80.c
 *   game_204660.c
 */

/* Defined in the C files of this subsystem. A typedef here can reorder the .bss of other files (see
 * init_50A0.c), a struct tag does not. */
struct ScatterTrail;
struct TrailGlow;
struct CharacterTrail;
struct TrailSound;
struct PissTrail;
struct TrailMessage;
struct TrailSpark;
struct Game10A350Actor;
struct FallingTrail;
struct LevelObject;
struct LevelEffect;
struct TrailBurstEmitter;
struct SprayTrail;
struct SprayEmitter;
struct TrailEmitterParams;
struct TrailEmitter;
struct MatrixTrailData;
struct MatrixTrail;
struct TrailTask;
union TrailEvent;
struct SurfaceTrailOwner;
struct SurfaceTrail;
struct TrailSpawner;
struct BoneTrailParams;
struct BoneTrailEffect;
struct BoneEmitterTask;
struct TrailRetryTask;
struct TrailTarget;
struct TrailTargetRef;
struct TrailScalarMotion;
struct RibbonActor;
struct StripEffect;
struct OwnedEffect;
struct CarrierParticle;
struct TrailBoneLink;
struct AnchoredTrailEmitter;
struct AnchoredTrail;

/* game_DC360.c */
void func_150AEEB0(Actor *obj, u8 owner);
s32 func_150AF1C0(MotionTrail *trail);

/* game_E1280.c */
void func_150B3DD0(void);
void func_150B3E74(struct ScatterTrail *trail);
void func_150B3EE8(struct ScatterTrail *trail);
struct ScatterTrail *func_150B3F5C(TrailStateParams *params, Vec3f *pos, s16 capacity);
s32 func_150B40E8(struct ScatterTrail *trail);
s32 func_150B4294(struct ScatterTrail *trail);
Gfx *func_150B4710(struct ScatterTrail *trail, Gfx *gfx, s16 camIdx);
void func_150B5060(struct ScatterTrail *trail);
void func_150B5088(struct ScatterTrail *trail);
void func_150B538C(struct ScatterTrail *trail, s32 arg1, u8 msg);

/* game_EEB10.c */
void func_150C1660(f32 x, f32 y, f32 z, u8 arg3);
s32 func_150C16C0(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s32 angle, s32 baseAngle, f32 radius, s32 arg11, f32 arg12, s32 arg13, u8 arg14);
s32 func_150C1978(MotionTrail *trail);

/* game_100810.c */
void func_150D3360(Actor *owner, u8 arg1, s32 arg2);
s32 func_150D3A68(struct CharacterTrail *actor);
s32 func_150D3FD4(struct CharacterTrail *actor);
Gfx *func_150D4300(struct CharacterTrail *actor, Gfx *gfx, s16 view);
void func_150D49C0(struct CharacterTrail *actor, ActorMsg *msg, u8 id);
s32 func_150D4AE0(Vec3f *start, Vec3f *end, Actor *owner, ModelAttachment *attachment);
void func_150D4BD4(struct CharacterTrail *actor);
void func_150D4C00(struct CharacterTrail *actor);
void func_150D4C2C(struct CharacterTrail *actor);
s32 func_150D4CC4(struct TrailGlow *particle);
void func_150D4D58(Vec3f *pos, Vec3f *vel, f32 angle, u8 arg3, s32 arg4);
s32 func_150D5124(struct TrailSound *snd, s32 arg1, u32 *arg2, s32 arg3, s32 *pitchOut, s32 arg5, u16 *playing);

/* game_105FC0.c */
void func_150D8B10(Actor *actor, Vec3f *out);
void func_150D8B3C(Actor *actor, Vec3f *out);
void func_150D8B88(Actor *actor);
void func_150D8D84(Vec3f *pos, Vec3f *vel, f32 dt);
void func_150D8E1C(struct PissTrail *trail);
s32 func_150D8E4C(struct PissTrail *trail);
s32 func_150D8FAC(struct PissTrail *trail);
s32 func_150D942C(struct PissTrail *trail);
Gfx *func_150D9C7C(struct PissTrail *trail, Gfx *gfx, s16 view);
void func_150DA484(struct PissTrail *trail);
void func_150DA4B4(struct PissTrail *trail);
void func_150DA4E0(struct PissTrail *trail);
void func_150DA50C(struct PissTrail *trail, struct TrailMessage *msg, u8 type);
void func_150DA5EC(Actor *actor, f32 speedScale);
s32 func_150DA628(struct TrailSpark *particle, s32 arg1);
void func_150DA67C(struct PissTrail *trail, RayHit *hit, f32 *dir);

/* game_10A350.c */
s32 func_150DCEA0(struct Game10A350Actor *actor);
Gfx *func_150DD640(struct Game10A350Actor *actor, Gfx *gfx, s16 view);

/* game_175250.c */
MotionTrail *func_15147DA0(EffectTemplate *spawn, TrailMotion *motion, s32 extraSize, s32 state, s32 arg4, s32 finishFn, s32 hitFn, s32 hitKind3Fn, s32 arg8, s32 arg9, s32 arg10, TrailRender *render, s32 arg12, u8 arg13, s32 arg14);
s8 func_15147EB8(MotionTrail *trail);
Gfx *func_1514803C(MotionTrail *trail, Gfx *gfx, s16 arg2);
s32 func_151488C4(MotionTrail *trail);
s32 func_15148AF4(MotionTrail *trail);
s32 func_15148BA4(MotionTrail *trail);
s32 func_15148DE0(MotionTrail *trail);
s32 func_15148EF8(MotionTrail *trail, s32 arg1, s32 arg2, s32 arg3);
void func_15148F1C(u8 texture, f32 x, f32 y, f32 z, s32 arg4, s32 yaw, s32 pitch, f32 speed, s16 arg8, f32 halfWidth, f32 gravity, u8 arg11);
s32 func_151490C8(MotionTrail *trail);
void func_15149104(EffectObj *obj);

/* game_1897A0.c */
struct FallingTrail *func_1515C2F0(EffectTemplate *header, s32 extraSize, TrailBurstParams *data, void *arg3, u8 arg4, s32 arg5);
s32 func_1515C388(struct FallingTrail *trail);
s32 func_1515C534(struct FallingTrail *trail);
Gfx *func_1515C6F4(struct FallingTrail *trail, Gfx *gfx, s16 view);
s32 func_1515CF9C(struct FallingTrail *trail, TrailBurstParams *data);
s32 func_1515D030(struct FallingTrail *trail, TrailBurstParams *data);
struct LevelEffect *func_1515D088(struct LevelObject *obj);
void func_1515D130(struct TrailBurstEmitter *owner);

/* game_1B5CC0.c */
void func_15188810(Actor *object, s32 duration, s32 maxDist);
void func_1518894C(WorldObject *owner, s32 duration, s32 maxDist);
void func_15188A58(BoneTrail *trail, BoneTrail **list);
void func_15188A9C(Actor *object);
void func_15188AD0(Actor *object);
void func_15188B74(s32 listIndex);
Gfx *func_15188D00(Gfx *gfx, s32 listIndex, s16 player);
void func_15188E48(BoneTrail *trail);
void func_15188F84(BoneTrail *arg);
void func_15189118(s32 listIndex);
void func_151895A4(BoneTrail *trail);
void func_151895CC(BoneTrail *trail);
Gfx *func_151895F4(Gfx *gfx, BoneTrail *trail, s16 player);
Gfx *func_151897A4(Gfx *gfx, BoneTrail *trail, s16 player);
void func_151898C0(Actor *object, s32 value);

/* game_1B6DB0.c */
void func_15189900(SprayDesc *desc, u8 arg1);
void func_15189A00(struct SprayEmitter *emitter);
s32 func_15189EBC(struct SprayTrail *trail);
s32 func_15189FD0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* game_1B8F40.c */
struct TrailEmitter *func_1518BA90(Actor *actor, u8 owner, s32 list);
void func_1518BBF4(Actor *actor);
struct TrailEmitter *func_1518BCD0(struct TrailEmitterParams *params, u8 owner, s32 list);
void func_1518BD60(struct TrailEmitter *emitter);
s32 func_1518C0B8(MotionTrail *trail);
s32 func_1518C540(MotionTrail *trail);
s32 func_1518C57C(MotionTrail *trail, s32 arg1, s32 arg2, s32 arg3, f32 hitY);
s32 func_1518C69C(MotionTrail *trail, s32 arg1, s32 arg2, s32 arg3, f32 hitY, s32 arg5);
void func_1518C850(struct TrailEmitter *emitter, s32 data, u8 type);

/* game_1CA420.c */
void func_1519CF70(u8 specIndex);
void func_1519CFA0(struct MatrixTrail *trail);
void func_1519CFD0(struct MatrixTrail *trail);
void func_1519D000(struct MatrixTrail *trail);
struct MatrixTrail *func_1519D030(Actor *owner, s32 specIndex, s16 lifetime, u8 arg3, u8 arg4, s32 arg5);
s32 func_1519D240(struct MatrixTrail *trail);
s32 func_1519D454(struct MatrixTrail *trail);
Gfx *func_1519D9F4(struct MatrixTrail *trail, Gfx *gfx, s16 view);
void func_1519E1F4(struct MatrixTrail *trail, union TrailEvent *event, u8 eventId);
s32 func_1519E304(struct MatrixTrailData *data, Vec3f *out, f32 x, f32 y, f32 z, f32 t);
void func_1519E3BC(Actor *owner, s16 lifetime, u8 arg2, s32 arg3);
void func_1519E464(struct TrailTask *task);
void func_1519E570(struct TrailTask *task, union TrailEvent *event, u8 eventId);
void func_1519E61C(EffectHeader *task, union TrailEvent *event, u8 eventId);
void func_1519E65C(s32 unused);
void func_1519E688(void);
void func_1519E6BC(Actor *owner);
void func_1519E754(struct TrailTask *task);
void func_1519E818(struct TrailTask *task, union TrailEvent *event, u8 eventId);
void func_1519E8CC(struct TrailTask *task);
void func_1519E8F8(struct TrailTask *task);
void func_1519E924(TimedEffect *task);
void func_1519E948(TimedEffect *task);

/* game_1CC440.c */
void func_1519F108(struct SurfaceTrail *trail);
void func_1519F168(struct SurfaceTrail *trail);
void func_1519F3B8(struct SurfaceTrailOwner *owner);
void func_1519F400(struct SurfaceTrailOwner *owner);
void func_1519F48C(struct SurfaceTrail *trail);
s32 func_1519F4F0(struct SurfaceTrail *trail);
s32 func_1519F7F0(struct SurfaceTrail *trail);
Gfx *func_1519FE6C(struct SurfaceTrail *trail, Gfx *gfx, s16 arg2);
void func_151A084C(struct TrailSpawner *spawner);
void func_151A0928(struct TrailSpawner *spawner);
void func_151A0950(struct SurfaceTrail *trail, ActorRef *event, u8 type);
void func_151A09B4(struct TrailSpawner *spawner, ActorRef *event, u8 type);

/* game_1D0840.c */
void func_151A3390(Actor *owner, u8 arg1);
struct BoneTrailEffect *func_151A3504(struct BoneTrailParams *params, u8 arg1);
s32 func_151A361C(struct BoneTrailEffect *effect);
s32 func_151A37C0(struct BoneTrailEffect *effect);
Gfx *func_151A3BE4(struct BoneTrailEffect *effect, Gfx *gfx, s16 view);
void func_151A4590(Actor *owner, u8 arg1);
void func_151A4638(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, EffectHeader *parent);
s32 func_151A483C(SpriteParticle *particle, u8 arg1);
s32 func_151A4900(SpriteParticle *particle, u8 arg1);
void func_151A499C(Actor *owner, u8 arg1);
void func_151A4A38(struct BoneEmitterTask *task);
void func_151A4CE0(struct BoneTrailEffect *effect, ActorMsg *msg, u8 type);
void func_151A4D88(struct BoneEmitterTask *task, ActorMsg *msg, u8 type);
s32 func_151A4E34(struct BoneTrailParams *params, Vec3f *out);
void func_151A4E9C(struct BoneTrailEffect *effect);
void func_151A4ECC(struct TrailRetryTask *task);
void func_151A4F7C(struct TrailRetryTask *task, ActorMsg *msg, u8 type);

/* game_1E37D0.c */
void func_151B6320(struct TrailTarget *target, u8 arg1, s32 arg2);
s32 func_151B6420(EffectObj *obj);
s32 func_151B65D4(EffectObj *obj);
Gfx *func_151B6928(EffectObj *obj, Gfx *gfx, s16 view);
void func_151B70B4(EffectObj *obj, void *msg, u8 msgType);
void func_151B7144(struct TrailTarget *target, u8 arg1, s32 arg2);
EffectObj *func_151B7328(void *args, u8 selector, s32 argsSize, u8 arg3, s32 arg4);
s32 func_151B7678(EffectObj *obj, Vec3f *pos);
s32 func_151B76CC(EffectObj *obj, Vec3f *pos);
s32 func_151B77F4(EffectObj *obj);
s32 func_151B7998(EffectObj *obj);
Gfx *func_151B7C38(EffectObj *obj, Gfx *gfx, s16 view);
void func_151B82CC(EffectObj *obj, void *msg, u8 msgType);
void func_151B8318(EffectObj *obj, struct TrailTargetRef *msg, u8 msgType);

/* game_1FBED0.c */
s32 func_151CEA20(struct TrailScalarMotion *motion);
WaveTrail *func_151CEAAC(Actor *owner, DescParticleHead *anchor, u8 arg2, u8 arg3, s32 arg4);

/* game_1FC0C0.c */
s32 func_151CEC10(Vec3f *out, Actor *owner, DescParticleHead *anchor);
s32 func_151CEC54(WaveTrail *trail);

/* game_1FC5D0.c */
s32 func_151CF120(WaveTrail *actor);

/* game_1FC830.c */
Gfx *func_151CF380(struct RibbonActor *ribbon, Gfx *gfx, s16 cameraIdx);
void func_151CF844(TrackerTrail *actor, s32 arg1, u8 arg2);
void func_151CF898(Actor *obj, f32 probeY, f32 arg2);
void func_151CFA4C(Actor *obj, u8 arg1, s32 arg2);
void func_151D0024(Actor *obj);
void func_151D0058(Actor *obj, u8 variant, u8 arg2, s32 arg3);
void func_151D0128(struct StripEffect *effect);
Gfx *func_151D014C(Gfx *gfx, struct StripEffect *effect, s16 cameraIdx);
void func_151D08F0(struct OwnedEffect *effect, void *msg, u8 type);
void func_151D09A8(Actor *obj, u8 arg1, s32 arg2);
void func_151D0ED8(struct CarrierParticle *particle);
void func_151D0F08(struct CarrierParticle *particle);
void func_151D0F34(struct CarrierParticle *particle);
void func_151D0F60(Actor *obj, u8 pointFn, u8 arg2, s32 arg3);
s32 func_151D1074(Actor *obj, Vec3f *out);
s32 func_151D10A4(Actor *obj, Vec3f *out);
s32 func_151D10C4(Actor *obj, Vec3f *out);
s32 func_151D10E4(Actor *obj, Vec3f *out, u8 which);
void func_151D1138(TrackerEffect *effect);
void func_151D1328(TrackerEffect *effect, s32 arg1, u8 arg2);
void func_151D1368(TrackerEffect *effect);
void func_151D1388(TrackerEffect *effect);
void func_151D13B4(TrackerEffect *effect);
void func_151D13E0(TrackerEffect *effect);
void func_151D1448(TrackerEffect *effect);

/* game_1FEA80.c */
s32 func_151D15D0(TrackerTrail *actor);
s32 func_151D197C(TrackerTrail *actor);
Gfx *func_151D1C98(TrackerTrail *actor, Gfx *gfx, s16 arg2);
void func_151D223C(TrackerTrail *actor);
void func_151D2258(TrackerTrail *actor);
void func_151D2284(TrackerTrail *actor);

/* game_204660.c */
TimedEffect *func_151D71B0(s16 arg0, u8 arg1, u8 anchorType, f32 maxStep, s32 extraSize, u8 arg5, s32 arg6);
void func_151D7264(struct AnchoredTrailEmitter *emitter);
void func_151D73A8(struct AnchoredTrailEmitter *emitter, struct TrailBoneLink *msg, u8 msgType);
void func_151D7404(struct AnchoredTrailEmitter *emitter);
void func_151D7424(TimedEffect *emitter);
void func_151D7450(TimedEffect *emitter);
void func_151D747C(Actor *actor);
void func_151D74B0(Actor *actor, u8 bone, s8 condition, u8 arg3, s32 arg4);
void func_151D7538(struct AnchoredTrailEmitter *emitter, struct TrailBoneLink *msg, u8 msgType);
s32 func_151D75C4(struct AnchoredTrailEmitter *emitter);
s32 func_151D7724(struct AnchoredTrailEmitter *emitter);
s32 func_151D7770(struct AnchoredTrailEmitter *emitter);
s32 func_151D779C(struct AnchoredTrailEmitter *emitter);
void func_151D77C8(struct AnchoredTrailEmitter *emitter);
void func_151D7830(struct AnchoredTrailEmitter *emitter);
s32 func_151D792C(struct AnchoredTrail *trail);
s32 func_151D7A38(struct AnchoredTrail *trail);
s32 func_151D7CD0(struct AnchoredTrail *trail);
Gfx *func_151D80C4(struct AnchoredTrail *trail, Gfx *gfx, s16 view);
void func_151D8718(Vec3f *pos, f32 *velY, f32 dt);
void func_151D8764(struct AnchoredTrail *trail);
void func_151D8780(struct AnchoredTrail *trail);
void func_151D87AC(struct AnchoredTrail *trail);

#endif
