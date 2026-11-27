#include <ultra64.h>

/* A volume that steps from current toward intended by rate per frame (music_stepVolume). */
struct VolumeRamp {
    u16 current;
    u16 intended;
    u16 rate;
};

/* One music sequence instance, from the pool tuneArray.
 * original: struct tune (rom_debug_struct.c); all field names but eventTypes, eventMasks, channelVolumes
 * and VolumeRamp.rate are Rare's. */
struct MusicTrack {
    /* 0x00 */ s32 player;            /* index into gChannelTunes, -1 while not on a channel */
    /* 0x04 */ s32 seq_number;        /* sequence id; -1 marks a free pool entry (original: "seq number") */
    /* 0x08 */ s32 *eventTypes;       /* how each of the 16 event bits behaves (music_readMarkers) */
    /* 0x0C */ u16 *eventMasks;       /* event bits raised by a marker on each marker channel */
    /* 0x10 */ struct MusicTrack *syncTune;  /* track whose markers the sync masks refer to */
    /* 0x14 */ u8 syncTogether;
    /* 0x15 */ u8 internallyPaused;   /* 0 playing, 1 paused while silent, 2 paused on request */
    /* 0x18 */ s32 startSync;         /* marker bits to wait for before replacing the old track */
    /* 0x1C */ s32 volumeSlideSync;   /* marker bits to wait for before stepping the ramps */
    /* 0x20 */ s32 jumpSync;          /* marker bits to wait for before the pending jump */
    /* 0x24 */ s32 jumpPoint;         /* marker + 1 to jump to, 0 for none */
    /* 0x28 */ s32 jumpVolume;
    /* 0x2C */ s32 fullVolume;
    /* 0x30 */ s32 currentVolume;     /* volume last sent to the channel */
    /* 0x34 */ s32 tuneStatus;        /* private state word of the sequence's update callback */
    /* 0x38 */ s32 tuneOffChannels;   /* muted MIDI channels */
    /* 0x3C */ u8 channelVolumes[16];
    /* 0x4C */ struct VolumeRamp vol0;       /* fade */
    /* 0x52 */ struct VolumeRamp vol1;       /* crossfade */
    /* 0x58 */ struct VolumeRamp vol2;       /* duck */
    /* 0x60 */ struct MusicTrack *waitingTune; /* queued to take over this track's channel */
}; /* size 0x64 */

/*
 * This file's .bss. It is defined before the subsystem headers: IDO places a variable that an
 * extern declared earlier (conker/music.h) after the others. The order also rotates with the number of
 * types and externs declared before it; a typedef for struct VolumeRamp here would move D_80041F0C first.
 */
static struct MusicTrack *gChannelTunes[3]; /* track playing on each music channel */
static s32 gMusicEventDecay[3][16];            /* per-event decay rate */
static s32 D_80041880[3];
static s32 gLastMarkerPos[3];                /* last marker position */
static s32 gMarkerInterval[3];                /* distance between the last two marker positions */
static u8 gMarkerBits[3];
s32 gMusicEventLevels[3][16];                   /* per-channel music event level */
static u8 gMusicEventToggles[3][16];             /* per-event toggle */
u8 gLastMarkerChannel;
static struct MusicTrack tuneArray[12];  /* track pool */
OSMesgQueue gMarkerQueues[3];
OSMesg gMarkerQueueBufs[3][8];                 /* message buffers of the gMarkerQueues queues */
u8 gMusicPaused;
s32 gMusicEventFlags;
s32 D_80041F08;
s32 D_80041F0C;

#include <math.h>
#include "conker/actor.h"
#include "conker/audio.h"
#include "conker/camera.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/face.h"
#include "conker/game.h"
#include "conker/mp3.h"
#include "conker/music.h"
#include "conker/sound.h"
#include "conker/zone.h"

/* Per-frame update hook of a music sequence: takes and returns its private state word. */
typedef s32 (*MusicCallback)(s32 state, s32 channel, f32 x, f32 y, f32 z);

/* Settings of one music sequence, indexed by sequence id. */
typedef struct {
    /* 0x0 */ u16 volume;
    /* 0x4 */ s32 flags; /* low nibble (with 0xF0 masked off) is the sequence kind */
    /* 0x8 */ void *update; /* a MusicCallback; the callbacks take different parameter types */
    /* 0xC */ u8 unkC;
} MusicInfo; /* size 0x10 */

/* This file's .data. */
static s32 D_8002B070 = 0;

/* Settings of each music sequence, indexed by sequence id. */
static MusicInfo gMusicInfo[150] = {
    /*   0 */ { 18000, 0x00, NULL, 0 },
    /*   1 */ { 14400, 0x20, music_updateSeq1, 1 },
    /*   2 */ { 26000, 0x00, NULL, 0 },
    /*   3 */ { 30000, 0x20, NULL, 0 },
    /*   4 */ { 18000, 0x22, NULL, 1 },
    /*   5 */ { 18000, 0x01, NULL, 0 },
    /*   6 */ { 18000, 0x00, NULL, 1 },
    /*   7 */ { 19000, 0x21, NULL, 0 },
    /*   8 */ { 18000, 0x00, NULL, 0 },
    /*   9 */ { 26000, 0x00, NULL, 0 },
    /*  10 */ { 20000, 0x00, NULL, 0 },
    /*  11 */ { 26000, 0x10, NULL, 0 },
    /*  12 */ { 26000, 0x01, NULL, 0 },
    /*  13 */ { 30000, 0x00, NULL, 1 },
    /*  14 */ { 18000, 0x04, NULL, 0 },
    /*  15 */ { 30000, 0x00, NULL, 0 },
    /*  16 */ { 18000, 0x00, music_updateSeq16, 0 },
    /*  17 */ { 18000, 0x04, NULL, 0 },
    /*  18 */ { 18000, 0x01, NULL, 0 },
    /*  19 */ { 18000, 0x10, music_updateSeq19, 0 },
    /*  20 */ { 18000, 0x01, music_updateSeq20, 0 },
    /*  21 */ { 18000, 0x85, NULL, 0 },
    /*  22 */ { 25000, 0x85, NULL, 0 },
    /*  23 */ { 21000, 0x85, NULL, 0 },
    /*  24 */ { 18000, 0x01, NULL, 0 },
    /*  25 */ { 18000, 0x52, music_updateSeq25, 0 },
    /*  26 */ { 28000, 0x01, NULL, 0 },
    /*  27 */ { 32000, 0x00, NULL, 0 },
    /*  28 */ { 18000, 0x00, NULL, 0 },
    /*  29 */ { 32700, 0x01, NULL, 0 },
    /*  30 */ { 13000, 0x10, NULL, 0 },
    /*  31 */ { 24000, 0x10, NULL, 0 },
    /*  32 */ { 18000, 0x10, NULL, 0 },
    /*  33 */ { 32000, 0x10, music_updateSeq33, 0 },
    /*  34 */ { 320, 0x02, music_updateSeq34, 0 },
    /*  35 */ { 260, 0x02, music_updateSeq35, 0 },
    /*  36 */ { 32767, 0x10, music_updateSeq36, 0 },
    /*  37 */ { 18000, 0x02, NULL, 0 },
    /*  38 */ { 28000, 0x00, NULL, 0 },
    /*  39 */ { 28000, 0x00, NULL, 0 },
    /*  40 */ { 32000, 0x00, NULL, 0 },
    /*  41 */ { 32000, 0x00, NULL, 0 },
    /*  42 */ { 28000, 0x00, NULL, 0 },
    /*  43 */ { 28000, 0x00, NULL, 0 },
    /*  44 */ { 32700, 0x00, NULL, 0 },
    /*  45 */ { 18000, 0x00, NULL, 0 },
    /*  46 */ { 18000, 0x00, NULL, 0 },
    /*  47 */ { 18000, 0x00, NULL, 0 },
    /*  48 */ { 18000, 0x00, NULL, 0 },
    /*  49 */ { 18000, 0x00, NULL, 0 },
    /*  50 */ { 32000, 0x00, NULL, 0 },
    /*  51 */ { 32000, 0x00, NULL, 0 },
    /*  52 */ { 28000, 0x00, NULL, 0 },
    /*  53 */ { 30000, 0x20, NULL, 0 },
    /*  54 */ { 32700, 0x01, NULL, 0 },
    /*  55 */ { 25000, 0x00, NULL, 0 },
    /*  56 */ { 26000, 0x02, NULL, 1 },
    /*  57 */ { 18000, 0x02, NULL, 0 },
    /*  58 */ { 22000, 0x00, NULL, 0 },
    /*  59 */ { 18000, 0x00, NULL, 0 },
    /*  60 */ { 18000, 0x01, NULL, 0 },
    /*  61 */ { 32767, 0x22, NULL, 0 },
    /*  62 */ { 18000, 0x30, music_updateSeq62, 0 },
    /*  63 */ { 32767, 0x10, NULL, 0 },
    /*  64 */ { 18000, 0x00, NULL, 0 },
    /*  65 */ { 32000, 0x00, NULL, 0 },
    /*  66 */ { 24000, 0x22, NULL, 7 },
    /*  67 */ { 32767, 0x00, NULL, 0 },
    /*  68 */ { 18000, 0x03, NULL, 0 },
    /*  69 */ { 28000, 0x01, NULL, 0 },
    /*  70 */ { 28000, 0x11, NULL, 0 },
    /*  71 */ { 28000, 0x01, NULL, 0 },
    /*  72 */ { 28000, 0x01, NULL, 0 },
    /*  73 */ { 18000, 0x02, NULL, 0 },
    /*  74 */ { 18000, 0x01, NULL, 0 },
    /*  75 */ { 32000, 0x10, NULL, 0 },
    /*  76 */ { 12000, 0x10, music_updateSeq76, 0 },
    /*  77 */ { 18000, 0x10, music_updateSeq77, 0 },
    /*  78 */ { 18000, 0x01, NULL, 0 },
    /*  79 */ { 28000, 0x01, NULL, 0 },
    /*  80 */ { 28000, 0x01, NULL, 0 },
    /*  81 */ { 24000, 0x00, NULL, 0 },
    /*  82 */ { 24000, 0x00, NULL, 0 },
    /*  83 */ { 24000, 0x00, NULL, 0 },
    /*  84 */ { 18000, 0x10, music_updateSeq84, 0 },
    /*  85 */ { 18000, 0x01, NULL, 0 },
    /*  86 */ { 18000, 0x01, NULL, 0 },
    /*  87 */ { 18000, 0x01, NULL, 0 },
    /*  88 */ { 16000, 0x00, music_updateSeq88, 0 },
    /*  89 */ { 32000, 0x58, NULL, 0 },
    /*  90 */ { 20000, 0x00, NULL, 0 },
    /*  91 */ { 32767, 0x18, NULL, 0 },
    /*  92 */ { 25000, 0x00, NULL, 0 },
    /*  93 */ { 25000, 0x00, NULL, 0 },
    /*  94 */ { 25000, 0x00, NULL, 0 },
    /*  95 */ { 24000, 0x10, NULL, 0 },
    /*  96 */ { 18000, 0x10, NULL, 0 },
    /*  97 */ { 18000, 0x10, NULL, 0 },
    /*  98 */ { 15000, 0x00, NULL, 0 },
    /*  99 */ { 24000, 0x01, NULL, 0 },
    /* 100 */ { 18000, 0x01, NULL, 0 },
    /* 101 */ { 32000, 0x08, NULL, 0 },
    /* 102 */ { 18000, 0x00, NULL, 0 },
    /* 103 */ { 18000, 0x00, NULL, 0 },
    /* 104 */ { 26000, 0x00, NULL, 0 },
    /* 105 */ { 18000, 0x01, NULL, 0 },
    /* 106 */ { 26000, 0x01, NULL, 0 },
    /* 107 */ { 26000, 0x00, NULL, 0 },
    /* 108 */ { 32000, 0x01, NULL, 0 },
    /* 109 */ { 26000, 0x01, NULL, 0 },
    /* 110 */ { 18000, 0x00, NULL, 0 },
    /* 111 */ { 22000, 0x08, NULL, 0 },
    /* 112 */ { 18000, 0x01, NULL, 0 },
    /* 113 */ { 32767, 0x01, NULL, 0 },
    /* 114 */ { 18000, 0x03, NULL, 0 },
    /* 115 */ { 18000, 0x01, NULL, 0 },
    /* 116 */ { 18000, 0x01, NULL, 0 },
    /* 117 */ { 24000, 0x01, NULL, 0 },
    /* 118 */ { 32000, 0x01, NULL, 0 },
    /* 119 */ { 18000, 0x01, NULL, 0 },
    /* 120 */ { 18000, 0x03, NULL, 0 },
    /* 121 */ { 18000, 0x01, NULL, 0 },
    /* 122 */ { 18000, 0x01, NULL, 0 },
    /* 123 */ { 32000, 0x03, NULL, 0 },
    /* 124 */ { 24000, 0x03, NULL, 0 },
    /* 125 */ { 18000, 0x03, NULL, 0 },
    /* 126 */ { 18000, 0x03, NULL, 0 },
    /* 127 */ { 18000, 0x03, NULL, 0 },
    /* 128 */ { 18000, 0x01, NULL, 0 },
    /* 129 */ { 24000, 0x01, NULL, 0 },
    /* 130 */ { 18000, 0x01, NULL, 0 },
    /* 131 */ { 18000, 0x01, NULL, 0 },
    /* 132 */ { 32000, 0x01, NULL, 0 },
    /* 133 */ { 18000, 0x10, music_updateSeq36, 0 },
    /* 134 */ { 18000, 0x00, NULL, 0 },
    /* 135 */ { 18000, 0x00, NULL, 0 },
    /* 136 */ { 28000, 0x00, NULL, 0 },
    /* 137 */ { 24000, 0x00, NULL, 0 },
    /* 138 */ { 18000, 0x01, NULL, 0 },
    /* 139 */ { 32000, 0x01, NULL, 0 },
    /* 140 */ { 18000, 0x01, NULL, 0 },
    /* 141 */ { 32000, 0x01, NULL, 0 },
    /* 142 */ { 24000, 0x02, NULL, 1 },
    /* 143 */ { 32767, 0x01, NULL, 0 },
    /* 144 */ { 32000, 0x01, NULL, 0 },
    /* 145 */ { 18000, 0x01, NULL, 0 },
    /* 146 */ { 28000, 0x10, NULL, 0 },
    /* 147 */ { 32767, 0x10, music_updateSeq147, 0 },
    /* 148 */ { 18000, 0x03, NULL, 0 },
    /* 149 */ { 18000, 0x00, NULL, 0 },
};

/* The default eventTypes and eventMasks of a track. */
static s32 gDefaultEventTypes[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };
static u16 gDefaultEventMasks[8] = { 0x1, 0x3, 0x4, 0x8, 0x10, 0x20, 0, 0 };

/* Returns the track playing sequence `id` on a channel, or NULL. */
MusicTrack *music_findTune(s32 id) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (gChannelTunes[i] != NULL && id == gChannelTunes[i]->seq_number) {
            return gChannelTunes[i];
        }
    }
    return NULL;
}

/* Like music_findTune, but also finds a track queued to replace one of the playing tracks. */
MusicTrack *music_findTuneOrQueued(s32 id) {
    s32 i;
    MusicTrack *next;

    for (i = 0; i < 3; i++) {
        if (gChannelTunes[i] != NULL && id == gChannelTunes[i]->seq_number) {
            return gChannelTunes[i];
        }
    }
    for (i = 0; i < 3; i++) {
        if (gChannelTunes[i] != NULL) {
            next = gChannelTunes[i]->waitingTune;
            if (next != NULL && id == next->seq_number) {
                return next;
            }
        }
    }
    return NULL;
}

/* Points every track that syncs to `track` back at itself. */
void music_clearSyncRefs(MusicTrack *track) {
    s32 i;
    MusicTrack *next;

    for (i = 0; i < 3; i++) {
        if (gChannelTunes[i] != NULL) {
            if (track == gChannelTunes[i]->syncTune) {
                gChannelTunes[i]->syncTune = gChannelTunes[i];
            }
            next = gChannelTunes[i]->waitingTune;
            if (next != NULL && track == next->syncTune) {
                next->syncTune = next;
            }
        }
    }
}

/* Takes a free track from the pool and initialises it for sequence `id`. */
MusicTrack *music_allocTune(s32 id) {
    s32 i;

    for (i = 0; i < 12; i++) {
        if (tuneArray[i].seq_number == -1) {
            bzero(&tuneArray[i], sizeof(MusicTrack));
            tuneArray[i].player = -1;
            if (id < 150) {
                tuneArray[i].fullVolume = gMusicInfo[id].volume;
            } else {
                tuneArray[i].fullVolume = 26000;
            }
            tuneArray[i].currentVolume = tuneArray[i].fullVolume;
            tuneArray[i].vol0.intended = tuneArray[i].vol0.current = tuneArray[i].vol1.current =
                tuneArray[i].vol1.intended = tuneArray[i].vol2.current = tuneArray[i].vol2.intended = 0x8000;
            tuneArray[i].seq_number = id;
            tuneArray[i].eventTypes = gDefaultEventTypes;
            tuneArray[i].eventMasks = gDefaultEventMasks;
            tuneArray[i].syncTune = &tuneArray[i];
            return &tuneArray[i];
        }
    }
    return NULL;
}

/* Queues `track` behind `current`, or with no current track puts it on a free channel (via a
   placeholder track it then replaces) or in place of a channel's stopped queued track. */
void music_queueTune(MusicTrack *track, MusicTrack *current) {
    s32 i;
    MusicTrack *placeholder;
    MusicTrack **slot;
    MusicTrack *entry;
    MusicTrack *next;

    placeholder = NULL;
    // matching: taking current's address makes IDO reload it from its arg slot
    if ((&current)[0] != NULL) {
        entry = current->waitingTune;
        if (entry != NULL && track != entry) {
            if (entry->seq_number == track->seq_number) {
                track->seq_number = -1;
                return;
            }
            entry->seq_number = -1;
        }
        current->waitingTune = track;
        return;
    }

    for (i = 0; i < 3; i++) {
        if (placeholder == NULL) {
            slot = &gChannelTunes[i];
            entry = *slot;
            if (entry == NULL || (entry->seq_number <= 0 && entry->waitingTune == NULL)) {
                entry = music_allocTune(0);
                placeholder = entry;
                if (entry != NULL) {
                    entry->player = i;
                    entry->waitingTune = track;
                    *slot = entry;
                    continue;
                } else {
                    track->seq_number = -1;
                    return;
                }
            }
        }
        if (placeholder == NULL) {
            slot = &gChannelTunes[i];
            entry = *slot;
            if (entry != NULL) {
                next = entry->waitingTune;
                if (next != NULL && next->seq_number == 0) {
                    placeholder = (MusicTrack *)-1; /* any non-NULL value ends the search */
                    music_clearSyncRefs(next);
                    (*slot)->waitingTune->seq_number = -1;
                    (*slot)->waitingTune->player = -1;
                    (*slot)->waitingTune = track;
                }
            }
        }
    }
}

/* Writes the ids of up to 3 tracks that are on a channel to `ids`; returns how many. */
s32 music_getPlayingIds(s32 *ids) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < 12; i++) {
        if (tuneArray[i].seq_number != -1 && tuneArray[i].player != -1) {
            if (count < 3) {
                *ids++ = tuneArray[i].seq_number;
                count++;
            }
        }
    }
    return count;
}

/* Update callback (MusicInfo.update) of sequence 1. */
s32 music_updateSeq1(s32 state, u8 channel, s32 arg2, s32 arg3) {
    s32 bit1;
    MusicTrack *track;

    bit1 = state & 2;
    state &= 1;
    track = gChannelTunes[channel];
    if (track == NULL || track->currentVolume < 500) {
        gMusicEventFlags &= ~1;
    }

    if (gMusicEventFlags & 1) {
        if (state == 0) {
            audio_setChannelsEnabled(channel, 0x8000, 1);
            if (level == LEVEL_W1_BARN_INSIDE || level == LEVEL_W1_BARN_OUTSIDE) {
                audio_fadeChannelsVolume(channel, 0x7000, 0, 0);
            } else if (level != LEVEL_W1_DOG_FISH) {
                audio_fadeChannelsVolume(channel, 0xCA, 0, 0);
            }
            audio_setChannelNoteMesg(channel, 15, 1);
        }
        state = 1;
    } else if (state != 0) {
        audio_setChannelsEnabled(channel, 0x8000, 0);
        if (level == LEVEL_W1_BARN_INSIDE || level == LEVEL_W1_BARN_OUTSIDE) {
            audio_fadeChannelsVolume(channel, 0x7000, 255, 0);
        } else if (level != LEVEL_W1_DOG_FISH) {
            audio_fadeChannelsVolume(channel, 0xCA, 255, 0);
        }
        audio_setChannelNoteMesg(channel, 15, 0);
        state = 0;
    }

    if (level == LEVEL_W1_BEE_CAVERN) {
        audio_requestMixSelector(4);
        if (bit1 == 0) {
            bit1 = 2;
            func_1000E704(1, 1, 0xFFFF);
        }
    } else if (bit1 != 0) {
        func_1000E704(1, 0, 0xFFFF);
        bit1 = 0;
    }

    return bit1 | state;
}

/* Update callback (MusicInfo.update) of sequence 16. */
s32 music_updateSeq16(s32 state, s32 channel, s32 arg2, s32 arg3) {
    s32 flag = camera_global_Cameras->SpecialFlags & 1;

    if (flag != 0 && state == 0) {
        state = 1;
        music_setVolume(16, 1000);
    } else if (flag == 0 && state != 0) {
        state = 0;
        music_setVolume(16, 18000);
    }
    return state;
}

/* Update callback (MusicInfo.update) of sequence 19. */
s32 music_updateSeq19(s32 state, u8 channel, f32 x, f32 y, f32 z) {
    s32 volume;

    if (level == LEVEL_W2_OUTSIDE_MOLEHOLE) {
        if ((state & 1) && D_80041F0C == 0) {
            music_fadeChannelVolume(19, 0, 0x1000, 0);
            state &= ~1;
        } else if (D_80041F0C != 0) {
            /* D_80041F08 / D_80041F0C: average of the values music_postEvent reported */
            volume = D_80041F08 / D_80041F0C / 80;
            if (volume > 100) {
                volume = 100;
            }
            music_setChannelVolume(19, volume, 0x1000);
            state |= 1;
        }
        D_80041F08 = 0;
        D_80041F0C = 0;
        if (!(state & 2)) {
            music_setFade(19, 0, 1);
            music_setFade(19, 0x8000, 0);
            state |= 2;
        }
        return state;
    }
    return music_updateSeq36(state, channel, x, y, z);
}

/* Update callback of sequence 77. The top byte of the state holds its positional volume; the rest
   is music_updateSeq36's. */
s32 music_updateSeq77(u32 state, u8 channel, f32 x, f32 y, f32 z) {
    u32 volume;
    s32 pan;
    s32 innerState;

    volume = 0;
    innerState = state & 0xFFFFFF;
    state &= 0xFF000000;
    sound_getPanVolumeAt(0, -377, 8227, 0x7FFF, 4000, 3000, &pan, &volume, NULL);
    volume = (volume << 16) & 0xFF000000;
    if (state != volume) {
        state = volume;
        music_setChannelVolume(77, state >> 24, 0x6000);
    }
    // matching: the u64 round trip steers IDO's temp register for the result
    volume = (u64)(music_updateSeq36(innerState, channel, x, y, z) & 0xFFFFFF);
    return state | volume;
}

/* Update callback of sequence 147: as music_updateSeq77, with the volume inverted. */
s32 music_updateSeq147(u32 state, u8 channel, f32 x, f32 y, f32 z) {
    u32 volume;
    s32 pan;
    s32 innerState;

    volume = 0;
    innerState = state & 0xFFFFFF;
    state &= 0xFF000000;
    sound_getPanVolumeAt(0, 0, 0, 0x7FF8, 3700, 2600, &pan, &volume, NULL);
    volume = ((0x7FFF - volume) << 16) & 0xFF000000;
    if (state != volume) {
        state = volume;
        music_setChannelVolume(147, state >> 24, 0x6000);
    }
    // matching: the u64 round trip steers IDO's temp register for the result
    volume = (u64)(music_updateSeq36(innerState, channel, x, y, z) & 0xFFFFFF);
    return state | volume;
}

/* Update callback (MusicInfo.update) of sequence 20. */
s32 music_updateSeq20(s32 state, s32 channel, s32 arg2, s32 arg3) {
    if (state == 0) {
        func_1000E704(20, 1, 0xFFFF);
        state = 1;
    }
    return state;
}

/* Update callback (MusicInfo.update) of sequence 25. */
s32 music_updateSeq25(s32 state, u8 channel, s32 arg2, s32 arg3) {
    s32 volume = audio_getChannelFade(channel, 0) + audio_getChannelFade(channel, 6) + 1;

    if (volume >= 256) {
        volume = 255;
    } else if (volume < 16) {
        volume = 1;
    }
    if (volume != state) {
        effect_randomizeBounceRecordSpeeds(volume - 1);
        state = volume;
    }
    return state;
}

/* Update callback (MusicInfo.update) of sequence 33. */
s32 music_updateSeq33(s32 state, u8 channel, f32 x, f32 y, f32 z) {
    f32 distance;
    u8 volume;

    // matching: channel's address is taken so IDO keeps it in its arg slot
    if (state == 0) {
        audio_fadeChannelsVolume(*&channel, 3, 16, 0);
        audio_setChannelsFade(channel, 4, 0);
        state = 1;
    } else if (level == LEVEL_W2_HARBOUR) {
        x -= 24.0f;
        z -= 3015.0f;
        distance = x * x + z * z;
        if (36000000.0f < distance) {
            volume = 4;
        } else {
            volume = (u8)((6000.0f - sqrtf(distance)) * 0.041833334f) + 4;
        }
        if (state != volume) {
            audio_setChannelsFade(channel, 3, volume);
        }
        if (zone_isActorOutsideId(0, 0x4041) == 0) {
            distance = 1290.0f - y;
            if (1290.0f < y) {
                volume = 32;
            } else {
                distance *= 0.37166667f;
                if (distance >= 223.0f) {
                    volume = 255;
                } else {
                    volume = (u8)distance + 32;
                }
            }
        } else {
            volume = 0;
        }
        if (volume != audio_getChannelFade(channel, 2)) {
            audio_setChannelsFade(channel, 4, volume);
        }
    }
    return state;
}

/* Update callback (MusicInfo.update) of sequence 34. */
u32 music_updateSeq34(u32 state, u8 channel, f32 x, f32 y, f32 z) {
    s32 oldMode;
    s32 countdown;
    u32 oldMix;
    s32 started;
    u32 volume;
    u32 pan;
    s32 mode;
    s32 volumeChanged;
    s32 panChanged;

    started = 0;
    oldMode = state & 3;
    oldMix = state >> 8;
    if (!(state & 0x80)) {
        music_setVolume(34, 24000);
        if (gCutsceneStates[0] == 1) {
            music_setFade(34, 20, 1);
        }
        state = 0x80;
        started = 1;
    }

    if (level != LEVEL_FRONTEND) {
        audio_stopSeq(channel);
        return state;
    }

    volumeChanged = 0;
    panChanged = 0;
    sound_getPanVolumeAt(-351, 0, 407, 0x7FFF, 3000, 300, &pan, &volume, NULL);

    if (volume < 0x4000) {
        volume = 0x40;
    } else {
        volume >>= 8;
    }

    if (volume != (oldMix >> 8)) {
        volumeChanged = 1;
        audio_setChannelsFade(channel, 6, volume);
    }

    if (pan != (oldMix & 0xFF)) {
        panChanged = 1;
        audio_setChannelPan(channel, 1, pan & 0x7F);
        audio_setChannelPan(channel, 2, pan & 0x7F);
        func_100086FC(channel, 1, pan >> 7);
        func_100086FC(channel, 2, pan >> 7);
    }

    oldMix = (pan << 8) | (volume << 16);

    sound_getPanVolumeAt(-64, 0, 543, 0x7FFF, 3000, 300, &pan, &volume, NULL);
    if (volumeChanged) {
        if (volume < 0x4000) {
            volume = 0x4000;
        }
        audio_setChannelsFade(channel, 1, volume >> 8);
    }

    if (panChanged) {
        audio_setChannelPan(channel, 0, 0x40);
        func_100086FC(channel, 0, 0);
    }

    sound_getPanVolumeAt(-294, 0, 656, 0x7FFF, 3000, 300, &pan, &volume, NULL);
    if (volumeChanged) {
        if (volume < 0x4000) {
            volume = 0x4000;
        }
        audio_setChannelsFade(channel, 24, volume >> 8);
    }

    if (panChanged) {
        audio_setChannelPan(channel, 3, pan & 0x7F);
        audio_setChannelPan(channel, 4, pan & 0x7F);
        func_100086FC(channel, 3, pan >> 7);
        func_100086FC(channel, 4, pan >> 7);
    }

    mode = D_80041F08;
    if (oldMode != mode) {
        switch (mode) {
        case 1:
            countdown = state & 0x7C;
            if (countdown != 0) {
                countdown -= (gFrameTicks >> 1) << 2;
                if (countdown <= 0) {
                    func_1000E704(34, 0, 0xFFFF);
                    state = 0x81;
                } else {
                    state = countdown | 0x80;
                }
            }
            music_fadeChannelVolume(34, 100, 0xFE0, 0);
            break;
        case 2:
            state = 0xF8;
            audio_requestMixSelector(4);
            func_1000E704(34, 1, 0xFFFF);
            music_fadeChannelVolume(34, 0, 0xFE0, started);
            break;
        }
    }

    return state | oldMix;
}

/* Update callback (MusicInfo.update) of sequence 35. */
s32 music_updateSeq35(s32 state, u8 channel, f32 x, f32 y, f32 z) {
    if (!(state & 0x80)) {
        state |= 0x80;
        if (gCutsceneStates[0] != 1) {
            audio_setChannelsFade(channel, 30, 1);
            audio_setChannelsFade(channel, 1, 1);
            music_setVolume(35, 25000);
        } else if (gCutsceneIds[0] == 3) {
            music_setVolume(35, 250);
            effect_requestRainState(2);
        } else if (gCutsceneIds[0] == 6) {
            audio_setChannelsFade(channel, 30, 1);
            audio_setChannelsFade(channel, 1, 0x40);
            effect_requestRainState(2);
        } else {
            music_setVolume(35, 25000);
        }
        return state;
    }
    if (level != LEVEL_FRONTEND) {
        audio_stopSeq(channel);
    } else if (D_80041F08 != (state & 0x7FU)) {
        switch (D_80041F08) {
        case 1:
            audio_fadeChannelsVolume(channel, 30, 0, 0);
            audio_fadeChannelsVolume(channel, 1, 0x40, 0);
            break;
        case 2:
            audio_fadeChannelsVolume(channel, 24, 255, 0);
            audio_fadeChannelsVolume(channel, 6, 0, 0);
            audio_fadeChannelsVolume(channel, 1, 1, 0);
            break;
        }
        state = D_80041F08 | 0x80;
    }
    return state;
}

/* Update callback of sequences 36 and 133, also run by those of 19, 77 and 147. State word:
   bits 0-1 marker mode, 2-7 countdown, 8-15 and 16-23 the mode's two parameters, 24-31 fade level. */
s32 music_updateSeq36(s32 state, u8 channel, f32 x, f32 y, f32 z) {
    s32 oldMode;
    s32 mode;
    s32 countdown;
    u32 upper;
    s32 oldLow;
    u32 fade;
    s32 low;
    s32 high;
    s32 oldHigh;

    upper = (u32)state >> 8;
    mode = oldMode = state & 3;
    countdown = ((u32)state >> 2) & 0x3F;
    oldLow = upper & 0xFF;
    oldHigh = ((u32)state >> 16) & 0xFF;
    fade = state & 0xFF000000;
    low = oldLow; high = oldHigh; // matching: one line, as -g3 line numbers steer the scheduler
    if (D_80041F08 != 0 && (oldMode != 2 || countdown == 0 || D_80041F08 == 2)) {
        mode = D_80041F08;
        low = D_80041F0C & 0xFF;
        high = D_80041F0C >> 8;
        countdown = 30;
    }
    if (countdown != 0) {
        countdown -= gFrameTicks;
        if (countdown <= 0) {
            countdown = 0;
            mode = 0;
        }
    }
    if (mode != oldMode) {
        if (oldMode != 0) {
            // matching: channel's address is taken so IDO keeps it in its arg slot
            audio_muteChannel(*&channel, oldMode + 9);
        }
        if (mode != 0) {
            audio_setChannelFade(channel, mode + 9, low);
            func_100086FC(channel, mode + 9, high >> 7);
            audio_setChannelPan(channel, mode + 9, high & 0x7F);
        }
    } else if (oldMode != 0 && D_80041F08 != 0 && upper != D_80041F0C) {
        if (low != oldLow) {
            audio_setChannelFade(channel, oldMode + 9, low);
        }
        if (high != oldHigh) {
            if ((high ^ oldHigh) & 0x80) {
                func_100086FC(channel, mode + 9, high >> 7);
            }
            audio_setChannelPan(channel, mode + 9, high & 0x7F);
        }
    }
    if (gMusicEventFlags & 0x10) {
        gMusicEventFlags &= ~0x10;
        if (fade == 0) {
            audio_setChannelsFade(channel, 0xC0, 0x80);
        }
        fade = 0xFF000000;
    }
    if (fade != 0) {
        upper = (gFrameTicks << 23) & 0xFF000000;
        if (upper < fade) {
            fade -= upper;
        } else {
            audio_fadeChannelsVolume(channel, 0xC0, 0, 90);
            fade = 0;
        }
    }
    D_80041F08 = 0;
    return (countdown * 4) | mode | (low << 8) | (high << 16) | fade;
}

/* Update callback (MusicInfo.update) of sequence 62. */
s32 music_updateSeq62(s32 state, s32 channel, f32 x, f32 y, f32 z) {
    f32 volume;

    if (level == LEVEL_W1_ROCK_SOLID) {
        if (state != 2) {
            if (music_findTune(9) == NULL) {
                func_1000E704(62, 0, 0xFFFF);
                music_setVolume(62, 0x7FFF);
                music_playTune(61, 62, 4);
            }
            return 2;
        }
        return 0;
    }
    if (D_8002B070 == 0) {
        D_8002B070 = 1;
    }
    if (state != D_8002B070) {
        state = D_8002B070;
    }
    x -= -4000.0f;
    volume = 24000.0f - sqrtf(x * x + z * z) * 10.0f;
    if (volume < 100.0f) {
        volume = 100.0f;
    } else if (24000.0f < volume) {
        volume = 24000.0f;
    }
    music_setVolume(62, volume);
    return state;
}

/* Update callback (MusicInfo.update) of sequence 76. */
s32 music_updateSeq76(s32 state, s32 channel, s32 arg2, s32 arg3) {
    s32 volume;
    s32 maxVolume;
    s32 flag;

    volume = 0;
    flag = camera_global_Cameras->SpecialFlags & 1;
    if (flag != 0) {
        maxVolume = 0x7FFF;
    } else {
        maxVolume = 12000;
    }
    if (level == LEVEL_W1_U_BEND && flag == 0) {
        sound_getPanVolumeAt(2200, 1066, -1600, maxVolume, 3000, 1500, NULL, &volume, NULL);
        volume &= 0xFF00;
        volume = maxVolume - volume;
    }
    // matching: the original masks the comparison's result, not its operands
    if ((volume != state) & 0xFFFF) {
        music_setVolume(84, volume);
    }
    return volume | 0x80000000;
}

/* Update callback (MusicInfo.update) of sequence 84. */
s32 music_updateSeq84(s32 state, s32 channel, s32 arg2, s32 arg3) {
    s32 volume;

    if (level == LEVEL_W1_U_BEND) {
        if (camera_global_Cameras->SpecialFlags & 1) {
            volume = 0;
        } else {
            sound_getPanVolumeAt(2200, 1066, -1600, 24000, 3000, 1500, NULL, &volume, NULL);
            volume &= 0xFF00;
        }
    } else {
        volume = 24000;
    }

    // matching: the original masks the comparison's result, not its operands
    if ((volume != state) & 0xFFFF) {
        music_setVolume(84, volume);
    }
    return volume | 0x80000000;
}

/* Update callback (MusicInfo.update) of sequence 88. */
s32 music_updateSeq88(s32 state, u8 channel, f32 x, f32 y, f32 z) {
    s32 bit1;

    bit1 = state & 2;
    state &= 1;
    if (level == LEVEL_W1_BRONTO_THROAT) {
        audio_requestMixSelector(4);
        if (state == 0) {
            state = 1;
            func_1000E704(88, 1, 0xFFFF);
        }
    } else if (state != 0) {
        func_1000E704(88, 0, 0xFFFF);
        music_setVolume(88, 16000);
        state = 0;
    }
    if (bit1 == 0) {
        audio_fadeChannelsVolume(channel, 0x1000, 0, 1);
        bit1 = 2;
    }
    return bit1 | state;
}

/* Starts fading the first two channels to `volume`. */
void music_fadeFirstTwoChannels(s32 volume) {
    if (gChannelTunes[0] != NULL) {
        gChannelTunes[0]->vol0.intended = volume;
        gChannelTunes[0]->vol0.rate = 0x500;
    }
    if (gChannelTunes[1] != NULL) {
        gChannelTunes[1]->vol0.intended = volume;
        gChannelTunes[1]->vol0.rate = 0x500;
    }
}

/* Sets the duck ramp of each channel in channelMask; a rate of 0 applies the target at once. */
void music_setDuck(s32 target, s32 rate, s32 channelMask) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if ((1 << i) & channelMask) {
            if (gChannelTunes[i] != NULL) {
                gChannelTunes[i]->vol2.intended = target;
                gChannelTunes[i]->vol2.rate = rate;
                if (rate == 0) {
                    gChannelTunes[i]->vol2.current = target;
                }
            }
        }
    }
}

/* Recomputes the channel's volume from its ramps and base volume and sends it if it changed. */
void music_updateChannelVolume(s32 channel) {
    s32 volume;
    MusicTrack *track;

    track = gChannelTunes[channel];
    if (track != NULL) {
        volume = (((u32)(track->vol2.current * ((u32)(track->vol0.current * track->vol1.current) >> 15)) >> 15) *
                  track->fullVolume) >> 15;
        if (volume != track->currentVolume) {
            if (track->currentVolume == 0) {
                audio_setChannelMask(channel, track->tuneOffChannels ^ 0xFFFF, 1);
            } else if (volume == 0) {
                audio_setChannelMask(channel, track->tuneOffChannels ^ 0xFFFF, 0);
            }
            track->currentVolume = volume;
            audio_setSeqVolume(channel, volume);
        }
    }
}

/* Steps `current` toward `target` by rate per frame. */
s32 music_stepVolume(s32 current, s32 target, s32 rate) {
    if (target != current) {
        rate *= gFrameTicks;
        if (current < target) {
            current += rate;
            if (target < current) {
                current = target;
            }
        } else {
            current -= rate;
            if (current < target || current < 0) {
                current = target;
            }
        }
    }
    return current;
}

/* Returns TRUE once syncTo's sequence has raised every marker bit in waitMask (or when there is
   nothing to wait for). */
s32 music_isSyncReached(u8 waitMask, MusicTrack *syncTo) {
    if (waitMask == 0) {
        return TRUE;
    }
    if (syncTo != NULL) {
        if (syncTo->player >= 0) {
            if (gChannelTunes[syncTo->player] == NULL || syncTo->seq_number <= 0) {
                return TRUE;
            }
            if (audio_getSeqState(syncTo->player) == 3) {
                return TRUE;
            }
            if (!(gMusicInfo[syncTo->seq_number].flags & 0x20)) {
                gMarkerBits[syncTo->player] |= 3;
            }
            waitMask &= ~gMarkerBits[syncTo->player];
        } else {
            return TRUE;
        }
    } else {
        return TRUE;
    }
    return waitMask == 0;
}

/* Drains the channel's marker messages into gMarkerBits and the per-event tables, then updates
   the channel's paused state and decays the event levels. */
void music_readMarkers(s32 channel) {
    s32 msg;
    s32 *types;
    s32 rate;
    u32 markerChannel;
    u16 bits;
    u16 *masks;
    s32 i;

    if (gChannelTunes[channel] != NULL) {
        types = gChannelTunes[channel]->eventTypes;
        masks = gChannelTunes[channel]->eventMasks;
        gMarkerBits[channel] = 0;
        while (osRecvMesg(&gMarkerQueues[channel], (OSMesg *)&msg, OS_MESG_NOBLOCK) == 0) {
            markerChannel = msg & 7;
            if (msg & 0x10) {
                if (markerChannel < 2) {
                    rate = 1300;
                    msg >>= 5;
                    if (gLastMarkerPos[channel] != 0) {
                        gMarkerInterval[channel] = msg - gLastMarkerPos[channel];
                    }
                    gLastMarkerPos[channel] = msg;
                    D_80041880[channel] = msg;
                }
                if (masks[markerChannel] != 0) {
                    bits = masks[markerChannel];
                    gMarkerBits[channel] |= bits & 0x7F;
                    for (i = 0; i < 16 && bits; i++, bits >>= 1) {
                        if (bits & 1) {
                            switch (types[i]) {
                            case 0:
                                gMusicEventToggles[channel][i] ^= 1;
                                gMusicEventLevels[channel][i] = 0x8000;
                                gMusicEventDecay[channel][i] = rate;
                                break;
                            case 1:
                                gMusicEventToggles[channel][i] ^= 1;
                                rate >>= 1;
                                if (gMusicEventToggles[channel][i] == 0) {
                                    gMusicEventLevels[channel][i] = rate * gFrameTicks + 0x8000;
                                }
                                gMusicEventDecay[channel][i] = rate;
                                break;
                            case 2:
                                gMusicEventToggles[channel][i & 7] ^= 1;
                                gMusicEventToggles[channel][i | 8] = gMusicEventToggles[channel][i & 7] ^ 1;
                                gMusicEventLevels[channel][i & 7] = 0x8000;
                                gMusicEventLevels[channel][i | 8] = 0x8000;
                                gMusicEventDecay[channel][i] = 0;
                                break;
                            default:
                                gMusicEventToggles[channel][i] |= 1;
                                gMusicEventLevels[channel][i] = rate * gFrameTicks;
                                gMusicEventDecay[channel][i] = rate;
                                break;
                            }
                        }
                    }
                }
                gLastMarkerChannel = channel;
            } else if (gMusicEventFlags & 1) {
                face_requestExpression(movement_obj, 0x44, 1, 0x12, 5);
            }
        }
        if (gChannelTunes[channel]->currentVolume == 0) {
            gMarkerBits[channel] |= 0x80;
            if (gChannelTunes[channel]->internallyPaused == 0) {
                audio_pauseSeq(channel);
                gChannelTunes[channel]->internallyPaused = 1;
            }
        } else if (gChannelTunes[channel]->internallyPaused == 1) {
            audio_playSeqIfStopped(channel);
            gChannelTunes[channel]->internallyPaused = 0;
        }
        for (i = 0; i < 16; i++) {
            if (gMusicEventLevels[channel][i] != 0) {
                gMusicEventLevels[channel][i] -= gMusicEventDecay[channel][i] * gFrameTicks;
                if (gMusicEventLevels[channel][i] < 0) {
                    gMusicEventLevels[channel][i] = 0;
                }
            }
        }
    }
}

/* Per-frame update of one music channel: pending jumps, stopping and handing the channel over
   to the queued track, the sequence's update callback and the volume ramps. */
void music_updateChannel(s32 index, f32 x, f32 y, f32 z) {
    s32 status;
    MusicTrack **slot;
    MusicTrack *track;
    s32 channel;
    s32 offset;
    MusicTrack *next;
    MusicTrack *syncTo;
    MusicCallback callback;
    s32 i;

    // matching: index and channel are read back from their stack slots on every use, which IDO
    // only does through these pointer views; offset is the byte offset of index in the tables
    channel = (u8)((u32 *)&index)[0];
    status = audio_getSeqState((u8)((u32 *)&index)[0]);
    offset = ((u32 *)&index)[0] * sizeof(*slot);
    slot = (MusicTrack **)&(*(u8 (*)[])gChannelTunes)[offset];
    track = *slot;
    if (track != NULL) {
        if (track->jumpPoint != 0 && music_isSyncReached(track->jumpSync, track->syncTune)) {
            audio_jumpToSeqMarker(((u8 *)&channel)[3], (*slot)->jumpPoint - 1);
            track = *slot;
            // matching: the u32 casts steer IDO's register choice for the pending volume
            if (track->jumpVolume != 0) {
                track->currentVolume = (u32)track->jumpVolume;
                (*slot)->fullVolume = (u32)track->jumpVolume;
                track = *slot;
            }
            track->vol0.current = 0x7FFF;
            (*slot)->vol0.intended = 0x7FFF;
            music_updateChannelVolume(((u32 *)&index)[0]);
            (*slot)->jumpPoint = 0;
            (*slot)->jumpSync = 0;
        }
        track = *slot;
        if (status == 0) {
            track->seq_number = 0;
            track = *slot;
        }
        next = track->waitingTune;
        if (next != NULL) {
            if (track->seq_number == 0) {
                if (music_isSyncReached(next->startSync, next->syncTune)) {
                    (*slot)->seq_number = -1;
                    music_clearSyncRefs(*slot);
                    next = (*slot)->waitingTune;
                    if (next->seq_number > 0) {
                        if (audio_playSequence(channel, next->seq_number) == -1) {
                            (*slot)->waitingTune->syncTune = NULL;
                            (*slot)->waitingTune->startSync = 0;
                            music_queueTune((*slot)->waitingTune, NULL);
                            (*slot)->waitingTune = NULL;
                            *slot = NULL;
                        } else {
                            *slot = (*slot)->waitingTune;
                            (*slot)->player = ((u32 *)&index)[0];
                            (*slot)->startSync = 0;
                            *(s32 *)((u8 *)gMarkerInterval + offset) = 0;
                            *(s32 *)((u8 *)gLastMarkerPos + offset) = 0;
                            *(s32 *)((u8 *)D_80041880 + offset) = 0;
                            track = *slot;
                            if (gMusicInfo[track->seq_number].unkC != 0) {
                                audio_findSeqLoopMarkers(channel, gMusicInfo[track->seq_number].unkC, 100);
                                track = *slot;
                            }
                            if (track->jumpPoint != 0) {
                                audio_jumpToSeqMarker(channel, track->jumpPoint - 1);
                                (*slot)->jumpPoint = 0;
                                track = *slot;
                            }
                            track->currentVolume = 0;
                            music_updateChannelVolume(((u32 *)&index)[0]);
                            audio_playSeqIfStopped(channel);
                            if ((*slot)->tuneOffChannels != 0) {
                                audio_setChannelsEnabled(channel, (*slot)->tuneOffChannels, 0);
                            }
                            for (i = 0; i < 16; i++) {
                                if ((*slot)->channelVolumes[i] != 0) {
                                    audio_fadeChannelVolume(channel, i, (*slot)->channelVolumes[i], 1);
                                    (*slot)->channelVolumes[i] = 0;
                                }
                            }
                        }
                    } else {
                        next->seq_number = -1;
                        (*slot)->waitingTune->syncTune = NULL;
                        (*slot)->waitingTune->startSync = 0;
                        (*slot)->player = -1;
                        *slot = NULL;
                    }
                }
            } else if (music_isSyncReached(next->startSync, next->syncTune)) {
                (*slot)->internallyPaused = 0;
                audio_stopSeq(channel);
            }
            track = *slot;
        } else if (track->seq_number == 0) {
            music_clearSyncRefs(track);
            track = NULL;
            (*slot)->seq_number = -1;
            (*slot)->player = -1;
            *slot = NULL;
        }
        if (track != NULL) {
            if (track->seq_number > 0) {
                callback = gMusicInfo[track->seq_number].update;
                if (callback != NULL) {
                    (*slot)->tuneStatus = callback(track->tuneStatus, (u8)channel, x, y, z);
                    track = *slot;
                }
            }
            if (music_isSyncReached(track->volumeSlideSync, track->syncTune)) {
                track = *slot;
                (*slot)->vol0.current = music_stepVolume(track->vol0.current, track->vol0.intended, track->vol0.rate);
                track = *slot;
                (*slot)->vol1.current =
                    music_stepVolume(track->vol1.current, track->vol1.intended, track->vol1.rate);
                track = *slot;
                (*slot)->vol2.current = music_stepVolume(track->vol2.current, track->vol2.intended, track->vol2.rate);
                (*slot)->volumeSlideSync = 0;
            }
            music_updateChannelVolume(((u32 *)&index)[0]);
            track = *slot;
            if (track->syncTogether != 0 &&
                ((syncTo = track->syncTune) == NULL || syncTo == track || syncTo->seq_number <= 0 ||
                 track->seq_number <= 0 || syncTo->player < 0)) {
                track->syncTogether = 0;
                track = *slot;
                track->syncTune = track;
            }
        }
    }
}

/* Per-frame music update: ducks the channels by the kinds of sequence playing, then runs
   music_readMarkers and music_updateChannel on every channel. */
void music_update(f32 x, f32 y, f32 z) {
    MusicInfo *infos;
    s32 kind5Mask;
    s32 noDuckMask;
    s32 kind34Mask;
    s32 kind12Mask;
    s32 i;
    s32 id;
    s32 flags;
    s32 kind;
    MusicTrack *track;

    kind5Mask = 0;
    noDuckMask = 0;
    kind34Mask = 0;
    kind12Mask = 0;
    if (gMusicPaused == 0) {
        infos = gMusicInfo;
        for (i = 0; i < 3; i++) {
            track = gChannelTunes[i];
            if (track != NULL) {
                id = track->seq_number;
                if (id > 0) {
                    flags = infos[id].flags;
                    kind = flags & ~0xF0;
                    if (flags & 0x40) {
                        noDuckMask |= 1 << i;
                    }
                    if (kind == 5) {
                        kind5Mask |= 1 << i;
                    } else if (kind == 4 || kind == 3) {
                        kind34Mask |= 1 << i;
                    } else if (kind == 1 || kind == 2) {
                        kind12Mask |= 1 << i;
                    }
                }
            }
        }
        if (kind5Mask != 0) {
            music_setDuck(6000, 0x400, kind5Mask ^ 0xFF ^ noDuckMask);
            music_setDuck(0x8000, 0x6400, kind5Mask);
        } else if (kind34Mask != 0) {
            music_setDuck(500, 0x400, kind34Mask ^ 0xFF);
            music_setDuck(0x8000, 0x800, kind34Mask);
        } else if (mp3_getPlayState() == 1 &&
                   (gMp3LastTrack < 125 || gMp3LastTrack >= 129) &&
                   gMp3LastTrack < 457 && gMp3LastTrack != 368 &&
                   gMp3LastTrack != 369) {
            music_setDuck(14000, 0x200, kind12Mask ^ 0xFF);
        } else {
            music_setDuck(0x8000, 0x800, 0xFF);
        }
        for (i = 0; i < 3; i++) {
            music_readMarkers(i);
        }
        for (i = 0; i < 3; i++) {
            music_updateChannel(i, x, y, z);
        }
    }
}

/* Starts sequence newId, replacing oldId with the given transition (0 for none):
   1/2 start once oldId's sequence raises a marker, 3 fades in, 4/6 crossfade, 5 fades out oldId. */
void music_playTune(s32 newId, s32 oldId, s32 transition) {
    MusicTrack *outgoing;
    MusicTrack *incoming;
    s32 sync;

    outgoing = NULL;
    incoming = NULL;
    sync = 0;
    newId &= 0xFFF;
    oldId &= 0xFFF;
    if (newId != 0 && music_findTuneOrQueued(newId) != NULL) {
        MusicTrack *track;
        MusicTrack *next;

        track = music_findTune(newId);
        if (track == NULL) {
            return;
        }
        next = track->waitingTune;
        if (next != NULL) {
            if (next->player == -1) {
                track->vol1.intended = 0x8000;
                track->vol1.rate = 0x400;
                next->seq_number = -1;
                track->waitingTune->player = -1;
                track->waitingTune = NULL;
            }
            newId = 0;
            if (oldId == 0) {
                return;
            }
            // matching: IDO shares this tail with the next branch only through the goto
            goto lookup_outgoing;
        } else if (track->internallyPaused != 0 || track->vol1.intended == 0) {
            track->vol1.intended = 0x8000;
            track->vol1.current = 0;
            track->vol1.rate = 0x400;
            newId = 0;
            if (oldId == 0) {
                return;
            }
lookup_outgoing:
            ;
        } else {
            return;
        }
    }
    if (oldId != 0) {
        outgoing = music_findTune(oldId);
    }
    if (outgoing != NULL) {
        // matching: the redundant NULL check and the dead `if` keep IDO's branch layout
        if (outgoing != NULL && outgoing->waitingTune != NULL) {
            if (newId != 0) {
                return;
            }
            return;
        }
        if (transition == 4 || transition == 6) {
            if (newId != 0) {
                incoming = music_allocTune(newId);
            }
            if (incoming != NULL || newId == 0) {
                if (newId != 0) {
                    MusicInfo *info;

                    info = &gMusicInfo[newId];
                    if ((info->flags & 0x20) && (gMusicInfo[oldId].flags & 0x20)) {
                        sync = 1;
                    }
                    incoming->syncTogether = sync;
                    if (info->flags & 8) {
                        incoming->vol1.intended = 0x8000;
                        incoming->vol1.current = 0;
                        incoming->vol1.rate = 60;
                    } else if (sync != 0) {
                        incoming->vol1.intended = 0x8000;
                        incoming->vol1.current = 0;
                        incoming->vol1.rate = 0x100;
                    }
                    incoming->syncTune = outgoing;
                    incoming->startSync = 2;
                    music_queueTune(incoming, NULL);
                }
                if (transition == 6) {
                    if (newId != 0) {
                        outgoing->volumeSlideSync = 2;
                    }
                    outgoing->syncTogether = 0;
                    outgoing->syncTune = outgoing;
                    outgoing->vol1.intended = 0;
                    if (gMusicInfo[oldId].flags & 8) {
                        outgoing->vol1.rate = 40;
                        return;
                    }
                    outgoing->vol1.rate = 160;
                    return;
                }
                incoming = music_allocTune(0);
                if (incoming != NULL) {
                    if (newId != 0) {
                        outgoing->volumeSlideSync = 2;
                    }
                    outgoing->syncTogether = 0;
                    outgoing->syncTune = outgoing;
                    outgoing->vol1.intended = 0;
                    if (sync != 0) {
                        outgoing->vol1.rate = 0x80;
                    } else {
                        outgoing->vol1.rate = 0x200;
                    }
                    incoming->startSync = 0x80;
                    incoming->syncTune = outgoing;
                    music_queueTune(incoming, outgoing);
                }
            }
            return;
        }
    } else if (oldId != 0) {
        outgoing = music_findTuneOrQueued(oldId);
        if (outgoing != NULL) {
            if (newId != 0) {
                bzero(outgoing, sizeof(MusicTrack));
                outgoing->player = -1;
                if (newId < 150) {
                    outgoing->fullVolume = gMusicInfo[newId].volume;
                } else {
                    outgoing->fullVolume = 26000;
                }
                outgoing->currentVolume = outgoing->fullVolume;
                outgoing->vol0.intended = outgoing->vol0.current = outgoing->vol1.current =
                    outgoing->vol1.intended = outgoing->vol2.current = outgoing->vol2.intended = 0x8000;
                outgoing->eventTypes = gDefaultEventTypes;
                outgoing->eventMasks = gDefaultEventMasks;
                outgoing->syncTune = outgoing;
            } else {
                outgoing->startSync = 0x80;
            }
            outgoing->seq_number = newId;
            return;
        }
    }
    if (newId != 0 || outgoing != NULL) {
        incoming = music_allocTune(newId);
    }
    if (incoming != NULL) {
        if (transition == 3) {
            incoming->vol1.intended = 0x8000;
            incoming->vol1.current = 0;
            incoming->vol1.rate = 0x400;
        } else if (transition == 2) {
            incoming->startSync = 1;
            incoming->syncTune = outgoing;
        } else if (transition == 1) {
            incoming->startSync = 2;
            incoming->syncTune = outgoing;
        } else if (transition == 5) {
            incoming->startSync = 0x80;
            incoming->syncTune = outgoing;
            if (outgoing != NULL) {
                outgoing->vol1.intended = 0;
                outgoing->vol1.rate = 0x200;
            }
        }
        music_queueTune(incoming, outgoing);
    }
}

/* Replaces sequence `id` (or, for id 0, every playing sequence) using transition `transition`. */
void music_stopTune(s32 id, s32 transition) {
    s32 count;
    s32 i;
    s32 ids[3];
    s32 playing;

    id &= 0xFFF;
    if (id == 0) {
        music_freeStoppedTunes();
        count = music_getPlayingIds(ids);
        for (i = 0; i < count; i++) {
            playing = ids[i];
            if (playing > 0) {
                music_playTune(0, playing, transition);
            }
        }
    } else {
        music_playTune(0, id, transition);
    }
}

/* Frees the pool tracks that are off a channel or whose channel stopped, and clears every queue. */
void music_freeStoppedTunes(void) {
    s32 i;
    s32 invalid = -1; // matching: IDO compares against -1 in this order only through a local
    s32 channel;

    for (i = 0; i < 12; i++) {
        channel = tuneArray[i].player;
        if (invalid == channel) {
            if (tuneArray[i].seq_number != invalid) {
                tuneArray[i].seq_number = invalid;
            }
        } else if (audio_getSeqState(channel) == 0) {
            gChannelTunes[tuneArray[i].player] = NULL;
            tuneArray[i].player = invalid;
            tuneArray[i].seq_number = invalid;
        }
        tuneArray[i].waitingTune = NULL;
    }
}

/* Sets the fade target of sequence `id`: frames 1 jumps there, 0 uses the default rate, 2 or
   more reach it in about that many frames. */
void music_setFade(s32 id, s32 volume, s32 frames) {
    MusicTrack *track;

    track = music_findTuneOrQueued(id);
    if (track != NULL) {
        track->vol0.intended = volume;
        if (frames == 1) {
            track->vol0.current = volume;
            if (track->player >= 0) {
                music_updateChannelVolume(track->player);
            }
        }
        if (frames >= 2) {
            s32 rate;

            rate = track->vol0.current - volume;
            if (rate < 0) {
                rate = -rate;
            }
            rate /= frames;
            if (rate <= 0) {
                rate = 2;
            } else if (rate >= 0x8000) {
                rate = 0x7FFF;
            }
            track->vol0.rate = rate;
        } else {
            track->vol0.rate = 0x200;
        }
    }
}

/* Pauses (pause != 0) or resumes sequence `id`. */
void music_pauseTune(s32 id, s32 pause) {
    MusicTrack *track;

    track = music_findTune(id);
    if (track != NULL) {
        if (2 == track->internallyPaused && pause == 0) {
            audio_playSeqIfStopped(track->player);
            track->internallyPaused = 0;
            track->currentVolume = -1;
            music_updateChannelVolume(track->player);
            return;
        }
        if (2 != track->internallyPaused && pause != 0) {
            if (track->internallyPaused != 1) {
                audio_pauseSeq(track->player);
            }
            track->internallyPaused = 2;
        }
    }
}

/* Returns whether sequence `id` exists with nothing queued behind it. */
s32 func_1000E0F8(s32 id) {
    MusicTrack *track;

    id &= 0xFFF;
    track = music_findTuneOrQueued(id);

    if (track != NULL && track->waitingTune == NULL) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/* Returns whether sequence `id` is of kind 1 or 3. */
s32 func_1000E134(s32 id) {
    s32 kind;

    if (id < 150) {
        kind = gMusicInfo[id].flags & ~0xF0;
        if (kind == 1 || kind == 3) {
            return TRUE;
        }
    }
    return FALSE;
}

#define MUSIC_KIND(track) (gMusicInfo[(track)->seq_number].flags & ~0xF0)

void func_1000E17C(void) {
    s32 i;
    MusicTrack *track;

    for (i = 0; i < 12; i++) {
        track = &tuneArray[i];
        if (track->seq_number > 0) {
            if ((1 == MUSIC_KIND(track) || 3 == MUSIC_KIND(track)) && track->player == -1) {
                track->seq_number = -1;
            }
        }
    }
    for (i = 0; i < 12; i++) {
        track = &tuneArray[i];
        if (track->seq_number > 0) {
            if (track->waitingTune != NULL && track->waitingTune->seq_number == -1) {
                track->waitingTune = NULL;
            }
            if (track->syncTune != NULL && track->syncTune->seq_number == -1) {
                track->syncTune = NULL;
            }
        }
    }
    for (i = 0; i < 12; i++) {
        track = &tuneArray[i];
        if (track->seq_number > 0) {
            if ((1 == MUSIC_KIND(track) || 3 == MUSIC_KIND(track)) && track->player != -1) {
                music_stopTune(track->seq_number, 4);
            }
        }
    }
}

/* Pauses (pause != 0) or resumes every playing channel; sequences with flag 0x10 are only muted. */
void music_pauseAll(s32 pause) {
    MusicTrack *track;
    s32 i;

    for (i = 0; i < 3; i++) {
        track = gChannelTunes[i];
        if (track == NULL) {
            continue;
        }
        if (track->seq_number <= 0) {
            continue;
        }
        if (track->internallyPaused != 0) {
            continue;
        }
        if (pause != 0) {
            audio_setSeqVolume(i, 0);
            if (gMusicInfo[gChannelTunes[i]->seq_number].flags & 0x10) {
                continue;
            }
            audio_pauseSeq(i);
        } else {
            if (!(gMusicInfo[track->seq_number].flags & 0x10)) {
                audio_playSeqIfStopped(i);
                track = gChannelTunes[i];
            }
            track->currentVolume = -1;
            music_updateChannelVolume(i);
        }
    }
    gMusicPaused = pause;
}

/* Sets the base volume (0..0x7FFF) of sequence `id`. */
void music_setVolume(s32 id, s32 volume) {
    MusicTrack *track;

    if (volume >= 0x8000) {
        volume = 0x7FFF;
    } else if (volume < 0) {
        volume = 0;
    }
    track = music_findTuneOrQueued(id);
    if (track != NULL) {
        if (track->player < 0) {
            track->currentVolume = volume;
        }
        track->fullVolume = volume;
    }
}

/* Sets the volume (percent) of the sequence's channels in channelMask, fading over `frames`
   when it is playing (frames < 0 sets it at once). Returns whether the sequence exists. */
s32 music_fadeChannelVolume(s32 id, s32 percent, s32 channelMask, s32 frames) {
    s32 i;
    MusicTrack *track;

    track = music_findTuneOrQueued(id);
    percent = (percent * 255) / 100;
    if (percent >= 256) {
        percent = 255;
    } else if (percent < 0) {
        percent = 0;
    }
    if (track != NULL) {
        if (track->player >= 0) {
            if (frames < 0) {
                audio_setChannelsFade(track->player, channelMask, percent);
                return TRUE;
            }
            audio_fadeChannelsVolume(track->player, channelMask, percent, frames);
            return TRUE;
        }
        if (percent == 0) {
            track->tuneOffChannels |= channelMask;
        } else if (percent > 0) {
            track->tuneOffChannels &= ~channelMask;
        }
        for (i = 0; channelMask != 0; i++, channelMask >>= 1) {
            if (channelMask & 1) {
                track->channelVolumes[i] = percent;
            }
        }
        return TRUE;
    }
    return FALSE;
}

/* Sets the volume (percent) of the sequence's channels in channelMask. */
s32 music_setChannelVolume(s32 id, s32 percent, s32 channelMask) {
    MusicTrack *track;

    track = music_findTuneOrQueued(id);
    if (track != NULL) {
        if (track->player >= 0) {
            if (percent > 100) {
                percent = 100;
            } else if (percent < 0) {
                percent = 0;
            }
            audio_setChannelsFade(track->player, channelMask, (percent * 255) / 100);
            return TRUE;
        }
        if (percent <= 0) {
            track->tuneOffChannels |= channelMask;
            return TRUE;
        }
        if (percent > 0) {
            track->tuneOffChannels &= ~channelMask;
            return TRUE;
        }
    }
    return FALSE;
}

/* Jumps sequence `id` to `marker`, at once or, with a waitMask, once syncId's sequence raises
   those marker bits. */
s32 music_jumpToMarker(s32 id, s32 marker, s32 waitMask, s32 syncId) {
    MusicTrack *track;
    MusicTrack *syncTo;

    track = music_findTuneOrQueued(id);
    syncTo = NULL;
    if (syncId >= 0) {
        syncTo = music_findTuneOrQueued(syncId);
    }

    if (track != NULL) {
        if (waitMask == 0) {
            if (track->player >= 0) {
                audio_jumpToSeqMarker(track->player, marker - 1);
                return TRUE;
            }
        }
        track->jumpPoint = marker;
        track->jumpSync = waitMask;
        if (syncTo != NULL) {
            track->syncTune = syncTo;
        }
        return TRUE;
    }
    return FALSE;
}

s32 func_1000E704(s32 id, s32 arg1, s32 arg2) {
    MusicTrack *track = music_findTune(id);

    if (track != NULL && track->player >= 0) {
        audio_setChannelsFxBus(track->player, arg2, arg1);
        return TRUE;
    }
    return FALSE;
}

void func_1000E75C(s32 arg0) {
    D_8002B070 = arg0 >> 1;
}

/* Reads the pending music mode request and its value (see music_postEvent); returns the flags. */
s32 music_getEventState(mode, value)
    s32 *mode;
    s32 *value;
{
    if (mode != NULL) {
        *mode = D_80041F08;
    }
    if (value != NULL) {
        *value = D_80041F0C;
    }
    return gMusicEventFlags;
}

/* Music event from the game: bit 0 sets flag 1, bit 1 adds a sample to the D_80041F08 /
   D_80041F0C average, bits 2 and 3 request a mode in D_80041F08, bit 4 sets flag 0x10. */
void music_postEvent(u32 event, s32 value) {
    if ((event & 1) == 1) {
        gMusicEventFlags |= 1;
        return;
    }
    if (event & 2) {
        D_80041F08 += value;
        D_80041F0C += 1;
        return;
    }
    if (event & 4) {
        D_80041F08 = value + 1;
        gMusicEventFlags |= 4;
        return;
    }
    if (event & 8) {
        D_80041F0C = value >> 8;
        value &= 0xFF;
        if (value == 0 || value == 4 || value == 5) {
            D_80041F08 = 2;
            return;
        }
        if (value == 10) {
            D_80041F08 = 1;
            return;
        }
        D_80041F08 = 3;
        return;
    }
    if (event & 0x10) {
        gMusicEventFlags |= 0x10;
    }
}

void music_clearEvent(s32 event) {
    if ((event & 1) == 1) {
        gMusicEventFlags &= ~1;
    }
}

/* Returns the marker bits sequence `id`'s channel raised this frame. */
u8 music_getMarkers(s32 id) {
    MusicTrack *track = music_findTune(id);

    if (track != NULL && track->player >= 0) {
        return gMarkerBits[track->player];
    } else {
        return 0;
    }
}

/* Resets the music system. */
void music_reset(void) {
    u32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 16; j++) {
            gMusicEventLevels[i][j] = 0x8000;
            gMusicEventDecay[i][j] = 0x100;
        }
        audio_stopSeq(i);
        gChannelTunes[i] = NULL;
        gMarkerInterval[i] = 0;
        gLastMarkerPos[i] = 0;
        D_80041880[i] = 0;
    }
    bzero(tuneArray, sizeof(tuneArray));
    gLastMarkerChannel = 0;
    for (k = 0; k < 12; k++) {
        tuneArray[k].seq_number = -1;
    }
}

u16 func_1000EA94(s32 arg0) {
    u16 id;

    if (arg0 == 0) {
        id = 82;
    } else if (arg0 == 2) {
        id = 81;
    } else if (arg0 == 1) {
        id = 83;
    }
    music_playTune(id, 0, 0);
    return id;
}
