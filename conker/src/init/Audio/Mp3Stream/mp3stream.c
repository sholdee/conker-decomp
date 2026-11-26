#include <ultra64.h>
#include "structs.h"

/*
 * This file's .bss. It is defined before functions.h and variables.h: IDO places a variable that an
 * extern declared earlier (conker/mp3.h) after the others.
 */
static MsgRing gMp3MessageRing;
static u8 gMp3MessageBuffer[64]; /* gMp3MessageRing's buffer */
s32 D_800427F0;           /* read and cleared by game/Face/ScriptedExpression/scriptedexpression.c */
u16 gMp3LastTrack;           /* the track last started */

#include "functions.h"
#include "variables.h"

/*
 * Streamed-audio front end: a 64-byte message ring (gMp3MessageRing over gMp3MessageBuffer) that the stream
 * player logs into, and helpers that start, stop and query the stream (stream state as returned
 * by func_151F2CDC: 0 = idle, 1 = running, 2 = paused, 5 = opening, 6 = paused while opening,
 * 7 = resuming).
 */

/*
 * Message callback registered with the stream player, called as (0, message, size).
 * Pushes the message onto the ring.
 */
void mp3_pushMessage(s32 unused, void *message, s32 size) {
    text_ringPush(&gMp3MessageRing, message, size);
}

/* Initialise the stream player from `heap` and hook its messages into the ring. */
void mp3_initStream(ALHeap *heap) {
    mp3_initVoice(heap);
    text_ringInit(&gMp3MessageRing, gMp3MessageBuffer, sizeof(gMp3MessageBuffer));
    func_151F3C1C(mp3_pushMessage);
}

/* When entering `levelId`, fade a running stream to silence and stop any other active one,
 * except when entering level 30 with gPrevLevel == 27. */
void mp3_stopForLevel(s32 levelId) {
    s32 state = func_151F2CDC();

    if (levelId != 30 || gPrevLevel != 27) {
        if (state == 1) {
            func_151F2D6C(0, 11010);
            return;
        }
        if (state != 0) {
            func_151F2BA8();
        }
    }
}

/* Start streaming track `id` at `volume` and `pan` (64 = centre). */
void mp3_startTrack(s32 id, s32 volume, s32 pan) {
    s32 data;
    s32 size;

    size = 0;
    data = (s32)rom_findPath(&size, 2, 22, id);
    gMp3LastTrack = id;
    if (data != 0 && size != 0) {
        func_151F2D6C(volume, 0);
        func_151F2DFC(pan, 1);
        if (id != 210) {
            func_151F2E4C(10, 11000);
        } else {
            func_151F2E4C(0, 0);
        }
        func_151F2960(data, size);
    }
}

/* Pop the next logged message into `dst` (at most `dstSize` bytes). */
s32 mp3_popMessage(u8 *dst, s32 dstSize) {
    return text_ringPop(&gMp3MessageRing, dst, dstSize);
}

/*
 * Start track `id` as if played from `obj`'s position: full `volume` and centred when `obj` is
 * a camera, otherwise attenuated between `nearDistance` and `farDistance` and panned.
 */
s32 mp3_startTrackAt(u16 id, Actor *obj, s32 volume, s16 nearDistance, u16 farDistance) {
    s32 pan;
    s32 attenuatedVolume;
    s32 unused;

    if (obj->CameraPtr != 0) {
        mp3_startTrack(id, volume, 64);
    } else {
        func_100114D0(obj->xpos, obj->ypos, obj->zpos, volume, farDistance, nearDistance,
                      &pan, &attenuatedVolume, &unused);
        mp3_startTrack(id, attenuatedVolume, pan);
    }
    return 1;
}

/* Returns 1 while the stream is running, paused or opening. */
s32 mp3_isPlaying(void) {
    s32 state = func_151F2CDC();

    if (state == 1 || state == 2 || state == 5) {
        return 1;
    }
    return 0;
}
