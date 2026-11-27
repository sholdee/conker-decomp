#include <ultra64.h>
#include <math.h>
#include "conker/beam.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/sprite.h"
#include "conker/texture.h"

/* Spawns a beam effect between actors 0x17 and 0x16. */
WavyBeamEffect *beam_spawnWavyBeam(s16 duration, u8 arg1, s32 arg2) {
    WavyBeamEffect *effect;
    WavyBeamData data;

    data.firstId = 0x17;
    data.secondId = 0x16;
    data.first = collision_findObjectById(0x17);
    data.second = collision_findObjectById(data.secondId);
    data.fadeEnd = 250.0f;
    data.width = 18.0f;
    data.fadeStart = 0.0f;
    data.fadeMax = 0.9f;
    data.fadeScale = 1.0f / data.fadeEnd;

    effect = effect_newTimedEffect(duration, -1, -1, 1, 1, 0x15, sizeof(data), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    return effect;
}

static const u8 gWavyBeamFirstBone = 1; // first's bone
static const u8 gWavyBeamSecondBone = 1; // second's bone
static const Vec3f gWavyBeamStart = { -3.0f, 11.0f, -4.0f };  // beam start, in first's bone space
static const Vec3f gWavyBeamEnd = { 0.0f, -5.0f, -7.0f };   // beam end, in second's bone space

/*
 * Beam effect draw: a red textured strip of 10 quads from a point on first's bone gWavyBeamFirstBone to a
 * point on second's bone gWavyBeamSecondBone, facing the eye of view viewIdx. Once the beam is longer than
 * fadeStart, its width ripples along a sine wave. Kills the effect when either actor is gone.
 */
Gfx *beam_drawWavyBeam(Gfx *gdl, WavyBeamEffect *effect, s16 viewIdx) {
    WavyBeamData *data;
    u8 failed;
    u8 flag;
    s32 i;
    Vtx *vtx;
    Vec3f *eye;
    f32 lenSq;
    f32 perpScale;
    Vec3f perp; // half-width offset, perpendicular to the beam and the view
    f32 scale;
    Vec3f start;
    Vec3f end;
    Vec3f dir;
    f32 viewX, viewY, viewZ; // eye to beam midpoint
    Vec3f cur;
    f32 texT;
    f32 count; // matching: never read, but IDO keeps its accumulation
    f32 length;
    f32 fade;
    f32 stepX, stepY, stepZ;
    f32 angle;
    f32 mag;
    f32 wave;

    failed = 0;
    data = &effect->data;
    if (data->first->type == 0 || data->first->unique_id != data->firstId) {
        failed = 1;
    }
    if (data->second->type == 0 || data->second->unique_id != data->secondId) {
        failed = 1;
    }
    if (!failed && data->first->ob_matrix_ptr != NULL && data->second->ob_matrix_ptr != NULL) {
        math_transformOffset(&gWavyBeamStart, &start, &data->first->ob_matrix_ptr[gWavyBeamFirstBone]);
        math_transformOffset(&gWavyBeamEnd, &end, &data->second->ob_matrix_ptr[gWavyBeamSecondBone]);
        flag = 1;
        vtx = data->vertices[gDrawBufferIndex];
        eye = &camera_global_Cameras[viewIdx].eye;
        gdl = math_gfxUpdateGeometryMode(gdl, 0x200005, 0x60600);
        gdl = math_gfxLoadTextureFrame(gdl, &gEffectTextures[86], 0, 0, 0, 0, 0x56, 0, 0, &flag, 3);
        gdl = math_gfxSetEnvColor(gdl, 0xFF, 0xFF, 0xFF, 0xFF, &flag);
        gdl = math_gfxSetPrimColor(gdl, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, &flag);
        gdl = particle_setCombineMode(gdl, 0x2E, &flag);
        gdl = math_gfxSetOtherMode(gdl, gSpriteTlutMode | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, gEffectRenderModes[5].cycle1 | gEffectRenderModes[5].cycle2, &flag);
        dir.x = end.x - start.x;
        dir.y = end.y - start.y;
        dir.z = end.z - start.z;
        length = math_vec3Length(&dir);
        viewX = dir.x * 0.5f + start.x - eye->x;
        viewY = dir.y * 0.5f + start.y - eye->y;
        viewZ = dir.z * 0.5f + start.z - eye->z;
        perp.x = dir.y * viewZ - viewY * dir.z;
        perp.y = dir.z * viewX - viewZ * dir.x;
        perp.z = dir.x * viewY - viewX * dir.y;
        lenSq = perp.x * perp.x + perp.y * perp.y + perp.z * perp.z;
        if (lenSq == 0.0f) {
            perp.x = 0.0f;
            perp.y = 0.0f;
            perp.z = 0.0f;
        } else {
            mag = sqrtf(lenSq);
            perpScale = data->width / mag;
            perp.x *= perpScale;
            perp.y *= perpScale;
            perp.z *= perpScale;
        }
        stepX = dir.x * 0.1f;
        stepY = dir.y * 0.1f;
        stepZ = dir.z * 0.1f;
        if (length < data->fadeStart) {
            fade = 0.0f;
        } else if (length < data->fadeEnd) {
            fade = (length - data->fadeStart) * data->fadeScale * data->fadeMax;
        } else {
            fade = data->fadeMax;
        }
        for (i = 0, cur = start, angle = 0.0f, texT = 0.0f, count = 0.0f; i < 10; i++) {
            wave = sinf(angle);
            scale = 1.0f - wave * fade;
            vtx[0].v.ob[0] = perp.x * scale + cur.x;
            vtx[0].v.ob[1] = perp.y * scale + cur.y;
            vtx[0].v.ob[2] = perp.z * scale + cur.z;
            vtx[0].v.cn[0] = 0xFF;
            vtx[0].v.cn[1] = 0;
            vtx[0].v.cn[2] = 0;
            vtx[0].v.cn[3] = 0xFF;
            vtx[0].v.tc[0] = 0;
            vtx[0].v.tc[1] = texT;
            vtx[1].v.ob[0] = cur.x - perp.x * scale;
            vtx[1].v.ob[1] = cur.y - perp.y * scale;
            vtx[1].v.ob[2] = cur.z - perp.z * scale;
            vtx[1].v.cn[0] = 0xFF;
            vtx[1].v.cn[1] = 0;
            vtx[1].v.cn[2] = 0;
            vtx[1].v.cn[3] = 0xFF;
            vtx[1].v.tc[0] = 0x7C0;
            vtx[1].v.tc[1] = texT;
            vtx += 2;
            wave = sinf(angle + 0.31415927f);
            scale = 1.0f - wave * fade;
            vtx[0].v.ob[0] = cur.x + stepX - perp.x * scale;
            vtx[0].v.ob[1] = cur.y + stepY - perp.y * scale;
            vtx[0].v.ob[2] = cur.z + stepZ - perp.z * scale;
            vtx[0].v.cn[0] = 0xFF;
            vtx[0].v.cn[1] = 0;
            vtx[0].v.cn[2] = 0;
            vtx[0].v.cn[3] = 0xFF;
            vtx[0].v.tc[0] = 0x7C0;
            vtx[0].v.tc[1] = texT + 204.8f;
            vtx[1].v.ob[0] = cur.x + stepX + perp.x * scale;
            vtx[1].v.ob[1] = cur.y + stepY + perp.y * scale;
            vtx[1].v.ob[2] = cur.z + stepZ + perp.z * scale;
            vtx[1].v.cn[0] = 0xFF;
            vtx[1].v.cn[1] = 0;
            vtx[1].v.cn[2] = 0;
            vtx[1].v.cn[3] = 0xFF;
            vtx[1].v.tc[0] = 0;
            vtx[1].v.tc[1] = texT + 204.8f;
            vtx += 2;
            gSPVertex(gdl++, vtx - 4, 4, 0);
            gSP1Triangle(gdl++, 0, 1, 2, 0);
            gSP1Triangle(gdl++, 0, 2, 3, 0);
            cur.x += stepX;
            cur.y += stepY;
            cur.z += stepZ;
            angle += 0.31415927f;
            texT += 204.8f;
            count += 0.1f;
        }
    }
    if (failed) {
        effect->base.header.state = -1;
    }
    return gdl;
}

/* Effect message handler: follows actor slot swaps (message 0x2D) and dies with either actor (message 0). */
void beam_wavyBeamMessage(WavyBeamEffect *effect, void *msg, u8 type) {
    WavyBeamData *data = &effect->data;

    if (type == 0x2D) {
        ActorSwapMsg *swap = msg;

        if (data->first == swap->actorA) {
            data->first = swap->actorB;
            data->firstId = swap->idB;
        } else if (data->first == swap->actorB) {
            data->first = swap->actorA;
            data->firstId = swap->idA;
        }

        if (data->second == swap->actorA) {
            data->second = swap->actorB;
            data->secondId = swap->idB;
        } else if (data->second == swap->actorB) {
            data->second = swap->actorA;
            data->secondId = swap->idA;
        }
    } else if (type == 0) {
        ActorRef *gone = msg;
        Actor *actor = gone->actor;

        if (actor == data->first || actor == data->second || gone->uniqueId == data->firstId || gone->uniqueId == data->secondId) {
            effect_destroyObject(&effect->base.header);
        }
    }
}
