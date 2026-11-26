#include <ultra64.h>
#include "structs.h"

// SoundRecordCallback (functions.h) is called each update for a record with flag 0x10, with the values
// sound_updateRecords is about to apply. Each callback gives callback_param1/2 its own meaning.

// SoundRecord.flags, as sound_updateRecords reads them:
//   0x0001 a voice is playing (handle valid)    0x0002 re-evaluate even while playing
//   0x0004 doppler pitch shift                  0x0008 keep the record while silent
//   0x0010 run the callback                     0x0040 position from y only (sound_computePathPanVolume)
//   0x0080 finished, removed by sound_update   0x0100 centered (no pan)
//   0x0200 mirror the pan                       0x0C00 bus override (0x400: bus 0, else 1)
//   0x1000 skip the next update (set when the record was just started)

// Per-player listener (gSoundListeners), refreshed from the cameras by sound_update.
typedef struct {
    s32 x0;      // Camera.focus (one listener, flag 0x80000 clear) or Camera.eye; deltas to sound_computePanVolume
    s32 y4;
    s32 z8;
    s32 xC;      // Camera.eye; the nearest listener is chosen by distance to this
    s32 y10;
    s32 z14;
    f32 field18; // Camera.TrueCameraAngle
} SoundSpatialRecord;

// Deferred voice stop, queued by sound_queueStop.
typedef struct {
    u16 handle;
    u8 delay; // updates to wait
    u8 slot;
} SoundQueueEntry;

// Sound variant table: a base sound id and how many consecutive variants follow it.
typedef struct {
    u16 base;
    u16 count;
} SoundVariant;

/*
 * This file's .bss. It is defined before functions.h and variables.h: IDO places a variable that an
 * extern declared earlier (conker/sound.h) after the others.
 */
static SoundQueueEntry gSoundStopQueue[16];
static s32 D_80041F50;
static s32 D_80041F54;            /* music volume; its low half is what func_10017780 takes */
static f32 D_80041F58;
static u8 *gSoundVariantHistory;            /* per-sound variant history */
static u8 D_80041F60;
static u8 D_80041F61;
static SoundSpatialRecord gSoundListeners[4];
s8 D_80041FD8;
u8 D_80041FD9;
s32 D_80041FDC;
static SoundRecord dynamicSounds[32];
static SoundSlot gVoiceSlots[32];  /* func_1000F1A8 clears all 32; a handle's low 4 bits pick one of the first 16 */
static s32 gDynamicSoundCount;            /* records in use in dynamicSounds */

#include "functions.h"
#include "variables.h"

// The variants of each effect (sound_pickEffectVariant), indexed [effect][variant].
static const SoundVariant gEffectSoundVariants[20][5] = {
    { { 0x000, 0 }, { 0x000, 0 }, { 0x000, 0 }, { 0x000, 0 }, { 0x000, 0 } },
    { { 0x25A, 4 }, { 0x254, 4 }, { 0x258, 2 }, { 0x253, 1 }, { 0x2AB, 4 } },
    { { 0x248, 4 }, { 0x24C, 4 }, { 0x250, 2 }, { 0x252, 1 }, { 0x2AB, 4 } },
    { { 0x270, 4 }, { 0x26A, 4 }, { 0x26E, 2 }, { 0x269, 1 }, { 0x2AB, 4 } },
    { { 0x29C, 4 }, { 0x296, 4 }, { 0x29A, 2 }, { 0x295, 1 }, { 0x2AB, 4 } },
    { { 0x23D, 4 }, { 0x241, 4 }, { 0x245, 2 }, { 0x247, 1 }, { 0x2AB, 4 } },
    { { 0x265, 4 }, { 0x25F, 4 }, { 0x263, 2 }, { 0x25E, 1 }, { 0x2AB, 4 } },
    { { 0x2A7, 4 }, { 0x2A1, 4 }, { 0x2A5, 2 }, { 0x2A0, 1 }, { 0x2AB, 4 } },
    { { 0x248, 4 }, { 0x24C, 4 }, { 0x250, 2 }, { 0x252, 1 }, { 0x2AB, 4 } },
    { { 0x27B, 4 }, { 0x275, 4 }, { 0x279, 2 }, { 0x274, 1 }, { 0x2AB, 4 } },
    { { 0x286, 4 }, { 0x280, 4 }, { 0x284, 2 }, { 0x27F, 1 }, { 0x2AB, 4 } },
    { { 0x286, 4 }, { 0x280, 4 }, { 0x284, 2 }, { 0x27F, 1 }, { 0x2AB, 4 } },
    { { 0x09E, 1 }, { 0x09E, 1 }, { 0x09E, 1 }, { 0x09E, 1 }, { 0x2AB, 4 } },
    { { 0x25A, 4 }, { 0x254, 4 }, { 0x258, 2 }, { 0x253, 1 }, { 0x2AB, 4 } },
    { { 0x000, 0 }, { 0x000, 0 }, { 0x000, 0 }, { 0x000, 0 }, { 0x000, 0 } },
    { { 0x291, 4 }, { 0x28B, 4 }, { 0x28F, 2 }, { 0x28A, 1 }, { 0x2AB, 4 } },
    { { 0x09F, 0 }, { 0x09F, 0 }, { 0x09F, 0 }, { 0x09F, 0 }, { 0x2AB, 4 } },
    { { 0x291, 4 }, { 0x28B, 4 }, { 0x28F, 2 }, { 0x28A, 1 }, { 0x2AB, 4 } },
    { { 0x0A0, 0 }, { 0x0A0, 0 }, { 0x0A0, 0 }, { 0x0A0, 0 }, { 0x2AB, 4 } },
    { { 0x286, 4 }, { 0x280, 4 }, { 0x284, 2 }, { 0x27F, 1 }, { 0x2AB, 4 } },
};

// Not referenced.
static const s16 D_8002C3D0[2][10] = {
    { 0, 10000, 7500, 11000, 9000, 11000, 22000, 16384, 9000, 11000 },
    { 9000, 9000, 16382, 9000, 9000, 9000, 9000, 9000, 0, 0 },
};

void func_10017780(u8 arg0, u16 arg1);
SndState *func_10017438(ALBank *, s16, u16, u8, f32, u8, u8, SndState **);

// Random ambient chirp: every 128-255 ticks, play one of sounds 0x6C-0x6E.
s32 sound_chirpCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents,
                  s32 *mix, u16 *sound) {
    if (record->sound_id != 0) {
        record->sound_id = 0;
    }
    *pan = 64;
    if (movement_obj[0].inwater || (*volume == 0)) {
        *volume = 0;
        *sound = 0;
        return 0;
    }
    record->callback_param1.timer -= D_800BE9E4;
    if (record->callback_param1.timer > 0) {
        *volume = 0;
        *sound = 0;
    } else {
        record->callback_param1.timer = (func_150ADA20() & 0x7F) + 0x80;
        record->sample = (func_150ADA20() % 3U) + 0x6C;
    }
    return 0;
}

// Fade out: once the timer runs out, lower the volume each tick; stop at zero.
s32 sound_fadeOutCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan) {
    s32 newVolume = record->max_volume;
    s32 timer = record->callback_param1.timer;

    timer -= D_800BE9E4;
    if (timer <= 0) {
        newVolume = newVolume - D_800BE9E4 * 1000;
        if (newVolume < 0) {
            return 1;
        }
        record->max_volume = newVolume;
    }
    record->callback_param1.timer = timer;
    return 0;
}

// Delayed non-positional sound: the record's sound moves to callback_param2, and plays when the
// countdown in the low half of callback_param1 runs out.
s32 sound_delayedPlayCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents,
                  s32 *mix, u16 *sound) {
    s16 remaining = record->callback_param1.half[1];

    if (*sound != 0) {
        record->callback_param2.val = *sound;
        record->sample = 0;
        *sound = 0;
    }

    remaining -= D_800BE9E4;

    if (remaining <= 0) {
        if (*volume != 0) {
            sound_play(record->callback_param2.val, *volume, *pan, *cents, *mix);
        }
        return 1;
    }
    record->callback_param1.delay = remaining;
    return 0;
}

s32 func_1000ECCC(SoundRecord *record, s32 *active, u32 *volume, s32 *pan,
                  s32 *cents, s32 *mix, u16 *sound) {
    s16 remaining;
    s16 delayedSound;
    s32 delay;

    delay = record->callback_param1.delay;
    remaining = delay;
    if (*sound != 0) {
        record->callback_param1.delay = (*sound << 16) | (delay & 0xFFFF);
        record->sample = 0;
        *sound = 0;
        delay = record->callback_param1.delay;
    }

    remaining -= D_800BE9E4;
    delayedSound = delay >> 16;
    if (remaining <= 0) {
        *sound = delayedSound;
        record->sample = delayedSound;
        if (sound_isActorSound1Playing(record->callback_param2.obj) == 0) {
            sound_playActorSound1(*sound, record->callback_param2.obj, record->max_volume, record->distance_loudest,
                          record->distance_max);
        }
        return 1;
    }

    record->callback_param1.delay = (delay & 0xFFFF0000) | remaining;
    return 0;
}

s32 func_1000EDA0(SoundRecord *record, s32 *active, u32 *volume, s32 *pan,
                  s32 *cents, s32 *mix, u16 *sound) {
    s16 remaining;
    s32 delay;

    delay = record->callback_param1.delay;
    remaining = delay;
    if (*sound != 0) {
        record->callback_param1.delay = (*sound << 16) | (delay & 0xFFFF);
        record->sample = 0;
        *sound = 0;
        delay = record->callback_param1.delay;
    }
    remaining -= D_800BE9E4;
    if (remaining <= 0) {
        *sound = delay >> 16;
        record->sample = *sound;
        sound_playActorUntracked(*sound, record->callback_param2.obj, record->max_volume, record->distance_loudest,
                      record->distance_max);
        return 1;
    }
    record->callback_param1.delay = (delay & 0xFFFF0000) | remaining;
    return 0;
}

// Follow the owning object: copy its position into the record while it lives.
s32 sound_followOwnerCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents,
                  s32 *mix, u16 *sound) {
    Actor *obj;
    u8 uid;

    if ((obj = record->callback_param1.owner) != NULL) {
        if (*volume != 0) {
            uid = record->callback_param2.val & 0xFF;
            if ((obj->type != 0) && (uid == obj->unique_id)) {
                *mix = (((u32)obj->floorMaterial >> 3) & 0x30) * 2;
                record->x = obj->xpos;
                record->y = obj->ypos;
                record->z = obj->zpos;
                return 0;
            }
            if (func_1000F44C(record->sound_id) == 0) {
                return 0;
            }
        }
    }
    return 1;
}

s32 func_1000EF40(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents,
                  s32 *mix, u16 *sound) {
    if (record->flags & 0x80) {
        record->flags &= ~0x80;
    }
    if (*volume == 0) {
        if (record->sound_id != 0) {
            sound_stop(record->sound_id);
            record->sound_id = 0;
        }
        *sound = 0;
    }
    return 0;
}

s32 func_1000EFB4(SoundRecord *record, s32 *active, u32 *volume, s32 *pan,
                  s32 *cents, s32 *mix, u16 *sound) {
    Actor *owner = record->callback_param1.owner;
    s32 gain;
    u16 *id;
    s32 soundId;

    if (owner != NULL) {
        gain = *volume;
        if (gain != 0) {
            if (owner->type != 0) {
                for (id = record->callback_param2.ids; *id != 0; id++) {
                    if ((owner->animseq.uh == *id) || !(record->flags & 1)) {
                        *mix = (((u32)owner->floorMaterial >> 3) & 0x30) * 2;
                        record->x = owner->xpos;
                        record->y = owner->ypos;
                        record->z = owner->zpos;
                        return 0;
                    }
                }
            }
            soundId = *sound;
            if (soundId == 0xCA) {
                sound_play(0xCB, gain, *pan, 0, *mix);
            } else if (soundId == 0x2CF) {
                sound_play(0x2D7, gain, *pan, 0, *mix);
                sound_play((func_150ADA20() % 3U) + 0x2EB, 16000, *pan, 0, *mix);
            } else if (soundId == 0x2D2) {
                sound_play(0x2DA, gain, *pan, 0, *mix);
                sound_play((func_150ADA20() % 3U) + 0x2EB, 16000, *pan, 0, *mix);
            }
        }
    }
    return 1;
}
void func_1000F1A8(void) {
    s32 i;

    gDynamicSoundCount = 0;
    D_80041FD9 = 1;
    D_80041FD8 = 0;
    bzero(gVoiceSlots, sizeof(gVoiceSlots));

    for (i = 0; i < 16; i++) {
        gVoiceSlots[i].nextHandle = i + 16;
    }

    D_80041F50 = 0;
    func_100176EC();
    D_80041F60 = D_80041F61 = 0;
}

void sound_initLevel(s32 mode) {
    func_1000F1A8();
    if (mode == 4) {
        D_80041F54 = 0;
        D_80041F58 = 0.01f;
    } else {
        D_80041F54 = 23000;
        D_80041F58 = 0.1f;
    }

    if (mode == 0x35) {
        D_80041FD9 = 0;
    } else if (mode == 0x36) {
        D_80041FD9 = 0;
    } else if (mode == 0x3C) {
        D_80041FD9 = 0;
        D_80041FD8 = 60;
    } else if (mode == 0x27) {
        D_80041FD9 = 0;
        D_80041FD8 = 40;
    } else if ((mode == 0x3A) || (mode == 0x40)) {
        D_80041FD9 = 0;
        D_80041FD8 = 60;
    }

    gSoundVariantHistory = allocate_memory(1762, 1, 0, 0);

    bzero(gSoundVariantHistory, 1762);

    if (mode == 0x31) {
        D_80041FDC = 14000;
    } else {
        D_80041FDC = 23000;
    }
    func_10011E88(mode);
    D_80041F60 = D_80041F61 = 0;
    func_10017780(0, D_80041F54);
    func_10017780(1, D_80041F54);
    func_10017780(2, 23000);
}

// Is the voice behind this handle still playing? A bare slot number (< 16) matches any
// handle in that slot.
s32 sound_isPlaying(u16 handle) {
    SoundSlot *slot;

    slot = &gVoiceSlots[handle & 0xF];
    if (slot->voice != NULL) {
        if ((slot->handle == handle) || (handle == (handle & 0xF))) {
            if (func_100173C4(&slot->voice) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_1000F44C(u16 handle) {
    u32 mask;
    SoundSlot *slot;
    SndState *voice;

    mask = __osDisableInt();
    slot = &gVoiceSlots[handle & 0xF];
    voice = slot->voice;
    if ((voice != NULL) && (slot->handle == handle) && (voice->flags & 2)) {
        __osRestoreInt(mask);
        return 1;
    }
    __osRestoreInt(mask);
    return 0;
}

// Is any voice playing this sound id?
s32 sound_isSoundPlaying(u16 sound) {
    s32 i;

    sound &= 0x7FFF;
    for (i = 0; i < 16; i++) {
        if (gVoiceSlots[i].voice != NULL) {
            if ((gVoiceSlots[i].sound & 0x7FFF) == sound) {
                if (func_100173C4(&gVoiceSlots[i].voice) != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// Picks one of the `count` consecutive sound ids starting at `base` at random. For fewer
s32 sound_pickVariant(s32 base, s32 count) {
    s32 roll;
    s32 pick;
    u8 unplayed;
    u8 left;
    s32 mask;

    roll = func_150ADA20() % (u32)count;
    pick = roll;
    if (base >= 1762) {
        return 1;
    }
    if (count < 2) {
        return base;
    }
    if (gSoundVariantHistory != NULL) {
        unplayed = gSoundVariantHistory[base];
        if (count < 8) {
            mask = (1 << count) - 1;
            if (!(unplayed & 0x80) || !(unplayed & mask)) {
                unplayed = 0xFF;
            }
            while (!(unplayed & (1 << pick))) {
                pick = (pick + 1) % count;
            }
            left = unplayed ^ (1 << pick);
            gSoundVariantHistory[base] = left;
            if (!(left & mask)) {
                // all played: start over, but not with the one just picked
                gSoundVariantHistory[base] = unplayed ^ mask;
            }
        } else {
            gSoundVariantHistory[base] = roll + 1;
        }
    }
    return base + pick;
}

// Volume scale (and *pan) of a sound at (x, y, z) for the nearest listener. arg0 is unused.
s32 func_1000F6B8(s32 arg0, s16 x, s16 y, s16 z, s32 *pan, s32 nearDistance, s32 farDistance) {
    s32 volume;
    SoundSpatialRecord *listener;
    s32 i;
    s32 dx;
    s32 dy;
    s32 bestX;
    s32 bestY;
    s32 bestZ;
    s32 dz;
    u32 distance;

    if (gScreenLayout != 0) {
        u32 closest;

        closest = 0xFFFFFFFF;
        for (i = 0; i <= gScreenLayout; i++) {
            dx = x - gSoundListeners[i].xC;
            dy = y - gSoundListeners[i].y10;
            dz = z - gSoundListeners[i].z14;
            distance = dx * dx + dy * dy + dz * dz;
            if (distance < closest) {
                closest = distance;
                listener = &gSoundListeners[i];
                bestX = dx;
                bestY = dy;
                bestZ = dz;
            }
        }
    } else {
        listener = gSoundListeners;
        bestX = x - listener->xC;
        bestY = y - listener->y10;
        bestZ = z - listener->z14;
    }
    sound_computePanVolume(bestX, bestY, bestZ, listener->field18,
                  x - listener->x0, y - listener->y4, z - listener->z8,
                  (s16)farDistance, (s16)nearDistance, pan, &volume, NULL);
    return volume;
}

void sound_setVoiceParam(u16 handle, s16 param, s32 value) {
    if (handle < 16) {
        return;
    }
    if (sound_isPlaying(handle) != 0) {
        if (param == 0x10) {
            // value is in cents; pass the voice its pitch ratio as raw f32 bits
            f32 ratio = alCents2Ratio(value);
            value = *(s32 *)&ratio;
        } else if (param == 0x11) {
            // value already holds a ratio's f32 bits
            param = 0x10;
        }
        func_10017714(gVoiceSlots[handle & 0xF].voice, param, value);
    }
}

void func_1000F91C(u16 handle, u16 volume, s16 cents, u8 mix, s32 arg4,
                   s16 x, s16 y, s16 z, s16 nearDistance, s16 farDistance) {
    s32 pan;
    s32 scale = func_1000F6B8(arg4, x, y, z, &pan, nearDistance, farDistance);

    sound_setVoiceParam(handle, 8, (u32)(scale * volume) >> 15);
    sound_setVoiceParam(handle, 4, pan & 0x7F);
    sound_setVoiceParam(handle, 0x100, (pan & 0x80) | mix);
    sound_setVoiceParam(handle, 0x10, cents);
}

void func_1000F9D4(u16 handle, s16 x, s16 y, s16 z) {
    s32 pan;

    func_1000F6B8(-1, x, y, z, &pan, 32760, 32765);
    sound_setVoiceParam(handle, 4, pan & 0x7F);
    sound_setVoiceParam(handle, 0x100, pan & 0x80);
}

u16 sound_addDynamicSound(u16 sound, s16 x, s16 y, s16 z, s32 volume, u16 farDistance,
                  s16 nearDistance, SoundRecordCallback callback, void *owner, s32 key,
                  s32 flags, s32 cents) {
    s32 count;
    s32 index;

    if (gDynamicSoundCount < 32) {
        index = gDynamicSoundCount;
        gDynamicSoundCount++;
    } else {
        return 0;
    }

    if (callback != NULL) {
        dynamicSounds[index].flags = flags | 0x12;
    } else {
        dynamicSounds[index].flags = (flags & 0x108) | 2;
    }

    if (flags & 0x40) {
        y = collision_findSpawnById(x);
        if (y == -1) {
            return 0;
        }
    }

    dynamicSounds[index].sound_id = 0;
    dynamicSounds[index].mix = 0;
    dynamicSounds[index].pan = 0;
    dynamicSounds[index].sample = sound;
    dynamicSounds[index].x = x;
    dynamicSounds[index].y = y;
    dynamicSounds[index].z = z;
    dynamicSounds[index].distance_max = farDistance;
    dynamicSounds[index].distance_loudest = nearDistance;
    dynamicSounds[index].max_volume = volume;
    dynamicSounds[index].callback_ptr = callback;
    dynamicSounds[index].callback_param1.ptr = owner;
    dynamicSounds[index].callback_param2.val = key;
    dynamicSounds[index].pitch = alCents2Ratio(cents);
    dynamicSounds[index].current_volume = 0;
    dynamicSounds[index].distance_current = 0;
    dynamicSounds[index].pitch_shift_cent = cents;
    count = gDynamicSoundCount;
    sound_updateRecords(dynamicSounds, &gDynamicSoundCount, index, index + 1);
    if (count == gDynamicSoundCount) {
        dynamicSounds[index].flags |= 0x1000;
        return dynamicSounds[index].sound_id;
    }
    return 0;
}

void func_1000FC18(u16 sound, s16 x, s16 y, s16 z, u16 farDistance) {
    s32 soundId;
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 i;
    SoundRecord *record;
    u16 handle;

    soundId = sound;
    posX = x;
    posY = y;
    posZ = z;
    for (i = 0; i < gDynamicSoundCount; i++) {
        record = &dynamicSounds[i];
        if ((soundId == record->sample) && (posX == record->x) && (posY == record->y) &&
            (posZ == record->z) && ((record->distance_max & 0x7FFF) == farDistance)) {
            handle = record->sound_id;
            if (handle != 0) {
                sound_stop(handle);
            }
            dynamicSounds[i].flags |= 0x80; // matching: indexed again, not through record
        }
    }
}

void sound_stopByCallback(SoundRecordCallback callback, void *owner, s32 key) {
    s32 i;

    for (i = 0; i < gDynamicSoundCount; i++) {
        if ((callback == dynamicSounds[i].callback_ptr) && (owner == dynamicSounds[i].callback_param1.ptr) &&
            (key == dynamicSounds[i].callback_param2.val)) {
            if (dynamicSounds[i].sound_id != 0) {
                sound_stop(dynamicSounds[i].sound_id);
            }
            dynamicSounds[i].flags |= 0x80;
        }
    }
}
void sound_stopByHandle(u16 handle) {
    s32 i;
    s32 handleId = handle;

    for (i = 0; i < gDynamicSoundCount; i++) {
        if ((u32)dynamicSounds[i].sound_id == (u32)handleId) { // matching: unsigned compare of the widened handle
            if (dynamicSounds[i].sound_id != 0) {
                sound_stop(dynamicSounds[i].sound_id);
            }
            dynamicSounds[i].flags |= 0x80;
        }
    }
}
s32 sound_stopRecord(SoundRecord *records, s32 index, s32 *count) {
    if (index < *count) {
        if (records[index].sound_id != 0) {
            sound_stop(records[index].sound_id);
        }
        records[index].flags |= 0x80;
        return 0;
    }
    return 1;
}

s32 sound_findByHandle(u16 handle, void *owner, s32 key) {
    s32 i;

    if (!handle) {
        return -1;
    }
    for (i = 0; i < gDynamicSoundCount; i++) {
        if ((handle == dynamicSounds[i].sound_id) && (owner == dynamicSounds[i].callback_param1.ptr) &&
            (key == dynamicSounds[i].callback_param2.val) && !(dynamicSounds[i].flags & 0x80)) {
            return i;
        }
    }
    return -1;
}

s32 sound_findByCallback(SoundRecordCallback callback, void *owner, s32 key) {
    s32 i;

    for (i = 0; i < gDynamicSoundCount; i++) {
        if ((callback == dynamicSounds[i].callback_ptr) &&
            ((owner == dynamicSounds[i].callback_param1.ptr) || (owner == (void *)-1)) &&
            ((key == dynamicSounds[i].callback_param2.val) || (key == -1)) &&
            !(dynamicSounds[i].flags & 0x80)) {
            return i;
        }
    }
    return -1;
}
void func_1001001C(SoundRecordCallback callback, void *owner, s32 key, s32 volume, s32 cents) {
    SoundRecord *record = dynamicSounds;
    s32 i;

    for (i = 0; i < gDynamicSoundCount; i++, record++) {
        if ((callback == record->callback_ptr) && (owner == record->callback_param1.ptr) && (key == record->callback_param2.val)) {
            record->pitch = alCents2Ratio(cents);
            record->max_volume = volume;
        }
    }
}
void sound_replaceCallback(SoundRecordCallback callback, void *owner, s32 key,
                   SoundRecordCallback newCallback, void *newOwner, s32 newKey) {
    s32 i;

    for (i = 0; i < gDynamicSoundCount; i++) {
        if ((callback == dynamicSounds[i].callback_ptr) && (owner == dynamicSounds[i].callback_param1.ptr) &&
            (key == dynamicSounds[i].callback_param2.val)) {
            dynamicSounds[i].callback_ptr = newCallback;
            dynamicSounds[i].callback_param1.ptr = newOwner;
            dynamicSounds[i].callback_param2.val = newKey;
        }
    }
}

s32 sound_playActorSound2(u16 sound, Actor *obj, u32 volume, s16 nearDistance, u16 farDistance) {
    s32 handle = 0;
    s32 flags = 0;

    if ((obj->type == 0) || (obj->type == 5)) {
        return 0;
    }
    if (obj->CameraPtr != NULL) {
        u32 mixFlags = obj->floorMaterial;

        handle = sound_startVoice(obj->soundHandle2, sound, volume, 64, 0, ((mixFlags >> 3) & 0x30) * 2,
                               D_80041FD9);
    } else {
        if (obj->character_type == 0x16) {
            nearDistance *= 2;
            if (farDistance < nearDistance) {
                nearDistance = farDistance - 200;
            }
        } else if ((obj->character_type == 5) || (obj->character_type == 0x4F) || (obj->character_type == 0x83)) {
            flags = 4;
        } else if ((obj->lifting != 0) && ((obj->character_type == 0x8A) || (obj->character_type == 0x23))) {
            flags = 0x100;
        }
        sound_stopByCallback((SoundRecordCallback)sound_followOwnerCallback, obj, obj->unique_id | 0x10000);
        if (obj->type != 0) {
            handle = sound_addDynamicSound(sound, obj->xpos, obj->ypos, obj->zpos,
                                   volume, farDistance, nearDistance,
                                   (SoundRecordCallback)sound_followOwnerCallback, obj,
                                   obj->unique_id | 0x10000, flags, 0);
        }
    }
    obj->soundHandle2 = handle;
    // no return statement here: the original falls off the end
}

s32 sound_playActorSound1(u16 sound, void *owner, u32 volume, s16 nearDistance, u16 farDistance) {
    Actor *obj = owner;
    s32 flags = 0;

    if ((obj->type == 0) || (obj->type == 5)) {
        return 0;
    }
    if ((s32)volume < 0) {
        volume = -(s32)volume;
        flags = 0x100;
    }
    if (obj->CameraPtr != NULL) {
        u8 bus;
        s32 mix;

        bus = D_80041FD9;
        mix = (((u32)obj->floorMaterial >> 3) & 0x30) * 2;
        if ((obj->mv != NULL) && (obj->mv->unk94 == 1)) {
            bus = 1;
            volume = 0x7FFF;
        }
        flags = sound_startVoice(obj->soundHandle1, sound, volume, 64, 0, mix, bus);
    } else {
        if (obj->character_type == 0x16) {
            nearDistance *= 2;
            if (farDistance < nearDistance) {
                nearDistance = farDistance - 200;
            }
        } else if ((obj->character_type == 0x8A) && (obj->lifting != 0)) {
            flags |= 0x100;
        } else if ((obj->character_type == 0x4F) || (obj->character_type == 0x83)) {
            flags |= 4;
        }
        sound_stopByCallback((SoundRecordCallback)sound_followOwnerCallback, obj, obj->unique_id | 0x20000);
        flags = sound_addDynamicSound(sound, obj->xpos, obj->ypos, obj->zpos, volume,
                              farDistance, nearDistance, (SoundRecordCallback)sound_followOwnerCallback,
                              obj, obj->unique_id | 0x20000, flags, 0);
    }
    obj->soundHandle1 = flags;
    return flags;
}

void sound_playActorSound1Delayed(u16 sound, Actor *obj, s32 volume, s16 nearDistance, u16 farDistance,
                   s32 delay) {
    if (delay <= 0) {
        sound_playActorSound1(sound, obj, volume, nearDistance, farDistance);
    } else {
        sound_addDynamicSound(sound, obj->xpos, obj->ypos, obj->zpos, volume,
                      farDistance, nearDistance, (SoundRecordCallback)func_1000ECCC,
                      (void *)delay, (s32)obj, 0, 0);
    }
}

// matching: the empty ifs add uses of volume so that IDO keeps it in s1 like the target
void sound_playActorUntracked(u16 sound, Actor *obj, u32 volume, s16 nearDistance, u16 farDistance) {
    if (volume) {}
    if (volume) {}
    if (volume) {}
    if (obj->type != 0) {
        if (obj->CameraPtr != NULL) {
            u32 mixFlags = obj->floorMaterial;

            sound_play(sound, volume, 64, 0, ((mixFlags >> 3) & 0x30) * 2);
        } else {
            sound_addDynamicSound(sound, obj->xpos, obj->ypos, obj->zpos, volume,
                          farDistance, nearDistance, (SoundRecordCallback)sound_followOwnerCallback, obj,
                          obj->unique_id, 0, 0);
        }
    }
}

void sound_playActorUntrackedDelayed(u16 sound, Actor *obj, s32 volume, s16 nearDistance, u16 farDistance,
                   s32 delay) {
    if (delay <= 0) {
        sound_playActorUntracked(sound, obj, volume, nearDistance, farDistance);
    } else {
        sound_addDynamicSound(sound, obj->xpos, obj->ypos, obj->zpos, volume,
                      farDistance, nearDistance, (SoundRecordCallback)func_1000EDA0,
                      (void *)delay, (s32)obj, 0, 0);
    }
}

s32 sound_isActorSound2Playing(Actor *obj) {
    if (obj->type == 0) {
        return 0;
    } else {
        if (obj->CameraPtr != NULL) {
            if (obj->soundHandle2 != 0) {
                if (sound_isPlaying(obj->soundHandle2) != 0) {
                    return 1;
                }
            }
        } else {
            if (sound_findByCallback((SoundRecordCallback)sound_followOwnerCallback, obj,
                              obj->unique_id | 0x10000) != -1) {
                return 1;
            }
        }
        obj->soundHandle2 = 0;
    }
    return 0;
}

s32 sound_isActorSound1Playing(Actor *obj) {
    if (obj->CameraPtr != NULL) {
        if (obj->soundHandle1 != 0) {
            if (sound_isPlaying(obj->soundHandle1) != 0) {
                return 1;
            }
        }
    } else {
        if (sound_findByCallback((SoundRecordCallback)sound_followOwnerCallback, obj,
                          obj->unique_id | 0x20000) != -1) {
            return 1;
        }
    }
    obj->soundHandle1 = 0;
    return 0;
}

void sound_setActorSound2Volume(Actor *obj, u32 volume) {
    s32 index;

    if ((volume != 0) && (obj->type != 0)) {
        if (obj->CameraPtr != NULL) {
            if (obj->soundHandle2 != 0) {
                sound_setVoiceParam(obj->soundHandle2, 8, *(s32 *)&volume); // matching: the pun makes IDO reload volume from its stack home
            }
        } else {
            index = sound_findByCallback((SoundRecordCallback)sound_followOwnerCallback, obj,
                                  obj->unique_id | 0x10000);
            if (index != -1) {
                dynamicSounds[index].max_volume = volume;
            } else {
                obj->soundHandle2 = 0;
            }
        }
    }
}

// Declared int but returns no value, in the pre-ANSI way: game/Actor/ActorActions/actoractions.c's calls only match with an
// int-returning view (the other callers match either way).
s32 sound_stopActorSound2(Actor *obj) {
    if (obj->CameraPtr != NULL) {
        if (obj->soundHandle2 != 0) {
            sound_stop(obj->soundHandle2);
        }
    } else {
        sound_stopByCallback((SoundRecordCallback)sound_followOwnerCallback, obj, obj->unique_id | 0x10000);
    }
    obj->soundHandle2 = 0;
}

void sound_stopActorSound1(Actor *obj) {
    if (obj->CameraPtr != NULL) {
        if (obj->soundHandle1 != 0) {
            sound_stop(obj->soundHandle1);
        }
    } else {
        sound_stopByCallback((SoundRecordCallback)sound_followOwnerCallback, obj, obj->unique_id | 0x20000);
    }
    obj->soundHandle1 = 0;
}

void sound_stopActorSounds(Actor *obj) {
    s32 index;
    SoundRecord *record;

    if (obj->CameraPtr != NULL) {
        if (obj->soundHandle1 && func_1000F44C(obj->soundHandle1)) {
            sound_stop(obj->soundHandle1);
        }
        if (obj->soundHandle2 && func_1000F44C(obj->soundHandle2)) {
            sound_stop(obj->soundHandle2);
        }
    } else {
        index = sound_findByHandle(obj->soundHandle1, obj, obj->unique_id);
        if (index != -1) {
            if (func_1000F44C(obj->soundHandle1)) {
                sound_stop(obj->soundHandle1);
            }
            record = &dynamicSounds[index];
            record->flags |= 0x80;
        }
        index = sound_findByHandle(obj->soundHandle2, obj, obj->unique_id);
        if (index != -1) {
            if (func_1000F44C(obj->soundHandle2)) {
                sound_stop(obj->soundHandle2);
            }
            record = &dynamicSounds[index];
            record->flags |= 0x80;
        }
    }
    obj->soundHandle1 = 0;
    obj->soundHandle2 = 0;
}

// Starts `sound` on a free voice slot (reusing `handle`'s slot if it is still ours) and
// returns the new handle, or 0. Handles are slot + 16 * n, so the low nibble is the slot.
u16 sound_startVoice(u16 handle, s32 sound, u16 volume, u8 pan, s16 cents, u8 mix, u8 bus) {
    u16 newHandle;
    u16 next;
    u32 slot;

    slot = handle & 0xF;
    if ((handle == gVoiceSlots[slot].handle) && (handle != 0)) {
        if ((gVoiceSlots[slot].voice != NULL) && (func_100173C4(&gVoiceSlots[slot].voice) != 0)) {
            func_10017594(gVoiceSlots[slot].voice);
            gVoiceSlots[slot].voice = NULL;
        }
    } else if (((gVoiceSlots[slot].voice != NULL) && (func_100173C4(&gVoiceSlots[slot].voice) != 0)) ||
               (gVoiceSlots[slot].sound & 0x8000)) {
        slot = 0;
        while ((slot < 16) &&
               (((gVoiceSlots[slot].voice != NULL) && (func_100173C4(&gVoiceSlots[slot].voice) != 0)) ||
                (gVoiceSlots[slot].sound & 0x8000))) {
            slot++;
        }
    }
    if (volume < 100) {
        return 0;
    }
    if (slot >= 16) {
        newHandle = 0;
    } else {
        if (sound == 0) {
            return 0;
        }
        if ((sound & 0x7FFF) >= 1763) {
            return 0;
        }
        newHandle = gVoiceSlots[slot].nextHandle;
        next = newHandle + 16;
        gVoiceSlots[slot].handle = newHandle;
        if (next < 16) { // wrapped around: never hand out a bare slot number
            next += 16;
        }
        gVoiceSlots[slot].nextHandle = next;
        gVoiceSlots[slot].sound = sound;
        if (gVoiceSlots[slot].voice != NULL) {
            gVoiceSlots[slot].voice->state = 5;
        }
        if ((mix & 0x7F) + (u8)D_80041FD8 < 0x80) {
            mix += (u8)D_80041FD8;
        } else {
            mix |= 0x7F;
        }
        func_10017438(gSoundBank, sound & 0x7FFF, volume, pan, alCents2Ratio(cents), mix, bus,
                      &gVoiceSlots[slot].voice);
    }
    return newHandle;
}

u16 func_10010E78(u16 handle, s32 sound, u16 volume, s16 cents, s32 mix, s32 arg5,
                  s32 x, s32 y, s32 z, s32 nearDistance, s32 farDistance) {
    s32 pan;
    u32 scaled;

    scaled = func_1000F6B8(arg5, x, y, z, &pan, (s16)nearDistance, (s16)farDistance);
    scaled = ((u32)volume * scaled) >> 15;
    if (scaled != 0) {
        return sound_startVoice(handle, sound, scaled, pan & 0x7F, cents,
                             (pan & 0x80) | (u8)mix, D_80041FD9);
    }
    return 0;
}

// Plays an unpositioned sound.
u16 sound_play(s32 sound, u16 volume, u8 pan, s16 cents, u8 mix) {
    return sound_startVoice(0, sound, volume, pan, cents, mix, D_80041FD9);
}

u16 func_10010F88(s32 sound, u16 volume, s16 cents, u8 mix, s32 arg4,
                  s16 x, s16 y, s16 z, s16 nearDistance, s16 farDistance) {
    return func_10010E78(0, sound, volume, cents, mix, arg4, x, y, z, nearDistance, farDistance);
}

// Plays a sound positioned on obj; a player-camera object (obj->camera set) plays it
// unpositioned instead.
s32 sound_playOnActor(u16 handle, s32 sound, u16 volume, s16 cents, u8 mix, Actor *obj) {
    f32 scale;

    if ((obj == NULL) || (obj->type == 0)) {
        return 0;
    }
    if (obj->CameraPtr != NULL) {
        handle = sound_startVoice(handle, sound, volume, 64, cents, mix, D_80041FD9);
    } else {
        volume = (volume + volume + volume) >> 2;
        if (obj->character_type != 0xFF) {
            scale = gModelDefs[obj->character_type]->bodyRadius * obj->scale_xz;
        } else {
            scale = 0.0f;
        }
        if (scale > 256.0f) {
            scale = 1.0f;
        } else if (scale < 80.0f) {
            scale = 0.3125f;
        } else {
            scale *= 1.0f / 256.0f;
        }
        handle = func_10010E78(handle, sound, volume, cents, mix, 0, obj->xpos,
                               obj->ypos, obj->zpos, 500,
                               (s32)(2000.0f * scale) + 501);
    }
    return handle;
}

// Stops the voice behind `handle` now.
void sound_stop(u16 handle) {
    SoundSlot *slot = &gVoiceSlots[handle & 0xF];

    if ((slot->voice != NULL) && (slot->handle == handle)) {
        slot->handle = 0;
        slot->sound = 0;
        func_10017594(slot->voice);
        slot->voice = NULL;
    }
}

// Stops the voice behind `handle` on the next update (now if the queue is full).
void sound_stopNextUpdate(u16 handle) {
    SoundSlot *slot = &gVoiceSlots[handle & 0xF];

    if ((slot->voice != NULL) && (slot->handle == handle)) {
        if (sound_queueStop(handle, 1) == 0) {
            func_10017594(slot->voice);
            slot->voice = NULL;
        }
    }
}

s32 sound_queueStop(s32 handle, s32 delay) {
    SoundQueueEntry *entry;
    s32 count = D_80041F50;

    if (count < 16) {
        entry = &gSoundStopQueue[count];
        entry->handle = handle;
        entry->delay = delay;
        entry->slot = handle & 0xF;
        D_80041F50 = count + 1;
        return 1;
    } else {
        return 0;
    }
}

// Runs the stop queue: counts down each entry's delay and stops its voice when it expires.
void sound_runStopQueue(void) {
    s32 i;
    s32 j;
    s32 stopped;
    s32 remaining;
    s32 slot;
    SndState *voice;

    stopped = 0;
    j = 0;
    remaining = D_80041F50;
    for (i = 0; i < D_80041F50; i++) {
        if (gSoundStopQueue[i].delay > 0) {
            gSoundStopQueue[i].delay--;
        } else {
            slot = gSoundStopQueue[i].slot;
            if (gVoiceSlots[slot].handle == gSoundStopQueue[i].handle) {
                voice = gVoiceSlots[slot].voice;
                gVoiceSlots[slot].handle = 0;
                gVoiceSlots[slot].sound = 0;
                if (voice != NULL) {
                    func_10017594(voice);
                }
                gVoiceSlots[slot].voice = NULL;
            }
            remaining--;
            stopped = 1;
        }
        if (j != i) {
            gSoundStopQueue[j] = gSoundStopQueue[i];
        }
        if (stopped) {
            j++;
        } else {
            stopped = 0; // matching: dead store IDO needs for the target's branch layout
        }
    }
    D_80041F50 = remaining;
}

// Returns the sound id playing behind `handle`, or -1.
s32 sound_getPlayingSound(u16 handle) {
    SoundSlot *slot;

    if (handle != 0) {
        slot = &gVoiceSlots[handle & 0xF];
        if (slot->handle == handle) {
            return slot->sound & 0x7FFF;
        }
    }
    return -1;
}

s32 func_100114D0(s32 x, s32 y, s32 z, s32 volume, s32 farDistance, s32 nearDistance,
                  s32 *pan, s32 *volumeOut, s32 *distance) {
    SoundSpatialRecord *best;
    u32 bestDist;
    u32 i;
    u32 dx;
    u32 dy;
    u32 dz;
    s32 scale;
    u32 d;
    s32 active;

    best = gSoundListeners;
    if (gScreenLayout != 0) {
        bestDist = 0xFFFFFFFF;
        for (i = 0; i <= gScreenLayout; i++) {
            dx = x - gSoundListeners[i].xC;
            dy = y - gSoundListeners[i].y10;
            dz = z - gSoundListeners[i].z14;
            d = dx * dx + dy * dy + dz * dz;
            if (d < bestDist) {
                bestDist = d;
                best = &gSoundListeners[i];
            }
        }
    }
    active = sound_computePanVolume(x - best->xC, y - best->y10, z - best->z14, best->field18,
                           x - best->x0, y - best->y4, z - best->z8, farDistance, nearDistance,
                           pan, &scale, distance);
    *volumeOut = ((u32)scale * volume) >> 15;
    return active;
}

// Updates records[start..end) (and below *count): re-evaluates each sound's volume, pan and
void sound_updateRecords(SoundRecord *records, s32 *count, s32 start, s32 end) {
    SoundRecord *record;
    u32 flags;
    s32 pan;
    s32 rate;
    u32 volume;
    s32 active;
    s32 distance;
    s32 cents;
    u16 sound;
    u16 handle;
    s32 mix;
    union {
        f32 value;
        s32 bits;
    } pitch;
    s32 index;
    s32 offset;
    s32 *panOut;
    f32 ratio;
    f32 unclampedRatio;
    f32 pitchDelta;

    // matching: a guarded do-while over a byte offset, with gotos for `continue`; the
    // for-loop form sets up the offset before the guard and loses it from s8
    index = start;
    if ((index < *count) && (index < end)) {
        offset = index * sizeof(SoundRecord);
        do {
        record = (SoundRecord *)((u8 *)records + offset);
        flags = record->flags;
        if (record->flags & 0x80) {
            goto next_record;
        }
        if (record->flags & 0x1000) {
            record->flags &= ~0x1000;
            goto next_record;
        }
        handle = record->sound_id;
        if ((flags & 1) && (handle != 0) && (sound_isPlaying(handle) == 0)) {
            // the voice ended on its own
            handle = 0;
            flags &= ~1;
            if (!(flags & 8)) {
                flags |= 0x80;
                record->sample = 0;
            }
            record->current_volume = 0;
            record->pan = 0;
            record->sound_id = 0;
            record->flags = flags;
            record->pitch = 1.0f;
        }
        if ((flags & 2) || (handle == 0)) {
            if (flags & 0x100) {
                pan = 64;
                panOut = NULL;
            } else {
                panOut = &pan;
            }
            if (flags & 0x40) {
                active = sound_computePathPanVolume(record->y,
                         gSoundListeners->xC, gSoundListeners->y10, gSoundListeners->z14,
                         gSoundListeners->field18, gSoundListeners->x0,
                         gSoundListeners->y4, gSoundListeners->z8,
                         record->distance_max, record->distance_loudest, &pan,
                         (s32 *)&volume, &distance);
                volume = (u32)(record->max_volume * volume) >> 15;
            } else {
                active = func_100114D0(record->x, record->y,
                         record->z, record->max_volume, record->distance_max,
                         record->distance_loudest, panOut, (s32 *)&volume, &distance);
            }
            if (D_800BE615 != 0) {
                volume = 0;
            }
            cents = record->pitch_shift_cent;
            mix = record->mix;
            sound = record->sample;
            if ((flags & 0x10) && (record->callback_ptr != NULL)) {
                record->flags = flags;
                // matching: the record address recomputed as integer offset + records
                if (record->callback_ptr((SoundRecord *)(offset + (u32)records),
                        &active, &volume, &pan, &cents, &mix, &sound) != 0) {
                    sound_stopRecord(records, index, count);
                    goto next_record;
                }
                handle = record->sound_id;
                flags = record->flags;
            }
            if (sound != 0) {
                if (volume != 0) {
                    s32 fxmix;

                    fxmix = (pan & 0x80) | mix;
                    pan &= 0x7F;
                    if (flags & 0x200) {
                        pan = 0x80 - pan;
                        if (pan == 0x80) {
                            pan = 0x7F;
                        }
                        fxmix ^= 0x80;
                    }
                    pitch.value = alCents2Ratio(cents);
                    if (handle == 0) {
                        if (flags & 0xC00) {
                            if ((flags & 0xC00) == 0x400) {
                                rate = 0;
                            } else {
                                rate = 1;
                            }
                        } else {
                            rate = D_80041FD9;
                        }
                        handle = sound_startVoice(0, sound, volume,
                                             pan, cents, fxmix, rate);
                        if (handle != 0) {
                            flags |= 1;
                            record->sample = sound;
                        }
                    } else {
                        if (volume != (u16)record->current_volume) {
                            sound_setVoiceParam(handle, 8, volume);
                        }
                        if (pan != (record->pan & 0x7F)) {
                            sound_setVoiceParam(handle, 4, pan);
                        }
                        if (fxmix != (record->mix | (record->pan & 0x80))) {
                            sound_setVoiceParam(handle, 0x100, fxmix);
                        }
                        if (flags & 4) {
                            // doppler: shift the pitch by how fast the distance changes
                            rate = ((u16)record->distance_current - distance) * D_800BE9E4 / 3;
                            if (rate >= 367) {
                                ratio = 2.0f;
                            } else {
                                unclampedRatio = 367.0f / (367 - rate);
                                ratio = unclampedRatio;
                                if (unclampedRatio > 2.0f) {
                                    ratio = 2.0f;
                                } else if (unclampedRatio < 0.5f) {
                                    ratio = 0.5f;
                                }
                            }
                            pitch.value *= ratio;
                            pitchDelta = pitch.value - record->pitch;
                            pitch.value = record->pitch + pitchDelta * 0.3f;
                        }
                        start = pitch.bits; // matching: the f32 bits go through the dead param
                        if (pitch.value != record->pitch) {
                            sound_setVoiceParam(handle, 0x11, start);
                        }
                    }
                    record->distance_current = distance;
                    record->current_volume = volume;
                    record->pan = pan | fxmix;
                    record->mix = mix;
                    record->pitch = pitch.value;
                } else if (flags & 8) {
                    // silent but persistent: drop the voice, keep the record
                    if (handle != 0) {
                        sound_stop(handle);
                    }
                    handle = 0;
                    flags &= ~1;
                } else {
                    sound_stopRecord(records, index, count);
                    goto next_record;
                }
            }
        }
        record->sound_id = handle;
        record->flags = flags;
next_record:
        index++;
        offset += sizeof(SoundRecord);
        } while ((index < *count) && (index < end));
    }
}

void sound_update(void) {
    s32 i;
    s32 kept;
    s32 delta;

    if ((D_80041F60 == 0) && (D_80041F61 == 0)) {
        for (i = 0; i <= gScreenLayout; i++) {
            Camera *camera = &camera_global_Cameras[i];

            if ((gScreenLayout != 0) || (camera->Mode & 0x80000)) {
                gSoundListeners[i].x0 = camera->eye.x;
                gSoundListeners[i].y4 = camera->eye.y;
                gSoundListeners[i].z8 = camera->eye.z;
            } else {
                gSoundListeners[i].x0 = camera->focus.x;
                gSoundListeners[i].y4 = camera->focus.y;
                gSoundListeners[i].z8 = camera->focus.z;
            }
            gSoundListeners[i].xC = camera->eye.x;
            gSoundListeners[i].y10 = camera->eye.y;
            gSoundListeners[i].z14 = camera->eye.z;
            gSoundListeners[i].field18 = camera->TrueCameraAngle;
        }
        sound_runStopQueue();
        sound_updateRecords(dynamicSounds, &gDynamicSoundCount, 0, gDynamicSoundCount);
        for (i = 0, kept = 0; i < gDynamicSoundCount; i++) {
            dynamicSounds[kept] = dynamicSounds[i];
            if (!(dynamicSounds[kept].flags & 0x80)) {
                kept++;
            }
        }
        gDynamicSoundCount = kept;
    }
    if ((D_80041F54 != D_80041FDC) || (D_80041F60 != D_80041F61)) {
        if (D_80041F61 == 1) {
            func_10017780(0, 0);
            func_10017780(1, 0);
        } else {
            func_10017780(0, D_80041F54);
            func_10017780(1, ((u16 *)&D_80041F54)[1]); // matching: low half via &D_80041F54, not D_80041F56
            delta = (D_80041FDC - D_80041F54) * D_80041F58;
            if (delta != 0) {
                D_80041F54 += delta;
            } else {
                D_80041F54 = D_80041FDC;
            }
        }
        D_80041F60 = D_80041F61;
    }
}

void func_10011E88(s32 mode) {
}

void sound_setMuted(s32 mute) {
    if (mute) {
        D_80041F61 = 1;
    } else {
        D_80041F61 = 0;
    }
}

// Picks the sound id for variant `variant` of effect `effect`, rotating through the
u16 sound_pickEffectVariant(s32 effect, s16 *volume, s32 variant) {
    s32 sound;

    effect = collision_getSurfaceType(effect);

    if (volume != NULL) {
        if (gScreenLayout != 0) {
            *volume = 0x7FFF / (gScreenLayout + 1);
        } else {
            *volume = 0x7FFF;
        }
    }

    sound = gEffectSoundVariants[effect][variant].base;
    if (gEffectSoundVariants[effect][variant].count >= 2) {
        sound = sound_pickVariant(sound, gEffectSoundVariants[effect][variant].count);
    }
    return sound;
}
