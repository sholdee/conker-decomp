#include <ultra64.h>
#define func_15083568 func_15083568_orig
#include "functions.h"
#undef func_15083568
#include "variables.h"

extern f32 D_80097B68;
extern f32 D_800970DC;
extern u8 D_800BEA0C;
extern s32 D_800902BC[];
void func_1000FD38(void *, void *, s32);
s32 func_15033BDC();

typedef struct Game5D2C0RenderState {
    u8 pad_0[0x2];
    u8 field_0x02;
    u8 pad_0x03[0x13];
    u8 field_0x16;
    u8 pad_0x17[0x29];
    f32 field_0x40;
} Game5D2C0RenderState;

Game5D2C0RenderState *func_15083568(struct127 *, s32, f32, s32);
extern void func_150A7A48(f32 arg0[4][4], f32 arg1[4][4], f32 arg2[4][4]);
extern void func_1503E5F8();
extern void *allocate_memory(s32 size, s32 arg1, s32 arg2, s32 arg3);
extern void func_10004074(void *arg0);
void func_15031C14(void *arg0);
void func_1503F7B8(void *arg0);
void func_1518CA04(s32 arg0);
s32 func_1502FE10(s32 arg0, s32 *arg1, u8 *arg2, s32 *arg3, s32 *arg4, s32 *arg5, s32 *arg6);
s32 func_1503F62C(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4, void *arg5, void *arg6);

typedef struct Game5D2C0EffectState {
    u8 pad_0[0x1];
    u8 field_0x01;
    u8 field_0x02;
    u8 pad_0x03[0x1F];
    s16 field_0x22;
    u8 pad_0x24[0x14];
    s32 field_0x38;
    s32 field_0x3C;
} Game5D2C0EffectState;

typedef struct Game5D2C0EventState {
    u8 pad_0[0x5];
    u8 field_0x05;
} Game5D2C0EventState;

typedef struct Game5D2C0FlagState {
    u8 pad_0[0x3];
    u8 field_0x03;
} Game5D2C0FlagState;

typedef struct Game5D2C0ActorState {
    u8 pad_0[0x11A];
    u8 field_0x11A;
    u8 pad_0x11B[0x81];
    u16 field_0x19C;
} Game5D2C0ActorState;

typedef struct Game5D2C0PositionState {
    s32 field_0x00;
    u8 pad_0x04[0x10];
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    u8 pad_0x20[0x64];
    u16 field_0x84;
    u8 pad_0x86[0x27];
    u8 field_0xAD;
    u8 pad_0xAE[0x6A];
    f32 field_0x118;
    u8 pad_0x11C[0x64];
    f32 field_0x180;
    u8 pad_0x184[0x198];
    Game5D2C0ActorState *field_0x31C;
} Game5D2C0PositionState;

typedef struct Game5D2C0AnimationState {
    u8 pad_0[0x4];
    u16 field_0x04;
    u8 pad_0x06[0x2];
    f32 field_0x08;
    u8 pad_0x0C[0xC];
    f32 field_0x18;
} Game5D2C0AnimationState;

typedef struct Game5D2C0AnimationOwner {
    u8 pad_0[0x48];
    struct197 *field_0x48;
} Game5D2C0AnimationOwner;

typedef struct {
    s32 unk0;
    u32 unk4;
    s32 unk8;
    u32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
} struct_func_1502FE10;

extern u8 D_800DF7D0[];
extern s32 D_800DF9B8[];
extern struct_func_1502FE10 *D_800E0148[];
struct_func_1502FE10 *func_1502B6BC(s32, s32, s32, s32, s32, s32);
void func_1510CE60(s32, s32, s32, s32, s32 *);
void func_15168E54(s32, s32);

s32 func_1502FE10(s32 arg0, s32 *arg1, u8 *arg2, s32 *arg3, s32 *arg4, s32 *arg5, s32 *arg6) {
    struct_func_1502FE10 *temp_s3;
    s32 i;

    if (D_800DF7D0[arg0] == 0) {
        temp_s3 = func_1502B6BC(0, 0, 0, 2, 9, arg0);
        if (temp_s3 == 0) {
            return 1;
        }
        *arg1 = temp_s3->unk0;
        *arg2 = temp_s3->unk4 >> 2;
        if ((s32)*arg2 > 0) {
            i = 0;
            do {
                ((s32 *)*arg1)[i] += (s32)temp_s3;
                func_1510CE60(((s32 *)*arg1)[i], 0, 1, 0x3E, &D_800DF9B8[arg0]);
                func_15168E54(((s32 *)*arg1)[i], (s32)temp_s3);
                i++;
            } while (i < (s32)*arg2);
        }
        D_800E0148[arg0] = temp_s3;
    } else {
        temp_s3 = D_800E0148[arg0];
        *arg1 = temp_s3->unk0;
        *arg2 = temp_s3->unk4 >> 2;
    }
    D_800DF7D0[arg0]++;
    *arg3 = (s32)&temp_s3->unk18;
    if (arg4 != 0) {
        *arg4 = temp_s3->unk10;
    }
    if (arg5 != 0) {
        *arg5 = temp_s3->unk8;
        if (arg6 != 0) {
            *arg6 = temp_s3->unkC >> 4;
        }
    }
    return 0;
}

typedef struct Func1502FFD8Arg0 {
    u8 pad0[1];
    u8 unk1;
    u8 pad2[2];
    u8 unk4;
    u8 pad5[3];
    u8 unk8;
    u8 unk9;
    u8 padA[0xD];
    s8 unk17;
    u8 pad18[0xC];
    s32 unk24;
    void *unk28;
    void *unk2C;
    s32 unk30;
    u8 pad34[0x14];
    void *unk48;
} Func1502FFD8Arg0;

typedef struct Func15030158Arg0 {
    u8 unk0;
    u8 unk1;
    u8 pad2[6];
    u8 unk8;
    u8 unk9;
    u8 padA[0x1A];
    s32 unk24;
    void *unk28;
    void *unk2C;
    s32 unk30;
    void *unk34;
    u8 pad38[0xC];
    void *unk44;
    void *unk48;
    void *unk4C;
    void *unk50;
    struct Func15030158Arg0 *unk54;
    struct Func15030158Arg0 *unk58;
} Func15030158Arg0;

s32 func_1502FFD8(Func1502FFD8Arg0 *arg0, u8 *arg1) {
    s32 sp3C;
    s32 flags;
    s32 sp34;

    if (arg0->unk24 == 0) {
        if (arg0->unk17 == -1) {
            if (func_1502FE10(arg0->unk1, &arg0->unk24, (u8 *)arg0 + 0x14, &sp3C, &arg0->unk30, 0, 0) != 0) {
                return 1;
            }
        } else {
            if (func_1503F62C(arg0->unk1, arg0->unk17, &arg0->unk24, (u8 *)arg0 + 0x14, &sp3C, &arg0->unk30, &arg0->unk48) != 0) {
                return 1;
            }
        }

        flags = arg1[0x66] & 0xC;
        sp34 = arg0->unk24 - sp3C;
        if (((arg0->unk4 & 0x10) == 0) && (flags == 8) && (arg0->unk30 != 0)) {
            arg0->unk28 = allocate_memory(sp34, 1, 2, 2);
            if (arg0->unk28 == 0) {
                return 1;
            }

            arg0->unk2C = allocate_memory(sp34, 1, 2, 2);
            if (arg0->unk2C == 0) {
                func_10004074(arg0->unk28);
                arg0->unk28 = 0;
                return 1;
            }

            bcopy((void *)sp3C, arg0->unk28, sp34);
            bcopy((void *)sp3C, arg0->unk2C, sp34);
            arg0->unk8 = 1;
        } else {
            arg0->unk28 = (void *)sp3C;
            arg0->unk2C = (void *)sp3C;
        }
        arg0->unk9 = 1;
    }
    return 0;
}

void func_15030158(Func15030158Arg0 *arg0, s32 arg1) {
    void *temp_a0;
    s32 *head;

    if (arg1 == 0) {
        func_15031C14(arg0);
    }
    if (arg0->unk9 != 0) {
        if (arg0->unk8 != 0) {
            if (arg1 != 0) {
                func_10004074(arg0->unk28);
                func_10004074(arg0->unk2C);
            } else {
                func_100043B4(arg0->unk28, 4);
                func_100043B4(arg0->unk2C, 4);
            }
        }
        temp_a0 = arg0->unk48;
        if (temp_a0 != 0) {
            func_1503F7B8(temp_a0);
        }
        func_1518CA04(arg0->unk1);
    }
    temp_a0 = arg0->unk34;
    if (temp_a0 != 0) {
        if (arg1 != 0) {
            func_10004074(temp_a0);
        } else {
            func_100043B4(temp_a0, 4);
        }
    }
    if (arg0->unk44 != 0) {
        void *temp_v0;

        temp_v0 = arg0->unk44;
        func_10004074(temp_v0);
    }
    temp_a0 = arg0->unk4C;
    if (temp_a0 != 0) {
        if (arg1 != 0) {
            func_10004074(temp_a0);
        } else {
            func_100043B4(temp_a0, 4);
        }
    }
    temp_a0 = arg0->unk50;
    if (temp_a0 != 0) {
        if (arg1 != 0) {
            func_10004074(temp_a0);
        } else {
            func_100043B4(temp_a0, 4);
        }
    }

    head = (s32 *)(s32)&D_800C3EE0;
    if ((s32)arg0 == *head) {
        *head = (s32)arg0->unk54;
    }
    if (arg0->unk58 != 0) {
        arg0->unk58->unk54 = arg0->unk54;
    }
    if (arg0->unk54 != 0) {
        arg0->unk54->unk58 = arg0->unk58;
    }
    func_10004074(arg0);
}

s32 func_15030310(struct127 *arg0, s32 arg1, s32 arg2);

s32 func_150302F0(void *arg0, s32 arg1) {
    return func_15030310(arg0, arg1, 0xFF);
}

struct S15030310 {
    u8 unk0;
    u8 pad1[5];
    u8 unk6;
    u8 pad7[0x4D];
    struct S15030310 *unk54;
};

s32 func_15030310(struct127 *arg0, s32 arg1, s32 arg2) {
    struct S15030310 *var;
    s32 ret;

    if (arg0->unique_id == 0) {
        return 0;
    }
    ret = 0;
    var = (struct S15030310 *)D_800C3EE0;
    while (var != 0) {
        struct S15030310 *next = var->unk54;

        if ((var->unk0 == arg0->unique_id) && (var->unk6 == arg1)) {
            if ((arg2 != 0xFF) && (arg2 != 0)) {
                arg2--;
                var = next;
                continue;
            } else {
                ret = 1;
                func_15030158(var, 0);
                if (arg2 == 0) {
                    break;
                }
            }
        }
        var = next;
    }
    return ret;
}

struct S150303E4 {
    u8 unk0;
    u8 pad1[0x53];
    struct S150303E4 *unk54;
};

s32 func_150303E4(struct127 *arg0) {
    struct S150303E4 *var;
    struct S150303E4 *next;
    s32 ret;

    if (arg0->unique_id == 0) {
        return 0;
    }

    var = (struct S150303E4 *)D_800C3EE0;
    ret = 0;
    if (var != 0) {
        do {
            next = var->unk54;
            if (var->unk0 == arg0->unique_id) {
                ret = 1;
                func_15030158(var, 0);
            }
            var = next;
        } while (next != 0);
    }
    return ret;
}

typedef struct S15031070Sub {
    u8 pad0[0x3E0];
    u8 *unk3E0[2];
    u8 *unk3E8[2];
    u8 pad3F0[6];
    u8 unk3F6;
} S15031070Sub;

typedef struct S15031070 {
    u8 pad0[2];
    u8 unk2;
    u8 pad3[0x1B];
    u16 unk1E;
    u16 unk20;
    u8 pad22[0x12];
    u8 *unk34;
    u8 pad38[0x10];
    S15031070Sub *unk48;
} S15031070;

typedef struct S15030468Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} S15030468Sub;

typedef struct S15030468 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
    u16 unkA[3];
    u8 pad10[6];
    u8 unk16;
    u8 unk17;
    s16 unk18;
    s16 unk1A;
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    s16 unk22;
    u8 pad24[0x10];
    u8 *unk34;
    u8 pad38[8];
    f32 unk40;
    S15030468Sub *unk44;
    S15031070Sub *unk48;
    u8 pad4C[8];
    struct S15030468 *unk54;
} S15030468;

typedef s32 (*S15030468Func)(S15030468 *, struct127 *);
extern S15030468Func D_800841C0[];
s32 func_15031070(S15031070 *arg0, struct127 *arg1, u8 **arg2, u8 **arg3);
void func_1503F4B0(S15031070Sub *);

void func_15030468(void) {
    S15030468 *var;
    S15030468 *next;
    struct127 *obj;
    u16 t;
    s32 ret;
    s32 i;
    s32 j;
    f32 tmp[4][4];
    f32 m[4][4];
    f32 res[4][4];
    f32 (*mf)[4];
    Mtx *mtx;
    s32 kill;
    f32 mfbuf[4][4];
    s32 pass;
    s32 flag;
    f32 inv;
    u8 *sp198;
    u8 *sp194;
    f32 m2[4][4];
    f32 res2[4][4];
    f32 m3[4][4];
    f32 res3[4][4];

    flag = 0;
    for (pass = 0; pass < 2; pass++) {
        if (pass == 1 && flag == 0) {
            break;
        }
        var = (S15030468 *)D_800C3EE0;
        while (var != NULL) {
            next = var->unk54;
            ret = 0;
            if (pass == 0) {
                if (var->unk1E != 0) {
                    flag = 1;
                    var = next;
                    continue;
                }
            } else if (var->unk1E == 0) {
                var = next;
                continue;
            }
            obj = func_15083E90(var->unk0);
            var->unk5 = 1;
            if (obj == NULL) {
                func_15030158((Func15030158Arg0 *)var, 0);
                var = next;
                continue;
            }
            if (var->unk7 != 0) {
                ret = D_800841C0[var->unk7](var, obj);
            }
            if (ret != 0) {
                func_15030158((Func15030158Arg0 *)var, 0);
                var = next;
                continue;
            }
            if (var->unk34 != NULL || var->unk48 != NULL) {
                if (obj->unk1D4 != NULL) {
                    i = var->unk1E;
                    if (var->unk1E != 0) {
                        ret = (s32)func_1503195C(obj, i, 0);
                        if (ret == 0) {
                            func_15030158((Func15030158Arg0 *)var, 0);
                            var = next;
                            continue;
                        }
                        if (func_15031070((S15031070 *)ret, obj, &sp198, &sp194) == 0) {
                            func_15030158((Func15030158Arg0 *)var, 0);
                            var = next;
                            continue;
                        }
                        mf = mfbuf;
                        if (((S15031070 *)ret)->unk48 != NULL) {
                            sp198 += var->unk20 << 6;
                            guMtxL2F(mf, (Mtx *)sp198);
                            mtx = (Mtx *)mf;
                        } else {
                            guMtxL2F(mf, (Mtx *)sp198);
                        }
                    } else {
                        mf = (f32 (*)[4])((u8 *)obj->unk1D4 + (var->unk2 << 6));
                    }
                    if (var->unk34 != NULL) {
                        mtx = (Mtx *)var->unk34;
                        mtx += D_800BE9C0;
                        if (var->unk44 != NULL) {
                            for (i = 0; i < 3; i++) {
                                inv = 1.0f / sqrtf(mf[i][0] * mf[i][0] + mf[i][1] * mf[i][1] + mf[i][2] * mf[i][2]);
                                for (j = 0; j < 3; j++) {
                                    tmp[i][j] = mf[i][j] * inv;
                                }
                                tmp[i][3] = 0.0f;
                                tmp[3][i] = mf[3][i];
                            }
                            tmp[3][3] = 1.0f;
                            func_150A9B0C(m, var->unk44->unkC, var->unk44->unk10, var->unk44->unk14, var->unk40, var->unk40, var->unk40);
                            m[3][0] = var->unk44->unk0;
                            m[3][1] = var->unk44->unk4;
                            m[3][2] = var->unk44->unk8;
                            m[0][3] = 0.0f;
                            m[1][3] = 0.0f;
                            m[2][3] = 0.0f;
                            m[3][3] = 1.0f;
                            func_150A7A48(m, tmp, res);
                            guMtxF2L(res, mtx);
                        } else if (var->unk16 == 4) {
                            for (i = 0; i < 3; i++) {
                                inv = sqrtf(mf[i][0] * mf[i][0] + mf[i][1] * mf[i][1] + mf[i][2] * mf[i][2]);
                                if (i == 1) {
                                    inv = var->unk40 / inv;
                                } else {
                                    inv = var->unk40 / inv;
                                }
                                for (j = 0; j < 3; j++) {
                                    tmp[i][j] = mf[i][j] * inv;
                                }
                                tmp[i][3] = 0.0f;
                                tmp[3][i] = mf[3][i];
                            }
                            tmp[3][3] = 1.0f;
                            if (var->unk22 != 0) {
                                func_150A8050(m2, 0.0f, 0.0f, var->unk22 * 0.0054931640625f);
                                func_150A7A48(m2, tmp, res2);
                                guMtxF2L(res2, mtx);
                            } else {
                                guMtxF2L(tmp, mtx);
                            }
                        } else {
                            mf[0][3] = 0.0f;
                            mf[1][3] = 0.0f;
                            mf[2][3] = 0.0f;
                            mf[3][3] = 1.0f;
                            if (var->unk22 != 0) {
                                func_150A8050(m3, 0.0f, 0.0f, var->unk22 * 0.0054931640625f);
                                func_150A7A48(m3, mf, res3);
                                guMtxF2L(res3, mtx);
                            } else {
                                guMtxF2L(mf, mtx);
                            }
                        }
                    }
                } else {
                    mf = NULL;
                    mtx = NULL;
                }
            }
            if (var->unk48 != NULL) {
                if (var->unk34 != NULL) {
                    var->unk48->unk3E0[D_800BE9C0] = (u8 *)mtx;
                } else {
                    var->unk48->unk3E0[D_800BE9C0] = (u8 *)mf;
                }
                func_1503F4B0(var->unk48);
            }
            if (var->unkA[0] != 0) {
                kill = 1;
                for (i = 0; i < 3; i++) {
                    if (var->unkA[i] != 0 && obj->unk84.uh == var->unkA[i]) {
                        kill = 0;
                        break;
                    }
                }
                if (kill) {
                    func_15030158((Func15030158Arg0 *)var, 0);
                    var = next;
                    continue;
                }
            }
            if ((t = var->unk1C) != 0xFFFF) {
                if (t != 0) {
                    if (t > D_800BE9E4) {
                        var->unk1C = t - D_800BE9E4;
                    } else {
                        var->unk1C = 0;
                    }
                } else {
                    func_15030158((Func15030158Arg0 *)var, 0);
                }
            }
            var = next;
        }
    }
}

typedef struct S15030AF4 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u8 pad10[3];
    u8 unk13;
    u8 pad14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    s16 unk18;
    s16 unk1A;
    u16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    void *unk34;
    s32 unk38;
    s32 unk3C;
    f32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    struct S15030AF4 *unk54;
    struct S15030AF4 *unk58;
} S15030AF4;

void func_15031A50(u8 *arg0, u8 *arg1);

S15030AF4 *func_15030AF4(struct127 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, u16 *arg9, s32 argA, s32 argB, s32 argC) {
    S15030AF4 *var;
    S15030AF4 *next;
    s32 err;

    if (arg0->unique_id == 0) {
        return 0;
    }
    if (!(arg3 & 8)) {
        var = (S15030AF4 *)D_800C3EE0;
        if (var != 0) {
            do {
                next = var->unk54;
                if ((var->unk0 == arg0->unique_id) && (var->unk6 == arg7) && (var->unk1 == arg1)) {
                    return 0;
                }
                var = next;
            } while (next != 0);
        }
    }
    var = allocate_memory(0x5C, 1, 0, 2);
    if (var == 0) {
        return 0;
    }
    err = 0;
    if ((S15030AF4 *)D_800C3EE0 != 0) {
        ((S15030AF4 *)D_800C3EE0)->unk58 = var;
    }
    var->unk54 = (S15030AF4 *)D_800C3EE0;
    var->unk58 = 0;
    D_800C3EE0 = (s32)var;
    var->unk0 = arg0->unique_id;
    var->unk1 = arg1;
    var->unk2 = arg2;
    var->unk24 = 0;
    var->unk28 = 0;
    var->unk2C = 0;
    var->unk30 = 0;
    var->unk3 = 0xFF;
    var->unk44 = 0;
    var->unk4 = arg3;
    var->unk6 = arg7;
    var->unk40 = 1.0f;
    var->unk7 = arg8;
    var->unk38 = 0;
    var->unk3C = 0;
    var->unk8 = 0;
    var->unk9 = 0;
    var->unk18 = 0;
    var->unk1A = 0;
    var->unkA = arg9[0];
    var->unkC = arg9[1];
    var->unkE = arg9[2];
    var->unk17 = argA;
    var->unk48 = 0;
    var->unk15 = arg4;
    var->unk16 = arg5;
    var->unk1E = argB;
    var->unk20 = argC;
    var->unk13 = 0;
    var->unk4C = 0;
    var->unk50 = 0;
    var->unk5 = 0;
    var->unk22 = 0;
    if ((arg5 == 4) || (arg3 & 4)) {
        var->unk34 = allocate_memory(0x80, 1, 2, 2);
        if (var->unk34 == 0) {
            err = 1;
        }
    } else {
        var->unk34 = 0;
    }
    if (arg6 == 0) {
        var->unk1C = 0xFFFF;
    } else {
        var->unk1C = arg6;
    }
    if (!err) {
        err |= func_1502FFD8((Func1502FFD8Arg0 *)var, (u8 *)arg0);
    }
    if (err) {
        func_15030158((Func15030158Arg0 *)var, 1);
        return 0;
    }
    func_15031A50((u8 *)var, (u8 *)arg0);
    return var;
}

extern void *allocate_memory(s32 size, s32 arg1, s32 arg2, s32 arg3);
void func_15030D54(void *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6)
{
  f32 *temp;
  temp = allocate_memory(0x18, 1, 0, 2);
  *((f32 **) (((u8 *) arg0) + 0x44)) = temp;
  if (temp)
  {
    temp[((0, (*((s32 *) (((u8 *) arg0) + 0x34))) == 0)) * 0] = arg1;
    *((f32 *) ((*((s32 *) (((u8 *) arg0) + 0x44))) + 0x4)) = arg2;
    *((f32 *) ((*((s32 *) (((u8 *) arg0) + 0x44))) + 0x8)) = arg3;
    *((f32 *) ((*((s32 *) (((u8 *) arg0) + 0x44))) + 0xC)) = arg4;
    *((f32 *) ((*((s32 *) (((u8 *) arg0) + 0x44))) + 0x10)) = arg5;
    temp = ((u8 *) arg0) + 0x44;
    *((f32 *) ((*((s32 *) temp)) + 0x14)) = arg6;
    if (0, (*((s32 *) (((u8 *) arg0) + 0x34))) == 0)
    {
      *((void **) (((u8 *) arg0) + 0x34)) = allocate_memory(0x80, 1, 2, 2);
    }
  }
}


struct S15030F94 {
    u8 unk0;
    u8 pad1[2];
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 pad6[0xD];
    u8 unk13;
    u8 unk14;
    u8 pad15[3];
    u16 unk18;
    u16 unk1A;
    u8 pad1C[8];
    Gfx **unk24;
    Gfx *unk28[2];
    u8 pad30[0x24];
    struct S15030F94 *unk54;
};

s32 func_1506196C(struct127 *, s32);
Gfx *func_150311C4(Gfx *, struct S15030F94 *, struct127 *, s32, s32);

s32 func_15030E08(s32 arg0, s32 arg1, s32 arg2) {
    struct S15030F94 *var;
    struct S15030F94 *next;
    struct127 *obj;
    s32 v;

    var = (struct S15030F94 *)D_800C3EE0;
    while (var != NULL) {
        next = var->unk54;
        if (var->unk3 == 0) {
            var = next;
            continue;
        }
        obj = func_15083E90(var->unk0);
        if ((obj == NULL) || (obj->unk1D4 == NULL) || ((obj->unk74 & (1 << arg1)) == (1 << arg1))) {
            var = next;
            continue;
        }
        if (arg2 == 2) {
            if (obj->unk5 != 7) {
                var = next;
                continue;
            }
        } else {
            v = func_1506196C(obj, arg1);
            if (arg2 == 1) {
                if (!(var->unk4 & 2) && (v == 0xFF) && (obj->unk5 != 1)) {
                    var = next;
                    continue;
                }
            } else if ((var->unk4 & 2) || (v < 0xFF) || (obj->unk5 == 1)) {
                var = next;
                continue;
            }
        }
        arg0 = (s32)func_150311C4((Gfx *)arg0, var, obj, arg1, 0);
        var = next;
    }
    return arg0;

}


s32 func_15030F94(s32 arg0, struct127 *arg1, s32 arg2, s32 arg3) {
    struct S15030F94 *var;

    if ((arg1 == NULL) || (arg1->unk1D4 == NULL) || (arg1->unique_id == 0)) {
        return arg0;
    }
    var = (struct S15030F94 *)D_800C3EE0;
    while (var != NULL) {
        struct S15030F94 *next = var->unk54;

        if ((var->unk0 != arg1->unique_id) || (var->unk3 == 0)) {
            var = next;
            continue;
        }
        arg0 = (s32)func_150311C4((Gfx *)arg0, var, arg1, arg2, arg3);
        var = next;
    }
    return arg0;
}


s32 func_15031070(S15031070 *arg0, struct127 *arg1, u8 **arg2, u8 **arg3) {
    S15031070 *other;

    if (arg0->unk48 != NULL) {
        if (arg0->unk48->unk3F6 == 0) {
            return 0;
        }
        *arg2 = arg0->unk48->unk3E8[D_800BE9C0];
        *arg3 = arg0->unk48->unk3E0[D_800BE9C0];
    } else if (arg0->unk34 != NULL) {
        *arg2 = arg0->unk34 + (D_800BE9C0 << 6);
        *arg3 = (u8 *)arg1->unk1D4 + (arg0->unk2 << 6);
    } else if (arg0->unk1E) {
        other = (S15031070 *)func_1503195C(arg1, arg0->unk1E, 0);
        if (other == NULL) {
            return 0;
        }
        if (func_15031070(other, arg1, arg2, arg3) == 0) {
            return 0;
        }
        {
            s32 pad;
            *arg2 += arg0->unk20 << 6;
        }
        return 1;
    } else {
        *arg2 = (u8 *)arg1->unk1D4;
        *arg2 += arg0->unk2 << 6;
        *arg3 = *arg2;
    }
    return 1;
}

extern u8 D_80082FC0[];
extern u8 D_80083140[];
extern u8 D_800D9E28[];
extern u8 D_800DD2E4[];
extern Gfx *D_800BE9C8[];
extern s32 D_800BEBA4;
extern void func_1502D54C(s32, s32 *);
extern void func_1502D630(struct127 *, s32 *, s32, s32);
extern void func_1502CC34(struct127 *, s32, s32, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *);
extern void func_150832AC(struct127 *);
extern s32 func_1510D0EC(void *, s32 *, s32, s32);
extern Gfx *func_1515D914(Gfx *, s32, s32, s32, s32, s32, s32, s32, void *, void *, s32, void *, s32, void *);
extern Gfx *func_1515E544(Gfx *, s32, s32, s32, void *);
extern Gfx *func_15035D6C(Gfx *, struct S15030F94 *, s32, s32, s32);

#define OBJ_U8(o, off) (*(u8 *)((u8 *)(o) + (off)))
#define OBJ_S32(o, off) (*(s32 *)((u8 *)(o) + (off)))

Gfx *func_150311C4(Gfx *gdl, struct S15030F94 *arg1, struct127 *arg2, s32 arg3, s32 arg4) {
    u8 *sp114;
    u8 *sp110;
    s32 sp104[3];
    s32 sp100;
    s32 spFC;
    s32 spF8;
    s32 spF4;
    s32 spF0;
    s32 spEC;
    s32 spE8;
    s32 spE4;
    s32 spE0;
    s32 spDC;
    s32 spD8;
    s32 spD4;
    s32 spD0;
    s32 spBC[5];
    u16 t;
    Gfx *orig;
    s32 v;
    s32 doit;
    s32 called;

    orig = gdl;
    if (arg1->unk5 == 0) {
        return gdl;
    }
    gDPPipeSync(gdl++);
    spD0 = arg1->unk3;
    v = func_1506196C(arg2, arg3);
    if (v != 0xFF) {
        spD0 = (s32)((u32)v * spD0) >> 8;
    }
    if (spD0 < 0xFF) {
        gSPSegment(gdl++, 8, D_80082FC0);
    } else {
        gSPSegment(gdl++, 8, D_80083140);
    }
    if (func_15031070((S15031070 *)arg1, arg2, &sp114, &sp110) == 0) {
        return gdl;
    }
    func_1502D54C(arg2 - D_800CC2D0, spBC);
    if (((s16 *)D_800B0DF0)[0x3E / 2] != 0) {
        func_1502D630(&D_800CC2D0[arg2 - D_800CC2D0], spBC, arg3, arg2 - D_800CC2D0);
    } else {
        spBC[3] = 0xFF;
    }
    func_1502CC34(arg2, arg3, spD0, spBC, &spF8, &spF4, &spF0, &spEC, &spE8, &spE4, &spE0, &spDC, &spD8, &spD4);
    doit = 1;
    sp100 = ((s16 *)D_800B0DF0)[0x3E / 2];
    if ((OBJ_U8(arg2, 0x66) & 0xC) == 0xC) {
        gSPSetGeometryMode(gdl++, 0x20000);
        gSPClearGeometryMode(gdl++, 0x400000);
    } else if (((OBJ_U8(arg2, 0x66) & 0xC) != 8) && !(arg1->unk4 & 1)) {
        gSPClearGeometryMode(gdl++, 0x420000);
        doit = 0;
    } else if (D_800DCD7C != 0) {
        gSPSetGeometryMode(gdl++, 0x420000);
    } else {
        gSPClearGeometryMode(gdl++, 0x420000);
        doit = 0;
    }
    if (doit != 0) {
        if (OBJ_S32(arg2, arg3 * 4 + 0x304) == 0) {
            func_150832AC(arg2);
            called = 1;
        } else {
            called = 0;
        }
        if (OBJ_S32(arg2, arg3 * 4 + 0x304) != 0) {
            if ((OBJ_U8(arg2, 0x2FD) != 0) || (called != 0)) {
                v = 0xA;
            } else {
                v = 2;
            }
            if ((arg2->unk5 != 5) && (OBJ_U8(arg2, 0x302) != 0) && (OBJ_U8(arg2, 0x66) & 0xC) && (called == 0)) {
                gdl = func_1515E544(gdl, OBJ_S32(arg2, arg3 * 4 + 0x304), OBJ_U8(arg2, 0x301), OBJ_U8(arg2, 0x302),
                                    (u8 *)OBJ_S32(arg2, 0x314) + D_800BE9C0 * 8);
            } else {
                gdl = func_1515D914(gdl, arg3, (s32)arg2->x_position, (s32)arg2->y_position, (s32)arg2->z_position,
                                    OBJ_S32(arg2, 0x184), OBJ_S32(arg2, arg3 * 4 + 0x304), OBJ_U8(arg2, 0x301),
                                    (u8 *)OBJ_S32(arg2, 0x314) + D_800BE9C0 * 8, (u8 *)arg2 + 0x302, 2,
                                    (u8 *)arg2 + 0x1DD, v, D_800D9E28);
            }
        }
    }
    if (sp100 == 0) {
        gDPSetFogColor(gdl++, spE0, spDC, spD8, spD4);
    } else {
        gDPSetFogColor(gdl++, D_800DD2E4[0], D_800DD2E4[1], D_800DD2E4[2], spD4);
    }
    gDPSetFogColor(gdl++, spE0, spDC, spD8, spD4);
    gDPSetPrimColor(gdl++, 0xF2, 0, spF8, spF4, spF0, 0);
    gDPSetEnvColor(gdl++, spEC, spE8, spE4, spD0);
    gSPSegment(gdl++, 3, sp114);
    gSPSegment(gdl++, 1, arg1->unk28[D_800BE9C0]);
    t = arg1->unk18;
    if (t != 0) {
        v = func_1510D0EC(t, sp104, 0x3E, 0);
        gSPSegment(gdl++, 6, v);
    }
    if (arg1->unk1A != 0) {
        v = func_1510D0EC(arg1->unk1A, sp104, 0x3E, 0);
        gSPSegment(gdl++, 7, v);
    }
    if (arg4 == 0) {
        gSPSetGeometryMode(gdl++, 0x400);
        for (called = 0; called < arg1->unk14; called++) {
            if (!(arg1->unk13 & (1 << called))) {
                gSPDisplayList(gdl++, arg1->unk24[called]);
            }
        }
    } else {
        gSPSetGeometryMode(gdl++, 0x200);
        gdl = func_15035D6C(gdl, arg1, spEC, spE8, spE4);
    }
    if (D_800BEBA4 < (gdl - D_800BE9C8[D_800BE9C0])) {
        v = 1;
    } else {
        v = 0;
    }
    if (v != 0) {
        return orig;
    }
    return gdl;
}

void func_1503192C(struct127 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    struct126 *temp;
    temp = func_1503195C(arg0, arg1, arg3);
    if (temp != 0) {
        *((u8 *)temp + 3) = arg2;
    }
}

struct S1503195C {
    u8 unk0;
    u8 pad1[5];
    u8 unk6;
    u8 pad7[0x4D];
    struct S1503195C *unk54;
};

struct126 *func_1503195C(struct127 *arg0, s32 arg1, s32 arg2) {
    s32 key;
    struct S1503195C *var;
    struct S1503195C *next;

    key = arg0->unique_id;
    if (key == 0) {
        return 0;
    }

    var = (struct S1503195C *)D_800C3EE0;
    if (var != 0) {
        do {
            next = var->unk54;
            if ((key == var->unk0) && (arg1 == var->unk6)) {
                if (arg2 != 0) {
                    arg2 -= 1;
                    var = next;
                } else {
                    arg2 -= 1;
                    return (struct126 *)var;
                }
            } else {
                var = next;
            }
        } while (var != 0);
    }
    return 0;
}

struct S150319CC {
    u8 unk0;
    u8 pad1[5];
    u8 unk6;
    u8 pad7[0x4D];
    struct S150319CC *unk54;
};
struct S150319CC *func_150319CC(s32 arg0, u8 *arg1) {
    struct S150319CC *var;
    struct S150319CC *next;
    u8 key;

    if (arg1 != 0) {
        var = (struct S150319CC *)D_800C3EE0;
        if (var != 0) {
            key = arg1[0x3B];
            do {
                next = var->unk54;
                if ((var->unk0 == key) && (var->unk6 == arg0)) {
                    return var;
                }
                var = next;
            } while (next != 0);
        }
    }
    var = (struct S150319CC *)D_800C3EE0;
    if (var != 0) {
        do {
            next = var->unk54;
            if (var->unk6 == arg0) {
                return var;
            }
            var = next;
        } while (next != 0);
    }
    return 0;
}

void func_150D5440(void *, u8, s32);
void func_150D3360(void *, u8, s32);
void func_151001B4();
void func_15163BE8(void *, u8, s32);
void func_151BD828(void *, u8, s32);
void func_151D74B0(void *, u8, s8, u8, s32);
extern s32 D_80090228;
extern s32 D_8009022C;

void func_15031A50(u8 *arg0, u8 *arg1) {
    s32 type = arg0[1];
    u8 *node;

    switch (type) {
    case 0x37:
        func_151001B4(arg1);
        break;
    case 0x5A:
        node = *(u8 **)(arg1 + 0x31C);
        if (node != 0) {
            *(u16 *)(node + 0x1A6) += 0xAA;
        }
        break;
    case 0x90:
        *(u32 *)(arg1 + 0x9C) |= 0x70;
        break;
    case 0x8F:
        *(u32 *)(arg1 + 0x9C) |= 0xE00;
        break;
    case 0x49:
        func_15163BE8(arg1, 0xFF, 1);
        break;
    case 0x5D:
        func_150D3360(arg1, 0xFF, 1);
        func_150D5440(arg1, 0xFF, 1);
        break;
    case 0x3D:
        func_151BD828(arg1, 0xFF, 1);
        break;
    case 0x1D:
        func_151D74B0(arg1, 0, 2, 0xFF, 1);
        break;
    case 0x5E:
    case 0x85:
        *(u32 *)(arg1 + 0x9C) |= 0x6000;
        break;
    case 0x8D:
        if (func_150859AC(0, 6) < 0x64) {
            *(s16 *)(arg0 + 0x18) = D_80090228;
        } else {
            *(s16 *)(arg0 + 0x18) = D_8009022C;
        }
        break;
    case 0x82:
        func_151D74B0(arg1, 6, -1, 0xFF, 1);
        break;
    }
}

struct S15031C14 {
    s32 unk0;
    u8 unk4;
};
void func_15100180(void *);
void func_151616D0(u8, u8, void *);
void func_15147D64(void *, s32);
void func_151BD7F4(void *);
void func_151D4668(void *);
void func_151D747C(void *);
void func_151027E8(void *);

void func_15031C14(void *arg0) {
    u8 *obj;
    struct S15031C14 sp2C;
    struct S15031C14 sp24;
    u8 *node;
    struct S15031C14 * volatile p;

    obj = (u8 *)func_15083E90(*(u8 *)arg0);
    if (obj != 0) {
        switch (((u8 *)arg0)[1]) {
        case 0x5A:
            node = *(u8 **)(obj + 0x31C);
            if (node != 0) {
                *(u16 *)(node + 0x1A6) -= 0xAA;
            }
            break;
        case 0x90:
            *(u32 *)(obj + 0x9C) &= ~0x70;
            break;
        case 0x8F:
            *(u32 *)(obj + 0x9C) &= ~0xE00;
            break;
        case 0x37:
        case 0x4B:
        case 0x4C:
            func_1000FD38(func_15033BDC, arg0, (s32)obj);
            if (((u8 *)arg0)[1] == 0x37) {
                func_15100180(obj);
            }
            break;
        case 0x49:
            sp2C.unk0 = (s32)obj;
            sp2C.unk4 = obj[0x3B];
            func_151616D0(0x10, 0x29, &sp2C);
            break;
        case 0x5D:
            sp24.unk0 = (s32)obj;
            sp24.unk4 = obj[0x3B];
            func_15147D64(p = &sp24, 0x2E);
            func_151494E0((s32)p, 0x2F);
            break;
        case 0x3D:
            func_151BD7F4(obj);
            break;
        case 0x1A:
        case 0x1B:
        case 0x5F:
        case 0x65:
        case 0x66:
            func_151D4668(obj);
            break;
        case 0x1D:
        case 0x82:
            func_151D747C(obj);
            break;
        case 0x5E:
        case 0x85:
            *(u32 *)(obj + 0x9C) &= ~0x6000;
            break;
        }
        switch (((u8 *)arg0)[6]) {
        case 0x16:
        case 0x63:
        case 0x89:
            func_151027E8(obj);
            func_151D4668(obj);
            break;
        }
    }
}

s32 func_15031E2C(void *arg0, s32 arg1) {
    s32 idx;

    idx = *(s32 *)((u8 *)arg0 + 0x38);

    if (idx >= 3) {
        idx = 5 - idx;
    }
    *(s16 *)((u8 *)arg0 + 0x18) = D_800902BC[idx];
    *(s32 *)((u8 *)arg0 + 0x38) = *(s32 *)((u8 *)arg0 + 0x38) + 1;
    if (*(s32 *)((u8 *)arg0 + 0x38) >= 6) {
        *(s32 *)((u8 *)arg0 + 0x38) = 0;
    }
    return 0;
}

s32 func_15031E7C(u8 *arg0, struct127 *arg1) {
    struct197 *anim;
    Gfx *gfx;
    s32 count;
    s32 i;
    f32 scale;

    anim = arg1->unk2D0;
    if (anim == 0) {
        return 0;
    }

    gfx = **(Gfx ***)(arg0 + 0x24);
    if (gfx == 0) {
        return 0;
    }

    if (arg1->unk84.uh == 0x55) {
        scale = 1.0f;
    } else if (arg1->unk84.uh == 0x56) {
        scale = 0.0f;
    } else {
        if ((0.0f <= anim->unk8) && (anim->unk8 <= 120.0f)) {
            scale = anim->unk8 * D_800970DC;
            scale = 1.0f - scale;
        } else {
            scale = 0.0f;
        }
    }

    i = 0;
    count = 4;
    do {
        count--;
        if (*(s8 *)((i << 3) + (u8 *)gfx) != -0xE) {
            do {
            } while (*(s8 *)((u8 *)gfx + (++i << 3)) != -0xE);
        }
        if (count != 0) {
            i++;
        }
    } while (count != 0);

    gfx[i].words.w0 = (G_SETTILESIZE << 24) | (2 << 12) | ((s32)((25.0f * scale) + 2.0f) & 0xFFF);
    return 0;
}

typedef struct F15031FC8Anim {
    u8 pad0[0x4];
    u16 unk4;
    u8 pad6[0x2];
    f32 unk8;
    u8 padC[0xC];
    f32 unk18;
    u8 pad1C[0x1D];
    s8 unk39;
    s16 unk3A;
    s16 unk3C;
    u8 pad3E[0x215 - 0x3E];
    u8 unk215;
} F15031FC8Anim;

typedef struct F15031FC8Arg0 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3[0x15];
    s16 unk18;
    u8 pad1A[0x4];
    s16 unk1E;
    s16 unk20;
    u8 pad22[0x26];
    F15031FC8Anim *unk48;
} F15031FC8Arg0;

typedef struct F15031FC8Arg1 {
    u8 pad0[0x4];
    u8 unk4;
    u8 unk5;
    u8 pad6[0x1E];
    f32 unk24;
    u8 pad28[0x14];
    f32 unk3C;
    u8 pad40[0x44];
    u16 unk84;
    u8 pad86[0x2D0 - 0x86];
    F15031FC8Anim *unk2D0;
    u8 pad2D4[0x14];
    s32 unk2E8;
} F15031FC8Arg1;

extern s32 D_800902D0;
extern s32 D_800902D4;
void func_1503F5B8(s32 arg0, s32 arg1, s32 arg2, f32 arg3, f32 arg4, s32 arg5);
extern void func_1505E060(F15031FC8Anim *arg0);

s32 func_15031FC8(F15031FC8Arg0 *arg0, F15031FC8Arg1 *arg1) {
    F15031FC8Anim *sub;
    s32 anim;
    u8 pad1;
    s32 id;
    F15031FC8Anim *a;
    s32 cur;
    f32 lim;
    s32 pad;
    s32 flag;

    sub = arg1->unk2D0;
    anim = -1;
    a = arg0->unk48;
    if (a == NULL) {
        return 0;
    }
    cur = a->unk4 & ~0x8000;
    id = arg1->unk84;
    flag = 1;
    switch (arg1->unk4) {
    case 0x58:
        if (arg0->unk1 == 0x9B) {
            arg0->unk2 = 0x12;
        }
        switch (id) {
        case 39:
            anim = 3;
            break;
        case 40:
            anim = 4;
            break;
        case 2:
            anim = 1;
            break;
        case 3:
            anim = 0;
            break;
        default:
            anim = 0;
            if (arg0->unk1 == 0x9B) {
                anim = 3;
            }
            break;
        }
        break;
    case 0x5B:
        anim = 0;
        break;
    case 0x5A:
    case 0x74:
    case 0x7A:
        if (arg1->unk2E8 != 0) {
            arg0->unk18 = D_800902D4;
        } else {
            arg0->unk18 = D_800902D0;
        }
        switch (id) {
        case 6:
            anim = 7;
            break;
        case 38:
        case 39:
            anim = 0xF;
            break;
        case 1:
        case 42:
            anim = 3;
            break;
        case 0:
            anim = 5;
            break;
        case 3:
            anim = 6;
            break;
        case 7:
            anim = 2;
            break;
        case 10:
            anim = 4;
            break;
        case 15:
        case 30:
            anim = 8;
            break;
        case 19:
            anim = 0xD;
            break;
        case 4:
            anim = 9;
            break;
        case 5:
            anim = 0xA;
            break;
        case 16:
            anim = 0xB;
            break;
        case 8:
            anim = 0x10;
            break;
        case 20:
            anim = 0xE;
            break;
        case 21:
            anim = 0xE;
            break;
        case 0x50:
            anim = 0x13;
            break;
        case 0x51:
            anim = 0x14;
            break;
        default:
            anim = 6;
            break;
        }
        break;
    default:
        switch (arg0->unk1) {
        case 0x9E:
            arg0->unk2 = 9;
            arg0->unk1E = 0x41;
            arg0->unk20 = 1;
            switch (id) {
            case 0x1D6:
                anim = 1;
                break;
            case 0x1D7:
                anim = 2;
                arg0->unk2 = 0x13;
                break;
            case 0x257:
                anim = 4;
                arg0->unk2 = 0x13;
                break;
            case 0x275:
                anim = 5;
                arg0->unk2 = 0x13;
                break;
            case 0x1D8:
                anim = 3;
                break;
            }
            flag = 0;
            if (arg0->unk2 == 0x13) {
                arg0->unk1E = 0;
            }
            break;
        case 0x9B:
            anim = 0;
            if (arg1->unk5 == 5) {
                arg0->unk2 = 0;
            } else if (arg1->unk4 == 0x8B) {
                arg0->unk2 = 6;
                switch (id) {
                case 12:
                    anim = 6;
                    break;
                case 19:
                    anim = 7;
                    break;
                }
            } else {
                arg0->unk2 = 9;
                switch (id) {
                case 0x22E:
                    anim = 5;
                    break;
                case 0x1B7:
                    anim = 1;
                    break;
                case 0x1B8:
                    anim = 2;
                    break;
                }
            }
            break;
        case 0x9F:
            anim = 0;
            if (arg1->unk5 == 5) {
                arg0->unk2 = 0;
            } else if (arg1->unk4 == 0xB5) {
                arg0->unk2 = 0x12;
                switch (id) {
                case 0x34:
                    anim = 1;
                    break;
                case 0x35:
                    anim = 2;
                    break;
                case 0x36:
                    anim = 3;
                    break;
                case 0x37:
                    anim = 4;
                    break;
                }
            } else {
                arg0->unk2 = 9;
            }
            break;
        case 0x9C:
            anim = 8;
            break;
        case 0x9D:
            anim = 0;
            if (arg1->unk5 == 5) {
                arg0->unk2 = 0;
            } else if (arg1->unk4 == 0x8B) {
                arg0->unk2 = 6;
                switch (id) {
                case 19:
                case 32:
                    anim = 4;
                    break;
                case 34:
                    anim = 3;
                    break;
                }
            } else {
                arg0->unk2 = 9;
                switch (id) {
                case 0x21C:
                    anim = 1;
                    break;
                case 0x241:
                    anim = 2;
                    break;
                }
            }
            break;
        case 0x9A:
            switch (arg1->unk4) {
            case 0x87:
                arg0->unk2 = 0x12;
                switch (id) {
                case 1:
                    anim = 1;
                    break;
                case 5:
                    anim = 2;
                    break;
                default:
                    anim = 0;
                    break;
                }
                break;
            case 0x99:
                arg0->unk2 = 0x15;
                switch (id) {
                case 20:
                    anim = 3;
                    break;
                case 21:
                    anim = 4;
                    break;
                case 22:
                    anim = 5;
                    break;
                case 23:
                    anim = 6;
                    break;
                case 24:
                    anim = 7;
                    break;
                case 26:
                    anim = 8;
                    break;
                case 27:
                    anim = 9;
                    break;
                case 28:
                    anim = 0xA;
                    break;
                case 29:
                    anim = 0xB;
                    break;
                case 30:
                    anim = 0xC;
                    break;
                case 31:
                    anim = 0xD;
                    break;
                case 32:
                    anim = 0xE;
                    break;
                case 33:
                    anim = 0xF;
                    break;
                case 34:
                    anim = 0x10;
                    break;
                case 35:
                    anim = 0x11;
                    break;
                default:
                    anim = 0;
                    break;
                }
                break;
            }
            break;
        case 0x8D:
            switch (id) {
            case 0x323:
                anim = 1;
                break;
            case 0x324:
                anim = 2;
                break;
            default:
                anim = 0;
                break;
            }
            break;
        case 0x8E:
            switch (id) {
            case 0xB4:
                anim = 0;
                break;
            case 0xB6:
                anim = 1;
                break;
            case 0xB5:
                anim = 2;
                break;
            case 0xDD:
                anim = 3;
                break;
            case 0xDE:
                anim = 3;
                break;
            default:
                return 1;
            }
            break;
        case 0x8F:
        case 0x90:
            switch (id) {
            case 0xB9:
                anim = 1;
                break;
            case 0xAE:
                anim = 2;
                break;
            case 0xDA:
                anim = 3;
                break;
            case 0x104:
                anim = 4;
                break;
            case 0x107:
                anim = 5;
                break;
            case 0x10B:
                anim = 6;
                break;
            case 0x11E:
                anim = 7;
                break;
            case 0x11F:
                anim = 8;
                break;
            case 0x134:
                anim = 9;
                break;
            case 0x135:
                anim = 0xA;
                break;
            case 0x136:
                anim = 0xB;
                break;
            case 0x13B:
                anim = 0xD;
                break;
            case 0x13C:
                anim = 0xE;
                break;
            case 0x145:
                anim = 0xF;
                break;
            case 0x146:
                anim = 0x10;
                break;
            case 0x141:
                anim = 0x11;
                break;
            case 0x147:
                anim = 0x12;
                break;
            case 0x14B:
                anim = 0x16;
                break;
            case 0x14A:
                anim = 0x15;
                break;
            case 0x149:
                anim = 0x14;
                break;
            case 0x148:
                anim = 0x13;
                break;
            case 0x151:
                anim = 0x18;
                break;
            case 0x17F:
                anim = 0x1A;
                break;
            case 0x180:
                anim = 0x1B;
                break;
            case 0x181:
                anim = 0x1C;
                break;
            case 0x182:
                anim = 0x1D;
                break;
            case 0x186:
                anim = 0x1E;
                break;
            case 0x187:
                anim = 0x1F;
                break;
            case 0x189:
                anim = 0x20;
                break;
            case 0x18C:
                anim = 0x21;
                break;
            case 0x18D:
                anim = 0x22;
                break;
            case 0x18E:
                anim = 0x23;
                break;
            case 0x1A0:
                anim = 0x24;
                break;
            case 0x1A3:
                anim = 0x25;
                break;
            case 0x1B3:
                anim = 0x26;
                break;
            case 0x1B4:
                anim = 0x27;
                break;
            case 0x1B5:
                anim = 0x28;
                break;
            case 0x1B6:
                anim = 0x29;
                break;
            case 0x1B7:
                anim = 0x2A;
                break;
            case 0x1B8:
                anim = 0x2B;
                break;
            case 0x1BD:
                anim = 0x2C;
                break;
            case 0x1BE:
                anim = 0x2D;
                break;
            case 0x1BF:
                anim = 0x2E;
                break;
            case 0x1C0:
                anim = 0x2F;
                break;
            case 0x1C1:
                anim = 0x30;
                break;
            case 0x1C2:
                anim = 0x31;
                break;
            case 0x1C3:
                anim = 0x32;
                break;
            case 0x1C4:
                anim = 0x33;
                break;
            case 0x1C5:
                anim = 0x34;
                break;
            case 0x1C6:
                anim = 0x35;
                break;
            case 0x1C7:
                anim = 0x36;
                break;
            case 0x1C8:
                anim = 0x37;
                break;
            case 0x1C9:
                anim = 0x38;
                break;
            case 0x1CA:
                anim = 0x39;
                break;
            case 0x1D1:
                anim = 0x3A;
                break;
            case 0x1D2:
                anim = 0x3B;
                break;
            case 0x1D3:
                anim = 0x3C;
                break;
            case 0x1D5:
                anim = 0x3D;
                break;
            case 0x1D9:
                anim = 0x3E;
                break;
            case 0x1DA:
                anim = 0x3F;
                break;
            case 0x1DB:
                anim = 0x40;
                break;
            case 0x1E3:
                anim = 0x41;
                break;
            case 0x1E4:
                anim = 0x42;
                break;
            case 0x1E5:
                anim = 0x43;
                break;
            case 0x1E6:
                anim = 0x44;
                break;
            case 0x1E7:
                anim = 0x45;
                break;
            case 0x1E8:
                anim = 0x47;
                break;
            case 0x1E9:
                anim = 0x44;
                break;
            case 0x1F0:
                anim = 0x48;
                break;
            case 0x1F1:
                anim = 0x49;
                break;
            case 0x1F2:
                anim = 0x4A;
                break;
            case 0x201:
                anim = 0x4B;
                break;
            case 0x202:
                anim = 0x4D;
                break;
            case 0x203:
                anim = 0x4E;
                break;
            case 0x204:
                anim = 0x4F;
                break;
            case 0x200:
                anim = 0x50;
                break;
            case 0x205:
                anim = 0x51;
                break;
            case 0x208:
                anim = 0x52;
                break;
            case 0x209:
                anim = 0x53;
                break;
            case 0x20A:
                anim = 0x54;
                break;
            case 0x20B:
                anim = 0x55;
                break;
            case 0x20C:
                anim = 0x56;
                break;
            case 0x211:
                anim = 0x57;
                break;
            case 0x212:
                anim = 0x58;
                break;
            case 0x216:
                anim = 0x59;
                break;
            case 0x217:
                anim = 0x5A;
                break;
            case 0x218:
                anim = 0x5B;
                break;
            case 0x219:
                anim = 0x5C;
                break;
            case 0x21A:
                anim = 0x5D;
                break;
            case 0x21B:
                anim = 0x5E;
                break;
            case 0xA8:
                anim = 0x5F;
                break;
            case 0x1FD:
                anim = 0x4C;
                break;
            case 0x228:
                anim = 0x62;
                break;
            case 0x22E:
                anim = 0x63;
                break;
            case 0x22F:
                anim = 0x64;
                break;
            case 0x232:
                anim = 0x61;
                break;
            case 0x220:
                anim = 0x60;
                break;
            case 0x23A:
                anim = 0x65;
                break;
            case 0x23B:
                anim = 0x66;
                break;
            case 0x24D:
                anim = 0x6E;
                break;
            case 0x21C:
                anim = 0x6F;
                break;
            case 0x241:
                anim = 0x70;
                break;
            case 0x23C:
                anim = 0x67;
                break;
            case 0x24B:
                anim = 0x6D;
                break;
            case 0x24A:
                anim = 0x6C;
                break;
            case 0x249:
                anim = 0x6B;
                break;
            case 0x248:
                anim = 0x6A;
                break;
            case 0x246:
                anim = 0x69;
                break;
            case 0x245:
                anim = 0x68;
                break;
            case 0x25C:
                anim = 0x71;
                break;
            case 0x25D:
                anim = 0x72;
                break;
            case 0x25E:
                anim = 0x73;
                break;
            case 0x260:
                anim = 0x74;
                break;
            case 0x266:
                anim = 0x77;
                break;
            case 0x267:
                anim = 0x78;
                break;
            case 0x268:
                anim = 0x79;
                break;
            case 0x26A:
                anim = 0x7A;
                break;
            case 0x26B:
                anim = 0x7B;
                break;
            case 0x26C:
                anim = 0x7C;
                break;
            case 0x26D:
                anim = 0x7D;
                break;
            case 0x275:
                anim = 0x7E;
                break;
            case 0x1D6:
                anim = 0x7F;
                break;
            case 0x1D7:
                anim = 0x86;
                break;
            case 0x1D8:
                anim = 0x85;
                break;
            case 0x276:
                anim = 0x80;
                break;
            case 0x277:
                anim = 0x81;
                break;
            case 0x278:
                anim = 0x82;
                break;
            case 0x265:
                anim = 0x76;
                break;
            case 0x27F:
                anim = 0x83;
                break;
            case 0x280:
                anim = 0x84;
                break;
            case 0x281:
                anim = 0x87;
                break;
            case 0x282:
                anim = 0x88;
                break;
            case 0x290:
                anim = 0x89;
                break;
            case 0x291:
                anim = 0x8A;
                break;
            case 0x292:
                anim = 0x8B;
                break;
            case 0x293:
                anim = 0x8C;
                break;
            case 0x294:
                anim = 0x8D;
                break;
            case 0x295:
                anim = 0x8E;
                break;
            case 0x298:
                anim = 0x8F;
                break;
            case 0x29A:
                anim = 0x90;
                break;
            case 0x29B:
                anim = 0x91;
                break;
            case 0x29C:
                anim = 0x92;
                break;
            case 0x29D:
                anim = 0x93;
                break;
            case 0x2A9:
                anim = 0x94;
                break;
            case 0x2AA:
                anim = 0x95;
                break;
            case 0x2AC:
                anim = 0x96;
                break;
            case 0x2AD:
                anim = 0x97;
                break;
            case 0x2AE:
                anim = 0x98;
                break;
            case 0x2AF:
                anim = 0x99;
                break;
            case 0x2B0:
                anim = 0x9A;
                break;
            case 0x2B2:
                anim = 0x9B;
                break;
            case 0x2B3:
                anim = 0x9C;
                break;
            case 0x2B4:
                anim = 0x9D;
                break;
            case 0x2B5:
                anim = 0x9E;
                break;
            case 0x2B6:
                anim = 0x9F;
                break;
            case 0x2B8:
                anim = 0xA0;
                break;
            case 0x2B9:
                anim = 0xA1;
                break;
            case 0x2BA:
                anim = 0xA2;
                break;
            case 0x2BB:
                anim = 0xA3;
                break;
            case 0x2BC:
                anim = 0xA4;
                break;
            case 0x2BD:
                anim = 0xA5;
                break;
            case 0x2BE:
                anim = 0xA6;
                break;
            case 0x2BF:
                anim = 0xA7;
                break;
            case 0x2C0:
                anim = 0xA8;
                break;
            case 0x2C1:
                anim = 0xA9;
                break;
            case 0x2C4:
                anim = 0xAA;
                break;
            case 0x2CF:
                anim = 0xAB;
                break;
            case 0x2DA:
                anim = 0xAC;
                break;
            case 0x2DB:
                anim = 0xAD;
                break;
            case 0x2DD:
                anim = 0xAE;
                break;
            case 0x2DC:
                anim = 0xAF;
                break;
            case 0x2D9:
                anim = 0xB0;
                break;
            case 0x2DF:
                anim = 0xB1;
                break;
            case 0x2E0:
                anim = 0xB2;
                break;
            case 0x2E1:
                anim = 0xB3;
                break;
            case 0x2E2:
                anim = 0xB4;
                break;
            case 0x2E3:
                anim = 0xB5;
                break;
            case 0x2EC:
                anim = 0xB6;
                break;
            case 0x2ED:
                anim = 0xB7;
                break;
            case 0x2EE:
                anim = 0xB8;
                break;
            case 0x2EF:
                anim = 0xB9;
                break;
            case 0x2F0:
                anim = 0xBA;
                break;
            case 0x2F1:
                anim = 0xBB;
                break;
            case 0x2F8:
                anim = 0xBC;
                break;
            case 0x2F9:
                anim = 0xBD;
                break;
            case 0x301:
                anim = 0xBE;
                break;
            case 0x304:
                anim = 0xBF;
                break;
            case 0x305:
                anim = 0xC0;
                break;
            case 0x306:
                anim = 0xC1;
                break;
            case 0x307:
                anim = 0xC2;
                break;
            case 0x308:
                anim = 0xC3;
                break;
            case 0x309:
                anim = 0xC4;
                break;
            case 0x30A:
                anim = 0xC5;
                break;
            case 0x30B:
                anim = 0xC6;
                break;
            case 0x30C:
                anim = 0xC7;
                break;
            case 0x30D:
                anim = 0xC8;
                break;
            case 0x30E:
                anim = 0xC9;
                break;
            case 0x30F:
                anim = 0xCA;
                break;
            case 0x314:
                anim = 0xCB;
                break;
            case 0x315:
                anim = 0xCC;
                break;
            case 0x316:
                anim = 0xCD;
                break;
            case 0x317:
                anim = 0xCE;
                break;
            case 0x318:
                anim = 0xCF;
                break;
            case 0x319:
                anim = 0xD0;
                break;
            case 0x31C:
                anim = 0xD1;
                break;
            case 0x31D:
                anim = 0xD2;
                break;
            case 0x31E:
                anim = 0xD3;
                break;
            case 0x320:
                anim = 0xD4;
                break;
            case 0x31F:
                anim = 0xD5;
                break;
            case 0x321:
                anim = 0xD6;
                break;
            case 0x322:
                anim = 0xD7;
                break;
            case 0x323:
                anim = 0xD8;
                break;
            case 0x324:
                anim = 0xD9;
                break;
            case 0x326:
                anim = 0xDA;
                break;
            case 0x327:
                anim = 0xDB;
                break;
            case 0x328:
                anim = 0xDC;
                break;
            case 0x32B:
                anim = 0xDD;
                break;
            case 0x32C:
                anim = 0xDE;
                break;
            case 0x32D:
                anim = 0xDF;
                break;
            case 0x32E:
                anim = 0xE0;
                break;
            case 0x32F:
                anim = 0xE1;
                break;
            case 0x330:
                anim = 0xE2;
                break;
            case 0x331:
                anim = 0xE3;
                break;
            case 0x332:
                anim = 0xE4;
                break;
            case 0x333:
                anim = 0xE5;
                break;
            case 0x334:
                anim = 0xE6;
                break;
            case 0x335:
                anim = 0xE7;
                break;
            case 0x345:
                anim = 0xE8;
                break;
            case 0x347:
                anim = 0xEA;
                break;
            case 0x348:
                anim = 0xEB;
                break;
            case 0x349:
                anim = 0xEC;
                break;
            case 0x34A:
                anim = 0xED;
                break;
            case 0x34B:
                anim = 0xEE;
                break;
            case 0x34C:
                anim = 0xEF;
                break;
            case 0x34D:
                anim = 0xF0;
                break;
            case 0x240:
                anim = 0xE9;
                break;
            default:
                anim = 0;
                break;
            }
            break;
        case 0x83:
            switch (id) {
            case 0x5E:
                anim = 0;
                break;
            case 0x5F:
            case 0xFA:
                anim = 1;
                break;
            case 0x60:
                anim = 2;
                break;
            case 0xB3:
                anim = 3;
                break;
            default:
                return 1;
            }
            break;
        case 0x85:
            if (id == 0x13E) {
                anim = 5;
            } else if (id == 0x13F) {
                anim = 4;
            } else if (id == 0x7C) {
                anim = 2;
            } else if (arg1->unk3C > 0.0f) {
                anim = 1;
            } else {
                anim = 0;
            }
            break;
        case 0x88:
            switch (id) {
            default:
                anim = 2;
                break;
            case 0x11:
                anim = 0;
                break;
            case 0x26:
                anim = 1;
                break;
            }
            break;
        case 0x98:
            if (arg1->unk4 == 0x8B) {
                arg0->unk2 = 6;
                anim = 5;
                break;
            }
            switch (id) {
            case 0x12C:
                anim = 0;
                break;
            case 0x12D:
                anim = 3;
                break;
            case 0x142:
                anim = 4;
                break;
            case 0x12E:
            case 0x358:
                anim = 2;
                break;
            case 0x2AA:
                anim = 6;
                break;
            default:
                return 1;
            }
            break;
        case 0x87:
            switch (id) {
            case 0x172:
                anim = 0;
                break;
            case 0x174:
                anim = 2;
                break;
            case 0x17A:
                anim = 3;
                break;
            case 0x171:
            case 0x173:
                anim = 1;
                break;
            default:
                return 1;
            }
            break;
        case 0x89:
            if (arg1->unk5 == 5) {
                anim = 3;
                arg0->unk2 = 0;
                break;
            }
            switch (id) {
            case 0x49:
                anim = 0;
                arg0->unk2 = 9;
                break;
            case 0x47:
                anim = 1;
                arg0->unk2 = 9;
                break;
            case 0x81:
                anim = 2;
                arg0->unk2 = 0x13;
                break;
            default:
                return 1;
            }
            break;
        case 0x91:
            if (arg1->unk5 == 5) {
                arg0->unk2 = 0;
                if (arg1->unk24 == 0.0f) {
                    anim = 3;
                } else {
                    anim = 0;
                }
            } else {
                arg0->unk2 = 0xE;
                switch (id) {
                default:
                    anim = 4;
                    break;
                case 0x12:
                    anim = 2;
                    break;
                case 0x10:
                    anim = 1;
                    break;
                }
            }
            break;
        }
        break;
    }
    if (anim != -1) {
        func_1503F5B8((s32)arg0->unk48, 0, anim, 0.0f, 0.0f, 1);
    }
    if (sub != NULL) {
        a = arg0->unk48;
        if (anim != cur) {
            goto skip1;
        }
        if (sub->unk3C != 0x3FF) {
            goto skip1;
        }
        func_1505E060(a);
        a = arg0->unk48;
    skip1:
        a->unk8 = sub->unk8;
        a = arg0->unk48;
        if (flag && (a->unk39 != 0 || a->unk215 == 0)) {
            a->unk3A = sub->unk3A;
            arg0->unk48->unk3C = sub->unk3C;
            a = arg0->unk48;
        }
        lim = a->unk18 - 1.0f;
        if (lim <= a->unk8) {
            a->unk8 = lim;
        }
    }
    return 0;
}

/* D_800841C0 (.data) holds this function's address: restores as1's indirect-jump-target view. */
static s32 (*const D_800841D4_entry[])(F15031FC8Arg0 *, F15031FC8Arg1 *) = { func_15031FC8 };


void func_1503F5B8(s32 arg0, s32 arg1, s32 arg2, f32 arg3, f32 arg4, s32 arg5);

s32 func_150331B8(Game5D2C0AnimationOwner *arg0, struct127 *arg1) {
    struct197 *ptr;
    struct197 *temp;

    ptr = arg1->unk2D0;
    temp = arg0->field_0x48;
    if (temp == 0) {
        return 0;
    }
    if ((arg1->unk2E4 & 0xFF) != 0xFF) {
        func_1503F5B8((s32)temp, 0, arg1->unk2E4 & 0xFF, 1.0f, 0.0f, 1);
    }
    if (ptr != 0) {
        arg0->field_0x48->unk8 = ptr->unk8;
        temp = arg0->field_0x48;
        if (temp->unk18 <= temp->unk8) {
            temp->unk8 = temp->unk18 - 1.0f;
        }
    }
    return 0;
}


s32 func_1503327C(Game5D2C0AnimationOwner *arg0, s32 arg1) {
    Game5D2C0AnimationState *ptr;

    ptr = (Game5D2C0AnimationState *)arg0->field_0x48;
    if (ptr == 0) {
        return 0;
    }
    if ((ptr->field_0x04 & 0x8000) != 0x8000) {
        func_1503F5B8((s32)ptr, 0, 0, 1.0f, 0.0f, 1);
        ptr = (Game5D2C0AnimationState *)arg0->field_0x48;
    }
    if (ptr->field_0x18 - 1.0f <= ptr->field_0x08) {
        return 1;
    }
    return 0;
}

s32 func_15033328(Game5D2C0EffectState *arg0, Game5D2C0PositionState *arg1) {
    s32 delta;

    if (D_800C35EA == 1) {
        return 0;
    }
    if (arg1->field_0xAD == 0) {
        if (arg1->field_0x118 < arg1->field_0x180) {
            delta = D_800BE9E4;
            arg1 = (Game5D2C0PositionState *)arg0->field_0x38;
            if (delta < (s32)arg1) {
                arg0->field_0x38 = (s32)arg1 - delta;
                return 0;
            } else {
                return 1;
            }
        }
    }
    arg0->field_0x38 = 0x1E;
    return 0;
}

s32 func_150333A8(Game5D2C0FlagState *arg0, Game5D2C0PositionState *arg1) {
    Game5D2C0ActorState *temp;

    if (D_800C35EA == 1) {
        return 0;
    }
    if (arg1->field_0xAD != 0) {
        temp = arg1->field_0x31C;
        if (temp != 0) {
            temp->field_0x11A = 0;
        }
        return 1;
    }
    if ((arg1->field_0x118 != D_80097B68) && (arg1->field_0x18 < arg1->field_0x118 + 300.0f)) {
        arg0->field_0x03 = 0;
    } else {
        arg0->field_0x03 = 0xFF;
    }
    return 0;
}

s32 func_15033440(Game5D2C0EffectState *arg0, Game5D2C0EventState *arg1) {
    s32 state;

    state = arg0->field_0x01;
    if (state == 0x27) {
        goto add_delta;
    }
    if (state == 0x29) {
        goto clear;
    }
    if (state != 0x35) {
        goto done;
    }

add_delta:
    if (arg1->field_0x05 != 5) {
        goto done;
    }
    arg0->field_0x02 = 0;
    arg0->field_0x22 += D_800BE9E4 * 0xAAA;
    goto done;

clear:
    if (arg1->field_0x05 != 5) {
        goto done;
    }
    arg0->field_0x02 = 0;

done:
    return 0;
}

s32 func_150334B8(arg0, arg1)
u8 *arg0;
s16 arg1;
{
    Gfx *gfx;
    s32 count;
    s32 i;
    s32 w0;
    s32 w1;
    s32 width;
    s32 s;
    s32 t;
    s32 height;
    s32 offset;

    gfx = 0;
    count = 0;
    offset = 0;
    if (arg0[1] == 0x37) {
        gfx = **(Gfx ***)(arg0 + 0x24);
        count = 4;
        offset = -0x64;
    }
    if (gfx != 0) {
        i = 0;
        if (count != 0) {
            do {
                count--;
                if (*(s8 *)((i << 3) + (u8 *)gfx) != -0xE) {
                    do {
                    } while (*(s8 *)((u8 *)gfx + (++i << 3)) != -0xE);
                }
                if (count != 0) {
                    i++;
                }
            } while (count != 0);
        }

        w0 = gfx[i].words.w0;
        w1 = gfx[i].words.w1;
        width = ((w1 >> 12) & 0xFFF) + 2;
        s = ((w0 >> 12) & 0xFFF) + offset;
        t = w0 & 0xFFF;
        if (s >= width) {
            s -= width;
        }
        if (s < 0) {
            s += width;
        }
        height = (w1 & 0xFFF) + 2;
        if (t >= height) {
            t -= height;
        }
        if (t < 0) {
            t += height;
        }
        gfx[i].words.w0 = (G_SETTILESIZE << 24) | ((s & 0xFFF) << 12) | (t & 0xFFF);
    }
    return 0;
}

void *func_150335C8(struct127 *arg0, struct127 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    Game5D2C0RenderState *temp_v0;
    f32 sp124[4][4];
    f32 spE4[4][4];
    f32 spA4[4][4];
    f32 sp64[4][4];
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 scale;

    if (arg1->unk1D4 == 0) {
        return 0;
    }

    temp_v0 = func_15083568(arg1, arg2, 1.0f, 0);
    if (temp_v0 != 0) {
        temp_v0->field_0x02 = arg3;
        temp_v0->field_0x40 = arg0->xz_scale;

        if (arg4 != 0) {
            temp_v0->field_0x16 = temp_v0->field_0x16 & ~4;
        } else {
            temp_v0->field_0x16 = temp_v0->field_0x16 | 4;
        }

        guMtxL2F(sp124, (Mtx *)((u8 *)arg1->unk1D4 + (arg3 << 6)));
        func_15048B10((s32)sp124, (s32)spE4);

        scale = arg0->xz_scale;
        func_150A9B0C(spA4, arg0->unkB8, arg0->unk40, arg0->unkC4, scale, arg0->y_scale, scale);

        spA4[3][0] = arg0->x_position;
        spA4[3][1] = arg0->y_position;
        spA4[3][2] = arg0->z_position;
        spA4[0][3] = 0.0f;
        spA4[1][3] = 0.0f;
        spA4[2][3] = 0.0f;
        spA4[3][3] = 1.0f;

        func_150A7A48(spA4, spE4, sp64);
        func_1503E5F8(sp64, &sp60, &sp5C, &sp58, &sp48, &sp44, &sp40, &sp54, &sp50, &sp4C);

        if (arg5 != 0) {
            sp58 = 0.0f;
            sp5C = 0.0f;
            sp60 = 0.0f;
        }

        func_15030D54(temp_v0, sp60, sp5C, sp58, sp48, sp44, sp40);
        return temp_v0;
    }

    return 0;
}

s32 func_1503378C(u8 *arg0, s16 *arg1) {
    u16 temp = (u16)arg1[0x42];
    if (arg0[1] == 0x11) {
        if ((temp == 0x3E) || (temp == 0x3D) || (temp == 0x41) || (temp == 0xD9) || (temp == 0x138) || (temp == 0x139)) {
            return 0;
        }
    }
    return 1;
}

extern s32 D_800902FC[];

s32 func_150337E4(void *arg0, s32 arg1) {
    s32 *p = (s32 *)arg0;
    p[0xE] += D_800BE9E4;
    if (p[0xE] >= 0x10) {
        p[0xE] = 0;
        p[0xF] ^= 1;
    }
    *(s16 *)((u8 *)arg0 + 0x18) = D_800902FC[p[0xF]];
    return 0;
}

void func_151026BC(void *, s32, s32, s32, s32, s32);
void func_151027E8(void *);

s32 func_15033838(u8 *arg0, u8 *arg1) {
    s32 var_a2;
    s32 var_v1;
    u8 *temp_v0_3;

    var_v1 = 0;
    if (arg0[6] == 0x16) {
        var_a2 = 4;
        if (*(u16 *)(arg1 + 0x84) == 0x165) {
            var_v1 = 1;
        }
    } else if (arg0[6] == 0x89) {
        var_a2 = 6;
        switch (*(u16 *)(arg1 + 0x84)) {
        case 0x221:
        case 0x223:
        case 0x31B:
            if ((*(u8 **)(arg1 + 0x31C))[0x198] == 2) {
                var_v1 = 1;
            }
            break;
        }
    } else {
        var_a2 = 5;
        if (*(u16 *)(arg1 + 0x84) == 0x157) {
            var_v1 = 1;
        }
    }
    if (*(s32 *)(arg0 + 0x38) == 0) {
        if (var_v1 != 0) {
            temp_v0_3 = *(u8 **)(arg1 + 0x31C);
            if ((temp_v0_3[0x197] != 0) || (var_a2 == 6)) {
                if ((*(u16 *)(temp_v0_3 + 0x8A) & 0x2000) == 0x2000) {
                    if (*(u16 *)(temp_v0_3 + 0x19E) == 0) {
                        *(s32 *)(arg0 + 0x38) = 1;
                        func_151026BC(arg1, -1, var_a2, 1, 0xFF, 1);
                    }
                }
            }
        }
    } else if ((var_v1 == 0) || (temp_v0_3 = *(u8 **)(arg1 + 0x31C), ((*(u16 *)(temp_v0_3 + 0x8A) & 0x2000) == 0)) || ((temp_v0_3[0x197] == 0) && (var_a2 != 6))) {
        *(s32 *)(arg0 + 0x38) = 0;
        func_151027E8(arg1);
    }
    return 0;
}

s32 func_150339C8(Game5D2C0EffectState *arg0, Game5D2C0PositionState *arg1) {
    func_150334B8(arg0, arg1);
    if (D_800C35EA != 1) {
        if (D_800BEA0C != 0) {
            if (arg0->field_0x3C != 0) {
                func_1000FD38(func_15033BDC, arg0, (s32)arg1);
            }
            arg0->field_0x3C = 0;
        } else if (arg0->field_0x3C == 0) {
            arg0->field_0x3C = func_1000FA64(0x448,
                (s16)(s32)arg1->field_0x14,
                (s16)(s32)arg1->field_0x18,
                (s16)(s32)arg1->field_0x1C,
                0x7D00, 0x3E8, 0x1F4, (s32)func_15033BDC, arg0, (s32)arg1, 0, 0) | 0x80000000;
        }
    }
    return 0;
}

s32 func_15033BDC();

s32 func_15033AD8(Game5D2C0EffectState *arg0, Game5D2C0PositionState *arg1) {
    if (D_800BE616 != 0) {
        func_1508B20C(arg1->field_0x14, arg1->field_0x18, arg1->field_0x1C, 900.0f);
    }
    if (arg0->field_0x38 == 0) {
        if (arg0->field_0x3C < 0x1E) {
            arg0->field_0x3C += D_800BE9E4;
        } else {
            arg0->field_0x3C = func_1000FA64(0x513,
                (s16)(s32)arg1->field_0x14,
                (s16)(s32)arg1->field_0x18,
                (s16)(s32)arg1->field_0x1C,
                0x7D00, 0x3E8, 0x1F4, (s32)func_15033BDC, arg0, (s32)arg1, 0, 0);
            arg0->field_0x38 = 0x513;
        }
    }
    return 0;
}

typedef struct Game5D2C0SoundInstance {
    u16 field_0x00;
    s16 field_0x02;
    s16 field_0x04;
    s16 field_0x06;
    u8 pad_0x08[0x10];
    Game5D2C0EffectState *field_0x18;
    Game5D2C0PositionState *field_0x1C;
    u8 pad_0x20[0x4];
    u16 field_0x24;
} Game5D2C0SoundInstance;

void func_10010FFC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, void *arg5);

s32 func_15033BDC(Game5D2C0SoundInstance *arg0, s32 arg1, s32 *arg2, s32 arg3, s32 arg4, s32 arg5, s16 *arg6) {
    Game5D2C0EffectState *effect;
    Game5D2C0PositionState *pos;
    Game5D2C0ActorState *actor;

    effect = arg0->field_0x18;
    pos = arg0->field_0x1C;
    if ((effect != 0) && (pos != 0) && (pos->field_0x00 != 0)) {
        arg0->field_0x02 = pos->field_0x14;
        arg0->field_0x04 = pos->field_0x18;
        arg0->field_0x06 = pos->field_0x1C;

        if (*arg2 != 0) {
            if (effect->field_0x01 == 0x37) {
                s32 currentId = effect->field_0x38;
                s32 sfxId = -1;

                if (pos->field_0x84 != (currentId & 0xFFFF)) {
                    if (pos->field_0x84 == 0x15F) {
                        sfxId = (func_150ADA20() & 3) + 0x444;
                    }
                }
                if (sfxId != -1) {
                    func_10010FFC(0, sfxId, 0x5DC0, 0, 0, pos);
                }
                effect->field_0x38 = pos->field_0x84;
            } else {
                actor = pos->field_0x31C;
                if (actor != 0) {
                    if (actor->field_0x19C < 0x78) {
                        if (effect->field_0x38 == 0x513) {
                            effect->field_0x38 = 0x3A1;
                            effect->field_0x3C = func_1000FA64(0x3A1,
                                (s16)(s32)pos->field_0x14,
                                (s16)(s32)pos->field_0x18,
                                (s16)(s32)pos->field_0x1C,
                                0x7D00, 0x3E8, 0x1F4, (s32)func_15033BDC, effect, (s32)pos, 0, 0);
                            return 1;
                        }
                    }
                }
            }
            return 0;
        }

        if (effect->field_0x01 == 0x37) {
            if (arg0->field_0x24 != 0) {
                func_100111C8(arg0->field_0x24);
                arg0->field_0x24 = 0;
            }
            *arg6 = 0;
            return 0;
        }
    }
    return 1;
}

s32 func_15033E00(s32 arg0, u8 *arg1) {
    if (arg1[5] == 3) {
        return 1;
    }
    return 0;
}

s32 func_15033E28(u8 *arg0, u8 **arg1) {
    u8 *var;
    u8 *cur;
    u8 *next;
    s32 count;

    var = (u8 *)D_800C3EE0;
    count = 0;
    if (var == 0) {
        return 0;
    }
    cur = var;
    if (cur != 0) {
        do {
            next = *(u8 **)(cur + 0x54);
            if (arg0[0x3B] == cur[0]) {
                arg1[count] = cur;
                count += 1;
            }
            cur = next;
        } while (next != 0);
    }
    return count;
}

struct S15033E84 {
    u8 unk0;
    u8 pad[0x53];
    struct S15033E84 *unk54;
};
struct S15033E84 *func_15033E84(u8 *arg0) {
    struct S15033E84 *var;
    struct S15033E84 *next;
    u8 key;

    var = (struct S15033E84 *)D_800C3EE0;
    if (var != 0) {
        key = arg0[0x3B];
        do {
            next = var->unk54;
            if (var->unk0 == key) {
                return var;
            }
            var = next;
        } while (next != 0);
    }
    return 0;
}

void func_15033EC4(s32 arg0, s32 arg1) {
    struct S15033E84 *var;
    struct S15033E84 *next;
    s32 b;
    s32 key;

    b = arg0;
    var = (struct S15033E84 *)D_800C3EE0;
    if (var != 0) {
        do {
            next = var->unk54;
            key = var->unk0;
            if (b == key) {
                var->unk0 = arg1;
            } else if (arg1 == key) {
                var->unk0 = b;
            }
            var = next;
        } while (next != 0);
    }
}

s32 func_15033F0C(s32 arg0, u8 *arg1) {
    u8 *temp;

    if (D_800C35EA == 1) {
        return 0;
    }
    temp = *(u8 **)(arg1 + 0x31C);
    if (temp != 0) {
        if (temp[0x78] != 9) {
            if (temp[0x11A] != 3) {
                temp[0x11A] = 0;
                return 1;
            }
        }
    }
    return 0;
}

s32 func_15033F70(s32 arg0, u8 *arg1) {
    u8 *temp;

    if (D_800C35EA == 1) {
        return 0;
    }
    temp = *(u8 **)(arg1 + 0x31C);
    if (temp != 0) {
        if ((temp[0x78] != 0xC) && (temp[0x78] != 0x16)) {
            if (temp[0x11A] != 3) {
                temp[0x11A] = 0;
                return 1;
            }
        }
    }
    return 0;
}
