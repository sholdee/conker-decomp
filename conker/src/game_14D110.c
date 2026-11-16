#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A3408;
extern f32 D_800A340C;
extern f32 D_800A3410;
extern f32 D_800A3414;
extern f32 D_800A3418;

typedef struct {
    u8 pad0[0x20];
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
    u8 unk4C;
    u8 unk4D;
    s8 unk4E;
    u8 pad4F;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    u8 pad5C[0x8];
    s32 unk64;
} CamSub740;

void func_151C9BA0(struct108 *arg0, s32 arg1);
void func_151216F8(struct108 *arg0);

#define FLD(type, base, off) (*(type *)((u8 *)(base) + (off)))

void func_1511FC60(struct108 *arg0) {
    CamSub740 *s;
    s32 state;

    s = (CamSub740 *)((u8 *)arg0 + 0x740);
    state = arg0->unk23E;
    FLD(s16, arg0, 0x84A) = 0x3C;
    if ((arg0->unk5F0 & 2) || (arg0->unk23E == 9) || (arg0->unk23E == 0x38) || (arg0->unk23E == 0x39) || (arg0->unk23E == 0x37) || (arg0->unk23E == 0x3B)) {
        *(volatile u8 *)&s->unk4D = 0;
        s->unk4D = 1;
    } else {
        s->unk4D = 1;
    }
    FLD(u8, arg0, 0x84C) = 0;
    if (FLD(s32, arg0, 0x34) == 0x40) {
        s->unk34 = arg0->unk37C;
    } else if (s->unk4D) {
        if ((arg0->unk23E == 9) || (arg0->unk23E == 0x38) || (arg0->unk23E == 0x39) || (arg0->unk23E == 0x37) ||
            (arg0->unk23E == 0x3B) || (arg0->unk23E == 0x15) || (arg0->unk23E == 0x26) || (arg0->unk23E == 0x3A)) {
            s->unk34 = (f32)((arg0->unk3D0->unk7A - FLD(s16, arg0->unk3D4, 0x12)) + 0xC000) * 0.005493164f;
        } else if ((D_800D1940 == 0x42) && (state == 0x1A)) {
            s->unk34 = arg0->unk3D0->unk40;
        } else {
            s->unk34 = arg0->unk3D0->unk40 - 180.0f;
        }
    } else {
        s->unk34 = arg0->unk37C;
        arg0->unk84 &= ~0x280;
    }
    if ((state == 9) || (state == 0x38) || (state == 0x39) || (state == 0x37) || (state == 0x3B) ||
        (state == 0x15) || (state == 0x26) || (state == 0x3A)) {
        s->unk38 = 0.0f;
        s->unk3C = 0.0f;
        func_151C9BA0(arg0, state);
        FLD(u8, arg0, 0x84C) = 1;
    } else if ((state == 2) || (state == 0x13)) {
        s->unk38 = 0.0f;
        s->unk3C = 0.0f;
    } else if (state == 0x1C) {
    } else if (state == 0x34) {
        s->unk38 = 0.0f;
        s->unk3C = 0.0f;
    } else if (state == 0xF) {
        s->unk38 = 27.0f;
        s->unk3C = 27.0f;
        s->unk34 = arg0->unk3D0->unk40 + 225.0f;
    } else {
        s->unk38 = 0.0f;
        s->unk3C = 0.0f;
    }
    func_151216F8(arg0);
    func_15048758(&s->unk34);
    if (s->unk4C) {
        s->unk38 = s->unk50;
        s->unk3C = s->unk50;
        s->unk44 = s->unk58;
        s->unk34 += s->unk54;
    }
    s->unk20 = arg0->unk37C;
    s->unk2C = arg0->unk134;
    if (FLD(u8, arg0, 0x92C)) {
        s->unk24 = sqrtf((arg0->unk2BC - arg0->unk2F8) * (arg0->unk2BC - arg0->unk2F8) + (arg0->unk2C4 - arg0->unk300) * (arg0->unk2C4 - arg0->unk300));
        s->unk28 = FLD(f32, arg0, 0x344);
        s->unk30 = arg0->unk354;
    } else {
        s->unk24 = arg0->unk374;
        s->unk28 = arg0->unk348;
        s->unk30 = arg0->unk354;
    }
    arg0->unk35C = arg0->unk2C0;
    arg0->unk354 = arg0->unk2C0;
    s->unk44 = 110.0f;
    arg0->unk348 = 30.0f;
    FLD(f32, arg0->unk3D4, 0x16C) = s->unk34;
    if ((state == 0) || (state == 0xC) || (state == 9) || (state == 0x38) || (state == 0x39) || (state == 0x37) ||
        (state == 0x3B) || (state == 0x15) || (state == 0x26) || (state == 0x3A)) {
        arg0->unk348 += 30.0f;
    }
    s->unk64 = 0;
    arg0->unk73C = 1;
    if (arg0->unk23C) {
        s->unk34 = arg0->unk3D0->unk40 - 180.0f;
        arg0->unk37C = s->unk34;
        arg0->unk39C = arg0->unk37C * 0.01745329238f;
        arg0->unk194 = arg0->unk198;
        arg0->unk18C = arg0->unk190;
    }
    if (s->unk44 < FLD(f32, arg0, 0x370)) {
        s->unk48 = s->unk44 * 1.200000048f;
        s->unk4E = -1;
    } else {
        s->unk48 = s->unk44 * 0.6666659713f;
        s->unk4E = 1;
    }
    if ((state == 0xA) || (state == 2) || (state == 0x12) || (state == 3) || (state == 0x34)) {
        s->unk48 = arg0->unk374;
        s->unk4E = -1;
    }
    arg0->unk84 &= ~0x280;
    arg0->unk374 = s->unk44;
}

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
    f32 unk44;
    f32 unk48;
    u8 unk4C;
    u8 unk4D;
    s8 unk4E;
    u8 pad4F;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    s32 unk64;
} Cam740;

void func_1510E7A4(s32, s32, f32 *, void *, s32, s32, f32, f32, f32, f32, s32, s32, f32, f32);
void func_150495B0(f32 *, f32, f32 *, f32, f32, f32);
void func_15123A54(struct108 *);
void func_151C9ED4(struct108 *);
void func_151218C4(struct108 *arg0);
void func_1512E140(struct108 *);

void func_15120158(struct108 *arg0) {
    Cam740 *s;
    f32 sp_out;
    f32 dist;
    s32 state;
    s32 flag;
    f32 d;

    s = (Cam740 *)((u8 *)arg0 + 0x740);
    state = arg0->unk23E;
    flag = (D_800D1940 == 0x42) && (state == 0x1A);
    func_1510E7A4(0, 0, &sp_out, (u8 *)arg0 + 0x360, 0, 0, arg0->unk2F8, arg0->unk2FC, arg0->unk300, arg0->unk2FC, 0, 0, -10000.0f, arg0->unk2FC);
    arg0->unk3D4->unk198 = arg0->unk73C;
    FLD(f32, arg0, 0x370) = sqrtf((arg0->unk2F8 - arg0->unk2BC) * (arg0->unk2F8 - arg0->unk2BC) + (arg0->unk300 - arg0->unk2C4) * (arg0->unk300 - arg0->unk2C4));
    if (arg0->unk23C) {
        s->unk30 = sp_out;
    }
    if ((state != 0x2A) && (arg0->unk73C != 3)) {
        if (state == 0xD) {
            arg0->unk190 = 100.0f;
        } else if (state == 3) {
            s->unk38 = 15.0f;
            arg0->unk198 = 0.0f;
            s->unk44 = 350.0f;
            if (FLD(u8, arg0, 0x600)) {
                s->unk3C *= FLD(f32, arg0, 0x604) * 0.002857142827f;
                s->unk3C = (s->unk3C < 0.0f) ? 0.0f : s->unk3C;
            }
        } else if (flag) {
            arg0->unk190 = 18.0f;
            arg0->unk198 = 60.0f;
            s->unk44 = 250.0f;
            s->unk34 = arg0->unk3D0->unk40;
        } else if (state == 0x1A) {
            arg0->unk190 = 35.0f;
            arg0->unk198 = 100.0f;
            s->unk44 = 250.0f;
        } else {
            arg0->unk190 = 20.0f;
            arg0->unk198 = 0.0f;
            func_151216F8(arg0);
        }
    }
    arg0->unk35C = arg0->unk354 = arg0->unk2C0;
    func_1506160C(arg0->unk3D0, 2, 0xFF, 0x20, arg0->unk23D);
    dist = s->unk44;
    if (arg0->unk73C == 1) {
        s32 cx;
        f32 sn;
        s32 v;

        if (FLD(s16, arg0, 0x84A) != 0) {
            FLD(s16, arg0, 0x84A) -= D_800BE9E4;
            if (FLD(s16, arg0, 0x84A) <= 0) {
                arg0->unk3D4->unk198 = 1;
                arg0->unk73C = 2;
                s->unk34 = arg0->unk37C;
                return;
            }
        }
        arg0->unk134 = 1;
        cx = cosf(s->unk3C * 0.01745329238f) * dist;
        sn = sinf(s->unk3C * 0.01745329238f);
        arg0->unk374 = cx;
        arg0->unk348 = (s32)(sn * dist);
        if (arg0->unk23C) {
            if (flag) {
                s->unk34 = arg0->unk3D0->unk40;
            } else {
                s->unk34 = arg0->unk3D0->unk40 - 180.0f;
            }
            arg0->unk37C = s->unk34;
            arg0->unk39C = arg0->unk37C * 0.01745329238f;
            arg0->unk194 = arg0->unk198;
            arg0->unk18C = arg0->unk190;
        }
        do {
        if (s->unk4D) {
            if (fabsf(func_15048A70(arg0->unk37C, s->unk34)) < 0.009999999776f) {
                
            }
        } else {
            arg0->unk3D4->unk198 = 1;
            arg0->unk73C = 2;
            break;
        }
            if (s->unk4E == -1) {
                v = FLD(f32, arg0, 0x370) < s->unk48;
            } else {
                v = s->unk48 < FLD(f32, arg0, 0x370);
            }
            if (v || (s->unk4C & 0xFF)) {
                if (FLD(u8, arg0, 0x600)) {
                    func_15049688(&arg0->unk37C, s->unk34, &s->unk18, 10.0f, 18.0f, FLD(f32, arg0, 0x7B4));
                } else {
                    func_15049688(&arg0->unk37C, s->unk34, &s->unk18, 5.0f, 9.0f, FLD(f32, arg0, 0x7B4));
                }
                func_150495B0(&arg0->unk194, arg0->unk198, (f32 *)((u8 *)arg0 + 0x7D0), 5.0f, 9.0f, FLD(f32, arg0, 0x7B4));
                func_150495B0(&arg0->unk18C, arg0->unk190, (f32 *)((u8 *)arg0 + 0x7D4), 5.0f, 9.0f, FLD(f32, arg0, 0x7B4));
                arg0->unk39C = arg0->unk37C * 0.01745329238f;
            }
        } while (0);
    } else if (arg0->unk73C == 2) {
        f32 cy;
        f32 sn;
        f32 rate;
        s32 pad3;
        s32 c;
        s8 *p;

        if (state) {
            rate = 0.01279999968f;
        } else {
            rate = 0.02500000037f;
        }
        if (arg0->unk23C) {
            if (flag) {
                s->unk34 = arg0->unk3D0->unk40;
            } else {
                s->unk34 = arg0->unk3D0->unk40 - 180.0f;
            }
            arg0->unk37C = s->unk34;
            arg0->unk39C = arg0->unk37C * 0.01745329238f;
            arg0->unk194 = arg0->unk198;
            arg0->unk18C = arg0->unk190;
        }
        if ((state == 9) || (state == 0x38) || (state == 0x39) || (state == 0x37) || (state == 0x3B) || (state == 0x15) || (state == 0x26) || (state == 0x3A)) {
            if (!FLD(u8, arg0, 0x84C)) {
                func_151C9BA0(arg0, state);
                FLD(u8, arg0, 0x84C) = 1;
            }
        } else if (FLD(u8, arg0, 0x84C)) {
            func_151C9ED4(arg0);
            FLD(u8, arg0, 0x84C) = 0;
        }
        if (state != 0xD) {
            pad3 = !arg0->unk3E8;
            if (!s->unk4D) {
                if (!pad3) {
                    s->unk64 = 0;
                } else {
                    if (s->unk64 == 0) {
                        c = ((s8 *)arg0->unk36C)[2];
                        s->unk64 |= (c > 0) ? 1 : ((c < 0) ? 2 : 0);
                    }
                    *(vertex *)&arg0->unk2F8 = *(vertex *)&arg0->unk304;
                    if (s->unk64 & 1) {
                        p = (s8 *)arg0->unk36C;
                        c = p[2];
                        if (c > 0) {
                            p[2] = 0;
                        } else {
                            p[2] = c;
                        }
                    } else if (s->unk64 & 2) {
                        p = (s8 *)arg0->unk36C;
                        c = p[2];
                        if (c < 0) {
                            p[2] = 0;
                        } else {
                            p[2] = c;
                        }
                    }
                }
            }
            if ((state == 9) || (state == 0x38) || (state == 0x39) || (state == 0x37) || (state == 0x3B) || (state == 0x15) || (state == 0x26) || (state == 0x3A)) {
                s->unk34 = (f32)((arg0->unk3D0->unk7A - FLD(s16, arg0->unk3D4, 0x12)) + 0xC000) * 0.005493164f;
            } else if (flag) {
                s->unk34 = arg0->unk3D0->unk40;
            } else if (state != 0xF) {
                if (arg0->unk5F0 & 2) {
                    s->unk34 = (((f32)(u32)arg0->unk3D0->unk7A - (f32)FLD(s16, arg0->unk3D4, 0x12) * 0.349999994f) + 49152.0f) * 0.005493164f;
                } else {
                    if (state == 0x34) {
                        s->unk34 = s->unk34 - (f32)((s8 *)arg0->unk36C)[2] * rate * (f32)D_800BE9E4;
                    }
                    s->unk34 = s->unk34 - (f32)((s8 *)arg0->unk36C)[2] * rate * (f32)D_800BE9E4;
                }
            }
            if ((state != 3) && (state != 0x1A) && !flag && (state != 0xF)) {
                s->unk38 += (f32)((s8 *)arg0->unk36C)[3] * rate * (f32)D_800BE9E4;
            }
            if (state == 0x38) {
                if (*arg0->unk36C & 0x2000) {
                    sn = FLD(f32, arg0, 0x8B4) - (f32)(FLD(u16, arg0->unk3D0->unk31C, 0x1A2) >> 8) * 1.098632812f;
                } else {
                    sn = 0.0f;
                }
                s->unk38 += sn;
                FLD(f32, arg0, 0x8B4) = (f32)(FLD(u16, arg0->unk3D0->unk31C, 0x1A2) >> 8) * 1.098632812f;
            }
            func_15048758(&s->unk34);
            if (state == 0x34) {
                s->unk38 = (s->unk38 < -5.0f) ? -5.0f : ((s->unk38 > 15.0f) ? 15.0f : s->unk38);
            } else if (state == 0x12) {
                s->unk38 = (s->unk38 < -40.0f) ? -40.0f : ((s->unk38 > 15.0f) ? 15.0f : s->unk38);
            } else if ((state == 2) || (state == 0xA) || (state == 0x13)) {
                s->unk38 = (s->unk38 < -37.0f) ? -37.0f : ((s->unk38 > 30.0f) ? 30.0f : s->unk38);
            } else {
                s->unk38 = (s->unk38 < -45.0f) ? -45.0f : ((s->unk38 > 80.0f) ? 80.0f : s->unk38);
            }
            if ((D_800BE616 == 0) && ((state == 9) || (state == 0x38) || (state == 0x39) || (state == 0x37) || (state == 0x3B) || (state == 0x15) || (state == 0x26) || (state == 0x3A))) {
                s->unk38 = (s->unk38 < -20.0f) ? -20.0f : s->unk38;
            }
            if (arg0->unk23C) {
                arg0->unk37C = s->unk34;
                s->unk3C = s->unk38;
                arg0->unk194 = arg0->unk198;
                arg0->unk18C = arg0->unk190;
            } else if (state || flag) {
                func_15049688(&arg0->unk37C, s->unk34, &s->unk18, 3.91f, 8.210000038f, FLD(f32, arg0, 0x7B4));
                func_15049688(&s->unk3C, s->unk38, &s->unk1C, 3.91f, 8.210000038f, FLD(f32, arg0, 0x7B4));
                func_150495B0(&arg0->unk194, arg0->unk198, (f32 *)((u8 *)arg0 + 0x7D0), 3.91f, 8.210000038f, FLD(f32, arg0, 0x7B4));
                func_150495B0(&arg0->unk18C, arg0->unk190, (f32 *)((u8 *)arg0 + 0x7D4), 3.91f, 8.210000038f, FLD(f32, arg0, 0x7B4));
            } else {
                func_15049688(&arg0->unk37C, s->unk34, &s->unk18, 4.3f, 7.300000191f, FLD(f32, arg0, 0x7B4));
                func_15049688(&s->unk3C, s->unk38, &s->unk1C, 4.3f, 7.300000191f, FLD(f32, arg0, 0x7B4));
            }
        }
        arg0->unk3D4->unk16C = arg0->unk37C;
        arg0->unk3D4->unk170 = s->unk3C;
        *(vertex *)&arg0->unk3D4->unk148 = *(vertex *)&arg0->unk2BC;
        *(vertex *)((u8 *)arg0->unk3D4 + 0x13C) = *(vertex *)&arg0->unk2F8;
        while (arg0->unk3D4->unk170 > 180.0f) {
            arg0->unk3D4->unk170 -= 360.0f;
        }
        while (arg0->unk3D4->unk170 < -180.0f) {
            arg0->unk3D4->unk170 += 360.0f;
        }
        cy = cosf(s->unk3C * 0.01745329238f) * dist;
        sn = sinf(s->unk3C * 0.01745329238f);
        arg0->unk374 = cy;
        arg0->unk348 = sn * dist;
        s->unk0 = sinf(arg0->unk37C * 0.01745329238f + 3.141592741f) * dist + arg0->unk2BC;
        s->unk4 = sinf(s->unk3C * 0.01745329238f + 3.141592741f) * dist + arg0->unk2C0;
        s->unk8 = cosf(arg0->unk37C * 0.01745329238f + 3.141592741f) * dist + arg0->unk2C4;
        if (s->unk3C >= 180.0f) {
            s->unk40 = s->unk3C - 360.0f;
        } else {
            s->unk40 = s->unk3C;
        }
        func_151218C4(arg0);
    } else if (arg0->unk73C == 3) {
        d = fabsf(func_15048A70(arg0->unk37C, s->unk20));
        arg0->unk134 = FLD(s32, arg0, 0x13C);
        if (FLD(s16, arg0, 0x84A) != 0) {
            FLD(s16, arg0, 0x84A) -= D_800BE9E4;
            if (FLD(s16, arg0, 0x84A) <= 0) {
                arg0->unk3D4->unk198 = 0;
                arg0->unk73C = 0;
                return;
            }
        }
        if ((arg0->unk84 & 0x200000) || (arg0->unk23E == 9) || (arg0->unk23E == 0x38) || (arg0->unk23E == 0x39) || (arg0->unk23E == 0x37) || (arg0->unk23E == 0x3B) || (arg0->unk23E == 0x15) || (arg0->unk23E == 0x26) || (arg0->unk23E == 0x3A)) {
            s->unk4D = 0;
        }
        if (!s->unk4D || (d < 45.0f)) {
            arg0->unk374 = s->unk24;
            arg0->unk348 = s->unk28;
            if ((arg0->unk5F0 & 0x40) || (FLD(f32, arg0, 0x360) != 0.0f)) {
                arg0->unk35C = s->unk30;
                arg0->unk354 = s->unk30;
            } else {
                arg0->unk35C = arg0->unk3D0->unk180;
                arg0->unk354 = arg0->unk3D0->unk180;
            }
        }
        if (!s->unk4D || (d < 0.05000000075f)) {
            arg0->unk3D4->unk198 = 0;
            arg0->unk73C = 0;
        } else {
            func_15049688(&arg0->unk37C, s->unk20, &s->unk18, 5.0f, 9.0f, FLD(f32, arg0, 0x7B4));
        }
    }
    arg0->unk39C = arg0->unk37C * 0.01745329238f;
    if (((state == 2) || (state == 0x13) || (state == 0x12) || (state == 0xA) || (state == 0x34)) && (state != 0xD)) {
        f32 cz;
        f32 pad4;
        f32 sx;
        f32 k;

        if (arg0->unk5F0 & 2) {
            k = s->unk34 * -3.0f;
        } else {
            k = -35.0f;
        }
        sx = sinf(arg0->unk39C - 1.570796371f) * k;
        cz = cosf(arg0->unk39C - 1.570796371f) * k;
        if (arg0->unk23C) {
            s->unkC = sx;
            s->unk14 = cz;
            s->unk5C = 0.0f;
        } else {
            func_150495B0(&s->unkC, sx, &s->unk5C, 4.0f, 9.0f, FLD(f32, arg0, 0x7B4));
            func_150495B0(&s->unk14, cz, &s->unk60, 4.0f, 9.0f, FLD(f32, arg0, 0x7B4));
        }
    } else {
        func_150495B0(&s->unkC, 0.0f, &s->unk5C, 4.0f, 9.0f, FLD(f32, arg0, 0x7B4));
        func_150495B0(&s->unk14, 0.0f, &s->unk60, 4.0f, 9.0f, FLD(f32, arg0, 0x7B4));
    }
    arg0->unk3D4->unk148 += s->unkC;
    arg0->unk3D4->y_scale += s->unk14;
    FLD(f32, arg0->unk3D4, 0x13C) += s->unkC;
    FLD(f32, arg0->unk3D4, 0x144) += s->unk14;
    func_150491EC((struct17 *)((u8 *)arg0->unk3D4 + 0x13C), (struct17 *)&arg0->unk3D4->unk148, (struct17 *)((u8 *)arg0->unk3D4 + 0x130));
    func_15123A54(arg0);
    func_1512E140(arg0);
    func_15125594(arg0);
}

void func_15121490(struct127 *arg0, u16 *arg1) {
    struct126 *stats;
    s32 state;
    vertex endPoint;
    vertex origin;
    f32 angle;
    vertex dir;
    f32 temp;

    arg0->unk31C->unk198 = 0;
    if (*arg1 & 0x10) {
        state = arg0->unk31C->unk78;
        if ((state == 0x9) || (state == 0x38) || (state == 0x39) || (state == 0x37) ||
            (state == 0x3B) || (state == 0x15) || (state == 0x26) || (state == 0x3A)) {
            stats = arg0->unk31C;
            angle = (arg0->unk40 - 90.0f) * D_800A3408;
            endPoint.x = arg0->x_position;
            endPoint.y = arg0->y_position + ((f32)(s16)stats->unk114 * 0.75f);
            endPoint.z = arg0->z_position;
            origin = endPoint;
            endPoint.x = (sinf(angle) * 100.0f) + origin.x;
            endPoint.y = origin.y;
            endPoint.z = (cosf(angle) * 100.0f) + origin.z;
            origin.y += arg0->y_position - arg0->old_y_position;
            *(vertex *)((u8 *)stats + 0x13C) = origin;
            *(vertex *)((u8 *)stats + 0x148) = endPoint;
            stats->unk198 = 1;
            dir.x = 1.0f;
            temp = dir.x * cosf(*(f32 *)((u8 *)stats + 0x170) * D_800A340C);
            dir.y = dir.x * sinf(*(f32 *)((u8 *)stats + 0x170) * D_800A3410);
            dir.x = temp;
            temp = dir.x * cosf(*(f32 *)((u8 *)stats + 0x16C) * D_800A3414);
            dir.z = dir.x * sinf(*(f32 *)((u8 *)stats + 0x16C) * D_800A3418);
            dir.x = temp;
            *(vertex *)((u8 *)stats + 0x130) = dir;
        }
    }
}

void func_151216F8(struct108 *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->unk23E;
    if (temp_v0) {
        if (temp_v0 == 0x34) {
            arg0->unk190 = 80.0f;
            *(f32 *)((u8 *)arg0 + 0x784) = 350.0f;
            return;
        }
        if ((temp_v0 == 0x3B) && ((D_800BE9F0 == 0x41) || (D_800BE9F0 == 0x3C))) {
            arg0->unk190 = -12.0f;
            *(f32 *)((u8 *)arg0 + 0x99C) = -28.0f;
            *(f32 *)((u8 *)arg0 + 0x784) = 106.0f;
            return;
        }
        if ((temp_v0 == 2) || (temp_v0 == 0xA) || (temp_v0 == 0x13)) {
            arg0->unk190 = 10.0f;
            arg0->unk84 |= 0x80;
            *(f32 *)((u8 *)arg0 + 0x784) = 135.0f;
            *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
            return;
        }
        if ((temp_v0 == 0x12) && (D_800BE9F0 == 0x3C)) {
            arg0->unk190 = 10.0f;
            *(f32 *)((u8 *)arg0 + 0x784) = 180.0f;
            *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
            return;
        }
        if (temp_v0 == 0xF) {
            arg0->unk190 = 10.0f;
            *(f32 *)((u8 *)arg0 + 0x784) = 480.0f;
            *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
            return;
        }
        if (temp_v0 == 0x38) {
            *(f32 *)((u8 *)arg0 + 0x784) = 110.0f;
            arg0->unk190 = 5.0f;
            *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
            return;
        }
        if (temp_v0 == 0x15) {
            *(f32 *)((u8 *)arg0 + 0x784) = 110.0f;
            arg0->unk190 = 25.0f;
            *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
            return;
        }
        arg0->unk190 = 5.0f;
        *(f32 *)((u8 *)arg0 + 0x784) = 75.0f;
        *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
        return;
    }
    *(f32 *)((u8 *)arg0 + 0x784) = 90.0f;
    arg0->unk190 = 25.0f;
    *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
}

typedef struct {
    u8 pad0[0x2C];
    f32 unk2C;
    u8 pad30[0x1C];
    u8 unk4C;
    u8 unk4D;
} Sub740; /* sub-block at actor+0x740 (func_1511FC60 uses +0x34/+0x4D too) */

void func_151C9ED4(struct108 *);

void func_151218C4(struct108 *arg0) {
    Sub740 *s = (Sub740 *)((u8 *)arg0 + 0x740);

    if (*arg0->unk36C & 0x10) {
        return;
    }
    if (*(s32 *)((u8 *)arg0 + 0x34) == 0x40) {
        if (arg0->unk6FC == 0xF) {
            *(f32 *)((u8 *)arg0 + 0x898) = 0.0f;
            *(f32 *)((u8 *)arg0 + 0x89C) = 0.0f;
        }
        arg0->unk3D4->unk198 = 0;
        arg0->unk73C = 0;
        *(s16 *)((u8 *)arg0 + 0x84A) = 0x3C;
    } else {
        *(s16 *)((u8 *)arg0 + 0x84A) = 0x3C;
        arg0->unk73C = 3;
    }
    arg0->unk374 = 200.0f;
    arg0->unk348 = 100.0f;
    arg0->unk134 = s->unk2C;
    s->unk4C = 0;
    arg0->unk190 = 0.0f;
    *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
    arg0->unk198 = 0.0f;
    func_1506160C(arg0->unk3D0, 2, 0xFF, 0x20, arg0->unk23D);
    if (*((u8 *)arg0 + 0x84C) != 0) {
        func_151C9ED4(arg0);
        *((u8 *)arg0 + 0x84C) = 0;
    }
}
