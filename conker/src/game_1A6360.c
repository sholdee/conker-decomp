#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/effect.h describes it. */
u16 gRainSoundHandle;      /* sound handle */
u8 gWindGustTimer;       /* wind gust timer */
u8 gWindStrength;       /* wind strength */
u8 gRainRequestedState;       /* requested rain state */
s16 gRainDropCount;      /* number of live drops */
Vec3s gRainCentre;    /* centre of the rain volume */
/* RAIN_MAX_DROPS drop slots (QuadFx *), allocated while it rains.
 * matching: int-typed; a pointer-typed global changes effect_requestRainState's register allocation. */
s32 gRainDrops;
s16 gRainStateTimer;      /* state timer */
s8 gRainState;       /* rain state */

#include "functions.h"
#include "variables.h"

#define RAIN_MAX_DROPS 300
#define RAIN_DROP(i) (*(QuadFx **)(gRainDrops + (i) * sizeof(QuadFx *)))

/* Resets the rain state. */
void effect_resetRain(void) {
    gRainRequestedState = 0;
    gRainState = 0;
    gRainStateTimer = 600;
    gWindGustTimer = 240;
    gWindStrength = 20;
    gRainDropCount = 0;
    gRainDrops = 0;
}

/* Requests a rain state; 2 removes every drop and stops the sound at once. */
void effect_requestRainState(s32 state) {
    s32 i;
    QuadFx *drop;

    if (state == 2) {
        gRainRequestedState = 0;
        gRainState = 0;
        if (gRainDrops != 0) {
            for (i = 0; i < RAIN_MAX_DROPS; i++) {
                drop = RAIN_DROP(i);
                if (drop != NULL) {
                    effect_destroyObject(&drop->link);
                    RAIN_DROP(i) = NULL;
                }
            }
            heap_free((void *)gRainDrops);
            gRainDrops = 0;
        }
        sound_stop(gRainSoundHandle);
        gRainStateTimer = 0;
        gRainDropCount = 0;
    } else if (state == 3) {
        if (gRainState == 0) {
            gRainStateTimer = 600;
        } else if (gRainState == 3) {
            gRainState = 1;
        }
        gRainRequestedState = state;
    } else {
        gRainRequestedState = state;
    }
}

/* Per-frame rain update: wind gusts, then the rain state machine (0 off, 1 starting, 2 raining,
 * 3 stopping, 4 stopping at once), then the drops' centre and splashes. */
void effect_updateRain(s32 player) {
    s32 i;
    s32 gustTimer;
    s32 maxDrops;

    maxDrops = RAIN_MAX_DROPS;
    gustTimer = gWindGustTimer;
    gustTimer -= D_800BE9E4;
    if (gustTimer <= 0) {
        if (gWindStrength < 40) {
            gustTimer = (func_150ADA20_u8() % 31U) + 20;
            gWindStrength = (func_150ADA20_u8() & 0xF) + 170;
        } else {
            gustTimer = (func_150ADA20_u8() % 63U) + 60;
            gWindStrength = (func_150ADA20_u8() & 0xF) + 20;
        }
    }
    gWindGustTimer = gustTimer;
    switch (gRainState) {
    case 0:
        if (gRainRequestedState == 0) {
            break;
        }
        if (gRainRequestedState == 4 || gRainStateTimer >= 600) {
            gRainDrops = (s32)allocate_memory(RAIN_MAX_DROPS * sizeof(QuadFx *), 1, 0, 0);
            i = 0;
            do { // matching: a for loop here changes the whole function's allocation
                RAIN_DROP(i) = NULL;
                i++;
            } while (i < maxDrops);
            if (gRainDrops != 0) {
                gRainState = 1;
                gRainStateTimer = 0;
                gRainSoundHandle = sound_play(0x6F, 200, 0x40, 0, 0x80);
            }
        } else if (D_800C35EA[player] == 0) {
            gRainStateTimer += D_800BE9E4;
        }
        break;
    case 1:
        if (gRainDropCount == maxDrops) {
            gRainState = 2;
            gRainStateTimer = 0;
            break;
        }
        for (i = 0; i < 4; i++) {
            if (gRainDropCount != maxDrops) {
                effect_spawnRainDrop(1, 1);
            }
        }
        if (gRainStateTimer > 10 && gRainDropCount < maxDrops) {
            sound_setVoiceParam(gRainSoundHandle, 8, (gRainDropCount << 14) / maxDrops);
            sound_setVoiceParam(gRainSoundHandle, 0x2000, 0);
        }
        gRainStateTimer += D_800BE9E4;
        break;
    case 2:
        if (gRainRequestedState == 1) {
            gRainState = 4;
            gRainStateTimer = gRainDropCount;
        } else if (gRainRequestedState == 0 || gRainStateTimer >= 120) {
            gRainState = 3;
            gRainStateTimer = gRainDropCount;
        } else if (gRainRequestedState == 3 && D_800C35EA[player] == 0) {
            gRainStateTimer += D_800BE9E4;
        }
        break;
    case 3:
        if (gRainDropCount == 0) {
            gRainState = 0;
            gRainStateTimer = 0;
            sound_stop(gRainSoundHandle);
            heap_free((void *)gRainDrops);
            gRainDrops = 0;
            break;
        }
        for (i = 0; i < 4; i++) {
            effect_markRainDropForRemoval();
        }
        if (gRainStateTimer != 0) {
            sound_setVoiceParam(gRainSoundHandle, 8, (gRainDropCount << 13) / gRainStateTimer);
        }
        break;
    case 4:
        for (i = 0; i < gRainDropCount; i++) {
            effect_markRainDropForRemoval();
        }
        gRainState = 3;
        gRainRequestedState = 0;
        break;
    }
    if (gRainDropCount != 0) {
        effect_centreRainVolume();
        effect_spawnRainSplashes(gRainDropCount / 30);
    }
}

/* Centres the rain volume 500 units ahead of the camera (camera_global_Cameras). */
void effect_centreRainVolume(void) {
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
    gRainCentre.x = x;
    gRainCentre.z = z;
    gRainCentre.y = y;
}

/* Spawns one rain drop near the camera: at a random height when scatterY, else 500 above it. */
void effect_spawnRainDrop(s32 scatterY, s32 dropFlags) {
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
    effect_initQuadFxTemplate(&packet, 0xFF, 0xFF, 0xFF, 0x50);

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
    packet.slot = gRainDropCount;

    drop = effect_spawnQuadFx(&packet, 0xFF, 0);
    if (drop != NULL) {
        RAIN_DROP(gRainDropCount) = drop;
        gRainDropCount++;
    }
}

/* Rain drop update (QuadFx callback 1): fades with distance, drifts with the wind and wraps
 * around the rain volume; a drop marked for removal is removed when it wraps. */
void effect_updateRainDrop(QuadFx *drop) {
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
    heading += (u32)(gWindStrength * 16 * ((func_150ADA20_u8() & 0xFF) + 128)) >> 9;
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
    cx = gRainCentre.x;
    cy = gRainCentre.y;
    cz = gRainCentre.z;
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
        gRainDropCount--;
        RAIN_DROP(drop->data.slot) = NULL;
        effect_destroyObject(&drop->link);
    } else if (dropFlags & 1) {
        if (wrapped) {
            drop->data.dropFlags = dropFlags ^ 1;
        } else {
            drop->data.unkA4 = 0;
        }
    }
}

/* Marks the last live drop that isn't already marked for removal. */
void effect_markRainDropForRemoval(void) {
    s32 i;
    s32 offset;
    QuadFx **slot;
    QuadFx *drop;
    s32 stride; // matching: a stride variable keeps IDO's separate byte-offset and slot induction registers

    i = gRainDropCount - 1;
    stride = sizeof(QuadFx *);
    if (i < 0) {
        return;
    }
    offset = i * stride;
    slot = (QuadFx **)((u8 *)gRainDrops + offset);
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
void effect_spawnRainSplashes(s32 count) {
    s16 centerX;
    s16 centerZ;
    s32 i;
    f32 noGround;
    s32 range;
    s32 x;
    s32 z;
    s32 groundY;
    QuadFxData packet;

    centerX = gRainCentre.x;
    centerZ = gRainCentre.z;
    for (i = 0; i < count; i++) {
        x = (func_150ADA20_u8() % 500) + centerX;
        z = (func_150ADA20_u8() % 500) + centerZ;
        groundY = collision_findMeshFloorNear(x, 10000, z, 0);
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
            effect_initQuadFxTemplate(&packet, 0xFF, 0xFF, 0xFF, 0xFF);
            effect_spawnQuadFx(&packet, 0xFF, 0);
        }
    }
}

/* Splash update (QuadFx callback 0xE): fades with distance from the camera. */
void effect_updateRainSplash(QuadFx *splash) {
    f32 dx;
    f32 dz;
    f32 dist;

    dx = splash->data.z - camera_global_Cameras->eye.z;
    dz = splash->data.x - camera_global_Cameras->eye.x;
    dist = sqrtf((dx * dx) + (dz * dz));
    splash->data.unkA4 = 255.0f - (160.0f * dist) / 1000.0f;
}
