#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 cosf(f32);
extern f32 sinf(f32);
extern f32 D_800A7154;
extern f32 D_800A7158;
extern void func_1510E82C(s32 *, s32, f32 *, s32, s32 *, s32, f32, f32, f32, f32, s32, s32);

/* Sprite animation (same layout as SpriteAnim in game_1944C0.c). */
typedef struct {
    /* 0x0 */ s32 *frames; // one segmented texture address (< 0x10000000) or a table of frameCount addresses
    /* 0x4 */ u8 frameCount;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u16 width;
    /* 0x8 */ u16 height;
    /* 0xA */ u8 unkA; // 5: two-texture (two-cycle) animation
} SpriteAnim;

extern SpriteAnim *D_8008CA4C[];

/* A textured sprite particle: one quad, drawn by func_151720C4. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Vtx vtx[4];
    /* 0x50 */ s16 lifetime;  // drawn while >= 2; 0 starts the -3 -> -1 removal countdown
    /* 0x52 */ s16 animFrame; // 8.8 fixed point frame index
    /* 0x54 */ s16 animSpeed;
    /* 0x56 */ u16 color;     // RGBA5551
    /* 0x58 */ u8 animIndex;  // index into D_8008CA4C
    /* 0x59 */ u8 flags;      // 1: cloud blend, 2: flat quad, 4: play animation once, 8: texture-only combine
} Particle19F150;

extern Particle19F150 *func_15167A68(s32, s32, s32, s32, s32, s32);

/* Allocates a particle and initialises its animation state. */
Particle19F150 *func_15171CA0(s16 lifetime, u16 color, u8 animIndex, s32 flags, s32 animSpeed, s32 arg5, u8 arg6,
                              s32 arg7) {
    Particle19F150 *particle = func_15167A68(arg5 == 0 ? 0xF : 0x43, arg7, sizeof(Particle19F150), 1, arg6, 1);

    if (particle == NULL) {
        return NULL;
    }
    particle->lifetime = lifetime;
    particle->animFrame = 0;
    particle->animSpeed = animSpeed;
    particle->color = color;
    particle->animIndex = animIndex;
    particle->flags = flags;
    particle->vtx[0].v.flag = 0;
    particle->vtx[1].v.flag = 0;
    particle->vtx[2].v.flag = 0;
    particle->vtx[3].v.flag = 0;
    return particle;
}

s32 func_15172B20(Vtx *vtx, f32 x, f32 y, f32 z, f32 angle, s32 dist, s32 size, u8 width);

/*
 * Spawns a sprite particle at (x, y, z). Normally the quad is laid on the floor below a point `dist` away along
 * `angle`; with flags & 2 it is a flat horizontal square of half-size `size`.
 */
void func_15171D4C(f32 x, f32 y, f32 z, s16 lifetime, u16 color, u8 animIndex, f32 angle, s32 dist, s32 size,
                   s32 flags, s32 animSpeed, s32 arg11, u8 arg12, s32 arg13) {
    Particle19F150 *particle;

    particle = func_15171CA0(lifetime, color, animIndex, flags, animSpeed, arg11, arg12, arg13);
    if (particle != NULL) {
        if (!(flags & 2)) {
            if (func_15172B20(particle->vtx, x, y, z, angle, dist, size, D_8008CA4C[particle->animIndex]->width) == 0) {
                func_1516972C((struct102 *)particle);
            }
        } else {
            particle->vtx[0].v.ob[0] = particle->vtx[3].v.ob[0] = x - (f32)size;
            particle->vtx[1].v.ob[0] = particle->vtx[2].v.ob[0] = (f32)size + x;
            particle->vtx[0].v.ob[1] = particle->vtx[1].v.ob[1] = particle->vtx[2].v.ob[1] = particle->vtx[3].v.ob[1] = y;
            particle->vtx[0].v.ob[2] = particle->vtx[1].v.ob[2] = z - (f32)size;
            particle->vtx[2].v.ob[2] = particle->vtx[3].v.ob[2] = (f32)size + z;
            particle->vtx[0].v.tc[0] = particle->vtx[3].v.tc[0] = 0x2000;
            particle->vtx[1].v.tc[0] = particle->vtx[2].v.tc[0] = (D_8008CA4C[animIndex]->width + 0x100) << 5;
            particle->vtx[0].v.tc[1] = particle->vtx[1].v.tc[1] = 0x2000;
            particle->vtx[2].v.tc[1] = particle->vtx[3].v.tc[1] = (D_8008CA4C[animIndex]->height + 0x100) << 5;
        }
    }
}

s32 func_151725FC(Vtx *vtx, s32 tri, f32 x, f32 y, f32 z, f32 angle, s32 size, s32 width);

/* Spawns a sprite particle laid on collision triangle `tri` at (x, y, z). */
void func_15171F04(f32 x, f32 y, f32 z, s32 tri, s16 lifetime, u16 color, u8 animIndex, s32 size, s32 flags,
                   s32 animSpeed, s32 arg10, u8 arg11, s32 arg12) {
    Particle19F150 *particle;

    particle = func_15171CA0(lifetime, color, animIndex, flags, animSpeed, arg10, arg11, arg12);
    if (particle != NULL) {
        if (func_151725FC(particle->vtx, tri, x, y, z, 0.0f, size, D_8008CA4C[particle->animIndex]->width) == 0) {
            func_1516972C((struct102 *)particle);
        }
    }
}

/* Per-frame update: counts the lifetime down by the frame delta and advances the animation. */
void func_15171FC0(Particle19F150 *particle) {
    s32 value;

    value = particle->lifetime;
    if (value < -1) {
        particle->lifetime = value + 1;
        if (particle->lifetime == -1) {
            func_1516972C((struct102 *)particle);
        }
        return;
    }

    if (value == 0) {
        particle->lifetime = -3;
        return;
    }

    if (value == -1) {
        return;
    }

    if (!(particle->flags & 4)) {
        if (D_800BE9E4 < value) {
            particle->lifetime = value - D_800BE9E4;
        } else {
            particle->lifetime = 0;
        }
    }

    particle->animFrame += particle->animSpeed;
    value = D_8008CA4C[particle->animIndex]->frameCount;
    if (particle->animFrame / 256 < value) {
        return;
    }

    if (particle->flags & 4) {
        particle->lifetime = -3;
    } else {
        particle->animFrame = (value << 8) - 0x100;
    }
}

extern s32 D_800DD1B4;
extern s32 D_800DD1B8;
extern s16 D_800DD1BC;
extern s16 D_800DD1C0;
extern s16 D_800DD1C2;
extern s16 D_800DD1C4;
extern s16 D_800DD1C6;
extern s32 D_800D2C9C;
struct Sprite151720C4 {
    s32 unk0[5];
};
Gfx *func_15094F70(Gfx *, void *, s32, void *, s32, s32, s32, s32, s32);
Gfx *func_15142FBC(Gfx *gfx, s32 modeH, s32 modeL, u8 *needSync);

/*
 * Draws a sprite particle (once its lifetime is >= 2), setting the texture, cycle type, render mode, combiner and
 * primitive colour only when they differ from the cached state in D_800DD1B4..D_800DD1C6.
 */
Gfx *func_151720C4(Gfx *gfx, Particle19F150 *particle, s32 arg2) {
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    s32 *frames;
    s32 cyc;
    s32 mode;
    s32 newCyc;
    s32 newMode;
    struct Sprite151720C4 sprite;
    s32 tex;
    s32 frame;
    s32 update;
    SpriteAnim *anim;
    s32 othermodeH;
    s32 othermodeL;

    update = 1;
    if (particle->lifetime >= 2) {
    anim = D_8008CA4C[particle->animIndex];
    frame = particle->animFrame >> 8;
    frames = anim->frames;
    tex = (s32)frames;
    if ((u32)frames >= 0x10000000) {
        tex = frames[frame];
        frame = 0;
    }
    if (tex != D_800DD1B4 || frame != D_800DD1B8) {
        gfx = func_15094F70(gfx, anim, particle->animFrame, &sprite, 0, 0, 0, 2, 3);
        D_800DD1B4 = tex;
        D_800DD1B8 = frame;
    }
    cyc = D_800DD1BC & 0xF;
    mode = D_800DD1BC & 0xF0;
    if (!(particle->flags & 1)) {
        newCyc = 1;
    } else {
        newCyc = 2;
    }
    if (anim->unkA == 5) {
        newCyc += 2;
    }
    if (cyc != newCyc) {
        cyc = newCyc;
        if (update) {
            gDPPipeSync(gfx++);
        }
        update = 0;
        switch (newCyc) {
        case 1:
            othermodeH = G_CYC_1CYCLE;
            othermodeL = G_RM_AA_ZB_XLU_DECAL | G_RM_AA_ZB_XLU_DECAL2;
            break;
        case 2:
            othermodeH = G_CYC_1CYCLE;
            othermodeL = G_RM_ZB_CLD_SURF | G_RM_ZB_CLD_SURF2;
            break;
        case 3:
            othermodeH = G_CYC_2CYCLE;
            othermodeL = G_RM_PASS | G_RM_AA_ZB_XLU_DECAL2;
            break;
        case 4:
            othermodeH = G_CYC_2CYCLE;
            othermodeL = G_RM_PASS | G_RM_ZB_CLD_SURF2;
            break;
        }
        gDPSetAlphaCompare(gfx++, G_AC_NONE);
        gSPClearGeometryMode(gfx++, G_FOG | G_LOD);
    }
    gfx = func_15142FBC(gfx, othermodeH | G_TP_PERSP | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_DISABLE, othermodeL, (u8 *)&update);
    if (!(particle->flags & 8)) {
        if (anim->unkA == 0) {
            if (particle->color) {
                newMode = 0x10;
            } else {
                newMode = 0x20;
            }
        } else if (anim->unkA == 5) {
            if (particle->color) {
                newMode = 0x60;
            } else {
                newMode = 0x50;
            }
        } else {
            newMode = 0x30;
        }
    } else {
        newMode = 0x40;
    }
    if (mode != newMode) {
        mode = newMode;
        if (update) {
            gDPPipeSync(gfx++);
        }
        switch (newMode) {
        case 0x10:
            gDPSetCombineLERP(gfx++, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0,
                              PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0);
            break;
        case 0x20:
            gDPSetCombineLERP(gfx++, 0, 0, 0, TEXEL0, PRIMITIVE, 0, TEXEL0, 0,
                              0, 0, 0, TEXEL0, PRIMITIVE, 0, TEXEL0, 0);
            break;
        case 0x30:
            gDPSetCombineLERP(gfx++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0,
                              0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0);
            break;
        case 0x40:
            gDPSetCombineLERP(gfx++, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0,
                              0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0);
            break;
        case 0x50:
            gDPSetCombineLERP(gfx++, 0, 0, 0, TEXEL0, PRIMITIVE, 0, TEXEL1, 0,
                              0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
            break;
        case 0x60:
            gDPSetCombineLERP(gfx++, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL1, 0,
                              0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
            break;
        }
    }
    D_800DD1BC = cyc | mode;
    r = (particle->color >> 8) & 0xF8;
    g = (particle->color >> 3) & 0xF8;
    b = (particle->color << 2) & 0xF8;
    if (particle->lifetime >= 0x1A) {
        a = 0xFF;
    } else {
        a = particle->lifetime * 10;
    }
    if (r != D_800DD1C0 || g != D_800DD1C2 || b != D_800DD1C4 || a != D_800DD1C6) {
        D_800DD1C0 = r;
        D_800DD1C2 = g;
        D_800DD1C4 = b;
        D_800DD1C6 = a;
        if (update) {
            gDPPipeSync(gfx++);
        }
        gDPSetPrimColor(gfx++, 1, 0, r, g, b, a);
    }
    gSPVertex(gfx++, particle->vtx, 4, 0);
    gSP1Triangle(gfx++, 0, 1, 2, 0);
    gSP1Triangle(gfx++, 0, 2, 3, 0);
    }
    return gfx;
}

typedef struct {
    s16 ob[3];
    u16 flag;
    s16 tc[2];
    u8 cn[4];
} Vtx151725FC;

extern s32 func_1510E388(s32, void *, f32 *, f32 *);
extern s16 D_800A6FD0[];
extern s16 D_800A6FE0[];
extern s16 D_800A6FF0[];

s32 func_151725FC(Vtx *arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7) {
    f32 lenA;
    f32 lenB;
    f32 minsq;
    f32 denom;
    f32 unused1;
    f32 unused2;
    f32 unused3;
    f32 v[2][3];
    f32 unused4;
    f32 angle;
    f32 z;
    f32 x;
    f32 cx;
    f32 cy;
    f32 cs;
    f32 sn;
    s16 (*tbl)[2];
    Vtx151725FC *vtx;
    s32 n;
    s32 i;

    vtx = (Vtx151725FC *)arg0;
    n = arg6 / 16;
    if (n == 0) {
        n = 1;
    }
    if (func_1510E388(arg1, 0, &x, &z) == 0) {
        return 0;
    }
    v[0][0] = 0.0f;
    v[0][2] = -n;
    v[0][1] = x * v[0][0] + z * v[0][2];
    denom = (z + z) * (-v[0][1] - v[0][1]) + ((-v[0][2] - v[0][2]) + (-v[0][2] - v[0][2]));
    if (denom == 0.0f) {
        denom = 0.001000000047f;
    }
    v[1][0] = -n;
    v[1][1] = ((x * v[1][0] + x * v[1][0]) * (-v[0][2] - v[0][2])) / denom;
    if (z == 0.0f) {
        v[1][2] = 0.0f;
    } else {
        v[1][2] = (v[1][1] - x * v[1][0]) / z;
    }
    lenA = v[0][0] * v[0][0] + v[0][1] * v[0][1] + v[0][2] * v[0][2];
    if (lenA > 100000000.0f) {
        lenA = 100000000.0f;
    }
    minsq = n * n;
    if (lenA < minsq) {
        lenA = minsq;
    }
    lenA = sqrtf(lenA);
    lenB = v[1][0] * v[1][0] + v[1][1] * v[1][1] + v[1][2] * v[1][2];
    if (lenB > 100000000.0f) {
        lenB = 100000000.0f;
    }
    if (lenB < minsq) {
        lenB = minsq;
    }
    lenB = sqrtf(lenB);
    for (i = 0; i < 3; i++) {
        v[0][i] = v[0][i] * arg6 / lenA;
        v[1][i] = v[1][i] * arg6 / lenB;
        vtx[0].ob[i] = v[0][i];
        vtx[1].ob[i] = v[1][i];
        vtx[2].ob[i] = -v[0][i];
        vtx[3].ob[i] = -v[1][i];
    }
    angle = (arg5 - 225.0f) * 0.01745329238f;
    cs = cosf(angle);
    sn = sinf(angle);
    if ((u8)arg7 == 0x10) {
        cy = cx = 256.0f;
        tbl = (s16 (*)[2])D_800A6FF0;
    } else if ((u8)arg7 == 0x20) {
        cy = cx = 512.0f;
        tbl = (s16 (*)[2])D_800A6FE0;
    } else {
        cy = cx = 1024.0f;
        tbl = (s16 (*)[2])D_800A6FD0;
    }
    for (i = 0; i < 4; i++) {
        ((Vtx151725FC *)arg0)[i].tc[0] = ((tbl[i][0] - cx) * cs - (tbl[i][1] - cy) * sn) + cx + 8192.0f;
        ((Vtx151725FC *)arg0)[i].tc[1] = ((tbl[i][0] - cx) * sn + (tbl[i][1] - cy) * cs) + cy + 8192.0f;
    }
    for (i = 0; i < 4; i++) {
        vtx[i].ob[0] = vtx[i].ob[0] + arg2;
        vtx[i].ob[1] = vtx[i].ob[1] + arg3;
        vtx[i].ob[2] = vtx[i].ob[2] + arg4;
    }
    return 1;
}


s32 func_15172B20(Vtx *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, u8 arg7) {
    struct sp15172B20 {
        s32 unk0;
        f32 unk4;
        s32 unk8;
        s32 unkC;
    };
    f32 temp_f0;
    f32 temp_f6;
    struct sp15172B20 sp40;
    f32 temp_f12;

    temp_f12 = (arg4 - 90.0f) * D_800A7154;
    temp_f6 = cosf(temp_f12);
    temp_f0 = sinf(temp_f12);
    arg1 += (f32) arg5 * temp_f0;
    arg3 += (f32) arg5 * temp_f6;
    func_1510E82C(&sp40.unk0, 0, &sp40.unk4, 0, &sp40.unk8, 0, arg1, arg2, arg3, arg2, 0, 0);
    if (sp40.unk0 == 0) {
        return 0;
    }
    if (sp40.unk4 == D_800A7158) {
        return 0;
    }
    return func_151725FC(arg0, sp40.unk0, arg1, sp40.unk4, arg3, arg4, arg6, arg7);
}
