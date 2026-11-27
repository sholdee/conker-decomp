#include <ultra64.h>

#include "conker/actor.h"
#include "conker/collision.h"
#include "conker/game.h"
#include "conker/sound.h"

/* This file's .bss: per-slot cooldowns of sound_playWithCooldown, in ticks. */
static u16 gSoundCooldowns[4];

// Clear the per-slot cooldowns of sound_playWithCooldown.
void sound_clearCooldowns(void) {
    bzero(gSoundCooldowns, sizeof(gSoundCooldowns));
}

// Count the per-slot cooldowns down by this frame's tick count.
void sound_updateCooldowns(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (gSoundCooldowns[i]) {
            if (gSoundCooldowns[i] > gFrameTicks) {
                gSoundCooldowns[i] -= gFrameTicks;
            } else {
                gSoundCooldowns[i] = 0;
            }
        }
    }
}

// The two callbacks below (records started by sound_playWithCooldown) use callback_param1.delay as a
// countdown in the low half (bit 15: skip the fade, sound_followDelayedCallback only) with the held-back sound
// id in the high half, and callback_param2.obj as the object the sound follows.

// Callback for an object with a camera: follow the object; while the delay runs, hold back
// the record's sound and play 0x5B0 instead. Once playing, the object owns the voice.
s32 sound_followDelayedCameraCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents,
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
            sound_playActorSound1(0x5B0, obj, 25000, 100, 120);
            return 0;
        }
        remaining -= gFrameTicks;
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
            sound_playActorSound1(0x5B1, obj, record->max_volume, record->distance_loudest, record->distance_max);
            return 1;
        }
        if (obj->soundHandle1 != record->sound_id) {
            sound_stop(obj->soundHandle1);
            obj->soundHandle1 = record->sound_id;
        }
        record->flags &= ~0x80;
    }
    record->callback_param1.delay = (record->callback_param1.delay & 0xFFFF0000) | remaining;
    return 0;
}

// Callback for an object without a camera: like sound_followDelayedCameraCallback, but with a 15-bit countdown,
// and once playing it sends the voice param 0x8000 unless delay bit 15 is set.
s32 sound_followDelayedCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents,
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
            sound_playActorSound1(0x5B0, obj, -25000, record->distance_loudest, record->distance_max);
            return 0;
        }
        if (remaining <= gFrameTicks) {
            *sound = record->callback_param1.delay >> 16;
            record->sample = *sound;
            record->callback_param1.delay &= ~0x7FFF;
            return 0;
        }
        remaining -= gFrameTicks;
    } else {
        if (record->sound_id == 0) {
            sound_playActorSound1(0x5B1, obj, -25000, record->distance_loudest, record->distance_max);
            return 1;
        }
        if (record->callback_param1.delay != 0) {
            if (!(record->callback_param1.delay & 0x8000)) {
                sound_setVoiceParam(record->sound_id, 0x8000, 2);
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
void sound_playWithCooldown(s32 sound, s32 objId, u32 variantCount, s32 slot, s32 pairVariants) {
    Actor *obj;
    s32 hasCamera;

    hasCamera = FALSE;
    if (gSoundCooldowns[slot] != 0) {
        if (sound != 0x58D) {
            return;
        }
    }

    obj = actor_findOnPath(objId);
    if (obj == NULL) {
        return;
    }
    if (sound_isActorSound1Playing(obj) != 0) {
        return;
    }

    if (variantCount != 0) {
        if (pairVariants) {
            sound += (math_random() % variantCount) * 2;
        } else {
            sound += math_random() % variantCount;
        }
    }

    if (obj->CameraPtr != NULL) {
        hasCamera = TRUE;
    } else if (pairVariants) {
        sound += 1;
    }

    if (hasCamera) {
        sound_addDynamicSound(sound, obj->xpos, obj->ypos, obj->zpos, 30000, 0x7FFF,
                      0x7FFE, sound_followDelayedCameraCallback, (void *)20, (s32)obj, 0x100, 0);
    } else {
        sound_addDynamicSound(sound, obj->xpos, obj->ypos, obj->zpos, 32700, 0x7FFF,
                      0x7FFE, sound_followDelayedCallback, (void *)((pairVariants << 15) | 20), (s32)obj, 0x100, 0);
    }

    gSoundCooldowns[slot] = 600;
}
