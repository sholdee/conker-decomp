#include <ultra64.h>
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/save.h"
#include "conker/zone.h"

/*
 * Called when the actor has moved from (prevX, prevZ): casts a ray from 80 units above it back towards
 * the previous position and, on a hit, spawns a burst of particles off the hit triangle (plus a
 * sprite when math_isObjectHidden accepts the hit object). First runs an anti-tamper checksum over the code
 * from save_checkCodeTamper to save_spawnMenuCharacter (a mismatch breaks player 1's controller mapping). Nothing is spawned while zone_isInNeutralZone(2 or 1, position) reports anything.
 */
void particle_spawnMoveHitBurst(Actor *actor, f32 prevX, f32 prevZ, u8 arg3, s32 arg4) {
    s32 pad[3]; // matching: unused locals of the original; the frame is 16 bytes larger than the named ones need
    u32 addr;
    u32 end;
    s32 sum;

    if (actor == NULL) {
        return;
    }
    sum = 0;
    end = (u32)save_spawnMenuCharacter;
    for (addr = (u32)save_checkCodeTamper; addr < end; addr += sizeof(s32)) {
        sum += ~*(s32 *)addr;
    }
    if (sum != 0x3843095E) {
        gPlayerChannels[0] = 0xFF;
    }
    if ((zone_isInNeutralZone(2, actor->xpos, actor->ypos, actor->zpos) == 0) &&
        (zone_isInNeutralZone(1, actor->xpos, actor->ypos, actor->zpos) == 0)) {
        f32 rayY = actor->ypos + 80.0f;
        f32 dz;
        f32 dy;
        f32 dx;
        Vec3f hit;
        CollisionTri triangle;
        s32 hitObject;
        SpriteDef sprite;
        SurfaceBurst burst;

        /* Ray back along the move, i.e. minus the displacement since (prevX, prevZ). */
        dx = -(actor->xpos - prevX);
        dy = 0.0f;
        dz = -(actor->zpos - prevZ);
        if (collision_castRayWorld(actor->xpos, rayY, actor->zpos, dx, dy, dz, NULL, &triangle, &hit.x,
                          &hit.y, &hit.z, NULL, &hitObject, NULL, 0.0f) == 0) {
            return;
        }
        if (math_isObjectHidden(hitObject) != 0) {
            sprite.texture = 6;
            sprite.handlerSet = 0;
            sprite.flags = 0x9F01;
            sprite.lifetime = (math_random() % 51U) + 100;
            sprite.frame = 0;
            sprite.frameSpeed = 0;
            sprite.alpha = (math_random() % 101U) + 155;
            sprite.colorLevel = 0xFF;
            sprite.rgba[0] = 0xFF;
            sprite.rgba[1] = 0xFF;
            sprite.rgba[2] = 0xFF;
            sprite.rgba[3] = 0xFF;
            sprite.renderModes = 0x3B0002;
            sprite.primMode = 0;
            sprite.envMode = 7;
            sprite.fadeStart = 25;
            sprite.fadeRate = 10;
            particle_spawnSurfaceQuadSprite(&sprite, 0, 0, &triangle, hit.x, hit.y, hit.z,
                          (math_randomFloat() * 20.0f) + 40.0f, (math_randomFloat() * 20.0f) + 40.0f, math_random() & 0xFF,
                          ((math_random() & 1) * 2) + (math_random() & 1), 3, 0xFF, 0, arg3, arg4);
        }
        burst.countBase = 15;
        burst.countRange = 10;
        burst.position = hit;
        burst.normal.x = -dx;
        burst.normal.y = -dy;
        burst.normal.z = -dz;
        burst.triangle = triangle;
        burst.spread = 675.0f;
        burst.scaleBase = 7.0f;
        burst.scaleRange = 10.0f;
        burst.unk4C = -1.022000074f;
        burst.unk50 = 0.5420000553f;
        burst.texture = 7;
        burst.renderModes = 0x3B01;
        burst.type = 0;
        burst.lifetimeBase = 26;
        burst.lifetimeRange = 20;
        burst.flags = 0x5C0001;
        burst.unk44Base = 100;
        burst.unk44Range = 155;
        burst.unk46 = 0;
        burst.unk47 = 7;
        burst.unk60 = 3;
        burst.unk64 = 0xFF;
        burst.fadeStart = 10;
        burst.fadeRate = 25;
        burst.speedBase = 0.0374f;
        burst.speedRange = 0.0235f;
        burst.unk6C = 0.940404f;
        burst.spin = 1.084000111f;
        particle_spawnSurfaceBurst(&burst, arg3, arg4);
    }
}

/* Fades the sprite out over its last 64 frames (def.alpha = 4 * lifetime). */
s32 particle_fadeMoveHitSprite(QuadSprite *sprite) {
    s16 lifetime = sprite->def.lifetime;

    if (lifetime < 64) {
        sprite->def.alpha = lifetime * 4;
    }
    return 1;
}

/* Moves the particle by its velocity and spins it, under gravity of -0.6 per frame. */
s32 particle_moveTumbling(TumblingParticle *particle) {
    particle->motion.vel.y += -0.6f * gFrameDelta;
    particle->head.def.pos.x += particle->motion.vel.x * gFrameDelta;
    particle->head.def.pos.y += particle->motion.vel.y * gFrameDelta;
    particle->head.def.pos.z += particle->motion.vel.z * gFrameDelta;
    particle->head.def.rot.x += particle->motion.rotVelX * gFrameDelta;
    particle->head.def.rot.y += particle->motion.rotVelY * gFrameDelta;
    return 1;
}

/*
 * Fades the particle with whichever is smaller of 16 * lifetime and 4 * its height above
 * motion.minY (capped at 255). Returns 0 (expire) once it falls below minY.
 */
s32 particle_fadeTumbling(TumblingParticle *particle) {
    f32 height = particle->head.def.pos.y - particle->motion.minY;
    s32 lifeAlpha;
    s32 heightAlpha;

    if (height < 0.0f) {
        return 0;
    }

    lifeAlpha = particle->head.def.lifetime * 16;
    if (lifeAlpha >= 256) {
        lifeAlpha = 255;
    }

    heightAlpha = (s32)height * 4;
    if (heightAlpha >= 256) {
        heightAlpha = 255;
    }

    if (heightAlpha < lifeAlpha) {
        particle->head.def.alpha = heightAlpha;
    } else {
        particle->head.def.alpha = lifeAlpha;
    }

    // matching: never true for a u8, but the original tests it (IDO reloads the stored byte)
    if (particle->head.def.alpha < 0) {
        return 0;
    }
    return 1;
}
