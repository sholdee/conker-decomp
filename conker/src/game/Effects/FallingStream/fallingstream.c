#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A falling stream: a ring buffer of points that fall under gravity and, where they reach the
 * floor, splash (splash and drop particles plus flat ripple sprites) and play a sound. The stream
 * is drawn as a textured ribbon of its records (effect_updateFallingStreamTrail updates it, effect_drawFallingStream draws it).
 */

void *memcpy(void *, const void *, u32);

/*
 * Creates falling stream `kind` (0 and 5: single streams; 1-4: four streams at (+-650, 736, +-650)
 * that need character 6, whose unk84 picks the floor height) and copies its tuning into the actor's
 * state. Returns the actor, or 0 on failure.
 */
s32 effect_createFallingStream(u8 kind) {
    EffectTemplate spawn;
    StreamState state;
    StreamActor *actor;

    state.count = 0.0f;
    state.texCoord = 0;
    spawn.flags = 0;
    state.widthAngle = 0;
    switch (kind) {
    case 0:
    case 5:
        switch (kind) {
        case 0:
            spawn.pos.x = 5.0f;
            spawn.pos.y = 115.0f;
            spawn.pos.z = -1938.0f;
            state.floor = 40.4f;
            break;
        case 5:
            spawn.pos.x = -693.79346f;
            spawn.pos.y = 205.64001f;
            spawn.pos.z = -1944.8138f;
            state.floor = 90.0f;
            break;
        }
        state.x = spawn.pos.x;
        state.z = spawn.pos.z;
        state.gravity = -1.6f;
        state.rate = 1.67f;
        state.widthBase = 14.0f;
        state.widthRange = 4.0f;
        state.swayZ = 3.6f;
        state.radius = 45.19f;
        state.speedBase = 40.0f;
        state.accelRange = -185.0f;
        state.sizeRange = 385.0f;
        state.splashRate = 0.225f;
        state.splashSpeedBase = 5.95f;
        state.splashSpeedRange = 5.63f;
        state.splashScaleBase = 5.0f;
        state.splashSizeBase = 1.1f;
        spawn.itemCount = 25;
        spawn.life = 133;
        state.timer = 120;
        state.unk28 = 255;
        state.alpha = 255;
        state.widthAngleRange = 12;
        state.texCoordStep = 2;
        state.texCoordStepRange = 2;
        state.splashSizeRange = 0.42f;
        state.angleXRange = 10;
        state.angleZRange = 5;
        spawn.flags |= 1;
        state.trailTexture = 24;
        state.unk42 = 81;
        state.unk44 = 8;
        state.unk58 = 5;
        state.unk60 = 1;
        state.pitchBase = 63;
        state.lifetime = 56;
        state.dropRate = 1.48f;
        state.accelBase = -128;
        state.fadeBase = 8;
        state.fadeRange = 10;
        state.sizeBase = 385;
        state.callback = -1;
        state.splashPitchBase = -63;
        state.splashPitchRange = 43;
        state.splashLifetime = 86;
        state.splashCountBase = 3;
        state.splashCountRange = 3;
        state.velocityY = 0.0f;
        state.angleX = 0;
        state.angleZ = 0;
        state.unk48 = 0;
        state.unk4C = 0;
        state.unk50 = 0;
        state.unk54 = 0;
        state.unk5C = 0;
        state.spawnRate = 0.0f;
        state.spawnCount = 0.0f;
        state.pitchRange = 0;
        state.texture = &D_800903C8;
        state.frameRate = 0;
        state.splashCount = 0.0f;
        state.unkCA = 12;
        state.dropCount = 0.0f;
        state.dropPitchBase = -62;
        state.dropPitchRange = 7;
        state.dropLifetime = 56;
        state.dropAccelBase = -57;
        state.dropAccelRange = 21;
        state.dropFadeBase = 7;
        state.dropFadeRange = 3;
        state.dropSizeBase = 309;
        state.dropTexture = &D_800903C8;
        state.dropFrameRate = 0;
        state.dropCallback = -1;
        state.ringRate = 0.0f;
        state.ringCount = 0.0f;
        state.swayX = 6.0f;
        state.splashScaleRange = 6.0f;
        state.ringScaleBase = 100.0f;
        state.ringScaleRange = 100.0f;
        state.speedRange = 25.0f;
        state.splashRadius = 25.0f;
        state.dropRadius = 17.19f;
        state.dropSpeedBase = 270.0f;
        state.dropSpeedRange = 154.0f;
        state.dropSizeRange = 173.0f;
        state.ringY = 60.0f;
        break;
    case 1:
    case 2:
    case 3:
    case 4: {
        Actor *character = collision_findObjectById(6);

        if (character == NULL) {
            return 0;
        }
        if (character->animseq.uh == 6) {
            state.floor = 498.0f;
        } else {
            state.floor = -37.0f;
        }
        spawn.pos.y = 736.0f;
        switch (kind) {
        case 1:
            spawn.pos.x = 650.0f;
            spawn.pos.z = -650.0f;
            break;
        case 2:
            spawn.pos.x = -650.0f;
            spawn.pos.z = -650.0f;
            break;
        case 3:
            spawn.pos.z = 650.0f;
            spawn.pos.x = -650.0f;
            break;
        case 4:
            spawn.pos.x = 650.0f;
            spawn.pos.z = 650.0f;
            break;
        }
        state.x = spawn.pos.x;
        state.z = spawn.pos.z;
        state.gravity = -2.0f;
        state.rate = 1.26f;
        state.widthBase = 132.0f;
        state.widthRange = 17.6f;
        state.velocityY = -9.1f;
        state.spawnRate = 0.32f;
        state.radius = 114.9f;
        state.speedBase = 1039.0f;
        state.speedRange = 769.0f;
        state.accelRange = -51.0f;
        state.splashRate = 0.31f;
        state.splashRadius = 139.53f;
        state.splashSpeedBase = 18.8f;
        spawn.itemCount = 30;
        state.splashScaleBase = 15.0f;
        state.splashScaleRange = 12.0f;
        spawn.life = 160;
        state.splashSizeBase = 2.75f;
        state.timer = 110;
        state.unk28 = 255;
        state.alpha = 255;
        state.widthAngleRange = 12;
        state.texCoordStep = 1;
        state.texCoordStepRange = 3;
        state.angleXRange = 6;
        state.angleZRange = 6;
        state.splashSizeRange = 1.19f;
        spawn.flags |= 1;
        state.trailTexture = 25;
        state.unk42 = 81;
        state.unk44 = 8;
        state.unk4C = 6;
        state.unk60 = 2;
        state.pitchBase = 36;
        state.pitchRange = 26;
        state.lifetime = 600;
        state.accelBase = -72;
        state.fadeBase = 6;
        state.dropRate = 0.72f;
        state.fadeRange = 3;
        state.sizeBase = 1300;
        state.frameRate = 171;
        state.callback = 12;
        state.splashPitchBase = -58;
        state.splashPitchRange = 11;
        state.splashLifetime = 70;
        state.splashCountBase = 4;
        state.splashCountRange = 2;
        state.unkCA = 9;
        state.angleX = 0;
        state.angleZ = 0;
        state.unk48 = 0;
        state.unk50 = 0;
        state.unk54 = 0;
        state.unk58 = 0;
        state.unk5C = 0;
        state.spawnCount = 0.0f;
        state.texture = &D_80090414;
        state.splashCount = 0.0f;
        state.dropCount = 0.0f;
        state.dropPitchBase = -56;
        state.dropPitchRange = 10;
        state.dropLifetime = 600;
        state.dropAccelBase = -223;
        state.dropAccelRange = 64;
        state.dropFadeBase = 6;
        state.dropFadeRange = 4;
        state.dropSizeBase = 1300;
        state.dropTexture = &D_80090414;
        state.dropFrameRate = 171;
        state.dropCallback = 12;
        state.ringCount = 0.0f;
        state.swayX = 22.0f;
        state.swayZ = 22.0f;
        state.ringScaleBase = 100.0f;
        state.ringScaleRange = 100.0f;
        state.sizeRange = 500.0f;
        state.dropSizeRange = 500.0f;
        state.splashSpeedRange = 25.0f;
        state.dropRadius = 131.0f;
        state.dropSpeedBase = 2366.0f;
        state.dropSpeedRange = 1231.0f;
        state.ringY = 3.0f;
        state.ringRate = 0.078f;
        }
        break;
    default:
        return 0;
    }
    spawn.kind = 5;
    state.sound = 440;
    actor = effect_newEffectObj(&spawn, 0x120, sizeof(StreamPoint), 4, 4, 4, 0, 0, 0, 0xFF, 1);
    if (actor != NULL) {
        memcpy(actor->state, &state, sizeof(StreamState));
    }
    return (s32)actor;
}

/*
 * Updates a stream: moves every record under gravity, and where one falls below the floor spawns
 * splash particles, drops and ripple sprites there and discards it with every older record. Then ramps
 * up the alpha of the four oldest records, plays the splash sound now and then, and tracks the oldest record's
 * position in endPos. Always returns 1.
 */
s32 effect_updateFallingStream(StreamActor *actor) {
    StreamState *state;
    StreamPoint *records;
    s32 index;
    Vec3f pos;
    Vec3f vel;
    StreamPoint *rec;
    s32 timer;

    state = actor->state;
    records = actor->records;
    index = actor->head.end;
    while (index != actor->head.start) {
        index--;
        if (index < 0) {
            index = actor->head.tmpl.itemCount - 1;
        }
        // matching: the u32 byte offset gives IDO's multu (&records[index] uses a signed multiply)
        rec = (StreamPoint *)((u8 *)records + index * sizeof(StreamPoint));
        rec->velocityY += state->gravity * gFrameDelta;
        rec->pos.y += rec->velocityY * gFrameDelta;
        if (rec->pos.y < state->floor) {
            {
                f32 speed;
                u8 angle;
                u8 angle2;
                f32 sine1;
                f32 cosine1;
                f32 sine2;
                f32 cosine2;
                f32 radius;

                state->splashCount += state->splashRate;
                while (state->splashCount > 1.0f) {
                    speed = func_150ADA68() * state->splashSpeedRange + state->splashSpeedBase;
                    angle = func_150ADA20();
                    sine1 = math_cosBinAngleMasked(angle);
                    cosine1 = math_cosBinAngleMasked(angle - 0x40);
                    angle2 = (func_150ADA20() % (state->splashPitchRange + 1)) + state->splashPitchBase;
                    sine2 = math_cosBinAngleMasked(angle2);
                    cosine2 = math_cosBinAngleMasked(angle2 - 0x40);
                    radius = func_150ADA68() * state->splashRadius;
                    pos.x = rec->pos.x + radius * cosine1;
                    pos.y = state->floor;
                    pos.z = rec->pos.z + radius * sine1;
                    vel.x = speed * sine2 * cosine1;
                    vel.y = -speed * cosine2;
                    vel.z = speed * sine2 * sine1;
                    particle_spawnSplash(&pos, &vel, func_150ADA68() * state->splashSizeRange + state->splashSizeBase,
                                  state->splashLifetime, (state->trailTexture == 0x18) ? 0x7D : 0xFF,
                                  func_150ADA68() * state->splashScaleRange + state->splashScaleBase,
                                  func_150ADA20() % (state->splashCountRange + 1) + state->splashCountBase, 1,
                                  1.7f, 1.7f, 1, (state->trailTexture == 0x18) ? 7 : 2, 0, 0x10, 0xF, 0,
                                  actor->head.header.owner, actor->head.header.kind);
                    state->splashCount -= 1.0f;
                }
            }
            {
                BillboardTemplate packet;
                f32 speed;
                u8 angle;
                u8 angle2;
                u8 ok;
                f32 sine1;
                f32 cosine1;
                f32 sine2;
                f32 cosine2;
                f32 radius;

                state->dropCount += state->dropRate;
                if (state->dropCount > 1.0f) {
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
                    packet.callback = state->dropCallback;
                    packet.flags = 0x12;
                    packet.frame = 0;
                    ok = 1;
                    packet.frameStep = state->dropFrameRate;
                    packet.anim = state->dropTexture;
                    packet.y = state->floor;
                    packet.life = state->dropLifetime;
                    do {
                        speed = func_150ADA68() * state->dropSpeedRange + state->dropSpeedBase;
                        angle = func_150ADA20();
                        sine1 = math_cosBinAngleMasked(angle);
                        cosine1 = math_cosBinAngleMasked(angle - 0x40);
                        angle2 = (func_150ADA20() % (state->dropPitchRange + 1)) + state->dropPitchBase;
                        sine2 = math_cosBinAngleMasked(angle2);
                        cosine2 = math_cosBinAngleMasked(angle2 - 0x40);
                        radius = func_150ADA68() * state->dropRadius;
                        packet.x = rec->pos.x + radius * cosine1;
                        packet.z = rec->pos.z + radius * sine1;
                        packet.velX = speed * sine2 * cosine1;
                        packet.velY = -speed * cosine2;
                        packet.velZ = speed * sine2 * sine1;
                        packet.accelY = (func_150ADA20() % (state->dropAccelRange + 1)) + state->dropAccelBase;
                        packet.fadeStep = (func_150ADA20() % (state->dropFadeRange + 1)) + state->dropFadeBase;
                        packet.width = packet.height = func_150ADA68() * state->dropSizeRange + state->dropSizeBase;
                        if (effect_spawnBillboard(&packet, 0, 0, -1, actor->head.header.owner, actor->head.header.kind) == NULL) {
                            ok = 0;
                        }
                        state->dropCount -= 1.0f;
                    } while (state->dropCount > 1.0f && ok);
                    while (state->dropCount > 1.0f) {
                        state->dropCount -= 1.0f;
                    }
                }
            }
            {
                u8 pad[8]; // matching: unused stack space
                SpriteDef ring;
                f32 scale;
                s32 ok;

                state->ringCount += state->ringRate;
                if (state->ringCount > 1.0f) {
                    ok = 1;
                    ring.frame = 0;
                    ring.frameSpeed = 0;
                    ring.flags = 1;
                    ring.lifetime = 100;
                    ring.unk10 = 0xFF;
                    ring.unk11 = 0xFF;
                    ring.rgba[0] = 0;
                    ring.rgba[1] = 0;
                    ring.rgba[2] = 0;
                    ring.rgba[3] = 0xFF;
                    ring.renderModes = 0x30001;
                    do {
                        scale = func_150ADA68() * state->ringScaleRange + state->ringScaleBase;
                        switch (func_150ADA20() & 1) {
                        case 0:
                            ring.texture = 0x1D;
                            break;
                        case 1:
                            ring.texture = 0x1E;
                            break;
                        }
                        if (particle_spawnFlatQuadSpriteNoModel(&ring, NULL, 5, 0, rec->pos.x, state->ringY, rec->pos.z, scale, scale,
                                          func_150ADA20() & 0xFF, (func_150ADA20() & 1) + (func_150ADA20() & 1), 0, 0xFF, 0) == NULL) {
                            ok = 0;
                        }
                        state->ringCount -= 1.0f;
                    } while (state->ringCount > 1.0f && ok);
                    while (state->ringCount > 1.0f) {
                        state->ringCount -= 1.0f;
                    }
                }
            }
            while (index != actor->head.start) {
                actor->head.start++;
                if (actor->head.start == actor->head.tmpl.itemCount) {
                    actor->head.start = 0;
                }
                actor->head.count--;
            }
            records[actor->head.start].pos.y = state->floor;
        }
    }
    // matching: byte-offset rec pointers as above (indexing records[] moves the multiplies)
    index = actor->head.start;
    records[index].alpha = 0;
    index++;
    if (index == actor->head.tmpl.itemCount) {
        index = 0;
    }
    rec = (StreamPoint *)((u8 *)records + index * sizeof(StreamPoint));
    rec->alpha = 0x3F;
    index++;
    if (index == actor->head.tmpl.itemCount) {
        index = 0;
    }
    rec = (StreamPoint *)((u8 *)records + index * sizeof(StreamPoint));
    rec->alpha = 0x7F;
    index++;
    if (index == actor->head.tmpl.itemCount) {
        index = 0;
    }
    rec = (StreamPoint *)((u8 *)records + index * sizeof(StreamPoint));
    rec->alpha = 0xBE;
    if (state->sound != 0) {
        timer = state->soundTimer;
        timer -= D_800BE9E4; // matching: a separate statement (one expression reorders the loads)
        if (timer <= 0) {
            sound_play(func_150ADA20() % 10U + state->sound, 0x7FFF, 0x40, 0, 0);
            timer = func_150ADA20() & 0xC;
        }
        state->soundTimer = timer;
    }
    if (actor->head.count > 0) {
        actor->head.modelPos = records[actor->head.start].pos;
    } else {
        actor->head.modelPos.x = 0.0f;
        actor->head.modelPos.y = 0.0f;
        actor->head.modelPos.z = 0.0f;
    }
    return 1;
}

/*
 * Updates a trail: sways the emission point around the actor, appends one record per whole step of
 * state->count (spawning particles at the actor on the way), and when state->timer runs out fades the
 * newest records and deactivates the trail. Always returns 1.
 */
s32 effect_updateFallingStreamTrail(StreamActor *actor) {
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
    state->angleX += (func_150ADA20() % (state->angleXRange + 1)) * D_800BE9E4;
    state->angleZ += (func_150ADA20() % (state->angleZRange + 1)) * D_800BE9E4;
    state->count += state->rate * gFrameDelta;
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

            scale = gFrameDelta;
            stepScale = gFrameDelta * inverse;
            x = state->x;
            z = state->z;
            dx = math_cosBinAngleMasked(state->angleX - 0x40) * state->swayX + actor->head.tmpl.pos.x - state->x;
            dz = math_cosBinAngleMasked(state->angleZ - 0x40) * state->swayZ + actor->head.tmpl.pos.z - state->z;
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
                            sinYaw = math_cosBinAngleMasked(yaw);
                            cosYaw = math_cosBinAngleMasked(yaw - 0x40);
                            pitch = (func_150ADA20() % (state->pitchRange + 1)) + state->pitchBase;
                            sinPitch = math_cosBinAngleMasked(pitch);
                            cosPitch = math_cosBinAngleMasked(pitch - 0x40);
                            packet.x = actor->head.tmpl.pos.x + state->radius * cosYaw;
                            packet.z = actor->head.tmpl.pos.z + state->radius * sinYaw;
                            packet.velX = speed * sinPitch * cosYaw;
                            packet.velY = -speed * cosPitch;
                            packet.velZ = speed * sinPitch * sinYaw;
                            packet.accelY = func_150ADA68() * state->accelRange + state->accelBase;
                            packet.fadeStep = (func_150ADA20() % (state->fadeRange + 1)) + state->fadeBase;
                            packet.width = packet.height = func_150ADA68() * state->sizeRange + state->sizeBase;
                            if (effect_spawnBillboard(&packet, 0, 0, -1, actor->head.header.owner, actor->head.header.kind) == NULL) {
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
                    state->widthAngle += func_150ADA20() % (state->widthAngleRange + 1);
                    records[actor->head.end].width = math_cosBinAngleMasked(state->widthAngle - 0x40) * state->widthRange + state->widthBase;
                    records[actor->head.end].alpha = state->alpha;
                    records[actor->head.end].texCoord = state->texCoord;
                    state->texCoord += state->texCoordStep + (func_150ADA20() % (state->texCoordStepRange + 1));
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

extern Mode150DD640 gEffectRenderModes[];

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
Gfx *effect_drawFallingStream(StreamActor *actor, Gfx *gfx, s16 view) {
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
        particle_getViewBuffer(actor->vtxBuf, view, (actor->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            state = actor->state;
            records = actor->records;
            update = 1;
            mode = state->unk42 & 0xF;
            sinYaw = gCameraAngleSines[view];
            unk42High = (state->unk42 & 0xF0) >> 4;
            cosYaw = gCameraAngleCosines[view];
            if (state->unk60 == 2) {
                tcAxis[0] = 1;
                tcAxis[1] = 0;
            } else {
                tcAxis[0] = 0;
                tcAxis[1] = 1;
            }
            gfx = math_gfxLoadTextureFrame(gfx, gEffectTextures + state->trailTexture, 0, 0, 0, 0, state->trailTexture, 0, 0, &update, 3);
            gfx = math_gfxSetEnvColor(gfx, state->unk28, state->unk28, state->unk28, 0xFF, &update);
            gfx = particle_setCombineMode(gfx, unk42High, &update);
            gfx = math_gfxUpdateGeometryMode(gfx, 1, 0x160600);
            gfx = math_gfxSetOtherMode(gfx, gSpriteTlutMode | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, gEffectRenderModes[mode].unk0 | gEffectRenderModes[mode].unk4, &update);
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
            SET_TEXCOORD(vtx, tcAxis[1], (gEffectTextures[state->trailTexture].width - 1) << 6);
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
            // matching: an unused label. Without it, in this unit IDO keeps vtx in a register across some of the
            // vertex stores of the first quad instead of reloading it after each one
            lbl:
                vtx->v.ob[1] = older.y;
                vtx->v.ob[2] = records[j].width * sinYaw + older.z;
                SET_TEXCOORD(vtx, tcAxis[0], texCoord << 6);
                SET_TEXCOORD(vtx, tcAxis[1], (gEffectTextures[state->trailTexture].width - 1) << 6);
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
