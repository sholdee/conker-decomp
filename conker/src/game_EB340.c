#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* The 8-byte payload func_150BDE90 copies to +0x28 of the object it creates. */
typedef struct {
    /* 0x00 */ Camera *owner;
    /* 0x04 */ s16 timer; // countdown to the next emitter spawn
    /* 0x06 */ s16 unk6;
} EB340Spawn;

/* The object func_150BDE90 creates; func_150BDF0C is its per-frame update. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EB340Spawn spawn;
} EB340Obj;

/* Creates the spawner object for `owner`, with its spawn timer at 0. */
void func_150BDE90(Camera *owner, u8 arg1, s32 arg2) {
    EB340Spawn spawn;
    EB340Obj *obj;

    spawn.owner = owner;
    spawn.timer = 0;

    obj = (EB340Obj *)func_15149130(300, -1, 0x4F, -1, 0, 60, 8, arg1, arg2);
    if (obj != NULL) {
        memcpy(&obj->spawn, &spawn, sizeof(spawn));
    }
}

/* Copied to +0x70 of the emitter object func_1515548C returns. */
typedef struct {
    /* 0x00 */ Camera *owner;
    /* 0x04 */ u8   pad4[0x8];
    /* 0x0C */ u8   unkC;
    /* 0x0D */ u8   unkD;
    /* 0x0E */ u8   padE[0x2];
    /* 0x10 */ f32  unk10;
    /* 0x14 */ u8   pad14[0x44];
} EB340Payload;

extern void *func_1515548C(ScreenSpriteInfo *, s32, s32, s32, s32, u8, s32);

/* Spawner update: when the timer runs out, spawns one emitter near the owner and
 * restarts the timer at a random 25..175. */
void func_150BDF0C(EB340Obj *obj) {
    EB340Spawn *spawn;
    ScreenSpriteInfo emitter;
    void *effect;
    EB340Payload payload;

    obj->spawn.timer -= D_800BE9E4;
    spawn = &obj->spawn;
    if (obj->spawn.timer < 0) {
        emitter.x = (func_150ADA68() * 270.0f) + -135.0f;
        emitter.y = -120.0f;
        emitter.halfWidth = (func_150ADA68() * 10.0f) + 3.0f;
        emitter.halfHeight = (func_150ADA68() * 17.0f) + 9.0f;
        emitter.texture = 0xAB;
        emitter.timer = 1000;
        emitter.flags = 0x31;
        emitter.fadeStart = 1;
        emitter.fadeRate = 0xFF;
        emitter.kind = 7;
        emitter.color1.red = 0xFF;
        emitter.color1.green = 0xFF;
        emitter.color1.blue = 0xFF;
        emitter.color1.alpha = (func_150ADA20() % 156U) + 100;
        emitter.color2.red = 0xFF;
        emitter.color2.green = 0xFF;
        emitter.color2.blue = 0xFF;
        emitter.color2.alpha = 0xFF;
        emitter.unk23 = 0xFF;
        emitter.otherModeH = 0;
        emitter.unk28 = 0x200004;
        emitter.unk2C = 0x1F0601;
        emitter.combineMode = 3;
        emitter.unk34 = 0x22;
        emitter.unk38 = 0x80;
        emitter.unk3C = 0x20;
        emitter.unk40 = 0;
        emitter.unk41 = 7;
        emitter.viewport = spawn->owner->playerIndex;
        emitter.scaleX = 1.0f;
        emitter.scaleY = 1.0f;
        emitter.offsetS = 0.0f;
        emitter.offsetT = 0.0f;
        payload.owner = spawn->owner;
        payload.unkD = 0;
        payload.unkC = 0;
        payload.unk10 = (func_150ADA68() * 4.06400013f) + 2.513000011f;
        effect = func_1515548C(&emitter, 10, 0, 0, sizeof(emitter), obj->base.header.owner, 0);
        if (effect != NULL) {
            memcpy((u8 *)effect + 0x70, &payload, sizeof(payload));
        }
        spawn->timer = (func_150ADA20() % 151U) + 25;
    }
}

/* Spawner message handler: destroys the spawner when its owner (event 0x21), or the
 * object whose +0x318 points at its owner (event 0), sends the message. */
void func_150BE150(EB340Obj *obj, void **msg, u8 event) {
    if (event == 0x21) {
        if (obj->spawn.owner == msg[0]) {
            func_1516972C(&obj->base.header);
        }
    } else if (event == 0) {
        Actor *sender = msg[0];

        if (sender->camera == obj->spawn.owner) {
            func_1516972C(&obj->base.header);
        }
    }
}

typedef struct {
    /* 0x00 */ u8  pad0[0x14];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8  pad18[0x68];
    /* 0x80 */ f32 unk80;
} EB340Rising;

/* Advances unk14 by unk80 per frame; returns 0 once it passes 120. */
s32 func_150BE1C4(EB340Rising *arg0) {
    arg0->unk14 += arg0->unk80 * D_800BE9A4;
    if (arg0->unk14 > 120.0f) {
        return 0;
    }
    return 1;
}

/* Behaviour scratch state (WorldObject.work) of func_150BE210. */
typedef struct {
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ f32 angle; // turns at spinY per frame until past 360
} TurnerWork;

#define TURNER_WORK(obj) ((TurnerWork *)&(obj)->work)

extern void func_1511650C(WorldObject *obj, s32, s32, f32);

/* Until finished (behaviorState & 3 == 3): calls func_1511650C(1, 0x62C, 500), advances the work angle by
 * spinY while flags bit 2 is set (or clamps it to 270), and once it passes 360 marks itself finished and stops
 * its sound. */
void func_150BE210(WorldObject *obj) {
    if ((obj->behaviorState & 3) == 3) {
        return;
    }
    func_1511650C(obj, 1, 0x62C, 500.0f);
    if (obj->flags & 4) {
        TURNER_WORK(obj)->angle += obj->spinY;
    } else if (TURNER_WORK(obj)->angle > 270.0f) {
        TURNER_WORK(obj)->angle = 270.0f;
    }
    if (TURNER_WORK(obj)->angle > 360.0f) {
        obj->behaviorState &= ~3;
        obj->behaviorState |= 3;
        obj->spinY = 0.0f;
        func_100111C8(obj->sound);
        obj->sound = 0;
    }
}

/* Behaviour scratch state (WorldObject.work) of func_150BE2E8: the end points of its path. */
typedef struct {
    /* 0x7C */ s16 from[3];
    /* 0x82 */ s16 to[3];
} MoverWork;

#define MOVER_WORK(obj) ((MoverWork *)&(obj)->work)

// matching: 0.128f read through its rodata symbol; the plain literal allocates FP registers differently
extern f32 D_800A0068;

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

    fromX = MOVER_WORK(mover)->from[0];
    fromY = MOVER_WORK(mover)->from[1];
    fromZ = MOVER_WORK(mover)->from[2];
    toX = MOVER_WORK(mover)->to[0];
    toY = MOVER_WORK(mover)->to[1];
    toZ = MOVER_WORK(mover)->to[2];
    speed = mover->param.half.hi * (1.0f / 16384.0f);
    progress = mover->param.half.lo * (1.0f / 16384.0f);
    speed -= speed * D_800A0068;
    progress += speed * D_800BE9E4;

    mover->x = (toX - fromX) * progress + fromX;
    mover->y = (toY - fromY) * progress + fromY;
    mover->z = (toZ - fromZ) * progress + fromZ;
    if (progress > 1.0f) {
        progress = 1.0; // matching: a 1.0f literal here gets CSE'd with the compare's 1.0f
    }

    mover->param.half.hi = speed * 16384.0f;
    mover->param.half.lo = progress * 16384.0f;
}

/* Two-halfword command, as in game_61490.c's GameCommand. */
typedef struct {
    /* 0x00 */ s16 opcode;
    /* 0x02 */ s16 value;
} EB340Command;

/* Writes object `id`'s two commands (0x68 = unk2E8, 0xE = unk2E4) and returns the next slot. */
EB340Command *func_150BE438(EB340Command *cmd, s32 id) {
    cmd[0].opcode = 0x68;
    cmd[0].value = D_800CC2D0[id].unk2E8;
    cmd[1].opcode = 0xE;
    cmd[1].value = D_800CC2D0[id].unk2E4;
    return cmd + 2;
}

extern s32 D_800A0010[];
extern f32 D_800A0038[];
extern s32 D_800A0060[];
extern f32 D_800A0064[];

void func_1515C244(Actor *, Vec3f *, f32 *, f32 *);
void func_1504715C(void *, Actor *);
void func_1515080C(DebrisSpray *, s32 *, f32 *, s32, s32, s32, s32, s32, s32, void *, f32, s32, s32, u8, s32);
void func_15152190(DebrisBurst *, s32 *, f32 *, s32, f32, s32, u8, s32);
void func_15153F18(AngleRange *, SplashSpray *, void *, u8, s32);

/* Spawns the burst of effects at `obj`'s centre: one func_1515080C, one func_15152190
 * and three func_15153F18 particle sets (kinds 0, 2 and 10). */
void func_150BE494(Actor *obj, u8 arg1, s32 arg2) {
    Vec3f pos;
    f32 radius;
    f32 unk2;
    f32 halfSize;
    u8 ground[0x24]; // filled by func_1504715C (a ground probe in other files)
    DebrisSpray primary;
    DebrisBurst secondary;

    if (obj != NULL) {
        halfSize = (obj->xz_scale + obj->y_scale) * 0.5f;
        func_1515C244(obj, &pos, &radius, &unk2);
        func_1504715C(ground, obj);
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
        func_1515080C(&primary, D_800A0010, D_800A0038, 10, 0, 0, 0, 1, -1, ground, 55.0f, 0, 0, arg1, arg2);
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
            func_15153F18(&particles.angles, &particles.spray, ground, arg1, arg2);
            particles.spray.kind = 2;
            func_15153F18(&particles.angles, &particles.spray, ground, arg1, arg2);
            particles.spray.kind = 10;
            func_15153F18(&particles.angles, &particles.spray, ground, arg1, arg2);
        }
    }
}
