#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s16 D_800C3EF0;
extern f32 D_80097D60;

struct Entry15034EB4 {
    u8 pad[0x34];
    f32 unk34;
    u8 pad2[0x8];
};

struct Obj15034EB4 {
    u8 pad[0x14C];
    f32 unk14C;
    u8 pad2[0x84];
    struct Entry15034EB4 *unk1D4;
};
void func_15034EB4(struct Obj15034EB4 *obj, s32 idx, s32 idx2);

extern u8 D_800C3F00;

void func_15034F20(void) {
    D_800C3F00 = 0;
}

void func_150A7A48(void *, void *, void *);
void guMtxF2L2(void *, void *);
void func_15034728(void *);
s32 func_150AD960(s32, s32, s32, s32);

void func_15034F30(u8 *arg0, u8 *arg1, s32 arg2, f32 (*arg3)[4][4], f32 (*arg4)[4][4], s32 arg5,
                   f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, f32 arg11, f32 arg12,
                   f32 arg13, f32 arg14, f32 arg15, f32 arg16, s32 arg17, s32 arg18) {
    f32 flip[4][4];
    f32 rotA[4][4];
    f32 rotB[4][4];
    s32 i;
    f32 (*src)[4];
    f32 dist;
    f32 temp[4][4];
    void *saved;

    guMtxIdentF(flip);
    if (arg2 == 1 || arg2 == 0) {
        flip[1][1] = -1.0f;
        for (i = 0; i < arg5; i++) {
            if (arg17 != 0) {
                guMtxL2F(temp, (Mtx *)((i << 6) + (u8 *)arg3));
                src = temp;
            } else {
                arg3[i][0][3] = 0.0f;
                arg3[i][1][3] = 0.0f;
                arg3[i][2][3] = 0.0f;
                arg3[i][3][3] = 1.0f;
                src = (void *)((i << 6) + (u8 *)arg3);
            }
            func_150A7A48(src, flip, arg4[i]);
            arg4[i][3][1] = src[3][1] - 2.0f * (src[3][1] - arg7);
            if (arg18 != 0) {
                bcopy((i << 6) + (u8 *)arg4, temp, 0x40);
                guMtxF2L2(temp, (i << 6) + (u8 *)arg4);
            }
        }
        dist = arg12 - arg7;
        if (arg15 < dist) {
            arg1[4] = 0;
        } else if (dist < arg14) {
            if (arg2 == 0) {
                if (dist < 10.0f) {
                    arg1[4] = arg16 / 10.0f * dist;
                } else {
                    arg1[4] = arg16;
                }
            } else {
                arg1[4] = arg16;
            }
        } else {
            dist -= arg14;
            dist = arg16 * dist / (arg15 - arg14);
            dist = arg16 - dist;
            arg1[4] = dist;
        }
    } else if (arg2 == 2) {
        flip[0][0] = -1.0f;
        func_150A8050(rotA, 0.0f, -arg10, 0.0f);
        func_150A8050(rotB, 0.0f, arg10, 0.0f);
        for (i = 0; i < arg5; i++) {
            if (arg17 != 0) {
                guMtxL2F(temp, (Mtx *)((i << 6) + (u8 *)arg3));
                src = temp;
            } else {
                arg3[i][0][3] = 0.0f;
                arg3[i][1][3] = 0.0f;
                arg3[i][2][3] = 0.0f;
                arg3[i][3][3] = 1.0f;
                src = (void *)((i << 6) + (u8 *)arg3);
            }
            bcopy(src, arg4[i], 0x40);
            arg4[i][3][0] -= arg6;
            arg4[i][3][1] -= arg7;
            arg4[i][3][2] -= arg8;
            func_150A7A48(arg4[i], rotA, arg4[i]);
            func_150A7A48(arg4[i], flip, arg4[i]);
            func_150A7A48(arg4[i], rotB, arg4[i]);
            arg4[i][3][0] += arg6;
            arg4[i][3][1] += arg7;
            arg4[i][3][2] += arg8;
            if (arg18 != 0) {
                bcopy((i << 6) + (u8 *)arg4, temp, 0x40);
                guMtxF2L2(temp, (i << 6) + (u8 *)arg4);
            }
        }
        dist = func_150AD960(arg6, arg8, arg11, arg13);
        if (arg15 < dist) {
            arg1[4] = 0;
        } else if (dist < arg14) {
            arg1[4] = arg16;
        } else {
            dist -= arg14;
            dist = arg16 * dist / (arg15 - arg14);
            dist = arg16 - dist;
            arg1[4] = dist;
        }
    } else {
        return;
    }
    if (arg0 != NULL && *(s32 *)(arg0 + 0x9C) != 0) {
        saved = *(void **)(arg0 + 0x1D4);
        *(void **)(arg0 + 0x1D4) = arg4;
        func_15034728(arg0);
        *(void **)(arg0 + 0x1D4) = saved;
    }
}

struct Entry150356C8 {
    u32 unk0;
    u8 unk4;
    u8 pad5[4];
    u8 unk9;
    u8 unkA;
    u8 unkB;
};

extern struct Entry150356C8 D_800C3F08[];

struct Entry150356C8 *func_150356C8(void) {
    if (D_800C3F00 == 15) {
        return NULL;
    }
    D_800C3F00++;
    return &D_800C3F08[D_800C3F00 - 1];
}


s32 func_150A6360(void *, void *, f32, f32, f32, f32, f32, f32);
extern f32 D_80097D70;
extern u8 D_800D9C10;

s32 func_15035714(s32 arg0, u8 *arg1, u8 *arg2, f32 arg3) {
    f32 temp_fa0;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 var_fv0;
    f32 var_fv1;
    s32 sp28;
    u8 *temp_v0;
    s32 index;
    f32 delta;

    sp28 = D_800BE628;
    index = arg1[4];
    temp_v0 = (u8 *)D_800D1C90[index];
    var_fv0 = (f32)(u32)*(u16 *)(temp_v0 + 0xE);
    temp_ft4 = var_fv0;
    temp_ft5 = var_fv0;
    temp_ft4 *= *(f32 *)(arg1 + 0x14C);
    temp_ft5 *= *(f32 *)(arg1 + 0x150);
    temp_fa0 = *(f32 *)(arg1 + 0x150) * (f32)*(s16 *)(temp_v0 + 0x10);
    if ((arg0 == 1) || (arg0 == 0)) {
        delta = (*(f32 *)(arg1 + 0x18) + temp_fa0) - arg3;
        var_fv1 = arg3 - delta;
    } else {
        var_fv1 = *(f32 *)(arg1 + 0x18) + temp_fa0;
    }
    if (func_150A6360((void *)sp28, &D_800D9C10,
                      *(f32 *)(arg2 + 0x30), var_fv1,
                      *(f32 *)(arg2 + 0x38), temp_ft4, temp_ft5,
                      D_80097D70) == 0) {
        return 1;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_623D0/func_15035808.s")

extern u8 D_80082FC0[];
extern u8 D_80083140[];

struct Obj15035D6C {
    u8 unk0;
    u8 pad1[2];
    u8 unk3;
    u8 pad4[2];
    u8 unk6;
    u8 pad7[0xC];
    u8 unk13;
    u8 unk14;
    u8 pad15[0xF];
    Gfx **unk24;
};

Gfx *func_15035D6C(Gfx *gfx, struct Obj15035D6C *obj, s32 red, s32 green, s32 blue) {
    struct Entry150356C8 *entry;
    s32 alpha;
    s32 i;
    s32 j;

    if (D_800C3F00 == 0) {
        return gfx;
    }

    gSPSetGeometryMode(gfx++, G_CULL_FRONT);

    for (i = 0; i < D_800C3F00; i++) {
        entry = &D_800C3F08[i];
        if (entry->unkB != 1) {
            continue;
        }
        if (entry->unk9 != obj->unk0) {
            continue;
        }
        if (entry->unkA != obj->unk6) {
            continue;
        }

        alpha = (entry->unk4 * obj->unk3) >> 8;

        gDPPipeSync(gfx++);
        gSPSegment(gfx++, 3, entry->unk0);
        gDPSetEnvColor(gfx++, red, green, blue, alpha);

        if (alpha < 255) {
            gSPSegment(gfx++, 8, D_80082FC0);
        } else {
            gSPSegment(gfx++, 8, D_80083140);
        }

        for (j = 0; j < obj->unk14; j++) {
            if (!(obj->unk13 & (1 << j))) {
                gSPDisplayList(gfx++, obj->unk24[j]);
            }
        }
    }

    gSPClearGeometryMode(gfx++, G_CULL_FRONT);

    return gfx;
}

extern Gfx *func_1502CCFC(Gfx *, s32, s32, u32, s32, s32 *, s32, s32);

Gfx *func_15035FE8(Gfx *gfx, s32 arg1) {
    struct Entry150356C8 *e;
    s32 col[4];
    s32 i;

    if (D_800C3F00 == 0) {
        return gfx;
    }
    gSPSetGeometryMode(gfx++, G_SHADE | G_SHADING_SMOOTH);
    gSPClearGeometryMode(gfx++, G_FOG | G_LOD);
    gDPSetAlphaCompare(gfx++, G_AC_NONE);
    for (i = 0; i < D_800C3F00; i++) {
        e = &D_800C3F08[i];
        if (e->unkB != 0) {
            continue;
        }
        col[0] = e->pad5[0];
        col[1] = e->pad5[1];
        col[2] = e->pad5[2];
        col[3] = 0xFF;
        gfx = func_1502CCFC(gfx, e->pad5[3], arg1, e->unk0, e->unk4, col, 0, 1);
    }
    return gfx;
}

void func_1507C3E0(struct127 *arg0, s16 *arg1, s16 *arg2, s16 *arg3);
void func_15035808(s32, s32, f32, f32, f32, f32, f32, f32, f32, f32);

void func_15036148(void) {
    s32 pad;
    s16 height;
    s16 sp90;
    s16 sp8E;
    s32 i;

    if (!(D_800DBFF0->unk5F0 & 1)) {
        for (i = 0; i < 25; i++) {
            if (D_800CC2D0[i].interaction_state != 0 && (D_800CC2D0[i].unk5 == 0 || D_800CC2D0[i].unk5 == 1)) {
                if (D_800CC2D0[i].unk118 != -10000.0f && D_800CC2D0[i].unk118 < D_800CC2D0[i].y_position && D_800CC2D0[i].unk180 < D_800CC2D0[i].unk118 && D_800CC2D0[i].unk28 > 5.0f) {
                    func_1507C3E0(&D_800CC2D0[i], &height, &sp90, &sp8E);
                    if ((f32)height < D_800CC2D0[i].unk118 - D_800CC2D0[i].unk180) {
                        func_15035808(1, i, D_800CC2D0[i].x_position, D_800CC2D0[i].unk118, D_800CC2D0[i].z_position, 0.0f, 0.0f, 100.0f, 150.0f, 127.0f);
                    }
                }
            }
        }
    }
}
