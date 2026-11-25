#include <ultra64.h>
// matching: callers here use the full s32 return of these, unlike their functions.h prototypes
#define func_150ADA20 func_150ADA20_u8_proto
#define func_10010F30 func_10010F30_void
#include "functions.h"
#undef func_150ADA20
#undef func_10010F30
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

extern s32 func_150ADA20(void);
extern s32 func_10010F30(s32, u16, u8, s16, u8);
extern s32 func_1000B060(f32, f32, u32);
extern void func_1000E7A0(u32, s32);
extern s32 func_1510F8CC(s32);
extern f32 D_800AAA30; /* 0.1f */

/* The view of D_800B0DF0 used here; struct104 has no field at 0x2C. */
typedef struct {
    u8 pad0[0x2C];
    s16 unk2C;
} GameStruct151C2050State;

/* Queues a positional sound event (func_1000E7A0 kind 8) for soundId, only in a few specific levels. */
void func_151C2050(struct127 *obj, Vec3f *pos, Vec3f *dir, s32 soundId, f32 distance) {
    s32 pan;
    s32 volume;
    struct108 *camera;
    s32 direction;
    s16 level;
    f32 volumeLevel; // matching: separate spill slot for the value kept across func_1000B060
    f32 intensity;

    if (obj != NULL) {
        if (D_800BE616 || (camera = obj->camera) == NULL) {
            level = ((GameStruct151C2050State *)D_800B0DF0)->unk2C;
            if (level == 0x24 || level == 0x13 || level == 0x4D || level == 0x85 || level == 0x93) {
                func_100114D0((s32)pos->x, (s32)pos->y, (s32)pos->z, 0x7FFF, 1000, 100, &pan, &volume, NULL);
                if ((u32)volume > 0x1000) {
                    volume = (u32)volume >> 7;
                    func_1000E7A0(8, (func_1510F8CC(soundId) + 1) | (volume << 8) | (pan << 16));
                }
            }
        } else {
            level = ((GameStruct151C2050State *)D_800B0DF0)->unk2C;
            if (level == 0x24 || level == 0x13) {
                // matching: the 0.1f literal (vs this rodata load) shifts the stack frame
                intensity = 255.0f - distance * D_800AAA30;
                if (intensity > 16.0f) {
                    volumeLevel = intensity;
                    /* 40.74 = 256 / 2pi: radians to a 256-step angle */
                    direction = func_1000B060(dir->x, dir->z, (u32)(camera->unk3A0 * 40.74366379f));
                    func_1000E7A0(8, (func_1510F8CC(soundId) + 1) | ((s32)volumeLevel << 8) | (direction << 16));
                }
            }
        }
    }
}


/* Result of a ray cast (func_15081690) or of a projectile's path setup (func_151C4B0C). */
typedef struct Game1F1F60Hit {
    /* 0x00 */ void *actor;
    /* 0x04 */ f32 dist;
    /* 0x08 */ Vec3f hit;
    /* 0x14 */ Vec3f normal;
    /* 0x20 */ Vec3f point;
    /* 0x2C */ Vec3f start;
    /* 0x38 */ Vec3f end; /* the ray direction */
    /* 0x44 */ s16 surface[9];
    /* 0x56 */ s16 field_56;
    /* 0x58 */ s8 field_58;
    /* 0x59 */ u8 type;
    /* 0x5A */ s8 field_5A;
    /* 0x5B */ u8 pad5B;
    /* 0x5C */ struct131 *field_5C;
    /* 0x60 */ s32 field_60;
} Game1F1F60Hit; /* 0x64 */

/* Projectile state, stored at offset 0x110 of the object func_151C229C spawns. */
typedef struct Game1EF500TrailExt {
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ Vec3f velocity;
    /* 0x10 */ Vec3f accel; /* eases toward drift each frame; also the length of the drawn tail */
    /* 0x1C */ Vec3f drift;
    /* 0x28 */ Game1F1F60Hit path;
    /* 0x8C */ f32 remaining; /* distance left before the projectile expires */
    /* 0x90 */ struct127 *owner;
    /* 0x94 */ u8 ownerId; /* owner->unique_id at spawn */
    /* 0x95 */ u8 pad95[3];
    /* 0x98 */ f32 scale;
    /* 0x9C */ s8 expireHandler; /* index into D_8008FBD0, -1 for none */
    /* 0x9D */ u8 pad9D;
    /* 0x9E */ u16 sound;
    /* 0xA0 */ u8 soundEnabled;
    /* 0xA1 */ u8 listener; /* index into D_800DBFF0 of the player the sound follows */
    /* 0xA2 */ u8 padA2[2];
    /* 0xA4 */ s32 fieldA4;
    /* 0xA8 */ s8 fieldA8;
    /* 0xA9 */ u8 padA9[3];
    /* 0xAC */ s32 handle;
} Game1EF500TrailExt; /* 0xB0 */

/* TrailExt flags */
#define TRAIL_HIT_CALLBACK 1    /* call D_800E0940 on expiry */
#define TRAIL_EXPIRE_HANDLER 2  /* call D_8008FBD0[expireHandler] on expiry */
#define TRAIL_COLLIDE 4
#define TRAIL_FLAG_8 8

/* The projectile object (allocated by func_1513D2F0). */
typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0x1F];
    /* 0x02C */ f32 width;
    /* 0x030 */ u8 pad30[4];
    /* 0x034 */ Vec3f position;
    /* 0x040 */ u8 pad40[0x1C];
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 pad5D[0x63];
    /* 0x0C0 */ u8 vertexTemplate[0x40];
    /* 0x100 */ u8 *buffers[4];
    /* 0x110 */ Game1EF500TrailExt trail;
} Game1EF500Trail;

/* Expiry handlers, chosen by Game1EF500TrailExt.expireHandler (entry 0 is func_151C2F48). */
typedef void (*TrailExpireHandler)(Game1EF500Trail *);
/* Signature of the hook in D_800E0940: (hit position, index of the struct131 that was hit). */
typedef void (*TrailHitCallback)(Vec3f *, s32);

extern TrailExpireHandler D_8008FBD0[];

typedef struct {
    u16 sound;
    u16 range;
} FlybySound;

/* Spawn template for func_1513D2F0 (0x58 bytes). */
typedef struct Game1EF500TrailSpawn {
    /* 0x00 */ u8 field00;
    /* 0x01 */ u8 field01;
    /* 0x02 */ s16 field02;
    /* 0x04 */ s16 field04;
    /* 0x06 */ u8 pad06[2];
    /* 0x08 */ s32 field08;
    /* 0x0C */ s32 field0C;
    /* 0x10 */ u8 field10;
    /* 0x11 */ u8 field11;
    /* 0x12 */ u8 field12;
    /* 0x13 */ u8 field13;
    /* 0x14 */ f32 field14;
    /* 0x18 */ f32 field18;
    /* 0x1C */ Vec3f position;
    /* 0x28 */ f32 field28[6];
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 field44;
    /* 0x45 */ u8 field45;
    /* 0x46 */ u8 field46;
    /* 0x47 */ u8 field47;
    /* 0x48 */ s32 field48;
    /* 0x4C */ u8 field4C;
    /* 0x4D */ u8 pad4D[7];
    /* 0x54 */ s16 field54;
    /* 0x56 */ s16 field56;
} Game1EF500TrailSpawn;

extern FlybySound D_8008FBE8[];
extern s32 func_1000A420(s32, s32, s32, f32, s32, s32, s32, s32, s32, s32 *, s32 *, s32);
void func_151C436C(Game1EF500Trail *trail, Game1EF500TrailExt *ext, s32 steps);
u8 func_151C43E0(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt);
void func_151C4510(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt);
s32 func_151C455C(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt);
void *memcpy(void *, const void *, u32);
s32 func_15145128(f32 *, f32 *, f32 *, f32 *);
s32 func_151D5B6C(Vec3f *, Vec3f *, struct127 *, s8, s32);
s32 func_151C4B0C(Game1F1F60Hit *arg0, Game1F1F60Hit *arg1, Vec3f *arg2, Vec3f *arg3, f32 *arg4, f32 arg5,
                  u8 arg6, Vec3f *arg7, Vec3f *arg8, u8 *arg9, u8 arg10, f32 arg11, void *arg12, s8 arg13,
                  u8 arg14, u8 arg15, s32 arg16, u8 arg17);

/*
 * Spawns a projectile at origin heading along direction (or from altOrigin along altDirection),
 * sets up its path and runs its first update of dt. Returns NULL if it could not be placed or
 * if that first update already ended it.
 */
Game1EF500Trail *func_151C229C(Vec3f *origin, Vec3f *direction, Vec3f *altOrigin, Vec3f *altDirection,
                               u8 altNormalized, Game1F1F60Hit *knownHit, f32 speed, f32 arg7, f32 arg8,
                               f32 driftSpeed, f32 arg10, u8 arg11, struct127 *owner, u8 arg13, u8 collide, u8 arg15,
                               u8 soundChance, u8 hitCallback, u8 expireHandler, s8 handlerIndex, s32 arg20,
                               f32 dt, u8 arg22, s8 arg23, s32 extraSize, u8 arg25, s32 arg26) {
    Game1EF500Trail *result;
    Game1EF500TrailSpawn spawn;
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
    ext.velocity.x = ext.path.end.x * speed;
    ext.velocity.y = ext.path.end.y * speed;
    ext.velocity.z = ext.path.end.z * speed;
    ext.accel.x = 0.0f;
    ext.accel.y = 0.0f;
    ext.accel.z = 0.0f;
    ext.drift.x = ext.path.end.x * driftSpeed;
    ext.drift.y = ext.path.end.y * driftSpeed;
    ext.drift.z = ext.path.end.z * driftSpeed;
    ext.owner = owner;
    ext.ownerId = (owner != NULL) ? owner->unique_id : 99;
    spawn.field00 = 0x41;
    ext.scale = speed;
    spawn.field01 = 4;
    spawn.field02 = 0x4503;
    spawn.field04 = 300;
    spawn.field08 = 0;
    spawn.field0C = 0;
    spawn.field10 = 0;
    spawn.field11 = 0;
    spawn.field12 = 0;
    spawn.field13 = 0xFF;
    spawn.field44 = arg11;
    spawn.field45 = 0xFF;
    spawn.field18 = driftSpeed;
    spawn.field14 = arg8;
    spawn.position = *origin;
    spawn.field28[0] = 0.0f;
    spawn.field28[1] = 0.0f;
    spawn.field28[2] = 0.0f;
    spawn.field28[3] = 0.0f;
    spawn.field28[4] = 0.0f;
    spawn.field28[5] = 0.0f;
    spawn.flags = 0xC04C0008;
    spawn.field46 = 0;
    spawn.field47 = 0;
    spawn.field4C = arg22;
    result = func_1513D2F0(&spawn, (s32)&D_800A4AA0, 0x16, 0, 0, 0x14, 0, 0, 0, extraSize + sizeof(ext), arg25, arg26);
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
            func_1516972C((struct102 *)result);
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
    struct108 *player;
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
                player = &D_800DBFF0[i];
                dx = player->unk2F8 - trail->position.x;
                dy = player->unk2FC - trail->position.y;
                dz = player->unk300 - trail->position.z;
                /* distance of the player ahead along the flight direction */
                dot = ext->path.end.x * dx + dy * ext->path.end.y + dz * ext->path.end.z;
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
            player = &D_800DBFF0[ext->listener];
        } else {
            ext->soundEnabled = FALSE;
        }
        if (player != NULL) {
            volume = 0x40;
            func_1000A420(trail->position.x - player->unk2F8, trail->position.y - player->unk2FC,
                          trail->position.z - player->unk300, player->unk380,
                          trail->position.x - player->unk2F8, trail->position.y - player->unk2FC,
                          trail->position.z - player->unk300, 10000, 9999, &volume, &pan, 0);
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

void func_151D5D60(u8 **, s16, s32, void **, u8 *);
Vec3f *func_15144B34(s32);

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

    func_151D5D60(trail->buffers, index, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(trail->buffers[index], trail->vertexTemplate, sizeof(trail->vertexTemplate));
            memcpy(trail->buffers[index] + 0x40, trail->vertexTemplate, sizeof(trail->vertexTemplate));
        }
    } else {
        return NULL;
    }
    // matching: the loop wrapper gives IDO an extra block boundary that sets register and schedule order
    do { ext = &trail->trail; } while (0);
    eye = func_15144B34(index);
    start = trail->position;
    end.x = trail->position.x - ext->accel.x;
    end.y = trail->position.y - ext->accel.y;
    end.z = trail->position.z - ext->accel.z;
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
        scale = trail->width / sqrtf(lengthSq);
        nx = nx * scale;
        ny = ny * scale;
        nz = nz * scale;
    }
    vertices->v.ob[0] = start.x - nx;
    vertices->v.ob[1] = start.y - ny;
    vertices->v.ob[2] = start.z - nz;
    vertices->v.cn[3] = trail->alpha;
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
    vertices->v.cn[3] = trail->alpha;
    vertices->v.flag = 0;
    return result;
}

/* Whether obj is a live object other than self (a ray-cast filter). */
s32 func_151C2E4C(struct127 *obj, struct127 *self) {
    if (obj == self) {
        return FALSE;
    }
    if (obj->interaction_state == 0) {
        return FALSE;
    }
    if (obj->id == 0xFF) {
        return FALSE;
    }
    return TRUE;
}

/* As func_151C2E4C, and obj's unk127 must also be set (not 0xFF). */
s32 func_151C2E94(struct127 *obj, struct127 *self) {
    if (obj == self) {
        return FALSE;
    }
    if (obj->interaction_state == 0) {
        return FALSE;
    }
    if (obj->id == 0xFF) {
        return FALSE;
    }
    if (obj->unk127 == 0xFF) {
        return FALSE;
    }
    return TRUE;
}

s32 func_151D4DAC(void *, struct127 *, Vec3f *, Vec3f *, Game1F1F60Hit *, s32, Vec3f *, u8, u8);

/* Impact handler: the projectile hit actor (at hitPos) during its ray cast. */
s32 func_151C2EF0(void *actor, struct127 *owner, Game1EF500Trail *trail, Vec3f *hitPos, Vec3f *hitPoint,
                  Game1F1F60Hit *hit) {
    return func_151D4DAC(actor, owner, hitPos, hitPoint, hit, trail->trail.fieldA4, &trail->trail.path.end,
                         trail->unkC, trail->unk1);
}

extern u8 D_800DCA20;
extern f32 D_800DCA24;
void func_15102920(f32, s32, s16 *, Vec3f *, s32, s32, s32, s32, u8, u8);
void func_15152B38(void *, u8, s32);
s32 func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);

typedef struct {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} Color8;

/* Settings of a func_15152B38 burst (layout as struct Local15152B38Arg in game_17CAF0.c). */
typedef struct {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f position;
    /* 0x14 */ f32 sizeBase;
    /* 0x18 */ f32 sizeRange;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 speedBase;
    /* 0x28 */ f32 speedRange;
    /* 0x2C */ s16 yawBase;
    /* 0x2E */ s16 yawRange;
    /* 0x30 */ s16 pitchBase;
    /* 0x32 */ s16 pitchRange;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 lifeBase;
    /* 0x3E */ s16 lifeRange;
    /* 0x40 */ u16 kind;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ Color8 colorBase;
    /* 0x49 */ Color8 colorRange;
    /* 0x4D */ Color8 color2Base;
    /* 0x51 */ Color8 color2Range;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ s8 unk70;
    /* 0x71 */ s8 unk71;
    /* 0x72 */ s8 unk72;
    /* 0x73 */ s8 unk73;
} BurstConfig;

/* Expiry handler 0 (D_8008FBD0[0]): debris, a flash and sometimes a burst where the projectile ended. */
void func_151C2F48(Game1EF500Trail *trail) {
    Game1EF500TrailExt *ext;
    Header header;
    Header2 header2;

    ext = &trail->trail;
    func_15102920((func_150ADA68() * 30.0f) + 8.0f, 0xFF, ext->path.surface, &ext->path.hit,
                  (func_150ADA20() % 46U) + 25, 1, 1, ext->path.field_60, trail->unkC, trail->unk1);
    if (func_150ADA68() < 0.253000021f * D_800DCA24) {
        header.unk0 = 3;
        header.unk1 = -1;
        header.unk2 = (func_150ADA20() % 7U) + 4;
        header.unk4 = 0;
        header2.unk0 = ext->path.hit.x;
        header2.unk4 = ext->path.hit.y;
        header2.unk8 = ext->path.hit.z;
        func_151602C0(&header, &header2, (func_150ADA20() % 9U) + 12, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, trail->unkC,
                      trail->unk1);

        {
            BurstConfig burst;

            burst.countBase = 5 >> D_800DCA20;
            burst.countRange = 4 >> D_800DCA20;
            burst.position = ext->path.hit;
            burst.sizeBase = 8.25f;
            burst.sizeRange = 5.859999657f;
            burst.unk1C = 0.0f;
            burst.unk20 = 0.0f;
            burst.speedBase = 7.0f;
            burst.speedRange = 16.0f;
            burst.yawBase = 0;
            burst.yawRange = 0xFF;
            burst.pitchBase = -61;
            burst.pitchRange = 80;
            burst.unk34 = 3;
            burst.unk38 = 1;
            burst.lifeBase = 12;
            burst.lifeRange = 10;
            burst.kind = 1;
            burst.unk42 = 4;
            burst.unk43 = 2;
            burst.unk44 = 3;
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
            burst.unk55 = 0xFF;
            burst.unk56 = 0;
            burst.unk57 = 3;
            burst.unk58 = 0x24;
            burst.unk5C = 0x200005;
            burst.unk60 = 0x60600;
            burst.unk64 = 10;
            burst.unk66 = 25;
            burst.unk68 = 1;
            burst.unk6A = 0;
            burst.unk6C = 1.0f;
            burst.unk70 = -1;
            burst.unk71 = 0;
            burst.unk72 = -1;
            burst.unk73 = -1;
            func_15152B38(&burst, trail->unkC, trail->unk1);
        }
    }
}

/* Particle descriptor passed to func_15130280 (0x70 bytes, as ParticleDesc in game_1228D0.c). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 lifetime;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ Vec3f pos;
    /* 0x3C */ Vec3f unk3C;
    /* 0x48 */ Vec3f vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 flags;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ f32 unk6C;
} ParticleDesc; /* 0x70 */

extern void *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);

/* Impact effect at pos: a burst, a randomly flipped particle and a flash. */
void func_151C329C(Vec3f *pos, u8 arg1, s32 arg2) {
    BurstConfig burst;
    ParticleDesc particle;
    Header header;
    Header2 header2;

    burst.countBase = 5;
    burst.countRange = 7;
    burst.position = *pos;
    burst.sizeBase = 3.069999933f;
    burst.sizeRange = 6.199999809f;
    burst.unk1C = -0.9690000415f;
    burst.unk20 = 0.7180000544f;
    burst.speedBase = 15.0f;
    burst.speedRange = 30.0f;
    burst.yawBase = 0;
    burst.yawRange = 0xFF;
    burst.pitchBase = -64;
    burst.pitchRange = 80;
    burst.unk34 = 3;
    burst.unk38 = 1;
    burst.lifeBase = 20;
    burst.lifeRange = 15;
    burst.kind = 1;
    burst.unk42 = 4;
    burst.unk43 = 2;
    burst.unk44 = 3;
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
    burst.unk55 = 0xFF;
    burst.unk56 = 0;
    burst.unk57 = 3;
    burst.unk58 = 0x24;
    burst.unk5C = 0x200005;
    burst.unk60 = 0x60600;
    burst.unk64 = 8;
    burst.unk66 = 31;
    burst.unk68 = 1;
    burst.unk6A = 0;
    burst.unk6C = 1.0f;
    burst.unk70 = -1;
    burst.unk71 = 0;
    burst.unk72 = -1;
    burst.unk73 = -1;
    func_15152B38(&burst, arg1, arg2);
    particle.unk1D = 0x2B;
    particle.unk8 = 0x4403;
    particle.unk0 = 0x200005;
    particle.unk4 = 0x20000;
    particle.lifetime = (func_150ADA20() % 7U) + 6;
    particle.unkC = 0;
    particle.unk10 = 0;
    particle.unk14 = 0xFF;
    particle.unk15 = 0xFF;
    particle.unk16 = 0xFF;
    particle.unk17 = 0xFF;
    particle.unk18 = 0xFF;
    particle.unk19 = 0xFF;
    particle.unk1A = 0xFF;
    particle.unk1B = 0xFF;
    particle.unk1C = 0xFF;
    particle.unk28 = particle.unk2C = func_150ADA68() * 196.0f + 106.0f;
    particle.pos = *pos;
    particle.unk1E = 3;
    particle.unk20 = 0x55;
    particle.unk22 = 1;
    particle.unk3C.x = 0.0f;
    particle.unk3C.y = 0.0f;
    particle.unk3C.z = 0.0f;
    particle.vel.x = 0.0f;
    particle.vel.y = 0.0f;
    particle.vel.z = 0.0f;
    particle.unk54 = 0.0f;
    particle.unk24 = 1.0f;
    particle.flags = 1 | ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x8000 | 0x4000 |
                     0x200;
    particle.unk60 = 6;
    particle.unk61 = 6;
    particle.unk62 = -1;
    particle.unk63 = -1;
    particle.unk64 = -1;
    particle.unk65 = 4;
    func_15130280(&particle, 1, 0, 0, arg1, arg2);
    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() % 7U) + 6;
    header.unk4 = 0;
    header2.unk0 = pos->x;
    header2.unk4 = pos->y;
    header2.unk8 = pos->z;
    func_151602C0(&header, &header2, (func_150ADA20() & 1) + 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg1, arg2);
}

/* Smaller variant of func_151C329C. */
void func_151C36D8(Vec3f *pos, u8 arg1, s32 arg2) {
    BurstConfig burst;
    ParticleDesc particle;
    Header header;
    Header2 header2;

    burst.countBase = 8;
    burst.countRange = 6;
    burst.position = *pos;
    burst.sizeBase = 2.599999905f;
    burst.sizeRange = 2.169999838f;
    burst.unk1C = -0.9690000415f;
    burst.unk20 = 0.7180000544f;
    burst.speedBase = 10.0f;
    burst.speedRange = 8.0f;
    burst.yawBase = 0;
    burst.yawRange = 0xFF;
    burst.pitchBase = -64;
    burst.pitchRange = 80;
    burst.unk34 = 3;
    burst.unk38 = 0;
    burst.lifeBase = 15;
    burst.lifeRange = 10;
    burst.kind = 1;
    burst.unk42 = 4;
    burst.unk43 = 2;
    burst.unk44 = 3;
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
    burst.unk55 = 0xFF;
    burst.unk56 = 0;
    burst.unk57 = 3;
    burst.unk58 = 0x24;
    burst.unk5C = 0x200005;
    burst.unk60 = 0x60600;
    burst.unk64 = 8;
    burst.unk66 = 31;
    burst.unk68 = 1;
    burst.unk6A = 0;
    burst.unk6C = 1.0f;
    burst.unk70 = -1;
    burst.unk71 = 0;
    burst.unk72 = -1;
    burst.unk73 = -1;
    func_15152B38(&burst, arg1, arg2);

    particle.unk1D = 0x2B;
    particle.unk8 = 0x4403;
    particle.unk0 = 0x200005;
    particle.unk4 = 0x20000;
    particle.lifetime = (func_150ADA20() % 5U) + 4;
    particle.unkC = 0;
    particle.unk10 = 0;
    particle.unk14 = 0xFF;
    particle.unk15 = 0xFF;
    particle.unk16 = 0xFF;
    particle.unk17 = 0xFF;
    particle.unk18 = 0xFF;
    particle.unk19 = 0xFF;
    particle.unk1A = 0xFF;
    particle.unk1B = 0xFF;
    particle.unk1C = 0xFF;
    particle.unk28 = particle.unk2C = (func_150ADA68() * 60.0f) + 80.0f;
    particle.pos = *pos;
    particle.unk1E = 2;
    particle.unk20 = 0x7F;
    particle.unk22 = 1;
    particle.unk3C.x = 0.0f;
    particle.unk3C.y = 0.0f;
    particle.unk3C.z = 0.0f;
    particle.vel.x = 0.0f;
    particle.vel.y = 0.0f;
    particle.vel.z = 0.0f;
    particle.unk54 = 0.0f;
    particle.unk24 = 1.0f;
    particle.flags = ((func_150ADA20() & 1) ? 0x40 : 0) | (((func_150ADA20() & 1) ? 0x80 : 0) | 1) | 0x8000 | 0x4000 |
                     0x200;
    particle.unk60 = 6;
    particle.unk61 = 6;
    particle.unk62 = -1;
    particle.unk63 = -1;
    particle.unk64 = -1;
    particle.unk65 = 4;
    func_15130280(&particle, 1, 0, 0, arg1, arg2);

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() % 7U) + 6;
    header.unk4 = 0;
    header2.unk0 = pos->x;
    header2.unk4 = pos->y;
    header2.unk8 = pos->z;
    func_151602C0(&header, &header2, (func_150ADA20() & 1) + 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg1, arg2);
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
} DebrisPacket; /* 0x28, func_1513C650 packet (as Packet15141F78 in game_16EE20.c) */

typedef struct {
    /* 0x00 */ s16 countBase;
    /* 0x02 */ s16 countRange;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 unk22;
    /* 0x23 */ u8 unk23;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 unk25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ Vec3f position;
    /* 0x44 */ s16 yawBase;
    /* 0x46 */ s16 pitchBase;
    /* 0x48 */ s16 yawRange;
    /* 0x4A */ s16 pitchRange;
    /* 0x4C */ f32 speedBase;
    /* 0x50 */ f32 speedRange;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s8 unk60;
    /* 0x61 */ s8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 pad65[3];
    /* 0x68 */ f32 extra;
} SmokeConfig; /* 0x6C, func_15153634 config (as Config15153634 in game_1EC390.c) */

void func_15153634(SmokeConfig *, u8, u8, s32);

/*
 * Impact effects where the projectile hit: with the given chances, debris, a spark, a smoke burst
 * and a flash. Their look depends on the hit surface's material and on the level (D_800BE9F0).
 */
void func_151C3B0C(Game1EF500Trail *trail, f32 debrisChance, f32 sparkChance, f32 smokeChance, f32 flashChance,
                   u8 debrisArg, u8 smokeArg, u8 sparkArg) {
    s32 material;
    Game1EF500TrailExt *ext;
    s32 type;

    material = trail->trail.path.field_60 & 0x1F;
    if ((D_800BE9F0 == 0x2B) || (D_800BE9F0 == 0x30)) {
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
    } else if ((material == 0xB) && (D_800BE9F0 == 4)) {
        type = 1;
    } else if ((material == 1 || material == 0xA) && (D_800BE9F0 == 0xE)) {
        type = 2;
    } else if (D_800BE9F0 == 0x34) {
        type = 3;
    } else if (D_800BE9F0 == 0x27) {
        type = 5;
    } else {
        type = 0;
        smokeChance = sparkChance = debrisChance = 0.0f;
    }
    if (func_150ADA68() < debrisChance) {
        DebrisPacket debris;
        f32 scale;
        const s32 shade[6] = { 0x6F, 0x6F, 0x70, 0x70, 0x4F, 0x76 };
        u8 big;

        scale = (func_150ADA68() * 15.0f) + 15.0f;
        debris.unk6 = shade[type];
        debris.unk7 = 0;
        debris.unk0 = (D_800BE616 ? 0x20000 : 0) | 0xBF01;
        debris.unk4 = (func_150ADA20() % 61U) + 0x3C;
        debris.unk8 = 0;
        debris.unkC = 0;
        debris.unk10 = (func_150ADA20() % 56U) + 0xC8;
        debris.unk11 = 0xFF;
        debris.unk12 = 0;
        debris.unk13 = 0;
        debris.unk14 = 0;
        debris.unk15 = 0xFF;
        big = (type == 4);
        debris.unk18 = ((big ? 0x48 : 0x3B) << 16) + 2;
        debris.unk16 = 0;
        debris.unk17 = big ? 6 : 7;
        debris.unk20 = debrisArg;
        debris.unk22 = 0x20;
        debris.unk24 = 7;
        ext = &trail->trail;
        func_1513C650((s32)&debris, 0, 0, (s32)ext->path.surface, ext->path.hit.x, ext->path.hit.y, ext->path.hit.z,
                      scale, scale, func_150ADA20() & 0xFF, 0, 0, 0, 0, trail->unkC, trail->unk1);
    }
    if (func_150ADA68() < sparkChance) {
        Game1EF500TrailSpawn spark;
        const s32 shade[6] = { 0x6D, 0x6D, 0x6E, 0x6E, 0x6D, 0x6D };

        ext = &trail->trail;
        spark.field00 = shade[type];
        spark.field01 = 0;
        spark.field02 = 0x3B03;
        spark.field04 = (func_150ADA20() % 6U) + 0xD;
        spark.field08 = 0;
        spark.field0C = 0;
        spark.field10 = 0xFF;
        spark.field11 = 0xFF;
        spark.field12 = 0xFF;
        spark.field13 = 0xFF;
        spark.field14 = (func_150ADA68() * 5.0f) + 7.0f;
        spark.field18 = (func_150ADA68() * 40.0f) + 10.0f;
        spark.position = ext->path.hit;
        spark.field28[0] = ext->path.hit.x - ext->path.end.x;
        spark.field28[1] = ext->path.hit.y - ext->path.end.y;
        spark.field28[2] = ext->path.hit.z - ext->path.end.z;
        spark.field28[3] = 1.269999981f;
        spark.field28[4] = 1.0f;
        spark.field28[5] = 56.81999969f;
        spark.flags = (D_800BE616 ? 0x40000000 : 0) | 0x08DC0009;
        spark.field44 = (func_150ADA20() % 76U) + 0xB4;
        spark.field45 = 0xFF;
        spark.field46 = 0;
        spark.field47 = 7;
        spark.field48 = 0;
        spark.field4C = sparkArg;
        spark.field54 = 8;
        spark.field56 = 0x1F;
        func_1513D2F0(&spark, (s32)&D_800A4AA0, 0x22, 0, 0, 0x1C, 2, 0, 0, 0, trail->unkC, trail->unk1);
    }
    if (func_150ADA68() < smokeChance) {
        SmokeConfig smoke;
        const u8 colorA[6][3] = { { 0xB7, 0x77, 0x34 }, { 0xB7, 0x77, 0x34 }, { 0xAA, 0x52, 0x1E },
                            { 0xAA, 0x52, 0x1E }, { 0x98, 0x92, 0x79 }, { 0xFF, 0xC0, 0x00 } };
        const u8 colorB[6][3] = { { 0x5C, 0x30, 0x00 }, { 0x5C, 0x30, 0x00 }, { 0x00, 0x00, 0x00 },
                            { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x8D }, { 0x8D, 0x00, 0x00 } };

        smoke.countBase = 5;
        smoke.countRange = 15;
        smoke.unk4 = 0x6C;
        smoke.unk6 = 0x5103;
        smoke.unk8 = 0x200005;
        smoke.unkC = 0;
        smoke.unk10 = 0x14;
        smoke.unk12 = 0xF;
        smoke.unk14 = 0;
        smoke.unk18 = 0;
        smoke.unk1F = 0xFF;
        smoke.unk1C = colorA[type][0];
        smoke.unk1D = colorA[type][1];
        smoke.unk1E = colorA[type][2];
        smoke.unk20 = colorB[type][0];
        smoke.unk21 = colorB[type][1];
        smoke.unk22 = colorB[type][2];
        smoke.unk23 = 0xE6;
        smoke.unk24 = 0x14;
        smoke.unk25 = 0xFF;
        smoke.unk26 = 0x19;
        smoke.unk28 = 0xA;
        smoke.unk2A = 0x1E;
        smoke.unk2C = 1.049999952f;
        smoke.unk30 = 60.0f;
        smoke.unk34 = 65.0f;
        smoke.position = trail->trail.path.hit;
        smoke.yawBase = 0;
        smoke.pitchBase = -40;
        smoke.yawRange = 0xFF;
        smoke.pitchRange = 40;
        smoke.speedBase = 9.0f;
        smoke.speedRange = 17.0f;
        smoke.unk54 = 0.00800000038f;
        smoke.unk58 = 0.2610000074f;
        smoke.unk5C = (D_800BE616 ? 0x800000 : 0) | 0x40E07;
        smoke.unk60 = 0x10;
        smoke.unk61 = -1;
        smoke.unk62 = 8;
        smoke.unk63 = 6;
        smoke.unk64 = 1;
        smoke.extra = 0.8702070117f;
        func_15153634(&smoke, smokeArg, trail->unkC, trail->unk1);
    }
    if (func_150ADA68() < flashChance) {
        Header header;
        Header2 header2;
        const u8 color[2][3] = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xA7, 0xA7 } };
        u8 sel;

        header.unk0 = 3;
        header.unk1 = -1;
        ext = &trail->trail;
        header.unk2 = (func_150ADA20() % 6U) + 3;
        header.unk4 = 0;
        header2.unk0 = (s32)ext->path.hit.x;
        header2.unk4 = (s32)ext->path.hit.y;
        header2.unk8 = (s32)ext->path.hit.z;
        sel = func_150ADA20() & 1;
        func_151602C0(&header, &header2, (func_150ADA20() % 6U) + 0xA, color[sel][0], color[sel][1], color[sel][2], 0xFF, 0, 0, trail->unkC, trail->unk1);
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

void func_15081690(void *, f32, f32, f32, f32, f32, f32, Game1F1F60Hit *, f32, s32, s32, s32, s32, s32, s32);

/*
 * Casts a ray along the projectile's flight over the distance it moves this frame; on a hit,
 * runs the impact handler and returns 0.
 */
u8 func_151C43E0(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt) {
    u8 result;
    struct127 *owner;
    f32 length;
    s32 pad[3]; // matching: unused stack space in the original frame
    Game1F1F60Hit ray;
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
            Vec3f *from = &trail->position;

            solid = !(ext->flags & TRAIL_FLAG_8);
            func_15081690(owner, from->x, from->y, from->z, ext->path.end.x, ext->path.end.y, ext->path.end.z,
                          &ray, length, 1, 0, solid, ext->fieldA8, 0, ext->handle);
            if (ray.type >= 2) {
                func_151C2EF0(ray.actor, owner, trail, &ray.hit, &ray.point, &ray);
                result = FALSE;
            }
        }
    }
    return result;
}

/* Moves the projectile by velocity * dt. */
void func_151C4510(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt) {
    trail->position.x += ext->velocity.x * dt;
    trail->position.y += ext->velocity.y * dt;
    trail->position.z += ext->velocity.z * dt;
}

/* Counts down the projectile's remaining distance; when it runs out, fires its expiry callbacks and returns 0. */
s32 func_151C455C(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt) {
    s32 alive;
    TrailHitCallback callback;
    struct131 *target;
    s8 handler;

    ext->remaining -= dt;
    alive = TRUE;
    if (ext->remaining <= 0.0f) {
        callback = (TrailHitCallback)D_800E0940;
        if (callback != NULL && (ext->flags & TRAIL_HIT_CALLBACK)) {
            target = ext->path.field_5C;
            if (target != NULL) {
                callback(&ext->path.hit, target - D_800DBEF4);
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

/* Data of a timed particle emitter effect, after its 0x28-byte effect head. */
typedef struct {
    /* 0x00 */ s16 *position; /* x, y, z */
    /* 0x04 */ s32 timer;
    /* 0x08 */ s32 minimumDelay;
    /* 0x0C */ s32 delayRange;
    /* 0x10 */ s16 minimumLife;
    /* 0x12 */ s16 lifeRange;
    /* 0x14 */ f32 base;
    /* 0x18 */ f32 jitter;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 spread;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
} Game1EF500TimedEmitter;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ Game1EF500TimedEmitter emitter;
} Game1EF500TimedOwner;

/* Data of each particle the timed emitter spawns (copied to the particle's +0x28). */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 angle10;
    /* 0x14 */ f32 angle14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ Vec3f position;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
} Game1EF500TimedParticle; /* 0x44 */

/* Timed emitter update: when its timer runs out, spawns one particle and picks a new random delay. */
void func_151C4644(Game1EF500TimedOwner *owner) {
    Game1EF500TimedEmitter *emitter;
    Game1EF500TimedParticle data;
    struct260 *effect;

    emitter = &owner->emitter;
    emitter->timer -= D_800BE9E4;
    if (emitter->timer < 0) {
        data.unk0 = emitter->unk1C;
        data.unk4 = emitter->unk20;
        data.unk8 = func_150ADA68() * emitter->spread;
        data.unkC = func_150ADA68() * emitter->spread;
        data.angle10 = func_150ADA68() * 6.283185482f; /* 2pi */
        data.angle14 = func_150ADA68() * 6.283185482f;
        data.unk18 = emitter->unk28;
        data.unk1C = emitter->unk28;
        data.unk20 = 0.0f;
        data.unk24 = (func_150ADA68() * emitter->jitter) + emitter->base;
        data.unk28 = 0.0f;
        data.position.x = emitter->position[0];
        data.position.y = emitter->position[1];
        data.position.z = emitter->position[2];
        data.unk38 = emitter->unk2C;
        data.unk3C = emitter->unk30;
        data.unk40 = emitter->unk34;

        // the struct37 * parameter is really the size of the effect's data
        effect = func_15149130((s16)((func_150ADA20() % (u32)(emitter->lifeRange + 1)) + emitter->minimumLife),
                               -1, 0x2B, -1, 1, 0, (struct37 *)sizeof(data), owner->unkC, owner->unk1);
        if (effect != NULL) {
            memcpy((u8 *)effect + 0x28, &data, sizeof(data));
        }

        emitter->timer = (func_150ADA20() % (u32)(emitter->delayRange + 1)) + emitter->minimumDelay;
    }
}
