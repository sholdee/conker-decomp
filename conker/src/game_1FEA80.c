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
struct260 *func_151CD2C0(void *arg0, u8 arg1, s32 arg2);
void func_151CD35C(s32 arg0);
void func_151CD394(s32 arg0);

typedef struct {
    void *unk0;
    s32 unk4;
    void *unk8;
    u8 unkC;
} Sub151CD3CC;
void func_151CD3CC(struct260 *arg0, s32 arg1, u8 arg2);

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
s32 func_151CD674(u8 *actor);

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
s32 func_151CD7BC(u8 *actor);

typedef struct Game1FA770Trail {
    f32 x;
    f32 y;
    u8 pad08[0xC];
    s16 alpha;
    u8 pad16[0x6];
} Game1FA770Trail;
s32 func_151CDB94(u8 *actor);

typedef struct {
    char pad0[0x1E];
    u16 unk1E;
    char pad20[0x10];
    u8 unk30;
} ConkerStruct151CE47C;
void func_151CE47C(ConkerStruct151CE47C *arg0);

extern void func_151478F4(void *);
void func_151CE49C(Func151CE4DCArg0 *arg0);

extern void func_15147928(s32);
void func_151CE4DC(Func151CE4DCArg0 *arg0);

struct Obj151CE634 *func_151CE634(s32);
void func_1515C1A0(void *, void *, f32 *, f32 *);
s32 func_1505D024(void *, s32, u16, s32);

typedef struct {
    f32 position[3];
    u8 unkC[0x10];
} Game1FA770TrailPoint;
void func_151CE51C(void *arg0, void *arg1);

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
Obj151CE634 *func_151CE634(s32 arg0);


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

s32 func_151D15D0(Obj_151D15D0 *arg0) {
    Src_151D15D0 *src;
    Rec_151D15D0 *recs;
    Base_151D15D0 *base;
    Inner_151D15D0 *inner;

    src = arg0->src;
    recs = arg0->recs;
    base = src->base;
    inner = &base->inner;
    if (D_800BE9E4 > 0) {
        arg0->pos = inner->pos;
        arg0->flags |= 4;
        {
        Rec_151D15D0 *rec;
        f32 len;

        rec = &recs[arg0->head];
        rec->pos = arg0->pos;
        rec->speed = inner->speed;
        len = sqrtf(inner->vx * inner->vx + inner->vz * inner->vz);
        if (len == 0.0f) {
            rec->invLen = 1.0f;
        } else {
            rec->invLen = 1.0f / len;
        }
        rec->unk1C = 100.0f;
        rec->unk18 = src->angle;
        rec->alpha = 0xFF;
        src->angle = src->angle + rec->speed * 21.85000038f;
        src->angle = func_15144528(src->angle, 8192.0f, 0.0f);
        if (len) {
            rec->n.x = -inner->vz * rec->invLen;
            rec->n.z = inner->vx * rec->invLen;
        } else {
            rec->n.x = 0.0f;
            rec->n.z = 0.0f;
        }
        arg0->head++;
        if (arg0->head == arg0->count) {
            arg0->head = 0;
        }
        arg0->n++;
        if (arg0->head == arg0->tail) {
            arg0->tail++;
            if (arg0->tail == arg0->count) {
                arg0->tail = 0;
            }
            arg0->n--;
        }
        if (arg0->n >= 3) {
            s16 i0;
            s16 i1;
            s16 i2;

            i1 = arg0->head - 1;
            if (i1 < 0) {
                i1 = arg0->count - 1;
            }
            i1 = i1 - 1;
            if (i1 < 0) {
                i1 = arg0->count - 1;
            }
            i2 = i1 - 1;
            if (i2 < 0) {
                i2 = arg0->count - 1;
            }
            if (rec->speed) {
                f32 dx0;
                f32 dz0;
                f32 inv1;
                Vec2_151D15D0 d1;
                Rec_151D15D0 *p1;

                p1 = &recs[i1];
                if (p1->speed) {
                    Rec_151D15D0 *p2;
                    Vec2_151D15D0 nrm;
                    f32 lenSq;

                    p2 = &recs[i2];
                    inv1 = p1->invLen;
                    dx0 = (rec->pos.x - p1->pos.x) * rec->invLen;
                    dz0 = (rec->pos.z - p1->pos.z) * rec->invLen;
                    d1.x = (p2->pos.x - p1->pos.x) * inv1;
                    d1.z = (p2->pos.z - p1->pos.z) * inv1;
                    nrm.x = -(dz0 - d1.z);
                    nrm.z = dx0;
                    nrm.z -= d1.x;
                    lenSq = nrm.x * nrm.x + nrm.z * nrm.z;
                    if (lenSq > 9.999999747e-05f) {
                        f32 inv;

                        inv = 1.0f / sqrtf(lenSq);
                        nrm.x *= inv;
                        nrm.z *= inv;
                        p1->n = nrm;
                    } else {
                        p1->n.x = 0.0f;
                        p1->n.z = 0.0f;
                    }
                }
            }
        }
        }
    }
    return 1;
}

typedef struct {
    u8 pad0[0x14C];
    f32 unk14C;
} Inner_151D197C;

typedef struct {
    u8 pad0[0x28];
    Inner_151D197C *unk28;
} Base_151D197C;

typedef struct {
    Base_151D197C *base;
    u8 pad4[4];
    s8 unk8;
    u8 pad9[3];
    f32 maxLen;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} Src_151D197C;

typedef struct {
    u8 pad0[0x1C];
    s16 unk1C;
    u16 flags;
    u8 pad20[5];
    u8 count;
    u8 pad26[6];
    s8 n;
    s8 tail;
    s8 head;
    u8 pad2F[0x94 - 0x2F];
    Rec_151D15D0 *recs;
    Src_151D197C *src;
} Obj_151D197C;

s32 func_151D197C(Obj_151D197C *arg0) {
    Src_151D197C *src;
    Rec_151D15D0 *recs;
    f32 total;
    f32 inv;
    s32 i;
    s32 prev;
    Rec_151D15D0 *p;
    Rec_151D15D0 *q;
    f32 t;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 qx;
    f32 qy;
    f32 qz;

    src = arg0->src;
    recs = arg0->recs;
    if (src->base != NULL) {
        Inner_151D197C *inner = src->base->unk28;
        src->unk1C = inner->unk14C;
    }
    if (arg0->n >= 2) {
        total = 0;
        i = arg0->head - 1;
        if (i < 0) {
            i = arg0->count - 1;
        }
        while (i != arg0->tail) {
            prev = i;
            i--;
            if (i < 0) {
                i = arg0->count - 1;
            }
            p = &recs[prev];
            total += p->speed;
            if (src->maxLen < total) {
                if (p->speed != 0.0f) {
                    t = (total - src->maxLen) / p->speed;
                    q = &recs[i];
                    qx = q->pos.x;
                    qy = q->pos.y;
                    qz = q->pos.z;
                    dx = qx - p->pos.x;
                    dy = qy - p->pos.y;
                    dz = qz - p->pos.z;
                    q->pos.x = qx - dx * t;
                    q->pos.y = qy - dy * t;
                    q->pos.z = qz - dz * t;
                    p->speed = p->speed * (1.0f - t);
                }
                while (i != arg0->tail) {
                    arg0->tail++;
                    if (arg0->tail == arg0->count) {
                        arg0->tail = 0;
                    }
                    arg0->n--;
                }
                total = src->maxLen;
            }
        }
        inv = (total != 0.0f) ? 1.0f / total : 0;
        ;
    }
    if (arg0->n >= 2) {
        f32 a;
        f32 b;
        f32 d;
        s32 j;

        j = arg0->head;
        d = total;
        a = src->unk14 * src->unk1C * inv;
        b = src->unk18 * inv;
        do {
            j--;
            if (j < 0) {
                j = arg0->count - 1;
            }
            {
            Rec_151D15D0 *r = &recs[j];
            r->unk1C = src->unk10 * src->unk1C + d * a;
            r->alpha = d * b;
            d -= r->speed;
            }
        } while (j != arg0->tail);
    }
    if ((arg0->flags & 1) && (arg0->flags & 8)) {
        src->unk8 = arg0->unk1C * 6;
    }
    return 1;
}

typedef struct {
    f32 x, y, z;
} Vec3f_151D1C98;

typedef struct Game1FEA80RibbonPoint {
    Vec3f_151D1C98 position;
    u8 pad0C[8];
    u8 alpha;
    u8 pad15[3];
    f32 texture;
    f32 width;
    f32 directionX;
    f32 directionZ;
} Game1FEA80RibbonPoint;

typedef struct Game1FEA80RibbonVertex {
    s16 x, y, z;
    s16 flag;
    s16 s, t;
    u8 r, g, b, a;
} Game1FEA80RibbonVertex;

typedef struct Game1FEA80RibbonState {
    u8 pad0[8];
    u8 alpha;
} Game1FEA80RibbonState;

void *memcpy(void *, const void *, u32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
s32 func_15142E24(s32, s32, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
extern u8 D_80091430[];
extern s32 D_800A4AC8[][2];
extern s32 D_800D2C9C;

Gfx *func_151D1C98(u8 *arg0, Gfx *arg1, s16 arg2) {
    Game1FEA80RibbonState *state;
    Vec3f_151D1C98 next;
    Vec3f_151D1C98 first;
    Game1FEA80RibbonPoint *points;
    s32 index;
    s32 previous;
    u8 flag;
    u8 nextAlpha;
    f32 firstX;
    f32 firstZ;
    f32 nextX;
    f32 nextZ;
    f32 firstTexture;
    f32 nextTexture;
    u16 firstAlpha;
    Game1FEA80RibbonVertex *vtx;

    if (*(s8 *)(arg0 + 0x2C) < 2) {
        return arg1;
    }
    state = *(Game1FEA80RibbonState **)(arg0 + 0x98);
    points = *(Game1FEA80RibbonPoint **)(arg0 + 0x94);
    func_151D5D60(arg0 + 0x84, arg2, (arg0[0x25] * 2 + 10) * sizeof(Game1FEA80RibbonVertex), (void **)&vtx, NULL);
    if (vtx == NULL) {
        return arg1;
    }
    flag = 1;
    arg1 = (Gfx *)func_15142E24((s32)arg1, (s32)D_80091430, 0, 0, 0, 0, 0xBC, 0, 0, &flag, 3);
    arg1 = func_15142B7C(arg1, 0x220005, 0x1D0600);
    arg1 = func_1513F4E4(arg1, 0x4C, &flag);
    arg1 = func_15142FBC(arg1, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[1][0] | D_800A4AC8[1][1], &flag);
    previous = *(s8 *)(arg0 + 0x2E) - 1;
    if (previous < 0) {
        previous = arg0[0x25] - 1;
    }
    index = previous - 1;
    if (index < 0) {
        index = arg0[0x25] - 1;
    }
    first = points[previous].position;
    firstX = points[previous].width * points[previous].directionX;
    firstZ = points[previous].width * points[previous].directionZ;
    firstAlpha = (points[previous].alpha * state->alpha) >> 8;
    firstTexture = points[previous].texture;
    next = points[index].position;
    nextX = points[index].width * points[index].directionX;
    nextZ = points[index].width * points[index].directionZ;
    nextAlpha = (points[index].alpha * state->alpha) >> 8;
    nextTexture = points[index].texture;
    vtx->x = first.x + firstX;
    vtx->y = first.y;
    vtx->z = first.z + firstZ;
    vtx->a = firstAlpha;
    vtx->t = firstTexture;
    vtx->s = 0x7FF;
    vtx->flag = 0;
    vtx++;
    vtx->x = first.x - firstX;
    vtx->y = first.y;
    vtx->z = first.z - firstZ;
    vtx->a = firstAlpha;
    vtx->t = firstTexture;
    vtx->s = 0;
    vtx->flag = 0;
    vtx++;
    do {
        vtx->x = next.x + nextX;
        vtx->y = next.y;
        vtx->z = next.z + nextZ;
        vtx->a = nextAlpha;
        vtx->t = nextTexture;
        vtx->s = 0x7FF;
        vtx->flag = 0;
        vtx++;
        vtx->x = next.x - nextX;
        vtx->y = next.y;
        vtx->z = next.z - nextZ;
        vtx->a = nextAlpha;
        vtx->t = nextTexture;
        vtx->s = 0;
        vtx->flag = 0;
        vtx++;
        gSPVertex(arg1++, vtx - 4, 4, 0);
        gSP2Triangles(arg1++, 0, 1, 2, 0, 1, 3, 2, 0);
        if (firstTexture < nextTexture) {
            memcpy(vtx, vtx - 2, 0x20);
            vtx[-2].t -= 0x2000;
            vtx++;
            vtx[-2].t -= 0x2000;
            vtx++;
        }
        previous = index;
        index--;
        if (index < 0) {
            index = arg0[0x25] - 1;
        }
        if (previous != *(s8 *)(arg0 + 0x2D)) {
            next = points[index].position;
            nextX = points[index].width * points[index].directionX;
            firstTexture = points[previous].texture;
            nextZ = points[index].width * points[index].directionZ;
            nextTexture = points[index].texture;
            nextAlpha = (points[index].alpha * state->alpha) >> 8;
        }
    } while (previous != *(s8 *)(arg0 + 0x2D));
    return arg1;
}

void func_151D223C(ConkerField98Owner *arg0) {
    ConkerField0Slot *temp_v0;
    ConkerField30Object *temp_v1;

    temp_v0 = arg0->field_0x98;
    temp_v1 = temp_v0->field_0x0;
    if (temp_v1 != 0) {
        temp_v1->field_0x30 = 0;
    }
}

extern void func_151478F4(void *);

void func_151D2258(s32 arg0) {
    func_151D223C(arg0);
    func_151478F4((void *)arg0);
}

extern void func_15147928(s32);

void func_151D2284(s32 arg0) {
    func_151D223C(arg0);
    func_15147928(arg0);
}

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
