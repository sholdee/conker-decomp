#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The 8-byte payload func_150BDE90 copies to +0x28 of the object it creates. */
typedef struct {
    /* 0x00 */ Camera *owner;
    /* 0x04 */ s16 timer; // countdown to the next emitter spawn
    /* 0x06 */ s16 unk6;
} EB340Spawn;

/* The object func_150BDE90 creates; func_150BDF0C is its per-frame update. */
typedef struct {
    /* 0x00 */ u8  pad0[0xC];
    /* 0x0C */ u8  unkC;
    /* 0x0D */ u8  padD[0x1B];
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

/* Same 0x58-byte particle-emitter descriptor as struct_150CF680 in game_FC5F0.c
 * and struct_151CAB78_sp90 in game_1F4650.c; func_1515548C copies 0x58 of it. */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
} EB340Emitter;

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

extern void *func_1515548C(EB340Emitter *, s32, s32, s32, s32, u8, s32);

/* Spawner update: when the timer runs out, spawns one emitter near the owner and
 * restarts the timer at a random 25..175. */
void func_150BDF0C(EB340Obj *obj) {
    EB340Spawn *spawn;
    EB340Emitter emitter;
    void *effect;
    EB340Payload payload;

    obj->spawn.timer -= D_800BE9E4;
    spawn = &obj->spawn;
    if (obj->spawn.timer < 0) {
        emitter.unk0 = (func_150ADA68() * 270.0f) + -135.0f;
        emitter.unk4 = -120.0f;
        emitter.unk8 = (func_150ADA68() * 10.0f) + 3.0f;
        emitter.unkC = (func_150ADA68() * 17.0f) + 9.0f;
        emitter.unk10 = 0xAB;
        emitter.unk12 = 1000;
        emitter.unk14 = 0x31;
        emitter.unk16 = 1;
        emitter.unk18 = 0xFF;
        emitter.unk1A = 7;
        emitter.unk1B = 0xFF;
        emitter.unk1C = 0xFF;
        emitter.unk1D = 0xFF;
        emitter.unk1E = (func_150ADA20() % 156U) + 100;
        emitter.unk1F = 0xFF;
        emitter.unk20 = 0xFF;
        emitter.unk21 = 0xFF;
        emitter.unk22 = 0xFF;
        emitter.unk23 = 0xFF;
        emitter.unk24 = 0;
        emitter.unk28 = 0x200004;
        emitter.unk2C = 0x1F0601;
        emitter.unk30 = 3;
        emitter.unk34 = 0x22;
        emitter.unk38 = 0x80;
        emitter.unk3C = 0x20;
        emitter.unk40 = 0;
        emitter.unk41 = 7;
        emitter.unk44 = spawn->owner->playerIndex;
        emitter.unk48 = 1.0f;
        emitter.unk4C = 1.0f;
        emitter.unk50 = 0.0f;
        emitter.unk54 = 0.0f;
        payload.owner = spawn->owner;
        payload.unkD = 0;
        payload.unkC = 0;
        payload.unk10 = (func_150ADA68() * 4.06400013f) + 2.513000011f;
        effect = func_1515548C(&emitter, 10, 0, 0, sizeof(emitter), obj->unkC, 0);
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
            func_1516972C((struct102 *)obj);
        }
    } else if (event == 0) {
        Actor *sender = msg[0];

        if (sender->camera == obj->spawn.owner) {
            func_1516972C((struct102 *)obj);
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

/* func_1515080C parameter block (same layout as game_11C2B0.c's Cfg150F1D10A). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s8 unk40;
} EB340Cfg80C;

/* func_15152190 parameter block (same layout as game_11C2B0.c's Cfg150F1D10B). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
} EB340Cfg2190;

/* func_15153F18 parameter block (same layout as game_16EE20.c's Params15153F18). */
typedef struct {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ s8 kind;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s32 unk48;
} EB340Cfg3F18;

extern s32 D_800A0010[];
extern f32 D_800A0038[];
extern s32 D_800A0060[];
extern f32 D_800A0064[];

void func_1515C244(Actor *, Vec3f *, f32 *, f32 *);
void func_1504715C(void *, Actor *);
void func_1515080C(EB340Cfg80C *, s32 *, f32 *, s32, s32, s32, s32, s32, s32, void *, f32, s32, s32, u8, s32);
void func_15152190(EB340Cfg2190 *, s32 *, f32 *, s32, f32, s32, u8, s32);
void func_15153F18(EB340Cfg3F18 *, Vec3f *, void *, u8, s32);

/* Spawns the burst of effects at `obj`'s centre: one func_1515080C, one func_15152190
 * and three func_15153F18 particle sets (kinds 0, 2 and 10). */
void func_150BE494(Actor *obj, u8 arg1, s32 arg2) {
    Vec3f pos;
    f32 radius;
    f32 unk2;
    f32 halfSize;
    u8 ground[0x24]; // filled by func_1504715C (a ground probe in other files)
    EB340Cfg80C primary;
    EB340Cfg2190 secondary;

    if (obj != NULL) {
        halfSize = (obj->xz_scale + obj->y_scale) * 0.5f;
        func_1515C244(obj, &pos, &radius, &unk2);
        func_1504715C(ground, obj);
        primary.unk0 = 10;
        primary.unk4 = 0;
        primary.pos = pos;
        primary.unk14 = 0;
        primary.unk16 = 0xFF;
        primary.unk18 = -0x40;
        primary.unk1A = 0x32;
        primary.unk1C = 13.0f;
        primary.unk20 = 9.0f;
        primary.unk24 = -1.326000094f;
        primary.unk28 = 0.4410000145f;
        primary.unk2C = 0x4B;
        primary.unk2E = 0x28;
        primary.unk30 = halfSize;
        primary.unk34 = 0.0f;
        primary.unk38 = 14.09800053f;
        primary.unk3C = 1.0f;
        primary.unk40 = 0xB;
        func_1515080C(&primary, D_800A0010, D_800A0038, 10, 0, 0, 0, 1, -1, ground, 55.0f, 0, 0, arg1, arg2);
        secondary.unk0 = 0x11;
        secondary.unk4 = 9;
        secondary.pos = pos;
        secondary.unk14 = 0;
        secondary.unk16 = 0xFF;
        secondary.unk18 = -0x3F;
        secondary.unk1A = 0x4B;
        secondary.unk1C = 14.0f;
        secondary.unk20 = 11.0f;
        secondary.unk2C = 0x28;
        secondary.unk2E = 0x14;
        secondary.unk24 = -0.8000000119f;
        secondary.unk28 = 0.3030000031f;
        secondary.unk30 = 0.151000008f;
        secondary.unk34 = 0.1590000093f;
        secondary.unk38 = 29.78700066f;
        func_15152190(&secondary, D_800A0060, D_800A0064, 1, 10.0f, 0, arg1, arg2);
        {
            EB340Cfg3F18 particles;

            particles.pos = pos;
            particles.unk14 = 6.900000095f;
            particles.unk18 = 14.90000057f;
            particles.unk1C = 1.281000018f;
            particles.unk20 = 0.5430000424f;
            particles.unk24 = 15.69999981f;
            particles.unk28 = 18.89999962f;
            particles.unk2C = 4;
            particles.unk2E = 3;
            particles.unk02 = 0xFF;
            particles.unk00 = 0;
            particles.unk04 = -0x2B;
            particles.unk06 = 0x1A;
            particles.unk30 = 3;
            particles.unk32 = 3;
            particles.unk34 = 0x1E;
            particles.unk36 = 0x14;
            particles.unk38 = 0x9B;
            particles.unk3A = 0x64;
            particles.unk44 = 0x10;
            particles.unk46 = 0xF;
            particles.unk48 = 0;
            particles.unk40 = 0.5f;
            particles.kind = 0;
            func_15153F18(&particles, &particles.pos, ground, arg1, arg2);
            particles.kind = 2;
            func_15153F18(&particles, &particles.pos, ground, arg1, arg2);
            particles.kind = 10;
            func_15153F18(&particles, &particles.pos, ground, arg1, arg2);
        }
    }
}
