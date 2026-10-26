#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern f32 D_800A3420;
extern f32 D_800A3424;

void func_151219D0(struct108 *arg0) {
    s32 i;
    f32 minX, maxX, minZ, maxZ, sumY;
    f32 dx, dz, maxDist;
    f32 midZ;
    f32 x, z;

    minZ = 10000.0f;
    maxZ = -10000.0f;
    minX = 10000.0f;
    maxX = -10000.0f;
    sumY = 0.0f;
    for (i = 0; i < D_8008FD8C; i++) {
        x = D_800CC2D0[i].x_position;
        if (x < minX) {
            minX = x;
        }
        if (maxX < x) {
            maxX = x;
        }
        z = D_800CC2D0[i].z_position;
        sumY += D_800CC2D0[i].y_position;
        if (z < minZ) {
            minZ = z;
        }
        if (maxZ < z) {
            maxZ = z;
        }
    }
    sumY /= D_8008FD8C;
    arg0->unk2BC = maxX;
    arg0->unk2C0 = sumY + 150.0f;
    maxDist = 0.0f;
    midZ = (minZ + maxZ) * 0.5f;
    arg0->unk2C4 = midZ;
    for (i = 0; i < D_8008FD8C; i++) {
        dx = (D_800CC2D0[i].x_position - maxX) * 0.5f;
        if (dx < 0.0f) {
            dx = 0.0f;
        }
        dz = D_800CC2D0[i].z_position - midZ;
        if (dz < 0.0f) {
            dz = -dz;
        }
        if (dz < dx) {
            dz = dx;
        }
        if (maxDist < dz) {
            maxDist = dz;
        }
    }
    if (maxDist < 300.0f) {
        maxDist = 300;
    }
    maxDist += 100.0f;
    arg0->unk2F8 = arg0->unk2BC + (maxDist + maxDist);
    arg0->unk2FC = arg0->unk2C0 + (maxDist * 0.5f);
    arg0->unk300 = arg0->unk2C4;
}

extern f32 D_800A3430;

void func_15121C00(struct108 *arg0, f32 arg1, s32 arg2, f32 arg3, f32 arg4) {
    func_15049688(&arg0->unk37C, arg1, (f32 *)&arg0->pad8C0, arg3, arg4, arg0->unk7B4);
    arg0->unk39C = arg0->unk37C * D_800A3430;
}

void func_15121C64(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 *p = &arg0;
}
