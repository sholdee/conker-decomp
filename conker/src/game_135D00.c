#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#define TRACK_NONE 999

/* User data of the two swinging lamp models (DynModel kind 1). */
typedef struct {
    /* 0x00 */ f32 swingX;   // sway offsets, sin(phase) * amplitude
    /* 0x04 */ f32 swingZ;
    /* 0x08 */ f32 phaseX;
    /* 0x0C */ f32 phaseZ;
    /* 0x10 */ f32 height;
    /* 0x14 */ s32 track;    // sequence track that sets the height, TRACK_NONE for none
    /* 0x18 */ u8 sceneTag;
    /* 0x19 */ u8 pad19[3];
    /* 0x1C */ s32 duration; // counts down; the track is dropped when it runs out
    /* 0x20 */ u8 lightOn;   // toggled by message 0x1E
    /* 0x21 */ u8 pad21[3];
} LampData; // size 0x24

#define LAMP_DATA(model) ((LampData *)DYNMODEL_USERDATA(model))

/*
 * Spawns the two swinging lamp models (assets 427 and 428). The first gets a light effect that moves
 * `light` with it; the second is drawn by func_15109120 while the light is on.
 */
void func_15108850(PointLight *light) {
    DynModel *model;

    {
        DynModelDesc desc;
        LampData lamp;

        lamp.height = 226.0f;
        lamp.track = TRACK_NONE;
        lamp.sceneTag = 0xFF;
        lamp.duration = -1;
        lamp.lightOn = TRUE;
        desc.drawFunc = 1;
        desc.lifetime = 300;
        desc.unk30 = 9;
        lamp.swingX = 0.0f;
        lamp.swingZ = 0.0f;
        lamp.phaseX = 0.0f;
        lamp.phaseZ = 0.0f;
        desc.flags = 0;
        desc.updateFunc = 0;
        desc.unk34 = 427;
        desc.unk8 = 1;
        desc.unkC = 0x220205;
        desc.unk10 = 0x40600;
        desc.unk24 = 0;
        desc.unk25 = 0;
        desc.unk14 = 1;
        desc.unk18 = 0x36;
        desc.unk1C = 0x80;
        desc.unk20 = 0x20;
        desc.kind = 1;

        model = func_1513B5E0(&desc, 1, sizeof(LampData), 0xFF, 1);
        if (model != NULL) {
            memcpy(LAMP_DATA(model), &lamp, sizeof(LampData));
            {
                LightEffectHeader header;
                DynModel *owner;
                LightEffect *effect;

                owner = model;
                header.flags = 2;
                header.update = 8;
                header.lifetime = 300;
                header.lightType = 0x13;
                effect = func_1516037C(&header, light, 4, 0xFF, 1);
                if (effect != NULL) {
                    memcpy(&effect->data.object, &owner, sizeof(owner));
                }
            }
        }
    }

    {
        DynModel *model2;
        DynModelDesc desc;
        LampData lamp;

        lamp.height = 226.0f;
        lamp.track = TRACK_NONE;
        lamp.sceneTag = 0xFF;
        lamp.duration = -1;
        lamp.lightOn = TRUE;
        desc.updateFunc = 1;
        desc.drawFunc = 4;
        desc.lifetime = 300;
        desc.unk30 = 9;
        lamp.swingX = 0.0f;
        lamp.swingZ = 0.0f;
        lamp.phaseX = 0.0f;
        lamp.phaseZ = 0.0f;
        desc.flags = 0;
        desc.unk34 = 428;
        desc.unk8 = 1;
        desc.unkC = 0x220205;
        desc.unk10 = 0x40600;
        desc.unk24 = 0;
        desc.unk25 = 0;
        desc.unk14 = 1;
        desc.unk18 = 0x36;
        desc.unk1C = 0x80;
        desc.unk20 = 0x20;
        desc.kind = 1;

        model2 = func_1513B5E0(&desc, 1, sizeof(LampData), 0xFF, 1);
        if (model2 != NULL) {
            memcpy(LAMP_DATA(model2), &lamp, sizeof(LampData));
        }
    }
}

/* Update function 0 (first lamp model): advances the sway phases, then the track timer and height. */
s32 func_15108AB4(DynModel *model) {
    LampData *lamp = LAMP_DATA(model);

    lamp->phaseX += 0.0857f * D_800BE9A4;
    lamp->phaseZ += 0.0783f * D_800BE9A4;
    lamp->phaseX = func_15144B68(lamp->phaseX);
    lamp->phaseZ = func_15144B68(lamp->phaseZ);
    lamp->swingX = sinf(lamp->phaseX) * 7.1050005f;
    lamp->swingZ = sinf(lamp->phaseZ) * 7.1050005f;
    func_15108B80(model);
    func_15108BC0(model);
    return 1;
}

/* Counts the track duration down and drops the track when it runs out. */
void func_15108B80(DynModel *model) {
    LampData *lamp = LAMP_DATA(model);

    if (lamp->track != TRACK_NONE) {
        if ((lamp->duration -= D_800BE9E4) < 0) {
            lamp->track = TRACK_NONE;
        }
    }
}

/* Takes the lamp height from its sequence track, or the default 226. */
void func_15108BC0(DynModel *model) {
    LampData *lamp = LAMP_DATA(model);
    s32 track = lamp->track;

    if (track == TRACK_NONE) {
        lamp->height = 226.0f;
    } else if (D_800C35EA[0] == 1) {
        SeqTrackState *state = &D_800C3958[0][track];

        lamp->height = state->y;
    } else {
        lamp->height = 226.0f;
    }
}

/* Update function 1 (second lamp model): as func_15108AB4, then draws with func_15109120 while lit. */
s32 func_15108C38(DynModel *model) {
    LampData *lamp = LAMP_DATA(model);

    lamp->phaseX += 0.0857f * D_800BE9A4;
    lamp->phaseZ += 0.0783f * D_800BE9A4;
    lamp->phaseX = func_15144B68(lamp->phaseX);
    lamp->phaseZ = func_15144B68(lamp->phaseZ);
    lamp->swingX = sinf(lamp->phaseX) * 7.1050005f;
    lamp->swingZ = sinf(lamp->phaseZ) * 7.1050005f;
    func_15108B80(model);
    func_15108BC0(model);
    if (lamp->lightOn) {
        model->desc.drawFunc = 4;
    } else {
        model->desc.drawFunc = 2;
    }
    return 1;
}

/* Draw function 1: builds this frame's lamp matrix, swung, raised to its height and scaled by 0.157. */
s32 func_15108D24(DynModel *model, s32 viewport) {
    MtxF mtx;
    LampData *lamp;

    lamp = LAMP_DATA(model);
    func_150A8050(mtx, lamp->swingX, 0.0f, lamp->swingZ);
    mtx[3][1] = lamp->height;
    mtx[0][0] *= 0.157f;
    mtx[0][1] *= 0.157f;
    mtx[0][2] *= 0.157f;
    mtx[1][0] *= 0.157f;
    mtx[1][1] *= 0.157f;
    mtx[1][2] *= 0.157f;
    mtx[2][0] *= 0.157f;
    mtx[2][1] *= 0.157f;
    mtx[2][2] *= 0.157f;
    guMtxF2L(mtx, &model->mtx[D_800BE9C0]);
    return 1;
}

/*
 * Update for the lamp's light effect (owner model stored at effect->data.object): moves the light to where a
 * ray from the lamp down along its swing hits the ground, and switches it with the lamp's light state.
 */
s32 func_15108E10(LightEffect *effect) {
    s32 pad; // matching: unused stack slot
    DynModel *model;
    LampData *lamp;
    Vec3f end;
    MtxF mtx;
    Vec3f pad2; // matching: unused stack slots
    Vec3f hit;

    model = effect->data.object;
    lamp = LAMP_DATA(model);
    func_150A8050(mtx, lamp->swingX, 0.0f, lamp->swingZ);
    mtx[3][1] = lamp->height;
    mtx[0][0] *= 0.157f;
    mtx[0][1] *= 0.157f;
    mtx[0][2] *= 0.157f;
    mtx[1][0] *= 0.157f;
    mtx[1][1] *= 0.157f;
    mtx[1][2] *= 0.157f;
    mtx[2][0] *= 0.157f;
    mtx[2][1] *= 0.157f;
    mtx[2][2] *= 0.157f;
    func_150A7960(mtx, 0.0f, -1108.0f, 0.0f, &end.x, &end.y, &end.z);
    {
        f32 dy = end.y - lamp->height;

        if (func_150AC9C0(0.0f, lamp->height, 0.0f, end.x, dy, end.z, NULL, NULL, &hit.x, &hit.y, &hit.z, NULL,
                          NULL, NULL, 0.0f) == 0) {
            hit = end;
        }
    }
    effect->light->x = hit.x;
    effect->light->y = hit.y;
    effect->light->z = hit.z;
    if (lamp->lightOn) {
        effect->light->disabled = 0;
    } else {
        effect->light->disabled = 1;
    }
    return 1;
}

extern IdList2 D_80088C50;
extern IdList2 D_80088C58;

/* Sends message 0x1D: makes the lamps follow `track` for `duration`. */
void func_15108FFC(s32 track, s32 duration, u8 sceneTag) {
    LampTrackMsg msg;
    IdList2 target;

    target = D_80088C50;
    msg.track = track;
    msg.duration = duration;
    msg.sceneTag = sceneTag;
    func_15169260(&target, 2, (s32)&msg, 0x1D);
}

/* Message handler: 0x1D sets the track to follow, 0x1E toggles the light. */
void func_15109064(DynModel *model, LampTrackMsg *msg, u8 event) {
    LampData *lamp = LAMP_DATA(model);

    switch (event) {
        case 0x1D:
            lamp->track = msg->track;
            lamp->sceneTag = msg->sceneTag;
            lamp->duration = msg->duration;
            // matching: this label ends the basic block, keeping the last store out of the jr delay slot
        end_track:
            break;
        case 0x1E:
            if (lamp->lightOn) {
                lamp->lightOn = FALSE;
            } else {
                lamp->lightOn = TRUE;
            }
            break;
    }
}

/* Sends message 0x1E: toggles the lamp light. */
void func_151090DC(void) {
    IdList2 target;

    target = D_80088C58;
    func_15169260(&target, 2, 0, 0x1E);
}

/*
 * Draw function 4: builds the lamp matrix and recomputes the texture coordinates of the 16 vertices of
 * `viewport`'s buffer, projected on the horizontal axis perpendicular to the camera.
 */
s32 func_15109120(DynModel *model, s16 viewport) {
    MtxF mtx;
    LampData *lamp;
    Vec3f *camera;
    f32 dirX;
    f32 dirZ;
    f32 x;
    f32 camX;
    f32 z;
    f32 s;
    f32 t;
    f32 y;
    f32 outZ;
    f32 outY;
    f32 outX;
    s32 i;

    lamp = LAMP_DATA(model);
    func_150A8050(mtx, lamp->swingX, 0.0f, lamp->swingZ);
    mtx[3][1] = lamp->height;
    mtx[0][0] *= 0.157f;
    mtx[0][1] *= 0.157f;
    mtx[0][2] *= 0.157f;
    mtx[1][0] *= 0.157f;
    mtx[1][1] *= 0.157f;
    mtx[1][2] *= 0.157f;
    mtx[2][0] *= 0.157f;
    mtx[2][1] *= 0.157f;
    mtx[2][2] *= 0.157f;
    guMtxF2L(mtx, &model->mtx[D_800BE9C0]);
    camera = func_15144B34(viewport);
    camX = camera->x;
    if (0.0001f < fabsf(camX) || 0.0001f < fabsf(camera->y)) {
        f32 camZ;
        f32 invLen;

        camZ = camera->z;
        invLen = 1.0f / sqrtf(camX * camX + camZ * camZ);
        dirX = camZ * invLen;
        dirZ = -camX * invLen;
    } else {
        dirX = 1.0f;
        dirZ = 0.0f;
    }
    for (i = 0; i != 16; i++) {
        Vtx *vtx;

        vtx = &model->vertices[D_800BE9C0][viewport][i];
        x = vtx->v.ob[0];
        y = vtx->v.ob[1];
        z = vtx->v.ob[2];
        func_150A7960(mtx, x, y, z, &outX, &outY, &outZ);
        t = outY * 0.358f + 81.0f;
        s = (outX * dirX + outZ * dirZ) * 0.21300001f + 26.0f;
        vtx->v.tc[0] = s * 32.0f;
        vtx->v.tc[1] = t * 32.0f;
    }
    return 1;
}
