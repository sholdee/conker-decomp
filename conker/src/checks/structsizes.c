/*
 * Struct size checks. The build compiles this file but does not link it, so it cannot change the ROM
 * or the .bss order of any other file (headers cannot hold the checks: IDO's .bss order depends on how
 * many types a file declares before its .bss). It checks each struct whose definition records its size,
 * or, for a struct with a variable tail, where the tail starts. A wrong size stops the build.
 *
 * IDO 5.3 has no _Static_assert, and it reports a negative array size only as a warning. A negative
 * bit-field width is an error, so the check is a one-bit field that becomes negative when cond is false.
 */
#include <ultra64.h>
#include <stddef.h>
#include "conker/beam.h"
#include "conker/camera.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/face.h"
#include "conker/level.h"
#include "conker/libc.h"
#include "conker/light.h"
#include "conker/particle.h"
#include "conker/rope.h"
#include "conker/screen.h"
#include "conker/shadow.h"
#include "conker/sprite.h"
#include "conker/swarm.h"
#include "conker/trail.h"
#include "conker/weapon.h"
#include "effects.h"
#include "structs.h"

#define STATIC_ASSERT(cond, name) struct name { unsigned int check : (cond) ? 1 : -1; }

/* structs.h */
STATIC_ASSERT(sizeof(Vec3f) == 0xC, Vec3f_size);
STATIC_ASSERT(sizeof(Tri3f) == 0x24, Tri3f_size);
STATIC_ASSERT(sizeof(Vec3s) == 0x6, Vec3s_size);
STATIC_ASSERT(sizeof(Vec2f) == 0x8, Vec2f_size);
STATIC_ASSERT(sizeof(Vec2fXZ) == 0x8, Vec2fXZ_size);
STATIC_ASSERT(sizeof(Vec3i) == 0xC, Vec3i_size);
STATIC_ASSERT(sizeof(SndState) == 0x58, SndState_size);
STATIC_ASSERT(sizeof(EffectHeader) == 0x10, EffectHeader_size);
STATIC_ASSERT(sizeof(TimedEffect) == 0x28, TimedEffect_size);
STATIC_ASSERT(sizeof(LevelMusic) == 0xE, LevelMusic_size);
STATIC_ASSERT(sizeof(LevelHeader) == 0x4C, LevelHeader_size);
STATIC_ASSERT(sizeof(ObjType) == 0x34, ObjType_size);
STATIC_ASSERT(sizeof(SoundRecord) == 0x30, SoundRecord_size);
STATIC_ASSERT(sizeof(IdleState) == 0x10, IdleState_size);
STATIC_ASSERT(sizeof(CollisionTri) == 0x12, CollisionTri_size);
STATIC_ASSERT(sizeof(RayHit) == 0x64, RayHit_size);
STATIC_ASSERT(sizeof(ActorCtl) == 0x1C0, ActorCtl_size);
STATIC_ASSERT(sizeof(SpawnRecord) == 0x30, SpawnRecord_size);
STATIC_ASSERT(sizeof(PlayerStats) == 0x1C, PlayerStats_size);
STATIC_ASSERT(sizeof(MatchState) == 0x20C, MatchState_size);
STATIC_ASSERT(sizeof(AnimInstance) == 0x3F8, AnimInstance_size);
STATIC_ASSERT(sizeof(ActorAnim) == 0x8, ActorAnim_size);
STATIC_ASSERT(sizeof(AnimHeader) == 0x10, AnimHeader_size);
STATIC_ASSERT(sizeof(SeqPathKey) == 0x8, SeqPathKey_size);
STATIC_ASSERT(sizeof(SeqTrackInfo) == 0x8, SeqTrackInfo_size);
STATIC_ASSERT(sizeof(ModelAttachment) == 0x5C, ModelAttachment_size);
STATIC_ASSERT(sizeof(ActorAimPoints) == 0x18, ActorAimPoints_size);
STATIC_ASSERT(sizeof(CollisionHit) == 0x24, CollisionHit_size);
STATIC_ASSERT(sizeof(VtxMorphState) == 0xC, VtxMorphState_size);
STATIC_ASSERT(sizeof(Actor) == 0x32C, Actor_size);
STATIC_ASSERT(sizeof(WorldObject) == 0xA0, WorldObject_size);
STATIC_ASSERT(sizeof(CamCmdState) == 0x30, CamCmdState_size);
STATIC_ASSERT(sizeof(CamCmd) == 0x38, CamCmd_size);
STATIC_ASSERT(sizeof(GameCommand) == 0x4, GameCommand_size);
STATIC_ASSERT(sizeof(CamAim) == 0x68, CamAim_size);
STATIC_ASSERT(sizeof(CamTransition) == 0x50, CamTransition_size);
STATIC_ASSERT(sizeof(CamRail) == 0x44, CamRail_size);
STATIC_ASSERT(sizeof(Camera) == 0x9A0, Camera_size);
STATIC_ASSERT(sizeof(AimPoint) == 0x14, AimPoint_size);
STATIC_ASSERT(sizeof(AimPointDef) == 0x18, AimPointDef_size);
STATIC_ASSERT(sizeof(SeqSubtitle) == 0x38, SeqSubtitle_size);
STATIC_ASSERT(sizeof(Texture) == 0xC, Texture_size);
STATIC_ASSERT(sizeof(SpriteParams) == 0x14, SpriteParams_size);
STATIC_ASSERT(sizeof(TextureDesc) == 0xC, TextureDesc_size);
STATIC_ASSERT(sizeof(ParticleTexInfo) == 0xA, ParticleTexInfo_size);
STATIC_ASSERT(sizeof(ParticleTexFile) == 0x10, ParticleTexFile_size);
STATIC_ASSERT(sizeof(BloodProfile) == 0x10, BloodProfile_size);
STATIC_ASSERT(sizeof(CamShot) == 0x38, CamShot_size);
STATIC_ASSERT(sizeof(TriggerZone) == 0x34, TriggerZone_size);
STATIC_ASSERT(sizeof(StageState) == 0x4C, StageState_size);
STATIC_ASSERT(sizeof(VtxColorGroup) == 0xC, VtxColorGroup_size);
STATIC_ASSERT(sizeof(TexAnimState) == 0x18, TexAnimState_size);
STATIC_ASSERT(sizeof(Bubble) == 0x10, Bubble_size);
STATIC_ASSERT(sizeof(WaterVtx) == 0x10, WaterVtx_size);
STATIC_ASSERT(sizeof(WaterBase) == 0xA, WaterBase_size);
STATIC_ASSERT(sizeof(WaterStage) == 0xC0, WaterStage_size);
STATIC_ASSERT(sizeof(RopeEnds) == 0x18, RopeEnds_size);
STATIC_ASSERT(sizeof(RopeSpawn) == 0x24, RopeSpawn_size);
STATIC_ASSERT(sizeof(StreamVoice) == 0xA0, StreamVoice_size);
STATIC_ASSERT(sizeof(SpawnPoint) == 0x18, SpawnPoint_size);
STATIC_ASSERT(sizeof(IgnoreList) == 0x10, IgnoreList_size);
STATIC_ASSERT(sizeof(ParticleAxes) == 0x18, ParticleAxes_size);
STATIC_ASSERT(sizeof(LightPulse) == 0x10, LightPulse_size);
STATIC_ASSERT(sizeof(LightJitter) == 0x20, LightJitter_size);
STATIC_ASSERT(sizeof(LightFlicker) == 0x14, LightFlicker_size);
STATIC_ASSERT(sizeof(LightActorRef) == 0x8, LightActorRef_size);
STATIC_ASSERT(sizeof(LightAttach) == 0x18, LightAttach_size);
STATIC_ASSERT(sizeof(LightBeam) == 0x28, LightBeam_size);
STATIC_ASSERT(sizeof(LightColourPulse) == 0x20, LightColourPulse_size);
STATIC_ASSERT(sizeof(LightCycle) == 0x2C, LightCycle_size);
STATIC_ASSERT(sizeof(LightStorm) == 0x34, LightStorm_size);
STATIC_ASSERT(sizeof(LightTrack) == 0x10, LightTrack_size);
STATIC_ASSERT(sizeof(LightFollow) == 0xC, LightFollow_size);
STATIC_ASSERT(sizeof(LightFlash) == 0x8, LightFlash_size);
STATIC_ASSERT(sizeof(LightTransformed) == 0x10, LightTransformed_size);
STATIC_ASSERT(sizeof(LightSwing) == 0x18, LightSwing_size);
STATIC_ASSERT(sizeof(struct GroundFlash) == 0x40, GroundFlash_size);
STATIC_ASSERT(sizeof(LightEffectHeader) == 0x6, LightEffectHeader_size);
STATIC_ASSERT(sizeof(SeqTrackState) == 0x44, SeqTrackState_size);
STATIC_ASSERT(sizeof(LightEventMsg) == 0x8, LightEventMsg_size);
STATIC_ASSERT(sizeof(IdRecord) == 0x20, IdRecord_size);
STATIC_ASSERT(sizeof(IdRecordList) == 0xC, IdRecordList_size);
STATIC_ASSERT(sizeof(BlockDebris) == 0xB8, BlockDebris_size);
STATIC_ASSERT(sizeof(ObjGroup) == 0x18, ObjGroup_size);
STATIC_ASSERT(sizeof(BounceRecord) == 0xA, BounceRecord_size);
STATIC_ASSERT(sizeof(EffectModelSlot) == 0x14, EffectModelSlot_size);
STATIC_ASSERT(sizeof(CamMsgQueue) == 0xB0, CamMsgQueue_size);
STATIC_ASSERT(sizeof(WaveDeformer) == 0x2C, WaveDeformer_size);
STATIC_ASSERT(sizeof(LensFlare) == 0x34, LensFlare_size);
STATIC_ASSERT(sizeof(CamPose) == 0x18, CamPose_size);
STATIC_ASSERT(sizeof(BoneTrail) == 0x18, BoneTrail_size);
STATIC_ASSERT(sizeof(RenderView) == 0x180, RenderView_size);
STATIC_ASSERT(sizeof(BoundCylinder) == 0xA, BoundCylinder_size);
STATIC_ASSERT(sizeof(BoundCylinderSet) == 0x8, BoundCylinderSet_size);
STATIC_ASSERT(sizeof(AimImpact) == 0x1C, AimImpact_size);
STATIC_ASSERT(sizeof(PipeSpot) == 0x10, PipeSpot_size);
STATIC_ASSERT(sizeof(BurstPoint) == 0x10, BurstPoint_size);
STATIC_ASSERT(sizeof(PropSounds) == 0xC, PropSounds_size);
STATIC_ASSERT(sizeof(ObjectBehavior) == 0xC, ObjectBehavior_size);
STATIC_ASSERT(sizeof(ModeSettings) == 0xA, ModeSettings_size);
STATIC_ASSERT(sizeof(HudSprite) == 0xC, HudSprite_size);
STATIC_ASSERT(sizeof(ZoneBounds) == 0x28, ZoneBounds_size);
STATIC_ASSERT(sizeof(PlayerMatchStats) == 0xC, PlayerMatchStats_size);
STATIC_ASSERT(sizeof(MenuState) == 0x98, MenuState_size);
STATIC_ASSERT(sizeof(ModeEntry) == 0xA, ModeEntry_size);
STATIC_ASSERT(sizeof(Racer) == 0x84, Racer_size);
STATIC_ASSERT(sizeof(PathNode) == 0x10, PathNode_size);
STATIC_ASSERT(sizeof(ObjTexture) == 0xC, ObjTexture_size);
STATIC_ASSERT(sizeof(VtxMorphGroup) == 0x8, VtxMorphGroup_size);
STATIC_ASSERT(sizeof(WaveSample) == 0xC, WaveSample_size);
STATIC_ASSERT(sizeof(WavePreset) == 0x18, WavePreset_size);
STATIC_ASSERT(sizeof(HitReaction) == 0x28, HitReaction_size);

/* effects.h */
STATIC_ASSERT(sizeof(ActorRef) == 0x8, ActorRef_size);
STATIC_ASSERT(sizeof(ActorSwapMsg) == 0xC, ActorSwapMsg_size);
STATIC_ASSERT(sizeof(ParticleDesc) == 0x70, ParticleDesc_size);
STATIC_ASSERT(sizeof(TextureChoices) == 0x10, TextureChoices_size);
STATIC_ASSERT(sizeof(ParticleSpawn) == 0x7C, ParticleSpawn_size);
STATIC_ASSERT(sizeof(ModelLink) == 0x20, ModelLink_size);
STATIC_ASSERT(sizeof(ModelParticle) == 0x170, ModelParticle_size);
STATIC_ASSERT(sizeof(BulletState) == 0xD0, BulletState_size);
STATIC_ASSERT(sizeof(Bullet) == 0x240, Bullet_size);
STATIC_ASSERT(sizeof(ParticleDef) == 0x58, ParticleDef_size);
STATIC_ASSERT(sizeof(DebrisEmitter) == 0x5C, DebrisEmitter_size);
STATIC_ASSERT(sizeof(EffectTemplate) == 0x1C, EffectTemplate_size);
STATIC_ASSERT(sizeof(EffectDesc) == 0x8, EffectDesc_size);
STATIC_ASSERT(sizeof(BloodDripDesc) == 0x30, BloodDripDesc_size);
STATIC_ASSERT(sizeof(GlowFlicker) == 0x1C, GlowFlicker_size);
STATIC_ASSERT(sizeof(GlowPulse) == 0x10, GlowPulse_size);
STATIC_ASSERT(sizeof(GlowEnvelope) == 0x24, GlowEnvelope_size);
STATIC_ASSERT(sizeof(GlowOrbit) == 0x18, GlowOrbit_size);
STATIC_ASSERT(sizeof(GlowShape) == 0x5C, GlowShape_size);
STATIC_ASSERT(sizeof(GlowParams) == 0x84, GlowParams_size);
STATIC_ASSERT(sizeof(EmitterAttach) == 0x20, EmitterAttach_size);
STATIC_ASSERT(sizeof(ScreenSpriteInfo) == 0x58, ScreenSpriteInfo_size);
STATIC_ASSERT(sizeof(BillboardTemplate) == 0x38, BillboardTemplate_size);
STATIC_ASSERT(sizeof(BillboardParticle) == 0x50, BillboardParticle_size);
STATIC_ASSERT(sizeof(AngleRange) == 0x8, AngleRange_size);
STATIC_ASSERT(sizeof(TrailBurstParams) == 0x3C, TrailBurstParams_size);
STATIC_ASSERT(sizeof(TrailBurst) == 0x74, TrailBurst_size);
STATIC_ASSERT(sizeof(SplashSpray) == 0x44, SplashSpray_size);
STATIC_ASSERT(sizeof(SplashSpraySpec) == 0x4C, SplashSpraySpec_size);
STATIC_ASSERT(sizeof(DropSpray) == 0x40, DropSpray_size);
STATIC_ASSERT(sizeof(DropSpraySpec) == 0x48, DropSpraySpec_size);
STATIC_ASSERT(sizeof(DebrisBurst) == 0x3C, DebrisBurst_size);
STATIC_ASSERT(sizeof(ConeBasis) == 0x2C, ConeBasis_size);
STATIC_ASSERT(sizeof(TriangleSource) == 0x24, TriangleSource_size);
STATIC_ASSERT(sizeof(JetSpray) == 0x54, JetSpray_size);
STATIC_ASSERT(sizeof(JetSpraySpec) == 0x5C, JetSpraySpec_size);
STATIC_ASSERT(sizeof(DirectedJetSpray) == 0x64, DirectedJetSpray_size);
STATIC_ASSERT(sizeof(SurfaceJetSpray) == 0x78, SurfaceJetSpray_size);
STATIC_ASSERT(sizeof(TrailRender) == 0x20, TrailRender_size);
STATIC_ASSERT(sizeof(PuffSpray) == 0x88, PuffSpray_size);
STATIC_ASSERT(sizeof(SurfacePuffSpray) == 0xA4, SurfacePuffSpray_size);
STATIC_ASSERT(sizeof(SpriteBurst) == 0x6C, SpriteBurst_size);
STATIC_ASSERT(sizeof(SimplePuffSpray) == 0x58, SimplePuffSpray_size);
STATIC_ASSERT(sizeof(DebrisSpray) == 0x44, DebrisSpray_size);
STATIC_ASSERT(sizeof(DecalSpray) == 0x30, DecalSpray_size);
STATIC_ASSERT(sizeof(PieceSpray) == 0x44, PieceSpray_size);
STATIC_ASSERT(sizeof(SurfaceDebrisBurst) == 0x58, SurfaceDebrisBurst_size);
STATIC_ASSERT(sizeof(StreakBurst) == 0x48, StreakBurst_size);
STATIC_ASSERT(sizeof(EffectParams) == 0x58, EffectParams_size);
STATIC_ASSERT(sizeof(BillboardBurst) == 0x40, BillboardBurst_size);
STATIC_ASSERT(sizeof(EffectMotion) == 0x34, EffectMotion_size);
STATIC_ASSERT(sizeof(EffectExtra) == 0x38, EffectExtra_size);
STATIC_ASSERT(offsetof(ModelEffect, extra) == 0x120, ModelEffect_extra); /* then the optional extension */
STATIC_ASSERT(sizeof(BloodStripDesc) == 0x3C, BloodStripDesc_size);
STATIC_ASSERT(sizeof(BloodStrip) == 0x7C, BloodStrip_size);
STATIC_ASSERT(sizeof(ParticleBurst) == 0x3C, ParticleBurst_size);
STATIC_ASSERT(sizeof(SurfaceBurst) == 0x74, SurfaceBurst_size);
STATIC_ASSERT(sizeof(DriftSpawn) == 0x38, DriftSpawn_size);
STATIC_ASSERT(sizeof(DriftBurst) == 0x4C, DriftBurst_size);
STATIC_ASSERT(sizeof(DebrisScatter) == 0x64, DebrisScatter_size);
STATIC_ASSERT(sizeof(SpriteDef) == 0x28, SpriteDef_size);
STATIC_ASSERT(sizeof(SpriteDef2C) == 0x2C, SpriteDef2C_size);
STATIC_ASSERT(sizeof(StreakParticleDesc) == 0x3C, StreakParticleDesc_size);
STATIC_ASSERT(sizeof(BoltChainConfig) == 0x30, BoltChainConfig_size);
STATIC_ASSERT(sizeof(BoltDesc) == 0x50, BoltDesc_size);
STATIC_ASSERT(sizeof(AnchorEmitterTemplate) == 0x50, AnchorEmitterTemplate_size);
STATIC_ASSERT(sizeof(AnchorEmitterParams) == 0x5C, AnchorEmitterParams_size);
STATIC_ASSERT(sizeof(ActorFollowerGroup) == 0x30, ActorFollowerGroup_size);
STATIC_ASSERT(sizeof(AnchorEmitter) == 0x8C, AnchorEmitter_size);
STATIC_ASSERT(sizeof(TrailMotion) == 0x20, TrailMotion_size);
STATIC_ASSERT(sizeof(ScatterPreset) == 0x24, ScatterPreset_size);
STATIC_ASSERT(sizeof(TrailParams) == 0x48, TrailParams_size);
STATIC_ASSERT(sizeof(BloodTrailDesc) == 0x1C, BloodTrailDesc_size);
STATIC_ASSERT(sizeof(BloodTrail) == 0x40, BloodTrail_size);
STATIC_ASSERT(sizeof(ParticleEmitterDesc) == 0x48, ParticleEmitterDesc_size);
STATIC_ASSERT(sizeof(TrailStateParams) == 0x40, TrailStateParams_size);
STATIC_ASSERT(sizeof(RadialEffectDef) == 0x30, RadialEffectDef_size);
STATIC_ASSERT(sizeof(EffectActorTemplate) == 0x44, EffectActorTemplate_size);
STATIC_ASSERT(sizeof(TumbleMotion) == 0x30, TumbleMotion_size);
STATIC_ASSERT(sizeof(EffectFade) == 0x8, EffectFade_size);
STATIC_ASSERT(offsetof(EffectActor, data) == 0xF8, EffectActor_data); /* then the type-specific data */
STATIC_ASSERT(sizeof(ModelBurst) == 0x78, ModelBurst_size);
STATIC_ASSERT(sizeof(DefParticleHead) == 0x110, DefParticleHead_size);
STATIC_ASSERT(sizeof(BurstData) == 0x18, BurstData_size);
STATIC_ASSERT(sizeof(BurstEffect) == 0x40, BurstEffect_size);
STATIC_ASSERT(sizeof(BeamScale) == 0x8, BeamScale_size);
STATIC_ASSERT(sizeof(BurstSparkData) == 0x5C, BurstSparkData_size);
STATIC_ASSERT(sizeof(BurstGlowData) == 0x18, BurstGlowData_size);
STATIC_ASSERT(sizeof(DescParticleHead) == 0xA8, DescParticleHead_size);
STATIC_ASSERT(sizeof(SparkParticle) == 0xAC, SparkParticle_size);
STATIC_ASSERT(sizeof(BillboardSpawn) == 0x34, BillboardSpawn_size);
STATIC_ASSERT(sizeof(QuadSprite) == 0xB0, QuadSprite_size);
STATIC_ASSERT(sizeof(EffectObjHead) == 0x60, EffectObjHead_size);
STATIC_ASSERT(sizeof(EffectObj) == 0xA0, EffectObj_size);
STATIC_ASSERT(sizeof(TrackerTrailData) == 0x20, TrackerTrailData_size);
STATIC_ASSERT(sizeof(WaveTrailData) == 0x28, WaveTrailData_size);
STATIC_ASSERT(sizeof(WaveTrailPoint) == 0x28, WaveTrailPoint_size);
STATIC_ASSERT(sizeof(ParticleStream) == 0x38, ParticleStream_size);
STATIC_ASSERT(sizeof(ProjectileStream) == 0x44, ProjectileStream_size);
STATIC_ASSERT(sizeof(StreamEmitter) == 0x38, StreamEmitter_size);
STATIC_ASSERT(sizeof(SprayDesc) == 0x50, SprayDesc_size);
STATIC_ASSERT(sizeof(Sprite2DData) == 0xC, Sprite2DData_size);
STATIC_ASSERT(sizeof(Sprite2DParams) == 0x3C, Sprite2DParams_size);
STATIC_ASSERT(sizeof(Sprite2D) == 0x4C, Sprite2D_size);
STATIC_ASSERT(sizeof(RumbleParams) == 0x8, RumbleParams_size);
STATIC_ASSERT(sizeof(WobbleData) == 0x10, WobbleData_size);
STATIC_ASSERT(sizeof(ParticlePulse) == 0x10, ParticlePulse_size);
STATIC_ASSERT(sizeof(RiseMotion) == 0x28, RiseMotion_size);
STATIC_ASSERT(sizeof(RiseParticle) == 0xD0, RiseParticle_size);
STATIC_ASSERT(sizeof(ParticleJitter) == 0x50, ParticleJitter_size);
STATIC_ASSERT(sizeof(JitterRelaunch) == 0x8, JitterRelaunch_size);
STATIC_ASSERT(sizeof(ParticleChannels) == 0x50, ParticleChannels_size);
STATIC_ASSERT(sizeof(AgeFadeParams) == 0xC, AgeFadeParams_size);
STATIC_ASSERT(sizeof(ParticleBounce) == 0x8, ParticleBounce_size);
STATIC_ASSERT(sizeof(ParticleFlash) == 0x14, ParticleFlash_size);
STATIC_ASSERT(sizeof(ActorEmitterParams) == 0xC, ActorEmitterParams_size);
STATIC_ASSERT(sizeof(ActorEmitterParticleData) == 0x58, ActorEmitterParticleData_size);
STATIC_ASSERT(sizeof(ActorEmitterSlot) == 0x18, ActorEmitterSlot_size);
STATIC_ASSERT(sizeof(CharacterWatcher) == 0x58, CharacterWatcher_size);
STATIC_ASSERT(sizeof(SpriteFlicker) == 0x20, SpriteFlicker_size);
STATIC_ASSERT(sizeof(FlickerSprite) == 0xD0, FlickerSprite_size);
STATIC_ASSERT(sizeof(WatcherEmitterLink) == 0xC, WatcherEmitterLink_size);
STATIC_ASSERT(sizeof(WatcherEmitter) == 0x64, WatcherEmitter_size);
STATIC_ASSERT(sizeof(DynModelDesc) == 0x3C, DynModelDesc_size);
STATIC_ASSERT(sizeof(MotionTrailPoint) == 0x14, MotionTrailPoint_size);
STATIC_ASSERT(sizeof(MotionTrail) == 0x9C, MotionTrail_size);
STATIC_ASSERT(sizeof(CamWobbleDef) == 0x38, CamWobbleDef_size);
STATIC_ASSERT(sizeof(EmitterDef) == 0x28, EmitterDef_size);
STATIC_ASSERT(sizeof(EmitterState) == 0x58, EmitterState_size);
STATIC_ASSERT(sizeof(ActorLink) == 0xC, ActorLink_size);
STATIC_ASSERT(sizeof(BoltSource) == 0xC, BoltSource_size);
STATIC_ASSERT(sizeof(FlashBurstEffect) == 0x30, FlashBurstEffect_size);
STATIC_ASSERT(sizeof(OwnerEffectData) == 0x8, OwnerEffectData_size);
STATIC_ASSERT(sizeof(OwnerEffect) == 0x30, OwnerEffect_size);
STATIC_ASSERT(sizeof(DDB60EffectData) == 0xC, DDB60EffectData_size);
STATIC_ASSERT(sizeof(TextLines) == 0x28, TextLines_size);
STATIC_ASSERT(sizeof(DiscEmitter) == 0x10, DiscEmitter_size);
STATIC_ASSERT(sizeof(Struct150C29F0) == 0x38, Struct150C29F0_size);
STATIC_ASSERT(sizeof(QuadFxData) == 0xA8, QuadFxData_size);
STATIC_ASSERT(sizeof(QuadFx) == 0xB8, QuadFx_size);
STATIC_ASSERT(sizeof(RenderModePair) == 0x8, RenderModePair_size);
STATIC_ASSERT(sizeof(Pulse) == 0x8, Pulse_size);

/* conker/beam.h */
STATIC_ASSERT(sizeof(BoltDef) == 0x50, BoltDef_size);
STATIC_ASSERT(sizeof(BoltLink) == 0x28, BoltLink_size);
STATIC_ASSERT(sizeof(StripDesc) == 0x24, StripDesc_size);
STATIC_ASSERT(sizeof(AimedBeamState) == 0x44, AimedBeamState_size);
STATIC_ASSERT(sizeof(SparkLink) == 0x10, SparkLink_size);
STATIC_ASSERT(sizeof(EmitterLink) == 0x8, EmitterLink_size);
STATIC_ASSERT(sizeof(ActorTrailLink) == 0x18, ActorTrailLink_size);
STATIC_ASSERT(sizeof(BoltSample) == 0x34, BoltSample_size);
STATIC_ASSERT(sizeof(BoltObj) == 0x88, BoltObj_size);
STATIC_ASSERT(sizeof(BoltChain) == 0x44, BoltChain_size);
STATIC_ASSERT(sizeof(ParticleSettings) == 0x2C, ParticleSettings_size);
STATIC_ASSERT(sizeof(WavyBeamData) == 0x5A8, WavyBeamData_size);

/* conker/camera.h */
STATIC_ASSERT(sizeof(CamWobble) == 0x68, CamWobble_size);
STATIC_ASSERT(sizeof(CamWobbleEnvelope) == 0xA0, CamWobbleEnvelope_size);
STATIC_ASSERT(sizeof(CamWobbleFalloff) == 0x18, CamWobbleFalloff_size);

/* conker/cutscene.h */
STATIC_ASSERT(sizeof(EvTimer) == 0x2C, EvTimer_size);

/* conker/effect.h */
STATIC_ASSERT(sizeof(TextureFrameTables) == 0x8, TextureFrameTables_size);
STATIC_ASSERT(sizeof(FxImpactSet) == 0x5, FxImpactSet_size);
STATIC_ASSERT(sizeof(EmitterConfig) == 0x2C, EmitterConfig_size);

/* conker/face.h */
STATIC_ASSERT(sizeof(ExpressionRecord) == 0xA, ExpressionRecord_size);

/* conker/level.h */
STATIC_ASSERT(sizeof(ObjectSpawn) == 0x44, ObjectSpawn_size);

/* conker/libc.h */
STATIC_ASSERT(sizeof(_Pft) == 0x38, _Pft_size);

/* conker/light.h */
STATIC_ASSERT(sizeof(LampSwing) == 0x24, LampSwing_size);

/* conker/particle.h */
STATIC_ASSERT(sizeof(Rgb8) == 0x3, Rgb8_size);
STATIC_ASSERT(sizeof(EffectE7970Data) == 0xC, EffectE7970Data_size);
STATIC_ASSERT(sizeof(PoolEmitterDesc) == 0x1C, PoolEmitterDesc_size);
STATIC_ASSERT(sizeof(PoolEmitter) == 0x48, PoolEmitter_size);
STATIC_ASSERT(sizeof(QuadDriftMotion) == 0x40, QuadDriftMotion_size);
STATIC_ASSERT(sizeof(RingSparkEmitter) == 0x60, RingSparkEmitter_size);
STATIC_ASSERT(sizeof(StreakImpact) == 0x30, StreakImpact_size);
STATIC_ASSERT(sizeof(Game113D60Emission) == 0x44, Game113D60Emission_size);
STATIC_ASSERT(sizeof(TwoSidedEmitterState) == 0x20, TwoSidedEmitterState_size);
STATIC_ASSERT(sizeof(EffectParticleState) == 0x94, EffectParticleState_size);
STATIC_ASSERT(sizeof(ObjectEmitterData) == 0x14, ObjectEmitterData_size);
STATIC_ASSERT(sizeof(MatrixEmitterData) == 0x10, MatrixEmitterData_size);
STATIC_ASSERT(sizeof(PairEmitterData) == 0x14, PairEmitterData_size);
STATIC_ASSERT(sizeof(SpinDebrisParticle) == 0xB8, SpinDebrisParticle_size);
STATIC_ASSERT(sizeof(ParticleTemplate) == 0x40, ParticleTemplate_size);
STATIC_ASSERT(sizeof(EmitterPoint) == 0x10, EmitterPoint_size);
STATIC_ASSERT(sizeof(DripMotion) == 0x60, DripMotion_size);
STATIC_ASSERT(sizeof(DripEmitter) == 0x300, DripEmitter_size);
STATIC_ASSERT(sizeof(DropKind) == 0x14, DropKind_size);
STATIC_ASSERT(sizeof(BoneMount) == 0x10, BoneMount_size);
STATIC_ASSERT(sizeof(EmitterKind) == 0x18, EmitterKind_size);
STATIC_ASSERT(sizeof(DustData) == 0x28, DustData_size);
STATIC_ASSERT(sizeof(DustParticle) == 0xD0, DustParticle_size);
STATIC_ASSERT(sizeof(DustEmitterData) == 0x68, DustEmitterData_size);
STATIC_ASSERT(sizeof(FixedEmitterData) == 0xC, FixedEmitterData_size);
STATIC_ASSERT(sizeof(PointSparkEmitter) == 0x14, PointSparkEmitter_size);
STATIC_ASSERT(sizeof(EffectSample) == 0x14, EffectSample_size);
STATIC_ASSERT(sizeof(SteamWobble) == 0x1C, SteamWobble_size);
STATIC_ASSERT(sizeof(ParticleSpin) == 0x1C, ParticleSpin_size);
STATIC_ASSERT(sizeof(SteamData) == 0x1C, SteamData_size);
STATIC_ASSERT(sizeof(DripData) == 0x18, DripData_size);
STATIC_ASSERT(sizeof(SplashBurst) == 0xC, SplashBurst_size);
STATIC_ASSERT(sizeof(SplashNode) == 0x14, SplashNode_size);
STATIC_ASSERT(sizeof(SquashData) == 0xC, SquashData_size);
STATIC_ASSERT(sizeof(ColorEmitterData) == 0x14, ColorEmitterData_size);

/* conker/rope.h */
STATIC_ASSERT(sizeof(HoltenRopeDesc) == 0x38, HoltenRopeDesc_size);

/* conker/screen.h */
STATIC_ASSERT(sizeof(MosaicData) == 0x8F0, MosaicData_size);
STATIC_ASSERT(sizeof(StrobeData) == 0x18, StrobeData_size);
STATIC_ASSERT(sizeof(struct ScreenTint) == 0x20, ScreenTint_size);

/* conker/shadow.h */
STATIC_ASSERT(sizeof(ClipVtx) == 0x14, ClipVtx_size);
STATIC_ASSERT(sizeof(ShadowVtx) == 0x14, ShadowVtx_size);

/* conker/sprite.h */
STATIC_ASSERT(sizeof(Game1A89B0Position) == 0xE, Game1A89B0Position_size);

/* conker/swarm.h */
STATIC_ASSERT(sizeof(SwarmFlight) == 0x38, SwarmFlight_size);

/* conker/trail.h */
STATIC_ASSERT(sizeof(CharacterTrailState) == 0x1C, CharacterTrailState_size);
STATIC_ASSERT(sizeof(StreamPoint) == 0x18, StreamPoint_size);
STATIC_ASSERT(sizeof(StreamState) == 0x11C, StreamState_size);
STATIC_ASSERT(sizeof(FallingTrailPoint) == 0x10, FallingTrailPoint_size);
STATIC_ASSERT(sizeof(LevelEffectData) == 0xC, LevelEffectData_size);
STATIC_ASSERT(sizeof(SprayEmitter) == 0x78, SprayEmitter_size);
STATIC_ASSERT(sizeof(TrailEmitterParams) == 0x1C, TrailEmitterParams_size);
STATIC_ASSERT(sizeof(TrailEmitter) == 0x44, TrailEmitter_size);
STATIC_ASSERT(sizeof(SurfaceTrailPoint) == 0x24, SurfaceTrailPoint_size);

/* conker/weapon.h */
STATIC_ASSERT(sizeof(Game1EF500TrailExt) == 0xB0, Game1EF500TrailExt_size);
