#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x; /* sin of the segment angle */
    f32 z; /* cos of the segment angle */
} RadialDir;

struct RadialEffect {
    EffectHeader header;
    RadialEffectDef def; /* 0x10 */
    f32 size;            /* 0x40 */
    f32 radius;          /* 0x44 */
    f32 step;            /* 0x48: angle between segments */
    s16 duration;        /* 0x4C: initial def.remaining */
    f32 pending;         /* 0x50: emissions owed, see effect_updateRingEffect */
    Vtx *vtx[2];         /* 0x54: double-buffered by D_800BE9C0 */
    u8 pad5C[0x304];
    RadialDir *dirs;     /* 0x360 */
    u8 *unk364;
    u8 data[1];          /* 0x368 */
};

typedef struct {
    f32 r;
    f32 y;
} RingPoint;

typedef struct {
    s32 count;
    const RingPoint *pts;
} RingProfile;

/* This file's .rodata front-end data: the ring's cross-section profile. */
static const RingPoint gRingProfilePoints[5] = {
    { 0.0f, 0.0f }, { -1.0f, 3.0f }, { -3.0f, 5.0f }, { -8.0f, 5.0f }, { -18.0f, 0.0f },
};

/* This file's .data. */
static RingProfile gRingProfile = { sizeof(gRingProfilePoints) / sizeof(gRingProfilePoints[0]), gRingProfilePoints };
/* The display lists that open and close the ring effect's pass (in the effect type table at 0x8008BE98). */
Gfx gRingRenderSetupDl[] = {
    gsDPPipeSync(),
    gsSPMatrix(&D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx gRingRenderResetDl[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};
/* The callbacks that RadialEffectDef's emitCallback, checkCallback and updateCallback index. */
static void (*gRingEmitFuncs[])(RadialEffect *, Vec3f *, u8, f32, f32) = { effect_ringEmitDarkParticle, effect_ringEmitBurst };
static s32 (*gRingCheckFuncs[])(RadialEffect *) = { NULL };
static void (*gRingUpdateFuncs[])(RadialEffect *, f32) = { effect_ringHitActors };

/*
 * Allocates a ring effect from def. Behind the header: dirs[count], two vertex buffers of
 * (count + 1) * 5 Vtx each, then extraSize bytes at unk364.
 */
RadialEffect *effect_newRingEffect(RadialEffectDef *def, s32 extraSize, u8 arg2, s32 arg3) {
    RadialEffect *effect;
    s32 i;

    effect = effect_newObject(0x31, arg3, (s32)((Vtx (*)[2])((u8 *)extraSize + def->count * 8 + 0x408) + def->count * 5), 1, arg2, 1);
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
    i = 0; // matching: i is zeroed before the angle scope opens
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

/* Per-tick update of an expanding ring: ages it, grows its radius and emits along its edge. */
void effect_updateRingEffect(RadialEffect *effect) {
    s32 pad; // matching: unused stack slot
    u8 expired;
    f32 oldRadius;
    f32 cosine;
    f32 sine;
    s32 bias;
    f32 emitPos[3]; // matching: a Vec3f local changes the code, so the callbacks get it through a cast
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
        effect->radius += effect->def.speed * gFrameDelta;
        // matching: remaining must be loaded inside the condition, after growUntil
        if (effect->def.growUntil < (remaining = effect->def.remaining)) {
            age = effect->duration - remaining;
            effect->size = age * effect->def.growRate;
        }
        if (remaining < effect->def.shrinkFrom) {
            effect->size = remaining * effect->def.shrinkRate;
        }

        if (effect->def.emitCallback != -1) {
            effect->pending += effect->def.emitRate * gFrameDelta * (6.283185482f * effect->radius);
            while (effect->pending > 1.0f) {
                bias = camera_global_Cameras[gCurrentView].TrueCameraAngle * 0.7111111283f;
                angle = (func_150ADA20() % 81U) + bias - 40;
                cosine = func_151423D8(angle - 64);
                sine = func_151423D8(angle);
                emitPos[0] = effect->def.pos.x + cosine * effect->radius;
                emitPos[1] = effect->def.pos.y;
                emitPos[2] = effect->def.pos.z + sine * effect->radius;
                if (emitPos[2] * emitPos[2] + emitPos[0] * emitPos[0] < 10894295.0f) {
                    gRingEmitFuncs[effect->def.emitCallback](effect, (Vec3f *)emitPos, angle, cosine, sine);
                }
                effect->pending -= 1.0f;
            }
        }
        if (effect->def.updateCallback != -1) {
            gRingUpdateFuncs[effect->def.updateCallback](effect, oldRadius);
        }
        if (effect->def.checkCallback != -1 && gRingCheckFuncs[effect->def.checkCallback](effect) == 0) {
            expired = 1;
        }
    }
    if (expired) {

        effect_destroyObject(&effect->header);
    }
}

extern struct { s32 unk0; s32 unk4; } D_800A4AC8[];

/*
 * Draws a ring effect as a strip of quads: for every segment direction, the ring profile
 * gRingProfile is swept out at the current radius into this frame's vertex buffer.
 */
Gfx *effect_drawRingEffect(Gfx *gdl, RadialEffect *effect, s32 arg2) {
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
    s32 texAddr = texture_getAddress(*(s32 *)gEffectTextures[67].addr, &palOffset, 3, 0);


    gSPTexture(gdl++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetTextureImage(gdl++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, texAddr);
    gDPSetTile(gdl++, G_IM_FMT_CI, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadBlock(gdl++, G_TX_LOADTILE, 0, 0, 0x3EF, 0);
    gDPSetTile(gdl++, G_IM_FMT_CI, G_IM_SIZ_4b, 2, 0, 1, 0, G_TX_WRAP, 6, G_TX_NOLOD, G_TX_WRAP, 5, G_TX_NOLOD);
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
                for (pt = 0; pt < gRingProfile.count; pt++) {
                    f32 dist;
                    f32 y;
                    f32 pad; // matching: unused local keeps the frame layout
                    f32 x;
                    f32 z;

                    dist = (gRingProfile.pts + pt)->r * effect->size;
                    y = (gRingProfile.pts + pt)->y * effect->size;
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
                for (; pt < gRingProfile.count; pt++) {
                    f32 dist;
                    f32 y;
                    f32 pad; // matching: unused local keeps the frame layout
                    f32 x;
                    f32 z;

                    dist = (gRingProfile.pts + pt)->r * effect->size;
                    y = (gRingProfile.pts + pt)->y * effect->size;
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
                stripLen = gRingProfile.count * 2;
                gSPVertex(gdl++, &effect->vtx[D_800BE9C0][vtxCount - stripLen], stripLen, 0);
                for (pt = 0; pt < gRingProfile.count - 1; pt++) {
                    gSP1Triangle(gdl++, pt, pt + 1, pt + gRingProfile.count + 1, 0);
                    gSP1Triangle(gdl++, pt, pt + gRingProfile.count + 1, pt + gRingProfile.count, 0);
                }
            }
        }
    }
    }
    return gdl;
}

/* Object spawned by func_15130374 from a ParticleDesc packet. */
struct ParticleObject {
    u8 pad0[0x10];
    ParticleDesc p;   /* 0x10 */
    u8 pad80[0x28];
    f32 damping;  /* 0xA8: velocity factor per tick, written by func_150CAA04 */
};

/* Ring emit callback 0: spawns a dark particle at pos, moving along heading. */
void effect_ringEmitDarkParticle(RadialEffect *effect, Vec3f *pos, u8 heading, f32 cosine, f32 sine) {
    ParticleDesc packet;
    s32 pad; // matching: unused stack slot
    s16 pitch;

    f32 speed;
    s32 flag40;
    s32 flag80;

    packet.texture = 0x26;
    packet.renderMode = 0xC01;
    packet.unk0 = 0x200005;
    packet.unk4 = 0;
    packet.timer = (func_150ADA20() % 81U) + 25;
    packet.frame = 0;
    packet.frameRate = 0;
    packet.primColorMode = 5;
    packet.envColorMode = 5;
    packet.r = 0;
    packet.g = 0;
    packet.b = 0;
    packet.a = 0xFF;
    packet.r2 = 0;
    packet.g2 = 0;
    packet.b2 = 0;
    packet.alpha = (func_150ADA20() % 156U) + 100;
    packet.brightness = 0xFF;
    packet.width = packet.height = (func_150ADA68() * 500.0f) + 500.0f;
    packet.pos = *pos;
    packet.offset.x = 0.0f;
    packet.offset.y = 0.0f;
    packet.offset.z = 0.0f;
    packet.gravity = ((func_150ADA68() * 550.0f) + -1416.0f) * 0.001000000047f;
    packet.fadeStart = 20;
    packet.fadeRate = 12;
    packet.scaleStart = 0;
    packet.scaleRate = 1.0f;
    flag40 = (func_150ADA20() & 1) ? 0x40 : 0;
    flag80 = (func_150ADA20() & 1) ? 0x80 : 0;
    packet.flags = 7 | flag80 | flag40 | 0x200;
    packet.updateFunc = -1;
    packet.deathFunc = -1;
    pitch = (func_150ADA20() % 11U) - 18;
    speed = func_150ADA68() * (effect->def.speed * 0.2430000156f) + effect->def.speed * 0.9610000253f;

    func_15143794(heading, pitch, speed, &packet.vel);
    func_15130374(&packet, 1, 0, effect->header.owner, effect->header.kind);
}

/* Ring update callback: hits actors that lie up to 250 units inside the ring edge. */
void effect_ringHitActors(RadialEffect *effect, f32 oldRadius) {
    s32 i;
    Actor *actor;
    f32 dx;
    f32 dz;
    f32 dist;
    f32 behind;

    for (i = 0; i < 25; i++) {
        actor = &movement_obj[i];
        if ((actor->type != 0) && (actor->shad < 20.0f) && (actor->imune == 0) &&
            ((actor->character_type == 0x53) || (actor->type == 1))) {
            dx = actor->xpos - effect->def.pos.x;
            dz = actor->zpos - effect->def.pos.z;
            dist = sqrtf((dx * dx) + (dz * dz)) - 100.0f;
            if ((actor->character_type == 0x53) && (actor->lifting == 0)) {
                dist -= 150.0f;
            }
            behind = effect->radius - dist;
            if ((behind >= 0.0f) && (behind < 250.0f)) {
                if (actor->character_type == 0x53) {
                    actor->norun = 10;
                    actor->scriptCmd = NULL;
                    actor->intelwalk = 0x13;
                    if (movement_obj[actor->owner].attach_char != 0) {
                        actor->intelwalk = 0x14;
                    }
                } else {
                    actor_hitScripted(actor, 0x6000E, actor->langle, -1);
                }
            }
        }
    }
}

/* Object spawned by func_15132A4C from a ParticleSpawn. */
struct BurstParticle {
    u8 pad0[0x10];
    ParticleSpawn p; /* 0x10 */
};

/* Ring emit callback 1: spawns a tumbling burst object at pos (the colour from func_150CCD90 goes unused). */
void effect_ringEmitBurst(RadialEffect *effect, Vec3f *pos, u8 heading, f32 cosine, f32 sine) {
    ParticleSpawn packet;
    f32 r;
    f32 g;
    f32 b;

    func_150CCD90(pos->z, &r, &g, &b);
    {
        typedef s32 T; // matching: a scope with a declaration orders the struct copy
        packet.pos = *pos;
    }
    packet.scale.x = 1.0f;
    packet.scale.y = 1.0f;
    packet.scale.z = 1.0f;
    packet.flags = 0x29E8;
    packet.rot.x = func_150ADA68() * 360.0f;
    packet.rot.y = func_150ADA68() * 360.0f;
    packet.rot.z = func_150ADA68() * 360.0f;
    packet.model = 0x20;
    packet.elasticity = 0.200000003f;
    packet.rotVel.y = 0.0f;
    packet.radius = 1.0f;
    // matching: the original func_150ADA20 returned a full word; its spill is the
    // call-argument slot at sp+0x20, 0x14 below packet
    ((s32 *)&packet)[-5] = func_150ADA20();
    func_15143794(heading, (((s32 *)&packet)[-5] % 26U) - 45,
                  (func_150ADA68() * 81.0f + 60.0f) * effect->def.speed * 0.009999999776f,
                  &packet.vel);
    packet.rotVel.x = (func_150ADA68() * 260.0f + -130.0f) * 0.1000000015f;
    packet.rotVel.z = (func_150ADA68() * 260.0f + -130.0f) * 0.1000000015f;
    packet.lifetime = (func_150ADA20() % 41U) + 40;
    packet.gravity = (func_150ADA68() * 1533.0f + -2500.0f) * 0.001000000047f;
    packet.sizeXZ = packet.sizeY = (func_150ADA68() * 400.0f + 199.0f) * 0.001000000047f;
    packet.variant = 0;
    packet.unk5C = 0;
    packet.alpha = (func_150ADA20() % 101U) + 155;
    packet.updateFn = 7;
    packet.update2Fn = 0;
    packet.floorHitFn = 0;
    packet.floorType3Fn = 0;
    packet.wallHitFn = 0;
    packet.destroyFn = 0;
    packet.squashFn = 0;
    packet.matrixFn = 0;
    packet.unk6A = 2;
    packet.owner = 0;
    packet.ownerId = 0;
    packet.fadeStart = 0x20;
    packet.fadeRate = 7;
    func_15132A4C(&packet, 3, 0xFF, 0, effect->header.owner, effect->header.kind);
}


/* Integrates a burst particle's position (with gravity) and rotation over one frame. */
s32 effect_moveBurstParticle(BurstParticle *particle) {
    particle->p.pos.x += particle->p.vel.x * gFrameDelta;
    particle->p.pos.y += (particle->p.vel.y * gFrameDelta) + (particle->p.gravity * gFrameDelta * gFrameDelta * 0.5f);
    particle->p.pos.z += particle->p.vel.z * gFrameDelta;
    particle->p.vel.y += particle->p.gravity * gFrameDelta;
    particle->p.rot.x += particle->p.rotVel.x * gFrameDelta;
    particle->p.rot.y += particle->p.rotVel.y * gFrameDelta;
    particle->p.rot.z += particle->p.rotVel.z * gFrameDelta;
    return 1;
}

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

/*
 * Impact effect at the actor: finds the floor below it, spawns an expanding ring there and,
 * when the floor was found, four func_1513C5B0 quads around the impact point plus particles.
 */
void effect_spawnImpactRing(Actor *actor) {
    Vec3f pos;
    f32 r, g, b;
    u8 red, green, blue;
    CollisionHit floorInfo;
    u8 onGround;
    u32 end;
    u32 addr;
    s32 sum;
    u8 padA[4]; // matching: frame padding

    onGround = 0;
    if (actor != NULL) {
        collision_initHitFromObject(&floorInfo, actor);
        func_1512D748(&camera_global_Cameras[D_800BE9E8], 7, 1);
        pos.x = actor->xpos;
        pos.y = actor->ypos + 100.0f;
        pos.z = actor->zpos;
        if (collision_probeFloorSurfABSet0Cached(&pos, 0, actor->ypos - 300.0f, &floorInfo) != 0) {
            onGround = 1;
            pos.y = floorInfo.height;
        } else {
            pos.y = actor->floorY;
        }
        /* Anti-tamper check: checksum the code from func_15193CA0 to func_151942B0. */
        sum = 0;
        end = (u32)func_151942B0;
        for (addr = (u32)func_15193CA0; addr < end; addr += 4) {
            sum ^= ~*(s32 *)addr;
        }
        if (sum != 0x79844C6B) {
            gBonePoolSize = 0;
        }
        func_150CCD90(pos.z, &r, &g, &b);
        red = r;
        green = g;
        blue = b;
        func_151D5404(&pos, 500.0f, 5000.0f, 0.0002f, 0xC, 0xF, 0xFF, 0);
        {
            RadialEffectDef def;
            u8 padB[0x14]; // matching: frame padding

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
            effect_newRingEffect(&def, 0, 0xFF, 1);
        }
        if (onGround) {
            Fx_150CA150 fx;
            f32 pad; // matching: unused stack slot
            f32 size;
            f32 extent;
            u8 heading;
            f32 sinH, cosH;
            u8 padC[8]; // matching: frame padding
            s16 colors[4][5];
            s16 randomArg;
            u8 spark[3];

            heading = -0x40 - (actor->langle >> 8);
            cosH = func_151423D8(heading);
            sinH = func_151423D8(heading - 0x40);
            size = extent = (func_150ADA68() * 96.0f + 352.0f) * 0.5f;
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
            colors[0][3] = 0xFF;
            colors[0][4] = 0;
            colors[1][3] = 0xFF;
            colors[1][4] = 0;
            colors[2][3] = 0xFF;
            colors[2][4] = 0;
            colors[3][3] = 0xFF;
            colors[3][4] = 0;
            fx.unk6 = 0x51;
            colors[0][0] = red;
            colors[1][0] = red;
            colors[2][0] = red;
            colors[3][0] = red;
            colors[0][1] = green;
            colors[1][1] = green;
            colors[2][1] = green;
            colors[3][1] = green;
            colors[0][2] = blue;
            colors[1][2] = blue;
            colors[2][2] = blue;
            colors[3][2] = blue;
            // matching: the corner offsets are written as (+/-1.0f * extent)
            func_1513C5B0(&fx, colors, 0xC, 0, extent * cosH + pos.x - extent * sinH, pos.y + 10.0f, extent * cosH + pos.z + extent * sinH, size, size, heading, 0, 0, 0xFF, 0);
            fx.unk6 = 0x52;
            func_1513C5B0(&fx, colors, 0xC, 0, extent * cosH + pos.x - (-1.0f * extent) * sinH, pos.y + 10.0f, (-1.0f * extent) * cosH + pos.z + extent * sinH, size, size, heading, 0, 0, 0xFF, 0);
            fx.unk6 = 0x53;
            func_1513C5B0(&fx, colors, 0xC, 0, (-1.0f * extent) * cosH + pos.x - extent * sinH, pos.y + 10.0f, extent * cosH + pos.z + (-1.0f * extent) * sinH, size, size, heading, 0, 0, 0xFF, 0);
            fx.unk6 = 0x54;
            func_1513C5B0(&fx, colors, 0xC, 0, (-1.0f * extent) * cosH + pos.x - (-1.0f * extent) * sinH, pos.y + 10.0f, (-1.0f * extent) * cosH + pos.z + (-1.0f * extent) * sinH, size, size, heading, 0, 0, 0xFF, 0);
            func_150CA930(&pos);
            randomArg = func_150ADA20() % 21U + 10;
            spark[0] = red;
            spark[1] = green;
            spark[2] = blue;
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

/* Fades the object out over its last 32 ticks by capping alpha at timer * 8. */
s32 func_150CA9D0(QuadSprite *arg0) {
    s32 maxAlpha;

    if (arg0->def.lifetime < 32) {
        maxAlpha = arg0->def.lifetime * 8;
        if (maxAlpha < arg0->def.unk10) {
            arg0->def.unk10 = maxAlpha;
        }
    }
    return 1;
}

/* Spawns a particle at (x, y, z) moving along heading; func_150CAC28 damps its velocity. */
s32 func_150CAA04(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s16 heading, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 arg14) {
    ParticleObject *obj;
    ParticleDesc packet;
    s32 pad1; // matching: unused stack slot
    f32 damping;
    s32 pad2; // matching: unused stack slot
    u32 random;

    packet.texture = 0x29;
    packet.renderMode = 0xE03;
    packet.unk0 = 0x200005;
    packet.unk4 = 0;
    packet.timer = (func_150ADA20() % 26U) + 25;
    packet.frame = 0;
    packet.frameRate = 0;
    packet.r2 = 0xB0;
    packet.g2 = 0xA0;
    packet.b2 = 0x2A;
    packet.r = 0x40;
    packet.g = 0x0B;
    packet.b = 0x6A;
    packet.a = 0xFF;
    packet.alpha = (func_150ADA20() % 156U) + 100;
    packet.brightness = 0xFF;
    packet.primColorMode = 3;
    packet.envColorMode = 3;

    packet.width = packet.height = (func_150ADA68() * 401.0f) + 800.0f;
    packet.pos.x = x;
    packet.pos.y = y;
    packet.pos.z = z;

    random = func_150ADA20();
    func_15143794(heading, (random % 12U) - 21,
                  (func_150ADA68() * 300.0f + 498.0f) * 0.1000000015f,
                  &packet.vel);

    packet.gravity = 0.0f;
    packet.flags = 0xE05;
    if (func_150ADA20() & 1) {
        packet.flags |= 0x40;
    }
    if (func_150ADA20() & 1) {
        packet.flags |= 0x80;
    }

    packet.updateFunc = 9;
    packet.scaleRate = 1.039999962f;
    damping = 0.9510000348f;
    packet.deathFunc = -1;
    packet.fadeStart = 25;
    packet.fadeRate = 10;
    packet.scaleStart = 0x20;

    obj = func_15130374(&packet, 1, 4, arg14, 1);
    if (obj != NULL) {
        memcpy(&obj->damping, &damping, sizeof(f32));
    }

    return 1;
}

/* Damps a particle's horizontal velocity once per elapsed tick. */
s32 func_150CAC28(ParticleObject *obj, s32 arg1) {
    f32 *damping; // matching: reading obj->damping directly changes the loop code
    s32 ticks;

    damping = &obj->damping;
    for (ticks = D_800BE9E4; ticks != 0; ticks--) {
        obj->p.vel.x *= *damping;
        obj->p.vel.z *= *damping;
    }
    return 1;
}

/* Spawns a tumbling burst object at (x, y, z) moving along heading. */
s32 func_150CADD0(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s16 heading, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, f32 *arg13, u8 arg14) {
    s32 pad; // matching: unused stack slot
    ParticleSpawn packet;

    packet.pos.x = x;
    packet.pos.y = y;
    packet.scale.x = 1.0f;
    packet.scale.y = 1.0f;
    packet.scale.z = 1.0f;
    packet.flags = 0x29E8;
    packet.rot.x = 0.0f;
    packet.rot.y = 0.0f;
    packet.rot.z = 0.0f;
    packet.rotVel.y = 0.0f;
    packet.radius = 1.0f;
    packet.model = 0x20;
    packet.pos.z = z;
    packet.elasticity = 0.200000003f;

    // matching: the original func_150ADA20 returned a full word; its spill is the
    // call-argument slot at sp+0x20, 8 below packet
    ((s32 *)&packet)[-2] = func_150ADA20();
    func_15143794(heading, (((s32 *)&packet)[-2] % 21U) - 40,
                  (func_150ADA68() * 204.0f + 500.0f) * 0.1000000015f,
                  &packet.vel);

    packet.rotVel.x = (func_150ADA68() * 260.0f + -130.0f) * 0.1000000015f;
    packet.rotVel.z = (func_150ADA68() * 260.0f + -130.0f) * 0.1000000015f;
    packet.lifetime = (func_150ADA20() % 51U) + 50;
    packet.gravity = (func_150ADA68() * 1008.0f + -3085.0f) * 0.001000000047f;

    packet.sizeXZ = packet.sizeY = (func_150ADA68() * 601.0f + 200.0f) * 0.001000000047f;
    packet.variant = 0;
    packet.unk5C = 0;

    packet.alpha = (func_150ADA20() % 76U) + 180;
    packet.updateFn = 4;
    packet.update2Fn = 0;
    packet.floorHitFn = 0;
    packet.floorType3Fn = 0;
    packet.wallHitFn = 0;
    packet.destroyFn = 0;
    packet.squashFn = 0;
    packet.matrixFn = 0;
    packet.unk6A = 2;
    packet.owner = 0;
    packet.ownerId = 0;
    packet.fadeStart = 0x20;
    packet.fadeRate = 7;

    func_15132A4C(&packet, 3, 0xFF, 0, arg14, 0);
    return 1;
}

/* Same per-frame integration as effect_moveBurstParticle. */
s32 func_150CB008(BurstParticle *particle) {
    particle->p.pos.x += particle->p.vel.x * gFrameDelta;
    particle->p.pos.y += (particle->p.vel.y * gFrameDelta) + (particle->p.gravity * gFrameDelta * gFrameDelta * 0.5f);
    particle->p.pos.z += particle->p.vel.z * gFrameDelta;
    particle->p.vel.y += particle->p.gravity * gFrameDelta;
    particle->p.rot.x += particle->p.rotVel.x * gFrameDelta;
    particle->p.rot.y += particle->p.rotVel.y * gFrameDelta;
    particle->p.rot.z += particle->p.rotVel.z * gFrameDelta;
    return 1;
}
