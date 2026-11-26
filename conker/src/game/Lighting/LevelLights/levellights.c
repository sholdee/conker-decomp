#include <ultra64.h>

#include "functions.h"
#include "variables.h"



/** Resets the light state: the global light colour, the colour tables and the point lists. */
void light_reset(void) {
    s32 i;

    gPointLights = NULL;
    D_800DCD7C = 1;
    gAmbientZones = NULL;
    gDirectionalZones = NULL;

    light_setAmbient(255, 255, 255, 0);

    D_800DCD24[0] = 0;
    D_800DCD24[1] = 0;
    D_800DCD24[2] = 0;

    D_800DCD28[0] = 255;
    D_800DCD28[1] = 255;
    D_800DCD28[2] = 255;

    for (i = 0; i < 4; i++) {
        gPlayerLightDirs[i][0] = 127;
        gPlayerLightDirs[i][1] = 0;
        gPlayerLightDirs[i][2] = 0;
    }
    D_800DCD3C = 0;
    D_800DCD3D = 0;
    bzero(&D_800DCD40, 56);
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
                    D_800DCD24[0] = red;
                    D_800DCD24[1] = green;
                    D_800DCD24[2] = blue;
                    D_800DCD28[0] = zones[index].data2 & 0xFF;
                    D_800DCD28[1] = (zones[index].data2 >> 8) & 0xFF;
                    D_800DCD28[2] = (zones[index].data2 >> 16) & 0xFF;
                    D_800DCD3C = (zones[index].data2 >> 24) & 0xFF;
                    D_800DCD3D = (zones[index].data1 >> 24) & 0xFF;
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
    func_15161408(light, 255, 1);
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

void func_15009768(PointLight *light, s32 id) {
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

void func_15009818(PointLight *light, s32 id) {
    light_spawnLampParticle(light, 0, 255, 1);
}

void func_15009844(PointLight *light, s32 id) {
    light_spawnLampParticle(light, 1, 255, 1);
}

void func_15009870(PointLight *light, s32 id) {
    light_spawnSwingingLamps(light);
}

void func_15009894(PointLight *light, s32 id) {
    light_createFlicker(light, 1, 0, 14, 255, 1);
}

void func_150098D0(PointLight *light, s32 id) {
    func_15162034(light, 255, 1);
}

void func_150098F8(PointLight *light, s32 id) {
    light_createColourCycle(light, 1, 22, 0, -1, 300, 255, 1);
}

void func_15009944(PointLight *light, s32 id) {
    light_createJitter(light, 1, 6, 0, 300, 2, 255, 1);
}

/* Per-light parameters of func_15009990 and func_15009A38, indexed by the light id. */
static const u8 sCycleArg1[4] = { 1, 2, 3, 4 };
static const u8 sCycleArg2[4] = { 23, 24, 25, 26 };
static const u8 sCycleObjectIds[4] = { 0xF6, 0xF3, 0xED, 0xF0 };
static const u8 sLightTypes[4] = { 27, 28, 29, 30 };

void func_15009990(PointLight *light, s32 id) {
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

void func_15009AA0(PointLight *light, s32 id) {
    light_createLightning(light, 1, 0, 2, 300, 13, 255, 1);
}

void func_15009AEC(PointLight *light, s32 id) {
    light_createJitter(light, 2, 6, 0, 300, 2, 255, 1);
}

void func_15009B38(PointLight *light, s32 id) {
    light_createJitter(light, 20, 6, 0, 300, 2, 255, 1);
}

void func_15009B84(PointLight *light, s32 id) {
    light_createJitter(light, 3, 6, 0, 300, 2, 255, 1);
}

void func_15009BD0(PointLight *light, s32 id) {
    LightEffect *effect;
    LightFollow params;

    params.base = 0.0f;
    params.scale = 90.0f;
    params.source = effect_newColorAnim(0, 0.0f, 0, 0, 0, 220, 220, 255);
    effect = func_1516387C(light, 2, 16, 300, 0, 12, 255, 1);
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
    effect = func_1516387C(light, 2, 16, 300, 0, 12, 255, 1);
    if (effect != NULL) {
        memcpy(&effect->data.follow, &params, sizeof(params));
    }
}

void func_15009D28(PointLight *light, s32 id) {
    light_createPulse(light, 1, 5, 300, 0, 0, 255, 1);
}

void func_15009D6C(PointLight *light, s32 id) {
    light_createPulse(light, 2, 5, 300, 0, 0, 255, 1);
}

void func_15009DB0(PointLight *light, s32 id) {
    light_createJitter(light, 4, 6, 0, 300, 2, 255, 1);
}

void func_15009DFC(PointLight *light, s32 id) {
    light_createJitter(light, 5, 6, 0, 300, 2, 255, 1);
}

void func_15009E48(PointLight *light, s32 id) {
    light_createFlicker(light, 1, 1, 14, 255, 1);
}

void func_15009E84(PointLight *light, s32 id) {
    light_createPulse(light, 3, 5, 300, 0, 0, 255, 1);
}

void func_15009EC8(PointLight *light, s32 id) {
    func_1516381C(light, 32, 255, 1);
}

void func_15009EF4(PointLight *light, s32 id) {
    light_createFlicker(light, 1, 3, 35, 255, 1);
}

void func_15009F30(PointLight *light, s32 id) {
    light_createPulse(light, 4, 36, 300, 0, 0, 255, 1);
}

void func_15009F74(PointLight *light, s32 id) {
    LightEffect *effect;
    LightFollow params;

    params.base = 0.0f;
    params.scale = 60.0f;
    params.source = effect_newColorAnim(1, 0.1f, 100, 100, 100, 255, 100, 100);
    effect = func_1516387C(light, 2, 16, 300, 0, 12, 255, 1);
    if (effect != NULL) {
        memcpy(&effect->data.follow, &params, sizeof(params));
    }
}

void func_1500A028(PointLight *light, s32 id) {
    light_createPulse(light, 5, 5, 300, 0, 0, 255, 1);
}

void func_1500A06C(PointLight *light, s32 id) {
    light_createPulse(light, 6, 5, 300, 0, 0, 255, 1);
}

void func_1500A0B0(PointLight *light, s32 id) {
    light_createJitter(light, 7, 6, 0, 300, 2, 255, 1);
}

void func_1500A0FC(PointLight *light, s32 id) {
    light_createJitter(light, 9, 6, 0, 300, 2, 255, 1);
}

void func_1500A148(PointLight *light, s32 id) {
    light_createJitter(light, 10, 6, 0, 300, 2, 255, 1);
}

void func_1500A194(PointLight *light, s32 id) {
    light_createJitter(light, 11, 38, 0, 300, 2, 255, 1);
}

void func_1500A1E0(PointLight *light, s32 id) {
    light_createFlicker(light, 1, 4, 14, 255, 1);
}

void func_1500A21C(PointLight *light, s32 id) {
    light_createPulse(light, 7, 5, 300, 0, 0, 255, 1);
}

void func_1500A260(PointLight *light, s32 id) {
    light_createLightning(light, 2, 0, 2, 300, 13, 255, 1);
}

void func_1500A2AC(PointLight *light, s32 id) {
    const u8 kinds[4] = { 2, 3, 4, 5 };

    light_spawnLampParticle(light, kinds[id - 51], 255, 1);
}

void func_1500A2F4(PointLight *light, s32 id) {
    const u8 kinds[2] = { 0, 1 };

    func_15164208(light, kinds[id - 56], 255, 1);
}

void func_1500A33C(PointLight *light, s32 id) {
    light_createPulse(light, 8, 5, 300, 0, 0, 255, 1);
}

void func_1500A380(PointLight *light, s32 id) {
    light_createPulse(light, 9, 40, 300, 0, 0, 255, 1);
}

void func_1500A3C4(PointLight *light, s32 id) {
    light_createJitter(light, 15, 6, 0, 300, 2, 255, 1);
}

void func_1500A410(PointLight *light, s32 id) {
    light_createPulse(light, 10, 5, 300, 0, 0, 255, 1);
}

void func_1500A454(PointLight *light, s32 id) {
    light_createFlicker(light, 0, 5, 14, 255, 1);
}

void func_1500A490(PointLight *light, s32 id) {
    light_createPulse(light, 11, 5, 300, 0, 0, 255, 1);
}

void func_1500A4D4(PointLight *light, s32 id) {
    light_createPulse(light, 12, 41, 300, 0, 0, 255, 1);
}

void func_1500A518(PointLight *light, s32 id) {
    light_createPulse(light, 13, 41, 300, 0, 0, 255, 1);
}

void func_1500A55C(PointLight *light, s32 id) {
    light_createColourCycle(light, 1, 22, 0, -1, 300, 255, 1);
}

void func_1500A5A8(PointLight *light, s32 id) {
    light_createJitter(light, 17, 6, 0, 300, 2, 255, 1);
}

void func_1500A5F4(PointLight *light, s32 id) {
    light_createJitter(light, 18, 6, 0, 300, 2, 255, 1);
}

void func_1500A640(PointLight *light, s32 id) {
    light_createJitter(light, 19, 6, 0, 300, 2, 255, 1);
}

void func_1500A68C(PointLight *light, s32 id) {
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

void func_1500A79C(PointLight *light, s32 id) {
    light_createJitter(light, 22, 6, 0, 300, 2, 255, 1);
}

/** Attaches the light to the object gfx_findObjectById returns for `id`, starting at its position. */
void func_1500A7E8(PointLight *light, s32 id) {
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

void func_1500A8C8(PointLight *light, s32 id) {
    light_createFlicker(light, 0, 6, 14, 255, 1);
}

void func_1500A904(PointLight *light, s32 id) {
    light_createFlicker(light, 1, id - 80, 14, 255, 1);
}

/* The light callbacks by id (light_runCallback). */
static void (*gLightCallbacks[89])(PointLight *light, s32 id) = {
    NULL, func_150095D8, func_15009600, func_15009628,
    func_15009628, func_15009628, func_15009628, func_1500969C,
    func_150096C4, func_150096C4, func_150096C4, func_150096C4,
    func_150096C4, func_15009740, func_15009768, func_150097A4,
    func_150097CC, func_15009818, func_15009844, func_15009870,
    func_15009894, func_150098D0, func_150098F8, func_15009944,
    func_15009990, func_15009990, func_15009990, func_15009990,
    func_15009AA0, func_15009AEC, func_15009B84, func_15009BD0,
    func_15009C7C, func_15009D28, func_15009D6C, func_15009DB0,
    func_15009DFC, func_15009E48, func_15009E84, func_15009EC8,
    func_15009EF4, func_15009F30, func_15009F74, func_1500A028,
    func_1500A06C, func_1500A0B0, func_1500A0FC, func_1500A148,
    func_1500A194, func_1500A1E0, func_1500A21C, func_1500A2AC,
    func_1500A2AC, func_1500A2AC, func_1500A2AC, func_1500A260,
    func_1500A2F4, func_1500A2F4, func_1500A33C, func_1500A380,
    func_1500A3C4, func_1500A410, func_1500A454, func_1500A490,
    func_1500A4D4, func_1500A55C, func_1500A5A8, func_1500A5F4,
    func_1500A640, func_15009A38, func_15009A38, func_15009A38,
    func_15009A38, func_1500A6D8, func_1500A6D8, func_1500A6D8,
    func_15009B38, func_1500A518, func_1500A68C, func_1500A79C,
    func_1500A7E8, func_1500A7E8, func_1500A7E8, func_1500A7E8,
    func_1500A7E8, func_1500A7E8, func_1500A8C8, func_1500A904,
    func_1500A904,
};

/** Runs light callback `id` (1..88) from the gLightCallbacks table on a freshly placed light. */
void light_runCallback(PointLight *light, s32 id) {
    if (id > 0 && id < 89) {
        if (gLightCallbacks[id] != NULL) {
            gLightCallbacks[id](light, id);
        }
    }
}

/** Payload (at +0x28) of the effect_newTimedEffect effect spawned by func_1500A990. */
typedef struct Game36680ParticleEffect {
    TimedEffect base;
    f32 params[12];
} Game36680ParticleEffect;

/** Ambient action 2: spawns a particle effect with three random angles. */
void func_1500A990(s32 unused) {
    f32 params[12];
    f32 fraction;
    Game36680ParticleEffect *effect;

    params[0] = 35.0f;
    params[1] = 35.0f;
    params[2] = 60.0f;
    params[3] = 34.0f;
    params[4] = 30.0f;
    params[5] = 50.0f;

    fraction = (osGetCount() * func_150ADA20()) & 0xFFFF;
    fraction *= 1.5259022e-05f; // 1/65535
    params[6] = (fraction + fraction) * 3.1415927f;

    fraction = (osGetCount() * func_150ADA20()) & 0xFFFF;
    fraction *= 1.5259022e-05f;
    params[7] = (fraction + fraction) * 3.1415927f;

    fraction = (osGetCount() * func_150ADA20()) & 0xFFFF;
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
    NULL, light_startColourCycle, func_1500A990, func_15011F20, func_150120B0, func_15011FA0,
};

/** Runs ambient action `action` (1..5) from the gAmbientActions table. */
void light_runAmbientAction(s32 action) {
    if (action > 0 && action < 6) {
        if (gAmbientActions[action] != NULL) {
            gAmbientActions[action](action);
        }
    }
}
