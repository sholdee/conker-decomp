#include <ultra64.h>

#include "functions.h"
#include "variables.h"

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

// The two callbacks below (records started by func_15103C14) use callback_param1.delay as a
// countdown in the low half (bit 15: skip the fade, func_15103AA0 only) with the held-back sound
// id in the high half, and callback_param2.obj as the object the sound follows.

// Callback for an object with a camera: follow the object; while the delay runs, hold back
// the record's sound and play 0x5B0 instead. Once playing, the object owns the voice.
s32 func_15103910(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents,
                  s32 *mix, u16 *sound) {
    s16 remaining;
    Actor *obj;

    obj = record->callback_param2.obj;
    remaining = record->callback_param1.delay;
    record->x = obj->xpos;
    record->y = obj->ypos;
    record->z = obj->zpos;
    if (remaining != 0) {
        if (*sound != 0) {
            record->callback_param1.delay = (*sound << 16) | (record->callback_param1.delay & 0xFFFF);
            record->sample = 0;
            *sound = 0;
            func_10010344(0x5B0, obj, 25000, 100, 120);
            return 0;
        }
        remaining -= D_800BE9E4;
        if (remaining <= 0) {
            *sound = record->callback_param1.delay >> 16;
            record->sample = *sound;
            *volume = record->max_volume;
            *active = 1;
            record->callback_param1.delay = 0;
            return 0;
        }
    } else {
        *volume = record->max_volume;
        *active = 1;
        if (record->sound_id == 0) {
            func_10010344(0x5B1, obj, record->max_volume, record->distance_loudest, record->distance_max);
            return 1;
        }
        if (obj->unk8C != record->sound_id) {
            func_100111C8(obj->unk8C);
            obj->unk8C = record->sound_id;
        }
        record->flags &= ~0x80;
    }
    record->callback_param1.delay = (record->callback_param1.delay & 0xFFFF0000) | remaining;
    return 0;
}

// Callback for an object without a camera: like func_15103910, but with a 15-bit countdown,
// and once playing it sends the voice param 0x8000 unless delay bit 15 is set.
s32 func_15103AA0(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents,
                  s32 *mix, u16 *sound) {
    u32 remaining;
    Actor *obj;

    obj = record->callback_param2.obj;
    remaining = record->callback_param1.delay & 0x7FFF;
    record->x = obj->xpos;
    record->y = obj->ypos;
    record->z = obj->zpos;
    if (remaining != 0) {
        if (*sound != 0) {
            record->callback_param1.delay = (*sound << 16) | (record->callback_param1.delay & 0xFFFF);
            record->sample = 0;
            *sound = 0;
            func_10010344(0x5B0, obj, -25000, record->distance_loudest, record->distance_max);
            return 0;
        }
        if (remaining <= D_800BE9E4) {
            *sound = record->callback_param1.delay >> 16;
            record->sample = *sound;
            record->callback_param1.delay &= ~0x7FFF;
            return 0;
        }
        remaining -= D_800BE9E4;
    } else {
        if (record->sound_id == 0) {
            func_10010344(0x5B1, obj, -25000, record->distance_loudest, record->distance_max);
            return 1;
        }
        if (record->callback_param1.delay != 0) {
            if (!(record->callback_param1.delay & 0x8000)) {
                func_1000F85C(record->sound_id, 0x8000, 2);
            }
            record->callback_param1.delay = 0;
        }
        record->flags &= ~0x80;
    }
    record->callback_param1.delay = (record->callback_param1.delay & ~0x7FFF) | remaining;
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

    if (obj->CameraPtr != NULL) {
        hasCamera = TRUE;
    } else if (pairVariants) {
        sound += 1;
    }

    if (hasCamera) {
        func_1000FA64(sound, obj->xpos, obj->ypos, obj->zpos, 30000, 0x7FFF,
                      0x7FFE, func_15103910, 20, obj, 0x100, 0);
    } else {
        func_1000FA64(sound, obj->xpos, obj->ypos, obj->zpos, 32700, 0x7FFF,
                      0x7FFE, func_15103AA0, (pairVariants << 15) | 20, obj, 0x100, 0);
    }

    D_800D9AB0[slot] = 600;
}
