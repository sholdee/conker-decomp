#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A1970;
extern f32 D_800A1980;
extern f32 D_800A1984;
extern f32 D_800A1950[];

s32 func_1509BE40();
void func_15143134(f32 *, f32 *, Mtx *);

/* State the emitter object keeps at +0x28: func_150F2C8C fills it, func_150F2D14 updates it each frame. */
typedef struct {
    /* 0x00 */ struct127 *target;
    /* 0x04 */ u8 targetId;   // target->unique_id when attached; a mismatch means the slot was reused
    /* 0x05 */ u8 prevUnk222; // target->unk222 seen last frame
    /* 0x08 */ f32 timer;     // one particle is emitted per 1.0 accumulated
    /* 0x0C */ s8 count;      // particles left in the current burst
} EmitterState;

/* The emitter object spawned by func_150F2C8C. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE; // -1 when the target is gone
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ EmitterState state;
} Emitter;

/* State each emitted particle keeps at +0x110 (copied in by func_150F2D14, updated by func_150F3214). */
typedef struct {
    /* 0x00 */ struct127 *target;
    /* 0x04 */ u8 targetId;
    /* 0x08 */ f32 age;
    /* 0x0C */ f32 lifetime;
    /* 0x10 */ f32 growth;   // particle size = growth * sqrtf(age)
    /* 0x14 */ f32 fadeStart; // particle->unk5C = fadeStart - fadeRate * age * age
    /* 0x18 */ f32 fadeRate;  // fadeStart / (lifetime * lifetime): unk5C reaches 0 at lifetime
    /* 0x1C */ struct17 unk1C;
    /* 0x28 */ struct17 unk28;
} ParticleState;

/* A particle spawned by func_1513D2F0. */
typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ f32 unk30;
    /* 0x034 */ f32 unk34[3];
    /* 0x040 */ u8 pad40[0x1C];
    /* 0x05C */ u8 unk5C;
    /* 0x05D */ u8 pad5D[0xB3];
    /* 0x110 */ ParticleState state;
} Particle;

extern struct {
    u8 pad0[0x120];
    u8 unk120;
} *D_800CC5EC;
extern s32 *D_800DBF94;
extern u8 D_800CC40C;
extern u8 D_800CC49A;
struct131 *func_151149AC(s32);
void func_15136C3C(struct127 *, s32, s32, s32, s32, s32, s32, s32);
void func_1507CD64(struct127 *, s32);
void func_15196318(s32, s32, s32);

/* While the player has unk31C, polls entries 0xFA and 0xF9 and resets the player when one has bit 0 set,
 * then scrolls the four D_800BE4F0 layers by object 0xE's unk2E4. */
void func_150F2A60(s32 arg0) {
    s32 i;
    s32 scroll;
    struct127 *obj;
    struct131 *entry;
    s32 entryIndex;

    if (D_800CC2D0->unk31C != NULL) {
        if (D_800CC5EC->unk120 == 0) {
            // matching: `| 0` hides the induction variable, so IDO keeps `slti` instead of testing i != 2
            for (i = 0; i < 2; i = (i + 1) | 0) {
                entry = func_151149AC((0xFA - i) & 0xFF);
                entryIndex = entry - D_800DBEF4;
                if (D_800DBF94[entryIndex] & 1) {
                    if (D_800CC40C != 0) {
                        func_15060F28(&D_800CC2D0[D_800CC40C - 100], 0);
                    }
                    D_800CC49A = 0;
                    func_15136C3C(D_800CC2D0, 0, 0, 1, 1, 0, 0xFF, 1);
                    func_10010630(0x627, D_800CC2D0, 0x6D60, 500, 1000);
                    func_15145A50(D_800CC2D0);
                    func_1507CD64(D_800CC2D0, 6);
                    break;
                }
            }
            obj = func_15083E90(0xE);
            if (obj != NULL) {
                scroll = obj->unk2E4;
            } else {
                scroll = 0;
            }
            func_15196318(D_800BE4F0->unk0, scroll, 0);
            func_15196318(D_800BE4F0->unk4, 0, -scroll);
            func_15196318(D_800BE4F0->unk8, scroll, 0);
            func_15196318(D_800BE4F0->unkC, -scroll, 0);
        }
    }
}

/* Spawns an emitter object that follows `target` (see func_150F2D14). */
void func_150F2C8C(struct127 *target) {
    EmitterState state;
    Emitter *emitter;

    state.target = target;
    state.targetId = target->unique_id;
    state.prevUnk222 = 0;
    state.timer = 0.0f;
    state.count = 0;

    emitter = (Emitter *)func_15149130(300, -1, 0x5C, -1, 0, 0x44, (struct37 *)0x10, 0xFF, 1);
    if (emitter != NULL) {
        memcpy(&emitter->state, &state, sizeof(state));
    }
}

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    struct17 unk1C;
    struct17 unk28;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    u8 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
} ParticleSpawn;

extern f32 D_800A195C[];
void func_1502EA98(struct127 *, s32, s32, s32, s32, s32, s32);
void func_15145EA4(f32 **, f32 **, Mtx *, s32);
extern s32 func_15146078(struct17 *, struct17 *, struct17 *);

/* Emitter update: while the target lives, emits a burst of 3 particles from its bone 15 each time its unk222
 * changes to a nonzero value. */
void func_150F2D14(Emitter *emitter) {
    EmitterState *state;
    struct127 *target;
    ParticleSpawn spawn;
    ParticleState particle;
    Particle *spawned;

    state = &emitter->state;
    if (D_800C35EA == 1) {
        return;
    }
    target = state->target;
    if (target->interaction_state == 0 || target->unique_id != state->targetId) {
        emitter->unkE = -1;
        return;
    }
    if (state->prevUnk222 != target->unk222 && target->unk222 != 0) {
        state->count = 3;
        state->timer = 0.0f;
    }
    state->prevUnk222 = target->unk222;
    if (target->unk222 != 0) {
        func_1502EA98(&D_800CC2D0[target->unk222], 0xFF, 0, 0, 0x50, 0, 0xF);
    }
    if (target->unk1D4 != NULL && state->count > 0) {
        state->timer += 0.148f * D_800BE9A4;
        if (state->timer > 1.0f) do {
            particle.target = target;
            particle.targetId = target->unique_id;
            particle.age = 0.0f;
            particle.lifetime = func_150ADA68() * 6.0f + 13.0f;
            particle.growth = 58.14f;
            particle.fadeStart = func_150ADA68() * 40.0f + 60.0f;
            particle.fadeRate = particle.fadeStart / (particle.lifetime * particle.lifetime);
            {
                f32 *src[2];
                f32 *dst[2];
                struct17 dir;

                src[0] = D_800A1950;
                src[1] = D_800A195C;
                dst[0] = &spawn.unk1C.unk0;
                dst[1] = &dir.unk0;
                func_15145EA4(src, dst, (Mtx *)target->unk1D4 + 15, 2);
            dir.unk0 -= spawn.unk1C.unk0;
            dir.unk4 -= spawn.unk1C.unk4;
            dir.unk8 -= spawn.unk1C.unk8;
            if (!func_15146078(&dir, &particle.unk1C, &particle.unk28)) {
                return;
            }
            spawn.unk44 = particle.fadeStart;
            spawn.unk0 = 0x79;
            spawn.unk1 = 0xC;
            spawn.unk2 = 0x4405;
            spawn.unk4 = 300;
            spawn.unk8 = 0;
            spawn.unkC = 0;
            spawn.unk10 = 0xD5;
            spawn.unk11 = 0xDB;
            spawn.unk12 = 0xFF;
            spawn.unk13 = 0xFF;
            spawn.unk18 = 0.0f;
            spawn.unk14 = 0.0f;
            spawn.unk28 = *(struct17 *)&D_800A5480;
            spawn.unk34 = 1.0f;
            spawn.unk38 = 1.0f;
            spawn.unk3C = 1.0f;
            spawn.unk40 = 0x04EC0000;
            spawn.unk45 = 0xFF;
            spawn.unk46 = 0;
            spawn.unk47 = 6;
            spawn.unk48 = 0;
            spawn.unk4C = 0xFF;
            spawn.unk50 = 0;
            spawn.unk54 = 1;
            spawn.unk56 = 0xFF;
            spawned = func_1513D2F0(&spawn, (s32)&D_800A4AA0, 0x26, 0, 0, 0x25, 0, 0, 0, 0x34, emitter->unkC, emitter->unk1);
            if (spawned != NULL) {
                memcpy(&spawned->state, &particle, sizeof(particle));
            }
            state->count--;
            state->timer -= 1.0f;
            }
        } while (state->timer > 1.0f && state->count > 0);
    }
}

void func_150F3194(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

void func_150F31D4(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x110, arg0 + 0x114, arg0);
}

/* Particle update: follows the target's bone 15, grows and fades with age. Returns 0 when it should die. */
s32 func_150F3214(Particle *particle) {
    ParticleState *state;
    struct127 *target;
    f32 size;

    state = &particle->state;
    target = state->target;

    if (target->interaction_state == 0) {
        goto return_zero;
    }

    do {
        if (target->unique_id != state->targetId) {
            goto return_zero;
        }

        if (target->unk1D4 != NULL) {
            break;
        }
return_zero:
        return 0;
    } while (0);

    func_15143134(D_800A1950, particle->unk34, (Mtx *)target->unk1D4 + 15);

    size = sqrtf(state->age) * state->growth;
    particle->unk30 = size;
    particle->unk2C = size;

    particle->unk5C = (u32)(state->fadeStart - ((state->fadeRate * state->age) * state->age));
    state->age = state->age + D_800BE9A4;

    if (state->lifetime < state->age) {
        return 0;
    }
    return 1;
}

void func_150F337C(s32 arg0, s16 arg1) {
    func_15140410(arg0, arg0 + 0x12C, arg0 + 0x138, arg1);
}

void func_150F33B0(struct131 *arg0) {
    if (D_800DBFF0->unk300 < -2000.0f) {
        *(u8*)&arg0->unk4F &= ~1;
        return;
    }

    *(u8*)&arg0->unk4F = (*(u8*)&arg0->unk4F & 0xFF) | 1;
}

void func_150F33F8(s32 arg0) {
    s16 *temp_v0_2;

    if (D_800DBFF0[arg0].unk300 < D_800A1970) {
        if (D_800DBFF0[arg0].unk2FC > 840.0f) {
            temp_v0_2 = (s16 *)D_800D9A40;
            *temp_v0_2 |= 1 << arg0;
            return;
        }
        temp_v0_2 = (s16 *)D_800D9A40;
        *temp_v0_2 &= ~(1 << arg0);
    }
}

f32 func_150F34A0(s32 arg0, f32 arg1) {
    f32 var_f2;

    if (arg1 < -5.0f) {
        var_f2 = (arg1 * -0.01f) + 0.7f;
    } else {
        var_f2 = 0.75f;
    }

    return var_f2;
}

typedef struct {
    /* 0x0 */ u16 buttons;
    /* 0x2 */ s8 stick_x;
    /* 0x3 */ s8 stick_y;
} ContPad;

extern u8 D_800CC26D;
extern u8 D_800CBDD3;
void func_150585F0(struct127 *arg0, f32 arg1);
void func_15052590(struct127 *arg0);
u8 func_150599C8(struct127 *arg0, u8 arg1, u16 arg2);
f32 func_1505A3A8(f32 arg0, void *arg1, f32 arg2, f32 arg3, u8 arg4);
void func_15073FA0(void);

#define PAD ((ContPad *)D_800CC284)

void func_150F34F4(struct127 *arg0) {
    u16 angle;
    f32 mag;
    f32 accel;
    f32 temp;
    f32 pitch;
    f32 speed;
    f32 animSpeed;
    s32 anim;
    s32 count;
    f32 floorLevel;
    s32 buttons;
    s32 i;
    f32 dist;
    f32 minDist;
    struct127 *obj;
    s32 idx;

    floorLevel = 0.0f;
    arg0->unkAB = 1;
    arg0->unk80 = 1;
    arg0->unk222 = 0;
    D_800CC288 = D_800BE710[0];
    D_800CC284 = *(s32 *)D_800BE728[0];
    if ((D_800C3E78 <= D_80082FA0) && ((&D_800DDE3C)[D_800C3E78] != 0) && ((&D_800DDDC8)[D_800C3E78] > 0.75f) && (D_800E0B94 != 2)) {
        PAD->stick_x = 0;
        PAD->stick_y = 0;
        PAD->buttons = 0;
        D_800CC288 = 0;
    }
    arg0->unkF8 |= 0x40;
    arg0->unk1CC = arg0->y_position;
    if (arg0->unk103 != 0) {
        arg0->unk103--;
    }
    if (arg0->stunned != 0) {
        func_150585F0(arg0, 0.25f);
        func_15059140(arg0);
        arg0->unk40 = ((s16)arg0->unk7A + 0x4000) * 0.0054931640625f;
        if (arg0->unk13C != 0) {
            struct127 *rider;

            idx = arg0->unk13C - 100;
            rider = &D_800CC2D0[idx];
            rider->unk13D = 0;
            rider->xz_velocity = arg0->xz_velocity;
            rider->unk232 = 0x21;
            rider->unk218 = 0;
            rider->stunned = 0;
            rider->unkF8 &= ~0x400;
            arg0->unk13C = 0;
        }
        return;
    }
    if (arg0->unk102 == 0) {
        arg0->in_water = 0;
        angle = func_1505A630(PAD->stick_x, PAD->stick_y, 0);
        mag = func_1505A5CC((struct49 *)D_800CC284);
        if (PAD->buttons & 0x10) {
            mag = 0.0f;
        }
        angle += D_800CC280;
        arg0->unk78 = arg0->unk7A;
        if (mag > 1.0f) {
            arg0->unk78 = angle;
        }
        arg0->unk44 = mag * 0.35f;
        arg0->unk232 = 7;
        arg0->unk218 = 0;
        if ((D_800CC288 & 0xC000) || (arg0->unk28 > 100.0f)) {
            arg0->unk232 = 8;
            arg0->unk102 = 1;
        }
        func_15052590(arg0);
        if (D_800CC26D != 0) {
            if (arg0->unk28 > 30.0f) {
                D_800D1580 = 0xFF010074;
                func_1506E8D8();
                func_1505959C(&D_800CC2D0[D_800CC26D - 100], D_800C3E78);
                arg0->unk13C = D_800CC26D;
                arg0->unk102 = 1;
            }
            arg0->y_velocity = 23.0f;
        }
        if (arg0->y_position < arg0->unk118) {
            arg0->unk102 = 1;
            arg0->unk86 = 1;
            arg0->y_velocity = -6.0f;
        }
        return;
    }
    speed = arg0->xz_velocity;
    if (speed > 60.0f) {
        speed = 60.0f;
    }
    pitch = PAD->stick_y * 1.25f;
    buttons = PAD->buttons & 0x10;
    if (buttons) {
        pitch = 0.0f;
    }
    if (arg0->unk13C != 0) {
        if (!buttons) {
            arg0->y_position -= 16.0f * D_800D1550[0];
        }
    } else {
        minDist = 1000.0f;
        for (i = 0; i < 25; i++) {
            if ((D_800CC2D0[i].interaction_state != 0) && (D_800CC2D0[i].health != 0) &&
                (D_800CC2D0[i].unk28 == 0.0f) && (D_800CC2D0[i].unk232 != 0x21) &&
                ((D_800CC2D0[i].id == 0x9C) || (D_800CC2D0[i].id == 0x9D))) {
                dist = func_1505A6F8(D_800D154C, &D_800CC2D0[i]);
                if (dist < minDist) {
                    arg0->unk222 = i;
                    minDist = dist;
                }
            }
        }
    }
    if (pitch < -90.0f) {
        pitch = -90.0f;
    }
    if (pitch > 90.0f) {
        pitch = 90.0f;
    }
    if (arg0->y_position > 2100.0f) {
        if (arg0->y_velocity > -30.0f) {
            arg0->y_velocity -= 1.5f;
        }
        pitch = 40.0f;
    }
    temp = PAD->stick_x;
    arg0->unkC4 += (temp - arg0->unkC4) * (0.2f * D_800D1550[0]);
    arg0->unk76 -= (s16)(arg0->unkC4 * D_800D1550[0] * 0.04f * ((100.0f - speed) * 0.015f) * 220.0f);
    arg0->unk7E = 0x19;
    func_150599C8(arg0, 8, arg0->unk76);
    arg0->target_speed = 0.0f;
    speed = 0.0f;
    accel = 1.3f;
    if (arg0->y_position < arg0->unk118) {
        if (arg0->unk86 == 0) {
            arg0->unk86 = 1;
            arg0->xz_velocity = 0.0f;
            arg0->y_velocity = -6.0f;
        }
    } else if (arg0->unk86 != 0) {
        arg0->unk86 = 0;
        arg0->y_velocity = 9.0f;
    }
    if (!(PAD->buttons & 0x10)) {
        if (PAD->buttons & 0x8000) {
            arg0->target_speed = -22.0f;
            speed = 1.0f;
            accel = 1.8f;
        }
        if (PAD->buttons & 0x4000) {
            arg0->target_speed = 30.0f;
            if (arg0->y_velocity < 0.0f) {
                arg0->target_speed -= arg0->y_velocity * 0.6f;
            }
            speed = 1.5f;
            accel = 0.3f;
        }
    }
    func_1505A3A8(arg0->target_speed, arg0, speed, accel, 1);
    arg0->unk40 = ((s16)arg0->unk7A + 0x4000) * 0.0054931640625f;
    arg0->gravity = pitch * 0.000125f * (200.0f - arg0->xz_velocity);
    if (arg0->unk13C == 0) {
        if (arg0->y_velocity > 26.0f) {
            arg0->y_velocity = 26.0f;
        }
        if (arg0->y_velocity < -34.0f) {
            arg0->y_velocity = -34.0f;
        }
    }
    temp = fabsf(pitch * 0.5f);
    if (arg0->unk86 != 0) {
        temp = temp * 0.4f;
        arg0->gravity *= 0.6f;
    }
    if (arg0->gravity == 0.0f) {
        if (fabsf(arg0->y_velocity) < 1.5f) {
            arg0->y_velocity = 0.0f;
        } else if (arg0->y_velocity > 0.0f) {
            arg0->gravity = 1.6f;
        } else {
            arg0->gravity = -1.6f;
        }
    } else if ((arg0->y_velocity > temp * 1.2f) && (arg0->gravity < 0.0f)) {
        arg0->gravity = 0.0f;
    } else if ((arg0->y_velocity < -temp * 1.5f) && (arg0->gravity > 0.0f)) {
        arg0->gravity = 0.0f;
    }
    arg0->in_water = 10;
    if (PAD->buttons & 0x10) {
        arg0->unkB8 += (PAD->stick_y - arg0->unkB8) * 0.2f;
        arg0->y_velocity = 0.0f;
        arg0->gravity = 0.0f;
    } else {
        arg0->unkB8 = (arg0->y_velocity - floorLevel) * -2.0f;
        if (arg0->unkB8 > 40.0f) {
            arg0->unkB8 = 40.0f;
        } else if (arg0->unkB8 < -55.0f) {
            arg0->unkB8 = -55.0f;
        }
    }
    speed = arg0->y_velocity;
    D_800CBDD3 = 1;
    func_15059140(arg0);
    D_800CBDD3 = 0;
    if ((arg0->unk13C != 0) && (arg0->unk28 < 50.0f)) {
        floorLevel = 50.0f;
    }
    if (!(PAD->buttons & 0x10)) {
        if ((D_800CC26D != 0) && (arg0->unk13C == 0) && (D_800CC2D0[D_800CC26D - 100].unk28 == 0.0f)) {
            arg0->y_position = D_800CC2D0[D_800CC26D - 100].y_position + 70.0f;
            arg0->y_velocity = 20.0f;
            func_1505959C(&D_800CC2D0[D_800CC26D - 100], D_800C3E78);
            arg0->unk13C = D_800CC26D;
            D_800D1580 = 0xFF010074;
            func_1506E8D8();
        }
        if (arg0->unk13C != 0) {
            if (arg0->unk25C & 2) {
                struct127 *rider;

                arg0->unk25C &= ~2;
                idx = arg0->unk13C - 100;
                rider = &D_800CC2D0[idx];
                rider->unk13D = 0;
                rider->xz_velocity = arg0->xz_velocity;
                rider->unk232 = 0x21;
                rider->unk218 = 0;
                rider->stunned = 0;
                rider->unkF8 &= ~0x400;
                arg0->unk13C = 0;
                arg0->y_velocity = 0.0f;
                D_800D1580 = 0x76;
                func_1506E8D8();
            }
        } else if ((D_800CC288 & 0x2000) && (arg0->unk103 == 0)) {
            count = 0;
            if (D_800BE9F0 == 0x3C) {
                obj = &D_800CC2D0[1]; do {
                    if ((obj->interaction_state != 0) && (obj->id == 0x25)) {
                        count++;
                    }
                    obj++;
                } while (obj != (struct127 *)&D_800D121C);
                if (count < 5) {
                    D_800D1580 = 6;
                    arg0->unk103 = 4;
                    func_15073FA0();
                }
            }
        }
    }
    if (arg0->unk28 <= floorLevel) {
        arg0->xz_velocity *= 0.6f;
        if ((PAD->buttons & 0x4000) || (arg0->unk86 != 0) || (arg0->unk13C != 0)) {
            if (arg0->unk13C != 0) {
                if (arg0->y_velocity < -10.0f) {
                    D_800D1580 = 0xFF0100A7;
                    func_1506E5FC();
                    D_800D1580 = 0xFF060372;
                    func_1506E8D8();
                }
            } else if (arg0->y_position < 0.0f) {
                D_800D1580 = 0xFF010072;
                func_1506E8D8();
            }
            if (arg0->unk13C == 0) {
                arg0->y_velocity = 20.0f;
            } else if (arg0->unk86 == 0) {
                arg0->y_velocity = 38.0f;
            }
            pitch = -80.0f;
        } else {
            arg0->y_velocity = speed * -1.0f;
            arg0->unk102 = 0;
            arg0->unk28 = 0.1f;
            arg0->unkB8 = 0.0f;
        }
    }
    if (!(PAD->buttons & 0x10)) {
        arg0->unkB8 += 20.0f;
    } else {
        arg0->y_velocity = arg0->unkB8 * -0.35f;
    }
    if (arg0->unk86 != 0) {
        arg0->unk83 = 0;
        if ((fabsf(pitch) > 8.0f) || (arg0->target_speed > 7.0f)) {
            animSpeed = 1.0f;
        } else {
            animSpeed = 0.5f;
        }
    } else {
        animSpeed = func_150F34A0((s32)arg0, pitch);
    }
    anim = 0xF;
    if ((arg0->unkB8 > 40.0f) && (pitch > 20.0f)) {
        anim = 0x11;
    }
    if ((arg0->xz_velocity < 20.0f) && (arg0->target_speed <= 0.0f) && (pitch == 0.0f)) {
        anim = 0x18;
    }
    if (arg0->xz_velocity < 0) {
        anim = 0x1F;
    }
    if (arg0->unk13C != 0) {
        anim = 0x17;
        animSpeed += 0.2f;
    }
    func_1505E650(arg0, anim, animSpeed, 9.0f, 0.0f, 0.0f, 0);
}

void func_150F43F0(struct108 *arg0) {
    if (arg0->unk23E == 0x3B) {
        func_1509BFB0(0, 0x405C, 1);
        if (arg0->unk2C == 0x100) {
            goto set_zero;
        }
        if (arg0->unk6C8 == 0) {
            if (func_15123934(arg0, 8, 0, 0, 0) != 0) {
                arg0->unk84 |= 0x300000;
                arg0->unk84 &= -5;
                arg0->unk1B4 = 1;
                arg0->unk1E0 = 3;
                func_15124B18(arg0);
            }
            arg0->unk134 = 0;
            arg0->unk348 = 125.0f;
            arg0->unk34C = 125.0f;
            arg0->unk374 = 220.0f;
            arg0->unk190 = 30.0f;
        } else {
set_zero:
            arg0->unk190 = 0.0f;
        }
    } else if (arg0->unk2C == 8) {
        if (arg0->unk6C8 == 0) {
            func_151239CC(arg0, 0);
            func_1509BFB0(0, 0x405C, 0);
        }
    }

    if ((func_1509BE40(1, 0x4054, 6, 0x9000) != 0) && (func_1509BE40(1, 0x405E, 6, 0x9000) == 0)) {
        arg0->unk84 |= (s32)0x80000000;
    } else {
        arg0->unk84 &= 0x7FFFFFFF;
    }
}
