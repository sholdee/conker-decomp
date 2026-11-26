/* game_3DF10.c -- func_15010A60.
 *
 * The five float constants are this function's OWN anonymous literal pool, not
 * globals: golden hoists them into callee-saved $f20..$f30, and IDO only hoists
 * LITERALS -- an `extern f32` is a memory object it reloads inside a
 * call-containing loop (measured: extern 330 / frame -0x1A0, literals 0 / -0x1D0;
 * `extern const f32` is identical to extern at 330).
 *
 * The pool is therefore migrated to this TU in conker.us.yaml:
 *     [0x23AF60, .rodata, game_3DF10]
 */
#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// fat struct to decipher

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ Vec3f unk18;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
} Struct3DF10Tail; /* size 0x2C */

extern Vec3f D_80096450[];
extern u8 D_80096498[];

void func_15010A60(void) {
    void *result;
    ParticleSpawn78 part;
    s32 i;
    DynModelDesc desc;
    Struct3DF10Tail tail;
    f32 sp[6];
    f32 rnd;
    DynModel *ent;
    LightEffectHeader header;
    Vec3i header2;
    void *payload;
    LightEffect *snd;
    Vec3f pos;

    func_150E8854();

    desc.updateFunc = 3;
    desc.drawFunc = 6;
    desc.lifetime = 0x12C;
    desc.unk30 = 9;
    desc.flags = 0;
    desc.unk34 = 0x1AF;
    desc.kind = 0;

    part.flags = 0xD00;
    part.lifetime = 0x12C;
    part.model = 0x56;
    part.variant = 0;
    part.alpha = 0xFF;
    part.updateFn = 0x10;
    part.update2Fn = 0;
    part.floorHitFn = 0;
    part.floorType3Fn = 0;
    part.wallHitFn = 0;
    part.destroyFn = 0;
    part.squashFn = 0;
    part.matrixFn = 2;
    part.unk6A = 2;
    part.owner = 0;
    part.ownerId = 0;
    part.fadeStart = 1;
    part.fadeRate = 0xFF;

    part.radius = 1.0f;
    part.elasticity = 1.0f;
    part.sizeY = 1.0f;
    part.sizeXZ = 1.0f;
    part.rot.x = 0.0f;
    part.rot.y = 0.0f;
    part.rot.z = 0.0f;
    part.scale.x = 1.0f;
    part.scale.y = 1.0f;
    part.scale.z = 1.0f;
    part.vel.x = 0.0f;
    part.vel.y = 0.0f;
    part.vel.z = 0.0f;
    part.rotVel.x = 0.0f;
    part.rotVel.y = 0.0f;
    part.rotVel.z = 0.0f;
    part.gravity = 0.0f;
    tail.unk24 = 0.0f;
    tail.unk28 = 0.0f;

    for (i = 0; i < 6; i++) {
        pos = D_80096450[i];
        part.pos = pos;
        tail.unk18 = pos;

        func_1510F800(0);
        part.unk5C = func_1510FD20((s32)part.pos.x, (s32)part.pos.z);

        rnd = (u32)(osGetCount() * func_150ADA20()) & 0xFFFF;
        rnd *= 1.525902189e-05f;
        sp[0] = (rnd + rnd) * 3.141592741f;
        tail.unk00 = sp[0];

        rnd = (u32)(osGetCount() * func_150ADA20()) & 0xFFFF;
        rnd *= 1.525902189e-05f;
        sp[1] = (rnd + rnd) * 3.141592741f;
        tail.unk04 = sp[1];

        rnd = (u32)(osGetCount() * func_150ADA20()) & 0xFFFF;
        rnd *= 1.525902189e-05f;
        sp[4] = rnd * 3.800000191f + 2.0f;
        tail.unk10 = sp[4];

        rnd = (u32)(osGetCount() * func_150ADA20()) & 0xFFFF;
        rnd *= 1.525902189e-05f;
        sp[5] = rnd * 3.800000191f + 2.0f;
        tail.unk14 = sp[5];

        rnd = (u32)(osGetCount() * func_150ADA20()) & 0xFFFF;
        rnd *= 1.525902189e-05f;
        sp[2] = rnd * 0.1100000069f + 0.05000000075f;
        tail.unk08 = sp[2];

        rnd = (u32)(osGetCount() * func_150ADA20()) & 0xFFFF;
        rnd *= 1.525902189e-05f;
        sp[3] = rnd * 0.1100000069f + 0.05000000075f;
        tail.unk0C = sp[3];

        result = func_15132A4C(&part, 3, 0xFF, 0x18, 0xFF, 1);
        if (result != NULL) {
            memcpy((u8 *)result + 0x170, sp, 0x18);
        }

        if (D_80096498[i] != 0) {
            ent = func_1513B5E0(&desc, 0, 0x2C, 0xFF, 1);
            if (ent != NULL) {
                memcpy(DYNMODEL_USERDATA(ent), &tail, sizeof(tail));
            }
            header2.x = (s32)D_80096450[i].x;
            header2.y = (s32)D_80096450[i].y;
            header2.z = (s32)D_80096450[i].z;
            payload = result;
            header.flags = 2;
            header.update = 0x16;
            header.lifetime = 0x12C;
            header.lightType = 0;
            snd = func_151602C0(&header, &header2, 0xC, 0xFF, 0xFF, 0xFF, 0xFF, 0, 4, 0xFF, 1);
            if (snd != NULL) {
                memcpy(&snd->data.object, &payload, sizeof(payload));
            }
        }
    }
}
