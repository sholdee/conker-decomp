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

void func_15047F00(MtxF mtx, u16 *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
void func_15048134(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
f32  func_1504A5E0(f32 arg0);


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


f32  func_15047C00(f32 arg0);
f32  func_15047D60(f32 arg0);
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
s32  func_15123934(Camera *camera, s32 newMode, s32 newMotion, s32 newTightness, CameraMode slot);
s32  func_151239CC(Camera *camera, CameraMode slot);
void func_15127EB8(Camera *arg0);
void func_1512D748(Camera*, s32, s32);
void func_15174690(s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_151EF954(Mtx *m, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far, f32 scale); // guOrtho: builds the float matrix (func_151EF800) and converts it with guMtxF2L2
s32 func_151F3C4C(s32 arg0, void *dst, s32 len, s32 offset);
void func_151FA130(void);
void func_1019EA88(void);
#include "subsystems.h"

#endif
