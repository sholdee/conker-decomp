#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawner message handler: destroys the spawner when its owner (event 0x21), or the
 * object whose +0x318 points at its owner (event 0), sends the message. */
void func_150BE150(EB340Obj *obj, void **msg, u8 event) {
    if (event == 0x21) {
        if (obj->spawn.owner == msg[0]) {
            func_1516972C(&obj->base.header);
        }
    } else if (event == 0) {
        Actor *sender = msg[0];

        if (sender->CameraPtr == obj->spawn.owner) {
            func_1516972C(&obj->base.header);
        }
    }
}

/* Advances unk14 by unk80 per frame; returns 0 once it passes 120. */
s32 func_150BE1C4(EB340Rising *arg0) {
    arg0->unk14 += arg0->unk80 * D_800BE9A4;
    if (arg0->unk14 > 120.0f) {
        return 0;
    }
    return 1;
}

/* Until finished (task_flag state TASK_FLAG_ON): calls func_1511650C(1, 0x62C, 500), advances the work angle by
 * spinY while flags bit 2 is set (or clamps it to 270), and once it passes 360 marks itself finished and stops
 * its sound. */
void func_150BE210(WorldObject *obj) {
    if ((obj->task_flag & TASK_FLAG_STATE) == TASK_FLAG_ON) {
        return;
    }
    func_1511650C(obj, 1, 0x62C, 500.0f);
    if (obj->object_status & 4) {
        TURNER_WORK(obj)->angle += obj->spinY;
    } else if (TURNER_WORK(obj)->angle > 270.0f) {
        TURNER_WORK(obj)->angle = 270.0f;
    }
    if (TURNER_WORK(obj)->angle > 360.0f) {
        obj->task_flag &= ~TASK_FLAG_STATE;
        obj->task_flag |= TASK_FLAG_ON;
        obj->spinY = 0.0f;
        func_100111C8(obj->sound_id);
        obj->sound_id = 0;
    }
}

/* Variant tables of func_150BE494's debris (model ids and scales). */
const s32 D_800A0010[10] = { 0xC6, 0xC7, 0xC8, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD };
const f32 D_800A0038[10] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
const s32 D_800A0060[1] = { 0x39 };
const f32 D_800A0064[1] = { 1.0f };

// matching: 0.128f is read from a const array; the plain literal allocates FP registers differently
const f32 D_800A0068[1] = { 0.128f };

/*
 * Moves the object along from -> to. param holds two 2.14 fixed-point halves: the speed (high), which decays
 * by 12.8% per frame, and the progress from `from` to `to` (low, 0..1), which it is added to.
 */
void func_150BE2E8(WorldObject *mover) {
    f32 fromZ;
    f32 fromY;
    f32 fromX;
    f32 toX;
    f32 toY;
    f32 toZ;
    f32 speed;
    f32 progress;

    fromX = PATH_MOVER_WORK(mover)->from[0];
    fromY = PATH_MOVER_WORK(mover)->from[1];
    fromZ = PATH_MOVER_WORK(mover)->from[2];
    toX = PATH_MOVER_WORK(mover)->to[0];
    toY = PATH_MOVER_WORK(mover)->to[1];
    toZ = PATH_MOVER_WORK(mover)->to[2];
    speed = mover->moveData.half.hi * (1.0f / 16384.0f);
    progress = mover->moveData.half.lo * (1.0f / 16384.0f);
    speed -= speed * D_800A0068[0];
    progress += speed * D_800BE9E4;

    mover->x = (toX - fromX) * progress + fromX;
    mover->y = (toY - fromY) * progress + fromY;
    mover->z = (toZ - fromZ) * progress + fromZ;
    if (progress > 1.0f) {
        progress = 1.0; // matching: a 1.0f literal here gets CSE'd with the compare's 1.0f
    }

    mover->moveData.half.hi = speed * 16384.0f;
    mover->moveData.half.lo = progress * 16384.0f;
}

/* Writes object `id`'s two commands (0x68 = taskint[1], 0xE = taskint[0]) and returns the next slot. */
GameCommand *func_150BE438(GameCommand *cmd, s32 id) {
    cmd[0].opcode = 0x68;
    cmd[0].value = movement_obj[id].taskint[1];
    cmd[1].opcode = 0xE;
    cmd[1].value = movement_obj[id].taskint[0];
    return cmd + 2;
}


/* Spawns the burst of effects at `obj`'s centre: one func_1515080C, one func_15152190
 * and three func_15153F18 particle sets (kinds 0, 2 and 10). */
void func_150BE494(Actor *obj, u8 arg1, s32 arg2) {
    Vec3f pos;
    f32 radius;
    f32 unk2;
    f32 halfSize;
    CollisionHit ground; // the floor under obj
    DebrisSpray primary;
    DebrisBurst secondary;

    if (obj != NULL) {
        halfSize = (obj->scale_xz + obj->scale_y) * 0.5f;
        func_1515C244(obj, &pos, &radius, &unk2);
        func_1504715C(&ground, obj);
        primary.countBase = 10;
        primary.countRange = 0;
        primary.pos = pos;
        primary.angles.yawBase = 0;
        primary.angles.yawRange = 0xFF;
        primary.angles.pitchBase = -0x40;
        primary.angles.pitchRange = 0x32;
        primary.speedBase = 13.0f;
        primary.speedRange = 9.0f;
        primary.gravityBase = -1.326000094f;
        primary.gravityRange = 0.4410000145f;
        primary.lifeBase = 0x4B;
        primary.lifeRange = 0x28;
        primary.scaleBase = halfSize;
        primary.scaleRange = 0.0f;
        primary.spin = 14.09800053f;
        primary.floorHitChance = 1.0f;
        primary.floorHitFn = 0xB;
        func_1515080C(&primary, D_800A0010, D_800A0038, 10, 0, 0, 0, 1, -1, &ground, 55.0f, 0, 0, arg1, arg2);
        secondary.countBase = 0x11;
        secondary.countRange = 9;
        secondary.pos = pos;
        secondary.angles.yawBase = 0;
        secondary.angles.yawRange = 0xFF;
        secondary.angles.pitchBase = -0x3F;
        secondary.angles.pitchRange = 0x4B;
        secondary.speedBase = 14.0f;
        secondary.speedRange = 11.0f;
        secondary.lifeBase = 0x28;
        secondary.lifeRange = 0x14;
        secondary.gravityBase = -0.8000000119f;
        secondary.gravityRange = 0.3030000031f;
        secondary.scaleBase = 0.151000008f;
        secondary.scaleRange = 0.1590000093f;
        secondary.spin = 29.78700066f;
        func_15152190(&secondary, D_800A0060, D_800A0064, 1, 10.0f, 0, arg1, arg2);
        {
            SplashSpraySpec particles;

            particles.spray.pos = pos;
            particles.spray.scaleBase = 6.900000095f;
            particles.spray.scaleRange = 14.90000057f;
            particles.spray.sizeBase = 1.281000018f;
            particles.spray.sizeRange = 0.5430000424f;
            particles.spray.speedBase = 15.69999981f;
            particles.spray.speedRange = 18.89999962f;
            particles.spray.countBase = 4;
            particles.spray.countRange = 3;
            particles.angles.yawRange = 0xFF;
            particles.angles.yawBase = 0;
            particles.angles.pitchBase = -0x2B;
            particles.angles.pitchRange = 0x1A;
            particles.spray.itemCountBase = 3;
            particles.spray.itemCountRange = 3;
            particles.spray.lifeBase = 0x1E;
            particles.spray.lifeRange = 0x14;
            particles.spray.unk30 = 0x9B;
            particles.spray.unk32 = 0x64;
            particles.spray.unk3C = 0x10;
            particles.spray.unk3E = 0xF;
            particles.spray.unk40 = 0;
            particles.spray.unk38 = 0.5f;
            particles.spray.kind = 0;
            func_15153F18(&particles.angles, &particles.spray, &ground, arg1, arg2);
            particles.spray.kind = 2;
            func_15153F18(&particles.angles, &particles.spray, &ground, arg1, arg2);
            particles.spray.kind = 10;
            func_15153F18(&particles.angles, &particles.spray, &ground, arg1, arg2);
        }
    }
}
