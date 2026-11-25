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
            s32 character; /* index into D_800CC2D0 and D_800DBFF0 */
        } swing;
        struct {
            s32 started;
            s32 delay;
            s32 timer;
        } fade;
        struct {
            s32 home; /* (y << 16) | x, saved on the first update */
            s32 unk80;
            s32 open; /* (x opening << 16) | y opening, each 0..13 */
        } opening;
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

/* State of func_1511EF40's swing, allocated on its first update. */
struct SwingMotion {
    /* 0x00 */ f32 x; /* centre of the arc */
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 angle; /* degrees: -90 at the start point, 90 at the end point */
    /* 0x14 */ f32 dx; /* half the horizontal offset from the start to the end point */
    /* 0x18 */ f32 dz;
    /* 0x1C */ s16 timer;
    /* 0x1E */ u8 pad1E[6];
}; /* size 0x24 */

typedef struct {
    f32 x, y, z;
} FVec3;

extern s32 func_15086CBC(s32, f32 *, f32 *, f32 *);
extern s32 func_15086D48(s32);

/*
 * Carries character work.swing.character along a semicircular arc from where it stands (state 1)
 * to the position returned for object 0x28, then marks the prop finished. While it swings
 * (state 2) the character's actor is put in mode 0x400000 at the prop's position; it is released
 * 60 frames later.
 */
void func_1511EF40(Prop *prop) {
    struct127 *character;
    f32 x, y, z, angle;
    SwingMotion *motion;
    struct108 *actor;
    f32 dx;

    if (prop->work.swing.motion == NULL) {
        motion = prop->work.swing.motion = allocate_memory(sizeof(SwingMotion), 1, 0, 0);
        bzero(motion, sizeof(SwingMotion));
    } else {
        motion = prop->work.swing.motion;
    }
    if (motion->timer > 0) {
        motion->timer -= D_800BE9E4;
        if (motion->timer <= 0) {
            actor = &D_800DBFF0[prop->work.swing.character];
            func_151239CC(actor, 10);
        }
    }
    if (prop->work.swing.state == 1) {
        character = &D_800CC2D0[prop->work.swing.character];
        prop->x = character->x_position;
        prop->y = (s16)character->y_position + 100;
        prop->z = character->z_position;
        func_15086CBC(func_15086D48(0x28), &x, &y, &z);
        motion->dx = (x - character->x_position) * 0.5f;
        dx = motion->dx;
        motion->dz = (z - character->z_position) * 0.5f;
        angle = motion->dz; // matching: angle doubles as dz; a separate local changes the frame
        motion->x = character->x_position + dx;
        motion->y = character->y_position + 100.0f;
        motion->z = character->z_position + angle;
        motion->radius = sqrtf(dx * dx + angle * angle);
        motion->angle = -90.0f;
        prop->work.swing.state = 2;
        return;
    }
    if (prop->work.swing.state == 2) {
        if (D_80082FA0 >= prop->work.swing.character) {
            actor = &D_800DBFF0[prop->work.swing.character];
            if (actor->unk2C != 0x400000) {
                func_15123934(actor, 0x400000, 0, actor->unk134, 10);
            }
            actor->unk2BC = prop->x;
            actor->unk2C0 = prop->y;
            actor->unk2C4 = prop->z;
            *(FVec3 *)&actor->unk2A4 = *(FVec3 *)&actor->unk2BC;
            actor->unk2F8 = 0.0f;
            actor->unk300 = 0.0f;
            actor->unk2FC = 300.0f;
            motion->timer = 60;
        }
        angle = (motion->angle * 3.1415927f) / 180.0f;
        prop->y = cosf(angle) * motion->radius + motion->y;
        dx = sinf(angle); // matching: dx doubles as the sine, as above
        prop->x = motion->x + motion->dx * dx;
        prop->z = motion->z + motion->dz * dx;
        motion->angle += D_800BE9E4;
        if (motion->angle > 90.0f) {
            motion->angle = 90.0f;
            prop->work.swing.state = 0;
            prop->finished = TRUE;
            if (D_80082FA0 >= prop->work.swing.character) {
                func_151239CC(actor, 10);
            }
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

/*
 * Waits params.hi seconds, then fades the prop out over params.lo seconds (alpha 255 -> 0) and
 * marks it finished.
 */
void func_1511F3E8(Prop *prop) {
    s32 fadeTime;

    if (!prop->work.fade.started) {
        fadeTime = prop->params.word;
        prop->work.fade.delay = ((fadeTime >> 16) & 0xFFFF) * 60;
        prop->work.fade.started = TRUE;
    }
    fadeTime = (prop->params.word & 0xFFFF) * 60;
    if (prop->work.fade.delay != 0) {
        if (D_800BE9E4 < prop->work.fade.delay) {
            prop->work.fade.delay -= D_800BE9E4;
        } else {
            prop->work.fade.delay = 0;
            prop->work.fade.timer = fadeTime;
        }
    } else if (D_800BE9E4 < prop->work.fade.timer) {
        prop->work.fade.timer -= D_800BE9E4;
        prop->alpha = (prop->work.fade.timer * 255) / fadeTime;
    } else {
        prop->finished = TRUE;
    }
}

/*
 * Opens and closes a group of four consecutive props around the first one's saved home position:
 * prop[0] and prop[1] part along y, then prop[2] and prop[3] part along x, each opening by up to
 * params * scaleY in 13 steps. MOVING opens the group (ending ARRIVED), RETURNING closes it again
 * (ending REST); state bit 2 swaps the two.
 */
void func_1511F4D0(Prop *prop) {
    s32 motion;
    s32 openY;
    s32 openX;
    s32 pad; // matching: unused stack slot of the original frame
    s32 span;
    s32 next;
    s32 home[4]; // matching: home x/y live in a stack array ([1] and [2]) in the original

    motion = prop->state & 3;
    openY = prop->state & 4; // matching: the flag test reuses openY's register
    if (openY == 4) {
        if (motion == PROP_MOVING) {
            motion = PROP_REST;
        } else if (motion == PROP_RETURNING) {
            motion = PROP_ARRIVED;
        }
    }
    span = prop->params.word; // matching: loaded into span before the conversion
    span = span * prop->scaleY;
    next = prop->work.opening.open;
    openX = (next >> 16) & 0xFFFF;
    openY = next & 0xFFFF;
    if (prop->work.opening.home == 0) {
        prop->work.opening.home = (prop->x & 0xFFFF) | ((prop->y & 0xFFFF) << 16);
        if (motion == PROP_ARRIVED) {
            openY = 13;
            openX = 13;
        } else {
            openX = 0;
            openY = 0;
        }
    }
    home[1] = (s16)(prop->work.opening.home & 0xFFFF);
    home[2] = (s16)((prop->work.opening.home >> 16) & 0xFFFF);
    if (motion == PROP_MOVING) {
        next = openY + D_800BE9E4;
        if (next < 13) {
            openY = next;
        } else {
            openY = 13;
        }
        if (openY >= 9) {
            next = openX + D_800BE9E4;
            if (next < 13) {
                openX = next;
            } else {
                openX = 13;
                motion = PROP_ARRIVED;
            }
        }
    } else if (motion == PROP_RETURNING) {
        next = openX - D_800BE9E4;
        if (next > 0) {
            openX = next;
        } else {
            openX = 0;
        }
        if (openX < 4) {
            next = openY - D_800BE9E4;
            if (next > 0) {
                openY = next;
            } else {
                openY = 0;
                motion = PROP_REST;
            }
        }
    }
    prop[0].y = home[2] + openY * span / 13;
    prop[1].y = home[2] - (span + 1) * openY / 13;
    next = openX * span / 13;
    prop[2].x = home[1] - next;
    prop[3].x = home[1] + next;
    prop->work.opening.open = (openY & 0xFFFF) | ((openX & 0xFFFF) << 16);
    prop->state &= ~3;
    prop->state |= motion;
}

/* Starts the prop's behaviour: moves it from state 0 to state 1. */
void func_1511F768(Prop *prop, s32 arg1) {
    if (prop->work.words[1] == 0) {
        prop->work.words[1] = 1;
    }
}

/*
 * Bobs the prop up and down by 25 units (moveY carries this frame's movement). Once started
 * (state 1) it waits params frames, then (state 2) sinks until it is far below the level and
 * marks itself finished.
 */
void func_1511F788(Prop *prop) {
    s32 pad[2]; // matching: pad and pad2 are unused stack slots of the original frame
    s32 waitTime;
    s32 pad2;
    s16 startY;
    s32 state;

    startY = prop->y + prop->yOffset;
    waitTime = prop->params.word; // matching: params goes through waitTime first
    waitTime = waitTime * 60 / 60;
    if (D_800BE9B4 != 0) {
        prop->work.bob.phase = func_150ADA20() & 0xFFFF;
    }

    prop->yOffset = cosf(prop->work.bob.phase * 0.0054931640625f) * 25.0f;
    state = prop->work.bob.state;
    prop->work.bob.phase += D_800BE9E4 * 10;

    if (state == 1) {
        prop->work.bob.timer += D_800BE9E4;
        if (waitTime < prop->work.bob.timer) {
            prop->work.bob.state = 2;
        }
    } else if (state == 2) {
        prop->y -= D_800BE9E4;
        if (prop->y < -10000) {
            prop->finished = TRUE;
        }
    }

    prop->moveY = (prop->y + prop->yOffset) - startY;
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
