#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_15143794(s32, s32, f32, f32 *);
s32 func_1510F8CC(s32);
void func_15152B38(void *, u8, s32);
s32 func_1513170C(s32 *, s32);
struct Obj151A2960;
extern void (*D_8008F8E0[])(struct Obj151A2960 *, s32);
extern f32 D_800A8D3C;
extern f32 D_800A8D40;
extern f32 D_800A8D44;
extern f32 D_800A8D30;
extern f32 D_800A8D34;
extern f32 D_800A8D38;
void func_151A11CC(f32, f32, f32, f32, f32, s32, s32, s32,
                   s32, s32, f32, s32, s32, s32, s32, s32,
                   s32, f32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32);


extern f32 D_800A8CE0;
extern f32 D_800A8CE4;
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

f32 fabsf(f32);
#pragma intrinsic(fabsf)

s32 func_1519EF90(arg0, arg1, arg2)
    u8 *arg0;
    u8 arg1;
    u8 *arg2;
{
    f32 *state;
    f32 *data;
    f32 dx;
    f32 dz;
    f32 dirX;
    f32 dirZ;
    f32 dist;

    data = (f32 *)(*(u8 **)arg0 + 0x58);
    state = *(f32 **)(*(u8 **)arg0 + 0x18);
    if (*(s32 *)((u8 *)state + 0x1D4) == 0) {
        return 0;
    }
    dx = state[5] - *(f32 *)(arg0 + 0x20);
    dz = state[7] - *(f32 *)(arg0 + 0x24);
    if ((D_800A8CE0 < fabsf(dx)) || (D_800A8CE0 < fabsf(dz))) {
        f32 inv;

        inv = 1.0f / sqrtf((dx * dx) + (dz * dz));
        dirX = dz * inv;
        dirZ = dx * inv;
    } else {
        u8 angle;

        angle = func_15143E08((struct127 *)state);
        dirX = func_151423D8(angle);
        dirZ = func_151423D8(angle - 0x40);
    }
    if (arg1 == 6) {
        dist = data[4];
    } else {
        dist = -data[4];
    }
    *(f32 *)(arg2 + 0) = state[5] + (dist * dirX);
    *(f32 *)(arg2 + 8) = state[7] - (dist * dirZ);
    if (state[0x46] < D_800A8CE4) {
        *(f32 *)(arg2 + 4) = state[6];
    } else {
        *(f32 *)(arg2 + 4) = state[0x46];
    }
    return 1;
}

struct Sub1519F108 {
    u8 pad0[0x58];
    s32 unk58;
    s32 unk5C;
    s32 unk60;
};

struct Obj1519F108 {
    u8 pad0[0x20];
    s32 unk20;
    u8 pad24[0x74];
    struct Sub1519F108 **unk98;
};

void func_151478F4(struct Obj1519F108 *);

void func_1519F108(struct Obj1519F108 *arg0) {
    struct Sub1519F108 **pp = arg0->unk98;
    s32 *ptr;
    if (*pp != 0) {
        ptr = (s32 *)((u8 *)*pp + 0x58);
        if (arg0->unk20 == 6) {
            ptr[0] = 0;
        }
        if (arg0->unk20 == 7) {
            ptr[2] = 0;
        }
    }
    func_151478F4(arg0);
}

void func_15147928(struct Obj1519F108 *);

void func_1519F168(struct Obj1519F108 *arg0) {
    struct Sub1519F108 **pp = arg0->unk98;
    s32 *ptr;
    if (*pp != 0) {
        ptr = (s32 *)((u8 *)*pp + 0x58);
        if (arg0->unk20 == 6) {
            ptr[0] = 0;
        }
        if (arg0->unk20 == 7) {
            ptr[2] = 0;
        }
    }
    func_15147928(arg0);
}

typedef struct {
    struct17 unk0;
    s16 unkC;
    u16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
} Header1519F1C8;

typedef struct {
    void *unk0;
    u8 unk4;
    u8 pad5[3];
    f32 unk8;
    f32 unkC;
    struct17 unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 pad2D;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    u8 pad36[2];
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 unk43;
    u8 unk44;
    u8 unk45;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
} Payload1519F1C8;

extern u8 D_800AB414[];
u8 func_151D8E20(void);
void *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, void *, s32, s32);

s32 *func_1519F1C8(arg0, arg1)
    u8 *arg0;
    u8 arg1;
{
    f32 *obj;
    Header1519F1C8 header;
    Payload1519F1C8 payload;
    s32 *result;
    u8 color;
    u32 mode;

    obj = *(f32 **)(arg0 + 0x18);
    color = func_151D8E20();
    payload.unk0 = arg0;
    header.unkE = 0x42;
    header.unkC = 0x12C;
    header.unk15 = 0xA;
    payload.unk2E = 0xC8;
    payload.unk4 = 6;
    payload.unk40 = 3;
    payload.unk41 = 0x55;
    payload.unk42 = 3;
    payload.unk43 = 0x55;
    payload.unk44 = 0x88;
    payload.unk45 = 0xC4;
    payload.unk28 = 0;
    payload.unk46 = 0;
    payload.unk30 = 0xFF;
    payload.unk32 = 0x28;
    payload.unk34 = 0x19;
    payload.unk48 = 6;
    payload.unk4A = 0x325;
    payload.unk8 = 0.0150000006f;
    payload.unk38 = 5.800000191f;
    payload.unk3C = 1.0f;
    payload.unkC = 0.0f;
    payload.unk1C = 0.0f;
    mode = arg1;
    header.unk10 = mode;
    payload.unk4C = 1.5f;
    payload.unk20 = obj[5];
    payload.unk24 = obj[7];
    if (func_1519EF90((u8 *)&payload, (u8)mode, (u8 *)&header) != 0) {
        payload.unk10 = header.unk0;
        header.unkE |= 4;
    }
    payload.unk2A = D_800AB414[color * 3];
    payload.unk2B = D_800AB414[color * 3 + 1];
    payload.unk2C = D_800AB414[color * 3 + 2];
    result = func_15147A80(&header, 0x50, 0x24, 5, 5, 5, 0, 0, arg0 + 0x34, 0xFF, 1);
    if (result != NULL) {
        memcpy((void *)result[0x98 / 4], &payload, 0x50);
    }
    return result;
}

s32 *func_1519F1C8();

struct Sub1519F3B8 {
    s32 *unk0;
    s32 unk4;
    s32 *unk8;
    s32 unkC;
};

struct Obj1519F3B8 {
    u8 pad0[0x58];
    struct Sub1519F3B8 unk58;
};

void func_1519F3B8(struct Obj1519F3B8 *arg0) {
    struct Sub1519F3B8 *p = &arg0->unk58;
    p->unk0 = func_1519F1C8(arg0, 6);
    p->unk4 = 0;
    p->unk8 = func_1519F1C8(arg0, 7);
    p->unkC = 0;
}

struct Obj1519F400 {
    u8 pad0[0x58];
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
};

void func_1519F48C();
void func_151A0928();

void func_1519F400(struct Obj1519F400 *arg0) {
    s32 *p;
    p = (s32 *)((u8 *)arg0 + 0x58);
    if (p[0] != 0) {
        func_1519F48C(p[0]);
    }
    if (p[2] != 0) {
        func_1519F48C(p[2]);
    }
    if (p[1] != 0) {
        func_151A0928(p[1]);
        func_1516972C((struct102 *)p[1]);
    }
    if (p[3] != 0) {
        func_151A0928(p[3]);
        func_1516972C((struct102 *)p[3]);
    }
}

struct Sub1519F48C {
    u8 pad0[0x58];
    s32 unk58;
    s32 unk5C;
    s32 unk60;
};

struct Ref1519F48C {
    struct Sub1519F48C *unk0;
    u8 unk4;
};

struct Obj1519F48C {
    u8 pad0[0x1E];
    u16 unk1E;
    s32 unk20;
    u8 pad24[0xC];
    u8 unk30;
    u8 pad31[0x67];
    struct Ref1519F48C *unk98;
};

void func_1519F48C(struct Obj1519F48C *arg0) {
    struct Ref1519F48C *p = arg0->unk98;
    s32 *ptr;

    if (p->unk0 != 0) {
        ptr = (s32 *)((u8 *)p->unk0 + 0x58);
        if (arg0->unk20 == 6) {
            ptr[0] = 0;
        }
        if (arg0->unk20 == 7) {
            ptr[2] = 0;
        }
        p->unk0 = 0;
    }
    arg0->unk30 = 0;
    arg0->unk1E &= ~2;
    p->unk4 |= 1;
}

typedef struct Vec3f_1519F4F0 {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_1519F4F0;

typedef struct Config_1519F4F0 {
    void *owner;
    u8 type;
    u8 pad5[3];
    f32 field8;
    f32 fieldC;
    Vec3f_1519F4F0 position;
    f32 field1C;
    f32 field20;
    f32 field24;
    s16 field28;
    u8 color[3];
    u8 pad2D;
    s16 field2E;
    s16 field30;
    s16 field32;
    s16 field34;
    u8 pad36[2];
    f32 field38;
    f32 field3C;
    u8 field40;
    u8 field41;
    u8 field42;
    u8 field43;
    u8 field44;
    u8 field45;
    s16 field46;
    s16 field48;
    s16 field4A;
    f32 field4C;
} Config_1519F4F0;

typedef struct TrailPoint_1519F4F0 {
    f32 x, z, y, fieldC;
    s16 size, delay, alpha;
    u8 pad16[2];
    s16 life;
    u8 pad1A[2];
    f32 velocityX, velocityZ;
} TrailPoint_1519F4F0;

s32 func_15045800(Vec3f_1519F4F0 *, u16, f32, void *);
extern f32 D_800BE9A4;
extern s32 D_800BE9E4;

s32 func_1519F4F0(u8 *arg0) {
    TrailPoint_1519F4F0 *base;
    TrailPoint_1519F4F0 *point;
    Config_1519F4F0 *config;
    s32 index, expired;

    config = *(Config_1519F4F0 **)(arg0 + 0x98);
    base = *(TrailPoint_1519F4F0 **)(arg0 + 0x94);
    if (*(s8 *)(arg0 + 0x2C) < 2 && (config->type & 1)) return 0;
    index = *(s8 *)(arg0 + 0x2E);
    if (index != *(s8 *)(arg0 + 0x2D)) {
        do {
            index--;
            expired = 0;
            if (index < 0) index = arg0[0x25] - 1;
            point = base + index;
            point->life = (u32)point->life - (u32)D_800BE9E4;
            if (point->life < 0) expired = 1;
            point->alpha = 255;
            if (point->delay > 0) point->delay = (u32)point->delay - (u32)D_800BE9E4;
            else point->size = (u32)point->size - (u32)config->field34 * (u32)D_800BE9E4;
            point->fieldC += config->field3C * D_800BE9A4;
            point->x += point->velocityX * D_800BE9A4;
            point->z += point->velocityZ * D_800BE9A4;
            {
                Vec3f_1519F4F0 probe;
                probe.x = point->x;
                probe.y = point->y + 30.0f;
                probe.z = point->z;
                if (20000.0f < fabsf(probe.x) || 20000.0f < fabsf(probe.z)) expired = 1;
                else if (func_15045800(&probe, 0, point->y - 30.0f, arg0 + 0x60) != 0) {
                    point->y = *(f32 *)(arg0 + 0x60);
                } else expired = 1;
            }
            if (point->size < 0) expired = 1;
            if (expired != 0) {
                if (index != *(s8 *)(arg0 + 0x2D)) {
                    do {
                        *(s8 *)(arg0 + 0x2D) += 1;
                        if (arg0[0x25] == *(s8 *)(arg0 + 0x2D)) *(s8 *)(arg0 + 0x2D) = 0;
                        *(s8 *)(arg0 + 0x2C) -= 1;
                    } while (index != *(s8 *)(arg0 + 0x2D));
                }
                base[*(s8 *)(arg0 + 0x2D)].size = 0;
            }
        } while (index != *(s8 *)(arg0 + 0x2D));
    }
    config->field46 = (u32)config->field46 + (u32)config->field48 * (u32)D_800BE9E4;
    if (*(s8 *)(arg0 + 0x2C) > 0) {
        *(f32 *)(arg0 + 0x54) = base[*(s8 *)(arg0 + 0x2D)].x;
        *(f32 *)(arg0 + 0x58) = base[*(s8 *)(arg0 + 0x2D)].y;
        *(f32 *)(arg0 + 0x5C) = base[*(s8 *)(arg0 + 0x2D)].z;
    } else {
        *(f32 *)(arg0 + 0x54) = 0.0f;
        *(f32 *)(arg0 + 0x58) = 0.0f;
        *(f32 *)(arg0 + 0x5C) = 0.0f;
    }
    return 1;
}

typedef struct {
    f32 x, y, z;
} Vec3_1519F7F0;

typedef struct {
    f32 x, z;
} Vec2_1519F7F0;

struct Big1519F7F0 {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
    u8 pad3C[0xDC];
    f32 unk118;
    u8 pad11C[0xB8];
    s32 unk1D4;
};

struct Sub1519F7F0 {
    u8 pad0[0x18];
    struct Big1519F7F0 *unk18;
    u8 unk1C;
    u8 pad1D[0x3B];
    s32 unk58[4];
};

struct Ref1519F7F0 {
    struct Sub1519F7F0 *unk0;
    u8 unk4;
    f32 unk8;
    f32 unkC;
    Vec3_1519F7F0 unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    s16 pad2A[2];
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    u8 pad34[4];
    f32 unk38;
    f32 unk3C;
    u8 pad40[4];
    u8 unk44;
    u8 unk45;
    u8 pad46[6];
    f32 unk4C;
};

struct Elem1519F7F0 {
    f32 x;
    f32 z;
    f32 unk8;
    f32 unkC;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    Vec2_1519F7F0 vel;
};

struct Obj1519F7F0 {
    u8 pad0[0x10];
    Vec3_1519F7F0 unk10;
    u8 pad1C[2];
    u16 unk1E;
    s32 unk20;
    u8 pad24;
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x31];
    f32 unk60;
    u8 pad64[0x30];
    struct Elem1519F7F0 *unk94;
    struct Ref1519F7F0 *unk98;
};

typedef struct {
    struct Sub1519F7F0 *sub;
    s8 type;
} FxData1519F7F0;

s32 func_1519F7F0(struct Obj1519F7F0 *arg0) {
    struct Ref1519F7F0 *ref;
    struct Elem1519F7F0 *elems;
    struct Big1519F7F0 *big;
    s32 padE;
    s32 found;
    f32 stepH;
    s32 i;
    s32 j;
    f32 invDist;
    FxData1519F7F0 data;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    f32 speedStep;
    f32 inv;
    Vec3_1519F7F0 cur;
    f32 speed;
    f32 y;
    f32 dyy;
    s32 *slots;
    struct260 *fx;
    f32 stepX;
    Vec2_1519F7F0 vel;
    f32 stepY;
    f32 stepZ;

    ref = arg0->unk98;
    elems = arg0->unk94;
    big = ref->unk0->unk18;
    if (big->unk0 == 0) {
        return 0;
    }
    if (big->unk1D4 == 0) {
        return 0;
    }
    if (big->unk3B != ref->unk0->unk1C) {
        return 0;
    }
    found = 0;
    if (!(arg0->unk1E & 4)) {
        if (func_1519EF90(ref, (u8)arg0->unk20, &arg0->unk10, elems) != 0) {
            found = 1;
            ref->unk10 = arg0->unk10;
            arg0->unk1E |= 4;
        } else {
            return 1;
        }
    }
    if (!found && func_1519EF90(ref, (u8)arg0->unk20, &arg0->unk10, elems) == 0) {
        func_1519F48C(arg0);
        slots = ref->unk0->unk58;
        data.sub = ref->unk0;
        data.type = arg0->unk20;
        fx = func_151491F4(0x12C, -1, 0xB, 0, 6, 8, 0xFF, 0);
        if (fx != NULL) {
            memcpy((u8 *)fx + 0x28, &data, 8);
        }
        if (arg0->unk20 == 6) {
            slots[1] = (s32)fx;
        } else {
            slots[3] = (s32)fx;
        }
        return 1;
    }
    ref->unk20 = big->unk14;
    ref->unk24 = big->unk1C;
    dx = arg0->unk10.x - ref->unk10.x;
    dy = arg0->unk10.y - ref->unk10.y;
    dz = arg0->unk10.z - ref->unk10.z;
    if (9.999999747e-05f < fabsf(dx) || 9.999999747e-05f < fabsf(dy) || 9.999999747e-05f < fabsf(dz)) {
        dist = sqrtf(dx * dx + dy * dy + dz * dz);
        ref->unkC += dist * ref->unk8;
        invDist = 1.0f / dist;
    }
    if (ref->unkC > 1.0f) {
        inv = 1.0f / ref->unkC;
        speed = ref->unk1C + D_800BE9A4;
        speedStep = speed * inv;
        cur = ref->unk10;
        y = ref->unk38 + ref->unk3C * speed;
        dyy = ref->unk38 - y;
        if (arg0->unk20 == 7) {
            vel.x = -dz * invDist * ref->unk4C;
            vel.z = dx * invDist * ref->unk4C;
        } else {
            vel.x = dz * invDist * ref->unk4C;
            vel.z = -dx * invDist * ref->unk4C;
        }
        i = arg0->unk2E - 1;
        if (i < 0) {
            i = arg0->unk25 - 1;
        }
        stepX = dx * inv;
        stepY = dy * inv;
        stepZ = dz * inv;
        stepH = dyy * inv;
        if (i != arg0->unk2D && arg0->unk2C != 0) {
            j = i - 1;
            if (j < 0) {
                j = arg0->unk25 - 1;
            }
            (elems + j)->vel.x = ((elems + j)->vel.x + vel.x) * 0.5f;
            (elems + j)->vel.z = ((elems + j)->vel.z + vel.z) * 0.5f;
        }
        do {
            s32 ok;
            struct Elem1519F7F0 *e;

            ok = 1;
            e = &elems[arg0->unk2E];
            e->vel = vel;
            e->x = cur.x;
            e->z = cur.z;
            e->unk18 = ref->unk2E;
            e->unkC = y;
            e->unk10 = ref->unk30;
            e->unk12 = ref->unk32;
            e->unk14 = 0xFF;
            e->unk16 = ref->unk28;
            ref->unk28 += ref->unk44 + (u32)func_150ADA20() % (ref->unk45 + 1);
            e->x += e->vel.x * speed;
            e->z += e->vel.z * speed;
            {
            f32 probe[3];

            probe[0] = e->x;
            if (big->unk118 < -9500.0f) {
                probe[1] = big->unk18 + 100.0f;
            } else {
                probe[1] = big->unk118 + 100.0f;
            }
            probe[2] = e->z;
            if (fabsf(probe[0]) > 20000.0f || fabsf(probe[2]) > 20000.0f) {
                ok = 0;
            } else if (func_15045800(probe, 0, probe[1] - 200.0f, &arg0->unk60) != 0) {
                e->unk8 = arg0->unk60;
            } else {
                ok = 0;
            }
            }
            if (ok) {
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
            cur.x += stepX;
            cur.y += stepY;
            cur.z += stepZ;
            y += stepH;
            speed -= speedStep;
            ref->unkC -= 1.0f;
        } while (ref->unkC > 1.0f);
        ref->unk10 = cur;
        ref->unk1C = speed;
    }
    return 1;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_1519FE6C;

typedef struct {
    f32 x;
    f32 z;
    f32 y;
    f32 width;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u8 pad18[0xC];
} Elem1519FE6C;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x25];
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 pad2D[0xB];
    f32 unk38;
    u8 pad3C[4];
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 unk43;
    u8 pad44[2];
    s16 unk46;
    u8 pad48[2];
    s16 unk4A;
} Params1519FE6C;

typedef struct {
    u8 pad0[0x10];
    Vec3f_1519FE6C unk10;
    u8 pad1C[2];
    u16 unk1E;
    s32 unk20;
    u8 pad24;
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x55];
    u8 unk84[0x10];
    Elem1519FE6C *unk94;
    Params1519FE6C *unk98;
} Obj1519FE6C;

extern u8 D_80090E0C[];
extern s32 D_800D2C9C;
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

#define EL(n) ((Elem1519FE6C *)((u8 *)elems + (n) * sizeof(Elem1519FE6C)))
Gfx *func_1519FE6C(Obj1519FE6C *arg0, Gfx *gfx, s16 arg2) {
    Vtx *vtx;
    Params1519FE6C *params;
    Elem1519FE6C *elems;
    Vec3f_1519FE6C cur;
    Vec3f_1519FE6C prev;
    s32 i;
    s32 j;
    f32 nx;
    s16 cnt1;
    s16 cnt2;
    s16 prevColor;
    s16 curColor;
    s32 prevAlpha;
    s32 curAlpha;
    f32 dx;
    f32 dz;
    f32 prevWidth;
    s32 a;
    s32 b;
    s16 tc[3];
    u8 alpha1;
    u8 alpha2;
    u8 update;
    f32 curWidth;
    f32 invlen;
    f32 nz;
    s32 k;

    if (arg0->unk2C >= 2) {
        func_151D5D60(arg0->unk84, arg2, (arg0->unk25 * 3 << 4) + 0x120, (void **)&vtx, 0);
        if (vtx != NULL) {
            params = arg0->unk98;
            elems = arg0->unk94;
            update = 1;
            if (params->unk4 & 2) {
                alpha1 = 0;
                cnt1 = params->unk40;
                j = arg0->unk2D;
                do {
                    elems[j].unk14 = alpha1;
                    j++;
                    alpha1 += params->unk41;
                    if (j == arg0->unk25) {
                        j = 0;
                    }
                    cnt1--;
                } while (cnt1 != 0 && j != arg0->unk2E);
                do { } while (0);
            }
            if (params->unk4 & 4) {
                alpha2 = 0;
                cnt2 = params->unk42;
                j = arg0->unk2E - 1;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                do {
                    elems[j].unk14 = (elems[j].unk14 * alpha2) >> 8;
                    j--;
                    alpha2 += params->unk43;
                    cnt2--;
                    if (j < 0) {
                        j = arg0->unk25 - 1;
                    }
                } while (cnt2 != 0 && j != arg0->unk2E);
            }
            gfx = func_15142E24(gfx, (s32)D_80090E0C, 0, 0, 0, 0, 0x39, 0, 0, &update, 0x3E);
            gfx = func_15142B7C(gfx, 1, 0x160600);
            gfx = func_15142C10(gfx, params->unk2A, params->unk2B, params->unk2C, 0xFF, &update);
            gfx = func_15142CF0(gfx, 0, 0, 0xFF, 0xFF, 0xFF, 0, &update);
            gfx = func_1513F4E4(gfx, 7, &update);
            gfx = func_15142FBC(gfx, G_TP_PERSP | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, 0x5049D8, &update);
            if (arg0->unk1E & 2) {
                i = arg0->unk2E - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                prev = arg0->unk10;
                prevAlpha = (EL(i)->unk14 * EL(i)->unk10) >> 8;
                prevColor = EL(i)->unk16;
                prevWidth = params->unk38;
            } else {
                k = arg0->unk2E - 1;
                if (k < 0) {
                    k = arg0->unk25 - 1;
                }
                i = k - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                prev.x = EL(k)->x;
                prev.y = EL(k)->y;
                prev.z = EL(k)->z;
                prevAlpha = (EL(k)->unk14 * EL(k)->unk10) >> 8;
                prevWidth = EL(k)->width;
                prevColor = EL(k)->unk16;
            }
            cur.x = EL(i)->x;
            cur.y = EL(i)->y;
            cur.z = EL(i)->z;
            b = 2;
            curColor = EL(i)->unk16;
            curWidth = EL(i)->width;
            curAlpha = (EL(i)->unk14 * EL(i)->unk10) >> 8;
            dx = cur.x - prev.x;
            dz = cur.z - prev.z;
            if (0.0001f < fabsf(dx) || 0.0001f < fabsf(dz)) {
                invlen = 1.0f / sqrtf(dx * dx + dz * dz);
            } else {
                invlen = 1.0f;
            }
            tc[0] = params->unk46;
            tc[2] = params->unk46 + params->unk4A;
            if (tc[2] < tc[0]) {
                tc[0] = tc[0] % 1024;
                params->unk46 = tc[0];
                tc[2] = params->unk46 + params->unk4A;
            }
            tc[1] = (tc[0] + tc[2]) >> 1;
            if (arg0->unk20 == 6) {
                a = 0;
            } else if (arg0->unk20 == 7) {
                a = 2;
                b = 0;
            } else {
                b = 0;
                a = 2;
            }
            nx = -dz * invlen * prevWidth;
            nz = dx * invlen * prevWidth;
            vtx->v.ob[0] = prev.x + nx;
            vtx->v.ob[1] = prev.y;
            vtx->v.ob[2] = prev.z + nz;
            vtx->v.tc[0] = prevColor;
            vtx->v.tc[1] = tc[a];
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = prev.x;
            vtx->v.ob[1] = prev.y;
            vtx->v.ob[2] = prev.z;
            vtx->v.tc[0] = prevColor;
            vtx->v.tc[1] = tc[1];
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = prevAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = prev.x - nx;
            vtx->v.ob[1] = prev.y;
            vtx->v.ob[2] = prev.z - nz;
            vtx->v.tc[0] = prevColor;
            vtx->v.tc[1] = tc[b];
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0;
            vtx->v.flag = 0;
            vtx++;
            do {
                nx = -dz * invlen * curWidth;
                nz = dx * invlen * curWidth;
                vtx->v.ob[0] = cur.x + nx;
                vtx->v.ob[1] = cur.y;
                vtx->v.ob[2] = cur.z + nz;
                vtx->v.tc[0] = curColor;
                vtx->v.tc[1] = tc[a];
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = 0;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = cur.x;
                vtx->v.ob[1] = cur.y;
                vtx->v.ob[2] = cur.z;
                vtx->v.tc[0] = curColor;
                vtx->v.tc[1] = tc[1];
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = curAlpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = cur.x - nx;
                vtx->v.ob[1] = cur.y;
                vtx->v.ob[2] = cur.z - nz;
                vtx->v.tc[0] = curColor;
                vtx->v.tc[1] = tc[b];
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = 0;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 6, 6, 0);
                gSP1Triangle(gfx++, 0, 3, 4, 0);
                gSP1Triangle(gfx++, 0, 4, 1, 0);
                gSP1Triangle(gfx++, 1, 4, 5, 0);
                gSP1Triangle(gfx++, 1, 5, 2, 0);
                if (prevColor != curColor) {
                }
                k = i--;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                if (k != arg0->unk2D) {
                    prev = cur;
                    cur.x = EL(i)->x;
                    cur.y = EL(i)->y;
                    cur.z = EL(i)->z;
                    dx = cur.x - prev.x;
                    dz = cur.z - prev.z;
                    prevColor = curColor;
                    curColor = EL(i)->unk16;
                    curAlpha = (EL(i)->unk14 * EL(i)->unk10) >> 8;
                    if (dx != 0.0f || dz != 0.0f) {
                        invlen = 1.0f / sqrtf(dx * dx + dz * dz);
                    } else {
                        invlen = 1.0f;
                    }
                    curWidth = EL(i)->width;
                }
            } while (k != arg0->unk2D);
        }
    }
    return gfx;
}


typedef struct Node151A084CData {
    s32 field0;
    u8 pad4[4];
    s32 field8;
} Node151A084CData;

typedef struct Node151A084C {
    u8 pad0[0x18];
    void *field18;
    u8 field1C;
    u8 pad1D[0x3B];
    Node151A084CData data58;
} Node151A084C;

void func_151A084C(u8 *arg0) {
    struct Lookup { Node151A084C *node; u8 mode; };
    s32 result;
    Node151A084CData *data;
    u8 *object;
    u8 done;
    struct Lookup *lookup;

    done = 0;
    lookup = (struct Lookup *)(arg0 + 0x28);
    object = lookup->node->field18;
    data = &lookup->node->data58;
    if (*(s32 *)object == 0) {
        done = 1;
    }
    if (lookup->node->field1C != object[0x3B]) {
        done = 1;
    }
    if ((done == 0) && (*(s32 *)(object + 0x1D4) != 0)) {
        done = 1;
        result = (s32)func_1519F1C8(lookup->node, lookup->mode, lookup, arg0);
        if (lookup->mode == 6) {
            data->field0 = result;
        } else {
            data->field8 = result;
        }
    }
    if (done) {
        func_151A0928(arg0);
        *(s16 *)(arg0 + 0xE) = -1;
        arg0[0xD] |= 1;
    }
}

struct Obj151A0928 {
    u8 pad0[0x28];
    s32 *unk28;
    u8 unk2C;
};

void func_151A0928(struct Obj151A0928 *arg0) {
    s32 *ptr = (s32 *)((u8 *)arg0->unk28 + 0x58);
    if (arg0->unk2C == 6) {
        ptr[1] = 0;
    } else {
        ptr[3] = 0;
    }
}

struct Sub151A0950 {
    u8 pad0[0x18];
    s32 unk18;
    u8 unk1C;
};

struct Obj151A0950 {
    u8 pad0[0x98];
    struct Sub151A0950 **unk98;
};

struct Arg151A0950 {
    s32 unk0;
    u8 unk4;
};

void func_151A0950(struct Obj151A0950 *arg0, struct Arg151A0950 *arg1, u8 arg2) {
    struct Sub151A0950 *v0;
    struct Sub151A0950 **pp = arg0->unk98;
    s32 temp;
    if (arg2 == 10) {
        v0 = *pp;
        if (v0 != 0) {
            temp = v0->unk18;
            if (arg1->unk0 == temp || arg1->unk4 == v0->unk1C) {
                func_1519F48C((struct Obj1519F48C *)arg0);
            }
        }
    }
}

struct Sub151A09B4 {
    u8 pad0[0x3B];
    u8 unk3B;
};

struct Mid151A09B4 {
    u8 pad0[0x18];
    struct Sub151A09B4 *unk18;
};

struct Obj151A09B4 {
    u8 pad0[0x28];
    struct Mid151A09B4 *unk28;
};

struct Arg151A09B4 {
    struct Sub151A09B4 *unk0;
    u8 unk4;
};

void func_151A09B4(struct Obj151A09B4 *arg0, struct Arg151A09B4 *arg1, u8 arg2) {
    struct Sub151A09B4 *v0 = arg0->unk28->unk18;
    if (arg2 == 0) {
        if (v0 == arg1->unk0 || arg1->unk4 == v0->unk3B) {
            func_151A0928((struct Obj151A0928 *)arg0);
            func_1516972C((struct102 *)arg0);
        }
    }
}

struct Obj151A0A10 {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x1C - 0x18];
    f32 unk1C;
    u8 pad20[0x180 - 0x20];
    f32 unk180;
    s32 unk184;
};

struct Payload151A0A10 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 unkC;
    u8 padD[0x10 - 0xD];
    f32 unk10;
};
void func_151A0A10(struct Obj151A0A10 *arg0, s32 arg1, u8 arg2, s32 arg3);

extern f32 D_800A8D10;
extern f32 D_800BE9A4;
extern f32 func_150ADA68(void);
void func_1514C678(f32, f32, s32, f32, s32, s32, s32, s32, s32, f32, s32, s32);

struct Sub151A0AF8 {
    f32 unk0;
    f32 unk4;
    s32 unk8;
    u8 unkC;
    u8 padD[0x10 - 0xD];
    f32 unk10;
};

struct Obj151A0AF8 {
    u8 pad0[0xC];
    u8 unkC;
    u8 padD[0x28 - 0xD];
    struct Sub151A0AF8 unk28;
};
void func_151A0AF8(struct Obj151A0AF8 *arg0);

void func_151A11CC(f32 arg0, f32 arg1, f32 arg2, f32 arg3,
                  f32 arg4, s32 arg5, s32 arg6, s32 arg7,
                  s32 arg8, s32 arg9, f32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, s32 arg14, s32 arg15,
                  s32 arg16, f32 arg17, s32 arg18, s32 arg19,
                  s32 arg20, s32 arg21, s32 arg22, s32 arg23,
                  s32 arg24, s32 arg25, s32 arg26, s32 arg27);
void func_151A0E40(f32 arg0, f32 arg1, f32 arg2, u8 arg3, s32 arg4);
void func_151A0F28(f32 arg0, f32 arg1, f32 arg2, u8 arg3, s32 arg4);
void func_151A11CC(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, f32 arg10, s32 arg11, s32 arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, f32 arg17, s32 arg18, s32 arg19, s32 arg20, s32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26, s32 arg27);

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
} Sub151A175C;

typedef struct {
    u8 pad0[0x20];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 pad26[0x6];
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 pad2F[0x5];
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8 pad3A[0x5];
    u8 unk3F;
    u8 pad40[0x10];
    Sub151A175C unk50;
} Obj151A175C;
void func_151A175C(Obj151A175C *arg0);

struct Src151A18DC {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    f32 unk34;
    f32 unk38;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    s16 unk44;
};

struct Blk151A18DC {
    s32 a;
    s32 b;
    s32 c;
};

struct Dst151A18DC {
    struct Blk151A18DC unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s32 pad20;
};
void func_151A18DC(struct Src151A18DC *arg0);

struct Sub151A1E34 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
};

struct Obj151A1E34 {
    u8 pad0[0x34];
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8 pad3A[0x3F - 0x3A];
    u8 unk3F;
    u8 pad40[0x50 - 0x40];
    struct Sub151A1E34 unk50;
};
void func_151A1E34(struct Obj151A1E34 *arg0);

struct Src151A1EE8 {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    f32 unk34;
    f32 unk38;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    s16 unk44;
    s16 unk46;
    s16 unk48;
};

struct Blk3151A1EE8 {
    s32 a;
    s32 b;
    s32 c;
};

struct Dst151A1EE8 {
    struct Blk3151A1EE8 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    union {
        struct {
            s16 unk18;
            s16 unk1A;
            s16 unk1C;
            s16 unk1E;
        } s;
        f64 _align;
    } u18;
    s16 unk20;
    s16 unk22;
};
void func_151A1EE8(struct Src151A1EE8 *arg0);

typedef struct Fade151A24A8 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} Fade151A24A8;

typedef struct Obj151A24A8 {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x2F - 0x18];
    u8 unk2F;
    u8 pad30[0x2];
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8 pad3A[0x5];
    u8 unk3F;
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 pad43;
    u16 unk44;
    u8 pad46[0xA];
    Fade151A24A8 unk50;
} Obj151A24A8;
void func_151A24A8(Obj151A24A8 *arg0);

typedef struct Fade151A25E0 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} Fade151A25E0;

typedef struct Obj151A25E0 {
    u8 pad0[0x32];
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8 pad3A[0x5];
    u8 unk3F;
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 pad43[0xD];
    Fade151A25E0 unk50;
} Obj151A25E0;
void func_151A25E0(Obj151A25E0 *arg0);

typedef struct {
    f32 x, y, z;
} Vec151A26EC;

typedef struct {
    s32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
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
    Vec151A26EC unk30;
    Vec151A26EC unk3C;
    Vec151A26EC unk48;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 pad66[0xA];
} Fx151A26EC;

typedef struct {
    f32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s8 unkA;
} Extra151A26EC;
s32 func_151A26EC(Vec151A26EC *arg0, Vec151A26EC *arg1, Vec151A26EC *arg2, f32 arg3, f32 arg4, f32 arg5, s16 arg6, u8 arg7, s16 arg8, s16 arg9, s16 argA, s8 argB, u8 argC, u8 argD, u8 argE, u8 argF, s32 arg10);

struct Sub151A2960 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s8 unk6;
};

struct Obj151A2960 {
    u8 pad0[0x1A];
    s16 unk1A;
    u8 pad1C[0x2C - 0x1C];
    u8 unk2C;
    u8 pad2D[0xB0 - 0x2D];
    struct Sub151A2960 unkB0;
};
void func_151A2960(struct Obj151A2960 *arg0, s32 arg1);

void func_151A2C24(s32, s32, s16, s16, f32, f32, f32, s32, f32, f32, f32, f32, s16, s16, s16, s16, s16, s16, s8, u8, s32, s32);
void func_151A2A14(s32 a0, s16 a1, s16 a2, f32 a3, f32 p4, f32 p5, s32 p6, f32 p7, f32 p8, f32 p9, f32 p10, s16 p11, s16 p12, s16 p13, s16 p14, s16 p15, s16 p16, s8 p17, u8 p18, s32 p19);
void func_151A2AD4(s32 a0, s32 a1, f32 a2, f32 a3, s32 p4, f32 p5, f32 p6, f32 p7, f32 p8, s16 p9, s16 p10, s16 p11, s16 p12, s16 p13, s16 p14, s8 p15, u8 p16, s32 p17);
void func_151A2B84(f32 *in, s32 a1, s32 a2, s32 a3, f32 t, f32 *out);
void func_151A2BD0(f32 *in, s32 a1, s32 a2, f32 a3, f32 t, f32 *out);

typedef struct {
    u8 c[4][3];
} Col151A2C24;
extern Col151A2C24 D_8008F8E8;
void func_151A2C24(s32 arg0, s32 arg1, s16 arg2, s16 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, f32 arg8, f32 arg9, f32 argA, f32 argB, s16 argC, s16 argD, s16 argE, s16 argF, s16 arg10, s16 arg11, s8 arg12, u8 arg13, s32 arg14, s32 arg15);
void func_151A2F0C(void *arg0);

typedef struct {
    /* 0x0 */ f32 a;
    /* 0x4 */ f32 b;
    /* 0x8 */ f32 c;
} Struct3W151A3150;

typedef struct Obj151A3150 {
    u8 pad0[0x1];
    u8 unk1;
    u8 pad2[0xC - 0x2];
    u8 unkC;
    u8 padD[0x38 - 0xD];
    f32 unk38;
    f32 unk3C;
    Struct3W151A3150 unk40;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    u8 pad58[0x68 - 0x58];
    s32 unk68;
} Obj151A3150;

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ Struct3W151A3150 unk08;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
    /* 0x4A */ u8 unk4A;
    /* 0x4B */ u8 unk4B;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ u8 unk4E;
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ s8 unk70;
    /* 0x71 */ s8 unk71;
    /* 0x72 */ s8 unk72;
    /* 0x73 */ s8 unk73;
} Struct151A3150;
void func_151A3150(Obj151A3150 *arg0);
