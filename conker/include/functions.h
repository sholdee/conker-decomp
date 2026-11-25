#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <ultra64.h>

#include "structs.h"

extern f32 fabsf(f32);
#pragma intrinsic (fabsf)

/* Sound bank loader (init_8F90.c): a bank fetch proc takes an owner slot (a ROM reference, or a record
 * once loading has started) and returns the bank's data, loading it on demand. */
typedef struct BankRecord BankRecord;
typedef void *(*BankFetchProc)(u32 *slot, s32 mode);

/* matching */
void func_1000349C(s32 arg0);
void func_100084D8(u8 arg0);
s32  func_1000853C(u8 arg0);
void func_10008570(u8 player, OSMesgQueue *queue);
void func_100085A4(s32 arg0, s32 arg1, s32 arg2);
void func_100085B8(u8 arg0, s32 arg1, u8 arg2);
void func_100085F8(u8 arg0, s32 arg1);
void func_1000862C(u8 arg0, s32 arg1);
void func_10008660(u8 arg0, u8 arg1, u8 arg2, s32 arg3);
void func_100086FC(u8 arg0, u8 arg1, u8 arg2);
void func_10008744(u8 arg0, u8 arg1, u8 arg2);
void func_10008790(u8 arg0, s32 arg1, u8 arg2, s32 arg3);
void func_10008824(u8 arg0, u8 arg1, u8 arg2);
void func_1000886C(u8 arg0, s32 arg1, u8 arg2);
void func_100088F0(u8 arg0, s32 arg1, s32 arg2);
void func_10008988(u8 idx, s32 mask, s32 enable);
u8   func_10008A4C(u8 idx, u8 chan);
void func_10008A94(u8 arg0, s32 arg1, s32 arg2);
void func_10008B2C(u8 arg0);
void func_10008B60(u8 arg0, u8 arg1, u8 arg2, u8 arg3, s32 arg4);
void func_10008BC0(u8 arg0, f32 arg1, f32 arg2);
void func_10008EE0(u8 arg0, s32 arg1);
void func_10008F24(u8 arg0);
void func_10008F58(u8 arg0);
void func_100093CC(void);
ALDMAproc func_10009980(void *state);
void func_10009B2C(BankRecord *record);
void func_10009B4C(BankRecord *record);
void func_10009B90(BankRecord *record);
void func_10011FA0(s32 *arg0);
void func_10011FB0(s32 arg0);
void func_10011FDC(s32 arg0);
void func_10011FEC(void);
s32  func_100173C4(SndState **arg0);
void func_10017594(SndState *arg0);
void func_100176C4(void);
void func_100176EC(void);
void func_10017714(s32 arg0, s16 arg1, s32 arg2);
void func_10017870(u8 arg0);
void func_10017944(s32 arg0, u32 arg1);


void n_alInit(void *globals, void *config);
void n_alClose(s32 arg0);
f32  alCents2Ratio(s32 arg0);
s32  func_1001E4A0(s16 arg0, s32 arg1, s32 arg2);
f32  func_1001FA78(MtxF arg0, s32 arg1);
/* chunk0 */
void func_15000AC0(void);
void func_15001B08(void);
u16* func_15001B10(void);
void func_15001BC8(s32 arg0, s16 *arg1, s16 *arg2);
void func_15001970(void);
void func_150026C4(s32 arg0);
void func_150026E8(s32 arg0);
void func_15002724(s32 arg0);
void func_15002F40(s32 arg0);
s32  func_15002FA0(s32 arg0);
void func_15002FB4(s32 arg0);
void func_1500310C(void);
s32  func_150034B4(void);
void func_150038A0(void);
s32  func_1500390C(s32 arg0);
void func_150039B0(s32 arg0);
void func_150039BC(s32 arg0);
void func_150045BC(void);
void func_15005A60(Camera *arg0, s32 arg1);
void func_15005AB0(Camera *arg0);
void func_15005AF0(s32 arg0);
void func_15005B00(s32 arg0);
void func_15005B10(Camera *arg0);
void func_15005B50(s32 arg0);
void func_15005B60(s32 arg0);
void func_15005B70(s32 arg0);
void func_15005BD0(Camera *arg0);
void func_15005C30(Camera *arg0);
void func_15005C80(Camera *arg0);
void func_15005CF0(s32 arg0);
void func_15005D00(Camera *arg0);
void func_15005D60(Camera *arg0);
void func_15005DB0(Camera *arg0);
void func_15005E30(Camera *arg0);
void func_15005E70(Camera *arg0);
void func_15005EA0(Camera *arg0);
void func_15005EE0(Camera *arg0);
void func_15005F20(Camera *arg0);
void func_15005F60(Camera *arg0);
void func_15005FA0(s32 arg0);
void func_15005FB0(Camera *arg0);
void func_15006010(Camera *arg0);
void func_15006140(Camera *arg0);
void func_15006170(Camera *arg0);
void func_150064E0(void);
void func_15007644(void);
void func_1500764C(void);
void func_15007668(void);
void func_15007684(void);
void func_150076A0(); /* defined with no parameters, but game_20BDA0.c passes one */
void func_150076BC(s32 arg0);
void func_15007718(s32 arg0);
void func_15007750(s32 arg0);
void func_150095D8(s32 arg0, s32 arg1);
void func_15009600(s32 arg0, s32 arg1);
void func_1500969C(s32 arg0, s32 arg1);
void func_15009740(s32 arg0, s32 arg1);
void func_15009768(s32 arg0, s32 arg1);
void func_150097A4(s32 arg0, s32 arg1);
void func_150097CC(s32 arg0, s32 arg1);
void func_15009818(s32 arg0, s32 arg1);
void func_15009844(s32 arg0, s32 arg1);
void func_15009870(s32 arg0, s32 arg1);
void func_15009894(s32 arg0, s32 arg1);
void func_150098D0(s32 arg0, s32 arg1);
void func_150098F8(s32 arg0, s32 arg1);
void func_15009944(s32 arg0, s32 arg1);
void func_15009AA0(s32 arg0, s32 arg1);
void func_15009AEC(s32 arg0, s32 arg1);
void func_15009B38(s32 arg0, s32 arg1);
void func_15009B84(s32 arg0, s32 arg1);
void func_15009D28(s32 arg0, s32 arg1);
void func_15009D6C(s32 arg0, s32 arg1);
void func_15009DB0(s32 arg0, s32 arg1);
void func_15009DFC(s32 arg0, s32 arg1);
void func_15009E48(s32 arg0, s32 arg1);
void func_15009E84(s32 arg0, s32 arg1);
void func_15009EC8(s32 arg0, s32 arg1);
void func_15009EF4(s32 arg0, s32 arg1);
void func_15009F30(s32 arg0, s32 arg1);
void func_1500A028(s32 arg0, s32 arg1);
void func_1500A06C(s32 arg0, s32 arg1);
void func_1500A0B0(s32 arg0, s32 arg1);
void func_1500A0FC(s32 arg0, s32 arg1);
void func_1500A148(s32 arg0, s32 arg1);
void func_1500A194(s32 arg0, s32 arg1);
void func_1500A1E0(s32 arg0, s32 arg1);
void func_1500A21C(s32 arg0, s32 arg1);
void func_1500A260(s32 arg0, s32 arg1);
void func_1500A33C(s32 arg0, s32 arg1);
void func_1500A380(s32 arg0, s32 arg1);
void func_1500A3C4(s32 arg0, s32 arg1);
void func_1500A410(s32 arg0, s32 arg1);
void func_1500A454(s32 arg0, s32 arg1);
void func_1500A490(s32 arg0, s32 arg1);
void func_1500A4D4(s32 arg0, s32 arg1);
void func_1500A518(s32 arg0, s32 arg1);
void func_1500A55C(s32 arg0, s32 arg1);
void func_1500A5A8(s32 arg0, s32 arg1);
void func_1500A5F4(s32 arg0, s32 arg1);
void func_1500A640(s32 arg0, s32 arg1);
void func_1500A68C(s32 arg0, s32 arg1);
void func_1500A79C(s32 arg0, s32 arg1);
void func_1500A8C8(s32 arg0, s32 arg1);
void func_1500A904(s32 arg0, s32 arg1);
void func_1500A94C(s32 arg0, s32 arg1);
void func_1500A990(s32 arg0);
void func_1500AB5C(s32 arg0);
void func_1500ABA0(s32 arg0);
void func_1500BE40(s32 arg0);
void func_1500BE68(void);

void func_1500EB20(void);
void func_1500EB30(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_1500EBC4(void);
void func_1500ED80(void);
void func_1500EE18(Actor *arg0, s32 arg1, s32 arg2);
void func_1500EE94(TriggerZone *zone);
void func_1500EF20(void);

void func_15010240(void);
void func_150102C0(void);
void func_150102D0(void);
void func_150103E0(void);
void func_15010538(Actor *arg0);
void func_15010680(void);
void func_150106A0(void);
void func_150106B0(void);
s32  func_150106D0(TriggerZone *zone);
void func_15010780(void);
void func_15010FB0(void);
void func_150110F0(void);
void func_15011170(void);
void func_15011330(void);
void func_15011360(void);
void func_15011A78(Actor *arg0, u8 arg1, s32 arg2);
void func_15011B00(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_15011B94(void);
void func_15011C40(void);
void func_15011C70(void);
void func_15011CA0(void);
void func_15011CC0(void);
void func_15011F20(s32 arg0);
void func_15011FA0(s32 arg0);
void func_15012020(s32 arg0, s32 arg1);
u8   func_15012720(void);
void func_15012770(void);
void func_15012780(void);
s32  func_150150A4(void);
s32  func_15015300(TriggerZone *obj);

void func_15047F00(MtxF mtx, u16 *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
void func_15048134(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
f32  func_1504A5E0(f32 arg0);

void func_15075498(void);
void func_15075A50(void);
void func_15075B60(void) ;

void func_1509B4A0(s32 arg0, s32 arg1);
void func_1509BFB0(s32 num, s32, s32, ...);
void func_150CEF10(Camera *arg0);

void func_150EC4B0(Camera *arg0);
void func_150FCA00(Camera *arg0);

void func_1510550C(EffectHeader *arg0, s32 arg1, u8 arg2);
typedef struct BoltArcObj BoltArcObj; /* game_131F30.c */
typedef struct SparkObj SparkObj;     /* game_131F30.c */
void func_15105548(BoltArcObj *obj, s32 *arg1, u8 msg);
void func_15105848(SparkObj *obj, s32 arg1, u8 msg);
void func_15122AE0(void);
void func_15124B18(Camera *arg0);
void func_15123508(Camera *arg0);
void func_15124770(Camera *arg0, s32 arg1);
void func_15124AB4(Camera *arg0);
void func_1512523C(Camera *arg0);
void func_15125330(Camera *arg0);
void func_15125394(Camera *arg0);
s32  func_151253CC(Camera *arg0);
void func_15125594(Camera *arg0);
void func_15125608(Camera *arg0);
void func_15125690(Camera *arg0, s32 arg1);
void func_15126138(Camera *arg0);
void func_15127FEC(Camera *arg0, s32 arg1, s32 arg2);
void func_15128680(Camera *arg0);
void func_1512868C(Camera *arg0);
void func_15128774(Camera *arg0, Actor *arg1);
void func_1512D380(s32 arg0);
struct DefParticleHead;

/* Actor attachment lists (Actor.attachments, game_179F30): `handle` is the attached effect or object. */
void *func_15167A68(s32 type, s32 list, s32 size, s32 arg3, s32 owner, s32 arg5);
void func_1516972C(EffectHeader *obj);
void func_1516979C(EffectHeader *obj);
void func_15169804(EffectHeader *obj);
void func_15169824(EffectHeader *obj);
void func_1519EF70(Actor *arg0, s32 arg1, s32 arg2);

void func_151DB004(struct ParticleDesc *desc);
void func_151DB068(struct ParticleDesc *desc);
void func_151DB0CC(struct ParticleDesc *desc);
void func_151DB15C(struct ParticleDesc *desc);
void func_151DB1EC(struct ParticleDesc *desc);
void func_151DB27C(struct ParticleDesc *desc);
void func_151DB2A8(struct ParticleDesc *desc);
void func_151DB2CC(struct ParticleDesc *desc);
void func_151DB330(struct ParticleDesc *desc);
void func_151DB3D8(struct ParticleDesc *desc);
void func_151DB43C(struct ParticleDesc *desc);
void func_151DBBD4(Vec3f *arg0, s32 arg1, u8 *arg2, u8 arg3, s32 arg4);
void func_151F0080(MtxF arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_151F00E0(MtxF arg0, MtxF arg1, MtxF arg2);

/* non-matching */

// s32  func_10002DB0(s32 arg0, s32 arg1);
void func_100030A0(OSPri priority, OSMesgQueue *mesgQueue, OSMesg *arg2, s32 arg3);
//func_100034E0
//func_10003658
// void func_10003BD0(void);
//func_10005570
//func_100056A0
//func_100057E0
//func_10005948
//func_10005AB0
void func_10005B04(s32 arg0);
//func_10005BE0
//func_10005C2C
void func_1000709C(void);
//func_10007C74
//func_10007CC4
//func_10007D28
//func_10007DA0
//func_10007DAC
//func_10008120
//func_10008180
//func_10008B60
//func_10008BC0
//func_10008C04
void func_10008C6C(u8 arg0, u8 arg1);
//func_10008CE8
//func_10008F90
void func_10009400(void *arg);
//func_100095A0
s32 func_100097CC(s32 addr, s32 len, void *state);
//func_100099BC
void func_10009BE4(BankRecord *record);
void *func_10009CBC(u32 *slot, s32 mode);
BankFetchProc func_10009FFC(void);
//func_1000A03C
//func_1000A348
//func_10012020
// void func_10017870(u8 arg0);
//func_1001AAE0
//func_1001ADA4
//func_1001AFEC
//func_1001B07C
//func_1001B200
// s32  func_1001B310(void *arg0, void *arg1);
// u8   func_1001B450(void *arg0, void *arg1);
Acmd *func_1001E530(s16 *outp, Acmd *p, s32 bus);
//func_1019EA88
//func_1019EAB0
//func_1019EAE0
//func_1019ECAC
//func_1019ED8C
//func_1019ED94
//func_1019ED9C
//func_1019EE0C
//func_1019EEAC
//func_1019F018
//func_1019F154
//func_1019F214
//func_1019F4E4
//func_1019F59C
//func_1019FA14
//func_1019FACC
//func_1019FD38
//func_1019FDC0
//func_1019FE18
//func_1019FF78
//func_101A0094
//func_101A0100
//func_101A017C
//func_101A0188
//func_101A02B8
//func_101A0344
//func_101A040C
//func_101A0430
//func_101A04EC
//func_101A04F4
//func_101A0538
void func_1001C224(void *arg0, s32 arg1, s32 arg2, s32 arg3);

u16 *func_15001DE0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

f32  func_15047C00(f32 arg0);
f32  func_15047D60(f32 arg0);
void func_1507BAD0(void);
s32  func_15081574(s32, f32, f32, s32, s32, s32);
void *func_15083568(Actor *parent, s32 setId, f32 arg2, s32 arg3);
s32  func_15083E0C(u8);
Actor *func_15083E90(u8 id);
void func_1508B20C(f32, f32, f32, f32);
void func_150A7CB0(MtxF mtx, f32 x, f32 y, f32 z);
void func_150A8050(MtxF mtx, f32 arg1, f32 arg2, f32 arg3);
void func_150A9B0C(MtxF mtx, f32, f32, f32, f32, f32, f32);
void func_150AD770(void);
f32  func_150AD780(f32);
f32  func_150AD78C(f32);
f32  func_150AD900(f32 *arg0, f32 *arg1);
f32  func_150AD930(f32 *arg0);
u32  func_150ADA20(void);
f32  func_150ADA68(void);
void func_150E2EA4(Actor*, s32, s32, s32, f32, f32, f32, f32, f32, f32, s32, s32, s32, f32, f32, s32, f32);
s32  func_1510FD20(s32 arg0, s32 arg1);
s32  func_15123934(Camera *camera, s32 newMode, s32 newMotion, s32 newTightness, CameraMode slot);
s32  func_151239CC(Camera *camera, CameraMode slot);
void func_15127EB8(Camera *arg0);
void func_1512D748(Camera*, s32, s32);
Gfx *func_1515D6D0(Gfx *gfx, s32 player);
void func_1515D4D4(s32 r, s32 g, s32 b, u8 priority);
void func_15169260(void *types, s32 count, s32 event, u8 eventType);
void func_1516944C(s32, s32, u8);
void func_15169850(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj);
void func_15174690(s32, s32, s32, s32, s32, s32, s32, s32, s32);
s32  func_15187EC0(s32 arg0, f32 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7);
void func_151B8DB0(s32 arg0, u8 arg1, u8 arg2, s32 arg3);
void func_151D5404(Vec3f*, f32, f32, f32, s32, s32, s32, s32);
void func_151D5714(Actor *obj, Vec3f *from, Vec3f *to, u8 bone, f32 scale, u8 arg5, s32 arg6);
void func_151D9B8C(u8 arg0, f32 arg1, u8 arg2, s32 arg3, Vec3f *arg4, s16 arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, s32 argA);
s32 func_151DA08C(u8 kind, f32 size, f32 shrink, u8 arg3, s16 lifetime, s32 arg5, Vec3f *pos, s32 arg7, s32 arg8);
void func_151DBCBC(u8 arg0, f32 arg1, s16 arg2, s32 arg3, void *arg4, u8 arg5, s32 arg6);
void func_151EF954(Mtx *m, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far, f32 scale); // guOrtho: builds the float matrix (func_151EF800) and converts it with guMtxF2L2
s32 func_151F3C4C(s32 arg0, void *dst, s32 len, s32 offset);
void func_151FA130(void);
void func_1019EA88(void);
void func_15168B10(EffectHeader *obj, s32 type);
#include "subsystems.h"

#endif
