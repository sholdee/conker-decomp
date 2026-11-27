#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/game.h"
#include "conker/math.h"
#include "conker/view.h"

/* Z-buffer scan parameters (gZScanParams[player][DistIndex], copied from gDefaultZScanParams by camera_resetZScans). */
typedef struct {
    s16 halfWidth; // pixels scanned on each side of the target
    s16 height;
    s16 threshold; // depth gap that counts as an occluder
    s16 altThreshold;
} ZScanParams;

/* One set of scans per player. */
typedef struct {
    /* 0x00 */ ZScan scan[4];
    /* 0x30 */ u8 lastBlockedScan;
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
static f32 gOcclusionEyeDrop;
static ZScanParams gZScanParams[4][4]; // per player

/* Updates the camera's floor heights (floorY, waterY; -10000 means none found) and the base y its heights are
 * measured from (baseY), with special cases for the camera modes and the tracked objects. */
void camera_updateFloorHeights(Camera *camera) {
    f32 unusedShad;
    s32 transport;
    s16 height;
    s16 width;
    s16 depth;
    f32 targetHeight;
    f32 pad68; // matching: frame padding
    f32 pad64;

    unusedShad = camera->CharPtr->shad;
    if (camera->mv != NULL) {
        transport = camera->mv->transport;
        targetHeight = camera->mv->height;
    } else {
        transport = 0;
        actor_computeSize(camera->CharPtr, &height, &width, &depth);
        targetHeight = height;
    }
    if (!camera->AvailableModes & 0x40) { // sic: (!AvailableModes) & 0x40 is always 0, but the original computes it
        camera->floorY = camera->baseY = camera->CharPtr->floorY;
        return;
    }
    if ((camera->Snap != 0) || (camera->AvailableModes & 0x400000) || (camera->SpecialFlags & 0x40) ||
        (collision_getTriHeightAt(camera->eye.x, camera->eye.z, camera->TrianglePtr, camera->floorTri.v, &camera->floorY) == 0)) {
        collision_findFloorEx(&camera->TrianglePtr, camera->floorTri.v, &camera->floorY, &camera->waterY, &camera->Attributes, 0,
                      camera->eye.x, camera->floorY, camera->eye.z, camera->prevEye.y, 0, 0, -10000.0f, camera->eye.y, 1);
    }
    if (unusedShad) {} // matching: keeps the otherwise dead shad load
    if (level == LEVEL_W2_TANKTUNNEL) {
        camera->zoneWaterY = -10000.0f;
        camera->waterY = -10000.0f;
    }
    if (-10000.0f != camera->zoneWaterY) {
        camera->waterY = camera->zoneWaterY;
    }
    camera_updateStatus(camera);
    if (!(camera->AvailableModes & 0x10) && (camera->CharPtr->unk102 == 0) && (gSpawnCutsceneActive == 0) && (camera->DoingRail == 0) &&
        (camera->CharPtr->ceilingY < camera->floorY)) {
        camera_placeAtObject(camera, camera->CharPtr);
    }
    if (-10000.0f == camera->floorY) {
        if (gIsMultiplayer != 0) {
            camera->floorY = camera->CharPtr->floorY;
        } else {
            camera->floorY = 10000.0f;
            camera->AvailableModes |= 0x800000;
            collision_findFloorEx(&camera->TrianglePtr, camera->floorTri.v, &camera->floorY, &camera->waterY, &camera->Attributes, 0,
                          camera->eye.x, camera->eye.y, camera->eye.z, camera->prevEye.y, 0, 0, -10000.0f, camera->eye.y, 1);
        }
    } else {
        camera->AvailableModes &= ~0x800000;
    }
    if (-10000.0f != camera->zoneWaterY) {
        camera->waterY = camera->zoneWaterY;
    }
    if (camera->CharPtr->unk137 != 0) {
        camera->floorY = camera->baseY = camera->CharPtr->ypos;
    }
    if ((gSpawnCutsceneActive != 0) || (camera->transition.state != 0)) {
        camera->floorY = camera->baseY = camera->CharPtr->floorY;
        return;
    }
    if (((camera->Mode & 0x100) || (camera->Mode != 0x80)) && (camera->floorY < camera->waterY) &&
        (camera->waterY < camera->eye.y)) {
        camera->floorY = camera->waterY;
        camera->baseY = camera->waterY;
    } else {
        camera->baseY = camera->floorY;
    }
    if (camera->DoingRail != 0) {
        if (((-10000.0f == camera->baseY) && (camera->baseY < camera->CharPtr->floorY)) ||
            (camera->RailMagY < fabsf(camera->baseY - camera->CharPtr->floorY))) {
            camera->baseY = camera->CharPtr->floorY;
        }
    } else if (gSpawnCutsceneActive == 0) {
        if (!(camera->AvailableModes & 0x20000000)) {
            if (camera->AvailableModes & 0x10000000) {
                if (camera->baseY < camera->CharPtr->floorY) {
                    camera->floorY = camera->baseY = camera->CharPtr->floorY;
                }
            }
            if ((camera->Mode != 0x80) || ((camera->SpecialFlags & 0x80) && (camera->CharPtr->count3 != 0)) ||
                (camera->AvailableModes & 0x800)) {
                if (camera->baseY < camera->focus.y) {
                    camera->baseY = camera->focus.y;
                }
            }
        }
    }
    if (!(camera->AvailableModes & 0x10000000) && (camera->Mode != 0x80)) {
        if ((camera->SpecialFlags & 1) && (transport != 0)) {
            camera->floorY = camera->CharPtr->waterHeight;
        }
        if (camera->Mode == 0x80) {
            if (camera->eye.y < camera->baseY + 30.0f) {
                camera->eye.y = camera->baseY + 30.0f;
            }
            camera_updatePitch(camera);
            return;
        }
        if ((camera->cmdRunning != 0) && (camera->cmd.height != 0)) {
            camera->height = camera->cmd.height;
        } else {
            camera->height = camera->idealHeight;
        }
    } else {
        camera->baseY = camera->floorY;
    }
    camera->baseY = camera->floorY + (camera->baseY - camera->floorY);
    if (!(camera->AvailableModes & 0x4000) || (camera_isInDeepWater(camera) == 0)) {
        if ((camera->CharPtr->inwater == 1) && (camera->Mode != 0x80) && (-10000.0f != camera->waterY)) {
            camera->baseY = camera->waterY + 50.0f;
        }
    }
    if ((camera->SpecialFlags & 0x40) && (camera->CharPtr->unk137 == 0)) {
        if (camera->focus.y - camera->lastPlayerPos.y < -5.0f) {
            camera->RopeDirection = 2;
            camera->TargetRopeDisp += 10.0f;
        } else if (camera->focus.y - camera->lastPlayerPos.y > 5.0f) {
            camera->RopeDirection = 1;
            camera->TargetRopeDisp -= 10.0f;
        } else {
            camera->RopeDirection = 0;
            camera->TargetRopeDisp = 0.0f;
        }
        camera->TargetRopeDisp = (camera->TargetRopeDisp < -300.0f) ? -300.0f : ((camera->TargetRopeDisp > 300.0f) ? 300.0f : camera->TargetRopeDisp);
        math_springTo(&camera->RopeDisp, camera->TargetRopeDisp, &camera->RopeDispVel, 1.0f, 1.5f, camera->SpringTime);
        camera->baseY = camera->focus.y + camera->RopeDisp + (targetHeight * 0.8500000238f);
        camera->baseY = (camera->baseY < camera->CharPtr->floorY + camera->height) ? camera->CharPtr->floorY + camera->height : camera->baseY;
        camera->baseY = (camera->baseY < camera->floorY + 50.0f) ? camera->floorY + 50.0f : camera->baseY;
        camera->baseY = (camera->baseY < camera->waterY + 50.0f) ? camera->waterY + 50.0f : camera->baseY;
        camera->CharTrackingSpeedTarget = 0.0f;
        camera->AvailableModes |= 0x10000000;
        camera->ropeActive = 1;
    lbl1:; // matching: the label keeps as1 from filling the branch delay slot
    } else if (camera->ropeActive != 0) {
        camera->CharTrackingSpeedTarget = 1.0f;
        camera->AvailableModes &= ~0x10000000;
        camera->RopeDirection = 0;
        camera->ropeActive = 0;
    }
    if ((camera->Mode != 0x100) && (camera->CharPtr->unk137 == 0)) {
        if (camera->eye.y < camera->floorY + 40.0f) {
            camera->eye.y = camera->floorY + 40.0f;
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
                gZScanParams[i][j].height *= 0.5f;
            }
            if ((level == LEVEL_W2_LIFTTUNNEL) || (level == LEVEL_W2_FLAMETUNNEL)) {
                gZScanParams[i][j].halfWidth *= 0.75f;
                gZScanParams[i][j].height *= 0.75f;
            }
            j++;
        }
    }

    bzero(gZScanSets, sizeof(gZScanSets));
}


/* Projects the camera target to the screen, scans the z-buffer row through it for occluders and, when blocked,
 * swings the camera around the target (camera_getOcclusionOffset's correction, smoothed). */
void camera_checkOcclusion(Camera *camera) {
    s32 screenX;
    s32 screenY;
    f32 *q; // matching: one local carries the projected depth (as s32) and later &gOcclusionEyeDrop
    f32 targetY;
    Vec3f pos;

    targetY = camera->target.y;
    if (camera_clearZRow(camera) != 0) {
        return;
    }
    if (!(camera->AvailableModes & 0x10)) {
        if (camera->CharPtr->floorY < camera->CharPtr->ceilingY) {
            if (camera->CharPtr->ceilingY - 40.0f <= targetY) {
                targetY = camera->CharPtr->ceilingY - 40.0f;
            }
        }
    }
    view_projectPoint(camera->CNumber, camera->target.x, targetY, camera->target.z, &gCameraTargetScreenX, &gCameraTargetScreenY, &gCameraTargetScreenDepth);
    q = (f32 *)(s32)gCameraTargetScreenDepth;
    screenX = ((s32)gCameraTargetScreenX < (s32)gViews[camera->CNumber].left) ? (s32)gViews[camera->CNumber].left : (((s32)gViews[camera->CNumber].right < (s32)gCameraTargetScreenX) ? (s32)gViews[camera->CNumber].right : (s32)gCameraTargetScreenX);
    screenY = ((s32)gCameraTargetScreenY < (s32)gViews[camera->CNumber].top) ? (s32)gViews[camera->CNumber].top : (((s32)gViews[camera->CNumber].bottom < (s32)gCameraTargetScreenY) ? (s32)gViews[camera->CNumber].bottom : (s32)gCameraTargetScreenY);
    if ((screenX < gViews[camera->CNumber].left) || (gViews[camera->CNumber].right <= screenX) || (screenY < gViews[camera->CNumber].top) || (gViews[camera->CNumber].bottom <= screenY)) {
        camera->SideHitsClear = 1;
        camera->HitsZBufferReadFlag = 0;
        return;
    }
    camera->HitsZBufferReadFlag = 1;
    camera->HitsZBufferRowIndex = screenY;
    camera_scanZRow(camera, screenX, screenY, (s32)q);
    pos = camera->eye;
    camera_getOcclusionOffset(camera, &camera->SideHitsRxz, &gOcclusionEyeDrop);
    if (fabsf(camera->SideHitsRxz) > 3.0f) {
        math_rotateAroundY(&pos, &camera->target, camera->SideHitsRxz, &pos);
        {
        f32 rate1;
        f32 rate2;

        rate1 = 6.0f;
        rate2 = 8.0f;
        if (camera->PadPtr->button & 3) {
            rate1 *= 1.2f;
            rate2 *= 1.2f;
        }
        math_springTo(&camera->eye.x, pos.x, &camera->HitVelX, rate1, rate2, camera->SpringTime);
        math_springTo(&camera->eye.y, pos.y, &camera->HitVelY, rate1, rate2, camera->SpringTime);
        math_springTo(&camera->eye.z, pos.z, &camera->HitVelZ, rate1, rate2, camera->SpringTime);
        }
        camera->SideHitsClear = 0;
    lbl:; // matching: the label keeps as1 from filling the branch delay slot below
    } else {
        camera->SideHitsClear = 1;
    }
    q = &gOcclusionEyeDrop;
    if (0.0f != gOcclusionEyeDrop) {
        camera->eye.y -= *q;
    }
    if (camera->SpecialMove == 1) {
        camera->SpecialMove = 0;
    }
    camera->unk698 = 0;
}

/* Runs the z-buffer occlusion check (camera_checkOcclusion) when the camera mode allows it; otherwise marks the
 * check as off and arms the SHitsOffLastFrame/boxedInTimer countdowns. */
void camera_updateOcclusion(Camera *camera) {
    if ((camera->Mode & ~0x100) && (gPaused == 0) && (gSpawnCutsceneActive == 0)) {
        if (camera->Mode != 0x40) {
            if ((camera->AvailableModes & 8) && !(camera->AvailableModes & 0x200) && (camera->Snap == 0)) {
                if (!(camera->SpecialFlags & 0x80) && !(camera->SpecialFlags & 0x40)) {
                    camera_checkOcclusion(camera);
                    return;
                }
            }
        }
        camera->SideHitsClear = 1;
        camera->SHitsOffLastFrame = 2;
        camera->boxedInTimer = 60;
    }
}

/* Scans the z-buffer row camera->HitsZBufferHoriDataPtr outwards from screenX, left then right, comparing each pixel's depth
 * with the target's depth; the first occluder on each side is recorded in that side's ZScan. */
void camera_scanZRow(Camera *camera, s32 screenX, s32 screenY, s32 depth) {
    s32 gaps[0x281];
    s32 start;
    s32 i;
    s32 mid;
    s16 halfWidth;
    u16 zcode;
    ZDecode *fmt;
    ZScanParams *params;
    s16 height;

    params = &gZScanParams[camera->CNumber][camera->DistIndex];
    halfWidth = params->halfWidth;
    height = params->height;
    camera->CurrentHitsWidth += (halfWidth - camera->CurrentHitsWidth) * 0.05000000075f;
    halfWidth = (s16)camera->CurrentHitsWidth + 0.5f;
    camera->CurrentHitsHeight += (height - camera->CurrentHitsHeight) * 0.05000000075f;
    start = halfWidth / 4;
    mid = (halfWidth * 3) / 4;

    for (i = start; i < halfWidth; i++) {
        if (gViews[camera->CNumber].left < screenX - i) {
            {
            s16 pad; // matching: block-scoped pad that IDO gives a stack slot
            zcode = camera->HitsZBufferHoriDataPtr[screenX - i];
            }
            // decode the 14-bit z value: 3-bit exponent, 11-bit mantissa (low 2 bits are dz)
            fmt = &gZDecodeTable[zcode >> 13];
            gaps[halfWidth - i] = depth - ((u32)(fmt->base + (((zcode >> 2) & 0x7FF) << fmt->shift)) >> 3);
            if (camera_testZPixel(camera, &gZScanSets[camera->CNumber].scan[0], gaps[halfWidth - i], i, mid, 0)) {
                i = halfWidth; // stop scanning
            }
        }
    }
    for (i = start; i < halfWidth; i++) {
        if (screenX + i < gViews[camera->CNumber].right) {
            {
            s16 pad; // matching: block-scoped pad that IDO gives a stack slot
            zcode = camera->HitsZBufferHoriDataPtr[screenX + i];
            }
            fmt = &gZDecodeTable[zcode >> 13];
            gaps[i + halfWidth] = depth - ((u32)(fmt->base + (((zcode >> 2) & 0x7FF) << fmt->shift)) >> 3);
            if (camera_testZPixel(camera, &gZScanSets[camera->CNumber].scan[1], gaps[i + halfWidth], i, mid, 0)) {
                i = halfWidth; // stop scanning
            }
        }
    }
}



/* While the SHitsOffLastFrame countdown runs, clears the z-buffer row to the far value and returns 1. */
s32 camera_clearZRow(Camera *camera) {
    if (camera->SHitsOffLastFrame != 0) {
        s32 i;

        camera->SideHitsClear = 1;
        for (i = 0; i < gViews[camera->CNumber].width; i++) {
            camera->HitsZBufferHoriDataPtr[i] = 0xFFFC;
        }
        camera->CurrentHitsWidth = 0.0f;
        camera->SHitsOffLastFrame--;
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
s32 camera_testZPixel(Camera *camera, ZScan *scan, s32 gap, s32 offset, s32 mid, s32 useAltThreshold) {
    s32 threshold;

    if (useAltThreshold == 0) {
        threshold = gZScanParams[camera->CNumber][camera->DistIndex].threshold;
    } else {
        threshold = gZScanParams[camera->CNumber][camera->DistIndex].altThreshold;
    }

    if (camera->AvailableModes & 0x8000) {
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
s32 camera_getOcclusionOffset(Camera *camera, f32 *offset, f32 *unused) {
    s32 blocked;
    u8 state0;
    u8 state1;
    u8 state2;
    u8 state3;
    f32 edge0;
    f32 edge1;
    f32 halfWidth;

    blocked = 0;
    state0 = gZScanSets[camera->CNumber].scan[0].state;
    state1 = gZScanSets[camera->CNumber].scan[1].state;
    state2 = gZScanSets[camera->CNumber].scan[2].state;
    state3 = gZScanSets[camera->CNumber].scan[3].state;
    edge0 = gZScanSets[camera->CNumber].scan[0].offset + 1.0f;
    edge1 = gZScanSets[camera->CNumber].scan[1].offset + 1.0f;
    if (state0 == 2 && state1 != 2) {
        gZScanSets[camera->CNumber].lastBlockedScan = 0;
    } else if (state0 != 2 && state1 == 2) {
        gZScanSets[camera->CNumber].lastBlockedScan = 1;
    }
    if (state2 == 2 && state3 != 2) {
        gZScanSets[camera->CNumber].lastBlockedScan = 2;
    } else if (state2 != 2 && state3 == 2) {
        gZScanSets[camera->CNumber].lastBlockedScan = 3;
    }
    if (state0 == 2 || state1 == 2) {
        halfWidth = gZScanParams[camera->CNumber][camera->DistIndex].halfWidth + 1;
        if (state0 == 2 && state1 == 2) {
            if (camera->CharPtr->inwater != 1 && camera->mv->death_State == 0) {
                camera->boxedInTimer -= gFrameTicks;
                if (camera->boxedInTimer < 0) {
                    camera->boxedInTimer = 0;
                }
            }
            if (camera->Mode != 0x80) {
                camera->SHitsPIPOutTimer = 30;
            }
            if (gZScanSets[camera->CNumber].scan[1].offset < gZScanSets[camera->CNumber].scan[0].offset) {
                *offset = halfWidth - edge0;
            } else if (gZScanSets[camera->CNumber].scan[0].offset < gZScanSets[camera->CNumber].scan[1].offset) {
                *offset = edge1 - halfWidth;
            } else if (gZScanSets[camera->CNumber].lastBlockedScan == 0) {
                *offset = halfWidth - edge0;
            } else {
                *offset = edge1 - halfWidth;
            }
        } else {
            camera->SHitsPIPOutTimer = 0;
            camera->boxedInTimer = 60;
            // matching: the unk23C tests below have identical branches in the original
            if (gZScanSets[camera->CNumber].scan[0].state == 2) {
                if (camera->Snap != 0) {
                    *offset = halfWidth - (gZScanSets[camera->CNumber].scan[0].offset + 1.0f);
                } else {
                    *offset = halfWidth - (gZScanSets[camera->CNumber].scan[0].offset + 1.0f);
                }
            } else {
                if (camera->Snap != 0) {
                    *offset = (gZScanSets[camera->CNumber].scan[1].offset + 1.0f) - halfWidth;
                } else {
                    *offset = (gZScanSets[camera->CNumber].scan[1].offset + 1.0f) - halfWidth;
                }
            }
        }
        {
        s32 pad; // matching: IDO reserves a stack slot for this block-scoped unused local
        blocked = 1;
        camera->SlackenK = 0.004999999888f;
        }
    } else {
        gZScanSets[camera->CNumber].scan[0].state = 4;
        gZScanSets[camera->CNumber].scan[1].state = 4;
        *offset -= *offset * 0.1000000015f;
        camera->boxedInTimer = 60;
    }
    if (camera->SHitsPIPOutTimer != 0) {
        camera->SHitsPIPOutTimer -= gFrameTicks;
        if (camera->SHitsPIPOutTimer < 0) {
            camera->SHitsPIPOutTimer = 0;
        }
        if (camera->SHitsPIPOutXZ == -1.0f) {
            camera->SHitsPIPOutXZ = camera->IdealTripodMagnitude;
            camera->SHitsPIPOutY = camera->height;
            camera->IdealTripodMagnitude = 150.0f;
            camera->height = 275.0f;
        }
    } else if (camera->SHitsPIPOutXZ != -1.0f) {
        camera->IdealTripodMagnitude = camera->SHitsPIPOutXZ;
        camera->height = camera->SHitsPIPOutY;
        camera->SHitsPIPOutXZ = -1.0f;
        camera->SHitsPIPOutY = -1.0f;
    }
    return blocked;
}

/* Picks the camera collision radii for the current mode, then moves a temporary probe object (type 0x2D)
 * from the camera position through the world and applies the corrected position. */
void camera_collide(Camera *camera) {
    s32 savedDD3;
    s32 flag;
    Actor probe;
    s32 hit;
    s32 saved2;
    s32 saved1;
    s32 savedDD4;
    f32 floorY;
    ActorCtl *ext;

    flag = 0;
    // matching: ext is loaded inside the condition, after the first flag test
    if (!(camera->AvailableModes & 0x40000) && (ext = camera->mv, ext->ladder == 0) && (camera->Mode != 0x40) &&
        !(camera->AvailableModes & 0x80) && !(camera->AvailableModes & 0x200)) {
        saved2 = gFloorProbeTypeEnabled[2];
        saved1 = gFloorProbeTypeEnabled[1];
        if (camera->SpecialFlags & 0x800) {
            camera->wallProbeWidthTarget = 20.0f;
            camera->wallProbeHeightTarget = 40.0f;
        } else if (camera->SpecialFlags & 0x10) {
            camera->wallProbeWidthTarget = 20.0f;
            camera->wallProbeHeightTarget = 72.0f;
        } else if (camera->IdealTripodMagnitude > 580.0f) {
            camera->wallProbeWidthTarget = 70.0f;
            camera->wallProbeHeightTarget = 140.0f;
        } else if ((level == LEVEL_W1_ROCK_SOLID) && (camera->CharContext == 3)) {
            flag = 1;
            camera->wallProbeWidthTarget = 20.0f;
            camera->wallProbeHeightTarget = 50.0f;
        } else {
            if (((camera->DoingAim != 0) && (camera->DoingAim != 1) && (camera->DoingAim != 3)) ||
                ((camera->CharPtr->unk102 != 0) &&
                 ((camera->CharPtr->xpos != camera->lastPlayerPos.x) || (camera->CharPtr->zpos != camera->lastPlayerPos.z))) ||
                (ext->unk1B3 != 0)) {
                camera->wallProbeWidthTarget = 20.0f;
                camera->wallProbeHeightTarget = 20.0f;
            } else if (camera->CharPtr->unk102 != 0) {
                flag = 1;
                camera->wallProbeWidthTarget = 25.0f;
                camera->wallProbeHeightTarget = 30.0f;
            } else {
                camera->wallProbeWidthTarget = 30.0f;
                camera->wallProbeHeightTarget = 60.0f;
            }
        }
        if (camera->Snap != 0) {
            camera->wallProbeWidthVel = 0.0f;
            camera->wallProbeWidth = camera->wallProbeWidthTarget;
            camera->wallProbeHeightVel = 0.0f;
            camera->wallProbeHeight = camera->wallProbeHeightTarget;
        } else {
            math_springTo(&camera->wallProbeWidth, camera->wallProbeWidthTarget, &camera->wallProbeWidthVel, 4.0f, 6.0f, camera->SpringTime);
            math_springTo(&camera->wallProbeHeight, camera->wallProbeHeightTarget, &camera->wallProbeHeightVel, 4.0f, 6.0f, camera->SpringTime);
        }
        probe.type = 0x2D;
        probe.CameraPtr = camera;
        probe.xpos = camera->eye.x;
        if (camera->SpecialFlags & 0x800) {
            probe.ypos = camera->eye.y + camera->YOffset;
        } else {
            probe.ypos = camera->eye.y;
        }
        probe.zpos = camera->eye.z;
        probe.floorTriVerts = camera->TrianglePtr;
        probe.shad = probe.ypos - camera->baseY;
        probe.floorY = camera->baseY;
        probe.yspd = 0.0f;
        probe.theta = camera->CameraAngle;
        if ((camera->AvailableModes & 0x80000000) || (level == LEVEL_W1_U_BEND)) {
            gFloorProbeTypeEnabled[2] = 0;
        }
        if (camera->AvailableModes & 0x10000) {
            gFloorProbeTypeEnabled[1] = 0;
        }
        gWallProbeIsCamera = 1;
        savedDD3 = D_800CBDD3;
        if (camera->CharPtr->unk102 != 0) {
            D_800CBDD3 = 1;
        } else {
            D_800CBDD3 = 0;
        }
        savedDD4 = D_800CBDD4;
        if ((camera->Snap != 0) || ((camera->Mode == 0x100) && (flag == 0))) {
            D_800CBDD4 = 1;
        } else {
            D_800CBDD4 = 0;
        }
        hit = collision_resolveActorWalls(camera->prevEye.x, camera->prevEye.y, camera->prevEye.z, &probe, 0, 0);
        D_800CBDD4 = savedDD4;
        gWallProbeIsCamera = 0;
        D_800CBDD3 = savedDD3;
        gFloorProbeTypeEnabled[2] = saved2;
        gFloorProbeTypeEnabled[1] = saved1;
        if (hit != 0) {
            camera->TonyHitsClear = 0;
            camera->SlackenK = 0.004999999888f;
        } else {
            camera->TonyHitsClear = 1;
            camera->SlackenK += (1.0f - camera->SlackenK) * 0.1000000015f;
        }
        camera->eye.x = probe.xpos;
        if (camera->Mode != 0x100) {
            if (camera->SpecialFlags & 0x800) {
                camera->eye.y = probe.ypos - camera->YOffset;
            } else {
                camera->eye.y = probe.ypos;
            }
        }
        camera->eye.z = probe.zpos;
        if ((level != LEVEL_W2_LIFTTUNNEL) && (hit != 0) && (camera->Mode != 0x100)) {
            collision_findFloorEx(NULL, NULL, &floorY, NULL, NULL, 0, camera->eye.x, camera->eye.y, camera->eye.z,
                          camera->prevEye.y, 0, 0, -10000.0f, camera->eye.y, 1);
            if (camera->eye.y < floorY + 40.0f) {
                s32 pad; // matching: IDO reserves a stack slot for this block-scoped unused local
                camera->eye.y = floorY + 40.0f;
            }
        }
    } else {
        camera->TonyHitsClear = 1;
    }
    camera_updateStatus(camera);
}

/* Measures the horizontal distance to the target; when too close, swings the yaw by 90 degrees. */
void camera_swingIfTooClose(Camera *camera) {
    f32 side;
    f32 dx;
    f32 dz;
    f32 dist;

    if (camera->CharRelAngle < 180.0f) {
        side = 2.0f;
    } else {
        side = -2.0f;
    }

    if ((camera->Mode != 0x80) && (camera->Mode != 0x40)) {
        dx = camera->eye.x - camera->target.x;
        dz = camera->eye.z - camera->target.z;
        dist = sqrtf((dx * dx) + (dz * dz));
        camera->TripodMagnitude = dist;

        if (dist < 42.0f) {
            camera->LockedCameraAngle = camera->CameraAngle - (45.0f * side);
            camera->LockedCamAngleK = 0.0f;
            camera_updateFollow(camera);
            camera->SpecialMove |= 2;
        }
    }

    camera_blendYaw(camera);
}

/* Advances the blend factor unk5CC towards 1, easing the yaw unk384 towards unk37C until it gets there. */
void camera_blendYaw(Camera *camera) {
    camera->LockedCamAngleK += 0.006000000052f;
    camera->LockedCamAngleK = (camera->LockedCamAngleK > 1.0f) ? 1.0f : camera->LockedCamAngleK;

    if (camera->LockedCamAngleK == 1.0f) {
        camera->LockedCameraAngle = 999.0f;
        camera->SpecialMove &= ~2;
    } else {
        camera->LockedCameraAngle += math_angleDiff(camera->LockedCameraAngle, camera->CameraAngle) * camera->LockedCamAngleK;
    }
}

void func_1512C200(Camera *camera) {
}

#define SQ(x) ((x) * (x))

/* Keeps the camera outside the circle of radius avoidRadius around avoidCenter (horizontally), pushing it out to the edge. */
void camera_keepOutsideCircle(Camera *camera) {
    f32 dz; // matching: also holds the target distance later (a separate local grows the frame)
    Vec3f pos;
    Vec3f offset;
    Vec3f toPos;

    if (camera->avoidActive) {
        f32 dx; // matching: also holds the yaw difference later

        dx = camera->eye.x - camera->avoidCenter.x;
        dz = camera->eye.z - camera->avoidCenter.z;
        pos = camera->eye;
        if (sqrtf(SQ(dx) + SQ(dz)) < camera->avoidRadius) {
            Vec3f target;
            f32 outwardYaw;
            f32 yaw;

            target = camera->focus;
            target.y = 0.0f;
            pos.y = 0.0f;
            camera->avoidCenter.y = 0.0f;
            math_dirTowards(&camera->avoidCenter, &pos, &offset);
            outwardYaw = math_vec3Heading(&offset) - 180.0f;
            math_normalizeDegrees(&outwardYaw);
            math_dirTowards(&target, &pos, &toPos);
            math_vec3Scale(&offset, camera->avoidRadius, &offset);
            pos.x = camera->avoidCenter.x + offset.x;
            pos.y = 0.0f;
            pos.z = camera->avoidCenter.z + offset.z;
            dz = sqrtf(SQ(target.x - pos.x) + SQ(target.z - pos.z));
            math_vec3Diff(&pos, &target, &offset);
            yaw = math_vec3Heading(&offset);
            math_normalizeDegrees(&yaw);
            camera->avoidYaw = yaw;
            dx = math_angleDiff(outwardYaw, yaw);
            if (fabsf(dx) < 35.0f) {
                if (dx <= 0.0f) {
                    dx = 5.0f;
                } else {
                    dx = -5.0f;
                }
                math_rotateAroundY(&pos, &camera->avoidCenter, dx, &pos);
            }
            gCameraAvoidTargetDist = dz;
            pos.y = camera->eye.y;
            camera->eye = pos;
        } else {
            gCameraAvoidTargetDist = 0.0f;
            camera->avoidYaw = camera->CameraAngle;
        }
    } else {
        gCameraAvoidTargetDist = 0.0f;
        camera->avoidYaw = 0.0f;
    }
    camera->avoidActive = 0;
}

s32 func_1512C47C(s32 arg0) {
    return 1;
}
