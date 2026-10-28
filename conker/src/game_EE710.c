#include <ultra64.h>
#include "functions.h"
#include "variables.h"


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
    s32 unk50;
    s16 unk54;
    s16 unk56;
    u8 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 pad69;
    u8 unk6A;
    void *unk6C;
    u8 unk70;
    s16 unk72;
    s16 unk74;
    s32 pad78;
} S150C1260Particle;

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x14C - 0x3C];
    f32 unk14C;
    f32 unk150;
    u8 pad154[0x180 - 0x154];
    f32 unk180;
    u8 pad184[0x1D4 - 0x184];
    u32 unk1D4;
} S150C1260;

extern void func_15142314(s32, s32, f32 *);
extern void func_1514C2F0(f32 arg0, f32 arg1, f32 arg2, f32 arg3,
                          u8 arg4, s8 arg5, s16 arg6, u8 arg7,
                          s32 arg8, f32 arg9, s32 arg10, u8 arg11);
extern void func_15165F80(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);

void func_150C1260(S150C1260 *arg0, u8 arg1) {
    struct { f32 x, y, z; } pos;
    s32 type;
    f32 half;
    S150C1260Particle p;
    f32 radius;
    f32 speed;
    f32 c;
    f32 s;
    f32 scale;
    u32 n;
    u8 angle;
    void *v;

    half = (arg0->unk14C + arg0->unk150) * 0.5f;
    if (arg0 != NULL) {
        if (arg0->unk1D4 != 0) {
            switch (arg1) {
            case 0:
                type = 0xD;
                break;
            case 1:
                type = 0x11;
                break;
            }
            func_15142314(arg0->unk1D4, type, (f32 *)&pos);
            radius = half * 80.0f;
            func_1514C2F0(pos.x, arg0->unk180, pos.z, radius, 0, 0x10, 0x10, 0, 0, radius * 4.0f, 0, 0xFF);
            func_15165F80(-1, pos.x, (f32)(s32)arg0->unk180 + 2.0f, pos.z, 0xB, 0x13, 0, 0xFF, 0);
            p.unk56 = 5;
            p.unk50 = 0x29E8;
            p.unk58 = 0;
            p.unk5C = 0;
            p.unk60 = 0xFF;
            p.unk61 = 1;
            p.unk62 = 0;
            p.unk63 = 0;
            p.unk64 = 0;
            p.unk65 = 0;
            p.unk66 = 0;
            p.unk67 = 0;
            p.unk68 = 0;
            p.unk6A = 1;
            p.unk6C = arg0;
            p.unk1C = 1.0f;
            p.unk20 = 1.0f;
            p.unk24 = 1.0f;
            p.unk04 = 0.400000006f;
            p.unk48 = 0.0f;
            p.unk4C = -0.8999999762f;
            p.unk70 = arg0->unk3B;
            p.unk72 = 0x10;
            p.unk74 = 0xF;
            n = (func_150ADA20() & 7) + 5;
            while (n--) {
                angle = func_150ADA20();
                angle &= 0xFF;
                speed = (func_150ADA68() * 0.8999999762f + 0.1000000015f) * 18.0f * half;
                c = func_151423D8(angle - 0x40);
                s = func_151423D8(angle);
                p.unk54 = (func_150ADA20() & 0x1F) + 0xF;
                p.unk28 = (radius * c) + pos.x;
                p.unk2C = arg0->unk180 + 2.0f;
                p.unk38 = speed;
                p.unk30 = (radius * s) + pos.z;
                p.unk34 = speed * c;
                p.unk3C = speed * s;
                scale = (func_150ADA68() * 0.8999999762f + 0.1000000015f) * half * 2.230000019f;
                p.unk08 = scale;
                p.unk0C = scale;
                p.unk00 = scale;
                p.unk10 = func_150ADA68() * 360.0f;
                p.unk14 = func_150ADA68() * 360.0f;
                p.unk18 = func_150ADA68() * 360.0f;
                p.unk40 = 25.0f - func_150ADA68() * 50.0f;
                p.unk44 = 25.0f - func_150ADA68() * 50.0f;
                v = func_15132A4C(&p, 3, 0xFF, 4, 0xFF, 0);
                if (v != NULL) {
                    *(f32 *)((u8 *)v + 0x170) = arg0->unk180;
                }
            }
        }
    }
}
