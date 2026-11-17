#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern f32 D_800A0C00;
extern f32 D_800A0C04;
extern f32 D_800A0C08;
extern f32 D_800A0C0C;
extern f32 D_800A0C10;
extern f32 D_800A0C14;
extern f32 D_800A0C18;
extern f32 D_800A0C1C;
extern f32 D_800A0C20;
extern f32 D_800A0C24;
extern f32 D_800A0C28;
extern f32 D_800A0C2C;
extern f32 D_800A0C30;
extern s32 D_80090514;

void func_15152F70(void *arg0, u8 arg1);
void func_15153298(void *arg0, u8 arg1, s32 arg2);
void func_15196438(s32 arg0, s32 arg1, u8 arg2, s32 arg3);

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s8 unk38;
    s8 unk39;
    u8 pad3A[2];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 unk54;
    u8 unk55;
    u8 pad56[2];
} Struct150DB9E0A;

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    s16 unk0C;
    s16 unk0E;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s8 unk28;
    s8 unk29;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    void *unk38;
    u8 unk3C;
    u8 unk3D;
    u8 pad3E[2];
} Struct150DB9E0B;

typedef struct {
    u8 pad_0x00[0x20];
    u8 field_0x20;
} EffectSpawnState;

typedef struct {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    f32 field_0x0C;
    f32 field_0x10;
} EffectSpawnPoint;

typedef struct {
    u8 pad_0x00[0x1];
    u8 field_0x01;
    u8 pad_0x02[0xA];
    u8 field_0x0C;
    u8 pad_0x0D[0x20];
    s8 field_0x2D;
    u8 pad_0x2E[0x66];
    EffectSpawnPoint *field_0x94;
    EffectSpawnState *field_0x98;
} EffectSpawnSource;

typedef struct {
    s32 field_0x00;
    s16 field_0x04;
    s8 field_0x06;
    u8 pad_0x07[0x1];
    s32 field_0x08;
    s32 field_0x0C;
    u8 field_0x10;
    u8 field_0x11;
    u8 field_0x12;
    u8 field_0x13;
    u8 field_0x14;
    u8 field_0x15;
    u8 pad_0x16[0x2];
    s32 field_0x18;
    u8 pad_0x1C[0xC];
    EffectSpawnPoint *field_0x28;
    EffectSpawnState *field_0x2C;
} EffectSpawnParams;
void func_150DB9E0(s32 arg0, s32 arg1, u8 arg2);
void func_150DBC60(void *arg0);

typedef struct {
    f32 u00;
    u8 u04;
    u8 pad05[0x1];
    s16 u06;
    f32 u08;
    u8 u0C;
    u8 pad0D[0x1];
    s16 u0E;
    f32 u10;
    f32 u14;
    f32 u18;
    f32 u1C;
    f32 u20;
    f32 u24;
    u8 u28;
    u8 u29;
    u8 pad2A[0x2];
    f32 u2C;
    f32 u30;
    u8 u34;
    u8 pad35[0x1];
    s16 u36;
    f32 u38;
    u8 u3C;
    u8 pad3D[0x1];
    s16 u3E;
    u8 u40;
    u8 u41;
    u8 u42;
    u8 pad43[0x1];
    s32 u44;
    s32 u48;
    s32 u4C;
    s32 u50;
    s32 u54;
    s32 u58;
    s32 u5C;
    u8 u60;
    u8 pad61[0x3];
    f32 u64;
    f32 u68;
    f32 u6C;
    s16 u70;
    s16 u72;
    f32 u74;
    f32 u78;
    s16 u7C;
    s16 u7E;
    f32 u80;
    s16 u84;
    s16 u86;
    s16 u88;
    u8 pad8A[0x2];
    f32 u8C;
    u8 * u90;
    s32 u94;
    s8 u98;
    u8 pad99[0x3];
    f32 u9C;
    f32 uA0;
    f32 uA4;
    s16 uA8;
    s16 uAA;
    f32 uAC;
    f32 uB0;
    f32 uB4;
    f32 uB8;
    f32 uBC;
    f32 uC0;
    s16 uC4;
    s16 uC6;
    s16 uC8;
    u8 uCA;
    u8 padCB[0x1];
    f32 uCC;
    f32 uD0;
    f32 uD4;
    s16 uD8;
    s16 uDA;
    f32 uDC;
    f32 uE0;
    s16 uE4;
    s16 uE6;
    s16 uE8;
    s16 uEA;
    s16 uEC;
    s16 uEE;
    f32 uF0;
    u8 * uF4;
    s32 uF8;
    s8 uFC;
    u8 padFD[0x3];
    f32 u100;
    f32 u104;
    f32 u108;
    f32 u10C;
    f32 u110;
    u8 pad114[0x2];
    s16 u116;
    s16 u118;
    u8 pad11A[0x2];
} S150DBD70;
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    s16 u0C;
    u16 u0E;
    s32 u10;
    u8 pad14;
    u8 u15;
    u8 pad16[6];
} T150DBD70;

extern u8 D_800903C8[];
extern u8 D_80090414[];
void *memcpy(void *, const void *, u32);
void *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
s32 func_150DBD70(u8 arg0);

void *func_15167D84(void *, s32, s32, s8, u8, s32);

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

typedef struct Game10A350Record {
    f32 x;
    f32 y;
    f32 z;
    f32 speed;
    f32 width;
    s16 alpha;
    u8 color;
    u8 pad17;
} Game10A350Record;

typedef struct Game10A350State {
    f32 swayX;
    u8 angleX;
    u8 pad5;
    s16 angleXRange;
    f32 swayZ;
    u8 angleZ;
    u8 padD;
    s16 angleZRange;
    f32 x;
    f32 z;
    f32 gravity;
    f32 rate;
    f32 count;
    u8 pad24[5];
    u8 alpha;
    u8 pad2A[2];
    f32 widthBase;
    f32 widthRange;
    u8 widthAngle;
    u8 pad35;
    s16 widthAngleRange;
    f32 speed;
    u8 colorStep;
    u8 pad3D;
    s16 colorStepRange;
    u8 color;
    u8 pad41[0x23];
    f32 spawnRate;
    f32 spawnCount;
    f32 radius;
    s16 pitchBase;
    s16 pitchRange;
    f32 speedBase;
    f32 speedRange;
    s16 lifetime;
    s16 accelBase;
    f32 accelRange;
    s16 fadeBase;
    s16 fadeRange;
    s16 sizeBase;
    u8 pad8A[2];
    f32 sizeRange;
    s32 texture;
    s32 frameRate;
    s8 callback;
    u8 pad99[0x7F];
    s16 timer;
} Game10A350State;

typedef struct Game10A350Actor {
    u8 pad0;
    u8 field1;
    u8 pad2[0xA];
    u8 fieldC;
    u8 padD[3];
    f32 x;
    f32 y;
    f32 z;
    u8 pad1C[2];
    u16 flags;
    u8 pad20[5];
    u8 capacity;
    u8 pad26[6];
    s8 count;
    s8 head;
    s8 tail;
    u8 pad2F;
    s8 active;
    u8 pad31[0x63];
    Game10A350Record *records;
    Game10A350State *state;
} Game10A350Actor;

s32 func_150DCEA0(Game10A350Actor *arg0) {
    Game10A350State *state;
    Game10A350Record *records;
    u8 angle2;
    s32 index;
    f32 stepScale;
    f32 inverse;
    f32 scale;
    f32 x;
    f32 z;
    f32 dx;
    f32 dz;
    f32 speed;

    state = arg0->state;
    records = arg0->records;
    state->angleX += (func_150ADA20() % (u32)(state->angleXRange + 1)) * D_800BE9E4;
    state->angleZ += (func_150ADA20() % (u32)(state->angleZRange + 1)) * D_800BE9E4;
    state->count += state->rate * D_800BE9A4;
    if (state->count > 0.99999f) {
        inverse = 1.0f / state->count;
        {
            Game10A350Emission packet;
            f32 stepX;
            f32 stepZ;
            f32 sine1;
            f32 cosine1;
            f32 sine2;
            f32 cosine2;
            u8 angle;
            s32 ok;

            scale = D_800BE9A4;
            stepScale = D_800BE9A4 * inverse;
            x = state->x;
            z = state->z;
            dx = ((func_151423D8((u8)(state->angleX - 0x40)) * state->swayX) + arg0->x) - state->x;
            dz = ((func_151423D8((u8)(state->angleZ - 0x40)) * state->swayZ) + arg0->z) - state->z;
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
                        ok = 1;
                        packet.frame_rate = state->frameRate;
                        packet.texture = state->texture;
                        packet.position[1] = (s16)arg0->y;
                        packet.lifetime = state->lifetime;
                        do {
                            speed = func_150ADA68() * state->speedRange + state->speedBase;
                            angle = func_150ADA20();
                            sine1 = func_151423D8(angle);
                            cosine1 = func_151423D8((u8)(angle - 0x40));
                            angle2 = ((func_150ADA20() % (u32)(state->pitchRange + 1)) + state->pitchBase);
                            sine2 = func_151423D8(angle2);
                            cosine2 = func_151423D8((u8)(angle2 - 0x40));
                            packet.position[0] = (s16)(arg0->x + state->radius * cosine1);
                            packet.position[2] = (s16)(arg0->z + state->radius * sine1);
                            packet.velocity_x = (s16)(speed * sine2 * cosine1);
                            packet.velocity_y = (s16)(-speed * cosine2);
                            packet.velocity_z = (s16)(speed * sine2 * sine1);
                            packet.acceleration = (s16)(func_150ADA68() * state->accelRange + state->accelBase);
                            packet.fade = (func_150ADA20() % (u32)(state->fadeRange + 1)) + state->fadeBase;
                            packet.width = packet.height = (s32)(func_150ADA68() * state->sizeRange + state->sizeBase);
                            if (func_15167D84(&packet, 0, 0, -1, arg0->fieldC, arg0->field1) == NULL) {
                                ok = 0;
                            }
                            state->spawnCount -= 1.0f;
                        } while (state->spawnCount > 1.0f && ok != 0);
                        while (state->spawnCount > 1.0f) {
                            state->spawnCount -= 1.0f;
                        }
                    }
                    records[arg0->tail].x = x;
                    records[arg0->tail].y = arg0->y;
                    records[arg0->tail].z = z;
                    records[arg0->tail].speed = state->speed;
                    state->widthAngle += func_150ADA20() % (u32)(state->widthAngleRange + 1);
                    records[arg0->tail].width = func_151423D8((u8)(state->widthAngle - 0x40)) * state->widthRange + state->widthBase;
                    records[arg0->tail].alpha = state->alpha;
                    records[arg0->tail].color = state->color;
                    state->color += state->colorStep + (func_150ADA20() % (u32)(state->colorStepRange + 1));
                    records[arg0->tail].speed += state->gravity * scale;
                    records[arg0->tail].y += records[arg0->tail].speed * scale;
                    arg0->tail++;
                    if (arg0->tail == arg0->capacity) {
                        arg0->tail = 0;
                    }
                    arg0->count++;
                    if (arg0->tail == arg0->head) {
                        arg0->head++;
                        if (arg0->head == arg0->capacity) {
                            arg0->head = 0;
                        }
                        arg0->count--;
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
        index = arg0->tail - 1;
        if (index < 0) {
            index = arg0->capacity - 1;
        }
        records[index].alpha = 0;
        index--;
        if (index < 0) {
            index = arg0->capacity - 1;
        }
        records[index].alpha = 0x3F;
        index--;
        if (index < 0) {
            index = arg0->capacity - 1;
        }
        records[index].alpha = 0x7F;
        index--;
        if (index < 0) {
            index = arg0->capacity - 1;
        }
        records[index].alpha = 0xBE;
        arg0->active = 0;
        arg0->flags &= ~2;
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
void *memcpy(void *, const void *, u32);

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
void func_150DDED0(void *arg0);

s32 func_150DBD70(u8);
s32 func_150DDF88(u8 *arg0);
s32 func_150DDFAC(EffectSpawnSource *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5);
s32 func_150DE12C(EffectSpawnSource *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5);
s32 func_150DE2A4(void *arg0);
void func_150DE2C4(void *arg0);
