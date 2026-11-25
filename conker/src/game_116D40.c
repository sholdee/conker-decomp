#include <ultra64.h>
#include "functions.h"
#include "variables.h"


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
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    u8 pad64[0x8];
} Spawn150E9890;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 scaleXZ;
    /* 0x0C */ f32 scaleY;
    /* 0x10 */ f32 rot[3];
    /* 0x1C */ f32 unk1C[3];
    /* 0x28 */ struct17 pos;
    /* 0x34 */ f32 vel[3];
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
    /* 0x69 */ u8 unk69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ void *unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x78 */ s32 unk78;
} Particle150E9890;

extern void *func_15130374(void *, u8, s32, u8, s32);
extern s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void *func_151602C0(void *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);
void func_15143794(s16 yaw, s16 pitch, f32 length, vertex *out);

void func_150E9890(vertex *arg0, vertex *arg1, f32 arg2, u8 arg3, s32 arg4) {
    s32 count;
    Spawn150E9890 spawn;
    s32 r0;
    Particle150E9890 p;
    struct { s8 unk0; s8 unk1; s16 unk2; s8 unk4; u8 pad5[3]; } hdr;
    Header2 hdr2;
    s32 r1;

    spawn.unk1E = spawn.unk22 = spawn.unkA = (func_150ADA20() % 31U) + 20;
    spawn.unk20 = 0xFF / spawn.unkA;
    spawn.unk1B = 0xFF;
    spawn.unk28 = spawn.unk2C = ((func_150ADA68() * 608.0f) + 808.0f) * arg2;
    spawn.unk24 = ((func_150ADA68() * 18.0f) + 1016.0f) * 0.001000000047f;
    spawn.unk1D = 0x16;
    spawn.unk8 = 0x2001;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk17 = 0xFF;
    spawn.unk1C = 0xFF;
    spawn.unk58 = 0x800E05;
    spawn.unk60 = 3;
    spawn.unk61 = 3;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk3C.unk0 = 0.0f;
    spawn.unk3C.unk4 = 0.0f;
    spawn.unk3C.unk8 = 0.0f;
    if (func_150ADA20() & 1) {
        spawn.unk58 |= 0x40;
    }
    if (func_150ADA20() & 1) {
        spawn.unk58 |= 0x80;
    }
    spawn.unk18 = 0xFF;
    spawn.unk19 = 0x8E;
    spawn.unk1A = 0x13;
    spawn.unk14 = 0x2F;
    spawn.unk15 = 0;
    spawn.unk16 = 0;
    spawn.unk30 = *(struct17 *)arg0;
    spawn.unk48 = *(struct17 *)arg1;
    spawn.unk54 = 0.0f;
    func_15130374(&spawn, 0, 0, arg3, arg4);

    count = (func_150ADA20() & 7) + 5;
    p.unk50 = 0x21E8;
    p.unk56 = 0x1B;
    p.unk0 = 0.0f;
    p.rot[0] = 0.0f;
    p.rot[1] = 0.0f;
    p.rot[2] = 0.0f;
    p.unk44 = 0.0f;
    p.unk58 = 0;
    p.unk5C = 0;
    p.unk61 = 1;
    p.unk62 = 0;
    p.unk63 = 0;
    p.unk64 = 0;
    p.unk65 = 0;
    p.unk66 = 0;
    p.unk67 = 0;
    p.unk68 = 0;
    p.unk6A = 2;
    p.unk6C = NULL;
    p.unk70 = 0;
    p.unk72 = 0x14;
    p.unk74 = 0xC;
    p.unk1C[0] = 1.0f;
    p.unk1C[1] = 1.0f;
    p.unk1C[2] = 1.0f;
    p.unk4 = 0.200000003f;
    while (count > 0) {
        p.pos = *(struct17 *)arg0;
        p.unk54 = (func_150ADA20() % 41U) + 0x1D;
        p.unk60 = (func_150ADA20() % 76U) + 0xB4;
        r0 = func_150ADA20();
        r1 = func_150ADA20();
        func_15143794((s16)(r0 & 0xFF), (s16)-(r1 % 65U), ((func_150ADA68() * 2031.0f) + 1641.0f) * 0.009999999776f, (vertex *)p.vel);
        p.scaleXZ = p.scaleY = ((func_150ADA68() * 3047.0f) + 1016.0f) * 9.999999747e-05f * arg2;
        p.unk4C = ((func_150ADA68() * 99.0f) + -206.0f) * 0.009999999776f;
        p.unk40 = (func_150ADA68() * 101.0f) + -49.0f;
        p.unk48 = (func_150ADA68() * 101.0f) + -49.0f;
        func_15132A4C(&p, 3, 0xFF, 0, arg3, arg4);
        count--;
    }
    hdr.unk0 = 3;
    hdr.unk1 = -1;
    hdr.unk2 = (func_150ADA20() % 13U) + 8;
    hdr.unk4 = 0;
    hdr2.unk0 = arg0->x;
    hdr2.unk4 = arg0->y;
    hdr2.unk8 = arg0->z;
    func_151602C0(&hdr, &hdr2, (func_150ADA20() % 31U) + 0x46, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg3, arg4);
}


void func_1514470C(s32, vertex *);

/* Spawner state at +0x28 of a burst emitter. */
typedef struct {
    /* 0x00 */ s32 source;      // where func_1514470C places each burst
    /* 0x04 */ f32 rateBase;    // bursts per unit of D_800BE9A4
    /* 0x08 */ f32 rateRange;
    /* 0x0C */ f32 scaleBase;   // size scale passed to func_150E9890
    /* 0x10 */ f32 scaleRange;
    /* 0x14 */ f32 spawnAccum;
} BurstSpawner;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ BurstSpawner spawner;
} BurstEmitter;

/* Emitter update (handler table entry): accumulates a random spawn rate and, for each whole unit,
 * fires a func_150E9890 burst at the spawner's source with a random direction and scale. */
void func_150E9E34(BurstEmitter *emitter) {
    BurstSpawner *spawner;
    vertex pos;
    vertex dir;
    f32 scale;
    s32 yaw;
    s32 pitch;

    spawner = &emitter->spawner;
    spawner->spawnAccum += (spawner->rateBase + func_150ADA68() * spawner->rateRange) * D_800BE9A4;
    while (spawner->spawnAccum > 1.0f) {
        scale = func_150ADA68() * spawner->scaleRange + spawner->scaleBase;
        func_1514470C(spawner->source, &pos);
        yaw = func_150ADA20();
        pitch = func_150ADA20();
        func_15143794(yaw & 0xFF, 0x40 - (pitch % 129U), func_150ADA68() * 600.0f * 0.01f, &dir);
        func_150E9890(&pos, &dir, scale, emitter->unkC, emitter->unk1);
        spawner->spawnAccum -= 1.0f;
    }
}
