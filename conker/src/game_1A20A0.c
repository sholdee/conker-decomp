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

typedef struct Game175390Actor {
    u8 pad0[0x90];
    s8 group, phase;
    s16 flags;
    u8 *model;
    f32 x, y, z;
    s16 timer, enabled;
    f32 pitch, yaw, roll, velocity, yawStep, fieldBC, scale;
    u8 padC4[4];
    f32 baseHeight;
    u8 padCC[4];
    s16 originX, originY, originZ;
    s16 range, minimum, spread;
    s16 fieldDC, fieldDE;
    f32 fieldE0;
    u8 padE4[4];
    u32 surfaceFlags;
    u8 brightness;
    s8 modelIndex;
    u16 sound;
    s8 opacity, fieldF1;
} Game175390Actor;

void func_15175390(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, u32 argB,
                   s32 argC, s32 argD, s32 argE, f32 argF, f32 arg10, s32 arg11) {
    Game175390Actor *actor;
    s32 x;
    s32 y;
    s32 z;
    s32 index;
    s32 yaw;
    s32 angle;
    s32 range;
    s32 groundFlag;
    struct17 diff;
    u32 sel;
    s32 surface;

    if ((arg9 & 0x40) && arg7 <= 0) {
        return;
    }
    if (arg9 & 0x10000) {
        diff.unk0 = (f32)arg2 - *(f32 *)((u8 *)D_800DBFF0 + arg6 * 0x9A0 + 0x2F8);
        diff.unk4 = (f32)arg3 - *(f32 *)((u8 *)D_800DBFF0 + arg6 * 0x9A0 + 0x2FC);
        diff.unk8 = (f32)arg4 - *(f32 *)((u8 *)D_800DBFF0 + arg6 * 0x9A0 + 0x300);
        angle = (s32)func_15048FC8(&diff);
    }
    for (index = 0; index < arg0; index++) {
        if (argB != 0) {
            sel = func_150ADA20() % argB;
        } else {
            sel = 0;
        }
        actor = func_15167A68(argA + sel, 0, 0xF8, 1, 0xFF, 1);
        if (actor == NULL) return;
        range = arg5 - arg8;
        groundFlag = arg11 & 1;
        x = (s32)func_150ADA20() % range;
        x = (x < 0) ? x - arg8 : x + arg8;
        x += arg2;
        y = arg3;
        z = (s32)func_150ADA20() % range;
        z = (z < 0) ? z - arg8 : z + arg8;
        z += arg4;
        if (argE == 0 || groundFlag) {
            actor->baseHeight = (f32)func_1510F8D8(x, arg3, z, &surface);
        }
        if (argE == 0) {
            y = (s32)actor->baseHeight;
        } else if (arg7 != 0) {
            y += func_150ADA20() % (u32)arg7;
        }
        actor->group = arg1;
        actor->flags = arg9;
        actor->phase = argE;
        actor->x = (f32)x;
        actor->y = (f32)y;
        actor->z = (f32)z;
        actor->timer = func_150ADA20() % 15U + 0xA;
        actor->enabled = 0;
        if (arg9 & 0x10000) {
            yaw = (s32)(func_150ADA20() & 0x1F) + angle - 0xF;
            if (yaw < 0) {
                yaw += 360;
            } else if (yaw >= 360) {
                yaw -= 360;
            }
        } else {
            yaw = func_150ADA20() % 360U;
        }
        if (arg9 & 0x20000) {
            actor->pitch = (f32)((s32)(func_150ADA20() & 0x1F) - 0xF);
            actor->velocity = -1.0f;
        } else {
            actor->pitch = 0.0f;
            actor->velocity = 0.0f;
        }
        actor->roll = 0.0f;
        actor->yaw = (f32)yaw;
        actor->yawStep = (f32)((s32)(func_150ADA20() & 0xFF) - 0x80) * 0.009999999776f;
        actor->fieldBC = 0.0f;
        actor->scale = (f32)(s32)(func_150ADA20() & 0xFFFF) * 0.000030517578125f * arg10 + argF;
        if (argD != -1) {
            if (argD != 0) {
                sel = func_150ADA20() & argD;
            } else {
                sel = 0;
            }
        }
        sel += argC;
        actor->model = (u8 *)*(s32 *)((u8 *)D_800DDE88 + sel * 20);
        actor->modelIndex = sel;
        actor->originX = arg2;
        actor->originY = arg3;
        actor->originZ = arg4;
        actor->range = arg5;
        actor->minimum = arg8;
        actor->spread = arg7;
        actor->fieldDC = 0;
        actor->fieldDE = 0;
        actor->surfaceFlags = 0;
        actor->fieldE0 = 0;
        if (index == 0) {
            if (argA == 10) {
                actor->sound = func_10010E78(0, 0xA2, 0x7D00, 0, 0, 0, x, y, z, 0x3E8, 0xFA0);
            }
        } else {
            actor->sound = 0;
        }
        if (groundFlag) {
            actor->flags |= 0x80;
            if (surface != 0) {
                range = (surface - D_800DBE3C) / 12;
                groundFlag = ((u32 *)D_800DBE5C)[range];
            } else {
                groundFlag = 0;
            }
            actor->surfaceFlags = groundFlag;
            actor->brightness = D_800A7170[((u32)groundFlag >> 5) & 3];
        } else {
            actor->brightness = 0;
        }
        actor->opacity = 0;
        actor->fieldF1 = 0;
    }
}

typedef struct Wave1A20A0 {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    f32 unk4;
    f32 unk8;
    u8 padC[4];
    f32 unk10;
    u8 pad14[4];
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    u8 pad2C[4];
    f32 unk30;
    f32 unk34;
} Wave1A20A0;

extern Wave1A20A0 D_8008D0B0[];
typedef struct Obj15175958 {
    u8 type;
    u8 pad1[0x8F];
    u8 unk90;
    u8 unk91;
    s16 unk92;
    s32 unk94;
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    s16 unkA4;
    s16 unkA6;
    f32 unkA8;
    f32 unkAC;
    f32 unkB0;
    f32 unkB4;
    f32 unkB8;
    f32 unkBC;
    u8 padC0[8];
    f32 unkC8;
    u8 padCC[4];
    s16 unkD0;
    s16 unkD2;
    s16 unkD4;
    s16 unkD6;
    s16 unkD8;
    s16 unkDA;
    s16 unkDC;
    s16 unkDE;
    f32 unkE0;
    f32 unkE4;
    s32 unkE8;
    u8 padEC;
    u8 unkED;
    u16 unkEE;
    u8 unkF0;
    u8 unkF1;
} Obj15175958;

typedef struct Slot15175958 {
    void *unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Slot15175958;

extern s32 D_800C35B0;
extern u8 D_800C35E8;
extern f32 D_800D9C50;
extern u8 D_800D9C10[];
extern f32 D_800D35E0;
extern f32 D_800D35E8;
s32 func_150490A8(struct17 *arg0);
s32 func_150AD960(s32, s32, s32, s32);
s32 func_1510AEE0(f32 *, f32, f32, f32, f32, f32, f32, f32, f32 *, f32 *);

void func_15175958(Obj15175958 *arg0) {
    s32 pad0;
    f32 dir[3];
    f32 ang[3];
    f32 sa;
    f32 sb;
    f32 ca;
    f32 cb;
    s32 off;
    f32 step;
    f32 lim;
    s32 sflags;
    s32 angle;
    s32 dist;
    struct17 vec;
    s32 surface;
    Wave1A20A0 *wave;
    Slot15175958 *slot;
    s32 fade;
    f32 dx;
    f32 dz;
    u16 snd;

    if (D_800DDE80[arg0->unkED].field0 == NULL || D_800DDF78[arg0->unkED] == 0) {
        func_1516972C((struct102 *)arg0);
        return;
    }
    fade = arg0->unkF0;
    if ((arg0->unkED == 8 || arg0->unkED == 9) && D_800BE9F0 == 6 && D_800C35EA != 0 && D_800C35E8 == 0xF) {
        arg0->unk92 &= ~8;
        if (D_800C35EA == 0 || D_800C35E8 != 0xF) {
            func_1516972C((struct102 *)arg0);
            return;
        }
        if (D_800C35B0 >= 60 && !(arg0->unk92 & 0x200)) {
            arg0->unk92 = (arg0->unk92 & ~0x4C) | 0x200;
            dx = arg0->unk98 - -3478.0f;
            dz = arg0->unkA0 - 3858.0f;
            arg0->unkB4 = 1.570796371f - func_150484A0(dx, dz);
            arg0->unkB8 = 0.01150000002f;
            arg0->unkBC = sqrtf(dx * dx + dz * dz);
        }
    }
    if (arg0->unkF1 == 0) {
        if ((arg0->unk92 & 8) && D_800DDF5C[arg0->unk90] == 0) {
            arg0->unkF1 = 1;
        } else if (fade != 0xFF) {
            fade += D_800BE9E4 * 4;
            if (fade >= 0x100) {
                fade = 0xFF;
            }
            arg0->unkF0 = fade;
        }
    } else {
        if (fade == 0) {
            func_1516972C((struct102 *)arg0);
            return;
        }
        fade -= D_800BE9E4 * 4;
        if (fade < 0) {
            fade = 0;
        }
        arg0->unkF0 = fade;
    }
    slot = (Slot15175958 *)&D_800DDE80[arg0->unkED];
    wave = &D_8008D0B0[arg0->unkED];
    D_800DDF68[arg0->unkED] = 1;
    snd = arg0->unkEE;
    if (snd != 0) {
        if (func_1000F3D0(snd) != 0) {
            func_1000F91C(arg0->unkEE, 0x7D00, 0, 0, 0, arg0->unk98, arg0->unk9C, arg0->unkA0, 1000, 4000);
        } else {
            arg0->unkEE = 0;
        }
    }
    if (arg0->unk92 & 4) {
        s32 flag;

        dist = func_150AD960((s32)arg0->unk98, (s32)arg0->unkA0, arg0->unkD0, arg0->unkD4);
        flag = ((arg0->unk92 & 0x10) && dist < arg0->unkD8) ? 1 : 0;
        if (arg0->unkD6 < dist || flag) {
            vec.unk0 = arg0->unkD0 - arg0->unk98;
            vec.unk8 = arg0->unkA0 - arg0->unkD4;
            angle = func_150490A8(&vec);
            if (flag) {
                angle += 0x80;
            }
            if (angle >= 0x100) {
                angle -= 0x100;
            }
            arg0->unkE0 = angle * 1.40625f;
            arg0->unkDC = wave->b3;
            arg0->unk91 = 3;
        }
        if (arg0->unk91 == 3) {
            step = D_800BE9E4 * 0.5f;
            if (arg0->unkE0 < arg0->unkAC) {
                step = -step;
            }
            if (fabsf(arg0->unkE0 - arg0->unkAC) >= 180.0f) {
                step = -step;
            }
            if ((step > 0.0f && arg0->unkB8 < 0.0f) || (step < 0.0f && arg0->unkB8 > 0.0f)) {
                arg0->unk91 = 2;
            } else {
                arg0->unkB8 += step;
            }
        }
        if (arg0->unk91 == 2) {
            if (arg0->unkB8 != 0.0f) {
                if (arg0->unkB8 > 0.0f) {
                    arg0->unkB8 -= D_800BE9E4 * 0.5f;
                    if (arg0->unkB8 < 0.0f) {
                        arg0->unkB8 = 0.0f;
                    }
                } else {
                    arg0->unkB8 += D_800BE9E4 * 0.5f;
                    if (arg0->unkB8 > 0.0f) {
                        arg0->unkB8 = 0.0f;
                    }
                }
            }
            arg0->unkDC -= D_800BE9E4;
            if (arg0->unkDC <= 0) {
                arg0->unkE0 = func_150ADA20() % 360;
                arg0->unkDC = wave->b2;
                arg0->unk91 = 3;
            }
        }
        if (wave->unk10 < fabsf(arg0->unkB8)) {
            if (arg0->unkB8 > 0.0f) {
                arg0->unkB8 = wave->unk10;
            } else {
                arg0->unkB8 = -wave->unk10;
            }
        }
    }
    if (arg0->unk92 & 0x80) {
        arg0->unkC8 = func_1510F8D8(arg0->unk98, arg0->unk9C, arg0->unkA0, &surface);
        if (surface != 0) {
            off = (surface - D_800DBE3C) / 12;
            sflags = ((u32 *)D_800DBE5C)[off];
        } else {
            sflags = 0;
        }
        arg0->unkE8 = sflags;
    }
    if (arg0->unk92 & 0x40) {
        if (arg0->unkDE >= 0) {
            arg0->unkDE -= D_800BE9E4;
            if (arg0->unkDE <= 0) {
                arg0->unkDE = -1;
                arg0->unkE4 = (f32)(func_150ADA20() % arg0->unkDA) + arg0->unkD2;
                if (arg0->unk9C < arg0->unkE4) {
                    arg0->unkB4 = -1.0f;
                } else {
                    arg0->unkB4 = 1.0f;
                }
            } else if (arg0->unkA8 > 0.0f) {
                arg0->unkB4 = -1.0f;
            } else if (arg0->unkA8 < 0.0f) {
                arg0->unkB4 = 1.0f;
            }
        } else if ((arg0->unkB4 > 0.0f && arg0->unk9C < arg0->unkE4) ||
                   (arg0->unkB4 < 0.0f && arg0->unkE4 < arg0->unk9C)) {
            arg0->unkDE = 120;
            arg0->unkB4 = -arg0->unkB4;
        }
    }
    if (!(arg0->unk92 & 0x200)) {
        arg0->unkA8 += arg0->unkB4;
        arg0->unkA8 = (arg0->unkA8 < -wave->unk8) ? -wave->unk8 : ((wave->unk8 < arg0->unkA8) ? wave->unk8 : arg0->unkA8);
        if (0.200000003f < fabsf(arg0->unkB8)) {
            arg0->unkAC += arg0->unkB8;
            if (arg0->unkAC < 0.0f) {
                arg0->unkAC += 360.0f;
            } else if (arg0->unkAC >= 360.0f) {
                arg0->unkAC -= 360.0f;
            }
            if (!(arg0->unk92 & 0x20)) {
                arg0->unkBC = -arg0->unkB8;
                arg0->unkB0 += arg0->unkBC;
                arg0->unkB0 = (arg0->unkB0 < -wave->unk4) ? -wave->unk4 : ((wave->unk4 < arg0->unkB0) ? wave->unk4 : arg0->unkB0);
            }
        } else {
            if (arg0->unkB0 > 0) {
                arg0->unkBC = -1.0f;
            } else if (arg0->unkB0 < 0) {
                arg0->unkBC = 1.0f;
            }
            arg0->unkB0 += arg0->unkBC;
            if (fabsf(arg0->unkB0) < 0.200000003f) {
                arg0->unkB0 = 0;
            }
        }
        arg0->unkA4 += arg0->unkA6;
        arg0->unkA4 = (arg0->unkA4 < wave->b1) ? wave->b1 : ((wave->b0 < arg0->unkA4) ? wave->b0 : arg0->unkA4);
    }
    if (!(arg0->unk92 & 1)) {
        if (arg0->unkA8 >= 0) {
            lim = wave->unk20;
            if (arg0->unk94 == slot->unk4 && wave->unk18 < lim && -lim < wave->unk18) {
                arg0->unk94 = slot->unk8;
            }
        } else {
            lim = wave->unk20;
            if (arg0->unk94 == slot->unk8 && wave->unk18 < lim && -lim < wave->unk18) {
                arg0->unk94 = slot->unk4;
            }
        }
    } else {
    }
    if (arg0->unk92 & 0x200) {
        f32 speed;
        speed = D_800BE9E4;
        arg0->unkB4 += arg0->unkB8 * speed;
        arg0->unkB8 += 0.005189999938f;
        if (0.1499999911f < arg0->unkB8) {
            arg0->unkB8 = 0.1499999911f;
        }
        arg0->unkBC -= 3.200000048f * speed;
        if (arg0->unkBC < 60.0f) {
            arg0->unkBC = 60.0f;
        }
        sa = func_150AD78C(arg0->unkB4);
        ca = func_150AD780(arg0->unkB4);
        arg0->unk98 = arg0->unkBC * ca + -3478.0f;
        arg0->unkA0 = arg0->unkBC * sa + 3858.0f;
        arg0->unk9C -= 100.0f / (arg0->unkBC - 60.0f + 4.0f);
        if (arg0->unk9C < -3100.0f || D_800C35EA == 0 || D_800C35E8 != 0xF) {
            func_1516972C((struct102 *)arg0);
            return;
        }
    } else {
        s32 speed;
        ang[0] = arg0->unkA8 * 0.01745329238f;
        ang[1] = arg0->unkAC * 0.01745329238f;
        ang[2] = arg0->unkB0 * 0.01745329238f;
        sa = func_150AD78C(ang[0]);
        sb = func_150AD78C(ang[1]);
        ca = func_150AD780(ang[0]);
        cb = func_150AD780(ang[1]);
        dir[0] = ca * sb;
        dir[1] = -sa;
        dir[2] = ca * cb;
        speed = arg0->unkA4 * D_800BE9E4;
        arg0->unk98 += dir[0] * speed;
        arg0->unk9C += dir[1] * speed;
        arg0->unkA0 += dir[2] * speed;
    }
    if (arg0->type == 0x13 && D_800DDF60[arg0->unk90] == 0) {
        func_10010F88(0x10, 0x7FFF, func_150ADA20() & 0x7F, 0, 0, arg0->unk98, arg0->unk9C, arg0->unkA0, 500, 4000);
        D_800DDF60[arg0->unk90] = (func_150ADA20() & 0x3F) + 30;
    }
    if ((arg0->unk92 & 2) &&
        func_1510AEE0((f32 *)D_800D9C10, arg0->unk98, arg0->unk9C, arg0->unkA0, D_800D9B20, D_800D9B1C, D_800D35E0, (&D_800D35E8)[-1], NULL, NULL) != 0 &&
        (D_800D2138 != 1 ||
         func_1510AEE0(&D_800D9C50, arg0->unk98, arg0->unk9C, arg0->unkA0, D_800D9B20, D_800D9B1C, D_800D35E0, (&D_800D35E8)[-1], NULL, NULL) != 0)) {
        func_1516972C((struct102 *)arg0);
    }
}

typedef struct {
    u8 pad0[0x90];
    u8 unk90;
    u8 unk91;
    s16 unk92;
    u8 pad94[4];
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    u8 padA4[4];
    f32 unkA8;
    f32 unkAC;
    u8 padB0[4];
    f32 unkB4;
    f32 unkB8;
    u8 padBC[0xC];
    f32 unkC8;
    f32 unkCC;
    u8 padD0[0x14];
    f32 unkE4;
    u8 padE8[5];
    u8 unkED;
} struct1517685C;

s32 func_150AD960(s32, s32, s32, s32);
void func_15175958();
u32 func_150ADA20(void);

void func_1517685C(struct1517685C *arg0) {
    s32 r;
    f32 cc;
    s32 t;

    if (D_800DDF5C[arg0->unk90] != 0) {
        D_800DDF68[arg0->unkED] = 1;
        switch (arg0->unk91) {
        case 0:
            if (func_150AD960((s32)D_800CC2D0->x_position, (s32)D_800CC2D0->z_position, (s32)arg0->unk98, (s32)arg0->unkA0) < 400) {
                arg0->unk91 = 1;
                arg0->unkB4 = -1 - (s32)(func_150ADA20() & 3); arg0->unkA8 = -8.0f;
                return;
            }
            {
                f32 a8;

                if (arg0->unkCC != 0.0f) {
                    arg0->unkA8 = arg0->unkA8 + arg0->unkCC;
                    if (arg0->unkCC < 0.0f) {
                        arg0->unkAC = arg0->unkAC + arg0->unkB8;
                    }
                    a8 = arg0->unkA8;
                    if (a8 > 45.0f) {
                        arg0->unkCC = -5.0f;
                    } else if ((arg0->unkCC < 0) && (a8 > 25.0f)) {
                        r = func_150ADA20() & 0xFF;
                        if (r < 0x33) {
                            arg0->unkCC = 5.0f;
                        }
                    }
                    if (arg0->unkA8 <= 10.0f) {
                        arg0->unkCC = 0.0f;
                        return;
                    }
                } else {
                    r = func_150ADA20() & 0xFF;
                    if (r < 0x19) {
                        arg0->unkCC = 5.0f;
                        if (r < 0xD) {
                            arg0->unkB8 = (s32)(func_150ADA20() % 6U) - 3;
                            return;
                        }
                        arg0->unkB8 = 0.0f;
                    lbl1:
                        return;
                    }
                }
            }
            break;
        case 1:
        lbl2:
            if (arg0->unk9C >= (300.0f + arg0->unkC8)) {
                arg0->unk91 = 2;
                arg0->unk92 &= ~1;
                arg0->unk92 |= 4;
                arg0->unkE4 = (s32)(func_150ADA20() & 0x7F) + (300.0f + arg0->unkC8);
            }
            func_15175958(arg0);
            return;
        default:
        case 2:
        case 3:
            if (arg0->unk9C < arg0->unkE4) {
                arg0->unkB4 = -1.5f;
            } else if (arg0->unkA8 < 0) {
                arg0->unkB4 = 1.5f;
            }
            func_15175958(arg0);
            return;
        }
    } else {
        func_1516972C((struct102 *)arg0);
    }
}

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
void func_15175958();

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

void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

void *func_15176DF0(Gfx *gdl, u8 *arg1, s16 arg2) {
    s32 pad;
    f32 rot[4][4];
    f32 mtx[4][4];
    s32 col[3];
    s32 fade;
    s32 target;

    if ((D_800DDE80[arg1[0xED]].field0 == NULL) || (D_800DDF78[arg1[0xED]] == 0)) {
        return gdl;
    }
    func_150A8050(mtx, 0.0f, *(f32 *)(arg1 + 0xAC), 0.0f);
    func_150A8050(rot, *(f32 *)(arg1 + 0xA8), 0.0f, *(f32 *)(arg1 + 0xB0));
    func_150A7A48(rot, mtx, mtx);
    mtx[3][0] = *(f32 *)(arg1 + 0x98);
    mtx[3][1] = *(f32 *)(arg1 + 0x9C);
    mtx[3][2] = *(f32 *)(arg1 + 0xA0);
    mtx[0][0] *= *(f32 *)(arg1 + 0xC0);
    mtx[0][1] *= *(f32 *)(arg1 + 0xC0);
    mtx[0][2] *= *(f32 *)(arg1 + 0xC0);
    mtx[1][0] *= *(f32 *)(arg1 + 0xC0);
    mtx[1][1] *= *(f32 *)(arg1 + 0xC0);
    mtx[1][2] *= *(f32 *)(arg1 + 0xC0);
    mtx[2][0] *= *(f32 *)(arg1 + 0xC0);
    mtx[2][1] *= *(f32 *)(arg1 + 0xC0);
    mtx[2][2] *= *(f32 *)(arg1 + 0xC0);
    guMtxF2L(mtx, (Mtx *)((arg1 + 0x10) + (D_800BE9C0 << 6)));
    gSPMatrix(gdl++, (Mtx *)((arg1 + 0x10) + (D_800BE9C0 << 6)), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(gdl++, 1, *(u32 *)(arg1 + 0x94));
    gDPPipeSync(gdl++);
    fade = arg1[0xEC];
    target = D_800A7170[(*(u32 *)(arg1 + 0xE8) >> 5) & 3];
    if (fade != target) {
        if (fade < target) {
            fade += D_800BE9E4 * 4;
            if (target < fade) {
                fade = target;
            }
        } else {
            fade -= D_800BE9E4 * 4;
            if (fade < 0) {
                fade = 0;
            }
        }
        arg1[0xEC] = fade;
    }
    col[0] = col[1] = col[2] = fade;
    {
    u8 update;
    update = 0;
    gdl = func_15142FBC(gdl, 0x82CA0, 0x504A50, &update);
    gdl->words.w0 = 0xFB000000; gdl->words.w1 = ((col[0] & 0xFF) << 24) | ((col[1] & 0xFF) << 16) | (arg1[0xF0] & 0xFF) | ((col[2] & 0xFF) << 8); gdl++;
    gSPDisplayList(gdl++, D_800DDE80[arg1[0xED]].fieldC);
    }
    return gdl;
}

typedef struct Game1770C8Packet {
    f32 m[4][4];
} Game1770C8Packet;

s32 func_1510AEE0(f32 *, f32, f32, f32, f32, f32, f32, f32, f32 *, f32 *);
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


void func_1517725C(u8 arg0) {
    f32 a;
    f32 s;
    s16 x;
    s16 y;
    Wave1A20A0 *w;
    s16 *v;
    f32 c;

    w = &D_8008D0B0[arg0];
    v = *(s16 **)((u8 *)D_800DDE88 + arg0 * 20);
    a = w->unk24 * 0.01745329238f;
    w->unk18 = func_150AD78C(a) * w->unk28;
    a = w->unk18 * 0.01745329238f;
    s = func_150AD78C(a);
    c = func_150AD780(a);
    x = w->unk30 * c;
    y = w->unk30 * s;
    v[0xA0 / 2] = -x;
    v[0xB0 / 2] = -x;
    v[0x110 / 2] = x;
    v[0x120 / 2] = x;
    v[0x112 / 2] = y;
    v[0x122 / 2] = y;
    v[0xA2 / 2] = y;
    v[0xB2 / 2] = y;
    a = (w->unk24 - 30.0f) * 0.01745329238f;
    w->unk18 = func_150AD78C(a) * w->unk28;
    a = w->unk18 * 0.01745329238f;
    s = func_150AD78C(a);
    c = func_150AD780(a);
    x = w->unk34 * c;
    y = w->unk34 * s;
    v[0xE0 / 2] = -x;
    v[0x130 / 2] = x;
    v[0xE2 / 2] = y;
    v[0x132 / 2] = y;
    w->unk24 += w->unk1C;
    if (w->unk24 >= 360.0f) {
        w->unk24 -= 360.0f;
    }
}
