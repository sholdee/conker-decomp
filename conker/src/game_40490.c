#include <ultra64.h>

/* func_15149130's seventh argument is the size of the effect data, not a struct37 pointer. */
#define func_1000FA64 func_1000FA64_narrow_proto
#define func_15149130 func_15149130_struct37_decl_in_functions_h
#define func_15144598 func_15144598_header
#define func_15015300 func_15015300_header
#include "functions.h"
#undef func_1000FA64
#undef func_15149130
#undef func_15144598
#undef func_15015300
struct260 *func_15149130(s16 duration, s8 arg1, s8 arg2, s8 arg3, u8 arg4, u8 arg5, s32 dataSize, u8 owner, s32 kind);
#define D_800D3098 D_800D3098_header
#define D_800D9A20 D_800D9A20_header
#define D_800D9A24 D_800D9A24_header
#define D_800D9AA0 D_800D9AA0_header
#define D_800E0900 D_800E0900_header
#include "variables.h"
#undef D_800D3098
#undef D_800D9A20
#undef D_800D9A24
#undef D_800D9AA0
#undef D_800E0900

/* A level object placement: one 0x34-byte entry of the table D_800D3098 points to. The object's
 * type (unk15 >> 2) selects its spawn handler from D_80082E30. */
typedef struct LevelObject {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 sizeX;
    /* 0x08 */ s16 sizeY;
    /* 0x0A */ s16 sizeZ;
    /* 0x0C */ f32 rotX;
    /* 0x10 */ f32 rotY;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15; /* type << 2 | shape */
    /* 0x16 */ u8 flags;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u32 unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ u8 pad24[0x10];
} LevelObject;

extern LevelObject *D_800D3098;
/* Level objects registered by their spawn handlers (the header types them as struct134). */
extern LevelObject *D_800D9A20;
extern LevelObject *D_800D9A24;
extern LevelObject *D_800D9AA0[3];
extern LevelObject *D_800E0900[6];
f32 func_15144598(LevelObject *obj);

void func_151A9390(s32 arg0, s32 arg1, struct LevelObject *arg2, s32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8);
extern s32 (*D_80082E30[])(LevelObject *);
extern void (*D_80082F28[])(struct LevelObject *, f32);
extern f32 D_80096650;
extern f32 D_80096648;
extern f32 D_8009664C;
extern f32 D_80096654;
extern f32 D_80096658;
extern f32 D_8009665C;
extern f32 D_80096660;
extern f32 D_80096664;
extern f32 D_80096668;
extern f32 D_8009666C;
extern f32 D_80096670;
extern f32 D_80096674;
extern f32 D_80096678;
extern f32 D_80096680;
extern f32 D_80096684;
extern u8 D_800C35E8;
extern void func_1510F800(s32);
s32 func_151BE850(struct17 *arg0, f32 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6);
s32 func_15149550(void *, s32, s32, u8, u8, s32);
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
typedef struct {
    s8 unk0;
    u8 pad1[3];
    struct17 unk4;
    struct17 unk10;
    f32 unk1C;
    s8 unk20;
    u8 pad21[3];
} Struct150156F4Payload;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 unk33;
    u8 unk34;
    u8 pad35[3];
} Struct150139ACEntry;

typedef struct {
    struct LevelObject *unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 pad43;
    u16 unk44;
    u8 pad46[2];
    f32 unk48;
    u8 pad4C[0x14];
    s32 unk60;
    u8 unk64;
    u8 unk65;
    u8 pad66[2];
    s32 unk68;
    s32 unk6C;
} Struct150139ACLocal;

extern Struct150139ACEntry D_80095FA0[];

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29;
    s16 unk2A;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    s32 unk30;
    u8 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
} Struct15013DE8;


void func_15012FE0(void) {
    D_800BE570 = 0;
    D_800BE574 = 0;
    D_800BE575 = 0;
}

/** Runs the spawn handler of every level object, chosen by the object's type. */
void func_15013000(void) {
    u32 i;

    D_800DCDC4 = 0;
    D_800DCD90 = 0.0f;
    // matching: the label ends the block, so IDO cannot hoist the D_800D3094 load above the stores
start:
    for (i = 0; i < D_800D3094; i++) {
        u8 type = D_800D3098[i].unk15 >> 2;
        s32 (*handler)(LevelObject *) = D_80082E30[type];

        if (handler != NULL) {
            handler(&D_800D3098[i]);
        }
    }
}

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
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 endX;
    f32 endY;
    f32 endZ;
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
} Struct54_150130B4;

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
    u8 pad24[0x24];
    f32 unk48;
    u8 pad4C[0x8];
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
    u8 pad59[3];
} StructAC_150130B4;


typedef struct {
    f32 unk0;
    f32 unk4;
    struct131 *unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} Payload18_150130B4;

extern s32 D_8009658C[];
extern s32 D_800965A0[];
extern s32 D_800965B0[];
void *func_151407D0(void *, s32, void *, s32, s32, s32, s32, s32, s32, s32);
extern struct131 *func_151149AC(u8);

/** Spawns the particle emitter a level object describes; unk1C selects the kind of emitter. */
s32 func_150130B4(LevelObject *obj) {
    StructAC_150130B4 emit;
    Struct54_150130B4 spawn;
    s32 kind;
    s32 texture;
    u8 onGround;
    f32 mtx[4][4];

    obj->flags |= 4;
    onGround = (obj->unk20 >> 24) & 0xFF;
    kind = obj->unk1C;
    // matching: the labels split basic blocks, which as1 schedules separately
start:
    spawn.posX = obj->x;
    spawn.posY = obj->y;
    spawn.posZ = obj->z;
    func_150A8050(mtx, obj->rotX, obj->rotY, 0.0f);
    func_150A7960(mtx[0], 0.0f, 0.0f, -500.0f, &spawn.endX, &spawn.endY, &spawn.endZ);
    spawn.endX += spawn.posX;
    spawn.endY += spawn.posY;
    spawn.endZ += spawn.posZ;
    // matching: the typedef-only block scope changes how as1 schedules these stores
    {
        typedef s32 PadT;

        emit.unk04 = obj->sizeX;
        emit.unk00 = emit.unk04;
        emit.unk0C = obj->sizeY;
        emit.unk08 = emit.unk0C;
        emit.unk18 = ((obj->unk20 >> 8) & 0xFF) * 0.003921568859f;
    }
    emit.unk1C = (obj->unk20 & 0xFF) * 0.003921568859f;
    texture = (obj->unk18 >> 24) & 0xFF;
    spawn.unk00 = D_8009658C[texture];
    spawn.unk02 = D_800965A0[texture] + 3;
    spawn.unk04 = 300;
    spawn.unk08 = 0;
    spawn.unk0C = 0;
    spawn.unk10 = obj->unk18 & 0xFF;
    spawn.unk11 = (obj->unk18 >> 8) & 0xFF;
    spawn.unk12 = (obj->unk18 >> 16) & 0xFF;
    spawn.unk13 = 0xFF;
    spawn.unk14 = 0.0f;
    spawn.unk18 = 0.0f;
    spawn.unk34 = 1.0f;
    spawn.unk38 = 1.0f;
    spawn.unk3C = 1.0f;
    spawn.unk45 = 0xFF;
    spawn.unk44 = 0xFF;
    spawn.unk46 = 0;
    spawn.unk47 = 6;
    spawn.unk01 = 3;
    emit.unk54 = 0;
    emit.unk55 = 0;
    emit.unk56 = 0;
    emit.unk57 = 0;
    emit.unk48 = 1.0f;
    emit.unk10 = 0.4099999964f;
    emit.unk14 = 0.200000003f;
    emit.unk20 = 9.999999975e-07f;
    if (onGround) {
ground:
        func_1510F800(0);
        spawn.unk48 = func_1510FD20((s32)spawn.posX, (s32)spawn.posZ);
    } else {
        spawn.unk48 = 0;
        onGround = FALSE; // matching: redundant store, but IDO's allocation needs it
    }
    emit.unk58 = 0;
    spawn.unk40 = 0x2006 | (((obj->unk20 >> 16) & 0xFF) ? 0x10 : 0) | (onGround ? 0x01000000 : 0) | 0x40000 | 0x80000 | 0x10000 | 0x400000;
    switch (kind) {
    case 8: {
        f32 extra[4];
        void *particle;

        extra[0] = 0.9750000238f;
        extra[1] = 0.3249999881f;
        extra[2] = 0.0f;
        extra[3] = 0.05959999934f;
        particle = func_151407D0(&emit, 0x70, &spawn, 0, 0x29, 0, 0, -1, 0xFF, 1);
        if (particle != NULL) {
            memcpy((u8 *)particle + 0x170, extra, sizeof(extra));
        }
        break;
    }
    case 1:
    case 2:
    case 3:
    case 4: {
        void *unused; // matching: the dead store of the result steers as1's scheduling

        emit.unk58 = D_800965B0[kind];
        unused = func_151407D0(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    }
    case 9:
    case 10:
    case 11:
    case 12: {
        void *unused; // matching: as above

        emit.unk58 = D_800965A0[kind];
        unused = func_151407D0(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    }
    case 5:
    case 6:
    case 7: {
        s32 ids[3] = { 12, 13, 14 };

        emit.unk58 = ids[kind - 5];
        func_151407D0(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    }
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18: {
        void *particle;
        Payload18_150130B4 extra;
        u8 ids[6] = { 2, 3, 4, 5, 6, 7 };

        extra.unk8 = func_151149AC(ids[kind - 13]);
        if (extra.unk8 == NULL) {
            return 1;
        }
        extra.unk0 = -80.0f;
        extra.unk4 = fabsf(-0.01250000019f);
        extra.unkC = extra.unk8->unk10;
        extra.unk10 = extra.unk8->unk12;
        extra.unk14 = extra.unk8->unk14;
        emit.unk58 = 0;
        particle = func_151407D0(&emit, 0x78, &spawn, 0x25, 0, 0, 0, -1, 0xFF, 1);
        if (particle != NULL) {
            memcpy((u8 *)particle + 0x170, &extra, sizeof(extra));
        }
        break;
    }
    case 19:
        emit.unk58 = 0x11;
        spawn.unk40 &= ~2;
        func_151407D0(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    default:
        func_151407D0(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        break;
    }
    return 1;
}

/** Runs the object's handler from D_80082EA0, chosen by unk17. */
s32 func_1501370C(LevelObject *obj) {
    u8 index = obj->unk17;
    void (*handler)(void) = D_80082EA0[index];

    if (handler != NULL) {
        handler();
    }
    return 1;
}

s32 func_1501374C(LevelObject *obj) {
    obj->flags |= 4;
    func_1515D088(obj);
    return 1;
}

// fat struct definition:
/* func_1000FA64 passes its arguments as full words (callers do not truncate). */
u16 func_1000FA64(s32, s32, s32, s32, s32, s32, s32, void *, void *, s32, s32, s32);
void func_150A8050(f32 mtx[4][4], f32 arg1, f32 arg2, f32 arg3);
extern f32 D_80096640;
extern f32 D_80096644;

typedef struct Game40490BeamPacket {
    f32 position[3];
    f32 direction[3];
    f32 height;
    f32 width;
    f32 rotation[2];
    f32 zero;
    f32 field2C;
    f32 field30;
} Game40490BeamPacket;

/** Spawns a beam effect of twice the object's sizeY, pointing along its rotation. */
s32 func_15013778(LevelObject *obj) {
    struct260 *effect;
    Game40490BeamPacket packet;
    f32 matrix[4][4];

    if (obj->sizeY != 0) {
        f32 inverse;
        s16 height;

        packet.height = 2.0f * obj->sizeY;
        inverse = 1.0f / packet.height;
        packet.width = obj->sizeX;
        packet.rotation[0] = obj->rotX;
        packet.rotation[1] = obj->rotY;
        packet.zero = 0.0f;
        packet.field2C = 0.2001999915f;
        packet.field30 = 0.04999999702f;
        packet.position[0] = obj->x;
        packet.position[1] = obj->y;
        packet.position[2] = obj->z;
        func_150A8050(matrix, obj->rotX, obj->rotY, 0.0f);
        func_150A7960(matrix[0], 0.0f, packet.height, 0.0f,
                      &packet.direction[0], &packet.direction[1], &packet.direction[2]);
        packet.direction[0] *= inverse;
        packet.direction[1] *= inverse;
        packet.direction[2] *= inverse;
        effect = func_15149130(300, -1, 0x19, -1, 0, 0x17, sizeof(packet), 0xFF, 0);
        if (effect != NULL) {
            memcpy((u8 *)effect + 0x28, &packet, sizeof(packet));
        }
        height = obj->sizeY;
        func_1000FA64(0x67C, obj->x, (s16)(obj->y + height), obj->z,
                      12000, height * 2, height / 2, (void *)func_1000EF40, 0, 0, 8, 0);
    }
    return 1;
}

/** Runs the object's handler from D_80082ECC, chosen by unk17. */
s32 func_1501396C(LevelObject *obj) {
    u8 index = obj->unk17;
    void (*handler)(void) = D_80082ECC[index];

    if (handler != NULL) {
        handler();
    }
    return 1;
}

/** Spawns the area effect described by entry unk18 of D_80095FA0 over the object's footprint. */
// matching: index lives in the (unused) second argument register
s32 func_150139AC(LevelObject *obj, s32 index) {
    Struct150139ACLocal tmp;
    Struct150139ACEntry *entry;
    struct260 *effect;

    index = obj->unk18;
    obj->flags |= 4;
    tmp.unk0 = obj;
    tmp.unkC = 0.0f;

    switch (obj->unk15 & 3) {
    default:
        tmp.unk4 = 1.0f;
        tmp.unk10 = 1000.0f;
        break;
    case 2:
        /* box: area of the footprint */
        tmp.unk4 = (f32)(obj->sizeX * obj->sizeZ) * 4.0f;
        tmp.unk10 = (f32)(obj->y + obj->sizeY);
        break;
    case 0:
    case 1:
        /* cylinder: area of the circle */
        tmp.unk4 = (f32)(obj->sizeX * obj->sizeX) * 3.141592741f;
        tmp.unk10 = (f32)(obj->y + obj->sizeY);
        break;
    }

    entry = &D_80095FA0[index];
    tmp.unk14 = entry->unk4;
    tmp.unk18 = entry->unk8;
    tmp.unk1C = entry->unkC;
    tmp.unk20 = entry->unk10;
    tmp.unk28 = entry->unk18;
    tmp.unk2C = entry->unk1C;
    tmp.unk60 = 0;
    tmp.unk64 = 0;
    tmp.unk65 = 0;
    tmp.unk68 = 0;
    tmp.unk24 = entry->unk14;
    tmp.unk26 = entry->unk16;
    tmp.unk30 = entry->unk20;
    tmp.unk34 = entry->unk24;
    tmp.unk38 = entry->unk28;
    tmp.unk3C = entry->unk2C;
    tmp.unk8 = entry->unk0;
    tmp.unk40 = entry->unk30;
    tmp.unk41 = entry->unk31;
    tmp.unk42 = entry->unk32;
    tmp.unk48 = -10000.0f;

    if (entry->unk34 != 0) {
        func_1510F800(0);
        tmp.unk6C = func_1510FD20(obj->x, obj->z);
    } else {
        tmp.unk6C = 0;
    }

    if (entry->unk33 != 0) {
        tmp.unk44 = func_1000FA64(0x4D, obj->x, obj->y, obj->z, 24000, 1000, 300, 0, NULL, 0, 0, 0);
    } else {
        tmp.unk44 = 0;
    }

    effect = func_151491F4(300, -1, 6, 0, 10, sizeof(tmp), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &tmp, sizeof(tmp));
    }
    return 1;
}

/** Runs the object's handler from D_80082F28 (chosen by unk18) with unk1C / 1000 as argument. */
s32 func_15013C38(LevelObject *obj) {
    s32 index;
    f32 value;
    void (*handler)(LevelObject *, f32);

    obj->flags |= 4;
    index = obj->unk18;

    if ((((u8 *)D_800D2E4C)[0x11] & 4) && (D_800BE9F0 == 0x13)) { // low byte of D_800D2E4C->unk10
        return 1;
    }

    if (D_800C35EA == 1) {
        if ((D_800C35E8 == 15) || (D_800C35E8 == 16) || (D_800C35E8 == 17)) {
            return 1;
        }
    }

    if (index >= 6) {
        return 1;
    }

    handler = D_80082F28[index];
    if (handler != NULL) {
        value = (f32)obj->unk1C * 0.001000000047f;
        handler(obj, value);
    }

    return 1;
}

/** Calls func_151BE850 at the object's position, with a count of unk18 (at least 1). */
s32 func_15013D38(LevelObject *obj) {
    struct17 pos;
    s32 count;

    obj->flags |= 4;
    pos.unk0 = obj->x;
    pos.unk4 = obj->y;
    pos.unk8 = obj->z;

    count = obj->unk18;
    func_151BE850(&pos, obj->rotY, count ? count : 1, ((u8 *)&obj->unk1C)[3], 1, 0xFF, 1);
    return 1;
}

/** Emits particles from the object's position, scaled by its sizeX and sizeY. */
s32 func_15013DE8(LevelObject *obj) {
    Struct15013DE8 params;
    f32 scaleX;
    f32 scaleY;

    obj->flags |= 4;

    scaleX = obj->sizeX * 0.05000000075f;
    scaleY = obj->sizeY * 0.01999999955f;

    params.unk20 = 0x6231; params.unk22 = 0x1A4D; // matching: on one line, as1 keeps these stores in this order
    params.unk26 = 0;
    params.unk27 = 0;
    params.unk28 = 0;
    params.unk29 = 0xFF;
    params.unk2A = 0;
    params.unk2D = 0xFF;
    params.unk30 = 0;
    params.unk2E = 0;
    params.unk2F = 0;
    params.unk34 = 0;
    params.unk3C = 99999.89844f;

    params.unk0 = obj->x;
    params.unk4 = obj->y;
    params.unk8 = obj->z;

    params.unk24 = 300;
    params.unk38 = 0.0f;
    params.unk40 = 0.04500000179f;
    params.unk44 = 0.6999999881f;

    params.unk2C = (func_150ADA20() % 56U) + 200;

    params.unkC = 22.60000038f * scaleX;
    params.unk14 = 13.78600025f * scaleX;
    params.unk10 = 57.96900177f * scaleY; params.unk18 = 42.7140007f * scaleY; // matching: on one line, as1 keeps these stores in this order
    params.unk1C = 118.7630005f * scaleY;

    func_15149550(&params, 10, 1, obj->unk18 ? 2 : 1, 0xFF, 1);
    return 1;
}

s32 func_15013F9C(LevelObject *obj) {
    func_151CD2C0(obj, 0xFF, 1);
    return 1;
}

/** Runs the object's handler from D_80082F40, chosen by the low byte of unk18. */
s32 func_15013FC4(LevelObject *obj) {
    u8 index = ((u8 *)&obj->unk18)[3];
    void (*handler)(void) = D_80082F40[index];

    if (handler != NULL) {
        handler();
    }
    return 1;
}

/** Registers the object in slot unk1C (0-5) of D_800E0900. */
s32 func_15014004(LevelObject *obj) {
    s32 slot = obj->unk1C;

    if (slot < 0) {
        return 1;
    }
    if (slot >= 6) {
        return 1;
    }
    D_800E0900[slot] = obj;
    return 1;
}

/** Registers the object as D_800D9A20 (unk18 == 0) or D_800D9A24 (unk18 == 1). */
s32 func_15014040(LevelObject *obj) {
    s32 which = obj->unk18;

    obj->flags |= 4;
    if (which == 0) {
        D_800D9A20 = obj;
    } else if (which == 1) {
        D_800D9A24 = obj;
    }
    return 1;
}

s32 func_1501407C(LevelObject *obj) {
    D_800D987C = 0;
    return 1;
}

/* Declared s32 but returns no value; giving it a return value or void changes the code. */
s32 func_15014094(LevelObject *obj) {
    struct {
        LevelObject *unk0;
        f32 unk4;
        f32 unk8;
        s32 unkC;
        u8 pad10[0x1C];
        s8 unk2C;
        u8 pad2D[7];
    } data;
    struct260 *effect;

    obj->flags |= 4;
    data.unk0 = obj;
    data.unk4 = func_15144598(obj);
    data.unk8 = 0.0f;

    func_1510F800(0);
    data.unkC = func_1510FD20(obj->x, obj->z);
    data.unk2C = 0;

    effect = func_15149130(300, -1, 0x21, -1, 0, 0, sizeof(data), 0xFF, 1);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
}

/** Calls func_151A9390 for the object with option bits taken from unk18 (bit 1 inverted). */
s32 func_15014144(LevelObject *obj) {
    s32 flags;
    s32 bit8;
    s32 bit4;
    s32 bit2;
    s32 bit1;
    s32 bit16;

    flags = obj->unk18;
    bit8 = (flags & 8) ? 8 : 0;
    bit4 = (flags & 4) ? 4 : 0;
    bit2 = (flags & 2) ? 0 : 2;
    bit1 = (flags & 1) ? 1 : 0;
    bit16 = (flags & 0x10) ? 0x10 : 0;

    func_151A9390((u8)(bit16 | bit1 | bit2 | bit4 | bit8), ((u8 *)&obj->unk1C)[3], obj, 0, -10000.0f, 100.0f, -1, 0xFF, 1);
    return 1;
}

s32 func_15014220(LevelObject *obj) {
    struct {
        f32 unk0;
        LevelObject *unk4;
        u8 unk8;
    } data;
    struct260 *effect;

    obj->flags |= 4;
    data.unk8 = 1;
    data.unk4 = obj;
    data.unk0 = 0.0f;

    effect = func_15149130(300, -1, 0x26, -1, 0, 0x24, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

/** Registers the object in slot (low byte of unk18, 0-2) of D_800D9AA0. */
s32 func_150142AC(LevelObject *obj) {
    s32 slot;

    slot = ((u8 *)&obj->unk18)[3];
    obj->flags |= 4;
    if ((slot < 0) || (slot >= 3)) {
        return 1;
    }
    D_800D9AA0[slot] = obj;
    return 1;
}

s32 func_150142EC(LevelObject *obj) {
    struct {
        LevelObject *unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        f32 unk10;
        f32 unk14;
    } data;
    struct260 *effect;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    f32 scale;

    obj->flags |= 4;
    if (D_80082FA0 >= 2) {
        return 1;
    }

    if ((((u8 *)D_800D2E4C)[0x11] & 4) && (D_800BE9F0 == 0x13)) { // low byte of D_800D2E4C->unk10
        return 1;
    }

    /* four 16-bit values, in thousandths */
    a = (f32)(obj->unk1C & 0xFFFF) * 0.001000000047f;
    b = (f32)((obj->unk1C >> 16) & 0xFFFF) * 0.001000000047f;
    c = (f32)(obj->unk20 & 0xFFFF) * 0.001000000047f;
    d = (f32)((obj->unk20 >> 16) & 0xFFFF) * 0.001000000047f;

    scale = func_1514462C((s32)obj);

    data.unk0 = obj;
    data.unk4 = (a * scale) * 1.271515948e-10f;
    data.unk8 = (b * scale) * 1.271515948e-10f;
    data.unkC = c;
    data.unk10 = d;
    data.unk14 = 0.0f;

    effect = func_15149130(300, -1, 0x29, -1, 0, 0, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}
typedef struct Game40490144B8Packet {
    void *owner;
    s32 choice;
    s32 low;
    s32 range;
    s16 countX, countY;
    f32 sizeX, sizeY;
    f32 angleX, angleY;
    f32 arc, factor;
    f32 steps, inverse, height;
} Game40490144B8Packet;

void func_15145974(void *, f32 *, f32 *);

/** Spawns an effect along the object's rotation, configured by the bytes of unk18 and unk1C. */
s32 func_150144B8(LevelObject *obj) {
    Game40490144B8Packet packet;
    u32 value; // matching: holds the random number, then the effect address; two locals grow the frame
    struct {
        f32 direction[3];
        f32 matrix[4][4];
    } scratch;

    obj->flags |= 4;
    if (D_80082FA0 >= 2) {
        return 1;
    }
    func_150A8050(scratch.matrix, obj->rotX, obj->rotY, 0.0f);
    func_150A7960(scratch.matrix[0], 0.0f, 100.0f, 0.0f, &scratch.direction[0], &scratch.direction[1], &scratch.direction[2]);
    func_15145974(scratch.direction, &packet.angleX, &packet.angleY);
    packet.angleX *= 0.017453292f;
    packet.angleY *= 0.017453292f;
    packet.low = obj->unk18 & 0xFF;
    packet.range = (obj->unk18 >> 8) & 0xFF;
    value = func_150ADA20();
    packet.owner = obj;
    packet.choice = value % (u32)(packet.range + 1) + packet.low;
    packet.countX = (obj->unk18 >> 16) & 0xFF;
    packet.countY = (obj->unk18 >> 24) & 0xFF;
    packet.sizeX = (f32)(obj->unk1C & 0xFF) * 0.015625f;
    packet.sizeY = (f32)((obj->unk1C >> 8) & 0xFF) * 0.015625f;
    packet.steps = (f32)((obj->unk1C >> 24) & 0xFF);
    packet.inverse = 1.0f / packet.steps;
    packet.height = (f32)obj->sizeY * packet.inverse;
    packet.arc = func_150484A0(obj->sizeX, obj->sizeY);
    packet.factor = (f32)((obj->unk1C >> 16) & 0xFF) * 0.00390625f * 0.7853982f;
    value = (u32)func_15149130(300, -1, 0x2A, -1, 0, 0, sizeof(packet), 0xFF, 0);
    if (value != 0) {
        memcpy((void *)((u32)value + 0x28U), &packet, sizeof(packet));
    }
    return 1;
}
extern f32 D_80096690;
extern f32 D_80096694;

typedef struct Game40490PlanePoint {
    f32 x;
    f32 z;
} Game40490PlanePoint;

typedef struct Game40490PlanePacket {
    Game40490PlanePoint start;
    Game40490PlanePoint end;
    f32 edge[4];
    f32 top;
    f32 bottom;
    f32 scaleX;
    f32 scaleZ;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    s8 bytes[4];
} Game40490PlanePacket;

/** Spawns an effect over the object's rotated rectangle (sizeX by sizeZ, sizeY high). */
s32 func_1501474C(LevelObject *obj) {
    Game40490PlanePacket packet;
    struct260 *effect;
    Game40490PlanePoint corners[4];
    f32 s;
    f32 c;
    f32 z;
    s32 pad; // matching: stack slot of a local the original had here
    f32 x;

    obj->flags |= 4;
    x = obj->x;
    z = obj->z;
    s = sinf(obj->rotY * 0.01745329238f);
    c = cosf(obj->rotY * 0.01745329238f);
    corners[0].x = obj->sizeX * c + x + obj->sizeZ * s;
    corners[0].z = obj->sizeZ * c + z - obj->sizeX * s;
    corners[1].x = obj->sizeX * c + x + -obj->sizeZ * s;
    corners[1].z = -obj->sizeZ * c + z - obj->sizeX * s;
    corners[2].x = -obj->sizeX * c + x + -obj->sizeZ * s;
    corners[2].z = -obj->sizeZ * c + z - -obj->sizeX * s;
    corners[3].x = -obj->sizeX * c + x + obj->sizeZ * s;
    corners[3].z = obj->sizeZ * c + z - -obj->sizeX * s;
    packet.start = corners[1];
    packet.end = corners[2];
    packet.edge[0] = corners[0].x - corners[1].x;
    packet.edge[1] = corners[0].z - corners[1].z;
    packet.edge[2] = corners[3].x - corners[2].x;
    packet.edge[3] = corners[3].z - corners[2].z;
    packet.top = obj->y + obj->sizeY;
    packet.bottom = obj->y;
    packet.scaleX = (f32)(obj->unk1C & 0xFF) * 0.00390625f * 0.5f;
    packet.scaleZ = (f32)((obj->unk1C >> 8) & 0xFF) * 0.00390625f * 0.5f;
    packet.unk30 = 0.0f;
    packet.unk34 = (f32)((obj->unk1C >> 16) & 0xFF) * 0.00390625f * 250.0f;
    packet.unk38 = (f32)((obj->unk1C >> 24) & 0xFF) * 0.00390625f * 250.0f;
    packet.bytes[0] = obj->unk18 & 0xFF;
    packet.bytes[1] = (obj->unk18 >> 8) & 0xFF;
    packet.bytes[2] = (obj->unk18 >> 16) & 0xFF;
    packet.bytes[3] = (obj->unk18 >> 24) & 0xFF;
    effect = func_15149130(300, -1, 0x2C, -1, 0, 0, sizeof(packet), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &packet, sizeof(packet));
    }
    return 1;
}
extern u8 D_800A0938[][3];
struct102 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, s32, s32);

typedef struct Game4049014B60Packet {
    f32 pos[3];
    f32 rot[3];
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    struct102 *effects[6];
    f32 unk3C;
    void *owner;
    u8 active;
} Game4049014B60Packet;

/** Creates six func_151602C0 objects at the object's position, coloured from D_800A0938, and an effect
 * with a random rotation that owns them (they are freed again if no effect slot is free). */
s32 func_15014B60(LevelObject *obj) {
    Game4049014B60Packet packet;
    s32 i;
    struct260 *effect;
    Header2 pos;
    Header header;
    u32 count;

    obj->flags |= 4;
    packet.active = 1;
    packet.owner = obj;
    packet.pos[0] = obj->x;
    packet.pos[1] = obj->y;
    packet.pos[2] = obj->z;
    count = osGetCount();
    packet.rot[0] = (f32)((func_150ADA20() * count) & 0xFFFF) * 1.525902189e-05f * 360.0f;
    count = osGetCount();
    packet.rot[1] = (f32)((func_150ADA20() * count) & 0xFFFF) * 1.525902189e-05f * 360.0f;
    count = osGetCount();
    packet.rot[2] = (f32)((func_150ADA20() * count) & 0xFFFF) * 1.525902189e-05f * 360.0f;
    packet.unk18 = (f32)(obj->unk18 & 0xFFFF) * 0.152587890625f * 0.001000000047f;
    packet.unk1C = (f32)((obj->unk18 >> 16) & 0xFFFF) * 0.152587890625f * 0.001000000047f;
    packet.unk20 = (f32)(obj->unk1C & 0xFFFF) * 0.152587890625f * 0.001000000047f;
    packet.unk3C = (f32)((obj->unk1C >> 16) & 0xFFFF) * 0.457763671875f * 9.999999975e-07f;
    pos.unk0 = obj->x;
    pos.unk4 = obj->y;
    pos.unk8 = obj->z;
    for (i = 0; i < 6; i++) {
        s32 r;
        s32 g;

        header.unk0 = 2;
        header.unk1 = -1;
        header.unk2 = 300;
        header.unk4 = 0;
        r = (D_800BE9F0 == 0x40) ? 0xFF : D_800A0938[i][0];
        g = (D_800BE9F0 == 0x40) ? 0 : D_800A0938[i][1];
        count = (D_800BE9F0 == 0x40) ? 0 : D_800A0938[i][2]; // matching: blue reuses count's register
        packet.effects[i] = func_151602C0(&header, &pos, 5, r, g, count, 0xFF, 0, 0, 0xFF, 0);
    }
    effect = func_15149130(300, -1, 0x2E, -1, 0, 0x29, sizeof(packet), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &packet, sizeof(packet));
        return 1;
    }
    for (i = 0; i < 6; i++) {
        if (packet.effects[i] != NULL) {
            func_1516972C(packet.effects[i]);
        }
    }
    return 1;
}

s32 func_15014F6C(LevelObject *obj) {
    struct {
        LevelObject *unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
        f32 mtx[4][4];
        u8 unk50;
        u8 unk51;
    } data;
    struct260 *effect;
    s32 flag;

    obj->flags |= 4;
    data.unk51 = obj->unk20;
    flag = (obj->unk1C & 1) ? 1 : 0;
    data.unk50 = flag;
    data.unk0 = obj;
    /* unk18 holds a minimum and a range: unkC is random in [min, min + range] */
    data.unk4 = obj->unk18 & 0xFFFF;
    data.unk8 = (obj->unk18 >> 16) & 0xFFFF;
    data.unkC = ((u32)func_150ADA20() % (u32)(data.unk8 + 1)) + data.unk4;
    func_150A8050(data.mtx, obj->rotX, obj->rotY, 0.0f);

    data.mtx[3][0] = obj->x;
    data.mtx[3][1] = obj->y;
    data.mtx[3][2] = obj->z;

    effect = func_15149130(300, -1, 0x31, -1, 0, 0x2A, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

s32 func_150150A4(void) {
    struct17 *item = func_1515F1B0();

    if (item == NULL) {
        return 1;
    }

    func_1515F25C(&D_800DCDC4, item);
    D_800DCD90 += item->unk8;
    return 1;
}

s32 func_15015104(LevelObject *obj) {
    struct {
        LevelObject *unk0;
        u8 unk4;
        u8 pad5[3];
        s32 unk8;
        u8 unkC;
        u8 padD[3];
        s32 unk10;
    } data;
    struct260 *effect;
    s32 bit1;
    s32 bit2;
    u32 value;

    obj->unk14 = 1;
    data.unk0 = obj;
    data.unk4 = obj->unk1C;

    func_1510F800(0);
    data.unk8 = func_1510FD20(obj->x, obj->z);

    value = obj->unk20;
    bit1 = (value != 0) ? 1 : 0;
    bit2 = (value != 0) ? 2 : 0;
    data.unkC = bit2 | bit1;
    data.unk10 = 0;

    effect = func_15149130(300, -1, -1, -1, 0, 0x2C, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}
s32 func_150151D4(LevelObject *obj) {
    struct {
        LevelObject *unk0;
        f32 unk4;
        s16 unk8;
        f32 unkC;
        f32 unk10;
        f32 unk14;
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        u8 pad24[0x14];
        s32 unk38;
        s8 unk3C;
        s8 unk3D;
        s32 unk40;
        s32 unk44;
    } data;
    struct260 *effect;

    obj->flags |= 4;
    obj->unk14 = 1;

    data.unk0 = obj;
    data.unk4 = 0.0f;
    data.unk8 = -1;
    data.unkC = obj->x;
    data.unk10 = obj->y;
    data.unk14 = obj->z;
    data.unk18 = obj->sizeX;
    data.unk1C = obj->sizeY;
    data.unk20 = -10000.0f;
    data.unk40 = 0;
    data.unk3D = 0;
    data.unk3C = 0;
    data.unk38 = 0;

    func_1510F800(0);
    data.unk44 = func_1510FD20(obj->x, obj->z);

    effect = func_15149130(300, -1, 0x3C, -1, 0, 0x2D, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

/** Runs the object's handler from D_80082F70, chosen by unk1C (0 or 1). */
s32 func_15015300(LevelObject *obj) {
    void (*handler)(void);
    s32 index = obj->unk1C;

    if ((index < 0) || (index >= 2)) {
        return 1;
    }

    handler = D_80082F70[index];
    if (handler != NULL) {
        handler();
    }

    return 1;
}

/** Spawns an effect on the object's rotated rectangle: its four corners and the edge vectors between them. */
void func_15015354(LevelObject *obj) {
    struct {
        LevelObject *unk0;
        s16 unk4;
        u8 pad6[2];
        f32 unk8;
        f32 points[4][3];
        f32 edges[4][3];
        s32 unk6C;
        s32 unk70;
    } data;
    struct260 *effect;
    f32 z;
    f32 y;
    f32 x;
    f32 corners[4][3];
    f32 mtx[4][4];
    u8 i;

    data.unk0 = obj;
    data.unk4 = -1;
    data.unk8 = 0.0f;
    obj->unk14 = 1;
    x = obj->x;
    y = obj->y;
    z = obj->z;

    corners[0][0] = obj->sizeX;
    corners[0][1] = 0.0f;
    corners[0][2] = obj->sizeZ;
    corners[1][0] = obj->sizeX;
    corners[1][1] = 0.0f;
    corners[1][2] = -obj->sizeZ;
    corners[2][0] = -obj->sizeX;
    corners[2][1] = 0.0f;
    corners[2][2] = -obj->sizeZ;
    corners[3][0] = -obj->sizeX;
    corners[3][1] = 0.0f;
    corners[3][2] = obj->sizeZ;

    func_150A8050(mtx, obj->rotX, obj->rotY, 0.0f);

    mtx[3][0] = x;
    mtx[3][1] = y;
    mtx[3][2] = z;

    for (i = 0; i < 4; i++) {
        func_150A7960(mtx[0], corners[i][0], corners[i][1], corners[i][2],
                      &data.points[i][0], &data.points[i][1], &data.points[i][2]);
    }

    data.edges[0][0] = data.points[1][0] - data.points[0][0];
    data.edges[0][1] = data.points[1][1] - data.points[0][1];
    data.edges[0][2] = data.points[1][2] - data.points[0][2];
    data.edges[1][0] = data.points[2][0] - data.points[1][0];
    data.edges[1][1] = data.points[2][1] - data.points[1][1];
    data.edges[1][2] = data.points[2][2] - data.points[1][2];
    data.edges[2][0] = data.points[3][0] - data.points[2][0];
    data.edges[2][1] = data.points[3][1] - data.points[2][1];
    data.edges[2][2] = data.points[3][2] - data.points[2][2];
    data.edges[3][0] = data.points[0][0] - data.points[3][0];
    data.edges[3][1] = data.points[0][1] - data.points[3][1];
    data.edges[3][2] = data.points[0][2] - data.points[3][2];

    func_1510F800(0);
    data.unk6C = func_1510FD20(obj->x, obj->z);
    data.unk70 = 0;

    effect = func_15149130(300, -1, 0x3D, -1, 0, 0x2E, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
}

s32 func_15015644(LevelObject *obj) {
    struct {
        LevelObject *unk0;
        f32 unk4;
        s32 unk8;
        s8 unkC;
    } data;
    struct260 *effect;

    obj->flags |= 4;
    obj->unk14 = 1;

    data.unk0 = obj;
    data.unk4 = func_15144598(obj);
    func_1510F800(0);
    data.unk8 = func_1510FD20(obj->x, obj->z);
    data.unkC = 0;

    effect = func_15149130(300, -1, 0x44, -1, 0, 0x2F, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

/** Passes the object's two end points (its position, and sizeY along its rotation from there) to
 * func_151ACBD4, higher end first. In levels 0xB, 0x2C and 0x26 only objects with a large enough unk18 do. */
s32 func_150156F4(LevelObject *obj) {
    struct17 points[2];
    f32 mtx[4][4];
    Struct150156F4Payload payload;

    if (D_800BE9F0 == 0xB) {
        if (obj->unk18 < 6U) {
            return 1;
        }
    } else if (D_800BE9F0 == 0x2C) {
        if (obj->unk18 < 7U) {
            return 1;
        }
    } else if (D_800BE9F0 == 0x26) {
        if (obj->unk18 < 2U) {
            return 1;
        }
    }

    func_150A8050(mtx, obj->rotX, obj->rotY, 0.0f);
    mtx[3][0] = obj->x;
    mtx[3][1] = obj->y;
    mtx[3][2] = obj->z;

    func_150A7960(mtx[0], 0.0f, obj->sizeY, 0.0f, &points[1].unk0, &points[1].unk4, &points[1].unk8);

    points[0].unk0 = obj->x;
    points[0].unk4 = obj->y;
    points[0].unk8 = obj->z;
    payload.unk0 = obj->unk18;

    /* unk4 gets the higher end, unk10 the lower */
    payload.unk4 = points[points[0].unk4 < points[1].unk4];
    payload.unk10 = points[points[1].unk4 < points[0].unk4];

    payload.unk20 = 0;
    payload.unk1C = fabsf(points[1].unk4 - points[0].unk4);
    func_151ACBD4(&payload, 0);

    return 1;
}
