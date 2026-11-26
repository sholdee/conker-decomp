#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/effect.h describes it. */
u16 D_800DD430;      /* sound handle */
u8 D_800DD432;       /* wind gust timer */
u8 D_800DD433;       /* wind strength */
u8 D_800DD434;       /* requested rain state */
s16 D_800DD436;      /* number of live drops */
Vec3s D_800DD438;    /* centre of the rain volume */
/* RAIN_MAX_DROPS drop slots (QuadFx *), allocated while it rains.
 * matching: int-typed; a pointer-typed global changes func_15178EFC's register allocation. */
s32 D_800DD440;
s16 D_800DD444;      /* state timer */
s8 D_800DD446;       /* rain state */

#include "functions.h"
#include "variables.h"

#define RAIN_MAX_DROPS 300
#define RAIN_DROP(i) (*(QuadFx **)(D_800DD440 + (i) * sizeof(QuadFx *)))

/* Resets the rain state. */
void func_15178EB0(void) {
    D_800DD434 = 0;
    D_800DD446 = 0;
    D_800DD444 = 600;
    D_800DD432 = 240;
    D_800DD433 = 20;
    D_800DD436 = 0;
    D_800DD440 = 0;
}

/* Requests a rain state; 2 removes every drop and stops the sound at once. */
void func_15178EFC(s32 state) {
    s32 i;
    QuadFx *drop;

    if (state == 2) {
        D_800DD434 = 0;
        D_800DD446 = 0;
        if (D_800DD440 != 0) {
            for (i = 0; i < RAIN_MAX_DROPS; i++) {
                drop = RAIN_DROP(i);
                if (drop != NULL) {
                    func_1516972C(&drop->link);
                    RAIN_DROP(i) = NULL;
                }
            }
            func_10004074((void *)D_800DD440);
            D_800DD440 = 0;
        }
        func_100111C8(D_800DD430);
        D_800DD444 = 0;
        D_800DD436 = 0;
    } else if (state == 3) {
        if (D_800DD446 == 0) {
            D_800DD444 = 600;
        } else if (D_800DD446 == 3) {
            D_800DD446 = 1;
        }
        D_800DD434 = state;
    } else {
        D_800DD434 = state;
    }
}

/* Per-frame rain update: wind gusts, then the rain state machine (0 off, 1 starting, 2 raining,
 * 3 stopping, 4 stopping at once), then the drops' centre and splashes. */
void func_15179008(s32 player) {
    s32 i;
    s32 gustTimer;
    s32 maxDrops;

    maxDrops = RAIN_MAX_DROPS;
    gustTimer = D_800DD432;
    gustTimer -= D_800BE9E4;
    if (gustTimer <= 0) {
        if (D_800DD433 < 40) {
            gustTimer = (func_150ADA20_u8() % 31U) + 20;
            D_800DD433 = (func_150ADA20_u8() & 0xF) + 170;
        } else {
            gustTimer = (func_150ADA20_u8() % 63U) + 60;
            D_800DD433 = (func_150ADA20_u8() & 0xF) + 20;
        }
    }
    D_800DD432 = gustTimer;
    switch (D_800DD446) {
    case 0:
        if (D_800DD434 == 0) {
            break;
        }
        if (D_800DD434 == 4 || D_800DD444 >= 600) {
            D_800DD440 = (s32)allocate_memory(RAIN_MAX_DROPS * sizeof(QuadFx *), 1, 0, 0);
            i = 0;
            do { // matching: a for loop here changes the whole function's allocation
                RAIN_DROP(i) = NULL;
                i++;
            } while (i < maxDrops);
            if (D_800DD440 != 0) {
                D_800DD446 = 1;
                D_800DD444 = 0;
                D_800DD430 = func_10010F30(0x6F, 200, 0x40, 0, 0x80);
            }
        } else if (D_800C35EA[player] == 0) {
            D_800DD444 += D_800BE9E4;
        }
        break;
    case 1:
        if (D_800DD436 == maxDrops) {
            D_800DD446 = 2;
            D_800DD444 = 0;
            break;
        }
        for (i = 0; i < 4; i++) {
            if (D_800DD436 != maxDrops) {
                func_15179600(1, 1);
            }
        }
        if (D_800DD444 > 10 && D_800DD436 < maxDrops) {
            func_1000F85C(D_800DD430, 8, (D_800DD436 << 14) / maxDrops);
            func_1000F85C(D_800DD430, 0x2000, 0);
        }
        D_800DD444 += D_800BE9E4;
        break;
    case 2:
        if (D_800DD434 == 1) {
            D_800DD446 = 4;
            D_800DD444 = D_800DD436;
        } else if (D_800DD434 == 0 || D_800DD444 >= 120) {
            D_800DD446 = 3;
            D_800DD444 = D_800DD436;
        } else if (D_800DD434 == 3 && D_800C35EA[player] == 0) {
            D_800DD444 += D_800BE9E4;
        }
        break;
    case 3:
        if (D_800DD436 == 0) {
            D_800DD446 = 0;
            D_800DD444 = 0;
            func_100111C8(D_800DD430);
            func_10004074((void *)D_800DD440);
            D_800DD440 = 0;
            break;
        }
        for (i = 0; i < 4; i++) {
            func_15179AB8();
        }
        if (D_800DD444 != 0) {
            func_1000F85C(D_800DD430, 8, (D_800DD436 << 13) / D_800DD444);
        }
        break;
    case 4:
        for (i = 0; i < D_800DD436; i++) {
            func_15179AB8();
        }
        D_800DD446 = 3;
        D_800DD434 = 0;
        break;
    }
    if (D_800DD436 != 0) {
        func_151794C8();
        func_15179B14(D_800DD436 / 30);
    }
}

/* Centres the rain volume 500 units ahead of the camera (camera_global_Cameras). */
void func_151794C8(void) {
    s32 x;
    s32 y;
    s32 z;
    f32 angle;
    f32 vy;
    f32 vz;
    f32 c;
    f32 s;
    f32 vx;

    x = camera_global_Cameras->eye.x;
    y = camera_global_Cameras->eye.y;
    z = camera_global_Cameras->eye.z;

    /* offset (0, 500, 0) rotated by unk398, then by unk3A0 */
    angle = camera_global_Cameras->CameraPitchRad;
    c = func_150AD78C(angle);
    s = func_150AD780(angle);
    vy = 500.0f * c;
    vz = -500.0f * s;
    vx = 0.0f;

    angle = camera_global_Cameras->TrueCameraAngleRad;
    c = func_150AD78C(angle);
    s = func_150AD780(angle);
    x += vx * s + vz * c;
    z += vz * s - vx * c;
    y += vy;
    D_800DD438.x = x;
    D_800DD438.z = z;
    D_800DD438.y = y;
}

/* Spawns one rain drop near the camera: at a random height when scatterY, else 500 above it. */
void func_15179600(s32 scatterY, s32 dropFlags) {
    s32 x;
    s32 y;
    s32 z;
    s32 baseX;
    s32 baseY;
    s32 baseZ;
    s32 rand;
    QuadFx *drop;
    s32 pad; // matching: unused stack slot
    QuadFxData packet;

    packet.animIndex = 0x11;
    packet.frameStep = 0x33;
    packet.callback = 1;
    packet.flags = 0;
    func_1516865C(&packet, 0xFF, 0xFF, 0xFF, 0x50);

    baseX = camera_global_Cameras->eye.x;
    baseY = camera_global_Cameras->eye.y;
    baseZ = camera_global_Cameras->eye.z;

    x = (func_150ADA20_u8() % 500) + baseX;
    z = (func_150ADA20_u8() % 500) + baseZ;
    if (scatterY == 0) {
        y = baseY + 500;
    } else {
        y = (func_150ADA20_u8() % 500) + baseY;
    }

    rand = func_150ADA20_u8() % 10U;
    packet.unk9C = 0;
    packet.heading = 0;
    packet.y = y;
    packet.x = x;
    packet.z = z;
    packet.unk92 = 4;
    packet.unk94 = rand + 35;
    packet.flags = 0x8C00;
    packet.speed = rand + 10;
    packet.dropFlags = dropFlags;
    packet.slot = D_800DD436;

    drop = func_15168800(&packet, 0xFF, 0);
    if (drop != NULL) {
        RAIN_DROP(D_800DD436) = drop;
        D_800DD436++;
    }
}

/* Rain drop update (QuadFx callback 1): fades with distance, drifts with the wind and wraps
 * around the rain volume; a drop marked for removal is removed when it wraps. */
void func_151797B0(QuadFx *drop) {
    f32 dist;
    f32 sn;
    f32 cs;
    f32 dx;
    f32 step;
    f32 dz;
    s32 heading;
    s32 alpha;
    s32 wrapped;
    s32 dropFlags;
    s16 cx;
    s16 cy;
    s16 cz;

    dx = drop->data.z - camera_global_Cameras->eye.z;
    dz = drop->data.x - camera_global_Cameras->eye.x;
    dist = sqrtf(dx * dx + dz * dz);
    alpha = 80.0f - (80.0f * dist) / 1000.0f;
    if (alpha < 0) {
        alpha = 0;
    }
    heading = drop->data.heading;
    drop->data.unkA4 = alpha;
    heading += (u32)(D_800DD433 * 16 * ((func_150ADA20_u8() & 0xFF) + 128)) >> 9;
    heading = heading * 0.888890028f;
    if (heading > 0x3200) {
        heading = 0x3200;
    }
    drop->data.heading = heading;
    step = drop->data.speed * D_800BE9E4;
    sn = func_15048A40(drop->data.heading >> 8);
    drop->data.x = drop->data.x + step * sn;
    cs = func_150489B0(drop->data.heading >> 8);
    drop->data.y = drop->data.y - step * cs;
    wrapped = FALSE;
    cx = D_800DD438.x;
    cy = D_800DD438.y;
    cz = D_800DD438.z;
    if (drop->data.x < cx - 500) {
        drop->data.x = cx - (cx - drop->data.x) + 1000;
        wrapped = TRUE;
    } else if (drop->data.x > cx + 500) {
        drop->data.x = cx + (drop->data.x - cx) - 1000;
        wrapped = TRUE;
    }
    if (drop->data.y < cy - 500) {
        drop->data.y = cy - (cy - drop->data.y) + 1000;
        wrapped = TRUE;
    } else if (drop->data.y > cy + 500) {
        drop->data.y = cy + (drop->data.y - cy) - 1000;
        wrapped = TRUE;
    }
    if (drop->data.z < cz - 500) {
        drop->data.z = cz - (cz - drop->data.z) + 1000;
        wrapped = TRUE;
    } else if (drop->data.z > cz + 500) {
        drop->data.z = cz + (drop->data.z - cz) - 1000;
        wrapped = TRUE;
    }
    dropFlags = drop->data.dropFlags;
    if ((dropFlags & 2) && wrapped) {
        D_800DD436--;
        RAIN_DROP(drop->data.slot) = NULL;
        func_1516972C(&drop->link);
    } else if (dropFlags & 1) {
        if (wrapped) {
            drop->data.dropFlags = dropFlags ^ 1;
        } else {
            drop->data.unkA4 = 0;
        }
    }
}

/* Marks the last live drop that isn't already marked for removal. */
void func_15179AB8(void) {
    s32 i;
    s32 offset;
    QuadFx **slot;
    QuadFx *drop;
    s32 stride; // matching: a stride variable keeps IDO's separate byte-offset and slot induction registers

    i = D_800DD436 - 1;
    stride = sizeof(QuadFx *);
    if (i < 0) {
        return;
    }
    offset = i * stride;
    slot = (QuadFx **)((u8 *)D_800DD440 + offset);
    do {
        drop = *slot;
        if (drop != NULL && !(drop->data.dropFlags & 2)) {
            drop->data.dropFlags |= 2;
            return;
        }
        offset -= stride;
        slot--;
    } while (offset >= 0);
}

/* Spawns count splashes on the ground at random points of the rain volume. */
void func_15179B14(s32 count) {
    s16 centerX;
    s16 centerZ;
    s32 i;
    f32 noGround;
    s32 range;
    s32 x;
    s32 z;
    s32 groundY;
    QuadFxData packet;

    centerX = D_800DD438.x;
    centerZ = D_800DD438.z;
    for (i = 0; i < count; i++) {
        x = (func_150ADA20_u8() % 500) + centerX;
        z = (func_150ADA20_u8() % 500) + centerZ;
        groundY = func_1510F8D8(x, 10000, z, 0);
        if (groundY != -10000.0f) {
            packet.dropFlags = 0;
            packet.frameStep = 0x80;
            packet.x = x;
            packet.y = groundY;
            packet.z = z;
            packet.unk94 = 25;
            packet.unk92 = 25;
            packet.floorY = 0;
            packet.callback = 0xE;
            packet.animIndex = 0x12;
            packet.flags = 0x9804;
            packet.speed = 0;
            func_1516865C(&packet, 0xFF, 0xFF, 0xFF, 0xFF);
            func_15168800(&packet, 0xFF, 0);
        }
    }
}

/* Splash update (QuadFx callback 0xE): fades with distance from the camera. */
void func_15179CB0(QuadFx *splash) {
    f32 dx;
    f32 dz;
    f32 dist;

    dx = splash->data.z - camera_global_Cameras->eye.z;
    dz = splash->data.x - camera_global_Cameras->eye.x;
    dist = sqrtf((dx * dx) + (dz * dz));
    splash->data.unkA4 = 255.0f - (160.0f * dist) / 1000.0f;
}
