#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A05AC;
extern f32 D_800A05B0;
extern f32 D_800A05B4;
extern f32 D_800A05B8;
extern f32 D_800A05BC;
extern f32 D_800A05C0;
extern f32 D_800A05C4;
extern f32 D_800A05C8;
extern f32 D_800A05CC;
extern f32 D_800A05D0;
extern f32 D_800A05D4;
extern f32 D_800A05D8;
extern f32 D_800A0570;
extern f32 D_800A0574;
extern f32 D_800A0578;
extern f32 D_800A057C;
extern f32 D_800A04F0;
extern f32 D_800A04F4;
extern f32 D_800A04F8;
extern f32 D_800A04FC;

void func_15143794(s32 heading, s32 pitch, f32 speed, f32 *out);
struct260 *func_15130374(void *, u8, s32, u8, s32);
s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void func_1503E5F8(f32 (*)[4], f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern struct260 *func_15073118(struct108 *, s32, s32, f32, f32, f32, f32, f32, f32, f32, s32);

typedef struct {
    u8 pad0[2];
    s8 unk2;
    u8 pad3;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} Struct150C8350;
void func_150C8350(void);

extern void func_150C8350(void);
void func_150C84F4(s32 arg0);

void func_150CDB6C(s32);
void func_150C851C(s32 arg0);


typedef struct {
    u8 unk0;
    u8 pad1[3];
    s16 unk4;
    s16 unk6;
    s16 unk8;
} Struct150C8600;

extern s32 D_80090230[5];
extern s32 D_80090244[5];
extern s32 func_1510D0EC(s32, s32 *, s32, s32);

s32 func_1510D0EC(s32, s32 *, s32, s32);
extern s32 *D_80088810[2];

typedef struct GameF5800Command {
    u32 w0;
    u32 w1;
} GameF5800Command;
GameF5800Command *func_150C8600(GameF5800Command *arg0);

typedef struct {
    u8 pad0[0x34];
    Mtx *unk34;
} Struct150C8730Arg0;

typedef struct {
    f32 sp48;
    f32 sp4C;
    f32 sp50;
    f32 sp54;
    f32 sp58;
    f32 sp5C;
    f32 sp60;
    f32 sp64[4][4];
    u8 padA4[0x14];
} Struct150C8730Stack;
s32 func_150C8730(Struct150C8730Arg0 *arg0, struct108 *arg1);

extern void *func_15167A68(s32, s32, s32, s32, u8, s32);

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Spawn parameters for an expanding ring effect (copied into RadialEffect.def). */
typedef struct {
    Vec3f pos;
    u8 flags;          /* 0x0C: bit 0 = expires when remaining runs out */
    s8 checkCallback;  /* 0x0D: index into D_80088860, -1 = none */
    f32 speed;         /* 0x10: radius growth per tick */
    s32 count;         /* 0x14: ring segments */
    s16 remaining;     /* 0x18: ticks left */
    s16 growUntil;     /* 0x1A */
    f32 growRate;      /* 0x1C */
    s16 shrinkFrom;    /* 0x20 */
    f32 shrinkRate;    /* 0x24 */
    f32 emitRate;      /* 0x28: emissions per unit of circumference per tick */
    s8 emitCallback;   /* 0x2C: index into D_80088858, -1 = none */
    s8 updateCallback; /* 0x2D: index into D_80088864, -1 = none */
} RadialEffectDef; /* size 0x30 */

typedef struct {
    f32 x; /* sin of the segment angle */
    f32 z; /* cos of the segment angle */
} RadialDir;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    RadialEffectDef def; /* 0x10 */
    f32 size;            /* 0x40 */
    f32 radius;          /* 0x44 */
    f32 step;            /* 0x48: angle between segments */
    s16 duration;        /* 0x4C: initial def.remaining */
    f32 pending;         /* 0x50: emissions owed, see func_150C8A68 */
    Vtx *vtx[2];         /* 0x54: double-buffered by D_800BE9C0 */
    u8 pad5C[0x304];
    RadialDir *dirs;     /* 0x360 */
    u8 *unk364;
    u8 data[1];          /* 0x368 */
} RadialEffect;

/*
 * Allocates a ring effect from def. Behind the header: dirs[count], two vertex buffers of
 * (count + 1) * 5 Vtx each, then extraSize bytes at unk364.
 */
RadialEffect *func_150C88D0(RadialEffectDef *def, s32 extraSize, u8 arg2, s32 arg3) {
    RadialEffect *effect;
    s32 i;

    effect = func_15167A68(0x31, arg3, (s32)((Vtx (*)[2])((u8 *)extraSize + def->count * 8 + 0x408) + def->count * 5), 1, arg2, 1);
    if (effect == NULL) {
        return NULL;
    }
    memcpy(&effect->def, def, sizeof(RadialEffectDef));
    effect->dirs = (RadialDir *)effect->data;
    effect->vtx[0] = (Vtx *)((u8 *)effect + def->count * 8 + 0x368);
    effect->vtx[1] = (Vtx *)((u8 *)effect + def->count * 8 + 0x3B8) + def->count * 5;
    effect->unk364 = (u8 *)((Vtx (*)[2])((u8 *)effect + def->count * 8 + 0x408) + def->count * 5);

    effect->size = 0.0f;
    effect->radius = 0.0f;
    effect->step = 6.28318548f / effect->def.count;
    effect->pending = 0.0f;
    effect->duration = effect->def.remaining;
    i = 0;
    {
        f32 angle = 0.0f;
        for (; i < effect->def.count; i++) {
            effect->dirs[i].x = sinf(angle);
            effect->dirs[i].z = cosf(angle);
            angle += effect->step;
        }
    }
    return effect;
}

extern void (*D_80088858[])(RadialEffect *, f32 *, u8, f32, f32);
extern s32 (*D_80088860[])(RadialEffect *);
extern void (*D_80088864[])(RadialEffect *, f32);

/* Per-tick update of an expanding ring: ages it, grows its radius and emits along its edge. */
void func_150C8A68(RadialEffect *effect) {
    s32 pad; // matching: unused stack slot
    u8 expired;
    f32 oldRadius;
    f32 cosine;
    f32 sine;
    s32 bias;
    f32 emitPos[3];
    u8 angle;

    expired = 0;
    if (effect->def.flags & 1) {
        effect->def.remaining -= D_800BE9E4;
        if (effect->def.remaining < 0) {
            expired = 1;
        }
    }
    if (!expired) {
        s16 remaining;
        s16 age;

        oldRadius = effect->radius;
        effect->radius += effect->def.speed * D_800BE9A4;
        // matching: remaining must be loaded inside the condition, after growUntil
        if (effect->def.growUntil < (remaining = effect->def.remaining)) {
            age = effect->duration - remaining;
            effect->size = age * effect->def.growRate;
        }
        if (remaining < effect->def.shrinkFrom) {
            effect->size = remaining * effect->def.shrinkRate;
        }

        if (effect->def.emitCallback != -1) {
            effect->pending += effect->def.emitRate * D_800BE9A4 * (6.283185482f * effect->radius);
            while (effect->pending > 1.0f) {
                bias = D_800DBFF0[D_80082FA4].unk380 * 0.7111111283f;
                angle = (func_150ADA20() % 81U) + bias - 40;
                cosine = func_151423D8(angle - 64);
                sine = func_151423D8(angle);
                emitPos[0] = effect->def.pos.x + cosine * effect->radius;
                emitPos[1] = effect->def.pos.y;
                emitPos[2] = effect->def.pos.z + sine * effect->radius;
                if (emitPos[2] * emitPos[2] + emitPos[0] * emitPos[0] < 10894295.0f) {
                    D_80088858[effect->def.emitCallback](effect, emitPos, angle, cosine, sine);
                }
                effect->pending -= 1.0f;
            }
        }
        if (effect->def.updateCallback != -1) {
            D_80088864[effect->def.updateCallback](effect, oldRadius);
        }
        if (effect->def.checkCallback != -1 && D_80088860[effect->def.checkCallback](effect) == 0) {
            expired = 1;
        }

    }
    if (expired) {
        func_1516972C((struct102 *)effect);
    }
}

typedef struct {
    f32 r;
    f32 y;
} Pt150C8DB8;

typedef struct {
    s32 count;
    Pt150C8DB8 *pts;
} Ring150C8DB8;

void func_150CCD90(f32, f32 *, f32 *, f32 *);
extern Ring150C8DB8 D_80088820;
extern s32 *D_80090E84;
extern struct { s32 unk0; s32 unk4; } D_800A4AC8[];
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *, s32, s32, s32, s32, s32, s32, u8 *);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

/*
 * Draws a ring effect as a strip of quads: for every segment direction, the ring profile
 * D_80088820 is swept out at the current radius into this frame's vertex buffer.
 */
Gfx *func_150C8DB8(Gfx *gdl, RadialEffect *effect, s32 arg2) {
    RadialEffect *first; // matching: a second, stack-homed copy of effect

    first = effect;
    {
    s32 palOffset;
    u8 flag;
    s32 vtxCount;
    s32 pt;
    s32 seg;
    s32 stripLen;
    s32 pad1; // matching: frame padding

    flag = 1;
    vtxCount = 0;
    gdl = func_15142B7C(gdl, 0x200205, 0x70400);
    gdl = func_15142C10(gdl, 0xFF, 0xFF, 0xFF, 0xFF, &flag);
    gdl = func_15142CF0(gdl, 0xF2, 0, 0xFF, 0xFF, 0xFF, 0xFF, &flag);
    gdl = func_1513F4E4(gdl, 0x21, &flag);
    gdl = func_15142FBC(gdl, 0x1DACA0, D_800A4AC8[14].unk0 | D_800A4AC8[14].unk4, &flag);
    {
    s32 texAddr = func_1510D0EC(*D_80090E84, &palOffset, 3, 0);


    gSPTexture(gdl++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetTextureImage(gdl++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, texAddr);
    gDPSetTile(gdl++, G_IM_FMT_CI, G_IM_SIZ_16b, 0, 0x0, G_TX_LOADTILE, 0, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadBlock(gdl++, G_TX_LOADTILE, 0, 0, 0x3EF, 0);
    gDPSetTile(gdl++, G_IM_FMT_CI, G_IM_SIZ_4b, 2, 0x0, 1, 0, G_TX_WRAP, 6, G_TX_NOLOD, G_TX_WRAP, 5, G_TX_NOLOD);
    gDPSetTileSize(gdl++, 1, 0x402, 0x402, 0x47E, 0x4FE);
    gDPSetTile(gdl++, G_IM_FMT_CI, G_IM_SIZ_4b, 1, 0x80, 2, 0, G_TX_WRAP, 5, 1, G_TX_WRAP, 4, 1);
    gDPSetTileSize(gdl++, 2, 0x202, 0x202, 0x23E, 0x27E);
    gDPSetTile(gdl++, G_IM_FMT_CI, G_IM_SIZ_4b, 1, 0xA0, 3, 0, G_TX_WRAP, 4, 2, G_TX_WRAP, 3, 2);
    gDPSetTileSize(gdl++, 3, 0x102, 0x102, 0x11E, 0x13E);
    gDPSetTile(gdl++, G_IM_FMT_CI, G_IM_SIZ_4b, 1, 0xB0, 4, 0, G_TX_WRAP, 3, 3, G_TX_WRAP, 2, 3);
    gDPSetTileSize(gdl++, 4, 0x82, 0x82, 0x8E, 0x9E);
    gDPSetTile(gdl++, G_IM_FMT_CI, G_IM_SIZ_4b, 1, 0xB8, 5, 0, G_TX_WRAP, 2, 4, G_TX_WRAP, 1, 4);
    gDPSetTileSize(gdl++, 5, 0x42, 0x42, 0x46, 0x4E);
    gDPSetTile(gdl++, G_IM_FMT_IA, G_IM_SIZ_4b, 2, 0xBC, G_TX_RENDERTILE, 0, G_TX_WRAP, 5, 15, G_TX_WRAP, 5, 15);
    gDPSetTileSize(gdl++, G_TX_RENDERTILE, 0x802, 0x802, 0x87E, 0x87E);
    gDPSetTextureImage(gdl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, palOffset + texAddr - 0x20);
    gDPSetTile(gdl++, G_IM_FMT_IA, G_IM_SIZ_4b, 0, 0x100, 6, 0, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gdl++);
    gDPLoadTLUTCmd(gdl++, 6, 15);
    gSPTexture(gdl++, 0xFFFF, 0xFFFF, 4, G_TX_RENDERTILE, G_ON);
    }
    {
        Vtx *vtxBase;

        {
            f32 dirX;
            f32 dirZ;

            seg = 0;
            dirX = first->dirs[0].x;
            dirZ = first->dirs[0].z;
            vtxBase = effect->vtx[D_800BE9C0];
            {
                s32 inRange;
                f32 centerX;
                f32 centerZ;
                s32 pad[6]; // matching: frame padding

                centerX = first->def.pos.x + first->radius * dirX;
                centerZ = first->def.pos.z + first->radius * dirZ;
                if (centerX * centerX + centerZ * centerZ < 10894295.0f) {
                    inRange = 1;
                } else {
                    inRange = 0;
                }
                for (pt = 0; pt < D_80088820.count; pt++) {
                    f32 dist;
                    f32 y;
                    f32 pad; // matching: unused local keeps the frame layout
                    f32 x;
                    f32 z;

                    dist = (D_80088820.pts + pt)->r * effect->size;
                    y = (D_80088820.pts + pt)->y * effect->size;
                    // matching: x and z are built in two steps
                    x = centerX;
                    x += dist * dirX;
                    y = (inRange ? y : 0.0f) + effect->def.pos.y;
                    z = centerZ;
                    z += dist * dirZ;
                    vtxBase[vtxCount].v.ob[0] = x;
                    vtxBase[vtxCount].v.ob[1] = y;
                    vtxBase[vtxCount].v.ob[2] = z;
                    vtxBase[vtxCount].v.tc[0] = -((x * 0.0398866199f + z * 0.03713737056f + 110.2300644f) * 32.0f);
                    vtxBase[vtxCount].v.tc[1] = -((x * 0.05412395298f + z * -0.05814454332f + 50.14429092f) * 32.0f);
                    {
                        f32 r;
                        f32 g;
                        f32 b;

                        func_150CCD90(z, &r, &g, &b);
                        vtxBase[vtxCount].v.cn[0] = r;
                        vtxBase[vtxCount].v.cn[1] = g;
                        vtxBase[vtxCount].v.cn[2] = b;
                    }
                    vtxBase[vtxCount].v.cn[3] = inRange ? 0xFF : 0;
                    vtxCount++;
                }
            }
        }

        for (; seg < first->def.count; seg++) {
            s32 next;
            f32 dirX;
            f32 dirZ;

            pt = 0;
            next = (seg + 1 == effect->def.count) ? 0 : seg + 1;
            dirX = effect->dirs[next].x;
            dirZ = effect->dirs[next].z;
            {
                s32 inRange;
                f32 centerX;
                f32 centerZ;
                s32 pad; // matching: frame padding

                centerX = effect->def.pos.x + effect->radius * dirX;
                centerZ = effect->def.pos.z + effect->radius * dirZ;
                if (centerX * centerX + centerZ * centerZ < 10894295.0f) {
                    inRange = 1;
                } else {
                    inRange = 0;
                }
                for (; pt < D_80088820.count; pt++) {
                    f32 dist;
                    f32 y;
                    f32 pad; // matching: unused local keeps the frame layout
                    f32 x;
                    f32 z;

                    dist = (D_80088820.pts + pt)->r * effect->size;
                    y = (D_80088820.pts + pt)->y * effect->size;
                    // matching: x and z are built in two steps
                    x = centerX;
                    x += dist * dirX;
                    y = (inRange ? y : 0.0f) + effect->def.pos.y;
                    z = centerZ;
                    z += dist * dirZ;
                    vtxBase[vtxCount].v.ob[0] = x;
                    vtxBase[vtxCount].v.ob[1] = y;
                    vtxBase[vtxCount].v.ob[2] = z;
                    vtxBase[vtxCount].v.tc[0] = -((x * 0.0398866199f + z * 0.03713737056f + 110.2300644f) * 32.0f);
                    vtxBase[vtxCount].v.tc[1] = -((x * 0.05412395298f + z * -0.05814454332f + 50.14429092f) * 32.0f);
                    {
                        f32 r;
                        f32 g;
                        f32 b;

                        func_150CCD90(z, &r, &g, &b);
                        vtxBase[vtxCount].v.cn[0] = r;
                        vtxBase[vtxCount].v.cn[1] = g;
                        vtxBase[vtxCount].v.cn[2] = b;
                    }
                    vtxBase[vtxCount].v.cn[3] = inRange ? 0xFF : 0;
                    vtxCount++;
                }
            }
            {
                s32 pad; // matching: frame padding
                stripLen = D_80088820.count * 2;
                gSPVertex(gdl++, &effect->vtx[D_800BE9C0][vtxCount - stripLen], stripLen, 0);
                for (pt = 0; pt < D_80088820.count - 1; pt++) {
                    gSP1Triangle(gdl++, pt, pt + 1, pt + D_80088820.count + 1, 0);
                    gSP1Triangle(gdl++, pt, pt + D_80088820.count + 1, pt + D_80088820.count, 0);
                }
            }
        }
    }
    }
    return gdl;
}

/* Spawn packet for func_15130374 particles; the spawned object holds these fields at +0x10. */
typedef struct {
    s32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    u8 color0[4];     /* 0x14: RGBA */
    u8 color1[4];     /* 0x18: RGBA */
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    Vec3f pos;        /* 0x30 */
    f32 unk3C[3];
    f32 velocity[3];  /* 0x48: damped per tick by func_150CAC28 */
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    u8 pad64[0xC];
} Particle; /* size 0x70 */

/* Ring emit callback 0: spawns a dark particle at pos, moving along heading. */
void func_150C99B4(RadialEffect *effect, Vec3f *pos, u8 heading, f32 cosine, f32 sine) {
    Particle packet;
    s32 pad; // matching: unused stack slot
    s16 pitch;

    f32 speed;
    s32 flag40;
    s32 flag80;

    packet.unk1D = 0x26;
    packet.unk08 = 0xC01;
    packet.unk00 = 0x200005;
    packet.unk04 = 0;
    packet.unk0A = (func_150ADA20() % 81U) + 25;
    packet.unk0C = 0;
    packet.unk10 = 0;
    packet.unk60 = 5;
    packet.unk61 = 5;
    packet.color0[0] = 0;
    packet.color0[1] = 0;
    packet.color0[2] = 0;
    packet.color0[3] = 0xFF;
    packet.color1[0] = 0;
    packet.color1[1] = 0;
    packet.color1[2] = 0;
    packet.color1[3] = (func_150ADA20() % 156U) + 100;
    packet.unk1C = 0xFF;
    packet.unk28 = packet.unk2C = (func_150ADA68() * 500.0f) + 500.0f;
    packet.pos = *pos;
    packet.unk3C[0] = 0.0f;
    packet.unk3C[1] = 0.0f;
    packet.unk3C[2] = 0.0f;
    packet.unk54 = ((func_150ADA68() * 550.0f) + -1416.0f) * 0.001000000047f;
    packet.unk1E = 20;
    packet.unk20 = 12;
    packet.unk22 = 0;
    packet.unk24 = 1.0f;
    flag40 = (func_150ADA20() & 1) ? 0x40 : 0;
    flag80 = (func_150ADA20() & 1) ? 0x80 : 0;
    packet.unk58 = 7 | flag80 | flag40 | 0x200;
    packet.unk62 = -1;
    packet.unk63 = -1;
    pitch = (func_150ADA20() % 11U) - 18;
    speed = func_150ADA68() * (effect->def.speed * 0.2430000156f) + effect->def.speed * 0.9610000253f;

    func_15143794(heading, pitch, speed, packet.velocity);
    func_15130374(&packet, 1, 0, effect->unkC, effect->unk1);
}

extern s32 func_1505D024(struct127 *, s32, u16, s32);

/* Ring update callback: hits actors that lie up to 250 units inside the ring edge. */
void func_150C9BDC(RadialEffect *effect, f32 oldRadius) {
    s32 i;
    struct127 *actor;
    f32 dx;
    f32 dz;
    f32 dist;
    f32 behind;

    for (i = 0; i < 25; i++) {
        actor = &D_800CC2D0[i];
        if ((actor->interaction_state != 0) && (actor->unk28 < 20.0f) && (actor->immune == 0) &&
            ((actor->id == 0x53) || (actor->interaction_state == 1))) {
            dx = actor->x_position - effect->def.pos.x;
            dz = actor->z_position - effect->def.pos.z;
            dist = sqrtf((dx * dx) + (dz * dz)) - 100.0f;
            if ((actor->id == 0x53) && (actor->unk13C == 0)) {
                dist -= 150.0f;
            }
            behind = effect->radius - dist;
            if ((behind >= 0.0f) && (behind < 250.0f)) {
                if (actor->id == 0x53) {
                    actor->disable_run = 10;
                    actor->unk218 = NULL;
                    actor->unk232 = 0x13;
                    if (D_800CC2D0[actor->unk124].unk65 != 0) {
                        actor->unk232 = 0x14;
                    }
                } else {
                    func_1505D024(actor, 0x6000E, actor->unk7A, -1);
                }
            }
        }
    }
}

/* Spawn packet for func_15132A4C objects; the spawned object holds these fields at +0x10. */
typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 rotation[3]; /* 0x10: degrees */
    f32 scale[3];    /* 0x1C */
    Vec3f pos;       /* 0x28 */
    f32 velocity[3]; /* 0x34 */
    f32 spin[3];     /* 0x40: rotation per tick */
    f32 gravity;     /* 0x4C */
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
    s32 unk6C;
    u8 unk70;
    s16 unk72;
    s16 unk74;
    u8 pad76[6];
} BurstPacket; /* size 0x7C */

/* Ring emit callback 1: spawns a tumbling burst object at pos (the colour from func_150CCD90 goes unused). */
void func_150C9DC4(RadialEffect *effect, Vec3f *pos, u8 heading, f32 cosine, f32 sine) {
    BurstPacket packet;
    f32 r;
    f32 g;
    f32 b;

    func_150CCD90(pos->z, &r, &g, &b);
    {
        typedef s32 T; // matching: a scope with a declaration orders the struct copy
        packet.pos = *pos;
    }
    packet.scale[0] = 1.0f;
    packet.scale[1] = 1.0f;
    packet.scale[2] = 1.0f;
    packet.unk50 = 0x29E8;
    packet.rotation[0] = func_150ADA68() * 360.0f;
    packet.rotation[1] = func_150ADA68() * 360.0f;
    packet.rotation[2] = func_150ADA68() * 360.0f;
    packet.unk56 = 0x20;
    packet.unk04 = 0.200000003f;
    packet.spin[1] = 0.0f;
    packet.unk00 = 1.0f;
    // matching: the original func_150ADA20 returned a full word; its spill is the
    // call-argument slot at sp+0x20, 0x14 below packet
    ((s32 *)&packet)[-5] = func_150ADA20();
    func_15143794(heading, (s16)((((s32 *)&packet)[-5] % 26U) - 45),
                  (func_150ADA68() * 81.0f + 60.0f) * effect->def.speed * 0.009999999776f,
                  packet.velocity);
    packet.spin[0] = (func_150ADA68() * 260.0f + -130.0f) * 0.1000000015f;
    packet.spin[2] = (func_150ADA68() * 260.0f + -130.0f) * 0.1000000015f;
    packet.unk54 = (func_150ADA20() % 41U) + 40;
    packet.gravity = (func_150ADA68() * 1533.0f + -2500.0f) * 0.001000000047f;
    packet.unk08 = packet.unk0C = (func_150ADA68() * 400.0f + 199.0f) * 0.001000000047f;
    packet.unk58 = 0;
    packet.unk5C = 0;
    packet.unk60 = (func_150ADA20() % 101U) + 155;
    packet.unk61 = 7;
    packet.unk62 = 0;
    packet.unk63 = 0;
    packet.unk64 = 0;
    packet.unk65 = 0;
    packet.unk66 = 0;
    packet.unk67 = 0;
    packet.unk68 = 0;
    packet.unk6A = 2;
    packet.unk6C = 0;
    packet.unk70 = 0;
    packet.unk72 = 0x20;
    packet.unk74 = 7;
    func_15132A4C(&packet, 3, 0xFF, 0, effect->unkC, effect->unk1);
}

/* Spawned burst particle: the BurstPacket fields live at +0x10. */
typedef struct {
    u8 pad0[0x20];
    f32 rotation[3]; /* 0x20 */
    u8 pad2C[0xC];
    f32 position[3]; /* 0x38 */
    f32 velocity[3]; /* 0x44 */
    f32 spin[3];     /* 0x50 */
    f32 gravity;     /* 0x5C */
} BurstParticle;

/* Integrates a burst particle's position (with gravity) and rotation over one frame. */
s32 func_150CA07C(BurstParticle *particle) {
    particle->position[0] += particle->velocity[0] * D_800BE9A4;
    particle->position[1] += (particle->velocity[1] * D_800BE9A4) + (particle->gravity * D_800BE9A4 * D_800BE9A4 * 0.5f);
    particle->position[2] += particle->velocity[2] * D_800BE9A4;
    particle->velocity[1] += particle->gravity * D_800BE9A4;
    particle->rotation[0] += particle->spin[0] * D_800BE9A4;
    particle->rotation[1] += particle->spin[1] * D_800BE9A4;
    particle->rotation[2] += particle->spin[2] * D_800BE9A4;
    return 1;
}

extern void func_1514C678(f32 x, f32 y, f32 z, f32 radius, s16, s16, s16, u8, s32, f32, void *, u8);

extern f32 D_800A05A0;
extern f32 D_800A05A4;
extern f32 D_800A05A8;
extern s32 func_15193CA0[];
extern s32 func_151942B0[];
void func_1504715C(void *, void *);
s32 func_15046C80(void *, s32, f32, void *);
void func_150CCD90(f32, f32 *, f32 *, f32 *);

void func_150CA930(Vec3f *pos);

typedef struct {
    Vec3f pos;
    u8 unkF0;
    s8 unkF1;
    f32 unkF4;
    s32 unkF8;
    s16 unkFC;
    s16 unkFE;
    f32 unk100;
    s16 unk104;
    f32 unk108;
    f32 unk10C;
    u8 unk110;
    u8 unk111;
} Def_150CA150;

typedef struct {
    s32 unk0;
    s16 unk4;
    u8 unk6;
    u8 pad7;
    s32 unk8;
    s32 unkC;
    u8 unk10, unk11, unk12, unk13;
    u8 unk14, unk15, unk16, unk17;
    s32 unk18;
} Fx_150CA150;

void func_150CA150(u8 *arg0) {
    Vec3f pos;
    f32 r, g, b;
    u8 cr, cg, cb;
    f32 floor[9];
    u8 hit;
    u32 end;
    u32 p;
    s32 sum;
    u8 padA[4];

    hit = 0;
    if (arg0 != NULL) {
        func_1504715C(floor, arg0);
        func_1512D748(&D_800DBFF0[D_800BE9E8], 7, 1);
        pos.x = *(f32 *)(arg0 + 0x14);
        pos.y = *(f32 *)(arg0 + 0x18) + 100.0f;
        pos.z = *(f32 *)(arg0 + 0x1C);
        if (func_15046C80(&pos, 0, *(f32 *)(arg0 + 0x18) - 300.0f, floor) != 0) {
            hit = 1;
            pos.y = floor[0];
        } else {
            pos.y = *(f32 *)(arg0 + 0x180);
        }
        sum = 0;
        end = (u32)func_151942B0;
        for (p = (u32)func_15193CA0; p < end; p += 4) {
            sum ^= ~*(s32 *)p;
        }
        if (sum != 0x79844C6B) {
            D_800C3E7C = 0;
        }
        func_150CCD90(pos.z, &r, &g, &b);
        cr = r;
        cg = g;
        cb = b;
        func_151D5404((struct17 *)&pos, 500.0f, 5000.0f, 0.0002f, 0xC, 0xF, 0xFF, 0);
        {
            RadialEffectDef def;
            u8 padB[0x14];

            def.pos = pos;
            def.speed = 43.20000076f;
            def.flags = 1;
            def.checkCallback = -1;
            def.count = 13;
            def.remaining = 101;
            def.growUntil = 88;
            def.growRate = 21.80000114f / (def.remaining - def.growUntil);
            def.shrinkFrom = 19;
            def.shrinkRate = 21.80000114f / def.shrinkFrom;
            def.emitCallback = 1;
            def.updateCallback = 0;
            def.emitRate = 7.999999798e-05f;
            func_150C88D0(&def, 0, 0xFF, 1);

        }
        if (hit != 0) {
            Fx_150CA150 fx;
            f32 d0;
            f32 r2;
            f32 sc;
            u8 ang;
            f32 sinv, cosv;
            u8 padC[8];
            s16 col[4][5];
            s16 randomArg;
            u8 spark[3];

            ang = -0x40 - (*(u16 *)(arg0 + 0x7A) >> 8);
            cosv = func_151423D8(ang);
            sinv = func_151423D8(ang - 0x40);
            r2 = sc = (func_150ADA68() * 96.0f + 352.0f) * 0.5f;
            fx.unk0 = 0x701;
            func_150ADA20();
            fx.unk4 = 0x408;
            fx.unk8 = 0;
            fx.unkC = 0;
            fx.unk10 = func_150ADA20() % 41U + 0xB4;
            fx.unk11 = 0xFF;
            fx.unk12 = 0xFF;
            fx.unk13 = 0xFF;
            fx.unk16 = 0;
            fx.unk14 = 0xFF;
            fx.unk15 = 0xFF;
            fx.unk18 = 0x260001;
            fx.unk17 = 6;
            col[0][3] = 0xFF;
            col[0][4] = 0;
            col[1][3] = 0xFF;
            col[1][4] = 0;
            col[2][3] = 0xFF;
            col[2][4] = 0;
            col[3][3] = 0xFF;
            col[3][4] = 0;
            fx.unk6 = 0x51;
            col[0][0] = cr;
            col[1][0] = cr;
            col[2][0] = cr;
            col[3][0] = cr;
            col[0][1] = cg;
            col[1][1] = cg;
            col[2][1] = cg;
            col[3][1] = cg;
            col[0][2] = cb;
            col[1][2] = cb;
            col[2][2] = cb;
            col[3][2] = cb;
            func_1513C5B0(&fx, col, 0xC, 0, sc * cosv + pos.x - sc * sinv, pos.y + 10.0f, sc * cosv + pos.z + sc * sinv, r2, r2, ang, 0, 0, 0xFF, 0);
            fx.unk6 = 0x52;
            func_1513C5B0(&fx, col, 0xC, 0, sc * cosv + pos.x - (-1.0f * sc) * sinv, pos.y + 10.0f, (-1.0f * sc) * cosv + pos.z + sc * sinv, r2, r2, ang, 0, 0, 0xFF, 0);
            fx.unk6 = 0x53;
            func_1513C5B0(&fx, col, 0xC, 0, (-1.0f * sc) * cosv + pos.x - sc * sinv, pos.y + 10.0f, sc * cosv + pos.z + (-1.0f * sc) * sinv, r2, r2, ang, 0, 0, 0xFF, 0);
            fx.unk6 = 0x54;
            func_1513C5B0(&fx, col, 0xC, 0, (-1.0f * sc) * cosv + pos.x - (-1.0f * sc) * sinv, pos.y + 10.0f, (-1.0f * sc) * cosv + pos.z + (-1.0f * sc) * sinv, r2, r2, ang, 0, 0, 0xFF, 0);
            func_150CA930(&pos);
            randomArg = func_150ADA20() % 21U + 10;
            spark[0] = cr;
            spark[1] = cg;
            spark[2] = cb;
            func_1514C678(pos.x, pos.y, pos.z, func_150ADA68() * 81.0f + 150.0f, 0, 0xFF, randomArg, 0x14, 0, 0.0f, spark, 0xFF);
        }
    }
}


void func_150CA930(Vec3f *pos) {
    s16 randomArg;

    randomArg = (func_150ADA20() % 21U) + 10;
    func_1514C678(pos->x, pos->y, pos->z, (func_150ADA68() * 59.0f) + 170.0f,
                  0, 0xFF, randomArg, 0x13, 0, 0.0f, NULL, 0xFF);
}

typedef struct {
    u8 pad0[0x1C];
    s16 timer;
    u8 pad1E[0xA];
    u8 alpha;
} Struct150CA9D0;

/* Fades the object out over its last 32 ticks by capping alpha at timer * 8. */
s32 func_150CA9D0(Struct150CA9D0 *arg0) {
    s32 maxAlpha;

    if (arg0->timer < 32) {
        maxAlpha = arg0->timer * 8;
        if (maxAlpha < arg0->alpha) {
            arg0->alpha = maxAlpha;
        }
    }
    return 1;
}

s32 func_150CAA04(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s16 arg8, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 arg14) {
    struct260 *ret;
    struct {
        s32 temp;
        u8 pad28[4];
        f32 sp2C;
        u8 pad30[4];
        struct {
            s32 unk00;
            s32 unk04;
            s16 unk08;
            s16 unk0A;
            s32 unk0C;
            s32 unk10;
            u8 unk14;
            u8 unk15;
            u8 unk16;
            u8 unk17;
            u8 unk18;
            u8 unk19;
            u8 unk1A;
            u8 unk1B;
            u8 unk1C;
            u8 unk1D;
            s16 unk1E;
            s16 unk20;
            s16 unk22;
            f32 unk24;
            f32 unk28;
            f32 unk2C;
            f32 unk30;
            f32 unk34;
            f32 unk38;
            u8 pad3C[0xC];
            f32 unk48;
            f32 unk4C;
            f32 unk50;
            f32 unk54;
            s32 unk58;
            u8 pad5C[4];
            u8 unk60;
            u8 unk61;
            u8 unk62;
            s8 unk63;
        } sp34;
        u8 pad98[0xC];
    } stack;

    stack.sp34.unk1D = 0x29;
    stack.sp34.unk08 = 0xE03;
    stack.sp34.unk00 = 0x200005;
    stack.sp34.unk04 = 0;
    stack.sp34.unk0A = (func_150ADA20() % 0x1AU) + 0x19;
    stack.sp34.unk0C = 0;
    stack.sp34.unk10 = 0;
    stack.sp34.unk18 = 0xB0;
    stack.sp34.unk19 = 0xA0;
    stack.sp34.unk1A = 0x2A;
    stack.sp34.unk14 = 0x40;
    stack.sp34.unk15 = 0xB;
    stack.sp34.unk16 = 0x6A;
    stack.sp34.unk17 = 0xFF;

    stack.sp34.unk1B = (func_150ADA20() % 0x9CU) + 0x64;
    stack.sp34.unk1C = 0xFF;
    stack.sp34.unk60 = 3;
    stack.sp34.unk61 = 3;

    stack.sp34.unk28 = stack.sp34.unk2C = (func_150ADA68() * D_800A05AC) + 800.0f;
    stack.sp34.unk30 = arg2;
    stack.sp34.unk34 = arg3;
    stack.sp34.unk38 = arg4;

    stack.temp = func_150ADA20();
    func_15143794(arg8, (s16)((stack.temp % 0xCU) - 0x15),
                  (func_150ADA68() * 300.0f + 498.0f) * D_800A05B0,
                  &stack.sp34.unk48);

    stack.sp34.unk54 = 0.0f;
    stack.sp34.unk58 = 0xE05;

    if (func_150ADA20() & 1) {
        stack.sp34.unk58 |= 0x40;
    }

    if (func_150ADA20() & 1) {
        stack.sp34.unk58 |= 0x80;
    }

    stack.sp34.unk62 = 9;
    stack.sp34.unk24 = D_800A05B4;
    stack.sp2C = D_800A05B8;
    stack.sp34.unk63 = -1;
    stack.sp34.unk1E = 0x19;
    stack.sp34.unk20 = 0xA;
    stack.sp34.unk22 = 0x20;

    ret = func_15130374(&stack.sp34, 1, 4, arg14, 1);
    if (ret != 0) {
        memcpy((u8 *)ret + 0xA8, &stack.sp2C, 4);
    }

    return 1;
}

typedef struct {
    u8 pad0[0x58];
    f32 unk58;
    u8 pad5C[4];
    f32 unk60;
    u8 pad64[0x44];
    f32 damping; /* 0xA8, written by func_150CAA04 */
} Struct150CAC28;

/* Applies the per-tick damping factor to unk58 and unk60 once per elapsed tick. */
s32 func_150CAC28(Struct150CAC28 *arg0, s32 arg1) {
    f32 *damping; // matching: reading arg0->damping directly changes the loop code
    s32 ticks;

    damping = &arg0->damping;
    for (ticks = D_800BE9E4; ticks != 0; ticks--) {
        arg0->unk58 *= *damping;
        arg0->unk60 *= *damping;
    }
    return 1;
}

s32 func_150CADD0(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s16 arg8, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, f32 *arg13, u8 arg14) {
    struct {
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
            u8 pad59[3];
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
            u8 pad6B;
            s32 unk6C;
            u8 unk70;
            u8 pad71;
            s16 unk72;
            s16 unk74;
            u8 pad76[0xA];
    } sp28;

    sp28.unk28 = arg2;
    sp28.unk2C = arg3;
    sp28.unk1C = 1.0f;
    sp28.unk20 = 1.0f;
    sp28.unk24 = 1.0f;
    sp28.unk50 = 0x29E8;
    sp28.unk10 = 0.0f;
    sp28.unk14 = 0.0f;
    sp28.unk18 = 0.0f;
    sp28.unk44 = 0.0f;
    sp28.unk00 = 1.0f;
    sp28.unk56 = 0x20;
    sp28.unk30 = arg4;
    sp28.unk04 = D_800A05BC;

    ((s32 *)&sp28)[-2] = func_150ADA20();
    func_15143794(arg8, (s16)((((s32 *)&sp28)[-2] % 21U) - 40),
                  (func_150ADA68() * 204.0f + 500.0f) * D_800A05C0,
                  &sp28.unk34);

    sp28.unk40 = (func_150ADA68() * 260.0f + -130.0f) * D_800A05C4;
    sp28.unk48 = (func_150ADA68() * 260.0f + -130.0f) * D_800A05C8;
    sp28.unk54 = (func_150ADA20() % 0x33U) + 0x32;
    sp28.unk4C = (func_150ADA68() * 1008.0f + D_800A05CC) * D_800A05D0;

    sp28.unk08 = sp28.unk0C = (func_150ADA68() * D_800A05D4 + 200.0f) * D_800A05D8;
    sp28.unk58 = 0;
    sp28.unk5C = 0;

    sp28.unk60 = (func_150ADA20() % 0x4CU) + 0xB4;
    sp28.unk61 = 4;
    sp28.unk62 = 0;
    sp28.unk63 = 0;
    sp28.unk64 = 0;
    sp28.unk65 = 0;
    sp28.unk66 = 0;
    sp28.unk67 = 0;
    sp28.unk68 = 0;
    sp28.unk6A = 2;
    sp28.unk6C = 0;
    sp28.unk70 = 0;
    sp28.unk72 = 0x20;
    sp28.unk74 = 7;

    func_15132A4C(&sp28, 3, 0xFF, 0, arg14, 0);
    return 1;
}

/* Same per-frame integration as func_150CA07C. */
s32 func_150CB008(BurstParticle *particle) {
    particle->position[0] += particle->velocity[0] * D_800BE9A4;
    particle->position[1] += (particle->velocity[1] * D_800BE9A4) + (particle->gravity * D_800BE9A4 * D_800BE9A4 * 0.5f);
    particle->position[2] += particle->velocity[2] * D_800BE9A4;
    particle->velocity[1] += particle->gravity * D_800BE9A4;
    particle->rotation[0] += particle->spin[0] * D_800BE9A4;
    particle->rotation[1] += particle->spin[1] * D_800BE9A4;
    particle->rotation[2] += particle->spin[2] * D_800BE9A4;
    return 1;
}
