#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A1050;
extern f32 D_800A1054;
extern void func_1505D1C4(f32, f32, f32, s32, s32, s32, s32, s32);
void func_151C3B0C(s32, f32, f32, f32, f32, s32, s32, s32);
extern s32 func_150AD9A0(s32, s32, s32);
extern u8 *func_150E3020(s32, s32, s32, s32, s32, s32, s32, f32, s32, f32, f32, f32, s32, s16);

typedef struct {
    char pad_0x0[0x30];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    char pad_0x3C[0x24];
    f32 field_0x60;
    f32 field_0x64;
    char pad_0x68[0x3C];
    s32 field_0xA4;
} Game1104D0EffectState;

typedef struct {
    char pad_0x0[0x110];
    Game1104D0EffectState field_0x110;
} Game1104D0EffectObj;

typedef struct {
    u8 field_0x0;
    char pad_0x1[0xF];
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    char pad_0x1C[0xC];
    f32 field_0x28;
    f32 field_0x2C;
    f32 field_0x30;
    char pad_0x34[0x14];
    u8 field_0x48;
    char pad_0x49[0x1];
    u16 field_0x4A;
    f32 field_0x4C;
    char pad_0x50[0x4];
    f32 field_0x54;
} Game1104D0EffectInstance;

typedef struct {
    char pad_0x0[0x10];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    u8 unk48;
    char pad_0x49[0x1];
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    char pad_0x58[0x14];
    s32 unk6C;
    u8 unk70;
    u8 unk71;
    char pad_0x72[0x2];
    s32 unk74;
    s32 unk78;
    s16 unk7C;
} Game1104D0Effect;

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

u8 *func_150E3020(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, f32 arg7, s32 arg8, f32 arg9, f32 argA, f32 argB, s32 argC, s16 argD) {
    Game1104D0Effect *obj;
    s32 i;

    obj = func_15167A68(0x27, 0, 0x80, 1, 0xFF, 1);
    if (obj == NULL) {
        return NULL;
    }
    obj->unk4C = 0.0f;
    obj->unk50 = 0.0f;
    obj->unk54 = -10000.0f;
    obj->unk6C = 0;
    obj->unk70 = 0;
    obj->unk71 = 0;
    obj->unk74 = 0;
    obj->unk7C = argD;
    obj->unk78 = argC;
    obj->unk10 = arg0;
    obj->unk14 = arg1;
    obj->unk18 = arg2;
    obj->unk1C = arg3;
    obj->unk20 = arg4;
    obj->unk24 = arg5;
    obj->unk28 = arg9;
    obj->unk2C = argA;
    obj->unk30 = argB;
    obj->unk34 = arg7;
    obj->unk38 = arg1;
    obj->unk3C = arg8;
    obj->unk40 = arg6;
    obj->unk44 = 0;
    obj->unk4A = 0;
    for (i = 0; i < 8; i++) {
        if (D_800D99D0[i] == 0) {
            D_800D99D0[i] = (s32)obj;
            obj->unk48 = i;
            return (u8 *)obj;
        }
    }
    obj->unk48 = 0;
    return (u8 *)obj;
}

s32 func_150E3208(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    u8 *temp;

    temp = func_150E3020(arg0, arg1, arg2, arg3, arg4, arg5, arg6,
                         (f32)func_150AD9A0(arg0 - arg3, arg1 - arg4, arg2 - arg5) / (f32)arg7,
                         0, 0.0f, 0.0f, 0.0f, 0, -99);
    if (temp != 0) {
        return temp[0x48] + 1;
    }
    return 0;
}

s32 func_150E32D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    u8 *temp;

    temp = func_150E3020(arg0, arg1, arg2, 0, 0, 0, arg4, arg5, arg3, 0.0f, 0.0f, 0.0f, 0, -99);
    if (temp != 0) {
        return temp[0x48] + 1;
    }
    return 0;
}

void func_150E3340(f32 *arg0, s32 *arg1, s32 arg2, s16 arg3) {
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a2;

    temp_a0 = arg1[0];
    temp_a1 = arg1[1];
    temp_a2 = arg1[2];
    func_150E3020(temp_a0, temp_a1, temp_a2, temp_a0, temp_a1, temp_a2, 0x1A, 10.0f, 0, arg0[0], arg0[1], arg0[2], arg2, arg3);
}

void func_1000E7A0(s32, s32);

s32 func_150E33CC(s32 arg0, s32 arg1, void **arg2, s32 arg3) {
    if (*arg2 == NULL) {
        return 1;
    }
    func_1000E7A0(2, (s32)*arg2);
    return 0;
}

s32 func_150E3414(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, f32 arg8) {
    Game1104D0EffectInstance *temp;

    temp = (Game1104D0EffectInstance *)func_150E3020(arg3, arg4, arg5, 0, 0, 0, arg7, arg8, arg6, (f32)arg0, (f32)arg1, (f32)arg2, 0, -99);
    if (temp != 0) {
        temp->field_0x4A = func_1000FA64(0x2D0, (s16)arg0, (s16)arg1, (s16)arg2, 0x5DC0, 0x1770, 0x3E8, (s32)func_150E33CC, 0, 0, 0, 0);
        return temp->field_0x48 + 1;
    }
    return 0;
}

void func_150E3514(Game1104D0EffectInstance *arg0) {
    s32 temp;
    s32 *slot;
    u16 id;

    temp = arg0->field_0x48;
    if (temp >= 0) {
        slot = &D_800D99D0[temp];
        if (*slot == (s32)arg0) {
            *slot = 0;
        }
    }

    id = arg0->field_0x4A;
    if (id != 0) {
        func_100111C8(id);
        arg0->field_0x4A = 0;
        func_10010F88(0x2D7, 0x5DC0, 0, 0, 0,
                      (s32)arg0->field_0x28,
                      (s32)arg0->field_0x2C,
                      (s32)arg0->field_0x30,
                      0x3E8, 0x1770);
    }

    func_1516972C((struct102 *)arg0);
}

extern u8 D_800DCE50[];

typedef struct Node150E35DC {
    u8 pad0[0x8];
    struct Node150E35DC *unk8;
    u8 padC[0x3C];
    u8 unk48;
} Node150E35DC;

void func_150E35DC(s32 arg0) {
    s32 i;
    Node150E35DC *node;

    arg0--;
    for (i = 0; i != 2; i++) {
        D_800DD190++;
        node = ((Node150E35DC *(*)[104])D_800DCE50)[i][39];
        while (node != NULL) {
            *(Node150E35DC **)((u8 *)D_800DD198 + (D_800DD190 << 2)) = node->unk8;
            if (arg0 == -1 || arg0 == node->unk48) {
                func_150E3514((Game1104D0EffectInstance *)node);
            }
            node = *(Node150E35DC **)((u8 *)D_800DD198 + (D_800DD190 << 2));
        }
        {
            extern s8 D_800DD190;

            D_800DD190--;
        }
    }
}


void func_150E36BC(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3) {
    Game1104D0EffectInstance *temp;

    arg0 -= 1;
    if ((arg0 >= 0) && (arg0 < 8)) {
        temp = (Game1104D0EffectInstance *)D_800D99D0[arg0];
        if (temp != 0) {
            if (temp->field_0x0 == 0x27) {
                *arg1 = (s32)temp->field_0x10;
                *arg2 = (s32)temp->field_0x14;
                *arg3 = (s32)temp->field_0x18;
            }
        }
    }
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3_150E3738;

typedef struct {
    s16 unk0[9];
} Sub_150E3738;

typedef struct {
    s32 unk0;
    f32 unk4;
    Vec3_150E3738 unk8;
    Vec3_150E3738 unk14;
    Vec3_150E3738 unk20;
    Vec3_150E3738 unk2C;
    Vec3_150E3738 unk38;
    Sub_150E3738 unk44;
    s16 unk56;
    u8 unk58;
    u8 unk59;
    u8 unk5A;
    s32 unk5C;
    s32 unk60;
} Particle_150E3738;

typedef struct {
    s32 unk0;
    char pad4[0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} Target_150E3738;

typedef struct {
    u8 unk0;
    u8 unk1;
    char pad2[0xA];
    u8 unkC;
    char padD[3];
    Vec3_150E3738 pos;
    Vec3_150E3738 target;
    Vec3_150E3738 unk28;
    f32 unk34;
    f32 unk38;
    Target_150E3738 *unk3C;
    s32 unk40;
    s32 unk44;
    u8 unk48;
    char pad49[1];
    u16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    Sub_150E3738 unk58;
    char pad6A[2];
    s32 unk6C;
    u8 unk70;
    u8 unk71;
    char pad72[2];
    s32 unk74;
    f32 *unk78;
    s16 unk7C;
} Effect_150E3738;

extern s32 func_15046C80(Vec3_150E3738 *, s32, f32, f32 *);
extern s32 func_15145128(Vec3_150E3738 *, Vec3_150E3738 *, f32 *, f32 *);
extern void func_15143874(s16, f32, f32 *, f32 *);
extern s32 func_151C229C(Vec3_150E3738 *, void *, s32, s32, s32, Particle_150E3738 *, f32, f32, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, f32, s32, s32, s32, s32, s32);
extern void func_15103254(s16, u8, f32, Vec3_150E3738 *, s32, s32, s32);

void func_150E3738(Game1104D0EffectInstance *arg0) {
    Effect_150E3738 *e;
    Vec3_150E3738 start;
    Vec3_150E3738 next;
    Vec3_150E3738 delta;
    Vec3_150E3738 step;
    f32 dist;
    f32 move;
    f32 scale;
    s32 spawned;
    Vec3_150E3738 p;
    Vec3_150E3738 hit;
    f32 pad178;
    Vec3_150E3738 pos;
    Vec3_150E3738 dir;
    f32 len;
    f32 pad158;
    f32 offz;
    f32 offx;
    f32 unused;
    f32 r2;
    s32 c;
    Particle_150E3738 particle;
    f32 r;
    s32 ang;

    e = (Effect_150E3738 *)arg0;
    spawned = 0;
    if (e->unk7C != -99) {
        e->unk7C -= D_800BE9E4;
        if (e->unk7C < 0) {
            func_150E3514((Game1104D0EffectInstance *)e);
        }
    }
    start = e->pos;
    if (e->unk3C != NULL) {
        if (e->unk3C->unk0 == 0) {
            func_150E3514((Game1104D0EffectInstance *)e);
            return;
        }
        e->target.x = e->unk3C->unk14;
        e->target.y = e->unk3C->unk18;
        e->target.z = e->unk3C->unk1C;
    } else if (e->unk78 != NULL) {
        e->target.x = e->unk78[0];
        e->target.y = e->unk78[1];
        e->target.z = e->unk78[2];
    }
    delta.x = e->target.x - e->pos.x;
    delta.y = e->target.y - e->pos.y;
    delta.z = e->target.z - e->pos.z;
    dist = sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    move = e->unk34 * D_800BE9E4;
    if (e->unk78 == NULL) {
        if (e->unk44 == 0) {
            if (dist <= move) {
                if (e->unk3C != NULL) {
                    e->unk44 = 30;
                    e->unk3C = NULL;
                    e->target.x += delta.x * 500.0f;
                    e->target.y += delta.y * 500.0f;
                    e->target.z += delta.z * 500.0f;
                } else {
                    func_150E3514((Game1104D0EffectInstance *)e);
                    return;
                }
            }
        } else {
            e->unk44 -= D_800BE9E4;
            if (e->unk44 <= 0) {
                func_150E3514((Game1104D0EffectInstance *)e);
                return;
            }
        }
    }
    if (dist != 0.0f) {
        extern f32 D_800BE9A4;

        scale = move / dist;
        step.x = scale * delta.x;
        step.y = scale * delta.y;
        step.z = scale * delta.z;
        next.x = start.x + step.x;
        next.y = start.y + step.y;
        next.z = start.z + step.z;
    } else {
        next = e->target;
        step = delta;
    }
    e->pos = next;
    e->unk4C += (526.0f + func_150ADA68() * 1228.0f) * 0.001000000047f * D_800BE9A4;
    while (e->unk4C > 1.0f) {
        pos = e->unk28;
        {
            f32 rnd;

            rnd = func_150ADA68();
            ang = func_150ADA20();
            func_15143874(ang & 0xFF, func_150ADA68() * 129.0f, &offx, &offz);
            p.x = rnd * step.x + start.x + offx;
            p.y = rnd * step.y + start.y + 200.0f;
            p.z = rnd * step.z + start.z + offz;
        }
        hit = p;
        if (func_15046C80(&p, 0, p.y - 1000.0f, &e->unk54) != 0) {
            hit.y = e->unk54;
        }
        {
            extern s32 func_15145128(Vec3_150E3738 *, Vec3_150E3738 *, f32 *, f32 *);

            dir.x = hit.x - pos.x;
            dir.y = hit.y - pos.y;
            dir.z = hit.z - pos.z;
        }
        if (func_15145128(&dir, &dir, &len, &unused) != 0) {
            move = func_150ADA68() * D_800BE9A4;
            scale = 190.0f * move;
            pos.x += dir.x * scale;
            pos.y += dir.y * scale;
            pos.z += dir.z * scale;
            len -= scale;
        }
        particle.unk0 = 0;
        particle.unk4 = len;
        particle.unk8 = hit;
        particle.unk14 = hit;
        particle.unk20 = hit;
        particle.unk2C = pos;
        particle.unk38 = dir;
        particle.unk44 = e->unk58;
        particle.unk56 = 0;
        particle.unk58 = 0;
        particle.unk59 = 1;
        particle.unk5A = 0;
        if (e->unk71 == 2 || e->unk71 == 3) {
            particle.unk5C = e->unk74;
        } else {
            particle.unk5C = 0;
        }
        particle.unk60 = e->unk6C;
        r2 = func_150ADA68();
        scale = func_150ADA68();
        c = func_150ADA20();
        if (spawned != 0 || e->unk78 != NULL) {
            ang = 2;
        } else {
            ang = 1;
        }
        if (func_151C229C(&pos, NULL, 0, 0, 0, &particle, 190.0f, 0.00526315812f, r2 * 50.0f + 10.0f,
                          scale * 191.0f + 160.0f, 80.0f, (c % 176U) + 80, 0, 0, 0, 0, 0, 0, 1, ang,
                          e->unk40, 0.0f, 0xFF, -1, 0, e->unkC, e->unk1) != 0) {
            spawned = 1;
        }
        e->unk4C -= 1.0f;
    }
    if (e->unk78 == NULL) {
        e->unk50 += (56.0f + func_150ADA68() * 235.0f) * 0.001000000047f * D_800BE9A4;
        while (e->unk50 > 1.0f) {
            c = func_150ADA20();
            ang = func_150ADA20();
            func_15103254((c & 3) + 6, (ang % 156U) + 100,
                          func_150ADA68() * 1548.0f + 1093.0f, &e->unk28, 0xFF, e->unkC, e->unk1);
            e->unk50 -= 1.0f;
        }
    }
}

void func_150E4010(Game1104D0EffectObj *arg0) {
    Game1104D0EffectState *temp;

    temp = &arg0->field_0x110;
    if (arg0->field_0x110.field_0xA4 != 0) {
        func_1505D1C4(temp->field_0x30, temp->field_0x34, temp->field_0x38,
                      temp->field_0xA4 | 0x60000, -1,
                      ((u16)(u32)(func_150484A0(temp->field_0x60, temp->field_0x64) * D_800A1050) - 0x4000) | 1, 0, 0);
    }
}

void func_150E411C(s32 arg0) {
    func_151C3B0C(arg0, 0.352000028f, 0.701000035f, 0.566000044f, D_800A1054, 0xFF, 0xFF, 0xFF);
}

extern f32 D_800BE9A4;

s32 func_150E4174(Game1104D0EffectInstance *arg0) {
    arg0->field_0x2C += arg0->field_0x4C * D_800BE9A4;
    arg0->field_0x30 += arg0->field_0x54 * D_800BE9A4;
    return 1;
}
