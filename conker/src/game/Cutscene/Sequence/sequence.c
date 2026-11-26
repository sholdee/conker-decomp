#include <ultra64.h>
#include <PR/sched.h>
#include "structs.h"

/* Cutscene ("sequence") data is kept per scene slot (0/1) and per track (up to 30 tracks). */

/* A channel keyframe: a timed value for one animated property (gSeqChannelKeys[scene][track][key]). */
typedef struct {
    /* 0x0 */ f32 value;
    /* 0x4 */ u8 interp;
    /* 0x5 */ u8 channel;
    /* 0x6 */ s16 time;
} SeqChannelKey; // size 0x8

/* Arc-length data for a track's path (gSeqTrackPaths[scene][track]), plus the cubic of the cached segment. */
typedef struct {
    /* 0x00 */ f32 length;          // total length of the path
    /* 0x04 */ f32 *keyDistance;    // distance along the path at each SeqPathKey
    /* 0x08 */ f32 coef0[3];        // segment point = ((coef3 * t + coef2) * t + coef1) * t + coef0
    /* 0x14 */ f32 coef1[3];
    /* 0x20 */ f32 coef2[3];
    /* 0x2C */ f32 coef3[3];
    /* 0x38 */ f32 *arcLength;      // distance at t = 0, 0.01, ... 1.00 along the cached segment
    /* 0x3C */ u8 segment;          // SeqPathKey index of the cached segment; 0xFF = none
    /* 0x3D */ u8 pad3D[3];
} SeqTrackPath; // size 0x40

/* Current animated values of a track (one SeqTrackState array per slot at gSeqTrackStates). Which property
   each unk field holds depends on the track's SeqTrackInfo type. */

/* Cutscene file layout: a SeqFileEntry table whose entry 0 points at this header, followed by
   numTracks path-key lists, numTracks entries starting with the SeqTrackInfo table, numChannelLists channel-key lists,
   numEventLists event lists and numUnkLists gSeqTextLists lists. */
typedef struct {
    u16 numTracks;
    u16 numChannelLists;
    u16 numEventLists;
    u16 numUnkLists;
} SeqFileHeader;

/* A block of the cutscene file; size >> 3 is its count of 8-byte elements. */
typedef struct SeqFileEntry {
    void *data;
    u32 size;
} SeqFileEntry;

/* This file's .bss, in address order (0x800BEAD0..0x800C3D60). It is defined before the headers
   declare it: an earlier extern changes the order (see init/Sched/SchedInit/schedinit.c). */
u64 *gAltUcodeData;
OSScTask *gCurGfxTask;
OSScTask gGfxTasks[2];
s32 D_800BEB98;
u64 *gUcodeData; // RSP ucode data, right after its text in gUcodeText
u8 D_800BEBA0;
s32 gDisplayListLimit;
s32 D_800BEBA8[2]; /* not referenced */
u64 gAltUcodeText[0x496];
u64 gUcodeText[0x496]; // RSP ucode text buffer
u8 gCutsceneSpawnedActorCounts[2];
u8 gCutsceneSpawnedActors[2][25];
u8 gCutsceneActivatedActorCounts[2];
u8 gCutsceneActivatedActors[2][30];
s16 gCutsceneAnchorPos[3];
f32 gCutsceneAnchorHeading;
s16 gNextCutsceneAnchorPos[3];
f32 gNextCutsceneAnchorHeading;
f32 gCutsceneAnchorTurn;
u8 D_800C35A8;
u8 D_800C35A9;
u8 gCutsceneKeepPlayer;
s32 gCutsceneTimes[2]; // per scene: current time
s32 gCutscenePrevTimes[2]; /* time at the previous update */
u8 D_800C35C0[2];
u8 gNextCutscene;
u8 gNextCutsceneMap;
u8 gPendingCutscene;
u8 D_800C35C5;
u16 *gSeqPathKeyCounts[2];
u16 *gSeqChannelKeyCounts[2];
u16 *gSeqTrackEventCounts[2]; /* each track's event count */
u8 **gSeqTextLists[2]; // per scene: text lists
u8 gCutsceneIds[2]; // per scene: changes with each run of the scene
u8 gCutsceneStates[2]; // per cutscene scene slot (0/1): cutscene state, 1 = playing
SeqTrackInfo *gSeqTrackInfos[2]; // per scene: what each track drives
u8 gSeqCameraTrack;
CamShot gCamShot; // scripted camera shot
u8 D_800C3638;
u8 gSeqTrackCounts[2]; /* track count */
s32 gCutsceneEndTimes[2]; /* end time */
f32 gViewFovOffset; // added to both FOVs of every view
f32 gNearPlaneOverride; // near-plane override when non-zero
f32 gFarPlaneOverride; // far-plane override when non-zero
u8 D_800C3654[2];
u8 D_800C3656[2];
u8 D_800C3658[2];
u8 gCutsceneKeepSpawnedActors[2];
u8 D_800C365C[2];
u8 D_800C365E[2];
u8 D_800C3660[2];
u8 gCutsceneHideWorld;
u8 gCutsceneAnchorSet;
SeqFileEntry *gCutsceneFiles[2];
u8 D_800C3670;
u8 D_800C3671;
u8 gCutsceneSkipped;
f32 gCutsceneCutbackAngle;
f32 gCutsceneEndTransitionYaw; // camera transition parameter (999.0f = unset); gCutsceneEndTransitionTime is its duration
s32 gCutsceneEndTransitionTime;
u8 D_800C3680;
u8 gCutsceneHideSky;
u8 D_800C3682;
u8 D_800C3683;
SeqEvent *gSeqEventTracks[2][30]; // cutscene script event tracks per scene
SeqChannelKey *gSeqChannelKeys[2][30];
SeqPathKey *gSeqPathKeys[2][30]; // per scene and track: path keyframes
SeqTrackState *gSeqTrackStates[2]; // per scene: the track states
u8 *gSeqActiveEvents[2][30];
SeqTrackPath *gSeqTrackPaths[2];
u8 D_800C3A58;
s64 gPlayedCutscenes[69];
u8 gCutsceneSkipState;
u8 gCutsceneSkipFadeTimer;
s8 D_800C3C8A;
u8 D_800C3C8B;
u8 gCutsceneHoldEnd;
u8 D_800C3C8D;
u16 gCutsceneStartTime;
u8 gCutsceneCallback;
s32 gCutsceneCallbackArg;
u8 gLastCutscene;
u8 gCutsceneChained;
u16 gSeqEventQueueSize;
u8 gCutsceneNoSkip;
u8 gCutsceneChainBobScale;
s16 D_800C3C9E;
SeqSubtitle gSubtitles[3];
u8 D_800C3D48[8]; // not referenced, but cutscene_closeSubtitles stops at its address: the end of gSubtitles
struct SeqQueueEntry *gSeqEventQueue; // dispatch queue, gSeqEventQueueSize entries (type in 50D80)

#include "functions.h"
#include "variables.h"

/* Defined in game/Effects/ObjectList/objectlist.c. Not in effect.h: game/Math/MathUtil/mathutil.c calls it with two more arguments. */
void effect_broadcastEvent(s32 event, u8 eventType);

extern u64 D_800CBE10[];

/* Append a parameterless GBI command (gDPNoParam) at the display-list write head gDisplayListHead. */
#define GFX_APPEND_NOPARAM(cmd)                 \
{                                               \
    Gfx *gfx = gDisplayListHead++;                    \
    gfx->words.w0 = _SHIFTL(cmd, 24, 8);        \
    gfx->words.w1 = 0;                          \
}

/* Finish this frame's display list and send it to the scheduler as an RSP graphics task. */
void cutscene_sendGfxTask(s32 unused, s32 swapBuffers) {
    s32 capacity;
    Gfx **dlBase;
    s32 used;

    dlBase = gDisplayListBuffers;
    capacity = dlBase[1] - dlBase[0];
    gCurGfxTask = &gGfxTasks[gDrawBufferIndex];
    gCurGfxTask->next = 0;
    used = gDisplayListHead - dlBase[gDrawBufferIndex];
    if ((used < 0) || (used > capacity)) {
        view_initFrameDisplayLists();
        gDisplayListHead = gDisplayListPrologueEnds[gDrawBufferIndex];
        GFX_APPEND_NOPARAM(G_RDPFULLSYNC);
        GFX_APPEND_NOPARAM(G_ENDDL);
        used = gDisplayListHead - dlBase[gDrawBufferIndex];
    }

    if ((used < 0) || (used > (capacity - 400))) {
        gDisplayListHead = dlBase[gDrawBufferIndex] + (capacity - 400);
        GFX_APPEND_NOPARAM(G_RDPFULLSYNC);
        GFX_APPEND_NOPARAM(G_ENDDL);
    }

    gCurGfxTask->list.t.data_ptr = (u64 *)dlBase[gDrawBufferIndex];
    gCurGfxTask->list.t.data_size = (gDisplayListHead - dlBase[gDrawBufferIndex]) * sizeof(Gfx);
    cutscene_loadMicrocodes();

    gCurGfxTask->list.t.type = M_GFXTASK;
    gCurGfxTask->list.t.flags = OS_TASK_LOADABLE;
    gCurGfxTask->list.t.ucode_boot = (u64 *)D_100290D0;
    gCurGfxTask->list.t.ucode_boot_size = D_100291A0 - D_100290D0;
    if (D_800BEBA0 != 0) {
        gCurGfxTask->list.t.ucode = gUcodeText;
        gCurGfxTask->list.t.ucode_data = gUcodeData;
    } else {
        gCurGfxTask->list.t.ucode = gAltUcodeText;
        gCurGfxTask->list.t.ucode_data = gAltUcodeData;
    }
    gCurGfxTask->list.t.ucode_data_size = 0x800;
    gCurGfxTask->list.t.dram_stack = D_800CBE10;
    gCurGfxTask->list.t.dram_stack_size = 0x400;
    gCurGfxTask->list.t.output_buff = (u64 *)gRdpOutputBuffer;
    gCurGfxTask->list.t.output_buff_size = (u64 *)gRdpOutputBufferEnd;
    gCurGfxTask->list.t.yield_data_ptr = gGfxYieldBuffer;
    gCurGfxTask->list.t.yield_data_size = 0xC00;
    gCurGfxTask->flags = 0x23;
    if (swapBuffers != 0) {
        gCurGfxTask->flags |= 0x40;
    }
    if (D_800BE617 != 0) {
        gCurGfxTask->flags |= (gScreenLayout + 1) << 16;
        D_800BE617 = 0;
    }
    gCurGfxTask->msgQ = &gGameMesgQueue;
    gCurGfxTask->msg = &gTaskDoneMsgs[gDrawBufferIndex];
    gCurGfxTask->framebuffer = gFramebuffers[gDrawBufferIndex];
    osWritebackDCacheAll();
    osSendMesg(&gSchedTaskQueue, gCurGfxTask, 1);
    if (swapBuffers != 0) {
        gDrawBufferIndex ^= 1;
    }
    gPendingGfxTasks++;
}

/* ROM offsets (relative to ucode_ROM_START) of one RSP microcode's text and data. */
typedef struct {
    s32 textStart;
    s32 textEnd;
    s32 dataStart;
    s32 dataEnd;
} UcodeRomEntry;

extern u8 ucode_ROM_START[];

static s8 gLoadedUcode = -1; // the ucode pair now loaded (-1: none yet)
static s8 gLoadedAltUcodeOffset = -1; // its offset (0 or 4) into gUcodeRomTable for gAltUcodeText's ucode (-1: none yet)
/* Indexed by the graphics mode: entries 0-3 for gUcodeText, 4-7 for gAltUcodeText. */
static UcodeRomEntry gUcodeRomTable[8] = {
    { 0x4560, 0x5A70, 0x10460, 0x10B20 },
    { 0x6F80, 0x84B0, 0x111E0, 0x118A0 },
    { 0x5A70, 0x6F80, 0x10B20, 0x111E0 },
    { 0x84B0, 0x99E0, 0x118A0, 0x11F60 },
    { 0x99E0, 0xAE80, 0x11F60, 0x12620 },
    { 0xC320, 0xD7C0, 0x12CE0, 0x133A0 },
    { 0xAE80, 0xC320, 0x12620, 0x12CE0 },
    { 0xD7C0, 0xEC60, 0x133A0, 0x13A60 },
};
static s32 D_800840F8 = 0; /* not referenced */

/* DMA the two RSP microcodes (gUcodeText and gAltUcodeText, each text then data) for the current
   graphics mode, unless they are already loaded. */
void cutscene_loadMicrocodes(void) {
    s32 ucode;
    s32 altOffset;
    UcodeRomEntry *alt;

    ucode = D_800B0DF0->ucode;
    if (ucode >= 2) {
        ucode -= 2;
        altOffset = 0;
    } else {
        altOffset = 4;
    }
    if (D_800BEB98 == 1) {
        ucode += 2;
    }
    if ((ucode != gLoadedUcode) || (altOffset != gLoadedAltUcodeOffset)) {
        rom_read(gUcodeRomTable[ucode].textStart + (s32)ucode_ROM_START, gUcodeText, gUcodeRomTable[ucode].textEnd - gUcodeRomTable[ucode].textStart, 1);
        gUcodeData = (u64 *)((gUcodeRomTable[ucode].textEnd - gUcodeRomTable[ucode].textStart) + (u8 *)gUcodeText);
        rom_read(gUcodeRomTable[ucode].dataStart + (s32)ucode_ROM_START, gUcodeData, gUcodeRomTable[ucode].dataEnd - gUcodeRomTable[ucode].dataStart, 1);
        gLoadedUcode = ucode;
        gLoadedAltUcodeOffset = altOffset;
        alt = gUcodeRomTable + (ucode + altOffset);
        rom_read(alt->textStart + (s32)ucode_ROM_START, gAltUcodeText, alt->textEnd - alt->textStart, 1);
        gAltUcodeData = (u64 *)((alt->textEnd - alt->textStart) + (u8 *)gAltUcodeText);
        rom_read(alt->dataStart + (s32)ucode_ROM_START, gAltUcodeData, alt->dataEnd - alt->dataStart, 1);
    }
}

/* Mark every track's active-event table (gSeqActiveEvents) as empty. */
void cutscene_clearActiveEvents(s32 scene) {
    s32 i;
    s32 j;

    for (i = 0; i < gSeqTrackCounts[scene]; i++) {
        for (j = 0; j < 16; j++) {
            gSeqActiveEvents[scene][i][j] = 0xFF;
        }
    }
}


/* Work out the scene's end time gCutsceneEndTimes[scene]: track 0's header length if set, else the latest
   header end time among tracks with events, else the time of the last channel key. */
void cutscene_computeEndTime(s32 scene) {
    s32 found;
    s32 i;
    s16 length;
    u16 numKeys;

    gCutsceneEndTimes[scene] = 0;
    if (gSeqTrackCounts[scene] != 0) {
        if (gSeqTrackEventCounts[scene][0] >= 4) {
            length = gSeqEventTracks[scene][0][2].value;
            if (length != -9999) {
                gCutsceneEndTimes[scene] = length;
                return;
            }
        }
        found = 0;
        for (i = 0; i < gSeqTrackCounts[scene]; i++) {
            if (gSeqPathKeyCounts[scene][i] != 0 && gSeqTrackEventCounts[scene][i] != 0) {
                if (gCutsceneEndTimes[scene] < gSeqEventTracks[scene][i][1].value) {
                    gCutsceneEndTimes[scene] = gSeqEventTracks[scene][i][1].value;
                    found = 1;
                }
            }
        }
        if (found == 0) {
            for (i = 0; i < gSeqTrackCounts[scene]; i++) {
                if (gSeqPathKeyCounts[scene][i] != 0) {
                    numKeys = gSeqChannelKeyCounts[scene][i];
                    if (numKeys) {
                        if (gCutsceneEndTimes[scene] < gSeqChannelKeys[scene][i][numKeys - 1].time) {
                            gCutsceneEndTimes[scene] = gSeqChannelKeys[scene][i][numKeys - 1].time;
                        }
                    }
                }
            }
        }
    }
}

/* Total event count of all tracks of the scene. */
/* Total event count of all tracks of the scene. */
/* Total event count of all tracks of the scene. */
s32 cutscene_countEvents(s32 scene) {
    s32 sum;
    s32 i;
    s32 count;
    s32 limit;
    u16 *counts;

    sum = 0;
    i = 0;
    if ((count = gSeqTrackCounts[scene]) > 0) {
        limit = count;
        counts = gSeqTrackEventCounts[scene];
    // matching: a goto loop; IDO unrolls the equivalent for and do-while loops
    loop:
        sum += *counts;
        i++;
        counts++;
        if (i < limit) {
            goto loop;
        }
    }
    return sum;
}


/* Reset the per-scene cutscene state before a new script is loaded. */
void cutscene_resetScene(s32 scene) {
    gCutscenePrevTimes[scene] = -100;
    gCutsceneTimes[scene] = 0;
    D_800C35C0[scene] = 0;
    gCutsceneStartTime = 0;
    gSeqTrackCounts[scene] = 0;
    gCutsceneStates[scene] = 0;
    gSeqEventQueue = NULL;
    gSeqCameraTrack = 0xFF;
    gViewFovOffset = 0.0f;
    gNearPlaneOverride = 0.0f;
    gFarPlaneOverride = 0.0f;
    D_800C3654[scene] = 0;
    D_800C3656[scene] = 0;
    D_800C3658[scene] = 1;
    gCutsceneKeepSpawnedActors[scene] = 0;
    D_800C365C[scene] = 0;
    D_800C365E[scene] = 0;
    D_800C3660[scene] = 0;
    D_800C3638 = 0;
    gCutsceneHideWorld = 0;
    gCutsceneHoldEnd = 0;
    D_800C3C8D = 0;
    gNextCutscene = 0;
    D_800C3A58 = 0;
    gCutsceneAnchorTurn = 0.0f;
    D_800C3C8A = -1;
    D_800C3C8B = 0;
    gCutsceneCutbackAngle = 999.0f;
    gCutsceneEndTransitionYaw = 999.0f;
    gCutsceneEndTransitionTime = 0;
    D_800C3680 = 0;
    gCutsceneSkipped = 0;
    gCutsceneChainBobScale = 100;
    D_800C3C9E = -1;
    cutscene_freeSubtitles();
    D_8008FD84[0] = 0;
    D_800C3670 = 0;
}

/* Load cutscene `cutscene` of map `map` into gCutsceneFiles[scene]; returns the file (0 if missing). */
SeqFileEntry *cutscene_loadFile(s32 map, s32 cutscene, s32 scene) {
    s32 size;
    u32 unused;
    SeqFileEntry *file;

    file = rom_loadPathTable(&unused, 0, &size, 3, 6, map, cutscene);
    if (file != NULL) {
        gCutsceneFiles[scene] = file;
    } else {
        gCutsceneFiles[scene] = NULL;
    }

    return file;
}

/* Flag cutscene `cutscene` of map `map` as played (unless D_800C3670 is set). */
void cutscene_setPlayed(s32 map, s32 cutscene) {
    if (!D_800C3670) {
        gPlayedCutscenes[map] |= (1LL << cutscene);
    }
}

/* Whether cutscene `cutscene` of map `map` has been played; always true while D_800C3670 is set. */
s32 cutscene_isPlayed(s32 map, s32 cutscene) {
    if (D_800C3670 != 0) {
        return 1;
    }
    if ((gPlayedCutscenes[map] & (1ULL << cutscene)) != 0) {
        return 1;
    }
    return 0;
}

/* Cutscene start callbacks (cutscene_start's `callback`; 0: none). */
static void (*gCutsceneStartCallbacks[])(s32, s32, s32, s32) = { NULL, cutscene_startCallback1 };

/* Start cutscene `cutscene` of map `map` in scene slot `scene`. If another map is loaded, switch
   maps first (game_requestLevelChange) and leave the request pending in gPendingCutscene. Otherwise load the
   cutscene file, allocate and fill the per-track tables, and run the first frame. */
void cutscene_start(s32 map, s32 cutscene, s32 scene, s32 callback, s32 callbackArg) {
    SeqFileHeader *hdr;
    SeqFileEntry *file;
    s32 state;
    s32 i;
    s32 k;
    s32 size;

    state = gCutsceneStates[scene];
    if (state != 1 && gPendingCutscene == 0) {
        gCutsceneCallback = callback;
        gCutsceneCallbackArg = callbackArg;
        if (map != level) {
            gPendingCutscene = cutscene + 1;
            game_requestLevelChange(1, map, 0, 0, 0);
            if (gCutsceneStates[scene] != 0) {
                D_800C35C5 = 0;
                return;
            }
            D_800C35C5 = 1;
            return;
        }
        gCutsceneIds[scene] = cutscene;
        if (state != 2) {
            D_800C35A8 = level;
            gLastCutscene = cutscene;
        }
        cutscene_resetScene(scene);
        if (cutscene_loadFile(map, cutscene, scene) != 0) {
            file = gCutsceneFiles[scene];
            hdr = file[0].data;
            gSeqPathKeyCounts[scene] = allocate_memory(hdr->numTracks * sizeof(u16), 1, 0, 0);
            k = 1;
            for (i = 0; i < hdr->numTracks; i++) {
                gSeqPathKeyCounts[scene][i] = file[k].size >> 3;
                k++;
            }
            size = hdr->numChannelLists;
            if (size < hdr->numTracks) {
                size = hdr->numTracks;
            }
            gSeqChannelKeyCounts[scene] = allocate_memory(size * sizeof(u16), 1, 0, 0);
            bzero(gSeqChannelKeyCounts[scene], size * sizeof(u16));
            size = hdr->numEventLists;
            if (size < hdr->numTracks) {
                size = hdr->numTracks;
            }
            gSeqTrackEventCounts[scene] = allocate_memory(size * sizeof(u16), 1, 0, 0);
            bzero(gSeqTrackEventCounts[scene], size * sizeof(u16));
            gSeqTextLists[scene] = allocate_memory(hdr->numUnkLists * sizeof(s32 *), 1, 0, 0);
            gSeqTrackPaths[scene] = allocate_memory(hdr->numTracks * sizeof(SeqTrackPath), 1, 0, 0);
            for (i = 0; i < hdr->numTracks; i++) {
                if (gSeqPathKeyCounts[scene][i] != 0) {
                    gSeqTrackPaths[scene][i].keyDistance = allocate_memory(gSeqPathKeyCounts[scene][i] * sizeof(f32), 1, 0, 0);
                    gSeqTrackPaths[scene][i].arcLength = allocate_memory(101 * sizeof(f32), 1, 0, 0);
                }
            }
            gSeqTrackStates[scene] = allocate_memory(hdr->numTracks * sizeof(SeqTrackState), 1, 0, 0);
            for (i = 0; i < hdr->numTracks; i++) {
                gSeqActiveEvents[scene][i] = allocate_memory(16, 1, 0, 0);
            }
            for (i = 0; i < hdr->numTracks; i++) {
                gSeqTrackEventCounts[scene][i] = 0;
                gSeqChannelKeyCounts[scene][i] = 0;
                gSeqPathKeyCounts[scene][i] = 0;
            }
            gSeqTrackCounts[scene] = hdr->numTracks;
            k = 1;
            for (i = 0; i < hdr->numTracks; i++) {
                gSeqPathKeyCounts[scene][i] = file[k].size >> 3;
                gSeqPathKeys[scene][i] = file[k].data;
                k++;
            }
            gSeqTrackInfos[scene] = file[k].data;
            k += hdr->numTracks;
            for (i = 0; i < hdr->numChannelLists; i++) {
                gSeqChannelKeyCounts[scene][i] = file[k].size >> 3;
                gSeqChannelKeys[scene][i] = file[k].data;
                k++;
            }
            for (i = 0; i < hdr->numEventLists; i++) {
                gSeqTrackEventCounts[scene][i] = file[k].size >> 3;
                gSeqEventTracks[scene][i] = file[k].data;
                k++;
            }
            for (i = 0; i < hdr->numUnkLists; i++) {
                gSeqTextLists[scene][i] = file[k].data;
                k++;
            }
            if (callback != 0) {
                gCutsceneStartCallbacks[callback](map, cutscene, scene, callbackArg);
            }
            gSeqEventQueueSize = cutscene_countEvents(scene);
            gSeqEventQueue = allocate_memory(gSeqEventQueueSize * 12, 1, 0, 0);
            gCutsceneTimes[scene] = 0;
            cutscene_clearActiveEvents(scene);
            cutscene_computeEndTime(scene);
            cutscene_anchorPaths(scene);
            for (i = 0; i < gSeqTrackCounts[scene]; i++) {
                cutscene_measurePath(i, scene);
            }
            cutscene_emptyStartHook();
            gCutsceneStates[scene] = 1;
            D_800C3638 = 1;
            cutscene_resetSpawnedActors(scene);
            cutscene_resetActivatedActors(scene);
            if (gSeqTrackCounts[scene] == 0) {
                cutscene_finish(0, scene);
            }
            cutscene_dispatchEvents(1, scene);
            cutscene_evalTracks(scene);
            cutscene_spawnTrackActors(scene);
            cutscene_activateTrackActors(scene);
            cutscene_applyObjectTracks(scene);
            cutscene_applyTracks(scene);
            gCameraSnapTimers[0] = 4;
            cutscene_prescanEvents(scene);
        }
    }
}


/* Scan the scene's scripts for type-5 events with values 0x1E, 0xE, 0x68 and 0x44 (setting the
   flags gCutsceneChained, gCutsceneNoSkip, gCutsceneKeepPlayer, D_800C35A9) and 0x77 (start time gCutsceneStartTime), and start
   the screen fade (screen_startViewFade) to the gCutsceneFadeColours colour named by the earliest type-1 value-1 event. */
void cutscene_prescanEvents(s32 scene) {
    s32 i;
    s32 best;
    SeqEvent *found;
    s32 score;
    SeqEvent *events;

    gCutsceneChained = 0;
    D_800C35A9 = 0;
    gCutsceneNoSkip = 0;
    gCutsceneKeepPlayer = 0;
    gCutsceneStartTime = 0;

    best = -1;
    for (i = 0; i < gSeqTrackCounts[scene]; i++) {
        events = gSeqEventTracks[scene][i];
        if (events != NULL) {
            found = NULL;
            cutscene_findEvent(&events[3], 5, i, &found, 1, 0x1E, 0, 0, 0, 0, scene);
            if (found != NULL) {
                gCutsceneChained = 1;
            }

            found = NULL;
            cutscene_findEvent(&events[3], 5, i, &found, 1, 0xE, 0, 0, 0, 0, scene);
            if (found != NULL) {
                gCutsceneNoSkip = 1;
            }

            found = NULL;
            cutscene_findEvent(&events[3], 5, i, &found, 1, 0x68, 0, 0, 0, 0, scene);
            if (found != NULL) {
                gCutsceneKeepPlayer = 1;
            }

            found = NULL;
            cutscene_findEvent(&events[3], 5, i, &found, 1, 0x44, 0, 0, 0, 0, scene);
            if (found != NULL) {
                D_800C35A9 = 1;
            }

            found = NULL;
            score = cutscene_findEvent(&events[3], 5, i, &found, 1, 0x77, 0, 0, 0, 0, scene);
            if (found != NULL) {
                gCutsceneStartTime = score;
            }

            found = NULL;
            score = cutscene_findEvent(&events[3], 5, i, &found, 1, 1, 1, 0, 0, 0, scene);
            if (found != NULL) {
                if ((best == -1) || (score < best)) {
                    best = score;
                    if (found->unk6 == 1) {
                        screen_startViewFade(gCutsceneFadeColours[found->unk5].r, gCutsceneFadeColours[found->unk5].g, gCutsceneFadeColours[found->unk5].b, 0, 1, 0);
                    } else {
                        screen_startViewFade(gCutsceneFadeColours[found->unk5].r, gCutsceneFadeColours[found->unk5].g, gCutsceneFadeColours[found->unk5].b, 0, 0, 0);
                    }
                }
            }
        }
    }

    gCutsceneTimes[scene] = gCutsceneStartTime;
}

/* Jump the scene to its end: advance the clock past the end time, update every track once, apply
   the result to the objects bound to tracks and finish the cutscene. */
void cutscene_jumpToEnd(s32 scene) {
    s32 i;
    s32 count;
    Actor *obj;

    gCutsceneTimes[scene] = gCutsceneEndTimes[scene] + gRawFrameTicks;
    cutscene_dispatchEvents(1, 0);
    cutscene_evalTracks(0);
    cutscene_dispatchEvents(0, 0);
    cutscene_applyTracks(0);

    // matching: the object count lives in its own register
    for (obj = movement_obj, i = 0, count = 25; i != count; i++, obj++) {
        if (obj->type) {
            dc_glbl_CharNum = i;
            if (cutscene_isActorBound(obj)) {
                cutscene_applyObjectTrack(obj, scene, -1);
            }
        }
    }

    cutscene_finish(1, scene);
}

/* Skip the running cutscene: jump to its end (again while the next one chains on, gCutsceneChained),
   then restore the screen fade and clear the objects' cutscene state. */
void cutscene_skip(s32 scene) {
    Actor *obj;
    u8 chained;
    s32 sceneCopy;
    s32 i;

    if (mp3_isPlaying() != 0) {
        func_151F2BA8();
    }
    obj = event_getOwnedObject(movement_obj, 0);
    if (obj != NULL) {
        actor_remove(obj, 0);
    }
    chained = gCutsceneChained;
    cutscene_jumpToEnd(scene);
    if (chained != 0) {
        if (gPendingCutscene == 0) {
            do {
                chained = gCutsceneChained;
                // matching: the argument goes through a local copy
                cutscene_jumpToEnd(sceneCopy = scene);
            } while ((chained != 0) && (gPendingCutscene == 0));
        }
    }
    if (D_800C35A9 == 0 && D_800C3C8A != 1 && D_800C3C8D == 0) {
        screen_startViewFade(0, 0, 0, 0x1E, 0, 0);
    }
    for (i = 0; i < 25; i++) {
        if (movement_obj[i].type != 0) {
            movement_obj[i].fogMode = 0;
        }
    }
    gCutsceneAnchorSet = 0;
    func_1000E17C();
    gCameraSnapTimers[0] = 4;
    gCutsceneSkipped = 1;
}

/* Whether the running cutscene may be skipped now (button 0x20, or map-specific rules for maps
   0x1D and 0x21). */
s32 cutscene_canSkip(s32 scene) {
    s32 map;
    s32 skipRequest;

    map = level;
    // matching: holding the address as an integer keeps IDO from folding it into the loads
    skipRequest = (s32)D_8008FD84;

    if (map == 0x1D) {
        // matching: the goto orders the gCutsceneIds[0] address load after the branch
        if (*(s8 *)skipRequest == 0) {
            goto noRequest;
        }
        *(s8 *)skipRequest = 0;
        return 1;
    noRequest:
        if (gCutsceneIds[0] != 5) {
            return 0;
        }
    }

    if (map == 0x21) {
        if ((gButtonsPressed[0] & 0x1000) && (D_8000030C == 1) && (gCutsceneTimes[0] >= 301)) {
            return 1;
        }
        return 0;
    }

    if (gButtonsPressed[0] & 0x20) {
        if (gCutsceneNoSkip == 0) {
            if (D_800D2E40 != 0) {
                return 1;
            }
            if (cutscene_isPlayed(map, gCutsceneIds[scene]) != 0) {
                if ((gCutsceneChained != 0) || ((gCutsceneTimes[scene] + 30) < gCutsceneEndTimes[scene])) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Cutscene skip state machine (gCutsceneSkipState): 0 = waiting for a skip request (needs at least 91
   ticks left), 1 = fading out for 30 ticks (gCutsceneSkipFadeTimer), 2 = skipping (cutscene_skip). */
void cutscene_updateSkip(s32 scene) {
    if (gCutsceneSkipState == 0) {
        if (gCutsceneStates[scene] != 1) {
            return;
        }
        if (cutscene_canSkip(scene) == 0) {
            return;
        }
        D_8008FD84[0] = 0;
        if (gCutsceneEndTimes[scene] - gCutsceneTimes[scene] < 91) {
            return;
        }
        gCutsceneSkipState = 1;
        gCutsceneSkipFadeTimer = 30;
        screen_startViewFade(0, 0, 0, 0x1E, 1, 0);
        func_1000E17C();
        if ((func_151F2CDC() == 1) || (func_151F2CDC() == 2)) {
            func_151F2D6C(0, 0x2B02);
        }
    } else if (gCutsceneSkipState == 1) {
        if (gRawFrameTicks < gCutsceneSkipFadeTimer) {
            gCutsceneSkipFadeTimer -= gRawFrameTicks;
            return;
        }
        gCutsceneSkipFadeTimer = 0;
        gCutsceneSkipState = 2;
        cutscene_skip(scene);
        gCutsceneSkipState = 0;
    }
}

/* At the scene's end time, finish it (cutscene_finish) unless gCutsceneHoldEnd holds it; if a chained
   scene started, run its first frame. */
void cutscene_checkEnd(s32 scene) {
    if ((gCutsceneStates[scene] == 1) && (gCutsceneTimes[scene] >= gCutsceneEndTimes[scene])) {
        if (gCutsceneHoldEnd == 0) {
            cutscene_finish(0, scene);
            if (gCutsceneStates[scene] == 1) {
                cutscene_advanceTime(0);
                cutscene_dispatchEvents(1, 0);
                cutscene_evalTracks(0);
                cutscene_dispatchEvents(0, 0);
                cutscene_applyTracks(0);
                cutscene_checkEnd(0);
            }
        } else {
            gCutsceneHoldEnd = 2;
        }
    }
}

/* Advance the scene clock gCutsceneTimes by this frame's ticks, clamped to the end time. */
void cutscene_advanceTime(s32 scene) {
    if (D_800C3A58 != 0) {
        D_800C3A58 -= 1;
    }
    gCutscenePrevTimes[scene] = gCutsceneTimes[scene];
    if (gCutsceneStates[scene] == 1) {
        cutscene_updateSkip(scene);
        if ((scene != 0) || (gPaused == 0)) {
            gCutsceneTimes[scene] += gRawFrameTicks;
        }
        if ((D_800C35C0[scene] == 0) &&
            (gCutsceneStartTime == gCutscenePrevTimes[scene]) &&
            (gCutsceneStartTime != gCutsceneTimes[scene])) {
            gCutscenePrevTimes[scene] = -1;
        }
        if (func_151F2CDC() == 1) {
            face_processMessages();
        }
        if (gCutsceneTimes[scene] >= gCutsceneEndTimes[scene]) {
            gCutsceneTimes[scene] = gCutsceneEndTimes[scene];
        }
    }
}


/* Free everything cutscene_start allocated for the scene, and the cutscene file itself. */
void cutscene_freeScene(s32 scene) {
    s32 track;
    SeqFileHeader *hdr;
    u8 **active;

    if (gCutsceneFiles[scene] != NULL) {
        hdr = gCutsceneFiles[scene][0].data;
        for (track = 0; track < hdr->numTracks; track++) {
            if (gSeqPathKeyCounts[scene][track] != 0) {
                heap_free(gSeqTrackPaths[scene][track].keyDistance);
                heap_free(gSeqTrackPaths[scene][track].arcLength);
            }
        }
        heap_free(gSeqTrackPaths[scene]);
        gSeqTrackPaths[scene] = NULL;
        heap_free(gSeqTrackStates[scene]);
        gSeqTrackStates[scene] = NULL;
        active = gSeqActiveEvents[scene];
        for (track = 0; track < hdr->numTracks; track++) {
            heap_free(active[track]);
            active[track] = NULL;
        }
        heap_free(gSeqPathKeyCounts[scene]);
        heap_free(gSeqChannelKeyCounts[scene]);
        heap_free(gSeqTrackEventCounts[scene]);
        heap_free(gSeqTextLists[scene]);
        gSeqPathKeyCounts[scene] = NULL;
        gSeqChannelKeyCounts[scene] = NULL;
        gSeqTrackEventCounts[scene] = NULL;
        gSeqTextLists[scene] = NULL;
        heap_free(gSeqEventQueue);
        gSeqEventQueue = NULL;
        heap_free(gCutsceneFiles[scene]);
        gCutsceneFiles[scene] = NULL;
    }
}

/* For each object bound to a track: wrap its eyeCode counters at 10 and call face_revertExpression. */
void cutscene_resetBoundFaces(s32 scene) {
    s32 i;
    Actor *obj;

    for (i = 0; i < gSeqTrackCounts[scene]; i++) {
        if (gSeqTrackInfos[scene][i].type == 2) {
            obj = collision_findObjectById(gSeqTrackInfos[scene][i].targetId);
            if (obj == NULL) {
                continue;
            }
            if (obj->eyeCode[0] >= 10) {
                obj->eyeCode[0] = 0;
                obj->eyeClosed[0] = 0;
            }
            if (obj->eyeCode[1] >= 10) {
                obj->eyeCode[1] = 0;
                obj->eyeClosed[1] = 0;
            }
            face_revertExpression(obj);
        }
    }
}

/* End the scene's cutscene: report it (effect_broadcastEvent), flag it played, release its data and restore
   the camera. If another cutscene is chained (gNextCutscene), start it instead. */
void cutscene_finish(s32 skipped, s32 scene) {
    Camera *player;

    player = camera_global_Cameras;
    if (gCutsceneStates[scene] != 0) {
        {
            u8 cutscene[1];
            s32 reason;

            cutscene[0] = gCutsceneIds[scene];
            reason = skipped ? 0xF : 0x10;
            effect_broadcastEvent((s32)cutscene, reason);
        }
        gCutscenePrevTimes[scene] = -100;
        gCutsceneTimes[scene] = gCutsceneStartTime;
        cutscene_setPlayed(level, gCutsceneIds[scene]);
        gCutsceneStates[scene] = 0;
        D_800C3638 = 0;
        cutscene_resetBoundFaces(scene);
        cutscene_closeSubtitles();
        gCutsceneHideWorld = 0;
        D_800C3683 = 0;
        cutscene_releasePlayer(scene);
        cutscene_restoreControl(scene);
        cutscene_freeScene(scene);
        gViewFovOffset = 0.0f;
        gNearPlaneOverride = 0.0f;
        gFarPlaneOverride = 0.0f;
        view_recordFovChange(0, player->FOVX, player->FOVY, 1.0f);
        if (gNextCutscene != 0) {
            s32 savedMap;
            s32 map;

            gCutsceneStates[scene] = 2;
            map = gNextCutsceneMap;
            if (0x25 == gNextCutsceneMap) {
                savedMap = level;
                level = 0x25;
                map = 0x25;
            }
            D_800C3671 = 0;
            cutscene_start(map, gNextCutscene - 1, scene, gCutsceneCallback, gCutsceneCallbackArg);
            if (0x25 == gNextCutsceneMap) {
                level = savedMap;
            }
            return;
        }
        if (D_800C3C8B != 0) {
            screen_startViewFade(0, 0, 0, 0x1E, 0, 0);
        }
        D_8008FD84[0] = 0;
        D_800C3670 = 0;
        D_800C3671 = 0;
    }
}


/* Hand control back after the scene's cutscene: restore the player camera and object state. */
void cutscene_restoreControl(s32 scene) {
    Camera *player;
    Actor *obj;
    s32 i;

    player = camera_global_Cameras;
    gCamShot.mode = 1;
    gCamShot.unk4 = scene;
    gCamShot.eye.x = player->eye.x;
    gCamShot.eye.y = player->eye.y;
    gCamShot.eye.z = player->eye.z;
    gCamShot.at.angles.sway = 0.0f;
    gCamShot.at.angles.pitch = player->CameraPitch;
    gCamShot.at.angles.yaw = player->CameraAngle;
    if (D_800C3671 == 0) {
        camera_queueMessage(player, 7, &gCamShot);
        if (gCutsceneCutbackAngle != 999.0f) {
            gCamShot.yaw = gCutsceneCutbackAngle;
            camera_queueMessage(player, 8, &gCamShot);
        }
        if (gCutsceneEndTransitionYaw != 999.0f) {
            gCamShot.yaw = gCutsceneEndTransitionYaw;
            gCamShot.duration = gCutsceneEndTransitionTime;
            camera_queueMessage(player, 10, &gCamShot);
        }
    }
    if (gNextCutscene == 0 && D_800C3671 == 0) {
        if (D_800C3680 == 0) {
            camera_queueMessage(player, 6, (void *)scene);
        } else {
            D_800C3680 = 0;
        }
    }
    if (gCutsceneKeepSpawnedActors[scene] == 0) {
        cutscene_removeSpawnedActors(scene);
    }
    cutscene_restoreInactive(scene);
    for (i = 0; i != 25; i++) {
        if (movement_obj[i].type != 0 && cutscene_isActorBound(&movement_obj[i]) != 0) {
            movement_obj[i].fogMode = 0;
            movement_obj[i].vtilt = 0.0f;
            movement_obj[i].lean_angle = 0.0f;
        }
    }
    if (gNextCutscene == 0) {
        D_800C3A58 = 2;
        gCutsceneAnchorSet = 0;
        gCutsceneAnchorHeading = gNextCutsceneAnchorHeading;
        gCutsceneAnchorPos[0] = gNextCutsceneAnchorPos[0];
        gCutsceneAnchorPos[1] = gNextCutsceneAnchorPos[1];
        gCutsceneAnchorPos[2] = gNextCutsceneAnchorPos[2];
    }
}

/* Evaluate every track of the scene at the current time gCutsceneTimes: start from the track's path
   start and type defaults, then interpolate each of its 15 channels between the keys around the
   current time (Catmull-Rom via math_catmullRom, or linear) and store them in its SeqTrackState.
   Channel 10 is the position along the path in percent. */
void cutscene_evalTracks(s32 scene) {
    s32 prevTime[15];
    s32 nextTime[15];
    s32 nextKey[15];
    s32 prevInterp[15];
    f32 firstValue[15];
    f32 lastValue[15];
    s32 numKeys[15];
    f32 value[15];
    s32 j;
    s32 k;
    s32 track;
    f32 t;
    s32 mask;
    s32 found;
    s32 n;
    f32 points[4];
    s32 start;
    s32 needed;
    s32 m;
    s32 channel;
    s32 q;
    s32 next;
    SeqChannelKey *key;

    // matching: indexing through &gCutsceneStates[0] rather than the array changes register allocation
    if ((&gCutsceneStates[0])[scene] == 1) {
        D_800C3638 = 0;
        for (track = 0; track < gSeqTrackCounts[scene]; track++) {
            if (gSeqPathKeyCounts[scene][track] != 0) {
                gSeqTrackStates[scene][track].x = gSeqPathKeys[scene][track]->x;
                gSeqTrackStates[scene][track].y = gSeqPathKeys[scene][track]->y;
                gSeqTrackStates[scene][track].z = gSeqPathKeys[scene][track]->z;
            } else {
                gSeqTrackStates[scene][track].x = 0.0f;
                gSeqTrackStates[scene][track].y = 0.0f;
                gSeqTrackStates[scene][track].z = 0.0f;
            }
            if (gSeqTrackInfos[scene][track].type == 4) {
                gSeqTrackStates[scene][track].unkC = 64.0f;
                gSeqTrackStates[scene][track].unk10 = 0.0f;
                gSeqTrackStates[scene][track].unk14 = 20.0f;
                gSeqTrackStates[scene][track].unk18 = 255.0f;
                gSeqTrackStates[scene][track].unk1C = 62.5f;
                gSeqTrackStates[scene][track].unk20 = 50.0f;
                gSeqTrackStates[scene][track].unk24 = 0.0f;
                gSeqTrackStates[scene][track].unk28 = 0.0f;
                gSeqTrackStates[scene][track].unk2C = 0.0f;
                gSeqTrackStates[scene][track].unk34 = 0.0f;
            } else {
                gSeqTrackStates[scene][track].unkC = 0.0f;
                gSeqTrackStates[scene][track].unk10 = gCutsceneAnchorTurn;
                gSeqTrackStates[scene][track].unk14 = 0.0f;
                gSeqTrackStates[scene][track].unk18 = -9999.0f;
                gSeqTrackStates[scene][track].unk1C = -9999.0f;
                gSeqTrackStates[scene][track].unk20 = -9999.0f;
                gSeqTrackStates[scene][track].unk24 = 0.0f;
                gSeqTrackStates[scene][track].unk28 = 0.0f;
                gSeqTrackStates[scene][track].unk2C = 0.0f;
                gSeqTrackStates[scene][track].unk3C = -9999.0f;
                gSeqTrackStates[scene][track].unk40 = -9999.0f;
                gSeqTrackStates[scene][track].unk30 = -9999.0f;
            }
            gSeqTrackStates[scene][track].yOffset = 0.0f;
            if (gSeqTrackInfos[scene][track].type == 1) {
                gSeqTrackStates[scene][track].unk34 = -9999.0f;
            } else if (gSeqTrackInfos[scene][track].type == 2) {
                gSeqTrackStates[scene][track].unk34 = 255.0f;
            } else if (gSeqTrackInfos[scene][track].type == 5) {
                gSeqTrackStates[scene][track].unk18 = 32.0f;
                gSeqTrackStates[scene][track].unk1C = 32.0f;
                gSeqTrackStates[scene][track].unk34 = 1.0f;
            } else if (gSeqTrackInfos[scene][track].type == 3) {
                gSeqTrackStates[scene][track].unk18 = -9999.0f;
                gSeqTrackStates[scene][track].unk1C = -9999.0f;
                gSeqTrackStates[scene][track].unk20 = -9999.0f;
                gSeqTrackStates[scene][track].unk34 = 255.0f;
            }
            if (gSeqChannelKeys[scene][track] != NULL) {
                for (j = 0; j < 15; j++) {
                    nextTime[j] = -9999;
                    prevTime[j] = -9999;
                    nextKey[j] = -9999;
                    prevInterp[j] = 1;
                    numKeys[j] = 0;
                    firstValue[j] = -9999.0f;
                    lastValue[j] = -9999.0f;
                }
                for (j = 0; j < gSeqChannelKeyCounts[scene][track]; j++) {
                    key = &gSeqChannelKeys[scene][track][j];
                    channel = key->channel;
                    if (nextKey[channel] == -9999) {
                        if (key->time >= gCutsceneTimes[scene]) {
                            nextKey[channel] = numKeys[channel];
                            nextTime[channel] = key->time;
                        } else {
                            prevInterp[channel] = key->interp;
                            prevTime[channel] = key->time;
                        }
                    }
                    lastValue[channel] = key->value;
                    if (firstValue[channel] == -9999.0f) {
                        firstValue[channel] = key->value;
                    }
                    numKeys[channel]++;
                }
                for (j = 0; j < 15; j++) {
                    value[j] = -9999.0f;
                    if (prevTime[j] == -9999 && nextTime[j] == -9999) {
                        continue;
                    }
                    if (prevTime[j] == -9999) {
                        value[j] = firstValue[j];
                    } else if (nextTime[j] == -9999) {
                        value[j] = lastValue[j];
                    } else {
                        t = (f32)(gCutsceneTimes[scene] - prevTime[j]) / (nextTime[j] - prevTime[j]);
                        mask = 6;
                        found = -1;
                        if (prevInterp[j] == 0) {
                            s32 skip;
                            if (nextKey[j] - 2 >= 0) {
                                mask = 7;
                                start = nextKey[j] - 2;
                                needed = 3;
                                q = 0;
                            } else {
                                start = nextKey[j] - 1;
                                needed = 2;
                                q = 1;
                            }
                            if (nextKey[j] + 1 < numKeys[j]) {
                                mask |= 8;
                                needed++;
                            }
                            skip = 0;
                            for (m = 0; m < gSeqChannelKeyCounts[scene][track]; m++) {
                                if (needed == 0) {
                                    break;
                                }
                                key = &gSeqChannelKeys[scene][track][m];
                                if (j == key->channel) {
                                    if (skip == start) {
                                        points[q++] = key->value;
                                        needed--;
                                    } else {
                                        skip++;
                                    }
                                }
                            }
                            if (!(mask & 1)) {
                                points[0] = points[1];
                            }
                            if (!(mask & 8)) {
                                points[3] = points[2];
                            }
                            value[j] = math_catmullRom(points, 0, t);
                        } else {
                            n = 0;
                            for (k = 0; k < gSeqChannelKeyCounts[scene][track]; k++) {
                                key = &gSeqChannelKeys[scene][track][k];
                                if (j == key->channel) {
                                    if (found == -1) {
                                        if (n == nextKey[j] - 1) {
                                            found = k;
                                        } else {
                                            n++;
                                        }
                                    } else {
                                        next = k;
                                        break;
                                    }
                                }
                            }
                            value[j] = gSeqChannelKeys[scene][track][next].value - gSeqChannelKeys[scene][track][found].value;
                            value[j] *= t;
                            value[j] += gSeqChannelKeys[scene][track][found].value;
                        }
                    }
                }
                if (value[10] == -9999.0f) {
                    value[10] = 0.0f;
                }
                if (gSeqActiveEvents[scene][track][0] != 0xFF) {
                    cutscene_getTrackHeading(track, value[10], &gSeqTrackStates[scene][track].unk10, scene);
                } else if (value[7] != -9999.0f) {
                    gSeqTrackStates[scene][track].unk10 = value[7];
                }
                if (value[8] != -9999.0f) {
                    gSeqTrackStates[scene][track].unkC = value[8];
                }
                if (value[6] != -9999.0f) {
                    gSeqTrackStates[scene][track].unk14 = value[6];
                }
                if (value[5] != -9999.0f) {
                    gSeqTrackStates[scene][track].unk18 = value[5];
                }
                if (value[4] != -9999.0f) {
                    gSeqTrackStates[scene][track].unk1C = value[4];
                }
                if (value[3] != -9999.0f) {
                    gSeqTrackStates[scene][track].unk20 = value[3];
                }
                if (value[1] != -9999.0f) {
                    gSeqTrackStates[scene][track].unk24 = value[1];
                }
                if (value[2] != -9999.0f) {
                    gSeqTrackStates[scene][track].unk28 = value[2];
                }
                if (value[0] != -9999.0f) {
                    gSeqTrackStates[scene][track].unk2C = value[0];
                }
                if (value[14] != -9999.0f) {
                    gSeqTrackStates[scene][track].unk30 = value[14];
                }
                if (value[9] != -9999.0f) {
                    gSeqTrackStates[scene][track].unk34 = value[9];
                }
                if (value[11] != -9999.0f) {
                    gSeqTrackStates[scene][track].yOffset = value[11];
                }
                if (value[13] != -9999.0f) {
                    gSeqTrackStates[scene][track].unk3C = value[13];
                }
                if (value[12] != -9999.0f) {
                    gSeqTrackStates[scene][track].unk40 = value[12];
                }
                cutscene_getTrackPos(track, value[10], &gSeqTrackStates[scene][track].x,
                              &gSeqTrackStates[scene][track].y,
                              &gSeqTrackStates[scene][track].z, scene);
            }
        }
        D_800C3638 = 1;
    }
}

/* Find the path segment containing distance `dist` along track `track` and write two points to
   `out`: the ends of the arc-length sample on a curved segment, or the interpolated point twice on
   a linear one. Returns the arc-length sample index. */
s32 cutscene_findPathSegment(s32 track, f32 dist, f32 *out, s32 scene, s32 *segmentOut) {
    s32 key;
    s32 lo;
    s32 hi;
    s32 sample;
    Vec3f points[4];
    f32 *distances;
    s32 i;
    u8 j;
    s32 linear;
    f32 t;

    lo = 0;
    hi = gSeqPathKeyCounts[scene][track] - 1;
    if (hi >= 0) {
        distances = gSeqTrackPaths[scene][track].keyDistance;
        do {
            key = (lo + hi) / 2;
            if (dist < distances[key]) {
                hi = key - 1;
            } else {
                lo = key + 1;
            }
            if (distances[key] <= dist && dist <= distances[key + 1]) {
                break;
            }
        } while (hi >= lo);
    }
    *segmentOut = key;
    if (key != gSeqTrackPaths[scene][track].segment) {
        cutscene_cacheSegment(track, key, scene);
    }
    linear = gSeqPathKeys[scene][track][key].linear;
    cutscene_getSegmentKeys((f32 *)points, gSeqPathKeys[scene][track], key, gSeqPathKeyCounts[scene][track]);
    if (linear == 0) {
        hi = 100;
        lo = 0;
        distances = gSeqTrackPaths[scene][track].arcLength;
        do {
            sample = (lo + hi) / 2;
            if (dist < distances[sample]) {
                hi = sample - 1;
            } else {
                lo = sample + 1;
            }
            if (distances[sample] <= dist && dist <= distances[sample + 1]) {
                break;
            }
        } while (hi >= lo);
        j = 0;
        for (i = 0; i < 2; i++) {
            t = (sample + i) * 0.01f;
            ((Vec3f *)out)[j].x = ((gSeqTrackPaths[scene][track].coef3[0] * t + gSeqTrackPaths[scene][track].coef2[0]) * t + gSeqTrackPaths[scene][track].coef1[0]) * t + gSeqTrackPaths[scene][track].coef0[0];
            ((Vec3f *)out)[j].y = ((gSeqTrackPaths[scene][track].coef3[1] * t + gSeqTrackPaths[scene][track].coef2[1]) * t + gSeqTrackPaths[scene][track].coef1[1]) * t + gSeqTrackPaths[scene][track].coef0[1];
            ((Vec3f *)out)[j].z = ((gSeqTrackPaths[scene][track].coef3[2] * t + gSeqTrackPaths[scene][track].coef2[2]) * t + gSeqTrackPaths[scene][track].coef1[2]) * t + gSeqTrackPaths[scene][track].coef0[2];
            j ^= 1;
        }
    } else {
        t = gSeqTrackPaths[scene][track].keyDistance[key + 1] - gSeqTrackPaths[scene][track].keyDistance[key];
        dist = dist - gSeqTrackPaths[scene][track].keyDistance[key];
        if (t != 0) {
            dist = dist / t;
        } else {
            dist = 0.0f;
        }
        j = 0;
        for (i = 0; i < 2; i++) {
            ((Vec3f *)out)[j].x = (points[2].x - points[1].x) * dist + points[1].x;
            ((Vec3f *)out)[j].y = (points[2].y - points[1].y) * dist + points[1].y;
            ((Vec3f *)out)[j].z = (points[2].z - points[1].z) * dist + points[1].z;
            j ^= 1;
        }
    }
    return sample;
}

/* Position of track `track` at `percent` (0-100) of its path length. */
void cutscene_getTrackPos(s32 track, f32 percent, f32 *x, f32 *y, f32 *z, s32 scene) {
    f32 dist;
    s32 segment;
    s32 sample;
    f32 positions[6];
    f32 *arc;
    f32 covered;
    f32 ratio;

    if (gSeqPathKeyCounts[scene][track] >= 2) {
        if (percent > 100.0f) {
            percent = 100.0f;
        }
        if (percent < 0.0f) {
            percent = 0.0f;
        }
        dist = gSeqTrackPaths[scene][track].length * percent * 0.009999999776f;
        sample = cutscene_findPathSegment(track, dist, positions, scene, &segment);
        if (gSeqPathKeys[scene][track][segment].linear == 0) {
            arc = gSeqTrackPaths[scene][track].arcLength + sample;
            percent = arc[1] - arc[0];
            covered = dist - arc[0];
            if (percent != 0) {
                ratio = covered / percent;
            } else {
                ratio = 0.0f;
            }
            *x = positions[0] + (positions[3] - positions[0]) * ratio;
            *y = positions[1] + (positions[4] - positions[1]) * ratio;
            *z = positions[2] + (positions[5] - positions[2]) * ratio;
            return;
        }
        *x = positions[0];
        *y = positions[1];
        *z = positions[2];
    }
}

/* Heading in degrees of track `track`'s path at `percent` (0-100) of its length. */
void cutscene_getTrackHeading(s32 track, f32 percent, f32 *heading, s32 scene) {
    f32 dx;
    f32 positions[6];
    s32 segment[3];
    f32 points[10];
    f32 dz;
    register f32 dist;

    if (gSeqPathKeyCounts[scene][track] >= 2) {
        if (percent > 100.0f) {
            percent = 100.0f;
        }
        if (percent < 0.0f) {
            percent = 0.0f;
        }

        dist = gSeqTrackPaths[scene][track].length * percent * 0.009999999776f;
        cutscene_findPathSegment(track, dist, positions, scene, segment);

        if (gSeqPathKeys[scene][track][segment[0]].linear == 0) {
            dx = positions[0] - positions[3];
            dz = positions[2] - positions[5];
        } else {
            // matching: the 12-float buffer starts 2 floats below a 10-float local (frame size)
            cutscene_getSegmentKeys(points - 2, gSeqPathKeys[scene][track], segment[0], gSeqPathKeyCounts[scene][track]);
            dx = points[4] - points[1];
            dz = points[6] - points[3];
        }
        *heading = math_atan2(dx, dz) * 57.29577637f;
    }
}

/* Copy the path keys around segment `key` (key-1 .. key+2) into out[0..3] as float points. A
   missing key before or after the segment is replaced by a copy of the nearest end point. */
void cutscene_getSegmentKeys(f32 *out, SeqPathKey *keys, s32 key, s32 numKeys) {
    s32 flags;
    s32 start;
    s32 count;
    s32 outIdx;
    s32 i;

    flags = 6;
    if ((key - 1) >= 0) {
        flags = 7;
        start = key - 1;
        count = 3;
        outIdx = 0;
    } else {
        start = key;
        count = 2;
        outIdx = 1;
    }

    if ((key + 2) < numKeys) {
        flags |= 8;
        count++;
    }

    for (i = start; i < start + count; outIdx++, i++) {
        ((Vec3f *)out)[outIdx].x = keys[i].x;
        ((Vec3f *)out)[outIdx].y = keys[i].y;
        ((Vec3f *)out)[outIdx].z = keys[i].z;
    }

    if (!(flags & 1)) {
        out[0] = out[3];
        out[1] = out[4];
        out[2] = out[5];
    }
    if (!(flags & 8)) {
        out[9] = out[6];
        out[10] = out[7];
        out[11] = out[8];
    }
}

/* Measure track `track`'s path: the distance at each key and the total length. */
void cutscene_measurePath(s32 track, s32 scene) {
    s32 i;
    s32 j;
    f32 pts[4][3];
    f32 buf[2][3];
    f32 z;
    f32 d[3];
    f32 c[3];
    f32 b[3];
    f32 a[3];
    f32 len;
    f32 t;
    f32 x;
    f32 y;
    f32 dist;
    u8 cur;
    u16 linear;

    if (gSeqPathKeyCounts[scene][track] >= 2) {
        gSeqTrackPaths[scene][track].segment = 0xFF;
        len = 0.0f;
        cur = 0;
        for (i = 0; i < gSeqPathKeyCounts[scene][track] - 1; i++) {
            linear = gSeqPathKeys[scene][track][i].linear;
            cutscene_getSegmentKeys(&pts[0][0], gSeqPathKeys[scene][track], i, gSeqPathKeyCounts[scene][track]);
            gSeqTrackPaths[scene][track].keyDistance[i] = len;
            if (linear == 0) {
                for (j = 0; j < 3; j++) {
                    a[j] = -0.5f * pts[0][j] + 1.5f * pts[1][j] + -1.5f * pts[2][j] + 0.5f * pts[3][j];
                    b[j] = pts[0][j] + -2.5f * pts[1][j] + 2.0f * pts[2][j] + -0.5f * pts[3][j];
                    c[j] = -0.5f * pts[0][j] + 0.5f * pts[2][j];
                    d[j] = pts[1][j];
                }
                for (j = 0; j < 3; j++) {
                    buf[!cur][j] = d[j];
                }
                for (t = 0.01f; t <= 1.0f; t += 0.01f) {
                    x = ((a[0] * t + b[0]) * t + c[0]) * t + d[0];
                    buf[cur][0] = x;
                    buf[cur][1] = ((a[1] * t + b[1]) * t + c[1]) * t + d[1];
                    buf[cur][2] = ((a[2] * t + b[2]) * t + c[2]) * t + d[2];
                    dist = x - buf[!cur][0];
                    z = buf[cur][1] - buf[!cur][1];
                    // matching: forces a reload of the stored z instead of reusing the register
                    x = *(volatile f32 *)&buf[cur][2] - buf[!cur][2];
                    dist = dist * dist;
                    z = z * z;
                    x = x * x;
                    dist += z + x;
                    if (dist != 0.0f) {
                        len += sqrtf(dist);
                    }
                    cur ^= 1;
                }
            } else {
                dist = pts[2][2] - pts[1][2];
                t = pts[2][0] - pts[1][0];
                y = pts[2][1] - pts[1][1];
                len += sqrtf(dist * dist + (t * t + y * y));
            }
        }
        gSeqTrackPaths[scene][track].keyDistance[i] = len;
        gSeqTrackPaths[scene][track].length = len;
        gSeqTrackPaths[scene][track].segment = 0xFF;
    }
}


/* Cache the cubic and the 101-sample arc-length table of segment `segment` of track `track`. */
void cutscene_cacheSegment(s32 track, s32 segment, s32 scene) {
    s32 i;
    s32 k;
    f32 points[12];
    f32 pos[2][3];
    u8 j;
    f32 d[3];
    f32 c[3];
    f32 b[3];
    f32 a[3];
    f32 total;
    f32 t;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 linear;

    if (segment >= gSeqPathKeyCounts[scene][track] - 1) {
        return;
    }
    j = 0;
    total = gSeqTrackPaths[scene][track].keyDistance[segment];
    linear = gSeqPathKeys[scene][track][segment].linear;
    cutscene_getSegmentKeys(points, gSeqPathKeys[scene][track], segment, gSeqPathKeyCounts[scene][track]);
    if (linear == 0) {
        for (i = 0; i < 3; i++) {
            a[i] = -0.5f * points[i] + 1.5f * points[i + 3] + -1.5f * points[i + 6] + points[i + 9] * 0.5f;
            b[i] = points[i] + -2.5f * points[i + 3] + (points[i + 6] + points[i + 6]) + points[i + 9] * -0.5f;
            c[i] = -0.5f * points[i] + points[i + 6] * 0.5f;
            d[i] = points[i + 3];
            gSeqTrackPaths[scene][track].coef0[i] = d[i];
            gSeqTrackPaths[scene][track].coef1[i] = c[i];
            gSeqTrackPaths[scene][track].coef2[i] = b[i];
            gSeqTrackPaths[scene][track].coef3[i] = a[i];
        }
        gSeqTrackPaths[scene][track].arcLength[0] = total;
        for (i = 0; i != 3; i++) {
            pos[!j][i] = d[i];
        }
        i = 1;
        t = 0.009999999776f;
        while (t <= 1.0f) {
            pos[j][0] = ((a[0] * t + b[0]) * t + c[0]) * t + d[0];
            pos[j][1] = ((a[1] * t + b[1]) * t + c[1]) * t + d[1];
            pos[j][2] = ((a[2] * t + b[2]) * t + c[2]) * t + d[2];
            dx = pos[j][0] - pos[!j][0];
            dx *= dx;
            dy = pos[j][1] - pos[!j][1];
            dy *= dy;
            t += 0.009999999776f;
            dz = pos[j][2] - pos[!j][2];
            dz *= dz;
            dx += dy + dz;
            if (dx != 0.0f) {
                total += sqrtf(dx);
            }
            gSeqTrackPaths[scene][track].arcLength[i] = total;
            i++;
            j ^= 1;
        }
    } else {
        total = gSeqTrackPaths[scene][track].keyDistance[segment + 1] - total;
        for (k = 0; k < 101; k++) {
            gSeqTrackPaths[scene][track].arcLength[k] = k * total / 101.0f;
        }
    }
    gSeqTrackPaths[scene][track].segment = segment;
}

// matching: byte-offset form of &gSeqTrackInfos[a][i]; plain indexing swaps two s-registers here
#define ENT(a, i) ((SeqTrackInfo *)((u8 *)gSeqTrackInfos[a] + (i) * 8))
#define ELEM(a, i) (&gSeqTrackStates[a][i])

/* Apply the scene's evaluated tracks: the camera track (type 1, the one chosen by cutscene_pickCameraTrack)
   drives the player camera through camera_queueMessage message 7, type 3 tracks drive actors, type 2
   tracks objects (cutscene_applyObjectTrack) and type 5 tracks sprite_queuePlayerBillboard. Non-camera tracks go first. */
void cutscene_applyTracks(s32 scene) {
    s32 track;
    s32 pass;
    Camera *player;
    SeqTrackState *state;
    WorldObject *actor;
    f32 tilt3C;
    f32 tilt40;

    // matching: indexing through &gCutsceneStates[0] rather than the array changes register allocation
    if ((&gCutsceneStates[0])[scene] == 1) {
        D_800C3638 = 0;
        if (gSeqCameraTrack == 0xFF) {
            gSeqCameraTrack = cutscene_pickCameraTrack(scene, gCutsceneTimes[scene]);
        }
        for (pass = 0; pass != 2; pass++) {
            for (track = 0; track < gSeqTrackCounts[scene]; track++) {
                if (gSeqChannelKeyCounts[scene][track] == 0) {
                    continue;
                }
                if (gSeqPathKeyCounts[scene][track] == 0) {
                    continue;
                }
                if (pass == 0) {
                    if (ENT(scene, track)->type == 1) {
                        continue;
                    }
                } else if (ENT(scene, track)->type != 1) {
                    continue;
                }
                if (ENT(scene, track)->type == 1) {
                    player = camera_global_Cameras;
                    if (track != gSeqCameraTrack) {
                        continue;
                    }
                    if (!(player->Mode & 0x40000)) {
                        if (D_800C3671 == 0) {
                            camera_queueMessage(player, 5, (void *)scene);
                        }
                        camera_processMessages(player);
                    }
                    state = ELEM(scene, track);
                    if (state->unk34 == -9999.0f) {
                        gCamShot.unk8 = 0.0f;
                    } else {
                        state->unk34 = (state->unk34 < -20.0f) ? -20.0f : (state->unk34 > 40.0f) ? 40.0f : state->unk34;
                        state = ELEM(scene, track);
                        gCamShot.unk8 = state->unk34;
                    }
                    if (state->unk3C == -9999.0f) {
                        tilt3C = 0.0f;
                    } else {
                        state->unk3C = (state->unk3C < -20.0f) ? -20.0f : (state->unk3C > 40.0f) ? 40.0f : state->unk3C;
                        state = ELEM(scene, track);
                        tilt3C = state->unk3C;
                    }
                    if (state->unk40 == -9999.0f) {
                        tilt40 = 0.0f;
                        while (0) { } // matching: orders the scene store after the ENT load
                        gCamShot.unk4 = scene;
                    } else {
                        state->unk40 = (state->unk40 < -20.0f) ? -20.0f : (state->unk40 > 40.0f) ? 40.0f : state->unk40;
                        if (ENT(scene, track)) { } // matching: same, in this branch
                        state = ELEM(scene, track);
                        tilt40 = state->unk40;
                        gCamShot.unk4 = scene;
                    }
                    gCamShot.eye.x = state->x;
                    gCamShot.eye.y = state->y;
                    gCamShot.eye.z = state->z;
                    if (ENT(scene, track)->targetId == 1) {
                        gCamShot.mode = 0;
                        gCamShot.at.focus.x = ELEM(scene, (s8)ENT(scene, track)->unk3)->x;
                        gCamShot.at.focus.y = ELEM(scene, (s8)ENT(scene, track)->unk3)->y + ENT(scene, track)->yOffset + (state = ELEM(scene, track))->yOffset;
                        gCamShot.at.focus.z = ELEM(scene, (s8)ENT(scene, track)->unk3)->z;
                        gCamShot.unk2C = -state->unk14;
                        gCamShot.unk30 = tilt3C;
                        gCamShot.unk34 = tilt40;
                    } else if (ENT(scene, track)->targetId == 2) {
                        Actor *obj;

                        obj = collision_findObjectById(ENT(scene, track)->unk3);
                        if (obj == NULL) {
                            continue;
                        }
                        gCamShot.mode = 0;
                        gCamShot.at.focus.x = obj->xpos;
                        gCamShot.at.focus.y = obj->ypos + ENT(scene, track)->yOffset + (state = ELEM(scene, track))->yOffset;
                        gCamShot.at.focus.z = obj->zpos;
                        gCamShot.unk2C = -state->unk14;
                        gCamShot.unk30 = tilt3C;
                        gCamShot.unk34 = tilt40;
                    } else if (ENT(scene, track)->targetId == 3) {
                        WorldObject *actor3;

                        actor3 = gfx_findObjectById(ENT(scene, track)->unk3);
                        if (actor3 == NULL) {
                            continue;
                        }
                        gCamShot.mode = 0;
                        gCamShot.at.focus.x = actor3->x;
                        gCamShot.at.focus.y = (actor3->y + ENT(scene, track)->yOffset) + (state = ELEM(scene, track))->yOffset;
                        gCamShot.at.focus.z = actor3->z;
                        gCamShot.unk2C = -state->unk14;
                        gCamShot.unk30 = tilt3C;
                        gCamShot.unk34 = tilt40;
                    } else if (ENT(scene, track)->targetId == 0) {
                        gCamShot.mode = 1;
                        gCamShot.at.angles.sway = -state->unk14;
                        gCamShot.at.angles.pitch = state->unkC;
                        gCamShot.at.angles.yaw = state->unk10;
                        gCamShot.eye.y += state->yOffset;
                        gCamShot.unk2C = tilt3C;
                        gCamShot.unk30 = tilt40;
                    }
                    if (D_800C3671 == 0) {
                        camera_queueMessage(player, 7, &gCamShot);
                    }
                } else if (ENT(scene, track)->type == 3) {
                    actor = gfx_findObjectById(ENT(scene, track)->targetId);
                    if (actor == NULL) {
                        continue;
                    }
                    actor->attach = 0;
                    actor->viewFlags &= ~0x80;
                    actor->viewFlags = actor->viewFlags; // matching: forces the byte reload
                    actor->x = ELEM(scene, track)->x;
                    actor->y = ELEM(scene, track)->y + ELEM(scene, track)->yOffset;
                    actor->z = ELEM(scene, track)->z;
                    actor->t = ELEM(scene, track)->unkC;
                    actor->u = ELEM(scene, track)->unk10;
                    actor->v = ELEM(scene, track)->unk14;
                    if (ELEM(scene, track)->unk18 != -9999.0f) {
                        actor->xScale = ELEM(scene, track)->unk18 * 0.01f;
                    }
                    if (ELEM(scene, track)->unk1C != -9999.0f) {
                        actor->yScale = ELEM(scene, track)->unk1C * 0.01f;
                    }
                    if (ELEM(scene, track)->unk20 != -9999.0f) {
                        actor->zScale = ELEM(scene, track)->unk20 * 0.01f;
                    }
                    if ((s32)ELEM(scene, track)->unk34 >= 0x100) {
                        actor->alpha = 0xFF;
                    } else if ((s32)ELEM(scene, track)->unk34 < 0) {
                        actor->alpha = 0;
                    } else {
                        actor->alpha = (s32)ELEM(scene, track)->unk34;
                    }
                } else if (ENT(scene, track)->type == 2) {
                    Actor *obj2;

                    obj2 = collision_findObjectById(ENT(scene, track)->targetId);
                    cutscene_applyObjectTrack(obj2, scene, track);
                } else if (ENT(scene, track)->type == 4) {
                    s32 arr[3];

                    arr[0] = ENT(scene, track)->targetId;
                    arr[1] = ENT(scene, track)->targetId;
                    arr[2] = ENT(scene, track)->targetId;
                } else if (ENT(scene, track)->type == 5) {
                    state = ELEM(scene, track);
                    if (state->unk34 >= 1.0f) {
                        sprite_queuePlayerBillboard(state->x, state->y, state->z, 0, state->unk18, state->unk1C);
                    }
                }
            }
        }
        D_800C3638 = 1;
    }
}


// matching: ((u8 *)&D_800D35E0)[-4] is gCurrentActorSlot; a distinct base keeps IDO from reusing the hoisted address

/* Move object `target` to the state of the scene's object track bound to it (all tracks, or only
   `onlyTrack` unless it is -1): position, velocity, floor probe, scale, facing and alpha. */
void cutscene_applyObjectTrack(Actor *target, s32 scene, s32 onlyTrack) {
    s32 track;
    f32 floorY;
    Actor *obj;
    s32 end;
    SeqTrackInfo *info;
    f32 angle;
    f32 oldPos[3];
    s32 start;
    s32 rescaled;

    if (gCutsceneStates[scene] == 1) {
        D_800C3638 = 0;
        if (onlyTrack != -1) {
            start = onlyTrack;
            end = onlyTrack + 1;
        } else {
            start = 0;
            end = gSeqTrackCounts[scene];
        }
        for (track = start; track < end; track++) {
            if (gSeqPathKeyCounts[scene][track] == 0) {
                continue;
            }
            info = &gSeqTrackInfos[scene][track];
            if (info->type == 2) {
                obj = collision_findObjectById(info->targetId);
                if (obj != NULL && obj == target) {
                    oldPos[0] = obj->xpos;
                    oldPos[1] = obj->ypos;
                    oldPos[2] = obj->zpos;
                    obj->xpos = gSeqTrackStates[scene][track].x;
                    obj->ypos = gSeqTrackStates[scene][track].y + gSeqTrackStates[scene][track].yOffset;
                    obj->zpos = gSeqTrackStates[scene][track].z;
                    obj->unk1CC = obj->ypos;
                    if (gCutsceneSkipState == 0) {
                        obj->speed = (obj->xpos - oldPos[0]) * (obj->xpos - oldPos[0]) + (obj->zpos - oldPos[2]) * (obj->zpos - oldPos[2]);
                        if (obj->speed) {
                            obj->speed = sqrtf(obj->speed) * 2.0f;
                        }
                    } else {
                        obj->speed = 0.0f;
                    }
                    if (obj->mv != NULL) {
                        obj->mv->actual_speed = obj->speed * 2.0f;
                    }
                    obj->unk54 = 0.0f;
                    obj->unkCC = 0;
                    rescaled = 0;
                    gCurrentActorSlot = (target - movement_obj) + 1;
                    collision_findFloorInRange(&obj->floorTriVerts, obj->floorTri.v, &floorY, &obj->waterHeight, &obj->floorMaterial, &obj->floorType,
                                  obj->xpos, obj->ypos, obj->zpos, obj->ypos, 0, obj,
                                  -10000.0f, target->ypos);
                    ((u8 *)&D_800D35E0)[-4] = 0;
                    obj->floorY = floorY;
                    obj->doing_hit = 0;
                    obj->shad = obj->ypos - obj->floorY;
                    if (obj->type == 1) {
                        if (obj->waterHeight != -10000.0f && obj->ypos < obj->waterHeight) {
                            obj->inwater = 1;
                        } else {
                            obj->inwater = 0;
                            obj->underwaterTime = 0;
                        }
                    }
                    if (gSeqTrackStates[scene][track].unk18 != -9999.0f) {
                        rescaled = 1;
                        obj->scale_xz = gSeqTrackStates[scene][track].unk18 * 0.009999999776f;
                        obj->unk154 = gSeqTrackStates[scene][track].unk18 * 0.009999999776f;
                    }
                    if (gSeqTrackStates[scene][track].unk1C != -9999.0f) {
                        rescaled = 1;
                        obj->scale_y = gSeqTrackStates[scene][track].unk1C * 0.009999999776f;
                        obj->unk158 = gSeqTrackStates[scene][track].unk1C * 0.009999999776f;
                    }
                    if (rescaled) {
                        actor_setScale(obj, obj->scale_xz, obj->scale_y);
                    }
                    obj->vtilt = gSeqTrackStates[scene][track].unkC;
                    angle = gSeqTrackStates[scene][track].unk10 + 180.0f;
                    while (angle >= 360.0f) {
                        angle -= 360.0f;
                    }
                    while (angle < 0.0f) {
                        angle += 360.0f;
                    }
                    obj->angle = obj->langle = obj->fangle = (s16)((u32)(angle * 182.0444489f) - 0x4000);
                    obj->theta = angle;
                    obj->lean_angle = gSeqTrackStates[scene][track].unk14;
                    gSeqTrackStates[scene][track].unk34 = (gSeqTrackStates[scene][track].unk34 < 0.0f) ? 0.0f : (gSeqTrackStates[scene][track].unk34 > 255.0f) ? 255.0f : gSeqTrackStates[scene][track].unk34;
                    actor_fadeCommand(obj, 1, gSeqTrackStates[scene][track].unk34, 0, 0);
                    obj->old_xpos = obj->xpos;
                    obj->old_ypos = obj->ypos;
                    obj->old_zpos = obj->zpos;
                }
            }
        }
        D_800C3638 = 1;
    }
}

/* If object `objIndex` is bound to a track of `scene`, report where it should look: through the
   track's active events in gSeqActiveEvents (slot 2: another track's position, slot 1: this track's
   unk24..unk2C or the player). Returns 1 when the out-angles were written, else 0. */
s32 cutscene_getLookTarget(f32 *outX, f32 *outY, f32 *outZ, f32 *target, s32 objIndex, s32 scene) {
    s32 track;
    u8 *active;
    Camera *player;
    f32 none;

    if (gCutsceneStates[scene] != 1) {
        return 0;
    }
    for (track = 0; track < gSeqTrackCounts[scene]; track++) {
        if (gSeqPathKeyCounts[scene][track] == 0) {
            continue;
        }
        if (gSeqTrackInfos[scene][track].type != 2 ||
            movement_obj[objIndex].unique_id != gSeqTrackInfos[scene][track].targetId) {
            continue;
        }
        active = gSeqActiveEvents[scene][track];
        if (active[2] != 0xFF) {
            SeqEvent *event;
            s32 other;

            event = &gSeqEventTracks[scene][track][active[2]];
            other = event->unk5;
            target[0] = gSeqTrackStates[scene][other].x;
            target[1] = gSeqTrackStates[scene][other].y + event->unk6;
            target[2] = gSeqTrackStates[scene][other].z;
            return 0;
        }
        if (active[1] != 0xFF) {
            SeqEvent *event;

            event = &gSeqEventTracks[scene][track][active[1]];
            if (event->unk5 == 0) {
                *outX = gSeqTrackStates[scene][track].unk28;
                *outY = gSeqTrackStates[scene][track].unk24;
                *outZ = gSeqTrackStates[scene][track].unk2C;
                return 1;
            }
            player = camera_global_Cameras;
            target[0] = player->eye.x;
            target[1] = player->eye.y + event->unk6;
            target[2] = player->eye.z;
            return 0;
        }
        none = -99999.0f;
        target[0] = none;
        target[1] = none;
        target[2] = none;
        return 0;
    }
    return 0;
}

/* If object `objIndex` is bound to a track of the scene, return 1 and its unk40 (outA) and unk3C
   (outB) values, each written twice (0 where unset). */
s32 cutscene_getHeadAngles(f32 *outA, f32 *outB, s32 objIndex, s32 scene) {
    s32 track;

    if (gCutsceneStates[scene] != 1) {
        return 0;
    }
    for (track = 0; track < gSeqTrackCounts[scene]; track++) {
        if (gSeqPathKeyCounts[scene][track] != 0) {
            if (gSeqTrackInfos[scene][track].type == 2 && movement_obj[objIndex].unique_id == gSeqTrackInfos[scene][track].targetId) {
                outA[1] = 0.0f;
                outB[1] = 0.0f;
                outA[0] = 0.0f;
                outB[0] = 0.0f;
                if (gSeqTrackStates[scene][track].unk3C != -9999.0f) {
                    outB[0] = gSeqTrackStates[scene][track].unk3C;
                    outB[1] = outB[0];
                }
                if (gSeqTrackStates[scene][track].unk40 != -9999.0f) {
                    outA[0] = gSeqTrackStates[scene][track].unk40;
                    outA[1] = outA[0];
                }
                return 1;
            }
        }
    }
    return 0;
}

/* Set gCutsceneAnchorPos/gCutsceneAnchorHeading and raise gCutsceneAnchorSet. */
void cutscene_setAnchor(s16 x, s16 y, s16 z, f32 value) {
    gCutsceneAnchorHeading = value;
    gCutsceneAnchorPos[0] = x;
    gCutsceneAnchorPos[1] = y;
    gCutsceneAnchorPos[2] = z;
    gCutsceneAnchorSet = 1;
}


/* Set gNextCutsceneAnchorPos/gNextCutsceneAnchorHeading (copied back to gCutsceneAnchorPos/gCutsceneAnchorHeading when a cutscene ends). */
void cutscene_setNextAnchor(s16 x, s16 y, s16 z, f32 value) {
    gNextCutsceneAnchorHeading = value;
    gNextCutsceneAnchorPos[0] = x;
    gNextCutsceneAnchorPos[1] = y;
    gNextCutsceneAnchorPos[2] = z;
}

void cutscene_resetSpawnedActors(s32 scene) {
    gCutsceneSpawnedActorCounts[scene] = 0;
}

void cutscene_resetActivatedActors(s32 scene) {
    gCutsceneActivatedActorCounts[scene] = 0;
}

/* Add `id` to the scene's gCutsceneSpawnedActors list unless it is already there. */
void cutscene_addSpawnedActor(s32 id, s32 scene) {
    s32 i;
    s32 count;
    s32 limit;
    u8 *countPtr;

    i = 0;
    countPtr = &gCutsceneSpawnedActorCounts[scene];
    count = *countPtr;
    limit = count;
    // matching: a do-while over a copy of the count (the for loop form schedules differently)
    if (count > 0) {
        do {
            if (id == gCutsceneSpawnedActors[scene][i]) {
                return;
            }
            i++;
        } while (i < limit);
    }

    gCutsceneSpawnedActors[scene][count] = id;
    *countPtr = count + 1;
}

/* Reset the animation state of the object bound to the scene's track `track`. */
void cutscene_resetTrackFace(s32 track, s32 scene) {
    Actor *obj;
    SeqTrackInfo *info;

    if (gSeqPathKeyCounts[scene][track] != 0) {
        info = &gSeqTrackInfos[scene][track];
        if (info->type == 2) {
            obj = collision_findObjectById(info->targetId);
            if (obj != NULL) {
                obj->eyeCode[0] = 0;
                obj->eyeCode[1] = 0;
                obj->eyeClosed[0] = 0;
                obj->eyeClosed[1] = 0;
                obj->headEffectTimer = 0;
                obj->headYaw[0] = 0;
                obj->headYaw[1] = 0;
                obj->headPitch[0] = 0;
                obj->headPitch[1] = 0;
                face_requestExpression(obj, 0, 3, 0xFFFF, 0xA);
                obj->expressionPriority = 0;
            }
        }
    }
}

/* Make sure the object bound to the scene's track `track` exists: spawn it from its gSpawnRecords
   slot if needed and add it to the scene's gCutsceneSpawnedActors list. */
void cutscene_spawnTrackActor(s32 track, s32 scene) {
    Actor *obj;
    s32 slot;
    s32 id;

    if (gSeqPathKeyCounts[scene][track] != 0) {
        if (gSeqTrackInfos[scene][track].type == 2) {
            id = gSeqTrackInfos[scene][track].targetId;
            slot = collision_findSpawnById(id);
            if (slot != -1) {
                obj = actor_findOnPath(slot);
                if (obj != NULL) {
                    if (obj->drawtype == 3) {
                        if (gSeqTrackInfos != &gSeqTrackInfos[scene]) {
                            obj->drawtype = 4;
                        } else {
                            obj->drawtype = 0;
                        }
                    }
                    if (id == 1) {
                        obj->mvmt_control &= -0x2B;
                    }
                } else {
                    if (slot < gSpawnRecordCount) {
                        collision_spawnObject((void *)((slot * 0x30) + (s32)gSpawnRecords), slot, 0, scene, 0); // matching: integer address math
                        obj = actor_findOnPath(slot);
                        if (obj != NULL) {
                            actor_playAnim(obj, 0, 0.0f, 0.0f, 0.0f, 0.0f, 1);
                            cutscene_addSpawnedActor(obj - movement_obj, scene);
                        }
                    }
                }
            }
        }
    }
}

/* Apply every object track of the scene to its object (cutscene_applyObjectTrack). */
void cutscene_applyObjectTracks(s32 scene) {
    s32 track;
    Actor *obj;

    for (track = 0; track < gSeqTrackCounts[scene]; track++) {
        if (gSeqChannelKeyCounts[scene][track] == 0) {
            continue;
        }
        if (gSeqPathKeyCounts[scene][track] == 0) {
            continue;
        }
        if (gSeqTrackInfos[scene][track].type == 2) {
            obj = collision_findObjectById(gSeqTrackInfos[scene][track].targetId);
            if (obj != NULL) {
                cutscene_applyObjectTrack(obj, scene, track);
            }
        }
    }
}

/* Add `id` to the scene's gCutsceneActivatedActors list unless it is already there. */
void cutscene_addActivatedActor(s32 id, s32 scene) {
    s32 i;
    s32 count;
    s32 limit;
    u8 *countPtr;

    i = 0;
    countPtr = &gCutsceneActivatedActorCounts[scene];
    count = *countPtr;
    limit = count;
    // matching: a do-while over a copy of the count, as in cutscene_addSpawnedActor
    if (count > 0) {
        do {
            if (id == gCutsceneActivatedActors[scene][i]) {
                return;
            }
            i++;
        } while (i < limit);
    }

    gCutsceneActivatedActors[scene][count] = id;
    *countPtr = count + 1;
}

/* If the actor bound to the scene's track `track` is inactive, record its id
   (cutscene_addActivatedActor) and clear the flag. */
void cutscene_activateTrackActor(s32 track, s32 scene) {
    WorldObject *actor;
    SeqTrackInfo *info;

    if (gSeqPathKeyCounts[scene][track] != 0) {
        info = &gSeqTrackInfos[scene][track];
        if (info->type == 3) {
            actor = gfx_findObjectById(info->targetId);
            if (actor != NULL) {
                if (actor->inactive == 1) {
                    cutscene_addActivatedActor(actor->unique_id, scene);
                    actor->inactive = 0;
                }
            }
        }
    }
}

void cutscene_activateTrackActors(s32 scene) {
    s32 track;

    for (track = 0; track < gSeqTrackCounts[scene]; track++) {
        cutscene_activateTrackActor(track, scene);
    }
}

/* Set inactive again on every actor recorded in the scene's gCutsceneActivatedActors list. */
void cutscene_restoreInactive(s32 scene) {
    s32 i;
    u8 *count;

    count = &gCutsceneActivatedActorCounts[scene];
    for (i = 0; i < *count; i++) {
        gfx_findObjectById(gCutsceneActivatedActors[scene][i])->inactive = 1;
    }
}

void cutscene_spawnTrackActors(s32 scene) {
    s32 track;

    if (gCutsceneStates[scene] == 1) {
        D_800C3638 = 0;
        for (track = 0; track < gSeqTrackCounts[scene]; track++) {
            cutscene_spawnTrackActor(track, scene);
            cutscene_resetTrackFace(track, scene);
        }
        D_800C3638 = 1;
    }
}

/* Call actor_remove on every live object in the scene's gCutsceneSpawnedActors list. */
void cutscene_removeSpawnedActors(s32 scene) {
    s32 i;
    s32 count;
    Actor *obj;
    u8 *countPtr;

    countPtr = &gCutsceneSpawnedActorCounts[scene];
    count = *countPtr;
    for (i = 0; i < count; i++) {
        obj = &movement_obj[gCutsceneSpawnedActors[scene][i]];
        if (obj->type != 0) {
            actor_remove(obj, 0);
            count = *countPtr;
        }
    }
}

/* Run the gActorUpdateHandlers handler of an object whose state (its first word) is 0x1B or 4. */
void cutscene_runActorHandler(Actor *obj) {
    if ((obj->type == 0x1B) || (obj->type == 4)) {
        if (gActorUpdateHandlers[obj->type] != NULL) {
            gActorUpdateHandlers[obj->type](obj);
        }
    }
}

/* Whether `obj` is bound to an object track of its scene (scene 1 when obj->unk5 is 4): 1 when
   bound itself, 2 when bound through its parent (obj->unk65). */
s32 cutscene_isActorBound(Actor *obj) {
    s32 i;
    s32 scene;
    s32 parent;
    s32 id;

    scene = 0;
    if (obj == NULL) {
        return 0;
    }
    if (obj->type == 0) {
        return 0;
    }
    if (gSeqPathKeyCounts[scene] == NULL) {
        return 0;
    }
    if (obj->drawtype == 4) {
        scene = 1;
    }
    parent = obj->attach_char;
    if (parent != 0) {
        id = movement_obj[parent - 1].unique_id;
    } else {
        id = obj->unique_id;
    }
    for (i = 0; i < gSeqTrackCounts[scene]; i++) {
        if (gSeqPathKeyCounts[scene][i] != 0) {
            if (gSeqTrackInfos[scene][i].type == 2 && id == gSeqTrackInfos[scene][i].targetId) {
                if (parent == 0) {
                    return 1;
                }
                return 2;
            }
        }
    }

    return 0;
}

/* Whether actor `actorIndex` (db_Object_array[actorIndex].unk72) is bound to a track of the scene. */
s32 cutscene_isActorIndexBound(s32 actorIndex, s32 scene) {
    s32 i;

    for (i = 0; i < gSeqTrackCounts[scene]; i++) {
        if (gSeqPathKeyCounts[scene][i] != 0) {
            if (gSeqTrackInfos[scene][i].targetId == db_Object_array[actorIndex].unique_id) {
                return 1;
            }
        }
    }

    return 0;
}

/* Re-anchor the scene's paths. A type 6 track shifts every path key so that its own path starts at
   its object (unk2 0) or actor (unk2 1). A type 7 track instead rotates the paths about its first
   key to the anchor gCutsceneAnchorPos/gCutsceneAnchorHeading and adjusts the heading channel (7) to match. */
void cutscene_anchorPaths(s32 scene) {
    s32 track;
    s32 j;
    s32 k;
    s32 hasHeadingEvent;
    f32 y;
    f32 x;
    f32 z;
    f32 dx;
    f32 dy;
    f32 dz;
    SeqPathKey *key;

    for (track = 0; track < gSeqTrackCounts[scene]; track++) {
        if (gSeqPathKeyCounts[scene][track] == 0) {
            continue;
        }
        if (gSeqTrackInfos[scene][track].type == 6) {
            if (gCutsceneAnchorSet == 0) {
                key = gSeqPathKeys[scene][track];
                cutscene_setAnchor(key->x, key->y, key->z, 0.0f);
            }
            if (gSeqTrackInfos[scene][track].targetId == 0) {
                Actor *obj;
                obj = collision_findObjectById(gSeqTrackInfos[scene][track].unk3);
                if (obj == NULL) {
                    return;
                }
                x = obj->xpos;
                y = obj->ypos;
                z = obj->zpos;
            } else if (gSeqTrackInfos[scene][track].targetId == 1) {
                WorldObject *obj;
                // matching: the s8 id goes through the pointer variable before the call
                obj = (WorldObject *)(s8)gSeqTrackInfos[scene][track].unk3;
                obj = gfx_findObjectById((s32)obj);
                if (obj == NULL) {
                    return;
                }
                x = obj->x;
                y = obj->y;
                z = obj->z;
            } else {
                return;
            }
            key = gSeqPathKeys[scene][track];
            dx = x - key->x;
            dy = y - key->y;
            dz = z - key->z;
            for (j = 0; j < gSeqTrackCounts[scene]; j++) {
                for (k = 0; k < gSeqPathKeyCounts[scene][j]; k++) {
                    gSeqPathKeys[scene][j][k].x = gSeqPathKeys[scene][j][k].x + dx;
                    gSeqPathKeys[scene][j][k].y = gSeqPathKeys[scene][j][k].y + dy;
                    gSeqPathKeys[scene][j][k].z = gSeqPathKeys[scene][j][k].z + dz;
                }
            }
            return;
        } else if (gSeqTrackInfos[scene][track].type == 7) {
            f32 c;
            f32 s;
            f32 turn;
            f32 radians;
            f32 rx;
            f32 rz;
            f32 nx;
            f32 heading;
            SeqChannelKey *chKey;

            heading = 0.0f;
            for (j = 0; j < gSeqChannelKeyCounts[scene][track]; j++) {
                chKey = &gSeqChannelKeys[scene][track][j];
                if (chKey->channel == 7) {
                    heading = chKey->value;
                    break;
                }
            }
            if (gCutsceneAnchorSet == 0) {
                key = gSeqPathKeys[scene][track];
                cutscene_setAnchor(key->x, key->y, key->z, heading);
            }
            turn = gCutsceneAnchorHeading - heading;
            gCutsceneAnchorTurn = turn;
            key = gSeqPathKeys[scene][track];
            radians = turn * 0.01745329238f;
            x = key->x;
            y = key->y;
            z = key->z;
            c = cosf(radians);
            s = sinf(radians);
            dy = gCutsceneAnchorPos[1] - y;
            for (j = 0; j < gSeqTrackCounts[scene]; j++) {
                for (k = 0, hasHeadingEvent = 0; k < gSeqPathKeyCounts[scene][j]; k++) {
                    rx = gSeqPathKeys[scene][j][k].x - x;
                    rz = gSeqPathKeys[scene][j][k].z - z;
                    nx = rx * c + rz * s;
                    rz = -rx * s + rz * c;
                    gSeqPathKeys[scene][j][k].x = gCutsceneAnchorPos[0] + nx;
                    gSeqPathKeys[scene][j][k].y = gSeqPathKeys[scene][j][k].y + dy;
                    gSeqPathKeys[scene][j][k].z = gCutsceneAnchorPos[2] + rz;
                }
                if (gSeqTrackEventCounts[scene][j] != 0 && gSeqEventTracks[scene][j][4].type == 7) {
                    hasHeadingEvent = 1;
                }
                if (hasHeadingEvent == 0) {
                    for (k = 0; k < gSeqChannelKeyCounts[scene][j]; k++) {
                        chKey = &gSeqChannelKeys[scene][j][k];
                        if (chKey->channel == 7) {
                            chKey->value = chKey->value + turn;
                        }
                    }
                }
            }
            return;
        }
    }
}

/* Pick the camera track: the value of the earliest type-3 event in any track, else the first
   type-1 track, else 0. */
s32 cutscene_pickCameraTrack(s32 scene, s32 time) {
    s32 track;
    s32 best;
    s32 result;
    s32 score;
    SeqEvent *found;
    SeqEvent *ptr;

    result = -1;
    best = 99999;

    for (track = 0; track < gSeqTrackCounts[scene]; track++) {
        ptr = gSeqEventTracks[scene][track];
        if (ptr != NULL) {
            found = NULL;
            score = cutscene_findEvent(&ptr[3], 3, track, &found, 0, 0, 0, 0, 0, 0, scene);
            if (found != NULL) {
                if (score < best) {
                    best = score;
                    result = found->value;
                }
            }
        }
    }

    if (result == -1) {
        result = 0;
        for (track = 0; track < gSeqTrackCounts[scene]; track++) {
            if (gSeqTrackInfos[scene][track].type == 1) {
                result = track;
                break;
            }
        }
    }

    return result;
}

/* Free all subtitle slots. */
void cutscene_freeSubtitles(void) {
    bzero(gSubtitles, sizeof(gSubtitles));
}

/* Close every open subtitle. */
void cutscene_closeSubtitles(void) {
    s32 i;
    s32 zero;
    SeqSubtitle *entry;

    i = 0;
    do {
        entry = &gSubtitles[i];
        if (entry->active != 0) {
            text_removeBox(entry->handle);
            entry->handle = 0;
            // matching: the cleared value goes through a local
            zero = 0;
            entry->active = zero;
        }
        i++;
    } while ((SeqSubtitle *)D_800C3D48 != &gSubtitles[i]);
}

/* Release subtitle slot `sub`: close it now if `force`, else let an open one finish (text_closeBoxSkipPages). */
void cutscene_releaseSubtitle(SeqSubtitle *sub, s32 force) {
    SeqSubtitle *entry;

    entry = sub;
    if (force != 0) {
        text_removeBox(entry->handle);
        entry->handle = 0;
    } else if (entry->isOpen != 0) {
        text_closeBoxSkipPages(entry->handle);
    } else {
        entry->handle = 0;
    }
    entry->active = 0;
}

/* Find the subtitle slot of `track`, or claim a free one (stealing slot 0 when all are busy). */
SeqSubtitle *cutscene_getSubtitleSlot(s32 track, s32 replaceOpen) {
    s32 i;
    SeqSubtitle *found;
    SeqSubtitle *slot;

    track++;
    found = NULL;
    slot = NULL;
    for (i = 0; i < 3; i++) {
        if (gSubtitles[i].active == track) {
            if (replaceOpen != 0 && gSubtitles[i].isOpen != 0) {
                slot = gSubtitles;
                cutscene_releaseSubtitle(slot, 1);
            } else {
                found = &gSubtitles[i];
            }
            break;
        }
        if (gSubtitles[i].active == 0) {
            slot = &gSubtitles[i];
            if (replaceOpen != 0 && slot->isOpen != 0) {
                cutscene_releaseSubtitle(slot, 1);
            }
            break;
        }
    }
    if (i == 3) {
        slot = gSubtitles;
        cutscene_releaseSubtitle(slot, 1);
    }
    if (found != NULL) {
        return found;
    }
    if (slot != NULL) {
        slot->active = track;
    }
    return slot;
}

/* Give object 1 back to normal control after a cutscene: stop it, drop it to the floor or leave it
   swimming, and clear its movement locks. */
s32 cutscene_releasePlayer(s32 scene) {
    Actor *obj;
    ActorCtl *ctl;
    s8 zero;

    movement_obj[0].task_attrib &= ~0x200;
    if (gCutsceneKeepPlayer == 0) {
        obj = collision_findObjectById(1);
        if (obj != NULL) {
            dc_glbl_CharNum = obj - movement_obj;
            ctl = obj->mv;
            obj->speed = 0.0f;
            if (ctl != NULL) {
                ctl->actual_speed = 0.0f;
            }
            obj->unk54 = 0.0f;
            obj->unkCC = 0;
            obj->doing_hit = 0;

            if ((-10000.0f != obj->waterHeight) && (obj->ypos < obj->waterHeight)) {
                obj->inwater = 1;
                obj->yspd = 0.0f;
                obj->ygrav = 0.0f;
                if (((obj->waterHeight - 50.0f) < obj->aim.y) || ((obj->mv->water_attribs & 1) != 0)) {
                    obj->underwaterTime = 0;
                    hud_resetBubbles();
                }
            } else {
                obj->inwater = 0;
                obj->underwaterTime = 0;
                obj->yspd = -4.0f;
                obj->ypos = obj->floorY;
                obj->unk1CC = obj->floorY;
                obj->ygrav = 5.0f;
            }

            obj->count3 = 0;
            obj->count5 = 0;
            obj->norun = 0;
            obj->nojump = 0;
            obj->stunned = 0;
            obj->shad = obj->ypos - obj->floorY;
            gCurrentActor = obj;
            anim_updateIdle();
            obj->mv->door_walkf_time = 0;
            obj->unk287 = 0;
            zero = obj->unk287;
            obj->headEffect = 0;
            obj->headEffectTimer = 0;
            obj->headEffectState = 0;
            // matching: the (u8) casts keep the register allocation of the u8 view this was matched with
            obj->unk286 = (u8)zero;
            obj->unk285 = (u8)zero;
            obj->unk284 = (u8)zero;
            return zero;
        }
    }
}

/* Per-frame update of the cutscene's objects (unless D_800C3654[0] hands it to actor_updateAll): run
   each live object's handler and apply the track of objects bound to one. */
void cutscene_updateActors(void) {
    s32 bound;
    s32 i;
    s32 limit;
    s32 one;
    Actor *obj;

    obj = movement_obj;
    i = 0;
    if (D_800C3654[0] != 0) {
        actor_updateAll();
        return;
    }
    // matching: 1 and 25 live in registers; the no-op `^ 0` and `& 0xFFFFFFFF` and the empty
    // do-while steer IDO's scheduling of them
    for (one = 1, limit = 25; i != (limit ^ 0); obj++) {
        if (obj->type != 0) {
            dc_glbl_CharNum = i;
            cutscene_runActorHandler(obj);
            face_update(obj);
            bound = cutscene_isActorBound(obj);
            if (bound != 0) {
                do { } while (0);
                cutscene_applyObjectTrack(obj, 0, -1);
                actor_onAnimChanged(obj);
                anim_updateIdleTimer(obj);
                obj->unk2FF = one & 0xFFFFFFFF;
            }
        }
        i++;
    }

    actor_updateTypeAnims();
}
