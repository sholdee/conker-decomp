#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawn-variant tables, reached through gSpiralVariantLists (VariantList: count, ids, scales). */
const s16 D_800A19F0[3] = { 9, 10, 11 };
const f32 D_800A19F8[3] = { 1.0f, 1.0f, 1.0f };
const s16 D_800A1A04[16] = {
    0x06, 0x07, 0x08, 0x23, 0x24, 0x25, 0x2D, 0x2E, 0x52, 0xA8, 0xA9, 0xAA, 0xAB, 0xBE, 0xBF, 0x95,
};
const f32 D_800A1A24[16] = {
    0.2f, 0.1f, 0.1f, 0.025f, 0.025f, 0.08f, 0.2f, 0.2f,
    0.1f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.35f, 0.2f,
};

/* Spawn variants of the camera emitters (CameraEmitterData.variants); game_127060 uses entry 1. */
VariantList gSpiralVariantLists[2] = {
    { 3, D_800A19F0, D_800A19F8 },
    { 16, D_800A1A04, D_800A1A24 },
};

/* Conditions of the camera emitters (CameraEmitterData.condition). */
static s32 (*D_80088B38[])() = { effect_consumeZoneFlag };

/* Flags of SpiralState. */
#define SPIRAL_FADE_NEAR 1 /* fade out as the camera gets close */
#define SPIRAL_SOUND 2     /* plays a positional sound */
#define SPIRAL_MUTED 4     /* sound no longer updated or restarted */

/* Spawns an effect that spirals from `from` to `to` at `speed` (invSpeed = 1 / speed), playing
 * `sound` near the camera when it is non-zero. */
void particle_spawnSpiral(Vec3f *from, Vec3f *to, u8 id, f32 speed, f32 invSpeed, f32 scale, u8 fadeNear, s32 sound,
                   u8 owner, s32 kind) {
    ParticleSpawn spawn;
    Vec3f dir;
    Vec3f delta;
    f32 length;
    f32 invLength;
    SpiralState spiral;
    SpiralParticle *obj;

    delta.x = to->x - from->x;
    delta.y = to->y - from->y;
    delta.z = to->z - from->z;
    if (func_15145128(&delta, &dir, &length, &invLength) != 0) {
        spiral.flags = (fadeNear ? SPIRAL_FADE_NEAR : 0) | (sound ? SPIRAL_SOUND : 0);
        spiral.timeLeft = length * invSpeed;
        spiral.center = *from;
        spiral.phase = func_150ADA68() * 6.2831855f;
        spiral.phaseRate = func_150ADA68() * 0.2f + 0.08000000566f;
        if (func_150ADA20() & 1) {
            spiral.phaseRate = -spiral.phaseRate;
        }
        spiral.radius = func_150ADA68() * 21.0f + 13.0f;
        {
            f32 angle = func_150ADA68() * 6.2831855f;
            spiral.cosDir = cosf(angle);
            spiral.sinDir = sinf(angle);
        }
        spawn.radius = 1.0f;
        spawn.elasticity = 1.0f;
        spawn.sizeXZ = spawn.sizeY = scale;
        spawn.rot.x = func_150ADA68() * 360.0f;
        spawn.rot.y = func_150ADA68() * 360.0f;
        spawn.rot.z = func_150ADA68() * 360.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.pos = *from;
        spawn.vel.x = dir.x * speed;
        spawn.vel.y = dir.y * speed;
        spawn.vel.z = dir.z * speed;
        spawn.rotVel.x = func_150ADA68() * 21.90300179f + -10.95150089f;
        spawn.rotVel.y = 0.0f;
        spawn.rotVel.z = func_150ADA68() * 21.90300179f + -10.95150089f;
        spawn.gravity = 0.0f;
        spawn.flags = 0x14900;
        spawn.lifetime = 300;
        spawn.model = id;
        spawn.variant = 8;
        spawn.unk5C = 0;
        spawn.alpha = 255;
        spawn.updateFn = 22;
        spawn.update2Fn = 7;
        spawn.floorHitFn = 0;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 2;
        spawn.drawFn = -1;
        spawn.unk6A = 2;
        spawn.owner = 0;
        spawn.ownerId = 0;
        spawn.fadeStart = 1;
        spawn.fadeRate = 255;
        spawn.sound = sound;
        obj = particle_newModelParticleWithFloor(&spawn, 3, 0xFF, 0, sizeof(spiral), owner, kind);
        if (obj != NULL) {
            memcpy(&obj->spiral, &spiral, sizeof(spiral));
        }
    }
}

/* Update: moves the spiral centre along the object's velocity and puts the object on the
 * circle around it; returns 0 once the travel time is up. */
s32 particle_moveSpiral(SpiralParticle *obj) {
    SpiralState *spiral;

    spiral = &obj->spiral;
    spiral->timeLeft -= gFrameDelta;
    if (spiral->timeLeft <= 0.0f) {
        return 0;
    }
    {
        f32 up;
        f32 side;
        func_1514373C(spiral->phase, spiral->radius, &side, &up);
        spiral->phase += spiral->phaseRate * gFrameDelta;
        spiral->center.x += obj->vel.x * gFrameDelta;
        spiral->center.y += obj->vel.y * gFrameDelta;
        spiral->center.z += obj->vel.z * gFrameDelta;
        obj->pos.x = spiral->center.x + side * spiral->cosDir;
        obj->pos.y = spiral->center.y + up;
        obj->pos.z = spiral->center.z - side * spiral->sinDir;
    }
    obj->rot.x += obj->spin.x * gFrameDelta;
    obj->rot.y += obj->spin.y * gFrameDelta;
    obj->rot.z += obj->spin.z * gFrameDelta;
    // matching: the original falls off the end here without setting a return value
}

/* Fades the object out near the camera and keeps its positional sound playing (started
 * within 200 units of the camera), as its SPIRAL_* flags ask; always returns 1. */
s32 particle_updateSpiralFadeSound(SpiralParticle *obj) {
    SpiralState *spiral;
    f32 dist;

    spiral = &obj->spiral;
    if ((spiral->flags & SPIRAL_FADE_NEAR) || (spiral->flags & SPIRAL_SOUND)) {
        Vec3f *camera;
        Vec3f delta;

        camera = func_15144B34(0);
        delta.x = obj->pos.x - camera->x;
        delta.y = obj->pos.y - camera->y;
        delta.z = obj->pos.z - camera->z;
        dist = func_15143E64(&delta);
    }
    if (spiral->flags & SPIRAL_FADE_NEAR) {
        if (dist < 500.0f) {
            obj->alpha = 0;
        } else if (dist < 1200.0f) {
            obj->alpha = (dist - 500.0f) * (1.0 / 700.0) * 255.0;
        } else {
            obj->alpha = 255;
        }
    }
    if ((spiral->flags & SPIRAL_SOUND) && !(spiral->flags & SPIRAL_MUTED)) {
        if (obj->sound >> 16) {
            func_1000F9D4(obj->sound >> 16, obj->pos.x, obj->pos.y, obj->pos.z);
        } else if (dist < 200.0f) {
            obj->sound |= func_10010F88(obj->sound & 0xFFFF, (func_150ADA20() & 0x3FFF) + 0x4000, 0, 0, -1,
                                        obj->pos.x, obj->pos.y, obj->pos.z, 10000, 20000) << 16;
        }
    }
    return 1;
}

/* Event handler: event 0x4E clears unk71 and sets SPIRAL_FADE_NEAR | SPIRAL_MUTED, 0x4F calls
 * effect_destroyObject. */
void particle_onSpiralEvent(SpiralParticle *obj, s32 arg1, u8 event) {
    SpiralState *spiral;
    spiral = &obj->spiral;
    if (event == 0x4E) {
        obj->unk71 = 0;
        spiral->flags |= SPIRAL_FADE_NEAR | SPIRAL_MUTED;
    } else if (event == 0x4F) {
        effect_destroyObject(&obj->header);
    }
}

/* Starts an emitter that spawns spirals around `camera` (see particle_updateCameraSpiralEmitter). */
void particle_startCameraSpiralEmitter(Camera *camera, s8 condition, u8 variants, u8 owner, s32 kind) {
    CameraEmitterData data;
    CameraEmitter *emitter;

    data.camera = camera;
    data.pending = 0.0f;
    data.condition = condition;
    data.variants = variants;

    emitter = effect_newTimedEffect(300, -1, 0x56, -1, 0, 0, sizeof(data), owner, kind);
    if (emitter != NULL) {
        memcpy(&emitter->data, &data, sizeof(data));
    }
}

/* Emitter update: while its condition callback (if any) holds, spawns 0.05-0.20 spirals per frame
 * (times gFrameDelta) around the camera, each between two random points on either side of
 * its view direction. */
void particle_updateCameraSpiralEmitter(CameraEmitter *emitter) {
    CameraEmitterData *data;
    Vec3f *eye;
    s32 yaw;
    Vec3f from;
    Vec3f to;
    VariantList *variants;
    u8 i;
    f32 speed;

    data = &emitter->data;
    if (data->condition != -1 && !D_80088B38[data->condition]()) {
        return;
    }
    data->pending += (0.05100000277f + func_150ADA68() * 0.153000012f) * gFrameDelta;
    if (data->pending > 1.0f) {
        eye = func_15144B34(data->camera->CNumber);
        /* degrees to 256-step binary angle */
        yaw = (data->camera->TrueCameraAngle + 180.0f) * 0.7111111283f;
        do {
            func_15143874(yaw + 21, func_150ADA68() * 381.0f + 254.0f, &from.x, &from.z);
            from.y = func_150ADA68() * 212.0f + -133.0f;
            from.x += eye->x;
            from.y += eye->y;
            from.z += eye->z;
            func_15143874(yaw - 21, func_150ADA68() * 381.0f + 254.0f, &to.x, &to.z);
            to.y = func_150ADA68() * 212.0f + -133.0f;
            to.x += eye->x;
            to.y += eye->y;
            to.z += eye->z;
            if (func_150ADA20() & 1) {
                Vec3f tmp;

                tmp = from;
                from = to;
                to = tmp;
            }
            variants = &gSpiralVariantLists[data->variants];
            i = func_150ADA20() % variants->count;
            speed = func_150ADA68() * 10.0f + 5.0f;
            particle_spawnSpiral(&from, &to, variants->ids[i], speed, 1.0f / speed,
                          variants->scales[i] * (func_150ADA68() * 0.07500000298f + 0.07500000298f), FALSE, 0,
                          emitter->base.header.owner, emitter->base.header.kind);
            data->pending -= 1.0f;
        } while (data->pending > 1.0f);
    }
}
