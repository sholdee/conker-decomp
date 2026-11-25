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
extern f32 D_800AAA7C;
extern f32 D_800AAA80;
extern f32 D_800AAA84;

/* The view of D_800B0DF0 used here; struct104 has no field at 0x2C. */
typedef struct {
    u8 pad0[0x2C];
    s16 unk2C;
} GameStruct151C2050State;

/* Queues a positional sound event (func_1000E7A0 kind 8) for sound arg3 in a few specific levels. */
void func_151C2050(struct127 *arg0, Vec3f *pos, Vec3f *dir, s32 arg3, f32 distance) {
    s32 pan;
    s32 volume;
    struct108 *camera;
    s32 direction;
    s16 level;
    f32 volumeLevel; // matching: separate spill slot for the value kept across func_1000B060
    f32 intensity;

    if (arg0 != NULL) {
        if (D_800BE616 || (camera = arg0->camera) == NULL) {
            level = ((GameStruct151C2050State *)D_800B0DF0)->unk2C;
            if (level == 0x24 || level == 0x13 || level == 0x4D || level == 0x85 || level == 0x93) {
                func_100114D0((s32)pos->x, (s32)pos->y, (s32)pos->z, 0x7FFF, 1000, 100, &pan, &volume, NULL);
                if ((u32)volume > 0x1000) {
                    volume = (u32)volume >> 7;
                    func_1000E7A0(8, (func_1510F8CC(arg3) + 1) | (volume << 8) | (pan << 16));
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
                    func_1000E7A0(8, (func_1510F8CC(arg3) + 1) | ((s32)volumeLevel << 8) | (direction << 16));
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
    /* 0x90 */ u8 *owner;
    /* 0x94 */ u8 ownerId;
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

typedef void (*GameFunc151C455CDispatch)(Game1EF500Trail *);
typedef void (*GameFunc151C455CCallback)(Vec3f *, s32);

extern GameFunc151C455CDispatch D_8008FBD0[];

typedef struct {
    u16 sound;
    u16 range;
} GameStruct151C2734Sfx;

typedef struct Game1EF500TrailSpawn {
    u8 field00;
    u8 field01;
    s16 field02;
    s16 field04;
    u8 pad06[2];
    s32 field08;
    s32 field0C;
    u8 field10;
    u8 field11;
    u8 field12;
    u8 field13;
    f32 field14;
    f32 field18;
    Vec3f position;
    f32 field28[6];
    s32 flags;
    u8 field44;
    u8 field45;
    u8 field46;
    u8 field47;
    u8 pad48[4];
    u8 field4C;
    u8 pad4D[0xB];
} Game1EF500TrailSpawn;

extern GameStruct151C2734Sfx D_8008FBE8[];
extern s32 func_1000A420(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32 *, s32 *, s32);
void func_151C436C(Game1EF500Trail *trail, Game1EF500TrailExt *ext, s32 steps);
u8 func_151C43E0(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt);
void func_151C4510(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt);
s32 func_151C455C(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt);
void *memcpy(void *, const void *, u32);
s32 func_15145128(f32 *, f32 *, f32 *, f32 *);
s32 func_151D5B6C(Vec3f *, Vec3f *, u8 *, s8, s32);
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
                               f32 driftSpeed, f32 arg10, u8 arg11, u8 *owner, u8 arg13, u8 collide, u8 arg15,
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
    ext.ownerId = (owner != NULL) ? owner[0x3B] : 99;
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
                          trail->position.z - player->unk300, *(s32 *)&player->unk380,
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

typedef struct {
    f32 x, y, z;
} Game151C2AD0Vec;

typedef struct {
    s16 x, y, z, flag;
    u8 pad8[7];
    u8 alpha;
} Game151C2AD0Vtx;

typedef struct {
    u8 pad0[0x10];
    Game151C2AD0Vec pos;
} Game151C2AD0Link;

typedef struct {
    u8 pad0[0x2C];
    f32 width;
    u8 pad30[4];
    Game151C2AD0Vec pos;
    u8 pad40[0x1C];
    u8 alpha;
    u8 pad5D[0xC0 - 0x5D];
    u8 unkC0[0x40];
    u8 *buffers[4];
    Game151C2AD0Link unk110;
} Game151C2AD0Owner;

void func_151D5D60(void *, s16, s32, void **, u8 *);
f32 *func_15144B34(s32);

void *func_151C2AD0(Game151C2AD0Owner *arg0, s16 arg1) {
    Game151C2AD0Vec start;
    Game151C2AD0Vec end;
    f32 dx, dy, dz;
    f32 nx, ny, nz;
    Game151C2AD0Vec mid;
    f32 tx, ty;
    Game151C2AD0Link *link;
    f32 len;
    f32 scale;
    Game151C2AD0Vtx *vertices;
    Game151C2AD0Vtx *result;
    f32 *cam;
    u8 fresh;
    Game151C2AD0Vtx *p;

    func_151D5D60((u8 *)arg0 + 0x100, arg1, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != 0) {
        if (fresh != 0) {
            memcpy(*(u8 **)((u8 *)arg0 + arg1 * 4 + 0x100), (u8 *)arg0 + 0xC0, 0x40);
            memcpy(*(u8 **)((u8 *)arg0 + arg1 * 4 + 0x100) + 0x40, (u8 *)arg0 + 0xC0, 0x40);
        }
    } else {
        return 0;
    }
    do { link = &arg0->unk110; } while (0);
    cam = func_15144B34(arg1);
    start = arg0->pos;
    end.x = arg0->pos.x - link->pos.x;
    end.y = arg0->pos.y - link->pos.y;
    end.z = arg0->pos.z - link->pos.z;
    dx = end.x - start.x;
    dy = end.y - start.y;
    dz = end.z - start.z;
    mid.x = (dx * 0.5f + start.x) - cam[0];
    mid.y = (dy * 0.5f + start.y) - cam[1];
    mid.z = (dz * 0.5f + start.z) - cam[2];
    nx = dy * mid.z - mid.y * dz;
    ny = dz * mid.x - mid.z * dx;
    nz = dx * mid.y - mid.x * dy;
    len = nx * nx + ny * ny + nz * nz;
    if (len == 0.0f) {
        nz = ny = nx = 0.0f;
    } else {
        scale = arg0->width / sqrtf(len);
        nx = nx * scale;
        ny = ny * scale;
        nz = nz * scale;
    }
    vertices->x = start.x - nx;
    vertices->y = start.y - ny;
    vertices->z = start.z - nz;
    vertices->alpha = arg0->alpha;
    vertices->flag = 0;
    vertices++;
    vertices->x = end.x - nx;
    vertices->y = end.y - ny;
    vertices->z = end.z - nz;
    vertices->alpha = 0;
    vertices->flag = 0;
    vertices++;
    vertices->alpha = 0;
    vertices->flag = 0;
    vertices->x = end.x + nx;
    vertices->y = end.y + ny;
    vertices->z = end.z + nz;
    vertices++;
    vertices->x = start.x + nx;
    vertices->y = start.y + ny;
    vertices->z = start.z + nz;
    vertices->alpha = arg0->alpha;
    vertices->flag = 0;
    return result;
}

typedef struct {
    s32 unk0;
    u8 unk4;
} GameStruct151C2E4C;

s32 func_151C2E4C(GameStruct151C2E4C *a0, GameStruct151C2E4C *a1) {
    if (a0 == a1) {
        return 0;
    }
    if (a0->unk0 == 0) {
        return 0;
    }
    if (a0->unk4 == 0xFF) {
        return 0;
    }
    return 1;
}

typedef struct {
    s32 unk0;
    u8 unk4;
    u8 pad[0x122];
    u8 unk127;
} GameStruct151C2E94;

s32 func_151C2E94(GameStruct151C2E94 *a0, GameStruct151C2E94 *a1) {
    if (a0 == a1) {
        return 0;
    }
    if (a0->unk0 == 0) {
        return 0;
    }
    if (a0->unk4 == 0xFF) {
        return 0;
    }
    if (a0->unk127 == 0xFF) {
        return 0;
    }
    return 1;
}

s32 func_151D4DAC(void *, u8 *, Vec3f *, Vec3f *, Game1F1F60Hit *, s32, Vec3f *, u8, u8);

/* Impact handler: the projectile hit actor (at hitPos) during its ray cast. */
s32 func_151C2EF0(void *actor, u8 *owner, Game1EF500Trail *trail, Vec3f *hitPos, Vec3f *hitPoint,
                  Game1F1F60Hit *hit) {
    return func_151D4DAC(actor, owner, hitPos, hitPoint, hit, trail->trail.fieldA4, &trail->trail.path.end,
                         trail->unkC, trail->unk1);
}

extern f32 D_800AAA3C;
extern f32 D_800AAA40;
extern u8 D_800DCA20;
extern f32 D_800DCA24;
void func_15102920(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32);
void func_15152B38(void *, u8, s32);
s32 func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);

typedef struct {
    /* 0x0 */ f32 a;
    /* 0x4 */ f32 b;
    /* 0x8 */ f32 c;
} GameVec151C2F48;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ GameVec151C2F48 unk30;
    /* 0x3C */ u8 pad3C[0x6C - 0x3C];
    /* 0x6C */ u8 unk6C[0x88 - 0x6C];
    /* 0x88 */ s32 unk88;
} GameSub151C2F48;

typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0x110 - 0xD];
    /* 0x110 */ GameSub151C2F48 unk110;
} GameObj151C2F48;

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ GameVec151C2F48 unk08;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
    /* 0x4A */ u8 unk4A;
    /* 0x4B */ u8 unk4B;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ u8 unk4E;
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 unk54;
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
} GameSpawn151C2F48;

void func_151C2F48(GameObj151C2F48 *arg0) {
    GameSub151C2F48 *sub;
    Header header;
    Header2 header2;

    sub = &arg0->unk110;
    func_15102920((func_150ADA68() * 30.0f) + 8.0f, 0xFF, sub->unk6C, &sub->unk30, (func_150ADA20() % 46U) + 0x19, 1, 1, sub->unk88, arg0->unkC, arg0->unk1);
    if (func_150ADA68() < (D_800AAA3C * D_800DCA24)) {
        header.unk0 = 3;
        header.unk1 = -1;
        header.unk2 = (func_150ADA20() % 7U) + 4;
        header.unk4 = 0;
        header2.unk0 = (s32) sub->unk30.a;
        header2.unk4 = (s32) sub->unk30.b;
        header2.unk8 = (s32) sub->unk30.c;
        func_151602C0(&header, &header2, (func_150ADA20() % 9U) + 0xC, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg0->unkC, arg0->unk1);

        {
            GameSpawn151C2F48 sp54;

            sp54.unk00 = 5 >> D_800DCA20;
            sp54.unk04 = 4 >> D_800DCA20;
            sp54.unk08 = sub->unk30;
            sp54.unk14 = 8.25f;
            sp54.unk18 = D_800AAA40;
            sp54.unk1C = 0.0f;
            sp54.unk20 = 0.0f;
            sp54.unk24 = 7.0f;
            sp54.unk28 = 16.0f;
            sp54.unk2C = 0;
            sp54.unk2E = 0xFF;
            sp54.unk30 = -0x3D;
            sp54.unk32 = 0x50;
            sp54.unk34 = 3;
            sp54.unk38 = 1;
            sp54.unk3C = 0xC;
            sp54.unk3E = 0xA;
            sp54.unk40 = 1;
            sp54.unk42 = 4;
            sp54.unk43 = 2;
            sp54.unk44 = 3;
            sp54.unk45 = 0xFF;
            sp54.unk46 = 0xFF;
            sp54.unk47 = 0xFF;
            sp54.unk48 = 0xFF;
            sp54.unk49 = 0;
            sp54.unk4A = 0;
            sp54.unk4B = 0;
            sp54.unk4C = 0;
            sp54.unk4D = 0xFF;
            sp54.unk4E = 0xFF;
            sp54.unk4F = 0xFF;
            sp54.unk50 = 0xFF;
            sp54.unk51 = 0;
            sp54.unk52 = 0;
            sp54.unk53 = 0;
            sp54.unk54 = 0;
            sp54.unk55 = 0xFF;
            sp54.unk56 = 0;
            sp54.unk57 = 3;
            sp54.unk58 = 0x24;
            sp54.unk5C = 0x200005;
            sp54.unk60 = 0x60600;
            sp54.unk64 = 0xA;
            sp54.unk66 = 0x19;
            sp54.unk68 = 1;
            sp54.unk6A = 0;
            sp54.unk6C = 1.0f;
            sp54.unk70 = -1;
            sp54.unk71 = 0;
            sp54.unk72 = -1;
            sp54.unk73 = -1;
            func_15152B38(&sp54, arg0->unkC, arg0->unk1);
        }
    }
}

typedef struct {
    s32 field0;
    s32 field4;
    struct17 position8;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    s16 field2C;
    s16 field2E;
    s16 field30;
    s16 field32;
    s32 field34;
    s32 field38;
    s16 field3C;
    s16 field3E;
    s16 field40;
    u8 fields42[23];
    s32 field5C;
    s32 field60;
    s16 field64;
    s16 field66;
    s16 field68;
    u8 field6A;
    f32 field6C;
    s8 fields70[4];
} GameEmitterParticle151C329C;

typedef struct {
    s32 field0;
    s32 field4;
    s16 field8;
    s16 fieldA;
    s32 fieldC;
    s32 field10;
    u8 colors14[9];
    u8 field1D;
    s16 field1E;
    s16 field20;
    s16 field22;
    f32 field24;
    f32 field28;
    f32 field2C;
    struct17 position30;
    f32 field3C;
    f32 field40;
    f32 field44;
    f32 field48;
    f32 field4C;
    f32 field50;
    f32 field54;
    s32 field58;
    s32 field5C;
    s8 fields60[6];
    u8 fields66[0xA];
} GameEmitterSpawn151C329C;

typedef struct {
    u8 field0;
    s8 field1;
    s16 field2;
    u8 field4;
} GameEmitterLight151C329C;

extern f32 D_800AAA44;
extern f32 D_800AAA48;
extern f32 D_800AAA4C;
extern f32 D_800AAA50;
extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_151C329C(struct17 *arg0, u8 arg1, s32 arg2) {
    GameEmitterParticle151C329C descriptor;
    GameEmitterSpawn151C329C spawn;
    GameEmitterLight151C329C light;
    Header2 coordinates;

    descriptor.field0 = 5;
    descriptor.field4 = 7;
    descriptor.position8 = *arg0;
    descriptor.field14 = D_800AAA44;
    descriptor.field18 = D_800AAA48;
    descriptor.field1C = D_800AAA4C;
    descriptor.field20 = D_800AAA50;
    descriptor.field24 = 15.0f;
    descriptor.field28 = 30.0f;
    descriptor.field2C = 0;
    descriptor.field2E = 0xFF;
    descriptor.field30 = -0x40;
    descriptor.field32 = 0x50;
    descriptor.field34 = 3;
    descriptor.field38 = 1;
    descriptor.field3C = 0x14;
    descriptor.field3E = 0xF;
    descriptor.field40 = 1;
    descriptor.fields42[0] = 4;
    descriptor.fields42[1] = 2;
    descriptor.fields42[2] = 3;
    descriptor.fields42[3] = 0xFF;
    descriptor.fields42[4] = 0xFF;
    descriptor.fields42[5] = 0xFF;
    descriptor.fields42[6] = 0xFF;
    descriptor.fields42[7] = 0;
    descriptor.fields42[8] = 0;
    descriptor.fields42[9] = 0;
    descriptor.fields42[10] = 0;
    descriptor.fields42[11] = 0xFF;
    descriptor.fields42[12] = 0xFF;
    descriptor.fields42[13] = 0xFF;
    descriptor.fields42[14] = 0xFF;
    descriptor.fields42[15] = 0;
    descriptor.fields42[16] = 0;
    descriptor.fields42[17] = 0;
    descriptor.fields42[18] = 0;
    descriptor.fields42[19] = 0xFF;
    descriptor.fields42[20] = 0;
    descriptor.fields42[21] = 3;
    descriptor.fields42[22] = 0x24;
    descriptor.field5C = 0x200005;
    descriptor.field60 = 0x60600;
    descriptor.field64 = 8;
    descriptor.field66 = 0x1F;
    descriptor.field68 = 1;
    descriptor.field6A = 0;
    descriptor.field6C = 1.0f;
    descriptor.fields70[0] = -1;
    descriptor.fields70[1] = 0;
    descriptor.fields70[2] = -1;
    descriptor.fields70[3] = -1;
    func_15152B38(&descriptor, arg1, arg2);
    spawn.field1D = 0x2B;
    spawn.field8 = 0x4403;
    spawn.field0 = 0x200005;
    spawn.field4 = 0x20000;
    spawn.fieldA = (func_150ADA20() % 7U) + 6;
    spawn.fieldC = 0;
    spawn.field10 = 0;
    spawn.colors14[0] = 0xFF;
    spawn.colors14[1] = 0xFF;
    spawn.colors14[2] = 0xFF;
    spawn.colors14[3] = 0xFF;
    spawn.colors14[4] = 0xFF;
    spawn.colors14[5] = 0xFF;
    spawn.colors14[6] = 0xFF;
    spawn.colors14[7] = 0xFF;
    spawn.colors14[8] = 0xFF;
    spawn.field28 = spawn.field2C = func_150ADA68() * 196.0f + 106.0f;
    spawn.position30 = *arg0;
    spawn.field1E = 3;
    spawn.field20 = 0x55;
    spawn.field22 = 1;
    spawn.field3C = 0.0f;
    spawn.field40 = 0.0f;
    spawn.field44 = 0.0f;
    spawn.field48 = 0.0f;
    spawn.field4C = 0.0f;
    spawn.field50 = 0.0f;
    spawn.field54 = 0.0f;
    spawn.field24 = 1.0f;
    spawn.field58 = 1 | ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x8000 | 0x4000 | 0x200;
    spawn.fields60[0] = 6;
    spawn.fields60[1] = 6;
    spawn.fields60[2] = -1;
    spawn.fields60[3] = -1;
    spawn.fields60[4] = -1;
    spawn.fields60[5] = 4;
    func_15130280(&spawn, 1, 0, 0, arg1, arg2);
    light.field0 = 3;
    light.field1 = -1;
    light.field2 = (func_150ADA20() % 7U) + 6;
    light.field4 = 0;
    coordinates.unk0 = (s32)arg0->unk0;
    coordinates.unk4 = (s32)arg0->unk4;
    coordinates.unk8 = (s32)arg0->unk8;
    func_151602C0((Header *)&light, &coordinates, (func_150ADA20() & 1) + 5,
                 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg1, arg2);
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
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
    /* 0x30 */ struct17 pos;
    /* 0x3C */ struct17 unk3C;
    /* 0x48 */ struct17 vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 unk58;
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
} GameParticle151C36D8; /* 0x70 */

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_151C36D8(struct17 *arg0, u8 arg1, s32 arg2) {
    GameSpawn151C2F48 spawn;
    GameParticle151C36D8 particle;
    Header header;
    Header2 header2;

    spawn.unk00 = 8;
    spawn.unk04 = 6;
    spawn.unk08 = *(GameVec151C2F48 *)arg0;
    spawn.unk14 = 2.599999905f;
    spawn.unk18 = 2.169999838f;
    spawn.unk1C = -0.9690000415f;
    spawn.unk20 = 0.7180000544f;
    spawn.unk24 = 10.0f;
    spawn.unk28 = 8.0f;
    spawn.unk2C = 0;
    spawn.unk2E = 0xFF;
    spawn.unk30 = -0x40;
    spawn.unk32 = 0x50;
    spawn.unk34 = 3;
    spawn.unk38 = 0;
    spawn.unk3C = 0xF;
    spawn.unk3E = 0xA;
    spawn.unk40 = 1;
    spawn.unk42 = 4;
    spawn.unk43 = 2;
    spawn.unk44 = 3;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0xFF;
    spawn.unk47 = 0xFF;
    spawn.unk48 = 0xFF;
    spawn.unk49 = 0;
    spawn.unk4A = 0;
    spawn.unk4B = 0;
    spawn.unk4C = 0;
    spawn.unk4D = 0xFF;
    spawn.unk4E = 0xFF;
    spawn.unk4F = 0xFF;
    spawn.unk50 = 0xFF;
    spawn.unk51 = 0;
    spawn.unk52 = 0;
    spawn.unk53 = 0;
    spawn.unk54 = 0;
    spawn.unk55 = 0xFF;
    spawn.unk56 = 0;
    spawn.unk57 = 3;
    spawn.unk58 = 0x24;
    spawn.unk5C = 0x200005;
    spawn.unk60 = 0x60600;
    spawn.unk64 = 8;
    spawn.unk66 = 0x1F;
    spawn.unk68 = 1;
    spawn.unk6A = 0;
    spawn.unk6C = 1.0f;
    spawn.unk70 = -1;
    spawn.unk71 = 0;
    spawn.unk72 = -1;
    spawn.unk73 = -1;
    func_15152B38(&spawn, arg1, arg2);

    particle.unk1D = 0x2B;
    particle.unk8 = 0x4403;
    particle.unk0 = 0x200005;
    particle.unk4 = 0x20000;
    particle.unkA = (func_150ADA20() % 5U) + 4;
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
    particle.pos = *arg0;
    particle.unk1E = 2;
    particle.unk20 = 0x7F;
    particle.unk22 = 1;
    particle.unk3C.unk0 = 0.0f;
    particle.unk3C.unk4 = 0.0f;
    particle.unk3C.unk8 = 0.0f;
    particle.vel.unk0 = 0.0f;
    particle.vel.unk4 = 0.0f;
    particle.vel.unk8 = 0.0f;
    particle.unk54 = 0.0f;
    particle.unk24 = 1.0f;
    particle.unk58 = ((func_150ADA20() & 1) ? 0x40 : 0) | (((func_150ADA20() & 1) ? 0x80 : 0) | 1) | 0x8000 | 0x4000 | 0x200;
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
    header2.unk0 = (s32) arg0->unk0;
    header2.unk4 = (s32) arg0->unk4;
    header2.unk8 = (s32) arg0->unk8;
    func_151602C0(&header, &header2, (func_150ADA20() & 1) + 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg1, arg2);
}

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ struct17 pos;
    /* 0x3C */ u8 pad3C[0x60 - 0x3C];
    /* 0x60 */ struct17 prev;
    /* 0x6C */ u8 unk6C[0x88 - 0x6C];
} GameSub151C3B0C;

typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0x110 - 0xD];
    /* 0x110 */ GameSub151C3B0C unk110;
    /* 0x198 */ s32 unk198;
} GameObj151C3B0C;

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
} GameDebris151C3B0C; /* 0x28 */

typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ u8 pad06[2];
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ struct17 pos;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 pad4D[7];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} GameSpark151C3B0C; /* 0x58 */

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
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
    /* 0x38 */ struct17 unk38;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s8 unk60;
    /* 0x61 */ s8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 pad65[3];
    /* 0x68 */ f32 unk68;
} GameSmoke151C3B0C; /* 0x6C */

void func_15153634(void *, u8, u8, s32);

void func_151C3B0C(GameObj151C3B0C *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u8 arg5, u8 arg6, u8 arg7) {
    s32 kind;
    GameSub151C3B0C *sub;
    s32 type;

    kind = arg0->unk198 & 0x1F;
    if ((D_800BE9F0 == 0x2B) || (D_800BE9F0 == 0x30)) {
        switch (kind) {
        case 1:
            type = 3;
            break;
        case 0:
        case 5:
        default:
            type = 4;
            break;
        }
    } else if ((kind == 0xB) && (D_800BE9F0 == 4)) {
        type = 1;
    } else if (((kind == 1) || (kind == 0xA)) && (D_800BE9F0 == 0xE)) {
        type = 2;
    } else if (D_800BE9F0 == 0x34) {
        type = 3;
    } else if (D_800BE9F0 == 0x27) {
        type = 5;
    } else {
        type = 0;
        arg3 = arg2 = arg1 = 0.0f;
    }
    if (func_150ADA68() < arg1) {
        GameDebris151C3B0C debris;
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
        debris.unk20 = arg5;
        debris.unk22 = 0x20;
        debris.unk24 = 7;
        sub = &arg0->unk110;
        func_1513C650((s32)&debris, 0, 0, (s32)sub->unk6C, sub->pos.unk0, sub->pos.unk4, sub->pos.unk8,
                      scale, scale, func_150ADA20() & 0xFF, 0, 0, 0, 0, arg0->unkC, arg0->unk1);
    }
    if (func_150ADA68() < arg2) {
        GameSpark151C3B0C spark;
        const s32 shade[6] = { 0x6D, 0x6D, 0x6E, 0x6E, 0x6D, 0x6D };

        sub = &arg0->unk110;
        spark.unk00 = shade[type];
        spark.unk01 = 0;
        spark.unk02 = 0x3B03;
        spark.unk04 = (func_150ADA20() % 6U) + 0xD;
        spark.unk08 = 0;
        spark.unk0C = 0;
        spark.unk10 = 0xFF;
        spark.unk11 = 0xFF;
        spark.unk12 = 0xFF;
        spark.unk13 = 0xFF;
        spark.unk14 = (func_150ADA68() * 5.0f) + 7.0f;
        spark.unk18 = (func_150ADA68() * 40.0f) + 10.0f;
        spark.pos = sub->pos;
        spark.unk28 = sub->pos.unk0 - sub->prev.unk0;
        spark.unk2C = sub->pos.unk4 - sub->prev.unk4;
        spark.unk30 = sub->pos.unk8 - sub->prev.unk8;
        spark.unk34 = 1.269999981f;
        spark.unk38 = 1.0f;
        spark.unk3C = 56.81999969f;
        spark.flags = (D_800BE616 ? 0x40000000 : 0) | 0x08DC0009;
        spark.unk44 = (func_150ADA20() % 76U) + 0xB4;
        spark.unk45 = 0xFF;
        spark.unk46 = 0;
        spark.unk47 = 7;
        spark.unk48 = 0;
        spark.unk4C = arg7;
        spark.unk54 = 8;
        spark.unk56 = 0x1F;
        func_1513D2F0(&spark, (s32)&D_800A4AA0, 0x22, 0, 0, 0x1C, 2, 0, 0, 0, arg0->unkC, arg0->unk1);
    }
    if (func_150ADA68() < arg3) {
        GameSmoke151C3B0C smoke;
        const u8 colorA[6][3] = { { 0xB7, 0x77, 0x34 }, { 0xB7, 0x77, 0x34 }, { 0xAA, 0x52, 0x1E },
                            { 0xAA, 0x52, 0x1E }, { 0x98, 0x92, 0x79 }, { 0xFF, 0xC0, 0x00 } };
        const u8 colorB[6][3] = { { 0x5C, 0x30, 0x00 }, { 0x5C, 0x30, 0x00 }, { 0x00, 0x00, 0x00 },
                            { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x8D }, { 0x8D, 0x00, 0x00 } };

        smoke.unk0 = 5;
        smoke.unk2 = 0xF;
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
        smoke.unk38 = arg0->unk110.pos;
        smoke.unk44 = 0;
        smoke.unk46 = -0x28;
        smoke.unk48 = 0xFF;
        smoke.unk4A = 0x28;
        smoke.unk4C = 9.0f;
        smoke.unk50 = 17.0f;
        smoke.unk54 = 0.00800000038f;
        smoke.unk58 = 0.2610000074f;
        smoke.unk5C = (D_800BE616 ? 0x800000 : 0) | 0x40E07;
        smoke.unk60 = 0x10;
        smoke.unk61 = -1;
        smoke.unk62 = 8;
        smoke.unk63 = 6;
        smoke.unk64 = 1;
        smoke.unk68 = 0.8702070117f;
        func_15153634(&smoke, arg6, arg0->unkC, arg0->unk1);
    }
    if (func_150ADA68() < arg4) {
        Header header;
        Header2 header2;
        const u8 color[2][3] = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xA7, 0xA7 } };
        u8 sel;

        header.unk0 = 3;
        header.unk1 = -1;
        sub = &arg0->unk110;
        header.unk2 = (func_150ADA20() % 6U) + 3;
        header.unk4 = 0;
        header2.unk0 = (s32)sub->pos.unk0;
        header2.unk4 = (s32)sub->pos.unk4;
        header2.unk8 = (s32)sub->pos.unk8;
        sel = func_150ADA20() & 1;
        func_151602C0(&header, &header2, (func_150ADA20() % 6U) + 0xA, color[sel][0], color[sel][1], color[sel][2], 0xFF, 0, 0, arg0->unkC, arg0->unk1);
    }
}

/* Eases the projectile's accel toward its drift, once per elapsed step. */
void func_151C436C(Game1EF500Trail *trail, Game1EF500TrailExt *ext, s32 steps) {
    f32 x;
    f32 tx;
    f32 y;
    f32 ty;
    f32 z;
    f32 tz;
    f32 factor;

    if (steps > 0) {
        x = ext->accel.x;
        tx = ext->drift.x;
        y = ext->accel.y;
        ty = ext->drift.y;
        z = ext->accel.z;
        tz = ext->drift.z;
        factor = D_800AAA7C;
        do {
            ext->accel.x += (tx - ext->accel.x) * factor;
            ext->accel.y += (ty - ext->accel.y) * factor;
            ext->accel.z += (tz - ext->accel.z) * factor;
            steps--;
        } while (steps > 0);
    }
}

void func_15081690(void *, f32, f32, f32, f32, f32, f32, Game1F1F60Hit *, f32, s32, s32, s32, s32, s32, s32);

/*
 * Casts a ray along the projectile's flight over the distance it moves this frame; on a hit,
 * runs the impact handler and returns 0.
 */
u8 func_151C43E0(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt) {
    u8 result;
    u8 *owner;
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
    GameFunc151C455CCallback callback;
    struct131 *target;
    s8 handler;

    ext->remaining -= dt;
    alive = TRUE;
    if (ext->remaining <= 0.0f) {
        callback = (GameFunc151C455CCallback)D_800E0940;
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

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} GameStruct151C4644Vec;

typedef struct {
    GameStruct151C4644Vec *unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
} GameStruct151C4644Sub;

typedef struct {
    u8 pad0[1];
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    GameStruct151C4644Sub unk28;
} GameStruct151C4644;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
} GameStruct151C4644Payload;

void func_151C4644(GameStruct151C4644 *arg0) {
    GameStruct151C4644Sub *sub;
    GameStruct151C4644Payload sp48;
    struct260 *temp_v0;

    sub = &arg0->unk28;
    sub->unk4 -= D_800BE9E4;
    if (sub->unk4 < 0) {
        sp48.unk0 = sub->unk1C;
        sp48.unk4 = sub->unk20;
        sp48.unk8 = func_150ADA68() * sub->unk24;
        sp48.unkC = func_150ADA68() * sub->unk24;
        sp48.unk10 = func_150ADA68() * D_800AAA80;
        sp48.unk14 = func_150ADA68() * D_800AAA84;
        sp48.unk18 = sub->unk28;
        sp48.unk1C = sub->unk28;
        sp48.unk20 = 0.0f;
        sp48.unk24 = (func_150ADA68() * sub->unk18) + sub->unk14;
        sp48.unk28 = 0.0f;
        sp48.unk2C = sub->unk0->unk0;
        sp48.unk30 = sub->unk0->unk2;
        sp48.unk34 = sub->unk0->unk4;
        sp48.unk38 = sub->unk2C;
        sp48.unk3C = sub->unk30;
        sp48.unk40 = sub->unk34;

        temp_v0 = func_15149130((s16)((func_150ADA20() % (u32)(sub->unk12 + 1)) + sub->unk10),
                                -1, 0x2B, -1, 1, 0, (struct37 *)0x44, arg0->unkC, arg0->unk1);
        if (temp_v0 != NULL) {
            memcpy((u8 *)temp_v0 + 0x28, &sp48, 0x44);
        }

        sub->unk4 = (func_150ADA20() % (u32)(sub->unkC + 1)) + sub->unk8;
    }
}
