#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "libc/string.h"

/* Spawns 25-35 particles on a ring around (111, -90, 0) in placement space, flying outward from its center. */
void particle_spawnPlacementRing(WorldObject *placement, u8 arg1, s32 arg2) {
    s16 count;
    ParticleDesc spawn;
    s16 remaining;
    Vec3f *points;
    s16 i;
    f32 drag;
    Vec3f *ring;

    count = remaining = math_random() % 11U + 25;
    if (count > 0) {
        ring = heap_alloc((count + 1) * sizeof(Vec3f), 3, 1, 0, 1);
        points = heap_alloc((count + 1) * sizeof(Vec3f), 3, 1, 0, 1);
        if (ring == NULL || points == NULL) {
            return;
        }
        for (i = 0; i < count; i++) {
            math_dirFromAngleXZ(math_randomFloat() * 3.141592741f + 3.141592741f, 114.0f, &ring[i].x, &ring[i].z);
            ring[i].x += 111.0f;
            ring[i].y = -90.0f;
        }
        i = 0;
        ring[count].x = 111.0f;
        ring[count].y = -90.0f;
        ring[count].z = 0.0f;
        math_transformObjectPoints(placement, ring, points, count + 1);
        spawn.texture = 0x6C;
        spawn.renderMode = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.r = 0xE7;
        spawn.g = 0xE7;
        spawn.b = 0xB6;
        spawn.r2 = 0x9E;
        spawn.g2 = 0x9E;
        spawn.b2 = 0x62;
        spawn.unk4 = 0;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.a = 0xFF;
        spawn.brightness = 0xFF;
        spawn.offset.x = 0.0f;
        spawn.offset.y = 0.0f;
        spawn.offset.z = 0.0f;
        spawn.fadeStart = 25;
        spawn.fadeRate = 10;
        spawn.flags = 0x80CE07;
        spawn.primColorMode = 8;
        spawn.envColorMode = 6;
        spawn.updateFunc = 16;
        spawn.deathFunc = -1;
        spawn.drawFunc = -1;
        spawn.handlerSet = 0;
        spawn.scaleRate = 1.040568948f;
        drag = 0.8533809781f;
        do { // matching: a for loop's entry test changes register allocation; count > 0 already holds
            f32 speed;
            s32 flipT;
            SpriteParticle *particle;

            spawn.flags &= ~(PARTICLE_FLIP_S | PARTICLE_FLIP_T);
            flipT = (math_random() & 1) ? PARTICLE_FLIP_T : 0;
            spawn.flags |= ((math_random() & 1) ? PARTICLE_FLIP_S : 0) | flipT;
            speed = (math_randomFloat() * 202.0f + 250.0f) * 0.001000000047f;
            spawn.scaleStart = spawn.timer = math_random() % 26U + 25;
            spawn.width = spawn.height = math_randomFloat() * 105.0f + 150.0f;
            spawn.pos = points[i];
            spawn.vel.x = (points[i].x - points[count].x) * speed;
            spawn.vel.y = (points[i].y - points[count].y) * speed;
            spawn.vel.z = (points[i].z - points[count].z) * speed;
            spawn.gravity = (math_randomFloat() * 880.0f + 13.0f) * 0.001000000047f;
            spawn.alpha = math_random() % 176U + 80;
            particle = particle_newSpriteParticleWithFloor(&spawn, 1, 0, sizeof(particle->data.drag), arg1, arg2);
            if (particle != NULL) {
                memcpy(&particle->data.drag, &drag, sizeof(drag));
            }
            remaining--;
            i++;
        } while (remaining > 0);
    }
}
