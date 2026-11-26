#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Updates a trail: sways the emission point around the actor, appends one record per whole step of
 * state->count (spawning particles at the actor on the way), and when state->timer runs out fades the
 * newest records and deactivates the trail. Always returns 1.
 */
s32 func_150DCEA0(StreamActor *actor) {
    StreamState *state;
    StreamPoint *records;
    u8 pitch;
    s32 index;
    f32 stepScale;
    f32 inverse;
    f32 scale;
    f32 x;
    f32 z;
    f32 dx;
    f32 dz;
    f32 speed;

    state = actor->state;
    records = actor->records;
    state->angleX += (func_150ADA20() % (u32)(state->angleXRange + 1)) * D_800BE9E4;
    state->angleZ += (func_150ADA20() % (u32)(state->angleZRange + 1)) * D_800BE9E4;
    state->count += state->rate * D_800BE9A4;
    if (state->count > 0.99999f) {
        inverse = 1.0f / state->count;
        {
            BillboardTemplate packet;
            f32 stepX;
            f32 stepZ;
            f32 sinYaw;
            f32 cosYaw;
            f32 sinPitch;
            f32 cosPitch;
            u8 yaw;
            s32 spawned;

            scale = D_800BE9A4;
            stepScale = D_800BE9A4 * inverse;
            x = state->x;
            z = state->z;
            dx = func_151423D8((u8)(state->angleX - 0x40)) * state->swayX + actor->head.tmpl.pos.x - state->x;
            dz = func_151423D8((u8)(state->angleZ - 0x40)) * state->swayZ + actor->head.tmpl.pos.z - state->z;
            if (state->count > 1.0f) {
                stepX = dx * inverse;
                stepZ = dz * inverse;
                do {
                    state->spawnCount += state->spawnRate;
                    if (state->spawnCount > 1.0f) {
                        packet.subX = 0;
                        packet.subY = 0;
                        packet.subZ = 0;
                        packet.primR = 0xFF;
                        packet.primG = 0xFF;
                        packet.primB = 0xFF;
                        packet.envR = 0;
                        packet.envG = 0;
                        packet.envB = 0;
                        packet.primA = 0xFF;
                        packet.callback = state->callback;
                        packet.flags = 0x12;
                        packet.frame = 0;
                        spawned = TRUE;
                        packet.frameStep = state->frameRate;
                        packet.anim = state->texture;
                        packet.y = actor->head.tmpl.pos.y;
                        packet.life = state->lifetime;
                        do {
                            speed = func_150ADA68() * state->speedRange + state->speedBase;
                            yaw = func_150ADA20();
                            sinYaw = func_151423D8(yaw);
                            cosYaw = func_151423D8((u8)(yaw - 0x40));
                            pitch = (func_150ADA20() % (u32)(state->pitchRange + 1)) + state->pitchBase;
                            sinPitch = func_151423D8(pitch);
                            cosPitch = func_151423D8((u8)(pitch - 0x40));
                            packet.x = actor->head.tmpl.pos.x + state->radius * cosYaw;
                            packet.z = actor->head.tmpl.pos.z + state->radius * sinYaw;
                            packet.velX = speed * sinPitch * cosYaw;
                            packet.velY = -speed * cosPitch;
                            packet.velZ = speed * sinPitch * sinYaw;
                            packet.accelY = func_150ADA68() * state->accelRange + state->accelBase;
                            packet.fadeStep = (func_150ADA20() % (u32)(state->fadeRange + 1)) + state->fadeBase;
                            packet.width = packet.height = func_150ADA68() * state->sizeRange + state->sizeBase;
                            if (func_15167D84(&packet, 0, 0, -1, actor->head.header.owner, actor->head.header.kind) == NULL) {
                                spawned = FALSE;
                            }
                            state->spawnCount -= 1.0f;
                        } while (state->spawnCount > 1.0f && spawned);
                        while (state->spawnCount > 1.0f) {
                            state->spawnCount -= 1.0f;
                        }
                    }
                    records[actor->head.end].pos.x = x;
                    records[actor->head.end].pos.y = actor->head.tmpl.pos.y;
                    records[actor->head.end].pos.z = z;
                    records[actor->head.end].velocityY = state->velocityY;
                    state->widthAngle += func_150ADA20() % (u32)(state->widthAngleRange + 1);
                    records[actor->head.end].width = func_151423D8((u8)(state->widthAngle - 0x40)) * state->widthRange + state->widthBase;
                    records[actor->head.end].alpha = state->alpha;
                    records[actor->head.end].texCoord = state->texCoord;
                    state->texCoord += state->texCoordStep + (func_150ADA20() % (u32)(state->texCoordStepRange + 1));
                    records[actor->head.end].velocityY += state->gravity * scale;
                    records[actor->head.end].pos.y += records[actor->head.end].velocityY * scale;
                    actor->head.end++;
                    if (actor->head.end == actor->head.tmpl.itemCount) {
                        actor->head.end = 0;
                    }
                    actor->head.count++;
                    if (actor->head.end == actor->head.start) {
                        actor->head.start++;
                        if (actor->head.start == actor->head.tmpl.itemCount) {
                            actor->head.start = 0;
                        }
                        actor->head.count--;
                    }
                    x += stepX;
                    z += stepZ;
                    scale -= stepScale;
                    state->count -= 1.0f;
                } while (state->count > 1.0f);
            }
        }
        state->x = x;
        state->z = z;
    }
    state->timer -= D_800BE9E4;
    if (state->timer < 0) {
        index = actor->head.end - 1;
        if (index < 0) {
            index = actor->head.tmpl.itemCount - 1;
        }
        /* fade out the newest four records */
        records[index].alpha = 0;
        index--;
        if (index < 0) {
            index = actor->head.tmpl.itemCount - 1;
        }
        records[index].alpha = 0x3F;
        index--;
        if (index < 0) {
            index = actor->head.tmpl.itemCount - 1;
        }
        records[index].alpha = 0x7F;
        index--;
        if (index < 0) {
            index = actor->head.tmpl.itemCount - 1;
        }
        records[index].alpha = 0xBE;
        actor->head.updateKind2 = 0;
        actor->head.tmpl.flags &= ~2;
    }
    return 1;
}

typedef struct {
    s32 unk0;
    s32 unk4;
} Mode150DD640;

extern EffectTexture D_80090B60[];
extern Mode150DD640 D_800A4AC8[];
void *memcpy(void *, const void *, u32);

// matching: the original branches on the axis index around two identical stores
#define SET_TEXCOORD(vtx, axis, value) \
    if (axis) {                        \
        (vtx)->v.tc[axis] = (value);   \
    } else {                           \
        (vtx)->v.tc[axis] = (value);   \
    }

/*
 * Draws the trail as a flat ribbon from the newest record back to actor->head.start, two vertices per record
 * spread sideways by the view's camera sin/cos and the record's width, with the texture running along the
 * ribbon (S, or T when state->unk60 == 2).
 */
Gfx *func_150DD640(StreamActor *actor, Gfx *gfx, s16 view) {
    Vtx *vtx;
    f32 cosYaw;
    Vec3f older;
    Vec3f newer;
    s32 j;
    s32 i;
    f32 sinYaw;
    u16 texCoord;
    u8 tcAxis[2];
    u8 padB; // matching: stack slot after tcAxis
    u8 prevTexCoord;
    u8 update;
    u8 mode;
    u8 unk42High;
    StreamState *state;
    StreamPoint *records;

    if (actor->head.count >= 2) {
        func_151D5D60(actor->unk84, view, (actor->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            state = actor->state;
            records = actor->records;
            update = 1;
            mode = state->unk42 & 0xF;
            sinYaw = D_800DD1D8[view];
            unk42High = (state->unk42 & 0xF0) >> 4;
            cosYaw = D_800DD1E8[view];
            if (state->unk60 == 2) {
                tcAxis[0] = 1;
                tcAxis[1] = 0;
            } else {
                tcAxis[0] = 0;
                tcAxis[1] = 1;
            }
            gfx = func_15142E24(gfx, D_80090B60 + state->trailTexture, 0, 0, 0, 0, state->trailTexture, 0, 0, &update, 3);
            gfx = func_15142C10(gfx, state->unk28, state->unk28, state->unk28, 0xFF, &update);
            gfx = func_1513F4E4(gfx, unk42High, &update);
            gfx = func_15142B7C(gfx, 1, 0x160600);
            gfx = func_15142FBC(gfx, D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[mode].unk0 | D_800A4AC8[mode].unk4, &update);
            if (actor->head.tmpl.flags & 2) {
                /* still emitting: the ribbon starts at the actor itself (i is left unset, as in the original) */
                j = actor->head.end - 1;
                if (j < 0) {
                    j = actor->head.tmpl.itemCount - 1;
                }
                newer = actor->head.tmpl.pos;
                older = records[j].pos;
                prevTexCoord = texCoord = records[j].texCoord;
            } else {
                i = actor->head.end - 1;
                if (i < 0) {
                    i = actor->head.tmpl.itemCount - 1;
                }
                j = i - 1;
                if (j < 0) {
                    j = actor->head.tmpl.itemCount - 1;
                }
                newer = records[i].pos;
                older = records[j].pos;
                prevTexCoord = records[i].texCoord;
                texCoord = records[j].texCoord;
            }
            vtx->v.ob[0] = records[j].width * cosYaw + newer.x;
            vtx->v.ob[1] = newer.y;
            vtx->v.ob[2] = newer.z - records[j].width * sinYaw;
            SET_TEXCOORD(vtx, tcAxis[0], prevTexCoord << 6);
            vtx->v.tc[tcAxis[1]] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = records[i].alpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = newer.x - records[j].width * cosYaw;
            vtx->v.ob[1] = newer.y;
            vtx->v.ob[2] = records[j].width * sinYaw + newer.z;
            SET_TEXCOORD(vtx, tcAxis[0], prevTexCoord << 6);
            SET_TEXCOORD(vtx, tcAxis[1], (D_80090B60[state->trailTexture].width - 1) << 6);
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = records[i].alpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                vtx->v.ob[0] = records[j].width * cosYaw + older.x;
                vtx->v.ob[1] = older.y;
                vtx->v.ob[2] = older.z - records[j].width * sinYaw;
                SET_TEXCOORD(vtx, tcAxis[0], texCoord << 6);
                vtx->v.tc[tcAxis[1]] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = records[j].alpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = older.x - records[j].width * cosYaw;
                vtx->v.ob[1] = older.y;
                vtx->v.ob[2] = records[j].width * sinYaw + older.z;
                SET_TEXCOORD(vtx, tcAxis[0], texCoord << 6);
                SET_TEXCOORD(vtx, tcAxis[1], (D_80090B60[state->trailTexture].width - 1) << 6);
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = records[j].alpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                if (prevTexCoord < texCoord) {
                    /* the u8 texture coordinate wrapped: repeat this pair with S/T pulled back 256 texels */
                    memcpy(vtx, vtx - 2, 2 * sizeof(Vtx));
                    vtx[-2].v.tc[tcAxis[0]] -= 0x4000;
                    vtx++;
                    vtx[-2].v.tc[tcAxis[0]] -= 0x4000;
                    vtx++;
                }
                prevTexCoord = texCoord;
                i = j;
                j--;
                if (j < 0) {
                    j = actor->head.tmpl.itemCount - 1;
                }
                newer = records[i].pos;
                older = records[j].pos;
                texCoord = records[j].texCoord;
            } while (i != actor->head.start);
        }
    }
    return gfx;
}
