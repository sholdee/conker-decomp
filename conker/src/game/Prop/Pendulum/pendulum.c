#include <ultra64.h>
#include "structs.h"

// This file's .bss (0x800D98C0-0x800D98DF). gLevelSprayEffects has its extern in conker/prop.h, so it is
// defined here, before functions.h (see init/Sched/SchedInit/schedinit.c).
u8 D_800D98C0[16]; /* not referenced */
EffectHeader *gLevelSprayEffects[4];

#include "functions.h"
#include "variables.h"

/*
 * Behaviour scratch state (WorldObject.touch) of the two behaviours here: the pendulum's push phase
 * (prop_swingPendulum) and the texture scroll state (prop_rampTextureScroll).
 */
typedef union {
    struct {
        /* 0x7C */ s32 phase; // drives the push of math_sinBinAngle; plays sound 0xF each time it wraps 256
    } pendulum;
    struct {
        /* 0x7C */ struct ScrollState *state; // allocated on first use
    } scroll;
} WorkF21D0;

#define WORK(obj) ((WorkF21D0 *)&(obj)->touch)

/*
 * Swings the object like a pendulum on rotX (degrees, spinX its velocity): a spring pulls it back toward
 * angle 0, a periodic push from math_sinBinAngle(phase) (plus a constant one while flags bit 2 is set) drives
 * it, and 20% damping is applied every frame.
 */
void prop_swingPendulum(WorldObject *pendulum) {
    f32 velocity;
    f32 angle;
    f32 push;
    s32 phase;

    angle = pendulum->t;
    velocity = pendulum->spinX;
    if (angle > 180.0f) {
        angle -= 360.0f;
    }
    phase = WORK(pendulum)->pendulum.phase;
    phase += gFrameTicks;
    velocity -= angle * 0.009f;
    if (phase >= 256.0f) {
        phase -= 256.0f;
        sound_playAt(0xF, 0x55F0, 0, 0, 0, pendulum->x, pendulum->y, pendulum->z, 500, 1000);
    }
    WORK(pendulum)->pendulum.phase = phase;
    push = math_sinBinAngle(phase);
    velocity += push * 0.1f;
    if ((pendulum->object_status & 4) == 4) {
        velocity += 0.1f;
    }
    velocity *= 0.8f;
    pendulum->spinX = velocity;
    pendulum->t += velocity;
    if (pendulum->t < 0.0f) {
        pendulum->t += 360.0f;
    } else if (pendulum->t >= 360.0f) {
        pendulum->t -= 360.0f;
    }
}

/* Per-object state of prop_rampTextureScroll, allocated on first use. */
typedef struct ScrollState {
    /* 0x00 */ s32 secondSetTImg; // index of the dl's second G_SETTIMG (0 if none), from prop_findSetTimg
    /* 0x04 */ s16 startT;        // vertex 0's initial t texture coordinate
    /* 0x06 */ u8  pad6[2];
    /* 0x08 */ f32 speed;         // t units scrolled per frame
    /* 0x0C */ f32 unkC;
} ScrollState;

/*
 * Scrolls the t texture coordinate of the object's vertices. Scroll mode (task_flag & 3): 0 and 3 stand still,
 * 2 speeds up to 300 per frame, 1 slows to 100 and then stops (switching to mode 3) once the scroll lines up
 * with the start position again. Binds texture 0x2C1 (modes 0, 2) or 0x1C0 (modes 1, 3) to segment 4 and,
 * when the dl has a second G_SETTIMG, the texture's last 0x200 bytes to segment 5.
 */
void prop_rampTextureScroll(WorldObject *obj) {
    s32 texture;
    s32 size;
    s32 addr;
    s32 tailAddr;
    ScrollState *scroll;
    s32 mode;
    s32 i;

    mode = obj->task_flag & TASK_FLAG_STATE;
    if (WORK(obj)->scroll.state == NULL) {
        scroll = WORK(obj)->scroll.state = allocate_memory(sizeof(ScrollState), 1, 0, 0);
        prop_findSetTimg(obj->displayList, &scroll->secondSetTImg);
        scroll->startT = obj->srcVertices->v.tc[1];
        scroll->speed = 0.0f;
        scroll->unkC = 0.0f;
    } else {
        scroll = WORK(obj)->scroll.state;
    }
    // matching: the texture index is loaded as a symbol address (lui/addiu), not as a literal
    if (mode == TASK_FLAG_OFF || mode == TASK_FLAG_GOING_ON) {
        texture = (s32)&D_2C1;
    } else {
        texture = (s32)&D_1C0;
    }
    if (mode == TASK_FLAG_OFF || mode == TASK_FLAG_ON) {
        scroll->speed = 0.0f;
        scroll->unkC = 0.0f;
    } else if (mode == TASK_FLAG_GOING_ON) {
        if (scroll->speed < 300.0f) {
            scroll->speed += 25.0f * gFrameTicks;
        } else {
            scroll->speed = 300.0f;
        }
    } else if (mode == TASK_FLAG_GOING_OFF) {
        s32 t;
        s32 startT;

        if (scroll->speed > 100.0f) {
            scroll->speed -= 25.0f * gFrameTicks;
        } else {
            scroll->speed = 100.0f;
            t = obj->vertices[gDrawBufferIndex]->v.tc[1] % 1024;
            while (t < 0) {
                t += 1024;
            }
            startT = scroll->startT % 1024;
            while (startT < 0) {
                startT += 1024;
            }
            if (t >= startT && t - scroll->speed <= startT) {
                scroll->speed = t - startT;
                obj->task_flag &= ~TASK_FLAG_STATE;
                obj->task_flag |= TASK_FLAG_ON; // mode 3: stopped
            }
        }
    }
    if (scroll->speed) {
        for (i = 0; i < obj->numVertices; i++) {
            obj->vertices[gDrawBufferIndex][i].v.tc[1] -= scroll->speed;
        }
    }
    if (obj->vertices[gDrawBufferIndex]->v.tc[1] < -0x2800) {
        for (i = 0; i < obj->numVertices; i++) {
            obj->vertices[gDrawBufferIndex][i].v.tc[1] += 0x2800;
        }
    }
    tailAddr = 0;
    addr = texture_getAddress(texture, &size, 3, 0);
    if (scroll->secondSetTImg != 0) {
        tailAddr = size + addr - 0x200;
    }
    texture_addBinding(obj, addr, tailAddr, 4, 5);
}

// the four objects level_setup08 spawns with level_spawnFlatSprite

/* Destroys the four objects in gLevelSprayEffects (effect_destroyObject) and clears their slots. */
void prop_destroyLevelSprays(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (gLevelSprayEffects[i] != NULL) {
            effect_destroyObject(gLevelSprayEffects[i]);
        }
        gLevelSprayEffects[i] = NULL;
    }
}


/* Returns whether gCutsceneStates is 1 and gCutsceneIds[0] is 0xB, 0xC or 0xD. */
s32 func_150C5280(void) {
    if (gCutsceneStates[0] == 1 && (gCutsceneIds[0] == 0xB || gCutsceneIds[0] == 0xC || gCutsceneIds[0] == 0xD)) {
        return TRUE;
    }
    return FALSE;
}

/* Update callback (gModelEffectUpdate2FuncsTail[4]): runs gModelEffectUpdate2FuncsTail[0] unless func_150C5280 holds. */
s32 func_150C52CC(void *fx) {
    if (func_150C5280()) {
        return FALSE;
    }
    return gModelEffectUpdate2FuncsTail[0](fx);
}

/* Callback (table at 0x80089910): sets flags bit 0x20000 while func_150C5280 holds, clears it otherwise. */
s32 func_150C5310(Struct150C5310 *obj) {
    if (func_150C5280()) {
        obj->flags |= 0x20000;
    } else {
        obj->flags &= ~0x20000;
    }
    return TRUE;
}
