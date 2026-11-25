#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void *func_15167D84(void *, s32, s32, s8, u8, s32);

/* Particle spawn request passed to func_15167D84. */

typedef struct Game10A350Emission {
    s32 texture;
    char pad4[8];
    s16 frame;
    s16 frame_rate;
    s16 position[3];
    s16 velocity_x;
    s16 velocity_z;
    char pad1A[2];
    u8 fraction_x;
    u8 fraction_y;
    u8 fraction_z;
    s8 callback;
    s16 velocity_y;
    s16 acceleration;
    s16 width;
    s16 height;
    s16 lifetime;
    s8 fade;
    char pad2B;
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    u8 environment[3];
    char pad33;
    u16 flags;
    char pad36[2];
} Game10A350Emission;

/* One point of the trail ring buffer. */
typedef struct Game10A350Record {
    /* 0x00 */ struct17 pos;
    /* 0x0C */ f32 velocityY;
    /* 0x10 */ f32 width;
    /* 0x14 */ s16 alpha;
    /* 0x16 */ u8 texCoord; /* S texture coordinate, in texels */
    /* 0x17 */ u8 pad17;
} Game10A350Record; // size 0x18

typedef struct Game10A350State {
    /* 0x00 */ f32 swayX;
    /* 0x04 */ u8 angleX;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 angleXRange;
    /* 0x08 */ f32 swayZ;
    /* 0x0C */ u8 angleZ;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 angleZRange;
    /* 0x10 */ f32 x;
    /* 0x14 */ f32 z;
    /* 0x18 */ f32 gravity;
    /* 0x1C */ f32 rate;
    /* 0x20 */ f32 count;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ u8 unk28; /* prim colour grey level */
    /* 0x29 */ u8 alpha;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ f32 widthBase;
    /* 0x30 */ f32 widthRange;
    /* 0x34 */ u8 widthAngle;
    /* 0x35 */ u8 pad35;
    /* 0x36 */ s16 widthAngleRange;
    /* 0x38 */ f32 velocityY;
    /* 0x3C */ u8 texCoordStep;
    /* 0x3D */ u8 pad3D;
    /* 0x3E */ s16 texCoordStepRange;
    /* 0x40 */ u8 texCoord;
    /* 0x41 */ u8 trailTexture; /* D_80090B60 index */
    /* 0x42 */ u8 unk42; /* low nibble: D_800A4AC8 render mode, high nibble: func_1513F4E4 argument */
    /* 0x43 */ u8 pad43[0x1D];
    /* 0x60 */ u8 unk60; /* 2: the texture runs along T instead of S */
    /* 0x61 */ u8 pad61[3];
    /* 0x64 */ f32 spawnRate;
    /* 0x68 */ f32 spawnCount;
    /* 0x6C */ f32 radius;
    /* 0x70 */ s16 pitchBase;
    /* 0x72 */ s16 pitchRange;
    /* 0x74 */ f32 speedBase;
    /* 0x78 */ f32 speedRange;
    /* 0x7C */ s16 lifetime;
    /* 0x7E */ s16 accelBase;
    /* 0x80 */ f32 accelRange;
    /* 0x84 */ s16 fadeBase;
    /* 0x86 */ s16 fadeRange;
    /* 0x88 */ s16 sizeBase;
    /* 0x8A */ u8 pad8A[2];
    /* 0x8C */ f32 sizeRange;
    /* 0x90 */ s32 texture;
    /* 0x94 */ s32 frameRate;
    /* 0x98 */ s8 callback;
    /* 0x99 */ u8 pad99[0x7F];
    /* 0x118 */ s16 timer;
} Game10A350State;

typedef struct Game10A350Actor {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unk0C;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ struct17 pos;
    /* 0x1C */ u8 pad1C[2];
    /* 0x1E */ u16 flags;
    /* 0x20 */ u8 pad20[5];
    /* 0x25 */ u8 capacity; /* records[] length */
    /* 0x26 */ u8 pad26[6];
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 head; /* oldest record */
    /* 0x2E */ s8 tail; /* next record to write */
    /* 0x2F */ u8 pad2F;
    /* 0x30 */ s8 active;
    /* 0x31 */ u8 pad31[0x53];
    /* 0x84 */ Vtx *vtxBufs[4]; /* one per view, see func_151D5D60 */
    /* 0x94 */ Game10A350Record *records;
    /* 0x98 */ Game10A350State *state;
} Game10A350Actor;

/*
 * Updates a trail: sways the emission point around the actor, appends one record per whole step of
 * state->count (spawning particles at the actor on the way), and when state->timer runs out fades the
 * newest records and deactivates the trail. Always returns 1.
 */
s32 func_150DCEA0(Game10A350Actor *actor) {
    Game10A350State *state;
    Game10A350Record *records;
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
            Game10A350Emission packet;
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
            dx = ((func_151423D8((u8)(state->angleX - 0x40)) * state->swayX) + actor->pos.unk0) - state->x;
            dz = ((func_151423D8((u8)(state->angleZ - 0x40)) * state->swayZ) + actor->pos.unk8) - state->z;
            if (state->count > 1.0f) {
                stepX = dx * inverse;
                stepZ = dz * inverse;
                do {
                    state->spawnCount += state->spawnRate;
                    if (state->spawnCount > 1.0f) {
                        packet.fraction_x = 0;
                        packet.fraction_z = 0;
                        packet.fraction_y = 0;
                        packet.red = 0xFF;
                        packet.green = 0xFF;
                        packet.blue = 0xFF;
                        packet.environment[0] = 0;
                        packet.environment[1] = 0;
                        packet.environment[2] = 0;
                        packet.alpha = 0xFF;
                        packet.callback = state->callback;
                        packet.flags = 0x12;
                        packet.frame = 0;
                        spawned = TRUE;
                        packet.frame_rate = state->frameRate;
                        packet.texture = state->texture;
                        packet.position[1] = actor->pos.unk4;
                        packet.lifetime = state->lifetime;
                        do {
                            speed = func_150ADA68() * state->speedRange + state->speedBase;
                            yaw = func_150ADA20();
                            sinYaw = func_151423D8(yaw);
                            cosYaw = func_151423D8((u8)(yaw - 0x40));
                            pitch = (func_150ADA20() % (u32)(state->pitchRange + 1)) + state->pitchBase;
                            sinPitch = func_151423D8(pitch);
                            cosPitch = func_151423D8((u8)(pitch - 0x40));
                            packet.position[0] = actor->pos.unk0 + state->radius * cosYaw;
                            packet.position[2] = actor->pos.unk8 + state->radius * sinYaw;
                            packet.velocity_x = speed * sinPitch * cosYaw;
                            packet.velocity_y = -speed * cosPitch;
                            packet.velocity_z = speed * sinPitch * sinYaw;
                            packet.acceleration = func_150ADA68() * state->accelRange + state->accelBase;
                            packet.fade = (func_150ADA20() % (u32)(state->fadeRange + 1)) + state->fadeBase;
                            packet.width = packet.height = func_150ADA68() * state->sizeRange + state->sizeBase;
                            if (func_15167D84(&packet, 0, 0, -1, actor->unk0C, actor->unk01) == NULL) {
                                spawned = FALSE;
                            }
                            state->spawnCount -= 1.0f;
                        } while (state->spawnCount > 1.0f && spawned);
                        while (state->spawnCount > 1.0f) {
                            state->spawnCount -= 1.0f;
                        }
                    }
                    records[actor->tail].pos.unk0 = x;
                    records[actor->tail].pos.unk4 = actor->pos.unk4;
                    records[actor->tail].pos.unk8 = z;
                    records[actor->tail].velocityY = state->velocityY;
                    state->widthAngle += func_150ADA20() % (u32)(state->widthAngleRange + 1);
                    records[actor->tail].width = func_151423D8((u8)(state->widthAngle - 0x40)) * state->widthRange + state->widthBase;
                    records[actor->tail].alpha = state->alpha;
                    records[actor->tail].texCoord = state->texCoord;
                    state->texCoord += state->texCoordStep + (func_150ADA20() % (u32)(state->texCoordStepRange + 1));
                    records[actor->tail].velocityY += state->gravity * scale;
                    records[actor->tail].pos.unk4 += records[actor->tail].velocityY * scale;
                    actor->tail++;
                    if (actor->tail == actor->capacity) {
                        actor->tail = 0;
                    }
                    actor->count++;
                    if (actor->tail == actor->head) {
                        actor->head++;
                        if (actor->head == actor->capacity) {
                            actor->head = 0;
                        }
                        actor->count--;
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
        index = actor->tail - 1;
        if (index < 0) {
            index = actor->capacity - 1;
        }
        records[index].alpha = 0;
        index--;
        if (index < 0) {
            index = actor->capacity - 1;
        }
        records[index].alpha = 0x3F;
        index--;
        if (index < 0) {
            index = actor->capacity - 1;
        }
        records[index].alpha = 0x7F;
        index--;
        if (index < 0) {
            index = actor->capacity - 1;
        }
        records[index].alpha = 0xBE;
        actor->active = 0;
        actor->flags &= ~2;
    }
    return 1;
}

typedef struct {
    f32 x, y, z;
} Vec3f_150DD640;

typedef struct {
    Vec3f_150DD640 pos;
    u8 pad0C[4];
    f32 width;
    s16 alpha;
    u8 level;
    u8 pad17;
} Point150DD640;

typedef struct {
    u8 pad0[0x28];
    u8 unk28;
    u8 pad29[0x41 - 0x29];
    u8 unk41;
    u8 unk42;
    u8 pad43[0x60 - 0x43];
    u8 unk60;
} Style150DD640;

typedef struct {
    u8 pad0[0x10];
    Vec3f_150DD640 pos;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x84 - 0x2F];
    u8 unk84[0x10];
    Point150DD640 *points;
    Style150DD640 *style;
} Owner150DD640;

typedef struct {
    u8 pad0[6];
    u16 unk6;
    u8 pad8[4];
} Tex150DD640;

typedef struct {
    s32 unk0;
    s32 unk4;
} Mode150DD640;

extern Tex150DD640 D_80090B60[];
extern Mode150DD640 D_800A4AC8[];
extern s32 D_800D2C9C;
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

Gfx *func_150DD640(Owner150DD640 *arg0, Gfx *gfx, s16 arg2) {
    Vtx *vtx;
    f32 cosv;
    Vec3f_150DD640 b;
    Vec3f_150DD640 a;
    s32 j;
    s32 i;
    f32 sinv;
    u16 level;
    u8 idx[2];
    u8 padB;
    u8 prev;
    u8 update;
    u8 mode;
    u8 shade;
    Style150DD640 *style;
    Point150DD640 *points;

    if (arg0->unk2C >= 2) {
        func_151D5D60(arg0->unk84, arg2, (arg0->unk25 * 2 + 10) * sizeof(Vtx), (void **)&vtx, NULL);
        if (vtx != NULL) {
            style = arg0->style;
            points = arg0->points;
            update = 1;
            mode = style->unk42 & 0xF;
            sinv = D_800DD1D8[arg2];
            shade = (style->unk42 & 0xF0) >> 4;
            cosv = D_800DD1E8[arg2];
            if (style->unk60 == 2) {
                idx[0] = 1;
                idx[1] = 0;
            } else {
                idx[0] = 0;
                idx[1] = 1;
            }
            gfx = func_15142E24(gfx, (s32)(D_80090B60 + style->unk41), 0, 0, 0, 0, style->unk41, 0, 0, &update, 3);
            gfx = func_15142C10(gfx, style->unk28, style->unk28, style->unk28, 0xFF, &update);
            gfx = func_1513F4E4(gfx, shade, &update);
            gfx = func_15142B7C(gfx, 1, 0x160600);
            gfx = func_15142FBC(gfx, D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[mode].unk0 | D_800A4AC8[mode].unk4, &update);
            if (arg0->unk1E & 2) {
                j = arg0->unk2E - 1;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                a = arg0->pos;
                b = points[j].pos;
                prev = level = points[j].level;
            } else {
                i = arg0->unk2E - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                j = i - 1;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                a = points[i].pos;
                b = points[j].pos;
                prev = points[i].level;
                level = points[j].level;
            }
            vtx->v.ob[0] = (s16)(s32)(points[j].width * cosv + a.x);
            vtx->v.ob[1] = (s16)(s32)a.y;
            vtx->v.ob[2] = (s16)(s32)(a.z - points[j].width * sinv);
            if (idx[0]) {
                vtx->v.tc[idx[0]] = prev << 6;
            } else {
                vtx->v.tc[idx[0]] = prev << 6;
            }
            vtx->v.tc[idx[1]] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = points[i].alpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = (s16)(s32)(a.x - points[j].width * cosv);
            vtx->v.ob[1] = (s16)(s32)a.y;
            vtx->v.ob[2] = (s16)(s32)(points[j].width * sinv + a.z);
            if (idx[0]) {
                vtx->v.tc[idx[0]] = prev << 6;
            } else {
                vtx->v.tc[idx[0]] = prev << 6;
            }
            if (idx[1]) {
                vtx->v.tc[idx[1]] = ((D_80090B60 + style->unk41)->unk6 - 1) << 6;
            } else {
                vtx->v.tc[idx[1]] = ((D_80090B60 + style->unk41)->unk6 - 1) << 6;
            }
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = points[i].alpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                vtx->v.ob[0] = (s16)(s32)(points[j].width * cosv + b.x);
                vtx->v.ob[1] = (s16)(s32)b.y;
                vtx->v.ob[2] = (s16)(s32)(b.z - points[j].width * sinv);
                if (idx[0]) vtx->v.tc[idx[0]] = level << 6; else vtx->v.tc[idx[0]] = level << 6;
                vtx->v.tc[idx[1]] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = points[j].alpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = (s16)(s32)(b.x - points[j].width * cosv);
                vtx->v.ob[1] = (s16)(s32)b.y;
                vtx->v.ob[2] = (s16)(s32)(points[j].width * sinv + b.z);
                if (idx[0]) {
                    vtx->v.tc[idx[0]] = level << 6;
                } else {
                    vtx->v.tc[idx[0]] = level << 6;
                }
                if (idx[1]) {
                    vtx->v.tc[idx[1]] = ((D_80090B60 + style->unk41)->unk6 - 1) << 6;
                } else {
                    vtx->v.tc[idx[1]] = ((D_80090B60 + style->unk41)->unk6 - 1) << 6;
                }
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = points[j].alpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                if (prev < level) {
                    memcpy(vtx, vtx - 2, 0x20);
                    vtx[-2].v.tc[idx[0]] -= 0x4000;
                    vtx++;
                    vtx[-2].v.tc[idx[0]] -= 0x4000;
                    vtx++;
                }
                prev = level;
                i = j;
                j--;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                a = points[i].pos;
                b = points[j].pos;
                level = points[j].level;
            } while (i != arg0->unk2D);
        }
    }
    return gfx;
}
