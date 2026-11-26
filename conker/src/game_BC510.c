#include <ultra64.h>

#include "functions.h"
#include "variables.h"


extern u8 D_D16[];

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
    BlockDebris *node;
    BlockDebris **link;
    f32 target;
    f32 base;
    f32 cur;
    f32 inc;

    link = &D_800D245C;
    node = D_800D245C;
    while (node != NULL) {
        node->vel.y -= 5.0f;
        node->pos.x += node->vel.x;
        node->pos.y += node->vel.y;
        node->pos.z += node->vel.z;
        node->rot.x += node->rotVel.x;
        node->rot.y += node->rotVel.y;
        node->rot.z += node->rotVel.z;
        timer = node->timer;
        timer -= D_800BE9E4;
        if (timer <= 0) {
            *link = node->next;
            func_100043B4(node, 3);
        } else {
            node->timer = timer;
            link = &node->next;
        }
        node = node->next;
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
    MtxF mtx;
    f32 unused;
    BlockDebris *node;

    D_800D2456 -= 1;
    D_800D2890[0] = func_150ADA20() % 5U;
    D_800D2890[2] = sqrtf(25.0f - D_800D2890[0] * D_800D2890[0]);
    node = allocate_memory(sizeof(*node), 1, 1, 0);
    node->next = D_800D245C;
    D_800D245C = node;
    guMtxL2F(mtx, &D_800D2590[D_800D2456][D_800BE9C0 ^ 1]);
    func_1503E5F8(mtx, &node->pos.x, &node->pos.y, &node->pos.z, &node->rot.x, &node->rot.y, &node->rot.z,
                  &unused, &unused, &unused);
    node->vel.x = (func_150ADA20() & 0xFU) + 10;
    node->vel.y = (func_150ADA20() & 0x1FU) + 20;
    node->vel.z = 0.0f;
    node->rotVel.x = func_150ADA20() & 0xFU;
    node->rotVel.y = func_150ADA20() & 0xFU;
    node->rotVel.z = func_150ADA20() & 0xFU;
    node->timer = 120;
}

/* Adds a block on top; it grows in from zero scale. */
void func_1508F9C4(void) {
    D_800D2410[D_800D2456++] = 0.0f;
}


/* Spawns and animates the bubbles while player 1 is in water (driven by its unkB2 counter). */
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
    if (movement_obj[0].inwater) {
        air = movement_obj[0].unkB2;
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

/* Builds this level's overlay display list: blocks, water, bubbles and HUD. Returns the
 * original pointer (drops everything) if the frame's display list buffer would overflow. */
Gfx *func_1508FD38(register Gfx *gfx) {
    Gfx *start;
    s32 i;
    s32 step;
    s32 overflow;
    s16 scaled[33]; /* scaled[i]: i pixels at the HUD scale D_8008FE1C */

    start = gfx;
    if (level == 0x1D) {
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
    if (D_800C35EA[0] == 1) {
        return gfx;
    }
    if (D_8008CD00 != 0) {
        step = 0x400;
        if (D_8008FE1C != 1.0f) {
            step = (s32)((f32)step / D_8008FE1C);
            for (i = 0; i < 33; i++) {
                scaled[i] = (s32)((f32)i * D_8008FE1C);
            }
        } else {
            // matching: one line; the loop body's line number steers as1's unrolled store order
            for (i = 0; i < 33; i++) scaled[i] = i;
        }
        gfx = func_15093B58(gfx, step, scaled, 0x2C30, 1);
    }
    if (D_800D2428[0] != (f32)D_8009DCB4[0]) {
        gfx = func_150900F0(gfx, 0);
    } else if (D_800D2588 != NULL) {
        func_1518CA04(0xB8);
        D_800D2588 = NULL;
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
extern s16 D_8009DC90[][3];

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
    MtxF mtx;
    MtxF base;
    BlockDebris *node;
    Gfx *dl;

    if (D_800D2588 == NULL) {
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
    dl = D_800D2588;
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
        for (node = D_800D245C; node != NULL; node = node->next) {
            func_15043E68(&node->mtx[D_800BE9C0], node->rot.x, node->rot.y, node->rot.z, node->pos.x,
                          node->pos.y, node->pos.z);
            gSPMatrix(gfx++, &node->mtx[D_800BE9C0], G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(gfx++, dl);
        }
    }
    return gfx;
}
/* matching: SpriteParams before tex (0x10 bytes), as an older revision with other field names; a full
 * SpriteParams on the stack changes the frame. func_15094F70 still writes tex past its end, and
 * func_15095A48 reads it back (see ParticleDesc64 in effects.h). */
typedef struct TexSprite {
    s32 unk0;
    s16 unk4;
    s16 w;
    s16 h;
    u8 alpha;
    u8 unkB;
    u8 unkC;
    u8 flags; /* SPRITE_FLIP_* */
    u8 padE[2];
} TexSprite;

#define SPRITE_FLIP_X 1
#define SPRITE_FLIP_Y 2

extern TextureDesc D_800903D4;
extern TextureDesc D_80091840;

/* Draws the bubbles: rising ones (states 1 and 2) wobble, popping ones (state 3) flip randomly. */
Gfx *func_15090630(Gfx *gfx) {
    s32 i;
    TexSprite sprite;
    s32 first;
    s32 x;
    s32 y;
    s32 flags;
    f32 wobble;
    s32 size;
    s32 timer;
    s32 tex;

    sprite.alpha = 255;
    sprite.flags = 0;
    gDPPipeSync(gfx++);
    gDPSetCombine(gfx++, 0xFFB5FF, 0xFFFCFE38);
    gDPSetOtherMode(gfx++, 0x102C0F, 0x0C184244);
    first = TRUE;
    for (i = 0; i < 6; i++) {
        if (D_800D2460[i].state == 1 || D_800D2460[i].state == 2) {
            if (first) {
                func_1510D0EC(D_80091840.addr, &tex, 3, 0);
                first = FALSE;
                gfx = func_15094F70(gfx, &D_800903D4, 0, &sprite, 0, 0, 0, 2, 3);
            }
            wobble = func_15048A40(D_800D2460[i].phase);
            x = D_800D2460[i].x;
            y = D_800D2460[i].y;
            sprite.w = D_800D2460[i].size * (0.2f * wobble + 1.0f);
            sprite.h = D_800D2460[i].size * (-0.2f * wobble + 1.0f);
            gfx = func_15095A48(gfx, &sprite, x, y);
        }
    }
    first = TRUE;
    for (i = 0; i != 6; i++) {
        flags = 0;
        if (D_800D2460[i].state == 3) {
            size = D_800D2460[i].size;
            x = D_800D2460[i].x;
            y = D_800D2460[i].y;
            sprite.w = size;
            sprite.h = size;
            if ((u8)func_150ADA20() < 0x80U) {
                flags = SPRITE_FLIP_X;
            }
            if ((u8)func_150ADA20() < 0x80U) {
                flags |= SPRITE_FLIP_Y;
            }
            sprite.flags = flags;
            if (first) {
                first = FALSE;
                gfx = func_15094F70(gfx, &D_80091840, 0, &sprite, 0, 0, 0, 2, 3);
            }
            gfx = func_15095A48(gfx, &sprite, x, y);
            timer = D_800D2460[i].timer;
            timer -= D_800BE9E4;
            if (timer < 0) {
                timer = 0;
            }
            D_800D2460[i].timer = timer;
        }
    }
    return gfx;
}
extern s32 D_800D2558[];
extern u16 D_8009025A;
extern u16 D_80090262;
extern u16 D_80090266;
extern u16 D_8009026E;
extern u16 D_80090272;

/* Animates the water surface mesh while player 1 is in water: blends its vertex positions,
 * tints its vertex colours and picks its textures by player 1's unkB2 counter. */
void func_1509093C(void) {
    WaterVtx *dst;
    WaterBase *base;
    WaterBase *target;
    s32 i;
    f32 blend;
    s32 tmp;
    s32 intensity;
    f32 lo;
    f32 hi;
    s32 fadeRaw;
    Gfx **dlists;
    u8 numDLists;
    WaterVtx *vtxStart;
    s32 unkOut1;
    s32 unkOut2;
    s32 unkOut3;
    s32 fade;
    s32 r;
    s32 g;
    u16 tex;
    f32 dt;
    s32 r2;
    s32 g2;
    s32 b2;
    s32 b;

    if (movement_obj->inwater) {
        intensity = movement_obj->unkB2;
    } else {
        intensity = 0;
    }
    if (intensity > 90) {
        if (D_800D24C8.alpha == 0) {
            D_800D24C8.alpha = 255;
            D_800D24C8.unk94 = 0.0f;
            D_800D24C8.unk98 = 0.02f;
            D_800D24C8.unkA0 = 0;
            D_800D24C8.unkA4 = 3.2f;
            D_800D24C8.unkA8 = 0;
            D_800D24C8.unkAC = 0;
            D_800D24C8.timer = (func_150ADA20() & 63) + 60;
        }
        tmp = D_800CC382;
    } else {
        D_800D24C8.alpha = 0;
        return;
    }
    {
        D_800D24C8.unk9C = (f32)(u32)tmp / 720.0f;
        if (D_800D24C8.unk9C > 1.0f) {
            D_800D24C8.unk9C = 1.0f;
        }
        if (intensity < 1441) {
            D_800D24C8.unk94 += D_800D24C8.unk98 * D_800BE9E4;
            lo = D_800D24C8.unk9C - 0.2f;
            if (lo < 0.0f) {
                lo = 0.0f;
            }
            if (D_800D24C8.unk94 <= lo) {
                D_800D24C8.unk94 = (lo + lo) - D_800D24C8.unk94;
                D_800D24C8.unk98 = -D_800D24C8.unk98;
            } else {
                hi = D_800D24C8.unk9C + 0.2f;
                if (hi > 1.0f) {
                    hi = 1.0f;
                }
                if (hi <= D_800D24C8.unk94) {
                    D_800D24C8.unk94 = (hi + hi) - D_800D24C8.unk94;
                    D_800D24C8.unk98 = -D_800D24C8.unk98;
                }
            }
        }
        if (D_800D24C8.dl == 0) {
            func_1502FE10(0xB9, &dlists, &numDLists, &vtxStart, &unkOut1, &unkOut2, &unkOut3);
            D_800D24C8.dl = *dlists;
            D_800D24C8.count = ((u32)dlists - (u32)vtxStart) >> 4;
            D_800D24C8.vtx[0] = vtxStart;
            D_800D24C8.vtx[1] = allocate_memory(D_800D24C8.count * sizeof(WaterVtx), 1, 2, 0);
            bcopy(vtxStart, D_800D24C8.vtx[1], D_800D24C8.count * sizeof(WaterVtx));
            D_800D24C8.base = allocate_memory(D_800D24C8.count * sizeof(WaterBase), 1, 0, 0);
            target = D_800D24C8.base;
            for (i = 0; i < D_800D24C8.count; i++) {
                target[i].ob[0] = vtxStart[i].ob[0];
                target[i].ob[1] = vtxStart[i].ob[1];
                target[i].ob[2] = vtxStart[i].ob[2];
                target[i].cn[0] = vtxStart[i].cn[0];
                target[i].cn[1] = vtxStart[i].cn[1];
                target[i].cn[2] = vtxStart[i].cn[2];
                target[i].cn[3] = vtxStart[i].cn[3];
            }
            /* D_800D2558 aliases D_800D24C8.target; the original addresses it by its own symbol */
            for (i = 1; i < 2; i++) {
                func_1502B7F0(&D_800D2558[i - 1], 2, 0x18, i - 1);
            }
        }
        base = D_800D24C8.base;
        dst = D_800D24C8.vtx[D_800BE9C0];
        target = D_800D24C8.target;
        blend = D_800D24C8.unk94;
        for (i = 0; i < D_800D24C8.count; i++) {
            dst[i].ob[0] = (f32)base[i].ob[0] + ((f32)target[i].ob[0] - (f32)base[i].ob[0]) * blend;
            dst[i].ob[1] = (f32)base[i].ob[1] + ((f32)target[i].ob[1] - (f32)base[i].ob[1]) * blend;
            dst[i].ob[2] = (f32)base[i].ob[2] + ((f32)target[i].ob[2] - (f32)base[i].ob[2]) * blend;
        }
        if (intensity < 480) {
            tex = D_8009026E;
            D_800D24C8.tex[3] = tex;
            D_800D24C8.tex[2] = tex;
            tex = D_8009025A;
            D_800D24C8.tex[1] = tex;
            D_800D24C8.tex[0] = tex;
        } else if (intensity < 960) {
            tex = D_80090272;
            D_800D24C8.tex[3] = tex;
            D_800D24C8.tex[2] = tex;
            tex = D_8009025A;
            D_800D24C8.tex[1] = tex;
            D_800D24C8.tex[0] = tex;
        } else if (intensity < 1200) {
            tex = D_80090272;
            D_800D24C8.tex[3] = tex;
            D_800D24C8.tex[2] = tex;
            tex = D_80090266;
            D_800D24C8.tex[1] = tex;
            D_800D24C8.tex[0] = tex;
        } else {
            tex = D_80090272;
            D_800D24C8.tex[3] = tex;
            D_800D24C8.tex[2] = tex;
            tex = D_80090262;
            D_800D24C8.tex[1] = tex;
            D_800D24C8.tex[0] = tex;
        }
        fadeRaw = (intensity - 720) * 255;
        fade = fadeRaw / 480;
        if (fade < 0) {
            fade = 0;
        } else if (fade >= 256) {
            fade = 255;
        }
        r = ((fade * -151) >> 8) + 255;
        g = ((fade * -164) >> 8) + 255;
        b = 255;
        r2 = ((fade * -97) >> 8) + 255;
        g2 = ((fade * -94) >> 8) + 255;
        b2 = ((fade * -91) >> 8) + 255;
        // matching: the dead store, the do-while shape and the |= 0 no-ops steer uopt's register choice
        fade = 0;
        i = 0;
        if (i < D_800D24C8.count) {
            r |= 0;
            g |= 0;
            b |= 0;
            do {
                if (i < 208) {
                    dst[i].cn[0] = (base[i].cn[0] * r) >> 8;
                    dst[i].cn[1] = (base[i].cn[1] * g) >> 8;
                    dst[i].cn[2] = (base[i].cn[2] * b) >> 8;
                } else if (i >= 220) {
                    dst[i].cn[0] = r;
                    dst[i].cn[1] = g;
                    dst[i].cn[2] = b;
                } else {
                    dst[i].cn[0] = r2;
                    dst[i].cn[1] = g2;
                    dst[i].cn[2] = b2;
                }
                i++;
            } while (i < D_800D24C8.count);
        }
        dt = D_800BE9E4;
        if (fadeRaw > 0x2CE2E) {
            D_800D24C8.unkAC = 0.0f;
        } else if (fadeRaw > 0x1DF1E) {
            if (fabsf(D_800D24C8.unkAC) < 0.6f) {
                D_800D24C8.unkAC = 0.6f;
            }
        } else if (D_800D24C8.timer != 0) {
            tmp = D_800D24C8.timer - D_800BE9E4;
            if (tmp <= 0) {
                tmp = 0;
                D_800D24C8.unkAC = 0.2f;
                D_800D24C8.unkA8 = dt * D_800D24C8.unkAC;
            }
            D_800D24C8.timer = tmp;
        }
        D_800D24C8.unkA8 += D_800D24C8.unkAC * dt;
        if (D_800D24C8.unkA8 > 2.0f) {
            D_800D24C8.unkA8 = 4.0f - D_800D24C8.unkA8;
            D_800D24C8.unkAC = -D_800D24C8.unkAC;
        } else if (D_800D24C8.unkA8 < -2.0f) {
            D_800D24C8.unkA8 = -4.0f - D_800D24C8.unkA8;
            D_800D24C8.unkAC = -D_800D24C8.unkAC;
        } else if (D_800D24C8.timer == 0 && fadeRaw < 480 * 255 && D_800D24C8.unkA8 >= 0 && D_800D24C8.unkA8 < D_800D24C8.unkAC * dt) {
            D_800D24C8.timer = (func_150ADA20() & 63) + 60;
            D_800D24C8.unkA8 = 0.0f;
            D_800D24C8.unkAC = 0.0f;
        }
        if (fadeRaw >= 720 * 255 && D_800D24C8.unkA0 > -22.0f) {
            D_800D24C8.unkA4 -= 0.25f * D_800BE9E4;
            D_800D24C8.unkA0 += D_800D24C8.unkA4;
        }
    }
}

/* Draws the water surface mesh animated by func_1509093C. */
Gfx *func_150911F4(Gfx *gfx) {
    s32 texBuf[6];
    f32 y;
    f32 x;
    s32 tex;

    if (D_800D24C8.alpha == 0) {
        return gfx;
    }
    gSPDisplayList(gfx++, D_80084160);
    gSPSetGeometryMode(gfx++, G_CULL_BACK);
    gSPClearGeometryMode(gfx++, G_ZBUFFER | G_CULL_FRONT | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR);
    gSPClearGeometryMode(gfx++, G_LIGHTING | 0x400000);
    if (D_800D24C8.alpha < 255) {
        gSPSegment(gfx++, 8, D_80083440);
    } else {
        gSPSegment(gfx++, 8, D_800835C0);
    }
    y = -11.0f + D_800D24C8.unkA8;
    x = -1.0f - D_800D24C8.unkA0;
    func_15043D90(&D_800D24C8.mtx[D_800BE9C0], x, y, 0.0f,
                  4.05f, 4.05f, 4.05f, 667.0f, 152.0f, -1668.0f);
    gSPMatrix(gfx++, &D_800D24C8.mtx[D_800BE9C0], G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gDPSetFogColor(gfx++, 0, 0, 0, 255);
    gDPSetPrimColor(gfx++, 0xF2, 0, 0, 0, 0, 0);
    gDPSetEnvColor(gfx++, 0, 0, 0, D_800D24C8.alpha);
    gSPSegment(gfx++, 3, &D_800D24C8.mtx[D_800BE9C0]);
    gSPSegment(gfx++, 1, D_800D24C8.vtx[D_800BE9C0]);
    if (D_800D24C8.tex[0]) {
        tex = func_1510D0EC(D_800D24C8.tex[0], texBuf, 3, 0);
        gSPSegment(gfx++, 6, tex);
    }
    if (D_800D24C8.tex[1]) {
        tex = func_1510D0EC(D_800D24C8.tex[1], texBuf, 3, 0);
        gSPSegment(gfx++, 7, tex);
    }
    if (D_800D24C8.tex[2]) {
        tex = func_1510D0EC(D_800D24C8.tex[2], texBuf, 3, 0);
        gSPSegment(gfx++, 10, tex);
    }
    if (D_800D24C8.tex[3]) {
        tex = func_1510D0EC(D_800D24C8.tex[3], texBuf, 3, 0);
        gSPSegment(gfx++, 11, tex);
    }
    gSPDisplayList(gfx++, D_800D24C8.dl);
    gSPDisplayList(gfx++, D_80084190);
    return gfx;
}

/* Loads HUD icon sheet D_D16 + `sheet` (a 64x64 CI4 texture with its palette after it).
 * Sets *loaded when the texture was available. */
Gfx *func_15091534(Gfx *gfx, s32 sheet, u8 *loaded) {
    s32 tex;

    *loaded = FALSE;
    tex = func_1510D0EC(sheet + (s32)D_D16, 0, 3, 0);

    if (tex != (s32)0x80000000) {
        gDPSetTextureImage(gfx++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, tex);
        gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 1023, 0);
        gDPPipeSync(gfx++);
        gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_4b, 4, 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0xFC, 0xFC);
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, tex + 0x800);
        gDPLoadSync(gfx++);
        gDPLoadTLUTCmd(gfx++, 6, 15);
        gDPSetOtherMode(gfx++, 0x00AC3F, 0x00504244);

        *loaded = TRUE;
    }

    return gfx;
}

/* Prints a race time given in frames (60 per second) as minutes, seconds and hundredths,
 * capped at 99 minutes; a negative time prints the placeholder strings instead. */
void func_150916B4(s32 x, s32 y, s32 frames, s32 style) {
    s32 pad[2]; // matching: reserves the original frame size
    s32 seconds;
    s32 hundredths;

    if (frames >= 99 * 60 * 60) {
        frames = 99 * 60 * 60;
    }

    hundredths = ((frames % 60) * 100) / 60;
    if (hundredths >= 100) {
        hundredths = 99;
    }

    x -= 8;
    if (frames >= 0) {
        seconds = frames / 60;
        func_15042D94((x - (((seconds / 60) >= 10) ? 5 : 0)) - 8, y, (u8)style, D_8009DCC0, seconds / 60);
        func_15042D94(x, y, style, D_8009DCC4, seconds % 60);
        func_15042D94(x + 16, y, style, D_8009DCCC, hundredths);
        return;
    }

    func_15042D94(x - 8, y, (u8)style, D_8009DCD4);
    func_15042D94(x, y, style, D_8009DCD8);
    func_15042D94(x + 16, y, style, D_8009DCE0);
}

typedef struct HudBars {
    u8 b[4];
} HudBars;



extern HudBars D_800873B0;
extern Gfx D_80087390[];
extern u8 D_800873B4[];
extern u8 D_800873BC[];
extern u8 D_800873C4[];
extern u8 D_800873C8[];
extern u8 D_CF1[];
extern u8 D_CF2[];
extern u8 D_D13[];
extern u8 D_D14[];

/* Draws the multiplayer HUD in each player's viewport, as enabled by the D_8008FDC0 flags:
 * 0x002 an icon picked by func_150859AC(i, 3), 0x400 score, 0x800 name, 0x004 held-item icons
 * (repeated per count) and bars from ActorCtl field 0x12C, 0x010 two rows of bars from
 * func_150859AC(i, 4), grenade icons, 0x080 race time and lap, 0x1000 a map with player dots,
 * 0x020 the radar (func_15093B58). */
Gfx *func_150918EC(Gfx *gfx) {
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    s32 mapTop;
    s32 h;
    s32 iconH;
    s32 texT;
    s32 n;
    s32 s;
    s32 v;
    s32 texS;
    s32 vpWidth;
    s32 lapCount;
    s32 vpRight;
    ActorCtl *a1; // matching: unused, yet removing it changes the register allocation
    Actor *player;
    s32 rowY;
    s32 cellShift;
    s32 k;
    s32 icon;
    s32 vpLeft;
    s32 barShift;
    s32 step;
    u8 texOk;
    u8 texLoaded;
    u8 barMode;
    HudBars barLen;
    s8 grenades;
    s16 scaled[33];

    barMode = 0;
    grenades = 0;
    barLen = D_800873B0;
    vpWidth = D_800BE628->right - D_800BE628->left;
    gSPDisplayList(gfx++, D_80087390);
    func_1504332C(255, 255, 255, 255);
    step = 0x400;
    if (D_8008FE1C != 1.0f) {
        step = (s32)((f32)step / D_8008FE1C);
        for (j = 0; j < 33; j++) {
            scaled[j] = (s32)((f32)j * D_8008FE1C);
        }
    } else {
        // matching: one line; the loop body's line number steers as1's unrolled store order
        for (j = 0; j < 33; j++) scaled[j] = j;
    }

    if (D_8008FDC0 & 0xC02) {
        texLoaded = 0;
        texOk = 0;
        for (i = 0; i <= D_80082FA0; i++) {
            if (D_80087270[i] != 10) {
                vpLeft = D_800BE628[i].left;
                y = D_800BE628[i].top + scaled[6];
                if (D_8008FDC0 & 2) {
                    if (!texLoaded) {
                        n = func_1510D0EC((s32)D_D14, 0, 3, 0);
                        texLoaded = 1;
                        if (n != (s32)0x80000000) {
                            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, n);
                            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD);
                            gDPLoadSync(gfx++);
                            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 1023, 0);
                            gDPPipeSync(gfx++);
                            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD);
                            gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 31 << G_TEXTURE_IMAGE_FRAC, 31 << G_TEXTURE_IMAGE_FRAC);
                            texOk = 1;
                        }
                        gDPSetOtherMode(gfx++, 0x002C3F, 0x00504244);
                        gDPPipeSync(gfx++);
                        gDPSetEnvColor(gfx++, 255, 255, 255, 128);
                    }
                    x = scaled[5] + vpLeft;
                    n = func_150859AC(i, 3);
                    // matching: both on one line (as1 orders the two loads by line number)
                    texS = D_800873C4[n & 3] << 5; texT = D_800873C8[n >> 2] << 5;
                    if (texOk) {
                        gSPScisTextureRectangle(gfx++, x * 4, y * 4, (scaled[5] + x) * 4, (scaled[7] + y) * 4, 0, texS, texT, step, step);
                    }
                }
                if (D_8008FDC0 & 0x400) {
                    x = (vpWidth >> 1) + vpLeft - 8;
                    if (D_8008FE1C != 1.0f) {
                        x -= 8;
                    }
                    func_15042D94(x, y, 0x41, "%d", D_8008FDD4->score[i]);
                }
                if (D_8008FDC0 & 0x800) {
                    x = (vpWidth >> 1) + vpLeft + 16;
                    if (D_8008FE1C != 1.0f) {
                        x += 16;
                    }
                    func_15042D94(x, y, 0x41, "(%s)", D_800E0BD8[8 + D_800E0AC0[i]]);
                }
            }
        }
    }

    if (D_8008FDC0 & 0x14) {
        texLoaded = 0;
        texOk = 0;
        for (i = 0; i <= D_80082FA0; i++) {
            if (D_80087270[i] != 10) {
                vpLeft = D_800BE628[i].left + scaled[5];
                vpRight = D_800BE628[i].right - scaled[5];
                y = D_800BE628[i].top + scaled[5];
                if (D_8008FDC0 & 4) {
                    player = &movement_obj[i];
                    n = 1;
                    h = player->mv->context_b;
                    h &= 0x7F;
                    if (player->type == 0x1F) {
                        h = 0;
                    }
                    switch (h) {
                    case 0x9:
                        icon = 0;
                        break;
                    case 0x16:
                        icon = 1;
                        break;
                    case 0x14:
                    case 0x3F:
                        icon = 3;
                        break;
                    case 0x20:
                        icon = 5;
                        break;
                    case 0x18:
                    case 0x41:
                        icon = 6;
                        break;
                    case 0x15:
                        icon = 7;
                        break;
                    case 0x22:
                        icon = 9;
                        break;
                    case 0x23:
                        icon = 10;
                        break;
                    case 0x21:
                        icon = 12;
                        break;
                    case 0x24:
                        icon = 13;
                        break;
                    case 0x25:
                        icon = 14;
                        break;
                    case 0x26:
                    case 0x3A:
                        icon = -1;
                        if (player->energy != 0) {
                            grenades = 1;
                        }
                        break;
                    case 0x38:
                        icon = -1;
                        break;
                    case 0x32:
                    case 0x33:
                        n = player->mv->ammo_count;
                        icon = 11;
                        break;
                    case 0x40:
                        icon = 4;
                        cellShift = -3;
                        break;
                    default:
                        if (level == 0x1A) {
                            icon = 4;
                            cellShift = 4;
                        } else {
                            icon = -1;
                        }
                        break;
                    }
                    if (player->lifting != 0) {
                        icon = -1;
                    }
                    if (icon >= 0) {
                        j = 0;
                        if (!texLoaded) {
                            texLoaded = 1;
                            gfx = func_15091534(gfx, 0, &texOk);
                        }
                        x = (vpRight - scaled[25]) * 4;
                        texT = D_800873B4[icon / 3] << 5;
                        iconH = scaled[D_800873BC[icon / 3]] * 4;
                        if (icon == 4) {
                            j = 4;
                            iconH -= 8;
                            texT += cellShift << 5;
                        }
                        if (icon == 1) {
                            iconH -= 4;
                        }
                        do {
                            if (texOk) {
                                gSPTextureRectangle(gfx++, x, y * 4 + j, scaled[22] * 4 + x, y * 4 + iconH + j, 0, ((icon % 3) * 25) << 5, texT, step, step);
                            }
                            x -= 72;
                        } while (--n > 0);
                    }
                }
                if (D_8008FDC0 & 0x10) {
                    if (!texLoaded) {
                        gfx = func_15091534(gfx, 0, &texOk);
                    }
                    s = func_150859AC(i, 4);
                    x = scaled[22] + vpLeft;
                    h = ((s + 1) >> 1) * 5;
                    if (h != 0 && texOk) {
                        gSPTextureRectangle(gfx++, (x - scaled[h]) * 4, y * 4, x * 4, scaled[4] * 4 + y * 4, 0, 0x620, 0xA0, step, step);
                    }
                    h = (s >> 1) * 5;
                    if (h != 0 && texOk) {
                        gSPTextureRectangle(gfx++, (x - scaled[h]) * 4, scaled[5] * 4 + y * 4, x * 4, scaled[9] * 4 + y * 4, 0, 0x620, 0xA0, step, step);
                    }
                }
            }
        }
    }

    if (D_8008FDC0 & 4) {
        u8 pad3a; // matching: unused stack slots in the original frame
        s32 cellOffset;
        s32 pad3b;
        texLoaded = 0;
        texOk = 0;
        for (i = 0; i <= D_80082FA0; i++) {
            if (D_80087270[i] != 10) {

                vpRight = D_800BE628[i].right - 5.0f;
                y = D_800BE628[i].top + 5.0f;
                n = 1;
                icon = -1;
                cellOffset = 0;
                if (movement_obj[i].type == 0x1E) {
                    j = movement_obj[i].mv->unk128;
                    if (j & 1) {
                        icon = 6;
                        barShift = 4;
                    } else if (j & 2) {
                        icon = 3;
                    } else if (j & 0x80) {
                        icon = 0;
                        barShift = 6;
                    } else if (j & 0x10) {
                        icon = 9;
                        barShift = -3;
                    }
                    if (icon >= 0 && icon != 3) {
                        if (barShift > 0) {
                            barLen.b[i] = movement_obj[i].mv->unk12C >> barShift;
                        } else {
                            barLen.b[i] = movement_obj[i].mv->unk12C << -barShift;
                        }
                        barMode = 1;
                    }
                } else {
                    cellOffset = 3;
                    switch (movement_obj[i].mv->context_b & 0x7F) {
                    case 0x37:
                        icon = 7;
                        break;
                    case 0x38:
                        icon = 4;
                        break;
                    case 0x39:
                        icon = 10;
                        break;
                    case 0x30:
                    case 0x31:
                        n = movement_obj[i].mv->ammo_count;
                        icon = 6;
                        break;
                    case 0x3E:
                        barLen.b[i] = movement_obj[i].mv->unk12C >> 4;
                        barMode = 2;
                        break;
                    }
                }
                if (icon >= 0) {
                    if (!texLoaded) {
                        texLoaded = 1;
                        gfx = func_15091534(gfx, 1, &texOk);
                    }
                    x = vpRight - scaled[25];
                    texS = (icon % 3) * 25;
                    icon = icon / 3;
                    texT = D_800873B4[icon] + cellOffset;
                    texS <<= 5;
                    texT <<= 5;
                    do {
                        if (texOk) {
                            gSPTextureRectangle(gfx++, x * 4, y * 4, (x + scaled[22]) * 4, (scaled[D_800873BC[icon]] + y) * 4, 0, texS, texT, step, step);
                        }
                        x -= 18;
                    } while (--n > 0);
                }
            }
        }
    }

    if (barMode) {
        gDPPipeSync(gfx++);
        gDPSetOtherMode(gfx++, 0x002C3F, 0x0F0A4004);
        gDPSetCombine(gfx++, 0xFFFFFF, 0xFFFDF6FB);
        gDPSetPrimColor(gfx++, 0, 0, 255, 0, 0, 255);
        if (barMode == 2) {
            k = 15;
        } else {
            k = 30;
        }
        for (i = 0; i <= D_80082FA0; i++) {
            if (barLen.b[i]) {
                x = (s32)D_800BE628[i].right - k;
                rowY = (s32)D_800BE628[i].top + 10;
                gDPFillRectangle(gfx++, x - barLen.b[i], rowY, x, rowY + 4);
            }
        }
    }

    if (grenades) {
        s32 pad2; // matching: three unused stack slots in the original frame
        s32 pad3;
        s32 pad4;
        s32 cellH;
        s32 cellW;
        // matching: the original keeps the pipe-sync command word in a variable (cellW, reused below)
        cellW = G_RDPPIPESYNC << 24;
        j = func_1510D0EC((s32)D_D13, 0, 3, 0);
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, j);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 1023, 0);
        { Gfx *g = gfx++; g->words.w0 = cellW; g->words.w1 = 0; } /* gDPPipeSync */
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 31 << G_TEXTURE_IMAGE_FRAC, 31 << G_TEXTURE_IMAGE_FRAC);
        { Gfx *g = gfx++; g->words.w0 = cellW; g->words.w1 = 0; } /* gDPPipeSync */
        gDPSetOtherMode(gfx++, 0x002C3F, 0x00504244);
        for (i = 0; i <= D_80082FA0; i++) {
            j = movement_obj[i].mv->context_b;
            if ((j &= 0x7F) == 0x26 || j == 0x3A) {
                x = (s32)D_800BE628[i].right - 15;
                x *= 4;
                rowY = (s32)D_800BE628[i].top + 5;
                rowY *= 4;
                // matching: constant-first operands; the swapped form changes the code
                if (0x1A == level || 0x3F == level) {
                    cellW = 11;
                    cellH = 14;
                    texS = 0x200;
                    v = 0x1E0;
                } else if (movement_obj[i].character_type == 0x75 || movement_obj[i].character_type == 0xB1) {
                    cellW = 5;
                    cellH = 20;
                    texS = 0xC0;
                    v = 0x100;
                } else if (movement_obj[i].character_type == 0x3B) {
                    cellW = 11;
                    cellH = 14;
                    texS = 0x200;
                    v = 0x1E0;
                } else {
                    cellW = 8;
                    cellH = 9;
                    texS = 0x240;
                    v = 0x60;
                }
                for (j = 0; j < movement_obj[i].mv->numGrenades; j++) {
                    gSPScisTextureRectangle(gfx++, x, rowY, scaled[cellW] * 4 + x, scaled[cellH] * 4 + rowY, 0, texS, v, 0x400, step);
                    x -= 44;
                }
            }
        }
    }

    if ((D_8008FDC0 & 0x80) && D_800872A0 != NULL) {
        lapCount = D_800E0BE0[7];
        for (i = 0; i <= D_80082FA0; i++) {
            rowY = (s32)D_800BE628[i].top + 5;
            func_1504332C(255, 255, 255, 255);
            if (D_800872A0[i].lastLapTime > 0) {
                x = (s32)D_800BE628[i].left;
                func_150916B4(x + 23, rowY + 25, D_800872A0[i].lastLapTime, 0xC0);
            }
            x = (s32)D_800BE628[i].left + (vpWidth >> 1);
            icon = (s8)(D_800872A0[i].lap + 1); /* the current lap */
            if (icon <= lapCount) {
                func_15042D94(x, rowY, 0xC1, "%s", D_800E0BD8[8 + D_800E0AC0[i]]);
                func_15042D94((s32)D_800BE628[i].left + 29, rowY + 14, 0xC1, "lap %d/%d", icon, lapCount);
                func_150916B4(x, (s32)D_800BE628[i].bottom - 12, D_800872A0[i].raceTime, 0xC0);
            }
        }
    }

    if (D_800BEAC1 == 0 && (D_8008FDC0 & 0x1000)) {
        s32 shift;
        s32 size;
        f32 scale;
        f32 ax;
        f32 az;
        scale = 0.013f;
        i = 0;
        x = (s32)D_800BE628->right - scaled[32];
        x *= 4;
        if (D_8008FE1C != 1.0f) {
            shift = 6;
            scale *= 1.56f;
            size = 99;
        } else {
            shift = 4;
            size = 64;
        }
        k = (D_8008FDBC & 0x40) ? (s32)D_CF2 : (s32)D_CF1;
        vpRight = func_1510D0EC(k, 0, 3, 0);
        gDPSetTextureImage(gfx++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, vpRight);
        gDPSetTile(gfx++, G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 2047, 0);
        gDPPipeSync(gfx++);
        gDPSetTile(gfx++, G_IM_FMT_IA, G_IM_SIZ_8b, 8, 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 63 << G_TEXTURE_IMAGE_FRAC, 63 << G_TEXTURE_IMAGE_FRAC);
        gDPSetOtherMode(gfx++, 0x002C3F, 0x00504244);
        for (i = 0; i < D_8008FD90; i++) {
            if (func_150859AC(i, 3) > 0) {
                rowY = scaled[21] + ((s32)D_800BE628[i].top + scaled[24]);
                rowY *= 4;
                gDPPipeSync(gfx++);
                gDPSetEnvColor(gfx++, 255, 255, 255, 128);
                gDPSetCombine(gfx++, 0x129A25, 0xFF37FFFF);
                s = x - scaled[32] * 4;
                mapTop = rowY - scaled[32] * 4;
                gSPScisTextureRectangle(gfx++, s, mapTop, size * 4 + s, size * 4 + mapTop, 0, 0, 0, step, step);
                gDPPipeSync(gfx++);
                gDPSetCombine(gfx++, 0x62FEC5, 0xFFFFFBFD);
                for (k = 0, player = movement_obj; k != 25; k++, player++) {
                    if (k == i || (k < D_8008FD8C && func_150859AC(k, 3) > 0) || player->unk328 != 0) {
                        gDPPipeSync(gfx++);
                        if (k == i) {
                            gDPSetEnvColor(gfx++, 0, 255, 0, 255);
                        } else if (k < D_8008FD8C) {
                            gDPSetEnvColor(gfx++, 255, 0, 0, 255);
                        } else {
                            gDPSetEnvColor(gfx++, 255, 255, 255, 255);
                        }
                        ax = player->xpos * scale;
                        az = player->zpos * scale;
                        v = (rowY + 3.4f) + (s32)az;
                        n = (x + 3.6f) + (s32)ax;
                        gSPScisTextureRectangle(gfx++, n - shift, v - shift, n + shift, v + shift, 0, 0, 0, 0x400, 0x400);
                    }
                }
            }
        }
    }
    if (D_800BEAC1 == 0 && (D_8008FDC0 & 0x20)) {
        gfx = func_15093B58(gfx, step, scaled, 0x2C30, 0);
    }
    return gfx;
}

/* Turns the on-screen clock (func_150938BC) on or off, loading its model when turned on. */
void func_15093818(s32 enable) {
    if ((enable != 0) && (D_800D2458 == 0)) {
        D_800D2458 = 1;
        func_15093878();
        return;
    }
    if ((enable == 0) && (D_800D2458 != 0)) {
        D_800D2458 = 0;
    }
}

/* Loads the clock model and allocates its two (double-buffered) matrices. */
void func_15093878(void) {
    D_800D2448 = func_1518C900(0xBA);
    D_800D244C = allocate_memory(2 * sizeof(Mtx), 1, 1, 0);
}

extern u8 D_11AD[];

/* Draws the clock: the four digits of D_800D2450 (frame count, 30 per second) as minutes and
 * seconds go into segments 1-4 as digit textures from D_11AD, then the clock model. */
Gfx *func_150938BC(Gfx *gfx) {
    s32 x = 120;
    s32 y = 129;
    s32 z = -303;
    s32 segment;
    s32 value;
    s32 tex;

    value = (D_800D2450 / 30) % 60;
    segment = 4;
    do {
        tex = func_1510D0EC((s32)D_11AD + (value % 10), 0, 3, 0);
        if (tex == (s32)0x80000000) {
            return gfx;
        }
        gSPSegment(gfx++, segment, tex);
        if (segment == 3) {
            value = (D_800D2450 / 30) / 60;
        } else {
            value = value / 10;
        }
        segment--;
    } while (segment != 0);

    func_150A7D00(&D_800D244C[D_800BE9C0], x, y, z);
    gSPMatrix(gfx++, &D_800D244C[D_800BE9C0], G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx++, D_800D2448);
    return gfx;
}

/* Draws the radar: a disc around each local player (larger when `large`) with a dot for every
 * visible other character, rotated by the camera heading; a dot gets a bar above or below it when
 * the character is on a higher or lower floor (func_15085DA8). */
Gfx *func_15093B58(Gfx *gfx, s32 step, s16 *scaled, s32 renderMode, u8 large) {
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
    s32 dotStep;
    s32 radius;
    s32 dotSize;
    s8 otherFloor;
    s8 floor;
    s32 skip;

    scale = 0.01349999942f;
    radiusSq = 3136.0f;
    dotStep = step;
    radius = scaled[16];
    if (large) {
        zoom = 1.329000115f;
    } else {
        zoom = 1.0f;
    }
    if (D_8008FD90 == 1) {
        scale = 0.02025f;
        radiusSq = 7744.0f;
        dotStep = (step * 2) / 3;
        radius = scaled[24];
    }
    scale *= D_8008FE1C;
    dotSize = (D_8008FE1C == 1.0f) ? 4 : 6;
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
    gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 31 << G_TEXTURE_IMAGE_FRAC, 31 << G_TEXTURE_IMAGE_FRAC);
    gDPSetOtherMode(gfx++, renderMode | 0xF, 0x00504244);

    for (i = 0; i < D_8008FD90; i++) {
        if (func_150859AC(i, 3) > 0) {
            s32 halfSize;
            s32 dsdx;
            s32 x;
            s32 y;
            Actor *obj;
            Camera *cam;
            Camera *cam2;

            x = (((s32)D_800BE628[i].right - scaled[7]) - radius) * 4;
            y = ((s32)D_800BE628[i].top + scaled[21] + radius) * 4;
            obj = &movement_obj[i];
            if (large) {
                x -= 454;
                y -= 70;
            }
            gDPPipeSync(gfx++);
            gDPSetEnvColor(gfx++, 255, 255, 255, 128);
            gDPSetCombine(gfx++, 0x129A25, 0xFF37FFFF);
            if (large) {
                halfSize = (s32)((f32)(radius * 4) * 0.8580000401f);
                // matching: the original rounds dotStep through s32 before scaling it
                dsdx = (s32)((f32)(s32)(f32)dotStep * 1.165501118f);
            } else {
                halfSize = radius * 4;
                dsdx = dotStep;
            }
            gSPScisTextureRectangle(gfx++, x - halfSize, y - halfSize, x + halfSize, y + halfSize, G_TX_RENDERTILE, 0, 0, dsdx, dsdx);
            gDPPipeSync(gfx++);
            gDPSetCombine(gfx++, 0x62FEC5, 0xFFFFFBFD);
            gDPSetEnvColor(gfx++, 255, 0, 0, 255);
            cam = &camera_global_Cameras[i];
            angle = cam->CameraAngle * 0.01745327935f;
            sinv = sinf(angle);
            cosv = cosf(angle);
            if (obj->type != 0x25) {
                /* matching: as1 (-g3) orders equally ranked instructions of a block by source line, lowest
                 * first. The target schedule (li; lwc1; lwc1; sw) needs the px/pz loads emitted
                 * before `skip = -1` while that statement carries the lower line number. The
                 * numbers only have to exceed every real line of this file with
                 * line(skip = -1) < line(px/pz); the lines after them stay monotonic. */
#line 20002
                px = obj->xpos; pz = obj->zpos;
#line 20000
                skip = -1;
                floor = func_15085DA8(obj->ypos);
            } else {
                cam2 = obj->CameraPtr;
                obj = movement_obj;
                skip = 0;
                if (cam2 != NULL && cam2->CNumber == 3) {
                    skip = 1;
                }
                obj += skip;
                px = obj->xpos;
                pz = obj->zpos;
                floor = func_15085DA8(obj->ypos);
            }
            for (j = 0; j != 25; j++) {
                s32 color;
                s32 rx;
                s32 ry;
                s32 xl;
                s32 yl;
                s32 xh;
                s32 yh;
                Actor *other;
                u8 *rgb;

                if (j == i || j == skip) {
                    continue;
                }
                other = &movement_obj[j];
                if (other->type != 0) {
                    if (other->unk328 != 0) {
                        color = other->unk328 - 1;
                        if (j < D_8008FD8C) {
                            if (func_150859AC(j, 3) <= 0) {
                                color = -1;
                            } else if (func_150859AC(j, 4) <= 0) {
                                color = -1;
                            } else if (D_800E0BCC & (1 << j)) {
                                color = -1;
                            } else if (other->energy == 0) {
                                color = -1;
                            } else if ((other->mv->context_b & 0x7F) == 0x21) {
                                color = -1;
                            }
                        }
                        if (color != -1) {
                            dx = (other->xpos - px) * scale;
                            dz = (other->zpos - pz) * scale;
                            // matching: the tripled index plus a row-0 pointer, not D_800AB62C[color]
                            color = color + color + color;
                            rgb = D_800AB62C[0] + color;
                            if (dx * dx + dz * dz < radiusSq) {
                                gDPPipeSync(gfx++);
                                gDPSetEnvColor(gfx++, rgb[0], rgb[1], rgb[2], 255);
                                rx = (s32)((dx * cosv - dz * sinv) * zoom) + x;
                                ry = (s32)((dx * sinv + dz * cosv) * zoom) + y;
                                xh = rx + dotSize;
                                xl = rx - dotSize;
                                yh = ry + dotSize;
                                yl = ry - dotSize;
                                gSPScisTextureRectangle(gfx++, xl, yl, xh, yh, G_TX_RENDERTILE, 0, 0, 0x400, 0x400);
                                otherFloor = func_15085DA8(other->ypos);
                                if (floor != otherFloor) {
                                    gDPPipeSync(gfx++);
                                    gDPSetEnvColor(gfx++, 0, 0, 0, 255);
                                    if (floor < otherFloor) {
                                        gSPScisTextureRectangle(gfx++, xl, yl - dotSize, xh, yl, G_TX_RENDERTILE, 0, 0, 0x400, 0x400);
                                    } else if (otherFloor < floor) {
                                        gSPScisTextureRectangle(gfx++, xl, yh, xh, yh + dotSize, G_TX_RENDERTILE, 0, 0, 0x400, 0x400);
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
