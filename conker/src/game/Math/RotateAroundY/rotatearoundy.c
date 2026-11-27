#include <ultra64.h>

#include <math.h>
#include "conker/math.h"

/*
 * This unit's .data and .bss, by their place between game/Mp/MpGame/mpgame's and game/Hud/PlayerOverlay/playeroverlay's: no function
 * here reads them. D_80087358 is read by the hand-written script_runTask (game/Script/ScriptInterp/scriptinterp.s);
 * gLinkGroupCount and gLinkGroups by the link-group functions at the end of game/Mp/MpGame/mpgame (player_loadLinkGroups
 * in game/Player/StatsInit/statsinit fills them).
 */
u8 D_80087350[5] = { 0, 2, 4, 6, 8 }; /* not referenced */
/* 40 bytes: gLinkGroupCount follows at 0x80087380 */
u8 D_80087358[40] = {
    1, 4, 4, 4, 4, 4, 3, 3, 3, 4, 4, 4, 4, 4, 3, 2, 3, 3, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 1, 4, 4,
};
u32 gLinkGroupCount = 0;  /* number of groups in gLinkGroups */
ObjGroup *gLinkGroups; /* the level's link groups */

void math_rotateAroundY(Vec3f *arg0, Vec3f *arg1, f32 arg2, Vec3f *arg3) {
    f32 dx;
    f32 dz;
    f32 ang;
    Vec3f out;

    dx = arg0->x - arg1->x;
    dz = arg0->z - arg1->z;
    ang = arg2 * 0.01745329238f; // degrees to radians
    out.x = ((cosf(ang) * dx) + (sinf(ang) * dz)) + arg1->x;
    out.z = ((-sinf(ang) * dx) + (cosf(ang) * dz)) + arg1->z;
    arg3->x = out.x;
    arg3->z = out.z;
}
