#include <ultra64.h>
#include "functions.h"
#include "variables.h"

Vec3f gLastBeamDir;

/* Sends message 0x3E (effect_sendEventToTimedEffects) for `owner`; beams it owns destroy themselves on it. */
void beam_sendOwnerGone(Actor *owner) {
    ActorRef msg;

    msg.actor = owner;
    msg.uniqueId = owner->unique_id;
    effect_sendEventToTimedEffects((s32)&msg, 0x3E);
}

/* Spawns a beam object for `owner`, with two end sprites (blue-tinted, then red-tinted). */
BeamObj *beam_spawnCastBeam(Actor *owner) {
    BeamState state;
    u8 i;
    BeamObj *obj;

    state.owner = owner;
    state.ownerId = owner->unique_id;
    for (i = 0; i < 2; i++) {
        state.ends[i] = NULL;
    }
    state.flags = 0;
    obj = effect_newTimedEffect(300, -1, 0x43, -1, 0, 0x37, sizeof(BeamState), 0xFF, 1);
    if (obj != NULL) {
        BeamState *beam;

        beam = &obj->beam;
        memcpy(beam, &state, sizeof(state));
        {
            ParticleDesc spawn;

            spawn.renderMode = 0x4417;
            spawn.unk0 = 0x200004;
            spawn.unk4 = 0;
            spawn.timer = 300;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.r = 0xFF;
            spawn.g = 0xFF;
            spawn.b = 0xFF;
            spawn.a = 0xFF;
            spawn.brightness = 0xFF;
            spawn.width = spawn.height = 1511.0f;
            spawn.pos = D_800A5480;
            spawn.offset = D_800A5480;
            spawn.vel = D_800A5480;
            spawn.fadeStart = 1;
            spawn.fadeRate = 0xFF;
            spawn.scaleStart = 1;
            spawn.flags = 0x64C000;
            spawn.primColorMode = 8;
            spawn.envColorMode = 6;
            spawn.updateFunc = -1;
            spawn.deathFunc = -1;
            spawn.drawFunc = 3;
            spawn.handlerSet = 0;
            spawn.viewMask = 0;
            spawn.views = 0xFF;
            spawn.minSize = 10;
            spawn.texture = 100;
            spawn.r2 = 0;
            spawn.g2 = 0;
            spawn.b2 = 0xFF;
            spawn.alpha = 0xFF;
            spawn.gravity = 0.0f;
            spawn.scaleRate = 1.0f;
            spawn.maxSize = 20.0f;
            beam->ends[0] = func_15130280(&spawn, 1, 0, 0, obj->base.header.owner, obj->base.header.kind);
            spawn.r2 = 0xFF;
            spawn.g2 = 0;
            spawn.b2 = 0;
            spawn.alpha = 0xFF;
            beam->ends[1] = func_15130280(&spawn, 1, 0, 0, obj->base.header.owner, obj->base.header.kind);
        }
    }
    return obj;
}

/* Per-frame update: re-casts the beam from the owner and moves the end sprites to where it hits. */
void beam_updateCastBeam(BeamObj *obj) {
    BeamState *beam;
    Actor *owner;
    Vec3f origin;
    Vec3f hitPos;

    beam = &obj->beam;
    owner = beam->owner;

    if (owner->type == 0 || owner->unique_id != beam->ownerId) {
        obj->base.header.state = -1;
        return;
    }

    if (D_800C35EA[0] == 1) {
        if (beam->ends[0] != NULL && beam->ends[1] != NULL) {
            beam->ends[0]->mode = 3;
            beam->ends[1]->mode = 3;
        }
        return;
    }

    origin.x = owner->xpos;
    origin.y = owner->ypos + 46.0f;
    origin.z = owner->zpos;
    func_15145740(owner, &gLastBeamDir, NULL, NULL, 0.0f);

    gLastBeamDir.x *= 0.001f;
    gLastBeamDir.y *= 0.001f;
    gLastBeamDir.z *= 0.001f;

    collision_castRay(owner, origin.x, origin.y, origin.z,
                  gLastBeamDir.x, gLastBeamDir.y, gLastBeamDir.z,
                  &beam->ray, 5000.0f, 0, 0, 1, -1, 0, 0);

    beam->flags |= 1;

    if (beam->ends[0] != NULL && beam->ends[1] != NULL) {
        if (beam->ray.hitType == 0) {
            beam->ends[0]->mode = beam->ends[1]->mode = 3;
        } else {
            hitPos = beam->ray.pos;
            beam->ends[1]->pos = hitPos;
            beam->ends[0]->pos = hitPos;

            if (beam->ray.hitType == 1) {
                beam->ends[0]->mode = -1;
                beam->ends[1]->mode = 3;
            } else if (actor_getAlpha(beam->ray.obj, 0) < 0xFF) {
                beam->ends[0]->mode = -1;
                beam->ends[1]->mode = 3;
            } else {
                actor_flashFog(beam->ray.obj, 0xFF, 0, 0, 0x7F, 0, 0x10);
                beam->ends[0]->mode = 3;
                beam->ends[1]->mode = -1;
            }
        }
    }
}

/* Message handler: destroys the beam when its owner is reported gone (0x3E); other messages go to the default handler. */
void beam_castBeamMessage(BeamObj *obj, ActorRef *msg, u8 msgId) {
    // matching: taking &obj keeps it in its arg slot (IDO reloads it instead of saving it in s0)
    BeamObj **objp = &obj;
    BeamState *beam;

    if (msgId == 0x3E) {
        beam = &(*objp)->beam;
        // matching: the punned read of msg makes IDO load it a second time for the id compare
        if (msg->actor == beam->owner || beam->ownerId == ((ActorRef *)*(s32 *)&msg)->uniqueId) {
            effect_destroyObject(&(*objp)->base.header);
        }
    } else {
        beam = &(*objp)->beam;
        effect_handleTimedEffectOwnerEvent((s32)msg, msgId, &beam->owner, &beam->ownerId, *objp);
    }
}

/* Destroys the beam's end sprites and detaches the beam from its owner. */
void beam_teardownCastBeam(BeamObj *obj) {
    BeamState *beam;
    u8 i;

    beam = &obj->beam;
    for (i = 0; i < 2; i++) {
        if (beam->ends[i] != NULL) {
            effect_destroyObject(&beam->ends[i]->header);
        }
    }
    effect_detachHandle(obj, beam->owner);
}

/* Beam callback: tears the beam down (beam_teardownCastBeam), then hands it to effect_defaultDestroyTimedEffect. */
void beam_destroyCastBeam(BeamObj *obj) {
    beam_teardownCastBeam(obj);
    effect_defaultDestroyTimedEffect(&obj->base);
}

/* Beam callback: tears the beam down (beam_teardownCastBeam), then hands it to effect_defaultFreeTimedEffect. */
void beam_freeCastBeam(BeamObj *obj) {
    beam_teardownCastBeam(obj);
    effect_defaultFreeTimedEffect(&obj->base);
}
