#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_proto
#include "functions.h"
#undef func_150ADA20
#include "variables.h"


typedef struct Game174BF0Actor {
    u8 pad0[0x90];
    s8 group, phase;
    s16 flags;
    u8 *model;
    f32 x, y, z;
    s16 timer, enabled;
    f32 pitch, yaw, roll, velocity, yawStep, fieldBC, scale;
    u8 padC4[4];
    f32 baseHeight;
    u8 padCC[0x1C];
    u32 surfaceFlags;
    u8 brightness;
    s8 modelIndex;
    u16 sound;
    s8 opacity, fieldF1;
} Game174BF0Actor;

typedef struct Game174FA4Surface {
    s32 height;
    u8 pad04[0xC];
} Game174FA4Surface;

void *func_15167A68(s32, s32, s32, s32, u8, u8);
u32 func_150ADA20(void);
u16 func_10010E78(u16 arg0, s32 arg1, u16 arg2, s32 arg3, s32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9,
                   s32 argA);
s32 func_1510F8D8(s32, s32, s32, s32 *);
extern u8 D_800A7170[];
extern s32 D_800DDE88[];

void func_15174BF0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                   f32 arg6, f32 arg7, s32 arg8) {
    Game174BF0Actor *actor;
    f32 height;
    s32 x;
    s32 z;
    s32 angle;
    s32 yaw;
    s32 index;
    s32 flag;
    s32 sel;
    s32 off;
    struct17 diff;
    u32 sflags;
    s32 surface;

    height = (f32)arg3;
    diff.unk0 = (f32)arg2 - *(f32 *)((u8 *)D_800DBFF0 + arg5 * 0x9A0 + 0x2F8);
    diff.unk4 = height - *(f32 *)((u8 *)D_800DBFF0 + arg5 * 0x9A0 + 0x2FC);
    diff.unk8 = (f32)arg4 - *(f32 *)((u8 *)D_800DBFF0 + arg5 * 0x9A0 + 0x300);
    angle = (s32)func_15048FC8(&diff);
    index = 0;
    if (arg0 > 0) {
        do {
            actor = func_15167A68(0xA, 0, 0xF8, 1, 0xFF, 1);
            if (actor == NULL) return;
            flag = arg8 & 1;
            x = (s32)(func_150ADA20() & 0x7F) + arg2 - 0x3F;
            z = (s32)(func_150ADA20() & 0x7F) + arg4 - 0x3F;
            actor->group = arg1;
            actor->flags = 3;
            actor->y = height;
            actor->x = (f32)x;
            actor->z = (f32)z;
            actor->timer = func_150ADA20() % 15U + 0xA;
            actor->enabled = 1;
            yaw = (s32)(func_150ADA20() & 0x1F) + angle - 0xF;
            if (yaw < 0) {
                yaw += 360;
            } else if (yaw >= 360) {
                yaw -= 360;
            }
            actor->pitch = (f32)((s32)(func_150ADA20() & 0x1F) - 0xF);
            actor->yaw = (f32)yaw;
            actor->roll = 0.0f;
            actor->velocity = -1.0f;
            actor->yawStep = (f32)((s32)(func_150ADA20() & 0xFF) - 0x80) * 0.009999999776f;
            actor->fieldBC = 0.0f;
            actor->scale = (f32)(s32)(func_150ADA20() & 0xFFFF) * 0.000030517578125f * arg7 + arg6;
            sel = func_150ADA20() & 1;
            actor->model = (u8 *)*(s32 *)((u8 *)D_800DDE88 + sel * 20);
            actor->modelIndex = sel;
            if (index == 0) {
                actor->sound = func_10010E78(0, 0xA2, 0x7D00, 0, 0, 0, x, arg3, z, 0x3E8, 0xFA0);
            } else {
                actor->sound = 0;
            }
            actor->surfaceFlags = 0;
            if (flag) {
                actor->flags |= 0x80;
                actor->baseHeight = (f32)func_1510F8D8(x, arg3, z, &surface);
                if (surface != 0) {
                    off = (surface - D_800DBE3C) / 12;
                    sflags = ((u32 *)D_800DBE5C)[off];
                } else {
                    sflags = 0;
                }
                actor->surfaceFlags = sflags;
                actor->brightness = D_800A7170[(sflags >> 5) & 3];
            } else {
                actor->brightness = 0;
            }
            actor->opacity = 0;
            actor->fieldF1 = 0;
            index++;
        } while (index != arg0);
    }
}

void *func_15167A68(s32, s32, s32, s32, u8, u8);
s32 func_150A3A70(s32, s32);
void func_1510F800(s32);
extern s32 D_800DDF14;
u32 func_150ADA20(void);
#define SURF ((Game174FA4Surface *)&D_800D3300)

void func_15174FA4(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                   s32 arg4, s32 arg5, s32 arg6, s32 arg7,
                   f32 arg8, f32 arg9) {
    f32 height;
    s32 initialX;
    s32 initialHeight;
    s32 x;
    s32 z;
    s32 radius;
    s32 minimum;
    s32 count;
    s32 i;
    s32 index;
    Game174BF0Actor *actor;

    initialX = arg2;
    initialHeight = arg3;
    if (arg7 == 0) func_1510F800(0);
    index = 0;
    if (arg0 > 0) {
        do {
            actor = func_15167A68(0x13, 0, 0xF8, 1, 0xFF, 1);
            if (actor == NULL) return;
            radius = arg5 >> 1;
            height = (f32)initialHeight;
            x = (func_150ADA20() % (u32)arg5) + initialX - radius;
            z = (func_150ADA20() % (u32)arg5) + arg4 - radius;
            actor->group = arg1;
            actor->x = (f32)x;
            actor->y = height;
            actor->z = (f32)z;
            actor->timer = func_150ADA20() % 3U + 4;
            actor->enabled = 0;
            actor->pitch = (f32)((s32)(func_150ADA20() & 0x1F) - 0xF);
            actor->yaw = (f32)(func_150ADA20() % 360U);
            actor->roll = (f32)((s32)(func_150ADA20() & 0x1F) - 0xF);
            actor->velocity = 0.0f;
            actor->fieldBC = 0.0f;
            actor->yawStep = 3.0f;
            actor->scale = (f32)(s32)(func_150ADA20() & 0xFFFF) * 0.000030517578125f * arg9 + arg8;
            actor->model = (u8 *)D_800DDF14;
            actor->modelIndex = 7;
            count = func_150A3A70(x, z);
            if (count == 0) {
                func_1516979C((struct102 *)actor);
            } else {
                minimum = SURF[0].height;
                for (i = 1; i < count; i++) {
                    if (SURF[i].height < minimum) {
                        minimum = SURF[i].height;
                    }
                }
                minimum >>= 8;
                actor->baseHeight = (f32)minimum;
                if (arg7 == 0) {
                    actor->phase = 0;
                    actor->y = (f32)(minimum + 10);
                    actor->flags = 1;
                } else {
                    actor->phase = 2;
                    actor->flags = 4;
                    *(f32 *)((u8 *)actor + 0xE4) = (f32)((s32)(func_150ADA20() & 0x7F) + minimum + 300);
                    actor->y = (f32)((s32)(func_150ADA20() & 0x7F) + minimum + 300);
                }
                *(f32 *)((u8 *)actor + 0xCC) = 0;
                *(s16 *)((u8 *)actor + 0xD0) = initialX;
                *(s16 *)((u8 *)actor + 0xD4) = arg4;
                *(s16 *)((u8 *)actor + 0xD6) = radius;
                *(f32 *)((u8 *)actor + 0xE0) = 0;
                *(s16 *)((u8 *)actor + 0xDC) = 0;
                actor->surfaceFlags = 0;
                actor->brightness = 0;
                actor->opacity = 0;
                actor->fieldF1 = 0;
            }
            index++;
        } while (index != arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_15175390.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_15175958.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_1517685C.s")

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Struct15176B84;

typedef struct {
    u8  pad0[0x90];
    u8  unk90;
    u8  unk91;
    u8  pad92[0x2];
    s32 unk94;
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    u8  padA4[0x24];
    f32 unkC8;
    u8  padCC[0x6];
    s16 unkD2;
    u8  padD4[0x6];
    s16 unkDA;
    u8  padDC[0x8];
    f32 unkE4;
    u8  padE8[0x5];
    u8  unkED;
    u8  padEE[0x2];
    u8  unkF0;
    u8  unkF1;
} Obj15176B84;

s32 func_150AD960(s32, s32, s32, s32);
void func_15175958(Obj15176B84 *arg0);

void func_15176B84(Obj15176B84 *obj) {
    s32 fade;

    if ((((Struct15176B84 *)D_800DDE80)[obj->unkED].unk0 == 0) || (D_800DDF78[obj->unkED] == 0)) {
        func_1516972C((struct102 *)obj);
        return;
    }

    fade = obj->unkF0;
    if (obj->unkF1 == 0) {
        if (D_800DDF5C[obj->unk90] == 0) {
            obj->unkF1 = 1;
        } else if (fade != 0xFF) {
            fade += D_800BE9E4 * 4;
            if (fade >= 0x100) {
                fade = 0xFF;
            }
            obj->unkF0 = fade;
        }
    } else {
        if (fade == 0) {
            func_1516972C((struct102 *)obj);
            return;
        }
        fade -= D_800BE9E4 * 4;
        if (fade < 0) {
            fade = 0;
        }
        obj->unkF0 = fade;
    }

    D_800DDF68[obj->unkED] = 1;

    switch (obj->unk91) {
        case 0:
            if (func_150AD960((s32)D_800CC2D0[0].x_position, (s32)D_800CC2D0[0].z_position, (s32)obj->unk98,
                              (s32)obj->unkA0) < 0xC8) {
                obj->unk91 = 2;
                obj->unkE4 = (f32)((u32)func_150ADA20() % obj->unkDA) + obj->unkD2;
                obj->unk94 = ((Struct15176B84 *)D_800DDE80)[obj->unkED].unk8;
            } else {
                obj->unk94 = ((Struct15176B84 *)D_800DDE80)[obj->unkED].unk4;
            }
            return;
        case 2:
        case 3:
            break;
        case 4:
            if (obj->unk9C < obj->unkC8) {
                func_1516972C((struct102 *)obj);
                return;
            }
            break;
    }
    func_15175958(obj);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_15176DF0.s")

typedef struct Game1770C8Packet {
    f32 m[4][4];
} Game1770C8Packet;

s32 func_1510AEE0(f32 *, f32, f32, f32, f32, f32, f32, f32, f32 *, f32 *);
void *func_15176DF0(u32 *, u8 *, s16);
extern f32 D_800D35E0;
extern f32 D_800D35E8; /* (&D_800D35E8)[-1] == 0x800D35E4, which has no symbol of its own */
extern u8 D_800D9C10[];

void *func_151770C8(u32 *gdl, u8 *arg1, s16 arg2) {
    Game1770C8Packet packet;
    s32 culled;

    culled = func_1510AEE0((f32 *)(D_800D9C10 + (arg2 << 6)),
                      *(f32 *)(arg1 + 0x98), *(f32 *)(arg1 + 0x9C), *(f32 *)(arg1 + 0xA0),
                      D_800D9B20, 4000.0f, D_800D35E0, (&D_800D35E8)[-1], 0, 0);
    if (culled == 0) {
        switch (arg1[0x91]) {
        case 0:
            func_150A8050(packet.m, *(f32 *)(arg1 + 0xA8), *(f32 *)(arg1 + 0xAC), *(f32 *)(arg1 + 0xB0));
            packet.m[3][0] = *(f32 *)(arg1 + 0x98);
            packet.m[3][1] = *(f32 *)(arg1 + 0x9C);
            packet.m[3][2] = *(f32 *)(arg1 + 0xA0);
            guMtxF2L(packet.m, (Mtx *)((arg1 + 0x10) + (D_800BE9C0 << 6)));
            { u32 *g = gdl; gdl += 2; g[0] = 0xDA380003; g[1] = (u32)((arg1 + 0x10) + (D_800BE9C0 << 6)); }
            { u32 *g = gdl; gdl += 2; g[0] = 0xDB060004; g[1] = *(u32 *)((u8 *)D_800DDE80 + 0xCC); }
            { u32 *g = gdl; gdl += 2; g[0] = 0xDE000000; g[1] = *(u32 *)((u8 *)D_800DDE80 + 0xD4); }
            break;
        case 1:
        case 2:
        case 3:
        default:
            gdl = func_15176DF0(gdl, arg1, arg2);
            break;
        }
    }
    return gdl;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_1517725C.s")
