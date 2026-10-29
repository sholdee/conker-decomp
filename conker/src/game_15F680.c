#include <ultra64.h>
#define func_15169260 func_15169260_s32
#include "functions.h"
#undef func_15169260
/* variables.h declares the D_800DC460/D_800DC464 list head/tail as s32 and D_800DC468 as u16;
 * func_15132444 walks them as node pointers and reads D_800DC468 with lh, so the real types
 * are made file-local here (the shared header is left alone). */
#define D_800DC460 D_800DC460_s32_decl_in_variables_h
#define D_800DC464 D_800DC464_s32_decl_in_variables_h
#define D_800DC468 D_800DC468_u16_decl_in_variables_h
#include "variables.h"
#undef D_800DC460
#undef D_800DC464
#undef D_800DC468
struct Node15132444;
extern struct Node15132444 *D_800DC460;
extern struct Node15132444 *D_800DC464;
extern s16 D_800DC468[];

void func_15132444(struct102 *arg0);
s32 func_1513264C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);
s32 func_151464B8(s16 *arg0);
extern u8 D_800C3E90;
extern s32 (*D_8008998C[])(s32);
extern s32 D_800A3880[];
extern s32 D_800DC640[];
s32 func_1502B6BC(s32 *arg0, s32 arg1, s32 *arg2, s32 arg3, s32 arg4, s32 arg5);
s32 func_1510CE60(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4);
void func_15168E54(s32 arg0, s32 arg1);
s32 func_15046C80(f32 *arg0, s32 arg1, f32 arg2, void *arg3);
s32 func_15044380(f32 arg0, f32 arg1, f32 arg2, void *arg3, s32 arg4, s32 arg5);

typedef struct {
    u8 bytes[0x10];
    u16 tail;
} Bytes18_15132DDC;

typedef struct {
    u8 pad_0[0x10];
    f32 field_0x10;
    u8 pad_14[0xC];
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    u8 pad_2C[0xC];
    f32 field_0x38;
    f32 field_0x3C;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4C;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5C;
    s32 field_0x60;
    u8 pad_64[0xF];
    u8 field_0x73;
    u8 field_0x74;
    u8 field_0x75;
    u8 pad_76[0x9A];
    f32 field_0x110;
    Bytes18_15132DDC field_0x114;
    u8 pad_126[0x6];
    u8 field_0x12C;
    u8 field_0x12D;
    u8 pad_12E[0x6];
    f32 field_0x134;
    f32 field_0x138;
    f32 field_0x13C;
    f32 field_0x140;
    f32 field_0x144;
    u8 field_0x148;
    u8 field_0x149;
    s8 field_0x14A;
} Func15132DDCArg0;

typedef struct {
    s32 field_0x0;
    u8 pad_4[0x10];
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    u8 pad_20[0x8];
    f32 field_0x28;
    u8 pad_2C[0xB8];
    s16 field_0xE4;
    s16 field_0xE6;
    u8 pad_E8[0x10];
    s32 field_0xF8;
    u8 pad_FC[0x50];
    f32 field_0x14C;
    f32 field_0x150;
    u8 pad_154[0x2C];
    f32 field_0x180;
    u8 pad_184[0x4];
    s32 field_0x188;
    Bytes18_15132DDC field_0x18C;
    u8 pad_19E[0x192];
    f32 field_0x330[3];
    f32 field_0x33C[3];
} Func15132DDCLocals;

typedef s32 (*Func15132DDCCallback34)(Func15132DDCArg0 *, f32, f32, f32, f32, void *);
typedef s32 (*Func15132DDCCallback70)(Func15132DDCArg0 *, f32, f32, f32, f32);
typedef s32 (*Func15132DDCCallback74)(Func15132DDCArg0 *, void *, f32, f32, f32);

extern Func15132DDCCallback34 D_80089934[];
extern Func15132DDCCallback70 D_80089970[];
extern Func15132DDCCallback74 D_80089974[];
extern f32 D_800A386C;
extern f32 D_800A3870;

typedef struct {
    u8 pad_0[0x60];
    s32 field_0x60;
    u8 pad_1[0x4];
    u8 field_0x68;
} Func151323ACArg0;

typedef struct {
    u8 pad0[0x60];
    u32 flags;
    u8 pad64[0x8];
    s16 *unk6C;
    u8 pad70[0x8];
    u8 unk78;
} Obj15132A88;

typedef struct {
    u8 pad_0[0x18];
    s32 field_0x18;
    s32 field_0x1C;
    s32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2C;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3C;
    f32 field_0x40;
} MatrixTransformFields;

typedef struct {
    u8 pad_0[0x18];
    f32 field_0x18;
    f32 field_0x1C;
    s32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    u8 pad_1[0xC];
    f32 field_0x38;
    f32 field_0x3C;
    f32 field_0x40;
} TransformDispatchFields;

typedef struct {
    u8 pad[4];
    f32 m[4][4];
} PaddedMtxF15133A94;

typedef struct {
    u8 pad_0[0x10];
    f32 field_0x10;
    f32 field_0x14;
    u8 pad_1[0x24];
    f32 field_0x3C;
    u8 pad_2[0x4];
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4C;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5C;
    s32 field_0x60;
} MotionFields15133;

typedef struct {
    s32 unk0;
    s32 unk4;
} Data15133E3C;

typedef struct {
    u8 pad_0[0x7C];
    s32 field_0x7C;
    u8 field_0x80;
} InteractionTargetFields;

typedef struct {
    s32 field_0x0;
    u8 field_0x4;
} InteractionKeyFields;

extern Data15133E3C D_800A3860;
void func_15169260(Data15133E3C *, s32, s32, u8);

typedef struct {
    u8 pad_0[0x38];
    struct17 field_0x38;
    u8 pad_44[0x1C];
    u32 field_0x60;
    s16 field_0x64;
    u8 pad_66[0xA];
    u8 field_0x70;
    u8 field_0x71;
    u8 field_0x72;
    u8 pad_73[0x3];
    u8 field_0x76;
    u8 field_0x77;
    u8 pad_78[0xA];
    s16 field_0x82;
    s16 field_0x84;
    u8 pad_86[0xC2];
    u8 field_0x148;
} Func151321D0Arg0;

typedef s32 (*Func151321D0Callback)(Func151321D0Arg0 *);

extern Func151321D0Callback D_800898B0[];
extern Func151321D0Callback D_80089914[];
extern Func151321D0Callback D_8008997C[];
extern Func151321D0Callback D_80089988[];
s32 func_1514672C(struct17 *arg0);

void func_151321D0(Func151321D0Arg0 *arg0) {
    s32 temp;
    u8 stopped;

    stopped = 0;
    arg0->field_0x60 &= 0xFFDFFFFF;
    if ((arg0->field_0x60 & 0x10) && (arg0->field_0x148 & 0x18)) {
        temp = D_80089988[arg0->field_0x77](arg0);
        if (temp == 0) {
            stopped = 1;
        }
    } else if (arg0->field_0x71 != 0) {
        temp = D_800898B0[arg0->field_0x71](arg0);
        if (temp == 0) {
            stopped = 1;
        }
    }

    if ((arg0->field_0x72 != 0) && !stopped) {
        temp = D_80089914[arg0->field_0x72](arg0);
        if (temp == 0) {
            stopped = 1;
        }
    }

    if ((arg0->field_0x60 & 0x80) && !stopped) {
        arg0->field_0x64 -= D_800BE9E4;
        if (arg0->field_0x64 < 0) {
            stopped = 1;
        }
    }

    if ((arg0->field_0x60 & 0x2000) && !stopped) {
        if (arg0->field_0x64 < arg0->field_0x82) {
            temp = arg0->field_0x64 * arg0->field_0x84;
            if (temp < arg0->field_0x70) {
                arg0->field_0x70 = temp;
            }
        }
    }

    if (!stopped && !(arg0->field_0x60 & 0x80000)) {
        stopped = func_1514672C(&arg0->field_0x38) == 0;
    }

    if (stopped) {
        if (arg0->field_0x76 != 0) {
            D_8008997C[arg0->field_0x76](arg0);
        }
        func_1516972C((struct102 *)arg0);
    }
}

extern void (*D_800899B0[])(s32);

void func_151323AC(Func151323ACArg0 *arg0) {
    s32 idx;

    idx = (arg0->field_0x60 & 0x100) ? arg0->field_0x68 : 0;
    D_800899B0[idx]((s32)arg0);
}

extern void (*D_800899D4[])(s32);

void func_151323F8(Func151323ACArg0 *arg0) {
    s32 idx;

    idx = (arg0->field_0x60 & 0x100) ? arg0->field_0x68 : 0;
    D_800899D4[idx]((s32)arg0);
}

typedef struct Node15132444 {
    s32 *unk0;
    struct Node15132444 *next;
    struct Node15132444 *prev;
    u8 padC[2];
    u8 unkE;
} Node15132444;

typedef struct {
    u8 pad0[0x66];
    u16 unk66;
    u8 pad68[0x24];
    Node15132444 *unk8C;
} Obj15132444;

void func_1510D630(s32);
void func_151325C8(void *arg0);

void func_15132444(struct102 *arg0) {
    D_800DC63C--;
    D_800DC468[((Obj15132444 *)arg0)->unk66]--;
    if (D_800DC468[((Obj15132444 *)arg0)->unk66] == 0) {
        if (((Obj15132444 *)arg0)->unk8C->unkE != 0) {
            D_800DC468[((Obj15132444 *)arg0)->unk66] = 1;
        } else {
            func_1510D630(D_800DC640[((Obj15132444 *)arg0)->unk66]);
            if (((Obj15132444 *)arg0)->unk8C->prev == NULL) {
                D_800DC460 = ((Obj15132444 *)arg0)->unk8C->next;
                if (D_800DC460 != NULL) {
                    D_800DC460->prev = NULL;
                } else {
                    D_800DC464 = NULL;
                }
            } else {
                ((Obj15132444 *)arg0)->unk8C->prev->next = ((Obj15132444 *)arg0)->unk8C->next;
                if (((Obj15132444 *)arg0)->unk8C->next != NULL) {
                    ((Obj15132444 *)arg0)->unk8C->next->prev = ((Obj15132444 *)arg0)->unk8C->prev;
                } else {
                    D_800DC464 = ((Obj15132444 *)arg0)->unk8C->prev;
                }
            }
            func_100043B4(((Obj15132444 *)arg0)->unk8C->unk0, 4);
            func_100043B4((s32 *)((Obj15132444 *)arg0)->unk8C, 4);
        }
    }
    func_151325C8(arg0);
}

void func_15132570(struct102 *arg0) {
    func_15132444(arg0);
    func_15169804(arg0);
}

void func_1513259C(struct102 *arg0) {
    func_15132444(arg0);
    func_15169824(arg0);
}

void func_151325C8(void *arg0) {
    s32 i;
    s32 *p;

    for (i = 0, p = (s32 *)arg0; i <= D_80082FA0; i++, p++) {
        if (p[0x55] != 0) {
            func_100043B4((s32 *)p[0x55], 4);
        }
    }

    p = (s32 *)((s32 *)arg0)[0x59];
    if (((s32 *)arg0)[0x59] != 0) {
        func_100043B4(p, 4);
    }
}

typedef struct {
    u8 pad0[0x34];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u8 pad40[0x10];
    s32 unk50;
    u8 pad54[2];
    u16 unk56;
} Arg1513264C;

typedef struct Node1513264C {
    s32 *unk0;
    struct Node1513264C *next;
    struct Node1513264C *prev;
    u16 unkC;
    u8 unkE;
} Node1513264C;

typedef struct {
    s32 w[9];
} Copy24_1513264C;

typedef struct {
    u8 pad0[0x10];
    u8 unk10[0x50];
    s32 unk60;
    u8 pad64[0x28];
    Node1513264C *unk8C;
    u8 pad90[0x80];
    f32 unk110;
    u8 pad114[0x14];
    s32 unk128;
    u8 unk12C;
    u8 unk12D;
    u8 pad12E[2];
    s32 unk130;
    f32 unk134;
    f32 unk138;
    f32 unk13C;
    f32 unk140;
    f32 unk144;
    u8 unk148;
    u8 unk149;
    u8 pad14A[2];
    s32 unk14C;
    u8 unk150;
    u8 pad151[3];
    s32 unk154[4];
    s32 unk164;
    s32 unk168;
} Obj1513264C;

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void *allocate_memory(s32, s32, s32, s32);
extern void func_15168A9C(void *);
extern void func_10004074(void *);
extern s32 func_1515D480(s32);
extern s32 func_1515D440(void);
s32 func_151336A8(s32 arg0, s32 **arg1, s32 arg2);

#define SRC ((Arg1513264C *)arg0)
s32 func_1513264C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    Obj1513264C *obj;
    Node1513264C *newNode;
    s32 i;

    if (D_800DC63C > 300) {
        return 0;
    }
    obj = func_15167A68((SRC->unk50 & 0x4000) ? 0x48 : 0x19, arg6, arg4 + 0x170, 1, arg5, (SRC->unk50 & 0x400000) ? 2 : 1);
    if (obj == NULL) {
        return 0;
    }
    if (D_800DC468[SRC->unk56] == 0) {
        newNode = allocate_memory(0x10, 1, 2, (SRC->unk50 & 0x400000) ? 2 : 1);
        if (newNode == NULL) {
            func_15168A9C(obj);
            func_10004074(obj);
            return 0;
        }
        if (func_151336A8(SRC->unk56, (s32 **)newNode, (s32)obj)) {
            newNode->next = (Node1513264C *)D_800DC460;
            if (newNode->next != NULL) {
                newNode->next->prev = newNode;
            } else {
                D_800DC464 = (struct Node15132444 *)newNode;
            }
            D_800DC460 = (struct Node15132444 *)newNode;
            newNode->prev = NULL;
            newNode->unkC = SRC->unk56;
            if (SRC->unk50 & 0x100000) {
                if (D_800BE9F0 == 0x3B || D_800BE9F0 == 6 || D_800BE9F0 == 0x13 || D_800BE616 != 0 ||
                    D_800BE9F0 == 2) {
                    newNode->unkE = 0;
                } else {
                    newNode->unkE = 1;
                }
            } else {
                newNode->unkE = 0;
            }
        } else {
            func_15168A9C(obj);
            func_10004074(obj);
            func_10004074(newNode);
            return 0;
        }
    } else {
        s32 count;

        newNode = (Node1513264C *)D_800DC460;
        count = 100;
        while (newNode->unkC != SRC->unk56) {
            count--;
            newNode = newNode->next;
            if (count <= 0) {
                break;
            }
        }
        if (count <= 0) {
            func_15168A9C(obj);
            func_10004074(obj);
            return 0;
        }
    }
    D_800DC468[SRC->unk56]++;
    obj->unk8C = newNode;
    memcpy(obj->unk10, (void *)arg0, 0x7C);
    obj->unk149 = 0;
    obj->unk134 = 0.0f;
    obj->unk138 = 0.0f;
    obj->unk13C = 0.0f;
    obj->unk140 = sqrtf(SRC->unk34 * SRC->unk34 + SRC->unk38 * SRC->unk38 + SRC->unk3C * SRC->unk3C);
    obj->unk148 = 0;
    obj->unk144 = 1.0f;
    if (arg3 != 0) {
        *(Copy24_1513264C *)&obj->unk110 = *(Copy24_1513264C *)arg3;
    } else {
        obj->unk110 = -10000.0f;
        obj->unk130 = 0;
        obj->unk12D = 0;
        obj->unk12C = 0;
        obj->unk128 = 0;
    }
    D_800DC63C++;
    obj->unk150 = 0;
    obj->unk14C = arg1;
    obj->unk168 = arg2;
    for (i = 0; i < 4; i++) {
        obj->unk154[i] = 0;
    }
    obj->unk164 = 0;
    if (arg1 != 0) {
        for (i = 0; i <= D_80082FA0; i++) {
            obj->unk154[i] = func_1515D480(arg1);
        }
        obj->unk164 = func_1515D440();
    }
    obj->unk60 &= ~0x200000;
    return (s32)obj;
}
#undef SRC

s32 func_15132A4C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    return func_1513264C(arg0, arg1, arg2, 0, arg3, arg4, arg5);
}

void func_15132A88(Obj15132A88 *arg0) {
    s16 *temp;
    s32 ret;
    s32 flags;

    arg0->flags &= 0xFFDFFFFF;
    if (arg0->flags & 0x20000) {
        return;
    }

    if (arg0->flags & 0x400) {
        temp = arg0->unk6C;
        if (temp != NULL) {
            if (func_151464B8(temp) != 0) {
                return;
            }
        }
    }

    flags = arg0->flags;
    if (flags & 0x80000) {
        arg0->flags = flags | 0x200000;
        return;
    }

    ret = D_8008998C[arg0->unk78]((s32)arg0 + 0x90 + (D_800BE9C0 << 6));
    if (ret == -1) {
        func_1516972C((struct102 *)arg0);
        return;
    }

    if (ret == 0) {
        arg0->flags &= 0xFFFBFFFF;
    } else {
        arg0->flags |= 0x240000;
    }
}

typedef struct {
    u8 pad_0[0x38];
    u8 field_0x38[0x28];
    s32 field_0x60;
    u8 pad_64[0xC];
    u8 field_0x70;
    u8 pad_71[0x8];
    s8 field_0x79;
    u8 field_0x7A;
    u8 pad_7B;
    s32 field_0x7C;
    u8 field_0x80;
    u8 pad_81[0xB];
    Gfx ***field_0x8C;
    u8 field_0x90[0x98];
    s32 field_0x128;
    u8 pad_12C[0x20];
    s32 field_0x14C[1];
} Obj15132B80;

extern u8 D_80083740[];
extern u8 D_800838C0[];
extern Gfx *(*D_800899A4[])(Gfx *, Obj15132B80 *, s16);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
void *func_151462C8(void *arg0, s32 *arg1, u8 arg2, void *arg3, u8 arg4, s16 arg5, struct17 *arg6, u8 arg7, s32 arg8);

Gfx *func_15132B80(Gfx *gfx, Obj15132B80 *arg1, s16 arg2) {
    Obj15132B80 *obj;
    u8 sync;

    obj = arg1;
    if (!(obj->field_0x60 & 0x200000)) {
        return gfx;
    }
    if (obj->field_0x60 & 0x20000) {
        return gfx;
    }
    if (!(obj->field_0x60 & 0x40000)) {
        return gfx;
    }
    sync = 1;
    if (obj->field_0x70 < 0xFF) {
        gSPSegment(gfx++, 8, D_80083740);
    } else {
        gSPSegment(gfx++, 8, D_800838C0);
    }
    if (obj->field_0x60 & 0x10000) {
        if (obj->field_0x79 != -1) {
            gfx = D_800899A4[obj->field_0x79](gfx, obj, arg2);
        }
    }
    gfx = func_15142C10(gfx, 0, 0, 0, obj->field_0x70, &sync);
    gfx = func_15142B7C(gfx,
                        ((obj->field_0x60 & 0x800) ? 0x20000 : 0) | ((obj->field_0x60 & 0x1000) ? 0x400000 : 0) | 1 | 4 | 0x200000,
                        ((obj->field_0x60 & 0x800) ? 0 : 0x20000) | ((obj->field_0x60 & 0x1000) ? 0 : 0x400000) | 0x600 | 0x10000 | 0x40000 | 0x80000);
    if (obj->field_0x60 & 0x800) {
        gfx = func_151462C8(gfx, obj->field_0x14C, obj->field_0x7A, (void *)obj->field_0x7C, obj->field_0x80, arg2,
                            (struct17 *)obj->field_0x38, (obj->field_0x60 & 0x1000) ? 1 : 0, obj->field_0x128);
    }
    gSPMatrix(gfx++, &obj->field_0x90[D_800BE9C0 << 6], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gfx++, **obj->field_0x8C);
    return gfx;
}

s32 func_15132DDC(Func15132DDCArg0 *arg0) {
    Func15132DDCLocals sp38;
    register f32 temp_f0;

    if (arg0->field_0x60 & 7) {
        memcpy(sp38.field_0x33C, &arg0->field_0x38, 0xC);
    }

    if (arg0->field_0x60 & 8) {
        arg0->field_0x48 += arg0->field_0x5C * D_800BE9A4;
    }

    if (arg0->field_0x60 & 0x20) {
        arg0->field_0x38 += arg0->field_0x44 * D_800BE9A4;
        arg0->field_0x3C += arg0->field_0x48 * D_800BE9A4;
        arg0->field_0x40 += arg0->field_0x4C * D_800BE9A4;
    }

    if (arg0->field_0x60 & 0x40) {
        arg0->field_0x20 += arg0->field_0x50 * D_800BE9A4;
        arg0->field_0x24 += arg0->field_0x54 * D_800BE9A4;
        arg0->field_0x28 += arg0->field_0x58 * D_800BE9A4;
    }

    if (arg0->field_0x60 & 7) {
        sp38.field_0x330[0] = arg0->field_0x38;
        sp38.field_0x330[1] = sp38.field_0x33C[1];
        sp38.field_0x330[2] = arg0->field_0x40;

        if (func_15046C80(sp38.field_0x330, 0, arg0->field_0x3C - arg0->field_0x10, &arg0->field_0x110) != 0) {
            if ((arg0->field_0x12D == 1) || ((arg0->field_0x12D == 2) && (arg0->field_0x12C & 1))) {
                arg0->field_0x148 |= 1;
                if (arg0->field_0x60 & 0x10) {
                    temp_f0 = sqrtf((arg0->field_0x44 * arg0->field_0x44) +
                                    (arg0->field_0x48 * arg0->field_0x48) +
                                    (arg0->field_0x4C * arg0->field_0x4C));
                    if (temp_f0 > 1.0f) {
                        arg0->field_0x148 |= 8;
                        arg0->field_0x140 = temp_f0;
                        arg0->field_0x144 = 1.0f / temp_f0;
                    }
                }

                if (arg0->field_0x73 != 0) {
                    if (D_80089934[arg0->field_0x73](arg0, sp38.field_0x33C[0], sp38.field_0x33C[1],
                                                     sp38.field_0x33C[2], arg0->field_0x110,
                                                     &arg0->field_0x114) == 0) {
                        return 0;
                    }
                }

                if (arg0->field_0x60 & 0x10) {
                    memcpy(&arg0->field_0x134, &arg0->field_0x38, 0xC);
                }
            }

            if (arg0->field_0x12D == 3) {
                arg0->field_0x148 |= 2;
                if (arg0->field_0x74 != 0) {
                    if (D_80089970[arg0->field_0x74](arg0, sp38.field_0x33C[0], sp38.field_0x33C[1],
                                                     sp38.field_0x33C[2], arg0->field_0x110) == 0) {
                        return 0;
                    }
                }
            }
        }

        if (arg0->field_0x60 & 4) {
            sp38.field_0x0 = 4;
            sp38.field_0x14 = arg0->field_0x38;
            sp38.field_0x18 = arg0->field_0x3C;
            sp38.field_0x1C = arg0->field_0x40;
            sp38.field_0x188 = 0;
            sp38.field_0x18C = arg0->field_0x114;
            sp38.field_0x28 = 1.0f;
            sp38.field_0x14C = 1.0f;
            sp38.field_0xE4 = (s16)(s32)arg0->field_0x10;
            sp38.field_0x150 = 1.0f;
            sp38.field_0xE6 = (s16)(s32)arg0->field_0x10;
            if (arg0->field_0x12C & 2) {
                sp38.field_0x180 = arg0->field_0x110;
            } else {
                sp38.field_0x180 = D_800A386C;
            }
            sp38.field_0xF8 = 0;

            if (func_15044380(sp38.field_0x33C[0], sp38.field_0x33C[1], sp38.field_0x33C[2], &sp38, 0, 0) != 0) {
                arg0->field_0x148 |= 4;
                if (arg0->field_0x60 & 0x10) {
                    temp_f0 = sqrtf((arg0->field_0x44 * arg0->field_0x44) +
                                    (arg0->field_0x48 * arg0->field_0x48) +
                                    (arg0->field_0x4C * arg0->field_0x4C));
                    if (temp_f0 > 1.0f) {
                        arg0->field_0x148 |= 0x10;
                        arg0->field_0x140 = temp_f0;
                        arg0->field_0x144 = 1.0f / temp_f0;
                        arg0->field_0x14A = (s8)(s32)(func_150484A0(sp38.field_0x14 - arg0->field_0x38,
                                                                     sp38.field_0x1C - arg0->field_0x40) *
                                                       D_800A3870);
                    }
                }

                if (arg0->field_0x75 != 0) {
                    if (D_80089974[arg0->field_0x75](arg0, &sp38, sp38.field_0x33C[0],
                                                     sp38.field_0x33C[1], sp38.field_0x33C[2]) == 0) {
                        return 0;
                    }
                }

                if (arg0->field_0x60 & 0x10) {
                    memcpy(&arg0->field_0x134, &arg0->field_0x38, 0xC);
                }
            }
        }
    }

    return 1;
}

typedef struct {
    u8 pad0[0x10];
    f32 field10;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    f32 field2C;
    f32 field30;
    f32 field34;
    f32 field38;
    f32 field3C;
    f32 field40;
    u8 pad44[0xC];
    f32 field50;
    f32 field54;
    f32 field58;
    u8 pad5C[4];
    s32 flags60;
    u8 pad64[0x134 - 0x64];
    f32 origin[3];
    u8 pad140[4];
    f32 field144;
    u8 flags148;
    u8 phase149;
    s8 angle14A;
} Func151332DCArg0;

s32 func_151332DC(Func151332DCArg0 *arg0) {
    s32 unused;
    f32 amplitude;
    f32 cosine;
    f32 sine;

    arg0->field2C = 1.0f;
    arg0->field30 = 1.0f;
    arg0->field34 = 1.0f;
    if (arg0->flags60 & 0x40) {
        arg0->field20 += arg0->field50 * D_800BE9A4;
        arg0->field24 += arg0->field54 * D_800BE9A4;
        arg0->field28 += arg0->field58 * D_800BE9A4;
    }
    arg0->phase149 += (s32)(576.0f * arg0->field144) * D_800BE9E4;
    if (arg0->phase149 < 0x80) {
        amplitude = func_151423D8((u8)(arg0->phase149 - 0x40));
        amplitude = (1.0f - arg0->field14) * (1.0f - arg0->field144) * amplitude;
        if (arg0->flags148 & 8) {
            arg0->field2C *= 1.0f + amplitude;
            arg0->field30 *= 1.0f - amplitude;
            arg0->field34 *= 1.0f + amplitude;
            arg0->field3C = arg0->origin[1] - amplitude * arg0->field10;
        }
        if (arg0->flags148 & 0x10) {
            cosine = func_151423D8((u8)(arg0->angle14A - 0x40));
            sine = func_151423D8((u8)arg0->angle14A);
            arg0->field2C *= 1.0f - amplitude;
            arg0->field30 *= 1.0f + amplitude;
            arg0->field34 *= 1.0f - amplitude;
            arg0->field38 = arg0->origin[0] - (amplitude * arg0->field10) * sine;
            arg0->field40 = arg0->origin[2] + (amplitude * arg0->field10) * cosine;
return_active:
            return 1;
        }
    } else {
        memcpy(&arg0->field38, arg0->origin, 0xC);
        arg0->phase149 = 0;
        arg0->flags148 &= 0xFFE7;
    }
    return 1;
}

s32 func_151424F4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, f32 arg11);

s32 func_15133510(s32 arg0, MatrixTransformFields *arg1) {
    func_151424F4(arg0,
                  arg1->field_0x18,
                  arg1->field_0x1C,
                  arg1->field_0x20,
                  arg1->field_0x24,
                  arg1->field_0x28,
                  arg1->field_0x2C,
                  arg1->field_0x30,
                  arg1->field_0x34,
                  arg1->field_0x38,
                  arg1->field_0x3C,
                  arg1->field_0x40);
    return 1;
}

extern void (*D_800899F8[])(void *, void *, u8);
void func_151B9660(void *arg0);
typedef struct 
{
  u8 pad_0[0x60];
  s32 field_0x60;
  u8 pad_64[2];
  u16 field_0x66;
  u8 field_0x68;
  u8 pad_69[0x13];
  s32 field_0x7C;
  u8 field_0x80;
} Func15133588Arg0;
typedef struct 
{
  s32 field_0x0;
  s32 field_0x4;
  u8 field_0x8;
  u8 field_0x9;
} Func15133588Arg1;
void func_15133588(Func15133588Arg0 *arg0, Func15133588Arg1 *arg1, u8 arg2)
{
  s32 temp_v0;
  s32 temp_v0_2;
  s32 temp_v1;
  u8 var_v0;
  void (*callback)(void *, void *, u8);
  temp_v0 = arg2;
  if ((temp_v0 == 0x1B) && (arg0->field_0x66 == 0x22))
  {
    func_151B9660(arg0);
  }
  if (temp_v0 == 0x2D)
  {
    temp_v0_2 = arg1->field_0x0;
    ;
    if (temp_v0_2 == arg0->field_0x7C)
    {
      arg0->field_0x7C = arg1->field_0x4;
      arg0->field_0x80 = arg1->field_0x9;
    }
    else
      if (arg0->field_0x7C != arg1->field_0x4)
    {
    }
    else
    {
      arg0->field_0x7C = temp_v0_2;
      arg0->field_0x80 = arg1->field_0x8;
      second_update_done:
      ;

      ;
      ;
    }
  }
  else
    if (temp_v0 == 0)
  {
    temp_v0_2 = arg1->field_0x0;
    if ((temp_v0_2 == arg0->field_0x7C) || (arg0->field_0x80 == (*((u8 *) (&arg1->field_0x4)))))
    {
      arg0->field_0x7C = 0;
      arg0->field_0x80 = 0;
    }
  }
  var_v0 = (arg0->field_0x60 & 0x100) ? (arg0->field_0x68) : (0);
  callback = D_800899F8[var_v0];
  if (0 != callback)
  {
    callback(arg0, arg1, arg2);
  }
}


s32 func_151336A8(s32 arg0, s32 **arg1, s32 arg2) {
    s32 sp2C;
    s32 sp28;

    *arg1 = (s32 *)func_1502B6BC(&sp2C, 0, &sp28, 2, 9, D_800A3880[arg0]);
    if (*arg1 == 0) {
        return 0;
    }
    func_1510CE60(**arg1, 0, 1, 0x3E, &D_800DC640[arg0]);
    func_15168E54(**arg1, (s32)*arg1);
    return 1;
}

s32 func_15142838(s32 arg0, f32 arg1, f32 arg2, s32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8);

s32 func_15133760(s32 arg0, TransformDispatchFields *arg1) {
    func_15142838(arg0,
                  arg1->field_0x18,
                  arg1->field_0x1C,
                  arg1->field_0x20,
                  arg1->field_0x24,
                  arg1->field_0x28,
                  arg1->field_0x38,
                  arg1->field_0x3C,
                  arg1->field_0x40);
    return 1;
}

s32 func_151337C0(f32 *arg0) {
    arg0[14] += arg0[17] * D_800BE9A4;
    arg0[15] += (arg0[18] * D_800BE9A4) + (((arg0[23] * D_800BE9A4) * D_800BE9A4) * 0.5f);
    arg0[16] += arg0[19] * D_800BE9A4;
    arg0[18] += arg0[23] * D_800BE9A4;
    arg0[8] += arg0[20] * D_800BE9A4;
    arg0[9] += arg0[21] * D_800BE9A4;
    arg0[10] += arg0[22] * D_800BE9A4;
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_15F680/func_15133894.s")

s32 func_151339D4(MotionFields15133 *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4) {
    f32 temp;

    arg0->field_0x3C = arg0->field_0x10 + arg4;
    temp = arg0->field_0x14;
    arg0->field_0x44 = arg0->field_0x44 * temp;
    arg0->field_0x48 = arg0->field_0x48 * -temp;
    arg0->field_0x4C = arg0->field_0x4C * temp;
    arg0->field_0x50 = arg0->field_0x50 * temp;
    arg0->field_0x54 = arg0->field_0x54 * temp;
    arg0->field_0x58 = arg0->field_0x58 * temp;
    return 1;
}

s32 func_15133A50(MotionFields15133 *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4) {
    arg0->field_0x3C = arg0->field_0x10 + arg4;
    arg0->field_0x44 = 0.0f;
    arg0->field_0x48 = 0.0f;
    arg0->field_0x4C = 0.0f;
    arg0->field_0x50 = 0.0f;
    arg0->field_0x54 = 0.0f;
    arg0->field_0x58 = 0.0f;
    return 1;
}

s32 func_15133A94(Mtx *arg0, u8 *arg1) {
    struct127 *obj;
    struct255 *mtx;
    PaddedMtxF15133A94 sp18;
    u8 *src;

    obj = *(struct127 **)(arg1 + 0x7C);
    if ((obj == NULL) || (obj->interaction_state == 0) || (obj->id == 0xFF) ||
        (arg1[0x80] != *(u8 *)((u8 *)obj + 0x3B))) {
        return -1;
    }
    mtx = obj->unk1D4;
    if (mtx == NULL) {
        return 0;
    }
    if ((obj->unk74 & 0xF) == 0xF) {
        return 0;
    }
    if (D_800C3E90) {
        src = (u8 *)mtx;
        src += arg1[0x170] << 6;
        memcpy(arg0, src, 0x40);
        return 1;
    }
    src = (u8 *)mtx;
    src += arg1[0x170] << 6;
    memcpy(sp18.m, src, 0x40);
    sp18.m[0][3] = 0.0f;
    sp18.m[1][3] = 0.0f;
    sp18.m[2][3] = 0.0f;
    sp18.m[3][3] = 1.0f;
    guMtxF2L(sp18.m, arg0);
    return 1;
}

s32 func_15133B98(MotionFields15133 *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    f32 temp;

    temp = arg0->field_0x14;
    arg0->field_0x3C = arg0->field_0x10 + arg4;
    arg0->field_0x44 = arg0->field_0x44 * temp;
    arg0->field_0x48 = arg0->field_0x48 * -temp;
    arg0->field_0x4C = arg0->field_0x4C * temp;
    arg0->field_0x50 = arg0->field_0x50 * temp;
    arg0->field_0x54 = arg0->field_0x54 * temp;
    arg0->field_0x58 = arg0->field_0x58 * temp;
    if (fabsf(arg0->field_0x48) < 4.0f) {
        arg0->field_0x60 &= -0x6A;
        arg0->field_0x44 = 0.0f;
        arg0->field_0x48 = 0.0f;
        arg0->field_0x4C = 0.0f;
        arg0->field_0x50 = 0.0f;
        arg0->field_0x54 = 0.0f;
        arg0->field_0x58 = 0.0f;
        arg0->field_0x5C = 0.0f;
    }
    return 1;
}

s32 func_15133C58(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    func_15133B98(arg0, arg1, arg2, arg3, arg4, arg5);
    if (func_150ADA20() & 1) {
        func_10010F88(((u32)func_150ADA20() % 9) + 0x2DE, 0x2EE0, 0, 0, 0,
                      (s32)*(f32 *)&arg1, (s32)*(f32 *)&arg2, (s32)*(f32 *)&arg3,
                      0x1F4, 0x3E8);
    }
    return 1;
}

s32 func_15133D20(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    func_15133B98(arg0, arg1, arg2, arg3, arg4, arg5);
    if (func_150ADA20() & 1) {
        func_10010F88(((u32)func_150ADA20() % 10) + 0x1B8, 0x5DC0, 0, 0, 0,
                      (s32)*(f32 *)&arg1, (s32)*(f32 *)&arg2, (s32)*(f32 *)&arg3,
                      0x1F4, 0x3E8);
    }
    return 1;
}

void func_15133DE8(InteractionTargetFields *arg0, InteractionKeyFields *arg1, u8 arg2) {
    s32 temp;

    if (arg2 == 0) {
        temp = arg1->field_0x0;
        if ((temp == arg0->field_0x7C) ||
            (arg0->field_0x80 == arg1->field_0x4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}

void func_15133E3C(s32 arg0, u8 arg1) {
    Data15133E3C sp18;

    sp18 = D_800A3860;
    func_15169260(&sp18, 2, arg0, arg1);
}

Gfx *func_15133EEC(Gfx *gfx, u16 arg1, u8 arg2, s32 arg3);

void func_15133E84(s32 arg0, void *arg1, s32 arg2) {
    func_15133EEC(arg0, *(u16 *)((u8 *)arg1 + 0x170), *(u8 *)((u8 *)arg1 + 0x172), *(s32 *)((u8 *)arg1 + 0x174));
}

void func_15133EB8(s32 arg0, void *arg1, s32 arg2) {
    func_15133EEC(arg0, *(u16 *)((u8 *)arg1 + 0x174), *(u8 *)((u8 *)arg1 + 0x176), *(s32 *)((u8 *)arg1 + 0x178));
}

typedef struct {
    s32 *unk0;
    u8 pad4[6];
    u8 unkA;
    u8 unkB;
} Entry15133EEC;

extern Entry15133EEC D_80090B60[];
extern s32 func_1510D0EC(s32, s32 *, s32, s32);

Gfx *func_15133EEC(Gfx *gfx, u16 arg1, u8 arg2, s32 arg3) {
    s32 data;
    u8 kind;
    u8 fmt;
    Entry15133EEC *e;
    s32 size;

    e = &D_80090B60[arg1];
    data = *e->unk0;
    kind = e->unkA;
    fmt = e->unkB;
    data = func_1510D0EC(data, &size, arg3, 0);
    gSPSegment(gfx++, arg2, data);
    if (kind == 2) {
        gSPSegment(gfx++, arg2 + 1, data + size - ((fmt == 1) ? 0x200 : 0x20));
    }
    return gfx;
}

typedef struct {
    u8 unk0[4];
    u16 unk4;
    u8 unk6;
    u8 unk7;
    s32 unk8;
} Elem15133FD8B;

typedef u8 Entry15133FD8[8];

s32 func_15133FD8(s32 arg0, void *arg1, s32 arg2) {
    u8 i;
    Entry15133FD8 *base;
    Elem15133FD8B *e;

    base = (Entry15133FD8 *)((u8 *)arg1 + 0x170);
    for (i = 0; i < (*base)[0]; i++) {
        e = (Elem15133FD8B *)base[i];
        arg0 = func_15133EEC(arg0, e->unk4, e->unk6, e->unk8);
    }
    return arg0;
}
