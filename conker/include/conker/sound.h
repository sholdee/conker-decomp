#ifndef CONKER_SOUND_H
#define CONKER_SOUND_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "sound": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init_A420.c
 *   init_EB00.c
 *   game_130CB0.c
 */

/* init_A420.c */
s32 func_1000A420(s32 dx, s32 dy, s32 dz, f32 angle, s32 distX, s32 distY, s32 distZ, s32 farDistance, s32 nearDistance, s32 *pan, s32 *volume, s32 *distanceOut);
s32 func_1000A750(s32 path, s32 panX, s32 panY, s32 panZ, f32 angle, s32 x, s32 y, s32 z, s32 farDistance, s32 nearDistance, s32 *pan, s32 *volume, s32 *distanceOut);
s32 func_1000B060(f32 dx, f32 dz, s32 angle);

/* init_EB00.c */
s32 func_1000EB00(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 func_1000EBC4(SoundRecord *record, s32 *active, u32 *volume, s32 *pan);
s32 func_1000EC24(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 func_1000ECCC(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 func_1000EDA0(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 func_1000EE70(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 func_1000EF40(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 func_1000EFB4(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
void func_1000F1A8(void);
void func_1000F248(s32 mode);
s32 func_1000F3D0(u16 handle);
s32 func_1000F44C(u16 handle);
s32 func_1000F4D8(u16 sound);
s32 func_1000F568(s32 base, s32 count);
s32 func_1000F6B8(s32 arg0, s16 x, s16 y, s16 z, s32 *pan, s32 nearDistance, s32 farDistance);
void func_1000F85C(u16 handle, s16 param, s32 value);
void func_1000F91C(u16 handle, u16 volume, s16 cents, u8 mix, s32 arg4, s16 x, s16 y, s16 z, s16 nearDistance, s16 farDistance);
void func_1000F9D4(u16 handle, s16 x, s16 y, s16 z);
u16 func_1000FA64(u16 sound, s16 x, s16 y, s16 z, s32 volume, u16 farDistance, s16 nearDistance, SoundRecordCallback callback, void *owner, s32 key, s32 flags, s32 cents);
void func_1000FC18(u16 sound, s16 x, s16 y, s16 z, u16 farDistance); /* stops a positional sound */
void func_1000FD38(SoundRecordCallback callback, void *owner, s32 key);
void func_1000FDF4(u16 handle);
s32 func_1000FE88(SoundRecord *records, s32 index, s32 *count);
s32 func_1000FEF0(u16 handle, void *owner, s32 key);
s32 func_1000FF90(SoundRecordCallback callback, void *owner, s32 key);
void func_1001001C(SoundRecordCallback callback, void *owner, s32 key, s32 volume, s32 cents);
void func_100100E0(SoundRecordCallback callback, void *owner, s32 key, SoundRecordCallback newCallback, void *newOwner, s32 newKey);
s32 func_10010154(u16 sound, Actor *obj, u32 volume, s16 nearDistance, u16 farDistance);
s32 func_10010344(u16 sound, void *owner, u32 volume, s16 nearDistance, u16 farDistance);
void func_10010558(u16 sound, Actor *obj, s32 volume, s16 nearDistance, u16 farDistance, s32 delay);
void func_10010630(u16 sound, Actor *obj, u32 volume, s16 nearDistance, u16 farDistance);
void func_10010720(u16 sound, Actor *obj, s32 volume, s16 nearDistance, u16 farDistance, s32 delay);
s32 func_100107F8(Actor *obj);
s32 func_10010894(Actor *obj);
void func_1001091C(Actor *obj, u32 volume);
s32 func_100109D0(Actor *obj);
void func_10010A3C(Actor *obj);
void func_10010AA8(Actor *obj);
u16 func_10010BE8(u16 handle, s32 sound, u16 volume, u8 pan, s16 cents, u8 mix, u8 bus);
u16 func_10010E78(u16 handle, s32 sound, u16 volume, s16 cents, s32 mix, s32 arg5, s32 x, s32 y, s32 z, s32 nearDistance, s32 farDistance);
/* old-style definition in init_EB00.c; this is the prototype its callers use */
u16 func_10010F30(s32 sound, u16 volume, u8 pan, s16 cents, u8 mix);
u16 func_10010F88(s32 sound, u16 volume, s16 cents, u8 mix, s32 arg4, s16 x, s16 y, s16 z, s16 nearDistance, s16 farDistance);
s32 func_10010FFC(u16 handle, s32 sound, u16 volume, s16 cents, u8 mix, Actor *obj);
void func_100111C8(u16 handle);
void func_1001123C(u16 handle);
s32 func_100112BC(s32 handle, s32 delay);
void func_10011310(void);
s32 func_1001147C(u16 handle);
s32 func_100114D0(s32 x, s32 y, s32 z, s32 volume, s32 farDistance, s32 nearDistance, s32 *pan, s32 *volumeOut, s32 *distance);
void func_10011624(SoundRecord *records, s32 *count, s32 start, s32 end);
void func_10011BB8(void);
void func_10011E88(s32 mode);
void func_10011E94(s32 mute);
u16 func_10011EB8(s32 effect, s16 *volume, s32 variant);
extern s8 D_80041FD8;
extern u8 D_80041FD9;
extern s32 D_80041FDC;

/* game_130CB0.c */
void func_15103800(void);
void func_15103828(void);
s32 func_15103910(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 func_15103AA0(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
void func_15103C14(s32 sound, s32 objId, u32 variantCount, s32 slot, s32 pairVariants);

#endif
