#include <ultra64.h>

#include "functions.h"
#include "variables.h"



/** Resets the light state: the global light colour, the colour tables and the point lists. */
void light_reset(void) {
    s32 i;

    gPointLights = NULL;
    gLightingEnabled = 1;
    gAmbientZones = NULL;
    gDirectionalZones = NULL;

    light_setAmbient(255, 255, 255, 0);

    gDirLightColour[0] = 0;
    gDirLightColour[1] = 0;
    gDirLightColour[2] = 0;

    gDirLightAmbientTint[0] = 255;
    gDirLightAmbientTint[1] = 255;
    gDirLightAmbientTint[2] = 255;

    for (i = 0; i < 4; i++) {
        gPlayerLightDirs[i][0] = 127;
        gPlayerLightDirs[i][1] = 0;
        gPlayerLightDirs[i][2] = 0;
    }
    gDirLightAmount = 0;
    gDirLightAmbientAmount = 0;
    bzero(&gPadLight, 56);
}

/** Applies every level point (see light_applyZone). */
void light_applyZones(void) {
    u32 i;

    for (i = 0; i < gZoneCount; i++) {
        light_applyZone(i);
    }
}

/**
 * Applies level point `index` by type: 10 sets the global light colour, 9 spawns a light (and runs its
 * callback), 16 sets the colour tables, 15 and 24 link the point into the gAmbientZones / gDirectionalZones lists.
 */
void light_applyZone(s32 index) {
    s32 pad; // matching: unused stack slot keeps the original frame layout
    s32 red;
    s32 green;
    s32 blue;
    s32 x;
    s32 y;
    s32 z;
    s32 settings;
    s32 strength;
    s32 enabled;
    s32 duration;
    PointLight *light;
    s32 callback;
    ZoneLink *link;
    s32 type;

    type = ZONE_TYPE(&zones[index]);
    switch (type) {
        case ZONE_TYPE_LIGHT_SETUP:
        case ZONE_TYPE_LIGHT_CONTROL:
        case ZONE_TYPE_LIGHT_DIRECTIONAL:
            red = zones[index].data1 & 0xFF;
            green = (zones[index].data1 >> 8) & 0xFF;
            blue = (zones[index].data1 >> 16) & 0xFF;
            zones[index].flags |= 4;
            switch (type) {
                case ZONE_TYPE_LIGHT_CONTROL:
                    light_setAmbient(red, green, blue, 0);
                    if (zones[index].data3 != 0) {
                        light_runAmbientAction(zones[index].data3);
                    }
                    break;
                case ZONE_TYPE_LIGHT_SETUP:
                    x = zones[index].x;
                    y = zones[index].y;
                    z = zones[index].z;
                    settings = (zones[index].data2 >> 8) & 0xFF;
                    strength = (zones[index].data1 >> 24) & 0x7F;
                    enabled = (zones[index].data1 >> 31) & 1;
                    callback = zones[index].data3 & 0xFFFF;
                    duration = (zones[index].data3 >> 16) & 0xFFFF;
                    light = func_1515D5AC(x, y, z, settings, red, green, blue, strength, enabled, duration);
                    if (light != 0 && callback != 0) {
                        light_runCallback(light, callback);
                    }
                    break;
                case ZONE_TYPE_LIGHT_DIRECTIONAL:
                    gDirLightColour[0] = red;
                    gDirLightColour[1] = green;
                    gDirLightColour[2] = blue;
                    gDirLightAmbientTint[0] = zones[index].data2 & 0xFF;
                    gDirLightAmbientTint[1] = (zones[index].data2 >> 8) & 0xFF;
                    gDirLightAmbientTint[2] = (zones[index].data2 >> 16) & 0xFF;
                    gDirLightAmount = (zones[index].data2 >> 24) & 0xFF;
                    gDirLightAmbientAmount = (zones[index].data1 >> 24) & 0xFF;
                    break;
            }
            break;
        case ZONE_TYPE_LIGHT_AMBIENT_LOCAL:
            link = allocate_memory(sizeof(ZoneLink), 1, 0, 0);
            link->zone = &zones[index];
            link->next = gAmbientZones;
            gAmbientZones = link;
        unused_label: // matching: an unused label keeps IDO from threading this break to the epilogue
            break;
        case ZONE_TYPE_LIGHT_DIRECTIONAL_LOCAL:
            link = allocate_memory(sizeof(ZoneLink), 1, 0, 0);
            link->zone = &zones[index];
            link->next = gDirectionalZones;
            gDirectionalZones = link;
            break;
    }
}

/*
 * Light callbacks, the gLightCallbacks table run by light_runCallback: each attaches an effect to `light`.
 * `id` is the callback's own index in the table; some use it to pick an entry of a small local table.
 */

void func_150095D8(PointLight *light, s32 id) {
    func_1516127C(light, 255, 1);
}

void func_15009600(PointLight *light, s32 id) {
    func_15161334(light, 255, 1);
}

void func_15009628(PointLight *light, s32 id) {
    LightEffectHeader header;
    u8 kinds[4] = { 1, 2, 3, 4 };

    header.flags = 0;
    header.update = -1;
    header.lifetime = 300;
    header.lightType = kinds[id - 3];

    light_createEffect(&header, light, 0, 255, 1);
}

void func_1500969C(PointLight *light, s32 id) {
    light_createMounted(light, 255, 1);
}

void func_150096C4(PointLight *light, s32 id) {
    LightEffectHeader header;
    u8 kinds[5] = { 10, 9, 12, 11, 13 };

    header.flags = 0;
    header.update = -1;
    header.lifetime = 300;
    header.lightType = kinds[id - 8];

    light_createEffect(&header, light, 0, 255, 1);
}

void func_15009740(PointLight *light, s32 id) {
    func_15161494(light, 255, 1);
}

void light_cbFlicker0(PointLight *light, s32 id) {
    light_createFlicker(light, 0, 0, 14, 255, 1);
}

void func_150097A4(PointLight *light, s32 id) {
    func_15161540(light, 255, 1);
}

void func_150097CC(PointLight *light, s32 id) {
    LightEffect *effect = light_createFlicker(light, 0, 2, 14, 255, 1);

    if (effect != NULL) {
        func_150121C0(effect);
    }
}

void light_cbLampParticle0(PointLight *light, s32 id) {
    light_spawnLampParticle(light, 0, 255, 1);
}

void light_cbLampParticle1(PointLight *light, s32 id) {
    light_spawnLampParticle(light, 1, 255, 1);
}

void light_cbSwingingLamps(PointLight *light, s32 id) {
    light_spawnSwingingLamps(light);
}

void light_cbFlickerSound0(PointLight *light, s32 id) {
    light_createFlicker(light, 1, 0, 14, 255, 1);
}

void func_150098D0(PointLight *light, s32 id) {
    func_15162034(light, 255, 1);
}

void func_150098F8(PointLight *light, s32 id) {
    light_createColourCycle(light, 1, 22, 0, -1, 300, 255, 1);
}

void light_cbJitter1(PointLight *light, s32 id) {
    light_createJitter(light, 1, 6, 0, 300, 2, 255, 1);
}

/* Per-light parameters of light_cbObjectCycle and func_15009A38, indexed by the light id. */
static const u8 sCycleArg1[4] = { 1, 2, 3, 4 };
static const u8 sCycleArg2[4] = { 23, 24, 25, 26 };
static const u8 sCycleObjectIds[4] = { 0xF6, 0xF3, 0xED, 0xF0 };
static const u8 sLightTypes[4] = { 27, 28, 29, 30 };

void light_cbObjectCycle(PointLight *light, s32 id) {
    LightEffect *effect;
    u8 extra[1];

    extra[0] = sCycleObjectIds[id - 24];
    effect = light_createCycle(light, sCycleArg1[id - 24], sCycleArg2[id - 24], 2, 300, 12, -1, 0, -1, 1, 1, 255, 1);
    if (effect != NULL) {
        memcpy(&effect->data.objectCycle.objectId, extra, 1);
    }
}

void func_15009A38(PointLight *light, s32 id) {
    LightEffectHeader header;

    header.flags = 2;
    header.update = -1;
    header.lifetime = 300;
    header.lightType = sLightTypes[id - 69];
    light_createEffect(&header, light, 0, 255, 1);
}

void light_cbLightning1(PointLight *light, s32 id) {
    light_createLightning(light, 1, 0, 2, 300, 13, 255, 1);
}

void light_cbJitter2(PointLight *light, s32 id) {
    light_createJitter(light, 2, 6, 0, 300, 2, 255, 1);
}

void light_cbJitter20(PointLight *light, s32 id) {
    light_createJitter(light, 20, 6, 0, 300, 2, 255, 1);
}

void light_cbJitter3(PointLight *light, s32 id) {
    light_createJitter(light, 3, 6, 0, 300, 2, 255, 1);
}

void func_15009BD0(PointLight *light, s32 id) {
    LightEffect *effect;
    LightFollow params;

    params.base = 0.0f;
    params.scale = 90.0f;
    params.source = effect_newColorAnim(0, 0.0f, 0, 0, 0, 220, 220, 255);
    effect = light_createSizedEffect(light, 2, 16, 300, 0, 12, 255, 1);
    if (effect != NULL) {
        memcpy(&effect->data.follow, &params, sizeof(params));
    }
}

void func_15009C7C(PointLight *light, s32 id) {
    LightEffect *effect;
    LightFollow params;

    params.base = 0.0f;
    params.scale = 50.0f;
    params.source = effect_newColorAnim(0, 0.0f, 0, 0, 0, 220, 220, 255);
    effect = light_createSizedEffect(light, 2, 16, 300, 0, 12, 255, 1);
    if (effect != NULL) {
        memcpy(&effect->data.follow, &params, sizeof(params));
    }
}

void light_cbPulse1(PointLight *light, s32 id) {
    light_createPulse(light, 1, 5, 300, 0, 0, 255, 1);
}

void light_cbPulse2(PointLight *light, s32 id) {
    light_createPulse(light, 2, 5, 300, 0, 0, 255, 1);
}

void light_cbJitter4(PointLight *light, s32 id) {
    light_createJitter(light, 4, 6, 0, 300, 2, 255, 1);
}

void light_cbJitter5(PointLight *light, s32 id) {
    light_createJitter(light, 5, 6, 0, 300, 2, 255, 1);
}

void light_cbFlickerSound1(PointLight *light, s32 id) {
    light_createFlicker(light, 1, 1, 14, 255, 1);
}

void light_cbPulse3(PointLight *light, s32 id) {
    light_createPulse(light, 3, 5, 300, 0, 0, 255, 1);
}

void func_15009EC8(PointLight *light, s32 id) {
    light_createEmptyEffect(light, 32, 255, 1);
}

void light_cbFlickerSound3(PointLight *light, s32 id) {
    light_createFlicker(light, 1, 3, 35, 255, 1);
}

void light_cbPulse4(PointLight *light, s32 id) {
    light_createPulse(light, 4, 36, 300, 0, 0, 255, 1);
}

void func_15009F74(PointLight *light, s32 id) {
    LightEffect *effect;
    LightFollow params;

    params.base = 0.0f;
    params.scale = 60.0f;
    params.source = effect_newColorAnim(1, 0.1f, 100, 100, 100, 255, 100, 100);
    effect = light_createSizedEffect(light, 2, 16, 300, 0, 12, 255, 1);
    if (effect != NULL) {
        memcpy(&effect->data.follow, &params, sizeof(params));
    }
}

void light_cbPulse5(PointLight *light, s32 id) {
    light_createPulse(light, 5, 5, 300, 0, 0, 255, 1);
}

void light_cbPulse6(PointLight *light, s32 id) {
    light_createPulse(light, 6, 5, 300, 0, 0, 255, 1);
}

void light_cbJitter7(PointLight *light, s32 id) {
    light_createJitter(light, 7, 6, 0, 300, 2, 255, 1);
}

void light_cbJitter9(PointLight *light, s32 id) {
    light_createJitter(light, 9, 6, 0, 300, 2, 255, 1);
}

void light_cbJitter10(PointLight *light, s32 id) {
    light_createJitter(light, 10, 6, 0, 300, 2, 255, 1);
}

void light_cbJitter11(PointLight *light, s32 id) {
    light_createJitter(light, 11, 38, 0, 300, 2, 255, 1);
}

void light_cbFlickerSound4(PointLight *light, s32 id) {
    light_createFlicker(light, 1, 4, 14, 255, 1);
}

void light_cbPulse7(PointLight *light, s32 id) {
    light_createPulse(light, 7, 5, 300, 0, 0, 255, 1);
}

void light_cbLightning2(PointLight *light, s32 id) {
    light_createLightning(light, 2, 0, 2, 300, 13, 255, 1);
}

void light_cbLampParticleById(PointLight *light, s32 id) {
    const u8 kinds[4] = { 2, 3, 4, 5 };

    light_spawnLampParticle(light, kinds[id - 51], 255, 1);
}

void light_cbFlashById(PointLight *light, s32 id) {
    const u8 kinds[2] = { 0, 1 };

    light_createFlash(light, kinds[id - 56], 255, 1);
}

void light_cbPulse8(PointLight *light, s32 id) {
    light_createPulse(light, 8, 5, 300, 0, 0, 255, 1);
}

void light_cbPulse9(PointLight *light, s32 id) {
    light_createPulse(light, 9, 40, 300, 0, 0, 255, 1);
}

void light_cbJitter15(PointLight *light, s32 id) {
    light_createJitter(light, 15, 6, 0, 300, 2, 255, 1);
}

void light_cbPulse10(PointLight *light, s32 id) {
    light_createPulse(light, 10, 5, 300, 0, 0, 255, 1);
}

void light_cbFlicker5(PointLight *light, s32 id) {
    light_createFlicker(light, 0, 5, 14, 255, 1);
}

void light_cbPulse11(PointLight *light, s32 id) {
    light_createPulse(light, 11, 5, 300, 0, 0, 255, 1);
}

void light_cbPulse12(PointLight *light, s32 id) {
    light_createPulse(light, 12, 41, 300, 0, 0, 255, 1);
}

void light_cbPulse13(PointLight *light, s32 id) {
    light_createPulse(light, 13, 41, 300, 0, 0, 255, 1);
}

void func_1500A55C(PointLight *light, s32 id) {
    light_createColourCycle(light, 1, 22, 0, -1, 300, 255, 1);
}

void light_cbJitter17(PointLight *light, s32 id) {
    light_createJitter(light, 17, 6, 0, 300, 2, 255, 1);
}

void light_cbJitter18(PointLight *light, s32 id) {
    light_createJitter(light, 18, 6, 0, 300, 2, 255, 1);
}

void light_cbJitter19(PointLight *light, s32 id) {
    light_createJitter(light, 19, 6, 0, 300, 2, 255, 1);
}

void light_cbJitter21(PointLight *light, s32 id) {
    light_createJitter(light, 21, 6, 0, 300, 2, 255, 1);
}

/** Attaches the light to the level object picked by `id` (and by whether level is 6). */
void func_1500A6D8(PointLight *light, s32 id) {
    LightEffectHeader header;
    LightSwing attachment;
    s32 variant;
    u8 objects[2][3] = { { 7, 8, 9 }, { 8, 9, 10 } };
    LightEffect *effect;

    variant = (level == 6) ? 1 : 0;
    if (func_150B060C(objects[variant][id - 73], &attachment, id) != 0) {
        header.flags = 0;
        header.update = 23;
        header.lifetime = 300;
        header.lightType = 0;
        effect = light_createEffect(&header, light, sizeof(attachment), 255, 1);
        if (effect != NULL) {
            memcpy(&effect->data.swing, &attachment, sizeof(attachment));
        }
    }
}

void light_cbJitter22(PointLight *light, s32 id) {
    light_createJitter(light, 22, 6, 0, 300, 2, 255, 1);
}

/** Attaches the light to the object gfx_findObjectById returns for `id`, starting at its position. */
void light_cbSwingOnObject(PointLight *light, s32 id) {
    LightEffectHeader header;
    LightSwing attachment;
    LightEffect *effect;
    u8 objects[6] = { 2, 3, 4, 5, 6, 7 };

    attachment.object = gfx_findObjectById(objects[id - 80]);
    if (attachment.object != NULL) {
        attachment.offset = -400.0f;
        attachment.scale = 4.0f;
        attachment.pivot.x = attachment.object->x;
        attachment.pivot.y = attachment.object->y;
        attachment.pivot.z = attachment.object->z;
        header.flags = 0;
        header.update = 23;
        header.lifetime = 300;
        header.lightType = 0;
        effect = light_createEffect(&header, light, sizeof(attachment), 255, 1);
        if (effect != NULL) {
            memcpy(&effect->data.swing, &attachment, sizeof(attachment));
        }
    }
}

void light_cbFlicker6(PointLight *light, s32 id) {
    light_createFlicker(light, 0, 6, 14, 255, 1);
}

void light_cbFlickerSoundById(PointLight *light, s32 id) {
    light_createFlicker(light, 1, id - 80, 14, 255, 1);
}

/* The light callbacks by id (light_runCallback). */
static void (*gLightCallbacks[89])(PointLight *light, s32 id) = {
    NULL, func_150095D8, func_15009600, func_15009628,
    func_15009628, func_15009628, func_15009628, func_1500969C,
    func_150096C4, func_150096C4, func_150096C4, func_150096C4,
    func_150096C4, func_15009740, light_cbFlicker0, func_150097A4,
    func_150097CC, light_cbLampParticle0, light_cbLampParticle1, light_cbSwingingLamps,
    light_cbFlickerSound0, func_150098D0, func_150098F8, light_cbJitter1,
    light_cbObjectCycle, light_cbObjectCycle, light_cbObjectCycle, light_cbObjectCycle,
    light_cbLightning1, light_cbJitter2, light_cbJitter3, func_15009BD0,
    func_15009C7C, light_cbPulse1, light_cbPulse2, light_cbJitter4,
    light_cbJitter5, light_cbFlickerSound1, light_cbPulse3, func_15009EC8,
    light_cbFlickerSound3, light_cbPulse4, func_15009F74, light_cbPulse5,
    light_cbPulse6, light_cbJitter7, light_cbJitter9, light_cbJitter10,
    light_cbJitter11, light_cbFlickerSound4, light_cbPulse7, light_cbLampParticleById,
    light_cbLampParticleById, light_cbLampParticleById, light_cbLampParticleById, light_cbLightning2,
    light_cbFlashById, light_cbFlashById, light_cbPulse8, light_cbPulse9,
    light_cbJitter15, light_cbPulse10, light_cbFlicker5, light_cbPulse11,
    light_cbPulse12, func_1500A55C, light_cbJitter17, light_cbJitter18,
    light_cbJitter19, func_15009A38, func_15009A38, func_15009A38,
    func_15009A38, func_1500A6D8, func_1500A6D8, func_1500A6D8,
    light_cbJitter20, light_cbPulse13, light_cbJitter21, light_cbJitter22,
    light_cbSwingOnObject, light_cbSwingOnObject, light_cbSwingOnObject, light_cbSwingOnObject,
    light_cbSwingOnObject, light_cbSwingOnObject, light_cbFlicker6, light_cbFlickerSoundById,
    light_cbFlickerSoundById,
};

/** Runs light callback `id` (1..88) from the gLightCallbacks table on a freshly placed light. */
void light_runCallback(PointLight *light, s32 id) {
    if (id > 0 && id < 89) {
        if (gLightCallbacks[id] != NULL) {
            gLightCallbacks[id](light, id);
        }
    }
}

/** Payload (at +0x28) of the effect_newTimedEffect effect spawned by light_startColourWave. */
typedef struct Game36680ParticleEffect {
    TimedEffect base;
    f32 params[12];
} Game36680ParticleEffect;

/** Ambient action 2: spawns a particle effect with three random angles. */
void light_startColourWave(s32 unused) {
    f32 params[12];
    f32 fraction;
    Game36680ParticleEffect *effect;

    params[0] = 35.0f;
    params[1] = 35.0f;
    params[2] = 60.0f;
    params[3] = 34.0f;
    params[4] = 30.0f;
    params[5] = 50.0f;

    fraction = (osGetCount() * math_random()) & 0xFFFF;
    fraction *= 1.5259022e-05f; // 1/65535
    params[6] = (fraction + fraction) * 3.1415927f;

    fraction = (osGetCount() * math_random()) & 0xFFFF;
    fraction *= 1.5259022e-05f;
    params[7] = (fraction + fraction) * 3.1415927f;

    fraction = (osGetCount() * math_random()) & 0xFFFF;
    fraction *= 1.5259022e-05f;

    params[8] = (fraction + fraction) * 3.1415927f;
    params[9] = -0.1370000094f;
    params[10] = 0.048f;
    params[11] = 0.007f;

    effect = effect_newTimedEffect(300, -1, 45, -1, 0, 0, sizeof(params), 255, 1);
    if (effect != NULL) {
        memcpy(effect->params, params, sizeof(params));
    }
}

/* The ambient actions by number (light_runAmbientAction). */
static void (*gAmbientActions[6])(s32 action) = {
    NULL, light_startColourCycle, light_startColourWave, level_startAmbientRedDrift, level_startAmbientColourCycle, level_startAmbientPulse,
};

/** Runs ambient action `action` (1..5) from the gAmbientActions table. */
void light_runAmbientAction(s32 action) {
    if (action > 0 && action < 6) {
        if (gAmbientActions[action] != NULL) {
            gAmbientActions[action](action);
        }
    }
}
