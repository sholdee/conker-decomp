#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern CollisionHit D_800D9860; /* the floor under the last spawn point */

extern RenderModePair D_800A4AC8[];

/* Draws the ribbon from its newest point back to its oldest, one quad per segment. */
Gfx *func_151CF380(RibbonActor *ribbon, Gfx *gfx, s16 cameraIdx) {
    RibbonInfo *info;
    Vec3f nextPos;
    Vec3f curPos;
    s32 cur;
    s32 next;
    RibbonActorPoint *points;
    u8 update;
    f32 curWidth;
    f32 curX;
    f32 curZ;
    f32 nextWidth;
    f32 nextX;
    f32 nextZ;
    TrailVec2 nextOffset;
    TrailVec2 curOffset;
    Vtx *vtx;

    if (ribbon->head.count < 2) {
        return gfx;
    }
    info = ribbon->info;
    points = ribbon->points;
    if (info->owner != NULL && info->owner->ob_matrix_ptr == NULL) {
        return gfx;
    }
    func_151D5D60(ribbon->vtxBuffer, cameraIdx, (ribbon->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, NULL);
    update = 1;
    gfx = func_15142B7C(func_15142C10(gfx, 0xED, 0xD2, 0x85, 0xC8, &update), 0x200005, 0x1F0600);
    gfx = func_15142FBC(func_1513F4E4(gfx, 0x54, &update), D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[1].cycle1 | D_800A4AC8[1].cycle2, &update);
    cur = ribbon->head.end - 1;
    if (cur < 0) {
        cur = ribbon->head.tmpl.itemCount - 1;
    }
    next = cur - 1;
    if (next < 0) {
        next = ribbon->head.tmpl.itemCount - 1;
    }
    curPos = points[cur].pos;
    curWidth = points[cur].width;
    curX = points[cur].dirX * curWidth;
    curZ = points[cur].dirZ * curWidth;
    curOffset = points[cur].offset;
    nextPos = points[next].pos;
    nextWidth = points[next].width;
    nextX = points[next].dirX * nextWidth;
    nextZ = points[next].dirZ * nextWidth;
    nextOffset = points[next].offset;
    vtx->v.ob[0] = (s32)(curPos.x + curOffset.x + curX);
    vtx->v.ob[1] = (s32)curPos.y;
    vtx->v.ob[2] = (s32)(curPos.z + curOffset.z + curZ);
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = (s32)(curPos.x - curOffset.x + curX);
    vtx->v.ob[1] = (s32)curPos.y;
    vtx->v.ob[2] = (s32)(curPos.z - curOffset.z + curZ);
    vtx->v.flag = 0;
    vtx++;
    do {
        vtx->v.ob[0] = (s32)(nextPos.x + nextOffset.x + nextX);
        vtx->v.ob[1] = (s32)nextPos.y;
        vtx->v.ob[2] = (s32)(nextPos.z + nextOffset.z + nextZ);
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = (s32)(nextPos.x - nextOffset.x + nextX);
        vtx->v.ob[1] = (s32)nextPos.y;
        vtx->v.ob[2] = (s32)(nextPos.z - nextOffset.z + nextZ);
        vtx->v.flag = 0;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP2Triangles(gfx++, 0, 1, 2, 0, 1, 3, 2, 0);
        cur = next;
        next--;
        if (next < 0) {
            next = ribbon->head.tmpl.itemCount - 1;
        }
        if (cur != ribbon->head.start) {
            curPos = points[cur].pos;
            // matching: dead recompute of curWidth/curX/curZ (DCE'd) steers the address base IDO keeps
            curWidth = points[cur].width;
            curX = points[cur].dirX * curWidth;
            curZ = points[cur].dirZ * curWidth;
            nextPos = points[next].pos;
            nextWidth = points[next].width;
            nextX = points[next].dirX * nextWidth;
            nextZ = points[next].dirZ * nextWidth;
            nextOffset = points[next].offset;
            curOffset = points[cur].offset;
        }
    } while (cur != ribbon->head.start);
    return gfx;
}

/* Message handler of a following actor: lets func_15169850 update or end the link to its owner. */
void func_151CF844(TrackerTrail *actor, s32 arg1, u8 arg2) {
    TrackerTrailData *data = actor->state;

    if (data->owner != NULL) {
        // the owner pointer and the byte after it are handled like an actor reference
        func_15169850(arg1, arg2, (Actor **)&data->owner, (u8 *)&data->texture, actor);
    }
}

/* Data of the ground effect spawned by func_151CF898. */
typedef struct {
    Vec3f pos;
    f32 unkC;
} GroundEffectData;

typedef struct {
    TimedEffect base;
    GroundEffectData data;
} GroundEffect;

/* With probability 0.222, spawns an effect on the ground at a random point up to 2000 units from
 * the camera of the object's player, within 40 degrees either side of the camera's yaw. */
void func_151CF898(Actor *obj, f32 probeY, f32 arg2) {
    Camera *player;
    Vec3f spawnPos;
    Vec3f *camPos;
    f32 yaw;
    f32 angle;
    f32 distance;
    GroundEffectData data;
    GroundEffect *effect;

    player = obj->CameraPtr;
    if (player != NULL) {
        if (func_150ADA68() < 0.2220000178f) {
            camPos = func_15144B34(player->CNumber);
            yaw = func_15144AA8(player->CNumber);
            angle = ((func_150ADA68() * 80.0f) + (yaw - 40.0f)) * 0.01745329238f;
            distance = func_150ADA68() * 2000.0f;

            spawnPos.x = camPos->x - (sinf(angle) * distance);
            spawnPos.y = probeY;
            spawnPos.z = camPos->z - (cosf(angle) * distance);

            if (func_15046C80(&spawnPos, 0, arg2, &D_800D9860) != 0) {
                spawnPos.y = D_800D9860.height;
                data.pos = spawnPos;
                data.unkC = 0.0f;

                effect = (GroundEffect *)func_15149130((s16)((func_150ADA20() % 131U) + 51), -1, 0x20, -1, 1, 0,
                                                       sizeof(data), 0xFF, 1);
                if (effect != NULL) {
                    memcpy(&effect->data, &data, sizeof(data));
                }
            }
        }
    }
}

extern Vec3f D_800AAEEC[];

/* Breaks the object apart: seven model pieces fly off its model's points with random velocity and
 * spin, plus three more effects at 200 units above it. */
void func_151CFA4C(Actor *obj, u8 arg1, s32 arg2) {
    Vec3f vecB;
    Vec3f pos;
    Vec3f vecA;
    Vec3f vecC;
    u8 i;

    pos.x = obj->xpos;
    pos.y = obj->ypos + 200.0f;
    pos.z = obj->zpos;
    if (obj->ob_matrix_ptr) {
        // matching: the matrix goes through s32; passing the pointer directly changes the temp registers
        func_1503F404((Mtx *)(s32)obj->ob_matrix_ptr, &vecA.x, &vecA.y, &vecA.z, &vecB.x, &vecB.y, &vecB.z,
                      &vecC.x, &vecC.y, &vecC.z);
        {
            ParticleSpawn78 piece;
            s32 models[7] = { 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75 };

            piece.radius = 1.0f;
            piece.elasticity = 0.6490000486f;
            piece.sizeXZ = piece.sizeY = (obj->scale_xz + obj->scale_y) * 0.5f;
            piece.rot = vecB;
            piece.scale.x = 1.0f;
            piece.scale.y = 0.0f;
            piece.scale.z = 1.0f;
            piece.rotVel.y = 0.0f;
            piece.variant = 0;
            piece.unk5C = 0;
            piece.alpha = 0xFF;
            piece.update2Fn = 0;
            piece.floorType3Fn = 0;
            piece.wallHitFn = 0;
            piece.destroyFn = 0;
            piece.squashFn = 0;
            piece.unk6A = 1;
            piece.owner = obj;
            piece.ownerId = obj->unique_id;
            piece.fadeStart = 12;
            piece.fadeRate = 21;
            piece.flags = 0x39E9;
            piece.updateFn = 13;
            piece.matrixFn = 2;
            piece.floorHitFn = 7;
            for (i = 0; i < 7; i++) {
                u32 yaw;
                u32 pitch;

                piece.model = models[i];
                func_15143134(&D_800AAEEC[i], &piece.pos, obj->ob_matrix_ptr);
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794(yaw & 0xFF, (pitch % 25U) - 64, (func_150ADA68() * 11.44999981f) + 16.93000031f, &piece.vel);
                piece.rotVel.x = (func_150ADA68() * 19.75200081f) + -9.876000404f;
                piece.rotVel.z = (func_150ADA68() * 19.75200081f) + -9.876000404f;
                piece.gravity = (func_150ADA68() * 0.5180000067f) + -1.539000034f;
                piece.lifetime = (func_150ADA20() % 101U) + 100;
                func_15132A4C((ParticleSpawn *)&piece /* matching: an 0x78-byte view */, 3, 0xFF, 0, arg1, arg2);
            }
        }
        {
            PuffSpray84 cfg;

            cfg.pos = pos;
            cfg.sizeBase = 8.0f;
            cfg.sizeRange = 13.0f;
            cfg.growthBase = 0.8000000119f;
            cfg.growthRange = 0.400000006f;
            cfg.speedBase = 23.0f;
            cfg.speedRange = 7.0f;
            cfg.countBase = 13;
            cfg.countRange = 6;
            cfg.angles.yawBase = 0;
            cfg.angles.yawRange = 0xFF;
            cfg.angles.pitchBase = -64;
            cfg.angles.pitchRange = 50;
            cfg.itemCountBase = 3;
            cfg.itemCountRange = 2;
            cfg.lifeBase = 40;
            cfg.lifeRange = 25;
            cfg.green = 9;
            cfg.headerFlags = 1;
            cfg.red = 0x48;
            cfg.handlers[0] = 1;
            cfg.handlers[1] = 0;
            cfg.handlers[2] = 0;
            cfg.handlers[3] = 0;
            cfg.handlers[4] = 0;
            cfg.handlers[5] = 0;
            cfg.blue = 0;
            cfg.alphaBase = 150;
            cfg.alphaRange = 105;
            cfg.unk58 = 3;
            cfg.unk5C = 0xFF;
            cfg.render.unk0 = 0;
            cfg.render.setGeometryMode = 0x220005;
            cfg.render.clearGeometryMode = 0x1D0600;
            cfg.render.renderMode = 3;
            cfg.render.unk10 = 0x3B;
            cfg.render.unk14 = 0x80;
            cfg.render.unk18 = 0x20;
            cfg.render.primMode = 0;
            cfg.render.envMode = 7;
            cfg.unk80 = 10;
            cfg.unk82 = 25;
            func_15151A38((PuffSpray *)&cfg /* matching: an 0x84-byte view */, arg1, arg2);
        }
        {
            DecalSpray cfg2;

            cfg2.countBase = 15;
            cfg2.countRange = 6;
            cfg2.pos = pos;
            cfg2.angles.yawBase = 0;
            cfg2.angles.yawRange = 0xFF;
            cfg2.angles.pitchBase = -29;
            cfg2.angles.pitchRange = 40;
            cfg2.maxDistance = 375.0f;
            cfg2.sizeBase = 20.0f;
            cfg2.sizeRange = 40.0f;
            cfg2.unk28 = 120;
            cfg2.unk2A = 120;
            cfg2.kind = 2;
            func_15150D1C(&cfg2, arg1, arg2);
        }
        {
            CollisionHit floor;
            s8 flag;
            s32 pad; // matching: unused stack slot in the original frame

            func_1504715C(&floor, obj);
            flag = 2;
            func_151A9834(&pos, pos.y - 2000.0f, 200.0f, &floor, (func_150ADA20() & 3) + 7, 0, &flag, arg1, arg2);
        }
    }
}

/* Tells every effect that follows the object that it is gone (message 0x18). */
void func_151D0024(Actor *obj) {
    ActorRef msg;

    msg.actor = obj;
    msg.uniqueId = obj->unique_id;
    func_151494E0((s32)&msg, 0x18);
}

/* Spawns a strip effect of the given variant on the object. */
void func_151D0058(Actor *obj, u8 variant, u8 arg2, s32 arg3) {
    StripEffect *effect;
    StripEffectData data;

    data.owner = obj;
    data.ownerId = obj->unique_id;
    data.scale = (obj->scale_xz + obj->scale_y) * 0.5f;
    data.variant = variant;
    data.alpha = (func_150ADA20() % 56U) + 200;
    data.ownerUnk84 = obj->animseq.uh;

    effect = (StripEffect *)func_15149130(300, -1, 0x61, 4, 0, 0x31, sizeof(data), arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Ends the strip effect once its owner's unk84 has changed. */
void func_151D0128(StripEffect *effect) {
    Actor *owner = effect->data.owner;

    if (effect->data.ownerUnk84 != owner->animseq.uh) {
        effect->base.header.state = -1;
    }
}

extern Vec3f D_800AAF5C[];
extern Vec3f D_800AAF74;
extern u8 D_800AAF80[];
extern EffectTexture D_80091154; /* an entry of the effect texture table D_80090B60 */

/* Draws the strip effect: six camera-facing quads joining two of the owner's model points, pinched
 * in the middle when the points are far apart. */
Gfx *func_151D014C(Gfx *gfx, StripEffect *effect, s16 cameraIdx) {
    StripEffectData *data;
    Vtx *vtx;
    Actor *owner;
    u8 update;
    Vec3f *camPos;
    f32 midX;
    f32 midY;
    f32 midZ;
    /* Scalars, not an array: as uopt live ranges they interfere with the 1/6 constant, which stops
     * its register piece before the loop header (an array left a dead reload and an extra literal). */
    f32 sideZ;
    f32 sideY;
    f32 sideX;
    f32 taper;
    Vec3f from;
    Vec3f to;
    Vec3f diff;
    f32 stepX;
    f32 stepY;
    f32 stepZ;
    Vec3f pos;
    f32 ang;
    f32 progress;
    f32 dist;
    f32 amp;
    f32 tex;
    f32 unused;
    f32 offsetX;
    f32 offsetY;
    f32 offsetZ;
    s32 i;

    owner = effect->data.owner;
    data = &effect->data;
    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        effect->base.header.state = -1;
        return gfx;
    }
    if (owner->ob_matrix_ptr == NULL) {
        return gfx;
    }
    func_151D5D60(effect->base.buffers, cameraIdx, 0x1C0, &vtx, NULL);
    if (vtx == NULL) {
        return gfx;
    }
    update = 1;
    func_15143134(&D_800AAF5C[data->variant], &from, &owner->ob_matrix_ptr[2]);
    func_15143134(&D_800AAF74, &to, &owner->ob_matrix_ptr[D_800AAF80[data->variant]]);
    camPos = func_15144B34(cameraIdx);
    gfx = func_15142B7C(gfx, 0x200005, 0x60600);
    gfx = func_15142E24(gfx, &D_80091154, 0, 0, 0, 0, 0x7F, 0, 0, &update, 3);
    gfx = func_1513F4E4(gfx, 0x3B, &update);
    gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, data->alpha, &update);
    gfx = func_15142FBC(gfx, D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[3].cycle1 | D_800A4AC8[3].cycle2, &update);
    diff.x = to.x - from.x;
    diff.y = to.y - from.y;
    diff.z = to.z - from.z;
    dist = func_15143E64(&diff);
    midX = diff.x * 0.5f + from.x - camPos->x;
    midY = diff.y * 0.5f + from.y - camPos->y;
    midZ = diff.z * 0.5f + from.z - camPos->z;
    offsetX = diff.y * midZ - midY * diff.z;
    offsetY = diff.z * midX - midZ * diff.x;
    offsetZ = diff.x * midY - midX * diff.y;
    if (offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ == 0.0f) {
        sideZ = sideY = sideX = 0.0f;
    } else {
        amp = (35.0f * data->scale) / sqrtf(offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ);
        sideX = offsetX * amp;
        sideY = offsetY * amp;
        sideZ = offsetZ * amp;
    }
    stepX = diff.x * 0.1666666716f;
    stepY = diff.y * 0.1666666716f;
    stepZ = diff.z * 0.1666666716f;
    if (dist < 20.0f) {
        amp = 0.0f;
    } else if (dist < 70.0f) {
        amp = (dist - 20.0f) * 0.01999999955f * 0.6000000238f;
    } else {
        amp = 0.6000000238f;
    }
    // matching: one source line; as1 orders the hoisted loop constants against the copy by line number
    pos = from; progress = 0.0f; ang = 0.0f; tex = 0.0f; i = 0; do {
        taper = 1.0f - sinf(ang) * amp;
        offsetX = sideX * taper;
        vtx->v.ob[0] = (s32)(offsetX + pos.x);
        vtx->v.ob[1] = (s32)(sideY * taper + pos.y);
        vtx->v.ob[2] = (s32)(sideZ * taper + pos.z);
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = (s32)tex;
        vtx++;
        vtx->v.ob[0] = (s32)(pos.x - offsetX);
        vtx->v.ob[1] = (s32)(pos.y - sideY * taper);
        vtx->v.ob[2] = (s32)(pos.z - sideZ * taper);
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = 0x7C0;
        vtx->v.tc[1] = (s32)tex;
        vtx++;
        taper = 1.0f - sinf(ang + 0.5235987902f) * amp;
        offsetX = sideX * taper;
        vtx->v.ob[0] = (s32)(pos.x + stepX - offsetX);
        offsetY = sideY * taper;
        vtx->v.ob[1] = (s32)(pos.y + stepY - offsetY);
        offsetZ = sideZ * taper;
        vtx->v.ob[2] = (s32)(pos.z + stepZ - offsetZ);
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = 0x7C0;
        vtx->v.tc[1] = (s32)(tex + 341.3333435f);
        vtx++;
        // matching: dead store; its two variable reads shift where uopt splits the basic block
        unused = offsetX + offsetY;
        vtx->v.ob[0] = (s32)(pos.x + stepX + offsetX);
        vtx->v.ob[1] = (s32)(pos.y + stepY + offsetY);
        vtx->v.ob[2] = (s32)(pos.z + stepZ + offsetZ);
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = (s32)(tex + 341.3333435f);
        vtx++;
        i++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
        pos.x += stepX;
        pos.y += stepY;
        pos.z += stepZ;
        progress += 0.1666666716f;
        ang += 0.5235987902f;
        tex += 341.3333435f;
    } while (i != 6);
    return gfx;
}

/* Message handler of the effects that follow an owner: frees the effect when its owner is gone
 * (messages 0 and 0x18) and follows the owner through a swap (0x2D). */
void func_151D08F0(OwnedEffect *effect, void *msg, u8 type) {
    ActorRef *ref = &effect->ref;

    if (type == 0 || type == 0x18) {
        ActorRef *gone = msg;
        Actor *obj = gone->actor;

        if (obj == ref->actor || gone->uniqueId == ref->uniqueId) {
            func_1516972C(&effect->base.header);
        }
    } else if (type == 0x2D) {
        ActorSwapMsg *swap = msg;

        if (ref->actor == swap->actorA) {
            ref->actor = swap->actorB;
            ref->uniqueId = swap->idB;
        } else if (ref->actor == swap->actorB) {
            ref->actor = swap->actorA;
            ref->uniqueId = swap->idA;
        }
    }
}

extern Vec3f D_800AAF84;
extern Vec3f D_800AAF90;
/* Emits two particle bursts along the line between two of the object's model points, and sometimes
 * 5-10 more particles that each carry a func_151CEAAC effect. */
void func_151D09A8(Actor *obj, u8 arg1, s32 arg2) {
    Vec3f pos;
    Vec3f dir;
    CollisionHit floor;
    ConeBasis shape;

    if (obj->ob_matrix_ptr != NULL) {
        {
            const Vec3f *srcs[2];
            Vec3f *dsts[2];

            srcs[0] = &D_800AAF84;
            srcs[1] = &D_800AAF90;
            dsts[0] = &pos;
            dsts[1] = &dir;
            func_15145EA4(srcs, dsts, &obj->ob_matrix_ptr[2], 2);
            dir.x -= pos.x;
            dir.y -= pos.y;
            dir.z -= pos.z;
        }
        func_1504715C(&floor, obj);
        shape.flags = 0;
        shape.direction = dir;
        /* Start from the zero vector D_800A5480. */
        shape.axis0 = shape.axis1 = D_800A5480;
        {
            DropSpray emitA;

            emitA.pos = pos;
            emitA.countBase = 6;
            emitA.countRange = 8;
            emitA.speedBase = 0.03999999911f;
            emitA.speedRange = 0.02999999933f;
            emitA.lifeBase = 30;
            emitA.lifeRange = 30;
            emitA.gravityBase = -1.467000127f;
            emitA.gravityRange = 0.756000042f;
            emitA.unk24 = 155;
            emitA.unk25 = 100;
            emitA.sizeBase = 403.0f;
            emitA.sizeRange = 455.0f;
            emitA.unk30 = 1;
            emitA.kind = 2;
            emitA.unk34 = 0.0f;
            emitA.wobbles = 1;
            emitA.bounceChance = 0.0f;
            shape.spread = 750.0f;
            func_1514FF44(&shape, &emitA, &floor, arg1, arg2);
        }
        {
            SplashSpray emitB;

            emitB.pos = pos;
            emitB.scaleBase = 10.0f;
            emitB.scaleRange = 25.0f;
            emitB.sizeBase = 1.253000021f;
            emitB.sizeRange = 0.8230000138f;
            emitB.speedBase = 0.03500000015f;
            emitB.speedRange = 0.03199999779f;
            emitB.countBase = 15;
            emitB.countRange = 8;
            emitB.itemCountBase = 3;
            emitB.itemCountRange = 2;
            emitB.lifeBase = 30;
            emitB.lifeRange = 25;
            emitB.unk30 = 155;
            emitB.unk32 = 100;
            emitB.unk38 = 0.0f;
            emitB.unk3C = 16;
            emitB.unk3E = 15;
            emitB.unk40 = 0;
            emitB.kind = 2;
            shape.spread = 750.0f;
            func_15153CCC(&shape, &emitB, &floor, arg1, arg2);
        }
        if (func_1514F6E8(&shape) != 0) {
            s32 count;
            ParticleDesc spawn;
            CarrierParticle *particle;
            s32 pad; // matching: unused stack slot in the original frame

            shape.spread = 1000.0f;
            count = (func_150ADA20() % 6U) + 5;
            spawn.texture = 0x3E;
            spawn.renderMode = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.r = 0;
            spawn.g = 0;
            spawn.b = 0;
            spawn.a = 0xFF;
            spawn.r2 = 0;
            spawn.g2 = 0;
            spawn.b2 = 0;
            spawn.alpha = 0;
            spawn.brightness = 0xFF;
            spawn.pos = pos;
            spawn.offset = D_800A5480;
            spawn.scaleStart = 1;
            spawn.flags = 0xC007;
            spawn.scaleRate = 1.0f;
            spawn.timer = (func_150ADA20() % 11U) + 40;
            spawn.fadeStart = 1;
            spawn.fadeRate = 0xFF;
            spawn.primColorMode = 6;
            spawn.envColorMode = 8;
            spawn.drawFunc = -1;
            spawn.updateFunc = -1;
            spawn.deathFunc = -1;
            spawn.handlerSet = 11;
            spawn.viewMask = 0;
            spawn.views = 0xFF;
            spawn.minSize = 0;
            spawn.height = 0.0f;
            spawn.width = 0.0f;
            spawn.maxSize = 10000.0f;
            while (count != 0) {
                func_1514F808(&shape, (func_150ADA68() * 0.03999999911f) + 0.04999999702f, &spawn.vel);
                spawn.gravity = (func_150ADA68() * 1.340000033f) + -2.328999996f;
                particle = func_15130280(&spawn, 1, 0, 4, arg1, arg2);
                if (particle != NULL) {
                    particle->effect = func_151CEAAC(NULL, &particle->head, 0, arg1, arg2);
                }
                count--;
            }
        }
    }
}

/* Frees the effect that travels with the particle. */
void func_151D0ED8(CarrierParticle *particle) {
    if (particle->effect != NULL) {
        // matching: a volatile read makes IDO reload the pointer for the call, as the original does
        func_1516972C(*(EffectHeader *volatile *)&particle->effect);
    }
}

/* Particle handler: frees the attached effect, then runs func_1513173C. */
void func_151D0F08(CarrierParticle *particle) {
    func_151D0ED8(particle);
    func_1513173C(&particle->head);
}

/* Particle handler: frees the attached effect, then runs func_1513175C. */
void func_151D0F34(CarrierParticle *particle) {
    func_151D0ED8(particle);
    func_1513175C(&particle->head);
}

/* Point functions: each writes a point of the object to out and returns whether it has one. */
extern s32 (*D_8008FC30[])(Actor *obj, Vec3f *out);

/* Spawns a tracker effect that follows the object's point D_8008FC30[pointFn]. */
void func_151D0F60(Actor *obj, u8 pointFn, u8 arg2, s32 arg3) {
    TrackerEffect *effect;
    TrackerData data;

    data.owner = obj;
    data.ownerId = obj->unique_id;
    data.trail = NULL;

    if (D_8008FC30[pointFn](obj, &data.pos) == 0) {
        data.pos = D_800A5480;
    }

    data.prevPos = data.pos;
    data.delta = D_800A5480;
    data.dist = 0.0f;
    data.pointFn = pointFn;

    effect = (TrackerEffect *)func_15149130(300, -1, 0x5F, -1, 0, 0x48, sizeof(data), arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Point function: 8 units above the object's unk180 height, at its x/z. */
s32 func_151D1074(Actor *obj, Vec3f *out) {
    out->x = obj->xpos;
    out->y = obj->floorY + 8.0f;
    out->z = obj->zpos;
    return TRUE;
}

/* Point function: the object's model point 0. */
s32 func_151D10A4(Actor *obj, Vec3f *out) {
    return func_151D10E4(obj, out, 0);
}

/* Point function: the object's model point 1. */
s32 func_151D10C4(Actor *obj, Vec3f *out) {
    return func_151D10E4(obj, out, 1);
}

extern Vec3f D_800AAF9C[];

/* Writes model point D_800AAF9C[which] of the object, in world space, to out. */
s32 func_151D10E4(Actor *obj, Vec3f *out, u8 which) {
    s32 joints = (s32)obj->ob_matrix_ptr;

    if (joints == 0) {
        return FALSE;
    }
    // matching: "| 0" keeps joints from being a plain copy, so IDO gives it v0 and which a3
    func_15143134(&D_800AAF9C[which], out, (Mtx *)(joints | 0));
    return TRUE;
}

/* Tracker update: follows the owner's point, and drops the trail when the point is lost, jumps more
 * than 300 units, stops, or the owner is airborne, in water or in state 5, 6, 9, 13 or 14. */
void func_151D1138(TrackerEffect *effect) {
    TrackerData *data;
    Actor *owner;
    u8 finished = FALSE;
    s32 state;

    data = &effect->data;
    owner = data->owner;

    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        effect->base.header.state = -1;
        return;
    }

    data->prevPos = data->pos;
    if (D_8008FC30[data->pointFn](owner, &data->pos) == 0) {
        data->pos = data->prevPos;
        finished = TRUE;
    }

    data->delta.x = data->pos.x - data->prevPos.x;
    data->delta.y = data->pos.y - data->prevPos.y;
    data->delta.z = data->pos.z - data->prevPos.z;
    data->dist = func_15143E64(&data->delta);

    state = func_1510F8CC(owner->floorMaterial);
    if (data->dist > 300.0f || data->dist <= 0.0f || owner->shad != 0.0f || owner->inwater != 0 ||
        state == 5 || state == 6 || state == 9 || state == 13 || state == 14) {
        finished = TRUE;
    }

    if (finished) {
        func_151D13E0(effect);
    } else if (data->trail == NULL) {
        func_151D1448(effect);
    }
}

/* Message handler of the tracker effect: lets func_15169850 update or end the link to its owner. */
void func_151D1328(TrackerEffect *effect, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

/* Tracker teardown: lets go of the trail. */
void func_151D1368(TrackerEffect *effect) {
    func_151D13E0(effect);
}

void func_151D1388(TrackerEffect *effect) {
    func_151D1368(effect);
    func_1514933C(&effect->base);
}

void func_151D13B4(TrackerEffect *effect) {
    func_151D1368(effect);
    func_15149368(&effect->base);
}

/* Lets go of the tracker's trail, which then fades out on its own. */
void func_151D13E0(TrackerEffect *effect) {
    TrackerData *data = &effect->data;
    TrackerTrailData *trailData;

    if (data->trail != NULL) {
        trailData = data->trail->state;
        data->trail->head.updateKind2 = 0;
        data->trail->head.tmpl.flags &= ~0x2;
        data->trail->head.tmpl.flags |= 0x8;
        data->trail->head.tmpl.flags |= 0x1;
        data->trail->head.tmpl.life = 40;
        trailData->owner = NULL;
        data->trail = NULL;
    }
}

/* Spawns the trail that follows the tracker effect. */
void func_151D1448(TrackerEffect *effect) {
    TrackerData *data;
    struct {
        EffectTemplate spawn;
        TrackerTrailData data;
    } req;
    Actor *owner;
    TrackerTrail *trail;

    data = &effect->data;
    owner = data->owner;
    req.data.owner = effect;
    req.data.alpha = 0xFF;
    req.data.texture = 0.0f;
    if (owner->character_type == 0x25) {
        req.data.maxLength = 800.0f;
        req.data.widthBase = 36.0f;
        req.data.widthScale = 50.0f;
        req.data.alphaScale = 200.0f;
    } else {
        if (D_80082FA0 > 0) {
            req.data.maxLength = 260.0f;
        } else {
            req.data.maxLength = 500.0f;
        }
        req.data.widthBase = 40.0f;
        req.data.widthScale = 40.0f;
        req.data.alphaScale = 180.0f;
    }
    req.data.ownerScale = data->owner->scale_xz;
    req.spawn.itemCount = 25;
    req.spawn.pos = data->pos;
    req.spawn.life = 300;
    req.spawn.flags = 54;
    req.spawn.kind = 19;
    req.spawn.unk14 = 6;
    req.spawn.unk18 = 0;
    trail = func_15147A80(&req.spawn, sizeof(TrackerTrailData), 0x28, 0, 0x11, 0x12, 0, 0, 0, effect->base.header.owner, effect->base.header.kind);
    if (trail != NULL) {
        memcpy(trail->state, &req.data, sizeof(TrackerTrailData));
        data->trail = trail;
    }
}
