#include <ultra64.h>
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "libc/string.h"


/* Starts a timer effect that expires after `duration` frames of gFrameTicks. */
EvTimer *cutscene_startTimer(s16 duration, EvTimerOwner *owner, u8 keepOwnerLink, f32 *arg3, f32 *arg4, u8 fxOwner,
                       s32 fxList) {
    EvTimer *timer = effect_newObject(0x26, fxList, sizeof(EvTimer), 1, fxOwner, 1);

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

/* Per-frame update of a cutscene_startTimer timer: frees it (unlinking its owner) once its time runs out. */
void cutscene_updateTimer(EvTimer *timer) {
    if (timer->flags & 1) {
        s32 expired = FALSE;

        timer->timeLeft -= gFrameTicks;
        if (timer->timeLeft < 0) {
            expired = TRUE;
        }
        if (expired) {
            if (timer->keepOwnerLink == 0) {
                EvTimerOwner *owner = timer->owner;
                owner->timer = NULL;
            }
            effect_destroyObject(&timer->header);
        }
    }
}

/* Spawns a particle_spawnDebrisBurst effect at pos (cutscene event; the caller passes an id < 0xE9 and a scale). */
void cutscene_spawnDebrisBurst(Vec3f *pos, u16 id, f32 scale, u8 owner, s32 list) {
    DebrisBurst def;
    s32 idArg;
    f32 scaleArg;

    def.countBase = 10;
    def.countRange = 7;
    def.pos = *pos;
    def.angles.yawBase = 0;
    def.angles.yawRange = 0xFF;
    def.angles.pitchBase = -0x35;
    def.angles.pitchRange = 0x18;
    def.speedBase = 10.0f;
    def.speedRange = 8.0f;
    def.gravityBase = -1.32400012f;
    def.gravityRange = 0.7440000176f;
    def.lifeBase = 50;
    def.lifeRange = 20;
    def.scaleBase = 0.6000000238f;
    def.scaleRange = 0.8000000119f;
    def.spin = 26.18200111f;
    idArg = id;
    scaleArg = scale;
    particle_spawnDebrisBurst(&def, &idArg, &scaleArg, 1, 0.0f, 0, owner, list);
}

/* Spawns a particle_newModelParticleWithFloor effect that follows a cutscene track (updated by cutscene_updateTrackEffect). */
void cutscene_spawnTrackEffect(SeqTrackState *track, u16 id, s16 duration, u8 owner, s32 list) {
    struct {
        SeqTrackState *track;
        ParticleSpawn spawn;
    } packet;
    TrackPhysicsParticle *obj;

    packet.track = track;
    packet.spawn.radius = 1.0f;
    packet.spawn.elasticity = 1.0f;
    packet.spawn.sizeXZ = track->unk18 * 0.1f;
    packet.spawn.sizeY = track->unk1C * 0.1f;
    packet.spawn.rot.x = track->pitch;
    packet.spawn.rot.y = track->yaw;
    packet.spawn.rot.z = track->roll;
    packet.spawn.scale.x = 1.0f;
    packet.spawn.scale.y = 1.0f;
    packet.spawn.scale.z = 1.0f;
    packet.spawn.pos.x = track->x;
    packet.spawn.pos.y = track->y;
    packet.spawn.pos.z = track->z;
    packet.spawn.vel = gZeroVec3f;
    packet.spawn.rotVel = gZeroVec3f;
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
    obj = particle_newModelParticleWithFloor(&packet.spawn, 3, 0xFF, 0, 4, owner, list);
    if (obj != NULL) {
        memcpy(&obj->track, &packet.track, sizeof(packet.track));
    }
}

/* Per-frame update of a cutscene_spawnTrackEffect effect: copies its track's current values into it. */
s32 cutscene_updateTrackEffect(TrackPhysicsParticle *obj) {
    SeqTrackState *track = obj->track;

    obj->unk18 = track->unk18 * 0.1f;
    obj->unk1C = track->unk1C * 0.1f;
    obj->unk20 = track->pitch;
    obj->unk24 = track->yaw;
    obj->unk28 = track->roll;
    obj->pos.x = track->x;
    obj->pos.y = track->y;
    obj->pos.z = track->z;
    return 1;
}

/* Spawns a particle that follows a cutscene track (updated by cutscene_updateTrackParticle). */
void cutscene_spawnTrackParticle(SeqTrackState *track, s32 texture, s16 lifetime, u8 owner, s32 list) {
    ParticleDef def;
    SeqTrackState *trackArg;
    TrackParticle *particle;

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
    def.rot.x = track->pitch;
    def.rot.y = track->yaw;
    def.rot.z = track->roll;
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

    particle = particle_newQuadParticle(&def, &gWhiteVtxColors, 0x27, 0, 0, 0x17, 0, 3, 0xFF, 4, owner, list);
    if (particle != NULL) {
        memcpy(&particle->track, &trackArg, sizeof(trackArg));
    }
}

/* Per-frame update of a cutscene_spawnTrackParticle particle: copies its track's current values into it. */
s32 cutscene_updateTrackParticle(TrackParticle *particle) {
    SeqTrackState *track = particle->track;

    particle->head.def.width = track->unk18 * 10.0f;
    particle->head.def.height = track->unk1C * 10.0f;
    particle->head.def.rot.x = track->pitch;
    particle->head.def.rot.y = track->yaw;
    particle->head.def.rot.z = track->roll;
    particle->head.def.pos.x = track->x;
    particle->head.def.pos.y = track->y;
    particle->head.def.pos.z = track->z;
    return 1;
}
