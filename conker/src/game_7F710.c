#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/*
 * While mv->unk6B < 4, moves the actor toward the s16 target position in its mv block
 * (all the way, or 40% per call in modes 1 and 3); otherwise clears the mode once unk28 runs out.
 * Ends the mode and sets inwater when the actor drops below unk118.
 */
void func_15052260(Actor *actor) {
    ActorCtl *ctrl = actor->mv;
    f32 blend;

    if (ctrl->unk6B == 1 || ctrl->unk6B == 3) {
        blend = 0.400000006f;
    } else {
        blend = 1.0f;
    }
    actor->unk1CC = actor->ypos;

    if (ctrl->unk6B < 4) {
        actor->xpos += ((f32)ctrl->holdX - actor->xpos) * blend;
        actor->ypos += ((f32)(ctrl->holdY - 80) - actor->ypos) * blend;
        actor->zpos += ((f32)ctrl->holdZ - actor->zpos) * blend;
        func_1505E650(actor, 20, 1.0f, 6.0f, 0.0f, 0.0f, 0);
        actor->yspd = -10.0f;
    } else {
        if ((s32)actor->shad == 0) {
            ctrl->unk6B = 0;
        } else {
            func_1505E650(actor, 56, 1.0f, 6.0f, 0.0f, 0.0f, 0);
        }
        func_1505A770(actor);
    }
    actor->ygrav = 6.199999809f;
    if (actor->ypos < actor->unk118) {
        actor->ygrav = 0.0f;
        actor->yspd = 0.0f;
        actor->mv->unk6B = 0;
        actor->count5 = 0;
        actor->inwater = 1;
    }
}

/* Clears inwater, unkB2 and mv->ladder, sets gravity 4, speed 15; a living actor also gets norun 6. */
void func_15052408(Actor *actor) {
    actor->unkB2 = 0;
    actor->mv->ladder = 0;
    if (actor->energy != 0) {
        actor->count3 = 0;
        actor->count5 = 0;
        actor->norun = 6;
    }
    actor->inwater = 0;
    actor->ygrav = 4.0f;
    actor->vtilt = 0.0f;
    actor->speed = 15.0f;
}

void func_15052458(s32 unused) {
}

/* Calls func_15060F28(actor, 1) once the actor's health reaches zero. */
void func_15052464(Actor *actor) {
    if (actor->energy == 0) {
        func_15060F28(actor, 1);
    }
}

/*
 * Sets unk40 to the facing angle plus a quarter turn, in degrees, and adds amount * scale to unkB8,
 * subtracting it instead when `angle` lies in the half-turn behind that direction.
 */
void func_15052490(Actor *actor, u16 angle, f32 amount, f32 scale) {
    scale *= D_800D1550;

    actor->theta = (actor->langle + 0x4000) * (360.0f / 65536.0f);

    if ((angle - actor->langle + 0x4000) & 0x8000) {
        actor->vtilt -= amount * scale;
    } else {
        actor->vtilt += amount * scale;
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
