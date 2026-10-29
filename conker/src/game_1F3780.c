#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_1F3780/func_151C62D0.s")

void func_151C69CC(void *a0, s32 a1, u8 a2);

void func_151C6974(void *a0, s32 a1, s32 a2) {
    func_151C69CC(a0, a2, 0);
}

void func_151C69A0(void *a0, s32 a1, s32 a2) {
    func_151C69CC(a0, a2, 1);
}

typedef struct 
{
  s32 unk0;
  s32 unk4;
  s32 unk8;
} D_800AAB08Entry;
extern s32 D_800AAB00[];
extern D_800AAB08Entry D_800AAB08[];
void func_151C69CC(void *a0, s32 a1, u8 a2);
void func_151C69CC(void *a0, s32 a1, u8 a2)
{
  D_800AAB08Entry *temp_a0;
  s32 temp_a2;
  temp_a2 = ((s32 *) a0)[0x1D4 / 4] + D_800AAB00[a2];
  temp_a0 = &D_800AAB08[a2];
  func_15143134(temp_a0, a1, temp_a2);
}


typedef struct {
    u8 pad0[8];
    struct17 pos;
    u8 pad14[0x2C - 0x14];
    struct17 unk2C;
    u8 pad38[0x59 - 0x38];
    u8 unk59;
    u8 pad5A[0x64 - 0x5A];
} Func151C6A28Big;

extern D_800AAB08Entry D_800AAB38[];
extern D_800AAB08Entry D_800AAB50[];
s32 func_15142A5C(struct127 *arg0);
void func_151D5148(struct127 *arg0);
extern s32 func_15145EA4(void *arg0, void *arg1, s32 arg2, s32 arg3);
void func_151D3F14(struct17 *arg0, u8 arg1, s32 arg2);
void func_151D4408(struct17 *, struct17 *, s32, struct127 *, f32, u8, s32);
void func_151C6D70(struct127 *, u8, struct17 *, s32, s32, s32);
s32 func_1514ECE0(s32, s16, void *);
void func_150F7470(struct17 *, s32, s32, s32, s32, Func151C6A28Big *, f32, f32, f32, struct127 *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);

void func_151C6A28(struct127 *arg0, u8 arg1, u8 arg2, s32 arg3) {
    struct17 sp124;
    struct17 sp118;
    struct17 sp10C;
    s32 sp108;

    if ((arg0 != NULL) && (arg1 < 2) && (func_15142A5C(arg0) == 0)) {
        func_151D5148(arg0);
        if (*(s32 *)&arg0->unk1D4 != 0) {
            {
                D_800AAB08Entry *spFC[3];
                struct17 *spF0[3];

                sp108 = *(s32 *)&arg0->unk1D4 + D_800AAB00[arg1];
                spFC[0] = &D_800AAB08[arg1];
                spFC[1] = &D_800AAB38[arg1];
                spFC[2] = &D_800AAB50[arg1];
                spF0[0] = &sp124;
                spF0[1] = &sp118;
                spF0[2] = &sp10C;
                func_15145EA4(spFC, spF0, sp108, 3);
                if (0) { sp10C.unk0 = 0.0f; }
                if (0) { sp10C.unk0 = 1.0f; }
                if (0) { sp10C.unk0 = 2.0f; }
                if (0) { sp10C.unk0 = 3.0f; }
                if (0) { sp10C.unk0 = 4.0f; }
                if (0) { sp10C.unk0 = 5.0f; }
                if (0) { sp10C.unk0 = 6.0f; }
                if (0) { sp10C.unk0 = 7.0f; }
                if (0) { sp10C.unk0 = 8.0f; }
                if (0) { sp10C.unk0 = 9.0f; }
                if (0) { sp10C.unk0 = 10.0f; }
                if (0) { sp10C.unk0 = 11.0f; }
                sp10C.unk0 -= sp118.unk0;
                sp10C.unk4 -= sp118.unk4;
                sp10C.unk8 -= sp118.unk8;
            }
            func_151D3F14(&sp124, arg2, arg3);
            func_151D4408(&sp118, &sp10C, sp108, arg0, 1.0f, arg2, arg3);
            func_151C6D70(arg0, arg1, &sp124, -1, arg2, arg3);
            {
                u8 *temp;
                struct17 spE0;
                void *spDC;
                u8 *p28;
                Func151C6A28Big *src;

                if (func_1514ECE0(*(s32 *)((u8 *)arg0 + 0x2F4), 0x1B, &spDC) != 0) {
                    temp = *(u8 **)((u8 *)spDC + 0x10);
                    p28 = temp + 0x28;
                    if (temp[0x9C] & 1) {
                        src = (Func151C6A28Big *)(p28 + 0x10);
                        if (p28[0x69] != 0) {
                            if (src->unk59 >= 2) {
                                f32 dx;
                                f32 dy;
                                f32 dz;

                                dx = src->pos.unk0 - sp124.unk0;
                                dy = src->pos.unk4 - sp124.unk4;
                                dz = src->pos.unk8 - sp124.unk8;
                                spE0.unk0 = (dx * 1.100000024f) + sp124.unk0;
                                spE0.unk4 = (dy * 1.100000024f) + sp124.unk4;
                                spE0.unk8 = (dz * 1.100000024f) + sp124.unk8;
                            } else {
                                spE0 = src->pos;
                            }
                            {
                                Func151C6A28Big sp64;

                                sp64 = *src;
                                sp64.unk2C = spE0;
                                func_150F7470(&sp124, 0, 0, 0, 0, &sp64, 54.00000381f, 0.01851851679f, 500.0f, arg0, 0,
                                              (src->unk59 >= 2) ? 0 : 1, 1, 0, 0x1A, -1, 0, 1, arg2, arg3);
                            }
                        }
                    }
                }
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1F3780/func_151C6D70.s")

struct blk12_C6EA0 { s32 a; s32 b; s32 c; };

typedef struct {
    u8 pad_0[0x3B];
    u8 field_0x3B;
} ActorIdByteFields;

typedef struct {
    u8 pad_0[0x170];
    u8 field_0x170;
} CreatedObjectFlagFields;

s32 func_1513264C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);

void func_151C6EA0(ActorIdByteFields *arg0, u8 arg1, s32 arg2) {
    struct {
        f32 unk00;
        f32 unk04;
        f32 unk08;
        f32 unk0C;
        struct blk12_C6EA0 unk10;
        f32 unk1C;
        f32 unk20;
        f32 unk24;
        struct blk12_C6EA0 unk28;
        struct blk12_C6EA0 unk34;
        struct blk12_C6EA0 unk40;
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
    } sp34;
    struct { u8 b; u8 pad[3]; } sp30;
    struct blk12_C6EA0 *src = (struct blk12_C6EA0 *)&D_800A5480;
    CreatedObjectFlagFields *temp_v0;

    sp30.b = 0;
    sp34.unk00 = 1.0f;
    sp34.unk04 = 1.0f;
    sp34.unk0C = 1.0f;
    sp34.unk08 = 1.0f;
    sp34.unk10 = *src;
    sp34.unk1C = 1.0f;
    sp34.unk20 = 1.0f;
    sp34.unk24 = 1.0f;
    sp34.unk28 = *src;
    sp34.unk34 = *src;
    sp34.unk40 = *src;
    sp34.unk50 = 0x11900;
    sp34.unk54 = 0x12C;
    sp34.unk56 = 0xF;
    sp34.unk58 = 0;
    sp34.unk5C = 0;
    sp34.unk60 = 0x64;
    sp34.unk61 = 0x17;
    sp34.unk62 = 0;
    sp34.unk63 = 0;
    sp34.unk64 = 0;
    sp34.unk65 = 0;
    sp34.unk66 = 0;
    sp34.unk67 = 0;
    sp34.unk68 = 0x5;
    sp34.unk69 = -1;
    sp34.unk6A = 1;
    sp34.unk4C = 0.0f;
    sp34.unk6C = (s32)arg0;
    sp34.unk70 = arg0->field_0x3B;
    sp34.unk72 = 1;
    sp34.unk74 = 0xFF;
    sp34.unk78 = 0;

    temp_v0 = (CreatedObjectFlagFields *)func_1513264C((s32)&sp34, 3, 0xFF, 0, 1, arg1, arg2);
    if (temp_v0 != 0) {
        memcpy(&temp_v0->field_0x170, &sp30.b, 1);
    }
}

typedef struct {
    u8 pad0[0x60];
    s32 unk60;
    u8 pad64[0x70 - 0x64];
    u8 unk70;
    u8 pad71[0x7C - 0x71];
    struct127 *unk7C;
} Func151C7038Arg;

s32 func_151C7038(Func151C7038Arg *arg0) {
    struct127 *obj;
    f32 speed;

    obj = arg0->unk7C;
    if (*(u16 *)((u8 *)obj + 0x84) != 0xE9) {
        return 0;
    }
    if (func_15142A5C(obj) != 0) {
        arg0->unk60 |= 0x20000;
    } else {
        arg0->unk60 &= ~0x20000;
    }
    speed = obj->animation_speed * 35.0f;
    if (speed > 255.0f) {
        speed = 255;
    } else if (speed < 0.0f) {
        speed = 0.0f;
    }
    {
        u8 val;

        val = speed;
        arg0->unk70 = val;
    }
    return 1;
}
