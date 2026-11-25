#include <ultra64.h>

#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Spawn packet consumed by func_151407D0 and func_1513D2F0 (copied whole, 0x58 bytes). */
typedef struct {
    /* 0x00 */ u8 field0;
    /* 0x01 */ s8 field1;
    /* 0x02 */ s16 field2;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 field8;
    /* 0x0C */ s32 fieldC;
    /* 0x10 */ u8 colour[4];
    /* 0x14 */ f32 scale[2];
    /* 0x1C */ Vec3f position;
    /* 0x28 */ Vec3f rotation;
    /* 0x34 */ f32 size[3];
    /* 0x40 */ s32 field40;
    /* 0x44 */ u8 field44;
    /* 0x45 */ u8 field45;
    /* 0x46 */ u8 field46;
    /* 0x47 */ u8 field47;
    /* 0x48 */ s32 field48;
    /* 0x4C */ u8 field4C;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ s32 field50;
    /* 0x54 */ s16 field54;
    /* 0x56 */ s16 field56;
} SpawnPacket; /* size 0x58 */

/* Emitter parameters passed to func_151407D0 (it is told 0x7C bytes). */
typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 pad24[0x24];
    /* 0x48 */ f32 scale;
    /* 0x4C */ u8 pad4C[0x8];
    /* 0x54 */ u8 unk54[4];
    /* 0x58 */ u8 unk58;
} EmitterParams;

/* Extra data copied to offset 0x170 of each emitter func_150113E0 spawns. */
typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
} EmitterExtra; /* size 0x1C */

struct181 *func_151407D0(EmitterParams *, s32, SpawnPacket *, u8, u8, u8, u8, s8, u8, s32);

/* Spawns four emitters sharing one setup; only the packet position and rotation change. */
void func_150113E0(void) {
    EmitterParams params;
    SpawnPacket packet;
    EmitterExtra extra;
    struct181 *emitter;

    extra.unk00 = 1.0420000553f;
    extra.unk04 = 0.9010000229f;
    extra.unk08 = 1.2009999752f;
    extra.unk0C = 1.0f;
    extra.unk10 = 0.0f;
    extra.unk14 = 7.0f;
    extra.unk18 = 0.7023000121f;

    packet.field0 = 3;
    packet.field2 = 0x3403;
    packet.lifetime = 300;
    packet.field8 = 0;
    packet.fieldC = 0;
    packet.colour[0] = 230;
    packet.colour[1] = 100;
    packet.colour[2] = 0;
    packet.colour[3] = 255;
    packet.scale[0] = 100.0f;
    packet.scale[1] = 100.0f;
    packet.position.x = 520.0f;
    packet.position.y = 72.0f;
    packet.position.z = 2337.0f;
    packet.rotation.x = 520.0f;
    packet.rotation.y = 72.0f;
    packet.rotation.z = 2220.0f;
    packet.size[0] = 1.0f;
    packet.size[1] = 1.0f;
    packet.size[2] = 1.0f;
    packet.field40 = 0xD2012;
    packet.field45 = 255;
    packet.field44 = 255;
    packet.field46 = 0;
    packet.field47 = 6;

    params.unk04 = 175.5f;
    params.unk00 = 175.5f;
    params.unk0C = 115.4000015259f;
    params.unk08 = 115.4000015259f;
    params.unk10 = 0.4099999964f;
    params.unk14 = 0.2000000030f;
    params.unk18 = 0.3499999940f;
    params.unk1C = 0.3499999940f;
    params.unk20 = 9.999999975e-07f;
    params.unk54[0] = 0;
    params.unk54[1] = 0;
    params.unk54[2] = 0;
    params.unk54[3] = 0;
    params.scale = 1.0f;
    params.unk58 = 0;

    emitter = func_151407D0(&params, 0x7C, &packet, 0, 32, 0, 0, -1, 255, 1);
    if (emitter != NULL) {
        memcpy(&emitter->unk170, &extra, sizeof(extra));
    }

    packet.position.x = -520.0f;
    packet.rotation.x = -520.0f;
    packet.position.y = 72.0f;
    packet.rotation.y = 72.0f;
    packet.position.z = 2337.0f;
    packet.rotation.z = 2220.0f;
    emitter = func_151407D0(&params, 0x7C, &packet, 0, 32, 0, 0, -1, 255, 1);
    if (emitter != NULL) {
        memcpy(&emitter->unk170, &extra, sizeof(extra));
    }

    packet.position.y = 70.0f;
    packet.rotation.y = 70.0f;
    packet.position.z = 926.0f;
    packet.rotation.z = 926.0f;
    packet.position.x = -710.0f;
    packet.rotation.x = -584.0f;
    emitter = func_151407D0(&params, 0x7C, &packet, 0, 32, 0, 0, -1, 255, 1);
    if (emitter != NULL) {
        memcpy(&emitter->unk170, &extra, sizeof(extra));
    }

    packet.position.x = 506.0f;
    packet.rotation.x = 506.0f;
    packet.position.y = 47.0f;
    packet.rotation.y = 47.0f;
    packet.position.z = -339.0f;
    packet.rotation.z = -208.0f;
    emitter = func_151407D0(&params, 0x7C, &packet, 0, 32, 0, 0, -1, 255, 1);
    if (emitter != NULL) {
        memcpy(&emitter->unk170, &extra, sizeof(extra));
    }
}

/* Per-owner data stored in the effect object func_1501175C spawns. */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 id;
    /* 0x08 */ struct MotionEffect *effect;
    /* 0x0C */ struct225 *light;
} Controller; /* size 0x10 */

typedef struct {
    /* 0x00 */ u8 header[0x28];
    /* 0x28 */ Controller controller;
} ControllerEffect;

/* Motion data copied into the effect spawned by func_1513D2F0. */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 id;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ ControllerEffect *controller;
    /* 0x54 */ f32 size;
    /* 0x58 */ u8 unk58;
} Motion; /* size 0x5C */

typedef struct MotionEffect {
    /* 0x000 */ u8 header[0x110];
    /* 0x110 */ Motion motion;
} MotionEffect;

struct225 *func_151602C0(Header *header, Header2 *header2, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7,
                         s32 offset, u8 arg9, s32 argA);

/* Spawns an effect tied to owner, with a moving child effect and a light attached to it. */
void func_1501175C(struct127 *owner, u8 arg1, s32 arg2) {
    ControllerEffect *object;
    Controller data;
    Controller *controller;

    data.owner = owner;
    data.id = owner->unique_id;
    data.effect = NULL;
    data.light = NULL;
    object = (ControllerEffect *)func_15149130(300, -1, 0x54, -1, 0, 0x34, (struct37 *)sizeof(Controller), arg1, arg2);
    if (object != NULL) {
        controller = &object->controller;
        memcpy(controller, &data, sizeof(data));
        // matching: the -g3 scope entry makes IDO load the shared constants at the top of this block
        {
            Motion motion;
            SpawnPacket packet;
            Header2 header2;
            Header header;

            motion.unk58 = 0;
            motion.size = (owner->xz_scale + owner->y_scale) * 0.5f;
            motion.owner = owner;
            motion.id = owner->unique_id;
            motion.unk08 = 15.04527187f;
            motion.unk10 = 5.867655754f;
            motion.unk18 = 12.11144447f;
            motion.unk0C = 0.3961740136f;
            motion.unk24 = 0.0f;
            motion.unk28 = 0.0f;
            motion.unk38 = 0.0f;
            packet.scale[0] = 0.0f;
            packet.scale[1] = 0.0f;
            motion.unk3C = 0.0f;
            motion.unk48 = 1.0f;
            motion.unk40 = 1.0f;
            motion.unk4C = 1.0f;
            motion.unk44 = 1.0f;
            motion.controller = object;
            packet.field0 = 0x5F;
            packet.field1 = 9;
            packet.field2 = 0x2203;
            packet.lifetime = 300;
            packet.field8 = 0;
            packet.fieldC = 0;
            packet.colour[0] = 255;
            packet.colour[1] = 255;
            packet.colour[2] = 255;
            packet.colour[3] = 255;
            motion.unk14 = 0.1042563021f;
            motion.unk20 = 0.5197371244f;
            motion.unk1C = 0.3440458775f;
            motion.unk2C = 160.0f;
            motion.unk30 = 95.0f;
            motion.unk34 = 207.5f;
            packet.position = *(Vec3f *)&D_800A5480;
            packet.rotation = *(Vec3f *)&D_800A5480;
            packet.size[0] = 1.0f;
            packet.size[1] = 1.0f;
            packet.size[2] = 1.0f;
            packet.field40 = 0x40EC0000; // bit pattern of 7.375f
            packet.field44 = 0;
            packet.field45 = 255;
            packet.field46 = 0;
            packet.field47 = 7;
            packet.field48 = 0;
            controller->effect = func_1513D2F0(&packet, (s32)&D_800A4AA0, 0, 0x25, 0, 0x1D,
                                               ((func_150ADA20() & 1) ? 2 : 0) + 1, 0, 0, sizeof(motion), arg1, arg2);
            if (controller->effect != NULL) {
                memcpy(&controller->effect->motion, &motion, sizeof(motion));
            }
            header.unk0 = 2;
            header.unk1 = -1;
            header.unk2 = 300;
            header.unk4 = 0;
            header2.unk0 = 0;
            header2.unk4 = 0;
            header2.unk8 = 0;
            controller->light = func_151602C0(&header, &header2, 40, 255, 255, 255, 255, 1, 0, arg1, arg2);
        }
    }
}

typedef struct {
    /* 0x00 */ u8 header[0x28];
    /* 0x28 */ struct182 data;
} OwnerEffect;

/* Spawns an effect that remembers its owner and the owner's unique id. */
void func_15011A78(struct127 *owner, u8 arg1, s32 arg2) {
    struct182 data;
    OwnerEffect *effect;

    data.unk0 = owner;
    data.unk4 = owner->unique_id;
    data.unk6 = 0;

    effect = (OwnerEffect *)func_15149130(300, -1, 0x55, -1, 0, 0x42, (struct37 *)sizeof(struct182), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}
