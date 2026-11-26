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

/* Positional sound records (init_EB00.c). A record callback runs each update with the values about to be
 * applied and may change them; returning nonzero stops the record. */
typedef struct SoundRecord SoundRecord;
typedef s32 (*SoundRecordCallback)(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix,
                                   u16 *sound);

/* A positional sound being tracked by func_10011624 (dynamicSounds, up to 32).
 * original: struct AlSSound (rom_debug_struct.c); field names are Rare's labels with spaces as underscores,
 * except mix and the members of the two callback parameter unions. */
struct SoundRecord {
    /* 0x00 */ u16 sample;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u16 distance_max;
    /* 0x0A */ s16 distance_loudest;
    /* 0x0C */ s32 max_volume;
    /* 0x10 */ u32 flags; /* see init_EB00.c */
    /* 0x14 */ SoundRecordCallback callback_ptr;
    /* 0x18 */ union {
        void *ptr;
        Actor *owner;   /* the object the sound follows (func_1000EE70, func_1000EFB4) */
        s32 timer;      /* countdown (func_1000EB00, func_1000EBC4) */
        s32 delay;      /* low half: countdown; high half: the sound held back meanwhile */
        u16 half[2];
    } callback_param1;
    /* 0x1C */ union {
        s32 val;        /* func_1000EE70: the owner's unique_id in the low byte */
        Actor *obj;     /* the object the delayed sound plays on (func_1000ECCC, game_130CB0.c) */
        u16 *ids;       /* func_1000EFB4: 0-terminated list of the owner's animation ids (Actor.unk84) */
    } callback_param2;
    /* 0x20 */ s16 pitch_shift_cent;
    /* 0x22 */ u8 pan;     /* last pan sent to the voice; bit 7 is the fx-mix bit */
    /* 0x23 */ u8 mix;
    /* 0x24 */ u16 sound_id; /* voice handle */
    /* 0x26 */ s16 current_volume;
    /* 0x28 */ s16 distance_current;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ f32 pitch;
}; /* size 0x30 */

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

/* game_130CB0.c */
void func_15103800(void);
void func_15103828(void);
s32 func_15103910(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 func_15103AA0(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
void func_15103C14(s32 sound, s32 objId, u32 variantCount, s32 slot, s32 pairVariants);

#endif
