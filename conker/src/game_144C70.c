#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* One F3DEX2 display-list command, read a byte at a time. */
typedef struct PropGfx {
    /* 0x0 */ s8 opcode;
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ u32 w1;
} PropGfx;

/* Behaviour scratch state of the props in this file (WorldObject.touch). */
typedef union {
    struct {
        s32 homeY; /* y | 0x80000000 once saved */
    } lift;
    struct {
        s32 homeXY; /* (y << 16) | x */
        s32 homeZ;  /* z | 0x80000000 once saved */
        s32 timer;
    } slide;
    struct {
        s32 counters; /* (timer << 8) | started */
        f32 accel;
        f32 speed;
    } hatch;
    struct {
        f32 bounce;
        f32 accel;
        f32 speed;
    } swing;
    struct {
        s32 speed;
        u32 position; /* 0-256 along the travel range */
        s32 homeY;    /* y | 0x80000000 once saved */
    } follow;
    struct {
        s32 timer;
        f32 phase;
        s32 ready;
    } sway;
} Work144C70;

#define WORK(prop) ((Work144C70 *)&(prop)->touch)

#define PROP_SET_MOTION(prop, motion) \
    ((prop)->task_flag &= ~TASK_FLAG_STATE, (prop)->task_flag |= (motion))

/*
 * Lowers the prop by a level-dependent distance (state MOVING -> ARRIVED) and raises it back to its
 * saved home height (RETURNING -> REST).
 */
void func_151177C0(WorldObject *prop) {
    s32 kind;
    s32 travel;
    s32 step;
    s32 autoReturn;
    s32 motion;
    void (*onArrive)(WorldObject *);
    s32 flag4;
    s16 lowered;
    s32 homeY;

    motion = prop->task_flag & TASK_FLAG_STATE;
    flag4 = prop->task_flag & TASK_FLAG_LOCKED;
    kind = prop->modelId;
    travel = 40;
    step = 3;
    autoReturn = FALSE;
    onArrive = NULL;
    if (kind == 0x800B && level == 12) {
        travel = 34;
    } else if (level == 49) {
        travel = 20;
        autoReturn = TRUE;
    } else if (level == 10) {
        travel = 20;
        autoReturn = TRUE;
    } else if (level == 52) {
        travel = 10;
        autoReturn = TRUE;
        step = 6;
        onArrive = beam_onPropArrival;
    } else if (kind == 4) {
        travel = 15;
    }
    if (WORK(prop)->lift.homeY == 0) {
        WORK(prop)->lift.homeY = prop->y | 0x80000000;
    }
    homeY = (s16)(WORK(prop)->lift.homeY & 0xFFFF);
    if (motion == TASK_FLAG_ON) {
        lowered = homeY - travel;
        if (prop->y != lowered) {
            prop->y = lowered;
        }
        if (autoReturn && !(prop->object_status & 4) && !flag4) {
            motion = TASK_FLAG_GOING_OFF;
        }
    } else if (motion == TASK_FLAG_OFF) {
        lowered = homeY - travel;
        if (prop->y != lowered) {
            prop->y = homeY;
        }
    } else if (motion == TASK_FLAG_GOING_ON) {
        lowered = homeY - travel;
        prop->y -= step;
        if (lowered >= prop->y) {
            prop->y = lowered;
            motion = TASK_FLAG_ON;
            if (onArrive != NULL) {
                onArrive(prop);
            }
        }
    } else if (motion == TASK_FLAG_GOING_OFF) {
        prop->y += 3;
        if (prop->y >= homeY) {
            prop->y = homeY;
            motion = TASK_FLAG_OFF;
        }
    }
    PROP_SET_MOTION(prop, motion);
}

s32 math_distance3D(s32 dx, s32 dy, s32 dz);

/*
 * Slides the prop out from its saved home position along its rotated Z axis by the signed
 * distance in params.hi (MOVING -> ARRIVED), waits params.lo frames, then slides back.
 */
void prop_slideOut(WorldObject *prop) {
    s16 homeX;
    s16 homeY;
    s16 homeZ;
    s16 distance;
    s32 current;
    s32 offset;
    f32 fOffset;
    f32 cxsy;
    s32 motion;
    s32 delay;
    f32 cx;
    f32 sx;
    f32 cy;
    f32 sy;
    f32 cz;
    f32 sz;

    motion = prop->task_flag & TASK_FLAG_STATE;
    distance = (prop->moveData.word >> 16) & 0xFFFF;
    delay = prop->moveData.word & 0xFFFF;
    if (WORK(prop)->slide.homeZ == 0) {
        WORK(prop)->slide.homeXY = (prop->x & 0xFFFF) | (prop->y << 16);
        WORK(prop)->slide.homeZ = prop->z | 0x80000000;
    }
    homeX = WORK(prop)->slide.homeXY & 0xFFFF;
    homeY = WORK(prop)->slide.homeXY >> 16;
    homeZ = WORK(prop)->slide.homeZ & 0xFFFF;
    if (motion == TASK_FLAG_OFF) {
        prop->x = homeX;
        prop->y = homeY;
        prop->z = homeZ;
        return;
    }
    current = math_distance3D(homeX - prop->x, homeY - prop->y, homeZ - prop->z);
    if (motion == TASK_FLAG_ON) {
        if (distance < 0) {
            offset = -distance;
        } else {
            offset = distance;
        }
        if (delay != 0 && !(prop->object_status & 4) && !(prop->task_flag & TASK_FLAG_LOCKED)) {
            if (D_800BE9E4 < WORK(prop)->slide.timer) {
                WORK(prop)->slide.timer -= D_800BE9E4;
            } else {
                motion = TASK_FLAG_GOING_OFF;
            }
        } else {
            WORK(prop)->slide.timer = delay;
        }
    } else if (motion == TASK_FLAG_GOING_ON) {
        offset = current + 6;
        if (offset >= ((distance < 0) ? -distance : distance)) {
            motion = TASK_FLAG_ON;
            if (distance < 0) {
                offset = -distance;
            } else {
                offset = distance;
            }
            WORK(prop)->slide.timer = delay;
        }
    } else if (motion == TASK_FLAG_GOING_OFF) {
        if (current < 3) {
            offset = 0;
            motion = TASK_FLAG_OFF;
        } else {
            offset = current - 3;
        }
    }
    if (current != offset) {
        cx = cosf(prop->t * 0.01745329238f);
        sx = sinf(prop->t * 0.01745329238f);
        cy = cosf(prop->u * 0.01745329238f);
        sy = sinf(prop->u * 0.01745329238f);
        cz = cosf(prop->v * 0.01745329238f);
        sz = sinf(prop->v * 0.01745329238f);
        if (distance < 0) {
            offset = -offset;
        }
        fOffset = offset;
        cxsy = cx * sy;
        prop->x = homeX + fOffset * ((cxsy * cz) + (sx * sz));
        prop->y = homeY + fOffset * ((cxsy * sz) - (sx * cz));
        prop->z = homeZ + fOffset * cx * cy;
    }
    PROP_SET_MOTION(prop, motion);
}

/* Starts the prop moving when an object with type 1 triggers it while it is at rest. */
void func_15117D3C(WorldObject *prop, Actor *obj) {
    s32 delay = prop->moveData.word & 0xFFFF;

    if (obj->type == 1 && (prop->object_status & 4) && !(prop->task_flag & TASK_FLAG_STATE) && !(prop->task_flag & TASK_FLAG_LOCKED) &&
        (delay == 0 || obj->mv->unk57 == 1)) {
        PROP_SET_MOTION(prop, TASK_FLAG_GOING_ON);
    }
}

/* Rotates the local point (x, y, z) by the prop's X then Y rotation and adds its position. */
void prop_localToWorld(WorldObject *prop, f32 x, f32 y, f32 z, f32 *out) {
    f32 angle;
    f32 s;
    f32 c;
    f32 rotatedX;

    if (prop->modelId == 0x8006 && level == 53) {
        prop = gfx_findObjectById(0xFD);
        func_1511F31C(prop);
        y = -555.0f;
        x = 0.0f;
        z = 0.0f;
    }

    angle = prop->t * 0.01745329238f;
    s = sinf(angle);
    c = cosf(angle);
    out[0] = x;
    out[1] = (y * c) - (z * s);
    out[2] = (y * s) + (z * c);

    angle = prop->u * 0.01745329238f;
    s = sinf(angle);
    c = cosf(angle);
    rotatedX = out[0];
    out[0] = (out[2] * s) + (rotatedX * c);
    out[2] = (out[2] * c) + (-rotatedX * s);

    out[0] += prop->x;
    out[1] += prop->y;
    out[2] += prop->z;
}


/*
 * A hinged prop that grabs an object close to its anchor point, swings rotX up to a level-dependent
 * angle carrying the object along, then swings back down.
 */
void func_15117F3C(WorldObject *prop) {
    f32 maxAngle;
    s32 pad1; // matching: pad1-pad4 are unused stack slots of the original frame
    f32 speed;
    f32 accel;
    Actor *obj;
    ActorCtl *control;
    s32 i;
    f32 dx;
    f32 dy;
    s32 stayOpen;
    s32 sound;
    f32 rangeSq;
    s32 pad2;
    Actor *target;
    s32 pad3;
    f32 anchorX;
    f32 anchorY;
    f32 anchorZ;
    f32 anchor[3];
    s32 pad4;
    s32 motion;
    s32 started;
    s32 flag4;
    s32 timer;
    f32 dz;
    f32 distSq;

    maxAngle = 40.0; // matching: a double literal; 40.0f loads the constant differently
    rangeSq = 40000.0f;
    sound = 0x4B8;
    motion = prop->task_flag & TASK_FLAG_STATE;
    flag4 = prop->task_flag & TASK_FLAG_LOCKED;
    stayOpen = prop->task_flag & 8;
    switch (level) {
    case 1:
    case 41:
        anchorX = -13.0f;
        anchorY = 76.0f;
        anchorZ = 354.0f;
        rangeSq = 10000.0f;
        break;
    case 11:
        anchorX = 70.0f;
        anchorY = 120.0f;
        anchorZ = 329.0f;
        sound = 0x4BA;
        stayOpen = 8;
        break;
    case 23:
        anchorX = 0.0f;
        anchorY = 125.0f;
        anchorZ = 322.0f;
        break;
    case 40:
        maxAngle = 55.0f;
        anchorX = 0.0f;
        anchorY = 99.0f;
        anchorZ = 168.0f;
        stayOpen = 8;
        break;
    case 53:
        maxAngle = 20.0f;
        anchorX = 0.0f;
        anchorY = -328.0f;
        anchorZ = 549.0f;
        break;
    case 47:
        maxAngle = 90.0f;
        rangeSq = 5625.0f;
        anchorX = 0.0f;
        anchorY = 61.0f;
        anchorZ = 104.0f;
        stayOpen = 8;
        break;
    case 60:
        maxAngle = 30; // matching: an int literal; 30.0f loads the constant differently
        rangeSq = 10000.0f;
        anchorX = 22.0f;
        anchorY = 13.0f;
        anchorZ = 419.0f;
        stayOpen = 8;
        break;
    case 65:
        maxAngle = 45.0f;
        if (flag4 == 4) {
            rangeSq = -1.0f;
        }
        anchorX = 0.0f;
        anchorY = 25.0f;
        anchorZ = 336.0f;
        stayOpen = 8;
        break;
    case 64:
        maxAngle = 45.0f;
        anchorX = 0.0f;
        anchorY = 47.0f;
        anchorZ = 72.0f;
        stayOpen = 8;
        rangeSq = 10000.0f;
        break;
    default:
        return;
    }
    anchorX *= prop->xScale;
    anchorY *= prop->yScale;
    anchorZ *= prop->zScale;
    rangeSq *= prop->zScale * prop->zScale;
    if (WORK(prop)->hatch.counters == 0) {
        WORK(prop)->hatch.counters = 2;
        if (motion == TASK_FLAG_ON) {
            prop->t = maxAngle;
        }
    }
    timer = (WORK(prop)->hatch.counters >> 8) & 0xFF;
    accel = WORK(prop)->hatch.accel;
    speed = WORK(prop)->hatch.speed;
    started = WORK(prop)->hatch.counters & 1;
    if (started) {} // matching: keeps the early read of started
    prop_localToWorld(prop, anchorX, anchorY, anchorZ, anchor);
    target = NULL;
    if (motion != TASK_FLAG_ON && motion != TASK_FLAG_GOING_OFF) {
        for (i = 0; i < D_8008FD8C; i++) {
            obj = &movement_obj[i];
            if (obj->attach_char) {
                continue;
            }
            if (obj->stunned) {
                continue;
            }
            control = obj->mv;
            if (control->env_suit) {
                continue;
            }
            if (control->jump_on_char) {
                continue;
            }
            if (control->unk6B >= 4) {
                continue;
            }
            if (obj->ypos + 40.0f < anchor[1]) {
                dy = (obj->ypos + obj->bodyHeight) - anchor[1];
                dz = obj->zpos - anchor[2];
                dx = obj->xpos - anchor[0];
                distSq = dz * dz + (dx * dx + dy * dy);
                if (distSq) {} // matching: keeps distSq in its own register
                if (distSq < rangeSq) {
                    target = obj;
                    break;
                }
            }
        }
    }
    if (target == NULL && motion == TASK_FLAG_GOING_ON) {
        for (i = 0; i < D_8008FD8C; i++) {
            obj = &movement_obj[i];
            if (obj->mv->unk6B != 0) {
                target = obj;
                break;
            }
        }
    }
    if (target != NULL || motion == TASK_FLAG_GOING_ON) {
        if (target != NULL && started == 0) {
            actor_playSound(0x16B, target);
        }
        if (flag4 == 0) {
            motion = TASK_FLAG_GOING_ON;
            if (started == 0) {
                timer = 50;
                started++;
                accel = 0.0f;
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
                }
            } else {
                speed += accel;
                if (30.0f < speed) {
                    speed = 30.0f;
                }
                accel += 0.02999999933f;
                if (0.200000003f < accel) {
                    if (prop->sound_id == 0) {
                        gfx_playObjectSound(prop, sound, 0x7D00, 0x5DC, 0xDAC, 0);
                    }
                }
                prop->t += speed;
                if (maxAngle <= prop->t) {
                    prop->t = maxAngle;
                    speed = 0.0f;
                    accel = 0.0f;
                    motion = TASK_FLAG_ON;
                    started = 0;
                    gfx_playObjectSound(prop, 0x14, 0x7D00, 0x5DC, 0xDAC, 4);
                    if (target != NULL) {
                        target->mv->unk6B = 0;
                    }
                }
            }
            if (target != NULL) {
                prop_localToWorld(prop, anchorX, anchorY, anchorZ, anchor);
                target->mv->holdX = anchor[0];
                target->mv->holdY = anchor[1];
                target->mv->holdZ = anchor[2];
            }
        } else {
            if (target != NULL && (control = target->mv) != NULL) {
                if (started == 0 && control->unk6B == 0) {
                    timer = 50;
                    started++;
                }
                if (timer != 0) {
                    control->unk6B = 3;
                } else if (control->unk6B == 3) {
                    control->unk6B = 4;
                    started = 0;
                }
                if (timer != 0) {
                    if (D_800BE9E4 < timer) {
                        timer -= D_800BE9E4;
                    } else {
                        timer = 0;
                    }
                }
            }
        }
        if (target != NULL) {
            if (target->mv != NULL) {
                prop_localToWorld(prop, anchorX, anchorY, anchorZ, anchor);
                target->mv->holdX = anchor[0];
                target->mv->holdY = anchor[1];
                target->mv->holdZ = anchor[2];
            }
        }
    } else if (stayOpen == 0 || motion == TASK_FLAG_GOING_OFF) {
        if (0.0f < prop->t) {
            if (started == 0) {
                accel = 0.1000000015f;
                started++;
            }
            motion = TASK_FLAG_GOING_OFF;
            speed += 0.1000000015f;
            if (30.0f < speed) {
                speed = 30.0f;
            }
            accel += 0.05000000075f;
            prop->t -= speed;
            if (prop->t <= 0.0f) {
                prop->t = 0.0f;
                started = 0;
                motion = TASK_FLAG_OFF;
                accel = 0.0f;
                speed = 0.0f;
            }
        }
        if (target != NULL) {
            target->mv->unk6B = 0;
        }
    }
    PROP_SET_MOTION(prop, motion);
    WORK(prop)->hatch.counters &= 0xFE;
    WORK(prop)->hatch.counters |= timer << 8;
    WORK(prop)->hatch.counters |= started;
    WORK(prop)->hatch.accel = accel;
    WORK(prop)->hatch.speed = speed;
    prop->object_status &= ~0x60;
    if (motion == TASK_FLAG_GOING_ON || (motion == TASK_FLAG_OFF && timer != 0)) {
        prop->object_status |= 0x20;
    } else {
        prop->object_status |= 0; // matching: the no-op OR still emits a load/store pair
    }
}

/* Door behaviours: prop_swingDoor with prop_registerLink mode 0 or 3. */
void func_1511896C(WorldObject *prop) {
    prop_swingDoor(prop, 0);
}

void func_1511898C(WorldObject *prop) {
    prop_swingDoor(prop, 3);
}


/*
 * A door that swings rotY between 0 and the signed angle in the low 10 bits of its params, bouncing
 * at the ends. Bits 24-31 of params pick a light (or similar) faded with the opening.
 */
void prop_swingDoor(WorldObject *prop, s32 arg1) {
    f32 speed;
    f32 target;
    f32 angle;
    f32 remaining;
    f32 bounce;
    f32 accel;
    s32 motion;
    s32 index;
    s32 openAngle;
    s32 row;
    s32 sound;
    s32 slow;
    f32 scale;

    sound = 0;
    slow = FALSE;
    prop_registerLink(prop, arg1);
    row = (prop->moveData.word & 0xFC00) >> 10;
    openAngle = (prop->moveData.word << 22) >> 22;
    motion = prop->task_flag & TASK_FLAG_STATE;
    target = openAngle;
    angle = prop->u;
    speed = WORK(prop)->swing.speed;
    if (motion == TASK_FLAG_OFF || motion == TASK_FLAG_GOING_OFF) {
        target = 0.0f;
        if (motion == TASK_FLAG_GOING_OFF && (prop->task_flag & TASK_FLAG_LOCKED)) {
            slow = TRUE;
        }
    }
    if (motion == TASK_FLAG_OFF || motion == TASK_FLAG_ON) {
        speed = 0.0f;
        angle = target;
    }
    if (angle != target || speed != 0.0f) {
        /* both arms use the same values (two separate rodata constants in the ROM) */
        if (WORK(prop)->swing.bounce == 0.0f) {
            if (slow) {
                WORK(prop)->swing.bounce = 0.3000000119f;
            } else {
                WORK(prop)->swing.bounce = 0.3000000119f;
            }
        }
        bounce = WORK(prop)->swing.bounce;
        if (WORK(prop)->swing.accel == 0.0f) {
            if (slow) {
                WORK(prop)->swing.accel = 0.07500000298f;
            } else {
                WORK(prop)->swing.accel = 0.07500000298f;
            }
        }
        accel = WORK(prop)->swing.accel;
        if (speed == 0.0f) {
            if (target == 0.0f) {
                sound = gPropSounds[row].close;
            } else {
                sound = gPropSounds[row].open;
            }
        }
        angle += speed;
        remaining = func_15048A70(angle, target);
        if (fabsf(remaining) < fabsf(speed)) {
            speed = -speed * bounce;
            if (fabsf(speed) < 1.0f) {
                speed = 0.0f;
                angle = target;
                if (target == 0.0f) {
                    motion = TASK_FLAG_OFF;
                    sound = gPropSounds[row].closed;
                } else {
                    motion = TASK_FLAG_ON;
                    sound = gPropSounds[row].opened;
                }
            } else {
                accel *= 0.5f;
                WORK(prop)->swing.accel = accel;
            }
        } else if (remaining > 0.0f) {
            speed += accel;
            if (speed > 5.0f) {
                speed = 5.0f;
            }
        } else {
            speed -= accel;
            if (speed < -5.0f) {
                speed = -5.0f;
            }
        }
        if ((remaining < 0 ? -remaining : remaining) > 30.0f && remaining * speed < 0.0f) {
            speed = 0.0f;
        }
        if (angle < -180.0f) {
            angle += 360.0f;
        } else if (angle >= 180.0f) {
            angle -= 360.0f;
        }
        PROP_SET_MOTION(prop, motion);
        if (sound != 0) {
            gfx_playObjectSound(prop, sound, 0x5DC0, 0x9C4, 0x1194, 0);
        }
    }
    WORK(prop)->swing.speed = speed;
    if (D_800BE9B4 != 0 || angle != prop->u) {
        if (D_800C35EA[0] == 1 && func_15022B08(prop - db_Object_array, 0) != 0) {
            angle = prop->u;
        } else {
            prop->u = angle;
        }
        index = (prop->moveData.word >> 24) & 0xFF;
        if (level == 2) {
            func_1000E75C(255.0f - fabsf(((angle - openAngle) * 255.0f) / openAngle));
            return;
        }
        if (index != 0) {
            scale = 1.0f - fabsf((fabsf(angle) - (openAngle < 0 ? -openAngle : openAngle)) / openAngle);
            effect_setColorAnimBlend(index - 1, scale);
            func_15173C60(scale * 255.0f, index - 1);
            if (level == 1 && index - 1 == 0) {
                func_15173C90(scale * 255.0f, 0, 0xF8);
            }
        }
    }
}

/*
 * A flap that swings rotZ between 0 and params.half.lo degrees, accelerating harder near its
 * ends, with a sound and a puff of particles (func_151669A0) at each end.
 */
void prop_swingFlap(WorldObject *prop) {
    f32 speed;
    f32 target;
    f32 angle;
    f32 remaining;
    f32 bounce;
    f32 accel;
    s32 motion;
    s32 index;
    s32 openAngle;
    f32 scale;

    prop_registerLink(prop, 1);
    motion = prop->task_flag & TASK_FLAG_STATE;
    openAngle = prop->moveData.half.lo;
    target = openAngle;
    angle = prop->v;
    speed = WORK(prop)->swing.speed;
    if (motion == TASK_FLAG_OFF || motion == TASK_FLAG_GOING_OFF) {
        target = 0.0f;
    }
    if (motion == TASK_FLAG_OFF || motion == TASK_FLAG_ON) {
        speed = 0.0f;
        angle = target;
    }
    if (angle != target || speed != 0.0f) {
        if (WORK(prop)->swing.bounce == 0.0f) {
            WORK(prop)->swing.bounce = 0.3000000119f;
        }
        bounce = WORK(prop)->swing.bounce;
        if (WORK(prop)->swing.accel == 0.0f) {
            WORK(prop)->swing.accel = 0.007499999832f;
        }
        if (angle > 90.0f) {
            accel = WORK(prop)->swing.accel * (180.0f - angle);
        } else {
            accel = WORK(prop)->swing.accel * angle;
        }
        if (speed == 0.0f) {
            if (target == 0.0f) {
                accel += 0.1000000015f;
            } else {
                func_10010F88(0x4BA, 0x5DC0, 0, 0, 0, prop->x, prop->y, prop->z, 200, 0x9C4);
                accel += 0.1000000015f;
            }
        }
        angle += speed;
        remaining = func_15048A70(angle, target);
        if (fabsf(remaining) < fabsf(speed)) {
            speed = -speed * bounce;
            if (fabsf(speed) < WORK(prop)->swing.accel * 133.3333435f) {
                speed = 0.0f;
                angle = target;
                if (target == 0.0f) {
                    motion = TASK_FLAG_OFF;
                } else {
                    motion = TASK_FLAG_ON;
                    func_10010F88(0x4BB, 0x5DC0, 0, 0, 0, prop->x, prop->y, prop->z, 200, 0x9C4);
                }
            } else {
                WORK(prop)->swing.accel *= 0.5f;
            }
            func_151669A0(prop->x, prop->y, prop->z, 0.15f, 0xFF, 0);
        } else if (remaining > 0.0f) {
            speed += accel;
            if (speed > 10.0f) {
                speed = 10.0f;
            }
        } else {
            speed -= accel;
            if (speed < -10.0f) {
                speed = -10.0f;
            }
        }
        if (angle < 0.0f) {
            angle += 360.0f;
        } else if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        PROP_SET_MOTION(prop, motion);
    }
    WORK(prop)->swing.speed = speed;
    if (D_800BE9B4 != 0 || angle != prop->v) {
        prop->v = angle;
        index = (prop->moveData.word >> 24) & 0xFF;
        if (index != 0 && openAngle != 0) {
            scale = 1.0f - fabsf((angle - openAngle) / openAngle);
            effect_setColorAnimBlend(index - 1, scale);
            func_15173C60(scale * 255.0f, index - 1);
        }
    }
}

/* Starts the prop moving when an object with type 1 (and unk65 clear) triggers it at rest. */
void func_151193AC(WorldObject *prop, Actor *obj) {
    if (obj->type == 1 && obj->attach_char == 0 && !(prop->task_flag & TASK_FLAG_STATE) && !(prop->task_flag & TASK_FLAG_LOCKED)) {
        PROP_SET_MOTION(prop, TASK_FLAG_GOING_ON);
    }
}

/*
 * Moves the prop's height towards home + range * position / 256 at work.follow.speed per frame,
 * where range is the high half of params. Motion becomes ARRIVED on reaching it.
 */
void func_151193F4(WorldObject *prop) {
    s32 speed;
    s32 targetY;
    s32 diff;
    s32 range;
    s32 homeY;
    u32 position;

    if (WORK(prop)->follow.homeY == 0) {
        WORK(prop)->follow.homeY = prop->y | 0x80000000;
    }
    speed = WORK(prop)->follow.speed;
    if (speed != 0) {
        position = WORK(prop)->follow.position;
        homeY = (s16)(WORK(prop)->follow.homeY & 0xFFFF);
        range = (u16)(prop->moveData.word >> 16);
        targetY = (s32)(range * position) / 256 + homeY;
        prop->task_flag &= ~TASK_FLAG_STATE;
        if (targetY != prop->y) {
            diff = prop->y - targetY;
            if (diff < 0) {
                if (-diff < speed) {
                    prop->y = targetY;
                } else {
                    prop->y += speed;
                }
            } else {
                if (diff < speed) {
                    prop->y = targetY;
                } else {
                    prop->y -= speed;
                }
            }
            prop->task_flag |= TASK_FLAG_OFF; // matching: the no-op OR still emits the original lbu/sb
        } else {
            prop->task_flag |= TASK_FLAG_ON;
        }
    }
}

/*
 * Flood-fills group id `group` from `seed` across the triangles of the prop's display list:
 * a vertex of a triangle that contains the seed joins the group when its x*x + z*z exceeds minDistSq.
 */
void prop_floodFillVertexGroup(WorldObject *prop, PropVtx *seed, s32 minDistSq, s32 group) {
    PropGfx *cmd;
    PropVtx *vtx;
    s32 base;
    s32 found;
    u32 i;
    u32 indices;
    u32 bits;

    cmd = (PropGfx *)prop->displayList;
    while (cmd->opcode != (s8)G_ENDDL) {
        switch (cmd->opcode) {
            case G_VTX:
                base = (cmd->w1 & 0xFFFFFF) >> 4;
                break;
            case G_TRI1:
                indices = cmd->w1;
                found = FALSE;
                bits = indices;
                for (i = 0; i < 3; i++) {
                    vtx = &((PropVtx *)prop->srcVertices)[(bits & 0xFF) / 10 + base];
                    if (seed->x == vtx->x && seed->y == vtx->y && seed->z == vtx->z) {
                        found = TRUE;
                        vtx->flag = group;
                    } else {
                        bits >>= 8;
                    }
                }
                if (found) {
                    bits = indices;
                    for (i = 0; i < 3; i++) {
                        vtx = &((PropVtx *)prop->srcVertices)[(bits & 0xFF) / 10 + base];
                        if (vtx->flag == 0) {
                            if (minDistSq < (vtx->z * vtx->z) + (vtx->x * vtx->x)) {
                                vtx->flag = group;
                                prop_floodFillVertexGroup(prop, vtx, minDistSq, group);
                            }
                        }
                        bits >>= 8;
                    }
                }
                break;
        }
        cmd++;
    }
}


/*
 * Splits the prop's outlying vertices (x*x + z*z beyond a kind-dependent radius) into connected
 * groups, rotates each group to face along its average direction and tags each vertex's flag with
 * ((group - 1) & 3) << 8 | that direction, for prop_swayVertexGroups to sway.
 */
void prop_initSwayGroups(WorldObject *prop) {
    PropVtx *vertices;
    PropVtx *vtx;
    f32 sinA;
    f32 cosA;
    s32 radiusSq;
    s32 group;
    s32 i;
    s32 sumX;
    s32 sumZ;
    s32 angle;
    s32 temp;
    u8 back;

    vtx = (PropVtx *)prop->srcVertices;
    radiusSq = 100;
    if (prop->modelId == 0x21 || prop->modelId == 0x22) {
        radiusSq = 400;
    }
    WORK(prop)->sway.ready = TRUE;
    group = 0;
    for (i = 0; i < prop->numVertices; i++) {
        if (vtx->flag == 0) {
            temp = vtx->x;
            temp = (vtx->z * vtx->z) + (temp * temp); // matching: x goes through temp first
            if (radiusSq < temp) {
                group++;
                vtx->flag = group;
                prop_floodFillVertexGroup(prop, vtx, radiusSq, group);
            }
        }
        vtx++;
    }

    for (; group != 0; group--) {
        vertices = (PropVtx *)prop->srcVertices;
        vtx = vertices;
        sumX = 0;
        sumZ = 0;
        for (i = 0; i < prop->numVertices; i++) {
            if (group == vtx->flag) {
                sumX += vtx->x;
                sumZ += vtx->z;
            }
            vtx++;
        }
        if (sumX != 0 || sumZ != 0) {
            vtx = vertices;
            angle = (u8)(s32)(func_150484A0(sumX, sumZ) * 40.74366379f); /* radians to 1/256 turns */
            if (group < angle) {
                back = -angle;
                sinA = func_15048A40(back);
                cosA = func_150489B0(back);
                for (i = 0; i < prop->numVertices; i++) {
                    if (group == vtx->flag) {
                        temp = vtx->x;
                        vtx->flag = (((group - 1) & 3) << 8) | angle;
                        vtx->x = (vtx->z * sinA) + (temp * cosA);
                        vtx->z = (vtx->z * cosA) - (temp * sinA);
                    }
                    vtx++;
                }
            }
        }
    }
}

/*
 * Sways the vertex groups set up by prop_initSwayGroups (plants or similar) and writes the transformed
 * vertices to this frame's output buffer. The sway settles once an object comes within range.
 */
void prop_swayVertexGroups(WorldObject *prop) {
    f32 distSq;
    PropVtx *vtx;
    PropVtx *out;
    s32 yaw;
    s32 i;
    f32 *bend;
    Actor *player;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    f32 x;
    f32 y;
    f32 z;
    f32 sine;
    f32 rot[4][2];
    f32 bendSin;
    f32 bendCos;
    f32 cosine;
    f32 angle;

    vtx = (PropVtx *)prop->srcVertices;
    out = (PropVtx *)prop->vertices[D_800BE9C0];
    posX = prop->x;
    posY = prop->y;
    posZ = prop->z;
    scaleX = prop->xScale;
    scaleY = prop->yScale;
    scaleZ = prop->zScale;
    yaw = prop->u * 0.7111111283f; /* 256 / 360 */
    if (WORK(prop)->sway.ready == FALSE) {
        prop_initSwayGroups(prop);
    }
    if (WORK(prop)->sway.timer >= 0x400) {
        WORK(prop)->sway.phase += ((WORK(prop)->sway.timer - 0x400) - WORK(prop)->sway.phase) * 0.1000000015f * gFrameDelta;
        if (WORK(prop)->sway.timer != 0x420) {
            WORK(prop)->sway.timer--;
            if (WORK(prop)->sway.timer == 0x400) {
                WORK(prop)->sway.timer = 0;
                WORK(prop)->sway.phase = 0.0f;
            }
        }
    } else {
        WORK(prop)->sway.phase += (0.01953125f + 0.00390625f * (func_151EF610() % 6)) * gFrameDelta;
        WORK(prop)->sway.timer -= func_151EF610() % 3;
        if (WORK(prop)->sway.timer < 20) {
            WORK(prop)->sway.timer = 236;
        }
    }
    for (i = 0; i < 4; i++) {
        if (WORK(prop)->sway.timer >= 0x400) {
            rot[i][0] = func_15048A40(WORK(prop)->sway.phase);
            rot[i][1] = func_150489B0(WORK(prop)->sway.phase);
        } else {
            angle = WORK(prop)->sway.phase * 6.283185482f - 0.3926990926f * i;
            if (angle <= 0.0f) {
                angle = 5.0f;
            } else {
                sine = func_15048A40(WORK(prop)->sway.timer >> 1);
                angle = sinf(angle) * (2.0f * sine) + 5.0f;
            }
            rot[i][0] = func_15048A40(angle);
            rot[i][1] = func_150489B0(angle);
        }
    }
    if (WORK(prop)->sway.timer != 0x420) {
        player = actor_findByPlayerNumber(0);
        if (player != NULL) {
            x = player->xpos - prop->x;
            y = player->ypos - prop->y;
            z = player->zpos - prop->z;
            distSq = x * x + y * y + z * z;
            if (distSq < 3072.0f) {
                if (player->mv->unk57 == 1) {
                    WORK(prop)->sway.timer = 0x420;
                } else {
                    WORK(prop)->sway.timer = 0x41F;
                }
            }
        }
    }
    for (i = 0; i < prop->numVertices; i++) {
        if (vtx->flag != 0) {
            sine = func_15048A40(vtx->flag + yaw);
            cosine = func_150489B0(vtx->flag + yaw);
            bend = rot[vtx->flag >> 8];
            bendSin = bend[0];
            bendCos = bend[1];
            z = vtx->y * bendSin + vtx->z * bendCos;
            y = vtx->y * bendCos - vtx->z * bendSin;
            if (y < 1.0f) {
                z -= y;
                y = 1.0f;
            }
            x = vtx->x * cosine + z * sine;
            z = z * cosine - vtx->x * sine;
        } else {
            x = vtx->x;
            y = vtx->y;
            z = vtx->z;
        }
        out->x = x * scaleX + posX;
        out->y = y * scaleY + posY;
        out->z = z * scaleZ + posZ;
        out++;
        vtx++;
    }
}
