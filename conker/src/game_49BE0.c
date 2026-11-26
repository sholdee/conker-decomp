#include <ultra64.h>
#include <PR/sched.h>
#include "structs.h"

/*
 * This file's .bss, linked at 0x800BE9A0..0x800BEAD0 in this order. Defined before functions.h and
 * variables.h declare the globals (conker/game.h): IDO lays out .bss in the order it first sees each
 * name, and an earlier extern would count. The order also depends on how many types and variables
 * are declared before it (see init_50A0.c).
 */
u8 D_800BE9A0;               // frame steps of this frame (half the ticks, see func_15019F20)
f32 D_800BE9A4;              // frame delta: half the ticks of this frame
f32 D_800BE9A8;              // 1 / D_800BE9A4 (0 when that is 0)
s32 D_800BE9AC;              // ticks since the start
s32 D_800BE9B0;              // D_800BE9AC % 30
u8 D_800BE9B4;
f32 D_800BE9B8;              // the average of the last 10 frame deltas
f32 D_800BE9BC;              // 1 / D_800BE9B8
u8 D_800BE9C0;               // which of the two matrix buffers is being built this frame
u16 *D_800BE9C4;             // z-buffer (D_800BE620 x D_800BE624)
Gfx *D_800BE9C8[2];          // display-list buffer per framebuffer (D_800BE9C0)
Gfx *D_800BE9D0;             // display-list write head
Gfx *D_800BE9D8[2];          // per framebuffer: end of the prologue built by func_1501A39C
OSMesg D_800BE9E0;
s32 D_800BE9E4;              // frames elapsed this update
s32 D_800BE9E8;              // number of players minus one
u8 D_800BE9EC;
u8 D_800BE9ED;
s32 level;                   // the current map
s32 D_800BE9F4;              // current scene / game mode id
s32 D_800BE9F8;              // the previous map
s32 D_800BE9FC;              /* not referenced */
s32 D_800BEA00;
s32 D_800BEA04;
s32 D_800BEA08;              // the ticks of this frame, before the D_800BEA0C divider
u8 D_800BEA0C;               // frame rate divider (0: none; 1: time stands still)
OSMesgQueue D_800BEA10;      // graphics task done messages (created by func_15007830)
OSMesg D_800BEA28[16];       // D_800BEA10's buffer
OSScMsg D_800BEA68[2];       // "task done" messages, one per frame buffer
s16 D_800BEAA8;              // graphics tasks sent
u8 D_800BEAAA;
u8 D_800BEAAB;
u8 D_800BEAB0[16];           /* not referenced */
u8 D_800BEAC0;
u8 D_800BEAC1;
u8 D_800BEAC2;
u8 D_800BEAC3;

#include "functions.h"
#include "variables.h"


void func_1501C730(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_1501C53C();
    D_800D2132 = 0;
    switch (arg0) {
    case 0:
        break;
    case 4:
    case 6:
        D_800BEA00 = arg4;
        D_800BEA04 = arg3;
        D_800BE9F4 = arg1;
        D_800DD2C0[0] = arg2;
        D_800BE615 = 2;
        break;
    case 1:
        D_800BE615 = 3;
        D_800BE9F4 = arg1;
        D_800D2132 = D_800CC382;
        break;
    case 2:
        D_800BE615 = 3;
        D_800BE9F4 = arg1;
        D_800DD2C0[0] = arg2;
        D_800D2132 = D_800CC382;
        break;
    case 3:
        if (D_800BE616 == 0) {
            D_800BE615 = 4;
            func_15016680();
        }
        break;
    case 5:
        D_800BE9F4 = 5;
        D_800BE615 = 1;
        break;
    }
}
