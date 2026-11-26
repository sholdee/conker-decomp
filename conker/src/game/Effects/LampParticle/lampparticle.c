#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Data of the swinging-light effect, stored at LightEffect.data by light_spawnLampParticle. */
typedef struct {
    /* 0x0 */ LampParticle *lamp;
    /* 0x4 */ DefParticleHead *beam;      // NULL if func_151407D0 failed
    /* 0x8 */ f32 invScale;    // 1 / D_800AA528[kind]
} SwingingLight;

/* Lamp position, by kind. */
static const Vec3f D_800AA4E0[6] = {
    { 1993.0f, 367.0f, 7294.0f },
    { 1993.0f, 367.0f, 6183.0f },
    { 695.0f, 737.0f, -2.0f },
    { -453.0f, 737.0f, -2.0f },
    { -1860.0f, 737.0f, 1650.0f },
    { -1830.0f, 737.0f, 3017.0f },
};

/* Lamp size, by kind. */
static const f32 D_800AA528[6] = { 1.0f, 1.0f, 2.0f, 2.0f, 2.0f, 2.0f };

/* matching: GlowParams without the animation; it is passed as paramSize, and the frame needs 0x60 bytes. */
typedef struct {
    /* 0x00 */ GlowShape shape;
    /* 0x5C */ u8 pad5C[4];
} GlowParams60;

/* Light callback: spawns a swinging lamp of the given kind with a light effect (gLightEffectUpdates slot 7) that
 * follows it, and a beam shining from it. */
void light_spawnLampParticle(PointLight *light, u8 kind, u8 arg2, s32 arg3) {
    void *obj; // matching: one local holds the lamp, then the effect (two locals shift the stack)
    ParticleSpawn spawn;
    LampSwing swing;

    swing.out = 0.0f;
    swing.targetAmp = swing.amp = 17.31599998f;
    swing.phase = func_150ADA68() * 6.283185482f;
    swing.targetSpeed = swing.speed = 0.08799999952f;
    swing.timer = 0.0f;
    swing.ampRate = 0.0f;
    swing.speedRate = 0.0f;

    spawn.radius = 1.0f;
    spawn.elasticity = 1.0f;
    spawn.sizeXZ = spawn.sizeY = D_800AA528[kind];
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos = D_800AA4E0[kind];
    spawn.vel.x = 0.0f;
    spawn.vel.y = 0.0f;
    spawn.vel.z = 0.0f;
    spawn.rotVel.x = 0.0f;
    spawn.rotVel.y = 0.0f;
    spawn.rotVel.z = 0.0f;
    spawn.gravity = 0.0f;
    spawn.lifetime = 300;
    spawn.model = 0x22;
    collision_selectMap(0);
    spawn.unk5C = collision_findZoneLeaf(spawn.pos.x, spawn.pos.z);
    spawn.flags = (spawn.unk5C ? 0x400 : 0) | 0x100 | 0x800;
    spawn.variant = 0;
    spawn.alpha = 255;
    spawn.updateFn = 9;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 4;
    spawn.unk6A = 2;
    spawn.owner = NULL;
    spawn.ownerId = 0;
    spawn.fadeStart = 1;
    spawn.fadeRate = 255;
    obj = func_15132A4C(&spawn, 3, 255, sizeof(LampSwing), arg2, arg3);
    if (obj != NULL) {
        LightEffectHeader header;

        memcpy(&((LampParticle *)obj)->swing, &swing, sizeof(swing));
        {
            SwingingLight data;
            SwingingLight *state;

            data.lamp = obj;
            data.beam = NULL;
            data.invScale = 1.0f / D_800AA528[kind];
            header.flags = 0;
            header.update = 7;
            header.lifetime = 300;
            header.lightType = 0x12;
            obj = light_createEffect(&header, light, sizeof(data), arg2, arg3);
            if (obj != NULL) {
                state = (SwingingLight *)&((LightEffect *)obj)->data;
                memcpy(state, &data, sizeof(data));
                {
                    GlowParams60 params;
                    ParticleDef desc;

                    params.shape.width1 = params.shape.height1 = D_800AA528[kind] * 43.29999924f;
                    params.shape.width0 = params.shape.height0 = D_800AA528[kind] * 30.20000076f;
                    params.shape.offset1 = 0.200000003f;
                    params.shape.offset0 = 0.25f;
                    params.shape.alpha1 = 0.8000000119f;
                    params.shape.alpha0 = 0.6000000238f;
                    params.shape.fadeRate = 9.999999975e-07f;
                    params.shape.screenX[0] = -1;
                    params.shape.screenY[0] = -1;
                    params.shape.screenX[1] = -1;
                    params.shape.screenY[1] = -1;
                    params.shape.screenX[2] = -1;
                    params.shape.screenY[2] = -1;
                    params.shape.screenX[3] = -1;
                    params.shape.screenY[3] = -1;
                    params.shape.attach = 0;
                    params.shape.scale = 1.0f;
                    params.shape.zbufSample[0] = 0;
                    params.shape.zbufSample[1] = 0;
                    params.shape.zbufSample[2] = 0;
                    params.shape.zbufSample[3] = 0;
                    params.shape.visibility[0] = 0;
                    params.shape.visibility[1] = 0;
                    params.shape.visibility[2] = 0;
                    params.shape.visibility[3] = 0;
                    params.shape.type = 0;
                    params.shape.handler = -1;

                    desc.texture = 0x69;
                    desc.type = 3;
                    desc.renderModes = 0x3403;
                    desc.lifetime = 300;
                    desc.frame = 0;
                    desc.frameSpeed = 0;
                    desc.rgba[0] = 255;
                    desc.rgba[1] = 255;
                    desc.rgba[2] = 255;
                    desc.rgba[3] = 255;
                    desc.width = 100.0f;
                    desc.height = 100.0f;
                    desc.pos = spawn.pos;
                    desc.rot.x = spawn.pos.x;
                    desc.rot.y = spawn.pos.y - 500.0f;
                    desc.rot.z = spawn.pos.z;
                    desc.scale.x = 1.0f;
                    desc.scale.y = 1.0f;
                    desc.scale.z = 1.0f;
                    desc.flags = 0x1CD2006;
                    desc.unk44 = 255;
                    desc.unk45 = 255;
                    desc.unk46 = 0;
                    desc.unk47 = 6;
                    desc.unk48 = spawn.unk5C;
                    desc.viewportMask = 255;
                    state->beam = func_151407D0(&params.shape, sizeof(params), &desc, 0, 0, 0, 0, -1, arg2, arg3);
                }
            }
        }
    }
}

/* Particle update (D_800898B0 slot 9): advances the swing and its amplitude/speed ramps. */
s32 light_updateLampParticle(LampParticle *particle) {
    LampSwing *swing = &particle->swing;

    swing->phase += swing->speed * gFrameDelta;
    swing->phase = func_15144B68(swing->phase);
    swing->out = sinf(swing->phase) * swing->amp;
    if (swing->timer > 0.0f) {
        swing->timer -= gFrameDelta;
        swing->speed += swing->speedRate * gFrameDelta;
        swing->amp += swing->ampRate * gFrameDelta;
    } else {
        swing->speed += (swing->targetSpeed - swing->speed) * 0.0234f;
        swing->amp += (swing->targetAmp - swing->amp) * 0.0234f;
    }
    return 1;
}

/* Matrix callback (D_8008998C slot 4): the particle rolled by its swing angle, scaled by its size and
 * placed at its position. */
s32 light_getLampParticleMatrix(Mtx *dest, LampParticle *particle) {
    MtxF mtx;
    s32 pad[2]; // matching: unused stack space below the matrix

    func_150A8050(mtx, 0.0f, 0.0f, particle->swing.out);

    mtx[3][0] = particle->head.pos.x;
    mtx[3][1] = particle->head.pos.y;
    mtx[3][2] = particle->head.pos.z;

    mtx[0][0] *= particle->head.sizeXZ;
    mtx[0][1] *= particle->head.sizeXZ;
    mtx[0][2] *= particle->head.sizeXZ;

    mtx[1][0] *= particle->head.sizeY;
    mtx[1][1] *= particle->head.sizeY;
    mtx[1][2] *= particle->head.sizeY;

    mtx[2][0] *= particle->head.sizeXZ;
    mtx[2][1] *= particle->head.sizeXZ;
    mtx[2][2] *= particle->head.sizeXZ;

    guMtxF2L(mtx, dest);
    return 1;
}

/* Light effect update (gLightEffectUpdates slot 7): moves the light to a point 150 units below the swinging lamp
 * and aims the beam from the lamp along the swing. */
s32 light_updateLampParticleLight(LightEffect *effect) {
    f32 extend; // matching: the 500.0f literal needs its own local (inline changes the frame)
    LampParticle *lamp;
    Vec3f point;
    MtxF mtx;
    f32 reach;
    f32 rate;
    f32 dx, dy, dz; // unit direction from the lamp down to the light point
    SwingingLight *state;

    state = (SwingingLight *)&effect->data;
    lamp = state->lamp;
    func_150A8050(mtx, 0.0f, 0.0f, lamp->swing.out);
    mtx[3][0] = lamp->head.pos.x;
    mtx[3][1] = lamp->head.pos.y;
    mtx[3][2] = lamp->head.pos.z;
    mtx[0][0] *= lamp->head.sizeXZ;
    mtx[0][1] *= lamp->head.sizeXZ;
    mtx[0][2] *= lamp->head.sizeXZ;
    mtx[1][0] *= lamp->head.sizeY;
    mtx[1][1] *= lamp->head.sizeY;
    mtx[1][2] *= lamp->head.sizeY;
    mtx[2][0] *= lamp->head.sizeXZ;
    mtx[2][1] *= lamp->head.sizeXZ;
    mtx[2][2] *= lamp->head.sizeXZ;
    func_150A7960(mtx, 0.0f, -150.0f, 0.0f, &point.x, &point.y, &point.z);
    effect->light->x = point.x;
    effect->light->y = point.y;
    effect->light->z = point.z;
    if (state->beam != NULL) {
        extend = 500.0f;
        reach = lamp->head.sizeY * 65.0f;
        rate = state->invScale * (-1.0f / 150.0f);
        dx = -(point.x - lamp->head.pos.x) * rate;
        dy = -(point.y - lamp->head.pos.y) * rate;
        dz = -(point.z - lamp->head.pos.z) * rate;
        state->beam->def.pos.x = lamp->head.pos.x + dx * reach;
        state->beam->def.pos.y = lamp->head.pos.y + dy * reach;
        state->beam->def.pos.z = lamp->head.pos.z + dz * reach;
        state->beam->def.rot.x = state->beam->def.pos.x + dx * extend;
        state->beam->def.rot.y = state->beam->def.pos.y + dy * extend;
        state->beam->def.rot.z = state->beam->def.pos.z + dz * extend;
    }
    return 1;
}

/* Event 0x1B handler for model 0x22 (see func_15133588): starts a 5-unit ramp of the swing. */
void func_151B9660(LampParticle *particle) {
    particle->swing.timer = 5.0f;
    particle->swing.ampRate = 9.256800652f;
    particle->swing.speedRate = 0.005399999209f;
}
