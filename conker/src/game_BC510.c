#include <ultra64.h>

#define func_15048A40 func_15048A40_hdr
#define func_150ADA20 func_150ADA20_hdr
#define func_150A7CB0 func_150A7CB0_hdr
#define func_1502B7F0 func_1502B7F0_hdr
#include "functions.h"
#undef func_15048A40
#undef func_150ADA20
#undef func_150A7CB0
#undef func_1502B7F0
void func_1509093C(void);
void func_150A7CB0(f32 mtx[4][4], f32 x, f32 y, f32 z);
#define D_800D24C8 D_800D24C8_hdr
#define D_800D2460 D_800D2460_hdr
#define D_800D2890 D_800D2890_hdr
#define D_800D2590 D_800D2590_hdr
#define D_800D2454 D_800D2454_hdr
#define D_800D23D0 D_800D23D0_hdr
#include "variables.h"
#undef D_800D24C8
#undef D_800D2460
#undef D_800D2890
#undef D_800D2590
#undef D_800D2454
#undef D_800D23D0
extern s32 func_150ADA20(void);
void func_1508F9F4(void);
void func_1508F0D4(void);
void func_1508F7BC(void);
extern u8 D_800CC49A;
extern f32 D_800D2890[3];
extern Mtx D_800D2590[6][2];
extern u16 D_800D2454;
extern Mtx D_800D23D0;

extern s32 func_1510D0EC(s32, s32, s32, s32);
void func_15042D94(s32, s32, u8, s32 *, ...);
extern u8 D_D16[];

/* One rising bubble sprite (drawn by func_15090630). */
typedef struct Bubble {
    s16 x;
    s16 y;
    s16 originX;
    s16 size;
    s16 radius;
    s16 timer;
    u8 accel;
    u8 state;
    u8 angle;
    u8 phase;
} Bubble;

extern Bubble D_800D2460[6];

/* Resets all bubbles to the idle state. */
void func_1508F060(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        D_800D2460[i].state = 0;
    }
    D_800D24C0 = 0;
}

/* Per-frame update of the block stack, the bubbles and the water surface. */
void func_1508F0A4(void) {
    func_1508F0D4();
    func_1508F9F4();
    func_1509093C();
}

/* Moves the debris particles, adds/removes blocks to follow D_800CC49A and moves the stack. */
void func_1508F0D4(void) {
    s32 timer;
    s32 i;
    s32 j;
    s32 pad1; // matching: unused stack slot in the original frame
    s32 count;
    struct256 *node;
    struct256 **link;
    f32 target;
    f32 base;
    f32 cur;
    f32 inc;

    link = &D_800D245C;
    node = D_800D245C;
    while (node != NULL) {
        node->unk94 -= 5.0f;
        node->unk84 += node->unk90;
        node->unk88 += node->unk94;
        node->unk8C += node->unk98;
        node->unk9C += node->unkA8;
        node->unkA0 += node->unkAC;
        node->unkA4 += node->unkB0;
        timer = node->unkB4;
        timer -= D_800BE9E4;
        if (timer <= 0) {
            *link = node->unk80;
            func_100043B4((s32 *)node, 3);
        } else {
            node->unkB4 = timer;
            link = &node->unk80;
        }
        node = node->unk80;
    }

    if (D_800CC49A < D_800D2457) {
        // matching: an empty if here splits the block; IDO's layout of the branch below depends on it
        if (node != NULL) {
        }
        D_800D2444 = 240;
        while (D_800CC49A != D_800D2457) {
            D_800D2457--;
            if (D_800D2428[0] == (f32)D_8009DCB4[3]) {
                func_1508F7BC();
            }
        }
        func_10010F30(0x1A6, 0x7FFF, 0x40, 0, 0);
    } else if (D_800D2457 < D_800CC49A) {
        D_800D2444 = 240;
        while (D_800CC49A != D_800D2457) {
            D_800D2457++;
            if (D_800D2428[0] == (f32)D_8009DCB4[3]) {
                func_1508F9C4();
            }
        }
    }
    if (D_800D2444 != 0) {
        cur = D_800D2428[0];
        target = D_8009DCB4[3];
        if (cur == target) {
            D_800D2444 -= D_800BE9E4;
            if (D_800D2444 <= 0) {
                D_800D2444 = 0;
            }
            for (j = 0; j < 4; j += 2) {
                for (i = 0; i < D_800BE9E4; i++) {
                    D_800D2890[j] += D_800D2438[j] * -0.1f;
                    D_800D2890[j] *= 0.85f;
                    D_800D2438[j] += D_800D2890[j] * (f32)D_800BE9E4;
                }
            }
        } else if (D_800D2457 < D_800D2456) {
            count = D_800D2456 - D_800D2457;
            D_800D2428[0] += 50.0f * (f32)D_800BE9E4;
            if (target <= D_800D2428[0]) {
                D_800D2428[0] = target;
                for (i = 0; i < count; i++) {
                    func_1508F7BC();
                }
                func_10010F30(0x1A6, 0x7FFF, 0x40, 0, 0);
            }
        } else {
            count = D_800D2457 - D_800D2456;
            D_800D2428[0] += 50.0f * (f32)D_800BE9E4;
            if (target <= D_800D2428[0]) {
                D_800D2428[0] = target;
                for (i = 0; i < count; i++) {
                    func_1508F9C4();
                }
            }
        }
    } else {
        cur = D_800D2428[0];
        base = D_8009DCB4[0];
        if (D_800D2428[0] != (f32)D_8009DCB4[0]) {
            target = ((f32)D_8009DCB4[3] - D_800D2428[0]) * 0.1f;
            if (target < 0.01f) {
                target = 0.01f;
            }
            D_800D2428[0] -= target * (f32)D_800BE9E4;
            if (D_800D2428[0] < base) {
                D_800D2428[0] = base;
            }
        }
    }
    for (i = 0; i < 6; i++) {
        if (D_800D2410[i] != 1.0f) {
            inc = D_800BE9E4 * 0.03f;
            D_800D2410[i] += inc;
            if (D_800D2410[i] > 1.0f) {
                D_800D2410[i] = 1.0f;
            }
        }
    }
}

/* Removes the top block and spawns a debris particle at its position. */
void func_1508F7BC(void) {
    f32 mtx[4][4];
    f32 unused;
    struct256 *node;

    D_800D2456 -= 1;
    D_800D2890[0] = func_150ADA20() % 5U;
    D_800D2890[2] = sqrtf(25.0f - D_800D2890[0] * D_800D2890[0]);
    node = allocate_memory(sizeof(*node), 1, 1, 0);
    node->unk80 = D_800D245C;
    D_800D245C = node;
    guMtxL2F(mtx, &D_800D2590[D_800D2456][D_800BE9C0 ^ 1]);
    func_1503E5F8(mtx, &node->unk84, &node->unk88, &node->unk8C, &node->unk9C, &node->unkA0, &node->unkA4,
                  &unused, &unused, &unused);
    node->unk90 = (func_150ADA20() & 0xFU) + 10;
    node->unk94 = (func_150ADA20() & 0x1FU) + 20;
    node->unk98 = 0.0f;
    node->unkA8 = func_150ADA20() & 0xFU;
    node->unkAC = func_150ADA20() & 0xFU;
    node->unkB0 = func_150ADA20() & 0xFU;
    node->unkB4 = 120;
}

/* Adds a block on top; it grows in from zero scale. */
void func_1508F9C4(void) {
    D_800D2410[D_800D2456++] = 0.0f;
}

extern f32 func_15048A40(u8 arg0);

/* Spawns and animates the bubbles while player 1 has been underwater for a while. */
void func_1508F9F4(void) {
    s32 i;
    s32 v;
    s32 spawn;
    s32 dt;
    s32 duration;
    s32 size;
    s32 air;
    s32 j;
    s32 y;

    air = D_800D24C0;
    if (air != 0) {
        air -= D_800BE9E4;
        if (air < 0) {
            air = 0;
        }
        D_800D24C0 = air;
    }
    if (D_800CC2D0[0].in_water) {
        air = D_800CC2D0[0].unkB2;
    } else {
        air = 0;
    }
    if (air > 90) {
        duration = air / 240;
        if (duration > 6) {
            duration = 6;
        }
        duration = 6 - duration;
        duration = duration * 9;
        if (duration == 0) {
            spawn = 0;
        }
    } else {
        spawn = 0;
    }
    for (i = 0; i < 6; i++) {
        Bubble *p = &D_800D2460[i];
        if (p->state == 2) {
            v = p->timer;
            v -= D_800BE9E4;
            if (v <= 0) {
                v = 6;
                p->state = 3;
            }
            p->timer = v;
        }
        switch (p->state) {
        case 0:
    // matching: the two labels split case 0 into extra blocks; IDO then drops a bnel at the loop end
    LX: ;
        l0:
            if (spawn != 0 && D_800D24C0 == 0) {
                D_800D24C0 = 10;
                p->state = 1;
                p->x = 252;
                p->y = 96;
                p->size = 0;
                p->accel = 0;
                p->angle = func_150ADA20();
                p->radius = 0x500;
                p->originX = 252 - ((s32)(p->radius * func_15048A40(p->angle)) >> 8);
                p->phase = 0;
                p->timer = duration;
            }
            break;
        case 1:
            v = p->size;
            v += D_800BE9E4 * 200;
            if (v >= 0x666) {
                p->state = 2;
                v = 0x666;
            }
            p->size = v;
            break;
        case 2:
            dt = D_800BE9E4;
            j = 0;
            v = p->accel;
            if (v < 32) {
                v = v + dt;
                if (v > 32) {
                    v = 32;
                }
                p->accel = v;
            }
            y = p->y;
            y -= (dt * p->accel) >> 4;
            // matching: volatile keeps the size load after the y store
            *(volatile s16 *)&p->y = y;
            size = *(volatile s16 *)&p->size;
            if (dt > 0) {
                do {
                    j++;
                    size = (size * 0x104) >> 8;
                } while (j < dt);
            }
            p->size = size;
            p->angle += dt * 5;
            p->radius += dt * 30;
            p->x = ((s32)(func_15048A40(p->angle) * p->radius) >> 8) + p->originX;
            p->phase += D_800BE9E4 * 10;
        // matching: label steers block layout of case 2's exit
        lbl:
            break;
        case 3:
            if (p->timer == 0) {
                p->state = 0;
            }
            break;
        }
    }
}
Gfx *func_150900F0(Gfx *, s32);
Gfx *func_150918EC(Gfx *);
Gfx *func_150938BC(Gfx *);
Gfx *func_15093B58(Gfx *, s32, s16 *, s32, u8);
Gfx *func_15090630(Gfx *);
Gfx *func_150911F4(Gfx *);
Gfx *func_15094F40(Gfx *);
Gfx *func_15096934(Gfx *);
void func_1518CA04(s32);
extern f32 D_8008FE1C;
extern s32 D_800BEBA4;
extern Gfx *D_800BE9C8[];

/* Builds this level's overlay display list: blocks, water, bubbles and HUD. Returns the
 * original pointer (drops everything) if the frame's display list buffer would overflow. */
Gfx *func_1508FD38(register Gfx *gfx) {
    Gfx *start;
    s32 i;
    s32 step;
    s32 overflow;
    s16 rows[33];

    start = gfx;
    if (D_800BE9F0 == 0x1D) {
        return gfx;
    }
    gSPPerspNormalize(gfx++, D_800D2454);
    gSPMatrix(gfx++, &D_800D23D0, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    if (D_800BE616 != 0) {
        gfx = func_150918EC(gfx);
        // matching: the 0/1 flag variable; testing the comparison directly changes the code
        overflow = D_800BEBA4 < gfx - D_800BE9C8[D_800BE9C0] ? 1 : 0;
        if (overflow != 0) {
            return start;
        }
        return gfx;
    }
    if (D_800D2458 != 0) {
        gfx = func_150938BC(gfx);
    }
    if (D_800C35EA == 1) {
        return gfx;
    }
    if (D_8008CD00 != 0) {
        step = 0x400;
        if (D_8008FE1C != 1.0f) {
            step = (s32)((f32)step / D_8008FE1C);
            for (i = 0; i < 33; i++) {
                rows[i] = (s32)((f32)i * D_8008FE1C);
            }
        } else {
            // matching: one line; the loop body's line number steers as1's unrolled store order
            for (i = 0; i < 33; i++) rows[i] = i;
        }
        gfx = func_15093B58(gfx, step, rows, 0x2C30, 1);
    }
    if (D_800D2428[0] != (f32)D_8009DCB4[0]) {
        gfx = func_150900F0(gfx, 0);
    } else if (D_800D2588 != 0) {
        func_1518CA04(0xB8);
        D_800D2588 = 0;
    }
    gfx = func_150911F4(gfx);
    gfx = func_15094F40(gfx);
    gfx = func_15090630(gfx);
    gfx = func_15096934(gfx);
    overflow = D_800BEBA4 < gfx - D_800BE9C8[D_800BE9C0] ? 1 : 0;
    if (overflow != 0) {
        return start;
    }
    return gfx;
}
extern s16 D_8008FDCC;
extern s16 D_8009DC90[][3];
extern u8 D_800CC49A;
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);

/* Draws the stacked blocks and the debris particles. With `fixed` set, instead draws D_800CC49A
 * blocks at full scale at a fixed spot near D_8009DCB4[3..5], after patching the block display
 * list's combiner and other-mode commands. */
Gfx *func_150900F0(Gfx *gfx, s32 fixed) {
    s32 i;
    s32 count;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    f32 mtx[4][4];
    f32 base[4][4];
    struct256 *node;
    Gfx *dl;

    if (D_800D2588 == 0) {
        D_800D2588 = func_1518C900(0xB8);
    }
    gSPClearGeometryMode(gfx++, G_ZBUFFER | G_CULL_FRONT | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR);
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gfx++, G_CULL_BACK);
    gDPPipeSync(gfx++);
    gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, 255);
    if (fixed == 0) {
        gDPSetOtherMode(gfx++, 0x08AC0F, 0x55204C);
        gDPSetCombine(gfx++, 0x127E24, 0xFFFFF9FC);
        gDPSetEnvColor(gfx++, 0, 0, 0, 0);
    } else {
        gSPPerspNormalize(gfx++, D_800D2454);
        gSPMatrix(gfx++, &D_800D23D0, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
        gDPSetOtherMode(gfx++, 0x08AC0F, 0x5041CC);
        gDPSetCombine(gfx++, 0x12D225, 0xFFA7FFFF);
        gDPSetEnvColor(gfx++, 255, 255, 255, D_8008FDCC);
    }
    count = D_800D2456;
    dl = (Gfx *)D_800D2588;
    if (fixed != 0) {
        x = (f32)D_8009DCB4[3] - 54.0f;
        y = (f32)D_8009DCB4[4] - 52.0f;
        z = (f32)D_8009DCB4[5] - 344.7000122f;
        count = D_800CC49A;
        func_150A8050(base, 50.0f, -20.0f, -20.0f);
        scale = 1.0f;
        {
            Gfx *cmd = dl;
            u32 op;
            do {
                op = (u8)(cmd->words.w0 >> 24);
                switch (op) {
                case G_SETCOMBINE:
                    cmd->words.w0 = 0;
                    break;
                case G_RDPSETOTHERMODE:
                    cmd->words.w1 = 0x5041C8;
                    break;
                }
                cmd++;
            } while (op != G_ENDDL);
        }
    } else {
        x = D_800D2428[0];
        y = D_800D2428[1];
        z = D_800D2428[2];
        func_150A8050(base, D_800D2438[0] + 50.0f, -20.0f, D_800D2438[2] + -20.0f);
    }
    func_15043EC8(base, 1.0f, 1.0f, 1.0f, x, y, z);
    for (i = 0; i < count; i++) {
        if (fixed == 0) {
            scale = D_800D2410[i];
        }
        func_150A7CB0(mtx, scale, scale, scale);
        mtx[3][0] = D_8009DC90[i][0];
        mtx[3][1] = D_8009DC90[i][1];
        mtx[3][2] = D_8009DC90[i][2];
        func_150A7A48(mtx, base, mtx);
        guMtxF2L(mtx, &D_800D2590[i][D_800BE9C0]);
        gSPMatrix(gfx++, &D_800D2590[i][D_800BE9C0], G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(gfx++, dl);
    }
    if (fixed == 0) {
        for (node = D_800D245C; node != NULL; node = node->unk80) {
            func_15043E68((Mtx *)node->pad0 + D_800BE9C0, node->unk9C, node->unkA0, node->unkA4, node->unk84,
                          node->unk88, node->unk8C);
            gSPMatrix(gfx++, (Mtx *)node->pad0 + D_800BE9C0, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(gfx++, dl);
        }
    }
    return gfx;
}
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
Gfx *func_15094F70(Gfx *, BC510Tex *, s32, BC510Sprite *, s32, s32, s32, s32, s32);
Gfx *func_15095A48(Gfx *, BC510Sprite *, f32, f32);

#define ENT ((BC510Entry *)D_800D2460)
Gfx *func_15090630(Gfx *gfx) {
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
    { Gfx *c = gfx++; c->words.w0 = 0xE7000000; c->words.w1 = 0; }
    { Gfx *c = gfx++; c->words.w0 = 0xFCFFB5FF; c->words.w1 = 0xFFFCFE38; }
    { Gfx *c = gfx++; c->words.w0 = 0xEF102C0F; c->words.w1 = 0x0C184244; }
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
typedef struct WaterVtx {
    s16 ob[3];
    u16 flag;
    s16 tc[2];
    u8 cn[4];
} WaterVtx;

typedef struct WaterBase {
    s16 ob[3];
    u8 cn[4];
} WaterBase;

typedef struct WaterStage {
    Mtx mtx[2];
    s32 dl;
    WaterVtx *vtx[2];
    WaterBase *base;
    WaterBase *target;
    f32 unk94;
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    f32 unkAC;
    u16 tex[4];
    s16 timer;
    u8 alpha;
    u8 count;
} WaterStage;

extern WaterStage D_800D24C8;
#define WATER D_800D24C8
extern s32 func_1502FE10(s32 arg0, s32 *arg1, u8 *arg2, s32 *arg3, s32 *arg4, s32 *arg5, s32 *arg6);
extern s32 func_1502B7F0(s32 *, s32, s32, s32);
extern s32 D_800D2558[];
extern u16 D_800CC382;
extern u16 D_8009025A;
extern u16 D_80090262;
extern u16 D_80090266;
extern u16 D_8009026E;
extern u16 D_80090272;

void func_1509093C(void) {
    WaterVtx *dst;
    WaterBase *base;
    WaterBase *target;
    s32 i;
    f32 t;
    s32 from;
    s32 level;
    f32 lo;
    f32 hi;
    s32 v;
    s32 *end;
    u8 sp73;
    WaterVtx *start;
    s32 sp68;
    s32 sp64;
    s32 sp60;
    s32 a;
    s32 r;
    s32 g;
    u16 b;
    f32 dt;
    s32 r2;
    s32 g2;
    s32 b2;
    s32 cb;

    if (D_800CC2D0->in_water) {
        level = D_800CC2D0->unkB2;
    } else {
        level = 0;
    }
    if (level > 0x5A) {
        if (WATER.alpha == 0) {
            WATER.alpha = 0xFF;
            WATER.unk94 = 0.0f;
            WATER.unk98 = 0.01999999955f;
            WATER.unkA0 = 0;
            WATER.unkA4 = 3.200000048f;
            WATER.unkA8 = 0;
            WATER.unkAC = 0;
            WATER.timer = (func_150ADA20() & 0x3F) + 0x3C;
        }
        from = D_800CC382;
    } else {
        WATER.alpha = 0;
        return;
    }
    {
        WATER.unk9C = (f32)(u32)from / 720.0f;
        if (WATER.unk9C > 1.0f) {
            WATER.unk9C = 1.0f;
        }
        if (level < 0x5A1) {
            WATER.unk94 += WATER.unk98 * D_800BE9E4;
            lo = WATER.unk9C - 0.200000003f;
            if (lo < 0.0f) {
                lo = 0.0f;
            }
            if (WATER.unk94 <= lo) {
                WATER.unk94 = (lo + lo) - WATER.unk94;
                WATER.unk98 = -WATER.unk98;
            } else {
                hi = WATER.unk9C + 0.200000003f;
                if (hi > 1.0f) {
                    hi = 1.0f;
                }
                if (hi <= WATER.unk94) {
                    WATER.unk94 = (hi + hi) - WATER.unk94;
                    WATER.unk98 = -WATER.unk98;
                }
            }
        }
        if (WATER.dl == 0) {
            func_1502FE10(0xB9, (s32 *)&end, &sp73, (s32 *)&start, &sp68, &sp64, &sp60);
            WATER.dl = *end;
            WATER.count = ((u32)end - (u32)start) >> 4;
            WATER.vtx[0] = start;
            WATER.vtx[1] = (WaterVtx *)allocate_memory(WATER.count * 16, 1, 2, 0);
            bcopy(start, WATER.vtx[1], WATER.count * 16);
            WATER.base = (WaterBase *)allocate_memory(WATER.count * sizeof(WaterBase), 1, 0, 0);
            target = WATER.base;
            for (i = 0; i < WATER.count; i++) {
                target[i].ob[0] = start[i].ob[0];
                target[i].ob[1] = start[i].ob[1];
                target[i].ob[2] = start[i].ob[2];
                target[i].cn[0] = start[i].cn[0];
                target[i].cn[1] = start[i].cn[1];
                target[i].cn[2] = start[i].cn[2];
                target[i].cn[3] = start[i].cn[3];
            }
            for (i = 1; i < 2; i++) {
                func_1502B7F0(&D_800D2558[i - 1], 2, 0x18, i - 1);
            }
        }
        base = WATER.base;
        dst = WATER.vtx[D_800BE9C0];
        target = WATER.target;
        t = WATER.unk94;
        for (i = 0; i < WATER.count; i++) {
            dst[i].ob[0] = (f32)base[i].ob[0] + ((f32)target[i].ob[0] - (f32)base[i].ob[0]) * t;
            dst[i].ob[1] = (f32)base[i].ob[1] + ((f32)target[i].ob[1] - (f32)base[i].ob[1]) * t;
            dst[i].ob[2] = (f32)base[i].ob[2] + ((f32)target[i].ob[2] - (f32)base[i].ob[2]) * t;
        }
        if (level < 480) {
            b = D_8009026E;
            WATER.tex[3] = b;
            WATER.tex[2] = b;
            b = D_8009025A;
            WATER.tex[1] = b;
            WATER.tex[0] = b;
        } else if (level < 960) {
            b = D_80090272;
            WATER.tex[3] = b;
            WATER.tex[2] = b;
            b = D_8009025A;
            WATER.tex[1] = b;
            WATER.tex[0] = b;
        } else if (level < 1200) {
            b = D_80090272;
            WATER.tex[3] = b;
            WATER.tex[2] = b;
            b = D_80090266;
            WATER.tex[1] = b;
            WATER.tex[0] = b;
        } else {
            b = D_80090272;
            WATER.tex[3] = b;
            WATER.tex[2] = b;
            b = D_80090262;
            WATER.tex[1] = b;
            WATER.tex[0] = b;
        }
        v = (level - 720) * 255;
        a = v / 480;
        if (a < 0) {
            a = 0;
        } else if (a >= 256) {
            a = 255;
        }
        r = ((a * -151) >> 8) + 255;
        g = ((a * -164) >> 8) + 255;
        cb = 255;
        r2 = ((a * -97) >> 8) + 255;
        g2 = ((a * -94) >> 8) + 255;
        b2 = ((a * -91) >> 8) + 255;
        a = 0;
        i = 0;
        if (i < WATER.count) {
            r |= 0;
            g |= 0;
            cb |= 0;
            do {
                if (i < 0xD0) {
                    dst[i].cn[0] = (base[i].cn[0] * r) >> 8;
                    dst[i].cn[1] = (base[i].cn[1] * g) >> 8;
                    dst[i].cn[2] = (base[i].cn[2] * cb) >> 8;
                } else if (i >= 0xDC) {
                    dst[i].cn[0] = r;
                    dst[i].cn[1] = g;
                    dst[i].cn[2] = cb;
                } else {
                    dst[i].cn[0] = r2;
                    dst[i].cn[1] = g2;
                    dst[i].cn[2] = b2;
                }
                i++;
            } while (i < WATER.count);
        }
        dt = D_800BE9E4;
        if (v > 0x2CE2E) {
            WATER.unkAC = 0.0f;
        } else if (v > 0x1DF1E) {
            if (fabsf(WATER.unkAC) < 0.6000000238f) {
                WATER.unkAC = 0.6000000238f;
            }
        } else if (WATER.timer != 0) {
            from = WATER.timer - D_800BE9E4;
            if (from <= 0) {
                from = 0;
                WATER.unkAC = 0.200000003f;
                WATER.unkA8 = dt * WATER.unkAC;
            }
            WATER.timer = from;
        }
        WATER.unkA8 += WATER.unkAC * dt;
        if (WATER.unkA8 > 2.0f) {
            WATER.unkA8 = 4.0f - WATER.unkA8;
            WATER.unkAC = -WATER.unkAC;
        } else if (WATER.unkA8 < -2.0f) {
            WATER.unkA8 = -4.0f - WATER.unkA8;
            WATER.unkAC = -WATER.unkAC;
        } else if (WATER.timer == 0 && v < 0x1DE20 && WATER.unkA8 >= 0 && WATER.unkA8 < WATER.unkAC * dt) {
            WATER.timer = (func_150ADA20() & 0x3F) + 0x3C;
            WATER.unkA8 = 0.0f;
            WATER.unkAC = 0.0f;
        }
        if (v >= 0x2CD30 && WATER.unkA0 > -22.0f) {
            WATER.unkA4 -= 0.25f * D_800BE9E4;
            WATER.unkA0 += WATER.unkA4;
        }
    }
}

typedef struct BC510Stage {
    Mtx mtx[2];
    u32 dl;
    u32 seg1[2];
    u8 pad8C[0xA0 - 0x8C];
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    u8 padAC[4];
    u16 tex[4];
    u8 padB8[2];
    u8 alpha;
    u8 padBB[5];
} BC510Stage;
extern u8 D_80083440[];
extern u8 D_800835C0[];
extern u8 D_80084160[];
extern u8 D_80084190[];
#define STAGE (*(BC510Stage *)&D_800D24C8)

Gfx *func_150911F4(Gfx *gfx) {
    s32 sp90[6];
    f32 y;
    f32 x;
    s32 t;

    if (STAGE.alpha == 0) {
        return gfx;
    }
    { Gfx *c = gfx++; c->words.w0 = 0xDE000000; c->words.w1 = (u32)D_80084160; }
    { Gfx *c = gfx++; c->words.w0 = 0xD9FFFFFF; c->words.w1 = 0x400; }
    { Gfx *c = gfx++; c->words.w0 = 0xD9F1FDFE; c->words.w1 = 0; }
    { Gfx *c = gfx++; c->words.w0 = 0xD9BDFFFF; c->words.w1 = 0; }
    if (STAGE.alpha < 0xFF) {
        Gfx *c = gfx++; c->words.w0 = 0xDB060020; c->words.w1 = (u32)D_80083440;
    } else {
        Gfx *c = gfx++; c->words.w0 = 0xDB060020; c->words.w1 = (u32)D_800835C0;
    }
    y = -11.0f; y += STAGE.unkA8;
    x = -1.0f - STAGE.unkA0;
    func_15043D90(&STAGE.mtx[D_800BE9C0], x, y, 0.0f,
                  4.050000191f, 4.050000191f, 4.050000191f, 667.0f, 152.0f, -1668.0f);
    { Gfx *c = gfx++; c->words.w0 = 0xDA380003; c->words.w1 = (u32)&STAGE.mtx[D_800BE9C0]; }
    { Gfx *c = gfx++; c->words.w0 = 0xF8000000; c->words.w1 = 0xFF; }
    { Gfx *c = gfx++; c->words.w0 = 0xFA00F200; c->words.w1 = 0; }
    { Gfx *c = gfx++; c->words.w0 = 0xFB000000; c->words.w1 = (STAGE.alpha & 0xFF); }
    { Gfx *c = gfx++; c->words.w0 = 0xDB06000C; c->words.w1 = (u32)&STAGE.mtx[D_800BE9C0]; }
    { Gfx *c = gfx++; c->words.w0 = 0xDB060004; c->words.w1 = STAGE.seg1[D_800BE9C0]; }
    if (STAGE.tex[0]) {
        Gfx *c;
        t = func_1510D0EC(STAGE.tex[0], (s32)sp90, 3, 0);
        c = gfx++; c->words.w0 = 0xDB060018; c->words.w1 = t;
    }
    if (STAGE.tex[1]) {
        Gfx *c;
        t = func_1510D0EC(STAGE.tex[1], (s32)sp90, 3, 0);
        c = gfx++; c->words.w0 = 0xDB06001C; c->words.w1 = t;
    }
    if (STAGE.tex[2]) {
        Gfx *c;
        t = func_1510D0EC(STAGE.tex[2], (s32)sp90, 3, 0);
        c = gfx++; c->words.w0 = 0xDB060028; c->words.w1 = t;
    }
    if (STAGE.tex[3]) {
        Gfx *c;
        t = func_1510D0EC(STAGE.tex[3], (s32)sp90, 3, 0);
        c = gfx++; c->words.w0 = 0xDB06002C; c->words.w1 = t;
    }
    { Gfx *c = gfx++; c->words.w0 = 0xDE000000; c->words.w1 = STAGE.dl; }
    { Gfx *c = gfx++; c->words.w0 = 0xDE000000; c->words.w1 = (u32)D_80084190; }
    return gfx;
}

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
typedef struct HudView {
    u8 pad0[0x24];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x180 - 0x34];
} HudView;

typedef struct HudBars {
    u8 b[4];
} HudBars;

typedef struct HudScore {
    u8 pad0[0x46];
    s16 score[4];
} HudScore;

typedef struct HudNames {
    u8 pad0[0x20];
    char *name[1];
} HudNames;

typedef struct HudTime {
    u8 pad0[0x1C];
    s32 unk1C;
    s32 unk20;
    u8 pad24[3];
    s8 unk27;
    u8 pad28[0x84 - 0x28];
} HudTime;

#define HUD_VIEWS ((HudView *)D_800BE628)
#define HUD_U8(p, off) (*(u8 *)((u8 *)(p) + (off)))
#define HUD_S8(p, off) (*(s8 *)((u8 *)(p) + (off)))
#define HUD_U16(p, off) (*(u16 *)((u8 *)(p) + (off)))
#define HUD_GW(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (a); _g->words.w1 = (b); }

extern HudBars D_800873B0;
extern Gfx D_80087390[];
extern u8 D_800873B4[];
extern u8 D_800873BC[];
extern u8 D_800873C4[];
extern u8 D_800873C8[];
extern u16 D_8008FDC0;
extern HudScore *D_8008FDD4;
extern s8 D_800E0AC0[];
extern HudNames *D_800E0BD8;
extern HudTime *D_800872A0;
extern s8 D_800E0BE7;
extern u8 D_CF1[];
extern u8 D_CF2[];
extern u8 D_D13[];
extern u8 D_D14[];
void func_150916B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

Gfx *func_150918EC(Gfx *gfx) {
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    s32 idx;
    s32 h;
    s32 left;
    s32 t;
    s32 n;
    s32 s;
    s32 v;
    s32 y2;
    s32 width;
    s32 lap_total;
    s32 right;
    struct126 *a1;
    struct127 *p;
    s32 w;
    s32 extra;
    s32 laps;
    s32 lap;
    s32 y3;
    s32 F3c;
    s32 step;
    u8 ok;
    u8 loaded;
    u8 mode;
    HudBars bars;
    s8 grenades;
    s16 rows[33];

    mode = 0;
    grenades = 0;
    bars = D_800873B0;
    width = HUD_VIEWS->unk30 - HUD_VIEWS->unk2C;
    gSPDisplayList((Gfx *)gfx++, D_80087390);
    func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
    step = 0x400;
    if (D_8008FE1C != 1.0f) {
        step = (s32)((f32)step / D_8008FE1C);
        for (j = 0; j < 33; j++) *(rows + j) = (s32)((f32)j * D_8008FE1C);
    } else {
        for (j = 0; j < 33; j++) *(rows + j) = j;
    }

    if (D_8008FDC0 & 0xC02) {
        loaded = 0;
        ok = 0;
        for (i = 0; i <= D_80082FA0; i++) {
            if (D_80087270[i] != 10) {
                y3 = HUD_VIEWS[i].unk2C;
                y = HUD_VIEWS[i].unk24 + rows[6];
                if (D_8008FDC0 & 2) {
                    if (!loaded) {
                        n = func_1510D0EC((s32)D_D14, 0, 3, 0);
                        loaded = 1;
                        if (n != (s32)0x80000000) {
                            HUD_GW(gfx++, 0xFD180000, n);
                            HUD_GW(gfx++, 0xF5180000, 0x07094250);
                            HUD_GW(gfx++, 0xE6000000, 0);
                            HUD_GW(gfx++, 0xF3000000, 0x073FF000);
                            HUD_GW(gfx++, 0xE7000000, 0);
                            HUD_GW(gfx++, 0xF5181000, 0x00094250);
                            HUD_GW(gfx++, 0xF2000000, 0x0007C07C);
                            ok = 1;
                        }
                        HUD_GW(gfx++, 0xEF002C3F, 0x00504244);
                        HUD_GW(gfx++, 0xE7000000, 0);
                        HUD_GW(gfx++, 0xFB000000, 0xFFFFFF80);
                    }
                    x = rows[5] + y3;
                    n = func_150859AC(i, 3);
                    y2 = D_800873C4[n & 3] << 5; t = D_800873C8[n >> 2] << 5;
                    if (ok) {
                        gSPScisTextureRectangle(gfx++, x * 4, y * 4, (rows[5] + x) * 4, (rows[7] + y) * 4, 0, y2, t, step, step);
                    }
                }
                if (D_8008FDC0 & 0x400) {
                    x = (width >> 1) + y3 - 8;
                    if (D_8008FE1C != 1.0f) {
                        x -= 8;
                    }
                    func_15042D94(x, y, 0x41, (s32 *)"%d", D_8008FDD4->score[i]);
                }
                if (D_8008FDC0 & 0x800) {
                    x = (width >> 1) + y3 + 0x10;
                    if (D_8008FE1C != 1.0f) {
                        x += 0x10;
                    }
                    func_15042D94(x, y, 0x41, (s32 *)"(%s)", D_800E0BD8->name[D_800E0AC0[i]]);
                }
            }
        }
    }

    if (D_8008FDC0 & 0x14) {
        loaded = 0;
        ok = 0;
        for (i = 0; i <= D_80082FA0; i++) {
            if (D_80087270[i] != 10) {
                y3 = HUD_VIEWS[i].unk2C + rows[5];
                right = HUD_VIEWS[i].unk30 - rows[5];
                y = HUD_VIEWS[i].unk24 + rows[5];
                if (D_8008FDC0 & 4) {
                    p = &D_800CC2D0[i];
                    n = 1;
                    h = p->unk31C->unk75;
                    h &= 0x7F;
                    if (p->interaction_state == 0x1F) {
                        h = 0;
                    }
                    switch (h) {
                    case 0x9:
                        lap = 0;
                        break;
                    case 0x16:
                        lap = 1;
                        break;
                    case 0x14:
                    case 0x3F:
                        lap = 3;
                        break;
                    case 0x20:
                        lap = 5;
                        break;
                    case 0x18:
                    case 0x41:
                        lap = 6;
                        break;
                    case 0x15:
                        lap = 7;
                        break;
                    case 0x22:
                        lap = 9;
                        break;
                    case 0x23:
                        lap = 10;
                        break;
                    case 0x21:
                        lap = 12;
                        break;
                    case 0x24:
                        lap = 13;
                        break;
                    case 0x25:
                        lap = 14;
                        break;
                    case 0x26:
                    case 0x3A:
                        lap = -1;
                        if (p->health != 0) {
                            grenades = 1;
                        }
                        break;
                    case 0x38:
                        lap = -1;
                        break;
                    case 0x32:
                    case 0x33:
                        n = HUD_S8(p->unk31C, 0x1AE);
                        lap = 11;
                        break;
                    case 0x40:
                        lap = 4;
                        extra = -3;
                        break;
                    default:
                        if (D_800BE9F0 == 0x1A) {
                            lap = 4;
                            extra = 4;
                        } else {
                            lap = -1;
                        }
                        break;
                    }
                    if (p->unk13C != 0) {
                        lap = -1;
                    }
                    if (lap >= 0) {
                        j = 0;
                        if (!loaded) {
                            loaded = 1;
                            gfx = (Gfx *)func_15091534((Gfx *)gfx, NULL, &ok);
                        }
                        x = (right - rows[25]) * 4;
                        t = D_800873B4[lap / 3] << 5;
                        left = rows[D_800873BC[lap / 3]] * 4;
                        if (lap == 4) {
                            j = 4;
                            left -= 8;
                            t += extra << 5;
                        }
                        if (lap == 1) {
                            left -= 4;
                        }
                        do {
                            if (ok) {
                                gSPTextureRectangle(gfx++, x, y * 4 + j, rows[22] * 4 + x, y * 4 + left + j, 0, ((lap % 3) * 25) << 5, t, step, step);
                            }
                            x -= 0x48;
                        } while (--n > 0);
                    }
                }
                if (D_8008FDC0 & 0x10) {
                    if (!loaded) {
                        gfx = (Gfx *)func_15091534((Gfx *)gfx, NULL, &ok);
                    }
                    s = func_150859AC(i, 4);
                    x = rows[22] + y3;
                    h = ((s + 1) >> 1) * 5;
                    if (h != 0 && ok) {
                        gSPTextureRectangle(gfx++, (x - rows[h]) * 4, y * 4, x * 4, rows[4] * 4 + y * 4, 0, 0x620, 0xA0, step, step);
                    }
                    h = (s >> 1) * 5;
                    if (h != 0 && ok) {
                        gSPTextureRectangle(gfx++, (x - rows[h]) * 4, rows[5] * 4 + y * 4, x * 4, rows[9] * 4 + y * 4, 0, 0x620, 0xA0, step, step);
                    }
                }
            }
        }
    }

    if (D_8008FDC0 & 4) {
        u8 pad3a;
        s32 ex3;
        s32 pad3b;
        loaded = 0;
        ok = 0;
        for (i = 0; i <= D_80082FA0; i++) {
            if (D_80087270[i] != 10) {

                right = HUD_VIEWS[i].unk30 - 5.0f;
                y = HUD_VIEWS[i].unk24 + 5.0f;
                n = 1;
                lap = -1;
                ex3 = 0;
                if (D_800CC2D0[i].interaction_state == 0x1E) {
                    j = D_800CC2D0[i].unk31C->unk128;
                    if (j & 1) {
                        lap = 6;
                        F3c = 4;
                    } else if (j & 2) {
                        lap = 3;
                    } else if (j & 0x80) {
                        lap = 0;
                        F3c = 6;
                    } else if (j & 0x10) {
                        lap = 9;
                        F3c = -3;
                    }
                    if (lap >= 0 && lap != 3) {
                        if (F3c > 0) {
                            bars.b[i] = HUD_U16(D_800CC2D0[i].unk31C, 0x12C) >> F3c;
                        } else {
                            bars.b[i] = HUD_U16(D_800CC2D0[i].unk31C, 0x12C) << -F3c;
                        }
                        mode = 1;
                    }
                } else {
                    switch (ex3 = 3, D_800CC2D0[i].unk31C->unk75 & 0x7F) {
                    case 0x37:
                        lap = 7;
                        break;
                    case 0x38:
                        lap = 4;
                        break;
                    case 0x39:
                        lap = 10;
                        break;
                    case 0x30:
                    case 0x31:
                        n = HUD_S8(D_800CC2D0[i].unk31C, 0x1AE);
                        lap = 6;
                        break;
                    case 0x3E:
                        bars.b[i] = HUD_U16(D_800CC2D0[i].unk31C, 0x12C) >> 4;
                        mode = 2;
                        break;
                    }
                }
                if (lap >= 0) {
                    if (!loaded) {
                        loaded = 1;
                        gfx = (Gfx *)func_15091534((Gfx *)gfx, (struct257 *)1, &ok);
                    }
                    x = right - rows[25];
                    y2 = (lap % 3) * 25;
                    lap = lap / 3;
                    t = D_800873B4[lap] + ex3;
                    y2 <<= 5;
                    t <<= 5;
                    do {
                        if (ok) {
                            gSPTextureRectangle(gfx++, x * 4, y * 4, (x + rows[22]) * 4, (rows[D_800873BC[lap]] + y) * 4, 0, y2, t, step, step);
                        }
                        x -= 0x12;
                    } while (--n > 0);
                }
            }
        }
    }

    if (mode) {
        gDPPipeSync((Gfx *)gfx++);
        HUD_GW(gfx++, 0xEF002C3F, 0x0F0A4004);
        HUD_GW(gfx++, 0xFCFFFFFF, 0xFFFDF6FB);
        HUD_GW(gfx++, 0xFA000000, 0xFF0000FF);
        if (mode == 2) {
            laps = 15;
        } else {
            laps = 30;
        }
        for (i = 0; i <= D_80082FA0; i++) {
            if (bars.b[i]) {
                x = (s32)HUD_VIEWS[i].unk30 - laps;
                w = (s32)HUD_VIEWS[i].unk24 + 10;
                gDPFillRectangle((Gfx *)gfx++, x - bars.b[i], w, x, w + 4);
            }
        }
    }

    if (grenades) {
        s32 pad2;
        s32 pad3;
        s32 pad4;
        s32 F5b;
        s32 F5d;
        F5d = 0xE7000000;
        j = func_1510D0EC((s32)D_D13, 0, 3, 0);
        HUD_GW(gfx++, 0xFD100000, j);
        HUD_GW(gfx++, 0xF5100000, 0x07094250);
        HUD_GW(gfx++, 0xE6000000, 0);
        HUD_GW(gfx++, 0xF3000000, 0x073FF000);
        HUD_GW(gfx++, F5d, 0);
        HUD_GW(gfx++, 0xF5101000, 0x00094250);
        HUD_GW(gfx++, 0xF2000000, 0x0007C07C);
        HUD_GW(gfx++, F5d, 0);
        HUD_GW(gfx++, 0xEF002C3F, 0x00504244);
        for (i = 0; i <= D_80082FA0; i++) {
            j = D_800CC2D0[i].unk31C->unk75;
            if ((j &= 0x7F) == 0x26 || j == 0x3A) {
                x = (s32)HUD_VIEWS[i].unk30 - 15;
                x *= 4;
                w = (s32)HUD_VIEWS[i].unk24 + 5;
                w *= 4;
                if (0x1A == D_800BE9F0 || 0x3F == D_800BE9F0) {
                    F5d = 11;
                    F5b = 14;
                    y2 = 0x200;
                    v = 0x1E0;
                } else if (D_800CC2D0[i].id == 0x75 || D_800CC2D0[i].id == 0xB1) {
                    F5d = 5;
                    F5b = 20;
                    y2 = 0xC0;
                    v = 0x100;
                } else if (D_800CC2D0[i].id == 0x3B) {
                    F5d = 11;
                    F5b = 14;
                    y2 = 0x200;
                    v = 0x1E0;
                } else {
                    F5d = 8;
                    F5b = 9;
                    y2 = 0x240;
                    v = 0x60;
                }
                for (j = 0; j < D_800CC2D0[i].unk31C->grenade_count; j++) {
                    gSPScisTextureRectangle(gfx++, x, w, rows[F5d] * 4 + x, rows[F5b] * 4 + w, 0, y2, v, 0x400, step);
                    x -= 0x2C;
                }
            }
        }
    }

    if ((D_8008FDC0 & 0x80) && D_800872A0 != NULL) {
        lap_total = D_800E0BE7;
        for (i = 0; i <= D_80082FA0; i++) {
            w = (s32)HUD_VIEWS[i].unk24 + 5;
            func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
            if (D_800872A0[i].unk20 > 0) {
                x = (s32)HUD_VIEWS[i].unk2C;
                func_150916B4(x + 0x17, w + 0x19, D_800872A0[i].unk20, 0xC0);
            }
            x = (s32)HUD_VIEWS[i].unk2C + (width >> 1);
            lap = (s8)(D_800872A0[i].unk27 + 1);
            if (lap <= lap_total) {
                func_15042D94(x, w, 0xC1, (s32 *)"%s", D_800E0BD8->name[D_800E0AC0[i]]);
                func_15042D94((s32)HUD_VIEWS[i].unk2C + 0x1D, w + 0xE, 0xC1, (s32 *)"lap %d/%d", lap, lap_total);
                func_150916B4(x, (s32)HUD_VIEWS[i].unk28 - 0xC, D_800872A0[i].unk1C, 0xC0);
            }
        }
    }

    if (D_800BEAC1 == 0 && (D_8008FDC0 & 0x1000)) {
        s32 shift;
        s32 size;
        f32 scale;
        f32 ax;
        f32 az;
        scale = 0.01300000027f;
        i = 0;
        x = (s32)HUD_VIEWS->unk30 - rows[32];
        x *= 4;
        if (D_8008FE1C != 1.0f) {
            shift = 6;
            scale *= 1.559999943f;
            size = 0x63;
        } else {
            shift = 4;
            size = 0x40;
        }
        laps = (D_8008FDBC & 0x40) ? (s32)D_CF2 : (s32)D_CF1;
        right = func_1510D0EC(laps, 0, 3, 0);
        HUD_GW(gfx++, 0xFD700000, right);
        HUD_GW(gfx++, 0xF5700000, 0x07098260);
        HUD_GW(gfx++, 0xE6000000, 0);
        HUD_GW(gfx++, 0xF3000000, 0x077FF000);
        HUD_GW(gfx++, 0xE7000000, 0);
        HUD_GW(gfx++, 0xF5681000, 0x00098260);
        HUD_GW(gfx++, 0xF2000000, 0x000FC0FC);
        HUD_GW(gfx++, 0xEF002C3F, 0x00504244);
        for (i = 0; i < D_8008FD90; i++) {
            if (func_150859AC(i, 3) > 0) {
                w = rows[21] + ((s32)HUD_VIEWS[i].unk24 + rows[24]);
                w *= 4;
                gDPPipeSync((Gfx *)gfx++);
                HUD_GW(gfx++, 0xFB000000, 0xFFFFFF80);
                HUD_GW(gfx++, 0xFC129A25, 0xFF37FFFF);
                s = x - rows[32] * 4;
                idx = w - rows[32] * 4;
                gSPScisTextureRectangle(gfx++, s, idx, size * 4 + s, size * 4 + idx, 0, 0, 0, step, step);
                gDPPipeSync((Gfx *)gfx++);
                HUD_GW(gfx++, 0xFC62FEC5, 0xFFFFFBFD);
                for (laps = 0, p = D_800CC2D0; laps != 25; laps++, p++) {
                    if (laps == i || (laps < D_8008FD8C && func_150859AC(laps, 3) > 0) || (s8)p->pad320[8] != 0) {
                        gDPPipeSync((Gfx *)gfx++);
                        if (laps == i) {
                            HUD_GW(gfx++, 0xFB000000, 0x00FF00FF);
                        } else if (laps < D_8008FD8C) {
                            HUD_GW(gfx++, 0xFB000000, 0xFF0000FF);
                        } else {
                            HUD_GW(gfx++, 0xFB000000, 0xFFFFFFFF);
                        }
                        ax = p->x_position * scale;
                        az = p->z_position * scale;
                        v = (w + 3.400000095f) + (s32)az;
                        n = (x + 3.599999905f) + (s32)ax;
                        gSPScisTextureRectangle(gfx++, n - shift, v - shift, n + shift, v + shift, 0, 0, 0, 0x400, 0x400);
                    }
                }
            }
        }
    }
    if (D_800BEAC1 == 0 && (D_8008FDC0 & 0x20)) {
        gfx = func_15093B58(gfx, step, rows, 0x2C30, 0);
    }
    return gfx;
}

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

Gfx *func_150938BC(Gfx *arg0) {
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
extern u8 D_CF2[];
extern u8 D_800AB62C[][3];
extern u16 D_800E0BCC;
u8 func_15085DA8(f32 arg0);

Gfx *func_15093B58(Gfx *gfx, s32 step, s16 *rows, s32 mode, u8 big) {
    f32 scale;
    f32 zoom;
    f32 sinv;
    f32 cosv;
    f32 px;
    f32 pz;
    f32 angle;
    f32 dx;
    f32 radiusSq;
    f32 dz;
    s32 j;
    s32 i;
    s32 tex;
    s32 st;
    s32 off;
    s32 sz;
    s8 h2;
    s8 h;
    s32 other;

    scale = 0.01349999942f;
    radiusSq = 3136.0f;
    st = step;
    off = rows[16];
    if (big) {
        zoom = 1.329000115f;
    } else {
        zoom = 1.0f;
    }
    if (D_8008FD90 == 1) {
        scale = 0.02025000006f;
        radiusSq = 7744.0f;
        st = (step * 2) / 3;
        off = rows[24];
    }
    scale *= D_8008FE1C;
    sz = (D_8008FE1C == 1.0f) ? 4 : 6;
    radiusSq *= D_8008FE1C;
    tex = func_1510D0EC((s32)(D_CF2 + 1), 0, 3, 0);
    if (tex == (s32)0x80000000) {
        return gfx;
    }
    gDPSetTextureImage(gfx++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, tex);
    gDPSetTile(gfx++, G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD);
    gDPLoadSync(gfx++);
    gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 511, 0);
    gDPPipeSync(gfx++);
    gDPSetTile(gfx++, G_IM_FMT_IA, G_IM_SIZ_8b, 4, 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD);
    gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
    gDPSetOtherMode(gfx++, mode | 0xF, 0x00504244);

    for (i = 0; i < D_8008FD90; i++) {
        if (func_150859AC(i, 3) > 0) {
            s32 half;
            s32 dsdx;
            s32 x;
            s32 y;
            struct127 *obj;
            struct108 *cam;
            struct108 *c2;
            u8 *p;

            p = (u8 *)D_800BE628 + i * 0x180;
            x = (((s32)*(f32 *)(p + 0x30) - rows[7]) - off) * 4;
            y = ((s32)*(f32 *)(p + 0x24) + rows[21] + off) * 4;
            obj = &D_800CC2D0[i];
            if (big) {
                x -= 0x1C6;
                y -= 0x46;
            }
            gDPPipeSync(gfx++);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, 0x80);
            gDPSetCombine(gfx++, 0x129A25, 0xFF37FFFF);
            if (big) {
                half = (s32)((f32)(off * 4) * 0.8580000401f);
                dsdx = (s32)((f32)(s32)(f32)st * 1.165501118f);
            } else {
                half = off * 4;
                dsdx = st;
            }
            gSPScisTextureRectangle(gfx++, x - half, y - half, x + half, y + half, G_TX_RENDERTILE, 0, 0, dsdx, dsdx);
            gDPPipeSync(gfx++);
            gDPSetCombine(gfx++, 0x62FEC5, 0xFFFFFBFD);
            gDPSetEnvColor(gfx++, 0xFF, 0x00, 0x00, 0xFF);
            cam = &D_800DBFF0[i];
            angle = cam->unk37C * 0.01745327935f;
            sinv = sinf(angle);
            cosv = cosf(angle);
            if (obj->interaction_state != 0x25) {
                /* as1 (-g3) orders equally ranked instructions of a block by source line, lowest
                 * first. The target schedule (li; lwc1; lwc1; sw) needs the px/pz loads emitted
                 * before `other = -1` while that statement carries the lower line number. The
                 * numbers only have to exceed every real line of this file with
                 * line(other = -1) < line(px/pz); the lines after them stay monotonic. */
#line 20002
                px = obj->x_position; pz = obj->z_position;
#line 20000
                other = -1;
                h = func_15085DA8(obj->y_position);
            } else {
                c2 = obj->camera;
                obj = D_800CC2D0;
                other = 0;
                if (c2 != NULL && c2->unk23D == 3) {
                    other = 1;
                }
                obj += other;
                px = obj->x_position;
                pz = obj->z_position;
                h = func_15085DA8(obj->y_position);
            }
            for (j = 0; j != 25; j++) {
                s32 col;
                s32 rx;
                s32 ry;
                s32 xl;
                s32 yl;
                s32 xh;
                s32 yh;
                struct127 *o;
                u8 *c;

                if (j == i || j == other) {
                    continue;
                }
                o = &D_800CC2D0[j];
                if (o->interaction_state != 0) {
                    if (((s8 *)o->pad320)[8] != 0) {
                        col = ((s8 *)o->pad320)[8] - 1;
                        if (j < D_8008FD8C) {
                            if (func_150859AC(j, 3) <= 0) {
                                col = -1;
                            } else if (func_150859AC(j, 4) <= 0) {
                                col = -1;
                            } else if (D_800E0BCC & (1 << j)) {
                                col = -1;
                            } else if (o->health == 0) {
                                col = -1;
                            } else if ((o->unk31C->unk75 & 0x7F) == 0x21) {
                                col = -1;
                            }
                        }
                        if (col != -1) {
                            dx = (o->x_position - px) * scale;
                            dz = (o->z_position - pz) * scale;
                            col = col + col + col;
                            c = D_800AB62C[0] + col;
                            if (dx * dx + dz * dz < radiusSq) {
                                gDPPipeSync(gfx++);
                                gDPSetEnvColor(gfx++, c[0], c[1], c[2], 0xFF);
                                rx = (s32)((dx * cosv - dz * sinv) * zoom) + x;
                                ry = (s32)((dx * sinv + dz * cosv) * zoom) + y;
                                xh = rx + sz;
                                xl = rx - sz;
                                yh = ry + sz;
                                yl = ry - sz;
                                gSPScisTextureRectangle(gfx++, xl, yl, xh, yh, G_TX_RENDERTILE, 0, 0, 0x400, 0x400);
                                h2 = func_15085DA8(o->y_position);
                                if (h != h2) {
                                    gDPPipeSync(gfx++);
                                    gDPSetEnvColor(gfx++, 0x00, 0x00, 0x00, 0xFF);
                                    if (h < h2) {
                                        gSPScisTextureRectangle(gfx++, xl, yl - sz, xh, yl, G_TX_RENDERTILE, 0, 0, 0x400, 0x400);
                                    } else if (h2 < h) {
                                        gSPScisTextureRectangle(gfx++, xl, yh, xh, yh + sz, G_TX_RENDERTILE, 0, 0, 0x400, 0x400);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return gfx;
}
