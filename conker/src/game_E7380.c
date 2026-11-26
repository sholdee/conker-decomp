#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Called when the actor has moved from (prevX, prevZ): casts a ray from 80 units above it back towards
 * the previous position and, on a hit, spawns a burst of particles off the hit triangle (plus a
 * sprite when func_15145C90 accepts the hit object). First runs an anti-tamper checksum over the code
 * from func_151E7EF8 to func_151E7F60 (a mismatch breaks player 1's controller mapping). Nothing is spawned while func_150A3058(2 or 1, position) reports anything.
 */
void func_150B9ED0(Actor *actor, f32 prevX, f32 prevZ, u8 arg3, s32 arg4) {
    s32 pad[3]; // matching: unused locals of the original; the frame is 16 bytes larger than the named ones need
    u32 addr;
    u32 end;
    s32 sum;

    if (actor == NULL) {
        return;
    }
    sum = 0;
    end = (u32)func_151E7F60;
    for (addr = (u32)func_151E7EF8; addr < end; addr += sizeof(s32)) {
        sum += ~*(s32 *)addr;
    }
    if (sum != 0x3843095E) {
        D_80084060[0] = 0xFF;
    }
    if ((func_150A3058(2, (s16)actor->xpos, (s16)actor->ypos, (s16)actor->zpos) == 0) &&
        (func_150A3058(1, (s16)actor->xpos, (s16)actor->ypos, (s16)actor->zpos) == 0)) {
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
        if (func_150AC9C0(actor->xpos, rayY, actor->zpos, dx, dy, dz, NULL, &triangle, &hit.x,
                          &hit.y, &hit.z, NULL, &hitObject, NULL, 0.0f) == 0) {
            return;
        }
        if (func_15145C90(hitObject) != 0) {
            sprite.texture = 6;
            sprite.unk7 = 0;
            sprite.flags = 0x9F01;
            sprite.lifetime = (func_150ADA20() % 51U) + 100;
            sprite.frame = 0;
            sprite.frameSpeed = 0;
            sprite.unk10 = (func_150ADA20() % 101U) + 155;
            sprite.unk11 = 0xFF;
            sprite.rgba[0] = 0xFF;
            sprite.rgba[1] = 0xFF;
            sprite.rgba[2] = 0xFF;
            sprite.rgba[3] = 0xFF;
            sprite.renderModes = 0x3B0002;
            sprite.unk16 = 0;
            sprite.unk17 = 7;
            sprite.fadeStart = 25;
            sprite.fadeRate = 10;
            func_1513C650(&sprite, 0, 0, (s32)&triangle, hit.x, hit.y, hit.z,
                          (func_150ADA68() * 20.0f) + 40.0f, (func_150ADA68() * 20.0f) + 40.0f, func_150ADA20() & 0xFF,
                          ((func_150ADA20() & 1) * 2) + (func_150ADA20() & 1), 3, 0xFF, 0, arg3, arg4);
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
        func_151511FC(&burst, arg3, arg4);
    }
}

/* Fades the sprite out over its last 64 frames (def.unk10 = 4 * lifetime). */
s32 func_150BA35C(QuadSprite *sprite) {
    s16 lifetime = sprite->def.lifetime;

    if (lifetime < 64) {
        sprite->def.unk10 = lifetime * 4;
    }
    return 1;
}

/* Moves the particle by its velocity and spins it, under gravity of -0.6 per frame. */
s32 func_150BA37C(TumblingParticle *particle) {
    particle->motion.vel.y += -0.6f * D_800BE9A4;
    particle->head.def.pos.x += particle->motion.vel.x * D_800BE9A4;
    particle->head.def.pos.y += particle->motion.vel.y * D_800BE9A4;
    particle->head.def.pos.z += particle->motion.vel.z * D_800BE9A4;
    particle->head.def.rot.x += particle->motion.rotVelX * D_800BE9A4;
    particle->head.def.rot.y += particle->motion.rotVelY * D_800BE9A4;
    return 1;
}

/*
 * Fades the particle with whichever is smaller of 16 * lifetime and 4 * its height above
 * motion.minY (capped at 255). Returns 0 (expire) once it falls below minY.
 */
s32 func_150BA424(TumblingParticle *particle) {
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
        particle->head.def.unk44 = heightAlpha;
    } else {
        particle->head.def.unk44 = lifeAlpha;
    }

    // matching: never true for a u8, but the original tests it (IDO reloads the stored byte)
    if (particle->head.def.unk44 < 0) {
        return 0;
    }
    return 1;
}
