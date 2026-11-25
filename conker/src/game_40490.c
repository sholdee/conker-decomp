#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#include "effects.h"


void func_151A9390(s32 arg0, s32 arg1, TriggerZone *arg2, s32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8);
extern s32 (*D_80082E30[])(TriggerZone *);
extern void (*D_80082F28[])(TriggerZone *, f32);
extern u8 D_800C35E8;
extern void func_1510F800(s32);
s32 func_151BE850(Vec3f *arg0, f32 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6);
s32 func_15149550(void *, s32, s32, u8, u8, s32);
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
typedef struct {
    s8 unk0;
    u8 pad1[3];
    Vec3f unk4;
    Vec3f unk10;
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
    TriggerZone *unk0;
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


void func_15012FE0(void) {
    D_800BE570 = 0;
    D_800BE574 = 0;
    D_800BE575 = 0;
}

/** Runs the spawn handler of every level object, chosen by the object's type. */
void func_15013000(void) {
    u32 i;

    D_800DCDC4 = NULL;
    D_800DCD90 = 0.0f;
    // matching: the label ends the block, so IDO cannot hoist the D_800D3094 load above the stores
start:
    for (i = 0; i < D_800D3094; i++) {
        u8 type = D_800D3098[i].kind >> 2;
        s32 (*handler)(TriggerZone *) = D_80082E30[type];

        if (handler != NULL) {
            handler(&D_800D3098[i]);
        }
    }
}


typedef struct {
    f32 unk0;
    f32 unk4;
    WorldObject *unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} Payload18_150130B4;

extern s32 D_8009658C[];
extern s32 D_800965A0[];
extern s32 D_800965B0[];
void *func_151407D0(GlowShape *, s32, ParticleDef *, s32, s32, s32, s32, s32, s32, s32);
extern WorldObject *func_151149AC(u8);

/** Spawns the particle emitter a level object describes; unk1C selects the kind of emitter. */
s32 func_150130B4(TriggerZone *obj) {
    GlowShape emit;
    ParticleDef spawn;
    s32 kind;
    s32 index;
    u8 onGround;
    MtxF mtx;

    obj->flags |= 4;
    onGround = (obj->unk20 >> 24) & 0xFF;
    kind = obj->unk1C;
    // matching: the labels split basic blocks, which as1 schedules separately
start:
    spawn.pos.x = obj->x;
    spawn.pos.y = obj->y;
    spawn.pos.z = obj->z;
    func_150A8050(mtx, obj->pitch, obj->yaw, 0.0f);
    func_150A7960(mtx[0], 0.0f, 0.0f, -500.0f, &spawn.rot.x, &spawn.rot.y, &spawn.rot.z);
    spawn.rot.x += spawn.pos.x;
    spawn.rot.y += spawn.pos.y;
    spawn.rot.z += spawn.pos.z;
    // matching: the typedef-only block scope changes how as1 schedules these stores
    {
        typedef s32 PadT;

        emit.height1 = obj->radius;
        emit.width1 = emit.height1;
        emit.height0 = obj->height;
        emit.width0 = emit.height0;
        emit.alpha1 = ((obj->unk20 >> 8) & 0xFF) * 0.003921568859f;
    }
    emit.alpha0 = (obj->unk20 & 0xFF) * 0.003921568859f;
    index = (obj->unk18 >> 24) & 0xFF;
    spawn.texture = D_8009658C[index];
    spawn.renderModes = D_800965A0[index] + 3;
    spawn.lifetime = 300;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.rgba[0] = obj->unk18 & 0xFF;
    spawn.rgba[1] = (obj->unk18 >> 8) & 0xFF;
    spawn.rgba[2] = (obj->unk18 >> 16) & 0xFF;
    spawn.rgba[3] = 0xFF;
    spawn.width = 0.0f;
    spawn.height = 0.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.unk45 = 0xFF;
    spawn.unk44 = 0xFF;
    spawn.unk46 = 0;
    spawn.unk47 = 6;
    spawn.type = 3;
    emit.visibility[0] = 0;
    emit.visibility[1] = 0;
    emit.visibility[2] = 0;
    emit.visibility[3] = 0;
    emit.scale = 1.0f;
    emit.offset1 = 0.4099999964f;
    emit.offset0 = 0.200000003f;
    emit.fadeRate = 9.999999975e-07f;
    if (onGround) {
ground:
        func_1510F800(0);
        spawn.unk48 = func_1510FD20((s32)spawn.pos.x, (s32)spawn.pos.z);
    } else {
        spawn.unk48 = 0;
        onGround = FALSE; // matching: redundant store, but IDO's allocation needs it
    }
    emit.type = 0;
    spawn.flags = 0x2006 | (((obj->unk20 >> 16) & 0xFF) ? 0x10 : 0) | (onGround ? 0x01000000 : 0) | 0x40000 | 0x80000 | 0x10000 | 0x400000;
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

        emit.type = D_800965B0[kind];
        unused = func_151407D0(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    }
    case 9:
    case 10:
    case 11:
    case 12: {
        void *unused; // matching: as above

        emit.type = D_800965A0[kind];
        unused = func_151407D0(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    }
    case 5:
    case 6:
    case 7: {
        s32 ids[3] = { 12, 13, 14 };

        emit.type = ids[kind - 5];
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
        extra.unkC = extra.unk8->x;
        extra.unk10 = extra.unk8->y;
        extra.unk14 = extra.unk8->z;
        emit.type = 0;
        particle = func_151407D0(&emit, 0x78, &spawn, 0x25, 0, 0, 0, -1, 0xFF, 1);
        if (particle != NULL) {
            memcpy((u8 *)particle + 0x170, &extra, sizeof(extra));
        }
        break;
    }
    case 19:
        emit.type = 0x11;
        spawn.flags &= ~2;
        func_151407D0(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    default:
        func_151407D0(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        break;
    }
    return 1;
}

/** Runs the object's handler from D_80082EA0, chosen by unk17. */
s32 func_1501370C(TriggerZone *obj) {
    u8 index = obj->subtype;
    void (*handler)(void) = D_80082EA0[index];

    if (handler != NULL) {
        handler();
    }
    return 1;
}

s32 func_1501374C(TriggerZone *obj) {
    obj->flags |= 4;
    func_1515D088(obj);
    return 1;
}

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
s32 func_15013778(TriggerZone *obj) {
    struct260 *effect;
    Game40490BeamPacket packet;
    MtxF matrix;

    if (obj->height != 0) {
        f32 inverse;
        s16 height;

        packet.height = 2.0f * obj->height;
        inverse = 1.0f / packet.height;
        packet.width = obj->radius;
        packet.rotation[0] = obj->pitch;
        packet.rotation[1] = obj->yaw;
        packet.zero = 0.0f;
        packet.field2C = 0.2001999915f;
        packet.field30 = 0.04999999702f;
        packet.position[0] = obj->x;
        packet.position[1] = obj->y;
        packet.position[2] = obj->z;
        func_150A8050(matrix, obj->pitch, obj->yaw, 0.0f);
        func_150A7960(matrix[0], 0.0f, packet.height, 0.0f,
                      &packet.direction[0], &packet.direction[1], &packet.direction[2]);
        packet.direction[0] *= inverse;
        packet.direction[1] *= inverse;
        packet.direction[2] *= inverse;
        effect = func_15149130(300, -1, 0x19, -1, 0, 0x17, sizeof(packet), 0xFF, 0);
        if (effect != NULL) {
            memcpy((u8 *)effect + 0x28, &packet, sizeof(packet));
        }
        height = obj->height;
        func_1000FA64(0x67C, obj->x, (s16)(obj->y + height), obj->z,
                      12000, height * 2, height / 2, func_1000EF40, 0, 0, 8, 0);
    }
    return 1;
}

/** Runs the object's handler from D_80082ECC, chosen by unk17. */
s32 func_1501396C(TriggerZone *obj) {
    u8 index = obj->subtype;
    void (*handler)(void) = D_80082ECC[index];

    if (handler != NULL) {
        handler();
    }
    return 1;
}

/** Spawns the area effect described by entry unk18 of D_80095FA0 over the object's footprint. */
// matching: index lives in the (unused) second argument register
s32 func_150139AC(TriggerZone *obj, s32 index) {
    Struct150139ACLocal data;
    Struct150139ACEntry *entry;
    struct260 *effect;

    index = obj->unk18;
    obj->flags |= 4;
    data.unk0 = obj;
    data.unkC = 0.0f;

    switch (obj->kind & 3) {
    default:
        data.unk4 = 1.0f;
        data.unk10 = 1000.0f;
        break;
    case 2:
        /* box: area of the footprint */
        data.unk4 = (f32)(obj->radius * obj->depth) * 4.0f;
        data.unk10 = (f32)(obj->y + obj->height);
        break;
    case 0:
    case 1:
        /* cylinder: area of the circle */
        data.unk4 = (f32)(obj->radius * obj->radius) * 3.141592741f;
        data.unk10 = (f32)(obj->y + obj->height);
        break;
    }

    entry = &D_80095FA0[index];
    data.unk14 = entry->unk4;
    data.unk18 = entry->unk8;
    data.unk1C = entry->unkC;
    data.unk20 = entry->unk10;
    data.unk28 = entry->unk18;
    data.unk2C = entry->unk1C;
    data.unk60 = 0;
    data.unk64 = 0;
    data.unk65 = 0;
    data.unk68 = 0;
    data.unk24 = entry->unk14;
    data.unk26 = entry->unk16;
    data.unk30 = entry->unk20;
    data.unk34 = entry->unk24;
    data.unk38 = entry->unk28;
    data.unk3C = entry->unk2C;
    data.unk8 = entry->unk0;
    data.unk40 = entry->unk30;
    data.unk41 = entry->unk31;
    data.unk42 = entry->unk32;
    data.unk48 = -10000.0f;

    if (entry->unk34 != 0) {
        func_1510F800(0);
        data.unk6C = func_1510FD20(obj->x, obj->z);
    } else {
        data.unk6C = 0;
    }

    if (entry->unk33 != 0) {
        data.unk44 = func_1000FA64(0x4D, obj->x, obj->y, obj->z, 24000, 1000, 300, 0, NULL, 0, 0, 0);
    } else {
        data.unk44 = 0;
    }

    effect = func_151491F4(300, -1, 6, 0, 10, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

/** Runs the object's handler from D_80082F28 (chosen by unk18) with unk1C / 1000 as argument. */
s32 func_15013C38(TriggerZone *obj) {
    s32 index;
    f32 value;
    void (*handler)(TriggerZone *, f32);

    obj->flags |= 4;
    index = obj->unk18;

    if ((D_800D2E4C[0x11] & 4) && (D_800BE9F0 == 0x13)) { // low byte of D_800D2E4C[0x10]
        return 1;
    }

    if (D_800C35EA[0] == 1) {
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
s32 func_15013D38(TriggerZone *obj) {
    Vec3f pos;
    s32 count;

    obj->flags |= 4;
    pos.x = obj->x;
    pos.y = obj->y;
    pos.z = obj->z;

    count = obj->unk18;
    func_151BE850(&pos, obj->yaw, count ? count : 1, (u8)obj->unk1C, 1, 0xFF, 1);
    return 1;
}

/** Emits particles from the object's position, scaled by its sizeX and sizeY. */
s32 func_15013DE8(TriggerZone *obj) {
    ParticleEmitterDesc params;
    f32 scaleX;
    f32 scaleY;

    obj->flags |= 4;

    scaleX = obj->radius * 0.05000000075f;
    scaleY = obj->height * 0.01999999955f;

    params.maxC = 0x6231; params.minC = 0x1A4D; // matching: on one line, as1 keeps these stores in this order
    params.color[0] = 0;
    params.color[1] = 0;
    params.color[2] = 0;
    params.color[3] = 0xFF;
    params.flags = 0;
    params.unk2D = 0xFF;
    params.extraSize = 0;
    params.unk2E = 0;
    params.unk2F = 0;
    params.effect = 0;
    params.maxHeight = 99999.89844f;

    params.pos.x = obj->x;
    params.pos.y = obj->y;
    params.pos.z = obj->z;

    params.life = 300;
    params.scale = 0.0f;
    params.shrinkRate = 0.04500000179f;
    params.growRate = 0.6999999881f;

    params.unk2C = (func_150ADA20() % 56U) + 200;

    params.maxA = 22.60000038f * scaleX;
    params.minA = 13.78600025f * scaleX;
    params.maxB = 57.96900177f * scaleY; params.minB = 42.7140007f * scaleY; // matching: on one line, as1 keeps these stores in this order
    params.altB = 118.7630005f * scaleY;

    func_15149550(&params, 10, 1, obj->unk18 ? 2 : 1, 0xFF, 1);
    return 1;
}

s32 func_15013F9C(TriggerZone *obj) {
    func_151CD2C0(obj, 0xFF, 1);
    return 1;
}

/** Runs the object's handler from D_80082F40, chosen by the low byte of unk18. */
s32 func_15013FC4(TriggerZone *obj) {
    u8 index = obj->unk18;
    void (*handler)(void) = D_80082F40[index];

    if (handler != NULL) {
        handler();
    }
    return 1;
}

/** Registers the object in slot unk1C (0-5) of D_800E0900. */
s32 func_15014004(TriggerZone *obj) {
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

/** Registers the object as D_800D9A20[unk18] (unk18 0 or 1). */
s32 func_15014040(TriggerZone *obj) {
    s32 which = obj->unk18;

    obj->flags |= 4;
    if (which == 0) {
        D_800D9A20[0] = obj;
    } else if (which == 1) {
        D_800D9A20[1] = obj;
    }
    return 1;
}

s32 func_1501407C(TriggerZone *obj) {
    D_800D987C = 0;
    return 1;
}

/* Declared s32 but returns no value; giving it a return value or void changes the code. */
s32 func_15014094(TriggerZone *obj) {
    struct {
        TriggerZone *unk0;
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
s32 func_15014144(TriggerZone *obj) {
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

    func_151A9390((u8)(bit16 | bit1 | bit2 | bit4 | bit8), (u8)obj->unk1C, obj, 0, -10000.0f, 100.0f, -1, 0xFF, 1);
    return 1;
}

s32 func_15014220(TriggerZone *obj) {
    struct {
        f32 unk0;
        TriggerZone *unk4;
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
s32 func_150142AC(TriggerZone *obj) {
    s32 slot;

    slot = (u8)obj->unk18;
    obj->flags |= 4;
    if ((slot < 0) || (slot >= 3)) {
        return 1;
    }
    D_800D9AA0[slot] = obj;
    return 1;
}

s32 func_150142EC(TriggerZone *obj) {
    struct {
        TriggerZone *unk0;
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

    if ((D_800D2E4C[0x11] & 4) && (D_800BE9F0 == 0x13)) { // low byte of D_800D2E4C[0x10]
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
s32 func_150144B8(TriggerZone *obj) {
    Game40490144B8Packet packet;
    u32 value; // matching: holds the random number, then the effect address; two locals grow the frame
    struct {
        f32 direction[3];
        MtxF matrix;
    } scratch;

    obj->flags |= 4;
    if (D_80082FA0 >= 2) {
        return 1;
    }
    func_150A8050(scratch.matrix, obj->pitch, obj->yaw, 0.0f);
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
    packet.height = (f32)obj->height * packet.inverse;
    packet.arc = func_150484A0(obj->radius, obj->height);
    packet.factor = (f32)((obj->unk1C >> 16) & 0xFF) * 0.00390625f * 0.7853982f;
    value = (u32)func_15149130(300, -1, 0x2A, -1, 0, 0, sizeof(packet), 0xFF, 0);
    if (value != 0) {
        memcpy((void *)((u32)value + 0x28U), &packet, sizeof(packet));
    }
    return 1;
}

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
s32 func_1501474C(TriggerZone *obj) {
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
    s = sinf(obj->yaw * 0.01745329238f);
    c = cosf(obj->yaw * 0.01745329238f);
    corners[0].x = obj->radius * c + x + obj->depth * s;
    corners[0].z = obj->depth * c + z - obj->radius * s;
    corners[1].x = obj->radius * c + x + -obj->depth * s;
    corners[1].z = -obj->depth * c + z - obj->radius * s;
    corners[2].x = -obj->radius * c + x + -obj->depth * s;
    corners[2].z = -obj->depth * c + z - -obj->radius * s;
    corners[3].x = -obj->radius * c + x + obj->depth * s;
    corners[3].z = obj->depth * c + z - -obj->radius * s;
    packet.start = corners[1];
    packet.end = corners[2];
    packet.edge[0] = corners[0].x - corners[1].x;
    packet.edge[1] = corners[0].z - corners[1].z;
    packet.edge[2] = corners[3].x - corners[2].x;
    packet.edge[3] = corners[3].z - corners[2].z;
    packet.top = obj->y + obj->height;
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
s32 func_15014B60(TriggerZone *obj) {
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

s32 func_15014F6C(TriggerZone *obj) {
    struct {
        TriggerZone *unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
        MtxF mtx;
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
    data.unkC = (func_150ADA20() % (u32)(data.unk8 + 1)) + data.unk4;
    func_150A8050(data.mtx, obj->pitch, obj->yaw, 0.0f);

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
    RegionNode *item = func_1515F1B0();

    if (item == NULL) {
        return 1;
    }

    func_1515F25C(&D_800DCDC4, item);
    D_800DCD90 += item->area;
    return 1;
}

s32 func_15015104(TriggerZone *obj) {
    struct {
        TriggerZone *unk0;
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

    obj->disabled = 1;
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
s32 func_150151D4(TriggerZone *obj) {
    struct {
        TriggerZone *unk0;
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
    obj->disabled = 1;

    data.unk0 = obj;
    data.unk4 = 0.0f;
    data.unk8 = -1;
    data.unkC = obj->x;
    data.unk10 = obj->y;
    data.unk14 = obj->z;
    data.unk18 = obj->radius;
    data.unk1C = obj->height;
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
s32 func_15015300(TriggerZone *obj) {
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
void func_15015354(TriggerZone *obj) {
    struct {
        TriggerZone *unk0;
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
    MtxF mtx;
    u8 i;

    data.unk0 = obj;
    data.unk4 = -1;
    data.unk8 = 0.0f;
    obj->disabled = 1;
    x = obj->x;
    y = obj->y;
    z = obj->z;

    corners[0][0] = obj->radius;
    corners[0][1] = 0.0f;
    corners[0][2] = obj->depth;
    corners[1][0] = obj->radius;
    corners[1][1] = 0.0f;
    corners[1][2] = -obj->depth;
    corners[2][0] = -obj->radius;
    corners[2][1] = 0.0f;
    corners[2][2] = -obj->depth;
    corners[3][0] = -obj->radius;
    corners[3][1] = 0.0f;
    corners[3][2] = obj->depth;

    func_150A8050(mtx, obj->pitch, obj->yaw, 0.0f);

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

s32 func_15015644(TriggerZone *obj) {
    struct {
        TriggerZone *unk0;
        f32 unk4;
        s32 unk8;
        s8 unkC;
    } data;
    struct260 *effect;

    obj->flags |= 4;
    obj->disabled = 1;

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
s32 func_150156F4(TriggerZone *obj) {
    Vec3f points[2];
    MtxF mtx;
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

    func_150A8050(mtx, obj->pitch, obj->yaw, 0.0f);
    mtx[3][0] = obj->x;
    mtx[3][1] = obj->y;
    mtx[3][2] = obj->z;

    func_150A7960(mtx[0], 0.0f, obj->height, 0.0f, &points[1].x, &points[1].y, &points[1].z);

    points[0].x = obj->x;
    points[0].y = obj->y;
    points[0].z = obj->z;
    payload.unk0 = obj->unk18;

    /* unk4 gets the higher end, unk10 the lower */
    payload.unk4 = points[points[0].y < points[1].y];
    payload.unk10 = points[points[1].y < points[0].y];

    payload.unk20 = 0;
    payload.unk1C = fabsf(points[1].y - points[0].y);
    func_151ACBD4(&payload, 0);

    return 1;
}
