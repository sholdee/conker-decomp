#include <ultra64.h>
#include "structs.h"

// This file's .bss (0x800D98C0-0x800D98DF). D_800D98D0 has its extern in conker/prop.h, so it is
// defined here, before functions.h (see init_50A0.c).
u8 D_800D98C0[16]; /* not referenced */
EffectHeader *D_800D98D0[4];

#include "functions.h"
#include "variables.h"

/*
 * Behaviour scratch state (WorldObject.touch) of the two behaviours here: the pendulum's push phase
 * (func_150C4D20) and the texture scroll state (func_150C4E9C).
 */
typedef union {
    struct {
        /* 0x7C */ s32 phase; // drives the push of func_15048A40; plays sound 0xF each time it wraps 256
    } pendulum;
    struct {
        /* 0x7C */ struct ScrollState *state; // allocated on first use
    } scroll;
} WorkF21D0;

#define WORK(obj) ((WorkF21D0 *)&(obj)->touch)

/*
 * Swings the object like a pendulum on rotX (degrees, spinX its velocity): a spring pulls it back toward
 * angle 0, a periodic push from func_15048A40(phase) (plus a constant one while flags bit 2 is set) drives
 * it, and 20% damping is applied every frame.
 */
void func_150C4D20(WorldObject *pendulum) {
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
    phase += D_800BE9E4;
    velocity -= angle * 0.009f;
    if (phase >= 256.0f) {
        phase -= 256.0f;
        func_10010F88(0xF, 0x55F0, 0, 0, 0, pendulum->x, pendulum->y, pendulum->z, 500, 1000);
    }
    WORK(pendulum)->pendulum.phase = phase;
    push = func_15048A40(phase);
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

// texture indices 0x2C1 and 0x1C0, linked as absolute symbols
extern u8 D_2C1;
extern u8 D_1C0;

/* Per-object state of func_150C4E9C, allocated on first use. */
typedef struct ScrollState {
    /* 0x00 */ s32 secondSetTImg; // index of the dl's second G_SETTIMG (0 if none), from func_1511A410
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
void func_150C4E9C(WorldObject *obj) {
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
        func_1511A410(obj->displayList, &scroll->secondSetTImg);
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
            scroll->speed += 25.0f * D_800BE9E4;
        } else {
            scroll->speed = 300.0f;
        }
    } else if (mode == TASK_FLAG_GOING_OFF) {
        s32 t;
        s32 startT;

        if (scroll->speed > 100.0f) {
            scroll->speed -= 25.0f * D_800BE9E4;
        } else {
            scroll->speed = 100.0f;
            t = obj->vertices[D_800BE9C0]->v.tc[1] % 1024;
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
            obj->vertices[D_800BE9C0][i].v.tc[1] -= scroll->speed;
        }
    }
    if (obj->vertices[D_800BE9C0]->v.tc[1] < -0x2800) {
        for (i = 0; i < obj->numVertices; i++) {
            obj->vertices[D_800BE9C0][i].v.tc[1] += 0x2800;
        }
    }
    tailAddr = 0;
    addr = texture_getAddress(texture, &size, 3, 0);
    if (scroll->secondSetTImg != 0) {
        tailAddr = size + addr - 0x200;
    }
    texture_addBinding(obj, addr, tailAddr, 4, 5);
}

// the four objects func_1500F40C spawns with func_1500F290

/* Destroys the four objects in D_800D98D0 (effect_destroyObject) and clears their slots. */
void func_150C522C(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_800D98D0[i] != NULL) {
            effect_destroyObject(D_800D98D0[i]);
        }
        D_800D98D0[i] = NULL;
    }
}


/* Returns whether D_800C35EA is 1 and D_800C35E8[0] is 0xB, 0xC or 0xD. */
s32 func_150C5280(void) {
    if (D_800C35EA[0] == 1 && (D_800C35E8[0] == 0xB || D_800C35E8[0] == 0xC || D_800C35E8[0] == 0xD)) {
        return TRUE;
    }
    return FALSE;
}

/* Update callback (D_8008ADA8[4]): runs D_8008ADA8[0] unless func_150C5280 holds. */
s32 func_150C52CC(void *fx) {
    if (func_150C5280()) {
        return FALSE;
    }
    return D_8008ADA8[0](fx);
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
