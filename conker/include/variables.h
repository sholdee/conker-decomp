#ifndef VARIABLES_H
#define VARIABLES_H

#include "structs.h"
#include "sched.h"

/* defined */



extern u32 D_80042450[512]; // chunk0 offsets


/* undefined */

extern OSThread *__osRunningThread;

/* FUNCTIONS */
// FIXME: we should be able to reference these directly
extern s32  D_10013598;


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
extern OSThread D_80035910; // piThread of libultra/io/pimgr.c (which declares it itself); stopped by the PI manager in init/Os/DevMgr/devmgr




/* 80040000 */


extern s32  D_80040000;





// BSS ENDS AT D_80043B40

extern OSMesg __osEepromTimerMsg;
extern s32  D_80043B40[]; // 0xFE0-byte block past BSS end (TLB slot table in 45B80)
extern u8   D_80044B20[];             // audio heap memory, 0x3E000 bytes

// this space is filled by D_80044B20
// 0x80044B20 + 253952 = 0x80082B20

/* 80080000 */

extern u8 (*gFontBitmaps[])[0xE0]; // per-font glyph bitmaps, 0xE0 bytes each
extern FontGlyph *gFontGlyphs[];



extern u64 gRandomState;





/* 8009 */

extern s32 jtbl_80091A30[];


// chunk0 data starts at rom 0x80082B20
// this chunk0 data
// D_800959E0 is 0x80082B20 + 0x00012ec0



// code_75400.c




extern f32 D_8009F6C0; // 0.01745329238474369f

/* 800A0000 */


// in game/Effects/Light/light.c



// end of data D_800B0DBC

/* 800B0000 */

extern LevelHeader *D_800B0DF0;

extern s32 D_800BE5D0;









/* 800C0000 */

extern u64 *gUcodeData; // RSP ucode data, right after its text in gUcodeText
extern u8 D_800BEBA0;
extern s32 gDisplayListLimit;
extern u64 gUcodeText[]; // RSP ucode text buffer
extern s32 gCutsceneTimes[]; // per scene: current time
extern s32 gCutscenePrevTimes[]; /* time at the previous update */
extern u8 D_800C35C0[];
extern u8 gNextCutscene;
extern u8 gNextCutsceneMap;
extern u8  gPendingCutscene;
extern u8  D_800C35C5;
extern u16 * gSeqTrackEventCounts[]; /* each track's event count */
extern u8 **gSeqTextLists[]; // per scene: text lists
extern u8 gCutsceneIds[2]; // per scene: changes with each run of the scene
extern u8  gCutsceneStates[2]; // per cutscene scene slot (0/1): cutscene state, 1 = playing
extern SeqTrackInfo *gSeqTrackInfos[]; // per scene: what each track drives
extern u8 gSeqCameraTrack;
extern CamShot gCamShot; // scripted camera shot
extern u8  D_800C3638;
extern u8 gSeqTrackCounts[]; /* track count */
extern s32 gCutsceneEndTimes[]; /* end time */
extern f32 gViewFovOffset; // added to both FOVs of every view
extern f32 gNearPlaneOverride; // near-plane override when non-zero
extern f32 gFarPlaneOverride; // far-plane override when non-zero
extern u8 D_800C3654[];
extern u8 D_800C3656[2];
extern u8 D_800C3658[];
extern u8 gCutsceneKeepSpawnedActors[];
extern u8 D_800C365C[2];
extern u8 D_800C365E[2];
extern u8 D_800C3660[2];
extern u8 gCutsceneHideWorld;
extern u8  gCutsceneAnchorSet;
extern u8  D_800C3670;
extern u8  D_800C3671;
extern u8 gCutsceneSkipped;
extern f32 gCutsceneCutbackAngle;
extern f32 gCutsceneEndTransitionYaw; // camera transition parameter (999.0f = unset); gCutsceneEndTransitionTime is its duration
extern s32 gCutsceneEndTransitionTime;
extern u8 D_800C3680;
extern u8  gCutsceneHideSky;
extern u8  D_800C3682;
extern u8  D_800C3683;
extern SeqEvent *gSeqEventTracks[][30]; // cutscene script event tracks per scene
extern SeqPathKey *gSeqPathKeys[][30]; // per scene and track: path keyframes
extern SeqTrackState *gSeqTrackStates[]; // per scene: the track states
extern u8 * gSeqActiveEvents[][30];
extern u8 D_800C3A58;
extern u8  gCutsceneSkipState;
extern u8 gCutsceneSkipFadeTimer;
extern s8 D_800C3C8A;
extern u8 D_800C3C8B;
extern u8 gCutsceneHoldEnd;
extern u8 D_800C3C8D;
extern u16 gCutsceneStartTime;
extern u8  gCutsceneCallback;
extern s32 gCutsceneCallbackArg;
extern u8 gLastCutscene;
extern u8 gCutsceneChained;
extern u16 gSeqEventQueueSize;
extern u8 gCutsceneNoSkip;
extern u8 gCutsceneChainBobScale;
extern s16 D_800C3C9E;
extern SeqSubtitle gSubtitles[3];
extern struct SeqQueueEntry *gSeqEventQueue; // dispatch queue, gSeqEventQueueSize entries (type in 50D80)
extern s64 gPlayedCutscenes[];

/* 800D0000 */

extern GroundCandidate gGroundCandidates[]; // ground-probe hits
extern u8  gCurrentActorSlot; // 1-based slot of the actor being updated (0 = none)
extern f32 D_800D35E0;
extern f32 gCullVtxFlags; /* 0x800D35E4 == (&gCullVtxFlags)[-1]; it has no symbol of its own */
extern u8 *D_800D3668; // per-face shadow flag table
extern u8 *D_800D366C; // end of D_800D3668
extern f32 D_800D3670;
extern s16 D_800D3674;
extern u16 gSurfSetA[40]; // grid-query result: triangle indices (u16 list for collision_gatherPropGroundTris / collision_transformPropTris)
extern u16 gSurfSetB[]; // second grid-query triangle index buffer
extern s32 gScriptCmdWordCount;
extern s32 gScriptSavedSp;

extern u8  D_800D987C;

/* One unit's whole .bss (0x800D9920..0x800D994F: two bytes, two 8-aligned u8[14] and the unit's padding). It lies
 * between game/Effects/TintedObject/tintedobject's .bss and game_10CD70's; no file of that range uses it, so it stays in asm. Set by script
 * command 2 (game/Script/EffectCmd/effectcmd.c), cleared by game/Level/LevelSetupReset/levelsetupreset.c and game/Level/LevelSetup02/levelsetup02.c; nothing reads it. */
extern u8  D_800D9920;
extern u8  D_800D9921;
extern u8  D_800D9928[14];
extern u8  D_800D9938[14];




extern s32 gMapTriCounts[4];
extern s16 gQuadNodeBitmapStride;
extern s16 gMap0QuadNodeCount;
extern s16 gMap0QuadLeafCount;
extern s16 gViewCameraZones[4]; // per-view camera zone
extern s32 gMapTriCollected;
extern MeshTri *gCurMapTris;
extern struct XZRange *gCurMapTriXZBounds;
extern struct YRange *gCurMapTriYBounds;
extern ZoneNode *gCurMapQuadTree; // zone tree root
extern s32 gCurMapTriCount;
extern s32 gCurMap;
extern u32 *gMeshTriMaterials; // per world-mesh triangle: material / surface flags
extern u8  gMeshBuilt;
extern u8  D_800DBE63;
extern f32 gFloorQueryReach;
extern f32 gFloorQueryX;
extern f32 gFloorQueryY;
extern f32 gFloorQueryZ;
extern f32 gRayProbeBottom;

extern u8 D_800DCA20;
extern f32 D_800DCA24;
extern u8 D_800DCA28;
extern f32 D_800DCA2C;
extern SpriteParams D_800DD230;
extern MessageEntry *D_800DD254;
extern MessageEntry *D_800DD258;






extern s32  D_800E9D00;
extern u8   D_800E9D10[]; // heap arena: from here to 0x80400000

/* other */

extern u32  D_00AB1950; // offsets table

extern u8   D_100290D0[]; /* rspboot text */
extern u8   D_100291A0[]; /* end of rspboot text */
extern s32 *D_1002AAD0;
extern s32  D_151F3C4C;

/* what is this */
extern u16  D_BC000C02;
extern s32  D_1FC007FC;

#include "subsystems.h"

#endif
