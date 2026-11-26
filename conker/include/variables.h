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

/* DATA section from D_8002C080 to D_8002D4B0 */


/* BSS STARTS at D_8002D4B0 */

/* 80030000 */
extern OSThread D_80035910; // piThread of libultra/io/pimgr.c (which declares it itself); stopped by the PI manager in init_2E50




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



extern u64 D_800885B0;





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

extern u64 *D_800BEB9C; // RSP ucode data, right after its text in D_800C1060
extern u8 D_800BEBA0;
extern s32 D_800BEBA4;
extern u64 D_800C1060[]; // RSP ucode text buffer
extern s32 D_800C35B0[]; // per scene: current time
extern s32 D_800C35B8[]; /* time at the previous update */
extern u8 D_800C35C0[];
extern u8 D_800C35C2;
extern u8 D_800C35C3;
extern u8  D_800C35C4;
extern u8  D_800C35C5;
extern u16 * D_800C35D8[]; /* each track's event count */
extern u8 **D_800C35E0[]; // per scene: text lists
extern u8 D_800C35E8[2]; // per scene: changes with each run of the scene
extern u8  D_800C35EA[2]; // per cutscene scene slot (0/1): cutscene state, 1 = playing
extern SeqTrackInfo *D_800C35F0[]; // per scene: what each track drives
extern u8 D_800C35F8;
extern CamShot gCamShot; // scripted camera shot
extern u8  D_800C3638;
extern u8 D_800C363A[]; /* track count */
extern s32 D_800C3640[]; /* end time */
extern f32 D_800C3648; // added to both FOVs of every view
extern f32 D_800C364C; // near-plane override when non-zero
extern f32 D_800C3650; // far-plane override when non-zero
extern u8 D_800C3654[];
extern u8 D_800C3656[2];
extern u8 D_800C3658[];
extern u8 D_800C365A[];
extern u8 D_800C365C[2];
extern u8 D_800C365E[2];
extern u8 D_800C3660[2];
extern u8 D_800C3662;
extern u8  D_800C3663;
extern u8  D_800C3670;
extern u8  D_800C3671;
extern u8 D_800C3672;
extern f32 D_800C3674;
extern f32 D_800C3678; // camera transition parameter (999.0f = unset); D_800C367C is its duration
extern s32 D_800C367C;
extern u8 D_800C3680;
extern u8  D_800C3681;
extern u8  D_800C3682;
extern u8  D_800C3683;
extern SeqEvent *D_800C3688[][30]; // cutscene script event tracks per scene
extern SeqPathKey *D_800C3868[][30]; // per scene and track: path keyframes
extern SeqTrackState *D_800C3958[]; // per scene: the track states
extern u8 * D_800C3960[][30];
extern u8 D_800C3A58;
extern u8  D_800C3C88;
extern u8 D_800C3C89;
extern s8 D_800C3C8A;
extern u8 D_800C3C8B;
extern u8 D_800C3C8C;
extern u8 D_800C3C8D;
extern u16 D_800C3C8E;
extern u8  D_800C3C90;
extern s32 D_800C3C94;
extern u8 D_800C3C98;
extern u8 D_800C3C99;
extern u16 D_800C3C9A;
extern u8 D_800C3C9C;
extern u8 D_800C3C9D;
extern s16 D_800C3C9E;
extern SeqSubtitle D_800C3CA0[3];
extern struct SeqQueueEntry *D_800C3D50; // dispatch queue, D_800C3C9A entries (type in 50D80)
extern s64 D_800C3A60[];

/* 800D0000 */

extern GroundCandidate D_800D3300[]; // ground-probe hits
extern u8  D_800D35DC; // 1-based slot of the actor being updated (0 = none)
extern f32 D_800D35E0;
extern f32 D_800D35E8; /* 0x800D35E4 == (&D_800D35E8)[-1]; it has no symbol of its own */
extern u8 *D_800D3668; // per-face shadow flag table
extern u8 *D_800D366C; // end of D_800D3668
extern f32 D_800D3670;
extern s16 D_800D3674;
extern u16 D_800D37E0[40]; // grid-query result: triangle indices (u16 list for func_150A43E0 / func_150A44F0)
extern u16 D_800D3830[]; // second grid-query triangle index buffer
extern s32 D_800D3840;
extern s32 D_800D3858;

extern u8  D_800D987C;

/* One unit's whole .bss (0x800D9920..0x800D994F: two bytes, two 8-aligned u8[14] and the unit's padding). It lies
 * between game_1048D0's .bss and game_10CD70's; no file of that range uses it, so it stays in asm. Set by script
 * command 2 (game_C9EC0.c), cleared by game_3BA70.c and game_3D9A0.c; nothing reads it. */
extern u8  D_800D9920;
extern u8  D_800D9921;
extern u8  D_800D9928[14];
extern u8  D_800D9938[14];




extern s32 D_800DBE18[4];
extern s16 D_800DBE28;
extern s16 D_800DBE2A;
extern s16 D_800DBE2C;
extern s16 D_800DBE30[4]; // per-view camera zone
extern s32 D_800DBE38;
extern MeshTri *D_800DBE3C;
extern struct XZRange *D_800DBE40;
extern struct YRange *D_800DBE44;
extern ZoneNode *D_800DBE48; // zone tree root
extern s32 D_800DBE4C;
extern s32 D_800DBE50;
extern u32 *D_800DBE5C; // per world-mesh triangle: material / surface flags
extern u8  D_800DBE62;
extern u8  D_800DBE63;
extern f32 D_800DBE64;
extern f32 D_800DBE68;
extern f32 D_800DBE6C;
extern f32 D_800DBE70;
extern f32 D_800DBE74;

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
