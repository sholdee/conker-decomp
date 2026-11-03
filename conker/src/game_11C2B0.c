#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    u8 pad0[0x94];
    s32 unk94;
    u8 pad98[0x13C];
    s32 unk1D4;
} struct_150EEE00_arg0;

extern u8 D_800A1638[];
extern u8 D_800A163C[];
extern f32 D_800A15F0[][3];
extern s32 (*D_8008FD00)(s32, u8);
extern f32 D_800A1830;
extern f32 D_800A1834;
extern f32 D_800A1870;
extern f32 D_800A1874;
extern u8 D_800A1674;
extern u8 D_800A1680;
extern f32 sinf(f32);

typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
} Struct150F0E48Src;

typedef struct {
    Struct150F0E48Src *unk0;
    u8 unk4;
    u8 pad5[3];
    f32 unk8;
    f32 unkC;
    f32 unk10;
} Struct150F0E48Sub;

typedef struct {
    u8 pad0[0x2B];
    u8 unk2B;
    u8 pad2C[0xC];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 pad4C[0x5C];
    Struct150F0E48Src *unkA8;
    u8 unkAC;
} Struct150F0E48Obj;

typedef struct {
    u8 pad_0[0x68];
    u8 field_0x68;
    u8 field_0x69;
    u8 pad_1[0x1A];
    u16 field_0x84;
    u8 pad_2[0x25E];
    s32 field_0x2E4;
} ActorStateFields;

void *func_150EEF80(struct_150EEE00_arg0 *, u8, u8, s32);
void func_15143134(void *, void *, s32);
void func_15143874(s32, f32, f32 *, f32 *);
void func_151C329C(void *, s32, s32);
void *func_151407D0(void *, s32, void *, s32, s32, s32, s32, s32, s32, s32);
struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);
s32 func_15160A58(s32, s32, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_150F0A24(f32 *);

void func_150EEE00(struct_150EEE00_arg0 *arg0, u8 arg1) {
    f32 sp54[3];
    Header sp4C;
    Header2 sp40;
    s32 temp_v1;

    if (D_800A163C[arg1] & arg0->unk94) {
        return;
    }

    func_150EEF80(arg0, arg1, 0xFF, 1);
    temp_v1 = arg0->unk1D4;
    if (temp_v1 != 0) {
        func_15143134(D_800A15F0[arg1], sp54, temp_v1 + (D_800A1638[arg1] << 6));
        sp4C.unk0 = 3;
        sp4C.unk1 = -1;
        sp4C.unk2 = (func_150ADA20() % 3U) + 4;
        sp4C.unk4 = 0;
        sp40.unk0 = (s32) sp54[0];
        sp40.unk4 = (s32) sp54[1];
        sp40.unk8 = (s32) sp54[2];
        func_151602C0(&sp4C, &sp40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 1);
        func_150F0A24(sp54);
    }
}

void func_150EEF40(struct210 *arg0, u8 arg1) {
    s8 sp18[6];

    *(struct210 **)&sp18[0] = arg0;
    sp18[4] = *(u8 *)((s32)arg0 + 0x3B);
    sp18[5] = arg1;
    func_151403A8(&sp18, 0x43);
}

typedef struct {
    u8 pad_0[0x3B];
    u8 field_0x3B;
} ActorIdByteFields;

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ struct17 unk1C;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} Spawn150EEF80; /* 0x58 */

typedef struct {
    /* 0x00 */ struct_150EEE00_arg0 *unk0;
    /* 0x04 */ u8 unk4;
    /* 0x08 */ struct127 *unk8;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 unk18;
    /* 0x1C */ void *unk1C[2];
    /* 0x24 */ struct260 *unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ struct225 *unk2C;
    /* 0x30 */ struct17 unk30;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
} Ext150EEF80; /* 0x44 */

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ s8 unk1D;
    /* 0x20 */ f32 unk20[10];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ u8 unk4C;
} Part150EEF80; /* 0x50 */

typedef struct {
    /* 0x00 */ struct_150EEE00_arg0 *unk0;
    /* 0x04 */ u8 unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ u8 unkC;
} Link150EEF80; /* 0x10 */

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ f32 unk4;
} Link2150EEF80; /* 0x8 */

typedef struct {
    u8 pad0[0x60];
    void *unk60;
} Obj150EEF80;

extern f32 D_800A1814;
extern f32 D_800A1818;
extern void *func_151A7950(void *, s32, u8, s32);
s32 func_150EFEC8(ActorIdByteFields *, u8, u8, u8, u8, s32, u8, s32);
struct225 *func_150F0198(u8, u8, u8, u8, s32, u8, s32);

void *func_150EEF80(struct_150EEE00_arg0 *arg0, u8 arg1, u8 arg2, s32 arg3) {
    void *ret;
    Spawn150EEF80 spawn;
    Ext150EEF80 ext;
    Ext150EEF80 *dst;

    if (arg0 == NULL) {
        return NULL;
    }
    if (arg1 >= 2) {
        return NULL;
    }
    ext.unk10 = func_150ADA68() * 6.283185482f;
    ext.unk14 = func_150ADA68() * 6.283185482f;
    ext.unk0 = arg0;
    ext.unk4 = arg0->pad0[0x3B];
    ext.unk8 = func_15083E90(0x12);
    ext.unkC = 0x12;
    ext.unkD = arg1;
    ext.unk30 = *(struct17 *)&D_800A5480;
    ext.unk18 = 0;
    spawn.unk0 = 0x94;
    spawn.unk1 = 7;
    spawn.unk2 = 0x2203;
    spawn.unk4 = 0x12C;
    spawn.unk8 = 0;
    spawn.unkC = 0;
    spawn.unk10 = 0xFF;
    spawn.unk11 = 0xFF;
    spawn.unk12 = 0xFF;
    spawn.unk13 = 0xFF;
    ext.unk40 = 1.0f;
    spawn.unk18 = 1.0f;
    ext.unk3C = 0.0f;
    spawn.unk34 = 0.0f;
    spawn.unk38 = 0.0f;
    spawn.unk3C = 0.0f;
    spawn.unk14 = 100.0f;
    spawn.unk1C = *(struct17 *)&D_800A5480;
    spawn.unk28 = *(struct17 *)&D_800A5480;
    spawn.unk40 = 0x046C0008;
    spawn.unk44 = 0xFF;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0;
    spawn.unk47 = 7;
    spawn.unk48 = 0;
    spawn.unk4C = 0xFF;
    spawn.unk50 = 0;
    spawn.unk54 = 1;
    spawn.unk56 = 0xFF;
    ret = func_1513D2F0(&spawn, (s32)&D_800A4AA0, 0x1D, 0, 0, 0x1C, 0, 0, 0, 0x44, arg2, arg3);
    if (ret != NULL) {
        dst = (Ext150EEF80 *)((u8 *)ret + 0x110);
        memcpy(dst, &ext, sizeof(ext));
        {
        Part150EEF80 part;
        Link150EEF80 link;
        Link2150EEF80 link2;
        struct260 *tmp;
        part.unk0 = 0x12C;
        part.unk2 = 9;
        part.unk4 = 1.0f;
        part.unk8 = 1.0f;
        part.unkC = 0;
        part.unk14 = 0xFF;
        part.unk15 = 0xDC;
        part.unk16 = 0xA0;
        part.unk17 = 0xC8;
        part.unk1C = 0;
        part.unk1D = -1;
        part.unk20[0] = 0.0f;
        part.unk20[1] = 0.0f;
        part.unk20[2] = 0.0f;
        part.unk20[3] = 0.0f;
        part.unk20[4] = 0.0f;
        part.unk20[5] = 0.0f;
        part.unk20[6] = 0.0f;
        part.unk20[7] = 0.0f;
        part.unk20[8] = 0.0f;
        part.unk20[9] = 0.0f;
        part.unk48 = 1.0f;
        link.unk0 = arg0;
        part.unk10 = 0.9499999881f;
        part.unk18 = 40.0f;
        link.unk4 = arg0->pad0[0x3B];
        link.unk8 = ret;
        part.unk4C = 1;
        for (link.unkC = 0; link.unkC < 2; link.unkC++) {
            dst->unk1C[link.unkC] = func_151A7950(&part, 0x10, arg2, arg3);
            if (dst->unk1C[link.unkC] != NULL) {
                memcpy(((Obj150EEF80 *)dst->unk1C[link.unkC])->unk60, &link, sizeof(link));
            }
        }
        link2.unk0 = ret;
        link2.unk4 = 0.0f;
        tmp = func_15149130(0x12C, -1, 0x22, -1, 0, 0x20, (struct37 *)8, arg2, arg3);
        dst->unk24 = tmp;
        if (tmp != NULL) {
            memcpy((u8 *)tmp + 0x28, &link2, sizeof(link2));
        }
        dst->unk28 = func_150EFEC8((ActorIdByteFields *)arg0, arg1, 0xFF, 0xD9, 0xA0, (s32)ret, arg2, arg3);
        dst->unk2C = func_150F0198(0x46, 0xFF, 0x82, 0, (s32)ret, arg2, arg3);
        }
    }
    return ret;
}

typedef struct { f32 x, y, z; } Vec3f_150EF38C;

typedef struct Game11C2B0Effect {
    u8 pad0[9];
    u8 hidden;
    u8 padA[4];
    s16 position[3];
} Game11C2B0Effect;

typedef struct Game11C2B0EffectOwner {
    u8 pad0[0x14];
    Game11C2B0Effect *sprite;
} Game11C2B0EffectOwner;

typedef struct Game11C2B0Link {
    u8 *actor;
    u8 group;
    u8 pad5[3];
    u8 *target;
    u8 targetGroup;
    u8 slot;
    u8 padE[2];
    f32 phase10;
    f32 phase14;
    u8 flags;
    u8 pad19[0x13];
    Game11C2B0EffectOwner *effect;
    Vec3f_150EF38C direction;
    f32 distance;
    f32 inverse;
} Game11C2B0Link;

typedef struct Game11C2B0Beam {
    u8 pad0[0x2C];
    f32 width;
    u8 pad30[4];
    Vec3f_150EF38C start;
    Vec3f_150EF38C end;
    u8 pad4C[0x10];
    u8 alpha;
    u8 pad5D[0xB3];
    Game11C2B0Link link;
} Game11C2B0Beam;

typedef struct Game11C2B0Hit {
    void *object;
    u8 pad4[0x55];
    u8 count;
    u8 pad5A[0xA];
} Game11C2B0Hit;

s32 func_15145128(f32 *, f32 *, f32 *, f32 *);
void func_15081690(void *, f32, f32, f32, f32, f32, f32, void *, f32, s32, s32, s32, s32, s32, s32);
s32 func_1505D024(void *, s32, s32, s32);
void func_1502EA98(void *, s32, s32, s32, s32, s32, s32);
extern f32 D_800A181C;
extern f32 D_800A1820;

s32 func_150EF38C(Game11C2B0Beam *arg0) {
    Game11C2B0Link *link;
    u8 *actor;
    u8 *target;
    s16 *position;
    Game11C2B0Hit hit;
    f32 angle;
    void *object;

    link = &arg0->link;
    actor = link->actor;
    target = link->target;
    if (link->group != actor[0x3B] || *(s32 *)actor == 0 || actor[4] == 0xFF) {
        return 0;
    }
    if (link->targetGroup != target[0x3B] || *(s32 *)target == 0) {
        return 0;
    }
    if (*(s32 *)(actor + 0x94) & D_800A163C[link->slot]) {
        return 0;
    }
    link->flags &= ~1;
    if (*(s32 *)(actor + 0x1D4) != 0) {
        func_15143134((f32 *)&((u8 *)D_800A15F0)[link->slot * 0xC], &arg0->start.x,
                      *(s32 *)(actor + 0x1D4) + (D_800A1638[link->slot] << 6));
        arg0->end.x = ((Vec3f_150EF38C *)(target + 0x14))->x;
        arg0->end.y = ((Vec3f_150EF38C *)(target + 0x14))->y;
        arg0->end.z = ((Vec3f_150EF38C *)(target + 0x14))->z;
        link->direction.x = arg0->end.x - arg0->start.x;
        link->direction.y = arg0->end.y - arg0->start.y;
        link->direction.z = arg0->end.z - arg0->start.z;
        if (func_15145128(&link->direction.x, &link->direction.x, &link->distance, &link->inverse) != 0) {
            link->flags |= 1;
        }
    }
    link->phase10 += D_800A181C * D_800BE9A4;
    angle = func_15144B68(link->phase10);
    link->phase10 = angle;
    arg0->width = sinf(angle) * 5.0f + 16.0f;
    link->phase14 += D_800A1820 * D_800BE9A4;
    angle = func_15144B68(link->phase14);
    link->phase14 = angle;
    arg0->alpha = sinf(angle) * 25.0f + 230.0f;
    if (link->flags & 1) {
        if (link->effect != NULL) {
            link->effect->sprite->hidden = 0;
            position = link->effect->sprite->position;
            position[0] = arg0->start.x;
            position[1] = arg0->start.y;
            position[2] = arg0->start.z;
        }
        func_15081690(actor, arg0->start.x, arg0->start.y, arg0->start.z,
                      link->direction.x, link->direction.y, link->direction.z,
                      &hit, link->distance, 1, 0, 1, 4, 0, 0);
        if (hit.count >= 2) {
            object = hit.object;
            func_1502EA98(object, 0xFF, 0, 0, 0x90, 0, 1);
            func_1505D024(object, 0xA0038, 0, (actor - (u8 *)D_800CC2D0) / 0x32C);
        }
    } else if (link->effect != NULL) {
        link->effect->sprite->hidden = 1;
    }
    return 1;
}

s32 func_150EF784(u8 *arg0, s32 arg1, s32 arg2) {
    if (arg0[4] == 0x28) {
        return 1;
    }
    return 0;
}

void func_150EF7B0(u8 *arg0) {
    s32 i;
    s32 v;
    s32 *p = (s32 *)(arg0 + 0x110);

    for (i = 0; i != 8; i += 4) {
        v = *(s32 *)((u8 *)p + i + 0x1C);
        if (v != 0) {
            func_1516972C((struct102 *)v);
        }
    }
    v = p[9];
    if (v != 0) {
        func_1516972C((struct102 *)v);
    }
    v = p[10];
    if (v != 0) {
        func_1516972C((struct102 *)v);
    }
    v = p[11];
    if (v != 0) {
        func_1516972C((struct102 *)v);
    }
    func_1513CA6C(arg0);
}

void func_150EF860(u8 *arg0) {
    s32 i;
    s32 v;
    s32 *p = (s32 *)(arg0 + 0x110);

    for (i = 0; i != 8; i += 4) {
        v = *(s32 *)((u8 *)p + i + 0x1C);
        if (v != 0) {
            func_1516979C((struct102 *)v);
        }
    }
    v = p[9];
    if (v != 0) {
        func_1516979C((struct102 *)v);
    }
    v = p[10];
    if (v != 0) {
        func_1516979C((struct102 *)v);
    }
    v = p[11];
    if (v != 0) {
        func_1516979C((struct102 *)v);
    }
    func_1513CAA0((struct210 *)arg0);
}

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        struct {
            u8 b4;
            u8 b5;
            u8 b6;
            u8 b7;
        } b;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB_150EF910;

typedef struct {
    s32 unk0;
    u8 unk4;
    u8 pad5[3];
    s32 unk8;
    u8 unkC;
    u8 unkD;
} SubA_150EF910;

void func_150EF910(void *arg0, s32 arg1, u8 arg2) {
    ArgB_150EF910 *b = (ArgB_150EF910 *)arg1;
    SubA_150EF910 *a = (SubA_150EF910 *)((u8 *)arg0 + 0x110);
    s32 bu0;
    s32 bu0b;
    s32 au0;
    u8 au4;
    u8 bu4;

    if (arg2 == 0) {
        au0 = a->unk0;
        bu0 = b->unk0;
        au4 = a->unk4;
        bu4 = b->u4.b.b4;
        if ((au0 == bu0) || (au4 == bu4)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        if (a->unk0 == b->unk0) {
            a->unk0 = b->u4.w4;
            a->unk4 = b->unk9;
        } else if (a->unk0 == b->u4.w4) {
            a->unk0 = b->unk0;
            a->unk4 = b->unk8;
        }
        if (a->unk8 == b->unk0) {
            a->unk8 = b->u4.w4;
            a->unkC = b->unk9;
        } else {
            if (a->unk8 == b->u4.w4) {
                a->unk8 = b->unk0;
                a->unkC = b->unk8;
            }
        block2_end: ;
        }
    } else if (arg2 == 0x43) {
        if ((a->unk0 == b->unk0) || (a->unk4 == b->u4.b.b4)) {
            if (a->unkD == b->u4.b.b5) {
                func_1516972C((struct102 *)arg0);
            }
        }
    }
}

typedef struct {
    f32 x, y, z;
} Vec3_150EFA4C;

typedef struct {
    u8 *actor;
    u8 pad4[9];
    u8 slot;
    u8 padE[0xA];
    u8 flags;
    u8 pad19[0x23];
    f32 unk3C;
    f32 unk40;
} Sub_150EFA4C;

typedef struct {
    u8 pad0[0x34];
    Vec3_150EFA4C a;
    Vec3_150EFA4C b;
    u8 pad4C[0xC4];
    Sub_150EFA4C sub;
} Obj_150EFA4C;

typedef struct {
    u8 pad0[8];
    Obj_150EFA4C *obj;
} Holder_150EFA4C;

typedef struct {
    u8 pad0[0x1C];
    u8 flags;
    u8 pad1D[0x13];
    Vec3_150EFA4C a;
    Vec3_150EFA4C b;
    Vec3_150EFA4C c;
    f32 unk54;
    f32 unk58;
    u8 pad5C[4];
    Holder_150EFA4C *holder;
} Self_150EFA4C;

extern f32 D_800A1620[][3];

s32 func_150EFA4C(Self_150EFA4C *arg0) {
    Obj_150EFA4C *obj;
    Sub_150EFA4C *sub;
    Holder_150EFA4C *holder;

    holder = arg0->holder;
    obj = holder->obj;
    sub = &obj->sub;
    if (sub->flags & 1) {
        u8 *actor;

        actor = sub->actor;
        if (*(s32 *)(actor + 0x1D4)) {
            u8 slot;

            slot = sub->slot;
            arg0->a = obj->a;
            arg0->b = obj->b;
            arg0->unk54 = sub->unk3C;
            arg0->unk58 = sub->unk40;
            func_15143134(D_800A1620[slot], &arg0->c, *(s32 *)(actor + 0x1D4) + (D_800A1638[slot] << 6));
            arg0->c.x = arg0->c.x - arg0->a.x;
            arg0->c.y = arg0->c.y - arg0->a.y;
            arg0->c.z = arg0->c.z - arg0->a.z;
            arg0->flags |= 2;
        } else {
            arg0->flags &= ~2;
        }
    } else {
        arg0->flags &= ~2;
    }
    return 1;
}


typedef struct {
    struct17 pos;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    s8 unk14;
    u8 unk15;
} Spawn150EFB80;

typedef struct {
    f32 unk0;
    f32 vel[3];
    f32 unk10;
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
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    s32 unk24;
    s32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    u8 unk32;
    f32 unk34;
    s8 unk38;
    u8 unk39;
} Part150EFB80;

typedef struct {
    u8 pad0[0x40];
    struct17 pos;
    u8 pad4C[0x128 - 0x4C];
    u8 flags;
} Obj150EFB80;

typedef struct {
    Obj150EFB80 *obj;
    f32 rate;
} Data150EFB80;

extern void func_15143794(s16, s16, f32, f32 *);
extern void func_1515C2F0(Spawn150EFB80 *, s32, Part150EFB80 *, s32, s32, s32);

void func_150EFB80(u8 *arg0) {
    Data150EFB80 *data;
    Obj150EFB80 *obj;
    u32 angle;
    u32 pitch;
    Spawn150EFB80 spawn;
    Part150EFB80 part;
    f32 sp88;

    data = (Data150EFB80 *)(arg0 + 0x28);
    data->rate += (364.0f + func_150ADA68() * 300.0f) * 0.001000000047f;
    if (data->rate > 1.0f) {
        obj = data->obj;
        if (obj->flags & 1) {
            spawn.pos = obj->pos;
            spawn.unkE = 5;
            spawn.unk10 = 0xA;
            part.unk14 = 4;
            part.unk15 = 2;
            part.unk16 = 3;
            part.unk17 = 0x9D;
            part.unk18 = 0x72;
            part.unk19 = 0x2E;
            part.unk1B = 0;
            part.unk1C = 0;
            part.unk1D = 0;
            part.unk1E = 0;
            part.unk1F = 0xFF;
            part.unk20 = 3;
            part.unk21 = 0x24;
            part.unk24 = 0x200005;
            part.unk28 = 0x60600;
            part.unk2C = 0xC;
            part.unk2E = 0x15;
            part.unk30 = 1;
            part.unk32 = 0;
            part.unk34 = 1.0f;
            part.unk38 = -1;
            part.unk39 = 0;
            do {
                spawn.unk15 = func_150ADA20() % 3U + 3;
                spawn.unkC = (func_150ADA20() & 0xF) + 0x28;
                part.unk0 = func_150ADA68() * 10.0f + 22.0f;
                angle = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794(angle & 0xFF, pitch % 31U - 55, func_150ADA68() * 15.0f + 25.0f, part.vel);
                part.unk10 = (func_150ADA68() * 740.0f + -1761.0f) * 0.001000000047f;
                part.unk1A = func_150ADA20() % 101U + 155;
                func_1515C2F0(&spawn, 0, &part, 0, arg0[0xC], arg0[1]);
                data->rate -= 1.0f;
            } while (data->rate > 1.0f);
        } else {
            do {
                data->rate -= 1.0f;
            } while (data->rate > 1.0f);
        }
    }
}

typedef struct {
    s32 unk00;
    u8 unk04;
    u8 unk05;
    u8 pad06[2];
    s32 unk08;
} Payload_150EFEC8;

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    f32 unk48;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
    s8 unk59;
    u8 pad5A[2];
} Struct98_150EFEC8;

typedef struct {
    u8 unk00;
    u8 unk01;
    s16 unk02;
    s16 unk04;
    u8 pad06[2];
    s32 unk08;
    s32 unk0C;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
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
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    u8 pad4C[0xC];
} Struct40_150EFEC8;

typedef struct {
    u8 pad_0[0x170];
    Payload_150EFEC8 field_0x170;
} CreatedObjectPayloadFields;

s32 func_150EFEC8(ActorIdByteFields *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, s32 arg5, u8 arg6, s32 arg7) {
    CreatedObjectPayloadFields *temp_v0;
    Struct98_150EFEC8 sp98;
    Struct40_150EFEC8 sp40;
    Payload_150EFEC8 sp34;

    sp34.unk00 = (s32)arg0;
    sp34.unk04 = arg0->field_0x3B;
    sp34.unk05 = arg1;
    sp34.unk08 = arg5;

    sp40.unk00 = D_8008FD00((s32)arg0, arg1);
    sp40.unk01 = 3;
    sp40.unk02 = 0x2203;
    sp40.unk04 = 0x12C;
    sp40.unk08 = 0;
    sp40.unk0C = 0;
    sp40.unk10 = arg2;
    sp40.unk11 = arg3;
    sp40.unk12 = arg4;
    sp40.unk13 = 0xFF;
    sp40.unk18 = 100.0f;
    sp40.unk14 = 100.0f;
    sp40.unk1C = 0.0f;
    sp40.unk20 = 0.0f;
    sp40.unk24 = 0.0f;
    sp40.unk28 = 0.0f;
    sp40.unk2C = 0.0f;
    sp40.unk30 = 0.0f;
    sp40.unk34 = 1.0f;
    sp40.unk38 = 1.0f;
    sp40.unk3C = 1.0f;
    sp40.unk40 = 0xCD2002;
    sp40.unk44 = 0xFF;
    sp40.unk45 = 0xFF;
    sp40.unk46 = 0;
    sp40.unk47 = 6;
    sp40.unk48 = 0;

    sp98.unk04 = 160.0f;
    sp98.unk00 = 160.0f;
    sp98.unk0C = 80.0f;
    sp98.unk08 = 80.0f;
    sp98.unk10 = 0.5f;
    sp98.unk14 = 0.5f;
    sp98.unk18 = 1.0f;
    sp98.unk1C = D_800A1830;
    sp98.unk20 = D_800A1834;
    sp98.unk24 = -1;
    sp98.unk34 = -1;
    sp98.unk28 = -1;
    sp98.unk38 = -1;
    sp98.unk2C = -1;
    sp98.unk3C = -1;
    sp98.unk30 = -1;
    sp98.unk40 = -1;
    sp98.unk44 = 0;
    sp98.unk48 = 1.0f;
    sp98.unk4C = 0;
    sp98.unk4E = 0;
    sp98.unk50 = 0;
    sp98.unk52 = 0;
    sp98.unk54 = 0xFF;
    sp98.unk55 = 0xFF;
    sp98.unk56 = 0xFF;
    sp98.unk57 = 0xFF;
    sp98.unk58 = 0xA;
    sp98.unk59 = -1;

    temp_v0 = func_151407D0(&sp98, 0x6C, &sp40, 0x1E, 0, 0, 0, -1, arg6, arg7);
    if (temp_v0 != 0) {
        memcpy(&temp_v0->field_0x170, &sp34, 0xC);
    }
    return (s32)temp_v0;
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Vec3w_150F00EC;

typedef struct {
    u8 pad0[0x18];
    u8 unk18;
    u8 pad19[0x17];
    f32 unk30;
    f32 unk34;
    f32 unk38;
} Sub110_150F00EC;

typedef struct {
    u8 pad0[0x34];
    Vec3w_150F00EC unk34;
    u8 pad40[0xD0];
} Sub178_150F00EC;

typedef struct {
    u8 pad0[0x34];
    Vec3w_150F00EC unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 pad4C[0xC];
    s32 unk58;
    u8 pad5C[0x11C];
    Sub178_150F00EC *unk178;
} Arg150F00EC;

s32 func_150F00EC(Arg150F00EC *arg0) {
    Sub178_150F00EC *temp_v0;
    Sub110_150F00EC *temp_v1;

    temp_v0 = arg0->unk178;
    temp_v1 = (Sub110_150F00EC *)(temp_v0 + 1);
    if (*(u8 *)((u8 *)temp_v0 + 0x128) & 1) {
        arg0->unk34 = temp_v0->unk34;
        arg0->unk40 = *(f32 *)&temp_v0->unk34.unk0 + (temp_v1->unk30 * 500.0f);
        arg0->unk44 = *(f32 *)&temp_v0->unk34.unk4 + (temp_v1->unk34 * 500.0f);
        arg0->unk48 = *(f32 *)&temp_v0->unk34.unk8 + (temp_v1->unk38 * 500.0f);
        arg0->unk58 |= 6;
    } else {
        arg0->unk58 &= ~4;
        arg0->unk58 &= ~2;
    }
    return 1;
}

struct225 *func_150F0198(u8 arg0, u8 arg1, u8 arg2, u8 arg3, s32 arg4, u8 arg5, s32 arg6) {
    struct225 *temp_v0;
    Header header;
    Header2 header2;
    s32 payload;

    payload = arg4;
    header.unk0 = 2;
    header.unk1 = -1;
    header.unk2 = 0x12C;
    header.unk4 = 0x21;
    header2.unk0 = 0;
    header2.unk4 = 0;
    header2.unk8 = 0;

    temp_v0 = func_151602C0(&header, &header2, arg0, arg1, arg2, arg3, 0xFF, 0, 4, arg5, arg6);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk18, &payload, 4);
    }
    return temp_v0;
}

typedef struct {
    u8 pad_0[0x12C];
    s32 field_0x12C[1];
} SlotTableFields;

typedef struct {
    u8 pad_0[0x8];
    SlotTableFields *field_0x8;
    u8 field_0xC;
} SlotTableLinkFields;

typedef struct {
    u8 pad_0[0x60];
    SlotTableLinkFields *field_0x60;
} SlotTableOwnerFields;

void func_150F02A0(SlotTableOwnerFields *arg0);

void func_150F0260(SlotTableOwnerFields *arg0) {
    func_150F02A0(arg0);
}

void func_150F0280(SlotTableOwnerFields *arg0) {
    func_150F02A0(arg0);
}

void func_150F02A0(SlotTableOwnerFields *arg0) {
    SlotTableLinkFields *temp = arg0->field_0x60;
    SlotTableFields *base = temp->field_0x8;
    base->field_0x12C[temp->field_0xC] = 0;
}

void func_150F0318(struct260 *arg0);

void func_150F02C0(struct260 *arg0) {
    func_150F0318(arg0);
    func_1514933C(arg0);
}

void func_150F02EC(struct260 *arg0) {
    func_150F0318(arg0);
    func_15149368(arg0);
}

void func_150F0318(struct260 *arg0) {
    s32 *t = *(s32 **)((u8 *)arg0 + 0x28);

    t[0x4D] = 0;
}

void func_150F0380(struct210 *arg0);

void func_150F0328(struct210 *arg0) {
    func_150F0380(arg0);
    func_151411A4(arg0);
}

void func_151411C4(struct210 *arg0);

void func_150F0354(struct210 *arg0) {
    func_150F0380(arg0);
    func_151411C4(arg0);
}

void func_150F0380(struct210 *arg0) {
    s32 *t = *(s32 **)((u8 *)arg0 + 0x178);

    t[0x4E] = 0;
}

void func_150F03E8(struct210 *arg0);
void func_151617C4(struct210 *arg0);

void func_150F0390(struct210 *arg0) {
    func_150F03E8(arg0);
    func_151617C4(arg0);
}

void func_151617E4(struct210 *arg0);

void func_150F03BC(struct210 *arg0) {
    func_150F03E8(arg0);
    func_151617E4(arg0);
}

void func_150F03E8(struct210 *arg0) {
    s32 *t = *(s32 **)((u8 *)arg0 + 0x18);

    t[0x4F] = 0;
}

typedef struct {
    u8 pad0[0x1D4];
    s32 unk1D4;
    u8 pad1D8[0x114];
    s32 unk2EC;
} Struct150F03F8;

extern f32 D_800A1640[][3];
extern f32 D_800A1658[][3];
extern u8 D_800A1670[];
extern s32 func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);
extern void func_15102B38(Struct150F03F8 *, u8, f32 *, f32 *, f32 *, s32, s32, f32, struct17 *, s32, s32, s32, u8, s32);
s32 func_151C229C(struct17 *arg0, struct17 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, Struct150F03F8 *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26);

s32 func_150F03F8(Struct150F03F8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 ret;
    struct17 pos;
    struct17 dir;
    s32 srcs[2];
    s32 dsts[2];
    Header hdr;
    Header2 hdr2;
    f32 sp98[2];
    s32 sp94;
    f32 rand1;
    f32 rand0;
    s32 sp88;
    u8 *sp84;

    if (arg0 == NULL) {
        return 0;
    }
    if (((u8 *)&arg1)[3] >= 2) {
        return 0;
    }
    if (arg0->unk1D4 == 0) {
        return 0;
    }
    if (arg0->unk2EC < 0x5A) {
        return 0;
    }
    srcs[0] = (s32)D_800A1640[((u8 *)&arg1)[3]];
    srcs[1] = (s32)D_800A1658[((u8 *)&arg1)[3]];
    dsts[0] = (s32)&pos;
    dsts[1] = (s32)&dir;
    sp88 = (D_800A1670[((u8 *)&arg1)[3]] << 6) + arg0->unk1D4;
    sp84 = &D_800A1670[((u8 *)&arg1)[3]];
    func_15145EA4(srcs, dsts, sp88, 2);
    dir.unk0 -= pos.unk0;
    dir.unk4 -= pos.unk4;
    dir.unk8 -= pos.unk8;
    {
        f32 func_150ADA68(void);

        rand0 = func_150ADA68();
        rand1 = func_150ADA68();
        func_150ADA20();
        ret = func_151C229C(&pos, &dir, 0, 0, 0, 0, 150.0f, 0.006666666828f, rand0 * 19.0f + 38.0f,
                            rand1 * 363.0f + 149.0f, 50.0f, 0xFF, arg0, 1, 1, 0, 8, 0, 1, 0, 0xA0038, 0.0f,
                            0xFF, -1, 0, ((u8 *)&arg2)[3], arg3);
    }
    hdr.unk0 = 3;
    hdr.unk1 = -1;
    hdr.unk2 = (func_150ADA20() % 5U) + 4;
    hdr.unk4 = 0;
    hdr2.unk0 = pos.unk0;
    hdr2.unk4 = pos.unk4;
    hdr2.unk8 = pos.unk8;
    sp88 = (func_150ADA20() % 51U) + 0x50;
    func_151602C0(&hdr, &hdr2, sp88, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,
                  ((u8 *)&arg2)[3], arg3);
    sp98[1] = (func_150ADA68() * 1906.0f + 969.0f) * 0.001000000047f;
    sp98[0] = (func_150ADA68() * 911.0f + 628.0f) * 0.1000000015f;
    sp88 = func_150ADA20();
    sp94 = func_150ADA20();
    rand0 = func_150ADA68() * 2000.0f + 2000.0f;
    func_15102B38(arg0, *sp84, D_800A1640[((u8 *)&arg1)[3]], D_800A1658[((u8 *)&arg1)[3]], sp98,
                  (sp88 % 3U) + 8, (sp94 % 1U) + 0xFF, rand0, &pos, 0xFF, 0, -1,
                  ((u8 *)&arg2)[3], arg3);
    return ret;
}

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_F07E4;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_F07E4;

void func_150F07E4(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_F07E4 *temp_v0 = *(SubA_F07E4 **)((u8 *)arg0 + 0x60);
    ArgB_F07E4 *b = (ArgB_F07E4 *)arg1;
    s32 b0;

    if (arg2 == 0) {
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
    s32 unk0;
    u8  unk4;
} SubA_F088C;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_F088C;

void func_150F088C(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_F088C *temp_v0 = (SubA_F088C *)((u8 *)arg0 + 0x170);
    ArgB_F088C *b = (ArgB_F088C *)arg1;
    s32 b0;

    if (arg2 == 0) {
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

void func_150F0938(s32 arg0) {
    func_15160A58(arg0, 0x25, &D_800A1674, 2, 0x12C, 4, 0, 0xFF, 0, 0xFF, 0, -1, 0, 0, 0xFF, 1);
    func_15160A58(arg0, 2, &D_800A1680, 2, 0x12C, 0xD, 0xFF, 0xFF, 0xFF, 0xFF, 0, -1, 0, 0, 0xFF, 1);
}

typedef struct { f32 x, y, z; } Vec3f_150F0A24;
typedef struct Game11D830EffectPacket {
    s32 field_0;
    s32 field_4;
    Vec3f_150F0A24 position;
    f32 field_14;
    f32 field_18;
    f32 field_1C;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    s16 field_2C;
    s16 field_2E;
    s16 field_30;
    s16 field_32;
    s32 field_34;
    s32 field_38;
    s16 field_3C;
    s16 field_3E;
    s16 field_40;
    u8 bytes_42[0x17];
    u8 pad59[3];
    s32 field_5C;
    s32 field_60;
    s16 field_64;
    s16 field_66;
    s16 field_68;
    s8 field_6A;
    u8 pad6B;
    f32 field_6C;
    s8 field_70;
    s8 field_71;
    s8 field_72;
    s8 field_73;
} Game11D830EffectPacket;

void func_15152B38(void *, s32, s32, void *);
extern f32 D_800A1858;
extern f32 D_800A185C;

void func_150F0A24(f32 *arg0) {
    Game11D830EffectPacket packet;

    packet.field_0 = 9;
    packet.field_4 = 6;
    packet.position = *(Vec3f_150F0A24 *)arg0;
    packet.field_14 = 14.88f;
    packet.field_18 = 14.88f;
    packet.field_1C = D_800A1858;
    packet.field_20 = D_800A185C;
    packet.field_24 = 39.0f;
    packet.field_28 = 35.0f;
    packet.field_2C = 0;
    packet.field_2E = 0xFF;
    packet.field_30 = -0x1F;
    packet.field_32 = 0x50;
    packet.field_34 = 3;
    packet.field_38 = 4;
    packet.field_3C = 0x14;
    packet.field_3E = 0x1E;
    packet.field_40 = 1;
    packet.bytes_42[0] = 4;
    packet.bytes_42[1] = 2;
    packet.bytes_42[2] = 3;
    packet.bytes_42[3] = 0xFF;
    packet.bytes_42[4] = 0xFF;
    packet.bytes_42[5] = 0xFF;
    packet.bytes_42[6] = 0xFF;
    packet.bytes_42[7] = 0;
    packet.bytes_42[8] = 0;
    packet.bytes_42[9] = 0;
    packet.bytes_42[0xA] = 0;
    packet.bytes_42[0xB] = 0xFF;
    packet.bytes_42[0xC] = 0xFF;
    packet.bytes_42[0xD] = 0xFF;
    packet.bytes_42[0xE] = 0xFF;
    packet.bytes_42[0xF] = 0;
    packet.bytes_42[0x10] = 0;
    packet.bytes_42[0x11] = 0;
    packet.bytes_42[0x12] = 0;
    packet.bytes_42[0x13] = 0xFF;
    packet.bytes_42[0x14] = 0;
    packet.bytes_42[0x15] = 3;
    packet.bytes_42[0x16] = 0x24;
    packet.field_5C = 0x200005;
    packet.field_60 = 0x60600;
    packet.field_64 = 8;
    packet.field_66 = 0x1F;
    packet.field_68 = 1;
    packet.field_6A = 0;
    packet.field_6C = 1.0f;
    packet.field_70 = -1;
    packet.field_71 = 0;
    packet.field_72 = -1;
    packet.field_73 = -1;
    func_15152B38(&packet, 0xFF, 1, arg0);
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
} Struct150F0BECParticle; /* 0x70 */

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern f32 D_800A1860;
extern f32 D_800A1864;
extern f32 D_800A1868;
extern f32 D_800A186C;

void func_150F0BEC(Struct150F0E48Src *arg0) {
    Struct150F0BECParticle spawn;
    Struct150F0E48Sub sub;
    void *ret;
    s32 temp_v0;
    s32 temp_v1;

    spawn.unk1D = 0x2B;
    spawn.unk8 = 0x4403;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0x70600;
    spawn.unkA = 0x12C;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk14 = 0xFF;
    spawn.unk15 = 0xFF;
    spawn.unk16 = 0xFF;
    spawn.unk17 = 0xFF;
    spawn.unk18 = 0xFF;
    spawn.unk19 = 0xD9;
    spawn.unk1A = 0xA0;
    spawn.unk1B = 0xFF;
    spawn.unk1C = 0xFF;
    spawn.unk2C = 250.0f;
    spawn.unk28 = 250.0f;
    spawn.pos.unk0 = arg0->unk14;
    spawn.pos.unk4 = arg0->unk18;
    spawn.pos.unk8 = arg0->unk1C;
    spawn.unk3C = *(struct17 *)&D_800A5480;
    spawn.vel = *(struct17 *)&D_800A5480;
    spawn.unk1E = 1;
    spawn.unk20 = 0xFF;
    spawn.unk22 = 1;
    spawn.unk54 = 0.0f;
    spawn.unk24 = 1.0f;
    if (func_150ADA20() & 1) {
        temp_v1 = 0x40;
    } else {
        temp_v1 = 0;
    }
    if (func_150ADA20() & 1) {
        temp_v0 = 0x80;
    } else {
        temp_v0 = 0;
    }
    spawn.unk58 = (temp_v0 | temp_v1) | 0x8000 | 0x4000 | 0x40000;
    spawn.unk60 = 6;
    spawn.unk61 = 6;
    spawn.unk62 = 0x25;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0xA;
    spawn.unk5C = 0;
    spawn.unk66 = 0xFF;
    spawn.unk68 = 0;
    sub.unk0 = arg0;
    spawn.unk6C = D_800A1860;
    sub.unk4 = arg0->unk3B;
    sub.unk8 = func_150ADA68() * 2.0f * D_800A1864;
    sub.unkC = func_150ADA68() * 2.0f * D_800A1868;
    sub.unk10 = func_150ADA68() * 2.0f * D_800A186C;
    ret = func_15130280(&spawn, 1, 0, 0x14, 0xFF, 1);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0xA8, &sub, sizeof(sub));
    }
}

s32 func_150F0E48(Struct150F0E48Obj *arg0, s32 arg1) {
    Struct150F0E48Sub *sub;
    Struct150F0E48Src *src;

    sub = (Struct150F0E48Sub *)((u8 *)arg0 + 0xA8);
    src = sub->unk0;
    if (src->unk3B != sub->unk4) {
        return 0;
    }

    arg0->unk40 = src->unk14;
    arg0->unk44 = src->unk18;
    arg0->unk48 = src->unk1C;
    sub->unk8 += D_800A1870 * D_800BE9A4;
    sub->unkC += D_800A1874 * D_800BE9A4;
    sub->unk10 += 0.25f * D_800BE9A4;
    sub->unk8 = func_15144B68(sub->unk8);
    sub->unkC = func_15144B68(sub->unkC);
    sub->unk10 = func_15144B68(sub->unk10);
    arg0->unk38 = (sinf(sub->unk8) * 243.0f) + 780.0f;
    arg0->unk3C = (sinf(sub->unkC) * 243.0f) + 780.0f;
    arg0->unk2B = (sinf(sub->unk10) * 50.0f) + 200.0f;
    return 1;
}

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA150F1020;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB150F1020;

void func_150F1020(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA150F1020 *temp_v0 = (SubA150F1020 *)((u8 *)arg0 + 0xA8);
    ArgB150F1020 *b = (ArgB150F1020 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else {
            if (temp_v0->unk0 == b->u4.w4) {
                temp_v0->unk0 = b->unk0;
                temp_v0->unk4 = b->unk8;
            }
        block_150F1020: ;
        }
    } else if (arg2 == 0 || arg2 == 0x43) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}

void func_150F10D4(struct210 *arg0) {
    s8 sp40[0x18];
    struct260 *temp_v0;

    *(struct210 **)&sp40[0] = arg0;
    sp40[4] = *(u8 *)((s32)arg0 + 0x3B);
    *(f32 *)&sp40[8] = 0.0f;
    *(f32 *)&sp40[0xC] = *(f32 *)((s32)arg0 + 0x14);
    *(f32 *)&sp40[0x10] = *(f32 *)((s32)arg0 + 0x18);
    *(f32 *)&sp40[0x14] = *(f32 *)((s32)arg0 + 0x1C);

    temp_v0 = func_15149130(0x12C, -1, 0x4C, -1, 0, 0x3A, (struct37 *)0x18, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp40, 0x18);
    }
}

typedef struct {
    Struct150F0E48Src *unk0;
    u8 unk4;
    u8 pad5[3];
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} Link150F1170;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD;
    s16 unkE;
    u8 pad10[0x18];
    Link150F1170 unk28;
} Obj150F1170;

typedef struct {
    u8 unk0;
    u8 unk1;
    s8 unk2;
    s8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} Sub150F1170;

void func_150F1170(Obj150F1170 *arg0) {
    Link150F1170 *sub;
    Struct150F0E48Src *src;
    struct17 pos;
    f32 dx;
    f32 dy;
    f32 dz;
    Struct150F0BECParticle spawn;
    Sub150F1170 ext;
    void *ret;
    s32 a;

    src = arg0->unk28.unk0;
    pos = *(struct17 *)&arg0->unk28.unkC;
    sub = &arg0->unk28;
    if ((*(s32 *)src == 0) || (src->unk3B != sub->unk4)) {
        arg0->unkE = -1;
        return;
    }
    sub->unkC = src->unk14;
    sub->unk10 = src->unk18;
    sub->unk14 = src->unk1C;
    dx = sub->unkC - pos.unk0;
    dy = sub->unk10 - pos.unk4;
    dz = sub->unk14 - pos.unk8;
    sub->unk8 += (0.1090000048f + func_150ADA68() * 0.2840000093f) * D_800BE9A4;
    if (sub->unk8 > 1.0f) {
        {
        s32 pad;
        }
        ext.unkC = 0.986371994f;
        spawn.unk1D = 0x6C;
        spawn.unk8 = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.unk1E = 0x23;
        spawn.unk20 = 7;
        spawn.unk58 = 0x90DE07;
        spawn.unk60 = 8;
        spawn.unk61 = 6;
        spawn.unk62 = 0x20;
        spawn.unk63 = -1;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk22 = 0x23;
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
        spawn.unk24 = 1.029834032f;
        do {
            {
            f32 r;
            r = func_150ADA68();
            spawn.pos.unk0 = dx * r + pos.unk0;
            spawn.pos.unk4 = dy * r + pos.unk4;
            spawn.pos.unk8 = dz * r + pos.unk8;
            }
            spawn.vel.unk0 = dx * D_800BE9A8 * 0.3330000043f;
            spawn.vel.unk4 = dy * D_800BE9A8 * 0.3330000043f;
            spawn.vel.unk8 = dz * D_800BE9A8 * 0.3330000043f;
            ext.unk0 = func_150ADA20();
            ext.unk1 = (s16)func_150ADA20();
            ext.unk2 = (func_150ADA20() % 5U) + 4;
            ext.unk3 = (func_150ADA20() % 5U) + 4;
            ext.unk4 = func_150ADA68() * 39.0f;
            ext.unk8 = func_150ADA68() * 39.0f;
            spawn.unk54 = func_150ADA68() * 0.5139999986f + 0.6560000181f;
            spawn.unk58 &= ~0xC0;
            a = (func_150ADA20() & 1) ? 0x80 : 0;
            spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | a;
            spawn.unk1B = (func_150ADA20() % 71U) + 0x50;
            spawn.unkA = (func_150ADA20() % 31U) + 0x3C;
            spawn.unk28 = spawn.unk2C = func_150ADA68() * 514.0f + 404.0f;
            ret = func_15130280(&spawn, 1, 0, 0x10, arg0->unkC, arg0->unk1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0xA8, &ext, sizeof(ext));
            }
            sub->unk8 -= 1.0f;
        } while (sub->unk8 > 1.0f);
    }
}

void func_150F15F8(s32 arg0, s32 arg1, u8 arg2) {
    s32 *p = &arg0;
    s32 q;
    s32 b0;

    if (arg2 == 0x43) {
        b0 = *(s32 *)arg1;
        q = *p + 0x28;
        if ((b0 == *(s32 *)q) || (*(u8 *)(q + 4) == *(u8 *)(*(s32 *)&arg1 + 4))) {
            func_1516972C((struct102 *)*p);
        }
    } else {
        q = *p + 0x28;
        func_15149514(arg1, arg2, q, q + 4, *p);
    }
}

struct func150F1684_sub {
    s32 unk0;
    u8 unk4;
};

void func_150F1684(struct func150F1684_sub *arg0, struct func150F1684_sub *arg1, u8 arg2) {
    struct func150F1684_sub *temp = (struct func150F1684_sub *)((s32)arg0 + 0x18);
    s32 word;

    if (arg2 == 0x43) {
        word = arg1->unk0;
        if ((temp->unk0 == word) || (arg1->unk4 == temp->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_11C2B0/func_150F16DC.s")

typedef struct {
    u16 v[3];
} Rates150F1A00;

typedef struct {
    u8 pad0[0x14];
    s16 unk14;
} Timer150F1A00;

typedef struct {
    u8 pad0[0xF80];
    f32 m[4][4];
} Model150F1A00;

typedef struct {
    u8 pad0[0x1D4];
    Model150F1A00 *unk1D4;
    u8 pad1D8[0x10C];
    s32 unk2E4;
} Actor150F1A00;

extern Rates150F1A00 D_80088B00;
extern f32 D_800A18E8;
extern f32 D_800A18EC;
extern f32 D_800A18F0;
f32 cosf(f32);

void func_150F1A00(Actor150F1A00 *arg0) {
    Model150F1A00 *model;
    Rates150F1A00 rates;
    s32 idx;
    Timer150F1A00 *timer;
    u16 rate;
    f32 scale;
    s32 i;
    s32 j;
    f32 *row;

    rates = D_80088B00;
    idx = 0;
    if ((arg0->unk2E4 & 3) == 3) {
        idx = 1;
    }
    if ((arg0->unk2E4 & 0xC) == 0xC) {
        idx = 2;
    }
    rate = rates.v[idx];
    timer = (Timer150F1A00 *)&D_800CC298;
    timer->unk14 += rate * D_800BE9E4;
    scale = (((cosf(timer->unk14 * D_800A18E8) + 1.0f) * 0.5f) * D_800A18EC) + D_800A18F0;
    model = arg0->unk1D4;
    for (i = 0, row = model->m[0]; i < 3; i++, row += 4) {
        for (j = 0; j < 3; j++) {
            row[j] *= scale;
        }
    }
}

typedef struct {
    u8 pad0[0x12];
    s16 unk12;
} Tilt150F1B48;

/* D_800CC298 is typed s32 in variables.h; this function sees it as a tilt record. */
#define TILT ((Tilt150F1B48 *)&D_800CC298)

extern f32 D_800A18F4;
extern f32 D_800A18F8;

s16 *func_150F1B48(s16 *arg0, s32 arg1) {
    s32 down;
    s32 step;
    s32 v;
    s32 i;

    down = (D_800CC2D0[arg1].unk84.uh == 0x21) ? 1 : 0;
    step = D_800BE9E4 * 0x222;
    if (down) {
        v = TILT->unk12 - step;
        if (D_800A18F4 < v) {
            TILT->unk12 = v;
        } else {
            TILT->unk12 = -0x1999;
        }
    } else {
        v = TILT->unk12 + step;
        if (v < D_800A18F8) {
            TILT->unk12 = v;
        } else {
            TILT->unk12 = 0x1555;
        }
    }
    for (i = 0; i < 6; i++) {
        *arg0++ = 0xB2 + i * 6;
        *arg0++ = TILT->unk12;
    }
    return arg0;

}

void func_150F1CB0(ActorStateFields *arg0) {
    if (arg0->field_0x84 == 0x14) {
        arg0->field_0x68 = 0x1B;
    } else {
        arg0->field_0x68 = 0xC;
    }
    arg0->field_0x69 = 0x13;
    if ((arg0->field_0x2E4 & 0x3) == 0x3) {
        arg0->field_0x69 = 0x14;
    }
    if ((arg0->field_0x2E4 & 0xC) == 0xC) {
        arg0->field_0x69 = 0x17;
    }
}

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
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ u8 unk40;
} Cfg150F1D10A;

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
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
} Cfg150F1D10B;

extern s32 D_800A18C0[];
extern f32 D_800A18CC[];
extern s32 D_800A18D8[];
extern f32 D_800A18E0[];

void func_151D5334(struct17 *, f32, f32, f32, s32, u8, s32);
void func_151D5514(struct17 *, u8, s32);
void func_151D3FF4(struct17 *, u8, s32);
void func_151541B8(struct17 *, f32, f32, f32, f32, u8, s32);
void func_1515080C(Cfg150F1D10A *, s32 *, f32 *, s32, s32, s32, s32, s32, s32, s32, f32, s32, s32, u8, s32);
void func_15152190(Cfg150F1D10B *, s32 *, f32 *, s32, f32, s32, u8, s32);

void func_150F1D10(struct127 *arg0, u8 arg1, s32 arg2) {
    struct17 pos;
    f32 scale;

    if (arg0 == NULL) {
        return;
    }
    pos.unk0 = arg0->x_position;
    pos.unk4 = arg0->y_position + 20.0f;
    pos.unk8 = arg0->z_position;
    scale = (arg0->xz_scale + arg0->y_scale) * 0.5f;
    func_151D5404(&pos, 506.0f, 1013.0f, 0.0009871669f, 0xF, 0x14, arg1, arg2);
    func_151D5334(&pos, 506.0f, 1013.0f, 0.0009871669f, 5, arg1, arg2);
    func_151D5514(&pos, arg1, arg2);
    func_10010F88((func_150ADA20() & 8) + 0x2B6, 0x7FFF, 0, 0, 0, pos.unk0, pos.unk4, pos.unk8, 0xFA0, 0x1770);
    func_151D3FF4(&pos, arg1, arg2);
    func_151541B8(&pos, func_150ADA68() * 4.0f + 12.0f, 1.6409999f,
                  (f32)(u32)(func_150ADA20() % 0x38U + 0xC8), 0.0f, arg1, arg2);
    {
        Cfg150F1D10A a;
        Cfg150F1D10B b;

    a.unk0 = 3;
    a.unk4 = 0;
    a.pos = pos;
    a.unk1C = 30.0f;
    a.unk20 = 20.0f;
    a.unk24 = -1.855000138f;
    a.unk28 = 0.6980000138f;
    a.unk14 = 0;
    a.unk16 = 0xFF;
    a.unk18 = -0x40;
    a.unk1A = 0x2C;
    a.unk2C = 0x3C;
    a.unk2E = 0x14;
    a.unk30 = 1 * scale;
    a.unk34 = 0 * scale;
    a.unk38 = 8.990000725f;
    a.unk3C = 0.0f;
    a.unk40 = 0xC;
    func_1515080C(&a, D_800A18C0, D_800A18CC, 3, 0, 0, 0, 1, -1, 0, 0.0f, 0, 0, arg1, arg2);
    b.unk0 = 5;
    b.unk4 = 3;
    b.pos = pos;
    b.unk1C = 25.0f;
    b.unk20 = 15.0f;
    b.unk14 = 0;
    b.unk16 = 0xFF;
    b.unk18 = -0x40;
    b.unk1A = 0x1E;
    b.unk2C = 0x46;
    b.unk2E = 0x14;
    b.unk24 = -1.239000082f;
    b.unk28 = 0.3760000169f;
    b.unk30 = 0.503000021f;
    b.unk34 = 0.2560000122f;
    b.unk38 = 13.93300056f;
    func_15152190(&b, D_800A18D8, D_800A18E0, 2, 0.0f, 0, arg1, arg2);
    }
}

extern u8 D_800D98F0;
extern f32 D_800A191C;

void func_150F20F0(s32 arg0) {
    f32 x;
    f32 z;
    s32 vol;
    s32 sp48;
    s32 sp44;
    struct127 *obj;

    obj = &D_800CC2D0[arg0];
    x = obj->x_position;
    z = obj->z_position;
    x *= x;
    z *= z;
    /* squares the already-squared terms: compares x^4 + z^4, as golden does */
    x = (x * x) + (z * z);
    if (D_800D98F0 != 0) {
        if (D_800BE9E4 < D_800D98F0) {
            D_800D98F0 -= D_800BE9E4;
        } else {
            D_800D98F0 = 0;
        }
    } else if (x > D_800A191C) {
        func_100114D0(0, (s32)D_800CC2D0[arg0].y_position, 0, 0x7FFF, 0x4E20, 0x4A38, &vol, &sp48, &sp44);
        vol += 0x80;
        D_800D98F0 = (func_150ADA20() & 0x7F) + 0x80;
        func_10010F30(((u32)func_150ADA20() % 3) + 0x6C, 0x5DC0, vol, 0, 0);
    }
}

void func_150F2230(struct127 *arg0, s32 arg1, s32 arg2) {
    struct { f32 x; f32 y; f32 z; } sp2C;
    struct { f32 x; f32 y; f32 z; } sp20;

    if ((arg0->unk1D4 != NULL) && ((arg0->unk74 & 0xF) != 0xF)) {
        sp2C.x = 0.0f;
        sp2C.y = 0.0f;
        sp2C.z = 0.0f;
        func_15143874((s16)(func_150ADA20() & 0xFF), 100.0f, &sp2C.x, &sp2C.z);
        func_15143134(&sp2C.x, &sp20.x, (s32)arg0->unk1D4 + 0x4C0);
        func_151C329C(&sp20.x, (u8)arg1, arg2);
    }
}
