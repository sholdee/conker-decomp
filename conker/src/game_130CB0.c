#include <ultra64.h>

#include "functions.h"
#include "variables.h"

typedef struct ObjSoundRecord ObjSoundRecord;

typedef s32 (*ObjSoundCallback)(ObjSoundRecord *record, s32 *active, u32 *volume, s32 *pan,
                                s32 *cents, s32 *mix, u16 *sound);

// The SoundRecord layout of init_EB00.c (D_80041FE0), as func_15103C14 fills it in: the
// owner word is a delay word and the key word is the object the sound follows.
struct ObjSoundRecord {
    /* 0x00 */ s16 sound;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u16 farDistance;
    /* 0x0A */ s16 nearDistance;
    /* 0x0C */ s32 volume;
    /* 0x10 */ s32 flags; // 0x80: finished
    /* 0x14 */ ObjSoundCallback callback;
    /* 0x18 */ s32 delay; // low half: countdown (bit 15: skip the fade, func_15103AA0 only);
                          // high half: the sound id held back while counting down
    /* 0x1C */ Actor *obj;
    /* 0x20 */ u8 pad20[4];
    /* 0x24 */ u16 handle;
};

Actor *func_1505EEF4(s32 id);

// Clear the per-slot cooldowns of func_15103C14.
void func_15103800(void) {
    bzero(D_800D9AB0, sizeof(D_800D9AB0));
}

// Count the per-slot cooldowns down by this frame's tick count.
void func_15103828(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_800D9AB0[i]) {
            if (D_800D9AB0[i] > D_800BE9E4) {
                D_800D9AB0[i] -= D_800BE9E4;
            } else {
                D_800D9AB0[i] = 0;
            }
        }
    }
}

// Callback for an object with a camera: follow the object; while the delay runs, hold back
// the record's sound and play 0x5B0 instead. Once playing, the object owns the voice.
s32 func_15103910(ObjSoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents,
                  s32 *mix, u16 *sound) {
    s16 remaining;
    Actor *obj;

    obj = record->obj;
    remaining = record->delay;
    record->x = obj->x_position;
    record->y = obj->y_position;
    record->z = obj->z_position;
    if (remaining != 0) {
        if (*sound != 0) {
            record->delay = (*sound << 16) | (record->delay & 0xFFFF);
            record->sound = 0;
            *sound = 0;
            func_10010344(0x5B0, obj, 25000, 100, 120);
            return 0;
        }
        remaining -= D_800BE9E4;
        if (remaining <= 0) {
            *sound = record->delay >> 16;
            record->sound = *sound;
            *volume = record->volume;
            *active = 1;
            record->delay = 0;
            return 0;
        }
    } else {
        *volume = record->volume;
        *active = 1;
        if (record->handle == 0) {
            func_10010344(0x5B1, obj, record->volume, record->nearDistance, record->farDistance);
            return 1;
        }
        if (obj->unk8C != record->handle) {
            func_100111C8(obj->unk8C);
            obj->unk8C = record->handle;
        }
        record->flags &= ~0x80;
    }
    record->delay = (record->delay & 0xFFFF0000) | remaining;
    return 0;
}

// Callback for an object without a camera: like func_15103910, but with a 15-bit countdown,
// and once playing it sends the voice param 0x8000 unless delay bit 15 is set.
s32 func_15103AA0(ObjSoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents,
                  s32 *mix, u16 *sound) {
    u32 remaining;
    Actor *obj;

    obj = record->obj;
    remaining = record->delay & 0x7FFF;
    record->x = obj->x_position;
    record->y = obj->y_position;
    record->z = obj->z_position;
    if (remaining != 0) {
        if (*sound != 0) {
            record->delay = (*sound << 16) | (record->delay & 0xFFFF);
            record->sound = 0;
            *sound = 0;
            func_10010344(0x5B0, obj, -25000, record->nearDistance, record->farDistance);
            return 0;
        }
        if (remaining <= D_800BE9E4) {
            *sound = record->delay >> 16;
            record->sound = *sound;
            record->delay &= ~0x7FFF;
            return 0;
        }
        remaining -= D_800BE9E4;
    } else {
        if (record->handle == 0) {
            func_10010344(0x5B1, obj, -25000, record->nearDistance, record->farDistance);
            return 1;
        }
        if (record->delay != 0) {
            if (!(record->delay & 0x8000)) {
                func_1000F85C(record->handle, 0x8000, 2);
            }
            record->delay = 0;
        }
        record->flags &= ~0x80;
    }
    record->delay = (record->delay & ~0x7FFF) | remaining;
    return 0;
}

// Play sound (or a random one of variantCount variants after it) on object objId, unless
// cooldown slot `slot` is still running (sound 0x58D ignores it). With pairVariants, the
// variants come in pairs: the even one for an object with a camera, the odd one otherwise.
void func_15103C14(s32 sound, s32 objId, u32 variantCount, s32 slot, s32 pairVariants) {
    Actor *obj;
    s32 hasCamera;

    hasCamera = FALSE;
    if (D_800D9AB0[slot] != 0) {
        if (sound != 0x58D) {
            return;
        }
    }

    obj = func_1505EEF4(objId);
    if (obj == NULL) {
        return;
    }
    if (func_10010894(obj) != 0) {
        return;
    }

    if (variantCount != 0) {
        if (pairVariants) {
            sound += (func_150ADA20() % variantCount) * 2;
        } else {
            sound += func_150ADA20() % variantCount;
        }
    }

    if (obj->camera != NULL) {
        hasCamera = TRUE;
    } else if (pairVariants) {
        sound += 1;
    }

    if (hasCamera) {
        func_1000FA64(sound, obj->x_position, obj->y_position, obj->z_position, 30000, 0x7FFF,
                      0x7FFE, func_15103910, 20, obj, 0x100, 0);
    } else {
        func_1000FA64(sound, obj->x_position, obj->y_position, obj->z_position, 32700, 0x7FFF,
                      0x7FFE, func_15103AA0, (pairVariants << 15) | 20, obj, 0x100, 0);
    }

    D_800D9AB0[slot] = 600;
}
