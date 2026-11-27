#include <ultra64.h>
#include "conker/actor.h"
#include "conker/attach.h"
#include "conker/audio.h"
#include "conker/beam.h"
#include "conker/blood.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/env.h"
#include "conker/event.h"
#include "conker/face.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/heap.h"
#include "conker/hud.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/math.h"
#include "conker/model.h"
#include "conker/mp3.h"
#include "conker/music.h"
#include "conker/particle.h"
#include "conker/prop.h"
#include "conker/save.h"
#include "conker/screen.h"
#include "conker/sight.h"
#include "conker/sound.h"
#include "conker/sprite.h"
#include "conker/swarm.h"
#include "conker/text.h"
#include "conker/texture.h"
#include "conker/trail.h"
#include "conker/view.h"
#include "conker/weapon.h"
#include "conker/zone.h"

/* matching: not in actor.h, as game/Effects/TintedObject/tintedobject.c was compiled with an s32 return (definition: game/Actor/Movement/movement.c) */
void actor_seatRider(Actor *mount, f32 height, s32 baseMtx, s32 attachFlags, s32 hideRider, s32 mountWalk, s32 immune,
                   s32 hitRoute, s32 force, s32 soundCmd);

/* Screen fade colours for screen_startViewFade, picked by sequence events (black, white). */
Color24 gCutsceneFadeColours[2] = { { 0, 0, 0 }, { 0xFF, 0xFF, 0xFF } };

/* Cutscene state, indexed by scene (and track). */

void cutscene_emptyStartHook(void) {
}


/*
 * Per-frame callback for a cutscene sound: follows its track's position, counts
 * down its lifetime and applies fade-in/fade-out to *volume. Returns 1 (after
 * freeing the state) when the sound should stop.
 */
s32 cutscene_updateSound(SoundRecord *voice, s32 active, u32 *volume, s32 pan, s32 cents, s32 mix, s16 *sound) {
    SeqSound *snd;
    SeqTrackState *states;
    SeqTrackState *state;
    s32 fadeOut;

    snd = voice->callback_param1.ptr;
    if ((snd->stopAtEnd != 0 && gCutsceneTimes[snd->scene] >= gCutsceneEndTimes[snd->scene]) || (voice->flags & 0x80)) {
        *volume = 0;
        *sound = 0;
        heap_free(snd);
        return 1;
    }
    if (snd->sceneTag == gCutsceneIds[snd->scene]) {
        states = gSeqTrackStates[snd->scene];
        if (states != NULL) {
            voice->x = states[snd->track].x;
            state = &gSeqTrackStates[snd->scene][snd->track];
            voice->y = state->yOffset + state->y;
            voice->z = gSeqTrackStates[snd->scene][snd->track].z;
        }
    }
    if (snd->remaining != 0xFFFF) {
        if (gFrameTicks < snd->remaining) {
            snd->remaining -= gFrameTicks;
        } else {
            *volume = 0;
            *sound = 0;
            heap_free(snd);
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
    if (snd->remaining != 0xFFFF && gCutsceneSkipState != 0) {
        *volume = *volume * gCutsceneSkipFadeTimer / 30;
        if (gFrameTicks >= gCutsceneSkipFadeTimer) {
            *volume = 0;
            *sound = 0;
            heap_free(snd);
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
s32 cutscene_findEvent(SeqEvent *after, s32 type, s32 track, SeqEvent **found, s32 matchValue, s32 value,
                  s32 matchUnk4, s32 unk4, s32 matchUnk5, s32 unk5, s32 scene) {
    SeqEvent *ev;
    s32 time;

    if (gSeqEventTracks[scene][track] != NULL) {
        time = gSeqEventTracks[scene][track]->value;
        ev = gSeqEventTracks[scene][track] + 4;
        while ((u32) (ev - gSeqEventTracks[scene][track]) < gSeqTrackEventCounts[scene][track]) {
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
                    if ((u32) (ev - gSeqEventTracks[scene][track]) >= gSeqTrackEventCounts[scene][track]) {
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
    return gCutsceneEndTimes[scene];
}

/*
 * Finds the last matching event at or before `time` (*prev, *prevTime) and the
 * first matching event after it (*next, *nextTime) on `track` of `scene`.
 * Events chained at the same time are compared against the scene's current time.
 */
void cutscene_findEventsAround(SeqEvent **prev, SeqEvent **next, s32 *prevTime, s32 *nextTime, s32 track, s32 type,
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
    *nextTime = gCutsceneEndTimes[scene];

    if (gSeqEventTracks[scene][track] == NULL) {
        return;
    }

    ev = gSeqEventTracks[scene][track] + 4;
    evTime = gSeqEventTracks[scene][track]->value;
    while ((u32) (ev - gSeqEventTracks[scene][track]) < gSeqTrackEventCounts[scene][track]) {
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
                if ((u32) (ev - gSeqEventTracks[scene][track]) >= gSeqTrackEventCounts[scene][track]) {
                    break;
                }
                sceneTime = gCutsceneTimes[scene];
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
void cutscene_flushEventQueue(s32 count, s32 scene) {
    s32 i;

    for (i = 0; i < count; i++) {
        cutscene_dispatchEvent(gSeqEventQueue[i].event, gSeqEventQueue[i].track, gSeqEventQueue[i].index, gSeqEventQueue[i].time, scene);
    }
}

/*
 * Inserts an event into the time-sorted dispatch queue. When the queue is full
 * it is flushed (dispatched) first and the new event is dropped.
 */
void cutscene_queueEvent(SeqQueueEntry *queue, s32 *count, SeqEvent *event, s32 track, s32 index, s32 time, s32 scene) {
    s32 pos;
    s32 i;

    if (*count >= gSeqEventQueueSize) {
        cutscene_flushEventQueue(*count, scene);
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

/*
 * Queues and dispatches every event of `scene` whose time falls in
 * (previous time, current time]. Mode 0 dispatches all event types except 0x11,
 * mode 1 only type 0x11.
 */
void cutscene_dispatchEvents(s32 mode, s32 scene) {
    s32 track;
    SeqEvent *ev;
    s32 time;
    s32 queued;

    // matching: indexing through &gCutsceneStates[0] rather than the array changes register allocation
    if ((gCutsceneHoldEnd != 2) && ((&gCutsceneStates[0])[scene] == 1) && (gSeqEventQueueSize != 0)) {
        gCutsceneActorLock = 0;
        queued = 0;
        for (track = 0; track < gSeqTrackCounts[scene]; track++) {
            if (gSeqEventTracks[scene][track] != NULL) {
                extern s32 gCutscenePrevTimes[]; // matching: the block-scope redeclaration changes IDO's codegen
                if (gCutscenePrevTimes[scene] != gCutsceneTimes[scene]) {
                    ev = gSeqEventTracks[scene][track] + 4;
                    time = gSeqEventTracks[scene][track]->value;
                    while ((u32) (ev - gSeqEventTracks[scene][track]) < gSeqTrackEventCounts[scene][track]) {
                        if ((ev->delta == 0) && (ev->type == 0)) {
                            time = ev->value;
                        } else {
                            if ((time <= gCutsceneTimes[scene]) && (time > gCutscenePrevTimes[scene])) {
                                for (;;) {
                                    if ((u32) (ev - gSeqEventTracks[scene][track]) >= gSeqTrackEventCounts[scene][track]) {
                                        break;
                                    }
                                    if (((mode == 0) && (ev->type != 0x11)) || ((mode == 1) && (ev->type == 0x11))) {
                                        cutscene_queueEvent(gSeqEventQueue, &queued, ev, track, (u32) (ev - gSeqEventTracks[scene][track]), time, scene);
                                    }
                                    if ((ev->delta != 0) || ((ev->delta == 0) && (ev->type == 0))) {
                                        break;
                                    } else {
                                        ev++;
                                        continue; // matching: IDO's loop layout needs the explicit continue
                                    }
                                }
                            }
                            if (time > gCutsceneTimes[scene]) {
                                break;
                            }
                            while (ev->delta == 0) {
                                if (ev->type == 0) {
                                    time = ev->value;
                                    break;
                                }
                                ev++;
                                if ((u32) (ev - gSeqEventTracks[scene][track]) >= gSeqTrackEventCounts[scene][track]) {
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
        cutscene_flushEventQueue(queued, scene);
        gCutsceneActorLock = 1;
        if (mode == 0) {
            gCutsceneEventsDispatched[scene] = 1;
        }
    }
}

/* Field access by offset, for WorldObject and ActorCtl fields their shared declarations still leave as padding. */
#define FU8(p, o) (*(u8 *) ((u8 *) (p) + (o)))
#define FS16(p, o) (*(s16 *) ((u8 *) (p) + (o)))
#define FS32(p, o) (*(s32 *) ((u8 *) (p) + (o)))
#define FF32(p, o) (*(f32 *) ((u8 *) (p) + (o)))
#define FPTR(p, o) (*(void **) ((u8 *) (p) + (o)))



/*
 * First cutscene event handler: subtitles (types 7, 0xE-0x10, 0x12; shown only
 * when gSettings[20] is set and gTextLanguage selects that type), event types 1, 3, 4,
 * 9, 0xC and 0xD, and most type-5 commands. Returns 1 when the event was handled.
 */
s32 cutscene_handleEvent1(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene, Actor *obj, WorldObject *actor, f32 *pos, s32 sceneTag) {
    if (gSettings[20] != 0 && (event->type == 7 || event->type == 0xE || event->type == 0xF || event->type == 0x10 || event->type == 0x12)) {
        s32 opening;
        SeqSubtitle *sub;
        u8 *text;
        u8 colors[3][3] = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xA0 }, { 0x3C, 0x3C, 0x3C } };

        if ((event->type == 7 && gTextLanguage != 0) || (event->type == 0xE && gTextLanguage != 4) ||
            (event->type == 0xF && gTextLanguage != 2) || (event->type == 0x10 && gTextLanguage != 3) ||
            (event->type == 0x12 && gTextLanguage != 1)) {
            return 1;
        }
        opening = event->unk4 == 0 ? 1 : 0;
        sub = cutscene_getSubtitleSlot(track, opening);
        if (sub != NULL) {
            if (event->unk4 == 0) {
                if (sub->config.numPages != 0) {
                    text_removeBox(sub->handle);
                }
                text = gSeqTextLists[scene][event->value];
                if (*text == 0xBE) {
                    sub->config.flags = 0x8013;
                    text++;
                } else if (*text == 0xBF) {
                    sub->config.flags = 0x8019;
                    text++;
                } else {
                    sub->config.flags = 0x8011;
                }
                sub->config.text = (s32)text;
                sub->config.duration = 100;
                sub->config.numPages = 1;
                sub->config.unk24 = 0;
                sub->config.screenXPercent = &gSeqTrackStates[scene][track].pitch;
                sub->config.screenYPercent = &gSeqTrackStates[scene][track].yaw;
                sub->config.cutsceneOnly = 1;
                sub->config.anglePercent = &gSeqTrackStates[scene][track].roll;
                sub->config.r = colors[event->unk5][0];
                sub->config.g = colors[event->unk5][1];
                sub->config.b = colors[event->unk5][2];
                if (event->unk5 == 2) {
                    sub->config.flags |= 0x40000;
                }
                sub->handle = text_createBox(0, 0, 0, 0, &sub->config, sceneTag);
            } else if (event->unk4 == 1) {
                cutscene_releaseSubtitle(sub, event->unk5);
            } else {
                if (event->unk5 == 0) {
                    sub->handle = text_closeBoxAfterPage(sub->handle);
                } else {
                    sub->handle = text_closeBoxNow(sub->handle);
                }
                if (event->unk6 != 0) {
                    s32 color = event->unk6 - 1;

                    sub->config.r = colors[color][0];
                    sub->config.g = colors[color][1];
                    sub->config.b = colors[color][2];
                    if (color == 2) {
                        sub->config.flags |= 0x40000;
                    } else {
                        sub->config.flags &= ~0x40000;
                    }
                }
            }
        }
    } else if (event->type == 0xC) {
        SeqEvent *found;
        s32 duration;

        if (event->value != 0) {
            if (event->unk6 != 0) {
                face_setDefaultExpression(obj, event->value);
            } else {
                if (event->unk5 != 0) {
                    duration = 0xFFFF;
                } else {
                    duration = cutscene_findEvent(event, event->type, track, &found, 0, 0, 0, 0, 0, 0, scene) - eventTime;
                }
                face_requestExpression(obj, event->value, 3, duration, event->unk4);
            }
        }
    } else if (event->type == 0xD) {
        ModelAttachment *att;

        if (obj == NULL) {
            return 1;
        }
        if (event->unk4 == 0) {
            collision_createPartSet(obj, event->value + 1, 1.0f, 0);
        } else if (event->unk4 == 1) {
            collision_removePartSet(obj, event->value + 1);
        } else {
            att = attach_find(obj, event->value + 1, 0);
            if (att != NULL) {
                if (event->unk4 == 2) {
                    att->seg6Id = D_80090214[event->unk5];
                } else if (event->unk4 == 3) {
                    att->seg7Id = D_80090214[event->unk5];
                }
            }
        }
    } else if (event->type == 4) {
        SeqEvent *found;
        s32 duration;

        if (obj == NULL) {
            return 1;
        }
        duration = (cutscene_findEvent(event, event->type, track, &found, 0, 0, 0, 0, 0, 0, scene) - eventTime) / 2;
        if (duration >= 0x100) {
            duration = 0xFF;
        } else if (duration == 0) {
            duration = 1;
        }
        if (event->unk4 == 0) {
            obj->morphTarget = event->value;
            obj->morphDuration = duration;
        }
    } else if (event->type == 3) {
        if (event->value >= gSeqTrackCounts[scene]) {
            return 1;
        }
        if (gSeqTrackInfos[scene][event->value].type == 1 && gSeqCameraTrack != event->value) {
            gSeqCameraTrack = event->value;
            gCameraSnapTimers[0] = 4;
            camera_global_Cameras->SpecialFlags |= 4;
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
        obj->forced_lod = v;
    } else if (event->type == 5) {
        if (event->value == 0x3B) {
            if (event->unk4 == 0) {
                gLetterbox = &gSeqTrackStates[scene][track].unk28;
            } else {
                gLetterbox = NULL;
            }
        } else if (event->value == 8) {
            gCutsceneShowUnboundActors[scene] = !event->unk4;
        } else if (event->value == 9) {
            gCutsceneShowUnboundObjects[scene] = !event->unk4;
        } else if (event->value == 0xD) {
            gCutsceneUpdateUnboundActors[scene] = !event->unk4;
        } else if (event->value == 0x12) {
            gCutsceneKeepSpawnedActors[scene] = !event->unk4;
        } else if (event->value == 0x13) {
            D_800C365C[scene] = event->unk4;
        } else if (event->value == 0x15) {
            gCutsceneRunTimedEvents[scene] = !event->unk4;
        } else if (event->value == 0x14) {
            if (obj == NULL) {
                return 1;
            }
            if (event->unk4 == 0) {
                actor_handleDeath(obj);
            } else {
                actor_remove(obj, 0);
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
                obj->fogColor = (u32)&gSeqTrackStates[scene][track].unk30;
            } else {
                obj->fogMode = 0;
                obj->fogColor = 0;
            }
        } else if (event->value == 0x44) {
            s32 mode;
            s32 spawnPoint;

            if (event->unk5 != 0) {
                spawnPoint = event->unk6;
                mode = 2;
            } else {
                spawnPoint = 0;
                mode = 1;
            }
            game_requestLevelChange(mode, event->unk4, spawnPoint, 0, 0);
        } else if (event->value == 0x17) {
            if (obj == NULL) {
                return 1;
            }
            obj->levelMode = !event->unk4;
            if (obj->levelMode != 0) {
                obj->levelMode += event->unk5;
            }
            if (event->unk4 == 0) {
                obj->levelRange = event->unk6;
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
                obj->level2Range = event->unk6;
            }
        } else if (event->value == 0x18) {
        } else if (event->value == 0x1D) {
            if (gSeqTrackInfos[scene][track].type == 3 && actor != NULL) {
                if (event->unk4 == 0) {
                    FU8(actor, 0x6E) = 1;
                } else {
                    FU8(actor, 0x6E) = 0;
                }
            }
        } else if (event->value == 0x1E) {
            gNextCutscene = event->unk4 + 1;
            gNextCutsceneMap = level;
        } else if (event->value == 3) {
            WorldObject *target = gfx_findObjectById(event->unk6);

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
            Vec3f point;
            Vec3f point2;

            switch (event->unk4) {
                case 0:
                    point.x = gSeqTrackStates[scene][track].x;
                    point.y = gSeqTrackStates[scene][track].y;
                    point.z = gSeqTrackStates[scene][track].z;
                    func_151BE850(&point, gSeqTrackStates[scene][track].yaw, 1, 0, 2, 0xFF, 1);
                    break;
                case 1:
                    point2.x = gSeqTrackStates[scene][track].x;
                    point2.y = gSeqTrackStates[scene][track].y;
                    point2.z = gSeqTrackStates[scene][track].z;
                    beam_spawnEmitterPreset(5, 1, 0, &point2, -10000.0f, 100.0f, -1, sceneTag, 1);
                    break;
                case 2:
                    effect_sendEventToTimedEffects(0, event->unk5 ? 0x2B : 0x2C);
                    break;
                case 3:
                    if (event->unk5) {
                        light_setGroupDisabled(1, 0);
                        light_setGroupDisabled(2, 1);
                    } else {
                        light_setGroupDisabled(1, 1);
                        light_setGroupDisabled(2, 0);
                    }
                    break;
                case 4:
                    light_setDaylight(event->unk5);
                    break;
                case 5:
                    light_sendEvent(0x28, 0x17, 0);
                    break;
                case 6:
                    trail_startStream(event->unk5);
                    break;
                case 7:
                    light_setGroupDisabled(3, event->unk5 ? 0 : 1);
                    break;
                case 8:
                    effect_spawnScatterTrailVariant(event->unk5);
                    break;
                case 9: {
                    u8 i;
                    const s32 ids[4] = { 0x12, 0x13, 0x14, 0x15 };

                    for (i = 0; i < 4; i++) {
                        sight_sendBeamCommand(2, &zones[ids[i]]);
                    }
                    break;
                }
                case 10:
                    sight_spawnScopeSight(camera_global_Cameras, 0, track, sceneTag, 1);
                    break;
                case 11:
                    sight_resetPlayerEffects(camera_global_Cameras);
                    break;
                case 12:
                    effect_setupWarpModelOnce();
                    break;
                case 13:
                    effect_turnLightsOn();
                    break;
                case 14:
                    effect_turnBeamsOff();
                    break;
                case 15:
                    effect_startLineEmitter(0xFF, 1);
                    break;
                case 16:
                    effect_broadcastEvent(0, 0x4E);
                    break;
                case 17:
                    effect_broadcastEvent(0, 0x4F);
                    break;
                case 18:
                    trail_stopStream(event->unk5);
                    break;
                case 19:
                    light_setGroupDisabled(0xB, 0);
                    break;
                case 20:
                    if (obj != NULL) {
                        particle_sendActorGoneToEmitters(obj);
                        particle_broadcastToActorEmitters(0, 0x49);
                    }
                    break;
                case 21:
                    effect_sendEventToTimedEffects(0, event->unk5 != 0 ? 0x55 : 0x56);
                    break;
                case 22:
                    if (obj != NULL) {
                        particle_attachBloodStrip(obj, 0xFF, 1);
                    }
                    break;
                case 23:
                    if (obj != NULL) {
                        ActorRef ref;

                        ref.actor = obj;
                        ref.uniqueId = obj->unique_id;
                        particle_sendDripMsg(&ref, 0x50);
                    }
                    break;
            }
        } else if (event->value == 0x6E) {
            const u8 *list;
            s32 count;

            list = particle_getObjEmitterData(obj, &count);
            if (list != NULL && event->unk6 < count) {
                particle_startAnchorEmitter(obj, list[event->unk6], -1, -1, -1, -1, 0, 0, 0xFF, 1);
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
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 1, event->unk5, scene) - eventTime;
                if (event->unk6 != 0) {
                    duration = 999;
                    c = 0xFF;
                } else {
                    c = sceneTag;
                }
                particle_startActorEmitter(obj, ids[idx], duration, 0, c, 0, "Cutscene 1");
            }
        } else if (event->value == 0x36) {
            if (event->unk4 == 0) {
                trail_startMatrixTrail(obj, 0, 300, 0, sceneTag, 0);
            } else if (event->unk4 == 1) {
                ActorRef ref;

                ref.actor = obj;
                ref.uniqueId = obj->unique_id;
                effect_sendEventToEffectObjs((s32)&ref, 7);
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
                effect_sendAttachmentMessage(obj, 0xD, v, 0);
            }
        } else if (event->value == 0x46) {
            if (event->unk4 != 0) {
                beam_sendStripMessage1(obj);
            } else {
                beam_sendStripMessage2(obj);
            }
        } else if (event->value == 0x25) {
            if (event->unk4 != 0) {
                gNearPlaneOverride = event->unk4;
            } else {
                gNearPlaneOverride = gDefaultNear;
            }
            view_setClipPlanes(0, gNearPlaneOverride, gFarPlaneOverride);
        } else if (event->value == 0x58) {
            if (event->unk6 != 0) {
                gFarPlaneOverride = event->unk6 * 10;
            } else {
                gFarPlaneOverride = gDefaultFar;
            }
            view_setClipPlanes(0, gNearPlaneOverride, gFarPlaneOverride);
        } else if (event->value == 0x3C) {
            s32 v;

            if (obj != NULL) {
                v = event->unk4;
                v <<= 16;
                v |= event->unk6;
                if (event->unk5 == 0) {
                    obj->hiddenParts = v;
                } else if (event->unk5 == 1) {
                    obj->hiddenParts |= v;
                } else {
                    obj->hiddenParts &= ~(v | 1);
                }
            }
        } else if (event->value == 0x3D) {
            s32 r;

            if (obj != NULL) {
                if (event->unk4 == 0) {
                    r = collision_findObjectSlotById(event->unk6);
                    if (r != -1) {
                        obj->unk101 |= 4;
                        obj->attach_char = r + 1;
                        obj->basemtx = event->unk5;
                    }
                } else {
                    obj->attach_char = 0;
                }
            }
        } else if (event->value == 0x42) {
            if (event->unk4 == 0) {
                gSpriteQueuedCmd = 1;
            } else if (event->unk4 == 1) {
                gSpriteQueuedCmd = 4;
            } else if (event->unk4 == 3) {
                gSpriteQueuedCmd = 2;
            } else if (event->unk4 == 4) {
                gSpriteQueuedCmd = 5;
            } else if (event->unk4 == 5) {
                gSpriteQueuedCmd = 6;
            } else if (event->unk4 == 6) {
                gSpriteQueuedCmd = 7;
            } else if (event->unk4 == 7) {
                gSpriteQueuedCmd = 8;
            } else if (event->unk4 == 8) {
                gSpriteQueuedCmd = 9;
            } else if (event->unk4 == 9) {
                gSpriteQueuedCmd = 10;
            }
        } else if (event->value == 0x43) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                sprite_spawnConverging(gSeqTrackStates[scene][track].unk24, gSeqTrackStates[scene][track].unk28, duration, sceneTag);
            }
        } else if (event->value == 0x45) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                screen_spawnTintEffect(0, duration, 0xFF, 0, 0, gLastPlayerIndex, 0, 0);
            }
        } else if (event->value == 0x49) {
            if (obj != NULL && event->unk4 < 3) {
                (&obj->taskint[0])[event->unk4] = event->unk6;
            }
        } else if (event->value == 0x4B) {
            if (obj != NULL) {
                obj->hiddenBones = (event->unk6 & 0xFFFF) | ((event->unk5 & 0xFF) << 16) | ((event->unk4 & 0xFF) << 24);
            }
        } else if (event->value == 0x4A) {
            if (event->unk4 == 0) {
                audio_setMixOverride(event->unk5);
            } else {
                audio_setMixOverride(0);
            }
        } else if (event->value == 0x54) {
            gCutsceneUseCullDistance[scene] = ~event->unk4;
        } else if (event->value == 0x55) {
            effect_randomizeBounceRecordSpeeds(event->unk4);
        } else if (event->value == 0x59) {
            gCutsceneFadeInAtEnd = 1;
        } else if (event->value == 0x5A) {
            dc_glbl_CharNum = obj - movement_obj;
            if (obj->character_type == 0x53) {
                obj->owner = 0;
                actor_seatRider(obj, 156.0f, 1, 4, 0, 10, 0xFF, 0, 1, 0);
            } else if (obj->character_type == 0x28) {
                obj->owner = 0;
                actor_seatRider(obj, 180.0f, 5, 0x44, 1, 1, 0xFF, 0, 1, 0);
            } else if (obj->character_type == 0x21) {
                obj->owner = 0;
                actor_seatRider(obj, 156.0f, 0x19, 4, 0, 0xF, 0xFF, 0, 1, 0);
            }
        } else if (event->value == 0x71) {
            movement_obj->heightOffset = 0;
            movement_obj->mv->transport = 0;
            movement_obj->attach_char = 0;
            movement_obj->mv->jump_on_char = 0;
            movement_obj->noshove &= ~0x01000000;
            if (obj != NULL) {
                obj->lifting = 0;
            }
        } else if (event->value == 0x37) {
            s32 duration;
            SeqEvent *found;
            s32 v;
            u8 colors[3][3] = { { 0xFF, 0, 0 }, { 0, 0x35, 0x46 }, { 0xFF, 0xFF, 0xFF } };

            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                v = event->unk6 != 0 ? event->unk6 : 0x7F;
                screen_startViewFlash(colors[event->unk5][0], colors[event->unk5][1], colors[event->unk5][2], v, duration, 0);
            } else {
                gFlashFrames[0] = 0;
            }
        } else if (event->value == 0x38) {
            s32 duration;
            SeqEvent *found;
            s32 v;

            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                v = event->unk6 != 0 ? event->unk6 : 4;
                screen_startTint(event->unk5, duration, v, 0x7F, 1);
            } else {
                gTintFrames = 0;
            }
        } else if (event->value == 0x5C) {
            SeqEvent *found;
            s32 total;
            s32 start;
            s32 end;

            if (event->unk4 == 0) {
                total = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 1, 1, event->unk5, scene);
                start = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 2, 1, event->unk5, scene) - eventTime;
                if (found == NULL) {
                    start = 0;
                }
                end = total - cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 3, 1, event->unk5, scene);
                if (found == NULL) {
                    end = 0;
                }
                if (level == LEVEL_W5_THRONE_ROOM) {
                    beam_showFadeSprite(start, total - eventTime - end - start, end, event->unk5);
                } else if (level == LEVEL_WFRONTEND_TABLE) {
                    sprite_showFadeSprite(start, total - eventTime - end - start, end);
                }
            }
        } else if (event->value == 0x62) {
            gLevelHeader->skyMode = 0;
        } else if (event->value == 0x7C) {
            gCutsceneHideSky = !event->unk4;
        } else if (event->value == 0x84) {
            gCutsceneSolidSky = !event->unk4;
        } else if (event->value == 0x85) {
            gCutsceneNoPause = !event->unk4;
        } else if (event->value == 0x63) {
            gSoundTargetVolume = event->unk6;
        } else if (event->value == 0x64) {
            if (event->unk4 == 0) {
                gCutsceneIrisOn = 1;
                gCutsceneIrisClosure = &gSeqTrackStates[scene][track].unk28;
                gCutsceneIrisX = &gSeqTrackStates[scene][track].unk24;
                gCutsceneIrisY = &gSeqTrackStates[scene][track].unk2C;
            } else {
                gCutsceneIrisOn = 0;
            }
        } else if (event->value == 0x66) {
            if (event->unk4 == 0) { // matching: empty, but it still shapes the code
            }
        } else if (event->value == 0x67) {
            func_151DE7D4();
        } else if (event->value == 0x73) {
            save_endMatch(0);
        } else if (event->value == 0x69) {
            if (obj != NULL) {
                if (event->unk4 == 0) {
                    obj->ygrav = 4.0f;
                    obj->yspd = 20.0f;
                } else if (event->unk4 == 1) {
                    obj->ygrav = 0.0f;
                    obj->yspd = 0.0f;
                } else if (event->unk4 == 2) {
                    collision_removePartSet(obj, 0x11);
                    obj->unkA9 = 0;
                } else if (event->unk4 == 3) {
                    dc_glbl_CharNum = obj - movement_obj;
                    movement_obj->lifting = dc_glbl_CharNum + 100;
                    movement_obj->speed = 0.0f;
                    FS16(movement_obj->mv, 0x18) = 0;
                    actor_attachToHost(obj, 0);
                    *(u32 *) &obj->basemtx = 9; // matching: an unsigned store
                    obj->attach_char = 1;
                    obj->unk101 |= 4;
                } else if (event->unk4 == 4) {
                    FU8(movement_obj->mv, 0x17) = 1;
                } else if (event->unk4 == 5) {
                    gCurrentActor = obj;
                    func_15073F1C();
                } else if (event->unk4 == 6) {
                    obj->mv->transport = 0;
                }
            }
        } else if (event->value == 0x6A) {
            gCutsceneCutbackAngle = event->unk6;
        } else if (event->value == 0x7A) {
            gCutsceneEndTransitionTime = event->unk5 * 60;
            gCutsceneEndTransitionYaw = event->unk6;
        } else if (event->value == 0x6B) {
        } else if (event->value == 0x6D) {
            gCutsceneHideWorld = !event->unk4;
        } else if (event->value == 0x74) {
            gFrameRateDivider = event->unk5;
        } else if (event->value == 0x6F) {
            if (obj != NULL) {
                obj->swapTexture[event->unk4] = event->unk5;
            }
        } else if (event->value == 0x70) {
            gCutsceneHoldEnd = 1;
        } else if (event->value == 0x72) {
            if (obj != NULL) {
                obj->rootOffsetY = gModelDefs[obj->character_type]->unk6;
            }
        } else if (event->value == 0x78) {
            gCutsceneNoSkipFadeIn = 1;
        } else if (event->value == 0x79) {
            if (actor != NULL) {
                FS32(actor, 0x84) = 0;
                FS32(actor, 0x80) = 0;
                FS32(actor, 0x7C) = 0;
            }
        } else if (event->value == 0x7B) {
            if (level == LEVEL_FRONTEND) {
                gCutsceneKeepCamera = 1;
            }
        } else if (event->value == 0x7D) {
            if (obj != NULL && event->unk4 == 0) {
                obj->drawtype = 1;
            }
        } else if (event->value == 0x7E) {
            if (obj != NULL) {
                gExpressionTarget = obj - movement_obj;
            }
        } else if (event->value == 0x7F) {
            D_800DBEAC = 1;
        } else if (event->value == 0x80) {
            gClearToWhite = event->unk4;
        } else if (event->value == 0x81) {
            hud_setClockEnabled(event->unk4 ^ 1);
            gHudClockTime = event->unk6 * 30;
        } else if (event->value == 0x82) {
            gCutsceneChainBobScale = event->unk4;
            if (gCutsceneChainBobScale > 100) {
                gCutsceneChainBobScale = 100;
            }
        } else if (event->value == 0x83) {
            gCutsceneCashOffset = event->unk6;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

/* matching: an s8 texture, as the call passes event->unk5 without narrowing (definition: game/Particle/StyleParticle/styleparticle.c, u8) */
void particle_spawnEmitterFollowingPos(f32 *position, s32 duration, s8 texture, u8 arg3, s32 arg4);

/*
 * Second cutscene event handler: types 0xB (mp3_startTrack / func_151F2xxx) and
 * 0x11 (records the event index in gSeqActiveEvents), and the effect-spawning type-5
 * commands. Returns 1 when the event was handled.
 */
s32 cutscene_handleEvent2(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene, Actor *obj, WorldObject *actor, f32 *pos, s32 sceneTag) {
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
            mp3_startTrack(event->value, vol, 0x40);
        } else if (event->unk4 == 1) {
            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 2, 0, 0, scene) - eventTime;
            if (event->unk6 == 0) {
                vol2 = 0x10000;
            } else {
                vol2 = (event->unk6 << 16) / 255;
            }
            duration = (duration * 0x5604) / 60;
            mp3_setVolume(vol2, duration);
        } else if (event->unk4 == 3) {
            mp3_stop();
        } else if (event->unk4 == 4) {
            mp3_pause();
        } else if (event->unk4 == 5) {
            mp3_resume();
        }
    } else if (event->type == 0x11) {
        if (event->unk4 != 0) {
            gSeqActiveEvents[scene][track][event->value] = index;
        } else {
            gSeqActiveEvents[scene][track][event->value] = 0xFF;
        }
    } else if (event->type == 5) {
        if (event->value == 0x11) {
            s32 duration;
            SeqEvent *found;
            s32 param;
            s32 idx;
            s32 r;
            s32 g;
            s32 b;
            s32 alpha;
            s32 envR;
            s32 envG;
            s32 t;
            s32 radius;
            u8 colors[3][6] = { { 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF, 0x00, 0xAB, 0xFF }, { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };

            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
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
                r = colors[idx][0];
                g = colors[idx][1];
                b = colors[idx][2];
                alpha = colors[idx][3];
                envR = colors[idx][4];
                envG = colors[idx][5];
                t = param >> 3;
                radius = ((param * 40) / 4096) & 0xFF;
                particle_newSpriteEmitter((s32)pos >> 16, (s32)pos & 0xFFFF, 0, 0x1B, 0,
                              r, g, b, alpha, envR, envG,
                              0, 1, 6, 0, 0, 0, 0, 0, t >> 8, t & 0xFF, 0, 0, 0xA, 0, 0, 0, 0, 0, 0, 0,
                              duration, radius, radius, 0xFF, 0xE, 8, 8, 0x25, 0, 0xFF, 8, 0, 0,
                              scene, 0, 0, sceneTag, 0);
            }
        } else if (event->value == 0xA) {
            s32 duration;
            SeqEvent *found;

            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                particle_newFallingEmitter(3, 3, 0, 0, 0, 0x320, 0x4B0, 0x190, 0xC8, 0x66, 0x1C7, 2, duration, pos, sceneTag, 0);
            }
        } else if (event->value == 0xB) {
            s32 duration;
            SeqEvent *found;

            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                particle_newFallingEmitter(0x2F, 1, 0, 0, 0, 0x7D0, 0x3E8, 0x190, 0xC8, 0x400, 0x400, 6, duration, pos, sceneTag, 0);
            }
        } else if (event->value == 0xC) {
            SpriteFxTemplate sprite;

            sprite.anim = gEffectAnimTextures[1];
            sprite.frame = 0;
            sprite.frameStep = 0x100;
            sprite.x = pos[0];
            sprite.y = pos[1] + event->unk4;
            sprite.z = pos[2];
            sprite.scaleX = 0x5DC;
            sprite.scaleY = 0x5DC;
            sprite.fadeStep = -10;
            sprite.callback = 0;
            effect_spawnSpriteFx(&sprite, sceneTag, 1);
        } else if (event->value == 0x19) {
            s32 duration;
            SeqEvent *found;
            s32 count;

            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                count = event->unk5;
                if (count == 0) {
                    count = 0xC;
                }
                swarm_createSwarm(count, pos, 0xFF, pos[0], pos[1], pos[2], 0, duration, sceneTag, 0, event->unk6);
            }
        } else if (event->value == 0x1A) {
            if (event->unk4 != 0 && event->unk4 == 1) { // matching: redundant test kept for codegen
                Vec3f from;
                Vec3f to;

                from.x = gSeqPathKeys[scene][track]->x;
                from.y = gSeqPathKeys[scene][track]->y;
                from.z = gSeqPathKeys[scene][track]->z;
                to.x = gSeqTrackStates[scene][track].x;
                to.y = gSeqTrackStates[scene][track].y + gSeqTrackStates[scene][track].yOffset;
                to.z = gSeqTrackStates[scene][track].z;
                particle_fireTrailProjectile(&from, &to, sceneTag, 1);
            } else {
                beam_spawnTracer(0, gSeqPathKeys[scene][track]->x, gSeqPathKeys[scene][track]->y, gSeqPathKeys[scene][track]->z,
                              gSeqTrackStates[scene][track].x, gSeqTrackStates[scene][track].y + gSeqTrackStates[scene][track].yOffset,
                              gSeqTrackStates[scene][track].z, 40.0f, 0.0f, 8.0f, 60.0f, 0x258, 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
            beam_spawnBoneTracer(obj, obj->unique_id, anim, -1, 0.0f, 0.0f, 156.0f, 0.0f, 0.0f, 636.0f, 3, 0xA, 0xF0, 0);
        } else if (event->value == 0x1B) {
            s32 duration;
            SeqEvent *found;

            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                switch (event->unk6) {
                    case 0:
                        particle_spawnEmitterFollowingPos(pos, duration, event->unk5, sceneTag, 0);
                        break;
                    case 1:
                        prop_startMirroredEmitters(duration);
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

                        x = obj->xpos;
                        y = obj->ypos;
                        z = obj->zpos;
                        angle = obj->langle >> 8;
                        s = math_sinBinAngle(angle);
                        c = math_cosBinAngle(angle);
                        func_1516F548(x + 20.0f, y + 475.0f, z, duration, 14.0f * s, 0xB00, 14.0f * c, 0xC8,
                                      0x32, 0xA, 6, 7, 0xBE, 0xBE, 0xBE, sceneTag, 0);
                        func_1516F548(x - 24.0f, y + 476.0f, z, duration, -23.0f * s, 0x800, -23.0f * c, 0xC8,
                                      0x32, 0xA, 6, 7, 0xBE, 0xBE, 0xBE, sceneTag, 0);
                        break;
                    }
                    case 3:
                        particle_startColorEmitter(pos, (s16)duration, 1, 1, sceneTag, 0);
                        break;
                    case 4:
                        particle_startColorEmitter(pos, (s16)duration, 0, 0, sceneTag, 0);
                        break;
                }
            }
        } else if (event->value == 0x1F) {
            if (obj == NULL) {
                return 1;
            }
            particle_attachOwnerEmitter(obj, sceneTag, 1);
        } else if (event->value == 0) {
            if (obj == NULL) {
                return 1;
            }
            trail_spawnActorBoneTrail(obj, sceneTag);
        } else if (event->value == 0x20) {
            effect_spawnQuadEffect(-1, pos[0], pos[1], pos[2], 0x14, 0x14, 1, sceneTag, 1);
        } else if (event->value == 6) {
        } else if (event->value == 7) {
            particle_spawnObjGroundBurst(obj, 9, -1);
        } else if (event->value == 0x21) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                blood_startShortTrail(pos, pos + 1, pos + 2, duration, sceneTag, 0);
            }
        } else if (event->value == 0x60) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                screen_spawnMosaic(obj, duration, sceneTag, 0);
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
                        effect_sendEventToTimedEffects((s32)val, 0x51);
                        break;
                    case 0x13:
                        effect_sendHopMessage((event->unk6 * 0.009999999776f * 300.0f) - 150.0f);
                        break;
                }
            }
            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 1, 1, event->unk5, scene) - eventTime;
                switch (event->unk5) {
                    case 1:
                        particle_startSegmentEmitter(obj, duration, -1, 0, 0.000709999993f, 0, sceneTag, 1);
                        break;
                    case 2:
                        particle_startSegmentEmitter(obj, duration, -1, 1, 0.00612499984f, 0, sceneTag, 1);
                        break;
                    case 3:
                        particle_startMatrixSegmentEmitter(actor, duration, -1, 0, 0.00612499984f, 0, sceneTag, 1);
                        break;
                    case 4:
                        particle_startMatrixSegmentEmitter(actor, duration, -1, 1, 0.00612499984f, 0, sceneTag, 1);
                        break;
                    case 5:
                        particle_startPointPairEmitter(obj, duration, sceneTag, 1);
                        break;
                    case 6:
                        light_createSeqTrack(track, duration, sceneTag, 1);
                        break;
                    case 7:
                        light_setLampTrack(track, duration, sceneTag);
                        break;
                    case 8: {
                        Vec3f p;

                        p.x = gSeqTrackStates[scene][track].x;
                        p.y = gSeqTrackStates[scene][track].y;
                        p.z = gSeqTrackStates[scene][track].z;
                        beam_spawnEmitterPreset(5, 1, 0, &p, -10000.0f, 100.0f, duration, sceneTag, 1);
                        break;
                    }
                    case 9: {
                        ActorLink link;

                        link.actor = obj;
                        link.actorId = obj->unique_id;
                        link.timer = duration;
                        link.flags = 1;
                        link.unk9 = 0;
                        switch (obj->character_type) {
                            case 0x57:
                            case 0x71:
                                link.kind = 1;
                                break;
                            case 0x96:
                                link.kind = 3;
                                break;
                            default:
                                link.kind = 0;
                                break;
                        }
                        particle_newDripEmitter(&link, 0, sceneTag, 1);
                        break;
                    }
                    case 10:
                        effect_spawnPuffFollower(&gSeqTrackStates[scene][track].x, duration, sceneTag, 1);
                        break;
                    case 11: {
                        s16 data[2];
                        void *fx;

                        particle_startGreyscaleScreen(duration, sceneTag, 1);
                        data[0] = 0;
                        data[1] = 0;
                        fx = effect_newTimedEffect(duration, -1, 0x4B, -1, 1, 0, 4, sceneTag, 1);
                        if (fx != NULL) {
                            memcpy((u8 *)fx + 0x28, data, 4);
                        }
                        break;
                    }
                    case 12:
                        beam_spawnBoneSparks(obj, duration, sceneTag, 1);
                        break;
                    case 13: {
                        u16 id;

                        id = event->unk6;
                        if (id >= 0xE9) {
                            id = 0;
                        }
                        cutscene_spawnTrackEffect(&gSeqTrackStates[scene][track], id, duration, sceneTag, 1);
                        break;
                    }
                    case 15:
                        if (obj != NULL && duration > 0) {
                            f32 half;

                            obj->taskint[2] = 1;
                            half = duration * 0.5f;
                            obj->unk2D8[0] = half;
                            obj->unk2D8[1] = 1.0f / half;
                            effect_spawnPulseEffect(obj, duration, sceneTag, 1);
                        }
                        break;
                    case 16:
                        if (obj != NULL && duration > 0) {
                            beam_spawnBoneBloodStrip(obj, duration, sceneTag, 1);
                        }
                        break;
                    case 17:
                        effect_newFadeTextBox(event->unk6, gSeqTextLists[scene][event->unk6], duration, sceneTag, 1);
                        break;
                    case 18:
                        effect_newPromptTextBox(event->unk6, gSeqTextLists[scene][event->unk6], duration, sceneTag, 1);
                        break;
                    case 19:
                        effect_spawnHopSprite(duration, sceneTag, 1);
                        break;
                    case 20:
                        particle_startObjDustEmitter(obj, duration, sceneTag, 1);
                        break;
                    case 21:
                        effect_newSlowFadeTextBox(event->unk6, gSeqTextLists[scene][event->unk6], duration, sceneTag, 1);
                        break;
                    case 22: {
                        u16 id;

                        id = event->unk6;
                        if (id >= 0xCF) {
                            id = 0;
                        }
                        cutscene_spawnTrackParticle(&gSeqTrackStates[scene][track], id, duration, sceneTag, 1);
                        break;
                    }
                    case 23:
                        effect_spawnScreenCash(duration, sceneTag, 1);
                        break;
                    case 24: {
                        Vec3f point;
                        Vec3f probe;
                        CollisionHit floor;
                        s32 hit;

                        point.x = gSeqTrackStates[scene][track].x;
                        point.y = gSeqTrackStates[scene][track].y + 100.0f;
                        point.z = gSeqTrackStates[scene][track].z;
                        probe.x = point.x;
                        probe.y = point.y + 100.0f;
                        probe.z = point.z;
                        if (collision_probeFloorSurfABSet0Cached(&probe, 0, point.y - 1000.0f, &floor) != 0) {
                            if (floor.source != 3) {
                                hit = 1;
                                point.y = floor.height;
                            } else {
                                hit = 0;
                            }
                        } else {
                            hit = 0;
                        }
                        if (hit) {
                            blood_spawnGrowingParticle((math_randomFloat() * 0.02000000142f) + 0.09000000358f,
                                          (math_randomFloat() * 0.0004399999743f) + 0.0008799999487f,
                                          (math_random() % 101) + 0x9B, 0xFF, duration, &floor.tri, &point,
                                          !event->unk6, 1, sceneTag, 1);
                        }
                        break;
                    }
                    case 25: {
                        f32 from[3];
                        s32 to[3];

                        from[0] = gSeqPathKeys[scene][track]->x;
                        from[1] = gSeqPathKeys[scene][track]->y;
                        from[2] = gSeqPathKeys[scene][track]->z;
                        to[0] = gSeqTrackStates[scene][track].x;
                        to[1] = gSeqTrackStates[scene][track].y;
                        to[2] = gSeqTrackStates[scene][track].z;
                        effect_startFollowingHoming(from, to, &gSeqTrackStates[scene][track].x, duration);
                        break;
                    }
                    case 26: {
                        RumbleParams rumble;

                        rumble.flags = 1;
                        rumble.duration = duration;
                        rumble.strength = 8;
                        rumble.playerMask = 1;
                        rumble.updateKind = -1;
                        cont_spawnRumble(&rumble, 0, sceneTag, 1);
                        break;
                    }
                }
            }
        } else if (event->value == 0x5B) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                beam_spawnWavyBeam((event->unk4 != 0) ? 5000 : duration, sceneTag, 0);
            }
        } else if (event->value == 0x5D) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                particle_attachBoneSideEmitter(obj, duration, sceneTag, 0);
            }
        } else if (event->value == 0x5E) {
            s32 duration;
            SeqEvent *found;

            if (event->unk5 == 1) {
                effect_startJointEmitterTask(obj, 0, 1000, sceneTag, 0);
            } else if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                effect_startJointEmitterTask(obj, 0, duration, sceneTag, 0);
            }
        } else if (event->value == 0x4C) {
            s32 duration;
            SeqEvent *found;
            const u8 *list;
            s32 count;

            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                if (obj->character_type == 0x49 || obj->character_type == 0x58) {
                    list = particle_getObjEmitterData(obj, &count);
                    if (list != NULL && event->unk6 < count) {
                        particle_startAnchorEmitter(obj, list[event->unk6], duration, -1, -1, -1, 0, 0, sceneTag, 1);
                    }
                } else {
                    particle_startActorEmitter(obj, event->unk5, duration, 0, sceneTag, 1, "Cutscene 2");
                }
            }
        } else if (event->value == 0x53) {
            if (event->unk4 == 0) {
                s32 endTime;
                SeqEvent *found;

                endTime = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene);
                {
                    f32 *p = pos; // matching: IDO needs the pointer copied to a local here
                    BloodTrailDesc18 trail;

                    trail.y = p + 1;
                    trail.z = p + 2;
                    trail.x = p;
                    trail.radius = 2.0f;
                    trail.rate = 1.0f;
                    trail.lifetime = endTime - eventTime;
                    trail.flags = 1;
                    trail.emitter = 2;
                    blood_spawnTrail(&trail, 0, sceneTag, 0);
                }
            }
        } else if (event->value == 0x50) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                effect_attachBone8SegmentStrip(obj, duration, sceneTag, 0);
            }
        } else if (event->value == 0x51) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                effect_attachDriftEmitter(obj, duration, sceneTag, 0);
            }
        } else if (event->value == 0x48) {
            Vec2f offset;
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 1, event->unk5, scene) - eventTime;
                offset.x = (math_randomFloat() * 200.0f) + -100.0f;
                offset.y = (math_randomFloat() * 300.0f) + -150.0f;
                blood_spawnSplat(&offset, (math_randomFloat() * 20.0f) + 10.0f,
                              ((math_randomFloat() * 65.0f) + 35.0f) * 0.001000000047f,
                              ((math_randomFloat() * 160.0f) + 200.0f) * 0.001000000047f,
                              (math_random() % 101) + 100, 0xFF, (math_random() % 3) + 4, duration, 4,
                              0x40, sceneTag, 1);
            }
        } else if (event->value == 0xF) {
            s32 duration;
            SeqEvent *found;

            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                blood_attachMirroredStrips(obj, duration, sceneTag, 1);
            }
        } else if (event->value == 0x3E || event->value == 0x3F) {
        } else if (event->value == 0x32) {
            s32 duration;
            SeqEvent *found;

            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                beam_spawnLinkedQuad(pos, &gSeqTrackStates[scene][track].yaw, &gSeqTrackStates[scene][track].unk18,
                              &gSeqTrackStates[scene][track].unk1C, &gSeqTrackStates[scene][track].unk20, duration,
                              sceneTag, 0);
            }
        } else if (event->value == 0x34) {
            s32 duration;
            SeqEvent *found;
            u8 t;
            u8 kind;
            Attachment *node;

            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                t = (event->unk5 <= 0) ? event->unk5 : 0;
                if (t == 0) {
                    kind = 0xD;
                }
                node = obj->attachments;
                while (node != NULL) {
                    if (node->type == kind || effect_isAttachTypeInTree(gAttachmentTypes[kind]->firstChild, node->type) != 0) {
                        ((EvTimerOwner *)node->handle)->timer =
                            cutscene_startTimer(duration, node->handle, 0, &gSeqTrackStates[scene][track].unk30,
                                          &gSeqTrackStates[scene][track].unk20, sceneTag, 0);
                    }
                    node = node->next;
                }
            }
        } else if (event->value == 0x23) {
            s32 duration;
            SeqEvent *found;

            if (obj == NULL) {
                return 1;
            }
            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                func_1516F2F8(obj - movement_obj, duration, duration, 0, sceneTag, 0);
            }
        } else if (event->value == 0x22) {
            s32 duration;
            SeqEvent *found;

            if (obj == NULL) {
                return 1;
            }
            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (event->unk4 == 0) {
                func_1516F1C0(obj - movement_obj, duration, sceneTag, 0);
            }
        } else if (event->value == 0x52) {
            if (obj == NULL) {
                return 1;
            }
            blood_scatterDecals(obj, 90.0f, sceneTag, 1);
        } else if (event->value == 0x5F) {
            switch (event->unk4) {
                case 0:
                    if (obj != NULL) {
                        Vec3f pos;
                        CollisionHit floor;
                        Vec3f out;

                        collision_initHitFromObject(&floor, obj);
                        pos.x = obj->xpos;
                        pos.y = obj->ypos + 100.0f;
                        pos.z = obj->zpos;
                        if (collision_probeFloorSurfABSet0Cached(&pos, 0, obj->ypos - 300.0f, &floor) != 0) {
                            out.x = pos.x;
                            out.y = floor.height;
                            out.z = pos.z;
                            effect_emitGroundCircle(&out);
                        }
                    }
                    break;
                case 1:
                    light_toggleLamp();
                    break;
                case 2: {
                    ModelAttachment *attachment;
                    Vec3f vec;

                    if (obj != NULL) {
                        switch (obj->character_type) {
                            case 0x5A:
                            case 0x74:
                            case 0x75:
                            case 0x7A:
                                attachment = attach_find(obj, 0x46, 0);
                                if (attachment != NULL) {
                                    const Vec3f *p1;
                                    Vec3f *p2;

                                    p1 = &gSegmentShotStart;
                                    p2 = &vec;
                                    if (math_transformAttachmentOffsets(obj, attachment, 0, &p1, &p2, 1) != 0) {
                                        weapon_spawnSegmentBeam(obj, &vec, 0x46, sceneTag, 1);
                                        weapon_spawnLightAt(&vec, sceneTag, 1);
                                    }
                                }
                                break;
                            case 0x86:
                                func_150FB324(obj, sceneTag, 1);
                                break;
                            case 0x4A:
                                effect_spawnSlotBoneBeam(obj, event->unk5, sceneTag, 1);
                                break;
                            case 0x97:
                                effect_spawnCutsceneBoneBeam(obj, sceneTag, 1);
                                break;
                            case 0x98:
                                effect_spawnGunMuzzleEffects(obj, sceneTag, 1);
                                break;
                        }
                    }
                    break;
                }
                case 3: {
                    Vec3f p;

                    p.x = gSeqTrackStates[scene][track].x;
                    p.y = gSeqTrackStates[scene][track].y;
                    p.z = gSeqTrackStates[scene][track].z;
                    func_151BE850(&p, gSeqTrackStates[scene][track].yaw, 1, 0, 2, sceneTag, 1);
                    break;
                }
                case 4:
                    trail_breakApart(obj, sceneTag, 1);
                    break;
                case 5:
                    if (obj != NULL) {
                        ModelAttachment *attachment;

                        attachment = NULL;
                        if (event->unk6 != 0) {
                            attachment = attach_find(obj, event->unk5 ? 0xA4 : 0xA5, 0);
                        }
                        weapon_fireJointShot(obj, event->unk5, 1, attachment, event->unk5 ? 0xA4 : 0xA5, sceneTag, 1);
                    }
                    break;
                case 6:
                    particle_spawnObjEffects(obj, 9, -1);
                    break;
                case 7:
                    func_150E0870(obj, sceneTag, 1);
                    break;
                case 8: {
                    Vec3f p;

                    p.x = pos[0];
                    p.y = pos[1];
                    p.z = pos[2];
                    particle_spawnBlast(&p, sceneTag, 1);
                    break;
                }
                case 9:
                    blood_spawnHitEffects(obj, 1, 1, 1, 1, 0, sceneTag, 1);
                    break;
                case 10: {
                    if (obj->ob_matrix_ptr != NULL) {
                        const Vec3f *sel;
                        const Vec3f a[3] = { { 73.0f, 14.0f, 0.0f }, { 68.0f, -10.0f, 8.0f }, { 62.0f, 1.0f, -5.0f } };
                        const Vec3f b[4] = { { -55.0f, -6.0f, -1.0f }, { -47.0f, -1.0f, -10.0f }, { -53.0f, -14.0f, 6.0f }, { -57.0f, 10.0f, 6.0f } };
                        const Vec3f c[6] = { { -1.0f, 123.0f, 10.0f }, { -10.0f, 116.0f, -1.0f }, { -36.0f, 88.0f, 6.0f }, { 4.0f, 112.0f, 6.0f }, { -6.0f, 112.0f, -18.0f }, { -14.0f, 101.0f, 34.0f } };
                        const s32 bones[3] = { 0x15, 0x20, 3 };
                        Vec3f out;

                        switch (event->unk5) {
                            case 0:
                                sel = &a[math_random() % 3];
                                break;
                            case 1:
                                sel = &b[math_random() & 3];
                                break;
                            case 2:
                                sel = &c[math_random() % 6];
                                break;
                        }
                        math_transformOffset(sel, &out, &obj->ob_matrix_ptr[bones[event->unk5]]);
                        weapon_spawnImpactBurst(&out, sceneTag, 1);
                    }
                    break;
                }
                case 11: {
                    u8 i;
                    const s32 ids[4] = { 6, 7, 8, 9 };

                    for (i = 0; i < 4; i++) {
                        sight_sendBeamCommand(1, &zones[ids[i]]);
                    }
                    break;
                }
                case 12:
                    beam_spawnBoundsImpact(obj, math_random() & 0xFF, (math_random() & 0x7F) - 0x40, sceneTag, 1);
                    break;
                case 13:
                    sight_flashSprites(camera_global_Cameras);
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
                    cutscene_spawnDebrisBurst((Vec3f *)p, id, scale, sceneTag, 1);
                    break;
                }
                case 15: {
                    u8 colour[4];
                    f32 size;

                    colour[0] = 0xAE;
                    colour[1] = 0xD2;
                    colour[2] = 0xFF;
                    colour[3] = (math_random() % 61) + 0x96;
                    size = (math_randomFloat() * 20.0f) + 25.0f;
                    if (event->unk5 != 0) {
                        size *= 0.349999994f;
                    }
                    beam_spawnBoundsBolt(obj, math_random() & 0xFF, (math_random() % 129) - 0x3F,
                                  (math_random() % 21) + 0xA, 4, 40.0f, size, 0, 2, colour, sceneTag, 1);
                    break;
                }
                case 16:
                    beam_spawnMuzzleBurst(obj, event->unk5, event->unk6, sceneTag, 1);
                    break;
                case 17: {
                    s32 unused; // matching: stack padding
                    RumbleParams rumble;

                    rumble.flags = 1;
                    rumble.duration = 0x32;
                    rumble.strength = 8;
                    rumble.playerMask = 1;
                    rumble.updateKind = -1;
                    cont_spawnRumble(&rumble, 0, sceneTag, 1);
                    break;
                }
                case 18: {
                    if (obj->ob_matrix_ptr) {
                        Vec3f out;
                        const Vec3f offs[2] = { { -65.0f, 172.0f, 9.0f }, { 15.0f, 63.0f, 9.0f } };

                        math_transformOffset(&offs[event->unk5], &out, obj->ob_matrix_ptr);
                        particle_spawnFlash(&out, 0.6460000277f, sceneTag, 0);
                    }
                    break;
                }
                case 19: {
                    if (obj->ob_matrix_ptr) {
                        Vec3f out;
                        const Vec3f offs[8] = { { 0.0f, -14.0f, 18.0f }, { 0.0f, -14.0f, 18.0f }, { 0.0f, -14.0f, 18.0f }, { 0.0f, -14.0f, 18.0f }, { -5.0f, 25.0f, -7.0f }, { -11.0f, 28.0f, -5.0f }, { -4.0f, 30.0f, -4.0f }, { 3.0f, 30.0f, -8.0f } };
                        const u8 bones[8] = { 7, 8, 9, 10, 13, 14, 15, 16 };

                        math_transformOffset(&offs[event->unk5], &out, &obj->ob_matrix_ptr[bones[event->unk5]]);
                        particle_spawnFlash(&out, 6.990000248f, sceneTag, 0);
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
                    } origin;

                    state = &gSeqTrackStates[scene][track];
                    origin.x = state->x;
                    origin.y = state->y + 50.0f;
                    origin.z = state->z;
                    effect_spawnModelBurst(&origin, 1, 1, 2, sceneTag, 0);
                    break;
                }
                case 21:
                    if (obj != NULL) {
                        actor_applyHeadShot(obj);
                        blood_spawnFatalHit(obj, sceneTag, 0);
                    }
                    break;
                case 22:
                    if (obj != NULL) {
                        particle_spawnBoneModelBurst(obj, sceneTag, 0);
                    }
                    break;
            }
        } else if (event->value == 0x40) {
            particle_startSparkEmitter(obj, 0x28, sceneTag, 1);
        } else if (event->value == 0x4D) {
            DebrisEmitter debris;

            debris.count = (math_random() % 5) + 5;
            debris.pos.x = pos[0];
            debris.pos.y = pos[1];
            debris.pos.z = pos[2];
            debris.radius = 4.700000286f;
            debris.speedBase = 24.80000114f;
            debris.speedRange = 15.5f;
            debris.sizeBase = 0.2670000196f;
            debris.lifetime = 0x5A;
            debris.yawBase = -7;
            debris.yawRange = 0x2D;
            debris.pitchBase = -0x34;
            debris.pitchRange = 0x20;
            debris.landPuffs = 0;
            debris.landPuffFlag = 0;
            debris.landPuffLifeBase = 0x32;
            debris.landPuffLifeRange = 0x19;
            debris.sizeRange = 0.2430000156f;
            debris.gravityBase = -2.041000128f;
            debris.dir.x = 0.0f;
            debris.dir.y = 0.0f;
            debris.dir.z = 0.0f;
            debris.gravityRange = 0;
            debris.spinBase = -3.0f;
            debris.spinRange = 6.0f;
            blood_spawnDebris(&debris, 1, sceneTag, 1);
        } else if (event->value == 0x4E) {
            DebrisEmitter debris;

            debris.count = (math_random() % 6) + 6;
            debris.pos.x = pos[0];
            debris.pos.y = pos[1];
            debris.pos.z = pos[2];
            debris.radius = 151.5f;
            debris.speedBase = 15.10000038f;
            debris.speedRange = 25.20000076f;
            debris.sizeBase = 0.3880000114f;
            debris.lifetime = 0x5A;
            debris.yawRange = 0xFF;
            debris.yawBase = 0;
            debris.pitchBase = -0x40;
            debris.pitchRange = 0x28;
            debris.landPuffs = 0;
            debris.landPuffFlag = 0;
            debris.landPuffLifeBase = 0x32;
            debris.landPuffLifeRange = 0x19;
            debris.sizeRange = 0.2430000156f;
            debris.gravityBase = -1.0200001f;
            debris.dir.x = 0.0f;
            debris.dir.y = 0.0f;
            debris.dir.z = 0.0f;
            debris.gravityRange = 0;
            debris.spinBase = -4.0f;
            debris.spinRange = 8.0f;
            blood_spawnDebris(&debris, 1, sceneTag, 1);
        } else if (event->value == 0x4F) {
            SplashSpraySpec splash;

            splash.spray.pos.x = pos[0];
            splash.spray.pos.y = pos[1];
            splash.spray.pos.z = pos[2];
            splash.spray.scaleBase = 2.900000095f;
            splash.spray.scaleRange = 17.0f;
            splash.spray.sizeBase = 2.5f;
            splash.spray.countBase = 0xF;
            splash.spray.sizeRange = 0.5770000219f;
            splash.spray.speedBase = 25.0f;
            splash.spray.speedRange = 20.20000076f;
            splash.spray.countRange = 0xA;
            splash.angles.yawBase = -0x14;
            splash.angles.yawRange = 0x31;
            splash.angles.pitchBase = -0x34;
            splash.angles.pitchRange = 0x2B;
            splash.spray.itemCountBase = 3;
            splash.spray.itemCountRange = 4;
            splash.spray.lifeBase = 0x28;
            splash.spray.lifeRange = 0x1E;
            splash.spray.alphaBase = 0x64;
            splash.spray.alphaRange = 0x64;
            splash.spray.kind = 0;
            splash.spray.fadeStart = 0x10;
            splash.spray.fadeRate = 0xF;
            splash.spray.material = 0;
            splash.spray.burstChance = 0.5f;
            particle_spawnSplashSprayAtAngles(&splash.angles, &splash.spray, 0, sceneTag, 1);
        } else if (event->value == 0x24) {
            Vec3f p;
            s32 count;
            u8 type;

            p.x = pos[0];
            p.y = pos[1];
            p.z = pos[2];
            count = (event->unk4 != 0) ? event->unk4 : 0xA;
            type = event->unk5;
            if (type >= 0xD) {
                type = 0;
            }
            particle_throwSplashes(&p, count, type, sceneTag, 0);
        } else if (event->value == 0x10) {
            u8 type;

            type = event->unk4;
            if (type >= 0x1B) {
                type = 0;
            }
            blood_sprayFromBone(obj, type, sceneTag, 0);
        } else if (event->value == 0x47) {
            particle_startObjDustEmitter(obj, 0x32, sceneTag, 0);
        } else if (event->value == 0x2E) {
            func_150C1F60(sceneTag);
            func_150C2290(sceneTag);
            func_150C2424(sceneTag);
        } else if (event->value == 0x31) {
            beam_spawnBoneQuad(obj, 0, sceneTag, 0);
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
            state = &gSeqTrackStates[scene][track];
            i4 = state->unk20 * 0.05000000075f;
            i5 = (state->yaw + 180.0f) * 0.7111111283f;
            i6 = state->pitch * 0.7111111283f;
            f7 = (state->yOffset * 0.1000000015f) + 0.5f;
            f9 = (state->unk18 * 0.1000000015f) + 1.0f;
            f10 = state->unk1C * 0.1000000015f;
            n = (event->unk5 != 0) ? event->unk5 : 0x64;
            trail_launchMotionTrail(type, pos[0], pos[1], pos[2], i4, i5, i6, f7, n, f9, f10, sceneTag);
        } else if (event->value == 0x35) {
            s32 type;

            type = (event->unk4 >= 2) ? 1 : event->unk4;
            effect_startPuffJets(obj, type, sceneTag);
        } else if (event->value == 0x26) {
            SeqEvent *found;
            s32 t;

            if (obj == NULL) {
                return 1;
            }
            t = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene);
            if (event->unk4 == 0) {
                obj->headEffect = 1;
                obj->headEffectTimer = t - eventTime;
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
            particle_spawnDroplets(pos, dist, speed, n, sceneTag);
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
            particle_spawnModelDroplets(pos, dist, speed, n, 0x1E, 0x64, sceneTag);
        } else if (event->value == 0x28) {
            if (actor != NULL) {
                effect_spawnObjectRectangleParticles(actor, sceneTag, 0);
            } else if (obj != NULL) {
                func_15171BF4(obj, sceneTag);
            }
        } else if (event->value == 0x29) {
            if (actor != NULL) {
                effect_spawnLevelModelBurst(actor, sceneTag, 0);
            } else if (obj != NULL) {
                func_151711C4(obj);
            }
        } else if (event->value == 0x2A) {
        } else if (event->value == 0x2B) {
            particle_spawnModelBurstWithBillboard(pos[0], pos[1], pos[2], sceneTag, 0);
        } else if (event->value == 0x2C) {
            SeqEvent *found;
            s32 pad[7]; // matching: stack padding

            cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene);
            if (event->unk4 == 0) { // matching: empty, but it still shapes the code
            }
        } else if (event->value == 0x2F) {
            s32 duration;
            SeqEvent *found;

            if (obj == NULL) {
                return 1;
            }
            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_1516ED68(obj - movement_obj, duration, sceneTag, 0);
            }
        } else if (event->value == 0x39) {
            s32 duration;
            SeqEvent *found;

            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                func_150C2804(pos, pos + 1, pos + 2, duration, sceneTag, 0);
            }
        } else if (event->value == 0x3A) {
            Vec3f p;
            s32 count;

            p.x = pos[0];
            p.y = pos[1];
            p.z = pos[2];
            count = (event->unk4 != 0) ? event->unk4 : 0xA;
            particle_throwSplashes(&p, count, 6, sceneTag, 0);
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

/*
 * Third cutscene event handler: sounds (types 6 and 10), func_1000xxxx audio
 * commands (8), object animations (2) and a few type-5 commands. `skipping` is set when the scene is
 * being skipped (gCutsceneSkipState == 2). Returns 1 when the event was handled.
 */
s32 cutscene_handleEvent3(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene, Actor *obj, WorldObject *actor, f32 *pos, s32 sceneTag, s32 skipping) {
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
            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 1, 0, 0, scene) - eventTime;
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
                markTime = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 2, 0, 0, scene);
                if (found != NULL) {
                    markTime -= eventTime;
                    if (markTime < duration) {
                        fadeIn = markTime;
                    }
                }
                markTime = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 3, 0, 0, scene);
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
                snd->sceneTag = gCutsceneIds[scene];
                snd->scene = scene;
                snd->track = track;
                snd->remaining = duration;
                snd->duration = duration;
                snd->fadeIn = fadeIn;
                snd->fadeOut = fadeOut;
                snd->volume = (vol * 0x7FFF) >> 8;
                snd->stopAtEnd = event->type == 10;
                sound_addDynamicSound(event->value, pos[0], pos[1], pos[2], snd->volume, 0x7D00, 0x7918,
                              (SoundRecordCallback)cutscene_updateSound, snd, 0, sflags, fade);
            }
        }
    } else if (event->type == 8) {
        s32 duration;
        SeqEvent *found;

        if (event->unk4 == 0) {
            if ((skipping != 0) && (func_1000E134(event->value) != 0)) {
                return 1;
            }
            music_playTune(event->value, 0, 0);
            if (event->unk6 != 0) { // matching: empty under -g3 but it still shapes the code
            }
        } else if (event->unk4 == 1) {
            music_setChannelVolume(event->value, event->unk6 * 100, 1 << (event->unk5 + 31));
        } else if (event->unk4 == 2) {
            music_setFade(event->value, event->unk6 * 328, 1);
        } else if (event->unk4 == 3) {
        } else if (event->unk4 == 4) {
            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 5, 1, event->unk5, scene) - eventTime;
            if (skipping != 0) {
                music_setChannelVolume(event->value, event->unk6, 1 << (event->unk5 + 31));
            } else {
                music_fadeChannelVolume(event->value, event->unk6, 1 << (event->unk5 + 31), duration);
            }
        } else if (event->unk4 == 6) {
            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 7, 0, 0, scene) - eventTime;
            if (duration < 2) {
                duration = 1;
            }
            music_setFade(event->value, event->unk6 * 328, duration);
        } else if (event->unk4 == 8) {
            music_jumpToMarker(event->value, event->unk6 + 1, 0, -1);
        } else if (event->unk4 == 9) {
            music_stopTune(event->value, 0);
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
        duration = cutscene_findEvent(event, event->type, track, &found, 0, 0, 0, 0, 0, 0, scene) - eventTime;
        if (event->unk4 == 0) {
            if (obj->anim == NULL) {
                return 1;
            }
            rate = 5.0f;
            id = event->value;
            if (event->unk6 > 0) {
                rate = event->unk6 - 1;
            }
            if (eventTime == gCutsceneStartTime) {
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
            if (id != obj->animseq.uh) {
                actor_playAnim(obj, id, 1.0f, rate, 0.0f, 0.0f, loop);
            }
            if (duration > 0.0f) {
                speed = obj->anim->numFrames[0] / duration * 2.0f;
            } else {
                speed = 0.0f;
            }
            if (event->unk5 != 0) {
                speed = 0.0f;
            }
            actor_playAnim(obj, id, speed, rate, 0.0f, 0.0f, loop);
        }
    } else if (event->type == 5) {
        if (event->value == 1) {
            SeqEvent *found;
            s32 duration;
            SeqEvent *prev;
            s32 prevTime;

            if (event->unk4 == 0) {
                duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
                if (skipping != 0) {
                    duration = 0;
                }
                screen_startViewFade(gCutsceneFadeColours[event->unk5].r, gCutsceneFadeColours[event->unk5].g, gCutsceneFadeColours[event->unk5].b, duration, !event->unk6, 0);
                gCutsceneFadeToColor = !event->unk6;
            } else if (skipping != 0) {
                cutscene_findEventsAround(&prev, &found, NULL, &prevTime, track, event->type, 1, event->value, 1, 0, gCutsceneTimes[scene], scene);
                if (prev != NULL) {
                    screen_startViewFade(gCutsceneFadeColours[prev->unk5].r, gCutsceneFadeColours[prev->unk5].g, gCutsceneFadeColours[prev->unk5].b, 0, !prev->unk6, 0);
                    gCutsceneFadeToColor = !prev->unk6;
                }
            }
        } else if (event->value == 4) {
            s32 kind;

            if (obj != NULL) {
                if (skipping == 0) {
                    kind = (obj->character_type == 0x4B) ? 2 : -1;
                    if (kind != -1) {
                        // matching: the original reuses the (otherwise unused) index parameter as the temp
                        index = (event->unk4 == 0) ? -1 : event->unk4 - 1;
                        model_breakIntoParts(obj, kind, index);
                    }
                } else {
                    actor_remove(obj, 1);
                }
            }
        } else if (event->value == 0x5C) {
            SeqEvent *found;
            s32 fadeIn;
            s32 fadeOut;
            s32 start;

            if (event->unk4 == 0) {
                if (skipping == 0) {
                    start = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 1, 1, event->unk5, scene);
                    fadeIn = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 2, 1, event->unk5, scene) - eventTime;
                    if (found == NULL) {
                        fadeIn = 0;
                    }
                    fadeOut = start - cutscene_findEvent(event, event->type, track, &found, 1, event->value, 1, 3, 1, event->unk5, scene);
                    if (found == NULL) {
                        fadeOut = 0;
                    }
                    beam_showFadeSprite(fadeIn, start - eventTime - fadeOut - fadeIn, fadeOut, event->unk5);
                }
            }
        } else if (event->value == 0x56) {
            if (event->unk4 == 0) {
                if (skipping == 0) {
                    effect_requestRainState(3);
                }
            } else if (skipping != 0) {
                effect_requestRainState(2);
            } else if (event->unk5 == 0) {
                effect_requestRainState(0);
            } else {
                effect_requestRainState(1);
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
            screen_setFrameEcho(event->unk6, mode);
        } else if (event->value == 0x2D) {
            s32 duration;
            SeqEvent *found;

            duration = cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime;
            if (actor != NULL) {
                if (event->unk4 == 0) {
                    trail_startObjectAfterimage(actor, duration, event->unk5);
                } else if (skipping != 0) { // matching: empty, but it still shapes the code
                }
            } else if (obj != NULL) {
                if (event->unk4 == 0) {
                    trail_startActorAfterimage(obj, duration, event->unk5);
                } else if (skipping != 0) {
                    trail_removeActorAfterimages(obj);
                }
            }
        } else if (event->value == 0x57) {
            s32 duration; // matching: unused, but its stack slot is needed
            SeqEvent *found;

            if (event->unk4 == 0) {
                effect_showPlayerMarker(0, cutscene_findEvent(event, event->type, track, &found, 1, event->value, 0, 0, 0, 0, scene) - eventTime, 0.0f);
            } else if (skipping != 0) {
                dc_glbl_CharNum = 0;
                effect_keepPlayerMarkerShown();
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
void cutscene_dispatchEvent(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene) {
    Actor *obj;
    WorldObject *actor;
    f32 *pos;
    SeqTrackInfo *info;
    s32 sceneTag;

    obj = NULL;
    actor = NULL;
    pos = NULL;
    sceneTag = gCutsceneIds[scene];
    info = &gSeqTrackInfos[scene][track];
    if (info->type == 2) {
        obj = collision_findObjectById(info->targetId);
        if (obj == NULL) {
            return;
        }
        pos = &obj->xpos;
    } else if (info->type == 3) {
        actor = gfx_findObjectById(info->targetId);
        if (actor == NULL) {
            return;
        }
    }
    if (pos == NULL) {
        pos = &gSeqTrackStates[scene][track].x;
    }
    if (gCutsceneSkipState != 2) {
        if (cutscene_handleEvent1(event, track, index, eventTime, scene, obj, actor, pos, sceneTag)) {
            return;
        }
        if (cutscene_handleEvent2(event, track, index, eventTime, scene, obj, actor, pos, sceneTag)) {
            return;
        }
        if (cutscene_handleEvent3(event, track, index, eventTime, scene, obj, actor, pos, sceneTag, 0)) {
            return;
        }
    } else {
        if (cutscene_handleEvent1(event, track, index, eventTime, scene, obj, actor, pos, sceneTag)) {
            return;
        }
        cutscene_handleEvent3(event, track, index, eventTime, scene, obj, actor, pos, sceneTag, 1);
    }
}
