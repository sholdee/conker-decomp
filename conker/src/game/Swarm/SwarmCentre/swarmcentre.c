#include <ultra64.h>
/* functions.h declares these with signatures that don't match their real ones; rename them out of the way. */
#include "functions.h"
#include "variables.h"


/* Object type 0x3A: a swarm centre that steers towards a target and owns a group of members. */
struct Swarm {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ union {
        f32 *pos;                       /* when targetZ.z == -0x8000 */
        struct { s16 x; s16 y; } fixed; /* otherwise, with targetZ.z */
        s32 packed;
    } target;
    /* 0x14 */ union {
        s16 z;
        s32 packed;
    } targetZ;
    /* 0x18 */ s32 x;
    /* 0x1C */ s32 y;
    /* 0x20 */ s32 z;
    /* 0x24 */ u8 pad24[0x4];
    /* 0x28 */ s16 prevX;
    /* 0x2A */ s16 prevY;
    /* 0x2C */ s16 prevZ;
    /* 0x2E */ u16 sfxHandle;
    /* 0x30 */ s16 yOffset;
    /* 0x32 */ s16 speed;
    /* 0x34 */ u8 tag;
    /* 0x35 */ u8 memberCount;
    /* 0x36 */ u8 viewMask; /* bit i set when visible in view i */
    /* 0x37 */ u8 playSfx;
};

/* Per-member state; a template copy of it initialises each new member. */
typedef struct SwarmMemberState {
    /* 0x00 */ TextureDesc *anim; /* an entry of gEffectAnimTextures */
    /* 0x04 */ Swarm *swarm;
    /* 0x08 */ s32 steer; /* (heading << 4) | timer */
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 velX;
    /* 0x18 */ s16 velZ;
    /* 0x1A */ s16 angle;
    /* 0x1C */ u8 pad1C[0x3];
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 turnRate;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ u8 pad30[0x4];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 pad36[0x2];
} SwarmMemberState; /* size 0x38 */

/* Object type 0x3B: one member of a Swarm. */
struct SwarmMember {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ SwarmMemberState s;
    /* 0x48 */ s8 unk48;
};

/* Spawns a swarm at (x, y, z) following targetPos (or the fixed point x, y, z), plus memberCount members. */
void swarm_createSwarm(u8 memberCount, f32 *targetPos, u8 tag, s32 x, s32 y, s32 z, s32 arg6, s32 arg7, u8 arg8,
                   s32 playSfx, s32 argA) {
    SwarmMemberState template;
    Swarm *swarm;
    SwarmMember *member;
    s32 i;
    s32 angle;
    f32 c;
    f32 s;
    f32 r;
    s32 px;
    s32 pz;
    s32 py;
    s32 pad; // matching: unused stack slot keeps the frame size

    swarm = effect_newObject(0x3A, 0, sizeof(Swarm), 0, arg8, 1);
    if (swarm == NULL) {
        return;
    }
    if (argA == 0) {
        argA = 280;
    } else {
        argA = 140;
    }
    if (targetPos != NULL) {
        swarm->target.pos = targetPos;
        swarm->targetZ.packed = 0x80000000;
    } else {
        swarm->target.packed = (x << 16) | (y & 0xFFFF);
        swarm->targetZ.packed = z << 16;
    }
    swarm->x = x;
    swarm->y = y;
    swarm->z = z;
    swarm->yOffset = 0;
    swarm->speed = 800;
    swarm->memberCount = memberCount;
    swarm->tag = tag;
    swarm->playSfx = playSfx;
    swarm->sfxHandle = 0;
    if (0) {} // matching: the extra basic block steers IDO's register allocation
    template.anim = gEffectAnimTextures[6];
    template.swarm = swarm;
    template.velX = 0;
    template.velZ = 0;
    template.angle = 0;
    template.unk1F = 6;
    template.unk20 = 0;
    template.unk22 = 0;
    template.unk24 = argA;
    template.unk26 = argA;
    template.unk28 = arg7;
    template.unk2A = 0;
    template.unk2C = 0xFF;
    template.unk2D = 0xFF;
    template.unk2E = 0xFF;
    template.unk2F = 0xFF;
    template.unk34 = 0;
    for (i = 0; i < memberCount; i++) {
        angle = math_random() & 0xFF;
        c = math_sinBinAngle(angle);
        s = math_cosBinAngle(angle);
        r = (math_random() & 0xFFFF) * 50.0f * (1.0f / 65536.0f);
        px = x + r * c;
        pz = z + r * s;
        py = (math_random() & 0xFF) + y - 78;
        template.turnRate = (math_random() & 7) + 7;
        template.unkC = 0;
        template.unkE = 0;
        template.x = px;
        template.y = py;
        template.z = pz;
        member = effect_newObject(0x3B, 0, sizeof(SwarmMember), 0, arg8, 1);
        if (member == NULL) {
            break;
        }
        bcopy(&template, &member->s, sizeof(template));
        member->unk48 = -1;
    }
}


/* Member update: steer towards the swarm, bob up and down, and pick a sprite from its velocity relative to camera_global_Cameras->TrueCameraAngle. */
void swarm_updateMember(SwarmMember *member) {
    s32 timer;
    s32 turn;
    s32 delta;
    s32 velX;
    s32 diff;
    s32 angle;
    s32 heading;
    s32 v;
    s32 absDiff;
    f32 dist;
    f32 x;
    f32 y;
    f32 z;
    s32 rnd;
    Vec3f vec; /* direction to the target; only x and z are set */
    f32 dx;
    f32 dz;
    f32 s;
    f32 c;
    f32 radius;
    u8 yaw;
    Camera *gp;
    Swarm *swarm;
    s32 fast;

    swarm = member->s.swarm;
    angle = member->s.angle;
    x = swarm->x;
    y = swarm->y;
    z = swarm->z;
    if (swarm->speed > 800) {
        fast = TRUE;
    } else {
        fast = FALSE;
    }
    { // matching: -g3 scope (with a local) is a scheduling barrier for as1
        s32 pad;

        y += swarm->yOffset;
    }
    dx = x - swarm->x;
    dz = z - swarm->z;
    dist = sqrtf(dx * dx + dz * dz);
    if (dist > 100.0f) {
        dist = 100.0f / dist;
        x = dx * dist + swarm->x;
        z = dz * dist + swarm->z;
    }
    vec.x = x - member->s.x;
    vec.z = z - member->s.z;
    timer = member->s.steer & 0xF;
    if (timer == 0) {
        heading = math_vec3HeadingBin(&vec);
        heading += (s32)(math_random() & 0x1F) - 15;
        if (!fast) {
            timer = 10;
        } else {
            timer = 2;
        }
    } else {
        timer--;
        heading = (member->s.steer >> 4) & 0xFF;
    }
    member->s.steer = ((heading & 0xFF) << 4) | timer;
    turn = member->s.turnRate;
    if (fast) {
        turn *= 2;
    }
    if (angle < heading) {
        delta = turn;
    } else {
        delta = -turn;
    }
    diff = heading - angle;
    if (diff < 0) {
        absDiff = -diff;
    } else {
        absDiff = diff;
    }
    if (absDiff >= 0x80) {
        delta = -delta;
    }
    angle += delta;
    if (angle < 0) {
        angle += 0x100;
    } else if (angle >= 0x100) {
        angle -= 0x100;
    }
    radius = swarm->speed;
    velX = math_sinBinAngle(angle) * radius;
    c = -math_cosBinAngle(angle);
    member->s.velX = velX;
    member->s.velZ = c * radius;
    member->s.angle = angle;
    rnd = (math_random() & 0x7F) + 100;
    if ((math_random() & 0x3F) + (y + 50.0f) < member->s.y) {
        member->s.unk22 = -rnd;
    } else {
        member->s.unk22 = rnd;
    }
    if (member->s.unk20 > 0x400) {
        member->s.unk20 = 0x400;
    } else if (member->s.unk20 < -0x400) {
        member->s.unk20 = -0x400;
    }
    gp = camera_global_Cameras;
    yaw = gp->TrueCameraAngle * (256.0f / 360.0f);
    s = math_sinBinAngle(yaw);
    c = math_cosBinAngle(yaw);
    v = member->s.velX * c - member->s.velZ * s;
    if (v >= 0) {
        member->s.unk34 = 0;
    } else {
        member->s.unk34 = 4;
        v = -v;
    }
    if (v > 0x200) {
        v = 6;
    } else if (v > 0x100) {
        v = 7;
    } else {
        v = member->s.velX * s + member->s.velZ * c;
        if (v >= 0) {
            v = 8;
        } else {
            v = 9;
        }
    }
    member->s.anim = gEffectAnimTextures[v];
    member->s.unkC ^= 0x100;
}

/* Member hook: defers to effect_drawBillboard only when the swarm's viewMask has bit `view` set. */
Gfx *swarm_drawMember(Gfx *gfx, SwarmMember *member, s16 view) {
    Swarm *swarm;

    swarm = member->s.swarm;
    if (swarm->viewMask & (1 << view)) {
        return effect_drawBillboard(gfx, (BillboardParticle *)member, view);
    }
    return gfx;
}

/* The empty function at 0x151787A4. */
/* An empty callback: the function table gObjectFrameHooks points here. */
void swarm_doNothing(void) {
}

/*
 * Swarm update: move up to 16 units towards the target, recompute viewMask, update the looping sound,
 * and speed members up while far from the target.
 */
// matching: x, y, z are scratch parameters; as locals IDO allocates them differently
void swarm_updateSwarm(Swarm *swarm, s32 x, s32 y, s32 z) {
    f32 tx;
    f32 ty;
    f32 tz;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    s32 i;
    s32 mask;
    s32 px;
    s32 py;
    s32 pz;
    s32 ix;
    s32 iy;
    s32 iz;
    s32 distSq;
    s16 targetZ;
    f32 fx;
    f32 fy;
    f32 fz;

    targetZ = swarm->targetZ.z;
    x = swarm->x;
    y = swarm->y;
    z = swarm->z;
    swarm->prevX = x;
    swarm->prevY = y;
    swarm->prevZ = z;
    if (targetZ == -0x8000) {
        f32 *pos = swarm->target.pos;

        tx = pos[0];
        ty = pos[1];
        tz = pos[2];
    } else {
        tx = swarm->target.fixed.x;
        ty = swarm->target.fixed.y;
        tz = targetZ;
    }
    fx = x;
    fy = y;
    fz = z;
    dx = tx - fx;
    dy = ty - fy;
    dz = tz - fz;
    dist = sqrtf(dx * dx + dy * dy + dz * dz);
    if (dist > 1.0f) {
        dist = 16.0f / dist;
        swarm->x = x + dx * dist;
        swarm->y = y + dy * dist;
        swarm->z = z + dz * dist;
    }
    { // matching: -g3 scope (with a local) is a scheduling barrier for as1
        s32 pad;
        mask = 0;
        for (i = 0; i <= gScreenLayout; i++) {
            if (view_testCylinderInView(&gViews[i], gViewMatrices[i], swarm->x, swarm->y, swarm->z,
                              100.0f, 100.0f, 2500.0f)) {
                mask |= 1 << i;
            }
    }
    }
    swarm->viewMask = mask;
    if (swarm->playSfx) {
        if (swarm->sfxHandle == 0) {
            swarm->sfxHandle = sound_playAt(0x4A2, 10000, 0, 0, 0, swarm->x, swarm->y, swarm->z, 100, 800);
        } else {
            sound_setVoiceSpatial(swarm->sfxHandle, 10000, 0, 0, 0, swarm->x, swarm->y, swarm->z, 100, 800);
        }
    }
    if (swarm->targetZ.z == -0x8000) {
        f32 *pos = swarm->target.pos;

        px = pos[0];
        py = pos[1];
        pz = pos[2];
    } else {
        px = swarm->target.fixed.x;
        py = swarm->target.fixed.y;
        pz = swarm->targetZ.z;
    }
    py += swarm->yOffset;
    ix = px - swarm->x;
    iy = py - swarm->y;
    iz = pz - swarm->z;
    distSq = ix * ix + iy * iy + iz * iz;
    if (distSq < 0 || distSq == 0) {} // matching: the empty test shifts IDO's temp register numbering
    if (distSq > 10000) {
        if (swarm->speed != 1600) {
            swarm->speed += gFrameTicks + 40;
            if (swarm->speed > 1600) {
                swarm->speed = 1600;
            }
        }
    } else {
        if (swarm->speed != 800) {
            swarm->speed -= gFrameTicks + 40;
            if (swarm->speed < 800) {
                swarm->speed = 800;
            }
        }
    }
}

/* Returns the live swarm with the given tag, or NULL. */
Swarm *swarm_findByTag(u8 tag) {
    Swarm *swarm;

    for (swarm = (Swarm *)gObjectLists[0][58]; swarm != NULL; swarm = (Swarm *)swarm->header.next) {
        if (swarm->tag == tag) {
            return swarm;
        }
    }
    return NULL;
}

/* Makes the swarm with the given tag follow targetPos. */
void swarm_setTarget(u8 tag, f32 *targetPos, s16 yOffset) {
    Swarm *swarm;

    swarm = swarm_findByTag(tag);
    if (swarm != NULL) {
        swarm->target.pos = targetPos;
        swarm->targetZ.packed = 0x80000000;
        swarm->yOffset = yOffset;
    }
}

/* Makes the swarm with the given tag head for the fixed point (x, y, z). */
void swarm_setTargetPoint(u8 tag, s32 x, s32 y, s32 z, s16 yOffset) {
    Swarm *swarm;

    swarm = swarm_findByTag(tag);
    if (swarm != NULL) {
        swarm->target.packed = (x << 16) | (y & 0xFFFF);
        swarm->targetZ.packed = z << 16;
        swarm->yOffset = yOffset;
    }
}

/* Returns the distance from the swarm with the given tag to movement_obj[index], or 1 if there is none. */
s32 swarm_getDistanceToActor(u8 tag, s32 index) {
    Swarm *swarm;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 sum;

    swarm = swarm_findByTag(tag);
    if (swarm != NULL) {
        dx = movement_obj[index].xpos - swarm->x;
        dy = movement_obj[index].ypos - swarm->y;
        dz = movement_obj[index].zpos - swarm->z;
        dx = dx * dx;
        dy = dy * dy;
        dz = dz * dz;
        sum = dx + dy + dz;
        return sqrtf(sum);
    }
    return 1;
}

/* Stops the swarm's sound, frees all of its members, then frees the swarm. */
void swarm_freeSwarm(Swarm *swarm) {
    SwarmMember *next;
    SwarmMember *member;

    member = (SwarmMember *)gObjectLists[0][59];
    sound_stop(swarm->sfxHandle);
    while (member != NULL) {
        next = (SwarmMember *)member->header.next;
        if (swarm == member->s.swarm) {
            effect_destroyObject(&member->header);
        }
        member = next;
    }
    effect_defaultFree(&swarm->header);
}

/* Destroys the swarm with the given tag. */
void swarm_destroyByTag(u8 tag) {
    swarm_freeSwarm(swarm_findByTag(tag));
}
