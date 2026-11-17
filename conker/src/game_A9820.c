#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* struct197 in structs.h stops at 0x1C; func_1507BDB0 reads 0x20, 0x28, 0x3A and
 * 0x3C off the same object (struct127::unk2D0), so a file-local widened view is
 * used here rather than retyping the shared header. */
typedef struct AnimPlay {
    /* 0x00 */ void (*unk0)(void);
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u8  pad6[0x2];
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8  padC[0x4];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8  pad14[0x4];
    /* 0x18 */ f32 unk18;
    /* 0x1C */ u8  pad1C[0x4];
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8  pad24[0x4];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8  pad2C[0xE];
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ s16 unk3C;
} AnimPlay;

extern u8 D_800BEA0C;
extern u8 D_800C365E;

f32 func_1506AD30(struct127 *arg0, f32 arg1, s32 arg2);
void func_1507BDB0(AnimPlay *arg0, f32 arg1, struct127 *arg2, s32 arg3);

s32 func_150229E4(void *);
void func_1507C22C(s32 arg0);
void func_1507C324(void *arg0, void *arg1);

void func_1507C3E0(struct127 *arg0, s16 *arg1, s16 *arg2, s16 *arg3);

void func_1507C370(void) {
    s32 i;
    struct127 *obj = D_800CC2D0;

    for (i = 0; i < D_8008FD8C; i++, obj++) {
        struct126 *p = obj->unk31C;
        if (p != NULL) {
            func_1507C3E0(obj, (s16 *)((u8 *)p + 0x114), (s16 *)((u8 *)p + 0x116), (s16 *)((u8 *)p + 0x118));
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_A9820/func_1507C3E0.s")
