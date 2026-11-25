#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Behaviour scratch state of the props in this file (WorldObject.work). */
typedef union {
    struct {
        s32 started;   /* set after the first update */
        s32 triggered; /* 1: move away from home, 0: return home */
        f32 home;      /* rotation or coordinate saved on the first update */
    } slide;
    struct {
        s32 started;
        s32 unk80;
        f32 phase; /* radians */
    } wobble;
} PropWork;

#define WORK(prop) ((PropWork *)&(prop)->work)

/* Motion states kept in bits 0-1 of WorldObject.behaviorState. */
#define PROP_REST 0
#define PROP_RETURNING 1
#define PROP_MOVING 2
#define PROP_ARRIVED 3

#define PROP_SOUND_OPEN 0
#define PROP_SOUND_CLOSE 1

extern void func_15114D24(WorldObject *prop, s32 sound, s32, s32, s32, s32);

extern f32 sinf(f32);

/*
 * Swings a hinged prop (rotY or rotZ, by variant) between its home rotation and a limit while
 * triggered, playing the row's open/close sounds; variants 10-13 rock rotZ on a sine instead.
 */
void func_1511E780(WorldObject *prop) {
    f32 speed;
    s32 soundSlot;
    s32 sound;
    s32 motion;
    f32 range;
    f32 rate;

    motion = prop->behaviorState & 3;
    soundSlot = -1;
    if (WORK(prop)->slide.triggered == 1) {
        if (motion == PROP_REST || motion == PROP_RETURNING) {
            motion = PROP_MOVING;
            soundSlot = PROP_SOUND_OPEN;
        }
        speed = 1.0f;
    } else if (WORK(prop)->slide.triggered == 0) {
        if (motion == PROP_ARRIVED || motion == PROP_MOVING) {
            motion = PROP_RETURNING;
            soundSlot = PROP_SOUND_CLOSE;
        }
        speed = -1.0f;
    } else {
        speed = 90.0f;
    }
    if (soundSlot != -1) {
        sound = prop->param.word & 0xFFFF; // matching: the row index must share sound's register
        sound = (&D_80089260[sound].open)[soundSlot];
        if (sound > 0) {
            func_15114D24(prop, sound, 24000, 200, 2500, 0);
        }
    }
    speed *= D_800BE9E4;
    range = 70.0f;
    rate = 6.0f;
    switch (prop->param.word >> 16) {
    case 6:
        rate = 3.0f;
        range = 82.0f;
        break;
    case 2:
    case 4:
        range = 80.0f;
        break;
    }
    switch (prop->param.word >> 16) {
    case 0:
        if (WORK(prop)->slide.started == 0) {
            WORK(prop)->slide.home = prop->rotZ;
        }
        prop->rotZ -= speed * rate;
        if (WORK(prop)->slide.home < prop->rotZ) {
            prop->rotZ = WORK(prop)->slide.home;
            motion = PROP_REST;
        }
        if (prop->rotZ < -110.0f) {
            prop->rotZ = -110.0f;
            motion = PROP_ARRIVED;
        }
        break;
    case 3:
        if (WORK(prop)->slide.started == 0) {
            WORK(prop)->slide.home = prop->rotY;
        }
        prop->rotY -= speed * 3.0f;
        if (prop->rotY < 260.0f) {
            prop->rotY = 260.0f;
            motion = PROP_ARRIVED;
        }
        if (WORK(prop)->slide.home < prop->rotY) {
            prop->rotY = WORK(prop)->slide.home;
            motion = PROP_REST;
        }
        break;
    case 2:
    case 8:
        if (WORK(prop)->slide.started == 0) {
            WORK(prop)->slide.home = prop->rotY;
        }
        prop->rotY += speed * rate;
        if (WORK(prop)->slide.home + range < prop->rotY) {
            prop->rotY = WORK(prop)->slide.home + range;
            motion = PROP_ARRIVED;
        }
        if (prop->rotY < WORK(prop)->slide.home) {
            prop->rotY = WORK(prop)->slide.home;
            motion = PROP_REST;
        }
        break;
    case 4:
    case 6:
    case 7:
        if (WORK(prop)->slide.started == 0) {
            WORK(prop)->slide.home = prop->rotY;
        }
        prop->rotY -= speed * rate;
        if (WORK(prop)->slide.home < prop->rotY) {
            prop->rotY = WORK(prop)->slide.home;
            motion = PROP_REST;
        }
        if (prop->rotY < WORK(prop)->slide.home - range) {
            prop->rotY = WORK(prop)->slide.home - range;
            motion = PROP_ARRIVED;
        }
        break;
    case 10:
        WORK(prop)->wobble.phase += 0.00999999977648f;
        /* fallthrough */
    case 11:
        WORK(prop)->wobble.phase += 0.0399999991059f * D_800BE9E4;
        prop->rotZ = sinf(WORK(prop)->wobble.phase) * 10.0f;
        if (WORK(prop)->wobble.phase > 12.5663709641f) { /* 4 pi */
            // matching: the multiplier converts as unsigned
            WORK(prop)->wobble.phase -= 12.5663709641f * (u32)((func_150ADA20() & 4) + 1);
            func_15114D24(prop, 0x503, 8000, 150, 600, 0);
        }
        break;
    case 12:
        WORK(prop)->wobble.phase += 0.00999999977648f;
        /* fallthrough */
    case 13:
        WORK(prop)->wobble.phase += 0.0399999991059f * D_800BE9E4;
        // matching: computed in double precision (180.0, not 180.0f)
        prop->rotZ = 180.0 - sinf(WORK(prop)->wobble.phase) * 10.0f;
        break;
    }
    WORK(prop)->slide.started = TRUE;
    prop->behaviorState &= ~3;
    prop->behaviorState |= motion;
}

/*
 * Raises the prop up to 500 units above home while triggered (then flags it finished) and lowers
 * it back otherwise; a nonzero params word makes it move at a quarter of the speed.
 */
void func_1511EC50(WorldObject *prop) {
    f32 step;
    f32 limit;

    if (WORK(prop)->slide.started == 0) {
        WORK(prop)->slide.home = prop->y;
    }

    step = D_800BE9E4 * 4.0f;
    if (prop->param.word != 0) {
        step *= 0.25f;
    }

    if (WORK(prop)->slide.triggered != 0) {
        prop->y += step * 5.0f;
        limit = WORK(prop)->slide.home + 500.0f;
        if (limit < prop->y) {
            prop->y = limit;
            prop->inactive = TRUE;
        }
    } else {
        prop->y -= step;
        if (prop->y < WORK(prop)->slide.home) {
            prop->y = WORK(prop)->slide.home;
        }
    }
    WORK(prop)->slide.started = TRUE;
}

/*
 * Slides the prop along z: away from home by up to 350 units while not triggered and back to home
 * once triggered. Bit 0 of the prop's id picks the direction.
 */
void func_1511ED84(WorldObject *prop) {
    f32 step;
    f32 limit;
    s32 direction;

    if (WORK(prop)->slide.started == 0) {
        WORK(prop)->slide.home = prop->z;
    }

    step = D_800BE9E4 * 4.0f;
    direction = 1 - (prop->id & 1);

    if (WORK(prop)->slide.triggered != 0) {
        if (direction & 1) {
            prop->z += step;
            if (WORK(prop)->slide.home < prop->z) {
                prop->z = WORK(prop)->slide.home;
            }
        } else {
            prop->z -= step;
            if (prop->z < WORK(prop)->slide.home) {
                prop->z = WORK(prop)->slide.home;
            }
        }
    } else if (direction & 1) {
        prop->z -= step;
        limit = WORK(prop)->slide.home - 350.0f;
        if (prop->z < limit) {
            prop->z = limit;
        }
    } else {
        prop->z += step;
        limit = WORK(prop)->slide.home + 350.0f;
        if (limit < prop->z) {
            prop->z = limit;
        }
    }
    WORK(prop)->slide.started = TRUE;
}
