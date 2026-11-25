#include <ultra64.h>
/* include/functions.h declares func_150ADA20 as returning u8.  The golden code for
 * func_150DFEFC feeds its result straight into `divu` with no zero-extension, which a
 * u8-returning declaration cannot produce, so the real return type is a word.  Per project
 * policy the shared header is left alone and the correct prototype is made file-local:
 * the header's declaration is renamed out of the way while functions.h is included. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
s32 func_150ADA20(void);
/* variables.h declares D_800D3098 as an array, but the code loads it as a pointer to the
 * table (func_150DF8C0 dereferences the symbol's first word), so it gets a local declaration. */
#define D_800D3098 D_800D3098_array_decl_in_variables_h
#include "variables.h"
#undef D_800D3098
extern struct178 *D_800D3098;


extern f32 D_800A0FB4;
extern f32 D_800A0FB8;
extern void *func_1513418C(void *, s32, u8, s32);

typedef union {
    s8 field_0x0_s8;
    s32 field_0x0_s32;
} DisplayListWord0;

typedef struct {
    DisplayListWord0 field_0x0;
    u8 pad_0x4[0x4];
} DisplayListEntry8;

typedef struct {
    u8 pad_0x0[0x13];
    u8 field_0x13;
    u8 pad_0x14[0x10];
    DisplayListEntry8 **field_0x24;
} Game10CD70Object;

typedef struct {
    u8 pad_0x0[0x2D8];
    f32 field_0x2D8;
    f32 field_0x2DC;
    u8 pad_0x2E0[0xC];
    s32 field_0x2EC;
} Game10CD70FadeState;

typedef struct {
    u8 pad_0x0[0x30];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
} Game10CD70Transform;

typedef struct {
    u8 pad_0x0[0x14];
    f32 field_0x14;
    u8 pad_0x18[0x4];
    f32 field_0x1C;
    u8 pad_0x20[0x1B4];
    s32 field_0x1D4;
} Game10CD70Actor;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 pad9[3];
    void *unkC;
    u8 unk10;
    u8 pad11[3];
    struct17 unk14;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    u8 unk2A;
    u8 unk2B;
    s8 unk2C;
    u8 unk2D;
} Struct150E0348;

typedef struct {
    u8 values[3];
} Game10CD70Indices;

extern Game10CD70Indices D_80088984;

/* Returns whether the D_800D3098 record selected by D_80088984[index] is active. */
s32 func_150DF8C0(s32 index) {
    Game10CD70Indices indices = D_80088984;

    if (D_800D3098[indices.values[index]].unk6[0xE] != 0) {
        return 1;
    }
    return 0;
}

/* One display-list command, with the opcode byte of its first word addressable. */
typedef struct {
    union {
        s8 opcode;
        u32 word;
    } w0;
    u32 w1;
} GfxCmd;

/* matching: the scan loops address dl by (j << 3) bytes; dl[j] would be strength-reduced */
#define GFX_CMD(dl, j) ((GfxCmd *)(((j) << 3) + (u8 *)(dl)))

extern u8 D_800D9950[];
extern void func_150A2864(s32, s32);
extern void func_150A3444(s32, s16, s16, s16);
extern void func_151749A0(s32, s32);

/* Places markers 3 and 4 above actors 6 and 7, then scrolls the texture of each of the three
 * materials in the D_800B0E00 display list that loads its image from segment 2 + i. */
void func_150DF920(s32 arg0) {
    struct127 *obj;
    s32 i;
    s32 j;
    s32 id;
    s32 slot;
    f32 x;
    f32 y;
    f32 z;
    s32 active;
    GfxCmd *dl;
    s32 opcode;
    s32 uls;
    s32 ult;

    if (arg0 != 0) {
        return;
    }

    func_151749A0(5, 4);

    id = 6;
    slot = 3;
    for (i = 0; i < 2; i++) {
        obj = func_15083E90((u8)id) /* takes a u8; functions.h says s32 */;
        id = 7;
        if (obj != NULL) {
            func_150A2864(slot, 0);
            x = obj->x_position;
            y = obj->y_position + 200.0f;
            z = obj->z_position;
            func_150A3444(slot, x, y, z);
        } else {
            func_150A2864(slot, 1);
        }
        slot = 4;
    }

    dl = (GfxCmd *)D_800B0E00[0];

    for (i = 0; i < 3; i++) {
        active = func_150DF8C0(i);
        if (active) {
            if (D_800D9950[i] < 0x60) {
                D_800D9950[i] += D_800BE9E4;
                if (D_800D9950[i] > 0x60) {
                    D_800D9950[i] = 0x60;
                }
            }
        } else {
            D_800D9950[i] = 0;
        }

        /* Find the G_SETTIMG whose image address is segment 2 + i. */
        j = -1;
        do {
            j++;
            while (GFX_CMD(dl, j)->w0.opcode != (s8)G_SETTIMG &&
                   GFX_CMD(dl, j)->w0.opcode != (s8)G_ENDDL) {
                j++;
            }
            if (GFX_CMD(dl, j)->w0.opcode == (s8)G_ENDDL) {
                j = -1;
                break;
            }
        } while (dl[j].w1 != (i << 24) + 0x02000000);

        if (j != -1) {
            /* Then the G_SETTILESIZE after it. */
            while ((opcode = GFX_CMD(dl, j)->w0.opcode) != (s8)G_SETTILESIZE) {
                j++;
            }

            if (j != -1) {
                if (active) {
                    uls = (dl[j].w0.word >> 12) & 0xFFF;
                    ult = dl[j].w0.word & 0xFFF;
                    ult += (D_800D9950[i] / 16) * D_800BE9E4;
                } else {
                    ult = 2;
                    uls = 2;
                }
                dl[j].w0.word = (G_SETTILESIZE << 24) | ((uls & 0xFFF) << 12) | (ult & 0xFFF);
            }
        }
    }
}

extern s32 D_80090324[];
extern u8 D_800DD405;

/* Binds segments 2..4 to the textures of the three materials func_150DF920 scrolls: an
 * active material gets entry 9 + D_800DD405 of D_80090324, an inactive one entry 8. */
Gfx *func_150DFBD0(Gfx *gdl) {
    s32 i;
    s32 segment;
    s32 addr;
    s32 unused;
    s32 asset;
    u8 variant;

    for (i = 0; i < 3; i++) {
        if (func_150DF8C0(i) != 0) {
            variant = D_800DD405;
            asset = D_80090324[variant + 9];
        } else {
            asset = D_80090324[8];
        }
        addr = func_1510D0EC(asset, &unused, 3, 0);
        segment = i + 2;
        gSPSegment(gdl++, segment, addr);
    }
    return gdl;
}

/* matching: 0.55f, kept in rodata; a literal folds the negation into a -0.55f constant */
extern f32 D_800A0FA4;

/* The object func_150DFCA8 animates (a handler in the 0x80088xxx object table). */
typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ f32 pos;
    /* 0x08 */ u8 pad8[0x5C];
    /* 0x64 */ f32 speed;
    /* 0x68 */ u8 pad68[0x14];
    /* 0x7C */ f32 targetSpeed;
} SwingObject;

/* Moves pos back and forth between 11 and 55: the target speed flips sign at either end and
 * the speed eases towards it by 0.05 per call. */
void func_150DFCA8(SwingObject *arg0) {
    f32 pos;
    f32 target;

    // matching: the step shares target's register
    target = arg0->speed * D_800BE9E4;
    arg0->pos += target;
    pos = arg0->pos;
    if (pos < 11.0f) {
        arg0->targetSpeed = target = 0.55f;
    } else if (arg0->pos > 55.0f) {
        pos = D_800A0FA4; // matching: the negated constant goes through pos's register
        arg0->targetSpeed = target = -pos;
    } else {
        target = arg0->targetSpeed;
    }
    if (arg0->speed < target) {
        arg0->speed += 0.05f;
        if (target < arg0->speed) {
            arg0->speed = target;
        }
    } else if (target < arg0->speed) {
        arg0->speed -= 0.05f;
        if (arg0->speed < target) {
            arg0->speed = target;
        }
    }
}

extern f32 D_800A0FB0;

void func_150DFDA4(Game10CD70Actor *arg0) {
    Game10CD70Transform *p = (Game10CD70Transform *)(arg0->field_0x1D4 + 0x40);

    p->field_0x30 = arg0->field_0x14;
    p->field_0x34 = D_800A0FB0;
    p->field_0x38 = arg0->field_0x1C;
}

s32 func_150DFDD0(Game10CD70Object *arg0, Game10CD70FadeState *arg1) {
    f32 temp_f2;
    DisplayListEntry8 *temp_v0;
    s32 temp_a0;
    s32 temp_a2;
    s32 temp_v1;

    if (arg1->field_0x2EC != 0) {
        arg0->field_0x13 = 2;
        arg1->field_0x2D8 = arg1->field_0x2D8 - D_800BE9A4;
        if (arg1->field_0x2D8 < 0.0f) {
            arg1->field_0x2EC = 0;
            return 1;
        }
        temp_f2 = 1.0f - (arg1->field_0x2DC * arg1->field_0x2D8);
    } else {
        temp_f2 = 0.0f;
        arg0->field_0x13 = 0;
    }

    temp_v0 = *arg0->field_0x24;
    if (temp_v0 != 0) {
        temp_v1 = 0;
        if (((DisplayListEntry8 *)((temp_v1 << 3) + (u8 *)temp_v0))->field_0x0.field_0x0_s8 != -0xE) {
            do {
            } while (((DisplayListEntry8 *)((u8 *)temp_v0 + (++temp_v1 << 3)))->field_0x0.field_0x0_s8 != -0xE);
        }
        temp_a0 = ((DisplayListEntry8 *)((u8 *)temp_v0 + (temp_v1 * 8)))->field_0x0.field_0x0_s32;
        arg1 = (Game10CD70FadeState *)((u8 *)temp_v0 + (temp_v1 * 8));
        temp_a2 = 2 - ((s32)(((u32)temp_a0 >> 12) & 0xFFF) / 3);
        while (temp_a2 < 0) {
            temp_a2 += 0x40;
        }
        ((DisplayListEntry8 *)arg1)->field_0x0.field_0x0_s32 = 0xF2000000 | ((((s32)((500.0f * temp_f2) + 2.0f)) & 0xFFF) << 12) | (temp_a2 & 0xFFF);
    }

    return 0;
}

/* The 12-byte record at +0x28 that func_150E02C0 also hands to func_15149514 as
 * (arg0 + 0x28, arg0 + 0x2C): an actor handle plus three independent countdowns. */
typedef struct {
    /* 0x00 */ struct127 *obj;
    /* 0x04 */ u8  id;
    /* 0x05 */ u8  pad5;
    /* 0x06 */ s16 timer0;
    /* 0x08 */ s16 timer1;
    /* 0x0A */ s16 timer2;
} SmokeSource; /* size 0xC */

typedef struct {
    /* 0x00 */ u8  pad0;
    /* 0x01 */ u8  unk1;
    /* 0x02 */ u8  pad2[0xA];
    /* 0x0C */ u8  unkC;
    /* 0x0D */ u8  padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8  pad10[0x18];
    /* 0x28 */ SmokeSource src;
} SmokeEmitter;

extern struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, u8, u8, s32, u8, s32);
extern void func_15107700(struct127 *, s16, s16, s16, s32, f32, f32, s16, s16, u8 *, u8, s32);
extern void func_15107B78(struct127 *, s32, s32, s32, s32);

void func_150DFEFC(SmokeEmitter *arg0) {
    SmokeSource *src;
    struct127 *obj;
    Header header;
    Header2 header2;
    s8 count;

    obj = arg0->src.obj;
    src = &arg0->src;
    if ((obj->interaction_state == 0) || (obj->unique_id != src->id)) {
        arg0->unkE = -1;
        return;
    }
    src->timer0 -= D_800BE9E4;
    if (src->timer0 < 0) {
        header2.unk0 = (s32)obj->x_position;
        header2.unk4 = (s32)obj->y_position;
        header2.unk8 = (s32)obj->z_position;
        header.unk0 = 3;
        header.unk1 = -1;
        header.unk2 = (func_150ADA20() % 0xBU) + 5;
        header.unk4 = 0;
        func_151602C0(&header, &header2, (func_150ADA20() % 0x79U) + 0x32, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg0->unkC, arg0->unk1);
        src->timer0 = (func_150ADA20() % 0x51U) + 0x45;
    }
    src->timer1 -= D_800BE9E4;
    if (src->timer1 < 0) {
        u8 colour[4];

        count = (func_150ADA20() % 3U) + 2;
        do {
            colour[0] = 0xAE;
            colour[1] = 0xD2;
            colour[2] = 0xFF;
            colour[3] = (func_150ADA20() % 0x3DU) + 0x96;
            func_15107700(obj, (s16)(func_150ADA20() & 0xFF), (s16)((func_150ADA20() % 0x81U) - 0x3F), (s16)((func_150ADA20() % 0x15U) + 0xA), 4, 40.0f, (func_150ADA68() * 20.0f) + 25.0f, 0, 2, colour, arg0->unkC, arg0->unk1);
            count--;
        } while (count > 0);
        src->timer1 = (func_150ADA20() % 0x8DU) + 0x19;
    }
    src->timer2 -= D_800BE9E4;
    if (src->timer2 < 0) {
        func_15107B78(obj, (s16)(func_150ADA20() & 0xFF), (s16)((func_150ADA20() & 0x7F) - 0x40), arg0->unkC, arg0->unk1);
        src->timer2 = (func_150ADA20() % 0x1AU) + 0xF;
    }
}

void func_150E02C0(s32 arg0, s32 arg1, s32 arg2) {
    func_15149514(arg1, (u8)arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

void func_1513BAE8(void);

void func_150E0300(void) {
    if (D_80088980 == 0) {
        func_1515F170(6, 0);
        func_1513BAE8();
        D_80088980 = 1;
    }
}

void func_150E0348(void *arg0, u8 arg1, s32 arg2) {
    Struct150E0348 sp18;

    sp18.unk0 = 0;
    sp18.unk4 = 0;
    sp18.unk8 = *((u8 *)arg0 + 0x3B);
    sp18.unk10 = 0;
    sp18.unkC = arg0;
    sp18.unk14 = *(struct17 *)&D_800A5480;
    sp18.unk28 = 0x12C;
    sp18.unk2A = 0x1B;
    sp18.unk2B = 0xB;
    sp18.unk2C = -1;
    sp18.unk2D = 0;
    sp18.unk20 = D_800A0FB4;
    sp18.unk24 = D_800A0FB8;
    func_1513418C(&sp18, 0, arg1, arg2);
}

void func_15143794(s16, s16, f32, void *);
void func_151A26EC(f32 *, f32 *, f32 *, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_150E03F8(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, void *arg6) {
    f32 position[3];
    f32 direction[3];
    f32 acceleration[3];
    f32 scale;
    f32 size_random;
    u32 random_b;
    u32 random_a;
    f32 alpha_random;

    position[0] = arg0;
    position[1] = arg1;
    position[2] = arg2;
    random_a = func_150ADA20();
    random_b = func_150ADA20();
    alpha_random = func_150ADA68();
    func_15143794((s16) (random_a & 0xFF), (s16) ((random_b % 65U) - 0x20), alpha_random * 5.87f, &direction[0]);
    scale = (func_150ADA68() * 0.16900001f) + 0.186f;
    acceleration[0] = 0.0f;
    acceleration[1] = 0.0f;
    acceleration[2] = 0.0f;
    direction[0] += -arg3 * D_800BE9A8 * scale;
    direction[1] += -arg4 * D_800BE9A8 * scale;
    direction[2] += -arg5 * D_800BE9A8 * scale;
    alpha_random = func_150ADA68();
    size_random = func_150ADA68();
    random_a = func_150ADA20();
    random_b = func_150ADA20();
    func_151A26EC(&position[0], &acceleration[0], &direction[0], 0.986891f, (alpha_random * 0.23f) + -0.337f, (size_random * 200.0f) + 150.0f, (random_a % 26U) + 0x28, (random_b % 201U) + 0x37, 0x1E, 0x14, 0, -1, 0x34, 0x35, 0x34, (s32) *((u8 *)arg6 + 0xC), (s32) *((u8 *)arg6 + 1));
}

extern f32 D_800A0FD4;
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void func_150E05F8(void *arg0) {
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 var_v0;
    temp_fv1 = *(f32 *)((u8 *)D_800DBFF0 + 0x300) - (f32) *(s16 *)((u8 *)arg0 + 0x14);
    temp_fa0 = *(f32 *)((u8 *)D_800DBFF0 + 0x2F8) - (f32) *(s16 *)((u8 *)arg0 + 0x10);
    temp_fa1 = *(f32 *)((u8 *)D_800DBFF0 + 0x2FC) - (f32) *(s16 *)((u8 *)arg0 + 0x12);
    temp_fv0 = sqrtf((temp_fv1 * temp_fv1) + ((temp_fa0 * temp_fa0) + (temp_fa1 * temp_fa1)));
    if (temp_fv0 <= 2570.0f) {
        var_v0 = 0xFF;
    } else if (temp_fv0 >= D_800A0FD4) {
        var_v0 = 0;
    } else {
        var_v0 = 255 - (temp_fv0 - 2570.0f) * (1 / (D_800A0FD4 - 2570.0f)) * 255.0f;
    }
    *(s8 *)((u8 *)arg0 + 0x8A) = var_v0;
}

extern u8 D_80088988;
extern const f32 D_800A0F70[][3];
extern const f32 D_800A0F88[][3];
extern s32 func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);
extern void func_15102B38(s32, u8, const f32 *, const f32 *, f32 *, s32, s32, f32, struct17 *, s32, s32, s32, u8, s32);

void func_150E06D8(s32 arg0, u8 arg1, u8 arg2, s32 arg3) {
    struct17 sp64;
    f32 scale;
    f32 sp58[2];
    s32 sp54[1];
    s32 sp50[1];
    f32 sp4C;
    s32 sp48;
    s32 sp44;

    if (((struct127 *)arg0)->unk1D4 == 0) {
        return;
    }
    sp54[0] = (s32)D_800A0F70[arg1];
    sp50[0] = (s32)&sp64;
    func_15145EA4(sp54, sp50, (s32)((struct127 *)arg0)->unk1D4 + (D_80088988 << 6), 1);
    scale = 1.0f;
    sp58[1] = func_150ADA68() * scale + 2.0f;
    sp58[0] = func_150ADA68() * 14.0f + 28.0f;
    sp44 = func_150ADA20();
    sp48 = func_150ADA20();
    sp4C = func_150ADA68() * 300.0f + 400.0f;
    func_15102B38(arg0, D_80088988, D_800A0F70[arg1], D_800A0F88[arg1], sp58,
                  (sp44 % 3U) + 4, (sp48 % 0x9CU) + 0x64,
                  sp4C, &sp64, 0xFF,
                  0, -1, arg2, arg3);
}
