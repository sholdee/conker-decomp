#ifndef VARIABLES_H
#define VARIABLES_H

#include "structs.h"
#include "sched.h"

/* defined */





/* undefined */

extern OSThread *__osRunningThread;



/* 80000000 */

extern s32  D_80000000;
extern s32  D_80000004;
extern s32  D_80000008;
extern s32  D_8000000C;


extern s32  D_80000300;
extern s32  D_80000308;
extern s32  D_8000030C; // osResetType; libultra's declaration is in src/libultra/os/osint.h
extern u32  D_80000310;
extern s32  D_8000031C[16]; // osAppNMIBuffer (64 bytes); libultra's declaration is in src/libultra/os/osint.h

/* 80020000 */





extern u32  __osShutdown;

/* DATA section from D_8002C080 to gIdleThreadStack */


/* BSS STARTS at gIdleThreadStack */

/* 80030000 */
extern OSThread D_80035910; // piThread of libultra/io/pimgr.c (which defines it); stopped by the PI manager in init/Os/DevMgr/devmgr




/* 80040000 */







// BSS ENDS AT gPageSlots

extern OSMesg __osEepromTimerMsg;
extern s32  gPageSlots[]; // 0xFE0-byte block past BSS end (TLB slot table in 45B80)

/* 80080000 */

extern u8 (*gFontBitmaps[])[0xE0]; // per-font glyph bitmaps, 0xE0 bytes each
extern FontGlyph *gFontGlyphs[];



extern u64 gRandomState;





/* 8009 */



// chunk0 data starts at rom 0x80082B20
// this chunk0 data
// D_800959E0 is 0x80082B20 + 0x00012ec0



// code_75400.c




extern f32 gRotateRPYFDegToRad; // 0.01745329238474369f

/* 800A0000 */


// in game/Effects/Light/light.c



// end of data D_800B0DBC

/* 800B0000 */

extern LevelHeader *D_800B0DF0; // game/Level/LevelHeader/levelheader.c

extern s32 D_800BE5D0;

/* 800D0000 */

extern GroundCandidate gGroundCandidates[]; // ground-probe hits
extern u8  gCurrentActorSlot; // 1-based slot of the actor being updated (0 = none)
extern f32 gFrustumSlopeX;
extern f32 gCullVtxFlags; /* 0x800D35E4 == (&gCullVtxFlags)[-1]; it has no symbol of its own */
extern u8 *gShadowFaceFlags; // per-face shadow flag table
extern u8 *gShadowFaceFlagsEnd; // end of gShadowFaceFlags
extern f32 D_800D3670;
extern s16 D_800D3674;
extern u16 gSurfSetA[40]; // grid-query result: triangle indices (u16 list for collision_gatherPropGroundTris / collision_transformPropTris)
extern u16 gSurfSetB[]; // second grid-query triangle index buffer
extern s32 gScriptCmdWordCount;
extern s32 gScriptSavedSp;













extern s32  D_800E9D00;
extern u8   D_800E9D10[]; // heap arena: from here to 0x80400000

/* other */


extern s32 *D_1002AAD0;


#include "subsystems.h"

#endif
