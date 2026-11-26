#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c). */
u8 D_800D3090;                // timer of the ZONE_TYPE_EFFECT subtype-6 zone
u32 gZoneCount;               // number of TriggerZones in zones
TriggerZone *zones;           // level placement / marker table (0x34-byte records)
ZoneBounds *D_800D309C;       // bounds of each trigger zone (zones)
u64 D_800D30A0[3];            // TriggerZones that are always tested
u64 D_800D30B8[3];            // default candidate TriggerZones
u16 D_800D30D0[16];           // per actor: 1 + index of the last zone box it was found in, 0 for none
u64 D_800D30F0[16][3];        // per actor: TriggerZones it was inside last time
TriggerZone **D_800D3270;     // the ZONE_TYPE_CHAR_FLAGS zones with subTypeData 0, 2 or 6
s32 D_800D3274;               // number of entries in D_800D3270
static s32 D_800D3278;        // cleared by func_150A11C4, otherwise unused
static u8 D_800D3280[0x80];   /* not referenced; neither this build nor the October 2000 debug build reads it */

#include "functions.h"
#include "variables.h"



/* Per-frame update of the zone state that does not depend on an actor (ticks is unused). */
void func_150A09D0(s32 ticks) {
    u32 countdown;
    s32 i;

    for (i = 0; i < gZoneCount; i++) {
        switch (ZONE_TYPE(&zones[i])) {
        case ZONE_TYPE_SOUND: /* sound zone: data3 is 1 while occupied (func_150A23E4 sets it each time); a zone left
                   unoccupied for a whole update gets its looping sound stopped */
            if (zones[i].data3 == 1) {
                zones[i].data3 ^= 3;
            } else if (zones[i].data3 == 2) {
                func_1000FC18(zones[i].data2 >> 16,
                              zones[i].x,
                              zones[i].y,
                              zones[i].z,
                              zones[i].data1);
                zones[i].data3 = 0;
            }
            if (D_800C35EA[0] == 1) {
                if (zone_isOutside(movement_obj, &zones[i], 0) == 0) {
                    func_150A23E4(&zones[i]);
                }
            }
            break;
        case ZONE_TYPE_TUNE_CHANNEL: /* music-channel zone (music_fadeChannelVolume sets channel volumes) */
            if (ZONE_AREA_TYPE(&zones[i]) == ZONE_AREA_GATE) {
                zones[i].data3 = 0;
            } else {
                if (zones[i].data3 == 1) {
                    zones[i].data3 = 2;
                } else if (zones[i].data3 == 2) {
                    if (music_fadeChannelVolume(zones[i].subTypeData, 0, zones[i].data1, 0)) {
                        if (music_fadeChannelVolume(zones[i].subTypeData, 100, zones[i].data2, 0)) {
                            zones[i].data3 = 0;
                        }
                    }
                }
            }
            break;
        case ZONE_TYPE_MINI_SHAPE:
            if (zones[i].subTypeData != 9) {
                /* count down the respawn delay of the object func_150A24C0 spawned */
                countdown = zones[i].data2 & 0xFF;
                if (countdown != 0) {
                    if (!zones[i].inactive) {
                        countdown--;
                    } else {
                        countdown = 0;
                    }
                    zones[i].data2 = (zones[i].data2 & ~0xFF) | countdown;
                    if (countdown == 0) {
                        countdown = zones[i].data2;
                        D_800DDF5C[(countdown >> 8) & 0xFF] = 0;
                        zones[i].data2 &= 0xFFFF0000;
                    }
                }
            }
            /* fallthrough */
        case ZONE_TYPE_CAMERA:
            if (zones[i].subTypeData == 12) {
                if (!zones[i].inactive) {
                    s32 j;

                    for (j = 0; j <= gScreenLayout; j++) {
                        Camera *cam = &camera_global_Cameras[j];

                        cam->unk93C.x = zones[i].x;
                        cam->unk93C.y = zones[i].y;
                        cam->unk93C.z = zones[i].z;
                        cam->unk938 = zones[i].xSize;
                        cam->unk949 = zones[i].data1;
                        cam->unk948 = 1;
                    }
                }
            }
            break;
        }
    }
}

/* Caches the sine and cosine of the zone's rotation angles. */
void zone_cacheAngles(TriggerZone *zone) {
    f32 angle;

    angle = zone->pitch * 0.01745329238f; /* degrees to radians */
    zone->sinPitch = func_150AD78C(angle);
    zone->cosPitch = func_150AD780(angle);
    angle = zone->yaw * 0.01745329238f;
    zone->sinYaw = func_150AD78C(angle);
    zone->cosYaw = func_150AD780(angle);
}

s32 math_distance3D(s32 dx, s32 dy, s32 dz); /* game_DAE50.s */

/* Starts the ZONE_TYPE_REFLECTION zone effect (model_queueReflection) for every active actor inside such a zone. */
void func_150A0D8C(void) {
    u32 i;
    s32 j;
    Actor *obj;
    s16 dy;
    f32 fade;

    for (i = 0; i < gZoneCount; i++) {
        if (!zones[i].inactive && ZONE_TYPE(&zones[i]) == ZONE_TYPE_REFLECTION) {
            for (j = 0; j < 25; j++) {
                obj = &movement_obj[j];
                if (obj->type == 0) continue;
                if (obj->drawtype != 0 && obj->drawtype != 1) continue;
                if (zone_isOutside(obj, &zones[i], 1) != 0) continue;

                dy = obj->ypos - zones[i].y;
                if (zones[i].subTypeData == 0) {
                    if (math_distance3D((s16)(obj->xpos - zones[i].x), dy,
                                      (s16)(obj->zpos - zones[i].z)) < zones[i].xSize) {
                        fade = zones[i].xSize * 0.2f;
                        if (fade > 100.0f) {
                            fade = 100.0f;
                        }
                        model_queueReflection(2, j, zones[i].x, zones[i].y, zones[i].z, 0.0f,
                                      zones[i].yaw, zones[i].xSize - fade, zones[i].xSize,
                                      255.0f);
                    }
                } else {
                    model_queueReflection(1, j, obj->xpos, obj->floorY, obj->zpos,
                                  0.0f, 0.0f, 100.0f, 150.0f, 255.0f);
                }
            }
        }
    }
}

/* Offsets a value by 0x400; used to hide the cartridge-check address and value in func_150A11C4. */
s32 func_150A1040(s32 value) {
    return value + 0x400;
}

/* Tests the camera position against the given TriggerZones and applies the ZONE_TYPE_CHAR_FLAGS ones (camera floor height and flag 0x40000). */
void func_150A104C(Camera *cam, TriggerZone **zoneList, s32 count) {
    Actor probe;

    probe.xpos = cam->eye.x;
    probe.ypos = cam->eye.y;
    probe.zpos = cam->eye.z;
    probe.floorY = probe.ypos;
    probe.old_xpos = cam->prevEye.x;
    probe.old_ypos = cam->prevEye.y;
    probe.old_zpos = cam->prevEye.z;
    cam->zoneWaterY = -10000.0f;
    cam->AvailableModes &= ~0x40000;
    while (count != 0) {
        if (!zoneList) { // matching: this empty test steers IDO's callee-saved register order (mask in s3)
        }
        if (*zoneList != NULL && zone_isOutside(&probe, *zoneList, 0) == 0) {
            if (ZONE_TYPE(*zoneList) == ZONE_TYPE_CHAR_FLAGS) {
                switch ((*zoneList)->subTypeData) {
                case 2:
                    cam->zoneWaterY = (*zoneList)->y + (*zoneList)->ySize;
                    break;
                case 0:
                    cam->AvailableModes |= 0x40000;
                    break;
                case 6:
                    cam->AvailableModes &= ~0x40000;
                    break;
                }
            }
        }
        count--;
        zoneList++;
    }
}

#define BOX_CONTAINS(b, px, py, pz) ((b)->minX < (px) && (px) < (b)->maxX && (b)->minY < (py) && \
                                     (py) < (b)->maxY && (b)->minZ < (pz) && (pz) < (b)->maxZ)

s32 __osPiGetAccess(void);
void __osPiRelAccess(void);
s32 osPiRawReadIo(u32, u32 *);

/* Finds the TriggerZones that contain the actor (narrowed through the zone bounding boxes) and applies them. */
void func_150A11C4(Actor *actor) {
    u32 i;
    u32 base;
    u64 *word;
    u64 *prev; // matching: also holds the actor's y (s16) until the box scan (one variable, one register)
    u32 bestUnk18;
    s32 unk127;
    s32 x;
    s32 y; // matching: unused (y lives in prev) but it shifts the -g3 stack frame
    s16 z;
    Camera *cam;
    ZoneBounds *box;
    u64 mask[3];
    s32 found;
    s32 k;
    u64 bits;
    u64 prevBits;
    s32 idx;

    bestUnk18 = 1000;
    unk127 = actor->playernumber;
    cam = actor->CameraPtr;
    if (cam == NULL) {
        unk127 = 0xFF;
    } else if (D_800D3274 != 0) {
        func_150A104C(cam, D_800D3270, D_800D3274);
    }
    x = (s16)actor->xpos;
    prev = (u64 *)(s16)actor->ypos;
    z = actor->zpos;
    if (actor->playernumber != 0xFF) {
        text_clearObjectText(actor->playernumber);
    }
    func_150829D8(actor);
    actor->unk120 = -10000;
    D_800D3278 = 0;
    idx = actor - movement_obj;
    found = 0;
    if (idx < 16) {
        if (D_800D30D0[idx] > 0) {
            box = &D_800D309C[D_800D30D0[idx] - 1];
            if (BOX_CONTAINS(box, x, (s32)prev, z)) {
                for (i = 0; i < 3; i++) {
                    mask[i] = box->overlaps[i];
                }
                found = 1;
            }
        }
        D_800D30D0[idx] = 0;
    }
    if (!found) {
        for (i = 0; i < 3; i++) {
            mask[i] = D_800D30B8[i];
        }
    }

    /* Keep only the candidates whose box contains the actor, narrowed by each box's overlap set. */
    word = mask;
    bits = *word;
    for (i = 0, base = 0; i < gZoneCount; i++) {
        if (bits == 0) {
            base += 64;
            i = base;
            bits = *++word;
            if (i >= gZoneCount) {
                continue;
            }
        }
        if (bits & 1) {
            box = &D_800D309C[i];
            if (BOX_CONTAINS(box, x, (s32)prev, z)) {
                for (k = 0; k < 3; k++) {
                    mask[k] &= box->overlaps[k];
                }
                if (idx < 16 && box->numOverlaps < bestUnk18) {
                    D_800D30D0[idx] = i + 1;
                    bestUnk18 = box->numOverlaps;
                }
            } else {
                *word ^= 1LL << (i - base);
            }
        }
        bits >>= 1;
    }

    /* Apply every candidate zone the actor is inside; prevBits bit 0 says it was inside last time. */
    word = mask;
    base = 0;
    for (i = 0; i < 3; i++) {
        mask[i] |= D_800D30A0[i];
    }
    if (idx < 16) {
        prev = D_800D30F0[idx];
        prevBits = *prev;
        *prev = 0;
    } else {
        prev = NULL;
    }
    bits = *word;
    for (i = 0; i < gZoneCount; i++) {
        if (bits == 0) {
            base += 64;
            i = base;
            if (i >= gZoneCount) {
                continue;
            }
            bits = *++word;
            if (prev != NULL) {
                prev++;
                prevBits = *prev;
                *prev = 0;
            }
        }
        if (bits & 1) {
            if (zone_isOutside(actor, &zones[i], 0) == 0) {
                switch (ZONE_TYPE(&zones[i])) {
                case ZONE_TYPE_CAMERA:
                    if (cam != NULL && zones[i].subTypeData != 12) {
                        func_1512D560(cam, 1, &zones[i]);
                        cam->cmd.id = i;
                    }
                    break;
                case ZONE_TYPE_CHAR_FLAGS:
                    switch (zones[i].subTypeData) {
                    case 0:
                        actor->noshove &= ~4;
                        break;
                    case 1:
                        actor->noshove &= ~2;
                        break;
                    case 2:
                        actor->unk120 = zones[i].y + zones[i].ySize;
                        break;
                    case 3:
                        actor->noshove |= 2;
                        break;
                    case 4:
                        func_15055C88(actor);
                        break;
                    case 5: {
                        u16 angle;
                        s32 flag;
                        f32 height;

                        /* yaw * 65536/360 is a binary angle */
                        if (actor->mv->ladder == 0 && actor->stunned == 0 && actor->energy != 0 &&
                            actor->norun == 0 &&
                            (actor->mv->weapon_status == 0 || level == 6) && actor->stickspeed > 8.0f) {
                            height = zones[i].y + zones[i].ySize;
                            flag = 0;
                            angle = (u32)(zones[i].yaw * 182.0444489f);
                            angle -= actor->mv->press_angle;
                            if (height < actor->ypos + 250.0f && actor->shad == 0.0f) {
                                angle -= 0x8000;
                                flag = 1;
                            }
                            if (angle > 0x2000 && angle < 0x6000) {
                                actor->mv->ladderFromTop = flag;
                                actor->mv->unk96 = 0;
                                actor->nojump = 18;
                                actor->mv->ladder = i + 1;
                                if (zones[i].data1 != 0) {
                                    actor->mv->ladder |= 0x80;
                                }
                            }
                        }
                    }
                        break;
                    case 6:
                        actor->noshove |= 4;
                        break;
                    case 7:
                        actor->mvmt_control |= 0x100000;
                        break;
                    case 8:
                        if (zones[i].data1 != 0) {
                            D_800DF0E1 |= 1 << idx;
                        } else {
                            D_800DF0E0 |= 1 << idx;
                        }
                        break;
                    case 9:
                        actor->noshove |= 0x8000000;
                        break;
                    }
                    break;
                case ZONE_TYPE_SOUND:
                    func_150A23E4(&zones[i]);
                    break;
                case ZONE_TYPE_TUNE_CHANNEL:
                    if (zones[i].data3 == 0) {
                        if (music_fadeChannelVolume(zones[i].subTypeData, 100, zones[i].data1, 0)) {
                            if (music_fadeChannelVolume(zones[i].subTypeData, 0, zones[i].data2, 0)) {
                                zones[i].data3 = 1;
                            }
                        }
                    } else {
                        zones[i].data3 = 1;
                    }
                    break;
                case ZONE_TYPE_TUNE:
                    if (unk127 != 0xFF && !(prevBits & 1)) {
                        music_playTune(zones[i].subTypeData, zones[i].data1, zones[i].data2 & 0xFF);
                        if (zones[i].subTypeData) {
                            music_fadeChannelVolume(zones[i].subTypeData, 0, zones[i].data3 & 0xFFFF, 0);
                        }
                    }
                    break;
                case ZONE_TYPE_AUDIO_CONTROL:
                    if (zones[i].subTypeData == 0) {
                        D_80041FD8 = zones[i].data1;
                    }
                    break;
                case ZONE_TYPE_MINI_SHAPE:
                    func_150A24C0(&zones[i], unk127);
                    break;
                case ZONE_TYPE_WARP: {
                    u32 raw;

                    /* Cartridge check: the ROM word at 0xB0000454 must be 0x1090827. */
                    __osPiGetAccess();
                    osPiRawReadIo(func_150A1040(0xB0000054), &raw);
                    __osPiRelAccess();
                    if (func_150A1040(raw) == 0x1090C2B && D_800C35EA[0] == 0 && actor->energy != 0) {
                        player_startWarp(actor, zones[i].subTypeData, zones[i].data1);
                        if (level != zones[i].subTypeData) {
                            actor->imune = 0xFF;
                            actor->unk1CC = actor->ypos;
                        }
                    }
                }
                    break;
                case ZONE_TYPE_CONTEXT:
                    text_queueObjectText(actor, &zones[i], 1, zones[i].subTypeData, zones[i].data1);
                    break;
                case ZONE_TYPE_EFFECT:
                    func_150A25D4(&zones[i], actor, unk127, prevBits);
                    break;
                case ZONE_TYPE_NEUTRAL:
                    if (zones[i].subTypeData == 5) {
                        func_151AE06C(actor, &zones[i]);
                    }
                    break;
                case ZONE_TYPE_CUTSCENE:
                    if (unk127 != 0xFF && !(prevBits & 1) && gFrontEndState == 0 && actor->energy != 0 &&
                        D_800C35EA[0] == 0) {
                        if (zones[i].subTypeData == 0) {
                            zones[i].inactive = 1;
                        }
                        func_1501D348(level, zones[i].data1, 0, 0, 0);
                        zones[i].data2++;
                    }
                    break;
                }
                if (prev != NULL) {
                    *prev |= 1LL << (i - base);
                }
            }
        }
        bits >>= 1;
        prevBits >>= 1;
    }
}

/*
 * Returns 0 if the actor is inside the zone, 1 if not (or the zone is inactive).  withMargin widens the
 * zone by the actor's unkE4.  Shapes: 0 cylinder (optionally rotated), 1 sphere, 2 rotated box,
 * 3 rotated gate crossed by the move from the old to the current position.
 */
s32 zone_isOutside(Actor *actor, TriggerZone *zone, s32 withMargin) {
    s16 dx;
    s16 dy;
    s16 dz;
    s16 margin;
    s16 odx;
    s16 ody;
    s16 odz;
    s16 w;
    f32 c;
    f32 s;
    f32 a;
    f32 b;
    f32 r;
    f32 a2;
    f32 b2;

    if (zone->inactive == 1 || actor == NULL || zone == NULL) {
        return 1;
    }
    dx = actor->xpos - zone->x;
    dy = actor->ypos - zone->y;
    dz = actor->zpos - zone->z;
    margin = withMargin ? actor->bodyRadius : 0;
    if (!(zone->flags & 1) || ((f32)(s32)actor->ypos - actor->floorY <= 5.0f)) {
        switch (ZONE_AREA_TYPE(zone)) {
        case ZONE_AREA_CYLINDER:
            if (0.01f < zone->pitch || 0.01f < zone->yaw) {
                s = zone->sinYaw;
                c = zone->cosYaw;
                a = dx;
                b = dz;
                dx = a * c - b * s;
                r = b * c + a * s;
                c = zone->cosPitch;
                s = zone->sinPitch;
                a = dy;
                dz = r * c - a * s;
                dy = a * c + r * s;
            }
            if (func_150AD960(dx, dz, 0, 0) < zone->xSize + margin && dy > 0 && dy < zone->ySize) {
                return 0;
            }
            break;
        case ZONE_AREA_SPHERE:
            if (math_distance3D(dx, dy, dz) < zone->xSize + margin) {
                return 0;
            }
            break;
        case ZONE_AREA_BOX:
            c = zone->cosYaw;
            s = zone->sinYaw;
            a = dx;
            b = dz;
            dx = a * c - b * s;
            if (dx < zone->xSize + margin && -zone->xSize - margin < dx) {
                r = b * c + a * s;
                c = zone->cosPitch;
                s = zone->sinPitch;
                a = dy;
                w = r * c - a * s;
                if (w < zone->zSize + margin && -zone->zSize - margin < w) {
                    dy = a * c + r * s;
                    if (dy < zone->ySize && dy > 0) {
                        return 0;
                    }
                }
            }
            break;
        case ZONE_AREA_GATE:
            odx = actor->old_xpos - zone->x;
            ody = actor->old_ypos - zone->y;
            odz = actor->old_zpos - zone->z;
            c = zone->cosYaw;
            s = zone->sinYaw;
            a = dx;
            b = dz;
            dx = a * c - b * s;
            a2 = odx;
            b2 = odz;
            odx = a2 * c - b2 * s;
            b = b * c + a * s;
            b2 = b2 * c + a2 * s;
            c = zone->cosPitch;
            s = zone->sinPitch;
            a = dy;
            dz = b * c - a * s;
            a2 = ody;
            odz = b2 * c - a2 * s;
            /* crossed from behind the gate plane to in front of it */
            if (odz < 0 && dz >= 0) {
                c = zone->cosPitch;
                s = zone->sinPitch;
                dy = a * c + b * s;
                b = (f32)odz / (odz - dz);
                if (fabsf(odx + b * (dx - odx)) < zone->xSize) {
                    ody = a2 * c + b2 * s;
                    if (ody + b * (dy - ody) >= 0.0f && ody + b * (dy - ody) < zone->ySize) {
                        return 0;
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    return 1;
}

/* Type-2 zone occupied: starts its looping positional sound (sound_addDynamicSound) if it is not playing yet. */
void func_150A23E4(TriggerZone *zone) {
    s32 flags;
    s32 farFlag;

    if (zone->data3 == 0) {
        farFlag = 0;
        flags = 8;
        if (ZONE_AREA_TYPE(zone) == ZONE_AREA_CYLINDER) {
            farFlag = 0x8000;
        }
        if (zone->subTypeData >> 7) {
            flags = 0x108;
        }
        sound_addDynamicSound(zone->data2 >> 16, zone->x, zone->y, zone->z, (zone->subTypeData & 0x7F) * 327,
                      zone->data1 | farFlag, zone->data2 & 0xFFFF,
                      NULL, NULL, NULL, flags, 0);
        zone->data3 = 1;
    fence:; // matching: keeps as1 from moving the store above into the branch delay slot
    } else {
        zone->data3 = 1;
    }
}


/* A TriggerZone with data1 and data2 viewed as halves and bytes. */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 xSize;
    /* 0x08 */ u8  pad8[0xF];
    /* 0x17 */ u8  subTypeData;
    /* 0x18 */ s16 data1Hi;
    /* 0x1A */ u8  data1Byte2;
    /* 0x1B */ u8  data1Byte3;
    /* 0x1C */ u8  data2Byte0;
    /* 0x1D */ u8  data2Byte1;
    /* 0x1E */ u8  data2Byte2;
    /* 0x1F */ u8  data2Byte3;
    /* 0x20 */ u32 data3;
} TriggerZoneBytes;

/* ZONE_TYPE_MINI_SHAPE zone: spawns its object (func_15183290) once the countdown in the low byte of data2 is 0;
   while the zone stays occupied the countdown is held at 2 or more. */
void func_150A24C0(TriggerZone *zone, s32 unk127) {
    s32 handle;
    s32 countdown;
    s32 subtype;
    s32 data1Lo;
    s32 data1Hi;
    s32 data1Byte2;
    s32 data2Byte0;
    s32 data2Byte1;
    s32 data3;

    if (unk127 != 0xFF) {
        if ((zone->data2 & 0xFF) == 0 || zone->subTypeData == 9) {
            /* matching: each argument goes through its own local, in this order */
            subtype = zone->subTypeData;
            data1Lo = zone->data1 & 0xFF;
            data1Hi = ((TriggerZoneBytes *)zone)->data1Hi;
            data1Byte2 = ((TriggerZoneBytes *)zone)->data1Byte2;
            data2Byte0 = ((TriggerZoneBytes *)zone)->data2Byte0;
            data2Byte1 = ((TriggerZoneBytes *)zone)->data2Byte1;
            data3 = zone->data3;
            handle = func_15183290(zone->x, zone->y + data2Byte1 * 16, zone->z, data1Lo, subtype, zone->xSize, unk127,
                                   data1Hi, data1Byte2, data2Byte0, data3);
            if (handle != -1 && zone->subTypeData != 9) {
                zone->data2 = (handle << 8) | 0xF0 | (zone->data2 & 0xFFFF0000);
            }
        } else {
            countdown = zone->data2 & 0xFF;
            if (countdown < 2) {
                countdown = 2;
            }
            zone->data2 = (zone->data2 & ~0xFF) | countdown;
        }
    }
}

/* ZONE_TYPE_EFFECT zone: subTypeData-specific effect on the actor inside it; prevBits bit 0 is "was inside last time". */
void func_150A25D4(TriggerZone *zone, Actor *actor, s32 unk127, s64 prevBits) {
    switch (zone->subTypeData) {
    case 0:
        break;
    case 1:
        if (unk127 != 0xFF) {
            func_150E7290(unk127, 0xFF, 0);
        }
        break;
    case 2:
        if (unk127 != 0xFF) {
            /* colour: one byte each from data1, data2, data3 */
            D_800D9EB8[unk127][0] = zone->data1;
            D_800D9EB8[unk127][1] = zone->data2;
            D_800D9EB8[unk127][2] = zone->data3;
        }
        break;
    case 3:
        if (unk127 != 0xFF) {
            func_150E68B0(actor, unk127, zone, ((TriggerZoneBytes *)zone)->data1Byte3);
        }
        break;
    case 6: {
        u8 *timer = &D_800D3090;
        if (!(prevBits & 1)) {
            *timer = 30;
            sound_playActorSound1(0x66F, actor, 0x5DC0, 0x190, 0x1F4);
        }
        *timer += D_800BE9E4;
        if (*timer >= 31) {
            if (actor->unkB2 > 1440) {
                actor->unkB2 = 1200;
            } else if (actor->unkB2 < 241) {
                actor->unkB2 = 90;
            } else {
                actor->unkB2 -= 240;
            }
            *timer = 0;
        }
    }
        break;
    case 8:
        sight_notifyActorMoved(actor, zone);
        break;
    case 9:
        func_1515F270(actor, zone);
        break;
    case 10:
        func_151CE51C(actor, zone);
        break;
    }
}

/* Applies the ZONE_TYPE_WARP TriggerZones containing the actor (player_startWarp), unless its unk65 is set. */
void func_150A278C(Actor *actor) {
    s32 i;

    if (actor->attach_char != 0) {
        return;
    }
    for (i = 0; i < gZoneCount; i++) {
        if (ZONE_TYPE(&zones[i]) == ZONE_TYPE_WARP) {
            if (zone_isOutside(actor, &zones[i], 0) == 0) {
                player_startWarp(actor, zones[i].subTypeData, zones[i].data1);
            }
        }
    }
}

/* Enables (0) or disables (1) zone id; disabling also clears it from every actor's occupancy bits. */
void func_150A2864(u32 id, s32 disabled) {
    s32 i;

    id &= 0xFFF;
    if (id < gZoneCount) {
        zones[id].inactive = disabled;
    }
    if (disabled == 1) {
        for (i = 0; i < 16; i++) {
            D_800D30F0[i][id >> 6] &= ~(1ULL << (id & 0x3F));
        }
    }
}

/* Sets zone id's data1 and subTypeData (-1 leaves one unchanged) and clears its data2. */
void func_150A2940(s32 id, s32 data1, s32 subTypeData) {
    id &= 0xFFF;
    if (id < gZoneCount) {
        if (data1 != -1) {
            zones[id].data1 = data1;
        }
        if (subTypeData != -1) {
            zones[id].subTypeData = subTypeData;
        }
        zones[id].data2 = 0;
    }
}

/* Returns 0 if actor `actor` is inside zone id, else 1 (cached occupancy bits when they are current). */
s32 func_150A29C8(u32 actor, u32 id) {
    id &= 0xFFF;
    if (gZoneCount < id) {
        return 1;
    }
    if (actor < 16 && (gMatchState.playerMask & (1 << actor))) {
        if ((D_800D30F0[actor][id >> 6] >> (id & 0x3F)) & 1) {
            return 0;
        }
        return 1;
    }
    return zone_isOutside(&movement_obj[actor], &zones[id], 1);
}

/* Returns the first of the count zone ids in ids[] that contains actor `actor`, or -1. */
s32 func_150A2AEC(actor, count, ids)
    u32 actor;
    s32 count;
    s32 *ids;
{
    s32 i;
    u32 id;

    if (actor < 16 && (gMatchState.playerMask & (1 << actor))) {
        for (i = 0; i < count; i++) {
            id = ids[i];
            id &= 0xFFF;
            if (id < gZoneCount) {
                if ((D_800D30F0[actor][id >> 6] >> (id & 0x3F)) & 1) {
                    return id;
                }
            }
        }
    } else {
        for (i = 0; i < count; i++) {
            id = ids[i];
            id &= 0xFFF;
            if (id < gZoneCount) {
                if (zone_isOutside(&movement_obj[actor], &zones[id], 1) == 0) {
                    return id;
                }
            }
        }
    }
    return -1;
}

/* Returns the first actor whose cached occupancy bits say it is inside zone id, or -1. */
s32 func_150A2CA4(u32 id) {
    s32 i;

    id &= 0xFFF;
    if (gZoneCount < id) {
        return -1;
    }
    for (i = 0; i != 16; i++) {
        if (gMatchState.playerMask & (1 << i)) {
            if ((D_800D30F0[i][id >> 6] >> (id & 0x3F)) & 1) {
                return i;
            }
        }
    }
    return -1;
}

/* Returns 0 if camera `camIndex`'s position is inside zone id, else 1. */
s32 func_150A2D84(s32 camIndex, s32 id) {
    Camera *cam;
    Actor probe;

    cam = &camera_global_Cameras[camIndex];
    id &= 0xFFF;
    if (gZoneCount < id) {
        return 1;
    }
    probe.xpos = cam->eye.x;
    probe.ypos = cam->eye.y;
    probe.zpos = cam->eye.z;
    probe.floorY = probe.ypos;
    probe.old_xpos = cam->prevEye.x;
    probe.old_ypos = cam->prevEye.y;
    probe.old_zpos = cam->prevEye.z;
    return zone_isOutside(&probe, &zones[id], 0);
}

/* Returns 1 if the point is behind (or on) the zone's rotated plane, else 0.
 * matching: the dead u/z computations reproduce the original's register allocation
 * (u reserves $f16 so t lands in $f18) and block IDO's (a - b <= 0) -> (a <= b) fold. */
s32 func_150A2E4C(TriggerZone *zone, f32 x, f32 y, f32 z) {
    f32 s;
    f32 c;
    f32 t;
    f32 u;
    f32 w;

    x -= zone->x;
    y -= zone->y;
    z = y - zone->z;
    s = zone->sinYaw;
    c = zone->cosYaw;
    u = z * s;
    t = z * c + x * s;
    s = zone->sinPitch;
    c = zone->cosPitch;
    u = u + t;
    w = t * c - y * s;
    z = y * c + t * s;
    u = u * u;
    if (w <= 0.0f) {
        return 1;
    }
    return 0;
}

/* Returns whether actor `actor` is behind gate zone id (0 for a zone that is not a gate, 1 for bad ids). */
s32 func_150A2EE4(s32 actor, s32 id) {
    id &= 0xFFF;
    if (gZoneCount < id) {
        return 1;
    }
    if (ZONE_AREA_TYPE(&zones[id]) == ZONE_AREA_GATE) {
        return func_150A2E4C(&zones[id], movement_obj[actor].xpos, movement_obj[actor].ypos,
                             movement_obj[actor].zpos);
    }
    return 0;
}

/* As func_150A2EE4, for camera `camIndex`'s position. */
s32 func_150A2FA4(s32 camIndex, s32 id) {
    Camera *cam;

    cam = &camera_global_Cameras[camIndex];
    id &= 0xFFF;
    if (gZoneCount < id) {
        return 1;
    }
    if (ZONE_AREA_TYPE(&zones[id]) == ZONE_AREA_GATE) {
        return func_150A2E4C(&zones[id], cam->eye.x, cam->eye.y, cam->eye.z);
    }
    return 0;
}

/* Returns 1 if the point is inside an enabled ZONE_TYPE_NEUTRAL zone of the given subTypeData. */
s32 func_150A3058(s32 subtype, s16 x, s16 y, s16 z) {
    u8 pad[4]; // matching: unused local of the original (frame layout)
    Actor probe;
    s32 i;

    probe.xpos = x;
    probe.old_xpos = x;
    probe.ypos = y;
    probe.floorY = y;
    probe.old_ypos = y;
    probe.zpos = z;
    probe.old_zpos = z;

    for (i = 0; i < gZoneCount; i++) {
        if (!zones[i].inactive) {
            if (ZONE_TYPE(&zones[i]) == ZONE_TYPE_NEUTRAL) {
                if (zones[i].subTypeData == subtype) {
                    if (zone_isOutside(&probe, &zones[i], 0) == 0) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/* Returns the first enabled zone of the given type and subtype that contains the point, or NULL. */
TriggerZone *zone_findAt(s32 type, s32 subtype, s32 x, s32 y, s32 z) {
    s32 i;
    Actor probe;

    probe.xpos = x;
    probe.old_xpos = x;
    probe.ypos = y;
    probe.floorY = y;
    probe.old_ypos = y;
    probe.zpos = z;
    probe.old_zpos = z;

    for (i = 0; i < gZoneCount; i++) {
        if (!zones[i].inactive) {
            if (type == ZONE_TYPE(&zones[i])) {
                if (subtype == zones[i].subTypeData) {
                    if (zone_isOutside(&probe, &zones[i], 0) == 0) {
                        return &zones[i];
                    }
                }
            }
        }
    }
    return NULL;
}

/* Returns 1 if the point is inside the zone. */
s32 func_150A32B4(TriggerZone *zone, s32 x, s32 y, s32 z) {
    Actor probe;
    u8 pad[8]; // matching: unused local of the original (frame layout)

    probe.xpos = x;
    probe.old_xpos = x;
    probe.ypos = y;
    probe.floorY = y;
    probe.old_ypos = y;
    probe.zpos = z;
    probe.old_zpos = z;
    if (zone_isOutside(&probe, zone, 0) == 0) {
        return 1;
    }
    return 0;
}

/* Reads zone id's subTypeData and parameters. */
void func_150A3330(s32 id, s32 *subTypeData, s32 *data1, s32 *data2, s32 *data3) {
    *subTypeData = zones[id].subTypeData;
    *data1 = zones[id].data1;
    *data2 = zones[id].data2;
    *data3 = zones[id].data3;
}

/* Writes the zone id fields selected by mask (1: subTypeData, 2: data1, 4: data2, 8: data3). */
void func_150A3398(s32 id, u16 mask, s32 subTypeData, s32 data1, s32 data2, s32 data3) {
    if (mask & 1) {
        zones[id].subTypeData = subTypeData;
    }
    if (mask & 2) {
        zones[id].data1 = data1;
    }
    if (mask & 4) {
        zones[id].data2 = data2;
    }
    if (mask & 8) {
        zones[id].data3 = data3;
    }
}

/* Moves zone id to (x, y, z). */
void func_150A3444(s32 id, s16 x, s16 y, s16 z) {
    zones[id].x = x;
    zones[id].y = y;
    zones[id].z = z;
}

/* Tests the segment p0-p1 against cyl (only an active ZONE_AREA_CYLINDER zone);
   on a hit writes the entry point to hit and returns TRUE. */
s32 func_150A34B0(TriggerZone *cyl, Vec3f *p0, Vec3f *p1, Vec3f *hit) {
    if (cyl->inactive == 1) {
        return FALSE;
    }
    if (ZONE_AREA_TYPE(cyl) == ZONE_AREA_CYLINDER) {
        return func_150A3504(cyl, p0, p1, hit);
    }
    return FALSE;
}

/* Segment p0-p1 vs. cylinder: moves both points into the cylinder's local space, clips the
   segment to the height slab [0, height] and to the radius, and on overlap writes the first
   contact point (back in world space) to hit and returns TRUE.
   func_150AD78C / func_150AD780 are sin / cos of an angle in radians. */
s32 func_150A3504(TriggerZone *cyl, Vec3f *p0, Vec3f *p1, Vec3f *hit) {
    s32 i;
    f32 scale;
    f32 tSlabMin;
    f32 tSlabMax;
    f32 local[2][3];
    f32 s;
    f32 c;
    f32 ang;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dir[3];
    f32 org[3];
    f32 disc;
    f32 a;
    f32 sq;
    f32 tCylMin;
    f32 tCylMax;
    f32 b;
    f32 t1; // matching: the roots go through their own stack slots before lo/hi
    f32 t2;

    /* Rotate p0, then p1, into cylinder space (rotate by yaw, then pitch). */
    dx = p0->x - cyl->x;
    dy = p0->y - cyl->y;
    dz = p0->z - cyl->z;
    for (i = 0; i < 2; i++) {
        c = cyl->cosYaw;
        s = cyl->sinYaw;
        local[i][0] = dx * c - dz * s;
        dz = dz * c + dx * s;
        c = cyl->cosPitch;
        s = cyl->sinPitch;
        local[i][2] = dz * c - dy * s;
        local[i][1] = dy * c + dz * s;
        if (i == 0) {
            dx = p1->x - cyl->x;
            dy = p1->y - cyl->y;
            dz = p1->z - cyl->z;
        }
    }
    org[0] = local[0][0];
    org[2] = local[0][2];
    dir[0] = local[1][0] - local[0][0];
    dir[1] = local[1][1] - local[0][1];
    org[1] = local[0][1];
    dir[2] = local[1][2] - local[0][2];
    /* Range of t in [0, 1] where the segment lies within the height slab. */
    if (dir[1] != 0.0f) {
        f32 tmp;

        scale = 1.0f / dir[1];
        tSlabMin = -org[1] * scale;
        tSlabMax = (cyl->ySize - local[0][1]) * scale;
        if (tSlabMax < tSlabMin) {
            tmp = tSlabMin;
            tSlabMin = tSlabMax;
            tSlabMax = tmp;
        }
        if (tSlabMax < 0.0f || tSlabMin > 1.0f) {
            return FALSE;
        }
        if (tSlabMin < 0.0f) {
            tSlabMin = 0.0f;
        }
        if (tSlabMax > 1.0f) {
            tSlabMax = 1.0f;
        }
    } else {
        if (org[1] < 0.0f || cyl->ySize < local[0][1]) {
            return FALSE;
        }
        tSlabMin = 0.0f;
        tSlabMax = 1.0f;
    }
    /* Range of t in [0, 1] where the segment lies within the radius (quadratic in t). */
    a = dir[0] * dir[0] + dir[2] * dir[2];
    if (a == 0.0f) {
        return FALSE;
    }
    b = 2.0f * (dir[0] * org[0] + dir[2] * org[2]);
    disc = b * b - 4.0f * a * ((org[0] * org[0] + org[2] * org[2]) - cyl->xSize * cyl->xSize);
    if (disc < 0.0f) {
        return FALSE;
    }
    sq = sqrtf(disc);
    scale = 0.5f / a;
    t1 = (-b + sq) * scale;
    t2 = (-b - sq) * scale;
    tCylMin = t1;
    tCylMax = t2;
    if (tCylMax < tCylMin) {
        sq = tCylMin; // matching: the swap reuses sq rather than a fresh temp
        tCylMin = tCylMax;
        tCylMax = sq;
    }
    if (tCylMax < 0.0f || tCylMin > 1.0f) {
        return FALSE;
    }
    if (tCylMin < 0.0f) {
        tCylMin = 0.0f;
    }
    if (tCylMax > 1.0f) {
        tCylMax = 1.0f;
    }
    if (tCylMax < tSlabMin || tSlabMax < tCylMin) {
        return FALSE;
    }
    /* First t inside both ranges. */
    if (tCylMin < tSlabMin) {
        a = tSlabMin; // matching: reuses a for t; a separate local grows the frame
    } else {
        a = tCylMin;
    }
    {
        f32 hz;
        f32 hy;
        f32 hx;

        hx = org[0] + dir[0] * a;
        hy = org[1] + dir[1] * a;
        hz = org[2] + dir[2] * a;
        /* Rotate the contact point back to world space (by -pitch, then -yaw). */
        ang = -(cyl->pitch * 0.01745329238f);
        s = func_150AD78C(ang);
        c = func_150AD780(ang);
        hit->z = hz * c - hy * s;
        hit->y = hy * c + hz * s;
        ang = -(cyl->yaw * 0.01745329238f);
        s = func_150AD78C(ang);
        c = func_150AD780(ang);
        hit->x = hx * c - hit->z * s;
        hit->z = hit->z * c + hx * s;
        hit->x += cyl->x;
        hit->y += cyl->y;
        hit->z += cyl->z;
    }
    return TRUE;
}
