#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Spawn parameters for func_15130374 (same layout as ParticleSpawnArgs in game_10E240.c). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ vertex position;
    /* 0x3C */ vertex unk3C;
    /* 0x48 */ vertex vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 flags;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ u8 pad64[8];
} BurstParticleSpawn;

/* Piece spawn template for func_15132A4C (same layout as PieceSpawn in game_1028F0.c). */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 scaleXZ;
    /* 0x0C */ f32 scaleY;
    /* 0x10 */ f32 rot[3];
    /* 0x1C */ f32 unk1C[3];
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
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
} BurstPiece;

extern void *func_15130374(BurstParticleSpawn *, u8, s32, u8, s32);
extern s32 func_15132A4C(BurstPiece *, s32, s32, s32, u8, s32);
extern void *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);
void func_15143794(s16 yaw, s16 pitch, f32 length, vertex *out);

/* Spawns a burst at pos scaled by scale: one func_15130374 particle moving along vel,
 * 5-12 func_15132A4C pieces flung in random directions, and a func_151602C0 flash. */
void func_150E9890(vertex *pos, vertex *vel, f32 scale, u8 arg3, s32 arg4) {
    s32 count;
    BurstParticleSpawn spawn;
    s32 yaw;
    BurstPiece piece;
    Header flash;
    Header2 origin;
    s32 pitch;

    spawn.unk1E = spawn.unk22 = spawn.unkA = (func_150ADA20() % 31U) + 20;
    spawn.unk20 = 255 / spawn.unkA;
    spawn.unk1B = 0xFF;
    spawn.unk28 = spawn.unk2C = ((func_150ADA68() * 608.0f) + 808.0f) * scale;
    spawn.unk24 = ((func_150ADA68() * 18.0f) + 1016.0f) * 0.001f;
    spawn.unk1D = 22;
    spawn.unk8 = 0x2001;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk17 = 0xFF;
    spawn.unk1C = 0xFF;
    spawn.flags = 0x800E05;
    spawn.unk60 = 3;
    spawn.unk61 = 3;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk3C.x = 0.0f;
    spawn.unk3C.y = 0.0f;
    spawn.unk3C.z = 0.0f;
    if (func_150ADA20() & 1) {
        spawn.flags |= 0x40;
    }
    if (func_150ADA20() & 1) {
        spawn.flags |= 0x80;
    }
    spawn.unk18 = 0xFF;
    spawn.unk19 = 0x8E;
    spawn.unk1A = 0x13;
    spawn.unk14 = 0x2F;
    spawn.unk15 = 0;
    spawn.unk16 = 0;
    spawn.position = *pos;
    spawn.vel = *vel;
    spawn.unk54 = 0.0f;
    func_15130374(&spawn, 0, 0, arg3, arg4);

    count = (func_150ADA20() & 7) + 5;
    piece.unk50 = 0x21E8;
    piece.unk56 = 27;
    piece.unk0 = 0.0f;
    piece.rot[0] = 0.0f;
    piece.rot[1] = 0.0f;
    piece.rot[2] = 0.0f;
    piece.unk44 = 0.0f;
    piece.unk58 = 0;
    piece.unk5C = 0;
    piece.unk61 = 1;
    piece.unk62 = 0;
    piece.unk63 = 0;
    piece.unk64 = 0;
    piece.unk65 = 0;
    piece.unk66 = 0;
    piece.unk67 = 0;
    piece.unk68 = 0;
    piece.unk6A = 2;
    piece.unk6C = NULL;
    piece.unk70 = 0;
    piece.unk72 = 20;
    piece.unk74 = 12;
    piece.unk1C[0] = 1.0f;
    piece.unk1C[1] = 1.0f;
    piece.unk1C[2] = 1.0f;
    piece.unk4 = 0.2f;
    while (count > 0) {
        piece.pos = *pos;
        piece.unk54 = (func_150ADA20() % 41U) + 29;
        piece.unk60 = (func_150ADA20() % 76U) + 180;
        yaw = func_150ADA20();
        pitch = func_150ADA20();
        func_15143794(yaw & 0xFF, -(pitch % 65U), ((func_150ADA68() * 2031.0f) + 1641.0f) * 0.01f, &piece.vel);
        piece.scaleXZ = piece.scaleY = ((func_150ADA68() * 3047.0f) + 1016.0f) * 0.0001f * scale;
        // matching: `+ -206.0f` (add.s of a negative constant); `- 206.0f` emits sub.s
        piece.unk4C = ((func_150ADA68() * 99.0f) + -206.0f) * 0.01f;
        piece.unk40 = (func_150ADA68() * 101.0f) + -49.0f;
        piece.unk48 = (func_150ADA68() * 101.0f) + -49.0f;
        func_15132A4C(&piece, 3, 0xFF, 0, arg3, arg4);
        count--;
    }

    flash.unk0 = 3;
    flash.unk1 = -1;
    flash.unk2 = (func_150ADA20() % 13U) + 8;
    flash.unk4 = 0;
    origin.unk0 = pos->x;
    origin.unk4 = pos->y;
    origin.unk8 = pos->z;
    func_151602C0(&flash, &origin, (func_150ADA20() % 31U) + 70, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg3, arg4);
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
