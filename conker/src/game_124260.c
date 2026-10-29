#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    char pad_0[0x3B];
    u8 field_0x3B;
} ActorUniqueIdFields;

void func_150F6DB0(ActorUniqueIdFields *arg0) {
    struct {
        s32 unk0;
        u8 unk4;
    } sp18;

    sp18.unk0 = (s32)arg0;
    sp18.unk4 = arg0->field_0x3B;
    func_151494E0((s32)&sp18, 0x3E);
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
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
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
} Func150F6DE4Spawn;

typedef struct {
    s32 unk0;
    u8 unk4;
    s32 unk8[27];
    u8 unk74;
} Func150F6DE4Data;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
} Func150F6DE4Obj;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

struct260 *func_150F6DE4(ActorUniqueIdFields *arg0) {
    Func150F6DE4Data data;
    u8 i;
    struct260 *temp_v0;

    data.unk0 = (s32)arg0;
    data.unk4 = arg0->field_0x3B;
    for (i = 0; i < 2; i++) {
        data.unk8[i] = 0;
    }
    data.unk74 = 0;
    temp_v0 = func_15149130(0x12C, -1, 0x43, -1, 0, 0x37, (struct37 *)0x78, 0xFF, 1);
    if (temp_v0 != NULL) {
        s32 dest;

        dest = (s32)temp_v0 + 0x28;
        memcpy(dest, &data, sizeof(data));
        {
            Func150F6DE4Spawn spawn;

            spawn.unk8 = 0x4417;
            spawn.unk0 = 0x200004;
            spawn.unk4 = 0;
            spawn.unkA = 0x12C;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk14 = 0xFF;
            spawn.unk15 = 0xFF;
            spawn.unk16 = 0xFF;
            spawn.unk17 = 0xFF;
            spawn.unk1C = 0xFF;
            spawn.unk28 = spawn.unk2C = 1511.0f;
            spawn.pos = *(struct17 *)&D_800A5480;
            spawn.unk3C = *(struct17 *)&D_800A5480;
            spawn.vel = *(struct17 *)&D_800A5480;
            spawn.unk1E = 1;
            spawn.unk20 = 0xFF;
            spawn.unk22 = 1;
            spawn.unk58 = 0x64C000;
            spawn.unk60 = 8;
            spawn.unk61 = 6;
            spawn.unk62 = -1;
            spawn.unk63 = -1;
            spawn.unk64 = 3;
            spawn.unk65 = 0;
            spawn.unk5C = 0;
            spawn.unk66 = 0xFF;
            spawn.unk68 = 0xA;
            spawn.unk1D = 0x64;
            spawn.unk18 = 0;
            spawn.unk19 = 0;
            spawn.unk1A = 0xFF;
            spawn.unk1B = 0xFF;
            spawn.unk54 = 0.0f;
            spawn.unk24 = 1.0f;
            spawn.unk6C = 20.0f;
            *(void **)(dest + 8) = func_15130280(&spawn, 1, 0, 0, ((Func150F6DE4Obj *)temp_v0)->unkC, ((Func150F6DE4Obj *)temp_v0)->unk1);
            spawn.unk18 = 0xFF;
            spawn.unk19 = 0;
            spawn.unk1A = 0;
            spawn.unk1B = 0xFF;
            *(void **)(dest + 0xC) = func_15130280(&spawn, 1, 0, 0, ((Func150F6DE4Obj *)temp_v0)->unkC, ((Func150F6DE4Obj *)temp_v0)->unk1);
        }
    }
    return temp_v0;
}

typedef struct {
    /* 0x00 */ u8 pad_0[0x40];
    /* 0x40 */ struct17 unk40;
    /* 0x4C */ u8 pad_4C[0x74 - 0x4C];
    /* 0x74 */ s8 unk74;
} BeamEnd;

typedef struct {
    /* 0x00 */ u8 *unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ struct17 unk8;
    /* 0x14 */ u8 pad_14[0x59 - 0x14];
    /* 0x59 */ u8 unk59;
    /* 0x5A */ u8 pad_5A[0x64 - 0x5A];
} BeamTrace;

typedef struct {
    /* 0x00 */ struct127 *unk0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 pad_5[3];
    /* 0x08 */ BeamEnd *unk8;
    /* 0x0C */ BeamEnd *unkC;
    /* 0x10 */ BeamTrace unk10;
    /* 0x74 */ u8 unk74;
} BeamState;

extern f32 D_800A1BB4;
extern f32 D_800A1BB8;

void func_15145740(struct127 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, f32 arg4);
void func_15081690(struct127 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, void *arg7, f32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, s32 argE);
s32 func_1506196C(u8 *arg0, s32 arg1);
void func_1502EA98(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

void func_150F706C(struct260 *arg0) {
    BeamState *beam;
    struct127 *obj;
    struct17 pos;
    struct17 hitPos;

    beam = (BeamState *)((u8 *)arg0 + 0x28);
    obj = beam->unk0;

    if ((obj->interaction_state == 0) || (obj->unique_id != beam->unk4)) {
        arg0->unkE = -1;
        return;
    }

    if (D_800C35EA == 1) {
        if ((beam->unk8 != 0) && (beam->unkC != 0)) {
            beam->unk8->unk74 = 3;
            beam->unkC->unk74 = 3;
        }
        return;
    }

    pos.unk0 = obj->x_position;
    pos.unk4 = obj->y_position + 46.0f;
    pos.unk8 = obj->z_position;
    func_15145740(obj, (struct17 *)&D_800D9A50, 0, 0, 0.0f);

    ((struct17 *)&D_800D9A50)->unk0 *= D_800A1BB4;
    ((struct17 *)&D_800D9A50)->unk4 *= D_800A1BB4;
    ((struct17 *)&D_800D9A50)->unk8 *= D_800A1BB4;

    func_15081690(obj, pos.unk0, pos.unk4, pos.unk8,
                  ((struct17 *)&D_800D9A50)->unk0, ((struct17 *)&D_800D9A50)->unk4,
                  ((struct17 *)&D_800D9A50)->unk8,
                  &beam->unk10, D_800A1BB8, 0, 0, 1, -1, 0, 0);

    beam->unk74 |= 1;

    if (beam->unk8 != 0) {
        if (beam->unkC == 0) {
            return;
        }

        if (beam->unk10.unk59 == 0) {
            beam->unkC->unk74 = 3;
            beam->unk8->unk74 = beam->unkC->unk74;
        } else {
            hitPos = beam->unk10.unk8;
            beam->unkC->unk40 = hitPos;
            beam->unk8->unk40 = hitPos;

            if (beam->unk10.unk59 == 1) {
                beam->unk8->unk74 = -1;
                beam->unkC->unk74 = 3;
            } else if (func_1506196C(beam->unk10.unk0, 0) < 0xFF) {
                beam->unk8->unk74 = -1;
                beam->unkC->unk74 = 3;
            } else {
                func_1502EA98(beam->unk10.unk0, 0xFF, 0, 0, 0x7F, 0, 0x10);
                beam->unk8->unk74 = 3;
                beam->unkC->unk74 = -1;
            }
        }
    }
}

void func_150F7310(s32 arg0, s32 arg1, u8 arg2) {
    s32 *p = &arg0;
    s32 q;
    if (arg2 == 0x3E) {
        q = *p + 0x28;
        if ((*(s32 *)q == *(s32 *)arg1) || (*(u8 *)(q + 4) == *(u8 *)(*(s32 *)&arg1 + 4))) {
            func_1516972C((struct102 *)*p);
        }
    } else {
        q = *p + 0x28;
        func_15149514(arg1, arg2, q, q + 4, *p);
    }
}

extern void func_1514EDF0(struct260 *, s32);

void func_150F739C(struct260 *arg0) {
    s32 i;
    s32 v;
    s32 *p = (s32 *)((u8 *)arg0 + 0x28);

    for (i = 0; i < 2; i = (u8)(i + 1)) {
        v = p[i + 2];
        if (v != 0) {
            func_1516972C((struct102 *)v);
        }
    }
    func_1514EDF0(arg0, p[0]);
}

void func_150F739C(struct260 *arg0);

void func_150F740C(struct260 *arg0) {
    func_150F739C(arg0);
    func_1514933C(arg0);
}

void func_15149368(struct260 *arg0);

void func_150F7438(struct260 *arg0) {
    func_150F739C(arg0);
    func_15149368(arg0);
}
