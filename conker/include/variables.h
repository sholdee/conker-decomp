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

extern u8 (*D_80085990[])[0xE0]; // per-font glyph bitmaps, 0xE0 bytes each
extern FontGlyph *D_80085994[];



extern u64 D_800885B0;



extern u8   D_8008CEB0;
extern u8   D_8008CD00;
extern s32 D_8008CEB4; /* axis D_800DDD18 is sorted along: 0 = x, else z */
extern f32 *D_8008CFFC[]; // per LevelHeader.unk10: sun position (game_1AB530 FLARE_SUN)

extern Vec3f * D_8008FC8C[];
extern u8 * D_8008FC94[];
extern u8 (*D_8008FCD0[])(void); // texture id pickers by effect kind (game_2062D0)
extern u8 (*D_8008FD04[])(void); // a second picker table by effect kind (game_2062D0)

/* 8009 */
extern s32 D_80090298[]; // texture ids
extern s32 D_800902B8;
extern s32 D_800902E4;
extern s32 D_800902DC;
extern s32 D_800902E8;
extern s32 D_800902EC;
extern s32 D_80090310;
extern s32 D_80090314;
extern s32 D_80090318;
extern s32 D_8009031C;
extern s32 D_80090320;

extern s32 jtbl_80091A30[];

extern s32 D_80090324[];
extern TextureDesc D_80090514;
/* Texture entries that the texture table D_8008CA4C (game_1944C0.c) points at. */
extern TextureDesc D_80090388;
extern TextureDesc D_800903C8;
extern TextureDesc D_800903D4;
extern TextureDesc D_80090420;
extern TextureDesc D_8009042C;
extern TextureDesc D_80090438;
extern TextureDesc D_80090444;
extern TextureDesc D_80090450;
extern TextureDesc D_8009045C;
extern TextureDesc D_80090468;
extern TextureDesc D_80090474;
extern TextureDesc D_80090494;
extern TextureDesc D_800904B4;
extern TextureDesc D_800904C0;
extern TextureDesc D_800904CC;
extern TextureDesc D_800904D8;
extern TextureDesc D_800904E4;
extern TextureDesc D_800904F0;
extern TextureDesc D_800904FC;
extern TextureDesc D_80090508;
extern TextureDesc D_80090520;
extern TextureDesc D_8009052C;
extern TextureDesc D_8009053C;
extern TextureDesc D_800905A8;
extern TextureDesc D_800905B4;
extern TextureDesc D_800905C0;
extern TextureDesc D_800905CC;
extern TextureDesc D_800905D8;
extern TextureDesc D_800905E4;
extern TextureDesc D_800905F0;
extern TextureDesc D_80090600;
extern TextureDesc D_80090634;
extern TextureDesc D_80090660;
extern TextureDesc D_8009066C;
extern TextureDesc D_80090678;
extern TextureDesc D_80090684;
extern TextureDesc D_80090690;
extern TextureDesc D_800906C4;
extern TextureDesc D_800906F8;
extern TextureDesc D_80090704;
extern TextureDesc D_80090710;
extern TextureDesc D_80090730;
extern TextureDesc D_8009075C;
extern TextureDesc D_8009077C;
extern TextureDesc D_8009078C;
extern TextureDesc D_80090798;
extern TextureDesc D_800907EC;
extern TextureDesc D_800907FC;
extern TextureDesc D_8009080C;
extern TextureDesc D_80091834;
extern TextureDesc D_8009184C;
extern TextureDesc D_80091858;
extern TextureDesc D_80091864;
extern TextureDesc D_80091870;
extern TextureDesc D_80091888;
extern TextureDesc D_80091894;
extern TextureDesc D_800918AC;
extern TextureDesc D_800918B8;
extern TextureDesc D_800918C4;
extern TextureDesc D_800918D0;
/* Asset id lists of D_80082B20 (game_305D0.c). */
extern s32 D_80090150[];
extern s32 D_8009016C[];
extern s32 D_80090184[];
extern s32 D_800901C4[];
extern TextureDesc D_80090B60[]; /* the effect texture table (the `texture` index of the effect descriptors) */
/* Entries of the effect texture table D_80090B60, referenced directly. */
extern TextureDesc D_80090CD4;
extern TextureDesc D_80090D70;
extern TextureDesc D_80090DE8;
extern TextureDesc D_80091100;
extern TextureDesc D_80091238;
extern Texture D_80091514;
extern s32 D_80091564[];
extern Texture D_800915B0;
extern s32 D_800917B8;
extern HudSprite D_800917EC;
extern HudSprite D_800917F8;
extern TextureDesc D_8009187C;
extern TextureDesc D_800918A0;

// chunk0 data starts at rom 0x80082B20
// this chunk0 data
// D_800959E0 is 0x80082B20 + 0x00012ec0



// code_75400.c




extern f32 D_8009F6C0; // 0.01745329238474369f

/* 800A0000 */


// in game_18D770.c



// end of data D_800B0DBC

/* 800B0000 */

extern u8 D_800AB62C[][3]; // team colours (RGB)
extern ModeEntry D_800AB68C[]; // multiplayer game-mode table
extern s32 D_800AB754[];
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




extern f32 D_800CC2C0;
extern f32 D_800CC2C4;
extern f32 D_800CC2C8;

extern Actor movement_obj[26]; // objects



/* 800D0000 */


extern f32 D_800D1550; // frame time step
extern s32 D_800D1548;
extern Actor *D_800D154C;



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

extern u8  D_800D9930[];
extern u8  D_800D9920;
extern u8  D_800D9921;
extern u8  D_800D9928[14];
extern u8  D_800D9929;
extern u8  D_800D9938[14];
extern u8  D_800D9939;
extern u8  D_800D992A[];
extern u8  D_800D993A[];
extern u8  D_800D9946[]; // just past the end of D_800D993A (func_15010600 stops there)


extern u8  D_800D9B68[4][3];
extern u8  D_800D9B78[4][3];
extern u8  D_800D9B84[];
extern u8  D_800D9B88[];
extern u8  D_800D9B8B;
extern u8  D_800D9B8C;


extern s32 D_800DBE18[];
extern s32 D_800DBE1C[];
extern s32 D_800DBE20;
extern s32 D_800DBE24;
extern s16 D_800DBE28;
extern s16 D_800DBE2A;
extern s16 D_800DBE2C;
extern s16 D_800DBE30[]; // per-view camera zone
extern u16 D_800DBE32;
extern u16 D_800DBE34;
extern u16 D_800DBE36;
extern s32 D_800DBE38;
extern MeshTri *D_800DBE3C;
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

extern u8 D_800DCA20;
extern f32 D_800DCA24;
extern u8 D_800DCA28;
extern f32 D_800DCA2C;
extern SpriteParams D_800DD230;
extern MessageEntry *D_800DD254;
extern MessageEntry *D_800DD258;

extern u8   D_800DD404;
extern u8   D_800DD405;
extern u8   D_800DD406;
extern WaveDeformer *D_800DD410[2]; // D_800DD418 of them in use
extern u8   D_800DD418;
extern s16  D_800DD444; // state timer
extern s32  D_800DD478[15 * 26]; // per texture type: 26 frame addresses (game_1A89B0)

extern ParticleTexFile D_800DDA90[15];
extern u8 D_800DDBC0[]; /* index into D_80095CE0 */
extern u8 D_800DDBD0[15]; /* frame count the phases cycle through */
extern u8 D_800DDBE0[]; /* frame shown while a sprite is disturbed (unkB >= 0x21) */
extern u8 D_800DDBF0[]; /* draw pass (see func_1517D7B0) */
extern s32  D_800DDC00;
extern s32  D_800DDC08;
extern s32  D_800DDC04;
extern u8 D_800DDC10[]; /* per-phase direction bits for ping-pong animation */
extern u8 D_800DDC80[]; /* row of D_8008CF00 */
extern s16 D_800DDC90[60]; /* animation phases, 4 per type (frame << 8) */
extern Vtx *D_800DDD10[2]; // this frame's sprite vertex buffer (game_1A89B0), one per frame buffer
extern s32  D_800DDD0C;
extern u8   D_800DDD1C;
extern s32  D_800DDD20;
extern u8   D_800DDD61;
extern LensFlare *D_800DDD64; // head of the lens flare list

extern s8   D_800DDDAC[]; // per view: colour fade direction (nonzero: fading to the colour)
extern u8   D_800DDDAD;
extern u8   D_800DDDAE;
extern u8   D_800DDDAF;
extern s32  D_800DDDB0[]; // per view: colour fade elapsed frames
extern s32  D_800DDDB4;
extern s32  D_800DDDB8;
extern s32  D_800DDDBC;
extern f32  D_800DDDC8[]; // per view: iris closure (0 open .. 1 closed)
extern f32  D_800DDDCC;
extern f32  D_800DDDD0;
extern f32  D_800DDDD4;
extern f32  D_800DDDD8[]; // per view: iris closure change per frame
extern f32  D_800DDDDC;
extern f32  D_800DDDE0;
extern f32  D_800DDDE4;
extern u16  D_800DDE08; // frames left
extern u16 D_800DDE10[]; // frames left
extern f32  *D_800DDE18;  // letterbox amount in percent, or NULL when off
extern u8   D_800DDE1C[]; // per view: iris style
extern u8   D_800DDE1D;
extern u8   D_800DDE1E;
extern u8   D_800DDE1F;
extern u8   D_800DDE20[]; // per view: scripted iris
extern u8   D_800DDE21;
extern u8   D_800DDE22;
extern u8   D_800DDE23;
extern s32  D_800DDE2C;
extern s32  D_800DDE28[]; // per view: colour fade duration
extern s32  D_800DDE30;
extern s32  D_800DDE34;
extern u8   D_800DDE38;
extern u8   D_800DDE3C[]; // per view: iris tracking the player
extern u8   D_800DDE3D;
extern u8   D_800DDE3E;
extern u8   D_800DDE3F;
extern u8   D_800DDE40; // nonzero: iris driven by the cutscene tracks
extern f32 * D_800DDE44; // closure, percent
extern f32 * D_800DDE48; // hole x, percent of the view width
extern f32 * D_800DDE4C; // hole y, percent of the view height

extern u8 D_800DF0E0;
extern u8 D_800DF0E1;
extern MtxS16 *D_800DF7C0; // bone matrices drawn by func_1502C974 mode 4
extern u8 D_800DF7C4; // alpha scale for func_1502C974 mode 4
extern BoneTrail *D_800DF7C8[2]; // [0]: Actor owners, [1]: world object owners

/* 800E0000 */
extern u8 D_800DF7D0[]; // per-model load reference counts
extern s16 *D_800DF9B8[]; // per model: written by func_1510CE60
extern AttachModelHeader *D_800E0148[]; // per model: loaded attachment model header
extern u8 D_800E0A00;
extern s32 D_800E0A74;
extern s16 D_800E0A80;
extern u8 D_800E0A8C;
extern s32 D_800E0A90;
extern u8 D_800E0A94; /* menu brightness, scales the page alpha */
extern u8 D_800E0A95; /* current page's fade level, 0-0xFF */
extern u8 D_800E0A97;
extern s16 D_800E0AA0[];
extern s8 D_800E0AC0[];
extern s16 D_800E0AD0[16];
extern MenuState D_800E0AF0; // the instance D_8008FDD4 points at
extern u8 **D_800E0B88; // bank 2 text strings (80)
extern u8 D_800E0B8C;
extern s8 D_800E0B90[];
extern u8   D_800E0B94;
extern u8 D_800E0B95;
extern u8 D_800E0B96;
extern u8 D_800E0B97;
extern u8 D_800E0B98;
extern u8 D_800E0B99;
extern s16 D_800E0B9A;
extern s8 D_800E0C00[]; /* team of each D_8008FE44 entry */
extern s16 D_800E0C10[];
extern s32 D_800E0C18[];
extern s8   D_800E0C20;
extern void (*D_800E0E00)(s32, void *, s32); // stream tag callback (set by func_151F3C1C; gets each MP3 frame's copyright tag)
extern s32  D_800E0E04;
extern TriggerZone *D_800E0900[6]; // per flamethrower character
extern void *D_800E0930;
extern void (*D_800E0934)(s32 x, s32 y, s32 z); // bullet impact hook (func_151C1654), set per level
extern void *D_800E0940;
extern u8   D_800E0A10;
extern StreamVoice D_800E0D80; // the streaming music player's voice

// end of zero'd out section

extern s32 *D_800E0E20; // Huffman table pointers, relocated by D_800E0E28
extern void *D_800E0E28; // the Huffman table data
extern f32 *D_800E0E30; // pow43 table storage (0x8000 bytes)
extern s32  D_800E9D00;
extern u8   D_800E9D10[]; // heap arena: from here to 0x80400000

/* debugger variables */

extern s32  D_16001B8C; // a struct
extern s8   D_160036F3; // control stick Y-axis ?
extern u8  *D_16003848[]; // exception names
extern u8   D_16003888;
extern u16  D_1600388C;
extern s32  D_160038A0;
extern u8   D_160038A4;
extern s32  D_160038A8;
extern s32  D_16003890;
extern s32  D_16003AF0;
extern u8   D_16003AF4;
extern s8   D_16003B1C;
extern u8  *D_16003B20[]; // "REGISTERS", "STACK"
extern s8   D_16003B28;
extern s32  D_16003B4C;
extern OSThread *D_1600389C; // faulted thread

extern u8   D_160046AC[]; // data
extern u8   D_160046D0[];
extern u8   D_160046DC[];
extern u8   D_160046E8[];
extern u8   D_160046F4[];
extern u8   D_16004700[];
extern u8   D_16004708[];
extern u8   D_16004710[];
extern u8   D_1600471C[];
extern u8   D_16004728[];
extern u8   D_1600472C[];
extern u8   D_16004734[];
extern u8   D_16004738[];
extern u8   D_16004740[];
extern u8   D_16004748[];
extern u8   D_160047B0[];
extern u8   D_160047BC[];
extern u8   D_160047C0[];
extern u8   D_160047D0[];
extern u8   D_160047D4[];
extern u8   D_160047DC[];
extern u8   D_160047E0[];

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
