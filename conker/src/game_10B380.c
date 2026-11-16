#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern f32 D_800A0C00;
extern f32 D_800A0C04;
extern f32 D_800A0C08;
extern f32 D_800A0C0C;
extern f32 D_800A0C10;
extern f32 D_800A0C14;
extern f32 D_800A0C18;
extern f32 D_800A0C1C;
extern f32 D_800A0C20;
extern f32 D_800A0C24;
extern f32 D_800A0C28;
extern f32 D_800A0C2C;
extern f32 D_800A0C30;
extern s32 D_80090514;

void func_15152F70(void *arg0, u8 arg1);
void func_15153298(void *arg0, u8 arg1, s32 arg2);
void func_15196438(s32 arg0, s32 arg1, u8 arg2, s32 arg3);

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s8 unk38;
    s8 unk39;
    u8 pad3A[2];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 unk54;
    u8 unk55;
    u8 pad56[2];
} Struct150DB9E0A;

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    s16 unk0C;
    s16 unk0E;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s8 unk28;
    s8 unk29;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    void *unk38;
    u8 unk3C;
    u8 unk3D;
    u8 pad3E[2];
} Struct150DB9E0B;

typedef struct {
    u8 pad_0x00[0x20];
    u8 field_0x20;
} EffectSpawnState;

typedef struct {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    f32 field_0x0C;
    f32 field_0x10;
} EffectSpawnPoint;

typedef struct {
    u8 pad_0x00[0x1];
    u8 field_0x01;
    u8 pad_0x02[0xA];
    u8 field_0x0C;
    u8 pad_0x0D[0x20];
    s8 field_0x2D;
    u8 pad_0x2E[0x66];
    EffectSpawnPoint *field_0x94;
    EffectSpawnState *field_0x98;
} EffectSpawnSource;

typedef struct {
    s32 field_0x00;
    s16 field_0x04;
    s8 field_0x06;
    u8 pad_0x07[0x1];
    s32 field_0x08;
    s32 field_0x0C;
    u8 field_0x10;
    u8 field_0x11;
    u8 field_0x12;
    u8 field_0x13;
    u8 field_0x14;
    u8 field_0x15;
    u8 pad_0x16[0x2];
    s32 field_0x18;
    u8 pad_0x1C[0xC];
    EffectSpawnPoint *field_0x28;
    EffectSpawnState *field_0x2C;
} EffectSpawnParams;
void func_150DB9E0(s32 arg0, s32 arg1, u8 arg2);
void func_150DBC60(void *arg0);

typedef struct {
    f32 u00;
    u8 u04;
    u8 pad05[0x1];
    s16 u06;
    f32 u08;
    u8 u0C;
    u8 pad0D[0x1];
    s16 u0E;
    f32 u10;
    f32 u14;
    f32 u18;
    f32 u1C;
    f32 u20;
    f32 u24;
    u8 u28;
    u8 u29;
    u8 pad2A[0x2];
    f32 u2C;
    f32 u30;
    u8 u34;
    u8 pad35[0x1];
    s16 u36;
    f32 u38;
    u8 u3C;
    u8 pad3D[0x1];
    s16 u3E;
    u8 u40;
    u8 u41;
    u8 u42;
    u8 pad43[0x1];
    s32 u44;
    s32 u48;
    s32 u4C;
    s32 u50;
    s32 u54;
    s32 u58;
    s32 u5C;
    u8 u60;
    u8 pad61[0x3];
    f32 u64;
    f32 u68;
    f32 u6C;
    s16 u70;
    s16 u72;
    f32 u74;
    f32 u78;
    s16 u7C;
    s16 u7E;
    f32 u80;
    s16 u84;
    s16 u86;
    s16 u88;
    u8 pad8A[0x2];
    f32 u8C;
    u8 * u90;
    s32 u94;
    s8 u98;
    u8 pad99[0x3];
    f32 u9C;
    f32 uA0;
    f32 uA4;
    s16 uA8;
    s16 uAA;
    f32 uAC;
    f32 uB0;
    f32 uB4;
    f32 uB8;
    f32 uBC;
    f32 uC0;
    s16 uC4;
    s16 uC6;
    s16 uC8;
    u8 uCA;
    u8 padCB[0x1];
    f32 uCC;
    f32 uD0;
    f32 uD4;
    s16 uD8;
    s16 uDA;
    f32 uDC;
    f32 uE0;
    s16 uE4;
    s16 uE6;
    s16 uE8;
    s16 uEA;
    s16 uEC;
    s16 uEE;
    f32 uF0;
    u8 * uF4;
    s32 uF8;
    s8 uFC;
    u8 padFD[0x3];
    f32 u100;
    f32 u104;
    f32 u108;
    f32 u10C;
    f32 u110;
    u8 pad114[0x2];
    s16 u116;
    s16 u118;
    u8 pad11A[0x2];
} S150DBD70;
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    s16 u0C;
    u16 u0E;
    s32 u10;
    u8 pad14;
    u8 u15;
    u8 pad16[6];
} T150DBD70;

extern u8 D_800903C8[];
extern u8 D_80090414[];
void *memcpy(void *, const void *, u32);
void *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
s32 func_150DBD70(u8 arg0);

void *func_15167D84(void *, s32, s32, s8, u8, s32);

typedef struct Game10A350Emission {
    s32 texture;
    char pad4[8];
    s16 frame;
    s16 frame_rate;
    s16 position[3];
    s16 velocity_x;
    s16 velocity_z;
    char pad1A[2];
    u8 fraction_x;
    u8 fraction_y;
    u8 fraction_z;
    s8 callback;
    s16 velocity_y;
    s16 acceleration;
    s16 width;
    s16 height;
    s16 lifetime;
    s8 fade;
    char pad2B;
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    u8 environment[3];
    char pad33;
    u16 flags;
    char pad36[2];
} Game10A350Emission;

typedef struct Game10A350Record {
    f32 x;
    f32 y;
    f32 z;
    f32 speed;
    f32 width;
    s16 alpha;
    u8 color;
    u8 pad17;
} Game10A350Record;

typedef struct Game10A350State {
    f32 swayX;
    u8 angleX;
    u8 pad5;
    s16 angleXRange;
    f32 swayZ;
    u8 angleZ;
    u8 padD;
    s16 angleZRange;
    f32 x;
    f32 z;
    f32 gravity;
    f32 rate;
    f32 count;
    u8 pad24[5];
    u8 alpha;
    u8 pad2A[2];
    f32 widthBase;
    f32 widthRange;
    u8 widthAngle;
    u8 pad35;
    s16 widthAngleRange;
    f32 speed;
    u8 colorStep;
    u8 pad3D;
    s16 colorStepRange;
    u8 color;
    u8 pad41[0x23];
    f32 spawnRate;
    f32 spawnCount;
    f32 radius;
    s16 pitchBase;
    s16 pitchRange;
    f32 speedBase;
    f32 speedRange;
    s16 lifetime;
    s16 accelBase;
    f32 accelRange;
    s16 fadeBase;
    s16 fadeRange;
    s16 sizeBase;
    u8 pad8A[2];
    f32 sizeRange;
    s32 texture;
    s32 frameRate;
    s8 callback;
    u8 pad99[0x7F];
    s16 timer;
} Game10A350State;

typedef struct Game10A350Actor {
    u8 pad0;
    u8 field1;
    u8 pad2[0xA];
    u8 fieldC;
    u8 padD[3];
    f32 x;
    f32 y;
    f32 z;
    u8 pad1C[2];
    u16 flags;
    u8 pad20[5];
    u8 capacity;
    u8 pad26[6];
    s8 count;
    s8 head;
    s8 tail;
    u8 pad2F;
    s8 active;
    u8 pad31[0x63];
    Game10A350Record *records;
    Game10A350State *state;
} Game10A350Actor;
s32 func_150DCEA0(Game10A350Actor *arg0);

typedef struct {
    f32 x, y, z;
} Vec3f_150DD640;

typedef struct {
    Vec3f_150DD640 pos;
    u8 pad0C[4];
    f32 width;
    s16 alpha;
    u8 level;
    u8 pad17;
} Point150DD640;

typedef struct {
    u8 pad0[0x28];
    u8 unk28;
    u8 pad29[0x41 - 0x29];
    u8 unk41;
    u8 unk42;
    u8 pad43[0x60 - 0x43];
    u8 unk60;
} Style150DD640;

typedef struct {
    u8 pad0[0x10];
    Vec3f_150DD640 pos;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x84 - 0x2F];
    u8 unk84[0x10];
    Point150DD640 *points;
    Style150DD640 *style;
} Owner150DD640;

typedef struct {
    u8 pad0[6];
    u16 unk6;
    u8 pad8[4];
} Tex150DD640;

typedef struct {
    s32 unk0;
    s32 unk4;
} Mode150DD640;

extern Tex150DD640 D_80090B60[];
extern Mode150DD640 D_800A4AC8[];
extern s32 D_800D2C9C;
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
void *memcpy(void *, const void *, u32);
Gfx *func_150DD640(Owner150DD640 *arg0, Gfx *gfx, s16 arg2);

void func_150DDED0(void *arg0) {
    struct102 *temp_v0;
    s32 temp_v1;

    temp_v0 = (struct102 *)func_151491F4(0x64, 2, -1, 1, 0, 1, 0xFF, 0);
    if (temp_v0 != 0) {
        temp_v1 = ((u8 *)arg0)[0x72];
        switch (temp_v1) {
        case 0xFB:
            ((u8 *)temp_v0)[0x28] = 1;
            return;
        case 0xFC:
            ((u8 *)temp_v0)[0x28] = 2;
            return;
        case 0xFD:
            ((u8 *)temp_v0)[0x28] = 4;
            return;
        case 0xFE:
            ((u8 *)temp_v0)[0x28] = 3;
            return;
        default:
            func_1516972C(temp_v0);
            break;
        }
    }
}

s32 func_150DBD70(u8);

s32 func_150DDF88(u8 *arg0) {
    return func_150DBD70(arg0[0x28]);
}

s32 func_150DDFAC(EffectSpawnSource *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    EffectSpawnParams tmp;
    f32 scale;
    s32 rand2;
    s32 rand1;
    s32 rand0;

    tmp.field_0x2C = arg0->field_0x98;
    tmp.field_0x28 = arg0->field_0x94;
    scale = func_150ADA68() * 70.0f + 10.0f;
    tmp.field_0x2C->field_0x20 = 4;

    switch (func_150ADA20() & 1) {
    case 0:
        tmp.field_0x06 = 0x1D;
        break;
    case 1:
        tmp.field_0x06 = 0x1E;
        break;
    }

    tmp.field_0x08 = 0;
    tmp.field_0x0C = 0;
    tmp.field_0x00 = 0x11;
    tmp.field_0x04 = 0x64;
    tmp.field_0x10 = 0xFF;
    tmp.field_0x11 = 0xFF;
    tmp.field_0x12 = 0;
    tmp.field_0x13 = 0;
    tmp.field_0x14 = 0;
    tmp.field_0x15 = 0xFF;
    tmp.field_0x18 = 0x30002;

    rand0 = func_150ADA20();
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();

    func_1513C73C((s32)&tmp, 5, 0, arg5,
                  tmp.field_0x28[arg0->field_0x2D].field_0x00,
                  arg4,
                  tmp.field_0x28[arg0->field_0x2D].field_0x08,
                  scale, scale,
                  rand0 & 0xFF, (rand2 & 1) + (rand1 & 1), 0,
                  arg0->field_0x0C, arg0->field_0x01);
    return 1;
}

s32 func_150DE12C(EffectSpawnSource *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    EffectSpawnParams tmp;
    f32 scale;
    s32 rand2;
    s32 rand1;
    s32 rand0;

    tmp.field_0x2C = arg0->field_0x98;
    tmp.field_0x28 = arg0->field_0x94;
    scale = func_150ADA68() * 10.0f + 10.0f;
    tmp.field_0x2C->field_0x20 = 4;

    switch (func_150ADA20() & 1) {
    case 0:
        tmp.field_0x06 = 0x1B;
        break;
    case 1:
        tmp.field_0x06 = 0x1C;
        break;
    }

    tmp.field_0x08 = 0;
    tmp.field_0x0C = 0;
    tmp.field_0x00 = 0x11;
    tmp.field_0x04 = 0x64;
    tmp.field_0x10 = 0xFF;
    tmp.field_0x11 = 0xFF;
    tmp.field_0x12 = 0;
    tmp.field_0x13 = 0;
    tmp.field_0x14 = 0;
    tmp.field_0x15 = 0xAA;
    tmp.field_0x18 = 0x30002;

    rand0 = func_150ADA20();
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();

    func_1513C73C((s32)&tmp, 5, 0, arg5,
                  tmp.field_0x28[arg0->field_0x2D].field_0x00,
                  arg4,
                  tmp.field_0x28[arg0->field_0x2D].field_0x08,
                  scale, scale,
                  rand0 & 0xFF, (rand2 & 1) + (rand1 & 1), 0,
                  arg0->field_0x0C, arg0->field_0x01);
    return 1;
}

s32 func_150DE2A4(void *arg0) {
    s16 temp = *(s16 *)((u8 *)arg0 + 0x1C);
    if (temp < 0x20) {
        *(s8 *)((u8 *)arg0 + 0x28) = temp << 3;
    }
    return 1;
}

void func_150DE2C4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x34) = *(s16 *)((u8 *)arg0 + 0x34) - 0x1A;
    *(s16 *)((u8 *)arg0 + 0x36) = *(s16 *)((u8 *)arg0 + 0x36) - 0x1A;
    if (*(s16 *)((u8 *)arg0 + 0x34) >= 0xA && *(s16 *)((u8 *)arg0 + 0x36) >= 0xA) {
        return;
    }
    *(s16 *)((u8 *)arg0 + 0x38) = 0;
}
