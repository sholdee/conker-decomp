#include <ultra64.h>
#define func_1513D2F0 func_1513D2F0_hdr
#include "functions.h"
#include "variables.h"
#undef func_1513D2F0
void *func_1513D2F0(void *arg0, void *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8, s32 arg9, u8 argA, s32 argB);

s32 func_1513FAB4(void *arg0, s32 arg1, f32 *arg2, s16 arg3);
s32 func_151BB0E0();
extern f32 D_800AA8E0;
extern f32 D_800AA8E8;
extern f32 D_800AA8EC;
extern f32 D_800AA8F0;
extern f32 D_800AA8F4;
extern f32 D_800AA8F8;
extern f32 D_800AA8FC;
extern f32 D_800AA900;
extern f32 D_800AA904;
extern f32 D_800AA908;
extern f32 D_800AA90C;
void func_15150F90(void *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_15153634(void *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);


typedef struct {
    u8 unk0;
    s8 unk1;
    u8 unk2;
    u8 pad3;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 unk24;
    u8 unk25;
    u8 pad26[0xA];
    s32 unk30;
    s32 unk34;
    u8 unk38;
    u8 unk39;
} Spawn151B9F00;

typedef struct {
    struct127 *owner;
    u8 ownerId;
} Extra151B9F00;

typedef struct {
    u8 pad0[0x50];
    s32 unk50; /* offset of the spawned object's extra block, from 0xF8 */
} Obj151B9F00;

extern s8 D_800E0BCE[];
extern s8 D_8008FD90;
void *func_1513B5E0(void *, s32, s32, u8, s32);
void *func_151B9F00(struct127 *arg0, u8 arg1, s32 arg2);

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_151BA468;

typedef struct {
    u8  pad[0x3B];
    u8  unk3B;
} SomeObj_151BA468;

typedef struct {
    u8  pad[0x50];
    s32 unk50;
} Base_151BA468;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_151BA468;
void func_151BA468(struct260 *arg0, s32 arg1, u8 arg2);

typedef struct { f32 x, y, z; } V3_151BA518;
typedef struct {
    s16 a0, a2, a4, a6;
    s32 a8, aC;
    f32 f10, f14, f18;
    f32 f1c, f20, f24, f28, f2c, f30;
    s32 i34, i38;
    f32 f3c, f40, f44, f48;
    s16 s4c[6];
    s8 b58;
} Cfg151BA518;
typedef struct {
    f32 f0, f4, f8, fC;
    V3_151BA518 pos;
    f32 f1c, f20, f24;
    V3_151BA518 t;
    V3_151BA518 vel;
    f32 f40, f44, f48, f4c;
    s32 i50;
    s16 s54, s56;
    s8 b58;
    s32 i5c;
    u8 b60, b61, b62, b63, b64, b65, b66, b67, b68, b69;
    s8 b6a;
    void *p6c;
    u8 b70;
    s16 s72, s74;
    s32 pad78, pad7c;
} Part151BA518;
extern V3_151BA518 D_800AA590, D_800AA59C, D_800AA5A8, D_800AA5B4[], D_800AA5CC[], D_800AA5FC[];
extern f32 D_800AA62C[];
extern V3_151BA518 D_800AA63C, D_800AA648[], D_800AA660[], D_800AA678[], D_800AA6A8[], D_800AA6D8[];
extern f32 D_800AA708[];
extern f32 D_800AA7BC, D_800AA7C0, D_800AA7C4, D_800AA7C8, D_800AA7CC, D_800AA7D0, D_800AA7D4, D_800AA7D8, D_800AA7DC;
void func_1514FCE8(s16 *, u8, s32);
extern void func_1504715C(void *, void *);
extern void func_1503F404(s32, f32 *, f32 *, f32 *, V3_151BA518 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern void func_1513264C(Part151BA518 *, s32, s32, void *, s32, u8, s32);
extern void func_15143134(V3_151BA518 *, V3_151BA518 *, s32);
void func_151BA518(void *arg0, u8 arg1, s32 arg2);
void func_151BB044(struct127 *arg0);

typedef struct { f32 x, y, z; } Vec3_151BB61C;
typedef struct { s16 v[9]; } Surf_151BB61C;
typedef struct {
    Vec3_151BB61C dir;      /* 0x00 */
    Surf_151BB61C surf;     /* 0x0C */
    u8 pad1E[2];
    f32 unk20;
    s32 unk24;
    s32 unk28;
    Vec3_151BB61C pos;      /* 0x2C */
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s32 unk50;
    s32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    s16 unk68;
    s16 unk6A;
    s16 unk6C;
    s16 unk6E;
    s16 unk70;
    s16 unk72;
    s8 unk74;
} Fbfc_151BB61C;
typedef struct {
    s32 count;
    s32 spread;
    Vec3_151BB61C pos;      /* 0x08 */
    Vec3_151BB61C vel;      /* 0x14 */
    Surf_151BB61C surf;     /* 0x20 */
    u8 pad32[2];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
} Desc_151BB61C;

typedef struct { s32 v[9]; } Colors_151BB61C;
typedef struct { f32 v[9]; } Scales_151BB61C;
extern Colors_151BB61C D_800AA718;
extern Scales_151BB61C D_800AA73C;
void func_15055A2C(s32, f32, f32, f32, s32);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, void *, f32 *, f32 *, f32 *, s32, s32 *, s32, f32);
s32 func_15145C90(s32);
s32 func_15144E80(void *, Vec3_151BB61C *, Vec3_151BB61C *, Vec3_151BB61C *);
void func_1514FBFC(void *, u8, s32);
void func_15151D6C(void *, s32 *, f32 *, s32, u8, s32);
void func_151C04F8(f32 *, u8, s32);
void func_151C05A4(void *, u8, s32);
void func_151C05F0(void *, u8, s32);
void func_151C0644(void *, u8, s32);
void func_151BBEE4(f32 *arg0, u8 arg1, s32 arg2);
void func_151BBFBC(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_151BB61C(s32 arg0, Vec3_151BB61C *arg1, Vec3_151BB61C *arg2, u8 arg3, s32 arg4);

typedef struct Vec3i_151BBA9C {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_151BBA9C;

typedef struct Effect_151BBA9C {
    s16 field00;
    s16 field02;
    s16 field04;
    s16 field06;
    s32 field08;
    s32 field0C;
    Vec3i_151BBA9C position;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    f32 field2C;
    f32 field30;
    s32 field34;
    s32 field38;
    f32 field3C;
    f32 field40;
    f32 field44;
    f32 field48;
    s16 field4C;
    s16 field4E;
    s16 field50;
    s16 field52;
    s16 field54;
    s16 field56;
    s8 field58;
    u8 pad59[3];
} Effect_151BBA9C;

typedef struct Config_151BBA9C {
    s32 countBase, countRange;
    Vec3_151BB61C position;
    s16 yawBase, yawRange, pitchBase, pitchRange;
    f32 speedBase, speedRange, durationBase, durationRange;
    s16 lifeBase, lifeRange;
    f32 sizeBase, sizeRange, jitter;
} Config_151BBA9C;

void func_1514FCE8(s16 *, u8, s32);
void func_15152190(void *, void *, void *, s32, f32, s32, s32, s32);
s32 func_15046C80(f32 *, u16, f32, void *);
void func_1504715C(void *, void *);
extern Vec3_151BB61C D_8008FBA0;
extern Vec3_151BB61C D_8008FBAC;
extern f32 D_800AA80C;
extern f32 D_800AA810;
extern f32 D_800AA814;
extern f32 D_800AA818;
extern f32 D_800AA81C;
extern f32 D_800AA820;
extern f32 D_800AA824;
extern f32 D_800AA828;
void func_151BBA9C(s32 arg0, f32 *arg1, f32 *arg2, u8 arg3, s32 arg4);
void func_151BBEE4(f32 *arg0, u8 arg1, s32 arg2);

void func_150E7FEC(f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_151BBFBC(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern f32 D_800AA82C;
extern u8 D_800AA760;
extern u8 D_800AA76C;
void func_15160CDC(s32, s32, void *, void *, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_151BC074(s32 arg0);

extern f32 D_800AA830;
extern f32 D_800DCA24;
void *func_15134908(void *, s32, u8, s32);
void *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void *func_15163414(Header *, f32 *, f32 *, f32 *, s8, u8, u8, u8, u8, u8, u8, u8, s32, u8, s32);

typedef struct {
    f32 *unk0;
    f32 *unk4;
    f32 *unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 pad1A[2];
} Spawn151BC104;

typedef struct {
    struct127 *owner;
    u8 ownerId;
} Extra151BC104;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec151BC104;

typedef struct {
    Vec151BC104 unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
} Header151BC104;

typedef struct {
    struct127 *owner;
    u8 ownerId;
    u8 pad5[3];
    Vec151BC104 unk8;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
} Payload151BC104;
void func_151BC104(struct127 *arg0, u8 arg1, s32 arg2);

void func_15143794(s16, s16, f32, void *);
void func_151A26EC(f32 *, f32 *, f32 *, f32, f32, f32, s32, s32, s32,
                    s32, s32, s32, s32, s32, s32, s32, s32);
void func_151BC370(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, void *arg6);
s32 func_151BC580(s32 *arg0);

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_151BC5A4;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_151BC5A4;
void func_151BC5A4(struct260 *arg0, s32 arg1, u8 arg2);

typedef struct Game1E9A30Trail {
    s32 x;
    s32 y;
    s32 z;
    s16 lifetime;
    u8 alpha;
    u8 pad_F[5];
} Game1E9A30Trail;

typedef struct Vec3i_151BC64C {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_151BC64C;
s32 func_151BC64C(u8 *actor);

typedef struct Trail151BC794 {
    struct17 pos;
    s16 lifetime;
    u8 alpha;
    u8 padF;
    f32 rot;
} Trail151BC794;

typedef struct Src151BC794 {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
} Src151BC794;

typedef struct Data151BC794 {
    Src151BC794 *src;
    u8 unk4;
    u8 pad5[3];
    struct17 pos;
    f32 count;
    f32 unk18;
    f32 unk1C;
    f32 rot;
} Data151BC794;

typedef struct Actor151BC794 {
    u8 pad0[0x10];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x65];
    Trail151BC794 *entries;
    Data151BC794 *data;
} Actor151BC794;

extern f32 func_15143E64(struct17 *);
s32 func_151BC794(Actor151BC794 *arg0);

typedef struct Trail151BCA90 {
    struct17 pos;
    s16 lifetime;
    u8 alpha;
    u8 padF;
    f32 rot;
} Trail151BCA90;

typedef struct Actor151BCA90 {
    u8 pad0[0x10];
    struct17 pos;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x55];
    u8 buffers[0x10];
    Trail151BCA90 *entries;
} Actor151BCA90;

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern struct17 *func_15144B34(s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
extern u8 D_80090CD4[];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];
Gfx *func_151BCA90(Actor151BCA90 *arg0, Gfx *gfx, s16 arg2);
void func_151BD21C(void *arg0, void *arg1, u8 arg2);
s32 func_151BD2BC(s32 *arg0);

extern f32 D_800AA850;
extern f32 D_800AA860;
extern f32 D_800AA864;

typedef struct {
    /* 0x00 */ u8  unk0;
    /* 0x01 */ u8  pad1[0x3];
    /* 0x04 */ void *unk4;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  pad9[0x3];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8  unk24;
    /* 0x25 */ u8  pad25[0x1];
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8  unk2E;
    /* 0x2F */ u8  unk2F;
    /* 0x30 */ s8  unk30;
    /* 0x31 */ u8  pad31[0x3];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ s8  unk39;
} struct_E9FE0_2F8;

void *func_15134DAC(struct_E9FE0_2F8 *, s32);
void func_151BD2F8(void *arg0, s32 arg1, s32 arg2);
s32 func_151BD42C(u8 *arg0);

typedef struct {
    s32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30[3];
    f32 unk3C[3];
    f32 unk48[3];
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
} Fx151BD43C;

typedef struct {
    u8 pad0[0x14];
    f32 unk14[3];
    u8 pad20[0x3B - 0x20];
    u8 unk3B;
    u8 pad3C[0x74 - 0x3C];
    u8 unk74;
} Owner151BD43C;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1C - 0xD];
    Owner151BD43C *unk1C;
    u8 pad20[0x74 - 0x20];
    f32 unk74;
} Actor151BD43C;

typedef struct {
    Owner151BD43C *owner;
    u8 ownerId;
} Extra151BD43C;

struct260 *func_15130374(void *, u8, s32, u8, s32);
void func_151BD43C(f32 *arg0, f32 *arg1, s32 arg2, s32 arg3, f32 arg4, Actor151BD43C *arg5);
f32 func_151BD750(s16 *arg0);

extern void func_1513170C(void);

struct S151BD79C_sub {
    s32 unk0;
    u8 unk4;
    u8 pad5[0xF];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

struct S151BD79C {
    u8 pad0[0x4C];
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    u8 padB0[0xB0 - 0x58];
    struct S151BD79C_sub *unkB0;
};
void func_151BD79C(struct S151BD79C *arg0);
void func_151BD7F4(s32 arg0);

s32 func_151407D0(void *, u32, void *, u8, u8, u8, u8, s8, u8, s32);

typedef struct Game1E9A30BurstData {
    void *owner;
    u8 id;
    u8 pad05[3];
    void *sparks[2];
    void *glows[2];
} Game1E9A30BurstData;

typedef struct Game1E9A30BurstSpark {
    void *owner;
    u8 id;
    u8 index;
    u8 pad06[2];
    f32 field08;
    f32 field0C;
    f32 field10;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    f32 field2C;
    f32 field30;
    f32 field34;
    f32 field38;
    f32 field3C;
    f32 field40;
    f32 field44;
    f32 field48;
    f32 field4C;
    void *object;
    f32 field54;
    s8 field58;
    u8 pad59[3];
} Game1E9A30BurstSpark;

typedef struct Game1E9A30BurstEffect {
    u8 kind;
    u8 mode;
    s16 flags;
    s16 lifetime;
    u8 pad06[2];
    s32 field08;
    s32 field0C;
    u8 color[4];
    f32 field14;
    f32 field18;
    struct17 field1C;
    struct17 field28;
    f32 field34;
    f32 field38;
    f32 field3C;
    s32 field40;
    u8 field44;
    u8 field45;
    u8 field46;
    u8 field47;
    s32 field48;
    u8 field4C;
    u8 pad4D[0xB];
} Game1E9A30BurstEffect;

typedef struct Game1E9A30BurstGlowShape {
    f32 field00;
    f32 field04;
    f32 field08;
    f32 field0C;
    f32 field10;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    s32 field24;
    s32 field28;
    s32 field2C;
    s32 field30;
    s32 field34;
    s32 field38;
    s32 field3C;
    s32 field40;
    s32 field44;
    f32 field48;
    s16 field4C;
    s16 field4E;
    s16 field50;
    s16 field52;
    u8 field54;
    u8 field55;
    u8 field56;
    u8 field57;
    u8 field58;
    s8 field59;
    u8 pad5A[2];
} Game1E9A30BurstGlowShape;

typedef struct Game1E9A30BurstGlow {
    s16 field00;
    u8 pad02[2];
    f32 field04;
    void *object;
    u8 index;
    u8 pad0D[3];
    void *owner;
    u8 id;
    u8 pad15[3];
} Game1E9A30BurstGlow;
void func_151BD828(u8 *arg0, u8 arg1, s32 arg2);

typedef struct {
    u8 pad0[0x48];
    f32 unk48;
    f32 unk4C;
    u8 pad50[8];
    u8 flags;
} Part151BDD8C;

typedef struct {
    u8 pad0[4];
    f32 unk4;
} Sub151BDD8C;

typedef struct {
    u8 pad0[0x34];
    struct17 start;
    struct17 end;
    u8 pad4C[0x110 - 0x4C];
    Part151BDD8C part;
} Obj151BDD8C_A;

typedef struct {
    u8 pad0[0x34];
    struct17 start;
    struct17 end;
    u8 pad4C[0xC];
    s32 flags58;
    u8 pad5C[0x170 - 0x5C];
    Sub151BDD8C sub;
} Obj151BDD8C_B;

typedef struct {
    struct127 *owner;
    u8 ownerId;
    Obj151BDD8C_A *a[2];
    Obj151BDD8C_B *b[2];
} Data151BDD8C;

typedef struct {
    u8 pad0[0xE];
    s16 state;
    u8 pad10[0x18];
    Data151BDD8C data;
} Effect151BDD8C;

extern struct17 D_800AA778;
extern struct17 D_800AA784;
extern struct17 D_800AA790;
extern struct17 D_800AA79C;
extern s32 func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);
void func_151BDD8C(Effect151BDD8C *effect);

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_151BE0AC;

typedef struct {
    s32 unk0;
    u8  b4;
} ArgB_151BE0AC;
void func_151BE0AC(s32 arg0, s32 arg1, u8 arg2);
void func_151BE138(struct260 *arg0);

extern void func_151BE138(struct260 *arg0);
void func_151BE1B8(struct260 *arg0);

extern void func_15149368(struct260 *arg0);
void func_151BE1E4(struct260 *arg0);

typedef struct {
    u8 pad0[0x8];
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
} Sub151BE210;

typedef struct {
    u8 pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x28];
    u8 unk5C;
    u8 pad5D[0xB3];
    union { Sub151BE210 unk110; struct { u8 pad[0x24]; f32 unk134; } s; } u;
} Obj151BE210;
s32 func_151BE210(Obj151BE210 *obj);

typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
} Obj151BE4B8;

typedef struct {
    Obj151BE4B8 *unk0;
    u8 unk4;
    u8 pad5[0x3B];
    f32 unk40;
    f32 unk44;
    u8 pad48[0xC];
    f32 unk54;
    u8 unk58;
} Sub151BE4B8;
s32 func_151BE4B8(void *arg0, s16 arg1);

typedef struct { s32 unk0; u8 unk4; } SubA_151BE558;
typedef struct { s32 unk0; union { s32 w4; u8 b4; } u4; u8 unk8; u8 unk9; } ArgB_151BE558;
void func_151BE558(struct260 *arg0, s32 arg1, u8 arg2);
void func_151BE604(s32 arg0, s32 arg1, u8 arg2);

struct Sub151BE644 {
    u8  pad0[0x5];
    u8  unk5;
    u8  pad6[0x4A];
    s32 unk50;
};
void func_151BE644(struct210 *arg0);
void func_151BE674(struct210 *arg0);
void func_151BE6A0(struct210 *arg0);

struct S4151BE6CC { s32 w[4]; };
extern struct S4151BE6CC D_800AA7A8;
extern f32 D_800AA8AC;

struct Bar151BE6CC {
    char pad48[0x48];
    f32 unk48;
};

struct Obj151BE6CC {
    char pad18[0x18];
    u8  unk18;
    char pad19[0x170 - 0x19];
    s16 unk170;
    char pad172[0x2];
    f32 unk174;
};
s32 func_151BE6CC(struct Obj151BE6CC *arg0);
void func_151BE788(s32 arg0, s32 arg1, u8 arg2);

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} BE7C8_elem;

typedef struct {
    u8 pad0[0x8];
    s32 *unk8;
    u8 unkC;
} BE7C8_sub;
void func_151BE7C8(struct210 *arg0);

extern void func_151BE7C8(struct210 *);
extern void func_151411A4(struct210 *);
void func_151BE7F8(struct210 *arg0);
void func_151BE824(struct210 *arg0);

struct S151BEB20 {
    u8 pad0[0x54];
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    u8 pad60[0x7C - 0x60];
    f32 mat[1][4][4];
    u8 padBC[0x120 - 0xBC];
    f32 unk120;
};
s32 func_151BEB20(struct S151BEB20 *arg0);
s32 func_151BEC94(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4);

typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[3];
    /* 0x010 */ u8 unk10;
    /* 0x011 */ u8 pad11[0x37];
    /* 0x048 */ u8 unk48;
    /* 0x049 */ u8 pad49[0xB];
    /* 0x054 */ struct17 unk54;
    /* 0x060 */ s32 unk60;
    /* 0x064 */ u8 pad64[0x98];
    /* 0x0FC */ s32 unkFC;
    /* 0x100 */ u8 pad100[0x1B];
    /* 0x11B */ u8 unk11B;
    /* 0x11C */ u8 pad11C[4];
} S151BECB8;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ struct17 unk4;
    /* 0x10 */ u8 unk10;
} S151BECB8Params;

extern void func_1515C1A0(struct127 *, f32 *, f32 *, f32 *);
extern f32 func_15143E64(struct17 *);
void func_151BF340(u8 arg0, s32 arg1);
void *func_151BEEE0(f32 arg0, struct17 *arg1, s32 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8);
void func_151BF0C8();
s32 func_151BEE94(struct127 *arg0);
s32 func_151BECB8(S151BECB8 *arg0);
s32 func_151BEE94(struct127 *arg0);

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ struct17 unk10;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ struct17 unk34;
    /* 0x40 */ struct17 unk40;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8  unk58;
    /* 0x59 */ u8  pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8  unk60;
    /* 0x61 */ u8  unk61;
    /* 0x62 */ u8  unk62;
    /* 0x63 */ u8  unk63;
    /* 0x64 */ u8  unk64;
    /* 0x65 */ u8  unk65;
    /* 0x66 */ u8  unk66;
    /* 0x67 */ u8  unk67;
    /* 0x68 */ u8  unk68;
    /* 0x69 */ u8  pad69;
    /* 0x6A */ u8  unk6A;
    /* 0x6B */ u8  pad6B;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8  unk70;
    /* 0x71 */ u8  pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x78 */ s32 pad78;
} S151BEEE0;

typedef struct { u8 b[3]; } U8x3_151BEEE0;
extern U8x3_151BEEE0 D_8008FBC0;
extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void func_15133760(void *, void *);

void *func_151BEEE0(f32 arg0, struct17 *arg1, s32 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8) {
    void *ret;
    U8x3_151BEEE0 tbl = D_8008FBC0;

    if (arg4 >= 3) {
        return NULL;
    }
    {
    S151BEEE0 sp44;
    s32 i;

    sp44.unk0 = 1.0f;
    sp44.unk4 = 1.0f;
    sp44.unkC = 0.9230000377f;
    sp44.unk8 = 0.9230000377f;
    sp44.unk10.unk0 = 0.0f;
    sp44.unk10.unk4 = arg0;
    sp44.unk10.unk8 = 0.0f;
    sp44.unk1C = 1.0f;
    sp44.unk20 = 1.0f;
    sp44.unk24 = 1.0f;
    sp44.unk28 = *arg1;
    sp44.unk34.unk0 = 0.0f;
    sp44.unk34.unk4 = 0.0f;
    sp44.unk34.unk8 = 0.0f;
    sp44.unk40.unk0 = 0.0f;
    sp44.unk40.unk4 = 0.0f;
    sp44.unk40.unk8 = 0.0f;
    sp44.unk4C = 0.0f;
    sp44.unk50 = (arg5 ? 0x4000 : 0) | 0x1D00 | 0x80000 | 0x40000;
    sp44.unk54 = 0x12C;
    sp44.unk56 = 0x26;
    sp44.unk58 = tbl.b[arg4];
    sp44.unk5C = arg6;
    sp44.unk60 = 0xFF;
    if (D_800BE9F0 == 7) {
        sp44.unk61 = 0x18;
    } else {
        sp44.unk61 = 0;
    }
    sp44.unk62 = 0;
    sp44.unk63 = 0;
    sp44.unk64 = 0;
    sp44.unk65 = 0;
    sp44.unk66 = 0;
    sp44.unk67 = 0;
    sp44.unk68 = 2;
    sp44.unk6A = 0;
    sp44.unk6C = 0;
    sp44.unk70 = 0;
    sp44.unk72 = 1;
    sp44.unk74 = 0xFF;
    ret = func_15132A4C(&sp44, arg2, arg3, 0, arg7, arg8);
    if (ret != NULL) {
        for (i = 0; i < 2; i++) {
            func_15133760((u8 *)ret + 0x90 + i * 0x40, ret);
        }
    }
    return ret;
    }
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3F_151BF0C8;

typedef struct {
    s32 unk0;
    s32 unk4;
    Vec3F_151BF0C8 unk8;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    u8 unk3C;
    u8 pad3D;
    s16 unk3E;
    s16 unk40;
} Struct151BF0C8_A;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 pad5;
    s16 unk6;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    Vec3F_151BF0C8 unk38;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    s32 unk5C;
    u8 unk60;
    s8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 pad65[3];
    f32 unk68;
} Struct151BF0C8_B;

void func_151BF0C8(Vec3F_151BF0C8 *arg0) {
    Struct151BF0C8_A sp84;
    Struct151BF0C8_B sp18;

    sp84.unk0 = 0x18;
    sp84.unk4 = 0xA;
    sp84.unk8 = *arg0;
    sp84.unk14 = D_800AA8E8;
    sp84.unk18 = D_800AA8EC;
    sp84.unk1C = 0;
    sp84.unk1E = 0xFF;
    sp84.unk20 = -0x32;
    sp84.unk22 = 0x3C;
    sp84.unk24 = 13.0f;
    sp84.unk28 = 13.0f;
    sp84.unk2C = D_800AA8F0;
    sp84.unk30 = D_800AA8F4;
    sp84.unk34 = D_800AA8F8;
    sp84.unk38 = D_800AA8FC;
    sp84.unk3C = 3;
    sp84.unk3E = 0x41;
    sp84.unk40 = 0x1E;
    func_15150F90(&sp84, 1, 0xFF, 1);

    sp18.unk0 = 0x14;
    sp18.unk2 = 8;
    sp18.unk4 = 0x6C;
    sp18.unk6 = 0x5103;
    sp18.unk8 = 0x200005;
    sp18.unkC = 0;
    sp18.unk10 = 0x28;
    sp18.unk12 = 0x28;
    sp18.unk14 = 0;
    sp18.unk18 = 0;
    sp18.unk1F = 0xFF;
    sp18.unk1C = 0xFF;
    sp18.unk1D = 0x91;
    sp18.unk1E = 6;
    sp18.unk20 = 0xFF;
    sp18.unk21 = 0xFF;
    sp18.unk22 = 0;
    sp18.unk23 = 0x14;
    sp18.unk24 = 0xC8;
    sp18.unk25 = 0xFF;
    sp18.unk26 = 0x32;
    sp18.unk28 = 5;
    sp18.unk2A = 0x32;
    sp18.unk2C = D_800AA900;
    sp18.unk30 = 175.0f;
    sp18.unk34 = 160.0f;
    sp18.unk38 = *arg0;
    sp18.unk44 = 0;
    sp18.unk46 = -0x33;
    sp18.unk48 = 0xFF;
    sp18.unk4A = 0x50;
    sp18.unk4C = 4.0f;
    sp18.unk50 = 13.0f;
    sp18.unk54 = D_800AA904;
    sp18.unk58 = D_800AA908;
    sp18.unk5C = 0x840E07;
    sp18.unk60 = 0x10;
    sp18.unk61 = -1;
    sp18.unk62 = 8;
    sp18.unk63 = 6;
    sp18.unk64 = 1;
    sp18.unk68 = D_800AA90C;
    func_15153634(&sp18, 0xFF, 0xFF, 1);
}

extern void *func_15157010(void *, s32, f32, s32, s32, s32, u8, s32);
typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ s8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ s8 unk03;
    /* 0x04 */ s8 unk04;
    /* 0x05 */ u8 pad05;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ struct17 unk44;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} S151BE850;
typedef struct {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
} S151BF81CPart;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} S151BF81CDigit;

/* The 0x18-byte block func_151BF340 copies to object+0x28. */
typedef struct {
    /* 0x00 */ S151BF81CPart *unk0;
    /* 0x04 */ S151BF81CDigit *unk4[2];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 unk14;
} S151BF81CState;

typedef struct {
    /* 0x00 */ u8 pad0[0xD];
    /* 0x0D */ u8 unkD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ S151BF81CState unk28;
} S151BF81CObj;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
} S151BF340Digit;

typedef struct {
    /* 0x00 */ struct17 unk0;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
} S151BF340Payload;

extern void *func_1515548C(S151BF340Digit *, s32, s32, s32, s32, u8, s32);

void func_151BF340(u8 arg0, s32 arg1) {
    S151BF81CState sp160;
    struct260 *ret;
    s32 count;
    s32 digits[2];
    S151BF81CState *s;

    func_151494E0(0, 0x57);
    count = func_150859AC(0, 3);
    if (count != 0) {
        count--;
    }
    if (count >= 100) {
        count = 99;
    }
    digits[0] = count / 10;
    digits[1] = count % 10;
    sp160.unk0 = NULL;
    bzero(sp160.unk4, 8);
    sp160.unkC = 110.0f;
    sp160.unk10 = sinf(1.905000091f);
    sp160.unk14 = 0;
    ret = func_15149130(0x12C, -1, 0x60, -1, 0, 0x49, (struct37 *)0x18, arg0, arg1);
    if (ret != NULL) {
        s = &((S151BF81CObj *)ret)->unk28;
        memcpy(s, &sp160, 0x18);
        {
        S151BE850 spF4;
        S151BF340Payload spE0;
        S151BF340Digit sp84;

        spF4.unk38 = 0;
        spF4.unk00 = 0xC0;
        spF4.unk01 = -1;
        spF4.unk02 = 5;
        spF4.unk03 = 3;
        spF4.unk04 = 0;
        spF4.unk06 = 0x12C;
        spF4.unk08 = 0xA3;
        spF4.unk0C = 8;
        spF4.unk10 = 0x100000;
        spF4.unk14 = 0x200004;
        spF4.unk18 = 0x60601;
        spF4.unk2D = 8;
        spF4.unk1C = 1;
        spF4.unk20 = 0x38;
        spF4.unk2C = 0;
        spF4.unk24 = 0x80;
        spF4.unk28 = 0x20;
        spF4.unk30 = 0xFF;
        spF4.unk31 = 0xFF;
        spF4.unk32 = 0xFF;
        spF4.unk33 = 0xFF;
        spF4.unk34 = 0xFF;
        spF4.unk35 = 0xFF;
        spF4.unk36 = 0xFF;
        spF4.unk37 = 0xFF;
        spF4.unk3C = 0;
        spF4.unk40 = 0;
        spF4.unk41 = 1;
        spF4.unk54 = 1;
        spF4.unk56 = 0xFF;
        spF4.unk44.unk0 = 300.0f;
        if (D_800D2458) {
            spF4.unk44.unk4 = 32.0f;
        } else {
            spF4.unk44.unk4 = 79.0f;
        }
        spF4.unk44.unk8 = 0.0f;
        spE0.unk0 = *(struct17 *)&D_800A5480;
        spE0.unkC = 0.3010000288f;
        spE0.unk10 = 0.3010000288f;
        s->unk0 = func_15157010(&spF4, 0, 1.0f, 0, 0, 0x14, arg0, arg1);
        if (s->unk0 != NULL) {
            memcpy((u8 *)s->unk0 + 0x120, &spE0, 0x14);
        }
        {
            const s32 tbl[10] = { 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD };
            u8 i;

            sp84.unk00 = -500.0f;
            if (D_800D2458) {
                sp84.unk04 = -15.0f;
            } else {
                sp84.unk04 = -62.0f;
            }
            sp84.unk12 = 0x12C;
            sp84.unk14 = 0x20;
            sp84.unk16 = 1;
            sp84.unk18 = 0xFF;
            sp84.unk1A = 0;
            sp84.unk1B = 0xFF;
            sp84.unk1C = 0xFF;
            sp84.unk1D = 0xFF;
            sp84.unk1E = 0xFF;
            sp84.unk1F = 0xFF;
            sp84.unk20 = 0xFF;
            sp84.unk21 = 0xFF;
            sp84.unk22 = 0xFF;
            sp84.unk23 = 0xFF;
            sp84.unk24 = 0;
            sp84.unk28 = 0x200004;
            sp84.unk2C = 0x9F0601;
            sp84.unk40 = 0;
            sp84.unk41 = 0;
            sp84.unk30 = 0x19;
            sp84.unk34 = 0x13;
            sp84.unk38 = 0x80;
            sp84.unk3C = 0x20;
            sp84.unk44 = 0;
            sp84.unk50 = 0.0f;
            sp84.unk54 = 0.0f;
            sp84.unk0C = 16.0f;
            sp84.unk08 = 16.0f;
            sp84.unk48 = 1.f;
            sp84.unk4C = 1.f;
            if (digits[0] == 0) {
                sp84.unk10 = tbl[digits[1]];
                s->unk4[0] = func_1515548C(&sp84, 0, 0, 0, 0, arg0, arg1);
                s->unk4[1] = NULL;
            } else {
                for (i = 0; i < 2; i++) {
                    sp84.unk10 = tbl[digits[i]];
                    s->unk4[i] = func_1515548C(&sp84, 0, 0, 0, 0, arg0, arg1);
                }
            }
        }
        }
    }
}

typedef struct Game1EC390Animation {
    void *primary;
    void *secondary[2];
    f32 timer;
    f32 amount;
    u8 flags;
} Game1EC390Animation;

s32 func_1517EF00(s32);
s32 func_15181CC8(s32);
extern f32 D_800AA918, D_800AA91C, D_800AA920;
extern f32 D_800AA924, D_800AA928, D_800AA92C;

void func_151BF81C(void *arg0) {
    f32 amount;
    Game1EC390Animation *animation;

    if (D_800BEAC1 != 0 || (animation = (Game1EC390Animation *)((u8 *)arg0 + 0x28), D_800BEAC0 != 0)) {
        *(s16 *)((u8 *)arg0 + 0xE) = -1;
        *((u8 *)arg0 + 0xD) |= 1;
        return;
    }
    if (!(animation->flags & 1)) {
        if (D_800C35EA == 1) return;
        if (func_15181CC8(0) == 0) return;
        if (func_1517EF00(0) != 0 || D_800BE9B4 != 0) return;
        animation->flags |= 1;
    }
    if (animation->timer < D_800AA918) {
        amount = sinf(animation->timer * D_800AA91C * D_800AA920);
    } else if (D_800AA924 < animation->timer) {
        amount = sinf((110.0f - animation->timer) * D_800AA928 * D_800AA92C);
    } else {
        amount = animation->amount;
    }
    if (animation->primary != NULL) {
        *(f32 *)((u8 *)animation->primary + 0x54) = amount * -85.0f + 150.0f;
        if (D_800D2458 != 0) *(f32 *)((u8 *)animation->primary + 0x58) = 32.0f;
        else *(f32 *)((u8 *)animation->primary + 0x58) = 79.0f;
    }
    if (animation->secondary[0] != NULL) {
        *(f32 *)((u8 *)animation->secondary[0] + 0x10) = ((amount * -85.0f + 150.0f) / 1.0f + 48.0f) - 12.0f;
        if (D_800D2458 != 0) *(f32 *)((u8 *)animation->secondary[0] + 0x14) = -15.0f;
        else *(f32 *)((u8 *)animation->secondary[0] + 0x14) = -62.0f;
    }
    if (animation->secondary[1] != NULL) {
        *(f32 *)((u8 *)animation->secondary[1] + 0x10) = ((amount * -85.0f + 150.0f) / 1.0f + 48.0f) + 12.0f;
        if (D_800D2458 != 0) *(f32 *)((u8 *)animation->secondary[1] + 0x14) = -15.0f;
        else *(f32 *)((u8 *)animation->secondary[1] + 0x14) = -62.0f;
    }
    animation->timer -= D_800BE9A4;
    if (animation->timer < 0.0f) {
        *(s16 *)((u8 *)arg0 + 0xE) = -1;
        *((u8 *)arg0 + 0xD) |= 1;
    }
}


void func_151BFB2C(struct260 *arg0) {
    s32 i;
    s32 v;
    s32 *p = (s32 *)((u8 *)arg0 + 0x28);

    if (p[0] != 0) {
        func_1516972C((struct102 *)p[0]);
    }
    for (i = 0; i < 2; i = (u8)(i + 1)) {
        v = p[i + 1];
        if (v != 0) {
            func_1516972C((struct102 *)v);
        }
    }
}

void func_151BFBA4(struct260 *arg0) {
    func_151BFB2C(arg0);
    func_1514933C(arg0);
}
void func_151BFBD0(struct260 *arg0);
void func_151BFBFC(struct102 *arg0, s32 arg1, u8 arg2);
