#include <ultra64.h>

#include "functions.h"
#include "variables.h"

typedef struct SwingMotion SwingMotion;

/*
 * A placed level prop: one 0xA0-byte entry of the D_800DBEF4 table. The words at 0x7C-0x87 are
 * scratch state whose meaning depends on the prop's behaviour function.
 */
typedef struct Prop {
    /* 0x00 */ f32 rotX; /* degrees */
    /* 0x04 */ f32 rotY;
    /* 0x08 */ f32 rotZ;
    /* 0x0C */ u8 pad0C[4];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ f32 yOffset;
    /* 0x1C */ u8 pad1C[0x10];
    /* 0x2C */ f32 scaleX;
    /* 0x30 */ f32 scaleY;
    /* 0x34 */ f32 scaleZ;
    /* 0x38 */ u8 pad38[4];
    /* 0x3C */ union {
        s32 word;
        struct {
            u16 hi;
            u16 lo;
        } half;
        u8 bytes[4];
    } params;
    /* 0x40 */ u8 pad40[0x14];
    /* 0x54 */ u16 kind;
    /* 0x56 */ u8 pad56[4];
    /* 0x5A */ s16 moveX; /* translation this frame */
    /* 0x5C */ s16 moveY;
    /* 0x5E */ s16 moveZ;
    /* 0x60 */ u8 pad60[0xE];
    /* 0x6E */ u8 finished; /* set to 1 when the behaviour has run its course */
    /* 0x6F */ u8 pad6F[3];
    /* 0x72 */ u8 id;
    /* 0x73 */ u8 state; /* bits 0-1: motion state; bit 2: reverses the requested motion */
    /* 0x74 */ u8 pad74[8];
    /* 0x7C */ union {
        s32 words[3];
        struct {
            SwingMotion *motion;
            s32 state;
            s32 player; /* index into D_800CC2D0 and D_800DBFF0 */
        } swing;
        struct {
            s32 started;
            s32 delay;
            s32 timer;
        } fade;
        struct {
            s32 home; /* (y << 16) | x, saved on the first update */
            s32 unk80;
            s32 extent; /* (height << 16) | width, each 0..13 */
        } door;
        struct {
            s32 phase;
            s32 state;
            s32 timer;
        } bob;
    } work;
    /* 0x88 */ u8 pad88[2];
    /* 0x8A */ u8 alpha;
    /* 0x8B */ u8 pad8B[0x15];
} Prop; /* size 0xA0 */

/* Motion states kept in bits 0-1 of Prop.state. */
#define PROP_REST 0
#define PROP_RETURNING 1
#define PROP_MOVING 2
#define PROP_ARRIVED 3

extern void *allocate_memory(s32, s32, s32, s32);
extern Prop *func_151149AC(u8 id); /* the prop whose id is `id`, or NULL */
extern void func_1511490C(f32 mtx[4][4], Prop *prop);
extern void func_150A7960(f32 mtx[4][4], f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

typedef struct {
    f32 x, y, z, radius, angle, dx, dz;
    s16 timer;
    u8 pad1E[6];
} Game11EF40Motion;

typedef struct {
    f32 x, y, z;
} Vec3f_1511EF40;

extern s32 func_15086CBC(s32, f32 *, f32 *, f32 *);
extern s32 func_15086D48(s32);
extern f32 D_800A3378;

void func_1511EF40(u8 *arg0) {
    u8 *player;
    f32 x, y, z, angle;
    Game11EF40Motion *motion;
    u8 *actor;
    f32 dx;

    if (*(Game11EF40Motion **)(arg0 + 0x7C) == NULL) {
        motion = *(Game11EF40Motion **)(arg0 + 0x7C) = (Game11EF40Motion *)allocate_memory(0x24, 1, 0, 0);
        bzero(motion, 0x24);
    } else {
        motion = *(Game11EF40Motion **)(arg0 + 0x7C);
    }
    if (motion->timer > 0) {
        motion->timer -= D_800BE9E4;
        if (motion->timer <= 0) {
            actor = (u8 *)D_800DBFF0 + *(s32 *)(arg0 + 0x84) * 0x9A0;
            func_151239CC((struct108 *)actor, 0xA);
        }
    }
    if (*(s32 *)(arg0 + 0x80) == 1) {
        player = (u8 *)D_800CC2D0 + *(s32 *)(arg0 + 0x84) * 0x32C;
        *(s16 *)(arg0 + 0x10) = (s32)*(f32 *)(player + 0x14);
        *(s16 *)(arg0 + 0x12) = (s16)*(f32 *)(player + 0x18) + 100;
        *(s16 *)(arg0 + 0x14) = (s32)*(f32 *)(player + 0x1C);
        func_15086CBC(func_15086D48(0x28), &x, &y, &z);
        motion->dx = (x - *(f32 *)(player + 0x14)) * 0.5f;
        dx = motion->dx;
        motion->dz = (z - *(f32 *)(player + 0x1C)) * 0.5f;
        angle = motion->dz;
        motion->x = *(f32 *)(player + 0x14) + dx;
        motion->y = *(f32 *)(player + 0x18) + 100.0f;
        motion->z = *(f32 *)(player + 0x1C) + angle;
        motion->radius = sqrtf(dx * dx + angle * angle);
        motion->angle = -90.0f;
        *(s32 *)(arg0 + 0x80) = 2;
        return;
    }
    if (*(s32 *)(arg0 + 0x80) == 2) {
        if (D_80082FA0 >= *(s32 *)(arg0 + 0x84)) {
            actor = (u8 *)D_800DBFF0 + *(s32 *)(arg0 + 0x84) * 0x9A0;
            if (*(s32 *)(actor + 0x2C) != 0x400000) {
                func_15123934((struct108 *)actor, 0x400000, 0, *(s32 *)(actor + 0x134), 0xA);
            }
            *(f32 *)(actor + 0x2BC) = *(s16 *)(arg0 + 0x10);
            *(f32 *)(actor + 0x2C0) = *(s16 *)(arg0 + 0x12);
            *(f32 *)(actor + 0x2C4) = *(s16 *)(arg0 + 0x14);
            *(Vec3f_1511EF40 *)(actor + 0x2A4) = *(Vec3f_1511EF40 *)(actor + 0x2BC);
            *(f32 *)(actor + 0x2F8) = 0.0f;
            *(f32 *)(actor + 0x300) = 0.0f;
            *(f32 *)(actor + 0x2FC) = 300.0f;
            motion->timer = 60;
        }
        angle = (motion->angle * D_800A3378) / 180.0f;
        *(s16 *)(arg0 + 0x12) = (s32)(cosf(angle) * motion->radius + motion->y);
        dx = sinf(angle);
        *(s16 *)(arg0 + 0x10) = (s32)(motion->x + motion->dx * dx);
        *(s16 *)(arg0 + 0x14) = (s32)(motion->z + motion->dz * dx);
        motion->angle += (f32)D_800BE9E4;
        if (motion->angle > 90.0f) {
            motion->angle = 90.0f;
            *(s32 *)(arg0 + 0x80) = 0;
            *(s8 *)(arg0 + 0x6E) = 1;
            if (D_80082FA0 >= *(s32 *)(arg0 + 0x84)) func_151239CC((struct108 *)actor, 0xA);
        }
    }
}

/* On level 0x35, places a kind-0x8005 prop at a fixed point in the frame of the prop it is linked to. */
void func_1511F31C(Prop *prop) {
    s32 pad[2]; // matching: unused stack slots of the original frame
    f32 mtx[4][4];
    f32 x;
    f32 y;
    f32 z;
    Prop *parent;
    u16 id;

    id = prop->params.half.lo;
    if (prop->kind == 0x8005 && D_800BE9F0 == 0x35) {
        parent = func_151149AC(id);
        if (parent != NULL) {
            func_1511490C(mtx, parent);
            func_150A7960(mtx, -3.0f, 223.0f, 549.0f, &x, &y, &z);
            prop->x = x;
            prop->y = y;
            prop->z = z;
        }
    }
}

struct Obj1511F3E8 {
    u8 pad0[0x3C];
    s32 unk3C;
    u8 pad40[0x2E];
    u8 unk6E;
    u8 pad6F[0xD];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    u8 pad88[2];
    u8 unk8A;
};

void func_1511F3E8(struct Obj1511F3E8 *arg0) {
    s32 ticks;

    if (arg0->unk7C == 0) {
        ticks = arg0->unk3C;
        arg0->unk80 = ((ticks >> 16) & 0xFFFF) * 60;
        arg0->unk7C = 1;
    }
    ticks = arg0->unk3C;
    ticks = (ticks & 0xFFFF) * 60;
    if (arg0->unk80 != 0) {
        if (D_800BE9E4 < arg0->unk80) {
            arg0->unk80 -= D_800BE9E4;
        } else {
            arg0->unk80 = 0;
            arg0->unk84 = ticks;
        }
    } else if (D_800BE9E4 < arg0->unk84) {
        arg0->unk84 -= D_800BE9E4;
        arg0->unk8A = (arg0->unk84 * 255) / ticks;
    } else {
        arg0->unk6E = 1;
    }
}


struct Obj1511F4D0 {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    u8 pad14[0x1C];
    f32 unk30;
    u8 pad34[0x8];
    s32 unk3C;
    u8 pad40[0x33];
    u8 unk73;
    u8 pad74[0x8];
    s32 unk7C;
    u8 pad80[0x4];
    s32 unk84;
    u8 pad88[0x2A];
    s16 unkB2;
    u8 padB4[0x9C];
    s16 unk150;
    u8 pad152[0x9E];
    s16 unk1F0;
};

void func_1511F4D0(struct Obj1511F4D0 *arg0) {
    s32 state;
    s32 width;
    s32 height;
    s32 unused;
    s32 scale;
    s32 tmp;
    s32 origin[4];

    state = arg0->unk73 & 3;
    width = arg0->unk73 & 4;
    if (width == 4) {
        if (state == 2) {
            state = 0;
        } else if (state == 1) {
            state = 3;
        }
    }
    scale = arg0->unk3C;
    scale = (s32)((f32)scale * arg0->unk30);
    tmp = arg0->unk84;
    height = (tmp >> 16) & 0xFFFF;
    width = tmp & 0xFFFF;
    if (arg0->unk7C == 0) {
        arg0->unk7C = (arg0->unk10 & 0xFFFF) | ((arg0->unk12 & 0xFFFF) << 16);
        if (state == 3) {
            width = 13;
            height = 13;
        } else {
            height = 0;
            width = 0;
        }
    }
    origin[1] = (s16)(arg0->unk7C & 0xFFFF);
    origin[2] = (s16)((arg0->unk7C >> 16) & 0xFFFF);
    if (state == 2) {
        tmp = width + D_800BE9E4;
        if (tmp < 13) {
            width = tmp;
        } else {
            width = 13;
        }
        if (width >= 9) {
            tmp = height + D_800BE9E4;
            if (tmp < 13) {
                height = tmp;
            } else {
                height = 13;
                state = 3;
            }
        }
    } else if (state == 1) {
        tmp = height - D_800BE9E4;
        if (tmp > 0) {
            height = tmp;
        } else {
            height = 0;
        }
        if (height < 4) {
            tmp = width - D_800BE9E4;
            if (tmp > 0) {
                width = tmp;
            } else {
                width = 0;
                state = 0;
            }
        }
    }
    arg0->unk12 = origin[2] + (s32)((u32)width * scale) / 13;
    arg0->unkB2 = origin[2] - (s32)((u32)(scale + 1) * width) / 13;
    tmp = (s32)((u32)height * scale) / 13;
    arg0->unk150 = origin[1] - tmp;
    arg0->unk1F0 = origin[1] + tmp;
    arg0->unk84 = (width & 0xFFFF) | ((height & 0xFFFF) << 16);
    arg0->unk73 &= ~3;
    arg0->unk73 |= state;
}

/* Starts the prop's behaviour: moves it from state 0 to state 1. */
void func_1511F768(Prop *prop, s32 arg1) {
    if (prop->work.words[1] == 0) {
        prop->work.words[1] = 1;
    }
}

struct Obj1511F788 {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[0x4];
    f32 unk18;
    u8 pad1C[0x20];
    s32 unk3C;
    u8 pad40[0x1C];
    s16 unk5C;
    u8 pad5E[0x10];
    s8 unk6E;
    u8 pad6F[0xD];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
};

union Temp1511F788 {
    struct {
        f32 pad;
        s32 w;
    } s;
    f64 d;
};

void func_1511F788(struct Obj1511F788 *arg0) {
    s32 dummy;
    union Temp1511F788 temp;
    s16 initial;
    s32 state;

    initial = (s16)(s32)((f32)arg0->unk12 + arg0->unk18);
    temp.s.w = arg0->unk3C;
    temp.s.w = (temp.s.w * 0x3C) / 0x3C;
    if (D_800BE9B4 != 0) {
        arg0->unk7C = func_150ADA20() & 0xFFFF;
    }

    arg0->unk18 = cosf((f32)arg0->unk7C * 0.0054931640625f) * 25.0f;
    state = arg0->unk80;
    arg0->unk7C = arg0->unk7C + (D_800BE9E4 * 10);

    if (state == 1) {
        arg0->unk84 = arg0->unk84 + D_800BE9E4;
        if (temp.s.w < arg0->unk84) {
            arg0->unk80 = 2;
        }
    } else if (state == 2) {
        arg0->unk12 = arg0->unk12 - D_800BE9E4;
        if (arg0->unk12 < -10000) {
            ((u8 *)arg0)[0x6E] = 1;
        }
    }

    arg0->unk5C = (s32)(((f32)arg0->unk12 + arg0->unk18) - (f32)initial);
}

/* Copies the position of the prop this one is linked to. */
void func_1511F92C(Prop *prop) {
    Prop *parent;

    parent = func_151149AC(prop->params.bytes[3]);
    if (parent != NULL) {
        prop->x = parent->x;
        prop->y = parent->y;
        prop->z = parent->z;
    }
}
