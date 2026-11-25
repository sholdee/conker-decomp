#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawn parameters of a particle (same layout as ParticleDesc in game_15D730.c). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode;
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ struct17 pos;
    /* 0x3C */ struct17 offset;
    /* 0x48 */ struct17 vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ s32 flags;
    /* 0x5C */ void *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;
    /* 0x63 */ s8 deathFunc;
    /* 0x64 */ s8 drawFunc;
    /* 0x65 */ u8 handlerSet;
    /* 0x66 */ u8 views;
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 minSize;
    /* 0x6A */ s16 unk6A;
    /* 0x6C */ f32 maxSize;
} ParticleDesc; /* size 0x70 */

/* Per-particle data of a pulsing particle: two sizes oscillating around a base size
 * (same layout as ParticlePulse in game_15D730.c). */
typedef struct {
    u8 phase[2];
    s8 phaseStep[2];
    f32 baseSize;
    f32 amplitude[2];
} ParticlePulse;

/* Partial view of the particle func_15130280 creates (Particle in game_15D730.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ ParticleDesc desc;
    /* 0x80 */ u8 pad80[0xA8 - 0x80];
    /* 0xA8 */ ParticlePulse pulse;
} Particle;

/* Data of the effect spawned by func_150C4120 (at +0x28 of the effect object). */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId;       /* owner->unique_id at spawn; a mismatch means the owner died */
    /* 0x08 */ f32 unk8;
    /* 0x0C */ Particle *particle;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 flags;
    /* 0x18 */ s32 unk18;
} EffectData; /* size 0x1C */

/* Effect object (struct260 is a shorter view of its head). */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 state; /* set to -1 to free the effect */
    /* 0x10 */ u8 pad10[0x28 - 0x10];
    /* 0x28 */ EffectData data;
} Effect;

extern Particle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);

/* Spawns an effect that follows `owner` (lifetime `duration`, -1 for the default 300) together with
 * a pulsing particle. */
void func_150C4120(struct127 *owner, s16 duration, u8 arg2, s32 arg3) {
    EffectData data;
    Effect *effect;
    EffectData *state;
    ParticleDesc spawn;
    ParticlePulse pulse;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.unk8 = 0.0f;
    data.particle = NULL;
    data.unk10 = 0.0f;
    data.unk14 = -1;
    data.flags = 0;
    data.unk18 = 0;
    if (owner->id == 0x34) {
        data.unk14 = 0x54;
        data.flags = 1;
    }
    effect = (Effect *)func_15149130((duration == -1) ? 300 : duration, -1, 0x53, -1, (duration == -1) ? 0 : 1,
                                     0x40, (struct37 *)sizeof(EffectData), arg2, arg3); /* arg6 is the data size; its prototype is wrong */
    if (effect != NULL) {
        state = &effect->data;
        memcpy(state, &data, sizeof(EffectData));
        {
        const s32 textures[4] = { 0x60, 0x61, 0x62, 0x63 };

        pulse.phase[0] = func_150ADA20();
        pulse.phase[1] = func_150ADA20();
        pulse.baseSize = spawn.width = spawn.height = 700.0f;
        pulse.phaseStep[0] = (s8)(func_150ADA20() % 6U) + 5; // matching: the (s8) steers t-register allocation
        pulse.phaseStep[1] = func_150ADA20() % 6U + 5;
        pulse.amplitude[0] = (func_150ADA68() * 0.25f + 0.200000003f) * pulse.baseSize;
        pulse.amplitude[1] = (func_150ADA68() * 0.25f + 0.200000003f) * pulse.baseSize;
        spawn.texture = *(textures + (func_150ADA20() & 3)); // matching: textures[i] reserves an extra temp
        spawn.renderMode = 0x2203;
        spawn.unk0 = 0x200005;
        spawn.unk4 = 0;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.r = 0;
        spawn.g = 0;
        spawn.b = 0;
        spawn.a = 0xFF;
        spawn.r2 = 0;
        spawn.g2 = 0;
        spawn.b2 = 0;
        spawn.alpha = 0xFF;
        spawn.brightness = 0xFF;
        spawn.pos = *(struct17 *)&D_800A5480;
        spawn.offset = *(struct17 *)&D_800A5480;
        spawn.vel = *(struct17 *)&D_800A5480;
        spawn.scaleStart = 1;
        spawn.gravity = 0.0f;
        spawn.scaleRate = 1.0f;
        spawn.flags = ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x8000 | 0x4000;
        spawn.timer = 300;
        spawn.maxSize = 10000.0f;
        spawn.fadeStart = 1;
        spawn.fadeRate = 0xFF;
        spawn.primColorMode = 6;
        spawn.envColorMode = 8;
        spawn.drawFunc = -1;
        spawn.updateFunc = 0x26;
        spawn.deathFunc = -1;
        spawn.handlerSet = 0;
        spawn.viewMask = NULL;
        spawn.views = 0xFF;
        spawn.minSize = 0;
        state->particle = func_15130280(&spawn, 1, 0, sizeof(ParticlePulse), arg2, arg3);
        if (state->particle != NULL) {
            memcpy(&state->particle->pulse, &pulse, sizeof(ParticlePulse));
        }
        }
    }
}

typedef struct {
    f32 x, y, z;
} Vec3f_C44A4;

typedef struct {
    u8 pad0[0x40];
    Vec3f_C44A4 pos; /* 0x40 */
    u8 pad4C[0x74 - 0x4C];
    s8 unk74;
} Sub28C_C44A4;

typedef struct {
    struct127 *obj;      /* 0x00 */
    u8 id;               /* 0x04 */
    u8 pad5[3];
    f32 unk8;            /* 0x08 */
    Sub28C_C44A4 *unkC;  /* 0x0C */
    f32 unk10;           /* 0x10 */
    s16 unk14;           /* 0x14 */
    u8 unk16;            /* 0x16 */
    u8 pad17;
    s32 unk18;           /* 0x18 */
} Sub28_C44A4;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD;
    s16 unkE;
    u8 pad10[0x18];
    Sub28_C44A4 unk28;
} Arg_C44A4;

extern void func_1504715C(void *, struct127 *);
extern s32 func_1514654C(struct127 *, struct126 *, s32, void *, void *, s32);
extern void func_15143134(void *, void *, s32);
extern void func_15143794(s32, s32, f32, f32 *);
extern s32 func_1515C0F8(struct127 *, f32 **);
extern void func_151C36D8(void *, s32, s32);
extern void func_151D9014(void *, void *, s32, f32, s32, s32, f32, s32, f32, f32, s32, void *, s32, s32, s32, s32);
extern Vec3f_C44A4 D_800A0350[];
extern u8 D_800A0374[];
extern Vec3f_C44A4 D_800A0378[];
extern u8 D_800A03A8[];

void func_150C44A4(Arg_C44A4 *arg0) {
    struct127 *obj;
    Sub28_C44A4 *s;
    Vec3f_C44A4 spFC;
    Vec3f_C44A4 spF0;
    f32 f;
    s32 i;
    f32 t;
    u8 spC0[0x24];
    Vec3f_C44A4 *spBC;
    Vec3f_C44A4 *spB8;
    struct126 *v0;
    s32 flag;
    f32 *spAC;
    f32 spA0[3];
    s32 r1;
    s32 r0;
    f32 f20;
    f32 f22;

    obj = arg0->unk28.obj;
    s = &arg0->unk28;
    if ((obj->interaction_state == 0) || (obj->unique_id != s->id)) {
        arg0->unkE = -1;
        return;
    }
    if ((obj->unk1D4 == NULL) || (s->unk16 & 2)) {
        if (s->unkC != NULL) {
            s->unkC->unk74 = 3;
        }
        return;
    }
    func_1504715C(spC0, obj);
    if (s->unk18 == 0) {
        s->unk18 = func_10010154(0x96, obj, 0x36B0, 0x190, 0x4268);
    }
    if (s->unk16 & 1) {
        v0 = func_1503195C(obj, s->unk14, 0);
        if (v0 == NULL) {
            return;
        }
        if (*(s32 *)((u8 *)obj + 0x2EC) != 0) {
            f = *(f32 *)((u8 *)obj + 0x2D8) * *(f32 *)((u8 *)obj + 0x2DC);
        } else {
            f = 0.0f;
        }
        i = f * 4.0f;
        if (i >= 4) {
            i = 3;
        }
        t = (f - i * 0.25f) * 4.0f;
        spF0.x = D_800A0378[i].x * t;
        spF0.y = D_800A0378[i].y * t;
        spF0.z = D_800A0378[i].z * t;
        spBC = &spF0;
        spB8 = &spFC;
        if (func_1514654C(obj, v0, D_800A03A8[i], &spBC, &spB8, 1) == 0) {
            spFC.x = obj->x_position;
            spFC.y = obj->y_position + 800.0f;
            spFC.z = obj->z_position;
        }
    } else {
        if (obj->unk2E8 != 0) {
            f = (f32)obj->unk2E4 / (f32)obj->unk2E8;
        } else {
            f = 1.0f;
        }
        i = f * 3.0f;
        if (i >= 3) {
            i = 2;
        }
        t = (f - i * 0.33333334f) * 3.0f;
        spF0.x = D_800A0350[i].x * t;
        spF0.y = D_800A0350[i].y * t;
        spF0.z = D_800A0350[i].z * t;
        func_15143134(&spF0, &spFC, (D_800A0374[i] << 6) + (s32)obj->unk1D4);
    }
    if (s->unkC != NULL) {
        s->unkC->unk74 = -1;
        s->unkC->pos = spFC;
    }
    flag = 1;
    if ((D_800BE9F0 == 2) && (func_150A29C8(obj - D_800CC2D0, 0x4065) == 0)) {
        flag = 0;
    }
    if (flag) {
        s->unk8 += (0.1290000081f + func_150ADA68() * 0.1170000061f) * D_800BE9A4;
        while (s->unk8 > 1.0f) {
            func_151C36D8(&spFC, arg0->unkC, arg0->unk1);
            s->unk8 -= 1.0f;
        }
    }
    s->unk10 += (0.1300000101f + func_150ADA68() * 0.2f) * D_800BE9A4;
    if (s->unk10 > 1.0f) {
        if (func_1515C0F8(obj, &spAC) == 0) {
            spAC = &D_800A5480;
        }
        do {
            r1 = func_150ADA20();
            r0 = func_150ADA20();
            func_15143794((s16)(r1 & 0xFF), (s16)((r0 % 25U) - 0x40), func_150ADA68() * 2.597000122f + 4.870000362f, spA0);
            spA0[0] -= spAC[0] * 0.6f;
            spA0[2] -= spAC[2] * 0.6f;
            f20 = func_150ADA68();
            r1 = func_150ADA20();
            r0 = func_150ADA20();
            f22 = func_150ADA68();
            func_151D9014(&spFC, spA0, 0xC, f20 * 0.2580000162f + -0.6080000401f, (r1 % 41U) + 60, (r0 % 56U) + 200,
                          f22 * 160.0f + 121.0f, func_150ADA68() < 0.7000000477f, 1.0f, 1.0f, 0, spC0, 1, 1,
                          arg0->unkC, arg0->unk1);
            s->unk10 -= 1.0f;
        } while (s->unk10 > 1.0f);
    }
}

void func_150C4AD8(struct260 *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x34) != 0) {
        func_1516972C(*(struct102 **)((u8 *)arg0 + 0x34));
    }
}

extern void func_150C4AD8(struct260 *);

void func_150C4B08(struct260 *arg0) {
    func_150C4AD8(arg0);
    func_1514933C(arg0);
}

extern void func_15149368(struct260 *);

void func_150C4B34(struct260 *arg0) {
    func_150C4AD8(arg0);
    func_15149368(arg0);
}

/* Event handler: events 0x55/0x56 clear/set flag 2 (suspend), others go to func_15149514. */
void func_150C4B60(s32 arg0, s32 arg1, u8 event) {
    s32 *p = &arg0;
    EffectData *data;

    if (event == 0x55) {
        data = (EffectData *)*p;
        data = (EffectData *)((s32)data + 0x28);
        data->flags = data->flags & 0xFFFD;
    } else if (event == 0x56) {
        data = (EffectData *)*p;
        data = (EffectData *)((s32)data + 0x28);
        data->flags = data->flags | 2;
    } else {
        data = (EffectData *)(*p + 0x28);
        func_15149514(arg1, event, (s32)data, (s32)&data->ownerId, *p);
    }
}
