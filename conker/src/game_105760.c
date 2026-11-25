#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* An actor's bone matrix: Actor.unk1D4 points at an array of these. */
typedef f32 MtxF[4][4];

void func_150A7960(MtxF, f32, f32, f32, f32 *, f32 *, f32 *);

void func_150D82B0(s32 arg0) {
}

extern s32 D_80090298[];

/* First byte of a Gfx command: its opcode, read as a signed byte. */
#define GFX_OPCODE(gfx) (*(s8 *)(gfx))

/* State passed to the display-list callbacks below (only the fields they use). */
typedef struct {
    u8 pad0[0x18];
    s16 unk18;
    u8 pad1A[0xA];
    Gfx **displayList;
    u8 pad28[0x10];
    s32 phase;
    s32 progress; /* percent, 0..100 */
} DLCallbackState;

/* While the actor runs action 13 with anim->unk8 in 23..40, scrolls the texture of the first
 * G_SETTILESIZE command in the display list horizontally. */
s32 func_150D82BC(DLCallbackState *state, Actor *actor) {
    struct197 *anim = actor->unk2D0;
    Gfx *list;
    s32 i;
    s32 uls;
    s32 w0;
    f32 t;

    if (anim == NULL) {
        return 0;
    }
    state->unk18 = D_80090298[6];
    if (actor->unk84.uh == 13) {
        if (23.0f <= anim->unk8 && anim->unk8 <= 40.0f) {
            state->unk18 = D_80090298[7];
            t = anim->unk8 - 23.0f;
            t *= 1.0f / 17.0f;
            list = *state->displayList;
            for (i = 0; GFX_OPCODE(&list[i]) != (s8)G_SETTILESIZE; i++) {
            }
            w0 = list[i].words.w0;
            uls = 70.0f * t + 2.0f;
            if (uls < 0) {
                uls += (w0 >> 12) & 0xFFF;
            }
            /* rewrite only w0 (uls, ult); the tile and lrs/lrt in w1 stay */
            list[i].words.w0 = _SHIFTL(G_SETTILESIZE, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(2, 0, 12);
        }
    }
    return 0;
}

/* Scrolls the first G_SETTILESIZE command's texture vertically by a progress value that rises
 * to 100% once action 0x18D reaches anim->unk8 46, then falls to 0 once action 0x18E reaches 34. */
s32 func_150D83D8(DLCallbackState *state, Actor *actor) {
    Gfx *list;
    s32 i;
    f32 t;

    switch (state->phase) {
        case 0:
            state->progress = 50;
            state->unk18 = D_80090298[27];
            if (actor->unk84.uh == 0x18D && 46.0f <= actor->unk2D0->unk8) {
                state->phase = 1;
                state->unk18 = D_80090298[28];
            }
            break;
        case 1:
            state->progress += D_800BE9E4;
            if (state->progress >= 100) {
                state->progress = 100;
                state->phase = 2;
            }
            break;
        case 2:
            if (actor->unk84.uh == 0x18E && 34.0f <= actor->unk2D0->unk8) {
                state->phase = 3;
            }
            break;
        case 3:
            state->progress -= D_800BE9E4 * 4;
            if (state->progress <= 0) {
                state->progress = 0;
                state->phase = 4;
            }
            break;
    }
    t = state->progress * 0.01f;
    list = *state->displayList;
    for (i = 0; GFX_OPCODE(&list[i]) != (s8)G_SETTILESIZE; i++) {
    }
    list[i].words.w0 = _SHIFTL(G_SETTILESIZE, 24, 8) | _SHIFTL(2, 12, 12) | _SHIFTL((s32)(120.0f * t + 2.0f), 0, 12);
    return 0;
}

/* Writes the two-halfword command { 0x42, 0 } and returns the position after it. */
s16 *func_150D8590(s16 *cmd, s32 arg1) {
    cmd[0] = 0x42;
    cmd[1] = 0;
    return cmd + 2;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

/* Ground probe result filled by func_15046C80 (size 0x24). */
typedef struct {
    f32 height;
    s16 points[9];
    s32 unk18;
    u8 flags;
    u8 unk1D;
    s32 unk20;
} GroundHit;

/* Particle burst description passed to func_15153634. */
typedef struct {
    s16 count;
    s16 countRange;
    u8 kind;
    u8 pad5;
    s16 unk6;
    s32 flags;
    s32 unkC;
    s16 timer;
    s16 timerRange;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    Vec3f position;
    s16 yaw;
    s16 pitch;
    s16 yawRange;
    s16 pitchRange;
    f32 speed;
    f32 speedRange;
    f32 unk54;
    f32 unk58;
    s32 unk5C;
    u8 unk60;
    s8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 pad65[3];
    f32 unk68;
} ParticleBurst;

void func_1504715C(GroundHit *, Actor *);
s32 func_15046C80(Vec3f *, s32, f32, GroundHit *);
void func_150E7FEC(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32, s32, s32);
void func_150E83AC(Vec3f *, s16, u8, s32);
void func_15153634(ParticleBurst *, u8, u8, s32);

/* Spawns impact effects at the actor's feet: a ground effect if the ground below is flagged,
 * then func_150E83AC's effect and a particle burst. */
void func_150D85AC(Actor *actor) {
    Vec3f pos;

    pos.x = actor->x_position;
    pos.y = actor->unk180;
    pos.z = actor->z_position;
    {
        Vec3f probe;
        GroundHit ground;

        probe.x = pos.x;
        probe.y = pos.y + 100.0f;
        probe.z = pos.z;
        func_1504715C(&ground, actor);
        {
            ParticleBurst burst;

            {
                union { u32 value; f64 align; } random; // matching: keeps the value in a stack slot, not a temp
                f32 randomScale;

                if (func_15046C80(&probe, 0, pos.y - 200.0f, &ground) != 0 && (ground.flags & 1)) {
                    pos.x = probe.x;
                    pos.y = ground.height;
                    pos.z = probe.z;
                    randomScale = func_150ADA68();
                    random.value = func_150ADA20();
                    func_150E7FEC(randomScale * 109.0f + 140.0f, (u8)((random.value % 86U) + 170),
                                  ground.points, &pos, (func_150ADA20() % 251U) + 500, 0, 1, 0, 0, 0, 255, 0);
                }
                func_150E83AC(&pos, (func_150ADA20() % 62U) + 120, 255, 1);
            }
            burst.count = 26;
            burst.countRange = 7;
            burst.kind = 0x6C;
            burst.unk6 = 0x5103;
            burst.flags = 0x200005;
            burst.unkC = 0;
            burst.timer = 30;
            burst.timerRange = 10;
            burst.unk14 = 0;
            burst.unk18 = 0;
            burst.unk1F = 255;
            burst.unk1C = 0xB9;
            burst.unk1D = 0xC7;
            burst.unk1E = 0xC4;
            burst.unk20 = 0x95;
            burst.unk21 = 0x91;
            burst.unk22 = 0x7E;
            burst.unk23 = 100;
            burst.unk24 = 0x9B;
            burst.unk25 = 255;
            burst.unk26 = 30;
            burst.unk28 = 8;
            burst.unk2A = 30;
            burst.unk2C = 1.022876024f;
            burst.unk30 = 150.0f;
            burst.unk34 = 298.0f;
            burst.position.x = pos.x;
            burst.position.y = pos.y + 35.0f;
            burst.position.z = pos.z;
            burst.yaw = 0;
            burst.pitch = -16;
            burst.yawRange = 255;
            burst.pitchRange = 20;
            burst.speed = 18.0f;
            burst.speedRange = 18.0f;
            burst.unk54 = 0.09300000221f;
            burst.unk58 = 0.302000016f;
            burst.unk5C = 0x840E07;
            burst.unk60 = 16;
            burst.unk61 = -1;
            burst.unk62 = 8;
            burst.unk63 = 6;
            burst.unk64 = 1;
            burst.unk68 = 0.9054819942f;
            func_15153634(&burst, 255, 255, 1);
        }
    }
}

struct S150D88AC {
    u8 pad0[0x14];
    u8 *unk14;
    u8 *unk18;
};

/* Sets unk14[9] to whether unk18[0x6F] is clear. */
s32 func_150D88AC(struct S150D88AC *arg0) {
    if (arg0->unk18[0x6F] != 0) {
        arg0->unk14[9] = 0;
    } else {
        arg0->unk14[9] = 1;
    }
    return 1;
}

/* Writes to *out the world position of a fixed offset from one of the actor's bones, chosen by
 * point (3..6). */
s32 func_150D88E0(Vec3f *out, Actor *actor, u8 point) {
    s32 bone;
    f32 x;
    f32 y;
    f32 z;

    switch (point) {
        case 3:
            bone = 18;
            x = -7.0f;
            z = 30.0f;
            break;
        case 4:
            bone = 21;
            x = -4.0f;
            z = 35.0f;
            break;
        case 5:
            bone = 15;
            x = 5.0f;
            z = 20.0f;
            break;
        case 6:
            bone = 24;
            x = 5.0f;
            z = 31.0f;
            break;
    }
    y = 0.0f;
    func_150A7960(((MtxF *)actor->unk1D4)[bone], x, y, z, &x, &y, &z);
    out->x = x;
    out->y = y;
    out->z = z;
    return 1;
}

s32 func_150D8A20(s32 arg0, s32 arg1) {
    return 8;
}

void func_151875E0(f32, f32, f32, s32, s32, s32, f32, f32);
void func_15165F80(s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Spawns two effects at bone point `point` of the actor (see func_150D88E0), if it has bones. */
void func_150D8A34(Actor *actor, s32 point, s32 arg2) {
    Vec3f pos;

    if (actor->unk1D4 != NULL) {
        func_150D88E0(&pos, actor, point);
        func_151875E0(pos.x, pos.y, pos.z, 30, 15, 7, 0.1000000015f, 0.02999999933f);
        func_15165F80(-1, pos.x, actor->unk180 + 4.0f, pos.z, 4, 50, 0, 255, 0);
    }
}
