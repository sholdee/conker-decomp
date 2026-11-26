#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The zones TriggerZones used here are collision cylinders (ZONE_AREA_CYLINDER, tested by
   func_150A34B0 in game_D0960.c). */

extern s32 D_80088AA0[4];
extern s32 D_80088AB0[8];

/* Tests the segment from start to start + delta against the gate cylinders. */
void func_150EB430(Vec3f *start, Vec3f *delta, Vec3f *hit) {
    Vec3f end;

    end.x = start->x + delta->x;
    end.y = start->y + delta->y;
    end.z = start->z + delta->z;
    func_150EB484(start, &end, hit);
}

/*
 * Tests the segment from start to end against the zones cylinders. While object 9 exists,
 * a hit on one of the four D_80088AA0 cylinders disables it (func_150A2864(id, 1)), stores
 * which one (1-4) in the object's taskint[1] and plays func_150EBC80's effect at it; the eight
 * D_80088AB0 cylinders are only tested. Otherwise only cylinder 76 is tested, and a hit sets
 * its data1. Returns TRUE when object 9 exists and the segment hit one of the twelve.
 */
s32 func_150EB484(Vec3f *start, Vec3f *end, Vec3f *hit) {
    s32 i;
    s32 id;
    Actor *obj;

    obj = func_15083E90(9);
    if (obj != NULL) {
        for (i = 0; i < 4; i++) {
            id = D_80088AA0[i];
            if (func_150A34B0(&zones[id], start, end, hit)) {
                func_150A2864(id, 1);
                obj->taskint[1] = i + 1;
                func_150EBC80(&zones[id], 0xFF, 1);
                return TRUE;
            }
        }
        for (i = 0; i < 8; i++) {
            if (func_150A34B0(&zones[D_80088AB0[i]], start, end, hit)) {
                return TRUE;
            }
        }
        return FALSE;
    }
    if (func_150A34B0(&zones[76], start, end, hit)) {
        zones[76].data1 = 1;
    }
    return FALSE;
}

#define WORK(obj) ((MoverWork *)&(obj)->touch)

/*
 * Mover handler. In states 0-1 it swings rotX on a damped spring toward an angle set by the
 * player's distance (with flags & 4: -40 degrees at the player rising to -32 at 740 units;
 * otherwise -40). Once the player's mv->unk57 is set it switches to state 2, which bounces
 * rotX back to 0 with func_15117770 until it stops; then state 3 keeps it at 0.
 */
void func_150EB614(WorldObject *obj) {
    f32 velocity;
    f32 dz;
    f32 target;
    f32 dx;
    f32 damping;
    f32 acceleration;
    f32 dy;
    f32 angle;
    f32 before;
    f32 distance;
    f32 delta;
    f32 magnitude;

    if ((obj->task_flag & TASK_FLAG_STATE) == TASK_FLAG_ON) {
        return;
    }
    if ((obj->task_flag & TASK_FLAG_STATE) != TASK_FLAG_GOING_ON) {
        if (obj->object_status & 4) {
            if (movement_obj->mv->unk57 != 0) {
                obj->task_flag &= ~TASK_FLAG_STATE;
                obj->task_flag |= TASK_FLAG_GOING_ON;
                obj->moveData.word = 0;
                WORK(obj)->bounce.rebound = 0.5f;
                WORK(obj)->bounce.acceleration = 0.3000000119f;
                WORK(obj)->bounce.velocity = 0.0f;
            }
            dz = obj->z - movement_obj->zpos;
            dx = obj->x - movement_obj->xpos;
            dy = obj->y - movement_obj->ypos;
            distance = sqrtf(dz * dz + (dx * dx + dy * dy));
            if (distance > 740.0f) {
                distance = 740.0f;
            }
            target = distance * 0.01081081107f + -40.0f;
        } else {
            target = -40.0f;
        }
        angle = obj->t;
        damping = WORK(obj)->swing.damping;
        acceleration = WORK(obj)->swing.acceleration;
        velocity = WORK(obj)->swing.velocity;
        if (angle != target || velocity != 0.0f) {
            angle += velocity * D_800BE9E4;
            delta = func_15048A70(angle, target);
            magnitude = fabsf(delta);
            if (delta > 0.0f) {
                velocity += magnitude * acceleration;
            } else {
                velocity -= magnitude * acceleration;
            }
            velocity *= damping;
            if (fabsf(delta) < 0.009999999776f && fabsf(velocity) < 0.009999999776f) {
                velocity = 0.0f;
                angle = target;
            } else if (angle < 0.0f) {
                angle += 360.0f;
            } else if (angle >= 360.0f) {
                angle -= 360.0f;
            }
            WORK(obj)->swing.velocity = velocity;
        }
        obj->t = angle;
    } else {
        before = obj->t;
        func_15117770(obj);
        if (before == obj->t) {
            obj->task_flag &= ~TASK_FLAG_STATE;
            obj->task_flag |= TASK_FLAG_ON;
            obj->t = 0.0f;
        }
    }
}

extern Vec3f D_800A1490;
extern Vec3f D_800A149C;
extern Vec3f D_800A14A8[4];
extern u8 D_800A14D8[4];

/*
 * Plays a fixed burst of effects: func_15164F0C, a func_150E83AC effect at D_800A149C lasting
 * 120-181 frames, the func_151D5514/func_151D3FF4/func_151541B8 trio at D_800A1490, and four
 * func_1513264C debris pieces thrown from the D_800A14A8 points at random yaw/pitch/speed.
 */
void func_150EB8C4(void) {
    ParticleSpawn spawn;

    func_15164F0C(2, D_800BE9E8, NULL, 0xFF, 1);
    func_150E83AC(&D_800A149C, func_150ADA20() % 62U + 120, 0xFF, 1);
    func_151D5514(&D_800A1490, 0xFF, 1);
    func_151D3FF4(&D_800A1490, 0xFF, 1);
    func_151541B8(&D_800A1490, func_150ADA68() * 4.0f + 12.0f, 1.640999913f,
                  func_150ADA20() % 56U + 200, 0.0f, 0xFF, 1);
    {
        u8 i;
        s32 yaw;
        s32 pitch;

        spawn.sizeY = 1.0f;
        spawn.sizeXZ = 1.0f;
        spawn.radius = 9.0f;
        spawn.elasticity = 0.4350000322f;
        spawn.rot = D_800A5480;
        spawn.rotVel.y = 0.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.flags = 0x29E9;
        spawn.variant = 0;
        spawn.unk5C = 0;
        spawn.alpha = 0xFF;
        spawn.updateFn = 13;
        spawn.update2Fn = 0;
        spawn.floorHitFn = 12;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 2;
        spawn.unk6A = 2;
        spawn.owner = NULL;
        spawn.ownerId = 0;
        spawn.fadeStart = 25;
        spawn.fadeRate = 10;
        spawn.sound = 0;
        // matching: an early use of pitch makes IDO give it $s1 and yaw $s2
        if (pitch) {
        }
        for (i = 0; i < 4; i++) {
            spawn.pos = D_800A14A8[i];
            yaw = func_150ADA20();
            pitch = func_150ADA20();
            func_15143794(yaw % 26U + 45, (pitch & 0xF) - 25, func_150ADA68() * 30.0f + 15.0f, &spawn.vel);
            spawn.rotVel.x = func_150ADA68() * 15.10800076f + -7.554000378f;
            spawn.rotVel.z = func_150ADA68() * 15.10800076f + -7.554000378f;
            spawn.gravity = func_150ADA68() * 0.6170000434f + -1.621000051f;
            spawn.lifetime = func_150ADA20() % 51U + 50;
            spawn.model = D_800A14D8[i];
            func_1513264C(&spawn, 3, 0xFF, 0, 0, 0xFF, 1);
        }
    }
}


/*
 * Effect burst at a gate cylinder: a func_150E83AC effect lasting 300-400 frames, a rumble
 * (type 1, 15-35 long), func_15164F0C, and the func_151D3FF4/func_151D5514/func_151541B8 trio at
 * the cylinder's base and again half its height up.
 */
void func_150EBC80(TriggerZone *gate, u8 owner, s32 kind) {
    Vec3f position;
    f32 random;
    f32 step;
    RumbleParams event;
    f32 growth;
    u8 i;

    position.x = gate->x;
    position.y = gate->y;
    position.z = gate->z;
    step = gate->ySize * 0.5f;
    func_150E83AC(&position, func_150ADA20() % 101U + 300, owner, kind);
    event.flags = 1;
    event.duration = func_150ADA20() % 21U + 15;
    event.strength = 0;
    event.updateKind = -1;
    event.playerMask = 1;
    func_151D8868(&event, 0, owner, kind);
    func_15164F0C(0, 0, NULL, owner, kind);
    growth = 1.640999913f; // matching: a local (not a literal) is loaded once into a saved FP register
    for (i = 0; i < 2; i++) {
        func_151D3FF4(&position, owner, kind);
        func_151D5514(&position, owner, kind);
        random = func_150ADA68();
        func_151541B8(&position, random * 4.0f + 12.0f, growth,
                      func_150ADA20() % 56U + 200, 0.0f, owner, kind);
        position.y += step;
    }
}
