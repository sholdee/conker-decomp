#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The five ropes of the level (higher end first). */
static const RopeEnds D_80096320 = { { -2544.0f, 2426.0f, 2213.0f }, { -2544.0f, 1144.0f, 2213.0f } };
static const RopeEnds D_80096338 = { { -1987.0f, 1380.0f, 1904.0f }, { -1987.0f, 249.0f, 1904.0f } };
static const RopeEnds D_80096350 = { { -2162.0f, 534.0f, 1344.0f }, { -2162.0f, -645.0f, 1344.0f } };
static const RopeEnds D_80096368 = { { -2128.0f, -701.0f, 2129.0f }, { -2128.0f, -1545.0f, 2129.0f } };
static const RopeEnds D_80096380 = { { -2893.0f, 2648.0f, 2174.0f }, { -2893.0f, 2111.0f, 2174.0f } };

/* Starts an object emitter at (x, y, z) over the floor at floorY. */
void func_1500F9D0(s32 x, s32 y, s32 z, s32 floorY, s32 arg4) {
    ObjectEmitter *emitter;

    emitter = func_151491F4((func_150ADA20() & 0x7F) + 0xA, 1, -1, 1, 0, 0xA, 0xFF, 0);
    if (emitter != NULL) {
        emitter->pos.x = x;
        emitter->pos.y = y;
        emitter->pos.z = z;
        emitter->pos.floorY = floorY;
        emitter->pos.unk8 = arg4;
    }
}

void func_1500FA64(void) {
    RopeSpawn tmp;

    tmp.unk20 = 0;

    tmp.ends = D_80096320;
    tmp.id = 1;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    func_151ACBD4(&tmp, 0);

    tmp.ends = D_80096338;
    tmp.id = 2;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    func_151ACBD4(&tmp, 0);

    tmp.ends = D_80096350;
    tmp.id = 3;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    func_151ACBD4(&tmp, 0);

    tmp.ends = D_80096368;
    tmp.id = 4;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    tmp.ends = D_80096380;
    tmp.id = 5;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    func_151ACBD4(&tmp, 0);

    func_1500F9D0(-0x6E7, -0x46A, 0x421, -0x648, 1);
    func_1500F9D0(-0xA71, 0x71B, 0x82B, -0x602, 2);
    func_1500F9D0(-0x870, 0x36E, 0x4E1, -0x602, 2);
    func_1500F9D0(-0xBD5, 0x3F1, 0x659, -0x602, 2);
    func_1500F9D0(-0xE6B, -0x263, -0x427, -0x647, 1);

    D_800DCDA4 = gAmbientColour[0];
    D_800DCDA8 = gAmbientColour[1];
    D_800DCDAC = gAmbientColour[2];
    D_800DCDB0 = D_800DCD28[0];
    D_800DCDB4 = D_800DCD28[1];
    D_800DCDB8 = D_800DCD28[2];
    D_800DCDBC = D_800DCD3C;
    D_800DCDC0 = D_800DCD3D;
}
