#ifndef CONKER_ZONE_H
#define CONKER_ZONE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "zone": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_32720.c
 *   game_357F0.c
 *   game_44C40.c
 *   game_45880.s
 *   game_6CCB0.c
 *   game_CDE80.c
 */

/* game/done/game_32720.c */
void func_15005270(void);

/* game_357F0.c */
void func_15008340(TriggerZone *region, f32 *x, f32 *z, f32 *top, f32 *bottom);

/* game_44C40.c */
void func_15017790(void);
void func_150177F8(void);
void func_15017868(void);
void func_15017930(s32 arg0);
void func_15017B20(TriggerZone *objects, s32 count);
void func_15017FA4(TriggerZone *obj, ZoneBounds *bounds);

/* game_45880.s: builds the zone grid from a bitmap with rows of stride bytes */
void func_150183D0(s32 stride, s32 height, u8 *bitmap, s32 *cells);

/* game_6CCB0.c */
s32 func_1503F800(void *unused, s16 x, s16 z, s32 bit, s32 unused2);
s32 func_1503F904(Actor *obj, s32 bit, s32 unused);
void func_1503F964(void);
extern u8 D_800C67F0; // nonzero: func_1503F964 cycles the zone-object cursor
extern u8 D_800C67F1; // zone-object cursor: index into movement_obj (0..24)

/* game_CDE80.c */
void func_150A09D0(s32 ticks);
void zone_cacheAngles(TriggerZone *zone);
void func_150A0D8C(void);
s32 func_150A1040(s32 value);
void func_150A104C(Camera *cam, TriggerZone **zoneList, s32 count);
void func_150A11C4(Actor *actor);
s32 zone_isOutside(Actor *actor, TriggerZone *zone, s32 withMargin);
void func_150A23E4(TriggerZone *zone);
void func_150A24C0(TriggerZone *zone, s32 unk127);
void func_150A25D4(TriggerZone *zone, Actor *actor, s32 unk127, s64 prevBits);
void func_150A278C(Actor *actor);
void func_150A2864(u32 id, s32 disabled);
void func_150A2940(s32 id, s32 data1, s32 subTypeData);
s32 func_150A29C8(u32 actor, u32 id);
/* (u32 actor, s32 count, s32 *ids); declared without a prototype: two callers also pass the Actor */
s32 func_150A2AEC();
s32 func_150A2CA4(u32 id);
s32 func_150A2D84(s32 camIndex, s32 id);
s32 func_150A2E4C(TriggerZone *zone, f32 x, f32 y, f32 z);
s32 func_150A2EE4(s32 actor, s32 id);
s32 func_150A2FA4(s32 camIndex, s32 id);
s32 func_150A3058(s32 subtype, s16 x, s16 y, s16 z);
TriggerZone *zone_findAt(s32 type, s32 subtype, s32 x, s32 y, s32 z);
s32 func_150A32B4(TriggerZone *zone, s32 x, s32 y, s32 z);
void func_150A3330(s32 id, s32 *subTypeData, s32 *data1, s32 *data2, s32 *data3);
void func_150A3398(s32 id, u16 mask, s32 subTypeData, s32 data1, s32 data2, s32 data3);
void func_150A3444(s32 id, s16 x, s16 y, s16 z);
s32 func_150A34B0(TriggerZone *cyl, Vec3f *p0, Vec3f *p1, Vec3f *hit);
s32 func_150A3504(TriggerZone *cyl, Vec3f *p0, Vec3f *p1, Vec3f *hit);
extern u32 gZoneCount;
extern TriggerZone *zones;
extern ZoneBounds *D_800D309C;
extern u64 D_800D30A0[3];
extern u64 D_800D30B8[3];
extern u16 D_800D30D0[16];
extern u64 D_800D30F0[16][3];
extern TriggerZone **D_800D3270;
extern s32 D_800D3274;

#endif
