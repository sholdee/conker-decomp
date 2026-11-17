#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_proto
#define func_10010F30 func_10010F30_void
#include "functions.h"
#undef func_150ADA20
#undef func_10010F30
#include "variables.h"

typedef void (*GameFunc151C455CDispatch)(s32);
typedef void (*GameFunc151C455CCallback)(struct17 *, s32);

extern GameFunc151C455CDispatch D_8008FBD0[];
extern s32 func_150ADA20(void);
extern s32 func_10010F30(s32, u16, u8, s16, u8);
extern s32 func_1000B060(f32, f32, u32);
extern void func_1000E7A0(u32, s32);
extern s32 func_1510F8CC(s32);
extern f32 D_800AAA30;
extern f32 D_800AAA34;
extern f32 D_800AAA7C;
extern f32 D_800AAA80;
extern f32 D_800AAA84;

typedef struct {
    u8 pad0[0x2C];
    s16 unk2C;
} GameStruct151C2050State;
void func_151C2050(struct127 *arg0, f32 *arg1, f32 *arg2, s32 arg3, f32 arg4);


typedef struct {
    u8 unk0;
    u8 pad1[0x5F];
    f32 unk60;
    f32 unk64;
    f32 unk68;
    u8 pad6C[0x20];
    f32 unk8C;
    s32 unk90;
    u8 pad94[4];
    f32 unk98;
    u8 pad9C[0xC];
    s8 unkA8;
    u8 padA9[3];
    s32 unkAC;
} Mover151C43E0;

typedef struct {
    u8 pad0[0x34];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
} GameStruct151C4510a;

typedef struct {
    u8 pad0[0x4];
    f32 unk4;
    f32 unk8;
    f32 unkC;
} GameStruct151C4510b;

typedef struct {
    u8 unk0;
    u8 pad1[0x83];
    struct131 *unk84;
    u8 pad88[0x4];
    f32 unk8C;
    u8 pad90[0xC];
    s8 unk9C;
} GameStruct151C455C;

typedef struct {
    u16 unk0;
    u16 unk2;
} GameStruct151C2734Sfx;

typedef struct {
    u8 pad0[0x60];
    f32 unk60;
    f32 unk64;
    f32 unk68;
    u8 pad6C[0x20];
    f32 unk8C;
    u8 pad90[0x8];
    f32 unk98;
    u8 pad9C[0x2];
    u16 unk9E;
    u8 unkA0;
    u8 unkA1;
} GameStruct151C2734Sub;

typedef struct {
    u8 pad0[0x34];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u8 pad40[0xD0];
    GameStruct151C2734Sub unk110;
} GameStruct151C2734;

extern GameStruct151C2734Sfx D_8008FBE8[];
extern s32 func_1000A420(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32 *, s32 *, s32);
void func_151C436C(s32 a0, f32 *a1, s32 a2);
u8 func_151C43E0(struct108 *arg0, Mover151C43E0 *arg1, f32 arg2);
void func_151C4510(GameStruct151C4510a *a0, GameStruct151C4510b *a1, f32 a2);
s32 func_151C455C(s32 arg0, GameStruct151C455C *arg1, f32 arg2);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_151C4B0C;

struct Game1F1F60Hit;
void *memcpy(void *, const void *, u32);
s32 func_15145128(f32 *, f32 *, f32 *, f32 *);
s32 func_151D5B6C(s32 arg0, s32 arg1, struct127 *arg2, s8 arg3, s32 arg4);
s32 func_151C4B0C(struct Game1F1F60Hit *arg0, struct Game1F1F60Hit *arg1, Vec3f_151C4B0C *arg2, Vec3f_151C4B0C *arg3, f32 *arg4, f32 arg5,
                  u8 arg6, Vec3f_151C4B0C *arg7, Vec3f_151C4B0C *arg8, u8 *arg9, u8 arg10, f32 arg11, void *arg12, s8 arg13,
                  u8 arg14, u8 arg15, s32 arg16, u8 arg17);

typedef struct Game1EF500TrailExt {
    u8 flags;
    u8 pad1[3];
    f32 velocity[3];
    f32 accel[3];
    f32 drift[3];
    u8 path[0x38];
    f32 direction[3];
    u8 pad6C[0x20];
    u8 extra[4];
    u8 *owner;
    u8 ownerId;
    u8 pad95[3];
    f32 scale;
    s8 field9C;
    u8 pad9D;
    s16 field9E;
    u8 chance;
    u8 fieldA1;
    u8 padA2[2];
    s32 fieldA4;
    s8 fieldA8;
    u8 padA9[3];
    s32 handle;
} Game1EF500TrailExt;

typedef struct Game1EF500TrailSpawn {
    u8 field00;
    u8 field01;
    s16 field02;
    s16 field04;
    u8 pad06[2];
    s32 field08;
    s32 field0C;
    u8 field10;
    u8 field11;
    u8 field12;
    u8 field13;
    f32 field14;
    f32 field18;
    Vec3f_151C4B0C position;
    f32 field28[6];
    s32 flags;
    u8 field44;
    u8 field45;
    u8 field46;
    u8 field47;
    u8 pad48[4];
    u8 field4C;
    u8 pad4D[0xB];
} Game1EF500TrailSpawn;
void *func_151C229C(Vec3f_151C4B0C *arg0, Vec3f_151C4B0C *arg1, s32 arg2, Vec3f_151C4B0C *arg3, u8 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, u8 arg11, u8 *arg12, u8 arg13, u8 arg14, u8 arg15, u8 arg16, u8 arg17, u8 arg18, s8 arg19, s32 arg20, f32 arg21, u8 arg22, s8 arg23, s32 arg24, u8 arg25, s32 arg26);
s8 func_151C2734(GameStruct151C2734 *arg0);

typedef struct {
    s32 unk0;
    u8 unk4;
} GameStruct151C2E4C;
s32 func_151C2E4C(GameStruct151C2E4C *a0, GameStruct151C2E4C *a1);

typedef struct {
    s32 unk0;
    u8 unk4;
    u8 pad[0x122];
    u8 unk127;
} GameStruct151C2E94;
s32 func_151C2E94(GameStruct151C2E94 *a0, GameStruct151C2E94 *a1);

s32 func_151D4DAC(s32, s32, s32, s32, s32, s32, void *, s32, s32);
s32 func_151C2EF0(s32 a0, s32 a1, struct108 *a2, s32 a3, s32 arg4, s32 arg5);

extern f32 D_800AAA3C;
extern f32 D_800AAA40;
extern u8 D_800DCA20;
extern f32 D_800DCA24;
void func_15102920(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32);
void func_15152B38(void *, u8, s32);
s32 func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);

typedef struct {
    /* 0x0 */ f32 a;
    /* 0x4 */ f32 b;
    /* 0x8 */ f32 c;
} GameVec151C2F48;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ GameVec151C2F48 unk30;
    /* 0x3C */ u8 pad3C[0x6C - 0x3C];
    /* 0x6C */ u8 unk6C[0x88 - 0x6C];
    /* 0x88 */ s32 unk88;
} GameSub151C2F48;

typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0x110 - 0xD];
    /* 0x110 */ GameSub151C2F48 unk110;
} GameObj151C2F48;

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ GameVec151C2F48 unk08;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
    /* 0x4A */ u8 unk4A;
    /* 0x4B */ u8 unk4B;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ u8 unk4E;
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ s8 unk70;
    /* 0x71 */ s8 unk71;
    /* 0x72 */ s8 unk72;
    /* 0x73 */ s8 unk73;
} GameSpawn151C2F48;
void func_151C2F48(GameObj151C2F48 *arg0);

typedef struct {
    s32 field0;
    s32 field4;
    struct17 position8;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    s16 field2C;
    s16 field2E;
    s16 field30;
    s16 field32;
    s32 field34;
    s32 field38;
    s16 field3C;
    s16 field3E;
    s16 field40;
    u8 fields42[23];
    s32 field5C;
    s32 field60;
    s16 field64;
    s16 field66;
    s16 field68;
    u8 field6A;
    f32 field6C;
    s8 fields70[4];
} GameEmitterParticle151C329C;

typedef struct {
    s32 field0;
    s32 field4;
    s16 field8;
    s16 fieldA;
    s32 fieldC;
    s32 field10;
    u8 colors14[9];
    u8 field1D;
    s16 field1E;
    s16 field20;
    s16 field22;
    f32 field24;
    f32 field28;
    f32 field2C;
    struct17 position30;
    f32 field3C;
    f32 field40;
    f32 field44;
    f32 field48;
    f32 field4C;
    f32 field50;
    f32 field54;
    s32 field58;
    s32 field5C;
    s8 fields60[6];
    u8 fields66[0xA];
} GameEmitterSpawn151C329C;

typedef struct {
    u8 field0;
    s8 field1;
    s16 field2;
    u8 field4;
} GameEmitterLight151C329C;

extern f32 D_800AAA44;
extern f32 D_800AAA48;
extern f32 D_800AAA4C;
extern f32 D_800AAA50;
extern void *func_15130280(void *, u8, s32, s32, u8, s32);
void func_151C329C(struct17 *arg0, u8 arg1, s32 arg2);

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ struct17 pos;
    /* 0x3C */ struct17 unk3C;
    /* 0x48 */ struct17 vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ f32 unk6C;
} GameParticle151C36D8; /* 0x70 */

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
void func_151C36D8(struct17 *arg0, u8 arg1, s32 arg2);

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ struct17 pos;
    /* 0x3C */ u8 pad3C[0x60 - 0x3C];
    /* 0x60 */ struct17 prev;
    /* 0x6C */ u8 unk6C[0x88 - 0x6C];
} GameSub151C3B0C;

typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0x110 - 0xD];
    /* 0x110 */ GameSub151C3B0C unk110;
    /* 0x198 */ s32 unk198;
} GameObj151C3B0C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
} GameDebris151C3B0C; /* 0x28 */

typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ u8 pad06[2];
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ struct17 pos;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 pad4D[7];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} GameSpark151C3B0C; /* 0x58 */

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 unk22;
    /* 0x23 */ u8 unk23;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 unk25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ struct17 unk38;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s8 unk60;
    /* 0x61 */ s8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 pad65[3];
    /* 0x68 */ f32 unk68;
} GameSmoke151C3B0C; /* 0x6C */

void func_15153634(void *, u8, u8, s32);
void func_151C3B0C(GameObj151C3B0C *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u8 arg5, u8 arg6, u8 arg7);
void func_151C436C(s32 a0, f32 *a1, s32 a2);

typedef struct {
    s32 unk00;
    f32 unk04;
    struct17 unk08;
    struct17 unk14;
    struct17 unk20;
    struct17 unk2C;
    struct17 unk38;
    u8 unk44[0x12];
    s16 unk56;
    u8 unk58;
    u8 unk59;
    u8 unk5A;
    u8 pad5B;
    s32 unk5C;
    s32 unk60;
    u8 pad64[0xC];
} Ray151C43E0;


typedef struct {
    u8 pad0[0x34];
    struct17 pos;
} Pos151C43E0;

void func_15081690(s32, f32, f32, f32, f32, f32, f32, Ray151C43E0 *, f32, s32, s32, s32, s32, s32, s32);
u8 func_151C43E0(struct108 *arg0, Mover151C43E0 *arg1, f32 arg2);
void func_151C4510(GameStruct151C4510a *a0, GameStruct151C4510b *a1, f32 a2);
s32 func_151C455C(s32 arg0, GameStruct151C455C *arg1, f32 arg2);

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} GameStruct151C4644Vec;

typedef struct {
    GameStruct151C4644Vec *unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
} GameStruct151C4644Sub;

typedef struct {
    u8 pad0[1];
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    GameStruct151C4644Sub unk28;
} GameStruct151C4644;

typedef struct {
    f32 unk0;
    f32 unk4;
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
} GameStruct151C4644Payload;
void func_151C4644(GameStruct151C4644 *arg0);

typedef struct Game1F1CD0Emission {
    f32 x, y;
    f32 amplitudeX, amplitudeY;
    f32 phaseX, phaseY;
    f32 speedX, speedY;
    f32 time, rate;
    f32 remaining;
    Vec3f_151C4B0C position;
    f32 scaleX, scaleY, parameter;
} Game1F1CD0Emission;
typedef struct Game1F1CD0Actor {
    u8 pad0;
    u8 kind;
    u8 pad2[0xA];
    u8 owner;
    u8 padD[0x1B];
    Game1F1CD0Emission emission;
} Game1F1CD0Actor;

void func_151436B4(f32, f32, f32, f32 *);
void func_151C4820(Game1F1CD0Actor *arg0);

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
} GameStruct151C4AB0a;

typedef struct {
    u8 pad0[0x90];
    s32 unk90;
    u8 unk94;
} GameStruct151C4AB0b;

void func_151C4AB0(void *a0, GameStruct151C4AB0a *a1, u8 a2) {
    GameStruct151C4AB0b *v0 = (GameStruct151C4AB0b *)((u8 *)a0 + 0x110);
    if (a2 == 0x2D) {
        if (v0->unk90 == a1->unk0) {
            v0->unk90 = a1->unk4;
            v0->unk94 = a1->unk9;
        } else if (v0->unk90 == a1->unk4) {
            v0->unk90 = a1->unk0;
            v0->unk94 = a1->unk8;
        }
    }
}


typedef struct Game1F1F60Hit {
    void *actor;
    f32 dist;
    Vec3f_151C4B0C hit;
    Vec3f_151C4B0C normal;
    Vec3f_151C4B0C point;
    Vec3f_151C4B0C start;
    Vec3f_151C4B0C end;
    s16 surface[9];
    s16 field_56;
    s8 field_58;
    u8 type;
    s8 field_5A;
    u8 pad5B;
    u8 *field_5C;
    s32 field_60;
} Game1F1F60Hit;

s32 func_15145128(f32 *, f32 *, f32 *, f32 *);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, s16 *, f32 *, f32 *, f32 *, f32 *, s32 *, void *, f32);

s32 func_151C4B0C(Game1F1F60Hit *arg0, Game1F1F60Hit *arg1, Vec3f_151C4B0C *arg2, Vec3f_151C4B0C *arg3, f32 *arg4, f32 arg5,
                  u8 arg6, Vec3f_151C4B0C *arg7, Vec3f_151C4B0C *arg8, u8 *arg9, u8 arg10, f32 arg11, void *arg12, s8 arg13,
                  u8 arg14, u8 arg15, s32 arg16, u8 arg17) {
    Game1F1F60Hit hit;

    if (arg0 != NULL) {
        *arg1 = *arg0;
        if (arg2->x != arg1->start.x || arg2->y != arg1->start.y || arg2->z != arg1->start.z) {
            arg1->end.x = arg1->hit.x - arg2->x;
            arg1->end.y = arg1->hit.y - arg2->y;
            arg1->end.z = arg1->hit.z - arg2->z;
            arg1->start = *arg2;
            if (func_15145128(&arg1->end.x, &arg1->end.x, &arg1->dist, NULL) == 0) {
                return 0;
            }
        }
        if (arg17 != 0 && (arg1->type == 2 || arg1->type == 3)) {
            func_15081690((s32)arg12, arg1->start.x, arg1->start.y, arg1->start.z, arg1->end.x, arg1->end.y,
                          arg1->end.z, (Ray151C43E0 *)&hit, 0.0f, 0, 1, 1, -1, 0, arg16);
            if (hit.type != 0) {
                *arg1 = hit;
            }
        }
        *arg4 = arg1->dist * arg5;
    } else if (arg6 != 0) {
        u8 found;
        s32 index;

        index = -1;
        if (arg7 != NULL && arg8 != NULL) {
            arg1->start = *arg7;
            arg1->end = *arg8;
        } else {
            arg1->start = *arg2;
            arg1->end = *arg3;
        }
        func_15081690((s32)arg12, arg1->start.x, arg1->start.y, arg1->start.z, arg1->end.x, arg1->end.y,
                      arg1->end.z, (Ray151C43E0 *)arg1, 0.0f, 0, arg14 ? 0 : 1, (arg15 != 0 && arg14 != 0) ? 0 : 1, arg13,
                      0, arg16);
        if (arg1->field_5C == NULL) {
            index = -1;
        } else {
            index = (arg1->field_5C - (u8 *)D_800DBEF4) / 160;
        }
        if (arg1->type == 0) {
            found = 0;
        } else if (arg1->type == 1) {
            found = 1;
        } else if (arg7 != NULL && arg8 != NULL) {
            f32 dist;
            Game1F1F60Hit saved;

            saved = *arg1;
            arg1->start = *arg2;
            arg1->end.x = arg1->hit.x - arg2->x;
            arg1->end.y = arg1->hit.y - arg2->y;
            arg1->end.z = arg1->hit.z - arg2->z;
            if (func_15145128(&arg1->end.x, &arg1->end.x, &arg1->dist, NULL) == 0) {
                return 0;
            }
            dist = arg1->dist;
            found = func_150AC9C0(arg1->hit.x, arg1->hit.y, arg1->hit.z, arg1->end.x, arg1->end.y, arg1->end.z,
                                  NULL, arg1->surface, &arg1->hit.x, &arg1->hit.y, &arg1->hit.z, &arg1->dist,
                                  &index, &arg1->field_60, 0.0f);
            if (found) {
                arg1->dist += dist;
                arg1->start = *arg2;
            }
        } else {
            found = func_150AC9C0(arg2->x, arg2->y, arg2->z, arg1->end.x, arg1->end.y, arg1->end.z, NULL,
                                  arg1->surface, &arg1->hit.x, &arg1->hit.y, &arg1->hit.z, &arg1->dist, &index,
                                  &arg1->field_60, 0.0f);
        }
        if (found) {
            *arg4 = arg1->dist * arg5;
            if (arg2->x != arg1->start.x || arg2->y != arg1->start.y || arg2->z != arg1->start.z) {
                arg1->end.x = arg1->hit.x - arg2->x;
                arg1->end.y = arg1->hit.y - arg2->y;
                arg1->end.z = arg1->hit.z - arg2->z;
                arg1->start = *arg2;
                if (func_15145128(&arg1->end.x, &arg1->end.x, &arg1->dist, NULL) == 0) {
                    return 0;
                }
            }
            func_151C2050((struct127 *)arg12, &arg1->hit.x, &arg1->end.x, arg1->field_60, arg1->dist);
            if (index == -1) {
                arg1->field_5C = NULL;
            } else {
                arg1->field_5C = (u8 *)(D_800DBEF4 + index);
            }
        } else {
            *arg9 &= ~arg10;
            *arg4 = arg11;
            if (arg3 == NULL && arg8 == NULL) {
                return 0;
            }
        }
    } else {
        *arg9 &= ~arg10;
        *arg4 = arg11;
        arg1->start = *arg2;
        if (arg3 != NULL) {
            arg1->end = *arg3;
        } else if (arg8 != NULL) {
            arg1->end = *arg8;
        } else {
            return 0;
        }
    }
    return 1;
}
