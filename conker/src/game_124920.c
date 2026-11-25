#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

extern f32 D_800A1BD8;
extern f32 sinf(f32);
extern s32 func_15145128(Vec3f *, Vec3f *, f32 *, f32 *);
extern s32 func_15146078(Vec3f *, Vec3f *, Vec3f *);

/* Per-object state of the projectile spawned by func_150F7470 (updated by func_150F78B4). */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 unk4;
    /* 0x08 */ u8 pad8[0x30];
    /* 0x38 */ Vec3f dir;
    /* 0x44 */ u8 pad44[0x20];
    /* 0x64 */ s32 unk64;
    /* 0x68 */ f32 timeLeft;
    /* 0x6C */ f32 speed;
    /* 0x70 */ u8 flags; /* 1: impact effect, 2: ray-test the path, 4: see func_150F78B4 */
    /* 0x71 */ u8 pad71[3];
    /* 0x74 */ Vec3f unk74;
    /* 0x80 */ Vec3f unk80;
    /* 0x8C */ f32 trailDist;
    /* 0x90 */ f32 trailAge;
    /* 0x94 */ f32 trailAgeStep;
    /* 0x98 */ Vec3f trailPos;
    /* 0xA4 */ Vec3f trailStep;
    /* 0xB0 */ s8 unkB0;
    /* 0xB1 */ u8 padB1[3];
} ProjectileState;

/* Per-object state of the effect spawned by func_150F802C (updated by func_150F81BC). */
typedef struct {
    /* 0x00 */ f32 *target; /* position (3 floats), then 3 more floats copied to unk20 */
    /* 0x04 */ Vec3f trailPos;
    /* 0x10 */ f32 trailAge;
} FollowState;

/* An object spawned by func_1513264C. */
typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0x13];
    /* 0x020 */ f32 unk20;
    /* 0x024 */ f32 unk24;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ u8 pad2C[0xC];
    /* 0x038 */ Vec3f pos;
    /* 0x044 */ Vec3f vel;
    /* 0x050 */ u8 pad50[0x2C];
    /* 0x07C */ Actor *owner;
    /* 0x080 */ u8 pad80[8];
    /* 0x088 */ s32 sound;
    /* 0x08C */ u8 pad8C[0xE4];
    /* 0x170 */ union {
        ProjectileState projectile;
        FollowState follow;
    } state;
} EffectObj;

extern s32 func_151C4B0C(s32, ProjectileState *, Vec3f *, Vec3f *, f32 *, f32, u8, s32, Vec3f *, u8 *, s32,
                         f32, Actor *, s32, u8, u8, s32, u8);
extern void func_15145974(Vec3f *, f32 *, f32 *);
EffectObj *func_1513264C(void *spawn, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);

/* Spawns a projectile at `pos`, aimed along `aim` (or at `target` when arg2 is set). */
void func_150F7470(Vec3f *pos, Vec3f *aim, s32 arg2, Vec3f *target, u8 targetIsDir, s32 arg5, f32 speed,
                   f32 arg7, f32 arg8, Actor *owner, u8 arg10, u8 arg11, u8 arg12, u8 arg13, s32 arg14,
                   s8 arg15, s32 sound, u8 arg17, u8 arg18, s32 arg19) {
    ProjectileState state;
    ParticleSpawn spawn;
    EffectObj *obj;
    Vec3f aimDir;
    u8 hasAimDir;
    Vec3f targetDir;
    u8 hasTargetDir;

    hasAimDir = FALSE;
    hasTargetDir = targetIsDir;
    if (targetIsDir) {
        targetDir = *target;
    }
    state.flags = (arg11 ? 1 : 0) | (arg12 ? 2 : 0) | (arg13 ? 4 : 0);
    state.unkB0 = arg15;
    if (arg2 != 0 && target != NULL) {
        if (!hasTargetDir) {
            if (!func_15145128(target, &targetDir, NULL, NULL)) {
                return;
            }
            hasTargetDir = TRUE;
        }
    } else if (aim != NULL) {
        if (!func_15145128(aim, &aimDir, NULL, NULL)) {
            return;
        }
        hasAimDir = TRUE;
    }
    if (!func_151C4B0C(arg5, &state, pos, hasAimDir ? &aimDir : NULL, &state.timeLeft, arg7, arg10, arg2,
                       hasTargetDir ? &targetDir : NULL, &state.flags, 1, arg8, owner, arg15, arg12, arg13, 0,
                       arg17)) {
        return;
    }
    if (!func_15146078(&state.dir, &state.unk74, &state.unk80)) {
        return;
    }
    state.unk64 = arg14;
    state.trailAge = 0.0f;
    state.trailDist = 0.0f;
    state.trailAgeStep = 70.0f * arg7;
    state.trailPos = *pos;
    state.speed = speed;
    state.trailStep.x = state.dir.x * 70.0f;
    state.trailStep.y = state.dir.y * 70.0f;
    state.trailStep.z = state.dir.z * 70.0f;
    state.flags |= (state.unk4 > 150.0f) ? 1 : 0;
    spawn.radius = 1.0f;
    spawn.elasticity = 1.0f;
    spawn.sizeY = 0.02000000142f;
    spawn.sizeXZ = 0.02000000142f;
    func_15145974(&state.dir, &spawn.rot.y, &spawn.rot.x);
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.rot.z = 0.0f;
    spawn.pos = *pos;
    spawn.vel.x = state.dir.x * speed;
    spawn.vel.y = state.dir.y * speed;
    spawn.vel.z = state.dir.z * speed;
    spawn.rotVel = D_800A5480;
    spawn.gravity = 0.0f;
    spawn.flags = 0x120;
    spawn.lifetime = 300;
    spawn.model = 0x2F;
    spawn.variant = 0;
    spawn.unk5C = 0;
    spawn.alpha = 0xFF;
    spawn.updateFn = 0x13;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 0;
    spawn.unk6A = 0;
    spawn.owner = owner;
    spawn.ownerId = owner->unique_id;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    if (sound != 0) {
        spawn.sound = func_10010F88(sound, 18000, -500, 0, -1, spawn.pos.x, spawn.pos.y, spawn.pos.z, 1000,
                                    2000);
    } else {
        spawn.sound = 0;
    }
    obj = func_1513264C(&spawn, 0, 0, 0, 0xB4, arg18, arg19);
    if (obj != NULL) {
        memcpy(&obj->state.projectile, &state, sizeof(state));
    }
}

/* Per-object state of a puff spawned by func_1513D2F0 (updated by func_150F7E20). */
typedef struct {
    /* 0x00 */ Vec3f unk0;
    /* 0x0C */ Vec3f unkC;
    /* 0x18 */ f32 timeLeft;
    /* 0x1C */ f32 phase;
    /* 0x20 */ f32 phaseSpeed;
    /* 0x24 */ f32 sizeBase;
    /* 0x28 */ f32 sizeAmplitude;
} PuffState;

/* An object spawned by func_1513D2F0. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ PuffState state;
} PuffObj;

/* Result of the func_15081690 ray cast. */
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ Vec3f unk14;
    /* 0x20 */ Vec3f unk20;
    /* 0x2C */ Vec3f unk2C;
    /* 0x38 */ Vec3f unk38;
    /* 0x44 */ u8 unk44[0x12];
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 hit;
    /* 0x5A */ u8 unk5A;
    /* 0x5B */ u8 pad5B;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ u8 pad64[0xC];
} RayHit;

void func_15081690(Actor *obj, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, RayHit *hit, f32 maxDist, s32 arg9,
                   s32 arg10, s32 arg11, s32 arg12, s32 arg13, s32 arg14);
void func_151D4DAC(s32, Actor *, Vec3f *, Vec3f *, RayHit *, s32, Vec3f *, u8, s32);
void func_150F7F8C(Vec3f *pos, u8 arg1, s32 arg2);

/*
 * Per-frame update of a projectile: ray-tests and moves it, runs out its lifetime, and leaves a
 * trail of puffs every 70 units travelled. Returns 0 once it hit something or expired.
 */
s8 func_150F78B4(EffectObj *obj) {
    ProjectileState *state;
    Actor *owner;
    s8 alive;
    f32 dt;
    f32 dist;
    RayHit hit;
    f32 puffs;
    PuffObj *puff;

    alive = TRUE;
    state = &obj->state.projectile;
    owner = obj->owner;
    dt = (state->timeLeft < D_800BE9A4) ? state->timeLeft : D_800BE9A4;
    if (obj->sound != 0) {
        func_1000F9D4(obj->sound, obj->pos.x, obj->pos.y, obj->pos.z);
    }
    dist = state->speed * dt;
    if (state->flags & 2) {
        func_15081690(owner, obj->pos.x, obj->pos.y, obj->pos.z, state->dir.x, state->dir.y,
                      state->dir.z, &hit, dist, 1, 0, !(state->flags & 4), state->unkB0, 0, 0);
        if (hit.hit >= 2) {
            func_151D4DAC(hit.unk00, owner, &hit.pos, &hit.unk20, &hit, 0x1A, &state->dir, obj->unkC, obj->unk1);
            if (state->flags & 1) {
                func_150F7F8C(&hit.pos, obj->unkC, obj->unk1);
            }
            alive = FALSE;
        }
    }
    obj->pos.x += obj->vel.x * dt;
    obj->pos.y += obj->vel.y * dt;
    obj->pos.z += obj->vel.z * dt;
    if (alive == TRUE) {
        state->timeLeft -= D_800BE9A4;
        if (state->timeLeft <= 0.0f) {
            alive = FALSE;
            if (state->flags & 1) {
                func_150F7F8C(&obj->pos, obj->unkC, obj->unk1);
            }
        }
    }
    {
        ParticleDef54 spawn;

        state->trailDist += dist;
        state->trailAge += dt;
        puffs = state->trailDist * (1.0f / 70.0f);
        if (puffs > 1.0f) {
            PuffState puffState;

            puffState.unk0 = state->unk74;
            puffState.unkC = state->unk80;
            puffState.sizeBase = 8.318000793f;
            puffState.sizeAmplitude = 8.318000793f;
            spawn.texture = 0x7A;
            spawn.type = 0;
            spawn.renderModes = 0x4404;
            spawn.lifetime = 10;
            spawn.frame = 0;
            spawn.frameSpeed = 0;
            spawn.rgba[0] = 0xFF;
            spawn.rgba[1] = 0xFF;
            spawn.rgba[2] = 0xFF;
            spawn.rgba[3] = 0xFF;
            spawn.width = 0.0f;
            spawn.height = 0.0f;
            puffState.phaseSpeed = 0.4670000076f;
            spawn.rot = D_800A5480;
            spawn.scale.x = 1.0f;
            spawn.scale.y = 1.0f;
            spawn.scale.z = 1.0f;
            spawn.flags = 0xCC0008;
            spawn.unk45 = 0xFF;
            spawn.unk46 = 0;
            spawn.unk47 = 6;
            spawn.unk48 = 0;
            spawn.viewportMask = 0xFF;
            spawn.unk50 = 0;
            do {
                extern s32 D_800A4AA0; // matching: a declaration here opens the loop scope IDO's scheduler needs

                puffState.phase = 4.712388992f;
                puffState.timeLeft = 27.0f - state->trailAge;
                spawn.pos = state->trailPos;
                spawn.unk44 = puffState.timeLeft * 6.481481552f;
                puff = func_1513D2F0(&spawn, (s32)&D_800A4AA0, 0x24, 0, 0, 0x21, 0, 0, 0, 0x2C, obj->unkC, obj->unk1); // matching: the int cast orders the arg setup
                if (puff != NULL) {
                    memcpy(&puff->state, &puffState, sizeof(puffState));
                }
                state->trailDist -= 70.0f;
                state->trailAge -= state->trailAgeStep;
                state->trailPos.x += state->trailStep.x;
                state->trailPos.y += state->trailStep.y;
                state->trailPos.z += state->trailStep.z;
                puffs -= 1.0f;
            } while (puffs > 1.0f);
        }
    }
    return alive;
}

/* Per-frame update of a puff: fades it out over its lifetime and pulses its size. Returns 0 once expired. */
s32 func_150F7E20(PuffObj *obj) {
    f32 size;
    PuffState *state;

    state = &obj->state;
    obj->head.def.unk44 = state->timeLeft * D_800A1BD8;
    size = (sinf(state->phase) * state->sizeAmplitude) + state->sizeBase;
    obj->head.def.height = size;
    obj->head.def.width = size;
    state->phase = func_15144B68(state->phase + (state->phaseSpeed * D_800BE9A4));
    state->timeLeft -= D_800BE9A4;
    if (state->timeLeft < 0.0f) {
        return 0;
    }
    return 1;
}

/* Forwards `obj`'s unk110 and unk11C to func_15140410. */
void func_150F7F58(struct210 *obj, s16 arg1) {
    func_15140410(obj, &obj->unk110, &obj->unk11C, arg1);
}

void func_151541B8(Vec3f *pos, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u8 arg5, s32 arg6);
void func_151D3F14(Vec3f *pos, u8, s32);
ActorCtl *func_150FF288(Actor *);
s32 func_150FF6E0(Vec3f *, Vec3f *, Vec3f *, Vec3f *, Vec3f *, Actor *, ActorCtl *);
void func_150FF474(Vec3f *position, Vec3f *points, u8 arg2, s32 arg3);
void func_151D4408(Vec3f *, Vec3f *, void *, Actor *, f32, s32, s32);

/* Impact effect at `pos`. */
void func_150F7F8C(Vec3f *pos, u8 arg1, s32 arg2) {
    f32 random1;
    f32 random0;

    random0 = func_150ADA68();
    random1 = func_150ADA68();
    func_151541B8(pos, random0 * 3.0f + 9.0f, 0.5119050145f, random1 * 70.0f + 70.0f, 0.0f, arg1, arg2);
    func_151D3F14(pos, arg1, arg2);
}

/* Spawns an effect that follows `target` (a position followed by three more floats), trailing puffs behind it. */
void func_150F802C(f32 *target, s16 arg1, u8 arg2, s32 arg3) {
    struct {
        ParticleSpawn spawn;
        FollowState follow;
    } packet;
    EffectObj *obj;

    packet.follow.target = target;
    packet.follow.trailPos.x = target[0];
    packet.follow.trailPos.y = target[1];
    packet.follow.trailPos.z = target[2];
    packet.follow.trailAge = 0.0f;
    packet.spawn.radius = 1.0f;
    packet.spawn.elasticity = 1.0f;
    packet.spawn.sizeXZ = 0.02000000142f;
    packet.spawn.sizeY = 0.02000000142f;
    packet.spawn.rot.x = target[3];
    packet.spawn.rot.y = target[4];
    packet.spawn.rot.z = target[5];
    packet.spawn.scale.x = 1.0f;
    packet.spawn.scale.y = 1.0f;
    packet.spawn.scale.z = 1.0f;
    packet.spawn.pos.x = target[0];
    packet.spawn.pos.y = target[1];
    packet.spawn.pos.z = target[2];
    packet.spawn.vel.x = 0.0f;
    packet.spawn.vel.y = 0.0f;
    packet.spawn.vel.z = 0.0f;
    packet.spawn.rotVel = D_800A5480;
    packet.spawn.gravity = 0.0f;
    packet.spawn.flags = 0x980;
    packet.spawn.lifetime = arg1;
    packet.spawn.model = 0x2F;
    packet.spawn.variant = 0;
    packet.spawn.unk5C = 0;
    packet.spawn.alpha = 0xFF;
    packet.spawn.updateFn = 0x14;
    packet.spawn.update2Fn = 0;
    packet.spawn.floorHitFn = 0;
    packet.spawn.floorType3Fn = 0;
    packet.spawn.wallHitFn = 0;
    packet.spawn.destroyFn = 0;
    packet.spawn.squashFn = 0;
    packet.spawn.matrixFn = 0;
    packet.spawn.unk6A = 0;
    packet.spawn.owner = NULL;
    packet.spawn.ownerId = 0;
    packet.spawn.fadeStart = 1;
    packet.spawn.fadeRate = 0xFF;
    packet.spawn.sound = 0;
    obj = func_1513264C(&packet, 3, 0xFF, 0, 0xB4, arg2, arg3);
    if (obj != NULL) {
        memcpy(&obj->state.follow, &packet.follow, sizeof(packet.follow));
    }
}

/* Per-frame update of a func_150F802C effect: snaps to its target and drops a puff every 20 units it moved. */
s32 func_150F81BC(EffectObj *obj) {
    FollowState *follow;

    follow = &obj->state.follow;
    obj->pos.x = follow->target[0];
    obj->pos.y = follow->target[1];
    obj->pos.z = follow->target[2];
    obj->unk20 = follow->target[3];
    obj->unk24 = follow->target[4];
    obj->unk28 = follow->target[5];
    {
        Vec3f delta;
        f32 dist;
        f32 unused;
        Vec3f dir;
        f32 puffs;

        delta.x = obj->pos.x - follow->trailPos.x;
        delta.y = obj->pos.y - follow->trailPos.y;
        delta.z = obj->pos.z - follow->trailPos.z;
        follow->trailAge += D_800BE9A4;
        if (func_15145128(&delta, &dir, &dist, &unused) != 0) {
            Vec3f step;
            PuffObj *puff;
            ParticleDef54 spawn;
            PuffState puffState;

            step.x = dir.x * 20.0f;
            step.y = dir.y * 20.0f;
            step.z = dir.z * 20.0f;
            puffs = dist * 0.05000000075f;
            if (puffs > 1.0f) {
                f32 ageStep;

                ageStep = follow->trailAge / puffs;
                func_15146078(&delta, &puffState.unk0, &puffState.unkC);
                puffState.phaseSpeed = 0.4630000293f;
                puffState.sizeBase = 8.241000175f;
                puffState.sizeAmplitude = 3.888999939f;
                spawn.texture = 0x7A;
                spawn.type = 0;
                spawn.renderModes = 0x4404;
                spawn.lifetime = 10;
                spawn.frame = 0;
                spawn.frameSpeed = 0;
                spawn.rgba[0] = 0xFF;
                spawn.rgba[1] = 0xFF;
                spawn.rgba[2] = 0xFF;
                spawn.rgba[3] = 0xFF;
                spawn.height = 0.0f;
                spawn.width = 0.0f;
                spawn.rot = D_800A5480;
                spawn.scale.x = 1.0f;
                spawn.scale.y = 1.0f;
                spawn.scale.z = 1.0f;
                spawn.flags = 0xCC0008;
                spawn.unk45 = 0xFF;
                spawn.unk46 = 0;
                spawn.unk47 = 6;
                spawn.unk48 = 0;
                spawn.viewportMask = 0xFF;
                spawn.unk50 = 0;
                do {
                    extern s32 D_800A4AA0; // matching: a declaration here opens the loop scope IDO's scheduler needs

                    puffState.phase = 4.712388992f;
                    puffState.timeLeft = 27.0f - follow->trailAge;
                    spawn.pos = follow->trailPos;
                    spawn.unk44 = puffState.timeLeft * 6.481481552f;
                    puff = func_1513D2F0(&spawn, (s32)&D_800A4AA0, 0x24, 0, 0, 0x21, 0, 0, 0, 0x2C, obj->unkC,
                                         obj->unk1); // matching: the int cast orders the arg setup
                    if (puff != NULL) {
                        memcpy(&puff->state, &puffState, sizeof(puffState));
                    }
                    follow->trailPos.x += step.x;
                    follow->trailPos.y += step.y;
                    follow->trailPos.z += step.z;
                    puffs -= 1.0f;
                    follow->trailAge -= ageStep;
                } while (puffs > 1.0f);
            }
        }
    }
    return 1;
}

extern struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, u8, u8, s32, u8, s32);
extern void func_15152B38(TrailBurst *, u8, s32);

/* Effects 50 units above `obj`: a func_151602C0 burst and a func_15152B38 emitter. */
void func_150F85A0(s32 arg0, Actor *obj, s32 arg2) {
    Vec3f pos;

    pos.x = obj->x_position;
    pos.y = obj->y_position + 50.0f;
    pos.z = obj->z_position;
    {
        Header header;
        Header2 header2;

        header.unk0 = 3;
        header.unk1 = -1;
        header.unk2 = (func_150ADA20() % 9U) + 5;
        header.unk4 = 0;
        header2.unk0 = pos.x;
        header2.unk4 = pos.y;
        header2.unk8 = pos.z;
        func_151602C0(&header, &header2, (func_150ADA20() % 31U) + 90, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 1);
    }
    {
        TrailBurst spawn;

        spawn.countBase = 8;
        spawn.countRange = 12;
        spawn.pos = pos;
        spawn.sizeBase = 3.07f;
        spawn.sizeRange = 6.2f;
        spawn.gravityBase = -0.9690000415f;
        spawn.gravityRange = 0.7180000544f;
        spawn.speedBase = 15.0f;
        spawn.speedRange = 30.0f;
        spawn.angles.yawBase = 0;
        spawn.angles.yawRange = 0xFF;
        spawn.angles.pitchBase = -64;
        spawn.angles.pitchRange = 80;
        spawn.capacityBase = 3;
        spawn.capacityRange = 1;
        spawn.lifeBase = 20;
        spawn.lifeRange = 15;
        spawn.headerFlags = 1;
        spawn.flags = 4;
        spawn.primMode = 2;
        spawn.envMode = 3;
        spawn.colorBase.red = 0xFF;
        spawn.colorBase.green = 0xFF;
        spawn.colorBase.blue = 0xFF;
        spawn.colorBase.alpha = 0xFF;
        spawn.colorRange.red = 0;
        spawn.colorRange.green = 0;
        spawn.colorRange.blue = 0;
        spawn.colorRange.alpha = 0;
        spawn.color2Base.red = 0xFF;
        spawn.color2Base.green = 0xFF;
        spawn.color2Base.blue = 0xFF;
        spawn.color2Base.alpha = 0xFF;
        spawn.color2Range.red = 0;
        spawn.color2Range.green = 0;
        spawn.color2Range.blue = 0;
        spawn.color2Range.alpha = 0;
        spawn.colorLevelBase = 0xFF;
        spawn.colorLevelRange = 0;
        spawn.renderMode = 3;
        spawn.unk21 = 0x24;
        spawn.setGeometryMode = 0x200005;
        spawn.clearGeometryMode = 0x60600;
        spawn.fadeStart = 8;
        spawn.fadeRate = 0x1F;
        spawn.shrinkStart = 1;
        spawn.shrinkPaused = 0;
        spawn.shrinkFactor = 1.0f;
        spawn.unk38 = -1;
        spawn.unk39 = 0;
        spawn.unk72 = -1;
        spawn.unk73 = -1;
        func_15152B38(&spawn, 0xFF, 1);
    }
}

/* A message passed to func_151494E0 and received by func_150F887C. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 unk4;
} StageMsg;

/* Broadcasts message 0x3F, which runs func_150F892C on the StageObj whose stage.unk0 is `arg1`. */
void func_150F884C(s32 arg0, s32 arg1) {
    StageMsg msg;

    msg.unk0 = arg1;
    func_151494E0((s32)&msg, 0x3F);
}

/* Something that steps through eight parts (one shown at a time) while emitting effects around (unk40, unk44). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5; /* bit 0 mirrors the effect angles */
    /* 0x06 */ s16 stageTimer;
    /* 0x08 */ u8 stage;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ WorldObject *parts[8]; /* the inactive ones are hidden */
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s16 eventTimer;
    /* 0x3E */ u8 pad3E[2];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
} StageState;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ StageState stage;
} StageObj;

/* Event passed to func_151D8868 (a camera event, as in game_12C1E0.c). */
typedef struct {
    u8 unk0;
    u8 pad1;
    s16 unk2;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    u8 pad7;
} CameraEvent;

void func_150F892C(StageObj *obj);

/* Message handler: 0x42 jumps to the last stage, 0x3F runs func_150F892C. */
void func_150F887C(StageObj *obj, StageMsg *msg, u8 type) {
    StageState *state = &obj->stage;

    if (type == 0x42) {
        if (state->unk4 == msg->unk4) {
            u8 i;

            for (i = 0; i < 7; i++) {
                state->parts[i]->inactive = 1;
            }
            state->parts[7]->inactive = 0;
            state->stage = 7;
        }
    } else if (type == 0x3F) {
        if (obj->stage.unk0 == msg->unk0) {
            func_150F892C(obj);
        }
    }
}

extern void func_15164F0C(s32 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_15143874(s16 angle, f32 radius, f32 *x, f32 *z);
extern void func_151C329C(Vec3f *pos, s32, s32);
extern void func_15143794(s16, s16, f32, Vec3f *out);
extern void *func_15130280(ParticleDesc *spawn, u8, s32, s32, u8, s32);
void func_151DB5D0(s32, Vec3f *pos, Vec3f *vel, f32, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32);

/*
 * While any object 0x98 in state 0x1EB is active: steps through the parts on a timer (with a sound and a
 * camera event per step), and emits effects on an arc of radius unk48 around (unk40, unk44).
 */
void func_150F892C(StageObj *obj) {
    s32 count;
    s32 i;
    StageState *state;

    count = 0;
    {
        Actor *other;

        other = D_800CC2D0;
        for (i = 0; i < 25; i++, other++) {
            if (other->id == 0x98 && other->unk84.uh == 0x1EB && other->unk222 == 0) {
                count++;
            }
        }
    }
    state = &obj->stage;
    if (count == 0) {
        return;
    }
    if (state->stage < 7) {
        state->stageTimer -= D_800BE9E4 * count;
        if (state->stageTimer < 0) {
            CameraEvent event;
            func_10010F88(func_150ADA20() % 3U + 0x2B3, 24000, 0, 0, -1, state->parts[state->stage]->x,
                          state->parts[state->stage]->y, state->parts[state->stage]->z, 500, 1000);
            state->parts[state->stage]->inactive = 1;
            state->stage++;
            state->parts[state->stage]->inactive = 0;
            func_15164F0C(7, 0, 0, obj->unkC, obj->unk1);
            event.unk0 = 1;
            event.unk2 = 25;
            event.unk5 = 1;
            event.unk4 = 8;
            event.unk6 = -1;
            func_151D8868(&event, 0, obj->unkC, obj->unk1);
            state->stageTimer = func_150ADA20() % 31U + 70;
        }
    }
    state->eventTimer -= D_800BE9E4 * count;
    if (state->eventTimer < 0) {
        CameraEvent event;
        event.unk0 = 1;
        event.unk2 = func_150ADA20() % 21U + 10;
        event.unk5 = 1;
        event.unk4 = (func_150ADA20() & 3) + 1;
        event.unk6 = -1;
        func_151D8868(&event, 0, obj->unkC, obj->unk1);
        func_10010F88((func_150ADA20() & 3) + 0x6D6, 24000, 0, 0, -1, state->parts[state->stage]->x,
                      state->parts[state->stage]->y, state->parts[state->stage]->z, 500, 1000);
        state->eventTimer = func_150ADA20() % 151U + 150;
    }

    state->unk38 += (0.02000000142f + func_150ADA68() * 0.025f) * (D_800BE9A4 * count);
    if (state->unk38 > 1.0f) {
        s16 side;
        Vec3f pos;
        side = (state->unk5 & 1) ? -1 : 1;
        do {
            func_15143874((func_150ADA20() % 126U - 220) * side, state->unk48, &pos.x, &pos.z);
            pos.y = func_150ADA68() * 150.0f + 30.0f;
            pos.x += state->unk40;
            pos.z += state->unk44;
            func_151C329C(&pos, obj->unkC, obj->unk1);
            state->unk38 -= 1.0f;
        } while (state->unk38 > 1.0f);
    }

    state->unk34 += (0.02000000142f + func_150ADA68() * 0.06000000238f) * (D_800BE9A4 * count);
    if (state->unk34 > 1.0f) {
        s16 pieces;
        s16 side;
        ParticleSpawn spawn;
        side = (state->unk5 & 1) ? -1 : 1;
        spawn.radius = 1.0f;
        spawn.elasticity = 1.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.flags = 0x39E8;
        spawn.model = 0x86;
        spawn.variant = 0;
        spawn.unk5C = 0;
        spawn.alpha = 0xFF;
        spawn.updateFn = 8;
        spawn.update2Fn = 0;
        spawn.floorHitFn = 0;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 0;
        spawn.unk6A = 2;
        spawn.owner = NULL;
        spawn.ownerId = 0;
        spawn.fadeStart = 12;
        spawn.fadeRate = 21;
        spawn.sound = 0;
        spawn.rotVel.y = 0.0f;
        do {
            s16 angle;

            pieces = func_150ADA20() % 7U + 5;
            angle = (func_150ADA20() % 126U - 220) * side;
            func_15143874(angle, state->unk48, &spawn.pos.x, &spawn.pos.z);
            spawn.pos.y = func_150ADA68() * 150.0f + 30.0f;
            spawn.pos.x += state->unk40;
            spawn.pos.z += state->unk44;
            i = angle - 7; // matching: the base angle reuses the counter i (a new local changes registers)
            for (; pieces > 0; pieces--) {
                spawn.lifetime = (func_150ADA20() & 0xF) + 35;
                spawn.sizeXZ = spawn.sizeY = func_150ADA68() * 0.012f + 0.005f;
                spawn.rot.x = func_150ADA68() * 360.0f;
                spawn.rot.y = func_150ADA68() * 360.0f;
                spawn.rot.z = func_150ADA68() * 360.0f;
                func_15143794((func_150ADA20() & 0xF) + i, func_150ADA20() % 37U - 63,
                              func_150ADA68() * 8.0f + 10.0f, &spawn.vel);
                spawn.rotVel.x = func_150ADA68() * 32.41f + -16.205f;
                spawn.rotVel.z = func_150ADA68() * 32.41f + -16.205f;
                spawn.gravity = func_150ADA68() * 0.337f + -1.177f;
                func_1513264C(&spawn, 3, 1, 0, 0, 0xFF, 1);
            }
            state->unk34 -= 1.0f;
        } while (state->unk34 > 1.0f);
    }

    state->unk30 += (0.05500000343f + func_150ADA68() * 0.05f) * (D_800BE9A4 * count);
    if (state->unk30 > 1.0f) {
        s16 side;
        s32 flares;
        ParticleDesc flare;
        side = (state->unk5 & 1) ? -1 : 1;
        flare.texture = 0x86;
        flare.renderMode = 0x4404;
        flare.unk0 = 0x200005;
        flare.unk4 = 0x9F0600;
        flare.frame = 0;
        flare.frameRate = 0;
        flare.r = 0xFF;
        flare.g = 0xFF;
        flare.b = 0xFF;
        flare.a = 0xFF;
        flare.r2 = 0xFF;
        flare.g2 = 0xFF;
        flare.b2 = 0xFF;
        flare.offset = D_800A5480;
        flare.fadeStart = 8;
        flare.fadeRate = 0x1F;
        flare.scaleStart = 1;
        flare.flags = 0x1C207;
        flare.primColorMode = 6;
        flare.envColorMode = 5;
        flare.updateFunc = -1;
        flare.deathFunc = -1;
        flare.drawFunc = -1;
        flare.handlerSet = 0;
        flare.viewMask = 0;
        flare.views = 0xFF;
        flare.minSize = 0;
        flare.scaleRate = 1.0f;
        flare.maxSize = 0.0f;
        do {
            s16 angle;

            flares = func_150ADA20() % 13U + 7;
            angle = (func_150ADA20() % 126U - 220) * side;
            func_15143874(angle, state->unk48, &flare.pos.x, &flare.pos.z);
            flare.pos.y = func_150ADA68() * 150.0f + 30.0f;
            flare.pos.x += state->unk40;
            flare.pos.z += state->unk44;
            i = angle - 4; // matching: the base angle reuses the counter i (a new local changes registers)
            for (; flares > 0; flares--) {
                s32 flip;

                flare.timer = func_150ADA20() % 21U + 30;
                flare.alpha = func_150ADA20() % 156U + 100;
                flare.width = flare.height = func_150ADA68() * 110.0f + 70.0f;
                flare.flags &= ~0xC0;
                flip = (func_150ADA20() & 1) ? 0x80 : 0;
                flare.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | flip;
                func_15143794(func_150ADA20() % 9U + i, (func_150ADA20() & 7) - 16,
                              func_150ADA68() * 10.0f + 6.0f, &flare.vel);
                flare.gravity = func_150ADA68() * 0.268f + -0.3870000243f;
                func_15130280(&flare, 1, 0, 0, 0xFF, 1);
            }
            state->unk30 -= 1.0f;
        } while (state->unk30 > 1.0f);
    }

    state->unk2C += (0.3360000253f + func_150ADA68() * 0.126f) * (D_800BE9A4 * count);
    if (state->unk2C > 1.0f) {
        s16 side;
        Vec3f pos;
        Vec3f vel;
        side = (state->unk5 & 1) ? -1 : 1;
        do {
            s16 angle;

            angle = (func_150ADA20() % 126U - 220) * side;
            func_15143874(angle, state->unk48, &pos.x, &pos.z);
            pos.y = func_150ADA68() * 150.0f + 30.0f;
            pos.x += state->unk40;
            pos.z += state->unk44;
            func_15143794(func_150ADA20() % 9U + angle - 4, (func_150ADA20() & 7) - 16,
                          func_150ADA68() * 6.0f + 4.0f, &vel);
            func_151DB5D0(8, &pos, &vel, func_150ADA68() * 200.0f + 200.0f, 1.025439024f, 0.938816011f,
                          func_150ADA68() * 0.2070000172f + -0.1690000147f, func_150ADA20() % 21U + 35,
                          func_150ADA20() % 121U + 100, 50, 5, 0, 0xFF, 1);
            state->unk2C -= 1.0f;
        } while (state->unk2C > 1.0f);
    }
}

extern u8 D_800A1C40[][2];

/* Broadcasts message 0x42 (jump to the last stage) for both StageState ids of pair `index`. */
void func_150F9720(u8 index) {
    StageMsg msg;

    msg.unk0 = 0;
    msg.unk4 = D_800A1C40[index][0];
    func_151494E0((s32)&msg, 0x42);
    msg.unk4 = D_800A1C40[index][1];
    func_151494E0((s32)&msg, 0x42);
}

void func_150F9788(struct260 *arg0) {
}

void func_150F9788(struct260 *);

void func_150F9794(struct260 *arg0) {
    func_150F9788(arg0);
    func_1514933C(arg0);
}

void func_150F97C0(struct260 *arg0) {
    func_150F9788(arg0);
    func_15149368(arg0);
}

extern u8 D_80088B50;
extern Vec3f D_800A1C48;
extern Vec3f D_800A1C54;
extern s32 func_15145EA4(Vec3f **src, Vec3f **dst, void *mtx, s32 count);
extern void func_15102B38(Actor *obj, u8 bone, Vec3f *arg2, Vec3f *arg3, f32 *arg4, s32 arg5, s32 arg6,
                          f32 arg7, Vec3f *position, s32 arg9, s32 arg10, s32 arg11, u8 arg12, s32 arg13);

/* Effect at the point D_800A1C48 on bone D_80088B50 of `obj`. */
void func_150F97EC(obj, arg1, arg2)
Actor *obj;
u8 arg1;
s32 arg2;
{
    Vec3f pos;
    f32 one;
    f32 motion[2];
    Vec3f *src;
    Vec3f *dst;
    void *mtx;
    s32 random1;
    s32 random0;

    if (obj->unk1D4 == NULL) {
        return;
    }
    src = &D_800A1C48;
    dst = &pos;
    mtx = obj->unk1D4[D_80088B50];
    func_15145EA4(&src, &dst, mtx, 1);
    one = 1.0f; // matching: IDO folds a literal "* 1.0f" away; the original multiplies by a variable
    motion[1] = func_150ADA68() * one + 2.0f;
    motion[0] = func_150ADA68() * 14.0f + 28.0f;
    random0 = func_150ADA20();
    random1 = func_150ADA20();
    func_15102B38(obj, D_80088B50, &D_800A1C48, &D_800A1C54, motion, (random0 % 3U) + 4, (random1 % 156U) + 100,
                  func_150ADA68() * 300.0f + 400.0f, &pos, 0xFF, 0, -1, arg1, arg2);
}

/* Muzzle effects of `obj`'s gun attachment (set 0x82): flash at the muzzle, impact at the hit point. */
void func_150F9950(obj, arg1, arg2)
Actor *obj;
u8 arg1;
s32 arg2;
{
    Vec3f points[6];
    Vec3f position;
    Vec3f hit;
    Vec3f normal;
    Vec3f unk30;
    ActorCtl *gun;

    gun = func_150FF288(obj);
    if (gun != NULL) {
        if (func_150FF6E0(points, &position, &hit, &normal, &unk30, obj, gun) != 0) {
            func_151D3F14(&position, arg1, arg2);
            func_151D4408(&hit, &normal, obj->unk1D4[((u8 *)gun)[2]], obj, 1.0f, arg1, arg2);
            func_150FF474(&position, points, arg1, arg2);
        }
    }
}
