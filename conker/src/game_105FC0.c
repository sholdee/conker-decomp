#include <ultra64.h>
#define func_10010F88 func_10010F88_void
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_10010F88
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

typedef struct {
    struct17 unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
} Header150D8B88;

typedef struct {
    void *unk0;
    u8 unk4;
    u8 pad5[3];
    struct17 unk8;
    struct17 unk14;
    struct17 unk20;
    f32 unk2C;
    s16 unk30;
    u8 pad32[2];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 unk4C;
    u8 unk4D;
    u8 pad4E[2];
    u8 pad50[4];
} Payload150D8B88;

typedef struct {
    char pad_0x00[0xC];
    s32 field_0x0C;
} ActorA8State;

extern f32 D_800A0B48;
extern f32 D_800A0B4C;
extern f32 D_800A0B50;
extern s32 func_151EF610(void);
extern void func_1000D96C(s32, s32, s32);
extern s32 func_10010F88(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_15131828(void *, void *, void *, void *);
extern void func_15131958(void *, s32, void *);


void func_150D8B10(f32 *a0, f32 *a1) {
    a1[0] = a0[5];
    a1[1] = a0[6] + 20.0f;
    a1[2] = a0[7];
}

extern u8 D_800BE616;
extern f32 D_800A0B40;
extern f32 D_800A0B44;
void func_15145740(void *, void *, void *, void *, f32);

void func_150D8B3C(void *arg0, void *arg1) {
    f32 sp34[3];
    f32 sp28[3];

    func_15145740(arg0, sp34, sp28, arg1, D_800BE616 ? D_800A0B40 : D_800A0B44);
}

void func_150D8B88(f32 *arg0) {
    Header150D8B88 header;
    Payload150D8B88 payload;
    struct17 vec;
    s32 *temp_v0;
    s32 temp_a0;

    if (*(s32 *)((u8 *)arg0 + 0x31C) != 0) {
        if (D_800BE9F0 == 0xA) {
            func_1000D96C(0xA, 0x37, 0);
        }

        func_150D8B10(arg0, (f32 *)&header.unk0);
        payload.unk4D = 0;
        payload.unk4C = 1;
        payload.unk0 = arg0;
        payload.unk4 = ((u8 *)arg0)[0x3B];
        vec = header.unk0;
        payload.unk8 = vec;
        payload.unk14 = vec;
        func_150D8B3C(arg0, &payload.unk20);

        payload.unk3C = D_800A0B48;
        temp_a0 = func_151EF610() % 2;
        payload.unk30 = func_10010F88(temp_a0 + 0x1B6, 0x7FFF, 0, 0, 0, 0, 0, 0, 0x1F4, 0x1388);
        payload.unk34 = 0.0f;
        payload.unk2C = 0.0f;
        payload.unk40 = 0.0f;
        payload.unk44 = 0.0f;
        payload.unk48 = 0.0f;
        payload.unk38 = -16384.0f;

        if (*(s32 *)((u8 *)arg0 + 0x31C) != 0) {
            if (*(u8 *)(*(s32 *)((u8 *)arg0 + 0x31C) + 0x84) != 0) {
                payload.unk4C &= ~1;
            }
        }

        header.unk15 = 0x32;
        header.unkC = 0x12C;
        header.unkE = 0x36;
        header.unk10 = 2;
        header.unk14 = 5;
        header.unk18 = 0;

        temp_v0 = (s32 *)func_15147A80(&header, 0x50, 0x24, 0xE, 1, 0x11, 3, 0xFF, 0, 0xFF, 1);
        if (temp_v0 != 0) {
            memcpy((void *)temp_v0[0x98 / 4], &payload, 0x50);
        }
    }
}

void func_150D8D84(struct17 *arg0, struct17 *arg1, f32 arg2) {
    struct17 sp4;

    sp4 = *arg1;
    arg1->unk4 += D_800A0B4C * arg2;
    arg0->unk0 += sp4.unk0 * arg2;
    arg0->unk4 += (sp4.unk4 * arg2) + ((D_800A0B50 * arg2) * arg2);
    arg0->unk8 += sp4.unk8 * arg2;
}

typedef struct {
    char pad0[0x1C];
    u16 unk1C;
    u16 unk1E;
    char pad20[0x10];
    u8 unk30;
} ConkerStruct150D8E1C;

void func_150D8E1C(ConkerStruct150D8E1C *arg0) {
    arg0->unk30 = 0;
    arg0->unk1E &= ~0x2;
    arg0->unk1E |= 0x8;
    arg0->unk1E |= 1;
    arg0->unk1C = 0x28;
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Func150D8E4CVec;

typedef struct {
    Func150D8E4CVec unk0;
    struct17 unkC;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} Func150D8E4CEntry;

typedef struct {
    struct127 *unk0;
    u8 pad4[0x49];
    s8 unk4D;
} Func150D8E4CPayload;

typedef struct {
    u8 pad0[0x1E];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x25];
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    u8 pad60[0x34];
    Func150D8E4CEntry *unk94;
    Func150D8E4CPayload *unk98;
} Func150D8E4CObj;

s32 func_150D8E4C(Func150D8E4CObj *arg0)
{
    Func150D8E4CPayload *payload;
    Func150D8E4CEntry *base;
    Func150D8E4CEntry *entry;
    struct127 *actor;
    struct126 *stats;
    s32 i;
    s32 stride;
    s32 *delta;

    payload = arg0->unk98;
    base = arg0->unk94;
    if ((arg0->unk2C < 2) && (arg0->unk1E & 8)) {
        return 0;
    }

    i = arg0->unk2E;
    if (i != arg0->unk2D) {
        do {
            i -= 1;
            if (i < 0) {
                i = arg0->unk25 - 1;
            }
            entry = &base[i];
            func_150D8D84((struct17 *)entry, &entry->unkC, D_800BE9A4);
        } while (i != arg0->unk2D);
    }

    delta = &D_800BE9E4;
    stride = sizeof(Func150D8E4CEntry);
stride_set:
    if (arg0->unk2C > 0) {
        *((Func150D8E4CVec *)&arg0->unk54) =
            ((Func150D8E4CEntry *)((u8 *)base + (arg0->unk2D * stride)))->unk0;
    } else {
        arg0->unk54 = 0.0f;
        arg0->unk58 = 0.0f;
        arg0->unk5C = 0.0f;
    }

    payload->unk4D += *delta;
    if (payload->unk4D >= 0x3D) {
        actor = payload->unk0;
        if (actor != NULL) {
            stats = actor->unk31C;
            if (stats != NULL) {
                stats->unk1AA++;
            }
        }
        payload->unk4D = 0;
    }

    return 1;
}

typedef struct {
    u8 pad0[0x78];
    u8 unk78;
    u8 pad79[0x11];
    u16 unk8A;
} Stats150D8FAC;

typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x18E];
    u8 unk1CA;
    u8 pad1CB[0x151];
    Stats150D8FAC *unk31C;
} Actor150D8FAC;

typedef struct {
    Actor150D8FAC *unk0;
    u8 unk4;
    u8 pad5[3];
    struct17 unk8;
    struct17 unk14;
    struct17 unk20;
    f32 unk2C;
    s16 unk30;
    u8 pad32[2];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 unk4C;
    u8 unk4D;
} Payload150D8FAC;

typedef struct {
    struct17 unk0;
    struct17 unkC;
    f32 unk18;
    u8 unk1C;
    u8 pad1D[3];
    f32 unk20;
} Entry150D8FAC;

typedef struct {
    u8 pad0[0x10];
    struct17 unk10;
    u8 pad1C[9];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x65];
    Entry150D8FAC *unk94;
    Payload150D8FAC *unk98;
} Obj150D8FAC;

s32 func_150D8FAC(Obj150D8FAC *arg0) {
    Payload150D8FAC *payload;
    Entry150D8FAC *base;
    Actor150D8FAC *actor;
    struct17 pos;
    struct17 vel;
    f32 time;
    f32 inv;
    u16 flags;
    f32 step;
    struct17 dpos;
    struct17 dvel;

    payload = arg0->unk98;
    base = arg0->unk94;
    actor = payload->unk0;
    if ((actor->unk31C->unk78 != 3) || (actor->unk1CA <= 0)) {
        func_150D8E1C((ConkerStruct150D8E1C *)arg0);
    }
    if ((actor->unk0 == 0) || (actor->unk3B != payload->unk4)) {
        return 0;
    }
    func_150D8B10((f32 *)actor, (f32 *)&pos);
    func_150D8B3C(actor, &vel);
    arg0->unk10 = pos;
    if (payload->unk4C & 1) {
        flags = actor->unk31C->unk8A;
        if (flags & 0x2000) {
            payload->unk3C += 0.001379000023f * D_800BE9A4;
            if (payload->unk3C > 0.04293100163f) {
                payload->unk3C = 0.04293100163f;
            }
        } else {
            payload->unk3C -= 0.001379000023f * D_800BE9A4;
            if (payload->unk3C < 0.01724099927f) {
                payload->unk3C = 0.01724099927f;
            }
        }
    }
    payload->unk34 += 0.380000025f * D_800BE9A4;
    if (payload->unk34 > 1.0f) {
        struct17 cur;
        struct17 curVel;
        f32 dpx;
        f32 dpy;
        f32 dpz;
        struct17 dvs;
        inv = 1.0f / payload->unk34;
        time = payload->unk2C + D_800BE9A4;
        cur = payload->unk14;
        curVel = payload->unk20;
        step = time * inv;
        dpos.unk0 = pos.unk0 - payload->unk14.unk0;
        dpos.unk4 = pos.unk4 - payload->unk14.unk4;
        dpos.unk8 = pos.unk8 - payload->unk14.unk8;
        dvel.unk0 = vel.unk0 - payload->unk20.unk0;
        dvel.unk4 = vel.unk4 - payload->unk20.unk4;
        dvel.unk8 = vel.unk8 - payload->unk20.unk8;
        dpx = dpos.unk0 * inv;
        dpy = dpos.unk4 * inv;
        dpz = dpos.unk8 * inv;
        dvs.unk0 = dvel.unk0 * inv;
        dvs.unk4 = dvel.unk4 * inv;
        dvs.unk8 = dvel.unk8 * inv;
        do {
            Entry150D8FAC *entry;

            entry = &base[arg0->unk2E];
            entry->unk0 = cur;
            entry->unkC.unk0 = payload->unk3C * curVel.unk0;
            entry->unkC.unk4 = payload->unk3C * curVel.unk4;
            entry->unkC.unk8 = payload->unk3C * curVel.unk8;
            entry->unk18 = 0.0f;
            entry->unk1C = 0xFF;
            entry->unk20 = 0.0f;
            func_150D8D84(&entry->unk0, &entry->unkC, time);
            arg0->unk2E++;
            time -= step;
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
            cur.unk0 += dpx;
            cur.unk4 += dpy;
            cur.unk8 += dpz;
            curVel.unk0 += dvs.unk0;
            curVel.unk4 += dvs.unk4;
            curVel.unk8 += dvs.unk8;
            payload->unk34 -= 1.0f;
        } while (payload->unk34 > 1.0f);
        payload->unk14 = cur;
        payload->unk20 = curVel;
        payload->unk2C = time;
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_105FC0/func_150D942C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_105FC0/func_150D9C7C.s")

void func_150DA484(void *arg0) {
    u16 *temp = (u16 *)*(u32 *)((u8 *)arg0 + 0x98);
    if (temp[0x18] != 0) {
        func_100111C8(temp[0x18]);
    }
}

void func_150DA484(void *);
void func_151478F4(void *);

void func_150DA4B4(void *arg0) {
    func_150DA484(arg0);
    func_151478F4(arg0);
}

void func_15147928(void *);

void func_150DA4E0(void *arg0) {
    func_150DA484(arg0);
    func_15147928(arg0);
}

typedef struct {
    s32 unk0;
    u8 unk4;
    char pad5[0x3C - 0x5];
    f32 unk3C;
} SubA_150DA50C;

typedef struct {
    s32 unk0;
    union {
        u8 b4;
        s32 w4;
    } u4;
    union {
        f32 f8;
        struct {
            u8 b8;
            u8 b9;
        } s;
    } u8;
} ArgB_150DA50C;

void func_150DA50C(void *arg0, s32 arg1, u8 arg2) {
    SubA_150DA50C *temp_v1 = *(SubA_150DA50C **)((u8 *)arg0 + 0x98);
    ArgB_150DA50C *b = (ArgB_150DA50C *)arg1;
    s32 temp;

    if (arg2 == 0x44) {
        if ((b->unk0 == temp_v1->unk0) || (b->u4.b4 == temp_v1->unk4)) {
            temp_v1->unk3C = b->u8.f8;
        }
trailing_label_150DA50C:
        ;
    } else if (arg2 == 0) {
        temp = b->unk0;
        if ((temp == temp_v1->unk0) || (temp_v1->unk4 == b->u4.b4)) {
            func_1516972C(arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v1->unk0 == b->unk0) {
            temp_v1->unk0 = b->u4.w4;
            temp_v1->unk4 = b->u8.s.b9;
        } else if (temp_v1->unk0 == b->u4.w4) {
            temp_v1->unk0 = b->unk0;
            temp_v1->unk4 = b->u8.s.b8;
        }
    }
}

struct s_150DA5EC {
    s32 unk0;
    u8 unk4;
    f32 unk8;
};

extern void func_15147D64(struct s_150DA5EC *, s32);

void func_150DA5EC(u8 *arg0, f32 arg1) {
    struct s_150DA5EC sp1C;

    sp1C.unk0 = (s32) arg0;
    sp1C.unk4 = arg0[0x3B];
    sp1C.unk8 = arg1;
    func_15147D64(&sp1C, 0x44);
}

s32 func_150DA628(u8 *arg0, s32 arg1) {
    ActorA8State *temp_a2;

    temp_a2 = (ActorA8State *)(arg0 + 0xA8);
    func_15131828(arg0, arg0 + 0xAC, temp_a2, arg0 + 0xAA);
    func_15131958(arg0 + 0x58, temp_a2->field_0x0C, temp_a2);
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
} Particle150DA67C; /* 0x70 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} Sub150DA67C;

typedef struct {
    u8 unk0[0x10];
    u16 unk10;
} Bytes150DA67C;

typedef struct {
    u8 pad0[4];
    u8 unk4;
} Kind150DA67C;

typedef struct {
    Kind150DA67C *unk0;
    u8 pad4[4];
    struct17 unk8;
    u8 pad14[0x30];
    Bytes150DA67C unk44;
    u8 pad56[3];
    u8 unk59;
} Obj150DA67C;

typedef struct {
    u8 pad0[0x40];
    f32 unk40;
    f32 unk44;
    f32 unk48;
} Data150DA67C;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x8B];
    Data150DA67C *unk98;
} Self150DA67C;

typedef struct {
    struct17 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    u8 unk34;
    u8 pad35[3];
    f32 unk38;
    s16 unk3C;
    s16 unk3E;
    s32 unk40;
} Emit150DA67C; /* 0x44 */

typedef struct {
    u8 unk0[0x24];
} Dst150DA67C;

typedef struct {
    u8 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    struct17 unk10;
    struct17 unk1C;
    f32 unk28;
} ShapeA150DA67C; /* 0x2C */

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    Bytes150DA67C unkC;
    f32 unk20;
} ShapeB150DA67C; /* 0x24 */

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern void func_150472C0(Dst150DA67C *, Obj150DA67C *);
extern void func_15153CCC(ShapeA150DA67C *, Emit150DA67C *, Dst150DA67C *, u8, s32);
extern void func_15153C84(ShapeB150DA67C *, Emit150DA67C *, Dst150DA67C *, u8, s32);

void func_150DA67C(Self150DA67C *arg0, Obj150DA67C *arg1, f32 *arg2) {
    Data150DA67C *data;
    Kind150DA67C *kind;

    data = arg0->unk98;
    kind = arg1->unk0;
    if (arg1->unk59 != 0) {
        if (arg1->unk59 >= 2 && (kind->unk4 == 0x33 || kind->unk4 == 0x3A)) {
            data->unk48 += (0.1990000159f + func_150ADA68() * 0.1540000141f) * D_800BE9A4;
            if (data->unk48 > 1.0f) {
                Particle150DA67C spawn;
                Sub150DA67C sub;
                void *ret;

                sub.unkC = 0.9936270118f;
                spawn.unk1D = 0x6C;
                spawn.unk8 = 0x5103;
                spawn.unk0 = 0x200005;
                spawn.unk1E = 0x2E;
                spawn.unk20 = 5;
                spawn.unk58 = 0x90DE07;
                spawn.unk60 = 8;
                spawn.unk61 = 6;
                spawn.unk62 = 0x24;
                spawn.unk63 = -1;
                spawn.unk4 = 0;
                spawn.unkC = 0;
                spawn.unk10 = 0;
                spawn.unk64 = -1;
                spawn.unk65 = 0;
                spawn.unk22 = 0x2E;
                spawn.unk14 = 0xDD;
                spawn.unk15 = 0xD3;
                spawn.unk16 = 0xCD;
                spawn.unk17 = 0xFF;
                spawn.unk18 = 0x57;
                spawn.unk19 = 0x55;
                spawn.unk1A = 0x5A;
                spawn.unk1C = 0xFF;
                spawn.unk3C.unk0 = 0.0f;
                spawn.unk3C.unk4 = 0.0f;
                spawn.unk3C.unk8 = 0.0f;
                spawn.unk24 = 1.038234949f;
                spawn.pos = arg1->unk8;
                spawn.vel = *(struct17 *)&D_800A5480;
                do {
                    sub.unk0 = func_150ADA20();
                    sub.unk1 = (s16)func_150ADA20();
                    sub.unk2 = (func_150ADA20() % 5U) + 4;
                    sub.unk3 = (func_150ADA20() % 5U) + 4;
                    sub.unk4 = func_150ADA68() * 10.0f;
                    sub.unk8 = func_150ADA68() * 10.0f;
                    spawn.unk54 = func_150ADA68() * 0.0910000056f + 0.1010000035f;
                    spawn.unk58 &= ~0xC0;
                    spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
                    spawn.unk1B = (func_150ADA20() % 101U) + 100;
                    spawn.unkA = (func_150ADA20() % 11U) + 70;
                    spawn.unk28 = spawn.unk2C = func_150ADA68() * 116.0f + 131.0f;
                    ret = func_15130280(&spawn, 1, 0, 0x10, arg0->unkC, arg0->unk1);
                    if (ret != NULL) {
                        memcpy((u8 *)ret + 0xA8, &sub, sizeof(sub));
                    }
                    data->unk48 -= 1.0f;
                } while (data->unk48 > 1.0f);
            }
        } else {
            data->unk44 += (0.06000000238f + func_150ADA68() * 0.06000000238f) * D_800BE9A4;
            if (data->unk44 > 1.0f) {
                Emit150DA67C emit;
                Dst150DA67C dst;

                func_150472C0(&dst, arg1);
                emit.unk0 = arg1->unk8;
                emit.unkC = 3.480000019f;
                emit.unk24 = 1;
                emit.unk26 = 2;
                emit.unk28 = 3;
                emit.unk2A = 1;
                emit.unk2C = 0x14;
                emit.unk2E = 0xA;
                emit.unk30 = 0xC8;
                emit.unk32 = 0x37;
                emit.unk3C = 0x10;
                emit.unk3E = 0xF;
                emit.unk40 = 0;
                emit.unk34 = 3;
                emit.unk10 = 2.659999847f;
                emit.unk14 = 1.33100009f;
                emit.unk18 = 1.160000086f;
                emit.unk1C = 0.01499999966f;
                emit.unk20 = 0.00699999975f;
                emit.unk38 = 0.200000003f;
                if (arg1->unk59 >= 2) {
                    ShapeA150DA67C a;

                    a.unk0 = 1;
                    a.unk4 = -arg2[0];
                    a.unk8 = -arg2[1];
                    a.unkC = -arg2[2];
                    a.unk10 = a.unk1C = *(struct17 *)&D_800A5480;
                    a.unk28 = 300.0f;
                    do {
                        func_15153CCC(&a, &emit, &dst, arg0->unkC, arg0->unk1);
                        data->unk44 -= 1.0f;
                    } while (data->unk44 > 1.0f);
                } else {
                    ShapeB150DA67C b;

                    b.unk0 = -arg2[0];
                    b.unk4 = -arg2[1];
                    b.unk8 = -arg2[2];
                    b.unkC = arg1->unk44;
                    b.unk20 = 300.0f;
                    do {
                        func_15153C84(&b, &emit, &dst, arg0->unkC, arg0->unk1);
                        data->unk44 -= 1.0f;
                    } while (data->unk44 > 1.0f);
                }
            }
        }
        if (arg1->unk59 == 1) {
            data->unk40 += (0.200000003f + func_150ADA68() * 0.1040000021f) * D_800BE9A4;
            if (data->unk40 > 1.0f) {
                do {
                    func_151D9B8C(3, func_150ADA68() * 30.0f + 15.0f, (func_150ADA20() % 101U) + 100, (s32)&arg1->unk44, &arg1->unk8,
                                  (func_150ADA20() % 81U) + 60, 1, 1, 1, arg0->unkC, arg0->unk1);
                    data->unk40 -= 1.0f;
                } while (data->unk40 > 1.0f);
            }
        }
    }
}

