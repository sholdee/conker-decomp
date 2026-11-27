#include <ultra64.h>
// This file's .bss, defined before functions.h and variables.h declare the globals (conker/aim.h): IDO lays
// out .bss in the order it first sees each name, and gAimTarget is static.
f32 gAimLookPoint[3];
u8 gAimHasOverride;
f32 gAimOverrideYaw;
f32 gAimOverridePitch;
static f32 gAimTarget[3]; /* current aim target */
s16 gAimFixedPoint[3];
u8 gAimPass; /* aim pass / slot counter */

#include "functions.h"
#include "variables.h"

/*
 * Per group (gNoAimAnimGroups[character_type] - 1): the animations during which an object does not aim
 * (aim_updateLock). gNoAimAnimCounts holds the length of each group's list.
 */
static const u16 gNoAimAnimList11[] = {
    0x16, 0xEB, 0xD, 0x19F, 0xE, 8, 0xF0, 0x30, 0x72, 0xEF, 0x58, 0xF1, 0xF2, 0x59, 0xF5, 0x14, 0x13,
    7, 0x3E, 0x3D, 0x41, 0x25, 0x2C, 0xA9, 0x24, 0xAD, 0x2E, 0x2D, 0x69, 0x2F, 0x171, 0x173, 0x174,
    0x17A, 0x172, 0xF6, 0x11, 0x26, 0x29, 0x7C, 0x55, 0x56, 0x57, 0x49, 0x47, 0x35, 0x3A, 0x3F, 0x5D,
    0x5E, 0x5F, 0x60, 0x61, 0x63, 0x82, 0x87, 0x88, 0x89, 0x8A, 0x67, 0x73, 0x86, 0x9A, 0x9B, 0xAF,
    0x94, 0x76, 0x77, 0x78, 0xBD, 0xC4, 0xC5, 0xC6, 0xC7, 0xBE, 0xBF, 0xC0, 0xC1, 0xB4, 0xB5, 0xB6,
    0xD9, 0xFA, 0x11B, 0x11C, 0x11D, 0x12C, 0x12D, 0x142, 0x12E, 0x10A, 0x3E, 0x3A, 0x18A, 0x18B,
    0x1B9, 0x1BA, 0x1BB, 0x1BC, 0x1CB, 0x1CC, 0x20E, 0x20F, 0x25A, 0x25B, 0x256, 0x25F, 0x1CD, 0x1CE,
    0x1CF, 0x1D0, 0x26F, 0x270, 0x271, 0x272, 0x273, 0x274
};
static const u8 gNoAimAnimGroups[] = {
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x12, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x15, 0x00, 0x00, 0x0E, 0x00, 0x04, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00,
    0x0A, 0x00, 0x00, 0x05, 0x0B, 0x00, 0x00, 0x00, 0x17, 0x00, 0x10, 0x11, 0x00, 0x00, 0x16, 0x10,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x10, 0x0C, 0x00, 0x00, 0x00, 0x13, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x14, 0x06, 0x10, 0x00, 0x00,
    0x0C, 0x00, 0x00, 0x06, 0x00, 0x00, 0x0C, 0x18, 0x00, 0x00, 0x0A, 0x0A, 0x0C, 0x0C, 0x00, 0x0C,
    0x0C, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C,
    0x0C, 0x0C, 0x02, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const u16 gNoAimAnimList17[] = { 4, 5, 6, 7, 9, 0xA, 0xB };
static const u16 gNoAimAnimList18[] = { 1, 3, 4, 5, 6 };
static const u16 gNoAimAnimList19[] = { 1 };
static const u16 gNoAimAnimList20[] = { 0, 1, 2, 3, 4, 5 };
static const u16 gNoAimAnimList14[] = { 0, 1, 2, 3, 4, 5, 6 };
static const u16 gNoAimAnimList13[] = { 1, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 2 }; /* 11 entries; gNoAimAnimCounts counts 10 */
static const u16 gNoAimAnimList00[] = { 1, 2, 5, 6, 8, 9 };
static const u16 gNoAimAnimList01[] = { 2, 3, 5 };
static const u16 gNoAimAnimList02[] = { 0, 2, 5, 0x22 };
static const u16 gNoAimAnimList03[] = { 2, 3, 6, 7, 8, 9 };
static const u16 gNoAimAnimList04[] = {
    2, 3, 6, 7, 8, 9, 0xC, 0xE, 0x11, 0x12, 0x13, 0x19, 0xB, 0xD, 0x10, 0x15, 0x1D, 0x1C, 0x22, 0x1F,
    4, 5, 0xD, 0x10
};
static const u16 gNoAimAnimList05[] = { 0, 3, 4, 5 };
static const u16 gNoAimAnimList06[] = { 0x15 };
static const u16 gNoAimAnimList07[] = { 0, 6, 9, 0xA, 0xB, 0x18 };
static const u16 gNoAimAnimList08[] = { 0xF };
static const u16 gNoAimAnimList09[] = { 0xD, 0xE, 0x10, 0x16, 0x12, 0x1E };
static const u16 gNoAimAnimList10[] = { 0xF };
static const u16 gNoAimAnimList12[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0x10, 0x11, 0x12, 0x13 };
static const u16 gNoAimAnimList15[] = { 0x25 };
static const u16 gNoAimAnimList16[] = { 8, 9 };
static const u16 gNoAimAnimList22[] = { 0x3B, 0x3C };
static const u16 gNoAimAnimList21[] = { 0xB, 0x13, 0x15, 0x14 };
static const u16 gNoAimAnimList23[] = { 0x1A, 0, 0, 0 };
static const u8 gNoAimAnimCounts[] = {
    0x06, 0x03, 0x04, 0x06, 0x18, 0x04, 0x01, 0x06, 0x01, 0x06, 0x01, 0x75, 0x12, 0x0A, 0x07, 0x01,
    0x02, 0x07, 0x05, 0x01, 0x06, 0x04, 0x02, 0x04,
};

static const u16 *gNoAimAnimLists[] = {
    gNoAimAnimList00, gNoAimAnimList01, gNoAimAnimList02, gNoAimAnimList03,
    gNoAimAnimList04, gNoAimAnimList05, gNoAimAnimList06, gNoAimAnimList07,
    gNoAimAnimList08, gNoAimAnimList09, gNoAimAnimList10, gNoAimAnimList11,
    gNoAimAnimList12, gNoAimAnimList13, gNoAimAnimList14, gNoAimAnimList15,
    gNoAimAnimList16, gNoAimAnimList17, gNoAimAnimList18, gNoAimAnimList19,
    gNoAimAnimList20, gNoAimAnimList21, gNoAimAnimList22, gNoAimAnimList23,
};



/* Yaw/pitch window an aim may turn within; beyond max + margin (or min - margin) it gives up. */
struct AimLimits {
    /* 0x00 */ f32 yawMax;
    /* 0x04 */ f32 yawMin;
    /* 0x08 */ f32 pitchMax;
    /* 0x0C */ f32 pitchMin;
    /* 0x10 */ f32 yawMargin;
    /* 0x14 */ f32 pitchMargin;
};

/* The aim limits slots (aim_getLimits). */
static const AimLimits gAimLimits[] = {
    { 70.0f, -70.0f, 47.0f, -47.0f, 30.0f, 43.0f },
    { 68.0f, -80.0f, 47.0f, -47.0f, 30.0f, 43.0f },
    { 70.0f, -70.0f, 30.0f, -10.0f, 20.0f, 60.0f },
    { 400.0f, -400.0f, 400.0f, -400.0f, 0.0f, 0.0f },
    { 120.0f, -120.0f, 90.0f, -40.0f, 50.0f, 30.0f },
    { 70.0f, -70.0f, 150.0f, -40.0f, 50.0f, 30.0f },
    { 90.0f, -90.0f, 80.0f, -80.0f, 30.0f, 30.0f },
    { 60.0f, -60.0f, 90.0f, -40.0f, 80.0f, 30.0f },
    { 70.0f, -70.0f, 40.0f, -40.0f, 20.0f, 30.0f },
    { 70.0f, -70.0f, 90.0f, -90.0f, 0.0f, 0.0f },
    { 90.0f, -90.0f, 90.0f, -90.0f, 0.0f, 0.0f },
    { 180.0f, -180.0f, 90.0f, -90.0f, 0.0f, 0.0f },
    { 35.0f, -35.0f, 90.0f, -90.0f, 45.0f, 45.0f },
};

/* Per-object aim easing state (Actor +0x324), one entry per aim slot (pass, gAimPass). */
typedef struct CamData {
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

/* An object's ob_matrix_ptr points at its array of 4x4 bone matrices. */
#define OBJ_BONES(obj) ((obj)->ob_matrix_ptr)

/* Allocates an object's camera-aim state and marks every slot as having no target. */
void aim_initState(Actor *obj) {
    s32 i;

    obj->camData = allocate_memory(sizeof(CamData), 1, 0, 0);
    bzero(obj->camData, sizeof(CamData));
    for (i = 0; i < 3; i++) {
        obj->camData->yaw[i] = -99999.0f;
        obj->camData->pitch[i] = -99999.0f;
    }
}


/*
 * Aim update for the current object (dc_glbl_CharNum): the first pass (gAimPass == 0) turns bone 7, later passes
 * bone 12. The listed object types, and every type while gCutsceneStates == 1, skip the first pass.
 */
void func_15036CE8(void) {
    Actor *obj;
    s32 limitIdx;
    CamData *data;
    s32 pad[4]; // matching: -g3 gives every local a stack slot; these keep the frame layout
    s32 argB;
    s32 pad2;
    u8 bone;

    obj = &movement_obj[dc_glbl_CharNum];
    data = obj->camData;
    if (data == NULL) {
        return;
    }
    switch (obj->character_type) {
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
            if (gAimPass == 0) {
                gAimPass = 1;
                return;
            }
            break;
    }
    if (gCutsceneStates[0] == 1 && gAimPass == 0) {
        gAimPass = 1;
        return;
    }
    limitIdx = 0;
    argB = 1;
    if (gAimPass == 0) {
        bone = 7;
        argB = 0;
        limitIdx = 1;
        if (gCurrentActor->unk2F8 & 0x100) {
            data->offYaw[0] = 45;
            data->offPitch[0] = 64;
        } else if (data->yaw[0] == -99999.0f) {
            gAimPass = 1;
            return;
        }
    } else {
        bone = 12;
        ; // matching: the empty statement steers IDO's scheduling of the -99999.0f load below
    }
    if (gAimPass == 0 && !(gCurrentActor->unk2F8 & 0x100)) {
        gAimTarget[0] = -99999.0f;
        gAimTarget[1] = -99999.0f;
        gAimTarget[2] = -99999.0f;
    } else {
        aim_findTarget(gAimTarget, bone, 0, dc_glbl_CharNum, 90.0f, 0xFF, 0);
    }
    aim_turnBone(bone, 0, gAimTarget, dc_glbl_CharNum, limitIdx, 0, gAimPass, 3, 0, 0, 0, argB, 0);
    gAimPass++;
}

/*
 * Per-type aim update for the current object: picks the bones, limits, rates and search options for its id,
 * searches for a target on the first pass (or when forced), and turns the bone with aim_turnBone.
 */
void aim_updateByType(void) {
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
    switch (movement_obj[dc_glbl_CharNum].character_type) {
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
        bone = 33;
        refBone = 32;
        break;
    case 135:
    case 181:
        bone = 4;
        refBone = 3;
        break;
    case 134:
        refBone = 3;
        bone = 5;
        if (gAimPass == 0) {
            bone = 4;
            rateIdx = 10;
        } else {
            refBone = 4;
        }
        break;
    case 153:
        refBone = 3;
        bone = 5;
        if (gAimPass == 0) {
            bone = 4;
            rateIdx = 10;
        } else {
            refBone = 4;
        }
        break;
    case 17:
        refBone = 3;
        bone = 5;
        if (gAimPass == 0) {
            bone = 4;
            rateIdx = 10;
        } else {
            refBone = 4;
        }
        break;
    case 20:
        refBone = 3;
        bone = 5;
        if (gAimPass == 0) {
            bone = 4;
            rateIdx = 10;
        } else {
            refBone = 4;
        }
        break;
    case 121:
        bone = 27;
        refBone = 26;
        break;
    case 33:
        bone = 25;
        refBone = 24;
        break;
    case 123:
        bone = 3;
        refBone = 2;
        if (gAimPass == 0) {
            anyRange = 1;
            break;
        }
        yOffset = -165.0f;
        limitIdx = 10;
        rateIdx = 7;
        switch (movement_obj[dc_glbl_CharNum].animseq.uh) {
        case 7:
        case 9:
            skipUnk127 = 1;
            break;
        case 20:
            preferId = 126;
            yOffset = 0.0f;
            rateIdx = 9;
            limitIdx = 11;
            break;
        default:
            clearTarget = 1;
        }
        if (gAimPass == 1) {
            bone = 22;
            refBone = 22;
            axisMode = 2;
            forceSearch = 1;
        } else {
            bone = 26;
            refBone = 26;
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
        bone = 17;
        refBone = 16;
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
        if (gIsMultiplayer != 0) {
            return;
        }
        bone = 4;
        refBone = 0;
        if (movement_obj[dc_glbl_CharNum].taskint[1] == 1) {
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
        bone = 29;
        refBone = 28;
        skipUnk127 = 1;
        break;
    case 41:
        bone = 2;
        refBone = 1;
        break;
    case 75:
        if (gAimPass == 0) {
            bone = 43;
            refBone = 4;
            rateIdx = 6;
            argB = 0;
            skipUnk127 = 1;
        } else {
            bone = 44;
            refBone = 4;
            skipUnk127 = 1;
        }
    setAnyRange:
        anyRange = 1;
        break;
    case 80:
    case 154:
    case 155:
        bone = 13;
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
        if (gAimPass == 0) {
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
        bone = 16;
        if (gAimPass == 0) {
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
        if (gAimPass == 0) {
            bone = 1;
            refBone = 0;
        } else if (gAimPass == 1) {
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
        gAimTarget[0] = -99999.0f;
        gAimTarget[1] = -99999.0f;
        gAimTarget[2] = -99999.0f;
    } else if (gAimPass == 0 || forceSearch) {
        aim_findTargetObject(gAimTarget, bone, refBone, dc_glbl_CharNum,
                      gAimLimits[limitIdx - 1].yawMax + gAimLimits[limitIdx - 1].yawMargin, preferId, &bestDist, noCone, skipUnk127, anyRange, axisMode);
        gAimTarget[1] += yOffset;
    }
    aim_turnBone(bone, refBone, gAimTarget, dc_glbl_CharNum, limitIdx, rateIdx, gAimPass, axes, noCone, relative, 0,
                  argB, axisMode);
    gAimPass++;
}


/*
 * Tests whether world point (x, y, z) lies within +-range degrees of yaw as seen from object
 * objIndex's bone matrix `bone` (relative to `refBone`). On success copies the point to out.
 */
s32 aim_isPointInCone(s32 objIndex, s32 refBone, s32 bone, f32 x, f32 y, f32 z, f32 *out, f32 range, s32 noCheck,
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
    refMtx = OBJ_BONES(&movement_obj[objIndex])[refBone];
    if (movement_obj[objIndex].ob_matrix_ptr == NULL) {
        return 0;
    }
    mtx = OBJ_BONES(&movement_obj[objIndex])[bone];
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
    aim_directionToAngles(&yaw, &pitch, localX, localY, localZ, axisMode);
    if (yaw < range && -range < yaw) {
        out[0] = x;
        out[1] = y;
        out[2] = z;
        return 1;
    }
    return 0;
}

/*
 * Handles the fixed target modes in an object's unk2F8 low bits: 1 aims at camera_global_Cameras's point,
 * 2 at the point in gAimFixedPoint (unless a type-4 object is more than 250 units away), 3 holds.
 * Returns 1 when one of these applied.
 */
s32 aim_getFixedTarget(s32 objIndex, f32 *out) {
    Actor *obj;
    s32 mode;
    f32 dx;
    f32 dz;

    obj = &movement_obj[objIndex];
    mode = obj->unk2F8 & 7;
    if (mode == 1) {
        out[0] = camera_global_Cameras->eye.x;
        out[1] = camera_global_Cameras->eye.y;
        out[2] = camera_global_Cameras->eye.z;
        return 1;
    }
    if (mode == 2) {
        if (obj->drawtype == 4) {
            dx = obj->xpos - gAimFixedPoint[0];
            dz = obj->zpos - gAimFixedPoint[2];
            if (62500.0f < dx * dx + dz * dz) {
                return 1;
            }
        }
        out[0] = gAimFixedPoint[0];
        out[1] = gAimFixedPoint[1];
        out[2] = gAimFixedPoint[2];
        return 1;
    }
    if (mode == 3) {
        return 1;
    }
    return 0;
}

/* The eight node types aim_findTarget scans for aim targets. */
typedef struct {
    s32 types[8];
} AimNodeTypes;

/* Entries of the gObjectLists per-type lists. */
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

static AimNodeTypes gAimNodeTypes = { { 0x0A, 0x15, 0x16, 0x17, 0x13, 0x14, 0x19, 0x48 } };

/*
 * Picks the point object objIndex should aim bone `bone` at, writing it to out (or -99999 when there is none):
 * its camera's target, a fixed target mode, the held object for anim 0x81, the nearest other object,
 * the nearest AimPoint, or the nearest node from the gObjectLists lists, each within +-range degrees.
 */
void aim_findTarget(f32 *out, s32 refBone, s32 bone, s32 objIndex, f32 range, s32 preferId, s32 axisMode) {
    Actor *obj;
    s32 hitObj;
    s32 foundKind;
    s16 objX;
    u32 idx;
    AimPoint *point;
    f32 radiusSq;
    f32 px;
    f32 py;
    s32 list;
    s32 i;
    u32 bestIdx;
    f32 bestDist;
    f32 pz;
    f32 distSq;
    static u32 D_80084400 = 0; /* aim point the next scan starts from */

    out[0] = -99999.0f;
    out[1] = -99999.0f;
    out[2] = -99999.0f;
    gAimLookPoint[0] = -99999.0f;
    gAimLookPoint[1] = -99999.0f;
    gAimLookPoint[2] = -99999.0f;
    if (gCutsceneStates[0] == 1 || gCutsceneStates[1] == 1) {
        return;
    }
    obj = &movement_obj[objIndex];
    if (obj->CameraPtr != NULL && obj->CameraPtr->Mode == 0x100 && obj->CameraPtr->DoingAim != 0) {
        Vec3f camTarget;

        camTarget = obj->CameraPtr->aim.orbitPos;
        out[0] = camTarget.x;
        out[1] = camTarget.y;
        out[2] = camTarget.z;
        return;
    }
    if (aim_getFixedTarget(objIndex, out)) {
        return;
    }
    if (objIndex == 0 && obj->animseq.uh == 0x81) {
        ModelAttachment *holder;
        AnimInstance *holderData;
        Mtx *model;
        Vec3f target;
        Actor *held;

        if ((holder = attach_findInSet(0x1F, obj)) != NULL && (holderData = holder->anim) != NULL &&
            holderData->unk3F6 != 0 && (model = holderData->partMtx[gDrawBufferIndex]) != NULL &&
            (held = collision_findObjectById(holder->ownerId)) != NULL) {
            if (held->drawtype == 5) {
                target.x = held->xpos;
                target.y = held->ypos;
                target.z = held->zpos;
            } else {
                math_mtxGetTranslation(&target, model);
            }
            if (aim_isPointInCone(objIndex, refBone, bone, target.x, target.y, target.z, out, range, 0, axisMode)) {
                return;
            }
        }
    }
    foundKind = 0;
    objX = movement_obj[objIndex].xpos;
    bestDist = 1600000000.0f;
    hitObj = aim_findNearestObject(out, refBone, bone, objIndex, range, preferId, &bestDist, 0, 0, 0, axisMode);
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

        if (D_80084400 >= gAimPointCount) {
            D_80084400 = 0;
        }
        if (gAimPointCount != 0) {
            idx = D_80084400;
            bestIdx = -1;
            do {
                point = &((AimPoint *)gAimPoints)[idx]; // matching: the no-op cast keeps the addu operand order
                if (objX < point->maxX && point->minX < objX && point->flags == 0) {
                    radiusSq = point->radiusSq;
                    px = point->x;
                    py = point->y;
                    pz = point->z;
                    dz = obj->zpos - pz;
                    dy = obj->ypos - py;
                    dz *= dz;
                    dx = objX - px;
                    dy *= dy;
                    distSq = dx * dx + dz + dy;
                    if (distSq < radiusSq && distSq < bestDist &&
                        aim_isPointInCone(objIndex, refBone, bone, px, py + 50.0f, pz, out, range, 0, axisMode)) {
                        bestIdx = idx;
                        bestDist = distSq;
                    }
                }
                idx++;
                if (idx == gAimPointCount) {
                    idx = 0;
                }
            } while (idx != D_80084400);
            if (bestIdx != -1) {
                D_80084400 = bestIdx;
                foundKind = 2;
            }
        }

        types = gAimNodeTypes;
        for (list = 0; list < 2; list++) {
            for (i = 0; i < 8; i++) {
                type = types.types[i];
                node = (AimNode *)gObjectLists[list][type];
                while (node != NULL) {
                    if (type == 25 || type == 72) {
                        /* matching: this redeclaration of gAimPointCount (conker/aim.h) opens a -g3 scope block, which shifts the scheduling of the loop */
                        extern s32 gAimPointCount;
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
                        dz = obj->zpos - pz;
                        dy = obj->ypos - py;
                        dz *= dz;
                        dx = objX - px;
                        dy *= dy;
                        distSq = dx * dx + dz + dy;
                        if (distSq < maxDist) {
                            dx = objX - pos[0];
                            if (dx * dx + dz + dy < bestDist &&
                                aim_isPointInCone(objIndex, refBone, bone, px, py, pz, out, range, 0, axisMode)) {
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
        if (gAimPoints[bestIdx].unk12 != 0) {
            func_1507E9E8(obj, gAimPoints[bestIdx].unk12 - 1);
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
s32 aim_findNearestObject(f32 *out, s32 refBone, s32 bone, s32 objIndex, f32 range, s32 preferId, f32 *bestDist,
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
    if (gCutsceneStates[0] == 1 || gCutsceneStates[1] == 1) {
        out[0] = -99999.0f;
        out[1] = -99999.0f;
        out[2] = -99999.0f;
        return 0;
    }
    selfLink = objIndex + 1;
    selfId = movement_obj[objIndex].character_type;
    if (selfId == 5 || selfId == 173 || selfId == 174 || selfId == 175) {
        allowSameId = 0;
    }
    if (selfId == 45) {
        excludedId = 44;
    }
    if (!anyRange) {
        maxDist = movement_obj[objIndex].unk23D * 8;
        maxDist = maxDist * maxDist;
    } else {
        maxDist = 1600000000.0f;
    }
    for (i = 0; i < 25; i++) {
        if (movement_obj[i].type == 0) {
            continue;
        }
        if (movement_obj[i].character_type == 0xFF) {
            continue;
        }
        if (i == objIndex) {
            continue;
        }
        if (selfLink == movement_obj[i].attach_char) {
            continue;
        }
        if (movement_obj[objIndex].attach_char == i + 1) {
            continue;
        }
        if (!allowSameId && selfId == movement_obj[i].character_type) {
            continue;
        }
        if (excludedId == movement_obj[i].character_type) {
            continue;
        }
        if ((skipUnk127 || (movement_obj[objIndex].unk2F8 & 7) == 4) && movement_obj[i].playernumber == 0xFF) {
            continue;
        }
        if ((movement_obj[objIndex].unk2F8 & 7) == 5 && movement_obj[i].playernumber != 0xFF) {
            continue;
        }
        dx = movement_obj[objIndex].xpos - movement_obj[i].xpos;
        dz = movement_obj[objIndex].zpos - movement_obj[i].zpos;
        dy = movement_obj[objIndex].ypos - movement_obj[i].ypos;
        dx = dx * dx;
        dz = dz * dz;
        dy = dy * dy;
        dist = dx + dz + dy;
        if (i == movement_obj[objIndex].unk222 &&
            (movement_obj[objIndex].unk223 == 1 || movement_obj[objIndex].unk223 == 12)) {
            dist = 0.0f;
        }
        if ((dist < maxDist && dist < *bestDist) || preferId == movement_obj[i].character_type) {
            if (aim_isPointInCone(objIndex, refBone, bone, movement_obj[i].aim.x, movement_obj[i].aim.topY,
                              movement_obj[i].aim.z, out, range, noCheck, axisMode)) {
                *bestDist = dist;
                found = i;
                if (preferId == movement_obj[i].character_type) {
                    break;
                }
            }
        }
    }
    if (found != -1) {
        return movement_obj[found].unk13B + 1;
    }
    out[0] = -99999.0f;
    out[1] = -99999.0f;
    out[2] = -99999.0f;
    return 0;
}


/*
 * Converts a bone-local direction (x, y, z) to yaw/pitch in degrees, (-180, 180]. axisMode picks which
 * local axis is "forward": 0 +z, 1 +x, 2 -x, 3 -y.
 */
void aim_directionToAngles(f32 *yaw, f32 *pitch, f32 x, f32 y, f32 z, s32 axisMode) {
    switch (axisMode) {
    case 0:
        *yaw = math_atan2(x, z);
        *pitch = math_atan2(y, sqrtf(x * x + z * z));
        break;
    case 1:
        *yaw = math_atan2(-z, x);
        *pitch = math_atan2(y, sqrtf(x * x + z * z));
        break;
    case 2:
        *yaw = math_atan2(z, -x);
        *pitch = math_atan2(y, sqrtf(x * x + z * z));
        break;
    case 3:
        *yaw = math_atan2(z, -y);
        *pitch = math_atan2(-x, sqrtf(y * y + z * z));
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

/* Per rate index: yaw rate, pitch rate, yaw max step, pitch max step. */
static const f32 gAimRates[][4] = {
    { 0.1f, 0.1f, 0.6f, 0.6f },
    { 0.4f, 0.4f, 1.5f, 1.5f },
    { 0.35f, 0.35f, 0.5f, 0.5f },
    { 0.35f, 0.35f, 0.5f, 0.5f },
    { 0.8f, 0.8f, 0.3f, 0.3f },
    { 0.7f, 0.7f, 2.0f, 2.0f },
    { 0.008f, 0.008f, 0.6f, 0.6f },
    { 0.08f, 0.08f, 0.6f, 0.6f },
    { 1.0f, 1.0f, 360.0f, 360.0f },
    { 0.08f, 0.08f, 0.6f, 0.6f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
};

/*
 * Turns bone matrix `bone` of object objIndex (measured against refBone) toward target (-99999 for none),
 * easing per aim slot through the object's CamData, with turn rates from gAimRates[rateIdx] and the
 * window from aim limits slot limitIdx. Bit 0/1 of axes enable yaw/pitch; argB is unused.
 */
void aim_turnBone(s32 bone, s32 refBone, f32 *target, s32 objIndex, s32 limitIdx, s32 rateIdx, s32 slot,
                   s32 axes, s32 noLimit, s32 relative, s32 yawDeadzone, s32 argB, s32 axisMode) {
    Actor *obj;
    f32 (*refMtx)[4];
    MtxF result;
    f32 scale[3];
    f32 dir[3];
    f32 localX;
    f32 localY;
    f32 localZ;
    f32 yaw;
    f32 pitch;
    f32 roll;
    MtxF mtxC;
    MtxF mtxA;
    MtxF mtxB;
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

    obj = &movement_obj[objIndex];
    data = obj->camData;
    if (data == NULL) {
        return;
    }
    yaw = 0.0f;
    pitch = 0.0f;
    roll = 0.0f;
    if (gAimPass == 0) {
        obj->unk2CA = 0;
    }
    hasOverride = cutscene_getLookTarget(&yaw, &pitch, &roll, target, objIndex, 0);
    if (hasOverride) {
        overrideAngles[0] = yaw;
        overrideAngles[1] = pitch;
    }
    blendMode = 0;
    gAimHasOverride = 0;
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
            if (gPaused == 0 && gFrameRateDivider == 0) {
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
        aim_directionToAngles(&yaw, &pitch, localX, localY, localZ, axisMode);
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
    if (gAimPass == 0) {
        obj->unk2CA = yaw * 0.7111111283f;
    }
    origYaw = yaw;
    origPitch = pitch;
    if ((timerDone && !hasOverride) || gLevelLoadState) {
        aim_clampToLimits(limitIdx, &yaw, &pitch, &prevYaw, &prevPitch, origYaw, origPitch, objIndex, slot);
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
            aim_directionToAngles(&yaw, &pitch, localX, localY, localZ, axisMode);
            if (yaw > 0.0f) {
                yaw -= yawDeadzone;
            } else if (yaw < 0.0f) {
                yaw += yawDeadzone;
            }
        } else {
            gAimHasOverride = 1;
            gAimOverrideYaw = yaw;
            gAimOverridePitch = pitch;
        }
        if (movement_obj[dc_glbl_CharNum].CameraPtr != NULL && movement_obj[dc_glbl_CharNum].CameraPtr->Mode == 0x100 &&
            (yaw > 100.0f || yaw < -100.0f)) {
            dir[0] = -dir[0];
            dir[1] = 20.0f - dir[1];
            dir[2] = -dir[2];
            localX = dir[0] * refMtx[0][0] + dir[1] * refMtx[0][1] + dir[2] * refMtx[0][2];
            localY = dir[0] * refMtx[1][0] + dir[1] * refMtx[1][1] + dir[2] * refMtx[1][2];
            localZ = dir[0] * refMtx[2][0] + dir[1] * refMtx[2][1] + dir[2] * refMtx[2][2];
            aim_directionToAngles(&yaw, &pitch, localX, localY, localZ, axisMode);
        }
        gAimLookPoint[0] = mtx[3][0] + dir[0] * 100.0f;
        gAimLookPoint[1] = mtx[3][1] + dir[1] * 100.0f;
        gAimLookPoint[2] = mtx[3][2] + dir[2] * 100.0f;
        if (noLimit == 0) {
            aim_clampToLimits(limitIdx, &yaw, &pitch, &prevYaw, &prevPitch, origYaw, origPitch, objIndex, slot);
        }
    } else {
        yaw = origYaw;
        pitch = origPitch;
    }
    if (gPaused == 0 && gFrameRateDivider == 0) {
        rateYaw = gAimRates[rateIdx][0];
        ratePitch = gAimRates[rateIdx][1];
        maxYaw = gAimRates[rateIdx][2];
        maxPitch = gAimRates[rateIdx][3];
        if (gCutsceneStates[0] == 1) {
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
            aim_clampToLimits(limitIdx, &yaw, &pitch, NULL, NULL, origYaw, origPitch, objIndex, slot);
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
    if (gAimPass == 0) {
        obj->unk2CA = yaw * 0.7111111283f;
    }
    finalYaw = data->offYaw[slot] + yaw;
    finalPitch = data->offPitch[slot] + pitch;
    switch (axisMode) {
    case 0:
        math_mtxRotateRPYF(mtxA, 0.0f, 0.0f, roll);
        math_mtxRotateRPYF(mtxB, -finalPitch, finalYaw, 0.0f);
        break;
    case 2:
        math_mtxRotateRPYF(mtxA, 0.0f, 0.0f, -finalPitch);
        math_mtxRotateRPYF(mtxB, roll, finalYaw, 0.0f);
        break;
    case 1:
        math_mtxRotateRPYF(mtxA, 0.0f, 0.0f, finalPitch);
        math_mtxRotateRPYF(mtxB, roll, finalYaw, 0.0f);
        break;
    case 3:
        math_mtxRotateRPYF(mtxA, 0.0f, 0.0f, -finalPitch);
        math_mtxRotateRPYF(mtxB, -finalYaw, roll, 0.0f);
        break;
    }
    math_mtxMulF(mtxA, mtxB, mtxC);
    math_mtxMulF(mtxC, refMtx, result);
    result[3][0] = mtx[3][0];
    result[3][1] = mtx[3][1];
    result[3][2] = mtx[3][2];
    if (blendMode != 0) {
        f32 qB[4];
        f32 qA[4];
        f32 qOut[4];
        s32 i;
        s32 j;
        bcopy(mtx, mtxA, sizeof(mtxA));
        bcopy(result, mtxB, sizeof(mtxB));
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
        math_mtxToQuat(mtxB, qB);
        math_mtxToQuat(mtxA, qA);
        math_quatAlign(qB, qA);
        math_quatSlerp(qB, qA, blendT, qOut);
        math_quatToMtx(qOut, result);
    } else {
        scale[0] = 1.0f;
        scale[1] = 1.0f;
        scale[2] = 1.0f;
    }
    if (relative != 0) {
        math_mtxMulF(mtxC, mtx, mtxA);
        bcopy(mtxA, mtx, sizeof(mtxA));
    } else {
        bcopy(result, mtx, sizeof(result));
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
    if (gPaused == 0 && gFrameRateDivider == 0) {
        if (data->blend[slot] > 0) {
            data->blend[slot]--;
        }
    }
    if (!noTarget) {
        data->timer[slot] = 40;
    }

    step = gFrameTicks * 10;
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
const AimLimits *aim_getLimits(s32 objIndex, s32 limitIdx) {
    return &gAimLimits[limitIdx];
}

/*
 * Clamps a yaw/pitch pair to the default limits (slot 0). When the object's camera is not in
 * mode 0x100 and the yaw is past its margin, falls back to the given yaw/pitch instead.
 * The unclamped angles are written to prevYaw/prevPitch when given.
 */
void aim_clampAngles(f32 *yaw, f32 *pitch, f32 *prevYaw, f32 *prevPitch, f32 fallbackYaw, f32 fallbackPitch,
                   s32 objIndex) {
    const AimLimits *lim;
    Camera *camera;

    camera = movement_obj[objIndex].CameraPtr;
    lim = aim_getLimits(objIndex, 0);
    if (camera != NULL && camera->Mode != 0x100) {
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
 * Per-frame aim bookkeeping: object 0x53 in anim 0x15 (or any object while gLevelLoadState is set) resets all aim
 * slots, and an object whose type has an "aim-locked" anim list gets unk2FB bit 2 set while it plays one of
 * them, or (for interaction state 1) while dead, slow in water, flagged in mv, or moving fast.
 */
void aim_updateLock(Actor *obj) {
    s32 id;
    s32 i;
    s32 group;
    s32 count;
    const u16 *anims;
    CamData *data;

    data = obj->camData;
    if (data != NULL) {
        id = obj->character_type;
        if ((id == 0x53 && obj->animseq.uh == 0x15) || gLevelLoadState != 0) {
            for (i = 0; i < 3; i++) {
                data->timer[i] = 40;
                data->blend[i] = 0;
                data->flag[i] = 4;
            }
        }

        group = gNoAimAnimGroups[id];
        if (group != 0) {
            group--;
            anims = gNoAimAnimLists[group];
            count = gNoAimAnimCounts[group];
            for (i = 0; i < count; i++) {
                if (obj->animseq.uh == anims[i]) {
                    obj->unk2FB |= 4;
                    return;
                }
            }

            if (obj->type == 1) {
                if (obj->energy == 0 || (obj->inwater != 0 && obj->speed < 5.0f) ||
                    obj->mv->SniperState != 0 || obj->speed > 20.0f) {
                    obj->unk2FB |= 4;
                }
            } else if (obj->energy == 0) {
                obj->unk2FB |= 4;
            }
        }
    }
}

/* aim_clampAngles for limits slot limitIdx (0 uses aim_clampAngles's camera-aware rules). */
void aim_clampToLimits(s32 limitIdx, f32 *yaw, f32 *pitch, f32 *prevYaw, f32 *prevPitch, f32 fallbackYaw,
                   f32 fallbackPitch, s32 objIndex, s32 slot) {
    const AimLimits *lim;

    if (limitIdx == 0) {
        aim_clampAngles(yaw, pitch, prevYaw, prevPitch, fallbackYaw, fallbackPitch, objIndex);
        return;
    }
    lim = aim_getLimits(objIndex, limitIdx);
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

/*
 * Recomputes an object's aim points (OBJ_AIM) from its type's aim bone: the bone origin pushed out by
 * unk30 along z, a top point unk0 up along y, two side points unk3A out along +-x, and a z direction in
 * dir. Objects without that bone data get points from their position and actor_computeSize's height.
 */
void aim_updatePoints(Actor *obj) {
    ModelDef *def;
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

    if (obj->character_type != 0xFF) {
        if ((obj->ob_matrix_ptr != NULL) && (gModelDefs[obj->character_type] != NULL)) {
            if (model_loadDef(obj->character_type, 0) == 0) {
                def = gModelDefs[obj->character_type];
                bone = obj->ob_matrix_ptr[0];
                bone = ((MtxF *)bone)[def->aimBone]; // matching: two steps; one OBJ_BONES() index mismatches
                scale = obj->scale_y;
                width = def->aimHalfWidth * scale;
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
                len = def->aimHeight * scale;
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
                len = def->aimReach * scale;
                if (len != 0) {
                    mag = sqrtf(vx * vx + vy * vy + vz * vz);
                    if (mag != 0) {
                        mag = len / mag;
                    }
                    vx *= mag;
                    vy *= mag;
                    vz *= mag;
                    mag = 127.0f / len;
                    obj->aim.dir[0] = vx * mag;
                    obj->aim.dir[1] = vy * mag;
                    obj->aim.dir[2] = vz * mag;
                } else {
                    mag = sqrtf(vx * vx + vy * vy + vz * vz);
                    if (mag != 0) {
                        mag = 127.0f / mag;
                    }
                    obj->aim.dir[0] = vx * mag;
                    obj->aim.dir[1] = vy * mag;
                    obj->aim.dir[2] = vz * mag;
                    vx = vy = vz = 0.0f;
                }
                obj->aim.x = bone[3][0] + vx;
                obj->aim.y = bone[3][1] + vy;
                obj->aim.z = bone[3][2] + vz;
                obj->aim.topY = bone[3][1] + upY;
                mag = obj->aim.x;
                obj->aim.side[0][0] = mag + sideX + upX;
                obj->aim.side[0][1] = obj->aim.y + sideY + upY;
                obj->aim.side[0][2] = obj->aim.z + sideZ + upZ;
                obj->aim.side[1][0] = mag - sideX + upX;
                obj->aim.side[1][1] = obj->aim.y - sideY + upY;
                obj->aim.side[1][2] = obj->aim.z - sideZ + upZ;
            }
        } else {
            def = gModelDefs[obj->character_type];
            if (def != NULL) {
                topHeight = def->aimHeight;
            } else {
                topHeight = 0;
            }
            actor_computeSize(obj, &height, &size2, &size3);
            obj->aim.x = obj->xpos;
            obj->aim.y = obj->ypos + (height * 7) / 8;
            obj->aim.z = obj->zpos;
            obj->aim.topY = obj->aim.y + topHeight * obj->scale_y;
            obj->aim.side[0][0] = obj->aim.x;
            obj->aim.side[1][0] = obj->aim.x;
            obj->aim.side[0][1] = obj->aim.topY;
            obj->aim.side[1][1] = obj->aim.topY;
            obj->aim.side[0][2] = obj->aim.z;
            obj->aim.side[1][2] = obj->aim.z;
            obj->unk2CA = 0;
        }
    }
}

/* Moves bone matrix 1's translation of the current object to (unk174, ypos, unk178). */
void func_1503A60C(void) {
    f32 (*mtx)[4] = OBJ_BONES(&movement_obj[dc_glbl_CharNum])[1];

    mtx[3][0] = movement_obj[dc_glbl_CharNum].unk174;
    mtx[3][1] = movement_obj[dc_glbl_CharNum].ypos;
    mtx[3][2] = movement_obj[dc_glbl_CharNum].unk178;
}

static u8 D_80084404[] = { 1, 4, 14, 18, 22 }; /* per aim pass: the bone func_1503A678 scales */

/*
 * For object type 0x22: scales bone D_80084404[gAimPass] by 1 + unk20A / 64 in x and z (the inverse for
 * every pass after the first), keeping its translation.
 */
void func_1503A678(void) {
    Actor *obj;
    f32 (*bone)[4];
    f32 scale[3];
    f32 factor;
    f32 unused; // matching: -g3 stack slot
    MtxF scaleMtx;
    MtxF tmp;
    f32 pos[3];

    obj = &movement_obj[dc_glbl_CharNum];
    if (obj->character_type == 0x22) {
        factor = movement_obj[dc_glbl_CharNum].unk20A;
        factor /= 64.0f;
        factor += 1.0f;
        if (gAimPass == 0) {
            scale[0] = factor;
            scale[1] = 1.0f;
            scale[2] = factor;
        } else {
            scale[0] = 1.0f / factor;
            scale[1] = 1.0f;
            scale[2] = 1.0f / factor;
        }
        // matching: byte-offset form; indexing OBJ_BONES() here changes the code
        bone = (f32 (*)[4])((u8 *)movement_obj[dc_glbl_CharNum].ob_matrix_ptr + (D_80084404[gAimPass] << 6));
        bone[3][3] = 1.0f;
        bone[0][3] = 0.0f;
        bone[1][3] = 0.0f;
        bone[2][3] = 0.0f;
        pos[0] = bone[3][0];
        pos[1] = bone[3][1];
        pos[2] = bone[3][2];
        guMtxIdentF(scaleMtx);
        math_mtxScaleF(scaleMtx, scale[0], scale[1], scale[2]);
        math_mtxMulF(bone, scaleMtx, tmp);
        bcopy(tmp, bone, sizeof(tmp));
        bone[3][0] = pos[0];
        bone[3][1] = pos[1];
        bone[3][2] = pos[2];
        gAimPass++;
    }
}

/* Runs aim_updateByType as the first pass (gAimPass = 0), then func_1503A678 with the real pass number. */
void func_1503A7F0(void) {
    s32 pass = gAimPass;

    gAimPass = 0;
    aim_updateByType();
    gAimPass = pass;
    func_1503A678();
}


#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define APPROACH(fld, tgt)                                       \
    if (ABS((tgt) - (fld)) < step) {                             \
        (fld) = (tgt);                                           \
    } else {                                                     \
        (fld) += step * (((tgt) - (fld)) < 0 ? -1 : 1);         \
    }

/*
 * Turns an object's head (two yaw/pitch pairs, Actor.unk27A/unk27E) toward its aim target, gAimTarget, found with
 * aim_findTarget for the object types that search, and runs the head effect selected by unk276 while unk282
 * counts down. Angles are clamped to +-45 yaw and -15..30 pitch, scaled by the type's turn ranges and eased
 * by up to `step` per frame.
 */
void aim_turnHead(Actor *obj) {
    s32 id;
    s32 bone;
    s32 preferId;
    s32 search;
    f32 (*mtx)[4];
    f32 delta[3];
    f32 yaw[2];
    f32 pitch[2];
    f32 yaw0;
    f32 yaw1;
    f32 pitch0;
    f32 pitch1;
    f32 padB4; // matching: -g3 stack slots
    f32 padB0;
    f32 localX;
    f32 localY;
    f32 localZ;
    s32 step;
    ModelAttachment *holder;
    AnimInstance *holderData;
    s32 mirror;
    s32 clamp;
    s32 snap;
    s32 speed;
    s32 aim;
    s32 noHolderTarget;
    ModelDef *unusedDef; // matching: -g3 stack slot

    if (obj->ob_matrix_ptr == NULL) {
        return;
    }
    gAimTarget[0] = gAimLookPoint[0];
    gAimTarget[1] = gAimLookPoint[1];
    gAimTarget[2] = gAimLookPoint[2];
    id = obj->character_type;
    step = 8;
    preferId = 0x100;
    search = 1;
    mirror = 1;
    clamp = 1;
    snap = 0;
    aim = 1;
    noHolderTarget = 1;
    switch (id) {
    case 154:
        bone = 13;
        break;
    case 146:
        bone = 3;
        break;
    case 82:
        bone = 0;
        break;
    case 21:
        bone = 3;
        break;
    case 84:
        bone = 29;
        break;
    case 69:
        bone = 3;
        break;
    case 24:
    case 30:
        bone = 2;
        if (id == 30) {
            preferId = 34;
        }
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 130:
    case 150:
        bone = 12;
        search = 0;
        break;
    default:
        return;
    }
    if (search) {
        aim_findTarget(gAimTarget, bone, bone, obj - movement_obj, 90.0f, preferId, 0);
    }
    // matching: byte-offset form; indexing OBJ_BONES() here changes the code
    mtx = (f32 (*)[4])((u8 *)movement_obj[obj - movement_obj].ob_matrix_ptr + (bone << 6));
    mtx[3][3] = 1.0f;
    mtx[2][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[0][3] = 0.0f;
    yaw[0] = pitch[0] = 0.0f;
    if (obj->headEffectTimer == 0) {
        obj->headEffectState = 0;
        obj->unk284 = 0;
        obj->unk285 = 0;
        obj->unk286 = 0;
        obj->unk287 = 0;
    }
    if (obj->headEffectTimer != 0) {
        aim = 0;
        if (obj->headEffect == 0) {
            s32 phase0;
            s32 phase1;
            f32 sinV;
            f32 cosV;
            s32 pad68; // matching: -g3 stack slot
            f32 scale;
            f32 amp;
            obj->headYaw[0] -= obj->unk284;
            obj->headPitch[0] -= obj->unk285;
            obj->headYaw[1] -= obj->unk286;
            obj->headPitch[1] -= obj->unk287;
            phase0 = obj->headEffectState & 0xFF;
            phase1 = obj->headEffectState >> 8;
            phase0 += gFrameTicks * 3;
            phase1 -= gFrameTicks * 4;
            obj->headEffectState = (phase1 << 8) | phase0;
            if (obj->headEffectTimer < 30) {
                scale = (f32)(obj->headEffectTimer + 0U) / 30.0f; // matching: unsigned int to float conversion
            } else {
                scale = 1.0f;
            }
            sinV = math_sinBinAngle(phase0);
            cosV = math_cosBinAngle(phase0);
            amp = 11.0f * scale;
            obj->unk284 = sinV * amp;
            obj->unk285 = cosV * amp;
            sinV = math_sinBinAngle(phase1);
            cosV = math_cosBinAngle(phase1);
            obj->unk286 = sinV * amp;
            obj->unk287 = cosV * amp;
        } else if (obj->headEffect == 1) {
            if (obj->headEffectState == 0) {
                obj->headEffectState = 1;
                obj->unk287 += math_randomU8() % 20U;
            }
            if (obj->headEffectTimer < 0xAE) {
                speed = obj->headEffectTimer / 6 + 1;
            } else {
                speed = 30;
            }
            obj->unk284 = 0;
            obj->unk285 += speed;
            obj->unk287 += speed;
            obj->unk286 = obj->unk284;
            snap = 1;
            yaw[0] = obj->unk284;
            pitch[0] = obj->unk285;
            yaw[1] = obj->unk286;
            pitch[1] = obj->unk287;
        } else if (obj->headEffect == 2) {
            Mtx *model;

            aim = 1;
            holder = attach_find(obj, 0x2B, 0);
            if (holder != NULL) {
                holderData = holder->anim;
                if (holderData != NULL) {
                    model = holderData->partMtx[gDrawBufferIndex];
                    if (model != NULL) {
                        model += 2; // the third part matrix
                        math_mtxGetTranslation((Vec3f *)gAimTarget, model);
                        noHolderTarget = 0;
                    }
                }
            }
        } else if (obj->headEffect == 3) {
            f32 (*targetBone)[4];

            if (obj->headEffectState != -1) { // always true for the u16; matching: removing it changes the code
                targetBone = obj->ob_matrix_ptr[0];
                if (targetBone != NULL) {
                    targetBone = ((MtxF *)targetBone)[obj->headEffectState];
                    gAimTarget[0] = targetBone[3][0];
                    gAimTarget[1] = targetBone[3][1];
                    gAimTarget[2] = targetBone[3][2];
                }
            }
        } else if (obj->headEffect == 4) {
            if (obj->animseq.uh == 0x26) {
                if (obj->headEffectState == 0) {
                    obj->unk284 -= gFrameTicks;
                    if (obj->unk284 < -20) {
                        obj->unk284 = -20;
                        obj->headEffectState = 1;
                    }
                } else {
                    obj->unk284 += gFrameTicks * 3;
                    if (obj->unk284 > 20) {
                        obj->unk284 = 20;
                        obj->headEffectState = 0;
                    }
                }
                obj->unk287 = 0;
                obj->unk286 = obj->unk284;
                obj->unk285 = obj->unk287;
                snap = 1;
            } else {
                obj->headEffectTimer = 0;
                aim = 1;
            }
        } else if (obj->headEffect == 5) {
            f32 kx = 0.3543307185f;
            f32 ky = 0.236220479f;

            yaw[0] = obj->unk284 * kx;
            yaw[1] = obj->unk286 * kx;
            pitch[0] = obj->unk285 * ky;
            pitch[1] = obj->unk287 * ky;
            clamp = 0;
            mirror = 0;
        } else if (obj->headEffect == 6) {
            if (obj->mv->rt_context_b == 0x2A) {
                pitch[0] = 30.0f;
                yaw[0] = -6.0f;
                yaw[1] = 6.0f;
            } else {
                yaw[0] = 0; // matching: integer zeros; 0.0f changes the code
                yaw[1] = 0;
                pitch[0] = 0;
                if (obj->headPitch[0] == 0) {
                    obj->headEffect = 0;
                }
            }
            mirror = 0;
            pitch[1] = pitch[0];
        }
        if (obj->headEffectTimer != 0xFFFF) {
            if (obj->headEffectTimer >= gFrameTicks) {
                obj->headEffectTimer -= gFrameTicks;
            } else {
                obj->headEffectTimer = 0;
            }
        }
    }
    if (aim) {
        if (!(obj->unk2FB & 4) || !noHolderTarget) {
            if (gAimTarget[0] != -99999.0f || gAimTarget[1] != -99999.0f || gAimTarget[2] != -99999.0f) {
                if (gAimHasOverride != 0) {
                    gAimHasOverride = 0;
                    yaw[0] = gAimOverrideYaw;
                    pitch[0] = gAimOverridePitch;
                } else {
                    delta[0] = gAimTarget[0] - mtx[3][0];
                    delta[1] = gAimTarget[1] - mtx[3][1];
                    delta[2] = gAimTarget[2] - mtx[3][2];
                    localX = delta[0] * mtx[0][0] + delta[1] * mtx[0][1] + delta[2] * mtx[0][2];
                    localY = delta[0] * mtx[1][0] + delta[1] * mtx[1][1] + delta[2] * mtx[1][2];
                    localZ = delta[0] * mtx[2][0] + delta[1] * mtx[2][1] + delta[2] * mtx[2][2];
                    yaw[0] = math_atan2(localX, localZ);
                    pitch[0] = math_atan2(localY, sqrtf(localX * localX + localZ * localZ));
                    yaw[0] *= 57.29577637f;
                    pitch[0] *= 57.29577637f;
                    if (yaw[0] > 180.0f) {
                        yaw[0] -= 360.0f;
                    }
                    if (pitch[0] > 180.0f) {
                        pitch[0] -= 360.0f;
                    }
                }
            } else {
                step = 4;
            }
        } else {
            yaw[0] = pitch[0] = 0.0f;
        }
    }
    if (obj->headEffectTimer == 0 && cutscene_getHeadAngles(yaw, pitch, obj - movement_obj, 0)) {
        clamp = 1;
        mirror = 0;
        snap = 0;
        step = 8;
    }
    if (clamp) {
        yaw[0] = CLAMP(yaw[0], -45.0f, 45.0f);
        pitch[0] = CLAMP(pitch[0], -15.0f, 30.0f);
        yaw[1] = CLAMP(yaw[1], -45.0f, 45.0f);
        pitch[1] = CLAMP(pitch[1], -15.0f, 30.0f);
    }
    if (mirror) {
        yaw[1] = yaw[0];
        pitch[1] = pitch[0];
    }
    if (gModelDefs[id] == NULL) {
        return;
    }
    yaw0 = (yaw[0] >= 0.0f) ? gModelDefs[id]->headYaw[1] * yaw[0] / 45.0f : gModelDefs[id]->headYaw[0] * yaw[0] / -45.0f;
    yaw1 = (yaw[1] >= 0.0f) ? gModelDefs[id]->headYaw[0] * yaw[1] / 45.0f : gModelDefs[id]->headYaw[1] * yaw[1] / -45.0f;
    yaw1 = -yaw1;
    pitch0 = (pitch[0] >= 0.0f) ? gModelDefs[id]->headPitch[0] * pitch[0] / 30.0f : gModelDefs[id]->headPitch[1] * pitch[0] / -15.0f;
    pitch1 = (pitch[1] >= 0.0f) ? gModelDefs[id]->headPitch[0] * pitch[1] / 30.0f : gModelDefs[id]->headPitch[1] * pitch[1] / -15.0f;
    APPROACH(obj->headYaw[0], (s32)yaw1);
    APPROACH(obj->headYaw[1], (s32)yaw0);
    APPROACH(obj->headPitch[0], (s32)pitch1);
    APPROACH(obj->headPitch[1], (s32)pitch0);
    if (snap) {
        obj->headYaw[0] = obj->unk284;
        obj->headPitch[0] = obj->unk285;
        obj->headYaw[1] = obj->unk286;
        obj->headPitch[1] = obj->unk287;
    }
    if (obj->headEffectTimer != 0 && obj->headEffect == 0) {
        obj->headYaw[0] += obj->unk284;
        obj->headPitch[0] += obj->unk285;
        obj->headYaw[1] += obj->unk286;
        obj->headPitch[1] += obj->unk287;
    }
}

/* Clears out and gAimLookPoint, then searches for a target object unless a fixed target mode applies. */
void aim_findTargetObject(f32 *out, s32 refBone, s32 bone, s32 objIndex, f32 range, s32 preferId, f32 *bestDist,
                   s32 noCheck, s32 skipUnk127, s32 anyRange, s32 axisMode) {
    f32 *lookAt;
    f32 none;

    lookAt = gAimLookPoint;
    none = -99999.0f;
    out[0] = none;
    out[1] = none;
    out[2] = none;
    lookAt[0] = none;
    lookAt[1] = none;
    lookAt[2] = none;
    if (!aim_getFixedTarget(objIndex, out)) {
        aim_findNearestObject(out, refBone, bone, objIndex, range, preferId, bestDist, noCheck, skipUnk127, anyRange,
                      axisMode);
    }
}
