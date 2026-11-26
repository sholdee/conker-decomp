#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Defined in game/Particle/SpriteParticle/spriteparticle.c as void particle_applyDrag(Vec3f *vel, f32 drag). This file was compiled with a
   view that takes a third argument. */
void particle_applyDrag(void *vel, f32 drag, void *arg2);

/* Trail anchor point: 20 units above the actor. */
void trail_getPissAnchor(Actor *actor, Vec3f *out) {
    out->x = actor->xpos;
    out->y = actor->ypos + 20.0f;
    out->z = actor->zpos;
}

/* Trail launch vector from the actor (math_getActorAim's offset direction). */
void trail_getPissLaunchDir(Actor *actor, Vec3f *out) {
    Vec3f target;
    Vec3f dir;

    math_getActorAim(actor, &target, &dir, out, gIsMultiplayer ? 0.8400000334f : 1.15200007f);
}

/* Spawns a trail object that follows the actor. */
void trail_spawnPissTrail(Actor *actor) {
    EffectTemplate spawn;
    s32 pad; // matching: stack slot above data
    PissTrailData data;
    Vec3f pos;
    PissTrail *trail;
    s32 variant;

    if (actor->mv != NULL) {
        if (level == 10) {
            music_playTune(10, 55, 0);
        }

        trail_getPissAnchor(actor, &spawn.pos);
        data.timer = 0;
        data.flags = 1;
        data.owner = actor;
        data.ownerId = actor->unique_id;
        pos = spawn.pos;
        data.unk8 = pos;
        data.lastPos = pos;
        trail_getPissLaunchDir(actor, &data.lastVel);

        data.speedScale = 0.01724099927f;
        variant = math_rand() % 2;
        data.sound = func_10010F88(variant + 0x1B6, 0x7FFF, 0, 0, 0, 0, 0, 0, 500, 5000);
        data.pending = 0.0f;
        data.time = 0.0f;
        data.surfaceAccum = 0.0f;
        data.impactAccum = 0.0f;
        data.sparkAccum = 0.0f;
        data.texScroll = -16384.0f;

        if (actor->mv != NULL) {
            if (actor->mv->bot != 0) {
                data.flags &= ~1;
            }
        }

        spawn.itemCount = 50;
        spawn.life = 300;
        spawn.flags = 54;
        spawn.kind = 2;
        spawn.unk14 = 5;
        spawn.material = 0;

        trail = effect_newEffectObj(&spawn, sizeof(PissTrailData), sizeof(PissTrailPoint), 0xE, 1, 0x11, 3, 0xFF, 0, 0xFF, 1);
        if (trail != NULL) {
            memcpy(trail->data, &data, sizeof(PissTrailData));
        }
    }
}

/* Advances pos by vel over dt under gravity. */
void trail_stepBallistic(Vec3f *pos, Vec3f *vel, f32 dt) {
    Vec3f v0;

    v0 = *vel;
    vel->y += -1.91200006f * dt;
    pos->x += v0.x * dt;
    pos->y += (v0.y * dt) + ((-0.95600003f * dt) * dt);
    pos->z += v0.z * dt;
}

void func_150D8E1C(PissTrail *trail) {
    trail->head.updateKind2 = 0;
    trail->head.tmpl.flags &= ~2;
    trail->head.tmpl.flags |= 8;
    trail->head.tmpl.flags |= 1;
    trail->head.tmpl.life = 40;
}

/* Moves every live point under gravity and ticks the owner's counter. */
s32 trail_movePissPoints(PissTrail *trail) {
    PissTrailData *data;
    PissTrailPoint *points;

    data = trail->data;
    points = trail->points;
    if ((trail->head.count < 2) && (trail->head.tmpl.flags & 8)) {
        return 0;
    } else {
        s32 i = trail->head.end;

        while (i != trail->head.start) {
            i--;
            if (i < 0) {
                i = trail->head.tmpl.itemCount - 1;
            }
            trail_stepBallistic(&points[i].pos, &points[i].vel, gFrameDelta);
        }
    }

    if (trail->head.count > 0) {
        trail->head.modelPos = points[trail->head.start].pos;
    } else {
        trail->head.modelPos.x = 0.0f;
        trail->head.modelPos.y = 0.0f;
        trail->head.modelPos.z = 0.0f;
    }

    data->timer += D_800BE9E4;
    if (data->timer >= 61) {
        if (data->owner != NULL && data->owner->mv != NULL) {
            data->owner->mv->shotsfired++;
        }
        data->timer = 0;
    }

    return 1;
}

/* Follows the owner and emits new trail points, interpolated over the elapsed time. */
s32 trail_emitPissPoints(PissTrail *trail) {
    PissTrailData *data;
    PissTrailPoint *points;
    Actor *actor;
    Vec3f pos;
    Vec3f vel;
    f32 time;
    f32 inv;
    u16 flags;
    f32 step;
    Vec3f dpos;
    Vec3f dvel;

    data = trail->data;
    points = trail->points;
    actor = data->owner;
    if ((actor->mv->rt_context_b != 3) || (actor->energy <= 0)) {
        func_150D8E1C(trail);
    }
    if ((actor->type == 0) || (actor->unique_id != data->ownerId)) {
        return 0;
    }
    trail_getPissAnchor(actor, &pos);
    trail_getPissLaunchDir(actor, &vel);
    trail->head.tmpl.pos = pos;
    if (data->flags & 1) {
        flags = actor->mv->buttons;
        if (flags & 0x2000) {
            data->speedScale += 0.001379000023f * gFrameDelta;
            if (data->speedScale > 0.04293100163f) {
                data->speedScale = 0.04293100163f;
            }
        } else {
            data->speedScale -= 0.001379000023f * gFrameDelta;
            if (data->speedScale < 0.01724099927f) {
                data->speedScale = 0.01724099927f;
            }
        }
    }
    data->pending += 0.380000025f * gFrameDelta;
    if (data->pending > 1.0f) {
        Vec3f cur;
        Vec3f curVel;
        f32 dpx;
        f32 dpy;
        f32 dpz;
        Vec3f dvs;

        inv = 1.0f / data->pending;
        time = data->time + gFrameDelta;
        cur = data->lastPos;
        curVel = data->lastVel;
        step = time * inv;
        dpos.x = pos.x - data->lastPos.x;
        dpos.y = pos.y - data->lastPos.y;
        dpos.z = pos.z - data->lastPos.z;
        dvel.x = vel.x - data->lastVel.x;
        dvel.y = vel.y - data->lastVel.y;
        dvel.z = vel.z - data->lastVel.z;
        dpx = dpos.x * inv;
        dpy = dpos.y * inv;
        dpz = dpos.z * inv;
        dvs.x = dvel.x * inv;
        dvs.y = dvel.y * inv;
        dvs.z = dvel.z * inv;
        do {
            PissTrailPoint *point;

            point = &points[trail->head.end];
            point->pos = cur;
            point->vel.x = data->speedScale * curVel.x;
            point->vel.y = data->speedScale * curVel.y;
            point->vel.z = data->speedScale * curVel.z;
            point->len = 0.0f;
            point->alpha = 255;
            point->texture = 0.0f;
            trail_stepBallistic(&point->pos, &point->vel, time);
            trail->head.end++;
            time -= step;
            if (trail->head.end == trail->head.tmpl.itemCount) {
                trail->head.end = 0;
            }
            trail->head.count++;
            if (trail->head.end == trail->head.start) {
                /* buffer full: drop the oldest point */
                trail->head.start++;
                if (trail->head.start == trail->head.tmpl.itemCount) {
                    trail->head.start = 0;
                }
                trail->head.count--;
            }
            cur.x += dpx;
            cur.y += dpy;
            cur.z += dpz;
            curVel.x += dvs.x;
            curVel.y += dvs.y;
            curVel.z += dvs.z;
            data->pending -= 1.0f;
        } while (data->pending > 1.0f);
        data->lastPos = cur;
        data->lastVel = curVel;
        data->time = time;
    }
    return 1;
}

/*
 * Walks the trail from newest to oldest point: clamps its total length, ray casts every
 * segment (hits trim the trail and affect the object hit), then scrolls the texture and
 * fades the oldest part.
 */
s32 trail_tracePissTrail(PissTrail *trail) {
    s32 count;
    s32 i;
    f32 totalLen;
    f32 pad; // matching: unused stack slot
    f32 maxLen;
    f32 excess;
    Vec3f delta;
    f32 len;
    f32 invLen;
    f32 dir[3];
    s32 pad2; // matching: stack slot (the old local view of RayHit was 0x68 bytes)
    RayHit hit;
    PissTrailPoint *points;
    PissTrailPoint *prev;
    PissTrailData *data;
    Actor *owner;
    Actor *target;
    f32 dx;
    f32 dz;
    s32 ownerIndex;
    s32 j, k;
    f32 invFadeLen;
    f32 texPos;
    f32 fadeLen;

    count = trail->head.count;
    data = trail->data;
    points = trail->points;
    owner = data->owner;
    if (count >= 2) {
        totalLen = 0.0f;
        i = trail->head.end;
        if (trail->head.tmpl.flags & 2) {
            prev = (PissTrailPoint *)&trail->head.tmpl.pos;
        } else {
            i--;
            if (i < 0) {
                i = trail->head.tmpl.itemCount - 1;
            }
            prev = &points[i];
        }
        maxLen = 1195.0f;
        do {
            PissTrailPoint *cur;

            invLen = -9999.0f;
            i--;
            if (i < 0) {
                i = trail->head.tmpl.itemCount - 1;
            }
            cur = &points[i];
            delta.x = cur->pos.x - prev->pos.x;
            delta.y = cur->pos.y - prev->pos.y;
            delta.z = cur->pos.z - prev->pos.z;
            len = math_vec3Length(&delta);
            totalLen += len;
            cur->len = len;
            if (maxLen < totalLen) {
                /* too long: cut this segment and drop everything older */
                // matching: integer 0 here; 0.0f hoists the zero constant and grows the frame
                if (cur->len != 0) {
                    invLen = 1.0f / cur->len;
                    dir[0] = delta.x * invLen;
                    dir[1] = delta.y * invLen;
                    excess = (totalLen - maxLen) * invLen;
                    dir[2] = delta.z * invLen;
                    cur->pos.x -= delta.x * excess;
                    cur->pos.y -= delta.y * excess;
                    cur->pos.z -= delta.z * excess;
                    cur->len *= 1.0f - excess;
                }
                totalLen = maxLen;
                while (i != trail->head.start) {
                    trail->head.start++;
                    if (trail->head.start == trail->head.tmpl.itemCount) {
                        trail->head.start = 0;
                    }
                    trail->head.count--;
                }
            }

            if (cur->len != 0.0f) {
                if (invLen == -9999.0f) {
                    invLen = 1.0f / cur->len;
                    dir[0] = delta.x * invLen;
                    dir[1] = delta.y * invLen;
                    dir[2] = delta.z * invLen;
                }
                collision_castRay(owner, prev->pos.x, prev->pos.y, prev->pos.z, dir[0], dir[1], dir[2], &hit, cur->len, 0, 0, 1, -1, 0, 0);
                if (hit.hitType != 0) {
                    trail_spawnPissImpact(trail, &hit, dir);
                    if (hit.hitType >= 2) {
                        target = hit.obj;
                        target->taskint[2] = 1;
                        if (target->character_type == 0x3A) {
                            target->intelwalk = 10;
                            target->scriptCmd = NULL;
                        } else if (target->character_type == 0x10 && target->unk251 == 1) {
                            actor_stepVector2D(target->xpos - hit.pos.x, target->zpos - hit.pos.z,
                                          ((1.0f - (totalLen - 50.0f) * 0.001052631531f) * 0.4550000131f) + 0.2010000050f,
                                          &target->ob_slopex, &target->ob_slopey);
                            if (target->intelwalk == 15) {
                                s32 pad; // matching: -g3 block scope
                                target->scriptCmd = NULL;
                            }
                        } else if (target->character_type == 0x10) {
                            dx = cur->pos.x - owner->xpos;
                            dz = cur->pos.z - owner->zpos;
                            if (target->intelwalk == 16) {
                                target->speed *= 0.7200000286f;
                                excess = 1.0f / (sqrtf(dx * dx + dz * dz) * -0.07000000030f);
                                actor_stepVector2D(dx * excess, dz * excess, 0.7f, &target->ob_slopex, &target->ob_slopey);
                            }
                            if (target->imune == 0) {
                                s32 pad; // matching: -g3 block scope
                                target->intelwalk = 15;
                                target->scriptCmd = NULL;
                            }
                        } else if (target->character_type == 0x91) {
                            target->intelwalk = 3;
                            target->scriptCmd = NULL;
                        } else if (target->character_type == 0x90) {
                            target->intelwalk = 44;
                            target->scriptCmd = NULL;
                        } else {
                            ownerIndex = owner - movement_obj;
                            actor_hitScripted(target, 0xE0036, ((u16)(math_atan2(dir[0], dir[2]) * 10430.37793f) - 0x4000) | 1, ownerIndex);
                        }
                    }
                    /* the hit ends the trail here */
                    cur->pos = hit.pos;
                    totalLen -= cur->len;
                    cur->len = hit.dist;
                    totalLen += cur->len;
                    while (i != trail->head.start) {
                        trail->head.start++;
                        if (trail->head.start == trail->head.tmpl.itemCount) {
                            trail->head.start = 0;
                        }
                        trail->head.count--;
                    }
                    if (data->sound != 0) {
                        func_1000F91C(data->sound, 0x7FFF, 0, 0, 0, cur->pos.x, cur->pos.y, cur->pos.z, 500, 5000);
                    }
                }
            }
            prev = cur;
        } while (i != trail->head.start);
        count = trail->head.count;
    }
    if (count >= 2) {
        data->texScroll += -395.0f * gFrameDelta;
        data->texScroll = math_wrapFloat(data->texScroll, 16383.0f, -16384.0f);
        texPos = 0.0f;
        j = trail->head.end;
        // matching: a do-while or plain break here changes the frame size (-g3)
        for (;;) {
            PissTrailPoint *cur2;

            j--;
            if (j < 0) {
                j = trail->head.tmpl.itemCount - 1;
            }
            cur2 = &points[j];
            texPos += cur2->len;
            cur2->texture = math_wrapFloat(data->texScroll + texPos * 29.02999878f, 16383.0f, -16384.0f);
            if (j != trail->head.start) {
                continue;
            }
            break;
        }
        count = trail->head.count;
    }
    if (count >= 2) {
        /* fade out the oldest 19.8% of the trail */
        len = 0.0f;
        dx = totalLen * 0.8020000458f;
        k = trail->head.end;
        fadeLen = totalLen - dx;
        invFadeLen = 1.0f / fadeLen;
        do {
            PissTrailPoint *cur3;

            k--;
            if (k < 0) {
                k = trail->head.tmpl.itemCount - 1;
            }
            cur3 = &points[k];
            len += cur3->len;
            if (dx < len) {
                s32 pad; // matching: -g3 block scope
                cur3->alpha = (u32)((fadeLen - (len - dx)) * invFadeLen * 155.0f);
            } else {
                cur3->alpha = 155;
            }
        } while (k != trail->head.start);
    }
    return 1;
}

extern s32 gEffectRenderModes[][2];

/* Draws the trail as a camera-facing ribbon, one quad per segment. */
Gfx *trail_drawPissTrail(PissTrail *trail, Gfx *gfx, s16 view) {
    PissTrailPoint *points;
    Vec3f next;
    Vec3f cur;
    Vec3f toCamera;
    Vec3f delta;
    Vec3f normal;
    PissTrailData *data;
    Vec3f *camera;
    s32 index;
    s32 previous;
    f32 lengthSquared;
    u8 flag;
    u8 alpha;
    f32 nextTexture;
    f32 curTexture;
    Vtx_t *vtx;

    if (trail->head.count < 2) {
        return gfx;
    }
    data = trail->data;
    points = trail->points;
    particle_getViewBuffer(trail->vtxBuf, view, (trail->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx_t), &vtx, NULL);
    if (vtx == NULL) {
        return gfx;
    }
    camera = math_getCameraPos(view);
    flag = 1;
    gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[146], 0, 0, 0, 0, 0x92, 0, 0, &flag, 3);
    gfx = math_gfxUpdateGeometryMode(gfx, 0x220005, 0x1D0600);
    gfx = particle_setCombineMode(gfx, 0x4E, &flag);
    gfx = math_gfxSetOtherMode(gfx, gSpriteTlutMode | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, gEffectRenderModes[3][0] | gEffectRenderModes[3][1], &flag);
    if (trail->head.tmpl.flags & 2) {
        index = trail->head.end - 1;
        if (index < 0) {
            index = trail->head.tmpl.itemCount - 1;
        }
        cur = trail->head.tmpl.pos;
        curTexture = data->texScroll;
    } else {
        previous = trail->head.end - 1;
        if (previous < 0) {
            previous = trail->head.tmpl.itemCount - 1;
        }
        index = previous - 1;
        if (index < 0) {
            index = trail->head.tmpl.itemCount - 1;
        }
        cur = points[previous].pos;
        curTexture = points[previous].texture;
    }
    next = points[index].pos;
    alpha = points[index].alpha;
    nextTexture = points[index].texture;
    delta.x = cur.x - next.x;
    delta.y = cur.y - next.y;
    delta.z = cur.z - next.z;
    toCamera.x = cur.x - camera->x;
    toCamera.y = cur.y - camera->y;
    toCamera.z = cur.z - camera->z;
    normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
    normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
    normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
    lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
    if (lengthSquared != 0.0f) {
        f32 scale = 4.0f / sqrtf(lengthSquared);

        normal.x *= scale;
        normal.y *= scale;
        normal.z *= scale;
    } else {
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
    }
    vtx->ob[0] = cur.x + normal.x;
    vtx->ob[1] = cur.y + normal.y;
    vtx->ob[2] = cur.z + normal.z;
    vtx->tc[0] = curTexture;
    vtx->tc[1] = 0x400;
    vtx->cn[0] = 0xFF;
    vtx->cn[1] = 0xFF;
    vtx->cn[2] = 0xFF;
    vtx->cn[3] = alpha;
    vtx->flag = 0;
    vtx++;
    vtx->ob[0] = cur.x - normal.x;
    vtx->ob[1] = cur.y - normal.y;
    vtx->ob[2] = cur.z - normal.z;
    vtx->tc[0] = curTexture;
    vtx->tc[1] = 0;
    vtx->cn[0] = 0xFF;
    vtx->cn[1] = 0xFF;
    vtx->cn[2] = 0xFF;
    vtx->cn[3] = alpha;
    vtx->flag = 0;
    vtx++;
    do {
        delta.x = cur.x - next.x;
        delta.y = cur.y - next.y;
        delta.z = cur.z - next.z;
        toCamera.x = next.x - camera->x;
        toCamera.y = next.y - camera->y;
        toCamera.z = next.z - camera->z;
        normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
        normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
        normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
        lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
        if (lengthSquared != 0.0f) {
            f32 scale = 4.0f / sqrtf(lengthSquared);

            normal.x *= scale;
            normal.y *= scale;
            normal.z *= scale;
        } else {
            normal.x = 0.0f;
            normal.y = 0.0f;
            normal.z = 0.0f;
        }
        vtx->ob[0] = next.x + normal.x;
        vtx->ob[1] = next.y + normal.y;
        vtx->ob[2] = next.z + normal.z;
        vtx->tc[0] = nextTexture;
        vtx->tc[1] = 0x400;
        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0xFF;
        vtx->cn[2] = 0xFF;
        vtx->cn[3] = alpha;
        vtx->flag = 0;
        vtx++;
        vtx->ob[0] = next.x - normal.x;
        vtx->ob[1] = next.y - normal.y;
        vtx->ob[2] = next.z - normal.z;
        vtx->tc[0] = nextTexture;
        vtx->tc[1] = 0;
        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0xFF;
        vtx->cn[2] = 0xFF;
        vtx->cn[3] = alpha;
        vtx->flag = 0;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 1, 3, 2, 0);
        if (nextTexture < curTexture) {
            /* texture wrapped: repeat the last pair shifted back one period */
            memcpy(vtx, vtx - 2, 2 * sizeof(Vtx_t));
            vtx[-2].tc[0] -= 0x8000;
            vtx++;
            vtx[-2].tc[0] -= 0x8000;
            vtx++;
        }
        previous = index;
        index--;
        if (index < 0) {
            index = trail->head.tmpl.itemCount - 1;
        }
        cur = points[previous].pos;
        next = points[index].pos;
        alpha = points[index].alpha;
        curTexture = points[previous].texture;
        nextTexture = points[index].texture;
    } while (previous != trail->head.start);
    return gfx;
}

/* Stops the trail's sound. */
void trail_stopPissSound(PissTrail *trail) {
    PissTrailData *data = trail->data;

    if (data->sound != 0) {
        sound_stop(data->sound);
    }
}

void trail_destroyPissTrail(PissTrail *trail) {
    trail_stopPissSound(trail);
    effect_defaultDestroyEffectObj(trail);
}

void trail_freePissTrail(PissTrail *trail) {
    trail_stopPissSound(trail);
    effect_defaultFreeEffectObj(trail);
}

/* Trail message handler: 0x44 sets the speed scale, 0 removes the trail, 0x2D swaps owners. */
void trail_onPissTrailMessage(PissTrail *trail, TrailMessage *msg, u8 type) {
    PissTrailData *data = trail->data;
    Actor *owner;

    if (type == 0x44) {
        if ((msg->owner == data->owner) || (msg->x4.ownerId == data->ownerId)) {
            data->speedScale = msg->x8.speedScale;
        }
    // matching: the label keeps a branch-to-epilogue after the store
    done:
        ;
    } else if (type == 0) {
        owner = msg->owner;
        if ((owner == data->owner) || (data->ownerId == msg->x4.ownerId)) {
            effect_destroyObject(&trail->head.header);
        }
    } else if (type == 0x2D) {
        if (data->owner == msg->owner) {
            data->owner = msg->x4.other;
            data->ownerId = msg->x8.ids.otherId;
        } else if (data->owner == msg->x4.other) {
            data->owner = msg->owner;
            data->ownerId = msg->x8.ids.ownerId;
        }
    }
}

/* Sets the speed scale of every trail owned by the actor. */
void trail_setPissSpeedScale(Actor *actor, f32 speedScale) {
    TrailMessage msg;

    msg.owner = actor;
    msg.x4.ownerId = actor->unique_id;
    msg.x8.speedScale = speedScale;
    effect_sendEventToEffectObjs((s32)&msg, 0x44);
}

s32 func_150DA628(SpriteParticle *particle, s32 arg1) {
    WobbleData *spark;

    spark = &particle->data.wobble;
    particle_wobbleSpriteOffset(&particle->head, spark->amplitude, spark->phase, spark->phaseStep);
    particle_applyDrag(&particle->head.desc.vel, spark->drag, spark);
    return 1;
}

/* Spawns the impact effects of a trail segment hitting something (hit from collision_castRay). */
void trail_spawnPissImpact(PissTrail *trail, RayHit *hit, f32 *dir) {
    PissTrailData *data;
    Actor *target;

    data = trail->data;
    target = hit->obj;
    if (hit->hitType != 0) {
        if (hit->hitType >= 2 && (target->character_type == 0x33 || target->character_type == 0x3A)) {
            data->sparkAccum += (0.1990000159f + func_150ADA68() * 0.1540000141f) * gFrameDelta;
            if (data->sparkAccum > 1.0f) {
                ParticleDesc spawn;
                WobbleData sub;
                SpriteParticle *spark;

                sub.drag = 0.9936270118f;
                spawn.texture = 0x6C;
                spawn.renderMode = 0x5103;
                spawn.unk0 = 0x200005;
                spawn.fadeStart = 0x2E;
                spawn.fadeRate = 5;
                spawn.flags = 0x90DE07;
                spawn.primColorMode = 8;
                spawn.envColorMode = 6;
                spawn.updateFunc = 0x24;
                spawn.deathFunc = -1;
                spawn.unk4 = 0;
                spawn.frame = 0;
                spawn.frameRate = 0;
                spawn.drawFunc = -1;
                spawn.handlerSet = 0;
                spawn.scaleStart = 0x2E;
                spawn.r = 0xDD;
                spawn.g = 0xD3;
                spawn.b = 0xCD;
                spawn.a = 0xFF;
                spawn.r2 = 0x57;
                spawn.g2 = 0x55;
                spawn.b2 = 0x5A;
                spawn.brightness = 0xFF;
                spawn.offset.x = 0.0f;
                spawn.offset.y = 0.0f;
                spawn.offset.z = 0.0f;
                spawn.scaleRate = 1.038234949f;
                spawn.pos = hit->pos;
                spawn.vel = gZeroVec3f;
                do {
                    sub.phase[0] = func_150ADA20();
                    sub.phase[1] = (s16)func_150ADA20(); // matching: the s16 cast gives IDO's extra move
                    sub.phaseStep[0] = (func_150ADA20() % 5U) + 4;
                    sub.phaseStep[1] = (func_150ADA20() % 5U) + 4;
                    sub.amplitude[0] = func_150ADA68() * 10.0f;
                    sub.amplitude[1] = func_150ADA68() * 10.0f;
                    spawn.gravity = func_150ADA68() * 0.0910000056f + 0.1010000035f;
                    spawn.flags &= ~0xC0;
                    spawn.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
                    spawn.alpha = (func_150ADA20() % 101U) + 100;
                    spawn.timer = (func_150ADA20() % 11U) + 70;
                    spawn.width = spawn.height = func_150ADA68() * 116.0f + 131.0f;
                    spark = particle_newSpriteParticleWithFloor(&spawn, 1, 0, 0x10, trail->head.header.owner, trail->head.header.kind);
                    if (spark != NULL) {
                        memcpy(&spark->data.wobble, &sub, sizeof(sub));
                    }
                    data->sparkAccum -= 1.0f;
                } while (data->sparkAccum > 1.0f);
            }
        } else {
            data->impactAccum += (0.06000000238f + func_150ADA68() * 0.06000000238f) * gFrameDelta;
            if (data->impactAccum > 1.0f) {
                SplashSpray emit;
                CollisionHit origin;

                collision_initHitFromRay(&origin, hit);
                emit.pos = hit->pos;
                emit.scaleBase = 3.480000019f;
                emit.countBase = 1;
                emit.countRange = 2;
                emit.itemCountBase = 3;
                emit.itemCountRange = 1;
                emit.lifeBase = 20;
                emit.lifeRange = 10;
                emit.unk30 = 200;
                emit.unk32 = 55;
                emit.unk3C = 16;
                emit.unk3E = 15;
                emit.unk40 = 0;
                emit.kind = 3;
                emit.scaleRange = 2.659999847f;
                emit.sizeBase = 1.33100009f;
                emit.sizeRange = 1.160000086f;
                emit.speedBase = 0.01499999966f;
                emit.speedRange = 0.00699999975f;
                emit.unk38 = 0.200000003f;
                if (hit->hitType >= 2) {
                    ConeBasis cone;

                    cone.flags = 1;
                    cone.direction.x = -dir[0];
                    cone.direction.y = -dir[1];
                    cone.direction.z = -dir[2];
                    cone.axis0 = cone.axis1 = gZeroVec3f;
                    cone.spread = 300.0f;
                    do {
                        particle_spawnSplashSpray(&cone, &emit, &origin, trail->head.header.owner, trail->head.header.kind);
                        data->impactAccum -= 1.0f;
                    } while (data->impactAccum > 1.0f);
                } else {
                    TriangleSource spray;

                    spray.normal.x = -dir[0];
                    spray.normal.y = -dir[1];
                    spray.normal.z = -dir[2];
                    spray.triangle = hit->tri;
                    spray.spread = 300.0f;
                    do {
                        particle_spawnSplashSprayOnTri(&spray, &emit, &origin, trail->head.header.owner, trail->head.header.kind);
                        data->impactAccum -= 1.0f;
                    } while (data->impactAccum > 1.0f);
                }
            }
        }
        if (hit->hitType == 1) {
            data->surfaceAccum += (0.200000003f + func_150ADA68() * 0.1040000021f) * gFrameDelta;
            while (data->surfaceAccum > 1.0f) {
                particle_spawnKindEmitter(3, func_150ADA68() * 30.0f + 15.0f, (func_150ADA20() % 101U) + 100, &hit->tri, &hit->pos,
                              (func_150ADA20() % 81U) + 60, 1, 1, 1, trail->head.header.owner, trail->head.header.kind);
                data->surfaceAccum -= 1.0f;
            }
        }
    }
}

