#include <ultra64.h>
#define func_15178E14 func_15178E14_header_mismatch
#define func_10010F88 func_10010F88_hdr
#define func_150ADA20 func_150ADA20_hdr
#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_150ADA20
#undef func_15048A40
#undef func_15178E14
#undef func_10010F88
#define func_10010F88_ret func_10010F88
#include "variables.h"

typedef struct Func15178750Inner {
    u8 pad0[0x36];
    u8 unk36;
} Func15178750Inner;

typedef struct Func15178750Arg1 {
    u8 pad0[0x14];
    Func15178750Inner *unk14;
} Func15178750Arg1;

extern s32 func_15168118();


u32 func_150ADA20(void);
f32 func_15048A40(u8);
f32 func_150489B0(u8);
void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern s32 D_8008CA64;

/* Object type 0x3A: a swarm centre that steers towards a target and owns a group of members. */
typedef struct Swarm {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ struct Swarm *next;
    /* 0x0C */ u8 padC[0x4];
    /* 0x10 */ union {
        f32 *pos;                       /* when targetZ.z == -0x8000 */
        struct { s16 x; s16 y; } fixed; /* otherwise, with targetZ.z */
        s32 packed;
    } target;
    /* 0x14 */ union {
        s16 z;
        s32 packed;
    } targetZ;
    /* 0x18 */ s32 x;
    /* 0x1C */ s32 y;
    /* 0x20 */ s32 z;
    /* 0x24 */ u8 pad24[0x4];
    /* 0x28 */ s16 prevX;
    /* 0x2A */ s16 prevY;
    /* 0x2C */ s16 prevZ;
    /* 0x2E */ u16 sfxHandle;
    /* 0x30 */ s16 yOffset;
    /* 0x32 */ s16 speed;
    /* 0x34 */ u8 tag;
    /* 0x35 */ u8 memberCount;
    /* 0x36 */ u8 viewMask; /* bit i set when visible in view i */
    /* 0x37 */ u8 playSfx;
} Swarm;

/* Per-member state; a template copy of it initialises each new member. */
typedef struct SwarmMemberState {
    /* 0x00 */ s32 unk0; /* pointer from the D_8008CA4C table */
    /* 0x04 */ Swarm *swarm;
    /* 0x08 */ s32 steer; /* (heading << 4) | timer */
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 velX;
    /* 0x18 */ s16 velZ;
    /* 0x1A */ s16 angle;
    /* 0x1C */ u8 pad1C[0x3];
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 turnRate;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ u8 pad30[0x4];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 pad36[0x2];
} SwarmMemberState; /* size 0x38 */

/* Object type 0x3B: one member of a Swarm. */
typedef struct SwarmMember {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ struct SwarmMember *next;
    /* 0x0C */ u8 padC[0x4];
    /* 0x10 */ SwarmMemberState s;
    /* 0x48 */ s8 unk48;
} SwarmMember;

/* Spawns a swarm at (x, y, z) following targetPos (or the fixed point x, y, z), plus memberCount members. */
void func_15177F90(u8 memberCount, f32 *targetPos, u8 tag, s32 x, s32 y, s32 z, s32 arg6, s32 arg7, u8 arg8,
                   s32 playSfx, s32 argA) {
    SwarmMemberState template;
    Swarm *swarm;
    SwarmMember *member;
    s32 i;
    s32 angle;
    f32 c;
    f32 s;
    f32 r;
    s32 px;
    s32 pz;
    s32 py;
    s32 pad; // matching: unused stack slot keeps the frame size

    swarm = func_15167A68(0x3A, 0, sizeof(Swarm), 0, arg8, 1);
    if (swarm == NULL) {
        return;
    }
    if (argA == 0) {
        argA = 280;
    } else {
        argA = 140;
    }
    if (targetPos != NULL) {
        swarm->target.pos = targetPos;
        swarm->targetZ.packed = 0x80000000;
    } else {
        swarm->target.packed = (x << 16) | (y & 0xFFFF);
        swarm->targetZ.packed = z << 16;
    }
    swarm->x = x;
    swarm->y = y;
    swarm->z = z;
    swarm->yOffset = 0;
    swarm->speed = 800;
    swarm->memberCount = memberCount;
    swarm->tag = tag;
    swarm->playSfx = playSfx;
    swarm->sfxHandle = 0;
    if (0) {} // matching: the extra basic block steers IDO's register allocation
    template.unk0 = D_8008CA64;
    template.swarm = swarm;
    template.velX = 0;
    template.velZ = 0;
    template.angle = 0;
    template.unk1F = 6;
    template.unk20 = 0;
    template.unk22 = 0;
    template.unk24 = argA;
    template.unk26 = argA;
    template.unk28 = arg7;
    template.unk2A = 0;
    template.unk2C = 0xFF;
    template.unk2D = 0xFF;
    template.unk2E = 0xFF;
    template.unk2F = 0xFF;
    template.unk34 = 0;
    for (i = 0; i < memberCount; i++) {
        angle = func_150ADA20() & 0xFF;
        c = func_15048A40(angle);
        s = func_150489B0(angle);
        r = (func_150ADA20() & 0xFFFF) * 50.0f * (1.0f / 65536.0f);
        px = x + r * c;
        pz = z + r * s;
        py = (func_150ADA20() & 0xFF) + y - 78;
        template.turnRate = (func_150ADA20() & 7) + 7;
        template.unkC = 0;
        template.unkE = 0;
        template.x = px;
        template.y = py;
        template.z = pz;
        member = func_15167A68(0x3B, 0, sizeof(SwarmMember), 0, arg8, 1);
        if (member == NULL) {
            break;
        }
        bcopy(&template, &member->s, sizeof(template));
        member->unk48 = -1;
    }
}

s32 func_150490A8(f32 *);
extern s32 D_8008CA4C[];

/* Member update: steer towards the swarm, bob up and down, and pick a sprite from its velocity relative to D_800DBFF0->unk380. */
void func_15178268(SwarmMember *member) {
    s32 timer;
    s32 turn;
    s32 delta;
    s32 velX;
    s32 diff;
    s32 angle;
    s32 heading;
    s32 v;
    s32 absDiff;
    f32 dist;
    f32 x;
    f32 y;
    f32 z;
    s32 rnd;
    f32 vec[3];
    f32 dx;
    f32 dz;
    f32 s;
    f32 c;
    f32 radius;
    u8 yaw;
    struct108 *gp;
    Swarm *swarm;
    s32 fast;

    swarm = member->s.swarm;
    angle = member->s.angle;
    x = swarm->x;
    y = swarm->y;
    z = swarm->z;
    if (swarm->speed > 800) {
        fast = TRUE;
    } else {
        fast = FALSE;
    }
    { // matching: -g3 scope (with a local) is a scheduling barrier for as1
        s32 pad;

        y += swarm->yOffset;
    }
    dx = x - swarm->x;
    dz = z - swarm->z;
    dist = sqrtf(dx * dx + dz * dz);
    if (dist > 100.0f) {
        dist = 100.0f / dist;
        x = dx * dist + swarm->x;
        z = dz * dist + swarm->z;
    }
    vec[0] = x - member->s.x;
    vec[2] = z - member->s.z;
    timer = member->s.steer & 0xF;
    if (timer == 0) {
        heading = func_150490A8(vec);
        heading += (s32)(func_150ADA20() & 0x1F) - 15;
        if (!fast) {
            timer = 10;
        } else {
            timer = 2;
        }
    } else {
        timer--;
        heading = (member->s.steer >> 4) & 0xFF;
    }
    member->s.steer = ((heading & 0xFF) << 4) | timer;
    turn = member->s.turnRate;
    if (fast) {
        turn *= 2;
    }
    if (angle < heading) {
        delta = turn;
    } else {
        delta = -turn;
    }
    diff = heading - angle;
    if (diff < 0) {
        absDiff = -diff;
    } else {
        absDiff = diff;
    }
    if (absDiff >= 0x80) {
        delta = -delta;
    }
    angle += delta;
    if (angle < 0) {
        angle += 0x100;
    } else if (angle >= 0x100) {
        angle -= 0x100;
    }
    radius = swarm->speed;
    velX = func_15048A40(angle) * radius;
    c = -func_150489B0(angle);
    member->s.velX = velX;
    member->s.velZ = c * radius;
    member->s.angle = angle;
    rnd = (func_150ADA20() & 0x7F) + 100;
    if ((f32)(func_150ADA20() & 0x3F) + (y + 50.0f) < member->s.y) {
        member->s.unk22 = -rnd;
    } else {
        member->s.unk22 = rnd;
    }
    if (member->s.unk20 > 0x400) {
        member->s.unk20 = 0x400;
    } else if (member->s.unk20 < -0x400) {
        member->s.unk20 = -0x400;
    }
    gp = D_800DBFF0;
    yaw = gp->unk380 * (256.0f / 360.0f);
    s = func_15048A40(yaw);
    c = func_150489B0(yaw);
    v = member->s.velX * c - member->s.velZ * s;
    if (v >= 0) {
        member->s.unk34 = 0;
    } else {
        member->s.unk34 = 4;
        v = -v;
    }
    if (v > 0x200) {
        v = 6;
    } else if (v > 0x100) {
        v = 7;
    } else {
        v = member->s.velX * s + member->s.velZ * c;
        if (v >= 0) {
            v = 8;
        } else {
            v = 9;
        }
    }
    member->s.unk0 = D_8008CA4C[v];
    member->s.unkC ^= 0x100;
}

s32 func_15178750(s32 arg0, Func15178750Arg1 *arg1, s16 arg2) {
    Func15178750Inner *temp;

    temp = arg1->unk14;
    if (temp->unk36 & (1 << arg2)) {
        return func_15168118();
    }
    return arg0;
}

static void func_15178750_pad(void) {
}

typedef struct Func151787ACObj {
    u8 pad0[0x10];
    union {
        f32 *pos;
        struct {
            s16 x;
            s16 y;
        } s;
    } unk10;
    s16 unk14;
    s16 pad16;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[4];
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    u16 unk2E;
    s16 unk30;
    s16 unk32;
    u8 pad34[2];
    u8 unk36;
    u8 unk37;
} Func151787ACObj;

extern f32 D_800D9C10[][4][4];
extern s32 func_150A6360(void *arg0, f32 arg1[4][4], f32 arg2, f32 arg3, f32 arg4,
                         f32 arg5, f32 arg6, f32 arg7);
u16 func_10010F88_ret(s32 arg0, u16 arg1, s16 arg2, u8 arg3, s32 arg4, s16 arg5, s16 arg6, s16 arg7, s16 arg8, s16 arg9);

void func_151787AC(Func151787ACObj *arg0, s32 arg1, s32 arg2, s32 arg3) {
    f32 tx;
    f32 ty;
    f32 tz;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    s32 i;
    s32 mask;
    s32 px;
    s32 py;
    s32 pz;
    s32 ix;
    s32 iy;
    s32 iz;
    s32 d2;
    s16 tzs;
    f32 fx;
    f32 fy;
    f32 fz;

    tzs = arg0->unk14;
    arg1 = arg0->unk18;
    arg2 = arg0->unk1C;
    arg3 = arg0->unk20;
    arg0->unk28 = arg1;
    arg0->unk2A = arg2;
    arg0->unk2C = arg3;
    if (tzs == -0x8000) {
        f32 *p = arg0->unk10.pos;
        tx = p[0];
        ty = p[1];
        tz = p[2];
    } else {
        tx = arg0->unk10.s.x;
        ty = arg0->unk10.s.y;
        tz = tzs;
    }
    fx = arg1;
    fy = arg2;
    fz = arg3;
    dx = tx - fx;
    dy = ty - fy;
    dz = tz - fz;
    dist = sqrtf(dx * dx + dy * dy + dz * dz);
    if (dist > 1.0f) {
        dist = 16.0f / dist;
        arg0->unk18 = arg1 + dx * dist;
        arg0->unk1C = arg2 + dy * dist;
        arg0->unk20 = arg3 + dz * dist;
    }
    {
        s32 pad;
    mask = 0;
    for (i = 0; i <= D_80082FA0; i++) {
        if (func_150A6360((void *)((i * 0x180) + D_800BE628), D_800D9C10[i],
                          arg0->unk18, arg0->unk1C, arg0->unk20,
                          100.0f, 100.0f, 2500.0f) != 0) {
            mask |= 1 << i;
        }
    }
    }
    arg0->unk36 = mask;
    if (arg0->unk37 != 0) {
        if (arg0->unk2E == 0) {
            arg0->unk2E = func_10010F88_ret(0x4A2, 10000, 0, 0, 0, arg0->unk18, arg0->unk1C, arg0->unk20, 100, 800);
        } else {
            func_1000F91C(arg0->unk2E, 10000, 0, 0, 0, arg0->unk18, arg0->unk1C, arg0->unk20, 100, 800);
        }
    }
    if (arg0->unk14 == -0x8000) {
        f32 *p = arg0->unk10.pos;
        px = p[0];
        py = p[1];
        pz = p[2];
    } else {
        px = arg0->unk10.s.x;
        py = arg0->unk10.s.y;
        pz = arg0->unk14;
    }
    py += arg0->unk30;
    ix = px - arg0->unk18;
    iy = py - arg0->unk1C;
    iz = pz - arg0->unk20;
    d2 = ix * ix + iy * iy + iz * iz;
    if (d2 < 0 || d2 == 0) {}
    if (d2 > 10000) {
        if (arg0->unk32 != 0x640) {
            arg0->unk32 += D_800BE9E4 + 0x28;
            if (arg0->unk32 > 0x640) {
                arg0->unk32 = 0x640;
            }
        }
    } else {
        if (arg0->unk32 != 0x320) {
            arg0->unk32 -= D_800BE9E4 + 0x28;
            if (arg0->unk32 < 0x320) {
                arg0->unk32 = 0x320;
            }
        }
    }
}

typedef struct Node15178B98 {
    char pad0[0x8];
    struct Node15178B98 *unk8;
    char padC[0x4];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    char pad24[0xC];
    s16 unk30;
    char pad32[0x2];
    u8 unk34;
} Node15178B98;

extern Node15178B98 *D_800DCF38;

Node15178B98 *func_15178B98(u8 arg0) {
    Node15178B98 *node;

    for (node = D_800DCF38; node != NULL; node = node->unk8) {
        if (node->unk34 == arg0) {
            return node;
        }
    }
    return NULL;
}

Node15178B98 *func_15178BE4(u8 arg0, s32 arg1, s16 arg2) {
    Node15178B98 *node;

    node = func_15178B98(arg0);
    if (node != NULL) {
        node->unk10 = arg1;
        node->unk14 = 0x80000000;
        node->unk30 = arg2;
    }
}

Node15178B98 *func_15178C34(u8 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4) {
    Node15178B98 *node;

    node = func_15178B98(arg0);
    if (node != NULL) {
        node->unk10 = (arg1 << 16) | (arg2 & 0xFFFF);
        node->unk14 = arg3 << 16;
        node->unk30 = arg4;
    }
}

extern f32 sqrtf(f32);

s32 func_15178C9C(u8 arg0, s32 arg1) {
    Node15178B98 *node;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 sum;

    node = func_15178B98(arg0);
    if (node != NULL) {
        dx = (s32)(D_800CC2D0[arg1].x_position - (f32)node->unk18);
        dy = (s32)(D_800CC2D0[arg1].y_position - (f32)node->unk1C);
        dz = (s32)(D_800CC2D0[arg1].z_position - (f32)node->unk20);
        dx = dx * dx;
        dy = dy * dy;
        dz = dz * dz;
        sum = dx + dy + dz;
        return (s32)sqrtf((f32)sum);
    }
    return 1;
}

typedef struct Node15178DA4 {
    char pad0[0x8];
    struct Node15178DA4 *unk8;
    char padC[0x8];
    struct Node15178DA4 *unk14;
    char pad18[0x16];
    u16 unk2E;
} Node15178DA4;

extern Node15178DA4 *D_800DCF3C;

void func_15178DA4(Node15178DA4 *arg0) {
    Node15178DA4 *next;
    Node15178DA4 *node;

    node = D_800DCF3C;
    func_100111C8(arg0->unk2E);
    if (node != NULL) {
        do {
            next = node->unk8;
            if (arg0 == node->unk14) {
                func_1516972C((struct102 *)node);
            }
            node = next;
        } while (next != NULL);
    }
    func_15169824((struct102 *)arg0);
}

void func_15178E14(u8 arg0) {
    func_15178DA4(func_15178B98(arg0));
}
