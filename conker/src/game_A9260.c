#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* struct197 in structs.h stops at 0x1C; func_1507BDB0 reads 0x20, 0x28, 0x3A and
 * 0x3C off the same object (Actor::unk2D0), so a file-local widened view is
 * used here rather than retyping the shared header. */
typedef struct AnimPlay {
    /* 0x00 */ void (*callback)(void); // called once when playback reaches the last frame
    /* 0x04 */ u16 flags;             // 0x8000: hold on the last frame instead of looping
    /* 0x06 */ u8  pad6[0x2];
    /* 0x08 */ f32 time;
    /* 0x0C */ u8  padC[0x4];
    /* 0x10 */ f32 speed;
    /* 0x14 */ u8  pad14[0x4];
    /* 0x18 */ f32 end;
    /* 0x1C */ u8  pad1C[0x4];
    /* 0x20 */ f32 start;
    /* 0x24 */ u8  pad24[0x4];
    /* 0x28 */ s32 animId;
    /* 0x2C */ u8  pad2C[0xE];
    /* 0x3A */ s16 step;
    /* 0x3C */ s16 countdown;
} AnimPlay;

extern u8 D_800BEA0C;
extern u8 D_800C365E[];

f32 func_1506AD30(Actor *obj, f32 time, s32 arg2);

/* Advances an animation playback by rate, handling looping, hold and end callbacks. */
void func_1507BDB0(AnimPlay *anim, f32 rate, Actor *obj, s32 mode) {
    f32 time;
    s32 hold;
    u8 jumped;
    f32 result;
    s32 oldAnimId;

    if (anim->animId == 0) {
        return;
    }

    if (obj != NULL) {
        if (D_800C35EA == 0) {
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

    if (anim->end < (anim->speed * rate)) {
        time = anim->end + anim->time;
    } else {
        time = (anim->speed * rate) + anim->time;
    }

    hold = anim->flags & 0x8000;
    if (obj != NULL) {
        hold |= obj->unkF4 & 0xE;
        obj->unk1FC &= ~4;
        D_800C3E78 = obj - D_800CC2D0;
        D_800D154C = obj;
    }

    jumped = 0;
    if (obj != NULL) {
        if (((&D_800C35EA)[mode] != 1) || (D_800C365E[mode] != 0)) {
            result = func_1506AD30(D_800D154C, time, 0);
            if (result != 0.0f) {
                time = result;
            }
        }
    }

    if (anim->speed >= 0.0f) {
        if (((anim->end - 1.0f) <= time) && (anim->time < (anim->end - 1.0f)) && (anim->callback != NULL)) {
            oldAnimId = anim->animId;
            if ((obj != NULL) && (obj->unkF4 & 8)) {
                obj->unk10C = 0;
                obj->events = NULL;
                anim->speed = 0.0f;
            }
            if (D_800C35EA == 0) {
                anim->callback();
            }
            if (oldAnimId != anim->animId) {
                time = anim->time;
                jumped = 1;
            }
        }

        if ((obj != NULL) && (obj->unk1FD != 0) && ((anim->end - 1.0f) <= time)) {
            obj->unk76 += obj->unk1FD << 8;
            obj->unk7A = obj->unk76;
            obj->unk78 = obj->unk76;
            anim->time = anim->start;
            if (obj->unkF4 & 4) {
                obj->unk21C = 0;
            }
        } else if ((hold == 0) && (anim->end <= time)) {
            anim->time = (time - anim->end) + anim->start;
            jumped = 1;
            if (obj != NULL) {
                obj->unk1FC |= 4;
            }
        } else if ((hold != 0) && ((anim->end - 1.0f) <= time)) {
            if (((obj != NULL) && (obj->unkF4 & 0xA)) || (anim->flags & 0x8000)) {
                anim->time = anim->end - 1.0f;
            } else if (anim->time < (anim->end - 1.0f)) {
                anim->time = anim->end - 1.0f;
                if ((obj != NULL) && (obj->unkF4 & 4)) {
                    obj->unk21C = 0;
                }
            } else {
                anim->time = anim->start;
                if (obj != NULL) {
                    obj->unk138 = 0;
                    obj->unk1FC |= 2;
                }
            }
            if ((obj != NULL) && (obj->unkF4 & 8)) {
                obj->unk10C = 0;
                obj->events = NULL;
                anim->speed = 0.0f;
            }
            if (anim->time < 0.0f) {
                anim->time = 0.0f;
            }
        } else {
            anim->time = time;
        }
    } else if (time < anim->start) {
        anim->time = (time - anim->start) + anim->end;
    } else {
        anim->time = time;
    }

    if (anim->countdown > 0) {
        anim->countdown -= anim->step * D_800BE9E4;
    }
    if (obj != NULL) {
        if (jumped) {
            func_1506AD30(D_800D154C, anim->time, 0);
        }
        // structs.h declares 0xB4 as s32 padB4; it holds the object's f32 animation time
        obj->lastEventTime = anim->time;
    }
}

s32 func_150229E4(Actor *obj);

/* Steps the animation of every active object; with onlyType4 set, only objects whose unk5 is 4. */
void func_1507C22C(s32 onlyType4) {
    s32 i;
    Actor *obj;
    struct197 *anim;

    for (i = 0; i < 25; i++) {
        obj = &D_800CC2D0[i];
        if (obj->interaction_state == 0) continue;
        if (obj->unk25C & 0x200) continue;
        if (onlyType4 && (obj->unk5 != 4)) continue;
        // matching: the s32 D_800C3654 is tested through its high byte (lbu)
        if (D_800C3638 && (*(u8 *)&D_800C3654 == 0) && (func_150229E4(obj) == 0)) continue;
        anim = obj->unk2D0;
        if ((anim != NULL) && obj->unk2FA) {
            func_1507BDB0((AnimPlay *)anim, obj->unk48, obj, onlyType4);
        }
    }
}

/* Copies src's animation time onto dst, clamped to dst's last frame. */
void func_1507C324(Actor *dst, Actor *src) {
    AnimPlay *dstAnim = (AnimPlay *)dst->unk2D0;
    AnimPlay *srcAnim = (AnimPlay *)src->unk2D0;
    f32 lastFrame;

    if (dstAnim != NULL && srcAnim != NULL) {
        dstAnim->time = srcAnim->time;
        lastFrame = dstAnim->end - 1.0f;
        if (lastFrame <= dstAnim->time) {
            dstAnim->time = lastFrame;
        }
    }
}
