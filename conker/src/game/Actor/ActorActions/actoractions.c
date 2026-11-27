#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/attach.h"
#include "conker/boot.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/event.h"
#include "conker/face.h"
#include "conker/game.h"
#include "conker/math.h"
#include "conker/mp3.h"
#include "conker/particle.h"
#include "conker/script.h"
#include "conker/sound.h"
#include "conker/text.h"
#include "conker/trail.h"
#include "conker/weapon.h"
#include "conker/zone.h"

/* Packet handed to actor_runHitHandler by actor_throwGrenade. */
typedef struct ThrowPacket {
    Actor *thrower;
    Vec3f position;
    s32 throwerId;          /* object index + 1 */
    volatile s32 anim;      // matching: volatile keeps the anim in its stack slot across the call
} ThrowPacket;

/* Returns the angle from this object to the object it is linked to (unk222). */
s16 actor_getAngleToLinked(Actor *arg0) {
    Actor *other;

    other = &movement_obj[arg0->unk222];
    return actor_getDirectionAngle(other->xpos - arg0->xpos, arg0->zpos - other->zpos, 0);
}

void func_15063404(Actor *arg0) {
    Actor *self = &movement_obj[arg0 - movement_obj];

    self->mv->rt_context_b = 9;
    collision_createPartSet(arg0, 0x1D, 1.0f, 0);
    collision_createPartSet(arg0, 0x1E, 1.0f, 0);
    arg0->nojump = 20;
    arg0->norun = 0;
    arg0->count5 = 0;
    self->mv->gun_before_reload = 60;
    self->mv->weapon_status = 2;
    actor_playAnim(arg0, 0x7F, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}


void func_150634E4(Actor *arg0) {
    movement_obj[arg0 - movement_obj].mv->rt_context_b = 0;
    movement_obj[arg0 - movement_obj].mv->weapon_status = 0;
    collision_removePartSet(arg0, 0x1D);
    collision_removePartSet(arg0, 0x1E);
    arg0->nojump = 0;
    arg0->norun = 0;
    arg0->count5 = 0;
}

void func_15063570(Actor *arg0) {
    movement_obj[arg0 - movement_obj].mv->rt_context_b = 0x3B;
    movement_obj[arg0 - movement_obj].mv->weapon_status = 2;
    collision_createPartSet(arg0, 0x89, 1.0f, 0);
    arg0->nojump = 20;
    arg0->norun = 0;
    arg0->count5 = 0;
    actor_playAnim(arg0, 0x221, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}


void actor_castAimRay(Actor *arg0, f32 arg1) {
    ActorCtl *ctl = arg0->mv;

    collision_castRay(arg0, ctl->aimEye.x, ctl->aimEye.y, ctl->aimEye.z,
                  ctl->aimDir.x, ctl->aimDir.y, ctl->aimDir.z, &ctl->rayHit,
                  arg1, 0, 0, 0, -1, 0, 0);
    actor_markRayTargetSniped(arg0);
}

/* Tells the actor hit by the last ray (if any) that this object is targeting it. */
void actor_markRayTargetSniped(Actor *arg0) {
    Actor *target;
    ActorCtl *targetCtl;

    target = arg0->mv->rayHit.obj;
    if (target != NULL) {
        targetCtl = target->mv;  // matching: the null test uses this copy; the stores reload mv
        if (targetCtl != NULL) {
            target->mv->being_sniped = 30;
            target->mv->being_s_char = arg0 - movement_obj;
        }
    }
}

/* Runs the work each object queued in its control block's `pending` flags this frame. */
void actor_runPending(void) {
    Actor *actor;
    s32 index;
    s32 action;
    Vec3f position;
    Vec3f direction;
    Vec3f side;
    Vec3f up;
    ActorCtl *ctl;
    Camera *camera;

    gActionsPending = 0;
    actor = movement_obj;
    for (index = 0; index < 25; index++, actor++) {
        if ((actor->type != 0) && (actor->mv != NULL) && (actor->mv->deferredActions != 0)) {
            dc_glbl_CharNum = index;
            gCurrentActor = actor;
            ctl = actor->mv;
            if (ctl->deferredActions & 1) {
                actor_castAimRay(actor, 0.0f);
                ctl = actor->mv;
            }
            if (ctl->deferredActions & 4) {
                particle_castGunRay(actor, 0);
                ctl = actor->mv;
            }
            if (ctl->deferredActions & 8) {
                particle_castGunRay(actor, 1);
                ctl = actor->mv;
            }
            if (ctl->deferredActions & 2) {
                action = ctl->rt_context_b & ~0x80;
                switch (action) {
                case 0x18:
                case 0x41:
                    actor_fireShot(actor, 6);
                    ctl = actor->mv;
                    ctl->shotsfired++;
                    camera = actor->CameraPtr;
                    if (camera != NULL) {
                        camera_startShake(camera, 1);
                    }
                    break;
                case 0x14:
                case 0x3F:
                    if (action == 0x14) {
                        actor_fireShot(actor, 2);
                    } else {
                        actor_fireShot(actor, 0x12);
                    }
                    ctl = actor->mv;
                    ctl->shotsfired++;
                    camera = actor->CameraPtr;
                    if (camera != NULL) {
                        camera_startShake(camera, 1);
                    }
                    break;
                case 0x24:
                    if (collision_applyRayHit(actor, &ctl->rayHit, 7) != 2) {
                        ctl = actor->mv;
                        if (ctl->unkAF != 0) {
                            ctl->unkAF = 0;
                            if (sound_isActorSound1Playing(actor) == 0) {
                                sound_playActorSound1Delayed(math_random() % 3 + 0x53B, actor, 0x7D00, 800, 3000, 30);
                            }
                        }
                    }
                    camera = actor->CameraPtr;
                    if (camera != NULL) {
                        camera_startShake(camera, 1);
                    }
                    break;
                case 0x3B:
                    collision_applyRayHit(actor, &ctl->rayHit, 0x10);
                    weapon_getShotFrame(actor, &position, &direction, &side, &up);
                    weapon_spawnFannedParticles(&position, &direction, 0xFF, 1);
                    break;
                case 0x23:
                    collision_applyRayHit(actor, &ctl->rayHit, 8);
                    camera = actor->CameraPtr;
                    if (camera != NULL) {
                        camera_startShake(camera, 0);
                    }
                    break;
                }
                ctl = actor->mv;
            }
            ctl->deferredActions = 0;
        }
    }
}

/* Stores in gWaistTiltTarget (and returns) how far this object's heading is off the bearing to its
 * linked object, clamped to +-16000. */
s32 actor_getWaistOffsetToLinked(Actor *arg0) {
    gWaistTiltTarget = arg0->langle - arg0->mv->pssd_waist_tilt - actor_getAngleToLinked(arg0);
    if (gWaistTiltTarget > 16000) {
        gWaistTiltTarget = 16000;
    }
    if (gWaistTiltTarget < -16000) {
        gWaistTiltTarget = -16000;
    }
    return gWaistTiltTarget;
}

/* When any of `mask` is set in `buttons`, turns the object's unk12 offset from the stick (or
 * toward its linked object), limited to +-9000; the excess turns the whole object. */
s32 actor_turnWaist(Actor *arg0, s32 buttons, s32 mask) {
    if (buttons & mask) {
        gWaistTiltTarget = gCurrentPad->stick_x * 200;
        if (arg0->mv->bot != 0) {
            gWaistTiltTarget = actor_getWaistOffsetToLinked(arg0);
        }

        gWaistTiltDamping = 12.0f;
        gWaistTiltTarget = arg0->mv->pssd_waist_tilt + ((gWaistTiltTarget * gActorStepPercent) / 2000);

        if (gWaistTiltTarget < -9000) {
            arg0->angle = arg0->langle = (arg0->langle - gWaistTiltTarget) - 9000;
            gWaistTiltTarget = -9000;
        }

        if (gWaistTiltTarget > 9000) {
            arg0->angle = arg0->langle = (arg0->langle - gWaistTiltTarget) + 9000;
            gWaistTiltTarget = 9000;
        }

        arg0->mv->pssd_waist_tilt = gWaistTiltTarget;
        return 1;
    }
    return 0;
}

/* Throws one of the object's grenades (anim 0x64 for ids 0x75 and 0xB1, 0x2C otherwise). */
void actor_throwGrenade(Actor *arg0) {
    ThrowPacket packet;

    if ((arg0->character_type == 0x75) || (arg0->character_type == 0xB1)) {
        packet.anim = 0x64;
    } else {
        packet.anim = 0x2C;
    }

    if (attach_find(arg0, packet.anim, 0) != NULL) {
        if (arg0->ob_matrix_ptr != NULL) {
            math_transformOffset(NULL, &packet.position, &arg0->ob_matrix_ptr[4]);
        } else {
            packet.position.x = arg0->xpos;
            packet.position.y = arg0->ypos;
            packet.position.z = arg0->zpos;
        }

        packet.throwerId = (arg0 - movement_obj) + 1;
        packet.thrower = arg0;
        actor_runHitHandler(arg0, 0xB, &packet);
        collision_removePartSet(arg0, packet.anim);

        if (arg0->mv->numGrenades != 0) {
            arg0->mv->numGrenades--;
        }
        arg0->mv->weapon_status = 0;
    }
}

/* Returns the active object with id `id` that is linked (unk65) to arg0, or NULL.
 * (asm-differ shows relocation-addend diffs here: the .s names interior symbols of
 * movement_obj[], C spells them base+offset; the linked bytes are identical.) */
Actor *actor_findLinked(Actor *arg0, s32 id) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((movement_obj[i].type != 0) && (id == movement_obj[i].character_type) &&
            (((arg0 - movement_obj) + 1) == movement_obj[i].attach_char)) {
            return &movement_obj[i];
        }
    }
    return NULL;
}

/* Starts `anim` as a new action when the object stands on the ground with nothing else going
 * on and B is pressed. Returns `anim`, or 999 when the action can't start. */
s32 actor_tryStartBAction(Actor *arg0, u16 anim, u16 flags, s32 action, s32 buttons) {
    s32 state;

    if ((buttons & B_BUTTON) && (arg0->shad == 0.0f) && (arg0->yspd <= 0.0f) &&
        (arg0->mv->ladder == 0) && (arg0->animseq.uh != anim)) {
        state = arg0->mv->weapon_status;
        if ((state == 0 || state == 2) && (arg0->mv->jump_on_char == 0)) {
            if (action != 0 && arg0->nojump != 0) {
                return 999;
            }
            if (flags & 1) {
                arg0->nojump = 20;
            }
            if (action != 0) {
                if (arg0->mv->weapon_status == 2) {
                    return 999;
                }
                arg0->mv->weapon_status = 1;
            } else {
                arg0->mv->weapon_status = 3;
            }
            arg0->mv->rt_context_b = action;
            arg0->norun = 0xFF;
            arg0->count5 = 0xFF;
            return anim;
        }
    }
    return 999;
}

/* Fires a shot of kind `kind` (2, 3, 6, 0x12) from the object's muzzle (unk13C..unk144),
 * aimed along its yaw/pitch or, when unk109 is set, at the point unkB8..unkC0. */
void actor_fireShot(Actor *arg0, s32 kind) {
    f32 velX;
    f32 velY;
    f32 velZ;
    f32 dist;
    f32 pos[3];
    f32 off[2];
    f32 height;
    f32 radius;
    f32 angle;
    f32 pitch;
    f32 yaw;
    f32 dx;
    f32 dy;
    f32 dz;
    ActorCtl *ctl;

    dist = 300.0f;
    height = 0.0f;
    radius = 0.0f;
    switch (kind) {
    case 2:
        radius = 27.0f;
        break;
    case 3:
    case 6:
        height = -11.0f;
        dist = 500.0f;
        break;
    }
    angle = arg0->mv->aimYaw * 0.01745329238f;
    off[0] = sinf(angle) * radius;
    off[1] = cosf(angle) * radius;
    pos[0] = off[0] + arg0->mv->aimEye.x;
    pos[1] = arg0->mv->aimEye.y + height;
    pos[2] = off[1] + arg0->mv->aimEye.z;
    ctl = arg0->mv;
    if (ctl->rayHit.hitType == 0) {
        pitch = -ctl->aimPitch;
        yaw = ctl->aimYaw + 90.0f;
    } else {
        dx = ctl->rayHit.pos.x - pos[0];
        dy = ctl->rayHit.pos.y - pos[1];
        dz = ctl->rayHit.pos.z - pos[2];
        pitch = math_atan2(dy, sqrtf(dx * dx + dz * dz)) * 57.29577637f;
        yaw = math_atan2(dx, dz) * 57.29577637f;
    }
    actor_splitVelocity((s16)(pitch * 182.0444489f), dist, 0.0f, &velX, &velY, &velZ);
    velY = -velY;
    event_spawnProjectile(yaw, velX, velY, 0.0f, 100, kind, 0.0f, 0.5f, pos, 1, 1);
}

/* Picks animA or animB at random; if one of them is already playing, picks animC instead
 * (or returns 999 while unk107 < 40). Returns 999 when animC is already playing. */
u16 actor_pickRandomAnim(Actor *arg0, u16 animA, u16 animB, u16 animC) {
    u16 selected;
    Actor *actor = arg0;  // matching: the copy reserves the extra stack slot IDO gave the original
    u16 angle;

    angle = 0;
    selected = animA;
    if (math_random() & 1) {
        selected = animB;
    }
    if ((actor->animseq.uh == animA) || (actor->animseq.uh == animB)) {
        if (actor->ivehitit < 40) {
            return 999;
        }
        selected = animC;
    }
    if (actor->animseq.uh == animC) {
        selected = 999;
    }
    if (actor_findTargetInCone(actor, &angle, 0, 0xFE, 0x40) != 0xFF) {
        actor->angle = angle;
    }
    actor->count5 = 0xFF;
    return selected;
}


/* Handles the scripted move requested in unk7B, then the default B-button move; plays the
 * chosen animation unless `noPlay` is set. */
void func_150642AC(Actor *arg0, s32 buttons, f32 arg2, s32 noPlay, s32 lockB) {
    s32 anim;
    f32 speed;
    f32 blend;
    Actor *obj;
    s32 pad;  // matching: unused stack slot
    MtxS16 *mtx;
    f32 dx;
    f32 dist;
    f32 dz;
    s16 x;
    s16 z;
    f32 pushX;
    f32 pushY;
    f32 pushZ;

    anim = 0;
    speed = 1.0f;
    blend = 4.0f;
    if (lockB) {
        buttons &= ~B_BUTTON;
        gActorNewButtons &= ~B_BUTTON;
    }
    if (arg0->mv->inst_context != 0) {
        switch (arg0->mv->inst_context) {
        case 1: {
            s16 pad;  // matching: unused stack slot

            arg0->count5 = 0xFF;
            arg0->norun = 0xFF;
            x = (arg0->taskint[0] >> 16) & 0xFFFF;
            z = arg0->taskint[0] & 0xFFFF;
            arg0->animseq.uh = 0;
            arg0->mv->gpoint = 0xD;
            arg0->mv->swing_xvel = x;
            arg0->mv->swing_zvel = z;
            anim = 0x96;
            arg0->angle = arg0->taskint[1] & 0xFFFF;
        }
            break;
        case 2:
            speed = 0.8000000119f;
            anim = 0xDD;
            arg0->norun = 0xFF;
            arg0->count5 = 0xFF;
            break;
        case 3:
            gEventParam = 1;
            func_15074664();
            if (arg0->inwater != 0 || (s32)arg0->shad != 0) {
                collision_createPartSet(arg0, 0x17, 1.0f, 0);
                arg0->mv->weapon_status = 2;
            } else {
                arg0->norun = 0xFF;
                arg0->nojump = 0xFF;
                arg0->count5 = 0xFF;
                speed = 3.0f;
                anim = 0x151;
                arg0->mv->weapon_status = 1;
            }
            break;
        case 4:
            gEventParam = 0;
            func_15074664();
            if (arg0->inwater != 0 || (s32)arg0->shad != 0) {
                collision_removePartSet(arg0, 0x17);
                arg0->mv->weapon_status = 0;
            } else {
                arg0->norun = 0xFF;
                arg0->nojump = 0xFF;
                arg0->count5 = 0xFF;
                speed = 3.0f;
                anim = 0x14E;
                arg0->mv->weapon_status = 3;
            }
            break;
        case 5:
            obj = actor_findByType(0x6C);
            arg0->count3 = 1;
            if (obj->taskint[0] == 0) {
                arg0->unk1CC = arg0->ypos;
                obj->taskint[0] = 100;
                anim = 0x38;
            }
            if (obj->taskint[0] >= 100) {
                if (obj->ob_matrix_ptr != NULL) {
                    mtx = &((MtxS16 *)obj->ob_matrix_ptr)[5];
                    arg0->ypos = mtx->intPart[3][1] + 20;
                }
            }
            if (buttons & A_BUTTON) {
                if (obj->taskint[0] == 100) {
                    obj->taskint[0] = 0x7A;
                    obj->taskint[1] = 0x7A;
                    anim = 0x16;
                    actor_playSound(0x38E, arg0);
                }
            }
            break;
        case 6:
        case 8:
            if ((buttons & B_BUTTON) && (arg2 <= 0.0f)) {
                speed = 2.0f;
                arg0->count5 = 0xFF;
                arg0->norun = 0xFF;
                anim = 0x33;
            }
            break;
        case 9:
            anim = 0x22D;
            arg0->norun = 0xFF;
            arg0->count5 = 0xFF;
            break;
        case 10:
            if (arg0->shad <= 1.0f) {
                anim = 0x19A;
                arg0->count5 = 0xFF;
            }
            arg0->norun = 0xC8;
            arg0->nojump = 0xC8;
            break;
        case 12:
            if (arg0->shad <= 1.0f) {
                anim = 0x2E7;
                arg0->count5 = 0xFF;
            }
            arg0->norun = 0xC8;
            arg0->nojump = 0xC8;
            break;
        case 11:
            dx = arg0->xpos - -1884.0f;
            dz = arg0->zpos - 100.0f;
            dist = sqrtf(dx * dx + dz * dz);
            if (dist < 1000.0f) {
                u16 angle;

                angle = actor_getDirectionAngle(-dx, dz, 0);
                actor_splitVelocity(angle, 20.0f, 0.0f, &pushX, &pushY, &pushZ);
                actor_stepVector2D(pushX, pushY, (1200.0f - dist) * 0.001000000047f, &arg0->ob_overslopex, &arg0->ob_overslopey);
            }
            break;
        default:
            break;
        }
        arg0->mv->inst_context = 0;
    }
    if (arg0->mv->AimState == 0 && anim == 0 && (buttons & B_BUTTON) && (arg0->mv->transport & 0xF) != 1 &&
        arg2 <= 0.0f && arg0->norun != 0xFF) {
        if (arg0->lifting >= 100) {
            if (!(arg0->task_attrib & 0x20)) {
                anim = 0xD6;
                speed = 1.149999976f;
                if (gIsMultiplayer != 0) {
                    anim = 0x118;
                    speed = 1.0f;
                }
                if (level == LEVEL_WMP_BEACH_WAR) {
                    anim = 0x28D;
                }
                arg0->count5 = 0xFF;
                arg0->norun = 0xFF;
            }
        } else if ((arg0->mv->context_b == 0 && arg0->mv->unk7C == 0) || arg0->mv->transport == 2) {
            if (level != LEVEL_WMP_MAIN_WAR && level != LEVEL_WMP_BEACH && level != LEVEL_WMP_BEACH_WAR && level != LEVEL_WMP_TUNNELS) {
                anim = 0x33;
                if (arg0->mv->pissed_up != 0) {
                    arg0->count5 = 0xFF;
                    arg0->norun = 0xFF;
                    speed = 1.600000024f;
                } else {
                    arg0->count5 = 0xFF;
                    if (gIsMultiplayer == 0) {
                        anim = 0x1DC;
                    } else if (D_800E9D00 & 0x400) {
                        anim = 0x355;
                    } else if (D_800E9D00 & 0x800) {
                        anim = 0x1DC;
                    } else {
                        anim = 0x354;
                    }
                    speed = 1.350000024f;
                    if (arg0->mv->bot == 0 && arg0->mv->transport == 0) {
                        arg0->norun = 0xFF;
                        arg0->yspd = -4.0f;
                        arg0->angle = arg0->mv->press_angle;
                        speed = 1.0;  // matching: double literal; 1.0f drops its .rodata entry
                        anim = 0x46;
                    }
                    arg0->count2 = 1;
                    blend = 0.0f;
                }
            }
        }
    }
    if (anim != 0 && anim != 999 && !noPlay) {
        actor_playAnim(arg0, anim, speed, blend, 0.0f, 0.0f, 0);
    }
}

/* Copies object srcIndex's animation position (unk2D0->unk8) to object dstIndex, keeping it
 * below dstIndex's unk18. */
void actor_copyAnimPosition(s32 srcIndex, s32 dstIndex) {
    Actor *objects;
    AnimInstance *dst;
    AnimInstance *src;

    objects = movement_obj;
    dst = objects[dstIndex].unk2D0;
    src = objects[srcIndex].unk2D0;

    if (dst != NULL && src != NULL) {
        dst->frame[0] = src->frame[0];
        if (dst->numFrames[0] <= dst->frame[0]) {
            dst->frame[0] = dst->numFrames[0] - 1.0f;
        }
    }
}

/* Plays the animation that pairs with what the linked object (unk65) is doing, and syncs
 * this object's animation to it. */
void actor_syncAnimToLinked(Actor *arg0) {
    Actor *partner;
    s32 anim;
    s32 variant;

    partner = &movement_obj[arg0->attach_char - 1];
    anim = 0xF;
    if (partner->character_type == 0xC) {
        anim = 0x5A;
        if (partner->animseq.uh == 0xC) {
            anim = 0x5B;
        } else if (partner->animseq.uh == 0xD) {
            anim = 0x5C;
        }
    } else if (partner->character_type == 0x53) {
        anim = 0x8C;
        if (partner->animseq.uh == 0xB) {
            anim = 0xA2;
        } else if (partner->animseq.uh == 0xD) {
            anim = 0xA3;
        }
    } else if (partner->character_type == 0x21) {
        anim = 0x169;
    } else if (partner->character_type == 0x8A) {
        variant = ((arg0->mv != NULL) && (arg0->mv->unk1B2 != 0)) ? 1 : 0;
        if (partner->intelwalk == 4) {
            anim = variant ? 0x350 : 0x1A4;
        } else {
            anim = variant ? 0x34F : 0x1A1;
        }
    }
    actor_playAnim(arg0, anim, 1.0f, 6.0f, 0.0f, 0.0f, 0);
    actor_copyAnimPosition(arg0->attach_char - 1, dc_glbl_CharNum);
}

/* Picks the object's movement animation from its action and speed, plays it, and returns it
 * (0 or 999 when nothing new plays). */
s32 actor_updateMoveAnim(Actor *arg0, f32 arg1, f32 arg2, s32 overrideAnim, f32 arg4) {
    s32 anim;
    f32 blend;
    f32 speed;
    ActorCtl *ctl;

    anim = 0;
    blend = 4.0f;
    ctl = arg0->mv;
    speed = 1.0f;
    if (ctl->bitten > 0) {
        anim = 0x4C;
        speed = arg1 / 24.0f;
    } else if (ctl->transport == 2) {
        anim = 5;
        if (ctl->unk50 > 0) {
            anim = 0x54;
        }
        blend = 2.0f;
    } else if (arg0->lifting >= 100) {
        if (arg0->speed < 1.0f) {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        } else {
            switch (movement_obj[gCurrentActor->lifting - 100].character_type) {
            case 0x8C:
                arg0->unkAA = 0x5A;
                anim = 0x1A6;
                speed = arg1 / 6.0f + 0.4f;
                if (arg0->speed > 14.0f) {
                    anim = 0x1A7;
                    speed = arg1 / 26.0f + 0.5f;
                }
                break;
            case 0x57:
                arg0->unkAA = 0x5A;
                anim = 0x116;
                speed = arg1 / 6.0f + 0.4f;
                if (arg0->speed > 14.0f) {
                    anim = 0x117;
                    speed = arg1 / 26.0f + 0.5f;
                }
                break;
            case 0xA8:
            case 0xA9:
                if (arg0->speed > 14.0f) {
                    anim = 0x1B1;
                    speed = arg1 / 22.0f + 0.4f;
                } else {
                    anim = 0x1B0;
                    speed = arg1 * 0.0625f + 0.6f;
                }
                break;
            case 0x13:
                anim = 0x14C;
                speed = arg1 / 6.0f;
                break;
            case 0x89:
                anim = 0x1FB;
                speed = arg1 / 6.0f;
                break;
            default:
                if (arg0->speed > 14.0f) {
                    anim = 0x2E4;
                    speed = arg1 * 0.0625f + 0.5f;
                } else {
                    anim = 0xD7;
                    speed = arg1 / 6.0f + 0.4f;
                }
                break;
            }
        }
    } else if (overrideAnim != 0 && ctl->rt_context_b == 0) {
        anim = overrideAnim;
        speed = arg1 / 15.0f + 0.6f;
    } else if (2.6f < arg0->stickspeed && (ctl->walk_attrib & 1)) {
        arg0->unkAA = 0x28;
        blend = 5.0f;
        speed = arg0->stickspeed / 15.0f + 0.5f;
        anim = actor_getDuckWalkAnim();
    } else if (ctl->rt_context_b == 0x16) {
        if (arg0->stickspeed >= 1.0f) {
            anim = ctl->unk1B2 != 0 ? 0x2C6 : 0xE8;
            arg0->unkAA = 0x1E;
            speed = arg1 * 0.0625f + 0.6f;
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 9) {
        if (arg0->stickspeed >= 1.0f) {
            if (gIsMultiplayer == 0) {
                arg0->unkAA = 0x37;
            }
            anim = 0x7E;
            if (gIsMultiplayer == 0) {
                speed = arg1 * 0.0625f + 0.6f;
            } else if (arg0->stickspeed >= 20.0f) {
                anim = 0x22C;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x38) {
        if (arg0->stickspeed >= 1.0f) {
            anim = 0x1E1;
            if (arg0->stickspeed >= 20.0f) {
                anim = 0x1E2;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x39) {
        if (arg0->animseq.uh != 0x1EB) {
            if (arg0->stickspeed >= 1.0f) {
                anim = 0x1EC;
                if (arg0->stickspeed >= 20.0f) {
                    anim = 0x1ED;
                    speed = arg1 / 22.0f + 0.4f;
                }
            } else {
                speed = 1.0f;
                anim = actor_updateIdleAnim();
            }
        } else {
            anim = 999;
        }
    } else if (ctl->rt_context_b == 0x37) {
        if (arg0->animseq.uh != 0x1F4) {
            if (arg0->stickspeed >= 1.0f) {
                anim = 0x1F5;
                if (arg0->stickspeed >= 20.0f) {
                    anim = 0x1F6;
                    speed = arg1 / 22.0f + 0.4f;
                }
            } else {
                speed = 1.0f;
                anim = actor_updateIdleAnim();
            }
        } else {
            anim = 999;
        }
    } else if (ctl->rt_context_b == 0x3B) {
        if (arg0->animseq.uh != 0x222 && arg0->animseq.uh != 0x236) {
            if (arg0->stickspeed >= 1.0f || ctl->door_walkf_time > 0) {
                anim = 0x223;
                arg0->unkAA = 0x37;
                if (arg0->stickspeed >= 20.0f) {
                    anim = 0x224;
                    speed = arg1 / 22.0f + 0.4f;
                }
            } else {
                speed = 1.0f;
                anim = actor_updateIdleAnim();
            }
        } else {
            anim = 999;
        }
    } else if (ctl->rt_context_b == 0x12 || ctl->rt_context_b == 0x18 || ctl->rt_context_b == 0x41) {
        if (arg0->stickspeed >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0xEC;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->stickspeed >= 20.0f) {
                anim = 0xF8;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x24) {
        if (arg0->stickspeed >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0x158;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->stickspeed >= 20.0f) {
                anim = 0x159;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x22) {
        if (arg0->stickspeed >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0x15B;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->stickspeed >= 20.0f) {
                anim = 0x15C;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x21) {
        if (arg0->stickspeed >= 1.0f) {
            arg0->unkAA = 0x6E;
            anim = 0x123;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->stickspeed >= 15.0f) {
                anim = 0x124;
                speed = arg1 / 27.0f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x26) {
        if (arg0->stickspeed >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0x178;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->stickspeed >= 20.0f) {
                anim = 0x179;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x3A) {
        if (arg0->stickspeed >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0x1FB;
            speed = arg1 * 0.0625f + 0.6f;
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x25) {
        if (arg0->stickspeed >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0xD7;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->stickspeed >= 20.0f) {
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x2D || ctl->rt_context_b == 0x2E) {
        if (arg0->stickspeed >= 1.0f) {
            anim = 0x1B0;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->stickspeed >= 20.0f) {
                anim = 0x1B1;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x23) {
        if (arg0->stickspeed >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0x161;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->stickspeed >= 20.0f) {
                anim = 0x162;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x15) {
        if (arg0->stickspeed >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0xED;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->stickspeed >= 20.0f) {
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x19 || ctl->rt_context_b == 0x40) {
        if (arg0->speed >= 1.0f) {
            anim = 0x112;
            blend = 8.0f;
            speed = arg1 * 0.125f + 0.3f;
            if (arg0->speed >= 18.0f) {
                anim = 0x111;
                speed = arg1 / 20.0f + 0.3f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->injured != 0 && arg2 > 1.0f) {
        anim = 0x6D;
        speed = arg1 / 6.5f + 0.3f;
        arg0->unkAA = 0x28;
        blend = 8.0f;
    } else if (ctl->pissed_up > 0) {
        if (arg2 < 1.0f) {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        } else {
            anim = ctl->pissed_walk;
            speed = arg1 / 6.0f + 0.3f;
            blend = 16.0f;
        }
    } else if (ctl->env_suit != 0) {
        blend = 5.0f;
        if (gCurrentPad->button & Z_TRIG) {
            if (arg0->animseq.uh != 0x283) {
                arg0->speed *= 0.2f;
                gEventParam = 0xFF010604;
                event_playSound2Range1800();
                speed = 1.0f;
            }
            anim = 0x283;
            arg0->norun = 3;
            arg0->nojump = 3;
        } else {
            if (arg0->animseq.uh == 0x283) {
                gEventParam = 0xFF010604;
                event_playSound2Range1800();
            }
            if (arg0->stickspeed >= 1.0f && arg0->norun == 0) {
                arg0->unkAA = 0x2D;
                speed = arg0->speed / 9.0f;
                anim = 0x4E;
            } else {
                speed = 1.0f;
                anim = actor_updateIdleAnim();
            }
        }
    } else if ((ctl->walk_attrib & 2) && arg0->stickspeed >= 1.0f) {
        ctl->turn = 0x1C;
        arg0->unkAA = 0xF;
        speed = arg4 / 5.0f + 0.3f;
        blend = 8.0f;
        anim = 0x64;
        if (math_random() % 1000U >= 988) {
            anim = (math_random() & 1) + 0x65;
            speed = 0.65f;
            arg0->norun = 0xFF;
            arg0->count5 = 0xFF;
        }
    }
    if (anim != 0 && anim != 999) {
        actor_playAnim(arg0, anim, speed, blend, 0.0f, 0.0f, 0);
    }
    return anim;
}

/* Per held-item kind: the animations of its four action steps. */
static const u16 D_800996A0[][4] = {
    { 0x0138, 0x003E, 0x003D, 0x0139 },
    { 0x0138, 0x003E, 0x00D9, 0x0139 },
    { 0x0035, 0x003A, 0x003F, 0x005D },
    { 0x01CD, 0x01CE, 0x01CF, 0x01D0 },
};
/* Zero-terminated sound lists for sound_addDynamicSound. */
static const u16 D_800996C0[] = { 0x007C, 0x020D, 0x0238, 0 };
static const u16 D_800996C8[] = { 0x01E0, 0x0253, 0x0254, 0 };
static const u16 D_800996D0[] = { 0x01A4, 0x0350, 0 };

/* Per-frame handler for the object's held item / special action `act`: starts, steps and ends
 * it from the held and pressed buttons. Returns the animation started (0 or 999 for none), or
 * 1000 when the action consumed the input. */
s32 func_15065A5C(Actor *arg0, s32 heldButtons, s32 pressedButtons, f32 arg3, s32 arg4, s32 act, s32 arg6, s32 arg7, s32 arg8) {
    s32 anim;
    f32 speed;
    f32 blend;
    f32 animParam;
    s32 clearAction;
    s32 setUnk58;
    u8 handled;
    s32 pendingSound;

    clearAction = 1;
    setUnk58 = 0;
    handled = 0;
    pendingSound = 0;
    speed = 1.0f;
    blend = 4.0f;
    animParam = 0.0f;
    anim = 0;

    if ((arg0->mv->weapon_status == 3) || (arg0->stunned != 0) || (arg4 != 0)) {
        pressedButtons &= ~B_BUTTON;
        gActorNewButtons &= ~B_BUTTON;
        if (arg4 != 0) {
            if (arg0->stunned == 0) {
                arg0->events = NULL;
            }
            pressedButtons &= ~Z_TRIG;
            gActorNewButtons &= ~Z_TRIG;
        }
    }
    if (arg0->yspd > 0.0f) {
        arg3 += 50.0f;
    }

    switch (act) {
    case 0x2:
    case 0xA:
    case 0x13:
    case 0x34:
        {
            s32 idx;

            handled = 1;
            if (act == 2) {
                idx = 0;
            } else if (act == 0x13) {
                idx = 1;
            } else if (act == 0xA) {
                idx = 2;
            } else {
                idx = 3;
            }
            if (arg6 != 0) {
                switch (act) {
                case 0x34:
                    collision_removePartSet(arg0, 0x3D);
                    arg0->mv->weapon_status = 0;
                    break;
                case 0xA:
                    collision_removePartSet(arg0, 0x28);
                    collision_removePartSet(arg0, 0x29);
                    arg0->mv->weapon_status = 0;
                    break;
                }
            }
            if (act == arg0->mv->rt_context_b) {
                arg0->speed = 0.0f;
            }
            if ((arg0->inwater != 0) && ((act == arg0->mv->rt_context_b) || (arg0->mv->weapon_status != 0))) {
                arg0->mv->rt_context_b = 0;
                arg0->norun = 0;
                arg0->count5 = 0;
                if (act == 0xA) {
                    collision_removePartSet(arg0, 0x28);
                    collision_removePartSet(arg0, 0x29);
                    arg0->mv->weapon_status = 0;
                }
            } else if (((arg3 <= 0.0f) && (pressedButtons & B_BUTTON)) || (arg4 != 0)) {
                if ((arg0->mv->rt_context_b != 0) || (arg4 != 0)) {
                    if ((arg0->mv->context_exclude_time == 0) || (arg4 != 0)) {
                        arg0->norun = 0xFF;
                        arg0->nojump = 0xFF;
                        arg0->count5 = 0xFF;
                        anim = D_800996A0[idx][3];
                        arg0->mv->weapon_status = 3;
                        arg0->mv->context_exclude_time = 0x5A;
                        setUnk58 = 1;
                    }
                } else if (arg0->mv->context_exclude_time == 0) {
                    if ((arg0->CameraPtr->DoingAim != 1) && (arg0->CameraPtr->DoingAim != 3)) {
                        arg0->mv->ind_cnt = 0;
                        arg0->mv->weapon_status = 1;
                        anim = D_800996A0[idx][0];
                        arg0->count5 = 0xFF;
                        arg0->norun = 0xFF;
                        arg0->mv->context_exclude_time = 0x5A;
                    }
                }
                pressedButtons &= ~B_BUTTON;
            }
            if (arg0->mv->rt_context_b && (pressedButtons & Z_TRIG) && (anim == 0) && (arg0->animseq.uh == D_800996A0[idx][1])) {
                anim = D_800996A0[idx][2];
                arg0->mv->ind_cnt++;
            }
            if (((arg0->animseq.uh == D_800996A0[idx][1]) || (arg0->animseq.uh == D_800996A0[idx][0]) ||
                 (arg0->animseq.uh == D_800996A0[idx][2]) || (anim != 0)) && (anim != D_800996A0[idx][3])) {
                arg0->mv->rt_context_b = act;
                gCurrentPad->button &= ~Z_TRIG;
                setUnk58 = 1;
            } else {
                arg0->mv->rt_context_b = 0;
            }
            if (D_800C3A58 != 0) {
                if ((arg0->mv->weapon_status == 1) || (arg0->mv->weapon_status == 2U)) {
                    anim = D_800996A0[idx][1];
                    arg0->count5 = 0xFF;
                    arg0->mv->rt_context_b = act;
                    arg0->mv->weapon_status = 2;
                    switch (act) {
                    case 0xA:
                        collision_createPartSet(arg0, 0x28, 1.0f, 0);
                        collision_createPartSet(arg0, 0x29, 1.0f, 0);
                        break;
                    case 0x2:
                    case 0x13:
                        collision_createPartSet(arg0, 8, 1.0f, 0);
                        break;
                    }
                } else {
                    arg0->mv->weapon_status = 0;
                    arg0->mv->rt_context_b = 0;
                    switch (act) {
                    case 0x34:
                        collision_removePartSet(arg0, 0x3D);
                        break;
                    case 0xA:
                        collision_removePartSet(arg0, 0x28);
                        collision_removePartSet(arg0, 0x29);
                        break;
                    case 0x2:
                    case 0x13:
                        collision_removePartSet(arg0, 8);
                        break;
                    }
                }
            }
        }
        break;
    case 0x14:
    case 0x3F:
        {
            s32 animStart;
            s32 animEnd;
            s32 animHit;
            s32 animSlow;
            s32 animFast;
            s32 sfx;

            if (act == 0x14) {
                animStart = 0x102;
                animEnd = 0x103;
                sfx = 0x28;
                animHit = 0x12B;
                animSlow = 0xF7;
                animFast = 0xC3;
            } else {
                animStart = 0x2CA;
                animEnd = 0x2CB;
                sfx = 0x2F;
                animHit = 0x2CE;
                animSlow = 0x2CC;
                animFast = 0x2CD;
            }
            anim = 999;
            if ((arg7 != 0) && (arg0->mv->rt_context_b != 0)) {
                collision_createPartSet(arg0, sfx, 1.0f, 0);
            }
            if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
                if (arg0->mv->rt_context_b != 0) {
                    arg0->norun = 0xFF;
                    arg0->count5 = 0xFF;
                    arg0->nojump = 0xFF;
                    arg0->mv->weapon_status = 3;
                    speed = 2.0f;
                    anim = animEnd;
                    arg0->mv->rt_context_b = 0;
                } else {
                    arg0->norun = 0x10;
                    arg0->count5 = 0x10;
                    arg0->nojump = 0x10;
                    speed = 2.0f;
                    arg0->mv->weapon_status = 1;
                    anim = animStart;
                    arg0->mv->rt_context_b = act;
                }
            }
            if (anim != 999) {
                speed = 2.0f;
            } else if ((arg4 != 0) && (arg0->attach_char != 0)) {
                collision_removePartSet(arg0, sfx);
                arg0->mv->weapon_status = 0;
            } else if (act == arg0->mv->rt_context_b) {
                arg0->unkAA = 0x4B;
                if (arg6 != 0) {
                    clearAction = 0;
                } else if (arg0->mv->SniperState != 0) {
                    arg0->nojump = 0xA;
                    arg0->norun = 0;
                    if (pressedButtons & Z_TRIG) {
                        if (arg0->mv->context_exclude_time == 0) {
                            arg0->count5 = 0xF;
                            arg0->mv->deferredActions |= 3;
                            gActionsPending = 1;
                            anim = animHit;
                            animParam = 6.0f;
                            arg0->mv->context_exclude_time = 0x28;
                        }
                    }
                } else if ((pressedButtons & Z_TRIG) && (arg3 <= 0.0f) && (arg0->norun == 0) && (arg0->mv->context_exclude_time == 0)) {
                    arg0->mv->context_exclude_time = 0x28;
                    if (arg0->speed > 2.0f) {
                        anim = animFast;
                    } else {
                        anim = animSlow;
                    }
                    arg0->norun = 0xF;
                    arg0->count5 = 0xF;
                    arg0->nojump = 0xF;
                }
            }
        }
        break;
    case 0x3:
        handled = 1;
        if ((arg4 != 0) || (arg6 != 0)) {
            if (level != LEVEL_W2_BOILERROOM) {
                arg0->mv->rt_context_b = 0;
                arg0->mv->pssd_is_pissing = 0;
                arg0->count5 = 0;
            }
            if (arg6 != 0) {
                clearAction = 0;
            }
        } else if (arg0->mv->pissed_hangover == 0) {
            if (arg0->mv->pissed_up != 0) {
                arg0->mv->pissed_up -= gFrameSteps;
                if (arg0->mv->pissed_up < 0x64) {
                    arg0->mv->pissed_walk = 0xAD;
                    arg0->mv->pissed_hangover = 0xC8;
                    face_setDefaultExpression(arg0, 0x2A);
                    speed = 1.3f;
                    arg0->norun = 0xFF;
                    arg0->count5 = 0xFF;
                    anim = 0x2D;
                    arg0->mv->rt_context_b = 0;
                }
            }
            if (arg0->mv->pissed_hangover == 0) {
                if ((arg3 <= 0.0f) && (pressedButtons & B_BUTTON)) {
                    arg0->norun = 0xFF;
                    arg0->count5 = 0xFF;
                    if (arg0->mv->rt_context_b != 0) {
                        anim = 0x2D;
                        speed = 1.3f;
                        arg0->mv->rt_context_b = 0;
                    } else {
                        arg0->mv->rt_context_b = 3;
                        anim = 0x2E;
                        arg0->mv->pssd_is_pissing = 0;
                        speed = 1.6f;
                    }
                } else if ((arg0->mv->rt_context_b == 3) && (arg0->norun != 0xFF)) {
                    arg0->mv->rt_context_b = 3;
                    anim = 0x2C;
                    if (gIsMultiplayer != 0) {
                        arg0->nojump = 0xA;
                    }
                    gWaistTiltTarget = gCurrentPad->stick_x * 0x96;
                    if (arg0->mv->bot != 0) {
                        arg0->mv->pssd_waist_tilt = 0;
                    } else if ((gWaistTiltTarget < -0x1F40) && (arg0->mv->pssd_waist_tilt < -0x1B58)) {
                        arg0->angle = arg0->langle = arg0->angle + (gActorStepPercent * 7);
                        anim = 0xE0;
                        speed = 3.0f;
                    } else if ((gWaistTiltTarget > 0x1F40) && (arg0->mv->pssd_waist_tilt > 0x1B58)) {
                        arg0->angle = arg0->langle = arg0->angle - (gActorStepPercent * 7);
                        anim = 0xE0;
                        speed = 3.0f;
                    }
                    gWaistTiltDamping = 10.0f;
                    blend = 8.0f;
                    arg0->norun = 0x10;
                    arg0->count5 = 0x10;
                    if (arg0->mv->pssd_is_pissing == 0) {
                        arg0->mv->pssd_is_pissing = 1;
                        trail_spawnPissTrail(arg0);
                    }
                }
            }
        }
        break;
    case 0x1:
    case 0x35:
        if ((pressedButtons & B_BUTTON) && ((arg3 <= 0.0f) || (act == 0x35)) && (movement_obj[0].mv->transport != 1) &&
            (arg0->lifting < 0x64) && (arg0->unkA9 == 0)) {
            arg0->count3 = 1;
            arg0->unkA9 = 2;
            arg0->norun = 0x78;
            arg0->ygrav = 6.0f;
            arg0->unk1CC = -100000.0f;
            anim = 0x4A;
            arg0->speed *= 0.5f;
            speed = 1.799999952f;
            arg0->yspd = 80.0f;
            arg0->mv->prop = 0;
        }
        break;
    case 0x4:
        if (pressedButtons & B_BUTTON) {
            arg0->mv->rt_context_b = act;
            return 0;
        }
        arg0->mv->rt_context_b = 0;
        return 0;
    case 0x42:
        if (!(gSeenTextBits[8] & 4)) {
            if (pressedButtons & B_BUTTON) {
                arg0->mv->rt_context_b = act;
                return 0;
            }
            arg0->mv->rt_context_b = 0;
        }
        return 0;
    case 0x5:
        if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
            if (arg0->animseq.uh == 0x2F) {
                anim = 999;
            } else {
                arg0->mv->gpoint = 0x12;
                anim = 0x2F;
                if (level == LEVEL_W1_ROCK_SOLID) {
                    arg0->mv->swing_xvel = zones[12].x;
                    arg0->mv->swing_zvel = zones[12].z;
                    effect_createFallingStream(5);
                } else {
                    arg0->mv->swing_xvel = zones[2].x;
                    arg0->mv->swing_zvel = zones[2].z;
                    effect_createFallingStream(0);
                }
                arg0->angle = 0xC000;
                speed = 1.049999952f;
                blend = 6.0f;
                arg0->norun = 0xFF;
                arg0->count5 = 0xFF;
                pendingSound = 0x143;
            }
        }
        break;
    case 0x6:
        if (arg0->mv->rt_context_b == 0) {
            if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f) && (arg0->mv->context_exclude_time == 0)) {
                arg0->norun = 0xFF;
                arg0->count5 = 0xFF;
                anim = 0x44;
                arg0->mv->rt_context_b = 6;
            }
        } else if (arg0->animseq.uh != 0x44) {
            arg0->norun = 0;
            arg0->count5 = 0;
            arg0->mv->rt_context_b = 0;
        } else {
            anim = 999;
        }
        break;
    case 0x36:
        if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
            ModelAttachment *found;

            found = attach_find(arg0, 0x3C, 0);
            if ((found != NULL) && (found->hookState < 0x12C)) {
                anim = 0x240;
                arg0->norun = 0xFF;
                arg0->nojump = 0xFF;
                arg0->count5 = 0xFF;
            }
        }
        break;
    case 0x7:
        if (arg0->mv->rt_context_b == 0) {
            if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f) && (arg0->mv->context_exclude_time == 0)) {
                arg0->norun = 0xFF;
                arg0->count5 = 0xFF;
                anim = 0x45;
                arg0->mv->rt_context_b = 7;
            }
        } else if (arg0->animseq.uh != 0x45) {
            arg0->norun = 0;
            arg0->count5 = 0;
            arg0->mv->rt_context_b = 0;
        } else {
            anim = 999;
        }
        break;
    case 0x8:
        {
            Actor *obj;

            obj = &movement_obj[arg0->attach_char - 1];
            arg0->nojump = 0x14;
            if (pressedButtons & (A_BUTTON | B_BUTTON)) {
                if ((obj->animseq.uh == 0) || (obj->unk22E != 0) || (obj->animseq.uh == 0xC) || (obj->animseq.uh == 0xD)) {
                    u8 mode;

                    mode = 0x10;
                    gCurrentActor = obj;
                    if (pressedButtons & B_BUTTON) {
                        mode = 0xE;
                    } else {
                        obj->animseq.uh = 0;
                    }
                    obj->scriptCmd = script_getScript(1, mode);
                    obj->unk21C = 0;
                    gCurrentActor = arg0;
                    obj->unk22E = 0;
                }
                anim = 999;
            }
        }
        break;
    case 0x27:
        if (act != arg0->mv->rt_context_b) {
            arg0->mv->rt_context_b = 0x27;
            arg0->forced_lod = 6;
            arg0->mv->env_suit = 1;
        }
        break;
    case 0x9:
        if (D_800C3A58 != 0) {
            if ((arg0->mv->weapon_status == 1) || (arg0->mv->weapon_status == 2)) {
                func_15063404(arg0);
            } else {
                func_150634E4(arg0);
            }
        }
        if (act == arg0->mv->rt_context_b) {
            s16 delta;

            delta = (gCurrentPad->stick_x * -100) - arg0->mv->unk80;
            if (delta > 600) {
                delta = 600;
            }
            if (delta < -600) {
                delta = -600;
            }
            arg0->mv->unk80 += delta;
            delta = ((gCurrentPad->stick_y << 5) - arg0->mv->unk82) + 576;
            if (delta > 300) {
                delta = 300;
            }
            if (delta < -300) {
                delta = -300;
            }
            arg0->mv->unk82 += delta;
            if (((arg4 != 0) && ((arg0->attach_char != 0) || (arg0->stunned != 0) || (arg0->shad != 0.0f))) || (arg0->inwater != 0)) {
                func_150634E4(arg0);
            } else if (arg4 != 0) {
                speed = 1.399999976f;
                pressedButtons |= B_BUTTON;
                anim = actor_tryStartBAction(arg0, 0x7D, 1, 0, pressedButtons);
                if (anim == 999) {
                    func_150634E4(arg0);
                }
            } else if (arg6 != 0) {
                clearAction = 0;
            } else {
                arg0->nojump = 0xA;
                if (arg0->mv->matrix_physics != 0) {
                    u16 prev;
                    u8 lowBits;

                    if ((heldButtons & Z_TRIG) && (arg0->speedup < 0.6000000238f)) {

                        prev = arg0->mv->unk7E;
                        arg0->mv->unk7E = prev + ((gActorStepPercent * 4) / 3);
                        if ((prev ^ arg0->mv->unk7E) & 0x100) {
                            gEventParam = 0x13;
                            if (arg0->mv->unk7E & 0x200) {
                                gEventParam = 0x14;
                            }
                            event_runAnimCommand();
                            gEventParam = -0xFCF920;
                            event_playSound2Range1800();
                            sound_playActorUntracked((math_random() & 1) + 0x2F5, arg0, 0x7D00, 0x1F4, 0x3E8);
                            if (!(math_random() & 7)) {
                                sound_playActorUntrackedDelayed(0x5CD, arg0, 0x5DC0, 0x1F4, 0x3E8, 0x3C);
                            }
                        }
                    } else {
                        lowBits = (arg0->mv->unk7E << 1) | 1;
                        arg0->mv->unk7E = (arg0->mv->unk7E & 0xFF00) | lowBits;
                    }
                } else {
                    actor_turnWaist(arg0, heldButtons, Z_TRIG | R_TRIG);
                    if ((arg0->animseq.uh != 0x11A) && (arg0->animseq.uh != 0x7B)) {
                        u8 endEffect;
                        endEffect = 0;
                        if (!(heldButtons & Z_TRIG) && (arg0->mv->prevButtons & Z_TRIG)) {
                            endEffect = 1;
                        }
                        if ((arg0->mv->gun_before_reload == 0) || (pressedButtons & A_BUTTON)) {
                            anim = 0x11A;
                            if (!(pressedButtons & A_BUTTON)) {
                                endEffect = 1;
                            }
                            arg0->mv->gun_before_reload = 0x3C;
                            arg0->norun = 0xFF;
                            arg0->count5 = 0xFF;
                        } else if ((heldButtons & Z_TRIG) && (arg0->norun == 0)) {
                            if (arg0->speed != 0.0f) {
                                if (arg0->stickspeed >= 20.0f) {
                                    anim = 0x238;
                                    speed = (arg0->speed / 22.0f) + 0.400000006f;
                                } else {
                                    anim = 0x20D;
                                }
                                if (!(heldButtons & R_TRIG)) {
                                    arg0->mv->pssd_waist_tilt = 0;
                                    arg0->mv->turn = 3;
                                } else {
                                    arg0->mv->turn = 0;
                                }
                                if (gIsMultiplayer == 0) {
                                    arg0->unkAA = 0x37;
                                }
                            } else {
                                anim = 0x7C;
                            }
                            arg0->count5 = 8;
                            if ((arg0->animseq.uh != 0x7C) && (arg0->animseq.uh != 0x20D) && (arg0->animseq.uh != 0x238)) {
                                particle_detachObjEmitters(arg0);
                                sound_addDynamicSound(0x2CF, arg0->xpos, arg0->ypos, arg0->zpos, 0x7FFF, 0x3E8, 0x1F4, sound_followOwnerAnimsCallback, arg0, (s32)D_800996C0, 0, 0); // matching: the (s32) casts on these tables steer IDO; a plain pointer reschedules
                                if (((arg0->character_type == 0x3BU) || (arg0->character_type == 0x80)) && !(math_random() & 3)) {
                                    s32 sound;

                                    if (arg0->character_type == 0x3B) {
                                        sound = sound_pickVariant(0x543, 4);
                                    } else {
                                        sound = sound_pickVariant(0x57C, 6);
                                    }
                                    sound_addDynamicSound(sound, arg0->xpos, arg0->ypos, arg0->zpos, 0x7FFF, 0x3E8, 0x1F4, sound_followOwnerAnimsCallback, arg0, (s32)D_800996C0, 0, 0);
                                }
                            }
                        } else if ((arg0->taskint[2] != 0) && (level == LEVEL_W5_BANK)) {
                            if ((pressedButtons & B_BUTTON) && (arg0->taskint[2] == 1) && (arg0->shad == 0.0f)) {
                                blend = 16.0f;
                                speed = 0.6999999881f;
                                anim = 0x20F;
                                arg0->angle = 0;
                                if (arg0->xpos > 0.0f) {
                                    arg0->angle = 0x8000;
                                    anim = 0x20E;
                                }
                                arg0->speed = 0.0f;
                                arg0->norun = 0xFF;
                                arg0->count5 = 0xFF;
                                arg0->count3 = 1;
                            }
                        } else {
                            speed = 1.399999976f;
                            anim = actor_tryStartBAction(arg0, 0x7D, 1, 0, pressedButtons);
                        }
                        if (endEffect != 0) {
                            particle_attachTwinEmitters(arg0);
                        }
                    }
                }
            }
        } else {
            speed = 1.399999976f;
            anim = actor_tryStartBAction(arg0, 0x7B, 1, act, pressedButtons);
            if (anim != 999) {
                arg0->mv->gun_before_reload = 0x3C;
            }
        }
        break;
    case 0x38:
        if (act == arg0->mv->rt_context_b) {
            if ((arg4 != 0) && (arg0->attach_char != 0)) {
                collision_removePartSet(arg0, 0x7B);
                arg0->mv->weapon_status = 0;
            } else if (arg6 != 0) {
                clearAction = 0;
            } else {
                u8 endEffect;
                u16 pad[2];  // matching: unused stack slots

                arg0->nojump = 0xA;
                actor_turnWaist(arg0, heldButtons, Z_TRIG | R_TRIG);
                if (arg0->animseq.uh != 0x1DD) {
                    endEffect = 0;
                    if (!(heldButtons & Z_TRIG) && (arg0->mv->prevButtons & Z_TRIG)) {
                        endEffect = 1;
                    }
                    if (heldButtons & Z_TRIG) {
                        if (arg0->speed != 0.0f) {
                            if (arg0->stickspeed >= 20.0f) {
                                anim = 0x254;
                                speed = (arg0->speed / 22.0f) + 0.400000006f;
                            } else {
                                anim = 0x253;
                            }
                            if (!(heldButtons & R_TRIG)) {
                                arg0->mv->turn = 3;
                                arg0->mv->pssd_waist_tilt = 0;
                            }
                        } else {
                            anim = 0x1E0;
                        }
                        blend = 0.0f;
                        arg0->count5 = 8;
                        if ((arg0->animseq.uh != 0x1E0) && (arg0->animseq.uh != 0x253) && (arg0->animseq.uh != 0x254)) {
                            particle_detachObjEmitters(arg0);
                            sound_addDynamicSound(0x2CF, arg0->xpos, arg0->ypos, arg0->zpos, 0x7FFF, 0x3E8, 0x1F4, sound_followOwnerAnimsCallback, arg0, (s32)D_800996C8, 0, 0);
                            if (((arg0->character_type == 0x3BU) || (arg0->character_type == 0x80)) && !(math_random() & 3)) {
                                s32 sound;

                                if (arg0->character_type == 0x3B) {
                                    sound = sound_pickVariant(0x543, 4);
                                } else {
                                    sound = sound_pickVariant(0x57C, 6);
                                }
                                sound_addDynamicSound(sound, arg0->xpos, arg0->ypos, arg0->zpos, 0x7FFF, 0x3E8, 0x1F4, sound_followOwnerAnimsCallback, arg0, (s32)D_800996C8, 0, 0);
                            }
                        }
                        arg0->mv->gunRecoilVel += gFrameTicks * 0xA;
                        if (arg0->mv->gunRecoil > 0xAF) {
                            arg0->mv->gunRecoil = 0xAF;
                        }
                        arg0->mv->gunRecoil += arg0->mv->gunRecoilVel * gFrameTicks;
                        if (arg0->mv->gunRecoil > 0x1E00) {
                            arg0->mv->gunRecoil = 0x1E00;
                        }
                    } else {
                        speed = 1.399999976f;
                        anim = actor_tryStartBAction(arg0, 0x1DE, 1, 0, pressedButtons);
                        arg0->mv->gunRecoil = 0;
                        arg0->mv->gunRecoilVel = 0;
                    }
                    if (endEffect != 0) {
                        particle_attachPointEmitter(arg0, 5, 0x78, 0xFF, 1);
                    }
                }
            }
        } else {
            speed = 1.399999976f;
            anim = actor_tryStartBAction(arg0, 0x1DD, 1, act, pressedButtons);
        }
        break;
    case 0x39:
        if (act == arg0->mv->rt_context_b) {
            if ((arg4 != 0) && (arg0->attach_char != 0)) {
                collision_removePartSet(arg0, 0x82);
                arg0->mv->weapon_status = 0;
            } else if (arg6 != 0) {
                clearAction = 0;
            } else {
                arg0->nojump = 0xA;
                actor_turnWaist(arg0, heldButtons, Z_TRIG | R_TRIG);
                if (arg0->animseq.uh != 0x1EE) {
                    if (arg0->animseq.uh == 0x21E) {
                        arg0->norun = 0xFF;
                    }
                    if ((arg0->mv->gun_before_reload == 0) || ((pressedButtons & A_BUTTON) && (arg0->animseq.uh != 0x21E))) {
                        if (arg0->shad == 0.0f) {
                            arg0->mv->gun_before_reload = 0x1E;
                            arg0->norun = 0xFF;
                            arg0->count5 = 0xFF;
                            anim = 0x21E;
                        }
                    } else if ((pressedButtons & Z_TRIG) && (arg0->animseq.uh != 0x1EE) && (arg0->animseq.uh != 0x21E)) {
                        if (arg0->speed == 0.0f) {
                            anim = 0x1EB;
                        } else {
                            anim = 999;
                            gEventParam = 0x53;
                            event_runAnimCommand();
                        }
                        sound_stopByCallback(effect_nullSoundCallback, arg0, arg0->unique_id);
                        sound_addDynamicSound((math_random() & 1) + 0x2F3, arg0->xpos, arg0->ypos, arg0->zpos, 0x7D00, 0x5DC, 0x1F4, effect_nullSoundCallback, arg0, arg0->unique_id, 0, 0);
                        if (arg0->CameraPtr != NULL) {
                            sound_playActorSound1Delayed((math_random() % 3U) + 0x2EB, arg0, 0x7D00, 0x64, 0x78, 0x1E);
                        }
                    } else {
                        speed = 1.399999976f;
                        anim = actor_tryStartBAction(arg0, 0x1EF, 1, 0, pressedButtons);
                    }
                }
            }
        } else {
            speed = 1.399999976f;
            anim = actor_tryStartBAction(arg0, 0x1EE, 1, act, pressedButtons);
            if (anim != 999) {
                arg0->mv->gun_before_reload = 0x1E;
            }
        }
        break;
    case 0x37:
        if (act == arg0->mv->rt_context_b) {
            if ((arg4 != 0) && (arg0->attach_char != 0)) {
                collision_removePartSet(arg0, 0x85);
                arg0->mv->weapon_status = 0;
            } else if (arg6 != 0) {
                clearAction = 0;
            } else {
                arg0->nojump = 0xA;
                actor_turnWaist(arg0, heldButtons, Z_TRIG | R_TRIG);
                if ((pressedButtons & Z_TRIG) && (arg0->animseq.uh != 0x1F7) && (arg0->mv->ReloadTimer == 0)) {
                    if (math_random() & 3) {
                        anim = 0x1F4;
                    } else {
                        anim = 0x235;
                    }
                    blend = 0.0f;
                    arg0->mv->ReloadTimer = 0x78;
                    arg0->norun = 0xFF;
                    arg0->count5 = 0xFF;
                } else if (pressedButtons & B_BUTTON) {
                    speed = 1.399999976f;
                    anim = actor_tryStartBAction(arg0, 0x1F8, 1, 0, pressedButtons);
                }
            }
        } else {
            speed = 1.399999976f;
            anim = actor_tryStartBAction(arg0, 0x1F7, 1, act, pressedButtons);
        }
        break;
    case 0xB:
        if (arg0->animseq.uh == 0x68) {
            if ((arg0->unk2D0->frame[0] < 68.0f) && (arg0->unk222 != 0)) {
                gWaistTiltTarget = (arg0->langle - actor_getAngleToLinked(arg0)) - 0x2000;
            }
            anim = 999;
            gWaistTiltDamping = 4.0f;
        } else if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
            speed = 1.200000048f;
            anim = 0x68;
            blend = 6.0f;
            arg0->norun = 0xFF;
            arg0->count5 = 0xFF;
        }
        break;
    case 0xC:
    case 0x16:
        anim = 999;
        if ((arg4 != 0) && (act == 0x16)) {
            if (act == arg0->mv->rt_context_b) {
                s32 cancel;

                cancel = 1;
                if ((s32)arg0->shad == 0) {
                    pressedButtons |= B_BUTTON;
                    anim = actor_tryStartBAction(arg0, 0x78, 1, 0, pressedButtons);
                    if (anim != 999) {
                        cancel = 0;
                    }
                }
                if (cancel != 0) {
                    collision_removePartSet(arg0, 0x24);
                    arg0->mv->weapon_status = 0;
                    arg0->count5 = 0;
                    arg0->norun = 0;
                }
            }
        } else if (act == arg0->mv->rt_context_b) {
            arg0->nojump = 0xA;
            if ((arg4 != 0) && ((arg0->attach_char != 0) || ((s32)arg0->shad != 0))) {
                collision_removePartSet(arg0, 0x24);
                arg0->mv->weapon_status = 0;
            } else if (arg6 != 0) {
                if ((act == 0x16) || ((act == 0xC) && (arg0->shad == 0.0f) && (arg0->energy != 0))) {
                    clearAction = 0;
                } else {
                    collision_removePartSet(arg0, 0x24);
                    arg0->mv->weapon_status = 0;
                }
            } else if ((pressedButtons & B_BUTTON) || (arg4 != 0)) {
                pressedButtons |= B_BUTTON;
                anim = actor_tryStartBAction(arg0, 0x78, 1, 0, pressedButtons);
                if ((anim == 999) && (arg4 != 0)) {
                    collision_removePartSet(arg0, 0x24);
                    arg0->mv->weapon_status = 0;
                    arg0->count5 = 0;
                }
            } else if ((pressedButtons & Z_TRIG) && (arg0->animseq.uh != 0x76) && (arg0->mv->ReloadTimer == 0)) {
                if (arg0->mv->unk1B2 != 0) {
                    anim = 0x2C7;
                } else if ((act == 0xC) || (level == LEVEL_WMP_BEACH) || (gIsMultiplayer == 0)) {
                    anim = 0x207;
                } else {
                    anim = 0x77;
                }
                blend = 0.0f;
                arg0->norun = 0xFF;
                arg0->count5 = 0xFF;
                if (arg0->mv->SniperState != 0) {
                    if (arg0->CameraPtr != NULL) {
                        camera_startShake(arg0->CameraPtr, 0);
                    }
                }
                arg0->mv->ReloadTimer = 0x78;
            } else if ((arg0->mv->SniperState != 0) && (arg0->CameraPtr != NULL) && (arg0->character_type == 0x80) && (sound_isActorSound1Playing(arg0) == 0) && !(math_random() & 0x7F)) {
                actor_playSound(sound_pickVariant(0x568, 4), arg0);
            }
        } else {
            anim = actor_tryStartBAction(arg0, 0x76, 1, act, pressedButtons);
        }
        if ((act == 0xC) && ((act == arg0->mv->rt_context_b) || (arg0->mv->weapon_status != 0))) {
            arg0->speed = 0.0f;
            if ((s32)arg0->shad != 0) {
                collision_removePartSet(arg0, 0x24);
                arg0->mv->weapon_status = 0;
                arg0->mv->rt_context_b = 0;
            }
        }
        break;
    case 0x19:
    case 0x40:
        {
            s32 animStart;
            s32 animLoop;

            if (act == 0x19) {
                animStart = 0x10E;
                animLoop = 0x110;
            } else {
                animStart = 0x2C2;
                animLoop = 0x2C3;
            }
            if (arg6 != 0) {
                clearAction = 0;
            } else if (act == arg0->mv->rt_context_b) {
                if (arg0->mv->weapon_status == 2) {
                    if (pressedButtons & Z_TRIG) {
                        anim = actor_pickRandomAnim(arg0, 0x113, 0x113, 0x119);
                        arg0->norun = 0xFF;
                    } else {
                        anim = actor_tryStartBAction(arg0, animLoop, 1, 0, pressedButtons);
                    }
                }
            } else {
                anim = actor_tryStartBAction(arg0, animStart, 1, act, pressedButtons);
            }
        }
        break;
    case 0xE:
        arg0->nojump = 0x14;
        anim = 999;
        break;
    case 0xF:
        anim = 999;
        if ((act == arg0->mv->rt_context_b) && ((arg6 != 0) || (arg0->attach_char != 0) || (arg4 != 0))) {
            collision_removePartSet(arg0, 0x3A);
            arg0->mv->weapon_status = 0;
            arg0->norun = 0;
            arg0->count5 = 0;
        } else if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
            anim = 0xB4;
            if (act == arg0->mv->rt_context_b) {
                anim = 0xB6;
            }
            arg0->norun = 0xFF;
            arg0->count5 = 0xFF;
            arg0->mv->rt_context_b = act;
        }
        if (act == arg0->mv->rt_context_b) {
            arg0->nojump = 0xA;
            arg0->speed = 0.0f;
        }
        break;
    case 0x10:
        speed = 1.399999976f;
        if (act == arg0->mv->rt_context_b) {
            gWaistTiltTarget = gCurrentPad->stick_x * 0xC8;
            arg0->nojump = 0xA;
            if ((heldButtons & Z_TRIG) && (anim = 0xC1, (arg0->animseq.uh != 0xBE))) {
                speed = 1.0f;
            } else {
                anim = actor_tryStartBAction(arg0, 0xC0, 1, 0, pressedButtons);
            }
        } else {
            anim = actor_tryStartBAction(arg0, 0xBE, 1, act, pressedButtons);
        }
        break;
    case 0x15:
        if (act == arg0->mv->rt_context_b) {
            if ((arg4 != 0) && (arg0->attach_char != 0)) {
                collision_removePartSet(arg0, 0x4B);
                arg0->mv->weapon_status = 0;
            } else if (arg6 != 0) {
                clearAction = 0;
            } else {
                arg0->nojump = 0xA;
                actor_turnWaist(arg0, heldButtons, Z_TRIG | R_TRIG);
                if ((heldButtons & Z_TRIG) && (arg0->animseq.uh != 0x168)) {
                    if (arg0->speed != 0.0f) {
                        arg0->unkAA = 0x50;
                        arg0->mv->turn = 3;
                        arg0->mv->pssd_waist_tilt = 0;
                        anim = 0x22B;
                        speed = (arg0->speed * 0.0625f) + 0.6000000238f;
                        if (arg0->stickspeed >= 20.0f) {
                            speed = (arg0->speed / 22.0f) + 0.400000006f;
                        }
                    } else {
                        anim = 0xE7;
                    }
                    arg0->norun = 0;
                    arg0->count5 = 8;
                    if ((arg0->animseq.uh != 0xE7) && (arg0->animseq.uh != 0x22B)) {
                        s32 anims[2];

                        anims[0] = 0xE7;
                        anims[1] = 0x22B;
                        arg0->mv->shotsfired++;
                        actor_runHitHandler(arg0, 9, anims);
                    }
                } else {
                    anim = actor_tryStartBAction(arg0, 0xE4, 1, 0, pressedButtons);
                }
            }
        } else {
            anim = actor_tryStartBAction(arg0, 0x168, 1, act, pressedButtons);
        }
        break;
    case 0x18:
    case 0x41:
        {
            s32 animStart;
            s32 animAlt;

            if (act == 0x18) {
                animStart = 0xFB;
                animAlt = 0xFC;
            } else {
                animStart = 0x2C8;
                animAlt = 0x2C9;
            }
            if (act == arg0->mv->rt_context_b) {
                if (arg6 != 0) {
                    clearAction = 0;
                } else if ((arg0->mv->SniperState != 0) && (arg0->mv->weapon_status == 2)) {
                    arg0->nojump = 0xA;
                    arg0->norun = 0;
                    arg0->count5 = 0xA;
                    if ((gCurrentPad->button & Z_TRIG) || (arg0->mv->prevButtons & Z_TRIG)) {
                        arg0->mv->deferredActions |= 1;
                        gActionsPending = 1;
                    }
                    if (animStart != arg0->animseq.uh) {
                        if (!(gCurrentPad->button & Z_TRIG) && (arg0->mv->prevButtons & Z_TRIG) && (arg0->mv->ReloadTimer == 0)) {
                            arg0->mv->deferredActions |= 2;
                            gActionsPending = 1;
                            blend = 0.0f;
                            arg0->norun = 0xFF;
                            arg0->count5 = 0xFF;
                            anim = 0x10C;
                            arg0->mv->ReloadTimer = 0x3C;
                        } else if (arg0->animseq.uh != 0x10C) {
                            anim = 0x10A;
                        }
                    }
                } else {
                    arg0->nojump = 0x14;
                    if ((pressedButtons & Z_TRIG) && (animStart != arg0->animseq.uh) && (arg0->mv->ReloadTimer == 0)) {
                        anim = animAlt;
                        blend = 0.0f;
                        arg0->norun = 0xFF;
                        arg0->count5 = 0xFF;
                        arg0->mv->ReloadTimer = 0x3C;
                    } else {
                        anim = actor_tryStartBAction(arg0, 0xC6, 1, 0, pressedButtons);
                    }
                }
            } else {
                anim = actor_tryStartBAction(arg0, animStart, 1, act, pressedButtons);
            }
        }
        break;
    case 0x11:
        if (pressedButtons & B_BUTTON) {
            u8 pad;  // matching: unused stack slot

            collision_findObjectById(7)->taskint[0] = 1;
        }
        break;
    case 0x25:
    case 0x2D:
    case 0x2E:
        arg0->mv->rt_context_b = act;
        arg0->nojump = 0x14;
        break;
    case 0x28:
        if (arg4 != 0) {
            arg0->mv->rt_context_b = 0;
        } else if ((arg3 <= 0.0f) && (pressedButtons & B_BUTTON)) {
            if (act != arg0->mv->rt_context_b) {
                arg0->mv->rt_context_b = act;
            }
        }
        break;
    case 0x3C:
        {
            u16 before;
            u16 after;

            before = gCameraYawAngle - arg0->langle;
            anim = 999;
            arg0->intelhitroute = 0x2F;
            if (arg0->lifting != 0) {
                s16 old;
                f32 mag;
                Actor *obj;

                old = arg0->mv->unk18;
                arg0->count5 = 0xFF;
                arg0->norun = 0xFF;
                if (arg0->mv->unk18 >= 0) {
                    arg0->mv->unk18 -= 0x14;
                } else {
                    arg0->mv->unk18 += 0x14;
                }
                if ((old ^ arg0->mv->unk18) & 0x8000) {
                    arg0->mv->unk18 = 0;
                }
                if (arg0->stickspeed > 10.0f) {
                    s16 diff;

                    diff = arg0->mv->press_angle - arg0->mv->unk7E;
                    if ((diff < 0x2000) && (diff > -0x2000) && ((diff > 0x100) || (diff < -0x100))) {
                        if (diff & 0x8000) {
                            arg0->mv->unk18 -= 0x28;
                        } else {
                            arg0->mv->unk18 += 0x28;
                        }
                        if (arg0->mv->unk18 > 0x1000) {
                            arg0->mv->unk18 = 0x1000;
                        }
                        if (arg0->mv->unk18 < -0x1000) {
                            arg0->mv->unk18 = -0x1000;
                        }
                    }
                }
                arg0->angle = arg0->langle = arg0->langle + ((arg0->mv->unk18 * gActorStepPercent) / 100);
                if ((arg0->mv->unk18 > 0x200) || (arg0->mv->unk18 < -0x200)) {
                    after = gCameraYawAngle - arg0->langle;
                    if ((((before < 0x1000) || (before > 0xF000)) && (after >= 0x1000) && (after < 0x7000) && (arg0->mv->unk18 < 0)) ||
                        ((before > 0x7000) && (after <= 0x7000) && (arg0->mv->unk18 > 0))) {
                        gEventParam = -0xFEFA4A;
                        event_playSound2Range1800();
                    }
                }
                arg0->mv->unk7E = arg0->mv->press_angle;
                anim = 0x288;
                mag = fabsf(arg0->mv->unk18);
                if (mag > 200.0f) {
                    anim = 0x286;
                }
                if (mag > 2000.0f) {
                    anim = 0x27E;
                }
                blend = 16.0f;
                if (gLevelLoadState != 0) {
                    blend = 0.0f;
                }
                if (pressedButtons & B_BUTTON) {
                    obj = collision_findObjectById(6);
                    obj->speed = 80.0f;
                    if (mag > 2000.0f) {
                        obj->speed = (mag + 1000.0f) / 36.0f;
                    } else {
                        obj->speed = (mag + 2000.0f) / 80.0f;
                    }
                    obj->stunned = 0;
                    obj->carried = 0;
                    obj->scriptCmd = NULL;
                    obj->intelwalk = 2;
                    arg0->lifting = 0;
                    arg0->count5 = 0;
                    arg0->norun = 0xA;
                }
            } else if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f) && (arg0->nojump == 0)) {
                if (arg0->animseq.uh != 0x27D) {
                    if ((arg0->norun == 0) || (arg0->animseq.uh < 0x27A) || (arg0->animseq.uh >= 0x27E)) {
                        anim = 0x27A;
                    } else if ((arg0->unk103 != 0) || (arg0->ivehitit != 0)) {
                        anim = arg0->animseq.uh + 1;
                    }
                    if (anim != 999) {
                        arg0->ivehitit = 0;
                        arg0->count5 = 0xFF;
                        arg0->norun = 0xFF;
                    }
                }
            }
        }
        break;
    case 0x21:
        if ((arg4 != 0) && (arg0->attach_char != 0)) {
            collision_removePartSet(arg0, 0x58);
            arg0->mv->weapon_status = 0;
        } else if (arg6 != 0) {
            clearAction = 0;
        } else if (arg3 <= 0.0f) {
            if (act == arg0->mv->rt_context_b) {
                if (arg0->mv->weapon_status == 2) {
                    if (pressedButtons & Z_TRIG) {
                        anim = actor_pickRandomAnim(arg0, 0x129, 0x12A, 0x128);
                        speed = 1.299999952f;
                        arg0->norun = 0xFF;
                    } else {
                        anim = actor_tryStartBAction(arg0, 0x121, 1, 0, pressedButtons);
                        speed = 1.399999976f;
                    }
                }
            } else {
                anim = actor_tryStartBAction(arg0, 0x120, 1, act, pressedButtons);
                speed = 1.399999976f;
            }
        }
        break;
    case 0x22:
        speed = 1.399999976f;
        if ((arg4 != 0) && (arg0->attach_char != 0)) {
            collision_removePartSet(arg0, 0x56);
            arg0->mv->weapon_status = 0;
        } else if (arg6 != 0) {
            clearAction = 0;
        } else if ((act == arg0->mv->rt_context_b) && (arg3 <= 0.0f)) {
            if (arg0->mv->weapon_status == 2) {
                if (pressedButtons & Z_TRIG) {
                    anim = actor_pickRandomAnim(arg0, 0x15F, 0x15F, 0x15F);
                    speed = 1.299999952f;
                    arg0->norun = 0xFF;
                } else {
                    anim = actor_tryStartBAction(arg0, 0x15E, 1, 0, pressedButtons);
                }
            }
        } else {
            anim = actor_tryStartBAction(arg0, 0x15D, 1, act, pressedButtons);
        }
        break;
    case 0x24:
        if (arg4 != 0) {
            if (act == arg0->mv->rt_context_b) {
                s32 cancel;

                cancel = 1;
                if (((s32)arg0->shad == 0) && (arg0->mv->weapon_status == 2)) {
                    pressedButtons |= B_BUTTON;
                    anim = actor_tryStartBAction(arg0, 0x155, 1, 0, pressedButtons);
                    if (anim != 999) {
                        cancel = 0;
                    }
                }
                if (cancel != 0) {
                    collision_removePartSet(arg0, 0x63);
                    arg0->mv->weapon_status = 0;
                    arg0->count5 = 0;
                    arg0->norun = 0;
                }
            }
        } else if (act == arg0->mv->rt_context_b) {
            if ((arg4 != 0) && (arg0->attach_char != 0)) {
                collision_removePartSet(arg0, 0x63);
                arg0->mv->weapon_status = 0;
            } else if (arg6 != 0) {
                clearAction = 0;
            } else if ((arg0->mv->SniperState != 0) && (arg0->mv->weapon_status == 2) && (arg4 == 0)) {
                arg0->nojump = 0xA;
                arg0->norun = 0;
                arg0->count5 = 0xA;
                if ((arg0->mv->ReloadTimer == 0) && ((gCurrentPad->button & Z_TRIG) || (arg0->mv->prevButtons & Z_TRIG))) {
                    arg0->mv->deferredActions |= 1;
                    gActionsPending = 1;
                }
                if (arg0->animseq.uh != 0x152) {
                    if (!(gCurrentPad->button & Z_TRIG) && (arg0->mv->prevButtons & Z_TRIG) && (arg0->animseq.uh == 0x157) && (arg0->mv->ReloadTimer == 0)) {
                        arg0->mv->deferredActions |= 2;
                        gActionsPending = 1;
                        blend = 0.0f;
                        arg0->norun = 0xFF;
                        arg0->count5 = 0xFF;
                        arg0->mv->ReloadTimer = 0x78;
                        anim = 0x156;
                        if ((arg0->CameraPtr != NULL) && (((arg0->floorMaterial & 0x1F) == 5) || ((arg0->floorMaterial & 0x1F) == 3) ||
                                                       ((arg0->floorMaterial & 0x1F) == 6) || ((arg0->floorMaterial & 0x1F) == 9))) {
                            sound_playActorSound1Delayed((math_random() % 3U) + 0x2EB, arg0, 0x7D00, 0x64, 0x78, 0x6E);
                        }
                    } else if (arg0->animseq.uh != 0x156) {
                        anim = 0x157;
                    }
                }
            } else {
                arg0->nojump = 0x14;
                if (arg4 != 0) {
                    arg0->norun = 0xFF;
                    arg0->count5 = 0xFF;
                    anim = 0x155;
                    if ((s32)arg0->shad != 0) {
                        collision_removePartSet(arg0, 0x63);
                        arg0->mv->weapon_status = 0;
                    }
                } else if ((pressedButtons & Z_TRIG) && (arg0->animseq.uh != 0x152) && (arg0->mv->ReloadTimer == 0)) {
                    anim = 0x154;
                    blend = 0.0f;
                    arg0->norun = 0xFF;
                    arg0->count5 = 0xFF;
                    arg0->mv->ReloadTimer = 0x78;
                } else {
                    anim = actor_tryStartBAction(arg0, 0x155, 1, 0, pressedButtons);
                }
            }
        } else {
            anim = actor_tryStartBAction(arg0, 0x152, 1, act, pressedButtons);
        }
        break;
    case 0x3B:
        {
            s32 held;

            if (D_800C3A58 != 0) {
                if (act == arg0->mv->rt_context_b) {
                    collision_createPartSet(arg0, 0x89, 1.0f, 0);
                    arg0->mv->weapon_status = 2;
                } else {
                    collision_removePartSet(arg0, 0x89);
                    arg0->mv->weapon_status = 0;
                }
            }
            if (((act == arg0->mv->rt_context_b) || (arg0->mv->weapon_status != 0)) && (arg0->inwater != 0)) {
                arg0->mv->rt_context_b = 0;
                arg0->norun = 0;
                arg0->count5 = 0;
                collision_removePartSet(arg0, 0x89);
                arg0->mv->weapon_status = 0;
            } else if (act == arg0->mv->rt_context_b) {
                if ((arg4 != 0) && ((arg0->attach_char != 0) || (arg0->stunned != 0) || (arg0->lifting != 0))) {
                    collision_removePartSet(arg0, 0x89);
                    arg0->mv->weapon_status = 0;
                    arg0->mv->rt_context_b = 0;
                } else if (arg4 != 0) {
                    pressedButtons |= B_BUTTON;
                    anim = actor_tryStartBAction(arg0, 0x226, 1, 0, pressedButtons);
                    if (anim == 999) {
                        collision_removePartSet(arg0, 0x89);
                        arg0->mv->weapon_status = 0;
                        arg0->mv->rt_context_b = 0;
                        arg0->count5 = 0;
                    }
                } else if (arg6 != 0) {
                    clearAction = 0;
                } else {
                    arg0->nojump = 0x14;
                    actor_turnWaist(arg0, heldButtons, Z_TRIG | R_TRIG);
                    if ((arg0->mv->ReloadTimer == 0) && ((gActorHeldButtons & Z_TRIG) || (arg0->mv->prevButtons & Z_TRIG))) {
                        if (arg0->mv->AimState == 2) {
                            arg0->mv->deferredActions |= 4;
                            gActionsPending = 1;
                        } else {
                            arg0->mv->deferredActions |= 1;
                            gActionsPending = 1;
                        }
                    }
                    if (arg4 != 0) {
                        arg0->norun = 0xFF;
                        arg0->count5 = 0xFF;
                        anim = 0x226;
                        if ((s32)arg0->shad != 0) {
                            collision_removePartSet(arg0, 0x89);
                            arg0->mv->weapon_status = 0;
                        }
                    } else {
                        held = gActorHeldButtons & R_TRIG;
                        if ((((held != 0) && !(gActorHeldButtons & Z_TRIG) && (arg0->mv->prevButtons & Z_TRIG)) ||
                                    ((held == 0) && (gActorNewButtons & Z_TRIG))) &&
                                   (arg0->animseq.uh != 0x225) && (arg0->animseq.uh != 0x222) && (arg0->animseq.uh != 0x236) &&
                                   (arg0->animseq.uh != 0x227) && (arg0->animseq.uh != 0x237) && (arg0->mv->ReloadTimer == 0)) {
                            if (gCurrentPad->button & R_TRIG) {
                                if (math_random() & 3) {
                                    anim = 0x227;
                                } else {
                                    anim = 0x237;
                                }
                                arg0->mv->deferredActions |= 2;
                                gActionsPending = 1;
                            } else {
                                if (math_random() & 3) {
                                    anim = 0x222;
                                } else {
                                    anim = 0x236;
                                }
                            }
                            blend = 0.0f;
                            arg0->norun = 0xFF;
                            arg0->count5 = 0xFF;
                            arg0->mv->ReloadTimer = 0x3C;
                        } else if ((held != 0) && (gActorHeldButtons & Z_TRIG) && (arg0->animseq.uh != 0x225)) {
                            arg0->norun = 0;
                            arg0->nojump = 8;
                            arg0->count5 = 8;
                            if ((arg0->animseq.uh != 0x222) && (arg0->animseq.uh != 0x236) && (arg0->animseq.uh != 0x227) && (arg0->animseq.uh != 0x237)) {
                                if ((arg0->stickspeed >= 1.0f) || (arg0->mv->door_walkf_time > 0)) {
                                    anim = 0x223;
                                    if (arg0->stickspeed >= 20.0f) {
                                        anim = 0x31B;
                                        speed = (arg0->speed / 22.0f) + 0.400000006f;
                                    }
                                } else {
                                    anim = 0x221;
                                }
                            }
                        } else {
                            anim = actor_tryStartBAction(arg0, 0x226, 1, 0, pressedButtons);
                        }
                    }
                }
            } else {
                anim = actor_tryStartBAction(arg0, 0x225, 1, act, pressedButtons);
            }
        }
        break;
    case 0x12:
        if (((act == arg0->mv->rt_context_b) || (arg0->mv->weapon_status != 0)) && (arg0->inwater != 0)) {
            arg0->mv->rt_context_b = 0;
            arg0->norun = 0;
            arg0->count5 = 0;
            arg0->mv->weapon_status = 0;
        } else if (act == arg0->mv->rt_context_b) {
            arg0->speed = 0.0f;
            if ((arg4 != 0) && (arg0->attach_char != 0)) {
                arg0->mv->weapon_status = 0;
            } else {
                arg0->nojump = 0x14;
                if ((gCurrentPad->button & Z_TRIG) || (arg0->mv->prevButtons & Z_TRIG)) {
                    if (arg0->mv->AimState == 2) {
                        arg0->mv->deferredActions |= 8;
                        gActionsPending = 1;
                    } else {
                        arg0->mv->deferredActions |= 1;
                        gActionsPending = 1;
                    }
                }
                if (arg4 != 0) {
                    arg0->norun = 0xFF;
                    arg0->count5 = 0xFF;
                    anim = 0xC6;
                    if ((s32)arg0->shad != 0) {
                        arg0->mv->weapon_status = 0;
                    }
                } else if (!(gCurrentPad->button & Z_TRIG) && (arg0->mv->prevButtons & Z_TRIG) && (arg0->animseq.uh != 0xC4) && (arg0->mv->ReloadTimer == 0)) {
                    anim = 0xC7;
                    blend = 0.0f;
                    arg0->norun = 0xFF;
                    arg0->count5 = 0xFF;
                    arg0->mv->ReloadTimer = 0x3C;
                } else if ((gCurrentPad->button & Z_TRIG) && (arg0->animseq.uh != 0xC4)) {
                    arg0->norun = 0;
                    arg0->nojump = 8;
                    arg0->count5 = 8;
                    if (arg0->animseq.uh != 0xC7) {
                        anim = 0x10A;
                    }
                } else {
                    anim = actor_tryStartBAction(arg0, 0xC6, 1, 0, pressedButtons);
                }
            }
        } else {
            anim = actor_tryStartBAction(arg0, 0xC4, 1, act, pressedButtons);
            if (anim != 999) {
                arg0->speed = 0.0f;
            }
        }
        break;
    case 0x23:
        if (act == arg0->mv->rt_context_b) {
            if ((arg4 != 0) && (arg0->attach_char != 0)) {
                collision_removePartSet(arg0, 0x16);
                arg0->mv->weapon_status = 0;
            } else if (arg6 != 0) {
                clearAction = 0;
            } else if ((arg0->mv->SniperState != 0) && (arg0->mv->weapon_status == 2)) {
                arg0->nojump = 0xA;
                arg0->norun = 0;
                arg0->count5 = 0xA;
                if ((arg0->mv->ReloadTimer == 0) && ((gCurrentPad->button & Z_TRIG) || (arg0->mv->prevButtons & Z_TRIG))) {
                    arg0->mv->deferredActions |= 1;
                    gActionsPending = 1;
                }
                if (arg0->animseq.uh != 0x163) {
                    if ((arg0->mv->shotCount >= 6) || ((pressedButtons & A_BUTTON) && (arg0->animseq.uh != 0x26E))) {
                        if (arg0->shad == 0.0f) {
                            arg0->norun = 0xFF;
                            arg0->count5 = 0xFF;
                            arg0->mv->ReloadTimer = 0xB4;
                            anim = 0x26E;
                            arg0->mv->shotCount = 0;
                        }
                    } else if (!(gCurrentPad->button & Z_TRIG) && (arg0->mv->prevButtons & Z_TRIG) && (arg0->animseq.uh == 0x165) && (arg0->mv->ReloadTimer == 0)) {
                        arg0->mv->deferredActions |= 2;
                        gActionsPending = 1;
                        blend = 0.0f;
                        arg0->norun = 0xFF;
                        arg0->count5 = 0xFF;
                        anim = 0x167;
                        arg0->mv->shotCount++;
                        arg0->mv->ReloadTimer = 0x1E;
                    } else if (arg0->animseq.uh != 0x167) {
                        anim = 0x165;
                    }
                }
            } else {
                arg0->nojump = 0x14;
                if ((arg0->animseq.uh != 0x163) && ((arg0->mv->shotCount >= 6) || ((pressedButtons & A_BUTTON) && (arg0->animseq.uh != 0x26E)))) {
                    if (arg0->shad == 0.0f) {
                        arg0->norun = 0xFF;
                        arg0->count5 = 0xFF;
                        arg0->mv->ReloadTimer = 0xB4;
                        anim = 0x26E;
                        arg0->mv->shotCount = 0;
                    }
                } else if ((pressedButtons & Z_TRIG) && (arg0->animseq.uh != 0x163) && (arg0->animseq.uh != 0x166) && (arg0->mv->ReloadTimer == 0)) {
                    blend = 0.0f;
                    arg0->norun = 0xFF;
                    arg0->count5 = 0xFF;
                    anim = 0x166;
                    arg0->mv->shotCount++;
                    arg0->mv->ReloadTimer = 0x1E;
                } else {
                    anim = actor_tryStartBAction(arg0, 0x164, 1, 0, pressedButtons);
                    if (anim == 0x163) {
                        arg0->mv->shotCount = 0;
                    }
                }
            }
        } else {
            anim = actor_tryStartBAction(arg0, 0x163, 1, act, pressedButtons);
        }
        break;
    case 0x17:
        anim = 999;
        if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
            s32 pad;  // matching: unused stack slot
            s32 ok;
            f32 angle;

            ok = 1;
            if (arg0->mv->cont_zone_ind != 0) {
                angle = math_angleDiff(zones[arg0->mv->cont_zone_ind].yaw, arg0->theta);
                if ((zones[arg0->mv->cont_zone_ind].data1 != 0) && ((angle < -90.0f) || (angle > 90.0f))) {
                    ok = 0;
                }
            }
            if (ok != 0) {
                arg0->norun = 0xFF;
                arg0->count5 = 0xFF;
                arg0->mv->rt_context_b = 0x17;
                anim = 0x8E;
                arg0->angle = (u32)(zones[arg0->mv->cont_zone_ind].yaw * 182.0444489f) - 0x4000;
            }
        }
        break;
    case 0x1A:
        handled = 1;
        anim = 999;
        if (act != arg0->mv->rt_context_b) {
            if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
                arg0->norun = 0xFF;
                arg0->nojump = 0xFF;
                arg0->count5 = 0xFF;
                arg0->speed = 0.0f;
                anim = 0x12C;
                arg0->mv->rt_context_b = act;
                text_openContextText(0);
            }
        } else if (arg6 != 0) {
            text_closeContextText();
            arg0->mv->rt_context_b = 0;
        } else if (text_isContextTextClosed() != 0) {
            if (gContextTextId == 0x42) {
                arg0->mv->rt_context_b = 0;
                anim = 0x358;
                gSeenTextBits[gContextTextId >> 3] |= 1 << (gContextTextId & 7);
            } else {
                anim = 0x12E;
            }
            arg0->norun = 0xFF;
            arg0->count5 = 0xFF;
        } else if ((arg0->animseq.uh != 0x12C) && (gButtonsPressed[dc_glbl_CharNum] & 0x8000) && (!(gContextTextBoxConfig.flags & 0x20) || (func_151F2CDC() == 0))) {
            anim = 0x142;
        }
        break;
    case 0x2A:
        handled = 1;
        anim = 999;
        if (act != arg0->mv->rt_context_b) {
            if ((arg3 <= 0.0f) && (arg0->unk110 == 0) && (arg0->nojump == 0)) {
                arg0->norun = 0xFF;
                arg0->nojump = 0xFF;
                arg0->count5 = 0xFF;
                arg0->speed = 0.0f;
                arg0->mv->rt_context_b = act;
                anim = 0x26F;
                arg0->headEffect = 6;
                arg0->headEffectTimer = 0x64;
                arg0->headEffectState = 0;
                arg0->unk284 = 0;
                text_openContextText(1);
            }
        } else if (arg6 != 0) {
            text_closeContextText();
            arg0->mv->rt_context_b = 0;
        } else if (text_isContextTextClosed() != 0) {
            arg0->mv->rt_context_b = 0;
            arg0->norun = 0xFF;
            arg0->count5 = 0;
            face_revertExpression(arg0);
        } else {
            s32 sounds[2] = { 0x39, 0x35 };

            arg0->headEffectTimer = 0x64;
            if ((s8)arg0->unk284 == 0) {  // matching: read signed (lb); Actor declares u8
                arg0->headEffectState = arg0->headEffectState + gFrameTicks;
                if (((math_random() % 120U) + 0xB4) < arg0->headEffectState) {
                    face_requestExpression(arg0, sounds[math_random() & 1], 1, (math_random() % 180U) + 0x1A4, 0);
                    arg0->headEffectState = 0;
                    arg0->unk284 = 1;
                }
            } else if ((arg0->unk284 != 0) && (arg0->expression == arg0->defaultExpression)) {
                arg0->unk284 = 0;
            }
            arg0->norun = 0xA;
            arg0->nojump = 0xA;
            arg0->count5 = 0xA;
        }
        break;
    case 0x1B:
        {
            Actor *obj;
            s32 animHold;
            s32 animIdle;
            s32 animRelease;
            s32 animGrab;
            s32 animSpecial;
            s32 next;

            obj = &movement_obj[arg0->attach_char - 1];
            arg0->mv->rt_context_b = 0x1B;
            arg0->nojump = 0x14;
            if ((gIsMultiplayer == 0) || (arg0->attach_char != 0)) {
                clearAction = 0;
            }
            if (arg0->mv->ReloadTimer == 0) {
                next = 0;
                if (obj->character_type == 0x23) {
                    animHold = 0xD;
                    animIdle = 0x10;
                    animRelease = 0x15;
                    animGrab = 0xA;
                    animSpecial = 0xC;
                } else {
                    animHold = 4;
                    animIdle = 0xB;
                    animRelease = 0xC;
                    animGrab = 1;
                    animSpecial = 0;
                }
                if (gCurrentPad->button & Z_TRIG) {
                    if (animHold != obj->intelwalk) {
                        sound_stopActorSound1(obj);
                        sound_stopActorSound2(obj);
                    }
                    obj->intelwalk = animHold;
                    obj->scriptCmd = NULL;
                    arg0->mv->shotCount = 1;
                    if ((animGrab != obj->animseq.uh) || ((D_800C3A58 != 0) && (animSpecial != 0))) {
                        particle_spawnTwoSidedEmitter(obj, arg0, 0, animGrab);
                        sound_stopActorSound1(obj);
                        if ((arg0->character_type == 0x80) && !(math_random() & 3)) {
                            sound_addDynamicSound(sound_pickVariant(0x57C, 6), arg0->xpos, arg0->ypos, arg0->zpos, 0x7FFF, 0x3E8, 0x1F4, sound_followOwnerAnimsCallback, arg0, (s32)D_800996D0, 0, 0);
                        }
                    }
                    if (gFrameTicks < arg0->mv->gun_before_reload) {
                        arg0->mv->gun_before_reload -= gFrameTicks;
                    } else {
                        next = animRelease;
                        sound_stopActorSound2(obj);
                    }
                } else if ((D_800C3A58 != 0) && (animSpecial != obj->intelwalk) && (animSpecial != 0)) {
                    obj->intelwalk = animSpecial;
                    obj->scriptCmd = NULL;
                } else if (arg0->mv->shotCount == 1) {
                    arg0->mv->shotCount = 0;
                    if (sound_findByCallback(sound_delayedPlayCallback, (void *)-1, -1) == -1) {
                        sound_addDynamicSound((math_random() % 3U) + 0x618, arg0->xpos, arg0->ypos, arg0->zpos, 0x7D00, 0x1F4, 0xC8, sound_delayedPlayCallback, (void *)0x1E, 0, 0x300, 0);
                    }
                }
                if (pressedButtons & B_BUTTON) {
                    next = animIdle;
                    sound_stopActorSound1(obj);
                    sound_stopActorSound2(obj);
                }
                if (next != 0) {
                    arg0->mv->gun_before_reload = 0x258;
                    arg0->mv->ReloadTimer = 0x96;
                    sound_addDynamicSound((math_random() % 3U) + 0x618, arg0->xpos, arg0->ypos, arg0->zpos, 0x7D00, 0x1F4, 0xC8, sound_delayedPlayCallback, (void *)0x1E, 0, 0x300, 0);
                    obj->intelwalk = next;
                    obj->scriptCmd = NULL;
                }
            }
            anim = 999;
        }
        break;
    case 0x26:
        {
            s32 animThrow;
            s32 animDrop;
            s32 item;
            s32 sfx;

            animThrow = 0x176;
            if ((arg0->character_type == 0x75) || (animDrop = 0x16C, (arg0->character_type == 0xB1))) {  // matching: IDO schedules the default store between the two tests
                animDrop = 0x16B;
                item = 0x64;
                sfx = 0x11;
            } else {
                animThrow = 0x175;
                item = 0x2C;
                sfx = 0x12;
            }
            if (arg6 != 0) {
                if ((act == arg0->mv->rt_context_b) || (arg0->mv->GrenadeThrowState == 4)) {
                    if ((attach_find(arg0, item, 0) != NULL) || (arg0->mv->GrenadeThrowState == 4)) {
                        collision_removePartSet(arg0, item);
                        gEventParam = (u16)sfx | 0x10000;
                        event_spawnThrownItem();
                        if (arg0->mv->numGrenades != 0) {
                            arg0->mv->numGrenades--;
                        }
                    }
                    arg0->mv->GrenadeThrowState = 0;
                    arg0->mv->weapon_status = 0;
                }
            } else {
                actor_turnWaist(arg0, heldButtons, R_TRIG);
                if (arg0->mv->GrenadeThrowState == 1) {
                    if (act != arg0->mv->rt_context_b) {
                        arg0->mv->rt_context_b = act;
                        arg0->mv->weapon_status = 1;
                        arg0->norun = 0x1E;
                        arg0->nojump = 0x1E;
                        arg0->count5 = 0x1E;
                        arg0->mv->GrenadeThrowState = 2;
                        anim = animThrow;
                        arg0->mv->GrenadeTimer = 0xF0;
                        arg0->mv->shotsfired++;
                    }
                } else if (arg0->mv->GrenadeThrowState == 2) {
                    if (!(gActorHeldButtons & D_CBUTTONS) && !(arg0->unk2F8 & 0x10)) {
                        arg0->mv->GrenadeThrowState = 3;
                    }
                    if (gFrameTicks < arg0->mv->GrenadeTimer) {
                        arg0->mv->GrenadeTimer -= gFrameTicks;
                    } else {
                        arg0->mv->GrenadeThrowState = 0;
                        arg0->mv->rt_context_b = 0;
                        arg0->imune = 0;
                        actor_throwGrenade(arg0);
                    }
                } else if ((arg0->mv->GrenadeThrowState == 3) && (arg3 <= 0.0f) && (animThrow != arg0->animseq.uh)) {
                    arg0->norun = 0xFF;
                    arg0->count5 = 0xFF;
                    arg0->mv->GrenadeThrowState = 4;
                    anim = animDrop;
                    arg0->mv->rt_context_b = 0;
                    arg0->mv->weapon_status = 3;
                    if (arg0->mv->numGrenades != 0) {
                        arg0->mv->numGrenades--;
                    }
                }
            }
        }
        break;
    case 0x3A:
        {
            Actor *obj;
            s32 item;

            if (level == LEVEL_WMP_RAPTOR) {
                item = 0xBA;
            } else {
                item = 0x89;
            }
            if (arg6 != 0) {
                if (act == arg0->mv->rt_context_b) {
                    obj = actor_findLinked(arg0, item);
                    if (obj != NULL) {
                        obj->attach_char = 0;
                        obj->ygrav = 4.0f;
                        obj->yspd = 0.0f;
                        obj->speed = 0.0f;
                    }
                    arg0->mv->GrenadeThrowState = 0;
                    arg0->mv->rt_context_b = 0;
                    arg0->mv->weapon_status = 0;
                }
            } else {
                actor_turnWaist(arg0, heldButtons, R_TRIG);
                if (arg0->mv->GrenadeThrowState == 5) {
                    if (act != arg0->mv->rt_context_b) {
                        if (item == 0x89) {
                            obj = collision_createPartSet(arg0, 0x86, 1.0f, 0);
                        } else {
                            obj = collision_createPartSet(arg0, 0x9F, 1.0f, 0);
                        }
                        if (obj != NULL) {
                            arg0->mv->rt_context_b = act;
                            anim = 0x1F9;
                            arg0->mv->weapon_status = 1;
                            arg0->norun = 0x1E;
                            arg0->nojump = 0xFF;
                            arg0->count5 = 0xFF;
                            arg0->mv->GrenadeThrowState = 6;
                            obj->imune = 0xFF;
                            actor_setScale(obj, 1.0f, 1.0f);
                            obj->taskint[0] = 0xF0;
                            obj->taskint[1] = 0xF0;
                            obj->owner = dc_glbl_CharNum + 1;
                            obj->noshove = 0x401807;
                            actor_playAnim(obj, 2, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                            effect_attachPulseEffect(obj);
                        } else {
                            arg0->mv->GrenadeThrowState = 0;
                        }
                    }
                } else if (arg0->mv->GrenadeThrowState == 6) {
                    if (!(gActorHeldButtons & D_CBUTTONS)) {
                        arg0->mv->GrenadeThrowState = 7;
                    }
                } else if ((arg0->mv->GrenadeThrowState == 7) && (arg3 <= 0.0f) && (arg0->animseq.uh != 0x1F9)) {
                    arg0->norun = 0xFF;
                    arg0->count5 = 0xFF;
                    arg0->mv->GrenadeThrowState = 0;
                    anim = 0x1FC;
                    arg0->mv->rt_context_b = 0;
                    arg0->mv->weapon_status = 0;
                    if (arg0->mv->numGrenades != 0) {
                        arg0->mv->numGrenades--;
                    }
                    arg0->mv->shotsfired++;
                    obj = actor_findLinked(arg0, item);
                    if (obj != NULL) {
                        s16 pitch;
                        f32 vx;
                        f32 vy;
                        f32 vz;
                        f32 dist;

                        obj->attach_char = 0;
                        dist = ((240 - obj->taskint[0]) * 210.0f) / 240.0f;
                        dist += 40.0f;
                        if (arg0->mv->AimState != 0) {
                            pitch = (30.0f - arg0->mv->aimPitch) * 182.0444489f;
                        } else {
                            pitch = 0x1555;
                        }
                        actor_splitVelocity(pitch, dist, 0.0f, &vx, &vy, &vz);
                        obj->yspd = -vy;
                        obj->speed = vx;
                        obj->ygrav = 4.0f;
                        obj->angle = obj->langle = arg0->langle - arg0->mv->pssd_waist_tilt;
                    }
                }
            }
        }
        break;
    case 0x3E:
        if (arg0->team == 0) {
            if (arg0->count5 == 0) {
                arg0->count5 = 1;
            }
            if (arg0->mv->unk12C >= gFrameTicks) {
                arg0->mv->unk12C -= gFrameTicks;
                arg0->unkAA = 0x96;
            } else {
                arg0->unkAA = 0x4B;
                arg0->mv->unk12C = 0;
            }
        }
        break;
    }

    if ((arg4 != 0) || (arg6 != 0)) {
        arg0->mv->context_exclude_time = 0;
        if (arg6 != 0) {
            anim = 0;
            handled = 1;
            if (clearAction != 0) {
                arg0->mv->rt_context_b = 0;
            }
        } else {
            arg0->mv->context_b = 0;
            arg0->mv->rt_context_b = 0;
            if (arg0->mv->weapon_status != 3) {
                arg0->mv->weapon_status = 0;
            }
        }
    }
    if ((anim != 0) && (anim != 999) && (arg8 == 0)) {
        actor_playAnim(arg0, anim, speed, blend, animParam, 0.0f, 0);
    }
    if (setUnk58 != 0) {
        arg0->mv->idle.active = 1;
    }
    if (pendingSound != 0) {
        actor_playSound(pendingSound, arg0);
    }
    if (handled != 0) {
        return 1000;
    } else {
        return anim;
    }
}
