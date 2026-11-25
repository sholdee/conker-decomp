#include <ultra64.h>
// functions.h declares func_15048A40 as returning void
#define func_15048A40 func_15048A40_void_decl_in_functions_h
#include "functions.h"
#undef func_15048A40
#include "variables.h"

/* One F3DEX2 display-list command, read a byte at a time. */
typedef struct PropGfx {
    /* 0x0 */ s8 opcode;
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ u32 w1;
} PropGfx;

/*
 * Vtx_t with named coordinates. matching: Vtx_t's ob[] subscripts make IDO schedule the coordinate
 * loads in a different order.
 */
typedef struct PropVtx {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];
    /* 0xC */ u8 cn[4];
} PropVtx;

/* Behaviour scratch state of the props in this file (WorldObject.work). */
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
} PropWork;

#define WORK(prop) ((PropWork *)&(prop)->work)

/* Motion states kept in bits 0-1 of WorldObject.behaviorState. */
#define PROP_REST 0
#define PROP_RETURNING 1
#define PROP_MOVING 2
#define PROP_ARRIVED 3

#define PROP_SET_MOTION(prop, motion) \
    ((prop)->behaviorState &= ~3, (prop)->behaviorState |= (motion))

extern void func_15104A80(WorldObject *prop);

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

    motion = prop->behaviorState & 3;
    flag4 = prop->behaviorState & 4;
    kind = prop->modelId;
    travel = 40;
    step = 3;
    autoReturn = FALSE;
    onArrive = NULL;
    if (kind == 0x800B && D_800BE9F0 == 12) {
        travel = 34;
    } else if (D_800BE9F0 == 49) {
        travel = 20;
        autoReturn = TRUE;
    } else if (D_800BE9F0 == 10) {
        travel = 20;
        autoReturn = TRUE;
    } else if (D_800BE9F0 == 52) {
        travel = 10;
        autoReturn = TRUE;
        step = 6;
        onArrive = func_15104A80;
    } else if (kind == 4) {
        travel = 15;
    }
    if (WORK(prop)->lift.homeY == 0) {
        WORK(prop)->lift.homeY = prop->y | 0x80000000;
    }
    homeY = (s16)(WORK(prop)->lift.homeY & 0xFFFF);
    if (motion == PROP_ARRIVED) {
        lowered = homeY - travel;
        if (prop->y != lowered) {
            prop->y = lowered;
        }
        if (autoReturn && !(prop->flags & 4) && !flag4) {
            motion = PROP_RETURNING;
        }
    } else if (motion == PROP_REST) {
        lowered = homeY - travel;
        if (prop->y != lowered) {
            prop->y = homeY;
        }
    } else if (motion == PROP_MOVING) {
        lowered = homeY - travel;
        prop->y -= step;
        if (lowered >= prop->y) {
            prop->y = lowered;
            motion = PROP_ARRIVED;
            if (onArrive != NULL) {
                onArrive(prop);
            }
        }
    } else if (motion == PROP_RETURNING) {
        prop->y += 3;
        if (prop->y >= homeY) {
            prop->y = homeY;
            motion = PROP_REST;
        }
    }
    PROP_SET_MOTION(prop, motion);
}

s32 func_150AD9A0(s32 dx, s32 dy, s32 dz);

/*
 * Slides the prop out from its saved home position along its rotated Z axis by the signed
 * distance in params.hi (MOVING -> ARRIVED), waits params.lo frames, then slides back.
 */
void func_151179BC(WorldObject *prop) {
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

    motion = prop->behaviorState & 3;
    distance = (prop->param.word >> 16) & 0xFFFF;
    delay = prop->param.word & 0xFFFF;
    if (WORK(prop)->slide.homeZ == 0) {
        WORK(prop)->slide.homeXY = (prop->x & 0xFFFF) | (prop->y << 16);
        WORK(prop)->slide.homeZ = prop->z | 0x80000000;
    }
    homeX = WORK(prop)->slide.homeXY & 0xFFFF;
    homeY = WORK(prop)->slide.homeXY >> 16;
    homeZ = WORK(prop)->slide.homeZ & 0xFFFF;
    if (motion == PROP_REST) {
        prop->x = homeX;
        prop->y = homeY;
        prop->z = homeZ;
        return;
    }
    current = func_150AD9A0(homeX - prop->x, homeY - prop->y, homeZ - prop->z);
    if (motion == PROP_ARRIVED) {
        if (distance < 0) {
            offset = -distance;
        } else {
            offset = distance;
        }
        if (delay != 0 && !(prop->flags & 4) && !(prop->behaviorState & 4)) {
            if (D_800BE9E4 < WORK(prop)->slide.timer) {
                WORK(prop)->slide.timer -= D_800BE9E4;
            } else {
                motion = PROP_RETURNING;
            }
        } else {
            WORK(prop)->slide.timer = delay;
        }
    } else if (motion == PROP_MOVING) {
        offset = current + 6;
        if (offset >= ((distance < 0) ? -distance : distance)) {
            motion = PROP_ARRIVED;
            if (distance < 0) {
                offset = -distance;
            } else {
                offset = distance;
            }
            WORK(prop)->slide.timer = delay;
        }
    } else if (motion == PROP_RETURNING) {
        if (current < 3) {
            offset = 0;
            motion = PROP_REST;
        } else {
            offset = current - 3;
        }
    }
    if (current != offset) {
        cx = cosf(prop->rotX * 0.01745329238f);
        sx = sinf(prop->rotX * 0.01745329238f);
        cy = cosf(prop->rotY * 0.01745329238f);
        sy = sinf(prop->rotY * 0.01745329238f);
        cz = cosf(prop->rotZ * 0.01745329238f);
        sz = sinf(prop->rotZ * 0.01745329238f);
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

/* Starts the prop moving when an object with interaction_state 1 triggers it while it is at rest. */
void func_15117D3C(WorldObject *prop, struct127 *obj) {
    s32 delay = prop->param.word & 0xFFFF;

    if (obj->interaction_state == 1 && (prop->flags & 4) && !(prop->behaviorState & 3) && !(prop->behaviorState & 4) &&
        (delay == 0 || obj->unk31C->unk57 == 1)) {
        PROP_SET_MOTION(prop, PROP_MOVING);
    }
}

extern WorldObject *func_151149AC(u32 id);
extern void func_1511F31C(WorldObject *prop);

/* Rotates the local point (x, y, z) by the prop's X then Y rotation and adds its position. */
void func_15117DA4(WorldObject *prop, f32 x, f32 y, f32 z, f32 *out) {
    f32 angle;
    f32 s;
    f32 c;
    f32 rotatedX;

    if (prop->modelId == 0x8006 && D_800BE9F0 == 53) {
        prop = func_151149AC(0xFD);
        func_1511F31C(prop);
        y = -555.0f;
        x = 0.0f;
        z = 0.0f;
    }

    angle = prop->rotX * 0.01745329238f;
    s = sinf(angle);
    c = cosf(angle);
    out[0] = x;
    out[1] = (y * c) - (z * s);
    out[2] = (y * s) + (z * c);

    angle = prop->rotY * 0.01745329238f;
    s = sinf(angle);
    c = cosf(angle);
    rotatedX = out[0];
    out[0] = (out[2] * s) + (rotatedX * c);
    out[2] = (out[2] * c) + (-rotatedX * s);

    out[0] += prop->x;
    out[1] += prop->y;
    out[2] += prop->z;
}

/* struct126 (struct127.unk31C) with the fields used here that structs.h leaves as padding. */
typedef struct ObjControl {
    /* 0x00 */ u8 pad0[0x17];
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 pad18[0xF];
    /* 0x27 */ u8 unk27;
    /* 0x28 */ u8 pad28[0x43];
    /* 0x6B */ u8 grabState;
    /* 0x6C */ s16 holdX;
    /* 0x6E */ s16 holdY;
    /* 0x70 */ s16 holdZ;
} ObjControl;

#define OBJ_CONTROL(obj) ((ObjControl *)(obj)->unk31C)

extern void func_15060A30(s32, struct127 *);
extern void func_15114D24(WorldObject *prop, s32 sound, s32, s32, s32, s32);

/*
 * A hinged prop that grabs an object close to its anchor point, swings rotX up to a level-dependent
 * angle carrying the object along, then swings back down.
 */
void func_15117F3C(WorldObject *prop) {
    f32 maxAngle;
    s32 pad1; // matching: pad1-pad4 are unused stack slots of the original frame
    f32 speed;
    f32 accel;
    struct127 *obj;
    ObjControl *control;
    s32 i;
    f32 dx;
    f32 dy;
    s32 stayOpen;
    s32 sound;
    f32 rangeSq;
    s32 pad2;
    struct127 *target;
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
    motion = prop->behaviorState & 3;
    flag4 = prop->behaviorState & 4;
    stayOpen = prop->behaviorState & 8;
    switch (D_800BE9F0) {
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
    anchorX *= prop->scaleX;
    anchorY *= prop->scaleY;
    anchorZ *= prop->scaleZ;
    rangeSq *= prop->scaleZ * prop->scaleZ;
    if (WORK(prop)->hatch.counters == 0) {
        WORK(prop)->hatch.counters = 2;
        if (motion == PROP_ARRIVED) {
            prop->rotX = maxAngle;
        }
    }
    timer = (WORK(prop)->hatch.counters >> 8) & 0xFF;
    accel = WORK(prop)->hatch.accel;
    speed = WORK(prop)->hatch.speed;
    started = WORK(prop)->hatch.counters & 1;
    if (started) {} // matching: keeps the early read of started
    func_15117DA4(prop, anchorX, anchorY, anchorZ, anchor);
    target = NULL;
    if (motion != PROP_ARRIVED && motion != PROP_RETURNING) {
        for (i = 0; i < D_8008FD8C; i++) {
            obj = &D_800CC2D0[i];
            if (obj->unk65) {
                continue;
            }
            if (obj->stunned) {
                continue;
            }
            control = OBJ_CONTROL(obj);
            if (control->unk17) {
                continue;
            }
            if (control->unk27) {
                continue;
            }
            if (control->grabState >= 4) {
                continue;
            }
            if (obj->y_position + 40.0f < anchor[1]) {
                dy = (obj->y_position + (f32)obj->unkE6) - anchor[1];
                dz = obj->z_position - anchor[2];
                dx = obj->x_position - anchor[0];
                distSq = dz * dz + (dx * dx + dy * dy);
                if (distSq) {} // matching: keeps distSq in its own register
                if (distSq < rangeSq) {
                    target = obj;
                    break;
                }
            }
        }
    }
    if (target == NULL && motion == PROP_MOVING) {
        for (i = 0; i < D_8008FD8C; i++) {
            obj = &D_800CC2D0[i];
            if (OBJ_CONTROL(obj)->grabState != 0) {
                target = obj;
                break;
            }
        }
    }
    if (target != NULL || motion == PROP_MOVING) {
        if (target != NULL && started == 0) {
            func_15060A30(0x16B, target);
        }
        if (flag4 == 0) {
            motion = PROP_MOVING;
            if (started == 0) {
                timer = 50;
                started++;
                accel = 0.0f;
            }
            if (target != NULL) {
                if (timer != 0) {
                    OBJ_CONTROL(target)->grabState = 1;
                } else {
                    OBJ_CONTROL(target)->grabState = 2;
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
                    if (prop->sound == 0) {
                        func_15114D24(prop, sound, 0x7D00, 0x5DC, 0xDAC, 0);
                    }
                }
                prop->rotX += speed;
                if (maxAngle <= prop->rotX) {
                    prop->rotX = maxAngle;
                    speed = 0.0f;
                    accel = 0.0f;
                    motion = PROP_ARRIVED;
                    started = 0;
                    func_15114D24(prop, 0x14, 0x7D00, 0x5DC, 0xDAC, 4);
                    if (target != NULL) {
                        OBJ_CONTROL(target)->grabState = 0;
                    }
                }
            }
            if (target != NULL) {
                func_15117DA4(prop, anchorX, anchorY, anchorZ, anchor);
                OBJ_CONTROL(target)->holdX = anchor[0];
                OBJ_CONTROL(target)->holdY = anchor[1];
                OBJ_CONTROL(target)->holdZ = anchor[2];
            }
        } else {
            if (target != NULL && (control = OBJ_CONTROL(target)) != NULL) {
                if (started == 0 && control->grabState == 0) {
                    timer = 50;
                    started++;
                }
                if (timer != 0) {
                    control->grabState = 3;
                } else if (control->grabState == 3) {
                    control->grabState = 4;
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
            if (OBJ_CONTROL(target) != NULL) {
                func_15117DA4(prop, anchorX, anchorY, anchorZ, anchor);
                OBJ_CONTROL(target)->holdX = anchor[0];
                OBJ_CONTROL(target)->holdY = anchor[1];
                OBJ_CONTROL(target)->holdZ = anchor[2];
            }
        }
    } else if (stayOpen == 0 || motion == PROP_RETURNING) {
        if (0.0f < prop->rotX) {
            if (started == 0) {
                accel = 0.1000000015f;
                started++;
            }
            motion = PROP_RETURNING;
            speed += 0.1000000015f;
            if (30.0f < speed) {
                speed = 30.0f;
            }
            accel += 0.05000000075f;
            prop->rotX -= speed;
            if (prop->rotX <= 0.0f) {
                prop->rotX = 0.0f;
                started = 0;
                motion = PROP_REST;
                accel = 0.0f;
                speed = 0.0f;
            }
        }
        if (target != NULL) {
            OBJ_CONTROL(target)->grabState = 0;
        }
    }
    PROP_SET_MOTION(prop, motion);
    WORK(prop)->hatch.counters &= 0xFE;
    WORK(prop)->hatch.counters |= timer << 8;
    WORK(prop)->hatch.counters |= started;
    WORK(prop)->hatch.accel = accel;
    WORK(prop)->hatch.speed = speed;
    prop->flags &= ~0x60;
    if (motion == PROP_MOVING || (motion == PROP_REST && timer != 0)) {
        prop->flags |= 0x20;
    } else {
        prop->flags |= 0; // matching: the no-op OR still emits a load/store pair
    }
}

void func_151189AC(WorldObject *prop, s32 arg1);

/* Door behaviours: func_151189AC with func_1511F990 mode 0 or 3. */
void func_1511896C(WorldObject *prop) {
    func_151189AC(prop, 0);
}

void func_1511898C(WorldObject *prop) {
    func_151189AC(prop, 3);
}

/* Sounds a swinging door plays, indexed by bits 10-15 of its params. */
typedef struct DoorSounds {
    /* 0x0 */ s16 open;
    /* 0x2 */ s16 close;
    /* 0x4 */ s16 opened;
    /* 0x6 */ s16 closed;
    /* 0x8 */ s16 unk8;
    /* 0xA */ s16 unkA;
} DoorSounds;

extern DoorSounds D_80089260[];

extern void func_15173C60(s32, s32);
extern void func_15173C90(s32, s32, s32);
extern void func_1518804C(s32, f32);
extern void func_1511F990(WorldObject *prop, s32);
extern s32 func_15022B08(s32, s32);

/*
 * A door that swings rotY between 0 and the signed angle in the low 10 bits of its params, bouncing
 * at the ends. Bits 24-31 of params pick a light (or similar) faded with the opening.
 */
void func_151189AC(WorldObject *prop, s32 arg1) {
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
    func_1511F990(prop, arg1);
    row = (prop->param.word & 0xFC00) >> 10;
    openAngle = (prop->param.word << 22) >> 22;
    motion = prop->behaviorState & 3;
    target = openAngle;
    angle = prop->rotY;
    speed = WORK(prop)->swing.speed;
    if (motion == PROP_REST || motion == PROP_RETURNING) {
        target = 0.0f;
        if (motion == PROP_RETURNING && (prop->behaviorState & 4)) {
            slow = TRUE;
        }
    }
    if (motion == PROP_REST || motion == PROP_ARRIVED) {
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
                sound = D_80089260[row].close;
            } else {
                sound = D_80089260[row].open;
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
                    motion = PROP_REST;
                    sound = D_80089260[row].closed;
                } else {
                    motion = PROP_ARRIVED;
                    sound = D_80089260[row].opened;
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
            func_15114D24(prop, sound, 0x5DC0, 0x9C4, 0x1194, 0);
        }
    }
    WORK(prop)->swing.speed = speed;
    if (D_800BE9B4 != 0 || angle != prop->rotY) {
        if (D_800C35EA == 1 && func_15022B08(prop - D_800DBEF4, 0) != 0) {
            angle = prop->rotY;
        } else {
            prop->rotY = angle;
        }
        index = (prop->param.word >> 24) & 0xFF;
        if (D_800BE9F0 == 2) {
            func_1000E75C(255.0f - fabsf(((angle - (f32)openAngle) * 255.0f) / (f32)openAngle));
            return;
        }
        if (index != 0) {
            scale = 1.0f - fabsf((fabsf(angle) - (openAngle < 0 ? -openAngle : openAngle)) / (f32)openAngle);
            func_1518804C(index - 1, scale);
            func_15173C60(scale * 255.0f, index - 1);
            if (D_800BE9F0 == 1 && index - 1 == 0) {
                func_15173C90(scale * 255.0f, 0, 0xF8);
            }
        }
    }
}

extern void func_151669A0(s32 x, s32 y, s32 z, f32, u8, s32);

/*
 * A flap that swings rotZ between 0 and params.half.lo degrees, accelerating harder near its
 * ends, with a sound and a puff of particles (func_151669A0) at each end.
 */
void func_15118F24(WorldObject *prop) {
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

    func_1511F990(prop, 1);
    motion = prop->behaviorState & 3;
    openAngle = prop->param.half.lo;
    target = openAngle;
    angle = prop->rotZ;
    speed = WORK(prop)->swing.speed;
    if (motion == PROP_REST || motion == PROP_RETURNING) {
        target = 0.0f;
    }
    if (motion == PROP_REST || motion == PROP_ARRIVED) {
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
                    motion = PROP_REST;
                } else {
                    motion = PROP_ARRIVED;
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
    if (D_800BE9B4 != 0 || angle != prop->rotZ) {
        prop->rotZ = angle;
        index = (prop->param.word >> 24) & 0xFF;
        if (index != 0 && openAngle != 0) {
            scale = 1.0f - fabsf((angle - (f32)openAngle) / (f32)openAngle);
            func_1518804C(index - 1, scale);
            func_15173C60(scale * 255.0f, index - 1);
        }
    }
}

/* Starts the prop moving when an object with interaction_state 1 (and unk65 clear) triggers it at rest. */
void func_151193AC(WorldObject *prop, struct127 *obj) {
    if (obj->interaction_state == 1 && obj->unk65 == 0 && !(prop->behaviorState & 3) && !(prop->behaviorState & 4)) {
        PROP_SET_MOTION(prop, PROP_MOVING);
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
        range = (u16)(prop->param.word >> 16);
        targetY = (s32)(range * position) / 256 + homeY;
        prop->behaviorState &= ~3;
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
            prop->behaviorState |= PROP_REST; // matching: the no-op OR still emits the original lbu/sb
        } else {
            prop->behaviorState |= PROP_ARRIVED;
        }
    }
}

/*
 * Flood-fills group id `group` from `seed` across the triangles of the prop's display list:
 * a vertex of a triangle that contains the seed joins the group when its x*x + z*z exceeds minDistSq.
 */
void func_151194D4(WorldObject *prop, PropVtx *seed, s32 minDistSq, s32 group) {
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
                                func_151194D4(prop, vtx, minDistSq, group);
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

extern f32 func_15048A40(u8 angle);
extern f32 func_150489B0(u8 angle);

/*
 * Splits the prop's outlying vertices (x*x + z*z beyond a kind-dependent radius) into connected
 * groups, rotates each group to face along its average direction and tags each vertex's flag with
 * ((group - 1) & 3) << 8 | that direction, for func_15119938 to sway.
 */
void func_151196D4(WorldObject *prop) {
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
                func_151194D4(prop, vtx, radiusSq, group);
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
            angle = (u8)(s32)(func_150484A0((f32)sumX, (f32)sumZ) * 40.74366379f); /* radians to 1/256 turns */
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

extern s32 func_151EF610(void);
extern struct127 *func_1505EFD0(s32);

/*
 * Sways the vertex groups set up by func_151196D4 (plants or similar) and writes the transformed
 * vertices to this frame's output buffer. The sway settles once an object comes within range.
 */
void func_15119938(WorldObject *prop) {
    f32 distSq;
    PropVtx *vtx;
    PropVtx *out;
    s32 yaw;
    s32 i;
    f32 *bend;
    struct127 *player;
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
    scaleX = prop->scaleX;
    scaleY = prop->scaleY;
    scaleZ = prop->scaleZ;
    yaw = prop->rotY * 0.7111111283f; /* 256 / 360 */
    if (WORK(prop)->sway.ready == FALSE) {
        func_151196D4(prop);
    }
    if (WORK(prop)->sway.timer >= 0x400) {
        WORK(prop)->sway.phase += ((f32)(WORK(prop)->sway.timer - 0x400) - WORK(prop)->sway.phase) * 0.1000000015f * D_800BE9A4;
        if (WORK(prop)->sway.timer != 0x420) {
            WORK(prop)->sway.timer--;
            if (WORK(prop)->sway.timer == 0x400) {
                WORK(prop)->sway.timer = 0;
                WORK(prop)->sway.phase = 0.0f;
            }
        }
    } else {
        WORK(prop)->sway.phase += (0.01953125f + 0.00390625f * (func_151EF610() % 6)) * D_800BE9A4;
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
        player = func_1505EFD0(0);
        if (player != NULL) {
            x = player->x_position - prop->x;
            y = player->y_position - prop->y;
            z = player->z_position - prop->z;
            distSq = x * x + y * y + z * z;
            if (distSq < 3072.0f) {
                if (player->unk31C->unk57 == 1) {
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
