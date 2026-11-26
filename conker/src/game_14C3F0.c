#include <ultra64.h>

#include "functions.h"
#include "variables.h"

typedef struct SwingMotion SwingMotion;

/* Behaviour scratch state of the props in this file (WorldObject.touch). */
typedef union {
    s32 words[3];
    struct {
        SwingMotion *motion;
        s32 state;
        s32 character; /* index into movement_obj and camera_global_Cameras */
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
} Work14C3F0;

#define WORK(prop) ((Work14C3F0 *)&(prop)->touch)

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

/*
 * Carries character work.swing.character along a semicircular arc from where it stands (state 1)
 * to the position returned for object 0x28, then marks the prop finished. While it swings
 * (state 2) the character's actor is put in mode 0x400000 at the prop's position; it is released
 * 60 frames later.
 */
void func_1511EF40(WorldObject *prop) {
    Actor *character;
    f32 x, y, z, angle;
    SwingMotion *motion;
    Camera *actor;
    f32 dx;

    if (WORK(prop)->swing.motion == NULL) {
        motion = WORK(prop)->swing.motion = allocate_memory(sizeof(SwingMotion), 1, 0, 0);
        bzero(motion, sizeof(SwingMotion));
    } else {
        motion = WORK(prop)->swing.motion;
    }
    if (motion->timer > 0) {
        motion->timer -= D_800BE9E4;
        if (motion->timer <= 0) {
            actor = &camera_global_Cameras[WORK(prop)->swing.character];
            func_151239CC(actor, CAMERA_MODE_CUTSCENE);
        }
    }
    if (WORK(prop)->swing.state == 1) {
        character = &movement_obj[WORK(prop)->swing.character];
        prop->x = character->xpos;
        prop->y = (s16)character->ypos + 100;
        prop->z = character->zpos;
        func_15086CBC(func_15086D48(0x28), &x, &y, &z);
        motion->dx = (x - character->xpos) * 0.5f;
        dx = motion->dx;
        motion->dz = (z - character->zpos) * 0.5f;
        angle = motion->dz; // matching: angle doubles as dz; a separate local changes the frame
        motion->x = character->xpos + dx;
        motion->y = character->ypos + 100.0f;
        motion->z = character->zpos + angle;
        motion->radius = sqrtf(dx * dx + angle * angle);
        motion->angle = -90.0f;
        WORK(prop)->swing.state = 2;
        return;
    }
    if (WORK(prop)->swing.state == 2) {
        if (gScreenLayout >= WORK(prop)->swing.character) {
            actor = &camera_global_Cameras[WORK(prop)->swing.character];
            if (actor->Mode != 0x400000) {
                func_15123934(actor, 0x400000, 0, actor->Tightness, CAMERA_MODE_CUTSCENE);
            }
            actor->target.x = prop->x;
            actor->target.y = prop->y;
            actor->target.z = prop->z;
            actor->focus = actor->target;
            actor->eye.x = 0.0f;
            actor->eye.z = 0.0f;
            actor->eye.y = 300.0f;
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
            WORK(prop)->swing.state = 0;
            prop->inactive = TRUE;
            if (gScreenLayout >= WORK(prop)->swing.character) {
                func_151239CC(actor, CAMERA_MODE_CUTSCENE);
            }
        }
    }
}

/* On level 0x35, places a kind-0x8005 prop at a fixed point in the frame of the prop it is linked to. */
void func_1511F31C(WorldObject *prop) {
    s32 pad[2]; // matching: unused stack slots of the original frame
    MtxF mtx;
    f32 x;
    f32 y;
    f32 z;
    WorldObject *parent;
    u16 id;

    id = prop->moveData.uhalf.lo;
    if (prop->modelId == 0x8005 && level == 0x35) {
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
void func_1511F3E8(WorldObject *prop) {
    s32 fadeTime;

    if (!WORK(prop)->fade.started) {
        fadeTime = prop->moveData.word;
        WORK(prop)->fade.delay = ((fadeTime >> 16) & 0xFFFF) * 60;
        WORK(prop)->fade.started = TRUE;
    }
    fadeTime = (prop->moveData.word & 0xFFFF) * 60;
    if (WORK(prop)->fade.delay != 0) {
        if (D_800BE9E4 < WORK(prop)->fade.delay) {
            WORK(prop)->fade.delay -= D_800BE9E4;
        } else {
            WORK(prop)->fade.delay = 0;
            WORK(prop)->fade.timer = fadeTime;
        }
    } else if (D_800BE9E4 < WORK(prop)->fade.timer) {
        WORK(prop)->fade.timer -= D_800BE9E4;
        prop->alpha = (WORK(prop)->fade.timer * 255) / fadeTime;
    } else {
        prop->inactive = TRUE;
    }
}

/*
 * Opens and closes a group of four consecutive props around the first one's saved home position:
 * prop[0] and prop[1] part along y, then prop[2] and prop[3] part along x, each opening by up to
 * params * scaleY in 13 steps. MOVING opens the group (ending ARRIVED), RETURNING closes it again
 * (ending REST); state bit 2 swaps the two.
 */
void func_1511F4D0(WorldObject *prop) {
    s32 motion;
    s32 openY;
    s32 openX;
    s32 pad; // matching: unused stack slot of the original frame
    s32 span;
    s32 next;
    s32 home[4]; // matching: home x/y live in a stack array ([1] and [2]) in the original

    motion = prop->task_flag & TASK_FLAG_STATE;
    openY = prop->task_flag & TASK_FLAG_LOCKED; // matching: the flag test reuses openY's register
    if (openY == 4) {
        if (motion == TASK_FLAG_GOING_ON) {
            motion = TASK_FLAG_OFF;
        } else if (motion == TASK_FLAG_GOING_OFF) {
            motion = TASK_FLAG_ON;
        }
    }
    span = prop->moveData.word; // matching: loaded into span before the conversion
    span = span * prop->yScale;
    next = WORK(prop)->opening.open;
    openX = (next >> 16) & 0xFFFF;
    openY = next & 0xFFFF;
    if (WORK(prop)->opening.home == 0) {
        WORK(prop)->opening.home = (prop->x & 0xFFFF) | ((prop->y & 0xFFFF) << 16);
        if (motion == TASK_FLAG_ON) {
            openY = 13;
            openX = 13;
        } else {
            openX = 0;
            openY = 0;
        }
    }
    home[1] = (s16)(WORK(prop)->opening.home & 0xFFFF);
    home[2] = (s16)((WORK(prop)->opening.home >> 16) & 0xFFFF);
    if (motion == TASK_FLAG_GOING_ON) {
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
                motion = TASK_FLAG_ON;
            }
        }
    } else if (motion == TASK_FLAG_GOING_OFF) {
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
                motion = TASK_FLAG_OFF;
            }
        }
    }
    prop[0].y = home[2] + openY * span / 13;
    prop[1].y = home[2] - (span + 1) * openY / 13;
    next = openX * span / 13;
    prop[2].x = home[1] - next;
    prop[3].x = home[1] + next;
    WORK(prop)->opening.open = (openY & 0xFFFF) | ((openX & 0xFFFF) << 16);
    prop->task_flag &= ~TASK_FLAG_STATE;
    prop->task_flag |= motion;
}

/* Starts the prop's behaviour: moves it from state 0 to state 1. */
void func_1511F768(WorldObject *prop, s32 arg1) {
    if (WORK(prop)->words[1] == 0) {
        WORK(prop)->words[1] = 1;
    }
}

/*
 * Bobs the prop up and down by 25 units (moveY carries this frame's movement). Once started
 * (state 1) it waits params frames, then (state 2) sinks until it is far below the level and
 * marks itself finished.
 */
void func_1511F788(WorldObject *prop) {
    s32 pad[2]; // matching: pad and pad2 are unused stack slots of the original frame
    s32 waitTime;
    s32 pad2;
    s16 startY;
    s32 state;

    startY = prop->y + prop->yOffset;
    waitTime = prop->moveData.word; // matching: params goes through waitTime first
    waitTime = waitTime * 60 / 60;
    if (D_800BE9B4 != 0) {
        WORK(prop)->bob.phase = func_150ADA20() & 0xFFFF;
    }

    prop->yOffset = cosf(WORK(prop)->bob.phase * 0.0054931640625f) * 25.0f;
    state = WORK(prop)->bob.state;
    WORK(prop)->bob.phase += D_800BE9E4 * 10;

    if (state == 1) {
        WORK(prop)->bob.timer += D_800BE9E4;
        if (waitTime < WORK(prop)->bob.timer) {
            WORK(prop)->bob.state = 2;
        }
    } else if (state == 2) {
        prop->y -= D_800BE9E4;
        if (prop->y < -10000) {
            prop->inactive = TRUE;
        }
    }

    prop->moveY = (prop->y + prop->yOffset) - startY;
}

/* Copies the position of the prop this one is linked to. */
void func_1511F92C(WorldObject *prop) {
    WorldObject *parent;

    parent = func_151149AC(prop->moveData.bytes[3]);
    if (parent != NULL) {
        prop->x = parent->x;
        prop->y = parent->y;
        prop->z = parent->z;
    }
}
