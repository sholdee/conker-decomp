#ifndef CONKER_ZONE_H
#define CONKER_ZONE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "zone": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Zone/ZoneCursor/zonecursor.c
 *   game/Zone/RandomPoint/randompoint.c
 *   game/Zone/ZoneBounds/zonebounds.c
 *   game_45880.s
 *   game/Zone/ZoneTree/zonetree.c
 *   game/Zone/ZoneState/zonestate.c
 */

/* game/Zone/ZoneCursor/zonecursor.c */
void zone_resetObjectCursor(void);

/* game/Zone/RandomPoint/randompoint.c */
void zone_getRandomPoint(TriggerZone *region, f32 *x, f32 *z, f32 *top, f32 *bottom);

/* game/Zone/ZoneBounds/zonebounds.c */
void zone_resetTaskProgress(void);
void zone_resetTaskWorkingSet(void);
void zone_reloadSectionTasks(void);
void zone_loadObjects(s32 arg0);
void zone_computeAllBounds(TriggerZone *objects, s32 count);
void zone_computeBounds(TriggerZone *obj, ZoneBounds *bounds);

/* game_45880.s: builds the zone grid from a bitmap with rows of stride bytes */
void func_150183D0(s32 stride, s32 height, u8 *bitmap, s32 *cells);

/* game/Zone/ZoneTree/zonetree.c */
s32 zone_isPointHiddenFromView(void *unused, s16 x, s16 z, s32 bit, s32 unused2);
s32 zone_isActorHiddenFromView(Actor *obj, s32 bit, s32 unused);
void zone_advanceObjectCursor(void);
extern u8 gZoneCursorEnabled; // nonzero: zone_advanceObjectCursor cycles the zone-object cursor
extern u8 gZoneObjectCursor; // zone-object cursor: index into movement_obj (0..24)

/* game/Zone/ZoneState/zonestate.c */
void zone_updateGlobal(s32 ticks);
void zone_cacheAngles(TriggerZone *zone);
void zone_updateReflections(void);
s32 zone_addCheckOffset(s32 value);
void zone_applyCameraZones(Camera *cam, TriggerZone **zoneList, s32 count);
void zone_updateActor(Actor *actor);
s32 zone_isOutside(Actor *actor, TriggerZone *zone, s32 withMargin);
void zone_startSound(TriggerZone *zone);
void zone_updateMiniShape(TriggerZone *zone, s32 unk127);
void zone_applyEffect(TriggerZone *zone, Actor *actor, s32 unk127, s64 prevBits);
void zone_applyWarps(Actor *actor);
void zone_setDisabled(u32 id, s32 disabled);
void zone_setData1(s32 id, s32 data1, s32 subTypeData);
s32 zone_isActorOutsideId(u32 actor, u32 id);
/* (u32 actor, s32 count, s32 *ids); declared without a prototype: two callers also pass the Actor */
s32 zone_findActorZone();
s32 zone_findActorInZone(u32 id);
s32 zone_isCameraOutsideId(s32 camIndex, s32 id);
s32 zone_isBehindPlane(TriggerZone *zone, f32 x, f32 y, f32 z);
s32 zone_isActorBehindGate(s32 actor, s32 id);
s32 zone_isCameraBehindGate(s32 camIndex, s32 id);
s32 zone_isInNeutralZone(s32 subtype, s16 x, s16 y, s16 z);
TriggerZone *zone_findAt(s32 type, s32 subtype, s32 x, s32 y, s32 z);
s32 zone_containsPoint(TriggerZone *zone, s32 x, s32 y, s32 z);
void zone_getParams(s32 id, s32 *subTypeData, s32 *data1, s32 *data2, s32 *data3);
void zone_setParams(s32 id, u16 mask, s32 subTypeData, s32 data1, s32 data2, s32 data3);
void zone_setPosition(s32 id, s16 x, s16 y, s16 z);
s32 zone_segmentHitsCylinder(TriggerZone *cyl, Vec3f *p0, Vec3f *p1, Vec3f *hit);
s32 zone_segmentCylinder(TriggerZone *cyl, Vec3f *p0, Vec3f *p1, Vec3f *hit);
extern u32 gZoneCount;
extern TriggerZone *zones;
extern ZoneBounds *gZoneBounds;
extern u64 gZonesAlwaysTested[3];
extern u64 gZonesCandidates[3];
extern u16 gActorLastZoneBox[16];
extern u64 gActorZoneBits[16][3];
extern TriggerZone **gCharFlagZones;
extern s32 gCharFlagZoneCount;

#endif
