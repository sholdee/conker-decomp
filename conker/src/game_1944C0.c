#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/effect.h describes it. */
EffectHeader *D_800DCE50[2][OBJ_TYPE_COUNT];
s8 D_800DD190;
void *D_800DD198[6];
s32 D_800DD1B0;
s32 D_800DD1B4;
s32 D_800DD1B8;
s16 D_800DD1BC;
s16 D_800DD1BE;
s16 D_800DD1C0;
s16 D_800DD1C2;
s16 D_800DD1C4;
s16 D_800DD1C6;
s16 D_800DD1C8;
s16 D_800DD1CA;
s16 D_800DD1CC;
s16 D_800DD1CE;
f32 D_800DD1D0[2];
f32 D_800DD1D8[4];
f32 D_800DD1E8[4];
static s16 D_800DD1F8;
s32 D_800DD1FC;
s32 D_800DD200;
s16 D_800DD204;
s16 D_800DD206;
s32 D_800DD208;
s32 D_800DD20C;
s32 D_800DD210;
SpriteParams *D_800DD214;
s32 D_800DD218;
s32 D_800DD21C;
s32 D_800DD220;
s32 D_800DD224;
TextureDesc *D_800DD228;

#include "functions.h"
#include "variables.h"


/*
 * While a list is being walked, the next node is saved in D_800DD198[depth] (depth D_800DD190) so that
 * callbacks may destroy the current node; func_151696DC repairs saved entries that point at a dying node.
 */

/* Type 0x10 object (0xF0 bytes): a display list drawn with a per-frame matrix (created by func_15168BE4). */
struct ModelFx {
    /* 0x00 */ EffectHeader link;
    /* 0x10 */ Mtx mtx[2];      /* double-buffered by D_800BE9C0 */
    /* 0x90 */ ModelFxData data;
};

/* Type 3 object (0x28 bytes): a fading, animated billboard sprite. */
struct SpriteFx {
    /* 0x00 */ EffectHeader link;
    /* 0x10 */ TextureDesc *anim;
    /* 0x14 */ s16 frame;       /* 8.8 fixed point index into the frame table at anim->addr */
    /* 0x16 */ s16 frameStep;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 z;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s8 fadeStep;     /* > 0: fade out by this per frame; < 0: fade on the last frame */
    /* 0x23 */ u8 alpha;
    /* 0x24 */ u8 callback;     /* index into D_8008CA20, 0 = none */
};

/* Render setup of object type 14 (QuadFx). */
static Gfx D_8008B480[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPClearGeometryMode(G_CULL_BOTH | G_FOG | G_LIGHTING | G_LOD),
    gsSPEndDisplayList(),
};

/* Render teardown of object type 16 (ModelFx): back to the fixed modelview matrix. */
static Gfx D_8008B498[] = {
    gsSPMatrix(&D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPEndDisplayList(),
};

/*
 * The object types: per type its update, draw, event and destroy handlers and the display lists drawn
 * around its objects (see ObjType). Entries: { unk0, unk4, draw, unkC, drawBeginDL, drawBegin, unk18,
 * handleEvent, drawEnd, drawEndDL, destroy, free, drawMode }.
 */
static ObjType D_8008B4A8[OBJ_TYPE_COUNT] = {
    /*   0 */ { func_15169824, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*   1 */ { func_15168A2C, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*   2 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*   3 */ { func_15167B44, NULL, func_15167C58, NULL, D_800873D0, NULL, NULL, NULL, NULL, D_80087408, NULL, NULL, 1 },
    /*   4 */ { func_15166204, NULL, func_15166268, NULL, D_8008B3A0, func_151668B8, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*   5 */ { func_15167E0C, NULL, func_15168118, NULL, D_800873D0, func_15169668, NULL, NULL, NULL, D_80087408, NULL, NULL, 1 },
    /*   6 */ { func_15187978, NULL, func_15187A98, NULL, D_8008D4F0, NULL, NULL, NULL, NULL, D_8008D520, NULL, NULL, 1 },
    /*   7 */ { func_151B9CB0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0 },
    /*   8 */ { func_1517A1EC, NULL, func_1517A3A0, NULL, D_8008CE20, func_1517A958, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*   9 */ { func_1517A84C, NULL, func_1517A3A0, NULL, D_8008CE20, func_1517A958, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  10 */ { func_15175958, NULL, func_15176DF0, NULL, NULL, func_15183BA4, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  11 */ { func_150E1D14, NULL, func_150E28DC, NULL, D_800889E0, NULL, NULL, func_150E2FC0, func_150E2F90, NULL, NULL, NULL, 1 },
    /*  12 */ { func_1517A1EC, NULL, func_1517A3A0, NULL, D_8008CE40, func_1517A958, NULL, NULL, func_1517AB7C, D_80087408, NULL, NULL, 1 },
    /*  13 */ { func_15166B50, NULL, func_15166D68, NULL, D_8008B440, func_15166F6C, NULL, NULL, func_15166FD8, NULL, NULL, NULL, 1 },
    /*  14 */ { func_15168870, NULL, func_150A6860, NULL, D_8008B480, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  15 */ { func_15171FC0, NULL, func_151720C4, NULL, D_8008CC30, NULL, NULL, NULL, NULL, D_8008CC58, NULL, NULL, 0 },
    /*  16 */ { func_15168BAC, NULL, func_15168C4C, NULL, NULL, NULL, NULL, NULL, NULL, D_8008B498, NULL, NULL, 0 },
    /*  17 */ { func_1516DB90, NULL, NULL, NULL, NULL, NULL, func_1516D400, NULL, NULL, NULL, NULL, NULL, 0 },
    /*  18 */ { func_1516D678, NULL, func_1516D738, NULL, D_8008CB70, NULL, NULL, NULL, NULL, D_8008CB90, NULL, NULL, 1 },
    /*  19 */ { func_1517685C, NULL, func_151770C8, NULL, NULL, func_15183BA4, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  20 */ { func_15175958, NULL, func_15176DF0, NULL, NULL, func_15183BA4, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  21 */ { func_15176B84, NULL, func_15176DF0, NULL, NULL, func_15183BA4, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  22 */ { func_15176B84, NULL, func_15176DF0, NULL, NULL, func_15183BA4, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  23 */ { func_15176B84, NULL, func_15176DF0, NULL, NULL, func_15183BA4, NULL, NULL, NULL, NULL, NULL, NULL, 0 },
    /*  24 */ { func_15176B84, NULL, func_15176DF0, NULL, NULL, func_15183BA4, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  25 */ { func_151321D0, NULL, func_15132B80, func_15132A88, NULL, NULL, NULL, func_15133588, NULL, NULL, func_151323AC, func_151323F8, 1 },
    /*  26 */ { func_15189A00, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  27 */ { func_1513C060, NULL, func_1513CBF0, NULL, D_80089EB8, NULL, NULL, func_1513CF9C, NULL, D_80089ED8, func_1513C9B0, func_1513C9FC, 1 },
    /*  28 */ { func_1513CFE4, NULL, func_1513DB00, NULL, D_80089EB8, func_151406AC, NULL, func_1513DF9C, NULL, D_80089ED8, func_1513CB58, func_1513CBA4, 1 },
    /*  29 */ { func_1518AA10, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  30 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  31 */ { func_1518BD60, NULL, NULL, NULL, NULL, NULL, NULL, func_1518C850, NULL, NULL, NULL, NULL, -1 },
    /*  32 */ { func_15190898, NULL, NULL, NULL, NULL, NULL, NULL, func_15191980, NULL, NULL, func_15190840, func_1519086C, -1 },
    /*  33 */ { func_1518D6E0, NULL, NULL, NULL, NULL, NULL, NULL, func_1518F49C, NULL, NULL, func_1518E4CC, func_1518E4F8, 0 },
    /*  34 */ { func_15147740, NULL, func_15147C4C, NULL, D_8008A1D0, NULL, NULL, func_15147D1C, NULL, D_8008A1F0, func_151479E0, func_15147A30, 1 },
    /*  35 */ { func_15149264, NULL, func_15149490, NULL, D_8008A470, NULL, NULL, func_15149434, NULL, D_8008A498, func_15149394, func_151493E4, 1 },
    /*  36 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  37 */ { func_150C0C38, NULL, NULL, NULL, NULL, NULL, NULL, func_150C09B0, NULL, NULL, func_150C09F0, func_150C0A1C, -1 },
    /*  38 */ { func_1519EA04, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  39 */ { func_150E3738, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  40 */ { func_151342BC, NULL, func_151346D0, NULL, NULL, NULL, NULL, func_151347CC, NULL, NULL, func_1513472C, func_1513477C, 0 },
    /*  41 */ { func_15134E48, NULL, NULL, NULL, NULL, NULL, NULL, func_15135480, NULL, NULL, func_1513532C, func_151353A8, -1 },
    /*  42 */ { func_151349D0, NULL, NULL, NULL, NULL, NULL, NULL, func_15134C98, NULL, NULL, NULL, NULL, -1 },
    /*  43 */ { func_151303EC, NULL, func_15130A9C, NULL, D_80089700, NULL, NULL, func_15131C2C, NULL, D_80089730, func_1513177C, func_151317C8, 1 },
    /*  44 */ { func_151563B8, NULL, func_151564F8, NULL, D_8008AD40, NULL, NULL, NULL, NULL, NULL, func_15156FB8, func_15156FE4, 1 },
    /*  45 */ { func_151A7A90, NULL, func_151A7D6C, NULL, D_8008F910, NULL, NULL, func_151A8A20, NULL, D_8008F930, func_151A8584, func_151A85D4, 1 },
    /*  46 */ { func_15158D2C, NULL, NULL, NULL, NULL, NULL, NULL, func_15158FA4, NULL, NULL, func_15158CD4, func_15158D00, -1 },
    /*  47 */ { NULL, func_151AD174, func_151AD92C, NULL, NULL, NULL, NULL, func_151AE640, NULL, NULL, NULL, NULL, 1 },
    /*  48 */ { NULL, func_151ACB60, NULL, NULL, NULL, NULL, NULL, func_151ACB94, NULL, NULL, NULL, NULL, -1 },
    /*  49 */ { func_150C8A68, NULL, func_150C8DB8, NULL, D_80088828, NULL, NULL, NULL, NULL, D_80088848, NULL, NULL, 1 },
    /*  50 */ { func_1515BFA8, NULL, NULL, NULL, NULL, NULL, NULL, func_1515C0B8, NULL, NULL, func_1515BF50, func_1515BF7C, -1 },
    /*  51 */ { func_151B3184, NULL, func_151B32C8, NULL, D_8008FB18, NULL, NULL, func_151B3A34, NULL, D_8008FB40, func_151B4C6C, func_151B4C98, 1 },
    /*  52 */ { func_1515FFEC, NULL, NULL, NULL, NULL, NULL, NULL, func_15160090, NULL, NULL, NULL, NULL, -1 },
    /*  53 */ { light_updateEffect, NULL, NULL, NULL, NULL, NULL, NULL, light_handleEffectEvent, NULL, NULL, func_15161804, func_15161860, -1 },
    /*  54 */ { func_151572D0, NULL, func_15157420, NULL, D_8008AD60, NULL, NULL, func_15157D88, NULL, D_8008AD80, func_15157248, func_1515728C, 1 },
    /*  55 */ { func_15158224, NULL, func_151582C8, NULL, D_8008ADD0, NULL, NULL, func_15158B3C, NULL, D_8008ADF0, func_15158AA4, func_15158AD0, 1 },
    /*  56 */ { func_1513B798, NULL, func_1513B83C, NULL, D_80089BC0, NULL, NULL, func_1513BA78, NULL, D_80089C08, func_1513BA10, func_1513BA44, 1 },
    /*  57 */ { func_1516489C, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  58 */ { func_151787AC, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, func_15178DA4, func_15178DA4, 0 },
    /*  59 */ { func_15167E0C, NULL, func_15178750, NULL, D_800873D0, func_15169668, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  60 */ { screen_updateOverlay, NULL, screen_drawOverlay, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0 },
    /*  61 */ { func_151D2C40, NULL, NULL, NULL, NULL, NULL, NULL, func_151D2E5C, NULL, NULL, func_151D2DCC, func_151D2E14, -1 },
    /*  62 */ { func_151D2F90, NULL, NULL, NULL, NULL, NULL, NULL, func_151D324C, NULL, NULL, func_151D31F4, func_151D3220, -1 },
    /*  63 */ { func_151D8A24, NULL, NULL, NULL, NULL, NULL, NULL, func_151D8D5C, NULL, NULL, func_151D8B88, func_151D8BB4, -1 },
    /*  64 */ { func_1510608C, NULL, func_151067B8, NULL, D_80088BE0, NULL, NULL, func_15106F50, NULL, D_80088C00, func_15106EF8, func_15106F24, 1 },
    /*  65 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  66 */ { func_15167E0C, NULL, func_15168118, NULL, D_800873D0, func_15169668, NULL, NULL, NULL, D_80087408, NULL, NULL, 1 },
    /*  67 */ { func_15171FC0, NULL, func_151720C4, NULL, D_8008CC30, NULL, NULL, NULL, NULL, D_8008CC58, NULL, NULL, 0 },
    /*  68 */ { func_15175958, NULL, func_15176DF0, NULL, NULL, func_15183BA4, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  69 */ { func_15175958, NULL, func_15176DF0, NULL, NULL, func_15183BA4, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  70 */ { func_150C5568, NULL, func_150C56A4, NULL, D_80088768, NULL, NULL, func_150C5CC4, NULL, D_80088790, func_150C5470, func_150C54C0, 1 },
    /*  71 */ { func_151303EC, NULL, func_15130A9C, NULL, D_80089700, NULL, NULL, func_15131C2C, NULL, D_80089730, func_1513177C, func_151317C8, 1 },
    /*  72 */ { func_151321D0, NULL, func_15132B80, func_15132A88, NULL, NULL, NULL, func_15133588, NULL, NULL, func_151323AC, func_151323F8, 1 },
    /*  73 */ { func_1513CFE4, NULL, func_1513DB00, NULL, D_80089EB8, NULL, NULL, func_1513DF9C, NULL, D_80089ED8, func_1513CB58, func_1513CBA4, 1 },
    /*  74 */ { func_1513C060, NULL, func_1513CBF0, NULL, D_80089EB8, NULL, NULL, func_1513CF9C, NULL, D_80089ED8, func_1513C9B0, func_1513C9FC, 1 },
    /*  75 */ { func_1510DA84, NULL, func_1510E120, NULL, D_800890C0, NULL, NULL, NULL, func_1510E634, NULL, NULL, NULL, 1 },
    /*  76 */ { func_151572D0, NULL, func_15157420, NULL, D_8008AD60, NULL, NULL, func_15157D88, NULL, D_8008AD80, func_15157248, func_1515728C, 1 },
    /*  77 */ { func_15147740, NULL, func_15147C4C, NULL, D_8008A1D0, NULL, NULL, func_15147D1C, NULL, D_8008A1F0, func_151479E0, func_15147A30, 1 },
    /*  78 */ { NULL, func_151AD174, func_151AD92C, NULL, NULL, NULL, NULL, func_151AE640, NULL, NULL, NULL, NULL, 1 },
    /*  79 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  80 */ { func_1515589C, NULL, func_15155CFC, NULL, NULL, NULL, NULL, func_15156028, NULL, D_8008AD30, func_15155EF8, func_15155EF8, 0 },
    /*  81 */ { text_updateBox, NULL, text_drawBox, NULL, D_800873D0, NULL, NULL, NULL, NULL, D_80087408, NULL, NULL, -1 },
    /*  82 */ { func_151303EC, NULL, func_15130A9C, NULL, D_80089700, NULL, NULL, func_15131C2C, NULL, D_80089730, func_1513177C, func_151317C8, 1 },
    /*  83 */ { func_151563B8, NULL, func_151564F8, NULL, D_8008AD40, NULL, NULL, NULL, NULL, NULL, func_15156FB8, func_15156FE4, 1 },
    /*  84 */ { func_1513B798, NULL, func_1513B83C, NULL, D_80089BC0, NULL, NULL, func_1513BA78, NULL, D_80089C08, func_1513BA10, func_1513BA44, 1 },
    /*  85 */ { func_15158224, NULL, func_151582C8, NULL, D_8008ADD0, NULL, NULL, func_15158B3C, NULL, D_8008ADF0, func_15158AA4, func_15158AD0, 1 },
    /*  86 */ { func_1513CFE4, NULL, func_1513DB00, NULL, D_80089EB8, func_151406AC, NULL, func_1513DF9C, NULL, D_80089ED8, func_1513CB58, func_1513CBA4, 1 },
    /*  87 */ { func_15154C90, NULL, func_15154D80, NULL, D_8008AC70, NULL, NULL, func_15155564, NULL, D_8008AC98, NULL, NULL, 1 },
    /*  88 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  89 */ { func_1517A1EC, NULL, func_1517A3A0, NULL, D_8008CE40, func_1517A958, NULL, NULL, func_1517AB7C, NULL, NULL, NULL, 1 },
    /*  90 */ { screen_updateTintEffect, NULL, func_151A5130, NULL, D_800873D0, NULL, NULL, func_151A55D4, NULL, D_80087408, NULL, NULL, 1 },
    /*  91 */ { func_151572D0, NULL, func_15157420, NULL, D_8008AD60, NULL, NULL, func_15157D88, NULL, D_8008AD80, func_15157248, func_1515728C, 1 },
    /*  92 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  93 */ { func_15154C90, NULL, func_15154D80, NULL, D_8008AC70, NULL, NULL, func_15155564, NULL, D_8008AC98, NULL, NULL, 1 },
    /*  94 */ { func_15169988, NULL, func_15169A48, NULL, D_800873D0, func_15169668, NULL, NULL, NULL, D_80087408, NULL, NULL, 1 },
    /*  95 */ { func_15149264, NULL, func_15149490, NULL, D_8008A470, NULL, NULL, func_15149434, NULL, D_8008A498, func_15149394, func_151493E4, 1 },
    /*  96 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  97 */ { func_1515BAE0, NULL, func_1515BBF0, NULL, D_8008B050, NULL, NULL, NULL, NULL, D_8008B068, NULL, NULL, 1 },
    /*  98 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  99 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /* 100 */ { NULL, screen_updateFade, screen_drawFade, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /* 101 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /* 102 */ { func_1516DB90, NULL, NULL, NULL, NULL, NULL, func_1516D400, NULL, NULL, NULL, NULL, NULL, 0 },
    /* 103 */ { func_1516D678, NULL, func_1516D738, NULL, D_8008CB70, NULL, NULL, NULL, NULL, D_8008CB90, NULL, NULL, 1 },
};

/* Particle callbacks (BillboardTemplate.callback, QuadFx), declared without a prototype: each takes its
 * own particle type. */
static void (*D_8008C9C8[])() = {
    func_151B9964, func_151797B0, func_15168B44, NULL,
    NULL, func_15174920, func_15178268, func_1516A3F4,
    NULL, func_1518CCA8, func_150AECCC, func_150AED4C,
    func_150DE2C4, func_150E4CBC, func_15179CB0, func_151934B4,
    func_151A175C, func_151A1E34, func_151A24A8, func_151A25E0,
    func_151A77C0, func_151A787C,
};

/* Object callbacks (SpriteFx and ModelFx callback, BillboardTemplate onDeath, Sprite2D behavior),
 * declared without a prototype: each takes its own object type. */
void (*D_8008CA20[])() = {
    NULL, NULL, func_15169C70, func_1516A538,
    func_150B6DFC, func_150B6E3C, func_150B71A8, func_150B73F0,
    func_150B709C, func_150F5590, func_150F55C8,
};

/* The texture table: the effects' animIndex / texture indices select an entry. */
TextureDesc *D_8008CA4C[] = {
    &D_8009053C, &D_80090388, &D_800904FC, &D_8009052C,
    &D_800903C8, &D_800903D4, &D_8009066C, &D_80090660,
    &D_80090678, &D_80090684, &D_8009053C, &D_80090600,
    &D_80090634, &D_8009053C, NULL, &D_8009080C,
    &D_800907FC, &D_80090690, &D_800906C4, &D_800906F8,
    &D_800905C0, &D_800905A8, &D_800905B4, &D_800905CC,
    &D_800905D8, &D_800905E4, &D_800905F0, &D_80090420,
    NULL, NULL, NULL, NULL,
    &D_8009042C, &D_80090438, &D_80090444, &D_80090450,
    &D_80090494, &D_800904C0, &D_800904B4, &D_800904CC,
    &D_80090730, &D_8009075C, &D_800904D8, &D_800904E4,
    &D_800904F0, &D_80090508, &D_8009077C, &D_80090514,
    &D_8009078C, &D_80090798, &D_80090704, &D_80090710,
    &D_80090520, &D_800907EC, &D_80091834, &D_80091864,
    &D_80091870, &D_8009187C, &D_80091888, &D_80091894,
    &D_800918A0, &D_800918AC, &D_800918B8, &D_800918C4,
    &D_800918D0, &D_8009184C, &D_80091858, &D_8009045C,
    &D_80090468, &D_80090474,
};

/* Per-frame hooks run by func_1516706C. */
static void (*D_8008CB64[])(void) = {
    func_15164EE4, func_1515C158, func_151787A4,
};

/* Calls every object type's per-frame global hook (D_8008B4A8[i].unk18). */
void func_15167010(void) {
    ObjType *entry;
    ObjType *end;
    void (*func)(void);
    s32 unused;

    entry = D_8008B4A8;
    end = (ObjType *)((u32)entry + 101 * sizeof(ObjType));
    unused = 0;
    // matching: a goto loop plus the dead `unused` (saved s1) give the target's base-relative end
    // and sltu exit; for/while/do forms fold `end` to a constant address
loop:
    func = entry->unk18;
    if (func != NULL) {
        func();
    }
    entry++;
    unused ^= 0;
    if ((u32)entry < (u32)end) {
        goto loop;
    }
}

/* Calls each of the three hooks in D_8008CB64 that is set. */
void func_1516706C(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_8008CB64[i] != NULL) {
            D_8008CB64[i]();
        }
    }
}

/* Runs every type's unk0 callback on each object of both lists. */
void func_151670C0(void) {
    s32 list;
    s32 type;
    EffectHeader *node;

    for (list = 0; list != 2; list++) {
        for (type = 0; type != 101; type++) {
            if (D_8008B4A8[type].unk0 != NULL) {
                node = D_800DCE50[list][type];
                D_800DD190++;
                while (node != NULL) {
                    D_800DD198[D_800DD190] = node->next;
                    D_8008B4A8[type].unk0(node);
                    node = D_800DD198[D_800DD190];
                }
                D_800DD190--;
            }
        }
    }
}

/* Runs every type's unk4 callback on each object of both lists. */
void func_151671E8(void) {
    s32 list;
    s32 type;
    EffectHeader *node;

    for (list = 0; list != 2; list++) {
        for (type = 0; type != 101; type++) {
            if (D_8008B4A8[type].unk4 != NULL) {
                node = D_800DCE50[list][type];
                D_800DD190++;
                while (node != NULL) {
                    D_800DD198[D_800DD190] = node->next;
                    D_8008B4A8[type].unk4(node);
                    node = D_800DD198[D_800DD190];
                }
                D_800DD190--;
            }
        }
    }
}


/* Caches sin/cos of each player's unk3A0 angle, then runs every type's unkC callback on its objects. */
void func_15167310(void) {
    s32 player;
    f32 angle;

    for (player = 0; player < gScreenLayout + 1; player++) {
        angle = camera_global_Cameras[player].TrueCameraAngleRad;
        D_800DD1D8[player] = sinf(angle);
        D_800DD1E8[player] = cosf(angle);
    }
    angle = camera_global_Cameras->TrueCameraAngleRad;
    D_800DD1D0[0] = sinf(angle);
    D_800DD1D0[1] = cosf(angle);
    {
        s32 list;
        s32 type;
        EffectHeader *node;

        for (type = 0; type != OBJ_TYPE_COUNT; type++) {
            for (list = 0; list != 2; list++) {
                if (D_800DCE50[list][type] != NULL && D_8008B4A8[type].unkC != NULL) {
                    node = D_800DCE50[list][type];
                    D_800DD190++;
                    while (node != NULL) {
                        D_800DD198[D_800DD190] = node->next;
                        D_8008B4A8[type].unkC(node);
                        node = D_800DD198[D_800DD190];
                    }
                    D_800DD190--;
                }
            }
        }
    }
}


/*
 * Draws every object of list `list` whose type is in group `group` (see func_15168F84). Draw mode 1 types
 * drop any object that overflows the display list buffer; on overflow the pass is abandoned and the
 * display list is rewound to the start of the type that overflowed.
 */
Gfx *func_151674F8(Gfx *gfx, s32 group, s16 arg2, s32 list) {
    s32 type;
    s32 first;
    s32 last;
    EffectHeader **head;
    EffectHeader *node;
    s32 overflowed;
    Gfx *mode0Start;
    Gfx *prev;
    Gfx *mode1Start;
    s32 full;

    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    func_15168F84(group, &first, &last);
    for (type = first; type < last; type++) {
        if (D_8008B4A8[type].draw != NULL) {
            head = &D_800DCE50[list][type];
            if (*head != NULL) {
                D_8003C8E0 = (type & 0xFFFFFF) | 0x7000000;
                overflowed = FALSE;
                if (D_8008B4A8[type].drawMode == 0) {
                    mode0Start = gfx;
                }
                if (D_8008B4A8[type].drawMode == 1) {
                    mode1Start = gfx;
                }
                D_800D2DAB = 0;
                D_800DD228 = NULL;
                D_800DD220 = 0;
                D_800DD224 = 2;
                *(u32 *)&D_800DD1B0 = -1; // matching: an unsigned store keeps the -1 in its own register
                D_800DD1B4 = -1;
                D_800DD1B8 = -1;
                D_800DD1BC = 0;
                D_800DD1BE = 0;
                D_800DD1C0 = D_800DD1C2 = D_800DD1C4 = D_800DD1C6 = -1;
                D_800DD1C8 = D_800DD1CA = D_800DD1CC = D_800DD1CE = -1;
                D_800DD1F8 = 0;
                D_800DD1FC = 0;
                D_800DD200 = 0;
                D_800DD204 = -1;
                D_800DD206 = -1;
                D_800DD208 = -1;
                D_800DD20C = -1;
                D_800DD210 = -1;
                D_800DD214 = NULL;
                D_800DD218 = 0;
                D_800DD21C = 0;
                if (D_8008B4A8[type].drawBeginDL != NULL) {
                    gSPDisplayList(gfx++, D_8008B4A8[type].drawBeginDL);
                }
                if (D_8008B4A8[type].drawBegin != NULL) {
                    gfx = D_8008B4A8[type].drawBegin(gfx, type, list, arg2);
                }
                gSPClearGeometryMode(gfx++, G_LIGHTING);
                node = *head;
                if (D_8008B4A8[type].drawMode == 1) {
                    D_800DD190++;
                    while (node != NULL) {
                        prev = gfx;
                        D_800DD198[D_800DD190] = node->next;
                        gfx = D_8008B4A8[type].draw(gfx, node, arg2);
                        full = ((gfx - gDisplayListBuffers[D_800BE9C0]) > D_800BEBA4) ? TRUE : FALSE;
                        if (full) {
                            overflowed = TRUE;
                            gfx = prev;
                        }
                        node = D_800DD198[D_800DD190];
                    }
                    D_800DD190--;
                } else {
                    D_800DD190++;
                    while (node != NULL) {
                        D_800DD198[D_800DD190] = node->next;
                        gfx = D_8008B4A8[type].draw(gfx, node, arg2);
                        node = D_800DD198[D_800DD190];
                    }
                    D_800DD190--;
                }
                if (D_8008B4A8[type].drawEndDL != NULL) {
                    gSPDisplayList(gfx++, D_8008B4A8[type].drawEndDL);
                }
                if (D_8008B4A8[type].drawEnd != NULL) {
                    gfx = D_8008B4A8[type].drawEnd(gfx, type, arg2);
                }
                if (overflowed) {
                    full = ((gfx - gDisplayListBuffers[D_800BE9C0]) > D_800BEBA4) ? TRUE : FALSE;
                    if (full) {
                        gSPSetGeometryMode(gfx, G_ZBUFFER);
                        return mode1Start;
                    }
                }
                if (D_8008B4A8[type].drawMode == 0) {
                    full = ((gfx - gDisplayListBuffers[D_800BE9C0]) > D_800BEBA4) ? TRUE : FALSE;
                    if (full) {
                        gSPSetGeometryMode(gfx, G_ZBUFFER);
                        return mode0Start;
                    }
                }
                D_8003C8E0 = 0;
            }
        }
    }
    gSPSetGeometryMode(gfx++, G_ZBUFFER);
    return gfx;
}

/* Allocates a `size`-byte object (func_10003C6C) and links it into D_800DCE50[list][type]. */
void *func_15167A68(type, list, size, arg3, owner, arg5)
    s32 type;
    s32 list;
    s32 size;
    s32 arg3;
    u8 owner;
    u8 arg5;
{
    EffectHeader *obj;

    obj = (EffectHeader *)func_10003C6C(size, 1, arg3, 0, arg5);
    if (obj != NULL) {
        obj->kind = list;
        func_15168A4C(obj, type);
        obj->owner = owner;
    }
    return obj;
}

/* Spawns a type 3 SpriteFx from the 0x18-byte template at `src` (anim .. fadeStep), fully opaque. */
void func_15167AD8(SpriteFxTemplate *src, u8 owner, s32 list) {
    SpriteFx *fx;

    fx = func_15167A68(3, list, sizeof(SpriteFx), 0, owner, 1);
    if (fx != NULL) {
        bcopy(src, &fx->anim, 0x18);
        fx->alpha = 0xFF;
    }
}

/* Type 3 update: advances the animation and fade; destroys the sprite after its last frame. */
void func_15167B44(SpriteFx *fx) {
    s32 index;
    s32 fade;
    s32 rise;
    s32 frame;

    index = fx->callback;
    if (index != 0) {
        D_8008CA20[index](&fx->link);
    }

    fade = fx->fadeStep;
    if (fade > 0) {
        index = fx->alpha;
        if (fade < index) {
            fx->alpha = index - fade;
        } else {
            fx->frame = fx->anim->frameCount << 8;
        }
    } else if (fade < 0) {
        rise = -fade;
        frame = fx->frame;
        if (frame / 256 >= fx->anim->frameCount - 1) {
            index = fx->alpha;
            if (rise < index) {
                fx->alpha = index - rise;
                fx->frame = frame - fx->frameStep;
            }
        }
    }

    fx->frame += fx->frameStep;
    if (fx->frame / 256 >= fx->anim->frameCount) {
        func_1516972C(&fx->link);
    }
}
typedef struct {
    s32 unk0;
    s32 unk4;
} RenderModeEntry;
extern RenderModeEntry D_800A4AC8[];
typedef struct {
    s32 unk0;
    s32 unk4;
} PaddedFlag;
/* Type 3 draw: the current animation frame as a billboard. */
Gfx *func_15167C58(Gfx *gdl, SpriteFx *fx, u8 arg2) {
    SpriteParams params;
    Gfx *gfx;
    PaddedFlag update; // matching: 8-byte stack object; only the first word (the update flag) is used
    TextureDesc *anim;
    s32 renderMode;
    s16 frame;

    if (arg2) {} // matching: references the otherwise unused arg2
    update.unk0 = 1;
    frame = fx->frame;
    anim = fx->anim;
    gfx = func_15142E24(gdl, anim, frame << 8, 2, 0x100, 0x100,
                        ((s32 *)anim->addr)[frame >> 8], 5, &params, (u8 *)&update, 3);
    params.x = fx->x;
    params.y = fx->y;
    params.z = fx->z;
    params.scaleX = fx->unk1E;
    params.scaleY = fx->unk20;
    params.alpha = fx->alpha;
    params.flip = 0;
    // matching: fadeStep is an s8, so it never equals 0xFF; the target still tests both ways
    if (fx->fadeStep != 0xFF) {
        renderMode = 3;
    } else if (fx->fadeStep == 0xFF) {
        renderMode = 9;
    }
    gfx = func_15142FBC(gfx, 0x2C00, D_800A4AC8[renderMode].unk0 | D_800A4AC8[renderMode].unk4 | 4, (u8 *)&update);
    return func_15095760(gfx, &params);
}
/* Spawns a particle (type 0x42 when `altType` is set, else 5) from the 0x38-byte template at `src`. */
BillboardParticle *func_15167D84(src, altType, extra, onDeath, owner, list)
    BillboardTemplate *src;
    s32 altType;
    s32 extra;
    s8 onDeath;
    u8 owner;
    s32 list;
{
    BillboardParticle *particle;

    particle = func_15167A68((altType == 0) ? 5 : 0x42, list, extra + 0x50, 0, owner, 1);
    if (particle == NULL) {
        return particle;
    }
    bcopy(src, &particle->tmpl, sizeof(BillboardTemplate));
    particle->onDeath = onDeath;
    return particle;
}
/* BillboardParticle update: life, fade, motion and animation; runs onDeath and destroys it when finished. */
void func_15167E0C(BillboardParticle *p) {
    void (*func)(EffectHeader *);
    u8 finished;
    s32 limit;

    finished = 0;
    if (p->tmpl.life != 0x7FFF) {
        p->tmpl.life -= D_800BE9E4;
        if (p->tmpl.life < 0) {
            p->tmpl.life = 0;
            finished = 1;
        }
    }

    if (p->tmpl.fadeStep > 0) {
        if (p->tmpl.fadeStep < p->tmpl.primA) {
            p->tmpl.primA -= p->tmpl.fadeStep;
        } else {
            p->tmpl.life = 0;
            finished = 1;
        }
    }

    if (p->tmpl.callback != -1) {
        func = D_8008C9C8[p->tmpl.callback];
        if (func != NULL) {
            func(&p->link);
        }
    }

    if (p->tmpl.life != 0) {
        s32 posY;
        s32 posX;
        s32 posZ;
        s32 delta;

        p->tmpl.velY += p->tmpl.accelY;

        // matching: 24.8 fixed point moves written step by step (incl. the redundant (u8)) for IDO's temp registers
        posY = p->tmpl.y;
        posY = (posY << 8) | p->tmpl.subY;
        delta = p->tmpl.velY * D_800BE9E4;
        posY += delta;
        p->tmpl.y = posY >> 8;
        posY = (u8)posY;
        p->tmpl.subY = posY;
        posX = p->tmpl.x;
        posX = (posX << 8) | p->tmpl.subX;
        delta = p->tmpl.velX * D_800BE9E4;
        posX += delta;
        p->tmpl.x = posX >> 8;
        posX = (u8)posX;
        p->tmpl.subX = posX;
        posZ = p->tmpl.z;
        posZ = (posZ << 8) | p->tmpl.subZ;
        delta = p->tmpl.velZ * D_800BE9E4;
        posZ += delta;
        p->tmpl.z = posZ >> 8;
        posZ = (u8)posZ;
        p->tmpl.subZ = posZ;
        if (p->tmpl.frameStep != 0) {
            limit = (p->tmpl.anim->frameCount << 8) - 1;
            p->tmpl.frame += p->tmpl.frameStep * D_800BE9E4;
            if (limit < p->tmpl.frame) {
                if (p->tmpl.flags & BILLBOARD_DIE_AT_END) {
                    p->tmpl.life = 0;
                    finished = 1;
                } else {
                    p->tmpl.flags |= BILLBOARD_LOOPED;
                    if (p->tmpl.flags & BILLBOARD_PINGPONG) {
                        p->tmpl.frame = limit - (p->tmpl.frame % limit);
                        p->tmpl.frameStep = -p->tmpl.frameStep;
                    } else {
                        do {
                            p->tmpl.frame -= limit;
                        } while (limit < p->tmpl.frame);
                    }
                }
            } else if (p->tmpl.frame < 0) {
                if (p->tmpl.flags & BILLBOARD_DIE_AT_START) {
                    p->tmpl.life = 0;
                    finished = 1;
                } else if (p->tmpl.flags & BILLBOARD_PINGPONG) {
                    p->tmpl.frame = -p->tmpl.frame % limit;
                    p->tmpl.frameStep = -p->tmpl.frameStep;
                } else {
                    do {
                        p->tmpl.frame += limit;
                    } while (p->tmpl.frame < 0);
                }
            }
        }
    } else {
        finished = 1;
    }

    if (finished != 0) {
        if (p->onDeath != -1) {
            D_8008CA20[p->onDeath](&p->link);
        }
        func_1516972C(&p->link);
    }
}

/* BillboardParticle draw: sets render/combine modes and colours, then the current frame as a billboard. */
Gfx *func_15168118(Gfx *gfx, BillboardParticle *p, s16 viewIndex) {
    SpriteParams params;
    s32 view;
    s32 pad1; // matching: stack layout of the target frame
    u8 update;
    s32 pad2[3]; // matching: stack layout of the target frame
    f32 angle;
    s32 size;
    s32 unused; // matching: an unused local the target allocates
    s32 c0, c1, c2; /* colour components, reused for prim and env */
    TextureDesc *anim;
    TextureDesc *white;

    params.flip = 0;
    if (p->tmpl.flags & BILLBOARD_FLIP_S) {
        params.flip = 1;
    }
    if (p->tmpl.flags & BILLBOARD_FLIP_T) {
        params.flip |= 2;
    }
    // matching: two empty tests of the same condition steer IDO's scheduling of the texture load
    if (p->tmpl.anim->addr < 0x10000000) {
    }
    if (p->tmpl.anim->addr < 0x10000000) {
    }
    update = 1;
    {
        s32 texture;

        anim = p->tmpl.anim;
        if (anim->addr < 0x10000000) {
            texture = anim->addr;
        } else {
            texture = ((s32 *)anim->addr)[p->tmpl.frame >> 8];
        }
        gfx = func_15142E24(gfx, anim, p->tmpl.frame << 8, 2, 0x100, 0x100, texture, 5, &params, &update, 3);
    }
    view = viewIndex;
    params.x = p->tmpl.x;
    params.y = p->tmpl.y;
    params.z = p->tmpl.z;
    size = p->tmpl.width;
    if (size < 0) {
        /* 0.0111 = 1/90: fold the view angle into 0..90 degrees */
        angle = fabsf(camera_global_Cameras[view].TrueCameraAngle + (p->tmpl.unk8 & 0xFF) * 1.40625f);
        while (angle >= 360.0f) {
            angle -= 360.0f;
        }
        if (angle >= 180.0f) {
            angle -= 180.0f;
        }
        if (angle >= 90.0f) {
            angle = 180.0f - angle;
        }
        size = size * angle * -0.01111111138f;
        if (size < 0x400) {
            size = 0x400;
        }
    }
    params.scaleX = size;
    size = p->tmpl.height;
    if (size < 0) {
        view = (p->tmpl.width < 0) ? 0 : p->tmpl.width;
        size = fabsf(camera_global_Cameras[viewIndex].CameraPitch) * (view + size) * 0.01111111138f - size;
    }
    params.scaleY = size;
    if (p->tmpl.flags & BILLBOARD_CUSTOM_MODE) {
        u8 renderMode;
        u8 combineMode;
        s32 pad3[2]; // matching: stack layout of the target frame

        renderMode = p->tmpl.unk4 & 0xFF;
        combineMode = (p->tmpl.unk4 & 0xFFFF0000) >> 16;
        gfx = func_15142B7C(gfx, 5, 0);
        gfx = func_1513F4E4(gfx, combineMode, &update);
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | ((p->tmpl.flags & BILLBOARD_2CYCLE) ? G_CYC_2CYCLE : G_CYC_1CYCLE) | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[renderMode].unk0 | D_800A4AC8[renderMode].unk4 | 4, &update);
    } else if (p->tmpl.anim->fmt != 5) {
        gfx = func_1513F4E4(gfx, 0xD, &update);
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4 | 4, &update);
    } else if (p->tmpl.anim->fmt == 3) {
        gfx = func_1513F4E4(gfx, 0xE, &update);
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4 | 4, &update);
    } else if (p->tmpl.flags & 0x10) {
        gfx = func_1513F4E4(gfx, 0xD, &update);
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[0x12].unk0 | D_800A4AC8[0x12].unk4 | 4, &update);
    } else {
        gfx = func_1513F4E4(gfx, 0xF, &update);
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_2CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[0x11].unk0 | D_800A4AC8[0x11].unk4 | 4, &update);
    }
    c0 = p->tmpl.primG;
    c1 = p->tmpl.primB;
    c2 = p->tmpl.primA;
    gfx = func_15142CF0(gfx, 1, 0, p->tmpl.primR, c0, c1, c2, &update);
    if (p->tmpl.flags & BILLBOARD_ENV_COLOR) {
        c0 = p->tmpl.envR;
        c1 = p->tmpl.envG;
        c2 = p->tmpl.envB;
    } else {
        anim = p->tmpl.anim;
        white = &D_8009053C;
        if (p->tmpl.anim == &D_800907FC) {
            c0 = 0x80;
            c1 = 0x80;
            c2 = 0xFF;
        } else if (white == anim) {
            c0 = 0xFF;
            c1 = 0xFF;
            c2 = 0xFF;
        } else {
            c0 = 0;
            c1 = 0;
            c2 = 0;
        }
    }
    gfx = func_15142C10(gfx, c0, c1, c2, 0, &update);
    return func_15095760(gfx, &params);
}

/* Fills a QuadFxData template's vertex colours and texture coordinates from its animation. */
void func_1516865C(QuadFxData *q, u8 r, u8 g, u8 b, u8 a) {
    s16 tB;
    s16 tA;
    s16 sB;
    s16 sA;
    s32 i;

    tA = 0x2000;
    tB = (D_8008CA4C[q->animIndex]->height + 0x100) << 5;
    sA = 0x2000;
    sB = (D_8008CA4C[q->animIndex]->width + 0x100) << 5;
    for (i = 0; i < 2; i++) {
        q->vtx[i * 4 + 1].v.flag = 0;
        q->vtx[i * 4 + 1].v.cn[0] = r;
        q->vtx[i * 4 + 1].v.cn[1] = g;
        q->vtx[i * 4 + 1].v.cn[2] = b;
        q->vtx[i * 4 + 1].v.cn[3] = a;
        q->vtx[i * 4 + 2].v.flag = 0;
        q->vtx[i * 4 + 2].v.cn[0] = r;
        q->vtx[i * 4 + 2].v.cn[1] = g;
        q->vtx[i * 4 + 2].v.cn[2] = b;
        q->vtx[i * 4 + 2].v.cn[3] = a;
        q->vtx[i * 4 + 3].v.flag = 0;
        q->vtx[i * 4 + 3].v.cn[0] = r;
        q->vtx[i * 4 + 3].v.cn[1] = g;
        q->vtx[i * 4 + 3].v.cn[2] = b;
        q->vtx[i * 4 + 3].v.cn[3] = a;
        q->vtx[i * 4 + 0].v.flag = 0;
        q->vtx[i * 4 + 0].v.cn[0] = r;
        q->vtx[i * 4 + 0].v.cn[1] = g;
        q->vtx[i * 4 + 0].v.cn[2] = b;
        q->vtx[i * 4 + 0].v.cn[3] = a;
    }
    if (q->flags & 0x80) {
        tB = 0x2000;
        tA = (D_8008CA4C[q->animIndex]->height + 0x100) << 5;
    }
    if (q->flags & 0x100) {
        sB = 0x2000;
        sA = (D_8008CA4C[q->animIndex]->width + 0x100) << 5;
    }
    q->vtx[1].v.tc[0] = q->vtx[5].v.tc[0] = q->vtx[2].v.tc[0] = q->vtx[6].v.tc[0] = sA;
    q->vtx[2].v.tc[1] = q->vtx[6].v.tc[1] = q->vtx[3].v.tc[1] = q->vtx[7].v.tc[1] = tA;
    q->vtx[0].v.tc[0] = q->vtx[4].v.tc[0] = q->vtx[3].v.tc[0] = q->vtx[7].v.tc[0] = sB;
    q->vtx[0].v.tc[1] = q->vtx[4].v.tc[1] = q->vtx[1].v.tc[1] = q->vtx[5].v.tc[1] = tB;
    q->frame = 0;
    q->unk9C = 0;
    q->heading = 0;
}

/* Spawns a type 0xE QuadFx from a template prepared with func_1516865C. */
QuadFx *func_15168800(src, owner, list)
    QuadFxData *src;
    u8 owner;
    s32 list;
{
    QuadFx *fx;

    fx = func_15167A68(0xE, list, sizeof(QuadFx), 1, owner, 1);
    if (fx == NULL) {
        return NULL;
    }
    bcopy(src, &fx->data, sizeof(QuadFxData));
    return fx;
}

/* Type 0xE update: advances the animation (wrapping, ping-ponging or finishing) and runs its callback. */
void func_15168870(QuadFx *fx) {
    s32 frame;
    s32 limit;
    u8 frameCount;
    s16 step;
    void (*func)(EffectHeader *);

    step = fx->data.frameStep;
    if (step != 0) {
        frameCount = D_8008CA4C[fx->data.animIndex]->frameCount;
        limit = (frameCount << 8) - 1;
        fx->data.frame += step * D_800BE9E4;
        frame = fx->data.frame;
        if (limit < frame) {
            if (fx->data.flags & 0x40) {
                fx->data.frame = limit - (frame % limit);
                fx->data.frameStep = -step;
            } else if (fx->data.flags & 4) {
                fx->data.frame = -1;
            } else {
                do {
                    fx->data.frame = frame - limit;
                    frame = fx->data.frame;
                } while (limit < frame);
            }
        } else if (frame < 0) {
            if (fx->data.flags & 0x40) {
                fx->data.frame = -frame % limit;
                fx->data.frameStep = -step;
            } else if (fx->data.flags & 4) {
                fx->data.frame = -1;
            } else {
                do {
                    fx->data.frame = frame + limit;
                    frame = fx->data.frame;
                } while (frame < 0);
            }
        }
    }

    frame = fx->data.callback;
    if (frame != -1) {
        func = D_8008C9C8[frame];
        if (func != NULL) {
            func(&fx->link);
        }
    }

    if (fx->data.frame == -1) {
        func_1516972C(&fx->link);
    }
}
/* Moves an object to type 0. */
void func_15168A2C(EffectHeader *obj) {
    func_15168B10(obj, 0);
}
/* Pushes obj onto the front of list D_800DCE50[obj->kind][type] and sets its type. */
void func_15168A4C(EffectHeader *obj, s32 type) {
    s32 list;
    EffectHeader **head;

    list = obj->kind;
    head = (EffectHeader **)&D_800DCE50[list][type];
    obj->next = *head;
    if (obj->next != NULL) {
        obj->next->prev = obj;
    }
    obj->type = type;
    obj->prev = NULL;
    *head = obj;
}

/* Unlinks obj from its current list. */
void func_15168A9C(EffectHeader *obj) {
    s32 list = obj->kind;
    s32 type = obj->type;
    EffectHeader **head = (EffectHeader **)&D_800DCE50[list][type];

    if (obj == *head) {
        *head = obj->next;
    }
    if (obj->next != NULL) {
        obj->next->prev = obj->prev;
    }
    if (obj->prev != NULL) {
        obj->prev->next = obj->next;
    }
}

/* Moves obj to the list for another type. */
void func_15168B10(EffectHeader *obj, s32 type) {
    func_15168A9C(obj);
    func_15168A4C(obj, type);
}

/*
 * BillboardParticle callback: tmpl.unk4 holds a fade rate (high half) and a delay (low half). Keeps the particle alive
 * (life = 30) while the delay counts down, then fades primA by the rate each call and lets it die.
 * Readable equivalent:
 *     word = p->tmpl.unk4;
 *     if (word & 0xFFFF) {
 *         p->tmpl.unk4 &= 0xFFFF0000; p->tmpl.life = 30; p->tmpl.unk4 |= (u16)((word & 0xFFFF) - 1); return;
 *     }
 *     if ((u16)(word >> 16) < p->tmpl.primA) { p->tmpl.primA -= (u16)(word >> 16); p->tmpl.life = 30; } else { p->tmpl.life = 0; }
 */
// matching: permuter output; every readable form tried allocates `word` and the delay in other registers

void func_15168B44(void *arg0)
{
  BillboardParticle *a;
  s32 v1;
  s32 new_var2;
  u16 t6;
  u16 hi;
  u8 a2;
  s32 new_var;
  s32 c0;
  s32 z0;
  s32 z1;
  s32 z2;
  s32 t9;
  s32 z3;
  s32 z4;
  s32 z5;
  s32 keep;
  a = arg0;
  hi = 0x1E;
  v1 = a->tmpl.unk4;
  new_var2 = v1;
  t6 = ((u16) new_var2) ^ 0;
  new_var = t6 - (1 & 0xFFFFFFFFu);
  if (t6 != 0)
  {
    c0 = v1;
    z0 = 0;
    z1 = 0;
    z2 = 0;
    z3 = 0;
    z4 = 0;
    z5 = 0;
    keep = new_var & 0xFFFF;
    new_var = keep * 0;
    if (a)
    {
    }
    hi++;
    hi--;
    t9 = (v1 & 0xFFFF0000) | new_var;
    v1 = 0;
    a->tmpl.unk4 = t9;
    a->tmpl.life = hi;
    if (keep)
    {
      ;
    }
    *((s32 *) (((s8 *) arg0) + 0x14)) = ((t9 | keep) | (((((((c0 * 0) | (z0 * 0)) | (z1 * 0)) | (z2 * 0)) | (z3 * 0)) | (z4 * 0)) | (z4 = z5 * 0))) & 0xFFFFFFFFu;
    return;
  }
  hi = (u16) (v1 >> 16);
  v1 = a->tmpl.primA;
  if (0)
  {
  }
  a2 = v1;
  if (hi < a2)
  {
    a->tmpl.primA = a2 - hi;
    if (1)
    {
    }
    a->tmpl.life = 0x1E;
  }
  else
  {
    a2 = 0;
    a->tmpl.life = (a2, 0);
  }
}
/* Type 0x10 update: runs the object's callback, if any. */
void func_15168BAC(ModelFx *fx) {
    u8 index;

    index = fx->data.callback;
    if (index != 0) {
        D_8008CA20[index](&fx->link);
    }
}

/* Spawns a type 0x10 ModelFx from the template at `src`, if it has a display list. */
void func_15168BE4(src, owner, list)
    ModelFxData *src;
    u8 owner;
    s32 list;
{
    ModelFx *fx;

    if (src->displayList != NULL) {
        fx = func_15167A68(0x10, list, sizeof(ModelFx), 1, owner, 1);
        if (fx != NULL) {
            bcopy(src, &fx->data, sizeof(ModelFxData));
        }
    }
}

/* Type 0x10 draw: builds this frame's matrix, sets fade/tile/render state and calls the display list. */
Gfx *func_15168C4C(Gfx *gfx, ModelFx *fx, s32 arg2) {
    s32 pad; // matching: stack layout of the target frame
    s32 renderMode;
    u8 update;

    func_15043D90(&fx->mtx[D_800BE9C0], fx->data.rotation[0], fx->data.rotation[1], fx->data.rotation[2],
                  fx->data.scale[0], fx->data.scale[1], fx->data.scale[2], fx->data.position[0], fx->data.position[1], fx->data.position[2]);

    gDPPipeSync(gfx++);
    gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, fx->data.primA);
    /* D_800DD1C0..C6: cached prim colour (reset to -1 per type by func_151674F8) */
    D_800DD1C6 = fx->data.primA;
    D_800DD1C0 = D_800DD1C2 = D_800DD1C4 = 0;
    gDPSetEnvColor(gfx++, 0, 0, 0, 0);
    gDPSetTileSize(gfx++, 0, 2, fx->data.tileUlt, 0x7E, 0xFE);

    switch (fx->data.renderMode) {
    case 1:
        renderMode = 0x552230;
        break;
    case 2:
        renderMode = 0x504A50;
        break;
    }

    update = 0;
    gfx = func_15142FBC(gfx, 0x8ACA0, renderMode, &update);

    if (fx->data.combineMode == 1) {
        gDPSetCombine(gfx++, 0x123824, 0xFF73FFFF);
    }

    gSPMatrix(gfx++, &fx->mtx[D_800BE9C0], G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx++, fx->data.displayList);

    return gfx;
}

/* Adds `offset` to an address unless it is segmented. */
void func_15168E34(s32 *addr, s32 offset) {
    s32 value = *addr;

    if ((value & 0x0F000000) == 0) {
        *addr = value + offset;
    }
}

/* One display list command seen as bytes: opcode, two parameter bytes, a third (G_MOVEMEM index), w1. */
struct DLCommand {
    /* 0x0 */ s8 opcode;
    /* 0x1 */ s8 unk1;
    /* 0x2 */ s8 unk2;
    /* 0x3 */ u8 index;
    /* 0x4 */ u32 addr;
};

#define IS_RELOCATABLE(cmd) \
    ((cmd)->opcode == G_VTX || ((cmd)->opcode == (s8)G_MOVEMEM && (cmd)->index == G_MV_MATRIX))

/* Relocates the vertex and matrix addresses of a display list by `offset` (segmented ones are left alone). */
void func_15168E54(Gfx *dl, s32 offset) {
    s32 i;
    DLCommand *cmd;

    // matching: indexing the Gfx array (not a DLCommand array) gives the target's saved-register use
    for (i = 0; ((DLCommand *)&dl[i])->opcode != (s8)G_ENDDL; i++) {
        cmd = (DLCommand *)&dl[i];
        if (IS_RELOCATABLE(cmd)) {
            func_15168E34((s32 *)&cmd->addr, offset);
        }
    }
}

/* Rebases the vertex and matrix addresses of a display list: drops their segment and adds `base`. */
void func_15168F08(DLCommand *dl, s32 base) {
    s32 i;
    DLCommand *cmd;
    s8 opcode;

    // matching: the early-out, the dead `opcode` loads and the do-while(0) reproduce the target's
    // double opcode load and register use; plain for/while loops do not
    i = 0;
    cmd = dl;
    if (dl->opcode == (s8)G_ENDDL) {
        return;
    }
    cmd = &dl[i];
    opcode = cmd->opcode;
    do {
        i++;
        if (IS_RELOCATABLE(cmd)) {
            cmd->addr = cmd->addr & 0xFFFFFF;
            cmd->addr += base;
        }
        cmd = &dl[i];
        do {
            opcode = cmd->opcode;
        } while (0);
    } while (cmd->opcode != (s8)G_ENDDL);
}
/* Gives the range [*first, *last) of object types drawn together as draw group `group`. */
void func_15168F84(s32 group, s32 *first, s32 *last) {
    if (group == 0) {
        *first = 1;
        *last = 65;
    } else if (group == 1) {
        *first = 66;
        *last = 79;
    } else if (group == 2) {
        *first = 80;
        *last = 88;
    } else if (group == 3) {
        *first = 89;
        *last = 92;
    } else if (group == 5) {
        *first = 97;
        *last = 99;
    } else if (group == 6) {
        *first = 100;
        *last = 101;
    } else {
        *first = 93;
        *last = 96;
    }
}

/* Sends an event to every object of every type. */
void func_15169040(s32 event, u8 eventType) {
    func_15169070(0, OBJ_TYPE_COUNT, event, eventType);
}

/* Sends an event to every object of types [first, last) (clamped by func_15143D18). */
void func_15169070(s32 first, s32 last, s32 event, u8 eventType) {
    s32 type;
    EffectHeader *node;

    func_15143D18(&first, &last, 2, OBJ_TYPE_COUNT);
    for (type = first; type < last; type++) {
        s32 list;
        for (list = 0; list != 2; list++) {
            if (D_8008B4A8[type].handleEvent != NULL) {
                node = D_800DCE50[list][type];
                D_800DD190++;
                while (node != NULL) {
                    D_800DD198[D_800DD190] = node->next;
                    func_1516968C(node, (u8 *)event, eventType);
                    D_8008B4A8[type].handleEvent(node, event, eventType);
                    node = D_800DD198[D_800DD190];
                }
                D_800DD190--;
            } else {
                node = D_800DCE50[list][type];
                D_800DD190++;
                while (node != NULL) {
                    D_800DD198[D_800DD190] = node->next;
                    func_1516968C(node, (u8 *)event, eventType);
                    node = D_800DD198[D_800DD190];
                }
                D_800DD190--;
            }
        }
    }
}

/* Sends an event to every object of the `count` types listed in `types`. */
void func_15169260(const s32 *types, s32 count, s32 event, u8 eventType) {
    const s32 *type;
    s32 i;
    s32 list;
    EffectHeader *node;

    for (i = 0, type = types; i < count; i++) {
        for (list = 0; list != 2; list++) {
            if (D_8008B4A8[*type].handleEvent != NULL) {
                node = D_800DCE50[list][*type];
                D_800DD190++;
                while (node != NULL) {
                    D_800DD198[D_800DD190] = node->next;
                    func_1516968C(node, (u8 *)event, eventType);
                    D_8008B4A8[*type].handleEvent(node, event, eventType);
                    node = D_800DD198[D_800DD190];
                }
                D_800DD190--;
            } else {
                node = D_800DCE50[list][*type];
                D_800DD190++;
                while (node != NULL) {
                    D_800DD198[D_800DD190] = node->next;
                    func_1516968C(node, (u8 *)event, eventType);
                    node = D_800DD198[D_800DD190];
                }
                D_800DD190--;
            }
        }
        type++;
    }
}

/* Sends an event to every object of one type (clamped to the last type). */
void func_1516944C(s32 type, s32 event, u8 eventType) {
    s32 list;
    EffectHeader *node;

    if (type >= OBJ_TYPE_COUNT) {
        type = OBJ_TYPE_COUNT - 1;
    }
    for (list = 0; list != 2; list++) {
        if (D_8008B4A8[type].handleEvent != NULL) {
            node = D_800DCE50[list][type];
            D_800DD190++;
            while (node != NULL) {
                D_800DD198[D_800DD190] = node->next;
                func_1516968C(node, (u8 *)event, eventType);
                D_8008B4A8[type].handleEvent(node, event, eventType);
                node = D_800DD198[D_800DD190];
            }
            D_800DD190--;
        } else {
            node = D_800DCE50[list][type];
            D_800DD190++;
            while (node != NULL) {
                D_800DD198[D_800DD190] = node->next;
                func_1516968C(node, (u8 *)event, eventType);
                node = D_800DD198[D_800DD190];
            }
            D_800DD190--;
        }
    }
}
/* Sends an actor event about `actor` to every object. */
void func_151695F0(Actor *actor, u8 eventType) {
    ActorRef event;

    event.actor = actor;
    event.uniqueId = actor->unique_id;
    func_15169040((s32)&event, eventType);
}

/* Sends an actor event about `actor` to every object of one type. */
void func_1516962C(s32 type, Actor *actor, u8 eventType) {
    ActorRef event;
    event.actor = actor;
    event.uniqueId = actor->unique_id;
    func_1516944C(type, (s32)&event, eventType);
}

/* Sets D_800D2DAB and returns its first argument unchanged. */
Gfx *func_15169668(Gfx *gfx, s32 type, s32 list, s16 view) {
    D_800D2DAB = 1;
    return gfx;
}

/* Default event handling: event types 0xF and 0x10 destroy objects owned by event[0]. */
void func_1516968C(EffectHeader *obj, u8 *event, u8 eventType) {
    if (eventType == 0xF || eventType == 0x10) {
        // matching: obj->owner read as a byte array; the struct field form swaps the two loads
        if (event[0] == ((u8 *)obj)[0xC]) {
            func_1516972C(obj);
        }
    }
}

/* Repairs saved iteration cursors that point at `obj` (it is about to leave its list). */
void func_151696DC(EffectHeader *obj) {
    s8 depth;

    for (depth = 0; depth < D_800DD190; depth++) {
        if (obj == D_800DD198[depth]) {
            D_800DD198[depth] = obj->next;
        }
    }
}

/* Destroys an object of type 2 or above: its type's destroy hook, or by default func_15169804. */
void func_1516972C(void *obj) {
    EffectHeader *head;
    void (*destroy)(EffectHeader *obj);

    func_151696DC(obj);

    head = obj;
    if (head->type >= 2) {
        destroy = D_8008B4A8[head->type].destroy;
        if (destroy != NULL) {
            destroy(obj);
            return;
        }
        func_15169804(obj);
    }
}

/* Frees an object: its type's free hook, or by default func_15169824. */
void func_1516979C(void *obj) {
    EffectHeader *head;
    void (*free)(EffectHeader *obj);

    func_151696DC(obj);
    head = obj;
    free = D_8008B4A8[head->type].free;
    if (free != NULL) {
        free(obj);
        return;
    }
    func_15169824(obj);
}

/* Default destroy: moves the object to type 1 (pending free). */
void func_15169804(void *obj) {
    func_15168B10(obj, 1);
}

/* Default free: unlinks the object and releases its memory. */
void func_15169824(void *obj) {
    func_15168A9C(obj);
    func_10004074(obj);
}

/*
 * Shared actor-event handler for objects that remember an owner (`owner`, `ownerId`): event type 0 destroys
 * `obj` if the event names its owner; type 0x2D hands ownership over to the other actor of the swap.
 */
void func_15169850(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj) {
    if (eventType == 0) {
        if (((ActorRef *)event)->actor == *owner || ((ActorRef *)event)->uniqueId == *ownerId) {
            func_1516972C(obj);
        }
    } else if (eventType == 0x2D) {
        Actor *actorA = ((ActorSwapMsg *)event)->actorA;

        if (actorA == *owner) {
            *owner = ((ActorSwapMsg *)event)->actorB;
            *ownerId = ((ActorSwapMsg *)event)->idB;
        } else if (((ActorSwapMsg *)event)->actorB == *owner) {
            *owner = actorA;
            *ownerId = ((ActorSwapMsg *)event)->idA;
        }
    }
}
