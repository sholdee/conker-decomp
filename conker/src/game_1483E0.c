#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Per-type update routines for world objects (WorldObject, the db_Object_array array). The mode is in the low
 * two bits of task_flag (TASK_FLAG_OFF closed, GOING_OFF closing, GOING_ON opening, ON open); per-type state lives in work
 * (Work1483E0, one member per routine).
 */

struct Wave1511CB44;
struct Swing1511DBC4;
struct Pulse1511DF6C;

/* Behaviour scratch state (WorldObject.touch) of the objects in this file, one member per routine. */
typedef union {
    struct {
        /* 0x7C */ f32 tilt;
        /* 0x80 */ f32 wobble;
        /* 0x84 */ f32 wobbleTarget;
    } w1511AF30;
    struct {
        /* 0x7C */ s32 homeX;
        /* 0x80 */ s32 homeZ;
        /* 0x84 */ f32 speed;
    } w1511B07C;
    struct {
        /* 0x7C */ f32 restitution;
        /* 0x80 */ f32 maxSpeed;
        /* 0x84 */ f32 velocity;
    } w1511B51C;
    struct {
        /* 0x7C */ s32 updates;
        /* 0x80 */ f32 prevAngle[2];
    } w1511B7D4;
    struct {
        /* 0x7C */ s32 restAndTimer;
        /* 0x80 */ s32 triggered;
        /* 0x84 */ f32 velocity;
    } w1511BEBC;
    struct {
        u8 pad7C[4];
        /* 0x80 */ f32 phaseX;
        /* 0x84 */ f32 phaseZ;
    } w1511C548;
    struct {
        /* 0x7C */ s32 delay;
        /* 0x80 */ s32 fade;
    } w1511C638;
    struct {
        /* 0x7C */ struct Wave1511CB44 *wave;
    } w1511CB44;
    struct {
        /* 0x7C */ s16 *baseS;
        /* 0x80 */ f32 velocity;
        /* 0x84 */ f32 level;
    } w1511D394;
    struct {
        /* 0x7C */ s16 (*uv)[2];
        /* 0x80 */ f32 uvAngle;
    } w1511D9E4;
    struct {
        /* 0x7C */ struct Swing1511DBC4 *swing;
    } w1511DBC4;
    struct {
        /* 0x7C */ struct Pulse1511DF6C *pulse;
        /* 0x80 */ s32 lastCharacter;
        /* 0x84 */ s32 filter;
    } w1511DF6C;
    struct {
        /* 0x7C */ s32 started;   /* set after the first update */
        /* 0x80 */ s32 triggered; /* 1: move away from home, 0: return home */
        /* 0x84 */ f32 home;      /* rotation or coordinate saved on the first update */
    } slide; /* func_1511E780, func_1511EC50, func_1511ED84 */
    struct {
        /* 0x7C */ s32 started;
        /* 0x80 */ s32 unk80;
        /* 0x84 */ f32 phase; /* radians */
    } wobble; /* func_1511E780 variants 10-13 */
} Work1483E0;

#define WORK(obj) ((Work1483E0 *)&(obj)->touch)

/* func_1511E780's row sound slots */
#define PROP_SOUND_OPEN 0
#define PROP_SOUND_CLOSE 1


/* Eases the tilt toward 0.75 degrees per level, then wobbles randomly around it. */
void func_1511AF30(WorldObject *obj) {
    f32 intensity;
    f32 target;
    f32 diff;
    s32 roll;

    intensity = obj->moveData.word;
    target = 75.0f * intensity * 0.009999999776f;
    if (target != WORK(obj)->w1511AF30.tilt) {
        diff = target - WORK(obj)->w1511AF30.tilt;
        if (fabsf(diff) < 1.0f) {
            WORK(obj)->w1511AF30.tilt = target;
        } else {
            WORK(obj)->w1511AF30.tilt += (diff < 0.0f) ? -1 : 1;
        }
        WORK(obj)->w1511AF30.wobble = 0.0f;
    } else {
        roll = func_150ADA20() % 1000;
        if (roll < 500) {
            WORK(obj)->w1511AF30.wobbleTarget = roll * 5.0f * intensity * 0.002000000095f * 0.009999999776f;
        }
        diff = WORK(obj)->w1511AF30.wobbleTarget - WORK(obj)->w1511AF30.wobble;
        WORK(obj)->w1511AF30.wobble += diff * 0.400000006f;
    }
    obj->t = WORK(obj)->w1511AF30.tilt + WORK(obj)->w1511AF30.wobble;
}

/* Slides the object between fixed waypoints chosen by its param, with movement sounds. */
void func_1511B07C(WorldObject *obj) {
    f32 speed;
    f32 maxSpeed;
    s32 mode;
    s32 state;
    s32 targetX;
    s32 targetZ;
    s32 step;
    s32 moveX;
    s32 moveZ;
    s32 sound;

    maxSpeed = 7.0f;
    moveX = 0;
    moveZ = 0;
    sound = 0;
    if (D_800C35EA[0] == 1) {
        if (func_15022B08(obj - db_Object_array, 0) != 0) {
            return;
        }
    }
    if (WORK(obj)->w1511B07C.homeX == 0) {
        WORK(obj)->w1511B07C.homeX = obj->x | 0x80000000;
        WORK(obj)->w1511B07C.homeZ = obj->z;
    }
    targetX = (s16)WORK(obj)->w1511B07C.homeX;
    targetZ = WORK(obj)->w1511B07C.homeZ;
    state = obj->moveData.word & 0x3FF;
    if (state >= 4 && state < 7) {
        func_1511F990(obj, 1);
    }
    mode = obj->task_flag & TASK_FLAG_STATE;
    if (mode == TASK_FLAG_ON || mode == TASK_FLAG_GOING_ON) {
        switch (state) {
        case 0:
            targetX = -69;
            targetZ = -120;
            if (mode == TASK_FLAG_GOING_ON) {
                sound = 0x4E;
            }
            moveX = 1;
            break;
        case 1:
            targetX = -69;
            targetZ = 120;
            moveX = 1;
            break;
        case 2:
            targetX = 100;
            if (mode == TASK_FLAG_GOING_ON) {
                sound = 0x4E;
            }
            break;
        case 3:
            targetX = 0;
            break;
        case 4:
            maxSpeed = 15.0f;
            targetX = 270;
            targetZ = -1878;
            moveZ = 1;
            if (mode == TASK_FLAG_GOING_ON) {
                sound = 0x4E;
            }
            break;
        case 5:
            targetZ = 140;
            if (mode == TASK_FLAG_GOING_ON) {
                sound = 0x4C1;
            }
            break;
        case 6:
            targetZ = -140;
            break;
        case 7:
            maxSpeed = 15.0f;
            targetX = -4100;
            targetZ = 422;
            moveX = 1;
            if (mode == TASK_FLAG_GOING_ON) {
                sound = 0x4E;
            }
            break;
        }
    } else {
        switch (state) {
        case 4:
            maxSpeed = 15.0f;
            moveX = 1;
            if (mode == TASK_FLAG_GOING_OFF) {
                sound = 0x4E;
            }
            break;
        case 7:
            maxSpeed = 15.0f;
            moveZ = 1;
            if (mode == TASK_FLAG_GOING_OFF) {
                sound = 0x4E;
            }
            break;
        }
    }
    if (sound != 0) {
        func_15114D24(obj, sound, 0x7D00, 100, 2500, 1);
    }
    speed = WORK(obj)->w1511B07C.speed;
    if (mode == TASK_FLAG_OFF || mode == TASK_FLAG_ON) {
        speed = 0.0f;
        obj->x = targetX;
        obj->z = targetZ;
    }
    if (speed < maxSpeed) {
        speed += (maxSpeed - speed) * 0.02999999933f;
    }
    if ((moveX != 0 || targetZ == obj->z) && targetX != obj->x && (moveZ == 0 || targetZ == obj->z)) {
        step = targetX - obj->x;
        if (step < -speed) {
            step = -speed;
        } else if (speed < step) {
            step = speed;
        }
        obj->x += step;
        if (targetX == obj->x) {
            speed = 0.0f;
            if (state == 0) {
                func_15114D24(obj, 0x4E, 0x7D00, 100, 2500, 1);
            }
        }
    } else if (targetZ != obj->z) {
        step = targetZ - obj->z;
        if (step < -speed) {
            step = -speed;
        } else if (speed < step) {
            step = speed;
        }
        obj->z += step;
    } else if (mode == TASK_FLAG_GOING_ON || mode == TASK_FLAG_GOING_OFF) {
        // matching: the embedded &= makes IDO store the cleared flags before merging
        obj->task_flag = ((mode == TASK_FLAG_GOING_ON) ? TASK_FLAG_ON : TASK_FLAG_OFF) | (obj->task_flag &= ~TASK_FLAG_STATE);
        if (obj->sound_id != 0) {
            func_100111C8(obj->sound_id);
            obj->sound_id = 0;
            func_15114D24(obj, 0x4C8, 0x7D00, 100, 2500, 1);
        }
    }
    WORK(obj)->w1511B07C.speed = speed;
}


/* Swings the angle toward its open/closed target with a damped bounce, driving a sound by it. */
void func_1511B51C(WorldObject *obj) {
    f32 velocity;
    f32 target;
    f32 angle;
    f32 diff;
    f32 restitution;
    f32 maxSpeed;
    s32 mode;
    s32 sound;
    s32 openAngle;
    f32 ratio;

    openAngle = obj->moveData.half.lo;
    mode = obj->task_flag & TASK_FLAG_STATE;
    target = openAngle;
    angle = obj->v;
    velocity = WORK(obj)->w1511B51C.velocity;
    if (mode == TASK_FLAG_OFF || mode == TASK_FLAG_GOING_OFF) {
        target = 0.0f;
    }
    if (mode == TASK_FLAG_OFF || mode == TASK_FLAG_ON) {
        angle = target;
        velocity = 0.0f;
    }
    if (angle != target || velocity != 0.0f) {
        restitution = WORK(obj)->w1511B51C.restitution;
        if (restitution == 0.0f) {
            restitution = 0.4499999881f;
        }
        maxSpeed = WORK(obj)->w1511B51C.maxSpeed;
        if (maxSpeed == 0.0f) {
            maxSpeed = 0.5f;
        }
        angle += velocity;
        if (angle != 0.0f) {} // matching: no-op use of angle steers IDO's FP register choice
        diff = func_15048A70(angle, target);
        if (fabsf(diff) < fabsf(velocity)) {
            velocity = -velocity * restitution;
            if (fabsf(velocity) < maxSpeed * 0.05000000075f) {
                velocity = 0.0f;
                angle = target;
                if (target == 0.0f) {
                    mode = TASK_FLAG_OFF;
                } else {
                    mode = TASK_FLAG_ON;
                }
            }
        } else if (diff > 0.0f) {
            velocity += (maxSpeed - velocity) * 0.009999999776f;
        } else {
            velocity -= (maxSpeed + velocity) * 0.009999999776f;
        }
        if (angle < 0.0f) {
            angle += 360.0f;
        } else if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        obj->task_flag &= ~TASK_FLAG_STATE;
        obj->task_flag |= mode;
    }
    WORK(obj)->w1511B51C.velocity = velocity;
    if (D_800BE9B4 != 0 || angle != obj->v) {
        obj->v = angle;
        sound = (obj->moveData.word >> 24) & 0xFF;
        if (sound != 0 && openAngle != 0) {
            ratio = fabsf((angle - openAngle) / openAngle);
            func_1518804C(sound - 1, ratio);
            func_15173C60(((1.0f - ratio) * 255.0f), sound - 1);
        }
    }
}

/* (vertex, pivot) index pairs: vertices that follow the rotation of a preceding object. */
extern u8 D_800A2F70;
extern u8 D_800A2F71[];

/* Rotates the listed vertices about the pivot objects just before this one in the object array. */
void func_1511B7D4(WorldObject *obj) {
    u8 *pair;
    s32 count;
    s32 dx;
    s32 dy;
    f32 angle;
    f32 sine;
    f32 cosine;
    s32 x;
    s32 y;
    s32 z;
    s32 rotatedY;
    WorldObject *pivots[2];

    count = 0;
    if (obj->moveData.word == 0) {
        pair = D_800A2F71;
        count = D_800A2F70;
        pivots[0] = obj - 1;
        pivots[1] = obj - 2;
    }
    if (WORK(obj)->w1511B7D4.prevAngle[0] != pivots[0]->v || WORK(obj)->w1511B7D4.prevAngle[1] != pivots[1]->v) {
        WORK(obj)->w1511B7D4.prevAngle[0] = pivots[0]->v;
        WORK(obj)->w1511B7D4.prevAngle[1] = pivots[1]->v;
        WORK(obj)->w1511B7D4.updates = 2;
    }
    if (WORK(obj)->w1511B7D4.updates != 0) {
        WORK(obj)->w1511B7D4.updates--;
        while (count--) {
            dx = pivots[pair[1]]->x - obj->x;
            angle = -pivots[pair[1]]->v * 0.01745329238f;
            dy = pivots[pair[1]]->y - obj->y;
            sine = sinf(angle);
            cosine = cosf(angle);
            y = obj->srcVertices[pair[0]].v.ob[1];
            x = obj->srcVertices[pair[0]].v.ob[0];
            z = obj->srcVertices[pair[0]].v.ob[2];
            y -= dy;
            x -= dx;
            rotatedY = (y * cosine - x * sine);
            x = (y * sine + x * cosine);
            y = rotatedY;
            x += dx;
            y += dy;
            x += obj->x;
            y += obj->y;
            z += obj->z;
            obj->vertices[D_800BE9C0][pair[0]].v.ob[0] = x;
            obj->vertices[D_800BE9C0][pair[0]].v.ob[1] = y;
            obj->vertices[D_800BE9C0][pair[0]].v.ob[2] = z;
            pair += 2;
        }
    }
}


/* Sets the alpha from a 0..1 level looked up by the param, never fully transparent. */
void func_1511BA24(WorldObject *obj) {
    f32 intensity;

    intensity = 0.0f;
    func_15188010(obj->moveData.word, &intensity);
    obj->alpha = intensity * 255.0f;
    if (obj->alpha == 0) {
        obj->alpha = 1;
    }
}

/*
 * Scrolls the mesh's texture coordinates so the texture stays fixed in world space under
 * (refX, refZ), scaled by 1/scaleS and 1/scaleT. Copies the source mesh into both buffers on first use.
 */
void func_1511BB04(Game1483E0State *obj, f32 refX, f32 refZ, f32 scaleS, f32 scaleT) {
    Game1483E0UvPair *coords;
    s32 i;
    f32 s_offset;
    f32 t_offset;
    f32 s_scale;
    f32 t_scale;

    if (obj->coords == NULL) {
        coords = allocate_memory(obj->count * sizeof(Game1483E0UvPair), 1, 0, 0);
        obj->coords = coords;
        for (i = 0; i < obj->count; i++) {
            coords[i].s = obj->source[i].s;
            coords[i].t = obj->source[i].t;
            obj->buffers[0][i] = obj->source[i];
            obj->buffers[1][i] = obj->source[i];
            obj->buffers[0][i].x += obj->x;
            obj->buffers[0][i].y += obj->y;
            obj->buffers[0][i].z += obj->z;
            obj->buffers[1][i].x += obj->x;
            obj->buffers[1][i].y += obj->y;
            obj->buffers[1][i].z += obj->z;
        }
    }

    coords = obj->coords;
    s_offset = refX - (obj->x + 3128.0f);
    t_offset = refZ - (obj->z + -2341.0f);
    s_offset = -s_offset;
    s_offset *= 4.300000191f;
    t_offset *= 4.300000191f;
    s_offset -= 1024.0f;
    t_offset -= 1024.0f;
    s_scale = 1.0f;
    t_scale = 1.0f;
    if (scaleS != 1.0f) {
        s_scale = 1.0f / scaleS;
    }
    if (scaleT != 1.0f) {
        t_scale = 1.0f / scaleT;
    }
    for (i = 0; i < obj->count; i++) {
        obj->buffers[D_800BE9C0][i].s = ((coords[i].s + s_offset) * s_scale + 1024.0f);
        obj->buffers[D_800BE9C0][i].t = ((coords[i].t + t_offset) * t_scale + 1024.0f);
    }
}

/* Projects the mesh texture under its reference object (or the object numbered reference_id). */
void func_1511BDF4(Game1483E0State *obj) {
    Actor *reference;

    if (obj->reference != NULL) {
        reference = obj->reference;
    } else {
        reference = func_15083E90(obj->reference_id);
    }
    if (reference != NULL) {
        func_1511BB04(obj, reference->xpos, reference->zpos, 1.0f, 1.0f);
    }
}

/* Turns the object to face the camera (yaw in degrees). */
void func_1511BE5C(WorldObject *obj) {
    obj->u = func_150484A0(obj->x - camera_global_Cameras->eye.x, obj->z - camera_global_Cameras->eye.z) * 57.29577637f;
}

/* After a delay, lowers onto a player found beneath it and holds them at its position; rises back to rest otherwise. */
void func_1511BEBC(WorldObject *obj) {
    f32 pos[3];
    s16 restY;
    Actor *target;
    Actor *character;
    Camera *player;
    s32 i;
    s32 mode;
    s32 flag8;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 timer;
    f32 radiusSq;
    s32 flag4;
    s32 triggered;
    f32 velocity;
    ActorCtl *trap;
    f32 dist;
    void (*onTrigger)(WorldObject *);

    onTrigger = func_150DDED0;
    func_1511BE5C(obj);
    mode = obj->task_flag & TASK_FLAG_STATE;
    flag4 = obj->task_flag & TASK_FLAG_LOCKED;
    flag8 = obj->task_flag & 8;
    triggered = WORK(obj)->w1511BEBC.triggered;
    velocity = WORK(obj)->w1511BEBC.velocity;
    if (level == 10) {
        radiusSq = (obj->zScale * 100.0f) * (obj->zScale * 100.0f);
        if (WORK(obj)->w1511BEBC.restAndTimer == 0) {
            if (obj->y == 0) {
                obj->y = 1;
            }
            WORK(obj)->w1511BEBC.restAndTimer = (u16)obj->y;
            restY = obj->y;
            if (mode == TASK_FLAG_ON) {
                obj->y = restY;
            }
        }
        restY = (WORK(obj)->w1511BEBC.restAndTimer & 0xFFFF);
        timer = WORK(obj)->w1511BEBC.restAndTimer >> 16;
        target = NULL;
        pos[0] = obj->x;
        pos[1] = obj->y + -729.0f; // matching: "- 729.0f" emits sub.s instead of add.s
        pos[2] = obj->z;
        if (mode != TASK_FLAG_ON && mode != TASK_FLAG_GOING_OFF) {
            for (i = 0; i < D_8008FD8C; i++) {
                player = &camera_global_Cameras[i];
                if (player->mv->unk6B >= 4) {
                    continue;
                }
                character = player->CharPtr;
                if (character->ypos + 40.0f < pos[1]) {
                    dy = (character->ypos + character->bodyHeight) - pos[1];
                    dz = character->zpos - pos[2];
                    dx = character->xpos - pos[0];
                    dist = dz * dz + (dx * dx + dy * dy);
                    if (dist != dist) {} // matching: no-op NaN self-test steers IDO's FP scheduling
                    if (dist < radiusSq) {
                        target = character;
                        break;
                    }
                }
            }
        }
        if (target == NULL && (mode == TASK_FLAG_GOING_ON || mode == TASK_FLAG_GOING_OFF)) {
            for (i = 0; i < D_8008FD8C; i++) {
                character = &movement_obj[i];
                if (character->mv->unk6B != 0) {
                    target = character;
                    break;
                }
            }
        }
        if (target != NULL || mode == TASK_FLAG_GOING_ON) {
            if (target != NULL && triggered == 0) {
                func_15060A30(0x16B, target);
            }
            if (flag4 == 0) {
                mode = TASK_FLAG_GOING_ON;
                if (triggered == 0) {
                    timer = 50;
                    triggered++;
                    if (onTrigger != NULL) {
                        onTrigger(obj);
                    }
                }
                if (target != NULL) {
                    if (timer != 0) {
                        target->mv->unk6B = 1;
                    } else {
                        target->mv->unk6B = 2;
                    }
                }
                if (timer != 0) {
                    if (D_800BE9E4 < timer) {
                        timer -= D_800BE9E4;
                    } else {
                        timer = 0;
                        func_10010F88(0x1A4, 0x7FFF, 0, 0, 0, obj->x, obj->y, obj->z, 1500, 3500);
                    }
                } else {
                    velocity += 1.0f;
                    obj->y = obj->y - velocity;
                    if (obj->y <= restY - 300.0f) {
                        obj->y = restY - 300.0f;
                        mode = TASK_FLAG_ON;
                        triggered = 0;
                        velocity = 0.0f;
                        if (target != NULL) {
                            target->mv->unk6B = 0;
                        }
                        func_10010F88(0x1A5, 0x7FFF, 0, 0, 0, obj->x, obj->y, obj->z, 1500, 3500);
                    }
                }
                if (target != NULL) {
                    target->mv->holdX = pos[0];
                    target->mv->holdY = pos[1];
                    target->mv->holdZ = pos[2];
                }
            } else if (target != NULL) {
                if (triggered == 0 && target->mv->unk6B == 0) {
                    timer = 50;
                    triggered++;
                }
                trap = target->mv;
                if (timer != 0) {
                    trap->unk6B = 3;
                } else if (trap->unk6B == 3) {
                    trap->unk6B = 4;
                    triggered = 0;
                }
                if (timer != 0) {
                    if (D_800BE9E4 < timer) {
                        timer -= D_800BE9E4;
                    } else {
                        timer = 0;
                    }
                }
            }
            if (target != NULL) {
                target->mv->holdX = pos[0];
                target->mv->holdY = pos[1];
                target->mv->holdZ = pos[2];
            }
        } else if (flag8 == 0 || mode == TASK_FLAG_GOING_OFF) {
            if (obj->y < restY) {
                if (triggered == 0) {
                    triggered++;
                }
                mode = TASK_FLAG_GOING_OFF;
                velocity += 1.299999952f;
                obj->y = obj->y + velocity;
                if (obj->y >= restY) {
                    obj->y = restY;
                    triggered = 0;
                    mode = TASK_FLAG_OFF;
                    velocity = 0.0f;
                }
            }
            if (target != NULL) {
                target->mv->unk6B = 0;
            }
        }
        WORK(obj)->w1511BEBC.triggered = triggered;
        obj->task_flag &= ~TASK_FLAG_STATE;
        obj->task_flag |= mode;
        WORK(obj)->w1511BEBC.restAndTimer &= 0xFFFF;
        WORK(obj)->w1511BEBC.restAndTimer |= timer << 16;
        WORK(obj)->w1511BEBC.velocity = velocity;
    }
}

/* Update routine for objects that need none. */
void func_1511C540(void) {
}

/* Sways the object around its X and Z axes by up to 16 degrees. */
void func_1511C548(WorldObject *obj) {
    f32 swayZ;
    f32 swayX;

    swayX = sinf(WORK(obj)->w1511C548.phaseX);
    swayZ = sinf(WORK(obj)->w1511C548.phaseZ);
    obj->t = swayX * 16.0f;
    obj->v = swayZ * 16.0f;
    WORK(obj)->w1511C548.phaseX += 0.07299999893f * D_800BE9A4;
    WORK(obj)->w1511C548.phaseZ += 0.06499999762f * D_800BE9A4;
    WORK(obj)->w1511C548.phaseX = func_15144B68(WORK(obj)->w1511C548.phaseX);
    {
        f32 wrapped = func_15144B68(WORK(obj)->w1511C548.phaseZ);

        WORK(obj)->w1511C548.phaseZ = wrapped;
    }
    if (obj->v == 0.0f && WORK(obj)->w1511C548.phaseZ == 0.0f) {
        obj->t = 7.5f;
        obj->v = 8.0f;
    }
}


/* The three sound ids func_1511C638 picks from: { 0x2B6, 0x2BD, 0x2BE }. The original local
 * initializer lies in the rodata of the larger original compilation unit (before game_142560's
 * slice), so it is copied from there. */
typedef struct { u16 ids[3]; } SoundTriple;
extern const SoundTriple D_800A2F84;

/*
 * Fades the object (and the one or two objects after it) in when triggered, playing the
 * effect chosen by the next object's param; turns them off again when reset.
 */
void func_1511C638(WorldObject *obj) {
    s32 mode;
    WorldObject *next;
    WorldObject *third;

    next = &obj[1];
    mode = obj->task_flag & TASK_FLAG_STATE;
    if (level == 4) {
        third = &obj[2];
    } else {
        third = NULL;
    }
    if (mode == TASK_FLAG_GOING_OFF) {
        mode = TASK_FLAG_OFF;
        obj->alpha = 0;
        next->inactive = 0;
        if (third != NULL) {
            third->inactive = 0;
        }
        if (obj->moveData.word != 0xFF) {
            func_15173C60(0, obj->moveData.word);
        }
    }
    if (D_800BE9B4 != 0) {
        if (mode == TASK_FLAG_ON) {
            obj->alpha = 0xFF;
            next->inactive = 1;
            if (third != NULL) {
                third->inactive = 1;
            }
        } else {
            obj->alpha = 0;
            next->inactive = 0;
            if (third != NULL) {
                third->inactive = 0;
            }
        }
    }
    if (mode == TASK_FLAG_GOING_ON) {
        if (WORK(obj)->w1511C638.delay < 0) {
            WORK(obj)->w1511C638.delay += D_800BE9E4;
        } else {
            if (WORK(obj)->w1511C638.fade == 0) {
                switch (next->moveData.word & 0xFF) {
                case 1:
                    func_15170F4C(next, 0xFF, 0);
                    func_10010F30(0x2BE, 0x7FFF, 64, 0, 0);
                    break;
                case 0: {
                    u16 first = func_150ADA20() % 3U;
                    u16 second;
                    SoundTriple sounds = D_800A2F84;

                    second = ((func_150ADA20() & 1) + first + 1) % 3U;
                    first = sounds.ids[first];
                    second = sounds.ids[second];
                    func_10010F30(0x2B7, 0x7FFF, 64, 0, 0);
                    func_10010F30(first, 0x7FFF, 10, 0, 0);
                    func_10010F30(second, 0x7FFF, 54, 0, 0);
                    first = func_150ADA20() % 5U;
                    second = ((func_150ADA20() & 3) + first + 1) % 5U;
                    func_1000FA64(first + 0x2B1, obj->x, obj->y, obj->z, 0x4000, 10001, 10000, func_1000EC24, (void *)0x1001E, 0, 0x200, 0);
                    func_1000FA64(second + 0x2B1, obj->x, obj->y, obj->z, 0x4000, 10001, 10000, func_1000EC24, (void *)0x1001E, 0, 0, 0);
                    first = func_150ADA20() % 5U;
                    second = ((func_150ADA20() & 3) + first + 1) % 5U;
                    func_1000FA64(first + 0x2B1, obj->x, obj->y, obj->z, 0x4000, 10001, 10000, func_1000EC24, (void *)60, 0, 0x300, 0);
                    func_1000FA64(second + 0x2B1, obj->x, obj->y, obj->z, 0x4000, 10001, 10000, func_1000EC24, (void *)60, 0, 0x100, 0);
                }
                break;
                case 2:
                    break;
                case 3:
                    func_150B2740(next->moveData.word >> 8, 0xFF);
                    break;
                }
            }
            obj->alpha = 0xFF;
            next->inactive = 1;
            if (third != NULL) {
                third->inactive = 1;
            }
            WORK(obj)->w1511C638.fade += D_800BE9E4 * 30;
            if (WORK(obj)->w1511C638.fade >= 0xFF) {
                WORK(obj)->w1511C638.fade = 0xFF;
                mode = TASK_FLAG_ON;
            }
            if (obj->moveData.word != 0xFF) {
                func_15173C60(WORK(obj)->w1511C638.fade, obj->moveData.word);
            }
            if (mode == TASK_FLAG_ON) {
                WORK(obj)->w1511C638.delay = 0;
                WORK(obj)->w1511C638.fade = 0;
            }
        }
    }
    obj->task_flag &= ~TASK_FLAG_STATE;
    obj->task_flag |= mode;
}

/* Gives the fixed wave origin Z (22000) used by func_1511CB44. */
void func_1511CB2C(s32 obj, f32 *originZ) {
    *originZ = 22000.0f;
}

struct Wave1511CB44 {
    u8 *phases;
    u16 phase;
    u16 speed;
    f32 amp;
    TexScroll *texScroll;
    u8 alphaPhase;
    u8 alphaPhaseOffset;
    u8 pulseAlpha;
    u8 speedPhaseOffset;
    f32 uvAngle;
    s16 (*uv)[2];
};

extern s32 D_800902D8;

/*
 * Animates a wave mesh: vertex heights follow a travelling sine wave with per-vertex phase,
 * shading and alpha pulse with it, and (variant 0xFE) the texture coordinates rotate.
 */
void func_1511CB44(WorldObject *obj) {
    Vtx *dst;
    Vtx *src;
    s32 pad9C; // matching: unused stack slots keep the original frame layout
    s32 pad98;
    s32 pad94;
    f32 originZ;
    s32 pad8C;
    s32 created;
    struct Wave1511CB44 *wave;
    s32 i;
    f32 scale;
    f32 height;
    f32 tmp;
    f32 z;
    f32 y;
    u32 halfSpeed;
    u16 speed;
    u8 shade;
    u8 phase;
    s32 mode;
    s32 pad5C;
    f32 uvRate;
    s32 uvArg4;
    s32 uvArg5;

    uvRate = 0.0f;
    if (level == 4) {
        mode = 0;
    } else if (level == 19) {
        mode = 1;
        if (obj->unique_id == 0xFE) {
            uvRate = 0.4530000091f;
            uvArg4 = 32;
            uvArg5 = 32;
        }
    } else {
        return;
    }
    func_1511CB2C((s32)obj, &originZ);
    originZ -= obj->z;
    if (WORK(obj)->w1511CB44.wave == NULL) {
        created = TRUE;
        WORK(obj)->w1511CB44.wave = allocate_memory(sizeof(struct Wave1511CB44), 1, 0, 0);
        wave = WORK(obj)->w1511CB44.wave;
        wave->phases = allocate_memory(obj->numVertices, 1, 0, 0);
        if (uvRate != 0.0f) {
            wave->uv = allocate_memory(obj->numVertices * 4, 1, 0, 0);
            for (i = 0; i < obj->numVertices; i++) {
                wave->uv[i][0] = obj->srcVertices[i].v.tc[0] - 0x2000;
                wave->uv[i][1] = obj->srcVertices[i].v.tc[1] - 0x2000;
            }
            wave->uvAngle = 0.0f;
        }
    } else {
        created = FALSE;
    }
    wave = WORK(obj)->w1511CB44.wave;
    if (created) {
        if (mode == 0) {
            wave->speed = 474;
            wave->speedPhaseOffset = 109;
            wave->amp = 100.0f;
            scale = 0.1000000015f;
        } else {
            scale = 1.0f;
            wave->speed = 601;
            wave->speedPhaseOffset = 109;
            wave->amp = 36.0f;
        }
        wave->phase = 0;
        dst = obj->vertices[D_800BE9C0];
        src = obj->srcVertices;
        for (i = 0; i < obj->numVertices; i++) {
            tmp = originZ - obj->srcVertices[i].v.ob[2];
            wave->phases[i] = tmp * scale;
            dst->v.ob[0] = src->v.ob[0] + obj->x;
            dst->v.ob[2] = src->v.ob[2] + obj->z;
            dst++;
            src++;
        }
        wave->pulseAlpha = 0;
        wave->alphaPhase = 0;
        if (level == 4) {
            if (obj->unique_id != 0xF2) {
                if (obj->unique_id == 0xF1) {
                    wave->alphaPhaseOffset = 0;
                } else {
                    wave->alphaPhaseOffset = 0x80;
                }
                wave->pulseAlpha = 1;
            }
        }
        if (mode == 1) {
            wave->texScroll = func_15195FB0(obj, D_800902D8, 0, 0, 0, 0, 8);
        }
    }
    dst = obj->vertices[D_800BE9C0];
    src = obj->srcVertices;
    phase = wave->phase >> 8;
    scale = 0.8100000024f;
    for (i = 0; i < obj->numVertices; i++) {
        height = func_150489B0(wave->phases[i] + phase);
        y = (src->v.ob[1] + obj->y) + height * wave->amp;
        if (mode == 0) {
            z = src->v.ob[2] + obj->z;
            if (z < 12000.0f) {
                height += 1.0f;
                height *= 0.5f;
                tmp = (z - 12000.0f) * ((1.0f - scale) * height + scale);
                z = 12000.0f + tmp;
                dst->v.ob[2] = z;
            }
        }
        dst->v.ob[1] = y;
        if (wave->pulseAlpha == 0) {
            height += 1.0f;
            shade = 32.5f * height + 190.0f;
            dst->v.cn[0] = shade;
            dst->v.cn[1] = shade;
            dst->v.cn[2] = shade;
        }
        dst++;
        src++;
    }
    speed = wave->speed;
    if (mode == 0) {
        halfSpeed = (u16)(wave->speed >> 1);
        shade = wave->speedPhaseOffset;
        tmp = (func_150489B0((wave->phase >> 8) + shade) + 1.0f) * 0.5f * halfSpeed;
        speed = halfSpeed + tmp;
    }
    wave->phase -= speed * D_800BE9E4;
    if (wave->pulseAlpha) {
        wave->alphaPhase += D_800BE9E4 * 2;
        obj->alpha = (func_150489B0(wave->alphaPhase + wave->alphaPhaseOffset) + 1.0f) * 0.5f * 205.0f + 50.0f;
    }
    if (uvRate != 0.0f) {
        func_15094AB8(obj->vertices[D_800BE9C0], wave->uv, obj->numVertices, wave->uvAngle, uvArg4, uvArg5);
        wave->uvAngle += uvRate;
        if (wave->uvAngle >= 360.0f) {
            wave->uvAngle -= 360.0f;
        } else if (wave->uvAngle < 0.0f) {
            wave->uvAngle += 360.0f;
        }
    }
}

struct Entry1511D394 {
    f32 followRate;
    f32 accel;
    u16 baseS;
    u16 rangeS;
    u8 phaseOffset;
    u8 padD;
    u16 amplitudeT;
    u8 alphaPhaseOffset;
    u8 alphaPhase;
    u8 pad12[2];
};

/* Texture-scroll settings, indexed by bits 8..15 of the object's param. */
extern struct Entry1511D394 D_80089444[];

/*
 * Scrolls the mesh texture with the wave phase of the owner object (numbered by the low param
 * byte): S eases toward the wave level, T drifts by a cosine amount, and the alpha pulses.
 */
void func_1511D394(WorldObject *obj) {
    struct Wave1511CB44 *ownerWave;
    WorldObject *owner;
    s32 pad; // matching: unused stack slot keeps the original frame layout
    u8 phaseOffset;
    s32 offsetS;
    f32 sine;
    f32 cosine;
    f32 diff;
    s32 rangeS;
    s32 baseS;
    s32 offsetT;
    s32 i;
    s16 *savedS;
    struct Entry1511D394 *entry;
    f32 followRate;
    s32 init;
    s32 id;
    f32 accel;
    s32 wavePhase;

    id = obj->moveData.word & 0xFF;
    owner = func_151149AC(id);
    if (owner == NULL) {
        return;
    }
    ownerWave = WORK(owner)->w1511CB44.wave;
    if (ownerWave == NULL) {
        return;
    }
    entry = &D_80089444[(obj->moveData.word >> 8) & 0xFF];
    rangeS = entry->rangeS;
    baseS = entry->baseS;
    phaseOffset = entry->phaseOffset;
    followRate = entry->followRate;
    accel = entry->accel;
    offsetT = entry->amplitudeT;
    if (WORK(obj)->w1511D394.baseS == NULL) {
        WORK(obj)->w1511D394.baseS = allocate_memory(obj->numVertices * sizeof(s16), 1, 0, 0);
        savedS = WORK(obj)->w1511D394.baseS;
        for (i = 0; i < obj->numVertices; i++) {
            savedS[i] = obj->srcVertices[i].v.tc[0];
        }
        entry->alphaPhase = 0;
        init = TRUE;
    } else {
        init = FALSE;
        savedS = WORK(obj)->w1511D394.baseS;
    }
    wavePhase = ownerWave->phase;
    sine = (func_150489B0(((wavePhase >> 8) & 0xFF) + phaseOffset) + 1.0f) * 0.5f;
    cosine = (func_15048A40(((wavePhase >> 8) & 0xFF) + phaseOffset) + 0.5f) * 0.5f;
    if (init) {
        WORK(obj)->w1511D394.level = sine;
        WORK(obj)->w1511D394.velocity = 0.0f;
    } else {
        diff = (sine - WORK(obj)->w1511D394.level) * followRate;
        if (diff > 0.0f) {
            WORK(obj)->w1511D394.velocity += accel;
            if (diff < WORK(obj)->w1511D394.velocity) {
                WORK(obj)->w1511D394.velocity = diff;
            }
        } else {
            WORK(obj)->w1511D394.velocity -= accel;
            if (WORK(obj)->w1511D394.velocity < diff) {
                WORK(obj)->w1511D394.velocity = diff;
            }
        }
        WORK(obj)->w1511D394.level += WORK(obj)->w1511D394.velocity;
    }
    offsetS = WORK(obj)->w1511D394.level * rangeS;
    offsetS += baseS;
    offsetT = offsetT * cosine;
    if (obj->vertices[D_800BE9C0][0].v.tc[1] > 0x800) {
        offsetT -= 0x800;
    }
    for (i = 0; i < obj->numVertices; i++) {
        obj->vertices[D_800BE9C0][i].v.tc[0] = savedS[i] - offsetS;
        obj->vertices[D_800BE9C0][i].v.tc[1] = obj->vertices[!D_800BE9C0][i].v.tc[1] + offsetT;
    }
    entry->alphaPhase += D_800BE9E4 * 2;
    obj->alpha = (func_150489B0(entry->alphaPhase + entry->alphaPhaseOffset) + 1.0f) * 0.5f * 205.0f + 50.0f;
}

/* Wobbles the mesh texture coordinates in a circle whose S/T amplitudes and speed come from the flags. */
void func_1511D7BC(Game1483E0WaveMesh *mesh) {
    f32 sine;
    s32 index;
    Game1483E0UvPair *coords;
    struct {
        s32 speed;
        s32 amplitude_t;
        s32 amplitude_s;
    } parameters; // matching: grouped in a struct for the original stack layout
    s16 s_delta;
    s16 t_delta;
    f32 cosine;
    u8 angle;
    Game1483E0WaveState *state;

    state = mesh->wave;
    if (state == NULL) {
        state = allocate_memory(sizeof(Game1483E0WaveState), 1, 0, 0);
        mesh->wave = state;
        state->field4 = 0;
        state->field8 = 0;
        coords = allocate_memory(mesh->count * sizeof(Game1483E0UvPair), 1, 0, 0);
        state->coords = coords;
        for (index = 0; index < mesh->count; index++) {
            coords[index].s = mesh->source[index].s;
            coords[index].t = mesh->source[index].t;
        }
    } else {
        coords = mesh->wave->coords;
    }
    parameters.amplitude_s = mesh->flags & 0xFFF;
    parameters.amplitude_t = (mesh->flags >> 12) & 0xFFF;
    parameters.speed = (mesh->flags >> 24) & 0xFF;
    angle = mesh->phase >> 4;
    cosine = func_15048A40(angle);
    sine = func_150489B0(angle);
    mesh->phase += parameters.speed * D_800BE9E4;
    s_delta = parameters.amplitude_s * sine;
    t_delta = parameters.amplitude_t * cosine;
    for (index = 0; index < mesh->count; index++) {
        mesh->buffers[D_800BE9C0][index].s = coords[index].s + s_delta;
        mesh->buffers[D_800BE9C0][index].t = coords[index].t + t_delta;
    }
    func_1511A494((WorldObject *)mesh, &state->field4, &state->field8);
}

/* Rotates the mesh texture coordinates at the speed in the param; places both buffers on first use. */
void func_1511D9E4(WorldObject *obj) {
    s16 (*uv)[2];
    s32 uvArg4;
    s32 uvArg5;
    s32 i;
    f32 speed;

    if (WORK(obj)->w1511D9E4.uv == NULL) {
        uv = WORK(obj)->w1511D9E4.uv = allocate_memory(obj->numVertices * 4, 1, 0, 0);
        for (i = 0; i < obj->numVertices; i++) {
            uv[i][0] = obj->srcVertices[i].v.tc[0] - 0x2000;
            uv[i][1] = obj->srcVertices[i].v.tc[1] - 0x2000;
        }
        for (i = 0; i < obj->numVertices; i++) {
            obj->vertices[0][i].v.ob[0] = obj->vertices[1][i].v.ob[0] = obj->srcVertices[i].v.ob[0] + obj->x;
            obj->vertices[0][i].v.ob[1] = obj->vertices[1][i].v.ob[1] = obj->srcVertices[i].v.ob[1] + obj->y;
            obj->vertices[0][i].v.ob[2] = obj->vertices[1][i].v.ob[2] = obj->srcVertices[i].v.ob[2] + obj->z;
        }
    } else {
        uv = WORK(obj)->w1511D9E4.uv;
    }
    uvArg4 = obj->moveData.word >> 24;
    uvArg5 = (obj->moveData.word >> 16) & 0xFF;
    speed = (f32)(s16)(obj->moveData.word & 0xFFFF) * 0.01;
    WORK(obj)->w1511D9E4.uvAngle += speed * D_800BE9E4;
    func_15094AB8(obj->vertices[D_800BE9C0], uv, obj->numVertices, WORK(obj)->w1511D9E4.uvAngle, uvArg4, uvArg5);
}

struct Swing1511DBC4 {
    f32 time;
    f32 velocity;
    f32 accel;
    f32 jerk;
};

/*
 * Opens the object to 90 degrees with ever-increasing acceleration, wobbles slightly while open,
 * and closes one degree per frame.
 */
void func_1511DBC4(WorldObject *obj) {
    s32 mode;
    struct Swing1511DBC4 *swing;
    f32 wobble;

    if (WORK(obj)->w1511DBC4.swing == NULL) {
        swing = allocate_memory(sizeof(struct Swing1511DBC4), 1, 0, 0);
        WORK(obj)->w1511DBC4.swing = swing;
        swing->time = 0.0f;
        swing->velocity = 0.0f;
        swing->accel = 0.0f;
        swing->jerk = 0.02500000037f;
    } else {
        swing = WORK(obj)->w1511DBC4.swing;
    }

    mode = obj->task_flag & TASK_FLAG_STATE;
    if (mode == TASK_FLAG_OFF) {
        obj->v = 0.0f;
    } else if (mode == TASK_FLAG_ON) {
        obj->v = 90.0f;
        wobble = cosf(swing->time * 0.01745329238f);
        wobble *= 0.5f;
        obj->v += wobble;
        swing->time += 3.0f * D_800BE9E4;
    } else if (mode == TASK_FLAG_GOING_ON) {
        if (obj->v == 0.0f) {
            swing->velocity = 0.0f;
            swing->accel = 0.0f;
        }
        swing->accel += swing->jerk;
        swing->velocity += swing->accel;
        obj->v += swing->velocity;
        if (obj->v >= 90.0f) {
            mode = TASK_FLAG_ON;
            obj->v = 90.0f;
        }
    } else {
        if (D_800BE9E4 < obj->v) {
            obj->v -= D_800BE9E4;
        } else {
            obj->v = 0.0f;
            mode = TASK_FLAG_OFF;
        }
    }

    obj->task_flag &= ~TASK_FLAG_STATE;
    obj->task_flag |= mode;
}

struct Pulse1511DF6C {
    u16 phaseX;
    u16 phaseZ;
    u16 state;
    u16 bouncePhase;
    f32 bounce;
    f32 fullScale;
};

/* The object returned by func_1509B570(0xBA). */
struct Owner1511DF6C {
    u8 pad0[0x1FE4];
    s32 unk1FE4;
};


/* Plays the impact sound for this object type, with extra sounds for characters 0x3B and 0x80. */
void func_1511DD98(WorldObject *obj, Actor *character) {
    s32 sound;
    s32 volume;
    s32 isChar3B;
    s32 isChar80;

    volume = 26000;
    isChar3B = character->character_type == 0x3B;
    isChar80 = character->character_type == 0x80;
    sound = 0;
    switch (obj->modelId) {
    case 0x39:
    case 0x5C:
    case 0x5D:
    case 0x5E:
        if (isChar3B) {
            func_10010630(0x542, character, 26000, 100, 1000);
        } else if (isChar80) {
            func_10010630(0x567, character, 26000, 100, 1000);
        }
        sound = 0x2B0;
        break;
    case 0x3A:
        if (isChar3B) {
            sound = (func_150ADA20() & 1) + 0x53E;
        } else if (isChar80) {
            sound = 0x567;
        } else {
            sound = 0x60A;
        }
        break;
    case 0x3B:
    case 0x51:
        sound = 0x181;
        break;
    case 0x3C:
    case 0x6A:
        sound = 0xB1;
        break;
    case 0x3D:
    case 0x6B:
        sound = 0x442;
        break;
    case 0x3E:
        sound = 0x444;
        break;
    case 0x46:
        sound = 0x2F0;
        break;
    case 0x47:
        sound = 0x4A3;
        break;
    case 0x50:
    case 0x55:
        sound = (func_150ADA20() & 1) + 0xB;
        break;
    case 0x52:
    case 0x58:
        sound = 0x510;
        break;
    case 0x53:
        sound = 0x2AF;
        if (isChar80) {
            func_10010558(0x567, character, 32000, 100, 1000, 30);
        }
        break;
    case 0x56:
        if (isChar80) {
            func_10010630(0x567, character, 26000, 100, 1000);
        }
        sound = 0x5AE;
        break;
    case 0x57:
    case 0x6D:
    case 0x6E:
        sound = 0x51;
        volume = 9000;
        break;
    case 0x64:
    case 0x65:
        sound = 0x629;
        volume = 0x7FFF;
        break;
    }
    if (sound != 0) {
        func_10010630(sound, character, volume, 100, 1000);
    }
}

/*
 * Pulses the object's scale and wobbles it; when a character (filtered by the "filter" field)
 * touches it, plays the impact sound, notifies func_151D6970 and shrinks it away.
 */
void func_1511DF6C(WorldObject *obj) {
    f32 dist;
    s32 hp; // matching: the stored copy of unk1FE4 steers register allocation
    f32 radius;
    f32 scale;
    s16 height;
    s16 unused1;
    s16 unused2;
    struct Pulse1511DF6C *pulse;
    s32 skip;
    s32 anim;
    s32 i;
    Actor *character;
    u8 *data; // matching: one pointer for both lookups; a second local grows the frame
    f32 y;

    if (WORK(obj)->w1511DF6C.pulse == NULL) {
        WORK(obj)->w1511DF6C.pulse = allocate_memory(sizeof(struct Pulse1511DF6C), 1, 0, 0);
        pulse = WORK(obj)->w1511DF6C.pulse;
        bzero(pulse, sizeof(struct Pulse1511DF6C));
        pulse->fullScale = obj->xScale;
        obj->zScale = 0.0f;
        obj->yScale = 0.0f;
        obj->xScale = 0.0f;
    } else {
        pulse = WORK(obj)->w1511DF6C.pulse;
    }
    skip = FALSE;
    anim = -1;
    switch (obj->modelId) {
    case 0x59:
    case 0x5A:
        break;
    case 0x54:
        anim = 0x17;
        if (level == 45) {
            data = func_1509B570(0xBA);
            if (data != NULL && (hp = ((struct Owner1511DF6C *)data)->unk1FE4) <= 0) {
                skip = TRUE;
            }
        }
        /* fallthrough */
    default:
        obj->u += D_800BE9E4 * 1.519999981f;
        dist = cosf(pulse->phaseX * 9.587380191e-05f);
        obj->t = 8.0f * dist;
        dist = cosf(pulse->phaseZ * 9.587380191e-05f);
        obj->yOffset = 12.0f * dist;
        pulse->phaseX += D_800BE9E4 * 507.0f;
        pulse->phaseZ += D_800BE9E4 * 419.0f;
        break;
    }
    if (pulse->state < 3 && !skip) {
        for (i = 0; i < D_8008FD8C; i++) {
            character = &movement_obj[i];
            if (character->type == 0 || character->energy <= 0 || character->playernumber == 0xFF) {
                continue;
            }
            if (character->unk300 == 0) {
                continue;
            }
            if ((WORK(obj)->w1511DF6C.filter == 1 && character->team != 0) || (WORK(obj)->w1511DF6C.filter == 2 && character->team == 0) || (WORK(obj)->w1511DF6C.filter == 3 && (character->team & 1))) {
                continue;
            }
            if (anim != -1 && func_1503195C(character, anim, 0) != NULL) {
                continue;
            }
            dist = (obj->x - character->xpos) * (obj->x - character->xpos) + (obj->z - character->zpos) * (obj->z - character->zpos);
            data = (u8 *)character->mv;
            radius = 100.0f;
            scale = (((ActorCtl *)data)->unk1AF & 0xFF) * 0.015625f;
            if (((ActorCtl *)data)->bot != 0) {
                scale *= 1.399999976f;
            }
            radius *= scale;
            if (dist < radius * radius) {
                func_1507C3E0(character, &height, &unused1, &unused2);
                y = obj->y;
                if (y < character->ceilingY && character->ypos < y + radius && y - radius < character->ypos + height) {
                    WORK(obj)->w1511DF6C.lastCharacter = character->unique_id;
                    pulse->state = 3;
                    func_1511DD98(obj, character);
                    func_151D6970(character, obj);
                    break;
                }
            }
        }
    }
    if (pulse->state == 0) {
        obj->zScale += 0.2599999905f * pulse->fullScale;
        if (pulse->fullScale <= obj->zScale) {
            obj->zScale = pulse->fullScale;
            pulse->bouncePhase = 0;
            pulse->state++;
            pulse->bounce = 0.3000000119f;
        }
        obj->yScale = obj->zScale;
        obj->xScale = obj->zScale;
    } else if (pulse->state == 1) {
        pulse->bounce -= 0.01999999955f * pulse->fullScale;
        if (pulse->bounce <= 0.0f) {
            pulse->state++;
            pulse->bounce = 0.0f;
        }
        obj->zScale = cosf(pulse->bouncePhase * 9.587380191e-05f) * pulse->bounce + pulse->fullScale;
        obj->xScale = obj->yScale = obj->zScale;
        pulse->bouncePhase += D_800BE9E4 * 0x1613;
    } else if (pulse->state == 2) {
    } else if (pulse->state == 3) {
        obj->zScale -= 0.2599999905f * pulse->fullScale;
        if (obj->zScale <= 0.0f) {
            pulse->state = 0;
            obj->inactive = 1;
            obj->zScale = 0.0f;
        }
        obj->yScale = obj->zScale;
        obj->xScale = obj->zScale;
    }
}

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

    motion = prop->task_flag & TASK_FLAG_STATE;
    soundSlot = -1;
    if (WORK(prop)->slide.triggered == 1) {
        if (motion == TASK_FLAG_OFF || motion == TASK_FLAG_GOING_OFF) {
            motion = TASK_FLAG_GOING_ON;
            soundSlot = PROP_SOUND_OPEN;
        }
        speed = 1.0f;
    } else if (WORK(prop)->slide.triggered == 0) {
        if (motion == TASK_FLAG_ON || motion == TASK_FLAG_GOING_ON) {
            motion = TASK_FLAG_GOING_OFF;
            soundSlot = PROP_SOUND_CLOSE;
        }
        speed = -1.0f;
    } else {
        speed = 90.0f;
    }
    if (soundSlot != -1) {
        sound = prop->moveData.word & 0xFFFF; // matching: the row index must share sound's register
        sound = (&D_80089260[sound].open)[soundSlot];
        if (sound > 0) {
            func_15114D24(prop, sound, 24000, 200, 2500, 0);
        }
    }
    speed *= D_800BE9E4;
    range = 70.0f;
    rate = 6.0f;
    switch (prop->moveData.word >> 16) {
    case 6:
        rate = 3.0f;
        range = 82.0f;
        break;
    case 2:
    case 4:
        range = 80.0f;
        break;
    }
    switch (prop->moveData.word >> 16) {
    case 0:
        if (WORK(prop)->slide.started == 0) {
            WORK(prop)->slide.home = prop->v;
        }
        prop->v -= speed * rate;
        if (WORK(prop)->slide.home < prop->v) {
            prop->v = WORK(prop)->slide.home;
            motion = TASK_FLAG_OFF;
        }
        if (prop->v < -110.0f) {
            prop->v = -110.0f;
            motion = TASK_FLAG_ON;
        }
        break;
    case 3:
        if (WORK(prop)->slide.started == 0) {
            WORK(prop)->slide.home = prop->u;
        }
        prop->u -= speed * 3.0f;
        if (prop->u < 260.0f) {
            prop->u = 260.0f;
            motion = TASK_FLAG_ON;
        }
        if (WORK(prop)->slide.home < prop->u) {
            prop->u = WORK(prop)->slide.home;
            motion = TASK_FLAG_OFF;
        }
        break;
    case 2:
    case 8:
        if (WORK(prop)->slide.started == 0) {
            WORK(prop)->slide.home = prop->u;
        }
        prop->u += speed * rate;
        if (WORK(prop)->slide.home + range < prop->u) {
            prop->u = WORK(prop)->slide.home + range;
            motion = TASK_FLAG_ON;
        }
        if (prop->u < WORK(prop)->slide.home) {
            prop->u = WORK(prop)->slide.home;
            motion = TASK_FLAG_OFF;
        }
        break;
    case 4:
    case 6:
    case 7:
        if (WORK(prop)->slide.started == 0) {
            WORK(prop)->slide.home = prop->u;
        }
        prop->u -= speed * rate;
        if (WORK(prop)->slide.home < prop->u) {
            prop->u = WORK(prop)->slide.home;
            motion = TASK_FLAG_OFF;
        }
        if (prop->u < WORK(prop)->slide.home - range) {
            prop->u = WORK(prop)->slide.home - range;
            motion = TASK_FLAG_ON;
        }
        break;
    case 10:
        WORK(prop)->wobble.phase += 0.00999999977648f;
        /* fallthrough */
    case 11:
        WORK(prop)->wobble.phase += 0.0399999991059f * D_800BE9E4;
        prop->v = sinf(WORK(prop)->wobble.phase) * 10.0f;
        if (WORK(prop)->wobble.phase > 12.5663709641f) { /* 4 pi */
            WORK(prop)->wobble.phase -= 12.5663709641f * ((func_150ADA20() & 4) + 1); // u32 multiplier
            func_15114D24(prop, 0x503, 8000, 150, 600, 0);
        }
        break;
    case 12:
        WORK(prop)->wobble.phase += 0.00999999977648f;
        /* fallthrough */
    case 13:
        WORK(prop)->wobble.phase += 0.0399999991059f * D_800BE9E4;
        // matching: computed in double precision (180.0, not 180.0f)
        prop->v = 180.0 - sinf(WORK(prop)->wobble.phase) * 10.0f;
        break;
    }
    WORK(prop)->slide.started = TRUE;
    prop->task_flag &= ~TASK_FLAG_STATE;
    prop->task_flag |= motion;
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
    if (prop->moveData.word != 0) {
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
    direction = 1 - (prop->unique_id & 1);

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
