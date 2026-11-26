#ifndef CONKER_LIGHT_H
#define CONKER_LIGHT_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "light": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_36680.c
 *   game_487D0.c
 *   game_FEFF0.c
 *   game_FF900.c
 *   game_135D00.c
 *   game_18A8F0.c
 *   game_18C660.c
 *   game_18D770.c
 *   game_1BD880.c
 *   game_1E6260.c
 */

typedef struct {
    TimedEffect base;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
} Obj150D1B40;

/* Per-channel colour wave: channel = base + amplitude * sin(phase). */
typedef struct {
    f32 baseR;
    f32 baseG;
    f32 baseB;
    f32 amplitudeR;
    f32 amplitudeG;
    f32 amplitudeB;
    f32 phaseR;
    f32 phaseG;
    f32 phaseB;
    f32 speedR; // phase advance per frame
    f32 speedG;
    f32 speedB;
} ColourWave;

/* A TimedEffect carrying a ColourWave. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ColourWave wave;
} ColourWaveEffect;

/* Payload of message 0x1D: follow a sequence track for a while. */
typedef struct {
    s32 track;
    s32 duration;
    u8 sceneTag;
} LampTrackMsg;

/* Event message about actors: event 0 removes `actor`; event 0x2D swaps actor and other. */
typedef struct {
    Actor *actor;
    union {
        Actor *other; // event 0x2D
        u8 actorId;       // event 0
    } u4;
    u8 actorId;           // event 0x2D: id of `actor`
    u8 otherId;           // event 0x2D: id of `other`
} ActorEvent;

/* Global colour cycle: a TimedEffect carrying a LightColourPulse. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ LightColourPulse pulse;
} ColourPulseEffect;

/* Event 0x33 message for func_1516434C. */
typedef struct {
    f32 level;
    u8 id;
    u8 r;
    u8 g;
    u8 b;
} LightFlashEvent;

/* Extra data of the swinging lamp particle: a sine swing whose amplitude and speed ramp at their rates
 * while `timer` runs (started by func_151B9660), then ease back toward their targets. */
typedef struct {
    /* 0x00 */ f32 out;         // current swing angle, used as the particle's roll
    /* 0x04 */ f32 amp;
    /* 0x08 */ f32 phase;
    /* 0x0C */ f32 speed;
    /* 0x10 */ f32 targetAmp;
    /* 0x14 */ f32 targetSpeed;
    /* 0x18 */ f32 timer;
    /* 0x1C */ f32 ampRate;
    /* 0x20 */ f32 speedRate;
} LampSwing; // size 0x24

/* The swinging lamp: a ModelParticle carrying a LampSwing as its data. */
typedef struct {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ LampSwing swing;
} LampParticle;

/* game_36680.c */
void func_150091D0(void);
void func_150092DC(void);
void func_15009334(s32 index);
void func_150095D8(PointLight *light, s32 id);
void func_15009600(PointLight *light, s32 id);
void func_15009628(PointLight *light, s32 id);
void func_1500969C(PointLight *light, s32 id);
void func_150096C4(PointLight *light, s32 id);
void func_15009740(PointLight *light, s32 id);
void func_15009768(PointLight *light, s32 id);
void func_150097A4(PointLight *light, s32 id);
void func_150097CC(PointLight *light, s32 id);
void func_15009818(PointLight *light, s32 id);
void func_15009844(PointLight *light, s32 id);
void func_15009870(PointLight *light, s32 id);
void func_15009894(PointLight *light, s32 id);
void func_150098D0(PointLight *light, s32 id);
void func_150098F8(PointLight *light, s32 id);
void func_15009944(PointLight *light, s32 id);
void func_15009990(PointLight *light, s32 id);
void func_15009A38(PointLight *light, s32 id);
void func_15009AA0(PointLight *light, s32 id);
void func_15009AEC(PointLight *light, s32 id);
void func_15009B38(PointLight *light, s32 id);
void func_15009B84(PointLight *light, s32 id);
void func_15009BD0(PointLight *light, s32 id);
void func_15009C7C(PointLight *light, s32 id);
void func_15009D28(PointLight *light, s32 id);
void func_15009D6C(PointLight *light, s32 id);
void func_15009DB0(PointLight *light, s32 id);
void func_15009DFC(PointLight *light, s32 id);
void func_15009E48(PointLight *light, s32 id);
void func_15009E84(PointLight *light, s32 id);
void func_15009EC8(PointLight *light, s32 id);
void func_15009EF4(PointLight *light, s32 id);
void func_15009F30(PointLight *light, s32 id);
void func_15009F74(PointLight *light, s32 id);
void func_1500A028(PointLight *light, s32 id);
void func_1500A06C(PointLight *light, s32 id);
void func_1500A0B0(PointLight *light, s32 id);
void func_1500A0FC(PointLight *light, s32 id);
void func_1500A148(PointLight *light, s32 id);
void func_1500A194(PointLight *light, s32 id);
void func_1500A1E0(PointLight *light, s32 id);
void func_1500A21C(PointLight *light, s32 id);
void func_1500A260(PointLight *light, s32 id);
void func_1500A2AC(PointLight *light, s32 id);
void func_1500A2F4(PointLight *light, s32 id);
void func_1500A33C(PointLight *light, s32 id);
void func_1500A380(PointLight *light, s32 id);
void func_1500A3C4(PointLight *light, s32 id);
void func_1500A410(PointLight *light, s32 id);
void func_1500A454(PointLight *light, s32 id);
void func_1500A490(PointLight *light, s32 id);
void func_1500A4D4(PointLight *light, s32 id);
void func_1500A518(PointLight *light, s32 id);
void func_1500A55C(PointLight *light, s32 id);
void func_1500A5A8(PointLight *light, s32 id);
void func_1500A5F4(PointLight *light, s32 id);
void func_1500A640(PointLight *light, s32 id);
void func_1500A68C(PointLight *light, s32 id);
void func_1500A6D8(PointLight *light, s32 id);
void func_1500A79C(PointLight *light, s32 id);
void func_1500A7E8(PointLight *light, s32 id);
void func_1500A8C8(PointLight *light, s32 id);
void func_1500A904(PointLight *light, s32 id);
void func_1500A94C(PointLight *light, s32 id);
void func_1500A990(s32 unused);
void func_1500AB5C(s32 action);
void func_1500ABA0(s32 arg0);
void func_1500AC14(void);
void func_1500AD84(s32 slot, s32 id, s32 flags);
s32 func_1500AF08(s32 index, s32 slot, s32 subtype, s16 x, s16 y, s16 z, s32 jitter, u8 kind, s32 parity);
s32 func_1500B1F4(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri);
s32 func_1500B3B0(s16 pattern, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri);
s32 func_1500B714(s16 amount, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri);
void func_1500B8F4(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri);
void func_1500BAB8(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri);
void func_1500BC7C(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri);
void func_1500BE40(s32 unused);
void func_1500BE68(void);
void func_1500BEC0(void);

/* game_487D0.c */
void func_1501B320(s32 player);
void func_1501B640(s32 player);

/* game_FEFF0.c */
void func_150D1B40(Obj150D1B40 *arg0);
void func_150D1BD0(s32 *arg0);

/* game_FF900.c */
void func_150D2450(ColourWaveEffect *object);

/* game_135D00.c */
void func_15108850(PointLight *light);
s32 func_15108AB4(DynModel *model);
void func_15108B80(DynModel *model);
void func_15108BC0(DynModel *model);
s32 func_15108C38(DynModel *model);
s32 func_15108D24(DynModel *model, s32 viewport);
s32 func_15108E10(LightEffect *effect);
void func_15108FFC(s32 track, s32 duration, u8 sceneTag);
void func_15109064(DynModel *model, LampTrackMsg *msg, u8 event);
void func_151090DC(void);
s32 func_15109120(DynModel *model, s16 viewport);

/* game_18A8F0.c */
void *func_1515D440(void);
void *func_1515D480(s32 count);
void func_1515D4D4(s32 r, s32 g, s32 b, u8 priority);
PointLight *func_1515D520(void);
PointLight *func_1515D5AC(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, u8 group);
PointLight *func_1515D5F8(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, u8 group); /* func_1515F10C frees it */
void func_1515D69C(void);
void func_1515D6C8(void);
Gfx *func_1515D6D0(Gfx *gfx, s32 player);
Gfx *func_1515D914(Gfx *gfx, s32 player, s32 x, s32 y, s32 z, u32 arg5, PointLightGfx *lightBuf, s32 maxLights,
                   Ambient *ambient, s8 *outCount, s32 mask, u8 *smoothColor, s32 flags, PointLight **outLights);
void func_1515E278(s32 x, s32 y, s32 z, s32 mask, u8 *color, u32 arg5, s32 flags);
void func_1515E43C(s32 x, s32 y, s32 z, s32 arg3, u8 *colorA, u8 *colorB, u8 *amountA, u8 *amountB);
Gfx *func_1515E544(Gfx *gfx, PointLightGfx *lightBuf, u8 maxLights, u8 countByte, Ambient *ambient);
void func_1515E888(s32 player, s32 x, s32 y, s32 z, s32 mask, u8 *color, u32 arg6, s32 flags);
TriggerZone *func_1515EB84(s32 x, s32 y, s32 z, s32 mask, ZoneLink *node);
void func_1515EC78(PointLight *light, s32 player, PointLightGfx *out, s32 x, s32 y, s32 z, s32 flags);
void func_1515EF74(s16 *mtx);
f32 func_1515F008(s16 *mtx, s32 i);
void func_1515F040(f32 value, s32 i);
void func_1515F0AC(f32 value, s32 i);
/* (PointLight *light); declared without a prototype: game_18D770.c passes a second argument */
void func_1515F10C();
void func_1515F170(s32 group, u8 disabled);

/* game_18C660.c */
RegionNode *func_1515F1B0(TriggerZone *region);
void func_1515F25C(RegionNode **list, RegionNode *node);
void func_1515F270(Actor *obj, TriggerZone *zone);
void func_1515F2B8(Actor *obj, TriggerZone *zone);
void func_1515F2E8(Actor *obj, TriggerZone *zone);
void func_1515F338(Actor *obj, TriggerZone *zone);
void func_1515F5C4(Actor *obj, TriggerZone *zone);
void func_1515F850(Actor *obj, TriggerZone *zone);
void func_1515FB70(Actor *obj, TriggerZone *zone);
void func_1515FB94(Actor *obj, TriggerZone *zone);
void func_1515FBC4(Actor *obj, TriggerZone *zone);
void func_1515FC34(Actor *obj, TriggerZone *zone);
void func_1515FC60(Actor *obj, TriggerZone *zone);

/* game_18D770.c */
LightEffect *func_151602C0(LightEffectHeader *header, Vec3i *position, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                           u8 arg6, u8 arg7, s32 size, u8 arg9, s32 argA);
LightEffect *func_1516037C(LightEffectHeader *header, PointLight *light, s32 size, u8 arg3, s32 arg4);
void func_151603FC(LightEffect *effect);
void func_151604A0(void *arg0, u8 *message, u8 event);
s32 func_15160600(LightEffect *effect);
s32 func_1516065C(LightEffect *effect);
s32 func_15160684(LightEffect *effect);
s32 func_151606A8(LightEffect *effect);
s32 func_151607A4(LightEffect *effect);
void func_15160954(f32 *intensity, f32 *peak, f32 *rate, f32 *delay, LightEffect *effect);
LightEffect *func_15160A58(Actor *actor, u8 bone, Vec3f *offset, u8 arg3, s16 lifetime, s32 arg5, s32 arg6, s32 arg7,
                           s32 arg8, s32 arg9, s32 argA, s8 callback, s32 extraSize, u8 eventHandler, u8 argE,
                           s32 argF);
u8 func_15160B74(LightEffect *effect);
LightEffect *func_15160CDC(Actor *actor, u8 bone, Vec3f *start, Vec3f *end, f32 scale, u8 arg5, s16 lifetime,
                           s32 arg7, s32 arg8, s32 arg9, s32 argA, u8 fromBone, u8 eventHandler, u8 flagHit, u8 argE,
                           s32 argF);
s32 func_15160E30(LightEffect *effect);
s32 func_15161238(Actor *actor, Actor *exclude);
LightEffect *func_1516127C(PointLight *light, u8 arg1, s32 arg2);
LightEffect *func_15161334(PointLight *light, u8 arg1, s32 arg2);
LightEffect *func_15161408(PointLight *light, u8 arg1, s32 arg2);
LightEffect *func_15161494(PointLight *light, u8 arg1, s32 arg2);
LightEffect *func_15161540(PointLight *light, u8 arg1, s32 arg2);
LightEffect *func_151615F8(PointLight *light, u8 sound, u8 kind, u8 type, u8 arg4, s32 arg5);
void light_sendEvent(u8 lightType, u8 event, s32 arg2);
void func_15161714(LightEffect *effect);
void func_15161740(LightEffect *effect);
void func_1516176C(LightEffect *effect);
void func_15161798(LightEffect *effect);
void func_151617C4(LightEffect *effect);
void func_151617E4(LightEffect *effect);
void func_15161804(LightEffect *effect);
void func_15161860(LightEffect *effect);
void func_151618BC(u16 arg0, s16 arg1, u8 arg2, s32 arg3, Vec3f *position, s16 arg5, s16 arg6);
LightEffect *func_151619A0(s32 index, s16 lifetime, u8 arg2, s32 arg3);
s32 func_15161A68(LightEffect *effect);
LightEffect *func_15161E24(Actor *actor, u8 eventHandler, u8 arg2, s16 lifetime, s32 arg4, s32 arg5, s32 arg6,
                           s32 arg7, u8 arg8, s32 arg9);
s32 func_15161F2C(LightEffect *effect);
void func_15161F4C(LightEffect *effect, ActorEvent *event, u8 eventId);
LightEffect *func_15162034(PointLight *light, u8 arg1, s32 arg2);
void func_15162110(s32 arg0);
void func_151621B8(ColourPulseEffect *object);
LightEffect *func_151623F4(PointLight *light, u8 kind, u8 type, u8 arg3, s8 update, s16 lifetime, u8 arg6, s32 arg7);
s32 func_15162510(LightEffect *effect);
LightEffect *func_15162740(PointLight *light, u8 kind, u8 type, u8 arg3, s16 lifetime, s8 update, u8 arg6, s32 arg7);
LightEffect *func_1516284C(LightEffectHeader *header, Vec3i *position, s32 arg2, s32 arg3, s32 arg4, u8 arg5,
                           u8 arg6, s32 extraSize, u8 kind, u8 arg9, s32 argA);
LightEffect *func_1516295C(PointLight *light, u8 kind, u8 type, u8 arg3, s16 lifetime, s8 update, s8 onLow,
                           s8 onRise, s8 onHigh, s8 onFall, s32 extraSize, u8 argB, s32 argC);
s32 func_15162B28(LightEffect *effect);
void func_15162EF8(LightEffect *effect);
void func_15162F50(LightEffect *effect);
LightEffect *func_15162FAC(PointLight *light, u8 kind, u8 type, u8 arg3, s16 lifetime, s8 update, u8 arg6, s32 arg7);
s32 func_151630F4(LightEffect *effect);
LightEffect *func_15163414(LightEffectHeader *header, f32 *x, f32 *y, f32 *z, s8 callback, u8 handler, u8 arg6,
                           u8 arg7, u8 arg8, u8 arg9, u8 argA, u8 argB, s32 extraSize, u8 argD, s32 argE);
s32 func_15163504(LightEffect *effect);
void func_151635A8(LightEffect *effect, s32 arg1, u8 eventId);
LightEffect *func_15163604(PointLight *light, u8 kind, u8 type, s16 lifetime, u8 arg4, s32 arg5, u8 arg6, s32 arg7);
s32 func_15163704(LightEffect *effect);
void func_1516381C(PointLight *light, u8 type, u8 arg2, s32 arg3);
LightEffect *func_1516387C(PointLight *light, u8 flags, s8 update, s16 lifetime, u8 type, s32 size, u8 arg6, s32 arg7);
s32 func_151638E0(LightEffect *effect);
void func_151639D0(LightEffect *effect, s32 arg1, u8 eventId);
void func_15163A18(LightEffect *effect, s32 arg1, u8 eventId);
LightEffect *func_15163A60(u8 objectId, u8 arg1, s32 arg2);
s32 func_15163B98(LightEffect *effect);
LightEffect *func_15163BE8(Actor *actor, u8 arg1, s32 arg2);
s32 func_15163CD0(LightEffect *effect);
void func_15163CF8(LightPulse *pulse, LightEffect *effect);
void func_15163DEC(LightEffect *effect, LightJitter *jitter);
s32 func_15163F50(LightEffect *effect, LightActorRef *ref);
void func_15163FEC(LightEffect *effect, ActorEvent *event, u8 eventId);
void func_151640C0(LightEffect *effect, ActorEvent *event, u8 eventId);
void func_15164134(LightEffect *effect, ActorEvent *event, u8 eventId);
LightEffect *func_15164208(PointLight *light, u8 id, u8 arg2, s32 arg3);
s32 func_1516429C(LightEffect *effect);
void func_1516434C(LightEffect *effect, LightFlashEvent *event, u8 eventId);
void func_151643A8(LightEffect *effect, s32 arg1, u8 eventId);
s32 func_151643F8(LightEffect *effect);
void func_1516441C(LightEffect *effect, LightTransformed *point);
s32 func_151644A8(LightEffect *effect);
void func_151644F4(LightEffect *effect, Vec3f *pivot, f32 offset, f32 angleX, f32 angleZ);
void func_151645C4(u8 enable);
extern const f32 D_800A6760[24];
extern const f32 D_800A67C0[24];
extern const f32 D_800A6820[24];

/* game_1BD880.c */
void func_151903D0(LightEffect *effect);

/* game_1E6260.c */
void func_151B8DB0(s32 light, u8 kind, u8 arg2, s32 arg3);
s32 func_151B9214(LampParticle *particle);
s32 func_151B9310(Mtx *dest, LampParticle *particle);
s32 func_151B9408(LightEffect *effect);
void func_151B9660(LampParticle *particle);

#endif
