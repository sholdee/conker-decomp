#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawn handler of each zone type (TriggerZone.kind >> 2, see enum ZoneType); NULL spawns nothing. */
s32 (*gZoneSpawnHandlers[26])(TriggerZone *) = {
    /*  0 */ NULL,
    /*  1 */ NULL,
    /*  2 */ NULL,
    /*  3 */ func_1501396C,
    /*  4 */ NULL,
    /*  5 */ NULL,
    /*  6 */ NULL,
    /*  7 */ NULL,
    /*  8 */ NULL,
    /*  9 */ NULL,
    /* 10 */ NULL,
    /* 11 */ NULL,
    /* 12 */ func_1501370C,
    /* 13 */ NULL,
    /* 14 */ NULL,
    /* 15 */ NULL,
    /* 16 */ NULL,
    /* 17 */ func_150130B4,
    /* 18 */ func_15014144,
    /* 19 */ func_150142EC,
    /* 20 */ func_150144B8,
    /* 21 */ func_1501474C,
    /* 22 */ func_15014B60,
    /* 23 */ func_15014F6C,
    /* 24 */ NULL,
    /* 25 */ NULL,
};
extern void (*D_80082F28[])(TriggerZone *, f32);

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
    ZoneNode *zone;
} Struct150139ACLocal;

extern Struct150139ACEntry D_80095FA0[];


void func_15012FE0(void) {
    D_800BE570 = NULL;
    D_800BE574 = 0;
    D_800BE575 = 0;
}

/** Runs the spawn handler of every level object, chosen by the object's type. */
void func_15013000(void) {
    u32 i;

    D_800DCDC4 = NULL;
    D_800DCD90 = 0.0f;
    // matching: the label ends the block, so IDO cannot hoist the gZoneCount load above the stores
start:
    for (i = 0; i < gZoneCount; i++) {
        u8 type = ZONE_TYPE(&zones[i]);
        s32 (*handler)(TriggerZone *) = gZoneSpawnHandlers[type];

        if (handler != NULL) {
            handler(&zones[i]);
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

/** Spawns the particle emitter a level object describes; data2 selects the kind of emitter. */
s32 func_150130B4(TriggerZone *obj) {
    GlowShape emit;
    ParticleDef spawn;
    s32 kind;
    s32 index;
    u8 onGround;
    MtxF mtx;

    obj->flags |= 4;
    onGround = (obj->data3 >> 24) & 0xFF;
    kind = obj->data2;
    // matching: the labels split basic blocks, which as1 schedules separately
start:
    spawn.pos.x = obj->x;
    spawn.pos.y = obj->y;
    spawn.pos.z = obj->z;
    func_150A8050(mtx, obj->pitch, obj->yaw, 0.0f);
    func_150A7960(mtx, 0.0f, 0.0f, -500.0f, &spawn.rot.x, &spawn.rot.y, &spawn.rot.z);
    spawn.rot.x += spawn.pos.x;
    spawn.rot.y += spawn.pos.y;
    spawn.rot.z += spawn.pos.z;
    // matching: the typedef-only block scope changes how as1 schedules these stores
    {
        typedef s32 PadT;

        emit.height1 = obj->xSize;
        emit.width1 = emit.height1;
        emit.height0 = obj->ySize;
        emit.width0 = emit.height0;
        emit.alpha1 = ((obj->data3 >> 8) & 0xFF) * 0.003921568859f;
    }
    emit.alpha0 = (obj->data3 & 0xFF) * 0.003921568859f;
    index = (obj->data1 >> 24) & 0xFF;
    spawn.texture = D_8009658C[index];
    spawn.renderModes = D_800965A0[index] + 3;
    spawn.lifetime = 300;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.rgba[0] = obj->data1 & 0xFF;
    spawn.rgba[1] = (obj->data1 >> 8) & 0xFF;
    spawn.rgba[2] = (obj->data1 >> 16) & 0xFF;
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
    spawn.flags = 0x2006 | (((obj->data3 >> 16) & 0xFF) ? 0x10 : 0) | (onGround ? 0x01000000 : 0) | 0x40000 | 0x80000 | 0x10000 | 0x400000;
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

        emit.type = D_800965B4[kind - 1];
        unused = func_151407D0(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    }
    case 9:
    case 10:
    case 11:
    case 12: {
        void *unused; // matching: as above

        emit.type = D_800965B4[kind - 5];
        unused = func_151407D0(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    }
    case 5:
    case 6:
    case 7: {
        const s32 ids[3] = { 12, 13, 14 };

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

/* Particle texture and render modes per glow kind (TriggerZone.data1 >> 24), and the emitter type for
 * spawn kinds 1-4 and 9-12. */
const s32 D_8009658C[5] = { 3, 0x15, 0x68, 0x69, 0x7E };
const s32 D_800965A0[5] = { 0x3400, 0x3400, 0x2200, 0x3400, 0x3400 };
const s32 D_800965B4[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };

/** Runs the object's handler from D_80082EA0, chosen by unk17. */
s32 func_1501370C(TriggerZone *obj) {
    u8 index = obj->subTypeData;
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
    TimedEffect *effect;
    Game40490BeamPacket packet;
    MtxF matrix;

    if (obj->ySize != 0) {
        f32 inverse;
        s16 height;

        packet.height = 2.0f * obj->ySize;
        inverse = 1.0f / packet.height;
        packet.width = obj->xSize;
        packet.rotation[0] = obj->pitch;
        packet.rotation[1] = obj->yaw;
        packet.zero = 0.0f;
        packet.field2C = 0.2001999915f;
        packet.field30 = 0.04999999702f;
        packet.position[0] = obj->x;
        packet.position[1] = obj->y;
        packet.position[2] = obj->z;
        func_150A8050(matrix, obj->pitch, obj->yaw, 0.0f);
        func_150A7960(matrix, 0.0f, packet.height, 0.0f,
                      &packet.direction[0], &packet.direction[1], &packet.direction[2]);
        packet.direction[0] *= inverse;
        packet.direction[1] *= inverse;
        packet.direction[2] *= inverse;
        effect = func_15149130(300, -1, 0x19, -1, 0, 0x17, sizeof(packet), 0xFF, 0);
        if (effect != NULL) {
            memcpy((u8 *)effect + 0x28, &packet, sizeof(packet));
        }
        height = obj->ySize;
        func_1000FA64(0x67C, obj->x, (s16)(obj->y + height), obj->z,
                      12000, height * 2, height / 2, func_1000EF40, 0, 0, 8, 0);
    }
    return 1;
}

/** Runs the object's handler from D_80082ECC, chosen by unk17. */
s32 func_1501396C(TriggerZone *obj) {
    u8 index = obj->subTypeData;
    void (*handler)(void) = D_80082ECC[index];

    if (handler != NULL) {
        handler();
    }
    return 1;
}

/** Spawns the area effect described by entry data1 of D_80095FA0 over the object's footprint. */
// matching: index lives in the (unused) second argument register
s32 func_150139AC(TriggerZone *obj, s32 index) {
    Struct150139ACLocal data;
    Struct150139ACEntry *entry;
    TimedEffect *effect;

    index = obj->data1;
    obj->flags |= 4;
    data.unk0 = obj;
    data.unkC = 0.0f;

    switch (ZONE_AREA_TYPE(obj)) {
    default:
        data.unk4 = 1.0f;
        data.unk10 = 1000.0f;
        break;
    case ZONE_AREA_BOX:
        /* box: area of the footprint */
        data.unk4 = (f32)(obj->xSize * obj->zSize) * 4.0f;
        data.unk10 = (f32)(obj->y + obj->ySize);
        break;
    case ZONE_AREA_CYLINDER:
    case ZONE_AREA_SPHERE:
        /* cylinder: area of the circle */
        data.unk4 = (f32)(obj->xSize * obj->xSize) * 3.141592741f;
        data.unk10 = (f32)(obj->y + obj->ySize);
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
        data.zone = func_1510FD20(obj->x, obj->z);
    } else {
        data.zone = NULL;
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

/** Runs the object's handler from D_80082F28 (chosen by data1) with data2 / 1000 as argument. */
s32 func_15013C38(TriggerZone *obj) {
    s32 index;
    f32 value;
    void (*handler)(TriggerZone *, f32);

    obj->flags |= 4;
    index = obj->data1;

    if ((D_800D2E4C[0x11] & 4) && (level == 0x13)) { // low byte of D_800D2E4C[0x10]
        return 1;
    }

    if (D_800C35EA[0] == 1) {
        if ((D_800C35E8[0] == 15) || (D_800C35E8[0] == 16) || (D_800C35E8[0] == 17)) {
            return 1;
        }
    }

    if (index >= 6) {
        return 1;
    }

    handler = D_80082F28[index];
    if (handler != NULL) {
        value = (f32)obj->data2 * 0.001000000047f;
        handler(obj, value);
    }

    return 1;
}

/** Calls func_151BE850 at the object's position, with a count of data1 (at least 1). */
s32 func_15013D38(TriggerZone *obj) {
    Vec3f pos;
    s32 count;

    obj->flags |= 4;
    pos.x = obj->x;
    pos.y = obj->y;
    pos.z = obj->z;

    count = obj->data1;
    func_151BE850(&pos, obj->yaw, count ? count : 1, (u8)obj->data2, 1, 0xFF, 1);
    return 1;
}

/** Emits particles from the object's position, scaled by its sizeX and sizeY. */
s32 func_15013DE8(TriggerZone *obj) {
    ParticleEmitterDesc params;
    f32 scaleX;
    f32 scaleY;

    obj->flags |= 4;

    scaleX = obj->xSize * 0.05000000075f;
    scaleY = obj->ySize * 0.01999999955f;

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

    func_15149550(&params, 10, 1, obj->data1 ? 2 : 1, 0xFF, 1);
    return 1;
}

s32 func_15013F9C(TriggerZone *obj) {
    func_151CD2C0(obj, 0xFF, 1);
    return 1;
}

/** Runs the object's handler from D_80082F40, chosen by the low byte of data1. */
s32 func_15013FC4(TriggerZone *obj) {
    u8 index = obj->data1;
    void (*handler)(void) = D_80082F40[index];

    if (handler != NULL) {
        handler();
    }
    return 1;
}

/** Registers the object in slot data2 (0-5) of D_800E0900. */
s32 func_15014004(TriggerZone *obj) {
    s32 slot = obj->data2;

    if (slot < 0) {
        return 1;
    }
    if (slot >= 6) {
        return 1;
    }
    D_800E0900[slot] = obj;
    return 1;
}

/** Registers the object as D_800D9A20[data1] (data1 0 or 1). */
s32 func_15014040(TriggerZone *obj) {
    s32 which = obj->data1;

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
        ZoneNode *zone;
        u8 pad10[0x1C];
        s8 unk2C;
        u8 pad2D[7];
    } data;
    TimedEffect *effect;

    obj->flags |= 4;
    data.unk0 = obj;
    data.unk4 = func_15144598(obj);
    data.unk8 = 0.0f;

    func_1510F800(0);
    data.zone = func_1510FD20(obj->x, obj->z);
    data.unk2C = 0;

    effect = func_15149130(300, -1, 0x21, -1, 0, 0, sizeof(data), 0xFF, 1);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
}

/** Calls func_151A9390 for the object with option bits taken from data1 (bit 1 inverted). */
s32 func_15014144(TriggerZone *obj) {
    s32 flags;
    s32 bit8;
    s32 bit4;
    s32 bit2;
    s32 bit1;
    s32 bit16;

    flags = obj->data1;
    bit8 = (flags & 8) ? 8 : 0;
    bit4 = (flags & 4) ? 4 : 0;
    bit2 = (flags & 2) ? 0 : 2;
    bit1 = (flags & 1) ? 1 : 0;
    bit16 = (flags & 0x10) ? 0x10 : 0;

    func_151A9390((u8)(bit16 | bit1 | bit2 | bit4 | bit8), (u8)obj->data2, obj, 0, -10000.0f, 100.0f, -1, 0xFF, 1);
    return 1;
}

s32 func_15014220(TriggerZone *obj) {
    struct {
        f32 unk0;
        TriggerZone *unk4;
        u8 unk8;
    } data;
    TimedEffect *effect;

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

/** Registers the object in slot (low byte of data1, 0-2) of D_800D9AA0. */
s32 func_150142AC(TriggerZone *obj) {
    s32 slot;

    slot = (u8)obj->data1;
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
    TimedEffect *effect;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    f32 scale;

    obj->flags |= 4;
    if (D_80082FA0 >= 2) {
        return 1;
    }

    if ((D_800D2E4C[0x11] & 4) && (level == 0x13)) { // low byte of D_800D2E4C[0x10]
        return 1;
    }

    /* four 16-bit values, in thousandths */
    a = (f32)(obj->data2 & 0xFFFF) * 0.001000000047f;
    b = (f32)((obj->data2 >> 16) & 0xFFFF) * 0.001000000047f;
    c = (f32)(obj->data3 & 0xFFFF) * 0.001000000047f;
    d = (f32)((obj->data3 >> 16) & 0xFFFF) * 0.001000000047f;

    scale = func_1514462C(obj);

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

/** Spawns a StreamEmitter aimed along the object's rotation, configured by the bytes of data1 and data2. */
s32 func_150144B8(TriggerZone *obj) {
    StreamEmitter emitter;
    u32 value; // matching: holds the random number, then the effect address; two locals grow the frame
    struct {
        Vec3f direction;
        MtxF matrix;
    } scratch;

    obj->flags |= 4;
    if (D_80082FA0 >= 2) {
        return 1;
    }
    func_150A8050(scratch.matrix, obj->pitch, obj->yaw, 0.0f);
    func_150A7960(scratch.matrix, 0.0f, 100.0f, 0.0f, &scratch.direction.x, &scratch.direction.y, &scratch.direction.z);
    func_15145974(&scratch.direction, &emitter.angleX, &emitter.angleY);
    emitter.angleX *= 0.017453292f;
    emitter.angleY *= 0.017453292f;
    emitter.minDelay = obj->data1 & 0xFF;
    emitter.delayRange = (obj->data1 >> 8) & 0xFF;
    value = func_150ADA20();
    emitter.position = &obj->x;
    emitter.timer = value % (u32)(emitter.delayRange + 1) + emitter.minDelay;
    emitter.minLife = (obj->data1 >> 16) & 0xFF;
    emitter.lifeRange = (obj->data1 >> 24) & 0xFF;
    emitter.rateBase = (f32)(obj->data2 & 0xFF) * 0.015625f;
    emitter.rateRange = (f32)((obj->data2 >> 8) & 0xFF) * 0.015625f;
    emitter.speed = (f32)((obj->data2 >> 24) & 0xFF);
    emitter.unk30 = 1.0f / emitter.speed;
    emitter.unk34 = (f32)obj->ySize * emitter.unk30;
    emitter.spread = func_150484A0(obj->xSize, obj->ySize);
    emitter.wobbleSpeed = (f32)((obj->data2 >> 16) & 0xFF) * 0.00390625f * 0.7853982f;
    value = (u32)func_15149130(300, -1, 0x2A, -1, 0, 0, sizeof(emitter), 0xFF, 0);
    if (value != 0) {
        memcpy((void *)((u32)value + 0x28U), &emitter, sizeof(emitter));
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
    TimedEffect *effect;
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
    corners[0].x = obj->xSize * c + x + obj->zSize * s;
    corners[0].z = obj->zSize * c + z - obj->xSize * s;
    corners[1].x = obj->xSize * c + x + -obj->zSize * s;
    corners[1].z = -obj->zSize * c + z - obj->xSize * s;
    corners[2].x = -obj->xSize * c + x + -obj->zSize * s;
    corners[2].z = -obj->zSize * c + z - -obj->xSize * s;
    corners[3].x = -obj->xSize * c + x + obj->zSize * s;
    corners[3].z = obj->zSize * c + z - -obj->xSize * s;
    packet.start = corners[1];
    packet.end = corners[2];
    packet.edge[0] = corners[0].x - corners[1].x;
    packet.edge[1] = corners[0].z - corners[1].z;
    packet.edge[2] = corners[3].x - corners[2].x;
    packet.edge[3] = corners[3].z - corners[2].z;
    packet.top = obj->y + obj->ySize;
    packet.bottom = obj->y;
    packet.scaleX = (f32)(obj->data2 & 0xFF) * 0.00390625f * 0.5f;
    packet.scaleZ = (f32)((obj->data2 >> 8) & 0xFF) * 0.00390625f * 0.5f;
    packet.unk30 = 0.0f;
    packet.unk34 = (f32)((obj->data2 >> 16) & 0xFF) * 0.00390625f * 250.0f;
    packet.unk38 = (f32)((obj->data2 >> 24) & 0xFF) * 0.00390625f * 250.0f;
    packet.bytes[0] = obj->data1 & 0xFF;
    packet.bytes[1] = (obj->data1 >> 8) & 0xFF;
    packet.bytes[2] = (obj->data1 >> 16) & 0xFF;
    packet.bytes[3] = (obj->data1 >> 24) & 0xFF;
    effect = func_15149130(300, -1, 0x2C, -1, 0, 0, sizeof(packet), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &packet, sizeof(packet));
    }
    return 1;
}
extern u8 D_800A0938[][3];

typedef struct Game4049014B60Packet {
    f32 pos[3];
    f32 rot[3];
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    EffectHeader *effects[6];
    f32 unk3C;
    void *owner;
    u8 active;
} Game4049014B60Packet;

/** Creates six func_151602C0 objects at the object's position, coloured from D_800A0938, and an effect
 * with a random rotation that owns them (they are freed again if no effect slot is free). */
s32 func_15014B60(TriggerZone *obj) {
    Game4049014B60Packet packet;
    s32 i;
    TimedEffect *effect;
    Vec3i pos;
    LightEffectHeader header;
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
    packet.unk18 = (f32)(obj->data1 & 0xFFFF) * 0.152587890625f * 0.001000000047f;
    packet.unk1C = (f32)((obj->data1 >> 16) & 0xFFFF) * 0.152587890625f * 0.001000000047f;
    packet.unk20 = (f32)(obj->data2 & 0xFFFF) * 0.152587890625f * 0.001000000047f;
    packet.unk3C = (f32)((obj->data2 >> 16) & 0xFFFF) * 0.457763671875f * 9.999999975e-07f;
    pos.x = obj->x;
    pos.y = obj->y;
    pos.z = obj->z;
    for (i = 0; i < 6; i++) {
        s32 r;
        s32 g;

        header.flags = 2;
        header.update = -1;
        header.lifetime = 300;
        header.lightType = 0;
        r = (level == 0x40) ? 0xFF : D_800A0938[i][0];
        g = (level == 0x40) ? 0 : D_800A0938[i][1];
        count = (level == 0x40) ? 0 : D_800A0938[i][2]; // matching: blue reuses count's register
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
    TimedEffect *effect;
    s32 flag;

    obj->flags |= 4;
    data.unk51 = obj->data3;
    flag = (obj->data2 & 1) ? 1 : 0;
    data.unk50 = flag;
    data.unk0 = obj;
    /* data1 holds a minimum and a range: unkC is random in [min, min + range] */
    data.unk4 = obj->data1 & 0xFFFF;
    data.unk8 = (obj->data1 >> 16) & 0xFFFF;
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
    ZoneEffectData data;
    ZoneEffect *effect;
    s32 bit1;
    s32 bit2;
    u32 value;

    obj->inactive = 1;
    data.zone = obj;
    data.unk4 = obj->data2;

    func_1510F800(0);
    data.node = func_1510FD20(obj->x, obj->z);

    value = obj->data3;
    bit1 = (value != 0) ? 1 : 0;
    bit2 = (value != 0) ? 2 : 0;
    data.flags = bit2 | bit1;
    data.actor = NULL;

    effect = func_15149130(300, -1, -1, -1, 0, 0x2C, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
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
        ZoneNode *zone;
    } data;
    TimedEffect *effect;

    obj->flags |= 4;
    obj->inactive = 1;

    data.unk0 = obj;
    data.unk4 = 0.0f;
    data.unk8 = -1;
    data.unkC = obj->x;
    data.unk10 = obj->y;
    data.unk14 = obj->z;
    data.unk18 = obj->xSize;
    data.unk1C = obj->ySize;
    data.unk20 = -10000.0f;
    data.unk40 = 0;
    data.unk3D = 0;
    data.unk3C = 0;
    data.unk38 = 0;

    func_1510F800(0);
    data.zone = func_1510FD20(obj->x, obj->z);

    effect = func_15149130(300, -1, 0x3C, -1, 0, 0x2D, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

/** Runs the object's handler from D_80082F70, chosen by data2 (0 or 1). */
s32 func_15015300(TriggerZone *obj) {
    void (*handler)(void);
    s32 index = obj->data2;

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
    ZoneRectData data;
    ZoneRectEffect *effect;
    f32 z;
    f32 y;
    f32 x;
    f32 corners[4][3];
    MtxF mtx;
    u8 i;

    data.zone = obj;
    data.unk4 = -1;
    data.unk8 = 0.0f;
    obj->inactive = 1;
    x = obj->x;
    y = obj->y;
    z = obj->z;

    corners[0][0] = obj->xSize;
    corners[0][1] = 0.0f;
    corners[0][2] = obj->zSize;
    corners[1][0] = obj->xSize;
    corners[1][1] = 0.0f;
    corners[1][2] = -obj->zSize;
    corners[2][0] = -obj->xSize;
    corners[2][1] = 0.0f;
    corners[2][2] = -obj->zSize;
    corners[3][0] = -obj->xSize;
    corners[3][1] = 0.0f;
    corners[3][2] = obj->zSize;

    func_150A8050(mtx, obj->pitch, obj->yaw, 0.0f);

    mtx[3][0] = x;
    mtx[3][1] = y;
    mtx[3][2] = z;

    for (i = 0; i < 4; i++) {
        func_150A7960(mtx, corners[i][0], corners[i][1], corners[i][2],
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
    data.node = func_1510FD20(obj->x, obj->z);
    data.actor = NULL;

    effect = func_15149130(300, -1, 0x3D, -1, 0, 0x2E, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

s32 func_15015644(TriggerZone *obj) {
    struct {
        TriggerZone *unk0;
        f32 unk4;
        ZoneNode *zone;
        s8 unkC;
    } data;
    TimedEffect *effect;

    obj->flags |= 4;
    obj->inactive = 1;

    data.unk0 = obj;
    data.unk4 = func_15144598(obj);
    func_1510F800(0);
    data.zone = func_1510FD20(obj->x, obj->z);
    data.unkC = 0;

    effect = func_15149130(300, -1, 0x44, -1, 0, 0x2F, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

/** Passes the object's two end points (its position, and sizeY along its rotation from there) to
 * func_151ACBD4, higher end first. In levels 0xB, 0x2C and 0x26 only objects with a large enough data1 do. */
s32 func_150156F4(TriggerZone *obj) {
    Vec3f points[2];
    MtxF mtx;
    RopeSpawn payload;

    if (level == 0xB) {
        if (obj->data1 < 6U) {
            return 1;
        }
    } else if (level == 0x2C) {
        if (obj->data1 < 7U) {
            return 1;
        }
    } else if (level == 0x26) {
        if (obj->data1 < 2U) {
            return 1;
        }
    }

    func_150A8050(mtx, obj->pitch, obj->yaw, 0.0f);
    mtx[3][0] = obj->x;
    mtx[3][1] = obj->y;
    mtx[3][2] = obj->z;

    func_150A7960(mtx, 0.0f, obj->ySize, 0.0f, &points[1].x, &points[1].y, &points[1].z);

    points[0].x = obj->x;
    points[0].y = obj->y;
    points[0].z = obj->z;
    payload.id = obj->data1;

    /* the higher end is the top */
    payload.ends.top = points[points[0].y < points[1].y];
    payload.ends.bottom = points[points[1].y < points[0].y];

    payload.unk20 = 0;
    payload.length = fabsf(points[1].y - points[0].y);
    func_151ACBD4(&payload, 0);

    return 1;
}
