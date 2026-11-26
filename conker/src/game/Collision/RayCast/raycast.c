#include <ultra64.h>

#include "functions.h"
#include "variables.h"



/* Casts a ray from origin along dir against the world and every active object (except self and those in
   ignoreMask or rejected by filter), recording the nearest hit closer than maxDist in out. */
void collision_castRay(Actor *self, f32 originX, f32 originY, f32 originZ, f32 dirX, f32 dirY, f32 dirZ, RayHit *out, f32 maxDist, s32 skipWorld, s32 skipObjs, s32 skipModels, s8 filter, void *filterArg, s32 ignoreMask) {
    s32 surf;
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    f32 dist;
    f32 farDist;
    s32 part;
    s32 bone;
    s16 offset[4];
    s32 i;
    Actor *obj;
    s32 best;
    s32 skip;
    s32 ret;
    u8 childSlot[25];
    Vec3f near;
    Vec3f far;
    Vec3f centre;
    Vec3f origin;
    Vec3f dir;
    s32 index;
    s32 surfIdx;

    origin.x = originX;
    origin.y = originY;
    origin.z = originZ;
    dir.x = dirX;
    dir.y = dirY;
    dir.z = dirZ;
    out->dist = -1.0f;
    out->obj = NULL;
    out->hitType = 0;
    out->origin = origin;
    best = -1;
    out->dir = dir;
    out->surface = NULL;
    out->material = 0;
    if (maxDist == 0.0f) {
        maxDist = 9999999.0f;
    }
    if (self != NULL) {
        skip = self->owner - 1;
    } else {
        skip = -1;
    }
    // childSlot[owner] = 1-based slot of an object owned by owner
    bzero(childSlot, sizeof(childSlot));
    for (i = 0; i < 25; i++) {
        if (movement_obj[i].type == 0) {
            continue;
        }
        if (movement_obj[i].attach_char != 0) {
            childSlot[movement_obj[i].attach_char - 1] = i + 1;
        }
    }
    if (skipWorld == 0 &&
        func_150AC9C0(originX, originY, originZ, dirX, dirY, dirZ, &surf, &out->tri, &hitX, &hitY, &hitZ, &dist, &surfIdx, &out->material, 0.0f) != 0 &&
        dist < maxDist) {
        out->dist = dist;
        out->hitType = 1;
        out->pos.x = hitX;
        out->pos.y = hitY;
        out->pos.z = hitZ;
        out->far = out->pos;
        if (surfIdx != -1) {
            out->surface = &db_Object_array[surfIdx];
        } else {
            out->surface = NULL;
        }
    }
    if (skipObjs == 0) {
        for (i = 0; i < 25; i++) {
            if ((1 << i) & ignoreMask) {
                continue;
            }
            if (skip == i) {
                continue;
            }
            obj = &movement_obj[i];
            if (obj->type == 0) {
                continue;
            }
            if (obj->drawtype == 3) {
                continue;
            }
            if (obj == self) {
                continue;
            }
            if (!(obj->noshove & 0x40)) {
                continue;
            }
            if (filter != -1 && gObjectFilters[filter](obj, self, filterArg) == 0) {
                continue;
            }
            if (skipModels == 0) {
                ret = collision_rayHitSpheres(obj, &origin, &dir, &near, &far, &centre, &dist, &index, &bone, &part, offset);
                if (ret > 0 && dist < maxDist && (out->dist == -1.0f || dist < out->dist) && best != childSlot[i]) {
                    out->dist = dist;
                    out->obj = obj;
                    best = i + 1;
                    out->part = part;
                    out->bone = bone;
                    out->pos = near;
                    out->centre = centre;
                    out->index = index;
                    out->far = far;
                    out->hitType = 3;
                }
                if (ret != -1) {
                    continue;
                }
            }
            if (collision_rayCylinder(&origin, &dir, obj, &near, &far, &dist, &farDist) != 0 &&
                dist < maxDist && (out->dist == -1.0f || dist < out->dist) && best != childSlot[i]) {
                out->dist = dist;
                out->obj = obj;
                best = i + 1;
                out->pos = near;
                out->far = far;
                out->hitType = 2;
            }
        }
    }
}

/* Intersects a ray with obj's collision cylinder (radius unkD2, centre raised by unkD6, y squashed by
   unkDC). Returns 1 with the entry/exit points and their distances from origin on a hit. */
s32 collision_rayCylinder(Vec3f *origin, Vec3f *dir, Actor *obj, Vec3f *outNear, Vec3f *outFar, f32 *outNearDist, f32 *outFarDist) {
    Vec3f pos;

    pos.x = obj->xpos;
    pos.y = obj->ypos + obj->unkD6;
    pos.z = obj->zpos;
    {
        Vec3f start;
        Vec3f unitDir;
        Vec3f center;
        f32 yScale;
        f32 yUnscale;

        yScale = obj->unkDC;
        yUnscale = obj->unkE0;
        start.x = origin->x;
        start.y = origin->y * yScale;
        start.z = origin->z;
        unitDir.x = dir->x;
        unitDir.y = dir->y * yScale;
        unitDir.z = dir->z;
        if (math_vec3Normalize(&unitDir, &unitDir, NULL, NULL) == 0) {
            return 0;
        }
        center.x = pos.x;
        center.y = pos.y * yScale;
        center.z = pos.z;
        if (math_raySphere(&start, &unitDir, &center, obj->unkD2, outNear, outFar, outNearDist, outFarDist) == 0) {
            return 0;
        }
        outNear->y *= yUnscale;
        outFar->y *= yUnscale;
        {
            Vec3f diff;

            diff.x = outNear->x - origin->x;
            diff.y = outNear->y - origin->y;
            diff.z = outNear->z - origin->z;
            *outNearDist = math_vec3Length(&diff);
            diff.x = outFar->x - origin->x;
            diff.y = outFar->y - origin->y;
            diff.z = outFar->z - origin->z;
            *outFarDist = math_vec3Length(&diff);
        }
    }
    return 1;
}

void func_15081E0C(Actor *obj, u16 part, u8 skip) {
    if (gIsMultiplayer == 0) {
        if (obj->energy != 0) {
            obj->energy = 7;
        }
        if ((skip == 0) || (part != 10)) {
            func_1507DF10(obj, part);
        }
    }
}

/* Applies the effects of a ray hit of the given kind fired by self: world impact effects for a world
   hit, otherwise hit reactions, sounds and effects on the object that was hit. Returns 0 for no hit,
   1 for a world hit and 2 for an object hit. */
s32 collision_applyRayHit(Actor *self, RayHit *hit, s32 kind) {
    s32 sound;
    f32 yaw;
    f32 pitch;
    u8 playImpact;
    Actor *obj;
    ActorCtl *ext;
    // matching: volatile keeps IDO from hoisting the li/lw of noHealth out of the if/else
    volatile s32 noHealth;
    s32 longShot;
    s32 effect;
    s32 spawnEffect;
    s32 pad;

    playImpact = 1;
    ext = self->mv;
    if (ext != NULL) {
        ext->shotsfired++;
    }
    if (hit->hitType == 0) {
        return 0;
    }
    if (hit->hitType == 1) {
        switch (kind) {
        case 18:
            effect = 4;
            break;
        case 6:
            effect = 0;
            break;
        case 2:
            effect = 1;
            break;
        case 7:
            effect = 5;
            break;
        case 11:
            effect = 3;
            break;
        case 12:
            effect = 2;
            break;
        case 13:
            effect = -1;
            effect_spawnRaySurfaceImpact(&hit->tri, &hit->pos, &hit->dir, 0xFF, 1);
            break;
        case 5:
            if (level == 14) {
                f32 vc;
                f32 front[3];
                f32 back[3];

                front[0] = hit->pos.x + hit->dir.x * 5.0f;
                front[1] = hit->pos.y + hit->dir.y * 5.0f;
                front[2] = hit->pos.z + hit->dir.z * 5.0f;
                back[0] = hit->pos.x - hit->dir.x * 5.0f;
                back[1] = hit->pos.y - hit->dir.y * 5.0f;
                back[2] = hit->pos.z - hit->dir.z * 5.0f;
                func_150EB484((Vec3f *)front, (Vec3f *)back, (Vec3f *)&vc);
            }
            // fallthrough
        case 9:
        case 10:
            effect = -1;
            func_15104608(hit->pos.x, hit->pos.y, hit->pos.z, self->owner);
            actor_runHitHandler(self, 2, NULL);
            break;
        default:
            effect = 5;
            break;
        }
        if (effect != -1) {
            particle_spawnHitImpact(&hit->pos, &hit->dir, &hit->tri, effect, 0xFF, 1);
        }
        return 1;
    }
    obj = hit->obj;
    if (kind == 5 || kind == 9 || kind == 10) {
        actor_runHitHandler(self, 3, NULL);
    }
    if (obj->energy == 0) {
    // matching: an (unused) label here stops IDO hoisting the store out of the then-block
    noHealthLabel:
        noHealth = 1;
    } else {
        noHealth = 0;
    }
    if ((obj->imune != 0 || obj->stunned != 0) && noHealth == 0) {
        return 2;
    }
    longShot = 0;
    sound = gProjectileHitValues[kind];
    if (kind == 7) {
        if (level == 43) {
            sound = 0x27;
        }
    }
    math_getHeadingElevation(&hit->dir, &yaw, &pitch);
    if (hit->hitType == 2) {
        if (obj->character_type == 0x94) {
            playImpact = 0;
            effect_spawnLargeObjBurstAt(&hit->pos, 0xFF, 1);
            effect_flashViewLightBlue(0xFF, 1);
        }
    } else if (hit->hitType == 3) {
        if (gIsMultiplayer == 0) {
            if (obj->character_type == 0x9F || obj->character_type == 0xA0) {
                if (1000.0f < hit->dist) {
                    longShot = 1;
                }
            }
        }
        if (hit->part == 10 && (obj->mv == NULL || obj->mv->unk1AD == 0) && longShot == 0) {
            // matching: unsigned compares keep the constants as immediates
            if ((u32)kind != 2 && (u32)kind != 18 && (u32)kind != 11 && (u32)kind != 12) {
                func_1507DE4C(obj);
                if (obj->mv != NULL) {
                    obj->mv->unk1AD = 1;
                }
                if (obj->character_type == 0x80 && hit->index == 0 && hit->pos.y - 5.5f > hit->centre.y) {
                    actor_playUntrackedSound((func_150ADA20() & 7) + 0x2E3, obj);
                }
                blood_spawnFatalHit(obj, 0xFF, 1);
                sound = 0x20;
                if (obj->character_type == 0x9F || obj->character_type == 0xA0) {
                    if (obj->taskint[2] != 0) {
                        sound = 0x3D;
                    }
                }
                actor_playSound(0xAA, obj);
            } else {
                actor_playSound(0x107, obj);
            }
            if (kind != 11 && kind != 12 && obj->energy != 0) {
                obj->energy = 1;
            }
        } else if (obj->character_type == 0x9F || obj->character_type == 0xA0) {
            func_15081E0C(obj, hit->part, longShot);
        }
        switch (kind) {
        case 2:
            spawnEffect = 0x2A;
            break;
        case 18:
            spawnEffect = 0x22;
            break;
        case 12:
            spawnEffect = 0x6B;
            break;
        case 11:
            spawnEffect = 0x6D;
            break;
        default:
            spawnEffect = 0;
            break;
        }
        if (spawnEffect != 0 && obj->energy < 2) {
            Actor source;

            source.xpos = hit->pos.x;
            source.ypos = hit->pos.y;
            source.zpos = hit->pos.z;
            source.vtilt = pitch;
            source.theta = yaw + 180.0f;
            source.lean_angle = 0.0f;
            source.scale_xz = obj->scale_xz;
            source.scale_y = obj->scale_y;
            attach_attachInPlace(&source, obj, spawnEffect, hit->bone, 1, 0);
            if (hit->part == 10) {
                sound = 0x20;
            }
        }
    }
    if (playImpact) {
        blood_spawnActorImpact(hit->obj, &hit->pos, &hit->far, &hit->dir, 0xFF, 1);
    }
    if (kind == 13) {
        effect_spawnRayHitDebris(&hit->pos, 0xFF, 1);
    }
    if (noHealth == 0) {
        actor_hitScripted(obj, sound | 0x80000, (yaw - 90.0f) * 182.04444885f, self - movement_obj);
    }
    return 2;
}
