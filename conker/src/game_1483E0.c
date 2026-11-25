#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Per-type update routines for 0xA0-byte level objects (the D_800DBEF4 array, WorldObject).
 * Shared layout: rotation f32[3] at 0x0, position s16[3] at 0x10, vertex count at 0x16,
 * vertex buffers at 0x20 (double-buffered by D_800BE9C0) and 0x28 (source), packed param at
 * 0x3C, mode in the low two bits of 0x73 (0 closed, 1 closing, 2 opening, 3 open), alpha at
 * 0x8A, and per-type state at 0x7C-0x87. Each routine views the object through its own struct.
 */

extern f32 func_150489B0(u8);
extern void *allocate_memory(s32, s32, s32, s32);
extern void *func_151149AC(u32);
extern void func_15188010(s32, f32 *);
extern s32 func_15022B08(s32, s32);
extern void func_1511F990(void *, s32);
extern void func_15114D24(void *, s32, s32, s32, s32, s32);
extern void func_1518804C(s32, f32);
extern void func_15173C60(s32, s32);
extern void func_15060A30(s32, void *);
extern void func_1511A494(void *, s32 *, s32 *);

struct Obj1511AF30 {
    f32 rotX;
    u8 pad4[0x38];
    s32 level;
    u8 pad40[0x3C];
    f32 tilt;
    f32 wobble;
    f32 wobbleTarget;
};

/* Eases the tilt toward 0.75 degrees per level, then wobbles randomly around it. */
void func_1511AF30(struct Obj1511AF30 *obj) {
    f32 level;
    f32 target;
    f32 diff;
    s32 roll;

    level = obj->level;
    target = 75.0f * level * 0.009999999776f;
    if (target != obj->tilt) {
        diff = target - obj->tilt;
        if (fabsf(diff) < 1.0f) {
            obj->tilt = target;
        } else {
            obj->tilt += (diff < 0.0f) ? -1 : 1;
        }
        obj->wobble = 0.0f;
    } else {
        roll = func_150ADA20() % 1000;
        if (roll < 500) {
            obj->wobbleTarget = roll * 5.0f * level * 0.002000000095f * 0.009999999776f;
        }
        diff = obj->wobbleTarget - obj->wobble;
        obj->wobble += diff * 0.400000006f;
    }
    obj->rotX = obj->tilt + obj->wobble;
}

struct Obj1511B07C {
    u8 pad0[0x10];
    s16 x;
    u8 pad12[0x2];
    s16 z;
    u8 pad16[0x26];
    s32 param;
    u8 pad40[0x33];
    u8 flags;
    u16 soundHandle;
    u8 pad76[0x6];
    s32 homeX;
    s32 homeZ;
    f32 speed;
};

/* Slides the object between fixed waypoints chosen by its param, with movement sounds. */
void func_1511B07C(struct Obj1511B07C *obj) {
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
    if (D_800C35EA == 1) {
        if (func_15022B08((WorldObject *)obj - D_800DBEF4, 0) != 0) {
            return;
        }
    }
    if (obj->homeX == 0) {
        obj->homeX = obj->x | 0x80000000;
        obj->homeZ = obj->z;
    }
    targetX = (s16)obj->homeX;
    targetZ = obj->homeZ;
    state = obj->param & 0x3FF;
    if (state >= 4 && state < 7) {
        func_1511F990(obj, 1);
    }
    mode = obj->flags & 3;
    if (mode == 3 || mode == 2) {
        switch (state) {
        case 0:
            targetX = -69;
            targetZ = -120;
            if (mode == 2) {
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
            if (mode == 2) {
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
            if (mode == 2) {
                sound = 0x4E;
            }
            break;
        case 5:
            targetZ = 140;
            if (mode == 2) {
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
            if (mode == 2) {
                sound = 0x4E;
            }
            break;
        }
    } else {
        switch (state) {
        case 4:
            maxSpeed = 15.0f;
            moveX = 1;
            if (mode == 1) {
                sound = 0x4E;
            }
            break;
        case 7:
            maxSpeed = 15.0f;
            moveZ = 1;
            if (mode == 1) {
                sound = 0x4E;
            }
            break;
        }
    }
    if (sound != 0) {
        func_15114D24(obj, sound, 0x7D00, 100, 2500, 1);
    }
    speed = obj->speed;
    if (mode == 0 || mode == 3) {
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
    } else if (mode == 2 || mode == 1) {
        // matching: the embedded &= makes IDO store the cleared flags before merging
        obj->flags = ((mode == 2) ? 3 : 0) | (obj->flags &= ~3);
        if (obj->soundHandle != 0) {
            func_100111C8(obj->soundHandle);
            obj->soundHandle = 0;
            func_15114D24(obj, 0x4C8, 0x7D00, 100, 2500, 1);
        }
    }
    obj->speed = speed;
}


struct Obj1511B51C {
    u8 pad0[0x8];
    f32 angle;
    u8 padC[0x30];
    union {
        s32 word;
        struct {
            s16 hi;
            s16 targetAngle;
        } half;
    } param;
    u8 pad40[0x33];
    u8 flags;
    u8 pad74[0x8];
    f32 restitution;
    f32 maxSpeed;
    f32 velocity;
};

/* Swings the angle toward its open/closed target with a damped bounce, driving a sound by it. */
void func_1511B51C(struct Obj1511B51C *obj) {
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

    openAngle = obj->param.half.targetAngle;
    mode = obj->flags & 3;
    target = openAngle;
    angle = obj->angle;
    velocity = obj->velocity;
    if (mode == 0 || mode == 1) {
        target = 0.0f;
    }
    if (mode == 0 || mode == 3) {
        angle = target;
        velocity = 0.0f;
    }
    if (angle != target || velocity != 0.0f) {
        restitution = obj->restitution;
        if (restitution == 0.0f) {
            restitution = 0.4499999881f;
        }
        maxSpeed = obj->maxSpeed;
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
                    mode = 0;
                } else {
                    mode = 3;
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
        obj->flags &= ~3;
        obj->flags |= mode;
    }
    obj->velocity = velocity;
    if (D_800BE9B4 != 0 || angle != obj->angle) {
        obj->angle = angle;
        sound = (obj->param.word >> 24) & 0xFF;
        if (sound != 0 && openAngle != 0) {
            ratio = fabsf((angle - openAngle) / openAngle);
            func_1518804C(sound - 1, ratio);
            func_15173C60((s32)((1.0f - ratio) * 255.0f), sound - 1);
        }
    }
}

struct Obj1511B7D4 {
    u8 pad0[0x8];
    f32 angle;
    u8 padC[0x4];
    s16 x;
    s16 y;
    s16 z;
    u8 pad16[0xA];
    Vtx *buffers[2];
    Vtx *source;
    u8 pad2C[0x10];
    s32 param;
    u8 pad40[0x3C];
    s32 updates;
    f32 prevAngle[2];
    u8 pad88[0x18];
};

/* (vertex, pivot) index pairs: vertices that follow the rotation of a preceding object. */
extern u8 D_800A2F70;
extern u8 D_800A2F71[];

/* Rotates the listed vertices about the pivot objects just before this one in the object array. */
void func_1511B7D4(struct Obj1511B7D4 *obj) {
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
    struct Obj1511B7D4 *pivots[2];

    count = 0;
    if (obj->param == 0) {
        pair = D_800A2F71;
        count = D_800A2F70;
        pivots[0] = obj - 1;
        pivots[1] = obj - 2;
    }
    if (obj->prevAngle[0] != pivots[0]->angle || obj->prevAngle[1] != pivots[1]->angle) {
        obj->prevAngle[0] = pivots[0]->angle;
        obj->prevAngle[1] = pivots[1]->angle;
        obj->updates = 2;
    }
    if (obj->updates != 0) {
        obj->updates--;
        while (count--) {
            dx = pivots[pair[1]]->x - obj->x;
            angle = -pivots[pair[1]]->angle * 0.01745329238f;
            dy = pivots[pair[1]]->y - obj->y;
            sine = sinf(angle);
            cosine = cosf(angle);
            y = obj->source[pair[0]].v.ob[1];
            x = obj->source[pair[0]].v.ob[0];
            z = obj->source[pair[0]].v.ob[2];
            y -= dy;
            x -= dx;
            rotatedY = (s32)(y * cosine - x * sine);
            x = (s32)(y * sine + x * cosine);
            y = rotatedY;
            x += dx;
            y += dy;
            x += obj->x;
            y += obj->y;
            z += obj->z;
            obj->buffers[D_800BE9C0][pair[0]].v.ob[0] = x;
            obj->buffers[D_800BE9C0][pair[0]].v.ob[1] = y;
            obj->buffers[D_800BE9C0][pair[0]].v.ob[2] = z;
            pair += 2;
        }
    }
}


struct Obj1511BA24 {
    u8 pad0[0x3C];
    s32 param;
    u8 pad40[0x4A];
    u8 alpha;
};

/* Sets the alpha from a 0..1 level looked up by the param, never fully transparent. */
void func_1511BA24(struct Obj1511BA24 *obj) {
    f32 level;

    level = 0.0f;
    func_15188010(obj->param, &level);
    obj->alpha = level * 255.0f;
    if (obj->alpha == 0) {
        obj->alpha = 1;
    }
}

typedef struct Game1483E0UvVertex {
    s16 x, y, z;
    u8 unknown06[2];
    s16 s, t;
    u32 unknown0C;
} Game1483E0UvVertex;

typedef struct Game1483E0UvPair {
    s16 s, t;
} Game1483E0UvPair;

typedef struct Game1483E0State {
    u8 unknown00[0x10];
    s16 x, y, z;
    u16 count;
    u8 unknown18[8];
    Game1483E0UvVertex *buffers[2];
    Game1483E0UvVertex *source;
    u8 unknown2C[0x13];
    u8 reference_id;
    u8 unknown40[0x3C];
    Game1483E0UvPair *coords;
    struct127 *reference;
} Game1483E0State;

extern f32 D_800A31D0;
extern f32 D_800A31D4;

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
    s_offset = refX - ((f32)obj->x + D_800A31D0);
    t_offset = refZ - ((f32)obj->z + D_800A31D4);
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
        obj->buffers[D_800BE9C0][i].s = (s16)(((f32)coords[i].s + s_offset) * s_scale + 1024.0f);
        obj->buffers[D_800BE9C0][i].t = (s16)(((f32)coords[i].t + t_offset) * t_scale + 1024.0f);
    }
}

/* Projects the mesh texture under its reference object (or the object numbered reference_id). */
void func_1511BDF4(Game1483E0State *obj) {
    struct127 *reference;

    if (obj->reference != NULL) {
        reference = obj->reference;
    } else {
        reference = func_15083E90(obj->reference_id);
    }
    if (reference != NULL) {
        func_1511BB04(obj, reference->x_position, reference->z_position, 1.0f, 1.0f);
    }
}

struct Obj1511BE5C {
    u8  pad0[0x4];
    f32 rotY;
    u8  pad8[0x8];
    s16 x;
    s16 y;
    s16 z;
};

/* Turns the object to face the camera (yaw in degrees). */
void func_1511BE5C(struct Obj1511BE5C *obj) {
    obj->rotY = func_150484A0((f32)obj->x - D_800DBFF0->unk2F8, (f32)obj->z - D_800DBFF0->unk300) * 57.29577637f;
}

struct Obj1511BEBC {
    u8 pad0[0x10];
    s16 x;
    s16 y;
    s16 z;
    u8 pad16[0x1E];
    f32 scaleZ;
    u8 pad38[0x3B];
    u8 flags;
    u8 pad74[0x8];
    s32 restAndTimer; /* low 16 bits: rest height, high 16 bits: timer */
    s32 triggered;
    f32 velocity;
};

/* Per-character trap state that func_1511BEBC drives while a character is caught. */
struct CharacterTrap {
    u8 pad0[0x6B];
    u8 state;
    s16 x;
    s16 y;
    s16 z;
};

/* The 0x32C-byte character object (struct127) as func_1511BEBC sees it. */
struct TrapCharacter {
    u8 pad0[0x14];
    f32 x;
    f32 y;
    f32 z;
    u8 pad20[0xC6];
    s16 height;
    u8 padE8[0x234];
    struct CharacterTrap *trap;
    u8 pad320[0xC];
};

/* The 0x9A0-byte player (struct108) as func_1511BEBC sees it. */
struct TrapPlayer {
    u8 pad0[0x3D0];
    struct TrapCharacter *character;
    struct CharacterTrap *trap;
    u8 pad3D8[0x9A0 - 0x3D8];
};

extern void func_150DDED0(void *);

/* After a delay, lowers onto a player found beneath it and holds them at its position; rises back to rest otherwise. */
void func_1511BEBC(struct Obj1511BEBC *obj) {
    f32 pos[3];
    s16 restY;
    struct TrapCharacter *target;
    struct TrapCharacter *character;
    struct TrapPlayer *player;
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
    struct CharacterTrap *trap;
    f32 dist;
    void (*onTrigger)(void *);

    onTrigger = func_150DDED0;
    func_1511BE5C((struct Obj1511BE5C *)obj);
    mode = obj->flags & 3;
    flag4 = obj->flags & 4;
    flag8 = obj->flags & 8;
    triggered = obj->triggered;
    velocity = obj->velocity;
    if (D_800BE9F0 == 10) {
        radiusSq = (obj->scaleZ * 100.0f) * (obj->scaleZ * 100.0f);
        if (obj->restAndTimer == 0) {
            if (obj->y == 0) {
                obj->y = 1;
            }
            obj->restAndTimer = (u16)obj->y;
            restY = obj->y;
            if (mode == 3) {
                obj->y = restY;
            }
        }
        restY = (s16)(obj->restAndTimer & 0xFFFF);
        timer = obj->restAndTimer >> 16;
        target = NULL;
        pos[0] = obj->x;
        pos[1] = obj->y + -729.0f; // matching: "- 729.0f" emits sub.s instead of add.s
        pos[2] = obj->z;
        if (mode != 3 && mode != 1) {
            for (i = 0; i < D_8008FD8C; i++) {
                player = &((struct TrapPlayer *)D_800DBFF0)[i];
                if (player->trap->state >= 4) {
                    continue;
                }
                character = player->character;
                if (character->y + 40.0f < pos[1]) {
                    dy = (character->y + (f32)character->height) - pos[1];
                    dz = character->z - pos[2];
                    dx = character->x - pos[0];
                    dist = dz * dz + (dx * dx + dy * dy);
                    if (dist != dist) {} // matching: no-op NaN self-test steers IDO's FP scheduling
                    if (dist < radiusSq) {
                        target = character;
                        break;
                    }
                }
            }
        }
        if (target == NULL && (mode == 2 || mode == 1)) {
            for (i = 0; i < D_8008FD8C; i++) {
                character = &((struct TrapCharacter *)D_800CC2D0)[i];
                if (character->trap->state != 0) {
                    target = character;
                    break;
                }
            }
        }
        if (target != NULL || mode == 2) {
            if (target != NULL && triggered == 0) {
                func_15060A30(0x16B, target);
            }
            if (flag4 == 0) {
                mode = 2;
                if (triggered == 0) {
                    timer = 50;
                    triggered++;
                    if (onTrigger != NULL) {
                        onTrigger(obj);
                    }
                }
                if (target != NULL) {
                    if (timer != 0) {
                        target->trap->state = 1;
                    } else {
                        target->trap->state = 2;
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
                        mode = 3;
                        triggered = 0;
                        velocity = 0.0f;
                        if (target != NULL) {
                            target->trap->state = 0;
                        }
                        func_10010F88(0x1A5, 0x7FFF, 0, 0, 0, obj->x, obj->y, obj->z, 1500, 3500);
                    }
                }
                if (target != NULL) {
                    target->trap->x = pos[0];
                    target->trap->y = pos[1];
                    target->trap->z = pos[2];
                }
            } else if (target != NULL) {
                if (triggered == 0 && target->trap->state == 0) {
                    timer = 50;
                    triggered++;
                }
                trap = target->trap;
                if (timer != 0) {
                    trap->state = 3;
                } else if (trap->state == 3) {
                    trap->state = 4;
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
                target->trap->x = pos[0];
                target->trap->y = pos[1];
                target->trap->z = pos[2];
            }
        } else if (flag8 == 0 || mode == 1) {
            if (obj->y < restY) {
                if (triggered == 0) {
                    triggered++;
                }
                mode = 1;
                velocity += 1.299999952f;
                obj->y = obj->y + velocity;
                if (obj->y >= restY) {
                    obj->y = restY;
                    triggered = 0;
                    mode = 0;
                    velocity = 0.0f;
                }
            }
            if (target != NULL) {
                target->trap->state = 0;
            }
        }
        obj->triggered = triggered;
        obj->flags &= ~3;
        obj->flags |= mode;
        obj->restAndTimer &= 0xFFFF;
        obj->restAndTimer |= timer << 16;
        obj->velocity = velocity;
    }
}

/* Update routine for objects that need none. */
void func_1511C540(void) {
}

struct Obj1511C548 {
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    u8 padC[0x74];
    f32 phaseX;
    f32 phaseZ;
};

/* Sways the object around its X and Z axes by up to 16 degrees. */
void func_1511C548(struct Obj1511C548 *obj) {
    f32 swayZ;
    f32 swayX;

    swayX = sinf(obj->phaseX);
    swayZ = sinf(obj->phaseZ);
    obj->rotX = swayX * 16.0f;
    obj->rotZ = swayZ * 16.0f;
    obj->phaseX += 0.07299999893f * D_800BE9A4;
    obj->phaseZ += 0.06499999762f * D_800BE9A4;
    obj->phaseX = func_15144B68(obj->phaseX);
    {
        f32 wrapped = func_15144B68(obj->phaseZ);

        obj->phaseZ = wrapped;
    }
    if (obj->rotZ == 0.0f && obj->phaseZ == 0.0f) {
        obj->rotX = 7.5f;
        obj->rotZ = 8.0f;
    }
}


struct Obj1511C638 {
    u8 pad0[0x10];
    s16 x;
    s16 y;
    s16 z;
    u8 pad16[0x26];
    s32 param;
    u8 pad40[0x2E];
    u8 unk6E;
    u8 pad6F[4];
    u8 flags;
    u8 pad74[8];
    s32 delay;
    s32 fade;
    u8 pad84[6];
    u8 alpha;
    u8 pad8B[0x15];
};

extern void func_15170F4C(struct Obj1511C638 *, u8, s32);
extern void func_150B2740(u8, u8);
extern s32 func_1000EC24();

/*
 * Fades the object (and the one or two objects after it) in when triggered, playing the
 * effect chosen by the next object's param; turns them off again when reset.
 */
void func_1511C638(struct Obj1511C638 *obj) {
    s32 mode;
    struct Obj1511C638 *next;
    struct Obj1511C638 *third;

    next = &obj[1];
    mode = obj->flags & 3;
    if (D_800BE9F0 == 4) {
        third = &obj[2];
    } else {
        third = NULL;
    }
    if (mode == 1) {
        mode = 0;
        obj->alpha = 0;
        next->unk6E = 0;
        if (third != NULL) {
            third->unk6E = 0;
        }
        if (obj->param != 0xFF) {
            func_15173C60(0, obj->param);
        }
    }
    if (D_800BE9B4 != 0) {
        if (mode == 3) {
            obj->alpha = 0xFF;
            next->unk6E = 1;
            if (third != NULL) {
                third->unk6E = 1;
            }
        } else {
            obj->alpha = 0;
            next->unk6E = 0;
            if (third != NULL) {
                third->unk6E = 0;
            }
        }
    }
    if (mode == 2) {
        if (obj->delay < 0) {
            obj->delay += D_800BE9E4;
        } else {
            if (obj->fade == 0) {
                switch (next->param & 0xFF) {
                case 1:
                    func_15170F4C(next, 0xFF, 0);
                    func_10010F30(0x2BE, 0x7FFF, 64, 0, 0);
                    break;
                case 0: {
                    u16 first = func_150ADA20() % 3U;
                    u16 second;
                    const u16 sounds[3] = { 0x2B6, 0x2BD, 0x2BE };

                    second = ((func_150ADA20() & 1) + first + 1) % 3U;
                    first = sounds[first];
                    second = sounds[second];
                    func_10010F30(0x2B7, 0x7FFF, 64, 0, 0);
                    func_10010F30(first, 0x7FFF, 10, 0, 0);
                    func_10010F30(second, 0x7FFF, 54, 0, 0);
                    first = func_150ADA20() % 5U;
                    second = ((func_150ADA20() & 3) + first + 1) % 5U;
                    func_1000FA64(first + 0x2B1, obj->x, obj->y, obj->z, 0x4000, 10001, 10000, (s32)func_1000EC24, (void *)0x1001E, 0, 0x200, 0);
                    func_1000FA64(second + 0x2B1, obj->x, obj->y, obj->z, 0x4000, 10001, 10000, (s32)func_1000EC24, (void *)0x1001E, 0, 0, 0);
                    first = func_150ADA20() % 5U;
                    second = ((func_150ADA20() & 3) + first + 1) % 5U;
                    func_1000FA64(first + 0x2B1, obj->x, obj->y, obj->z, 0x4000, 10001, 10000, (s32)func_1000EC24, (void *)60, 0, 0x300, 0);
                    func_1000FA64(second + 0x2B1, obj->x, obj->y, obj->z, 0x4000, 10001, 10000, (s32)func_1000EC24, (void *)60, 0, 0x100, 0);
                }
                break;
                case 2:
                    break;
                case 3:
                    func_150B2740(next->param >> 8, 0xFF);
                    break;
                }
            }
            obj->alpha = 0xFF;
            next->unk6E = 1;
            if (third != NULL) {
                third->unk6E = 1;
            }
            obj->fade += D_800BE9E4 * 30;
            if (obj->fade >= 0xFF) {
                obj->fade = 0xFF;
                mode = 3;
            }
            if (obj->param != 0xFF) {
                func_15173C60(obj->fade, obj->param);
            }
            if (mode == 3) {
                obj->delay = 0;
                obj->fade = 0;
            }
        }
    }
    obj->flags &= ~3;
    obj->flags |= mode;
}

extern f32 D_800A31F4;

/* Gives the fixed wave origin Z (22000) used by func_1511CB44. */
void func_1511CB2C(s32 obj, f32 *originZ) {
    *originZ = D_800A31F4;
}

struct Wave1511CB44 {
    u8 *phases;
    u16 phase;
    u16 speed;
    f32 amp;
    s32 unkC;
    u8 alphaPhase;
    u8 alphaPhaseOffset;
    u8 pulseAlpha;
    u8 speedPhaseOffset;
    f32 uvAngle;
    s16 (*uv)[2];
};

struct Obj1511CB44 {
    u8 pad0[0x10];
    s16 x;
    s16 y;
    s16 z;
    u16 count;
    u8 pad18[0x8];
    Vtx *buffers[2];
    Vtx *source;
    u8 pad2C[0x46];
    u8 variant;
    u8 pad73[0x9];
    struct Wave1511CB44 *wave;
    u8 pad80[0xA];
    u8 alpha;
};

extern s32 D_800902D8;
extern s32 func_15195FB0(void *, s32, s32, s32, s32, s32, s32);
extern void func_15094AB8(Vtx *arg0, s16 (*arg1)[2], s32 arg2, f32 arg3, s32 arg4, s32 arg5);

/*
 * Animates a wave mesh: vertex heights follow a travelling sine wave with per-vertex phase,
 * shading and alpha pulse with it, and (variant 0xFE) the texture coordinates rotate.
 */
void func_1511CB44(struct Obj1511CB44 *obj) {
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
    if (D_800BE9F0 == 4) {
        mode = 0;
    } else if (D_800BE9F0 == 19) {
        mode = 1;
        if (obj->variant == 0xFE) {
            uvRate = 0.4530000091f;
            uvArg4 = 32;
            uvArg5 = 32;
        }
    } else {
        return;
    }
    func_1511CB2C((s32)obj, &originZ);
    originZ -= obj->z;
    if (obj->wave == NULL) {
        created = TRUE;
        obj->wave = allocate_memory(sizeof(struct Wave1511CB44), 1, 0, 0);
        wave = obj->wave;
        wave->phases = allocate_memory(obj->count, 1, 0, 0);
        if (uvRate != 0.0f) {
            wave->uv = allocate_memory(obj->count * 4, 1, 0, 0);
            for (i = 0; i < obj->count; i++) {
                wave->uv[i][0] = obj->source[i].v.tc[0] - 0x2000;
                wave->uv[i][1] = obj->source[i].v.tc[1] - 0x2000;
            }
            wave->uvAngle = 0.0f;
        }
    } else {
        created = FALSE;
    }
    wave = obj->wave;
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
        dst = obj->buffers[D_800BE9C0];
        src = obj->source;
        for (i = 0; i < obj->count; i++) {
            tmp = originZ - obj->source[i].v.ob[2];
            wave->phases[i] = tmp * scale;
            dst->v.ob[0] = src->v.ob[0] + obj->x;
            dst->v.ob[2] = src->v.ob[2] + obj->z;
            dst++;
            src++;
        }
        wave->pulseAlpha = 0;
        wave->alphaPhase = 0;
        if (D_800BE9F0 == 4) {
            if (obj->variant != 0xF2) {
                if (obj->variant == 0xF1) {
                    wave->alphaPhaseOffset = 0;
                } else {
                    wave->alphaPhaseOffset = 0x80;
                }
                wave->pulseAlpha = 1;
            }
        }
        if (mode == 1) {
            wave->unkC = func_15195FB0(obj, D_800902D8, 0, 0, 0, 0, 8);
        }
    }
    dst = obj->buffers[D_800BE9C0];
    src = obj->source;
    phase = wave->phase >> 8;
    scale = 0.8100000024f;
    for (i = 0; i < obj->count; i++) {
        height = func_150489B0(wave->phases[i] + phase);
        y = (f32)(src->v.ob[1] + obj->y) + height * wave->amp;
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
        func_15094AB8(obj->buffers[D_800BE9C0], wave->uv, obj->count, wave->uvAngle, uvArg4, uvArg5);
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

struct Obj1511D394 {
    u8 pad0[0x16];
    u16 count;
    u8 pad18[0x8];
    Vtx *buffers[2];
    Vtx *source;
    u8 pad2C[0x10];
    s32 param;
    u8 pad40[0x3C];
    s16 *baseS;
    f32 velocity;
    f32 level;
    u8 pad88[2];
    u8 alpha;
};

struct Owner1511D394 {
    u8 pad0[0x7C];
    u16 *unk7C;
};

/*
 * Scrolls the mesh texture with the wave phase of the owner object (numbered by the low param
 * byte): S eases toward the wave level, T drifts by a cosine amount, and the alpha pulses.
 */
void func_1511D394(struct Obj1511D394 *obj) {
    u16 *ownerWave;
    struct Owner1511D394 *owner;
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

    id = obj->param & 0xFF;
    owner = func_151149AC((u8)id);
    if (owner == NULL) {
        return;
    }
    ownerWave = owner->unk7C;
    if (ownerWave == NULL) {
        return;
    }
    entry = &D_80089444[(obj->param >> 8) & 0xFF];
    rangeS = entry->rangeS;
    baseS = entry->baseS;
    phaseOffset = entry->phaseOffset;
    followRate = entry->followRate;
    accel = entry->accel;
    offsetT = entry->amplitudeT;
    if (obj->baseS == NULL) {
        obj->baseS = allocate_memory(obj->count * sizeof(s16), 1, 0, 0);
        savedS = obj->baseS;
        for (i = 0; i < obj->count; i++) {
            savedS[i] = obj->source[i].v.tc[0];
        }
        entry->alphaPhase = 0;
        init = TRUE;
    } else {
        init = FALSE;
        savedS = obj->baseS;
    }
    wavePhase = ownerWave[2];
    sine = (func_150489B0(((wavePhase >> 8) & 0xFF) + phaseOffset) + 1.0f) * 0.5f;
    cosine = (func_15048A40(((wavePhase >> 8) & 0xFF) + phaseOffset) + 0.5f) * 0.5f;
    if (init) {
        obj->level = sine;
        obj->velocity = 0.0f;
    } else {
        diff = (sine - obj->level) * followRate;
        if (diff > 0.0f) {
            obj->velocity += accel;
            if (diff < obj->velocity) {
                obj->velocity = diff;
            }
        } else {
            obj->velocity -= accel;
            if (obj->velocity < diff) {
                obj->velocity = diff;
            }
        }
        obj->level += obj->velocity;
    }
    offsetS = obj->level * rangeS;
    offsetS += baseS;
    offsetT = offsetT * cosine;
    if (obj->buffers[D_800BE9C0][0].v.tc[1] > 0x800) {
        offsetT -= 0x800;
    }
    for (i = 0; i < obj->count; i++) {
        obj->buffers[D_800BE9C0][i].v.tc[0] = savedS[i] - offsetS;
        obj->buffers[D_800BE9C0][i].v.tc[1] = obj->buffers[!D_800BE9C0][i].v.tc[1] + offsetT;
    }
    entry->alphaPhase += D_800BE9E4 * 2;
    obj->alpha = (func_150489B0(entry->alphaPhase + entry->alphaPhaseOffset) + 1.0f) * 0.5f * 205.0f + 50.0f;
}

typedef struct Game1483E0WaveState {
    Game1483E0UvPair *coords;
    s32 field4;
    s32 field8;
} Game1483E0WaveState;

typedef struct Game1483E0WaveMesh {
    u8 pad0[0x16];
    u16 count;
    u8 pad18[8];
    Game1483E0UvVertex *buffers[2];
    Game1483E0UvVertex *source;
    u8 pad2C[0x10];
    s32 flags; /* bits 0-11: S amplitude, 12-23: T amplitude, 24-31: speed */
    u8 pad40[0x3C];
    Game1483E0WaveState *wave;
    s32 phase;
} Game1483E0WaveMesh;

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
    s_delta = (f32)parameters.amplitude_s * sine;
    t_delta = (f32)parameters.amplitude_t * cosine;
    for (index = 0; index < mesh->count; index++) {
        mesh->buffers[D_800BE9C0][index].s = coords[index].s + s_delta;
        mesh->buffers[D_800BE9C0][index].t = coords[index].t + t_delta;
    }
    func_1511A494(mesh, &state->field4, &state->field8);
}

struct Obj1511D9E4 {
    u8 pad0[0x10];
    s16 x;
    s16 y;
    s16 z;
    u16 count;
    u8 pad18[0x8];
    Vtx *buffers[2];
    Vtx *source;
    u8 pad2C[0x10];
    s32 param; /* bits 24-31, 16-23: rotation args; bits 0-15: speed in 1/100 degree */
    u8 pad40[0x3C];
    s16 (*uv)[2];
    f32 uvAngle;
};

/* Rotates the mesh texture coordinates at the speed in the param; places both buffers on first use. */
void func_1511D9E4(struct Obj1511D9E4 *obj) {
    s16 (*uv)[2];
    s32 uvArg4;
    s32 uvArg5;
    s32 i;
    f32 speed;

    if (obj->uv == NULL) {
        uv = obj->uv = allocate_memory(obj->count * 4, 1, 0, 0);
        for (i = 0; i < obj->count; i++) {
            uv[i][0] = obj->source[i].v.tc[0] - 0x2000;
            uv[i][1] = obj->source[i].v.tc[1] - 0x2000;
        }
        for (i = 0; i < obj->count; i++) {
            obj->buffers[0][i].v.ob[0] = obj->buffers[1][i].v.ob[0] = obj->source[i].v.ob[0] + obj->x;
            obj->buffers[0][i].v.ob[1] = obj->buffers[1][i].v.ob[1] = obj->source[i].v.ob[1] + obj->y;
            obj->buffers[0][i].v.ob[2] = obj->buffers[1][i].v.ob[2] = obj->source[i].v.ob[2] + obj->z;
        }
    } else {
        uv = obj->uv;
    }
    uvArg4 = obj->param >> 24;
    uvArg5 = (obj->param >> 16) & 0xFF;
    speed = (f32)(s16)(obj->param & 0xFFFF) * 0.01;
    obj->uvAngle += speed * D_800BE9E4;
    func_15094AB8(obj->buffers[D_800BE9C0], uv, obj->count, obj->uvAngle, uvArg4, uvArg5);
}

struct Swing1511DBC4 {
    f32 time;
    f32 velocity;
    f32 accel;
    f32 jerk;
};

struct Obj1511DBC4 {
    u8 pad0[0x8];
    f32 angle;
    u8 padC[0x67];
    u8 flags;
    u8 pad74[0x8];
    struct Swing1511DBC4 *swing;
};

/*
 * Opens the object to 90 degrees with ever-increasing acceleration, wobbles slightly while open,
 * and closes one degree per frame.
 */
void func_1511DBC4(struct Obj1511DBC4 *obj) {
    s32 mode;
    struct Swing1511DBC4 *swing;
    f32 wobble;

    if (obj->swing == NULL) {
        swing = allocate_memory(sizeof(struct Swing1511DBC4), 1, 0, 0);
        obj->swing = swing;
        swing->time = 0.0f;
        swing->velocity = 0.0f;
        swing->accel = 0.0f;
        swing->jerk = 0.02500000037f;
    } else {
        swing = obj->swing;
    }

    mode = obj->flags & 3;
    if (mode == 0) {
        obj->angle = 0.0f;
    } else if (mode == 3) {
        obj->angle = 90.0f;
        wobble = cosf(swing->time * 0.01745329238f);
        wobble *= 0.5f;
        obj->angle += wobble;
        swing->time += 3.0f * (f32)D_800BE9E4;
    } else if (mode == 2) {
        if (obj->angle == 0.0f) {
            swing->velocity = 0.0f;
            swing->accel = 0.0f;
        }
        swing->accel += swing->jerk;
        swing->velocity += swing->accel;
        obj->angle += swing->velocity;
        if (obj->angle >= 90.0f) {
            mode = 3;
            obj->angle = 90.0f;
        }
    } else {
        if ((f32)D_800BE9E4 < obj->angle) {
            obj->angle -= (f32)D_800BE9E4;
        } else {
            obj->angle = 0.0f;
            mode = 0;
        }
    }

    obj->flags &= ~3;
    obj->flags |= mode;
}

struct Pulse1511DF6C {
    u16 phaseX;
    u16 phaseZ;
    u16 state;
    u16 bouncePhase;
    f32 bounce;
    f32 fullScale;
};

struct Obj1511DF6C {
    f32 rotX;
    f32 rotY;
    u8 pad8[0x8];
    s16 x;
    s16 y;
    s16 z;
    u8 pad16[0x2];
    f32 unk18;
    u8 pad1C[0x10];
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    u8 pad38[0x1C];
    u16 type;
    u8 pad56[0x18];
    u8 unk6E;
    u8 pad6F[0xD];
    struct Pulse1511DF6C *pulse;
    s32 lastCharacter;
    s32 filter;
};

/* The object returned by func_1509B570(0xBA). */
struct Owner1511DF6C {
    u8 pad0[0x1FE4];
    s32 unk1FE4;
};

extern void func_10010558(u16, void *, s32, s32, s32, s32);
extern void *func_1509B570(s32);
extern void func_1507C3E0(struct127 *, s16 *, s16 *, s16 *);
extern void func_151D6970(struct127 *, struct Obj1511DF6C *);

/* Plays the impact sound for this object type, with extra sounds for characters 0x3B and 0x80. */
void func_1511DD98(struct Obj1511DF6C *obj, struct127 *character) {
    s32 sound;
    s32 volume;
    s32 isChar3B;
    s32 isChar80;

    volume = 26000;
    isChar3B = character->id == 0x3B;
    isChar80 = character->id == 0x80;
    sound = 0;
    switch (obj->type) {
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
void func_1511DF6C(struct Obj1511DF6C *obj) {
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
    struct127 *character;
    u8 *data; // matching: one pointer for both lookups; a second local grows the frame
    f32 y;

    if (obj->pulse == NULL) {
        obj->pulse = allocate_memory(sizeof(struct Pulse1511DF6C), 1, 0, 0);
        pulse = obj->pulse;
        bzero(pulse, sizeof(struct Pulse1511DF6C));
        pulse->fullScale = obj->scaleX;
        obj->scaleZ = 0.0f;
        obj->scaleY = 0.0f;
        obj->scaleX = 0.0f;
    } else {
        pulse = obj->pulse;
    }
    skip = FALSE;
    anim = -1;
    switch (obj->type) {
    case 0x59:
    case 0x5A:
        break;
    case 0x54:
        anim = 0x17;
        if (D_800BE9F0 == 45) {
            data = func_1509B570(0xBA);
            if (data != NULL && (hp = ((struct Owner1511DF6C *)data)->unk1FE4) <= 0) {
                skip = TRUE;
            }
        }
        /* fallthrough */
    default:
        obj->rotY += (f32)D_800BE9E4 * 1.519999981f;
        dist = cosf(pulse->phaseX * 9.587380191e-05f);
        obj->rotX = 8.0f * dist;
        dist = cosf(pulse->phaseZ * 9.587380191e-05f);
        obj->unk18 = 12.0f * dist;
        pulse->phaseX += (f32)D_800BE9E4 * 507.0f;
        pulse->phaseZ += (f32)D_800BE9E4 * 419.0f;
        break;
    }
    if (pulse->state < 3 && !skip) {
        for (i = 0; i < D_8008FD8C; i++) {
            character = &D_800CC2D0[i];
            if (character->interaction_state == 0 || character->health <= 0 || character->unk127 == 0xFF) {
                continue;
            }
            if (character->unk300 == 0) {
                continue;
            }
            if ((obj->filter == 1 && *(u8 *)&character->pad128 != 0) || (obj->filter == 2 && *(u8 *)&character->pad128 == 0) || (obj->filter == 3 && (*(u8 *)&character->pad128 & 1))) {
                continue;
            }
            if (anim != -1 && func_1503195C(character, anim, 0) != NULL) {
                continue;
            }
            dist = ((f32)obj->x - character->x_position) * ((f32)obj->x - character->x_position) + ((f32)obj->z - character->z_position) * ((f32)obj->z - character->z_position);
            data = (u8 *)character->unk31C;
            radius = 100.0f;
            scale = (f32)(*(s8 *)(data + 0x1AF) & 0xFF) * 0.015625f;
            if (data[0x84] != 0) {
                scale *= 1.399999976f;
            }
            radius *= scale;
            if (dist < radius * radius) {
                func_1507C3E0(character, &height, &unused1, &unused2);
                y = (f32)obj->y;
                if (y < *(f32 *)&character->pad17C && character->y_position < y + radius && y - radius < character->y_position + height) {
                    obj->lastCharacter = character->unique_id;
                    pulse->state = 3;
                    func_1511DD98(obj, character);
                    func_151D6970(character, obj);
                    break;
                }
            }
        }
    }
    if (pulse->state == 0) {
        obj->scaleZ += 0.2599999905f * pulse->fullScale;
        if (pulse->fullScale <= obj->scaleZ) {
            obj->scaleZ = pulse->fullScale;
            pulse->bouncePhase = 0;
            pulse->state++;
            pulse->bounce = 0.3000000119f;
        }
        obj->scaleY = obj->scaleZ;
        obj->scaleX = obj->scaleZ;
    } else if (pulse->state == 1) {
        pulse->bounce -= 0.01999999955f * pulse->fullScale;
        if (pulse->bounce <= 0.0f) {
            pulse->state++;
            pulse->bounce = 0.0f;
        }
        obj->scaleZ = cosf(pulse->bouncePhase * 9.587380191e-05f) * pulse->bounce + pulse->fullScale;
        obj->scaleX = obj->scaleY = obj->scaleZ;
        pulse->bouncePhase += D_800BE9E4 * 0x1613;
    } else if (pulse->state == 2) {
    } else if (pulse->state == 3) {
        obj->scaleZ -= 0.2599999905f * pulse->fullScale;
        if (obj->scaleZ <= 0.0f) {
            pulse->state = 0;
            obj->unk6E = 1;
            obj->scaleZ = 0.0f;
        }
        obj->scaleY = obj->scaleZ;
        obj->scaleX = obj->scaleZ;
    }
}
