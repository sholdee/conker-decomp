#ifndef CONKER_CAMERA_H
#define CONKER_CAMERA_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "camera": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_32740.c
 *   game/done/game_32F10.c
 *   game/done/game_32F60.c
 *   game/done/game_32FB0.c
 *   game/done/game_32FC0.c
 *   game/done/game_33000.c
 *   game/done/game_33010.c
 *   game/done/game_33020.c
 *   game/done/game_33080.c
 *   game/done/game_330E0.c
 *   game/done/game_33130.c
 *   game/done/game_331B0.c
 *   game/done/game_33210.c
 *   game/done/game_33260.c
 *   game/done/game_332E0.c
 *   game/done/game_33320.c
 *   game/done/game_33390.c
 *   game/done/game_333D0.c
 *   game/done/game_33410.c
 *   game/done/game_33450.c
 *   game/done/game_33460.c
 *   game/done/game_334C0.c
 *   game/done/game_33620.c
 *   game_E3C90.c
 *   game_E8BB0.c
 *   game_EBD00.c
 *   game/done/game_F20A0.c
 *   game_F4800.c
 *   game_F4C20.c
 *   game_F52B0.c
 *   game/done/game_FC3C0.c
 *   game_FDB00.c
 *   game_FE9E0.c
 *   game_10CCD0.c
 *   game/done/game_10ECB0.c
 *   game_10ED10.c
 *   game_118670.c
 *   game/done/game_119960.c
 *   game_122760.c
 *   game_123FB0.c
 *   game_126ED0.c
 *   game/done/game_129EB0.c
 *   game_1308E0.c
 *   game_1319D0.c
 *   game_135490.c
 *   game_14D110.c
 *   game_14EE80.c
 *   game_14F0B0.c
 *   game_14F130.c
 *   game_14F580.c
 *   game_14F620.c
 *   game_14F8F0.c
 *   game_14FE30.c
 *   game_14FF90.c
 *   game_155C90.c
 *   game_156160.c
 *   game_156D70.c
 *   game_1570E0.c
 *   game_157840.c
 *   game_159940.c
 *   game/done/game_15A830.c
 *   game_15A840.c
 *   game_15AA10.c
 *   game_15ABA0.c
 *   game_15B200.c
 *   game_15B5F0.c
 *   game_15B960.c
 *   game_15BAA0.c
 *   game_15BD90.c
 *   game_191C30.c
 */

/* Defined in the C files of this subsystem. A typedef here can reorder the .bss of other files (see
 * init_50A0.c), a struct tag does not. */
struct CamSound;
struct ZScan;
struct CamWobble;
struct CamWobblePreset;
struct CamWobbleFalloff;
struct CamWobbleEnv;

/* game_32740.c */
void func_15005290(s32 resource, s32 mode);
void func_150054C4(Camera *view, s32 index, s16 resource, s32 mode);
void func_15005818(Camera *view, Actor *actor, Vec3f *eye);

/* game/done/game_32F10.c */
void func_15005A60(Camera *arg0, s32 arg1);

/* game/done/game_32F60.c */
void func_15005AB0(Camera *arg0);
void func_15005AF0(s32 arg0);

/* game/done/game_32FB0.c */
void func_15005B00(s32 arg0);

/* game/done/game_32FC0.c */
void func_15005B10(Camera *arg0);

/* game/done/game_33000.c */
void func_15005B50(s32 arg0);

/* game/done/game_33010.c */
void func_15005B60(s32 arg0);

/* game/done/game_33020.c */
void func_15005B70(s32 arg0);

/* game/done/game_33080.c */
void func_15005BD0(Camera *arg0);

/* game/done/game_330E0.c */
void func_15005C30(Camera *arg0);

/* game/done/game_33130.c */
void func_15005C80(Camera *arg0);
void func_15005CF0(s32 arg0);

/* game/done/game_331B0.c */
void func_15005D00(Camera *arg0);

/* game/done/game_33210.c */
void func_15005D60(Camera *arg0);
void func_15005DA0(s32 arg0);

/* game/done/game_33260.c */
void func_15005DB0(Camera *arg0);

/* game/done/game_332E0.c */
void func_15005E30(Camera *arg0);

/* game/done/game_33320.c */
void func_15005E70(Camera *arg0);
void func_15005EA0(Camera *arg0);

/* game/done/game_33390.c */
void func_15005EE0(Camera *arg0);

/* game/done/game_333D0.c */
void func_15005F20(Camera *arg0);

/* game/done/game_33410.c */
void func_15005F60(Camera *arg0);

/* game/done/game_33450.c */
void func_15005FA0(s32 arg0);

/* game/done/game_33460.c */
void func_15005FB0(Camera *arg0);

/* game/done/game_334C0.c */
void func_15006010(Camera *actor);
void func_15006140(Camera *actor);

/* game/done/game_33620.c */
void func_15006170(Camera *arg0);

/* game_E3C90.c */
void func_150B67E0(Camera *camera);

/* game_E8BB0.c */
void func_150BB700(s32 *arg0);

/* game_EBD00.c */
void func_150BE850(Camera *actor);

/* game/done/game_F20A0.c */
void func_150C4BF0(Camera *arg0);

/* game_F4800.c */
void func_150C7350(Camera *arg0);

/* game_F4C20.c */
void func_150C7770(Camera *arg0);

/* game_F52B0.c */
void func_150C7E00(Camera *camera);

/* game/done/game_FC3C0.c */
void func_150CEF10(Camera *arg0);

/* game_FDB00.c */
void func_150D0650(Camera *arg0);

/* game_FE9E0.c */
void func_150D1530(Camera *camera);

/* game_10CCD0.c */
void func_150DF820(Camera *arg0);

/* game/done/game_10ECB0.c */
void func_150E1800(Camera *arg0);

/* game_10ED10.c */
void func_150E1860(Camera *arg0);

/* game_118670.c */
void func_150EB1C0(Camera *actor);

/* game/done/game_119960.c */
void func_150EC4B0(Camera *arg0);

/* game_122760.c */
void func_150F52B0(s32 *arg0);
void func_150F5310(s32 arg0);

/* game_123FB0.c */
void func_150F6B00(Camera *actor);

/* game_126ED0.c */
void func_150F9A20(Camera *arg0);

/* game/done/game_129EB0.c */
void func_150FCA00(Camera *arg0);

/* game_1308E0.c */
void func_15103430(Camera *actor);

/* game_1319D0.c */
void func_15104520(Camera *camera);

/* game_135490.c */
void func_15107FE0(Camera *arg0);

/* game_14D110.c */
void func_1511FC60(Camera *camera);
void func_15120158(Camera *camera);
void func_15121490(Actor *player, OSContPad *pad);
void func_151216F8(Camera *camera);
void func_151218C4(Camera *camera);

/* game_14EE80.c */
void func_151219D0(Camera *camera);

/* game_14F0B0.c */
void func_15121C00(Camera *camera, f32 targetYaw, s32 unused, f32 speedGain, f32 accelGain);
void func_15121C64(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* game_14F130.c */
void func_15121C80(Camera *camera, f32 yawOffset);

/* game_14F580.c */
void func_151220D0(Camera *arg0);

/* game_14F620.c */
void func_15122170(Camera *arg0);

/* game_14F8F0.c */
void func_15122440(Camera *camera);

/* game_14FE30.c */
void func_15122980(Camera *camera);

/* game_14FF90.c */
void func_15122AE0(void);
void func_15122C5C(Camera *camera);
void func_15123070(Camera *camera);
void func_1512317C(Camera *camera);
void func_15123508(Camera *camera);
void func_15123568(Camera *camera);
void func_151236D0(Camera *camera);
s32 func_15123934(Camera *camera, s32 newMode, s32 newMotion, s32 newTightness, CameraMode slot);
s32 func_151239CC(Camera *camera, CameraMode slot);
void func_15123A54(Camera *camera);
void func_15124770(Camera *camera, s32 index);
void func_151247C0(Camera *camera);
void func_15124AB4(Camera *camera);
void func_15124B18(Camera *camera);
void func_15124C38(Camera *camera, f32 *outY);
void func_1512523C(Camera *camera);
void func_15125330(Camera *camera);
void func_15125394(Camera *camera);
s32 func_151253CC(Camera *camera);
s32 func_15125490(Camera *camera);
void func_151254F4(Camera *camera, s32 index);
void func_15125594(Camera *camera);
void func_15125608(Camera *camera);
void func_15125628(void);
void func_15125690(Camera *camera, s32 frames);
void func_151256BC(Camera *camera);
void func_15125924(Camera *camera);
void func_15125A6C(Camera *camera);
void func_15125C40(Camera *camera);
void func_15125DB4(Camera *camera);
void func_15126138(Camera *camera);
void func_1512623C(Actor *obj, ActorCtl *ext, s32 zone, f32 *outX, f32 *outY, f32 *outZ, s32 lowView);
s32 func_15126378(Camera *camera);
s32 func_15127520(struct CamSound *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *soundId);
s32 func_151277B0(struct CamSound *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *soundId);
s32 func_151279A0(Actor *obj, OSContPad *contPad);
void func_15127EB8(Camera *camera);
void func_15127FEC(Camera *camera, s32 arg1, s32 arg2);
s32 func_15128030(Camera *camera);
void func_151283B8(Camera *camera);
void func_151284C4(Camera *camera);
s32 func_15128540(Camera *camera);
void func_15128680(Camera *camera);
void func_1512868C(Camera *camera);
void func_15128774(Camera *camera, Actor *obj);
extern const f32 D_800A34B0[4][2];

/* game_155C90.c */
void func_151287E0(Camera *camera, f32 *outUnk5E8, f32 *outUnk3A8);

/* game_156160.c */
void func_15128CB0(Camera *cam);

/* game_156D70.c */
void func_151298C0(Camera *cam, s32 arg1);
void func_15129934(Camera *cam);

/* game_1570E0.c */
void func_15129C30(Camera *cam);
void func_1512A360(Camera *cam);

/* game_157840.c */
void func_1512A390(Camera *arg0);
void func_1512ABF8(void);
void func_1512AD54(Camera *arg0);
void func_1512B100(Camera *arg0);
void func_1512B1B8(Camera *arg0, s32 screenX, s32 screenY, s32 depth);
s32 func_1512B53C(Camera *arg0);
void func_1512B5FC(struct ZScan *scan, s32 offset, s32 gap);
s32 func_1512B630(Camera *arg0, struct ZScan *scan, s32 gap, s32 offset, s32 mid, s32 useAltThreshold);
s32 func_1512B730(Camera *arg0, f32 *offset, f32 *unused);
void func_1512BB10(Camera *arg0);
void func_1512C068(Camera *arg0);
void func_1512C150(Camera *arg0);
void func_1512C200(Camera *camera);
void func_1512C20C(Camera *arg0);
s32 func_1512C47C(s32 arg0);

/* game_159940.c */
void func_1512C490(Camera *camera);
void func_1512D070(Camera *camera);
void func_1512D238(void);
void func_1512D2E4(Camera *camera, s32 index);
void func_1512D2F8(Camera *camera);
void func_1512D368(Camera *camera);

/* game/done/game_15A830.c */
void func_1512D380(s32 arg0);

/* game_15A840.c */
void func_1512D390(Camera *camera);

/* game_15AA10.c */
void func_1512D560(Camera *camera, s32 type, void *arg);
u8 *func_1512D604(); /* (Camera *camera): the definition has a second, scratch parameter */
void func_1512D66C(Camera *camera);
s32 func_1512D6B0(Camera *camera);

/* game_15ABA0.c */
void func_1512D6F0(Camera *view);
void func_1512D748(Camera *view, s32 presetIndex, s32 mode);
void func_1512D980(Camera *view);

/* game_15B200.c */
void func_1512DD50(Camera *camera);
void func_1512DEA4(Camera *camera);

/* game_15B5F0.c */
void func_1512E140(Camera *camera);

/* game_15B960.c */
void func_1512E4B0(Camera *camera);

/* game_15BAA0.c */
Gfx *func_1512E5F0(Gfx *gfx, Camera *camera);

/* game_15BD90.c */
void func_1512E8E0(Camera *camera);
void func_1512FE00(Camera *camera, s32 action);

/* game_191C30.c */
struct CamWobble *func_15164780(CamWobbleDef *def, s32 extraSize, u8 arg2, s32 arg3);
void func_15164888(struct CamWobble *wobble);
void func_1516489C(struct CamWobble *wobble);
void func_15164EE4(void);
struct CamWobbleEnv *func_15164F0C(u8 presetIndex, u8 playerIndex, struct CamWobbleFalloff *falloff, u8 arg3, s32 arg4);
struct CamWobbleEnv *func_15165628(struct CamWobblePreset *preset, u8 playerIndex, s8 scaleFn, s32 extraSize, u8 arg4, s32 arg5);
s32 func_151658DC(struct CamWobbleEnv *wobble);
f32 func_15165B80(struct CamWobbleEnv *wobble);
f32 func_15165BB0(struct CamWobble *wobble, Vec3f *origin, f32 radius, f32 fadeRange, f32 invFadeRange);

#endif
