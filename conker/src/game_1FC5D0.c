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

s32 func_151CF120(Game1FC5D0Actor *arg0) {
    Game1FC5D0Config *config;
    Game1FC5D0Point *points;
    s32 count;

    count = arg0->count;
    config = arg0->config;
    points = arg0->points;
    if (count >= 2) {
        f32 total;
        f32 limit;
        s32 index;
        s32 current;
        Game1FC5D0Point *point;

        total = 0.0f;
        index = arg0->tail - 1;
        if (index < 0) index = arg0->capacity - 1;
        if (index != arg0->head) {
            limit = config->limit;
            do {
                current = index;
                index--;
                if (index < 0) index = arg0->capacity - 1;
                point = &points[current];
                total += point->distance;
                if (limit < total) {
                    if (point->distance != 0) {
                        Game1FC5D0Point *previous;
                        f32 ratio;
                        f32 px, py, pz;
                        f32 dx, dy, dz;

                        ratio = (total - limit) / point->distance;
                        previous = &points[index];
                        px = previous->x; py = previous->y; pz = previous->z;
                        dx = px - point->x; dy = py - point->y; dz = pz - point->z;
                        previous->x = px - dx * ratio;
                        previous->y = py - dy * ratio;
                        previous->z = pz - dz * ratio;
                        point->distance *= 1.0f - ratio;
                    }
                    if (index != arg0->head) {
                        do {
                            arg0->head++;
                            if (arg0->head == arg0->capacity) {
                                arg0->head = 0;
                            }
                            arg0->count--;
                        } while (index != arg0->head);
                    }
                    limit = config->limit;
                    total = limit;
                }
            } while (index != arg0->head);
            count = arg0->count;
        }
    }
    if (count >= 2) {
        f32 amplitude;
        f32 phase;
        s32 index;

        amplitude = 0.0f;
        index = arg0->tail;
        phase = config->phase;
        do {
            Game1FC5D0Point *point;
            f32 result;
            f32 distance;

            index--;
            if (index < 0) index = arg0->capacity - 1;
            point = &points[index];
            result = sinf(phase);
            point->wave = result * amplitude;
            distance = point->distance;
            amplitude += distance * 0.037499998f;
            phase += distance * 0.0122f;
        } while (index != arg0->head);
    }
    config->phase += -0.70900005f * D_800BE9A4;
    return 1;
}

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
