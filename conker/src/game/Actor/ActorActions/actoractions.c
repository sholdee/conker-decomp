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

/* Returns the angle from this object to the object it is linked to (targetIndex). */
s16 actor_getAngleToLinked(Actor *actor) {
    Actor *other;

    other = &movement_obj[actor->targetIndex];
    return actor_getDirectionAngle(other->xpos - actor->xpos, actor->zpos - other->zpos, 0);
}

void func_15063404(Actor *actor) {
    Actor *self = &movement_obj[actor - movement_obj];

    self->mv->rt_context_b = 9;
    collision_createPartSet(actor, 0x1D, 1.0f, 0);
    collision_createPartSet(actor, 0x1E, 1.0f, 0);
    actor->nojump = 20;
    actor->norun = 0;
    actor->count5 = 0;
    self->mv->gun_before_reload = 60;
    self->mv->weapon_status = 2;
    actor_playAnim(actor, 0x7F, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}


void func_150634E4(Actor *actor) {
    movement_obj[actor - movement_obj].mv->rt_context_b = 0;
    movement_obj[actor - movement_obj].mv->weapon_status = 0;
    collision_removePartSet(actor, 0x1D);
    collision_removePartSet(actor, 0x1E);
    actor->nojump = 0;
    actor->norun = 0;
    actor->count5 = 0;
}

void func_15063570(Actor *actor) {
    movement_obj[actor - movement_obj].mv->rt_context_b = 0x3B;
    movement_obj[actor - movement_obj].mv->weapon_status = 2;
    collision_createPartSet(actor, 0x89, 1.0f, 0);
    actor->nojump = 20;
    actor->norun = 0;
    actor->count5 = 0;
    actor_playAnim(actor, 0x221, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}


void actor_castAimRay(Actor *actor, f32 maxDist) {
    ActorCtl *ctl = actor->mv;

    collision_castRay(actor, ctl->aimEye.x, ctl->aimEye.y, ctl->aimEye.z,
                  ctl->aimDir.x, ctl->aimDir.y, ctl->aimDir.z, &ctl->rayHit,
                  maxDist, 0, 0, 0, -1, 0, 0);
    actor_markRayTargetSniped(actor);
}

/* Tells the actor hit by the last ray (if any) that this object is targeting it. */
void actor_markRayTargetSniped(Actor *actor) {
    Actor *target;
    ActorCtl *targetCtl;

    target = actor->mv->rayHit.obj;
    if (target != NULL) {
        targetCtl = target->mv;  // matching: the null test uses this copy; the stores reload mv
        if (targetCtl != NULL) {
            target->mv->being_sniped = 30;
            target->mv->being_s_char = actor - movement_obj;
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
s32 actor_getWaistOffsetToLinked(Actor *actor) {
    gWaistTiltTarget = actor->langle - actor->mv->pssd_waist_tilt - actor_getAngleToLinked(actor);
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
s32 actor_turnWaist(Actor *actor, s32 buttons, s32 mask) {
    if (buttons & mask) {
        gWaistTiltTarget = gCurrentPad->stick_x * 200;
        if (actor->mv->bot != 0) {
            gWaistTiltTarget = actor_getWaistOffsetToLinked(actor);
        }

        gWaistTiltDamping = 12.0f;
        gWaistTiltTarget = actor->mv->pssd_waist_tilt + ((gWaistTiltTarget * gActorStepPercent) / 2000);

        if (gWaistTiltTarget < -9000) {
            actor->angle = actor->langle = (actor->langle - gWaistTiltTarget) - 9000;
            gWaistTiltTarget = -9000;
        }

        if (gWaistTiltTarget > 9000) {
            actor->angle = actor->langle = (actor->langle - gWaistTiltTarget) + 9000;
            gWaistTiltTarget = 9000;
        }

        actor->mv->pssd_waist_tilt = gWaistTiltTarget;
        return 1;
    }
    return 0;
}

/* Throws one of the object's grenades (anim 0x64 for ids 0x75 and 0xB1, 0x2C otherwise). */
void actor_throwGrenade(Actor *actor) {
    ThrowPacket packet;

    if ((actor->character_type == 0x75) || (actor->character_type == 0xB1)) {
        packet.anim = 0x64;
    } else {
        packet.anim = 0x2C;
    }

    if (attach_find(actor, packet.anim, 0) != NULL) {
        if (actor->ob_matrix_ptr != NULL) {
            math_transformOffset(NULL, &packet.position, &actor->ob_matrix_ptr[4]);
        } else {
            packet.position.x = actor->xpos;
            packet.position.y = actor->ypos;
            packet.position.z = actor->zpos;
        }

        packet.throwerId = (actor - movement_obj) + 1;
        packet.thrower = actor;
        actor_runHitHandler(actor, 0xB, &packet);
        collision_removePartSet(actor, packet.anim);

        if (actor->mv->numGrenades != 0) {
            actor->mv->numGrenades--;
        }
        actor->mv->weapon_status = 0;
    }
}

/* Returns the active object with id `id` that is linked (unk65) to actor, or NULL.
 * (asm-differ shows relocation-addend diffs here: the .s names interior symbols of
 * movement_obj[], C spells them base+offset; the linked bytes are identical.) */
Actor *actor_findLinked(Actor *actor, s32 id) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((movement_obj[i].type != 0) && (id == movement_obj[i].character_type) &&
            (((actor - movement_obj) + 1) == movement_obj[i].attach_char)) {
            return &movement_obj[i];
        }
    }
    return NULL;
}

/* Starts `anim` as a new action when the object stands on the ground with nothing else going
 * on and B is pressed. Returns `anim`, or 999 when the action can't start. */
s32 actor_tryStartBAction(Actor *actor, u16 anim, u16 flags, s32 action, s32 buttons) {
    s32 state;

    if ((buttons & B_BUTTON) && (actor->shad == 0.0f) && (actor->yspd <= 0.0f) &&
        (actor->mv->ladder == 0) && (actor->animseq.uh != anim)) {
        state = actor->mv->weapon_status;
        if ((state == 0 || state == 2) && (actor->mv->jump_on_char == 0)) {
            if (action != 0 && actor->nojump != 0) {
                return 999;
            }
            if (flags & 1) {
                actor->nojump = 20;
            }
            if (action != 0) {
                if (actor->mv->weapon_status == 2) {
                    return 999;
                }
                actor->mv->weapon_status = 1;
            } else {
                actor->mv->weapon_status = 3;
            }
            actor->mv->rt_context_b = action;
            actor->norun = 0xFF;
            actor->count5 = 0xFF;
            return anim;
        }
    }
    return 999;
}

/* Fires a shot of kind `kind` (2, 3, 6, 0x12) from the object's muzzle (unk13C..unk144),
 * aimed along its yaw/pitch or, when unk109 is set, at the point unkB8..unkC0. */
void actor_fireShot(Actor *actor, s32 kind) {
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
    angle = actor->mv->aimYaw * 0.01745329238f;
    off[0] = sinf(angle) * radius;
    off[1] = cosf(angle) * radius;
    pos[0] = off[0] + actor->mv->aimEye.x;
    pos[1] = actor->mv->aimEye.y + height;
    pos[2] = off[1] + actor->mv->aimEye.z;
    ctl = actor->mv;
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
u16 actor_pickRandomAnim(Actor *obj, u16 animA, u16 animB, u16 animC) {
    u16 selected;
    Actor *actor = obj;  // matching: the copy reserves the extra stack slot IDO gave the original
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
void func_150642AC(Actor *actor, s32 buttons, f32 height, s32 noPlay, s32 lockB) {
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
    if (actor->mv->inst_context != 0) {
        switch (actor->mv->inst_context) {
        case 1: {
            s16 pad;  // matching: unused stack slot

            actor->count5 = 0xFF;
            actor->norun = 0xFF;
            x = (actor->taskint[0] >> 16) & 0xFFFF;
            z = actor->taskint[0] & 0xFFFF;
            actor->animseq.uh = 0;
            actor->mv->gpoint = 0xD;
            actor->mv->swing_xvel = x;
            actor->mv->swing_zvel = z;
            anim = 0x96;
            actor->angle = actor->taskint[1] & 0xFFFF;
        }
            break;
        case 2:
            speed = 0.8000000119f;
            anim = 0xDD;
            actor->norun = 0xFF;
            actor->count5 = 0xFF;
            break;
        case 3:
            gEventParam = 1;
            func_15074664();
            if (actor->inwater != 0 || (s32)actor->shad != 0) {
                collision_createPartSet(actor, 0x17, 1.0f, 0);
                actor->mv->weapon_status = 2;
            } else {
                actor->norun = 0xFF;
                actor->nojump = 0xFF;
                actor->count5 = 0xFF;
                speed = 3.0f;
                anim = 0x151;
                actor->mv->weapon_status = 1;
            }
            break;
        case 4:
            gEventParam = 0;
            func_15074664();
            if (actor->inwater != 0 || (s32)actor->shad != 0) {
                collision_removePartSet(actor, 0x17);
                actor->mv->weapon_status = 0;
            } else {
                actor->norun = 0xFF;
                actor->nojump = 0xFF;
                actor->count5 = 0xFF;
                speed = 3.0f;
                anim = 0x14E;
                actor->mv->weapon_status = 3;
            }
            break;
        case 5:
            obj = actor_findByType(0x6C);
            actor->count3 = 1;
            if (obj->taskint[0] == 0) {
                actor->fallStartY = actor->ypos;
                obj->taskint[0] = 100;
                anim = 0x38;
            }
            if (obj->taskint[0] >= 100) {
                if (obj->ob_matrix_ptr != NULL) {
                    mtx = &((MtxS16 *)obj->ob_matrix_ptr)[5];
                    actor->ypos = mtx->intPart[3][1] + 20;
                }
            }
            if (buttons & A_BUTTON) {
                if (obj->taskint[0] == 100) {
                    obj->taskint[0] = 0x7A;
                    obj->taskint[1] = 0x7A;
                    anim = 0x16;
                    actor_playSound(0x38E, actor);
                }
            }
            break;
        case 6:
        case 8:
            if ((buttons & B_BUTTON) && (height <= 0.0f)) {
                speed = 2.0f;
                actor->count5 = 0xFF;
                actor->norun = 0xFF;
                anim = 0x33;
            }
            break;
        case 9:
            anim = 0x22D;
            actor->norun = 0xFF;
            actor->count5 = 0xFF;
            break;
        case 10:
            if (actor->shad <= 1.0f) {
                anim = 0x19A;
                actor->count5 = 0xFF;
            }
            actor->norun = 0xC8;
            actor->nojump = 0xC8;
            break;
        case 12:
            if (actor->shad <= 1.0f) {
                anim = 0x2E7;
                actor->count5 = 0xFF;
            }
            actor->norun = 0xC8;
            actor->nojump = 0xC8;
            break;
        case 11:
            dx = actor->xpos - -1884.0f;
            dz = actor->zpos - 100.0f;
            dist = sqrtf(dx * dx + dz * dz);
            if (dist < 1000.0f) {
                u16 angle;

                angle = actor_getDirectionAngle(-dx, dz, 0);
                actor_splitVelocity(angle, 20.0f, 0.0f, &pushX, &pushY, &pushZ);
                actor_stepVector2D(pushX, pushY, (1200.0f - dist) * 0.001000000047f, &actor->ob_overslopex, &actor->ob_overslopey);
            }
            break;
        default:
            break;
        }
        actor->mv->inst_context = 0;
    }
    if (actor->mv->AimState == 0 && anim == 0 && (buttons & B_BUTTON) && (actor->mv->transport & 0xF) != 1 &&
        height <= 0.0f && actor->norun != 0xFF) {
        if (actor->lifting >= 100) {
            if (!(actor->task_attrib & 0x20)) {
                anim = 0xD6;
                speed = 1.149999976f;
                if (gIsMultiplayer != 0) {
                    anim = 0x118;
                    speed = 1.0f;
                }
                if (level == LEVEL_WMP_BEACH_WAR) {
                    anim = 0x28D;
                }
                actor->count5 = 0xFF;
                actor->norun = 0xFF;
            }
        } else if ((actor->mv->context_b == 0 && actor->mv->unk7C == 0) || actor->mv->transport == 2) {
            if (level != LEVEL_WMP_MAIN_WAR && level != LEVEL_WMP_BEACH && level != LEVEL_WMP_BEACH_WAR && level != LEVEL_WMP_TUNNELS) {
                anim = 0x33;
                if (actor->mv->pissed_up != 0) {
                    actor->count5 = 0xFF;
                    actor->norun = 0xFF;
                    speed = 1.600000024f;
                } else {
                    actor->count5 = 0xFF;
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
                    if (actor->mv->bot == 0 && actor->mv->transport == 0) {
                        actor->norun = 0xFF;
                        actor->yspd = -4.0f;
                        actor->angle = actor->mv->press_angle;
                        speed = 1.0;  // matching: double literal; 1.0f drops its .rodata entry
                        anim = 0x46;
                    }
                    actor->count2 = 1;
                    blend = 0.0f;
                }
            }
        }
    }
    if (anim != 0 && anim != 999 && !noPlay) {
        actor_playAnim(actor, anim, speed, blend, 0.0f, 0.0f, 0);
    }
}

/* Copies object srcIndex's animation position (anim->unk8) to object dstIndex, keeping it
 * below dstIndex's unk18. */
void actor_copyAnimPosition(s32 srcIndex, s32 dstIndex) {
    Actor *objects;
    AnimInstance *dst;
    AnimInstance *src;

    objects = movement_obj;
    dst = objects[dstIndex].anim;
    src = objects[srcIndex].anim;

    if (dst != NULL && src != NULL) {
        dst->frame[0] = src->frame[0];
        if (dst->numFrames[0] <= dst->frame[0]) {
            dst->frame[0] = dst->numFrames[0] - 1.0f;
        }
    }
}

/* Plays the animation that pairs with what the linked object (unk65) is doing, and syncs
 * this object's animation to it. */
void actor_syncAnimToLinked(Actor *actor) {
    Actor *partner;
    s32 anim;
    s32 variant;

    partner = &movement_obj[actor->attach_char - 1];
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
        variant = ((actor->mv != NULL) && (actor->mv->unk1B2 != 0)) ? 1 : 0;
        if (partner->intelwalk == 4) {
            anim = variant ? 0x350 : 0x1A4;
        } else {
            anim = variant ? 0x34F : 0x1A1;
        }
    }
    actor_playAnim(actor, anim, 1.0f, 6.0f, 0.0f, 0.0f, 0);
    actor_copyAnimPosition(actor->attach_char - 1, dc_glbl_CharNum);
}

/* Picks the object's movement animation from its action and speed, plays it, and returns it
 * (0 or 999 when nothing new plays). */
s32 actor_updateMoveAnim(Actor *actor, f32 vel, f32 arg2, s32 overrideAnim, f32 moveSpeed) {
    s32 anim;
    f32 blend;
    f32 speed;
    ActorCtl *ctl;

    anim = 0;
    blend = 4.0f;
    ctl = actor->mv;
    speed = 1.0f;
    if (ctl->bitten > 0) {
        anim = 0x4C;
        speed = vel / 24.0f;
    } else if (ctl->transport == 2) {
        anim = 5;
        if (ctl->unk50 > 0) {
            anim = 0x54;
        }
        blend = 2.0f;
    } else if (actor->lifting >= 100) {
        if (actor->speed < 1.0f) {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        } else {
            switch (movement_obj[gCurrentActor->lifting - 100].character_type) {
            case 0x8C:
                actor->speedPercent = 0x5A;
                anim = 0x1A6;
                speed = vel / 6.0f + 0.4f;
                if (actor->speed > 14.0f) {
                    anim = 0x1A7;
                    speed = vel / 26.0f + 0.5f;
                }
                break;
            case 0x57:
                actor->speedPercent = 0x5A;
                anim = 0x116;
                speed = vel / 6.0f + 0.4f;
                if (actor->speed > 14.0f) {
                    anim = 0x117;
                    speed = vel / 26.0f + 0.5f;
                }
                break;
            case 0xA8:
            case 0xA9:
                if (actor->speed > 14.0f) {
                    anim = 0x1B1;
                    speed = vel / 22.0f + 0.4f;
                } else {
                    anim = 0x1B0;
                    speed = vel * 0.0625f + 0.6f;
                }
                break;
            case 0x13:
                anim = 0x14C;
                speed = vel / 6.0f;
                break;
            case 0x89:
                anim = 0x1FB;
                speed = vel / 6.0f;
                break;
            default:
                if (actor->speed > 14.0f) {
                    anim = 0x2E4;
                    speed = vel * 0.0625f + 0.5f;
                } else {
                    anim = 0xD7;
                    speed = vel / 6.0f + 0.4f;
                }
                break;
            }
        }
    } else if (overrideAnim != 0 && ctl->rt_context_b == 0) {
        anim = overrideAnim;
        speed = vel / 15.0f + 0.6f;
    } else if (2.6f < actor->stickspeed && (ctl->walk_attrib & 1)) {
        actor->speedPercent = 0x28;
        blend = 5.0f;
        speed = actor->stickspeed / 15.0f + 0.5f;
        anim = actor_getDuckWalkAnim();
    } else if (ctl->rt_context_b == 0x16) {
        if (actor->stickspeed >= 1.0f) {
            anim = ctl->unk1B2 != 0 ? 0x2C6 : 0xE8;
            actor->speedPercent = 0x1E;
            speed = vel * 0.0625f + 0.6f;
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 9) {
        if (actor->stickspeed >= 1.0f) {
            if (gIsMultiplayer == 0) {
                actor->speedPercent = 0x37;
            }
            anim = 0x7E;
            if (gIsMultiplayer == 0) {
                speed = vel * 0.0625f + 0.6f;
            } else if (actor->stickspeed >= 20.0f) {
                anim = 0x22C;
                speed = vel / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x38) {
        if (actor->stickspeed >= 1.0f) {
            anim = 0x1E1;
            if (actor->stickspeed >= 20.0f) {
                anim = 0x1E2;
                speed = vel / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x39) {
        if (actor->animseq.uh != 0x1EB) {
            if (actor->stickspeed >= 1.0f) {
                anim = 0x1EC;
                if (actor->stickspeed >= 20.0f) {
                    anim = 0x1ED;
                    speed = vel / 22.0f + 0.4f;
                }
            } else {
                speed = 1.0f;
                anim = actor_updateIdleAnim();
            }
        } else {
            anim = 999;
        }
    } else if (ctl->rt_context_b == 0x37) {
        if (actor->animseq.uh != 0x1F4) {
            if (actor->stickspeed >= 1.0f) {
                anim = 0x1F5;
                if (actor->stickspeed >= 20.0f) {
                    anim = 0x1F6;
                    speed = vel / 22.0f + 0.4f;
                }
            } else {
                speed = 1.0f;
                anim = actor_updateIdleAnim();
            }
        } else {
            anim = 999;
        }
    } else if (ctl->rt_context_b == 0x3B) {
        if (actor->animseq.uh != 0x222 && actor->animseq.uh != 0x236) {
            if (actor->stickspeed >= 1.0f || ctl->door_walkf_time > 0) {
                anim = 0x223;
                actor->speedPercent = 0x37;
                if (actor->stickspeed >= 20.0f) {
                    anim = 0x224;
                    speed = vel / 22.0f + 0.4f;
                }
            } else {
                speed = 1.0f;
                anim = actor_updateIdleAnim();
            }
        } else {
            anim = 999;
        }
    } else if (ctl->rt_context_b == 0x12 || ctl->rt_context_b == 0x18 || ctl->rt_context_b == 0x41) {
        if (actor->stickspeed >= 1.0f) {
            actor->speedPercent = 0x50;
            anim = 0xEC;
            speed = vel * 0.0625f + 0.6f;
            if (actor->stickspeed >= 20.0f) {
                anim = 0xF8;
                speed = vel / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x24) {
        if (actor->stickspeed >= 1.0f) {
            actor->speedPercent = 0x50;
            anim = 0x158;
            speed = vel * 0.0625f + 0.6f;
            if (actor->stickspeed >= 20.0f) {
                anim = 0x159;
                speed = vel / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x22) {
        if (actor->stickspeed >= 1.0f) {
            actor->speedPercent = 0x50;
            anim = 0x15B;
            speed = vel * 0.0625f + 0.6f;
            if (actor->stickspeed >= 20.0f) {
                anim = 0x15C;
                speed = vel / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x21) {
        if (actor->stickspeed >= 1.0f) {
            actor->speedPercent = 0x6E;
            anim = 0x123;
            speed = vel * 0.0625f + 0.6f;
            if (actor->stickspeed >= 15.0f) {
                anim = 0x124;
                speed = vel / 27.0f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x26) {
        if (actor->stickspeed >= 1.0f) {
            actor->speedPercent = 0x50;
            anim = 0x178;
            speed = vel * 0.0625f + 0.6f;
            if (actor->stickspeed >= 20.0f) {
                anim = 0x179;
                speed = vel / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x3A) {
        if (actor->stickspeed >= 1.0f) {
            actor->speedPercent = 0x50;
            anim = 0x1FB;
            speed = vel * 0.0625f + 0.6f;
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x25) {
        if (actor->stickspeed >= 1.0f) {
            actor->speedPercent = 0x50;
            anim = 0xD7;
            speed = vel * 0.0625f + 0.6f;
            if (actor->stickspeed >= 20.0f) {
                speed = vel / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x2D || ctl->rt_context_b == 0x2E) {
        if (actor->stickspeed >= 1.0f) {
            anim = 0x1B0;
            speed = vel * 0.0625f + 0.6f;
            if (actor->stickspeed >= 20.0f) {
                anim = 0x1B1;
                speed = vel / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x23) {
        if (actor->stickspeed >= 1.0f) {
            actor->speedPercent = 0x50;
            anim = 0x161;
            speed = vel * 0.0625f + 0.6f;
            if (actor->stickspeed >= 20.0f) {
                anim = 0x162;
                speed = vel / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x15) {
        if (actor->stickspeed >= 1.0f) {
            actor->speedPercent = 0x50;
            anim = 0xED;
            speed = vel * 0.0625f + 0.6f;
            if (actor->stickspeed >= 20.0f) {
                speed = vel / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->rt_context_b == 0x19 || ctl->rt_context_b == 0x40) {
        if (actor->speed >= 1.0f) {
            anim = 0x112;
            blend = 8.0f;
            speed = vel * 0.125f + 0.3f;
            if (actor->speed >= 18.0f) {
                anim = 0x111;
                speed = vel / 20.0f + 0.3f;
            }
        } else {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        }
    } else if (ctl->injured != 0 && arg2 > 1.0f) {
        anim = 0x6D;
        speed = vel / 6.5f + 0.3f;
        actor->speedPercent = 0x28;
        blend = 8.0f;
    } else if (ctl->pissed_up > 0) {
        if (arg2 < 1.0f) {
            speed = 1.0f;
            anim = actor_updateIdleAnim();
        } else {
            anim = ctl->pissed_walk;
            speed = vel / 6.0f + 0.3f;
            blend = 16.0f;
        }
    } else if (ctl->env_suit != 0) {
        blend = 5.0f;
        if (gCurrentPad->button & Z_TRIG) {
            if (actor->animseq.uh != 0x283) {
                actor->speed *= 0.2f;
                gEventParam = 0xFF010604;
                event_playSound2Range1800();
                speed = 1.0f;
            }
            anim = 0x283;
            actor->norun = 3;
            actor->nojump = 3;
        } else {
            if (actor->animseq.uh == 0x283) {
                gEventParam = 0xFF010604;
                event_playSound2Range1800();
            }
            if (actor->stickspeed >= 1.0f && actor->norun == 0) {
                actor->speedPercent = 0x2D;
                speed = actor->speed / 9.0f;
                anim = 0x4E;
            } else {
                speed = 1.0f;
                anim = actor_updateIdleAnim();
            }
        }
    } else if ((ctl->walk_attrib & 2) && actor->stickspeed >= 1.0f) {
        ctl->turn = 0x1C;
        actor->speedPercent = 0xF;
        speed = moveSpeed / 5.0f + 0.3f;
        blend = 8.0f;
        anim = 0x64;
        if (math_random() % 1000U >= 988) {
            anim = (math_random() & 1) + 0x65;
            speed = 0.65f;
            actor->norun = 0xFF;
            actor->count5 = 0xFF;
        }
    }
    if (anim != 0 && anim != 999) {
        actor_playAnim(actor, anim, speed, blend, 0.0f, 0.0f, 0);
    }
    return anim;
}

/* Per context action (act 2, 0x13, 0xA, 0x34): the animations of its four steps: start (B), hold, the Z
 * press while holding, and end. */
static const u16 gContextStepAnims[][4] = {
    { 0x0138, 0x003E, 0x003D, 0x0139 },
    { 0x0138, 0x003E, 0x00D9, 0x0139 },
    { 0x0035, 0x003A, 0x003F, 0x005D },
    { 0x01CD, 0x01CE, 0x01CF, 0x01D0 },
};
/* Zero-terminated animation lists for sound_followOwnerAnimsCallback (via sound_addDynamicSound): the sound
 * follows its owner while the owner plays one of them. */
static const u16 D_800996C0[] = { 0x007C, 0x020D, 0x0238, 0 };
static const u16 D_800996C8[] = { 0x01E0, 0x0253, 0x0254, 0 };
static const u16 D_800996D0[] = { 0x01A4, 0x0350, 0 };

/* Per-frame handler for the object's held item / special action `act`: starts, steps and ends
 * it from the held and pressed buttons. Returns the animation started (0 or 999 for none), or
 * 1000 when the action consumed the input. */
s32 func_15065A5C(Actor *actor, s32 heldButtons, s32 pressedButtons, f32 height, s32 nested, s32 act, s32 isStunned, s32 expired, s32 noPlay) {
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

    if ((actor->mv->weapon_status == 3) || (actor->stunned != 0) || (nested != 0)) {
        pressedButtons &= ~B_BUTTON;
        gActorNewButtons &= ~B_BUTTON;
        if (nested != 0) {
            if (actor->stunned == 0) {
                actor->events = NULL;
            }
            pressedButtons &= ~Z_TRIG;
            gActorNewButtons &= ~Z_TRIG;
        }
    }
    if (actor->yspd > 0.0f) {
        height += 50.0f;
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
            if (isStunned != 0) {
                switch (act) {
                case 0x34:
                    collision_removePartSet(actor, 0x3D);
                    actor->mv->weapon_status = 0;
                    break;
                case 0xA:
                    collision_removePartSet(actor, 0x28);
                    collision_removePartSet(actor, 0x29);
                    actor->mv->weapon_status = 0;
                    break;
                }
            }
            if (act == actor->mv->rt_context_b) {
                actor->speed = 0.0f;
            }
            if ((actor->inwater != 0) && ((act == actor->mv->rt_context_b) || (actor->mv->weapon_status != 0))) {
                actor->mv->rt_context_b = 0;
                actor->norun = 0;
                actor->count5 = 0;
                if (act == 0xA) {
                    collision_removePartSet(actor, 0x28);
                    collision_removePartSet(actor, 0x29);
                    actor->mv->weapon_status = 0;
                }
            } else if (((height <= 0.0f) && (pressedButtons & B_BUTTON)) || (nested != 0)) {
                if ((actor->mv->rt_context_b != 0) || (nested != 0)) {
                    if ((actor->mv->context_exclude_time == 0) || (nested != 0)) {
                        actor->norun = 0xFF;
                        actor->nojump = 0xFF;
                        actor->count5 = 0xFF;
                        anim = gContextStepAnims[idx][3];
                        actor->mv->weapon_status = 3;
                        actor->mv->context_exclude_time = 0x5A;
                        setUnk58 = 1;
                    }
                } else if (actor->mv->context_exclude_time == 0) {
                    if ((actor->CameraPtr->DoingAim != 1) && (actor->CameraPtr->DoingAim != 3)) {
                        actor->mv->ind_cnt = 0;
                        actor->mv->weapon_status = 1;
                        anim = gContextStepAnims[idx][0];
                        actor->count5 = 0xFF;
                        actor->norun = 0xFF;
                        actor->mv->context_exclude_time = 0x5A;
                    }
                }
                pressedButtons &= ~B_BUTTON;
            }
            if (actor->mv->rt_context_b && (pressedButtons & Z_TRIG) && (anim == 0) && (actor->animseq.uh == gContextStepAnims[idx][1])) {
                anim = gContextStepAnims[idx][2];
                actor->mv->ind_cnt++;
            }
            if (((actor->animseq.uh == gContextStepAnims[idx][1]) || (actor->animseq.uh == gContextStepAnims[idx][0]) ||
                 (actor->animseq.uh == gContextStepAnims[idx][2]) || (anim != 0)) && (anim != gContextStepAnims[idx][3])) {
                actor->mv->rt_context_b = act;
                gCurrentPad->button &= ~Z_TRIG;
                setUnk58 = 1;
            } else {
                actor->mv->rt_context_b = 0;
            }
            if (D_800C3A58 != 0) {
                if ((actor->mv->weapon_status == 1) || (actor->mv->weapon_status == 2U)) {
                    anim = gContextStepAnims[idx][1];
                    actor->count5 = 0xFF;
                    actor->mv->rt_context_b = act;
                    actor->mv->weapon_status = 2;
                    switch (act) {
                    case 0xA:
                        collision_createPartSet(actor, 0x28, 1.0f, 0);
                        collision_createPartSet(actor, 0x29, 1.0f, 0);
                        break;
                    case 0x2:
                    case 0x13:
                        collision_createPartSet(actor, 8, 1.0f, 0);
                        break;
                    }
                } else {
                    actor->mv->weapon_status = 0;
                    actor->mv->rt_context_b = 0;
                    switch (act) {
                    case 0x34:
                        collision_removePartSet(actor, 0x3D);
                        break;
                    case 0xA:
                        collision_removePartSet(actor, 0x28);
                        collision_removePartSet(actor, 0x29);
                        break;
                    case 0x2:
                    case 0x13:
                        collision_removePartSet(actor, 8);
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
            if ((expired != 0) && (actor->mv->rt_context_b != 0)) {
                collision_createPartSet(actor, sfx, 1.0f, 0);
            }
            if ((pressedButtons & B_BUTTON) && (height <= 0.0f)) {
                if (actor->mv->rt_context_b != 0) {
                    actor->norun = 0xFF;
                    actor->count5 = 0xFF;
                    actor->nojump = 0xFF;
                    actor->mv->weapon_status = 3;
                    speed = 2.0f;
                    anim = animEnd;
                    actor->mv->rt_context_b = 0;
                } else {
                    actor->norun = 0x10;
                    actor->count5 = 0x10;
                    actor->nojump = 0x10;
                    speed = 2.0f;
                    actor->mv->weapon_status = 1;
                    anim = animStart;
                    actor->mv->rt_context_b = act;
                }
            }
            if (anim != 999) {
                speed = 2.0f;
            } else if ((nested != 0) && (actor->attach_char != 0)) {
                collision_removePartSet(actor, sfx);
                actor->mv->weapon_status = 0;
            } else if (act == actor->mv->rt_context_b) {
                actor->speedPercent = 0x4B;
                if (isStunned != 0) {
                    clearAction = 0;
                } else if (actor->mv->SniperState != 0) {
                    actor->nojump = 0xA;
                    actor->norun = 0;
                    if (pressedButtons & Z_TRIG) {
                        if (actor->mv->context_exclude_time == 0) {
                            actor->count5 = 0xF;
                            actor->mv->deferredActions |= 3;
                            gActionsPending = 1;
                            anim = animHit;
                            animParam = 6.0f;
                            actor->mv->context_exclude_time = 0x28;
                        }
                    }
                } else if ((pressedButtons & Z_TRIG) && (height <= 0.0f) && (actor->norun == 0) && (actor->mv->context_exclude_time == 0)) {
                    actor->mv->context_exclude_time = 0x28;
                    if (actor->speed > 2.0f) {
                        anim = animFast;
                    } else {
                        anim = animSlow;
                    }
                    actor->norun = 0xF;
                    actor->count5 = 0xF;
                    actor->nojump = 0xF;
                }
            }
        }
        break;
    case 0x3:
        handled = 1;
        if ((nested != 0) || (isStunned != 0)) {
            if (level != LEVEL_W2_BOILERROOM) {
                actor->mv->rt_context_b = 0;
                actor->mv->pssd_is_pissing = 0;
                actor->count5 = 0;
            }
            if (isStunned != 0) {
                clearAction = 0;
            }
        } else if (actor->mv->pissed_hangover == 0) {
            if (actor->mv->pissed_up != 0) {
                actor->mv->pissed_up -= gFrameSteps;
                if (actor->mv->pissed_up < 0x64) {
                    actor->mv->pissed_walk = 0xAD;
                    actor->mv->pissed_hangover = 0xC8;
                    face_setDefaultExpression(actor, 0x2A);
                    speed = 1.3f;
                    actor->norun = 0xFF;
                    actor->count5 = 0xFF;
                    anim = 0x2D;
                    actor->mv->rt_context_b = 0;
                }
            }
            if (actor->mv->pissed_hangover == 0) {
                if ((height <= 0.0f) && (pressedButtons & B_BUTTON)) {
                    actor->norun = 0xFF;
                    actor->count5 = 0xFF;
                    if (actor->mv->rt_context_b != 0) {
                        anim = 0x2D;
                        speed = 1.3f;
                        actor->mv->rt_context_b = 0;
                    } else {
                        actor->mv->rt_context_b = 3;
                        anim = 0x2E;
                        actor->mv->pssd_is_pissing = 0;
                        speed = 1.6f;
                    }
                } else if ((actor->mv->rt_context_b == 3) && (actor->norun != 0xFF)) {
                    actor->mv->rt_context_b = 3;
                    anim = 0x2C;
                    if (gIsMultiplayer != 0) {
                        actor->nojump = 0xA;
                    }
                    gWaistTiltTarget = gCurrentPad->stick_x * 0x96;
                    if (actor->mv->bot != 0) {
                        actor->mv->pssd_waist_tilt = 0;
                    } else if ((gWaistTiltTarget < -0x1F40) && (actor->mv->pssd_waist_tilt < -0x1B58)) {
                        actor->angle = actor->langle = actor->angle + (gActorStepPercent * 7);
                        anim = 0xE0;
                        speed = 3.0f;
                    } else if ((gWaistTiltTarget > 0x1F40) && (actor->mv->pssd_waist_tilt > 0x1B58)) {
                        actor->angle = actor->langle = actor->angle - (gActorStepPercent * 7);
                        anim = 0xE0;
                        speed = 3.0f;
                    }
                    gWaistTiltDamping = 10.0f;
                    blend = 8.0f;
                    actor->norun = 0x10;
                    actor->count5 = 0x10;
                    if (actor->mv->pssd_is_pissing == 0) {
                        actor->mv->pssd_is_pissing = 1;
                        trail_spawnPissTrail(actor);
                    }
                }
            }
        }
        break;
    case 0x1:
    case 0x35:
        if ((pressedButtons & B_BUTTON) && ((height <= 0.0f) || (act == 0x35)) && (movement_obj[0].mv->transport != 1) &&
            (actor->lifting < 0x64) && (actor->unkA9 == 0)) {
            actor->count3 = 1;
            actor->unkA9 = 2;
            actor->norun = 0x78;
            actor->ygrav = 6.0f;
            actor->fallStartY = -100000.0f;
            anim = 0x4A;
            actor->speed *= 0.5f;
            speed = 1.799999952f;
            actor->yspd = 80.0f;
            actor->mv->prop = 0;
        }
        break;
    case 0x4:
        if (pressedButtons & B_BUTTON) {
            actor->mv->rt_context_b = act;
            return 0;
        }
        actor->mv->rt_context_b = 0;
        return 0;
    case 0x42:
        if (!(gSeenTextBits[8] & 4)) {
            if (pressedButtons & B_BUTTON) {
                actor->mv->rt_context_b = act;
                return 0;
            }
            actor->mv->rt_context_b = 0;
        }
        return 0;
    case 0x5:
        if ((pressedButtons & B_BUTTON) && (height <= 0.0f)) {
            if (actor->animseq.uh == 0x2F) {
                anim = 999;
            } else {
                actor->mv->gpoint = 0x12;
                anim = 0x2F;
                if (level == LEVEL_W1_ROCK_SOLID) {
                    actor->mv->swing_xvel = zones[12].x;
                    actor->mv->swing_zvel = zones[12].z;
                    effect_createFallingStream(5);
                } else {
                    actor->mv->swing_xvel = zones[2].x;
                    actor->mv->swing_zvel = zones[2].z;
                    effect_createFallingStream(0);
                }
                actor->angle = 0xC000;
                speed = 1.049999952f;
                blend = 6.0f;
                actor->norun = 0xFF;
                actor->count5 = 0xFF;
                pendingSound = 0x143;
            }
        }
        break;
    case 0x6:
        if (actor->mv->rt_context_b == 0) {
            if ((pressedButtons & B_BUTTON) && (height <= 0.0f) && (actor->mv->context_exclude_time == 0)) {
                actor->norun = 0xFF;
                actor->count5 = 0xFF;
                anim = 0x44;
                actor->mv->rt_context_b = 6;
            }
        } else if (actor->animseq.uh != 0x44) {
            actor->norun = 0;
            actor->count5 = 0;
            actor->mv->rt_context_b = 0;
        } else {
            anim = 999;
        }
        break;
    case 0x36:
        if ((pressedButtons & B_BUTTON) && (height <= 0.0f)) {
            ModelAttachment *found;

            found = attach_find(actor, 0x3C, 0);
            if ((found != NULL) && (found->hookState < 0x12C)) {
                anim = 0x240;
                actor->norun = 0xFF;
                actor->nojump = 0xFF;
                actor->count5 = 0xFF;
            }
        }
        break;
    case 0x7:
        if (actor->mv->rt_context_b == 0) {
            if ((pressedButtons & B_BUTTON) && (height <= 0.0f) && (actor->mv->context_exclude_time == 0)) {
                actor->norun = 0xFF;
                actor->count5 = 0xFF;
                anim = 0x45;
                actor->mv->rt_context_b = 7;
            }
        } else if (actor->animseq.uh != 0x45) {
            actor->norun = 0;
            actor->count5 = 0;
            actor->mv->rt_context_b = 0;
        } else {
            anim = 999;
        }
        break;
    case 0x8:
        {
            Actor *obj;

            obj = &movement_obj[actor->attach_char - 1];
            actor->nojump = 0x14;
            if (pressedButtons & (A_BUTTON | B_BUTTON)) {
                if ((obj->animseq.uh == 0) || (obj->alertCooldown != 0) || (obj->animseq.uh == 0xC) || (obj->animseq.uh == 0xD)) {
                    u8 mode;

                    mode = 0x10;
                    gCurrentActor = obj;
                    if (pressedButtons & B_BUTTON) {
                        mode = 0xE;
                    } else {
                        obj->animseq.uh = 0;
                    }
                    obj->scriptCmd = script_getScript(1, mode);
                    obj->scriptTimer = 0;
                    gCurrentActor = actor;
                    obj->alertCooldown = 0;
                }
                anim = 999;
            }
        }
        break;
    case 0x27:
        if (act != actor->mv->rt_context_b) {
            actor->mv->rt_context_b = 0x27;
            actor->forced_lod = 6;
            actor->mv->env_suit = 1;
        }
        break;
    case 0x9:
        if (D_800C3A58 != 0) {
            if ((actor->mv->weapon_status == 1) || (actor->mv->weapon_status == 2)) {
                func_15063404(actor);
            } else {
                func_150634E4(actor);
            }
        }
        if (act == actor->mv->rt_context_b) {
            s16 delta;

            delta = (gCurrentPad->stick_x * -100) - actor->mv->unk80;
            if (delta > 600) {
                delta = 600;
            }
            if (delta < -600) {
                delta = -600;
            }
            actor->mv->unk80 += delta;
            delta = ((gCurrentPad->stick_y << 5) - actor->mv->unk82) + 576;
            if (delta > 300) {
                delta = 300;
            }
            if (delta < -300) {
                delta = -300;
            }
            actor->mv->unk82 += delta;
            if (((nested != 0) && ((actor->attach_char != 0) || (actor->stunned != 0) || (actor->shad != 0.0f))) || (actor->inwater != 0)) {
                func_150634E4(actor);
            } else if (nested != 0) {
                speed = 1.399999976f;
                pressedButtons |= B_BUTTON;
                anim = actor_tryStartBAction(actor, 0x7D, 1, 0, pressedButtons);
                if (anim == 999) {
                    func_150634E4(actor);
                }
            } else if (isStunned != 0) {
                clearAction = 0;
            } else {
                actor->nojump = 0xA;
                if (actor->mv->matrix_physics != 0) {
                    u16 prev;
                    u8 lowBits;

                    if ((heldButtons & Z_TRIG) && (actor->speedup < 0.6000000238f)) {

                        prev = actor->mv->unk7E;
                        actor->mv->unk7E = prev + ((gActorStepPercent * 4) / 3);
                        if ((prev ^ actor->mv->unk7E) & 0x100) {
                            gEventParam = 0x13;
                            if (actor->mv->unk7E & 0x200) {
                                gEventParam = 0x14;
                            }
                            event_runAnimCommand();
                            gEventParam = -0xFCF920;
                            event_playSound2Range1800();
                            sound_playActorUntracked((math_random() & 1) + 0x2F5, actor, 0x7D00, 0x1F4, 0x3E8);
                            if (!(math_random() & 7)) {
                                sound_playActorUntrackedDelayed(0x5CD, actor, 0x5DC0, 0x1F4, 0x3E8, 0x3C);
                            }
                        }
                    } else {
                        lowBits = (actor->mv->unk7E << 1) | 1;
                        actor->mv->unk7E = (actor->mv->unk7E & 0xFF00) | lowBits;
                    }
                } else {
                    actor_turnWaist(actor, heldButtons, Z_TRIG | R_TRIG);
                    if ((actor->animseq.uh != 0x11A) && (actor->animseq.uh != 0x7B)) {
                        u8 endEffect;
                        endEffect = 0;
                        if (!(heldButtons & Z_TRIG) && (actor->mv->prevButtons & Z_TRIG)) {
                            endEffect = 1;
                        }
                        if ((actor->mv->gun_before_reload == 0) || (pressedButtons & A_BUTTON)) {
                            anim = 0x11A;
                            if (!(pressedButtons & A_BUTTON)) {
                                endEffect = 1;
                            }
                            actor->mv->gun_before_reload = 0x3C;
                            actor->norun = 0xFF;
                            actor->count5 = 0xFF;
                        } else if ((heldButtons & Z_TRIG) && (actor->norun == 0)) {
                            if (actor->speed != 0.0f) {
                                if (actor->stickspeed >= 20.0f) {
                                    anim = 0x238;
                                    speed = (actor->speed / 22.0f) + 0.400000006f;
                                } else {
                                    anim = 0x20D;
                                }
                                if (!(heldButtons & R_TRIG)) {
                                    actor->mv->pssd_waist_tilt = 0;
                                    actor->mv->turn = 3;
                                } else {
                                    actor->mv->turn = 0;
                                }
                                if (gIsMultiplayer == 0) {
                                    actor->speedPercent = 0x37;
                                }
                            } else {
                                anim = 0x7C;
                            }
                            actor->count5 = 8;
                            if ((actor->animseq.uh != 0x7C) && (actor->animseq.uh != 0x20D) && (actor->animseq.uh != 0x238)) {
                                particle_detachObjEmitters(actor);
                                sound_addDynamicSound(0x2CF, actor->xpos, actor->ypos, actor->zpos, 0x7FFF, 0x3E8, 0x1F4, sound_followOwnerAnimsCallback, actor, (s32)D_800996C0, 0, 0); // matching: the (s32) casts on these tables steer IDO; a plain pointer reschedules
                                if (((actor->character_type == 0x3BU) || (actor->character_type == 0x80)) && !(math_random() & 3)) {
                                    s32 sound;

                                    if (actor->character_type == 0x3B) {
                                        sound = sound_pickVariant(0x543, 4);
                                    } else {
                                        sound = sound_pickVariant(0x57C, 6);
                                    }
                                    sound_addDynamicSound(sound, actor->xpos, actor->ypos, actor->zpos, 0x7FFF, 0x3E8, 0x1F4, sound_followOwnerAnimsCallback, actor, (s32)D_800996C0, 0, 0);
                                }
                            }
                        } else if ((actor->taskint[2] != 0) && (level == LEVEL_W5_BANK)) {
                            if ((pressedButtons & B_BUTTON) && (actor->taskint[2] == 1) && (actor->shad == 0.0f)) {
                                blend = 16.0f;
                                speed = 0.6999999881f;
                                anim = 0x20F;
                                actor->angle = 0;
                                if (actor->xpos > 0.0f) {
                                    actor->angle = 0x8000;
                                    anim = 0x20E;
                                }
                                actor->speed = 0.0f;
                                actor->norun = 0xFF;
                                actor->count5 = 0xFF;
                                actor->count3 = 1;
                            }
                        } else {
                            speed = 1.399999976f;
                            anim = actor_tryStartBAction(actor, 0x7D, 1, 0, pressedButtons);
                        }
                        if (endEffect != 0) {
                            particle_attachTwinEmitters(actor);
                        }
                    }
                }
            }
        } else {
            speed = 1.399999976f;
            anim = actor_tryStartBAction(actor, 0x7B, 1, act, pressedButtons);
            if (anim != 999) {
                actor->mv->gun_before_reload = 0x3C;
            }
        }
        break;
    case 0x38:
        if (act == actor->mv->rt_context_b) {
            if ((nested != 0) && (actor->attach_char != 0)) {
                collision_removePartSet(actor, 0x7B);
                actor->mv->weapon_status = 0;
            } else if (isStunned != 0) {
                clearAction = 0;
            } else {
                u8 endEffect;
                u16 pad[2];  // matching: unused stack slots

                actor->nojump = 0xA;
                actor_turnWaist(actor, heldButtons, Z_TRIG | R_TRIG);
                if (actor->animseq.uh != 0x1DD) {
                    endEffect = 0;
                    if (!(heldButtons & Z_TRIG) && (actor->mv->prevButtons & Z_TRIG)) {
                        endEffect = 1;
                    }
                    if (heldButtons & Z_TRIG) {
                        if (actor->speed != 0.0f) {
                            if (actor->stickspeed >= 20.0f) {
                                anim = 0x254;
                                speed = (actor->speed / 22.0f) + 0.400000006f;
                            } else {
                                anim = 0x253;
                            }
                            if (!(heldButtons & R_TRIG)) {
                                actor->mv->turn = 3;
                                actor->mv->pssd_waist_tilt = 0;
                            }
                        } else {
                            anim = 0x1E0;
                        }
                        blend = 0.0f;
                        actor->count5 = 8;
                        if ((actor->animseq.uh != 0x1E0) && (actor->animseq.uh != 0x253) && (actor->animseq.uh != 0x254)) {
                            particle_detachObjEmitters(actor);
                            sound_addDynamicSound(0x2CF, actor->xpos, actor->ypos, actor->zpos, 0x7FFF, 0x3E8, 0x1F4, sound_followOwnerAnimsCallback, actor, (s32)D_800996C8, 0, 0);
                            if (((actor->character_type == 0x3BU) || (actor->character_type == 0x80)) && !(math_random() & 3)) {
                                s32 sound;

                                if (actor->character_type == 0x3B) {
                                    sound = sound_pickVariant(0x543, 4);
                                } else {
                                    sound = sound_pickVariant(0x57C, 6);
                                }
                                sound_addDynamicSound(sound, actor->xpos, actor->ypos, actor->zpos, 0x7FFF, 0x3E8, 0x1F4, sound_followOwnerAnimsCallback, actor, (s32)D_800996C8, 0, 0);
                            }
                        }
                        actor->mv->gunRecoilVel += gFrameTicks * 0xA;
                        if (actor->mv->gunRecoil > 0xAF) {
                            actor->mv->gunRecoil = 0xAF;
                        }
                        actor->mv->gunRecoil += actor->mv->gunRecoilVel * gFrameTicks;
                        if (actor->mv->gunRecoil > 0x1E00) {
                            actor->mv->gunRecoil = 0x1E00;
                        }
                    } else {
                        speed = 1.399999976f;
                        anim = actor_tryStartBAction(actor, 0x1DE, 1, 0, pressedButtons);
                        actor->mv->gunRecoil = 0;
                        actor->mv->gunRecoilVel = 0;
                    }
                    if (endEffect != 0) {
                        particle_attachPointEmitter(actor, 5, 0x78, 0xFF, 1);
                    }
                }
            }
        } else {
            speed = 1.399999976f;
            anim = actor_tryStartBAction(actor, 0x1DD, 1, act, pressedButtons);
        }
        break;
    case 0x39:
        if (act == actor->mv->rt_context_b) {
            if ((nested != 0) && (actor->attach_char != 0)) {
                collision_removePartSet(actor, 0x82);
                actor->mv->weapon_status = 0;
            } else if (isStunned != 0) {
                clearAction = 0;
            } else {
                actor->nojump = 0xA;
                actor_turnWaist(actor, heldButtons, Z_TRIG | R_TRIG);
                if (actor->animseq.uh != 0x1EE) {
                    if (actor->animseq.uh == 0x21E) {
                        actor->norun = 0xFF;
                    }
                    if ((actor->mv->gun_before_reload == 0) || ((pressedButtons & A_BUTTON) && (actor->animseq.uh != 0x21E))) {
                        if (actor->shad == 0.0f) {
                            actor->mv->gun_before_reload = 0x1E;
                            actor->norun = 0xFF;
                            actor->count5 = 0xFF;
                            anim = 0x21E;
                        }
                    } else if ((pressedButtons & Z_TRIG) && (actor->animseq.uh != 0x1EE) && (actor->animseq.uh != 0x21E)) {
                        if (actor->speed == 0.0f) {
                            anim = 0x1EB;
                        } else {
                            anim = 999;
                            gEventParam = 0x53;
                            event_runAnimCommand();
                        }
                        sound_stopByCallback(effect_nullSoundCallback, actor, actor->unique_id);
                        sound_addDynamicSound((math_random() & 1) + 0x2F3, actor->xpos, actor->ypos, actor->zpos, 0x7D00, 0x5DC, 0x1F4, effect_nullSoundCallback, actor, actor->unique_id, 0, 0);
                        if (actor->CameraPtr != NULL) {
                            sound_playActorSound1Delayed((math_random() % 3U) + 0x2EB, actor, 0x7D00, 0x64, 0x78, 0x1E);
                        }
                    } else {
                        speed = 1.399999976f;
                        anim = actor_tryStartBAction(actor, 0x1EF, 1, 0, pressedButtons);
                    }
                }
            }
        } else {
            speed = 1.399999976f;
            anim = actor_tryStartBAction(actor, 0x1EE, 1, act, pressedButtons);
            if (anim != 999) {
                actor->mv->gun_before_reload = 0x1E;
            }
        }
        break;
    case 0x37:
        if (act == actor->mv->rt_context_b) {
            if ((nested != 0) && (actor->attach_char != 0)) {
                collision_removePartSet(actor, 0x85);
                actor->mv->weapon_status = 0;
            } else if (isStunned != 0) {
                clearAction = 0;
            } else {
                actor->nojump = 0xA;
                actor_turnWaist(actor, heldButtons, Z_TRIG | R_TRIG);
                if ((pressedButtons & Z_TRIG) && (actor->animseq.uh != 0x1F7) && (actor->mv->ReloadTimer == 0)) {
                    if (math_random() & 3) {
                        anim = 0x1F4;
                    } else {
                        anim = 0x235;
                    }
                    blend = 0.0f;
                    actor->mv->ReloadTimer = 0x78;
                    actor->norun = 0xFF;
                    actor->count5 = 0xFF;
                } else if (pressedButtons & B_BUTTON) {
                    speed = 1.399999976f;
                    anim = actor_tryStartBAction(actor, 0x1F8, 1, 0, pressedButtons);
                }
            }
        } else {
            speed = 1.399999976f;
            anim = actor_tryStartBAction(actor, 0x1F7, 1, act, pressedButtons);
        }
        break;
    case 0xB:
        if (actor->animseq.uh == 0x68) {
            if ((actor->anim->frame[0] < 68.0f) && (actor->targetIndex != 0)) {
                gWaistTiltTarget = (actor->langle - actor_getAngleToLinked(actor)) - 0x2000;
            }
            anim = 999;
            gWaistTiltDamping = 4.0f;
        } else if ((pressedButtons & B_BUTTON) && (height <= 0.0f)) {
            speed = 1.200000048f;
            anim = 0x68;
            blend = 6.0f;
            actor->norun = 0xFF;
            actor->count5 = 0xFF;
        }
        break;
    case 0xC:
    case 0x16:
        anim = 999;
        if ((nested != 0) && (act == 0x16)) {
            if (act == actor->mv->rt_context_b) {
                s32 cancel;

                cancel = 1;
                if ((s32)actor->shad == 0) {
                    pressedButtons |= B_BUTTON;
                    anim = actor_tryStartBAction(actor, 0x78, 1, 0, pressedButtons);
                    if (anim != 999) {
                        cancel = 0;
                    }
                }
                if (cancel != 0) {
                    collision_removePartSet(actor, 0x24);
                    actor->mv->weapon_status = 0;
                    actor->count5 = 0;
                    actor->norun = 0;
                }
            }
        } else if (act == actor->mv->rt_context_b) {
            actor->nojump = 0xA;
            if ((nested != 0) && ((actor->attach_char != 0) || ((s32)actor->shad != 0))) {
                collision_removePartSet(actor, 0x24);
                actor->mv->weapon_status = 0;
            } else if (isStunned != 0) {
                if ((act == 0x16) || ((act == 0xC) && (actor->shad == 0.0f) && (actor->energy != 0))) {
                    clearAction = 0;
                } else {
                    collision_removePartSet(actor, 0x24);
                    actor->mv->weapon_status = 0;
                }
            } else if ((pressedButtons & B_BUTTON) || (nested != 0)) {
                pressedButtons |= B_BUTTON;
                anim = actor_tryStartBAction(actor, 0x78, 1, 0, pressedButtons);
                if ((anim == 999) && (nested != 0)) {
                    collision_removePartSet(actor, 0x24);
                    actor->mv->weapon_status = 0;
                    actor->count5 = 0;
                }
            } else if ((pressedButtons & Z_TRIG) && (actor->animseq.uh != 0x76) && (actor->mv->ReloadTimer == 0)) {
                if (actor->mv->unk1B2 != 0) {
                    anim = 0x2C7;
                } else if ((act == 0xC) || (level == LEVEL_WMP_BEACH) || (gIsMultiplayer == 0)) {
                    anim = 0x207;
                } else {
                    anim = 0x77;
                }
                blend = 0.0f;
                actor->norun = 0xFF;
                actor->count5 = 0xFF;
                if (actor->mv->SniperState != 0) {
                    if (actor->CameraPtr != NULL) {
                        camera_startShake(actor->CameraPtr, 0);
                    }
                }
                actor->mv->ReloadTimer = 0x78;
            } else if ((actor->mv->SniperState != 0) && (actor->CameraPtr != NULL) && (actor->character_type == 0x80) && (sound_isActorSound1Playing(actor) == 0) && !(math_random() & 0x7F)) {
                actor_playSound(sound_pickVariant(0x568, 4), actor);
            }
        } else {
            anim = actor_tryStartBAction(actor, 0x76, 1, act, pressedButtons);
        }
        if ((act == 0xC) && ((act == actor->mv->rt_context_b) || (actor->mv->weapon_status != 0))) {
            actor->speed = 0.0f;
            if ((s32)actor->shad != 0) {
                collision_removePartSet(actor, 0x24);
                actor->mv->weapon_status = 0;
                actor->mv->rt_context_b = 0;
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
            if (isStunned != 0) {
                clearAction = 0;
            } else if (act == actor->mv->rt_context_b) {
                if (actor->mv->weapon_status == 2) {
                    if (pressedButtons & Z_TRIG) {
                        anim = actor_pickRandomAnim(actor, 0x113, 0x113, 0x119);
                        actor->norun = 0xFF;
                    } else {
                        anim = actor_tryStartBAction(actor, animLoop, 1, 0, pressedButtons);
                    }
                }
            } else {
                anim = actor_tryStartBAction(actor, animStart, 1, act, pressedButtons);
            }
        }
        break;
    case 0xE:
        actor->nojump = 0x14;
        anim = 999;
        break;
    case 0xF:
        anim = 999;
        if ((act == actor->mv->rt_context_b) && ((isStunned != 0) || (actor->attach_char != 0) || (nested != 0))) {
            collision_removePartSet(actor, 0x3A);
            actor->mv->weapon_status = 0;
            actor->norun = 0;
            actor->count5 = 0;
        } else if ((pressedButtons & B_BUTTON) && (height <= 0.0f)) {
            anim = 0xB4;
            if (act == actor->mv->rt_context_b) {
                anim = 0xB6;
            }
            actor->norun = 0xFF;
            actor->count5 = 0xFF;
            actor->mv->rt_context_b = act;
        }
        if (act == actor->mv->rt_context_b) {
            actor->nojump = 0xA;
            actor->speed = 0.0f;
        }
        break;
    case 0x10:
        speed = 1.399999976f;
        if (act == actor->mv->rt_context_b) {
            gWaistTiltTarget = gCurrentPad->stick_x * 0xC8;
            actor->nojump = 0xA;
            if ((heldButtons & Z_TRIG) && (anim = 0xC1, (actor->animseq.uh != 0xBE))) {
                speed = 1.0f;
            } else {
                anim = actor_tryStartBAction(actor, 0xC0, 1, 0, pressedButtons);
            }
        } else {
            anim = actor_tryStartBAction(actor, 0xBE, 1, act, pressedButtons);
        }
        break;
    case 0x15:
        if (act == actor->mv->rt_context_b) {
            if ((nested != 0) && (actor->attach_char != 0)) {
                collision_removePartSet(actor, 0x4B);
                actor->mv->weapon_status = 0;
            } else if (isStunned != 0) {
                clearAction = 0;
            } else {
                actor->nojump = 0xA;
                actor_turnWaist(actor, heldButtons, Z_TRIG | R_TRIG);
                if ((heldButtons & Z_TRIG) && (actor->animseq.uh != 0x168)) {
                    if (actor->speed != 0.0f) {
                        actor->speedPercent = 0x50;
                        actor->mv->turn = 3;
                        actor->mv->pssd_waist_tilt = 0;
                        anim = 0x22B;
                        speed = (actor->speed * 0.0625f) + 0.6000000238f;
                        if (actor->stickspeed >= 20.0f) {
                            speed = (actor->speed / 22.0f) + 0.400000006f;
                        }
                    } else {
                        anim = 0xE7;
                    }
                    actor->norun = 0;
                    actor->count5 = 8;
                    if ((actor->animseq.uh != 0xE7) && (actor->animseq.uh != 0x22B)) {
                        s32 anims[2];

                        anims[0] = 0xE7;
                        anims[1] = 0x22B;
                        actor->mv->shotsfired++;
                        actor_runHitHandler(actor, 9, anims);
                    }
                } else {
                    anim = actor_tryStartBAction(actor, 0xE4, 1, 0, pressedButtons);
                }
            }
        } else {
            anim = actor_tryStartBAction(actor, 0x168, 1, act, pressedButtons);
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
            if (act == actor->mv->rt_context_b) {
                if (isStunned != 0) {
                    clearAction = 0;
                } else if ((actor->mv->SniperState != 0) && (actor->mv->weapon_status == 2)) {
                    actor->nojump = 0xA;
                    actor->norun = 0;
                    actor->count5 = 0xA;
                    if ((gCurrentPad->button & Z_TRIG) || (actor->mv->prevButtons & Z_TRIG)) {
                        actor->mv->deferredActions |= 1;
                        gActionsPending = 1;
                    }
                    if (animStart != actor->animseq.uh) {
                        if (!(gCurrentPad->button & Z_TRIG) && (actor->mv->prevButtons & Z_TRIG) && (actor->mv->ReloadTimer == 0)) {
                            actor->mv->deferredActions |= 2;
                            gActionsPending = 1;
                            blend = 0.0f;
                            actor->norun = 0xFF;
                            actor->count5 = 0xFF;
                            anim = 0x10C;
                            actor->mv->ReloadTimer = 0x3C;
                        } else if (actor->animseq.uh != 0x10C) {
                            anim = 0x10A;
                        }
                    }
                } else {
                    actor->nojump = 0x14;
                    if ((pressedButtons & Z_TRIG) && (animStart != actor->animseq.uh) && (actor->mv->ReloadTimer == 0)) {
                        anim = animAlt;
                        blend = 0.0f;
                        actor->norun = 0xFF;
                        actor->count5 = 0xFF;
                        actor->mv->ReloadTimer = 0x3C;
                    } else {
                        anim = actor_tryStartBAction(actor, 0xC6, 1, 0, pressedButtons);
                    }
                }
            } else {
                anim = actor_tryStartBAction(actor, animStart, 1, act, pressedButtons);
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
        actor->mv->rt_context_b = act;
        actor->nojump = 0x14;
        break;
    case 0x28:
        if (nested != 0) {
            actor->mv->rt_context_b = 0;
        } else if ((height <= 0.0f) && (pressedButtons & B_BUTTON)) {
            if (act != actor->mv->rt_context_b) {
                actor->mv->rt_context_b = act;
            }
        }
        break;
    case 0x3C:
        {
            u16 before;
            u16 after;

            before = gCameraYawAngle - actor->langle;
            anim = 999;
            actor->intelhitroute = 0x2F;
            if (actor->lifting != 0) {
                s16 old;
                f32 mag;
                Actor *obj;

                old = actor->mv->unk18;
                actor->count5 = 0xFF;
                actor->norun = 0xFF;
                if (actor->mv->unk18 >= 0) {
                    actor->mv->unk18 -= 0x14;
                } else {
                    actor->mv->unk18 += 0x14;
                }
                if ((old ^ actor->mv->unk18) & 0x8000) {
                    actor->mv->unk18 = 0;
                }
                if (actor->stickspeed > 10.0f) {
                    s16 diff;

                    diff = actor->mv->press_angle - actor->mv->unk7E;
                    if ((diff < 0x2000) && (diff > -0x2000) && ((diff > 0x100) || (diff < -0x100))) {
                        if (diff & 0x8000) {
                            actor->mv->unk18 -= 0x28;
                        } else {
                            actor->mv->unk18 += 0x28;
                        }
                        if (actor->mv->unk18 > 0x1000) {
                            actor->mv->unk18 = 0x1000;
                        }
                        if (actor->mv->unk18 < -0x1000) {
                            actor->mv->unk18 = -0x1000;
                        }
                    }
                }
                actor->angle = actor->langle = actor->langle + ((actor->mv->unk18 * gActorStepPercent) / 100);
                if ((actor->mv->unk18 > 0x200) || (actor->mv->unk18 < -0x200)) {
                    after = gCameraYawAngle - actor->langle;
                    if ((((before < 0x1000) || (before > 0xF000)) && (after >= 0x1000) && (after < 0x7000) && (actor->mv->unk18 < 0)) ||
                        ((before > 0x7000) && (after <= 0x7000) && (actor->mv->unk18 > 0))) {
                        gEventParam = -0xFEFA4A;
                        event_playSound2Range1800();
                    }
                }
                actor->mv->unk7E = actor->mv->press_angle;
                anim = 0x288;
                mag = fabsf(actor->mv->unk18);
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
                    actor->lifting = 0;
                    actor->count5 = 0;
                    actor->norun = 0xA;
                }
            } else if ((pressedButtons & B_BUTTON) && (height <= 0.0f) && (actor->nojump == 0)) {
                if (actor->animseq.uh != 0x27D) {
                    if ((actor->norun == 0) || (actor->animseq.uh < 0x27A) || (actor->animseq.uh >= 0x27E)) {
                        anim = 0x27A;
                    } else if ((actor->unk103 != 0) || (actor->ivehitit != 0)) {
                        anim = actor->animseq.uh + 1;
                    }
                    if (anim != 999) {
                        actor->ivehitit = 0;
                        actor->count5 = 0xFF;
                        actor->norun = 0xFF;
                    }
                }
            }
        }
        break;
    case 0x21:
        if ((nested != 0) && (actor->attach_char != 0)) {
            collision_removePartSet(actor, 0x58);
            actor->mv->weapon_status = 0;
        } else if (isStunned != 0) {
            clearAction = 0;
        } else if (height <= 0.0f) {
            if (act == actor->mv->rt_context_b) {
                if (actor->mv->weapon_status == 2) {
                    if (pressedButtons & Z_TRIG) {
                        anim = actor_pickRandomAnim(actor, 0x129, 0x12A, 0x128);
                        speed = 1.299999952f;
                        actor->norun = 0xFF;
                    } else {
                        anim = actor_tryStartBAction(actor, 0x121, 1, 0, pressedButtons);
                        speed = 1.399999976f;
                    }
                }
            } else {
                anim = actor_tryStartBAction(actor, 0x120, 1, act, pressedButtons);
                speed = 1.399999976f;
            }
        }
        break;
    case 0x22:
        speed = 1.399999976f;
        if ((nested != 0) && (actor->attach_char != 0)) {
            collision_removePartSet(actor, 0x56);
            actor->mv->weapon_status = 0;
        } else if (isStunned != 0) {
            clearAction = 0;
        } else if ((act == actor->mv->rt_context_b) && (height <= 0.0f)) {
            if (actor->mv->weapon_status == 2) {
                if (pressedButtons & Z_TRIG) {
                    anim = actor_pickRandomAnim(actor, 0x15F, 0x15F, 0x15F);
                    speed = 1.299999952f;
                    actor->norun = 0xFF;
                } else {
                    anim = actor_tryStartBAction(actor, 0x15E, 1, 0, pressedButtons);
                }
            }
        } else {
            anim = actor_tryStartBAction(actor, 0x15D, 1, act, pressedButtons);
        }
        break;
    case 0x24:
        if (nested != 0) {
            if (act == actor->mv->rt_context_b) {
                s32 cancel;

                cancel = 1;
                if (((s32)actor->shad == 0) && (actor->mv->weapon_status == 2)) {
                    pressedButtons |= B_BUTTON;
                    anim = actor_tryStartBAction(actor, 0x155, 1, 0, pressedButtons);
                    if (anim != 999) {
                        cancel = 0;
                    }
                }
                if (cancel != 0) {
                    collision_removePartSet(actor, 0x63);
                    actor->mv->weapon_status = 0;
                    actor->count5 = 0;
                    actor->norun = 0;
                }
            }
        } else if (act == actor->mv->rt_context_b) {
            if ((nested != 0) && (actor->attach_char != 0)) {
                collision_removePartSet(actor, 0x63);
                actor->mv->weapon_status = 0;
            } else if (isStunned != 0) {
                clearAction = 0;
            } else if ((actor->mv->SniperState != 0) && (actor->mv->weapon_status == 2) && (nested == 0)) {
                actor->nojump = 0xA;
                actor->norun = 0;
                actor->count5 = 0xA;
                if ((actor->mv->ReloadTimer == 0) && ((gCurrentPad->button & Z_TRIG) || (actor->mv->prevButtons & Z_TRIG))) {
                    actor->mv->deferredActions |= 1;
                    gActionsPending = 1;
                }
                if (actor->animseq.uh != 0x152) {
                    if (!(gCurrentPad->button & Z_TRIG) && (actor->mv->prevButtons & Z_TRIG) && (actor->animseq.uh == 0x157) && (actor->mv->ReloadTimer == 0)) {
                        actor->mv->deferredActions |= 2;
                        gActionsPending = 1;
                        blend = 0.0f;
                        actor->norun = 0xFF;
                        actor->count5 = 0xFF;
                        actor->mv->ReloadTimer = 0x78;
                        anim = 0x156;
                        if ((actor->CameraPtr != NULL) && (((actor->floorMaterial & 0x1F) == 5) || ((actor->floorMaterial & 0x1F) == 3) ||
                                                       ((actor->floorMaterial & 0x1F) == 6) || ((actor->floorMaterial & 0x1F) == 9))) {
                            sound_playActorSound1Delayed((math_random() % 3U) + 0x2EB, actor, 0x7D00, 0x64, 0x78, 0x6E);
                        }
                    } else if (actor->animseq.uh != 0x156) {
                        anim = 0x157;
                    }
                }
            } else {
                actor->nojump = 0x14;
                if (nested != 0) {
                    actor->norun = 0xFF;
                    actor->count5 = 0xFF;
                    anim = 0x155;
                    if ((s32)actor->shad != 0) {
                        collision_removePartSet(actor, 0x63);
                        actor->mv->weapon_status = 0;
                    }
                } else if ((pressedButtons & Z_TRIG) && (actor->animseq.uh != 0x152) && (actor->mv->ReloadTimer == 0)) {
                    anim = 0x154;
                    blend = 0.0f;
                    actor->norun = 0xFF;
                    actor->count5 = 0xFF;
                    actor->mv->ReloadTimer = 0x78;
                } else {
                    anim = actor_tryStartBAction(actor, 0x155, 1, 0, pressedButtons);
                }
            }
        } else {
            anim = actor_tryStartBAction(actor, 0x152, 1, act, pressedButtons);
        }
        break;
    case 0x3B:
        {
            s32 held;

            if (D_800C3A58 != 0) {
                if (act == actor->mv->rt_context_b) {
                    collision_createPartSet(actor, 0x89, 1.0f, 0);
                    actor->mv->weapon_status = 2;
                } else {
                    collision_removePartSet(actor, 0x89);
                    actor->mv->weapon_status = 0;
                }
            }
            if (((act == actor->mv->rt_context_b) || (actor->mv->weapon_status != 0)) && (actor->inwater != 0)) {
                actor->mv->rt_context_b = 0;
                actor->norun = 0;
                actor->count5 = 0;
                collision_removePartSet(actor, 0x89);
                actor->mv->weapon_status = 0;
            } else if (act == actor->mv->rt_context_b) {
                if ((nested != 0) && ((actor->attach_char != 0) || (actor->stunned != 0) || (actor->lifting != 0))) {
                    collision_removePartSet(actor, 0x89);
                    actor->mv->weapon_status = 0;
                    actor->mv->rt_context_b = 0;
                } else if (nested != 0) {
                    pressedButtons |= B_BUTTON;
                    anim = actor_tryStartBAction(actor, 0x226, 1, 0, pressedButtons);
                    if (anim == 999) {
                        collision_removePartSet(actor, 0x89);
                        actor->mv->weapon_status = 0;
                        actor->mv->rt_context_b = 0;
                        actor->count5 = 0;
                    }
                } else if (isStunned != 0) {
                    clearAction = 0;
                } else {
                    actor->nojump = 0x14;
                    actor_turnWaist(actor, heldButtons, Z_TRIG | R_TRIG);
                    if ((actor->mv->ReloadTimer == 0) && ((gActorHeldButtons & Z_TRIG) || (actor->mv->prevButtons & Z_TRIG))) {
                        if (actor->mv->AimState == 2) {
                            actor->mv->deferredActions |= 4;
                            gActionsPending = 1;
                        } else {
                            actor->mv->deferredActions |= 1;
                            gActionsPending = 1;
                        }
                    }
                    if (nested != 0) {
                        actor->norun = 0xFF;
                        actor->count5 = 0xFF;
                        anim = 0x226;
                        if ((s32)actor->shad != 0) {
                            collision_removePartSet(actor, 0x89);
                            actor->mv->weapon_status = 0;
                        }
                    } else {
                        held = gActorHeldButtons & R_TRIG;
                        if ((((held != 0) && !(gActorHeldButtons & Z_TRIG) && (actor->mv->prevButtons & Z_TRIG)) ||
                                    ((held == 0) && (gActorNewButtons & Z_TRIG))) &&
                                   (actor->animseq.uh != 0x225) && (actor->animseq.uh != 0x222) && (actor->animseq.uh != 0x236) &&
                                   (actor->animseq.uh != 0x227) && (actor->animseq.uh != 0x237) && (actor->mv->ReloadTimer == 0)) {
                            if (gCurrentPad->button & R_TRIG) {
                                if (math_random() & 3) {
                                    anim = 0x227;
                                } else {
                                    anim = 0x237;
                                }
                                actor->mv->deferredActions |= 2;
                                gActionsPending = 1;
                            } else {
                                if (math_random() & 3) {
                                    anim = 0x222;
                                } else {
                                    anim = 0x236;
                                }
                            }
                            blend = 0.0f;
                            actor->norun = 0xFF;
                            actor->count5 = 0xFF;
                            actor->mv->ReloadTimer = 0x3C;
                        } else if ((held != 0) && (gActorHeldButtons & Z_TRIG) && (actor->animseq.uh != 0x225)) {
                            actor->norun = 0;
                            actor->nojump = 8;
                            actor->count5 = 8;
                            if ((actor->animseq.uh != 0x222) && (actor->animseq.uh != 0x236) && (actor->animseq.uh != 0x227) && (actor->animseq.uh != 0x237)) {
                                if ((actor->stickspeed >= 1.0f) || (actor->mv->door_walkf_time > 0)) {
                                    anim = 0x223;
                                    if (actor->stickspeed >= 20.0f) {
                                        anim = 0x31B;
                                        speed = (actor->speed / 22.0f) + 0.400000006f;
                                    }
                                } else {
                                    anim = 0x221;
                                }
                            }
                        } else {
                            anim = actor_tryStartBAction(actor, 0x226, 1, 0, pressedButtons);
                        }
                    }
                }
            } else {
                anim = actor_tryStartBAction(actor, 0x225, 1, act, pressedButtons);
            }
        }
        break;
    case 0x12:
        if (((act == actor->mv->rt_context_b) || (actor->mv->weapon_status != 0)) && (actor->inwater != 0)) {
            actor->mv->rt_context_b = 0;
            actor->norun = 0;
            actor->count5 = 0;
            actor->mv->weapon_status = 0;
        } else if (act == actor->mv->rt_context_b) {
            actor->speed = 0.0f;
            if ((nested != 0) && (actor->attach_char != 0)) {
                actor->mv->weapon_status = 0;
            } else {
                actor->nojump = 0x14;
                if ((gCurrentPad->button & Z_TRIG) || (actor->mv->prevButtons & Z_TRIG)) {
                    if (actor->mv->AimState == 2) {
                        actor->mv->deferredActions |= 8;
                        gActionsPending = 1;
                    } else {
                        actor->mv->deferredActions |= 1;
                        gActionsPending = 1;
                    }
                }
                if (nested != 0) {
                    actor->norun = 0xFF;
                    actor->count5 = 0xFF;
                    anim = 0xC6;
                    if ((s32)actor->shad != 0) {
                        actor->mv->weapon_status = 0;
                    }
                } else if (!(gCurrentPad->button & Z_TRIG) && (actor->mv->prevButtons & Z_TRIG) && (actor->animseq.uh != 0xC4) && (actor->mv->ReloadTimer == 0)) {
                    anim = 0xC7;
                    blend = 0.0f;
                    actor->norun = 0xFF;
                    actor->count5 = 0xFF;
                    actor->mv->ReloadTimer = 0x3C;
                } else if ((gCurrentPad->button & Z_TRIG) && (actor->animseq.uh != 0xC4)) {
                    actor->norun = 0;
                    actor->nojump = 8;
                    actor->count5 = 8;
                    if (actor->animseq.uh != 0xC7) {
                        anim = 0x10A;
                    }
                } else {
                    anim = actor_tryStartBAction(actor, 0xC6, 1, 0, pressedButtons);
                }
            }
        } else {
            anim = actor_tryStartBAction(actor, 0xC4, 1, act, pressedButtons);
            if (anim != 999) {
                actor->speed = 0.0f;
            }
        }
        break;
    case 0x23:
        if (act == actor->mv->rt_context_b) {
            if ((nested != 0) && (actor->attach_char != 0)) {
                collision_removePartSet(actor, 0x16);
                actor->mv->weapon_status = 0;
            } else if (isStunned != 0) {
                clearAction = 0;
            } else if ((actor->mv->SniperState != 0) && (actor->mv->weapon_status == 2)) {
                actor->nojump = 0xA;
                actor->norun = 0;
                actor->count5 = 0xA;
                if ((actor->mv->ReloadTimer == 0) && ((gCurrentPad->button & Z_TRIG) || (actor->mv->prevButtons & Z_TRIG))) {
                    actor->mv->deferredActions |= 1;
                    gActionsPending = 1;
                }
                if (actor->animseq.uh != 0x163) {
                    if ((actor->mv->shotCount >= 6) || ((pressedButtons & A_BUTTON) && (actor->animseq.uh != 0x26E))) {
                        if (actor->shad == 0.0f) {
                            actor->norun = 0xFF;
                            actor->count5 = 0xFF;
                            actor->mv->ReloadTimer = 0xB4;
                            anim = 0x26E;
                            actor->mv->shotCount = 0;
                        }
                    } else if (!(gCurrentPad->button & Z_TRIG) && (actor->mv->prevButtons & Z_TRIG) && (actor->animseq.uh == 0x165) && (actor->mv->ReloadTimer == 0)) {
                        actor->mv->deferredActions |= 2;
                        gActionsPending = 1;
                        blend = 0.0f;
                        actor->norun = 0xFF;
                        actor->count5 = 0xFF;
                        anim = 0x167;
                        actor->mv->shotCount++;
                        actor->mv->ReloadTimer = 0x1E;
                    } else if (actor->animseq.uh != 0x167) {
                        anim = 0x165;
                    }
                }
            } else {
                actor->nojump = 0x14;
                if ((actor->animseq.uh != 0x163) && ((actor->mv->shotCount >= 6) || ((pressedButtons & A_BUTTON) && (actor->animseq.uh != 0x26E)))) {
                    if (actor->shad == 0.0f) {
                        actor->norun = 0xFF;
                        actor->count5 = 0xFF;
                        actor->mv->ReloadTimer = 0xB4;
                        anim = 0x26E;
                        actor->mv->shotCount = 0;
                    }
                } else if ((pressedButtons & Z_TRIG) && (actor->animseq.uh != 0x163) && (actor->animseq.uh != 0x166) && (actor->mv->ReloadTimer == 0)) {
                    blend = 0.0f;
                    actor->norun = 0xFF;
                    actor->count5 = 0xFF;
                    anim = 0x166;
                    actor->mv->shotCount++;
                    actor->mv->ReloadTimer = 0x1E;
                } else {
                    anim = actor_tryStartBAction(actor, 0x164, 1, 0, pressedButtons);
                    if (anim == 0x163) {
                        actor->mv->shotCount = 0;
                    }
                }
            }
        } else {
            anim = actor_tryStartBAction(actor, 0x163, 1, act, pressedButtons);
        }
        break;
    case 0x17:
        anim = 999;
        if ((pressedButtons & B_BUTTON) && (height <= 0.0f)) {
            s32 pad;  // matching: unused stack slot
            s32 ok;
            f32 angle;

            ok = 1;
            if (actor->mv->cont_zone_ind != 0) {
                angle = math_angleDiff(zones[actor->mv->cont_zone_ind].yaw, actor->theta);
                if ((zones[actor->mv->cont_zone_ind].data1 != 0) && ((angle < -90.0f) || (angle > 90.0f))) {
                    ok = 0;
                }
            }
            if (ok != 0) {
                actor->norun = 0xFF;
                actor->count5 = 0xFF;
                actor->mv->rt_context_b = 0x17;
                anim = 0x8E;
                actor->angle = (u32)(zones[actor->mv->cont_zone_ind].yaw * 182.0444489f) - 0x4000;
            }
        }
        break;
    case 0x1A:
        handled = 1;
        anim = 999;
        if (act != actor->mv->rt_context_b) {
            if ((pressedButtons & B_BUTTON) && (height <= 0.0f)) {
                actor->norun = 0xFF;
                actor->nojump = 0xFF;
                actor->count5 = 0xFF;
                actor->speed = 0.0f;
                anim = 0x12C;
                actor->mv->rt_context_b = act;
                text_openContextText(0);
            }
        } else if (isStunned != 0) {
            text_closeContextText();
            actor->mv->rt_context_b = 0;
        } else if (text_isContextTextClosed() != 0) {
            if (gContextTextId == 0x42) {
                actor->mv->rt_context_b = 0;
                anim = 0x358;
                gSeenTextBits[gContextTextId >> 3] |= 1 << (gContextTextId & 7);
            } else {
                anim = 0x12E;
            }
            actor->norun = 0xFF;
            actor->count5 = 0xFF;
        } else if ((actor->animseq.uh != 0x12C) && (gButtonsPressed[dc_glbl_CharNum] & 0x8000) && (!(gContextTextBoxConfig.flags & 0x20) || (mp3_getPlayState() == 0))) {
            anim = 0x142;
        }
        break;
    case 0x2A:
        handled = 1;
        anim = 999;
        if (act != actor->mv->rt_context_b) {
            if ((height <= 0.0f) && (actor->slideState == 0) && (actor->nojump == 0)) {
                actor->norun = 0xFF;
                actor->nojump = 0xFF;
                actor->count5 = 0xFF;
                actor->speed = 0.0f;
                actor->mv->rt_context_b = act;
                anim = 0x26F;
                actor->headEffect = 6;
                actor->headEffectTimer = 0x64;
                actor->headEffectState = 0;
                actor->headEffectOffset[0][0] = 0;
                text_openContextText(1);
            }
        } else if (isStunned != 0) {
            text_closeContextText();
            actor->mv->rt_context_b = 0;
        } else if (text_isContextTextClosed() != 0) {
            actor->mv->rt_context_b = 0;
            actor->norun = 0xFF;
            actor->count5 = 0;
            face_revertExpression(actor);
        } else {
            s32 sounds[2] = { 0x39, 0x35 };

            actor->headEffectTimer = 0x64;
            if (actor->headEffectOffset[0][0] == 0) {
                actor->headEffectState = actor->headEffectState + gFrameTicks;
                if (((math_random() % 120U) + 0xB4) < actor->headEffectState) {
                    face_requestExpression(actor, sounds[math_random() & 1], 1, (math_random() % 180U) + 0x1A4, 0);
                    actor->headEffectState = 0;
                    actor->headEffectOffset[0][0] = 1;
                }
            } else if ((actor->headEffectOffset[0][0] != 0) && (actor->expression == actor->defaultExpression)) {
                actor->headEffectOffset[0][0] = 0;
            }
            actor->norun = 0xA;
            actor->nojump = 0xA;
            actor->count5 = 0xA;
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

            obj = &movement_obj[actor->attach_char - 1];
            actor->mv->rt_context_b = 0x1B;
            actor->nojump = 0x14;
            if ((gIsMultiplayer == 0) || (actor->attach_char != 0)) {
                clearAction = 0;
            }
            if (actor->mv->ReloadTimer == 0) {
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
                    actor->mv->shotCount = 1;
                    if ((animGrab != obj->animseq.uh) || ((D_800C3A58 != 0) && (animSpecial != 0))) {
                        particle_spawnTwoSidedEmitter(obj, actor, 0, animGrab);
                        sound_stopActorSound1(obj);
                        if ((actor->character_type == 0x80) && !(math_random() & 3)) {
                            sound_addDynamicSound(sound_pickVariant(0x57C, 6), actor->xpos, actor->ypos, actor->zpos, 0x7FFF, 0x3E8, 0x1F4, sound_followOwnerAnimsCallback, actor, (s32)D_800996D0, 0, 0);
                        }
                    }
                    if (gFrameTicks < actor->mv->gun_before_reload) {
                        actor->mv->gun_before_reload -= gFrameTicks;
                    } else {
                        next = animRelease;
                        sound_stopActorSound2(obj);
                    }
                } else if ((D_800C3A58 != 0) && (animSpecial != obj->intelwalk) && (animSpecial != 0)) {
                    obj->intelwalk = animSpecial;
                    obj->scriptCmd = NULL;
                } else if (actor->mv->shotCount == 1) {
                    actor->mv->shotCount = 0;
                    if (sound_findByCallback(sound_delayedPlayCallback, (void *)-1, -1) == -1) {
                        sound_addDynamicSound((math_random() % 3U) + 0x618, actor->xpos, actor->ypos, actor->zpos, 0x7D00, 0x1F4, 0xC8, sound_delayedPlayCallback, (void *)0x1E, 0, 0x300, 0);
                    }
                }
                if (pressedButtons & B_BUTTON) {
                    next = animIdle;
                    sound_stopActorSound1(obj);
                    sound_stopActorSound2(obj);
                }
                if (next != 0) {
                    actor->mv->gun_before_reload = 0x258;
                    actor->mv->ReloadTimer = 0x96;
                    sound_addDynamicSound((math_random() % 3U) + 0x618, actor->xpos, actor->ypos, actor->zpos, 0x7D00, 0x1F4, 0xC8, sound_delayedPlayCallback, (void *)0x1E, 0, 0x300, 0);
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
            if ((actor->character_type == 0x75) || (animDrop = 0x16C, (actor->character_type == 0xB1))) {  // matching: IDO schedules the default store between the two tests
                animDrop = 0x16B;
                item = 0x64;
                sfx = 0x11;
            } else {
                animThrow = 0x175;
                item = 0x2C;
                sfx = 0x12;
            }
            if (isStunned != 0) {
                if ((act == actor->mv->rt_context_b) || (actor->mv->GrenadeThrowState == 4)) {
                    if ((attach_find(actor, item, 0) != NULL) || (actor->mv->GrenadeThrowState == 4)) {
                        collision_removePartSet(actor, item);
                        gEventParam = (u16)sfx | 0x10000;
                        event_spawnThrownItem();
                        if (actor->mv->numGrenades != 0) {
                            actor->mv->numGrenades--;
                        }
                    }
                    actor->mv->GrenadeThrowState = 0;
                    actor->mv->weapon_status = 0;
                }
            } else {
                actor_turnWaist(actor, heldButtons, R_TRIG);
                if (actor->mv->GrenadeThrowState == 1) {
                    if (act != actor->mv->rt_context_b) {
                        actor->mv->rt_context_b = act;
                        actor->mv->weapon_status = 1;
                        actor->norun = 0x1E;
                        actor->nojump = 0x1E;
                        actor->count5 = 0x1E;
                        actor->mv->GrenadeThrowState = 2;
                        anim = animThrow;
                        actor->mv->GrenadeTimer = 0xF0;
                        actor->mv->shotsfired++;
                    }
                } else if (actor->mv->GrenadeThrowState == 2) {
                    if (!(gActorHeldButtons & D_CBUTTONS) && !(actor->unk2F8 & 0x10)) {
                        actor->mv->GrenadeThrowState = 3;
                    }
                    if (gFrameTicks < actor->mv->GrenadeTimer) {
                        actor->mv->GrenadeTimer -= gFrameTicks;
                    } else {
                        actor->mv->GrenadeThrowState = 0;
                        actor->mv->rt_context_b = 0;
                        actor->imune = 0;
                        actor_throwGrenade(actor);
                    }
                } else if ((actor->mv->GrenadeThrowState == 3) && (height <= 0.0f) && (animThrow != actor->animseq.uh)) {
                    actor->norun = 0xFF;
                    actor->count5 = 0xFF;
                    actor->mv->GrenadeThrowState = 4;
                    anim = animDrop;
                    actor->mv->rt_context_b = 0;
                    actor->mv->weapon_status = 3;
                    if (actor->mv->numGrenades != 0) {
                        actor->mv->numGrenades--;
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
            if (isStunned != 0) {
                if (act == actor->mv->rt_context_b) {
                    obj = actor_findLinked(actor, item);
                    if (obj != NULL) {
                        obj->attach_char = 0;
                        obj->ygrav = 4.0f;
                        obj->yspd = 0.0f;
                        obj->speed = 0.0f;
                    }
                    actor->mv->GrenadeThrowState = 0;
                    actor->mv->rt_context_b = 0;
                    actor->mv->weapon_status = 0;
                }
            } else {
                actor_turnWaist(actor, heldButtons, R_TRIG);
                if (actor->mv->GrenadeThrowState == 5) {
                    if (act != actor->mv->rt_context_b) {
                        if (item == 0x89) {
                            obj = collision_createPartSet(actor, 0x86, 1.0f, 0);
                        } else {
                            obj = collision_createPartSet(actor, 0x9F, 1.0f, 0);
                        }
                        if (obj != NULL) {
                            actor->mv->rt_context_b = act;
                            anim = 0x1F9;
                            actor->mv->weapon_status = 1;
                            actor->norun = 0x1E;
                            actor->nojump = 0xFF;
                            actor->count5 = 0xFF;
                            actor->mv->GrenadeThrowState = 6;
                            obj->imune = 0xFF;
                            actor_setScale(obj, 1.0f, 1.0f);
                            obj->taskint[0] = 0xF0;
                            obj->taskint[1] = 0xF0;
                            obj->owner = dc_glbl_CharNum + 1;
                            obj->noshove = 0x401807;
                            actor_playAnim(obj, 2, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                            effect_attachPulseEffect(obj);
                        } else {
                            actor->mv->GrenadeThrowState = 0;
                        }
                    }
                } else if (actor->mv->GrenadeThrowState == 6) {
                    if (!(gActorHeldButtons & D_CBUTTONS)) {
                        actor->mv->GrenadeThrowState = 7;
                    }
                } else if ((actor->mv->GrenadeThrowState == 7) && (height <= 0.0f) && (actor->animseq.uh != 0x1F9)) {
                    actor->norun = 0xFF;
                    actor->count5 = 0xFF;
                    actor->mv->GrenadeThrowState = 0;
                    anim = 0x1FC;
                    actor->mv->rt_context_b = 0;
                    actor->mv->weapon_status = 0;
                    if (actor->mv->numGrenades != 0) {
                        actor->mv->numGrenades--;
                    }
                    actor->mv->shotsfired++;
                    obj = actor_findLinked(actor, item);
                    if (obj != NULL) {
                        s16 pitch;
                        f32 vx;
                        f32 vy;
                        f32 vz;
                        f32 dist;

                        obj->attach_char = 0;
                        dist = ((240 - obj->taskint[0]) * 210.0f) / 240.0f;
                        dist += 40.0f;
                        if (actor->mv->AimState != 0) {
                            pitch = (30.0f - actor->mv->aimPitch) * 182.0444489f;
                        } else {
                            pitch = 0x1555;
                        }
                        actor_splitVelocity(pitch, dist, 0.0f, &vx, &vy, &vz);
                        obj->yspd = -vy;
                        obj->speed = vx;
                        obj->ygrav = 4.0f;
                        obj->angle = obj->langle = actor->langle - actor->mv->pssd_waist_tilt;
                    }
                }
            }
        }
        break;
    case 0x3E:
        if (actor->team == 0) {
            if (actor->count5 == 0) {
                actor->count5 = 1;
            }
            if (actor->mv->unk12C >= gFrameTicks) {
                actor->mv->unk12C -= gFrameTicks;
                actor->speedPercent = 0x96;
            } else {
                actor->speedPercent = 0x4B;
                actor->mv->unk12C = 0;
            }
        }
        break;
    }

    if ((nested != 0) || (isStunned != 0)) {
        actor->mv->context_exclude_time = 0;
        if (isStunned != 0) {
            anim = 0;
            handled = 1;
            if (clearAction != 0) {
                actor->mv->rt_context_b = 0;
            }
        } else {
            actor->mv->context_b = 0;
            actor->mv->rt_context_b = 0;
            if (actor->mv->weapon_status != 3) {
                actor->mv->weapon_status = 0;
            }
        }
    }
    if ((anim != 0) && (anim != 999) && (noPlay == 0)) {
        actor_playAnim(actor, anim, speed, blend, animParam, 0.0f, 0);
    }
    if (setUnk58 != 0) {
        actor->mv->idle.active = 1;
    }
    if (pendingSound != 0) {
        actor_playSound(pendingSound, actor);
    }
    if (handled != 0) {
        return 1000;
    } else {
        return anim;
    }
}
