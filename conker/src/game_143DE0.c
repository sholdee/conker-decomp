#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Behaviour handlers for scenery objects that move, spin or animate their vertices toward a target.
 * They are listed in the behaviour tables in 22D710.rodata.
 */
typedef struct MoverObj {
    /* 0x00 */ f32 rot[3]; /* degrees */
    /* 0x0C */ u8 pad0C[4];
    /* 0x10 */ union {
        /* matching: func_15116EA4 reads x, y, z as scalars; through v[] IDO allocates its registers differently */
        s16 v[3];
        struct {
            s16 x, y, z;
        } s;
    } pos;
    /* 0x16 */ u8 pad16[0xA];
    /* 0x20 */ Vtx *vertices[2]; /* one per frame buffer */
    /* 0x28 */ u8 pad28[0x14];
    /* 0x3C */ s32 params;
    /* 0x40 */ u8 pad40[0xF];
    /* 0x4F */ u8 flags;
    /* 0x50 */ u8 pad50[0xA];
    /* 0x5A */ s16 move[3]; /* translation this frame */
    /* 0x60 */ f32 spin[3]; /* rotation speed, degrees per tick */
    /* 0x6C */ u8 pad6C[7];
    /* 0x73 */ u8 state;
    /* 0x74 */ u16 sound; /* handle from func_15114D24 */
    /* 0x76 */ u8 pad76[6];
    /* 0x7C */ union {
        s32 targetPos[3];
        f32 targetSpin[3];
        struct {
            s32 target; /* animation position to reach, 0..0xFFFF */
            u8 *frames; /* vertex frames loaded from ROM */
            s32 pos;
        } anim;
        struct {
            f32 damping;
            f32 acceleration;
            f32 velocity;
        } swing;
    } work;
} MoverObj;

/* One row of D_80089260, selected by params >> 10. */
typedef struct MoverSounds {
    /* 0x0 */ s16 start; /* played when the object starts moving */
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 stop; /* played when it stops */
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 arrive; /* played just before it reaches the target; selects func_15116BAC */
    /* 0xA */ s16 arriveLead; /* how early (in units of the move rate) to play `arrive` */
} MoverSounds;

/* Animation entry of D_80089320: which ROM file holds the vertex frames, and how many there are. */
typedef struct VertexAnim {
    u8 file;
    u8 frameCount;
} VertexAnim;

/* ROM file table entry. */
typedef struct RomFile {
    s32 start;
    s32 end;
} RomFile;

extern MoverSounds D_80089260[];
extern VertexAnim D_80089320[];
extern RomFile D_8002AAD0[];

extern void *allocate_memory(s32, s32, s32, s32);
extern void func_15114D24(MoverObj *obj, s32 sound, s32 arg2, s16 arg3, s16 arg4, s32 flags);
void func_151169B4(MoverObj *obj);
void func_15116BAC(MoverObj *obj);

/* Trigger handler: when an actor touches a resting triggerable object, start it moving. */
void func_15116930(MoverObj *obj, struct127 *actor) {
    if (obj->flags & 4) {
        if (!(obj->state & 3) && !(obj->state & 4)) {
            if (actor->unk31C->unk57 == 1) {
                obj->state &= ~3;
                obj->state |= 2;
            }
        }
    }
}

/* Moves the object only while it has been triggered. */
void func_15116984(MoverObj *obj) {
    if (obj->state & 2) {
        func_151169B4(obj);
    }
}

/* Steps pos toward work.targetPos at the rate in params, playing the start and stop sounds. */
void func_151169B4(MoverObj *obj) {
    s32 step;
    s32 sound;
    s16 *pos;
    s16 *move;
    s32 *target;
    s32 i;

    sound = obj->params >> 10;
    if (obj->work.targetPos[0] != obj->pos.v[0] || obj->work.targetPos[1] != obj->pos.v[1] ||
        obj->work.targetPos[2] != obj->pos.v[2]) {
        if (sound != 0 && D_80089260[sound].arrive != 0) {
            func_15116BAC(obj);
            return;
        }
        step = obj->params & ~0xFC00;
        if (sound != 0 && obj->sound == 0 && D_80089260[sound].start != 0) {
            func_15114D24(obj, D_80089260[sound].start, 24000, 2000, 4000, 0);
        }
        step = (step * D_800BE9E4) >> 1;
        pos = obj->pos.v;
        move = obj->move;
        target = obj->work.targetPos;
        for (i = 0; i < 3; i++) {
            move[i] = pos[i];
            if (pos[i] != target[i]) {
                if (target[i] < pos[i]) {
                    pos[i] -= step;
                    if (pos[i] < target[i]) {
                        pos[i] = target[i];
                    }
                } else {
                    pos[i] += step;
                    if (target[i] < pos[i]) {
                        pos[i] = target[i];
                    }
                }
            }
            move[i] = pos[i] - move[i];
        }
    } else {
        obj->move[0] = obj->move[1] = obj->move[2] = 0;
        if (obj->sound != 0) {
            if (sound != 0 && D_80089260[sound].stop != 0) {
                func_15114D24(obj, D_80089260[sound].stop, 24000, 2000, 4000, 4);
            }
            obj->sound = 0;
        }
    }
}

/* Like func_151169B4, but plays the arrive sound arriveLead steps before reaching the target. */
void func_15116BAC(MoverObj *obj) {
    s32 rate;
    s32 step;
    s32 sound;
    s32 started;
    s16 *pos;
    s16 *move;
    s32 *target;
    s32 dist;
    s32 i;
    s32 maxDist;

    sound = obj->params >> 10;
    started = FALSE;
    maxDist = 0;
    rate = obj->params & ~0xFC00;
    if (sound != 0 && obj->sound == 0 && D_80089260[sound].start != 0) {
        func_15114D24(obj, D_80089260[sound].start, 24000, 2000, 4000, 0);
        started = TRUE;
    }
    step = (rate * D_800BE9E4) >> 1;
    pos = obj->pos.v;
    move = obj->move;
    target = obj->work.targetPos;
    for (i = 0; i < 3; i++) {
        move[i] = pos[i];
        if (pos[i] != target[i]) {
            if (target[i] < pos[i]) {
                pos[i] -= step;
                dist = pos[i] - target[i];
                if (dist < 0) {
                    pos[i] = target[i];
                }
            } else {
                pos[i] += step;
                dist = target[i] - pos[i];
                if (dist < 0) {
                    pos[i] = target[i];
                }
            }
            if (maxDist < dist) {
                maxDist = dist;
            }
        }
        move[i] = pos[i] - move[i];
    }
    if (sound != 0 && D_80089260[sound].arrive != 0) {
        maxDist -= (D_80089260[sound].arriveLead * rate) >> 1;
        if (maxDist <= 0) {
            if (maxDist > -step || started) {
                func_15114D24(obj, D_80089260[sound].arrive, 24000, 2000, 4000, 0);
            }
        }
    }
}

/* Accelerates spin toward work.targetSpin (params / 256 per tick) and applies it to rot, wrapping to 0..360. */
void func_15116D7C(MoverObj *obj) {
    f32 accel;
    f32 dt;
    f32 *rot;
    f32 *spin;
    f32 *target;
    s32 i;

    accel = obj->params * (1.0f / 256);
    dt = D_800BE9E4;
    rot = obj->rot;
    spin = obj->spin;
    target = obj->work.targetSpin;
    for (i = 0; i < 3; i++) {
        if (spin[i] != target[i]) {
            if (target[i] < spin[i]) {
                spin[i] -= accel;
                if (spin[i] < target[i]) {
                    spin[i] = target[i];
                }
            } else {
                spin[i] += accel;
                if (target[i] < spin[i]) {
                    spin[i] = target[i];
                }
            }
        }
        rot[i] += spin[i] * dt;
        if (rot[i] < 0.0f) {
            rot[i] += 360.0f;
        } else if (rot[i] >= 360.0f) {
            rot[i] -= 360.0f;
        }
    }
}

/*
 * Vertex animation: interpolates the object's vertices between the two ROM frames around work.anim.pos
 * (0..0xFFFF over the whole animation), then steps pos toward work.anim.target by params >> 16.
 * params & 0xFFFF selects the D_80089320 animation; the frames are loaded on first use.
 */
void func_15116EA4(MoverObj *obj) {
    s32 size;
    f32 t;
    u32 frameSize;
    s32 vertexCount;
    s32 i;
    s16 (*cur)[3];
    s16 (*next)[3];
    s32 anim;
    s32 pos;
    s32 file;
    s32 frameCount;
    s32 speed;
    s32 frame;

    anim = obj->params & 0xFFFF;
    file = D_80089320[anim].file;
    pos = obj->work.anim.pos;
    frameCount = D_80089320[anim].frameCount;
    size = D_8002AAD0[file].end - D_8002AAD0[file].start;
    speed = obj->params >> 16;
    if (obj->work.anim.frames == NULL) {
        obj->work.anim.frames = allocate_memory(size, 1, 2, 0);
        func_10004514(D_8002AAD0[file].start, obj->work.anim.frames, size, 1);
    }
    t = frameCount * (pos * (1.0f / 65535));
    frame = t;
    t = t - frame;
    if (frameCount == frame) {
        t = 1.0f;
        frame--;
    }
    frameSize = (size / frameCount) & ~1;
    vertexCount = frameSize / 6;
    cur = (s16(*)[3])(obj->work.anim.frames + frame * frameSize);
    if (frame + 1 != (u32)frameCount) { // matching: the unsigned compare changes register allocation
        next = (s16(*)[3])(obj->work.anim.frames + (frame + 1) * frameSize);
    } else {
        next = cur;
    }
    for (i = 0; i < vertexCount; i++) {
        obj->vertices[D_800BE9C0][i].v.ob[0] = cur[i][0] + (next[i][0] - (f32)cur[i][0]) * t + obj->pos.s.x;
        obj->vertices[D_800BE9C0][i].v.ob[2] = cur[i][2] + (next[i][2] - (f32)cur[i][2]) * t + obj->pos.s.y;
        obj->vertices[D_800BE9C0][i].v.ob[1] = cur[i][1] + (next[i][1] - (f32)cur[i][1]) * t + obj->pos.s.z;
    }
    if (obj->work.anim.target < pos) {
        pos -= speed;
        if (pos < obj->work.anim.target) {
            pos = obj->work.anim.target;
        }
    } else if (pos < obj->work.anim.target) {
        pos += speed;
        if (obj->work.anim.target < pos) {
            pos = obj->work.anim.target;
        }
    }
    obj->work.anim.pos = pos;
}

/*
 * Swings angle toward the target in the low 10 bits of params (signed degrees) with a damped spring,
 * starting the params >> 10 start sound when it begins moving. Returns the new angle.
 */
f32 func_151172D8(MoverObj *obj, f32 angle) {
    f32 velocity;
    f32 delta;
    f32 target;
    f32 magnitude;
    f32 damping;
    f32 acceleration;

    damping = obj->work.swing.damping;
    acceleration = obj->work.swing.acceleration;
    velocity = obj->work.swing.velocity;
    target = (obj->params << 22) >> 22;
    if (angle != target || velocity != 0.0f) {
        if (velocity == 0.0f && (obj->params >> 10) != 0) {
            if (D_80089260[obj->params >> 10].start != 0) {
                func_15114D24(obj, D_80089260[obj->params >> 10].start, 24000, 200, 2500, 0);
            }
        }
        angle += velocity * D_800BE9E4;
        delta = func_15048A70(angle, target);
        magnitude = fabsf(delta);
        if (delta > 0.0f) {
            velocity += magnitude * acceleration;
        } else {
            velocity -= magnitude * acceleration;
        }
        velocity *= damping;
        if (fabsf(delta) < 0.01f && fabsf(velocity) < 0.01f) {
            angle = target;
            velocity = 0.0f;
        } else if (angle < 0.0f) {
            angle += 360.0f;
        } else if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        obj->work.swing.velocity = velocity;
    }
    return angle;
}
