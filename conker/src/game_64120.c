#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_15048A40
#include "variables.h"

s32 func_150380C0(f32 *, s32, s32, s32, f32, s32, f32 *, s32, s32, s32, s32);
extern f32 D_800C3FD0[];


extern f32 D_80098250;
extern s16 D_800C3FF4[];
extern u16 *D_80084380[];
extern u8 D_80097E7C[];
extern u8 D_80098050[];

void func_15039A78(f32 *, f32 *, f32 *, f32 *, f32, f32, s32);

/* Yaw/pitch window an aim may turn within; beyond max + margin (or min - margin) it gives up. */
typedef struct {
    /* 0x00 */ f32 yawMax;
    /* 0x04 */ f32 yawMin;
    /* 0x08 */ f32 pitchMax;
    /* 0x0C */ f32 pitchMin;
    /* 0x10 */ f32 yawMargin;
    /* 0x14 */ f32 pitchMargin;
} AimLimits;

extern AimLimits D_80098068[];
void func_1503B708(void *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4,
                   s32 arg5, f32 *arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10);


typedef struct {
    /* 0x00 */ f32 yaw[3];
    /* 0x0C */ f32 pitch[3];
    /* 0x18 */ f32 yawVel[3];
    /* 0x24 */ f32 pitchVel[3];
    /* 0x30 */ u8 timer[3];
    /* 0x33 */ u8 blend[3];
    /* 0x36 */ u8 flag[3];
    /* 0x39 */ u8 pad39;
    /* 0x3A */ s16 offYaw[3];
    /* 0x40 */ s16 offPitch[3];
} CamData;

/* struct127 +0x324 is still padding (pad320) in include/structs.h */
#define OBJ_CAM_DATA(obj) (*(CamData **)&(obj)->pad320[4])

/* Aim points at struct127 +0x1A4 (still padding in include/structs.h), filled in by func_1503A08C. */
typedef struct {
    /* 0x1A4 */ s16 x;
    /* 0x1A6 */ s16 y;
    /* 0x1A8 */ s16 z;
    /* 0x1AA */ s16 topY;
    /* 0x1AC */ s16 sideA[3];
    /* 0x1B2 */ s16 sideB[3];
    /* 0x1B8 */ s8 dir[3];
} ObjAimPoints;

#define OBJ_AIM(obj) (*(ObjAimPoints *)&(obj)->pad1A4)

/* An object's unk1D4 points at its array of 4x4 bone matrices. */
#define OBJ_BONES(obj) ((f32 (*)[4][4])(obj)->unk1D4)

/* Allocates an object's camera-aim state and marks every slot as having no target. */
void func_15036C70(struct127 *obj) {
    s32 i;

    OBJ_CAM_DATA(obj) = allocate_memory(sizeof(CamData), 1, 0, 0);
    bzero(OBJ_CAM_DATA(obj), sizeof(CamData));
    for (i = 0; i < 3; i++) {
        OBJ_CAM_DATA(obj)->yaw[i] = -99999.0f;
        OBJ_CAM_DATA(obj)->pitch[i] = -99999.0f;
    }
}

extern u8 D_800C3FFA;
static f32 D_800C3FE8[3];
void func_150379DC(f32 *, s32, s32, s32, f32, s32, s32);
void func_15038620(s32, s32, f32 *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_15036CE8(void) {
    struct127 *obj;
    s32 limitIdx;
    CamData *data;
    s32 pad[4]; // matching: -g3 gives every local a stack slot; these keep the frame layout
    s32 argB;
    s32 pad2;
    u8 bone;

    obj = &D_800CC2D0[D_800C3E78];
    data = OBJ_CAM_DATA(obj);
    if (data == NULL) {
        return;
    }
    switch (obj->id) {
        case 59:
        case 117:
        case 128:
        case 136:
        case 144:
        case 150:
        case 152:
        case 156:
        case 157:
        case 159:
        case 160:
        case 176:
        case 177:
        case 178:
        case 180:
            if (D_800C3FFA == 0) {
                D_800C3FFA = 1;
                return;
            }
            break;
    }
    if (D_800C35EA == 1 && D_800C3FFA == 0) {
        D_800C3FFA = 1;
        return;
    }
    limitIdx = 0;
    argB = 1;
    if (D_800C3FFA == 0) {
        bone = 7;
        argB = 0;
        limitIdx = 1;
        if (D_800D154C->unk2F8 & 0x100) {
            data->offYaw[0] = 45;
            data->offPitch[0] = 64;
        } else if (data->yaw[0] == -99999.0f) {
            D_800C3FFA = 1;
            return;
        }
    } else {
        bone = 12;
        ; // matching: the empty statement steers IDO's scheduling of the -99999.0f load below
    }
    if (D_800C3FFA == 0 && !(D_800D154C->unk2F8 & 0x100)) {
        D_800C3FE8[0] = -99999.0f;
        D_800C3FE8[1] = -99999.0f;
        D_800C3FE8[2] = -99999.0f;
    } else {
        func_150379DC(D_800C3FE8, bone, 0, D_800C3E78, 90.0f, 0xFF, 0);
    }
    func_15038620(bone, 0, D_800C3FE8, D_800C3E78, limitIdx, 0, D_800C3FFA, 3, 0, 0, 0, argB, 0);
    D_800C3FFA++;
}

void func_15036F34(void) {
    s32 bone;
    s32 refBone;
    s32 rateIdx;
    s32 limitIdx;
    s32 axes;
    s32 skipUnk127;
    f32 bestDist;
    f32 yOffset;
    s32 noCone;
    s32 preferId;
    s32 anyRange;
    s32 relative;
    s32 clearTarget;
    s32 forceSearch;
    s32 pad; // matching: -g3 stack slot
    s32 argB;
    s32 axisMode;

    preferId = 0x100;
    argB = 1;
    bestDist = 1600000000.0f;
    rateIdx = 1;
    limitIdx = 2;
    axes = 3;
    forceSearch = 0;
    noCone = 0;
    skipUnk127 = 0;
    relative = 0;
    anyRange = 0;
    axisMode = 0;
    yOffset = 0.0f;
    do {} while (0); // matching: the empty loop splits the block and swaps IDO's register choice for clearTarget/forceSearch
    clearTarget = 0;
    switch (D_800CC2D0[D_800C3E78].id) {
    case 130:
        bone = 12;
        refBone = 1;
        break;
    case 162:
        bone = 4;
        refBone = 3;
        break;
    case 164:
        bone = 4;
        refBone = 3;
        break;
    case 94:
        bone = 4;
        refBone = 3;
        break;
    case 151:
        bone = 4;
        refBone = 3;
        break;
    case 49:
        bone = 5;
        refBone = 4;
        break;
    case 139:
        bone = 0x21;
        refBone = 0x20;
        break;
    case 135:
    case 181:
        bone = 4;
        refBone = 3;
        break;
    case 134:
        refBone = 3;
        bone = 5;
        if (D_800C3FFA == 0) {
            bone = 4;
            rateIdx = 10;
        } else {
            refBone = 4;
        }
        break;
    case 153:
        refBone = 3;
        bone = 5;
        if (D_800C3FFA == 0) {
            bone = 4;
            rateIdx = 10;
        } else {
            refBone = 4;
        }
        break;
    case 17:
        refBone = 3;
        bone = 5;
        if (D_800C3FFA == 0) {
            bone = 4;
            rateIdx = 10;
        } else {
            refBone = 4;
        }
        break;
    case 20:
        refBone = 3;
        bone = 5;
        if (D_800C3FFA == 0) {
            bone = 4;
            rateIdx = 10;
        } else {
            refBone = 4;
        }
        break;
    case 121:
        bone = 0x1B;
        refBone = 0x1A;
        break;
    case 33:
        bone = 0x19;
        refBone = 0x18;
        break;
    case 123:
        bone = 3;
        refBone = 2;
        if (D_800C3FFA == 0) {
            anyRange = 1;
            break;
        }
        yOffset = -165.0f;
        limitIdx = 10;
        rateIdx = 7;
        switch (D_800CC2D0[D_800C3E78].unk84.uh) {
        case 7:
        case 9:
            skipUnk127 = 1;
            break;
        case 20:
            preferId = 0x7E;
            yOffset = 0.0f;
            rateIdx = 9;
            limitIdx = 11;
            break;
        default:
            clearTarget = 1;
        }
        if (D_800C3FFA == 1) {
            bone = 0x16;
            refBone = 0x16;
            axisMode = 2;
            forceSearch = 1;
        } else {
            bone = 0x1A;
            refBone = 0x1A;
            axisMode = 1;
        }
        goto setAnyRange; // matching: sharing case 75's tail; a separate `anyRange = 1; break;` mismatches
    case 124:
        bone = 1;
        refBone = 0;
        skipUnk127 = 1;
        axisMode = 3;
        break;
    case 108:
        bone = 0x11;
        refBone = 0x10;
        break;
    case 52:
        bone = 4;
        refBone = 3;
        break;
    case 118:
        bone = 4;
        refBone = 3;
        break;
    case 105:
        bone = 4;
        refBone = 3;
        break;
    case 138:
        if (D_800BE616 != 0) {
            return;
        }
        bone = 4;
        refBone = 0;
        if (D_800CC2D0[D_800C3E78].unk2E8 == 1) {
            return;
        }
        yOffset = -100.0f;
        axes = 2;
        limitIdx = 12;
        break;
    case 90:
    case 95:
    case 116:
    case 122:
    case 141:
        bone = 3;
        refBone = 2;
        break;
    case 88:
        bone = 4;
        refBone = 3;
        break;
    case 91:
        bone = 4;
        refBone = 3;
        break;
    case 84:
        bone = 0x1D;
        refBone = 0x1C;
        skipUnk127 = 1;
        break;
    case 41:
        bone = 2;
        refBone = 1;
        break;
    case 75:
        if (D_800C3FFA == 0) {
            bone = 0x2B;
            refBone = 4;
            rateIdx = 6;
            argB = 0;
            skipUnk127 = 1;
        } else {
            bone = 0x2C;
            refBone = 4;
            skipUnk127 = 1;
        }
    setAnyRange:
        anyRange = 1;
        break;
    case 80:
    case 154:
    case 155:
        bone = 0xD;
        refBone = 2;
        break;
    case 74:
        bone = 1;
        refBone = 0;
        axes = 1;
        noCone = 1;
        break;
    case 71:
        bone = 1;
        refBone = 0;
        axes = 1;
        noCone = 1;
        break;
    case 68:
        bone = 3;
        refBone = 3;
        break;
    case 40:
        if (D_800C3FFA == 0) {
            bone = 2;
            refBone = 0;
            axes = 1;
            noCone = 1;
            rateIdx = 4;
            relative = 1;
        } else {
            bone = 3;
            refBone = 2;
            axes = 2;
            noCone = 1;
            rateIdx = 4;
            relative = 1;
        }
        break;
    case 43:
        bone = 1;
        refBone = 0;
        limitIdx = 3;
        rateIdx = 3;
        axes = 1;
        noCone = 1;
        skipUnk127 = 1;
        break;
    case 58:
    case 61:
        bone = 5;
        refBone = 4;
        limitIdx = 5;
        break;
    case 83:
    case 165:
        bone = 5;
        refBone = 0;
        limitIdx = 7;
        break;
    case 54:
        bone = 4;
        refBone = 3;
        limitIdx = 4;
        break;
    case 45:
        bone = 5;
        refBone = 4;
        skipUnk127 = 1;
        break;
    case 12:
    case 22:
    case 34:
    case 179:
        bone = 4;
        refBone = 3;
        break;
    case 125:
        bone = 4;
        refBone = 3;
        break;
    case 32:
    case 140:
    case 147:
        bone = 2;
        refBone = 1;
        break;
    case 5:
    case 173:
    case 174:
    case 175:
        refBone = 0;
        bone = 0x10;
        if (D_800C3FFA == 0) {
            bone = 1;
            rateIdx = 2;
            argB = 0;
        } else {
            refBone = 1;
        }
        break;
    case 18:
        axes = 2;
        limitIdx = 8;
        anyRange = 1;
        if (D_800C3FFA == 0) {
            bone = 1;
            refBone = 0;
        } else if (D_800C3FFA == 1) {
            bone = 3;
            refBone = 0;
        } else {
            bone = 5;
            refBone = 0;
        }
        skipUnk127 = 1;
        break;
    default:
        return;
    }
    if (clearTarget) {
        D_800C3FE8[0] = -99999.0f;
        D_800C3FE8[1] = -99999.0f;
        D_800C3FE8[2] = -99999.0f;
    } else if (D_800C3FFA == 0 || forceSearch) {
        func_1503B708(D_800C3FE8, bone, refBone, D_800C3E78,
                      D_80098068[limitIdx - 1].yawMax + D_80098068[limitIdx - 1].yawMargin, preferId, &bestDist, noCone, skipUnk127, anyRange, axisMode);
        D_800C3FE8[1] += yOffset;
    }
    func_15038620(bone, refBone, D_800C3FE8, D_800C3E78, limitIdx, rateIdx, D_800C3FFA, axes, noCone, relative, 0, argB, axisMode);
    D_800C3FFA++;
}

void func_15038468(f32 *arg0, f32 *arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);

/*
 * Tests whether world point (x, y, z) lies within +-range degrees of yaw as seen from object
 * objIndex's bone matrix `bone` (relative to `refBone`). On success copies the point to out.
 */
s32 func_15037698(s32 objIndex, s32 refBone, s32 bone, f32 x, f32 y, f32 z, f32 *out, f32 range, s32 noCheck,
                  s32 axisMode) {
    f32 (*refMtx)[4];
    f32 (*mtx)[4];
    f32 delta[3];
    f32 unused[3];
    f32 localX;
    f32 localY;
    f32 localZ;
    f32 yaw;
    f32 pitch;

    if (noCheck) {
        out[0] = x;
        out[1] = y;
        out[2] = z;
        return 1;
    }
    refMtx = OBJ_BONES(&D_800CC2D0[objIndex])[refBone];
    if (D_800CC2D0[objIndex].unk1D4 == NULL) {
        return 0;
    }
    mtx = OBJ_BONES(&D_800CC2D0[objIndex])[bone];
    refMtx[0][3] = refMtx[1][3] = refMtx[2][3] = mtx[0][3] = mtx[1][3] = mtx[2][3] = 0.0f;
    refMtx[3][3] = mtx[3][3] = 1.0f;
    unused[0] = refMtx[1][0]; // matching: dead stores IDO keeps (stack array); removing them changes the code
    unused[1] = refMtx[1][1];
    unused[2] = refMtx[1][2];
    delta[0] = x - refMtx[3][0];
    delta[1] = y - refMtx[3][1];
    delta[2] = z - refMtx[3][2];
    localX = delta[0] * mtx[0][0] + delta[1] * mtx[0][1] + delta[2] * mtx[0][2];
    localY = delta[0] * mtx[1][0] + delta[1] * mtx[1][1] + delta[2] * mtx[1][2];
    localZ = delta[0] * mtx[2][0] + delta[1] * mtx[2][1] + delta[2] * mtx[2][2];
    func_15038468(&yaw, &pitch, localX, localY, localZ, axisMode);
    if (yaw < range && -range < yaw) {
        out[0] = x;
        out[1] = y;
        out[2] = z;
        return 1;
    }
    return 0;
}

/*
 * Handles the fixed target modes in an object's unk2F8 low bits: 1 aims at D_800DBFF0's point,
 * 2 at the point in D_800C3FF4 (unless a type-4 object is more than 250 units away), 3 holds.
 * Returns 1 when one of these applied.
 */
s32 func_15037880(s32 objIndex, f32 *out) {
    struct127 *obj;
    s32 mode;
    f32 dx;
    f32 dz;

    obj = &D_800CC2D0[objIndex];
    mode = obj->unk2F8 & 7;
    if (mode == 1) {
        out[0] = D_800DBFF0->unk2F8;
        out[1] = D_800DBFF0->unk2FC;
        out[2] = D_800DBFF0->unk300;
        return 1;
    }
    if (mode == 2) {
        if (obj->unk5 == 4) {
            dx = obj->x_position - (f32)D_800C3FF4[0];
            dz = obj->z_position - (f32)D_800C3FF4[2];
            if (62500.0f < dx * dx + dz * dz) {
                return 1;
            }
        }
        out[0] = D_800C3FF4[0];
        out[1] = D_800C3FF4[1];
        out[2] = D_800C3FF4[2];
        return 1;
    }
    if (mode == 3) {
        return 1;
    }
    return 0;
}

typedef struct {
    f32 x, y, z;
} Vec3f;

/* The eight node types func_150379DC scans for aim targets. */
typedef struct {
    s32 types[8];
} AimNodeTypes;

/* Entries of the D_800DCE50 per-type lists. */
typedef struct AimNode {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ struct AimNode *next;
    /* 0x0C */ u8 padC[0x2C];
    /* 0x38 */ f32 unk38; /* position used by node types 25 and 72 */
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ u8 pad44[0x22];
    /* 0x66 */ u16 unk66; /* types 25/72 are skipped while non-zero */
    /* 0x68 */ u8 pad68[0x30];
    /* 0x98 */ f32 unk98; /* position used by every other type */
    /* 0x9C */ f32 unk9C;
    /* 0xA0 */ f32 unkA0;
} AimNode;

typedef struct {
    /* 0x000 */ u8 pad0[0x3E8];
    /* 0x3E8 */ struct113 *unk3E8[2]; /* indexed by D_800BE9C0 */
    /* 0x3F0 */ u8 pad3F0[6];
    /* 0x3F6 */ u8 unk3F6;
} AimHolderData;

/* What func_150319CC returns (see game_5D2C0.c); only the fields used here. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 pad1[0x47];
    /* 0x48 */ AimHolderData *unk48;
} AimHolder;

/* D_800C6650 holds a pointer to D_800C6654 struct160 aim points (variables.h declares it as an array). */
#define AIM_POINTS (*(struct160 **)D_800C6650)

extern AimNodeTypes D_800843E0;
extern u8 D_80084404[];
extern AimNode *D_800DCE50[2][104];
AimHolder *func_150319CC(s32, struct127 *);
void func_1507E9E8(struct127 *, s32);

/*
 * Picks the point object objIndex should aim bone `bone` at, writing it to out (or -99999 when there is none):
 * its camera's target, a fixed target mode, the held object for anim 0x81, the nearest other object,
 * the nearest struct160 aim point, or the nearest node from the D_800DCE50 lists, each within +-range degrees.
 */
void func_150379DC(f32 *out, s32 refBone, s32 bone, s32 objIndex, f32 range, s32 preferId, s32 axisMode) {
    struct127 *obj;
    s32 hitObj;
    s32 foundKind;
    s16 objX;
    u32 idx;
    struct160 *point;
    f32 radiusSq;
    f32 px;
    f32 py;
    s32 list;
    s32 i;
    u32 bestIdx;
    f32 bestDist;
    f32 pz;
    f32 distSq;
    static u32 D_80084400; /* aim point the next scan starts from */

    out[0] = -99999.0f;
    out[1] = -99999.0f;
    out[2] = -99999.0f;
    D_800C3FD0[0] = -99999.0f;
    D_800C3FD0[1] = -99999.0f;
    D_800C3FD0[2] = -99999.0f;
    if ((&D_800C35EA)[0] == 1 || (&D_800C35EA)[1] == 1) {
        return;
    }
    obj = &D_800CC2D0[objIndex];
    if (obj->camera != NULL && obj->camera->unk2C == 0x100 && obj->camera->unk73C != 0) {
        Vec3f camTarget;

        camTarget = *(Vec3f *)&obj->camera->pad73E[2]; /* struct108 +0x740 */
        out[0] = camTarget.x;
        out[1] = camTarget.y;
        out[2] = camTarget.z;
        return;
    }
    if (func_15037880(objIndex, out)) {
        return;
    }
    if (objIndex == 0 && obj->unk84.uh == 0x81) {
        AimHolder *holder;
        AimHolderData *holderData;
        struct113 *model;
        Vec3f target;
        struct127 *held;

        if ((holder = func_150319CC(0x1F, obj)) != NULL && (holderData = holder->unk48) != NULL &&
            holderData->unk3F6 != 0 && (model = holderData->unk3E8[D_800BE9C0]) != NULL &&
            (held = func_15083E90(holder->unk0)) != NULL) {
            if (held->unk5 == 5) {
                target.x = held->x_position;
                target.y = held->y_position;
                target.z = held->z_position;
            } else {
                func_15043FF0((vertex *)&target, model);
            }
            if (func_15037698(objIndex, refBone, bone, target.x, target.y, target.z, out, range, 0, axisMode)) {
                return;
            }
        }
    }
    foundKind = 0;
    objX = (s32)D_800CC2D0[objIndex].x_position;
    bestDist = 1600000000.0f;
    hitObj = func_150380C0(out, refBone, bone, objIndex, range, preferId, &bestDist, 0, 0, 0, axisMode);
    if (hitObj != 0 && bestDist != 1600000000.0f) {
        foundKind = 1;
    }
    {
        f32 pos[3];
        f32 maxDist;
        AimNodeTypes types;
        s32 usable;
        s32 type;
        AimNode *node;
        f32 dx;
        f32 dy;
        f32 dz;

        if (D_80084400 >= D_800C6654) {
            D_80084400 = 0;
        }
        if (D_800C6654 != 0) {
            idx = D_80084400;
            bestIdx = -1;
            do {
                point = &AIM_POINTS[idx];
                if (objX < point->unkE && point->unk10 < objX && (u16)point->unk6 == 0) {
                    radiusSq = point->unk8;
                    px = point->unk0;
                    py = point->unk2;
                    pz = point->unk4;
                    dz = obj->z_position - pz;
                    dy = obj->y_position - py;
                    dz *= dz;
                    dx = objX - px;
                    dy *= dy;
                    distSq = dx * dx + dz + dy;
                    if (distSq < radiusSq && distSq < bestDist &&
                        func_15037698(objIndex, refBone, bone, px, py + 50.0f, pz, out, range, 0, axisMode)) {
                        bestIdx = idx;
                        bestDist = distSq;
                    }
                }
                idx++;
                if (idx == D_800C6654) {
                    idx = 0;
                }
            } while (idx != D_80084400);
            if (bestIdx != -1) {
                D_80084400 = bestIdx;
                foundKind = 2;
            }
        }

        types = D_800843E0;
        for (list = 0; list < 2; list++) {
            for (i = 0; i < 8; i++) {
                type = types.types[i];
                node = D_800DCE50[list][type];
                while (node != NULL) {
                    if (type == 25 || type == 72) {
                        extern s32 D_800C6654; // matching: this redeclaration shifts IDO's scheduling of the loop
                        if (node->unk66 == 0) {
                            maxDist = 250000.0f;
                            usable = 1;
                            pos[0] = node->unk38;
                            pos[1] = node->unk3C;
                            pos[2] = node->unk40;
                        } else {
                            usable = 0;
                        }
                    } else {
                        maxDist = 250000.0f;
                        usable = 1;
                        pos[0] = node->unk98;
                        pos[1] = node->unk9C;
                        pos[2] = node->unkA0;
                    }
                    if (usable) {
                        pz = pos[2];
                        py = pos[1];
                        px = pos[0];
                        dz = obj->z_position - pz;
                        dy = obj->y_position - py;
                        dz *= dz;
                        dx = objX - px;
                        dy *= dy;
                        distSq = dx * dx + dz + dy;
                        if (distSq < maxDist) {
                            dx = objX - pos[0];
                            if (dx * dx + dz + dy < bestDist &&
                                func_15037698(objIndex, refBone, bone, px, py, pz, out, range, 0, axisMode)) {
                                foundKind = 3;
                                bestDist = distSq;
                            }
                        }
                    }
                    node = node->next;
                }
            }
        }
    }
    if (foundKind == 1) {
        func_1507E9E8(obj, hitObj - 1);
        return;
    }
    if (foundKind == 2) {
        if (AIM_POINTS[bestIdx].unk12 != 0) {
            func_1507E9E8(obj, AIM_POINTS[bestIdx].unk12 - 1);
        }
    } else if (foundKind == 3) {
        // matching: the empty branch keeps the shared epilogue branch IDO emits
    }
}

/*
 * Finds the nearest active object (other than objIndex and its linked objects) whose aim point lies within
 * the bone's +-range degree cone, preferring object id preferId. Writes the point to out and the distance
 * to *bestDist, and returns the object's unk13B + 1 (0 and out = -99999 when nothing qualifies).
 */
s32 func_150380C0(f32 *out, s32 refBone, s32 bone, s32 objIndex, f32 range, s32 preferId, f32 *bestDist,
                  s32 noCheck, s32 skipUnk127, s32 anyRange, s32 axisMode) {
    s32 i;
    f32 dist;
    f32 dx;
    f32 dz;
    f32 dy;
    s32 found;
    f32 maxDist;
    s32 selfId;
    s32 selfLink;
    s32 allowSameId;
    s32 excludedId;

    found = -1;
    allowSameId = 1;
    excludedId = -1;
    if ((&D_800C35EA)[0] == 1 || (&D_800C35EA)[1] == 1) {
        out[0] = -99999.0f;
        out[1] = -99999.0f;
        out[2] = -99999.0f;
        return 0;
    }
    selfLink = objIndex + 1;
    selfId = D_800CC2D0[objIndex].id;
    if (selfId == 5 || selfId == 173 || selfId == 174 || selfId == 175) {
        allowSameId = 0;
    }
    if (selfId == 45) {
        excludedId = 44;
    }
    if (!anyRange) {
        maxDist = D_800CC2D0[objIndex].unk23D * 8;
        maxDist = maxDist * maxDist;
    } else {
        maxDist = 1600000000.0f;
    }
    for (i = 0; i < 25; i++) {
        if (D_800CC2D0[i].interaction_state == 0) {
            continue;
        }
        if (D_800CC2D0[i].id == 0xFF) {
            continue;
        }
        if (i == objIndex) {
            continue;
        }
        if (selfLink == D_800CC2D0[i].unk65) {
            continue;
        }
        if (D_800CC2D0[objIndex].unk65 == i + 1) {
            continue;
        }
        if (!allowSameId && selfId == D_800CC2D0[i].id) {
            continue;
        }
        if (excludedId == D_800CC2D0[i].id) {
            continue;
        }
        if ((skipUnk127 || (D_800CC2D0[objIndex].unk2F8 & 7) == 4) && D_800CC2D0[i].unk127 == 0xFF) {
            continue;
        }
        if ((D_800CC2D0[objIndex].unk2F8 & 7) == 5 && D_800CC2D0[i].unk127 != 0xFF) {
            continue;
        }
        dx = D_800CC2D0[objIndex].x_position - D_800CC2D0[i].x_position;
        dz = D_800CC2D0[objIndex].z_position - D_800CC2D0[i].z_position;
        dy = D_800CC2D0[objIndex].y_position - D_800CC2D0[i].y_position;
        dx = dx * dx;
        dz = dz * dz;
        dy = dy * dy;
        dist = dx + dz + dy;
        if (i == D_800CC2D0[objIndex].unk222 &&
            (D_800CC2D0[objIndex].unk223 == 1 || D_800CC2D0[objIndex].unk223 == 12)) {
            dist = 0.0f;
        }
        if ((dist < maxDist && dist < *bestDist) || preferId == D_800CC2D0[i].id) {
            if (func_15037698(objIndex, refBone, bone, OBJ_AIM(&D_800CC2D0[i]).x, OBJ_AIM(&D_800CC2D0[i]).topY,
                              OBJ_AIM(&D_800CC2D0[i]).z, out, range, noCheck, axisMode)) {
                *bestDist = dist;
                found = i;
                if (preferId == D_800CC2D0[i].id) {
                    break;
                }
            }
        }
    }
    if (found != -1) {
        return D_800CC2D0[found].pad139[2] + 1;
    }
    out[0] = -99999.0f;
    out[1] = -99999.0f;
    out[2] = -99999.0f;
    return 0;
}

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

/*
 * Converts a bone-local direction (x, y, z) to yaw/pitch in degrees, (-180, 180]. axisMode picks which
 * local axis is "forward": 0 +z, 1 +x, 2 -x, 3 -y.
 */
void func_15038468(f32 *yaw, f32 *pitch, f32 x, f32 y, f32 z, s32 axisMode) {
    switch (axisMode) {
    case 0:
        *yaw = func_150484A0(x, z);
        *pitch = func_150484A0(y, sqrtf(x * x + z * z));
        break;
    case 1:
        *yaw = func_150484A0(-z, x);
        *pitch = func_150484A0(y, sqrtf(x * x + z * z));
        break;
    case 2:
        *yaw = func_150484A0(z, -x);
        *pitch = func_150484A0(y, sqrtf(x * x + z * z));
        break;
    case 3:
        *yaw = func_150484A0(z, -y);
        *pitch = func_150484A0(-x, sqrtf(y * y + z * z));
        break;
    }
    *yaw *= 57.295776f;
    *pitch *= 57.295776f;
    if (*yaw > 180.0f) {
        *yaw -= 360.0f;
    }
    if (*pitch > 180.0f) {
        *pitch -= 360.0f;
    }
}

/* D_800CC5E8 is &D_800CC2D0[0].camera; the original code indexes it as its own array (matching: using
 * D_800CC2D0[i].camera instead changes the address arithmetic). */
typedef struct {
    struct108 *camera;
    u8 pad4[0x328];
} ObjCameraSlot;

extern ObjCameraSlot D_800CC5E8[];

/* matching: reading D_800BE9E4 as a member of D_800BE9B4 lets IDO share one address register */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 pad1[0x2F];
    /* 0x30 */ s32 unk30;
} GameStruct_800BE9B4;

void func_15039ED0(s32 arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 *arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8);
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);
extern u8 D_800BEA0C;
extern u8 D_800C3FDC;
extern f32 D_800C3FE0;
extern f32 D_800C3FE4;
extern f32 D_800981A0[][4]; /* per rate index: yaw rate, pitch rate, yaw max step, pitch max step */
s32 func_15021DB8(f32 *, f32 *, f32 *, f32 *, s32, s32);
void func_15049C40(f32 *arg0, f32 *arg1);
void func_15049CB8(f32 m[4][4], f32 *q);
void func_15049EDC(f32 *a, f32 *b, f32 t, f32 *out);
void func_1504A140(f32 *q, f32 m[4][4]);

/*
 * Turns bone matrix `bone` of object objIndex (measured against refBone) toward target (-99999 for none),
 * easing per aim slot through the object's CamData, with turn rates from D_800981A0[rateIdx] and the
 * window from aim limits slot limitIdx. Bit 0/1 of axes enable yaw/pitch.
 */
void func_15038620(s32 bone, s32 refBone, f32 *target, s32 objIndex, s32 limitIdx, s32 rateIdx, s32 slot,
                   s32 axes, s32 noLimit, s32 relative, s32 yawDeadzone, s32 argB, s32 axisMode) {
    struct127 *obj;
    f32 (*refMtx)[4];
    f32 result[4][4];
    f32 scale[3];
    f32 dir[3];
    f32 localX;
    f32 localY;
    f32 localZ;
    f32 yaw;
    f32 pitch;
    f32 roll;
    f32 mtxC[4][4];
    f32 mtxA[4][4];
    f32 mtxB[4][4];
    f32 forward[3];
    f32 origYaw;
    f32 origPitch;
    f32 prevYaw;
    f32 prevPitch;
    f32 dYaw;
    f32 dPitch;
    s32 hasOverride;
    f32 (*mtx)[4];
    s32 timerDone;
    s32 blendMode;
    f32 blendT;
    f32 rateYaw;
    f32 ratePitch;
    f32 maxYaw;
    f32 maxPitch;
    s32 noTarget;
    s32 locked;
    f32 overrideAngles[2];
    f32 finalYaw;
    f32 finalPitch;
    s32 step;
    CamData *data;

    obj = &D_800CC2D0[objIndex];
    data = OBJ_CAM_DATA(obj);
    if (data == NULL) {
        return;
    }
    yaw = 0.0f;
    pitch = 0.0f;
    roll = 0.0f;
    if (D_800C3FFA == 0) {
        obj->unk2CA = 0;
    }
    hasOverride = func_15021DB8(&yaw, &pitch, &roll, target, objIndex, 0);
    if (hasOverride) {
        overrideAngles[0] = yaw;
        overrideAngles[1] = pitch;
    }
    blendMode = 0;
    D_800C3FDC = 0;
    prevPitch = 0.0; // matching: double literals; 0.0f changes IDO's register choice
    prevYaw = 0.0;
    if (target[0] == -99999.0f && target[1] == -99999.0f && target[2] == -99999.0f) {
        noTarget = 1;
    } else {
        noTarget = 0;
    }
    if (noTarget && !hasOverride) {
        if (data->timer[slot] == 0) {
            data->yawVel[slot] = 0.0f;
            data->pitchVel[slot] = 0.0f;
            data->blend[slot] = 10;
            data->yaw[slot] = -99999.0f;
            data->pitch[slot] = -99999.0f;
            data->offYaw[slot] = 0;
            data->offPitch[slot] = 0;
            return;
        }
    }
    locked = obj->unk2FB & 4;
    if (!hasOverride && locked) {
        if (data->flag[slot] == 0) {
            data->blend[slot] = 10;
        }
        if (data->blend[slot] == 0) {
            data->yawVel[slot] = 0.0f;
            data->pitchVel[slot] = 0.0f;
            data->yaw[slot] = -99999.0f;
            data->pitch[slot] = -99999.0f;
            data->offYaw[slot] = 0;
            data->offPitch[slot] = 0;
            return;
        }
    }
    timerDone = 0;
    data->flag[slot] = locked;
    if (data->blend[slot] < 2) {
        if (data->timer[slot] > 0) {
            if (D_800BEAC0 == 0 && D_800BEA0C == 0) {
                --data->timer[slot];
            }
            if (data->timer[slot] == 0) {
                timerDone = 1;
            }
        }
    }
    if (!noTarget) {
        step = data->timer[slot];
        if (step != 0 && step < 10) {
            data->blend[slot] = ((10 - step) * 10) / 10.0f;
            data->timer[slot] = 0;
        }
    }
    forward[1] = 0.0f;
    switch (axisMode) {
    case 0:
        forward[0] = 0.0f;
        forward[2] = 1.0f;
        break;
    case 1:
        forward[2] = 0.0f;
        forward[0] = 1.0f;
        break;
    case 2:
        forward[2] = 0.0f;
        forward[0] = -1.0f;
        break;
    case 3:
        forward[0] = 0.0f;
        forward[2] = 0.0f;
        forward[1] = -1.0f;
        break;
    }
    mtx = OBJ_BONES(obj)[bone];
    refMtx = OBJ_BONES(obj)[refBone];
    mtx[0][3] = mtx[1][3] = mtx[2][3] = refMtx[0][3] = refMtx[1][3] = refMtx[2][3] = 0.0f;
    mtx[3][3] = refMtx[3][3] = 1.0f;
    if (!hasOverride) {
        dir[0] = forward[0] * mtx[0][0] + forward[1] * mtx[1][0] + forward[2] * mtx[2][0];
        dir[1] = forward[0] * mtx[0][1] + forward[1] * mtx[1][1] + forward[2] * mtx[2][1];
        dir[2] = forward[0] * mtx[0][2] + forward[1] * mtx[1][2] + forward[2] * mtx[2][2];
        localX = dir[0] * refMtx[0][0] + dir[1] * refMtx[0][1] + dir[2] * refMtx[0][2];
        localY = dir[0] * refMtx[1][0] + dir[1] * refMtx[1][1] + dir[2] * refMtx[1][2];
        localZ = dir[0] * refMtx[2][0] + dir[1] * refMtx[2][1] + dir[2] * refMtx[2][2];
        func_15038468(&yaw, &pitch, localX, localY, localZ, axisMode);
    }
    if (yaw > 180.0f) {
        yaw -= 360.0f;
    }
    if (pitch > 180.0f) {
        pitch -= 360.0f;
    }
    if (-99999.0f == data->yaw[slot]) {
        data->yaw[slot] = yaw;
        data->pitch[slot] = pitch;
        data->yawVel[slot] = 0.0f;
        data->pitchVel[slot] = 0.0f;
    }
    if (D_800C3FFA == 0) {
        obj->unk2CA = yaw * 0.7111111283f;
    }
    origYaw = yaw;
    origPitch = pitch;
    if ((timerDone && !hasOverride) || D_800BE9B4) {
        func_15039ED0(limitIdx, &yaw, &pitch, &prevYaw, &prevPitch, origYaw, origPitch, objIndex, slot);
        data->yawVel[slot] = 0.0f;
        data->pitchVel[slot] = 0.0f;
        *(volatile f32 *)&data->yaw[slot] = yaw; // matching: IDO would drop this store overwritten below
        data->blend[slot] = 10;
        data->yaw[slot] = -99999.0f;
        data->pitch[slot] = -99999.0f;
        return;
    }
    dir[0] = target[0] - mtx[3][0];
    dir[1] = target[1] - mtx[3][1];
    dir[2] = target[2] - mtx[3][2];
    if (!noTarget || hasOverride) {
        if (!hasOverride) {
            localX = dir[0] * refMtx[0][0] + dir[1] * refMtx[0][1] + dir[2] * refMtx[0][2];
            localY = dir[0] * refMtx[1][0] + dir[1] * refMtx[1][1] + dir[2] * refMtx[1][2];
            localZ = dir[0] * refMtx[2][0] + dir[1] * refMtx[2][1] + dir[2] * refMtx[2][2];
            func_15038468(&yaw, &pitch, localX, localY, localZ, axisMode);
            if (yaw > 0.0f) {
                yaw -= yawDeadzone;
            } else if (yaw < 0.0f) {
                yaw += yawDeadzone;
            }
        } else {
            D_800C3FDC = 1;
            D_800C3FE0 = yaw;
            D_800C3FE4 = pitch;
        }
        if (D_800CC5E8[D_800C3E78].camera != NULL && D_800CC5E8[D_800C3E78].camera->unk2C == 0x100 &&
            (yaw > 100.0f || yaw < -100.0f)) {
            dir[0] = -dir[0];
            dir[1] = 20.0f - dir[1];
            dir[2] = -dir[2];
            localX = dir[0] * refMtx[0][0] + dir[1] * refMtx[0][1] + dir[2] * refMtx[0][2];
            localY = dir[0] * refMtx[1][0] + dir[1] * refMtx[1][1] + dir[2] * refMtx[1][2];
            localZ = dir[0] * refMtx[2][0] + dir[1] * refMtx[2][1] + dir[2] * refMtx[2][2];
            func_15038468(&yaw, &pitch, localX, localY, localZ, axisMode);
        }
        D_800C3FD0[0] = mtx[3][0] + dir[0] * 100.0f;
        D_800C3FD0[1] = mtx[3][1] + dir[1] * 100.0f;
        D_800C3FD0[2] = mtx[3][2] + dir[2] * 100.0f;
        if (noLimit == 0) {
            func_15039ED0(limitIdx, &yaw, &pitch, &prevYaw, &prevPitch, origYaw, origPitch, objIndex, slot);
        }
    } else {
        yaw = origYaw;
        pitch = origPitch;
    }
    if (D_800BEAC0 == 0 && D_800BEA0C == 0) {
        rateYaw = D_800981A0[rateIdx][0];
        ratePitch = D_800981A0[rateIdx][1];
        maxYaw = D_800981A0[rateIdx][2];
        maxPitch = D_800981A0[rateIdx][3];
        if (D_800C35EA == 1) {
            rateYaw = 1.0f;
            ratePitch = 1.0f;
            maxYaw = 5.0f;
            maxPitch = 5.0f;
            if (1) {} // matching: the empty block splits the basic block before the blend checks
        }
        if (data->blend[slot] > 0) {
            blendMode = 2;
            blendT = 10 - data->blend[slot];
            blendT = blendT / 10.0f;
            if (locked == 0) {
                blendT = 1.0f - blendT;
            }
        } else {
            if (data->timer[slot] && data->timer[slot] < 10) {
                blendMode = 1;
                blendT = 10 - data->timer[slot];
                blendT = blendT / 10.0f;
            }
        }
        if (blendMode == 1) {
            rateYaw += (1.0f - rateYaw) * blendT;
            ratePitch += (1.0f - ratePitch) * blendT;
        }
        if (noLimit != 0) {
            if (yaw > 90.0f && data->yaw[slot] < -90.0f) {
                data->yaw[slot] += 360.0f;
            } else if (yaw < -90.0f && data->yaw[slot] > 90.0f) {
                data->yaw[slot] -= 360.0f;
            }
        }
        dYaw = (yaw - data->yaw[slot]) * rateYaw;
        dPitch = (pitch - data->pitch[slot]) * ratePitch;
        if (blendMode != 0) {
            yaw = data->yaw[slot] + dYaw;
            pitch = data->pitch[slot] + dPitch;
        } else {
            if (fabsf(dYaw) < fabsf(data->yawVel[slot]) + maxYaw) {
                data->yawVel[slot] = dYaw;
            } else {
                data->yawVel[slot] += maxYaw * ((dYaw < 0) ? -1 : 1);
            }
            if (fabsf(dPitch) < fabsf(data->pitchVel[slot]) + maxPitch) {
                data->pitchVel[slot] = dPitch;
            } else {
                data->pitchVel[slot] += maxPitch * ((dPitch < 0) ? -1 : 1);
            }
            yaw = data->yaw[slot] + data->yawVel[slot];
            pitch = data->pitch[slot] + data->pitchVel[slot];
        }
        if (noLimit == 0) {
            func_15039ED0(limitIdx, &yaw, &pitch, NULL, NULL, origYaw, origPitch, objIndex, slot);
        }
    } else {
        yaw = data->yaw[slot];
        pitch = data->pitch[slot];
    }
    if (!(axes & 1)) {
        yaw = origYaw;
    }
    if (!(axes & 2)) {
        pitch = origPitch;
    }
    if (hasOverride) {
        blendMode = 0;
        yaw = overrideAngles[0];
        pitch = overrideAngles[1];
    }
    if (D_800C3FFA == 0) {
        obj->unk2CA = yaw * 0.7111111283f;
    }
    finalYaw = data->offYaw[slot] + yaw;
    finalPitch = data->offPitch[slot] + pitch;
    switch (axisMode) {
    case 0:
        func_150A8050(mtxA, 0.0f, 0.0f, roll);
        func_150A8050(mtxB, -finalPitch, finalYaw, 0.0f);
        break;
    case 2:
        func_150A8050(mtxA, 0.0f, 0.0f, -finalPitch);
        func_150A8050(mtxB, roll, finalYaw, 0.0f);
        break;
    case 1:
        func_150A8050(mtxA, 0.0f, 0.0f, finalPitch);
        func_150A8050(mtxB, roll, finalYaw, 0.0f);
        break;
    case 3:
        func_150A8050(mtxA, 0.0f, 0.0f, -finalPitch);
        func_150A8050(mtxB, -finalYaw, roll, 0.0f);
        break;
    }
    func_150A7A48(mtxA, mtxB, mtxC);
    func_150A7A48(mtxC, refMtx, result);
    result[3][0] = mtx[3][0];
    result[3][1] = mtx[3][1];
    result[3][2] = mtx[3][2];
    if (blendMode != 0) {
        f32 qB[4];
        f32 qA[4];
        f32 qOut[4];
        s32 i;
        s32 j;
        bcopy(mtx, mtxA, 0x40);
        bcopy(result, mtxB, 0x40);
        for (i = 0; i < 3; i++) {
            scale[i] = sqrtf(mtx[i][0] * mtx[i][0] + mtx[i][1] * mtx[i][1] + mtx[i][2] * mtx[i][2]);
            localY = 1.0f / scale[i]; // localY reused as the inverse length
            for (j = 0; j < 3; j++) {
                mtxA[j][i] = mtx[j][i] * localY;
            }
            localY = 1.0f / sqrtf(result[i][0] * result[i][0] + result[i][1] * result[i][1] + result[i][2] * result[i][2]);
            for (j = 0; j < 3; j++) {
                mtxB[j][i] = result[j][i] * localY;
            }
        }
        func_15049CB8(mtxB, qB);
        func_15049CB8(mtxA, qA);
        func_15049C40(qB, qA);
        func_15049EDC(qB, qA, blendT, qOut);
        func_1504A140(qOut, result);
    } else {
        scale[0] = 1.0f;
        scale[1] = 1.0f;
        scale[2] = 1.0f;
    }
    if (relative != 0) {
        func_150A7A48(mtxC, mtx, mtxA);
        bcopy(mtxA, mtx, 0x40);
    } else {
        bcopy(result, mtx, 0x40);
        if (blendMode != 0) {
            if (scale[0] != 1.0f) {
                mtx[0][0] *= scale[0];
                mtx[1][0] *= scale[0];
                mtx[2][0] *= scale[0];
            }
            if (scale[1] != 1.0f) {
                mtx[0][1] *= scale[1];
                mtx[1][1] *= scale[1];
                mtx[2][1] *= scale[1];
            }
            if (scale[2] != 1.0f) {
                mtx[0][2] *= scale[2];
                mtx[1][2] *= scale[2];
                mtx[2][2] *= scale[2];
            }
        }
    }
    data->yaw[slot] = yaw;
    data->pitch[slot] = pitch;
    if (D_800BEAC0 == 0 && D_800BEA0C == 0) {
        if (data->blend[slot] > 0) {
            data->blend[slot]--;
        }
    }
    if (!noTarget) {
        data->timer[slot] = 40;
    }

    step = ((GameStruct_800BE9B4 *)&D_800BE9B4)->unk30 * 10;
    if (step < ((data->offYaw[slot] < 0) ? -data->offYaw[slot] : data->offYaw[slot])) {
        data->offYaw[slot] -= step * ((data->offYaw[slot] < 0) ? -1 : 1);
    } else {
        data->offYaw[slot] = 0;
    }
    if (step < ((data->offPitch[slot] < 0) ? -data->offPitch[slot] : data->offPitch[slot])) {
        data->offPitch[slot] -= step * ((data->offPitch[slot] < 0) ? -1 : 1);
    } else {
        data->offPitch[slot] = 0;
    }
}


/* Returns aim limits slot limitIdx (the same for every object). */
AimLimits *func_15039A54(s32 objIndex, s32 limitIdx) {
    return &D_80098068[limitIdx];
}

/*
 * Clamps a yaw/pitch pair to the default limits (slot 0). When the object's camera is not in
 * mode 0x100 and the yaw is past its margin, falls back to the given yaw/pitch instead.
 * The unclamped angles are written to prevYaw/prevPitch when given.
 */
void func_15039A78(f32 *yaw, f32 *pitch, f32 *prevYaw, f32 *prevPitch, f32 fallbackYaw, f32 fallbackPitch,
                   s32 objIndex) {
    AimLimits *lim;
    struct108 *camera;

    camera = D_800CC2D0[objIndex].camera;
    lim = func_15039A54(objIndex, 0);
    if (camera != NULL && camera->unk2C != 0x100) {
        if (lim->yawMax + lim->yawMargin < *yaw || *yaw < lim->yawMin - lim->yawMargin) {
            *yaw = fallbackYaw;
            *pitch = fallbackPitch;
            if (prevYaw != NULL) {
                *prevYaw = *yaw;
                *prevPitch = *pitch;
            }
        } else {
            if (prevYaw != NULL) {
                *prevYaw = *yaw;
                *prevPitch = *pitch;
            }
            *yaw = (*yaw < lim->yawMin) ? lim->yawMin : ((lim->yawMax < *yaw) ? lim->yawMax : *yaw);
            if (lim->pitchMax + lim->pitchMargin < *pitch || *pitch < lim->pitchMin - lim->pitchMargin) {
                *pitch = fallbackPitch;
            } else {
                *pitch = (*pitch < lim->pitchMin) ? lim->pitchMin : ((lim->pitchMax < *pitch) ? lim->pitchMax : *pitch);
            }
        }
    } else {
        *yaw = (*yaw < lim->yawMin) ? lim->yawMin : ((lim->yawMax < *yaw) ? lim->yawMax : *yaw);
        *pitch = (*pitch < lim->pitchMin) ? lim->pitchMin : ((lim->pitchMax < *pitch) ? lim->pitchMax : *pitch);
    }
}

/*
 * Per-frame aim bookkeeping: object 0x53 in anim 0x15 (or any object while D_800BE9B4 is set) resets all aim
 * slots, and an object whose type has an "aim-locked" anim list gets unk2FB bit 2 set while it plays one of
 * them, or (for interaction state 1) while dead, slow in water, flagged in unk31C, or moving fast.
 */
void func_15039CC8(struct127 *obj) {
    s32 id;
    s32 i;
    s32 group;
    s32 count;
    u16 *anims;
    CamData *data;

    data = OBJ_CAM_DATA(obj);
    if (data != NULL) {
        id = obj->id;
        if ((id == 0x53 && obj->unk84.uh == 0x15) || D_800BE9B4 != 0) {
            for (i = 0; i < 3; i++) {
                data->timer[i] = 40;
                data->blend[i] = 0;
                data->flag[i] = 4;
            }
        }

        group = D_80097E7C[id];
        if (group != 0) {
            group--;
            anims = D_80084380[group];
            count = D_80098050[group];
            for (i = 0; i < count; i++) {
                if (obj->unk84.uh == anims[i]) {
                    obj->unk2FB |= 4;
                    return;
                }
            }

            if (obj->interaction_state == 1) {
                if (obj->health == 0 || (obj->in_water != 0 && obj->xz_velocity < 5.0f) ||
                    obj->unk31C->pad196[1] != 0 /* +0x197 */ || obj->xz_velocity > 20.0f) {
                    obj->unk2FB |= 4;
                }
            } else if (obj->health == 0) {
                obj->unk2FB |= 4;
            }
        }
    }
}

/* func_15039A78 for limits slot limitIdx (0 uses func_15039A78's camera-aware rules). */
void func_15039ED0(s32 limitIdx, f32 *yaw, f32 *pitch, f32 *prevYaw, f32 *prevPitch, f32 fallbackYaw,
                   f32 fallbackPitch, s32 objIndex, s32 slot) {
    AimLimits *lim;

    if (limitIdx == 0) {
        func_15039A78(yaw, pitch, prevYaw, prevPitch, fallbackYaw, fallbackPitch, objIndex);
        return;
    }
    lim = func_15039A54(objIndex, limitIdx);
    if (lim->yawMax + lim->yawMargin < *yaw || *yaw < lim->yawMin - lim->yawMargin) {
        *yaw = fallbackYaw;
        *pitch = fallbackPitch;
        if (prevYaw != NULL) {
            *prevYaw = *yaw;
            *prevPitch = *pitch;
        }
    } else {
        if (prevYaw != NULL) {
            *prevYaw = *yaw;
            *prevPitch = *pitch;
        }
        *yaw = (*yaw < lim->yawMin) ? lim->yawMin : ((lim->yawMax < *yaw) ? lim->yawMax : *yaw);
        if (lim->pitchMax + lim->pitchMargin < *pitch || *pitch < lim->pitchMin - lim->pitchMargin) {
            *pitch = fallbackPitch;
        } else {
            *pitch = (*pitch < lim->pitchMin) ? lim->pitchMin : ((lim->pitchMax < *pitch) ? lim->pitchMax : *pitch);
        }
    }
}

/* Per-type object definitions in D_800D1C90 (include/structs.h types that array as struct124, which does not
 * match these offsets); only the fields used here. */
typedef struct {
    /* 0x00 */ s16 unk0; /* aim height above the bone */
    /* 0x02 */ u8 pad2[0x24];
    /* 0x26 */ u8 unk26; /* bone the aim points are taken from */
    /* 0x27 */ u8 pad27[5];
    /* 0x2C */ s8 unk2C; /* head turn range, yaw left/right and pitch up/down (see func_1503A830) */
    /* 0x2D */ s8 unk2D;
    /* 0x2E */ s8 unk2E;
    /* 0x2F */ s8 unk2F;
    /* 0x30 */ s8 unk30; /* aim reach along the bone's z axis */
    /* 0x31 */ u8 pad31[9];
    /* 0x3A */ u8 unk3A; /* aim half-width along the bone's x axis */
} ObjTypeDef;

s32 func_1503D774(s32 arg0, s32 arg1);
void func_1507C3E0(struct127 *arg0, s16 *arg1, s16 *arg2, s16 *arg3);


/*
 * Recomputes an object's aim points (OBJ_AIM) from its type's aim bone: the bone origin pushed out by
 * unk30 along z, a top point unk0 up along y, two side points unk3A out along +-x, and a z direction in
 * dir. Objects without that bone data get points from their position and func_1507C3E0's height.
 */
void func_1503A08C(struct127 *obj) {
    ObjTypeDef *def;
    f32 (*bone)[4];
    f32 vx;
    f32 vy;
    f32 vz;
    f32 mag;
    f32 sideX;
    f32 sideY;
    f32 sideZ;
    f32 width;
    f32 upX;
    f32 upY;
    f32 upZ;
    f32 len;
    s16 height;
    s16 size2;
    s16 size3;
    s32 topHeight;
    f32 scale;

    if (obj->id != 0xFF) {
        if ((obj->unk1D4 != NULL) && (D_800D1C90[obj->id] != NULL)) {
            if (func_1503D774(obj->id, 0) == 0) {
                def = (ObjTypeDef *)D_800D1C90[obj->id];
                bone = (f32 (*)[4])obj->unk1D4;
                bone = ((f32 (*)[4][4])bone)[def->unk26]; // matching: two steps; one OBJ_BONES() index mismatches
                scale = obj->y_scale;
                width = def->unk3A * scale;
                if (width != 0) {
                    vx = bone[0][0];
                    vy = bone[0][1];
                    vz = bone[0][2];
                    len = vx * vx + vy * vy + vz * vz;
                    mag = len;
                    if (len != 0) {
                        mag = sqrtf(len);
                    }
                    if (mag != 0) {
                        mag = width / mag;
                    }
                    sideX = vx * mag;
                    sideY = vy * mag;
                    sideZ = vz * mag;
                } else {
                    sideZ = 0.0f;
                    sideY = 0.0f;
                    sideX = 0.0f;
                }
                len = def->unk0 * scale;
                if (len != 0) {
                    vx = bone[1][0];
                    vy = bone[1][1];
                    vz = bone[1][2];
                    mag = sqrtf(vx * vx + vy * vy + vz * vz);
                    if (mag != 0) {
                        mag = len / mag;
                    }
                    upX = vx * mag;
                    upY = vy * mag;
                    upZ = vz * mag;
                } else {
                    upX = upY = upZ = 0.0f;
                }
                vx = bone[2][0];
                vy = bone[2][1];
                vz = bone[2][2];
                len = def->unk30 * scale;
                if (len != 0) {
                    mag = sqrtf(vx * vx + vy * vy + vz * vz);
                    if (mag != 0) {
                        mag = len / mag;
                    }
                    vx *= mag;
                    vy *= mag;
                    vz *= mag;
                    mag = 127.0f / len;
                    OBJ_AIM(obj).dir[0] = vx * mag;
                    OBJ_AIM(obj).dir[1] = vy * mag;
                    OBJ_AIM(obj).dir[2] = vz * mag;
                } else {
                    mag = sqrtf(vx * vx + vy * vy + vz * vz);
                    if (mag != 0) {
                        mag = 127.0f / mag;
                    }
                    OBJ_AIM(obj).dir[0] = vx * mag;
                    OBJ_AIM(obj).dir[1] = vy * mag;
                    OBJ_AIM(obj).dir[2] = vz * mag;
                    vx = vy = vz = 0.0f;
                }
                OBJ_AIM(obj).x = bone[3][0] + vx;
                OBJ_AIM(obj).y = bone[3][1] + vy;
                OBJ_AIM(obj).z = bone[3][2] + vz;
                OBJ_AIM(obj).topY = bone[3][1] + upY;
                mag = OBJ_AIM(obj).x;
                OBJ_AIM(obj).sideA[0] = mag + sideX + upX;
                OBJ_AIM(obj).sideA[1] = OBJ_AIM(obj).y + sideY + upY;
                OBJ_AIM(obj).sideA[2] = OBJ_AIM(obj).z + sideZ + upZ;
                OBJ_AIM(obj).sideB[0] = mag - sideX + upX;
                OBJ_AIM(obj).sideB[1] = OBJ_AIM(obj).y - sideY + upY;
                OBJ_AIM(obj).sideB[2] = OBJ_AIM(obj).z - sideZ + upZ;
            }
        } else {
            def = (ObjTypeDef *)D_800D1C90[obj->id];
            if (def != NULL) {
                topHeight = def->unk0;
            } else {
                topHeight = 0;
            }
            func_1507C3E0(obj, &height, &size2, &size3);
            OBJ_AIM(obj).x = obj->x_position;
            OBJ_AIM(obj).y = obj->y_position + (height * 7) / 8;
            OBJ_AIM(obj).z = obj->z_position;
            OBJ_AIM(obj).topY = OBJ_AIM(obj).y + topHeight * obj->y_scale;
            OBJ_AIM(obj).sideA[0] = OBJ_AIM(obj).x;
            OBJ_AIM(obj).sideB[0] = OBJ_AIM(obj).x;
            OBJ_AIM(obj).sideA[1] = OBJ_AIM(obj).topY;
            OBJ_AIM(obj).sideB[1] = OBJ_AIM(obj).topY;
            OBJ_AIM(obj).sideA[2] = OBJ_AIM(obj).z;
            OBJ_AIM(obj).sideB[2] = OBJ_AIM(obj).z;
            obj->unk2CA = 0;
        }
    }
}

typedef struct {
    u8  pad0[0x30];
    f32 unk30;
    f32 unk34;
    f32 unk38;
} GameStruct_1503A60C;

void func_1503A60C(void) {
    GameStruct_1503A60C *dst = (GameStruct_1503A60C *)((u8 *)D_800CC2D0[D_800C3E78].unk1D4 + 0x40);

    dst->unk30 = *(f32 *)&D_800CC2D0[D_800C3E78].pad174;
    dst->unk34 = D_800CC2D0[D_800C3E78].y_position;
    dst->unk38 = *(f32 *)&D_800CC2D0[D_800C3E78].pad178;
}

extern u8 D_800C3FFA;
extern u8 D_80084404[];
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);

void func_1503A678(void) {
    struct127 *obj;
    f32 (*m)[4];
    f32 sz;
    f32 sy;
    f32 sx;
    f32 f;
    f32 inv;
    f32 mtx[4][4];
    f32 tmp[4][4];
    f32 pos[3];

    obj = &D_800CC2D0[D_800C3E78];
    if (obj->id == 0x22) {
        f = *((u8 *)&D_800CC2D0[D_800C3E78] + 0x20A);
        f /= 64.0f;
        f += 1.0f;
        if (D_800C3FFA == 0) {
            sx = f;
            sy = 1.0f;
            sz = f;
        } else {
            sx = 1.0f / f;
            sy = 1.0f;
            sz = 1.0f / f;
        }
        m = (f32 (*)[4])((u8 *)D_800CC2D0[D_800C3E78].unk1D4 + (D_80084404[D_800C3FFA] << 6));
        m[3][3] = 1.0f;
        m[0][3] = 0.0f;
        m[1][3] = 0.0f;
        m[2][3] = 0.0f;
        pos[0] = m[3][0];
        pos[1] = m[3][1];
        pos[2] = m[3][2];
        guMtxIdentF(mtx);
        func_150A7CB0(mtx, *(s32 *)&sx, *(s32 *)&sy, *(s32 *)&sz);
        func_150A7A48(m, mtx, tmp);
        bcopy(tmp, m, 0x40);
        m[3][0] = pos[0];
        m[3][1] = pos[1];
        m[3][2] = pos[2];
        D_800C3FFA++;
    }
}

extern u8 D_800C3FFA;
extern void func_15036F34(void);
extern void func_1503A678(void);

void func_1503A7F0(void) {
    s32 temp = D_800C3FFA;
    D_800C3FFA = 0;
    func_15036F34();
    D_800C3FFA = temp;
    func_1503A678();
}
f32 func_15048A40(u8 arg0);

#define VEC0 D_800C3FE8[0]
#define VEC1 D_800C3FE8[1]
#define VEC2 D_800C3FE8[2]
extern u8 D_800C3FDC;
extern f32 D_800C3FE0;
extern f32 D_800C3FE4;
f32 func_150489B0(u8);
s32 func_15022024(f32 *, f32 *, s32, s32);

typedef struct {
    u8 pad0[0x2C];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    s8 unk2F;
} Def_1503A830;

#define A_U8(o) (*(u8 *)((u8 *)arg0 + (o)))
#define A_S8(o) (*(s8 *)((u8 *)arg0 + (o)))
#define A_U16(o) (*(u16 *)((u8 *)arg0 + (o)))
#define A_S16(o) (*(s16 *)((u8 *)arg0 + (o)))
#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define APPROACH(fld, tgt)                                       \
    if (ABS((tgt) - (fld)) < step) {                             \
        (fld) = (tgt);                                           \
    } else {                                                     \
        (fld) += step * (((tgt) - (fld)) < 0 ? -1 : 1);         \
    }

void func_1503A830(struct127 *arg0) {
    s32 id;
    s32 type;
    s32 color;
    s32 doCall;
    f32 (*m)[4];
    f32 d[3];
    f32 a[2];
    f32 b[2];
    f32 fA;
    f32 fB;
    f32 fC;
    f32 fD;
    f32 padB4;
    f32 padB0;
    f32 vx;
    f32 vy;
    f32 vz;
    s32 step;
    u8 *p;
    u8 *q;
    s32 copyAB;
    s32 clamp;
    s32 snap;
    s32 n;
    s32 doAim;
    s32 noFx;
    Def_1503A830 *def;

    if (arg0->unk1D4 == NULL) {
        return;
    }
    VEC0 = D_800C3FD0[0];
    VEC1 = D_800C3FD0[1];
    VEC2 = D_800C3FD0[2];
    id = arg0->id;
    step = 8;
    color = 0x100;
    doCall = 1;
    copyAB = 1;
    clamp = 1;
    snap = 0;
    doAim = 1;
    noFx = 1;
    switch (id) {
        case 0x9A:
            type = 0xD;
            break;
        case 0x92:
            type = 3;
            break;
        case 0x52:
            type = 0;
            break;
        case 21:
            type = 3;
            break;
        case 0x54:
            type = 0x1D;
            break;
        case 0x45:
            type = 3;
            break;
        case 24:
        case 30:
            type = 2;
            if (id == 30) {
                color = 0x22;
            }
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 0x82:
        case 0x96:
            type = 0xC;
            doCall = 0;
            break;
        default:
            return;
    }
    if (doCall) {
        func_150379DC(D_800C3FE8, type, type, arg0 - D_800CC2D0, 90.0f, color, 0);
    }
    m = (f32 (*)[4])((u8 *)D_800CC2D0[arg0 - D_800CC2D0].unk1D4 + (type << 6));
    m[3][3] = 1.0f;
    m[2][3] = 0.0f;
    m[1][3] = 0.0f;
    m[0][3] = 0.0f;
    a[0] = b[0] = 0.0f;
    if (A_U16(0x282) == 0) {
        A_U16(0x278) = 0;
        A_S8(0x284) = 0;
        A_S8(0x285) = 0;
        A_S8(0x286) = 0;
        A_S8(0x287) = 0;
    }
    if (A_U16(0x282) != 0) {
        doAim = 0;
        if (A_U8(0x276) == 0) {
            s32 lo;
            s32 hi;
            f32 s;
            f32 c;
            s32 pad68;
            f32 scale;
            f32 k;
            A_S16(0x27A) -= A_S8(0x284);
            A_S16(0x27E) -= A_S8(0x285);
            A_S16(0x27C) -= A_S8(0x286);
            A_S16(0x280) -= A_S8(0x287);
            lo = A_U16(0x278) & 0xFF;
            hi = A_U16(0x278) >> 8;
            lo += D_800BE9E4 * 3;
            hi -= D_800BE9E4 * 4;
            A_U16(0x278) = (hi << 8) | lo;
            if (A_U16(0x282) < 30) {
                scale = (f32)(A_U16(0x282) + 0U) / 30.0f;
            } else {
                scale = 1.0f;
            }
            s = func_15048A40(lo);
            c = func_150489B0(lo);
            k = 11.0f * scale;
            A_S8(0x284) = s * k;
            A_S8(0x285) = c * k;
            s = func_15048A40(hi);
            c = func_150489B0(hi);
            A_S8(0x286) = s * k;
            A_S8(0x287) = c * k;
        } else if (A_U8(0x276) == 1) {
            if (A_U16(0x278) == 0) {
                A_U16(0x278) = 1;
                A_S8(0x287) += func_150ADA20() % 20U;
            }
            if (A_U16(0x282) < 0xAE) {
                n = A_U16(0x282) / 6 + 1;
            } else {
                n = 30;
            }
            A_S8(0x284) = 0;
            A_S8(0x285) += n;
            A_S8(0x287) += n;
            A_S8(0x286) = A_S8(0x284);
            snap = 1;
            a[0] = A_S8(0x284);
            b[0] = A_S8(0x285);
            a[1] = A_S8(0x286);
            b[1] = A_S8(0x287);
        } else if (A_U8(0x276) == 2) {
            u8 *r;

            doAim = 1;
            p = (u8 *)func_1503195C(arg0, 0x2B, 0);
            if (p != NULL) {
                q = *(u8 **)(p + 0x48);
                if (q != NULL) {
                    r = ((u8 **)(q + 0x3E8))[D_800BE9C0];
                    if (r != NULL) {
                        r += 0x80;
                        func_15043FF0((vertex *)D_800C3FE8, (struct113 *)r);
                        noFx = 0;
                    }
                }
            }
        } else if (A_U8(0x276) == 3) {
            f32 (*mtx)[4];

            if (A_U16(0x278) != -1) {
                mtx = (f32 (*)[4])arg0->unk1D4;
                if (mtx != NULL) {
                    mtx = ((f32 (*)[4][4])mtx)[A_U16(0x278)];
                    VEC0 = mtx[3][0];
                    VEC1 = mtx[3][1];
                    VEC2 = mtx[3][2];
                }
            }
        } else if (A_U8(0x276) == 4) {
            if (arg0->unk84.uh == 0x26) {
                if (A_U16(0x278) == 0) {
                    A_S8(0x284) -= D_800BE9E4;
                    if (A_S8(0x284) < -20) {
                        A_S8(0x284) = -20;
                        A_U16(0x278) = 1;
                    }
                } else {
                    A_S8(0x284) += D_800BE9E4 * 3;
                    if (A_S8(0x284) > 20) {
                        A_S8(0x284) = 20;
                        A_U16(0x278) = 0;
                    }
                }
                A_S8(0x287) = 0;
                A_S8(0x286) = A_S8(0x284);
                A_S8(0x285) = A_S8(0x287);
                snap = 1;
            } else {
                A_U16(0x282) = 0;
                doAim = 1;
            }
        } else if (A_U8(0x276) == 5) {
            f32 kx = 0.3543307185f;
            f32 ky = 0.236220479f;

            a[0] = A_S8(0x284) * kx;
            a[1] = A_S8(0x286) * kx;
            b[0] = A_S8(0x285) * ky;
            b[1] = A_S8(0x287) * ky;
            clamp = 0;
            copyAB = 0;
        } else if (A_U8(0x276) == 6) {
            if (arg0->unk31C->unk78 == 0x2A) {
                b[0] = 30.0f;
                a[0] = -6.0f;
                a[1] = 6.0f;
            } else {
                a[0] = 0;
                a[1] = 0;
                b[0] = 0;
                if (A_S16(0x27E) == 0) {
                    A_U8(0x276) = 0;
                }
            }
            copyAB = 0;
            b[1] = b[0];
        }
        if (A_U16(0x282) != 0xFFFF) {
            if (A_U16(0x282) >= D_800BE9E4) {
                A_U16(0x282) -= D_800BE9E4;
            } else {
                A_U16(0x282) = 0;
            }
        }
    }
    if (doAim) {
        if (!(A_U8(0x2FB) & 4) || !noFx) {
            if (VEC0 != -99999.0f || VEC1 != -99999.0f || VEC2 != -99999.0f) {
            if (D_800C3FDC != 0) {
                D_800C3FDC = 0;
                a[0] = D_800C3FE0;
                b[0] = D_800C3FE4;
            } else {
                d[0] = VEC0 - m[3][0];
                d[1] = VEC1 - m[3][1];
                d[2] = VEC2 - m[3][2];
                vx = d[0] * m[0][0] + d[1] * m[0][1] + d[2] * m[0][2];
                vy = d[0] * m[1][0] + d[1] * m[1][1] + d[2] * m[1][2];
                vz = d[0] * m[2][0] + d[1] * m[2][1] + d[2] * m[2][2];
                a[0] = func_150484A0(vx, vz);
                b[0] = func_150484A0(vy, sqrtf(vx * vx + vz * vz));
                a[0] *= 57.29577637f;
                b[0] *= 57.29577637f;
                if (a[0] > 180.0f) {
                    a[0] -= 360.0f;
                }
                if (b[0] > 180.0f) {
                    b[0] -= 360.0f;
                }
            }
            } else {
                step = 4;
            }
        } else {
            a[0] = b[0] = 0.0f;
        }
    }
    if (A_U16(0x282) == 0 && func_15022024(a, b, arg0 - D_800CC2D0, 0)) {
        clamp = 1;
        copyAB = 0;
        snap = 0;
        step = 8;
    }
    if (clamp) {
        a[0] = CLAMP(a[0], -45.0f, 45.0f);
        b[0] = CLAMP(b[0], -15.0f, 30.0f);
        a[1] = CLAMP(a[1], -45.0f, 45.0f);
        b[1] = CLAMP(b[1], -15.0f, 30.0f);
    }
    if (copyAB) {
        a[1] = a[0];
        b[1] = b[0];
    }
    if (D_800D1C90[id] == NULL) {
        return;
    }
    fA = (a[0] >= 0.0f) ? ((Def_1503A830 *)D_800D1C90[id])->unk2D * a[0] / 45.0f : ((Def_1503A830 *)D_800D1C90[id])->unk2C * a[0] / -45.0f;
    fB = (a[1] >= 0.0f) ? ((Def_1503A830 *)D_800D1C90[id])->unk2C * a[1] / 45.0f : ((Def_1503A830 *)D_800D1C90[id])->unk2D * a[1] / -45.0f;
    fB = -fB;
    fC = (b[0] >= 0.0f) ? ((Def_1503A830 *)D_800D1C90[id])->unk2E * b[0] / 30.0f : ((Def_1503A830 *)D_800D1C90[id])->unk2F * b[0] / -15.0f;
    fD = (b[1] >= 0.0f) ? ((Def_1503A830 *)D_800D1C90[id])->unk2E * b[1] / 30.0f : ((Def_1503A830 *)D_800D1C90[id])->unk2F * b[1] / -15.0f;
    APPROACH(A_S16(0x27A), (s32)fB);
    APPROACH(A_S16(0x27C), (s32)fA);
    APPROACH(A_S16(0x27E), (s32)fD);
    APPROACH(A_S16(0x280), (s32)fC);
    if (snap) {
        A_S16(0x27A) = A_S8(0x284);
        A_S16(0x27E) = A_S8(0x285);
        A_S16(0x27C) = A_S8(0x286);
        A_S16(0x280) = A_S8(0x287);
    }
    if (A_U16(0x282) != 0 && A_U8(0x276) == 0) {
        A_S16(0x27A) += A_S8(0x284);
        A_S16(0x27E) += A_S8(0x285);
        A_S16(0x27C) += A_S8(0x286);
        A_S16(0x280) += A_S8(0x287);
    }
}

void func_1503B708(void *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4,
                   s32 arg5, f32 *arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10) {
    f32 *scratch;
    f32 value;

    scratch = D_800C3FD0;
    value = -99999.0f;
    *(f32 *)((u8 *)arg0 + 0) = value;
    *(f32 *)((u8 *)arg0 + 4) = value;
    *(f32 *)((u8 *)arg0 + 8) = value;
    scratch[0] = value;
    scratch[1] = value;
    scratch[2] = value;
    if (func_15037880(arg3, arg0) == 0) {
        func_150380C0(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
                      arg8, arg9, arg10);
    }
}
