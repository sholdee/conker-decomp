#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_15175390(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, f32, f32, s32);
void func_1517AA20(f32, s32, s32, s32, s32);
void func_15177F90(u8, s32, u8, s32, s32, s32, s32, s32, u8, s32, s32);
void func_15174BF0(s32, s32, s32, s32, s32, s32, f32, f32, s32);
void func_15174FA4(s32, s32, s32, s32, s32, s32, s32, s32, f32, f32);
void func_151838B0(s32);
void func_15183974(s32);
void func_151839F0(s32, s32);
void func_15183ACC(s32);
s32 func_15184118();
extern f32 D_800CC2E4;

s32 func_15183290(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA) {
    s32 i;
    struct127 *obj;

    for (i = 0; i < 4; i++) {
        if (D_800DDF5C[i] == 0) {
            break;
        }
    }
    if (i == 4) {
        return -1;
    }
    arg7 *= 16;
    arg8 *= 16;
    arg9 *= 16;
    obj = D_800DBFF0[arg6].unk3D0;
    switch (arg4) {
        case 0:
            func_15177F90(arg3, 0, 0xFE, arg0, arg1, arg2, 0, 0x7FFF, 0xFF, 1, 0);
            break;
        case 1:
            func_15177F90(arg3, (s32)&D_800CC2E4, 0xFE, arg0, arg1, arg2, 0, 0x7FFF, 0xFF, 1, 0);
            break;
        case 2:
            func_151838B0(0);
            func_15175390(arg3, i, arg0, arg1, arg2, 0x3F, arg6, 0x1E, 0, 0x30003, 10, 0, 0, 1, 2, 0.8000000119f, 0.5f, argA);
            break;
        case 3:
            if (func_15184118(obj)) {
                func_15174BF0(arg3, i, arg0, arg1, arg2, arg6, 0.8000000119f, 0.5f, argA);
                break;
            }
            return -1;
        case 8:
            return -1;
        case 4:
            func_151839F0(7, 10);
            func_15174FA4(arg3, i, arg0, arg1, arg2, arg7 * 4, arg6, 0, 0.8000000119f, 0.5f);
            break;
        case 10:
            func_15183ACC(7);
            func_15174FA4(arg3, i, arg0, arg1, arg2, arg7 * 4, arg6, 1, 0.8000000119f, 0.5f);
            break;
        case 11:
            func_15183ACC(2);
            func_15183ACC(3);
            func_15183ACC(4);
            func_15183ACC(5);
            func_15175390(arg3, i, arg0, arg1, arg2, arg7, arg6, arg8, arg9, 0x4D, 0x15, 3, 2, -1, 0, 0.8000000119f, 0.5f, argA);
            break;
        case 5:
            func_15183ACC(2);
            func_15183ACC(3);
            func_15183ACC(4);
            func_15183ACC(5);
            func_15175390(arg3, i, arg0, arg1, arg2, arg7, arg6, arg8, arg9, 0x4D, 0x15, 3, 2, -1, 2, 0.8000000119f, 0.5f, argA);
            break;
        case 6:
            func_15183974(8);
            func_15175390(arg3, i, arg0, arg1, arg2, arg7, arg6, arg8, arg9, 0x7D, 0x44, 2, 8, -1, 2, 0.8000000119f, 0.5f, argA);
            break;
        case 7:
            func_15183ACC(6);
            func_15175390(arg3, i, arg0, arg1, arg2, arg7, arg6, arg8, arg9, 0x4D, 0x14, 0, 6, 0, 2, 0.03999999911f, 0.02500000037f, argA);
            break;
        case 9:
            func_1517AA20(obj->xz_velocity, obj->x_position, obj->y_position, obj->z_position, arg3);
            break;
    }
    if (arg4 != 9) {
        D_800DDF5C[i] = 1;
    }
    return i;
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Struct15183974;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Inner1502B6BC;

extern u8 D_800DDF69[];
void func_15183ACC(s32);
Inner1502B6BC *func_1502B6BC(s32, s32, s32, s32, s32, s32);

void func_151838B0(s32 arg0) {
    Inner1502B6BC *ret;

    if (((Struct15183974 *)D_800DDE80)[arg0].unk0 == 0) {
        func_15183ACC(arg0);
    }
    if (((Struct15183974 *)D_800DDE80)[arg0 + 1].unk0 == 0) {
        ret = func_1502B6BC(0, 0, 0, 2, 9, arg0 + 0xAE);
        if (ret != 0) {
            ((Struct15183974 *)D_800DDE80)[arg0 + 1].unk0 = (s32)ret;
            ((Struct15183974 *)D_800DDE80)[arg0 + 1].unk4 = ret->unk0;
            ((Struct15183974 *)D_800DDE80)[arg0 + 1].unk8 = ret->unk8;
            ((Struct15183974 *)D_800DDE80)[arg0 + 1].unk10 = 0;
            ((Struct15183974 *)D_800DDE80)[arg0 + 1].unkC = ((Struct15183974 *)D_800DDE80)[arg0].unkC;
            D_800DDF69[arg0] = 1;
        }
    }
}

void func_15183974(s32 arg0) {
    if (((Struct15183974 *)D_800DDE80)[arg0].unk0 == 0) {
        func_15183ACC(arg0);
    }
    if (((Struct15183974 *)D_800DDE80)[arg0 + 1].unk0 == 0) {
        func_15183ACC(arg0 + 1);
        ((Struct15183974 *)D_800DDE80)[arg0 + 1].unkC = ((Struct15183974 *)D_800DDE80)[arg0].unkC;
    }
}

void func_151839F0(s32 arg0, s32 arg1) {
    Inner1502B6BC *ret;

    if (((Struct15183974 *)D_800DDE80)[arg0].unk0 == 0) {
        func_15183ACC(arg0);
    }
    if (((Struct15183974 *)D_800DDE80)[arg0].unk0 != 0) {
        if (((Struct15183974 *)D_800DDE80)[arg1].unk0 == 0) {
            ret = func_1502B6BC(0, 0, 0, 2, 9, arg1 + 0xAD);
            if (ret != 0) {
                ((Struct15183974 *)D_800DDE80)[arg1].unk0 = (s32)ret;
                ((Struct15183974 *)D_800DDE80)[arg1].unk4 = ret->unk0;
                ((Struct15183974 *)D_800DDE80)[arg1].unk8 = ret->unk8;
                ((Struct15183974 *)D_800DDE80)[arg1].unkC = ((s32 *)ret)[6];
                ((Struct15183974 *)D_800DDE80)[arg1].unk10 = 0;
                D_800DDF68[arg1] = 1;
            }
        }
    }
}

void func_1510CE60(s32, s32, s32, s32, void *);

void func_15183ACC(s32 arg0) {
    u8 *allocated;

    if (D_800DDE80[arg0].field0 == 0) {
        allocated = (u8 *)func_1502B6BC(0, 0, 0, 2, 9, arg0 + 0xAD);
        if (allocated != 0) {
            D_800DDE80[arg0].field0 = allocated;
            *(s32 *)((u8 *)(&D_800DDE80[arg0]) + 4) = *(s32 *)allocated;
            *(s32 *)((u8 *)(&D_800DDE80[arg0]) + 8) = *(s32 *)(allocated + 8);
            D_800DDE80[arg0].fieldC = *(s32 *)(allocated + 0x18);
            D_800DDF78[arg0] = *(s32 *)(allocated + 0x10);
            func_1510CE60(D_800DDF78[arg0], 0, 1, 0x3E, (u8 *)(&D_800DDE80[arg0]) + 0x10);
            D_800DDF68[arg0] = 1;
        }
    }
}

extern u8 D_800A72D0[];

u32 *func_15183BA4(u32 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 idx;
    s32 i;
    u32 *p;

    idx = -1;
    for (i = 0; i != 11; i++) {
        if (arg1 == D_800A72D0[i]) {
            idx = i;
            break;
        }
    }

    if (idx != -1) {
        if (D_800DDF78[idx] != 0) {
            p = arg0;
            arg0 += 2;
            p[0] = 0xDE000000;
            p[1] = D_800DDF78[idx];
        }
    }
    return arg0;
}

extern s8 D_800DDF70;
extern s8 D_800DDF71;
extern s32 D_800DDF20;
extern s32 D_800DDF98;
void func_1510D630(s32);
void func_10004074(s16 *);

void func_15183C28(s32 arg0) {
    if (((Struct15183974 *)D_800DDE80)[arg0].unk10 != 0) {
        func_1510D630(((Struct15183974 *)D_800DDE80)[arg0].unk10);
        ((Struct15183974 *)D_800DDE80)[arg0].unk10 = 0;
    }
    if (arg0 == 8) {
        if (D_800DDF71 != -2) {
            return;
        }
    } else if (arg0 == 9) {
        if (D_800DDF70 == -2) {
            if (D_800DDF20 != 0) {
                func_10004074((s16 *) D_800DDF20);
                D_800DDF20 = 0;
                D_800DDF98 = 0;
            }
        }
    }
    if (((Struct15183974 *)D_800DDE80)[arg0].unk0 != 0) {
        func_10004074((s16 *) ((Struct15183974 *)D_800DDE80)[arg0].unk0);
        ((Struct15183974 *)D_800DDE80)[arg0].unk0 = 0;
        D_800DDF78[arg0] = 0;
    }
}

typedef struct {
    void *field0;
    u8 pad4[8];
    s32 fieldC;
    void *field10;
} Game1B0740Slot;

typedef struct Game1B0740Wave {
    u8 pad0[0x18];
    f32 angle;
    u8 pad1C[8];
    f32 velocity;
    f32 amplitude;
    f32 frequency;
    f32 radius;
    u8 pad34[4];
} Game1B0740Wave;

void func_15165C80(f32 *, u8 *, f32, f32 *, f32, f32, u8 *, s32);
void func_1517725C(u8);
void func_15179DB0(u8);
void func_15183C28(s32);
extern Game1B0740Wave D_8008D0B0[];
extern Game1B0740Wave D_8008D318[];
extern u8 *D_8008D3C0[];
extern u8 D_800A72E8[];

void func_15183D28(void) {
    s32 i;
    s32 value;
    Game1B0740Wave *wave;

    for (i = 0; i < 11; i++) {
        if (D_800DDF68[i] >= -1 && (((Game1B0740Slot *)D_800DDE80)[i].field0 == NULL || D_800DDF78[i] == 0)) {
            D_800DDF68[i] = -2;
        }
    }
    for (i = 0; i < 4; i++) {
        if (D_800DDF60[i] > 0) {
            value = D_800DDF60[i] - D_800BE9E4;
            if (value < 0) {
                value = 0;
            }
            D_800DDF60[i] = value;
        }
    }
    for (i = 0; i < 7; i++) {
        if (D_800DDF68[i] == 1) {
            wave = &D_8008D0B0[i];
            func_15165C80(&wave->angle, *(u8 **)((u8 *)&((Game1B0740Slot *)D_800DDE80)[i] + 8), wave->radius, &wave->velocity,
                          wave->amplitude, wave->frequency, D_8008D3C0[i], D_800A72E8[i]);
        }
    }
    for (i = 2; i < 5; i++) {
        if (D_800DDF68[i] == 1) {
            wave = &D_8008D318[i - 2];
            func_15165C80(&wave->angle, *(u8 **)((u8 *)&((Game1B0740Slot *)D_800DDE80)[i] + 4), wave->radius, &wave->velocity,
                          wave->amplitude, wave->frequency, D_8008D3C0[i], D_800A72E8[i]);
        }
    }
    if (D_800DDF68[7] == 1) {
        func_1517725C(7);
    }
    for (i = 0; i < 2; i++) {
        if (D_800DDF68[i + 8] == 1) {
            func_15179DB0(i + 8);
        }
    }
    if (D_800DDF68[7] == 1) {
        D_800DDF68[10] = 1;
    }
    for (i = 0; i < 11; i++) {
        if (D_800DDF68[i] == -2 && ((Game1B0740Slot *)D_800DDE80)[i].field0 != NULL) {
            func_15183C28(i);
        }
    }
    for (i = 0; i < 11; i++) {
        if (D_800DDF68[i] > 0) {
            D_800DDF68[i] = 0;
        } else if (D_800DDF68[i] >= -1) {
            D_800DDF68[i]--;
        }
    }
}

typedef struct {
    char pad[0x57];
    u8 unk57;
} Struct15184118;

typedef struct {
    char pad[0x31C];
    Struct15184118 *unk31C;
} Obj15184118;

s32 func_15184118(Obj15184118 *a0) {
    if (a0->unk31C != 0) {
        if (a0->unk31C->unk57 != 0) {
            return 1;
        }
    }
    return 0;
}
