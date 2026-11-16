#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr0
#define func_15169260 func_15169260_s32
#define func_1513D524 func_1513D524_void_proto
#include "functions.h"
#undef func_15048A40
#undef func_1513D524
extern f32 func_15048A40(u8);
#undef func_15169260
#include "variables.h"

void func_1518E308(void *);
void func_15169260(void *, s32, s32, u8);
extern f32 D_800A8004;
extern f32 D_800A7B68;
extern f32 D_800A7B6C;
extern f32 D_800A7B70;
extern f32 D_800A8018;
extern void func_151D5D60();
extern struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, u8, u8, s32, u8, s32);
struct Struct1519021CArg1;
extern void func_151D2AB0(s32);
extern s32 func_151D2F00(s32, s32, s32, s32);
extern struct225 *func_1519021C(s32, struct Struct1519021CArg1 *, u8, s16, u8, s32);

typedef struct Game1BBFC0Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Game1BBFC0Vec3f;

typedef struct Game1BBFC0Vertex {
    s16 x, y, z, flag;
    u8 other[8];
} Game1BBFC0Vertex;

typedef struct Game1BBFC0Beam {
    u8 unknown0[0x2C];
    f32 width;
    f32 progress;
    Game1BBFC0Vec3f start;
    Game1BBFC0Vec3f end;
    u8 unknown4C[0x74];
    u8 vertexTemplate[0x40];
    u8 *buffers[1];
} Game1BBFC0Beam;

typedef struct Game1BBFC0Camera {
    u8 unknown0[0x2F8];
    Game1BBFC0Vec3f position;
    u8 unknown304[0x69C];
} Game1BBFC0Camera;

void *memcpy(void *, const void *, u32);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

struct SoundLinkedPayload {
    u8 pad_0x0[0x46];
    s16 field_0x46;
    u8 pad_0x48[0x3C];
    u8 field_0x84;
};

struct SoundLinkedObject {
    u8 pad_0x0[0x31C];
    struct SoundLinkedPayload *field_0x31C;
};

struct TimedSoundTrigger {
    s32 field_0x0;
    s32 field_0x4;
    struct127 *field_0x8;
};

struct TimedSoundObject {
    u8 pad_0x0[0x10];
    struct SoundLinkedObject *field_0x10;
    u8 pad_0x14[0x7C];
    struct TimedSoundTrigger field_0x90;
};

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
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
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Particle1518CD20; /* 0x70 */

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern void func_1514373C(f32 arg0, f32 arg1, f32 *arg2, f32 *arg3);
extern void func_15145DB4(s32, struct17 *, struct17 *, s32);

void func_1518CD20(s32 arg0, u8 arg1, s32 arg2) {
    s16 count;
    Particle1518CD20 spawn;
    s16 remaining;
    struct17 *b;
    s16 i;
    f32 scale;
    struct17 *a;

    count = remaining = func_150ADA20() % 11U + 25;
    if (count > 0) {
        a = (struct17 *)func_10003C6C((count + 1) * sizeof(struct17), 3, 1, 0, 1);
        b = (struct17 *)func_10003C6C((count + 1) * sizeof(struct17), 3, 1, 0, 1);
        if (a == NULL || b == NULL) {
            return;
        }
        {
            for (i = 0; i < count; i++) {
                func_1514373C(func_150ADA68() * 3.141592741f + 3.141592741f, 114.0f, &a[i].unk0, &a[i].unk8);
                a[i].unk0 += 111.0f;
                a[i].unk4 = -90.0f;
            }
            i = 0;
            a[count].unk0 = 111.0f;
            a[count].unk4 = -90.0f;
            a[count].unk8 = 0.0f;
            func_15145DB4(arg0, a, b, count + 1);
            spawn.unk1D = 0x6C;
            spawn.unk8 = 0x5103;
            spawn.unk0 = 0x200005;
            spawn.unk14 = 0xE7;
            spawn.unk15 = 0xE7;
            spawn.unk16 = 0xB6;
            spawn.unk18 = 0x9E;
            spawn.unk19 = 0x9E;
            spawn.unk1A = 0x62;
            spawn.unk4 = 0;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk17 = 0xFF;
            spawn.unk1C = 0xFF;
            spawn.unk3C.unk0 = 0.0f;
            spawn.unk3C.unk4 = 0.0f;
            spawn.unk3C.unk8 = 0.0f;
            spawn.unk1E = 0x19;
            spawn.unk20 = 0xA;
            spawn.unk58 = 0x80CE07;
            spawn.unk60 = 8;
            spawn.unk61 = 6;
            spawn.unk62 = 0x10;
            spawn.unk63 = -1;
            spawn.unk64 = -1;
            spawn.unk65 = 0;
            spawn.unk24 = 1.040568948f;
            scale = 0.8533809781f;
            do {
                f32 speed;
                s32 flags;
                void *ret;

                spawn.unk58 &= ~0xC0;
                flags = (func_150ADA20() & 1) ? 0x80 : 0;
                spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | flags;
                speed = (func_150ADA68() * 202.0f + 250.0f) * 0.001000000047f;
                spawn.unk22 = spawn.unkA = func_150ADA20() % 26U + 25;
                spawn.unk28 = spawn.unk2C = func_150ADA68() * 105.0f + 150.0f;
                spawn.pos = b[i];
                spawn.vel.unk0 = (b[i].unk0 - b[count].unk0) * speed;
                spawn.vel.unk4 = (b[i].unk4 - b[count].unk4) * speed;
                spawn.vel.unk8 = (b[i].unk8 - b[count].unk8) * speed;
                spawn.unk54 = (func_150ADA68() * 880.0f + 13.0f) * 0.001000000047f;
                spawn.unk1B = func_150ADA20() % 176U + 80;
                ret = func_15130280(&spawn, 1, 0, 4, arg1, arg2);
                if (ret != NULL) {
                    memcpy((u8 *)ret + 0xA8, &scale, 4);
                }
                remaining--;
                i++;
            } while (remaining > 0);
        }
    }
}

void *func_1518E3C4(void *arg0, s32 arg1);

typedef struct Node1518D1C0 {
    u8 pad0[0x8];
    struct Node1518D1C0 *unk8;
    u8 padC[0xC];
    void *unk18;
    u8 unk1C;
} Node1518D1C0;

typedef struct Owner1518D1C0 {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x18E];
    u8 unk1CA;
} Owner1518D1C0;

typedef struct Spawn1518D1C0 {
    void *owner;
    u8 unk4;
    s16 unk6;
    u8 unk8;
    s8 unk9;
    u8 unkA;
} Spawn1518D1C0;

extern u8 D_800DCE50[];
void *func_1518D1C0(Owner1518D1C0 *arg0, s16 arg1, s16 arg2, u8 arg3, u8 arg4, u8 arg5, void *arg6);

typedef struct {
    f32 x, y, z;
} Vec3_1518D6E0;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 unk3A;
    /* 0x3B */ u8 unk3B;
    /* 0x3C */ u8 unk3C;
    /* 0x3D */ u8 unk3D;
    /* 0x3E */ u8 unk3E;
    /* 0x3F */ u8 unk3F;
} Tmpl1518D6E0;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ Vec3_1518D6E0 vel;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ Tmpl1518D6E0 *tmpl;
} PData1518D6E0; /* 0x58 */

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 unk1C;
    /* 0x01E */ u8 pad1E[0x16];
    /* 0x034 */ Vec3_1518D6E0 pos;
    /* 0x040 */ Vec3_1518D6E0 prev;
    /* 0x04C */ u8 pad4C[0xC];
    /* 0x058 */ s32 unk58;
    /* 0x05C */ u8 pad5C[0xB4];
    /* 0x110 */ PData1518D6E0 sub;
} Ptcl1518D6E0;

typedef struct Slot1518D6E0 {
    /* 0x00 */ Ptcl1518D6E0 *p;
    /* 0x04 */ s32 timer;
    /* 0x08 */ Vec3_1518D6E0 off;
    /* 0x14 */ struct Slot1518D6E0 *next;
} Slot1518D6E0; /* 0x18 */

typedef struct {
    /* 0x0 */ Vec3_1518D6E0 pos;
    /* 0xC */ s16 bone;
} Point1518D6E0; /* 0x10 */

typedef struct {
    /* 0x00 */ Tmpl1518D6E0 *tmpl;
    /* 0x04 */ s32 numSlots;
    /* 0x08 */ Point1518D6E0 *pts;
    /* 0x0C */ f32 rate;
    /* 0x10 */ s32 maxCount;
    /* 0x14 */ s16 lifeMin;
    /* 0x16 */ s16 lifeRange;
    /* 0x18 */ u8 spread;
    /* 0x19 */ u8 mode;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ f32 speed;
    /* 0x20 */ f32 speedY;
    /* 0x24 */ f32 yOff;
    /* 0x28 */ f32 x;
    /* 0x2C */ f32 y;
    /* 0x30 */ f32 z;
    /* 0x34 */ u8 bone;
} Info1518D6E0;

typedef struct {
    /* 0x0 */ s16 count;
    /* 0x4 */ u8 *ids;
} Ent1518D6E0;

typedef struct {
    /* 0x0 */ Info1518D6E0 **list;
    /* 0x4 */ s16 n;
    /* 0x8 */ Ent1518D6E0 *ents;
} Table1518D6E0;

typedef struct {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ u8 pad4[0x10];
    /* 0x014 */ f32 x;
    /* 0x018 */ f32 y;
    /* 0x01C */ f32 z;
    /* 0x020 */ u8 pad20[0x1B];
    /* 0x03B */ u8 unk3B;
    /* 0x03C */ u8 pad3C[0x38];
    /* 0x074 */ u8 unk74;
    /* 0x075 */ u8 pad75[0xF];
    /* 0x084 */ u16 unk84;
    /* 0x086 */ u8 pad86[0x142];
    /* 0x1C8 */ u8 unk1C8;
    /* 0x1C9 */ u8 pad1C9[0xB];
    /* 0x1D4 */ s32 mtx;
} Obj1518D6E0;

typedef struct {
    /* 0x18 */ u8 pad0[0x18];
    /* 0x18 */ f32 *unk18;
} Src1518D6E0;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ Obj1518D6E0 *obj;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 pad1D;
    /* 0x1E */ s16 timer;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ s8 cb;
    /* 0x22 */ u8 table;
    /* 0x23 */ u8 pad23;
    /* 0x24 */ f32 accum;
    /* 0x28 */ s32 count;
    /* 0x2C */ u8 state;
    /* 0x2D */ u8 mode;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ Src1518D6E0 *src;
    /* 0x34 */ f32 scale;
    /* 0x38 */ Vec3_1518D6E0 pos;
    /* 0x44 */ u8 pad44[4];
    /* 0x48 */ Slot1518D6E0 slots[100];
} Emit1518D6E0;

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ u8 pad1C[0x18];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 pad46[0x12];
} Spawn1518D6E0; /* 0x58 */

extern Table1518D6E0 *D_8008D5E0[];
extern void (*D_8008D620[])(u8 *, u8 *, u8 *, u8 *, u8 *, u8 *, s16 *);
extern s32 (*D_8008D628[])(void *);
extern void *func_1513D524(void *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8);
extern void func_1513F680();
extern void func_15143134(void *, void *, void *);
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
void func_1518E308(void *);
void func_1518D6E0(Emit1518D6E0 *arg0);

struct Struct1518E298Inner {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

struct Struct1518E298 {
    u8 pad0[2];
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u8 pad8[0x14];
    struct Struct1518E298Inner *unk1C;
};
s32 func_1518E298(struct Struct1518E298 *arg0, s32 arg1, s32 arg2, s32 arg3);

struct Struct1518E308Entry {
    struct102 *unk0;
    u8 pad4[0x14];
};
void func_1518E308(void *arg0);
void func_1518E37C(void *arg0, u8 arg1);

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

struct Struct1518E3C4 {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0xC];
    f32 unk24;
    s32 unk28;
    u8 unk2C;
    u8 unk2D;
    u8 pad2E[0x2];
    s32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
};
void *func_1518E3C4(void *arg0, s32 arg1);

extern void func_1518E308(void *);
extern void func_1514EDF0(void *, s32);
void func_1518E4A0(void *arg0);
void func_1518E4CC(void *arg0);
void func_1518E4F8(void *arg0);
void func_1518E524(u8 *arg0, u8 *arg1, u8 *arg2, u8 *arg3, u8 *arg4, u8 *arg5, s16 *arg6);

extern u8 func_150ADA20(void);
extern void func_151429E0(u8, s32, s32, s32);
void func_1518E5D8(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5, s16 *arg6);

extern void *func_1518D1C0(Owner1518D1C0 *, s16, s16, u8, u8, u8, void *);
extern u8 D_800A7460[];
s32 func_1518E66C(void *arg0);

extern u8 D_800A749C[];
s32 func_1518E6D4(void *arg0);

struct Struct1518E73C {
    u8 pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    u8 pad44[0x4];
    f32 unk48;
    u8 pad4C[0x74];
    u8 unkC0[0x40];
    u8 *unk100[0x18];
    f32 unk160;
};

typedef struct Vtx1518E73C {
    s16 x, y, z, flag;
    u8 other[8];
} Vtx1518E73C;

extern f32 func_150489B0(u8);
void *func_1518E73C(struct Struct1518E73C *arg0, s16 arg1);
void *func_1518EB10(Game1BBFC0Beam *arg0, s16 arg1);

struct Struct1518EF54Sub {
    u8 pad0[0x4C];
    f32 unk4C;
};

struct Struct1518EF54 {
    u8 pad0[0x34];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 pad4C[0xF4];
    f32 unk140;
    f32 unk144;
    f32 unk148;
    f32 unk14C;
    f32 unk150;
    u8 pad154[0x8];
    f32 unk15C;
};
s32 func_1518EF54(struct Struct1518EF54 *arg0);

extern f32 D_800BE9A4;
s32 func_1518F058(f32 *arg0);


struct Struct1518F108 {
    char pad1C[0x1C];
    s16 unk1C;
    char pad1E[0xE];
    f32 unk2C;
    f32 unk30;
    char pad34[0x28];
    u8 unk5C;
    char pad5D[0xF7];
    f32 unk154;
    s16 unk158;
    s16 unk15A;
};
s32 func_1518F108(struct Struct1518F108 *arg0);
s32 func_1518F15C(void *arg0);

struct Struct1518F1A0Obj {
    char pad0[0x24];
    s32 unk24;
    char pad28[0x4];
    f32 unk2C;
    f32 unk30;
};

struct Struct1518F1A0Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
};
s32 func_1518F1A0(struct Struct1518F1A0Obj *arg0);

extern f32 func_150ADA68(void);
extern f32 D_800A7B74;
extern f32 D_800A7B78;

struct Struct1518F384 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    char pad14[0x4];
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
};
s32 func_1518F384(void *arg0);

struct Struct1518F45CData {
    s32 unk0;
};

extern struct Struct1518F45CData D_800A74D4;
void func_1518F45C(s32 arg0, u8 arg1);

struct Struct1518F49C {
    s32 unk0;
    u8 unk4;
};
void func_1518F49C(void *arg0, struct Struct1518F49C *arg1, u8 arg2);

extern s32 func_1518FDC4(void *, u8 *, u8);
extern s32 func_1518F5D0(void *, void *, s16, s8, s8, s8, u8, s32, u8, s32);
extern s32 D_8008D630[];
s32 func_1518F51C(void *arg0, u8 arg1, s32 arg2, s32 arg3, s8 arg4, s8 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9);

struct Struct1518F5D0Data {
    u32 unk0[0x11];
    s32 unk44;
    u32 unk48[2];
};

struct Struct1518F5D0Payload {
    f32 unk0;
    struct Struct1518F5D0Data unk4;
    struct225 *unk54;
    s8 unk58;
    s8 unk59;
    u8 unk5A;
    u8 unk5B;
    u32 pad5C;
};

struct Struct1518F5D0Init {
    void *unk0;
    u8 unk4;
    u8 pad5;
    s16 unk6;
    s8 unk8;
    s8 unk9;
    s8 unkA;
    s8 unkB;
    s8 unkC;
    s8 unkD;
    u8 padE[2];
};
s32 func_1518F5D0(void *arg0, void *arg1, s16 arg2, s8 arg3, s8 arg4, s8 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9);

extern f32 func_150ADA68(void);
extern f32 D_800BE9A4;
extern s32 (*D_8008D67C[])(void *);
extern void func_1518F8E0(void *);

struct Obj7C4 { f32 unk0; f32 unk4; f32 unk8; char pad[0x4C]; s8 unk58; };
s32 func_1518F7C4(void *arg0);

extern void (*D_8008D680[])(void);
void func_1518F858(void *arg0);

extern f32 func_150ADA68(void);
extern void func_1518F8E0(void *);

struct Vec898 { f32 unk0; f32 unk4; f32 unk8; f32 unkC; f32 unk10; };
struct Obj898 { char pad[0x30]; struct Vec898 vec; };
void func_1518F89C(struct Obj898 *arg0);

struct Struct1518F8E0Frame {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ struct17 unk4;
};

struct Struct1518F8E0Sub {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 unk25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0x18];
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ f32 unk50;
};

struct Struct1518F8E0Spawn {
    /* 0x00 */ struct127 *unk0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ struct17 unk8;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 pad1B;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ void *unk20;
};

struct Struct1518F8E0Size {
    f32 unk0;
    f32 unk4;
};

extern void func_15143134(void *, void *, void *);
extern s32 func_1514B8E4(struct17 *, struct Struct1518F8E0Size *, s16, u8, f32 *, f32, f32, f32, u8,
                         u8, u8, s32, u8, s16, s16, s32, u8, s32);
extern void func_151D2BA4(struct Struct1518F8E0Spawn *, struct17 *, s32, u8, s32);
extern f32 D_800A5480;
void func_1518F8E0(void *arg0);

extern void func_1514BE20(void *);

struct Vec3i { s32 x, y, z; };
s32 func_1518FC04(void *arg0, struct Vec3i *arg1);

extern void func_1514BF50(void *);

struct Vec3i2 { s32 x, y, z; };
s32 func_1518FC44(void *arg0, struct Vec3i2 *arg1);

typedef struct Fc84Params {
    u8 pad0[0x30];
    f32 scale;      /* 0x30 */
    f32 base;       /* 0x34 */
    f32 range;      /* 0x38 */
    f32 unk3C;      /* 0x3C */
    s16 unk40;
    s16 unk42;
    s16 unk44;
    s16 unk46;
    u8 pad48[4];
    u8 unk4C;
} Fc84Params;

typedef struct Fc84Info {
    u8 pad0[0x30];
    Fc84Params p;
} Fc84Info;

typedef struct Fc84Sub {
    u8 pad0[0x44];
    f32 unk44;
    f32 unk48;
} Fc84Sub;

typedef struct Fc84Obj {
    u8 pad0[0x1C];
    s16 unk1C;
    u8 pad1E[0x58 - 0x1E];
    s32 unk58;
    u8 pad5C[0x6C - 0x5C];
    s16 unk6C;
    s16 unk6E;
    u8 unk70;
    u8 unk71;
    u8 pad72[0x110 - 0x72];
    Fc84Sub sub;
    u8 pad15C[0x148 - 0x15C];
} Fc84Obj;

typedef struct Fc84Arg0 {
    u8 pad0[0x2C];
    struct102 *obj;
    Fc84Info *info;
    f32 x, y, z;
} Fc84Arg0;
void func_1518FC84(Fc84Arg0 *arg0, f32 *arg1);
s32 func_1518FDC4(void *arg0, u8 *arg1, u8 arg2);

extern u8 D_800A7AC8;
extern u8 D_800A7ACC;
extern u8 D_800A7AD0;
extern u8 D_800A7AD4;
extern u8 D_800A7AD8;
extern u8 D_800A7ADC;
extern u8 D_800A7AE0;
extern u8 D_800A7AE4;
extern u8 D_800A7AE8;
extern u8 D_800A7AEC;
extern u8 D_800A7AF0;
extern u8 D_800A7AF4;
extern u8 D_800A7AF8;
extern u8 D_800A7AFC;
extern u8 D_800A7B00;
extern u8 D_800A7B04;
extern u8 D_800A7B08;
extern u8 D_800A7B0C;
extern u8 D_800A7B10;
u8 *func_1519003C(u8 *arg0, s32 *arg1);

struct Struct1519021CArg1 {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
};

struct Struct1519021CData {
    struct Struct1519021CArg1 *unk0;
    u8 unk4;
    u8 pad5[3];
};

struct Struct1519021CLocal {
    s32 unk0;
    f32 unk4[8];
    struct Struct1519021CData unk24;
    Header unk2C;
    s32 pad34;
};
struct225 *func_1519021C(s32 arg0, struct Struct1519021CArg1 *arg1, u8 arg2, s16 arg3, u8 arg4, s32 arg5);

extern void func_15163DEC(void *, void *);
extern s32 func_15163F50(void *, void *);
void func_151903D0(void *arg0);
void func_15190400(struct102 *arg0, void *arg1, u8 arg2);
void func_15190454(void *arg0);

extern void func_15190454(void *);
extern void func_151617C4(void *);
void func_15190464(void *arg0);

extern void func_151617E4(void *);
void func_15190490(void *arg0);

extern void func_1000FD38(void *, void *, s32);
extern void func_151D2B4C(s32);
void func_151904BC(void *arg0);

struct tmp15190518 {
    s32 unk0;
    s8  unk4;
};
void func_15190518(void *arg0);

extern void func_151D33FC(void *, s32);
extern void (*D_8008D684[])(void *, s32, u8);
void func_15190550(void *arg0, s32 arg1, u8 arg2);

extern s32 func_1505D024(void *, s32, u16, s32);
s32 func_151905BC(struct TimedSoundObject *arg0);
void func_151906C4(void *arg0);
void func_151906E0(void *arg0, s32 *arg1, u8 arg2);

extern void func_151D343C(void *, s32);
extern void func_1518F45C(s32, u8);

struct tmp1519072C {
    s32 unk0;
    s8  unk4;
};
void func_1519072C(void *arg0);

extern void func_15191B8C(s32, u8);
extern u8 func_151D8E20(void);

struct Struct15190770Local {
    s32 unk0;
    u8 unk4;
    u8 pad5[0x3];
};
void *func_15190770(void *arg0, s32 arg1, s32 arg2, s32 arg3);

struct Struct15191400Arg;
extern void func_15191400(struct Struct15191400Arg *);
void func_15190840(void *arg0);
void func_1519086C(void *arg0);

struct OscillatorMotionState {
    u8 pad_0x0[0x21];
    u8 field_0x21;
    u8 field_0x22;
    s8 field_0x23;
    s8 field_0x24;
    u8 pad_0x25[0x3];
    f32 field_0x28;
    f32 field_0x2C;
    f32 field_0x30;
    f32 field_0x34;
};

struct OscillatorObject {
    u8 pad_0x0[0x2C];
    f32 field_0x2C;
    f32 field_0x30;
    u8 pad_0x34[0xDC];
    struct OscillatorMotionState field_0x110;
};
s32 func_15190F9C(struct OscillatorObject *arg0);

struct Struct1519108C {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    char pad44[0x4];
    f32 unk48;
    char pad4C[0x74];
    char unkC0[0x40];
    s16 *unk100[0xB];
    f32 unk12C;
};
s16 *func_1519108C(struct Struct1519108C *arg0, s16 arg1);

struct Struct15191400Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[0x14];
    u8 unk20;
};
struct Struct15191400Obj {
    u8 pad0[0x1C];
    s16 unk1C;
    u8 pad1E[0x3A];
    s32 unk58;
    u8 pad5C[0xB4];
    struct Struct15191400Sub unk110;
};
struct Struct15191400Slot {
    struct Struct15191400Obj *unk0;
    s32 unk4;
    s32 unk8;
};
struct Struct15191400Range {
    s16 unk0;
    s16 unk2;
    u8 pad4[0x10];
};
struct Struct15191400Arg {
    u8 pad0[0x1C];
    f32 unk1C;
    s32 unk20;
    struct Struct15191400Slot *unk24;
};
extern struct Struct15191400Range D_800A9D40[];
extern void func_1513F680();
void func_15191400(struct Struct15191400Arg *arg0);

struct Sub15191520 {
    f32 vx;
    f32 vy;
    f32 vz;
    f32 grav;
    f32 drag;
    u8 pad_14[4];
    f32 damp;
    u8 pad_1C[0x1C];
    f32 floor;
    u8 unk3C[0x19];
    u8 type;
    u8 pad_56[6];
    u8 color;
};

struct Obj15191520 {
    u8 pad_0;
    u8 unk1;
    u8 pad_2[0xA];
    u8 unkC;
    u8 pad_D[0x1F];
    f32 unk2C;
    f32 unk30;
    f32 x;
    f32 y;
    f32 z;
    f32 x2;
    f32 y2;
    f32 z2;
    u8 pad_4C[0x10];
    u8 unk5C;
    u8 pad_5D[0xB3];
    struct Sub15191520 sub;
};

s32 func_1514672C(struct17 *arg0);
s32 func_15046C80(f32 *arg0, s32 arg1, f32 arg2, void *arg3);
void func_151DAB58(u8, f32, u8, struct17 *, s32, u8, u8);
s8 func_15191520(struct Obj15191520 *arg0);
s32 func_1519187C(void *arg0);
void func_151918BC(void *arg0);
void func_15191980(struct102 *arg0, s32 arg1, u8 arg2);

void func_1516972C(struct102 *arg0);
void func_15169850(s32, u8, s32, s32, s32);
void func_15191A84(struct102 *arg0, s32 arg1, u8 arg2);


extern s32 D_800A8010;
void func_15191B8C(s32 arg0, u8 arg1);
