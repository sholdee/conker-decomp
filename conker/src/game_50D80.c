#include <ultra64.h>
#define func_1514D3B0 func_1514D3B0_hdr
#define func_15048A40 func_15048A40_void_proto
#define func_150ADA20 func_150ADA20_u8_proto
#include "functions.h"
#undef func_1514D3B0
#undef func_15048A40
#undef func_150ADA20
#define D_800C3958 D_800C3958_hdr
#include "variables.h"
#undef D_800C3958

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

/* Per-track state of a running cutscene (0x44 bytes); D_800C3958[scene] holds one per track. */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
} SeqTrackState;

extern SeqTrackState *D_800C3958[];

typedef struct {
    u16 unk0;
    u8 unk2;
    u8 pad3[5];
} Elem1502A8A0;

extern SeqQueueEntry *D_800C3D50;
extern u8 D_800C35E8[];
extern Elem1502A8A0 *D_800C35F0[];

extern struct131 *func_151149AC(s32);
extern s32 func_1502460C(SeqEvent *, s32, s32, s32, s32, struct127 *, struct131 *, f32 *, s32);
extern s32 func_150265CC(SeqEvent *, s32, s32, s32, s32, struct127 *, struct131 *, f32 *, s32);
extern s32 func_15029BB8(SeqEvent *, s32, s32, s32, s32, struct127 *, struct131 *, f32 *, s32, s32);
extern void func_1502A8A0(SeqEvent *, s32, s32, s32, s32);


void func_150238D0(void) {
}

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u8 unkE;
} Snd150238D8;

typedef struct {
    u8 pad0[2];
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u8 pad8[8];
    s32 unk10;
    u8 pad14[4];
    Snd150238D8 *unk18;
} Obj150238D8;

extern u8 D_800C3C89;
extern void func_10004074(void *);
extern s32 D_800C35B0[];
extern s32 D_800C3640[];

s32 func_150238D8(Obj150238D8 *arg0, s32 arg1, u32 *arg2, s32 arg3, s32 arg4, s32 arg5, s16 *arg6) {
    Snd150238D8 *s;
    SeqTrackState *p;
    SeqTrackState *q;
    s32 a;

    s = arg0->unk18;
    if ((s->unk2 != 0 && D_800C35B0[s->unk0] >= D_800C3640[s->unk0]) || (arg0->unk10 & 0x80)) {
        *arg2 = 0;
        *arg6 = 0;
        func_10004074(s);
        return 1;
    }
    if (s->unkE == D_800C35E8[s->unk0]) {
        p = D_800C3958[s->unk0];
        if (p != NULL) {
            arg0->unk2 = p[s->unk1].x;
            q = &D_800C3958[s->unk0][s->unk1];
            arg0->unk4 = q->unk38 + q->y;
            arg0->unk6 = D_800C3958[s->unk0][s->unk1].z;
        }
    }
    if (s->unk4 != 0xFFFF) {
        if (D_800BE9E4 < s->unk4) {
            s->unk4 -= D_800BE9E4;
        } else {
            *arg2 = 0;
            *arg6 = 0;
            func_10004074(s);
            return 1;
        }
    }
    *arg2 = s->unkC;
    a = s->unkA;
    if (s->unk4 < a) {
        if (s->unk4 != 0) {
            *arg2 = *arg2 * (u32)s->unk4 / a;
        }
    } else {
        if ((u32)s->unk8 != 0) {
            if ((s->unk6 - s->unk8) < s->unk4) {
                *arg2 = *arg2 * (s->unk6 - s->unk4) / s->unk8;
            }
        }
    }
    if (s->unk4 != 0xFFFF && D_800C3C88 != 0) {
        *arg2 = *arg2 * D_800C3C89 / 30;
        if (D_800BE9E4 >= D_800C3C89) {
            *arg2 = 0;
            *arg6 = 0;
            func_10004074(s);
            return 1;
        }
    }
    return 0;
}

extern SeqEvent *D_800C3688[][30];
extern u16 *D_800C35D8[];
extern s32 D_800C35B0[];
extern s32 D_800C3640[];

s32 func_15023BB0(SeqEvent *arg0, s32 arg1, s32 arg2, SeqEvent **arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10) {
    SeqEvent *p;
    s32 time;

    if (D_800C3688[arg10][arg2] != NULL) {
        time = D_800C3688[arg10][arg2]->value;
        p = D_800C3688[arg10][arg2] + 4;
        while ((u32) (p - D_800C3688[arg10][arg2]) < D_800C35D8[arg10][arg2]) {
            if ((p->delta == 0) && (p->type == 0)) {
                time = p->value;
            } else {
                if ((arg0 < p) && (arg1 == p->type) && ((arg4 == 0) || ((arg4 == 1) && (arg5 == p->value))) && ((arg6 == 0) || ((arg6 == 1) && (arg7 == p->unk4))) && ((arg8 == 0) || ((arg8 == 1) && (arg9 == p->unk5)))) {
                    *arg3 = p;
                    return time;
                }
                while (p->delta == 0) {
                    if (p->type == 0) {
                        time = p->value;
                        break;
                    }
                    p++;
                    if ((u32) (p - D_800C3688[arg10][arg2]) >= D_800C35D8[arg10][arg2]) {
                        break;
                    }
                    if ((arg0 < p) && (arg1 == p->type) && ((arg4 == 0) || ((arg4 == 1) && (arg5 == p->value))) && ((arg6 == 0) || ((arg6 == 1) && (arg7 == p->unk4))) && ((arg8 == 0) || ((arg8 == 1) && (arg9 == p->unk5)))) {
                        *arg3 = p;
                        return time;
                    }
                }
                time += p->delta;
            }
            p++;
        }
    }
    *arg3 = NULL;
    return D_800C3640[arg10];
}


void func_15023DE0(SeqEvent **arg0, SeqEvent **arg1, s32 *arg2, s32 *arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11) {
    SeqEvent *p;
    s32 time;
    s32 curTime;

    if (arg0 != NULL) {
        *arg0 = NULL;
    }
    *arg1 = NULL;
    if (arg2 != NULL) {
        *arg2 = 0;
    }
    *arg3 = D_800C3640[arg11];

    if (D_800C3688[arg11][arg4] == NULL) {
        return;
    }

    p = D_800C3688[arg11][arg4] + 4;
    time = D_800C3688[arg11][arg4]->value;
    while ((u32) (p - D_800C3688[arg11][arg4]) < D_800C35D8[arg11][arg4]) {
        if ((p->delta == 0) && (p->type == 0)) {
            time = p->value;
        } else {
            if (time <= arg10) {
                if ((arg5 == p->type) && ((arg6 == 0) || ((arg6 == 1) && (arg7 == p->value))) && ((arg8 == 0) || ((arg8 == 1) && (arg9 == p->unk4)))) {
                    if (arg0 != NULL) {
                        *arg0 = p;
                    }
                    if (arg2 != NULL) {
                        *arg2 = time;
                    }
                }
            }
            if (time > arg10) {
                if ((arg5 == p->type) && ((arg6 == 0) || ((arg6 == 1) && (arg7 == p->value))) && ((arg8 == 0) || ((arg8 == 1) && (arg9 == p->unk4)))) {
                    *arg1 = p;
                    *arg3 = time;
                    return;
                }
            }
            while (p->delta == 0) {
                if (p->type == 0) {
                    time = p->value;
                    break;
                }
                p++;
                if ((u32) (p - D_800C3688[arg11][arg4]) >= D_800C35D8[arg11][arg4]) {
                    break;
                }
                curTime = D_800C35B0[arg11];
                if (time <= curTime) {
                    if ((arg5 == p->type) && ((arg6 == 0) || ((arg6 == 1) && (arg7 == p->value))) && ((arg8 == 0) || ((arg8 == 1) && (arg9 == p->unk4)))) {
                        if (arg0 != NULL) {
                            *arg0 = p;
                        }
                        if (arg2 != NULL) {
                            *arg2 = time;
                        }
                        continue;
                    }
                }
                if (time > curTime) {
                    if ((arg5 == p->type) && ((arg6 == 0) || ((arg6 == 1) && (arg7 == p->value))) && ((arg8 == 0) || ((arg8 == 1) && (arg9 == p->unk4)))) {
                        *arg1 = p;
                        *arg3 = time;
                        return;
                    }
                }
            }
            time += p->delta;
        }
        p++;
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

extern s32 D_800C35B8[];
extern u8 D_800C35C0[];
extern u8 D_800C3C8C;
extern u8 D_800C363A[];

void func_150242F8(s32 arg0, s32 arg1) {
    s32 i;
    SeqEvent *p;
    s32 time;
    s32 count;

    if ((D_800C3C8C != 2) && ((&D_800C35EA)[arg1] == 1) && (D_800C3C9A != 0)) {
        D_800C3638 = 0;
        count = 0;
        for (i = 0; i < D_800C363A[arg1]; i++) {
            if (D_800C3688[arg1][i] != NULL) {
                extern s32 D_800C35B8[];
                if (D_800C35B8[arg1] != D_800C35B0[arg1]) {
                    p = D_800C3688[arg1][i] + 4;
                    time = D_800C3688[arg1][i]->value;
                    while ((u32) (p - D_800C3688[arg1][i]) < D_800C35D8[arg1][i]) {
                        if ((p->delta == 0) && (p->type == 0)) {
                            time = p->value;
                        } else {
                            if ((time <= D_800C35B0[arg1]) && (time > D_800C35B8[arg1])) {
                                for (;;) {
                                    if ((u32) (p - D_800C3688[arg1][i]) >= D_800C35D8[arg1][i]) {
                                        break;
                                    }
                                    if (((arg0 == 0) && (p->type != 0x11)) || ((arg0 == 1) && (p->type == 0x11))) {
                                        func_150241B4(D_800C3D50, &count, p, i, (u32) (p - D_800C3688[arg1][i]), time, arg1);
                                    }
                                    if ((p->delta != 0) || ((p->delta == 0) && (p->type == 0))) {
                                        break;
                                    } else {
                                        p++;
                                        continue;
                                    }
                                }
                            }
                            if (time > D_800C35B0[arg1]) {
                                break;
                            }
                            while (p->delta == 0) {
                                if (p->type == 0) {
                                    time = p->value;
                                    break;
                                }
                                p++;
                                if ((u32) (p - D_800C3688[arg1][i]) >= D_800C35D8[arg1][i]) {
                                    break;
                                }
                            }
                            time += p->delta;
                        }
                        p++;
                    }
                }
            }
        }
        func_15024130(count, arg1);
        D_800C3638 = 1;
        if (arg0 == 0) {
            D_800C35C0[arg1] = 1;
        }
    }
}

typedef struct {
    s16 active;
    u8 pad2[2];
    s32 flags;
    u8 *text;
    u8 flagC;
    u8 padD[0xF];
    f32 *unk1C;
    f32 *unk20;
    f32 *unk24;
    s16 unk28;
    s16 unk2A;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 pad30[4];
    s32 handle;
} Rec1502460C;

typedef struct {
    struct127 *obj;
    u8 id;
} ObjRef1502460C;

#define FU8(p, o) (*(u8 *) ((u8 *) (p) + (o)))
#define FS8(p, o) (*(s8 *) ((u8 *) (p) + (o)))
#define FS16(p, o) (*(s16 *) ((u8 *) (p) + (o)))
#define FS32(p, o) (*(s32 *) ((u8 *) (p) + (o)))
#define FF32(p, o) (*(f32 *) ((u8 *) (p) + (o)))
#define FPTR(p, o) (*(void **) ((u8 *) (p) + (o)))

extern s8 D_800E0BF4;
extern u8 **D_800C35E0[];
extern s32 D_80090214[];
extern u8 D_800C363A[];
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
extern u8 D_800C3C8C;
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
extern s32 func_1514D3B0(void *, s16, s32, s32);
extern void func_15023440(Rec1502460C *, s32);
extern Rec1502460C *func_150234A4(s32, s32);
extern s32 func_15052F9C(struct127 *, f32, s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_15053430(struct127 *);
extern void func_1507E7E4(void *, s32, s32, s32, s32);
extern void func_1507EB4C(void *, s32);
extern void func_150836CC(struct127 *, s32);
extern s32 func_15083FB0(s32);
extern void func_150B648C(u8);
extern void func_150B7220(s32, s32, s32, s32);
extern void func_150C5C74(s32);
extern void func_150C5C9C(s32);
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
extern s32 func_1518D1C0(struct127 *, s16, s16, s32, u8, s32, char *);
extern void func_1518F45C(s32, u8);
extern s32 func_1518F51C(void *, u8, s16, s8, s8, s8, u8, s32, s32, s32);
extern u8 *func_1519003C(struct127 *, s32 *);
extern void func_15190518(void *);
extern void func_15191B8C(ObjRef1502460C *, u8);
extern void func_151937F4(struct127 *, s32, s32);
extern s32 func_1519D030(void *, s32, s32, s32, s32, s32);
extern s32 func_151A4FD0(s32, s32, s32, s32, s32, s32, u8, s32);
extern void func_151A9390(s32, s32, s32, f32 *, f32, f32, s32, s32, s32);
extern void func_151BE850(f32 *, f32, s32, s32, s32, s32, s32);
extern void func_151C970C(s32, s32);
extern void func_151C9DE8(void *, u8, s8, u8, s32);
extern void func_151C9ED4(struct108 *);
extern void func_151CD35C(s32);
extern void func_151CD394(s32);
extern void func_151DE7D4(void);
extern void func_151E53E8(s32);

s32 func_1502460C(SeqEvent *event, s32 arg1, s32 arg2, s32 arg3, s32 arg4, struct127 *arg5, struct131 *arg6, f32 *arg7, s32 arg8) {

    if (D_800E0BF4 != 0 && (event->type == 7 || event->type == 0xE || event->type == 0xF || event->type == 0x10 || event->type == 0x12)) {
        s32 flag;
        Rec1502460C *rec;
        u8 *text;
        u8 colors[3][3] = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xA0 }, { 0x3C, 0x3C, 0x3C } };

        if ((event->type == 7 && D_800BEAAB != 0) || (event->type == 0xE && D_800BEAAB != 4) ||
            (event->type == 0xF && D_800BEAAB != 2) || (event->type == 0x10 && D_800BEAAB != 3) ||
            (event->type == 0x12 && D_800BEAAB != 1)) {
            return 1;
        }
        flag = event->unk4 == 0 ? 1 : 0;
        rec = func_150234A4(arg1, flag);
        if (rec != NULL) {
            if (event->unk4 == 0) {
                if (rec->flagC != 0) {
                    func_1516D2E0(rec->handle);
                }
                text = D_800C35E0[arg4][event->value];
                if (*text == 0xBE) {
                    rec->flags = 0x8013;
                    text++;
                } else if (*text == 0xBF) {
                    rec->flags = 0x8019;
                    text++;
                } else {
                    rec->flags = 0x8011;
                }
                rec->text = text;
                rec->unk2A = 100;
                rec->flagC = 1;
                rec->unk28 = 0;
                rec->unk1C = &D_800C3958[arg4][arg1].unkC;
                rec->unk20 = &D_800C3958[arg4][arg1].unk10;
                rec->unk2F = 1;
                rec->unk24 = &D_800C3958[arg4][arg1].unk14;
                rec->unk2C = colors[event->unk5][0];
                rec->unk2D = colors[event->unk5][1];
                rec->unk2E = colors[event->unk5][2];
                if (event->unk5 == 2) {
                    rec->flags |= 0x40000;
                }
                rec->handle = func_1516A7B0(0, 0, 0, 0, &rec->flags, arg8);
            } else if (event->unk4 == 1) {
                func_15023440(rec, event->unk5);
            } else {
                if (event->unk5 == 0) {
                    rec->handle = func_1516D378(rec->handle);
                } else {
                    rec->handle = func_1516D3C4(rec->handle);
                }
                if (event->unk6 != 0) {
                    s32 idx = event->unk6 - 1;

                    rec->unk2C = colors[idx][0];
                    rec->unk2D = colors[idx][1];
                    rec->unk2E = colors[idx][2];
                    if (idx == 2) {
                        rec->flags |= 0x40000;
                    } else {
                        rec->flags &= ~0x40000;
                    }
                }
            }
        }
    } else if (event->type == 0xC) {
        SeqEvent *p;
        s32 val;

        if (event->value != 0) {
            if (event->unk6 != 0) {
                func_1507EB4C(arg5, event->value);
            } else {
                if (event->unk5 != 0) {
                    val = 0xFFFF;
                } else {
                    val = func_15023BB0(event, event->type, arg1, &p, 0, 0, 0, 0, 0, 0, arg4) - arg3;
                }
                func_1507E7E4(arg5, event->value & 0xFF, 3, val & 0xFFFF, event->unk4);
            }
        }
    } else if (event->type == 0xD) {
        struct126 *s;

        if (arg5 == NULL) {
            return 1;
        }
        if (event->unk4 == 0) {
            func_15083568(arg5, event->value + 1, 1.0f, 0);
        } else if (event->unk4 == 1) {
            func_150836CC(arg5, event->value + 1);
        } else {
            s = func_1503195C(arg5, event->value + 1, 0);
            if (s != NULL) {
                if (event->unk4 == 2) {
                    FS16(s, 0x18) = D_80090214[event->unk5];
                } else if (event->unk4 == 3) {
                    FS16(s, 0x1A) = D_80090214[event->unk5];
                }
            }
        }
    } else if (event->type == 4) {
        SeqEvent *p;
        s32 val;

        if (arg5 == NULL) {
            return 1;
        }
        val = (func_15023BB0(event, event->type, arg1, &p, 0, 0, 0, 0, 0, 0, arg4) - arg3) / 2;
        if (val >= 0x100) {
            val = 0xFF;
        } else if (val == 0) {
            val = 1;
        }
        if (event->unk4 == 0) {
            FU8(arg5, 0x134) = event->value;
            FU8(arg5, 0x135) = val;
        }
    } else if (event->type == 3) {
        if (event->value >= D_800C363A[arg4]) {
            return 1;
        }
        if (D_800C35F0[arg4][event->value].unk0 == 1 && D_800C35F8 != event->value) {
            D_800C35F8 = event->value;
            D_800DBFF4[0] = 4;
            D_800DBFF0->unk5F0 |= 4;
        }
    } else if (event->type == 9) {
        if (arg5 == NULL) {
            return 1;
        }
        ((u8 *) arg5 + 0x6C)[event->unk4] = event->value + 10;
    } else if (event->type == 1) {
        s32 v;

        if (arg5 == NULL) {
            return 1;
        }
        v = event->value;
        if (v == 0) {
            v = 0xFF;
        }
        FU8(arg5, 0x1C9) = v;
    } else if (event->type == 5) {
        if (event->value == 0x3B) {
            if (event->unk4 == 0) {
                D_800DDE18 = (s32) &D_800C3958[arg4][arg1].unk28;
            } else {
                D_800DDE18 = 0;
            }
        } else if (event->value == 8) {
            D_800C3656[arg4] = !event->unk4;
        } else if (event->value == 9) {
            D_800C3658[arg4] = !event->unk4;
        } else if (event->value == 0xD) {
            ((u8 *) &D_800C3654)[arg4] = !event->unk4;
        } else if (event->value == 0x12) {
            D_800C365A[arg4] = !event->unk4;
        } else if (event->value == 0x13) {
            D_800C365C[arg4] = event->unk4;
        } else if (event->value == 0x15) {
            D_800C365E[arg4] = !event->unk4;
        } else if (event->value == 0x14) {
            if (arg5 == NULL) {
                return 1;
            }
            if (event->unk4 == 0) {
                func_15053430(arg5);
            } else {
                func_15060F28(arg5, 0);
            }
        } else if (event->value == 0x41) {
            u8 colors[2][3] = { { 0xFF, 0, 0 }, { 0, 0, 0 } };

            if (arg5 == NULL) {
                return 1;
            }
            if (event->unk4 == 0) {
                FU8(arg5, 0xA5) = colors[event->unk5][0];
                FU8(arg5, 0xA6) = colors[event->unk5][1];
                FU8(arg5, 0xA7) = colors[event->unk5][2];
                FU8(arg5, 0xA4) = 1;
                FPTR(arg5, 0xA0) = &D_800C3958[arg4][arg1].unk30;
            } else {
                FU8(arg5, 0xA4) = 0;
                FPTR(arg5, 0xA0) = NULL;
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
            if (arg5 == NULL) {
                return 1;
            }
            FU8(arg5, 0x1FF) = !event->unk4;
            if (FU8(arg5, 0x1FF) != 0) {
                FU8(arg5, 0x1FF) += event->unk5;
            }
            if (event->unk4 == 0) {
                FU8(arg5, 0x201) = event->unk6;
            }
        } else if (event->value == 0x16) {
            if (arg5 == NULL) {
                return 1;
            }
            FU8(arg5, 0x206) = !event->unk4;
            if (FU8(arg5, 0x206) != 0) {
                FU8(arg5, 0x206) += event->unk5;
            }
            if (event->unk4 == 0) {
                FU8(arg5, 0x208) = event->unk6;
            }
        } else if (event->value == 0x18) {
        } else if (event->value == 0x1D) {
            if (D_800C35F0[arg4][arg1].unk0 == 3 && arg6 != NULL) {
                if (event->unk4 == 0) {
                    FU8(arg6, 0x6E) = 1;
                } else {
                    FU8(arg6, 0x6E) = 0;
                }
            }
        } else if (event->value == 0x1E) {
            D_800C35C2 = event->unk4 + 1;
            D_800C35C3 = D_800BE9F0;
        } else if (event->value == 3) {
            struct131 *actor = func_151149AC(FU8(event, 7));

            if (actor == NULL) {
                return 1;
            }
            FU8(actor, 0x73) &= ~3;
            if (event->unk5 == 0) {
                FU8(actor, 0x73) |= 2;
            } else {
                FU8(actor, 0x73) |= 1;
            }
        } else if (event->value == 0x6C) {
            f32 pos[3];
            f32 pos2[3];

            switch (FU8(event, 4)) {
                case 0:
                    pos[0] = D_800C3958[arg4][arg1].x;
                    pos[1] = D_800C3958[arg4][arg1].y;
                    pos[2] = D_800C3958[arg4][arg1].z;
                    func_151BE850(pos, D_800C3958[arg4][arg1].unk10, 1, 0, 2, 0xFF, 1);
                    break;
                case 1:
                    pos2[0] = D_800C3958[arg4][arg1].x;
                    pos2[1] = D_800C3958[arg4][arg1].y;
                    pos2[2] = D_800C3958[arg4][arg1].z;
                    func_151A9390(5, 1, 0, pos2, -10000.0f, 100.0f, -1, arg8, 1);
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
                        func_151C970C(2, (s32) &(*(struct178 **) &D_800D3098)[ids[i]]);
                    }
                    break;
                }
                case 10:
                    func_151C9DE8(D_800DBFF0, 0, arg1, arg8, 1);
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
                    if (arg5 != NULL) {
                        func_15190518(arg5);
                        func_1518F45C(0, 0x49);
                    }
                    break;
                case 21:
                    func_151494E0(0, event->unk5 != 0 ? 0x55 : 0x56);
                    break;
                case 22:
                    if (arg5 != NULL) {
                        func_151937F4(arg5, 0xFF, 1);
                    }
                    break;
                case 23:
                    if (arg5 != NULL) {
                        ObjRef1502460C ref;

                        ref.obj = arg5;
                        ref.id = arg5->unique_id;
                        func_15191B8C(&ref, 0x50);
                    }
                    break;
            }
        } else if (event->value == 0x6E) {
            u8 *list;
            s32 count;

            list = func_1519003C(arg5, &count);
            if (list != NULL && event->unk6 < count) {
                func_1518F51C(arg5, list[event->unk6], -1, -1, -1, -1, 0, 0, 0xFF, 1);
            }
        } else if (event->value == 5) {
            s32 idx;
            SeqEvent *p;
            s32 val;
            s32 ids[4] = { 0xD, 0xE, 0xF, 0x10 };
            u8 c;

            if (event->unk4 == 0) {
                idx = event->unk5;
                if (idx >= 4) {
                    idx = 3;
                }
                val = func_15023BB0(event, event->type, arg1, &p, 1, event->value, 0, 0, 1, event->unk5, arg4) - arg3;
                if (event->unk6 != 0) {
                    val = 999;
                    c = 0xFF;
                } else {
                    c = arg8;
                }
                func_1518D1C0(arg5, ids[idx], val, 0, c, 0, "Cutscene 1");
            }
        } else if (event->value == 0x36) {
            if (event->unk4 == 0) {
                func_1519D030(arg5, 0, 300, 0, arg8, 0);
            } else if (event->unk4 == 1) {
                ObjRef1502460C ref;

                ref.obj = arg5;
                ref.id = arg5->unique_id;
                func_15147D64(&ref, 7);
            }
        } else if (event->value == 0x30) {
            u8 v;

            if (arg5 == NULL) {
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
                func_1514D3B0(arg5, 0xD, v, 0);
            }
        } else if (event->value == 0x46) {
            if (event->unk4 != 0) {
                func_150C5C74((s32) arg5);
            } else {
                func_150C5C9C((s32) arg5);
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

            if (arg5 != NULL) {
                v = event->unk4;
                v <<= 16;
                v |= event->unk6;
                if (event->unk5 == 0) {
                    arg5->unk94 = v;
                } else if (event->unk5 == 1) {
                    arg5->unk94 |= v;
                } else {
                    arg5->unk94 &= ~(v | 1);
                }
            }
        } else if (event->value == 0x3D) {
            s32 r;

            if (arg5 != NULL) {
                if (event->unk4 == 0) {
                    r = func_15083FB0(FU8(event, 7));
                    if (r != -1) {
                        arg5->unk101 |= 4;
                        arg5->unk65 = r + 1;
                        arg5->unk5C = event->unk5;
                    }
                } else {
                    arg5->unk65 = 0;
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
            s32 time;
            SeqEvent *p;

            if (event->unk4 == 0) {
                time = func_15023BB0(event, event->type, arg1, &p, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                func_150B7220(D_800C3958[arg4][arg1].unk24, D_800C3958[arg4][arg1].unk28, time, arg8);
            }
        } else if (event->value == 0x45) {
            s32 time;
            SeqEvent *p;

            if (event->unk4 == 0) {
                time = func_15023BB0(event, event->type, arg1, &p, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                func_151A4FD0(0, time, 0xFF, 0, 0, D_800BE9E8, 0, 0);
            }
        } else if (event->value == 0x49) {
            if (arg5 != NULL && event->unk4 < 3) {
                ((s32 *) ((u8 *) arg5 + 0x2E4))[event->unk4] = event->unk6;
            }
        } else if (event->value == 0x4B) {
            if (arg5 != NULL) {
                arg5->unk9C = (event->unk6 & 0xFFFF) | ((event->unk5 & 0xFF) << 16) | ((event->unk4 & 0xFF) << 24);
            }
        } else if (event->value == 0x4A) {
            if (event->unk4 == 0) {
                func_10011FB0(event->unk5);
            } else {
                func_10011FB0(0);
            }
        } else if (event->value == 0x54) {
            D_800C3660[arg4] = ~event->unk4;
        } else if (event->value == 0x55) {
            func_150C851C(event->unk4);
        } else if (event->value == 0x59) {
            D_800C3C8B = 1;
        } else if (event->value == 0x5A) {
            D_800C3E78 = arg5 - D_800CC2D0;
            if (arg5->id == 0x53) {
                arg5->unk124 = 0;
                func_15052F9C(arg5, 156.0f, 1, 4, 0, 10, 0xFF, 0, 1, 0);
            } else if (arg5->id == 0x28) {
                arg5->unk124 = 0;
                func_15052F9C(arg5, 180.0f, 5, 0x44, 1, 1, 0xFF, 0, 1, 0);
            } else if (arg5->id == 0x21) {
                arg5->unk124 = 0;
                func_15052F9C(arg5, 156.0f, 0x19, 4, 0, 0xF, 0xFF, 0, 1, 0);
            }
        } else if (event->value == 0x71) {
            D_800CC2D0->unkCC = 0;
            FU8(D_800CC2D0->unk31C, 0x4E) = 0;
            D_800CC2D0->unk65 = 0;
            FU8(D_800CC2D0->unk31C, 0x27) = 0;
            D_800CC2D0->unkF8 &= ~0x01000000;
            if (arg5 != NULL) {
                arg5->unk13C = 0;
            }
        } else if (event->value == 0x37) {
            s32 val;
            SeqEvent *p;
            s32 v;
            u8 colors[3][3] = { { 0xFF, 0, 0 }, { 0, 0x35, 0x46 }, { 0xFF, 0xFF, 0xFF } };

            val = func_15023BB0(event, event->type, arg1, &p, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
v = event->unk6 != 0 ? event->unk6 : 0x7F;
                func_1517F488(colors[event->unk5][0], colors[event->unk5][1], colors[event->unk5][2], v, val, 0);
            } else {
                D_800DDE10 = 0;
            }
        } else if (event->value == 0x38) {
            s32 val;
            SeqEvent *p;
            s32 v;

            val = func_15023BB0(event, event->type, arg1, &p, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
v = event->unk6 != 0 ? event->unk6 : 4;
                func_1517F720(event->unk5, val, v, 0x7F, 1);
            } else {
                D_800DDE08 = 0;
            }
        } else if (event->value == 0x5C) {
            SeqEvent *p;
            s32 total;
            s32 start;
            s32 end;

            if (event->unk4 == 0) {
                total = func_15023BB0(event, event->type, arg1, &p, 1, event->value, 1, 1, 1, event->unk5, arg4);
                start = func_15023BB0(event, event->type, arg1, &p, 1, event->value, 1, 2, 1, event->unk5, arg4) - arg3;
                if (p == NULL) {
                    start = 0;
                }
                end = total - func_15023BB0(event, event->type, arg1, &p, 1, event->value, 1, 3, 1, event->unk5, arg4);
                if (p == NULL) {
                    end = 0;
                }
                if (D_800BE9F0 == 0x18) {
                    func_150F5420(start, total - arg3 - end - start, end, event->unk5);
                } else if (D_800BE9F0 == 0x21) {
                    func_1510A930(start, total - arg3 - end - start, end);
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
                D_800DDE44 = &D_800C3958[arg4][arg1].unk28;
                D_800DDE48 = &D_800C3958[arg4][arg1].unk24;
                D_800DDE4C = &D_800C3958[arg4][arg1].unk2C;
            } else {
                D_800DDE40 = 0;
            }
        } else if (event->value == 0x66) {
            if (event->unk4 == 0) {
            }
        } else if (event->value == 0x67) {
            func_151DE7D4();
        } else if (event->value == 0x73) {
            func_151E53E8(0);
        } else if (event->value == 0x69) {
            if (arg5 != NULL) {
                if (event->unk4 == 0) {
                    arg5->gravity = 4.0f;
                    arg5->y_velocity = 20.0f;
                } else if (event->unk4 == 1) {
                    arg5->gravity = 0.0f;
                    arg5->y_velocity = 0.0f;
                } else if (event->unk4 == 2) {
                    func_150836CC(arg5, 0x11);
                    arg5->unkA9 = 0;
                } else if (event->unk4 == 3) {
                    D_800C3E78 = arg5 - D_800CC2D0;
                    D_800CC2D0->unk13C = D_800C3E78 + 100;
                    D_800CC2D0->xz_velocity = 0.0f;
                    FS16(D_800CC2D0->unk31C, 0x18) = 0;
                    func_1505959C(arg5, 0);
                    *(u32 *) &arg5->unk5C = 9;
                    arg5->unk65 = 1;
                    arg5->unk101 |= 4;
                } else if (event->unk4 == 4) {
                    FU8(D_800CC2D0->unk31C, 0x17) = 1;
                } else if (event->unk4 == 5) {
                    D_800D154C = arg5;
                    func_15073F1C();
                } else if (event->unk4 == 6) {
                    FU8(arg5->unk31C, 0x4E) = 0;
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
            if (arg5 != NULL) {
                ((u8 *) arg5 + 0x68)[event->unk4] = event->unk5;
            }
        } else if (event->value == 0x70) {
            D_800C3C8C = 1;
        } else if (event->value == 0x72) {
            if (arg5 != NULL) {
                FF32(arg5, 0xC8) = FS16(D_800D1C90[arg5->id], 6);
            }
        } else if (event->value == 0x78) {
            D_800C3C8D = 1;
        } else if (event->value == 0x79) {
            if (arg6 != NULL) {
                FS32(arg6, 0x84) = 0;
                FS32(arg6, 0x80) = 0;
                FS32(arg6, 0x7C) = 0;
            }
        } else if (event->value == 0x7B) {
            if (D_800BE9F0 == 0x1D) {
                D_800C3680 = 1;
            }
        } else if (event->value == 0x7D) {
            if (arg5 != NULL && event->unk4 == 0) {
                arg5->unk5 = 1;
            }
        } else if (event->value == 0x7E) {
            if (arg5 != NULL) {
                D_800D18D0 = arg5 - D_800CC2D0;
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

typedef struct {
    f32 unk0;
    f32 unk4;
    u8 pad8[0x15];
    u8 unk1D;
    u8 pad1E[6];
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
    struct127 *unk0;
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
extern void func_150E2DB4(struct127 *, u8, s16, s32, f32, f32, f32, f32, f32, f32, s32, s32, s32, s32);
extern void func_1516E778(f32 *, s32, s8, u8, s32);
extern void func_150BEF7C(s32);
extern f32 func_150489B0(u8);
extern f32 func_15048A40(u8);
extern u32 func_150ADA20(void);
extern void func_1516F548(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
extern void func_151DC8BC(f32 *, s16, s32, s32, s32, s32);
extern void func_150BA4C0(struct127 *, u8, s32);
extern void func_151A3390(struct127 *, u8);
extern void func_15165F80(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_150B2494(struct127 *, s32, s32);
extern void func_15136A50(f32 *, f32 *, f32 *, s16, s32, s32);
extern void func_150CE150(struct127 *, s16, u8, s32);
extern void func_150CFC38(f32);
extern void func_15109410(struct127 *, s16, s32, s32, f32, s32, s32, s32);
extern void func_15109ED4(struct131 *, s16, s32, s32, f32, s32, s32, s32);
extern void func_1510A344(struct127 *, s16, u8, s32);
extern void func_151619A0(s32, s16, u8, s32);
extern void func_15108FFC(s32, s32, u8);
extern void func_15190770(Ev15190770 *, s32, u8, s32);
extern void func_150F802C(SeqTrackState *, s16, u8, s32);
extern void func_150AF738(s16, u8, s32);
extern void func_150F5C08(struct127 *, s16, u8, s32);
extern void func_1519EB8C(SeqTrackState *, u16, s16, u8, s32);
extern void func_150C4120(struct127 *, s16, u8, s32);
extern void func_150F6890(struct127 *, s16, u8, s32);
extern void func_150D0134(u8, s32, s16, u8, s32);
extern void func_150D02B4(u8, s32, s16, u8, s32);
extern void func_150D04C4(u8, s32, s16, u8, s32);
extern void func_150CF680(s16, u8, s32);
extern void func_151925C4(struct127 *, s16, u8, s32);
extern void func_1519ED84(SeqTrackState *, u16, s16, u8, s32);
extern void func_150FAE18(s16, u8, s32);
extern s32 func_15046C80(f32 *, s32, f32, EvFloor *);
extern void func_15136698(f32, f32, u8, s32, s32, f32 *, f32 *, s32, s32, s32, s32);
extern void func_150E3340(f32 *, s32 *, SeqTrackState *, s16);
extern void func_151D8868(Ev151D8868 *, s32, u8, s32);
extern void func_151B5240(s16, u8, s32);
extern void func_15192D48(struct127 *, s16, u8, s32);
extern void func_151B09BC(struct127 *, s32, s16, u8, s32);
extern void func_15134908(Ev15134908 *, s32, u8, s32);
extern void func_150E9FE0(struct127 *, s16, u8, s32);
extern void func_150EA500(struct127 *, s16, u8, s32);
extern void func_151356D4(f32 *, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_150C7020(struct127 *, s32, u8, s32);
extern void func_150DB518(f32 *, f32 *, f32 *, f32 *, f32 *, s32, s32, s32);
extern s32 func_1514D4B8(s16, s16);
extern s32 func_1519E970(s16, EvNodeData *, s32, f32 *, f32 *, s32, s32);
extern void func_1516F2F8(s32, s32, s32, s32, s32, s32);
extern void func_1516F1C0(s32, s32, u8, s32);
extern void func_15136404(struct127 *, f32, u8, s32);
extern void func_1504715C(EvFloor *, struct127 *);
extern void func_150CA930(f32 *);
extern void func_151090DC(void);
extern void func_150FB324(struct127 *, u8, s32);
extern void func_150F97EC(struct127 *, u8, s32);
extern void func_150F9950(struct127 *, u8, s32);
extern s32 func_1514654C(struct127 *, struct126 *, s32, f32 **, f32 **, s32);
extern void func_151C5588(struct127 *, f32 *, s32, u8, s32);
extern void func_151C56A4(f32 *, u8, s32);
extern void func_150E06D8(struct127 *, u8, u8, s32);
extern void func_151CFA4C(struct127 *, u8, s32);
extern void func_151C62D0(struct127 *, u8, s32, struct126 *, s32, s32, s32);
extern void func_150BAA14(struct127 *, s32, s32);
extern void func_150E0870(struct127 *, u8, s32);
extern void func_151D5648(f32 *, u8, s32);
extern void func_15136C3C(struct127 *, s32, s32, s32, s32, s32, s32, s32);
extern void func_15143134(const f32 *, f32 *, void *);
extern void func_151C329C(f32 *, u8, s32);
extern void func_15107B78(struct127 *, s16, s16, u8, s32);
extern void func_151CC290(struct108 *);
extern void func_1519EA78(f32 *, u16, f32, u8, s32);
extern void func_15107700(struct127 *, s16, s16, s16, s32, f32, f32, s32, s32, u8 *, s32, s32);
extern void func_150F16DC(struct127 *, u8, u8, u8, s32);
extern void func_15131EE4(f32 *, f32, u8, s32);
extern void func_15170B90(void *, s32, s32, s32, s32, s32);
extern void func_1507DE4C(struct127 *);
extern void func_15138C80(struct127 *, u8, s32);
extern void func_15193660(struct127 *, u8, s32);
extern void func_151A0A10(struct127 *, s32, u8, s32);
extern void func_151A6F00(void *, s32, u8, s32);
extern void func_15153F18(s16 *, f32 *, s32, u8, s32);
extern void func_151DBAA8(f32 *, s32, u8, u8, s32);
extern void func_151AF4D0(struct127 *, u8, u8, s32);
extern void func_150C1F60(u8);
extern void func_150C2290(u8);
extern void func_150C2424(u8);
extern void func_150DAE70(struct127 *, s32, u8, s32);
extern void func_15148F1C(u8, f32, f32, f32, s32, s32, s32, f32, s16, f32, f32, s32);
extern void func_150DB9E0(struct127 *, s32, u8);
extern void func_1518B264(f32 *, f32, f32, s32, s32);
extern void func_1518B2A8(f32 *, f32, f32, s32, s32, s32, s32);
extern void func_15171600(struct131 *, u8, s32);
extern void func_15171BF4(struct127 *, u8);
extern void func_15170EC4(struct131 *, u8, s32);
extern void func_151711C4(struct127 *);
extern void func_1518B6B0(f32, f32, f32, u8, s32);
extern void func_1516ED68(s32, s32, u8, s32);
extern void func_150C2804(f32 *, f32 *, f32 *, s16, s32, s32);


s32 func_150265CC(SeqEvent *event, s32 arg1, s32 arg2, s32 arg3, s32 arg4, struct127 *arg5, struct131 *arg6, f32 *arg7, s32 arg8) {

    if (event->type == 0xB) {
        s32 vol;
        s32 vol2;
        s32 delay;
        SeqEvent *found;

        if (event->unk4 == 0) {
            if (event->unk6 == 0) {
                vol = 0x10000;
            } else {
                vol = (event->unk6 << 16) / 255;
            }
            func_1001263C(event->value, vol, 0x40);
        } else if (event->unk4 == 1) {
            delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 1, 2, 0, 0, arg4) - arg3;
            if (event->unk6 == 0) {
                vol2 = 0x10000;
            } else {
                vol2 = (event->unk6 << 16) / 255;
            }
            delay = (delay * 0x5604) / 60;
            func_151F2D6C(vol2, delay);
        } else if (event->unk4 == 3) {
            func_151F2BA8();
        } else if (event->unk4 == 4) {
            func_151F2BE8();
        } else if (event->unk4 == 5) {
            func_151F2C4C();
        }
    } else if (event->type == 0x11) {
        if (event->unk4 != 0) {
            D_800C3960[arg4][arg1][event->value] = arg2;
        } else {
            D_800C3960[arg4][arg1][event->value] = 0xFF;
        }
    } else if (event->type == 5) {
        if (event->value == 0x11) {
            s32 delay;
            SeqEvent *found;
            s32 dur;
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

            delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
                if (event->unk6 == 0) {
                    dur = 0x1000;
                } else if (event->unk6 > 200) {
                    dur = event->unk6;
                } else {
                    dur = 200;
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
                t = dur >> 3;
                u = ((dur * 40) / 4096) & 0xFF;
                func_1516D99C((s32)arg7 >> 16, (s32)arg7 & 0xFFFF, 0, 0x1B, 0,
                              c0, c1, c2, c3, c4, c5,
                              0, 1, 6, 0, 0, 0, 0, 0, t >> 8, t & 0xFF, 0, 0, 0xA, 0, 0, 0, 0, 0, 0, 0,
                              delay, u, u, 0xFF, 0xE, 8, 8, 0x25, 0, 0xFF, 8, 0, 0,
                              arg4, 0, 0, arg8, 0);
            }
        } else if (event->value == 0xA) {
            s32 delay;
            SeqEvent *found;

            delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
                func_151B9BF0(3, 3, 0, 0, 0, 0x320, 0x4B0, 0x190, 0xC8, 0x66, 0x1C7, 2, delay, arg7, arg8, 0);
            }
        } else if (event->value == 0xB) {
            s32 delay;
            SeqEvent *found;

            delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
                func_151B9BF0(0x2F, 1, 0, 0, 0, 0x7D0, 0x3E8, 0x190, 0xC8, 0x400, 0x400, 6, delay, arg7, arg8, 0);
            }
        } else if (event->value == 0xC) {
            Ev15167AD8 sp;

            sp.unk0 = D_8008CA50;
            sp.unk4 = 0;
            sp.unk6 = 0x100;
            sp.unk8 = arg7[0];
            sp.unkA = arg7[1] + event->unk4;
            sp.unkC = arg7[2];
            sp.unkE = 0x5DC;
            sp.unk10 = 0x5DC;
            sp.unk12 = -10;
            sp.unk14 = 0;
            func_15167AD8(&sp, arg8, 1);
        } else if (event->value == 0x19) {
            s32 delay;
            SeqEvent *found;
            s32 count;

            delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
                count = event->unk5;
                if (count == 0) {
                    count = 0xC;
                }
                func_15177F90(count, arg7, 0xFF, arg7[0], arg7[1], arg7[2], 0, delay, arg8, 0, event->unk6);
            }
        } else if (event->value == 0x1A) {
            if (event->unk4 != 0 && event->unk4 == 1) {
                f32 from[3];
                f32 to[3];

                from[0] = D_800C3868[arg4][arg1][0];
                from[1] = D_800C3868[arg4][arg1][1];
                from[2] = D_800C3868[arg4][arg1][2];
                to[0] = D_800C3958[arg4][arg1].x;
                to[1] = D_800C3958[arg4][arg1].y + D_800C3958[arg4][arg1].unk38;
                to[2] = D_800C3958[arg4][arg1].z;
                func_150B0094(from, to, arg8, 1);
            } else {
                func_150E1AB0(0, D_800C3868[arg4][arg1][0], D_800C3868[arg4][arg1][1], D_800C3868[arg4][arg1][2], D_800C3958[arg4][arg1].x, D_800C3958[arg4][arg1].y + D_800C3958[arg4][arg1].unk38, D_800C3958[arg4][arg1].z, 40.0f, 0.0f, 8.0f, 60.0f, 0x258, 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            }
        } else if (event->value == 0x1C) {
            s32 anim;

            if (arg5 == NULL) {
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
            func_150E2DB4(arg5, arg5->unique_id, anim, -1, 0.0f, 0.0f, 156.0f, 0.0f, 0.0f, 636.0f, 3, 0xA, 0xF0, 0);
        } else if (event->value == 0x1B) {
            s32 delay;
            SeqEvent *found;

            delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
                switch (*(u16 *)&event->unk6) {
                    case 0:
                        func_1516E778(arg7, delay, event->unk5, arg8, 0);
                        break;
                    case 1:
                        func_150BEF7C(delay);
                        break;
                    case 2: {
                        f32 x;
                        f32 y;
                        f32 z;
                        s32 angle;
                        s32 pad[3];
                        f32 s;
                        f32 c;
                        s32 pad2;

                        x = arg5->x_position;
                        y = arg5->y_position;
                        z = arg5->z_position;
                        angle = arg5->unk7A >> 8;
                        s = func_15048A40(angle);
                        c = func_150489B0(angle);
                        func_1516F548(x + 20.0f, y + 475.0f, z, delay, 14.0f * s, 0xB00, 14.0f * c, 0xC8, 0x32, 0xA, 6, 7, 0xBE, 0xBE, 0xBE, arg8, 0);
                        func_1516F548(x - 24.0f, y + 476.0f, z, delay, -23.0f * s, 0x800, -23.0f * c, 0xC8, 0x32, 0xA, 6, 7, 0xBE, 0xBE, 0xBE, arg8, 0);
                        break;
                    }
                    case 3:
                        func_151DC8BC(arg7, delay, 1, 1, arg8, 0);
                        break;
                    case 4:
                        func_151DC8BC(arg7, delay, 0, 0, arg8, 0);
                        break;
                }
            }
        } else if (event->value == 0x1F) {
            if (arg5 == NULL) {
                return 1;
            }
            func_150BA4C0(arg5, arg8, 1);
        } else if (event->value == 0) {
            if (arg5 == NULL) {
                return 1;
            }
            func_151A3390(arg5, arg8);
        } else if (event->value == 0x20) {
            func_15165F80(-1, arg7[0], arg7[1], arg7[2], 0x14, 0x14, 1, arg8, 1);
        } else if (event->value == 6) {
        } else if (event->value == 7) {
            func_150B2494(arg5, 9, -1);
        } else if (event->value == 0x21) {
            s32 delay;
            SeqEvent *found;

            if (event->unk4 == 0) {
                delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                func_15136A50(arg7, arg7 + 1, arg7 + 2, delay, arg8, 0);
            }
        } else if (event->value == 0x60) {
            s32 delay;
            SeqEvent *found;

            if (event->unk4 == 0) {
                delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                func_150CE150(arg5, delay, arg8, 0);
            }
        } else if (event->value == 0x65) {
            s8 val[4];
            s32 pad;
            s32 delay;
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
                delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 1, 1, 1, event->unk5, arg4) - arg3;
                switch (event->unk5) {
                    case 1:
                        func_15109410(arg5, delay, -1, 0, 0.000709999993f, 0, arg8, 1);
                        break;
                    case 2:
                        func_15109410(arg5, delay, -1, 1, 0.00612499984f, 0, arg8, 1);
                        break;
                    case 3:
                        func_15109ED4(arg6, delay, -1, 0, 0.00612499984f, 0, arg8, 1);
                        break;
                    case 4:
                        func_15109ED4(arg6, delay, -1, 1, 0.00612499984f, 0, arg8, 1);
                        break;
                    case 5:
                        func_1510A344(arg5, delay, arg8, 1);
                        break;
                    case 6:
                        func_151619A0(arg1, delay, arg8, 1);
                        break;
                    case 7:
                        func_15108FFC(arg1, delay, arg8);
                        break;
                    case 8: {
                        f32 p[3];

                        p[0] = D_800C3958[arg4][arg1].x;
                        p[1] = D_800C3958[arg4][arg1].y;
                        p[2] = D_800C3958[arg4][arg1].z;
                        func_151A9390(5, 1, 0, p, -10000.0f, 100.0f, delay, arg8, 1);
                        break;
                    }
                    case 9: {
                        Ev15190770 sp;

                        sp.unk0 = arg5;
                        sp.unk4 = arg5->unique_id;
                        sp.unk6 = delay;
                        sp.unk8 = 1;
                        sp.unk9 = 0;
                        switch (arg5->id) {
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
                        func_15190770(&sp, 0, arg8, 1);
                        break;
                    }
                    case 10:
                        func_150F802C(&D_800C3958[arg4][arg1], delay, arg8, 1);
                        break;
                    case 11: {
                        s16 data[2];
                        void *p;

                        func_150AF738((s16)delay, arg8, 1);
                        data[0] = 0;
                        data[1] = 0;
                        p = func_15149130((s16)delay, -1, 0x4B, -1, 1, 0, (struct37 *)4, arg8, 1);
                        if (p != NULL) {
                            memcpy((u8 *)p + 0x28, data, 4);
                        }
                        break;
                    }
                    case 12:
                        func_150F5C08(arg5, delay, arg8, 1);
                        break;
                    case 13: {
                        u16 id;

                        id = event->unk6;
                        if (id >= 0xE9) {
                            id = 0;
                        }
                        func_1519EB8C(&D_800C3958[arg4][arg1], id, delay, arg8, 1);
                        break;
                    }
                    case 15:
                        if (arg5 != NULL && delay > 0) {
                            f32 t;

                            *(s32 *)((u8 *)arg5 + 0x2EC) = 1;
                            t = (f32)delay * 0.5f;
                            *(f32 *)((u8 *)arg5 + 0x2D8) = t;
                            *(f32 *)((u8 *)arg5 + 0x2DC) = 1.0f / t;
                            func_150C4120(arg5, delay, arg8, 1);
                        }
                        break;
                    case 16:
                        if (arg5 != NULL && delay > 0) {
                            func_150F6890(arg5, delay, arg8, 1);
                        }
                        break;
                    case 17:
                        func_150D0134(event->unk6, D_800C35E0[arg4][event->unk6], delay, arg8, 1);
                        break;
                    case 18:
                        func_150D02B4(event->unk6, D_800C35E0[arg4][event->unk6], delay, arg8, 1);
                        break;
                    case 19:
                        func_150CF680(delay, arg8, 1);
                        break;
                    case 20:
                        func_151925C4(arg5, delay, arg8, 1);
                        break;
                    case 21:
                        func_150D04C4(event->unk6, D_800C35E0[arg4][event->unk6], delay, arg8, 1);
                        break;
                    case 22: {
                        u16 id;

                        id = event->unk6;
                        if (id >= 0xCF) {
                            id = 0;
                        }
                        func_1519ED84(&D_800C3958[arg4][arg1], id, delay, arg8, 1);
                        break;
                    }
                    case 23:
                        func_150FAE18(delay, arg8, 1);
                        break;
                    case 24: {
                        f32 pos[3];
                        f32 probe[3];
                        EvFloor floor;
                        s32 hit;

                        pos[0] = D_800C3958[arg4][arg1].x;
                        pos[1] = D_800C3958[arg4][arg1].y + 100.0f;
                        pos[2] = D_800C3958[arg4][arg1].z;
                        probe[0] = pos[0];
                        probe[1] = pos[1] + 100.0f;
                        probe[2] = pos[2];
                        if (func_15046C80(probe, 0, pos[1] - 1000.0f, &floor) != 0) {
                            if (floor.unk1D != 3) {
                                hit = 1;
                                pos[1] = floor.unk0;
                            } else {
                                hit = 0;
                            }
                        } else {
                            hit = 0;
                        }
                        if (hit) {
                            func_15136698((func_150ADA68() * 0.02000000142f) + 0.09000000358f, (func_150ADA68() * 0.0004399999743f) + 0.0008799999487f, (func_150ADA20() % 101) + 0x9B, 0xFF, delay, &floor.unk4, pos, !event->unk6, 1, arg8, 1);
                        }
                        break;
                    }
                    case 25: {
                        f32 from[3];
                        s32 to[3];

                        from[0] = D_800C3868[arg4][arg1][0];
                        from[1] = D_800C3868[arg4][arg1][1];
                        from[2] = D_800C3868[arg4][arg1][2];
                        to[0] = D_800C3958[arg4][arg1].x;
                        to[1] = D_800C3958[arg4][arg1].y;
                        to[2] = D_800C3958[arg4][arg1].z;
                        func_150E3340(from, to, &D_800C3958[arg4][arg1], delay);
                        break;
                    }
                    case 26: {
                        Ev151D8868 sp;

                        sp.unk0 = 1;
                        sp.unk2 = delay;
                        sp.unk4 = 8;
                        sp.unk5 = 1;
                        sp.unk6 = -1;
                        func_151D8868(&sp, 0, arg8, 1);
                        break;
                    }
                }
            }
        } else if (event->value == 0x5B) {
            s32 delay;
            SeqEvent *found;

            if (event->unk4 == 0) {
                delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                func_151B5240((event->unk4 != 0) ? 5000 : delay, arg8, 0);
            }
        } else if (event->value == 0x5D) {
            s32 delay;
            SeqEvent *found;

            if (event->unk4 == 0) {
                delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                func_15192D48(arg5, delay, arg8, 0);
            }
        } else if (event->value == 0x5E) {
            s32 delay;
            SeqEvent *found;

            if (event->unk5 == 1) {
                func_151B09BC(arg5, 0, 1000, arg8, 0);
            } else if (event->unk4 == 0) {
                delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                func_151B09BC(arg5, 0, delay, arg8, 0);
            }
        } else if (event->value == 0x4C) {
            s32 delay;
            SeqEvent *found;
            u8 *list;
            s32 count;

            if (event->unk4 == 0) {
                delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                if (arg5->id == 0x49 || arg5->id == 0x58) {
                    list = func_1519003C(arg5, &count);
                    if (list != NULL && event->unk6 < count) {
                        func_1518F51C(arg5, list[event->unk6], delay, -1, -1, -1, 0, 0, arg8, 1);
                    }
                } else {
                    func_1518D1C0(arg5, event->unk5, delay, 0, arg8, 1, "Cutscene 2");
                }
            }
        } else if (event->value == 0x53) {
            if (event->unk4 == 0) {
                s32 t;
                SeqEvent *found;

                t = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4);
                {
                    f32 *pos = arg7;
                    Ev15134908 sp;

                    sp.unk4 = pos + 1;
                    sp.unk8 = pos + 2;
                    sp.unk0 = pos;
                    sp.unkC = 2.0f;
                    sp.unk10 = 1.0f;
                    sp.unk14 = t - arg3;
                    sp.unk16 = 1;
                    sp.unk17 = 2;
                    func_15134908(&sp, 0, arg8, 0);
                }
            }
        } else if (event->value == 0x50) {
            s32 delay;
            SeqEvent *found;

            if (event->unk4 == 0) {
                delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                func_150E9FE0(arg5, delay, arg8, 0);
            }
        } else if (event->value == 0x51) {
            s32 delay;
            SeqEvent *found;

            if (event->unk4 == 0) {
                delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                func_150EA500(arg5, delay, arg8, 0);
            }
        } else if (event->value == 0x48) {
            f32 offset[2];
            s32 delay;
            SeqEvent *found;

            if (event->unk4 == 0) {
                delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 1, event->unk5, arg4) - arg3;
                offset[0] = (func_150ADA68() * 200.0f) + -100.0f;
                offset[1] = (func_150ADA68() * 300.0f) + -150.0f;
                func_151356D4(offset, (func_150ADA68() * 20.0f) + 10.0f, ((func_150ADA68() * 65.0f) + 35.0f) * 0.001000000047f, ((func_150ADA68() * 160.0f) + 200.0f) * 0.001000000047f, (func_150ADA20() % 101) + 100, 0xFF, (func_150ADA20() % 3) + 4, delay, 4, 0x40, arg8, 1);
            }
        } else if (event->value == 0xF) {
            s32 delay;
            SeqEvent *found;

            delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
                func_150C7020(arg5, (s16)delay, arg8, 1);
            }
        } else if (event->value == 0x3E || event->value == 0x3F) {
        } else if (event->value == 0x32) {
            s32 delay;
            SeqEvent *found;

            delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
                func_150DB518(arg7, &D_800C3958[arg4][arg1].unk10, &D_800C3958[arg4][arg1].unk18, &D_800C3958[arg4][arg1].unk1C, &D_800C3958[arg4][arg1].unk20, delay, arg8, 0);
            }
        } else if (event->value == 0x34) {
            s32 delay;
            SeqEvent *found;
            u8 t;
            u8 kind;
            EvNode *node;

            delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
                t = (event->unk5 <= 0) ? event->unk5 : 0;
                if (t == 0) {
                    kind = 0xD;
                }
                node = *(EvNode **)((u8 *)arg5 + 0x2F4);
                while (node != NULL) {
                    if (node->unk1C == kind || func_1514D4B8(D_8008ABE8[kind]->unk4, node->unk1C) != 0) {
                        node->unk10->unk30 = func_1519E970(delay, node->unk10, 0, &D_800C3958[arg4][arg1].unk30, &D_800C3958[arg4][arg1].unk20, arg8, 0);
                    }
                    node = node->unk14;
                }
            }
        } else if (event->value == 0x23) {
            s32 delay;
            SeqEvent *found;

            if (arg5 == NULL) {
                return 1;
            }
            delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
                func_1516F2F8(arg5 - D_800CC2D0, delay, delay, 0, arg8, 0);
            }
        } else if (event->value == 0x22) {
            s32 delay;
            SeqEvent *found;

            if (arg5 == NULL) {
                return 1;
            }
            delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
                func_1516F1C0(arg5 - D_800CC2D0, delay, arg8, 0);
            }
        } else if (event->value == 0x52) {
            if (arg5 == NULL) {
                return 1;
            }
            func_15136404(arg5, 90.0f, arg8, 1);
        } else if (event->value == 0x5F) {
            switch (*(u8 *)&event->unk4) {
                case 0:
                    if (arg5 != NULL) {
                        f32 pos[3];
                        EvFloor floor;
                        f32 out[3];

                        func_1504715C(&floor, arg5);
                        pos[0] = arg5->x_position;
                        pos[1] = arg5->y_position + 100.0f;
                        pos[2] = arg5->z_position;
                        if (func_15046C80(pos, 0, arg5->y_position - 300.0f, &floor) != 0) {
                            out[0] = pos[0];
                            out[1] = floor.unk0;
                            out[2] = pos[2];
                            func_150CA930(out);
                        }
                    }
                    break;
                case 1:
                    func_151090DC();
                    break;
                case 2: {
                    struct126 *bone;
                    f32 vec[3];

                    if (arg5 != NULL) {
                        switch (arg5->id) {
                            case 0x5A:
                            case 0x74:
                            case 0x75:
                            case 0x7A:
                                bone = func_1503195C(arg5, 0x46, 0);
                                if (bone != NULL) {
                                    f32 *p1;
                                    f32 *p2;

                                    p1 = (f32 *)&D_800AAA90;
                                    p2 = vec;
                                    if (func_1514654C(arg5, bone, 0, &p1, &p2, 1) != 0) {
                                        func_151C5588(arg5, vec, 0x46, arg8, 1);
                                        func_151C56A4(vec, arg8, 1);
                                    }
                                }
                                break;
                            case 0x86:
                                func_150FB324(arg5, arg8, 1);
                                break;
                            case 0x4A:
                                func_150E06D8(arg5, event->unk5, arg8, 1);
                                break;
                            case 0x97:
                                func_150F97EC(arg5, arg8, 1);
                                break;
                            case 0x98:
                                func_150F9950(arg5, arg8, 1);
                                break;
                        }
                    }
                    break;
                }
                case 3: {
                    f32 p[3];

                    p[0] = D_800C3958[arg4][arg1].x;
                    p[1] = D_800C3958[arg4][arg1].y;
                    p[2] = D_800C3958[arg4][arg1].z;
                    func_151BE850(p, D_800C3958[arg4][arg1].unk10, 1, 0, 2, arg8, 1);
                    break;
                }
                case 4:
                    func_151CFA4C(arg5, arg8, 1);
                    break;
                case 5:
                    if (arg5 != NULL) {
                        struct126 *bone;

                        bone = NULL;
                        if (event->unk6 != 0) {
                            bone = func_1503195C(arg5, event->unk5 ? 0xA4 : 0xA5, 0);
                        }
                        func_151C62D0(arg5, event->unk5, 1, bone, event->unk5 ? 0xA4 : 0xA5, arg8, 1);
                    }
                    break;
                case 6:
                    func_150BAA14(arg5, 9, -1);
                    break;
                case 7:
                    func_150E0870(arg5, arg8, 1);
                    break;
                case 8: {
                    f32 p[3];

                    p[0] = arg7[0];
                    p[1] = arg7[1];
                    p[2] = arg7[2];
                    func_151D5648(p, arg8, 1);
                    break;
                }
                case 9:
                    func_15136C3C(arg5, 1, 1, 1, 1, 0, arg8, 1);
                    break;
                case 10: {
                    if (arg5->unk1D4 != NULL) {
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
                        func_15143134(sel, out, (u8 *)arg5->unk1D4 + (bones[event->unk5] << 6));
                        func_151C329C(out, arg8, 1);
                    }
                    break;
                }
                case 11: {
                    u8 i;
                    const s32 ids[4] = { 6, 7, 8, 9 };

                    for (i = 0; i < 4; i++) {
                        func_151C970C(1, &(*(struct178 **)&D_800D3098)[ids[i]]);
                    }
                    break;
                }
                case 12:
                    func_15107B78(arg5, func_150ADA20() & 0xFF, (func_150ADA20() & 0x7F) - 0x40, arg8, 1);
                    break;
                case 13:
                    func_151CC290(D_800DBFF0);
                    break;
                case 14: {
                    f32 p[3];
                    u16 id;
                    f32 scale;

                    p[0] = arg7[0];
                    p[1] = arg7[1];
                    p[2] = arg7[2];
                    id = event->unk5;
                    if (id >= 0xE9) {
                        id = 0;
                    }
                    if (event->unk6 != 0) {
                        scale = event->unk6 * 0.009999999776f;
                    } else {
                        scale = 1.0f;
                    }
                    func_1519EA78(p, id, scale, arg8, 1);
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
                    func_15107700(arg5, func_150ADA20() & 0xFF, (func_150ADA20() % 129) - 0x3F, (func_150ADA20() % 21) + 0xA, 4, 40.0f, size, 0, 2, colour, arg8, 1);
                    break;
                }
                case 16:
                    func_150F16DC(arg5, event->unk5, ((u8 *)event)[7], arg8, 1);
                    break;
                case 17: {
                    s32 unused;
                    Ev151D8868 sp;

                    sp.unk0 = 1;
                    sp.unk2 = 0x32;
                    sp.unk4 = 8;
                    sp.unk5 = 1;
                    sp.unk6 = -1;
                    func_151D8868(&sp, 0, arg8, 1);
                    break;
                }
                case 18: {
                    if (arg5->unk1D4) {
                        f32 out[3];
                        const f32 offs[2][3] = { { -65.0f, 172.0f, 9.0f }, { 15.0f, 63.0f, 9.0f } };

                        func_15143134(offs[event->unk5], out, arg5->unk1D4);
                        func_15131EE4(out, 0.6460000277f, arg8, 0);
                    }
                    break;
                }
                case 19: {
                    if (arg5->unk1D4) {
                        f32 out[3];
                        const f32 offs[8][3] = { { 0.0f, -14.0f, 18.0f }, { 0.0f, -14.0f, 18.0f }, { 0.0f, -14.0f, 18.0f }, { 0.0f, -14.0f, 18.0f }, { -5.0f, 25.0f, -7.0f }, { -11.0f, 28.0f, -5.0f }, { -4.0f, 30.0f, -4.0f }, { 3.0f, 30.0f, -8.0f } };
                        const u8 bones[8] = { 7, 8, 9, 10, 13, 14, 15, 16 };

                        func_15143134(offs[event->unk5], out, (u8 *)arg5->unk1D4 + (bones[event->unk5] << 6));
                        func_15131EE4(out, 6.990000248f, arg8, 0);
                    }
                    break;
                }
                case 20: {
                    SeqTrackState *pos;
                    struct {
                        u8 pad0[0x10];
                        s16 x;
                        s16 y;
                        s16 z;
                        u8 pad16[0x86];
                    } sp;

                    pos = &D_800C3958[arg4][arg1];
                    sp.x = pos->x;
                    sp.y = pos->y + 50.0f;
                    sp.z = pos->z;
                    func_15170B90(&sp, 1, 1, 2, arg8, 0);
                    break;
                }
                case 21:
                    if (arg5 != NULL) {
                        func_1507DE4C(arg5);
                        func_15138C80(arg5, arg8, 0);
                    }
                    break;
                case 22:
                    if (arg5 != NULL) {
                        func_15193660(arg5, arg8, 0);
                    }
                    break;
            }
        } else if (event->value == 0x40) {
            func_151A0A10(arg5, 0x28, arg8, 1);
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
            sp.unk4 = arg7[0];
            sp.unk8 = arg7[1];
            sp.unkC = arg7[2];
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
            func_151A6F00(&sp, 1, arg8, 1);
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
            sp.unk4 = arg7[0];
            sp.unk8 = arg7[1];
            sp.unkC = arg7[2];
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
            func_151A6F00(&sp, 1, arg8, 1);
        } else if (event->value == 0x4F) {
            struct {
                s16 unk0;
                s16 unk2;
                s16 unk4;
                s16 unk6;
                f32 unk8;
                f32 unkC;
                f32 unk10;
                f32 unk14;
                f32 unk18;
                f32 unk1C;
                f32 unk20;
                f32 unk24;
                f32 unk28;
                s16 unk2C;
                s16 unk2E;
                s16 unk30;
                s16 unk32;
                s16 unk34;
                s16 unk36;
                s16 unk38;
                s16 unk3A;
                s8 unk3C;
                f32 unk40;
                s16 unk44;
                s16 unk46;
                s32 unk48;
            } sp;

            sp.unk8 = arg7[0];
            sp.unkC = arg7[1];
            sp.unk10 = arg7[2];
            sp.unk14 = 2.900000095f;
            sp.unk18 = 17.0f;
            sp.unk1C = 2.5f;
            sp.unk2C = 0xF;
            sp.unk20 = 0.5770000219f;
            sp.unk24 = 25.0f;
            sp.unk28 = 20.20000076f;
            sp.unk2E = 0xA;
            sp.unk0 = -0x14;
            sp.unk2 = 0x31;
            sp.unk4 = -0x34;
            sp.unk6 = 0x2B;
            sp.unk30 = 3;
            sp.unk32 = 4;
            sp.unk34 = 0x28;
            sp.unk36 = 0x1E;
            sp.unk38 = 0x64;
            sp.unk3A = 0x64;
            sp.unk3C = 0;
            sp.unk44 = 0x10;
            sp.unk46 = 0xF;
            sp.unk48 = 0;
            sp.unk40 = 0.5f;
            func_15153F18(&sp.unk0, &sp.unk8, 0, arg8, 1);
        } else if (event->value == 0x24) {
            f32 p[3];
            s32 count;
            u8 type;

            p[0] = arg7[0];
            p[1] = arg7[1];
            p[2] = arg7[2];
            count = (event->unk4 != 0) ? event->unk4 : 0xA;
            type = event->unk5;
            if (type >= 0xD) {
                type = 0;
            }
            func_151DBAA8(p, count, type, arg8, 0);
        } else if (event->value == 0x10) {
            u8 type;

            type = event->unk4;
            if (type >= 0x1B) {
                type = 0;
            }
            func_151AF4D0(arg5, type, arg8, 0);
        } else if (event->value == 0x47) {
            func_151925C4(arg5, 0x32, arg8, 0);
        } else if (event->value == 0x2E) {
            func_150C1F60(arg8);
            func_150C2290(arg8);
            func_150C2424(arg8);
        } else if (event->value == 0x31) {
            func_150DAE70(arg5, 0, arg8, 0);
        } else if (event->value == 0x33) {
            u8 type;
            SeqTrackState *pos;
            s32 i4, i5, i6;
            f32 f7, f9, f10;
            s16 n;

            if (event->unk4 >= 0x10) {
                type = 0xF;
            } else {
                type = event->unk4;
            }
            pos = &D_800C3958[arg4][arg1];
            i4 = pos->unk20 * 0.05000000075f;
            i5 = (pos->unk10 + 180.0f) * 0.7111111283f;
            i6 = pos->unkC * 0.7111111283f;
            f7 = (pos->unk38 * 0.1000000015f) + 0.5f;
            f9 = (pos->unk18 * 0.1000000015f) + 1.0f;
            f10 = pos->unk1C * 0.1000000015f;
            n = (event->unk5 != 0) ? event->unk5 : 0x64;
            func_15148F1C(type, arg7[0], arg7[1], arg7[2], i4, i5, i6, f7, n, f9, f10, arg8);
        } else if (event->value == 0x35) {
            s32 type;

            type = (event->unk4 >= 2) ? 1 : event->unk4;
            func_150DB9E0(arg5, type, arg8);
        } else if (event->value == 0x26) {
            SeqEvent *found;
            s32 t;

            if (arg5 == NULL) {
                return 1;
            }
            t = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4);
            if (event->unk4 == 0) {
                arg5->unk276 = 1;
                arg5->unk282 = t - arg3;
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
            func_1518B264(arg7, dist, speed, n, arg8);
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
            func_1518B2A8(arg7, dist, speed, n, 0x1E, 0x64, arg8);
        } else if (event->value == 0x28) {
            if (arg6 != NULL) {
                func_15171600(arg6, arg8, 0);
            } else if (arg5 != NULL) {
                func_15171BF4(arg5, arg8);
            }
        } else if (event->value == 0x29) {
            if (arg6 != NULL) {
                func_15170EC4(arg6, arg8, 0);
            } else if (arg5 != NULL) {
                func_151711C4(arg5);
            }
        } else if (event->value == 0x2A) {
        } else if (event->value == 0x2B) {
            func_1518B6B0(arg7[0], arg7[1], arg7[2], arg8, 0);
        } else if (event->value == 0x2C) {
            SeqEvent *found;
            s32 pad[7];

            func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4);
            if (event->unk4 == 0) {
            }
        } else if (event->value == 0x2F) {
            s32 delay;
            SeqEvent *found;

            if (arg5 == NULL) {
                return 1;
            }
            if (event->unk4 == 0) {
                delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                func_1516ED68(arg5 - D_800CC2D0, delay, arg8, 0);
            }
        } else if (event->value == 0x39) {
            s32 delay;
            SeqEvent *found;

            if (event->unk4 == 0) {
                delay = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                func_150C2804(arg7, arg7 + 1, arg7 + 2, delay, arg8, 0);
            }
        } else if (event->value == 0x3A) {
            f32 p[3];
            s32 count;

            p[0] = arg7[0];
            p[1] = arg7[1];
            p[2] = arg7[2];
            count = (event->unk4 != 0) ? event->unk4 : 0xA;
            func_151DBAA8(p, count, 6, arg8, 0);
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u16 unkC;
    u8 unkE;
    u8 padF;
} Snd15029BB8;

extern void *allocate_memory(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_1000D96C(s32, s32, s32);
extern void func_1000DE1C(s32, s32);
extern s32 func_1000E134(s32);
extern void func_1503DE70(struct127 *, s32, s32);
extern void func_151557FC(s32, s32, f32);
extern void func_15155F90(void);
extern void func_15178EFC(s32);
extern void func_1517EE40(s32, s32, s32, s32, s32, s32);
extern void func_15188810(struct127 *, s32, s32);
extern void func_1518894C(struct131 *, s32, s32);
extern void func_15188AD0(struct127 *);
extern void func_151D66F0(s32, s32);
extern s32 func_150238D8();

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} Color15029BB8;

extern Color15029BB8 D_80084110[];
extern s8 D_800C3C8A;
extern u16 D_800C3C8E;

s32 func_15029BB8(SeqEvent *event, s32 arg1, s32 arg2, s32 arg3, s32 arg4, struct127 *obj, struct131 *actor, f32 *pos, s32 flags, s32 arg9) {
    if ((event->type == 6) || (event->type == 10)) {
        {
            s32 vol;
            s32 time;
            s32 t;
            s32 pad;
            SeqEvent *found;
            s32 fade;
            s32 fadeOut;
            s32 fadeIn;
            s32 sflags;
            Snd15029BB8 *snd;

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
                        pad = (vol > 0xFF) ? 0xFF : vol;
                        vol = pad;
                    }
                }
                time = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 1, 1, 0, 0, arg4) - arg3;
                if (event->type == 6) {
                    time = 0xFFFF;
                } else if (found == NULL) {
                    time = 0xFFFF;
                }
                if ((arg9 != 0) && ((event->type != 10) || (found != NULL))) {
                    return 1;
                }
                fade = event->unk5 * 50;
                fadeOut = 0;
                fadeIn = 0;
                if (event->type == 10) {
                    t = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 1, 2, 0, 0, arg4);
                    if (found != NULL) {
                        t -= arg3;
                        if (t < time) {
                            fadeIn = t;
                        }
                    }
                    t = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 1, 3, 0, 0, arg4);
                    if (found != NULL) {
                        t -= arg3;
                        if (t < time) {
                            fadeOut = time - t;
                        }
                    }
                } else if (event->unk4 == 2) {
                    sflags = 0x800;
                } else {
                    sflags = 0x400;
                }
                snd = allocate_memory(0x10, 1, 0, 0);
                if (snd != NULL) {
                    snd->unkE = D_800C35E8[arg4];
                    snd->unk0 = arg4;
                    snd->unk1 = arg1;
                    snd->unk4 = time;
                    snd->unk6 = time;
                    snd->unk8 = fadeIn;
                    snd->unkA = fadeOut;
                    snd->unkC = (vol * 0x7FFF) >> 8;
                    snd->unk2 = event->type == 10;
                    func_1000FA64(event->value, (s16)pos[0], (s16)pos[1], (s16)pos[2], snd->unkC, 0x7D00, 0x7918, (s32)func_150238D8, snd, 0, sflags, fade);
                }
            }
        }
    } else if (event->type == 8) {
        {
            s32 time;
            SeqEvent *found;

            if (event->unk4 == 0) {
                if ((arg9 != 0) && (func_1000E134(event->value) != 0)) {
                    return 1;
                }
                func_1000D96C(event->value, 0, 0);
                if (event->unk6 != 0) {
                }
            } else if (event->unk4 == 1) {
                func_1000E588(event->value, event->unk6 * 100, 1 << (event->unk5 + 31));
            } else if (event->unk4 == 2) {
                func_1000DF68(event->value, event->unk6 * 328, 1);
            } else if (event->unk4 == 3) {
            } else if (event->unk4 == 4) {
                time = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 1, 5, 1, event->unk5, arg4) - arg3;
                if (arg9 != 0) {
                    func_1000E588(event->value, event->unk6, 1 << (event->unk5 + 31));
                } else {
                    func_1000E46C(event->value, event->unk6, 1 << (event->unk5 + 31), time);
                }
            } else if (event->unk4 == 6) {
                time = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 1, 7, 0, 0, arg4) - arg3;
                if (time < 2) {
                    time = 1;
                }
                func_1000DF68(event->value, event->unk6 * 328, time);
            } else if (event->unk4 == 8) {
                func_1000E654(event->value, event->unk6 + 1, 0, -1);
            } else if (event->unk4 == 9) {
                func_1000DE1C(event->value, 0);
            }
        }
    } else if (event->type == 2) {
        {
            f32 speed;
            f32 rate;
            f32 time;
            SeqEvent *found;
            s32 loop;
            s32 id;

            if (obj == NULL) {
                return 1;
            }
            time = func_15023BB0(event, event->type, arg1, &found, 0, 0, 0, 0, 0, 0, arg4) - arg3;
            if (event->unk4 == 0) {
                if (obj->unk2D0 == NULL) {
                    return 1;
                }
                rate = 5.0f;
                id = event->value;
                if (event->unk6 > 0) {
                    rate = event->unk6 - 1;
                }
                if (arg3 == D_800C3C8E) {
                    rate = 0.0f;
                }
                if (arg9 != 0) {
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
                if (time > 0.0f) {
                    speed = obj->unk2D0->unk18 / time * 2.0f;
                } else {
                    speed = 0.0f;
                }
                if (event->unk5 != 0) {
                    speed = 0.0f;
                }
                func_1505E650(obj, id, speed, rate, 0.0f, 0.0f, loop);
            }
        }
    } else if (event->type == 5) {
            if (event->value == 1) {
                SeqEvent *found;
                s32 time;
                SeqEvent *prev;
                s32 prevTime;

                if (event->unk4 == 0) {
                    time = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                    if (arg9 != 0) {
                        time = 0;
                    }
                    func_1517EE40(D_80084110[event->unk5].unk0, D_80084110[event->unk5].unk1, D_80084110[event->unk5].unk2, time, !event->unk6, 0);
                    D_800C3C8A = !event->unk6;
                } else if (arg9 != 0) {
                    func_15023DE0(&prev, &found, NULL, &prevTime, arg1, event->type, 1, event->value, 1, 0, D_800C35B0[arg4], arg4);
                    if (prev != NULL) {
                        func_1517EE40(D_80084110[prev->unk5].unk0, D_80084110[prev->unk5].unk1, D_80084110[prev->unk5].unk2, 0, !prev->unk6, 0);
                        D_800C3C8A = !prev->unk6;
                    }
                }
            } else if (event->value == 4) {
                s32 a;

                if (obj != NULL) {
                    if (arg9 == 0) {
                        a = (obj->id == 0x4B) ? 2 : -1;
                        if (a != -1) {
                            arg2 = (event->unk4 == 0) ? -1 : event->unk4 - 1;
                            func_1503DE70(obj, a, arg2);
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
                    if (arg9 == 0) {
                        start = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 1, 1, 1, event->unk5, arg4);
                        fadeIn = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 1, 2, 1, event->unk5, arg4) - arg3;
                        if (found == NULL) {
                            fadeIn = 0;
                        }
                        fadeOut = start - func_15023BB0(event, event->type, arg1, &found, 1, event->value, 1, 3, 1, event->unk5, arg4);
                        if (found == NULL) {
                            fadeOut = 0;
                        }
                        func_150F5420(fadeIn, start - arg3 - fadeOut - fadeIn, fadeOut, event->unk5);
                    }
                }
            } else if (event->value == 0x56) {
                if (event->unk4 == 0) {
                    if (arg9 == 0) {
                        func_15178EFC(3);
                    }
                } else if (arg9 != 0) {
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
                s32 time;
                SeqEvent *found;

                time = func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3;
                if (actor != NULL) {
                    if (event->unk4 == 0) {
                        func_1518894C(actor, time, event->unk5);
                    } else if (arg9 != 0) {
                    }
                } else if (obj != NULL) {
                    if (event->unk4 == 0) {
                        func_15188810(obj, time, event->unk5);
                    } else if (arg9 != 0) {
                        func_15188AD0(obj);
                    }
                }
            } else if (event->value == 0x57) {
                s32 time;
                SeqEvent *found;

                if (event->unk4 == 0) {
                    func_151557FC(0, func_15023BB0(event, event->type, arg1, &found, 1, event->value, 0, 0, 0, 0, arg4) - arg3, 0.0f);
                } else if (arg9 != 0) {
                    D_800C3E78 = 0;
                    func_15155F90();
                }
            }
    } else {
        return 0;
    }
    return 1;
}

void func_1502A8A0(SeqEvent *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    struct127 *obj;
    struct131 *actor;
    f32 *pos;
    Elem1502A8A0 *elem;
    s32 flags;

    obj = NULL;
    actor = NULL;
    pos = NULL;
    flags = D_800C35E8[arg4];
    elem = &D_800C35F0[arg4][arg1];
    if (elem->unk0 == 2) {
        obj = func_15083E90(elem->unk2);
        if (obj == NULL) {
            return;
        }
        pos = &obj->x_position;
    } else if (elem->unk0 == 3) {
        actor = func_151149AC(elem->unk2);
        if (actor == NULL) {
            return;
        }
    }
    if (pos == NULL) {
        pos = &D_800C3958[arg4][arg1].x;
    }
    if (D_800C3C88 != 2) {
        if (func_1502460C(arg0, arg1, arg2, arg3, arg4, obj, actor, pos, flags)) {
            return;
        }
        if (func_150265CC(arg0, arg1, arg2, arg3, arg4, obj, actor, pos, flags)) {
            return;
        }
        if (func_15029BB8(arg0, arg1, arg2, arg3, arg4, obj, actor, pos, flags, 0)) {
            return;
        }
    } else {
        if (func_1502460C(arg0, arg1, arg2, arg3, arg4, obj, actor, pos, flags)) {
            return;
        }
        func_15029BB8(arg0, arg1, arg2, arg3, arg4, obj, actor, pos, flags, 1);
    }
}
