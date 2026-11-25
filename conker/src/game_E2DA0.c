#include <ultra64.h>
/* functions.h types func_150ADA20 as returning u8; golden spills/uses the full u32 return
 * (same shadow-prototype idiom as game_121A20.c). */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

extern s32 D_8009FC3C[][3];
extern f32 D_8009FC60[];
extern f32 D_8009FC6C[];
extern f32 D_8009FC78[];
extern s16 D_8009FC84[];
extern s16 D_8009FC8E[][2];
extern f32 D_8009FCCC;
extern f32 D_8009FCD0;
extern f32 D_8009FCD4;
extern f32 D_8009FCD8;
extern f32 D_8009FCDC;
extern f32 D_8009FCE0;
extern f32 D_8009FCE4;
f32 func_151423D8(u8);
void func_15143794(s32, s32, f32, f32 *);
void func_150B3F5C(void *, void *, s16);
/** Spawns a func_15152B38 effect at pos, plus the func_150B5E34 and func_150B6000 particles. */
void func_150B5A3C(vertex *pos, u8 arg1, s32 arg2);
void func_150B5E34(vertex *pos, u8 arg1, s32 arg2);
void func_150B6000(vertex *pos, u8 arg1, s32 arg2);
void func_150B60E0(struct127 *obj, vertex *out);

/* Effect instance whose per-type data (from 0x28) tracks a game object. */
typedef struct Arg150B5950 {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ struct127 *obj;
    /* 0x2C */ u8 objUniqueId;
} Arg150B5950;

/* Effect template passed to func_15152B38. */
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ vertex pos;
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
} EffectSpawn; /* size 0x74 */

void func_15152B38(EffectSpawn *spawn, u8 arg1, s32 arg2);


typedef struct GameCommand {
    s16 opcode;
    s16 value;
} GameCommand;

/** Emits command 0x1A carrying object objIndex's facing angle (skipped when D_800C35EA == 1). */
GameCommand *func_150B58F0(GameCommand *cmd, s32 objIndex) {
    if (D_800C35EA == 1) {
        return cmd;
    }
    cmd->opcode = 0x1A;
    cmd->value = D_800CC2D0[objIndex].unk7A;
    return cmd + 1;
}

/** Randomly spawns particle bursts at the tracked object; ends the effect (unkE = -1) once it is gone. */
void func_150B5950(Arg150B5950 *arg0) {
    vertex *posp;
    vertex pos;
    struct127 *obj;

    obj = arg0->obj;
    posp = &pos; // matching: IDO keeps &pos in a saved register only via a named pointer
    if ((obj->interaction_state == 0) || (obj->unique_id != arg0->objUniqueId)) {
        arg0->unkE = -1;
        return;
    }

    if (obj->unk1D4 != NULL) {
        if (func_150ADA68() < 0.10824f) {
            pos.x = obj->x_position;
            pos.y = obj->y_position;
            pos.z = obj->z_position;
            func_150B5A3C(posp, arg0->unkC, arg0->unk1);
        }

        if (func_150ADA68() < 0.10824f) {
            func_150B60E0(obj, posp);
            func_150B5A3C(posp, arg0->unkC, arg0->unk1);
        }
    }
}

/** Spawns a func_15152B38 effect at pos, plus the func_150B5E34 and func_150B6000 particles. */
void func_150B5A3C(vertex *pos, u8 arg1, s32 arg2) {
    EffectSpawn spawn;

    spawn.unk00 = 5;
    spawn.unk04 = 5;
    spawn.pos = *pos;
    spawn.unk14 = 20.0f;
    spawn.unk18 = 10.08f;
    spawn.unk1C = -1.353f;
    spawn.unk20 = 0.731f;
    spawn.unk24 = 39.0f;
    spawn.unk28 = 35.0f;
    spawn.unk2C = 0;
    spawn.unk2E = 0xFF;
    spawn.unk30 = -31;
    spawn.unk32 = 80;
    spawn.unk34 = 3;
    spawn.unk38 = 2;
    spawn.unk3C = 20;
    spawn.unk3E = 30;
    spawn.unk40 = 1;
    spawn.unk42 = 4;
    spawn.unk43 = 2;
    spawn.unk44 = 3;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0xFF;
    spawn.unk47 = 0xFF;
    spawn.unk48 = 0xFF;
    spawn.unk49 = 0;
    spawn.unk4A = 0;
    spawn.unk4B = 0;
    spawn.unk4C = 0;
    spawn.unk4D = 0xFF;
    spawn.unk4E = 0xFF;
    spawn.unk4F = 0xFF;
    spawn.unk50 = 0xFF;
    spawn.unk51 = 0;
    spawn.unk52 = 0;
    spawn.unk53 = 0;
    spawn.unk54 = 0;
    spawn.unk55 = 0xFF;
    spawn.unk56 = 0;
    spawn.unk57 = 3;
    spawn.unk58 = 36;
    spawn.unk5C = 0x200005;
    spawn.unk60 = 0x60600;
    spawn.unk64 = 8;
    spawn.unk66 = 31;
    spawn.unk68 = 1;
    spawn.unk6A = 0;
    spawn.unk6C = 1.0f;
    spawn.unk70 = -1;
    spawn.unk71 = 0;
    spawn.unk72 = -1;
    spawn.unk73 = -1;

    func_15152B38(&spawn, arg1, arg2);
    func_150B5E34(pos, arg1, arg2);
    func_150B6000(pos, arg1, arg2);
}

/** Larger variant of func_150B5A3C (different template values). */
void func_150B5C38(vertex *pos, u8 arg1, s32 arg2) {
    EffectSpawn spawn;

    spawn.unk00 = 13;
    spawn.unk04 = 8;
    spawn.pos = *pos;
    spawn.unk14 = 15.88f;
    spawn.unk18 = 36.1f;
    spawn.unk1C = -3.603000164f;
    spawn.unk20 = 2.166000128f;
    spawn.unk24 = 152.0f;
    spawn.unk28 = 100.0f;
    spawn.unk2C = 0;
    spawn.unk2E = 0xFF;
    spawn.unk30 = -20;
    spawn.unk32 = 50;
    spawn.unk34 = 3;
    spawn.unk38 = 2;
    spawn.unk3C = 20;
    spawn.unk3E = 15;
    spawn.unk40 = 1;
    spawn.unk42 = 4;
    spawn.unk43 = 2;
    spawn.unk44 = 3;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0xFF;
    spawn.unk47 = 0xB4;
    spawn.unk48 = 0xFF;
    spawn.unk49 = 0;
    spawn.unk4A = 0;
    spawn.unk4B = 0;
    spawn.unk4C = 0;
    spawn.unk4D = 0xFF;
    spawn.unk4E = 0xFF;
    spawn.unk4F = 0xB4;
    spawn.unk50 = 0xFF;
    spawn.unk51 = 0;
    spawn.unk52 = 0;
    spawn.unk53 = 0;
    spawn.unk54 = 0;
    spawn.unk55 = 0xFF;
    spawn.unk56 = 0;
    spawn.unk57 = 3;
    spawn.unk58 = 36;
    spawn.unk5C = 0x200005;
    spawn.unk60 = 0x60600;
    spawn.unk64 = 8;
    spawn.unk66 = 31;
    spawn.unk68 = 1;
    spawn.unk6A = 0;
    spawn.unk6C = 1.0f;
    spawn.unk70 = -1;
    spawn.unk71 = 0;
    spawn.unk72 = -1;
    spawn.unk73 = -1;

    func_15152B38(&spawn, arg1, arg2);
    func_150B5E34(pos, arg1, arg2);
    func_150B6000(pos, arg1, arg2);
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
    vertex pos;
    vertex unk3C;
    vertex vel;
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
} Func150B5E34Spawn; /* 0x70 */

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_150B5E34(vertex *pos, u8 arg1, s32 arg2) {
    Func150B5E34Spawn spawn;
    s32 r2;
    s32 r1;

    spawn.unk1D = 0x2B;
    spawn.unk8 = 0x4403;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0x20000;
    spawn.unkA = func_150ADA20() % 7U + 4;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk14 = 0xFF;
    spawn.unk15 = 0xFF;
    spawn.unk16 = 0xFF;
    spawn.unk17 = 0xFF;
    spawn.unk18 = 0xFF;
    spawn.unk19 = 0xFF;
    spawn.unk1A = 0xFF;
    spawn.unk1B = 0xFF;
    spawn.unk1C = 0xFF;
    spawn.unk28 = spawn.unk2C = func_150ADA68() * 500.0f + 500.0f;
    spawn.pos = *pos;
    spawn.unk1E = 3;
    spawn.unk20 = 0x55;
    spawn.unk22 = 1;
    spawn.unk3C.x = 0.0f;
    spawn.unk3C.y = 0.0f;
    spawn.unk3C.z = 0.0f;
    spawn.vel.x = 0.0f;
    spawn.vel.y = 0.0f;
    spawn.vel.z = 0.0f;
    spawn.unk54 = 0.0f;
    spawn.unk24 = 1.0f;
    r1 = (func_150ADA20() & 1) ? 0x40 : 0;
    r2 = (func_150ADA20() & 1) ? 0x80 : 0;
    spawn.unk58 = r2 | 1 | r1 | 0x200 | 0x4000 | 0x8000;
    spawn.unk60 = 6;
    spawn.unk61 = 6;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 4;
    func_15130280(&spawn, 1, 0, 0, arg1, arg2);
}

void func_150B6000(vertex *pos, u8 arg1, s32 arg2) {
    Header header;
    Header2 header2;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() % 9U) + 3;
    header.unk4 = 0;

    header2.unk0 = pos->x;
    header2.unk4 = pos->y;
    header2.unk8 = pos->z;

    func_151602C0(&header, &header2, (func_150ADA20() % 0xC9U) + 0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg1, arg2);
}

extern void func_15143134(vertex *in, vertex *out, void *mtx);
extern vertex D_8009FC30;

/** Transforms the fixed point D_8009FC30 by the matrix at obj->unk1D4 + 0x140 into out. */
void func_150B60E0(struct127 *obj, vertex *out) {
    func_15143134(&D_8009FC30, out, (u8 *)obj->unk1D4 + 0x140);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    u8 padA[0xA];
    u8 unk14;
} Obj150B6110;

typedef struct {
    Obj150B6110 *obj;
    f32 accum;
} Data150B6110;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Obj150B6110 *unk28;
    f32 unk2C;
    u8 unk30;
} Arg150B6110;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec150B6110;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s32 unk20;
    s32 unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
} Desc150B6110;

extern f32 D_8009FCBC;
extern f32 D_8009FCC0;
extern f32 D_8009FCC4;
extern f32 D_8009FCC8;
extern void func_1514373C(f32 arg0, f32 arg1, f32 *arg2, f32 *arg3);
extern void func_15106F98(Vec150B6110 *, Vec150B6110 *, s32, Desc150B6110 *, f32, s32, s32, s32);

void func_150B6110(Arg150B6110 *arg0) {
    Data150B6110 *data;
    Obj150B6110 *obj;
    u8 prev;
    u8 a;

    obj = arg0->unk28;
    prev = arg0->unk30;
    arg0->unk30 = obj->unk14;
    if (obj->unk14 == 1) {
        if (arg0->unk30 != prev) {
            func_151494E0(0, 0x4A);
        }
        return;
    }
    data = (Data150B6110 *)((u8 *)arg0 + 0x28);
    data->accum += (D_8009FCBC + func_150ADA68() * D_8009FCC0) * D_800BE9A4;
    if (data->accum > 1.0f) {
        Desc150B6110 desc;
        Vec150B6110 from;
        Vec150B6110 to;
        f32 size;

        desc.unk0 = 35.0f;
        desc.unk4 = 35.0f;
        desc.unk8 = 76.0f;
        desc.unkC = 103.0f;
        desc.unk10 = D_8009FCC4;
        desc.unk14 = 0x28;
        desc.unk16 = 0;
        desc.unk18 = 4;
        desc.unk1A = 3;
        desc.unk1C = 7;
        desc.unk20 = 4;
        desc.unk24 = 3;
        desc.unk28 = 0x7E;
        desc.unk29 = 0xF9;
        desc.unk2A = 0xFF;
        desc.unk2B = 0x7F;
        desc.unk2C = 0x80;
        size = D_8009FCC8;
        from.y = obj->unk2 + obj->unk8;
        to.y = obj->unk2 + obj->unk8;
        do {
            a = func_150ADA20();
            func_1514373C((u32)a, obj->unk6, &from.x, &from.z);
            from.x += obj->unk0;
            from.z += obj->unk4;
            a += func_150ADA20() % 129 + 64;
            func_1514373C((u32)a, obj->unk6, &to.x, &to.z);
            to.x += obj->unk0;
            to.z += obj->unk4;
            func_15106F98(&from, &to, 4, &desc, size, 1, arg0->unkC, arg0->unk1);
            data->accum -= 1.0f;
        } while (data->accum > 1.0f);
    }
}

void func_150B6450(struct102 *arg0, s32 arg1, u8 arg2) {
    if (arg2 == 0x4A) {
        func_1516972C(arg0);
    }
}

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    u8 unk10;
    u8 pad11[3];
    f32 unk14;
    f32 unk18;
    u8 unk1C;
    u8 pad1D[3];
    f32 unk20;
    u8 unk24;
    u8 pad25;
    s16 unk26;
    f32 unk28;
    f32 unk2C;
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 unk33;
    u8 unk34;
    u8 unk35;
    s16 unk36;
    u8 unk38;
    u8 pad39[3];
    f32 unk3C;
} Stack150B648C;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3F150B648C;

void func_150B648C(u8 arg0) {
    Stack150B648C sp30;
    Vec3F150B648C sp24;

    if (arg0 == 0) {
        sp30.unk00 = func_151423D8(0xAA) * 10.0f;
        sp30.unk04 = 3.0f;
        sp30.unk08 = func_151423D8(0xEA) * 10.0f;
        sp24.x = D_8009FCCC - (func_151423D8(0xAA) * 150.0f);
        sp24.y = D_8009FCD0;
        sp24.z = D_8009FCD4 - (func_151423D8(0xEA) * 150.0f);
    } else if (arg0 == 1) {
        func_15143794(-0x45, 0x1C, 9.349999f, &sp30.unk00);
        sp24 = *(Vec3F150B648C *)D_8009FC3C[arg0];
    } else {
        func_15143794(0x18, 0xF, 9.349999f, &sp30.unk00);
        sp24 = *(Vec3F150B648C *)D_8009FC3C[arg0];
    }

    sp30.unk10 = 4;
    sp30.unk1C = 6;
    sp30.unk0C = D_8009FC60[arg0];
    sp30.unk24 = 0x80;
    sp30.unk26 = 0xFF;
    sp30.unk30 = 2;
    sp30.unk31 = 5;
    sp30.unk32 = 5;
    sp30.unk33 = 0x33;
    sp30.unk34 = 3;
    sp30.unk35 = 0x55;
    sp30.unk38 = 0;
    sp30.unk14 = D_8009FCD8;
    sp30.unk18 = D_8009FCDC;
    sp30.unk20 = D_8009FCE0;
    sp30.unk28 = D_8009FC6C[arg0];
    sp30.unk2C = D_8009FC78[arg0];
    sp30.unk36 = D_8009FC84[arg0];
    sp30.unk3C = D_8009FCE4;
    func_150B3F5C(&sp30, &sp24, D_8009FC8E[arg0][0]);
}

struct inner150B66DC {
    u8 pad[0x68];
    u8 unk68;
};
struct sub150B66DC {
    u8 pad0[0x14];
    u8 *unk14;
    struct inner150B66DC *unk18;
};

s32 func_150B66DC(struct sub150B66DC *arg0) {
    struct inner150B66DC *p = arg0->unk18;
    switch (p->unk68 - 0xF) {
    case 0:
        arg0->unk14[9] = 1;
        break;
    case 1:
        arg0->unk14[9] = 0;
        arg0->unk14[0x2F] = 0x14;
        break;
    case 2:
    default:
        arg0->unk14[9] = 0;
        arg0->unk14[0x2F] = 0x28;
        break;
    }
    return 1;
}

extern void func_15182670(s32, s32, s32, s32, s32, s32, s32, s32);

void func_150B6754(u8 arg0, s32 arg1) {
    u32 temp1;
    u32 temp0;

    temp0 = func_150ADA20();
    temp1 = func_150ADA20();
    func_15182670(0xCC, 0xCC, 0xFF, (u8)(temp0 % 0x38 + 0xC8), temp1 % 0xB + 0xF, 0, arg0, arg1);
}
