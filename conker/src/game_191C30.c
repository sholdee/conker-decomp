#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 D_800DCDE0[0x60];

struct Obj15164780 {
    u8 pad0[0x10];
    u8 unk10[0x38];
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
};

extern void *func_15167A68(s32, s32, s32, s32, u8, s32);

void *func_15164780(struct127 *arg0, s32 arg1, u8 arg2, s32 arg3) {
    void *temp_v1;
    struct108 *temp_v0;

    temp_v0 = &D_800DBFF0[arg0->unk6];
    temp_v1 = temp_v0->unk3D0;
    if (temp_v1 != 0) {
        temp_v0 = (struct108 *) ((struct127 *) temp_v1)->unk31C;
        if ((temp_v0 != 0) && (((struct126 *) temp_v0)->unk198 != 0)) {
            return NULL;
        }
    }
    temp_v1 = func_15167A68(0x39, arg3, arg1 + 0x68, 1, arg2, 1);
    if (temp_v1 == 0) {
        return NULL;
    }
    memcpy((s32)temp_v1 + 0x10, arg0, 0x38);
    ((struct Obj15164780 *) temp_v1)->unk60 = 1;
    ((struct Obj15164780 *) temp_v1)->unk61 = 1;
    ((struct Obj15164780 *) temp_v1)->unk62 = 1;
    ((struct Obj15164780 *) temp_v1)->unk63 = 1;
    ((struct Obj15164780 *) temp_v1)->unk64 = 1;
    ((struct Obj15164780 *) temp_v1)->unk65 = 1;
    ((struct Obj15164780 *) temp_v1)->unk48 = 0.0f;
    ((struct Obj15164780 *) temp_v1)->unk4C = 0.0f;
    ((struct Obj15164780 *) temp_v1)->unk50 = 0.0f;
    ((struct Obj15164780 *) temp_v1)->unk54 = 0.0f;
    ((struct Obj15164780 *) temp_v1)->unk58 = 0.0f;
    ((struct Obj15164780 *) temp_v1)->unk5C = 0.0f;
    return temp_v1;
}

typedef struct {
    u8 pad0[0x10];
    /* 0x10 */ u8 unk10;
} S15164888;

void func_15164888(S15164888 *arg0) {
    arg0->unk10 |= 0x2;
}

typedef struct {
    u8 v[3];
} S1516489CB3;

typedef struct {
    u8 pad0[0x10];
    /* 0x10 */ u8 unk10;
    /* 0x11 */ s8 unk11;
    /* 0x12 */ s8 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 unk16;
    /* 0x18 */ f32 amp[6];
    /* 0x30 */ f32 speed[6];
    /* 0x48 */ f32 phase[6];
    /* 0x60 */ S1516489CB3 flagA;
    /* 0x63 */ S1516489CB3 flagB;
} S1516489C;

extern s32 (*D_8008B390[])(S1516489C *);
extern f32 (*D_8008B394[])(S1516489C *);
extern f32 D_800A6B20[6];

#define WAVE(i, flag, prev) \
    if (arg0->speed[i] != 0.0f) { \
        arg0->phase[i] = func_15144B68(arg0->speed[i] * D_800BE9A4 + arg0->phase[i]); \
        flag = arg0->phase[i] < D_800A6B20[i]; \
        if ((arg0->unk10 & 2) && (prev != flag)) { \
            arg0->speed[i] = 0.0f; \
            arg0->phase[i] = 0.0f; \
        } \
        ((f32 (*)[6])D_800DCDE0)[arg0->unk16][i] += sinf(arg0->phase[i]) * arg0->amp[i] * scale; \
    }

void func_1516489C(S1516489C *arg0) {
    s32 pad;
    u8 done;
    u8 kill;
    f32 scale;
    S1516489CB3 prevA;
    S1516489CB3 prevB;

    done = 0;
    kill = 0;
    if (arg0->unk10 & 1) {
        arg0->unk14 -= D_800BE9E4;
        if (arg0->unk14 < 0) {
            kill = 1;
        }
    }
    if (!kill && arg0->unk11 != -1) {
        if (!D_8008B390[arg0->unk11](arg0)) {
            kill = 1;
        }
    }
    if (kill) {
        func_15164888((S15164888 *)arg0);
    }
    prevA = arg0->flagA;
    prevB = arg0->flagB;
    scale = 1.0f;
    if (arg0->unk10 & 4) {
        if (arg0->unk12 != -1) {
            scale = D_8008B394[arg0->unk12](arg0);
        }
    }
    WAVE(0, arg0->flagA.v[0], prevA.v[0])
    WAVE(1, arg0->flagA.v[1], prevA.v[1])
    WAVE(2, arg0->flagA.v[2], prevA.v[2])
    WAVE(3, arg0->flagB.v[0], prevB.v[0])
    WAVE(4, arg0->flagB.v[1], prevB.v[1])
    WAVE(5, arg0->flagB.v[2], prevB.v[2])
    if (arg0->speed[0] == 0.0f && arg0->speed[1] == 0.0f && arg0->speed[2] == 0.0f &&
        arg0->speed[3] == 0.0f && arg0->speed[4] == 0.0f && arg0->speed[5] == 0.0f) {
        done = 1;
    }
    if (done) {
        func_1516972C((struct102 *)arg0);
    }
}

void func_15164EE4(void) {
    bzero(D_800DCDE0, 0x60);
}

typedef struct {
    /* 0x00 */ struct17 unk0;
    /* 0x0C */ struct17 unkC;
    /* 0x18 */ struct17 unk18;
    /* 0x24 */ struct17 unk24;
} S15165Pose;

typedef struct {
    /* 0x00 */ S15165Pose rate;
    /* 0x30 */ S15165Pose fall;
    /* 0x60 */ S15165Pose peak;
    /* 0x90 */ f32 riseTime;
    /* 0x94 */ f32 holdTime;
    /* 0x98 */ f32 fallTime;
    /* 0x9C */ f32 time;
} S15165Blend;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ S15165Pose unk18;
    /* 0x48 */ u8 pad48[0x20];
    /* 0x68 */ S15165Blend unk68;
} S15165Obj;

typedef struct {
    /* 0x00 */ S15165Pose pose;
    /* 0x30 */ f32 riseTime;
    /* 0x34 */ f32 holdTime;
    /* 0x38 */ f32 fallTime;
} S15165Def;

typedef struct {
    /* 0x00 */ u8  unk0;
    /* 0x01 */ s8  unk1;
    /* 0x02 */ s8  unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8  unk6;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
} S15165Child; /* size 0x38 */

void *func_15165628(S15165Def *arg0, u8 arg1, s8 arg2, s32 arg3, u8 arg4, s32 arg5);

void *func_15164F0C(u8 arg0, u8 arg1, void *arg2, u8 arg3, s32 arg4) {
    switch (arg0) {
    case 7: {
        f32 d[15];
        d[0] = 8.0f;
        d[1] = 12.0f;
        d[2] = 9.0f;
        d[3] = 0.5f;
        d[4] = 0.199999988f;
        d[5] = 0.299999982f;
        d[6] = 1.15600002f;
        d[7] = 0.964000046f;
        d[8] = 0.458000034f;
        d[9] = 2.27859998f;
        d[10] = 1.91939998f;
        d[11] = 1.52499998f;
        d[12] = 4.0f;
        d[13] = 10.0f;
        d[14] = 16.0f;
        func_15165628((S15165Def *)d, arg1, -1, 0, arg3, arg4);
        break;
    }
    case 0: {
        f32 d[15];
        d[0] = 10.0f;
        d[1] = 4.0f;
        d[2] = 7.0f;
        d[3] = 0.780599952f;
        d[4] = 0.286899984f;
        d[5] = 0.888799965f;
        d[6] = 0.666000009f;
        d[7] = 1.19400001f;
        d[8] = 1.00999999f;
        d[9] = 0.435299993f;
        d[10] = 0.586699963f;
        d[11] = 0.709699988f;
        d[12] = 6.0f;
        d[13] = 17.0f;
        d[14] = 48.0f;
        func_15165628((S15165Def *)d, arg1, -1, 0, arg3, arg4);
        break;
    }
    case 1: {
        f32 d[15];
        d[0] = 20.0f;
        d[1] = 19.0f;
        d[2] = 53.0f;
        d[3] = 0.0f;
        d[4] = 0.0f;
        d[5] = 1.47780001f;
        d[6] = 0.602000058f;
        d[7] = 1.02100003f;
        d[8] = 1.1910001f;
        d[9] = 0.0f;
        d[10] = 0.0f;
        d[11] = 2.90499997f;
        d[12] = 3.0f;
        d[13] = 5.0f;
        d[14] = 30.0f;
        func_15165628((S15165Def *)d, arg1, -1, 0, arg3, arg4);
        break;
    }
    case 2: {
        f32 d[15];
        d[0] = 10.0f;
        d[1] = 7.0f;
        d[2] = 6.0f;
        d[3] = 1.0029f;
        d[4] = 0.497699976f;
        d[5] = 1.43059993f;
        d[6] = 0.654000044f;
        d[7] = 1.06200004f;
        d[8] = 0.32100001f;
        d[9] = 0.516900003f;
        d[10] = 0.772799969f;
        d[11] = 0.175599992f;
        d[12] = 2.0f;
        d[13] = 27.0f;
        d[14] = 15.0f;
        func_15165628((S15165Def *)d, arg1, -1, 0, arg3, arg4);
        break;
    }
    case 3: {
        f32 d[15];
        d[0] = 7.0f;
        d[1] = 11.0f;
        d[2] = 5.0f;
        d[3] = 0.432099998f;
        d[4] = 0.6329f;
        d[5] = 0.808200002f;
        d[6] = 0.276000023f;
        d[7] = 0.539000034f;
        d[8] = 0.43900001f;
        d[9] = 1.67269993f;
        d[10] = 0.494699985f;
        d[11] = 0.98939997f;
        d[12] = 5.0f;
        d[13] = 15.0f;
        d[14] = 12.0f;
        func_15165628((S15165Def *)d, arg1, -1, 0, arg3, arg4);
        break;
    }
    case 4: {
        f32 d[15];
        d[0] = 8.0f;
        d[1] = 17.0f;
        d[2] = 4.0f;
        d[3] = 0.761399984f;
        d[4] = 1.57069993f;
        d[5] = 1.26679993f;
        d[6] = 0.587000012f;
        d[7] = 0.906000018f;
        d[8] = 0.403000027f;
        d[9] = 1.15919995f;
        d[10] = 0.697299957f;
        d[11] = 1.34640002f;
        d[12] = 5.0f;
        d[13] = 3.0f;
        d[14] = 45.0f;
        func_15165628((S15165Def *)d, arg1, 0, 0, arg3, arg4);
        break;
    }
    case 5: {
        f32 d[15];
        void *ret;
        d[0] = 15.0f;
        d[1] = 10.0f;
        d[2] = 19.0f;
        d[3] = 0.539699972f;
        d[4] = 0.76789999f;
        d[5] = 0.988099992f;
        d[6] = 1.04500008f;
        d[7] = 0.636000037f;
        d[8] = 1.40400004f;
        d[9] = 0.452600002f;
        d[10] = 0.484999985f;
        d[11] = 0.684300005f;
        d[12] = 5.0f;
        d[13] = 5.0f;
        d[14] = 25.0f;
        ret = func_15165628((S15165Def *)d, arg1, 1, 24, arg3, arg4);
        if (ret != NULL) {
            memcpy((u8 *)ret + 0x108, arg2, 0x18);
        }
        return ret;
    }
    case 6: {
        f32 d[15];
        void *ret;
        d[0] = 15.0f;
        d[1] = 10.0f;
        d[2] = 19.0f;
        d[3] = 0.539699972f;
        d[4] = 0.76789999f;
        d[5] = 0.988099992f;
        d[6] = 1.04500008f;
        d[7] = 0.636000037f;
        d[8] = 1.40400004f;
        d[9] = 0.452600002f;
        d[10] = 0.484999985f;
        d[11] = 0.684300005f;
        d[12] = 5.0f;
        d[13] = 5.0f;
        d[14] = 25.0f;
        ret = func_15165628((S15165Def *)d, arg1, 1, 24, arg3, arg4);
        if (ret != NULL) {
            memcpy((u8 *)ret + 0x108, arg2, 0x18);
        }
        return ret;
    }
    }
}

void *func_15165628(S15165Def *arg0, u8 arg1, s8 arg2, s32 arg3, u8 arg4, s32 arg5) {
    S15165Child child;
    S15165Blend blend;
    void *obj;
    f32 riseScale;
    f32 fallScale;

    if ((arg0->riseTime == 0.0f) || (arg0->fallTime == 0.0f)) {
        return NULL;
    }

    riseScale = 1.0f / arg0->riseTime;
    fallScale = 1.0f / arg0->fallTime;

    child.unk0 = 4;
    child.unk1 = 0;
    child.unk4 = 0x12C;
    child.unk6 = arg1;
    child.unk2 = arg2;
    child.unk8 = 0;
    child.unkC = 0;
    child.unk10 = 0;
    child.unk14 = 0;
    child.unk18 = 0;
    child.unk1C = 0;
    child.unk20 = 0;
    child.unk24 = 0;
    child.unk28 = 0;
    child.unk2C = 0;
    child.unk30 = 0;
    child.unk34 = 0;

    blend.rate.unk0.unk0 = arg0->pose.unk0.unk0 * riseScale;
    blend.rate.unk0.unk4 = arg0->pose.unk0.unk4 * riseScale;
    blend.rate.unk0.unk8 = arg0->pose.unk0.unk8 * riseScale;
    blend.rate.unkC.unk0 = arg0->pose.unkC.unk0 * riseScale;
    blend.rate.unkC.unk4 = arg0->pose.unkC.unk4 * riseScale;
    blend.rate.unkC.unk8 = arg0->pose.unkC.unk8 * riseScale;
    blend.rate.unk18.unk0 = arg0->pose.unk18.unk0 * riseScale;
    blend.rate.unk18.unk4 = arg0->pose.unk18.unk4 * riseScale;
    blend.rate.unk18.unk8 = arg0->pose.unk18.unk8 * riseScale;
    blend.rate.unk24.unk0 = arg0->pose.unk24.unk0 * riseScale;
    blend.rate.unk24.unk4 = arg0->pose.unk24.unk4 * riseScale;
    blend.rate.unk24.unk8 = arg0->pose.unk24.unk8 * riseScale;

    blend.fall.unkC.unk0 = arg0->pose.unk0.unk0 * fallScale;
    blend.fall.unkC.unk4 = arg0->pose.unk0.unk4 * fallScale;
    blend.fall.unkC.unk8 = arg0->pose.unk0.unk8 * fallScale;
    blend.fall.unk0.unk0 = arg0->pose.unkC.unk0 * fallScale;
    blend.fall.unk0.unk4 = arg0->pose.unkC.unk4 * fallScale;
    blend.fall.unk0.unk8 = arg0->pose.unkC.unk8 * fallScale;
    blend.fall.unk24.unk0 = arg0->pose.unk18.unk0 * fallScale;
    blend.fall.unk24.unk4 = arg0->pose.unk18.unk4 * fallScale;
    blend.fall.unk24.unk8 = arg0->pose.unk18.unk8 * fallScale;
    blend.fall.unk18.unk0 = arg0->pose.unk24.unk0 * fallScale;
    blend.fall.unk18.unk4 = arg0->pose.unk24.unk4 * fallScale;
    blend.fall.unk18.unk8 = arg0->pose.unk24.unk8 * fallScale;

    blend.peak.unk0 = arg0->pose.unk0;
    blend.peak.unkC = arg0->pose.unkC;
    blend.peak.unk18 = arg0->pose.unk18;
    blend.peak.unk24 = arg0->pose.unk24;

    blend.riseTime = arg0->riseTime;
    blend.holdTime = arg0->holdTime;
    blend.fallTime = arg0->fallTime;
    blend.time = 0;

    obj = func_15164780((struct127 *)&child, arg3 + 0xA0, arg4, arg5);
    if (obj != NULL) {
        memcpy((s32)obj + 0x68, &blend, 0xA0);
    }
    return obj;
}

s32 func_151658DC(S15165Obj *arg0) {
    S15165Blend *p;
    s32 ret;

    p = &arg0->unk68;
    ret = 1;
    p->time = p->time + D_800BE9A4;
    if (p->time < p->riseTime) {
        arg0->unk18.unk0.unk0 = p->rate.unk0.unk0 * p->time;
        arg0->unk18.unk0.unk4 = p->rate.unk0.unk4 * p->time;
        arg0->unk18.unk0.unk8 = p->rate.unk0.unk8 * p->time;
        arg0->unk18.unkC.unk0 = p->rate.unkC.unk0 * p->time;
        arg0->unk18.unkC.unk4 = p->rate.unkC.unk4 * p->time;
        arg0->unk18.unkC.unk8 = p->rate.unkC.unk8 * p->time;
        arg0->unk18.unk18.unk0 = p->rate.unk18.unk0 * p->time;
        arg0->unk18.unk18.unk4 = p->rate.unk18.unk4 * p->time;
        arg0->unk18.unk18.unk8 = p->rate.unk18.unk8 * p->time;
        arg0->unk18.unk24.unk0 = p->rate.unk24.unk0 * p->time;
        arg0->unk18.unk24.unk4 = p->rate.unk24.unk4 * p->time;
        arg0->unk18.unk24.unk8 = p->rate.unk24.unk8 * p->time;
    } else if (p->time < p->riseTime + p->holdTime) {
        arg0->unk18.unk0 = p->peak.unk0;
        arg0->unk18.unkC = p->peak.unkC;
        arg0->unk18.unk18 = p->peak.unk18;
        arg0->unk18.unk24 = p->peak.unk24;
    } else if (p->time < (p->riseTime + p->holdTime) + p->fallTime) {
        f32 t = p->time - (p->riseTime + p->holdTime);

        arg0->unk18.unk0.unk0 = p->peak.unk0.unk0 - p->fall.unkC.unk0 * t;
        arg0->unk18.unk0.unk4 = p->peak.unk0.unk4 - p->fall.unkC.unk4 * t;
        arg0->unk18.unk0.unk8 = p->peak.unk0.unk8 - p->fall.unkC.unk8 * t;
        arg0->unk18.unkC.unk0 = p->peak.unkC.unk0 - p->fall.unk0.unk0 * t;
        arg0->unk18.unkC.unk4 = p->peak.unkC.unk4 - p->fall.unk0.unk4 * t;
        arg0->unk18.unkC.unk8 = p->peak.unkC.unk8 - p->fall.unk0.unk8 * t;
        arg0->unk18.unk18.unk0 = p->peak.unk18.unk0 - p->fall.unk24.unk0 * t;
        arg0->unk18.unk18.unk4 = p->peak.unk18.unk4 - p->fall.unk24.unk4 * t;
        arg0->unk18.unk18.unk8 = p->peak.unk18.unk8 - p->fall.unk24.unk8 * t;
        arg0->unk18.unk24.unk0 = p->peak.unk24.unk0 - p->fall.unk18.unk0 * t;
        arg0->unk18.unk24.unk4 = p->peak.unk24.unk4 - p->fall.unk18.unk4 * t;
        arg0->unk18.unk24.unk8 = p->peak.unk24.unk8 - p->fall.unk18.unk8 * t;
    } else {
        ret = 0;
    }
    return ret;
}

f32 func_15165BB0(s32 *arg0, struct17 *arg1, f32 arg2, f32 arg3, f32 arg4);

void func_15165B80(s32 *arg0) {
    func_15165BB0(arg0, (struct17 *)((s32)arg0 + 0x108), *(f32 *)&arg0[0x45], *(f32 *)&arg0[0x46], *(f32 *)((s32)arg0 + 0x11C));
}

extern struct17 *func_15144B34(s32);
extern f32 func_15143E64(struct17 *);

f32 func_15165BB0(s32 *arg0, struct17 *arg1, f32 arg2, f32 arg3, f32 arg4) {
    f32 ret;
    struct17 sp20;
    struct17 *p;
    f32 dist;

    p = func_15144B34(*(u8 *)((s32)arg0 + 0x16));
    sp20.unk0 = arg1->unk0 - p->unk0;
    sp20.unk4 = arg1->unk4 - p->unk4;
    sp20.unk8 = arg1->unk8 - p->unk8;
    dist = func_15143E64(&sp20);
    if (dist < arg2) {
        ret = 1.0f;
    } else if (arg2 + arg3 < dist) {
        ret = 0.0f;
    } else {
        ret = 1.0f - (dist - arg2) * arg4;
    }
    return ret;
}
