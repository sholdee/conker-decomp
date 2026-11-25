#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_15048A40
#include "variables.h"


extern f32 func_15048A40(u8 arg0);

/* An object func_150C4D20 swings like a pendulum about angle 0. */
typedef struct {
    /* 0x00 */ f32 angle; // degrees, wrapped into [0, 360)
    /* 0x04 */ u8  pad4[0xC];
    /* 0x10 */ s16 position[3];
    /* 0x16 */ u8  pad16[0x39];
    /* 0x4F */ u8  flags;
    /* 0x50 */ u8  pad50[0x10];
    /* 0x60 */ f32 angularVelocity; // degrees per frame
    /* 0x64 */ u8  pad64[0x18];
    /* 0x7C */ s32 phase; // drives the push of func_15048A40; plays sound 0xF each time it wraps 256
} Pendulum;

/* Swings the pendulum: a spring pulls it back toward angle 0, a periodic push from func_15048A40(phase)
 * (plus a constant one while flags bit 2 is set) drives it, and 20% damping is applied every frame. */
void func_150C4D20(Pendulum *pendulum) {
    f32 velocity;
    f32 angle;
    f32 push;
    s32 phase;

    angle = pendulum->angle;
    velocity = pendulum->angularVelocity;
    if (angle > 180.0f) {
        angle -= 360.0f;
    }
    phase = pendulum->phase;
    phase += D_800BE9E4;
    velocity -= angle * 0.009f;
    if (phase >= 256.0f) {
        phase -= 256.0f;
        func_10010F88(0xF, 0x55F0, 0, 0, 0, pendulum->position[0], pendulum->position[1], pendulum->position[2],
                      500, 1000);
    }
    pendulum->phase = phase;
    push = func_15048A40(phase);
    velocity += push * 0.1f;
    if ((pendulum->flags & 4) == 4) {
        velocity += 0.1f;
    }
    velocity *= 0.8f;
    pendulum->angularVelocity = velocity;
    pendulum->angle += velocity;
    if (pendulum->angle < 0.0f) {
        pendulum->angle += 360.0f;
    } else if (pendulum->angle >= 360.0f) {
        pendulum->angle -= 360.0f;
    }
}

extern void *allocate_memory(s32, s32, s32, s32);
extern s32 func_1511A410(Gfx *dl, s32 *second);
extern s32 func_1510D0EC(s32 texture, s32 *sizeOut, s32 lifetime, s32 countUse);
extern void func_1510D874(void *owner, s32 segment4, s32 segment5, s32 segment4Id, s32 segment5Id);
// texture indices 0x2C1 and 0x1C0, linked as absolute symbols
extern u8 D_2C1;
extern u8 D_1C0;

/* Per-object state of func_150C4E9C, allocated on first use. */
typedef struct {
    /* 0x00 */ s32 secondSetTImg; // index of the dl's second G_SETTIMG (0 if none), from func_1511A410
    /* 0x04 */ s16 startT;        // vertex 0's initial t texture coordinate
    /* 0x06 */ u8  pad6[2];
    /* 0x08 */ f32 speed;         // t units scrolled per frame
    /* 0x0C */ f32 unkC;
} TexScroll;

typedef struct {
    /* 0x00 */ u8  pad0[0x16];
    /* 0x16 */ u16 vertexCount;
    /* 0x18 */ u8  pad18[4];
    /* 0x1C */ Gfx *dl;
    /* 0x20 */ Vtx *vertices[2]; // double-buffered, indexed by D_800BE9C0
    /* 0x28 */ Vtx *unk28;
    /* 0x2C */ u8  pad2C[0x47];
    /* 0x73 */ u8  state; // low two bits: scroll mode
    /* 0x74 */ u8  pad74[8];
    /* 0x7C */ TexScroll *scroll;
} TexScrollObject;

/*
 * Scrolls the t texture coordinate of the object's vertices. Scroll mode (state & 3): 0 and 3 stand still,
 * 2 speeds up to 300 per frame, 1 slows to 100 and then stops (switching to mode 3) once the scroll lines up
 * with the start position again. Binds texture 0x2C1 (modes 0, 2) or 0x1C0 (modes 1, 3) to segment 4 and,
 * when the dl has a second G_SETTIMG, the texture's last 0x200 bytes to segment 5.
 */
void func_150C4E9C(TexScrollObject *obj) {
    s32 texture;
    s32 size;
    s32 addr;
    s32 tailAddr;
    TexScroll *scroll;
    s32 mode;
    s32 i;

    mode = obj->state & 3;
    if (obj->scroll == NULL) {
        scroll = obj->scroll = allocate_memory(sizeof(TexScroll), 1, 0, 0);
        func_1511A410(obj->dl, &scroll->secondSetTImg);
        scroll->startT = obj->unk28->v.tc[1];
        scroll->speed = 0.0f;
        scroll->unkC = 0.0f;
    } else {
        scroll = obj->scroll;
    }
    // matching: the texture index is loaded as a symbol address (lui/addiu), not as a literal
    if (mode == 0 || mode == 2) {
        texture = (s32)&D_2C1;
    } else {
        texture = (s32)&D_1C0;
    }
    if (mode == 0 || mode == 3) {
        scroll->speed = 0.0f;
        scroll->unkC = 0.0f;
    } else if (mode == 2) {
        if (scroll->speed < 300.0f) {
            scroll->speed += 25.0f * D_800BE9E4;
        } else {
            scroll->speed = 300.0f;
        }
    } else if (mode == 1) {
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
                obj->state &= ~3;
                obj->state |= 3; // mode 3: stopped
            }
        }
    }
    if (scroll->speed) {
        for (i = 0; i < obj->vertexCount; i++) {
            obj->vertices[D_800BE9C0][i].v.tc[1] -= scroll->speed;
        }
    }
    if (obj->vertices[D_800BE9C0]->v.tc[1] < -0x2800) {
        for (i = 0; i < obj->vertexCount; i++) {
            obj->vertices[D_800BE9C0][i].v.tc[1] += 0x2800;
        }
    }
    tailAddr = 0;
    addr = func_1510D0EC(texture, &size, 3, 0);
    if (scroll->secondSetTImg != 0) {
        tailAddr = size + addr - 0x200;
    }
    func_1510D874(obj, addr, tailAddr, 4, 5);
}

extern s32 *D_800D98D0[];

void func_150C522C(void) {
    s32 i;
    s32 v;

    i = 0;
    do {
        v = (s32)D_800D98D0[i];
        if (v != 0) {
            func_1516972C((struct102 *)v);
        }
        i++;
        D_800D98D0[i - 1] = 0;
    } while ((s32 **)&D_800D98E0 != &D_800D98D0[i]);
}

extern u8 D_800C35E8;

s32 func_150C5280(void) {
    u8 v0;
    if (D_800C35EA == 1) {
        v0 = D_800C35E8;
        if (v0 == 0xB || v0 == 0xC || v0 == 0xD) {
            return 1;
        }
    }
    return 0;
}

extern s32 func_150C5280(void);
extern s32 (*D_8008ADA8)(s32);

s32 func_150C52CC(s32 arg0) {
    if (func_150C5280()) {
        return 0;
    }
    return D_8008ADA8(arg0);
}

s32 func_150C5310(s32 *arg0) {
    if (func_150C5280()) {
        arg0[0x18] |= 0x20000;
    } else {
        arg0[0x18] &= ~0x20000;
    }
    return 1;
}
