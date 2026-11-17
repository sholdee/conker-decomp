#include <ultra64.h>
#define func_15048A40 func_15048A40_void_unused
#define func_10010F88 func_10010F88_void_decl_in_functions_h
#include "functions.h"
#undef func_15048A40
#undef func_10010F88
s32 func_10010F88(s32 arg0, u16 arg1, s16 arg2, u8 arg3, s32 arg4, s16 arg5, s16 arg6, s16 arg7, s16 arg8, s16 arg9);
#include "variables.h"
extern f32 func_15048A40(u8);
extern void func_15188010(s32, f32 *);
extern s32 func_151149AC(u32);
extern f32 D_800A2F8C;
extern void *allocate_memory(s32, s32, s32, s32);
extern f32 D_800A3210;
extern f32 D_800A3214;
extern f32 D_800A2FF4;
extern f32 D_800A2FF8;
extern f32 D_800A2FFC;
extern f32 D_800A2FB0;
extern f32 D_800A2FB4;

struct Struct1511F31CArg {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x28];
    u16 unk3E;
    u8 pad40[0x14];
    u16 unk54;
};

extern void func_1511F31C(struct Struct1511F31CArg *arg0);

struct Struct1511490C {
    char pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    char pad16[2];
    f32 unk18;
    char pad1C[0x10];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
};

extern void func_1511490C(f32 arg0[4][4], struct Struct1511490C *arg1);
extern void func_150A7960(f32 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6);

struct Obj151151FC {
    f32 unk0;
    u8 pad4[0x38];
    s32 unk3C;
    u8 pad40[0x20];
    f32 unk60;
};

struct Obj151150BC {
    u8 pad0[0x8];
    f32 unk8;
    u8 padC[0x30];
    s32 unk3C;
    u8 pad40[0x28];
    f32 unk68;
};

struct Obj1511515C {
    u8 pad0[0x4];
    f32 unk4;
    u8 pad8[0x34];
    s32 unk3C;
    u8 pad40[0x24];
    f32 unk64;
};

struct Obj151152A8 {
    char pad_0x0[0x12];
    s16 field_0x12;
    char pad_0x14[0x28];
    s32 field_0x3C;
    char pad_0x40[0xF];
    u8 field_0x4F;
    char pad_0x50[0x23];
    u8 field_0x73;
    char pad_0x74[0x8];
    s32 field_0x7C;
};
void func_151150B0(s32 arg0);
void func_151150BC(struct Obj151150BC *arg0);
void func_1511515C(struct Obj1511515C *arg0);
void func_151151FC(struct Obj151151FC *arg0);
void func_1511529C(s32 arg0);
void func_151152A8(struct Obj151152A8 *arg0);
void func_151155C0(void *arg0);

struct Obj1511575C {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x3C - 0x16];
    s32 unk3C;
    u8 pad40[0xF];
    u8 unk4F;
    u8 pad50[0xA];
    s16 unk5A;
    s16 unk5C;
    s16 unk5E;
    u8 pad60[0x1C];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
};

extern s32 func_10010E78(u16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_1511575C(struct Obj1511575C *arg0);

extern f32 D_800A2FA0;
extern f32 D_800A2FA4;

struct S_15115EDC { u8 pad[0x7C]; f32 unk7C; f32 unk80; };
struct A_15115EDC { u8 pad[0x84]; u16 unk84; };

struct B_15115E0C {
    u8 pad0[0x4];
    f32 unk4;
    u8 pad8[0x8];
    s16 unk10;
    u8 pad12[0x2];
    s16 unk14;
    u8 pad16[0x39];
    u8 unk4F;
    u8 pad50[0x2C];
    f32 unk7C;
    f32 unk80;
};

struct C_15115E0C {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x4];
    f32 unk1C;
};

struct Obj151163C0 {
    f32 unk0;
    u8 pad4[0xE];
    s16 unk12;
    u8 pad14[0x4];
    f32 unk18;
    u8 pad1C[0x20];
    s32 unk3C;
    u8 pad40[0x1C];
    s16 unk5C;
    u8 pad5E[0x2];
    f32 unk60;
    u8 pad64[0x18];
    s32 unk7C;
    s32 unk80;
};
void func_15115E0C(struct B_15115E0C *arg0, struct C_15115E0C *arg1);
void func_15115EDC(struct S_15115EDC *arg0, struct A_15115EDC *arg1);

struct Obj1511617C {
    u8 pad0[0x10];
    s16 x;
    s16 y;
    s16 z;
    u8 pad16[0x26];
    s32 vel;
    u8 pad40[0xF];
    u8 flags;
    u8 pad50[0xC];
    s16 dy;
    u8 pad5E[0x1E];
    f32 ground;
};

extern u16 D_800CC354;
extern f32 D_800A2FA8;
extern f32 D_800A2FAC;
extern f32 func_1510F648(f32, f32, f32);
void func_1511617C(struct Obj1511617C *arg0);
void func_151162D4(void *arg0);
void func_151163C0(struct Obj151163C0 *arg0);

struct Obj1511650C {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x26];
    s32 unk3C;
    u8 pad40[0xF];
    u8 unk4F;
    u8 pad50[0x14];
    f32 unk64;
    u8 pad68[0xC];
    u16 unk74;
    u8 pad76[0x6];
    s32 unk7C;
    s32 unk80;
};

struct Obj15116888;
s32 func_15116888(s32 arg0, s32 arg1, s32 arg2, struct Obj15116888 *arg3);
void func_1511650C(struct Obj1511650C *arg0, s32 arg1, s32 arg2, f32 arg3);

struct Obj15116888 {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x4];
    f32 unk1C;
    u8 pad20[0x1C];
    f32 unk3C;
    u8 pad40[0x36];
    u16 unk76;
};

struct Vec15116888 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[0x4];
};

s32 func_150490A8(struct17 *arg0);
s32 func_15116888(s32 arg0, s32 arg1, s32 arg2, struct Obj15116888 *arg3);
void func_15116924(s32 arg0);
void func_15116930(u8 *arg0, u8 *arg1);

struct Obj15116BAC {
    u8 pad0[0x10];
    s16 unk10[3];
    u8 pad16[0x26];
    s32 unk3C;
    u8 pad40[0x1A];
    s16 unk5A[3];
    u8 pad60[0x14];
    u16 unk74;
    u8 pad76[0x6];
    s32 unk7C[3];
};

struct SoundEntry15116BAC {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
};

extern struct SoundEntry15116BAC D_80089260[];
extern struct SoundEntry15116BAC D_80089264[];
extern struct SoundEntry15116BAC D_80089268[];
extern void func_15114D24(struct Obj15116BAC *, s32, s32, s32, s32, s32);
extern void func_151169B4(struct Obj15116BAC *arg0);
extern void func_15116BAC(struct Obj15116BAC *arg0);
void func_15116984(u8 *arg0);
void func_151169B4(struct Obj15116BAC *arg0);
void func_15116BAC(struct Obj15116BAC *arg0);

struct Obj15116D7C {
    f32 unk0[3];
    u8 padC[0x30];
    s32 unk3C;
    u8 pad40[0x20];
    f32 unk60[3];
    u8 pad6C[0x10];
    f32 unk7C[3];
};
void func_15116D7C(struct Obj15116D7C *arg0);

typedef struct {
    u8 frames;
    u8 count;
} Anim15116EA4;

typedef struct {
    s32 start;
    s32 end;
} Rom15116EA4;

extern Anim15116EA4 D_80089320[];
extern Rom15116EA4 D_8002AAD0[];

struct Obj15116EA4 {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0xA];
    Vtx *unk20[2];
    u8 pad28[0x14];
    s32 unk3C;
    u8 pad40[0x3C];
    s32 unk7C;
    s16 *unk80;
    s32 unk84;
};
void func_15116EA4(struct Obj15116EA4 *arg0);

typedef struct Motion151172D8 {
    u8 pad0[0x3C];
    s32 packedTarget;
    u8 pad40[0x3C];
    f32 damping;
    f32 acceleration;
    f32 velocity;
} Motion151172D8;

extern f32 D_800A2FD8;
f32 func_151172D8(Motion151172D8 *arg0, f32 angle);

struct Obj151174A0 {
    char pad0[0x8];
    f32 unk8;
};
void func_151174A0(struct Obj151174A0 *arg0);

struct Obj151174C8 {
    char pad0[0x4];
    f32 unk4;
};
void func_151174C8(struct Obj151174C8 *arg0);

struct Obj151174F0 {
    f32 unk0;
};
void func_151174F0(struct Obj151174F0 *arg0);
f32 func_15117518(struct Obj151174F0 *arg0, f32 current);

extern f32 func_15117518(struct Obj151174F0 *, f32);
void func_15117770(struct Obj151174F0 *arg0);

struct Obj15117798 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
};

struct Obj15117DA4 {
    f32 unk0;
    f32 unk4;
    u8 pad8[0x8];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x28];
    u16 unk3E;
    u8 pad40[0x14];
    u16 unk54;
};
void func_15117798(struct Obj15117798 *arg0);

s32 func_150AD9A0(s32, s32, s32);
extern f32 D_800A2FDC;
extern f32 D_800A2FE0;
extern f32 D_800A2FE4;
extern f32 D_800A2FE8;
extern f32 D_800A2FEC;
extern f32 D_800A2FF0;
void func_151179BC(void *arg0);
void func_15117D3C(struct Obj151152A8 *arg0, struct127 *arg1);
void func_15117DA4(struct Obj15117DA4 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4);

struct F7Cw {
    u8 p0[0x17];
    u8 unk17;
    u8 p18[0xF];
    u8 unk27;
    u8 p28[0x43];
    u8 unk6B;
    s16 unk6C;
    s16 unk6E;
    s16 unk70;
};

struct F7Tg {
    s32 p0;
    u8 p4[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 p20[0x45];
    u8 unk65;
    u8 p66[0x80];
    s16 unkE6;
    u8 pE8[0x1C];
    u8 unk104;
    u8 p105[0x216];
    struct F7Cw *w;
    u8 p320[0xC];
};

struct F7O {
    f32 unk0;
    u8 p4[0x28];
    f32 ox;
    f32 oy;
    f32 oz;
    u8 p38[0x17];
    u8 unk4F;
    u8 p50[0x23];
    u8 unk73;
    u16 unk74;
    u8 p76[6];
    s32 unk7C;
    f32 unk80;
    f32 unk84;
};

extern void func_15060A30(s32, void *);
void func_15117F3C(struct F7O *o);

s32 func_151189AC(void *arg0, s32 arg1);
s32 func_1511896C(void *arg0);
s32 func_1511898C(void *arg0);

extern void func_15173C60(s32, s32);
extern void func_15173C90(s32, s32, s32);
extern void func_1518804C(s32, f32);
extern void func_1511F990(void *, s32);
extern s32 func_15022B08(s32, s32);
extern struct SoundEntry15116BAC D_80089262[];
extern f32 D_800A3138;
extern f32 D_800A313C;
extern f32 D_800A3140;
extern f32 D_800A3144;
s32 func_151189AC(void *arg0, s32 arg1);

extern void func_1511F990(void *, s32);
extern void func_15173C60(s32, s32);
extern void func_1518804C(s32, f32);
extern void func_151669A0(s32, s32, s32, f32, u8, s32);
extern f32 D_800A3148;
extern f32 D_800A314C;
extern f32 D_800A3150;
extern f32 D_800A3154;
extern f32 D_800A3158;
void func_15118F24(u8 *arg0);

struct Obj151193AC_0 {
    char pad0[0x73];
    u8 unk73;
};

struct Obj151193AC_1 {
    s32 unk0;
    char pad4[0x61];
    u8 unk65;
};
void func_151193AC(struct Obj151193AC_0 *arg0, struct Obj151193AC_1 *arg1);

struct Obj151193F4 {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[0x28];
    s32 unk3C;
    u8 pad40[0x33];
    u8 unk73;
    u8 pad74[0x8];
    s32 unk7C;
    u32 unk80;
    s32 unk84;
};
void func_151193F4(struct Obj151193F4 *arg0);


struct Obj151194D4Node {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u16 unk6;
    char pad8[8];
};

struct Obj151194D4Cmd {
    s8 unk0;
    char pad1[3];
    u32 unk4;
};

struct Obj151194D4 {
    char pad0[0x16];
    u16 unk16;
    char pad18[4];
    struct Obj151194D4Cmd *unk1C;
    char pad20[8];
    struct Obj151194D4Node *unk28;
    char pad2C[0x28];
    u16 unk54;
    char pad56[0x2E];
    s32 unk84;
};
void func_151194D4(struct Obj151194D4 *arg0, struct Obj151194D4Node *arg1, s32 arg2, s32 arg3);

extern f32 func_150489B0(u8);
extern f32 D_800A315C;
void func_151196D4(struct Obj151194D4 *arg0);

typedef struct {
    s16 x, y, z;
    u16 group;
    u8 pad8[8];
} Game144C70Vertex;

typedef struct {
    u8 pad0[4];
    f32 unk4;
    u8 pad8[8];
    s16 x, y, z;
    u16 count;
    u8 pad18[8];
    Game144C70Vertex *outputs[2];
    Game144C70Vertex *vertices;
    f32 scaleX, scaleY, scaleZ;
    u8 pad38[0x1C];
    u16 kind;
    u8 pad56[0x26];
    s32 timer;
    f32 phase;
    s32 active;
} Game144C70VertexState;

extern s32 func_151EF610(void);
struct127 *func_1505EFD0(s32);
extern f32 D_800A3160;
extern f32 D_800A3164;
extern f32 D_800A3168;
extern f32 D_800A316C;
void func_15119938(Game144C70VertexState *arg0);

typedef struct Obj15119FC0 {
    f32 x, y, z;
    u8 pad0C[4];
    s16 position[3];
    u8 pad16[0x26];
    s32 flags;
    u8 pad40[0xF];
    s8 state;
    u8 pad50[0x1E];
    s8 removed;
    u8 pad6F;
    u8 flags70;
    u8 pad71[0xB];
    f32 phase;
    s32 baseHeight;
    s32 timer;
    u8 pad88[2];
    u8 alpha;
} Obj15119FC0;


extern s32 func_15060BA4(void *, s32);
extern void func_1507C3E0(struct127 *, s16 *, s16 *, s16 *);
extern void func_1508EE0C(s32, s32);
extern void func_151D69B4(void *, void *);
void func_15119FC0(Obj15119FC0 *arg0);

typedef struct {
    s32 *unk0;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    u8 unk7;
    u8 unk8;
    u8 pad9[3];
} Struct1511A494Entry;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[0x34];
    u16 unk54;
} Struct1511A494Arg;

extern Struct1511A494Entry D_80089324[];
extern s32 func_1511A410(s32, s32 *);
extern s32 func_1510D0EC(s32, s32 *, s32, s32);
extern void func_1510D874(void *, s32, s32, s32, s32);
void func_1511A494(Struct1511A494Arg *arg0, s32 *arg1, s32 *arg2);

extern void func_15116110(void *);
void func_1511A6FC(s32 *arg0);

extern u8 D_800C35EA;
extern struct131 *D_800DBEF4;
extern s32 func_15022B08(s32, s32);
extern void func_151162D4(void *);
void func_1511A738(struct131 *arg0);

struct S_1511A7C0 {
    u8 pad0[0x4];
    f32 *unk4;
    u8 pad8[0x8];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
};
struct A_1511A7C0 {
    u8 pad0[0x16];
    u16 unk16;
    u8 pad18[0x68];
    struct S_1511A7C0 *unk80;
};
void func_1511A7C0(struct A_1511A7C0 *arg0);

typedef struct {
    u8 *bytes;
    f32 *values;
    f32 *rates;
    f32 *scales;
    f32 field_10;
    f32 field_14;
    f32 field_18;
    u8 field_1C;
    u8 field_1D;
    u8 field_1E;
    s32 field_20;
    s32 field_24;
} Game147C70Output;

typedef struct {
    u8 pad0[0x16];
    u16 value_count;
    u8 pad18[8];
    Vtx_t *lists[2];
    Vtx_t *vertices;
    u8 pad2C[0x10];
    s32 packed3C;
    u8 pad40[0x33];
    u8 flags73;
    u8 pad74[0xC];
    Game147C70Output *output;
    s32 packed84;
} Game147C70State;

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
void func_1511A838(Game147C70State *arg0);

struct Obj1511AF30 {
    f32 pos;
    u8 pad4[0x38];
    s32 level;
    u8 pad40[0x3C];
    f32 base;
    f32 wobble;
    f32 wobbleTarget;
};

extern f32 D_800A3188;
extern f32 D_800A318C;
extern f32 D_800A3190;
extern f32 D_800A3194;

void func_1511AF30(struct Obj1511AF30 *arg0) {
    f32 level;
    f32 target;
    f32 diff;
    s32 r;

    level = arg0->level;
    target = 75.0f * level * D_800A3188;
    if (target != arg0->base) {
        diff = target - arg0->base;
        if (fabsf(diff) < 1.0f) {
            arg0->base = target;
        } else {
            arg0->base += (diff < 0.0f) ? -1 : 1;
        }
        arg0->wobble = 0.0f;
    } else {
        r = (u32)func_150ADA20() % 1000;
        if (r < 500) {
            arg0->wobbleTarget = r * 5.0f * level * D_800A318C * D_800A3190;
        }
        diff = arg0->wobbleTarget - arg0->wobble;
        arg0->wobble += diff * D_800A3194;
    }
    arg0->pos = arg0->base + arg0->wobble;

}

struct Obj1511B07C {
    u8 pad0[0x10];
    s16 unk10;
    u8 pad12[0x2];
    s16 unk14;
    u8 pad16[0x26];
    s32 unk3C;
    u8 pad40[0x33];
    u8 unk73;
    u16 unk74;
    u8 pad76[0x6];
    s32 unk7C;
    s32 unk80;
    f32 unk84;
};

void func_1511B07C(struct Obj1511B07C *arg0) {
    f32 cur;
    f32 speed;
    s32 mode;
    s32 state;
    s32 targetX;
    s32 targetZ;
    s32 d;
    s32 moveX;
    s32 moveZ;
    s32 sound;

    speed = 7.0f;
    moveX = 0;
    moveZ = 0;
    sound = 0;
    if (D_800C35EA == 1) {
        if (func_15022B08((struct131 *)arg0 - D_800DBEF4, 0) != 0) {
            return;
        }
    }
    if (arg0->unk7C == 0) {
        arg0->unk7C = arg0->unk10 | 0x80000000;
        arg0->unk80 = arg0->unk14;
    }
    targetX = (s16)arg0->unk7C;
    targetZ = arg0->unk80;
    state = arg0->unk3C & 0x3FF;
    if (state >= 4 && state < 7) {
        func_1511F990(arg0, 1);
    }
    mode = arg0->unk73 & 3;
    if (mode == 3 || mode == 2) {
        switch (state) {
        case 0:
            targetX = -0x45;
            targetZ = -0x78;
            if (mode == 2) {
                sound = 0x4E;
            }
            moveX = 1;
            break;
        case 1:
            targetX = -0x45;
            targetZ = 0x78;
            moveX = 1;
            break;
        case 2:
            targetX = 0x64;
            if (mode == 2) {
                sound = 0x4E;
            }
            break;
        case 3:
            targetX = 0;
            break;
        case 4:
            speed = 15.0f;
            targetX = 0x10E;
            targetZ = -0x756;
            moveZ = 1;
            if (mode == 2) {
                sound = 0x4E;
            }
            break;
        case 5:
            targetZ = 0x8C;
            if (mode == 2) {
                sound = 0x4C1;
            }
            break;
        case 6:
            targetZ = -0x8C;
            break;
        case 7:
            speed = 15.0f;
            targetX = -0x1004;
            targetZ = 0x1A6;
            moveX = 1;
            if (mode == 2) {
                sound = 0x4E;
            }
            break;
        }
    } else {
        switch (state) {
        case 4:
            speed = 15.0f;
            moveX = 1;
            if (mode == 1) {
                sound = 0x4E;
            }
            break;
        case 7:
            speed = 15.0f;
            moveZ = 1;
            if (mode == 1) {
                sound = 0x4E;
            }
            break;
        }
    }
    if (sound != 0) {
        func_15114D24(arg0, sound, 0x7D00, 0x64, 0x9C4, 1);
    }
    cur = arg0->unk84;
    if (mode == 0 || mode == 3) {
        cur = 0.0f;
        arg0->unk10 = targetX;
        arg0->unk14 = targetZ;
    }
    if (cur < speed) {
        cur += (speed - cur) * 0.02999999933f;
    }
    if ((moveX != 0 || targetZ == arg0->unk14) && targetX != arg0->unk10 && (moveZ == 0 || targetZ == arg0->unk14)) {
        d = targetX - arg0->unk10;
        if (d < -cur) {
            d = -cur;
        } else if (cur < d) {
            d = cur;
        }
        arg0->unk10 += d;
        if (targetX == arg0->unk10) {
            cur = 0.0f;
            if (state == 0) {
                func_15114D24(arg0, 0x4E, 0x7D00, 0x64, 0x9C4, 1);
            }
        }
    } else if (targetZ != arg0->unk14) {
        d = targetZ - arg0->unk14;
        if (d < -cur) {
            d = -cur;
        } else if (cur < d) {
            d = cur;
        }
        arg0->unk14 += d;
    } else if (mode == 2 || mode == 1) {
        arg0->unk73 = ((mode == 2) ? 3 : 0) | (arg0->unk73 &= ~3);
        if (arg0->unk74 != 0) {
            func_100111C8(arg0->unk74);
            arg0->unk74 = 0;
            func_15114D24(arg0, 0x4C8, 0x7D00, 0x64, 0x9C4, 1);
        }
    }
    arg0->unk84 = cur;
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_1483E0/func_1511B51C.s")

struct Vtx1511B7D4 {
    s16 x;
    s16 y;
    s16 z;
    u8 pad6[0xA];
};

struct Obj1511B7D4 {
    u8 pad0[0x8];
    f32 unk8;
    u8 padC[0x4];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0xA];
    struct Vtx1511B7D4 *unk20[2];
    struct Vtx1511B7D4 *unk28;
    u8 pad2C[0x10];
    s32 unk3C;
    u8 pad40[0x3C];
    s32 unk7C;
    f32 unk80;
    f32 unk84;
    u8 pad88[0x18];
};

extern u8 D_800A2F70;
extern u8 D_800A2F71[];

void func_1511B7D4(struct Obj1511B7D4 *arg0) {
    u8 *table;
    s32 count;
    s32 dx;
    s32 dy;
    f32 angle;
    f32 s;
    f32 c;
    s32 x;
    s32 y;
    s32 z;
    s32 tmp;
    struct Obj1511B7D4 *objs[2];

    count = 0;
    if (arg0->unk3C == 0) {
        table = D_800A2F71;
        count = D_800A2F70;
        objs[0] = arg0 - 1;
        objs[1] = arg0 - 2;
    }
    if (arg0->unk80 != objs[0]->unk8 || arg0->unk84 != objs[1]->unk8) {
        arg0->unk80 = objs[0]->unk8;
        arg0->unk84 = objs[1]->unk8;
        arg0->unk7C = 2;
    }
    if (arg0->unk7C != 0) {
        arg0->unk7C--;
        while (count--) {
            dx = objs[table[1]]->unk10 - arg0->unk10;
            angle = -objs[table[1]]->unk8 * 0.01745329238f;
            dy = objs[table[1]]->unk12 - arg0->unk12;
            s = sinf(angle);
            c = cosf(angle);
            y = arg0->unk28[table[0]].y;
            x = arg0->unk28[table[0]].x;
            z = arg0->unk28[table[0]].z;
            y -= dy;
            x -= dx;
            tmp = (s32)(y * c - x * s);
            x = (s32)(y * s + x * c);
            y = tmp;
            x += dx;
            y += dy;
            x += arg0->unk10;
            y += arg0->unk12;
            z += arg0->unk14;
            arg0->unk20[D_800BE9C0][table[0]].x = x;
            arg0->unk20[D_800BE9C0][table[0]].y = y;
            arg0->unk20[D_800BE9C0][table[0]].z = z;
            table += 2;
        }
    }
}


struct Obj1511BA24 {
    u8 pad0[0x3C];
    s32 unk3C;
    u8 pad40[0x4A];
    u8 unk8A;
};

void func_1511BA24(struct Obj1511BA24 *arg0) {
    f32 sp1C;

    sp1C = 0.0f;
    func_15188010(arg0->unk3C, &sp1C);
    arg0->unk8A = sp1C * 255.0f;
    if (arg0->unk8A == 0) {
        arg0->unk8A = 1;
    }
}

typedef struct Game1483E0UvVertex {
    s16 x, y, z;
    u8 unknown06[2];
    s16 s, t;
    u32 unknown0C;
} Game1483E0UvVertex;

typedef struct Game1483E0UvPair {
    s16 s, t;
} Game1483E0UvPair;

typedef struct Game1483E0State {
    u8 unknown00[0x10];
    s16 x, y, z;
    u16 count;
    u8 unknown18[8];
    Game1483E0UvVertex *buffers[2];
    Game1483E0UvVertex *source;
    u8 unknown2C[0x13];
    u8 reference_id;
    u8 unknown40[0x3C];
    Game1483E0UvPair *coords;
    void *reference;
} Game1483E0State;

extern f32 D_800A31D0;
extern f32 D_800A31D4;
extern f32 D_800A31D8;
extern f32 D_800A31DC;

void func_1511BB04(Game1483E0State *arg0, f32 arg1, f32 arg2,
                   f32 arg3, f32 arg4) {
    Game1483E0UvPair *coords;
    s32 i;
    f32 s_offset;
    f32 t_offset;
    f32 s_scale;
    f32 t_scale;

    if (arg0->coords == 0) {
        coords = allocate_memory(arg0->count * 4, 1, 0, 0);
        arg0->coords = coords;
        for (i = 0; i < arg0->count; i++) {
            coords[i].s = arg0->source[i].s;
            coords[i].t = arg0->source[i].t;
            arg0->buffers[0][i] = arg0->source[i];
            arg0->buffers[1][i] = arg0->source[i];
            arg0->buffers[0][i].x += arg0->x;
            arg0->buffers[0][i].y += arg0->y;
            arg0->buffers[0][i].z += arg0->z;
            arg0->buffers[1][i].x += arg0->x;
            arg0->buffers[1][i].y += arg0->y;
            arg0->buffers[1][i].z += arg0->z;
        }
    }

    coords = arg0->coords;
    s_offset = arg1 - ((f32)arg0->x + D_800A31D0);
    t_offset = arg2 - ((f32)arg0->z + D_800A31D4);
    s_offset = -s_offset;
    s_offset *= D_800A31D8;
    t_offset *= D_800A31DC;
    s_offset -= 1024.0f;
    t_offset -= 1024.0f;
    s_scale = 1.0f;
    t_scale = 1.0f;
    if (arg3 != 1.0f) {
        s_scale = 1.0f / arg3;
    }
    if (arg4 != 1.0f) {
        t_scale = 1.0f / arg4;
    }
    for (i = 0; i < arg0->count; i++) {
        arg0->buffers[D_800BE9C0][i].s =
            (s16)(((f32)coords[i].s + s_offset) * s_scale + 1024.0f);
        arg0->buffers[D_800BE9C0][i].t =
            (s16)(((f32)coords[i].t + t_offset) * t_scale + 1024.0f);
    }
}

struct Obj1511BDF4 {
    u8  pad0[0x3F];
    u8  unk3F;
    u8  pad40[0x40];
    struct127 *unk80;
};

extern void func_1511BB04(Game1483E0State *, f32, f32, f32, f32);

void func_1511BDF4(struct Obj1511BDF4 *arg0) {
    struct127 *temp;
    struct127 *sp;

    temp = arg0->unk80;
    if (temp != 0) {
        sp = temp;
    } else {
        sp = func_15083E90(arg0->unk3F);
    }
    if (sp != 0) {
        func_1511BB04((Game1483E0State *)arg0, sp->x_position, sp->z_position, 1.0f, 1.0f);
    }
}

struct Obj1511BE5C {
    u8  pad0[0x4];
    f32 unk4;
    u8  pad8[0x8];
    s16 unk10;
    s16 unk12;
    s16 unk14;
};

extern f32 D_800A31E0;

void func_1511BE5C(struct Obj1511BE5C *arg0) {
    arg0->unk4 = func_150484A0((f32)arg0->unk10 - D_800DBFF0->unk2F8, (f32)arg0->unk14 - D_800DBFF0->unk300) * D_800A31E0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1483E0/func_1511BEBC.s")

void func_1511C540(void) {
}

struct Obj1511C548 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[0x74];
    f32 unk80;
    f32 unk84;
};

extern f32 D_800A31EC;
extern f32 D_800A31F0;

void func_1511C548(struct Obj1511C548 *arg0) {
    f32 z;
    f32 x;

    x = sinf(arg0->unk80);
    z = sinf(arg0->unk84);
    arg0->unk0 = x * 16.0f;
    arg0->unk8 = z * 16.0f;
    arg0->unk80 += D_800A31EC * D_800BE9A4;
    arg0->unk84 += D_800A31F0 * D_800BE9A4;
    arg0->unk80 = func_15144B68(arg0->unk80);
    {
        extern f32 func_15144B68(f32);

        arg0->unk84 = func_15144B68(arg0->unk84);
    }
    if (arg0->unk8 == 0.0f && arg0->unk84 == 0.0f) {
        arg0->unk0 = 7.5f;
        arg0->unk8 = 8.0f;
    }
}


struct Obj1511C638 {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x26];
    s32 unk3C;
    u8 pad40[0x2E];
    u8 unk6E;
    u8 pad6F[4];
    u8 unk73;
    u8 pad74[8];
    s32 unk7C;
    s32 unk80;
    u8 pad84[6];
    u8 unk8A;
    u8 pad8B[0x15];
};

extern void func_15170F4C(struct Obj1511C638 *, u8, s32);
extern void func_150B2740(u8, u8);
extern s32 func_1000EC24();

void func_1511C638(struct Obj1511C638 *arg0) {
    s32 state;
    struct Obj1511C638 *sub;
    struct Obj1511C638 *other;

    sub = &arg0[1];
    state = arg0->unk73 & 3;
    if (D_800BE9F0 == 4) {
        other = &arg0[2];
    } else {
        other = NULL;
    }
    if (state == 1) {
        state = 0;
        arg0->unk8A = 0;
        sub->unk6E = 0;
        if (other != NULL) {
            other->unk6E = 0;
        }
        if (arg0->unk3C != 0xFF) {
            func_15173C60(0, arg0->unk3C);
        }
    }
    if (D_800BE9B4 != 0) {
        if (state == 3) {
            arg0->unk8A = 0xFF;
            sub->unk6E = 1;
            if (other != NULL) {
                other->unk6E = 1;
            }
        } else {
            arg0->unk8A = 0;
            sub->unk6E = 0;
            if (other != NULL) {
                other->unk6E = 0;
            }
        }
    }
    if (state == 2) {
        if (arg0->unk7C < 0) {
            arg0->unk7C += D_800BE9E4;
        } else {
            if (arg0->unk80 == 0) {
                switch (sub->unk3C & 0xFF) {
                case 1:
                    func_15170F4C(sub, 0xFF, 0);
                    func_10010F30(0x2BE, 0x7FFF, 0x40, 0, 0);
                    break;
                case 0: {
                    u16 sp5A = func_150ADA20() % 3U;
                    u16 sp58;
                    const u16 sounds[3] = { 0x2B6, 0x2BD, 0x2BE };

                    sp58 = ((func_150ADA20() & 1) + sp5A + 1) % 3U;
                    sp5A = sounds[sp5A];
                    sp58 = sounds[sp58];
                    func_10010F30(0x2B7, 0x7FFF, 0x40, 0, 0);
                    func_10010F30(sp5A, 0x7FFF, 10, 0, 0);
                    func_10010F30(sp58, 0x7FFF, 0x36, 0, 0);
                    sp5A = func_150ADA20() % 5U;
                    sp58 = ((func_150ADA20() & 3) + sp5A + 1) % 5U;
                    func_1000FA64(sp5A + 0x2B1, arg0->unk10, arg0->unk12, arg0->unk14, 0x4000, 0x2711, 0x2710, (s32)func_1000EC24, (void *)0x1001E, 0, 0x200, 0);
                    func_1000FA64(sp58 + 0x2B1, arg0->unk10, arg0->unk12, arg0->unk14, 0x4000, 0x2711, 0x2710, (s32)func_1000EC24, (void *)0x1001E, 0, 0, 0);
                    sp5A = func_150ADA20() % 5U;
                    sp58 = ((func_150ADA20() & 3) + sp5A + 1) % 5U;
                    func_1000FA64(sp5A + 0x2B1, arg0->unk10, arg0->unk12, arg0->unk14, 0x4000, 0x2711, 0x2710, (s32)func_1000EC24, (void *)0x3C, 0, 0x300, 0);
                    func_1000FA64(sp58 + 0x2B1, arg0->unk10, arg0->unk12, arg0->unk14, 0x4000, 0x2711, 0x2710, (s32)func_1000EC24, (void *)0x3C, 0, 0x100, 0);
                }
                    break;
                case 2:
                    break;
                case 3:
                    func_150B2740(sub->unk3C >> 8, 0xFF);
                    break;
                }
            }
            arg0->unk8A = 0xFF;
            sub->unk6E = 1;
            if (other != NULL) {
                other->unk6E = 1;
            }
            arg0->unk80 += D_800BE9E4 * 30;
            if (arg0->unk80 >= 0xFF) {
                arg0->unk80 = 0xFF;
                state = 3;
            }
            if (arg0->unk3C != 0xFF) {
                func_15173C60(arg0->unk80, arg0->unk3C);
            }
            if (state == 3) {
                arg0->unk7C = 0;
                arg0->unk80 = 0;
            }
        }
    }
    arg0->unk73 &= ~3;
    arg0->unk73 |= state;
}

extern f32 D_800A31F4;

void func_1511CB2C(s32 arg0, f32 *arg1) {
    *arg1 = D_800A31F4;
}

struct Wave1511CB44 {
    u8 *phases;
    u16 phase;
    u16 speed;
    f32 amp;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 angle;
    s16 (*uv)[2];
};

struct Obj1511CB44 {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u16 unk16;
    u8 pad18[0x8];
    Vtx *unk20[2];
    Vtx *unk28;
    u8 pad2C[0x46];
    u8 unk72;
    u8 pad73[0x9];
    struct Wave1511CB44 *unk7C;
    u8 pad80[0xA];
    u8 unk8A;
};

extern s32 D_800902D8;
extern s32 func_15195FB0(void *, s32, s32, s32, s32, s32, s32);
extern f32 func_150489B0(u8);
extern void func_15094AB8(Vtx *arg0, s16 (*arg1)[2], s32 arg2, f32 arg3, s32 arg4, s32 arg5);

void func_1511CB44(struct Obj1511CB44 *arg0) {
    Vtx *dst;
    Vtx *src;
    s32 pad9C;
    s32 pad98;
    s32 pad94;
    f32 base;
    s32 pad8C;
    s32 created;
    struct Wave1511CB44 *st;
    s32 i;
    f32 scale;
    f32 w;
    f32 s;
    f32 z;
    f32 y;
    u32 half;
    u16 speed;
    u8 c;
    u8 off;
    s32 mode;
    s32 pad5C;
    f32 rate;
    s32 p1;
    s32 p2;

    rate = 0.0f;
    if (D_800BE9F0 == 4) {
        mode = 0;
    } else if (D_800BE9F0 == 0x13) {
        mode = 1;
        if (arg0->unk72 == 0xFE) {
            rate = 0.4530000091f;
            p1 = 0x20;
            p2 = 0x20;
        }
    } else {
        return;
    }
    func_1511CB2C((s32)arg0, &base);
    base -= arg0->unk14;
    if (arg0->unk7C == NULL) {
        created = 1;
        arg0->unk7C = allocate_memory(0x1C, 1, 0, 0);
        st = arg0->unk7C;
        st->phases = allocate_memory(arg0->unk16, 1, 0, 0);
        if (rate != 0.0f) {
            st->uv = allocate_memory(arg0->unk16 * 4, 1, 0, 0);
            for (i = 0; i < arg0->unk16; i++) {
                st->uv[i][0] = arg0->unk28[i].v.tc[0] - 0x2000;
                st->uv[i][1] = arg0->unk28[i].v.tc[1] - 0x2000;
            }
            st->angle = 0.0f;
        }
    } else {
        created = 0;
    }
    st = arg0->unk7C;
    if (created) {
        if (mode == 0) {
            st->speed = 0x1DA;
            st->unk13 = 0x6D;
            st->amp = 100.0f;
            scale = 0.1000000015f;
        } else {
            scale = 1.0f;
            st->speed = 0x259;
            st->unk13 = 0x6D;
            st->amp = 36.0f;
        }
        st->phase = 0;
        dst = arg0->unk20[D_800BE9C0];
        src = arg0->unk28;
        for (i = 0; i < arg0->unk16; i++) {
            s = base - arg0->unk28[i].v.ob[2];
            st->phases[i] = s * scale;
            dst->v.ob[0] = src->v.ob[0] + arg0->unk10;
            dst->v.ob[2] = src->v.ob[2] + arg0->unk14;
            dst++;
            src++;
        }
        st->unk12 = 0;
        st->unk10 = 0;
        if (D_800BE9F0 == 4) {
            if (arg0->unk72 != 0xF2) {
                if (arg0->unk72 == 0xF1) {
                    st->unk11 = 0;
                } else {
                    st->unk11 = 0x80;
                }
                st->unk12 = 1;
            }
        }
        if (mode == 1) {
            st->unkC = func_15195FB0(arg0, D_800902D8, 0, 0, 0, 0, 8);
        }
    }
    dst = arg0->unk20[D_800BE9C0];
    src = arg0->unk28;
    off = st->phase >> 8;
    scale = 0.8100000024f;
    for (i = 0; i < arg0->unk16; i++) {
        w = func_150489B0(st->phases[i] + off);
        y = (f32)(src->v.ob[1] + arg0->unk12) + w * st->amp;
        if (mode == 0) {
            z = src->v.ob[2] + arg0->unk14;
            if (z < 12000.0f) {
                w += 1.0f;
                w *= 0.5f;
                s = (z - 12000.0f) * ((1.0f - scale) * w + scale);
                z = 12000.0f + s;
                dst->v.ob[2] = z;
            }
        }
        dst->v.ob[1] = y;
        if (st->unk12 == 0) {
            w += 1.0f;
            c = 32.5f * w + 190.0f;
            dst->v.cn[0] = c;
            dst->v.cn[1] = c;
            dst->v.cn[2] = c;
        }
        dst++;
        src++;
    }
    speed = st->speed;
    if (mode == 0) {
        half = (u16)(st->speed >> 1);
        c = st->unk13;
        s = (func_150489B0((st->phase >> 8) + c) + 1.0f) * 0.5f * half;
        speed = half + s;
    }
    st->phase -= speed * D_800BE9E4;
    if (st->unk12) {
        st->unk10 += D_800BE9E4 * 2;
        arg0->unk8A = (func_150489B0(st->unk10 + st->unk11) + 1.0f) * 0.5f * 205.0f + 50.0f;
    }
    if (rate != 0.0f) {
        func_15094AB8(arg0->unk20[D_800BE9C0], st->uv, arg0->unk16, st->angle, p1, p2);
        st->angle += rate;
        if (st->angle >= 360.0f) {
            st->angle -= 360.0f;
        } else if (st->angle < 0.0f) {
            st->angle += 360.0f;
        }
    }
}

struct Entry1511D394 {
    f32 unk0;
    f32 unk4;
    u16 unk8;
    u16 unkA;
    u8 unkC;
    u8 padD;
    u16 unkE;
    u8 unk10;
    u8 unk11;
    u8 pad12[2];
};

extern struct Entry1511D394 D_80089444[];

struct Obj1511D394 {
    u8 pad0[0x16];
    u16 unk16;
    u8 pad18[0x8];
    Vtx *unk20[2];
    Vtx *unk28;
    u8 pad2C[0x10];
    s32 unk3C;
    u8 pad40[0x3C];
    s16 *unk7C;
    f32 unk80;
    f32 unk84;
    u8 pad88[2];
    u8 unk8A;
};

struct Owner1511D394 {
    u8 pad0[0x7C];
    u16 *unk7C;
};

void func_1511D394(struct Obj1511D394 *arg0) {
    u16 *src;
    struct Owner1511D394 *owner;
    s32 dx;
    u8 phase;
    s32 dy;
    f32 s;
    f32 c;
    f32 diff;
    s32 offY;
    s32 baseY;
    s32 amp;
    s32 i;
    s16 *buf;
    struct Entry1511D394 *e;
    f32 rate;
    s32 init;
    s32 id;
    f32 step;
    s32 idx;

    id = arg0->unk3C & 0xFF;
    owner = (struct Owner1511D394 *)func_151149AC((u8)id);
    if (owner == NULL) {
        return;
    }
    src = owner->unk7C;
    if (src == NULL) {
        return;
    }
    e = &D_80089444[(arg0->unk3C >> 8) & 0xFF];
    offY = e->unkA;
    baseY = e->unk8;
    phase = e->unkC;
    rate = e->unk0;
    step = e->unk4;
    amp = e->unkE;
    if (arg0->unk7C == NULL) {
        arg0->unk7C = allocate_memory(arg0->unk16 * 2, 1, 0, 0);
        buf = arg0->unk7C;
        for (i = 0; i < arg0->unk16; i++) {
            buf[i] = arg0->unk28[i].v.tc[0];
        }
        e->unk11 = 0;
        init = 1;
    } else {
        init = 0;
        buf = arg0->unk7C;
    }
    i = 0;
    idx = src[2];
    s = (func_150489B0(((idx >> 8) & 0xFF) + phase) + 1.0f) * 0.5f;
    c = (func_15048A40(((idx >> 8) & 0xFF) + phase) + 0.5f) * 0.5f;
    if (init) {
        arg0->unk84 = s;
        arg0->unk80 = 0.0f;
    } else {
        diff = (s - arg0->unk84) * rate;
        if (diff > 0.0f) {
            arg0->unk80 += step;
            if (diff < arg0->unk80) {
                arg0->unk80 = diff;
            }
        } else {
            arg0->unk80 -= step;
            if (arg0->unk80 < diff) {
                arg0->unk80 = diff;
            }
        }
        arg0->unk84 += arg0->unk80;
    }
    dy = arg0->unk84 * offY;
    dy += baseY;
    amp = amp * c;
    if (arg0->unk20[D_800BE9C0][0].v.tc[1] > 0x800) {
        amp -= 0x800;
    }
    for (i = 0; i < arg0->unk16; i++) {
        arg0->unk20[D_800BE9C0][i].v.tc[0] = buf[i] - dy;
        arg0->unk20[D_800BE9C0][i].v.tc[1] = arg0->unk20[!D_800BE9C0][i].v.tc[1] + amp;
    }
    e->unk11 += D_800BE9E4 * 2;
    arg0->unk8A = (func_150489B0(e->unk11 + e->unk10) + 1.0f) * 0.5f * 205.0f + 50.0f;
}

typedef struct Game1483E0WaveState {
    Game1483E0UvPair *coords;
    s32 field4;
    s32 field8;
} Game1483E0WaveState;

typedef struct Game1483E0WaveMesh {
    u8 pad0[0x16];
    u16 count;
    u8 pad18[8];
    Game1483E0UvVertex *buffers[2];
    Game1483E0UvVertex *source;
    u8 pad2C[0x10];
    s32 flags;
    u8 pad40[0x3C];
    Game1483E0WaveState *wave;
    s32 phase;
} Game1483E0WaveMesh;

void func_1511D7BC(Game1483E0WaveMesh *arg0) {
    f32 sine;
    s32 index;
    Game1483E0UvPair *coords;
    struct {
        s32 speed;
        s32 amplitude_t;
        s32 amplitude_s;
    } parameters;
    s16 s_delta;
    s16 t_delta;
    f32 cosine;
    u8 angle;
    Game1483E0WaveState *state;

    state = arg0->wave;
    if (state == NULL) {
        state = allocate_memory(0xC, 1, 0, 0);
        arg0->wave = state;
        state->field4 = 0;
        state->field8 = 0;
        coords = allocate_memory(arg0->count * 4, 1, 0, 0);
        state->coords = coords;
        for (index = 0; index < arg0->count; index++) {
            coords[index].s = arg0->source[index].s;
            coords[index].t = arg0->source[index].t;
        }
    } else {
        coords = arg0->wave->coords;
    }
    parameters.amplitude_s = arg0->flags & 0xFFF;
    parameters.amplitude_t = (arg0->flags >> 12) & 0xFFF;
    parameters.speed = (arg0->flags >> 24) & 0xFF;
    angle = arg0->phase >> 4;
    cosine = func_15048A40(angle);
    sine = func_150489B0(angle);
    arg0->phase += parameters.speed * D_800BE9E4;
    s_delta = (f32)parameters.amplitude_s * sine;
    t_delta = (f32)parameters.amplitude_t * cosine;
    for (index = 0; index < arg0->count; index++) {
        arg0->buffers[D_800BE9C0][index].s = coords[index].s + s_delta;
        arg0->buffers[D_800BE9C0][index].t = coords[index].t + t_delta;
    }
    func_1511A494((Struct1511A494Arg *)arg0, &state->field4, &state->field8);
}

struct Obj1511D9E4 {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u16 unk16;
    u8 pad18[0x8];
    Vtx *unk20[2];
    Vtx *unk28;
    u8 pad2C[0x10];
    s32 unk3C;
    u8 pad40[0x3C];
    s16 (*unk7C)[2];
    f32 unk80;
};

extern void func_15094AB8(Vtx *arg0, s16 (*arg1)[2], s32 arg2, f32 arg3, s32 arg4, s32 arg5);

void func_1511D9E4(struct Obj1511D9E4 *arg0) {
    s16 (*uv)[2];
    s32 hi;
    s32 mid;
    s32 i;
    f32 speed;

    if (arg0->unk7C == NULL) {
        uv = arg0->unk7C = allocate_memory(arg0->unk16 * 4, 1, 0, 0);
        for (i = 0; i < arg0->unk16; i++) {
            uv[i][0] = arg0->unk28[i].v.tc[0] - 0x2000;
            uv[i][1] = arg0->unk28[i].v.tc[1] - 0x2000;
        }
        for (i = 0; i < arg0->unk16; i++) {
            arg0->unk20[0][i].v.ob[0] = arg0->unk20[1][i].v.ob[0] = arg0->unk28[i].v.ob[0] + arg0->unk10;
            arg0->unk20[0][i].v.ob[1] = arg0->unk20[1][i].v.ob[1] = arg0->unk28[i].v.ob[1] + arg0->unk12;
            arg0->unk20[0][i].v.ob[2] = arg0->unk20[1][i].v.ob[2] = arg0->unk28[i].v.ob[2] + arg0->unk14;
        }
    } else {
        uv = arg0->unk7C;
    }
    hi = arg0->unk3C >> 24;
    mid = (arg0->unk3C >> 16) & 0xFF;
    speed = (f32)(s16)(arg0->unk3C & 0xFFFF) * 0.01;
    arg0->unk80 += speed * D_800BE9E4;
    func_15094AB8(arg0->unk20[D_800BE9C0], uv, arg0->unk16, arg0->unk80, hi, mid);
}

struct Obj1511DBC4Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
};

struct Obj1511DBC4 {
    u8 pad0[0x8];
    f32 unk8;
    u8 padC[0x67];
    u8 unk73;
    u8 pad74[0x8];
    struct Obj1511DBC4Sub *unk7C;
};

void func_1511DBC4(struct Obj1511DBC4 *arg0) {
    s32 state;
    struct Obj1511DBC4Sub *temp;
    f32 ftemp;

    if (arg0->unk7C == NULL) {
        temp = allocate_memory(0x10, 1, 0, 0);
        arg0->unk7C = temp;
        temp->unk0 = 0.0f;
        temp->unk4 = 0.0f;
        temp->unk8 = 0.0f;
        temp->unkC = D_800A3210;
    } else {
        temp = arg0->unk7C;
    }

    state = arg0->unk73 & 3;
    if (state == 0) {
        arg0->unk8 = 0.0f;
    } else if (state == 3) {
        arg0->unk8 = 90.0f;
        ftemp = cosf(temp->unk0 * D_800A3214);
        ftemp = ftemp * 0.5f;
        arg0->unk8 = arg0->unk8 + ftemp;
        temp->unk0 = temp->unk0 + (3.0f * (f32)D_800BE9E4);
    } else if (state == 2) {
        if (arg0->unk8 == 0.0f) {
            temp->unk4 = 0.0f;
            temp->unk8 = 0.0f;
        }
        temp->unk8 = temp->unk8 + temp->unkC;
        temp->unk4 = temp->unk4 + temp->unk8;
        arg0->unk8 = arg0->unk8 + temp->unk4;
        if (arg0->unk8 >= 90.0f) {
            state = 3;
            arg0->unk8 = 90.0f;
        }
    } else {
        if ((f32)D_800BE9E4 < arg0->unk8) {
            arg0->unk8 = arg0->unk8 - (f32)D_800BE9E4;
        } else {
            arg0->unk8 = 0.0f;
            state = 0;
        }
    }

    arg0->unk73 &= 0xFFFC;
    arg0->unk73 |= state;
}

void func_10010558(u16, void *, s32, s32, s32, s32);
void func_1511DD98(u8 *arg0, u8 *arg1) {
    s32 sound;
    s32 volume;
    s32 type_3B;
    s32 type_80;

    volume = 0x6590;
    type_3B = arg1[4] == 0x3B;
    type_80 = arg1[4] == 0x80;
    sound = 0;
    switch (*(u16 *)(arg0 + 0x54)) {
    case 0x39:
    case 0x5C:
    case 0x5D:
    case 0x5E:
        if (type_3B) {
            func_10010630(0x542, (struct127 *)arg1, 0x6590, 100, 1000);
        } else if (type_80) {
            func_10010630(0x567, (struct127 *)arg1, 0x6590, 100, 1000);
        }
        sound = 0x2B0;
        break;
    case 0x3A:
        if (type_3B) {
            sound = (func_150ADA20() & 1) + 0x53E;
        } else if (type_80) {
            sound = 0x567;
        } else {
            sound = 0x60A;
        }
        break;
    case 0x3B:
    case 0x51:
        sound = 0x181;
        break;
    case 0x3C:
    case 0x6A:
        sound = 0xB1;
        break;
    case 0x3D:
    case 0x6B:
        sound = 0x442;
        break;
    case 0x3E:
        sound = 0x444;
        break;
    case 0x46:
        sound = 0x2F0;
        break;
    case 0x47:
        sound = 0x4A3;
        break;
    case 0x50:
    case 0x55:
        sound = (func_150ADA20() & 1) + 0xB;
        break;
    case 0x52:
    case 0x58:
        sound = 0x510;
        break;
    case 0x53:
        sound = 0x2AF;
        if (type_80) {
            func_10010558(0x567, (struct127 *)arg1, 0x7D00, 100, 1000, 30);
        }
        break;
    case 0x56:
        if (type_80) {
            func_10010630(0x567, (struct127 *)arg1, 0x6590, 100, 1000);
        }
        sound = 0x5AE;
        break;
    case 0x57:
    case 0x6D:
    case 0x6E:
        sound = 0x51;
        volume = 0x2328;
        break;
    case 0x64:
    case 0x65:
        sound = 0x629;
        volume = 0x7FFF;
        break;
    }
    if (sound != 0) {
        func_10010630(sound, (struct127 *)arg1, volume, 100, 1000);
    }
}

struct Obj1511DF6CSub {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    f32 unk8;
    f32 unkC;
};

struct Obj1511DF6C {
    f32 unk0;
    f32 unk4;
    u8 pad8[0x8];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x2];
    f32 unk18;
    u8 pad1C[0x10];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    u8 pad38[0x1C];
    u16 unk54;
    u8 pad56[0x18];
    u8 unk6E;
    u8 pad6F[0xD];
    struct Obj1511DF6CSub *unk7C;
    s32 unk80;
    s32 unk84;
};

extern void *func_1509B570(s32);
extern void func_1507C3E0(struct127 *, s16 *, s16 *, s16 *);
extern void func_151D6970(struct127 *, struct Obj1511DF6C *);

void func_1511DF6C(struct Obj1511DF6C *arg0) {
    f32 dist;
    s32 hp;
    f32 radius;
    f32 scale;
    s16 height;
    s16 sp64;
    s16 sp62;
    struct Obj1511DF6CSub *sub;
    s32 skip;
    s32 anim;
    s32 i;
    struct127 *obj;
    u8 *res;
    f32 y;

    if (arg0->unk7C == NULL) {
        arg0->unk7C = allocate_memory(0x10, 1, 0, 0);
        sub = arg0->unk7C;
        bzero(sub, 0x10);
        sub->unkC = arg0->unk2C;
        arg0->unk34 = 0.0f;
        arg0->unk30 = 0.0f;
        arg0->unk2C = 0.0f;
    } else {
        sub = arg0->unk7C;
    }
    skip = 0;
    anim = -1;
    switch (arg0->unk54) {
    case 0x59:
    case 0x5A:
        break;
    case 0x54:
        anim = 0x17;
        if (D_800BE9F0 == 0x2D) {
            res = func_1509B570(0xBA);
            if (res != NULL && (hp = *(s32 *)(res + 0x1FE4)) <= 0) {
                skip = 1;
            }
        }
    default:
        arg0->unk4 += (f32)D_800BE9E4 * 1.519999981f;
        dist = cosf(sub->unk0 * 9.587380191e-05f);
        arg0->unk0 = 8.0f * dist;
        dist = cosf(sub->unk2 * 9.587380191e-05f);
        arg0->unk18 = 12.0f * dist;
        sub->unk0 += (f32)D_800BE9E4 * 507.0f;
        sub->unk2 += (f32)D_800BE9E4 * 419.0f;
        break;
    }
    if (sub->unk4 < 3 && skip == 0) {
        for (i = 0; i < D_8008FD8C; i++) {
            obj = &D_800CC2D0[i];
            if (obj->interaction_state == 0 || obj->health <= 0 || obj->unk127 == 0xFF) {
                continue;
            }
            if (obj->unk300 == 0) {
                continue;
            }
            if ((arg0->unk84 == 1 && *(u8 *)&obj->pad128 != 0) || (arg0->unk84 == 2 && *(u8 *)&obj->pad128 == 0) ||
                (arg0->unk84 == 3 && (*(u8 *)&obj->pad128 & 1))) {
                continue;
            }
            if (anim != -1 && func_1503195C(obj, anim, 0) != NULL) {
                continue;
            }
            dist = ((f32)arg0->unk10 - obj->x_position) * ((f32)arg0->unk10 - obj->x_position) + ((f32)arg0->unk14 - obj->z_position) * ((f32)arg0->unk14 - obj->z_position);
            res = (u8 *)obj->unk31C;
            radius = 100.0f;
            scale = (f32)(*(s8 *)(res + 0x1AF) & 0xFF) * 0.015625f;
            if (res[0x84] != 0) {
                scale *= 1.399999976f;
            }
            radius *= scale;
            if (dist < radius * radius) {
                func_1507C3E0(obj, &height, &sp64, &sp62);
                y = (f32)arg0->unk12;
                if (y < *(f32 *)&obj->pad17C && obj->y_position < y + radius &&
                    y - radius < obj->y_position + height) {
                    arg0->unk80 = obj->unique_id;
                    sub->unk4 = 3;
                    func_1511DD98((u8 *)arg0, (u8 *)obj);
                    func_151D6970(obj, arg0);
                    break;
                }
            }
        }
    }
    if (sub->unk4 == 0) {
        arg0->unk34 += 0.2599999905f * sub->unkC;
        if (sub->unkC <= arg0->unk34) {
            arg0->unk34 = sub->unkC;
            sub->unk6 = 0;
            sub->unk4++;
            sub->unk8 = 0.3000000119f;
        }
        arg0->unk30 = arg0->unk34;
        arg0->unk2C = arg0->unk34;
    } else if (sub->unk4 == 1) {
        sub->unk8 -= 0.01999999955f * sub->unkC;
        if (sub->unk8 <= 0.0f) {
            sub->unk4++;
            sub->unk8 = 0.0f;
        }
        arg0->unk34 = cosf(sub->unk6 * 9.587380191e-05f) * sub->unk8 + sub->unkC;
        arg0->unk2C = arg0->unk30 = arg0->unk34;
        sub->unk6 += D_800BE9E4 * 0x1613;
    } else if (sub->unk4 == 2) {
    } else if (sub->unk4 == 3) {
        arg0->unk34 -= 0.2599999905f * sub->unkC;
        if (arg0->unk34 <= 0.0f) {
            sub->unk4 = 0;
            arg0->unk6E = 1;
            arg0->unk34 = 0.0f;
        }
        arg0->unk30 = arg0->unk34;
        arg0->unk2C = arg0->unk34;
    }
}

struct Obj1511EC50 {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[0x28];
    s32 unk3C;
    u8 pad40[0x2E];
    s8 unk6E;
    u8 pad6F[0xD];
    s32 unk7C;
    s32 unk80;
    f32 unk84;
};

struct Obj1511ED84 {
    u8 pad0[0x14];
    s16 unk14;
    u8 pad16[0x5C];
    u8 unk72;
    u8 pad73[0x9];
    s32 unk7C;
    s32 unk80;
    f32 unk84;
};
void func_1511EC50(struct Obj1511EC50 *arg0);
void func_1511ED84(struct Obj1511ED84 *arg0);

typedef struct {
    f32 x, y, z, radius, angle, dx, dz;
    s16 timer;
    u8 pad1E[6];
} Game11EF40Motion;

typedef struct {
    f32 x, y, z;
} Vec3f_1511EF40;

extern s32 func_15086CBC(s32, f32 *, f32 *, f32 *);
extern s32 func_15086D48(s32);
extern f32 D_800A3378;
void func_1511EF40(u8 *arg0);
void func_1511F31C(struct Struct1511F31CArg *arg0);

struct Obj1511F3E8 {
    u8 pad0[0x3C];
    s32 unk3C;
    u8 pad40[0x2E];
    u8 unk6E;
    u8 pad6F[0xD];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    u8 pad88[2];
    u8 unk8A;
};
void func_1511F3E8(struct Obj1511F3E8 *arg0);
void func_1511F768(s32 *arg0, s32 arg1);

struct Obj1511F788 {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[0x4];
    f32 unk18;
    u8 pad1C[0x20];
    s32 unk3C;
    u8 pad40[0x1C];
    s16 unk5C;
    u8 pad5E[0x10];
    s8 unk6E;
    u8 pad6F[0xD];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
};

union Temp1511F788 {
    struct {
        f32 pad;
        s32 w;
    } s;
    f64 d;
};
void func_1511F788(struct Obj1511F788 *arg0);

extern s32 func_151149AC(u32);
void func_1511F92C(s16 *arg0);
