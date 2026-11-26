#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Trail types (see game/Trail/MotionTrail/motiontrail.c). */

/* Creates a spray emitter from desc; bit 0 of its flags records whether arg1 was set. */
void trail_createSprayEmitter(SprayDesc *desc, u8 arg1) {
    SprayEmitter *emitter;
    s32 flag;

    emitter = effect_newObject(0x1A, 1, sizeof(SprayEmitter), 1, 0xFF, 1);
    if (emitter != NULL) {
        memcpy(&emitter->desc, desc, sizeof(SprayDesc));
        collision_selectMap(0);
        emitter->zone = collision_findZoneLeaf(emitter->desc.origin.x + (emitter->desc.extent.x * 0.5f),
                                      emitter->desc.origin.z + (emitter->desc.extent.z * 0.5f));
        if (arg1 != 0) {
            flag = 1;
        } else {
            flag = 0;
        }
        emitter->flags = flag;
        emitter->idleTimer = 0;
        emitter->pending = 0.0f;
        if (emitter->desc.extent.y < 0.0f) {
            emitter->floorY = emitter->desc.origin.y + emitter->desc.extent.y;
        } else {
            emitter->floorY = emitter->desc.origin.y;
        }
    }
}

/* Spray emitter update: every 50 ticks checks for a player within 1581 units (sqrt 2500000) and, while one
   was found, spawns about desc.rate trail particles per frame along the spawn segment. */
void trail_updateSprayEmitter(SprayEmitter *emitter) {
    s32 i;
    s32 found;
    f32 *playerPos; /* unk2F8..unk300 of the player: its position */
    f32 dx;
    f32 dy;
    f32 dz;
    s32 player;
    u8 pad[4]; // matching: frame size
    EffectTemplate spawn;
    TrailMotion motion;
    f32 floorY;
    TrailRender render;
    SprayTrail *trail;
    SprayTrailParams *params;

    if ((emitter->zone != NULL) && (math_isZoneHiddenFromViews(emitter->zone) != 0)) {
        return;
    }
    i = gScreenLayout; // matching: dead store makes uopt color the loop bound before the timer reload
    emitter->idleTimer -= D_800BE9E4;
    if (emitter->idleTimer < 0) {
        emitter->idleTimer += 50;
        found = FALSE;
        for (i = 0; (i <= gScreenLayout) && !found; i++) {
            player = gViewPlayers[i];
            playerPos = &camera_global_Cameras[player].eye.x;
            dx = playerPos[0] - emitter->desc.origin.x;
            dy = playerPos[1] - emitter->desc.origin.y;
            dz = playerPos[2] - emitter->desc.origin.z;
            if (((dx * dx) + (dy * dy) + (dz * dz)) < 2500000.0f) {
                found = TRUE;
            }
        }
        if (!found) {
            return;
        }
    }
    emitter->pending += (emitter->desc.rate + (math_randomFloat() * emitter->desc.rateRange)) * gFrameDelta;
    if (emitter->pending > 1.0f) {
        floorY = emitter->floorY;
        spawn.kind = 1;
        spawn.flags = 5;
        motion.unk1C = 16;
        motion.unk1E = 15;
        motion.unk14 = -10000.0f;
        motion.flags = 0x48;
        motion.texture = 3;
        motion.shade = 0xFF;
        render.unk0 = 0;
        render.setGeometryMode = 1;
        render.clearGeometryMode = 0x160600;
        render.renderMode = 3;
        render.unk10 = 0x22;
        render.unk14 = 0x80;
        render.unk18 = 0x20;
        render.primMode = 0;
        render.envMode = 7;
        do {
            spawn.itemCount = (math_random() % (emitter->desc.pointCountRange + 1)) + emitter->desc.pointCount;
            {
                f32 t;
                t = math_randomFloat();
                spawn.pos.x = emitter->desc.origin.x + (t * emitter->desc.extent.x);
                spawn.pos.y = emitter->desc.origin.y + (t * emitter->desc.extent.y);
                spawn.pos.z = emitter->desc.origin.z + (t * emitter->desc.extent.z);
            }
            motion.halfWidth = (math_randomFloat() * emitter->desc.halfWidthRange) + emitter->desc.halfWidth;
            {
                f32 speed;
                s16 yaw;
                s16 pitch;
                f32 sinYaw;
                f32 cosYaw;
                f32 sinPitch;
                f32 cosPitch;
                // matching: IDO needs the double negation here; `+ yawSpread` does not match
                yaw = emitter->desc.yaw - (math_random() % ((emitter->desc.yawSpread * 2) + 1)) - -emitter->desc.yawSpread;
                pitch = (math_random() % (emitter->desc.pitchRange + 1)) + emitter->desc.pitch;
                speed = (math_randomFloat() * emitter->desc.speedRange) + emitter->desc.speed;
                sinYaw = math_cosBinAngleMasked(yaw);
                cosYaw = math_cosBinAngleMasked(yaw - 0x40);
                sinPitch = math_cosBinAngleMasked(pitch);
                cosPitch = math_cosBinAngleMasked(pitch - 0x40);
                motion.velocity.x = (speed * sinPitch) * cosYaw;
                motion.velocity.y = -speed * cosPitch;
                motion.velocity.z = (speed * sinPitch) * sinYaw;
            }
            motion.gravity = (math_randomFloat() * emitter->desc.gravityRange) + emitter->desc.gravity;
            motion.alpha = (math_random() % (emitter->desc.unk1BRange + 1)) + emitter->desc.unk1B;
            spawn.life = (math_random() % (emitter->desc.unkCRange + 1)) + emitter->desc.unkC;
            trail = trail_createMotionTrail(&spawn, &motion, 8, 1, (emitter->flags & 1) ? 10 : 0, 0, 0, 0, 0, 0, 0, &render, 0,
                                  emitter->link.owner, emitter->link.kind);
            if (trail != NULL) {
                params = trail->params;
                memcpy(&params->floorY, &floorY, sizeof(floorY));
            }
            emitter->pending -= 1.0f;
        } while (emitter->pending > 1.0f);
    }
}

/* Spray trail handler: once the oldest point falls below the floor, maybe splashes there, then ends the trail. */
s32 trail_landSprayTrail(SprayTrail *trail) {
    f64 chance;
    SprayTrailParams *params = trail->params;
    MotionTrailPoint *points = trail->points;
    Vec3f splashPos;

    if (points[trail->head.start].pos.y < params->floorY) {
        chance = math_randomFloat();
        if (chance < 0.8) {
            splashPos.x = points[trail->head.start].pos.x;
            splashPos.y = params->floorY;
            splashPos.z = points[trail->head.start].pos.z;
            particle_spawnKindMark(particle_getContextEffectKind(), params->base.motion.halfWidth * 6.0f, params->base.motion.alpha, 0,
                          &splashPos, trail->head.header.owner, trail->head.header.kind);
        }
        params->base.state = 4;
        params->floorY = -10000.0f;
    }
    return 1;
}

/* Empty handler with four arguments. */
s32 func_15189FD0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    (void)&arg0; // matching: taking an argument's address makes IDO home all four argument registers
    return 1;
}
