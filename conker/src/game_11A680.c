#define func_1514D3B0 func_1514D3B0_hdr
#include <ultra64.h>
#include "functions.h"
#undef func_1514D3B0
extern s32 func_1514D3B0(void *, s16, s32, s32);
#include "variables.h"

extern void func_151254F4(struct108 *camera, s32 index);
extern void func_1517F488(s32 r, s32 g, s32 b, s32 alpha, s32 duration, s32 view);
extern f32 func_15144BC8(f32 arg0);

/* File-local view of struct127.unk31C; structs.h's struct126 stops at 0x1AC, before the aim angles. */
typedef struct {
    /* 0x000 */ u8 pad0[0x75];
    /* 0x075 */ u8 unk75;
    /* 0x076 */ u8 pad76[2];
    /* 0x078 */ u8 unk78;
    /* 0x079 */ u8 pad79[0xB];
    /* 0x084 */ u8 unk84;
    /* 0x085 */ u8 pad85[0x9B];
    /* 0x120 */ u8 unk120;
    /* 0x121 */ u8 pad121[7];
    /* 0x128 */ u8 flags;
    /* 0x129 */ s8 target;   /* index into D_800CC2D0, or -1 */
    /* 0x12A */ s8 unk12A;
    /* 0x12B */ u8 pad12B;
    /* 0x12C */ u16 timer;   /* counts down by D_800BE9E4 */
    /* 0x12E */ u8 pad12E[0x69];
    /* 0x197 */ u8 unk197;
    /* 0x198 */ u8 pad198[0x20];
    /* 0x1B8 */ f32 aimPitch; /* degrees, clamped to [-20, 80] */
    /* 0x1BC */ f32 aimYaw;   /* degrees, wrapped to [0, 360) */
} ObjState;

#define STATE(obj) ((ObjState *)(obj)->unk31C)

/* variables.h declares these as plain arrays: D_800BE748 is the OSContPad array and
   D_800BE728 holds a pointer to the per-controller OSContPad pointers. */
#define PADS ((OSContPad *)D_800BE748)
#define CONT_PADS (*(OSContPad ***)&D_800BE728)

#define U8AT(p, o) (*(u8 *)((u8 *)(p) + (o)))
#define S8AT(p, o) (*(s8 *)((u8 *)(p) + (o)))
#define S16AT(p, o) (*(s16 *)((u8 *)(p) + (o)))

/* Signed angle in degrees from `from` to `to`, in (-180, 180]. */
f32 func_150ED1D0(f32 from, f32 to) {
    f32 delta;

    from = func_15144BC8(from);
    delta = func_15144BC8(func_15144BC8(to) - from);
    if (delta > 180.0f) {
        delta = -360.0f + delta;
    }
    return delta;
}

/* Signed angle in degrees from `self`'s heading (unk40) to the direction of `target`. */
f32 func_150ED234(struct127 *self, struct127 *target) {
    s32 angle;

    angle = func_1505A630(target->x_position - self->x_position, self->z_position - target->z_position, 0) + 0x4000;
    return func_150ED1D0(angle * (360.0f / 65536.0f), self->unk40);
}

extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

/*
 * Ballistic aim error for a shell fired from `obj` (from its bone matrix 3 when it has a skeleton) at
 * elevation `pitch` degrees towards (targetX, targetY, targetZ), aiming `radius` short of it.
 * Returns how far the shell lands short (+) or long (-) of the target, or, when the target is too high
 * to reach, 1000 times the extra elevation it needs.
 */
f32 func_150ED298(struct127 *obj, f32 pitch, f32 targetX, f32 targetY, f32 targetZ, f32 radius) {
    Mtx *bones;
    f32 gravity;
    f32 disc;
    f32 x;
    f32 y;
    f32 z;
    f32 dx;
    f32 speedY;
    f32 speedXZ;
    f32 dz;
    f32 dy;
    f32 dist;
    f32 time;
    f32 result;
    f32 root;
    f32 mf[4][4];
    f32 firstTime;

    if (obj->unk1D4 != NULL) {
        bones = (Mtx *)obj->unk1D4;
        guMtxL2F(mf, &bones[3]);
        func_150A7960(mf[0], 0.0f, 0.0f, 1.0f, &x, &y, &z);
        x -= mf[3][0];
        y -= mf[3][1];
        z -= mf[3][2];
        pitch = func_150484A0(y, sqrtf(x * x + z * z)) * 57.29577637f;
        if (pitch > 180.0f) {
            pitch -= 360.0f;
        }
        x = mf[3][0];
        y = mf[3][1];
        z = mf[3][2];
    } else {
        x = obj->x_position;
        y = obj->y_position;
        z = obj->z_position;
    }
    dx = targetX - x;
    dy = targetY - y;
    dz = targetZ - z;
    dist = sqrtf(dx * dx + dz * dz) - radius;
    if (dist < 100.0f) {
        dist = 100.0f;
    }
    result = 5000.0f;
    speedXZ = cosf(pitch * 0.01745329238f) * 148.25f;
    speedY = sinf(pitch * 0.01745329238f) * 148.25f;
    gravity = -2.5f;
    disc = speedY * speedY + 2 * (dy * gravity);
    if (disc > 0.0f) {
        root = sqrtf(disc);
        firstTime = (-speedY - root) / gravity;
        time = firstTime;
        if (firstTime > 0.0f) {
            disc = (-speedY + root) / gravity;
            if (disc > 0.0f && firstTime < disc) {
                time = disc;
            }
        }
        if (time > 0.0f) {
            time *= speedXZ;
            result = dist - time;
        }
    } else {
        result = (func_15048408(sqrtf(-2.0f * dy * gravity) * 0.006745362654f) * 57.29577637f - pitch) * 1000.0f;
    }
    return result;
}

/* Lets `obj` see the controlling player's camera (bit in unk2FC) while that player holds R with unk197 set. */
void func_150ED578(struct127 *obj) {
    struct126 *state;
    s32 player;
    struct127 *owner;
    struct108 *camera;

    state = obj->unk31C;
    if ((state != NULL) && (state->unk84 == 0)) {
        player = D_800BE616 ? obj->unk127 : obj->unk124;
        if (CONT_PADS[player]->button & R_TRIG) {
            owner = &D_800CC2D0[player];
            if (STATE(owner)->unk197 != 0) {
                camera = owner->camera;
                if (camera != NULL) {
                    obj->pad2FC[0] |= 1 << camera->unk23D;
                }
            }
        }
    }
}

// matching: arg2 as s32 here; the u8 of game_83300.c's definition would add a narrowing
extern void func_15062FC0(struct127 *actor, s32 group, s32 arg2, s32 wrapS, s32 wrapT, s32 count, s32 ds, s32 dt);

/* Scrolls texture groups 0 and 1 of `obj` (its two treads) by the track speeds, each clamped to [-45, 45]. */
void func_150ED638(struct127 *obj, s32 left, s32 right) {
    s32 temp;
    s32 size;

    left = (left < -45) ? -45 : (left > 45) ? 45 : left;
    right = (right < -45) ? -45 : (right > 45) ? 45 : right;

    if (obj->id == 0x28) {
        size = 124;
    } else {
        temp = left;
        left = right;
        size = 28;
        right = temp;
    }

    func_15062FC0(obj, 0, 0, 0x800, 0x800, size, -right * 7, 0);
    func_15062FC0(obj, 1, size, 0x800, 0x800, size, -left * 7, 0);
}

extern s8 D_800E0BCE[];
extern s32 func_1508DAEC(s32, s32);
extern struct216 *func_1507BB28(s32, s32);
extern s32 func_15052F9C(struct127 *arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
extern void func_1505327C(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, s32, f32 *, f32 *, f32 *, f32 *, s32, s32, f32);
extern void func_15052590(struct127 *obj);
extern s32 func_151452C4(f32 *, f32 *, f32 *, f32, f32 *, f32 *, f32 *, f32 *);

void func_150ED748(struct127 *arg0) {
    s32 idxa[1];
    f32 vel;
    s32 health;
    u8 active;
    u8 player;
    u16 oldAngle;
    s32 v;
    f32 yv;
    s32 flags;
    f32 xv;
    f32 speed;
    s8 stick;
    s16 turn;
    s16 d;
    s32 left;
    s32 right;
    s32 iv;
    f32 k;
    struct127 *p;
    struct127 *o;
    s32 rev;
    OSContPad *pp;
    s8 t;
    struct127 *tgt;
    f32 offs;
    f32 rad;
    f32 sx;
    f32 sz;
    f32 dx;
    f32 pitch;
    f32 vel2;
    f32 vx;
    f32 dz;
    f32 h;
    f32 ddx;
    f32 ddz;
    f32 dd;
    f32 best;
    s32 i;
    s32 bestIdx;
    s32 count;
    f32 step;
    f32 a;
    s8 cam;
    s32 j;
    s32 res;
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    f32 dir[3];
    f32 lim;
    f32 dist;
    f32 vA[3];
    f32 vB[3];
    f32 t1;
    f32 t2;
    struct108 *c;
    struct127 *co;


    health = 1;
    if (D_800BE616) {
        idxa[0] = arg0->unk127;
    } else {
        idxa[0] = 0;
    }
    arg0->pad2FC[0] = 0;
    arg0->unk124 = 0;
    active = 0;
    player = arg0->unk124;
    oldAngle = arg0->unk7A;
    arg0->unk247 = 0x10;
    arg0->unk248 = 8;
    if (D_800BE616 == 0) {
        if (arg0->unk13C != 0) {
            active = 1;
            vel = arg0->xz_velocity;
            if (fabsf(vel) > 5.0f) {
                arg0->unkD0 = 1;
                arg0->unk114 = 2.0f;
                if (vel < 0.0f) {
                    arg0->unkD0 = 2;
                }
            }
        }
    } else {
        player = arg0->unk127;
        if (arg0->unk127 != 0xFF) {
            if (D_800CC2D0[arg0->unk127].unk31C->unk84 == 0) {
                active = 1;
            }
            p = &D_800CC2D0[player];
            if (p->health != 0) {
                if (STATE(p)->flags & 0x80) {
                    if (STATE(p)->timer != 0) {
                        arg0->immune = 0xFF;
                        if (D_800BE9E4 < STATE(&D_800CC2D0[player])->timer) {
                            STATE(&D_800CC2D0[player])->timer -= D_800BE9E4;
                        } else {
                            STATE(&D_800CC2D0[player])->timer = 0;
                            func_1508DAEC(player, 0);
                            arg0->immune = 0;
                        }
                    }
                } else if ((STATE(p)->flags & 0x10) && (STATE(p)->timer == 0)) {
                    func_1508DAEC(player, 0);
                }
            }
        }
    }
    if (active) {
        s32 dummyA;
        speed = 0.0f;
        flags = 0;
        health = D_800CC2D0[player].health;
        if (health != 0) {
            stick = PADS[player].stick_x;
            arg0->unk247 = 0x32;
            arg0->unk248 = 0x32;
            speed = 0.6399999857f;
            iv = STATE(&D_800CC2D0[player])->flags; if ((iv & 1) && (CONT_PADS[player]->button & 0x8000) && (STATE(&D_800CC2D0[player])->timer != 0)) {
                arg0->unk247 = 0x1C;
                arg0->unk248 = 0x1C;
                PADS[player].stick_y = 0x50;
                speed += speed;
                if (D_800BE9E4 < STATE(&D_800CC2D0[player])->timer) {
                    STATE(&D_800CC2D0[player])->timer -= D_800BE9E4;
                } else {
                    STATE(&D_800CC2D0[player])->timer = 0;
                    func_1508DAEC(player, 0);
                }
            }
            turn = 18.0f - arg0->xz_velocity * (0.3199999928f / speed);
            turn = (turn * 0.5f + 9.0f) * D_800BE9A4;
            if (turn < 10) {
                turn = 10;
            }
            if ((CONT_PADS[player]->button & 0x10) && ((STATE(&D_800CC2D0[player])->flags & 2) || (D_800BE616 == 0))) {
                speed = 0.0f;
                flags = 0x1F;
                stick = 0;
                arg0->unk22B = 0;
                arg0->xz_velocity = 0.0f;
            }
            S8AT(arg0, 0x22B) += (stick - S8AT(arg0, 0x22B)) / 4;
            if (D_800BE616 != 0) {
                arg0->unk22B = stick;
            }
            if (stick == 0) {
                if ((S8AT(arg0, 0x22B) >= -4) && (S8AT(arg0, 0x22B) < 5)) {
                    arg0->unk22B = 0;
                }
            }
            arg0->unk78 = arg0->unk7A - S8AT(arg0, 0x22B) * turn;
            arg0->unkF4 |= 0x40;
            v = player;
            if ((D_800BE616 != 0) && (player < 2)) {
                if (D_800E0BCE[player] != 0) {
                    v = D_800E0BCE[player];
                }
            }
            if (D_800BE710[v] & 0x2000) {
                arg0->unk21C = 0;
                arg0->unk218 = func_1507BB28(0, 2);
            }
        }
        if (arg0->disable_run != 0) {
            speed = 0.0f;
            arg0->disable_run--;
        }
        arg0->unk44 = PADS[player].stick_y * speed;
        STATE(&D_800CC2D0[player])->unk75 = flags;
        STATE(&D_800CC2D0[player])->unk78 = flags;
    } else if (D_800C3E78 < D_8008FD8C) {
        func_1504C854(arg0);
        if ((player < 4) && (CONT_PADS[player]->button & 0x2000)) {
            D_800CC288 = 0x2000;
        }
        if (D_800CC288 & 0x2000) {
            arg0->unk244 = 1;
        }
    }
    arg0->unk247 = 0x32;
    arg0->unk248 = 0x32;
    if ((D_800BE616 == 0) || (D_800C3E78 >= D_8008FD8C) || (STATE(arg0)->unk120 == 0)) {
        func_15052590(arg0);
    }
    if (arg0->interaction_state != 0) {
        if (arg0->unk232 == 0xD) {
            func_15052F9C(arg0, 180.0f, 5, 0x44, 1, 0x10, 0xFF, 0, 0, 0);
        } else if ((D_800BE616 == 0) && (D_800CC268 & 1)) {
            co = &D_800CC2D0[arg0->unk124];
            if ((co->unk65 == 0) && (STATE(co)->unk78 == 0) && (arg0->unk25C & 0x400)) {
                if ((co->unk44 > 15.0f) && (co->y_velocity > 10.0f)) {
                    func_1505327C(arg0, 68.0f, 4.0f, 0xD, 5);
                } else if ((co->disable_run == 0) && (co->unk28 > 70.0f) && (co->stunned == 0) && (func_1505A6F8(co, arg0) < 100.0f)) {
                    func_15052F9C(arg0, 180.0f, 5, 0x44, 1, 0x10, 0xFF, 0, 1, 0);
                }
            }
        }
        {
        s32 dummy;
        k = 1.0f;
        if (arg0->health != 0) {
            typedef s32 dummyTT2;
            yv = arg0->unk44;
            xv = arg0->xz_velocity;
            d = (s16)(arg0->unk7A - oldAngle) / 70;
            if (fabsf(yv) > 4.0f) {
                xv = xv * 0.6000000238f + yv * (0.400000006f * k);
            }
            left = (s32)xv + d;
            right = (s32)xv - d;
        } else {
            right = 0;
            left = 0;
        }
        func_150ED638(arg0, left, right);
        }
        if ((arg0->id == 0x77) || (arg0->id == 0x28)) {
            if (((xv = arg0->xz_velocity) > 5.0f) && (arg0->unk44 <= 0.0f)) {
                rev = 1;
            } else if ((xv < -5.0f) && (arg0->unk44 >= 0.0f)) {
                rev = 1;
            } else {
                rev = 0;
            }
            if (rev) {
                func_1514D3B0((s32)arg0, 0x19, 0xD, 0);
            } else {
                func_1514D3B0((s32)arg0, 0x19, 0xE, 0);
            }
            rev = rev ? 0xC : 0xF;
            U8AT(arg0, 0x6C) = rev + 0xA;
            U8AT(arg0, 0x6D) = 0x13;
        }
        if (D_800CC2D0[idxa[0]].unk31C->unk84 != 0) {
            if (STATE(arg0)->target >= 0) {
                f32 s;
                tgt = &D_800CC2D0[STATE(arg0)->target];
                a = tgt->x_position - arg0->x_position;
                dd = tgt->z_position - arg0->z_position;
                vel2 = sqrtf(a * a + dd * dd) / 200.0f; vel2 = tgt->xz_velocity * vel2;
                rad = tgt->unk40 * 0.01745329238f;
                sx = sinf(rad) * vel2;
                sz = cosf(rad) * vel2;
                tgt->x_position += sx;
                tgt->z_position += sz;
                pitch = func_150ED234(arg0, tgt) * 0.01745329238f;
                vx = sinf(pitch) * cosf(arg0->unkC4 * 0.01745329238f);
                s = cosf(pitch) * cosf(arg0->unkB8 * 0.01745329238f);
                h = func_150484A0(vx * 1000.0f, s * 1000.0f);
                offs = STATE(arg0)->unk12A * 400.0f;
                STATE(arg0)->aimYaw -= func_150ED1D0(h * 57.29577637f + STATE(arg0)->unk12A * 10.0f, STATE(arg0)->aimYaw) * 0.125f;
                STATE(arg0)->aimPitch += func_150ED298(arg0, STATE(arg0)->aimPitch - arg0->unkB8, tgt->x_position, tgt->y_position, tgt->z_position, offs) * 0.001000000047f;
                tgt->x_position -= sx;
                tgt->z_position -= sz;
            } else {
                STATE(arg0)->aimYaw *= 0.8999999762f;
                STATE(arg0)->aimPitch *= 0.8999999762f;
            }
        } else if (active && health) {
            typedef s32 dummyT8;
            best = 0.0f;
            step = 3.0f * D_800BE9A4;
            bestIdx = -1;
            cam = 0;
            count = D_8008FD8C;
            if ((D_800BE616 != 0) && (idxa[0] < 2) && (D_800E0BCE[idxa[0]] != 0)) {
                cam = D_800E0BCE[idxa[0]] + 1;
            } else {
                pp = &PADS[idxa[0]];
                if (pp->button & 1) {
                    STATE(arg0)->aimYaw += step;
                }
                if (pp->button & 2) {
                    STATE(arg0)->aimYaw -= step;
                }
            }
            if (STATE(&D_800CC2D0[player])->unk197 != 0) {
                cam = idxa[0] + 1;
            }
            if (D_800BE616 == 0) {
                count = 0x19;
            }
            if (cam == 0) {
                typedef s32 dummyT6;
                for (i = 0; i < count; i++) {
                    typedef s32 dummyT;
                    if (i != idxa[0]) {
                        o = &D_800CC2D0[i];
                        if ((D_800BE616 == 0) && !(o->unkF8 & 0x40)) {
                            continue;
                        }
                            a = func_150ED1D0(func_150ED234(arg0, o), STATE(arg0)->aimYaw);
                            dd = a;
                            if (a < 0.0f) {
                                dd = -a;
                            }
                            dd = 35.0f - dd;
                            if (dd < 0.0f) {
                                dd = 0.0f;
                            }
                            if (dd > 0.0f) {
                                ddz = arg0->x_position - o->x_position;
                                ddx = arg0->z_position - o->z_position;
                                ddz = sqrtf(ddz * ddz + ddx * ddx);
                                if (ddz < 7000.0f) {
                                    if (ddz < 1.0f) {
                                        ddz = 1.0f;
                                    }
                                    dd = dd / ddz;
                                    if (best < dd) {
                                        best = dd;
                                        bestIdx = i;
                                    }
                                }
                            }
                    }
                }
                if (bestIdx != -1) {
                    typedef s32 dummyT5;
                    co = &D_800CC2D0[bestIdx];
                    if (D_800BE616 != 0) {
                        lim = co->y_position + 40.0f;
                    } else {
                        lim = S16AT(co, 0x1AA);
                    }
                    ddx = func_150ED298(arg0, 0.0f, co->x_position, lim, co->z_position, 0.0f) * 0.001000000047f;
                    lim = 4.0f * D_800BE9A4;
                    if (lim < ddx) {
                        ddx = lim;
                    }
                    if (ddx < -lim) {
                        ddx = -lim;
                    }
                    STATE(arg0)->aimPitch += ddx;
                } else {
                    STATE(arg0)->aimPitch *= 0.8999999762f;
                }
            } else {
                c = &D_800DBFF0[cam - 1];
                co = c->unk3D4;
                if (c->unk3D4->unk197 != 0) {
                    typedef s32 dummyT7;
                    h = -co->unk170; h -= arg0->unkB8; dz = co->unk16C; dz = (arg0->unk40 - 90.0f) - dz;
                    while (h > 180.0f) { h -= 360.0f; if (h) {} }
                    while (h < -180.0f) {
                        h += 360.0f;
                    }
                    while (dz > 180.0f) {
                        dz -= 360.0f;
                    }
                    while (dz < -180.0f) {
                        dz += 360.0f;
                    }
                    STATE(arg0)->aimPitch = h;
                    STATE(arg0)->aimYaw = dz;
                } else {
                    dir[0] = c->unk2BC - c->unk2F8;
                    dir[1] = c->unk2C0 - c->unk2FC;
                    dir[2] = c->unk2C4 - c->unk300;
                    dd = 1.0f / sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
                    dir[0] *= dd;
                    dir[1] *= dd;
                    dir[2] *= dd;
                    dist = 100000.0f;
                    res = func_150AC9C0(c->unk2F8, c->unk2FC, c->unk300, dir[0], dir[1], dir[2], 0, 0, &hitX, &hitY, &hitZ, &dist, 0, 0, 0.0f);
                    if (res != 0) {
                        res = 1;
                    }
                    for (j = 0; j < D_8008FD8C; j++) {
                        if (U8AT(arg0, 0x128) != U8AT(&D_800CC2D0[j], 0x128)) {
                            hitX = D_800CC2D0[j].x_position - c->unk2F8;
                            hitZ = D_800CC2D0[j].z_position - c->unk300;
                            if (sqrtf(hitX * hitX + hitZ * hitZ) + 200.0f < dist) {
                                if (func_151452C4(&c->unk2F8, dir, &D_800CC2D0[j].x_position, 150.0f, vA, vB, &t1, &t2) && (t1 < dist)) {
                                    dist = t1;
                                    res = 2;
                                }
                            }
                        }
                    }
                    U8AT(co, 0x128) &= ~0x40;
                    if (res != 0) {
                        if (res == 2) {
                            U8AT(co, 0x128) |= 0x40;
                        }
                        hitX = dir[0] * dist + c->unk2F8;
                        hitY = dir[1] * dist + c->unk2FC;
                        hitZ = dir[2] * dist + c->unk300;
                        STATE(arg0)->aimPitch += func_150ED298(arg0, 0.0f, hitX, hitY, hitZ, 0.0f) * 0.001000000047f;
                    } else {
                        STATE(arg0)->aimPitch *= 0.8999999762f;
                    }
                }
            }
        }
        if (STATE(arg0)->aimPitch < -20.0f) {
            STATE(arg0)->aimPitch = -20.0f;
        }
        if (STATE(arg0)->aimPitch > 80.0f) {
            STATE(arg0)->aimPitch = 80.0f;
        }
        if (STATE(arg0)->aimYaw > 360.0f) {
            STATE(arg0)->aimYaw -= 360.0f;
        }
        if (STATE(arg0)->aimYaw < 0.0f) {
            STATE(arg0)->aimYaw += 360.0f;
        }
        S16AT(arg0, 0x2E4) = STATE(arg0)->aimYaw * 50.0f;
        S16AT(arg0, 0x2E6) = -(s16)(STATE(arg0)->aimPitch * 50.0f);
    }
}

/* Sets `obj` to state 3 (immune) and gives its camera back to player object 0 or 1, clearing that pad's R. */
void func_150EEC84(struct127 *obj) {
    struct108 *camera;
    struct127 *player;
    s32 index;

    obj->unk5 = 3;
    obj->unkE4 = 0;
    obj->immune = 0xFF;
    obj->pad320[8] = 0;
    if (obj->camera != NULL) {
        camera = obj->camera;
        index = 0;
        if (camera->unk23D == 3) {
            index = 1;
        }
        func_151254F4(camera, index);
        obj->unk31C->unk78 = 0x29;
        PADS[index].button &= ~R_TRIG;
        player = &D_800CC2D0[index];
        player->pad2FC[0] |= 1 << camera->unk23D;
        if (player->pad10A != 0) {
            func_1517F488(0xFF, 0, 0, 180, 20, camera->unk23D);
            player->pad10A = 0;
        }
    }
}

extern void func_151045E0(void *, s32, f32);

/* Sets `obj` to state 3 (immune) if it is not already, then runs the common object update. */
void func_150EEDA8(struct127 *obj) {
    if (obj->unk5 != 3) {
        func_151045E0(obj, 15, 250.0f);
        obj->unk5 = 3;
        obj->unkE4 = 0;
        obj->immune = 0xFF;
    }
    func_15052590(obj);
}
