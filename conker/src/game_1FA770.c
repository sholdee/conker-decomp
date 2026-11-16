/* functions.h types func_1513D2F0 arg1 as s32; func_151CE6D0 needs a pointer arg1 (an int-typed
 * address changes which loop invariants IDO keeps in saved registers). */
#define func_1513D2F0 func_1513D2F0_s32_decl_in_functions_h
#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#undef func_1513D2F0

void *func_151CD4C0();
void func_151CE47C();

struct Vec3w151CEC10;

extern f32 D_800AB020;
extern f32 D_800AB024;
extern f32 D_800AB008;
extern f32 D_800D9860;
extern f32 func_15144AA8(s32);
extern struct17 *func_15144B34(s32);
extern s32 func_15046C80(f32 *, s32, f32, f32 *);
extern struct260 *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
s32 func_151CEC10(struct Vec3w151CEC10 *, f32 *, void *);

typedef struct {
    char pad_0x0[0x48];
    s32 field_0x48;
} Func151CE4DCTemp;

typedef struct {
    char pad_0x0[0x98];
    s32 field_0x98;
} Func151CE4DCArg0;

typedef struct {
    char pad_0x0[0x30];
    s32 field_0x30;
} ConkerField30Object;

typedef struct {
    ConkerField30Object *field_0x0;
} ConkerField0Slot;

typedef struct {
    char pad_0x0[0x98];
    ConkerField0Slot *field_0x98;
} ConkerField98Owner;


struct260 *func_151CD2C0(void *arg0, u8 arg1, s32 arg2) {
    struct260 *temp_v0;
    struct {
        void *unk0;
        s32 unk4;
        s32 unk8;
        u8 unkC;
        u8 padD;
        u8 padE;
        u8 padF;
    } sp34;

    sp34.unk0 = arg0;
    sp34.unk4 = *(s32 *)((s32)arg0 + 0x18);
    sp34.unk8 = 0;
    sp34.unkC = *(s32 *)((s32)arg0 + 0x1C);

    temp_v0 = func_15149130(0x12C, -1, -1, -1, 0, 0x1E, (struct37 *)0x10, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0x10);
    }
    return temp_v0;
}

void func_151CD35C(s32 arg0) {
    s32 sp1C;

    if (arg0 >= 0 && arg0 < 4) {
        sp1C = arg0;
        func_151494E0((s32)&sp1C, 0x17);
    }
}

void func_151CD394(s32 arg0) {
    s32 sp1C;

    if (arg0 >= 0 && arg0 < 4) {
        sp1C = arg0;
        func_151494E0((s32)&sp1C, 0x18);
    }
}

typedef struct {
    void *unk0;
    s32 unk4;
    void *unk8;
    u8 unkC;
} Sub151CD3CC;

void func_151CD3CC(struct260 *arg0, s32 arg1, u8 arg2) {
    Sub151CD3CC *sub;
    /* Reserves 12 bytes at the top of the -g3 stack frame that the shipped
     * build allocated but never referenced. */
    s32 unused[3];

    sub = (Sub151CD3CC *)((s32)arg0 + 0x28);
    if (arg2 == 0x17) {
        if (sub->unk4 == *(s32 *)arg1) {
            if (sub->unk8 == NULL) {
                sub->unk8 = func_151CD4C0(sub->unk0, sub->unkC, sub->unk4, arg0->pad0[0xC], arg0->pad0[1]);
                *(u8 *)((s32)sub->unk0 + 0x14) = 0;
            dummy_label_151CD3CC_1: ;
            }
        }
    } else if (arg2 == 0x18) {
        if (sub->unk4 == *(s32 *)arg1) {
            if (sub->unk8 != NULL) {
                func_151CE47C(sub->unk8);
            }
        }
    } else if (arg2 == 0x23) {
        if (sub->unk4 == *(s32 *)arg1) {
            sub->unk8 = NULL;
            *(u8 *)((s32)sub->unk0 + 0x14) = 1;
        }
    }
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} Arg151CD4C0;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
} Hdr151CD4C0;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    s32 unk48;
    u8 unk4C;
    u8 pad4D[3];
} Payload151CD4C0;

void *func_151CD4C0(arg0, arg1, arg2, arg3, arg4)
    Arg151CD4C0 *arg0;
    u8 arg1;
    s32 arg2;
    u8 arg3;
    s32 arg4;
{
    struct260 *temp_v0;
    Hdr151CD4C0 header;
    Payload151CD4C0 payload;
    f32 size;

    size = arg0->unk6;
    header.unk15 = 0x23;
    header.unk0 = arg0->unk0;
    header.unk4 = arg0->unk2 + arg0->unk8;
    header.unk8 = arg0->unk4;
    header.unkC = 0x12C;
    header.unkE = 0x34;
    header.unk10 = 0xB;
    payload.unk44 = arg0->unk2;
    payload.unk0 = size * 0.125f;
    payload.unk10 = 0.03000000119f;
    payload.unk8 = 0.0f;
    payload.unkC = 0.0f;
    payload.unk4 = size * 0.125f;
    payload.unk14 = 0.04000000283f;
    payload.unk18 = header.unk0;
    payload.unk1C = header.unk8;
    payload.unk24 = size * 0.875f;
    payload.unk34 = payload.unk24;
    payload.unk28 = size * 0.0625f;
    payload.unk2C = 0.0f;
    payload.unk30 = 0.05500000343f;
    payload.unk40 = 0.0f;
    payload.unk48 = arg2;
    payload.unk38 = -16384.0f;
    payload.unk3C = -16384.0f;
    payload.unk20 = 0.0f;
    payload.unk4C = arg1;
    header.unk18 = 0;
    header.unk14 = 7;
    temp_v0 = func_15147A80(&header, 0x50, 0x1C, 9, 9, 9, 0, 0, 0, arg3, arg4);
    if (temp_v0 != NULL) {
        memcpy(*(void **)((u8 *)temp_v0 + 0x98), &payload, 0x50);
    }
    return temp_v0;
}

typedef struct Game1FA770Particle {
    f32 x;
    f32 y;
    u8 pad08[4];
    f32 velocity;
    u8 pad10[0xC];
} Game1FA770Particle;

typedef struct Vec3i151CD674 {
    s32 x;
    s32 y;
    s32 z;
} Vec3i151CD674;

s32 func_151CD674(u8 *actor) {
    u8 *owner;
    Game1FA770Particle *particles;
    Game1FA770Particle *particle;
    s32 cursor;
    f32 velocity;
    f32 gravity;
    f32 acceleration;
    f32 step;
    f32 travel;
    f32 squared;

    owner = *(u8 **)(actor + 0x98);
    particles = *(Game1FA770Particle **)(actor + 0x94);
    if ((*(s8 *)(actor + 0x2C) < 2) && (*(u16 *)(actor + 0x1E) & 8)) {
        return 0;
    }
    cursor = *(s8 *)(actor + 0x2E);
    if (cursor != *(s8 *)(actor + 0x2D)) {
        gravity = -0.25500003f;
        acceleration = -0.12750001f;
        do {
            cursor--;
            if (cursor < 0) {
                cursor = actor[0x25] - 1;
            }
            particle = &particles[cursor];
            velocity = particle->velocity;
            step = D_800BE9A4;
            travel = velocity * step;
            squared = step * step;
            particle->y = particle->y + (travel + acceleration * squared);
            particle->velocity = (f32)velocity + gravity * D_800BE9A4;
            if (particle->y < *(f32 *)(owner + 0x44)) {
                if (cursor != *(s8 *)(actor + 0x2D)) {
                    do {
                        *(s8 *)(actor + 0x2D) += 1;
                        if (actor[0x25] == *(s8 *)(actor + 0x2D)) {
                            *(s8 *)(actor + 0x2D) = 0;
                        }
                        *(s8 *)(actor + 0x2C) -= 1;
                    } while (cursor != *(s8 *)(actor + 0x2D));
                }
                particles[*(s8 *)(actor + 0x2D)].y = *(f32 *)(owner + 0x44);
            }
        } while (cursor != *(s8 *)(actor + 0x2D));
    }
    *(Vec3i151CD674 *)(actor + 0x54) = *(Vec3i151CD674 *)(actor + 0x10);
    return 1;
}

typedef struct Vec2f151CD7BC { f32 x; f32 z; } Vec2f151CD7BC;

typedef struct Owner151CD7BC {
    f32 amp0;
    f32 amp1;
    f32 phase0;
    f32 phase1;
    f32 rate0;
    f32 rate1;
    Vec2f151CD7BC pos;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 phase2;
    f32 rate2;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
} Owner151CD7BC;

typedef struct Particle151CD7BC {
    f32 x;
    f32 y;
    f32 z;
    f32 vel;
    f32 unk10;
    s16 alpha;
    f32 unk18;
} Particle151CD7BC;

s32 func_151CD7BC(u8 *actor) {
    Owner151CD7BC *owner;
    Particle151CD7BC *particles;
    Particle151CD7BC *p;
    f32 h;
    f32 dh;
    f32 dz;
    f32 dx;
    Vec2f151CD7BC pos;
    f32 inv;
    f32 a;
    f32 da;
    f32 b;
    f32 vel;
    f32 db;
    f32 gravity;
    f32 acceleration;
    s32 tail;

    owner = *(Owner151CD7BC **)(actor + 0x98);
    particles = *(Particle151CD7BC **)(actor + 0x94);
    owner->phase0 += owner->rate0 * D_800BE9A4;
    owner->phase1 += owner->rate1 * D_800BE9A4;
    owner->phase0 = func_15144B68(owner->phase0);
    owner->phase1 = func_15144B68(owner->phase1);
    owner->phase2 += owner->rate2 * D_800BE9A4;
    owner->phase2 = func_15144B68(owner->phase2);
    owner->unk40 += 0.2730129957f * D_800BE9A4;
    owner->unk3C += (10220.0f + func_150ADA68() * 9434.0f) * 0.009999999776f * D_800BE9A4;
    while (owner->unk3C >= 16384.0f) {
        owner->unk3C -= 16384.0f;
    }
    if (owner->unk40 > 1.0f) {
        inv = 1.0f / owner->unk40;
        h = owner->unk20 + D_800BE9A4;
        dh = -(h * inv);
        pos = owner->pos;
        dx = (*(f32 *)(actor + 0x10) + (sinf(owner->phase0) * owner->amp0)) - owner->pos.x;
        dz = (*(f32 *)(actor + 0x18) + (sinf(owner->phase1) * owner->amp1)) - owner->pos.z;
        da = (owner->unk24 + (sinf(owner->phase2) * owner->unk28)) - owner->unk34;
        db = owner->unk3C - owner->unk38;
        a = owner->unk34;
        b = owner->unk3C;
        gravity = -0.255000025f;
        acceleration = -0.1275000125f;
        vel = -3.772000074f;
        while (owner->unk40 > 1.0f) {
            p = &particles[*(s8 *)(actor + 0x2E)];
            p->x = pos.x;
            p->y = *(f32 *)(actor + 0x14);
            p->z = pos.z;
            p->vel = vel;
            p->unk10 = a;
            p->alpha = 0xFF;
            p->unk18 = b;
            p->y = p->y + ((vel * D_800BE9A4) + (acceleration * (D_800BE9A4 * D_800BE9A4)));
            p->vel = (f32)vel + gravity * D_800BE9A4;
            *(s8 *)(actor + 0x2E) += 1;
            if (actor[0x25] == *(s8 *)(actor + 0x2E)) {
                *(s8 *)(actor + 0x2E) = 0;
            }
            tail = *(s8 *)(actor + 0x2D);
            *(s8 *)(actor + 0x2C) += 1;
            if (tail == *(s8 *)(actor + 0x2E)) {
                *(s8 *)(actor + 0x2D) = tail + 1;
                if (actor[0x25] == *(s8 *)(actor + 0x2D)) {
                    *(s8 *)(actor + 0x2D) = 0;
                }
                *(s8 *)(actor + 0x2C) -= 1;
            }
            pos.x += dx * inv;
            pos.z += dz * inv;
            a += da * inv;
            b += db * inv;
            h += dh;
            owner->unk40 -= 1.0f;
        }
        owner->pos = pos;
        owner->unk34 = a;
        owner->unk38 = b;
        owner->unk20 = h;
    }
    return 1;
}

typedef struct Game1FA770Trail {
    f32 x;
    f32 y;
    u8 pad08[0xC];
    s16 alpha;
    u8 pad16[0x6];
} Game1FA770Trail;

s32 func_151CDB94(u8 *actor) {
    Game1FA770Trail *trail;
    Game1FA770Trail *start;
    Game1FA770Trail *cur;
    s32 i;
    f32 range;
    f32 inv;
    f32 d;
    f32 fade;
    f32 k;

    trail = *(Game1FA770Trail **)(actor + 0x94);
    if (*(s8 *)(actor + 0x2C) <= 0) {
        return 1;
    }
    i = *(s8 *)(actor + 0x2E) - 1;
    if (i < 0) {
        i = actor[0x25] - 1;
    }
    start = &trail[*(s8 *)(actor + 0x2D)];
    cur = &trail[i];
    range = fabsf(cur->y - start->y);
    if (range == 0.0f) {
        return 1;
    }
    inv = 1.0f / range;
    fade = inv * 10.0f;
    i = *(s8 *)(actor + 0x2D);
    do {
        cur = &trail[i];
        d = cur->y - start->y;
        cur->alpha = 0xFF;
        if (d < range * 0.1970000118f) {
            k = inv * 5.076141834f;
            cur->alpha = (u8)(d * k * 255.0f);
        } else if (range - range * 0.1000000015f < d) {
            cur->alpha = (u8)((range - d) * fade * 255.0f);
        }
        i++;
        if (i >= actor[0x25]) {
            i = 0;
        }
    } while (i != *(s8 *)(actor + 0x2E));
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151CDE20.s")

typedef struct {
    char pad0[0x1E];
    u16 unk1E;
    char pad20[0x10];
    u8 unk30;
} ConkerStruct151CE47C;

void func_151CE47C(ConkerStruct151CE47C *arg0) {
    arg0->unk30 = 0;
    arg0->unk1E &= ~0x2;
    arg0->unk1E |= 0x8;
}

extern void func_151478F4(void *);

void func_151CE49C(Func151CE4DCArg0 *arg0) {
    s32 sp1C;
    Func151CE4DCTemp *temp;

    temp = (Func151CE4DCTemp *)arg0->field_0x98;
    sp1C = temp->field_0x48;
    func_151494E0((s32)&sp1C, 0x23);
    func_151478F4((void *)arg0);
}

extern void func_15147928(s32);

void func_151CE4DC(Func151CE4DCArg0 *arg0) {
    s32 sp1C;
    Func151CE4DCTemp *temp;

    temp = (Func151CE4DCTemp *)arg0->field_0x98;
    sp1C = temp->field_0x48;
    func_151494E0((s32)&sp1C, 0x23);
    func_15147928((s32)arg0);
}

struct Obj151CE634 *func_151CE634(s32);
void func_1515C1A0(void *, void *, f32 *, f32 *);
s32 func_1505D024(void *, s32, u16, s32);

typedef struct {
    f32 position[3];
    u8 unkC[0x10];
} Game1FA770TrailPoint;

void func_151CE51C(void *arg0, void *arg1) {
    f32 position[3];
    f32 radius;
    f32 height;
    u8 *obj;

    obj = (u8 *)func_151CE634(*(s32 *)((u8 *)arg1 + 0x18));
    if (obj != 0) {
        func_1515C1A0(arg0, position, &radius, &height);
        if (*(u16 *)(obj + 0x1E) & 8) {
            f32 bottom;
            f32 level;

            bottom = position[1] - height;
            if (*(s8 *)(obj + 0x2C) > 0) {
                s32 index;
                Game1FA770TrailPoint *points;

                index = *(s8 *)(obj + 0x2E) - 1;
                if (index < 0) {
                    index = obj[0x25] - 1;
                }
                points = *(Game1FA770TrailPoint **)(obj + 0x94);
                level = points[index].position[1];
            } else {
                return;
            }
            if (bottom <= level) {
                func_1505D024(arg0, 0x60019, 0, -1);
            }
        } else {
            f32 top;
            f32 level;

            top = position[1] + height;
            if (*(s8 *)(obj + 0x2C) > 0) {
                Game1FA770TrailPoint *points;

                points = *(Game1FA770TrailPoint **)(obj + 0x94);
                level = points[*(s8 *)(obj + 0x2D)].position[1];
            } else {
                return;
            }
            if (level <= top) {
                func_1505D024(arg0, 0x60019, 0, -1);
            }
        }
    }
}

typedef struct Obj151CE634Sub {
    u8 pad0[0x48];
    s32 unk48;
} Obj151CE634Sub;

typedef struct Obj151CE634 {
    u8 pad0[8];
    struct Obj151CE634 *next;
    u8 padC[0x20 - 0xC];
    s32 unk20;
    u8 pad24[0x98 - 0x24];
    Obj151CE634Sub *unk98;
} Obj151CE634;

extern s32 D_800A5760[];
extern Obj151CE634 *D_800DCE50[][0x68];

Obj151CE634 *func_151CE634(s32 arg0) {
    u8 i;
    s32 j;
    Obj151CE634 *p;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            p = D_800DCE50[j][D_800A5760[i]];
            while (p != NULL) {
                if (p->unk20 == 0xB) {
                    Obj151CE634Sub *sub = p->unk98;

                    if (sub->unk48 == arg0) {
                        return p;
                    }
                }
                p = p->next;
            }
        }
    }
    return NULL;
}


typedef struct Game1FBB80Owner {
    s16 field00;
    s16 mask;
} Game1FBB80Owner;

typedef struct Game1FBB80Effect {
    void *shape;
    f32 rate;
    f32 accumulator;
    Game1FBB80Owner *owner;
    f32 height;
} Game1FBB80Effect;

typedef struct Game1FBB80Object {
    u8 pad00, flags01;
    u8 pad02[0xA];
    u8 flags0C;
    u8 pad0D[0x1B];
    Game1FBB80Effect effect;
} Game1FBB80Object;

typedef struct Game1FBB80Packet {
    u8 field00, field01;
    s16 field02, field04;
    u8 pad06[2];
    s32 field08, field0C;
    u8 field10, field11, field12, field13;
    f32 field14, field18;
    f32 x, y, z;
    f32 field28, field2C, field30, field34, field38, field3C;
    u32 flags40;
    u8 field44, field45, field46, field47;
    Game1FBB80Owner *owner;
    u8 pad4C[0xC]; /* func_1513D2F0 copies the complete 0x58-byte packet. */
} Game1FBB80Packet;

s32 func_1504530C(void *, f32, void *);
void *func_1513D2F0(void *, void *, u8, u8, u8, u8, u8, s32, s32, s32, u8, s32);
void func_151432BC(void *, f32 *, f32 *, f32 *, f32 *);
void func_151CE6D0(Game1FBB80Object *arg0);
s32 func_151CEA20(f32 *arg0);
struct260 *func_151CEAAC(void *arg0, void *arg1, u8 arg2, u8 arg3, s32 arg4);

struct Vec3w151CEC10 { s32 x, y, z; };
s32 func_151CEC10(struct Vec3w151CEC10 *arg0, f32 *arg1, void *arg2);

extern f32 D_800AB00C;
extern f32 D_800AB010;
f32 func_15143E64(struct17 *);

typedef struct { f32 x, y, z; } TrailVec3;
typedef struct { f32 x, z; } TrailVec2;
typedef struct { f32 w[4]; } TrailQuad;

typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
} TrailSub;

typedef struct {
    TrailSub *sub;
    u8 unk4;
    u8 pad5[3];
    void *unk8;
    TrailVec2 scaled;
    TrailVec2 normal;
    u8 flags;
} TrailInfo;

typedef struct {
    TrailVec3 pos;
    f32 dist;
    f32 invLen;
    f32 unk14;
    TrailVec2 scaled;
    TrailVec2 normal;
} TrailEntry;

typedef struct {
    u8 pad0[0x10];
    TrailVec3 pos;
    u8 pad1C[2];
    u16 flags;
    u8 pad20[5];
    u8 count;
    u8 pad26[6];
    s8 num;
    s8 tail;
    s8 head;
    u8 pad2F[0x65];
    TrailEntry *entries;
    TrailInfo *info;
} TrailObj;
s32 func_151CEC54(TrailObj *arg0);

typedef struct {
    f32 x, y, z, distance;
    u8 pad10[4];
    f32 wave;
    u8 pad18[0x10];
} Game1FC5D0Point;

typedef struct {
    u8 pad00[0x20];
    f32 limit, phase;
} Game1FC5D0Config;

typedef struct {
    u8 pad00[0x25];
    u8 capacity;
    u8 pad26[6];
    s8 count;
    s8 head;
    s8 tail;
    u8 pad2F[0x65];
    Game1FC5D0Point *points;
    Game1FC5D0Config *config;
} Game1FC5D0Actor;
s32 func_151CF120(Game1FC5D0Actor *arg0);

struct s151CF844 { s32 pad[0x26]; s32 *unk98p; };
void func_151CF844(struct s151CF844 *arg0, s32 arg1, u8 arg2);

typedef struct {
    char pad0[0x23D];
    u8 unk23D;
} ConkerStruct151CF898Camera;

typedef struct {
    struct17 vec;
    f32 unkC;
} ConkerStruct151CF898Payload;
void func_151CF898(struct127 *arg0, f32 arg1, f32 arg2);
void func_151D0024(struct127 *arg0);
void func_151D0058(struct127 *arg0, u8 arg1, u8 arg2, s32 arg3);

typedef struct {
    char pad84[0x84];
    u16 unk84;
} ConkerStruct151D0128b;

typedef struct {
    char padE[0xE];
    s16 unkE;
    char pad10[0x18];
    ConkerStruct151D0128b *unk28;
    char pad2C[0x2];
    u16 unk2E;
} ConkerStruct151D0128;
void func_151D0128(ConkerStruct151D0128 *arg0);

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA151D08F0;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB151D08F0;
void func_151D08F0(struct260 *arg0, s32 arg1, u8 arg2);

typedef struct {
    u8 unk0;
    struct17 unk4;
    struct17 unk10;
    struct17 unk1C;
    f32 unk28;
} ShapeA151D09A8; /* 0x2C */

typedef struct {
    u8 unk0[0x24];
} Dst151D09A8;

typedef struct {
    struct17 unk0;
    s16 unkC;
    s16 unkE;
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 unk25;
    u8 pad26[2];
    f32 unk28;
    f32 unk2C;
    u8 unk30;
    u8 unk31;
    u8 pad32[2];
    f32 unk34;
    u8 unk38;
    u8 pad39[3];
    f32 unk3C;
} EmitA151D09A8; /* 0x40 */

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
} EmitB151D09A8; /* 0x44 */

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
} Particle151D09A8; /* 0x70 */

extern f32 D_800AAF84[3];
extern f32 D_800AAF90[3];
extern void func_15145EA4(s32 *, s32 *, s32, s32);
extern void func_1504715C(void *, struct127 *);
extern void func_1514FF44(ShapeA151D09A8 *, EmitA151D09A8 *, Dst151D09A8 *, u8, s32);
extern void func_15153CCC(ShapeA151D09A8 *, EmitB151D09A8 *, Dst151D09A8 *, u8, s32);
extern s32 func_1514F6E8(ShapeA151D09A8 *);
extern void func_1514F808(ShapeA151D09A8 *, f32, struct17 *);
extern void *func_15130280(void *, u8, s32, s32, u8, s32);
void func_151D09A8(struct127 *arg0, u8 arg1, s32 arg2);
void func_151D0ED8(void *arg0);

void func_151D0ED8(void *arg0);

void func_1513173C(void *arg0);
void func_151D0F08(void *arg0);

void func_1513175C(void *arg0);
void func_151D0F34(void *arg0);

extern s32 (*D_8008FC30[])(struct127 *, void *);

struct vec3_151D0F60 { f32 x; f32 y; f32 z; };
void func_151D0F60(struct127 *arg0, u8 arg1, u8 arg2, s32 arg3);
s32 func_151D1074(f32 *arg0, f32 *arg1);

s32 func_151D10E4(void *arg0, s32 arg1, u8 arg2);
s32 func_151D10A4(void *arg0, s32 arg1);
s32 func_151D10C4(void *arg0, s32 arg1);

typedef struct {
    struct127 *unk0;
    u8 unk4;
    u8 pad5[3];
    s32 unk8;
    struct17 unkC;
    struct17 unk18;
    struct17 unk24;
    f32 unk30;
    u8 unk34;
} Sub151D1138;

extern f32 func_15143E64(struct17 *);
extern s32 func_1510F8CC(s32);
void func_151D13E0(struct260 *arg0);
void func_151D1448();
void func_151D1138(struct260 *arg0);
void func_151D1328(s32 arg0, s32 arg1, u8 arg2);

void func_151D13E0(struct260 *arg0);
void func_151D1368(struct260 *arg0);
void func_151D1388(struct260 *arg0);

void func_15149368(struct260 *arg0);
void func_151D13B4(struct260 *arg0);

struct Obj151D13E0 {
    char pad0[0x1C];
    s16 unk1C;
    u16 unk1E;
    char pad20[0x10];
    u8 unk30;
    char pad31[0x67];
    s32 *unk98;
};

struct Slot151D13E0 {
    char pad0[0x8];
    struct Obj151D13E0 *unk8;
};
void func_151D13E0(struct260 *arg0);
void func_151D1448(u8 *arg0);

typedef struct {
    f32 x, y, z;
} Vec3_151D15D0;

typedef struct {
    f32 x, z;
} Vec2_151D15D0;

typedef struct {
    Vec3_151D15D0 pos;
    f32 speed;
    f32 invLen;
    u8 alpha;
    u8 pad15[3];
    f32 unk18;
    f32 unk1C;
    Vec2_151D15D0 n;
} Rec_151D15D0;

typedef struct {
    u8 pad0[0x18];
    Vec3_151D15D0 pos;
    f32 vx;
    f32 vy;
    f32 vz;
    f32 speed;
} Inner_151D15D0;

typedef struct {
    u8 pad0[0x28];
    Inner_151D15D0 inner;
} Base_151D15D0;

typedef struct {
    Base_151D15D0 *base;
    f32 angle;
} Src_151D15D0;

typedef struct {
    u8 pad0[0x10];
    Vec3_151D15D0 pos;
    u8 pad1C[2];
    u16 flags;
    u8 pad20[5];
    u8 count;
    u8 pad26[6];
    s8 n;
    s8 tail;
    s8 head;
    u8 pad2F[0x94 - 0x2F];
    Rec_151D15D0 *recs;
    Src_151D15D0 *src;
} Obj_151D15D0;

extern f32 func_15144528(f32, f32, f32);
s32 func_151D15D0(Obj_151D15D0 *arg0);
void func_151D223C(ConkerField98Owner *arg0);

extern void func_151478F4(void *);
void func_151D2258(s32 arg0);

extern void func_15147928(s32);
void func_151D2284(s32 arg0);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3_151D22B0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ Vec3_151D22B0 pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s8 unk40;
} Cfg080C_151D22B0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ Vec3_151D22B0 pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
} Cfg2190_151D22B0;

typedef struct {
    /* 0x00 */ Vec3_151D22B0 pos;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s8 unk38;
    /* 0x39 */ s8 unk39;
    /* 0x3A */ u8 unk3A;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s8 unk7C;
    /* 0x7D */ s8 unk7D;
    /* 0x7E */ s16 pad7E;
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s16 unk82;
    /* 0x84 */ s32 pad84;
} Cfg1A38_151D22B0;

extern s32 D_800AB0B0[];
extern f32 D_800AB0E0[];
extern s32 D_800AB110[];
extern f32 D_800AB114[];

void func_1515C244(struct127 *, f32 *, f32 *, f32 *);
void func_1504715C(void *, struct127 *);
void func_1515080C(Cfg080C_151D22B0 *, s32 *, f32 *, s32, s32, s32, s32, s32, s32, void *, f32, s32, s32, u8, s32);
void func_15152190(Cfg2190_151D22B0 *, s32 *, f32 *, s32, f32, s32, u8, s32);
void func_15151A38(Cfg1A38_151D22B0 *, u8, s32);
void func_151D22B0(struct127 *arg0, u8 arg1, s32 arg2);
