#include <ultra64.h>

#define func_15048A40 func_15048A40_hdr
#define func_150ADA20 func_150ADA20_hdr
#include "functions.h"
#undef func_15048A40
#undef func_150ADA20
#include "variables.h"
extern s32 func_150ADA20(void);
void func_1508F9F4(void);

extern s32 func_1510D0EC(s32, s32, s32, s32);
void func_15042D94(s32, s32, u8, s32 *, ...);
extern u8 D_D16[];

// need to figure out D_800D2460
void func_1508F060(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        D_800D2460[i][13] = 0;
    }
    D_800D24C0 = 0;
}

void func_1508F0A4(void) {
    func_1508F0D4();
    func_1508F9F4();
    func_1509093C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_BC510/func_1508F0D4.s")

void func_1508F7BC(void) {
    f32 sp48[4][4];
    f32 sp44;
    struct256 *temp_v0;

    D_800D2456 -= 1;
    D_800D2890.unk0 = (u32) (func_150ADA20() % 5U);
    D_800D2890.unk8 = sqrtf(25.0f - (D_800D2890.unk0 * D_800D2890.unk0));
    temp_v0 = allocate_memory(sizeof(*temp_v0), 1, 1, 0);
    temp_v0->unk80 = D_800D245C;
    D_800D245C = temp_v0;
    guMtxL2F(&sp48, (D_800D2456 << 7) + ((D_800BE9C0 ^ 1) << 6) + &D_800D2590);
    func_1503E5F8(&sp48, &temp_v0->unk84, &temp_v0->unk88, &temp_v0->unk8C, &temp_v0->unk9C, &temp_v0->unkA0, &temp_v0->unkA4, &sp44, &sp44, &sp44);

    temp_v0->unk90 = (u32)((func_150ADA20() & 0xF) + 10);
    temp_v0->unk94 = (u32)((func_150ADA20() & 0x1F) + 20);
    temp_v0->unk98 = 0.0f;
    temp_v0->unkA8 = (u32)(func_150ADA20() & 0xF);
    temp_v0->unkAC = (u32)(func_150ADA20() & 0xF);
    temp_v0->unkB0 = (u32)(func_150ADA20() & 0xF);
    temp_v0->unkB4 = 120;
}

void func_1508F9C4(void) {
    D_800D2410[D_800D2456++] = 0.0f;
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 state;
    u8 unkE;
    u8 unkF;
} BC510Bubble;
extern f32 func_15048A40(u8 arg0);

void func_1508F9F4(void) {
    s32 i;
    s32 v;
    s32 flag;
    s32 d;
    s32 n;
    s32 t;
    s32 u;
    s32 j;
    s32 x;

    u = D_800D24C0;
    if (u != 0) {
        u -= D_800BE9E4;
        if (u < 0) {
            u = 0;
        }
        D_800D24C0 = u;
    }
    if (D_800CC2D0[0].in_water) {
        u = D_800CC2D0[0].unkB2;
    } else {
        u = 0;
    }
    if (u > 90) {
        n = u / 240;
        if (n > 6) {
            n = 6;
        }
        n = 6 - n;
        n = n * 9;
        if (n == 0) {
            flag = 0;
        }
    } else {
        flag = 0;
    }
    for (i = 0; i < 6; i++) {
        BC510Bubble *p = (BC510Bubble *)D_800D2460[i];
        if (p->state == 2) {
            v = p->unkA;
            v -= D_800BE9E4;
            if (v <= 0) {
                v = 6;
                p->state = 3;
            }
            p->unkA = v;
        }
    sw:
        switch (p->state) {
        case 0:
    LX: ;
        l0:
            if (flag != 0 && D_800D24C0 == 0) {
                D_800D24C0 = 10;
                p->state = 1;
                p->unk0 = 0xFC;
                p->unk2 = 0x60;
                p->unk6 = 0;
                p->unkC = 0;
                p->unkE = func_150ADA20();
                p->unk8 = 0x500;
                p->unk4 = 0xFC - ((s32)(p->unk8 * func_15048A40(p->unkE)) >> 8);
                p->unkF = 0;
                p->unkA = n;
            }
            break;
        case 1:
            v = p->unk6;
            v += D_800BE9E4 * 200;
            if (v >= 0x666) {
                p->state = 2;
                v = 0x666;
            }
            p->unk6 = v;
            break;
        case 2:
            d = D_800BE9E4;
            j = 0;
            v = p->unkC;
            if (v < 0x20) {
                v = v + d;
                if (v > 0x20) {
                    v = 0x20;
                }
                p->unkC = v;
            }
            x = p->unk2;
            x -= (d * p->unkC) >> 4;
            *(volatile s16 *)&p->unk2 = x;
            t = *(volatile s16 *)&p->unk6;
            if (d > 0) {
                do {
                    j++;
                    t = (t * 0x104) >> 8;
                } while (j < d);
            }
            p->unk6 = t;
            p->unkE += d * 5;
            p->unk8 += d * 30;
            p->unk0 = ((s32)(func_15048A40(p->unkE) * p->unk8) >> 8) + p->unk4;
            p->unkF += D_800BE9E4 * 10;
        lbl:
            break;
        case 3:
            if (p->unkA == 0) {
                p->state = 0;
            }
            break;
        }
    }
}
typedef struct GameBC510Command {
    u32 w0;
    u32 w1;
} GameBC510Command;
GameBC510Command *func_150900F0(GameBC510Command *, s32);
GameBC510Command *func_150918EC(GameBC510Command *);
GameBC510Command *func_150938BC(GameBC510Command *);
GameBC510Command *func_15093B58(GameBC510Command *, s32, s16 *, s32, s32);
GameBC510Command *func_15090630(GameBC510Command *);
GameBC510Command *func_150911F4(GameBC510Command *);
void *func_15094F40(void *);
void *func_15096934(void *);
void func_1518CA04(s32);
extern f32 D_8008FE1C;
extern s32 D_800BEBA4;
extern GameBC510Command *D_800BE9C8[];

GameBC510Command *func_1508FD38(register GameBC510Command *arg0) {
    GameBC510Command *original;
    s32 index;
    s32 step;
    s32 overflow;
    s16 rows[33];

    original = arg0;
    if (D_800BE9F0 == 0x1D) return arg0;
    { GameBC510Command *packet0 = arg0++; packet0->w0 = 0xDB0E0000; packet0->w1 = *(u16 *)&D_800D2454; }
    { GameBC510Command *packet1 = arg0++; packet1->w0 = 0xDA380007; packet1->w1 = (u32)&D_800D23D0; }
    if (D_800BE616 != 0) {
        arg0 = func_150918EC(arg0);
        overflow = D_800BEBA4 < arg0 - *(D_800BE9C8 + D_800BE9C0) ? 1 : 0;
        if (overflow != 0) return original;
        return arg0;
    }
    if (D_800D2458 != 0) arg0 = func_150938BC(arg0);
    if (D_800C35EA == 1) return arg0;
    if (D_8008CD00 != 0) {
        step = 0x400;
        if (D_8008FE1C != 1.0f) {
            step = (s32)((f32)step / D_8008FE1C);
            for (index = 0; index < 33; index++) *(rows + index) = (s32)((f32)index * D_8008FE1C);
        } else {
            for (index = 0; index < 33; index++) *(rows + index) = index;
        }
        arg0 = func_15093B58(arg0, step, rows, 0x2C30, 1);
    }
    if (D_800D2428[0] != (f32)D_8009DCB4[0]) arg0 = func_150900F0(arg0, 0);
    else if (D_800D2588 != 0) {
        func_1518CA04(0xB8);
        D_800D2588 = 0;
    }
    arg0 = func_150911F4(arg0);
    arg0 = func_15094F40(arg0);
    arg0 = func_15090630(arg0);
    arg0 = func_15096934(arg0);
    overflow = D_800BEBA4 < arg0 - *(D_800BE9C8 + D_800BE9C0) ? 1 : 0;
    if (overflow != 0) return original;
    return arg0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_BC510/func_150900F0.s")
typedef struct BC510Entry {
    s16 x;
    s16 y;
    s16 unk4;
    s16 scale;
    s16 unk8;
    s16 timer;
    u8 unkC;
    u8 state;
    u8 unkE;
    u8 sound;
} BC510Entry;

typedef struct BC510Sprite {
    s32 unk0;
    s16 unk4;
    s16 w;
    s16 h;
    u8 alpha;
    u8 unkB;
    u8 unkC;
    u8 flip;
    u8 padE[2];
} BC510Sprite;

typedef struct BC510Tex {
    s32 id;
    s32 unk4;
    s32 unk8;
} BC510Tex;

extern f32 func_15048A40(u8 arg0);
extern BC510Tex D_800903D4;
extern BC510Tex D_80091840;
GameBC510Command *func_15094F70(GameBC510Command *, BC510Tex *, s32, BC510Sprite *, s32, s32, s32, s32, s32);
GameBC510Command *func_15095A48(GameBC510Command *, BC510Sprite *, f32, f32);

#define ENT ((BC510Entry *)D_800D2460)
GameBC510Command *func_15090630(GameBC510Command *gfx) {
    s32 i;
    BC510Sprite sprite;
    s32 first;
    s32 x;
    s32 y;
    s32 bits;
    f32 f;
    s32 scale;
    s32 t;
    s32 tex;

    sprite.alpha = 0xFF;
    sprite.flip = 0;
    { GameBC510Command *c = gfx++; c->w0 = 0xE7000000; c->w1 = 0; }
    { GameBC510Command *c = gfx++; c->w0 = 0xFCFFB5FF; c->w1 = 0xFFFCFE38; }
    { GameBC510Command *c = gfx++; c->w0 = 0xEF102C0F; c->w1 = 0x0C184244; }
    first = 1;
    i = 0;
    do {
        if (ENT[i].state == 1 || ENT[i].state == 2) {
            if (first) {
                func_1510D0EC(D_80091840.id, (s32)&tex, 3, 0);
                first = 0;
                gfx = func_15094F70(gfx, &D_800903D4, 0, &sprite, 0, 0, 0, 2, 3);
            }
            f = func_15048A40(ENT[i].sound);
            x = ENT[i].x;
            y = ENT[i].y;
            sprite.w = ENT[i].scale * (0.200000003f * f + 1.0f);
            sprite.h = ENT[i].scale * (-0.200000003f * f + 1.0f);
            gfx = func_15095A48(gfx, &sprite, x, y);
        }
        i++;
    } while (i < 6);
    first = 1;
    i = 0;
    do {
        bits = 0;
        if (ENT[i].state == 3) {
            scale = ENT[i].scale;
            x = ENT[i].x;
            y = ENT[i].y;
            sprite.w = scale;
            sprite.h = scale;
            if ((u8)func_150ADA20() < 0x80U) {
                bits = 1;
            }
            if ((u8)func_150ADA20() < 0x80U) {
                bits |= 2;
            }
            sprite.flip = bits;
            if (first) {
                first = 0;
                gfx = func_15094F70(gfx, &D_80091840, 0, &sprite, 0, 0, 0, 2, 3);
            }
            gfx = func_15095A48(gfx, &sprite, x, y);
            t = ENT[i].timer;
            t -= D_800BE9E4;
            if (t < 0) {
                t = 0;
            }
            ENT[i].timer = t;
        }
        i++;
    } while (i != 6);
    return gfx;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_BC510/func_1509093C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_BC510/func_150911F4.s")

Gfx* func_15091534(Gfx* arg0, struct257 *arg1, u8 *arg2) {
    s32 temp_v0;

    *arg2 = 0;
    temp_v0 = func_1510D0EC((s32)arg1 + (s32)D_D16, 0, 3, 0);

    if (temp_v0 != (s32)0x80000000) {
        gDPSetTextureImage(arg0++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, temp_v0);
        gDPSetTile(arg0++, G_IM_FMT_CI, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD);
        gDPLoadSync(arg0++);
        gDPLoadBlock(arg0++, G_TX_LOADTILE, 0, 0, 1023, 0);
        gDPPipeSync(arg0++);
        gDPSetTile(arg0++, G_IM_FMT_CI, G_IM_SIZ_4b, 4, 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD);
        gDPSetTileSize(arg0++, G_TX_RENDERTILE, 0, 0, 0xFC, 0xFC);
        gDPSetTextureImage(arg0++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, temp_v0 + 0x800);
        gDPLoadSync(arg0++);
        gDPLoadTLUTCmd(arg0++, 6, 15);
        gDPSetOtherMode(arg0++, 0x00AC3F, 0x00504244);

        *arg2 = 1;
    }

    return arg0;
}

void func_150916B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    volatile s32 pad[2];
    s32 seconds;
    s32 hundredths;

    if (arg2 >= 0x57030) {
        arg2 = 0x57030;
    }

    hundredths = ((arg2 % 60) * 100) / 60;
    if (hundredths >= 100) {
        hundredths = 99;
    }

    arg0 -= 8;
    if (arg2 >= 0) {
        seconds = arg2 / 60;
        func_15042D94((arg0 - (((seconds / 60) >= 10) ? 5 : 0)) - 8, arg1, (u8)arg3, &D_8009DCC0, seconds / 60);
        func_15042D94(arg0, arg1, arg3, &D_8009DCC4, seconds % 60);
        func_15042D94(arg0 + 0x10, arg1, arg3, &D_8009DCCC, hundredths);
        return;
    }

    func_15042D94(arg0 - 8, arg1, (u8)arg3, &D_8009DCD4);
    func_15042D94(arg0, arg1, arg3, &D_8009DCD8);
    func_15042D94(arg0 + 0x10, arg1, arg3, &D_8009DCE0);
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_BC510/func_150918EC.s")

void func_15093818(s32 arg0) {
    if ((arg0 != 0) && (D_800D2458 == 0)) {
        D_800D2458 = 1;
        func_15093878();
        return;
    }
    if ((arg0 == 0) && (D_800D2458 != 0)) {
        D_800D2458 = 0;
    }
}

void func_15093878(void) {
    D_800D2448 = func_1518C900(0xBA);
    D_800D244C = allocate_memory(0x80, 1, 1, 0);
}

extern s32 D_800D2450;
extern u8 D_11AD[];
void func_150A7D00(s32, f32, f32, f32);

GameBC510Command *func_150938BC(GameBC510Command *arg0) {
    s32 x = 120;
    s32 y = 129;
    s32 z = -303;
    s32 i;
    s32 val;
    s32 tex;

    val = (D_800D2450 / 30) % 60;
    i = 4;
    do {
        tex = func_1510D0EC((s32)D_11AD + (val % 10), 0, 3, 0);
        if (tex == (s32)0x80000000) {
            return arg0;
        }
        gSPSegment((Gfx *)arg0++, i, tex);
        if (i == 3) {
            val = (D_800D2450 / 30) / 60;
        } else {
            val = val / 10;
        }
        i--;
    } while (i != 0);

    func_150A7D00((D_800BE9C0 << 6) + D_800D244C, x, y, z);
    gSPMatrix((Gfx *)arg0++, (D_800BE9C0 << 6) + D_800D244C, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList((Gfx *)arg0++, D_800D2448);
    return arg0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_BC510/func_15093B58.s")
