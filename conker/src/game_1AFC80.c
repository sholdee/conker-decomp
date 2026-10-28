#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 D_800DDE54[];
extern u8 D_8008D058[];

#pragma GLOBAL_ASM("asm/nonmatchings/game_1AFC80/func_151827D0.s")

typedef struct {
    u8 pad0[8];
    f32 unk8;
} Entry15182C5C;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 pad6[0xA];
} Vtx15182C5C;

typedef struct {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0xA];
    Vtx15182C5C *unk20[2];
    Vtx15182C5C *unk28;
    u8 pad2C[0x10];
    s32 unk3C;
} Obj15182C5C;

typedef struct {
    s16 unk0;
    u8 pad2[0x16];
} Tbl15182C5C;

extern Tbl15182C5C D_8008D060[];
extern Tbl15182C5C D_8008D062[];
extern Entry15182C5C *D_800DDE60[];
f32 func_15182F58(s32 arg0, s32 arg1);
void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

typedef struct { u8 pad0[0x16]; u16 unk16; u8 pad18[0x88]; } Big15182C5C;



void func_15182C5C(Obj15182C5C *arg0) {
    s32 idx;
    Vtx15182C5C *out;
    s32 i;
    f32 f;
    f32 w[2];
    s32 lo;
    s32 hi;
    f32 mtx[4][4];
    f32 x;
    f32 y;
    f32 z;
    f32 frac;
    s32 ofs;

    idx = arg0->unk3C;
    out = arg0->unk20[D_800BE9C0];
    func_150A8050(mtx, 0.0f, -D_8008D060[D_800DDE54[idx]].unk0, 0.0f);
    for (i = 0; i < ((Big15182C5C *)D_800DBEF4)[D_8008D062[D_800DDE54[idx]].unk0].unk16; i++) {
        f = func_15182F58(arg0->unk28[i].unk4, idx);
        lo = f;
        hi = lo + 1;
        if (hi >= 40) {
            hi = 39;
        }
        frac = f - lo;
        w[0] = D_800DDE60[idx][lo].unk8 * (1.0f - frac);
        w[1] = D_800DDE60[idx][hi].unk8 * frac;
        ofs = (s32)(w[0] + w[1]);
        ofs >>= 4;
        func_150A7960(&mtx[0][0], arg0->unk28[i].unk0, arg0->unk28[i].unk2 + ofs, arg0->unk28[i].unk4, &x, &y, &z);
        out[i].unk0 = arg0->unk10 + x;
        out[i].unk2 = arg0->unk12 + y;
        out[i].unk4 = arg0->unk14 + z;
    }
}

f32 func_15182F58(s32 arg0, s32 arg1) {
    f32 var_f0;
    f32 var_f2;

    var_f0 = 0.0f;
    var_f2 = *(f32 *) &D_8008D058[D_800DDE54[arg1] * 0x18] * (f32) (arg0 * 0x28);
    if (var_f2 < var_f0) {
        var_f2 = var_f0;
    } else {
        var_f0 = 39.0f;
        if (var_f0 < var_f2) {
            var_f2 = var_f0;
        }
    }
    return var_f2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1AFC80/func_15182FDC.s")
