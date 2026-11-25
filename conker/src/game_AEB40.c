#include <ultra64.h>

// functions.h and variables.h declare several of the symbols below with types that do not fit how
// this file uses them (struct127 lacks or mistypes many object fields, model tables are declared as
// plain s32, ...). Their header declarations are renamed out of the way and redeclared further down.
#define func_150ADA20 func_150ADA20_hdr
#define func_15083E90 func_15083E90_hdr
#define func_15083568 func_15083568_hdr
#define func_1505E650 func_1505E650_hdr
#define func_1505F0AC func_1505F0AC_hdr
#define func_15060F28 func_15060F28_hdr
#define func_150615DC func_150615DC_hdr
#define func_1506160C func_1506160C_hdr
#define func_15062BDC func_15062BDC_hdr
#include "functions.h"
#undef func_150ADA20
#undef func_15083E90
#undef func_15083568
#undef func_1505E650
#undef func_1505F0AC
#undef func_15060F28
#undef func_150615DC
#undef func_1506160C
#undef func_15062BDC

#define D_800CC2D0 D_800CC2D0_hdr
#define D_800C5A90 D_800C5A90_hdr
#define D_800D19A0 D_800D19A0_hdr
#define D_800D1C90 D_800D1C90_hdr
#define D_800D1F80 D_800D1F80_hdr
#define D_800D2040 D_800D2040_hdr
#define D_800D20FC D_800D20FC_hdr
#define D_800D2110 D_800D2110_hdr
#define D_800D2138 D_800D2138_hdr
#define D_800BE628 D_800BE628_hdr
#define D_800D2132 D_800D2132_hdr
#include "variables.h"
#undef D_800CC2D0
#undef D_800C5A90
#undef D_800D19A0
#undef D_800D1C90
#undef D_800D1F80
#undef D_800D2040
#undef D_800D20FC
#undef D_800D2110
#undef D_800D2138
#undef D_800BE628
#undef D_800D2132

// File-local view of the 0x31C extension block of a game object (header struct126, 0x1C0 bytes).
typedef struct GameObjExt {
    /* 0x000 */ u8 pad0[0x56];
    /* 0x056 */ u8 unk56;
    /* 0x057 */ u8 pad57[0x2D];
    /* 0x084 */ u8 unk84;
    /* 0x085 */ u8 pad85[0x115];
    /* 0x19A */ u8 grenade_count;
    /* 0x19B */ u8 pad19B[0xF];
    /* 0x1AA */ s16 unk1AA;
    /* 0x1AC */ u8 pad1AC;
    /* 0x1AD */ u8 unk1AD;
    /* 0x1AE */ u8 pad1AE;
    /* 0x1AF */ u8 unk1AF;
    /* 0x1B0 */ s8 unk1B0;
    /* 0x1B1 */ u8 pad1B1;
    /* 0x1B2 */ u8 unk1B2;
    /* 0x1B3 */ u8 pad1B3[0x5];
    /* 0x1B8 */ f32 unk1B8;
    /* 0x1BC */ u8 pad1BC[0x4];
} GameObjExt; // size 0x1C0

// One 0x30-byte entry of the level's spawn table D_800D20FC.
typedef struct SpawnRecord {
    /* 0x00 */ u16 flags;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 interaction_state;
    /* 0x04 */ u8 type;         // model id, 0xFF for none
    /* 0x05 */ u8 unk5;
    /* 0x06 */ s16 x;
    /* 0x08 */ s16 y;
    /* 0x0A */ s16 z;
    /* 0x0C */ u8 angle;        // high byte of the facing angle
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 unkF;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[0x2];
    /* 0x18 */ s32 objFlags;    // copied to the object's unkF8
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 pad1E[0x2];
    /* 0x20 */ f32 xz_scale;
    /* 0x24 */ f32 y_scale;
    /* 0x28 */ u8 unique_id;
    /* 0x29 */ u8 partSet;      // 1-based index into D_80086CC4, 0 for none
    /* 0x2A */ u8 pad2A[0x2];
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
} SpawnRecord; // size 0x30

// The 0xC-byte block a game object keeps at 0x260 for some models.
typedef struct ObjAlloc260 {
    /* 0x0 */ f32 unk0;
    /* 0x4 */ f32 unk4;
    /* 0x8 */ u8 unk8;
    /* 0x9 */ u8 unk9;
} ObjAlloc260;

// File-local view of a game object (struct127, the 0x32C-byte entries of D_800CC2D0).
// include/structs.h lacks or mistypes several fields used here (0x10 is a u16, 0x38 an s16, 0x140 a
// u8, 0x2C4 a pointer, ...); fold them in during the central pass.
typedef struct GameObj {
    /* 0x000 */ s32 interaction_state;
    /* 0x004 */ u8 id;
    /* 0x005 */ u8 unk5;
    /* 0x006 */ u8 unk6;
    /* 0x007 */ u8 pad7[0x9];
    /* 0x010 */ u16 unk10;
    /* 0x012 */ u8 pad12[0x2];
    /* 0x014 */ f32 x_position;
    /* 0x018 */ f32 y_position;
    /* 0x01C */ f32 z_position;
    /* 0x020 */ f32 y_velocity;
    /* 0x024 */ f32 gravity;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ f32 old_x_position;
    /* 0x030 */ f32 old_y_position;
    /* 0x034 */ f32 old_z_position;
    /* 0x038 */ s16 unk38;
    /* 0x03A */ u8 pad3A;
    /* 0x03B */ u8 unique_id;
    /* 0x03C */ f32 xz_velocity;
    /* 0x040 */ f32 unk40;
    /* 0x044 */ u8 pad44[0x14];
    /* 0x058 */ s32 unk58;
    /* 0x05C */ s32 unk5C;
    /* 0x060 */ u8 pad60[0x5];
    /* 0x065 */ u8 unk65;       // 1-based index of the owning object, 0 for none
    /* 0x066 */ u8 unk66;
    /* 0x067 */ u8 pad67;
    /* 0x068 */ u8 unk68;
    /* 0x069 */ u8 unk69;
    /* 0x06A */ u8 pad6A[0xC];
    /* 0x076 */ u16 unk76;
    /* 0x078 */ u16 unk78;
    /* 0x07A */ u16 unk7A;
    /* 0x07C */ u8 pad7C[0x14];
    /* 0x090 */ u16 unk90;
    /* 0x092 */ u16 unk92;
    /* 0x094 */ s32 unk94;
    /* 0x098 */ u8 pad98[0x13];
    /* 0x0AB */ u8 unkAB;
    /* 0x0AC */ u8 unkAC;
    /* 0x0AD */ u8 padAD[0x5];
    /* 0x0B2 */ u16 unkB2;
    /* 0x0B4 */ u8 padB4[0x4];
    /* 0x0B8 */ f32 unkB8;
    /* 0x0BC */ u8 padBC[0x8];
    /* 0x0C4 */ f32 unkC4;
    /* 0x0C8 */ f32 unkC8;
    /* 0x0CC */ u8 padCC[0x6];
    /* 0x0D2 */ s16 unkD2;
    /* 0x0D4 */ s16 unkD4;
    /* 0x0D6 */ s16 unkD6;
    /* 0x0D8 */ u8 padD8[0x4];
    /* 0x0DC */ f32 unkDC;
    /* 0x0E0 */ f32 unkE0;
    /* 0x0E4 */ s16 unkE4;
    /* 0x0E6 */ s16 unkE6;
    /* 0x0E8 */ u16 unkE8;
    /* 0x0EA */ u8 padEA[0xE];
    /* 0x0F8 */ s32 unkF8;
    /* 0x0FC */ u8 padFC[0x5];
    /* 0x101 */ u8 unk101;
    /* 0x102 */ u8 pad102[0x2];
    /* 0x104 */ u8 stunned;
    /* 0x105 */ u8 pad105[0x1E];
    /* 0x123 */ u8 unk123;
    /* 0x124 */ u8 unk124;
    /* 0x125 */ u8 immune;
    /* 0x126 */ u8 pad126;
    /* 0x127 */ u8 unk127;
    /* 0x128 */ u8 unk128;
    /* 0x129 */ u8 pad129[0x12];
    /* 0x13B */ u8 unk13B;
    /* 0x13C */ u8 pad13C[0x3];
    /* 0x13F */ u8 unk13F;      // index of the spawn record this object came from
    /* 0x140 */ u8 unk140;
    /* 0x141 */ u8 pad141[0x3];
    /* 0x144 */ SpawnRecord *spawn;
    /* 0x148 */ u8 pad148[0x4];
    /* 0x14C */ f32 xz_scale;
    /* 0x150 */ f32 y_scale;
    /* 0x154 */ f32 unk154;
    /* 0x158 */ f32 unk158;
    /* 0x15C */ u8 pad15C[0x4];
    /* 0x160 */ s16 unk160;
    /* 0x162 */ u8 pad162[0x67];
    /* 0x1C9 */ u8 unk1C9;
    /* 0x1CA */ u8 health;
    /* 0x1CB */ u8 pad1CB;
    /* 0x1CC */ f32 unk1CC;
    /* 0x1D0 */ s8 unk1D0;
    /* 0x1D1 */ u8 pad1D1[0xC];
    /* 0x1DD */ u8 unk1DD[3];
    /* 0x1E0 */ u8 unk1E0[3];
    /* 0x1E3 */ u8 pad1E3;
    /* 0x1E4 */ u8 unk1E4;
    /* 0x1E5 */ u8 unk1E5;
    /* 0x1E6 */ u8 unk1E6;
    /* 0x1E7 */ u8 pad1E7[0x15];
    /* 0x1FC */ u8 unk1FC;
    /* 0x1FD */ u8 pad1FD[0x12];
    /* 0x20F */ u8 unk20F;
    /* 0x210 */ u8 unk210;
    /* 0x211 */ u8 unk211;
    /* 0x212 */ u8 unk212;
    /* 0x213 */ u8 pad213[0xE];
    /* 0x221 */ s8 unk221;
    /* 0x222 */ u8 pad222[0x2];
    /* 0x224 */ s16 unk224;
    /* 0x226 */ u8 pad226[0xC];
    /* 0x232 */ u8 unk232;
    /* 0x233 */ u8 pad233[0x9];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 unk23D;
    /* 0x23E */ u8 unk23E;
    /* 0x23F */ u8 pad23F[0x7];
    /* 0x246 */ u8 unk246;
    /* 0x247 */ u8 unk247;
    /* 0x248 */ u8 unk248;
    /* 0x249 */ u8 pad249[0x17];
    /* 0x260 */ ObjAlloc260 *unk260;
    /* 0x264 */ u8 pad264[0xC];
    /* 0x270 */ f32 unk270;
    /* 0x274 */ u8 pad274[0x14];
    /* 0x288 */ u8 unk288;
    /* 0x289 */ u8 pad289[0x3];
    /* 0x28C */ u8 *modelData[7][2];  // per model in modelIds: shared or private copies of D_800D19A0[id]
    /* 0x2C4 */ u8 *modelIds;
    /* 0x2C8 */ u8 unk2C8;
    /* 0x2C9 */ u8 modelCount;
    /* 0x2CA */ u8 pad2CA;
    /* 0x2CB */ s8 unk2CB;
    /* 0x2CC */ s32 unk2CC;
    /* 0x2D0 */ struct197 *unk2D0;
    /* 0x2D4 */ u8 pad2D4[0x18];
    /* 0x2EC */ s32 unk2EC;
    /* 0x2F0 */ u8 pad2F0[0x10];
    /* 0x300 */ u8 unk300;
    /* 0x301 */ u8 unk301;
    /* 0x302 */ u8 pad302[0x2];
    /* 0x304 */ s32 unk304[4];      // one per player
    /* 0x314 */ s32 *unk314;
    /* 0x318 */ struct108 *camera;
    /* 0x31C */ GameObjExt *unk31C;
    /* 0x320 */ u8 pad320[0xC];
} GameObj; // size 0x32C

// One entry of a model's definition table D_800D1C90 (header struct124).
typedef struct ModelDef {
    /* 0x00 */ u8 pad0[0x2];
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8 subModelCount;
    /* 0x05 */ u8 subModelList;     // index into D_80086CAC
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 pad8[0x6];
    /* 0x0E */ u16 unkE;
    /* 0x10 */ u8 pad10[0x2];
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 pad13[0x16];
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u16 unk2A;
    /* 0x2C */ u8 pad2C[0x7];
    /* 0x33 */ s8 unk33;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 extraModelCount;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 pad3A;
    /* 0x3B */ u8 unk3B;
    /* 0x3C */ u8 unk3C;
} ModelDef;

// One 0x10-byte part of a part set (D_80086CC4): a child object or an attachment.
typedef struct PartDef {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 type;     // 0: child object (func_15083AC8), otherwise func_15030AF4
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ u8 data[6];
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 unkF;
} PartDef;

typedef struct PartSet {
    /* 0x0 */ PartDef *parts;
    /* 0x4 */ u8 count;
} PartSet;

// Result of a ray cast (func_15081690), consumed by func_15081E78.
typedef struct RayHit {
    /* 0x00 */ GameObj *obj;
    /* 0x04 */ f32 dist;
    /* 0x08 */ struct17 hit;
    /* 0x14 */ struct17 centre;
    /* 0x20 */ struct17 far;
    /* 0x2C */ struct17 origin;
    /* 0x38 */ struct17 dir;
    /* 0x44 */ u8 unk44[0x12];
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 hitType;      // 0 none, 1 world, 2 object cylinder, 3 object model
    /* 0x5A */ u8 unk5A;
    /* 0x5C */ struct131 *surface;
    /* 0x60 */ s32 unk60;
} RayHit;

typedef struct {
    /* 0x0 */ u32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
} ModelReloc;

typedef struct {
    /* 0x000 */ u8 pad0[0x208];
    /* 0x208 */ u16 unk208;     // bit per player
} Struct800D2138;

extern GameObj D_800CC2D0[26];
extern u16 D_800C5A90[187];
extern u8 *D_800D19A0[187];
extern ModelDef *D_800D1C90[187];
extern u8 D_800D1F80[187];
extern u8 D_800D2040[187];
extern SpawnRecord *D_800D20FC;
extern u16 *D_800D2110;
extern Struct800D2138 D_800D2138;
extern u8 (*D_800BE628)[0x180];   // one 0x180-byte entry per player
extern f32 D_8009CC3C;
extern u8 *D_800D210C;
extern s16 D_800D2132;
extern u8 D_800D2100;
extern u8 D_800D2101;
extern s8 D_800D2130;
extern u8 *D_80086CAC[];
extern PartSet D_80086CC4[];
extern s32 (*D_80086C90[])(GameObj *, GameObj *, s32);
extern s8 D_8008FE44[];
extern u8 D_80099AC4[];
extern u8 D_80099AD1;
extern u8 D_800A7230[][8];
extern u8 D_800BE590;
extern u16 D_800BE598[];
extern ModelReloc *D_800C5338[];
extern u16 D_800C5628[];
extern u16 D_800C57A0[];
extern f32 D_800D9C10[][4][4];

// func_150ADA20 returns a full-width random value here (functions.h has it as u8).
extern s32 func_150ADA20(void);
extern void func_1505E650(GameObj *obj, u16 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6);
extern GameObj *func_1505F0AC(s32);
extern void func_15060F28(GameObj *obj, s32 arg1);
extern void func_150615DC(GameObj *obj);
extern void func_1506160C(GameObj *obj, s32, u8, u8, s32);
extern void func_15062BDC(GameObj *obj, f32 arg1, f32 arg2);
extern void *allocate_memory(s32 size, s32, s32, s32);
extern s32 func_10004074(void *);
extern void func_100043B4(s32 *arg0, u32 arg1);
extern s32 func_1502B020(s32, s32, s32, s32);
extern void func_150302F0(GameObj *obj, s32 setId);
extern GameObj *func_15030AF4(GameObj *, s32, s32, s32, s32, s32, s32, s32, s32, u8 *, s32, s32, s32);
extern void func_150335C8(GameObj *, GameObj *, s32, s32, s32, s32);
extern void func_15036C70(GameObj *obj);
extern s32 func_1503D660(s32 modelId, s32 arg1);
extern s32 func_1503D774(s32 modelId, s32 arg1);
extern s32 func_1503DA9C(GameObj *, s32, s32, s32);
extern s32 func_1505D024(GameObj *, s32, u16, s32);
extern GameObj *func_1505ED34(void);
extern GameObj *func_1505EEF4(void);
extern void func_1505F188(GameObj *obj);
extern void func_15060A30(s32, GameObj *);
extern void func_15060A9C(s32, GameObj *);
extern void func_1506AC8C(GameObj *, s32, s32);
extern void func_1507DE4C(GameObj *);
extern void func_1507DF10(GameObj *obj, s32 arg1);
extern s32 func_1508114C(GameObj *obj, struct17 *rayOrigin, struct17 *rayDir, struct17 *outNear, struct17 *outFar, struct17 *outCentre, f32 *outDist, s32 *outIndex, s32 *outBone, s32 *outPart, s16 *outOffset);
extern s32 func_15084D00(GameObj *);
extern s32 func_150A6360(void *arg0, f32 arg1[4][4], f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, f32 *, f32 *, f32 *, f32 *, s32 *, void *, f32);
extern void func_150B5C38(void *, u8, s32);
extern void func_150B6754(u8, s32);
extern void func_150D7120(void *, u8, s32);
extern void func_150D728C(void *, void *, void *, u8, s32);
extern void func_150EB484(f32 *, f32 *, f32 *);
extern void func_15104608(f32, f32, s32, s32);
extern void func_1510D720(s32 arg0);
extern void func_15137610(GameObj *, void *, void *, void *, u8, s32);
extern void func_15138C80(GameObj *, s32, s32);
extern f32 func_15143E64(struct17 *);
extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);
extern s32 func_151452C4(struct17 *arg0, struct17 *arg1, struct17 *arg2, f32 arg3, struct17 *arg4, struct17 *arg5, f32 *arg6, f32 *arg7);
extern void func_15145974(void *, f32 *, f32 *);
extern s32 func_1514D15C(GameObj *obj);
extern s32 *func_1515D440(void);
extern s32 func_1515D480(s32);
extern s32 func_151D3480(void *, void *, void *, u8, u8, s32);
extern s32 func_151E5F64(s32 arg0);

s32 func_15081C20(struct17 *origin, struct17 *dir, GameObj *obj, struct17 *outNear, struct17 *outFar, f32 *outNearDist, f32 *outFarDist);
void func_15081E0C(GameObj *obj, u16 part, u8 skip);
s32 func_150825C0(s32 spawnIdx, s32 arg1);
s32 func_15082A44(SpawnRecord *spawn, s32 spawnIdx, s32 arg2, s32 arg3, s32 slot);
void func_150829D8(GameObj *obj);
void func_150832AC(GameObj *obj);
void func_15083384(GameObj *obj, s32 arg1);
GameObj *func_15083568(GameObj *parent, s32 setId, f32 arg2, s32 arg3);
void func_150837D4(s32 objIdx, s32 modelId, s32 arg2);
s32 func_150838EC(GameObj *obj, s32 arg1, s32 arg2, f32 arg3);
void func_150839B8(GameObj *obj, s32 modelId, SpawnRecord *spawn);
GameObj *func_15083AC8(GameObj *parent, s32 arg1, s32 modelId, s32 arg3, s32 state, s32 arg5, s32 inheritScale, s32 arg7, f32 arg8);
GameObj *func_15083E90(u8 id);
void func_150843AC(GameObj *obj, s32 model);
void func_15084488(SpawnRecord *spawn, s32 arg1, s32 arg2);
u8 func_150849A0(GameObj *obj);
void func_15084C30(GameObj *obj);
s32 func_15084CB0(s32 modelId);

/* Casts a ray from origin along dir against the world and every active object (except self and those in
   ignoreMask or rejected by filter), recording the nearest hit closer than maxDist in out. */
void func_15081690(GameObj *self, f32 originX, f32 originY, f32 originZ, f32 dirX, f32 dirY, f32 dirZ, RayHit *out, f32 maxDist, s32 skipWorld, s32 skipObjs, s32 skipModels, s8 filter, s32 filterArg, s32 ignoreMask) {
    s32 surf;
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    f32 dist;
    f32 farDist;
    s32 part;
    s32 bone;
    s16 offset[4];
    s32 i;
    GameObj *obj;
    s32 best;
    s32 skip;
    s32 ret;
    u8 childSlot[25];
    struct17 near;
    struct17 far;
    struct17 centre;
    struct17 origin;
    struct17 dir;
    s32 index;
    s32 surfIdx;

    origin.unk0 = originX;
    origin.unk4 = originY;
    origin.unk8 = originZ;
    dir.unk0 = dirX;
    dir.unk4 = dirY;
    dir.unk8 = dirZ;
    out->dist = -1.0f;
    out->obj = NULL;
    out->hitType = 0;
    out->origin = origin;
    best = -1;
    out->dir = dir;
    out->surface = NULL;
    out->unk60 = 0;
    if (maxDist == 0.0f) {
        maxDist = 9999999.0f;
    }
    if (self != NULL) {
        skip = self->unk124 - 1;
    } else {
        skip = -1;
    }
    // childSlot[owner] = 1-based slot of an object owned by owner
    bzero(childSlot, sizeof(childSlot));
    for (i = 0; i < 25; i++) {
        if (D_800CC2D0[i].interaction_state == 0) {
            continue;
        }
        if (D_800CC2D0[i].unk65 != 0) {
            childSlot[D_800CC2D0[i].unk65 - 1] = i + 1;
        }
    }
    if (skipWorld == 0 &&
        func_150AC9C0(originX, originY, originZ, dirX, dirY, dirZ, &surf, out->unk44, &hitX, &hitY, &hitZ, &dist, &surfIdx, &out->unk60, 0.0f) != 0 &&
        dist < maxDist) {
        out->dist = dist;
        out->hitType = 1;
        out->hit.unk0 = hitX;
        out->hit.unk4 = hitY;
        out->hit.unk8 = hitZ;
        out->far = out->hit;
        if (surfIdx != -1) {
            out->surface = &D_800DBEF4[surfIdx];
        } else {
            out->surface = NULL;
        }
    }
    if (skipObjs == 0) {
        for (i = 0; i < 25; i++) {
            if ((1 << i) & ignoreMask) {
                continue;
            }
            if (skip == i) {
                continue;
            }
            obj = &D_800CC2D0[i];
            if (obj->interaction_state == 0) {
                continue;
            }
            if (obj->unk5 == 3) {
                continue;
            }
            if (obj == self) {
                continue;
            }
            if (!(obj->unkF8 & 0x40)) {
                continue;
            }
            if (filter != -1 && D_80086C90[filter](obj, self, filterArg) == 0) {
                continue;
            }
            if (skipModels == 0) {
                ret = func_1508114C(obj, &origin, &dir, &near, &far, &centre, &dist, &index, &bone, &part, offset);
                if (ret > 0 && dist < maxDist && (out->dist == -1.0f || dist < out->dist) && best != childSlot[i]) {
                    out->dist = dist;
                    out->obj = obj;
                    best = i + 1;
                    out->unk58 = part;
                    out->unk56 = bone;
                    out->hit = near;
                    out->centre = centre;
                    out->unk5A = index;
                    out->far = far;
                    out->hitType = 3;
                }
                if (ret != -1) {
                    continue;
                }
            }
            if (func_15081C20(&origin, &dir, obj, &near, &far, &dist, &farDist) != 0 &&
                dist < maxDist && (out->dist == -1.0f || dist < out->dist) && best != childSlot[i]) {
                out->dist = dist;
                out->obj = obj;
                best = i + 1;
                out->hit = near;
                out->far = far;
                out->hitType = 2;
            }
        }
    }
}

/* Intersects a ray with obj's collision cylinder (radius unkD2, centre raised by unkD6, y squashed by
   unkDC). Returns 1 with the entry/exit points and their distances from origin on a hit. */
s32 func_15081C20(struct17 *origin, struct17 *dir, GameObj *obj, struct17 *outNear, struct17 *outFar, f32 *outNearDist, f32 *outFarDist) {
    struct17 pos;

    pos.unk0 = obj->x_position;
    pos.unk4 = obj->y_position + obj->unkD6;
    pos.unk8 = obj->z_position;
    {
        struct17 start;
        struct17 unitDir;
        struct17 center;
        f32 yScale;
        f32 yUnscale;

        yScale = obj->unkDC;
        yUnscale = obj->unkE0;
        start.unk0 = origin->unk0;
        start.unk4 = origin->unk4 * yScale;
        start.unk8 = origin->unk8;
        unitDir.unk0 = dir->unk0;
        unitDir.unk4 = dir->unk4 * yScale;
        unitDir.unk8 = dir->unk8;
        if (func_15145128(&unitDir, &unitDir, NULL, NULL) == 0) {
            return 0;
        }
        center.unk0 = pos.unk0;
        center.unk4 = pos.unk4 * yScale;
        center.unk8 = pos.unk8;
        if (func_151452C4(&start, &unitDir, &center, obj->unkD2, outNear, outFar, outNearDist, outFarDist) == 0) {
            return 0;
        }
        outNear->unk4 *= yUnscale;
        outFar->unk4 *= yUnscale;
        {
            struct17 diff;

            diff.unk0 = outNear->unk0 - origin->unk0;
            diff.unk4 = outNear->unk4 - origin->unk4;
            diff.unk8 = outNear->unk8 - origin->unk8;
            *outNearDist = func_15143E64(&diff);
            diff.unk0 = outFar->unk0 - origin->unk0;
            diff.unk4 = outFar->unk4 - origin->unk4;
            diff.unk8 = outFar->unk8 - origin->unk8;
            *outFarDist = func_15143E64(&diff);
        }
    }
    return 1;
}

void func_15081E0C(GameObj *obj, u16 part, u8 skip) {
    if (D_800BE616 == 0) {
        if (obj->health != 0) {
            obj->health = 7;
        }
        if ((skip == 0) || (part != 10)) {
            func_1507DF10(obj, part);
        }
    }
}

/* Applies the effects of a ray hit of the given kind fired by self: world impact effects for a world
   hit, otherwise hit reactions, sounds and effects on the object that was hit. Returns 0 for no hit,
   1 for a world hit and 2 for an object hit. */
s32 func_15081E78(GameObj *self, RayHit *hit, s32 kind) {
    s32 sound;
    f32 yaw;
    f32 pitch;
    u8 playImpact;
    GameObj *obj;
    GameObjExt *ext;
    // matching: volatile keeps IDO from hoisting the li/lw of noHealth out of the if/else
    volatile s32 noHealth;
    s32 longShot;
    s32 effect;
    s32 spawnEffect;
    s32 pad;

    playImpact = 1;
    ext = self->unk31C;
    if (ext != NULL) {
        ext->unk1AA++;
    }
    if (hit->hitType == 0) {
        return 0;
    }
    if (hit->hitType == 1) {
        switch (kind) {
        case 18:
            effect = 4;
            break;
        case 6:
            effect = 0;
            break;
        case 2:
            effect = 1;
            break;
        case 7:
            effect = 5;
            break;
        case 11:
            effect = 3;
            break;
        case 12:
            effect = 2;
            break;
        case 13:
            effect = -1;
            func_150D728C(hit->unk44, &hit->hit, &hit->dir, 0xFF, 1);
            break;
        case 5:
            if (D_800BE9F0 == 14) {
                f32 vc;
                f32 front[3];
                f32 back[3];

                front[0] = hit->hit.unk0 + hit->dir.unk0 * 5.0f;
                front[1] = hit->hit.unk4 + hit->dir.unk4 * 5.0f;
                front[2] = hit->hit.unk8 + hit->dir.unk8 * 5.0f;
                back[0] = hit->hit.unk0 - hit->dir.unk0 * 5.0f;
                back[1] = hit->hit.unk4 - hit->dir.unk4 * 5.0f;
                back[2] = hit->hit.unk8 - hit->dir.unk8 * 5.0f;
                func_150EB484(front, back, &vc);
            }
            // fallthrough
        case 9:
        case 10:
            effect = -1;
            // matching: z goes to func_15104608 in a2 as its raw bits
            func_15104608(hit->hit.unk0, hit->hit.unk4, *(s32 *)&hit->hit.unk8, self->unk124);
            func_1506AC8C(self, 2, 0);
            break;
        default:
            effect = 5;
            break;
        }
        if (effect != -1) {
            func_151D3480(&hit->hit, &hit->dir, hit->unk44, effect, 0xFF, 1);
        }
        return 1;
    }
    obj = hit->obj;
    if (kind == 5 || kind == 9 || kind == 10) {
        func_1506AC8C(self, 3, 0);
    }
    if (obj->health == 0) {
    // matching: an (unused) label here stops IDO hoisting the store out of the then-block
    noHealthLabel:
        noHealth = 1;
    } else {
        noHealth = 0;
    }
    if ((obj->immune != 0 || obj->stunned != 0) && noHealth == 0) {
        return 2;
    }
    longShot = 0;
    sound = D_80099AC4[kind];
    if (kind == 7) {
        if (D_800BE9F0 == 43) {
            sound = 0x27;
        }
    }
    func_15145974(&hit->dir, &yaw, &pitch);
    if (hit->hitType == 2) {
        if (obj->id == 0x94) {
            playImpact = 0;
            func_150B5C38(&hit->hit, 0xFF, 1);
            func_150B6754(0xFF, 1);
        }
    } else if (hit->hitType == 3) {
        if (D_800BE616 == 0) {
            if (obj->id == 0x9F || obj->id == 0xA0) {
                if (1000.0f < hit->dist) {
                    longShot = 1;
                }
            }
        }
        if (hit->unk58 == 10 && (obj->unk31C == NULL || obj->unk31C->unk1AD == 0) && longShot == 0) {
            // matching: unsigned compares keep the constants as immediates
            if ((u32)kind != 2 && (u32)kind != 18 && (u32)kind != 11 && (u32)kind != 12) {
                func_1507DE4C(obj);
                if (obj->unk31C != NULL) {
                    obj->unk31C->unk1AD = 1;
                }
                if (obj->id == 0x80 && hit->unk5A == 0 && hit->hit.unk4 - 5.5f > hit->centre.unk4) {
                    func_15060A9C((func_150ADA20() & 7) + 0x2E3, obj);
                }
                func_15138C80(obj, 0xFF, 1);
                sound = 0x20;
                if (obj->id == 0x9F || obj->id == 0xA0) {
                    if (obj->unk2EC != 0) {
                        sound = 0x3D;
                    }
                }
                func_15060A30(0xAA, obj);
            } else {
                func_15060A30(0x107, obj);
            }
            if (kind != 11 && kind != 12 && obj->health != 0) {
                obj->health = 1;
            }
        } else if (obj->id == 0x9F || obj->id == 0xA0) {
            func_15081E0C(obj, hit->unk58, longShot);
        }
        switch (kind) {
        case 2:
            spawnEffect = 0x2A;
            break;
        case 18:
            spawnEffect = 0x22;
            break;
        case 12:
            spawnEffect = 0x6B;
            break;
        case 11:
            spawnEffect = 0x6D;
            break;
        default:
            spawnEffect = 0;
            break;
        }
        if (spawnEffect != 0 && obj->health < 2) {
            GameObj source;

            source.x_position = hit->hit.unk0;
            source.y_position = hit->hit.unk4;
            source.z_position = hit->hit.unk8;
            source.unkB8 = pitch;
            source.unk40 = yaw + 180.0f;
            source.unkC4 = 0.0f;
            source.xz_scale = obj->xz_scale;
            source.y_scale = obj->y_scale;
            func_150335C8(&source, obj, spawnEffect, hit->unk56, 1, 0);
            if (hit->unk58 == 10) {
                sound = 0x20;
            }
        }
    }
    if (playImpact) {
        func_15137610(hit->obj, &hit->hit, &hit->far, &hit->dir, 0xFF, 1);
    }
    // matching: compares addresses (D_80099AD1 is D_80099AC4[13]) rather than kind == 13
    if (&D_80099AD1 == &D_80099AC4[kind]) {
        func_150D7120(&hit->hit, 0xFF, 1);
    }
    if (noHealth == 0) {
        func_1505D024(obj, sound | 0x80000, (yaw - 90.0f) * 182.04444885f, self - D_800CC2D0);
    }
    return 2;
}

/* Spawns the object for spawn record spawnIdx once it is due: skips records that are in use,
   disabled (x == 0x7FFF) or still counting down their D_800D2110 delay, and records whose flags
   ask for no player nearby (radius by flags & 6) or for being off-screen (flags & 8). */
s32 func_150825C0(s32 spawnIdx, s32 arg1) {
    f32 radiusSq;
    s32 i;
    u16 range;
    u8 clear;
    u8 flags;
    s32 j;
    f32 dx;
    f32 dz;

    if (D_800D210C[spawnIdx] != 0) {
        return 0;
    }
    if (D_800D20FC[spawnIdx].unk2 == 1) {
        return 0;
    }
    if (D_800D20FC[spawnIdx].x == 0x7FFF) {
        return 0;
    }
    if (D_800D2110[spawnIdx]) {
        // matching: arg1 is dead here and IDO homes the frame time in its arg slot
        arg1 = D_800BE9A0;
        if (D_800D2110[spawnIdx] <= arg1) {
            D_800D2110[spawnIdx] = 0;
        } else {
            D_800D2110[spawnIdx] = D_800D2110[spawnIdx] - arg1;
        }
        return 0;
    }
    flags = D_800D20FC[spawnIdx].flags;
    if (spawnIdx >= 5) {
        range = flags & 6;
        if (range != 0) {
            if (range == 2) {
                radiusSq = 422500.0f;
            } else if (range == 4) {
                radiusSq = 1690000.0f;
            } else {
                radiusSq = 7562500.0f;
            }
            clear = 1;
            for (j = 0; j < D_8008FD8C; j++) {
                if (D_800CC2D0[j].interaction_state == 0) {
                    continue;
                }
                if (D_800CC2D0[j].unk127 == 0xFF) {
                    continue;
                }
                if (D_800D2138.unk208 & (1 << D_800CC2D0[j].unk127)) {
                    // matching: squaring in place (d = d * d) reuses the operand's register
                    dx = D_800CC2D0[j].x_position - D_800D20FC[spawnIdx].x;
                    dx = dx * dx;
                    dz = D_800CC2D0[j].z_position - D_800D20FC[spawnIdx].z;
                    dz = dz * dz;
                    if (dx + dz <= radiusSq) {
                        clear = 0;
                        break;
                    }
                }
            }
            if (clear) {
                return 0;
            }
        }
    }
    if (flags & 8) {
        clear = 1;
        for (i = 0; i <= D_80082FA0; i++) {
            // matching: offset + base operand order
            if (func_150A6360(i + D_800BE628, D_800D9C10[i],
                              D_800D20FC[spawnIdx].x,
                              D_800D20FC[spawnIdx].y + 150,
                              D_800D20FC[spawnIdx].z,
                              150.0f, 150.0f, D_8009CC3C) != 0) {
                clear = 0;
            }
        }
        if (!clear) {
            return 0;
        }
    }
    return func_15082A44(&D_800D20FC[spawnIdx], spawnIdx, 0, 0, 0);
}

/* Runs func_150825C0 over the spawn records: [0, D_800D2101) when firstRange is set, else
   [D_800D2101, D_800D2100). */
void func_1508295C(s32 arg0, s32 arg1, s32 firstRange) {
    s32 start;
    s32 end;
    s32 i;

    if (firstRange != 0) {
        start = 0;
        end = D_800D2101;
    } else {
        start = D_800D2101;
        end = D_800D2100;
    }

    for (i = start; i < end; i++) {
        func_150825C0(i, arg1);
    }
}

void func_150829D8(GameObj *obj) {
    obj->unkF8 &= 0xF7FFFFFD;
    obj->unkF8 |= 4;
    if ((obj->unk31C != NULL) && (obj->unk31C->unk56 == 0)) {
        switch (D_800BE9F0) {
        case 0:
        case 10:
        case 14:
        case 18:
        case 19:
        case 25:
        case 27:
        case 44:
        case 60:
        case 61:
        case 66:
        case 67:
        case 68:
            break;
        default:
            obj->unkF8 |= 2;
            break;
        }
    }
}

/* Creates and initialises a game object from spawn record spawnIdx (slot: 1-based object slot to
   reuse, 0 for the default). Returns the 1-based slot, or 0 on failure. */
s32 func_15082A44(SpawnRecord *spawn, s32 spawnIdx, s32 arg2, s32 arg3, s32 slot) {
    s32 type;
    GameObj *obj;
    s32 idx;
    s32 ret;
    s32 t;
    s32 i;
    s32 found;
    u32 model;
    ObjAlloc260 *mem;
    u16 angle;

    if (D_800D210C[spawnIdx] & ~0x80) {
        return 0;
    }
    type = spawn->type;
    if (slot != 0) {
        obj = &D_800CC2D0[slot - 1];
        func_1505F188(obj);
    } else if ((spawnIdx != -1) && (spawnIdx < D_8008FD8C)) {
        obj = &D_800CC2D0[spawnIdx];
        func_1505F188(obj);
    } else {
        obj = func_1505ED34();
    }
    idx = obj - D_800CC2D0;
    if (idx == 25) {
        return 0;
    }
    if (spawnIdx != -1) {
        if (spawn->flags & 0x21) {
            D_800D210C[spawnIdx] = idx + 1;
        } else {
            D_800D210C[spawnIdx] = idx + 0x81;
        }
        obj->unk13F = spawnIdx;
        if (spawnIdx < D_8008FD8C) {
            obj->unk127 = spawnIdx;
        } else {
            obj->unk127 = 0xFF;
        }
    }
    obj->spawn = spawn;
    ret = idx + 1;
    func_15084488(spawn, obj->unk13F, arg3);
    if ((idx < D_8008FD8C) || (type == 0x98) || (type == 0x28)) {
        obj->unk31C = allocate_memory(sizeof(GameObjExt), 1, 0, 0);
        bzero(obj->unk31C, sizeof(GameObjExt));
        obj->unk31C->unk1B8 = 5.0f;
    }
    obj->unique_id = spawn->unique_id;
    obj->unk1D0 = 2;
    obj->xz_scale = spawn->xz_scale;
    obj->y_scale = spawn->y_scale;
    obj->unk154 = spawn->xz_scale;
    obj->unk158 = spawn->y_scale;
    obj->interaction_state = spawn->interaction_state;
    obj->x_position = spawn->x;
    obj->y_position = spawn->y;
    obj->z_position = spawn->z;
    obj->unk212 = 0xFF;
    obj->unk20F = 0xFF;
    obj->unk210 = 0xFF;
    obj->unk211 = 0xFF;
    obj->unk128 = 0xFF;
    obj->old_x_position = obj->x_position;
    obj->old_y_position = obj->y_position;
    obj->unk1CC = obj->y_position;
    obj->old_z_position = obj->z_position;
    if ((spawn->flags & 6) || (spawn->flags & 4) || (spawn->flags & 2)) {
        func_1506160C(obj, 4, 0, 0, 0);
    }
    obj->unk232 = spawn->unkD;
    if ((spawnIdx < D_8008FD8C) && (D_8008FE44[spawnIdx] == -2)) {
        obj->unk31C->unk84 = 1;
        obj->unk232 = 1;
    }
    {
        // matching: unused stack slot
        u8 pad;

        if (obj->unk127 != 0xFF) {
            obj->unk128 = func_151E5F64(obj->unk127);
            obj->unk31C->grenade_count = D_800D2130;
            obj->unk31C->unk1AF = 0x40;
            obj->unk31C->unk1B0 = -1;
        }
    }
    obj->health = spawn->unkE & 0xF;
    obj->unk1E5 = spawn->unkF;
    obj->unk1E6 = obj->unk1E5;
    obj->unk221 = 1;
    obj->unk23C = spawn->unk1C;
    obj->unk23D = spawn->unk1D;
    obj->unk76 = spawn->angle << 8;
    angle = obj->unk76;
    obj->unk7A = angle;
    obj->unk78 = angle;
    obj->unk40 = (angle + 0x4000) * (360.0f / 65536.0f);
    obj->unk140 = arg2;
    obj->gravity = 6.0f;
    obj->unkF8 = spawn->objFlags;
    if (obj->unkF8 & 0x200000) {
        obj->unk224 = obj->y_position;
    }
    if (obj->unkF8 & 0x10000000) {
        obj->unk38 = obj->y_position - 500.0f;
    }
    if (obj->unkF8 & 0x800000) {
        D_800C67F0 = 1;
    }
    obj->unk1E4 = spawn->unk11;
    obj->unk66 = spawn->unk10;
    obj->unk1DD[0] = 120;
    obj->unk1DD[1] = 120;
    obj->unk1DD[2] = 120;
    obj->unk1E0[0] = 255;
    obj->unk1E0[1] = 255;
    obj->unk1E0[2] = 255;
    obj->unk23E = 4;
    obj->unk246 = 1;
    obj->unk247 = 100;
    obj->unk248 = 100;
    obj->unk1FC |= 2;
    func_150839B8(obj, type, spawn);
    t = spawn->unk13 & 0x7F;
    obj->unk92 = ((spawn->unk13 & 0x80) << 8) | t;
    obj->unk90 = spawn->unk14;
    func_150837D4(idx, type, 1);
    if (type != 0xFF) {
        func_150838EC(obj, 0, 0, 1.0f);
    } else {
        obj->unk5 = 3;
    }
    if (obj->unkF8 & 0x4000) {
        func_1503DA9C(obj, obj->id, 0, 0);
    }
    if (spawn->partSet) {
        func_15083568(obj, spawn->partSet, 1.0f, 0);
    }
    if (obj->unk127 != 0xFF) {
        obj->health = func_150859AC(obj->unk127, 4);
        obj->unkB2 = D_800D2132;
        func_150829D8(obj);
        func_15083384(obj, func_150859AC(idx, 1));
    }
    if (((obj->unk66 & 0xC) == 8) || ((obj->unk66 & 0xC) == 0xC)) {
        func_150832AC(obj);
    } else {
        obj->unk314 = NULL;
    }
    obj->unk288 = func_15084CB0(type);
    if (D_800BE616 != 0) {
        t = func_151E5F64(obj->unk127);
        if (t >= 4) {
            t = 3;
        }
        if (type == 0x3B) {
            obj->unk68 = t + 1;
        } else if (type == 0x77) {
            obj->unk68 = t + 0x10;
        }
    }
    if ((D_80038080 == 0) && (D_800BE9F0 != 29) && (spawn->unk2E == 0xFF)) {
        found = obj->modelCount;
        if (found == 0) {
            found = 1;
        }
        for (i = 0; i < found; i++) {
            D_800D2040[obj->modelIds[i]] = 0xFF;
        }
    }
    if (D_800BE9F0 == 29) {
        obj->unk66 &= ~0x20;
    }
    func_1514D15C(obj);
    model = func_150849A0(obj);
    found = 0;
    for (i = 0; i < 2; i++) {
        if (model == D_800A7230[i][0]) {
            found = 1;
            break;
        }
    }
    if (found) {
        mem = allocate_memory(sizeof(ObjAlloc260), 1, 0, 2);
        obj->unk260 = mem;
        if (mem != NULL) {
            mem->unk8 = 0;
            mem->unk9 = 0xFF;
            mem->unk0 = 0.0f;
            mem->unk4 = 1.0f;
        }
    }
    func_15084C30(obj);
    return ret;
}

void func_150832AC(GameObj *obj) {
    SpawnRecord *spawn;
    u8 n;
    s32 i;
    s32 count;

    spawn = obj->spawn;
    if (spawn != NULL) {
        n = spawn->unk2F;
        if (n == 0) {
            if (obj->unk127 == 0) {
                count = 5;
            } else {
                count = 3;
            }
        } else {
            count = n;
        }
        for (i = 0; i <= D_80082FA0; i++) {
            obj->unk304[i] = func_1515D480(count);
        }
        if (obj->unk304[i - 1] != 0) {
            obj->unk301 = count;
        }
        obj->unk314 = func_1515D440();
    }
}

void func_15083384(GameObj *obj, s32 arg1) {
    s32 partSet;
    s32 group;
    GameObjExt *ext;

    partSet = -1;
    switch (arg1) {
    case 37:
    case 38:
        ext = obj->unk31C;
        if (ext != NULL) {
            ext->unk1B2 = 1;
        }
        break;
    case 9:
        obj->unk94 |= 2;
        break;
    case 13:
        partSet = 0x76;
        break;
    case 14:
        partSet = 0x78;
        break;
    case 15:
        partSet = 0x77;
        obj->unk94 |= 2;
        break;
    case 29:
    case 30:
    case 31:
    case 32:
        {
            s32 n;

            n = arg1 - 29;
            obj->unk94 = 0x1E;
            if (n < 2) {
                obj->unk94 &= ~0x14;
            } else {
                obj->unk94 &= ~8;
            }
            if (n >> 1) {
                obj->unk94 &= ~2;
            }
        }
        break;
    case 17: case 18: case 19: case 20: case 21: case 22:
    case 23: case 24: case 25: case 26: case 27: case 28:
        group = (arg1 - 17) >> 2;
        obj->unk94 = 0x3E;
        switch (group) {
        case 0:
            obj->unk94 &= ~4;
            break;
        case 1:
            obj->unk94 &= ~8;
            break;
        case 2:
            obj->unk94 &= ~0x10;
            break;
        }
        switch (arg1 - group * 4 - 17) {
        case 1:
            obj->unk94 &= ~2;
            break;
        case 2:
            obj->unk94 &= ~0x20;
            break;
        case 3:
            obj->unk94 &= ~0x22;
            break;
        }
        break;
    }
    if (partSet != -1) {
        func_15083568(obj, partSet, 0.0f, 0);
    }
}

/* Creates every part of part set setId (1-based) on parent; returns the last part created. */
GameObj *func_15083568(GameObj *parent, s32 setId, f32 arg2, s32 arg3) {
    PartDef *parts;
    GameObj *ret;
    s32 i;

    setId--;
    ret = NULL;
    parts = D_80086CC4[setId].parts;
    for (i = 0; i < D_80086CC4[setId].count; i++) {
        if (parts[i].type == 0) {
            ret = func_15083AC8(parent, parts[i].unk1, parts[i].unk0, 0, parts[i].unk2, parts[i].unk4, parts[i].unk5, parts[i].unk6, arg2);
        } else {
            ret = func_15030AF4(parent, parts[i].unk0, parts[i].unk1, parts[i].unk7, parts[i].unk4, parts[i].unk5, arg3, setId + 1, parts[i].unk2,
                                parts[i].data, (parts[i].type == 2) ? parts[i].unk6 : -1, parts[i].unkE, parts[i].unkF);
        }
    }
    return ret;
}

/* Removes the child objects of part set setId (1-based) that belong to parent. */
void func_150836CC(GameObj *parent, s32 setId) {
    PartSet *set;
    s32 i;
    PartDef *parts;

    func_150302F0(parent, setId);
    setId--;
    set = &D_80086CC4[setId];
    parts = set->parts;
    for (i = 0; i < set->count; i++) {
        GameObj *child;

        if (parts[i].type == 0) {
            child = func_1505F0AC(parts[i].unk0);
            if (child != NULL) {
                if (child->unk65 == ((parent - D_800CC2D0) + 1)) {
                    func_15060F28(child, 0);
                }
            }
        }
    }
}

/* Sets object objIdx's model to modelId (0xFF for none) and loads its per-model state. */
void func_150837D4(s32 objIdx, s32 modelId, s32 arg2) {
    GameObj *obj = &D_800CC2D0[objIdx];
    ModelDef *def;

    obj->id = modelId;
    if (modelId == 0xFF) {
        obj->unkE4 = 0;
        obj->unkE6 = 0;
        obj->unkE8 = 0;
        obj->unkD2 = 70;
        obj->unkD4 = 70;
        obj->unkD6 = 0;
        obj->unk160 = 0;
        obj->unkC8 = 0.0f;
    } else {
        obj->unkC8 = D_800D1C90[modelId]->unk6;
        func_15062BDC(obj, obj->xz_scale, obj->y_scale);
        def = D_800D1C90[modelId];
        obj->unk5 = def->unk12;
    }
    obj->unk6 = func_15084D00(obj);
    if (D_800C5A90[obj->unk6] != 0) {
        obj->unk58 = func_1502B020(0, 2, 2, obj->unk6);
    }
}

/* Allocates the animation state block of obj's model. Returns 1 only if the allocation fails. */
s32 func_150838EC(GameObj *obj, s32 arg1, s32 arg2, f32 arg3) {
    s32 modelId;

    modelId = obj->id;
    if (D_800D1588[modelId] == 0) {
        return 0;
    }
    if (D_800C5A90[modelId] == 0) {
        return 0;
    }
    obj->unk2D0 = allocate_memory(0x3E0, 1, 2, 0);
    if (obj->unk2D0 == NULL) {
        return 1;
    }
    bzero(obj->unk2D0, 0x40);
    func_1505E650(obj, arg1, arg3, 0.0f, 0.0f, 0.0f, arg2);
    return 0;
}

/* Copies model modelId's defaults from its ModelDef into obj; spawn (may be NULL) overrides unk10. */
void func_150839B8(GameObj *obj, s32 modelId, SpawnRecord *spawn) {
    ModelDef *def;
    u16 override;

    if (modelId != 0xFF) {
        def = D_800D1C90[modelId];
        if (spawn != NULL) {
            override = spawn->unk2C;
            if (override == 0) {
                obj->unk10 = def->unk2A;
            } else {
                if (override == 1) {
                    obj->unk10 = 0;
                } else {
                    obj->unk10 = override;
                }
            }
            if (D_800BE9F0 == 29) {
                obj->unk10 = 1000;
            }
        } else {
            obj->unk10 = 0;
        }
        obj->unk13B = def->unk39;
        obj->unk2CB = def->unk33;
        obj->unk2CC = def->unk34;
        obj->unk5 = def->unk12;
        obj->unk68 = def->unk3B;
        obj->unk69 = def->unk3C;
        obj->unk160 = def->unk2;
        if (def->subModelCount != 0) {
            obj->modelIds = D_80086CAC[def->subModelList];
            obj->unk2C8 = def->subModelCount;
        }
        obj->modelCount = obj->unk2C8 + def->extraModelCount;
        if (def->unk29 != 0) {
            func_15036C70(obj);
        }
    }
}

/* Creates a child object of parent with model modelId (a part from func_15083568). */
GameObj *func_15083AC8(GameObj *parent, s32 arg1, s32 modelId, s32 arg3, s32 state, s32 arg5, s32 inheritScale, s32 arg7, f32 arg8) {
    GameObj *obj;
    // matching: unused stack slot
    s32 pad;
    s32 animFlag;
    s32 blocked;
    s32 i;

    func_1503D774(modelId, 0);
    func_1503D660(modelId, 0);
    obj = func_1505ED34();
    obj->unique_id = 0xFF - (obj - D_800CC2D0);
    obj->unkAB = arg3;
    obj->interaction_state = state;
    func_150615DC(obj);
    obj->unk232 = 1;
    obj->health = 1;
    obj->immune = 0xFF;
    obj->unk65 = (parent - D_800CC2D0) + 1;
    obj->unk5C = arg1;
    obj->unk123 = 1;
    obj->id = modelId;
    obj->unk66 = parent->unk66;
    obj->unk66 &= ~0x10;
    // matching: a no-op |= 0 (a constant flag field upstream) keeps the reload of unk66
    obj->unk66 |= 0;
    obj->unk1DD[0] = parent->unk1DD[0];
    obj->unk1DD[1] = parent->unk1DD[1];
    obj->unk1DD[2] = parent->unk1DD[1];
    obj->unk1E0[0] = parent->unk1E0[0];
    obj->unk1E0[1] = parent->unk1E0[1];
    obj->unk1E0[2] = parent->unk1E0[2];
    if (inheritScale == 0) {
        obj->xz_scale = 1.0f;
        obj->y_scale = 1.0f;
        obj->unk154 = 1.0f;
        obj->unk158 = 1.0f;
    } else {
        obj->xz_scale = parent->xz_scale;
        obj->y_scale = parent->y_scale;
        obj->unk154 = parent->unk154;
        obj->unk158 = parent->unk158;
    }
    obj->unk101 = inheritScale;
    obj->unkAC = arg5;
    func_1503D774(modelId, 0);
    if (arg5 & 1) {
        obj->unkF8 |= 0x4000;
        obj->unk270 = (f32)(u32)D_800D1C90[modelId]->unkE * obj->xz_scale;
    }
    animFlag = 0;
    if (arg5 & 2) {
        animFlag = 1;
    }
    for (i = 0; i <= D_80082FA0; i++) {
        obj->unk304[i] = func_1515D480(1);
    }
    if (obj->unk304[i - 1] != 0) {
        obj->unk301 = 1;
    }
    obj->unk314 = func_1515D440();
    if (D_800C35EA == 1) {
        blocked = 1;
    } else {
        blocked = 0;
    }
    if (blocked) {
        D_800C3638 = 0;
    }
    func_150839B8(obj, modelId, NULL);
    func_150837D4(obj - D_800CC2D0, modelId, 1);
    func_150838EC(obj, arg7, animFlag, arg8);
    if (blocked) {
        D_800C3638 = 1;
    }
    return obj;
}

/* Returns the lowest unique id (1..255) that no spawn record uses. */
s32 func_15083DA0(void) {
    s32 id;
    s32 i;

    id = 1;
    for (i = 0; i < D_800D2100; i++) {
        if (id == D_800D20FC[i].unique_id) {
            id++;
            i = 0;
        }
    }
    if (id >= 0x100) {
        id = 0xFF;
    }
    return id;
}

/* Returns the index of the spawn record with unique id id, or -1. */
s32 func_15083E0C(u8 id) {
    s32 i;

    if (id == 0) {
        return -1;
    }
    for (i = 0; i < D_800D2100; i++) {
        if (D_800D20FC[i].unique_id == id) {
            return &D_800D20FC[i] - D_800D20FC;
        }
    }
    return -1;
}

/* Returns the active object with unique id id, or NULL. */
GameObj *func_15083E90(u8 id) {
    GameObj *obj;
    s32 i;

    if (id == 0) {
        return NULL;
    }
    obj = D_800CC2D0;
    for (i = 0; i < 26; i++) {
        if (obj->interaction_state != 0 && obj->unique_id == id) {
            return obj;
        }
        obj++;
    }
    return NULL;
}

/* Returns the slot of the active object with unique id id, or -1. */
s32 func_15083FB0(u8 id) {
    GameObj *obj;

    obj = func_15083E90(id);
    if (obj != NULL) {
        return obj - D_800CC2D0;
    }
    return -1;
}

s32 func_15084000(void) {
    GameObj *obj = func_1505EEF4();

    if (obj != NULL) {
        return obj - D_800CC2D0;
    }
    return -1;
}

/* Sets up obj's two data pointers for its model'th model: shared with D_800D19A0[id] or private
   copies, depending on the owning spawn record's mode (unk10 & 3). Returns 1 if an allocation failed. */
s32 func_15084044(GameObj *obj, s32 model) {
    u8 *slot;
    SpawnRecord *spawn;
    s32 mode;
    u8 id;
    s32 val;
    s32 ret;
    s32 i;
    s32 size;

    slot = (u8 *)((s32)obj + model * 8);
    if (obj->unk65 != 0) {
        spawn = D_800CC2D0[obj->unk65 - 1].spawn;
    } else {
        spawn = obj->spawn;
    }
    id = obj->modelIds[model];
    mode = (spawn != NULL) ? (spawn->unk10 & 3) : 0;
    if (id == 0) {
        mode = 2;
    }
    if (*(s32 *)(slot + 0x28C) != 0 && *(s32 *)(slot + 0x290) != 0) {
        return 0;
    }
    val = (s32)D_800D19A0[id];
    if ((s32)D_800D19A0[id] == 0) {
        return 0;
    }
    ret = 0;
    if (mode == 0) {
        val = (s32)D_800D19A0[id];
        *(s32 *)(slot + 0x290) = val;
        *(s32 *)(slot + 0x28C) = val;
        D_800D1F80[id] += 2;
    } else if (mode == 1) {
        if (*(s32 *)(slot + 0x28C) == 0 || *(s32 *)(slot + 0x28C) == (s32)D_800D19A0[id]) {
            if (D_800D1F80[id] != 0) {
                size = D_800C57A0[id] * 16;
                *(s32 *)(slot + 0x28C) = (s32)allocate_memory(size, 1, 2, 2);
                if (*(s32 *)(slot + 0x28C) == 0) {
                    ret = 1;
                } else if (*(s32 *)(slot + 0x28C) != 0) {
                    bcopy((void *)(s32)D_800D19A0[id], (void *)*(s32 *)(slot + 0x28C), size);
                } else {
                    *(s32 *)(slot + 0x28C) = (s32)D_800D19A0[id];
                }
            } else {
                val = (s32)D_800D19A0[id];
        *(s32 *)(slot + 0x290) = val;
        *(s32 *)(slot + 0x28C) = val;
                D_800D1F80[id] += 2;
            }
        }
        *(s32 *)(slot + 0x290) = *(s32 *)(slot + 0x28C);
    } else if (mode == 2) {
        for (i = 0; i < 2; i++) {
            if (((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] == 0 || ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] == (s32)D_800D19A0[id] || (i == 1 && *(s32 *)(slot + 0x28C) == *(s32 *)(slot + 0x290))) {
                if (D_800D1F80[id] != 0) {
                    size = D_800C57A0[id] * 16;
                    ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] = (s32)allocate_memory(size, 1, 2, 2);
                    if (((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] == 0) {
                        ret = 1;
                        if (i == 1) {
                            if (*(s32 *)(slot + 0x28C) != (s32)D_800D19A0[id]) {
                                func_10004074((void *)*(s32 *)(slot + 0x28C));
                            } else {
                                D_800D1F80[id] -= 1;
                            }
                            *(s32 *)(slot + 0x28C) = 0;
                        }
                        break;
                    } else if (((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] != 0) {
                        bcopy((void *)(s32)D_800D19A0[id], (void *)((s32 (*)[2])((u8 *)obj + 0x28C))[model][i], size);
                    } else {
                        ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] = (s32)D_800D19A0[id];
                    }
                } else {
                    ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] = (s32)D_800D19A0[id];
                    D_800D1F80[id] += 1;
                }
            }
        }
    }
    return ret;
}

/* Releases the data pointers of all of obj's models. */
void func_1508434C(GameObj *obj) {
    s32 count;
    s32 i;

    count = obj->modelCount;
    if (count == 0) {
        count = 1;
    }
    for (i = 0; i < count; i++) {
        func_150843AC(obj, i);
    }
}

/* Releases the data pointers of obj's model'th model: frees private copies, drops the
   reference count of shared ones. */
void func_150843AC(GameObj *obj, s32 model) {
    // matching: a named trip count takes its saved register before the D_800D19A0 base
    u8 limit;
    u8 *data;
    s32 i;
    u8 id;

    if (obj->modelData[model][0] == NULL && obj->modelData[model][1] == NULL) {
        return;
    }
    limit = 2;
    id = obj->modelIds[model];
    for (i = 0; i != limit; i++) {
        data = obj->modelData[model][i];
        if (data != NULL) {
            if (data != D_800D19A0[id]) {
                func_100043B4((s32 *)data, 3);
            } else {
                D_800D1F80[id] -= 1;
            }
            obj->modelData[model][i] = NULL;
        }
    }
}

/* Loads model spawn->type and all of its sub-models. */
void func_15084488(SpawnRecord *spawn, s32 arg1, s32 arg2) {
    s32 count;
    s32 i;
    ModelDef *def;
    u8 *list;
    s32 modelId;

    modelId = spawn->type;
    if (modelId != 0xFF) {
        func_1503D774(modelId, arg2);
        def = D_800D1C90[modelId];
        list = &spawn->type;
        count = def->subModelCount;
        if (count == 0) {
            count = 1;
        } else {
            list = D_80086CAC[def->subModelList];
        }
        count += def->extraModelCount;
        for (i = 0; i < count; i++) {
            func_1503D774(list[i], arg2);
            func_1503D660(list[i], arg2);
        }
    }
}

/* Releases obj's model data, and frees each of its models (and their tables) that no other
   active object still uses. */
void func_15084558(GameObj *self) {
    s32 id;
    u8 *list;
    s32 i;
    s32 j;
    s32 count;
    s32 k;
    s32 unique;
    s32 freeData;
    s32 freeDef;
    GameObj *obj;
    u8 otherCount;
    u8 *otherList;
    // matching: unused stack slot
    s32 pad;
    s32 data;
    u8 *block;
    s32 table;

    id = self->id;
    if (id != 0xFF && D_800D19A0[id] != NULL) {
        count = self->modelCount;
        list = self->modelIds;
        func_1508434C(self);
        for (i = 0; i < count; i++) {
            unique = 1;
            freeData = 1;
            freeDef = 1;
            for (j = 0; j < i; j++) {
                if (list[j] == list[i]) {
                }
            }
            for (j = 0; j < 25; j++) {
                if (j != (self - D_800CC2D0)) {
                    obj = &D_800CC2D0[j];
                    if (obj->interaction_state != 0 && obj->id != 0xFF && obj->unk5 != 2) {
                        otherCount = obj->modelCount;
                        otherList = obj->modelIds;
                        for (k = 0; k < otherCount; k++) {
                            if (otherList[k] == list[i]) {
                                unique = 0;
                            }
                            if (D_800D1588[otherList[k]] == D_800D1588[list[i]] && D_800D1588[otherList[k]] != 0) {
                                freeData = 0;
                            }
                            if (D_800D1C90[otherList[k]] == D_800D1C90[list[i]] && D_800D1C90[otherList[k]] != NULL) {
                                freeDef = 0;
                            }
                        }
                    }
                }
            }
            if (freeData && D_800D1588[list[i]] != 0) {
                data = D_800D1588[list[i]];
                {
                    func_100043B4((s32 *)(data - 0x10), 3);
                    for (k = 0; k < 187; k++) {
                        if (data == D_800D1588[k]) {
                            D_800D1588[k] = 0;
                        }
                    }
                }
            }
            if (freeDef) {
                block = (u8 *)D_800D1C90[list[i]];
                if (block != NULL) {
                    block -= 0x10;  // the allocation starts 0x10 bytes before the data
                    func_100043B4((s32 *)block, 3);
                    D_800D1C90[list[i]] = NULL;
                }
            }
            if (unique) {
                if (D_800D19A0[list[i]] != NULL) {
                    if (D_800C5C08[id] != 0) {
                        func_10004074((void *)D_800C5C08[id]);
                        D_800C5C08[id] = 0;
                        func_10004074((void *)D_800C6070[id]);
                        D_800C6070[id] = 0;
                    }
                    table = D_800C6360[id];
                    if (table != 0) {
                        func_10004074((void *)table);
                        D_800C6360[id] = 0;
                    }
                }
            }
        }
    }
}

/* Returns obj's current model id (modelIds[unk1C9 - 1], or the first). */
u8 func_150849A0(GameObj *obj) {
    s32 current = obj->unk1C9;

    if (current != 0) {
        return obj->modelIds[current - 1];
    }
    return obj->modelIds[0];
}

/* Like func_150849A0, falling back to modelIds[unk2C8 - 1]; stores the index in *outIndex. */
u8 func_150849CC(GameObj *obj, s32 *outIndex) {
    s32 index;
    s32 n;

    n = obj->unk1C9;
    if (n != 0) {
        index = n - 1;
    } else {
        n = obj->unk2C8;
        if (n != 0) {
            index = n - 1;
        } else {
            index = 0;
        }
    }
    if (outIndex != NULL) {
        *outIndex = index;
    }
    return obj->modelIds[index];
}

/* Ticks the D_800D2040 countdowns; a model whose countdown expires while it has no D_800C5C08
   table is unloaded and every object's references to it are dropped. */
void func_15084A18(void) {
    s32 i;
    s32 m;
    GameObj *obj;
    s32 j;
    s32 k;
    ModelReloc *relocs;
    u8 *timer;

    timer = D_800D2040;
    for (i = 0; i < 187; i++, timer++) {
        if (*timer == 0xFF || *timer == 0) {
            continue;
        }
        if (*timer == 1) {
            if (D_800D19A0[i] != NULL && D_800C5C08[i] == 0) {
                for (m = 0; m < 25; m++) {
                    obj = &D_800CC2D0[m];
                    if (obj->interaction_state != 0) {
                        for (j = 0; j < obj->modelCount; j++) {
                            for (k = 0; k < 2; k++) {
                                if (obj->modelData[j][k] == D_800D19A0[i]) {
                                    obj->modelData[j][k] = NULL;
                                }
                            }
                            if (i == obj->modelIds[j]) {
                                func_150843AC(obj, j);
                            }
                        }
                    }
                }
                relocs = D_800C5338[i];
                for (k = 0; k < D_800C5628[i]; k++) {
                    if (relocs[k].unk0 >= 0x10000000U) {
                        func_1510D720(relocs[k].unk4);
                    }
                }
                func_10004074(D_800D19A0[i] - 0x38);
                D_800D1F80[i] = 0;
                D_800D19A0[i] = NULL;
            }
        }
        (*timer)--;
    }
}

void func_15084C30(GameObj *obj) {
    struct197 *anim;
    f32 phase;

    if (obj->id == 0x94) {
        anim = obj->unk2D0;
        phase = (u32)func_150ADA20() % (u32)(s32)anim->unk18;
        anim->unk8 = phase;
    }
}

/* Returns the index of modelId in D_800BE598, or 0. */
s32 func_15084CB0(s32 modelId) {
    s32 ret;
    s32 i;

    ret = 0;
    for (i = 0; i < D_800BE590; i++) {
        if (modelId == D_800BE598[i]) {
            ret = i;
            break;
        }
    }
    return ret;
}
