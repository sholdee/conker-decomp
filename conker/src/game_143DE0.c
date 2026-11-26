#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Behaviour handlers for scenery objects (WorldObject) that move, spin or animate their vertices toward a
 * target. They are listed in the behaviour tables in 22D710.rodata.
 */

#define WORK(obj) ((MoverWork *)&(obj)->touch)

/* Animation entry of gPropVertexAnims: which ROM file holds the vertex frames, and how many there are. */
typedef struct VertexAnim {
    u8 file;
    u8 frameCount;
} VertexAnim;

/* .data, linked at 0x80089260..0x80089324 (the unit continues in game_1478C0.c). */

/* Sounds of the props, indexed by the sound row of the object's param (row 0: none). */
PropSounds gPropSounds[16] = {
    { 0, 0, 0, 0, 0, 0 },
    { 0x4B5, 0x4B6, 0, 0x4B7, 0, 0 },
    { 0x4B8, 0x4B8, 0x4B9, 0x4B9, 0, 0 },
    { 0x4C1, 0x4C2, -1, 0x4C3, 0, 0 },
    { 0x361, 0x361, 0x4C1, 0x4C1, 0, 0 },
    { 0x4BC, 0x4BC, 0, 0, 0x4BD, 30 },
    { 0, 0, 0x82, 0x82, 0, 0 },
    { 0x4BE, 0x4BF, 0x4C0, 0, 0, 0 },
    { 0x4BF, 0x4BF, -1, 0, 0, 0 },
    { 0x4C2, 0x4C2, -1, -1, 0, 0 },
    { 0x4C4, 0x4C5, -1, -1, 0, 0 },
    { 0x1A, 0x4C5, -1, -1, 0, 0 },
    { 0x4BA, 0, 0x4BB, 0, 0, 0 },
    { 0x4B, 0x4B, 0x4C8, 0x4C8, 0, 0 },
    { 0x67B, 0x67B, 0x4C8, 0x4C8, 0, 0 },
    { 0x62C, 0x62C, 0x4C2, 0x4C2, 0, 0 },
};

static VertexAnim gPropVertexAnims[2] = { { 0, 0 }, { 0, 0 } };


/*
 * Actor handler paired with prop_updateMover: a triggerable object (flags & 4) at rest (state bits 0-2 clear)
 * enters the moving state 2 when the actor's mv->unk57 is 1.
 */
void func_15116930(WorldObject *obj, Actor *actor) {
    if ((obj->object_status & 4) && !(obj->task_flag & TASK_FLAG_STATE) && !(obj->task_flag & TASK_FLAG_LOCKED) && actor->mv->unk57 == 1) {
        obj->task_flag &= ~TASK_FLAG_STATE;
        obj->task_flag |= TASK_FLAG_GOING_ON;
    }
}

/* Moves the object toward its target while it is in the moving state. */
void prop_updateMover(WorldObject *obj) {
    if (obj->task_flag & 2) {
        prop_stepToTarget(obj);
    }
}

/* Steps pos toward work.targetPos at the rate in param, playing the start and stop sounds. */
void prop_stepToTarget(WorldObject *obj) {
    s32 step;
    s32 sound;
    s16 *pos;
    s16 *move;
    s32 *target;
    s32 i;

    sound = obj->moveData.word >> 10;
    // matching: x, y, z indexed through &obj->x; as plain fields IDO loads them before the targets
    if (WORK(obj)->targetPos[0] != (&obj->x)[0] || WORK(obj)->targetPos[1] != (&obj->x)[1] ||
        WORK(obj)->targetPos[2] != (&obj->x)[2]) {
        if (sound != 0 && gPropSounds[sound].arrive != 0) {
            func_15116BAC(obj);
            return;
        }
        step = obj->moveData.word & ~0xFC00;
        if (sound != 0 && obj->sound_id == 0 && gPropSounds[sound].open != 0) {
            gfx_playObjectSound(obj, gPropSounds[sound].open, 24000, 2000, 4000, 0);
        }
        step = (step * D_800BE9E4) >> 1;
        pos = &obj->x;
        move = &obj->moveX;
        target = WORK(obj)->targetPos;
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
        obj->moveX = obj->moveY = obj->moveZ = 0;
        if (obj->sound_id != 0) {
            if (sound != 0 && gPropSounds[sound].opened != 0) {
                gfx_playObjectSound(obj, gPropSounds[sound].opened, 24000, 2000, 4000, 4);
            }
            obj->sound_id = 0;
        }
    }
}

/* Like prop_stepToTarget, but plays the arrive sound arriveLead steps before reaching the target. */
void func_15116BAC(WorldObject *obj) {
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

    sound = obj->moveData.word >> 10;
    started = FALSE;
    maxDist = 0;
    rate = obj->moveData.word & ~0xFC00;
    if (sound != 0 && obj->sound_id == 0 && gPropSounds[sound].open != 0) {
        gfx_playObjectSound(obj, gPropSounds[sound].open, 24000, 2000, 4000, 0);
        started = TRUE;
    }
    step = (rate * D_800BE9E4) >> 1;
    pos = &obj->x;
    move = &obj->moveX;
    target = WORK(obj)->targetPos;
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
    if (sound != 0 && gPropSounds[sound].arrive != 0) {
        maxDist -= (gPropSounds[sound].arriveLead * rate) >> 1;
        if (maxDist <= 0 && (maxDist > -step || started)) {
            gfx_playObjectSound(obj, gPropSounds[sound].arrive, 24000, 2000, 4000, 0);
        }
    }
}

/* Accelerates spin toward work.targetSpin (param / 256 per tick) and applies it to rot, wrapping to 0..360. */
void prop_spinToTarget(WorldObject *obj) {
    f32 accel;
    f32 dt;
    f32 *rot;
    f32 *spin;
    f32 *target;
    s32 i;

    accel = obj->moveData.word * (1.0f / 256);
    dt = D_800BE9E4;
    rot = &obj->t;
    spin = &obj->spinX;
    target = WORK(obj)->targetSpin;
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
 * (0..0xFFFF over the whole animation), then steps pos toward work.anim.target by param >> 16.
 * param & 0xFFFF selects the gPropVertexAnims animation; the frames are loaded on first use.
 */
void prop_animateVertices(WorldObject *obj) {
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

    anim = obj->moveData.word & 0xFFFF;
    file = gPropVertexAnims[anim].file;
    pos = WORK(obj)->anim.pos;
    frameCount = gPropVertexAnims[anim].frameCount;
    size = D_8002AAD0[file].end - D_8002AAD0[file].start;
    speed = obj->moveData.word >> 16;
    if (WORK(obj)->anim.frames == NULL) {
        WORK(obj)->anim.frames = allocate_memory(size, 1, 2, 0);
        rom_read(D_8002AAD0[file].start, WORK(obj)->anim.frames, size, 1);
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
    cur = (s16(*)[3])(WORK(obj)->anim.frames + frame * frameSize);
    if (frame + 1 != (u32)frameCount) { // matching: the unsigned compare changes register allocation
        next = (s16(*)[3])(WORK(obj)->anim.frames + (frame + 1) * frameSize);
    } else {
        next = cur;
    }
    for (i = 0; i < vertexCount; i++) {
        obj->vertices[D_800BE9C0][i].v.ob[0] = cur[i][0] + (next[i][0] - (f32)cur[i][0]) * t + obj->x;
        obj->vertices[D_800BE9C0][i].v.ob[2] = cur[i][2] + (next[i][2] - (f32)cur[i][2]) * t + obj->y;
        obj->vertices[D_800BE9C0][i].v.ob[1] = cur[i][1] + (next[i][1] - (f32)cur[i][1]) * t + obj->z;
    }
    if (WORK(obj)->anim.target < pos) {
        pos -= speed;
        if (pos < WORK(obj)->anim.target) {
            pos = WORK(obj)->anim.target;
        }
    } else if (pos < WORK(obj)->anim.target) {
        pos += speed;
        if (WORK(obj)->anim.target < pos) {
            pos = WORK(obj)->anim.target;
        }
    }
    WORK(obj)->anim.pos = pos;
}

/*
 * Swings angle toward the target in the low 10 bits of param (signed degrees) with a damped spring,
 * starting the param >> 10 start sound when it begins moving. Returns the new angle.
 */
f32 prop_springAngle(WorldObject *obj, f32 angle) {
    f32 velocity;
    f32 delta;
    f32 target;
    f32 magnitude;
    f32 damping;
    f32 acceleration;

    damping = WORK(obj)->swing.damping;
    acceleration = WORK(obj)->swing.acceleration;
    velocity = WORK(obj)->swing.velocity;
    target = (obj->moveData.word << 22) >> 22;
    if (angle != target || velocity != 0.0f) {
        if (velocity == 0.0f && (obj->moveData.word >> 10) != 0 && gPropSounds[obj->moveData.word >> 10].open != 0) {
            gfx_playObjectSound(obj, gPropSounds[obj->moveData.word >> 10].open, 24000, 200, 2500, 0);
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
        WORK(obj)->swing.velocity = velocity;
    }
    return angle;
}
