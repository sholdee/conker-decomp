#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Defined in game_981E0.c. Not in event.h: game_83300.c calls it with a fourth argument. */
f32 func_1506AD30(Actor *obj, f32 time, u8 filtered);

/* Advances an animation playback by rate, handling looping, hold and end callbacks. */
void func_1507BDB0(AnimInstance *anim, f32 rate, Actor *obj, s32 mode) {
    f32 time;
    s32 hold;
    u8 jumped;
    f32 result;
    s32 oldAnimId;

    if (anim->frameRom[0] == 0) {
        return;
    }

    if (obj != NULL) {
        if (D_800C35EA[0] == 0) {
            if (D_800BE9A4 < 2.0f) {
                rate = D_800BE9A4 * rate;
            } else {
                rate += rate;
            }
        } else {
            rate = D_800BE9A4;
        }
    }
    if (D_800BEA0C != 0) {
        rate = 0.0f;
    }

    if (anim->numFrames[0] < (anim->speed[0] * rate)) {
        time = anim->frame[0] + anim->numFrames[0];
    } else {
        time = anim->frame[0] + (anim->speed[0] * rate);
    }

    hold = anim->anim[0] & 0x8000;
    if (obj != NULL) {
        hold |= obj->mvmt_control & 0xE;
        obj->unk1FC &= ~4;
        dc_glbl_CharNum = obj - movement_obj;
        D_800D154C = obj;
    }

    jumped = 0;
    if (obj != NULL) {
        if ((D_800C35EA[mode] != 1) || (D_800C365E[mode] != 0)) {
            result = func_1506AD30(D_800D154C, time, 0);
            if (result != 0.0f) {
                time = result;
            }
        }
    }

    if (anim->speed[0] >= 0.0f) {
        if (((anim->numFrames[0] - 1.0f) <= time) && (anim->frame[0] < (anim->numFrames[0] - 1.0f)) && (anim->callback != NULL)) {
            oldAnimId = anim->frameRom[0];
            if ((obj != NULL) && (obj->mvmt_control & 8)) {
                obj->unk10C = 0;
                obj->events = NULL;
                anim->speed[0] = 0.0f;
            }
            if (D_800C35EA[0] == 0) {
                anim->callback();
            }
            if (oldAnimId != anim->frameRom[0]) {
                time = anim->frame[0];
                jumped = 1;
            }
        }

        if ((obj != NULL) && (obj->unk1FD != 0) && ((anim->numFrames[0] - 1.0f) <= time)) {
            obj->angle += obj->unk1FD << 8;
            obj->langle = obj->angle;
            obj->fangle = obj->angle;
            anim->frame[0] = anim->start[0];
            if (obj->mvmt_control & 4) {
                obj->unk21C = 0;
            }
        } else if ((hold == 0) && (anim->numFrames[0] <= time)) {
            anim->frame[0] = anim->start[0] + (time - anim->numFrames[0]);
            jumped = 1;
            if (obj != NULL) {
                obj->unk1FC |= 4;
            }
        } else if ((hold != 0) && ((anim->numFrames[0] - 1.0f) <= time)) {
            if (((obj != NULL) && (obj->mvmt_control & 0xA)) || (anim->anim[0] & 0x8000)) {
                anim->frame[0] = anim->numFrames[0] - 1.0f;
            } else if (anim->frame[0] < (anim->numFrames[0] - 1.0f)) {
                anim->frame[0] = anim->numFrames[0] - 1.0f;
                if ((obj != NULL) && (obj->mvmt_control & 4)) {
                    obj->unk21C = 0;
                }
            } else {
                anim->frame[0] = anim->start[0];
                if (obj != NULL) {
                    obj->speccnt = 0;
                    obj->unk1FC |= 2;
                }
            }
            if ((obj != NULL) && (obj->mvmt_control & 8)) {
                obj->unk10C = 0;
                obj->events = NULL;
                anim->speed[0] = 0.0f;
            }
            if (anim->frame[0] < 0.0f) {
                anim->frame[0] = 0.0f;
            }
        } else {
            anim->frame[0] = time;
        }
    } else if (time < anim->start[0]) {
        anim->frame[0] = anim->numFrames[0] + (time - anim->start[0]);
    } else {
        anim->frame[0] = time;
    }

    if (anim->unk3C > 0) {
        anim->unk3C -= anim->unk3A * D_800BE9E4;
    }
    if (obj != NULL) {
        if (jumped) {
            func_1506AD30(D_800D154C, anim->frame[0], 0);
        }
        // structs.h declares 0xB4 as s32 padB4; it holds the object's f32 animation time
        obj->lastEventTime = anim->frame[0];
    }
}

/* Steps the animation of every active object; with onlyType4 set, only objects whose unk5 is 4. */
void func_1507C22C(s32 onlyType4) {
    s32 i;
    Actor *obj;
    AnimInstance *anim;

    for (i = 0; i < 25; i++) {
        obj = &movement_obj[i];
        if (obj->type == 0) continue;
        if (obj->task_attrib & 0x200) continue;
        if (onlyType4 && (obj->drawtype != 4)) continue;
        if (D_800C3638 && (D_800C3654[0] == 0) && (func_150229E4(obj) == 0)) continue;
        anim = obj->unk2D0;
        if ((anim != NULL) && obj->unk2FA) {
            func_1507BDB0(anim, obj->speedup, obj, onlyType4);
        }
    }
}

/* Copies src's animation time onto dst, clamped to dst's last frame. */
void func_1507C324(Actor *dst, Actor *src) {
    AnimInstance *dstAnim = dst->unk2D0;
    AnimInstance *srcAnim = src->unk2D0;
    f32 lastFrame;

    if (dstAnim != NULL && srcAnim != NULL) {
        dstAnim->frame[0] = srcAnim->frame[0];
        lastFrame = dstAnim->numFrames[0] - 1.0f;
        if (lastFrame <= dstAnim->frame[0]) {
            dstAnim->frame[0] = lastFrame;
        }
    }
}
