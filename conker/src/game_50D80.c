#include <ultra64.h>
#include "functions.h"
#include "effects.h"
/* variables.h declares these with the wrong types; they are redeclared below with the types this file uses. */
#include "variables.h"

/* One 8-byte cutscene script event. Each track starts with a 4-event header whose value is the start time. */
typedef struct {
    /* 0x0 */ s8 type;
    /* 0x1 */ u8 delta; /* time step to the next event; 0 chains events at the same time */
    /* 0x2 */ s16 value;
    /* 0x4 */ s8 unk4;
    /* 0x5 */ s8 unk5;
    /* 0x6 */ s16 unk6;
} SeqEvent;

/* An event queued for dispatch by func_150241B4, kept sorted by time. */
typedef struct {
    /* 0x0 */ SeqEvent *event;
    /* 0x4 */ s32 time;
    /* 0x8 */ u8 track;
    /* 0xA */ u16 index;
} SeqQueueEntry;


/* What a cutscene track is bound to: 2 = an object (func_15083E90), 3 = an actor (func_151149AC). */
typedef struct {
    /* 0x0 */ u16 targetType;
    /* 0x2 */ u8 targetId;
    /* 0x3 */ u8 pad3[5];
} SeqTrackInfo;

/* Cutscene state, indexed by scene (and track). */
extern SeqEvent *D_800C3688[][30]; /* each track's events */
extern u16 *D_800C35D8[];          /* each track's event count */
extern u8 D_800C363A[];            /* track count */
extern SeqTrackInfo *D_800C35F0[];
extern s32 D_800C35B0[];           /* current time */
extern s32 D_800C35B8[];           /* time at the previous update */
extern s32 D_800C3640[];           /* end time */
extern u8 D_800C35E8[];            /* changes with each run of the scene */
extern SeqQueueEntry *D_800C3D50;  /* dispatch queue, D_800C3C9A entries */

extern WorldObject *func_151149AC(s32);
extern s32 func_1502460C(SeqEvent *, s32, s32, s32, s32, Actor *, WorldObject *, f32 *, s32);
extern s32 func_150265CC(SeqEvent *, s32, s32, s32, s32, Actor *, WorldObject *, f32 *, s32);
extern s32 func_15029BB8(SeqEvent *, s32, s32, s32, s32, Actor *, WorldObject *, f32 *, s32, s32);
extern void func_1502A8A0(SeqEvent *, s32, s32, s32, s32);

void func_150238D0(void) {
}

/* Per-sound state allocated by func_15029BB8 for a cutscene sound event (0x10 bytes). */
typedef struct {
    /* 0x0 */ u8 scene;
    /* 0x1 */ u8 track;
    /* 0x2 */ u8 stopAtEnd;  /* stop when the scene reaches its end time */
    /* 0x4 */ u16 remaining; /* ticks left, 0xFFFF = unlimited */
    /* 0x6 */ u16 duration;
    /* 0x8 */ u16 fadeIn;
    /* 0xA */ u16 fadeOut;
    /* 0xC */ u16 volume;
    /* 0xE */ u8 sceneTag;   /* D_800C35E8[scene] when the sound started */
} SeqSound;

/* The sound voice handed to the func_150238D8 callback. */
typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ SeqSound *userData;
} SeqVoice;

extern u8 D_800C3C89;
extern void func_10004074(void *);

/*
 * Per-frame callback for a cutscene sound: follows its track's position, counts
 * down its lifetime and applies fade-in/fade-out to *volume. Returns 1 (after
 * freeing the state) when the sound should stop.
 */
s32 func_150238D8(SeqVoice *voice, s32 arg1, u32 *volume, s32 arg3, s32 arg4, s32 arg5, s16 *arg6) {
    SeqSound *snd;
    SeqTrackState *states;
    SeqTrackState *state;
    s32 fadeOut;

    snd = voice->userData;
    if ((snd->stopAtEnd != 0 && D_800C35B0[snd->scene] >= D_800C3640[snd->scene]) || (voice->flags & 0x80)) {
        *volume = 0;
        *arg6 = 0;
        func_10004074(snd);
        return 1;
    }
    if (snd->sceneTag == D_800C35E8[snd->scene]) {
        states = D_800C3958[snd->scene];
        if (states != NULL) {
            voice->x = states[snd->track].x;
            state = &D_800C3958[snd->scene][snd->track];
            voice->y = state->yOffset + state->y;
            voice->z = D_800C3958[snd->scene][snd->track].z;
        }
    }
    if (snd->remaining != 0xFFFF) {
        if (D_800BE9E4 < snd->remaining) {
            snd->remaining -= D_800BE9E4;
        } else {
            *volume = 0;
            *arg6 = 0;
            func_10004074(snd);
            return 1;
        }
    }
    *volume = snd->volume;
    fadeOut = snd->fadeOut;
    // matching: the (u32) casts keep IDO from sharing one temp between the u16 load and its int promotion
    if (snd->remaining < fadeOut) {
        if (snd->remaining != 0) {
            *volume = *volume * (u32)snd->remaining / fadeOut;
        }
    } else {
        if ((u32)snd->fadeIn != 0) {
            if ((snd->duration - snd->fadeIn) < snd->remaining) {
                *volume = *volume * (snd->duration - snd->remaining) / snd->fadeIn;
            }
        }
    }
    if (snd->remaining != 0xFFFF && D_800C3C88 != 0) {
        *volume = *volume * D_800C3C89 / 30;
        if (D_800BE9E4 >= D_800C3C89) {
            *volume = 0;
            *arg6 = 0;
            func_10004074(snd);
            return 1;
        }
    }
    return 0;
}

/*
 * Searches `track` of `scene` for the first event after `after` of the given type
 * (optionally also matching value, unk4 and unk5). Stores it in *found and returns
 * its time; on failure stores NULL and returns the scene's end time.
 */
s32 func_15023BB0(SeqEvent *after, s32 type, s32 track, SeqEvent **found, s32 matchValue, s32 value,
                  s32 matchUnk4, s32 unk4, s32 matchUnk5, s32 unk5, s32 scene) {
    SeqEvent *ev;
    s32 time;

    if (D_800C3688[scene][track] != NULL) {
        time = D_800C3688[scene][track]->value;
        ev = D_800C3688[scene][track] + 4;
        while ((u32) (ev - D_800C3688[scene][track]) < D_800C35D8[scene][track]) {
            if ((ev->delta == 0) && (ev->type == 0)) {
                time = ev->value;
            } else {
                if ((after < ev) &&
                    (type == ev->type) &&
                    ((matchValue == 0) || ((matchValue == 1) && (value == ev->value))) &&
                    ((matchUnk4 == 0) || ((matchUnk4 == 1) && (unk4 == ev->unk4))) &&
                    ((matchUnk5 == 0) || ((matchUnk5 == 1) && (unk5 == ev->unk5)))) {
                    *found = ev;
                    return time;
                }
                while (ev->delta == 0) {
                    if (ev->type == 0) {
                        time = ev->value;
                        break;
                    }
                    ev++;
                    if ((u32) (ev - D_800C3688[scene][track]) >= D_800C35D8[scene][track]) {
                        break;
                    }
                    if ((after < ev) &&
                        (type == ev->type) &&
                        ((matchValue == 0) || ((matchValue == 1) && (value == ev->value))) &&
                        ((matchUnk4 == 0) || ((matchUnk4 == 1) && (unk4 == ev->unk4))) &&
                        ((matchUnk5 == 0) || ((matchUnk5 == 1) && (unk5 == ev->unk5)))) {
                        *found = ev;
                        return time;
                    }
                }
                time += ev->delta;
            }
            ev++;
        }
    }
    *found = NULL;
    return D_800C3640[scene];
}

/*
 * Finds the last matching event at or before `time` (*prev, *prevTime) and the
 * first matching event after it (*next, *nextTime) on `track` of `scene`.
 * Events chained at the same time are compared against the scene's current time.
 */
void func_15023DE0(SeqEvent **prev, SeqEvent **next, s32 *prevTime, s32 *nextTime, s32 track, s32 type,
                   s32 matchValue, s32 value, s32 matchUnk4, s32 unk4, s32 time, s32 scene) {
    SeqEvent *ev;
    s32 evTime;
    s32 sceneTime;

    if (prev != NULL) {
        *prev = NULL;
    }
    *next = NULL;
    if (prevTime != NULL) {
        *prevTime = 0;
    }
    *nextTime = D_800C3640[scene];

    if (D_800C3688[scene][track] == NULL) {
        return;
    }

    ev = D_800C3688[scene][track] + 4;
    evTime = D_800C3688[scene][track]->value;
    while ((u32) (ev - D_800C3688[scene][track]) < D_800C35D8[scene][track]) {
        if ((ev->delta == 0) && (ev->type == 0)) {
            evTime = ev->value;
        } else {
            if (evTime <= time) {
                if ((type == ev->type) &&
                    ((matchValue == 0) || ((matchValue == 1) && (value == ev->value))) &&
                    ((matchUnk4 == 0) || ((matchUnk4 == 1) && (unk4 == ev->unk4)))) {
                    if (prev != NULL) {
                        *prev = ev;
                    }
                    if (prevTime != NULL) {
                        *prevTime = evTime;
                    }
                }
            }
            if (evTime > time) {
                if ((type == ev->type) &&
                    ((matchValue == 0) || ((matchValue == 1) && (value == ev->value))) &&
                    ((matchUnk4 == 0) || ((matchUnk4 == 1) && (unk4 == ev->unk4)))) {
                    *next = ev;
                    *nextTime = evTime;
                    return;
                }
            }
            while (ev->delta == 0) {
                if (ev->type == 0) {
                    evTime = ev->value;
                    break;
                }
                ev++;
                if ((u32) (ev - D_800C3688[scene][track]) >= D_800C35D8[scene][track]) {
                    break;
                }
                sceneTime = D_800C35B0[scene];
                if (evTime <= sceneTime) {
                    if ((type == ev->type) &&
                        ((matchValue == 0) || ((matchValue == 1) && (value == ev->value))) &&
                        ((matchUnk4 == 0) || ((matchUnk4 == 1) && (unk4 == ev->unk4)))) {
                        if (prev != NULL) {
                            *prev = ev;
                        }
                        if (prevTime != NULL) {
                            *prevTime = evTime;
                        }
                        continue;
                    }
                }
                if (evTime > sceneTime) {
                    if ((type == ev->type) &&
                        ((matchValue == 0) || ((matchValue == 1) && (value == ev->value))) &&
                        ((matchUnk4 == 0) || ((matchUnk4 == 1) && (unk4 == ev->unk4)))) {
                        *next = ev;
                        *nextTime = evTime;
                        return;
                    }
                }
            }
            evTime += ev->delta;
        }
        ev++;
    }
}

/* Dispatches the first `count` queued events of `scene` in order. */
void func_15024130(s32 count, s32 scene) {
    s32 i;

    for (i = 0; i < count; i++) {
        func_1502A8A0(D_800C3D50[i].event, D_800C3D50[i].track, D_800C3D50[i].index, D_800C3D50[i].time, scene);
    }
}

extern u16 D_800C3C9A;

/*
 * Inserts an event into the time-sorted dispatch queue. When the queue is full
 * it is flushed (dispatched) first and the new event is dropped.
 */
void func_150241B4(SeqQueueEntry *queue, s32 *count, SeqEvent *event, s32 track, s32 index, s32 time, s32 scene) {
    s32 pos;
    s32 i;

    if (*count >= D_800C3C9A) {
        func_15024130(*count, scene);
        *count = 0;
        return;
    }
    pos = *count;
    for (i = 0; i < *count; i++) {
        if (queue[i].time >= time) {
            pos = i;
            break;
        }
    }
    for (i = *count - 1; i >= pos; i--) {
        queue[i + 1] = queue[i];
    }
    queue[pos].event = event;
    queue[pos].track = track;
    queue[pos].index = index;
    queue[pos].time = time;
    (*count)++;
}

extern u8 D_800C35C0[];
extern u8 D_800C3C8C;

/*
 * Queues and dispatches every event of `scene` whose time falls in
 * (previous time, current time]. Mode 0 dispatches all event types except 0x11,
 * mode 1 only type 0x11.
 */
void func_150242F8(s32 mode, s32 scene) {
    s32 track;
    SeqEvent *ev;
    s32 time;
    s32 queued;

    // matching: indexing through &D_800C35EA[0] rather than the array changes register allocation
    if ((D_800C3C8C != 2) && ((&D_800C35EA[0])[scene] == 1) && (D_800C3C9A != 0)) {
        D_800C3638 = 0;
        queued = 0;
        for (track = 0; track < D_800C363A[scene]; track++) {
            if (D_800C3688[scene][track] != NULL) {
                extern s32 D_800C35B8[]; // matching: the block-scope redeclaration changes IDO's codegen
                if (D_800C35B8[scene] != D_800C35B0[scene]) {
                    ev = D_800C3688[scene][track] + 4;
                    time = D_800C3688[scene][track]->value;
                    while ((u32) (ev - D_800C3688[scene][track]) < D_800C35D8[scene][track]) {
                        if ((ev->delta == 0) && (ev->type == 0)) {
                            time = ev->value;
                        } else {
                            if ((time <= D_800C35B0[scene]) && (time > D_800C35B8[scene])) {
                                for (;;) {
                                    if ((u32) (ev - D_800C3688[scene][track]) >= D_800C35D8[scene][track]) {
                                        break;
                                    }
                                    if (((mode == 0) && (ev->type != 0x11)) || ((mode == 1) && (ev->type == 0x11))) {
                                        func_150241B4(D_800C3D50, &queued, ev, track, (u32) (ev - D_800C3688[scene][track]), time, scene);
                                    }
                                    if ((ev->delta != 0) || ((ev->delta == 0) && (ev->type == 0))) {
                                        break;
                                    } else {
                                        ev++;
                                        continue; // matching: IDO's loop layout needs the explicit continue
                                    }
                                }
                            }
                            if (time > D_800C35B0[scene]) {
                                break;
                            }
                            while (ev->delta == 0) {
                                if (ev->type == 0) {
                                    time = ev->value;
                                    break;
                                }
                                ev++;
                                if ((u32) (ev - D_800C3688[scene][track]) >= D_800C35D8[scene][track]) {
                                    break;
                                }
                            }
                            time += ev->delta;
                        }
                        ev++;
                    }
                }
            }
        }
        func_15024130(queued, scene);
        D_800C3638 = 1;
        if (mode == 0) {
            D_800C35C0[scene] = 1;
        }
    }
}

/* A subtitle slot (see func_150234A4); from `flags` on it is the parameter block passed to func_1516A7B0. */
typedef struct {
    /* 0x00 */ s16 active; /* owning track + 1, 0 = free */
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ s32 flags;
    /* 0x08 */ u8 *text;
    /* 0x0C */ u8 isOpen;
    /* 0x0D */ u8 padD[0xF];
    /* 0x1C */ f32 *unk1C;
    /* 0x20 */ f32 *unk20;
    /* 0x24 */ f32 *unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ u8 r;
    /* 0x2D */ u8 g;
    /* 0x2E */ u8 b;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ u8 pad30[4];
    /* 0x34 */ s32 handle;
} SeqSubtitle;

typedef struct {
    Actor *obj;
    u8 id;
} ObjRef1502460C;

/* Field access by offset, for WorldObject, ActorCtl, struct104 and model fields their shared declarations still leave as padding. */
#define FU8(p, o) (*(u8 *) ((u8 *) (p) + (o)))
#define FS16(p, o) (*(s16 *) ((u8 *) (p) + (o)))
#define FS32(p, o) (*(s32 *) ((u8 *) (p) + (o)))
#define FF32(p, o) (*(f32 *) ((u8 *) (p) + (o)))
#define FPTR(p, o) (*(void **) ((u8 *) (p) + (o)))

extern s8 D_800E0BF4;
extern u8 **D_800C35E0[];
extern s32 D_80090214[];
extern u8 D_800C35F8;
extern u8 D_800C3656[];
extern u8 D_800C3658[];
extern u8 D_800C365A[];
extern u8 D_800C365C[];
extern u8 D_800C365E[];
extern u8 D_800C35C2;
extern u8 D_800C35C3;
extern f32 D_800C364C;
extern f32 D_800C3650;
extern u8 D_800C3660[];
extern u8 D_800C3C8B;
extern u8 D_800C3C8D;
extern s16 D_800DDE10;
extern f32 *D_800DDE44;
extern f32 *D_800DDE48;
extern f32 *D_800DDE4C;
extern f32 D_800C3674;
extern f32 D_800C3678;
extern s32 D_800C367C;
extern u8 D_800C3662;
extern u8 D_800BEA0C;
extern u8 D_800C3680;
extern u8 D_800DBEAC;
extern s32 D_800D2450;
extern u8 D_800C3C9D;
extern s16 D_800C3C9E;

extern void func_1501C730(s32, s32, s32, s32, s32);
extern void func_15023440(SeqSubtitle *, s32);
extern SeqSubtitle *func_150234A4(s32, s32);
extern s32 func_15052F9C(Actor *, f32, s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_15053430(Actor *);
extern void func_1507E7E4(void *, s32, s32, s32, s32);
extern void func_1507EB4C(void *, s32);
extern void func_150836CC(Actor *, s32);
extern s32 func_15083FB0(s32);
extern void func_150B648C(u8);
extern void func_150B7220(s32, s32, s32, s32);
extern void func_150C5C74(Actor *);
extern void func_150C5C9C(Actor *);
extern void func_150C851C(s32);
extern void func_150E0300(void);
extern void func_150F5340(void);
extern void func_150F5420(s32, s32, s32, s32);
extern void func_150FAA40(s32, s32);
extern void func_150FAD28(void);
extern void func_1510A930(s32, s32, s32);
extern void func_1510B458(s32, f32, f32);
extern void func_15147D64(ObjRef1502460C *, s32);
extern void func_1515F170(s32, u8);
extern void func_151616D0(u8, u8, s32);
extern void func_151645C4(u8);
extern void func_15169040(s32, u8);
extern s32 func_1516A7B0(s32, s32, s32, s32, void *, s32);
extern void func_1516D2E0(s32);
extern s32 func_1516D378(s32);
extern s32 func_1516D3C4(s32);
extern void func_1517F488(s32, s32, s32, s32, s32, s32);
extern void func_1517F720(s32, s32, s32, s32, s32);
extern s32 func_1518D1C0(Actor *, s16, s16, s32, u8, s32, char *);
extern void func_1518F45C(s32, u8);
extern s32 func_1518F51C(void *, u8, s16, s8, s8, s8, u8, s32, s32, s32);
extern u8 *func_1519003C(Actor *, s32 *);
extern void func_15190518(void *);
extern void func_15191B8C(ObjRef1502460C *, u8);
extern void func_151937F4(Actor *, s32, s32);
extern s32 func_1519D030(void *, s32, s32, s32, s32, s32);
extern s32 func_151A4FD0(s32, s32, s32, s32, s32, s32, u8, s32);
extern void func_151A9390(s32, s32, s32, f32 *, f32, f32, s32, s32, s32);
extern void func_151BE850(f32 *, f32, s32, s32, s32, s32, s32);
extern void func_151C970C(s32, TriggerZone *);
extern void func_151C9DE8(void *, u8, s8, u8, s32);
extern void func_151C9ED4(Camera *);
extern void func_151CD35C(s32);
extern void func_151CD394(s32);
extern void func_151DE7D4(void);
extern void func_151E53E8(s32);

/*
 * First cutscene event handler: subtitles (types 7, 0xE-0x10, 0x12; shown only
 * when D_800E0BF4 is set and D_800BEAAB selects that type), event types 1, 3, 4,
 * 9, 0xC and 0xD, and most type-5 commands. Returns 1 when the event was handled.
 */
s32 func_1502460C(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene, Actor *obj, WorldObject *actor, f32 *pos, s32 sceneTag) {
    if (D_800E0BF4 != 0 && (event->type == 7 || event->type == 0xE || event->type == 0xF || event->type == 0x10 || event->type == 0x12)) {
        s32 opening;
        SeqSubtitle *sub;
        u8 *text;
        u8 colors[3][3] = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xA0 }, { 0x3C, 0x3C, 0x3C } };

        if ((event->type == 7 && D_800BEAAB != 0) || (event->type == 0xE && D_800BEAAB != 4) ||
            (event->type == 0xF && D_800BEAAB != 2) || (event->type == 0x10 && D_800BEAAB != 3) ||
            (event->type == 0x12 && D_800BEAAB != 1)) {
            return 1;
        }
        opening = event->unk4 == 0 ? 1 : 0;
        sub = func_150234A4(track, opening);
        if (sub != NULL) {
            if (event->unk4 == 0) {
                if (sub->isOpen != 0) {
                    func_1516D2E0(sub->handle);
                }
                text = D_800C35E0[scene][event->value];
                if (*text == 0xBE) {
                    sub->flags = 0x8013;
                    text++;
                } else if (*text == 0xBF) {
                    sub->flags = 0x8019;
                    text++;
                } else {
                    sub->flags = 0x8011;
                }
                sub->text = text;
                sub->unk2A = 100;
                sub->isOpen = 1;
                sub->unk28 = 0;
                sub->unk1C = &D_800C3958[scene][track].unkC;
                sub->unk20 = &D_800C3958[scene][track].unk10;
                sub->unk2F = 1;
                sub->unk24 = &D_800C3958[scene][track].unk14;
                sub->r = colors[event->unk5][0];
                sub->g = colors[event->unk5][1];
                sub->b = colors[event->unk5][2];
                if (event->unk5 == 2) {
                    sub->flags |= 0x40000;
                }
                sub->handle = func_1516A7B0(0, 0, 0, 0, &sub->flags, sceneTag);
            } else if (event->unk4 == 1) {
                func_15023440(sub, event->unk5);
            } else {
                if (event->unk5 == 0) {
                    sub->handle = func_1516D378(sub->handle);
                } else {
                    sub->handle = func_1516D3C4(sub->handle);
                }
                if (event->unk6 != 0) {
                    s32 color = event->unk6 - 1;

                    sub->r = colors[color][0];
                    sub->g = colors[color][1];
                    sub->b = colors[color][2];
                    if (color == 2) {
                        sub->flags |= 0x40000;
                    } else {
                        sub->flags &= ~0x40000;
                    }
                }
            }
        }
    } else if (event->type == 0xC) {
        SeqEvent *found;
        s32 duration;

        if (event->value != 0) {
            if (event->unk6 != 0) {
                func_1507EB4C(obj, event->value);
            } else {
                if (event->unk5 != 0) {
                    duration = 0xFFFF;
                } else {
                    duration = func_15023BB0(event, event->type, track, &found, 0, 0, 0, 0, 0, 0, scene) - eventTime;
                }
                func_1507E7E4(obj, event->value & 0xFF, 3, duration & 0xFFFF, event->unk4);
            }
        }
    } else if (event->type == 0xD) {
        ActorCtl *s;

        if (obj == NULL) {
            return 1;
        }
        if (event->unk4 == 0) {
            func_15083568(obj, event->value + 1, 1.0f, 0);
        } else if (event->unk4 == 1) {
            func_150836CC(obj, event->value + 1);
        } else {
            s = func_1503195C(obj, event->value + 1, 0);
            if (s != NULL) {
                if (event->unk4 == 2) {
                    FS16(s, 0x18) = D_80090214[event->unk5];
                } else if (event->unk4 == 3) {
                    FS16(s, 0x1A) = D_80090214[event->unk5];
                }
            }
        }
    } else if (event->type == 4) {
        SeqEvent *found;
        s32 duration;

        if (obj == NULL) {
            return 1;
        }
        duration = (func_15023BB0(event, event->type, track, &found, 0, 0, 0, 0, 0, 0, scene) - eventTime) / 2;
        if (duration >= 0x100) {
            duration = 0xFF;
        } else if (duration == 0) {
            duration = 1;
        }
        if (event->unk4 == 0) {
            obj->unk134 = event->value;
            obj->morphDuration = duration;
        }
    } else if (event->type == 3) {
        if (event->value >= D_800C363A[scene]) {
            return 1;
        }
        if (D_800C35F0[scene][event->value].targetType == 1 && D_800C35F8 != event->value) {
            D_800C35F8 = event->value;
            D_800DBFF4[0] = 4;
            D_800DBFF0->status |= 4;
        }
    } else if (event->type == 9) {
        if (obj == NULL) {
            return 1;
        }
        obj->eyeCode[event->unk4] = event->value + 10;
    } else if (event->type == 1) {
        s32 v;

        if (obj == NULL) {
            return 1;
        }
        v = event->value;
        if (v == 0) {
            v = 0xFF;
        }
        obj->lodRequest = v;
    } else if (event->type == 5) {
        if (event->value == 0x3B) {
            if (event->unk4 == 0) {
                D_800DDE18 = &D_800C3958[scene][track].unk28;
            } else {
                D_800DDE18 = NULL;
            }
        } else if (event->value == 8) {
            D_800C3656[scene] = !event->unk4;
        } else if (event->value == 9) {
            D_800C3658[scene] = !event->unk4;
        } else if (event->value == 0xD) {
            D_800C3654[scene] = !event->unk4;
        } else if (event->value == 0x12) {
            D_800C365A[scene] = !event->unk4;
        } else if (event->value == 0x13) {
            D_800C365C[scene] = event->unk4;
        } else if (event->value == 0x15) {
            D_800C365E[scene] = !event->unk4;
        } else if (event->value == 0x14) {
            if (obj == NULL) {
                return 1;
            }
            if (event->unk4 == 0) {
                func_15053430(obj);
            } else {
                func_15060F28(obj, 0);
            }
        } else if (event->value == 0x41) {
            u8 colors[2][3] = { { 0xFF, 0, 0 }, { 0, 0, 0 } };

            if (obj == NULL) {
                return 1;
            }
            if (event->unk4 == 0) {
                obj->fogLevel = colors[event->unk5][0]; // mode 1: fogLevel/fogRate/fogFade hold the colour
                obj->fogRate = colors[event->unk5][1];
                obj->fogFade = colors[event->unk5][2];
                obj->fogMode = 1;
                obj->fogColor = (u32)&D_800C3958[scene][track].unk30;
            } else {
                obj->fogMode = 0;
                obj->fogColor = 0;
            }
        } else if (event->value == 0x44) {
            s32 a;
            s32 c;

            if (event->unk5 != 0) {
                c = event->unk6;
                a = 2;
            } else {
                c = 0;
                a = 1;
            }
            func_1501C730(a, event->unk4, c, 0, 0);
        } else if (event->value == 0x17) {
            if (obj == NULL) {
                return 1;
            }
            obj->levelMode = !event->unk4;
            if (obj->levelMode != 0) {
                obj->levelMode += event->unk5;
            }
            if (event->unk4 == 0) {
                obj->unk201 = event->unk6;
            }
        } else if (event->value == 0x16) {
            if (obj == NULL) {
                return 1;
            }
            obj->level2Mode = !event->unk4;
            if (obj->level2Mode != 0) {
                obj->level2Mode += event->unk5;
            }
            if (event->unk4 == 0) {
                obj->unk208 = event->unk6;
            }
        } else if (event->value == 0x18) {
        } else if (event->value == 0x1D) {
            if (D_800C35F0[scene][track].targetType == 3 && actor != NULL) {
                if (event->unk4 == 0) {
                    FU8(actor, 0x6E) = 1;
                } else {
                    FU8(actor, 0x6E) = 0;
                }
            }
        } else if (event->value == 0x1E) {
            D_800C35C2 = event->unk4 + 1;
            D_800C35C3 = D_800BE9F0;
        } else if (event->value == 3) {
            WorldObject *target = func_151149AC((u8) event->unk6);

            if (target == NULL) {
                return 1;
            }
            FU8(target, 0x73) &= ~3;
            if (event->unk5 == 0) {
                FU8(target, 0x73) |= 2;
            } else {
                FU8(target, 0x73) |= 1;
            }
        } else if (event->value == 0x6C) {
            f32 point[3];
            f32 point2[3];

            // matching: the switch reads the field as an unsigned byte
            switch (*(u8 *) &event->unk4) {
                case 0:
                    point[0] = D_800C3958[scene][track].x;
                    point[1] = D_800C3958[scene][track].y;
                    point[2] = D_800C3958[scene][track].z;
                    func_151BE850(point, D_800C3958[scene][track].unk10, 1, 0, 2, 0xFF, 1);
                    break;
                case 1:
                    point2[0] = D_800C3958[scene][track].x;
                    point2[1] = D_800C3958[scene][track].y;
                    point2[2] = D_800C3958[scene][track].z;
                    func_151A9390(5, 1, 0, point2, -10000.0f, 100.0f, -1, sceneTag, 1);
                    break;
                case 2:
                    func_151494E0(0, event->unk5 ? 0x2B : 0x2C);
                    break;
                case 3:
                    if (event->unk5) {
                        func_1515F170(1, 0);
                        func_1515F170(2, 1);
                    } else {
                        func_1515F170(1, 1);
                        func_1515F170(2, 0);
                    }
                    break;
                case 4:
                    func_151645C4(event->unk5);
                    break;
                case 5:
                    func_151616D0(0x28, 0x17, 0);
                    break;
                case 6:
                    func_151CD35C(event->unk5);
                    break;
                case 7:
                    func_1515F170(3, event->unk5 ? 0 : 1);
                    break;
                case 8:
                    func_150B648C(event->unk5);
                    break;
                case 9: {
                    u8 i;
                    const s32 ids[4] = { 0x12, 0x13, 0x14, 0x15 };

                    for (i = 0; i < 4; i++) {
                        func_151C970C(2, &D_800D3098[ids[i]]);
                    }
                    break;
                }
                case 10:
                    func_151C9DE8(D_800DBFF0, 0, track, sceneTag, 1);
                    break;
                case 11:
                    func_151C9ED4(D_800DBFF0);
                    break;
                case 12:
                    func_150E0300();
                    break;
                case 13:
                    func_150FAD28();
                    break;
                case 14:
                    func_150F5340();
                    break;
                case 15:
                    func_150FAA40(0xFF, 1);
                    break;
                case 16:
                    func_15169040(0, 0x4E);
                    break;
                case 17:
                    func_15169040(0, 0x4F);
                    break;
                case 18:
                    func_151CD394(event->unk5);
                    break;
                case 19:
                    func_1515F170(0xB, 0);
                    break;
                case 20:
                    if (obj != NULL) {
                        func_15190518(obj);
                        func_1518F45C(0, 0x49);
                    }
                    break;
                case 21:
                    func_151494E0(0, event->unk5 != 0 ? 0x55 : 0x56);
                    break;
                case 22:
                    if (obj != NULL) {
                        func_151937F4(obj, 0xFF, 1);
                    }
                    break;
                case 23:
                    if (obj != NULL) {
                        ObjRef1502460C ref;

                        ref.obj = obj;
                        ref.id = obj->unique_id;
                        func_15191B8C(&ref, 0x50);
                    }
                    break;
            }
        } else if (event->value == 0x6E) {
            u8 *list;
            s32 count;

            list = func_1519003C(obj, &count);
            if (list != NULL && event->unk6 < count) {
                func_1518F51C(obj, list[event->unk6], -1, -1, -1, -1, 0, 0, 0xFF, 1);
            }
        } else if (event->value == 5) {
            s32 idx;
            SeqEvent *found;
            s32 duration;
            s32 ids[4] = { 0xD, 0xE, 0xF, 0x10 };
            u8 c;

            if (event->unk4 == 0) {
                idx = event->unk5;
                if (idx >= 4) {
                    idx = 3;
                }
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 1, event->unk5, scene) - eventTime;
                if (event->unk6 != 0) {
                    duration = 999;
                    c = 0xFF;
                } else {
                    c = sceneTag;
                }
                func_1518D1C0(obj, ids[idx], duration, 0, c, 0, "Cutscene 1");
            }
        } else if (event->value == 0x36) {
            if (event->unk4 == 0) {
                func_1519D030(obj, 0, 300, 0, sceneTag, 0);
            } else if (event->unk4 == 1) {
                ObjRef1502460C ref;

                ref.obj = obj;
                ref.id = obj->unique_id;
                func_15147D64(&ref, 7);
            }
        } else if (event->value == 0x30) {
            u8 v;

            if (obj == NULL) {
                return 1;
            }
            {
                switch (event->unk4) {
                    case 0:
                        v = 0xC;
                        break;
                    case 1:
                        v = 4;
                        break;
                    case 2:
                        v = 7;
                        break;
                    case 3:
                        v = 0xA;
                        break;
                    default:
                        v = 0xC;
                        break;
                }
                func_1514D3B0(obj, 0xD, v, 0);
            }
        } else if (event->value == 0x46) {
            if (event->unk4 != 0) {
                func_150C5C74(obj);
            } else {
                func_150C5C9C(obj);
            }
        } else if (event->value == 0x25) {
            if (event->unk4 != 0) {
                D_800C364C = event->unk4;
            } else {
                D_800C364C = D_800D9B20;
            }
            func_1510B458(0, D_800C364C, D_800C3650);
        } else if (event->value == 0x58) {
            if (event->unk6 != 0) {
                D_800C3650 = event->unk6 * 10;
            } else {
                D_800C3650 = D_800D9B1C;
            }
            func_1510B458(0, D_800C364C, D_800C3650);
        } else if (event->value == 0x3C) {
            s32 v;

            if (obj != NULL) {
                v = event->unk4;
                v <<= 16;
                v |= event->unk6;
                if (event->unk5 == 0) {
                    obj->unk94 = v;
                } else if (event->unk5 == 1) {
                    obj->unk94 |= v;
                } else {
                    obj->unk94 &= ~(v | 1);
                }
            }
        } else if (event->value == 0x3D) {
            s32 r;

            if (obj != NULL) {
                if (event->unk4 == 0) {
                    r = func_15083FB0((u8) event->unk6);
                    if (r != -1) {
                        obj->unk101 |= 4;
                        obj->unk65 = r + 1;
                        obj->unk5C = event->unk5;
                    }
                } else {
                    obj->unk65 = 0;
                }
            }
        } else if (event->value == 0x42) {
            if (event->unk4 == 0) {
                D_800D9890 = 1;
            } else if (event->unk4 == 1) {
                D_800D9890 = 4;
            } else if (event->unk4 == 3) {
                D_800D9890 = 2;
            } else if (event->unk4 == 4) {
                D_800D9890 = 5;
            } else if (event->unk4 == 5) {
                D_800D9890 = 6;
            } else if (event->unk4 == 6) {
                D_800D9890 = 7;
            } else if (event->unk4 == 7) {
                D_800D9890 = 8;
            } else if (event->unk4 == 8) {
                D_800D9890 = 9;
            } else if (event->unk4 == 9) {
                D_800D9890 = 10;
            }
        } else if (event->value == 0x43) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_150B7220(D_800C3958[scene][track].unk24, D_800C3958[scene][track].unk28, duration, sceneTag);
            }
        } else if (event->value == 0x45) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_151A4FD0(0, duration, 0xFF, 0, 0, D_800BE9E8, 0, 0);
            }
        } else if (event->value == 0x49) {
            if (obj != NULL && event->unk4 < 3) {
                (&obj->unk2E4)[event->unk4] = event->unk6;
            }
        } else if (event->value == 0x4B) {
            if (obj != NULL) {
                obj->unk9C = (event->unk6 & 0xFFFF) | ((event->unk5 & 0xFF) << 16) | ((event->unk4 & 0xFF) << 24);
            }
        } else if (event->value == 0x4A) {
            if (event->unk4 == 0) {
                func_10011FB0(event->unk5);
            } else {
                func_10011FB0(0);
            }
        } else if (event->value == 0x54) {
            D_800C3660[scene] = ~event->unk4;
        } else if (event->value == 0x55) {
            func_150C851C(event->unk4);
        } else if (event->value == 0x59) {
            D_800C3C8B = 1;
        } else if (event->value == 0x5A) {
            D_800C3E78 = obj - D_800CC2D0;
            if (obj->id == 0x53) {
                obj->unk124 = 0;
                func_15052F9C(obj, 156.0f, 1, 4, 0, 10, 0xFF, 0, 1, 0);
            } else if (obj->id == 0x28) {
                obj->unk124 = 0;
                func_15052F9C(obj, 180.0f, 5, 0x44, 1, 1, 0xFF, 0, 1, 0);
            } else if (obj->id == 0x21) {
                obj->unk124 = 0;
                func_15052F9C(obj, 156.0f, 0x19, 4, 0, 0xF, 0xFF, 0, 1, 0);
            }
        } else if (event->value == 0x71) {
            D_800CC2D0->unkCC = 0;
            D_800CC2D0->unk31C->unk4E = 0;
            D_800CC2D0->unk65 = 0;
            D_800CC2D0->unk31C->unk27 = 0;
            D_800CC2D0->unkF8 &= ~0x01000000;
            if (obj != NULL) {
                obj->unk13C = 0;
            }
        } else if (event->value == 0x37) {
            s32 duration;
            SeqEvent *found;
            s32 v;
            u8 colors[3][3] = { { 0xFF, 0, 0 }, { 0, 0x35, 0x46 }, { 0xFF, 0xFF, 0xFF } };

            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                v = event->unk6 != 0 ? event->unk6 : 0x7F;
                func_1517F488(colors[event->unk5][0], colors[event->unk5][1], colors[event->unk5][2], v, duration, 0);
            } else {
                D_800DDE10 = 0;
            }
        } else if (event->value == 0x38) {
            s32 duration;
            SeqEvent *found;
            s32 v;

            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                v = event->unk6 != 0 ? event->unk6 : 4;
                func_1517F720(event->unk5, duration, v, 0x7F, 1);
            } else {
                D_800DDE08 = 0;
            }
        } else if (event->value == 0x5C) {
            SeqEvent *found;
            s32 total;
            s32 start;
            s32 end;

            if (event->unk4 == 0) {
                total = func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 1, 1, event->unk5, scene);
                start = func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 2, 1, event->unk5, scene) - eventTime;
                if (found == NULL) {
                    start = 0;
                }
                end = total - func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 3, 1, event->unk5, scene);
                if (found == NULL) {
                    end = 0;
                }
                if (D_800BE9F0 == 0x18) {
                    func_150F5420(start, total - eventTime - end - start, end, event->unk5);
                } else if (D_800BE9F0 == 0x21) {
                    func_1510A930(start, total - eventTime - end - start, end);
                }
            }
        } else if (event->value == 0x62) {
            FU8(D_800B0DF0, 8) = 0;
        } else if (event->value == 0x7C) {
            D_800C3681 = !event->unk4;
        } else if (event->value == 0x84) {
            D_800C3682 = !event->unk4;
        } else if (event->value == 0x85) {
            D_800C3683 = !event->unk4;
        } else if (event->value == 0x63) {
            D_80041FDC = event->unk6;
        } else if (event->value == 0x64) {
            if (event->unk4 == 0) {
                D_800DDE40 = 1;
                D_800DDE44 = &D_800C3958[scene][track].unk28;
                D_800DDE48 = &D_800C3958[scene][track].unk24;
                D_800DDE4C = &D_800C3958[scene][track].unk2C;
            } else {
                D_800DDE40 = 0;
            }
        } else if (event->value == 0x66) {
            if (event->unk4 == 0) { // matching: empty, but it still shapes the code
            }
        } else if (event->value == 0x67) {
            func_151DE7D4();
        } else if (event->value == 0x73) {
            func_151E53E8(0);
        } else if (event->value == 0x69) {
            if (obj != NULL) {
                if (event->unk4 == 0) {
                    obj->gravity = 4.0f;
                    obj->y_velocity = 20.0f;
                } else if (event->unk4 == 1) {
                    obj->gravity = 0.0f;
                    obj->y_velocity = 0.0f;
                } else if (event->unk4 == 2) {
                    func_150836CC(obj, 0x11);
                    obj->unkA9 = 0;
                } else if (event->unk4 == 3) {
                    D_800C3E78 = obj - D_800CC2D0;
                    D_800CC2D0->unk13C = D_800C3E78 + 100;
                    D_800CC2D0->xz_velocity = 0.0f;
                    FS16(D_800CC2D0->unk31C, 0x18) = 0;
                    func_1505959C(obj, 0);
                    *(u32 *) &obj->unk5C = 9; // matching: an unsigned store
                    obj->unk65 = 1;
                    obj->unk101 |= 4;
                } else if (event->unk4 == 4) {
                    FU8(D_800CC2D0->unk31C, 0x17) = 1;
                } else if (event->unk4 == 5) {
                    D_800D154C = obj;
                    func_15073F1C();
                } else if (event->unk4 == 6) {
                    obj->unk31C->unk4E = 0;
                }
            }
        } else if (event->value == 0x6A) {
            D_800C3674 = event->unk6;
        } else if (event->value == 0x7A) {
            D_800C367C = event->unk5 * 60;
            D_800C3678 = event->unk6;
        } else if (event->value == 0x6B) {
        } else if (event->value == 0x6D) {
            D_800C3662 = !event->unk4;
        } else if (event->value == 0x74) {
            D_800BEA0C = event->unk5;
        } else if (event->value == 0x6F) {
            if (obj != NULL) {
                obj->unk68[event->unk4] = event->unk5;
            }
        } else if (event->value == 0x70) {
            D_800C3C8C = 1;
        } else if (event->value == 0x72) {
            if (obj != NULL) {
                obj->unkC8 = FS16(D_800D1C90[obj->id], 6);
            }
        } else if (event->value == 0x78) {
            D_800C3C8D = 1;
        } else if (event->value == 0x79) {
            if (actor != NULL) {
                FS32(actor, 0x84) = 0;
                FS32(actor, 0x80) = 0;
                FS32(actor, 0x7C) = 0;
            }
        } else if (event->value == 0x7B) {
            if (D_800BE9F0 == 0x1D) {
                D_800C3680 = 1;
            }
        } else if (event->value == 0x7D) {
            if (obj != NULL && event->unk4 == 0) {
                obj->unk5 = 1;
            }
        } else if (event->value == 0x7E) {
            if (obj != NULL) {
                D_800D18D0 = obj - D_800CC2D0;
            }
        } else if (event->value == 0x7F) {
            D_800DBEAC = 1;
        } else if (event->value == 0x80) {
            D_800BE9ED = event->unk4;
        } else if (event->value == 0x81) {
            func_15093818(event->unk4 ^ 1);
            D_800D2450 = event->unk6 * 30;
        } else if (event->value == 0x82) {
            D_800C3C9D = event->unk4;
            if (D_800C3C9D > 100) {
                D_800C3C9D = 100;
            }
        } else if (event->value == 0x83) {
            D_800C3C9E = event->unk6;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

typedef struct EvNode {
    u8 pad0[0x10];
    struct EvNodeData *unk10;
    struct EvNode *unk14;
    u8 pad18[4];
    s16 unk1C;
} EvNode;

typedef struct EvNodeData {
    u8 pad0[0x30];
    s32 unk30;
} EvNodeData;

typedef struct {
    u8 pad0[4];
    s16 unk4;
} Ev8008ABE8;

/* Result of a func_15046C80 ground probe. */
typedef struct {
    /* 0x00 */ f32 y; /* height of the ground that was hit */
    /* 0x04 */ f32 unk4;
    /* 0x08 */ u8 pad8[0x15];
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 pad1E[6];
} EvFloor;

typedef struct {
    f32 *unk0;
    f32 *unk4;
    f32 *unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s8 unk16;
    s8 unk17;
} Ev15134908;

typedef struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 pad13;
    s8 unk14;
} Ev15167AD8;

typedef struct {
    Actor *unk0;
    u8 unk4;
    s16 unk6;
    s8 unk8;
    s8 unk9;
    s8 unkA;
} Ev15190770;

typedef struct {
    s8 unk0;
    s16 unk2;
    s8 unk4;
    s8 unk5;
    s8 unk6;
} Ev151D8868;

extern u8 *D_800C3960[][30];
extern s16 *D_800C3868[][30];
extern Ev8008ABE8 *D_8008ABE8[];
extern s32 D_8008CA50;
extern f32 *D_800AAA90;
typedef struct {
    u8 c[3][6];
} EvColors;

extern EvColors D_80084148;

extern void func_151F2BA8(void);
extern void func_151F2BE8(void);
extern void func_151F2C4C(void);
extern void func_151F2D6C(s32, s32);
extern void func_1516D99C(s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_151B9BF0(s32, s32, s16, s16, s16, s16, s16, s16, s16, s16, s16, s16, s16, f32 *, s32, s32);
extern void func_15167AD8(Ev15167AD8 *, u8, s32);
extern void func_15177F90(u8, f32 *, u8, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_150B0094(f32 *, f32 *, u8, s32);
extern void func_150E1AB0(s32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, f32, f32, f32, f32, f32, f32);
extern void func_150E2DB4(Actor *, u8, s16, s32, f32, f32, f32, f32, f32, f32, s32, s32, s32, s32);
extern void func_1516E778(f32 *, s32, s8, u8, s32);
extern void func_150BEF7C(s32);
extern f32 func_150489B0(u8);
extern void func_1516F548(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
extern void func_151DC8BC(f32 *, s16, s32, s32, s32, s32);
extern void func_150BA4C0(Actor *, u8, s32);
extern void func_151A3390(Actor *, u8);
extern void func_15165F80(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_150B2494(Actor *, s32, s32);
extern void func_15136A50(f32 *, f32 *, f32 *, s16, s32, s32);
extern void func_150CE150(Actor *, s16, u8, s32);
extern void func_150CFC38(f32);
extern void func_15109410(Actor *, s16, s32, s32, f32, s32, s32, s32);
extern void func_15109ED4(WorldObject *, s16, s32, s32, f32, s32, s32, s32);
extern void func_1510A344(Actor *, s16, u8, s32);
extern void func_151619A0(s32, s16, u8, s32);
extern void func_15108FFC(s32, s32, u8);
extern void func_15190770(Ev15190770 *, s32, u8, s32);
extern void func_150F802C(SeqTrackState *, s16, u8, s32);
extern void func_150AF738(s16, u8, s32);
extern void func_150F5C08(Actor *, s16, u8, s32);
extern void func_1519EB8C(SeqTrackState *, u16, s16, u8, s32);
extern void func_150C4120(Actor *, s16, u8, s32);
extern void func_150F6890(Actor *, s16, u8, s32);
extern void func_150D0134(u8, s32, s16, u8, s32);
extern void func_150D02B4(u8, s32, s16, u8, s32);
extern void func_150D04C4(u8, s32, s16, u8, s32);
extern void func_150CF680(s16, u8, s32);
extern void func_151925C4(Actor *, s16, u8, s32);
extern void func_1519ED84(SeqTrackState *, u16, s16, u8, s32);
extern void func_150FAE18(s16, u8, s32);
extern s32 func_15046C80(f32 *, s32, f32, EvFloor *);
extern void func_15136698(f32, f32, u8, s32, s32, f32 *, f32 *, s32, s32, s32, s32);
extern void func_150E3340(f32 *, s32 *, SeqTrackState *, s16);
extern void func_151D8868(Ev151D8868 *, s32, u8, s32);
extern void func_151B5240(s16, u8, s32);
extern void func_15192D48(Actor *, s16, u8, s32);
extern void func_151B09BC(Actor *, s32, s16, u8, s32);
extern void func_15134908(Ev15134908 *, s32, u8, s32);
extern void func_150E9FE0(Actor *, s16, u8, s32);
extern void func_150EA500(Actor *, s16, u8, s32);
extern void func_151356D4(f32 *, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_150C7020(Actor *, s32, u8, s32);
extern void func_150DB518(f32 *, f32 *, f32 *, f32 *, f32 *, s32, s32, s32);
extern s32 func_1514D4B8(s16, s16);
extern s32 func_1519E970(s16, EvNodeData *, s32, f32 *, f32 *, s32, s32);
extern void func_1516F2F8(s32, s32, s32, s32, s32, s32);
extern void func_1516F1C0(s32, s32, u8, s32);
extern void func_15136404(Actor *, f32, u8, s32);
extern void func_1504715C(EvFloor *, Actor *);
extern void func_150CA930(f32 *);
extern void func_151090DC(void);
extern void func_150FB324(Actor *, u8, s32);
extern void func_150F97EC(Actor *, u8, s32);
extern void func_150F9950(Actor *, u8, s32);
extern s32 func_1514654C(Actor *, ActorCtl *, s32, f32 **, f32 **, s32);
extern void func_151C5588(Actor *, f32 *, s32, u8, s32);
extern void func_151C56A4(f32 *, u8, s32);
extern void func_150E06D8(Actor *, u8, u8, s32);
extern void func_151CFA4C(Actor *, u8, s32);
extern void func_151C62D0(Actor *, u8, s32, ActorCtl *, s32, s32, s32);
extern void func_150BAA14(Actor *, s32, s32);
extern void func_150E0870(Actor *, u8, s32);
extern void func_151D5648(f32 *, u8, s32);
extern void func_15136C3C(Actor *, s32, s32, s32, s32, s32, s32, s32);
extern void func_15143134(const f32 *, f32 *, void *);
extern void func_151C329C(f32 *, u8, s32);
extern void func_15107B78(Actor *, s16, s16, u8, s32);
extern void func_151CC290(Camera *);
extern void func_1519EA78(f32 *, u16, f32, u8, s32);
extern void func_15107700(Actor *, s16, s16, s16, s32, f32, f32, s32, s32, u8 *, s32, s32);
extern void func_150F16DC(Actor *, u8, u8, u8, s32);
extern void func_15131EE4(f32 *, f32, u8, s32);
extern void func_15170B90(void *, s32, s32, s32, s32, s32);
extern void func_1507DE4C(Actor *);
extern void func_15138C80(Actor *, u8, s32);
extern void func_15193660(Actor *, u8, s32);
extern void func_151A0A10(Actor *, s32, u8, s32);
extern void func_151A6F00(void *, s32, u8, s32);
extern void func_15153F18(AngleRange *, SplashSpray *, s32, u8, s32);
extern void func_151DBAA8(f32 *, s32, u8, u8, s32);
extern void func_151AF4D0(Actor *, u8, u8, s32);
extern void func_150C1F60(u8);
extern void func_150C2290(u8);
extern void func_150C2424(u8);
extern void func_150DAE70(Actor *, s32, u8, s32);
extern void func_15148F1C(u8, f32, f32, f32, s32, s32, s32, f32, s16, f32, f32, s32);
extern void func_150DB9E0(Actor *, s32, u8);
extern void func_1518B264(f32 *, f32, f32, s32, s32);
extern void func_1518B2A8(f32 *, f32, f32, s32, s32, s32, s32);
extern void func_15171600(WorldObject *, u8, s32);
extern void func_15171BF4(Actor *, u8);
extern void func_15170EC4(WorldObject *, u8, s32);
extern void func_151711C4(Actor *);
extern void func_1518B6B0(f32, f32, f32, u8, s32);
extern void func_1516ED68(s32, s32, u8, s32);
extern void func_150C2804(f32 *, f32 *, f32 *, s16, s32, s32);

/*
 * Second cutscene event handler: types 0xB (func_1001263C / func_151F2xxx) and
 * 0x11 (records the event index in D_800C3960), and the effect-spawning type-5
 * commands. Returns 1 when the event was handled.
 */
s32 func_150265CC(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene, Actor *obj, WorldObject *actor, f32 *pos, s32 sceneTag) {
    if (event->type == 0xB) {
        s32 vol;
        s32 vol2;
        s32 duration;
        SeqEvent *found;

        if (event->unk4 == 0) {
            if (event->unk6 == 0) {
                vol = 0x10000;
            } else {
                vol = (event->unk6 << 16) / 255;
            }
            func_1001263C(event->value, vol, 0x40);
        } else if (event->unk4 == 1) {
            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 2, 0, 0, scene) - eventTime;
            if (event->unk6 == 0) {
                vol2 = 0x10000;
            } else {
                vol2 = (event->unk6 << 16) / 255;
            }
            duration = (duration * 0x5604) / 60;
            func_151F2D6C(vol2, duration);
        } else if (event->unk4 == 3) {
            func_151F2BA8();
        } else if (event->unk4 == 4) {
            func_151F2BE8();
        } else if (event->unk4 == 5) {
            func_151F2C4C();
        }
    } else if (event->type == 0x11) {
        if (event->unk4 != 0) {
            D_800C3960[scene][track][event->value] = index;
        } else {
            D_800C3960[scene][track][event->value] = 0xFF;
        }
    } else if (event->type == 5) {
        if (event->value == 0x11) {
            s32 duration;
            SeqEvent *found;
            s32 param;
            s32 idx;
            s32 c0;
            s32 c1;
            s32 c2;
            s32 c3;
            s32 c4;
            s32 c5;
            s32 t;
            s32 u;
            u8 colors[3][6] = { { 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF, 0x00, 0xAB, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };

            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                if (event->unk6 == 0) {
                    param = 0x1000;
                } else if (event->unk6 > 200) {
                    param = event->unk6;
                } else {
                    param = 200;
                }
                if (event->unk5 >= 3) {
                    idx = 0;
                } else {
                    idx = event->unk5;
                }
                c0 = colors[idx][0];
                c1 = colors[idx][1];
                c2 = colors[idx][2];
                c3 = colors[idx][3];
                c4 = colors[idx][4];
                c5 = colors[idx][5];
                t = param >> 3;
                u = ((param * 40) / 4096) & 0xFF;
                func_1516D99C((s32)pos >> 16, (s32)pos & 0xFFFF, 0, 0x1B, 0,
                              c0, c1, c2, c3, c4, c5,
                              0, 1, 6, 0, 0, 0, 0, 0, t >> 8, t & 0xFF, 0, 0, 0xA, 0, 0, 0, 0, 0, 0, 0,
                              duration, u, u, 0xFF, 0xE, 8, 8, 0x25, 0, 0xFF, 8, 0, 0,
                              scene, 0, 0, sceneTag, 0);
            }
        } else if (event->value == 0xA) {
            s32 duration;
            SeqEvent *found;

            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                func_151B9BF0(3, 3, 0, 0, 0, 0x320, 0x4B0, 0x190, 0xC8, 0x66, 0x1C7, 2, duration, pos, sceneTag, 0);
            }
        } else if (event->value == 0xB) {
            s32 duration;
            SeqEvent *found;

            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                func_151B9BF0(0x2F, 1, 0, 0, 0, 0x7D0, 0x3E8, 0x190, 0xC8, 0x400, 0x400, 6, duration, pos, sceneTag, 0);
            }
        } else if (event->value == 0xC) {
            Ev15167AD8 sp;

            sp.unk0 = D_8008CA50;
            sp.unk4 = 0;
            sp.unk6 = 0x100;
            sp.unk8 = pos[0];
            sp.unkA = pos[1] + event->unk4;
            sp.unkC = pos[2];
            sp.unkE = 0x5DC;
            sp.unk10 = 0x5DC;
            sp.unk12 = -10;
            sp.unk14 = 0;
            func_15167AD8(&sp, sceneTag, 1);
        } else if (event->value == 0x19) {
            s32 duration;
            SeqEvent *found;
            s32 count;

            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                count = event->unk5;
                if (count == 0) {
                    count = 0xC;
                }
                func_15177F90(count, pos, 0xFF, pos[0], pos[1], pos[2], 0, duration, sceneTag, 0, event->unk6);
            }
        } else if (event->value == 0x1A) {
            if (event->unk4 != 0 && event->unk4 == 1) { // matching: redundant test kept for codegen
                f32 from[3];
                f32 to[3];

                from[0] = D_800C3868[scene][track][0];
                from[1] = D_800C3868[scene][track][1];
                from[2] = D_800C3868[scene][track][2];
                to[0] = D_800C3958[scene][track].x;
                to[1] = D_800C3958[scene][track].y + D_800C3958[scene][track].yOffset;
                to[2] = D_800C3958[scene][track].z;
                func_150B0094(from, to, sceneTag, 1);
            } else {
                func_150E1AB0(0, D_800C3868[scene][track][0], D_800C3868[scene][track][1], D_800C3868[scene][track][2],
                              D_800C3958[scene][track].x, D_800C3958[scene][track].y + D_800C3958[scene][track].yOffset,
                              D_800C3958[scene][track].z, 40.0f, 0.0f, 8.0f, 60.0f, 0x258, 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            }
        } else if (event->value == 0x1C) {
            s32 anim;

            if (obj == NULL) {
                return 1;
            }
            switch (event->unk4) {
                case 0:
                    anim = 3;
                    break;
                case 1:
                    anim = 2;
                    break;
                default:
                    return 1;
            }
            func_150E2DB4(obj, obj->unique_id, anim, -1, 0.0f, 0.0f, 156.0f, 0.0f, 0.0f, 636.0f, 3, 0xA, 0xF0, 0);
        } else if (event->value == 0x1B) {
            s32 duration;
            SeqEvent *found;

            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                // matching: the switch reads the field as an unsigned halfword
                switch (*(u16 *) &event->unk6) {
                    case 0:
                        func_1516E778(pos, duration, event->unk5, sceneTag, 0);
                        break;
                    case 1:
                        func_150BEF7C(duration);
                        break;
                    case 2: {
                        f32 x;
                        f32 y;
                        f32 z;
                        s32 angle;
                        s32 pad[3]; // matching: stack padding
                        f32 s;
                        f32 c;
                        s32 pad2; // matching: stack padding

                        x = obj->x_position;
                        y = obj->y_position;
                        z = obj->z_position;
                        angle = obj->unk7A >> 8;
                        s = func_15048A40(angle);
                        c = func_150489B0(angle);
                        func_1516F548(x + 20.0f, y + 475.0f, z, duration, 14.0f * s, 0xB00, 14.0f * c, 0xC8,
                                      0x32, 0xA, 6, 7, 0xBE, 0xBE, 0xBE, sceneTag, 0);
                        func_1516F548(x - 24.0f, y + 476.0f, z, duration, -23.0f * s, 0x800, -23.0f * c, 0xC8,
                                      0x32, 0xA, 6, 7, 0xBE, 0xBE, 0xBE, sceneTag, 0);
                        break;
                    }
                    case 3:
                        func_151DC8BC(pos, duration, 1, 1, sceneTag, 0);
                        break;
                    case 4:
                        func_151DC8BC(pos, duration, 0, 0, sceneTag, 0);
                        break;
                }
            }
        } else if (event->value == 0x1F) {
            if (obj == NULL) {
                return 1;
            }
            func_150BA4C0(obj, sceneTag, 1);
        } else if (event->value == 0) {
            if (obj == NULL) {
                return 1;
            }
            func_151A3390(obj, sceneTag);
        } else if (event->value == 0x20) {
            func_15165F80(-1, pos[0], pos[1], pos[2], 0x14, 0x14, 1, sceneTag, 1);
        } else if (event->value == 6) {
        } else if (event->value == 7) {
            func_150B2494(obj, 9, -1);
        } else if (event->value == 0x21) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_15136A50(pos, pos + 1, pos + 2, duration, sceneTag, 0);
            }
        } else if (event->value == 0x60) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_150CE150(obj, duration, sceneTag, 0);
            }
        } else if (event->value == 0x65) {
            s8 val[4];
            s32 pad; // matching: stack padding
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 2) {
                switch (event->unk5) {
                    case 0x11:
                    case 0x12:
                    case 0x15:
                        val[0] = event->unk6;
                        func_151494E0((s32)val, 0x51);
                        break;
                    case 0x13:
                        func_150CFC38(((f32)event->unk6 * 0.009999999776f * 300.0f) - 150.0f);
                        break;
                }
            }
            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 1, 1, event->unk5, scene) - eventTime;
                switch (event->unk5) {
                    case 1:
                        func_15109410(obj, duration, -1, 0, 0.000709999993f, 0, sceneTag, 1);
                        break;
                    case 2:
                        func_15109410(obj, duration, -1, 1, 0.00612499984f, 0, sceneTag, 1);
                        break;
                    case 3:
                        func_15109ED4(actor, duration, -1, 0, 0.00612499984f, 0, sceneTag, 1);
                        break;
                    case 4:
                        func_15109ED4(actor, duration, -1, 1, 0.00612499984f, 0, sceneTag, 1);
                        break;
                    case 5:
                        func_1510A344(obj, duration, sceneTag, 1);
                        break;
                    case 6:
                        func_151619A0(track, duration, sceneTag, 1);
                        break;
                    case 7:
                        func_15108FFC(track, duration, sceneTag);
                        break;
                    case 8: {
                        f32 p[3];

                        p[0] = D_800C3958[scene][track].x;
                        p[1] = D_800C3958[scene][track].y;
                        p[2] = D_800C3958[scene][track].z;
                        func_151A9390(5, 1, 0, p, -10000.0f, 100.0f, duration, sceneTag, 1);
                        break;
                    }
                    case 9: {
                        Ev15190770 sp;

                        sp.unk0 = obj;
                        sp.unk4 = obj->unique_id;
                        sp.unk6 = duration;
                        sp.unk8 = 1;
                        sp.unk9 = 0;
                        switch (obj->id) {
                            case 0x57:
                            case 0x71:
                                sp.unkA = 1;
                                break;
                            case 0x96:
                                sp.unkA = 3;
                                break;
                            default:
                                sp.unkA = 0;
                                break;
                        }
                        func_15190770(&sp, 0, sceneTag, 1);
                        break;
                    }
                    case 10:
                        func_150F802C(&D_800C3958[scene][track], duration, sceneTag, 1);
                        break;
                    case 11: {
                        s16 data[2];
                        void *p;

                        func_150AF738((s16)duration, sceneTag, 1);
                        data[0] = 0;
                        data[1] = 0;
                        p = func_15149130((s16)duration, -1, 0x4B, -1, 1, 0, 4, sceneTag, 1);
                        if (p != NULL) {
                            memcpy((u8 *)p + 0x28, data, 4);
                        }
                        break;
                    }
                    case 12:
                        func_150F5C08(obj, duration, sceneTag, 1);
                        break;
                    case 13: {
                        u16 id;

                        id = event->unk6;
                        if (id >= 0xE9) {
                            id = 0;
                        }
                        func_1519EB8C(&D_800C3958[scene][track], id, duration, sceneTag, 1);
                        break;
                    }
                    case 15:
                        if (obj != NULL && duration > 0) {
                            f32 half;

                            obj->unk2EC = 1;
                            half = (f32)duration * 0.5f;
                            obj->unk2D8[0] = half;
                            obj->unk2D8[1] = 1.0f / half;
                            func_150C4120(obj, duration, sceneTag, 1);
                        }
                        break;
                    case 16:
                        if (obj != NULL && duration > 0) {
                            func_150F6890(obj, duration, sceneTag, 1);
                        }
                        break;
                    case 17:
                        func_150D0134(event->unk6, D_800C35E0[scene][event->unk6], duration, sceneTag, 1);
                        break;
                    case 18:
                        func_150D02B4(event->unk6, D_800C35E0[scene][event->unk6], duration, sceneTag, 1);
                        break;
                    case 19:
                        func_150CF680(duration, sceneTag, 1);
                        break;
                    case 20:
                        func_151925C4(obj, duration, sceneTag, 1);
                        break;
                    case 21:
                        func_150D04C4(event->unk6, D_800C35E0[scene][event->unk6], duration, sceneTag, 1);
                        break;
                    case 22: {
                        u16 id;

                        id = event->unk6;
                        if (id >= 0xCF) {
                            id = 0;
                        }
                        func_1519ED84(&D_800C3958[scene][track], id, duration, sceneTag, 1);
                        break;
                    }
                    case 23:
                        func_150FAE18(duration, sceneTag, 1);
                        break;
                    case 24: {
                        f32 point[3];
                        f32 probe[3];
                        EvFloor floor;
                        s32 hit;

                        point[0] = D_800C3958[scene][track].x;
                        point[1] = D_800C3958[scene][track].y + 100.0f;
                        point[2] = D_800C3958[scene][track].z;
                        probe[0] = point[0];
                        probe[1] = point[1] + 100.0f;
                        probe[2] = point[2];
                        if (func_15046C80(probe, 0, point[1] - 1000.0f, &floor) != 0) {
                            if (floor.unk1D != 3) {
                                hit = 1;
                                point[1] = floor.y;
                            } else {
                                hit = 0;
                            }
                        } else {
                            hit = 0;
                        }
                        if (hit) {
                            func_15136698((func_150ADA68() * 0.02000000142f) + 0.09000000358f,
                                          (func_150ADA68() * 0.0004399999743f) + 0.0008799999487f,
                                          (func_150ADA20() % 101) + 0x9B, 0xFF, duration, &floor.unk4, point,
                                          !event->unk6, 1, sceneTag, 1);
                        }
                        break;
                    }
                    case 25: {
                        f32 from[3];
                        s32 to[3];

                        from[0] = D_800C3868[scene][track][0];
                        from[1] = D_800C3868[scene][track][1];
                        from[2] = D_800C3868[scene][track][2];
                        to[0] = D_800C3958[scene][track].x;
                        to[1] = D_800C3958[scene][track].y;
                        to[2] = D_800C3958[scene][track].z;
                        func_150E3340(from, to, &D_800C3958[scene][track], duration);
                        break;
                    }
                    case 26: {
                        Ev151D8868 sp;

                        sp.unk0 = 1;
                        sp.unk2 = duration;
                        sp.unk4 = 8;
                        sp.unk5 = 1;
                        sp.unk6 = -1;
                        func_151D8868(&sp, 0, sceneTag, 1);
                        break;
                    }
                }
            }
        } else if (event->value == 0x5B) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_151B5240((event->unk4 != 0) ? 5000 : duration, sceneTag, 0);
            }
        } else if (event->value == 0x5D) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_15192D48(obj, duration, sceneTag, 0);
            }
        } else if (event->value == 0x5E) {
            s32 duration;
            SeqEvent *found;

            if (event->unk5 == 1) {
                func_151B09BC(obj, 0, 1000, sceneTag, 0);
            } else if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_151B09BC(obj, 0, duration, sceneTag, 0);
            }
        } else if (event->value == 0x4C) {
            s32 duration;
            SeqEvent *found;
            u8 *list;
            s32 count;

            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                if (obj->id == 0x49 || obj->id == 0x58) {
                    list = func_1519003C(obj, &count);
                    if (list != NULL && event->unk6 < count) {
                        func_1518F51C(obj, list[event->unk6], duration, -1, -1, -1, 0, 0, sceneTag, 1);
                    }
                } else {
                    func_1518D1C0(obj, event->unk5, duration, 0, sceneTag, 1, "Cutscene 2");
                }
            }
        } else if (event->value == 0x53) {
            if (event->unk4 == 0) {
                s32 endTime;
                SeqEvent *found;

                endTime = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene);
                {
                    f32 *p = pos; // matching: IDO needs the pointer copied to a local here
                    Ev15134908 sp;

                    sp.unk4 = p + 1;
                    sp.unk8 = p + 2;
                    sp.unk0 = p;
                    sp.unkC = 2.0f;
                    sp.unk10 = 1.0f;
                    sp.unk14 = endTime - eventTime;
                    sp.unk16 = 1;
                    sp.unk17 = 2;
                    func_15134908(&sp, 0, sceneTag, 0);
                }
            }
        } else if (event->value == 0x50) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_150E9FE0(obj, duration, sceneTag, 0);
            }
        } else if (event->value == 0x51) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_150EA500(obj, duration, sceneTag, 0);
            }
        } else if (event->value == 0x48) {
            f32 offset[2];
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 1, event->unk5, scene) - eventTime;
                offset[0] = (func_150ADA68() * 200.0f) + -100.0f;
                offset[1] = (func_150ADA68() * 300.0f) + -150.0f;
                func_151356D4(offset, (func_150ADA68() * 20.0f) + 10.0f,
                              ((func_150ADA68() * 65.0f) + 35.0f) * 0.001000000047f,
                              ((func_150ADA68() * 160.0f) + 200.0f) * 0.001000000047f,
                              (func_150ADA20() % 101) + 100, 0xFF, (func_150ADA20() % 3) + 4, duration, 4,
                              0x40, sceneTag, 1);
            }
        } else if (event->value == 0xF) {
            s32 duration;
            SeqEvent *found;

            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                func_150C7020(obj, (s16)duration, sceneTag, 1);
            }
        } else if (event->value == 0x3E || event->value == 0x3F) {
        } else if (event->value == 0x32) {
            s32 duration;
            SeqEvent *found;

            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                func_150DB518(pos, &D_800C3958[scene][track].unk10, &D_800C3958[scene][track].unk18,
                              &D_800C3958[scene][track].unk1C, &D_800C3958[scene][track].unk20, duration,
                              sceneTag, 0);
            }
        } else if (event->value == 0x34) {
            s32 duration;
            SeqEvent *found;
            u8 t;
            u8 kind;
            EvNode *node;

            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                t = (event->unk5 <= 0) ? event->unk5 : 0;
                if (t == 0) {
                    kind = 0xD;
                }
                node = (EvNode *)obj->attachments; // EvNode: this file's view of struct Attachment
                while (node != NULL) {
                    if (node->unk1C == kind || func_1514D4B8(D_8008ABE8[kind]->unk4, node->unk1C) != 0) {
                        node->unk10->unk30 = func_1519E970(duration, node->unk10, 0,
                                                           &D_800C3958[scene][track].unk30,
                                                           &D_800C3958[scene][track].unk20, sceneTag, 0);
                    }
                    node = node->unk14;
                }
            }
        } else if (event->value == 0x23) {
            s32 duration;
            SeqEvent *found;

            if (obj == NULL) {
                return 1;
            }
            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                func_1516F2F8(obj - D_800CC2D0, duration, duration, 0, sceneTag, 0);
            }
        } else if (event->value == 0x22) {
            s32 duration;
            SeqEvent *found;

            if (obj == NULL) {
                return 1;
            }
            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                func_1516F1C0(obj - D_800CC2D0, duration, sceneTag, 0);
            }
        } else if (event->value == 0x52) {
            if (obj == NULL) {
                return 1;
            }
            func_15136404(obj, 90.0f, sceneTag, 1);
        } else if (event->value == 0x5F) {
            // matching: the switch reads the field as an unsigned byte
            switch (*(u8 *) &event->unk4) {
                case 0:
                    if (obj != NULL) {
                        f32 pos[3];
                        EvFloor floor;
                        f32 out[3];

                        func_1504715C(&floor, obj);
                        pos[0] = obj->x_position;
                        pos[1] = obj->y_position + 100.0f;
                        pos[2] = obj->z_position;
                        if (func_15046C80(pos, 0, obj->y_position - 300.0f, &floor) != 0) {
                            out[0] = pos[0];
                            out[1] = floor.y;
                            out[2] = pos[2];
                            func_150CA930(out);
                        }
                    }
                    break;
                case 1:
                    func_151090DC();
                    break;
                case 2: {
                    ActorCtl *bone;
                    f32 vec[3];

                    if (obj != NULL) {
                        switch (obj->id) {
                            case 0x5A:
                            case 0x74:
                            case 0x75:
                            case 0x7A:
                                bone = func_1503195C(obj, 0x46, 0);
                                if (bone != NULL) {
                                    f32 *p1;
                                    f32 *p2;

                                    p1 = (f32 *)&D_800AAA90;
                                    p2 = vec;
                                    if (func_1514654C(obj, bone, 0, &p1, &p2, 1) != 0) {
                                        func_151C5588(obj, vec, 0x46, sceneTag, 1);
                                        func_151C56A4(vec, sceneTag, 1);
                                    }
                                }
                                break;
                            case 0x86:
                                func_150FB324(obj, sceneTag, 1);
                                break;
                            case 0x4A:
                                func_150E06D8(obj, event->unk5, sceneTag, 1);
                                break;
                            case 0x97:
                                func_150F97EC(obj, sceneTag, 1);
                                break;
                            case 0x98:
                                func_150F9950(obj, sceneTag, 1);
                                break;
                        }
                    }
                    break;
                }
                case 3: {
                    f32 p[3];

                    p[0] = D_800C3958[scene][track].x;
                    p[1] = D_800C3958[scene][track].y;
                    p[2] = D_800C3958[scene][track].z;
                    func_151BE850(p, D_800C3958[scene][track].unk10, 1, 0, 2, sceneTag, 1);
                    break;
                }
                case 4:
                    func_151CFA4C(obj, sceneTag, 1);
                    break;
                case 5:
                    if (obj != NULL) {
                        ActorCtl *bone;

                        bone = NULL;
                        if (event->unk6 != 0) {
                            bone = func_1503195C(obj, event->unk5 ? 0xA4 : 0xA5, 0);
                        }
                        func_151C62D0(obj, event->unk5, 1, bone, event->unk5 ? 0xA4 : 0xA5, sceneTag, 1);
                    }
                    break;
                case 6:
                    func_150BAA14(obj, 9, -1);
                    break;
                case 7:
                    func_150E0870(obj, sceneTag, 1);
                    break;
                case 8: {
                    f32 p[3];

                    p[0] = pos[0];
                    p[1] = pos[1];
                    p[2] = pos[2];
                    func_151D5648(p, sceneTag, 1);
                    break;
                }
                case 9:
                    func_15136C3C(obj, 1, 1, 1, 1, 0, sceneTag, 1);
                    break;
                case 10: {
                    if (obj->unk1D4 != NULL) {
                        const f32 *sel;
                        const f32 a[3][3] = { { 73.0f, 14.0f, 0.0f }, { 68.0f, -10.0f, 8.0f }, { 62.0f, 1.0f, -5.0f } };
                        const f32 b[4][3] = { { -55.0f, -6.0f, -1.0f }, { -47.0f, -1.0f, -10.0f }, { -53.0f, -14.0f, 6.0f }, { -57.0f, 10.0f, 6.0f } };
                        const f32 c[6][3] = { { -1.0f, 123.0f, 10.0f }, { -10.0f, 116.0f, -1.0f }, { -36.0f, 88.0f, 6.0f }, { 4.0f, 112.0f, 6.0f }, { -6.0f, 112.0f, -18.0f }, { -14.0f, 101.0f, 34.0f } };
                        const s32 bones[3] = { 0x15, 0x20, 3 };
                        f32 out[3];

                        switch (event->unk5) {
                            case 0:
                                sel = a[func_150ADA20() % 3];
                                break;
                            case 1:
                                sel = b[func_150ADA20() & 3];
                                break;
                            case 2:
                                sel = c[func_150ADA20() % 6];
                                break;
                        }
                        func_15143134(sel, out, &((MtxF *) obj->unk1D4)[bones[event->unk5]]);
                        func_151C329C(out, sceneTag, 1);
                    }
                    break;
                }
                case 11: {
                    u8 i;
                    const s32 ids[4] = { 6, 7, 8, 9 };

                    for (i = 0; i < 4; i++) {
                        func_151C970C(1, &D_800D3098[ids[i]]);
                    }
                    break;
                }
                case 12:
                    func_15107B78(obj, func_150ADA20() & 0xFF, (func_150ADA20() & 0x7F) - 0x40, sceneTag, 1);
                    break;
                case 13:
                    func_151CC290(D_800DBFF0);
                    break;
                case 14: {
                    f32 p[3];
                    u16 id;
                    f32 scale;

                    p[0] = pos[0];
                    p[1] = pos[1];
                    p[2] = pos[2];
                    id = event->unk5;
                    if (id >= 0xE9) {
                        id = 0;
                    }
                    if (event->unk6 != 0) {
                        scale = event->unk6 * 0.009999999776f;
                    } else {
                        scale = 1.0f;
                    }
                    func_1519EA78(p, id, scale, sceneTag, 1);
                    break;
                }
                case 15: {
                    u8 colour[4];
                    f32 size;

                    colour[0] = 0xAE;
                    colour[1] = 0xD2;
                    colour[2] = 0xFF;
                    colour[3] = (func_150ADA20() % 61) + 0x96;
                    size = (func_150ADA68() * 20.0f) + 25.0f;
                    if (event->unk5 != 0) {
                        size *= 0.349999994f;
                    }
                    func_15107700(obj, func_150ADA20() & 0xFF, (func_150ADA20() % 129) - 0x3F,
                                  (func_150ADA20() % 21) + 0xA, 4, 40.0f, size, 0, 2, colour, sceneTag, 1);
                    break;
                }
                case 16:
                    func_150F16DC(obj, event->unk5, (u8) event->unk6, sceneTag, 1);
                    break;
                case 17: {
                    s32 unused; // matching: stack padding
                    Ev151D8868 sp;

                    sp.unk0 = 1;
                    sp.unk2 = 0x32;
                    sp.unk4 = 8;
                    sp.unk5 = 1;
                    sp.unk6 = -1;
                    func_151D8868(&sp, 0, sceneTag, 1);
                    break;
                }
                case 18: {
                    if (obj->unk1D4) {
                        f32 out[3];
                        const f32 offs[2][3] = { { -65.0f, 172.0f, 9.0f }, { 15.0f, 63.0f, 9.0f } };

                        func_15143134(offs[event->unk5], out, obj->unk1D4);
                        func_15131EE4(out, 0.6460000277f, sceneTag, 0);
                    }
                    break;
                }
                case 19: {
                    if (obj->unk1D4) {
                        f32 out[3];
                        const f32 offs[8][3] = { { 0.0f, -14.0f, 18.0f }, { 0.0f, -14.0f, 18.0f }, { 0.0f, -14.0f, 18.0f }, { 0.0f, -14.0f, 18.0f }, { -5.0f, 25.0f, -7.0f }, { -11.0f, 28.0f, -5.0f }, { -4.0f, 30.0f, -4.0f }, { 3.0f, 30.0f, -8.0f } };
                        const u8 bones[8] = { 7, 8, 9, 10, 13, 14, 15, 16 };

                        func_15143134(offs[event->unk5], out, &((MtxF *) obj->unk1D4)[bones[event->unk5]]);
                        func_15131EE4(out, 6.990000248f, sceneTag, 0);
                    }
                    break;
                }
                case 20: {
                    SeqTrackState *state;
                    struct {
                        u8 pad0[0x10];
                        s16 x;
                        s16 y;
                        s16 z;
                        u8 pad16[0x86];
                    } sp;

                    state = &D_800C3958[scene][track];
                    sp.x = state->x;
                    sp.y = state->y + 50.0f;
                    sp.z = state->z;
                    func_15170B90(&sp, 1, 1, 2, sceneTag, 0);
                    break;
                }
                case 21:
                    if (obj != NULL) {
                        func_1507DE4C(obj);
                        func_15138C80(obj, sceneTag, 0);
                    }
                    break;
                case 22:
                    if (obj != NULL) {
                        func_15193660(obj, sceneTag, 0);
                    }
                    break;
            }
        } else if (event->value == 0x40) {
            func_151A0A10(obj, 0x28, sceneTag, 1);
        } else if (event->value == 0x4D) {
            struct {
                s16 unk0;
                f32 unk4;
                f32 unk8;
                f32 unkC;
                s16 unk10;
                f32 unk14;
                s16 unk18;
                s16 unk1A;
                s16 unk1C;
                s16 unk1E;
                f32 unk20;
                f32 unk24;
                u8 pad28[8];
                f32 unk30;
                f32 unk34;
                f32 unk38;
                f32 unk3C;
                f32 unk40;
                f32 unk44;
                f32 unk48;
                f32 unk4C;
                f32 unk50;
                s8 unk54;
                s8 unk55;
                s16 unk56;
                s16 unk58;
            } sp;

            sp.unk0 = (func_150ADA20() % 5) + 5;
            sp.unk4 = pos[0];
            sp.unk8 = pos[1];
            sp.unkC = pos[2];
            sp.unk14 = 4.700000286f;
            sp.unk20 = 24.80000114f;
            sp.unk24 = 15.5f;
            sp.unk3C = 0.2670000196f;
            sp.unk10 = 0x5A;
            sp.unk18 = -7;
            sp.unk1A = 0x2D;
            sp.unk1C = -0x34;
            sp.unk1E = 0x20;
            sp.unk54 = 0;
            sp.unk55 = 0;
            sp.unk56 = 0x32;
            sp.unk58 = 0x19;
            sp.unk40 = 0.2430000156f;
            sp.unk44 = -2.041000128f;
            sp.unk30 = 0.0f;
            sp.unk34 = 0.0f;
            sp.unk38 = 0.0f;
            sp.unk48 = 0;
            sp.unk4C = -3.0f;
            sp.unk50 = 6.0f;
            func_151A6F00(&sp, 1, sceneTag, 1);
        } else if (event->value == 0x4E) {
            struct {
                s16 unk0;
                f32 unk4;
                f32 unk8;
                f32 unkC;
                s16 unk10;
                f32 unk14;
                s16 unk18;
                s16 unk1A;
                s16 unk1C;
                s16 unk1E;
                f32 unk20;
                f32 unk24;
                u8 pad28[8];
                f32 unk30;
                f32 unk34;
                f32 unk38;
                f32 unk3C;
                f32 unk40;
                f32 unk44;
                f32 unk48;
                f32 unk4C;
                f32 unk50;
                s8 unk54;
                s8 unk55;
                s16 unk56;
                s16 unk58;
            } sp;

            sp.unk0 = (func_150ADA20() % 6) + 6;
            sp.unk4 = pos[0];
            sp.unk8 = pos[1];
            sp.unkC = pos[2];
            sp.unk14 = 151.5f;
            sp.unk20 = 15.10000038f;
            sp.unk24 = 25.20000076f;
            sp.unk3C = 0.3880000114f;
            sp.unk10 = 0x5A;
            sp.unk1A = 0xFF;
            sp.unk18 = 0;
            sp.unk1C = -0x40;
            sp.unk1E = 0x28;
            sp.unk54 = 0;
            sp.unk55 = 0;
            sp.unk56 = 0x32;
            sp.unk58 = 0x19;
            sp.unk40 = 0.2430000156f;
            sp.unk44 = -1.0200001f;
            sp.unk30 = 0.0f;
            sp.unk34 = 0.0f;
            sp.unk38 = 0.0f;
            sp.unk48 = 0;
            sp.unk4C = -4.0f;
            sp.unk50 = 8.0f;
            func_151A6F00(&sp, 1, sceneTag, 1);
        } else if (event->value == 0x4F) {
            SplashSpraySpec sp;

            sp.spray.pos.x = pos[0];
            sp.spray.pos.y = pos[1];
            sp.spray.pos.z = pos[2];
            sp.spray.scaleBase = 2.900000095f;
            sp.spray.scaleRange = 17.0f;
            sp.spray.sizeBase = 2.5f;
            sp.spray.countBase = 0xF;
            sp.spray.sizeRange = 0.5770000219f;
            sp.spray.speedBase = 25.0f;
            sp.spray.speedRange = 20.20000076f;
            sp.spray.countRange = 0xA;
            sp.angles.yawBase = -0x14;
            sp.angles.yawRange = 0x31;
            sp.angles.pitchBase = -0x34;
            sp.angles.pitchRange = 0x2B;
            sp.spray.itemCountBase = 3;
            sp.spray.itemCountRange = 4;
            sp.spray.lifeBase = 0x28;
            sp.spray.lifeRange = 0x1E;
            sp.spray.unk30 = 0x64;
            sp.spray.unk32 = 0x64;
            sp.spray.kind = 0;
            sp.spray.unk3C = 0x10;
            sp.spray.unk3E = 0xF;
            sp.spray.unk40 = 0;
            sp.spray.unk38 = 0.5f;
            func_15153F18(&sp.angles, &sp.spray, 0, sceneTag, 1);
        } else if (event->value == 0x24) {
            f32 p[3];
            s32 count;
            u8 type;

            p[0] = pos[0];
            p[1] = pos[1];
            p[2] = pos[2];
            count = (event->unk4 != 0) ? event->unk4 : 0xA;
            type = event->unk5;
            if (type >= 0xD) {
                type = 0;
            }
            func_151DBAA8(p, count, type, sceneTag, 0);
        } else if (event->value == 0x10) {
            u8 type;

            type = event->unk4;
            if (type >= 0x1B) {
                type = 0;
            }
            func_151AF4D0(obj, type, sceneTag, 0);
        } else if (event->value == 0x47) {
            func_151925C4(obj, 0x32, sceneTag, 0);
        } else if (event->value == 0x2E) {
            func_150C1F60(sceneTag);
            func_150C2290(sceneTag);
            func_150C2424(sceneTag);
        } else if (event->value == 0x31) {
            func_150DAE70(obj, 0, sceneTag, 0);
        } else if (event->value == 0x33) {
            u8 type;
            SeqTrackState *state;
            s32 i4, i5, i6;
            f32 f7, f9, f10;
            s16 n;

            if (event->unk4 >= 0x10) {
                type = 0xF;
            } else {
                type = event->unk4;
            }
            state = &D_800C3958[scene][track];
            i4 = state->unk20 * 0.05000000075f;
            i5 = (state->unk10 + 180.0f) * 0.7111111283f;
            i6 = state->unkC * 0.7111111283f;
            f7 = (state->yOffset * 0.1000000015f) + 0.5f;
            f9 = (state->unk18 * 0.1000000015f) + 1.0f;
            f10 = state->unk1C * 0.1000000015f;
            n = (event->unk5 != 0) ? event->unk5 : 0x64;
            func_15148F1C(type, pos[0], pos[1], pos[2], i4, i5, i6, f7, n, f9, f10, sceneTag);
        } else if (event->value == 0x35) {
            s32 type;

            type = (event->unk4 >= 2) ? 1 : event->unk4;
            func_150DB9E0(obj, type, sceneTag);
        } else if (event->value == 0x26) {
            SeqEvent *found;
            s32 t;

            if (obj == NULL) {
                return 1;
            }
            t = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene);
            if (event->unk4 == 0) {
                obj->unk276 = 1;
                obj->unk282 = t - eventTime;
            }
        } else if (event->value == 0x27) {
            f32 speed;
            f32 dist;
            s32 n;

            speed = 1.0f;
            dist = 30.0f;
            n = 1;
            if (event->unk4 != 0) {
                n = event->unk4;
            }
            if (event->unk5 != 0) {
                speed = event->unk5 * 0.1000000015f;
            }
            if (event->unk6 >= 0) {
                dist = event->unk6;
            }
            func_1518B264(pos, dist, speed, n, sceneTag);
        } else if (event->value == 2) {
            f32 speed;
            f32 dist;
            s32 n;

            speed = 1.0f;
            dist = 30.0f;
            n = 1;
            if (event->unk4 != 0) {
                n = event->unk4;
            }
            if (event->unk5 != 0) {
                speed = event->unk5 * 0.1000000015f;
            }
            if (event->unk6 >= 0) {
                dist = event->unk6;
            }
            func_1518B2A8(pos, dist, speed, n, 0x1E, 0x64, sceneTag);
        } else if (event->value == 0x28) {
            if (actor != NULL) {
                func_15171600(actor, sceneTag, 0);
            } else if (obj != NULL) {
                func_15171BF4(obj, sceneTag);
            }
        } else if (event->value == 0x29) {
            if (actor != NULL) {
                func_15170EC4(actor, sceneTag, 0);
            } else if (obj != NULL) {
                func_151711C4(obj);
            }
        } else if (event->value == 0x2A) {
        } else if (event->value == 0x2B) {
            func_1518B6B0(pos[0], pos[1], pos[2], sceneTag, 0);
        } else if (event->value == 0x2C) {
            SeqEvent *found;
            s32 pad[7]; // matching: stack padding

            func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene);
            if (event->unk4 == 0) { // matching: empty, but it still shapes the code
            }
        } else if (event->value == 0x2F) {
            s32 duration;
            SeqEvent *found;

            if (obj == NULL) {
                return 1;
            }
            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_1516ED68(obj - D_800CC2D0, duration, sceneTag, 0);
            }
        } else if (event->value == 0x39) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_150C2804(pos, pos + 1, pos + 2, duration, sceneTag, 0);
            }
        } else if (event->value == 0x3A) {
            f32 p[3];
            s32 count;

            p[0] = pos[0];
            p[1] = pos[1];
            p[2] = pos[2];
            count = (event->unk4 != 0) ? event->unk4 : 0xA;
            func_151DBAA8(p, count, 6, sceneTag, 0);
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

extern void *allocate_memory(s32 size, s32 arg1, s32 arg2, s32 arg3);
extern void func_1000D96C(s32, s32, s32);
extern void func_1000DE1C(s32, s32);
extern s32 func_1000E134(s32);
extern void func_1503DE70(Actor *, s32, s32);
extern void func_151557FC(s32, s32, f32);
extern void func_15155F90(void);
extern void func_15178EFC(s32);
extern void func_1517EE40(s32, s32, s32, s32, s32, s32);
extern void func_15188810(Actor *, s32, s32);
extern void func_1518894C(WorldObject *, s32, s32);
extern void func_15188AD0(Actor *);
extern void func_151D66F0(s32, s32);

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} SeqColor;

extern SeqColor D_80084110[];
extern s8 D_800C3C8A;
extern u16 D_800C3C8E;

/*
 * Third cutscene event handler: sounds (types 6 and 10), func_1000xxxx audio
 * commands (8), object animations (2) and a few type-5 commands. `skipping` is set when the scene is
 * being skipped (D_800C3C88 == 2). Returns 1 when the event was handled.
 */
s32 func_15029BB8(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene, Actor *obj, WorldObject *actor, f32 *pos, s32 sceneTag, s32 skipping) {
    if ((event->type == 6) || (event->type == 10)) {
        s32 vol;
        s32 duration;
        s32 markTime;
        s32 clamped;
        SeqEvent *found;
        s32 fade;
        s32 fadeOut;
        s32 fadeIn;
        s32 sflags;
        SeqSound *snd;

        found = NULL;
        sflags = 0;
        if ((event->unk4 == 0) || (event->type == 6)) {
            if (event->unk6 == 0) {
                vol = 0xFF;
            } else {
                vol = event->unk6 & 0xFF;
                if (vol < 0) {
                    vol = 0;
                } else {
                    // matching: IDO needs the clamp result in its own local
                    clamped = (vol > 0xFF) ? 0xFF : vol;
                    vol = clamped;
                }
            }
            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 1, 0, 0, scene) - eventTime;
            if (event->type == 6) {
                duration = 0xFFFF;
            } else if (found == NULL) {
                duration = 0xFFFF;
            }
            if ((skipping != 0) && ((event->type != 10) || (found != NULL))) {
                return 1;
            }
            fade = event->unk5 * 50;
            fadeOut = 0;
            fadeIn = 0;
            if (event->type == 10) {
                markTime = func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 2, 0, 0, scene);
                if (found != NULL) {
                    markTime -= eventTime;
                    if (markTime < duration) {
                        fadeIn = markTime;
                    }
                }
                markTime = func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 3, 0, 0, scene);
                if (found != NULL) {
                    markTime -= eventTime;
                    if (markTime < duration) {
                        fadeOut = duration - markTime;
                    }
                }
            } else if (event->unk4 == 2) {
                sflags = 0x800;
            } else {
                sflags = 0x400;
            }
            snd = allocate_memory(sizeof(SeqSound), 1, 0, 0);
            if (snd != NULL) {
                snd->sceneTag = D_800C35E8[scene];
                snd->scene = scene;
                snd->track = track;
                snd->remaining = duration;
                snd->duration = duration;
                snd->fadeIn = fadeIn;
                snd->fadeOut = fadeOut;
                snd->volume = (vol * 0x7FFF) >> 8;
                snd->stopAtEnd = event->type == 10;
                func_1000FA64(event->value, (s16)pos[0], (s16)pos[1], (s16)pos[2], snd->volume, 0x7D00, 0x7918,
                              (SoundRecordCallback)func_150238D8, snd, 0, sflags, fade);
            }
        }
    } else if (event->type == 8) {
        s32 duration;
        SeqEvent *found;

        if (event->unk4 == 0) {
            if ((skipping != 0) && (func_1000E134(event->value) != 0)) {
                return 1;
            }
            func_1000D96C(event->value, 0, 0);
            if (event->unk6 != 0) { // matching: empty under -g3 but it still shapes the code
            }
        } else if (event->unk4 == 1) {
            func_1000E588(event->value, event->unk6 * 100, 1 << (event->unk5 + 31));
        } else if (event->unk4 == 2) {
            func_1000DF68(event->value, event->unk6 * 328, 1);
        } else if (event->unk4 == 3) {
        } else if (event->unk4 == 4) {
            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 5, 1, event->unk5, scene) - eventTime;
            if (skipping != 0) {
                func_1000E588(event->value, event->unk6, 1 << (event->unk5 + 31));
            } else {
                func_1000E46C(event->value, event->unk6, 1 << (event->unk5 + 31), duration);
            }
        } else if (event->unk4 == 6) {
            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 7, 0, 0, scene) - eventTime;
            if (duration < 2) {
                duration = 1;
            }
            func_1000DF68(event->value, event->unk6 * 328, duration);
        } else if (event->unk4 == 8) {
            func_1000E654(event->value, event->unk6 + 1, 0, -1);
        } else if (event->unk4 == 9) {
            func_1000DE1C(event->value, 0);
        }
    } else if (event->type == 2) {
        f32 speed;
        f32 rate;
        f32 duration;
        SeqEvent *found;
        s32 loop;
        s32 id;

        if (obj == NULL) {
            return 1;
        }
        duration = func_15023BB0(event, event->type, track, &found, 0, 0, 0, 0, 0, 0, scene) - eventTime;
        if (event->unk4 == 0) {
            if (obj->unk2D0 == NULL) {
                return 1;
            }
            rate = 5.0f;
            id = event->value;
            if (event->unk6 > 0) {
                rate = event->unk6 - 1;
            }
            if (eventTime == D_800C3C8E) {
                rate = 0.0f;
            }
            if (skipping != 0) {
                rate = 0.0f;
            }
            if (found != NULL) {
                loop = !found->unk4;
                if ((found->unk4 == 1) && (found->unk5 == 1)) {
                    loop = 1;
                }
            } else {
                loop = 1;
            }
            if (id != obj->unk84.uh) {
                func_1505E650(obj, id, 1.0f, rate, 0.0f, 0.0f, loop);
            }
            if (duration > 0.0f) {
                speed = obj->unk2D0->unk18 / duration * 2.0f;
            } else {
                speed = 0.0f;
            }
            if (event->unk5 != 0) {
                speed = 0.0f;
            }
            func_1505E650(obj, id, speed, rate, 0.0f, 0.0f, loop);
        }
    } else if (event->type == 5) {
        if (event->value == 1) {
            SeqEvent *found;
            s32 duration;
            SeqEvent *prev;
            s32 prevTime;

            if (event->unk4 == 0) {
                duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                if (skipping != 0) {
                    duration = 0;
                }
                func_1517EE40(D_80084110[event->unk5].r, D_80084110[event->unk5].g, D_80084110[event->unk5].b, duration, !event->unk6, 0);
                D_800C3C8A = !event->unk6;
            } else if (skipping != 0) {
                func_15023DE0(&prev, &found, NULL, &prevTime, track, event->type, 1, event->value, 1, 0, D_800C35B0[scene], scene);
                if (prev != NULL) {
                    func_1517EE40(D_80084110[prev->unk5].r, D_80084110[prev->unk5].g, D_80084110[prev->unk5].b, 0, !prev->unk6, 0);
                    D_800C3C8A = !prev->unk6;
                }
            }
        } else if (event->value == 4) {
            s32 kind;

            if (obj != NULL) {
                if (skipping == 0) {
                    kind = (obj->id == 0x4B) ? 2 : -1;
                    if (kind != -1) {
                        // matching: the original reuses the (otherwise unused) index parameter as the temp
                        index = (event->unk4 == 0) ? -1 : event->unk4 - 1;
                        func_1503DE70(obj, kind, index);
                    }
                } else {
                    func_15060F28(obj, 1);
                }
            }
        } else if (event->value == 0x5C) {
            SeqEvent *found;
            s32 fadeIn;
            s32 fadeOut;
            s32 start;

            if (event->unk4 == 0) {
                if (skipping == 0) {
                    start = func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 1, 1, event->unk5, scene);
                    fadeIn = func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 2, 1, event->unk5, scene) - eventTime;
                    if (found == NULL) {
                        fadeIn = 0;
                    }
                    fadeOut = start - func_15023BB0(event, event->type, track, &found, 1, event->value, 1, 3, 1, event->unk5, scene);
                    if (found == NULL) {
                        fadeOut = 0;
                    }
                    func_150F5420(fadeIn, start - eventTime - fadeOut - fadeIn, fadeOut, event->unk5);
                }
            }
        } else if (event->value == 0x56) {
            if (event->unk4 == 0) {
                if (skipping == 0) {
                    func_15178EFC(3);
                }
            } else if (skipping != 0) {
                func_15178EFC(2);
            } else if (event->unk5 == 0) {
                func_15178EFC(0);
            } else {
                func_15178EFC(1);
            }
        } else if (event->value == 0x76) {
            s32 mode;

            if (event->unk4 == 1) {
                mode = 0;
            } else if (event->unk5 == 1) {
                mode = 2;
            } else {
                mode = 1;
            }
            func_151D66F0(event->unk6, mode);
        } else if (event->value == 0x2D) {
            s32 duration;
            SeqEvent *found;

            duration = func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (actor != NULL) {
                if (event->unk4 == 0) {
                    func_1518894C(actor, duration, event->unk5);
                } else if (skipping != 0) { // matching: empty, but it still shapes the code
                }
            } else if (obj != NULL) {
                if (event->unk4 == 0) {
                    func_15188810(obj, duration, event->unk5);
                } else if (skipping != 0) {
                    func_15188AD0(obj);
                }
            }
        } else if (event->value == 0x57) {
            s32 duration; // matching: unused, but its stack slot is needed
            SeqEvent *found;

            if (event->unk4 == 0) {
                func_151557FC(0, func_15023BB0(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime, 0.0f);
            } else if (skipping != 0) {
                D_800C3E78 = 0;
                func_15155F90();
            }
        }
    } else {
        return 0;
    }
    return 1;
}

/*
 * Dispatches one cutscene event: resolves the track's object/actor and position,
 * then offers the event to each handler until one accepts it.
 */
void func_1502A8A0(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene) {
    Actor *obj;
    WorldObject *actor;
    f32 *pos;
    SeqTrackInfo *info;
    s32 sceneTag;

    obj = NULL;
    actor = NULL;
    pos = NULL;
    sceneTag = D_800C35E8[scene];
    info = &D_800C35F0[scene][track];
    if (info->targetType == 2) {
        obj = func_15083E90(info->targetId);
        if (obj == NULL) {
            return;
        }
        pos = &obj->x_position;
    } else if (info->targetType == 3) {
        actor = func_151149AC(info->targetId);
        if (actor == NULL) {
            return;
        }
    }
    if (pos == NULL) {
        pos = &D_800C3958[scene][track].x;
    }
    if (D_800C3C88 != 2) {
        if (func_1502460C(event, track, index, eventTime, scene, obj, actor, pos, sceneTag)) {
            return;
        }
        if (func_150265CC(event, track, index, eventTime, scene, obj, actor, pos, sceneTag)) {
            return;
        }
        if (func_15029BB8(event, track, index, eventTime, scene, obj, actor, pos, sceneTag, 0)) {
            return;
        }
    } else {
        if (func_1502460C(event, track, index, eventTime, scene, obj, actor, pos, sceneTag)) {
            return;
        }
        func_15029BB8(event, track, index, eventTime, scene, obj, actor, pos, sceneTag, 1);
    }
}
