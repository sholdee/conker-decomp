#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

void func_151541B8(Vec3f *, f32, f32, f32, f32, u8, s32);

/* Particle parameters for a bullet impact, filled in by func_151BFC40. */
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s8  unk50;
} ImpactFxConfig; /* size 0x54 */

/* Collision surface (0x12 bytes, 2-byte aligned) as stored in a ray-cast result. */
typedef struct {
    /* 0x00 */ s16 unk00[9];
} HitSurface;

typedef struct {
    /* 0x00 */ u8 pad00[0x4F];
    /* 0x4F */ u8 unk4F; /* (unk4F & 0x60) == 0x40: bullets pass through */
} RayHitPart;

/* A surface hit record: the floor under a point, or the first pass-through surface of a bullet trace. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ HitSurface surface;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s8  unk1C;
    /* 0x1D */ s8  unk1D;
    /* 0x20 */ RayHitPart *part;
} SurfaceHit;

/* Argument block for func_1514FB98: an impact with no surface. */
typedef struct {
    /* 0x00 */ Vec3f vel;
    /* 0x0C */ f32 scale;
    /* 0x10 */ ImpactFxConfig config;
} ImpactFx; /* size 0x64 */

/* Argument block for func_1514FBFC: an impact against a collision surface. */
typedef struct {
    /* 0x00 */ Vec3f vel;
    /* 0x0C */ HitSurface surface;
    /* 0x20 */ f32 scale;
    /* 0x24 */ ImpactFxConfig config;
} SurfaceImpactFx; /* size 0x78 */

/* Argument block for func_1514FCE8. */
typedef struct {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s8  unk58;
} DebrisFx;

/* Fills in the impact particle parameters; fewer particles on busy frames. */
void func_151BFC40(ImpactFxConfig *config, f32 *scale) {
    config->unk00 = 3;
    config->unk04 = 4 >> ((D_80082FA0 >= 2) ? 1 : 0);
    *scale = 494.0f;
    config->unk14 = 0.06089999899f;
    config->unk18 = 0.02520000003f;
    config->unk1C = 45.0f;
    config->unk20 = 53.0f;
    config->unk24 = 203.0f;
    config->unk28 = 414.0f;
    config->unk2C = 7;
    config->unk30 = 3 >> ((D_80082FA0 >= 2 || D_8008FD8C >= 8) ? 1 : 0);
    config->unk34 = 15.0f;
    config->unk38 = 0.8800000548f;
    config->unk3C = -0.3980000317f;
    config->unk40 = 0.3910000324f;
    config->unk44 = 25;
    config->unk46 = 15;
    config->unk48 = 100;
    config->unk4A = 100;
    config->unk4C = 12;
    config->unk4E = 20;
    config->unk50 = (D_80082FA0 >= 2 || D_8008FD8C >= 8) ? -1 : 0;
}

void func_151C0418(Vec3f *pos, u8 fxOwner, s32 fxList);
void func_151C04F8(Vec3f *pos, u8 fxOwner, s32 fxList);
void func_151C05A4(Vec3f *pos, u8 fxOwner, s32 fxList);
void func_151C05F0(Vec3f *pos, u8 fxOwner, s32 fxList);
void func_1514FB98(ImpactFx *, u8, s32);
void func_1514FBFC(SurfaceImpactFx *, u8, s32);
void func_1514FCE8(DebrisFx *, u8, s32);
void func_1504715C(SurfaceHit *, Actor *);
s32 func_15046C80(Vec3f *, s32, f32, SurfaceHit *);
void func_151C0360(HitSurface *surface, Vec3f *pos, u8 fxOwner, s32 fxList);
void func_151C0644(Vec3f *pos, u8 fxOwner, s32 fxList);
s32 func_15144E80(HitSurface *, Vec3f *, Vec3f *, Vec3f *);

/* Bullet hit with no surface: sparks and smoke at pos, plus impact particles thrown back along -dir. */
void func_151BFDA0(Vec3f *pos, Vec3f *dir, u8 withParticles, u8 fxOwner, s32 fxList) {
    ImpactFx fx;

    func_151C0418(pos, fxOwner, fxList);
    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);
    if (withParticles) {
        fx.vel.x = -dir->x;
        fx.vel.y = -dir->y;
        fx.vel.z = -dir->z;
        func_151BFC40(&fx.config, &fx.scale);
        fx.config.pos = *pos;
        func_1514FB98(&fx, fxOwner, fxList);
    }
}

/* Bullet hit against a collision surface (or none). */
void func_151BFE84(HitSurface *surface, Vec3f *pos, Vec3f *dir, u8 withParticles, u8 fxOwner, s32 fxList) {
    Vec3f v0;
    Vec3f v1;
    Vec3f v2;

    // matching: taking the addresses keeps fxOwner/fxList in their stack slots (reloaded at each use)
    func_151C0418(pos, *(u8 *)&fxOwner, *(s32 *)&fxList);
    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);
    if (surface != NULL && withParticles) {
        if (func_15144E80(surface, &v0, &v1, &v2) != 0 && v2.y < 0.0f) {
            func_151C0644(pos, fxOwner, fxList);
        }
    }
    if (surface != NULL && withParticles) {
        func_151C0360(surface, pos, fxOwner, fxList);
    }
    if (withParticles) {
        if (surface != NULL) {
            SurfaceImpactFx fx;

            fx.vel.x = -dir->x;
            fx.vel.y = -dir->y;
            fx.vel.z = -dir->z;
            fx.surface = *surface;
            fx.config.pos = *pos;
            func_151BFC40(&fx.config, &fx.scale);
            func_1514FBFC(&fx, fxOwner, fxList);
        } else {
            ImpactFx fx;

            fx.vel.x = -dir->x;
            fx.vel.y = -dir->y;
            fx.vel.z = -dir->z;
            func_151BFC40(&fx.config, &fx.scale);
            fx.config.pos = *pos;
            func_1514FB98(&fx, fxOwner, fxList);
        }
    }
}

/* Bullet hit on an object: sparks and debris at pos, plus a ground mark below it if hitObj is set. */
void func_151C0098(Vec3f *pos, Actor *hitObj, u8 withParticles, u8 fxOwner, s32 fxList) {
    DebrisFx fx;
    Vec3f probe;
    SurfaceHit floor;
    Vec3f markPos;

    func_151C0418(pos, fxOwner, fxList);
    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);

    // matching: taking the address keeps withParticles in its stack slot (reloaded at each use)
    if (*(u8 *)&withParticles) {
        fx.unk00 = 0;
        fx.unk02 = 255;
        fx.unk04 = -64;
        fx.unk06 = 71;
        fx.unk08 = 6;
        fx.unk0C = 4;
        fx.pos = *pos;
        fx.unk1C = 23.0f;
        fx.unk20 = 30.0f;
        fx.unk24 = 45.0f;
        fx.unk28 = 53.0f;
        fx.unk2C = 203.0f;
        fx.unk30 = 414.0f;
        fx.unk34 = 7;
        fx.unk38 = 3;
        fx.unk3C = 15.0f;
        fx.unk40 = 0.8720000386f;
        fx.unk44 = -0.3980000317f;
        fx.unk48 = 0.3640000224f;
        fx.unk4C = 25;
        fx.unk4E = 15;
        fx.unk50 = 100;
        fx.unk52 = 100;
        fx.unk54 = 12;
        fx.unk56 = 20;
        fx.unk58 = 0;
        func_1514FCE8(&fx, fxOwner, fxList);
    }

    if (withParticles && hitObj != NULL) {
        probe.x = pos->x;
        probe.y = pos->y + 100.0f;
        probe.z = pos->z;
        func_1504715C(&floor, hitObj);
        if (func_15046C80(&probe, 0, pos->y - 1000.0f, &floor) != 0) {
            markPos.x = probe.x;
            markPos.y = floor.height;
            markPos.z = probe.z;
            func_151C0360(&floor.surface, &markPos, fxOwner, fxList);
            func_151C0644(pos, fxOwner, fxList);
        }
    }
}

/* Whether obj is a valid target: not in the ignore list, not self, active, and with a valid id. */
s32 func_151C02E4(Actor *obj, Actor *self, s32 ignoreCount, Actor **ignore) {
    u8 i;

    for (i = 0; i < ignoreCount; i++) {
        if (obj == ignore[i]) {
            return FALSE;
        }
    }
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

void func_150E7FEC(f32, s32, HitSurface *, Vec3f *, s32, s32, s32, s32, s32, s32, s32, s32);

void func_151C0360(HitSurface *surface, Vec3f *pos, u8 fxOwner, s32 fxList) {
    // matching: the f64 member gives the random value an 8-byte aligned stack slot
    union { u32 w; f64 align; } shade;
    f32 rand;
    u32 duration;

    rand = func_150ADA68();
    shade.w = func_150ADA20();
    duration = func_150ADA20();
    func_150E7FEC(rand * 75.0f + 75.0f, (u8)(shade.w % 56 + 200), surface, pos,
                  duration % 205 + 299, 1, 1, 0, 0, 0, fxOwner, 0);
}

s32 func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);

void func_151C0418(Vec3f *pos, u8 fxOwner, s32 fxList) {
    Header header;
    Header2 header2;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = func_150ADA20() % 11 + 20;
    header.unk4 = 0;

    header2.unk0 = pos->x;
    header2.unk4 = pos->y;
    header2.unk8 = pos->z;

    func_151602C0(&header, &header2, func_150ADA20() % 121 + 60, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, fxOwner, fxList);
}

void func_151C04F8(Vec3f *pos, u8 fxOwner, s32 fxList) {
    // matching: the f64 member gives the random value an 8-byte aligned stack slot
    union { f32 f; f64 align; } rand;
    u32 shade;

    rand.f = func_150ADA68();
    shade = func_150ADA20();
    func_151541B8(pos, rand.f * 4.0f + 14.0f, 1.182089925f, (f32)(shade % 56 + 200), 0.0f, fxOwner, fxList);
}

void func_151D5334(Vec3f *, f32, f32, f32, s32, u8, s32);

void func_151C05A4(Vec3f *pos, u8 fxOwner, s32 fxList) {
    func_151D5334(pos, 800.0f, 2000.0f, 0.0005f, 5, fxOwner, fxList);
}

void func_151C05F0(Vec3f *pos, u8 fxOwner, s32 fxList) {
    func_151D5404(pos, 800.0f, 2000.0f, 0.0005f, 15, 20, fxOwner, fxList);
}

void func_150E83AC(Vec3f *, s16, u8, s32);

void func_151C0644(Vec3f *pos, u8 fxOwner, s32 fxList) {
    func_150E83AC(pos, func_150ADA20() % 62 + 120, fxOwner, fxList);
}

/* Result of a ray cast (func_15081690). */
typedef struct {
    /* 0x00 */ Actor *obj;
    /* 0x04 */ f32 dist;
    /* 0x08 */ Vec3f hitPos;
    /* 0x14 */ Vec3f unk14;
    /* 0x20 */ Vec3f unk20;
    /* 0x2C */ Vec3f start;
    /* 0x38 */ Vec3f dir;
    /* 0x44 */ HitSurface surface;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8  unk58;
    /* 0x59 */ u8  hitType; /* 0: nothing, 1: end of trace, >= 2: hit */
    /* 0x5A */ u8  unk5A;
    /* 0x5B */ u8  pad5B;
    /* 0x5C */ RayHitPart *part;
    /* 0x60 */ s32 unk60;
} RayHit;

/* Objects the bullet trace ignores (see func_151C1940). */
typedef struct {
    /* 0x00 */ s32 count;
    /* 0x04 */ Actor *objs[3];
} IgnoreList;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s16 *unk14; /* [7..9]: position */
} TrailFx;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ Vec3f pos;
} FollowFx;

typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ s32 unk110;
} Bullet88;

/* Per-bullet state, stored at Bullet.state (0xD0 bytes). */
typedef struct {
    /* 0x000 */ RayHit ray;
    /* 0x064 */ f32 lifetime;
    /* 0x068 */ f32 speed;
    /* 0x06C */ Actor *owner;
    /* 0x070 */ IgnoreList ignore;
    /* 0x080 */ TrailFx *trail;
    /* 0x084 */ FollowFx *follow;
    /* 0x088 */ Bullet88 *unk88;
    /* 0x08C */ void *unk8C;
    /* 0x090 */ u16 sound;
    /* 0x092 */ u8 impactType;
    /* 0x093 */ u8 pad93;
    /* 0x094 */ f32 passTimer;
    /* 0x098 */ Vec3f passPos;
    /* 0x0A4 */ SurfaceHit pass;
    /* 0x0C8 */ u8 flags; /* bit 0: the trace went through a pass-through surface */
    /* 0x0C9 */ u8 padC9[3];
    /* 0x0CC */ s32 mode;
} BulletState;

/* A bullet object, spawned by func_151C0698 and updated by func_151C1180. */
typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 fxList;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 fxOwner;
    /* 0x00D */ u8 padD[0x2B];
    /* 0x038 */ Vec3f pos;
    /* 0x044 */ Vec3f vel;
    /* 0x050 */ u8 pad50[0x120];
    /* 0x170 */ BulletState state;
} Bullet;

/* A positional sound; see func_151C110C. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[0x10];
    /* 0x18 */ Bullet *bullet;
} SoundEmitter;

f32 func_15144AA8(u8, Camera *);
void func_15145740(Actor *, Vec3f *, void *, s32, f32);
s32 func_151D5B6C(Vec3f *, Vec3f *, Actor *, s32, IgnoreList *);
Bullet *func_15132A4C(ParticleSpawn *, s32, s32, s32, u8, s32);
void func_15145974(Vec3f *, f32 *, f32 *);
FollowFx *func_151B8908(Bullet *);
Bullet88 *func_151B8400(Bullet *);
void *func_151B8668(Bullet *, u8, s32);
void func_15145EA4(Vec3f **, Vec3f **, MtxF *, s32);
void func_15102B38(Actor *, s32, Vec3f *, Vec3f *, f32 *, s32, s32, f32,
                   Vec3f *, s32, s32, s32, u8, s32);
void func_15103254(s16, u8, f32, Vec3f *, s32, u8, s32);
void func_151C1798(Actor *obj, Vec3f *pos);
void func_151C1860(Vec3f *pos, u8 fxOwner, s32 fxList);
s32 func_151C196C(RayHit *ray, Vec3f *start, Vec3f *dir, Vec3f *aimPos, Vec3f *aimDir,
                  Actor *shooter, IgnoreList *ignore, Vec3f *passPos, f32 *passDist,
                  SurfaceHit *pass, s32 mode);
s32 func_151C110C(SoundEmitter *emitter, s32 arg1, void **arg2, s32 arg3, s32 arg4, s32 arg5, s16 *arg6);

/* Fires a bullet from shooter (aimed by aimer's view, if given) and spawns its object and effects. */
void func_151C0698(Actor *shooter, Actor *aimer, IgnoreList *ignore, u8 impactType, u8 fxOwner, s32 fxList) {
    RayHit ray;
    f32 passDist;
    Vec3f passPos;
    SurfaceHit pass;
    u8 useAim;
    Vec3f pos;
    Vec3f dir;
    Vec3f aimPos;
    Vec3f aimDir;
    s32 mode;
    Vec3f *src;
    Actor *who;
    f32 angle;
    u8 scratch[0x14];
    BulletState state;
    ParticleSpawn desc;
    void *obj; // matching: one local (one stack slot) holds the camera, then the new bullet
    BulletState *bulletState;
    Header header;
    Header2 header2;

    passDist = 0.0f;
    if (D_800BE616 != 0) {
        if (shooter->unk1D4 != NULL) {
            func_151C1798(shooter, &pos);
        } else {
            pos.x = shooter->x_position;
            pos.y = shooter->y_position + 65.0f;
            pos.z = shooter->z_position;
        }
    } else {
        src = &shooter->unk31C->aimEye;
        obj = shooter->camera;
        if (obj != NULL) {
            if (shooter->unk65 != 0) {
lbl1: // matching: an unused label splits IDO's basic block here (register choice)
                pos.x = src->x;
                pos.y = src->y + -13.0f;
                pos.z = src->z;
            } else if (shooter->unk31C->aimState != 0) {
                extern u8 D_800BE616; // matching: a declaration makes this an IDO scope block (register choice)

                angle = func_15144AA8(((Camera *)obj)->playerIndex, obj) * 0.01745329238f;
                pos.x = cosf(angle) * 25.0f + src->x;
                pos.y = src->y;
                pos.z = src->z - sinf(angle) * 25.0f;
            } else if (shooter->unk1D4 != NULL) {
                func_151C1798(shooter, &pos);
            } else {
                pos.x = shooter->x_position;
                pos.y = shooter->y_position + 65.0f;
                pos.z = shooter->z_position;
            }
        } else if (shooter->unk1D4 != NULL) {
            func_151C1798(shooter, &pos);
        } else {
            pos.x = shooter->x_position;
            pos.y = shooter->y_position + 65.0f;
            pos.z = shooter->z_position;
        }
    }

    if ((shooter->unk31C != NULL && shooter->unk31C->aimState != 0) || aimer != NULL) {
        extern u8 D_800BE616; // matching: a declaration makes this an IDO scope block (register choice)

        who = (aimer != NULL) ? aimer : shooter;

        useAim = TRUE;
        src = &who->unk31C->aimEye;
        aimPos = *src;
        aimDir.x = who->unk31C->aimDir.x;
        aimDir.y = who->unk31C->aimDir.y;
        aimDir.z = who->unk31C->aimDir.z;
    } else {
        func_15145740(shooter, &dir, scratch, 0, 0.0f);
        useAim = FALSE;
        dir.x *= 0.001000000047f;
        dir.y *= 0.001000000047f;
        dir.z *= 0.001000000047f;
    }

    if (D_800BE9F0 == 36) {
        mode = 0;
    } else {
        mode = func_151D5B6C(useAim ? &aimPos : &pos, useAim ? &aimDir : &dir, shooter, 3, ignore);
    }

    if (func_151C196C(&ray, &pos, useAim ? NULL : &dir, useAim ? &aimPos : NULL,
                      useAim ? &aimDir : NULL, shooter, ignore, &passPos, &passDist, &pass,
                      mode) != 0) {
        extern u8 D_800BE616; // matching: a declaration makes this an IDO scope block (register choice)

        state.mode = mode;
        state.trail = NULL;
        state.ray = ray;
        state.follow = NULL;
        desc.radius = 1.0f;
        desc.elasticity = 1.0f;
        desc.sizeXZ = 0.04700000212f;
        desc.sizeY = 0.04700000212f;
        desc.rot.x = 0.0f;
        desc.rot.y = 0.0f;
        desc.rot.z = 0.0f;
        desc.scale.x = 1.0f;
        desc.scale.y = 1.0f;
        desc.scale.z = 1.0f;
        desc.pos = pos;
        desc.gravity = 0.0f;
        desc.flags = 0x500920;
        desc.lifetime = 300;
        desc.model = 47;
        desc.variant = 1;
        state.owner = shooter;
        state.ignore = *ignore;
        state.passTimer = passDist * 0.004999999888f;
        state.passPos = passPos;
        state.flags = (s8)((passDist != 0) ? 1 : 0);
        state.pass = pass;
        state.speed = 200.0f;
        desc.vel.x = state.ray.dir.x * state.speed;
        desc.vel.y = state.ray.dir.y * state.speed;
        desc.vel.z = state.ray.dir.z * state.speed;
        func_15145974(&desc.vel, &desc.rot.y, &desc.rot.x);
        desc.rot.z = 0.0f;
        desc.rotVel.x = 0.0f;
        desc.rotVel.y = 0.0f;
        desc.rotVel.z = 0.0f;
        state.lifetime = state.ray.dist * 0.004999999888f;
        state.impactType = impactType;
        desc.unk5C = 0;
        desc.alpha = 255;
        desc.updateFn = 10;
        desc.update2Fn = 0;
        desc.floorHitFn = 0;
        desc.floorType3Fn = 0;
        desc.wallHitFn = 0;
        desc.destroyFn = 0;
        desc.squashFn = 0;
        desc.matrixFn = 2;
        desc.unk6A = 2;
        desc.owner = 0;
        desc.ownerId = 0;
        desc.fadeStart = 1;
        desc.fadeRate = 255;
        obj = func_15132A4C(&desc, 3, 0xFF, sizeof(BulletState), fxOwner, fxList);
        if (obj != NULL) {
            bulletState = &((Bullet *)obj)->state;
            memcpy(bulletState, &state, sizeof(BulletState));
            header.unk0 = 2;
            header.unk1 = -1;
            header.unk2 = 300;
            header.unk4 = 0;
            header2.unk0 = pos.x;
            header2.unk4 = pos.y;
            header2.unk8 = pos.z;
            bulletState->trail = (TrailFx *)func_151602C0(&header, &header2, 40, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, fxOwner, fxList);
            bulletState->follow = func_151B8908(obj);
            bulletState->sound = func_1000FA64(func_150ADA20() % 3 + 750, pos.x, pos.y, pos.z, 30000, 1000, 500,
                                               (SoundRecordCallback)func_151C110C, obj, 0, 4, 0);
            bulletState->unk88 = func_151B8400(obj);
            if (D_80082FA0 < 2 && D_8008FD8C < 4) {
                bulletState->unk8C = func_151B8668(obj, fxOwner, fxList);
            } else {
                extern u8 D_800BE616; // matching: a declaration makes this an IDO scope block (register choice)

                bulletState->unk8C = NULL;
            }
        } else {
            if (state.trail != NULL) {
                func_1516972C((struct102 *)state.trail);
            }
            if (state.follow != NULL) {
                func_1516972C((struct102 *)state.follow);
            }
        }
        if (shooter->unk1D4 != NULL && (shooter->unk74 & 0xF) != 0xF) {
            if (shooter->id != 0x77 && shooter->id != 0x28) {
                const Vec3f a = { 3.0f, 125.0f, 35.0f };
                const Vec3f b = { 6.0f, -50.0f, 35.0f };
                const Vec3f c = { 3.0f, 200.0f, 35.0f };
                f32 scale[2];
                Vec3f outA;
                Vec3f outB;
                Vec3f *in[2];
                Vec3f *out[2];

                scale[0] = func_150ADA68() * 10.0f + 25.0f;
                scale[1] = func_150ADA68() * 1.009000063f + 2.013000011f;
                in[0] = &a;
                in[1] = &b;
                out[0] = &outA;
                out[1] = &outB;
                func_15145EA4(in, out, &((MtxF *)shooter->unk1D4)[9], 2);
                func_15102B38(shooter, 9, &a, &c, scale, func_150ADA20() % 3 + 8,
                              func_150ADA20() % 56 + 200, func_150ADA68() * 204.0f + 601.0f,
                              &outA, 0xFF, 0, -1, fxOwner, fxList);
                func_15103254(func_150ADA20() % 3 + 8, func_150ADA20() % 56 + 200,
                              func_150ADA68() * 204.0f + 601.0f, &outB, 0xFF, fxOwner, fxList);
                func_151C1860(&outB, fxOwner, fxList);
            }
        }
    }
}

/* Sound callback registered by func_151C0698: keeps the bullet's sound at the bullet's position. */
s32 func_151C110C(SoundEmitter *emitter, s32 arg1, void **arg2, s32 arg3, s32 arg4, s32 arg5, s16 *arg6) {
    Bullet *bullet;

    bullet = emitter->bullet;
    if (bullet != NULL && *arg2 != NULL) {
        emitter->x = bullet->pos.x;
        emitter->y = bullet->pos.y;
        emitter->z = bullet->pos.z;
        return 0;
    }

    *arg6 = 0;
    return 0;
}

void func_15055A2C(s32, f32, f32, f32, s32);
void func_151C1654(Vec3f *pos, Vec3f *dir, Actor *owner, u8 impactType, HitSurface *surface);
void func_15081690(Actor *, f32, f32, f32, f32, f32, f32, RayHit *, f32, s32, s32, s32, s32,
                   IgnoreList *, s32);
s32 func_151ABE40(Vec3f *, SurfaceHit *, s32, s32, s32);

/* Per-frame bullet update: moves it, traces for hits and spawns the impact. Returns FALSE when it is done. */
s32 func_151C1180(Bullet *bullet) {
    BulletState *state;
    f32 step;
    Vec3f delta;
    Vec3f hitPos;
    RayHit hit;
    TrailFx *trail;

    state = &bullet->state;
    if (state->lifetime < D_800BE9A4) {
        step = state->speed * state->lifetime;
    } else {
        step = state->speed * D_800BE9A4;
    }
    delta.x = state->ray.dir.x * step;
    delta.y = state->ray.dir.y * step;
    delta.z = state->ray.dir.z * step;
    if (D_800E0930 != NULL) {
        if (((s32 (*)(Vec3f *, Vec3f *, Vec3f *))D_800E0930)(&bullet->pos, &delta, &hitPos) != 0) {
            func_15055A2C(0, hitPos.x, hitPos.y, hitPos.z, 0);
            func_151C1654(&hitPos, &state->ray.dir, state->owner, state->impactType, NULL);
            func_151BFDA0(&hitPos, &state->ray.dir, !(state->flags & 1), bullet->fxOwner, bullet->fxList);
            return FALSE;
        }
    }
    if (step) {
    fence: // matching: an unused label splits IDO's basic block here
        func_15081690(state->owner, bullet->pos.x, bullet->pos.y, bullet->pos.z, delta.x, delta.y, delta.z,
                      &hit, step, 1, 0, 1, 3, &state->ignore, state->mode);

        if (hit.hitType >= 2) {
            func_15055A2C(1, hit.hitPos.x, hit.hitPos.y, hit.hitPos.z, 0);
            func_151C1654(&hit.hitPos, &state->ray.dir, state->owner, state->impactType, NULL);
            func_151C0098(&hit.hitPos, hit.obj, !(state->flags & 1), bullet->fxOwner, bullet->fxList);
            return FALSE;
        }
    }
    bullet->pos.x += bullet->vel.x * D_800BE9A4;
    bullet->pos.y += bullet->vel.y * D_800BE9A4;
    bullet->pos.z += bullet->vel.z * D_800BE9A4;
    if (state->flags & 1) {
        if (state->passTimer > 0.0f) {
            state->passTimer -= D_800BE9A4;
            if (state->passTimer <= 0.0f) {
                func_151ABE40(&state->passPos, &state->pass, 2, bullet->fxOwner, bullet->fxList);
            }
        }
    }
    state->lifetime -= D_800BE9A4;
    if (state->lifetime <= 0.0f) {
        if (state->ray.hitType == 1) {
            func_15055A2C(0, state->ray.hitPos.x, state->ray.hitPos.y, state->ray.hitPos.z, 0);
            func_151C1654(&state->ray.hitPos, &state->ray.dir, state->owner, state->impactType, &state->ray.surface);
            func_151BFE84(&state->ray.surface, &state->ray.hitPos, &bullet->vel, !(state->flags & 1),
                          bullet->fxOwner, bullet->fxList);
        }
        return FALSE;
    }
    trail = state->trail;
    if (trail != NULL) {
        trail->unk14[7] = bullet->pos.x;
        if ((bullet->fxList && bullet->fxList) && bullet->fxList) { // matching: no-op that steers register choice for trail
        }
        trail->unk14[8] = bullet->pos.y;
        trail->unk14[9] = bullet->pos.z;
    }
    if (state->follow != NULL) {
        state->follow->pos = bullet->pos;
    }
    return TRUE;
}

void func_1000FD38(s32 (*)(), Bullet *, s32);

/* Releases a bullet's effects and its sound callback. */
void func_151C1570(Bullet *bullet) {
    BulletState *state;

    state = &bullet->state;
    if (state->trail != NULL) {
        func_1516972C((struct102 *)state->trail);
    }
    if (state->follow != NULL) {
        func_1516972C((struct102 *)state->follow);
    }
    if (state->unk88 != NULL) {
        // matching: the pointer temp gets its own stack slot, which the frame size needs
        s32 *flag = &state->unk88->unk110;

        *flag = 0;
    }
    if (state->unk8C != NULL) {
        func_1516972C(state->unk8C);
    }
    func_1000FD38(func_151C110C, bullet, 0);
}

void func_15132570(Bullet *);
void func_1513259C(Bullet *);

void func_151C15FC(Bullet *bullet) {
    func_151C1570(bullet);
    func_15132570(bullet);
}

void func_151C1628(Bullet *bullet) {
    func_151C1570(bullet);
    func_1513259C(bullet);
}

void func_151D40D4(Vec3f *, Vec3f *, Actor *, s32, HitSurface *, s32, s32, s32);
void func_151D42E8(Vec3f *, Vec3f *, Actor *, HitSurface *, s32);

/* Impact sound/decal for a bullet hit, chosen by impactType. */
void func_151C1654(Vec3f *pos, Vec3f *dir, Actor *owner, u8 impactType, HitSurface *surface) {
    void (*callback)(s32, s32, s32);

    callback = (void (*)(s32, s32, s32))D_800E0934;
    if (callback != NULL) {
        callback(pos->x, pos->y, pos->z);
    }

    switch (impactType) {
    case 1:
        func_151D42E8(pos, dir, owner, surface, 36);
        return;
    case 2:
        func_151D42E8(pos, dir, owner, surface, 37);
        return;
    case 3:
        func_151D40D4(pos, dir, owner, 0, surface, 22, 38, 0);
        return;
    case 0:
    default:
        func_151D40D4(pos, dir, owner, 0, surface, 22, 21, 0);
        return;
    }
}

void func_15143134(Vec3f *, Vec3f *, MtxF *);
extern u8 D_800AA954[];      /* bone index per muzzle */
extern Vec3f D_800AA958[];  /* muzzle offset in bone space */

/* World position of obj's gun muzzle, from its bone matrices. */
void func_151C1798(Actor *obj, Vec3f *pos) {
    s32 index;

    switch (obj->id) {
    case 0x77:
        index = 1;
        break;
    case 0x28:
        index = 2;
        break;
    default:
        index = 0;
        break;
    }
    func_15143134(&D_800AA958[index], pos, &((MtxF *)obj->unk1D4)[D_800AA954[index]]);
}

/* Event handler: event 0x2D hands the bullet's ownership between the two objects in pair. */
void func_151C1814(Bullet *bullet, Actor **pair, u8 event) {
    BulletState *state = &bullet->state;
    if (event == 0x2D) {
        // matching: the indexed form state[0] keeps IDO's compare operands in source order
        if (pair[0] == state[0].owner) {
            state->owner = pair[1];
        } else if (pair[1] == state[0].owner) {
            state->owner = pair[0];
        }
    }
}

void func_151C1860(Vec3f *pos, u8 fxOwner, s32 fxList) {
    Header header;
    Header2 header2;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = func_150ADA20() % 5 + 5;
    header.unk4 = 0;

    header2.unk0 = pos->x;
    header2.unk4 = pos->y;
    header2.unk8 = pos->z;

    func_151602C0(&header, &header2, func_150ADA20() % 11 + 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, fxOwner, fxList);
}

/* Target filter callback: func_151C02E4 with the ignore list packed in a struct. */
s32 func_151C1940(Actor *obj, Actor *self, IgnoreList *ignore) {
    return func_151C02E4(obj, self, ignore->count, ignore->objs);
}

void func_151C1D5C(Actor *, Vec3f *, Vec3f *, RayHit *, f32, s32, s32, s32, s8,
                   IgnoreList *, Vec3f *, f32 *, SurfaceHit *, s32);
s32 func_15145128(Vec3f *, Vec3f *, f32 *, f32 *);

/*
 * Traces a bullet from start along dir. With an aim view (aimPos/aimDir), first traces the view ray and
 * then re-aims from start at whatever it hit. On a miss, ray describes a point 5000 units along the
 * direction. Returns FALSE only when re-aiming fails.
 */
s32 func_151C196C(RayHit *ray, Vec3f *start, Vec3f *dir, Vec3f *aimPos, Vec3f *aimDir,
                  Actor *shooter, IgnoreList *ignore, Vec3f *passPos, f32 *passDist,
                  SurfaceHit *pass, s32 mode) {
    u8 trace;
    u8 hit;
    Vec3f *src;

    trace = TRUE;
    hit = FALSE;

    if (aimPos != NULL && aimDir != NULL) {
        ray->start = *aimPos;
        ray->dir = *aimDir;
        func_151C1D5C(shooter, &ray->start, &ray->dir, ray, 0.0f, 0, 0, 1, 3, ignore, passPos,
                      passDist, pass, mode);

        if (ray->hitType == 0) {
            if (start->x != ray->start.x || start->y != ray->start.y || start->z != ray->start.z) {
                ray->start = *start;
                ray->dir = *aimDir;
            } else {
                trace = FALSE;
            }
        } else {
            if (start->x != ray->start.x || start->y != ray->start.y || start->z != ray->start.z) {
                ray->start = *start;
                ray->dir.x = ray->hitPos.x - start->x;
                ray->dir.y = ray->hitPos.y - start->y;
                ray->dir.z = ray->hitPos.z - start->z;
                if (func_15145128(&ray->dir, &ray->dir, &ray->dist, NULL) == 0) {
                    return FALSE;
                }
            }
        }
    } else {
        ray->start = *start;
        ray->dir = *dir;
    }

    if (trace) {
        func_151C1D5C(shooter, &ray->start, &ray->dir, ray, 0.0f, 0, 1, 1, 3, ignore, passPos,
                      passDist, pass, mode);
        if (ray->hitType != 0) {
            hit = TRUE;
        }
    }

    if (!hit) {
        ray->obj = NULL;
        ray->dist = 5000.0f;
        ray->start = *start;
        if (aimDir != NULL) {
            src = aimDir;
        } else {
            src = dir;
        }
        ray->dir = *src;
        ray->hitPos.x = ray->start.x + ray->dir.x * 5000.0f;
        ray->hitPos.y = ray->start.y + ray->dir.y * 5000.0f;
        ray->hitPos.z = ray->start.z + ray->dir.z * 5000.0f;
        ray->unk14 = ray->hitPos;
        ray->unk20 = ray->hitPos;
        ray->unk56 = 0;
        ray->unk58 = 0;
        ray->hitType = 0;
        ray->unk5A = 0;
        ray->part = NULL;
        ray->unk60 = 0;
    }
    return TRUE;
}

/*
 * Ray cast that continues through pass-through surfaces; the first one it passes is reported in
 * passPos/passDist/pass. ray->dist is the total distance travelled.
 */
void func_151C1D5C(Actor *shooter, Vec3f *start, Vec3f *dir, RayHit *ray, f32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s8 arg8, IgnoreList *ignore, Vec3f *passPos,
                   f32 *passDist, SurfaceHit *pass, s32 mode) {
    RayHitPart *part;
    Vec3f pos;
    f32 dist;
    s32 more;

    dist = 0.0f;
    *passDist = 0.0f;
    pos = *start;

    do {
        func_15081690(shooter, pos.x, pos.y, pos.z, dir->x, dir->y, dir->z, ray, arg4, arg5,
                      arg6, arg7, arg8, ignore, mode);
        dist += ray->dist;
        if (ray->part != NULL) {
            RayHitPart *p = ray->part;

            if ((p->unk4F & 0x60) == 0x40) {
                pos.x = ray->hitPos.x + ray->dir.x * 0.1f;
                part = p;
                pos.y = ray->hitPos.y + ray->dir.y * 0.1f;
                pos.z = ray->hitPos.z + ray->dir.z * 0.1f;
                *passPos = ray->hitPos;
                *passDist = dist;
                dist += 0.1f;
                pass->height = ray->hitPos.y;
                pass->surface = ray->surface;
                pass->unk18 = ray->unk60;
                pass->unk1C = 7;
                pass->unk1D = 3;
                pass->part = part;
                more = TRUE;
            } else {
                more = FALSE;
            }
        } else {
            more = FALSE;
        }
    } while (more);

    ray->start = *start;
    ray->dist = dist;
}

typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ u8 unk7;
} Event151D8868;

void func_151D8868(Event151D8868 *, s32, s32, s32);

void func_151C1FB8(Actor *obj) {
    Event151D8868 event;

    if (obj->camera != NULL) {
        event.unk0 = 1;
        event.unk2 = (func_150ADA20() & 7) + 13;
        event.unk5 = 1 << obj->camera->playerIndex;
        event.unk4 = func_150ADA20() % 3 + 6;
        event.unk6 = -1;
        func_151D8868(&event, 0, 0xFF, 1);
    }
}
