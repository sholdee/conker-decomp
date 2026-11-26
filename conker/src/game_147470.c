#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A spinning, bobbing pickup that the player collects by walking into it. Its param word is < 0 while
 * inactive; its upper 16 bits are the respawn delay in seconds (0 = never).
 */
typedef struct {
    /* 0x7C */ f32 phase;
    /* 0x80 */ s32 baseHeight;
    /* 0x84 */ s32 respawnTimer;
} PickupWork; // WorldObject.touch

#define WORK(obj) ((PickupWork *)&(obj)->touch)


/* Animates the pickup and lets any nearby player collect it; afterwards it fades back in or is removed. */
void func_15119FC0(WorldObject *obj) {
    s32 i;
    f32 playerY;
    s16 height;
    s16 width;
    s16 depth;
    f32 distSq;

    if (WORK(obj)->respawnTimer < 32) {
        if (WORK(obj)->baseHeight == 0) {
            WORK(obj)->baseHeight = obj->y;
        }
        // phase runs over 0..6*pi: one full spin, three tilt wobbles, six bobs
        obj->t = sinf(WORK(obj)->phase) * 15.0f;
        obj->u = WORK(obj)->phase * 19.098593f; // 60 / pi
        obj->v = cosf(WORK(obj)->phase) * 15.0f;
        WORK(obj)->phase += 0.08726647f * D_800BE9A4; // 5 degrees per frame
        if (18.849556f <= WORK(obj)->phase) {
            WORK(obj)->phase -= 18.849556f;
        }
        obj->y = (s32)(cosf(WORK(obj)->phase + WORK(obj)->phase) * 10.0f) + WORK(obj)->baseHeight + 10;

        if (WORK(obj)->respawnTimer == 0) {
            for (i = 0; i < D_8008FD8C; i++) {
                if (movement_obj[i].type == 0 || movement_obj[i].playernumber == 0xFF || obj->moveData.word < 0) {
                    continue;
                }
                distSq = (obj->x - movement_obj[i].xpos) * (obj->x - movement_obj[i].xpos) +
                         (obj->z - movement_obj[i].zpos) * (obj->z - movement_obj[i].zpos);
                if (distSq < 80.0f * 80.0f) {
                    func_1507C3E0(&movement_obj[i], &height, &width, &depth);
                    obj->object_status = 0x31;
                    playerY = movement_obj[i].ypos;
                    if (playerY < obj->y + 80.0f && obj->y - 80.0f < playerY + height &&
                        movement_obj[i].energy > 0) {
                        if (func_15060BA4(&movement_obj[i], 1)) {
                            if (obj->moveData.word != 0) {
                                WORK(obj)->respawnTimer = (u16)(obj->moveData.word >> 16) * 60;
                                obj->state |= 4;
                            } else {
                                obj->inactive = TRUE;
                                func_1508EE0C(2, obj - db_Object_array);
                            }
                            obj->object_status = 0x20;
                            func_10010344(0x1CF, &movement_obj[i], 0x7D00, 200, 2500);
                            func_151D69B4(obj, &movement_obj[i]);
                        } else {
                            // fade from 25% (at the centre) to 100% (at the edge) of full opacity
                            distSq *= 0.0001171875f; // 0.75 / (80 * 80)
                            obj->object_status = 0x31;
                            obj->alpha = (0.25f + distSq) * 255.0f;
                        }
                        break;
                    }
                } else {
                    obj->alpha = 255;
                    obj->object_status = 0x21;
                }
            }
        } else {
            // fade back in over the last 32 frames of the respawn delay
            obj->object_status = 0x31;
            obj->alpha = (32 - WORK(obj)->respawnTimer) << 3;
        }
    }
    if (WORK(obj)->respawnTimer > 0) {
        WORK(obj)->respawnTimer -= D_800BE9E4;
        if (WORK(obj)->respawnTimer <= 0) {
            WORK(obj)->respawnTimer = 0;
            obj->alpha = 255;
            obj->object_status = 0x21;
        }
    }
}
