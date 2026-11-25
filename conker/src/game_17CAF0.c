#include <ultra64.h>
#define func_15169260 func_15169260_s32_decl
#define func_150ADA20 func_150ADA20_hdr_decl
#include "functions.h"
#undef func_15169260
#undef func_150ADA20
#include "variables.h"

extern s32 func_150ADA20(void);

struct Vec3F { f32 x; f32 y; f32 z; };
struct ConeBasis;
extern s32 func_15146078(struct Vec3F *, struct Vec3F *, struct Vec3F *);
extern s32 func_15144E80(s16 *, struct Vec3F *, struct Vec3F *, struct Vec3F *);
extern f32 func_15144A74(struct Vec3F *, struct Vec3F *);
extern s32 func_15145128(struct Vec3F *, struct Vec3F *, f32 *, f32 *);
struct ConeSprayConfig;
extern void func_1514F8F8(struct ConeSprayConfig *, struct Vec3F *, struct Vec3F *, struct Vec3F *, f32, u8, s32);
extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern f32 D_800A601C;
extern f32 D_800A6020;
extern f32 D_800A6024;
extern f32 D_800A6028;
extern f32 D_800A602C;
extern f32 D_800A6004;
extern f32 D_800A6008;
extern f32 D_800A600C;
extern f32 D_800A6010;
extern f32 D_800A6014;
extern f32 D_800A6018;
extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, s16 *, f32 *, f32 *, f32 *, f32 *, s32 *, s32, f32);
extern s32 func_15145C90(s32);

struct Local1515572CData {
    s32 unk0;
    s32 unk4;
};

extern struct Local1515572CData D_800A6038;
extern void func_15169260(struct Local1515572CData *, s32, s32, u8);

struct AngleRange {
    s16 yawBase;
    s16 yawRange;
    s16 pitchBase;
    s16 pitchRange;
};

struct Local15150D1CArg {
    s32 countBase;
    s32 countRange;
    struct Vec3F origin;
    struct AngleRange angles;
    f32 maxDistance;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    s16 unk2A;
    u8 unk2C;
};

struct Local151539B4Header {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Local151539B4Arg {
    s16 unk0;
    s16 unk2;
    struct Local151539B4Header unk4;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 pad31;
    s16 unk32;
    s16 unk34;
    u16 unk36;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 unk41;
    s16 unk42;
    s16 unk44;
    u8 unk46;
};

struct Local151539B4Spawn {
    struct Local151539B4Header unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 pad29;
    s16 unk2A;
    u16 unk2C;
    u8 pad2E[2];
    f32 unk30;
    u8 unk34;
    u8 pad35;
    s16 unk36;
    s16 unk38;
    u8 pad3A[2];
};

struct Local15153CCCArg {
    struct Local151539B4Header unk0;
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
};

extern void func_15143794(s16, s16, f32, f32 *);
extern void func_151C5F44(s32, f32 *, f32, f32, s32, s32, f32, s32, s32, u8, s32);
extern s32 (*D_8008ACC8[])(void *);
extern void func_15156190(void *, u8, s32, u8, s32);
extern f32 D_800A5FFC;
extern f32 D_800A6000;
extern void func_1514F808(struct ConeBasis *, f32, struct Vec3F *);
extern void func_151DA6F8(struct Local15153CCCArg *, struct Vec3F *, f32, s16, s32, f32, s32, s32, f32, f32, s32, u8, s32, s16, s16, s32, u8, s32);

extern void func_1515C2F0(void *, s32, void *, s32, u8, s32);

extern s32 func_15147DA0(void *, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32 *, s32, u8, s32);

struct Local15152B38Header {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Local15152B38Arg {
    s32 unk0;
    s32 unk4;
    struct Local15152B38Header unk8;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s32 unk34;
    s32 unk38;
    s16 unk3C;
    s16 unk3E;
    u16 unk40;
    u8 unk42;
    u8 unk43;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    u8 unk48;
    u8 unk49;
    u8 unk4A;
    u8 unk4B;
    u8 unk4C;
    u8 unk4D;
    u8 unk4E;
    u8 unk4F;
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    s32 unk60;
    s16 unk64;
    s16 unk66;
    s16 unk68;
    u8 unk6A;
    u8 pad6B;
    f32 unk6C;
    s8 unk70;
    s8 unk71;
};

struct Local15152B38Param {
    struct Local15152B38Header unk0;
    s16 unkC;
    u16 unkE;
    s32 unk10;
    u8 pad14;
    s8 unk15;
    u8 pad16[2];
    u8 pad18[4];
};

struct Local15152B38Spawn {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    s8 unk17;
    s8 unk18;
    s8 unk19;
    s8 unk1A;
    s8 unk1B;
    s8 unk1C;
    s8 unk1D;
    s8 unk1E;
    s8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 pad22[2];
    s32 unk24;
    s32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    u8 unk32;
    u8 pad33;
    f32 unk34;
    s8 unk38;
    s8 unk39;
    u8 pad3A[2];
};

struct Local15153634Block {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Local15153634Arg {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 pad5;
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    struct Local15153634Block unk38;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    s32 unk5C;
    s8 unk60;
    s8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 pad65[3];
    f32 unk68;
};

struct Local15153634Spawn {
    s32 unk00;
    s32 unk04;
    u16 unk08;
    s16 unk0A;
    s32 unk0C;
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
    struct Local15153634Block unk30;
    u8 pad3C[0xC];
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    u8 pad64[2];
    u8 unk66;
    u8 pad67[9];
};

/* A random direction inside a cone around `direction`, built lazily (see func_1514F6E8). */
struct ConeBasis {
    u8 flags; /* 1: direction normalized and scaled, 2: axes built, 4: axes normalized */
    u8 pad1[3];
    struct Vec3F direction;
    struct Vec3F axis0;
    struct Vec3F axis1;
    f32 spread;
};

/* A cone emitter described by a surface triangle (three s16 vertices) and the side its normal faces. */
struct TriangleSource {
    struct Vec3F normal;
    s16 triangle[9];
    f32 spread;
};

struct Local1514FF44Arg;

/* Builds a cone basis from a triangle: the axes are two triangle edges, the direction is its normal,
 * flipped to face the same side as source->normal. */
void func_1514F640(struct TriangleSource *source, struct ConeBasis *cone) {
    cone->flags = 2;
    cone->spread = source->spread;
    if (func_15144E80(source->triangle, &cone->axis0, &cone->axis1, &cone->direction) != 0) {
        if (func_15144A74(&cone->direction, &source->normal) < 0.0f) {
            cone->direction.x = -cone->direction.x;
            cone->direction.y = -cone->direction.y;
            cone->direction.z = -cone->direction.z;
        }
    }
}

/* Finishes whatever parts of the cone basis are still missing; returns 0 if a vector is degenerate. */
s32 func_1514F6E8(struct ConeBasis *cone) {
    if (!(cone->flags & 1)) {
        if (func_15145128(&cone->direction, &cone->direction, NULL, NULL) == 0) {
            return 0;
        }
        cone->direction.x *= 1000.0f;
        cone->direction.y *= 1000.0f;
        cone->direction.z *= 1000.0f;
        cone->flags |= 1;
    }
    if (!(cone->flags & 2)) {
        if (func_15146078(&cone->direction, &cone->axis0, &cone->axis1) == 0) {
            return 0;
        }
        cone->flags |= 2 | 4;
    }
    if (!(cone->flags & 4)) {
        if (func_15145128(&cone->axis0, &cone->axis0, NULL, NULL) == 0) {
            return 0;
        }
        if (func_15145128(&cone->axis1, &cone->axis1, NULL, NULL) == 0) {
            return 0;
        }
        cone->flags |= 4;
    }
    return 1;
}

extern void func_15143874(s32, f32, f32 *, f32 *);

/* Picks a random direction inside the cone and scales it by `speed`. */
void func_1514F808(struct ConeBasis *cone, f32 speed, struct Vec3F *out) {
    f32 second;
    f32 first;

    func_15143874((s16)(func_150ADA20() & 0xFF), func_150ADA68() * cone->spread, &first, &second);
    out->x = (((cone->axis0.x * first) + (cone->axis1.x * second)) + cone->direction.x) * speed;
    out->y = (((cone->axis0.y * first) + (cone->axis1.y * second)) + cone->direction.y) * speed;
    out->z = (((cone->axis0.z * first) + (cone->axis1.z * second)) + cone->direction.z) * speed;
}

struct ConeSprayConfig {
    s32 countBase;
    s32 countRange;
    struct Vec3F position;
    f32 speedBase;
    f32 speedRange;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s32 unk2C;
    s32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s8 unk50;
};

/* An emitter aimed along a direction (func_1514FB98). */
struct DirectionEmitter {
    struct Vec3F direction;
    f32 spread;
    struct ConeSprayConfig config;
};

/* An emitter aimed off a surface triangle (func_1514FBFC). */
struct TriangleEmitter {
    struct TriangleSource source;
    struct ConeSprayConfig config;
};

extern void func_151A2AD4(struct Vec3F *, struct Vec3F *, f32, f32, s32, f32, f32, f32, f32, s16, s16, s16, s16, s16, s16, s8, u8, s32);

/* Spawns a random number of particles from config->position, each moving in a random direction inside
 * the cone around `direction` (scaled by 1000 here) spanned by axis0/axis1. */
void func_1514F8F8(struct ConeSprayConfig *config, struct Vec3F *direction, struct Vec3F *axis0, struct Vec3F *axis1, f32 spread, u8 arg5, s32 arg6) {
    s32 count;
    struct Vec3F velocity;
    f32 second;
    f32 first;
    f32 speed;

    direction->x *= 1000.0f;
    direction->y *= 1000.0f;
    direction->z *= 1000.0f;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        speed = (func_150ADA68() * config->speedRange) + config->speedBase;
        func_15143874((s16)(func_150ADA20() & 0xFF), func_150ADA68() * spread, &first, &second);
        velocity.x = ((axis0->x * first) + (axis1->x * second) + direction->x) * speed;
        velocity.y = ((axis0->y * first) + (axis1->y * second) + direction->y) * speed;
        velocity.z = ((axis0->z * first) + (axis1->z * second) + direction->z) * speed;
        func_151A2AD4(&config->position, &velocity,
                      (func_150ADA68() * config->unk20) + config->unk1C,
                      (func_150ADA68() * config->unk28) + config->unk24,
                      (func_150ADA20() % (u32)(config->unk30 + 1)) + config->unk2C,
                      config->unk34, config->unk38, config->unk3C, config->unk40,
                      config->unk44, config->unk46, config->unk48, config->unk4A,
                      config->unk4C, config->unk4E, config->unk50, arg5, arg6);
        count--;
    }
}

void func_1514FB98(struct DirectionEmitter *emitter, u8 arg1, s32 arg2) {
    struct Vec3F axis0;
    struct Vec3F axis1;

    if (func_15146078(&emitter->direction, &axis0, &axis1) != 0) {
        func_1514F8F8(&emitter->config, &emitter->direction, &axis0, &axis1, emitter->spread, arg1, arg2);
    }
}

void func_1514FBFC(struct TriangleEmitter *emitter, u8 arg1, s32 arg2) {
    struct Vec3F axis0;
    f32 length0;
    f32 inverse0;
    struct Vec3F axis1;
    f32 length1;
    f32 inverse1;
    struct Vec3F normal;
    f32 normalLength;
    f32 normalInverse;

    if (func_15144E80(emitter->source.triangle, &axis0, &axis1, &normal) != 0) {
        func_15145128(&axis0, &axis0, &length0, &inverse0);
        func_15145128(&axis1, &axis1, &length1, &inverse1);
        func_15145128(&normal, &normal, &normalLength, &normalInverse);
        if (func_15144A74(&normal, &emitter->source.normal) < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        func_1514F8F8(&emitter->config, &normal, &axis0, &axis1, emitter->source.spread, arg1, arg2);
    }
}

struct Local1514FCE8 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s32 countBase;
    s32 countRange;
    struct Vec3F position;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    s32 unk34;
    s32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    s8 unk58;
};

extern void func_151A2A14(struct Vec3F *, s16, s16, f32, f32, f32, s32, f32, f32, f32, f32, s16, s16, s16, s16, s16, s16, s8, u8, s32);

void func_1514FCE8(struct Local1514FCE8 *config, u8 arg1, s32 arg2) {
    s32 count;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        func_151A2A14(&config->position,
                      (s16)((func_150ADA20() % (u32)(config->unk2 + 1)) + config->unk0),
                      (s16)((func_150ADA20() % (u32)(config->unk6 + 1)) + config->unk4),
                      (func_150ADA68() * config->unk20) + config->unk1C,
                      (func_150ADA68() * config->unk28) + config->unk24,
                      (func_150ADA68() * config->unk30) + config->unk2C,
                      (func_150ADA20() % (u32)(config->unk38 + 1)) + config->unk34,
                      config->unk3C, config->unk40, config->unk44, config->unk48,
                      config->unk4C, config->unk4E, config->unk50,
                      config->unk52, config->unk54, config->unk56,
                      config->unk58, arg1, arg2);
        count--;
    }
}

struct Local1514FF44Arg {
    struct Vec3F position;
    s16 countBase;
    s16 countRange;
    f32 speedBase;
    f32 speedRange;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 unk25;
    u8 pad26[2];
    f32 unk28;
    f32 unk2C;
    u8 pad30;
    u8 unk31;
    u8 pad32[2];
    f32 unk34; /* probability */
    u8 unk38;
    u8 pad39[3];
    f32 unk3C; /* probability */
};

extern void func_1514F640(struct TriangleSource *, struct ConeBasis *);
extern void func_1514FF44(struct ConeBasis *, struct Local1514FF44Arg *, s32, u8, s32);
extern void func_151D9014(struct Vec3F *, struct Vec3F *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, u8, s32);

void func_1514FEFC(struct TriangleSource *source, struct Local1514FF44Arg *config, s32 arg2, u8 arg3, s32 arg4) {
    struct ConeBasis cone;

    func_1514F640(source, &cone);
    func_1514FF44(&cone, config, arg2, arg3, arg4);
}

/* Spawns a random number of particles, each moving in a random direction inside the cone. */
void func_1514FF44(struct ConeBasis *cone, struct Local1514FF44Arg *config, s32 arg2, u8 arg3, s32 arg4) {
    s32 count;
    struct Vec3F velocity;

    if (func_1514F6E8(cone) != 0) {
        count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
        while (count != 0) {
            func_1514F808(cone, (func_150ADA68() * config->speedRange) + config->speedBase, &velocity);
            func_151D9014(&config->position, &velocity, config->unk31,
                          (func_150ADA68() * config->unk20) + config->unk1C,
                          (func_150ADA20() % (u32)(config->unk1A + 1)) + config->unk18,
                          (func_150ADA20() % (u32)(config->unk25 + 1)) + config->unk24,
                          (func_150ADA68() * config->unk2C) + config->unk28,
                          func_150ADA68() < config->unk34, 1.4f, 1.4f, 1, arg2, config->unk38,
                          func_150ADA68() < config->unk3C, arg3, arg4);
            count--;
        }
    }
}


/* Like func_1514FF44, but each direction is a random yaw/pitch from `angles`. */
void func_15150178(struct AngleRange *angles, struct Local1514FF44Arg *config, s32 arg2, u8 arg3, s32 arg4) {
    s32 count;
    struct Vec3F velocity;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        func_15143794((s16)((func_150ADA20() % (u32)(angles->yawRange + 1)) + angles->yawBase),
                      (s16)((func_150ADA20() % (u32)(angles->pitchRange + 1)) + angles->pitchBase),
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &velocity.x);
        func_151D9014(&config->position, &velocity, config->unk31,
                      (func_150ADA68() * config->unk20) + config->unk1C,
                      (func_150ADA20() % (u32)(config->unk1A + 1)) + config->unk18,
                      (func_150ADA20() % (u32)(config->unk25 + 1)) + config->unk24,
                      (func_150ADA68() * config->unk2C) + config->unk28,
                      func_150ADA68() < config->unk34, 1.4f, 1.4f, 1, arg2, config->unk38,
                      func_150ADA68() < config->unk3C, arg3, arg4);
        count--;
    }
}

struct Arg15150400 {
    s32 countBase;
    s32 countRange;
    struct Vec3F origin;
    f32 speedBase;
    f32 speedRange;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    f32 scaleBase;
    f32 scaleRange;
    f32 spin; /* spawn.spin.x/z are random in [-spin, spin] */
    s32 unk34;
    f32 unk38; /* probability */
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    s32 *variantIds;
    f32 *variantScales;
    u32 variantCount;
    f32 radius;
    u8 flags;
    u8 unk59;
    u8 pad5A[2];
    s32 extraSize;
    void *extraData;
};

/* Spawn descriptor for func_1513264C. */
typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    struct Vec3F rotation; /* degrees */
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct Vec3F position;
    struct Vec3F velocity;
    struct Vec3F spin;
    f32 unk4C;
    s32 flags;
    s16 unk54;
    s16 variantId;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    s8 unk69;
    u8 unk6A;
    u8 pad6B;
    s32 unk6C;
    u8 unk70;
    u8 pad71;
    s16 unk72;
    s16 unk74;
    u8 pad76[2];
    s32 unk78;
} Spawn1513264C;

extern void *func_1513264C(Spawn1513264C *spawn, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);

/* Spawns a random number of pieces in random directions around config->origin; each picks a random
 * entry of the variant tables and gets a copy of config->extraData. */
void func_15150400(struct Arg15150400 *config, struct AngleRange *angles, u8 arg2, s32 arg3) {
    s32 count;
    Spawn1513264C spawn;
    s16 yaw;
    s16 pitch;

    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.spin.y = 0.0f;
    spawn.flags = 0x1 | 0x8 | 0x20 | 0x40 | 0x80 | 0x100 | ((config->flags & 1) ? 0x800 : 0) | 0x2000 | 0x10000;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk62 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 2;
    spawn.unk69 = -1;
    spawn.unk6A = 0;
    spawn.unk6C = 0;
    spawn.unk70 = 0;
    spawn.unk72 = 12;
    spawn.unk74 = 21;
    spawn.unk78 = 0;
    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        yaw = (func_150ADA20() % (u32)(angles->yawRange + 1)) + angles->yawBase;
        pitch = (func_150ADA20() % (u32)(angles->pitchRange + 1)) + angles->pitchBase;
        if (func_150ADA68() < config->unk38) {
            spawn.unk61 = 13;
            spawn.unk63 = config->unk59;
        } else {
            spawn.unk61 = 8;
            spawn.unk63 = 0;
        }
        spawn.unk04 = (func_150ADA68() * config->unk40) + config->unk3C;
        spawn.rotation.x = func_150ADA68() * 360.0f;
        spawn.rotation.y = func_150ADA68() * 360.0f;
        spawn.rotation.z = func_150ADA68() * 360.0f;
        func_15143794(yaw, pitch, (func_150ADA68() * config->speedRange) + config->speedBase, &spawn.velocity.x);
        spawn.spin.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
        spawn.spin.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
        spawn.unk4C = (func_150ADA68() * config->unk20) + config->unk1C;
        spawn.unk54 = (func_150ADA20() % (u32)(config->unk26 + 1)) + config->unk24;
        func_15143794(yaw, pitch, config->radius, &spawn.position.x);
        spawn.position.x += config->origin.x;
        spawn.position.y += config->origin.y;
        spawn.position.z += config->origin.z;
        {
        u32 variant;
        void *piece;
        variant = func_150ADA20() % config->variantCount;
        spawn.variantId = config->variantIds[variant];
        spawn.unk0C = (config->scaleBase + (func_150ADA68() * config->scaleRange)) * config->variantScales[variant];
        spawn.unk08 = spawn.unk0C;
        spawn.unk00 = config->unk44 * spawn.unk08;
        piece = func_1513264C(&spawn, (config->flags & 1) ? 3 : 0, 0xFF, config->unk34, config->extraSize, arg2, arg3);
        if (config->extraSize > 0 && piece != NULL) {
            memcpy((u8 *)piece + 0x170, config->extraData, config->extraSize);
        }
        }
        count--;
    }
}

struct Arg1515080C {
    s32 countBase;
    s32 countRange;
    struct Vec3F origin;
    struct AngleRange angles;
    f32 speedBase;
    f32 speedRange;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    f32 scaleBase;
    f32 scaleRange;
    f32 spin;
    f32 unk3C; /* probability */
    u8 unk40;
};

/* Like func_15150400, but the variant tables are passed in and each variant is used at most once. */
void func_1515080C(struct Arg1515080C *config, s32 *variantIds, f32 *variantScales, s32 variantCount, void *extraData, s32 extraSize, u8 arg6, u8 arg7, s8 arg8, s32 arg9, f32 radius, u8 argB, u8 argC, u8 argD, s32 argE) {
    u8 *used;
    Spawn1513264C spawn;
    s32 remaining;
    s32 count;
    s32 slot;
    s32 done;
    s16 yaw;
    s16 pitch;

    used = allocate_memory(variantCount, 2, 2, 1);
    if (used != NULL) {
        {
        s32 i;
        for (i = 0; i < variantCount; i++) {
            used[i] = FALSE;
        }
        }
        remaining = variantCount;
        spawn.unk00 = 1.0f;
        spawn.unk1C = 1.0f;
        spawn.unk20 = 1.0f;
        spawn.unk24 = 1.0f;
        spawn.unk04 = 0.5610000491f;
        spawn.spin.y = 0.0f;
        spawn.flags = 0x8 | 0x20 | 0x40 | (arg7 ? 0x80 : 0) | 0x100 | 0x800 | 0x1000 | 0x2000 | 0x10000 | (argB ? 0x4000 : 0) | (argC ? 0x100000 : 0);
        count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
        if (variantCount < count) {
            count = variantCount;
        }
        spawn.unk58 = arg6;
        spawn.unk5C = 0;
        spawn.unk60 = 0xFF;
        spawn.unk62 = 0;
        spawn.unk64 = 0;
        spawn.unk65 = 0;
        spawn.unk66 = 0;
        spawn.unk67 = 0;
        spawn.unk68 = 2;
        spawn.unk6A = 0;
        spawn.unk6C = 0;
        spawn.unk70 = 0;
        spawn.unk72 = 12;
        spawn.unk74 = 21;
        spawn.unk69 = arg8;
        while (count != 0) {
            yaw = (func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase;
            pitch = (func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase;
            func_15143794(yaw, pitch, (func_150ADA68() * config->speedRange) + config->speedBase, &spawn.velocity.x);
            func_15143794(yaw, pitch, radius, &spawn.position.x);
            spawn.position.x += config->origin.x;
            spawn.position.y += config->origin.y;
            spawn.position.z += config->origin.z;
            spawn.rotation.x = func_150ADA68() * 360.0f;
            spawn.rotation.y = func_150ADA68() * 360.0f;
            spawn.rotation.z = func_150ADA68() * 360.0f;
            spawn.spin.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            spawn.spin.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            spawn.unk4C = (func_150ADA68() * config->unk28) + config->unk24;
            spawn.unk54 = (func_150ADA20() % (u32)(config->unk2E + 1)) + config->unk2C;
            {
            /* pick the skip-th unused variant, wrapping around */
            u32 skip;
            skip = func_150ADA20() % (u32)remaining;
            slot = 0;
            done = FALSE;
            do {
                while (used[slot]) {
                    slot++;
                }
                if (skip != 0) {
                    skip--;
                    slot++;
                    if (slot >= variantCount) {
                        slot = 0;
                    }
                } else {
                    done = TRUE;
                }
            } while (!done);
            }
            spawn.variantId = variantIds[slot];
            spawn.unk0C = (config->scaleBase + (func_150ADA68() * config->scaleRange)) * variantScales[slot];
            remaining--;
            spawn.unk08 = spawn.unk0C;
            used[slot] = TRUE;
            {
            void *piece;
            if (func_150ADA68() < config->unk3C) {
                spawn.flags |= 1;
                spawn.unk63 = config->unk40;
                spawn.unk61 = 13;
            } else {
                spawn.flags &= ~1;
                spawn.unk63 = 0;
                spawn.unk61 = 8;
            }
            piece = func_1513264C(&spawn, 3, 0xFF, arg9, extraSize, argD, argE);
            if (extraData != NULL && piece != NULL) {
                memcpy((u8 *)piece + 0x170, extraData, extraSize);
            }
            }
            count--;
        }
    }
}
/* Casts a random number of rays from config->origin and spawns a decal (func_151D9B8C) on each
 * suitable surface hit closer than config->maxDistance. */
void func_15150D1C(struct Local15150D1CArg *config, u8 arg1, s32 arg2) {
    s16 *hitTriangle;
    struct Vec3F direction;
    s16 triangle[9];
    struct17 hit;
    f32 distance;
    s32 surface;
    s32 count;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    hitTriangle = triangle; // matching: IDO keeps the buffer address in a saved register only via a variable
    while (count != 0) {
        func_15143794((s16)((func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase),
                      (s16)((func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase),
                      100.0f, &direction.x);
        if (func_150AC9C0(config->origin.x, config->origin.y, config->origin.z,
                          direction.x, direction.y, direction.z, NULL, hitTriangle,
                          &hit.unk0, &hit.unk4, &hit.unk8, &distance, &surface, 0, 0.0f) != 0
            && func_15145C90(surface) != 0 && distance < config->maxDistance) {
            func_151D9B8C(config->unk2C, (func_150ADA68() * config->unk24) + config->unk20,
                          (func_150ADA20() % 156U) + 100, (s32)hitTriangle, &hit,
                          (func_150ADA20() % (u32)(config->unk2A + 1)) + config->unk28,
                          1, 1, 1, arg1, arg2);
        }
        count--;
    }
}

typedef struct {
    s32 countBase, countRange;
    struct Vec3F position;
    f32 speedBase, speedRange;
    struct AngleRange angles;
    f32 directionBase, directionRange, jitter;
    f32 sizeBase, sizeRange, scale;
    u8 kind, pad3D;
    s16 lifeBase, lifeRange;
} Game150F90Config;

void *func_1518A3C0(struct Vec3F *, struct Vec3F *, f32, struct Vec3F *, struct Vec3F *, f32, f32, u8, s16, u8, s32, u8, s32);

/* Spawns a random number of randomly rotated pieces (func_1518A3C0) flying out of config->position. */
void func_15150F90(Game150F90Config *config, u8 arg1, u8 arg2, s32 arg3) {
    struct Vec3F jitter;
    f32 width;
    s32 count;
    struct Vec3F rotation;
    struct Vec3F direction;

    width = config->jitter + config->jitter;
    jitter.y = 0.0f;
    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        rotation.x = func_150ADA68() * 360.0f;
        rotation.y = func_150ADA68() * 360.0f;
        rotation.z = func_150ADA68() * 360.0f;
        func_15143794((s16)((func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase),
                      (s16)((func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase),
                      func_150ADA68() * config->directionRange + config->directionBase,
                      &direction.x);
        jitter.x = func_150ADA68() * width - config->jitter;
        jitter.z = func_150ADA68() * width - config->jitter;
        func_1518A3C0(&config->position, &rotation,
                      func_150ADA68() * config->speedRange + config->speedBase,
                      &direction, &jitter,
                      func_150ADA68() * config->sizeRange + config->sizeBase,
                      config->scale, config->kind,
                      (func_150ADA20() % (u32)(config->lifeRange + 1)) + config->lifeBase,
                      arg1, 0, arg2, arg3);
        count--;
    }
}

typedef struct {
    s16 countBase;
    s16 countRange;
    struct Vec3F position;
    struct Vec3F normal;
    s16 triangle[9];
    u8 pad2E[2];
    f32 spread;
    u8 unk34;
    u8 unk35;
    u16 unk36;
    s16 unk38;
    s16 unk3A;
    f32 scaleBase;
    f32 scaleRange;
    s32 flags;
    u8 unk48;
    u8 unk49;
    u8 pad4A[2];
    f32 unk4C;
    f32 unk50;
    f32 speedBase;
    f32 speedRange;
    u8 unk5C;
    u8 unk5D;
    u8 pad5E[2];
    s32 unk60;
    s32 unk64;
    s16 unk68;
    s16 unk6A;
    f32 unk6C;
    f32 spin; /* extra.spin.x/z are random in [-spin, spin] */
} Local151511FCArg;

/* Spawn descriptor for func_1513D2F0. */
typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    struct Vec3F position;
    struct Vec3F rotation; /* degrees */
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 flags;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    s32 pad4C;
    s32 pad50;
    s16 unk54;
    s16 unk56;
} Local151511FCDesc;

/* Copied into the spawned object at +0x110. */
typedef struct {
    struct Vec3F velocity;
    f32 unkC;
    f32 unk10;
    struct Vec3F spin;
} Local151511FCExtra;

/* Spawns a random number of objects off a surface triangle, each moving in a random direction inside
 * the cone around the triangle's normal. */
void func_151511FC(Local151511FCArg *config, u8 arg1, s32 arg2) {
    s32 count;
    struct Vec3F axis0;
    f32 length0;
    f32 inverse0;
    struct Vec3F axis1;
    f32 length1;
    f32 inverse1;
    struct Vec3F normal;
    f32 normalLength;
    f32 normalInverse;
    Local151511FCDesc desc;
    Local151511FCExtra extra;
    f32 speed;
    f32 second;
    f32 first;
    void *object;

    desc.unk54 = config->unk68;
    desc.unk56 = config->unk6A;
    desc.unk0 = config->unk34;
    desc.unk1 = config->unk35;
    desc.unk2 = config->unk36;
    desc.unk8 = 0;
    desc.unkC = 0;
    desc.unk10 = 0xFF;
    desc.unk11 = 0xFF;
    desc.unk12 = 0xFF;
    desc.unk13 = 0xFF;
    desc.position = config->position;
    desc.unk34 = 1.0f;
    desc.unk38 = 1.0f;
    desc.unk3C = 1.0f;
    desc.flags = config->flags | 0x08000000;
    desc.unk45 = 0xFF;
    desc.unk46 = config->unk5C;
    desc.unk47 = config->unk5D;
    extra.unk10 = config->unk6C;
    if (func_15144E80(config->triangle, &axis0, &axis1, &normal) != 0) {
        func_15145128(&axis0, &axis0, &length0, &inverse0);
        func_15145128(&axis1, &axis1, &length1, &inverse1);
        func_15145128(&normal, &normal, &normalLength, &normalInverse);
        if (func_15144A74(&normal, &config->normal) < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        extra.spin.y = 0.0f;
        normal.x *= 1000.0f;
        normal.y *= 1000.0f;
        normal.z *= 1000.0f;
        count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
        while (count != 0) {
            func_15143874((s16)(func_150ADA20() & 0xFF), func_150ADA68() * config->spread, &first, &second);
            speed = (func_150ADA68() * config->speedRange) + config->speedBase;
            desc.unk4 = (func_150ADA68() * config->unk3A) + config->unk38;
            desc.unk14 = desc.unk18 = (func_150ADA68() * config->scaleRange) + config->scaleBase;
            desc.rotation.x = func_150ADA68() * 360.0f;
            desc.rotation.y = func_150ADA68() * 360.0f;
            desc.rotation.z = func_150ADA68() * 360.0f;
            desc.unk44 = (func_150ADA20() % (u32)(config->unk49 + 1)) + config->unk48;
            extra.velocity.x = ((axis0.x * first) + (axis1.x * second) + normal.x) * speed;
            extra.velocity.y = ((axis0.y * first) + (axis1.y * second) + normal.y) * speed;
            extra.velocity.z = ((axis0.z * first) + (axis1.z * second) + normal.z) * speed;
            extra.unkC = (func_150ADA68() * config->unk50) + config->unk4C;
            extra.spin.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            extra.spin.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            // matching: the comma expression makes IDO load &D_800A4AA0 inside the loop's register plan
            object = func_1513D2F0(&desc, (s32)(0, &D_800A4AA0), 25, 0, 0, 23, ((func_150ADA20() & 1) * 2) + (func_150ADA20() & 1),
                                   config->unk60, config->unk64, 32, arg1, arg2);
            if (object != NULL) {
                memcpy((u8 *)object + 0x110, &extra, sizeof(extra));
            }
            count--;
        }
    }
}

/* First two arguments of func_15147DA0. */
typedef struct {
    struct Vec3F position;
    s16 life;
    s16 enabled;
    s32 active;
    u8 pad14;
    u8 fade;
    u8 pad16[2];
    s32 unk18;
} Spawn15147DA0;

typedef struct {
    f32 size;
    struct Vec3F velocity;
    f32 growth;
    u8 pad14[4];
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    s16 unk1C;
    s16 unk1E;
} Motion15147DA0;

/* The block func_15147DA0 takes by pointer (its 12th argument). */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
} Params15147DA0;

typedef struct {
    struct Vec3F position;
    struct Vec3F normal;
    s16 triangle[9];
    u8 pad2A[2];
    f32 spread;
    f32 sizeBase, sizeRange, growthBase, growthRange, speedBase, speedRange;
    s16 countBase, countRange, fadeBase, fadeRange, lifeBase, lifeRange;
    u8 green, enabled, red, pad57;
    s32 unk58[6];
    u8 blue, alphaBase, alphaRange, pad73;
    s32 unk74, unk78;
    Params15147DA0 params;
    s16 unk9C, unk9E;
    s32 unkA0;
} Game151670Config;

/* Spawns a random number of particles off a surface triangle, each moving in a random direction
 * inside the cone around the triangle's normal. */
void func_15151670(Game151670Config *config, u8 arg1, s32 arg2) {
    Spawn15147DA0 spawn;
    Motion15147DA0 motion;
    u8 kind;
    struct Vec3F axis0;
    f32 length0, inverse0;
    struct Vec3F axis1;
    f32 length1, inverse1;
    struct Vec3F normal;
    f32 normalLength, normalInverse;
    f32 speed;
    f32 second;
    f32 first;
    s32 count;
    u32 rand;

    kind = arg1; // matching: IDO keeps arg1 in a saved register only through a copy
    if (func_15144E80(config->triangle, &axis0, &axis1, &normal) != 0) {
        func_15145128(&axis0, &axis0, &length0, &inverse0);
        func_15145128(&axis1, &axis1, &length1, &inverse1);
        func_15145128(&normal, &normal, &normalLength, &normalInverse);
        if (func_15144A74(&normal, &config->normal) < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        normal.x *= 1000.0f;
        normal.y *= 1000.0f;
        normal.z *= 1000.0f;
        spawn.active = 1;
        spawn.enabled = config->enabled;
        motion.green = config->green;
        motion.blue = config->blue;
        motion.red = config->red;
        motion.unk1C = config->unk9C;
        motion.unk1E = config->unk9E;
        spawn.position = config->position;
        spawn.unk18 = config->unkA0;
        count = func_150ADA20() % (u32)(config->countRange + 1) + config->countBase;
        while (count != 0) {
            rand = func_150ADA20();
            func_15143874((s16)(rand & 0xFF), func_150ADA68() * config->spread, &first, &second);
            speed = func_150ADA68() * config->speedRange + config->speedBase;
            motion.alpha = func_150ADA20() % (u32)(config->alphaRange + 1) + config->alphaBase;
            spawn.fade = func_150ADA20() % (u32)(config->fadeRange + 1) + config->fadeBase;
            spawn.life = func_150ADA20() % (u32)(config->lifeRange + 1) + config->lifeBase;
            motion.size = func_150ADA68() * config->sizeRange + config->sizeBase;
            motion.growth = func_150ADA68() * config->growthRange + config->growthBase;
            motion.velocity.x = (axis0.x * first + axis1.x * second + normal.x) * speed;
            motion.velocity.y = (axis0.y * first + axis1.y * second + normal.y) * speed;
            motion.velocity.z = (axis0.z * first + axis1.z * second + normal.z) * speed;
            func_15147DA0(&spawn, &motion, 0, config->unk58[0], config->unk58[1],
                          config->unk58[2], config->unk58[3], config->unk58[4],
                          config->unk58[5], config->unk74, config->unk78,
                          (s32 *)&config->params, 0, kind, arg2);
            count--;
        }
    }
}

struct Local15151A38Arg {
    struct Vec3F position;
    f32 sizeBase;
    f32 sizeRange;
    f32 growthBase;
    f32 growthRange;
    f32 speedBase;
    f32 speedRange;
    s16 countBase;
    s16 countRange;
    s16 yawBase;
    s16 yawRange;
    s16 pitchBase;
    s16 pitchRange;
    s16 fadeBase;
    s16 fadeRange;
    s16 lifeBase;
    s16 lifeRange;
    u8 green;
    u8 enabled;
    u8 red;
    u8 pad3B;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 blue;
    u8 alphaBase;
    u8 alphaRange;
    u8 pad57;
    s32 unk58;
    s32 unk5C;
    Params15147DA0 params;
    s16 unk80;
    s16 unk82;
    s32 unk84;
};

/* Spawns a random number of particles at config->position, each moving at a random yaw (around y)
 * and pitch; func_151423D8 is the cosine of a 256-step angle. */
void func_15151A38(struct Local15151A38Arg *config, u8 arg1, s32 arg2) {
    Spawn15147DA0 spawn;
    Motion15147DA0 motion;
    s32 count;
    s16 yaw;
    s16 pitch;
    f32 speed;
    f32 sinPitch;
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 horizontal;

    spawn.active = 1;
    spawn.enabled = config->enabled;
    motion.green = config->green;
    motion.blue = config->blue;
    motion.red = config->red;
    spawn.position = config->position;
    motion.unk1C = config->unk80;
    motion.unk1E = config->unk82;
    spawn.unk18 = config->unk84;
    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        yaw = (func_150ADA20() % (u32)(config->yawRange + 1)) + config->yawBase;
        pitch = (func_150ADA20() % (u32)(config->pitchRange + 1)) + config->pitchBase;
        cosPitch = func_151423D8((u8)pitch);
        sinPitch = func_151423D8((u8)(pitch - 0x40));
        cosYaw = func_151423D8((u8)yaw);
        sinYaw = func_151423D8((u8)(yaw - 0x40));
        speed = (func_150ADA68() * config->speedRange) + config->speedBase;
        motion.alpha = (func_150ADA20() % (u32)(config->alphaRange + 1)) + config->alphaBase;
        spawn.fade = (func_150ADA20() % (u32)(config->fadeRange + 1)) + config->fadeBase;
        spawn.life = (func_150ADA20() % (u32)(config->lifeRange + 1)) + config->lifeBase;
        motion.size = (func_150ADA68() * config->sizeRange) + config->sizeBase;
        motion.growth = (func_150ADA68() * config->growthRange) + config->growthBase;
        horizontal = speed * cosPitch;
        motion.velocity.x = horizontal * sinYaw;
        motion.velocity.y = -speed * sinPitch;
        motion.velocity.z = horizontal * cosYaw;
        func_15147DA0(&spawn, &motion, 0, config->unk3C, config->unk40, config->unk44, config->unk48,
                      config->unk4C, config->unk50, config->unk58, config->unk5C, (s32 *)&config->params, 0, arg1, arg2);
        count--;
    }
}

struct Arg15151D6C {
    s32 countBase;
    s32 countRange;
    struct Vec3F position;
    struct Vec3F normal;
    s16 triangle[9];
    f32 spread;
    f32 speedBase;
    f32 speedRange;
    f32 unk40;
    f32 unk44;
    s16 unk48;
    s16 unk4A;
    f32 scaleBase;
    f32 scaleRange;
    f32 spin; /* spawn.spin.x/z are random in [-spin, spin] */
};

/* Spawn descriptor for func_15132A4C: the first 0x78 bytes of Spawn1513264C. */
typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    struct Vec3F rotation; /* degrees */
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct Vec3F position;
    struct Vec3F velocity;
    struct Vec3F spin;
    f32 unk4C;
    s32 flags;
    s16 unk54;
    s16 variantId;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 pad69;
    u8 unk6A;
    u8 pad6B;
    s32 unk6C;
    u8 unk70;
    u8 pad71;
    s16 unk72;
    s16 unk74;
    s16 pad76;
} Spawn15132A4C;

/* Spawns a random number of pieces off a surface triangle, each moving in a random direction inside
 * the cone around the triangle's normal and using a random entry of the variant tables. */
void func_15151D6C(struct Arg15151D6C *config, s32 *variantIds, f32 *variantScales, u32 variantCount, u8 arg4, s32 arg5) {
    s32 count;
    s32 variant;
    Spawn15132A4C spawn;
    struct Vec3F axis0;
    f32 length0;
    f32 inverse0;
    struct Vec3F axis1;
    f32 length1;
    f32 inverse1;
    struct Vec3F normal;
    f32 normalLength;
    f32 normalInverse;
    f32 second;
    f32 first;
    f32 speed;
    f32 pad[2]; // matching: unused stack slots in the original frame

    if (func_15144E80(config->triangle, &axis0, &axis1, &normal) != 0) {
        func_15145128(&axis0, &axis0, &length0, &inverse0);
        func_15145128(&axis1, &axis1, &length1, &inverse1);
        func_15145128(&normal, &normal, &normalLength, &normalInverse);
        if (func_15144A74(&normal, &config->normal) < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        normal.x *= 1000.0f;
        normal.y *= 1000.0f;
        normal.z *= 1000.0f;
        spawn.unk00 = 1.0f;
        spawn.unk04 = 1.0f;
        spawn.unk1C = 1.0f;
        spawn.unk20 = 1.0f;
        spawn.unk24 = 1.0f;
        spawn.position = config->position;
        spawn.spin.y = 0.0f;
        spawn.flags = 0x29E8;
        count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
        spawn.unk58 = 0;
        spawn.unk5C = 0;
        spawn.unk60 = 0xFF;
        spawn.unk61 = 8;
        spawn.unk62 = 0;
        spawn.unk63 = 0;
        spawn.unk64 = 0;
        spawn.unk65 = 0;
        spawn.unk66 = 0;
        spawn.unk67 = 0;
        spawn.unk68 = 2;
        spawn.unk6A = 0;
        spawn.unk6C = 0;
        spawn.unk70 = 0;
        spawn.unk72 = 12;
        spawn.unk74 = 21;
        while (count != 0) {
            speed = (func_150ADA68() * config->speedRange) + config->speedBase;
            func_15143874((s16)(func_150ADA20() & 0xFF), func_150ADA68() * config->spread, &first, &second);
            spawn.velocity.x = ((axis0.x * first) + (axis1.x * second) + normal.x) * speed;
            spawn.velocity.y = ((axis0.y * first) + (axis1.y * second) + normal.y) * speed;
            spawn.velocity.z = ((axis0.z * first) + (axis1.z * second) + normal.z) * speed;
            spawn.rotation.x = func_150ADA68() * 360.0f;
            spawn.rotation.y = func_150ADA68() * 360.0f;
            spawn.rotation.z = func_150ADA68() * 360.0f;
        // matching: the labels end basic blocks, which keeps as1 from filling the calls' delay slots
        pickVariant:
            variant = func_150ADA20() % variantCount;
            spawn.variantId = variantIds[variant];
            spawn.unk0C = (config->scaleBase + (func_150ADA68() * config->scaleRange)) * variantScales[variant];
            spawn.unk08 = spawn.unk0C;
        randomizeSpin:
            spawn.spin.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            spawn.spin.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            spawn.unk4C = (func_150ADA68() * config->unk44) + config->unk40;
            spawn.unk54 = (func_150ADA20() % (u32)(config->unk4A + 1)) + config->unk48;
            func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
            count--;
        }
    }
}

struct Arg15152190 {
    s32 countBase;
    s32 countRange;
    struct Vec3F origin;
    struct AngleRange angles;
    f32 speedBase;
    f32 speedRange;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    f32 scaleBase;
    f32 scaleRange;
    f32 spin; /* spawn.spin.x/z are random in [-spin, spin] */
};

/* Spawns a random number of pieces in random directions around config->origin, each using a random
 * entry of the variant tables. */
void func_15152190(struct Arg15152190 *config, s32 *variantIds, f32 *variantScales, u32 variantCount, f32 radius, u8 arg5, u8 arg6, s32 arg7) {
    s32 count;
    s32 variant;
    Spawn15132A4C spawn;
    s16 yaw;
    s16 pitch;

    spawn.unk00 = 1.0f;
    spawn.unk04 = 1.0f;
    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.spin.y = 0.0f;
    spawn.flags = (arg5 ? 0x100000 : 0) | 0x29E8;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk61 = 8;
    spawn.unk62 = 0;
    spawn.unk63 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 2;
    spawn.unk6A = 0;
    spawn.unk6C = 0;
    spawn.unk70 = 0;
    spawn.unk72 = 12;
    spawn.unk74 = 21;
    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        yaw = (func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase;
        pitch = (func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase;
        spawn.rotation.x = func_150ADA68() * 360.0f;
        spawn.rotation.y = func_150ADA68() * 360.0f;
        spawn.rotation.z = func_150ADA68() * 360.0f;
        func_15143794(yaw, pitch, (func_150ADA68() * config->speedRange) + config->speedBase, &spawn.velocity.x);
        spawn.spin.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
        spawn.spin.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
        spawn.unk4C = (func_150ADA68() * config->unk28) + config->unk24;
        spawn.unk54 = (func_150ADA20() % (u32)(config->unk2E + 1)) + config->unk2C;
        func_15143794(yaw, pitch, radius, &spawn.position.x);
        spawn.position.x += config->origin.x;
        spawn.position.y += config->origin.y;
        spawn.position.z += config->origin.z;
    // matching: the label ends a basic block, which keeps as1 from filling a call's delay slot
    pickVariant:
        variant = func_150ADA20() % variantCount;
        spawn.variantId = variantIds[variant];
        spawn.unk0C = (config->scaleBase + (func_150ADA68() * config->scaleRange)) * variantScales[variant];
        spawn.unk08 = spawn.unk0C;
        {
            void *piece; // matching: storing the unused result changes as1's ordering of the argument stores
            piece = func_15132A4C(&spawn, 3, 0xFF, 0, arg6, arg7);
        }
        count--;
    }
}
typedef struct {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} Color8;

struct Local15152520Arg {
    s32 countBase;
    s32 countRange;
    struct Vec3F position;
    struct AngleRange angles;
    f32 speedBase;
    f32 speedRange;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    s16 unk30;
    s16 unk32;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    struct Vec3F unk50; /* base of motion.unk1C */
    struct Vec3F unk5C; /* range of motion.unk1C */
    Color8 unk68;
    Color8 unk6C;
    u8 unk70;
    u8 unk71;
    s16 unk72;
    s16 unk74;
};

/* Second argument of func_15157898. */
struct Local15152520Motion {
    struct Vec3F rotation; /* degrees */
    f32 unk0C;
    struct Vec3F velocity;
    struct Vec3F unk1C;
    f32 unk28;
    f32 unk2C;
    u8 unk30;
    u8 pad31[3];
};

/* First argument of func_15157898. */
struct Local15152520Params {
    u8 flags;
    u8 unk01;
    u8 unk02;
    s8 unk03;
    s8 unk04;
    u8 pad05;
    s16 unk06;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u8 unk2C;
    u8 unk2D;
    u8 pad2E[2];
    Color8 unk30;
    Color8 unk34;
    u8 unk38;
    u8 pad39[3];
    s32 unk3C;
    u8 unk40;
    u8 unk41;
    u8 pad42[2];
    struct Vec3F position;
    u8 pad50[4];
    s16 unk54;
    s16 unk56;
};

extern void func_15157898(struct Local15152520Params *, struct Local15152520Motion *, s32, f32, s32, s32, s32, u8, s32);

void func_15152520(struct Local15152520Arg *config, u8 arg1, s32 arg2) {
    s32 pad; // matching: unused stack slot
    struct Local15152520Params params;
    struct Local15152520Motion motion;
    s32 count;

    params.unk38 = 0;
    params.flags = config->unk70 | 2;
    params.unk01 = 0;
    params.unk02 = 1;
    params.unk03 = -1;
    params.unk04 = -1;
    params.unk08 = config->unk34;
    params.unk0C = config->unk38;
    params.unk24 = 0x80;
    params.unk28 = 0x20;
    params.unk10 = 0;
    params.unk14 = 0x220405;
    params.unk18 = 0x40200;
    params.unk2C = 0;
    params.unk2D = 8;
    params.unk1C = 1;
    params.unk20 = 0x38;
    params.unk3C = 0;
    params.unk40 = 0;
    params.unk41 = 2;
    params.position = config->position;
    motion.unk2C = config->unk24;
    params.unk30 = config->unk68;
    params.unk34 = config->unk6C;
    params.unk54 = config->unk72;
    params.unk56 = config->unk74;
    motion.unk30 = config->unk71;
    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        params.unk06 = (func_150ADA20() % (u32)(config->unk32 + 1)) + config->unk30;
        motion.rotation.x = func_150ADA68() * 360.0f;
        motion.rotation.y = func_150ADA68() * 360.0f;
        motion.rotation.z = func_150ADA68() * 360.0f;
        motion.unk0C = (func_150ADA68() * config->unk4C) + config->unk48;
        func_15143794((s16)((func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase),
                      (s16)((func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase),
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &motion.velocity.x);
        motion.unk1C.x = (func_150ADA68() * config->unk5C.x) + config->unk50.x;
        motion.unk1C.y = (func_150ADA68() * config->unk5C.y) + config->unk50.y;
        motion.unk1C.z = (func_150ADA68() * config->unk5C.z) + config->unk50.z;
        motion.unk28 = (func_150ADA68() * config->unk2C) + config->unk28;
        func_15157898(&params, &motion, config->unk3C, (func_150ADA68() * config->unk44) + config->unk40, 0, 0, 0, arg1, arg2);
        count--;
    }
}

struct Local15152874Arg {
    s32 countBase;
    s32 countRange;
    struct Vec3F position;
    struct AngleRange angles;
    f32 speedBase;
    f32 speedRange;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    s8 colorFunc; /* index into D_8008AC60, or -1 for opaque white */
    u8 pad31;
    s16 unk32;
    s16 unk34;
    u8 pad36[2];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
};

/* First argument of func_150CCEB0. */
struct Local15152874Spawn {
    struct Vec3F position;
    struct Vec3F velocity;
    f32 unk18;
    f32 unk1C;
    Color8 color;
    s32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    s32 pad34;
};

extern void func_150CCEB0(struct Local15152874Spawn *, u8, u8);
extern void (*D_8008AC60[])(Color8 *);

void func_15152874(struct Local15152874Arg *config, u8 arg1, s32 arg2) {
    struct Local15152874Spawn spawn;
    s32 count;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    spawn.position = config->position;
    spawn.unk18 = config->unk24;
    spawn.unk30 = config->unk48;
    while (count != 0) {
        func_15143794((s16)((func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase),
                      (s16)((func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase),
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &spawn.velocity.x);
        spawn.unk1C = (func_150ADA68() * config->unk2C) + config->unk28;
        spawn.unk24 = (func_150ADA20() % (u32)(config->unk34 + 1)) + config->unk32;
        spawn.unk28 = (func_150ADA68() * config->unk40) + config->unk38;
        spawn.unk2C = (func_150ADA68() * config->unk44) + config->unk3C;
        if (config->colorFunc != -1) {
            D_8008AC60[config->colorFunc](&spawn.color);
        } else {
            spawn.color.red = 0xFF;
            spawn.color.green = 0xFF;
            spawn.color.blue = 0xFF;
            spawn.color.alpha = 0xFF;
        }
        func_150CCEB0(&spawn, arg1, arg2);
        count--;
    }
}

/* Five RGB colors. */
extern u8 D_800A5FE0[][3];

/* A D_8008AC60 color function: a random color of D_800A5FE0 with a random alpha in [155, 255]. */
void func_15152ABC(Color8 *out) {
    u8 *pad; // matching: unused stack slot above rgb
    u8 *rgb;

    rgb = D_800A5FE0[(u8)(func_150ADA20() % 5U)];
    out->alpha = (func_150ADA20() % 101U) + 155;
    out->red = rgb[0];
    out->green = rgb[1];
    out->blue = rgb[2];
}

void func_15152B38(struct Local15152B38Arg *arg0, u8 arg1, s32 arg2) {
    struct Local15152B38Param param;
    struct Local15152B38Spawn spawn;
    s32 count;
    s32 rand0;
    s32 rand1;

    count = (func_150ADA20() % (u32)(arg0->unk4 + 1)) + arg0->unk0;
    param.unk0 = arg0->unk8;
    param.unkE = arg0->unk40;
    param.unk10 = 0xA;
    spawn.unk14 = arg0->unk42;
    spawn.unk15 = arg0->unk43;
    spawn.unk16 = arg0->unk44;
    spawn.unk20 = arg0->unk57;
    spawn.unk21 = arg0->unk58;
    spawn.unk24 = arg0->unk5C;
    spawn.unk28 = arg0->unk60;
    spawn.unk2C = arg0->unk64;
    spawn.unk2E = arg0->unk66;
    spawn.unk38 = arg0->unk70;
    spawn.unk39 = arg0->unk71;
    spawn.unk30 = arg0->unk68;
    spawn.unk32 = arg0->unk6A;
    spawn.unk34 = arg0->unk6C;

    if (count != 0) {
        do {
            param.unk15 = (func_150ADA20() % (u32)(arg0->unk38 + 1)) + arg0->unk34;
            param.unkC = (func_150ADA20() % (u32)(arg0->unk3E + 1)) + arg0->unk3C;
            spawn.unk00 = (func_150ADA68() * arg0->unk18) + arg0->unk14;
            spawn.unk10 = (func_150ADA68() * arg0->unk20) + arg0->unk1C;
            rand0 = func_150ADA20();
            rand1 = func_150ADA20();
            func_15143794((s16)((rand0 % (u32)(arg0->unk2E + 1)) + arg0->unk2C),
                          (s16)((rand1 % (u32)(arg0->unk32 + 1)) + arg0->unk30),
                          (func_150ADA68() * arg0->unk28) + arg0->unk24,
                          &spawn.unk04);
            spawn.unk17 = (func_150ADA20() % (u32)(arg0->unk49 + 1)) + arg0->unk45;
            spawn.unk18 = (func_150ADA20() % (u32)(arg0->unk4A + 1)) + arg0->unk46;
            spawn.unk19 = (func_150ADA20() % (u32)(arg0->unk4B + 1)) + arg0->unk47;
            spawn.unk1A = (func_150ADA20() % (u32)(arg0->unk4C + 1)) + arg0->unk48;
            spawn.unk1B = (func_150ADA20() % (u32)(arg0->unk51 + 1)) + arg0->unk4D;
            spawn.unk1C = (func_150ADA20() % (u32)(arg0->unk52 + 1)) + arg0->unk4E;
            spawn.unk1D = (func_150ADA20() % (u32)(arg0->unk53 + 1)) + arg0->unk4F;
            spawn.unk1E = (func_150ADA20() % (u32)(arg0->unk54 + 1)) + arg0->unk50;
            spawn.unk1F = (func_150ADA20() % (u32)(arg0->unk56 + 1)) + arg0->unk55;
            func_1515C2F0(&param, 0, &spawn, 0, arg1, arg2);
            count--;
        } while (count != 0);
    }
}

struct Local15152F70Header {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Local15152F70Arg {
    struct Local15152F70Header unk0;
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
    s16 unk34;
    s16 unk36;
    u8 unk38;
    u8 unk39;
    u8 pad3A[2];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 unk54;
    u8 unk55;
    u8 pad56[2];
};

struct Local15152F70Pos {
    struct Local15152F70Header unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 pad14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
};

struct Local15152F70Spawn {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    u8 pad14[4];
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
};

struct Local15152F70Style {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
};

void func_15152F70(struct Local15152F70Arg *arg0, u8 arg1) {
    struct Local15152F70Pos pos;
    s32 count;
    struct Local15152F70Spawn spawn;
    s16 randS1;
    struct Local15152F70Style style;
    s16 randS2;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f26;
    f32 temp_f28;

    pos.unk10 = 1;
    pos.unkE = 1;
    spawn.unk19 = arg0->unk38;
    spawn.unk1A = arg0->unk54;
    spawn.unk18 = arg0->unk39;
    spawn.unk1B = arg0->unk55;
    pos.unk0 = arg0->unk0;
    style.unk0 = 0;
    style.unk4 = 1;
    style.unk8 = 0x160600;
    style.unkC = 3;
    style.unk10 = 0x10;
    style.unk14 = 0x80;
    style.unk18 = 0x20;
    style.unk1C = 0;
    style.unk1D = 9;
    count = (func_150ADA20() % (u32)(arg0->unk26 + 1)) + arg0->unk24;

    if (count != 0) {
        do {
            randS1 = (func_150ADA20() % (u32)(arg0->unk2A + 1)) + arg0->unk28;
            randS2 = (func_150ADA20() % (u32)(arg0->unk2E + 1)) + arg0->unk2C;
            temp_f22 = func_151423D8((u8)randS2);
            temp_f24 = func_151423D8((u8)(randS2 - 0x40));
            temp_f26 = func_151423D8((u8)randS1);
            temp_f28 = func_151423D8((u8)(randS1 - 0x40));
            temp_f20 = (func_150ADA68() * arg0->unk20) + arg0->unk1C;
            pos.unk15 = (func_150ADA20() % (u32)(arg0->unk32 + 1)) + arg0->unk30;
            pos.unkC = (func_150ADA20() % (u32)(arg0->unk36 + 1)) + arg0->unk34;
            spawn.unk0 = (func_150ADA68() * arg0->unk10) + arg0->unkC;
            spawn.unk10 = (func_150ADA68() * arg0->unk18) + arg0->unk14;
            spawn.unk4 = (temp_f20 * temp_f22) * temp_f28;
            spawn.unk8 = -temp_f20 * temp_f24;
            spawn.unkC = (temp_f20 * temp_f22) * temp_f26;
            func_15147DA0(&pos, &spawn, 0, arg0->unk3C, arg0->unk40, arg0->unk44, arg0->unk48,
                          arg0->unk4C, arg0->unk50, 0, 0, (s32 *)&style, 0, arg1, 1);
            count--;
        } while (count != 0);
    }
}


typedef struct {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} Color153298;

typedef struct {
    struct Vec3F position;
    s16 countBase, countRange, yawBase, yawRange, pitchBase, pitchRange;
    f32 speedBase, speedRange;
    s16 lifeBase, lifeRange, sizeBase, sizeRange;
    s8 mode;
    u8 kind;
    s16 fadeBase, fadeRange, field2E;
    s16 field30, field32, field34, field36;
    s32 flags;
    u8 color, alpha;
} Game153298Config;

typedef struct {
    s32 flags;
    u8 pad04[8];
    s16 field0C, field0E;
    s16 x, y, z, velocityX, velocityZ;
    u8 pad1A[2];
    s8 fractionX, fractionY, fractionZ, mode;
    s16 velocityY, size, field24, field26, life;
    s8 fade;
    u8 pad2B;
    Color153298 color;
    s8 field30, field31, field32;
    u8 pad33;
    s16 kind;
} Game153298Packet;

extern void *func_15167D84(void *, s32, s32, s8, u8, s32);

void func_15153298(Game153298Config *arg0, u8 arg1, s32 arg2) {
    Game153298Packet packet;
    f32 speed;
    f32 cosinePitch;
    f32 sineYaw;
    f32 cosineYaw;
    f32 sinePitch;
    f32 horizontal;
    s16 yaw;
    s16 pitch;
    s32 count;

    packet.x = (s32)arg0->position.x;
    packet.y = (s32)arg0->position.y;
    packet.z = (s32)arg0->position.z;
    packet.fractionX = (u16)(s32)(arg0->position.x * 256.0f);
    packet.fractionY = (u16)(s32)(arg0->position.y * 256.0f);
    packet.fractionZ = (u16)(s32)(arg0->position.z * 256.0f);
    packet.color.red = arg0->color;
    packet.color.green = arg0->color;
    packet.color.blue = arg0->color;
    packet.field30 = 0;
    packet.field31 = 0;
    packet.field32 = 0;
    packet.color.alpha = arg0->alpha;
    packet.mode = arg0->mode;
    packet.kind = arg0->kind;
    packet.field0C = 0;
    packet.field0E = arg0->field2E;
    packet.flags = arg0->flags;
    count = func_150ADA20() % (u32)(arg0->countRange + 1) + arg0->countBase;
    if (count != 0) {
        do {
            yaw = func_150ADA20() % (u32)(arg0->yawRange + 1) + arg0->yawBase;
            pitch = func_150ADA20() % (u32)(arg0->pitchRange + 1) + arg0->pitchBase;
            sinePitch = func_151423D8(pitch);
            cosinePitch = func_151423D8(pitch - 64);
            sineYaw = func_151423D8(yaw);
            cosineYaw = func_151423D8(yaw - 64);
            speed = func_150ADA68() * arg0->speedRange + arg0->speedBase;
            packet.life = func_150ADA20() % (u32)(arg0->lifeRange + 1) + arg0->lifeBase;
            horizontal = speed * sinePitch;
            packet.velocityX = (s32)(horizontal * cosineYaw);
            packet.velocityY = (s32)(-speed * cosinePitch);
            packet.velocityZ = (s32)(horizontal * sineYaw);
            packet.size = func_150ADA20() % (u32)(arg0->sizeRange + 1) + arg0->sizeBase;
            packet.fade = func_150ADA20() % (u32)(arg0->fadeRange + 1) + arg0->fadeBase;
            packet.field24 = func_150ADA20() % (u32)(arg0->field32 + 1) + arg0->field30;
            packet.field26 = func_150ADA20() % (u32)(arg0->field36 + 1) + arg0->field34;
            func_15167D84(&packet, 0, 0, -1, arg1, arg2);
            count--;
        } while (count != 0);
    }
}

void func_15153634(struct Local15153634Arg *arg0, u8 arg1, u8 arg2, s32 arg3) {
    struct Local15153634Spawn sp90;
    f32 sp8C;
    s32 count;
    s16 randS1;
    s16 randS2;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f26;
    f32 temp_f28;
    f32 temp_f2;
    register void *ret;

    count = (func_150ADA20() % (u32)(arg0->unk2 + 1)) + arg0->unk0;
    sp8C = arg0->unk68;
    sp90.unk1D = arg0->unk4;
    sp90.unk08 = arg0->unk6;
    sp90.unk04 = arg0->unkC;
    sp90.unk00 = arg0->unk8;
    sp90.unk0C = 0;
    sp90.unk14 = arg0->unk1C;
    sp90.unk15 = arg0->unk1D;
    sp90.unk16 = arg0->unk1E;
    sp90.unk17 = arg0->unk1F;
    sp90.unk18 = arg0->unk20;
    sp90.unk19 = arg0->unk21;
    sp90.unk1A = arg0->unk22;
    sp90.unk30 = arg0->unk38;
    sp90.unk58 = arg0->unk5C;
    sp90.unk1C = arg0->unk25;
    sp90.unk62 = arg0->unk60;
    sp90.unk63 = arg0->unk61;
    sp90.unk1E = arg0->unk26;
    sp90.unk20 = arg0->unk28;
    sp90.unk60 = arg0->unk62;
    sp90.unk61 = arg0->unk63;
    sp90.unk22 = arg0->unk2A;
    sp90.unk24 = arg0->unk2C;
    sp90.unk66 = arg1;

    if (count != 0) {
        do {
            randS1 = (func_150ADA20() % (u32)(arg0->unk48 + 1)) + arg0->unk44;
            randS2 = (func_150ADA20() % (u32)(arg0->unk4A + 1)) + arg0->unk46;
            temp_f24 = func_151423D8((u8)randS2);
            temp_f26 = func_151423D8((u8)(randS2 - 0x40));
            temp_f28 = func_151423D8((u8)randS1);
            temp_f22 = func_151423D8((u8)(randS1 - 0x40));
            temp_f20 = (func_150ADA68() * arg0->unk50) + arg0->unk4C;
            sp90.unk0A = (func_150ADA20() % (u32)(arg0->unk12 + 1)) + arg0->unk10;
            sp90.unk10 = (func_150ADA20() % (u32)(arg0->unk18 + 1)) + arg0->unk14;
            temp_f2 = temp_f20 * temp_f24;
            sp90.unk28 = sp90.unk2C = (func_150ADA68() * arg0->unk34) + arg0->unk30;
            sp90.unk48 = temp_f2 * temp_f22;
            sp90.unk4C = -temp_f20 * temp_f26;
            sp90.unk50 = temp_f2 * temp_f28;
            sp90.unk54 = (func_150ADA68() * arg0->unk58) + arg0->unk54;
            sp90.unk1B = (func_150ADA20() % (u32)(arg0->unk24 + 1)) + arg0->unk23;
            ret = func_15130280(&sp90, arg0->unk64, 0, 4, arg2, arg3);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0xA8, &sp8C, 4);
            }
            count--;
        } while (count != 0);
    }
}

void func_151539B4(struct Local151539B4Arg *arg0, u8 arg1) {
    struct Local151539B4Spawn sp7C;
    s32 count;
    s16 randS1;
    s16 randS2;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f2;
    f32 temp_f0;

    count = (func_150ADA20() % (u32)(arg0->unk2 + 1)) + arg0->unk0;
    sp7C.unk0 = arg0->unk4;
    sp7C.unk20 = arg0->unk28;
    sp7C.unk21 = arg0->unk29;
    sp7C.unk22 = arg0->unk2A;
    sp7C.unk23 = arg0->unk2B;
    sp7C.unk24 = arg0->unk2C;
    sp7C.unk25 = arg0->unk2D;
    sp7C.unk26 = arg0->unk2E;
    sp7C.unk27 = arg0->unk2F;
    sp7C.unk28 = arg0->unk30;
    sp7C.unk2C = arg0->unk36;
    sp7C.unk36 = arg0->unk42;
    sp7C.unk38 = arg0->unk44;

    if (count != 0) {
        do {
            randS1 = (func_150ADA20() % (u32)(arg0->unk14 + 1)) + arg0->unk10;
            randS2 = (func_150ADA20() % (u32)(arg0->unk16 + 1)) + arg0->unk12;
            temp_f20 = func_151423D8((u8)randS2);
            temp_f22 = func_151423D8((u8)(randS2 - 0x40));
            temp_f24 = func_151423D8((u8)randS1);
            temp_f2 = 10.0f * temp_f20;
            temp_f0 = func_151423D8((u8)(randS1 - 0x40));
            sp7C.unkC = temp_f2 * temp_f0;
            sp7C.unk10 = -10.0f * temp_f22;
            sp7C.unk14 = temp_f2 * temp_f24;
            sp7C.unk18 = (func_150ADA68() * arg0->unk1C) + arg0->unk18;
            sp7C.unk1C = (func_150ADA68() * arg0->unk24) + arg0->unk20;
            sp7C.unk2A = (func_150ADA20() % (u32)(arg0->unk34 + 1)) + arg0->unk32;
            sp7C.unk30 = (func_150ADA68() * arg0->unk3C) + arg0->unk38;
            sp7C.unk34 = (func_150ADA20() % (u32)(arg0->unk41 + 1)) + arg0->unk40;
            func_15156190(&sp7C, arg0->unk46, 0, arg1, 0);
            count--;
        } while (count != 0);
    }
}

extern void func_15153CCC(struct ConeBasis *, struct Local15153CCCArg *, s32, u8, s32);

void func_15153C84(void *arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4) {
    struct ConeBasis sp24;

    func_1514F640(arg0, &sp24);
    func_15153CCC(&sp24, (struct Local15153CCCArg *)arg1, arg2, arg3, arg4);
}

void func_15153CCC(struct ConeBasis *arg0, struct Local15153CCCArg *arg1Struct, s32 arg2, u8 arg3, s32 arg4) {
    u8 spB4[4];
    s32 count;
    s32 rand0;
    s32 rand1;
    s32 rand2;
    f32 randf0;
    f32 randf1;
    f32 temp_f24;

    if (func_1514F6E8(arg0) != 0) {
        count = (func_150ADA20() % (u32)(arg1Struct->unk26 + 1)) + arg1Struct->unk24;
        if (count != 0) {
            temp_f24 = D_800A5FFC;
            do {
                func_1514F808(arg0,
                              (func_150ADA68() * arg1Struct->unk20) + arg1Struct->unk1C,
                              (struct Vec3F *)(spB4 - 0xC));
                randf1 = func_150ADA68();
                rand2 = func_150ADA20();
                rand0 = func_150ADA20();
                randf0 = func_150ADA68();
                rand1 = func_150ADA20();
                func_151DA6F8(arg1Struct,
                              (struct Vec3F *)(spB4 - 0xC),
                              ((randf1 = randf1) * arg1Struct->unk18) + arg1Struct->unk14,
                              (s16)((rand2 % (u32)(arg1Struct->unk2E + 1)) + arg1Struct->unk2C),
                              (rand0 % (u32)(arg1Struct->unk32 + 1)) + arg1Struct->unk30,
                              ((randf0 = randf0) * arg1Struct->unk10) + arg1Struct->unkC,
                              (rand1 % (u32)(arg1Struct->unk2A + 1)) + arg1Struct->unk28,
                              func_150ADA68() < arg1Struct->unk38,
                              temp_f24,
                              temp_f24,
                              1,
                              arg1Struct->unk34,
                              arg2,
                              arg1Struct->unk3C,
                              arg1Struct->unk3E,
                              arg1Struct->unk40,
                              arg3,
                              arg4);
                count--;
            } while (count != 0);
        }
    }
}

void func_15153F18(struct AngleRange *arg0, struct Local15153CCCArg *arg1, register s32 arg2, u8 arg3, register s32 arg4) {
    u8 spB4[4];
    s32 count;
    s32 rand0;
    s32 rand1;
    s32 rand2;
    f32 randf0;
    f32 randf1;
    f32 temp_f24;

    count = (func_150ADA20() % (u32)(arg1->unk26 + 1)) + arg1->unk24;

    if (count != 0) {
        temp_f24 = D_800A6000;
        do {
            rand1 = func_150ADA20();
            rand0 = func_150ADA20();
            func_15143794((s16)((rand1 % (u32)(arg0->yawRange + 1)) + arg0->yawBase),
                          (s16)((rand0 % (u32)(arg0->pitchRange + 1)) + arg0->pitchBase),
                          (func_150ADA68() * arg1->unk20) + arg1->unk1C,
                          (f32 *)(spB4 - 0xC));

            randf0 = func_150ADA68();
            rand2 = func_150ADA20();
            rand1 = func_150ADA20();
            randf1 = func_150ADA68();
            rand0 = func_150ADA20();

            func_151DA6F8(arg1,
                          (struct Vec3F *)(spB4 - 0xC),
                          ((randf0 = randf0) * arg1->unk18) + arg1->unk14,
                          (s16)((rand2 % (u32)(arg1->unk2E + 1)) + arg1->unk2C),
                          (rand1 % (u32)(arg1->unk32 + 1)) + arg1->unk30,
                          ((randf1 = randf1) * arg1->unk10) + arg1->unkC,
                          (rand0 % (u32)(arg1->unk2A + 1)) + arg1->unk28,
                          func_150ADA68() < arg1->unk38,
                          temp_f24,
                          temp_f24,
                          1,
                          arg1->unk34,
                          arg2,
                          arg1->unk3C,
                          arg1->unk3E,
                          arg1->unk40,
                          arg3,
                          arg4);
            count--;
        } while (count != 0);
    }
}

void *func_151541B8(struct Vec3F *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u8 arg5, s32 arg6) {
    void *ret;
    s32 flag;
    struct {
        f32 unk00;
        f32 unk04;
        f32 unk08;
        f32 unk0C;
        f32 unk10;
        f32 unk14;
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        f32 unk24;
        struct Vec3F unk28;
        f32 unk34;
        f32 unk38;
        f32 unk3C;
        f32 unk40;
        f32 unk44;
        f32 unk48;
        f32 unk4C;
        s32 unk50;
        s16 unk54;
        s16 unk56;
        u8 unk58;
        u8 pad59[3];
        s32 unk5C;
        u8 unk60;
        u8 unk61;
        u8 unk62;
        u8 unk63;
        u8 unk64;
        u8 unk65;
        u8 unk66;
        u8 unk67;
        u8 unk68;
        u8 pad69;
        u8 unk6A;
        u8 pad6B;
        s32 unk6C;
        u8 unk70;
        u8 pad71;
        s16 unk72;
        s16 unk74;
        u8 pad76[2];
    } sp40;
    struct {
        s32 pad00;
        s32 pad04;
        f32 unk08;
        f32 unk0C;
        f32 unk10;
        f32 unk14;
        f32 unk18;
        u8 unk1C;
        u8 pad1D[3];
    } sp28;

    if (arg1 <= 0.0f) {
        return 0;
    }

    sp28.unk08 = 0.0f;
    sp28.unk0C = arg1;
    sp28.unk10 = arg2;
    sp28.unk14 = arg3;
    sp28.unk18 = arg3 / (arg1 * arg1);
    if (arg4 != 0) {
        flag = 1;
    } else {
        flag = 0;
    }
    sp28.unk1C = flag;

    sp40.unk08 = 0.0f;
    sp40.unk0C = arg4;
    sp40.unk00 = 1.0f;
    sp40.unk04 = 1.0f;
    if (arg4 != 0) {
        sp40.unk10 = 0.0f;
    } else {
        sp40.unk10 = func_150ADA68() * 360.0f;
    }
    sp40.unk14 = func_150ADA68() * 360.0f;
    if (arg4 != 0) {
        sp40.unk18 = 0.0f;
    } else {
        sp40.unk18 = func_150ADA68() * 360.0f;
    }
    sp40.unk1C = 1.0f;
    sp40.unk20 = 1.0f;
    sp40.unk24 = 1.0f;
    sp40.unk28 = *arg0;
    sp40.unk34 = 0.0f;
    sp40.unk38 = 0.0f;
    sp40.unk3C = 0.0f;
    if (arg4 != 0) {
        sp40.unk40 = 0.0f;
    } else {
        sp40.unk40 = (func_150ADA68() * D_800A6004) + D_800A6008;
    }
    if (arg4 != 0) {
        sp40.unk44 = (func_150ADA68() * D_800A600C) + D_800A6010;
    } else {
        sp40.unk44 = 0.0f;
    }
    if (arg4 != 0) {
        sp40.unk48 = 0.0f;
    } else {
        sp40.unk48 = (func_150ADA68() * D_800A6014) + D_800A6018;
    }
    sp40.unk4C = 0.0f;
    sp40.unk50 = 0x100140;
    sp40.unk56 = 0x54;
    sp40.unk58 = 0;
    sp40.unk5C = 0;
    sp40.unk60 = arg3;
    sp40.unk61 = 0xF;
    sp40.unk62 = 0;
    sp40.unk63 = 0;
    sp40.unk64 = 0;
    sp40.unk65 = 0;
    sp40.unk66 = 0;
    sp40.unk67 = 0;
    sp40.unk68 = 2;
    sp40.unk6A = 0;
    sp40.unk6C = 0;
    sp40.unk70 = 0;
    sp40.unk72 = 1;
    sp40.unk74 = 0xFF;
    sp40.unk54 = 0x12C;

    ret = func_15132A4C(&sp40, 0, 0, 0x18, arg5, arg6);
    if (ret != 0) {
        memcpy((u8 *)ret + 0x170, &sp28.unk08, 0x18);
    }
    return ret;
}

f32 sqrtf(f32);

s32 func_1515452C(u8 *arg0) {
    f32 temp_fv1;
    u8 *temp_v0;

    temp_v0 = (void *)(arg0 + 0x170);
    temp_fv1 = *(f32 *)((u8 *)arg0 + 0x178) * sqrtf(*(f32 *)((u8 *)arg0 + 0x170));
    *(f32 *)((u8 *)arg0 + 0x18) = temp_fv1;
    if (!(*(u8 *)((u8 *)arg0 + 0x184) & 1)) {
        *(f32 *)((u8 *)arg0 + 0x1C) = temp_fv1;
    }
    {
        f32 temp_fv0 = *(f32 *)temp_v0;
    *(s8 *)((u8 *)arg0 + 0x70) = (s8) (u32) (*(f32 *)((u8 *)temp_v0 + 0xC) - (*(f32 *)((u8 *)temp_v0 + 0x10) * temp_fv0 * temp_fv0));
    *(f32 *)temp_v0 += D_800BE9A4;
    if (*(f32 *)((u8 *)temp_v0 + 4) < *(f32 *)temp_v0) {
        return 0;
    }
    *(f32 *)((u8 *)arg0 + 0x20) += *(f32 *)((u8 *)arg0 + 0x50) * D_800BE9A4;
    *(f32 *)((u8 *)arg0 + 0x24) += *(f32 *)((u8 *)arg0 + 0x54) * D_800BE9A4;
    *(f32 *)((u8 *)arg0 + 0x28) += *(f32 *)((u8 *)arg0 + 0x58) * D_800BE9A4;
    return 1;
    }
}

struct Local15154684Arg {
    s16 unk00;
    s16 unk02;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s16 unk0C;
    s16 unk0E;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    s32 unk38;
};

void func_15154684(struct Local15154684Arg *arg0, u8 arg1, s32 arg2) {
    f32 sp80[4];
    s32 count;
    s32 temp_s2;
    s32 temp_s1;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f0;

    count = (func_150ADA20() % (u32)(arg0->unk02 + 1)) + arg0->unk00;
    while (count > 0) {
        temp_s2 = func_150ADA20();
        temp_s1 = func_150ADA20();
        temp_f0 = func_150ADA68();
        func_15143794((s16)(temp_s2 & 0xFF),
                      (s16)((temp_s1 % (u32)(arg0->unk0E + 1)) + arg0->unk0A),
                      ((temp_f0 = temp_f0) * arg0->unk14) + arg0->unk10,
                      sp80);

        temp_f20 = func_150ADA68();
        temp_s2 = func_150ADA20();
        temp_s1 = func_150ADA20();
        temp_f22 = func_150ADA68();
        temp_f0 = func_150ADA68();

        func_151C5F44(arg0->unk04,
                      sp80,
                      ((temp_f20 = temp_f20) * arg0->unk1C) + arg0->unk18,
                      arg0->unk20,
                      (temp_s2 % (u32)(arg0->unk25 + 1)) + arg0->unk24,
                      (temp_s1 % (u32)(arg0->unk28 + 1)) + arg0->unk26,
                      ((temp_f22 = temp_f22) * arg0->unk30) + arg0->unk2C,
                      temp_f0 < arg0->unk34,
                      arg0->unk38,
                      arg1,
                      arg2);
        count--;
    }
}

void *func_15154884(struct Vec3F *arg0, f32 arg1, f32 arg2, f32 arg3, u8 arg4, s32 arg5) {
    void *ret;
    struct {
        f32 unk00;
        f32 unk04;
        f32 unk08;
        f32 unk0C;
        f32 unk10;
        f32 unk14;
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        f32 unk24;
        struct Vec3F unk28;
        f32 unk34;
        f32 unk38;
        f32 unk3C;
        f32 unk40;
        f32 unk44;
        f32 unk48;
        f32 unk4C;
        s32 unk50;
        s16 unk54;
        s16 unk56;
        u8 unk58;
        u8 pad59[3];
        s32 unk5C;
        u8 unk60;
        u8 unk61;
        u8 unk62;
        u8 unk63;
        u8 unk64;
        u8 unk65;
        u8 unk66;
        u8 unk67;
        u8 unk68;
        u8 pad69;
        u8 unk6A;
        u8 pad6B;
        s32 unk6C;
        u8 unk70;
        u8 pad71;
        s16 unk72;
        s16 unk74;
        u8 pad76[6];
    } sp40;
    struct {
        s32 pad00;
        f32 unk04;
        f32 unk08;
        f32 unk0C;
        f32 unk10;
        f32 unk14;
    } sp28;

    if (arg1 <= 0.0f) {
        return 0;
    }

    sp28.unk04 = arg1;
    sp28.unk08 = arg2;
    sp28.unk10 = 0.0f;
    sp40.unk08 = 0.0f;
    sp40.unk0C = 0.0f;
    sp40.unk00 = 1.0f;
    sp40.unk04 = 1.0f;
    sp28.unk0C = arg3;
    sp28.unk14 = D_800A601C / arg1;
    sp40.unk10 = func_150ADA68() * 360.0f;
    sp40.unk14 = func_150ADA68() * 360.0f;
    sp40.unk18 = func_150ADA68() * 360.0f;
    sp40.unk1C = 1.0f;
    sp40.unk20 = 1.0f;
    sp40.unk24 = 1.0f;
    sp40.unk28 = *arg0;
    sp40.unk34 = 0.0f;
    sp40.unk38 = 0.0f;
    sp40.unk3C = 0.0f;
    sp40.unk40 = (func_150ADA68() * D_800A6020) + D_800A6024;
    sp40.unk44 = 0.0f;
    sp40.unk48 = (func_150ADA68() * D_800A6028) + D_800A602C;
    sp40.unk4C = 0.0f;
    sp40.unk50 = 0x140;
    sp40.unk56 = 0x55;
    sp40.unk58 = 0;
    sp40.unk5C = 0;
    sp40.unk60 = 0xFF;
    sp40.unk61 = 0x12;
    sp40.unk62 = 0;
    sp40.unk63 = 0;
    sp40.unk64 = 0;
    sp40.unk65 = 0;
    sp40.unk66 = 0;
    sp40.unk67 = 0;
    sp40.unk68 = 2;
    sp40.unk6A = 0;
    sp40.unk6C = 0;
    sp40.unk70 = 0;
    sp40.unk72 = 1;
    sp40.unk74 = 0xFF;
    sp40.unk54 = 0x12C;

    ret = func_15132A4C(&sp40, 0, 0, 0x14, arg4, arg5);
    if (ret != 0) {
        memcpy((u8 *)ret + 0x170, &sp28.unk04, 0x14);
    }
    return ret;
}

s32 func_15154A88(void *arg0) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        f32 unk10;
    } Struct15154A88;
    register Struct15154A88 *temp_v1;
    f32 temp_f0;
    f32 temp_f2;

    temp_v1 = (Struct15154A88 *)((s32)arg0 + 0x170);
    temp_f0 = sinf(*(f32 *)((s32)arg0 + 0x17C));
    temp_f2 = temp_v1->unk4 * temp_f0;
    *(f32 *)((s32)arg0 + 0x1C) = temp_f2;
    *(f32 *)((s32)arg0 + 0x18) = temp_f2;
    *(u8 *)((s32)arg0 + 0x70) = (f32)(u8)(u32)temp_v1->unk8 * temp_f0;
    temp_v1->unk0 -= (&D_800BE9A4)[0];
    if (temp_v1->unk0 <= 0.0f) {
        return 0;
    }

    temp_v1->unkC += temp_v1->unk10 * (&D_800BE9A4)[0];
    *(f32 *)((s32)arg0 + 0x20) += *(f32 *)((s32)arg0 + 0x50) * (&D_800BE9A4)[0];
    *(f32 *)((s32)arg0 + 0x24) += *(f32 *)((s32)arg0 + 0x54) * (&D_800BE9A4)[0];
    *(f32 *)((s32)arg0 + 0x28) += *(f32 *)((s32)arg0 + 0x58) * (&D_800BE9A4)[0];
    return 1;
}

typedef struct {
    u8 pad0[0x22];
    s16 unk22;
    u16 unk24;
    s16 unk26;
    s16 unk28;
    u8 pad2A[0x4];
    u8 unk2E;
    u8 pad2F[0x39];
    u8 unk68;
} Struct15154C90;
void func_15154C90(Struct15154C90 *arg0);

struct Arg1515548C {
    u8 pad0[8];
    f32 unk8;
    f32 unkC;
    u8 pad10[4];
    u16 unk14;
    u8 pad16[0x42];
};

extern s32 func_151555AC();
extern u8 *func_15167A68(s32, s32, s32, s32, u8, s32);
u8 *func_1515548C(struct Arg1515548C *arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);

struct Local15155564 { u8 pad[0x2A]; u8 unk2A; };
extern void (*D_8008AD04[])(struct Local15155564 *, s32, u8);
void func_15155564(struct Local15155564 *arg0, s32 arg1, u8 arg2);

struct Node151555AC {
    u8 pad0[8];
    struct Node151555AC *next;
    u8 padC[4];
    f32 pos[2];
    f32 size[2];
    u8 pad20[0xA];
    u8 unk2A;
};
struct Ids151555AC { s32 id[2]; };
extern struct Ids151555AC D_800A6030;
extern u8 D_800DCE50[];
s32 func_151555AC(f32 *arg0, f32 *arg1, s32 *arg2, s32 arg3);
void func_1515572C(s32 arg0, u8 arg1);
