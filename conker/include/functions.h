#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <ultra64.h>

#include "structs.h"

extern f32 fabsf(f32);
#pragma intrinsic (fabsf)

/* matching */
s32  func_100173C4(SndState **arg0);
void func_10017594(SndState *arg0);
void func_100176C4(void);
void func_100176EC(void);
void func_10017714(s32 arg0, s16 arg1, s32 arg2);


void n_alInit(void *globals, void *config);
void n_alClose(s32 arg0);
f32  alCents2Ratio(s32 arg0);
/* chunk0 */



struct DefParticleHead;


/* non-matching */

// s32  func_10002DB0(s32 arg0, s32 arg1);
//func_100034E0
//func_10003658
// void func_10003BD0(void);
//func_10005570
//func_100056A0
//func_100057E0
//func_10005948
//func_10005AB0
//func_10005BE0
//func_10005C2C
//func_10007C74
//func_10007CC4
//func_10007D28
//func_10007DA0
//func_10007DAC
//func_10008120
// void func_10017870(u8 arg0);
//func_1001AAE0
//func_1001ADA4
//func_1001AFEC
//func_1001B07C
//func_1001B200
// s32  func_1001B310(void *arg0, void *arg1);
// u8   func_1001B450(void *arg0, void *arg1);
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


void func_150A7CB0(MtxF mtx, f32 x, f32 y, f32 z);
u32  func_150ADA20(void);
#include "subsystems.h"

#endif
