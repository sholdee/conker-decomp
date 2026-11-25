#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#define func_10010F30 func_10010F30_void
#include "functions.h"
#undef func_15048A40
#undef func_10010F30
#define D_800DD444 D_800DD444_hdr
#define D_800C35EA D_800C35EA_hdr
#include "variables.h"
#undef D_800DD444
#undef D_800C35EA

#define RAIN_MAX_DROPS 300

/* Link fields shared by every object (see game_1944C0.c). */
typedef struct ObjLink {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 list;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ struct ObjLink *prev;
    /* 0x8 */ struct ObjLink *next;
    /* 0xC */ u8 owner;
} ObjLink;

/* Template / payload of a type 0xE QuadFx object (see game_1944C0.c). */
typedef struct {
    /* 0x00 */ Vtx vtx[8];
    /* 0x80 */ s32 dropFlags;   /* 1: hidden until it first wraps, 2: remove when it next wraps */
    /* 0x84 */ s16 slot;        /* index in D_800DD440 */
    /* 0x86 */ u8 pad86[2];
    /* 0x88 */ s16 frame;
    /* 0x8A */ s16 frameStep;   /* 8.8 fixed point */
    /* 0x8C */ s16 x;
    /* 0x8E */ s16 y;
    /* 0x90 */ s16 z;
    /* 0x92 */ s16 unk92;
    /* 0x94 */ s16 unk94;
    /* 0x96 */ s16 unk96;
    /* 0x98 */ u16 flags;
    /* 0x9A */ u8 pad9A[2];
    /* 0x9C */ s16 unk9C;
    /* 0x9E */ u16 heading;     /* binary angle, high byte indexes the sin/cos tables */
    /* 0xA0 */ u8 animIndex;    /* index into D_8008CA4C */
    /* 0xA1 */ u8 speed;
    /* 0xA2 */ s8 callback;     /* index into D_8008C9C8, -1 = none */
    /* 0xA3 */ u8 padA3;
    /* 0xA4 */ u8 unkA4;
} QuadFxData;

/* Type 0xE object. */
typedef struct {
    /* 0x00 */ ObjLink link;
    /* 0x10 */ QuadFxData data;
} QuadFx;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} Vec3s;

extern u16 D_800DD430;      /* sound handle */
extern u8 D_800DD432;       /* wind gust timer */
extern u8 D_800DD433;       /* wind strength */
extern u8 D_800DD434;       /* requested rain state */
extern s16 D_800DD436;      /* number of live drops */
extern Vec3s D_800DD438;    /* centre of the rain volume */
/* RAIN_MAX_DROPS drop slots (QuadFx *), allocated while it rains.
 * matching: int-typed; a pointer-typed global changes func_15178EFC's register allocation. */
extern s32 D_800DD440;
#define RAIN_DROP(i) (*(QuadFx **)(D_800DD440 + (i) * sizeof(QuadFx *)))
extern s16 D_800DD444;      /* state timer */
extern s8 D_800DD446;       /* rain state */
extern u8 D_800C35EA[];
extern f32 D_800A7208;

extern f32 sqrtf(f32);
extern void func_10004074(void *);
extern s32 func_10010F30(s32, u16, u8, s16, u8);
extern void *allocate_memory(s32, s32, s32, s32);
extern f32 func_15048A40(u8);
extern f32 func_150489B0(u8);
void func_1516865C(QuadFxData *q, s32 r, s32 g, s32 b, s32 a);
QuadFx *func_15168800();
s32 func_1510F8D8(s32 x, s32 y, s32 z, s32 arg3);

void func_151794C8(void);
void func_15179600(s32 arg0, s32 dropFlags);
void func_15179AB8(void);
void func_15179B14(s32 count);

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
                    func_1516972C((struct102 *)drop);
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
            gustTimer = (func_150ADA20() % 31U) + 20;
            D_800DD433 = (func_150ADA20() & 0xF) + 170;
        } else {
            gustTimer = (func_150ADA20() % 63U) + 60;
            D_800DD433 = (func_150ADA20() & 0xF) + 20;
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
            do {
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

/* Centres the rain volume 500 units ahead of the camera (D_800DBFF0). */
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

    x = (s32)D_800DBFF0->unk2F8;
    y = (s32)D_800DBFF0->unk2FC;
    z = (s32)D_800DBFF0->unk300;

    /* offset (0, 500, 0) rotated by unk398, then by unk3A0 */
    angle = D_800DBFF0->unk398;
    c = func_150AD78C(angle);
    s = func_150AD780(angle);
    vy = 500.0f * c;
    vz = -500.0f * s;
    vx = 0.0f;

    angle = D_800DBFF0->unk3A0;
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
    s32 pad[2];
    QuadFxData packet;

    packet.animIndex = 0x11;
    packet.frameStep = 0x33;
    packet.callback = 1;
    packet.flags = 0;
    func_1516865C(&packet, 0xFF, 0xFF, 0xFF, 0x50);

    baseX = (s32)D_800DBFF0->unk2F8;
    baseY = (s32)D_800DBFF0->unk2FC;
    baseZ = (s32)D_800DBFF0->unk300;

    x = (func_150ADA20() % 500) + baseX;
    z = (func_150ADA20() % 500) + baseZ;
    if (scatterY == 0) {
        y = baseY + 500;
    } else {
        y = (func_150ADA20() % 500) + baseY;
    }

    rand = func_150ADA20() % 10U;
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

    drop = func_15168800(&packet, 0xFF, 0, y);
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

    dx = (f32)drop->data.z - D_800DBFF0->unk300;
    dz = (f32)drop->data.x - D_800DBFF0->unk2F8;
    dist = sqrtf(dx * dx + dz * dz);
    alpha = 80.0f - (80.0f * dist) / 1000.0f;
    if (alpha < 0) {
        alpha = 0;
    }
    heading = drop->data.heading;
    drop->data.unkA4 = alpha;
    heading += (u32)((D_800DD433 << 4) * ((func_150ADA20() & 0xFF) + 0x80)) >> 9;
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
        func_1516972C((struct102 *)drop);
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
        x = (func_150ADA20() % 500) + centerX;
        z = (func_150ADA20() % 500) + centerZ;
        groundY = func_1510F8D8(x, 10000, z, 0);
        if (groundY != -10000.0f) {
            packet.dropFlags = 0;
            packet.frameStep = 0x80;
            packet.x = x;
            packet.y = groundY;
            packet.z = z;
            packet.unk94 = 25;
            packet.unk92 = 25;
            packet.unk96 = 0;
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

    dx = (f32)splash->data.z - D_800DBFF0->unk300;
    dz = (f32)splash->data.x - D_800DBFF0->unk2F8;
    dist = sqrtf((dx * dx) + (dz * dz));
    splash->data.unkA4 = (u32)(255.0f - ((160.0f * dist) / 1000.0f));
}
