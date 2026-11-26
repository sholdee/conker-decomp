#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <ultra64.h>

#include "structs.h"

extern f32 fabsf(f32);
#pragma intrinsic (fabsf)

/* matching */
s32  func_100173C4(SndState **arg0);
SndState *func_10017438(ALBank *bank, s16 arg1, u16 arg2, u8 arg3, f32 arg4, u8 arg5, u8 arg6, SndState **arg7);
void func_10017594(SndState *arg0);
void func_100176C4(void);
void func_100176EC(void);
void func_10017714(SndState *state, s16 type, s32 arg);
void func_10017780(u8 arg0, u16 arg1);


void n_alInit(void *globals, void *config);
void n_alClose(void *globals);
Acmd *n_alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen);
f32  alCents2Ratio(s32 arg0);
/* chunk0 */



struct DefParticleHead;


/* non-matching */

// s32  func_10002DB0(s32 arg0, s32 arg1);
//func_100034E0
//func_10003658
// void heap_init(void);
//func_10005570
//func_100056A0
//func_100057E0
//func_10005948
//boot_mapCodeSegment
//boot_resetPageBitmap
//boot_handleGamePageFault
//boot_drawHexWord
//boot_drawString
//boot_drawGlyph
//boot_debugTrap
//boot_runFaultDebugger
//func_10008120
// void func_10017870(u8 arg0);
//__n_unmapVoice
//__n_voiceNeedsNoteKill
//__n_lookupVoice
//__n_lookupSoundQuick
//func_1001B200
// s32  audio_getVoiceFxMix(void *arg0, void *arg1);
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


void math_mtxScaleF(MtxF mtx, f32 x, f32 y, f32 z);
#include "subsystems.h"

#endif
