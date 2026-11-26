#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800AAA30; /* 0.1f; ROM 0x24F4F0, just before this file's .rodata slice */

/* Queues a positional sound event (func_1000E7A0 kind 8) for soundId, only in a few specific levels. */
void func_151C2050(Actor *obj, Vec3f *pos, Vec3f *dir, s32 soundId, f32 distance) {
    s32 pan;
    s32 volume;
    Camera *camera;
    s32 direction;
    s16 song;
    f32 volumeLevel; // matching: separate spill slot for the value kept across func_1000B060
    f32 intensity;

    if (obj != NULL) {
        if (D_800BE616 || (camera = obj->CameraPtr) == NULL) {
            song = D_800B0DF0->music.song;
            if (song == 0x24 || song == 0x13 || song == 0x4D || song == 0x85 || song == 0x93) {
                func_100114D0((s32)pos->x, (s32)pos->y, (s32)pos->z, 0x7FFF, 1000, 100, &pan, &volume, NULL);
                if ((u32)volume > 0x1000) {
                    volume = (u32)volume >> 7;
                    func_1000E7A0(8, (func_1510F8CC(soundId) + 1) | (volume << 8) | (pan << 16));
                }
            }
        } else {
            song = D_800B0DF0->music.song;
            if (song == 0x24 || song == 0x13) {
                // matching: the 0.1f literal (vs this rodata load) shifts the stack frame
                intensity = 255.0f - distance * D_800AAA30;
                if (intensity > 16.0f) {
                    volumeLevel = intensity;
                    /* 40.74 = 256 / 2pi: radians to a 256-step angle */
                    direction = func_1000B060(dir->x, dir->z, (u32)(camera->TrueCameraAngleRad * 40.74366379f));
                    func_1000E7A0(8, (func_1510F8CC(soundId) + 1) | ((s32)volumeLevel << 8) | (direction << 16));
                }
            }
        }
    }
}

/* TrailExt flags */
#define TRAIL_HIT_CALLBACK 1    /* call D_800E0940 on expiry */
#define TRAIL_EXPIRE_HANDLER 2  /* call D_8008FBD0[expireHandler] on expiry */
#define TRAIL_COLLIDE 4
#define TRAIL_FLAG_8 8

/* Expiry handlers, chosen by Game1EF500TrailExt.expireHandler (entry 0 is func_151C2F48). */
typedef void (*TrailExpireHandler)(Game1EF500Trail *);
/* Signature of the hook in D_800E0940: (hit position, index of the WorldObject that was hit). */
typedef void (*TrailHitCallback)(Vec3f *, s32);

extern TrailExpireHandler D_8008FBD0[];

typedef struct {
    u16 sound;
    u16 range;
} FlybySound;

extern FlybySound D_8008FBE8[];
void *memcpy(void *, const void *, u32);

/*
 * Spawns a projectile at origin heading along direction (or from altOrigin along altDirection),
 * sets up its path and runs its first update of dt. Returns NULL if it could not be placed or
 * if that first update already ended it.
 */
Game1EF500Trail *func_151C229C(Vec3f *origin, Vec3f *direction, Vec3f *altOrigin, Vec3f *altDirection,
                               u8 altNormalized, RayHit *knownHit, f32 speed, f32 arg7, f32 arg8,
                               f32 driftSpeed, f32 arg10, u8 arg11, Actor *owner, u8 arg13, u8 collide, u8 arg15,
                               u8 soundChance, u8 hitCallback, u8 expireHandler, s8 handlerIndex, s32 arg20,
                               f32 dt, u8 arg22, s8 arg23, s32 extraSize, u8 arg25, s32 arg26) {
    Game1EF500Trail *result;
    ParticleDef spawn;
    Game1EF500TrailExt ext;
    Vec3f sourceDir;
    u8 useSource;
    Vec3f targetDir;
    u8 useTarget;
    s32 unused; // matching: unused stack slot in the original frame
    Vec3f *from;
    Vec3f *dir;

    useSource = FALSE;
    useTarget = altNormalized && altDirection != NULL;
    if (useTarget) {
        targetDir = *altDirection;
    }
    ext.flags = (hitCallback ? TRAIL_HIT_CALLBACK : 0) | (expireHandler ? TRAIL_EXPIRE_HANDLER : 0) |
                (collide ? TRAIL_COLLIDE : 0) | (arg15 ? TRAIL_FLAG_8 : 0);
    ext.handle = 0;
    ext.fieldA8 = arg23;
    ext.expireHandler = handlerIndex;
    ext.fieldA4 = arg20;
    ext.soundEnabled = (func_150ADA20() & 0xFFU) < soundChance ? TRUE : FALSE;
    ext.sound = 0;
    ext.listener = 0;
    if (altOrigin != NULL && altDirection != NULL) {
        if (!useTarget) {
            if (func_15145128(&altDirection->x, &targetDir.x, NULL, NULL) != 0) {
                useTarget = TRUE;
            } else {
                return NULL;
            }
        }
        from = altOrigin;
        dir = &targetDir;
    } else {
        if (direction != NULL) {
            if (func_15145128(&direction->x, &sourceDir.x, NULL, NULL) != 0) {
                useSource = TRUE;
            } else {
                return NULL;
            }
        }
        from = origin;
        dir = &sourceDir;
    }
    ext.handle = collide ? func_151D5B6C(from, dir, owner, arg23, 0) : -1;
    if (func_151C4B0C(knownHit, &ext.path, origin, useSource ? &sourceDir : NULL, &ext.remaining, arg7, arg13,
                      altOrigin, useTarget ? &targetDir : NULL, &ext.flags, 3, arg10, owner, arg23, collide, arg15,
                      ext.handle, 0) == 0) {
        return NULL;
    }
    ext.velocity.x = ext.path.dir.x * speed;
    ext.velocity.y = ext.path.dir.y * speed;
    ext.velocity.z = ext.path.dir.z * speed;
    ext.accel.x = 0.0f;
    ext.accel.y = 0.0f;
    ext.accel.z = 0.0f;
    ext.drift.x = ext.path.dir.x * driftSpeed;
    ext.drift.y = ext.path.dir.y * driftSpeed;
    ext.drift.z = ext.path.dir.z * driftSpeed;
    ext.owner = owner;
    ext.ownerId = (owner != NULL) ? owner->unique_id : 99;
    spawn.texture = 0x41;
    ext.scale = speed;
    spawn.type = 4;
    spawn.renderModes = 0x4503;
    spawn.lifetime = 300;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.rgba[0] = 0;
    spawn.rgba[1] = 0;
    spawn.rgba[2] = 0;
    spawn.rgba[3] = 0xFF;
    spawn.unk44 = arg11;
    spawn.unk45 = 0xFF;
    spawn.height = driftSpeed;
    spawn.width = arg8;
    spawn.pos = *origin;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.scale.x = 0.0f;
    spawn.scale.y = 0.0f;
    spawn.scale.z = 0.0f;
    spawn.flags = 0xC04C0008;
    spawn.unk46 = 0;
    spawn.unk47 = 0;
    spawn.viewportMask = arg22;
    result = func_1513D2F0(&spawn, &D_800A4AA0, 0x16, 0, 0, 0x14, 0, 0, 0, extraSize + sizeof(ext), arg25, arg26);
    if (result != NULL) {
        u8 failed;

        failed = FALSE;
        memcpy(&result->trail, &ext, sizeof(ext));
        func_151C436C(result, &result->trail, dt);
        if (func_151C43E0(result, &result->trail, dt) == 0) {
            failed = TRUE;
        }
        func_151C4510(result, &result->trail, dt);
        if (func_151C455C(result, &result->trail, dt) == 0) {
            failed = TRUE;
        }
        if (failed) {
            func_1516972C(&result->head.header);
            return NULL;
        }
    }
    return result;
}

/*
 * Per-frame update of a projectile: moves it, checks collisions and expiry, and plays a fly-by
 * sound once it passes close in front of a player. Returns 0 once the projectile is finished.
 */
s8 func_151C2734(Game1EF500Trail *trail) {
    s8 alive;
    Game1EF500TrailExt *ext;
    Camera *player;
    s32 sound;
    s32 sfxIndex;
    s32 i;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dot;
    f32 dist;
    s32 volume;
    s32 pan;

    ext = &trail->trail;
    alive = TRUE;
    func_151C436C(trail, ext, D_800BE9E4);
    if (func_151C43E0(trail, ext, D_800BE9A4) == 0) {
        alive = FALSE;
    }
    func_151C4510(trail, ext, D_800BE9A4);
    if (func_151C455C(trail, ext, D_800BE9A4) == 0) {
        alive = FALSE;
    }
    if (ext->soundEnabled && alive == TRUE) {
        sound = 0;
        player = NULL;
        if (ext->sound == 0) {
            sfxIndex = func_150ADA20() & 7;
            for (i = 0; i <= D_80082FA0; i++) {
                player = &camera_global_Cameras[i];
                dx = player->eye.x - trail->head.def.pos.x;
                dy = player->eye.y - trail->head.def.pos.y;
                dz = player->eye.z - trail->head.def.pos.z;
                /* distance of the player ahead along the flight direction */
                dot = ext->path.dir.x * dx + dy * ext->path.dir.y + dz * ext->path.dir.z;
                if (dot < (u32)D_8008FBE8[sfxIndex].range * ext->scale &&
                    (D_8008FBE8[sfxIndex].range - 10) * ext->scale < dot) {
                    dist = sqrtf(dx * dx + dy * dy + dz * dz);
                    if (dist != 0.0f && dist < ext->remaining * ext->scale) {
                        dot /= dist;
                        if (0.8999999762f < dot) {
                            sound = D_8008FBE8[sfxIndex].sound;
                            ext->listener = i;
                            break;
                        }
                    }
                }
                player = NULL;
            }
        } else if (func_1000F3D0(ext->sound)) {
            player = &camera_global_Cameras[ext->listener];
        } else {
            ext->soundEnabled = FALSE;
        }
        if (player != NULL) {
            volume = 0x40;
            func_1000A420(trail->head.def.pos.x - player->eye.x, trail->head.def.pos.y - player->eye.y,
                          trail->head.def.pos.z - player->eye.z, player->TrueCameraAngle,
                          trail->head.def.pos.x - player->eye.x, trail->head.def.pos.y - player->eye.y,
                          trail->head.def.pos.z - player->eye.z, 10000, 9999, &volume, &pan, 0);
            if (sound != 0) {
                ext->sound = func_10010F30(sound, 0x6590, volume & 0x7F, 0, volume & 0x80);
            } else {
                func_1000F85C(ext->sound, 4, volume & 0x7F);
                func_1000F85C(ext->sound, 0x100, volume & 0x80);
            }
        }
    }
    return alive;
}

/*
 * Builds the projectile's tail for view index: a quad from its position back along accel, widened
 * by width perpendicular to the view and faded to zero alpha at the far end. Returns the 4
 * vertices, or NULL if no vertex buffer was available.
 */
Vtx *func_151C2AD0(Game1EF500Trail *trail, s16 index) {
    Vec3f start;
    Vec3f end;
    f32 dx, dy, dz;
    f32 nx, ny, nz;
    Vec3f mid;
    f32 unusedX, unusedY; // matching: unused locals that still take stack slots
    Game1EF500TrailExt *ext;
    f32 lengthSq;
    f32 scale;
    Vtx *vertices;
    Vtx *result;
    Vec3f *eye;
    u8 fresh;
    Vtx *unusedVtx; // matching: as above

    func_151D5D60(trail->head.vtxBuffers, index, 0x40, &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(trail->head.vtxBuffers[index], trail->head.vtxTemplate, sizeof(trail->head.vtxTemplate));
            memcpy(trail->head.vtxBuffers[index] + 4, trail->head.vtxTemplate, sizeof(trail->head.vtxTemplate));
        }
    } else {
        return NULL;
    }
    // matching: the loop wrapper gives IDO an extra block boundary that sets register and schedule order
    do { ext = &trail->trail; } while (0);
    eye = func_15144B34(index);
    start = trail->head.def.pos;
    end.x = trail->head.def.pos.x - ext->accel.x;
    end.y = trail->head.def.pos.y - ext->accel.y;
    end.z = trail->head.def.pos.z - ext->accel.z;
    dx = end.x - start.x;
    dy = end.y - start.y;
    dz = end.z - start.z;
    mid.x = (dx * 0.5f + start.x) - eye->x;
    mid.y = (dy * 0.5f + start.y) - eye->y;
    mid.z = (dz * 0.5f + start.z) - eye->z;
    /* side vector: tail direction x view direction */
    nx = dy * mid.z - mid.y * dz;
    ny = dz * mid.x - mid.z * dx;
    nz = dx * mid.y - mid.x * dy;
    lengthSq = nx * nx + ny * ny + nz * nz;
    if (lengthSq == 0.0f) {
        nz = ny = nx = 0.0f;
    } else {
        scale = trail->head.def.width / sqrtf(lengthSq);
        nx = nx * scale;
        ny = ny * scale;
        nz = nz * scale;
    }
    vertices->v.ob[0] = start.x - nx;
    vertices->v.ob[1] = start.y - ny;
    vertices->v.ob[2] = start.z - nz;
    vertices->v.cn[3] = trail->head.def.unk44;
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = end.x - nx;
    vertices->v.ob[1] = end.y - ny;
    vertices->v.ob[2] = end.z - nz;
    vertices->v.cn[3] = 0;
    vertices->v.flag = 0;
    vertices++;
    vertices->v.cn[3] = 0;
    vertices->v.flag = 0;
    vertices->v.ob[0] = end.x + nx;
    vertices->v.ob[1] = end.y + ny;
    vertices->v.ob[2] = end.z + nz;
    vertices++;
    vertices->v.ob[0] = start.x + nx;
    vertices->v.ob[1] = start.y + ny;
    vertices->v.ob[2] = start.z + nz;
    vertices->v.cn[3] = trail->head.def.unk44;
    vertices->v.flag = 0;
    return result;
}

/* Whether obj is a live object other than self (a ray-cast filter). */
s32 func_151C2E4C(Actor *obj, Actor *self) {
    if (obj == self) {
        return FALSE;
    }
    if (obj->type == 0) {
        return FALSE;
    }
    if (obj->character_type == 0xFF) {
        return FALSE;
    }
    return TRUE;
}

/* As func_151C2E4C, and obj's unk127 must also be set (not 0xFF). */
s32 func_151C2E94(Actor *obj, Actor *self) {
    if (obj == self) {
        return FALSE;
    }
    if (obj->type == 0) {
        return FALSE;
    }
    if (obj->character_type == 0xFF) {
        return FALSE;
    }
    if (obj->playernumber == 0xFF) {
        return FALSE;
    }
    return TRUE;
}

/* Impact handler: the projectile hit actor (at hitPos) during its ray cast. */
s32 func_151C2EF0(void *actor, Actor *owner, Game1EF500Trail *trail, Vec3f *hitPos, Vec3f *hitPoint,
                  RayHit *hit) {
    return func_151D4DAC(actor, owner, hitPos, hitPoint, hit, trail->trail.fieldA4, &trail->trail.path.dir,
                         trail->head.header.owner, trail->head.header.kind);
}

/* Expiry handler 0 (D_8008FBD0[0]): debris, a flash and sometimes a burst where the projectile ended. */
void func_151C2F48(Game1EF500Trail *trail) {
    Game1EF500TrailExt *ext;
    LightEffectHeader header;
    Vec3i header2;

    ext = &trail->trail;
    func_15102920((func_150ADA68() * 30.0f) + 8.0f, 0xFF, &ext->path.tri, &ext->path.pos,
                  (func_150ADA20() % 46U) + 25, 1, 1, ext->path.material, trail->head.header.owner, trail->head.header.kind);
    if (func_150ADA68() < 0.253000021f * D_800DCA24) {
        header.flags = 3;
        header.update = -1;
        header.lifetime = (func_150ADA20() % 7U) + 4;
        header.lightType = 0;
        header2.x = ext->path.pos.x;
        header2.y = ext->path.pos.y;
        header2.z = ext->path.pos.z;
        func_151602C0(&header, &header2, (func_150ADA20() % 9U) + 12, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, trail->head.header.owner,
                      trail->head.header.kind);

        {
            TrailBurst burst;

            burst.countBase = 5 >> D_800DCA20;
            burst.countRange = 4 >> D_800DCA20;
            burst.pos = ext->path.pos;
            burst.sizeBase = 8.25f;
            burst.sizeRange = 5.859999657f;
            burst.gravityBase = 0.0f;
            burst.gravityRange = 0.0f;
            burst.speedBase = 7.0f;
            burst.speedRange = 16.0f;
            burst.angles.yawBase = 0;
            burst.angles.yawRange = 0xFF;
            burst.angles.pitchBase = -61;
            burst.angles.pitchRange = 80;
            burst.capacityBase = 3;
            burst.capacityRange = 1;
            burst.lifeBase = 12;
            burst.lifeRange = 10;
            burst.headerFlags = 1;
            burst.flags = 4;
            burst.primMode = 2;
            burst.envMode = 3;
            burst.colorBase.red = 0xFF;
            burst.colorBase.green = 0xFF;
            burst.colorBase.blue = 0xFF;
            burst.colorBase.alpha = 0xFF;
            burst.colorRange.red = 0;
            burst.colorRange.green = 0;
            burst.colorRange.blue = 0;
            burst.colorRange.alpha = 0;
            burst.color2Base.red = 0xFF;
            burst.color2Base.green = 0xFF;
            burst.color2Base.blue = 0xFF;
            burst.color2Base.alpha = 0xFF;
            burst.color2Range.red = 0;
            burst.color2Range.green = 0;
            burst.color2Range.blue = 0;
            burst.color2Range.alpha = 0;
            burst.colorLevelBase = 0xFF;
            burst.colorLevelRange = 0;
            burst.renderMode = 3;
            burst.unk21 = 0x24;
            burst.setGeometryMode = 0x200005;
            burst.clearGeometryMode = 0x60600;
            burst.fadeStart = 10;
            burst.fadeRate = 25;
            burst.shrinkStart = 1;
            burst.shrinkPaused = 0;
            burst.shrinkFactor = 1.0f;
            burst.unk38 = -1;
            burst.unk39 = 0;
            burst.unk72 = -1;
            burst.unk73 = -1;
            func_15152B38(&burst, trail->head.header.owner, trail->head.header.kind);
        }
    }
}

/* Impact effect at pos: a burst, a randomly flipped particle and a flash. */
void func_151C329C(Vec3f *pos, u8 arg1, s32 arg2) {
    TrailBurst burst;
    ParticleDesc particle;
    LightEffectHeader header;
    Vec3i header2;

    burst.countBase = 5;
    burst.countRange = 7;
    burst.pos = *pos;
    burst.sizeBase = 3.069999933f;
    burst.sizeRange = 6.199999809f;
    burst.gravityBase = -0.9690000415f;
    burst.gravityRange = 0.7180000544f;
    burst.speedBase = 15.0f;
    burst.speedRange = 30.0f;
    burst.angles.yawBase = 0;
    burst.angles.yawRange = 0xFF;
    burst.angles.pitchBase = -64;
    burst.angles.pitchRange = 80;
    burst.capacityBase = 3;
    burst.capacityRange = 1;
    burst.lifeBase = 20;
    burst.lifeRange = 15;
    burst.headerFlags = 1;
    burst.flags = 4;
    burst.primMode = 2;
    burst.envMode = 3;
    burst.colorBase.red = 0xFF;
    burst.colorBase.green = 0xFF;
    burst.colorBase.blue = 0xFF;
    burst.colorBase.alpha = 0xFF;
    burst.colorRange.red = 0;
    burst.colorRange.green = 0;
    burst.colorRange.blue = 0;
    burst.colorRange.alpha = 0;
    burst.color2Base.red = 0xFF;
    burst.color2Base.green = 0xFF;
    burst.color2Base.blue = 0xFF;
    burst.color2Base.alpha = 0xFF;
    burst.color2Range.red = 0;
    burst.color2Range.green = 0;
    burst.color2Range.blue = 0;
    burst.color2Range.alpha = 0;
    burst.colorLevelBase = 0xFF;
    burst.colorLevelRange = 0;
    burst.renderMode = 3;
    burst.unk21 = 0x24;
    burst.setGeometryMode = 0x200005;
    burst.clearGeometryMode = 0x60600;
    burst.fadeStart = 8;
    burst.fadeRate = 31;
    burst.shrinkStart = 1;
    burst.shrinkPaused = 0;
    burst.shrinkFactor = 1.0f;
    burst.unk38 = -1;
    burst.unk39 = 0;
    burst.unk72 = -1;
    burst.unk73 = -1;
    func_15152B38(&burst, arg1, arg2);
    particle.texture = 0x2B;
    particle.renderMode = 0x4403;
    particle.unk0 = 0x200005;
    particle.unk4 = 0x20000;
    particle.timer = (func_150ADA20() % 7U) + 6;
    particle.frame = 0;
    particle.frameRate = 0;
    particle.r = 0xFF;
    particle.g = 0xFF;
    particle.b = 0xFF;
    particle.a = 0xFF;
    particle.r2 = 0xFF;
    particle.g2 = 0xFF;
    particle.b2 = 0xFF;
    particle.alpha = 0xFF;
    particle.brightness = 0xFF;
    particle.width = particle.height = func_150ADA68() * 196.0f + 106.0f;
    particle.pos = *pos;
    particle.fadeStart = 3;
    particle.fadeRate = 0x55;
    particle.scaleStart = 1;
    particle.offset.x = 0.0f;
    particle.offset.y = 0.0f;
    particle.offset.z = 0.0f;
    particle.vel.x = 0.0f;
    particle.vel.y = 0.0f;
    particle.vel.z = 0.0f;
    particle.gravity = 0.0f;
    particle.scaleRate = 1.0f;
    particle.flags = 1 | ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x8000 | 0x4000 |
                     0x200;
    particle.primColorMode = 6;
    particle.envColorMode = 6;
    particle.updateFunc = -1;
    particle.deathFunc = -1;
    particle.drawFunc = -1;
    particle.handlerSet = 4;
    func_15130280(&particle, 1, 0, 0, arg1, arg2);
    header.flags = 3;
    header.update = -1;
    header.lifetime = (func_150ADA20() % 7U) + 6;
    header.lightType = 0;
    header2.x = pos->x;
    header2.y = pos->y;
    header2.z = pos->z;
    func_151602C0(&header, &header2, (func_150ADA20() & 1) + 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg1, arg2);
}

/* Smaller variant of func_151C329C. */
void func_151C36D8(Vec3f *pos, u8 arg1, s32 arg2) {
    TrailBurst burst;
    ParticleDesc particle;
    LightEffectHeader header;
    Vec3i header2;

    burst.countBase = 8;
    burst.countRange = 6;
    burst.pos = *pos;
    burst.sizeBase = 2.599999905f;
    burst.sizeRange = 2.169999838f;
    burst.gravityBase = -0.9690000415f;
    burst.gravityRange = 0.7180000544f;
    burst.speedBase = 10.0f;
    burst.speedRange = 8.0f;
    burst.angles.yawBase = 0;
    burst.angles.yawRange = 0xFF;
    burst.angles.pitchBase = -64;
    burst.angles.pitchRange = 80;
    burst.capacityBase = 3;
    burst.capacityRange = 0;
    burst.lifeBase = 15;
    burst.lifeRange = 10;
    burst.headerFlags = 1;
    burst.flags = 4;
    burst.primMode = 2;
    burst.envMode = 3;
    burst.colorBase.red = 0xFF;
    burst.colorBase.green = 0xFF;
    burst.colorBase.blue = 0xFF;
    burst.colorBase.alpha = 0xFF;
    burst.colorRange.red = 0;
    burst.colorRange.green = 0;
    burst.colorRange.blue = 0;
    burst.colorRange.alpha = 0;
    burst.color2Base.red = 0xFF;
    burst.color2Base.green = 0xFF;
    burst.color2Base.blue = 0xFF;
    burst.color2Base.alpha = 0xFF;
    burst.color2Range.red = 0;
    burst.color2Range.green = 0;
    burst.color2Range.blue = 0;
    burst.color2Range.alpha = 0;
    burst.colorLevelBase = 0xFF;
    burst.colorLevelRange = 0;
    burst.renderMode = 3;
    burst.unk21 = 0x24;
    burst.setGeometryMode = 0x200005;
    burst.clearGeometryMode = 0x60600;
    burst.fadeStart = 8;
    burst.fadeRate = 31;
    burst.shrinkStart = 1;
    burst.shrinkPaused = 0;
    burst.shrinkFactor = 1.0f;
    burst.unk38 = -1;
    burst.unk39 = 0;
    burst.unk72 = -1;
    burst.unk73 = -1;
    func_15152B38(&burst, arg1, arg2);

    particle.texture = 0x2B;
    particle.renderMode = 0x4403;
    particle.unk0 = 0x200005;
    particle.unk4 = 0x20000;
    particle.timer = (func_150ADA20() % 5U) + 4;
    particle.frame = 0;
    particle.frameRate = 0;
    particle.r = 0xFF;
    particle.g = 0xFF;
    particle.b = 0xFF;
    particle.a = 0xFF;
    particle.r2 = 0xFF;
    particle.g2 = 0xFF;
    particle.b2 = 0xFF;
    particle.alpha = 0xFF;
    particle.brightness = 0xFF;
    particle.width = particle.height = (func_150ADA68() * 60.0f) + 80.0f;
    particle.pos = *pos;
    particle.fadeStart = 2;
    particle.fadeRate = 0x7F;
    particle.scaleStart = 1;
    particle.offset.x = 0.0f;
    particle.offset.y = 0.0f;
    particle.offset.z = 0.0f;
    particle.vel.x = 0.0f;
    particle.vel.y = 0.0f;
    particle.vel.z = 0.0f;
    particle.gravity = 0.0f;
    particle.scaleRate = 1.0f;
    particle.flags = ((func_150ADA20() & 1) ? 0x40 : 0) | (((func_150ADA20() & 1) ? 0x80 : 0) | 1) | 0x8000 | 0x4000 |
                     0x200;
    particle.primColorMode = 6;
    particle.envColorMode = 6;
    particle.updateFunc = -1;
    particle.deathFunc = -1;
    particle.drawFunc = -1;
    particle.handlerSet = 4;
    func_15130280(&particle, 1, 0, 0, arg1, arg2);

    header.flags = 3;
    header.update = -1;
    header.lifetime = (func_150ADA20() % 7U) + 6;
    header.lightType = 0;
    header2.x = pos->x;
    header2.y = pos->y;
    header2.z = pos->z;
    func_151602C0(&header, &header2, (func_150ADA20() & 1) + 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg1, arg2);
}

/*
 * Impact effects where the projectile hit: with the given chances, debris, a spark, a smoke burst
 * and a flash. Their look depends on the hit surface's material and on the level (level).
 */
void func_151C3B0C(Game1EF500Trail *trail, f32 debrisChance, f32 sparkChance, f32 smokeChance, f32 flashChance,
                   u8 debrisArg, u8 smokeArg, u8 sparkArg) {
    s32 material;
    Game1EF500TrailExt *ext;
    s32 type;

    material = trail->trail.path.material & 0x1F;
    if ((level == 0x2B) || (level == 0x30)) {
        switch (material) {
        case 1:
            type = 3;
            break;
        case 0:
        case 5:
        default:
            type = 4;
            break;
        }
    } else if ((material == 0xB) && (level == 4)) {
        type = 1;
    } else if ((material == 1 || material == 0xA) && (level == 0xE)) {
        type = 2;
    } else if (level == 0x34) {
        type = 3;
    } else if (level == 0x27) {
        type = 5;
    } else {
        type = 0;
        smokeChance = sparkChance = debrisChance = 0.0f;
    }
    if (func_150ADA68() < debrisChance) {
        SpriteDef debris;
        f32 scale;
        // not const (here and below): the ROM keeps these tables ahead of D_800AAA30, outside the .rodata slice
        s32 shade[6] = { 0x6F, 0x6F, 0x70, 0x70, 0x4F, 0x76 };
        u8 big;

        scale = (func_150ADA68() * 15.0f) + 15.0f;
        debris.texture = shade[type];
        debris.unk7 = 0;
        debris.flags = (D_800BE616 ? 0x20000 : 0) | 0xBF01;
        debris.lifetime = (func_150ADA20() % 61U) + 0x3C;
        debris.frame = 0;
        debris.frameSpeed = 0;
        debris.unk10 = (func_150ADA20() % 56U) + 0xC8;
        debris.unk11 = 0xFF;
        debris.rgba[0] = 0;
        debris.rgba[1] = 0;
        debris.rgba[2] = 0;
        debris.rgba[3] = 0xFF;
        big = (type == 4);
        debris.renderModes = ((big ? 0x48 : 0x3B) << 16) + 2;
        debris.unk16 = 0;
        debris.unk17 = big ? 6 : 7;
        debris.viewportMask = debrisArg;
        debris.fadeStart = 0x20;
        debris.fadeRate = 7;
        ext = &trail->trail;
        func_1513C650(&debris, 0, 0, &ext->path.tri, ext->path.pos.x, ext->path.pos.y, ext->path.pos.z,
                      scale, scale, func_150ADA20() & 0xFF, 0, 0, 0, 0, trail->head.header.owner, trail->head.header.kind);
    }
    if (func_150ADA68() < sparkChance) {
        ParticleDef spark;
        s32 shade[6] = { 0x6D, 0x6D, 0x6E, 0x6E, 0x6D, 0x6D };

        ext = &trail->trail;
        spark.texture = shade[type];
        spark.type = 0;
        spark.renderModes = 0x3B03;
        spark.lifetime = (func_150ADA20() % 6U) + 0xD;
        spark.frame = 0;
        spark.frameSpeed = 0;
        spark.rgba[0] = 0xFF;
        spark.rgba[1] = 0xFF;
        spark.rgba[2] = 0xFF;
        spark.rgba[3] = 0xFF;
        spark.width = (func_150ADA68() * 5.0f) + 7.0f;
        spark.height = (func_150ADA68() * 40.0f) + 10.0f;
        spark.pos = ext->path.pos;
        spark.rot.x = ext->path.pos.x - ext->path.dir.x;
        spark.rot.y = ext->path.pos.y - ext->path.dir.y;
        spark.rot.z = ext->path.pos.z - ext->path.dir.z;
        spark.scale.x = 1.269999981f;
        spark.scale.y = 1.0f;
        spark.scale.z = 56.81999969f;
        spark.flags = (D_800BE616 ? 0x40000000 : 0) | 0x08DC0009;
        spark.unk44 = (func_150ADA20() % 76U) + 0xB4;
        spark.unk45 = 0xFF;
        spark.unk46 = 0;
        spark.unk47 = 7;
        spark.unk48 = 0;
        spark.viewportMask = sparkArg;
        spark.fadeStart = 8;
        spark.fadeRate = 0x1F;
        func_1513D2F0(&spark, &D_800A4AA0, 0x22, 0, 0, 0x1C, 2, 0, 0, 0, trail->head.header.owner, trail->head.header.kind);
    }
    if (func_150ADA68() < smokeChance) {
        SpriteBurst smoke;
        u8 colorA[6][3] = { { 0xB7, 0x77, 0x34 }, { 0xB7, 0x77, 0x34 }, { 0xAA, 0x52, 0x1E },
                            { 0xAA, 0x52, 0x1E }, { 0x98, 0x92, 0x79 }, { 0xFF, 0xC0, 0x00 } };
        u8 colorB[6][3] = { { 0x5C, 0x30, 0x00 }, { 0x5C, 0x30, 0x00 }, { 0x00, 0x00, 0x00 },
                            { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 }, { 0x8D, 0x8D, 0x00 } };

        smoke.countBase = 5;
        smoke.countRange = 15;
        smoke.texture = 0x6C;
        smoke.renderMode = 0x5103;
        smoke.unk8 = 0x200005;
        smoke.unkC = 0;
        smoke.timerBase = 0x14;
        smoke.timerRange = 0xF;
        smoke.frameRateBase = 0;
        smoke.frameRateRange = 0;
        smoke.a = 0xFF;
        smoke.r = colorA[type][0];
        smoke.g = colorA[type][1];
        smoke.b = colorA[type][2];
        smoke.r2 = colorB[type][0];
        smoke.g2 = colorB[type][1];
        smoke.b2 = colorB[type][2];
        smoke.alphaBase = 0xE6;
        smoke.alphaRange = 0x14;
        smoke.brightness = 0xFF;
        smoke.fadeStart = 0x19;
        smoke.fadeRate = 0xA;
        smoke.scaleStart = 0x1E;
        smoke.scaleRate = 1.049999952f;
        smoke.sizeBase = 60.0f;
        smoke.sizeRange = 65.0f;
        smoke.pos = trail->trail.path.pos;
        smoke.yawBase = 0;
        smoke.pitchBase = -40;
        smoke.yawRange = 0xFF;
        smoke.pitchRange = 40;
        smoke.speedBase = 9.0f;
        smoke.speedRange = 17.0f;
        smoke.gravityBase = 0.00800000038f;
        smoke.gravityRange = 0.2610000074f;
        smoke.flags = (D_800BE616 ? 0x800000 : 0) | 0x40E07;
        smoke.updateFunc = 0x10;
        smoke.deathFunc = -1;
        smoke.primColorMode = 8;
        smoke.envColorMode = 6;
        smoke.unk64 = 1;
        smoke.extra = 0.8702070117f;
        func_15153634(&smoke, smokeArg, trail->head.header.owner, trail->head.header.kind);
    }
    if (func_150ADA68() < flashChance) {
        LightEffectHeader header;
        Vec3i header2;
        u8 color[2][3] = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xA7, 0xA7 } };
        u8 sel;

        header.flags = 3;
        header.update = -1;
        ext = &trail->trail;
        header.lifetime = (func_150ADA20() % 6U) + 3;
        header.lightType = 0;
        header2.x = (s32)ext->path.pos.x;
        header2.y = (s32)ext->path.pos.y;
        header2.z = (s32)ext->path.pos.z;
        sel = func_150ADA20() & 1;
        func_151602C0(&header, &header2, (func_150ADA20() % 6U) + 0xA, color[sel][0], color[sel][1], color[sel][2], 0xFF, 0, 0, trail->head.header.owner, trail->head.header.kind);
    }
}

/* Eases the projectile's accel toward its drift, once per elapsed step. */
void func_151C436C(Game1EF500Trail *trail, Game1EF500TrailExt *ext, s32 steps) {
    while (steps > 0) {
        steps--;
        ext->accel.x += (ext->drift.x - ext->accel.x) * 0.228659004f;
        ext->accel.y += (ext->drift.y - ext->accel.y) * 0.228659004f;
        ext->accel.z += (ext->drift.z - ext->accel.z) * 0.228659004f;
    }
}


/*
 * Casts a ray along the projectile's flight over the distance it moves this frame; on a hit,
 * runs the impact handler and returns 0.
 */
u8 func_151C43E0(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt) {
    u8 result;
    Actor *owner;
    f32 length;
    s32 pad[3]; // matching: unused stack space in the original frame
    RayHit ray;
    s32 solid;

    owner = ext->owner;
    result = TRUE;
    if (ext->flags & TRAIL_COLLIDE) {
        if (ext->remaining < dt) {
            length = ext->scale * ext->remaining;
        } else {
            length = ext->scale * dt;
        }
        if (length != 0.0f) {
            Vec3f *from = &trail->head.def.pos;

            solid = !(ext->flags & TRAIL_FLAG_8);
            func_15081690(owner, from->x, from->y, from->z, ext->path.dir.x, ext->path.dir.y, ext->path.dir.z,
                          &ray, length, 1, 0, solid, ext->fieldA8, 0, ext->handle);
            if (ray.hitType >= 2) {
                func_151C2EF0(ray.obj, owner, trail, &ray.pos, &ray.far, &ray);
                result = FALSE;
            }
        }
    }
    return result;
}

/* Moves the projectile by velocity * dt. */
void func_151C4510(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt) {
    trail->head.def.pos.x += ext->velocity.x * dt;
    trail->head.def.pos.y += ext->velocity.y * dt;
    trail->head.def.pos.z += ext->velocity.z * dt;
}

/* Counts down the projectile's remaining distance; when it runs out, fires its expiry callbacks and returns 0. */
s32 func_151C455C(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt) {
    s32 alive;
    TrailHitCallback callback;
    WorldObject *target;
    s8 handler;

    ext->remaining -= dt;
    alive = TRUE;
    if (ext->remaining <= 0.0f) {
        callback = (TrailHitCallback)D_800E0940;
        if (callback != NULL && (ext->flags & TRAIL_HIT_CALLBACK)) {
            target = ext->path.surface;
            if (target != NULL) {
                callback(&ext->path.pos, target - db_Object_array);
            }
        }
        if (ext->flags & TRAIL_EXPIRE_HANDLER) {
            handler = ext->expireHandler;
            if (handler != -1) {
                D_8008FBD0[handler](trail);
            }
        }
        alive = FALSE;
    }
    return alive;
}

/* StreamEmitter update: when its timer runs out, starts one ProjectileStream and picks a new random delay. */
void func_151C4644(Game1EF500TimedOwner *owner) {
    StreamEmitter *emitter;
    ProjectileStream data;
    TimedEffect *effect;

    emitter = &owner->emitter;
    emitter->timer -= D_800BE9E4;
    if (emitter->timer < 0) {
        data.stream.angleX = emitter->angleX;
        data.stream.angleY = emitter->angleY;
        data.stream.amplitudeX = func_150ADA68() * emitter->spread;
        data.stream.amplitudeY = func_150ADA68() * emitter->spread;
        data.stream.phaseX = func_150ADA68() * 6.283185482f; /* 2pi */
        data.stream.phaseY = func_150ADA68() * 6.283185482f;
        data.stream.speedX = emitter->wobbleSpeed;
        data.stream.speedY = emitter->wobbleSpeed;
        data.stream.time = 0.0f;
        data.stream.rate = (func_150ADA68() * emitter->rateRange) + emitter->rateBase;
        data.stream.remaining = 0.0f;
        data.stream.position.x = emitter->position[0];
        data.stream.position.y = emitter->position[1];
        data.stream.position.z = emitter->position[2];
        data.speed = emitter->speed;
        data.unk3C = emitter->unk30;
        data.unk40 = emitter->unk34;

        effect = func_15149130((s16)((func_150ADA20() % (u32)(emitter->lifeRange + 1)) + emitter->minLife),
                               -1, 0x2B, -1, 1, 0, sizeof(data), owner->base.header.owner, owner->base.header.kind);
        if (effect != NULL) {
            memcpy((u8 *)effect + 0x28, &data, sizeof(data));
        }

        emitter->timer = (func_150ADA20() % (u32)(emitter->delayRange + 1)) + emitter->minDelay;
    }
}
