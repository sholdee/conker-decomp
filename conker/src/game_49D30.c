#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Cutscene ("sequence") data is kept per scene slot (0/1) and per track (up to 30 tracks). */

/* One 8-byte cutscene script event (D_800C3688[scene][track][i]). Each track starts with a
   4-event header whose value is the start time (see game_50D80.c). */
typedef struct {
    /* 0x0 */ s8 type;
    /* 0x1 */ u8 delta; /* time step to the next event; 0 chains events at the same time */
    /* 0x2 */ s16 value;
    /* 0x4 */ s8 unk4;
    /* 0x5 */ s8 unk5;
    /* 0x6 */ s16 unk6;
} SeqEvent;

/* A path keyframe: a point the track's actor moves through (D_800C3868[scene][track][key]). */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u16 linear;  // nonzero: straight segment to the next key, else Catmull-Rom curve
} SeqPathKey; // size 0x8

/* A channel keyframe: a timed value for one animated property (D_800C3778[scene][track][key]). */
typedef struct {
    /* 0x0 */ f32 value;
    /* 0x4 */ u8 interp;
    /* 0x5 */ u8 channel;
    /* 0x6 */ s16 time;
} SeqChannelKey; // size 0x8

/* What a track drives (D_800C35F0[scene][track]); type 2 tracks name an actor id in unk2. */
typedef struct {
    /* 0x0 */ u16 type;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
    /* 0x4 */ s16 yOffset;
    /* 0x6 */ s16 unk6;
} SeqTrackInfo; // size 0x8

/* Arc-length data for a track's path (D_800C3A50[scene][track]), plus the cubic of the cached segment. */
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

/* Current animated values of a track (one SeqTrackState array per slot at D_800C3958). Which property
   each unk field holds depends on the track's SeqTrackInfo type. */

extern u16 *D_800C35C8[];
extern u16 *D_800C35D8[];
extern SeqTrackInfo *D_800C35F0[];
extern u8 D_800C363A[];
extern void func_1507E7E4(void *, s32, s32, s32, s32);
extern s8 D_8008FD84[];
extern u8 D_800C35E8[];
extern s32 D_800C35B0[];
extern s32 D_800C3640[];
extern u8 D_800C3C99;
extern u8 D_800C3C9C;
extern u8 D_800D2E40;
s32 func_1501D2C4(s32 map, s32 cutscene);
extern u8 D_800C35AA;
extern void func_1507F640(void);
extern void func_1508F060(void);
extern void func_150242F8(s32, s32);
void func_1501EC38(s32 scene);
void func_15020EC4(s32 scene);
void func_1502178C(Actor *target, s32 scene, s32 onlyTrack);
void func_1501E81C(s32 skipped, s32 scene);
s32 func_150229E4(Actor *obj);
extern s32 func_1502B6BC(s32 *, s32, s32 *, s32, s32, s32, s32);
extern SeqEvent *D_800C3688[][30];
extern s32 D_800C3668[];
extern s32 func_15023BB0(SeqEvent *, s32, s32, SeqEvent **, s32, s32, s32, s32, s32, s32, s32);
void func_15022640(s32 id, s32 scene);
extern WorldObject *func_151149AC(u8);
extern Actor *func_1505EEF4(s32);
extern void func_15082A44(void *, s32, s32, s32, s32);
extern u8 D_800D2100;



extern SeqTrackPath *D_800C3A50[];
extern SeqPathKey *D_800C3868[][30];
extern SeqSubtitle D_800C3D48; /* == &D_800C3CA0[3] */
s32 func_1501F72C(s32 track, f32 dist, f32 *out, s32 scene, s32 *segmentOut);
void func_1501FFE8(f32 *out, SeqPathKey *keys, s32 key, s32 numKeys);
extern void func_1516D2E0(s32);
extern void func_1516D328(s32);

typedef struct {
    /* 0x00 */ s32 next;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u32 flags;
    /* 0x10 */ s32 framebuffer;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ OSTask_t list;
    /* 0x58 */ OSMesgQueue *msgQ;
    /* 0x5C */ void *msg;
} GfxSchedTask; /* 0x60 */

extern GfxSchedTask *D_800BEAD4;
extern GfxSchedTask D_800BEAD8[];
extern u8 D_800BEBA0;
extern u64 *D_800BEB9C;
extern u64 *D_800BEAD0;
extern u64 D_800BEBB0[];
extern u64 D_800C1060[];
extern u64 D_800CBE10[];
extern u64 D_8003A5E8[];
extern void func_1501A39C(void);
void func_1501CC3C(void);

/* Append a parameterless GBI command (gDPNoParam) at the display-list write head D_800BE9D0. */
#define GFX_APPEND_NOPARAM(cmd)                 \
{                                               \
    Gfx *gfx = D_800BE9D0++;                    \
    gfx->words.w0 = _SHIFTL(cmd, 24, 8);        \
    gfx->words.w1 = 0;                          \
}

/* Finish this frame's display list and send it to the scheduler as an RSP graphics task. */
void func_1501C880(s32 unused, s32 swapBuffers) {
    s32 capacity;
    Gfx **dlBase;
    s32 used;

    dlBase = D_800BE9C8;
    capacity = dlBase[1] - dlBase[0];
    D_800BEAD4 = &D_800BEAD8[D_800BE9C0];
    D_800BEAD4->next = 0;
    used = D_800BE9D0 - dlBase[D_800BE9C0];
    if ((used < 0) || (used > capacity)) {
        func_1501A39C();
        D_800BE9D0 = D_800BE9D8[D_800BE9C0];
        GFX_APPEND_NOPARAM(G_RDPFULLSYNC);
        GFX_APPEND_NOPARAM(G_ENDDL);
        used = D_800BE9D0 - dlBase[D_800BE9C0];
    }

    if ((used < 0) || (used > (capacity - 400))) {
        D_800BE9D0 = dlBase[D_800BE9C0] + (capacity - 400);
        GFX_APPEND_NOPARAM(G_RDPFULLSYNC);
        GFX_APPEND_NOPARAM(G_ENDDL);
    }

    D_800BEAD4->list.data_ptr = (u64 *)dlBase[D_800BE9C0];
    D_800BEAD4->list.data_size = (D_800BE9D0 - dlBase[D_800BE9C0]) * sizeof(Gfx);
    func_1501CC3C();

    D_800BEAD4->list.type = M_GFXTASK;
    D_800BEAD4->list.flags = OS_TASK_LOADABLE;
    D_800BEAD4->list.ucode_boot = (u64 *)D_100290D0;
    D_800BEAD4->list.ucode_boot_size = D_100291A0 - D_100290D0;
    if (D_800BEBA0 != 0) {
        D_800BEAD4->list.ucode = D_800C1060;
        D_800BEAD4->list.ucode_data = D_800BEB9C;
    } else {
        D_800BEAD4->list.ucode = D_800BEBB0;
        D_800BEAD4->list.ucode_data = D_800BEAD0;
    }
    D_800BEAD4->list.ucode_data_size = 0x800;
    D_800BEAD4->list.dram_stack = D_800CBE10;
    D_800BEAD4->list.dram_stack_size = 0x400;
    D_800BEAD4->list.output_buff = (u64 *)D_80038090;
    D_800BEAD4->list.output_buff_size = (u64 *)D_80038094;
    D_800BEAD4->list.yield_data_ptr = D_8003A5E8;
    D_800BEAD4->list.yield_data_size = 0xC00;
    D_800BEAD4->flags = 0x23;
    if (swapBuffers != 0) {
        D_800BEAD4->flags |= 0x40;
    }
    if (D_800BE617 != 0) {
        D_800BEAD4->flags |= (D_80082FA0 + 1) << 16;
        D_800BE617 = 0;
    }
    D_800BEAD4->msgQ = &D_800BEA10;
    D_800BEAD4->msg = &D_800BEA68[D_800BE9C0];
    D_800BEAD4->framebuffer = D_8002AAE8[D_800BE9C0];
    osWritebackDCacheAll();
    osSendMesg(&D_8003B1E8, D_800BEAD4, 1);
    if (swapBuffers != 0) {
        D_800BE9C0 ^= 1;
    }
    D_800BEAA8++;
}

/* ROM offsets (relative to D_2D4B0) of one RSP microcode's text and data. */
typedef struct {
    s32 textStart;
    s32 textEnd;
    s32 dataStart;
    s32 dataEnd;
} UcodeRomEntry;

extern u8 D_2D4B0[];
extern s8 D_80084070;
extern s8 D_80084074;
extern UcodeRomEntry D_80084078[];
extern s32 D_800BEB98;

/* DMA the two RSP microcodes (D_800C1060 and D_800BEBB0, each text then data) for the current
   graphics mode, unless they are already loaded. */
void func_1501CC3C(void) {
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
    if ((ucode != D_80084070) || (altOffset != D_80084074)) {
        func_10004514(D_80084078[ucode].textStart + (s32)D_2D4B0, D_800C1060, D_80084078[ucode].textEnd - D_80084078[ucode].textStart, 1);
        D_800BEB9C = (u64 *)((D_80084078[ucode].textEnd - D_80084078[ucode].textStart) + (u8 *)D_800C1060);
        func_10004514(D_80084078[ucode].dataStart + (s32)D_2D4B0, D_800BEB9C, D_80084078[ucode].dataEnd - D_80084078[ucode].dataStart, 1);
        D_80084070 = ucode;
        D_80084074 = altOffset;
        alt = D_80084078 + (ucode + altOffset);
        func_10004514(alt->textStart + (s32)D_2D4B0, D_800BEBB0, alt->textEnd - alt->textStart, 1);
        D_800BEAD0 = (u64 *)((alt->textEnd - alt->textStart) + (u8 *)D_800BEBB0);
        func_10004514(alt->dataStart + (s32)D_2D4B0, D_800BEAD0, alt->dataEnd - alt->dataStart, 1);
    }
}

extern u8 *D_800C3960[][30];

/* Mark every track's active-event table (D_800C3960) as empty. */
void func_1501CDC0(s32 scene) {
    s32 i;
    s32 j;

    for (i = 0; i < D_800C363A[scene]; i++) {
        for (j = 0; j < 16; j++) {
            D_800C3960[scene][i][j] = 0xFF;
        }
    }
}

extern u16 *D_800C35D0[];
extern SeqChannelKey *D_800C3778[][30];

/* Work out the scene's end time D_800C3640[scene]: track 0's header length if set, else the latest
   header end time among tracks with events, else the time of the last channel key. */
void func_1501CE54(s32 scene) {
    s32 found;
    s32 i;
    s16 length;
    u16 numKeys;

    D_800C3640[scene] = 0;
    if (D_800C363A[scene] != 0) {
        if (D_800C35D8[scene][0] >= 4) {
            length = D_800C3688[scene][0][2].value;
            if (length != -9999) {
                D_800C3640[scene] = length;
                return;
            }
        }
        found = 0;
        for (i = 0; i < D_800C363A[scene]; i++) {
            if (D_800C35C8[scene][i] != 0 && D_800C35D8[scene][i] != 0) {
                if (D_800C3640[scene] < D_800C3688[scene][i][1].value) {
                    D_800C3640[scene] = D_800C3688[scene][i][1].value;
                    found = 1;
                }
            }
        }
        if (found == 0) {
            for (i = 0; i < D_800C363A[scene]; i++) {
                if (D_800C35C8[scene][i] != 0) {
                    numKeys = D_800C35D0[scene][i];
                    if (numKeys) {
                        if (D_800C3640[scene] < D_800C3778[scene][i][numKeys - 1].time) {
                            D_800C3640[scene] = D_800C3778[scene][i][numKeys - 1].time;
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
s32 func_1501CFF8(s32 scene) {
    s32 sum;
    s32 i;
    s32 count;
    s32 limit;
    u16 *counts;

    sum = 0;
    i = 0;
    if ((count = D_800C363A[scene]) > 0) {
        limit = count;
        counts = D_800C35D8[scene];
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

extern s32 D_800C35B8[];
extern u8 D_800C35C0[];
extern u16 D_800C3C8E;
extern u8 D_800C35F8;
extern f32 D_800C3648;
extern f32 D_800C364C;
extern f32 D_800C3650;
extern u8 D_800C3656[];
extern u8 D_800C3658[];
extern u8 D_800C365A[];
extern u8 D_800C365C[];
extern u8 D_800C365E[];
extern u8 D_800C3660[];
extern u8 D_800C3662;
extern u8 D_800C3C8C;
extern u8 D_800C3C8D;
extern u8 D_800C35C2;
extern u8 D_800C3A58;
extern f32 D_800C35A4;
extern s8 D_800C3C8A;
extern u8 D_800C3C8B;
extern f32 D_800C3674;
extern f32 D_800C3678;
extern s32 D_800C367C;
extern u8 D_800C3680;
extern u8 D_800C3672;
extern u8 D_800C3C9D;
extern s16 D_800C3C9E;
void func_150233BC(void);

/* Reset the per-scene cutscene state before a new script is loaded. */
void func_1501D044(s32 scene) {
    D_800C35B8[scene] = -100;
    D_800C35B0[scene] = 0;
    D_800C35C0[scene] = 0;
    D_800C3C8E = 0;
    D_800C363A[scene] = 0;
    D_800C35EA[scene] = 0;
    D_800C3D50 = NULL;
    D_800C35F8 = 0xFF;
    D_800C3648 = 0.0f;
    D_800C364C = 0.0f;
    D_800C3650 = 0.0f;
    D_800C3654[scene] = 0;
    D_800C3656[scene] = 0;
    D_800C3658[scene] = 1;
    D_800C365A[scene] = 0;
    D_800C365C[scene] = 0;
    D_800C365E[scene] = 0;
    D_800C3660[scene] = 0;
    D_800C3638 = 0;
    D_800C3662 = 0;
    D_800C3C8C = 0;
    D_800C3C8D = 0;
    D_800C35C2 = 0;
    D_800C3A58 = 0;
    D_800C35A4 = 0.0f;
    D_800C3C8A = -1;
    D_800C3C8B = 0;
    D_800C3674 = 999.0f;
    D_800C3678 = 999.0f;
    D_800C367C = 0;
    D_800C3680 = 0;
    D_800C3672 = 0;
    D_800C3C9D = 100;
    D_800C3C9E = -1;
    func_150233BC();
    D_8008FD84[0] = 0;
    D_800C3670 = 0;
}

/* Load cutscene `cutscene` of map `map` into D_800C3668[scene]; returns the file (0 if missing). */
s32 func_1501D1D4(s32 map, s32 cutscene, s32 scene) {
    s32 size;
    s32 unused;
    s32 file;

    file = func_1502B6BC(&unused, 0, &size, 3, 6, map, cutscene);
    if (file != 0) {
        D_800C3668[scene] = file;
    } else {
        D_800C3668[scene] = 0;
    }

    return file;
}

/* Flag cutscene `cutscene` of map `map` as played (unless D_800C3670 is set). */
void func_1501D258(s32 map, s32 cutscene) {
    if (!D_800C3670) {
        D_800C3A60[map] |= (1LL << cutscene);
    }
}

/* Whether cutscene `cutscene` of map `map` has been played; always true while D_800C3670 is set. */
s32 func_1501D2C4(s32 map, s32 cutscene) {
    if (D_800C3670 != 0) {
        return 1;
    }
    if ((D_800C3A60[map] & (1ULL << cutscene)) != 0) {
        return 1;
    }
    return 0;
}

/* Cutscene file layout: a SeqFileEntry table whose entry 0 points at this header, followed by
   numTracks path-key lists, numTracks entries starting with the SeqTrackInfo table, numChannelLists channel-key lists,
   numEventLists event lists and numUnkLists D_800C35E0 lists. */
typedef struct {
    u16 numTracks;
    u16 numChannelLists;
    u16 numEventLists;
    u16 numUnkLists;
} SeqFileHeader;

/* A block of the cutscene file; size >> 3 is its count of 8-byte elements. */
typedef struct {
    s32 data;
    u32 size;
} SeqFileEntry;

extern void *allocate_memory(s32, s32, s32, s32);
extern void func_1501C730(s32, s32, s32, s32, s32);
void func_15020388(s32 track, s32 scene);
void func_15022BA4(s32 scene);
extern void func_150238D0(void);
void func_15022234(s32 scene);
void func_15022248(s32 scene);
void func_15022848(s32 scene);
void func_15022754(s32 scene);
void func_15022528(s32 scene);
void func_1501DAAC(s32 scene);
extern void (*D_800840FC[])(s32, s32, s32, s32);
extern u8 D_800C35A8;
extern u8 D_800C3C98;
extern u16 D_800C3C9A;
extern s32 *D_800C35E0[];

/* Start cutscene `cutscene` of map `map` in scene slot `scene`. If another map is loaded, switch
   maps first (func_1501C730) and leave the request pending in D_800C35C4. Otherwise load the
   cutscene file, allocate and fill the per-track tables, and run the first frame. */
void func_1501D348(s32 map, s32 cutscene, s32 scene, s32 callback, s32 callbackArg) {
    SeqFileHeader *hdr;
    SeqFileEntry *file;
    s32 state;
    s32 i;
    s32 k;
    s32 size;

    state = D_800C35EA[scene];
    if (state != 1 && D_800C35C4 == 0) {
        D_800C3C90 = callback;
        D_800C3C94 = callbackArg;
        if (map != D_800BE9F0) {
            D_800C35C4 = cutscene + 1;
            func_1501C730(1, map, 0, 0, 0);
            if (D_800C35EA[scene] != 0) {
                D_800C35C5 = 0;
                return;
            }
            D_800C35C5 = 1;
            return;
        }
        D_800C35E8[scene] = cutscene;
        if (state != 2) {
            D_800C35A8 = D_800BE9F0;
            D_800C3C98 = cutscene;
        }
        func_1501D044(scene);
        if (func_1501D1D4(map, cutscene, scene) != 0) {
            file = (SeqFileEntry *)D_800C3668[scene];
            hdr = *(SeqFileHeader **)file;
            D_800C35C8[scene] = allocate_memory(hdr->numTracks * sizeof(u16), 1, 0, 0);
            k = 1;
            for (i = 0; i < hdr->numTracks; i++) {
                D_800C35C8[scene][i] = file[k].size >> 3;
                k++;
            }
            size = hdr->numChannelLists;
            if (size < hdr->numTracks) {
                size = hdr->numTracks;
            }
            D_800C35D0[scene] = allocate_memory(size * sizeof(u16), 1, 0, 0);
            bzero(D_800C35D0[scene], size * sizeof(u16));
            size = hdr->numEventLists;
            if (size < hdr->numTracks) {
                size = hdr->numTracks;
            }
            D_800C35D8[scene] = allocate_memory(size * sizeof(u16), 1, 0, 0);
            bzero(D_800C35D8[scene], size * sizeof(u16));
            D_800C35E0[scene] = allocate_memory(hdr->numUnkLists * sizeof(s32 *), 1, 0, 0);
            D_800C3A50[scene] = allocate_memory(hdr->numTracks * sizeof(SeqTrackPath), 1, 0, 0);
            for (i = 0; i < hdr->numTracks; i++) {
                if (D_800C35C8[scene][i] != 0) {
                    D_800C3A50[scene][i].keyDistance = allocate_memory(D_800C35C8[scene][i] * sizeof(f32), 1, 0, 0);
                    D_800C3A50[scene][i].arcLength = allocate_memory(101 * sizeof(f32), 1, 0, 0);
                }
            }
            D_800C3958[scene] = allocate_memory(hdr->numTracks * sizeof(SeqTrackState), 1, 0, 0);
            for (i = 0; i < hdr->numTracks; i++) {
                D_800C3960[scene][i] = allocate_memory(16, 1, 0, 0);
            }
            for (i = 0; i < hdr->numTracks; i++) {
                D_800C35D8[scene][i] = 0;
                D_800C35D0[scene][i] = 0;
                D_800C35C8[scene][i] = 0;
            }
            D_800C363A[scene] = hdr->numTracks;
            k = 1;
            for (i = 0; i < hdr->numTracks; i++) {
                D_800C35C8[scene][i] = file[k].size >> 3;
                D_800C3868[scene][i] = (SeqPathKey *)file[k].data;
                k++;
            }
            D_800C35F0[scene] = (SeqTrackInfo *)file[k].data;
            k += hdr->numTracks;
            for (i = 0; i < hdr->numChannelLists; i++) {
                D_800C35D0[scene][i] = file[k].size >> 3;
                D_800C3778[scene][i] = (SeqChannelKey *)file[k].data;
                k++;
            }
            for (i = 0; i < hdr->numEventLists; i++) {
                D_800C35D8[scene][i] = file[k].size >> 3;
                D_800C3688[scene][i] = (SeqEvent *)file[k].data;
                k++;
            }
            for (i = 0; i < hdr->numUnkLists; i++) {
                D_800C35E0[scene][i] = file[k].data;
                k++;
            }
            if (callback != 0) {
                D_800840FC[callback](map, cutscene, scene, callbackArg);
            }
            D_800C3C9A = func_1501CFF8(scene);
            D_800C3D50 = allocate_memory(D_800C3C9A * 12, 1, 0, 0);
            D_800C35B0[scene] = 0;
            func_1501CDC0(scene);
            func_1501CE54(scene);
            func_15022BA4(scene);
            for (i = 0; i < D_800C363A[scene]; i++) {
                func_15020388(i, scene);
            }
            func_150238D0();
            D_800C35EA[scene] = 1;
            D_800C3638 = 1;
            func_15022234(scene);
            func_15022248(scene);
            if (D_800C363A[scene] == 0) {
                func_1501E81C(0, scene);
            }
            func_150242F8(1, scene);
            func_1501EC38(scene);
            func_15022848(scene);
            func_15022754(scene);
            func_15022528(scene);
            func_15020EC4(scene);
            D_800DBFF4[0] = 4;
            func_1501DAAC(scene);
        }
    }
}

/* A screen fade colour for func_1517EE40 (r, g, b, ticks, direction, layer; see game_1AC2F0.c). */
typedef struct {
    u8 r;
    u8 g;
    u8 b;
} FadeColor;

extern u8 D_800C35A9;
extern FadeColor D_80084110[];
extern void func_1517EE40(s32, s32, s32, s32, s32, s32);

/* Scan the scene's scripts for type-5 events with values 0x1E, 0xE, 0x68 and 0x44 (setting the
   flags D_800C3C99, D_800C3C9C, D_800C35AA, D_800C35A9) and 0x77 (start time D_800C3C8E), and start
   the screen fade (func_1517EE40) to the D_80084110 colour named by the earliest type-1 value-1 event. */
void func_1501DAAC(s32 scene) {
    s32 i;
    s32 best;
    SeqEvent *found;
    s32 score;
    SeqEvent *events;

    D_800C3C99 = 0;
    D_800C35A9 = 0;
    D_800C3C9C = 0;
    D_800C35AA = 0;
    D_800C3C8E = 0;

    best = -1;
    for (i = 0; i < D_800C363A[scene]; i++) {
        events = D_800C3688[scene][i];
        if (events != NULL) {
            found = NULL;
            func_15023BB0(&events[3], 5, i, &found, 1, 0x1E, 0, 0, 0, 0, scene);
            if (found != NULL) {
                D_800C3C99 = 1;
            }

            found = NULL;
            func_15023BB0(&events[3], 5, i, &found, 1, 0xE, 0, 0, 0, 0, scene);
            if (found != NULL) {
                D_800C3C9C = 1;
            }

            found = NULL;
            func_15023BB0(&events[3], 5, i, &found, 1, 0x68, 0, 0, 0, 0, scene);
            if (found != NULL) {
                D_800C35AA = 1;
            }

            found = NULL;
            func_15023BB0(&events[3], 5, i, &found, 1, 0x44, 0, 0, 0, 0, scene);
            if (found != NULL) {
                D_800C35A9 = 1;
            }

            found = NULL;
            score = func_15023BB0(&events[3], 5, i, &found, 1, 0x77, 0, 0, 0, 0, scene);
            if (found != NULL) {
                D_800C3C8E = score;
            }

            found = NULL;
            score = func_15023BB0(&events[3], 5, i, &found, 1, 1, 1, 0, 0, 0, scene);
            if (found != NULL) {
                if ((best == -1) || (score < best)) {
                    best = score;
                    if (found->unk6 == 1) {
                        func_1517EE40(D_80084110[found->unk5].r, D_80084110[found->unk5].g, D_80084110[found->unk5].b, 0, 1, 0);
                    } else {
                        func_1517EE40(D_80084110[found->unk5].r, D_80084110[found->unk5].g, D_80084110[found->unk5].b, 0, 0, 0);
                    }
                }
            }
        }
    }

    D_800C35B0[scene] = D_800C3C8E;
}

/* Jump the scene to its end: advance the clock past the end time, update every track once, apply
   the result to the objects bound to tracks and finish the cutscene. */
void func_1501DE18(s32 scene) {
    s32 i;
    s32 count;
    Actor *obj;

    D_800C35B0[scene] = D_800C3640[scene] + D_800BEA08;
    func_150242F8(1, 0);
    func_1501EC38(0);
    func_150242F8(0, 0);
    func_15020EC4(0);

    // matching: the object count lives in its own register
    for (obj = D_800CC2D0, i = 0, count = 25; i != count; i++, obj++) {
        if (obj->interaction_state) {
            D_800C3E78 = i;
            if (func_150229E4(obj)) {
                func_1502178C(obj, scene, -1);
            }
        }
    }

    func_1501E81C(1, scene);
}

extern void func_1000E17C(void);
extern void func_151F2BA8(void);
void func_1501DE18(s32 scene);
/* Skip the running cutscene: jump to its end (again while the next one chains on, D_800C3C99),
   then restore the screen fade and clear the objects' cutscene state. */
void func_1501DF04(s32 scene) {
    Actor *obj;
    u8 chained;
    s32 sceneCopy;

    if (func_100127D0() != 0) {
        func_151F2BA8();
    }
    obj = func_15072208(D_800CC2D0, 0);
    if (obj != NULL) {
        func_15060F28(obj, 0);
    }
    chained = D_800C3C99;
    func_1501DE18(scene);
    if (chained != 0) {
        if (D_800C35C4 == 0) {
            do {
                chained = D_800C3C99;
                // matching: the argument goes through a local copy
                func_1501DE18(sceneCopy = scene);
            } while ((chained != 0) && (D_800C35C4 == 0));
        }
    }
    // matching: the fade call and the loop setup share a source line (-g3 line numbers steer scheduling)
    if (D_800C35A9 == 0 && D_800C3C8A != 1 && D_800C3C8D == 0) { func_1517EE40(0, 0, 0, 0x1E, 0, 0); } { Actor *obj = D_800CC2D0; Actor *end = &D_800D121C; do {
        if (obj->interaction_state != 0) {
            obj->fogMode = 0;
        }
        obj++;
    } while (obj != end); }
    D_800C3663 = 0;
    func_1000E17C();
    D_800DBFF4[0] = 4;
    D_800C3672 = 1;
}

/* Whether the running cutscene may be skipped now (button 0x20, or map-specific rules for maps
   0x1D and 0x21). */
s32 func_1501E05C(s32 scene) {
    s32 map;
    s32 skipRequest;

    map = D_800BE9F0;
    // matching: holding the address as an integer keeps IDO from folding it into the loads
    skipRequest = (s32)D_8008FD84;

    if (map == 0x1D) {
        // matching: the goto orders the D_800C35E8 address load after the branch
        if (*(s8 *)skipRequest == 0) {
            goto noRequest;
        }
        *(s8 *)skipRequest = 0;
        return 1;
    noRequest:
        if (D_800C35E8[0] != 5) {
            return 0;
        }
    }

    if (map == 0x21) {
        if ((D_800BE710[0] & 0x1000) && (D_8000030C == 1) && (D_800C35B0[0] >= 301)) {
            return 1;
        }
        return 0;
    }

    if (D_800BE710[0] & 0x20) {
        if (D_800C3C9C == 0) {
            if (D_800D2E40 != 0) {
                return 1;
            }
            if (func_1501D2C4(map, D_800C35E8[scene]) != 0) {
                if ((D_800C3C99 != 0) || ((D_800C35B0[scene] + 30) < D_800C3640[scene])) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

extern u8 D_800C3C89;
s32 func_151F2CDC(void);
void func_151F2D6C(s32, s32);

/* Cutscene skip state machine (D_800C3C88): 0 = waiting for a skip request (needs at least 91
   ticks left), 1 = fading out for 30 ticks (D_800C3C89), 2 = skipping (func_1501DF04). */
void func_1501E1B4(s32 scene) {
    if (D_800C3C88 == 0) {
        if (D_800C35EA[scene] != 1) {
            return;
        }
        if (func_1501E05C(scene) == 0) {
            return;
        }
        D_8008FD84[0] = 0;
        if (D_800C3640[scene] - D_800C35B0[scene] < 91) {
            return;
        }
        D_800C3C88 = 1;
        D_800C3C89 = 30;
        func_1517EE40(0, 0, 0, 0x1E, 1, 0);
        func_1000E17C();
        if ((func_151F2CDC() == 1) || (func_151F2CDC() == 2)) {
            func_151F2D6C(0, 0x2B02);
        }
    } else if (D_800C3C88 == 1) {
        if (D_800BEA08 < D_800C3C89) {
            D_800C3C89 -= D_800BEA08;
            return;
        }
        D_800C3C89 = 0;
        D_800C3C88 = 2;
        func_1501DF04(scene);
        D_800C3C88 = 0;
    }
}

void func_1501E400(s32 scene);

/* At the scene's end time, finish it (func_1501E81C) unless D_800C3C8C holds it; if a chained
   scene started, run its first frame. */
void func_1501E2F8(s32 scene) {
    if ((D_800C35EA[scene] == 1) && (D_800C35B0[scene] >= D_800C3640[scene])) {
        if (D_800C3C8C == 0) {
            func_1501E81C(0, scene);
            if (D_800C35EA[scene] == 1) {
                func_1501E400(0);
                func_150242F8(1, 0);
                func_1501EC38(0);
                func_150242F8(0, 0);
                func_15020EC4(0);
                func_1501E2F8(0);
            }
        } else {
            D_800C3C8C = 2;
        }
    }
}

void func_15080228(void);

/* Advance the scene clock D_800C35B0 by this frame's ticks, clamped to the end time. */
void func_1501E400(s32 scene) {
    if (D_800C3A58 != 0) {
        D_800C3A58 -= 1;
    }
    D_800C35B8[scene] = D_800C35B0[scene];
    if (D_800C35EA[scene] == 1) {
        func_1501E1B4(scene);
        if ((scene != 0) || (D_800BEAC0 == 0)) {
            D_800C35B0[scene] += D_800BEA08;
        }
        if ((D_800C35C0[scene] == 0) &&
            (D_800C3C8E == D_800C35B8[scene]) &&
            (D_800C3C8E != D_800C35B0[scene])) {
            D_800C35B8[scene] = -1;
        }
        if (func_151F2CDC() == 1) {
            func_15080228();
        }
        if (D_800C35B0[scene] >= D_800C3640[scene]) {
            D_800C35B0[scene] = D_800C3640[scene];
        }
    }
}

void func_10004074(s32);

/* Free everything func_1501D348 allocated for the scene, and the cutscene file itself. */
void func_1501E540(s32 scene) {
    s32 track;
    SeqFileHeader *hdr;
    u8 **active;

    if (D_800C3668[scene] != 0) {
        hdr = *(SeqFileHeader **)D_800C3668[scene];
        for (track = 0; track < hdr->numTracks; track++) {
            if (D_800C35C8[scene][track] != 0) {
                func_10004074((s32)D_800C3A50[scene][track].keyDistance);
                func_10004074((s32)D_800C3A50[scene][track].arcLength);
            }
        }
        func_10004074((s32)D_800C3A50[scene]);
        D_800C3A50[scene] = NULL;
        func_10004074((s32)D_800C3958[scene]);
        D_800C3958[scene] = NULL;
        active = D_800C3960[scene];
        for (track = 0; track < hdr->numTracks; track++) {
            func_10004074((s32)active[track]);
            active[track] = NULL;
        }
        func_10004074((s32)D_800C35C8[scene]);
        func_10004074((s32)D_800C35D0[scene]);
        func_10004074((s32)D_800C35D8[scene]);
        func_10004074((s32)D_800C35E0[scene]);
        D_800C35C8[scene] = NULL;
        D_800C35D0[scene] = NULL;
        D_800C35D8[scene] = NULL;
        D_800C35E0[scene] = NULL;
        func_10004074(D_800C3D50);
        D_800C3D50 = NULL;
        func_10004074(D_800C3668[scene]);
        D_800C3668[scene] = 0;
    }
}

void func_1507EABC(void *);

/* For each object bound to a track: wrap its eyeCode counters at 10 and call func_1507EABC. */
void func_1501E73C(s32 scene) {
    s32 i;
    Actor *obj;

    for (i = 0; i < D_800C363A[scene]; i++) {
        if (D_800C35F0[scene][i].type == 2) {
            obj = func_15083E90(D_800C35F0[scene][i].unk2);
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
            func_1507EABC(obj);
        }
    }
}

extern void func_15169040(u8 *, u8);
extern u8 D_800C35C3;
void func_1501EA18(s32 scene);
void func_150233E4(void);
s32 func_150235DC(s32 scene);

/* End the scene's cutscene: report it (func_15169040), flag it played, release its data and restore
   the camera. If another cutscene is chained (D_800C35C2), start it instead. */
void func_1501E81C(s32 skipped, s32 scene) {
    Camera *player;

    player = D_800DBFF0;
    if (D_800C35EA[scene] != 0) {
        {
            u8 cutscene[1];
            s32 reason;

            cutscene[0] = D_800C35E8[scene];
            reason = skipped ? 0xF : 0x10;
            func_15169040(cutscene, reason);
        }
        D_800C35B8[scene] = -100;
        D_800C35B0[scene] = D_800C3C8E;
        func_1501D258(D_800BE9F0, D_800C35E8[scene]);
        D_800C35EA[scene] = 0;
        D_800C3638 = 0;
        func_1501E73C(scene);
        func_150233E4();
        D_800C3662 = 0;
        D_800C3683 = 0;
        func_150235DC(scene);
        func_1501EA18(scene);
        func_1501E540(scene);
        D_800C3648 = 0.0f;
        D_800C364C = 0.0f;
        D_800C3650 = 0.0f;
        func_1510B32C(0, player->unk19C, player->unk1A0, 1.0f);
        if (D_800C35C2 != 0) {
            s32 savedMap;
            s32 map;

            D_800C35EA[scene] = 2;
            map = D_800C35C3;
            if (0x25 == D_800C35C3) {
                savedMap = D_800BE9F0;
                D_800BE9F0 = 0x25;
                map = 0x25;
            }
            D_800C3671 = 0;
            func_1501D348(map, D_800C35C2 - 1, scene, D_800C3C90, D_800C3C94);
            if (0x25 == D_800C35C3) {
                D_800BE9F0 = savedMap;
            }
            return;
        }
        if (D_800C3C8B != 0) {
            func_1517EE40(0, 0, 0, 0x1E, 0, 0);
        }
        D_8008FD84[0] = 0;
        D_800C3670 = 0;
        D_800C3671 = 0;
    }
}

extern void func_1512D560(void *, s32, void *);
void func_150228E4(s32 scene);
void func_150227BC(s32 scene);
extern s16 D_800C358C[];
extern s16 D_800C3598[];
extern f32 D_800C3594;
extern f32 D_800C35A0;

/* Hand control back after the scene's cutscene: restore the player camera and object state. */
void func_1501EA18(s32 scene) {
    Camera *player;
    Actor *obj;
    s32 i;

    player = D_800DBFF0;
    D_800C3600.unk0 = 1;
    D_800C3600.unk4 = scene;
    D_800C3600.unk14 = player->eye.x;
    D_800C3600.unk18 = player->eye.y;
    D_800C3600.unk1C = player->eye.z;
    D_800C3600.unk20 = 0.0f;
    D_800C3600.unk24 = player->pitch;
    D_800C3600.unk28 = player->yaw;
    if (D_800C3671 == 0) {
        func_1512D560(player, 7, &D_800C3600);
        if (D_800C3674 != 999.0f) {
            D_800C3600.unkC = D_800C3674;
            func_1512D560(player, 8, &D_800C3600);
        }
        if (D_800C3678 != 999.0f) {
            D_800C3600.unkC = D_800C3678;
            D_800C3600.unk10 = D_800C367C;
            func_1512D560(player, 10, &D_800C3600);
        }
    }
    if (D_800C35C2 == 0 && D_800C3671 == 0) {
        if (D_800C3680 == 0) {
            func_1512D560(player, 6, (void *)scene);
        } else {
            D_800C3680 = 0;
        }
    }
    if (D_800C365A[scene] == 0) {
        func_150228E4(scene);
    }
    func_150227BC(scene);
    for (i = 0; i != 25; i++) {
        if (D_800CC2D0[i].interaction_state != 0 && func_150229E4(&D_800CC2D0[i]) != 0) {
            D_800CC2D0[i].fogMode = 0;
            D_800CC2D0[i].unkB8 = 0.0f;
            D_800CC2D0[i].unkC4 = 0.0f;
        }
    }
    if (D_800C35C2 == 0) {
        D_800C3A58 = 2;
        D_800C3663 = 0;
        D_800C3594 = D_800C35A0;
        D_800C358C[0] = D_800C3598[0];
        D_800C358C[1] = D_800C3598[1];
        D_800C358C[2] = D_800C3598[2];
    }
}

extern f32 func_150497E0(f32 *, s32, f32);
void func_1501FC8C(s32 track, f32 percent, f32 *x, f32 *y, f32 *z, s32 scene);
void func_1501FE68(s32 track, f32 percent, f32 *heading, s32 scene);

/* Evaluate every track of the scene at the current time D_800C35B0: start from the track's path
   start and type defaults, then interpolate each of its 15 channels between the keys around the
   current time (Catmull-Rom via func_150497E0, or linear) and store them in its SeqTrackState.
   Channel 10 is the position along the path in percent. */
void func_1501EC38(s32 scene) {
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

    // matching: indexing through &D_800C35EA[0] rather than the array changes register allocation
    if ((&D_800C35EA[0])[scene] == 1) {
        D_800C3638 = 0;
        for (track = 0; track < D_800C363A[scene]; track++) {
            if (D_800C35C8[scene][track] != 0) {
                D_800C3958[scene][track].x = D_800C3868[scene][track]->x;
                D_800C3958[scene][track].y = D_800C3868[scene][track]->y;
                D_800C3958[scene][track].z = D_800C3868[scene][track]->z;
            } else {
                D_800C3958[scene][track].x = 0.0f;
                D_800C3958[scene][track].y = 0.0f;
                D_800C3958[scene][track].z = 0.0f;
            }
            if (D_800C35F0[scene][track].type == 4) {
                D_800C3958[scene][track].unkC = 64.0f;
                D_800C3958[scene][track].unk10 = 0.0f;
                D_800C3958[scene][track].unk14 = 20.0f;
                D_800C3958[scene][track].unk18 = 255.0f;
                D_800C3958[scene][track].unk1C = 62.5f;
                D_800C3958[scene][track].unk20 = 50.0f;
                D_800C3958[scene][track].unk24 = 0.0f;
                D_800C3958[scene][track].unk28 = 0.0f;
                D_800C3958[scene][track].unk2C = 0.0f;
                D_800C3958[scene][track].unk34 = 0.0f;
            } else {
                D_800C3958[scene][track].unkC = 0.0f;
                D_800C3958[scene][track].unk10 = D_800C35A4;
                D_800C3958[scene][track].unk14 = 0.0f;
                D_800C3958[scene][track].unk18 = -9999.0f;
                D_800C3958[scene][track].unk1C = -9999.0f;
                D_800C3958[scene][track].unk20 = -9999.0f;
                D_800C3958[scene][track].unk24 = 0.0f;
                D_800C3958[scene][track].unk28 = 0.0f;
                D_800C3958[scene][track].unk2C = 0.0f;
                D_800C3958[scene][track].unk3C = -9999.0f;
                D_800C3958[scene][track].unk40 = -9999.0f;
                D_800C3958[scene][track].unk30 = -9999.0f;
            }
            D_800C3958[scene][track].yOffset = 0.0f;
            if (D_800C35F0[scene][track].type == 1) {
                D_800C3958[scene][track].unk34 = -9999.0f;
            } else if (D_800C35F0[scene][track].type == 2) {
                D_800C3958[scene][track].unk34 = 255.0f;
            } else if (D_800C35F0[scene][track].type == 5) {
                D_800C3958[scene][track].unk18 = 32.0f;
                D_800C3958[scene][track].unk1C = 32.0f;
                D_800C3958[scene][track].unk34 = 1.0f;
            } else if (D_800C35F0[scene][track].type == 3) {
                D_800C3958[scene][track].unk18 = -9999.0f;
                D_800C3958[scene][track].unk1C = -9999.0f;
                D_800C3958[scene][track].unk20 = -9999.0f;
                D_800C3958[scene][track].unk34 = 255.0f;
            }
            if (D_800C3778[scene][track] != NULL) {
                for (j = 0; j < 15; j++) {
                    nextTime[j] = -9999;
                    prevTime[j] = -9999;
                    nextKey[j] = -9999;
                    prevInterp[j] = 1;
                    numKeys[j] = 0;
                    firstValue[j] = -9999.0f;
                    lastValue[j] = -9999.0f;
                }
                for (j = 0; j < D_800C35D0[scene][track]; j++) {
                    key = &D_800C3778[scene][track][j];
                    channel = key->channel;
                    if (nextKey[channel] == -9999) {
                        if (key->time >= D_800C35B0[scene]) {
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
                        t = (f32)(D_800C35B0[scene] - prevTime[j]) / (f32)(nextTime[j] - prevTime[j]);
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
                            for (m = 0; m < D_800C35D0[scene][track]; m++) {
                                if (needed == 0) {
                                    break;
                                }
                                key = &D_800C3778[scene][track][m];
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
                            value[j] = func_150497E0(points, 0, t);
                        } else {
                            n = 0;
                            for (k = 0; k < D_800C35D0[scene][track]; k++) {
                                key = &D_800C3778[scene][track][k];
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
                            value[j] = D_800C3778[scene][track][next].value - D_800C3778[scene][track][found].value;
                            value[j] *= t;
                            value[j] += D_800C3778[scene][track][found].value;
                        }
                    }
                }
                if (value[10] == -9999.0f) {
                    value[10] = 0.0f;
                }
                if (D_800C3960[scene][track][0] != 0xFF) {
                    func_1501FE68(track, value[10], &D_800C3958[scene][track].unk10, scene);
                } else if (value[7] != -9999.0f) {
                    D_800C3958[scene][track].unk10 = value[7];
                }
                if (value[8] != -9999.0f) {
                    D_800C3958[scene][track].unkC = value[8];
                }
                if (value[6] != -9999.0f) {
                    D_800C3958[scene][track].unk14 = value[6];
                }
                if (value[5] != -9999.0f) {
                    D_800C3958[scene][track].unk18 = value[5];
                }
                if (value[4] != -9999.0f) {
                    D_800C3958[scene][track].unk1C = value[4];
                }
                if (value[3] != -9999.0f) {
                    D_800C3958[scene][track].unk20 = value[3];
                }
                if (value[1] != -9999.0f) {
                    D_800C3958[scene][track].unk24 = value[1];
                }
                if (value[2] != -9999.0f) {
                    D_800C3958[scene][track].unk28 = value[2];
                }
                if (value[0] != -9999.0f) {
                    D_800C3958[scene][track].unk2C = value[0];
                }
                if (value[14] != -9999.0f) {
                    D_800C3958[scene][track].unk30 = value[14];
                }
                if (value[9] != -9999.0f) {
                    D_800C3958[scene][track].unk34 = value[9];
                }
                if (value[11] != -9999.0f) {
                    D_800C3958[scene][track].yOffset = value[11];
                }
                if (value[13] != -9999.0f) {
                    D_800C3958[scene][track].unk3C = value[13];
                }
                if (value[12] != -9999.0f) {
                    D_800C3958[scene][track].unk40 = value[12];
                }
                func_1501FC8C(track, value[10], &D_800C3958[scene][track].x,
                              &D_800C3958[scene][track].y,
                              &D_800C3958[scene][track].z, scene);
            }
        }
        D_800C3638 = 1;
    }
}

void func_15020878(s32 track, s32 segment, s32 scene);

/* Find the path segment containing distance `dist` along track `track` and write two points to
   `out`: the ends of the arc-length sample on a curved segment, or the interpolated point twice on
   a linear one. Returns the arc-length sample index. */
s32 func_1501F72C(s32 track, f32 dist, f32 *out, s32 scene, s32 *segmentOut) {
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
    hi = D_800C35C8[scene][track] - 1;
    if (hi >= 0) {
        distances = D_800C3A50[scene][track].keyDistance;
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
    if (key != D_800C3A50[scene][track].segment) {
        func_15020878(track, key, scene);
    }
    linear = D_800C3868[scene][track][key].linear;
    func_1501FFE8((f32 *)points, D_800C3868[scene][track], key, D_800C35C8[scene][track]);
    if (linear == 0) {
        hi = 100;
        lo = 0;
        distances = D_800C3A50[scene][track].arcLength;
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
            t = (f32)(sample + i) * 0.01f;
            ((Vec3f *)out)[j].x = ((D_800C3A50[scene][track].coef3[0] * t + D_800C3A50[scene][track].coef2[0]) * t + D_800C3A50[scene][track].coef1[0]) * t + D_800C3A50[scene][track].coef0[0];
            ((Vec3f *)out)[j].y = ((D_800C3A50[scene][track].coef3[1] * t + D_800C3A50[scene][track].coef2[1]) * t + D_800C3A50[scene][track].coef1[1]) * t + D_800C3A50[scene][track].coef0[1];
            ((Vec3f *)out)[j].z = ((D_800C3A50[scene][track].coef3[2] * t + D_800C3A50[scene][track].coef2[2]) * t + D_800C3A50[scene][track].coef1[2]) * t + D_800C3A50[scene][track].coef0[2];
            j ^= 1;
        }
    } else {
        t = D_800C3A50[scene][track].keyDistance[key + 1] - D_800C3A50[scene][track].keyDistance[key];
        dist = dist - D_800C3A50[scene][track].keyDistance[key];
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
void func_1501FC8C(s32 track, f32 percent, f32 *x, f32 *y, f32 *z, s32 scene) {
    f32 dist;
    s32 segment;
    s32 sample;
    f32 positions[6];
    f32 *arc;
    f32 covered;
    f32 ratio;

    if (D_800C35C8[scene][track] >= 2) {
        if (percent > 100.0f) {
            percent = 100.0f;
        }
        if (percent < 0.0f) {
            percent = 0.0f;
        }
        dist = D_800C3A50[scene][track].length * percent * 0.009999999776f;
        sample = func_1501F72C(track, dist, positions, scene, &segment);
        if (D_800C3868[scene][track][segment].linear == 0) {
            arc = D_800C3A50[scene][track].arcLength + sample;
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

// matching: a second 0.01f literal would share FC8C's rodata entry; the ROM has one per function
extern f32 D_800969D8;

/* Heading in degrees of track `track`'s path at `percent` (0-100) of its length. */
void func_1501FE68(s32 track, f32 percent, f32 *heading, s32 scene) {
    f32 dx;
    f32 positions[6];
    s32 segment[3];
    f32 points[10];
    f32 dz;
    register f32 dist;

    if (D_800C35C8[scene][track] >= 2) {
        if (percent > 100.0f) {
            percent = 100.0f;
        }
        if (percent < 0.0f) {
            percent = 0.0f;
        }

        dist = D_800C3A50[scene][track].length * percent * D_800969D8;
        func_1501F72C(track, dist, positions, scene, segment);

        if (D_800C3868[scene][track][segment[0]].linear == 0) {
            dx = positions[0] - positions[3];
            dz = positions[2] - positions[5];
        } else {
            // matching: the 12-float buffer starts 2 floats below a 10-float local (frame size)
            func_1501FFE8(points - 2, D_800C3868[scene][track], segment[0], D_800C35C8[scene][track]);
            dx = points[4] - points[1];
            dz = points[6] - points[3];
        }
        *heading = func_150484A0(dx, dz) * 57.29577637f;
    }
}

/* Copy the path keys around segment `key` (key-1 .. key+2) into out[0..3] as float points. A
   missing key before or after the segment is replaced by a copy of the nearest end point. */
void func_1501FFE8(f32 *out, SeqPathKey *keys, s32 key, s32 numKeys) {
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
void func_15020388(s32 track, s32 scene) {
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

    if (D_800C35C8[scene][track] >= 2) {
        D_800C3A50[scene][track].segment = 0xFF;
        len = 0.0f;
        cur = 0;
        for (i = 0; i < D_800C35C8[scene][track] - 1; i++) {
            linear = D_800C3868[scene][track][i].linear;
            func_1501FFE8(&pts[0][0], D_800C3868[scene][track], i, D_800C35C8[scene][track]);
            D_800C3A50[scene][track].keyDistance[i] = len;
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
        D_800C3A50[scene][track].keyDistance[i] = len;
        D_800C3A50[scene][track].length = len;
        D_800C3A50[scene][track].segment = 0xFF;
    }
}

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

/* Cache the cubic and the 101-sample arc-length table of segment `segment` of track `track`. */
void func_15020878(s32 track, s32 segment, s32 scene) {
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

    if (segment >= D_800C35C8[scene][track] - 1) {
        return;
    }
    j = 0;
    total = D_800C3A50[scene][track].keyDistance[segment];
    linear = D_800C3868[scene][track][segment].linear;
    func_1501FFE8(points, D_800C3868[scene][track], segment, D_800C35C8[scene][track]);
    if (linear == 0) {
        for (i = 0; i < 3; i++) {
            a[i] = -0.5f * points[i] + 1.5f * points[i + 3] + -1.5f * points[i + 6] + points[i + 9] * 0.5f;
            b[i] = points[i] + -2.5f * points[i + 3] + (points[i + 6] + points[i + 6]) + points[i + 9] * -0.5f;
            c[i] = -0.5f * points[i] + points[i + 6] * 0.5f;
            d[i] = points[i + 3];
            D_800C3A50[scene][track].coef0[i] = d[i];
            D_800C3A50[scene][track].coef1[i] = c[i];
            D_800C3A50[scene][track].coef2[i] = b[i];
            D_800C3A50[scene][track].coef3[i] = a[i];
        }
        D_800C3A50[scene][track].arcLength[0] = total;
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
            D_800C3A50[scene][track].arcLength[i] = total;
            i++;
            j ^= 1;
        }
    } else {
        total = D_800C3A50[scene][track].keyDistance[segment + 1] - total;
        for (k = 0; k < 101; k++) {
            D_800C3A50[scene][track].arcLength[k] = (f32)k * total / 101.0f;
        }
    }
    D_800C3A50[scene][track].segment = segment;
}

Vec3f D_800C3620;
extern void func_15128CB0(Camera *);
extern void func_1517D5FC(s16, s16, s16, s32, s32, s32);
s32 func_15023264(s32 scene, s32 time);

// matching: going through a cast of &D_800C3600 changes IDO's address CSE in func_15020EC4
#define MSG (*(struct168 *)&D_800C3600)
// matching: byte-offset form of &D_800C35F0[a][i]; plain indexing swaps two s-registers here
#define ENT(a, i) ((SeqTrackInfo *)((u8 *)D_800C35F0[a] + (i) * 8))
#define ELEM(a, i) (&D_800C3958[a][i])

/* Apply the scene's evaluated tracks: the camera track (type 1, the one chosen by func_15023264)
   drives the player camera through func_1512D560 message 7, type 3 tracks drive actors, type 2
   tracks objects (func_1502178C) and type 5 tracks func_1517D5FC. Non-camera tracks go first. */
void func_15020EC4(s32 scene) {
    s32 track;
    s32 pass;
    Camera *player;
    SeqTrackState *state;
    WorldObject *actor;
    f32 tilt3C;
    f32 tilt40;

    // matching: indexing through &D_800C35EA[0] rather than the array changes register allocation
    if ((&D_800C35EA[0])[scene] == 1) {
        D_800C3638 = 0;
        if (D_800C35F8 == 0xFF) {
            D_800C35F8 = func_15023264(scene, D_800C35B0[scene]);
        }
        for (pass = 0; pass != 2; pass++) {
            for (track = 0; track < D_800C363A[scene]; track++) {
                if (D_800C35D0[scene][track] == 0) {
                    continue;
                }
                if (D_800C35C8[scene][track] == 0) {
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
                    player = D_800DBFF0;
                    if (track != D_800C35F8) {
                        continue;
                    }
                    if (!(player->mode & 0x40000)) {
                        if (D_800C3671 == 0) {
                            func_1512D560(player, 5, (void *)scene);
                        }
                        func_15128CB0(player);
                    }
                    state = ELEM(scene, track);
                    if (state->unk34 == -9999.0f) {
                        MSG.unk8 = 0.0f;
                    } else {
                        state->unk34 = (state->unk34 < -20.0f) ? -20.0f : (state->unk34 > 40.0f) ? 40.0f : state->unk34;
                        state = ELEM(scene, track);
                        MSG.unk8 = state->unk34;
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
                        MSG.unk4 = scene;
                    } else {
                        state->unk40 = (state->unk40 < -20.0f) ? -20.0f : (state->unk40 > 40.0f) ? 40.0f : state->unk40;
                        if (ENT(scene, track)) { } // matching: same, in this branch
                        state = ELEM(scene, track);
                        tilt40 = state->unk40;
                        MSG.unk4 = scene;
                    }
                    MSG.unk14 = state->x;
                    MSG.unk18 = state->y;
                    MSG.unk1C = state->z;
                    if (ENT(scene, track)->unk2 == 1) {
                        MSG.unk0 = 0;
                        MSG.unk20 = ELEM(scene, (s8)ENT(scene, track)->unk3)->x;
                        MSG.unk24 = ELEM(scene, (s8)ENT(scene, track)->unk3)->y + ENT(scene, track)->yOffset + (state = ELEM(scene, track))->yOffset;
                        MSG.unk28 = ELEM(scene, (s8)ENT(scene, track)->unk3)->z;
                        MSG.unk2C = -state->unk14;
                        MSG.unk30 = tilt3C;
                        MSG.unk34 = tilt40;
                    } else if (ENT(scene, track)->unk2 == 2) {
                        Actor *obj;

                        obj = func_15083E90(ENT(scene, track)->unk3);
                        if (obj == NULL) {
                            continue;
                        }
                        MSG.unk0 = 0;
                        D_800C3620.x = obj->x_position;
                        D_800C3620.y = obj->y_position + ENT(scene, track)->yOffset + (state = ELEM(scene, track))->yOffset;
                        D_800C3620.z = obj->z_position;
                        MSG.unk2C = -state->unk14;
                        MSG.unk30 = tilt3C;
                        MSG.unk34 = tilt40;
                    } else if (ENT(scene, track)->unk2 == 3) {
                        WorldObject *actor3;

                        actor3 = func_151149AC(ENT(scene, track)->unk3);
                        if (actor3 == NULL) {
                            continue;
                        }
                        MSG.unk0 = 0;
                        D_800C3620.x = actor3->x;
                        D_800C3620.y = (actor3->y + ENT(scene, track)->yOffset) + (state = ELEM(scene, track))->yOffset;
                        D_800C3620.z = actor3->z;
                        MSG.unk2C = -state->unk14;
                        MSG.unk30 = tilt3C;
                        MSG.unk34 = tilt40;
                    } else if (ENT(scene, track)->unk2 == 0) {
                        MSG.unk0 = 1;
                        MSG.unk20 = -state->unk14;
                        MSG.unk24 = state->unkC;
                        MSG.unk28 = state->unk10;
                        MSG.unk18 = MSG.unk18 + state->yOffset;
                        MSG.unk2C = tilt3C;
                        MSG.unk30 = tilt40;
                    }
                    if (D_800C3671 == 0) {
                        func_1512D560(player, 7, &MSG);
                    }
                } else if (ENT(scene, track)->type == 3) {
                    actor = func_151149AC(ENT(scene, track)->unk2);
                    if (actor == NULL) {
                        continue;
                    }
                    actor->attach = 0;
                    actor->viewFlags &= ~0x80;
                    actor->viewFlags = actor->viewFlags; // matching: forces the byte reload
                    actor->x = ELEM(scene, track)->x;
                    actor->y = ELEM(scene, track)->y + ELEM(scene, track)->yOffset;
                    actor->z = ELEM(scene, track)->z;
                    actor->rotX = ELEM(scene, track)->unkC;
                    actor->rotY = ELEM(scene, track)->unk10;
                    actor->rotZ = ELEM(scene, track)->unk14;
                    if (ELEM(scene, track)->unk18 != -9999.0f) {
                        actor->scaleX = ELEM(scene, track)->unk18 * 0.01f;
                    }
                    if (ELEM(scene, track)->unk1C != -9999.0f) {
                        actor->scaleY = ELEM(scene, track)->unk1C * 0.01f;
                    }
                    if (ELEM(scene, track)->unk20 != -9999.0f) {
                        actor->scaleZ = ELEM(scene, track)->unk20 * 0.01f;
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

                    obj2 = func_15083E90(ENT(scene, track)->unk2);
                    func_1502178C(obj2, scene, track);
                } else if (ENT(scene, track)->type == 4) {
                    s32 arr[3];

                    arr[0] = ENT(scene, track)->unk2;
                    arr[1] = ENT(scene, track)->unk2;
                    arr[2] = ENT(scene, track)->unk2;
                } else if (ENT(scene, track)->type == 5) {
                    state = ELEM(scene, track);
                    if (state->unk34 >= 1.0f) {
                        func_1517D5FC(state->x, state->y, state->z, 0, state->unk18, state->unk1C);
                    }
                }
            }
        }
        D_800C3638 = 1;
    }
}

extern void func_1510E7A4(void *, void *, f32 *, void *, void *, void *, f32, f32, f32, f32, u16, Actor *, f32, f32);

// matching: ((u8 *)&D_800D35E0)[-4] is D_800D35DC; a distinct base keeps IDO from reusing the hoisted address
extern f32 D_800D35E0;

/* Move object `target` to the state of the scene's object track bound to it (all tracks, or only
   `onlyTrack` unless it is -1): position, velocity, floor probe, scale, facing and alpha. */
void func_1502178C(Actor *target, s32 scene, s32 onlyTrack) {
    s32 track;
    f32 floorY;
    Actor *obj;
    s32 end;
    SeqTrackInfo *info;
    f32 angle;
    f32 oldPos[3];
    s32 start;
    s32 rescaled;

    if (D_800C35EA[scene] == 1) {
        D_800C3638 = 0;
        if (onlyTrack != -1) {
            start = onlyTrack;
            end = onlyTrack + 1;
        } else {
            start = 0;
            end = D_800C363A[scene];
        }
        for (track = start; track < end; track++) {
            if (D_800C35C8[scene][track] == 0) {
                continue;
            }
            info = &D_800C35F0[scene][track];
            if (info->type == 2) {
                obj = func_15083E90(info->unk2);
                if (obj != NULL && obj == target) {
                    oldPos[0] = obj->x_position;
                    oldPos[1] = obj->y_position;
                    oldPos[2] = obj->z_position;
                    obj->x_position = D_800C3958[scene][track].x;
                    obj->y_position = D_800C3958[scene][track].y + D_800C3958[scene][track].yOffset;
                    obj->z_position = D_800C3958[scene][track].z;
                    obj->unk1CC = obj->y_position;
                    if (D_800C3C88 == 0) {
                        obj->xz_velocity = (obj->x_position - oldPos[0]) * (obj->x_position - oldPos[0]) + (obj->z_position - oldPos[2]) * (obj->z_position - oldPos[2]);
                        if (obj->xz_velocity) {
                            obj->xz_velocity = sqrtf(obj->xz_velocity) * 2.0f;
                        }
                    } else {
                        obj->xz_velocity = 0.0f;
                    }
                    if (obj->unk31C != NULL) {
                        obj->unk31C->unk0 = obj->xz_velocity * 2.0f;
                    }
                    obj->unk54 = 0.0f;
                    obj->unkCC = 0;
                    rescaled = 0;
                    D_800D35DC = (target - D_800CC2D0) + 1;
                    func_1510E7A4(&obj->unk188, obj->floorTri.v, &floorY, &obj->unk118, &obj->unk184, &obj->floorType,
                                  obj->x_position, obj->y_position, obj->z_position, obj->y_position, 0, obj,
                                  -10000.0f, target->y_position);
                    ((u8 *)&D_800D35E0)[-4] = 0;
                    obj->floorY = floorY;
                    obj->unkD0 = 0;
                    obj->unk28 = obj->y_position - obj->floorY;
                    if (obj->interaction_state == 1) {
                        if (obj->unk118 != -10000.0f && obj->y_position < obj->unk118) {
                            obj->in_water = 1;
                        } else {
                            obj->in_water = 0;
                            obj->unkB2 = 0;
                        }
                    }
                    if (D_800C3958[scene][track].unk18 != -9999.0f) {
                        rescaled = 1;
                        obj->xz_scale = D_800C3958[scene][track].unk18 * 0.009999999776f;
                        obj->unk154 = D_800C3958[scene][track].unk18 * 0.009999999776f;
                    }
                    if (D_800C3958[scene][track].unk1C != -9999.0f) {
                        rescaled = 1;
                        obj->y_scale = D_800C3958[scene][track].unk1C * 0.009999999776f;
                        obj->unk158 = D_800C3958[scene][track].unk1C * 0.009999999776f;
                    }
                    if (rescaled) {
                        func_15062BDC(obj, obj->xz_scale, obj->y_scale);
                    }
                    obj->unkB8 = D_800C3958[scene][track].unkC;
                    angle = D_800C3958[scene][track].unk10 + 180.0f;
                    while (angle >= 360.0f) {
                        angle -= 360.0f;
                    }
                    while (angle < 0.0f) {
                        angle += 360.0f;
                    }
                    obj->unk76 = obj->unk7A = obj->unk78 = (s16)((u32)(angle * 182.0444489f) - 0x4000);
                    obj->unk40 = angle;
                    obj->unkC4 = D_800C3958[scene][track].unk14;
                    D_800C3958[scene][track].unk34 = (D_800C3958[scene][track].unk34 < 0.0f) ? 0.0f : (D_800C3958[scene][track].unk34 > 255.0f) ? 255.0f : D_800C3958[scene][track].unk34;
                    func_1506160C(obj, 1, D_800C3958[scene][track].unk34, 0, 0);
                    obj->old_x_position = obj->x_position;
                    obj->old_y_position = obj->y_position;
                    obj->old_z_position = obj->z_position;
                }
            }
        }
        D_800C3638 = 1;
    }
}

/* If object `objIndex` is bound to a track of `scene`, report where it should look: through the
   track's active events in D_800C3960 (slot 2: another track's position, slot 1: this track's
   unk24..unk2C or the player). Returns 1 when the out-angles were written, else 0. */
s32 func_15021DB8(f32 *outX, f32 *outY, f32 *outZ, f32 *target, s32 objIndex, s32 scene) {
    s32 track;
    u8 *active;
    Camera *player;
    f32 none;

    if (D_800C35EA[scene] != 1) {
        return 0;
    }
    for (track = 0; track < D_800C363A[scene]; track++) {
        if (D_800C35C8[scene][track] == 0) {
            continue;
        }
        if (D_800C35F0[scene][track].type != 2 ||
            D_800CC2D0[objIndex].unique_id != D_800C35F0[scene][track].unk2) {
            continue;
        }
        active = D_800C3960[scene][track];
        if (active[2] != 0xFF) {
            SeqEvent *event;
            s32 other;

            event = &D_800C3688[scene][track][active[2]];
            other = event->unk5;
            target[0] = D_800C3958[scene][other].x;
            target[1] = D_800C3958[scene][other].y + (f32)event->unk6;
            target[2] = D_800C3958[scene][other].z;
            return 0;
        }
        if (active[1] != 0xFF) {
            SeqEvent *event;

            event = &D_800C3688[scene][track][active[1]];
            if (event->unk5 == 0) {
                *outX = D_800C3958[scene][track].unk28;
                *outY = D_800C3958[scene][track].unk24;
                *outZ = D_800C3958[scene][track].unk2C;
                return 1;
            }
            player = D_800DBFF0;
            target[0] = player->eye.x;
            target[1] = player->eye.y + (f32)event->unk6;
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
s32 func_15022024(f32 *outA, f32 *outB, s32 objIndex, s32 scene) {
    s32 track;

    if (D_800C35EA[scene] != 1) {
        return 0;
    }
    for (track = 0; track < D_800C363A[scene]; track++) {
        if (D_800C35C8[scene][track] != 0) {
            if (D_800C35F0[scene][track].type == 2 && D_800CC2D0[objIndex].unique_id == D_800C35F0[scene][track].unk2) {
                outA[1] = 0.0f;
                outB[1] = 0.0f;
                outA[0] = 0.0f;
                outB[0] = 0.0f;
                if (D_800C3958[scene][track].unk3C != -9999.0f) {
                    outB[0] = D_800C3958[scene][track].unk3C;
                    outB[1] = outB[0];
                }
                if (D_800C3958[scene][track].unk40 != -9999.0f) {
                    outA[0] = D_800C3958[scene][track].unk40;
                    outA[1] = outA[0];
                }
                return 1;
            }
        }
    }
    return 0;
}

/* Set D_800C358C/D_800C3594 and raise D_800C3663. */
void func_15022190(s16 x, s16 y, s16 z, f32 value) {
    D_800C3594 = value;
    D_800C358C[0] = x;
    D_800C358C[1] = y;
    D_800C358C[2] = z;
    D_800C3663 = 1;
}

extern u8 D_800C3510[];
extern u8 D_800C3518[][25];
extern u8 D_800C354A[];
extern u8 D_800C3550[][30];

/* Set D_800C3598/D_800C35A0 (copied back to D_800C358C/D_800C3594 when a cutscene ends). */
void func_150221E8(s16 x, s16 y, s16 z, f32 value) {
    D_800C35A0 = value;
    D_800C3598[0] = x;
    D_800C3598[1] = y;
    D_800C3598[2] = z;
}

void func_15022234(s32 scene) {
    D_800C3510[scene] = 0;
}

void func_15022248(s32 scene) {
    D_800C354A[scene] = 0;
}

/* Add `id` to the scene's D_800C3518 list unless it is already there. */
void func_1502225C(s32 id, s32 scene) {
    s32 i;
    s32 count;
    s32 limit;
    u8 *countPtr;

    i = 0;
    countPtr = &D_800C3510[scene];
    count = *countPtr;
    limit = count;
    // matching: a do-while over a copy of the count (the for loop form schedules differently)
    if (count > 0) {
        do {
            if (id == D_800C3518[scene][i]) {
                return;
            }
            i++;
        } while (i < limit);
    }

    D_800C3518[scene][count] = id;
    *countPtr = count + 1;
}

/* Reset the animation state of the object bound to the scene's track `track`. */
void func_150222E0(s32 track, s32 scene) {
    Actor *obj;
    SeqTrackInfo *info;

    if (D_800C35C8[scene][track] != 0) {
        info = &D_800C35F0[scene][track];
        if (info->type == 2) {
            obj = func_15083E90(info->unk2);
            if (obj != NULL) {
                obj->eyeCode[0] = 0;
                obj->eyeCode[1] = 0;
                obj->eyeClosed[0] = 0;
                obj->eyeClosed[1] = 0;
                obj->unk282 = 0;
                obj->unk27A[0] = 0;
                obj->unk27A[1] = 0;
                obj->unk27E[0] = 0;
                obj->unk27E[1] = 0;
                func_1507E7E4(obj, 0, 3, 0xFFFF, 0xA);
                obj->expressionPriority = 0;
            }
        }
    }
}

/* Make sure the object bound to the scene's track `track` exists: spawn it from its D_800D20FC
   slot if needed and add it to the scene's D_800C3518 list. */
void func_15022398(s32 track, s32 scene) {
    Actor *obj;
    s32 slot;
    s32 id;

    if (D_800C35C8[scene][track] != 0) {
        if (D_800C35F0[scene][track].type == 2) {
            id = D_800C35F0[scene][track].unk2;
            slot = func_15083E0C(id);
            if (slot != -1) {
                obj = func_1505EEF4(slot);
                if (obj != NULL) {
                    if (obj->unk5 == 3) {
                        if (D_800C35F0 != &D_800C35F0[scene]) {
                            obj->unk5 = 4;
                        } else {
                            obj->unk5 = 0;
                        }
                    }
                    if (id == 1) {
                        obj->unkF4 &= -0x2B;
                    }
                } else {
                    if (slot < D_800D2100) {
                        func_15082A44((void *)((slot * 0x30) + (s32)D_800D20FC), slot, 0, scene, 0); // matching: integer address math
                        obj = func_1505EEF4(slot);
                        if (obj != NULL) {
                            func_1505E650(obj, 0, 0.0f, 0.0f, 0.0f, 0.0f, 1);
                            func_1502225C(obj - D_800CC2D0, scene);
                        }
                    }
                }
            }
        }
    }
}

/* Apply every object track of the scene to its object (func_1502178C). */
void func_15022528(s32 scene) {
    s32 track;
    Actor *obj;

    for (track = 0; track < D_800C363A[scene]; track++) {
        if (D_800C35D0[scene][track] == 0) {
            continue;
        }
        if (D_800C35C8[scene][track] == 0) {
            continue;
        }
        if (D_800C35F0[scene][track].type == 2) {
            obj = func_15083E90(D_800C35F0[scene][track].unk2);
            if (obj != NULL) {
                func_1502178C(obj, scene, track);
            }
        }
    }
}

/* Add `id` to the scene's D_800C3550 list unless it is already there. */
void func_15022640(s32 id, s32 scene) {
    s32 i;
    s32 count;
    s32 limit;
    u8 *countPtr;

    i = 0;
    countPtr = &D_800C354A[scene];
    count = *countPtr;
    limit = count;
    // matching: a do-while over a copy of the count, as in func_1502225C
    if (count > 0) {
        do {
            if (id == D_800C3550[scene][i]) {
                return;
            }
            i++;
        } while (i < limit);
    }

    D_800C3550[scene][count] = id;
    *countPtr = count + 1;
}

/* If the actor bound to the scene's track `track` is inactive, record its id
   (func_15022640) and clear the flag. */
void func_150226BC(s32 track, s32 scene) {
    WorldObject *actor;
    SeqTrackInfo *info;

    if (D_800C35C8[scene][track] != 0) {
        info = &D_800C35F0[scene][track];
        if (info->type == 3) {
            actor = func_151149AC(info->unk2);
            if (actor != NULL) {
                if (actor->inactive == 1) {
                    func_15022640(actor->id, scene);
                    actor->inactive = 0;
                }
            }
        }
    }
}

void func_150226BC(s32 track, s32 scene);

void func_15022754(s32 scene) {
    s32 track;

    for (track = 0; track < D_800C363A[scene]; track++) {
        func_150226BC(track, scene);
    }
}

/* Set inactive again on every actor recorded in the scene's D_800C3550 list. */
void func_150227BC(s32 scene) {
    s32 i;
    u8 *count;

    count = &D_800C354A[scene];
    for (i = 0; i < *count; i++) {
        func_151149AC(D_800C3550[scene][i])->inactive = 1;
    }
}

void func_15022398(s32 track, s32 scene);
void func_150222E0(s32 track, s32 scene);

void func_15022848(s32 scene) {
    s32 track;

    if (D_800C35EA[scene] == 1) {
        D_800C3638 = 0;
        for (track = 0; track < D_800C363A[scene]; track++) {
            func_15022398(track, scene);
            func_150222E0(track, scene);
        }
        D_800C3638 = 1;
    }
}

/* Call func_15060F28 on every live object in the scene's D_800C3518 list. */
void func_150228E4(s32 scene) {
    s32 i;
    s32 count;
    Actor *obj;
    u8 *countPtr;

    countPtr = &D_800C3510[scene];
    count = *countPtr;
    for (i = 0; i < count; i++) {
        obj = &D_800CC2D0[D_800C3518[scene][i]];
        if (obj->interaction_state != 0) {
            func_15060F28(obj, 0);
            count = *countPtr;
        }
    }
}

/* Run the D_80086014 handler of an object whose state (its first word) is 0x1B or 4. */
void func_15022998(s32 *obj) {
    if ((*obj == 0x1B) || (*obj == 4)) {
        if (D_80086014[*obj] != 0) {
            D_80086014[*obj](obj);
        }
    }
}

/* Whether `obj` is bound to an object track of its scene (scene 1 when obj->unk5 is 4): 1 when
   bound itself, 2 when bound through its parent (obj->unk65). */
s32 func_150229E4(Actor *obj) {
    s32 i;
    s32 scene;
    s32 parent;
    s32 id;

    scene = 0;
    if (obj == NULL) {
        return 0;
    }
    if (obj->interaction_state == 0) {
        return 0;
    }
    if (D_800C35C8[scene] == NULL) {
        return 0;
    }
    if (obj->unk5 == 4) {
        scene = 1;
    }
    parent = obj->unk65;
    if (parent != 0) {
        id = D_800CC2D0[parent - 1].unique_id;
    } else {
        id = obj->unique_id;
    }
    for (i = 0; i < D_800C363A[scene]; i++) {
        if (D_800C35C8[scene][i] != 0) {
            if (D_800C35F0[scene][i].type == 2 && id == D_800C35F0[scene][i].unk2) {
                if (parent == 0) {
                    return 1;
                }
                return 2;
            }
        }
    }

    return 0;
}

/* Whether actor `actorIndex` (D_800DBEF4[actorIndex].unk72) is bound to a track of the scene. */
s32 func_15022B08(s32 actorIndex, s32 scene) {
    s32 i;

    for (i = 0; i < D_800C363A[scene]; i++) {
        if (D_800C35C8[scene][i] != 0) {
            if (D_800C35F0[scene][i].unk2 == D_800DBEF4[actorIndex].id) {
                return 1;
            }
        }
    }

    return 0;
}

/* Re-anchor the scene's paths. A type 6 track shifts every path key so that its own path starts at
   its object (unk2 0) or actor (unk2 1). A type 7 track instead rotates the paths about its first
   key to the anchor D_800C358C/D_800C3594 and adjusts the heading channel (7) to match. */
void func_15022BA4(s32 scene) {
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

    for (track = 0; track < D_800C363A[scene]; track++) {
        if (D_800C35C8[scene][track] == 0) {
            continue;
        }
        if (D_800C35F0[scene][track].type == 6) {
            if (D_800C3663 == 0) {
                key = D_800C3868[scene][track];
                func_15022190(key->x, key->y, key->z, 0.0f);
            }
            if (D_800C35F0[scene][track].unk2 == 0) {
                Actor *obj;
                obj = func_15083E90(D_800C35F0[scene][track].unk3);
                if (obj == NULL) {
                    return;
                }
                x = obj->x_position;
                y = obj->y_position;
                z = obj->z_position;
            } else if (D_800C35F0[scene][track].unk2 == 1) {
                WorldObject *obj;
                // matching: the s8 id goes through the pointer variable before the call
                obj = (WorldObject *)(s8)D_800C35F0[scene][track].unk3;
                obj = func_151149AC((s32)obj);
                if (obj == NULL) {
                    return;
                }
                x = obj->x;
                y = obj->y;
                z = obj->z;
            } else {
                return;
            }
            key = D_800C3868[scene][track];
            dx = x - key->x;
            dy = y - key->y;
            dz = z - key->z;
            for (j = 0; j < D_800C363A[scene]; j++) {
                for (k = 0; k < D_800C35C8[scene][j]; k++) {
                    D_800C3868[scene][j][k].x = D_800C3868[scene][j][k].x + dx;
                    D_800C3868[scene][j][k].y = D_800C3868[scene][j][k].y + dy;
                    D_800C3868[scene][j][k].z = D_800C3868[scene][j][k].z + dz;
                }
            }
            return;
        } else if (D_800C35F0[scene][track].type == 7) {
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
            for (j = 0; j < D_800C35D0[scene][track]; j++) {
                chKey = &D_800C3778[scene][track][j];
                if (chKey->channel == 7) {
                    heading = chKey->value;
                    break;
                }
            }
            if (D_800C3663 == 0) {
                key = D_800C3868[scene][track];
                func_15022190(key->x, key->y, key->z, heading);
            }
            turn = D_800C3594 - heading;
            D_800C35A4 = turn;
            key = D_800C3868[scene][track];
            radians = turn * 0.01745329238f;
            x = key->x;
            y = key->y;
            z = key->z;
            c = cosf(radians);
            s = sinf(radians);
            dy = D_800C358C[1] - y;
            for (j = 0; j < D_800C363A[scene]; j++) {
                for (k = 0, hasHeadingEvent = 0; k < D_800C35C8[scene][j]; k++) {
                    rx = D_800C3868[scene][j][k].x - x;
                    rz = D_800C3868[scene][j][k].z - z;
                    nx = rx * c + rz * s;
                    rz = -rx * s + rz * c;
                    D_800C3868[scene][j][k].x = D_800C358C[0] + nx;
                    D_800C3868[scene][j][k].y = D_800C3868[scene][j][k].y + dy;
                    D_800C3868[scene][j][k].z = D_800C358C[2] + rz;
                }
                if (D_800C35D8[scene][j] != 0 && D_800C3688[scene][j][4].type == 7) {
                    hasHeadingEvent = 1;
                }
                if (hasHeadingEvent == 0) {
                    for (k = 0; k < D_800C35D0[scene][j]; k++) {
                        chKey = &D_800C3778[scene][j][k];
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
s32 func_15023264(s32 scene, s32 time) {
    s32 track;
    s32 best;
    s32 result;
    s32 score;
    SeqEvent *found;
    SeqEvent *ptr;

    result = -1;
    best = 99999;

    for (track = 0; track < D_800C363A[scene]; track++) {
        ptr = D_800C3688[scene][track];
        if (ptr != NULL) {
            found = NULL;
            score = func_15023BB0(&ptr[3], 3, track, &found, 0, 0, 0, 0, 0, 0, scene);
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
        for (track = 0; track < D_800C363A[scene]; track++) {
            if (D_800C35F0[scene][track].type == 1) {
                result = track;
                break;
            }
        }
    }

    return result;
}

/* Free all subtitle slots. */
void func_150233BC(void) {
    bzero(D_800C3CA0, sizeof(D_800C3CA0));
}

/* Close every open subtitle. */
void func_150233E4(void) {
    s32 i;
    s32 zero;
    SeqSubtitle *entry;

    i = 0;
    do {
        entry = &D_800C3CA0[i];
        if (entry->active != 0) {
            func_1516D2E0(entry->handle);
            entry->handle = 0;
            // matching: the cleared value goes through a local
            zero = 0;
            entry->active = zero;
        }
        i++;
    } while (&D_800C3D48 != &D_800C3CA0[i]);
}

/* Release subtitle slot `sub`: close it now if `force`, else let an open one finish (func_1516D328). */
void func_15023440(SeqSubtitle *sub, s32 force) {
    SeqSubtitle *entry;

    entry = sub;
    if (force != 0) {
        func_1516D2E0(entry->handle);
        entry->handle = 0;
    } else if (entry->isOpen != 0) {
        func_1516D328(entry->handle);
    } else {
        entry->handle = 0;
    }
    entry->active = 0;
}

/* Find the subtitle slot of `track`, or claim a free one (stealing slot 0 when all are busy). */
SeqSubtitle *func_150234A4(s32 track, s32 replaceOpen) {
    s32 i;
    SeqSubtitle *found;
    SeqSubtitle *slot;

    track++;
    found = NULL;
    slot = NULL;
    for (i = 0; i < 3; i++) {
        if (D_800C3CA0[i].active == track) {
            if (replaceOpen != 0 && D_800C3CA0[i].isOpen != 0) {
                slot = D_800C3CA0;
                func_15023440(slot, 1);
            } else {
                found = &D_800C3CA0[i];
            }
            break;
        }
        if (D_800C3CA0[i].active == 0) {
            slot = &D_800C3CA0[i];
            if (replaceOpen != 0 && slot->isOpen != 0) {
                func_15023440(slot, 1);
            }
            break;
        }
    }
    if (i == 3) {
        slot = D_800C3CA0;
        func_15023440(slot, 1);
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
s32 func_150235DC(s32 scene) {
    Actor *obj;
    ActorCtl *ctl;
    s8 zero;

    D_800CC2D0[0].unk25C &= ~0x200;
    if (D_800C35AA == 0) {
        obj = func_15083E90(1);
        if (obj != NULL) {
            D_800C3E78 = obj - D_800CC2D0;
            ctl = obj->unk31C;
            obj->xz_velocity = 0.0f;
            if (ctl != NULL) {
                ctl->unk0 = 0.0f;
            }
            obj->unk54 = 0.0f;
            obj->unkCC = 0;
            obj->unkD0 = 0;

            if ((-10000.0f != obj->unk118) && (obj->y_position < obj->unk118)) {
                obj->in_water = 1;
                obj->y_velocity = 0.0f;
                obj->gravity = 0.0f;
                if (((obj->unk118 - 50.0f) < obj->aim.y) || ((obj->unk31C->unk20 & 1) != 0)) {
                    obj->unkB2 = 0;
                    func_1508F060();
                }
            } else {
                obj->in_water = 0;
                obj->unkB2 = 0;
                obj->y_velocity = -4.0f;
                obj->y_position = obj->floorY;
                obj->unk1CC = obj->floorY;
                obj->gravity = 5.0f;
            }

            obj->unk81 = 0;
            obj->unk83 = 0;
            obj->disable_run = 0;
            obj->disable_jump = 0;
            obj->stunned = 0;
            obj->unk28 = obj->y_position - obj->floorY;
            D_800D154C = obj;
            func_1507F640();
            obj->unk31C->unk23 = 0;
            obj->unk287 = 0;
            zero = obj->unk287;
            obj->unk276 = 0;
            obj->unk282 = 0;
            obj->unk278 = 0;
            // matching: the (u8) casts keep the register allocation of the u8 view this was matched with
            obj->unk286 = (u8)zero;
            obj->unk285 = (u8)zero;
            obj->unk284 = (u8)zero;
            return zero;
        }
    }
}

extern void func_1504A730(void);
extern void func_1507E73C(Actor *);
extern void func_1507FEA0(Actor *);
extern void func_15061B4C(void);
void func_15022998(s32 *obj);
/* Per-frame update of the cutscene's objects (unless D_800C3654[0] hands it to func_1504A730): run
   each live object's handler and apply the track of objects bound to one. */
void func_1502378C(void) {
    s32 bound;
    s32 i;
    s32 limit;
    s32 one;
    Actor *obj;

    obj = D_800CC2D0;
    i = 0;
    if (D_800C3654[0] != 0) {
        func_1504A730();
        return;
    }
    // matching: 1 and 25 live in registers; the no-op `^ 0` and `& 0xFFFFFFFF` and the empty
    // do-while steer IDO's scheduling of them
    for (one = 1, limit = 25; i != (limit ^ 0); obj++) {
        if (obj->interaction_state != 0) {
            D_800C3E78 = i;
            func_15022998((s32 *)obj);
            func_1507E73C(obj);
            bound = func_150229E4(obj);
            if (bound != 0) {
                do { } while (0);
                func_1502178C(obj, 0, -1);
                func_150627D4(obj);
                func_1507FEA0(obj);
                obj->unk2FF = one & 0xFFFFFFFF;
            }
        }
        i++;
    }

    func_15061B4C();
}
