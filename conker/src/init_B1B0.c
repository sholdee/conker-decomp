#include <ultra64.h>

/* The shared headers give this file's music tables and lookups placeholder types; their real
   declarations follow below. */
#define func_1000B1B0 func_1000B1B0_hdr
#define func_1000B1FC func_1000B1FC_hdr
#define func_1000DF68 func_1000DF68_hdr
#define D_8002B074 D_8002B074_hdr
#define D_8002B9D4 D_8002B9D4_hdr
#define D_8002B9F4 D_8002B9F4_hdr
#define D_800417B0 D_800417B0_hdr
#define D_800418B0 D_800418B0_hdr
#define D_800419A8 D_800419A8_hdr
#define D_800427F4 D_800427F4_hdr
#include "functions.h"
#include "variables.h"
#undef func_1000B1B0
#undef func_1000B1FC
#undef func_1000DF68
#undef D_8002B074
#undef D_8002B9D4
#undef D_8002B9F4
#undef D_800417B0
#undef D_800418B0
#undef D_800419A8
#undef D_800427F4

/* Per-frame update hook of a music sequence: takes and returns its private state word. */
typedef s32 (*MusicCallback)(s32 state, s32 channel, f32 x, f32 y, f32 z);

/* Settings of one music sequence, indexed by sequence id. */
typedef struct {
    /* 0x0 */ u16 volume;
    /* 0x2 */ u16 pad2;
    /* 0x4 */ s32 flags; /* low nibble (with 0xF0 masked off) is the sequence kind */
    /* 0x8 */ MusicCallback update;
    /* 0xC */ u8 unkC;
    /* 0xD */ u8 padD[3];
} MusicInfo; /* size 0x10 */

/* A volume that steps from current toward target by rate per frame (func_1000CD40). */
typedef struct {
    u16 current;
    u16 target;
    u16 rate;
} VolumeRamp;

typedef struct MusicTrack MusicTrack;

/* One music sequence instance, from the pool D_800419A8. */
struct MusicTrack {
    /* 0x00 */ s32 channel;          /* index into D_800417B0, -1 while not on a channel */
    /* 0x04 */ s32 id;               /* sequence id; -1 marks a free pool entry */
    /* 0x08 */ s32 *eventTypes;      /* how each of the 16 event bits behaves (func_1000CEAC) */
    /* 0x0C */ u16 *eventMasks;      /* event bits raised by a marker on each marker channel */
    /* 0x10 */ MusicTrack *syncTo;   /* track whose markers the wait masks refer to */
    /* 0x14 */ u8 followSync;
    /* 0x15 */ u8 pauseState;        /* 0 playing, 1 paused while silent, 2 paused on request */
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 startWait;        /* marker bits to wait for before replacing the old track */
    /* 0x1C */ s32 rampWait;         /* marker bits to wait for before stepping the ramps */
    /* 0x20 */ s32 jumpWait;         /* marker bits to wait for before the pending jump */
    /* 0x24 */ s32 pendingJump;      /* marker + 1 to jump to, 0 for none */
    /* 0x28 */ s32 pendingVolume;
    /* 0x2C */ s32 baseVolume;
    /* 0x30 */ s32 volume;           /* volume last sent to the channel */
    /* 0x34 */ s32 callbackState;
    /* 0x38 */ s32 mutedChannels;
    /* 0x3C */ u8 channelVolumes[16];
    /* 0x4C */ VolumeRamp fade;
    /* 0x52 */ VolumeRamp crossfade;
    /* 0x58 */ VolumeRamp duck;
    /* 0x5E */ u8 pad5E[2];
    /* 0x60 */ MusicTrack *next;     /* queued to take over this track's channel */
}; /* size 0x64 */

extern MusicInfo D_8002B074[];        /* indexed by sequence id (< 150) */
extern s32 D_8002B9D4[];              /* default eventTypes */
extern u16 D_8002B9F4[];              /* default eventMasks */
extern MusicTrack *D_800417B0[3];     /* track playing on each music channel */
extern s32 D_800417C0[3][16];         /* per-event decay rate */
extern s32 D_80041880[3];
extern s32 D_80041890[3];             /* last marker position */
extern s32 D_800418A0[3];             /* distance between the last two marker positions */
extern s32 D_800418B0[3][16];         /* per-event level */
extern u8 D_80041970[3][16];          /* per-event toggle */
extern MusicTrack D_800419A8[12];     /* track pool */
extern u16 D_800427F4;
extern u8 D_800C35E8;

MusicTrack *func_1000B1B0(s32 id);
MusicTrack *func_1000B1FC(s32 id);
void func_1000D96C(s32 newId, s32 oldId, s32 transition);
void func_1000DEC4(void);
void func_1000DF68(s32 id, s32 volume, s32 frames);
void func_10008C04(u8, u8, s32);
s32 func_10008CE8(u8, s32);
void func_1507E7E4(struct127 *, u8, u8, u16, s32);
void func_15178EFC(s32 arg0);
s32 func_151F2CDC(void);

/* Returns the track playing sequence `id` on a channel, or NULL. */
MusicTrack *func_1000B1B0(s32 id) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_800417B0[i] != NULL && id == D_800417B0[i]->id) {
            return D_800417B0[i];
        }
    }
    return NULL;
}

/* Like func_1000B1B0, but also finds a track queued to replace one of the playing tracks. */
MusicTrack *func_1000B1FC(s32 id) {
    s32 i;
    MusicTrack *next;

    for (i = 0; i < 3; i++) {
        if (D_800417B0[i] != NULL && id == D_800417B0[i]->id) {
            return D_800417B0[i];
        }
    }
    for (i = 0; i < 3; i++) {
        if (D_800417B0[i] != NULL) {
            next = D_800417B0[i]->next;
            if (next != NULL && id == next->id) {
                return next;
            }
        }
    }
    return NULL;
}

/* Points every track that syncs to `track` back at itself. */
void func_1000B294(MusicTrack *track) {
    s32 i;
    MusicTrack *next;

    for (i = 0; i < 3; i++) {
        if (D_800417B0[i] != NULL) {
            if (track == D_800417B0[i]->syncTo) {
                D_800417B0[i]->syncTo = D_800417B0[i];
            }
            next = D_800417B0[i]->next;
            if (next != NULL && track == next->syncTo) {
                next->syncTo = next;
            }
        }
    }
}

/* Takes a free track from the pool and initialises it for sequence `id`. */
MusicTrack *func_1000B2F4(s32 id) {
    s32 i;

    for (i = 0; i < 12; i++) {
        if (D_800419A8[i].id == -1) {
            bzero(&D_800419A8[i], sizeof(MusicTrack));
            D_800419A8[i].channel = -1;
            if (id < 150) {
                D_800419A8[i].baseVolume = D_8002B074[id].volume;
            } else {
                D_800419A8[i].baseVolume = 26000;
            }
            D_800419A8[i].volume = D_800419A8[i].baseVolume;
            D_800419A8[i].fade.target = D_800419A8[i].fade.current = D_800419A8[i].crossfade.current =
                D_800419A8[i].crossfade.target = D_800419A8[i].duck.current = D_800419A8[i].duck.target = 0x8000;
            D_800419A8[i].id = id;
            D_800419A8[i].eventTypes = D_8002B9D4;
            D_800419A8[i].eventMasks = D_8002B9F4;
            D_800419A8[i].syncTo = &D_800419A8[i];
            return &D_800419A8[i];
        }
    }
    return NULL;
}

/* Queues `track` behind `current`, or with no current track puts it on a free channel (via a
   placeholder track it then replaces) or in place of a channel's stopped queued track. */
void func_1000B3D4(MusicTrack *track, MusicTrack *current) {
    s32 i;
    MusicTrack *placeholder;
    MusicTrack **slot;
    MusicTrack *entry;
    MusicTrack *next;

    placeholder = NULL;
    // matching: current is read through its address so IDO reloads it from the arg slot
    if ((&current)[0] != NULL) {
        entry = (&current)[0]->next;
        if (entry != NULL && track != entry) {
            if (entry->id == track->id) {
                track->id = -1;
                return;
            }
            entry->id = -1;
        }
        (&current)[0]->next = track;
        return;
    }

    for (i = 0; i < 3; i++) {
        if (placeholder == NULL) {
            slot = &D_800417B0[i];
            entry = *slot;
            if (entry == NULL || (entry->id <= 0 && entry->next == NULL)) {
                entry = func_1000B2F4(0);
                placeholder = entry;
                if (entry != NULL) {
                    entry->channel = i;
                    entry->next = track;
                    *slot = entry;
                    continue;
                } else {
                    track->id = -1;
                    return;
                }
            }
        }
        if (placeholder == NULL) {
            slot = &D_800417B0[i];
            entry = *slot;
            if (entry != NULL) {
                next = entry->next;
                if (next != NULL && next->id == 0) {
                    placeholder = (MusicTrack *)-1; /* any non-NULL value ends the search */
                    func_1000B294(next);
                    (*slot)->next->id = -1;
                    (*slot)->next->channel = -1;
                    (*slot)->next = track;
                }
            }
        }
    }
}

/* Writes the ids of up to 3 tracks that are on a channel to `ids`; returns how many. */
s32 func_1000B548(s32 *ids) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < 12; i++) {
        if (D_800419A8[i].id != -1 && D_800419A8[i].channel != -1) {
            if (count < 3) {
                *ids++ = D_800419A8[i].id;
                count++;
            }
        }
    }
    return count;
}

s32 func_1000B638(s32 state, u8 channel, s32 arg2, s32 arg3) {
    s32 bit1;
    MusicTrack *track;

    bit1 = state & 2;
    state &= 1;
    track = D_800417B0[channel];
    if (track == NULL || track->volume < 500) {
        D_80041F04 &= ~1;
    }

    if (D_80041F04 & 1) {
        if (state == 0) {
            func_100088F0(channel, 0x8000, 1);
            if (D_800BE9F0 == 1 || D_800BE9F0 == 12) {
                func_10008790(channel, 0x7000, 0, 0);
            } else if (D_800BE9F0 != 7) {
                func_10008790(channel, 0xCA, 0, 0);
            }
            func_100085B8(channel, 15, 1);
        }
        state = 1;
    } else if (state != 0) {
        func_100088F0(channel, 0x8000, 0);
        if (D_800BE9F0 == 1 || D_800BE9F0 == 12) {
            func_10008790(channel, 0x7000, 255, 0);
        } else if (D_800BE9F0 != 7) {
            func_10008790(channel, 0xCA, 255, 0);
        }
        func_100085B8(channel, 15, 0);
        state = 0;
    }

    if (D_800BE9F0 == 39) {
        func_10011FA0((s32 *)4);
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

s32 func_1000B830(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 flag = D_800DBFF0->unk5F0 & 1;

    if (flag != 0 && arg0 == 0) {
        arg0 = 1;
        func_1000E40C(16, 1000);
    } else if (flag == 0 && arg0 != 0) {
        arg0 = 0;
        func_1000E40C(16, 18000);
    }
    return arg0;
}

s32 func_1000B8B8(s32 state, u8 channel, f32 x, f32 y, f32 z) {
    s32 level;

    if (D_800BE9F0 == 4) {
        if ((state & 1) && D_80041F0C == 0) {
            func_1000E46C(19, 0, 0x1000, 0);
            state &= ~1;
        } else if (D_80041F0C != 0) {
            /* D_80041F08 / D_80041F0C: average of the values func_1000E7A0 reported */
            level = D_80041F08 / D_80041F0C / 80;
            if (level > 100) {
                level = 100;
            }
            func_1000E588(19, level, 0x1000);
            state |= 1;
        }
        D_80041F08 = 0;
        D_80041F0C = 0;
        if (!(state & 2)) {
            func_1000DF68(19, 0, 1);
            func_1000DF68(19, 0x8000, 0);
            state |= 2;
        }
        return state;
    }
    return func_1000C530(state, channel, x, y, z);
}

/* The top byte of the state holds this track's positional volume; the rest is func_1000C530's. */
s32 func_1000BA18(u32 state, u8 channel, f32 x, f32 y, f32 z) {
    u32 volume;
    s32 pan;
    s32 innerState;

    volume = 0;
    innerState = state & 0xFFFFFF;
    state &= 0xFF000000;
    func_100114D0(0, -377, 8227, 0x7FFF, 4000, 3000, &pan, &volume, NULL);
    volume = (volume << 16) & 0xFF000000;
    if (state != volume) {
        state = volume;
        func_1000E588(77, state >> 24, 0x6000);
    }
    // matching: the u64 round trip steers IDO's temp register for the result
    volume = (u64)(func_1000C530(innerState, channel, x, y, z) & 0xFFFFFF);
    return state | volume;
}

/* As func_1000BA18, with the volume inverted (louder further away). */
s32 func_1000BAFC(u32 state, u8 channel, f32 x, f32 y, f32 z) {
    u32 volume;
    s32 pan;
    s32 innerState;

    volume = 0;
    innerState = state & 0xFFFFFF;
    state &= 0xFF000000;
    func_100114D0(0, 0, 0, 0x7FF8, 3700, 2600, &pan, &volume, NULL);
    volume = ((0x7FFF - volume) << 16) & 0xFF000000;
    if (state != volume) {
        state = volume;
        func_1000E588(147, state >> 24, 0x6000);
    }
    // matching: the u64 round trip steers IDO's temp register for the result
    volume = (u64)(func_1000C530(innerState, channel, x, y, z) & 0xFFFFFF);
    return state | volume;
}

s32 func_1000BBE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg0 == 0) {
        func_1000E704(20, 1, 0xFFFF);
        arg0 = 1;
    }
    return arg0;
}

s32 func_1000BC28(s32 arg0, u8 arg1, s32 arg2, s32 arg3) {
    s32 level = func_10008A4C(arg1, 0) + func_10008A4C(arg1, 6) + 1;

    if (level >= 256) {
        level = 255;
    } else if (level < 16) {
        level = 1;
    }
    if (level != arg0) {
        func_150C851C(level - 1);
        arg0 = level;
    }
    return arg0;
}

s32 func_1000BCBC(s32 state, u8 channel, f32 x, f32 y, f32 z) {
    f32 distance;
    u8 volume;

    // matching: channel's address is taken so IDO keeps it in its arg slot
    if (state == 0) {
        func_10008790(*&channel, 3, 16, 0);
        func_1000886C(channel, 4, 0);
        state = 1;
    } else if (D_800BE9F0 == 19) {
        x -= 24.0f;
        z -= 3015.0f;
        distance = x * x + z * z;
        if (36000000.0f < distance) {
            volume = 4;
        } else {
            volume = (u8)((6000.0f - sqrtf(distance)) * 0.041833334f) + 4;
        }
        if (state != volume) {
            func_1000886C(channel, 3, volume);
        }
        if (func_150A29C8(0, 0x4041) == 0) {
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
        if (volume != func_10008A4C(channel, 2)) {
            func_1000886C(channel, 4, volume);
        }
    }
    return state;
}

u32 func_1000BF60(u32 state, u8 channel, f32 x, f32 y, f32 z) {
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
        func_1000E40C(34, 24000);
        if (D_800C35EA == 1) {
            func_1000DF68(34, 20, 1);
        }
        state = 0x80;
        started = 1;
    }

    if (D_800BE9F0 != 29) {
        func_10008F24(channel);
        return state;
    }

    volumeChanged = 0;
    panChanged = 0;
    func_100114D0(-351, 0, 407, 0x7FFF, 3000, 300, &pan, &volume, NULL);

    if (volume < 0x4000) {
        volume = 0x40;
    } else {
        volume >>= 8;
    }

    if (volume != (oldMix >> 8)) {
        volumeChanged = 1;
        func_1000886C(channel, 6, volume);
    }

    if (pan != (oldMix & 0xFF)) {
        panChanged = 1;
        func_10008744(channel, 1, pan & 0x7F);
        func_10008744(channel, 2, pan & 0x7F);
        func_100086FC(channel, 1, pan >> 7);
        func_100086FC(channel, 2, pan >> 7);
    }

    oldMix = (pan << 8) | (volume << 16);

    func_100114D0(-64, 0, 543, 0x7FFF, 3000, 300, &pan, &volume, NULL);
    if (volumeChanged) {
        if (volume < 0x4000) {
            volume = 0x4000;
        }
        func_1000886C(channel, 1, volume >> 8);
    }

    if (panChanged) {
        func_10008744(channel, 0, 0x40);
        func_100086FC(channel, 0, 0);
    }

    func_100114D0(-294, 0, 656, 0x7FFF, 3000, 300, &pan, &volume, NULL);
    if (volumeChanged) {
        if (volume < 0x4000) {
            volume = 0x4000;
        }
        func_1000886C(channel, 24, volume >> 8);
    }

    if (panChanged) {
        func_10008744(channel, 3, pan & 0x7F);
        func_10008744(channel, 4, pan & 0x7F);
        func_100086FC(channel, 3, pan >> 7);
        func_100086FC(channel, 4, pan >> 7);
    }

    mode = D_80041F08;
    if (oldMode != mode) {
        switch (mode) {
        case 1:
            countdown = state & 0x7C;
            if (countdown != 0) {
                countdown -= (D_800BE9E4 >> 1) << 2;
                if (countdown <= 0) {
                    func_1000E704(34, 0, 0xFFFF);
                    state = 0x81;
                } else {
                    state = countdown | 0x80;
                }
            }
            func_1000E46C(34, 100, 0xFE0, 0);
            break;
        case 2:
            state = 0xF8;
            func_10011FA0((s32 *)4);
            func_1000E704(34, 1, 0xFFFF);
            func_1000E46C(34, 0, 0xFE0, started);
            break;
        }
    }

    return state | oldMix;
}

s32 func_1000C350(s32 state, u8 channel, f32 x, f32 y, f32 z) {
    if (!(state & 0x80)) {
        state |= 0x80;
        if (D_800C35EA != 1) {
            func_1000886C(channel, 30, 1);
            func_1000886C(channel, 1, 1);
            func_1000E40C(35, 25000);
        } else if (D_800C35E8 == 3) {
            func_1000E40C(35, 250);
            func_15178EFC(2);
        } else if (D_800C35E8 == 6) {
            func_1000886C(channel, 30, 1);
            func_1000886C(channel, 1, 0x40);
            func_15178EFC(2);
        } else {
            func_1000E40C(35, 25000);
        }
        return state;
    }
    if (D_800BE9F0 != 29) {
        func_10008F24(channel);
    } else if (D_80041F08 != (state & 0x7FU)) {
        switch (D_80041F08) {
        case 1:
            func_10008790(channel, 30, 0, 0);
            func_10008790(channel, 1, 0x40, 0);
            break;
        case 2:
            func_10008790(channel, 24, 255, 0);
            func_10008790(channel, 6, 0, 0);
            func_10008790(channel, 1, 1, 0);
            break;
        }
        state = D_80041F08 | 0x80;
    }
    return state;
}

/* Default sequence update. State word: bits 0-1 marker mode, 2-7 countdown, 8-15 and 16-23 the
   mode's two parameters, 24-31 fade-out level. */
s32 func_1000C530(s32 state, u8 channel, f32 x, f32 y, f32 z) {
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
    low = oldLow; high = oldHigh;
    if (D_80041F08 != 0 &&
        (oldMode != 2 || countdown == 0 || D_80041F08 == 2)) {
        mode = D_80041F08;
        low = D_80041F0C & 0xFF;
        high = D_80041F0C >> 8;
        countdown = 30;
    }
    if (countdown != 0) {
        countdown -= D_800BE9E4;
        if (countdown <= 0) {
            countdown = 0;
            mode = 0;
        }
    }
    if (mode != oldMode) {
        if (oldMode != 0) {
            // matching: channel's address is taken so IDO keeps it in its arg slot
            func_100085F8(*&channel, oldMode + 9);
        }
        if (mode != 0) {
            func_10008824(channel, mode + 9, low);
            func_100086FC(channel, mode + 9, high >> 7);
            func_10008744(channel, mode + 9, high & 0x7F);
        }
    } else if (oldMode != 0 && D_80041F08 != 0 && upper != D_80041F0C) {
        if (low != oldLow) {
            func_10008824(channel, oldMode + 9, low);
        }
        if (high != oldHigh) {
            if ((high ^ oldHigh) & 0x80) {
                func_100086FC(channel, mode + 9, high >> 7);
            }
            func_10008744(channel, mode + 9, high & 0x7F);
        }
    }
    if (D_80041F04 & 0x10) {
        D_80041F04 &= ~0x10;
        if (fade == 0) {
            func_1000886C(channel, 0xC0, 0x80);
        }
        fade = 0xFF000000;
    }
    if (fade != 0) {
        upper = ((u32)D_800BE9E4 << 23) & 0xFF000000;
        if (upper < fade) {
            fade -= upper;
        } else {
            func_10008790(channel, 0xC0, 0, 90);
            fade = 0;
        }
    }
    D_80041F08 = 0;
    return (countdown * 4) | mode | (low << 8) | (high << 16) | fade;
}

s32 func_1000C7E8(s32 state, s32 channel, f32 x, f32 y, f32 z) {
    f32 distance;
    f32 volume;

    if (D_800BE9F0 == 49) {
        if (state != 2) {
            if (func_1000B1B0(9) == NULL) {
                func_1000E704(62, 0, 0xFFFF);
                func_1000E40C(62, 0x7FFF);
                func_1000D96C(61, 62, 4);
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
    distance = volume;
    if (distance < 100.0f) {
        volume = 100.0f;
    } else if (24000.0f < distance) {
        volume = 24000.0f;
    }
    func_1000E40C(62, (s32)volume);
    return state;
}

s32 func_1000C934(s32 state, s32 channel, s32 arg2, s32 arg3) {
    s32 volume;
    s32 maxVolume;
    s32 flag;

    volume = 0;
    flag = D_800DBFF0->unk5F0 & 1;
    if (flag != 0) {
        maxVolume = 0x7FFF;
    } else {
        maxVolume = 12000;
    }
    if (D_800BE9F0 == 55 && flag == 0) {
        func_100114D0(2200, 1066, -1600, maxVolume, 3000, 1500, NULL, &volume, NULL);
        volume &= 0xFF00;
        volume = maxVolume - volume;
    }
    if ((volume != state) & 0xFFFF) {
        func_1000E40C(84, volume);
    }
    return volume | 0x80000000;
}

s32 func_1000CA18(s32 state, s32 channel, s32 arg2, s32 arg3) {
    s32 volume;

    if (D_800BE9F0 == 55) {
        if (D_800DBFF0->unk5F0 & 1) {
            volume = 0;
        } else {
            func_100114D0(2200, 1066, -1600, 24000, 3000, 1500, NULL, &volume, NULL);
            volume &= 0xFF00;
        }
    } else {
        volume = 24000;
    }

    if ((volume != state) & 0xFFFF) {
        func_1000E40C(84, volume);
    }
    return volume | 0x80000000;
}

s32 func_1000CAE4(s32 state, u8 channel, f32 x, f32 y, f32 z) {
    s32 bit1;

    bit1 = state & 2;
    state &= 1;
    if (D_800BE9F0 == 66) {
        func_10011FA0((s32 *)4);
        if (state == 0) {
            state = 1;
            func_1000E704(88, 1, 0xFFFF);
        }
    } else if (state != 0) {
        func_1000E704(88, 0, 0xFFFF);
        func_1000E40C(88, 16000);
        state = 0;
    }
    if (bit1 == 0) {
        func_10008790(channel, 0x1000, 0, 1);
        bit1 = 2;
    }
    return bit1 | state;
}

/* Starts fading the first two channels to `volume`. */
void func_1000CBA8(s32 volume) {
    if (D_800417B0[0] != NULL) {
        D_800417B0[0]->fade.target = volume;
        D_800417B0[0]->fade.rate = 0x500;
    }
    if (D_800417B0[1] != NULL) {
        D_800417B0[1]->fade.target = volume;
        D_800417B0[1]->fade.rate = 0x500;
    }
}

/* Sets the duck ramp of each channel in channelMask; a rate of 0 applies the target at once. */
void func_1000CBF0(s32 target, s32 rate, s32 channelMask) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if ((1 << i) & channelMask) {
            if (D_800417B0[i] != NULL) {
                D_800417B0[i]->duck.target = target;
                D_800417B0[i]->duck.rate = rate;
                if (rate == 0) {
                    D_800417B0[i]->duck.current = target;
                }
            }
        }
    }
}

/* Recomputes the channel's volume from its ramps and base volume and sends it if it changed. */
void func_1000CC54(s32 channel) {
    s32 volume;
    MusicTrack *track;

    track = D_800417B0[channel];
    if (track != NULL) {
        volume = (((u32)(track->duck.current * ((u32)(track->fade.current * track->crossfade.current) >> 15)) >> 15) *
                  track->baseVolume) >> 15;
        if (volume != track->volume) {
            if (track->volume == 0) {
                func_10008988(channel, track->mutedChannels ^ 0xFFFF, 1);
            } else if (volume == 0) {
                func_10008988(channel, track->mutedChannels ^ 0xFFFF, 0);
            }
            track->volume = volume;
            func_10008EE0(channel, volume);
        }
    }
}

/* Steps `current` toward `target` by rate per frame. */
s32 func_1000CD40(s32 current, s32 target, s32 rate) {
    if (target != current) {
        rate *= D_800BE9E4;
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
s32 func_1000CDA0(u8 waitMask, MusicTrack *syncTo) {
    if (waitMask == 0) {
        return TRUE;
    }
    if (syncTo != NULL) {
        if (syncTo->channel >= 0) {
            if (D_800417B0[syncTo->channel] == NULL || syncTo->id <= 0) {
                return TRUE;
            }
            if (func_1000853C(syncTo->channel) == 3) {
                return TRUE;
            }
            if (!(D_8002B074[syncTo->id].flags & 0x20)) {
                D_800418AC[syncTo->channel] |= 3;
            }
            waitMask &= ~D_800418AC[syncTo->channel];
        } else {
            return TRUE;
        }
    } else {
        return TRUE;
    }
    return waitMask == 0;
}

/* Drains the channel's marker messages into D_800418AC and the per-event tables, then updates
   the channel's paused state and decays the event levels. */
void func_1000CEAC(s32 channel) {
    s32 msg;
    s32 *types;
    s32 rate;
    u32 markerChannel;
    u16 bits;
    u16 *masks;
    s32 i;

    if (D_800417B0[channel] != NULL) {
        types = D_800417B0[channel]->eventTypes;
        masks = D_800417B0[channel]->eventMasks;
        D_800418AC[channel] = 0;
        while (osRecvMesg(&D_80041E58[channel], (OSMesg *)&msg, OS_MESG_NOBLOCK) == 0) {
            markerChannel = msg & 7;
            if (msg & 0x10) {
                if (markerChannel < 2) {
                    rate = 1300;
                    msg >>= 5;
                    if (D_80041890[channel] != 0) {
                        D_800418A0[channel] = msg - D_80041890[channel];
                    }
                    D_80041890[channel] = msg;
                    D_80041880[channel] = msg;
                }
                if (masks[markerChannel] != 0) {
                    bits = masks[markerChannel];
                    D_800418AC[channel] |= bits & 0x7F;
                    for (i = 0; i < 16 && bits; i++, bits >>= 1) {
                        if (bits & 1) {
                            switch (types[i]) {
                            case 0:
                                D_80041970[channel][i] ^= 1;
                                D_800418B0[channel][i] = 0x8000;
                                D_800417C0[channel][i] = rate;
                                break;
                            case 1:
                                D_80041970[channel][i] ^= 1;
                                rate >>= 1;
                                if (D_80041970[channel][i] == 0) {
                                    D_800418B0[channel][i] = rate * D_800BE9E4 + 0x8000;
                                }
                                D_800417C0[channel][i] = rate;
                                break;
                            case 2:
                                D_80041970[channel][i & 7] ^= 1;
                                D_80041970[channel][i | 8] = D_80041970[channel][i & 7] ^ 1;
                                D_800418B0[channel][i & 7] = 0x8000;
                                D_800418B0[channel][i | 8] = 0x8000;
                                D_800417C0[channel][i] = 0;
                                break;
                            default:
                                D_80041970[channel][i] |= 1;
                                D_800418B0[channel][i] = rate * D_800BE9E4;
                                D_800417C0[channel][i] = rate;
                                break;
                            }
                        }
                    }
                }
                D_800419A0 = channel;
            } else if (D_80041F04 & 1) {
                func_1507E7E4(D_800CC2D0, 0x44, 1, 0x12, 5);
            }
        }
        if (D_800417B0[channel]->volume == 0) {
            D_800418AC[channel] |= 0x80;
            if (D_800417B0[channel]->pauseState == 0) {
                func_10008F58(channel);
                D_800417B0[channel]->pauseState = 1;
            }
        } else if (D_800417B0[channel]->pauseState == 1) {
            func_100084D8(channel);
            D_800417B0[channel]->pauseState = 0;
        }
        for (i = 0; i < 16; i++) {
            if (D_800418B0[channel][i] != 0) {
                D_800418B0[channel][i] -= D_800417C0[channel][i] * D_800BE9E4;
                if (D_800418B0[channel][i] < 0) {
                    D_800418B0[channel][i] = 0;
                }
            }
        }
    }
}

/* Per-frame update of one music channel: pending jumps, stopping and handing the channel over
   to the queued track, the sequence's update callback and the volume ramps. */
void func_1000D2F8(s32 index, f32 x, f32 y, f32 z) {
    s32 status;
    MusicTrack **slot;
    MusicTrack *track;
    s32 channel;
    s32 offset;
    MusicTrack *next;
    MusicTrack *syncTo;
    MusicCallback callback;
    s32 i;

    channel = (u8)((u32 *)&index)[0];
    status = func_1000853C((u8)((u32 *)&index)[0]);
    offset = (s32)((u32 *)&index)[0] * sizeof(*slot);
    slot = (MusicTrack **)&(*(u8 (*)[])D_800417B0)[offset];
    track = *slot;
    if (track != NULL) {
        if (track->pendingJump != 0 && func_1000CDA0(track->jumpWait, track->syncTo)) {
            func_10008C6C(((u8 *)&channel)[3], (*slot)->pendingJump - 1);
            track = *slot;
            if (((u32)track->pendingVolume) != 0) {
                track->volume = ((u32)track->pendingVolume);
                (*slot)->baseVolume = ((u32)track->pendingVolume);
                track = *slot;
            }
            track->fade.current = 0x7FFF;
            (*slot)->fade.target = 0x7FFF;
            func_1000CC54((s32)((u32 *)&index)[0]);
            (*slot)->pendingJump = 0;
            (*slot)->jumpWait = 0;
        }
        track = *slot;
        if (status == 0) {
            track->id = 0;
            track = *slot;
        }
        next = track->next;
        if (next != NULL) {
            if (track->id == 0) {
                if (func_1000CDA0(next->startWait, next->syncTo)) {
                    (*slot)->id = -1;
                    func_1000B294(*slot);
                    next = (*slot)->next;
                    if (next->id > 0) {
                        if (func_10008CE8(channel, next->id) == -1) {
                            (*slot)->next->syncTo = NULL;
                            (*slot)->next->startWait = 0;
                            func_1000B3D4((*slot)->next, NULL);
                            (*slot)->next = NULL;
                            *slot = NULL;
                        } else {
                            *slot = (*slot)->next;
                            (*slot)->channel = (s32)((u32 *)&index)[0];
                            (*slot)->startWait = 0;
                            *(s32 *)((u8 *)D_800418A0 + offset) = 0;
                            *(s32 *)((u8 *)D_80041890 + offset) = 0;
                            *(s32 *)((u8 *)D_80041880 + offset) = 0;
                            track = *slot;
                            if (D_8002B074[track->id].unkC != 0) {
                                func_10008C04(channel, D_8002B074[track->id].unkC, 100);
                                track = *slot;
                            }
                            if (track->pendingJump != 0) {
                                func_10008C6C(channel, track->pendingJump - 1);
                                (*slot)->pendingJump = 0;
                                track = *slot;
                            }
                            track->volume = 0;
                            func_1000CC54((s32)((u32 *)&index)[0]);
                            func_100084D8(channel);
                            if ((*slot)->mutedChannels != 0) {
                                func_100088F0(channel, (*slot)->mutedChannels, 0);
                            }
                            i = 0;
                            do {
                                if ((*slot)->channelVolumes[i] != 0) {
                                    func_10008660(channel, i, (*slot)->channelVolumes[i], 1);
                                    (*slot)->channelVolumes[i] = 0;
                                }
                                i++;
                            } while (i != 16);
                        }
                    } else {
                        next->id = -1;
                        (*slot)->next->syncTo = NULL;
                        (*slot)->next->startWait = 0;
                        (*slot)->channel = -1;
                        *slot = NULL;
                    }
                }
            } else if (func_1000CDA0(next->startWait, next->syncTo)) {
                (*slot)->pauseState = 0;
                func_10008F24(channel);
            }
            track = *slot;
        } else if (track->id == 0) {
            func_1000B294(track);
            track = NULL;
            (*slot)->id = -1;
            (*slot)->channel = -1;
            *slot = NULL;
        }
        if (track != NULL) {
            if (track->id > 0) {
                callback = D_8002B074[track->id].update;
                if (callback != NULL) {
                    (*slot)->callbackState = callback(track->callbackState, (u8)channel, x, y, z);
                    track = *slot;
                }
            }
            if (func_1000CDA0(track->rampWait, track->syncTo)) {
                track = *slot;
                (*slot)->fade.current = func_1000CD40(track->fade.current, track->fade.target, track->fade.rate);
                track = *slot;
                (*slot)->crossfade.current =
                    func_1000CD40(track->crossfade.current, track->crossfade.target, track->crossfade.rate);
                track = *slot;
                (*slot)->duck.current = func_1000CD40(track->duck.current, track->duck.target, track->duck.rate);
                (*slot)->rampWait = 0;
            }
            func_1000CC54((s32)((u32 *)&index)[0]);
            track = *slot;
            if (track->followSync != 0 &&
                ((syncTo = track->syncTo) == NULL || syncTo == track || syncTo->id <= 0 ||
                 track->id <= 0 || syncTo->channel < 0)) {
                track->followSync = 0;
                track = *slot;
                track->syncTo = track;
            }
        }
    }
}

/* Per-frame music update: ducks the channels by the kinds of sequence playing, then runs
   func_1000CEAC and func_1000D2F8 on every channel. */
void func_1000D758(f32 x, f32 y, s32 z) {
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
    if (D_80041F00 == 0) {
        infos = D_8002B074;
        for (i = 0; i < 3; i++) {
            track = D_800417B0[i];
            if (track != NULL) {
                id = track->id;
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
            func_1000CBF0(6000, 0x400, kind5Mask ^ 0xFF ^ noDuckMask);
            func_1000CBF0(0x8000, 0x6400, kind5Mask);
        } else if (kind34Mask != 0) {
            func_1000CBF0(500, 0x400, kind34Mask ^ 0xFF);
            func_1000CBF0(0x8000, 0x800, kind34Mask);
        } else if (func_151F2CDC() == 1 &&
                   (D_800427F4 < 125 || D_800427F4 >= 129) &&
                   D_800427F4 < 457 && D_800427F4 != 368 &&
                   D_800427F4 != 369) {
            func_1000CBF0(14000, 0x200, kind12Mask ^ 0xFF);
        } else {
            func_1000CBF0(0x8000, 0x800, 0xFF);
        }
        i = 0;
        do {
            func_1000CEAC(i);
            i++;
        } while (i < 3);
        i = 0;
        do {
            func_1000D2F8(i, x, y, *(f32 *)&z);
            i++;
        } while (i != 3);
    }
}

/* Starts sequence newId, replacing oldId with the given transition (0 for none):
   1/2 start once oldId's sequence raises a marker, 3 fades in, 4/6 crossfade, 5 fades out oldId. */
void func_1000D96C(s32 newId, s32 oldId, s32 transition) {
    MusicTrack *outgoing;
    MusicTrack *incoming;
    s32 sync;

    outgoing = NULL;
    incoming = NULL;
    sync = 0;
    newId &= 0xFFF;
    oldId &= 0xFFF;
    if (newId != 0 && func_1000B1FC(newId) != NULL) {
        MusicTrack *track;
        MusicTrack *next;

        track = func_1000B1B0(newId);
        if (track == NULL) {
            return;
        }
        next = track->next;
        if (next != NULL) {
            if (next->channel == -1) {
                track->crossfade.target = 0x8000;
                track->crossfade.rate = 0x400;
                next->id = -1;
                track->next->channel = -1;
                track->next = NULL;
            }
            newId = 0;
            if (oldId == 0) {
                return;
            }
            goto lookup_outgoing;
        } else if (track->pauseState != 0 || track->crossfade.target == 0) {
            track->crossfade.target = 0x8000;
            track->crossfade.current = 0;
            track->crossfade.rate = 0x400;
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
        outgoing = func_1000B1B0(oldId);
    }
    if (outgoing != NULL) {
        if (outgoing != NULL && outgoing->next != NULL) {
            if (newId != 0) {
                return;
            }
        } else if (transition == 4 || transition == 6) {
            if (newId != 0) {
                incoming = func_1000B2F4(newId);
            }
            if (incoming != NULL || newId == 0) {
                if (newId != 0) {
                    MusicInfo *info;

                    info = &D_8002B074[newId];
                    if ((info->flags & 0x20) && (D_8002B074[oldId].flags & 0x20)) {
                        sync = 1;
                    }
                    incoming->followSync = sync;
                    if (info->flags & 8) {
                        incoming->crossfade.target = 0x8000;
                        incoming->crossfade.current = 0;
                        incoming->crossfade.rate = 60;
                    } else if (sync != 0) {
                        incoming->crossfade.target = 0x8000;
                        incoming->crossfade.current = 0;
                        incoming->crossfade.rate = 0x100;
                    }
                    incoming->syncTo = outgoing;
                    incoming->startWait = 2;
                    func_1000B3D4(incoming, NULL);
                }
                if (transition == 6) {
                    if (newId != 0) {
                        outgoing->rampWait = 2;
                    }
                    outgoing->followSync = 0;
                    outgoing->syncTo = outgoing;
                    outgoing->crossfade.target = 0;
                    if (D_8002B074[oldId].flags & 8) {
                        outgoing->crossfade.rate = 40;
                        return;
                    }
                    outgoing->crossfade.rate = 160;
                    return;
                }
                incoming = func_1000B2F4(0);
                if (incoming != NULL) {
                    if (newId != 0) {
                        outgoing->rampWait = 2;
                    }
                    outgoing->followSync = 0;
                    outgoing->syncTo = outgoing;
                    outgoing->crossfade.target = 0;
                    if (sync != 0) {
                        outgoing->crossfade.rate = 0x80;
                    } else {
                        outgoing->crossfade.rate = 0x200;
                    }
                    incoming->startWait = 0x80;
                    incoming->syncTo = outgoing;
                    func_1000B3D4(incoming, outgoing);
                }
            }
        } else {
            goto simple_transition;
        }
        return;
    } else {
        goto fallback_lookup;
    }
fallback_lookup:
    if (oldId != 0) {
        outgoing = func_1000B1FC(oldId);
        if (outgoing != NULL) {
            if (newId != 0) {
                bzero(outgoing, sizeof(MusicTrack));
                outgoing->channel = -1;
                if (newId < 150) {
                    outgoing->baseVolume = D_8002B074[newId].volume;
                } else {
                    outgoing->baseVolume = 26000;
                }
                outgoing->volume = outgoing->baseVolume;
                outgoing->fade.target = outgoing->fade.current = outgoing->crossfade.current =
                    outgoing->crossfade.target = outgoing->duck.current = outgoing->duck.target = 0x8000;
                outgoing->eventTypes = D_8002B9D4;
                outgoing->eventMasks = D_8002B9F4;
                outgoing->syncTo = outgoing;
            } else {
                outgoing->startWait = 0x80;
            }
            outgoing->id = newId;
            return;
        }
    }
simple_transition:
    if (newId != 0 || outgoing != NULL) {
        incoming = func_1000B2F4(newId);
    }
    if (incoming != NULL) {
        if (transition == 3) {
            incoming->crossfade.target = 0x8000;
            incoming->crossfade.current = 0;
            incoming->crossfade.rate = 0x400;
        } else if (transition == 2) {
            incoming->startWait = 1;
            incoming->syncTo = outgoing;
        } else if (transition == 1) {
            incoming->startWait = 2;
            incoming->syncTo = outgoing;
        } else if (transition == 5) {
            incoming->startWait = 0x80;
            incoming->syncTo = outgoing;
            if (outgoing != NULL) {
                outgoing->crossfade.target = 0;
                outgoing->crossfade.rate = 0x200;
            }
        }
        func_1000B3D4(incoming, outgoing);
    }
}

/* Replaces sequence `id` (or, for id 0, every playing sequence) using transition `transition`. */
void func_1000DE1C(s32 id, s32 transition) {
    s32 count;
    s32 i;
    s32 ids[3];
    s32 playing;

    id &= 0xFFF;
    if (id == 0) {
        func_1000DEC4();
        count = func_1000B548(ids);
        for (i = 0; i < count; i++) {
            playing = ids[i];
            if (playing > 0) {
                func_1000D96C(0, playing, transition);
            }
        }
    } else {
        func_1000D96C(0, id, transition);
    }
}

/* Frees the pool tracks that are off a channel or whose channel stopped, and clears every queue. */
void func_1000DEC4(void) {
    MusicTrack *track;
    s32 invalid = -1;
    s32 channel;
 track = D_800419A8; do { channel = track->channel; if (invalid == channel) { if (track->id != invalid) { track->id = invalid;
      }
    }
    else
      if (func_1000853C(channel) == 0)
    {
      D_800417B0[track->channel] = NULL;
      track->channel = invalid;
      track->id = invalid;
    }
    track++;
    track[-1].next = NULL;
  }
  while (track != (MusicTrack *)D_80041E58);
}

/* Sets the fade target of sequence `id`: frames 1 jumps there, 0 uses the default rate, 2 or
   more reach it in about that many frames. */
void func_1000DF68(s32 id, s32 volume, s32 frames) {
    MusicTrack *track;

    track = func_1000B1FC(id);
    if (track != NULL) {
        track->fade.target = volume;
        if (frames == 1) {
            track->fade.current = volume;
            if (track->channel >= 0) {
                func_1000CC54(track->channel);
            }
        }
        if (frames >= 2) {
            s32 rate;

            rate = track->fade.current - volume;
            if (rate < 0) {
                rate = -rate;
            }
            rate /= frames;
            if (rate <= 0) {
                rate = 2;
            } else if (rate >= 0x8000) {
                rate = 0x7FFF;
            }
            track->fade.rate = rate;
        } else {
            track->fade.rate = 0x200;
        }
    }
}

/* Pauses (pause != 0) or resumes sequence `id`. */
void func_1000E054(s32 id, s32 pause) {
    MusicTrack *track;

    track = func_1000B1B0(id);
    if (track != NULL) {
        if (2 == track->pauseState && pause == 0) {
            func_100084D8(track->channel);
            track->pauseState = 0;
            track->volume = -1;
            func_1000CC54(track->channel);
            return;
        }
        if (2 != track->pauseState && pause != 0) {
            if (track->pauseState != 1) {
                func_10008F58(track->channel);
            }
            track->pauseState = 2;
        }
    }
}

/* Returns whether sequence `id` exists with nothing queued behind it. */
s32 func_1000E0F8(s32 id) {
    MusicTrack *track;

    id &= 0xFFF;
    track = func_1000B1FC(id);

    if (track != NULL && track->next == NULL) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/* Returns whether sequence `id` is of kind 1 or 3. */
s32 func_1000E134(s32 id) {
    s32 kind;

    if (id < 150) {
        kind = D_8002B074[id].flags & ~0xF0;
        if (kind == 1 || kind == 3) {
            return TRUE;
        }
    }
    return FALSE;
}

#define MUSIC_KIND(track) (D_8002B074[(track)->id].flags & ~0xF0)

void func_1000E17C(void) {
    s32 i;
    MusicTrack *track;

    for (i = 0; i < 12; i++) {
        track = &D_800419A8[i];
        if (track->id > 0) {
            if ((1 == MUSIC_KIND(track) || 3 == MUSIC_KIND(track)) && track->channel == -1) {
                track->id = -1;
            }
        }
    }
    for (i = 0; i < 12; i++) {
        track = &D_800419A8[i];
        if (track->id > 0) {
            if (track->next != NULL && track->next->id == -1) {
                track->next = NULL;
            }
            if (track->syncTo != NULL && track->syncTo->id == -1) {
                track->syncTo = NULL;
            }
        }
    }
    for (i = 0; i < 12; i++) {
        track = &D_800419A8[i];
        if (track->id > 0) {
            if ((1 == MUSIC_KIND(track) || 3 == MUSIC_KIND(track)) && track->channel != -1) {
                func_1000DE1C(track->id, 4);
            }
        }
    }
}

/* Pauses (pause != 0) or resumes every playing channel; sequences with flag 0x10 are only muted. */
void func_1000E2F4(s32 pause) {
    MusicTrack *track;
    s32 i;

    for (i = 0; i < 3; i++) {
        track = D_800417B0[i];
        if (track == NULL) {
            continue;
        }
        if (track->id <= 0) {
            continue;
        }
        if (track->pauseState != 0) {
            continue;
        }
        if (pause != 0) {
            func_10008EE0(i, 0);
            if (D_8002B074[D_800417B0[i]->id].flags & 0x10) {
                continue;
            }
            func_10008F58(i);
        } else {
            if (!(D_8002B074[track->id].flags & 0x10)) {
                func_100084D8(i);
                track = D_800417B0[i];
            }
            track->volume = -1;
            func_1000CC54(i);
        }
    }
    D_80041F00 = pause;
}

/* Sets the base volume (0..0x7FFF) of sequence `id`. */
void func_1000E40C(s32 id, s32 volume) {
    MusicTrack *track;

    if (volume >= 0x8000) {
        volume = 0x7FFF;
    } else if (volume < 0) {
        volume = 0;
    }
    track = func_1000B1FC(id);
    if (track != NULL) {
        if (track->channel < 0) {
            track->volume = volume;
        }
        track->baseVolume = volume;
    }
}

/* Sets the volume (percent) of the sequence's channels in channelMask, fading over `frames`
   when it is playing (frames < 0 sets it at once). Returns whether the sequence exists. */
s32 func_1000E46C(s32 id, s32 percent, s32 channelMask, s32 frames) {
    s32 i;
    MusicTrack *track;
    MusicTrack *playing;

    track = func_1000B1FC(id);
    percent = (percent * 255) / 100;
    playing = track;
    if (percent >= 256) {
        percent = 255;
    } else if (percent < 0) {
        percent = 0;
    }
    if (track != NULL) {
        if (track->channel >= 0) {
            if (frames < 0) {
                func_1000886C(playing->channel, channelMask, percent);
                return TRUE;
            }
            func_10008790(playing->channel, channelMask, percent, frames);
            return TRUE;
        }
        if (percent == 0) {
            track->mutedChannels |= channelMask;
        } else if (percent > 0) {
            track->mutedChannels &= ~channelMask;
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
s32 func_1000E588(s32 id, s32 percent, s32 channelMask) {
    MusicTrack *track;

    track = func_1000B1FC(id);
    if (track != NULL) {
        if (track->channel >= 0) {
            if (percent > 100) {
                percent = 100;
            } else if (percent < 0) {
                percent = 0;
            }
            func_1000886C(track->channel, channelMask, (percent * 255) / 100);
            return TRUE;
        }
        if (percent <= 0) {
            track->mutedChannels |= channelMask;
            return TRUE;
        }
        if (percent > 0) {
            track->mutedChannels &= ~channelMask;
            return TRUE;
        }
    }
    return FALSE;
}

/* Jumps sequence `id` to `marker`, at once or, with a waitMask, once syncId's sequence raises
   those marker bits. */
s32 func_1000E654(s32 id, s32 marker, s32 waitMask, s32 syncId) {
    MusicTrack *track;
    MusicTrack *syncTo;

    track = func_1000B1FC(id);
    syncTo = NULL;
    if (syncId >= 0) {
        syncTo = func_1000B1FC(syncId);
    }

    if (track != NULL) {
        if (waitMask == 0) {
            if (track->channel >= 0) {
                func_10008C6C(track->channel, marker - 1);
                return TRUE;
            }
        }
        track->pendingJump = marker;
        track->jumpWait = waitMask;
        if (syncTo != NULL) {
            track->syncTo = syncTo;
        }
        return TRUE;
    }
    return FALSE;
}

s32 func_1000E704(s32 id, s32 arg1, s32 arg2) {
    MusicTrack *track = func_1000B1B0(id);

    if (track != NULL && track->channel >= 0) {
        func_10008A94(track->channel, arg2, arg1);
        return TRUE;
    }
    return FALSE;
}

void func_1000E75C(s32 arg0) {
    D_8002B070 = arg0 >> 1;
}

s32 func_1000E770(s32 *arg0, s32 *arg1) {
    if (arg0 != NULL) {
        *arg0 = D_80041F08;
    }
    if (arg1 != NULL) {
        *arg1 = D_80041F0C;
    }
    return D_80041F04;
}

/* Music event from the game: bit 0 sets flag 1, bit 1 adds a sample to the D_80041F08 /
   D_80041F0C average, bits 2 and 3 request a mode in D_80041F08, bit 4 sets flag 0x10. */
void func_1000E7A0(u32 event, s32 value) {
    if ((event & 1) == 1) {
        D_80041F04 |= 1;
        return;
    }
    if (event & 2) {
        D_80041F08 += value;
        D_80041F0C += 1;
        return;
    }
    if (event & 4) {
        D_80041F08 = value + 1;
        D_80041F04 |= 4;
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
        D_80041F04 |= 0x10;
    }
}

void func_1000E8C4(s32 event) {
    if ((event & 1) == 1) {
        D_80041F04 &= ~1;
    }
}

/* Returns the marker bits sequence `id`'s channel raised this frame. */
u8 func_1000E8F0(s32 id) {
    MusicTrack *track = func_1000B1B0(id);

    if (track != NULL && track->channel >= 0) {
        return D_800418AC[track->channel];
    } else {
        return 0;
    }
}

/* Resets the music system. */
void func_1000E934(void) {
    u32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 16; j++) {
            D_800418B0[i][j] = 0x8000;
            D_800417C0[i][j] = 0x100;
        }
        func_10008F24(i);
        D_800417B0[i] = NULL;
        D_800418A0[i] = 0;
        D_80041890[i] = 0;
        D_80041880[i] = 0;
    }
    bzero(D_800419A8, sizeof(D_800419A8));
    D_800419A0 = 0;
    for (k = 0; k < 12; k++) {
        D_800419A8[k].id = -1;
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
    func_1000D96C(id, 0, 0);
    return id;
}
