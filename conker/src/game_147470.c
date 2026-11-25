#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A spinning, bobbing pickup that the player collects by walking into it (0xA0 bytes, like struct131). */
typedef struct Obj15119FC0 {
    /* 0x00 */ f32 rotX; // tilt in degrees
    /* 0x04 */ f32 rotY; // spin in degrees, 0..360
    /* 0x08 */ f32 rotZ; // tilt in degrees
    /* 0x0C */ u8 pad0C[4];
    /* 0x10 */ s16 position[3];
    /* 0x16 */ u8 pad16[0x26];
    /* 0x3C */ s32 flags; // < 0: inactive; upper 16 bits: respawn delay in seconds (0 = never)
    /* 0x40 */ u8 pad40[0xF];
    /* 0x4F */ s8 state;
    /* 0x50 */ u8 pad50[0x1E];
    /* 0x6E */ s8 removed;
    /* 0x6F */ u8 pad6F;
    /* 0x70 */ u8 flags70;
    /* 0x71 */ u8 pad71[0xB];
    /* 0x7C */ f32 phase;
    /* 0x80 */ s32 baseHeight;
    /* 0x84 */ s32 respawnTimer;
    /* 0x88 */ u8 pad88[2];
    /* 0x8A */ u8 alpha;
    /* 0x8B */ u8 pad8B[0x15];
} Obj15119FC0;

extern s32 func_15060BA4(struct127 *, s32);
extern void func_1507C3E0(struct127 *, s16 *, s16 *, s16 *);
extern void func_1508EE0C(u16, u16);
extern void func_151D69B4(Obj15119FC0 *, struct127 *);

/* Animates the pickup and lets any nearby player collect it; afterwards it fades back in or is removed. */
void func_15119FC0(Obj15119FC0 *obj) {
    s32 i;
    f32 playerY;
    s16 height;
    s16 width;
    s16 depth;
    f32 distSq;

    if (obj->respawnTimer < 32) {
        if (obj->baseHeight == 0) {
            obj->baseHeight = obj->position[1];
        }
        // phase runs over 0..6*pi: one full spin, three tilt wobbles, six bobs
        obj->rotX = sinf(obj->phase) * 15.0f;
        obj->rotY = obj->phase * 19.098593f; // 60 / pi
        obj->rotZ = cosf(obj->phase) * 15.0f;
        obj->phase += 0.08726647f * D_800BE9A4; // 5 degrees per frame
        if (18.849556f <= obj->phase) {
            obj->phase -= 18.849556f;
        }
        obj->position[1] = (s32)(cosf(obj->phase + obj->phase) * 10.0f) + obj->baseHeight + 10;

        if (obj->respawnTimer == 0) {
            for (i = 0; i < D_8008FD8C; i++) {
                if (D_800CC2D0[i].interaction_state == 0 || D_800CC2D0[i].unk127 == 0xFF || obj->flags < 0) {
                    continue;
                }
                distSq = ((f32)obj->position[0] - D_800CC2D0[i].x_position) * ((f32)obj->position[0] - D_800CC2D0[i].x_position) +
                         ((f32)obj->position[2] - D_800CC2D0[i].z_position) * ((f32)obj->position[2] - D_800CC2D0[i].z_position);
                if (distSq < 80.0f * 80.0f) {
                    func_1507C3E0(&D_800CC2D0[i], &height, &width, &depth);
                    obj->state = 0x31;
                    playerY = D_800CC2D0[i].y_position;
                    if (playerY < (f32)obj->position[1] + 80.0f && (f32)obj->position[1] - 80.0f < playerY + (f32)height &&
                        D_800CC2D0[i].health > 0) {
                        if (func_15060BA4(&D_800CC2D0[i], 1)) {
                            if (obj->flags != 0) {
                                obj->respawnTimer = (u16)(obj->flags >> 16) * 60;
                                obj->flags70 |= 4;
                            } else {
                                obj->removed = TRUE;
                                func_1508EE0C(2, (struct131 *)obj - D_800DBEF4);
                            }
                            obj->state = 0x20;
                            func_10010344(0x1CF, &D_800CC2D0[i], 0x7D00, 200, 2500);
                            func_151D69B4(obj, &D_800CC2D0[i]);
                        } else {
                            // fade from 25% (at the centre) to 100% (at the edge) of full opacity
                            distSq *= 0.0001171875f; // 0.75 / (80 * 80)
                            obj->state = 0x31;
                            obj->alpha = (0.25f + distSq) * 255.0f;
                        }
                        break;
                    }
                } else {
                    obj->alpha = 255;
                    obj->state = 0x21;
                }
            }
        } else {
            // fade back in over the last 32 frames of the respawn delay
            obj->state = 0x31;
            obj->alpha = (32 - obj->respawnTimer) << 3;
        }
    }
    if (obj->respawnTimer > 0) {
        obj->respawnTimer -= D_800BE9E4;
        if (obj->respawnTimer <= 0) {
            obj->respawnTimer = 0;
            obj->alpha = 255;
            obj->state = 0x21;
        }
    }
}
