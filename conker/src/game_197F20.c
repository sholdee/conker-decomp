#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    u8 *unk0;
    s32 unk4;
} struct_game_197C20_0;

typedef struct {
    s32 unk0;
    s32 unk4;
} struct_game_197C20_1;

typedef struct {
    s32 field_0x0;
    char pad_0[0x4];
    u8 field_0x8;
} struct_game_197C20_2;

typedef struct {
    u8 field_0x0;
    char pad_0[0xF];
    struct_game_197C20_2 *field_0x10;
    char pad_1[0x1];
    u8 field_0x15;
    char pad_2[0xA];
    u8 field_0x20;
} struct_game_197C20_3;

extern struct_game_197C20_0 *D_800DD254;
extern struct_game_197C20_0 *D_800DD258;
s32 func_1516A770(u8 *arg0);

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    char padB[0x26 - 0xB];
    u16 unk26;
    char pad28[4];
    s32 unk2C;
} Struct1516A7B0_Data;

typedef struct {
    char pad0[0xD];
    u8 unkD;
    u8 unkE;
    u8 unkF;
    Struct1516A7B0_Data *unk10;
    u8 unk14;
    u8 unk15;
    u16 unk16;
    u16 unk18;
    u8 unk1A;
    char pad1B[4];
    u8 unk1F;
    u8 unk20;
    char pad21[3];
    u8 *unk24;
    f32 unk28;
    char pad2C[0xC];
    f32 unk38;
    f32 unk3C;
} Struct1516A7B0;

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_150428D4(u8 *, s32 *, s32 *, s32 *);
u8 *func_1516C878(struct_game_197C20_1 *arg0, s32 arg1, s16 arg2);
Struct1516A7B0 *func_1516A7B0(u32 arg0, s32 arg1, s32 arg2, s32 arg3, Struct1516A7B0_Data *arg4, s32 arg5);

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
    char padA[0x26 - 0xA];
    u16 unk26;
    char pad28[3];
    u8 unk2B;
    void (*unk2C)(void *, s32, s32);
} Struct1516AA70_Data;

typedef struct {
    char pad0[0xD];
    u8 unkD;
    u8 unkE;
    u8 unkF;
    Struct1516AA70_Data *unk10;
    u8 unk14;
    u8 unk15;
    s16 unk16;
    s16 unk18;
    u8 unk1A;
    u8 unk1B[3];
    u8 unk1E;
    u8 unk1F;
    s8 unk20;
    char pad21[3];
    u8 *unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    char pad38[8];
    f32 unk40;
    f32 unk44;
    char pad48[0x14];
    f32 unk5C;
    f32 unk60;
    char pad64[0xC];
    f32 unk70;
} Struct1516AA70;

extern u8 D_800CC4B3[][0x32C];
s32 func_151F2CDC(void);
void func_1516CC58();

void func_1516AA70(Struct1516AA70 *arg0) {
    Struct1516AA70_Data *data;
    struct108 *sp48;
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    s32 flag;
    s32 done;

    sp48 = D_800DBFF0;
    flag = arg0->unk10->unk0 & 0x400;
    if (arg0->unk10->unk2B != 0 && D_800C35EA != 1) {
        arg0->unk10->unk8 = 0;
        func_1516972C((struct102 *)arg0);
        return;
    }
    arg0->unk40 = arg0->unk5C;
    arg0->unk44 = arg0->unk60;
    switch (arg0->unk15) {
    case 0x1:
        arg0->unk16 = 0;
        arg0->unk18 = 0;
        if (flag) {
            arg0->unk2C = 1.0f;
        } else {
            arg0->unk2C = 0.3333333433f;
        }
        if (arg0->unk10->unk0 & 2) {
            arg0->unk1B[2] = 0;
            arg0->unk1B[1] = 0;
            arg0->unk1B[0] = 0;
            arg0->unk1E = 2;
            arg0->unk16 = 4;
            arg0->unk18 = 10;
            arg0->unk15 = 8;
            arg0->unk28 = 1.0f;
            func_1516CC58(arg0);
            arg0->unkE = 1;
            arg0->unk70 = arg0->unk5C;
        } else {
            arg0->unk15 = 2;
        }
        arg0->unk30 = 1.0f;
        arg0->unk34 = 1.0f;
        arg0->unk1A = 0;
        break;
    case 0x2:
        func_150428D4(arg0->unk24, &sp44, &sp40, &sp3C);
        arg0->unk30 = sp44 >> 1;
        arg0->unk34 = sp40 >> 1;
        arg0->unk28 += arg0->unk2C * D_800BE9A4;
        if (flag) {
            arg0->unk18 = 0;
        }
        if (arg0->unk28 > 0.75f && arg0->unk18 == 0) {
            arg0->unk18 = 10;
        }
        if (arg0->unk18 != 0) {
            arg0->unk18 -= D_800BE9A0;
            arg0->unk18 = (arg0->unk18 < 0) ? 0 : arg0->unk18;
            arg0->unk1A = (10.0f - arg0->unk18) / 10.0f * 255.0f;
        }
        if (arg0->unk28 > 1.0f) {
            arg0->unk28 = 1.0f;
            arg0->unk15 = 4;
            arg0->unk16 = 180;
        }
        arg0->unkE = 1;
        break;
    case 0x4:
        if (flag) {
            arg0->unk16 = 0;
        }
        arg0->unk16 -= D_800BE9A0 * 45;
        arg0->unk16 = (arg0->unk16 < 0) ? 0 : arg0->unk16;
        arg0->unk18 -= D_800BE9A0;
        arg0->unk18 = (arg0->unk18 < 0) ? 0 : arg0->unk18;
        arg0->unk1A = (10.0f - arg0->unk18) / 10.0f * 255.0f;
        if (arg0->unk16 == 0) {
            arg0->unk15 = 0x10;
        }
        break;
    case 0x8:
        arg0->unk16 -= D_800BE9A0;
        arg0->unk16 = (arg0->unk16 < 0) ? 0 : arg0->unk16;
        arg0->unk1B[arg0->unk1E] = (4 - (f32)arg0->unk16) / 4 * 170.0f;
        if (arg0->unk16 == 0) {
            if (--arg0->unk1E > 100) {
                arg0->unk15 = 0x10;
                arg0->unk18 = 10;
            } else {
                arg0->unk16 = 4;
            }
        }
        break;
    case 0x40:
        arg0->unk16 -= D_800BE9A0;
        arg0->unk16 = (arg0->unk16 < 0) ? 0 : arg0->unk16;
        if (flag) {
            arg0->unk16 = 0;
        }
        arg0->unk1A = arg0->unk16 / 10.0f * 255.0f;
        if (arg0->unk16 == 0) {
            arg0->unk20++;
            if (arg0->unk20 < arg0->unk10->unk8) {
                arg0->unk24 = func_1516C878((struct_game_197C20_1 *)arg0->unk10, arg0->unk20, arg0->unkD);
                arg0->unk18 = 10;
                arg0->unk15 = 0x10;
                arg0->unk10->unk0 |= 0x2000;
                if (arg0->unk10->unk2C != NULL) {
                    arg0->unk10->unk2C(arg0->unk10, arg0->unk20 - 1, arg0->unk15);
                }
                arg0->unk10->unk0 &= ~0x2000;
            } else {
                arg0->unk18 = 5;
                arg0->unk15 = 0x80;
                arg0->unk28 = 1.0f;
                arg0->unk2C = 0.25f;
            }
        }
        if (!flag) {
            break;
        }
    case 0x10:
        func_150428D4(arg0->unk24, &sp44, &sp40, &sp3C);
        arg0->unk30 = sp44 >> 1;
        arg0->unk34 = sp40 >> 1;
        arg0->unk18 -= D_800BE9A0;
        arg0->unk18 = (arg0->unk18 < 0) ? 0 : arg0->unk18;
        if (flag) {
            arg0->unk18 = 0;
        }
        arg0->unk1A = (10.0f - arg0->unk18) / 10.0f * 255.0f;
        if (arg0->unk18 == 0) {
            arg0->unk16 = arg0->unk10->unk26;
            arg0->unk15 = 0x20;
            arg0->unk10->unk0 |= 0x4000;
        }
        if (!flag) {
            break;
        }
    case 0x20: {
        s32 bits;
        done = 0;
        if ((u16)(arg0->unk10->unk0 & 0x4000)) {
            if (arg0->unk10->unk2C != NULL) {
                arg0->unk10->unk2C(arg0->unk10, arg0->unk20, arg0->unk15);
            }
            arg0->unk10->unk0 &= ~0x4000;
        }
        arg0->unk16 -= D_800BE9A0;
        arg0->unk16 = (arg0->unk16 < 0) ? 0 : arg0->unk16;
        if ((u16)(arg0->unk10->unk0 & 0x10) && arg0->unk14 == 0) {
            if (!(arg0->unk10->unk0 & 0x20) || func_151F2CDC() == 0) {
                if ((arg0->unk10->unk0 & 0x80) && arg0->unk20 + 1 == arg0->unk10->unk8) {
                    if (D_800BE710[0] & 0x4000) {
                        arg0->unk20 = -1;
                        done = 1;
                        D_800D1994 = 0;
                    } else if (D_800BE710[0] & 0x8000) {
                        done = 1;
                    }
                } else if (D_800BE710[0] & 0x8000) {
                    done = 1;
                } else if (!(arg0->unk10->unk0 & 0x40) && (D_800BE710[0] & 0x4000)) {
                    done = 1;
                    arg0->unk20 = arg0->unk10->unk8;
                }
            }
        }
        bits = arg0->unk10->unk0 & 0xF00;
        if (bits & 0x100) {
            arg0->unk10->unk0 &= ~0x100;
            done = 1;
            bits = arg0->unk10->unk0 & 0xF00;
        }
        if (!(bits & 0x200) && done) {
            arg0->unk16 = 10;
            arg0->unk15 = 0x40;
        }
    }
        break;
    case 0x80:
        if (arg0->unk10->unk0 & 2) {
            arg0->unk18 -= D_800BE9A0;
            arg0->unk18 = (arg0->unk18 < 0) ? 0 : arg0->unk18;
            arg0->unk1B[0] = arg0->unk18 / 5.0f * 85.0f;
            arg0->unk1B[1] = arg0->unk1B[0];
            arg0->unk1B[2] = arg0->unk1B[0];
        } else {
            arg0->unk28 -= arg0->unk2C * D_800BE9A4;
        }
        if (arg0->unk28 < 0.0f || arg0->unk18 == 0) {
            arg0->unk28 = 0.0f;
            arg0->unk15 = 0;
            arg0->unk20 = 0;
            arg0->unk10->unk0 |= 0x1000;
            if (arg0->unk10->unk2C != NULL) {
                arg0->unk10->unk2C(arg0->unk10, arg0->unk20, arg0->unk15);
            }
            arg0->unk10->unk0 &= ~0x1000;
            D_800CC4B3[arg0->unk10->unk9][0] = 0;
            *(s16 *)((u8 *)sp48 + 0x6CC) = 0;
            arg0->unk10->unk8 = 0;
            func_1516972C((struct102 *)arg0);
        }
        break;
    }
    func_1516CC58(arg0);
}

typedef struct {
    u8 pad0[6];
    s16 w;
    s16 h;
    u8 alpha;
    u8 r;
    u8 g;
    u8 flags;
    u8 padE[2];
    void *tex;
} Sprite_1516B6BC;

typedef struct {
    s32 unk0;
    char pad4[0x24];
    u8 unk28;
    u8 unk29;
    u8 unk2A;
} Data_1516B6BC;

typedef struct {
    char pad0[0xE];
    u8 unkE;
    char padF;
    Data_1516B6BC *unk10;
    char pad14;
    u8 unk15;
    char pad16[4];
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    char pad1E[0xA];
    f32 unk28;
    char pad2C[0xC];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    char pad50[8];
    f32 unk58;
    f32 unk5C;
    char pad60[0x10];
    f32 unk70;
} Obj_1516B6BC;
typedef struct {
    char pad0[6];
    u16 unk6;
    u16 unk8;
} Struct1516CC58_Vp;

extern Struct1516CC58_Vp D_8009055C;
extern Struct1516CC58_Vp D_8009057C;
#define WGFX1516B6BC(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (u32)(a); _g->words.w1 = (u32)(b); }
extern Struct1516CC58_Vp D_8009056C;
extern Struct1516CC58_Vp D_8009058C;
extern Struct1516CC58_Vp D_8009059C;
extern u16 D_80090564;
extern s32 D_800A4AC8[][2];
extern s32 D_800A4B90;
extern s32 D_800A4B94;
extern s32 D_800BE9C8[];
extern s32 D_800BEBA4;
extern u8 D_800D2DA8;
extern u8 D_800D2DA9;
extern u8 D_800D2DAA;
extern u8 D_800E0BDC;
Gfx *func_15094F70(Gfx *, Struct1516CC58_Vp *, s32, Sprite_1516B6BC *, s32, s32, s32, s32, s32);
Gfx *func_15095A48(Gfx *, Sprite_1516B6BC *, f32, f32);
Gfx *func_1516C934(Gfx *, Obj_1516B6BC *, s32, s32, s32, s32);

Gfx *func_1516B6BC(Gfx *gfx, Obj_1516B6BC *obj, s32 arg2) {
    Sprite_1516B6BC sprite;
    s32 pad80;
    f32 xoff;
    f32 yoff;
    s32 flag;
    s32 scale;
    s32 overflow;
    Gfx *start;
    f32 tw;
    f32 th;

    start = gfx;
    if (obj->unk10->unk0 & 4) {
        flag = 1;
    } else {
        flag = 0;
    }
    if (obj->unkE == 0) {
        return gfx;
    }
    if (D_800E0BDC == 0) {
        return gfx;
    }
    D_800D2DAA = 0;
    D_800D2DA9 = D_800D2DAA;
    D_800D2DA8 = D_800D2DA9;
    scale = D_800E0BDC + 1;
    sprite.flags = 0;
    sprite.alpha = (64 * scale) >> 8;
    tw = (f32)D_8009055C.unk6;
    th = (f32)D_8009055C.unk8;
    sprite.w = (2.0f * obj->unk38 + 1.0f) / (f32)D_8009055C.unk6 * 2355.19995117f * obj->unk28;
    sprite.h = (obj->unk3C + 1.0f) / (f32)D_8009055C.unk8 * 4259.83984375f * obj->unk28;
    xoff = obj->unk38 / (f32)D_8009055C.unk6 * (f32)D_8009057C.unk6 * 0.25f * obj->unk28;
    yoff = obj->unk3C / (f32)D_8009055C.unk8 * (f32)D_8009057C.unk8 * 0.5f * obj->unk28;
    obj->unk48 = cosf(obj->unk58 + 3.14159274101f) * (obj->unk38 * 0.5f) * 1.14999997616f * obj->unk28 + obj->unk40 - xoff;
    obj->unk4C = sinf(obj->unk58 + 3.14159274101f) * (obj->unk3C * 0.5f) * 1.03999996185f * obj->unk28 + obj->unk44 - yoff;
    WGFX1516B6BC(gfx++, 0xE7000000, 0);
    WGFX1516B6BC(gfx++, 0xFC30B261, 0xFF67FFFF);
    WGFX1516B6BC(gfx++, 0xEF002C0F, D_800A4AC8[25][0] | D_800A4AC8[25][1] | 4);
    if (obj->unk10->unk0 & 2) {
        sprite.alpha = (f32)obj->unk1B * 64.0f / 255.0f;
        sprite.alpha = (sprite.alpha * scale) >> 8;
        gfx = func_15094F70(gfx, &D_8009056C, 0, &sprite, 0, 0, 0, 2, 3);
    } else {
        gfx = func_15094F70(gfx, &D_8009055C, 0, &sprite, 0, 0, 0, 2, 3);
    }
    if (!flag) {
        gfx = func_15095A48(gfx, &sprite, obj->unk48 + 5.0f, obj->unk4C + -5.0f);
    }
    if (obj->unk58 > 0.0f && obj->unk58 < 3.14159274101f) {
        sprite.flags &= ~2;
    } else {
        sprite.flags |= 2;
    }
    if (obj->unk58 > 1.5707963705f && obj->unk58 < 4.71238899231f) {
        sprite.flags |= 1;
    } else {
        sprite.flags &= ~1;
    }
    overflow = (D_800BEBA4 < (((s32)gfx - D_800BE9C8[D_800BE9C0]) >> 3)) ? 1 : 0;
    if (overflow != 0) {
        return start;
    }
    if (obj->unk10->unk0 & 2) {
        if (obj->unk15 == 1) {
            obj->unk70 = obj->unk5C;
        }
        gfx = func_15094F70(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        sprite.alpha = (f32)obj->unk1C * 64.0f / 255.0f;
        sprite.alpha = (sprite.alpha * scale) >> 8;
        if (obj->unk58 > 1.5707963705f) {
            obj->unk70 = obj->unk70 + ((obj->unk40 + xoff) - obj->unk70) * 0.300000011921f;
            gfx = func_15095A48(gfx, &sprite, obj->unk70, obj->unk44 - yoff);
        } else {
            obj->unk70 = obj->unk70 + ((obj->unk40 - xoff) - obj->unk70) * 0.300000011921f;
            gfx = func_15095A48(gfx, &sprite, obj->unk70, obj->unk44 - yoff);
        }
        sprite.w /= 2;
        sprite.h /= 2;
        sprite.alpha = (f32)obj->unk1D * 64.0f / 255.0f;
        sprite.alpha = (sprite.alpha * scale) >> 8;
        gfx = func_15094F70(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        if (obj->unk58 > 1.5707963705f) {
            gfx = func_15095A48(gfx, &sprite, obj->unk40 - 2.5f, obj->unk44 + -2.5f);
        } else {
            gfx = func_15095A48(gfx, &sprite, obj->unk40 + 2.5f, obj->unk44 + -2.5f);
        }
    } else {
        gfx = func_15094F70(gfx, &D_8009057C, 0, &sprite, 0, 0, 0, 2, 3);
        if (!flag) {
            if (obj->unk58 > 1.5707963705f) {
                gfx = func_15095A48(gfx, &sprite, obj->unk40 + xoff - 2.5f, obj->unk44 - yoff + -2.5f);
            } else {
                gfx = func_15095A48(gfx, &sprite, obj->unk40 - xoff + 2.5f, obj->unk44 - yoff + -2.5f);
            }
        }
    }
    overflow = (D_800BEBA4 < (((s32)gfx - D_800BE9C8[D_800BE9C0]) >> 3)) ? 1 : 0;
    if (overflow != 0) {
        return start;
    }
    if (flag) {
        D_800D2DA8 = D_800D2DA9 = D_800D2DAA = 0;
    } else {
        D_800D2DA8 = obj->unk10->unk28;
        D_800D2DA9 = obj->unk10->unk29;
        D_800D2DAA = obj->unk10->unk2A;
    }
    sprite.flags = 0;
    if (flag) {
        sprite.alpha = 0x55;
    } else {
        sprite.alpha = 0xFF;
    }
    sprite.alpha = (sprite.alpha * scale) >> 8;
    WGFX1516B6BC(gfx++, 0xE7000000, 0);
    WGFX1516B6BC(gfx++, 0xFC30B261, 0xFF67FFFF);
    WGFX1516B6BC(gfx++, 0xEF002C0F, D_800A4AC8[25][0] | D_800A4AC8[25][1] | 4);
    if (obj->unk58 > 0.0f && obj->unk58 < 3.14159274101f) {
        sprite.flags &= ~2;
    } else {
        sprite.flags |= 2;
    }
    if (obj->unk58 > 1.5707963705f && obj->unk58 < 4.71238899231f) {
        sprite.flags |= 1;
    } else {
        sprite.flags &= ~1;
    }
    sprite.w = obj->unk38 / (f32)D_8009055C.unk6 * 2355.19995117f * obj->unk28;
    sprite.h = obj->unk3C / (f32)D_8009055C.unk8 * 4259.83984375f * obj->unk28;
    if (obj->unk10->unk0 & 2) {
        gfx = func_15094F70(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        sprite.alpha = obj->unk1C;
        sprite.alpha = (sprite.alpha * scale) >> 8;
        gfx = func_15095A48(gfx, &sprite, obj->unk70, obj->unk44 - yoff);
        sprite.w /= 2;
        sprite.h /= 2;
        gfx = func_15094F70(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        sprite.alpha = obj->unk1D;
        sprite.alpha = (sprite.alpha * scale) >> 8;
        if (obj->unk58 > 1.5707963705f) {
            gfx = func_15095A48(gfx, &sprite, obj->unk40, obj->unk44);
        } else {
            gfx = func_15095A48(gfx, &sprite, obj->unk40, obj->unk44);
        }
    } else if (obj->unk10->unk0 & 8) {
        gfx = func_15094F70(gfx, &D_8009059C, 0, &sprite, 0, 0, 0, 2, 3);
        if (obj->unk58 > 1.5707963705f) {
            gfx = func_15095A48(gfx, &sprite, obj->unk40 + xoff, obj->unk44 - yoff);
        } else {
            gfx = func_15095A48(gfx, &sprite, obj->unk40 - xoff, obj->unk44 - yoff);
        }
    } else {
        gfx = func_15094F70(gfx, &D_8009057C, 0, &sprite, 0, 0, 0, 2, 3);
        if (obj->unk58 > 1.5707963705f) {
            gfx = func_15095A48(gfx, &sprite, obj->unk40 + xoff, obj->unk44 - yoff);
        } else {
            gfx = func_15095A48(gfx, &sprite, obj->unk40 - xoff, obj->unk44 - yoff);
        }
    }
    sprite.w = 2.0f * obj->unk38 / (f32)D_8009055C.unk6 * 2355.19995117f * obj->unk28;
    sprite.h = obj->unk3C / (f32)D_8009055C.unk8 * 4259.83984375f * obj->unk28;
    if (obj->unk10->unk0 & 2) {
        sprite.alpha = obj->unk1B;
        sprite.alpha = (sprite.alpha * scale) >> 8;
        gfx = func_15094F70(gfx, &D_8009056C, 0, &sprite, 0, 0, 0, 2, 3);
    } else {
        gfx = func_15094F70(gfx, &D_8009055C, 0, &sprite, 0, 0, 0, 2, 3);
    }
    gfx = func_15095A48(gfx, &sprite, (s32)obj->unk48, (s32)obj->unk4C);
    overflow = (D_800BEBA4 < (((s32)gfx - D_800BE9C8[D_800BE9C0]) >> 3)) ? 1 : 0;
    if (overflow != 0) {
        return start;
    }
    return func_1516C934(gfx, obj, 0, 0, 0, (obj->unk1A * scale) >> 8);
}

u8 *func_1516C878(struct_game_197C20_1 *arg0, s32 arg1, s16 arg2) {
    u8 *temp_v1;
    u8 *sp0;
    s32 temp_v0;
    s32 temp_a0;

    switch (arg2) {
    case 0:
        temp_v1 = D_800DD254[arg0->unk4].unk0;
        break;
    case 1:
        temp_v1 = D_800DD258[arg0->unk4].unk0;
        break;
    case 2:
        temp_v1 = (u8 *)arg0->unk4;
        break;
    default:
        temp_v1 = sp0;
        break;
    }

    temp_v0 = 0;
    if (arg1 > 0) {
        temp_a0 = temp_v1[0];
        do {
            temp_v0++;
            if (temp_a0 != 0) {
                do {
                    temp_a0 = temp_v1[1];
                    temp_v1++;
                } while (temp_a0 != 0);
            }
            if (temp_a0 == 0) {
                do {
                    temp_a0 = temp_v1[1];
                    temp_v1++;
                } while (temp_a0 == 0);
            }
        } while (temp_v0 != arg1);
    }

    return temp_v1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_197F20/func_1516C934.s")

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
    char padA[2];
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 *unk18;
    f32 *unk1C;
    f32 *unk20;
} Struct1516CC58_10;

typedef struct {
    char pad0[0x10];
    Struct1516CC58_10 *unk10;
    char pad14[0xB];
    u8 unk1F;
    char pad20[8];
    f32 unk28;
    char pad2C[0xC];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    char pad48[8];
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
} Struct1516CC58;

typedef struct {
    char pad0[4];
    f32 unk4;
    f32 unk8;
} Struct1516CC58_BE628;

typedef struct {
    f32 unk0;
    char pad4[0x328];
} Struct1516CC58_CC310;

extern Struct1516CC58_CC310 D_800CC310[];
extern f32 cosf(f32);
extern u16 D_80090562;
extern u16 D_80090582;
extern f32 sinf(f32);
void func_1501A764(s16 arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6);
void func_1516D0CC(Struct1516CC58 *arg0, f32 *arg1, f32 *arg2, f32 *arg3);

void func_1516CC58(Struct1516CC58 *arg0) {
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    Struct1516CC58_10 *temp_v0;
    s32 sp44;
    s32 sp40;
    f32 temp_f0;
    f32 temp_f2;
    f32 var_f0_2;

    temp_v0 = arg0->unk10;
    if (temp_v0->unk0 & 0x8000) {
        temp_f2 = *temp_v0->unk18 * 0.00999999977648f;
        arg0->unk5C = (*(Struct1516CC58_BE628 **)&D_800BE628)->unk4 * temp_f2;
        temp_f2 = *temp_v0->unk1C * 0.00999999977648f;
        temp_f2 = 1.0f - temp_f2;
        arg0->unk60 = (*(Struct1516CC58_BE628 **)&D_800BE628)->unk8 * temp_f2;
        var_f0_2 = *temp_v0->unk20 * 0.00999999977648f;
        var_f0_2 = (var_f0_2 < 0.0f) ? 0.0f : ((var_f0_2 > 1.0f) ? 1.0f : var_f0_2);
        var_f0_2 = (70.0f * var_f0_2) + 55.0f;
        var_f0_2 = var_f0_2 * 0.0174532923847f;
        arg0->unk58 = var_f0_2;
        if (temp_v0->unk0 & 0x10000) {
            func_1516D0CC(arg0, &arg0->unk5C, &arg0->unk60, &arg0->unk58);
        }
    } else {
        func_1501A764(0, temp_v0->unkC, temp_v0->unk10, temp_v0->unk14, &sp54, &sp50, &sp4C);
        if (arg0->unk1F == 0) {
            arg0->unk64 = sp54;
            arg0->unk68 = sp50;
            arg0->unk6C = (D_800CC310[arg0->unk10->unk9].unk0 - D_800DBFF0->unk37C) - 180.0f;
            arg0->unk1F = 1;
        }
        var_f0_2 = arg0->unk6C;
        arg0->unk5C = arg0->unk64;
        arg0->unk60 = arg0->unk68;
        if (var_f0_2 < 0.0f) {
            var_f0_2 += 360.0f;
        }
        if (var_f0_2 > 180.0f) {
            var_f0_2 -= 360.0f;
        }
        if (var_f0_2 > 90.0f) {
            var_f0_2 = 180.0f - var_f0_2;
        } else if (var_f0_2 < -90.0) {
            var_f0_2 = -180.0f - var_f0_2;
        }
        var_f0_2 = (var_f0_2 / -180.0f) + 0.5f;
        var_f0_2 = (70.0f * var_f0_2) + 55.0f;
        var_f0_2 = var_f0_2 * 0.0174532923847f;
        arg0->unk58 = var_f0_2;
        func_1516D0CC(arg0, &arg0->unk5C, &arg0->unk60, &arg0->unk58);
    }
    arg0->unk40 = arg0->unk5C;
    arg0->unk44 = arg0->unk60;
    sp44 = (arg0->unk38 / (f32)(u32)D_8009055C.unk6) * (f32)(u32)D_8009057C.unk6 * 0.25f;
    sp40 = (arg0->unk3C / (f32)(u32)D_8009055C.unk8) * (f32)(u32)D_8009057C.unk8 * 0.5f;
    arg0->unk50 = (s32)(((cosf(arg0->unk58 + 3.14159274101f) * (arg0->unk38 * 0.5f) * 1.14999997616f) + arg0->unk5C) - (f32)sp44);
    arg0->unk54 = ((sinf(arg0->unk58 + 3.14159274101f) * (arg0->unk3C * 0.5f) * 1.03999996185f) + arg0->unk60) - (f32)sp40;
}

typedef struct {
    char pad0[0x24];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} Struct1516D0CC_BE628;

void func_1516D0CC(Struct1516CC58 *arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
    f32 top;
    f32 y;
    f32 left;
    f32 right;
    f32 xoff;
    f32 halfW;
    f32 h;
    f32 x;

    xoff = (arg0->unk38 / (f32)(u32)D_80090562) * (f32)(u32)D_80090582 * 0.25f * arg0->unk28;
    x = (*arg1 + (cosf(arg0->unk58 + 3.14159274101f) * (arg0->unk38 * 0.5f) * 1.14999997616f * arg0->unk28)) - xoff;
    halfW = arg0->unk38 * 0.5f;
    h = arg0->unk3C;
    y = *arg2;
    top = *arg2 - h;
    left = x - halfW;
    right = halfW + x;
    if (arg0 != NULL) {
        if (top < (*(Struct1516D0CC_BE628 **)&D_800BE628)->unk24) {
            *arg2 = (*(Struct1516D0CC_BE628 **)&D_800BE628)->unk24 + h;
        } else if ((*(Struct1516D0CC_BE628 **)&D_800BE628)->unk28 < y) {
            *arg2 = (*(Struct1516D0CC_BE628 **)&D_800BE628)->unk28;
        }
        if (left < (*(Struct1516D0CC_BE628 **)&D_800BE628)->unk2C) {
            *arg1 -= left - (*(Struct1516D0CC_BE628 **)&D_800BE628)->unk2C;
        } else if ((*(Struct1516D0CC_BE628 **)&D_800BE628)->unk30 < right) {
            *arg1 += (*(Struct1516D0CC_BE628 **)&D_800BE628)->unk30 - right;
        }
        *arg1 = (s32)*arg1;
        *arg2 = (s32)*arg2;
    }
}

void func_1516D2D8(void) {
}

void func_1516D2E0(struct_game_197C20_3 *arg0) {
    if ((arg0 != 0) && (arg0->field_0x0 == 0x51)) {
        arg0->field_0x10->field_0x8 = 0;
        func_1516972C((struct102 *)arg0);
        arg0->field_0x15 = 0;
    }
}

void func_1516D328(struct_game_197C20_3 *arg0) {
    if ((arg0 != 0) && (arg0->field_0x0 == 0x51)) {
        arg0->field_0x10->field_0x0 |= 0x100;
        arg0->field_0x10->field_0x0 &= ~0x400;
        arg0->field_0x20 = arg0->field_0x10->field_0x8;
    }
}

u8 *func_1516D378(struct_game_197C20_3 *arg0) {
    if ((arg0 != 0) && (arg0->field_0x0 == 0x51)) {
        arg0->field_0x10->field_0x0 |= 0x100;
        arg0->field_0x10->field_0x0 &= ~0x400;
        return (u8 *)arg0;
    }
    return 0;
}

s32 func_1516D3C4(u8 *arg0) {
    s32 *temp;

    if ((arg0 != 0) && (*arg0 == 0x51)) {
        temp = *(s32 **)(arg0 + 0x10);
        *temp |= 0x500;
        return (s32)arg0;
    }
    return 0;
}
