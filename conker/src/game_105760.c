#include <ultra64.h>
/* functions.h types func_150ADA20 as returning u8; golden spills/uses the full u32 return
 * (same shadow-prototype idiom as game_121A20.c). */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

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
s32 func_150D82BC(DLCallbackState *state, struct127 *actor) {
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
s32 func_150D83D8(DLCallbackState *state, struct127 *actor) {
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
} Vec3F_150D85AC;

typedef struct {
    f32 unk0;
    u8 unk4[0x18];
    u8 unk1C;
    u8 unk1D;
    s32 unk20;
} Ground_150D85AC;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 pad5;
    s16 unk6;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
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
    Vec3F_150D85AC unk38;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
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
} Struct150D85AC;

void func_1504715C(Ground_150D85AC *, struct127 *);
s32 func_15046C80(Vec3F_150D85AC *, s32, f32, Ground_150D85AC *);
void func_150E7FEC(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32, s32, s32);
void func_150E83AC(Vec3F_150D85AC *, s16, u8, s32);
void func_15153634(Struct150D85AC *, u8, u8, s32);

void func_150D85AC(struct127 *arg0) {
    Vec3F_150D85AC pos;

    pos.x = arg0->x_position;
    pos.y = arg0->unk180;
    pos.z = arg0->z_position;
    {
        Vec3F_150D85AC probe;
        Ground_150D85AC ground;

        probe.x = pos.x;
        probe.y = pos.y + 100.0f;
        probe.z = pos.z;
        func_1504715C(&ground, arg0);
        {
            Struct150D85AC sp48;

            {
                union { u32 w; f64 _a; } r1;
                f32 sp3C;

                if (func_15046C80(&probe, 0, pos.y - 200.0f, &ground) != 0 && (ground.unk1C & 1)) {
                    pos.x = probe.x;
                    pos.y = ground.unk0;
                    pos.z = probe.z;
                    sp3C = func_150ADA68();
                    r1.w = func_150ADA20();
                    func_150E7FEC(sp3C * 109.0f + 140.0f, (u8)((r1.w % 86U) + 170), ground.unk4, &pos,
                                  (func_150ADA20() % 251U) + 500, 0, 1, 0, 0, 0, 0xFF, 0);
                }
                func_150E83AC(&pos, (func_150ADA20() % 62U) + 120, 0xFF, 1);
            }
            sp48.unk0 = 0x1A;
            sp48.unk2 = 7;
            sp48.unk4 = 0x6C;
            sp48.unk6 = 0x5103;
            sp48.unk8 = 0x200005;
            sp48.unkC = 0;
            sp48.unk10 = 0x1E;
            sp48.unk12 = 0xA;
            sp48.unk14 = 0;
            sp48.unk18 = 0;
            sp48.unk1F = 0xFF;
            sp48.unk1C = 0xB9;
            sp48.unk1D = 0xC7;
            sp48.unk1E = 0xC4;
            sp48.unk20 = 0x95;
            sp48.unk21 = 0x91;
            sp48.unk22 = 0x7E;
            sp48.unk23 = 0x64;
            sp48.unk24 = 0x9B;
            sp48.unk25 = 0xFF;
            sp48.unk26 = 0x1E;
            sp48.unk28 = 8;
            sp48.unk2A = 0x1E;
            sp48.unk2C = 1.022876024f;
            sp48.unk30 = 150.0f;
            sp48.unk34 = 298.0f;
            sp48.unk38.x = pos.x;
            sp48.unk38.y = pos.y + 35.0f;
            sp48.unk38.z = pos.z;
            sp48.unk44 = 0;
            sp48.unk46 = -0x10;
            sp48.unk48 = 0xFF;
            sp48.unk4A = 0x14;
            sp48.unk4C = 18.0f;
            sp48.unk50 = 18.0f;
            sp48.unk54 = 0.09300000221f;
            sp48.unk58 = 0.302000016f;
            sp48.unk5C = 0x840E07;
            sp48.unk60 = 0x10;
            sp48.unk61 = -1;
            sp48.unk62 = 8;
            sp48.unk63 = 6;
            sp48.unk64 = 1;
            sp48.unk68 = 0.9054819942f;
            func_15153634(&sp48, 0xFF, 0xFF, 1);
        }
    }
}

struct S150D88AC {
    u8 pad0[0x14];
    u8 *unk14;
    u8 *unk18;
};

s32 func_150D88AC(struct S150D88AC *arg0) {
    if (arg0->unk18[0x6F] != 0) {
        arg0->unk14[0x9] = 0;
    } else {
        arg0->unk14[0x9] = 1;
    }
    return 1;
}

s32 func_150D88E0(void *arg0, struct127 *arg1, u8 arg2) {
    struct {
        f32 z;
        f32 y;
        f32 x;
        s32 temp_a1;
    } sp30;

    switch (arg2) {
    case 3:
        sp30.temp_a1 = 0x12;
        sp30.x = -7.0f;
        sp30.z = 30.0f;
        break;
    case 4:
        sp30.temp_a1 = 0x15;
        sp30.x = -4.0f;
        sp30.z = 35.0f;
        break;
    case 5:
        sp30.temp_a1 = 0xF;
        sp30.x = 5.0f;
        sp30.z = 20.0f;
        break;
    case 6:
        sp30.temp_a1 = 0x18;
        sp30.x = 5.0f;
        sp30.z = 31.0f;
        break;
    }

    sp30.y = 0.0f;
    func_150A7960((f32 *)((u8 *)arg1->unk1D4 + (sp30.temp_a1 << 6)), sp30.x, sp30.y, sp30.z, &sp30.x, &sp30.y, &sp30.z);
    ((f32 *)arg0)[0] = sp30.x;
    ((f32 *)arg0)[1] = sp30.y;
    ((f32 *)arg0)[2] = sp30.z;
    return 1;
}

s32 func_150D8A20(s32 arg0, s32 arg1) {
    return 0x8;
}

extern f32 D_800A0B30;
extern f32 D_800A0B34;
s32 func_150D88E0(void *, struct127 *, u8);
void func_151875E0(f32, f32, f32, s32, s32, s32, f32, f32);
void func_15165F80(s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_150D8A34(struct127 *arg0, s32 arg1, s32 arg2) {
    struct { f32 x; f32 y; f32 z; } sp34;

    if (arg0->unk1D4 != NULL) {
        func_150D88E0(&sp34, arg0, (u8)arg1);
        func_151875E0(sp34.x, sp34.y, sp34.z, 0x1E, 0xF, 0x7, D_800A0B30, D_800A0B34);
        func_15165F80(-1, (s32)sp34.x, (s32)(arg0->unk180 + 4.0f), (s32)sp34.z, 4, 0x32, 0, 0xFF, 0);
    }
}
