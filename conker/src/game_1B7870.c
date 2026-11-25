#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

/* Orientation, scale and simple ballistic motion of a tumbling piece. */
typedef struct {
    /* 0x00 */ Vec3f angle;
    /* 0x0C */ f32 scale;
    /* 0x10 */ Vec3f velocity;
    /* 0x1C */ Vec3f angularVelocity;
    /* 0x28 */ f32 gravity;
    /* 0x2C */ f32 damping;
} EffectMotion;

/* Alpha fade-out read by func_15158AFC once the lifetime drops below duration. */
typedef struct {
    /* 0x00 */ s16 duration;
    /* 0x04 */ s32 alphaPerTick;
} EffectFade;

/* 0x44-byte template that func_151580B0 copies into EffectActor bytes 0x10..0x53. */
typedef struct {
    /* 0x00 */ u8 flags; /* 1: lifetime counts down, 2: attached to owner, 4: attach option */
    /* 0x01 */ u8 unk01;
    /* 0x02 */ s8 updateCallback; /* index into D_8008AE00 */
    /* 0x03 */ s8 drawCallback;   /* index into D_8008AE0C */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 displayList;    /* index into D_8008AFB8 */
    /* 0x07 */ u8 pad07;
    /* 0x08 */ s32 otherModeH;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 renderPreset;  /* index into D_800A4AC8 */
    /* 0x18 */ s32 unk18;         /* low byte lands in the actor's combineMode */
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s8 primColorMode;
    /* 0x25 */ s8 envColorMode;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ u8 color0[4]; /* [3] is alpha */
    /* 0x2C */ u8 color1[4];
    /* 0x30 */ void *owner;
    /* 0x34 */ u8 ownerId;
    /* 0x35 */ u8 attachMode;
    /* 0x36 */ u8 pad36[2];
    /* 0x38 */ Vec3f position;
} EffectTemplate;

/* Effect actor (see game_185560.c) with this file's type-specific data after the 0xF8-byte header. */
typedef struct {
    /* 0x000 */ u8 pad0[0x48];
    /* 0x048 */ Vec3f position;
    /* 0x054 */ u8 pad54[0xF8 - 0x54];
    /* 0x0F8 */ EffectFade fade;
    /* 0x100 */ EffectMotion motion;
} EffectActor;

extern s32 D_8008D5B0[];
extern s32 D_8008D5B8[];
extern EffectActor *func_151580B0(EffectTemplate *, s32, s32, u8, s32, u8, s32);

/*
 * Spawns a tumbling piece at position with the given motion; it fades out over its last 12 frames.
 * altDisplayList picks the display list table (D_8008D5B8 instead of D_8008D5B0).
 */
EffectActor *func_1518A3C0(Vec3f *position, Vec3f *angle, f32 scale, Vec3f *velocity, Vec3f *angularVelocity,
                           f32 gravity, f32 damping, u8 flags, s16 lifetime, u8 altDisplayList, s32 extraSize,
                           u8 arg11, s32 arg12) {
    EffectActor *actor;
    EffectTemplate template;
    EffectMotion motion;
    EffectFade fade;

    motion.angle = *angle;
    motion.velocity = *velocity;
    motion.angularVelocity = *angularVelocity;
    motion.scale = scale;
    motion.gravity = gravity;
    motion.damping = damping;
    template.flags = flags;
    template.updateCallback = 1;
    template.drawCallback = 1;
    template.lifetime = lifetime;

    if (altDisplayList) {
        template.displayList = D_8008D5B8[func_150ADA20() & 1];
    } else {
        template.displayList = D_8008D5B0[func_150ADA20() & 1];
    }

    template.unk0C = 0x220205;
    template.unk10 = 0x40600;
    template.envColorMode = 7;
    template.renderPreset = 1;
    template.unk18 = 0x4A;
    template.unk1C = 0x80;
    template.unk20 = 0x20;
    template.otherModeH = 0;
    template.primColorMode = 0;
    template.unk01 = 1;
    template.color0[0] = 255;
    template.color0[1] = 255;
    template.color0[2] = 255;
    template.color0[3] = 255;
    template.color1[0] = 255;
    template.color1[1] = 255;
    template.color1[2] = 255;
    template.color1[3] = 255;
    template.owner = NULL;
    template.ownerId = 0;
    template.attachMode = 2;
    template.position = *position;

    fade.duration = 12;
    fade.alphaPerTick = 21;

    actor = func_151580B0(&template, 3, 255, 1, extraSize + 0x38, arg11, arg12);
    if (actor == NULL) {
        return NULL;
    }
    memcpy(&actor->fade, &fade, sizeof(fade));
    memcpy(&actor->motion, &motion, sizeof(motion));
    return actor;
}

s32 func_15158AFC(EffectActor *);

/*
 * Update callback: fades the actor out, applies gravity and per-frame damping to the velocity,
 * moves the position with the average velocity over the step and spins the angle.
 */
s32 func_1518A5F4(EffectActor *actor) {
    EffectMotion *motion = &actor->motion;

    func_15158AFC(actor);

    {
        Vec3f prevVel;
        s32 i;

        prevVel = motion->velocity;
        motion->velocity.y += motion->gravity * D_800BE9A4;
        for (i = D_800BE9E4; i != 0; i--) {
            motion->velocity.x *= motion->damping;
            motion->velocity.y *= motion->damping;
            motion->velocity.z *= motion->damping;
        }
        {
            f32 accelX;
            f32 accelY;
            f32 accelZ;

            accelX = (motion->velocity.x - prevVel.x) * D_800BE9A8;
            accelY = (motion->velocity.y - prevVel.y) * D_800BE9A8;
            accelZ = (motion->velocity.z - prevVel.z) * D_800BE9A8;
            actor->position.x += (prevVel.x + 0.5f * accelX * D_800BE9A4) * D_800BE9A4;
            actor->position.y += (prevVel.y + 0.5f * accelY * D_800BE9A4) * D_800BE9A4;
            actor->position.z += (prevVel.z + 0.5f * accelZ * D_800BE9A4) * D_800BE9A4;
        }
    }
    motion->angle.x += motion->angularVelocity.x * D_800BE9A4;
    motion->angle.y += motion->angularVelocity.y * D_800BE9A4;
    motion->angle.z += motion->angularVelocity.z * D_800BE9A4;
    return 1;
}

/* Draw callback: builds the actor's model matrix from its angle, scale and position. */
s32 func_1518A914(Mtx *mtx, EffectActor *actor) {
    EffectMotion *motion = &actor->motion;
    f32 mf[4][4];

    func_150A8050(mf, motion->angle.x, motion->angle.y, motion->angle.z);
    mf[3][0] = actor->position.x;
    mf[3][1] = actor->position.y;
    mf[3][2] = actor->position.z;
    mf[0][0] *= motion->scale;
    mf[0][1] *= motion->scale;
    mf[0][2] *= motion->scale;
    mf[1][0] *= motion->scale;
    mf[1][1] *= motion->scale;
    mf[1][2] *= motion->scale;
    mf[2][0] *= motion->scale;
    mf[2][1] *= motion->scale;
    mf[2][2] *= motion->scale;
    guMtxF2L(mf, mtx);
    return 1;
}
