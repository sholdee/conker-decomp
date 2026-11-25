#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"
#include "libc/string.h"

/* Per-track state of a running cutscene (as in game_50D80.c); the effects below follow one. */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
} SeqTrackState; // size 0x44


/* First argument of func_15152190 (as Arg15108120 in game_1355D0.c). */
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ struct17 pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
} Arg15152190;

void func_15152190(Arg15152190 *, s32 *, f32 *, s32, f32, s32, s32, s32);

/* Owner of a func_1519E970 timer (EvNodeData in game_50D80.c): unk30 holds the running timer. */
typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ struct EvTimer *timer;
} EvTimerOwner;

/* Effect created by func_1519E970 (0x2C bytes), counted down by func_1519EA04. */
typedef struct EvTimer {
    /* 0x00 */ u8 pad0[0x10]; // effect header, see func_15167A68
    /* 0x10 */ s32 flags;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ f32 *unk18;
    /* 0x1C */ f32 *unk1C;
    /* 0x20 */ s16 timeLeft;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ EvTimerOwner *owner;
    /* 0x28 */ u8 keepOwnerLink; /* 0: clear owner->timer on expiry */
    /* 0x29 */ u8 pad29[3];
} EvTimer; // size 0x2C

extern void *func_15167A68(s32, s32, s32, s32, u8, s32);

/* Starts a timer effect that expires after `duration` frames of D_800BE9E4. */
EvTimer *func_1519E970(s16 duration, EvTimerOwner *owner, u8 keepOwnerLink, f32 *arg3, f32 *arg4, u8 arg5,
                       s32 arg6) {
    EvTimer *timer = func_15167A68(0x26, arg6, sizeof(EvTimer), 1, arg5, 1);

    if (timer == NULL) {
        return NULL;
    }
    timer->unk18 = arg3;
    timer->unk1C = arg4;
    timer->timeLeft = duration;
    timer->keepOwnerLink = keepOwnerLink;
    timer->owner = owner;
    timer->flags = 1;
    timer->unk14 = 0;
    return timer;
}

/* Per-frame update of a func_1519E970 timer: frees it (unlinking its owner) once its time runs out. */
void func_1519EA04(EvTimer *timer) {
    if (timer->flags & 1) {
        s32 expired = FALSE;

        timer->timeLeft -= D_800BE9E4;
        if (timer->timeLeft < 0) {
            expired = TRUE;
        }
        if (expired) {
            if (timer->keepOwnerLink == 0) {
                EvTimerOwner *owner = timer->owner;
                owner->timer = NULL;
            }
            func_1516972C((struct102 *)timer);
        }
    }
}

/* Spawns a func_15152190 effect at pos (cutscene event; the caller passes an id < 0xE9 and a scale). */
void func_1519EA78(struct17 *pos, u16 id, f32 scale, u8 arg3, s32 arg4) {
    Arg15152190 def;
    s32 idArg;
    f32 scaleArg;

    def.unk00 = 10;
    def.unk04 = 7;
    def.pos = *pos;
    def.unk14 = 0;
    def.unk16 = 0xFF;
    def.unk18 = -0x35;
    def.unk1A = 0x18;
    def.unk1C = 10.0f;
    def.unk20 = 8.0f;
    def.unk24 = -1.32400012f;
    def.unk28 = 0.7440000176f;
    def.unk2C = 50;
    def.unk2E = 20;
    def.unk30 = 0.6000000238f;
    def.unk34 = 0.8000000119f;
    def.unk38 = 26.18200111f;
    idArg = id;
    scaleArg = scale;
    func_15152190(&def, &idArg, &scaleArg, 1, 0.0f, 0, arg3, arg4);
}

/* An object spawned by func_1513264C (EffectObj in game_124920.c), here following a cutscene track. */
typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ f32 unk18;
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ f32 unk20;
    /* 0x024 */ f32 unk24;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ u8 pad2C[0xC];
    /* 0x038 */ struct17 pos;
    /* 0x044 */ u8 pad44[0x12C];
    /* 0x170 */ SeqTrackState *track;
} EffectObj;

EffectObj *func_1513264C(void *spawn, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);

/* Spawns a func_1513264C effect that follows a cutscene track (updated by func_1519ED24). */
void func_1519EB8C(SeqTrackState *track, u16 id, s16 duration, u8 arg3, s32 arg4) {
    struct {
        SeqTrackState *track;
        ParticleSpawn spawn;
    } packet;
    EffectObj *obj;

    packet.track = track;
    packet.spawn.radius = 1.0f;
    packet.spawn.elasticity = 1.0f;
    packet.spawn.sizeXZ = track->unk18 * 0.1f;
    packet.spawn.sizeY = track->unk1C * 0.1f;
    packet.spawn.rot.x = track->unkC;
    packet.spawn.rot.y = track->unk10;
    packet.spawn.rot.z = track->unk14;
    packet.spawn.scale.x = 1.0f;
    packet.spawn.scale.y = 1.0f;
    packet.spawn.scale.z = 1.0f;
    packet.spawn.pos.x = track->x;
    packet.spawn.pos.y = track->y;
    packet.spawn.pos.z = track->z;
    packet.spawn.vel = *(vertex *)&D_800A5480;
    packet.spawn.rotVel = *(vertex *)&D_800A5480;
    packet.spawn.gravity = 0.0f;
    packet.spawn.flags = 0x980;
    packet.spawn.lifetime = duration;
    packet.spawn.model = id;
    packet.spawn.variant = 0;
    packet.spawn.unk5C = 0;
    packet.spawn.alpha = 0xFF;
    packet.spawn.updateFn = 0x15;
    packet.spawn.update2Fn = 0;
    packet.spawn.floorHitFn = 0;
    packet.spawn.floorType3Fn = 0;
    packet.spawn.wallHitFn = 0;
    packet.spawn.destroyFn = 0;
    packet.spawn.squashFn = 0;
    packet.spawn.matrixFn = 2;
    packet.spawn.unk6A = 0;
    packet.spawn.owner = NULL;
    packet.spawn.ownerId = 0;
    packet.spawn.fadeStart = 1;
    packet.spawn.fadeRate = 0xFF;
    packet.spawn.sound = 0;
    obj = func_1513264C(&packet.spawn, 3, 0xFF, 0, 4, arg3, arg4);
    if (obj != NULL) {
        memcpy(&obj->track, &packet.track, sizeof(packet.track));
    }
}

/* Per-frame update of a func_1519EB8C effect: copies its track's current values into it. */
s32 func_1519ED24(EffectObj *obj) {
    SeqTrackState *track = obj->track;

    obj->unk18 = track->unk18 * 0.1f;
    obj->unk1C = track->unk1C * 0.1f;
    obj->unk20 = track->unkC;
    obj->unk24 = track->unk10;
    obj->unk28 = track->unk14;
    obj->pos.unk0 = track->x;
    obj->pos.unk4 = track->y;
    obj->pos.unk8 = track->z;
    return 1;
}

/* A particle spawned by func_1513D2F0 (Particle in game_12FDD0.c), here following a cutscene track. */
typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ ParticleDef def;
    /* 0x070 */ u8 pad70[0xA0];
    /* 0x110 */ SeqTrackState *track;
} Particle;

/* Spawns a particle that follows a cutscene track (updated by func_1519EF04). */
void func_1519ED84(SeqTrackState *track, s32 texture, s16 lifetime, u8 arg3, s32 arg4) {
    ParticleDef def;
    SeqTrackState *trackArg;
    Particle *particle;

    trackArg = track;
    def.texture = texture;
    def.type = 0;
    def.renderModes = 0x3B03;
    def.lifetime = lifetime;
    def.frame = 0;
    def.frameSpeed = 0;
    def.rgba[0] = 0xFF;
    def.rgba[1] = 0xFF;
    def.rgba[2] = 0xFF;
    def.rgba[3] = 0xFF;
    def.width = track->unk18 * 10.0f;
    def.height = track->unk1C * 10.0f;
    def.pos.x = track->x;
    def.pos.y = track->y;
    def.pos.z = track->z;
    def.rot.x = track->unkC;
    def.rot.y = track->unk10;
    def.rot.z = track->unk14;
    def.scale.x = 1.0f;
    def.scale.y = 1.0f;
    def.scale.z = 1.0f;
    def.flags = 0x45C0081;
    def.unk44 = 0xFF;
    def.unk45 = 0xFF;
    def.unk46 = 0;
    def.unk47 = 7;
    def.unk48 = NULL;
    def.viewportMask = 0xFF;
    def.unk50 = 0;
    def.fadeStart = 1;
    def.fadeRate = 0xFF;

    particle = func_1513D2F0(&def, (s32)&D_800A4AA0, 0x27, 0, 0, 0x17, 0, 3, 0xFF, 4, arg3, arg4);
    if (particle != NULL) {
        memcpy(&particle->track, &trackArg, sizeof(trackArg));
    }
}

/* Per-frame update of a func_1519ED84 particle: copies its track's current values into it. */
s32 func_1519EF04(Particle *particle) {
    SeqTrackState *track = particle->track;

    particle->def.width = track->unk18 * 10.0f;
    particle->def.height = track->unk1C * 10.0f;
    particle->def.rot.x = track->unkC;
    particle->def.rot.y = track->unk10;
    particle->def.rot.z = track->unk14;
    particle->def.pos.x = track->x;
    particle->def.pos.y = track->y;
    particle->def.pos.z = track->z;
    return 1;
}
