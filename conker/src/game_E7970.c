#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_8009FE64;
void func_15143E94(s32 arg0, s32 arg1);
void func_15165F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_1514C678(f32 arg0, f32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, f32 arg9, s32 argA, s32 argB);

/* Per-effect data: the object the effect is attached to and a spawn accumulator. */
typedef struct {
    /* 0x0 */ struct127 *owner;
    /* 0x4 */ u8 ownerId; // owner's unique_id when the effect was created
    /* 0x8 */ f32 spawnAccum;
} EffectE7970Data; // size 0xC

/* Effect slot as returned by func_15149130 (a struct260); the effect data lives at 0x28. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ EffectE7970Data data;
} EffectE7970;

/* Spawns an effect attached to `owner`. */
void func_150BA4C0(struct127 *owner, u8 arg1, s32 arg2) {
    EffectE7970Data data;
    struct260 *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.spawnAccum = 0.0f;

    effect = func_15149130((func_150ADA20() % 9U) + 15, -1, 0x52, -1, 1, 0x3F, (struct37 *)0xC, arg1, arg2);
    if (effect != NULL) {
        memcpy(&((EffectE7970 *)effect)->data, &data, sizeof(data));
    }
}

void func_15143794(s16, s16, f32, f32 *);
void func_1515C244(struct127 *, f32 *, f32 *, f32 *);
void func_1518A3C0(f32 *arg0, f32 *arg1, f32 arg2, f32 *arg3, f32 *arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC);

/* Per-frame update: kills the effect once its owner is gone, otherwise emits particles around the owner at a random rate. */
void func_150BA55C(EffectE7970 *effect) {
    struct127 *owner;
    EffectE7970Data *data;
    s32 heading;
    f32 ownerPos[3];
    f32 radius;
    f32 unused;
    f32 growth[3];
    f32 size;
    f32 spawnPos[3];
    f32 angles[3];
    f32 velocity[3];
    f32 speed;
    u32 randomYaw;

    owner = effect->data.owner;
    data = &effect->data;
    if ((owner->interaction_state == 0) || (owner->unique_id != data->ownerId)) {
        effect->unkE = -1;
        return;
    }
    if (owner->unk1D4 != NULL) {
        data->spawnAccum += (0.3143000007f + (func_150ADA68() * 0.58100003f)) * D_800BE9A4;
        if (data->spawnAccum > 1.0f) {
            size = (owner->xz_scale + owner->y_scale) * 0.5f;
            heading = (owner->unk76 >> 8) - 64;
            func_1515C244(owner, ownerPos, &radius, &unused);
            growth[1] = 0.0f;
            do {
                randomYaw = func_150ADA20();
                func_15143794((randomYaw % 127U) + heading - 63, (func_150ADA20() % 31U) - 50, radius, spawnPos);
                spawnPos[1] *= owner->unkF0;
                spawnPos[0] += ownerPos[0];
                spawnPos[1] += ownerPos[1];
                spawnPos[2] += ownerPos[2];
                angles[0] = func_150ADA68() * 360.0f;
                angles[1] = func_150ADA68() * 360.0f;
                angles[2] = func_150ADA68() * 360.0f;
                speed = (func_150ADA68() * 0.09400000423f) + 0.05100000277f;
                velocity[0] = (spawnPos[0] - ownerPos[0]) * speed;
                velocity[1] = (spawnPos[1] - ownerPos[1]) * speed;
                velocity[2] = (spawnPos[2] - ownerPos[2]) * speed;
                growth[0] = (func_150ADA68() * 18.64500046f) + -9.322500229f;
                growth[2] = (func_150ADA68() * 18.64500046f) + -9.322500229f;
                func_1518A3C0(spawnPos, angles, ((func_150ADA68() * 0.08000000566f) + 0.1000000015f) * size, velocity, growth,
                              (func_150ADA68() * 0.324000001f) + -0.5630000234f, 0.9448540211f, 7, (func_150ADA20() % 41U) + 60, 0, 0,
                              effect->unkC, effect->unk1);
                data->spawnAccum -= 1.0f;
            } while (data->spawnAccum > 1.0f);
        }
    }
}

void func_150BA8F0(EffectE7970 *effect, s32 arg1, u8 arg2) {
    func_15149514(arg1, arg2, (s32)&effect->data.owner, (s32)&effect->data.ownerId, (s32)effect);
}

extern f32 D_8009FE60;
void func_1504715C(s32 *arg0);
void func_15046C80(struct17 *arg0, s32 arg1, f32 arg2, s32 *arg3);

s32 func_150BA930(struct17 *arg0, struct127 *arg1, s32 *arg2, s32 arg3) {
    struct17 sp24;

    arg0->unk0 = arg1->x_position;
    if (D_8009FE60 < arg1->unk180) {
        arg0->unk4 = arg1->unk180;
    } else {
        arg0->unk4 = arg1->y_position;
    }
    arg0->unk8 = arg1->z_position;
    if (arg2 == 0) {
        return 1;
    }
    sp24.unk0 = arg0->unk0;
    sp24.unk4 = arg0->unk4 + 100.0f;
    sp24.unk8 = arg0->unk8;
    func_1504715C(arg2);
    func_15046C80(&sp24, 0, arg0->unk4 - 100.0f, arg2);
}

s32 func_150BAA00(s32 arg0, s32 arg1) {
    return 0x9;
}

void func_150BAA14(struct127 *arg0, s32 arg1, s32 arg2) {
    struct17 sp6C;
    s32 sp48[9];
    s32 sp44;
    u8 sp43;

    if (arg0->unk1D4 == 0) {
        return;
    }

    sp43 = func_150BA930(&sp6C, arg0, sp48, (u8)arg1);
    func_151D5404(&sp6C, 1307.0f, 2000.0f, 0.0005f, 0xC, 0xF, 0xFF, 0);
    func_15143E94(5, 0x4022);

    if (sp43 != 0) {
        sp44 = (s32)(D_800DBFF0[D_80082FA4].unk380 * D_8009FE64);
        func_15165F80(-1, (s32)sp6C.unk0, (s32)(sp6C.unk4 + 6.0f), (s32)sp6C.unk8,
                      0x19, 0x12, 0, 0xFF, 1);
        *((f32 *)&sp44 - 3) = func_150ADA68();
        func_1514C678(sp6C.unk0, sp6C.unk4, *(s32 *)&sp6C.unk8,
                      (*((f32 *)&sp44 - 3) * 50.0f) + 40.0f, sp44 + 0x3C, sp44 - 0x3C,
                      (func_150ADA20() % 11U) + 0x1E, 5, 0, 0.0f, 0, 0xFF);
    }
}
