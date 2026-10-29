#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_100810/func_150D3360.s")

typedef struct {
    struct17 pos;
    f32 unkC;
    f32 unk10;
} Entry150D3A68;

typedef struct {
    u8 pad0[0x34];
    struct17 unk34;
    struct17 unk40;
    u8 pad4C[0xC];
    s32 unk58;
    u8 pad5C[0x170 - 0x5C];
    f32 unk170;
    f32 unk174;
} Obj150D3A68;

typedef struct {
    struct127 *unk0;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
    struct126 *unk8;
    Obj150D3A68 *unkC;
    Obj150D3A68 *unk10;
    f32 unk14;
    f32 unk18;
} State150D3A68;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    struct17 unk10;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x94 - 0x2F];
    Entry150D3A68 *unk94;
    State150D3A68 *unk98;
} Actor150D3A68;

s32 func_150D4AE0(struct17 *arg0, struct17 *arg1, struct127 *arg2, struct126 *arg3);
void func_150D4D58(struct17 *arg0, struct17 *arg1, f32 arg2, u8 arg3, s32 arg4);
extern f32 func_15143E64(struct17 *);
extern s32 func_1515C0F8(struct127 *, f32 **);

s32 func_150D3A68(Actor150D3A68 *arg0) {
    State150D3A68 *state;
    Entry150D3A68 *table;
    struct127 *obj;
    struct126 *s126;
    struct17 prev;
    struct17 diff;
    f32 dist;
    struct17 dir;
    f32 *sub;
    u8 old;
    s32 idx;

    state = arg0->unk98;
    table = arg0->unk94;
    obj = state->unk0;
    s126 = state->unk8;
    if (obj->interaction_state == 0 || obj->unique_id != state->unk4) {
        return 0;
    }
    if (D_800BE9E4 > 0) {
        prev = arg0->unk10;
        switch (func_150D4AE0(&arg0->unk10, &dir, obj, s126)) {
        case 0:
            if (state->unkC != NULL) {
                state->unkC->unk58 &= ~2;
            }
            if (state->unk10 != NULL) {
                state->unk10->unk58 &= ~2;
            }
            arg0->unk1E &= ~4;
            return 1;
        case 1:
            if (!(arg0->unk1E & 4)) {
                prev = arg0->unk10;
            }
            arg0->unk1E |= 4;
            if (state->unkC != NULL) {
                state->unkC->unk34 = arg0->unk10;
                state->unkC->unk40 = dir;
                state->unkC->unk58 &= ~4;
                state->unkC->unk58 |= 2;
            }
            if (state->unk10 != NULL) {
                state->unk10->unk34 = arg0->unk10;
                state->unk10->unk40 = dir;
                state->unk10->unk58 &= ~4;
                state->unk10->unk58 |= 2;
            }
            break;
        case 2:
            if (!(arg0->unk1E & 4)) {
                prev = arg0->unk10;
            }
            arg0->unk1E |= 4;
            if (state->unkC != NULL) {
                state->unkC->unk58 &= ~2;
            }
            if (state->unk10 != NULL) {
                state->unk10->unk58 &= ~2;
            }
            break;
        }
        diff.unk0 = arg0->unk10.unk0 - prev.unk0;
        diff.unk4 = arg0->unk10.unk4 - prev.unk4;
        diff.unk8 = arg0->unk10.unk8 - prev.unk8;
        dist = func_15143E64(&diff);
        {
            Entry150D3A68 *e = &table[arg0->unk2E];
        e->pos = arg0->unk10;
        e->unkC = dist;
        e->unk10 = 100.0f;
        arg0->unk2E++;
        if (arg0->unk2E == arg0->unk25) {
            arg0->unk2E = 0;
        }
        arg0->unk2C++;
        if (arg0->unk2E == arg0->unk2D) {
            arg0->unk2D++;
            if (arg0->unk2D == arg0->unk25) {
                arg0->unk2D = 0;
            }
            arg0->unk2C--;
        }
        }
        if (obj->unk31C != NULL) {
            sub = (state->unk10 != NULL) ? &state->unk10->unk170 : NULL;
            old = state->unk5;
            if (obj->unk31C->unk84 != 0) {
                if (obj->unk44 > 6.0f) {
                    state->unk5 = 0;
                } else if (obj->unk44 > 3.0f) {
                    state->unk5 = 1;
                } else {
                    state->unk5 = 2;
                }
            } else {
                idx = (u8)(obj - D_800CC2D0);
                if (idx >= 4) {
                    state->unk5 = 1;
                } else {
                                        if (((s8 *)((*(void ***)&D_800BE728)[idx]))[3] > 20) {
                        state->unk5 = 0;
                    } else if (((s8 *)((*(void ***)&D_800BE728)[idx]))[3] < -20) {
                        state->unk5 = 2;
                    } else {
                        state->unk5 = 1;
                    }
                }
            }
            if (state->unk5 == 0) {
                if (sub != NULL) {
                    sub[1] = 2.099999905f;
                }
                state->unk18 = 50.0f;
                if (old != state->unk5) {
                    f32 *sp3C;

                    if (func_1515C0F8(obj, &sp3C) == 0) {
                        sp3C = &D_800A5480;
                    }
                    func_150D4D58(&arg0->unk10, (struct17 *)sp3C, obj->unk40, arg0->unkC, arg0->unk1);
                }
            } else if (state->unk5 == 2) {
                if (sub != NULL) {
                    sub[1] = 0.200000003f;
                }
                state->unk18 = 10.0f;
                if (old != state->unk5) {
                }
            } else {
                state->unk18 = 20.0f;
                if (sub != NULL) {
                    sub[1] = 1.0f;
                }
            }
        }
    }
    state->unk14 = state->unk14 + (state->unk18 - state->unk14) * 0.3500000238f;
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_100810/func_150D3FD4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_100810/func_150D4300.s")

typedef struct {
    struct127 *unk0;
    u8 unk4;
    u8 pad5[3];
    struct126 *unk8;
} SubA_150D49C0;

typedef struct {
    struct127 *unk0;
    union {
        struct127 *w4;
        u8 b4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB_150D49C0;

void func_150D49C0(void *arg0, s32 arg1, u8 arg2) {
    SubA_150D49C0 *temp_v1 = *(SubA_150D49C0 **)((u8 *)arg0 + 0x98);
    ArgB_150D49C0 *b = (ArgB_150D49C0 *)arg1;
    struct127 *temp;

    if (arg2 == 0 || arg2 == 0x2E || arg2 == 3) {
        temp = b->unk0;
        if ((temp == temp_v1->unk0) || (b->u4.b4 == temp_v1->unk4)) {
            func_1516972C(arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v1->unk0 == b->unk0) {
            temp_v1->unk0 = b->u4.w4;
            temp_v1->unk4 = b->unk9;
            temp_v1->unk8 = func_1503195C(temp_v1->unk0, (temp_v1->unk0->id == 0x48) ? 0x7A : 0x79, 0);
        } else if (temp_v1->unk0 == b->u4.w4) {
            temp_v1->unk0 = b->unk0;
            temp_v1->unk4 = b->unk8;
            temp_v1->unk8 = func_1503195C(temp_v1->unk0, (temp_v1->unk0->id == 0x48) ? 0x7A : 0x79, 0);
        }
    }
}

extern s32 func_1514654C(struct127 *, struct126 *, s32, void *, void *, s32);
extern f32 D_800A09D0;
extern f32 D_800A09DC;

s32 func_150D4AE0(struct17 *arg0, struct17 *arg1, struct127 *arg2, struct126 *arg3) {
    void *sp30[2];
    void *sp28[2];

    if (arg2->unk1D4 != 0) {
        sp30[0] = &D_800A09D0;
        sp30[1] = &D_800A09DC;
        sp28[0] = arg0;
        sp28[1] = (void *)arg1;
        if (func_1514654C(arg2, arg3, 0, sp30, sp28, 2) != 0) {
            return 1;
        }
        return 0;
    }

    arg0->unk0 = arg2->x_position;
    arg0->unk4 = arg2->y_position + 100.0f;
    arg0->unk8 = arg2->z_position;
    arg1->unk0 = arg2->x_position;
    arg1->unk4 = arg2->y_position + 100.0f + 500.0f;
    arg1->unk8 = arg2->z_position;
    return 2;
}

extern void func_150D4C2C(void *);
extern void func_151478F4(void *);
extern void func_15147928(void *);

void func_150D4BD4(void *arg0) {
    func_150D4C2C(arg0);
    func_151478F4(arg0);
}

void func_150D4C00(void *arg0) {
    func_150D4C2C(arg0);
    func_15147928(arg0);
}

extern void func_1000FD38(void *, void *, s32);
typedef struct {
    struct127 *unk0;
    u8 pad4;
    u8 unk5;
} Src150D5124;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u8 pad8[4];
    s32 unkC;
    u8 pad10[8];
    Src150D5124 *unk18;
    s32 unk1C;
    s16 unk20;
    u8 pad22[2];
    u16 unk24;
} Snd150D5124;

s32 func_150D5124(Snd150D5124 *arg0, s32 arg1, u32 *arg2, s32 arg3, s32 *arg4,
                  s32 arg5, u16 *arg6);
extern s8 D_8008FD8C;

void func_150D4C2C(void *arg0) {
    struct102 **s0;

    s0 = *(struct102 ***)((u8 *)arg0 + 0x98);
    if (D_8008FD8C < 5) {
        func_1000FD38(func_150D5124, s0, 0x5B3);
    }
    func_1000FD38(func_150D5124, s0, 0x5B4);
    func_1000FD38(func_150D5124, s0, 0x5BC);
    if (s0[3] != 0) {
        func_1516972C(s0[3]);
    }
    if (s0[4] != 0) {
        func_1516972C(s0[4]);
    }
}

struct Bar150D4CC4 {
    char pad48[0x48];
    f32 unk48;
};

struct Obj150D4CC4 {
    char pad18[0x18];
    u8 unk18;
    char pad19[0x170 - 0x19];
    s16 unk170;
    char pad172[0x2];
    f32 unk174;
};

extern s32 D_800A09C0[];
extern f32 D_800A0A14;

s32 func_150D4CC4(struct Obj150D4CC4 *arg0) {
    struct Bar150D4CC4 *v1 = (struct Bar150D4CC4 *)((char *)arg0 + 0x110);

    arg0->unk170 -= D_800BE9E4;
    if (arg0->unk170 < 0) {
        arg0->unk18 = D_800A09C0[func_150ADA20() & 3];
        arg0->unk170 = (func_150ADA20() & 7) + 3;
    }

    v1->unk48 = v1->unk48 + (arg0->unk174 - v1->unk48) * D_800A0A14;
    return 1;
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Func150D4D58Spawn;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, f32 *);

void func_150D4D58(struct17 *arg0, struct17 *arg1, f32 arg2, u8 arg3, s32 arg4) {
    Func150D4D58Spawn spawn;
    s32 count;
    s16 angle;
    f32 scale;
    s32 r1;
    s32 r2;

    count = func_150ADA20() % 11U + 10;
    spawn.unk8 = 0x2203;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk14 = 0xFF;
    spawn.unk15 = 0xFF;
    spawn.unk16 = 0xFF;
    spawn.unk17 = 0xFF;
    spawn.unk18 = 0xFF;
    spawn.unk19 = 0xFF;
    spawn.unk1A = 0xFF;
    spawn.unk1C = 0xFF;
    spawn.unk30 = *arg0;
    spawn.unk3C = *(struct17 *)&D_800A5480;
    spawn.unk1E = 0xA;
    spawn.unk20 = 0x19;
    spawn.unk22 = 1;
    spawn.unk58 = 0x4C207;
    spawn.unk60 = 0;
    spawn.unk61 = 7;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk5C = 0;
    spawn.unk66 = 0xFF;
    angle = (s16)(arg2 * 182.0444489f);
    angle >>= 8;
    angle += 0x40;
    spawn.unk24 = 1.0f;
    do {
        scale = func_150ADA68() * 0.2230000049f + 0.8050000668f;
        spawn.unk1D = *(D_800A09C0 + (func_150ADA20() & 3));
        spawn.unkA = func_150ADA20() % 18U + 27;
        spawn.unk1B = func_150ADA20() % 156U + 100;
        spawn.unk28 = spawn.unk2C = func_150ADA68() * 233.0f + 199.0f;
        r1 = func_150ADA20();
        r2 = func_150ADA20();
        func_15143794(r1 % 130U + angle, r2 % 81U - 64, func_150ADA68() * 10.0f + 5.0f, &spawn.unk48.unk0);
        spawn.unk48.unk0 -= arg1->unk0 * scale;
        spawn.unk48.unk8 -= arg1->unk8 * scale;
        spawn.unk54 = func_150ADA68() * 0.6460000277f + -0.7940000296f;
        spawn.unk58 &= ~0xC0;
        r1 = (func_150ADA20() & 1) ? 0x80 : 0;
        r2 = (func_150ADA20() & 1) ? 0x40 : 0;
        spawn.unk58 |= r2 | r1;
        func_15130280(&spawn, 1, 0, 0, arg3, arg4);
        count--;
    } while (count > 0);
}

extern void func_100111C8(u16);

s32 func_150D5124(Snd150D5124 *arg0, s32 arg1, u32 *arg2, s32 arg3, s32 *arg4,
                  s32 arg5, u16 *arg6) {
    Src150D5124 *src;
    s32 volume;
    s32 pitch;
    s32 targetVolume;
    s32 targetPitch;

    targetVolume = 0;
    targetPitch = 0;
    src = arg0->unk18;
    volume = arg0->unkC;
    pitch = *arg4;

    if (arg0->unk1C == 0x5B3) {
        if (src->unk0 != 0) {
            targetVolume = src->unk0->unk28 * 25.0f;
            if (targetVolume > 5000) {
                targetVolume = 5000;
            } else if (targetVolume < 500) {
                targetVolume = 0;
            }
            targetPitch += src->unk0->unk28 * 5.0f;
            if (targetPitch > 1000) {
                targetPitch = 1000;
            }
        }
        if (src->unk5 == 2) {
            targetVolume += 7000;
        } else if (src->unk5 == 1) {
            targetPitch = 400;
            targetVolume += 7000;
        } else {
            targetPitch = 400;
            if (targetVolume != 0) {
                volume = targetVolume;
            }
        }
    } else if (arg0->unk1C == 0x5BC) {
        targetVolume = 24000;
        if (src->unk5 == 0) {
            volume = targetVolume;
        } else {
            targetVolume = 400;
        }
    } else if (arg0->unk1C == 0x5B4) {
        if (src->unk5 == 0) {
            targetVolume = 16000;
            targetPitch = 400;
            volume = targetVolume;
        } else if ((src->unk5 == 1) || (D_8008FD8C >= 5)) {
            targetVolume = 12000;
        } else {
            targetPitch = 400;
        }
    }

    if (volume != targetVolume) {
        if (volume < targetVolume) {
            volume += D_800BE9E4 * 256;
            if (volume > targetVolume) {
                volume = targetVolume;
            }
        } else {
            volume -= D_800BE9E4 * 256;
            if (volume < targetVolume) {
                volume = targetVolume;
            }
        }
    }
    if (pitch != targetPitch) {
        if (pitch < targetPitch) {
            pitch += D_800BE9E4 * 256;
            if (pitch > targetPitch) {
                pitch = targetPitch;
            }
        } else {
            pitch -= D_800BE9E4 * 8;
            if (pitch < targetPitch) {
                pitch = targetPitch;
            }
        }
    }

    if (volume < 500) {
        volume = 0;
    } else if (*arg6 == 0) {
        arg0->unk0 = arg0->unk1C;
        *arg6 = arg0->unk1C;
    }

    arg0->unk2 = src->unk0->x_position;
    arg0->unk4 = src->unk0->y_position;
    arg0->unk6 = src->unk0->z_position;

    if (D_800C35EA == 1) {
        arg0->unkC = volume >> 1;
        *arg2 = *arg2 >> 1;
    } else {
        arg0->unkC = volume;
    }
    *arg4 = arg0->unk20 = pitch;

    if (*arg2 == 0) {
        if (arg0->unk24 != 0) {
            func_100111C8(arg0->unk24);
            arg0->unk24 = 0;
        }
        *arg6 = 0;
    }
    return 0;
}
