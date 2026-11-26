#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_151DB5D0 (game_2062D0.c) is declared in each file that calls it: game/Effects/Blood/blood.c was compiled with a
// different view of it.
SteamParticle *func_151DB5D0(s32 kind, f32 *pos, f32 *vel, f32 size, f32 arg4, f32 drag, f32 gravity, s16 lifetime,
                             u8 arg8, s16 fadeFrames, s16 fadeStep, u8 spin, u8 argC, s32 argD);

/* Read by game_3F1F0.c. */
const u8 D_800A1C00[8][8] = {
    { 0xFD, 0xFB, 0xF9, 0xF7, 0xF5, 0xF3, 0xF1, 0xEF },
    { 0xFC, 0xFA, 0xF8, 0xF6, 0xF4, 0xF2, 0xF0, 0xEE },
    { 0xE6, 0xE5, 0xE4, 0xE3, 0xE2, 0xE1, 0xE0, 0xDF },
    { 0xDE, 0xDD, 0xDC, 0xDB, 0xDA, 0xD9, 0xD8, 0xD7 },
    { 0xD6, 0xD5, 0xD4, 0xD3, 0xD2, 0xD1, 0xD0, 0xCF },
    { 0xCE, 0xCD, 0xCC, 0xCB, 0xCA, 0xC9, 0xC8, 0xC7 },
    { 0xC6, 0xC5, 0xC4, 0xC3, 0xC2, 0xC1, 0xC0, 0xBF },
    { 0xBE, 0xBD, 0xBC, 0xBB, 0xBA, 0xB9, 0xB8, 0xB7 },
};
static const u8 D_800A1C40[4][2] = { { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 } };
static const Vec3f D_800A1C48 = { -124.0f, -8.0f, 28.0f };
static const Vec3f D_800A1C54 = { -170.0f, -8.0f, 28.0f };

/* Effects 50 units above `obj`: a light_createWithEffect burst and a func_15152B38 emitter. */
void func_150F85A0(s32 arg0, Actor *obj, s32 arg2) {
    Vec3f pos;

    pos.x = obj->xpos;
    pos.y = obj->ypos + 50.0f;
    pos.z = obj->zpos;
    {
        LightEffectHeader header;
        Vec3i header2;

        header.flags = 3;
        header.update = -1;
        header.lifetime = (func_150ADA20() % 9U) + 5;
        header.lightType = 0;
        header2.x = pos.x;
        header2.y = pos.y;
        header2.z = pos.z;
        light_createWithEffect(&header, &header2, (func_150ADA20() % 31U) + 90, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 1);
    }
    {
        TrailBurst spawn;

        spawn.countBase = 8;
        spawn.countRange = 12;
        spawn.pos = pos;
        spawn.sizeBase = 3.07f;
        spawn.sizeRange = 6.2f;
        spawn.gravityBase = -0.9690000415f;
        spawn.gravityRange = 0.7180000544f;
        spawn.speedBase = 15.0f;
        spawn.speedRange = 30.0f;
        spawn.angles.yawBase = 0;
        spawn.angles.yawRange = 0xFF;
        spawn.angles.pitchBase = -64;
        spawn.angles.pitchRange = 80;
        spawn.capacityBase = 3;
        spawn.capacityRange = 1;
        spawn.lifeBase = 20;
        spawn.lifeRange = 15;
        spawn.headerFlags = 1;
        spawn.flags = 4;
        spawn.primMode = 2;
        spawn.envMode = 3;
        spawn.colorBase.red = 0xFF;
        spawn.colorBase.green = 0xFF;
        spawn.colorBase.blue = 0xFF;
        spawn.colorBase.alpha = 0xFF;
        spawn.colorRange.red = 0;
        spawn.colorRange.green = 0;
        spawn.colorRange.blue = 0;
        spawn.colorRange.alpha = 0;
        spawn.color2Base.red = 0xFF;
        spawn.color2Base.green = 0xFF;
        spawn.color2Base.blue = 0xFF;
        spawn.color2Base.alpha = 0xFF;
        spawn.color2Range.red = 0;
        spawn.color2Range.green = 0;
        spawn.color2Range.blue = 0;
        spawn.color2Range.alpha = 0;
        spawn.colorLevelBase = 0xFF;
        spawn.colorLevelRange = 0;
        spawn.renderMode = 3;
        spawn.unk21 = 0x24;
        spawn.setGeometryMode = 0x200005;
        spawn.clearGeometryMode = 0x60600;
        spawn.fadeStart = 8;
        spawn.fadeRate = 0x1F;
        spawn.shrinkStart = 1;
        spawn.shrinkPaused = 0;
        spawn.shrinkFactor = 1.0f;
        spawn.unk38 = -1;
        spawn.unk39 = 0;
        spawn.unk72 = -1;
        spawn.unk73 = -1;
        func_15152B38(&spawn, 0xFF, 1);
    }
}

/* A message passed to func_151494E0 and received by func_150F887C. */
struct StageMsg {
    /* 0x00 */ TriggerZone *zone; /* 0x3F: the StageState.zone to run */
    /* 0x04 */ u8 set;            /* 0x42: the StageState.set to finish */
};

/* Zone handler: broadcasts message 0x3F, which runs func_150F892C on the StageObj whose stage.zone is `zone`. */
void func_150F884C(Actor *obj, TriggerZone *zone) {
    StageMsg msg;

    msg.zone = zone;
    func_151494E0((s32)&msg, 0x3F);
}

struct StageObj {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ StageState stage;
};

/* Message handler: 0x42 jumps to the last stage, 0x3F runs func_150F892C. */
void func_150F887C(StageObj *obj, StageMsg *msg, u8 type) {
    StageState *state = &obj->stage;

    if (type == 0x42) {
        if (state->set == msg->set) {
            u8 i;

            for (i = 0; i < 7; i++) {
                state->parts[i]->inactive = 1;
            }
            state->parts[7]->inactive = 0;
            state->stage = 7;
        }
    } else if (type == 0x3F) {
        if (obj->stage.zone == msg->zone) {
            func_150F892C(obj);
        }
    }
}

/*
 * While any object 0x98 in state 0x1EB is active: steps through the parts on a timer (with a sound and a
 * rumble per step), and emits effects on an arc of radius `radius` around (centerX, centerZ).
 */
void func_150F892C(StageObj *obj) {
    s32 count;
    s32 i;
    StageState *state;

    count = 0;
    {
        Actor *other;

        other = movement_obj;
        for (i = 0; i < 25; i++, other++) {
            if (other->character_type == 0x98 && other->animseq.uh == 0x1EB && other->unk222 == 0) {
                count++;
            }
        }
    }
    state = &obj->stage;
    if (count == 0) {
        return;
    }
    if (state->stage < 7) {
        state->stageTimer -= D_800BE9E4 * count;
        if (state->stageTimer < 0) {
            RumbleParams event;
            func_10010F88(func_150ADA20() % 3U + 0x2B3, 24000, 0, 0, -1, state->parts[state->stage]->x,
                          state->parts[state->stage]->y, state->parts[state->stage]->z, 500, 1000);
            state->parts[state->stage]->inactive = 1;
            state->stage++;
            state->parts[state->stage]->inactive = 0;
            func_15164F0C(7, 0, 0, obj->unkC, obj->unk1);
            event.flags = 1;
            event.duration = 25;
            event.playerMask = 1;
            event.strength = 8;
            event.updateKind = -1;
            func_151D8868(&event, 0, obj->unkC, obj->unk1);
            state->stageTimer = func_150ADA20() % 31U + 70;
        }
    }
    state->eventTimer -= D_800BE9E4 * count;
    if (state->eventTimer < 0) {
        RumbleParams event;
        event.flags = 1;
        event.duration = func_150ADA20() % 21U + 10;
        event.playerMask = 1;
        event.strength = (func_150ADA20() & 3) + 1;
        event.updateKind = -1;
        func_151D8868(&event, 0, obj->unkC, obj->unk1);
        func_10010F88((func_150ADA20() & 3) + 0x6D6, 24000, 0, 0, -1, state->parts[state->stage]->x,
                      state->parts[state->stage]->y, state->parts[state->stage]->z, 500, 1000);
        state->eventTimer = func_150ADA20() % 151U + 150;
    }

    state->unk38 += (0.02000000142f + func_150ADA68() * 0.025f) * (gFrameDelta * count);
    if (state->unk38 > 1.0f) {
        s16 side;
        Vec3f pos;
        side = (state->mirror & 1) ? -1 : 1;
        do {
            func_15143874((func_150ADA20() % 126U - 220) * side, state->radius, &pos.x, &pos.z);
            pos.y = func_150ADA68() * 150.0f + 30.0f;
            pos.x += state->centerX;
            pos.z += state->centerZ;
            weapon_spawnImpactBurst(&pos, obj->unkC, obj->unk1);
            state->unk38 -= 1.0f;
        } while (state->unk38 > 1.0f);
    }

    state->unk34 += (0.02000000142f + func_150ADA68() * 0.06000000238f) * (gFrameDelta * count);
    if (state->unk34 > 1.0f) {
        s16 pieces;
        s16 side;
        ParticleSpawn spawn;
        side = (state->mirror & 1) ? -1 : 1;
        spawn.radius = 1.0f;
        spawn.elasticity = 1.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.flags = 0x39E8;
        spawn.model = 0x86;
        spawn.variant = 0;
        spawn.unk5C = 0;
        spawn.alpha = 0xFF;
        spawn.updateFn = 8;
        spawn.update2Fn = 0;
        spawn.floorHitFn = 0;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 0;
        spawn.unk6A = 2;
        spawn.owner = NULL;
        spawn.ownerId = 0;
        spawn.fadeStart = 12;
        spawn.fadeRate = 21;
        spawn.sound = 0;
        spawn.rotVel.y = 0.0f;
        do {
            s16 angle;

            pieces = func_150ADA20() % 7U + 5;
            angle = (func_150ADA20() % 126U - 220) * side;
            func_15143874(angle, state->radius, &spawn.pos.x, &spawn.pos.z);
            spawn.pos.y = func_150ADA68() * 150.0f + 30.0f;
            spawn.pos.x += state->centerX;
            spawn.pos.z += state->centerZ;
            i = angle - 7; // matching: the base angle reuses the counter i (a new local changes registers)
            for (; pieces > 0; pieces--) {
                spawn.lifetime = (func_150ADA20() & 0xF) + 35;
                spawn.sizeXZ = spawn.sizeY = func_150ADA68() * 0.012f + 0.005f;
                spawn.rot.x = func_150ADA68() * 360.0f;
                spawn.rot.y = func_150ADA68() * 360.0f;
                spawn.rot.z = func_150ADA68() * 360.0f;
                func_15143794((func_150ADA20() & 0xF) + i, func_150ADA20() % 37U - 63,
                              func_150ADA68() * 8.0f + 10.0f, &spawn.vel);
                spawn.rotVel.x = func_150ADA68() * 32.41f + -16.205f;
                spawn.rotVel.z = func_150ADA68() * 32.41f + -16.205f;
                spawn.gravity = func_150ADA68() * 0.337f + -1.177f;
                func_1513264C(&spawn, 3, 1, 0, 0, 0xFF, 1);
            }
            state->unk34 -= 1.0f;
        } while (state->unk34 > 1.0f);
    }

    state->unk30 += (0.05500000343f + func_150ADA68() * 0.05f) * (gFrameDelta * count);
    if (state->unk30 > 1.0f) {
        s16 side;
        s32 flares;
        ParticleDesc flare;
        side = (state->mirror & 1) ? -1 : 1;
        flare.texture = 0x86;
        flare.renderMode = 0x4404;
        flare.unk0 = 0x200005;
        flare.unk4 = 0x9F0600;
        flare.frame = 0;
        flare.frameRate = 0;
        flare.r = 0xFF;
        flare.g = 0xFF;
        flare.b = 0xFF;
        flare.a = 0xFF;
        flare.r2 = 0xFF;
        flare.g2 = 0xFF;
        flare.b2 = 0xFF;
        flare.offset = D_800A5480;
        flare.fadeStart = 8;
        flare.fadeRate = 0x1F;
        flare.scaleStart = 1;
        flare.flags = 0x1C207;
        flare.primColorMode = 6;
        flare.envColorMode = 5;
        flare.updateFunc = -1;
        flare.deathFunc = -1;
        flare.drawFunc = -1;
        flare.handlerSet = 0;
        flare.viewMask = 0;
        flare.views = 0xFF;
        flare.minSize = 0;
        flare.scaleRate = 1.0f;
        flare.maxSize = 0.0f;
        do {
            s16 angle;

            flares = func_150ADA20() % 13U + 7;
            angle = (func_150ADA20() % 126U - 220) * side;
            func_15143874(angle, state->radius, &flare.pos.x, &flare.pos.z);
            flare.pos.y = func_150ADA68() * 150.0f + 30.0f;
            flare.pos.x += state->centerX;
            flare.pos.z += state->centerZ;
            i = angle - 4; // matching: the base angle reuses the counter i (a new local changes registers)
            for (; flares > 0; flares--) {
                s32 flip;

                flare.timer = func_150ADA20() % 21U + 30;
                flare.alpha = func_150ADA20() % 156U + 100;
                flare.width = flare.height = func_150ADA68() * 110.0f + 70.0f;
                flare.flags &= ~0xC0;
                flip = (func_150ADA20() & 1) ? 0x80 : 0;
                flare.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | flip;
                func_15143794(func_150ADA20() % 9U + i, (func_150ADA20() & 7) - 16,
                              func_150ADA68() * 10.0f + 6.0f, &flare.vel);
                flare.gravity = func_150ADA68() * 0.268f + -0.3870000243f;
                func_15130280(&flare, 1, 0, 0, 0xFF, 1);
            }
            state->unk30 -= 1.0f;
        } while (state->unk30 > 1.0f);
    }

    state->unk2C += (0.3360000253f + func_150ADA68() * 0.126f) * (gFrameDelta * count);
    if (state->unk2C > 1.0f) {
        s16 side;
        Vec3f pos;
        Vec3f vel;
        side = (state->mirror & 1) ? -1 : 1;
        do {
            s16 angle;

            angle = (func_150ADA20() % 126U - 220) * side;
            func_15143874(angle, state->radius, &pos.x, &pos.z);
            pos.y = func_150ADA68() * 150.0f + 30.0f;
            pos.x += state->centerX;
            pos.z += state->centerZ;
            func_15143794(func_150ADA20() % 9U + angle - 4, (func_150ADA20() & 7) - 16,
                          func_150ADA68() * 6.0f + 4.0f, &vel);
            func_151DB5D0(8, &pos.x, &vel.x, func_150ADA68() * 200.0f + 200.0f, 1.025439024f, 0.938816011f,
                          func_150ADA68() * 0.2070000172f + -0.1690000147f, func_150ADA20() % 21U + 35,
                          func_150ADA20() % 121U + 100, 50, 5, 0, 0xFF, 1);
            state->unk2C -= 1.0f;
        } while (state->unk2C > 1.0f);
    }
}


/* Broadcasts message 0x42 (jump to the last stage) for both StageState ids of pair `index`. */
void func_150F9720(u8 index) {
    StageMsg msg;

    msg.zone = NULL;
    msg.set = D_800A1C40[index][0];
    func_151494E0((s32)&msg, 0x42);
    msg.set = D_800A1C40[index][1];
    func_151494E0((s32)&msg, 0x42);
}

void func_150F9788(TimedEffect *arg0) {
}

void func_150F9794(TimedEffect *arg0) {
    func_150F9788(arg0);
    func_1514933C(arg0);
}

void func_150F97C0(TimedEffect *arg0) {
    func_150F9788(arg0);
    func_15149368(arg0);
}

static u8 D_80088B50 = 23; /* the bone of func_150F97EC's effect */

/* Effect at the point D_800A1C48 on bone D_80088B50 of `obj`. */
void func_150F97EC(obj, arg1, arg2)
Actor *obj;
u8 arg1;
s32 arg2;
{
    Vec3f pos;
    f32 one;
    ParticleSize motion;
    const Vec3f *src;
    Vec3f *dst;
    void *mtx;
    s32 random1;
    s32 random0;

    if (obj->ob_matrix_ptr == NULL) {
        return;
    }
    src = &D_800A1C48;
    dst = &pos;
    mtx = obj->ob_matrix_ptr[D_80088B50];
    func_15145EA4(&src, &dst, mtx, 1);
    one = 1.0f; // matching: IDO folds a literal "* 1.0f" away; the original multiplies by a variable
    motion.height = func_150ADA68() * one + 2.0f;
    motion.width = func_150ADA68() * 14.0f + 28.0f;
    random0 = func_150ADA20();
    random1 = func_150ADA20();
    func_15102B38(obj, D_80088B50, &D_800A1C48, &D_800A1C54, &motion, (random0 % 3U) + 4, (random1 % 156U) + 100,
                  func_150ADA68() * 300.0f + 400.0f, &pos, 0xFF, 0, -1, arg1, arg2);
}

/* Muzzle effects of `obj`'s gun attachment (set 0x82): flash at the muzzle, impact at the hit point. */
void func_150F9950(obj, arg1, arg2)
Actor *obj;
u8 arg1;
s32 arg2;
{
    Vec3f points[6];
    Vec3f position;
    Vec3f hit;
    Vec3f normal;
    Vec3f unk30;
    ModelAttachment *gun;

    gun = weapon_getGunAttachment(obj);
    if (gun != NULL) {
        if (weapon_getGunMuzzlePoints(points, &position, &hit, &normal, &unk30, obj, gun) != 0) {
            func_151D3F14(&position, arg1, arg2);
            func_151D4408(&hit, &normal, obj->ob_matrix_ptr[gun->bone], obj, 1.0f, arg1, arg2);
            weapon_spawnMuzzleFlashes(&position, points, arg1, arg2);
        }
    }
}
