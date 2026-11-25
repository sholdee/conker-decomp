#include <ultra64.h>
// matching: functions.h declares func_10010F88 void and func_150ADA20 u8; callers here use full s32 results
#define func_10010F88 func_10010F88_void_decl_in_functions_h
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_10010F88
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

extern f32 D_800A1BD8;
extern f32 sinf(f32);
extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);
extern s32 func_15146078(struct17 *, struct17 *, struct17 *);

/* Spawn descriptor for func_1513264C (0x7C bytes); EffectObj holds the same fields 0x10.. at +0x10. */
typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ struct17 pos;
    /* 0x34 */ struct17 vel;
    /* 0x40 */ struct17 unk40;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ s8 unk69;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct127 *owner;
    /* 0x70 */ u8 ownerId; /* owner->unique_id */
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ u8 pad76[2];
    /* 0x78 */ s32 sound; /* positional sound handle (func_10010F88), or 0 */
} EffectSpawn;

/* Per-object state of the projectile spawned by func_150F7470 (updated by func_150F78B4). */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 unk4;
    /* 0x08 */ u8 pad8[0x30];
    /* 0x38 */ struct17 dir;
    /* 0x44 */ u8 pad44[0x20];
    /* 0x64 */ s32 unk64;
    /* 0x68 */ f32 timeLeft;
    /* 0x6C */ f32 speed;
    /* 0x70 */ u8 flags; /* 1: impact effect, 2: ray-test the path, 4: see func_150F78B4 */
    /* 0x71 */ u8 pad71[3];
    /* 0x74 */ struct17 unk74;
    /* 0x80 */ struct17 unk80;
    /* 0x8C */ f32 trailDist;
    /* 0x90 */ f32 trailAge;
    /* 0x94 */ f32 trailAgeStep;
    /* 0x98 */ struct17 trailPos;
    /* 0xA4 */ struct17 trailStep;
    /* 0xB0 */ s8 unkB0;
    /* 0xB1 */ u8 padB1[3];
} ProjectileState;

/* Per-object state of the effect spawned by func_150F802C (updated by func_150F81BC). */
typedef struct {
    /* 0x00 */ f32 *target; /* position (3 floats), then 3 more floats copied to unk20 */
    /* 0x04 */ struct17 trailPos;
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
    /* 0x038 */ struct17 pos;
    /* 0x044 */ struct17 vel;
    /* 0x050 */ u8 pad50[0x2C];
    /* 0x07C */ struct127 *owner;
    /* 0x080 */ u8 pad80[8];
    /* 0x088 */ s32 sound;
    /* 0x08C */ u8 pad8C[0xE4];
    /* 0x170 */ union {
        ProjectileState projectile;
        FollowState follow;
    } state;
} EffectObj;

extern s32 func_151C4B0C(s32, ProjectileState *, struct17 *, struct17 *, f32 *, f32, u8, s32, struct17 *, u8 *, s32,
                         f32, struct127 *, s32, u8, u8, s32, u8);
extern void func_15145974(struct17 *, f32 *, f32 *);
EffectObj *func_1513264C(void *spawn, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);
s32 func_10010F88(s32 sound, u16 volume, s16 cents, u8 mix, s32 arg4, s16 x, s16 y, s16 z, s16 nearDistance,
                  s16 farDistance);

/* Spawns a projectile at `pos`, aimed along `aim` (or at `target` when arg2 is set). */
void func_150F7470(struct17 *pos, struct17 *aim, s32 arg2, struct17 *target, u8 targetIsDir, s32 arg5, f32 speed,
                   f32 arg7, f32 arg8, struct127 *owner, u8 arg10, u8 arg11, u8 arg12, u8 arg13, s32 arg14,
                   s8 arg15, s32 sound, u8 arg17, u8 arg18, s32 arg19) {
    ProjectileState state;
    EffectSpawn spawn;
    EffectObj *obj;
    struct17 aimDir;
    u8 hasAimDir;
    struct17 targetDir;
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
    state.trailStep.unk0 = state.dir.unk0 * 70.0f;
    state.trailStep.unk4 = state.dir.unk4 * 70.0f;
    state.trailStep.unk8 = state.dir.unk8 * 70.0f;
    state.flags |= (state.unk4 > 150.0f) ? 1 : 0;
    spawn.unk00 = 1.0f;
    spawn.unk04 = 1.0f;
    spawn.unk0C = 0.02000000142f;
    spawn.unk08 = 0.02000000142f;
    func_15145974(&state.dir, &spawn.unk14, &spawn.unk10);
    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.unk18 = 0.0f;
    spawn.pos = *pos;
    spawn.vel.unk0 = state.dir.unk0 * speed;
    spawn.vel.unk4 = state.dir.unk4 * speed;
    spawn.vel.unk8 = state.dir.unk8 * speed;
    spawn.unk40 = *(struct17 *)&D_800A5480;
    spawn.unk4C = 0.0f;
    spawn.unk50 = 0x120;
    spawn.unk54 = 300;
    spawn.unk56 = 0x2F;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk61 = 0x13;
    spawn.unk62 = 0;
    spawn.unk63 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 0;
    spawn.unk6A = 0;
    spawn.owner = owner;
    spawn.ownerId = owner->unique_id;
    spawn.unk72 = 1;
    spawn.unk74 = 0xFF;
    if (sound != 0) {
        spawn.sound = func_10010F88(sound, 18000, -500, 0, -1, spawn.pos.unk0, spawn.pos.unk4, spawn.pos.unk8, 1000,
                                    2000);
    } else {
        spawn.sound = 0;
    }
    obj = func_1513264C(&spawn, 0, 0, 0, 0xB4, arg18, arg19);
    if (obj != NULL) {
        memcpy(&obj->state.projectile, &state, sizeof(state));
    }
}

/* Spawn descriptor for func_1513D2F0. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 color[4];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ struct17 pos;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ s32 unk50;
} PuffSpawn;

/* Per-object state of a puff spawned by func_1513D2F0 (updated by func_150F7E20). */
typedef struct {
    /* 0x00 */ struct17 unk0;
    /* 0x0C */ struct17 unkC;
    /* 0x18 */ f32 timeLeft;
    /* 0x1C */ f32 phase;
    /* 0x20 */ f32 phaseSpeed;
    /* 0x24 */ f32 sizeBase;
    /* 0x28 */ f32 sizeAmplitude;
} PuffState;

/* An object spawned by func_1513D2F0. */
typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ f32 unk30;
    /* 0x034 */ u8 pad34[0x28];
    /* 0x05C */ u8 unk5C;
    /* 0x05D */ u8 pad5D[0xB3];
    /* 0x110 */ PuffState state;
} PuffObj;

/* Result of the func_15081690 ray cast. */
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ struct17 pos;
    /* 0x14 */ struct17 unk14;
    /* 0x20 */ struct17 unk20;
    /* 0x2C */ struct17 unk2C;
    /* 0x38 */ struct17 unk38;
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

void func_15081690(struct127 *obj, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, RayHit *hit, f32 maxDist, s32 arg9,
                   s32 arg10, s32 arg11, s32 arg12, s32 arg13, s32 arg14);
void func_151D4DAC(s32, struct127 *, struct17 *, struct17 *, RayHit *, s32, struct17 *, u8, s32);
void func_150F7F8C(struct17 *pos, u8 arg1, s32 arg2);

/*
 * Per-frame update of a projectile: ray-tests and moves it, runs out its lifetime, and leaves a
 * trail of puffs every 70 units travelled. Returns 0 once it hit something or expired.
 */
s8 func_150F78B4(EffectObj *obj) {
    ProjectileState *state;
    struct127 *owner;
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
        func_1000F9D4(obj->sound, obj->pos.unk0, obj->pos.unk4, obj->pos.unk8);
    }
    dist = state->speed * dt;
    if (state->flags & 2) {
        func_15081690(owner, obj->pos.unk0, obj->pos.unk4, obj->pos.unk8, state->dir.unk0, state->dir.unk4,
                      state->dir.unk8, &hit, dist, 1, 0, !(state->flags & 4), state->unkB0, 0, 0);
        if (hit.hit >= 2) {
            func_151D4DAC(hit.unk00, owner, &hit.pos, &hit.unk20, &hit, 0x1A, &state->dir, obj->unkC, obj->unk1);
            if (state->flags & 1) {
                func_150F7F8C(&hit.pos, obj->unkC, obj->unk1);
            }
            alive = FALSE;
        }
    }
    obj->pos.unk0 += obj->vel.unk0 * dt;
    obj->pos.unk4 += obj->vel.unk4 * dt;
    obj->pos.unk8 += obj->vel.unk8 * dt;
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
        PuffSpawn spawn;

        state->trailDist += dist;
        state->trailAge += dt;
        puffs = state->trailDist * (1.0f / 70.0f);
        if (puffs > 1.0f) {
            PuffState puffState;

            puffState.unk0 = state->unk74;
            puffState.unkC = state->unk80;
            puffState.sizeBase = 8.318000793f;
            puffState.sizeAmplitude = 8.318000793f;
            spawn.unk0 = 0x7A;
            spawn.unk1 = 0;
            spawn.unk2 = 0x4404;
            spawn.unk4 = 10;
            spawn.unk8 = 0;
            spawn.unkC = 0;
            spawn.color[0] = 0xFF;
            spawn.color[1] = 0xFF;
            spawn.color[2] = 0xFF;
            spawn.color[3] = 0xFF;
            spawn.unk14 = 0.0f;
            spawn.unk18 = 0.0f;
            puffState.phaseSpeed = 0.4670000076f;
            spawn.unk28 = *(struct17 *)&D_800A5480;
            spawn.unk34 = 1.0f;
            spawn.unk38 = 1.0f;
            spawn.unk3C = 1.0f;
            spawn.unk40 = 0xCC0008;
            spawn.unk45 = 0xFF;
            spawn.unk46 = 0;
            spawn.unk47 = 6;
            spawn.unk48 = 0;
            spawn.unk4C = 0xFF;
            spawn.unk50 = 0;
            do {
                extern s32 D_800A4AA0; // matching: a declaration here opens the loop scope IDO's scheduler needs

                puffState.phase = 4.712388992f;
                puffState.timeLeft = 27.0f - state->trailAge;
                spawn.pos = state->trailPos;
                spawn.unk44 = puffState.timeLeft * 6.481481552f;
                puff = func_1513D2F0(&spawn, (s32)&D_800A4AA0, 0x24, 0, 0, 0x21, 0, 0, 0, 0x2C, obj->unkC, obj->unk1);
                if (puff != NULL) {
                    memcpy(&puff->state, &puffState, sizeof(puffState));
                }
                state->trailDist -= 70.0f;
                state->trailAge -= state->trailAgeStep;
                state->trailPos.unk0 += state->trailStep.unk0;
                state->trailPos.unk4 += state->trailStep.unk4;
                state->trailPos.unk8 += state->trailStep.unk8;
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
    obj->unk5C = state->timeLeft * D_800A1BD8;
    size = (sinf(state->phase) * state->sizeAmplitude) + state->sizeBase;
    obj->unk30 = size;
    obj->unk2C = size;
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

extern f32 func_150ADA68(void);
void func_151541B8(struct17 *pos, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u8 arg5, s32 arg6);
void func_151D3F14(struct17 *pos, u8, s32);
struct126 *func_150FF288(struct127 *);
s32 func_150FF6E0(struct17 *, struct17 *, struct17 *, struct17 *, struct17 *, struct127 *, struct126 *);
void func_150FF474(struct17 *position, struct17 *points, u8 arg2, s32 arg3);
void func_151D4408(struct17 *, struct17 *, void *, struct127 *, f32, s32, s32);

/* Impact effect at `pos`. */
void func_150F7F8C(struct17 *pos, u8 arg1, s32 arg2) {
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
        EffectSpawn spawn;
        FollowState follow;
    } packet;
    EffectObj *obj;

    packet.follow.target = target;
    packet.follow.trailPos.unk0 = target[0];
    packet.follow.trailPos.unk4 = target[1];
    packet.follow.trailPos.unk8 = target[2];
    packet.follow.trailAge = 0.0f;
    packet.spawn.unk00 = 1.0f;
    packet.spawn.unk04 = 1.0f;
    packet.spawn.unk08 = 0.02000000142f;
    packet.spawn.unk0C = 0.02000000142f;
    packet.spawn.unk10 = target[3];
    packet.spawn.unk14 = target[4];
    packet.spawn.unk18 = target[5];
    packet.spawn.unk1C = 1.0f;
    packet.spawn.unk20 = 1.0f;
    packet.spawn.unk24 = 1.0f;
    packet.spawn.pos.unk0 = target[0];
    packet.spawn.pos.unk4 = target[1];
    packet.spawn.pos.unk8 = target[2];
    packet.spawn.vel.unk0 = 0.0f;
    packet.spawn.vel.unk4 = 0.0f;
    packet.spawn.vel.unk8 = 0.0f;
    packet.spawn.unk40 = *(struct17 *)&D_800A5480;
    packet.spawn.unk4C = 0.0f;
    packet.spawn.unk50 = 0x980;
    packet.spawn.unk54 = arg1;
    packet.spawn.unk56 = 0x2F;
    packet.spawn.unk58 = 0;
    packet.spawn.unk5C = 0;
    packet.spawn.unk60 = 0xFF;
    packet.spawn.unk61 = 0x14;
    packet.spawn.unk62 = 0;
    packet.spawn.unk63 = 0;
    packet.spawn.unk64 = 0;
    packet.spawn.unk65 = 0;
    packet.spawn.unk66 = 0;
    packet.spawn.unk67 = 0;
    packet.spawn.unk68 = 0;
    packet.spawn.unk6A = 0;
    packet.spawn.owner = NULL;
    packet.spawn.ownerId = 0;
    packet.spawn.unk72 = 1;
    packet.spawn.unk74 = 0xFF;
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
    obj->pos.unk0 = follow->target[0];
    obj->pos.unk4 = follow->target[1];
    obj->pos.unk8 = follow->target[2];
    obj->unk20 = follow->target[3];
    obj->unk24 = follow->target[4];
    obj->unk28 = follow->target[5];
    {
        struct17 delta;
        f32 dist;
        f32 unused;
        struct17 dir;
        f32 puffs;

        delta.unk0 = obj->pos.unk0 - follow->trailPos.unk0;
        delta.unk4 = obj->pos.unk4 - follow->trailPos.unk4;
        delta.unk8 = obj->pos.unk8 - follow->trailPos.unk8;
        follow->trailAge += D_800BE9A4;
        if (func_15145128(&delta, &dir, &dist, &unused) != 0) {
            struct17 step;
            PuffObj *puff;
            PuffSpawn spawn;
            PuffState puffState;

            step.unk0 = dir.unk0 * 20.0f;
            step.unk4 = dir.unk4 * 20.0f;
            step.unk8 = dir.unk8 * 20.0f;
            puffs = dist * 0.05000000075f;
            if (puffs > 1.0f) {
                f32 ageStep;

                ageStep = follow->trailAge / puffs;
                func_15146078(&delta, &puffState.unk0, &puffState.unkC);
                puffState.phaseSpeed = 0.4630000293f;
                puffState.sizeBase = 8.241000175f;
                puffState.sizeAmplitude = 3.888999939f;
                spawn.unk0 = 0x7A;
                spawn.unk1 = 0;
                spawn.unk2 = 0x4404;
                spawn.unk4 = 10;
                spawn.unk8 = 0;
                spawn.unkC = 0;
                spawn.color[0] = 0xFF;
                spawn.color[1] = 0xFF;
                spawn.color[2] = 0xFF;
                spawn.color[3] = 0xFF;
                spawn.unk18 = 0.0f;
                spawn.unk14 = 0.0f;
                spawn.unk28 = *(struct17 *)&D_800A5480;
                spawn.unk34 = 1.0f;
                spawn.unk38 = 1.0f;
                spawn.unk3C = 1.0f;
                spawn.unk40 = 0xCC0008;
                spawn.unk45 = 0xFF;
                spawn.unk46 = 0;
                spawn.unk47 = 6;
                spawn.unk48 = 0;
                spawn.unk4C = 0xFF;
                spawn.unk50 = 0;
                do {
                    extern s32 D_800A4AA0; // matching: a declaration here opens the loop scope IDO's scheduler needs

                    puffState.phase = 4.712388992f;
                    puffState.timeLeft = 27.0f - follow->trailAge;
                    spawn.pos = follow->trailPos;
                    spawn.unk44 = puffState.timeLeft * 6.481481552f;
                    puff = func_1513D2F0(&spawn, (s32)&D_800A4AA0, 0x24, 0, 0, 0x21, 0, 0, 0, 0x2C, obj->unkC,
                                         obj->unk1);
                    if (puff != NULL) {
                        memcpy(&puff->state, &puffState, sizeof(puffState));
                    }
                    follow->trailPos.unk0 += step.unk0;
                    follow->trailPos.unk4 += step.unk4;
                    follow->trailPos.unk8 += step.unk8;
                    puffs -= 1.0f;
                    follow->trailAge -= ageStep;
                } while (puffs > 1.0f);
            }
        }
    }
    return 1;
}

/* Spawn descriptor for func_15152B38. */
typedef struct {
    s32 unk0;
    s32 unk4;
    struct17 pos;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s32 unk34;
    s32 unk38;
    s16 unk3C;
    s16 unk3E;
    u16 unk40;
    u8 unk42;
    u8 unk43;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    u8 unk48;
    u8 unk49;
    u8 unk4A;
    u8 unk4B;
    u8 unk4C;
    u8 unk4D;
    u8 unk4E;
    u8 unk4F;
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    s32 unk60;
    s16 unk64;
    s16 unk66;
    s16 unk68;
    u8 unk6A;
    u8 pad6B[1];
    f32 unk6C;
    s8 unk70;
    s8 unk71;
    s8 unk72;
    s8 unk73;
} EmitterSpawn;

extern struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, u8, u8, s32, u8, s32);
extern void func_15152B38(EmitterSpawn *, u8, s32);

/* Effects 50 units above `obj`: a func_151602C0 burst and a func_15152B38 emitter. */
void func_150F85A0(s32 arg0, struct127 *obj, s32 arg2) {
    struct17 pos;

    pos.unk0 = obj->x_position;
    pos.unk4 = obj->y_position + 50.0f;
    pos.unk8 = obj->z_position;
    {
        Header header;
        Header2 header2;

        header.unk0 = 3;
        header.unk1 = -1;
        header.unk2 = (func_150ADA20() % 9U) + 5;
        header.unk4 = 0;
        header2.unk0 = pos.unk0;
        header2.unk4 = pos.unk4;
        header2.unk8 = pos.unk8;
        func_151602C0(&header, &header2, (func_150ADA20() % 31U) + 90, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 1);
    }
    {
        EmitterSpawn spawn;

        spawn.unk0 = 8;
        spawn.unk4 = 12;
        spawn.pos = pos;
        spawn.unk14 = 3.07f;
        spawn.unk18 = 6.2f;
        spawn.unk1C = -0.9690000415f;
        spawn.unk20 = 0.7180000544f;
        spawn.unk24 = 15.0f;
        spawn.unk28 = 30.0f;
        spawn.unk2C = 0;
        spawn.unk2E = 0xFF;
        spawn.unk30 = -64;
        spawn.unk32 = 80;
        spawn.unk34 = 3;
        spawn.unk38 = 1;
        spawn.unk3C = 20;
        spawn.unk3E = 15;
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

/* One of a StageState's eight parts. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[0x58];
    /* 0x6E */ u8 unk6E;
} StagePart;

/* Something that steps through eight parts (one shown at a time) while emitting effects around (unk40, unk44). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5; /* bit 0 mirrors the effect angles */
    /* 0x06 */ s16 stageTimer;
    /* 0x08 */ u8 stage;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ StagePart *parts[8];
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
                state->parts[i]->unk6E = 1;
            }
            state->parts[7]->unk6E = 0;
            state->stage = 7;
        }
    } else if (type == 0x3F) {
        if (obj->stage.unk0 == msg->unk0) {
            func_150F892C(obj);
        }
    }
}

/* Spawn descriptor for func_15130280 (same layout as FlareSpawn in game_133190.c). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 life;
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
    /* 0x58 */ u32 flags; /* 0x40, 0x80: random flips */
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
} FlareSpawn;

extern void func_15164F0C(s32 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_15143874(s16 angle, f32 radius, f32 *x, f32 *z);
extern void func_151C329C(struct17 *pos, s32, s32);
extern void func_15143794(s16, s16, f32, struct17 *out);
extern void *func_15130280(FlareSpawn *spawn, u8, s32, s32, u8, s32);
void func_151DB5D0(s32, struct17 *pos, struct17 *vel, f32, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32);

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
        struct127 *other;

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
            state->parts[state->stage]->unk6E = 1;
            state->stage++;
            state->parts[state->stage]->unk6E = 0;
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
        struct17 pos;
        side = (state->unk5 & 1) ? -1 : 1;
        do {
            func_15143874((func_150ADA20() % 126U - 220) * side, state->unk48, &pos.unk0, &pos.unk8);
            pos.unk4 = func_150ADA68() * 150.0f + 30.0f;
            pos.unk0 += state->unk40;
            pos.unk8 += state->unk44;
            func_151C329C(&pos, obj->unkC, obj->unk1);
            state->unk38 -= 1.0f;
        } while (state->unk38 > 1.0f);
    }

    state->unk34 += (0.02000000142f + func_150ADA68() * 0.06000000238f) * (D_800BE9A4 * count);
    if (state->unk34 > 1.0f) {
        s16 pieces;
        s16 side;
        EffectSpawn spawn;
        side = (state->unk5 & 1) ? -1 : 1;
        spawn.unk00 = 1.0f;
        spawn.unk04 = 1.0f;
        spawn.unk1C = 1.0f;
        spawn.unk20 = 1.0f;
        spawn.unk24 = 1.0f;
        spawn.unk50 = 0x39E8;
        spawn.unk56 = 0x86;
        spawn.unk58 = 0;
        spawn.unk5C = 0;
        spawn.unk60 = 0xFF;
        spawn.unk61 = 8;
        spawn.unk62 = 0;
        spawn.unk63 = 0;
        spawn.unk64 = 0;
        spawn.unk65 = 0;
        spawn.unk66 = 0;
        spawn.unk67 = 0;
        spawn.unk68 = 0;
        spawn.unk6A = 2;
        spawn.owner = NULL;
        spawn.ownerId = 0;
        spawn.unk72 = 12;
        spawn.unk74 = 21;
        spawn.sound = 0;
        spawn.unk40.unk4 = 0.0f;
        do {
            s16 angle;

            pieces = func_150ADA20() % 7U + 5;
            angle = (func_150ADA20() % 126U - 220) * side;
            func_15143874(angle, state->unk48, &spawn.pos.unk0, &spawn.pos.unk8);
            spawn.pos.unk4 = func_150ADA68() * 150.0f + 30.0f;
            spawn.pos.unk0 += state->unk40;
            spawn.pos.unk8 += state->unk44;
            i = angle - 7; // matching: the base angle reuses the counter i (a new local changes registers)
            for (; pieces > 0; pieces--) {
                spawn.unk54 = (func_150ADA20() & 0xF) + 35;
                spawn.unk08 = spawn.unk0C = func_150ADA68() * 0.012f + 0.005f;
                spawn.unk10 = func_150ADA68() * 360.0f;
                spawn.unk14 = func_150ADA68() * 360.0f;
                spawn.unk18 = func_150ADA68() * 360.0f;
                func_15143794((func_150ADA20() & 0xF) + i, func_150ADA20() % 37U - 63,
                              func_150ADA68() * 8.0f + 10.0f, &spawn.vel);
                spawn.unk40.unk0 = func_150ADA68() * 32.41f + -16.205f;
                spawn.unk40.unk8 = func_150ADA68() * 32.41f + -16.205f;
                spawn.unk4C = func_150ADA68() * 0.337f + -1.177f;
                func_1513264C(&spawn, 3, 1, 0, 0, 0xFF, 1);
            }
            state->unk34 -= 1.0f;
        } while (state->unk34 > 1.0f);
    }

    state->unk30 += (0.05500000343f + func_150ADA68() * 0.05f) * (D_800BE9A4 * count);
    if (state->unk30 > 1.0f) {
        s16 side;
        s32 flares;
        FlareSpawn flare;
        side = (state->unk5 & 1) ? -1 : 1;
        flare.unk1D = 0x86;
        flare.unk8 = 0x4404;
        flare.unk0 = 0x200005;
        flare.unk4 = 0x9F0600;
        flare.unkC = 0;
        flare.unk10 = 0;
        flare.unk14 = 0xFF;
        flare.unk15 = 0xFF;
        flare.unk16 = 0xFF;
        flare.unk17 = 0xFF;
        flare.unk18 = 0xFF;
        flare.unk19 = 0xFF;
        flare.unk1A = 0xFF;
        flare.unk3C = *(struct17 *)&D_800A5480;
        flare.unk1E = 8;
        flare.unk20 = 0x1F;
        flare.unk22 = 1;
        flare.flags = 0x1C207;
        flare.unk60 = 6;
        flare.unk61 = 5;
        flare.unk62 = -1;
        flare.unk63 = -1;
        flare.unk64 = -1;
        flare.unk65 = 0;
        flare.unk5C = 0;
        flare.unk66 = 0xFF;
        flare.unk68 = 0;
        flare.unk24 = 1.0f;
        flare.unk6C = 0.0f;
        do {
            s16 angle;

            flares = func_150ADA20() % 13U + 7;
            angle = (func_150ADA20() % 126U - 220) * side;
            func_15143874(angle, state->unk48, &flare.pos.unk0, &flare.pos.unk8);
            flare.pos.unk4 = func_150ADA68() * 150.0f + 30.0f;
            flare.pos.unk0 += state->unk40;
            flare.pos.unk8 += state->unk44;
            i = angle - 4; // matching: the base angle reuses the counter i (a new local changes registers)
            for (; flares > 0; flares--) {
                s32 flip;

                flare.life = func_150ADA20() % 21U + 30;
                flare.unk1B = func_150ADA20() % 156U + 100;
                flare.unk28 = flare.unk2C = func_150ADA68() * 110.0f + 70.0f;
                flare.flags &= ~0xC0;
                flip = (func_150ADA20() & 1) ? 0x80 : 0;
                flare.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | flip;
                func_15143794(func_150ADA20() % 9U + i, (func_150ADA20() & 7) - 16,
                              func_150ADA68() * 10.0f + 6.0f, &flare.vel);
                flare.unk54 = func_150ADA68() * 0.268f + -0.3870000243f;
                func_15130280(&flare, 1, 0, 0, 0xFF, 1);
            }
            state->unk30 -= 1.0f;
        } while (state->unk30 > 1.0f);
    }

    state->unk2C += (0.3360000253f + func_150ADA68() * 0.126f) * (D_800BE9A4 * count);
    if (state->unk2C > 1.0f) {
        s16 side;
        struct17 pos;
        struct17 vel;
        side = (state->unk5 & 1) ? -1 : 1;
        do {
            s16 angle;

            angle = (func_150ADA20() % 126U - 220) * side;
            func_15143874(angle, state->unk48, &pos.unk0, &pos.unk8);
            pos.unk4 = func_150ADA68() * 150.0f + 30.0f;
            pos.unk0 += state->unk40;
            pos.unk8 += state->unk44;
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
extern struct17 D_800A1C48;
extern struct17 D_800A1C54;
extern s32 func_15145EA4(struct17 **src, struct17 **dst, void *mtx, s32 count);
extern void func_15102B38(struct127 *obj, u8 bone, struct17 *arg2, struct17 *arg3, f32 *arg4, s32 arg5, s32 arg6,
                          f32 arg7, struct17 *position, s32 arg9, s32 arg10, s32 arg11, u8 arg12, s32 arg13);

/* Effect at the point D_800A1C48 on bone D_80088B50 of `obj`. */
void func_150F97EC(obj, arg1, arg2)
struct127 *obj;
u8 arg1;
s32 arg2;
{
    struct17 pos;
    f32 one;
    f32 motion[2];
    struct17 *src;
    struct17 *dst;
    void *mtx;
    s32 random1;
    s32 random0;

    if (obj->unk1D4 == NULL) {
        return;
    }
    src = &D_800A1C48;
    dst = &pos;
    mtx = ((f32 (*)[4][4])obj->unk1D4)[D_80088B50];
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
struct127 *obj;
u8 arg1;
s32 arg2;
{
    struct17 points[6];
    struct17 position;
    struct17 hit;
    struct17 normal;
    struct17 unk30;
    struct126 *gun;

    gun = func_150FF288(obj);
    if (gun != NULL) {
        if (func_150FF6E0(points, &position, &hit, &normal, &unk30, obj, gun) != 0) {
            func_151D3F14(&position, arg1, arg2);
            func_151D4408(&hit, &normal, ((f32 (*)[4][4])obj->unk1D4)[((u8 *)gun)[2]], obj, 1.0f, arg1, arg2);
            func_150FF474(&position, points, arg1, arg2);
        }
    }
}
