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


typedef struct {
    TrailVec3 pos;
    u8 pad0C[8];
    f32 width;
    TrailVec2 off;
    f32 dx, dz;
} Ent151CF380;

typedef struct {
    u8 pad0[0x1D4];
    s32 unk1D4;
} Sub151CF380;

typedef struct {
    Sub151CF380 *sub;
} Info151CF380;

typedef struct {
    u8 pad0[0x25];
    u8 count;
    u8 pad26[6];
    s8 num;
    s8 head;
    s8 tail;
    u8 pad2F[0x55];
    u8 unk84[0x10];
    Ent151CF380 *points;
    Info151CF380 *info;
} Obj151CF380;

typedef struct {
    s32 unk0;
    s32 unk4;
} Mode151CF380;

extern Mode151CF380 D_800A4AC8[];
extern s32 D_800D2C9C;
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

Gfx *func_151CF380(Obj151CF380 *arg0, Gfx *gfx, s16 arg2) {
    Info151CF380 *info;
    TrailVec3 b;
    TrailVec3 a;
    s32 i;
    s32 j;
    Ent151CF380 *points;
    u8 update;
    f32 wa;
    f32 ax;
    f32 az;
    f32 wb;
    f32 bx;
    f32 bz;
    TrailVec2 d;
    TrailVec2 c;
    Vtx *vtx;

    if (arg0->num < 2) {
        return gfx;
    }
    info = arg0->info;
    points = arg0->points;
    if (info->sub != NULL && info->sub->unk1D4 == 0) {
        return gfx;
    }
    func_151D5D60(arg0->unk84, arg2, (arg0->count * 2 + 10) * sizeof(Vtx), (void **)&vtx, NULL);
    update = 1;
    gfx = func_15142B7C(func_15142C10(gfx, 0xED, 0xD2, 0x85, 0xC8, &update), 0x200005, 0x1F0600);
    gfx = func_15142FBC(func_1513F4E4(gfx, 0x54, &update), D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[1].unk0 | D_800A4AC8[1].unk4, &update);
    i = arg0->tail - 1;
    if (i < 0) {
        i = arg0->count - 1;
    }
    j = i - 1;
    if (j < 0) {
        j = arg0->count - 1;
    }
    a = points[i].pos;
    wa = points[i].width;
    ax = points[i].dx * wa;
    az = points[i].dz * wa;
    c = points[i].off;
    b = points[j].pos;
    wb = points[j].width;
    bx = points[j].dx * wb;
    bz = points[j].dz * wb;
    d = points[j].off;
    vtx->v.ob[0] = (s32)(a.x + c.x + ax);
    vtx->v.ob[1] = (s32)a.y;
    vtx->v.ob[2] = (s32)(a.z + c.z + az);
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = (s32)(a.x - c.x + ax);
    vtx->v.ob[1] = (s32)a.y;
    vtx->v.ob[2] = (s32)(a.z - c.z + az);
    vtx->v.flag = 0;
    vtx++;
    do {
        vtx->v.ob[0] = (s32)(b.x + d.x + bx);
        vtx->v.ob[1] = (s32)b.y;
        vtx->v.ob[2] = (s32)(b.z + d.z + bz);
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = (s32)(b.x - d.x + bx);
        vtx->v.ob[1] = (s32)b.y;
        vtx->v.ob[2] = (s32)(b.z - d.z + bz);
        vtx->v.flag = 0;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP2Triangles(gfx++, 0, 1, 2, 0, 1, 3, 2, 0);
        i = j;
        j--;
        if (j < 0) {
            j = arg0->count - 1;
        }
        if (i != arg0->head) {
            a = points[i].pos;
            wa = points[i].width;
            ax = points[i].dx * wa;
            az = points[i].dz * wa;
            b = points[j].pos;
            wb = points[j].width;
            bx = points[j].dx * wb;
            bz = points[j].dz * wb;
            d = points[j].off;
            c = points[i].off;
        }
    } while (i != arg0->head);
    return gfx;
}

struct s151CF844 { s32 pad[0x26]; s32 *unk98p; };

void func_151CF844(struct s151CF844 *arg0, s32 arg1, u8 arg2) {
    s32 *temp = arg0->unk98p;
    if (temp[0] != 0) {
        func_15169850(arg1, arg2, (s32)temp, (s32)temp + 4, (s32)arg0);
    }
}

typedef struct {
    char pad0[0x23D];
    u8 unk23D;
} ConkerStruct151CF898Camera;

typedef struct {
    struct17 vec;
    f32 unkC;
} ConkerStruct151CF898Payload;

void func_151CF898(struct127 *arg0, f32 arg1, f32 arg2) {
    ConkerStruct151CF898Camera *camera;
    struct17 sp58;
    struct17 *pos;
    f32 yaw;
    f32 angle;
    f32 distance;
    ConkerStruct151CF898Payload sp38;
    struct260 *temp_v0;

    camera = (ConkerStruct151CF898Camera *)arg0->camera;
    if (camera != NULL) {
        if (func_150ADA68() < D_800AB020) {
            pos = func_15144B34(camera->unk23D);
            yaw = func_15144AA8(camera->unk23D);
            angle = ((func_150ADA68() * 80.0f) + (yaw - 40.0f)) * D_800AB024;
            distance = func_150ADA68() * 2000.0f;

            sp58.unk0 = pos->unk0 - (sinf(angle) * distance);
            sp58.unk4 = arg1;
            sp58.unk8 = pos->unk8 - (cosf(angle) * distance);

            if (func_15046C80((f32 *)&sp58, 0, arg2, &D_800D9860) != 0) {
                sp58.unk4 = D_800D9860;
                sp38.vec = sp58;
                sp38.unkC = 0.0f;

                temp_v0 = func_15149130((s16)((func_150ADA20() % 0x83U) + 0x33),
                                        -1, 0x20, -1, 1, 0, (struct37 *)0x10, 0xFF, 1);
                if (temp_v0 != NULL) {
                    memcpy((u8 *)temp_v0 + 0x28, &sp38, 0x10);
                }
            }
        }
    }
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3_151CFA4C;

typedef struct {
    /* 0x00 */ Vec3_151CFA4C pos;
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
} Cfg1A38_151CFA4C;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ struct17 unk10;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ struct17 pos;
    /* 0x34 */ struct17 vel;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ struct127 *unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
} Part151CFA4C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ struct17 pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s8 unk2C;
} Cfg0D1C_151CFA4C;

typedef struct { s32 v[7]; } Ids151CFA4C;

extern struct17 D_800AAEEC[];
extern Ids151CFA4C D_800AAF40;
void func_1503F404(s32, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
void func_15143134(struct17 *, struct17 *, struct255 *);
void func_15143794(s16, s16, f32, struct17 *);
s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
void func_15150D1C(Cfg0D1C_151CFA4C *, u8, s32);
void func_151A9834(struct17 *, f32, f32, void *, s32, s32, s8 *, u8, s32);
void func_15151A38(void *, u8, s32);
void func_1504715C(void *, struct127 *);

void func_151CFA4C(struct127 *arg0, u8 arg1, s32 arg2) {
    struct17 c;
    struct17 pos;
    struct17 b;
    struct17 a;
    u8 i;

    pos.unk0 = arg0->x_position;
    pos.unk4 = arg0->y_position + 200.0f;
    pos.unk8 = arg0->z_position;
    if (arg0->unk1D4) {
        func_1503F404((s32)arg0->unk1D4, &b.unk0, &b.unk4, &b.unk8, &c.unk0, &c.unk4, &c.unk8, &a.unk0, &a.unk4, &a.unk8);
        {
            Part151CFA4C p;
            const s32 ids[7] = { 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75 };

            p.unk0 = 1.0f;
            p.unk4 = 0.6490000486f;
            p.unk8 = p.unkC = (arg0->xz_scale + arg0->y_scale) * 0.5f;
            p.unk10 = c;
            p.unk1C = 1.0f;
            p.unk20 = 0.0f;
            p.unk24 = 1.0f;
            p.unk44 = 0.0f;
            p.unk58 = 0;
            p.unk5C = 0;
            p.unk60 = 0xFF;
            p.unk62 = 0;
            p.unk64 = 0;
            p.unk65 = 0;
            p.unk66 = 0;
            p.unk67 = 0;
            p.unk6A = 1;
            p.unk6C = arg0;
            p.unk70 = arg0->unique_id;
            p.unk72 = 0xC;
            p.unk74 = 0x15;
            p.unk50 = 0x39E9;
            p.unk61 = 0xD;
            p.unk68 = 2;
            p.unk63 = 7;
            for (i = 0; i < 7; i++) {
                u32 r1;
                u32 r2;

                p.unk56 = ids[i];
                func_15143134(&D_800AAEEC[i], &p.pos, arg0->unk1D4);
                r1 = func_150ADA20();
                r2 = func_150ADA20();
                func_15143794(r1 & 0xFF, (r2 % 25U) - 0x40, (func_150ADA68() * 11.44999981f) + 16.93000031f, &p.vel);
                p.unk40 = (func_150ADA68() * 19.75200081f) + -9.876000404f;
                p.unk48 = (func_150ADA68() * 19.75200081f) + -9.876000404f;
                p.unk4C = (func_150ADA68() * 0.5180000067f) + -1.539000034f;
                p.unk54 = (func_150ADA20() % 101U) + 100;
                func_15132A4C(&p, 3, 0xFF, 0, arg1, arg2);
            }
        }
        {
            Cfg1A38_151CFA4C cfg;

            cfg.pos = *(Vec3_151CFA4C *)&pos;
            cfg.unk0C = 8.0f;
            cfg.unk10 = 13.0f;
            cfg.unk14 = 0.8000000119f;
            cfg.unk18 = 0.400000006f;
            cfg.unk1C = 23.0f;
            cfg.unk20 = 7.0f;
            cfg.unk24 = 0xD;
            cfg.unk26 = 6;
            cfg.unk28 = 0;
            cfg.unk2A = 0xFF;
            cfg.unk2C = -0x40;
            cfg.unk2E = 0x32;
            cfg.unk30 = 3;
            cfg.unk32 = 2;
            cfg.unk34 = 0x28;
            cfg.unk36 = 0x19;
            cfg.unk38 = 9;
            cfg.unk39 = 1;
            cfg.unk3A = 0x48;
            cfg.unk3C = 1;
            cfg.unk40 = 0;
            cfg.unk44 = 0;
            cfg.unk48 = 0;
            cfg.unk4C = 0;
            cfg.unk50 = 0;
            cfg.unk54 = 0;
            cfg.unk55 = 0x96;
            cfg.unk56 = 0x69;
            cfg.unk58 = 3;
            cfg.unk5C = 0xFF;
            cfg.unk60 = 0;
            cfg.unk64 = 0x220005;
            cfg.unk68 = 0x1D0600;
            cfg.unk6C = 3;
            cfg.unk70 = 0x3B;
            cfg.unk74 = 0x80;
            cfg.unk78 = 0x20;
            cfg.unk7C = 0;
            cfg.unk7D = 7;
            cfg.unk80 = 0xA;
            cfg.unk82 = 0x19;
            func_15151A38(&cfg, arg1, arg2);
        }
        {
            Cfg0D1C_151CFA4C cfg2;

            cfg2.unk0 = 0xF;
            cfg2.unk4 = 6;
            cfg2.pos = pos;
            cfg2.unk14 = 0;
            cfg2.unk16 = 0xFF;
            cfg2.unk18 = -0x1D;
            cfg2.unk1A = 0x28;
            cfg2.unk1C = 375.0f;
            cfg2.unk20 = 20.0f;
            cfg2.unk24 = 40.0f;
            cfg2.unk28 = 0x78;
            cfg2.unk2A = 0x78;
            cfg2.unk2C = 2;
            func_15150D1C(&cfg2, arg1, arg2);
        }
        {
            u8 dst[0x24];
            s8 flag;
            s32 pad;

            func_1504715C(dst, arg0);
            flag = 2;
            func_151A9834(&pos, pos.unk4 - 2000.0f, 200.0f, dst, (func_150ADA20() & 3) + 7, 0, &flag, arg1, arg2);
        }
    }
}

void func_151D0024(struct127 *arg0) {
    struct {
        s32 unk0;
        u8 unk4;
    } sp18;

    sp18.unk0 = (s32)arg0;
    sp18.unk4 = arg0->unique_id;
    func_151494E0((s32)&sp18, 0x18);
}

void func_151D0058(struct127 *arg0, u8 arg1, u8 arg2, s32 arg3) {
    struct260 *temp_v0;
    struct {
        struct127 *unk0;
        u8 unk4;
        u8 pad5;
        u16 unk6;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE;
        u8 padF;
    } sp3C;

    sp3C.unk0 = arg0;
    sp3C.unk4 = arg0->unique_id;
    sp3C.unk8 = (arg0->xz_scale + arg0->y_scale) * 0.5f;
    sp3C.unkD = arg1;
    sp3C.unkC = (func_150ADA20() % 0x38U) + 0xC8;
    sp3C.unk6 = arg0->unk84.uh;

    temp_v0 = func_15149130(0x12C, -1, 0x61, 4, 0, 0x31, (struct37 *)0x10, arg2, arg3);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp3C, 0x10);
    }
}

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

void func_151D0128(ConkerStruct151D0128 *arg0) {
    ConkerStruct151D0128b *p = arg0->unk28;
    if (arg0->unk2E != p->unk84) {
        arg0->unkE = -1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FC830/func_151D014C.s")

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

void func_151D08F0(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA151D08F0 *temp_v0 = (SubA151D08F0 *)((u8 *)arg0 + 0x28);
    ArgB151D08F0 *b = (ArgB151D08F0 *)arg1;
    s32 b0;

    if (arg2 == 0 || arg2 == 0x18) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else if (temp_v0->unk0 == b->u4.w4) {
            temp_v0->unk0 = b->unk0;
            temp_v0->unk4 = b->unk8;
        }
    }
}

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

void func_151D09A8(struct127 *arg0, u8 arg1, s32 arg2) {
    struct17 pos;
    struct17 dir;
    Dst151D09A8 dst;
    ShapeA151D09A8 a;

    if (arg0->unk1D4 != NULL) {
        {
            s32 srcs[2];
            s32 dsts[2];

            srcs[0] = (s32)D_800AAF84;
            srcs[1] = (s32)D_800AAF90;
            dsts[0] = (s32)&pos;
            dsts[1] = (s32)&dir;
            func_15145EA4(srcs, dsts, (s32)arg0->unk1D4 + 0x80, 2);
            dir.unk0 -= pos.unk0;
            dir.unk4 -= pos.unk4;
            dir.unk8 -= pos.unk8;
        }
        func_1504715C(&dst, arg0);
        a.unk0 = 0;
        a.unk4 = dir;
        a.unk10 = a.unk1C = *(struct17 *)&D_800A5480;
        {
            EmitA151D09A8 emitA;

            emitA.unk0 = pos;
            emitA.unkC = 6;
            emitA.unkE = 8;
            emitA.unk10 = 0.03999999911f;
            emitA.unk14 = 0.02999999933f;
            emitA.unk18 = 0x1E;
            emitA.unk1A = 0x1E;
            emitA.unk1C = -1.467000127f;
            emitA.unk20 = 0.756000042f;
            emitA.unk24 = 0x9B;
            emitA.unk25 = 0x64;
            emitA.unk28 = 403.0f;
            emitA.unk2C = 455.0f;
            emitA.unk30 = 1;
            emitA.unk31 = 2;
            emitA.unk34 = 0.0f;
            emitA.unk38 = 1;
            emitA.unk3C = 0.0f;
            a.unk28 = 750.0f;
            func_1514FF44(&a, &emitA, &dst, arg1, arg2);
        }
        {
            EmitB151D09A8 emitB;

            emitB.unk0 = pos;
            emitB.unkC = 10.0f;
            emitB.unk10 = 25.0f;
            emitB.unk14 = 1.253000021f;
            emitB.unk18 = 0.8230000138f;
            emitB.unk1C = 0.03500000015f;
            emitB.unk20 = 0.03199999779f;
            emitB.unk24 = 0xF;
            emitB.unk26 = 8;
            emitB.unk28 = 3;
            emitB.unk2A = 2;
            emitB.unk2C = 0x1E;
            emitB.unk2E = 0x19;
            emitB.unk30 = 0x9B;
            emitB.unk32 = 0x64;
            emitB.unk38 = 0.0f;
            emitB.unk3C = 0x10;
            emitB.unk3E = 0xF;
            emitB.unk40 = 0;
            emitB.unk34 = 2;
            a.unk28 = 750.0f;
            func_15153CCC(&a, &emitB, &dst, arg1, arg2);
        }
        if (func_1514F6E8(&a) != 0) {
            s32 count;
            Particle151D09A8 spawn;
            void *ret;
            s32 pad;

            a.unk28 = 1000.0f;
            count = (func_150ADA20() % 6U) + 5;
            spawn.unk1D = 0x3E;
            spawn.unk8 = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk14 = 0;
            spawn.unk15 = 0;
            spawn.unk16 = 0;
            spawn.unk17 = 0xFF;
            spawn.unk18 = 0;
            spawn.unk19 = 0;
            spawn.unk1A = 0;
            spawn.unk1B = 0;
            spawn.unk1C = 0xFF;
            spawn.pos = pos;
            spawn.unk3C = *(struct17 *)&D_800A5480;
            spawn.unk22 = 1;
            spawn.unk58 = 0xC007;
            spawn.unk24 = 1.0f;
            spawn.unkA = (func_150ADA20() % 11U) + 0x28;
            spawn.unk1E = 1;
            spawn.unk20 = 0xFF;
            spawn.unk60 = 6;
            spawn.unk61 = 8;
            spawn.unk64 = -1;
            spawn.unk62 = -1;
            spawn.unk63 = -1;
            spawn.unk65 = 0xB;
            spawn.unk5C = 0;
            spawn.unk66 = 0xFF;
            spawn.unk68 = 0;
            spawn.unk2C = 0.0f;
            spawn.unk28 = 0.0f;
            spawn.unk6C = 10000.0f;
            while (count != 0) {
                func_1514F808(&a, (func_150ADA68() * 0.03999999911f) + 0.04999999702f, &spawn.vel);
                spawn.unk54 = (func_150ADA68() * 1.340000033f) + -2.328999996f;
                ret = func_15130280(&spawn, 1, 0, 4, arg1, arg2);
                if (ret != NULL) {
                    *(struct260 **)((u8 *)ret + 0xA8) = func_151CEAAC(NULL, ret, 0, arg1, arg2);
                }
                count--;
            }
        }
    }
}

void func_151D0ED8(void *arg0) {
    if (*(struct102 **)((u8 *)arg0 + 0xA8) != 0) {
        func_1516972C(*(struct102 *volatile *)((u8 *)arg0 + 0xA8));
    }
}

void func_151D0ED8(void *arg0);

void func_1513173C(void *arg0);

void func_151D0F08(void *arg0) {
    func_151D0ED8(arg0);
    func_1513173C(arg0);
}

void func_1513175C(void *arg0);

void func_151D0F34(void *arg0) {
    func_151D0ED8(arg0);
    func_1513175C(arg0);
}

extern s32 (*D_8008FC30[])(struct127 *, void *);

struct vec3_151D0F60 { f32 x; f32 y; f32 z; };

void func_151D0F60(struct127 *arg0, u8 arg1, u8 arg2, s32 arg3) {
    struct260 *temp_v0;
    struct {
        struct127 *unk0;
        u8 unk4;
        u8 pad5[3];
        s32 unk8;
        struct vec3_151D0F60 unkC;
        struct vec3_151D0F60 unk18;
        struct vec3_151D0F60 unk24;
        f32 unk30;
        u8 unk34;
    } sp34;

    sp34.unk0 = arg0;
    sp34.unk4 = arg0->unique_id;
    sp34.unk8 = 0;

    if (D_8008FC30[arg1](arg0, &sp34.unk18) == 0) {
        sp34.unk18 = *(struct vec3_151D0F60 *)&D_800A5480;
    }

    sp34.unkC = sp34.unk18;
    sp34.unk24 = *(struct vec3_151D0F60 *)&D_800A5480;
    sp34.unk30 = 0.0f;
    sp34.unk34 = arg1;

    temp_v0 = func_15149130(0x12C, -1, 0x5F, -1, 0, 0x48, (struct37 *)0x38, arg2, arg3);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0x38);
    }
}

s32 func_151D1074(f32 *arg0, f32 *arg1) {
    arg1[0] = arg0[5];
    arg1[1] = arg0[96] + 8.0f;
    arg1[2] = arg0[7];
    return 1;
}

s32 func_151D10E4(void *arg0, s32 arg1, u8 arg2);

s32 func_151D10A4(void *arg0, s32 arg1) {
    return func_151D10E4(arg0, arg1, 0);
}

s32 func_151D10C4(void *arg0, s32 arg1) {
    return func_151D10E4(arg0, arg1, 1);
}

extern struct17 D_800AAF9C[];

s32 func_151D10E4(void *arg0, s32 arg1, u8 arg2) {
    s32 temp = (s32)((struct127 *)arg0)->unk1D4;

    if (temp == 0) {
        return 0;
    }
    func_15143134(&D_800AAF9C[arg2], (struct17 *)arg1, (struct255 *)(temp | 0));
    return 1;
}

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

void func_151D1138(struct260 *arg0) {
    Sub151D1138 *sub;
    struct127 *obj;
    u8 finished = 0;
    s32 state;

    sub = (Sub151D1138 *)((u8 *)arg0 + 0x28);
    obj = sub->unk0;

    if (obj->interaction_state == 0 || obj->unique_id != sub->unk4) {
        arg0->unkE = -1;
        return;
    }

    sub->unkC = sub->unk18;
    if (D_8008FC30[sub->unk34](obj, &sub->unk18) == 0) {
        sub->unk18 = sub->unkC;
        finished = 1;
    }

    sub->unk24.unk0 = sub->unk18.unk0 - sub->unkC.unk0;
    sub->unk24.unk4 = sub->unk18.unk4 - sub->unkC.unk4;
    sub->unk24.unk8 = sub->unk18.unk8 - sub->unkC.unk8;
    sub->unk30 = func_15143E64(&sub->unk24);

    state = func_1510F8CC(obj->unk184);
    if (sub->unk30 > 300.0f || sub->unk30 <= 0.0f || obj->unk28 != 0.0f || obj->in_water != 0 ||
        state == 5 || state == 6 || state == 9 || state == 0xD || state == 0xE) {
        finished = 1;
    }

    if (finished != 0) {
        func_151D13E0(arg0);
    } else if (sub->unk8 == 0) {
        func_151D1448(arg0);
    }
}

void func_151D1328(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

void func_151D13E0(struct260 *arg0);

void func_151D1368(struct260 *arg0) {
    func_151D13E0(arg0);
}

void func_151D1388(struct260 *arg0) {
    func_151D1368(arg0);
    func_1514933C(arg0);
}

void func_15149368(struct260 *arg0);

void func_151D13B4(struct260 *arg0) {
    func_151D1368(arg0);
    func_15149368(arg0);
}

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

void func_151D13E0(struct260 *arg0) {
    struct Slot151D13E0 *slot = (struct Slot151D13E0 *)((char *)arg0 + 0x28);
    struct Obj151D13E0 *obj;
    s32 *p;

    if (slot->unk8 != 0) {
        p = slot->unk8->unk98;
        slot->unk8->unk30 = 0;
        slot->unk8->unk1E &= ~0x2;
        slot->unk8->unk1E |= 0x8;
        slot->unk8->unk1E |= 0x1;
        slot->unk8->unk1C = 0x28;
        *p = 0;
        slot->unk8 = 0;
    }
}

void func_151D1448(u8 *arg0) {
    u8 *link;
    struct {
        Vec3i151CD674 position;
        s16 type;
        s16 subtype;
        s32 mode;
        u8 flag;
        u8 value;
        u8 pad_16[2];
        s32 reserved;
        u8 *object;
        f32 zero;
        u8 color;
        u8 pad_25[3];
        f32 range;
        f32 width;
        f32 height;
        f32 extent;
        f32 source_value;
    } packet;
    u8 *state;
    u8 *result;

    link = arg0 + 0x28;
    state = *(u8 **)link;
    packet.object = arg0;
    packet.color = 0xFF;
    packet.zero = 0.0f;
    if (state[4] == 0x25) {
        packet.range = 800.0f;
        packet.width = 36.0f;
        packet.height = 50.0f;
        packet.extent = 200.0f;
    } else {
        if (D_80082FA0 > 0) {
            packet.range = 260.0f;
        } else {
            packet.range = 500.0f;
        }
        packet.width = 40.0f;
        packet.height = 40.0f;
        packet.extent = 180.0f;
    }
    packet.source_value = *(f32 *)(*(u8 **)link + 0x14C);
    packet.value = 0x19;
    packet.position = *(Vec3i151CD674 *)(link + 0x18);
    packet.type = 0x12C;
    packet.subtype = 0x36;
    packet.mode = 0x13;
    packet.flag = 6;
    packet.reserved = 0;
    result = (u8 *)func_15147A80(&packet, 0x20, 0x28, 0, 0x11, 0x12, 0, 0, 0, arg0[0xC], arg0[1]);
    if (result != 0) {
        memcpy(*(void **)(result + 0x98), &packet.object, 0x20);
        *(u8 **)(link + 8) = result;
    }
}

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
void func_151D22B0(struct127 *arg0, u8 arg1, s32 arg2);
