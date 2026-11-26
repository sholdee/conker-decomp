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
extern u8 D_80084160[];
extern u8 D_80084190[];
extern Vtx  D_80084300[5];
extern void (*D_800844B0[])(s32 arg0);
extern s32  D_800848B0;

extern u8 (*D_80085990[])[0xE0]; // per-font glyph bitmaps, 0xE0 bytes each
extern FontGlyph *D_80085994[];
extern Gfx D_800859A0[]; // shared display list (2D/sprite render setup)
extern f32 D_80085FD0;
extern f32 D_80085FD4;
extern f32 D_80085FD8;
extern f32 D_80085FDC;
extern f32 D_80085FE0;
extern f32 D_80085FE4;
extern f32 D_80085FE8;


extern s32 D_80086000;
extern void (*D_80086004[])(s32);
extern void (*D_8008600C[])(s32);
extern void (*D_80086014[])(Actor *);
extern s16 D_800860C0[];
extern f32 D_800860CC[];
extern u16 D_800860E4[];
extern void (*D_80086150[])(s32);
extern void (*D_80086730[])(void);
extern u8 * D_80086CAC[];
extern s8  D_80087260;
extern s8  D_80087264;
extern s8 D_80087268;
extern s8  D_8008726C;
extern s8  D_80087270[];
extern u8  D_80087280[];
extern s16 D_80087290; /* node count */
extern u8 D_80087298; /* goal node reached by the last search, 0xFF if none */
extern u8 D_8008729C;
extern Racer *D_800872A0; // per-racer state of the race mode
extern u32 D_80087380;

extern Gfx D_800873D0[];
extern Gfx D_80087408[];
extern u64 D_800885B0;
extern Actor *D_80088720;
extern Actor *D_80088730;
extern struct Queue *D_80088750;
extern Vec3f D_80088724;
extern s32 D_80088890;
extern s32 D_80088894;
extern s32 D_80088898;
extern s32 D_8008889C;
extern u8  D_80088980;
extern VariantList D_80088B20[];
extern u8  D_80088B40;
extern u8  D_80088B60;
extern u8  D_80088B90;
extern u8  D_80088BB0;
extern ObjectBehavior D_80088C90[]; // level-object behaviour handlers
extern u8 D_80089120[4]; // per collision set: 1 = enabled
extern s32 * D_800891BC[];
extern DrawEntry **D_80089240[]; // per-view draw lists
extern Mtx ** D_80089250[]; // per frame buffer: one Mtx per object
extern PropSounds D_80089260[]; // prop sound rows (doors, movers, sliders)
extern Mtx D_80089470; // fixed modelview matrix
extern s32 D_800894B0;
extern s32 D_80089520;
extern s32 D_80089524[]; // struct arrays?
extern s32 D_80089534[];
extern s16 D_80089550;
extern void (*D_80089AAC[10])(void);
extern void (*D_80089AD4[10])(void);
extern void (*D_80089C44[])(void);
extern void (*D_80089C54[])(void);
extern void (*D_80089EE8[])(void);
extern void (*D_80089EF0[])(void);
extern void (*D_80089EF8[])(void);
extern void (*D_80089F2C[])(void);
extern void (*D_80089F60[])(s32, s32, u8);
extern f32 D_800895B0;
extern ZDecode D_80089630[8]; // z-buffer decompression table
extern const s32 *D_80089A20[];
extern IdList2 D_8008A074; // effect-object kinds (game_16DC80)
extern ActorEffectDef D_8008A0B4[];
extern s32  D_8008A084[];
extern void (*D_8008A4E8[])(TimedEffect*);
extern void (*D_8008A4C0[])(TimedEffect*);
extern void (*D_8008A688[])(TimedEffect*);
extern void (*D_8008A7B0[])(TimedEffect*);
extern void (*D_8008A8D8[74])(TimedEffect*, s32, u8);
extern s32 (*D_8008A670[])(s32, TimedEffect*, s16);
extern s32  (*D_8008B0F0[])(void);
extern u8   (*D_8008B1F8[])(LightEffect *);
extern void (*D_8008B208[])(LightEffect *);
extern void (*D_8008B2B0[])(LightEffect *);
extern s32 (*D_8008B36C[])(void);
extern void (*D_8008B370[])(LightEffect *effect, s32 arg1, u8 eventId); // light event handlers
extern void (*D_8008B374[])(LightEffect *effect);

extern AttachmentType * D_8008ABE8[];
extern ObjType D_8008B4A8[OBJ_TYPE_COUNT];
extern EffectHeader *D_800DCE50[2][OBJ_TYPE_COUNT]; /* object lists: [list][type] heads a doubly linked list (game_1944C0.c) */


extern TextureDesc *D_8008CA4C[]; /* texture table: pointers to texture entries (indexed by the effects' animIndex / texture) */
extern u8   D_8008CEB0;
extern u8   D_8008CD00;
extern s32 D_8008CEB4; /* axis D_800DDD18 is sorted along: 0 = x, else z */
extern f32 *D_8008CFFC[]; // per LevelHeader.unk10: sun position (game_1AB530 FLARE_SUN)

extern WavePreset D_8008D050[]; // static parameters of each wave spring chain
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

extern f32 D_8009A220[];
extern f32 D_8009A420[];
extern f32 D_8009A620[];
extern HitReaction D_8009A6D8[]; // knockback reactions
extern HitReaction D_8009A9F8[]; // hit-reaction table

extern s16 D_8009DCB4[];
extern char D_8009DCC0[];
extern char D_8009DCC4[];
extern char D_8009DCD4[];
extern char D_8009DCD8[];
extern char D_8009DCE0[];

extern char D_8009DCCC[];

extern u8 D_8009DEB0[];
extern u8 D_8009DEB4[];
extern u8 D_8009DEB8[];
extern u8 D_8009DEBC[];
extern f32 D_8009F6C0; // 0.01745329238474369f

/* 800A0000 */
extern PipeSpot D_8009F920[3]; // the three exploding pipes
extern Vec3f D_800A1B80;
extern Vec3f D_800A1B8C;
extern const Vec3f D_800A1FB0;
extern const Vec3f D_800A1FBC;
extern u8  D_800A2CD0[][3];

extern s32 D_800A4AA0; //?
extern s32 D_800A5168[4]; // object list indices (game_16DC80)
extern s32 D_800A5178[3]; // effect-object kinds (game_169510)
extern s32 D_800A5760[]; // event types passed to func_15169260
extern s32 D_800A5770[];

// in game_18D770.c
extern s32 D_800A6060[3]; /* { 0x36, 0x4C, 0x5B }: the model effect types */


extern VtxMorphGroup D_800A7230[2]; // vertex morph groups
extern u16 D_800AB140[];
extern IdList1 D_800AB168; // effect-object kinds (game_1FFF60)

// end of data D_800B0DBC

/* 800B0000 */

extern u8 D_800AB62C[][3]; // team colours (RGB)
extern ModeEntry D_800AB68C[]; // multiplayer game-mode table
extern s32 D_800AB754[];
extern LevelHeader *D_800B0DF0;

extern s32 D_800BE5D0;
extern u8  D_800BE9A0;
extern f32 D_800BE9A4;
extern f32 D_800BE9A8;
extern s32 D_800BE9AC;
extern u8 D_800BE9EB;
extern s32 level;



extern Gfx *D_800BE9C8[2]; // display-list buffer per framebuffer (D_800BE9C0)
extern Gfx *D_800BE9D0;    // display-list write head
extern Gfx *D_800BE9D8[2]; // per framebuffer: end of the prologue built by func_1501A39C



extern u8   D_800BE9B4;
extern u8   D_800BE9C0; // which of the two matrix buffers is being built this frame
extern u16 *D_800BE9C4; // z-buffer (D_800BE620 x D_800BE624)
extern OSMesg D_800BE9E0;
extern s32  D_800BE9E4; // frames elapsed this update
extern s32  D_800BE9E8;
extern u8   D_800BE9EC;
extern u8   D_800BE9ED;
extern s16 D_800BE9F2;
extern s32 D_800BE9F4;   // current scene / game mode id
extern s32  D_800BE9F8;


extern s32 D_800BEA00;
extern s32 D_800BEA04;
extern s32 D_800BEA08;
extern u8 D_800BEA0C;
extern OSMesgQueue D_800BEA10;
extern OSMesg      D_800BEA28;
extern OSScMsg D_800BEA68[2]; // "task done" messages, one per frame buffer
extern s16 D_800BEAA8;
extern u8  D_800BEAAA;
extern u8  D_800BEAAB;
extern u8  D_800BEAC0;
extern u8  D_800BEAC1;

/* 800C0000 */

extern u8 D_800BEAC2;
extern u8 D_800BEAC3;
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
extern u8  dc_glbl_CharNum;
extern u16 D_800C3E7A; // bone matrices used this frame
extern u16 D_800C3E7C; // bone-matrix pool size, in 0x40-byte matrices
/* The bone-matrix pool of each frame buffer. Bones are built in it as MtxF; func_1502E474 converts them in
   place to Mtx for drawing. The cursors move in bytes. */
extern u8 *D_800C3E80[2];
extern u8 *D_800C3E88; // bone-matrix pool cursor
extern u8 *D_800C3E8C; // bone-matrix pool start
extern u8 D_800C3E90;
extern ModelAttachment *D_800C3EE0; // head of the model attachment list
extern Vtx *D_800C3FC0[]; // per actor: a 5-vertex quad buffer; [0] is the template D_80084300
extern s32 D_800C3FC4;
extern s64 D_800C3A60[];
extern u8  D_800C4000;

extern s32 * D_800C4020[]; // per-id first vertex of each part
extern u16 D_800C4310[]; // part count, indexed by VtxMorphGroup.kind
extern Gfx **D_800C4488[]; // per-model display-list parts
extern u16 D_800C4778[]; // display-list parts per model id
extern Gfx **D_800C48F0[]; // per model: display list table (ModelHeader dlists2)
extern void *D_800C4BE0[]; // per model: bone table (16-byte entries)
extern u16 D_800C4ED0[]; // bone count per model id
extern ObjTexture *D_800C5338[]; // per-model texture tables
extern u16 D_800C5628[]; // textures count
extern u16 D_800C57A0[]; // vertex count
extern u16 D_800C5918[]; // triangle weight, see func_1503D984
extern u16 D_800C5A90[187]; // per model: animation count
extern VtxRun *D_800C5C08[187]; // per model id: vertex runs (D_800C5EF8[id] of them)
extern u16 D_800C5EF8[];
extern VtxTri *D_800C6070[187]; // per model id: triangles
extern TexCoord *D_800C6360[187]; // per model id: saved texture coordinates (func_1503D804)

extern AimPoint *gAimPoints; // aim points (gAimPointCount of them)
extern s32 gAimPointCount;
extern ObjPartsState D_800C6660[];
extern u8  D_800C67F0;
extern u8  D_800C67F1;
extern s32 D_800C6860[];
extern u8  D_800C68A0;
extern u8  D_800C68A1;

extern u16 D_800CBD4E; // animation clock (high bits pick icon frames)
extern u16 D_800CBDA0[];
extern u8 D_800CBDD3;
extern u8 D_800CBDD4;
extern f32 D_800CBDF4;
extern f32 D_800CBDF8;
extern TriggerNode *D_800CBE00; // timed trigger nodes (func_15044964)
extern u8  D_800CBD60;
extern u8  D_800CBD61;
extern u8  D_800CBD62;
extern u8  D_800CBD63;
extern s16 D_800CBD70;
extern s16 D_800CBD72;
extern u8  D_800CBD74;
extern Actor *D_800CBD78; // text queue: anchor actor for the next text
extern s16 D_800CBD7C;

extern f32 D_800CBD80;

extern f32 D_800CC210;
extern f32 D_800CC214;
extern f32 D_800CC218;
extern f32 D_800CC21C;
extern f32 D_800CC220;
extern f32 D_800CC224;
extern f32 D_800CC228;
extern f32 D_800CC22C;
extern f32 D_800CC230;
extern f32 D_800CC234;
extern f32 D_800CC238;
extern f32 D_800CC23C;

extern s32 D_800CC250;

extern f32 D_800CC254;
extern f32 D_800CC258;
extern f32 D_800CC25C;
extern s32 D_800CC260;
extern s16 D_800CC264;
extern s32 D_800CC268;
extern u8 D_800CC26C;
extern u8 D_800CC26D;
extern u8  D_800CC26E;
extern f32 D_800CC270; // attacker position saved at the hit (x, z, y)
extern f32 D_800CC274;
extern f32 D_800CC278;
extern u8  D_800CC27C;

extern f32 D_800CC2C0;
extern f32 D_800CC2C4;
extern f32 D_800CC2C8;

extern Actor movement_obj[26]; // objects
extern u8  D_800CC2D4[];
extern f32 D_800CC2E4;
extern f32 D_800CC2E8[];
extern f32 D_800CC2EC;
extern f32 D_800CC30C[];
extern u8  D_800CC335;
extern u16 D_800CC34A[];
extern u8  D_800CC37D;
extern u16 D_800CC382;
extern u8  D_800CC3D4[];
extern s8  D_800CC3D7;
extern u8  D_800CC3F5[] ; // ??

extern u8 D_800CC49A;
extern u8 D_800CC4B3[][0x32C]; /* byte 0x1E3 of each movement_obj object */
extern Actor  D_800CC5A0[];
extern u8  D_800CC521[]; // ??

extern Actor *D_800CE370;

/* 800D0000 */

extern Actor D_800D121C; // == &movement_obj[25]: dummy attacker for scripted hits; also the end of the 25 live actors
extern f32 D_800D1230;
extern f32 D_800D1234;
extern f32 D_800D1238;
extern s16 D_800D1292;
extern s16 D_800D1296;
extern s8  D_800D12EC;
extern f32 D_800D1330;
extern u8  D_800D1340;
extern s8  D_800D1359;
extern f32 D_800D1368;
extern f32 D_800D136C;

extern s32 D_800D1510;
extern f32 D_800D1550; // frame time step
extern s32 D_800D1580; // parameter of the animation event being run (D_800D1874: the same bits as an f32)
extern s16 D_800D1582;
extern u8  D_800D1583;
extern ActorAnim *D_800D1588[187]; // per model id: animation table, after its AnimHeader (see func_1503D660)
extern s32 D_800D1548;
extern Actor *D_800D154C;

extern f32 D_800D1874;
extern f32 D_800D1878;
extern s32 D_800D1880;
extern u8  D_800D1890;
extern u8  D_800D1891;
extern u8  D_800D1892;
extern u8  D_800D1893;
extern f32 D_800D1898[];
extern u16 D_800D18A0;
extern u16 D_800D18A2;
extern u8  D_800D18A4[4]; // per player: half-size of the square, minus 4 (game_A9D90)
extern u8  D_800D18A8;
extern u8 D_800D18A9;
extern BoundCylinderSet *D_800D18B0; // per-level actor-confining cylinder sets (func_150163D0 loads, func_15055E50 uses)
extern u8  D_800D18C0;
extern u8  D_800D18D0;

extern u8  D_800D1940;
extern u8  D_800D1941;
extern TextResource *D_800D1944; // current text
extern TextResource *D_800D1948; // header text prepended to it, or NULL
extern u8 *D_800D194C; // header + page break + current text
extern TextBox *D_800D1950; // open text box
extern TextBoxConfig D_800D1958;
extern f32 D_800D1988;
extern f32 D_800D198C;
extern f32 D_800D1990;
extern u8  D_800D1994;
extern u8  D_800D1995;
extern u16 *D_800D1998; // sound effects voiced while the text types out

extern u8 *D_800D19A0[187]; // per model id: model data (just past its header)
extern ModelDef *D_800D1C90[187];
extern u8  D_800D1F80[187];
extern u8  D_800D2040[187]; // per model id counter
extern SpawnRecord *D_800D20FC;

extern u8 D_800D2100;
extern u8 D_800D2101;
extern PathPoint **D_800D2104;
extern u8 *D_800D2108;
extern u8 * D_800D210C;
extern s8 D_800D2130;
extern PathNode *D_800D2350; // the level path graph nodes (func_15085B70)
extern s32 D_800D2354;
extern f32 D_800D239C;
extern f32 D_800D23A0;
extern f32 D_800D23A4;
extern s8 D_800D23A8;
extern s32 D_800D2450;
extern s32 D_800D2C9C; // G_TT_RGBA16 when the last loaded texture has a palette, else G_TT_NONE
extern u8 *D_800D2CA0; // current texture frame (0x80000000 = missing asset)
extern MtxF D_800D2CA8[]; // view matrix per player
extern u8 D_800D2DA8; // sprite prim colour r, g, b (used while D_800D2DAB is 0)
extern u8 D_800D2DA9;
extern u8 D_800D2DAA;
extern u8  D_800D2DAB;
extern u16 *D_800D2110;
extern s32 D_800D2120;
extern s32 D_800D2124;
extern s32 D_800D2128;
extern SpawnPoint *D_800D212C;
extern s16 D_800D2132;
extern MatchState D_800D2138;

extern u16 D_800D2340;
extern s8  D_800D23A9;
extern ObjGroup *D_800D23C0;
extern Mtx  D_800D23D0;

extern f32 D_800D2410[];
extern f32 D_800D2428[3];
extern f32 D_800D2438[3];
extern s32 D_800D2444;
extern Gfx *D_800D2448;
extern Mtx  *D_800D244C;
extern u16  D_800D2454; // perspNorm for D_800D23D0
extern u8  D_800D2456;
extern u8  D_800D2457;
extern u8  D_800D2458;
extern BlockDebris *D_800D245C;
extern Bubble D_800D2460[6];
extern u8  D_800D246D;
extern u8  D_800D247D;
extern s32 D_800D2548;
extern Gfx *D_800D2588; // display list built by game_BC510
extern Mtx  D_800D2590[6][2];
extern s32 D_800D24C0;
extern WaterStage D_800D24C8;

extern f32 D_800D2890[3];

extern u8  D_800D2C68;
extern s32 D_800D2C28[16]; // segment base addresses

extern u8 D_800D2E40;
extern s32 D_800D2E48;
extern s32 D_800D2E50;
extern u8  D_800D2E60[];
extern u8  D_800D2E68;
extern u8 D_800D2E69;
extern u8 D_800D2E70[0xF0]; // one state byte per id
extern u8  D_800D2E41;
extern u8  D_800D2E42;
extern u8  D_800D2E43;
extern u8  D_800D2E44;
extern u8  D_800D2E45;
extern u8 *D_800D2E4C; // 0x1B-byte bit array of per-id flags (saved with the game)

extern u8  D_800D2F3C;
extern u16 *D_800D2F40;
extern IdRecordList gTaskWorkingSet;
extern f32 D_800D2DB0;
extern s32 D_800D2DB4;

extern s32 D_800D2F58;
extern u32 gZoneCount; // number of TriggerZones in zones
extern TriggerZone *zones; // level placement / marker table (0x34-byte records)
extern ZoneBounds *D_800D309C; // bounds of each trigger zone (zones)
extern u64 D_800D30A0[3]; /* TriggerZones that are always tested */
extern u64 D_800D30B8[3]; /* default candidate TriggerZones */
extern u16 D_800D30D0[16]; /* per actor: 1 + index of the last zone box it was found in, 0 for none */
extern u64 D_800D30F0[16][3]; /* per actor: TriggerZones it was inside last time */
extern TriggerZone ** D_800D3270;
extern s32 D_800D3274;
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

extern EffectHeader *D_800D98D0[4];
extern struct Queue *D_800D98E0;
extern u8  D_800D987C;

extern u8 D_800D98F0;
extern u8 D_800D9900;
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

extern u8 D_800D9950[];
extern AimImpact D_800D9960[]; // aim-impact slots (func_150E4550 / func_150E4E04 index them; 0xFF = none)
extern struct HomingEffect *D_800D99D0[8]; // live homing effects by handle - 1 (defined in game_1104D0)
extern TriggerZone *D_800D9A20[2]; // registered by func_15014040 (by unk18)
extern ZoneNode *D_800D9A40; // zone at (0x1500, 0x700); func_150F33F8 sets its unk0 bits per player
extern Vec3f D_800D9A50;
extern TriggerZone *D_800D9AA0[3];
extern u16 D_800D9AB0[4];
extern Vec3f D_800D9AC0[];
extern u8 D_800D9AF0;
extern ViewClip gViewClips[];

extern u8  D_800D9B18;
extern f32 D_800D9B1C;
extern f32 D_800D9B20;
extern u8  D_800D9B68[4][3];
extern u8  D_800D9B78[4][3];
extern u8  D_800D9B84[];
extern u8  D_800D9B88[];
extern u8  D_800D9B8B;
extern u8  D_800D9B8C;
extern LookAt D_800D9B90[];
extern Ambient D_800D9BD0[][2]; // per view and framebuffer
extern PointLightGfx *D_800D9E10[];
extern u8  D_800D9E20;

// extern u16 D_800D9E70[][3];
// extern u8 D_800D9E88[][3];
// extern u8 D_800D9E98[][3];
// extern u8 D_800D9EA8[][3];
// extern u8 D_800D9EB4[][3];
// extern u8 D_800D9EB8[][3];
//
// extern u8 D_800A2D04[][3];
// extern u8 D_800A2CE8[][3];
// extern u8 D_800A2CEC[][3];

extern MtxF D_800D9C10[]; // per view: view matrix
extern MtxF D_800D9D10[]; // per view: view matrix * projection
extern u8 D_800D9E21; // light count byte func_1515D914 stored for player 0
extern PointLight *D_800D9E28[]; // point lights func_1515D914 picked this frame
extern u8  D_800D9E64;
extern u16 D_800D9E70[4][3];
extern u8 D_800D9E88[4][3];
extern u8 D_800D9E98[4][3];
extern u8 D_800D9EA8[4][3];
extern u8 D_800D9EB4[3];
extern u8 D_800D9EB8[4][3];
extern s32 D_800D9F58;
extern s32 D_800D9F5C;
extern u8  D_800D9F60;

extern u8 D_800D9F68[]; /* per-id reference count */
extern void *D_800DBDD8[]; // per map: triangle table, x/z bounds, y bounds (carved from D_800B0DC0)
extern void *D_800DBDE8[];
extern void *D_800DBDF8[];

extern struct QuadNode *D_800DBE08[];
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
extern void *D_800DBE80; // the current sky data (star field, tile formats or gradient texture)
extern Vtx * D_800DBE88[][2]; // per view, per framebuffer
extern u8 D_800DBEA8[];
extern u8 D_800DBEAB;
extern u8 D_800DBEAC;
extern Mtx *D_800DBEB0; // sky projection matrices, 2 per view
extern u16 D_800DBEE8[]; // per view: entries in D_80089240
extern s32 gWorldObjectCount; // number of db_Object_array entries
extern WorldObject *db_Object_array; // original name (Oct 2000 debugger); gWorldObjectCount entries
extern void **D_800DBEF8; // per object: handle released with func_10004074
extern u8 *D_800DBEFC; // per object: frames to keep the handle once hidden
extern u8  D_800DBF00;
extern MtxF D_800DBF08[]; // per-frame
extern u8  D_800DBF88;
extern Gfx *D_800DBF8C; // display list of object D_800DBF88 (func_15004574)
extern Vtx *D_800DBF90; // its source vertices
extern s32 * gWorldObjectContacts; // per object: bit N set when actor movement_obj[N] touches it
extern s32 D_800DBF98; // grouped objects, kept at the end of db_Object_array
extern Camera *camera_global_Cameras;
extern u8  D_800DBFF4[4]; // per-player camera hold timers; *(s32 *)D_800DBFF4 tests all four at once
extern u8  D_800DBFF5;
extern u8  D_800DBFF6;
extern u8  D_800DBFF7;

extern f32 D_800DC004; // camera transition height (see func_15128030)
extern Camera *D_800DC010; // saved cameras (one per view)
extern CamPose *D_800DC020; // the level's camera pose table (D_80089550 entries)
extern CamPose *D_800DC024; // the pose game_15BD90 is moving to
extern Mtx  *D_800DC2A0[2]; // per framebuffer: one view matrix per view
extern CamMsgQueue *D_800DC2B0; // one per camera
extern ViewEnvelope D_800DC2C0[4]; // per-view camera wobble envelope (game_15ABA0.c)
extern s32 D_800DC9F0; /* live glow count */
extern u8 D_800DCA20;
extern f32 D_800DCA24;
extern u8 D_800DCA28;
extern f32 D_800DCA2C;
extern BurstPoint D_800DCA30[3][10]; // random burst points around each pipe (func_150124A0 fills, func_15156B54 uses)
extern Mtx D_800DCC10[]; // per-view orthographic projection (func_151EF954)
extern ParticleNode *D_800DC460; // loaded particle models: list head
extern ParticleNode *D_800DC464; // list tail
extern s16 D_800DC468[];
extern s32 D_800DC63C;
extern u8 D_800DCD27; // priority of the current ambient colour
extern s8 D_800DCD30[4][3]; // per-player directional light direction
extern u8  D_800DCD31;
extern u8  D_800DCD32;
extern u8  D_800DCD33;
extern u8  D_800DCD34;
extern u8  D_800DCD35;
extern u8  D_800DCD36;
extern u8  D_800DCD37;
extern u8  D_800DCD38;
extern u8  D_800DCD39;
extern u8  D_800DCD3A;
extern u8  D_800DCD3B;
extern u8  D_800DCD3C;
extern u8  D_800DCD3D;
extern PointLightGfx D_800DCD40; // padding light for odd light counts
extern PointLight *D_800DCD78; // list of all point lights
extern s32 D_800DCD7C;
extern ZoneLink *D_800DCD80; // ambient colour TriggerZones
extern ZoneLink *D_800DCD84; // tint TriggerZones
extern f32 D_800DCD90;
extern f32 D_800DCD94;
extern f32 D_800DCD98;
extern f32 D_800DCD9C;
extern f32 D_800DCDA0;
extern f32 D_800DCDA4;
extern f32 D_800DCDA8;
extern f32 D_800DCDAC;
extern f32 D_800DCDB0;
extern f32 D_800DCDB4;
extern f32 D_800DCDB8;
extern f32 D_800DCDBC;
extern f32 D_800DCDC0;

extern u8 D_800DCD20[3]; // ambient light colour

extern u8 D_800DCD24[3]; // tint colour
extern u8 D_800DCD28[3];

extern RegionNode *D_800DCDC4; // spawn regions; their areas add up to D_800DCD90
extern u8   D_800DCDD0;
extern CamOffset D_800DCDE0[4]; // per-player camera offset (position, rotation), summed from the wobbles (game_191C30.c)
extern s8   D_800DD190;
extern void *D_800DD198[6]; // per object-list walk depth (D_800DD190): the next node to visit
extern s32 D_800DD1B0; // texture last loaded by func_1517A9A8
extern s32 D_800DD1B4;
extern s32 D_800DD1B8;
extern s16 D_800DD1BC;
extern s16 D_800DD1BE;
extern s16 D_800DD1C0;
extern s16 D_800DD1C2;
extern s16 D_800DD1C4;
extern s16 D_800DD1C6;
extern s16 D_800DD1C8;
extern s16 D_800DD1CA;
extern s16 D_800DD1CC;
extern s16 D_800DD1CE;
extern f32  D_800DD1D8[];
extern f32  D_800DD1E8[];

extern s32 D_800DD1FC;
extern s32 D_800DD200;
extern s16 D_800DD204;
extern s16 D_800DD206;
extern s32 D_800DD208;
extern s32 D_800DD20C;
extern s32 D_800DD210;
extern SpriteParams *D_800DD214; // compared by func_15142E24, but never set to its sprite
extern s32 D_800DD218;
extern s32 D_800DD21C;
extern s32 D_800DD220;
extern s32 D_800DD224;
extern TextureDesc *D_800DD228; // texture of the current draw hook (func_151668B8, func_15166F6C)
extern SpriteParams D_800DD230;
extern MessageEntry *D_800DD254;
extern MessageEntry *D_800DD258;
extern u8   D_800DD2C0[16]; // per player: spawn point
extern u8   D_800DD2D0;
extern u8   D_800DD2A0[4];

extern u8 D_800DD2E4[];
extern u16 D_800DD2E8;
extern u16 D_800DD2EC;
extern s16 D_800DD2F2;
extern s16 D_800DD2F4;
extern u8   D_800DD404;
extern u8   D_800DD405;
extern u8   D_800DD406;
extern WaveDeformer *D_800DD410[2]; // D_800DD418 of them in use
extern u8   D_800DD418;
extern s16  D_800DD444; // state timer
extern VtxMorphFrame **D_800DD460[2]; // [group][part][frame]
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
extern u8   D_800DDE50;
extern u8 D_800DDE54[];
extern WaveSample *D_800DDE60[]; // samples of each active wave chain (40 each)
extern EffectModelSlot D_800DDE80[11];

extern u8 D_800DF0E0;
extern u8 D_800DF0E1;
extern MtxS16 *D_800DF7C0; // bone matrices drawn by func_1502C974 mode 4
extern u8 D_800DF7C4; // alpha scale for func_1502C974 mode 4
extern BoneTrail *D_800DF7C8[2]; // [0]: Actor owners, [1]: world object owners
extern u8   D_800DDF5C[];
extern u8   D_800DDF60[];
extern s8   D_800DDF68[];
extern Gfx *D_800DDF78[]; // per effect model slot: the model's second display list

/* 800E0000 */
extern u8 D_800DF7D0[]; // per-model load reference counts
extern s16 *D_800DF9B8[]; // per model: written by func_1510CE60
extern AttachModelHeader *D_800E0148[]; // per model: loaded attachment model header
extern struct TrailTask *D_800E0920; // type in 1CA420
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
extern u8   D_800E0950[20]; // reference counts (game_1FFF60)
extern s32  D_800E0964;
extern u16  D_800E0968[]; // anchor count per set (D_800E0990)
extern struct BoneAnchor *D_800E0990[]; // anchor set per resource (type in 1BC650)
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
