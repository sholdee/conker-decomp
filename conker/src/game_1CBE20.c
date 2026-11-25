#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* First argument of func_15152190 (as Arg15108120 in game_1355D0.c). */
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
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
} Arg15152190;

void func_15152190(Arg15152190 *, s32 *, f32 *, s32, f32, s32, s32, s32);

/* Owner of a func_1519E970 timer (EvNodeData in game_50D80.c): unk30 holds the running timer. */
typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ struct EvTimer *timer;
} EvTimerOwner;

/* Effect created by func_1519E970 (0x2C bytes), counted down by func_1519EA04. */
typedef struct EvTimer {
    /* 0x00 */ u8 pad0[0x10]; // effect header, see func_15167A68
    /* 0x10 */ s32 flags;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ f32 *unk18;
    /* 0x1C */ f32 *unk1C;
    /* 0x20 */ s16 timeLeft;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ EvTimerOwner *owner;
    /* 0x28 */ u8 keepOwnerLink; /* 0: clear owner->timer on expiry */
    /* 0x29 */ u8 pad29[3];
} EvTimer; // size 0x2C

extern void *func_15167A68(s32, s32, s32, s32, u8, s32);

/* Starts a timer effect that expires after `duration` frames of D_800BE9E4. */
EvTimer *func_1519E970(s16 duration, EvTimerOwner *owner, u8 keepOwnerLink, f32 *arg3, f32 *arg4, u8 arg5,
                       s32 arg6) {
    EvTimer *timer = func_15167A68(0x26, arg6, sizeof(EvTimer), 1, arg5, 1);

    if (timer == NULL) {
        return NULL;
    }
    timer->unk18 = arg3;
    timer->unk1C = arg4;
    timer->timeLeft = duration;
    timer->keepOwnerLink = keepOwnerLink;
    timer->owner = owner;
    timer->flags = 1;
    timer->unk14 = 0;
    return timer;
}

/* Per-frame update of a func_1519E970 timer: frees it (unlinking its owner) once its time runs out. */
void func_1519EA04(EvTimer *timer) {
    if (timer->flags & 1) {
        s32 expired = FALSE;

        timer->timeLeft -= D_800BE9E4;
        if (timer->timeLeft < 0) {
            expired = TRUE;
        }
        if (expired) {
            if (timer->keepOwnerLink == 0) {
                EvTimerOwner *owner = timer->owner;
                owner->timer = NULL;
            }
            func_1516972C((struct102 *)timer);
        }
    }
}

/* Spawns a func_15152190 effect at pos (cutscene event; the caller passes an id < 0xE9 and a scale). */
void func_1519EA78(struct17 *pos, u16 id, f32 scale, u8 arg3, s32 arg4) {
    Arg15152190 def;
    s32 idArg;
    f32 scaleArg;

    def.unk00 = 10;
    def.unk04 = 7;
    def.pos = *pos;
    def.unk14 = 0;
    def.unk16 = 0xFF;
    def.unk18 = -0x35;
    def.unk1A = 0x18;
    def.unk1C = 10.0f;
    def.unk20 = 8.0f;
    def.unk24 = -1.32400012f;
    def.unk28 = 0.7440000176f;
    def.unk2C = 50;
    def.unk2E = 20;
    def.unk30 = 0.6000000238f;
    def.unk34 = 0.8000000119f;
    def.unk38 = 26.18200111f;
    idArg = id;
    scaleArg = scale;
    func_15152190(&def, &idArg, &scaleArg, 1, 0.0f, 0, arg3, arg4);
}

void *func_1513264C(void *, s32, s32, s32, s32, u8, s32);

void func_1519EB8C(f32 *arg0, u16 arg1, s16 arg2, u8 arg3, s32 arg4) {
    typedef struct { s32 words[3]; } Copy3;
    struct {
        f32 *owner;
        f32 values[13];
        Copy3 copy38;
        Copy3 copy44;
        f32 field50;
        s32 field54;
        s16 field58;
        u16 field5A;
        u8 field5C;
        u8 pad5D[3];
        s32 field60;
        u8 bytes64[9];
        u8 pad6D;
        u8 field6E;
        u8 pad6F;
        s32 field70;
        u8 field74;
        u8 pad75;
        s16 field76;
        s16 field78;
        u8 pad7A[2];
        s32 field7C;
    } packet;
    void *result;

    packet.owner = arg0;
    packet.values[0] = 1.0f;
    packet.values[1] = 1.0f;
    packet.values[2] = arg0[6] * 0.1f;
    packet.values[3] = arg0[7] * 0.1f;
    packet.values[4] = arg0[3];
    packet.values[5] = arg0[4];
    packet.values[6] = arg0[5];
    packet.values[7] = 1.0f;
    packet.values[8] = 1.0f;
    packet.values[9] = 1.0f;
    packet.values[10] = arg0[0];
    packet.values[11] = arg0[1];
    packet.values[12] = arg0[2];
    packet.copy38 = *(Copy3 *)&D_800A5480;
    packet.copy44 = *(Copy3 *)&D_800A5480;
    packet.field50 = 0.0f;
    packet.field54 = 0x980;
    packet.field58 = arg2;
    packet.field5A = arg1;
    packet.field5C = 0;
    packet.field60 = 0;
    packet.bytes64[0] = 0xFF;
    packet.bytes64[1] = 0x15;
    packet.bytes64[2] = 0;
    packet.bytes64[3] = 0;
    packet.bytes64[4] = 0;
    packet.bytes64[5] = 0;
    packet.bytes64[6] = 0;
    packet.bytes64[7] = 0;
    packet.bytes64[8] = 2;
    packet.field6E = 0;
    packet.field70 = 0;
    packet.field74 = 0;
    packet.field76 = 1;
    packet.field78 = 0xFF;
    packet.field7C = 0;
    result = func_1513264C(&packet.values[0], 3, 0xFF, 0, 4, arg3, arg4);
    if (result != 0) {
        memcpy((u8 *)result + 0x170, &packet.owner, 4);
    }
}

struct Src1519ED24 {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
};

struct Obj1519ED24 {
    char pad0[0x18];
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    char pad2C[0x38 - 0x2C];
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    char pad44[0x170 - 0x44];
    /* 0x170 */ struct Src1519ED24 *unk170;
};

extern f32 D_800A8CD8;

/* scale is read once into a local; src lives in a nested block (a -g3 scheduling barrier),
 * which is what lets the scale load win the cycle-0 tie against the arg0->unk170 load. */
s32 func_1519ED24(struct Obj1519ED24 *arg0) {
    f32 scale;

    scale = D_800A8CD8;
    {
        struct Src1519ED24 *src = arg0->unk170;

        arg0->unk18 = src->unk18 * scale;
        arg0->unk1C = src->unk1C * scale;
        arg0->unk20 = src->unk0C;
        arg0->unk24 = src->unk10;
        arg0->unk28 = src->unk14;
        arg0->unk38 = src->unk00;
        arg0->unk3C = src->unk04;
        arg0->unk40 = src->unk08;
        return 1;
    }
}

typedef struct {
    /* 0x00 */ u8  unk0;
    /* 0x01 */ u8  unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8  pad6[2];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  unk11;
    /* 0x12 */ u8  unk12;
    /* 0x13 */ u8  unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  unk45;
    /* 0x46 */ u8  unk46;
    /* 0x47 */ u8  unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8  unk4C;
    /* 0x4D */ u8  pad4D[3];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} Struct1519ED84;

void func_1519ED84(f32 *arg0, s32 arg1, s16 arg2, u8 arg3, s32 arg4) {
    Struct1519ED84 sp48;
    s32 sp44;
    void *temp_v0;

    sp44 = (s32)arg0;
    sp48.unk0 = arg1;
    sp48.unk1 = 0;
    sp48.unk2 = 0x3B03;
    sp48.unk4 = arg2;
    sp48.unk8 = 0;
    sp48.unkC = 0;
    sp48.unk10 = 0xFF;
    sp48.unk11 = 0xFF;
    sp48.unk12 = 0xFF;
    sp48.unk13 = 0xFF;
    sp48.unk14 = arg0[6] * 10.0f;
    sp48.unk18 = arg0[7] * 10.0f;
    sp48.unk1C = arg0[0];
    sp48.unk20 = arg0[1];
    sp48.unk24 = arg0[2];
    sp48.unk28 = arg0[3];
    sp48.unk2C = arg0[4];
    sp48.unk30 = arg0[5];
    sp48.unk34 = 1.0f;
    sp48.unk38 = 1.0f;
    sp48.unk3C = 1.0f;
    sp48.unk40 = 0x45C0081;
    sp48.unk44 = 0xFF;
    sp48.unk45 = 0xFF;
    sp48.unk46 = 0;
    sp48.unk47 = 7;
    sp48.unk48 = 0;
    sp48.unk4C = 0xFF;
    sp48.unk50 = 0;
    sp48.unk54 = 1;
    sp48.unk56 = 0xFF;

    temp_v0 = func_1513D2F0(&sp48, &D_800A4AA0, 0x27, 0, 0, 0x17, 0, 3, 0xFF, 4, arg3, arg4);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x110, &sp44, 4);
    }
}

struct Sub1519EF04 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

struct Obj1519EF04 {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    char pad4C[0x110 - 0x4C];
    struct Sub1519EF04 *unk110;
};

s32 func_1519EF04(struct Obj1519EF04 *a0) {
    struct Sub1519EF04 *v1 = a0->unk110;

    a0->unk2C = v1->unk18 * 10.0f;
    a0->unk30 = v1->unk1C * 10.0f;
    a0->unk40 = v1->unkC;
    a0->unk44 = v1->unk10;
    a0->unk48 = v1->unk14;
    a0->unk34 = v1->unk0;
    a0->unk38 = v1->unk4;
    a0->unk3C = v1->unk8;
    return 1;
}
