#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Z-buffer scan parameters (gZScanParams[player][unk1B4], copied from gDefaultZScanParams by camera_resetZScans). */
typedef struct {
    s16 halfWidth; // pixels scanned on each side of the target
    s16 unk2;
    s16 threshold; // depth gap that counts as an occluder
    s16 altThreshold;
} ZScanParams;

/* One set of scans per player. */
typedef struct {
    /* 0x00 */ ZScan scan[4];
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 pad31[0x1B];
} ZScanSet;

/* Default z-buffer scan parameters, one per scan. */
static ZScanParams gDefaultZScanParams[4] = {
    { 35, 45, 400, 300 },
    { 35, 45, 300, 300 },
    { 35, 45, 250, 300 },
    { 35, 45, 530, 300 },
};
f32 gCameraAvoidTargetDist = 0.0f; // camera_keepOutsideCircle: target distance of the swung camera position (0 when not swinging)
static s32 D_800895B4[7] = { 0, 0, 0, 0, 1, 0, 0 }; /* not referenced */

static ZScanSet gZScanSets[4];
static f32 gCameraTargetScreenX; // the camera target projected to the screen: x, y and depth
static f32 gCameraTargetScreenY;
static f32 gCameraTargetScreenDepth;
static f32 D_800DC1FC;
static ZScanParams gZScanParams[4][4]; // per player

/* Updates the camera's floor heights (floorY, unk360; -10000 means none found) and the height it aims for
 * (unk354), with special cases for the camera modes and the tracked objects. */
void camera_updateFloorHeights(Camera *arg0) {
    f32 unused28;
    s32 flag4E;
    s16 height;
    s16 width;
    s16 depth;
    f32 targetHeight;
    f32 pad68; // matching: frame padding
    f32 pad64;

    unused28 = arg0->CharPtr->shad;
    if (arg0->mv != NULL) {
        flag4E = arg0->mv->transport;
        targetHeight = arg0->mv->height;
    } else {
        flag4E = 0;
        actor_computeSize(arg0->CharPtr, &height, &width, &depth);
        targetHeight = height;
    }
    if (!arg0->AvailableModes & 0x40) { // sic: (!unk84) & 0x40 is always 0, but the original computes it
        arg0->floorY = arg0->unk354 = arg0->CharPtr->floorY;
        return;
    }
    if ((arg0->Snap != 0) || (arg0->AvailableModes & 0x400000) || (arg0->SpecialFlags & 0x40) ||
        (collision_getTriHeightAt(arg0->eye.x, arg0->eye.z, arg0->TrianglePtr, arg0->floorTri.v, &arg0->floorY) == 0)) {
        collision_findFloorEx(&arg0->TrianglePtr, arg0->floorTri.v, &arg0->floorY, &arg0->waterY, &arg0->Attributes, 0,
                      arg0->eye.x, arg0->floorY, arg0->eye.z, arg0->prevEye.y, 0, 0, -10000.0f, arg0->eye.y, 1);
    }
    if (unused28) {} // matching: keeps the otherwise dead unk28 load
    if (level == 0x28) {
        arg0->zoneWaterY = -10000.0f;
        arg0->waterY = -10000.0f;
    }
    if (-10000.0f != arg0->zoneWaterY) {
        arg0->waterY = arg0->zoneWaterY;
    }
    camera_updateStatus(arg0);
    if (!(arg0->AvailableModes & 0x10) && (arg0->CharPtr->unk102 == 0) && (D_800C3671 == 0) && (arg0->DoingRail == 0) &&
        (arg0->CharPtr->ceilingY < arg0->floorY)) {
        camera_placeAtObject(arg0, arg0->CharPtr);
    }
    if (-10000.0f == arg0->floorY) {
        if (gIsMultiplayer != 0) {
            arg0->floorY = arg0->CharPtr->floorY;
        } else {
            arg0->floorY = 10000.0f;
            arg0->AvailableModes |= 0x800000;
            collision_findFloorEx(&arg0->TrianglePtr, arg0->floorTri.v, &arg0->floorY, &arg0->waterY, &arg0->Attributes, 0,
                          arg0->eye.x, arg0->eye.y, arg0->eye.z, arg0->prevEye.y, 0, 0, -10000.0f, arg0->eye.y, 1);
        }
    } else {
        arg0->AvailableModes &= ~0x800000;
    }
    if (-10000.0f != arg0->zoneWaterY) {
        arg0->waterY = arg0->zoneWaterY;
    }
    if (arg0->CharPtr->unk137 != 0) {
        arg0->floorY = arg0->unk354 = arg0->CharPtr->ypos;
    }
    if ((D_800C3671 != 0) || (arg0->transition.state != 0)) {
        arg0->floorY = arg0->unk354 = arg0->CharPtr->floorY;
        return;
    }
    if (((arg0->Mode & 0x100) || (arg0->Mode != 0x80)) && (arg0->floorY < arg0->waterY) &&
        (arg0->waterY < arg0->eye.y)) {
        arg0->floorY = arg0->waterY;
        arg0->unk354 = arg0->waterY;
    } else {
        arg0->unk354 = arg0->floorY;
    }
    if (arg0->DoingRail != 0) {
        if (((-10000.0f == arg0->unk354) && (arg0->unk354 < arg0->CharPtr->floorY)) ||
            (arg0->RailMagY < fabsf(arg0->unk354 - arg0->CharPtr->floorY))) {
            arg0->unk354 = arg0->CharPtr->floorY;
        }
    } else if (D_800C3671 == 0) {
        if (!(arg0->AvailableModes & 0x20000000)) {
            if (arg0->AvailableModes & 0x10000000) {
                if (arg0->unk354 < arg0->CharPtr->floorY) {
                    arg0->floorY = arg0->unk354 = arg0->CharPtr->floorY;
                }
            }
            if ((arg0->Mode != 0x80) || ((arg0->SpecialFlags & 0x80) && (arg0->CharPtr->count3 != 0)) ||
                (arg0->AvailableModes & 0x800)) {
                if (arg0->unk354 < arg0->focus.y) {
                    arg0->unk354 = arg0->focus.y;
                }
            }
        }
    }
    if (!(arg0->AvailableModes & 0x10000000) && (arg0->Mode != 0x80)) {
        if ((arg0->SpecialFlags & 1) && (flag4E != 0)) {
            arg0->floorY = arg0->CharPtr->waterHeight;
        }
        if (arg0->Mode == 0x80) {
            if (arg0->eye.y < arg0->unk354 + 30.0f) {
                arg0->eye.y = arg0->unk354 + 30.0f;
            }
            camera_updatePitch(arg0);
            return;
        }
        if ((arg0->cmdRunning != 0) && (arg0->cmd.unk34 != 0)) {
            arg0->height = arg0->cmd.unk34;
        } else {
            arg0->height = arg0->unk34C;
        }
    } else {
        arg0->unk354 = arg0->floorY;
    }
    arg0->unk354 = arg0->floorY + (arg0->unk354 - arg0->floorY);
    if (!(arg0->AvailableModes & 0x4000) || (camera_isInDeepWater(arg0) == 0)) {
        if ((arg0->CharPtr->inwater == 1) && (arg0->Mode != 0x80) && (-10000.0f != arg0->waterY)) {
            arg0->unk354 = arg0->waterY + 50.0f;
        }
    }
    if ((arg0->SpecialFlags & 0x40) && (arg0->CharPtr->unk137 == 0)) {
        if (arg0->focus.y - arg0->lastPlayerPos.y < -5.0f) {
            arg0->RopeDirection = 2;
            arg0->TargetRopeDisp += 10.0f;
        } else if (arg0->focus.y - arg0->lastPlayerPos.y > 5.0f) {
            arg0->RopeDirection = 1;
            arg0->TargetRopeDisp -= 10.0f;
        } else {
            arg0->RopeDirection = 0;
            arg0->TargetRopeDisp = 0.0f;
        }
        arg0->TargetRopeDisp = (arg0->TargetRopeDisp < -300.0f) ? -300.0f : ((arg0->TargetRopeDisp > 300.0f) ? 300.0f : arg0->TargetRopeDisp);
        math_springTo(&arg0->RopeDisp, arg0->TargetRopeDisp, &arg0->RopeDispVel, 1.0f, 1.5f, arg0->SpringTime);
        arg0->unk354 = arg0->focus.y + arg0->RopeDisp + (targetHeight * 0.8500000238f);
        arg0->unk354 = (arg0->unk354 < arg0->CharPtr->floorY + arg0->height) ? arg0->CharPtr->floorY + arg0->height : arg0->unk354;
        arg0->unk354 = (arg0->unk354 < arg0->floorY + 50.0f) ? arg0->floorY + 50.0f : arg0->unk354;
        arg0->unk354 = (arg0->unk354 < arg0->waterY + 50.0f) ? arg0->waterY + 50.0f : arg0->unk354;
        arg0->CharTrackingSpeedTarget = 0.0f;
        arg0->AvailableModes |= 0x10000000;
        arg0->unk848 = 1;
    lbl1:; // matching: the label keeps as1 from filling the branch delay slot
    } else if (arg0->unk848 != 0) {
        arg0->CharTrackingSpeedTarget = 1.0f;
        arg0->AvailableModes &= ~0x10000000;
        arg0->RopeDirection = 0;
        arg0->unk848 = 0;
    }
    if ((arg0->Mode != 0x100) && (arg0->CharPtr->unk137 == 0)) {
        if (arg0->eye.y < arg0->floorY + 40.0f) {
            arg0->eye.y = arg0->floorY + 40.0f;
        }
    }
}

/* Resets every player's z-buffer scan parameters from the defaults, narrowed for split screen and some maps,
 * and clears all scan state. */
void camera_resetZScans(void) {
    u8 i;
    u8 j;

    for (i = 0; i < 4; i++) {
        j = 0; // matching: a for loop schedules this init differently
        while (j < 4) {
            gZScanParams[i][j] = gDefaultZScanParams[j];
            if ((gScreenLayout > 0) && (gScreenLayout < 4)) {
                gZScanParams[i][j].halfWidth *= 0.5f;
                gZScanParams[i][j].unk2 *= 0.5f;
            }
            if ((level == 0x1B) || (level == 0x1E)) {
                gZScanParams[i][j].halfWidth *= 0.75f;
                gZScanParams[i][j].unk2 *= 0.75f;
            }
            j++;
        }
    }

    bzero(gZScanSets, sizeof(gZScanSets));
}


/* Projects the camera target to the screen, scans the z-buffer row through it for occluders and, when blocked,
 * swings the camera around the target (camera_getOcclusionOffset's correction, smoothed). */
void camera_checkOcclusion(Camera *arg0) {
    s32 screenX;
    s32 screenY;
    f32 *q; // matching: one local carries the projected depth (as s32) and later &D_800DC1FC
    f32 targetY;
    Vec3f pos;

    targetY = arg0->target.y;
    if (camera_clearZRow(arg0) != 0) {
        return;
    }
    if (!(arg0->AvailableModes & 0x10)) {
        if (arg0->CharPtr->floorY < arg0->CharPtr->ceilingY) {
            if (arg0->CharPtr->ceilingY - 40.0f <= targetY) {
                targetY = arg0->CharPtr->ceilingY - 40.0f;
            }
        }
    }
    view_projectPoint(arg0->CNumber, arg0->target.x, targetY, arg0->target.z, &gCameraTargetScreenX, &gCameraTargetScreenY, &gCameraTargetScreenDepth);
    q = (f32 *)(s32)gCameraTargetScreenDepth;
    screenX = ((s32)gCameraTargetScreenX < (s32)gViews[arg0->CNumber].left) ? (s32)gViews[arg0->CNumber].left : (((s32)gViews[arg0->CNumber].right < (s32)gCameraTargetScreenX) ? (s32)gViews[arg0->CNumber].right : (s32)gCameraTargetScreenX);
    screenY = ((s32)gCameraTargetScreenY < (s32)gViews[arg0->CNumber].top) ? (s32)gViews[arg0->CNumber].top : (((s32)gViews[arg0->CNumber].bottom < (s32)gCameraTargetScreenY) ? (s32)gViews[arg0->CNumber].bottom : (s32)gCameraTargetScreenY);
    if ((screenX < gViews[arg0->CNumber].left) || (gViews[arg0->CNumber].right <= screenX) || (screenY < gViews[arg0->CNumber].top) || (gViews[arg0->CNumber].bottom <= screenY)) {
        arg0->SideHitsClear = 1;
        arg0->HitsZBufferReadFlag = 0;
        return;
    }
    arg0->HitsZBufferReadFlag = 1;
    arg0->HitsZBufferRowIndex = screenY;
    camera_scanZRow(arg0, screenX, screenY, (s32)q);
    pos = arg0->eye;
    camera_getOcclusionOffset(arg0, &arg0->SideHitsRxz, &D_800DC1FC);
    if (fabsf(arg0->SideHitsRxz) > 3.0f) {
        math_rotateAroundY(&pos, &arg0->target, arg0->SideHitsRxz, &pos);
        {
        f32 rate1;
        f32 rate2;

        rate1 = 6.0f;
        rate2 = 8.0f;
        if (arg0->PadPtr->button & 3) {
            rate1 *= 1.2f;
            rate2 *= 1.2f;
        }
        math_springTo(&arg0->eye.x, pos.x, &arg0->HitVelX, rate1, rate2, arg0->SpringTime);
        math_springTo(&arg0->eye.y, pos.y, &arg0->HitVelY, rate1, rate2, arg0->SpringTime);
        math_springTo(&arg0->eye.z, pos.z, &arg0->HitVelZ, rate1, rate2, arg0->SpringTime);
        }
        arg0->SideHitsClear = 0;
    lbl:; // matching: the label keeps as1 from filling the branch delay slot below
    } else {
        arg0->SideHitsClear = 1;
    }
    q = &D_800DC1FC;
    if (0.0f != D_800DC1FC) {
        arg0->eye.y -= *q;
    }
    if (arg0->SpecialMove == 1) {
        arg0->SpecialMove = 0;
    }
    arg0->unk698 = 0;
}

/* Runs the z-buffer occlusion check (camera_checkOcclusion) when the camera mode allows it; otherwise marks the
 * check as off and arms the unk5FC/unk5FE countdowns. */
void camera_updateOcclusion(Camera *arg0) {
    if ((arg0->Mode & ~0x100) && (gPaused == 0) && (D_800C3671 == 0)) {
        if (arg0->Mode != 0x40) {
            if ((arg0->AvailableModes & 8) && !(arg0->AvailableModes & 0x200) && (arg0->Snap == 0)) {
                if (!(arg0->SpecialFlags & 0x80) && !(arg0->SpecialFlags & 0x40)) {
                    camera_checkOcclusion(arg0);
                    return;
                }
            }
        }
        arg0->SideHitsClear = 1;
        arg0->SHitsOffLastFrame = 2;
        arg0->unk5FE = 60;
    }
}

/* Scans the z-buffer row arg0->unk8BC outwards from screenX, left then right, comparing each pixel's depth
 * with the target's depth; the first occluder on each side is recorded in that side's ZScan. */
void camera_scanZRow(Camera *arg0, s32 screenX, s32 screenY, s32 depth) {
    s32 gaps[0x281];
    s32 start;
    s32 i;
    s32 mid;
    s16 halfWidth;
    u16 zcode;
    ZDecode *fmt;
    ZScanParams *params;
    s16 height;

    params = &gZScanParams[arg0->CNumber][arg0->DistIndex];
    halfWidth = params->halfWidth;
    height = params->unk2;
    arg0->CurrentHitsWidth += (halfWidth - arg0->CurrentHitsWidth) * 0.05000000075f;
    halfWidth = (s16)arg0->CurrentHitsWidth + 0.5f;
    arg0->CurrentHitsHeight += (height - arg0->CurrentHitsHeight) * 0.05000000075f;
    start = halfWidth / 4;
    mid = (halfWidth * 3) / 4;

    for (i = start; i < halfWidth; i++) {
        if (gViews[arg0->CNumber].left < screenX - i) {
            {
            s16 pad; // matching: block-scoped pad that IDO gives a stack slot
            zcode = arg0->HitsZBufferHoriDataPtr[screenX - i];
            }
            // decode the 14-bit z value: 3-bit exponent, 11-bit mantissa (low 2 bits are dz)
            fmt = &gZDecodeTable[zcode >> 13];
            gaps[halfWidth - i] = depth - ((u32)(fmt->base + (((zcode >> 2) & 0x7FF) << fmt->shift)) >> 3);
            if (camera_testZPixel(arg0, &gZScanSets[arg0->CNumber].scan[0], gaps[halfWidth - i], i, mid, 0)) {
                i = halfWidth; // stop scanning
            }
        }
    }
    for (i = start; i < halfWidth; i++) {
        if (screenX + i < gViews[arg0->CNumber].right) {
            {
            s16 pad; // matching: block-scoped pad that IDO gives a stack slot
            zcode = arg0->HitsZBufferHoriDataPtr[screenX + i];
            }
            fmt = &gZDecodeTable[zcode >> 13];
            gaps[i + halfWidth] = depth - ((u32)(fmt->base + (((zcode >> 2) & 0x7FF) << fmt->shift)) >> 3);
            if (camera_testZPixel(arg0, &gZScanSets[arg0->CNumber].scan[1], gaps[i + halfWidth], i, mid, 0)) {
                i = halfWidth; // stop scanning
            }
        }
    }
}



/* While the unk5FC countdown runs, clears the z-buffer row to the far value and returns 1. */
s32 camera_clearZRow(Camera *arg0) {
    if (arg0->SHitsOffLastFrame != 0) {
        s32 i;

        arg0->SideHitsClear = 1;
        for (i = 0; i < gViews[arg0->CNumber].width; i++) {
            arg0->HitsZBufferHoriDataPtr[i] = 0xFFFC;
        }
        arg0->CurrentHitsWidth = 0.0f;
        arg0->SHitsOffLastFrame--;
        return 1;
    }
    return 0;
}


/* Records a hit at offset in scan, noting whether it moved inwards (movingIn) since the previous one. */
void camera_recordZHit(ZScan *scan, s32 offset, s32 gap) {
    scan->prevMovingIn = scan->movingIn;
    if (scan->offset < offset) {
        scan->movingIn = 0;
    } else {
        scan->movingIn = 1;
    }
    scan->gap = gap;
    scan->offset = offset;
}

/* Tests one scanned pixel: a depth gap above the threshold is an occluder, state 2 when it lies inside mid
 * and 1 when outside. Returns 1 when the pixel is an occluder, else resets scan and returns 0. */
s32 camera_testZPixel(Camera *arg0, ZScan *scan, s32 gap, s32 offset, s32 mid, s32 useAltThreshold) {
    s32 threshold;

    if (useAltThreshold == 0) {
        threshold = gZScanParams[arg0->CNumber][arg0->DistIndex].threshold;
    } else {
        threshold = gZScanParams[arg0->CNumber][arg0->DistIndex].altThreshold;
    }

    if (arg0->AvailableModes & 0x8000) {
        threshold = (s16)(threshold << 1);
    }

    if ((threshold < gap) && (offset < mid)) {
        scan->state = 2;
        camera_recordZHit(scan, offset, gap);
        return 1;
    }

    if ((threshold < gap) && (mid < offset)) {
        scan->state = 1;
        camera_recordZHit(scan, offset, gap);
        return 1;
    }

    scan->movingIn = 0;
    scan->state = 0;
    scan->gap = 0xFFFC;
    return 0;
}


/* Turns the left/right z-buffer scans into a sideways correction *offset for the camera.
 * Returns whether either side is blocked. */
s32 camera_getOcclusionOffset(Camera *arg0, f32 *offset, f32 *unused) {
    s32 blocked;
    u8 state0;
    u8 state1;
    u8 state2;
    u8 state3;
    f32 edge0;
    f32 edge1;
    f32 halfWidth;

    blocked = 0;
    state0 = gZScanSets[arg0->CNumber].scan[0].state;
    state1 = gZScanSets[arg0->CNumber].scan[1].state;
    state2 = gZScanSets[arg0->CNumber].scan[2].state;
    state3 = gZScanSets[arg0->CNumber].scan[3].state;
    edge0 = gZScanSets[arg0->CNumber].scan[0].offset + 1.0f;
    edge1 = gZScanSets[arg0->CNumber].scan[1].offset + 1.0f;
    if (state0 == 2 && state1 != 2) {
        gZScanSets[arg0->CNumber].unk30 = 0;
    } else if (state0 != 2 && state1 == 2) {
        gZScanSets[arg0->CNumber].unk30 = 1;
    }
    if (state2 == 2 && state3 != 2) {
        gZScanSets[arg0->CNumber].unk30 = 2;
    } else if (state2 != 2 && state3 == 2) {
        gZScanSets[arg0->CNumber].unk30 = 3;
    }
    if (state0 == 2 || state1 == 2) {
        halfWidth = gZScanParams[arg0->CNumber][arg0->DistIndex].halfWidth + 1;
        if (state0 == 2 && state1 == 2) {
            if (arg0->CharPtr->inwater != 1 && arg0->mv->death_State == 0) {
                arg0->unk5FE -= D_800BE9E4;
                if (arg0->unk5FE < 0) {
                    arg0->unk5FE = 0;
                }
            }
            if (arg0->Mode != 0x80) {
                arg0->SHitsPIPOutTimer = 30;
            }
            if (gZScanSets[arg0->CNumber].scan[1].offset < gZScanSets[arg0->CNumber].scan[0].offset) {
                *offset = halfWidth - edge0;
            } else if (gZScanSets[arg0->CNumber].scan[0].offset < gZScanSets[arg0->CNumber].scan[1].offset) {
                *offset = edge1 - halfWidth;
            } else if (gZScanSets[arg0->CNumber].unk30 == 0) {
                *offset = halfWidth - edge0;
            } else {
                *offset = edge1 - halfWidth;
            }
        } else {
            arg0->SHitsPIPOutTimer = 0;
            arg0->unk5FE = 60;
            // matching: the unk23C tests below have identical branches in the original
            if (gZScanSets[arg0->CNumber].scan[0].state == 2) {
                if (arg0->Snap != 0) {
                    *offset = halfWidth - (gZScanSets[arg0->CNumber].scan[0].offset + 1.0f);
                } else {
                    *offset = halfWidth - (gZScanSets[arg0->CNumber].scan[0].offset + 1.0f);
                }
            } else {
                if (arg0->Snap != 0) {
                    *offset = (gZScanSets[arg0->CNumber].scan[1].offset + 1.0f) - halfWidth;
                } else {
                    *offset = (gZScanSets[arg0->CNumber].scan[1].offset + 1.0f) - halfWidth;
                }
            }
        }
        {
        s32 pad; // matching: IDO reserves a stack slot for this block-scoped unused local
        blocked = 1;
        arg0->SlackenK = 0.004999999888f;
        }
    } else {
        gZScanSets[arg0->CNumber].scan[0].state = 4;
        gZScanSets[arg0->CNumber].scan[1].state = 4;
        *offset -= *offset * 0.1000000015f;
        arg0->unk5FE = 60;
    }
    if (arg0->SHitsPIPOutTimer != 0) {
        arg0->SHitsPIPOutTimer -= D_800BE9E4;
        if (arg0->SHitsPIPOutTimer < 0) {
            arg0->SHitsPIPOutTimer = 0;
        }
        if (arg0->SHitsPIPOutXZ == -1.0f) {
            arg0->SHitsPIPOutXZ = arg0->IdealTripodMagnitude;
            arg0->SHitsPIPOutY = arg0->height;
            arg0->IdealTripodMagnitude = 150.0f;
            arg0->height = 275.0f;
        }
    } else if (arg0->SHitsPIPOutXZ != -1.0f) {
        arg0->IdealTripodMagnitude = arg0->SHitsPIPOutXZ;
        arg0->height = arg0->SHitsPIPOutY;
        arg0->SHitsPIPOutXZ = -1.0f;
        arg0->SHitsPIPOutY = -1.0f;
    }
    return blocked;
}

/* Picks the camera collision radii for the current mode, then moves a temporary probe object (type 0x2D)
 * from the camera position through the world and applies the corrected position. */
void camera_collide(Camera *arg0) {
    s32 savedDD3;
    s32 flag;
    Actor probe;
    s32 hit;
    s32 saved2;
    s32 saved1;
    s32 savedDD4;
    f32 floorY;
    ActorCtl *sub;

    flag = 0;
    // matching: sub is loaded inside the condition, after the first flag test
    if (!(arg0->AvailableModes & 0x40000) && (sub = arg0->mv, sub->ladder == 0) && (arg0->Mode != 0x40) &&
        !(arg0->AvailableModes & 0x80) && !(arg0->AvailableModes & 0x200)) {
        saved2 = gFloorProbeTypeEnabled[2];
        saved1 = gFloorProbeTypeEnabled[1];
        if (arg0->SpecialFlags & 0x800) {
            arg0->unk94C = 20.0f;
            arg0->unk950 = 40.0f;
        } else if (arg0->SpecialFlags & 0x10) {
            arg0->unk94C = 20.0f;
            arg0->unk950 = 72.0f;
        } else if (arg0->IdealTripodMagnitude > 580.0f) {
            arg0->unk94C = 70.0f;
            arg0->unk950 = 140.0f;
        } else if ((level == 0x31) && (arg0->CharContext == 3)) {
            flag = 1;
            arg0->unk94C = 20.0f;
            arg0->unk950 = 50.0f;
        } else {
            if (((arg0->DoingAim != 0) && (arg0->DoingAim != 1) && (arg0->DoingAim != 3)) ||
                ((arg0->CharPtr->unk102 != 0) &&
                 ((arg0->CharPtr->xpos != arg0->lastPlayerPos.x) || (arg0->CharPtr->zpos != arg0->lastPlayerPos.z))) ||
                (sub->unk1B3 != 0)) {
                arg0->unk94C = 20.0f;
                arg0->unk950 = 20.0f;
            } else if (arg0->CharPtr->unk102 != 0) {
                flag = 1;
                arg0->unk94C = 25.0f;
                arg0->unk950 = 30.0f;
            } else {
                arg0->unk94C = 30.0f;
                arg0->unk950 = 60.0f;
            }
        }
        if (arg0->Snap != 0) {
            arg0->unk954 = 0.0f;
            arg0->unk95C = arg0->unk94C;
            arg0->unk958 = 0.0f;
            arg0->unk960 = arg0->unk950;
        } else {
            math_springTo(&arg0->unk95C, arg0->unk94C, &arg0->unk954, 4.0f, 6.0f, arg0->SpringTime);
            math_springTo(&arg0->unk960, arg0->unk950, &arg0->unk958, 4.0f, 6.0f, arg0->SpringTime);
        }
        probe.type = 0x2D;
        probe.CameraPtr = arg0;
        probe.xpos = arg0->eye.x;
        if (arg0->SpecialFlags & 0x800) {
            probe.ypos = arg0->eye.y + arg0->YOffset;
        } else {
            probe.ypos = arg0->eye.y;
        }
        probe.zpos = arg0->eye.z;
        probe.floorTriVerts = arg0->TrianglePtr;
        probe.shad = probe.ypos - arg0->unk354;
        probe.floorY = arg0->unk354;
        probe.yspd = 0.0f;
        probe.theta = arg0->CameraAngle;
        if ((arg0->AvailableModes & 0x80000000) || (level == 0x37)) {
            gFloorProbeTypeEnabled[2] = 0;
        }
        if (arg0->AvailableModes & 0x10000) {
            gFloorProbeTypeEnabled[1] = 0;
        }
        D_800CBDD2 = 1;
        savedDD3 = D_800CBDD3;
        if (arg0->CharPtr->unk102 != 0) {
            D_800CBDD3 = 1;
        } else {
            D_800CBDD3 = 0;
        }
        savedDD4 = D_800CBDD4;
        if ((arg0->Snap != 0) || ((arg0->Mode == 0x100) && (flag == 0))) {
            D_800CBDD4 = 1;
        } else {
            D_800CBDD4 = 0;
        }
        hit = collision_resolveActorWalls(arg0->prevEye.x, arg0->prevEye.y, arg0->prevEye.z, &probe, 0, 0);
        D_800CBDD4 = savedDD4;
        D_800CBDD2 = 0;
        D_800CBDD3 = savedDD3;
        gFloorProbeTypeEnabled[2] = saved2;
        gFloorProbeTypeEnabled[1] = saved1;
        if (hit != 0) {
            arg0->TonyHitsClear = 0;
            arg0->SlackenK = 0.004999999888f;
        } else {
            arg0->TonyHitsClear = 1;
            arg0->SlackenK += (1.0f - arg0->SlackenK) * 0.1000000015f;
        }
        arg0->eye.x = probe.xpos;
        if (arg0->Mode != 0x100) {
            if (arg0->SpecialFlags & 0x800) {
                arg0->eye.y = probe.ypos - arg0->YOffset;
            } else {
                arg0->eye.y = probe.ypos;
            }
        }
        arg0->eye.z = probe.zpos;
        if ((level != 0x1B) && (hit != 0) && (arg0->Mode != 0x100)) {
            collision_findFloorEx(NULL, NULL, &floorY, NULL, NULL, 0, arg0->eye.x, arg0->eye.y, arg0->eye.z,
                          arg0->prevEye.y, 0, 0, -10000.0f, arg0->eye.y, 1);
            if (arg0->eye.y < floorY + 40.0f) {
                s32 pad; // matching: IDO reserves a stack slot for this block-scoped unused local
                arg0->eye.y = floorY + 40.0f;
            }
        }
    } else {
        arg0->TonyHitsClear = 1;
    }
    camera_updateStatus(arg0);
}

/* Measures the horizontal distance to the target; when too close, swings the yaw by 90 degrees. */
void camera_swingIfTooClose(Camera *arg0) {
    f32 side;
    f32 dx;
    f32 dz;
    f32 dist;

    if (arg0->CharRelAngle < 180.0f) {
        side = 2.0f;
    } else {
        side = -2.0f;
    }

    if ((arg0->Mode != 0x80) && (arg0->Mode != 0x40)) {
        dx = arg0->eye.x - arg0->target.x;
        dz = arg0->eye.z - arg0->target.z;
        dist = sqrtf((dx * dx) + (dz * dz));
        arg0->TripodMagnitude = dist;

        if (dist < 42.0f) {
            arg0->LockedCameraAngle = arg0->CameraAngle - (45.0f * side);
            arg0->LockedCamAngleK = 0.0f;
            camera_updateFollow(arg0);
            arg0->SpecialMove |= 2;
        }
    }

    camera_blendYaw(arg0);
}

/* Advances the blend factor unk5CC towards 1, easing the yaw unk384 towards unk37C until it gets there. */
void camera_blendYaw(Camera *arg0) {
    arg0->LockedCamAngleK += 0.006000000052f;
    arg0->LockedCamAngleK = (arg0->LockedCamAngleK > 1.0f) ? 1.0f : arg0->LockedCamAngleK;

    if (arg0->LockedCamAngleK == 1.0f) {
        arg0->LockedCameraAngle = 999.0f;
        arg0->SpecialMove &= ~2;
    } else {
        arg0->LockedCameraAngle += math_angleDiff(arg0->LockedCameraAngle, arg0->CameraAngle) * arg0->LockedCamAngleK;
    }
}

void func_1512C200(Camera *camera) {
}

#define SQ(x) ((x) * (x))

/* Keeps the camera outside the circle of radius avoidRadius around avoidCenter (horizontally), pushing it out to the edge. */
void camera_keepOutsideCircle(Camera *arg0) {
    f32 dz; // matching: also holds the target distance later (a separate local grows the frame)
    Vec3f pos;
    Vec3f offset;
    Vec3f toPos;

    if (arg0->avoidActive) {
        f32 dx; // matching: also holds the yaw difference later

        dx = arg0->eye.x - arg0->avoidCenter.x;
        dz = arg0->eye.z - arg0->avoidCenter.z;
        pos = arg0->eye;
        if (sqrtf(SQ(dx) + SQ(dz)) < arg0->avoidRadius) {
            Vec3f target;
            f32 outwardYaw;
            f32 yaw;

            target = arg0->focus;
            target.y = 0.0f;
            pos.y = 0.0f;
            arg0->avoidCenter.y = 0.0f;
            math_dirTowards(&arg0->avoidCenter, &pos, &offset);
            outwardYaw = math_vec3Heading(&offset) - 180.0f;
            math_normalizeDegrees(&outwardYaw);
            math_dirTowards(&target, &pos, &toPos);
            math_vec3Scale(&offset, arg0->avoidRadius, &offset);
            pos.x = arg0->avoidCenter.x + offset.x;
            pos.y = 0.0f;
            pos.z = arg0->avoidCenter.z + offset.z;
            dz = sqrtf(SQ(target.x - pos.x) + SQ(target.z - pos.z));
            math_vec3Diff(&pos, &target, &offset);
            yaw = math_vec3Heading(&offset);
            math_normalizeDegrees(&yaw);
            arg0->avoidYaw = yaw;
            dx = math_angleDiff(outwardYaw, yaw);
            if (fabsf(dx) < 35.0f) {
                if (dx <= 0.0f) {
                    dx = 5.0f;
                } else {
                    dx = -5.0f;
                }
                math_rotateAroundY(&pos, &arg0->avoidCenter, dx, &pos);
            }
            gCameraAvoidTargetDist = dz;
            pos.y = arg0->eye.y;
            arg0->eye = pos;
        } else {
            gCameraAvoidTargetDist = 0.0f;
            arg0->avoidYaw = arg0->CameraAngle;
        }
    } else {
        gCameraAvoidTargetDist = 0.0f;
        arg0->avoidYaw = 0.0f;
    }
    arg0->avoidActive = 0;
}

s32 func_1512C47C(s32 arg0) {
    return 1;
}
