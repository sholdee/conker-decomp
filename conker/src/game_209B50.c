#include <ultra64.h>
/* matching: this file calls func_150ADA20 as returning u32 (functions.h says u8); a u8 return
 * spills the random values as bytes. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
/* functions.h declares func_15149130 with a struct37 * where this file passes the data size. */
#define func_15149130 func_15149130_struct37_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
#undef func_15149130
u32 func_150ADA20(void);
#include "variables.h"

/* Direction spread of a burst: func_15143794 takes a random angle from each range (see game_2062D0.c). */
typedef struct {
    s16 angle1;
    s16 angle1Range;
    s16 angle2;
    s16 angle2Range;
} BurstAngles;

/* Parameters for func_15150178, which throws count + rand(countRange) func_151D9014 particles
 * (same layout as DropBurstSpec in game_2062D0.c). */
typedef struct {
    /* 0x00 */ BurstAngles angles;
    /* 0x08 */ struct17 pos;
    /* 0x14 */ s16 count;
    /* 0x16 */ s16 countRange;
    /* 0x18 */ f32 speed;
    /* 0x1C */ f32 speedRange;
    /* 0x20 */ s16 lifetime;
    /* 0x22 */ s16 lifetimeRange;
    /* 0x24 */ f32 gravity;
    /* 0x28 */ f32 gravityRange;
    /* 0x2C */ u8 unk2C; /* particle arg5 */
    /* 0x2D */ u8 unk2CRange;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ f32 size;
    /* 0x34 */ f32 sizeRange;
    /* 0x38 */ s8 unk38;
    /* 0x39 */ u8 kind;
    /* 0x3A */ u8 pad3A[2];
    /* 0x3C */ f32 unk3CChance; /* probability of particle arg7 */
    /* 0x40 */ s8 wobble;
    /* 0x41 */ u8 pad41[3];
    /* 0x44 */ f32 bounceChance;
} DropBurstSpec; /* 0x48 */

void func_15103254(s16 lifetime, u8 alpha, f32 size, struct17 *pos, u8 views, u8 arg5, s32 arg6);
void func_15150178(DropBurstSpec *spec, struct17 *pos, s32 arg2, u8 owner, s32 kind);
void func_151D3F14(struct17 *pos, u8 owner, s32 kind);
void func_15182670(u8 r, u8 g, u8 b, u8 alpha, s32 duration, s32 view, u8 owner, s32 kind);

/* Spawns a flash particle, a burst of 12-18 drops, the func_151D3F14 effect and a white screen
 * flash at `pos`. */
void func_151DC6A0(struct17 *pos, u8 owner, s32 kind) {
    func_15103254(func_150ADA20() % 5U + 12, func_150ADA20() % 56U + 200, func_150ADA68() * 600.0f + 1700.0f,
                  pos, 0xFF, owner, kind);
    {
        DropBurstSpec drops;

        drops.pos = *pos;
        drops.count = 12;
        drops.countRange = 7;
        drops.angles.angle1 = 0;
        drops.angles.angle1Range = 0xFF;
        drops.angles.angle2 = -64;
        drops.angles.angle2Range = 60;
        drops.lifetime = 36;
        drops.lifetimeRange = 60;
        drops.unk2C = 200;
        drops.unk2CRange = 55;
        drops.unk38 = 0;
        drops.kind = 12;
        drops.wobble = 1;
        drops.speed = 8.0f;
        drops.speedRange = 10.0f;
        drops.gravity = -1.097f;
        drops.gravityRange = 0.37f;
        drops.size = 280.0f;
        drops.sizeRange = 390.0f;
        drops.unk3CChance = 0.6f;
        drops.bounceChance = 1.0f;
        func_15150178(&drops, &drops.pos, 0, owner, kind);
    }
    func_151D3F14(pos, owner, kind);
    func_15182670(0xFF, 0xFF, 0xFF, func_150ADA20() % 51U + 150, func_150ADA20() % 5U + 8, 0, owner, kind);
}


/* Data of the effect spawned by func_151DC8BC and updated by func_151DC97C. */
typedef struct {
    /* 0x00 */ struct17 pos;
    /* 0x0C */ f32 accum;        /* particles still to emit; func_151DC97C emits one per whole unit */
    /* 0x10 */ u8 randomColors;  /* nonzero: each particle takes one of three random colour pairs */
    /* 0x11 */ u8 pad11[3];
} EmitterData; /* 0x14 */

/* Effect created by func_15149130 (only the fields used here). */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;          /* passed to func_15130280 as its 6th argument */
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;          /* passed to func_15130280 as its 5th argument */
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ EmitterData data;
} Emitter;

void *func_15149130(s16 duration, s8 arg1, s8 arg2, s8 arg3, u8 arg4, u8 arg5, s32 dataSize, u8 owner, s32 kind);

/* Spawns an emitter effect at `pos` that lasts `duration` frames and is updated by func_151DC97C
 * (D_8008A4E8[0x5B]), preceded by a func_151DC6A0 burst when `burst` is set. */
void func_151DC8BC(struct17 *pos, s32 duration, u8 burst, u8 randomColors, u8 owner, s32 kind) {
    EmitterData data;
    Emitter *emitter;

    if (burst) {
        func_151DC6A0(pos, owner, kind);
    }

    data.pos = *pos;
    data.accum = 0.0f;
    data.randomColors = randomColors;

    emitter = func_15149130(duration, -1, 0x5B, -1, 1, 0, sizeof(data), owner, kind);
    if (emitter != NULL) {
        memcpy(&emitter->data, &data, sizeof(data));
    }
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
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67[9];
} Spawn151DC97C;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} Extra151DC97C;

typedef struct {
    struct17 pos;
    f32 accum;
    u8 unk10;
} Sub151DC97C;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Sub151DC97C unk28;
} Obj151DC97C;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_151DC97C(Obj151DC97C *arg0) {
    Sub151DC97C *sub;

    sub = &arg0->unk28;
    sub->accum += (0.1360000074f + func_150ADA68() * 0.1210000068f) * D_800BE9A4;
    if (sub->accum > 1.0f) {
        Spawn151DC97C spawn;
        Extra151DC97C extra;
        void *ret;
        u8 i;

        extra.unkC = 0.9704700112f;
        spawn.unk1D = 0x6C;
        spawn.unk8 = 0x5103;
        spawn.unk0 = 0x200005;
        extra.unk0 = 0;
        extra.unk1 = 0;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk1E = 0x28;
        spawn.unk20 = 6;
        spawn.unk58 = 0x80DE07;
        spawn.unk60 = 8;
        spawn.unk61 = 6;
        spawn.unk62 = 0x16;
        spawn.unk63 = -1;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk22 = 0x78;
        spawn.unk1C = 0xFF;
        spawn.unk24 = 1.006579041f;
        spawn.unk30 = sub->pos;
        spawn.unk3C.unk0 = 0.0f;
        spawn.unk3C.unk4 = 0.0f;
        spawn.unk3C.unk8 = 0.0f;
        spawn.unk48.unk0 = 0.0f;
        spawn.unk48.unk4 = 0.0f;
        spawn.unk48.unk8 = 0.0f;
        if (sub->unk10 == 0) {
            spawn.unk14 = 0xEF;
            spawn.unk15 = 0xE0;
            spawn.unk16 = 0xCD;
            spawn.unk17 = 0xFF;
            spawn.unk18 = 0x30;
            spawn.unk19 = 0x30;
            spawn.unk1A = 0x30;
        }
        do {
            extra.unk2 = func_150ADA20() % 5U + 4;
            extra.unk3 = func_150ADA20() % 5U + 4;
            extra.unk4 = func_150ADA68() * 50.0f;
            extra.unk8 = func_150ADA68() * 50.0f;
            spawn.unk54 = func_150ADA68() * 0.151000008f + 0.1910000145f;
            spawn.unk58 &= ~0xC0;
            spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
            spawn.unk1B = func_150ADA20() % 56U + 200;
            spawn.unkA = func_150ADA20() % 36U + 110;
            spawn.unk28 = spawn.unk2C = func_150ADA68() * 370.0f + 403.0f;
            if (sub->unk10 != 0) {
                const u8 colA[3][3] = { { 0xEF, 0xE0, 0xCD }, { 0x9E, 0x9C, 0xA0 }, { 0x9E, 0x9C, 0xA0 } };
                const u8 colB[3][3] = { { 0x30, 0x30, 0x30 }, { 0x9A, 0x69, 0xD2 }, { 0xD5, 0x83, 0x91 } };
                i = func_150ADA20() % 3U;
                spawn.unk14 = colA[i][0];
                spawn.unk15 = colA[i][1];
                spawn.unk16 = colA[i][2];
                spawn.unk17 = 0xFF;
                spawn.unk18 = colB[i][0];
                spawn.unk19 = colB[i][1];
                spawn.unk1A = colB[i][2];
            }
            ret = func_15130280(&spawn, 0, 0, 0x10, arg0->unkC, arg0->unk1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0xA8, &extra, sizeof(extra));
            }
            sub->accum -= 1.0f;
        } while (sub->accum > 1.0f);
    }
}
