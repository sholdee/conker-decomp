#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    u8 pad0[9];
    u8 unk9;
    u8 padA[4];
    s16 unkE;
    s16 unk10;
    s16 unk12;
    u8 pad14[0x1B];
    u8 unk2F;
} FF0E0ChildData;

typedef struct {
    EffectHeader header;
    u8 pad10[0x4];
    FF0E0ChildData *unk14;
} FF0E0Child;

/* Per-object state stored at offset 0x28 of the object. */
typedef struct {
    Vec3f position;
    Vec3f rotation;
    Vec3f angularVelocity;
    FF0E0Child *children[6];
    f32 intensity;
    s32 unk40;
    u8 flags; // bit 0: enabled
} FF0E0State;

struct FF0E0Object {
    TimedEffect base;
    FF0E0State state;
};

/*
 * This file's .rodata front-end data: the six ray directions, the colours of the six children
 * (level_spawnDiscoBallZone of game_40490.c reads the first six), and the spin presets that game/Script/EffectCmd/effectcmd.c sends
 * with message 0x32.
 */
static const Vec3f D_800A08F0[6] = {
    { 0.0f, 10.0f, 0.0f }, { 0.0f, -10.0f, 0.0f }, { 10.0f, 0.0f, 0.0f },
    { -10.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, -10.0f },
};
const u8 D_800A0938[13][3] = {
    { 0xFF, 0x00, 0x00 }, { 0xFF, 0x00, 0x00 }, { 0x00, 0xFF, 0x00 }, { 0x00, 0xFF, 0x00 },
    { 0x00, 0x00, 0xFF }, { 0x00, 0x00, 0xFF }, { 0x00, 0x00, 0xFF }, { 0x00, 0x00, 0xFF },
    { 0x00, 0x00, 0xFF }, { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF }, { 0xFF, 0x00, 0xFF },
    { 0xFF, 0x00, 0x00 },
};
const FF0E0SpinMsg D_800A0960[3] = {
    { { 0.91200006f, 2.4320002f, -1.064f }, 0.00129f },
    { { 1.2160001f, 5.6250005f, -0.76000005f }, 0.012363f },
    { { 3.3450003f, 9.882001f, -2.4320002f }, 0.03f },
};

/* Spins the object and casts a ray from its position along each of the six rotated directions,
 * moving each child to the hit point (unk9 = 0) or flagging it unk9 = 1 on a miss or while disabled. */
void func_150D1C30(FF0E0Object *obj) {
    FF0E0State *state;
    FF0E0Child *child;
    MtxF mtx;
    s32 i;

    state = &obj->state;
    if (obj->state.flags & 1) {
        state->rotation.x += state->angularVelocity.x * gFrameDelta;
        state->rotation.y += state->angularVelocity.y * gFrameDelta;
        state->rotation.z += state->angularVelocity.z * gFrameDelta;
        func_150A8050(mtx, state->rotation.x, state->rotation.y, state->rotation.z);
        for (i = 0; i < 6; i++) {
            child = state->children[i];
            if (child != NULL) {
                f32 targetZ;
                f32 targetY;
                f32 targetX;
                f32 hitZ;
                f32 hitY;
                f32 hitX;
                f32 fraction;
                func_150A7960(mtx, D_800A08F0[i].x, D_800A08F0[i].y, D_800A08F0[i].z, &targetX, &targetY, &targetZ);
                if (func_150AC9C0(state->position.x, state->position.y, state->position.z, targetX, targetY, targetZ,
                                  NULL, NULL, &hitX, &hitY, &hitZ, &fraction, NULL, NULL, 0.0f)) {
                    child->unk14->unk9 = 0;
                    child->unk14->unkE = hitX;
                    child->unk14->unk10 = hitY;
                    child->unk14->unk12 = hitZ;
                    child->unk14->unk2F = state->intensity * fraction;
                } else {
                    child->unk14->unk9 = 1;
                }
            }
        }
    } else {
        for (i = 0; i < 6; i++) {
            FF0E0Child *disabledChild = state->children[i];
            if (disabledChild != NULL) {
                disabledChild->unk14->unk9 = 1;
            }
        }
    }
}
/* Message handler: 0x32 sets the spin and intensity, 0x30/0x31 enable/disable the object when the
 * argument matches unk40, 0x4E resets the spin from gZeroVec3f, 0x4F passes the object to effect_destroyObject. */
void func_150D1F6C(FF0E0Object *obj, s32 arg, u8 msg) {
    FF0E0State *state;
    FF0E0SpinMsg *msgData;

    state = &obj->state;
    if (msg == 0x32) {
        msgData = (FF0E0SpinMsg *)arg;
        state->angularVelocity = msgData->angularVelocity;
        state->intensity = msgData->intensity;
    // matching: empty labelled block makes as1 fill the branch delay slot from the epilogue
    end_set: ;
    } else if (msg == 0x30 || msg == 0x31) {
        if (arg == state->unk40) {
            if (msg == 0x30) {
                state->flags |= 1;
            } else {
                state->flags &= ~1;
            // matching: as above
            end_disable: ;
            }
        }
    } else if (msg == 0x4E) {
        obj->state.angularVelocity = gZeroVec3f;
    } else if (msg == 0x4F) {
        effect_destroyObject(&obj->base.header);
    }
}

/* Passes every child to effect_destroyObject. */
void func_150D2054(FF0E0Object *obj) {
    FF0E0State *state;
    FF0E0Child *child;
    u8 i;

    state = &obj->state;
    for (i = 0; i < 6; i++) {
        child = state->children[i];
        if (child != NULL) {
            effect_destroyObject(&child->header);
        }
    }
}

void func_150D20B0(FF0E0Object *obj) {
    func_150D2054(obj);
    effect_defaultFreeTimedEffect(&obj->base);
}

void func_150D20DC(FF0E0Object *obj) {
    func_150D2054(obj);
    effect_defaultDestroyTimedEffect(&obj->base);
}
