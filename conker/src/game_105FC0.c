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

typedef struct {
    struct17 pos;
    u8 padC[0xC];
    f32 len;
    u8 alpha;
    u8 pad1D[3];
    f32 unk20;
} Seg150D942C; /* 0x24 */

typedef struct {
    u8 *obj;
    u8 pad4[0x2C];
    u16 unk30;
    u8 pad32[6];
    f32 unk38;
} Data150D942C;

typedef struct {
    u8 pad0[0x10];
    struct17 pos;
    u8 pad1C[2];
    u16 flags;
    u8 pad20[5];
    u8 cap;
    u8 pad26[6];
    s8 count;
    s8 tail;
    s8 head;
    u8 pad2F[0x94 - 0x2F];
    Seg150D942C *buf;
    Data150D942C *data;
} Self150D942C;

typedef struct {
    u8 *obj;
    f32 dist;
    struct17 pos;
    u8 pad14[0x45];
    u8 flag;
    u8 pad5A[0x68 - 0x5A];
} Hit150D942C;

f32 sqrtf(f32);




extern f32 func_150484A0(f32, f32);
extern f32 func_15143E64(f32 *);
extern f32 func_15144528(f32, f32, f32);
extern void func_15081690(u8 *, s32, s32, s32, f32, f32, f32, Hit150D942C *, f32, s32, s32, s32, s32, s32, s32);
extern void func_1505A250(f32, f32, f32, f32 *, f32 *);
extern void func_1505D024(u8 *, s32, u16, s32);
extern void func_150DA67C();

#define OB(o, off, T) (*(T *)((u8 *)(o) + (off)))

s32 func_150D942C(Self150D942C *arg0) {
    s32 sz;
    s32 i;
    f32 acc;
    f32 sp17C;
    f32 f24;
    f32 f2;
    f32 sp164[3];
    f32 f0;
    f32 f12;
    f32 sp150[3];
    Hit150D942C spE8;
    Seg150D942C *buf;
    Seg150D942C *prev;
    Data150D942C *data;
    u8 *obj;
    u8 *hobj;
    f32 t16;
    f32 t18;
    s32 conv;
    s32 i2, i3;
    f32 rcp;
    f32 spB8;
    f32 inv;

    sz = arg0->count;
    data = arg0->data;
    buf = arg0->buf;
    obj = data->obj;
    if (sz >= 2) {
        acc = 0.0f;
        i = arg0->head;
        if (arg0->flags & 2) {
            prev = (Seg150D942C *)&arg0->pos;
        } else {
            i -= 1;
            if (i < 0) {
                i = arg0->cap - 1;
            }
            prev = &buf[i];
        }
        f24 = 1195.0f;
        do {
            Seg150D942C *cur;
            f12 = -9999.0f;
            i -= 1;
            if (i < 0) {
                i = arg0->cap - 1;
            }
            cur = &buf[i];
            sp164[0] = cur->pos.unk0 - prev->pos.unk0;
            sp164[1] = cur->pos.unk4 - prev->pos.unk4;
            sp164[2] = cur->pos.unk8 - prev->pos.unk8;
            f0 = func_15143E64(sp164);
            acc += f0;
            cur->len = f0;
            if (f24 < acc) {
                if (cur->len != 0) {
                    f12 = 1.0f / cur->len;
                    sp150[0] = sp164[0] * f12;
                    sp150[1] = sp164[1] * f12;
                    f2 = (acc - f24) * f12;
                    sp150[2] = sp164[2] * f12;
                    cur->pos.unk0 = cur->pos.unk0 - sp164[0] * f2;
                    cur->pos.unk4 = cur->pos.unk4 - sp164[1] * f2;
                    cur->pos.unk8 = cur->pos.unk8 - sp164[2] * f2;
                    cur->len = cur->len * (1.0f - f2);
                }
                acc = f24;
                if (i != arg0->tail) {
                    do {
                        arg0->tail += 1;
                        if (arg0->tail == arg0->cap) {
                            arg0->tail = 0;
                        }
                        arg0->count -= 1;
                    } while (i != arg0->tail);
                }
            }

            if (cur->len != 0.0f) {
                if (f12 == -9999.0f) {
                    f12 = 1.0f / cur->len;
                    sp150[0] = sp164[0] * f12;
                    sp150[1] = sp164[1] * f12;
                    sp150[2] = sp164[2] * f12;
                }
                func_15081690(obj, *(s32 *)&prev->pos.unk0, *(s32 *)&prev->pos.unk4, *(s32 *)&prev->pos.unk8, sp150[0], sp150[1], sp150[2], &spE8, cur->len, 0, 0, 1, -1, 0, 0);
                if (spE8.flag != 0) {
                    func_150DA67C(arg0, &spE8, sp150);
                    if (spE8.flag >= 2) {
                        hobj = spE8.obj;
                        OB(hobj, 0x2EC, s32) = 1;
                        if (OB(hobj, 4, u8) == 0x3A) {
                            OB(hobj, 0x232, u8) = 0xA;
                            OB(hobj, 0x218, s32) = 0;
                        } else if (OB(hobj, 4, u8) == 0x10 && OB(hobj, 0x251, u8) == 1) {
                            func_1505A250(OB(hobj, 0x14, f32) - spE8.pos.unk0, OB(hobj, 0x1C, f32) - spE8.pos.unk8, ((1.0f - (acc - 50.0f) * 0.001052631531f) * 0.4550000131f) + 0.2010000050f, &OB(hobj, 0x164, f32), &OB(hobj, 0x168, f32));
                            if (OB(hobj, 0x232, u8) == 0xF) {
                                s32 padx;
                                OB(hobj, 0x218, s32) = 0;
                            }
                        } else if (OB(hobj, 4, u8) == 0x10) {
                            t16 = cur->pos.unk0 - OB(obj, 0x14, f32);
                            t18 = cur->pos.unk8 - OB(obj, 0x1C, f32);
                            if (OB(hobj, 0x232, u8) == 0x10) {
                                OB(hobj, 0x3C, f32) = OB(hobj, 0x3C, f32) * 0.7200000286f;
                                f2 = 1.0f / (sqrtf(t16 * t16 + t18 * t18) * -0.07000000030f);
                                func_1505A250(t16 * f2, t18 * f2, 0.7f, &OB(hobj, 0x164, f32), &OB(hobj, 0x168, f32));
                            }
                            if (OB(hobj, 0x125, u8) == 0) {
                                s32 pady;
                                OB(hobj, 0x232, u8) = 0xF;
                                OB(hobj, 0x218, s32) = 0;
                            }
                        } else if (OB(hobj, 4, u8) == 0x91) {
                            OB(hobj, 0x232, u8) = 3;
                            OB(hobj, 0x218, s32) = 0;
                        } else if (OB(hobj, 4, u8) == 0x90) {
                            OB(hobj, 0x232, u8) = 0x2C;
                            OB(hobj, 0x218, s32) = 0;
                        } else {
                            conv = (struct127 *)obj - D_800CC2D0;
                            func_1505D024(hobj, 0xE0036, ((u16)(func_150484A0(sp150[0], sp150[2]) * 10430.37793f) - 0x4000) | 1, conv);
                        }
                    }
                    cur->pos = spE8.pos;
                    acc = acc - cur->len;
                    cur->len = spE8.dist;
                    acc += cur->len;
                    if (i != arg0->tail) {
                        do {
                            arg0->tail += 1;
                            if (arg0->tail == arg0->cap) {
                                arg0->tail = 0;
                            }
                            arg0->count -= 1;
                        } while (i != arg0->tail);
                    }
                    if (data->unk30 != 0) {
                        func_1000F91C(data->unk30, 0x7FFF, 0, 0, 0, (s32)cur->pos.unk0, (s32)cur->pos.unk4, (s32)cur->pos.unk8, 0x1F4, 0x1388);
                    }
                }
            }
            prev = cur;
        } while (i != arg0->tail);
        sz = arg0->count;
    }
    if (sz >= 2) {
        data->unk38 = data->unk38 + -395.0f * D_800BE9A4;
        data->unk38 = func_15144528(data->unk38, 16383.0f, -16384.0f);
        spB8 = 0.0f;
        i2 = arg0->head;
        for (;;) {
            Seg150D942C *cur2;
            i2 -= 1;
            if (i2 < 0) {
                i2 = arg0->cap - 1;
            }
            cur2 = &buf[i2];
            spB8 += cur2->len;
            cur2->unk20 = func_15144528(data->unk38 + spB8 * 29.02999878f, 16383.0f, -16384.0f);
            if (i2 != arg0->tail) {
                continue;
            }
            break;
        }
        sz = arg0->count;
    }
    if (sz >= 2) {
        f0 = 0.0f;
        t16 = acc * 0.8020000458f;
        i3 = arg0->head;
        inv = acc - t16;
        rcp = 1.0f / inv;
        do {
            Seg150D942C *cur3;
            i3 -= 1;
            if (i3 < 0) {
                i3 = arg0->cap - 1;
            }
            cur3 = &buf[i3];
            f0 += cur3->len;
            if (t16 < f0) {
                s32 padz;
                cur3->alpha = (u32)((inv - (f0 - t16)) * rcp * 155.0f);
            } else {
                cur3->alpha = 0x9B;
            }
        } while (i3 != arg0->tail);
    }
    return 1;
}


typedef struct Game105FC0Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Game105FC0Vec3f;
typedef struct Game105FC0TrailRecord {
    Game105FC0Vec3f position;
    u8 pad0C[0x10];
    u8 alpha;
    u8 pad1D[3];
    f32 texture;
} Game105FC0TrailRecord;

void func_151D5D60(void *, s16, s32, void **, u8 *);
s32 func_15144B34(s32);
s32 func_15142E24(s32, s32, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
extern u8 D_80091238[];
extern s32 D_800A4AC8[][2];
extern s32 D_800D2C9C;

Gfx *func_150D9C7C(u8 *arg0, Gfx *arg1, s16 arg2) {
    Game105FC0TrailRecord *records;
    Game105FC0Vec3f next;
    Game105FC0Vec3f cur;
    Game105FC0Vec3f toCamera;
    Game105FC0Vec3f delta;
    Game105FC0Vec3f normal;
    u8 *state;
    Game105FC0Vec3f *camera;
    s32 index;
    s32 previous;
    f32 lengthSquared;
    u8 flag;
    u8 alpha;
    f32 nextTexture;
    f32 curTexture;
    Vtx_t *vtx;

    if (*(s8 *)(arg0 + 0x2C) < 2) {
        return arg1;
    }
    state = *(u8 **)(arg0 + 0x98);
    records = *(Game105FC0TrailRecord **)(arg0 + 0x94);
    func_151D5D60(arg0 + 0x84, arg2, (arg0[0x25] * 2 + 10) * sizeof(Vtx_t), (void **)&vtx, NULL);
    if (vtx == NULL) {
        return arg1;
    }
    camera = (Game105FC0Vec3f *)func_15144B34(arg2);
    flag = 1;
    arg1 = (Gfx *)func_15142E24((s32)arg1, (s32)D_80091238, 0, 0, 0, 0, 0x92, 0, 0, &flag, 3);
    arg1 = func_15142B7C(arg1, 0x220005, 0x1D0600);
    arg1 = func_1513F4E4(arg1, 0x4E, &flag);
    arg1 = func_15142FBC(arg1, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[3][0] | D_800A4AC8[3][1], &flag);
    if (*(u16 *)(arg0 + 0x1E) & 2) {
        index = *(s8 *)(arg0 + 0x2E) - 1;
        if (index < 0) {
            index = arg0[0x25] - 1;
        }
        cur = *(Game105FC0Vec3f *)(arg0 + 0x10);
        curTexture = *(f32 *)(state + 0x38);
    } else {
        previous = *(s8 *)(arg0 + 0x2E) - 1;
        if (previous < 0) {
            previous = arg0[0x25] - 1;
        }
        index = previous - 1;
        if (index < 0) {
            index = arg0[0x25] - 1;
        }
        cur = records[previous].position;
        curTexture = records[previous].texture;
    }
    next = records[index].position;
    alpha = records[index].alpha;
    nextTexture = records[index].texture;
    delta.x = cur.x - next.x;
    delta.y = cur.y - next.y;
    delta.z = cur.z - next.z;
    toCamera.x = cur.x - camera->x;
    toCamera.y = cur.y - camera->y;
    toCamera.z = cur.z - camera->z;
    normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
    normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
    normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
    lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
    if (lengthSquared != 0.0f) {
        f32 scale = 4.0f / sqrtf(lengthSquared);

        normal.x *= scale;
        normal.y *= scale;
        normal.z *= scale;
    } else {
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
    }
    vtx->ob[0] = cur.x + normal.x;
    vtx->ob[1] = cur.y + normal.y;
    vtx->ob[2] = cur.z + normal.z;
    vtx->tc[0] = curTexture;
    vtx->tc[1] = 0x400;
    vtx->cn[0] = 0xFF;
    vtx->cn[1] = 0xFF;
    vtx->cn[2] = 0xFF;
    vtx->cn[3] = alpha;
    vtx->flag = 0;
    vtx++;
    vtx->ob[0] = cur.x - normal.x;
    vtx->ob[1] = cur.y - normal.y;
    vtx->ob[2] = cur.z - normal.z;
    vtx->tc[0] = curTexture;
    vtx->tc[1] = 0;
    vtx->cn[0] = 0xFF;
    vtx->cn[1] = 0xFF;
    vtx->cn[2] = 0xFF;
    vtx->cn[3] = alpha;
    vtx->flag = 0;
    vtx++;
    do {
        delta.x = cur.x - next.x;
        delta.y = cur.y - next.y;
        delta.z = cur.z - next.z;
        toCamera.x = next.x - camera->x;
        toCamera.y = next.y - camera->y;
        toCamera.z = next.z - camera->z;
        normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
        normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
        normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
        lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
        if (lengthSquared != 0.0f) {
            f32 scale = 4.0f / sqrtf(lengthSquared);

            normal.x *= scale;
            normal.y *= scale;
            normal.z *= scale;
        } else {
            normal.x = 0.0f;
            normal.y = 0.0f;
            normal.z = 0.0f;
        }
        vtx->ob[0] = next.x + normal.x;
        vtx->ob[1] = next.y + normal.y;
        vtx->ob[2] = next.z + normal.z;
        vtx->tc[0] = nextTexture;
        vtx->tc[1] = 0x400;
        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0xFF;
        vtx->cn[2] = 0xFF;
        vtx->cn[3] = alpha;
        vtx->flag = 0;
        vtx++;
        vtx->ob[0] = next.x - normal.x;
        vtx->ob[1] = next.y - normal.y;
        vtx->ob[2] = next.z - normal.z;
        vtx->tc[0] = nextTexture;
        vtx->tc[1] = 0;
        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0xFF;
        vtx->cn[2] = 0xFF;
        vtx->cn[3] = alpha;
        vtx->flag = 0;
        vtx++;
        gSPVertex(arg1++, vtx - 4, 4, 0);
        gSP1Triangle(arg1++, 0, 1, 2, 0);
        gSP1Triangle(arg1++, 1, 3, 2, 0);
        if (nextTexture < curTexture) {
            memcpy(vtx, vtx - 2, 0x20);
            vtx[-2].tc[0] -= 0x8000;
            vtx++;
            vtx[-2].tc[0] -= 0x8000;
            vtx++;
        }
        previous = index;
        index--;
        if (index < 0) {
            index = arg0[0x25] - 1;
        }
        cur = records[previous].position;
        next = records[index].position;
        alpha = records[index].alpha;
        curTexture = records[previous].texture;
        nextTexture = records[index].texture;
    } while (previous != *(s8 *)(arg0 + 0x2D));
    return arg1;
}

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

