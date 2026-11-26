#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 D_800886F0[];
extern u8 D_800886F4[];
extern u8 D_800886F8[];
extern void (*D_8008B37C[])(LightEffect *);

extern void (*D_8008B358[])(LightEffect *, ActorEvent *, u8);

/* Creates a light at `position` and an effect instance (with `size` bytes of payload) that owns it. */
LightEffect *func_151602C0(LightEffectHeader *header, Vec3i *position, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7, s32 size, u8 arg9, s32 argA) {
    LightEffect *effect;
    PointLight *light;

    effect = NULL;
    light = func_1515D5F8(position->x, position->y, position->z, arg2, arg3, arg4, arg5, arg6, arg7, 0);
    if (light != NULL) {
        effect = func_1516037C(header, light, size, arg9, argA);
        if (effect != NULL) {
            effect->flags |= 2;
        } else {
            func_1515F10C(light);
        }
    }
    return effect;
}

/* Allocates an effect instance (0x18-byte header plus `size` bytes of payload) for a light. */
LightEffect *func_1516037C(LightEffectHeader *header, PointLight *light, s32 size, u8 arg3, s32 arg4) {
    LightEffect *effect = func_15167A68(53, arg4, size + 0x18, 1, arg3, 1);

    if (effect == NULL) {
        return NULL;
    }
    memcpy(&effect->flags, header, sizeof(LightEffectHeader));
    effect->light = light;
    return effect;
}

/* Per-frame update: counts down the lifetime and runs the update callback; removes the effect when either ends. */
void func_151603FC(LightEffect *effect) {
    s32 pad; // matching: unused stack slot keeps the original frame size
    u8 done = FALSE;

    if (effect->flags & 1) {
        effect->lifetime -= D_800BE9E4;
        if (effect->lifetime < 0) {
            done = TRUE;
        }
    }
    if (!done && effect->update != -1 && D_8008B0F0[effect->update]() == 0) {
        done = TRUE;
    }
    if (done) {
        func_1516972C(effect);
    }
}

extern void (*D_8008B150[])(void *);

/* Event handler: events 0x17-0x22 addressed to this effect's type toggle or remove it; others go to the type's handler. */
void func_151604A0(void *arg0, u8 *message, u8 event) {
    LightEffect *effect = arg0;
    void (*handler)(void *);
    Vec3f position;

    switch (event) {
    case 0x17:
    case 0x18:
    case 0x1C:
    case 0x1F:
    case 0x22:
        // matching: a byte-array read of unk12 makes IDO load *message first
        if (*message != ((u8 *)effect)[0x12]) {
            return;
        }
        switch (event) {
        case 0:
            break;
        case 0x17:
            effect->light->disabled = 0;
            break;
        case 0x18:
            effect->light->disabled = 1;
            break;
        case 0x1C:
            position.x = effect->light->x;
            position.y = effect->light->y;
            position.z = effect->light->z;
            func_151618BC(16000, 0, 0, 0, &position, 500, 1000);
            return;
        case 0x1F:
            effect->update = 11;
            effect->light->disabled = 0;
            break;
        case 0x22:
            func_1516972C(effect);
            return;
        }
        break;
    default:
        handler = D_8008B150[effect->lightType];
        if (handler != NULL) {
            handler(effect);
        }
        break;
    }
}

/* Sets the light's intensity from the "scale" debug value. */
s32 func_15160600(LightEffect *effect) {
    effect->light->radius = func_151422DC(0, "scale", 0, 0xFF, 0xFF, "../Effects/Light/light.c", 575);
    return 1;
}

/* Light effect parameter tables, indexed by effect kind; the rest follow func_151618BC. */
static const f32 D_800A66B4[3] = { 0.0f, -415.0f, 0.0f };
static const f32 D_800A66C0[9] = { 70.0f, 38.0f, 70.0f, 55.0f, 50.0f, 80.0f, 140.0f, 70.0f, 34.0f };

s32 func_1516065C(LightEffect *effect) {
    func_15163CF8(&effect->data.pulse, effect);
    return 1;
}

s32 func_15160684(LightEffect *effect) {
    func_15163DEC(effect, &effect->data.jitter);
    return 1;
}

/* Places the light at the point (0, -415, 0) in the mount's rotated and translated frame. */
s32 func_151606A8(LightEffect *effect) {
    WorldObject **mount;
    union {
        MtxF mf;
        s64 align;
    } mtx;
    f32 z;
    f32 y;
    f32 x;

    mount = &effect->data.mount;
    func_150A8050(mtx.mf, (*mount)->t, (*mount)->u, (*mount)->v);
    mtx.mf[3][0] = (*mount)->x;
    mtx.mf[3][1] = (*mount)->y;
    mtx.mf[3][2] = (*mount)->z;
    func_150A7960(mtx.mf, D_800A66B4[0], D_800A66B4[1], D_800A66B4[2], &x, &y, &z);
    effect->light->x = x;
    effect->light->y = y;
    effect->light->z = z;
    return 1;
}

s32 func_151607A4(LightEffect *effect) {
    LightFlicker *flicker = &effect->data.flicker;

    if (flicker->count != 0) {
        if (flicker->delay < 0.0f) {
            flicker->level += flicker->rate * D_800BE9A4;
            if (flicker->peak < flicker->level) {
                flicker->level = flicker->peak;
                flicker->count--;
                if (flicker->count != 0) {
                    func_15160954(&flicker->level, &flicker->peak, &flicker->rate, &flicker->delay, effect);
                }
            }
        } else {
            flicker->delay -= D_800BE9A4;
        }
    } else if (func_150ADA68() < 0.005000000354f) {
        flicker->count = (func_150ADA20() % 5U) + 1;
        func_15160954(&flicker->level, &flicker->peak, &flicker->rate, &flicker->delay, effect);
    }
    effect->light->radius = flicker->level;
    return 1;
}

/* Starts one flicker: drops to dark, picks a ramp rate and delay, and optionally plays a sound at the light. */
void func_15160954(f32 *intensity, f32 *peak, f32 *rate, f32 *delay, LightEffect *effect) {
    s32 pad0; // matching: unused stack slot
    Vec3f position;
    s32 pad1; // matching: unused stack slot

    *intensity = 0.0f;
    *rate = *peak / ((func_150ADA68() * 12.0f) + 8.0f);
    *delay = (func_150ADA68() * 28.0f) + 1.0f;

    position.x = effect->light->x;
    position.y = effect->light->y;
    position.z = effect->light->z;

    if (effect->data.flicker.sound) {
        func_151618BC(25000, 0, 0, 0, &position, 500, 1500);
    }
}

/* Creates a light that follows `actor` (see func_15160B74). */
LightEffect *func_15160A58(Actor *actor, u8 bone, Vec3f *offset, u8 arg3, s16 lifetime, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s8 callback, s32 extraSize, u8 eventHandler, u8 argE, s32 argF) {
    LightEffect *effect;
    LightEffectHeader header;
    LightAttach attach;
    Vec3i position;

    if (actor == NULL) {
        return NULL;
    }

    header.flags = arg3;
    header.update = 5;
    header.lifetime = lifetime;
    header.lightType = 16;

    attach.actor = actor;
    attach.actorId = actor->unique_id;
    attach.bone = bone;
    attach.offset = *offset;

    position.x = 0;
    position.y = 0;
    position.z = 0;

    attach.callback = callback;
    attach.eventHandler = eventHandler;

    effect = func_151602C0(&header, &position, arg5, arg6, arg7, arg8, 0xFF, 0, extraSize + sizeof(LightAttach), argE, argF);
    if (effect != NULL) {
        memcpy(&effect->data, &attach, sizeof(LightAttach));
    }
    return effect;
}

/* Moves the light to its actor (or the bone point); returns 0 once the actor is gone. */
u8 func_15160B74(LightEffect *effect) {
    u8 result;
    s8 callback;
    LightAttach *attach;
    Actor *actor;

    actor = effect->data.attach.actor;
    result = 1;
    attach = &effect->data.attach;
    if (actor->type == 0) {
        return 0;
    }
    if (actor->unique_id != attach->actorId) {
        return 0;
    }
    if (actor->ob_matrix_ptr != NULL && (actor->unk74 & 0xF) != 0xF) {
        f32 position[3];

        func_15143134(&attach->offset.x, position, actor->ob_matrix_ptr[attach->bone]);
        effect->light->x = position[0];
        effect->light->y = position[1];
        effect->light->z = position[2];
    } else {
        effect->light->x = actor->xpos;
        effect->light->y = actor->ypos;
        effect->light->z = actor->zpos;
    }
    callback = attach->callback;
    if (callback != -1) {
        result = D_8008B1F8[callback](effect);
    }
    return result;
}

/* Creates a beam light on `actor` (see func_15160E30). */
LightEffect *func_15160CDC(Actor *actor, u8 bone, Vec3f *start, Vec3f *end, f32 scale, u8 arg5, s16 lifetime, s32 arg7, s32 arg8, s32 arg9, s32 argA, u8 fromBone, u8 eventHandler, u8 flagHit, u8 argE, s32 argF) {
    LightEffect *effect;
    LightEffectHeader header;
    LightBeam beam;
    Vec3i position;
    s32 flag1;
    s32 flag2;

    if (actor == NULL) {
        return NULL;
    }

    header.flags = arg5;
    header.update = 6;
    header.lifetime = lifetime;
    header.lightType = 17;

    beam.actor = actor;
    beam.actorId = actor->unique_id;
    beam.bone = bone;
    beam.start = *start;
    beam.end = *end;
    beam.scale = scale;

    flag1 = fromBone ? 1 : 0;
    flag2 = flagHit ? 2 : 0;
    beam.flags = flag2 | flag1;
    beam.eventHandler = eventHandler;

    position.x = 0;
    position.y = 0;
    position.z = 0;

    effect = func_151602C0(&header, &position, arg7, arg8, arg9, argA, 0xFF, 0, sizeof(LightBeam), argE, argF);
    if (effect != NULL) {
        memcpy(&effect->data, &beam, sizeof(LightBeam));
    }
    return effect;
}

/* Casts the beam and puts the light where it hits; the light is off and at the actor when nothing is hit. */
s32 func_15160E30(LightEffect *effect) {
    Actor *actor;
    LightBeam *beam;
    s32 hitType;

    actor = effect->data.beam.actor;
    beam = &effect->data.beam;
    if (actor->type == 0) {
        return 0;
    }
    if (actor->unique_id != beam->actorId) {
        return 0;
    }
    if (actor->ob_matrix_ptr != NULL && (beam->flags & 1) && (actor->unk74 & 0xF) != 0xF) {
        f32 start[3];
        f32 dir[3];
        RayHit ray;
        f32 *in[2];
        f32 *out[2];

        in[0] = &beam->start.x;
        in[1] = &beam->end.x;
        out[0] = start;
        out[1] = dir;
        func_15145EA4(in, out, actor->ob_matrix_ptr[beam->bone], 2);
        {
            f32 length;

            dir[0] -= start[0];
            dir[1] -= start[1];
            dir[2] -= start[2];
            {
                f32 invLength;

                if (func_15145128(dir, dir, &length, &invLength) == 0) {
                    effect->light->x = actor->xpos;
                    effect->light->y = actor->ypos;
                    effect->light->z = actor->zpos;
                    effect->light->radius = 0;
                    effect->light->disabled = 1;
                    return 1;
                }
            }
        }
        func_15081690(actor, start[0], start[1], start[2], dir[0], dir[1], dir[2], &ray, 0.0f, 0, 0, 1, -1, 0, 0);
        hitType = ray.hitType;
        if (hitType == 0) {
            effect->light->x = actor->xpos;
            effect->light->y = actor->ypos;
            effect->light->z = actor->zpos;
            effect->light->radius = 0;
            effect->light->disabled = 1;
            return 1;
        }
        if ((beam->flags & 2) && hitType >= 2) {
            Actor *hit;

            hit = ray.obj;
            if (hit->character_type == 0x9E) {
                hit->taskint[0] = 1;
            }
        }
        effect->light->x = ray.pos.x;
        effect->light->y = ray.pos.y;
        effect->light->z = ray.pos.z;
        {
            f32 intensity;

            intensity = ray.dist * beam->scale;
            if (intensity > 255.0f) {
                intensity = 255.0f;
            }
            if (intensity < 0.0f) {
                intensity = 0.0f;
            }
            effect->light->radius = intensity;
        }
        effect->light->disabled = 0;
    } else {
        effect->light->x = actor->xpos;
        effect->light->y = actor->ypos;
        effect->light->z = actor->zpos;
        effect->light->radius = 0;
        effect->light->disabled = 1;
    }
    return 1;
}

/* Returns whether `actor` is live and is not `exclude`. */
s32 func_15161238(Actor *actor, Actor *exclude) {
    if (actor->type == 0) {
        return 0;
    }
    if (actor->character_type == 0xFF) {
        return 0;
    }
    if (actor == exclude) {
        return 0;
    }
    return 1;
}

LightEffect *func_1516127C(PointLight *light, u8 arg1, s32 arg2) {
    LightEffect *effect;
    LightEffectHeader header;
    LightPulse pulse;

    pulse.base = 50.0f;
    pulse.amplitude = 40.0f;
    pulse.phase = func_150ADA68() * 6.283185482f;

    header.flags = 0;
    header.update = 1;
    header.lifetime = 300;
    header.lightType = 5;

    pulse.speed = 0.1020999998f;

    effect = func_1516037C(&header, light, sizeof(pulse), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &pulse, sizeof(pulse));
    }
    return effect;
}

LightEffect *func_15161334(PointLight *light, u8 arg1, s32 arg2) {
    LightEffect *effect;
    LightEffectHeader header;
    LightJitter jitter;

    jitter.low = 1.0f;
    jitter.target = 1.0f;

    header.flags = 0;
    header.update = 2;
    header.lifetime = 300;
    header.lightType = 6;

    jitter.mid = 20.0f;
    jitter.high = 50.0f;
    jitter.timer = 0.0f;
    jitter.interval = 10.0f;
    jitter.smoothing = 0.2488999963f;
    jitter.level = 127.0f;

    effect = func_1516037C(&header, light, sizeof(jitter), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &jitter, sizeof(jitter));
    }
    return effect;
}

LightEffect *func_15161408(PointLight *light, u8 arg1, s32 arg2) {
    LightEffect *effect;
    LightEffectHeader header;
    WorldObject *mount;

    mount = func_151149AC(249);

    header.flags = 0;
    header.update = 3;
    header.lifetime = 300;
    header.lightType = 8;

    effect = func_1516037C(&header, light, sizeof(mount), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &mount, sizeof(mount));
    }
    return effect;
}

LightEffect *func_15161494(PointLight *light, u8 arg1, s32 arg2) {
    LightEffect *effect;
    LightEffectHeader header;
    LightPulse pulse;

    header.flags = 0;
    header.update = 1;
    header.lifetime = 300;
    header.lightType = 5;

    pulse.base = 127.0f;
    pulse.amplitude = 100.0f;
    pulse.phase = 0.0f;
    pulse.speed = 0.07999999821f;

    effect = func_1516037C(&header, light, sizeof(pulse), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &pulse, sizeof(pulse));
    }
    return effect;
}

LightEffect *func_15161540(PointLight *light, u8 arg1, s32 arg2) {
    LightEffect *effect;
    LightEffectHeader header;
    LightPulse pulse;

    pulse.base = 28.0f;
    pulse.amplitude = 27.0f;
    pulse.phase = func_150ADA68() * 6.283185482f;
    pulse.speed = 0.07660000026f;

    header.flags = 0;
    header.update = 1;
    header.lifetime = 300;
    header.lightType = 5;

    effect = func_1516037C(&header, light, sizeof(pulse), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &pulse, sizeof(pulse));
    }
    return effect;
}

/* Creates a flicker effect; `kind` picks the peak intensity from D_800A66C0. */
LightEffect *func_151615F8(PointLight *light, u8 sound, u8 kind, u8 type, u8 arg4, s32 arg5) {
    LightEffect *effect;
    LightEffectHeader header;
    LightFlicker flicker;
    f32 peak;

    if (kind < 0) { // matching: always false for a u8, but removing it changes the object
        return NULL;
    }
    if (kind >= 9) {
        return NULL;
    }

    peak = D_800A66C0[kind];
    flicker.level = peak;
    flicker.rate = 0.0f;
    flicker.delay = 0.0f;
    flicker.peak = peak;
    flicker.count = 0;
    flicker.sound = sound;

    header.flags = 0;
    header.update = 4;
    header.lifetime = 300;
    header.lightType = type;

    effect = func_1516037C(&header, light, sizeof(flicker), arg4, arg5);
    if (effect != NULL) {
        memcpy(&effect->data, &flicker, sizeof(flicker));
    }
    return effect;
}

/* Sends event `event` to every light effect (of type lightType, for the addressed events). */
void light_sendEvent(u8 lightType, u8 event, s32 arg2) {
    LightEventMsg message;

    message.lightType = lightType;
    message.arg = arg2;

    func_1516944C(53, &message, event);
}

void func_15161714(LightEffect *effect) {
    func_1514EDF0(effect, effect->data.actorRef.actor);
    func_151617C4(effect);
}

void func_15161740(LightEffect *effect) {
    func_1514EDF0(effect, effect->data.actorRef.actor);
    func_151617E4(effect);
}

void func_1516176C(LightEffect *effect) {
    func_1514EDF0(effect, effect->data.actorRef.actor);
    func_151617C4(effect);
}

void func_15161798(LightEffect *effect) {
    func_1514EDF0(effect, effect->data.actorRef.actor);
    func_151617E4(effect);
}

void func_151617C4(LightEffect *effect) {
    func_15169804(effect);
}

void func_151617E4(LightEffect *effect) {
    func_15169824(effect);
}

/* Frees the effect's light if it owns one, then runs the type's handler from D_8008B208. */
void func_15161804(LightEffect *effect) {
    if (effect->flags & 2) {
        func_1515F10C(effect->light, effect);
    }
    D_8008B208[effect->lightType](effect);
}

/* Frees the effect's light if it owns one, then runs the type's handler from D_8008B2B0. */
void func_15161860(LightEffect *effect) {
    if (effect->flags & 2) {
        func_1515F10C(effect->light, effect);
    }
    D_8008B2B0[effect->lightType](effect);
}

/* Plays one of ten random sounds at `position`. */
void func_151618BC(u16 arg0, s16 arg1, u8 arg2, s32 arg3, Vec3f *position, s16 arg5, s16 arg6) {
    const s32 sounds[10] = { 1099, 1100, 1101, 1102, 1103, 1104, 1105, 1106, 1107, 1108 };

    func_10010F88(sounds[func_150ADA20() % 10U], arg0, arg1, arg2, arg3, position->x, position->y, position->z, arg5, arg6);
}

/*
 * More light effect tables. They follow func_151618BC because its sound list goes before them in the
 * .rodata (game_1BC650.c reads the jitter colour tables D_800A6760/67C0/6820 too).
 */
static const Vec3f D_800A670C[3] = { { 128.0f, 59.5f, 59.5f }, { 187.0f, 59.5f, 59.5f }, { 128.0f, 17.5f, 0.0f } };
static const Vec3f D_800A6730[3] = { { 128.0f, 128.0f, 128.0f }, { 68.0f, 59.5f, 59.5f }, { 127.0f, 17.5f, 0.0f } };
static const f32 D_800A6754[3] = { 0.0f, 0.1273f, 0.1273f };
const f32 D_800A6760[24] = {
    0.0f, 1.0f, 3.0f, 20.0f, 10.0f, 10.0f, 35.0f, 2.0f,
    0.0f, 1.0f, 5.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 30.0f, 30.0f, 10.0f, 3.0f, 0.0f, 0.0f, 0.0f,
};
const f32 D_800A67C0[24] = {
    0.0f, 12.0f, 10.0f, 30.0f, 25.0f, 18.0f, 55.0f, 25.0f,
    14.0f, 8.0f, 30.0f, 70.0f, 10.0f, 15.0f, 80.0f, 18.0f,
    8.0f, 110.0f, 90.0f, 30.0f, 10.0f, 25.0f, 30.0f, 35.0f,
};
const f32 D_800A6820[24] = {
    0.0f, 20.0f, 18.0f, 65.0f, 48.0f, 30.0f, 100.0f, 62.0f,
    60.0f, 19.0f, 60.0f, 255.0f, 50.0f, 40.0f, 255.0f, 44.0f,
    15.0f, 210.0f, 170.0f, 100.0f, 18.0f, 45.0f, 60.0f, 70.0f,
};
static const f32 D_800A6880[5] = { 0.0f, 50.0f, 50.0f, 50.0f, 50.0f };
static const f32 D_800A6894[5] = { 0.0f, 15.0f, 15.0f, 15.0f, 15.0f };
static const f32 D_800A68A8[5] = { 0.0f, 0.0f, 15.5f, 31.0f, 46.5f };
static const f32 D_800A68BC[5] = { 10.0f, 12.0f, 12.0f, 12.0f, 12.0f };
static const f32 D_800A68D0[5] = { 10.0f, 25.0f, 25.0f, 25.0f, 25.0f };
static const f32 D_800A68E4[5] = { 10.0f, 0.0f, 0.0f, 0.0f, 0.0f };
static const f32 D_800A68F8[3] = { 1.0f, 0.165333f, 0.165333f };
static const f32 D_800A6904[3] = { 1.0f, 102.0f, 102.0f };
static const f32 D_800A6910[3] = { 0.0f, 105.0f, 105.0f };
static const s32 D_800A691C[3] = { 5, 1, 1 };
static const s32 D_800A6928[3] = { 0, 4, 4 };
static const f32 D_800A6934[3] = { 1.0f, 6.0f, 6.0f };
static const f32 D_800A6940[3] = { 0.0f, 9.0f, 9.0f };
static const f32 D_800A694C[3] = { 255.0f, 155.0f, 255.0f };
static const f32 D_800A6958[3] = { 0.0f, 100.0f, 0.0f };
static const f32 D_800A6964[14] = {
    127.0f, 25.0f, 40.0f, 80.0f, 85.0f, 90.0f, 60.0f, 87.0f,
    21.0f, 35.0f, 40.0f, 120.0f, 25.0f, 17.5f,
};
static const f32 D_800A699C[14] = {
    126.0f, 24.0f, 40.0f, 40.0f, 55.0f, 60.0f, 30.0f, 43.0f,
    20.0f, 35.0f, 30.0f, 50.0f, 15.0f, 12.5f,
};
static const f32 D_800A69D4[14] = {
    0.1021f, 0.1021f, 0.1021f, 0.1021f, 0.1021f, 0.1021f, 0.2021f, 0.1021f,
    0.1021f, 0.39999998f, 0.1021f, 0.1021f, 0.1021f, 0.19999999f,
};
static const Vec3f D_800A6A0C = { 0.0f, 35.0f, 73.0f };

/* Creates a light driven by entry `index` of D_800C3958 (see func_15161A68). */
LightEffect *func_151619A0(s32 index, s16 lifetime, u8 arg2, s32 arg3) {
    LightEffect *effect;
    LightEffectHeader header;
    s32 payload;
    Vec3i position;

    payload = index;

    header.flags = 3;
    header.update = 9;
    header.lifetime = lifetime;
    header.lightType = 20;

    position.x = 0;
    position.y = 0;
    position.z = 0;

    effect = func_151602C0(&header, &position, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, sizeof(payload), arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &payload, sizeof(payload));
    }
    return effect;
}

/* Copies position, colour and intensity (each clamped to 0-255) from the effect's D_800C3958 entry to the light. */
s32 func_15161A68(LightEffect *effect) {
    // matching: the table-address local is needed for IDO's register order
    SeqTrackState **table = D_800C3958;

    if (D_800C35EA[0] == 1) {
        f32 value;
        SeqTrackState *source = *table + effect->data.index;

        effect->light->x = source->x;
        effect->light->y = source->y;
        effect->light->z = source->z;

        value = (source->unk24 < 0.0f) ? 0.0f : (source->unk24 > 255.0f) ? 255.0f : source->unk24;
        source->unk24 = value;
        effect->light->color[0] = value;

        value = (source->unk28 < 0.0f) ? 0.0f : (source->unk28 > 255.0f) ? 255.0f : source->unk28;
        source->unk28 = value;
        effect->light->color[1] = value;

        value = (source->unk2C < 0.0f) ? 0.0f : (source->unk2C > 255.0f) ? 255.0f : source->unk2C;
        source->unk2C = value;
        effect->light->color[2] = value;

        value = (source->unk18 < 0.0f) ? 0.0f : (source->unk18 > 255.0f) ? 255.0f : source->unk18;
        source->unk18 = value;
        effect->light->radius = value;
    }
    return 1;
}

/* Creates a light at `actor` that stays tied to it (see func_15163F50). */
LightEffect *func_15161E24(Actor *actor, u8 eventHandler, u8 arg2, s16 lifetime, s32 arg4, s32 arg5, s32 arg6, s32 arg7, u8 arg8, s32 arg9) {
    LightEffect *effect;
    LightEffectHeader header;
    LightActorRef ref;
    Vec3i position;

    if (actor == NULL) {
        return NULL;
    }

    header.flags = arg2;
    header.update = 10;
    header.lifetime = lifetime;
    header.lightType = 21;

    ref.actor = actor;
    ref.actorId = actor->unique_id;
    ref.eventHandler = eventHandler;

    position.x = actor->xpos;
    position.y = actor->ypos;
    position.z = actor->zpos;

    effect = func_151602C0(&header, &position, arg4, arg5, arg6, arg7, 0xFF, 0, sizeof(ref), arg8, arg9);
    if (effect != NULL) {
        memcpy(&effect->data, &ref, sizeof(ref));
    }
    return effect;
}

s32 func_15161F2C(LightEffect *effect) {
    return func_15163F50(effect, &effect->data.actorRef);
}

/* Event handler: drops the effect when its actor is removed and follows actor swaps, then runs the payload's event handler (D_8008B358). */
void func_15161F4C(LightEffect *effect, ActorEvent *event, u8 eventId) {
    LightActorRef *ref = &effect->data.actorRef;
    void (*handler)(LightEffect *, ActorEvent *, u8);

    if (eventId == 0) {
        if (event->actor == ref->actor || event->u4.actorId == ref->actorId) {
            func_1516972C(effect);
        // matching: this label ends a basic block, letting IDO copy the handler load into the branch delay slot
        trailing_label:
            ;
        }
    } else if (eventId == 0x2D) {
        if (ref->actor == event->actor) {
            ref->actor = event->u4.other;
            ref->actorId = event->otherId;
        } else if (ref->actor == event->u4.other) {
            ref->actor = event->actor;
            ref->actorId = event->actorId;
        }
    }

    handler = D_8008B358[effect->data.actorRef.eventHandler];
    if (handler != NULL) {
        handler(effect, event, eventId);
    }
}

LightEffect *func_15162034(PointLight *light, u8 arg1, s32 arg2) {
    LightEffect *effect;
    LightEffectHeader header;
    LightJitter jitter;

    header.flags = 0;
    header.update = 2;
    header.lifetime = 300;
    header.lightType = 6;

    jitter.low = 15.0f;
    jitter.mid = 22.0f;

    jitter.high = 45.0f;
    jitter.target = 1.0f;
    jitter.timer = 0.0f;
    jitter.interval = 10.0f;
    jitter.smoothing = 0.2488999963f;
    jitter.level = 127.0f;

    effect = func_1516037C(&header, light, sizeof(jitter), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &jitter, sizeof(jitter));
    }
    return effect;
}

/* Starts a global colour cycle object (see func_151621B8). */
void func_15162110(s32 arg0) {
    ColourPulseEffect *object;
    LightColourPulse pulse;

    pulse.base.y = 15.0f;
    pulse.base.z = 0.0f;
    pulse.amplitude.y = 15.0f;
    pulse.amplitude.z = 0.0f;
    pulse.phase = 0.0f;
    pulse.base.x = 42.5f;
    pulse.amplitude.x = 37.5f;
    pulse.speed = 0.07609999925f;

    object = func_15149130(300, -1, 30, -1, 0, 0, sizeof(pulse), 0xFF, 1);
    if (object != NULL) {
        memcpy(&object->pulse, &pulse, sizeof(pulse));
    }
}

/* Sets the colour passed to func_1515D4D4 from the object's colour cycle and advances it. */
void func_151621B8(ColourPulseEffect *object) {
    LightColourPulse *pulse;
    f32 sine;

    pulse = &object->pulse;
    sine = sinf(pulse->phase);

    func_1515D4D4(
        (u8)(pulse->base.x + (sine * pulse->amplitude.x)),
        (u8)(pulse->base.y + (sine * pulse->amplitude.y)),
        (u8)(pulse->base.z + (sine * pulse->amplitude.z)),
        0);
    pulse->phase += pulse->speed * D_800BE9A4;
    pulse->phase = func_15144B68(pulse->phase);
}

/* Creates a colour-cycling effect; `kind` (0-2) picks the base, amplitude and speed tables. */
LightEffect *func_151623F4(PointLight *light, u8 kind, u8 type, u8 arg3, s8 update, s16 lifetime, u8 arg6, s32 arg7) {
    LightColourPulse pulse;
    LightEffectHeader header;
    LightEffect *effect;

    if (kind >= 3) {
        return NULL;
    }

    pulse.base = D_800A670C[kind];
    pulse.amplitude = D_800A6730[kind];

    pulse.phase = 0.0f;
    pulse.speed = D_800A6754[kind];
    header.flags = arg3;
    header.update = update;
    header.lifetime = lifetime;
    header.lightType = type;

    effect = func_1516037C(&header, light, sizeof(pulse), arg6, arg7);
    if (effect != NULL) {
        memcpy(&effect->data, &pulse, sizeof(pulse));
    }
    return effect;
}

/* Sets the light's colour from the effect's colour cycle and advances it. */
s32 func_15162510(LightEffect *effect) {
    LightColourPulse *pulse;
    f32 sine;

    pulse = &effect->data.colourPulse;
    sine = sinf(pulse->phase);

    effect->light->color[0] = pulse->base.x + (sine * pulse->amplitude.x);
    effect->light->color[1] = pulse->base.y + (sine * pulse->amplitude.y);
    effect->light->color[2] = pulse->base.z + (sine * pulse->amplitude.z);

    pulse->phase += pulse->speed * D_800BE9A4;
    pulse->phase = func_15144B68(pulse->phase);
    return 1;
}

/* Creates a jitter effect; `kind` (0-23) picks mid, low and high from tables. */
LightEffect *func_15162740(PointLight *light, u8 kind, u8 type, u8 arg3, s16 lifetime, s8 update, u8 arg6, s32 arg7) {
    LightEffect *effect;
    LightEffectHeader header;
    LightJitter jitter;

    if (kind >= 24) {
        return NULL;
    }

    jitter.mid = D_800A67C0[kind];
    jitter.low = D_800A6760[kind];
    jitter.high = D_800A6820[kind];
    jitter.target = 1.0f;
    jitter.timer = 0.0f;
    jitter.interval = 10.0f;
    jitter.smoothing = 0.2488999963f;
    jitter.level = 127.0f;

    header.flags = arg3;
    header.update = update;
    header.lifetime = lifetime;
    header.lightType = type;

    effect = func_1516037C(&header, light, sizeof(jitter), arg6, arg7);
    if (effect != NULL) {
        memcpy(&effect->data, &jitter, sizeof(jitter));
    }
    return effect;
}

/* Like func_15162740, but also creates the light at `position`; `extraSize` bytes follow the jitter payload. */
LightEffect *func_1516284C(LightEffectHeader *header, Vec3i *position, s32 arg2, s32 arg3, s32 arg4, u8 arg5, u8 arg6, s32 extraSize, u8 kind, u8 arg9, s32 argA) {
    LightEffect *effect;
    LightJitter jitter;

    if (kind >= 24) {
        return NULL;
    }

    jitter.mid = D_800A67C0[kind];
    jitter.low = D_800A6760[kind];
    jitter.high = D_800A6820[kind];
    jitter.target = 1.0f;
    jitter.timer = 0.0f;
    jitter.interval = 10.0f;
    jitter.smoothing = 0.2488999963f;
    jitter.level = 127.0f;
    effect = func_151602C0(header, position, 0, arg2, arg3, arg4, arg5, arg6, extraSize + sizeof(jitter), arg9, argA);
    if (effect != NULL) {
        memcpy(&effect->data, &jitter, sizeof(jitter));
    }
    return effect;
}

/* Creates a cycle effect; `kind` (0-4) picks the levels and phase lengths from tables. */
LightEffect *func_1516295C(PointLight *light, u8 kind, u8 type, u8 arg3, s16 lifetime, s8 update, s8 onLow, s8 onRise, s8 onHigh, s8 onFall, s32 extraSize, u8 argB, s32 argC) {
    LightEffect *effect;
    LightEffectHeader header;
    LightCycle cycle;

    if (kind >= 5) {
        return NULL;
    }

    cycle.onLow = onLow;
    cycle.onRise = onRise;
    cycle.onHigh = onHigh;
    cycle.onFall = onFall;
    cycle.high = D_800A6880[kind];
    cycle.low = D_800A6894[kind];
    cycle.range = D_800A6880[kind] - D_800A6894[kind];
    cycle.time = D_800A68A8[kind];
    cycle.riseStart = D_800A68BC[kind];
    cycle.riseEnd = D_800A68BC[kind] + D_800A68D0[kind];
    cycle.fallStart = (D_800A68BC[kind] + D_800A68D0[kind]) + D_800A68E4[kind];
    cycle.period = ((D_800A68BC[kind] + D_800A68D0[kind]) + D_800A68E4[kind]) + D_800A68D0[kind];

    if (cycle.time < D_800A68BC[kind]) {
        cycle.state = 0;
    } else if (cycle.time < cycle.riseEnd) {
        cycle.state = 1;
    } else if (cycle.time < cycle.fallStart) {
        cycle.state = 2;
    } else if (cycle.time < cycle.period) {
        cycle.state = 3;
    } else {
        cycle.state = 4;
    }

    header.flags = arg3;
    header.update = update;
    header.lifetime = lifetime;
    header.lightType = type;

    cycle.invRamp = 1.0f / D_800A68D0[kind];

    // matching: the original copies 0x30 bytes, 4 more than sizeof(cycle)
    effect = func_1516037C(&header, light, extraSize + 0x30, argB, argC);
    if (effect != NULL) {
        memcpy(&effect->data, &cycle, 0x30);
    }
    return effect;
}

extern void (*D_8008B364[])(LightEffect *, s32);

/* Sets the intensity for the current point of the cycle, runs the state's callback on a state change, and advances time. */
s32 func_15162B28(LightEffect *effect) {
    LightCycle *cycle;
    s8 callback;
    u8 prevState;
    s32 state;

    cycle = &effect->data.cycle;
    prevState = cycle->state;
    if (cycle->time < cycle->riseStart) {
        effect->light->radius = cycle->low;
        cycle->state = 0;
    } else if (cycle->time < cycle->riseEnd) {
        f32 t;

        t = (cycle->time - cycle->riseStart) * cycle->invRamp;
        effect->light->radius = cycle->low + cycle->range * t;
        cycle->state = 1;
    } else if (cycle->time < cycle->fallStart) {
        effect->light->radius = cycle->high;
        cycle->state = 2;
    } else {
        f32 t;

        t = 1.0f - (cycle->time - cycle->fallStart) * cycle->invRamp;
        effect->light->radius = cycle->low + cycle->range * t;
        cycle->state = 3;
    }
    state = cycle->state;
    if (state != prevState) {
        switch (state) {
        case 0:
            callback = cycle->onLow;
            break;
        case 1:
            callback = cycle->onRise;
            break;
        case 2:
            callback = cycle->onHigh;
            break;
        case 3:
            callback = cycle->onFall;
            break;
        default:
            callback = -1;
            break;
        }
        if (callback != -1) {
            D_8008B364[callback](effect, state);
        }
    }
    cycle->time += D_800BE9A4;
    while (cycle->period < cycle->time) {
        cycle->time -= cycle->period;
    }
    return 1;
}

/* Cycle callback: sets bit 0 of touch.i.a on the payload's object and sends event 36. */
void func_15162EF8(LightEffect *effect) {
    s8 message[9];
    WorldObject *object;

    object = func_151149AC(effect->data.objectCycle.objectId);
    if (object != NULL) {
        object->touch.i.a |= 1;
    }
    message[0] = effect->lightType - 21;
    func_151403A8(&message, 36);
}

/* Cycle callback: clears bit 0 of touch.i.a on the payload's object and sends event 37. */
void func_15162F50(LightEffect *effect) {
    s8 message[9];
    WorldObject *object;

    object = func_151149AC(effect->data.objectCycle.objectId);
    if (object != NULL) {
        object->touch.i.a &= ~1;
    }
    message[0] = effect->lightType - 21;
    func_151403A8(&message, 37);
}

/* Creates a lightning effect; `kind` (0-2) picks its timings from tables. */
LightEffect *func_15162FAC(PointLight *light, u8 kind, u8 type, u8 arg3, s16 lifetime, s8 update, u8 arg6, s32 arg7) {
    LightEffect *effect;
    LightEffectHeader header;
    LightStorm storm;

    if (kind >= 3) {
        return NULL;
    }

    storm.level = 0.0f;
    storm.decay = D_800A68F8[kind];
    storm.waitTimer = 0.0f;
    storm.flashes = 0;
    storm.flashTimer = 0.0f;
    storm.waitMin = D_800A6904[kind];
    storm.waitRange = D_800A6910[kind];
    storm.flashGapMin = D_800A6934[kind];
    storm.flashGapRange = D_800A6940[kind];
    storm.flashMin = D_800A694C[kind];
    storm.flashesMin = D_800A691C[kind];
    storm.flashesRange = D_800A6928[kind];
    storm.flashRange = D_800A6958[kind];

    header.flags = arg3;
    header.update = update;
    header.lifetime = lifetime;
    header.lightType = type;

    effect = func_1516037C(&header, light, sizeof(storm), arg6, arg7);
    if (effect != NULL) {
        memcpy(&effect->data, &storm, sizeof(storm));
    }
    return effect;
}

/* Lightning update: decays the flash, fires the next flash and thunder sound of a burst, or waits for the next burst. */
s32 func_151630F4(LightEffect *effect) {
    LightStorm *storm;
    s32 pan;
    s32 sound[2];
    s32 volume;

    storm = &effect->data.storm;
    if (effect->light->disabled == 1) {
        return 1;
    }
    effect->light->radius = storm->level;
    {
        s32 i;
        for (i = D_800BE9E4; i != 0; i--) {
            storm->level -= storm->level * storm->decay;
        }
    }
    if (storm->flashes > 0) {
        storm->flashTimer -= D_800BE9A4;
        if (storm->flashTimer < 0.0f) {
            storm->level = func_150ADA68() * storm->flashRange + storm->flashMin;
            storm->flashTimer = func_150ADA68() * storm->flashGapRange + storm->flashGapMin;
        // matching: this label keeps IDO from moving the flashTimer store into the jal delay slot
        thunder:
            pan = func_151EF610() % 1200 + storm->flashes * 800 - 2400;
            volume = 60 - storm->flashes * 10;
            sound[0] = volume; // matching: dead stores; the original schedule needs them
            sound[1] = pan;
            func_1000E770(NULL, NULL, volume, pan);
            storm->flashes--;
        }
    } else {
        storm->waitTimer -= D_800BE9A4;
        if (storm->waitTimer < 0.0f) {
            storm->flashes = func_150ADA20() % (u32)(storm->flashesRange + 1) + storm->flashesMin;
            storm->flashTimer = 0.0f;
            storm->waitTimer = func_150ADA68() * storm->waitRange + storm->waitMin;
        }
    }
    return 1;
}

/* Creates a light at (*x, *y, *z) that follows those coordinates. */
LightEffect *func_15163414(LightEffectHeader *header, f32 *x, f32 *y, f32 *z, s8 callback, u8 handler, u8 arg6, u8 arg7, u8 arg8, u8 arg9, u8 argA, u8 argB, s32 extraSize, u8 argD, s32 argE) {
    LightEffect *effect;
    LightTrack track;
    Vec3i position;

    track.x = x;
    track.y = y;
    track.z = z;
    track.callback = callback;
    track.handler = handler;

    position.x = *x;
    position.y = *y;
    position.z = *z;

    effect = func_151602C0(header, &position, arg6, arg7, arg8, arg9, argA, argB, extraSize + sizeof(track), argD, argE);
    if (effect != NULL) {
        memcpy(&effect->data, &track, sizeof(track));
    }
    return effect;
}

/* Keeps the light at the tracked coordinates and runs the optional update callback. */
s32 func_15163504(LightEffect *effect) {
    s32 result;
    LightTrack *track;

    result = 1;
    track = &effect->data.track;
    effect->light->x = *track->x;
    effect->light->y = *track->y;
    effect->light->z = *track->z;
    if (track->callback != -1) {
        return D_8008B36C[track->callback]();
    }
    return result;
}

/* Event handler: forwards every event to the tracked light's D_8008B370 handler. */
void func_151635A8(LightEffect *effect, s32 arg1, u8 eventId) {
    LightTrack *track = &effect->data.track;

    if (D_8008B370[track->handler] != NULL) {
        D_8008B370[track->handler](effect, arg1, eventId);
    }
}

/* Creates a pulse effect; `kind` (0-13) picks base, amplitude and speed from tables, with a random phase. */
LightEffect *func_15163604(PointLight *light, u8 kind, u8 type, s16 lifetime, u8 arg4, s32 arg5, u8 arg6, s32 arg7) {
    LightEffect *effect;
    LightEffectHeader header;
    LightPulse pulse;

    if (kind < 0) { // matching: always false for a u8, kept like func_151615F8's
        return NULL;
    }
    if (kind >= 14) {
        return NULL;
    }

    pulse.base = D_800A6964[kind];
    pulse.amplitude = D_800A699C[kind];
    pulse.phase = func_150ADA68() * 6.283185482f;
    pulse.speed = D_800A69D4[kind];

    header.flags = arg4;
    header.update = 1;
    header.lifetime = lifetime;
    header.lightType = type;

    effect = func_1516037C(&header, light, sizeof(pulse), arg6, arg7);
    if (effect != NULL) {
        memcpy(&effect->data, &pulse, sizeof(pulse));
    }
    return effect;
}

/* One-shot pulse: like func_15163CF8, but returns 0 once the phase passes pi. */
s32 func_15163704(LightEffect *effect) {
    LightPulse *pulse;
    f32 sine;
    s32 pad; // matching: unused stack slot

    pulse = &effect->data.pulse;
    sine = sinf(pulse->phase);
    effect->light->radius = pulse->base + (pulse->amplitude * sine);

    pulse->phase += pulse->speed * D_800BE9A4;
    pulse->phase = func_15144B68(pulse->phase);

    if (pulse->phase > 3.141592741f) {
        return 0;
    }
    return 1;
}

/* Creates an effect with no payload and no update. */
void func_1516381C(PointLight *light, u8 type, u8 arg2, s32 arg3) {
    LightEffectHeader header;

    header.flags = 0;
    header.update = -1;
    header.lifetime = 300;
    header.lightType = type;
    func_1516037C(&header, light, 0, arg2, arg3);
}

/* Creates an effect with an uninitialised payload of `size` bytes. */
LightEffect *func_1516387C(PointLight *light, u8 flags, s8 update, s16 lifetime, u8 type, s32 size, u8 arg6, s32 arg7) {
    LightEffectHeader header;

    header.flags = flags;
    header.update = update;
    header.lifetime = lifetime;
    header.lightType = type;

    return func_1516037C(&header, light, size, arg6, arg7);
}

s32 func_151638E0(LightEffect *effect) {
    LightFollow *follow;
    s32 unused[3];
    f32 value;

    follow = &effect->data.follow;
    func_15187FC0(follow->source, unused);
    func_15188010(follow->source, &value);
    effect->light->radius = follow->base + (follow->scale * value);
    return 1;
}

/* Event handler: event 0x27 turns the light off (unk9 = 1), event 0x28 back on. */
void func_151639D0(LightEffect *effect, s32 arg1, u8 eventId) {
    if (eventId == 0x27) {
        effect->light->disabled = 1;
    } else if (eventId == 0x28) {
        effect->light->disabled = 0;
    }
}

/* Event handler: the inverse of func_151639D0. */
void func_15163A18(LightEffect *effect, s32 arg1, u8 eventId) {
    if (eventId == 0x27) {
        effect->light->disabled = 0;
    } else if (eventId == 0x28) {
        effect->light->disabled = 1;
    }
}

/* Creates a pulsing light at object `objectId` that lasts until the object is inactive (see func_15163B98). */
LightEffect *func_15163A60(u8 objectId, u8 arg1, s32 arg2) {
    LightEffect *effect;
    LightEffectHeader header;
    LightPulse pulse;
    WorldObject *object;
    Vec3i position;

    object = func_151149AC(objectId);
    if (object == NULL) {
        return NULL;
    }
    pulse.base = D_800A6964[6];
    pulse.amplitude = D_800A699C[6];
    pulse.phase = func_150ADA68() * 6.283185482f;
    pulse.speed = D_800A69D4[6];

    position.x = object->x;
    position.y = object->y;
    position.z = object->z;

    header.flags = 2;
    header.update = 17;
    header.lifetime = 300;
    header.lightType = 5;

    effect = func_151602C0(&header, &position, D_800A6964[6], 0, 0xFF, 0, 0xFF, 0, sizeof(pulse) + sizeof(object), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data.objectPulse.pulse, &pulse, sizeof(pulse));
        memcpy(&effect->data.objectPulse.object, &object, sizeof(object));
    }
    return effect;
}

/* Pulse update that also ends the effect once its object is inactive. */
s32 func_15163B98(LightEffect *effect) {
    WorldObject *object;

    if (func_1516065C(effect) == 0) {
        return 0;
    } else {
        object = effect->data.objectPulse.object;
        if (object->inactive == 1) {
            return 0;
        } else {
            return 1;
        }
    }
}

/* Creates a pulsing light attached to `actor`. */
LightEffect *func_15163BE8(Actor *actor, u8 arg1, s32 arg2) {
    LightEffect *effect;
    LightPulse pulse;

    pulse.base = D_800A6964[6];
    pulse.amplitude = D_800A699C[6];
    pulse.phase = func_150ADA68() * 6.283185482f;
    pulse.speed = D_800A69D4[6];

    effect = func_15160A58(actor, 1, &D_800A6A0C, 2, 300, 100, 0, 0xFF, 0, 0xFF, 0, 0, sizeof(pulse), 1, arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data.attachPulse.pulse, &pulse, sizeof(pulse));
    }
    return effect;
}

s32 func_15163CD0(LightEffect *effect) {
    func_15163CF8(&effect->data.attachPulse.pulse, effect);
    return 1;
}

/* Sets the intensity from the pulse and advances its phase. */
void func_15163CF8(LightPulse *pulse, LightEffect *effect) {
    f32 sine = sinf(pulse->phase);

    effect->light->radius = pulse->base + (pulse->amplitude * sine);
    pulse->phase = pulse->phase + (pulse->speed * D_800BE9A4);
    pulse->phase = func_15144B68(pulse->phase);
}

/* Sets the intensity from the jitter state, picking a new random target when its timer runs out. */
void func_15163DEC(LightEffect *effect, LightJitter *jitter) {
    jitter->timer = jitter->timer - D_800BE9A4;
    if (jitter->timer < 0.0f) {
        jitter->timer = func_150ADA68() * jitter->interval;
        if ((func_150ADA20() & 3) != 0) {
            jitter->target = (func_150ADA68() * (jitter->mid - jitter->low)) + jitter->low;
        } else {
            jitter->target = (func_150ADA68() * (jitter->high - jitter->mid)) + jitter->mid;
        }
    }
    jitter->level += (jitter->target - jitter->level) * jitter->smoothing;
    effect->light->radius = jitter->level;
}

s32 func_15163F50(LightEffect *effect, LightActorRef *ref) {
    Actor *actor;

    actor = ref->actor;
    if (actor->type == 0) {
        return 0;
    }
    if (actor->character_type == 0xFF) {
        return 0;
    }
    if (actor->unique_id != ref->actorId) {
        return 0;
    }
    effect->light->x = actor->xpos;
    effect->light->y = actor->ypos;
    effect->light->z = actor->zpos;
    return 1;
}

/* Event handler for bone-attached lights: like func_15161F4C, other events go to the payload's D_8008B374 handler. */
void func_15163FEC(LightEffect *effect, ActorEvent *event, u8 eventId) {
    LightAttach *attach = &effect->data.attach;

    if (eventId == 0) {
        Actor *actor = event->actor;

        if (actor == attach->actor || event->u4.actorId == attach->actorId) {
            func_1516972C(effect);
        }
    } else if (eventId == 0x2D) {
        if (attach->actor == event->actor) {
            attach->actor = event->u4.other;
            attach->actorId = event->otherId;
        } else if (attach->actor == event->u4.other) {
            LightAttach *dst = attach;

            dst->actor = event->actor;
            dst->actorId = event->actorId;
        }
    } else if (D_8008B374[attach->eventHandler] != NULL) {
        D_8008B374[attach->eventHandler](effect);
    }
}

/* Event 0x29 (the message points at another actor reference): drops the effect if it refers to the same actor. */
void func_151640C0(LightEffect *effect, ActorEvent *event, u8 eventId) {
    LightActorRef *other;
    LightActorRef *ref = &effect->data.actorRef;
    Actor *otherActor;
    Actor *actor;
    u8 otherId;
    u8 actorId;

    if (eventId == 0x29) {
        other = (LightActorRef *)event->u4.other;
        otherActor = other->actor;
        actor = ref->actor;
        actorId = ref->actorId;
        otherId = other->actorId;
        if (actor == otherActor || actorId == otherId || actor->unique_id == otherId) {
            func_1516972C(effect);
        }
    }
}

/* Event handler for beam lights: like func_15161F4C, other events go to the payload's D_8008B37C handler. */
void func_15164134(LightEffect *effect, ActorEvent *event, u8 eventId) {
    LightBeam *beam = &effect->data.beam;

    if (eventId == 0) {
        Actor *actor = event->actor;

        if (actor == beam->actor || event->u4.actorId == beam->actorId) {
            func_1516972C(effect);
        }
    } else if (eventId == 0x2D) {
        if (beam->actor == event->actor) {
            beam->actor = event->u4.other;
            beam->actorId = event->otherId;
        } else if (beam->actor == event->u4.other) {
            LightBeam *dst = beam;

            dst->actor = event->actor;
            dst->actorId = event->actorId;
        }
    } else if (D_8008B37C[beam->eventHandler] != NULL) {
        D_8008B37C[beam->eventHandler](effect);
    }
}

LightEffect *func_15164208(PointLight *light, u8 id, u8 arg2, s32 arg3) {
    LightEffect *effect;
    LightEffectHeader header;
    LightFlash flash;

    flash.level = 0.0f;
    flash.id = id;

    header.flags = 0;
    header.update = 20;
    header.lifetime = 300;
    header.lightType = 39;

    effect = func_1516037C(&header, light, sizeof(flash), arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &flash, sizeof(flash));
    }
    return effect;
}

s32 func_1516429C(LightEffect *effect) {
    LightFlash *flash = &effect->data.flash;

    effect->light->radius = flash->level;
    flash->level = flash->level - (flash->level * 0.7000000477f);
    return 1;
}

/* Event 0x33 with a matching id relights the flash and sets the light's colour. */
void func_1516434C(LightEffect *effect, LightFlashEvent *event, u8 eventId) {
    LightFlash *flash = &effect->data.flash;

    if (eventId == 0x33) {
        if (flash->id == event->id) {
            flash->level = event->level;
            effect->light->color[0] = event->r;
            effect->light->color[1] = event->g;
            effect->light->color[2] = event->b;
        }
    }
}

/* Beam event handler: event 64 turns on casting from the bone, event 65 turns it off. */
void func_151643A8(LightEffect *effect, s32 arg1, u8 eventId) {
    LightBeam *beam;

    beam = &effect->data.beam;
    if (eventId == 64) {
        beam->flags |= 1;
    } else if (eventId == 65) {
        beam->flags &= ~1;
    }
}

s32 func_151643F8(LightEffect *effect) {
    func_1516441C(effect, &effect->data.transformed);
    return 1;
}

/* Moves the light to the point transformed by its matrix. */
void func_1516441C(LightEffect *effect, LightTransformed *point) {
    LightTransformed *in;
    f32 *out;
    f32 position[3];

    in = point;
    out = &position[0];
    func_15145CD0(point->mtx, &in, &out, 1);
    effect->light->x = position[0];
    effect->light->y = position[1];
    effect->light->z = position[2];
}

/* Swings the light about the pivot with its object's rotation. */
s32 func_151644A8(LightEffect *effect) {
    LightSwing *swing = &effect->data.swing;
    f32 scale;
    WorldObject *object;

    object = swing->object;
    scale = swing->scale;
    func_151644F4(effect, &swing->pivot, swing->offset, object->t * scale, object->v * scale);
    return 1;
}

/* Moves the light to `offset` in the frame rotated by (angleX, 0, angleZ) about `pivot`. */
void func_151644F4(LightEffect *effect, Vec3f *pivot, f32 offset, f32 angleX, f32 angleZ) {
    f32 z;
    f32 y;
    f32 x;
    MtxF mtx;

    func_150A8050(mtx, angleX, 0.0f, angleZ);
    mtx[3][0] = pivot->x;
    mtx[3][1] = pivot->y;
    mtx[3][2] = pivot->z;
    func_150A7960(mtx, 0.0f, offset, 0.0f, &x, &y, &z);
    effect->light->x = x;
    effect->light->y = y;
    effect->light->z = z;
}

/* Turns the special lighting for the current level setting on or off, setting the colours in D_800DCD20 and D_800B0DF0. */
void func_151645C4(u8 enable) {
    D_800DCDD0 = enable;
    if (enable) {
        switch (level) {
        case 11:
            func_1511172C(4);
            break;
        case 6:
            func_1515F170(10, 1);
            func_1511172C(4);
            // fallthrough
        case 57:
            D_800DCD20[0] = D_800886F0[0];
            D_800DCD20[1] = D_800886F0[1];
            D_800DCD20[2] = D_800886F0[2];
            D_800B0DF0->unk5 = D_800886F4[0];
            D_800B0DF0->unk6 = D_800886F4[1];
            D_800B0DF0->unk7 = D_800886F4[2];
            break;
        case 7:
        case 12:
            break;
        }
    } else {
        switch (level) {
        case 6:
            func_1515F170(10, 0);
            // fallthrough
        case 7:
        case 12:
        case 57:
            func_1511172C(6);
            D_800DCD20[0] = D_800886F8[0];
            D_800DCD20[1] = D_800886F8[1];
            D_800DCD20[2] = D_800886F8[2];
            D_800B0DF0->unk5 = 0xFF;
            D_800B0DF0->unk6 = 0xFF;
            D_800B0DF0->unk7 = 0xFF;
            break;
        case 11:
            func_1511172C(6);
            break;
        case 41:
            func_1511172C(1);
            break;
        }
    }
}
