#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_DF260/func_150B1DB0.s")
void func_150B1E20(struct108 *arg0);

struct df260a { s32 unk0[3]; };
void func_150B1EE0(struct127 *arg0, struct df260a *arg1, s32 arg2, s32 arg3);

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
    s16 unk68;
    f32 unk6C;
} SpawnDF260;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} ExtraDF260;

typedef struct {
    struct17 pos;
    f32 timer;
} TimerDF260;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
void func_150B1F90(u8 *arg0);
void func_150B2340(struct108 *arg0);
