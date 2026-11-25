#include <ultra64.h>
#define func_1000E770 func_1000E770_2args
#include "functions.h"
#undef func_1000E770
#include "variables.h"

s32 func_151149AC(u32);
void func_1516441C(struct225 *arg0, struct227 *arg1);
void func_15187FC0(s32 arg0, s32 *arg1);
void func_15188010(s32 arg0, f32 *arg1);
void func_1511172C(s32 arg0);
void func_1515F170(s32 arg0, u8 arg1);

extern u8 D_800886F0[];
extern u8 D_800886F4[];
extern u8 D_800886F8[];
extern void (*D_8008B358[])(struct225 *, s32, u8);
extern void (*D_8008B37C[])(s32);

typedef struct {
    s32 unk0;
    u8 unk4;
} SubA15161F4C;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8 b4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB15161F4C;

struct225 *func_151602C0(Header *header, Header2 *header2, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7, s32 offset, u8 arg9, s32 argA);
struct225 *func_1516037C(Header *src, struct226 *arg1, s32 size, u8 arg3, s32 arg4);
void func_151603FC(struct225 *arg0);
s32  func_15160600(struct225 *arg0);
s32  func_1516065C(struct225 *arg0);
s32  func_15160684(struct225 *arg0);
s32  func_151607A4(struct225 *arg0);
void func_15160954(f32 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, struct225 *arg4);
s32  func_15161238(struct127 *arg0, struct127 *arg1);
struct225 *func_1516127C(struct226 *light, u8 arg1, s32 arg2);
struct225 *func_15161334(struct226 *light, u8 arg1, s32 arg2);
struct225 *func_15161408(struct226 *light, u8 arg1, s32 arg2);
struct225 *func_15161494(struct226 *light, u8 arg1, s32 arg2);
struct225 *func_15161540(struct226 *light, u8 arg1, s32 arg2);
struct225 *func_151615F8(struct226 *light, u8 sound, u8 kind, u8 type, u8 arg4, s32 arg5);
void func_151616D0(u8 arg0, u8 arg1, s32 arg2);
void func_15161714(struct225 *arg0);
void func_15161740(struct225 *arg0);
void func_1516176C(struct225 *arg0);
void func_15161798(struct225 *arg0);
void func_151617C4(struct225 *arg0);
void func_151617E4(struct225 *arg0);
void func_15161804(struct225 *arg0);
void func_15161860(struct225 *arg0);
void func_151618BC(u16 arg0, s16 arg1, u8 arg2, s32 arg3, struct17 *arg4, s16 arg5, s16 arg6);
struct225 *func_151619A0(s32 arg0, s16 arg1, u8 arg2, s32 arg3);
s32  func_15161A68(struct225 *arg0);
struct225 *func_15161E24(struct127 *arg0, u8 arg1, u8 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, u8 arg8, s32 arg9);
void func_15161F2C(struct225 *arg0);
struct225 *func_15162034(s32 arg0, u8 arg1, s32 arg2);
void func_15162110(s32 arg0);
struct225 *func_15162740(s32 arg0, u8 arg1, u8 arg2, u8 arg3, s16 arg4, s8 arg5, u8 arg6, s32 arg7);
struct225 *func_1516284C(Header *header, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, u8 arg6, s32 size, u8 arg8, u8 arg9, s32 argA);
void func_15162EF8(struct239 *arg0);
void func_15162F50(struct239 *arg0);
struct225 *func_15162FAC(s32 arg0, u8 arg1, u8 arg2, u8 arg3, s16 arg4, s8 arg5, u8 arg6, s32 arg7);
struct225 *func_15163414(Header *header, f32* arg1, f32* arg2, f32* arg3, s8 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, u8 argA, u8 argB, s32 argC, u8 argD, s32 argE);
struct225 *func_15163604(s32 arg0, u8 arg1, u8 arg2, s16 arg3, u8 arg4, s32 arg5, u8 arg6, s32 arg7);
s32  func_15163704(struct225 *arg0);
void func_1516381C(s32 arg0, u8 arg1, u8 arg2, s32 arg3);
void func_1516387C(s32 arg0, u8 arg1, s8 arg2, s16 arg3, u8 arg4, s32 arg5, u8 arg6, s32 arg7);
s32  func_151639D0(struct225 *arg0, s32 arg1, u8 arg2);
void func_15163A18(struct225 *arg0, s32 arg1, u8 arg2);
struct225 *func_15163A60(u8 arg0, u8 arg1, s32 arg2);
s32  func_15163B98(struct230 *arg0);
struct225 *func_15163BE8(s32 arg0, u8 arg1, s32 arg2);
s32  func_15163CD0(struct225 *arg0);
void func_15163CF8(struct227 *arg0, struct225 *arg1);
void func_15163DEC(struct225 *arg0, struct228 *arg1);
s32  func_15163F50(struct225 *arg0, struct225 *arg1);
struct225 *func_15164208(s32 arg0, u8 arg1, u8 arg2, s32 arg3);
s32  func_1516429C(struct237 *arg0);
void func_151643A8(struct225 *arg0, s32 arg1, u8 arg2);
s32  func_151643F8(struct225 *arg0);
void func_151644F4(struct225 *effect, struct17 *pivot, f32 offset, f32 angleX, f32 angleZ);
s32  func_151644A8(struct225 *effect);

/* Creates a light at header2's position and an effect instance that owns it. */
struct225 *func_151602C0(Header *header, Header2 *header2, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7, s32 offset, u8 arg9, s32 argA) {
    struct225 *effect;
    struct226 *light;

    effect = NULL;
    light = func_1515D5F8(header2->unk0, header2->unk4, header2->unk8, arg2, arg3, arg4, arg5, arg6, arg7, 0);
    if (light != NULL) {
        effect = func_1516037C(header, light, offset, arg9, argA);
        if (effect != NULL) {
            effect->unkE |= 2;
        } else {
            func_1515F10C(light);
        }
    }
    return effect;
}

/* Allocates an effect instance (0x18-byte header plus `size` bytes of payload) for a light. */
struct225 *func_1516037C(Header *header, struct226 *light, s32 size, u8 arg3, s32 arg4) {
    struct225 *effect = func_15167A68(53, arg4, size + 0x18, 1, arg3, 1);

    if (effect == NULL) {
        return NULL;
    }
    memcpy(&effect->unkE, header, sizeof(Header));
    effect->unk14 = light;
    return effect;
}

/* Per-frame update: counts down the lifetime and runs the update callback; removes the effect when either ends. */
void func_151603FC(struct225 *effect) {
    s32 pad; // matching: unused stack slot keeps the original frame size
    u8 done = FALSE;

    if (effect->unkE & 1) {
        effect->unk10 -= D_800BE9E4;
        if (effect->unk10 < 0) {
            done = TRUE;
        }
    }
    if (!done && effect->unkF != -1 && D_8008B0F0[effect->unkF]() == 0) {
        done = TRUE;
    }
    if (done) {
        func_1516972C(effect);
    }
}

// requires jump table
extern void (*D_8008B150[])(void *);

/* Event handler: events 0x17-0x22 addressed to this effect's type toggle or remove it; others go to the type's handler. */
void func_151604A0(void *arg0, u8 *message, u8 event) {
    struct225 *effect = arg0;
    void (*handler)(void *);
    struct17 position;

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
            effect->unk14->unk9 = 0;
            break;
        case 0x18:
            effect->unk14->unk9 = 1;
            break;
        case 0x1C:
            position.unk0 = effect->unk14->unkE;
            position.unk4 = effect->unk14->unk10;
            position.unk8 = effect->unk14->unk12;
            func_151618BC(16000, 0, 0, 0, &position, 500, 1000);
            return;
        case 0x1F:
            effect->unkF = 11;
            effect->unk14->unk9 = 0;
            break;
        case 0x22:
            func_1516972C(effect);
            return;
        }
        break;
    default:
        handler = D_8008B150[effect->unk12];
        if (handler != NULL) {
            handler(effect);
        }
        break;
    }
}

/* Sets the light's intensity from the "scale" debug value. */
s32 func_15160600(struct225 *effect) {
    effect->unk14->unk2F = func_151422DC(0, "scale", 0, 0xFF, 0xFF, "../Effects/Light/light.c", 575);
    return 1;
}

s32 func_1516065C(struct225 *effect) {
    func_15163CF8((struct227 *)&effect->unk18, effect);
    return 1;
}

s32 func_15160684(struct225 *effect) {
    func_15163DEC(effect, (struct228 *)&effect->unk18);
    return 1;
}

void func_150A7960(f32 *mtx, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/* Payload of func_151606A8: the object the light hangs from. */
typedef struct {
    f32 unk0; // rotation passed to func_150A8050
    f32 unk4;
    f32 unk8;
    u8  padC[0x4];
    s16 unk10; // position
    s16 unk12;
    s16 unk14;
} LightMount;

/* Places the light at the point (0, -415, 0) in the mount's rotated and translated frame. */
s32 func_151606A8(struct225 *effect) {
    LightMount **mount;
    union {
        f32 mf[4][4];
        s64 align;
    } mtx;
    f32 z;
    f32 y;
    f32 x;

    mount = (LightMount **)&effect->unk18;
    func_150A8050(mtx.mf, (*mount)->unk0, (*mount)->unk4, (*mount)->unk8);
    mtx.mf[3][0] = (*mount)->unk10;
    mtx.mf[3][1] = (*mount)->unk12;
    mtx.mf[3][2] = (*mount)->unk14;
    func_150A7960(mtx.mf[0], D_800A66B4[0], D_800A66B4[1], D_800A66B4[2], &x, &y, &z);
    effect->unk14->unkE = x;
    effect->unk14->unk10 = y;
    effect->unk14->unk12 = z;
    return 1;
}

/* Payload of func_151607A4: bursts of flickers that ramp the light up from dark. */
typedef struct {
    f32 peak;    // intensity a flicker ramps up to
    f32 level;   // current intensity
    f32 rate;    // intensity gained per frame
    f32 delay;   // frames before the current flicker starts ramping
    s16 count;   // flickers left in this burst
    u8  sound;   // play a sound on each flicker
} LightFlicker;

s32 func_151607A4(struct225 *effect) {
    LightFlicker *flicker = (LightFlicker *)&effect->unk18;

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
    effect->unk14->unk2F = flicker->level;
    return 1;
}

/* Starts one flicker: drops to dark, picks a ramp rate and delay, and optionally plays a sound at the light. */
void func_15160954(f32 *level, f32 *peak, f32 *rate, f32 *delay, struct225 *effect) {
    s32 pad0; // matching: unused stack slot
    struct17 position;
    s32 pad1; // matching: unused stack slot

    *level = 0.0f;
    *rate = *peak / ((func_150ADA68() * 12.0f) + 8.0f);
    *delay = (func_150ADA68() * 28.0f) + 1.0f;

    position.unk0 = effect->unk14->unkE;
    position.unk4 = effect->unk14->unk10;
    position.unk8 = effect->unk14->unk12;

    if (((LightFlicker *)&effect->unk18)->sound) {
        func_151618BC(25000, 0, 0, 0, &position, 500, 1500);
    }
}

/* Payload of an effect whose light follows an actor, or a point on one of its bones. */
typedef struct {
    struct127 *actor;
    u8 actorId;      // actor->unique_id when attached; a change means the actor slot was reused
    u8 bone;         // index of the bone matrix in actor->unk1D4
    u8 pad6[0x2];
    struct17 offset; // position in the bone's frame
    s8 callback;     // index into D_8008B1F8, or -1
    u8 unk15;
} LightAttach;

/* Creates a light that follows `actor` (see func_15160B74). */
struct225 *func_15160A58(struct127 *actor, u8 bone, struct17 *offset, u8 arg3, s16 lifetime, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s8 callback, s32 extraSize, u8 unk15, u8 argE, s32 argF) {
    struct225 *effect;
    Header header;
    LightAttach attach;
    Header2 position;

    if (actor == NULL) {
        return NULL;
    }

    header.unk0 = arg3;
    header.unk1 = 5;
    header.unk2 = lifetime;
    header.unk4 = 16;

    attach.actor = actor;
    attach.actorId = actor->unique_id;
    attach.bone = bone;
    attach.offset = *offset;

    position.unk0 = 0;
    position.unk4 = 0;
    position.unk8 = 0;

    attach.callback = callback;
    attach.unk15 = unk15;

    effect = func_151602C0(&header, &position, arg5, arg6, arg7, arg8, 0xFF, 0, extraSize + sizeof(LightAttach), argE, argF);
    if (effect != NULL) {
        memcpy(&effect->unk18, &attach, sizeof(LightAttach));
    }
    return effect;
}

void func_15143134(f32 *in, f32 *out, f32 mtx[4][4]);

/* Moves the light to its actor (or the bone point); returns 0 once the actor is gone. */
u8 func_15160B74(struct225 *effect) {
    u8 result;
    s8 callback;
    LightAttach *attach;
    struct127 *actor;

    actor = ((LightAttach *)&effect->unk18)->actor;
    result = 1;
    attach = (LightAttach *)&effect->unk18;
    if (actor->interaction_state == 0) {
        return 0;
    }
    if (actor->unique_id != attach->actorId) {
        return 0;
    }
    if (actor->unk1D4 != NULL && (actor->unk74 & 0xF) != 0xF) {
        f32 position[3];

        func_15143134(&attach->offset.unk0, position, ((f32 (*)[4][4])actor->unk1D4)[attach->bone]);
        effect->unk14->unkE = position[0];
        effect->unk14->unk10 = position[1];
        effect->unk14->unk12 = position[2];
    } else {
        effect->unk14->unkE = actor->x_position;
        effect->unk14->unk10 = actor->y_position;
        effect->unk14->unk12 = actor->z_position;
    }
    callback = attach->callback;
    if (callback != -1) {
        result = D_8008B1F8[callback](effect);
    }
    return result;
}

/* Payload of func_15160E30: a light cast along a beam from an actor's bone, stopping at the first hit. */
typedef struct {
    struct127 *actor;
    u8 actorId;      // actor->unique_id when attached
    u8 bone;         // index of the bone matrix in actor->unk1D4
    u8 pad6[0x2];
    struct17 start;  // beam start in the bone's frame
    struct17 end;    // beam end in the bone's frame
    f32 scale;       // hit distance to intensity
    u8 flags;        // 1: cast from the bone, 2: flag the hit actor
    u8 unk25;
} LightBeam;

/* Creates a beam light on `actor` (see func_15160E30). */
struct225 *func_15160CDC(struct127 *actor, u8 bone, struct17 *start, struct17 *end, f32 scale, u8 arg5, s16 lifetime, s32 arg7, s32 arg8, s32 arg9, s32 argA, u8 fromBone, u8 unk25, u8 flagHit, u8 argE, s32 argF) {
    struct225 *effect;
    Header header;
    LightBeam beam;
    Header2 position;
    s32 flag1;
    s32 flag2;

    if (actor == NULL) {
        return NULL;
    }

    header.unk0 = arg5;
    header.unk1 = 6;
    header.unk2 = lifetime;
    header.unk4 = 17;

    beam.actor = actor;
    beam.actorId = actor->unique_id;
    beam.bone = bone;
    beam.start = *start;
    beam.end = *end;
    beam.scale = scale;

    flag1 = fromBone ? 1 : 0;
    flag2 = flagHit ? 2 : 0;
    beam.flags = flag2 | flag1;
    beam.unk25 = unk25;

    position.unk0 = 0;
    position.unk4 = 0;
    position.unk8 = 0;

    effect = func_151602C0(&header, &position, arg7, arg8, arg9, argA, 0xFF, 0, sizeof(LightBeam), argE, argF);
    if (effect != NULL) {
        memcpy(&effect->unk18, &beam, sizeof(LightBeam));
    }
    return effect;
}

/* Result of func_15081690's ray cast. */
typedef struct {
    struct127 *hit;
    f32 distance;
    f32 position[3];
    u8 pad14[0x45];
    u8 hitCount;
    u8 pad5A[0xA];
} RayHit;

void func_15081690(struct127 *, f32, f32, f32, f32, f32, f32, RayHit *, f32, s32, s32, s32, s32, s32, s32);
s32 func_15145128(f32 *, f32 *, f32 *, f32 *);
s32 func_15145EA4(f32 **in, f32 **out, f32 mtx[4][4], s32 count);

/* Casts the beam and puts the light where it hits; the light is off and at the actor when nothing is hit. */
s32 func_15160E30(struct225 *effect) {
    struct127 *actor;
    LightBeam *beam;
    s32 hitCount;

    actor = ((LightBeam *)&effect->unk18)->actor;
    beam = (LightBeam *)&effect->unk18;
    if (actor->interaction_state == 0) {
        return 0;
    }
    if (actor->unique_id != beam->actorId) {
        return 0;
    }
    if (actor->unk1D4 != NULL && (beam->flags & 1) && (actor->unk74 & 0xF) != 0xF) {
        f32 start[3];
        f32 dir[3];
        RayHit ray;
        f32 *in[2];
        f32 *out[2];

        in[0] = &beam->start.unk0;
        in[1] = &beam->end.unk0;
        out[0] = start;
        out[1] = dir;
        func_15145EA4(in, out, ((f32 (*)[4][4])actor->unk1D4)[beam->bone], 2);
        {
            f32 length;

            dir[0] -= start[0];
            dir[1] -= start[1];
            dir[2] -= start[2];
            {
                f32 invLength;

                if (func_15145128(dir, dir, &length, &invLength) == 0) {
                    effect->unk14->unkE = actor->x_position;
                    effect->unk14->unk10 = actor->y_position;
                    effect->unk14->unk12 = actor->z_position;
                    effect->unk14->unk2F = 0;
                    effect->unk14->unk9 = 1;
                    return 1;
                }
            }
        }
        func_15081690(actor, start[0], start[1], start[2], dir[0], dir[1], dir[2], &ray, 0.0f, 0, 0, 1, -1, 0, 0);
        hitCount = ray.hitCount;
        if (hitCount == 0) {
            effect->unk14->unkE = actor->x_position;
            effect->unk14->unk10 = actor->y_position;
            effect->unk14->unk12 = actor->z_position;
            effect->unk14->unk2F = 0;
            effect->unk14->unk9 = 1;
            return 1;
        }
        if ((beam->flags & 2) && hitCount >= 2) {
            struct127 *hit;

            hit = ray.hit;
            if (hit->id == 0x9E) {
                hit->unk2E4 = 1;
            }
        }
        effect->unk14->unkE = ray.position[0];
        effect->unk14->unk10 = ray.position[1];
        effect->unk14->unk12 = ray.position[2];
        {
            f32 intensity;

            intensity = ray.distance * beam->scale;
            if (intensity > 255.0f) {
                intensity = 255.0f;
            }
            if (intensity < 0.0f) {
                intensity = 0.0f;
            }
            effect->unk14->unk2F = intensity;
        }
        effect->unk14->unk9 = 0;
    } else {
        effect->unk14->unkE = actor->x_position;
        effect->unk14->unk10 = actor->y_position;
        effect->unk14->unk12 = actor->z_position;
        effect->unk14->unk2F = 0;
        effect->unk14->unk9 = 1;
    }
    return 1;
}

/* Returns whether `actor` is live and is not `exclude`. */
s32 func_15161238(struct127 *actor, struct127 *exclude) {
    if (actor->interaction_state == 0) {
        return 0;
    }
    if (actor->id == 0xFF) {
        return 0;
    }
    if (actor == exclude) {
        return 0;
    }
    return 1;
}

/* Payload of func_15163CF8: intensity = base + amplitude * sin(phase). */
typedef struct {
    f32 base;
    f32 amplitude;
    f32 phase;
    f32 speed; // phase advance per frame
} LightPulse;

/* Payload of func_15163DEC: intensity eases towards random targets, mostly between low and mid. */
typedef struct {
    f32 mid;
    f32 low;
    f32 high;
    f32 target;
    f32 timer;     // frames until the next target
    f32 interval;  // longest time between targets
    f32 smoothing; // fraction of the distance to the target covered per frame
    f32 level;     // current intensity
} LightJitter;

struct225 *func_1516127C(struct226 *light, u8 arg1, s32 arg2) {
    struct225 *effect;
    Header header;
    LightPulse pulse;

    pulse.base = 50.0f;
    pulse.amplitude = 40.0f;
    pulse.phase = func_150ADA68() * 6.283185482f;

    header.unk0 = 0;
    header.unk1 = 1;
    header.unk2 = 300;
    header.unk4 = 5;

    pulse.speed = 0.1020999998f;

    effect = func_1516037C(&header, light, sizeof(pulse), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->unk18, &pulse, sizeof(pulse));
    }
    return effect;
}

struct225 *func_15161334(struct226 *light, u8 arg1, s32 arg2) {
    struct225 *effect;
    Header header;
    LightJitter jitter;

    jitter.low = 1.0f;
    jitter.target = 1.0f;

    header.unk0 = 0;
    header.unk1 = 2;
    header.unk2 = 300;
    header.unk4 = 6;

    jitter.mid = 20.0f;
    jitter.high = 50.0f;
    jitter.timer = 0.0f;
    jitter.interval = 10.0f;
    jitter.smoothing = 0.2488999963f;
    jitter.level = 127.0f;

    effect = func_1516037C(&header, light, sizeof(jitter), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->unk18, &jitter, sizeof(jitter));
    }
    return effect;
}

struct225 *func_15161408(struct226 *light, u8 arg1, s32 arg2) {
    struct225 *effect;
    Header header;
    s32 mount;

    mount = func_151149AC(249);

    header.unk0 = 0;
    header.unk1 = 3;
    header.unk2 = 300;
    header.unk4 = 8;

    effect = func_1516037C(&header, light, sizeof(mount), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->unk18, &mount, sizeof(mount));
    }
    return effect;
}

struct225 *func_15161494(struct226 *light, u8 arg1, s32 arg2) {
    struct225 *effect;
    Header header;
    LightPulse pulse;

    header.unk0 = 0;
    header.unk1 = 1;
    header.unk2 = 300;
    header.unk4 = 5;

    pulse.base = 127.0f;
    pulse.amplitude = 100.0f;
    pulse.phase = 0.0f;
    pulse.speed = 0.07999999821f;

    effect = func_1516037C(&header, light, sizeof(pulse), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->unk18, &pulse, sizeof(pulse));
    }
    return effect;
}

struct225 *func_15161540(struct226 *light, u8 arg1, s32 arg2) {
    struct225 *effect;
    Header header;
    LightPulse pulse;

    pulse.base = 28.0f;
    pulse.amplitude = 27.0f;
    pulse.phase = func_150ADA68() * 6.283185482f;
    pulse.speed = 0.07660000026f;

    header.unk0 = 0;
    header.unk1 = 1;
    header.unk2 = 300;
    header.unk4 = 5;

    effect = func_1516037C(&header, light, sizeof(pulse), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->unk18, &pulse, sizeof(pulse));
    }
    return effect;
}

/* Creates a flicker effect; `kind` picks the peak intensity from D_800A66C0. */
struct225 *func_151615F8(struct226 *light, u8 sound, u8 kind, u8 type, u8 arg4, s32 arg5) {
    struct225 *effect;
    Header header;
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

    header.unk0 = 0;
    header.unk1 = 4;
    header.unk2 = 300;
    header.unk4 = type;

    effect = func_1516037C(&header, light, sizeof(flicker), arg4, arg5);
    if (effect != NULL) {
        memcpy(&effect->unk18, &flicker, sizeof(flicker));
    }
    return effect;
}

/* Sends event `event` to every light effect. */
void func_151616D0(u8 arg0, u8 event, s32 arg2) {
    struct234 message;

    message.unk0 = arg0;
    message.unk4 = arg2;

    func_1516944C(53, &message, event);
}

void func_15161714(struct225 *effect) {
    func_1514EDF0(effect, effect->unk18);
    func_151617C4(effect);
}

void func_15161740(struct225 *effect) {
    func_1514EDF0(effect, effect->unk18);
    func_151617E4(effect);
}

void func_1516176C(struct225 *effect) {
    func_1514EDF0(effect, effect->unk18);
    func_151617C4(effect);
}

void func_15161798(struct225 *effect) {
    func_1514EDF0(effect, effect->unk18);
    func_151617E4(effect);
}

void func_151617C4(struct225 *effect) {
    func_15169804(effect);
}

void func_151617E4(struct225 *effect) {
    func_15169824(effect);
}

/* Frees the effect's light if it owns one, then runs the type's handler from D_8008B208. */
void func_15161804(struct225 *effect) {
    if (effect->unkE & 2) {
        func_1515F10C(effect->unk14, effect);
    }
    D_8008B208[effect->unk12](effect);
}

/* Frees the effect's light if it owns one, then runs the type's handler from D_8008B2B0. */
void func_15161860(struct225 *effect) {
    if (effect->unkE & 2) {
        func_1515F10C(effect->unk14, effect);
    }
    D_8008B2B0[effect->unk12](effect);
}

void func_151618BC(u16 arg0, s16 arg1, u8 arg2, s32 arg3, struct17 *arg4, s16 arg5, s16 arg6) {
    typedef struct {
        s32 unk0[10];
    } temp151618BC;
    temp151618BC sp38;

    sp38 = *(temp151618BC *)D_800A66E4;
    func_10010F88(sp38.unk0[func_150ADA20() % 10U], arg0, arg1, arg2, arg3, (s32)arg4->unk0, (s32)arg4->unk4, (s32)arg4->unk8, arg5, arg6);
}

struct225 *func_151619A0(s32 arg0, s16 arg1, u8 arg2, s32 arg3) {
    struct225 *temp_v0;
    Header header;
    s32 tmp;
    Header2 header2;

    tmp = arg0;

    header.unk0 = 3;
    header.unk1 = 9;
    header.unk2 = arg1;
    header.unk4 = 20;

    header2.unk0 = 0;
    header2.unk4 = 0;
    header2.unk8 = 0;

    temp_v0 = func_151602C0(&header, &header2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, sizeof(tmp), arg2, arg3);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk18, &tmp, sizeof(tmp));
    }
    return temp_v0;
}

s32 func_15161A68(struct225 *arg0) {
    struct233 **tmp = &D_800C3958; // fakematch?

    if (D_800C35EA == 1) {
        f32 phi_f0;
        f32 phi_f2;
        struct233 *temp_v0 = *tmp + (s32) arg0->unk18;

        arg0->unk14->unkE = temp_v0->unk0;
        arg0->unk14->unk10 = temp_v0->unk4;
        arg0->unk14->unk12 = temp_v0->unk8;

        if (temp_v0->unk24 < 0.0f) {
            phi_f0 = 0.0f;
        } else {
            if (temp_v0->unk24 > 255.0f) {
                phi_f2 = 255.0f;
            } else {
                phi_f2 = temp_v0->unk24;
            }
            phi_f0 = phi_f2;
        }
        temp_v0->unk24 = phi_f0;
        arg0->unk14->unk5 = phi_f0;
        if (temp_v0->unk28 < 0.0f) {
            phi_f0 = 0.0f;
        } else {
            if (temp_v0->unk28 > 255.0f) {
                phi_f2 = 255.0f;
            } else {
                phi_f2 = temp_v0->unk28;
            }
            phi_f0 = phi_f2;
        }
        temp_v0->unk28 = phi_f0;
        arg0->unk14->unk6 = phi_f0;
        if (temp_v0->unk2C < 0.0f) {
            phi_f0 = 0.0f;
        } else {
            if (temp_v0->unk2C > 255.0f) {
                phi_f2 = 255.0f;
            } else {
                phi_f2 = temp_v0->unk2C;
            }
            phi_f0 = phi_f2;
        }
        temp_v0->unk2C = phi_f0;
        arg0->unk14->unk7 = phi_f0;

        if (temp_v0->unk18 < 0.0f) {
            phi_f0 = 0.0f;
        } else {
            if (temp_v0->unk18 > 255.0f) {
                phi_f2 = 255.0f;
            } else {
                phi_f2 = temp_v0->unk18;
            }
            phi_f0 = phi_f2;
        }
        temp_v0->unk18 = phi_f0;
        arg0->unk14->unk2F = phi_f0;

    }
    return 1;
}

struct225 *func_15161E24(struct127 *arg0, u8 arg1, u8 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, u8 arg8, s32 arg9) {
    struct225 *temp_v0;
    Header header;
    struct244 tmp;
    Header2 header2;

    if (arg0 == NULL) {
        return 0;
    }

    header.unk0 = arg2;
    header.unk1 = 10;
    header.unk2 = arg3;
    header.unk4 = 21;

    tmp.unk0 = arg0;
    tmp.unk4 = arg0->unique_id;
    tmp.unk5 = arg1;

    header2.unk0 = arg0->x_position;
    header2.unk4 = arg0->y_position;
    header2.unk8 = arg0->z_position;

    temp_v0 = func_151602C0(&header, &header2, arg4, arg5, arg6, arg7, 0xFF, 0, sizeof(tmp), arg8, arg9);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk18, &tmp, sizeof(tmp));
    }
    return temp_v0;
}

void func_15161F2C(struct225 *arg0) {
    func_15163F50(arg0, &arg0->unk18);
}

void func_15161F4C(struct225 *arg0, s32 arg1, u8 arg2) {
    SubA15161F4C *temp_v0 = (SubA15161F4C *)((u8 *)arg0 + 0x18);
    ArgB15161F4C *b = (ArgB15161F4C *)arg1;
    void (*fn)(struct225 *, s32, u8);

    if (arg2 == 0) {
        if ((b->unk0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C(arg0);
        trailing_label_15161F4C:
            ;
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else if (temp_v0->unk0 == b->u4.w4) {
            temp_v0->unk0 = b->unk0;
            temp_v0->unk4 = b->unk8;
        }
    }

    fn = D_8008B358[*(u8 *)((u8 *)arg0 + 0x1D)];
    if (fn != NULL) {
        fn(arg0, arg1, arg2);
    }
}

struct225 *func_15162034(s32 arg0, u8 arg1, s32 arg2) {
    struct225 *temp_v0;
    Header header;
    f32 src[8];

    header.unk0 = 0;
    header.unk1 = 2;
    header.unk2 = 0x12C;
    header.unk4 = 6;

    src[1] = 15.0f;
    src[0] = 22.0f;

    src[2] = 45.0f;
    src[3] = 1.0f;
    src[4] = 0.0f;
    src[5] = 10.0f;
    src[6] = D_800A6AF0;
    src[7] = 127.0f;

    temp_v0 = func_1516037C(&header, arg0, sizeof(src), arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk18, &src, sizeof(src));
    }
    return temp_v0;
}

void func_15162110(s32 arg0) {
    struct236 *temp_v0;
    f32 src[8];

    src[1] = 15.0f;
    src[2] = 0.0f;
    src[4] = 15.0f;
    src[5] = 0.0f;
    src[6] = 0.0f;
    src[0] = 42.5f;
    src[3] = 37.5f;
    src[7] = D_800A6AF4;

    temp_v0 = func_15149130(300, -1, 30, -1, 0, 0, sizeof(src), 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk28, &src, sizeof(src));
    }
}

void func_151621B8(struct236 *arg0) {
    struct235 *temp_v0;
    f32 temp_f0;

    temp_v0 = &arg0->unk28;
    temp_f0 = sinf(temp_v0->unk18);

    func_1515D4D4(
        (u8)(temp_v0->unk0 + (temp_f0 * temp_v0->unkC)),
        (u8)(temp_v0->unk4 + (temp_f0 * temp_v0->unk10)),
        (u8)(temp_v0->unk8 + (temp_f0 * temp_v0->unk14)),
        0);
    temp_v0->unk18 += temp_v0->unk1C * D_800BE9A4;
    temp_v0->unk18 = func_15144B68(temp_v0->unk18);
}
// NON-MATCHING: not quite right here
// struct235 *func_151621B8(struct236 *arg0) {
//     struct235 *temp_v0;
//     f32 temp_f0;
//     u8 temp_a0;
//     u8 temp_a1;
//     u8 temp_a2;
//
//     temp_v0 = &arg0->unk28;
//     temp_f0 = sinf(temp_v0->unk18);
//
//     temp_a0 = temp_v0->unk0 + (temp_f0 * temp_v0->unkC);
//     temp_a1 = temp_v0->unk4 + (temp_f0 * temp_v0->unk10);
//     temp_a2 = temp_v0->unk8 + (temp_f0 * temp_v0->unk14);
//
//
//     func_1515D4D4(temp_a0, temp_a1, temp_a2, 0);
//     temp_v0->unk18 += temp_v0->unk1C * D_800BE9A4;
//     temp_v0->unk18 = func_15144B68(temp_v0->unk18);
//
//     return temp_v0;
// }

struct vec3w { s32 unk0; s32 unk4; s32 unk8; };

struct225 *func_151623F4(struct226 *arg0, u8 arg1, u8 arg2, u8 arg3, s8 arg4, s16 arg5, u8 arg6, s32 arg7) {
    struct {
        struct vec3w a;
        struct vec3w b;
        f32 c;
        f32 d;
    } sp30;
    Header header;
    struct225 *temp_v0;

    if (arg1 >= 3) {
        return NULL;
    }

    sp30.a = *(struct vec3w *)D_800A670C[arg1];
    sp30.b = *(struct vec3w *)D_800A6730[arg1];

    sp30.c = 0.0f;
    sp30.d = D_800A6754[arg1];
    header.unk0 = arg3;
    header.unk1 = arg4;
    header.unk2 = arg5;
    header.unk4 = arg2;

    temp_v0 = func_1516037C(&header, arg0, 0x20, arg6, arg7);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x18), &sp30, 0x20);
    }
    return temp_v0;
}
s32 func_15162510(void *arg0) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        f32 unk10;
        f32 unk14;
        f32 unk18;
        f32 unk1C;
    } Struct15162510;
    register Struct15162510 *temp_v1;
    f32 temp_f0;

    temp_v1 = (Struct15162510 *)((s32)arg0 + 0x18);
    temp_f0 = sinf(*(f32 *)((s32)arg0 + 0x30));

    (*(struct226 **)((s32)arg0 + 0x14))->unk5 = temp_v1->unk0 + (temp_f0 * temp_v1->unkC);
    (*(struct226 **)((s32)arg0 + 0x14))->unk6 = temp_v1->unk4 + (temp_f0 * temp_v1->unk10);
    (*(struct226 **)((s32)arg0 + 0x14))->unk7 = temp_v1->unk8 + (temp_f0 * temp_v1->unk14);

    temp_v1->unk18 += temp_v1->unk1C * D_800BE9A4;
    temp_v1->unk18 = func_15144B68(temp_v1->unk18);
    return 1;
}
// s32 func_15162510(struct237 *arg0) {
//     struct235 *temp_v1;
//     f32 temp_f0;
//
//     temp_f0 = sinf(arg0->unk18.unk18);
//     temp_v1 = &arg0->unk18;
//
//     arg0->unk14->unk5 = temp_v1->unk0 + (temp_f0 * arg0->unk0[2]);
//     arg0->unk14->unk6 = temp_v1->unk4 + (temp_f0 * temp_v1->unk10);
//     arg0->unk14->unk7 = temp_v1->unk8 + (temp_f0 * temp_v1->unk14);
//
//     temp_v1->unk18 += temp_v1->unk1C * D_800BE9A4;
//     temp_v1->unk18 = func_15144B68(temp_v1->unk18);
//     return 1;
// }

struct225 *func_15162740(s32 arg0, u8 arg1, u8 arg2, u8 arg3, s16 arg4, s8 arg5, u8 arg6, s32 arg7) {
    struct225 *temp_v0;
    Header header;
    f32 src[8];

    if (arg1 >= 24) {
        return 0;
    }

    src[0] = D_800A67C0[arg1];
    src[1] = D_800A6760[arg1];
    src[2] = D_800A6820[arg1];
    src[3] = 1.0f;
    src[4] = 0.0f;
    src[5] = 10.0f;
    src[6] = D_800A6AF8;
    src[7] = 127.0f;

    header.unk0 = arg3;
    header.unk1 = arg5;
    header.unk2 = arg4;
    header.unk4 = arg2;

    temp_v0 = func_1516037C(&header, arg0, sizeof(src), arg6, arg7);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk18, &src, sizeof(src));
    }
    return temp_v0;
}

struct225 *func_1516284C(Header *header, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, u8 arg6, s32 offset, u8 arg8, u8 arg9, s32 argA) {
    struct225 *temp_v0;
    f32 src[8];

    if (arg8 >= 0x18) {
        return 0;
    }

    src[0] = D_800A67C0[arg8];
    src[1] = D_800A6760[arg8];
    src[2] = D_800A6820[arg8];
    src[3] = 1.0f;
    src[4] = 0.0f;
    src[5] = 10.0f;
    src[6] = D_800A6AFC;
    src[7] = 127.0f;
    temp_v0 = func_151602C0(header, arg1, 0, arg2, arg3, arg4, arg5, arg6, offset + sizeof(src), arg9, argA);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk18, &src, sizeof(src));
    }
    return temp_v0;
}

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s8  unk24;
    s8  unk25;
    s8  unk26;
    s8  unk27;
    s8  unk28;
} Src1516295C;

struct225 *func_1516295C(struct226 *arg0, u8 arg1, u8 arg2, u8 arg3, s16 arg4, s8 arg5, s8 arg6, s8 arg7, s8 arg8, s8 arg9, s32 argA, u8 argB, s32 argC) {
    struct225 *temp_v0;
    Header header;
    Src1516295C src;

    if (arg1 >= 5) {
        return NULL;
    }

    src.unk25 = arg6;
    src.unk26 = arg7;
    src.unk27 = arg8;
    src.unk28 = arg9;
    src.unk0 = D_800A6880[arg1];
    src.unk4 = D_800A6894[arg1];
    src.unk8 = D_800A6880[arg1] - D_800A6894[arg1];
    src.unkC = D_800A68A8[arg1];
    src.unk10 = D_800A68BC[arg1];
    src.unk14 = D_800A68BC[arg1] + D_800A68D0[arg1];
    src.unk18 = (D_800A68BC[arg1] + D_800A68D0[arg1]) + D_800A68E4[arg1];
    src.unk1C = ((D_800A68BC[arg1] + D_800A68D0[arg1]) + D_800A68E4[arg1]) + D_800A68D0[arg1];


    if (src.unkC < D_800A68BC[arg1]) {
        src.unk24 = 0;
    } else if (src.unkC < src.unk14) {
        src.unk24 = 1;
    } else if (src.unkC < src.unk18) {
        src.unk24 = 2;
    } else if (src.unkC < src.unk1C) {
        src.unk24 = 3;
    } else {
        src.unk24 = 4;
    }

    header.unk0 = arg3;
    header.unk1 = arg5;
    header.unk2 = arg4;
    header.unk4 = arg2;

    src.unk20 = 1.0f / D_800A68D0[arg1];

    temp_v0 = func_1516037C(&header, arg0, argA + 0x30, argB, argC);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk18, &src, 0x30);
    }
    return temp_v0;
}
// s32 func_1516295C(s32 arg0, u8 arg1, u8 arg2, u8 arg3, s16 arg4, s8 arg5, s8 arg6, s8 arg7, s8 arg8, s8 arg9, s32 argA, u8 argB, s32 argC) {
//     struct225 *temp_v0;
//     Header header;
//     struct238 src;
//
//     f32 temp_f0;
//     f32 temp_f12;
//     f32 temp_f16;
//     f32 temp_f18;
//     f32 temp_f2;
//     f32 temp_f8;
//
//     if (arg1 >= 5) {
//         return 0;
//     }
//
//     temp_f2 = D_800A6880[arg1];
//     temp_f12 = D_800A6894[arg1];
//     temp_f8 = D_800A68A8[arg1];
//     temp_f0 = D_800A68BC[arg1];
//     temp_f18 = D_800A68D0[arg1];
//     temp_f16 = D_800A68E4[arg1] ;
//
//     src.unk0 = temp_f2; // sp30
//     src.unk4 = temp_f12;
//     src.unk8 = temp_f2 - temp_f12;
//     src.unkC = temp_f8;
//     src.unk10 = temp_f0;
//     src.unk14 = temp_f18 + temp_f0;
//     src.unk18 = temp_f18 + temp_f0 + temp_f16;
//     src.unk1C = temp_f18 + temp_f0 + temp_f16 + temp_f18;
//
//     src.unk25 = arg6;
//     src.unk26 = arg7;
//     src.unk27 = arg8;
//     src.unk28 = arg9;
//
//     if (src.unkC < temp_f0) {
//         src.unk24 = 0;
//     } else if (src.unkC < src.unk14) {
//         src.unk24 = 1;
//     } else if (src.unkC < src.unk18) {
//         src.unk24 = 2;
//     } else if (src.unkC < src.unk1C) {
//         src.unk24 = 3;
//     } else {
//         src.unk24 = 4;
//     }
//
//     header.unk0 = arg3;
//     header.unk1 = arg5;
//     header.unk2 = arg4;
//     header.unk4 = arg2;
//
//     src.unk20 = 1.0f / temp_f18;
//
//     temp_v0 = func_1516037C(&header, arg0, argA + 0x30, argB, argC);
//     if (temp_v0 != NULL) {
//         memcpy(&temp_v0->unk18, &src, 0x30);
//     }
//     return temp_v0;
// }

// a biggun'
typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u8  unk24;
    s8  unk25;
    s8  unk26;
    s8  unk27;
    s8  unk28;
} Sub15162B28;

typedef struct {
    u8 pad0[0x14];
    struct226 *unk14;
    Sub15162B28 unk18;
} Arg15162B28;

extern void (*D_8008B364[])(Arg15162B28 *, s32);

s32 func_15162B28(Arg15162B28 *arg0) {
    Sub15162B28 *sub;
    s8 idx;
    u8 prev;
    s32 state;

    prev = arg0->unk18.unk24;
    sub = &arg0->unk18;
    if (arg0->unk18.unkC < arg0->unk18.unk10) {
        arg0->unk14->unk2F = sub->unk4;
        sub->unk24 = 0;
    } else {
        if (sub->unkC < sub->unk14) {
            f32 t;

            t = (sub->unkC - sub->unk10) * sub->unk20;
            arg0->unk14->unk2F = sub->unk4 + sub->unk8 * t;
            sub->unk24 = 1;
        } else {
            if (sub->unkC < sub->unk18) {
                arg0->unk14->unk2F = sub->unk0;
                sub->unk24 = 2;
            } else {
                f32 t;

                t = 1.0f - (sub->unkC - sub->unk18) * sub->unk20;
                arg0->unk14->unk2F = sub->unk4 + sub->unk8 * t;
                sub->unk24 = 3;
            }
        }
    }
    state = sub->unk24;
    if (state != prev) {
        switch (state) {
        case 0:
            idx = sub->unk25;
            break;
        case 1:
            idx = sub->unk26;
            break;
        case 2:
            idx = sub->unk27;
            break;
        case 3:
            idx = sub->unk28;
            break;
        default:
            idx = -1;
            break;
        }
        if (idx != -1) {
            D_8008B364[idx](arg0, state);
        }
    }
    sub->unkC += D_800BE9A4;
    while (sub->unk1C < sub->unkC) {
        sub->unkC -= sub->unk1C;
    }
    return 1;
}

void func_15162EF8(struct239 *arg0) {
    s8 sp1C[9]; // FIXME: what size should this be?
    struct240 *temp_v0;

    temp_v0 = func_151149AC(arg0->unk48);
    if (temp_v0 != NULL) {
        temp_v0->unk7C |= 1;
    }
    sp1C[0] = arg0->unk12 - 21;
    func_151403A8(&sp1C, 36);
}

void func_15162F50(struct239 *arg0) {
    s8 sp1C[9]; // FIXME: what size should this be?
    struct240 *temp_v0;

    temp_v0 = func_151149AC(arg0->unk48);
    if (temp_v0 != NULL) {
        temp_v0->unk7C &= ~1;
    }
    sp1C[0] = arg0->unk12 - 21;
    func_151403A8(&sp1C, 37);
}

struct225 *func_15162FAC(s32 arg0, u8 arg1, u8 arg2, u8 arg3, s16 arg4, s8 arg5, u8 arg6, s32 arg7) {
    struct225 *temp_v0;
    Header header;
    struct241 src;

    if (arg1 >= 3) {
        return NULL;
    }

    src.unk0 = 0.0f;
    src.unk4 = D_800A68F8[arg1];
    src.unk8 = 0.0f;
    src.unk14 = 0;
    src.unk20 = 0.0f;
    src.unkC = D_800A6904[arg1];
    src.unk10 = D_800A6910[arg1];
    src.unk24 = D_800A6934[arg1];
    src.unk28 = D_800A6940[arg1];
    src.unk2C = D_800A694C[arg1];
    src.unk18 = D_800A691C[arg1];
    src.unk1C = D_800A6928[arg1];
    src.unk30 = D_800A6958[arg1];

    header.unk0 = arg3;
    header.unk1 = arg5;
    header.unk2 = arg4;
    header.unk4 = arg2;

    temp_v0 = func_1516037C(&header, arg0, sizeof(src), arg6, arg7);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk18, &src, sizeof(src));
    }
    return temp_v0;
}

// big one
extern s32 func_151EF610(void);
s32 func_1000E770();

s32 func_151630F4(struct225 *arg0) {
    struct241 *s;
    s32 r;
    s32 sp20[2];
    s32 n;

    s = (struct241 *)&arg0->unk18;
    if (arg0->unk14->unk9 == 1) {
        return 1;
    }
    arg0->unk14->unk2F = s->unk0;
    {
        s32 i;
        for (i = D_800BE9E4; i != 0; i--) {
            s->unk0 -= s->unk0 * s->unk4;
        }
    }
    if (s->unk14 > 0) {
        s->unk20 -= D_800BE9A4;
        if (s->unk20 < 0.0f) {
            s->unk0 = func_150ADA68() * s->unk30 + s->unk2C;
            s->unk20 = func_150ADA68() * s->unk28 + s->unk24;
lab2:
            r = func_151EF610() % 1200 + s->unk14 * 800 - 2400;
            n = 60 - s->unk14 * 10;
            sp20[0] = n;
            sp20[1] = r;
            func_1000E770(NULL, NULL, n, r);
            s->unk14--;
        }
    } else {
        s->unk8 -= D_800BE9A4;
        if (s->unk8 < 0.0f) {
            s->unk14 = func_150ADA20() % (u32)(s->unk1C + 1) + s->unk18;
            s->unk20 = 0.0f;
            s->unk8 = func_150ADA68() * s->unk10 + s->unkC;
        }
    }
    return 1;
}

struct225 *func_15163414(Header *header, f32* arg1, f32* arg2, f32* arg3, s8 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, u8 argA, u8 argB, s32 argC, u8 argD, s32 argE) {
    struct225 *temp_v0;
    struct245 data;
    Header2 header2;

    data.unk0 = arg1;
    data.unk4 = arg2;
    data.unk8 = arg3;
    data.unkC = arg4;
    data.unkD = arg5;

    header2.unk0 = *arg1;
    header2.unk4 = *arg2;
    header2.unk8 = *arg3;

    temp_v0 = func_151602C0(header, &header2, arg6, arg7, arg8, arg9, argA, argB, argC + sizeof(data), argD, argE);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk18, &data, sizeof(data));
    }
    return temp_v0;
}

s32 func_15163504(struct225 *arg0) {
    s32 ret;

    ret = 1;
    arg0->unk14->unkE = *(f32 *)*(s32 *)((s32)arg0 + 0x18);
    arg0->unk14->unk10 = *(f32 *)*(s32 *)((s32)arg0 + 0x1C);
    arg0->unk14->unk12 = *(f32 *)*(s32 *)((s32)arg0 + 0x20);
    if (*(volatile s8 *)((s32)arg0 + 0x24) != -1) {
        return D_8008B36C[*(volatile s8 *)((s32)arg0 + 0x24)]();
    }
    return ret;
}
// NON-MATCHING: something is missing..
// s32 func_15163504(struct225 *arg0) {
//     s32 ret = 1;
//     arg0->unk14->unkE = arg0->unk18->unk0;
//     arg0->unk14->unk10 = arg0->unk18->unk4;
//     arg0->unk14->unk12 = arg0->unk18->unk8;
//     if (arg0->unk24 != -1) {
//         ret = D_8008B36C[arg0->unk24]();
//     }
//
//     return ret;
// }

void func_151635A8(struct225 *arg0, s32 arg1, u8 arg2) {
    void (**temp_v0)(struct225 *, s32, u8);

    temp_v0 = (void (**)(struct225 *, s32, u8))D_8008B370;
    if (temp_v0[*(volatile u8 *)((s32)arg0 + 0x25)] != NULL) {
        temp_v0[*(volatile u8 *)((s32)arg0 + 0x25)](arg0, arg1, arg2);
    }
}
// NON-MATCHING: similar issue to func_15163504
// void func_151635A8(struct225 *arg0, s32 arg1, u8 arg2) {
//     s32 (*func)(s32) = D_8008B370[arg0->unk25];
//     if (func != NULL) {
//         func(arg2);
//     }
// }

struct225 *func_15163604(s32 arg0, u8 arg1, u8 arg2, s16 arg3, u8 arg4, s32 arg5, u8 arg6, s32 arg7) {
    struct225 *temp_v0;
    Header header;
    f32 src[4];

    if (arg1 < 0) {
        return 0;
    }
    if (arg1 >= 14) {
        return 0;
    }

    src[0] = D_800A6964[arg1];
    src[1] = D_800A699C[arg1];
    src[2] = func_150ADA68() * D_800A6B00;
    src[3] = D_800A69D4[arg1];

    header.unk0 = arg4;
    header.unk1 = 1;
    header.unk2 = arg3;
    header.unk4 = arg2;

    temp_v0 = func_1516037C(&header, arg0, sizeof(src), arg6, arg7);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk18, &src, sizeof(src));
    }
    return temp_v0;
}

s32 func_15163704(struct225 *arg0) {
    struct227 *temp_v0;
    f32 tmp;
    s32 pad1;

    temp_v0 = &arg0->unk18;
    tmp = sinf(temp_v0->unk8);
    arg0->unk14->unk2F = temp_v0->unk0 + (temp_v0->unk4 * tmp);

    temp_v0->unk8 += temp_v0->unkC * D_800BE9A4;
    temp_v0->unk8 = func_15144B68(temp_v0->unk8);

    if (temp_v0->unk8 > D_800A6B04) {
        return 0;
    }
    return 1;
}

void func_1516381C(s32 arg0, u8 arg1, u8 arg2, s32 arg3) {
    Header tmp;
    tmp.unk0 = 0;
    tmp.unk1 = -1;
    tmp.unk2 = 300;
    tmp.unk4 = arg1;
    func_1516037C(&tmp, arg0, 0, arg2, arg3);
}

void func_1516387C(s32 arg0, u8 arg1, s8 arg2, s16 arg3, u8 arg4, s32 offset, u8 arg6, s32 arg7) {
    Header tmp;
    tmp.unk0 = arg1;
    tmp.unk1 = arg2;
    tmp.unk2 = arg3;
    tmp.unk4 = arg4;

    func_1516037C(&tmp, arg0, offset, arg6, arg7);
}

s32 func_151638E0(struct225 *arg0) {
    struct227 *temp_v1;
    s32 sp20[3];
    f32 sp1C;

    temp_v1 = &arg0->unk18;
    func_15187FC0(*(s32 *)temp_v1, sp20);
    func_15188010(*(s32 *)temp_v1, &sp1C);
    arg0->unk14->unk2F = temp_v1->unk4 + (temp_v1->unk8 * sp1C);
    return 1;
}
// NON-MATCHING: struct isnt quite right..
// s32 func_151638E0(struct225 *arg0) {
//     struct227 *temp_v1;
//     f32 sp20[3];
//     f32 sp1C;
//
//     temp_v1 = &arg0->unk18;
//     func_15187FC0(arg0->unk18, &sp20); // missing a lw here
//     func_15188010(temp_v1, &sp1C);
//     arg0->unk14->unk2F = (temp_v1->unk4 + (temp_v1->unk8 * sp1C));
//     return 1;
// }

s32 func_151639D0(struct225 *arg0, s32 arg1, u8 arg2) {
    if (arg2 == 0x27) {
        arg0->unk14->unk9 = 1;
    } else if (arg2 == 0x28) {
        arg0->unk14->unk9 = 0;
    }
}

void func_15163A18(struct225 *arg0, s32 arg1, u8 arg2) {
    if (arg2 == 0x27) {
        arg0->unk14->unk9 = 0;
    } else if (arg2 == 0x28) {
        arg0->unk14->unk9 = 1;
    }
}

struct225 *func_15163A60(u8 arg0, u8 arg1, s32 arg2) {
    struct225 *temp_v0_2;
    Header header;
    f32 tmp1[4];
    struct226 *temp_v0;
    Header2 header2;

    temp_v0 = func_151149AC(arg0); // maybe different types?
    if (temp_v0 == 0) {
        return 0;
    }
    tmp1[0] = D_800A6964[6]; // D_800A697C
    tmp1[1] = D_800A699C[6]; // D_800A69B4
    tmp1[2] = func_150ADA68() * D_800A6B08;
    tmp1[3] = D_800A69D4[6]; // D_800A69EC;

    header2.unk0 = temp_v0->unk10;
    header2.unk4 = temp_v0->unk12;
    header2.unk8 = temp_v0->unk14;

    header.unk0 = 2;
    header.unk1 = 17;
    header.unk2 = 300;
    header.unk4 = 5;

    temp_v0_2 = func_151602C0(&header, &header2, D_800A6964[6], 0, 0xFF, 0, 0xFF, 0, 20, arg1, arg2);
    if (temp_v0_2 != 0) {
        memcpy(&temp_v0_2->unk18, &tmp1, 16);
        memcpy(&temp_v0_2->unk28, &temp_v0, 4);
    }
    return temp_v0_2;
}

s32 func_15163B98(struct230 *arg0) {
    struct228* tmp;
    if (func_1516065C(arg0) == 0) {
        return 0;
    } else {
        tmp = arg0->unk28;
        if (tmp->unk6E == 1) {
            return 0;
        } else {
            return 1;
        }
    }
}

struct225 *func_15163BE8(s32 arg0, u8 arg1, s32 arg2) {
    struct225 *temp_v0;
    f32 src[4];

    src[0] = D_800A6964[6];
    src[1] = D_800A699C[6];
    src[2] = func_150ADA68() * D_800A6B0C;
    src[3] = D_800A69D4[6];

    temp_v0 = func_15160A58(arg0, 1, &D_800A6A0C, 2, 300, 100, 0, 0xFF, 0, 0xFF, 0, 0, sizeof(src), 1, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk30, &src, sizeof(src));
    }
    return temp_v0;
}

s32 func_15163CD0(struct225 *arg0) {
    func_15163CF8(&arg0->unk30, arg0);
    return 1;
}

void func_15163CF8(struct227 *arg0, struct225 *arg1) {
    f32 tmp = sinf(arg0->unk8);
    arg1->unk14->unk2F = arg0->unk0 + (arg0->unk4 * tmp);
    arg0->unk8 = arg0->unk8 + (arg0->unkC * D_800BE9A4);
    arg0->unk8 = func_15144B68(arg0->unk8);
}

void func_15163DEC(struct225 *arg0, struct228 *arg1) {
    arg1->unk10 = arg1->unk10 - D_800BE9A4;
    if (arg1->unk10 < 0.0f) {
        arg1->unk10 = func_150ADA68() * arg1->unk14;
        if ((func_150ADA20() & 3) != 0) {
            arg1->unkC = (func_150ADA68() * (arg1->unk0 - arg1->unk4)) + arg1->unk4;
        } else {
            arg1->unkC = (func_150ADA68() * (arg1->unk8 - arg1->unk0)) + arg1->unk0;
        }
    }
    arg1->unk1C += (arg1->unkC - arg1->unk1C) * arg1->unk18;
    arg0->unk14->unk2F = arg1->unk1C;
}

s32 func_15163F50(struct225 *arg0, struct225 *arg1) {
    struct127 *temp_v1;

    temp_v1 = arg1->unk0;
    if (temp_v1->interaction_state == 0) {
        return 0;
    }
    if (temp_v1->id == 0xFF) {
        return 0;
    }
    if (temp_v1->unique_id != arg1->unk4) {
        return 0;
    }
    arg0->unk14->unkE = temp_v1->x_position;
    arg0->unk14->unk10 = temp_v1->y_position;
    arg0->unk14->unk12 = temp_v1->z_position;
    return 1;
}

typedef struct {
    s32 field_0x0;
    union {
        s32 w;
        u8 b;
    } field_0x4;
    u8 field_0x8;
    u8 field_0x9;
} ActorRefMessage;

typedef struct {
    s32 field_0x0;
    u8 field_0x4;
} ActorRefPayload;

typedef struct {
    u8 pad_0[0x18];
    ActorRefPayload field_0x18;
} ActorRefObject;

void func_15163FEC(struct225 *arg0, ActorRefMessage *arg1, u8 arg2) {
    ActorRefPayload *temp_v0;
    s32 temp_v1;

    temp_v0 = (ActorRefPayload *)((u8 *)arg0 + 0x18);
    if (arg2 == 0) {
        temp_v1 = arg1->field_0x0;
        if ((temp_v1 == temp_v0->field_0x0) || (arg1->field_0x4.b == temp_v0->field_0x4)) {
            func_1516972C(arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->field_0x0 == arg1->field_0x0) {
            temp_v0->field_0x0 = arg1->field_0x4.w;
            temp_v0->field_0x4 = arg1->field_0x9;
        } else {
            if (temp_v0->field_0x0 == arg1->field_0x4.w) {
                temp_v0->field_0x0 = arg1->field_0x0;
                temp_v0->field_0x4 = arg1->field_0x8;
            }
        trailing_label_15163FEC:
            ;
        }
    } else if (D_8008B374[arg0->unk2D] != NULL) {
        D_8008B374[arg0->unk2D]((s32)arg0);
    }
}
void func_151640C0(ActorRefObject *arg0, ActorRefMessage *arg1, u8 arg2) {
    ActorRefObject **sp18;
    ActorRefMessage **sp1C;
    s32 temp_v1;
    s32 temp_v0;
    s32 temp_a0;
    s32 temp_a1;
    u8 temp_a3;
    u8 temp_a2;
    sp18 = &arg0;
    sp1C = &arg1;
    if (arg2 == 0x29) {
        temp_v1 = (*sp1C)->field_0x4.w;
        temp_v0 = (s32)*sp18;
        temp_a0 = ((ActorRefPayload *)temp_v1)->field_0x0;
        temp_a1 = ((ActorRefObject *)temp_v0)->field_0x18.field_0x0;
        temp_v0 += 0x18;
        temp_a2 = ((ActorRefPayload *)temp_v0)->field_0x4;
        temp_a3 = ((ActorRefPayload *)temp_v1)->field_0x4;
        if ((temp_a1 == temp_a0) || (temp_a2 == temp_a3) || (((struct127 *)temp_a1)->unique_id == temp_a3)) {
            func_1516972C((struct225 *)*sp18);
        }
    }
}

void func_15164134(struct225 *arg0, ActorRefMessage *arg1, u8 arg2) {
    ActorRefPayload *temp_v0;
    s32 temp_v1;

    temp_v0 = (ActorRefPayload *)((u8 *)arg0 + 0x18);
    if (arg2 == 0) {
        temp_v1 = arg1->field_0x0;
        if ((temp_v1 == temp_v0->field_0x0) || (arg1->field_0x4.b == temp_v0->field_0x4)) {
            func_1516972C(arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->field_0x0 == arg1->field_0x0) {
            temp_v0->field_0x0 = arg1->field_0x4.w;
            temp_v0->field_0x4 = arg1->field_0x9;
        } else {
            if (temp_v0->field_0x0 == arg1->field_0x4.w) {
                temp_v0->field_0x0 = arg1->field_0x0;
                temp_v0->field_0x4 = arg1->field_0x8;
            }
        trailing_label_15164134:
            ;
        }
    } else if (D_8008B37C[*(u8 *)((s32)arg0 + 0x3D)] != NULL) {
        D_8008B37C[*(u8 *)((s32)arg0 + 0x3D)]((s32)arg0);
    }
}

struct225 *func_15164208(s32 arg0, u8 arg1, u8 arg2, s32 arg3) {
    struct225 *temp_v0;
    Header header;
    struct243 src;

    src.unk0 = 0.0f;
    src.unk4 = arg1;

    header.unk0 = 0;
    header.unk1 = 20;
    header.unk2 = 300;
    header.unk4 = 39;

    temp_v0 = func_1516037C(&header, arg0, sizeof(src), arg2, arg3);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk18, &src, sizeof(src));
    }
    return temp_v0;
}

s32 func_1516429C(struct237 *arg0) {
    arg0->unk14->unk2F = arg0->unk18.unk0;
    arg0->unk18.unk0 = arg0->unk18.unk0 - (arg0->unk18.unk0 * D_800A6B10);
    return 1;
}

void func_1516434C(struct225 *arg0, struct227 *arg1, u8 arg2) {
    struct227 *temp = &arg0->unk18;

    if (arg2 == 0x33) {
        if (*(u8 *)((u8 *)arg1 + 4) == *(u8 *)((u8 *)temp + 4)) {
            temp->unk0 = arg1->unk0;
            arg0->unk14->unk5 = *(u8 *)((u8 *)arg1 + 5);
            arg0->unk14->unk6 = *(u8 *)((u8 *)arg1 + 6);
            arg0->unk14->unk7 = *(u8 *)((u8 *)arg1 + 7);
        }
    }
}

void func_151643A8(struct225 *arg0, s32 arg1, u8 arg2) {
    struct227 *tmp;
    tmp = &arg0->unk18;
    if (arg2 == 64) {
        tmp->unk24 |= 1;      // force odd?
    } else if (arg2 == 65) {
        tmp->unk24 &= 0xFFFE; // force even?
    }
}

s32 func_151643F8(struct225 *arg0) {
    func_1516441C(arg0, &arg0->unk18);
    return 1;
}

void func_15145CD0(s32 arg0, struct227 **arg1, f32 **arg2, s32 arg3);

void func_1516441C(struct225 *arg0, struct227 *arg1) {
    struct227 *parg1;
    f32 *pout;
    f32 out[3];

    parg1 = arg1;
    pout = &out[0];
    func_15145CD0(*(s32 *)&arg1->unkC, &parg1, &pout, 1);
    arg0->unk14->unkE = (s32)out[0];
    arg0->unk14->unk10 = (s32)out[1];
    arg0->unk14->unk12 = (s32)out[2];
}

/* Payload of func_151644A8: a light swinging below a pivot. */
typedef struct {
    f32 unk0;         // offset below the pivot
    f32 unk4;         // scale applied to the swing angles
    struct17 *unk8;   // swing angles (x and z are used)
    struct17 unkC;    // pivot position
} LightSwing;

s32 func_151644A8(struct225 *effect) {
    LightSwing *swing = (LightSwing *)&effect->unk18;
    f32 scale;
    struct17 *angles;

    angles = swing->unk8;
    scale = swing->unk4;
    func_151644F4(effect, &swing->unkC, swing->unk0, angles->unk0 * scale, angles->unk8 * scale);
    return 1;
}

/* Moves the light to `offset` in the frame rotated by (angleX, 0, angleZ) about `pivot`. */
void func_151644F4(struct225 *effect, struct17 *pivot, f32 offset, f32 angleX, f32 angleZ) {
    f32 z;
    f32 y;
    f32 x;
    f32 mtx[4][4];

    func_150A8050(mtx, angleX, 0.0f, angleZ);
    mtx[3][0] = pivot->unk0;
    mtx[3][1] = pivot->unk4;
    mtx[3][2] = pivot->unk8;
    func_150A7960(mtx[0], 0.0f, offset, 0.0f, &x, &y, &z);
    effect->unk14->unkE = x;
    effect->unk14->unk10 = y;
    effect->unk14->unk12 = z;
}

void func_151645C4(u8 arg0) {
    D_800DCDD0 = arg0;
    if (arg0 != 0) {
        switch (D_800BE9F0) {
        case 0xB:
            func_1511172C(4);
            break;
        case 6:
            func_1515F170(10, 1);
            func_1511172C(4);
        case 0x39:
            ((u8 *)&D_800DCD20)[0] = D_800886F0[0];
            ((u8 *)&D_800DCD20)[1] = D_800886F0[1];
            ((u8 *)&D_800DCD20)[2] = D_800886F0[2];
            D_800B0DF0->unk5 = D_800886F4[0];
            D_800B0DF0->unk6 = D_800886F4[1];
            D_800B0DF0->unk7 = D_800886F4[2];
            break;
        case 7:
        case 0xC:
            break;
        }
    } else {
        switch (D_800BE9F0) {
        case 6:
            func_1515F170(10, 0);
        case 7:
        case 0xC:
        case 0x39:
            func_1511172C(6);
            ((u8 *)&D_800DCD20)[0] = D_800886F8[0];
            ((u8 *)&D_800DCD20)[1] = D_800886F8[1];
            ((u8 *)&D_800DCD20)[2] = D_800886F8[2];
            D_800B0DF0->unk5 = 0xFF;
            D_800B0DF0->unk6 = 0xFF;
            D_800B0DF0->unk7 = 0xFF;
            break;
        case 0xB:
            func_1511172C(6);
            break;
        case 0x29:
            func_1511172C(1);
            break;
        }
    }
}
