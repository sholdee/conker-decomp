#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    f32 x, y, z;
} Vec3_151B8DB0;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ Vec3_151B8DB0 pos;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    s32 pad78;
} Spawn_151B8DB0;

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
} Payload24_151B8DB0;

typedef struct {
    s32 unk00;
    s32 unk04;
    f32 unk08;
} Payload0C_151B8DB0;

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    f32 unk48;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
    s8 unk59;
    u8 pad5A[6];
} Struct98_151B8DB0;

typedef struct {
    u8 unk00;
    u8 unk01;
    s16 unk02;
    s16 unk04;
    u8 pad06[2];
    s32 unk08;
    s32 unk0C;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    Vec3_151B8DB0 unk1C;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
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
    u8 pad4D[0xB];
} Struct40_151B8DB0;

extern Vec3_151B8DB0 D_800AA4E0[];
extern f32 D_800AA528[];
extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void func_1510F800(s32);
void *func_151407D0(void *, s32, void *, s32, s32, s32, s32, s32, s32, s32);
struct225 *func_1516037C(Header *src, struct226 *arg1, s32 size, u8 arg3, s32 arg4);

void func_151B8DB0(s32 arg0, u8 arg1, u8 arg2, s32 arg3) {
    void *v;
    Spawn_151B8DB0 sp;
    Payload24_151B8DB0 pl;

    pl.unk00 = 0.0f;
    pl.unk10 = pl.unk04 = 17.31599998f;
    pl.unk08 = func_150ADA68() * 6.283185482f;
    pl.unk14 = pl.unk0C = 0.08799999952f;
    pl.unk18 = 0.0f;
    pl.unk1C = 0.0f;
    pl.unk20 = 0.0f;

    sp.unk00 = 1.0f;
    sp.unk04 = 1.0f;
    sp.unk0C = D_800AA528[arg1];
    sp.unk08 = sp.unk0C;
    sp.unk10 = 0.0f;
    sp.unk14 = 0.0f;
    sp.unk18 = 0.0f;
    sp.unk1C = 1.0f;
    sp.unk20 = 1.0f;
    sp.unk24 = 1.0f;
    sp.pos = D_800AA4E0[arg1];
    sp.unk34 = 0.0f;
    sp.unk38 = 0.0f;
    sp.unk3C = 0.0f;
    sp.unk40 = 0.0f;
    sp.unk44 = 0.0f;
    sp.unk48 = 0.0f;
    sp.unk4C = 0.0f;
    sp.unk54 = 0x12C;
    sp.unk56 = 0x22;
    func_1510F800(0);
    sp.unk5C = func_1510FD20((s32)sp.pos.x, (s32)sp.pos.z);
    sp.unk50 = (sp.unk5C ? 0x400 : 0x0) | 0x100 | 0x800;
    sp.unk58 = 0;
    sp.unk60 = 0xFF;
    sp.unk61 = 9;
    sp.unk62 = 0;
    sp.unk63 = 0;
    sp.unk64 = 0;
    sp.unk65 = 0;
    sp.unk66 = 0;
    sp.unk67 = 0;
    sp.unk68 = 4;
    sp.unk6A = 2;
    sp.unk6C = 0;
    sp.unk70 = 0;
    sp.unk72 = 1;
    sp.unk74 = 0xFF;
    v = func_15132A4C(&sp, 3, 0xFF, 0x24, arg2, arg3);
    if (v != NULL) {
        Header header;

        memcpy((u8 *)v + 0x170, &pl, sizeof(pl));
        {
            Payload0C_151B8DB0 pl2;
            Payload0C_151B8DB0 *p;

            pl2.unk00 = (s32)v;
            pl2.unk04 = 0;
            pl2.unk08 = 1.0f / D_800AA528[arg1];
            header.unk0 = 0;
            header.unk1 = 7;
            header.unk2 = 300;
            header.unk4 = 0x12;
            v = func_1516037C(&header, (struct226 *)arg0, sizeof(pl2), arg2, arg3);
            if (v != NULL) {
                p = (Payload0C_151B8DB0 *)&((struct225 *)v)->unk18;
                memcpy(p, &pl2, sizeof(pl2));
                {
                    Struct98_151B8DB0 sp98;
                    Struct40_151B8DB0 sp40;

                    sp98.unk00 = sp98.unk04 = D_800AA528[arg1] * 43.29999924f;
                    sp98.unk08 = sp98.unk0C = D_800AA528[arg1] * 30.20000076f;
                    sp98.unk10 = 0.200000003f;
                    sp98.unk14 = 0.25f;
                    sp98.unk18 = 0.8000000119f;
                    sp98.unk1C = 0.6000000238f;
                    sp98.unk20 = 9.999999975e-07f;
                    sp98.unk24 = -1;
                    sp98.unk34 = -1;
                    sp98.unk28 = -1;
                    sp98.unk38 = -1;
                    sp98.unk2C = -1;
                    sp98.unk3C = -1;
                    sp98.unk30 = -1;
                    sp98.unk40 = -1;
                    sp98.unk44 = 0;
                    sp98.unk48 = 1.0f;
                    sp98.unk4C = 0;
                    sp98.unk4E = 0;
                    sp98.unk50 = 0;
                    sp98.unk52 = 0;
                    sp98.unk54 = 0;
                    sp98.unk55 = 0;
                    sp98.unk56 = 0;
                    sp98.unk57 = 0;
                    sp98.unk58 = 0;
                    sp98.unk59 = -1;

                    sp40.unk00 = 0x69;
                    sp40.unk01 = 3;
                    sp40.unk02 = 0x3403;
                    sp40.unk04 = 0x12C;
                    sp40.unk08 = 0;
                    sp40.unk0C = 0;
                    sp40.unk10 = 0xFF;
                    sp40.unk11 = 0xFF;
                    sp40.unk12 = 0xFF;
                    sp40.unk13 = 0xFF;
                    sp40.unk14 = 100.0f;
                    sp40.unk18 = 100.0f;
                    sp40.unk1C = sp.pos;
                    sp40.unk28 = sp.pos.x;
                    sp40.unk2C = sp.pos.y - 500.0f;
                    sp40.unk30 = sp.pos.z;
                    sp40.unk34 = 1.0f;
                    sp40.unk38 = 1.0f;
                    sp40.unk3C = 1.0f;
                    sp40.unk40 = 0x1CD2006;
                    sp40.unk44 = 0xFF;
                    sp40.unk45 = 0xFF;
                    sp40.unk46 = 0;
                    sp40.unk47 = 6;
                    sp40.unk48 = sp.unk5C;
                    sp40.unk4C = 0xFF;
                    p->unk04 = (s32)func_151407D0(&sp98, 0x60, &sp40, 0, 0, 0, 0, -1, arg2, arg3);
                }
            }
        }
    }
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_1E6260/func_151B9214.s")

void func_150A8050(f32 (*)[4], f32, f32, f32);

s32 func_151B9310(s32 *arg0, s32 *arg1) {
    f32 m[18];

    func_150A8050((f32 (*)[4])&m[2], 0.0f, 0.0f, *(f32 *)((u8 *)arg1 + 0x170));

    m[14] = *(f32 *)((u8 *)arg1 + 0x38);
    m[15] = *(f32 *)((u8 *)arg1 + 0x3C);
    m[16] = *(f32 *)((u8 *)arg1 + 0x40);

    m[2] = *(f32 *)((u8 *)arg1 + 0x18) * m[2];
    m[3] = *(f32 *)((u8 *)arg1 + 0x18) * m[3];
    m[4] = *(f32 *)((u8 *)arg1 + 0x18) * m[4];

    m[6] = *(f32 *)((u8 *)arg1 + 0x1C) * m[6];
    m[7] = *(f32 *)((u8 *)arg1 + 0x1C) * m[7];
    m[8] = *(f32 *)((u8 *)arg1 + 0x1C) * m[8];

    m[10] = *(f32 *)((u8 *)arg1 + 0x18) * m[10];
    m[11] = *(f32 *)((u8 *)arg1 + 0x18) * m[11];
    m[12] = *(f32 *)((u8 *)arg1 + 0x18) * m[12];

    guMtxF2L((f32 (*)[4])&m[2], (Mtx *)arg0);
    return 1;
}

void func_150A7960(void *, f32, f32, f32, f32 *, f32 *, f32 *);
extern f32 D_800AA568;

typedef struct Game1E67C0State {
    u8 *source;
    u8 *target;
    f32 rate;
} Game1E67C0State;

s32 func_151B9408(u8 *actor) {
    f32 extend;
    u8 *source;
    struct {
        f32 matrix[16];
        f32 x, y, z;
    } work;
    f32 reach;
    f32 rate;
    register f32 dx, dy, dz;
    Game1E67C0State *state;
    u8 *target;

    state = (Game1E67C0State *)(actor + 0x18);
    source = state->source;
    func_150A8050((f32 (*)[4])work.matrix, 0.0f, 0.0f, *(f32 *)(source + 0x170));
    work.matrix[12] = *(f32 *)(source + 0x38);
    work.matrix[13] = *(f32 *)(source + 0x3C);
    work.matrix[14] = *(f32 *)(source + 0x40);
    work.matrix[0] *= *(f32 *)(source + 0x18);
    work.matrix[1] *= *(f32 *)(source + 0x18);
    work.matrix[2] *= *(f32 *)(source + 0x18);
    work.matrix[4] *= *(f32 *)(source + 0x1C);
    work.matrix[5] *= *(f32 *)(source + 0x1C);
    work.matrix[6] *= *(f32 *)(source + 0x1C);
    work.matrix[8] *= *(f32 *)(source + 0x18);
    work.matrix[9] *= *(f32 *)(source + 0x18);
    work.matrix[10] *= *(f32 *)(source + 0x18);
    func_150A7960(work.matrix, 0.0f, -150.0f, 0.0f, &work.x, &work.y, &work.z);
    *(s16 *)(*(u8 **)(actor + 0x14) + 0xE) = (s16)(s32)work.x;
    *(s16 *)(*(u8 **)(actor + 0x14) + 0x10) = (s16)(s32)work.y;
    *(s16 *)(*(u8 **)(actor + 0x14) + 0x12) = (s16)(s32)work.z;
    target = state->target;
    if (target != 0) {
        extend = 500.0f;
        reach = *(f32 *)(source + 0x1C) * 65.0f;
        rate = state->rate * D_800AA568;
        dx = -(work.x - *(f32 *)(source + 0x38)) * rate;
        dy = -(work.y - *(f32 *)(source + 0x3C)) * rate;
        dz = -(work.z - *(f32 *)(source + 0x40)) * rate;
        *(f32 *)(target + 0x34) = (f32)(*(f32 *)(source + 0x38) + dx * reach);
        *(f32 *)(state->target + 0x38) = (f32)(*(f32 *)(source + 0x3C) + dy * reach);
        *(f32 *)(state->target + 0x3C) = (f32)(*(f32 *)(source + 0x40) + dz * reach);
        target = state->target;
        *(f32 *)(target + 0x40) = (f32)(*(f32 *)(target + 0x34) + dx * extend);
        target = state->target;
        *(f32 *)(target + 0x44) = (f32)(*(f32 *)(target + 0x38) + dy * extend);
        target = state->target;
        *(f32 *)(target + 0x48) = (f32)(*(f32 *)(target + 0x3C) + dz * extend);
    }
    return 1;
}

extern f32 D_800AA56C;
extern f32 D_800AA570;

void func_151B9660(s32 *arg0) {
    *(f32 *)((u8 *)arg0 + 0x188) = 5.0f;
    *(f32 *)((u8 *)arg0 + 0x18C) = D_800AA56C;
    *(f32 *)((u8 *)arg0 + 0x190) = D_800AA570;
}
