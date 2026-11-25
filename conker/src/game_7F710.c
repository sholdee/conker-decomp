#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_1505A770(Actor *arg0);

/*
 * While unk31C->unk6B < 4, moves the actor toward the s16 target position in its unk31C block
 * (all the way, or 40% per call in modes 1 and 3); otherwise clears the mode once unk28 runs out.
 * Ends the mode and sets in_water when the actor drops below unk118.
 */
void func_15052260(Actor *actor) {
    ActorCtl *ctrl = actor->unk31C;
    f32 blend;

    if (ctrl->unk6B == 1 || ctrl->unk6B == 3) {
        blend = 0.400000006f;
    } else {
        blend = 1.0f;
    }
    actor->unk1CC = actor->y_position;

    if (ctrl->unk6B < 4) {
        actor->x_position += ((f32)ctrl->holdX - actor->x_position) * blend;
        actor->y_position += ((f32)(ctrl->holdY - 80) - actor->y_position) * blend;
        actor->z_position += ((f32)ctrl->holdZ - actor->z_position) * blend;
        func_1505E650(actor, 20, 1.0f, 6.0f, 0.0f, 0.0f, 0);
        actor->y_velocity = -10.0f;
    } else {
        if ((s32)actor->unk28 == 0) {
            ctrl->unk6B = 0;
        } else {
            func_1505E650(actor, 56, 1.0f, 6.0f, 0.0f, 0.0f, 0);
        }
        func_1505A770(actor);
    }
    actor->gravity = 6.199999809f;
    if (actor->y_position < actor->unk118) {
        actor->gravity = 0.0f;
        actor->y_velocity = 0.0f;
        actor->unk31C->unk6B = 0;
        actor->unk83 = 0;
        actor->in_water = 1;
    }
}

/* Clears in_water, unkB2 and unk31C->unk95, sets gravity 4, xz_velocity 15; a living actor also gets disable_run 6. */
void func_15052408(Actor *actor) {
    actor->unkB2 = 0;
    actor->unk31C->unk95 = 0;
    if (actor->health != 0) {
        actor->unk81 = 0;
        actor->unk83 = 0;
        actor->disable_run = 6;
    }
    actor->in_water = 0;
    actor->gravity = 4.0f;
    actor->unkB8 = 0.0f;
    actor->xz_velocity = 15.0f;
}

void func_15052458(s32 unused) {
}

/* Calls func_15060F28(actor, 1) once the actor's health reaches zero. */
void func_15052464(Actor *actor) {
    if (actor->health == 0) {
        func_15060F28(actor, 1);
    }
}

/*
 * Sets unk40 to the facing angle plus a quarter turn, in degrees, and adds amount * scale to unkB8,
 * subtracting it instead when `angle` lies in the half-turn behind that direction.
 */
void func_15052490(Actor *actor, u16 angle, f32 amount, f32 scale) {
    scale *= D_800D1550;

    actor->unk40 = (actor->unk7A + 0x4000) * (360.0f / 65536.0f);

    if ((angle - actor->unk7A + 0x4000) & 0x8000) {
        actor->unkB8 -= amount * scale;
    } else {
        actor->unkB8 += amount * scale;
    }
}

/* Steps unkCC toward unkCE by a twelfth of the gap plus one, without overshooting. */
void func_1505250C(Actor *actor, s32 unused) {
    s16 target = actor->unkCE;
    s16 current = actor->unkCC;

    if (target != current) {
        if (current < target) {
            actor->unkCC = current + (target - current) / 12 + 1;
            if (target < actor->unkCC) {
                actor->unkCC = target;
            }
        } else {
            actor->unkCC = current + (target - current) / 12 - 1;
            if (actor->unkCC < target) {
                actor->unkCC = target;
            }
        }
    }
}
