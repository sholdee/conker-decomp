#include <ultra64.h>

// This file's .bss, defined before functions.h and variables.h declare the globals: IDO lays out .bss in
// the order it first sees each name, and that order also depends on how many types and variables are
// declared before it (see init_50A0.c), which the subsystem headers change often.
f32 D_800BE5E8[10]; // the last 10 frame deltas
u32 D_800BE610;     // the leftover tick remainder
u8 D_800BE614;
u8 D_800BE615;
u8 D_800BE616;
u8 D_800BE617;

#include "functions.h"
#include "variables.h"



/* Game main loop: runs frames until func_1501878C says to stop. */
void func_150186D0(void) {
    func_10001444();
    func_1501A39C();
    func_151E6BFC();

    D_800BE5E0 = 1;
    D_800D18A0 = 0;
    D_800D18A2 = 0;
    *(s32 *)D_800D18A4 = 0; // clears all four players with one word store
    D_800D23A9 = 0;

    while (func_1501878C() != 0) {
    }

    D_8002AC60 = 1;
    D_8002AC64 = 3500;
    D_800BE728 = func_1501BBB8();

    while (func_151DCFD8(1) != 0) {
    }
}

/*
 * Append one GBI command (w0, w1) at the frame's display-list write head D_800BE9D0.
 * matching: GBI macros on a post-incremented head, or taking &D_800BE9D0, move the pointer to v0.
 */
#define DL_APPEND(hi, lo)                       \
    {                                           \
        Gfx *cmd = D_800BE9D0++;                \
        cmd->words.w0 = (hi);                   \
        cmd->words.w1 = (u32)(lo);              \
    }

/* gSPLoadUcode(textStart, dataStart) at the display-list head, written out like DL_APPEND. */
#define DL_LOAD_UCODE(textStart, dataStart)     \
    {                                           \
        Gfx *cmd = D_800BE9D0++;                \
        cmd->words.w0 = _SHIFTL(G_RDPHALF_1, 24, 8); \
        cmd->words.w1 = (u32)(dataStart);       \
        cmd = D_800BE9D0++;                     \
        cmd->words.w0 = _SHIFTL(G_LOAD_UCODE, 24, 8) | (SP_UCODE_DATA_SIZE - 1); \
        cmd->words.w1 = (u32)(textStart);       \
    }

/*
 * Waits for the next message on D_800BEA10 and handles it; type 1 runs and draws one game frame.
 * Returns 0 when the main loop should end.
 */
s32 func_1501878C(void) {
    s16 *message;
    s16 i;

    message = NULL;
    osRecvMesg(&D_800BEA10, (OSMesg *)&message, OS_MESG_BLOCK);
    D_8002AC60 = 0;
    D_8002AC64 = 800;
    switch (*message) {
    case 2:
        func_1517E1AC();
        if (D_800DC9F0 != 0) {
            func_151412BC();
        }
        D_800BEAA8--;
        if (D_800BE615 != 0 && D_800BEAA8 == 0) {
            func_10004074(D_800BE9C8[0]);
            func_10004074(D_800BE9C8[1]);
            D_800BE9C8[0] = NULL;
            return 0;
        }
        break;
    case 1:
        if (D_800BE615 == 0) {
            func_151E51EC();
            if (D_800BE615 == 0) {
                if (D_800BE614 != 0) {
                    func_1501A39C();
                    D_800BE614 = 0;
                }
                if (D_800D9AF0 != 0) {
                    func_1510B3B0();
                }
                if (D_800D9B18 != 0) {
                    func_1510B51C();
                }
                func_1501905C();
                if (D_800BEAC0 != 0 || func_15019130() != 0) {
                    D_800BE9D0 = func_1501A680(D_800BE9D0);
                    func_1515D69C();
                    func_15167310();
                    for (i = 0; i <= D_80082FA0; i++) {
                        D_80082FA4 = D_800BE630[i];
                        func_15018F80(D_80082FA4);
                        D_800BE9D0 = func_15019464(D_800BE9D0, D_80082FA4);
                    }
                    if (D_800BEBA0 == 0) {
                        DL_LOAD_UCODE(D_800C1060, D_800BEB9C);
                        DL_APPEND(0xD9C2F9FA, 0);        // gSPClearGeometryMode(0x3D0605)
                        DL_APPEND(0xD9FFFFFF, 0x220005); // gSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_LIGHTING | G_SHADING_SMOOTH)
                        DL_APPEND(0xDA380003, &D_80089470); // gSPMatrix(G_MTX_LOAD)
                        DL_APPEND(0xDC08000A, &D_800D9B90[D_800BE9C0].l[0]); // gSPLookAtX
                        DL_APPEND(0xDC08060A, &D_800D9B90[D_800BE9C0].l[1]); // gSPLight(1)
                    }
                    {
                        Gfx *savedHead;

                        for (i = 0; i <= D_80082FA0; i++) {
                            savedHead = D_800BE9D0;
                            D_80082FA4 = D_800BE630[i];
                            D_800BE9D0 = func_1515D6D0(D_800BE9D0, D_80082FA4);
                            D_800BE9D0 = func_150195A0(D_800BE9D0, D_80082FA4);
                            // Out of display-list space: drop this player's commands.
                            // matching: the ?: materialises the flag (move v1,zero / li v1,1)
                            if ((D_800BE9D0 - D_800BE9C8[D_800BE9C0] > D_800BEBA4) ? 1 : 0) {
                                D_800BE9D0 = savedHead;
                            }
                        }
                        D_80082FA4 = 0;
                        func_15019414();
                        D_800BE9D0 = func_150198FC(D_800BE9D0);
                        for (i = 0; i <= D_80082FA0; i++) {
                            D_80082FA4 = D_800BE630[i];
                            D_800BE9D0 = func_1515D6D0(D_800BE9D0, D_80082FA4);
                            D_800BE9D0 = func_15019CC8(D_800BE9D0, D_80082FA4);
                            if (D_800BEAC0 == 0 && D_80082FA0 > 0) {
                                D_800BE9D0 = func_15019BB8(D_800BE9D0, D_80082FA4);
                            }
                        }
                        D_800BE9D0 = func_15019E60(D_800BE9D0);
                        if (D_800BEAC0 == 0 && D_80082FA0 == 0) {
                            for (i = 0; i <= D_80082FA0; i++) {
                                D_80082FA4 = D_800BE630[i];
                                D_800BE9D0 = func_15019BB8(D_800BE9D0, D_80082FA4);
                            }
                        }
                        D_800BE9D0 = func_151E84B0(D_800BE9D0);
                        D_800BE9D0 = func_15043384(D_800BE9D0);
                    }
                    func_150347E8();
                    func_1502E474();
                    func_15189118(0);
                    func_15084A18();
                    func_15125628();
                    DL_APPEND(_SHIFTL(G_RDPFULLSYNC, 24, 8), 0); // gDPFullSync
                    DL_APPEND(_SHIFTL(G_ENDDL, 24, 8), 0); // gSPEndDisplayList
                    func_10004674();
                    func_1510D404();
                    func_15114A1C();
                    func_1517CFC4();
                    func_1501C880(0, 1);
                    D_800BE9B4 = 0;
                    if (D_800BEAC0 != 0) {
                        D_800BEAC2 = 2;
                    } else if (D_800BEAC2 > 0) {
                        D_800BEAC2--;
                    }
                    if (D_800BEAC0 != D_800BEAC1) {
                        D_800BEAC0 = D_800BEAC1;
                        if (D_800BEAC0 != 0) {
                            func_1000E2F4(1);
                            func_10011E94(1);
                            if (func_151F2CDC() != 0) {
                                func_151F2BE8();
                            }
                        } else {
                            func_1000E2F4(0);
                            func_10011E94(0);
                            if (func_151F2CDC() != 0) {
                                func_151F2C4C();
                            }
                        }
                    }
                }
            }
        }
        break;
    case 4:
        while (func_151DCFD8(1) != 0) {
        }
        for (;;) {
        }
    }
    return 1;
}

/* Per-frame updates called from func_15019130, most of them only outside D_800BEAC0 mode. */
void func_15018DFC(void) {
    func_150A09D0(D_800BE9E4);
    func_150619A8();
    func_1501C870();
    if (D_800BEAC0 == 0) {
        if (D_800C35EA[0] == 1) {
            func_1502378C();
        } else {
            func_1504A730();
        }
        func_15085ABC((s16)D_800D2340);
    }
    func_1507C370();
    func_1507C8FC();
}

/* Draws group 0 of player's display lists (func_151674F8), then func_15174AA4 for the level. */
Gfx *func_15018E88(Gfx *gfx, s16 player) {
    if (D_800BE616 == 0) {
        func_1517D7B0(&gfx, 1);
    }
    gfx = func_151674F8(gfx, 0, player, 0);
    gfx = func_151674F8(gfx, 0, player, 1);
    return func_15174AA4(gfx, level, player);
}

/* Counterpart of func_15018E88: func_15174B48, then group 1 of player's display lists. */
Gfx *func_15018F08(Gfx *gfx, s16 player) {
    gfx = func_15174B48(gfx, level, player);
    gfx = func_151674F8(gfx, 1, player, 0);
    gfx = func_151674F8(gfx, 1, player, 1);

    func_1517D7B0(&gfx, 2);

    return gfx;
}

/* Per-player setup before drawing player's view (D_800D3674 follows the sign of camera->CameraPitch). */
void func_15018F80(s16 player) {
    Camera *camera = &camera_global_Cameras[player];
    D_8003C8E0 = 0xA000000;
    if ((func_1517F40C(player) == 0) && (D_800BEAC0 == 0)) {
        func_1517F448(player);
    }
    func_1501B640(player);
    D_800D3674 = (camera->CameraPitch < 0.0f) ? -1000 : 5000;
    func_15172D80(player);
    D_8003C8E0 = 0;
}

/* Start of frame: reset the display-list head to this frame's buffer and update timing. */
void func_1501905C(void) {
    D_800BE9D0 = D_800BE9D8[D_800BE9C0];
    D_800BE728 = func_1501BBB8();
    func_15019F20();
    if ((D_800BEAC1 != 0) && (D_800BEAC0 == 0)) {
        func_15169040(0, 0x47);
    }
    func_1501C1B0();
    func_1501BB20();
    func_1502C380();
    if (0) {} // matching: IDO picks v0 (not v1) for the camera_global_Cameras load only with this
    func_1000D758(camera_global_Cameras->focus.x, camera_global_Cameras->focus.y, camera_global_Cameras->focus.z);
    func_10011BB8();
    if (D_800E0B94 == 2) {
        func_150ADACC(0x81280783);
    }
    func_10012020();
}


// D_80043B40 is the 508-entry TLB slot table (see the handwritten miss handler at 0x10006138,
// which uses the same {page, age} layout); this pass ages every slot by one frame.
typedef struct {
    s32 page;
    s32 age;
} TlbSlot;

#define TLB_SLOT_COUNT 508

/* The frame's game-logic update; returns 1 when the frame should be drawn. */
s32 func_15019130(void) {
    s32 i;
    u8 player;

    func_1510D864();
    func_1509BA04(0);
    func_1509BBA0(2);
    D_800DCD27 = 0;
    func_10004250();
    func_1501E400(0);
    func_15034F20();
    func_1510B690();
    func_1510F800(0);
    func_15113180();
    if (D_800BEAC0 == 0) {
        func_15113E54(1);
    }
    if (D_800BEAC0 == 0) {
        func_15114188();
    }
    func_15044A28();
    func_15018DFC();
    func_150242F8(1, 0);
    func_1501EC38(0);
    func_150242F8(0, 0);
    func_15020EC4(0);
    func_1501E2F8(0);
    if (D_800D23A9 != 0) {
        func_15087CC0();
    }
    func_15122AE0();
    func_1504ADD0();
    func_1510F800(0);
    for (i = 0; i <= D_80082FA0; i++) {
        func_1510FC34(i);
    }
    func_1510B690();
    for (player = 0; player <= D_80082FA0; player++) {
        func_15094EA0(player);
    }
    func_15112A80(0);
    func_151749F8(level, 0);
    func_1501C860();
    func_1511FC20(D_800BE9C0);
    func_15188B74(0);
    if (D_800BEAC0 == 0) {
        func_1516706C();
        func_151671E8();
    }
    func_1502BEE4();
    if (D_800BE616 == 0) {
        func_150A0D8C();
    }
    func_15113218();
    func_15188B74(1);
    func_151738C4(level);
    func_1502C1A4();
    func_15113C88();
    if (D_800CC2B0 != 0) {
        func_150636F0();
    }
    if (D_800BEAC0 == 0) {
        func_15183D28();
        func_151670C0();
        func_15177A94();
    }
    if (D_800BEAC0 == 0) {
        func_151814FC();
    }
    func_1517F75C();
    func_1517F7B4();
    func_15036148();
    func_1515D6C8();
    for (i = 0; i < TLB_SLOT_COUNT; i++) {
        ((TlbSlot *)D_80043B40)[i].age++;
    }
    return 1;
}

void func_15019414(void) {
    if (D_800BEAC0 == 0) {
        func_15167010();
    }
    func_1517ABB0();
    if (D_800BE616 == 0) {
        func_1508F0A4();
    }
}

/* Sets up player's viewport and draws the first pass of their scene. */
Gfx *func_15019464(Gfx *gfx, s16 player) {
    func_1510B958(player);
    gSPViewport(gfx++, &D_800BE628[player].viewports[D_800BE9C0]);
    gfx = func_1501A490(gfx, player, 0, 0, 0, 0);
    if (D_800BEAC0 != 0) {
        return gfx;
    }
    if (D_80084480 != 0) {
        return gfx;
    }
    gfx = func_1510FEA0(gfx, level);
    if ((func_1517EFAC(player) != 0) || (D_800D18A0 & (1 << player))) {
        return gfx;
    }
    gfx = func_1515D6D0(gfx, player);
    return func_1510B9D0(gfx, player);
}

/* Draws the main pass of player's scene. */
Gfx *func_150195A0(Gfx *gfx, s16 player) {
    Camera *camera = &camera_global_Cameras[player];

    if (D_800BEAC0 != 0) {
        return gfx;
    }
    if (D_80084480 != 0) {
        return gfx;
    }
    if ((func_1517EFAC(player) != 0) || (D_800D18A0 & (1 << player))) {
        return gfx;
    }
    if (D_80082FA0 > 0) {
        gfx = func_1515D6D0(func_1510B7B4(gfx, player), player);
        func_1510B958(player);
        gSPViewport(gfx++, &D_800BE628[player].viewports[D_800BE9C0]);
        gfx = func_1501A490(gfx, player, 0, 0, 0, 0);
    }
    gfx = func_151135C4(gfx, 1, player);
    if (D_800BE575 == 2) {
        gfx = func_151D6778(gfx);
    }
    gfx = func_151674F8(gfx, 6, player, 0);
    gfx = func_151674F8(gfx, 6, player, 1);
    gfx = func_15186794(gfx, player);
    gfx = func_1501A490(gfx, player, 0, 0, 0, 0);
    gfx = func_1502BAD0(gfx, 1, player);
    gfx = func_15035FE8(gfx, player);
    gSPMatrix(gfx++, &D_80089470, G_MTX_LOAD);
    if (camera->SpecialFlags & 1) {
        gfx = func_15018E88(gfx, player);
        gfx = func_151135C4(gfx, 0, player);
        gfx = func_15018F08(gfx, player);
    } else {
        gfx = func_15018F08(gfx, player);
        gfx = func_151135C4(gfx, 0, player);
        gfx = func_15018E88(gfx, player);
    }
    gfx = func_15188D00(gfx, 0, player);
    gfx = func_15188D00(gfx, 1, player);
    gfx = func_1502BAD0(gfx, 2, player);
    gfx = func_151674F8(gfx, 2, player, 0);
    return func_151674F8(gfx, 2, player, 1);
}

/* Full-screen pass drawn once after every player's main pass. */
Gfx *func_150198FC(Gfx *gfx) {
    static s8 D_80082F90 = 0; // frames drawn since D_800BEAC0 was set, counting up to 5

    if (D_800BE575 == 1) {
        gfx = func_151D6778(gfx);
    }
    if (D_80082FA0 != 0) {
        D_80082FA4 = D_80082FA0 + 1;
        gSPViewport(gfx++, &(D_80082FA4 + D_800BE628)->viewports[0]); // matching: index + base operand order
    } else {
        gSPViewport(gfx++, &D_800BE628[0].viewports[0]);
    }
    gfx = func_1501A490(gfx, 0xFF, 0, 0, 0, 0);
    if (D_800BEAC0 != 0) {
        if (D_80082F90 == 0) {
            gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_800BE620, D_800BE9C4);
            gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, D_800BE620, D_800BE624);
            gfx = func_151D5E90(gfx, D_8002AAE8[1 - D_800BE9C0], 0, 1);
            gfx = func_1501A680(gfx);
            D_80082F90 = 1;
        } else if (D_80082F90 < 5) {
            D_80082F90++;
            func_151D61B0(D_800BE9C4);
        }
        gfx = func_151D5E90(gfx, D_800BE9C4, 0, 1);
    } else {
        D_80082F90 = 0;
    }
    if (func_1517EFAC(0) == 0) {
        gfx = func_1517E28C(gfx, level);
        gfx = func_15172F70(gfx);
        gfx = func_15181EE0(gfx);
    }
    gfx = func_1517F564(gfx);
    if ((D_800BEAC0 == 0) && (D_800BEAC1 == 0)) {
        gfx = func_1508FD38(gfx);
    }
    gfx = func_151674F8(gfx, 3, 0, 0);
    gfx = func_151674F8(gfx, 3, 0, 1);
    return func_15096934(gfx);
}

/* Draws player's late pass (group 4 display lists) into their viewport. */
Gfx *func_15019BB8(Gfx *gfx, s32 player) {
    gSPViewport(gfx++, &D_800BE628[player].viewports[D_800BE9C0]);
    gfx = func_1501A490(gfx, player, 0, 0, 0, 0);
    gfx = func_1517F4D8(gfx, player);
    gfx = func_1517F3A0(gfx, player);
    gfx = func_15180580(gfx, player);
    if ((D_800D18A0 & (1 << player)) || (D_800D18A2 & (1 << player))) {
        gfx = func_1507DB6C(gfx, player);
    }
    gfx = func_151674F8(gfx, 4, player, 0);
    return func_151674F8(gfx, 4, player, 1);
}

/* D_800DC2A0 is one Mtx array per framebuffer, indexed by player. */
#define FRAME_MTX(fb, player) (&D_800DC2A0[fb][player])

/* Loads player's viewport and projection, then draws group 5 display lists. */
Gfx *func_15019CC8(Gfx *gfx, s32 player) {
    gSPViewport(gfx++, &D_800BE628[player].viewports[D_800BE9C0]);
    gfx = func_1501A490(gfx, player, 0, 0, 0, 0);
    gSPMatrix(gfx++, &D_80089470, G_MTX_LOAD);
    gSPMatrix(gfx++, &D_800BE628[player].projectionMtx[D_800BE9C0], G_MTX_PROJECTION | G_MTX_LOAD);
    if (level == 0x1D) {
        gfx = func_1502BAD0(gfx, 6, player);
    }
    gfx = func_151674F8(gfx, 5, 0, 0);
    gfx = func_151674F8(gfx, 5, 0, 1);
    gSPMatrix(gfx++, &D_800BE628[player].projectionMtx[D_800BE9C0], G_MTX_PROJECTION | G_MTX_LOAD);
    gSPMatrix(gfx++, FRAME_MTX(D_800BE9C0, player), G_MTX_PROJECTION);
    return gfx;
}

/* Sets the full-screen viewport and closes the frame's scene drawing. */
Gfx *func_15019E60(Gfx *gfx) {
    if (D_80082FA0 != 0) {
        D_80082FA4 = D_80082FA0 + 1;
        gSPViewport(gfx++, &(D_80082FA4 + D_800BE628)->viewports[0]); // matching: index + base operand order
    } else {
        gSPViewport(gfx++, &D_800BE628[0].viewports[0]);
    }
    return func_15043384(func_151E8620(func_1501A490(gfx, 0xFF, 0, 0, 0, 0)));
}

extern s32 D_800BE9B0;
extern f32 D_800BE9B8;
extern f32 D_800BE9BC;

/*
 * Frame timing: turns the ticks elapsed since the last frame (D_800BE9E4) into the frame delta
 * D_800BE9A4 and its reciprocal D_800BE9A8, slowed down by D_800BEA0C when that is nonzero, and keeps
 * a 10-frame running average in D_800BE9B8 / D_800BE9BC.
 */
void func_15019F20(void) {
    s32 ticks;
    u8 rate;
    f32 sum;
    u32 i;

    ticks = D_800BE9E4;
    D_800BEA08 = ticks;
    rate = D_800BEA0C;
    if (rate) {
        s32 pad; // matching: a local in this block puts a -g3 block end on the join (as1 branch fill)
        if (rate == 1) {
            D_800BE9E4 = 0;
            D_800BE5E0 = 0;
            ticks = 0;
            D_800BE9A4 = 0.0f;
        } else {
            i = ((u32 *)D_800BE5E8)[10]; // matching: reads D_800BE610 (the word after the array) through D_800BE5E8, keeping its address out of CSE
            i += ticks;
            D_800BE9A4 = (f32)ticks / (f32)(u32)rate;
            D_800BE9E4 = D_800BE9A4;
            D_800BE9A4 *= 0.5f;
            if (i >= rate) {
                D_800BE9E4 = i / rate;
                i -= D_800BE9E4 * i;
                if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} // matching: steers ticks/i register colouring
                D_800BE610 = i;
                ticks = D_800BE9E4;
            } else {
                D_800BE610 = i;
                D_800BE9E4 = ticks = 0;
            }
        }
    } else {
        D_800BE9A4 = (f32)ticks * 0.5f;
    }
    D_800BE5E0 &= 1;
    D_800BE5E0 += ticks;
    D_800BE9A0 = D_800BE5E0 >> 1;
    if (D_800BE9A4 != 0) {
        D_800BE9A8 = 1.0f / D_800BE9A4;
    } else {
        D_800BE9A4 = 0.0f;
        D_800BE9A8 = 0.0f;
        ; // matching: empty statement (a compiled-out macro?); its line entry steers as1's scheduling
    }
    D_800BE9AC += ticks;
    D_800BE9B0 = D_800BE9AC % 30;
    D_800BE5E8[D_800BE619] = D_800BE9A4;
    D_800BE619++;
    if (D_800BE619 == 10) {
        D_800BE619 = 0;
    }
    if (D_800BE61A < 10) {
        D_800BE61A++;
        D_800BE9B8 = D_800BE9A4;
        D_800BE9BC = D_800BE9A8;
    } else {
        sum = 0.0f;
        for (i = 0; i < 10; i++) {
            sum += D_800BE5E8[i];
        }
        D_800BE9B8 = sum * 0.1f;
        if (D_800BE9B8 != 0) {
            D_800BE9BC = 1.0f / D_800BE9B8;
        } else {
            D_800BE9BC = 0.0f;
        }
    }
    func_150A019C();
}
