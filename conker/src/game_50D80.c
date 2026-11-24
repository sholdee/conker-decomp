#include <ultra64.h>
#define func_1514D3B0 func_1514D3B0_hdr
#define func_15048A40 func_15048A40_void_proto
#define func_150ADA20 func_150ADA20_u8_proto
#include "functions.h"
#undef func_1514D3B0
#undef func_15048A40
#undef func_150ADA20
#include "variables.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
    u16 unkA;
} Elem15024130;

typedef struct {
    u16 unk0;
    u8 unk2;
    u8 pad3[5];
} Elem1502A8A0;

extern Elem15024130 *D_800C3D50;
extern u8 D_800C35E8[];
extern Elem1502A8A0 *D_800C35F0[];

extern struct131 *func_151149AC(s32);
extern s32 func_1502460C(s32, s32, s32, s32, s32, struct127 *, struct131 *, f32 *, s32);
extern s32 func_150265CC(s32, s32, s32, s32, s32, struct127 *, struct131 *, f32 *, s32);
extern s32 func_15029BB8(s32, s32, s32, s32, s32, struct127 *, struct131 *, f32 *, s32, s32);
extern void func_1502A8A0(s32, s32, s32, s32, s32);


void func_150238D0(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_50D80/func_150238D8.s")

typedef struct {
    s8 unk0;
    u8 unk1;
    s16 unk2;
    s8 unk4;
    s8 unk5;
    s16 unk6;
} Elem15023DE0;

extern Elem15023DE0 *D_800C3688[][30];
extern u16 *D_800C35D8[];
extern s32 D_800C35B0[];
extern s32 D_800C3640[];

s32 func_15023BB0(Elem15023DE0 *arg0, s32 arg1, s32 arg2, Elem15023DE0 **arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10) {
    Elem15023DE0 *p;
    s32 time;

    if (D_800C3688[arg10][arg2] != NULL) {
        time = D_800C3688[arg10][arg2]->unk2;
        p = D_800C3688[arg10][arg2] + 4;
        while ((u32) (p - D_800C3688[arg10][arg2]) < D_800C35D8[arg10][arg2]) {
            if ((p->unk1 == 0) && (p->unk0 == 0)) {
                time = p->unk2;
            } else {
                if ((arg0 < p) && (arg1 == p->unk0) && ((arg4 == 0) || ((arg4 == 1) && (arg5 == p->unk2))) && ((arg6 == 0) || ((arg6 == 1) && (arg7 == p->unk4))) && ((arg8 == 0) || ((arg8 == 1) && (arg9 == p->unk5)))) {
                    *arg3 = p;
                    return time;
                }
                while (p->unk1 == 0) {
                    if (p->unk0 == 0) {
                        time = p->unk2;
                        break;
                    }
                    p++;
                    if ((u32) (p - D_800C3688[arg10][arg2]) >= D_800C35D8[arg10][arg2]) {
                        break;
                    }
                    if ((arg0 < p) && (arg1 == p->unk0) && ((arg4 == 0) || ((arg4 == 1) && (arg5 == p->unk2))) && ((arg6 == 0) || ((arg6 == 1) && (arg7 == p->unk4))) && ((arg8 == 0) || ((arg8 == 1) && (arg9 == p->unk5)))) {
                        *arg3 = p;
                        return time;
                    }
                }
                time += p->unk1;
            }
            p++;
        }
    }
    *arg3 = NULL;
    return D_800C3640[arg10];
}


void func_15023DE0(Elem15023DE0 **arg0, Elem15023DE0 **arg1, s32 *arg2, s32 *arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11) {
    Elem15023DE0 *p;
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
    time = D_800C3688[arg11][arg4]->unk2;
    while ((u32) (p - D_800C3688[arg11][arg4]) < D_800C35D8[arg11][arg4]) {
        if ((p->unk1 == 0) && (p->unk0 == 0)) {
            time = p->unk2;
        } else {
            if (time <= arg10) {
                if ((arg5 == p->unk0) && ((arg6 == 0) || ((arg6 == 1) && (arg7 == p->unk2))) && ((arg8 == 0) || ((arg8 == 1) && (arg9 == p->unk4)))) {
                    if (arg0 != NULL) {
                        *arg0 = p;
                    }
                    if (arg2 != NULL) {
                        *arg2 = time;
                    }
                }
            }
            if (time > arg10) {
                if ((arg5 == p->unk0) && ((arg6 == 0) || ((arg6 == 1) && (arg7 == p->unk2))) && ((arg8 == 0) || ((arg8 == 1) && (arg9 == p->unk4)))) {
                    *arg1 = p;
                    *arg3 = time;
                    return;
                }
            }
            while (p->unk1 == 0) {
                if (p->unk0 == 0) {
                    time = p->unk2;
                    break;
                }
                p++;
                if ((u32) (p - D_800C3688[arg11][arg4]) >= D_800C35D8[arg11][arg4]) {
                    break;
                }
                curTime = D_800C35B0[arg11];
                if (time <= curTime) {
                    if ((arg5 == p->unk0) && ((arg6 == 0) || ((arg6 == 1) && (arg7 == p->unk2))) && ((arg8 == 0) || ((arg8 == 1) && (arg9 == p->unk4)))) {
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
                    if ((arg5 == p->unk0) && ((arg6 == 0) || ((arg6 == 1) && (arg7 == p->unk2))) && ((arg8 == 0) || ((arg8 == 1) && (arg9 == p->unk4)))) {
                        *arg1 = p;
                        *arg3 = time;
                        return;
                    }
                }
            }
            time += p->unk1;
        }
        p++;
    }
}

void func_15024130(s32 arg0, s32 arg1) {
    s32 i;
    s32 off;
    Elem15024130 *e;

    i = 0;
    if (arg0 > 0) {
        off = 0; do {
            e = (Elem15024130 *)((u8 *)D_800C3D50 + off);
            func_1502A8A0(e->unk0, e->unk8, e->unkA, e->unk4, arg1);
            i++;
            off += 0xC;
        } while (i != arg0);
    }
}

extern u16 D_800C3C9A;

void func_150241B4(Elem15024130 *arr, s32 *count, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 idx;
    s32 i;

    if (*count >= D_800C3C9A) {
        func_15024130(*count, arg6);
        *count = 0;
        return;
    }
    idx = *count;
    for (i = 0; i < *count; i++) {
        if (arr[i].unk4 >= arg5) {
            idx = i;
            break;
        }
    }
    i = *count - 1;
    if (i >= idx) {
loop2:
        arr[i + 1] = arr[i];
        i--;
        if (i >= idx) goto loop2;
    }
    arr[idx].unk0 = arg2;
    arr[idx].unk8 = arg3;
    arr[idx].unkA = arg4;
    arr[idx].unk4 = arg5;
    (*count)++;
}
extern s32 D_800C35B8[];
extern u8 D_800C35C0[];
extern u8 D_800C3C8C;
extern u8 D_800C363A[];

void func_150242F8(s32 arg0, s32 arg1) {
    s32 i;
    Elem15023DE0 *p;
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
                    time = D_800C3688[arg1][i]->unk2;
                    while ((u32) (p - D_800C3688[arg1][i]) < D_800C35D8[arg1][i]) {
                        if ((p->unk1 == 0) && (p->unk0 == 0)) {
                            time = p->unk2;
                        } else {
                            if ((time <= D_800C35B0[arg1]) && (time > D_800C35B8[arg1])) {
                                for (;;) {
                                    if ((u32) (p - D_800C3688[arg1][i]) >= D_800C35D8[arg1][i]) {
                                        break;
                                    }
                                    if (((arg0 == 0) && (p->unk0 != 0x11)) || ((arg0 == 1) && (p->unk0 == 0x11))) {
                                        func_150241B4(D_800C3D50, &count, (s32) p, i, (u32) (p - D_800C3688[arg1][i]), time, arg1);
                                    }
                                    if ((p->unk1 != 0) || ((p->unk1 == 0) && (p->unk0 == 0))) {
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
                            while (p->unk1 == 0) {
                                if (p->unk0 == 0) {
                                    time = p->unk2;
                                    break;
                                }
                                p++;
                                if ((u32) (p - D_800C3688[arg1][i]) >= D_800C35D8[arg1][i]) {
                                    break;
                                }
                            }
                            time += p->unk1;
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
    f32 x;
    f32 y;
    f32 z;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 pad18[0xC];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    u8 unk30[0x14];
} Pos1502460C;

typedef struct {
    struct127 *obj;
    u8 id;
} ObjRef1502460C;

#define POSTAB ((Pos1502460C **) &D_800C3958)
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

#define cmd ((Elem15023DE0 *) arg0)
s32 func_1502460C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, struct127 *arg5, struct131 *arg6, f32 *arg7, s32 arg8) {

    if (D_800E0BF4 != 0 && (cmd->unk0 == 7 || cmd->unk0 == 0xE || cmd->unk0 == 0xF || cmd->unk0 == 0x10 || cmd->unk0 == 0x12)) {
        s32 flag;
        Rec1502460C *rec;
        u8 *text;
        u8 colors[3][3] = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xA0 }, { 0x3C, 0x3C, 0x3C } };

        if ((cmd->unk0 == 7 && D_800BEAAB != 0) || (cmd->unk0 == 0xE && D_800BEAAB != 4) ||
            (cmd->unk0 == 0xF && D_800BEAAB != 2) || (cmd->unk0 == 0x10 && D_800BEAAB != 3) ||
            (cmd->unk0 == 0x12 && D_800BEAAB != 1)) {
            return 1;
        }
        flag = cmd->unk4 == 0 ? 1 : 0;
        rec = func_150234A4(arg1, flag);
        if (rec != NULL) {
            if (cmd->unk4 == 0) {
                if (rec->flagC != 0) {
                    func_1516D2E0(rec->handle);
                }
                text = D_800C35E0[arg4][cmd->unk2];
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
                rec->unk1C = &POSTAB[arg4][arg1].unkC;
                rec->unk20 = (f32 *) &POSTAB[arg4][arg1].unk10;
                rec->unk2F = 1;
                rec->unk24 = &POSTAB[arg4][arg1].unk14;
                rec->unk2C = colors[cmd->unk5][0];
                rec->unk2D = colors[cmd->unk5][1];
                rec->unk2E = colors[cmd->unk5][2];
                if (cmd->unk5 == 2) {
                    rec->flags |= 0x40000;
                }
                rec->handle = func_1516A7B0(0, 0, 0, 0, &rec->flags, arg8);
            } else if (cmd->unk4 == 1) {
                func_15023440(rec, cmd->unk5);
            } else {
                if (cmd->unk5 == 0) {
                    rec->handle = func_1516D378(rec->handle);
                } else {
                    rec->handle = func_1516D3C4(rec->handle);
                }
                if (cmd->unk6 != 0) {
                    s32 idx = cmd->unk6 - 1;

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
    } else if (cmd->unk0 == 0xC) {
        Elem15023DE0 *p;
        s32 val;

        if (cmd->unk2 != 0) {
            if (cmd->unk6 != 0) {
                func_1507EB4C(arg5, cmd->unk2);
            } else {
                if (cmd->unk5 != 0) {
                    val = 0xFFFF;
                } else {
                    val = func_15023BB0(cmd, cmd->unk0, arg1, &p, 0, 0, 0, 0, 0, 0, arg4) - arg3;
                }
                func_1507E7E4(arg5, cmd->unk2 & 0xFF, 3, val & 0xFFFF, cmd->unk4);
            }
        }
    } else if (cmd->unk0 == 0xD) {
        struct126 *s;

        if (arg5 == NULL) {
            return 1;
        }
        if (cmd->unk4 == 0) {
            func_15083568(arg5, cmd->unk2 + 1, 1.0f, 0);
        } else if (cmd->unk4 == 1) {
            func_150836CC(arg5, cmd->unk2 + 1);
        } else {
            s = func_1503195C(arg5, cmd->unk2 + 1, 0);
            if (s != NULL) {
                if (cmd->unk4 == 2) {
                    FS16(s, 0x18) = D_80090214[cmd->unk5];
                } else if (cmd->unk4 == 3) {
                    FS16(s, 0x1A) = D_80090214[cmd->unk5];
                }
            }
        }
    } else if (cmd->unk0 == 4) {
        Elem15023DE0 *p;
        s32 val;

        if (arg5 == NULL) {
            return 1;
        }
        val = (func_15023BB0(cmd, cmd->unk0, arg1, &p, 0, 0, 0, 0, 0, 0, arg4) - arg3) / 2;
        if (val >= 0x100) {
            val = 0xFF;
        } else if (val == 0) {
            val = 1;
        }
        if (cmd->unk4 == 0) {
            FU8(arg5, 0x134) = cmd->unk2;
            FU8(arg5, 0x135) = val;
        }
    } else if (cmd->unk0 == 3) {
        if (cmd->unk2 >= D_800C363A[arg4]) {
            return 1;
        }
        if (D_800C35F0[arg4][cmd->unk2].unk0 == 1 && D_800C35F8 != cmd->unk2) {
            D_800C35F8 = cmd->unk2;
            D_800DBFF4[0] = 4;
            D_800DBFF0->unk5F0 |= 4;
        }
    } else if (cmd->unk0 == 9) {
        if (arg5 == NULL) {
            return 1;
        }
        ((u8 *) arg5 + 0x6C)[cmd->unk4] = cmd->unk2 + 10;
    } else if (cmd->unk0 == 1) {
        s32 v;

        if (arg5 == NULL) {
            return 1;
        }
        v = cmd->unk2;
        if (v == 0) {
            v = 0xFF;
        }
        FU8(arg5, 0x1C9) = v;
    } else if (cmd->unk0 == 5) {
        if (cmd->unk2 == 0x3B) {
            if (cmd->unk4 == 0) {
                D_800DDE18 = (s32) &POSTAB[arg4][arg1].unk28;
            } else {
                D_800DDE18 = 0;
            }
        } else if (cmd->unk2 == 8) {
            D_800C3656[arg4] = !cmd->unk4;
        } else if (cmd->unk2 == 9) {
            D_800C3658[arg4] = !cmd->unk4;
        } else if (cmd->unk2 == 0xD) {
            ((u8 *) &D_800C3654)[arg4] = !cmd->unk4;
        } else if (cmd->unk2 == 0x12) {
            D_800C365A[arg4] = !cmd->unk4;
        } else if (cmd->unk2 == 0x13) {
            D_800C365C[arg4] = cmd->unk4;
        } else if (cmd->unk2 == 0x15) {
            D_800C365E[arg4] = !cmd->unk4;
        } else if (cmd->unk2 == 0x14) {
            if (arg5 == NULL) {
                return 1;
            }
            if (cmd->unk4 == 0) {
                func_15053430(arg5);
            } else {
                func_15060F28(arg5, 0);
            }
        } else if (cmd->unk2 == 0x41) {
            u8 colors[2][3] = { { 0xFF, 0, 0 }, { 0, 0, 0 } };

            if (arg5 == NULL) {
                return 1;
            }
            if (cmd->unk4 == 0) {
                FU8(arg5, 0xA5) = colors[cmd->unk5][0];
                FU8(arg5, 0xA6) = colors[cmd->unk5][1];
                FU8(arg5, 0xA7) = colors[cmd->unk5][2];
                FU8(arg5, 0xA4) = 1;
                FPTR(arg5, 0xA0) = &POSTAB[arg4][arg1].unk30;
            } else {
                FU8(arg5, 0xA4) = 0;
                FPTR(arg5, 0xA0) = NULL;
            }
        } else if (cmd->unk2 == 0x44) {
            s32 a;
            s32 c;

            if (cmd->unk5 != 0) {
                c = cmd->unk6;
                a = 2;
            } else {
                c = 0;
                a = 1;
            }
            func_1501C730(a, cmd->unk4, c, 0, 0);
        } else if (cmd->unk2 == 0x17) {
            if (arg5 == NULL) {
                return 1;
            }
            FU8(arg5, 0x1FF) = !cmd->unk4;
            if (FU8(arg5, 0x1FF) != 0) {
                FU8(arg5, 0x1FF) += cmd->unk5;
            }
            if (cmd->unk4 == 0) {
                FU8(arg5, 0x201) = cmd->unk6;
            }
        } else if (cmd->unk2 == 0x16) {
            if (arg5 == NULL) {
                return 1;
            }
            FU8(arg5, 0x206) = !cmd->unk4;
            if (FU8(arg5, 0x206) != 0) {
                FU8(arg5, 0x206) += cmd->unk5;
            }
            if (cmd->unk4 == 0) {
                FU8(arg5, 0x208) = cmd->unk6;
            }
        } else if (cmd->unk2 == 0x18) {
        } else if (cmd->unk2 == 0x1D) {
            if (D_800C35F0[arg4][arg1].unk0 == 3 && arg6 != NULL) {
                if (cmd->unk4 == 0) {
                    FU8(arg6, 0x6E) = 1;
                } else {
                    FU8(arg6, 0x6E) = 0;
                }
            }
        } else if (cmd->unk2 == 0x1E) {
            D_800C35C2 = cmd->unk4 + 1;
            D_800C35C3 = D_800BE9F0;
        } else if (cmd->unk2 == 3) {
            struct131 *actor = func_151149AC(FU8(cmd, 7));

            if (actor == NULL) {
                return 1;
            }
            FU8(actor, 0x73) &= ~3;
            if (cmd->unk5 == 0) {
                FU8(actor, 0x73) |= 2;
            } else {
                FU8(actor, 0x73) |= 1;
            }
        } else if (cmd->unk2 == 0x6C) {
            f32 pos[3];
            f32 pos2[3];

            switch (FU8(cmd, 4)) {
                case 0:
                    pos[0] = POSTAB[arg4][arg1].x;
                    pos[1] = POSTAB[arg4][arg1].y;
                    pos[2] = POSTAB[arg4][arg1].z;
                    func_151BE850(pos, POSTAB[arg4][arg1].unk10, 1, 0, 2, 0xFF, 1);
                    break;
                case 1:
                    pos2[0] = POSTAB[arg4][arg1].x;
                    pos2[1] = POSTAB[arg4][arg1].y;
                    pos2[2] = POSTAB[arg4][arg1].z;
                    func_151A9390(5, 1, 0, pos2, -10000.0f, 100.0f, -1, arg8, 1);
                    break;
                case 2:
                    func_151494E0(0, cmd->unk5 ? 0x2B : 0x2C);
                    break;
                case 3:
                    if (cmd->unk5) {
                        func_1515F170(1, 0);
                        func_1515F170(2, 1);
                    } else {
                        func_1515F170(1, 1);
                        func_1515F170(2, 0);
                    }
                    break;
                case 4:
                    func_151645C4(cmd->unk5);
                    break;
                case 5:
                    func_151616D0(0x28, 0x17, 0);
                    break;
                case 6:
                    func_151CD35C(cmd->unk5);
                    break;
                case 7:
                    func_1515F170(3, cmd->unk5 ? 0 : 1);
                    break;
                case 8:
                    func_150B648C(cmd->unk5);
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
                    func_151CD394(cmd->unk5);
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
                    func_151494E0(0, cmd->unk5 != 0 ? 0x55 : 0x56);
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
        } else if (cmd->unk2 == 0x6E) {
            u8 *list;
            s32 count;

            list = func_1519003C(arg5, &count);
            if (list != NULL && cmd->unk6 < count) {
                func_1518F51C(arg5, list[cmd->unk6], -1, -1, -1, -1, 0, 0, 0xFF, 1);
            }
        } else if (cmd->unk2 == 5) {
            s32 idx;
            Elem15023DE0 *p;
            s32 val;
            s32 ids[4] = { 0xD, 0xE, 0xF, 0x10 };
            u8 c;

            if (cmd->unk4 == 0) {
                idx = cmd->unk5;
                if (idx >= 4) {
                    idx = 3;
                }
                val = func_15023BB0(cmd, cmd->unk0, arg1, &p, 1, cmd->unk2, 0, 0, 1, cmd->unk5, arg4) - arg3;
                if (cmd->unk6 != 0) {
                    val = 999;
                    c = 0xFF;
                } else {
                    c = arg8;
                }
                func_1518D1C0(arg5, ids[idx], val, 0, c, 0, "Cutscene 1");
            }
        } else if (cmd->unk2 == 0x36) {
            if (cmd->unk4 == 0) {
                func_1519D030(arg5, 0, 300, 0, arg8, 0);
            } else if (cmd->unk4 == 1) {
                ObjRef1502460C ref;

                ref.obj = arg5;
                ref.id = arg5->unique_id;
                func_15147D64(&ref, 7);
            }
        } else if (cmd->unk2 == 0x30) {
            u8 v;

            if (arg5 == NULL) {
                return 1;
            }
            {
                switch (cmd->unk4) {
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
        } else if (cmd->unk2 == 0x46) {
            if (cmd->unk4 != 0) {
                func_150C5C74((s32) arg5);
            } else {
                func_150C5C9C((s32) arg5);
            }
        } else if (cmd->unk2 == 0x25) {
            if (cmd->unk4 != 0) {
                D_800C364C = cmd->unk4;
            } else {
                D_800C364C = D_800D9B20;
            }
            func_1510B458(0, D_800C364C, D_800C3650);
        } else if (cmd->unk2 == 0x58) {
            if (cmd->unk6 != 0) {
                D_800C3650 = cmd->unk6 * 10;
            } else {
                D_800C3650 = D_800D9B1C;
            }
            func_1510B458(0, D_800C364C, D_800C3650);
        } else if (cmd->unk2 == 0x3C) {
            s32 v;

            if (arg5 != NULL) {
                v = cmd->unk4;
                v <<= 16;
                v |= cmd->unk6;
                if (cmd->unk5 == 0) {
                    arg5->unk94 = v;
                } else if (cmd->unk5 == 1) {
                    arg5->unk94 |= v;
                } else {
                    arg5->unk94 &= ~(v | 1);
                }
            }
        } else if (cmd->unk2 == 0x3D) {
            s32 r;

            if (arg5 != NULL) {
                if (cmd->unk4 == 0) {
                    r = func_15083FB0(FU8(cmd, 7));
                    if (r != -1) {
                        arg5->unk101 |= 4;
                        arg5->unk65 = r + 1;
                        arg5->unk5C = cmd->unk5;
                    }
                } else {
                    arg5->unk65 = 0;
                }
            }
        } else if (cmd->unk2 == 0x42) {
            if (cmd->unk4 == 0) {
                D_800D9890 = 1;
            } else if (cmd->unk4 == 1) {
                D_800D9890 = 4;
            } else if (cmd->unk4 == 3) {
                D_800D9890 = 2;
            } else if (cmd->unk4 == 4) {
                D_800D9890 = 5;
            } else if (cmd->unk4 == 5) {
                D_800D9890 = 6;
            } else if (cmd->unk4 == 6) {
                D_800D9890 = 7;
            } else if (cmd->unk4 == 7) {
                D_800D9890 = 8;
            } else if (cmd->unk4 == 8) {
                D_800D9890 = 9;
            } else if (cmd->unk4 == 9) {
                D_800D9890 = 10;
            }
        } else if (cmd->unk2 == 0x43) {
            s32 time;
            Elem15023DE0 *p;

            if (cmd->unk4 == 0) {
                time = func_15023BB0(cmd, cmd->unk0, arg1, &p, 1, cmd->unk2, 0, 0, 0, 0, arg4) - arg3;
                func_150B7220(POSTAB[arg4][arg1].unk24, POSTAB[arg4][arg1].unk28, time, arg8);
            }
        } else if (cmd->unk2 == 0x45) {
            s32 time;
            Elem15023DE0 *p;

            if (cmd->unk4 == 0) {
                time = func_15023BB0(cmd, cmd->unk0, arg1, &p, 1, cmd->unk2, 0, 0, 0, 0, arg4) - arg3;
                func_151A4FD0(0, time, 0xFF, 0, 0, D_800BE9E8, 0, 0);
            }
        } else if (cmd->unk2 == 0x49) {
            if (arg5 != NULL && cmd->unk4 < 3) {
                ((s32 *) ((u8 *) arg5 + 0x2E4))[cmd->unk4] = cmd->unk6;
            }
        } else if (cmd->unk2 == 0x4B) {
            if (arg5 != NULL) {
                arg5->unk9C = (cmd->unk6 & 0xFFFF) | ((cmd->unk5 & 0xFF) << 16) | ((cmd->unk4 & 0xFF) << 24);
            }
        } else if (cmd->unk2 == 0x4A) {
            if (cmd->unk4 == 0) {
                func_10011FB0(cmd->unk5);
            } else {
                func_10011FB0(0);
            }
        } else if (cmd->unk2 == 0x54) {
            D_800C3660[arg4] = ~cmd->unk4;
        } else if (cmd->unk2 == 0x55) {
            func_150C851C(cmd->unk4);
        } else if (cmd->unk2 == 0x59) {
            D_800C3C8B = 1;
        } else if (cmd->unk2 == 0x5A) {
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
        } else if (cmd->unk2 == 0x71) {
            D_800CC2D0->unkCC = 0;
            FU8(D_800CC2D0->unk31C, 0x4E) = 0;
            D_800CC2D0->unk65 = 0;
            FU8(D_800CC2D0->unk31C, 0x27) = 0;
            D_800CC2D0->unkF8 &= ~0x01000000;
            if (arg5 != NULL) {
                arg5->unk13C = 0;
            }
        } else if (cmd->unk2 == 0x37) {
            s32 val;
            Elem15023DE0 *p;
            s32 v;
            u8 colors[3][3] = { { 0xFF, 0, 0 }, { 0, 0x35, 0x46 }, { 0xFF, 0xFF, 0xFF } };

            val = func_15023BB0(cmd, cmd->unk0, arg1, &p, 1, cmd->unk2, 0, 0, 0, 0, arg4) - arg3;
            if (cmd->unk4 == 0) {
v = cmd->unk6 != 0 ? cmd->unk6 : 0x7F;
                func_1517F488(colors[cmd->unk5][0], colors[cmd->unk5][1], colors[cmd->unk5][2], v, val, 0);
            } else {
                D_800DDE10 = 0;
            }
        } else if (cmd->unk2 == 0x38) {
            s32 val;
            Elem15023DE0 *p;
            s32 v;

            val = func_15023BB0(cmd, cmd->unk0, arg1, &p, 1, cmd->unk2, 0, 0, 0, 0, arg4) - arg3;
            if (cmd->unk4 == 0) {
v = cmd->unk6 != 0 ? cmd->unk6 : 4;
                func_1517F720(cmd->unk5, val, v, 0x7F, 1);
            } else {
                D_800DDE08 = 0;
            }
        } else if (cmd->unk2 == 0x5C) {
            Elem15023DE0 *p;
            s32 total;
            s32 start;
            s32 end;

            if (cmd->unk4 == 0) {
                total = func_15023BB0(cmd, cmd->unk0, arg1, &p, 1, cmd->unk2, 1, 1, 1, cmd->unk5, arg4);
                start = func_15023BB0(cmd, cmd->unk0, arg1, &p, 1, cmd->unk2, 1, 2, 1, cmd->unk5, arg4) - arg3;
                if (p == NULL) {
                    start = 0;
                }
                end = total - func_15023BB0(cmd, cmd->unk0, arg1, &p, 1, cmd->unk2, 1, 3, 1, cmd->unk5, arg4);
                if (p == NULL) {
                    end = 0;
                }
                if (D_800BE9F0 == 0x18) {
                    func_150F5420(start, total - arg3 - end - start, end, cmd->unk5);
                } else if (D_800BE9F0 == 0x21) {
                    func_1510A930(start, total - arg3 - end - start, end);
                }
            }
        } else if (cmd->unk2 == 0x62) {
            FU8(D_800B0DF0, 8) = 0;
        } else if (cmd->unk2 == 0x7C) {
            D_800C3681 = !cmd->unk4;
        } else if (cmd->unk2 == 0x84) {
            D_800C3682 = !cmd->unk4;
        } else if (cmd->unk2 == 0x85) {
            D_800C3683 = !cmd->unk4;
        } else if (cmd->unk2 == 0x63) {
            D_80041FDC = cmd->unk6;
        } else if (cmd->unk2 == 0x64) {
            if (cmd->unk4 == 0) {
                D_800DDE40 = 1;
                D_800DDE44 = &POSTAB[arg4][arg1].unk28;
                D_800DDE48 = &POSTAB[arg4][arg1].unk24;
                D_800DDE4C = &POSTAB[arg4][arg1].unk2C;
            } else {
                D_800DDE40 = 0;
            }
        } else if (cmd->unk2 == 0x66) {
            if (cmd->unk4 == 0) {
            }
        } else if (cmd->unk2 == 0x67) {
            func_151DE7D4();
        } else if (cmd->unk2 == 0x73) {
            func_151E53E8(0);
        } else if (cmd->unk2 == 0x69) {
            if (arg5 != NULL) {
                if (cmd->unk4 == 0) {
                    arg5->gravity = 4.0f;
                    arg5->y_velocity = 20.0f;
                } else if (cmd->unk4 == 1) {
                    arg5->gravity = 0.0f;
                    arg5->y_velocity = 0.0f;
                } else if (cmd->unk4 == 2) {
                    func_150836CC(arg5, 0x11);
                    arg5->unkA9 = 0;
                } else if (cmd->unk4 == 3) {
                    D_800C3E78 = arg5 - D_800CC2D0;
                    D_800CC2D0->unk13C = D_800C3E78 + 100;
                    D_800CC2D0->xz_velocity = 0.0f;
                    FS16(D_800CC2D0->unk31C, 0x18) = 0;
                    func_1505959C(arg5, 0);
                    *(u32 *) &arg5->unk5C = 9;
                    arg5->unk65 = 1;
                    arg5->unk101 |= 4;
                } else if (cmd->unk4 == 4) {
                    FU8(D_800CC2D0->unk31C, 0x17) = 1;
                } else if (cmd->unk4 == 5) {
                    D_800D154C = arg5;
                    func_15073F1C();
                } else if (cmd->unk4 == 6) {
                    FU8(arg5->unk31C, 0x4E) = 0;
                }
            }
        } else if (cmd->unk2 == 0x6A) {
            D_800C3674 = cmd->unk6;
        } else if (cmd->unk2 == 0x7A) {
            D_800C367C = cmd->unk5 * 60;
            D_800C3678 = cmd->unk6;
        } else if (cmd->unk2 == 0x6B) {
        } else if (cmd->unk2 == 0x6D) {
            D_800C3662 = !cmd->unk4;
        } else if (cmd->unk2 == 0x74) {
            D_800BEA0C = cmd->unk5;
        } else if (cmd->unk2 == 0x6F) {
            if (arg5 != NULL) {
                ((u8 *) arg5 + 0x68)[cmd->unk4] = cmd->unk5;
            }
        } else if (cmd->unk2 == 0x70) {
            D_800C3C8C = 1;
        } else if (cmd->unk2 == 0x72) {
            if (arg5 != NULL) {
                FF32(arg5, 0xC8) = FS16(D_800D1C90[arg5->id], 6);
            }
        } else if (cmd->unk2 == 0x78) {
            D_800C3C8D = 1;
        } else if (cmd->unk2 == 0x79) {
            if (arg6 != NULL) {
                FS32(arg6, 0x84) = 0;
                FS32(arg6, 0x80) = 0;
                FS32(arg6, 0x7C) = 0;
            }
        } else if (cmd->unk2 == 0x7B) {
            if (D_800BE9F0 == 0x1D) {
                D_800C3680 = 1;
            }
        } else if (cmd->unk2 == 0x7D) {
            if (arg5 != NULL && cmd->unk4 == 0) {
                arg5->unk5 = 1;
            }
        } else if (cmd->unk2 == 0x7E) {
            if (arg5 != NULL) {
                D_800D18D0 = arg5 - D_800CC2D0;
            }
        } else if (cmd->unk2 == 0x7F) {
            D_800DBEAC = 1;
        } else if (cmd->unk2 == 0x80) {
            D_800BE9ED = cmd->unk4;
        } else if (cmd->unk2 == 0x81) {
            func_15093818(cmd->unk4 ^ 1);
            D_800D2450 = cmd->unk6 * 30;
        } else if (cmd->unk2 == 0x82) {
            D_800C3C9D = cmd->unk4;
            if (D_800C3C9D > 100) {
                D_800C3C9D = 100;
            }
        } else if (cmd->unk2 == 0x83) {
            D_800C3C9E = cmd->unk6;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
} Ev3958;

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
extern void func_150F802C(Ev3958 *, s16, u8, s32);
extern void func_150AF738(s16, u8, s32);
extern void func_150F5C08(struct127 *, s16, u8, s32);
extern void func_1519EB8C(Ev3958 *, u16, s16, u8, s32);
extern void func_150C4120(struct127 *, s16, u8, s32);
extern void func_150F6890(struct127 *, s16, u8, s32);
extern void func_150D0134(u8, s32, s16, u8, s32);
extern void func_150D02B4(u8, s32, s16, u8, s32);
extern void func_150D04C4(u8, s32, s16, u8, s32);
extern void func_150CF680(s16, u8, s32);
extern void func_151925C4(struct127 *, s16, u8, s32);
extern void func_1519ED84(Ev3958 *, u16, s16, u8, s32);
extern void func_150FAE18(s16, u8, s32);
extern s32 func_15046C80(f32 *, s32, f32, EvFloor *);
extern void func_15136698(f32, f32, u8, s32, s32, f32 *, f32 *, s32, s32, s32, s32);
extern void func_150E3340(f32 *, s32 *, Ev3958 *, s16);
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

#define EV ((Elem15023DE0 *)arg0)
#define POS(a4, a1) (&((Ev3958 **)&D_800C3958)[a4][a1])

s32 func_150265CC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, struct127 *arg5, struct131 *arg6, f32 *arg7, s32 arg8) {

    if (EV->unk0 == 0xB) {
        s32 vol;
        s32 vol2;
        s32 delay;
        Elem15023DE0 *found;

        if (EV->unk4 == 0) {
            if (EV->unk6 == 0) {
                vol = 0x10000;
            } else {
                vol = (EV->unk6 << 16) / 255;
            }
            func_1001263C(EV->unk2, vol, 0x40);
        } else if (EV->unk4 == 1) {
            delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 1, 2, 0, 0, arg4) - arg3;
            if (EV->unk6 == 0) {
                vol2 = 0x10000;
            } else {
                vol2 = (EV->unk6 << 16) / 255;
            }
            delay = (delay * 0x5604) / 60;
            func_151F2D6C(vol2, delay);
        } else if (EV->unk4 == 3) {
            func_151F2BA8();
        } else if (EV->unk4 == 4) {
            func_151F2BE8();
        } else if (EV->unk4 == 5) {
            func_151F2C4C();
        }
    } else if (EV->unk0 == 0x11) {
        if (EV->unk4 != 0) {
            D_800C3960[arg4][arg1][EV->unk2] = arg2;
        } else {
            D_800C3960[arg4][arg1][EV->unk2] = 0xFF;
        }
    } else if (EV->unk0 == 5) {
        if (EV->unk2 == 0x11) {
            s32 delay;
            Elem15023DE0 *found;
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

            delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
            if (EV->unk4 == 0) {
                if (EV->unk6 == 0) {
                    dur = 0x1000;
                } else if (EV->unk6 > 200) {
                    dur = EV->unk6;
                } else {
                    dur = 200;
                }
                if (EV->unk5 >= 3) {
                    idx = 0;
                } else {
                    idx = EV->unk5;
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
        } else if (EV->unk2 == 0xA) {
            s32 delay;
            Elem15023DE0 *found;

            delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
            if (EV->unk4 == 0) {
                func_151B9BF0(3, 3, 0, 0, 0, 0x320, 0x4B0, 0x190, 0xC8, 0x66, 0x1C7, 2, delay, arg7, arg8, 0);
            }
        } else if (EV->unk2 == 0xB) {
            s32 delay;
            Elem15023DE0 *found;

            delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
            if (EV->unk4 == 0) {
                func_151B9BF0(0x2F, 1, 0, 0, 0, 0x7D0, 0x3E8, 0x190, 0xC8, 0x400, 0x400, 6, delay, arg7, arg8, 0);
            }
        } else if (EV->unk2 == 0xC) {
            Ev15167AD8 sp;

            sp.unk0 = D_8008CA50;
            sp.unk4 = 0;
            sp.unk6 = 0x100;
            sp.unk8 = arg7[0];
            sp.unkA = arg7[1] + EV->unk4;
            sp.unkC = arg7[2];
            sp.unkE = 0x5DC;
            sp.unk10 = 0x5DC;
            sp.unk12 = -10;
            sp.unk14 = 0;
            func_15167AD8(&sp, arg8, 1);
        } else if (EV->unk2 == 0x19) {
            s32 delay;
            Elem15023DE0 *found;
            s32 count;

            delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
            if (EV->unk4 == 0) {
                count = EV->unk5;
                if (count == 0) {
                    count = 0xC;
                }
                func_15177F90(count, arg7, 0xFF, arg7[0], arg7[1], arg7[2], 0, delay, arg8, 0, EV->unk6);
            }
        } else if (EV->unk2 == 0x1A) {
            if (EV->unk4 != 0 && EV->unk4 == 1) {
                f32 from[3];
                f32 to[3];

                from[0] = D_800C3868[arg4][arg1][0];
                from[1] = D_800C3868[arg4][arg1][1];
                from[2] = D_800C3868[arg4][arg1][2];
                to[0] = POS(arg4, arg1)->unk0;
                to[1] = POS(arg4, arg1)->unk4 + POS(arg4, arg1)->unk38;
                to[2] = POS(arg4, arg1)->unk8;
                func_150B0094(from, to, arg8, 1);
            } else {
                func_150E1AB0(0, D_800C3868[arg4][arg1][0], D_800C3868[arg4][arg1][1], D_800C3868[arg4][arg1][2], POS(arg4, arg1)->unk0, POS(arg4, arg1)->unk4 + POS(arg4, arg1)->unk38, POS(arg4, arg1)->unk8, 40.0f, 0.0f, 8.0f, 60.0f, 0x258, 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            }
        } else if (EV->unk2 == 0x1C) {
            s32 anim;

            if (arg5 == NULL) {
                return 1;
            }
            switch (EV->unk4) {
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
        } else if (EV->unk2 == 0x1B) {
            s32 delay;
            Elem15023DE0 *found;

            delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
            if (EV->unk4 == 0) {
                switch (*(u16 *)&EV->unk6) {
                    case 0:
                        func_1516E778(arg7, delay, EV->unk5, arg8, 0);
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
        } else if (EV->unk2 == 0x1F) {
            if (arg5 == NULL) {
                return 1;
            }
            func_150BA4C0(arg5, arg8, 1);
        } else if (EV->unk2 == 0) {
            if (arg5 == NULL) {
                return 1;
            }
            func_151A3390(arg5, arg8);
        } else if (EV->unk2 == 0x20) {
            func_15165F80(-1, arg7[0], arg7[1], arg7[2], 0x14, 0x14, 1, arg8, 1);
        } else if (EV->unk2 == 6) {
        } else if (EV->unk2 == 7) {
            func_150B2494(arg5, 9, -1);
        } else if (EV->unk2 == 0x21) {
            s32 delay;
            Elem15023DE0 *found;

            if (EV->unk4 == 0) {
                delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
                func_15136A50(arg7, arg7 + 1, arg7 + 2, delay, arg8, 0);
            }
        } else if (EV->unk2 == 0x60) {
            s32 delay;
            Elem15023DE0 *found;

            if (EV->unk4 == 0) {
                delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
                func_150CE150(arg5, delay, arg8, 0);
            }
        } else if (EV->unk2 == 0x65) {
            s8 val[4];
            s32 pad;
            s32 delay;
            Elem15023DE0 *found;

            if (EV->unk4 == 2) {
                switch (EV->unk5) {
                    case 0x11:
                    case 0x12:
                    case 0x15:
                        val[0] = EV->unk6;
                        func_151494E0((s32)val, 0x51);
                        break;
                    case 0x13:
                        func_150CFC38(((f32)EV->unk6 * 0.009999999776f * 300.0f) - 150.0f);
                        break;
                }
            }
            if (EV->unk4 == 0) {
                delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 1, 1, 1, EV->unk5, arg4) - arg3;
                switch (EV->unk5) {
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

                        p[0] = POS(arg4, arg1)->unk0;
                        p[1] = POS(arg4, arg1)->unk4;
                        p[2] = POS(arg4, arg1)->unk8;
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
                        func_150F802C(POS(arg4, arg1), delay, arg8, 1);
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

                        id = EV->unk6;
                        if (id >= 0xE9) {
                            id = 0;
                        }
                        func_1519EB8C(POS(arg4, arg1), id, delay, arg8, 1);
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
                        func_150D0134(EV->unk6, D_800C35E0[arg4][EV->unk6], delay, arg8, 1);
                        break;
                    case 18:
                        func_150D02B4(EV->unk6, D_800C35E0[arg4][EV->unk6], delay, arg8, 1);
                        break;
                    case 19:
                        func_150CF680(delay, arg8, 1);
                        break;
                    case 20:
                        func_151925C4(arg5, delay, arg8, 1);
                        break;
                    case 21:
                        func_150D04C4(EV->unk6, D_800C35E0[arg4][EV->unk6], delay, arg8, 1);
                        break;
                    case 22: {
                        u16 id;

                        id = EV->unk6;
                        if (id >= 0xCF) {
                            id = 0;
                        }
                        func_1519ED84(POS(arg4, arg1), id, delay, arg8, 1);
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

                        pos[0] = POS(arg4, arg1)->unk0;
                        pos[1] = POS(arg4, arg1)->unk4 + 100.0f;
                        pos[2] = POS(arg4, arg1)->unk8;
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
                            func_15136698((func_150ADA68() * 0.02000000142f) + 0.09000000358f, (func_150ADA68() * 0.0004399999743f) + 0.0008799999487f, (func_150ADA20() % 101) + 0x9B, 0xFF, delay, &floor.unk4, pos, !EV->unk6, 1, arg8, 1);
                        }
                        break;
                    }
                    case 25: {
                        f32 from[3];
                        s32 to[3];

                        from[0] = D_800C3868[arg4][arg1][0];
                        from[1] = D_800C3868[arg4][arg1][1];
                        from[2] = D_800C3868[arg4][arg1][2];
                        to[0] = POS(arg4, arg1)->unk0;
                        to[1] = POS(arg4, arg1)->unk4;
                        to[2] = POS(arg4, arg1)->unk8;
                        func_150E3340(from, to, POS(arg4, arg1), delay);
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
        } else if (EV->unk2 == 0x5B) {
            s32 delay;
            Elem15023DE0 *found;

            if (EV->unk4 == 0) {
                delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
                func_151B5240((EV->unk4 != 0) ? 5000 : delay, arg8, 0);
            }
        } else if (EV->unk2 == 0x5D) {
            s32 delay;
            Elem15023DE0 *found;

            if (EV->unk4 == 0) {
                delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
                func_15192D48(arg5, delay, arg8, 0);
            }
        } else if (EV->unk2 == 0x5E) {
            s32 delay;
            Elem15023DE0 *found;

            if (EV->unk5 == 1) {
                func_151B09BC(arg5, 0, 1000, arg8, 0);
            } else if (EV->unk4 == 0) {
                delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
                func_151B09BC(arg5, 0, delay, arg8, 0);
            }
        } else if (EV->unk2 == 0x4C) {
            s32 delay;
            Elem15023DE0 *found;
            u8 *list;
            s32 count;

            if (EV->unk4 == 0) {
                delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
                if (arg5->id == 0x49 || arg5->id == 0x58) {
                    list = func_1519003C(arg5, &count);
                    if (list != NULL && EV->unk6 < count) {
                        func_1518F51C(arg5, list[EV->unk6], delay, -1, -1, -1, 0, 0, arg8, 1);
                    }
                } else {
                    func_1518D1C0(arg5, EV->unk5, delay, 0, arg8, 1, "Cutscene 2");
                }
            }
        } else if (EV->unk2 == 0x53) {
            if (EV->unk4 == 0) {
                s32 t;
                Elem15023DE0 *found;

                t = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4);
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
        } else if (EV->unk2 == 0x50) {
            s32 delay;
            Elem15023DE0 *found;

            if (EV->unk4 == 0) {
                delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
                func_150E9FE0(arg5, delay, arg8, 0);
            }
        } else if (EV->unk2 == 0x51) {
            s32 delay;
            Elem15023DE0 *found;

            if (EV->unk4 == 0) {
                delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
                func_150EA500(arg5, delay, arg8, 0);
            }
        } else if (EV->unk2 == 0x48) {
            f32 offset[2];
            s32 delay;
            Elem15023DE0 *found;

            if (EV->unk4 == 0) {
                delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 1, EV->unk5, arg4) - arg3;
                offset[0] = (func_150ADA68() * 200.0f) + -100.0f;
                offset[1] = (func_150ADA68() * 300.0f) + -150.0f;
                func_151356D4(offset, (func_150ADA68() * 20.0f) + 10.0f, ((func_150ADA68() * 65.0f) + 35.0f) * 0.001000000047f, ((func_150ADA68() * 160.0f) + 200.0f) * 0.001000000047f, (func_150ADA20() % 101) + 100, 0xFF, (func_150ADA20() % 3) + 4, delay, 4, 0x40, arg8, 1);
            }
        } else if (EV->unk2 == 0xF) {
            s32 delay;
            Elem15023DE0 *found;

            delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
            if (EV->unk4 == 0) {
                func_150C7020(arg5, (s16)delay, arg8, 1);
            }
        } else if (EV->unk2 == 0x3E || EV->unk2 == 0x3F) {
        } else if (EV->unk2 == 0x32) {
            s32 delay;
            Elem15023DE0 *found;

            delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
            if (EV->unk4 == 0) {
                func_150DB518(arg7, &POS(arg4, arg1)->unk10, &POS(arg4, arg1)->unk18, &POS(arg4, arg1)->unk1C, &POS(arg4, arg1)->unk20, delay, arg8, 0);
            }
        } else if (EV->unk2 == 0x34) {
            s32 delay;
            Elem15023DE0 *found;
            u8 t;
            u8 kind;
            EvNode *node;

            delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
            if (EV->unk4 == 0) {
                t = (EV->unk5 <= 0) ? EV->unk5 : 0;
                if (t == 0) {
                    kind = 0xD;
                }
                node = *(EvNode **)((u8 *)arg5 + 0x2F4);
                while (node != NULL) {
                    if (node->unk1C == kind || func_1514D4B8(D_8008ABE8[kind]->unk4, node->unk1C) != 0) {
                        node->unk10->unk30 = func_1519E970(delay, node->unk10, 0, &POS(arg4, arg1)->unk30, &POS(arg4, arg1)->unk20, arg8, 0);
                    }
                    node = node->unk14;
                }
            }
        } else if (EV->unk2 == 0x23) {
            s32 delay;
            Elem15023DE0 *found;

            if (arg5 == NULL) {
                return 1;
            }
            delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
            if (EV->unk4 == 0) {
                func_1516F2F8(arg5 - D_800CC2D0, delay, delay, 0, arg8, 0);
            }
        } else if (EV->unk2 == 0x22) {
            s32 delay;
            Elem15023DE0 *found;

            if (arg5 == NULL) {
                return 1;
            }
            delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
            if (EV->unk4 == 0) {
                func_1516F1C0(arg5 - D_800CC2D0, delay, arg8, 0);
            }
        } else if (EV->unk2 == 0x52) {
            if (arg5 == NULL) {
                return 1;
            }
            func_15136404(arg5, 90.0f, arg8, 1);
        } else if (EV->unk2 == 0x5F) {
            switch (*(u8 *)&EV->unk4) {
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
                                func_150E06D8(arg5, EV->unk5, arg8, 1);
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

                    p[0] = POS(arg4, arg1)->unk0;
                    p[1] = POS(arg4, arg1)->unk4;
                    p[2] = POS(arg4, arg1)->unk8;
                    func_151BE850(p, POS(arg4, arg1)->unk10, 1, 0, 2, arg8, 1);
                    break;
                }
                case 4:
                    func_151CFA4C(arg5, arg8, 1);
                    break;
                case 5:
                    if (arg5 != NULL) {
                        struct126 *bone;

                        bone = NULL;
                        if (EV->unk6 != 0) {
                            bone = func_1503195C(arg5, EV->unk5 ? 0xA4 : 0xA5, 0);
                        }
                        func_151C62D0(arg5, EV->unk5, 1, bone, EV->unk5 ? 0xA4 : 0xA5, arg8, 1);
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

                        switch (EV->unk5) {
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
                        func_15143134(sel, out, (u8 *)arg5->unk1D4 + (bones[EV->unk5] << 6));
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
                    id = EV->unk5;
                    if (id >= 0xE9) {
                        id = 0;
                    }
                    if (EV->unk6 != 0) {
                        scale = EV->unk6 * 0.009999999776f;
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
                    if (EV->unk5 != 0) {
                        size *= 0.349999994f;
                    }
                    func_15107700(arg5, func_150ADA20() & 0xFF, (func_150ADA20() % 129) - 0x3F, (func_150ADA20() % 21) + 0xA, 4, 40.0f, size, 0, 2, colour, arg8, 1);
                    break;
                }
                case 16:
                    func_150F16DC(arg5, EV->unk5, ((u8 *)EV)[7], arg8, 1);
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

                        func_15143134(offs[EV->unk5], out, arg5->unk1D4);
                        func_15131EE4(out, 0.6460000277f, arg8, 0);
                    }
                    break;
                }
                case 19: {
                    if (arg5->unk1D4) {
                        f32 out[3];
                        const f32 offs[8][3] = { { 0.0f, -14.0f, 18.0f }, { 0.0f, -14.0f, 18.0f }, { 0.0f, -14.0f, 18.0f }, { 0.0f, -14.0f, 18.0f }, { -5.0f, 25.0f, -7.0f }, { -11.0f, 28.0f, -5.0f }, { -4.0f, 30.0f, -4.0f }, { 3.0f, 30.0f, -8.0f } };
                        const u8 bones[8] = { 7, 8, 9, 10, 13, 14, 15, 16 };

                        func_15143134(offs[EV->unk5], out, (u8 *)arg5->unk1D4 + (bones[EV->unk5] << 6));
                        func_15131EE4(out, 6.990000248f, arg8, 0);
                    }
                    break;
                }
                case 20: {
                    Ev3958 *pos;
                    struct {
                        u8 pad0[0x10];
                        s16 x;
                        s16 y;
                        s16 z;
                        u8 pad16[0x86];
                    } sp;

                    pos = POS(arg4, arg1);
                    sp.x = pos->unk0;
                    sp.y = pos->unk4 + 50.0f;
                    sp.z = pos->unk8;
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
        } else if (EV->unk2 == 0x40) {
            func_151A0A10(arg5, 0x28, arg8, 1);
        } else if (EV->unk2 == 0x4D) {
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
        } else if (EV->unk2 == 0x4E) {
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
        } else if (EV->unk2 == 0x4F) {
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
        } else if (EV->unk2 == 0x24) {
            f32 p[3];
            s32 count;
            u8 type;

            p[0] = arg7[0];
            p[1] = arg7[1];
            p[2] = arg7[2];
            count = (EV->unk4 != 0) ? EV->unk4 : 0xA;
            type = EV->unk5;
            if (type >= 0xD) {
                type = 0;
            }
            func_151DBAA8(p, count, type, arg8, 0);
        } else if (EV->unk2 == 0x10) {
            u8 type;

            type = EV->unk4;
            if (type >= 0x1B) {
                type = 0;
            }
            func_151AF4D0(arg5, type, arg8, 0);
        } else if (EV->unk2 == 0x47) {
            func_151925C4(arg5, 0x32, arg8, 0);
        } else if (EV->unk2 == 0x2E) {
            func_150C1F60(arg8);
            func_150C2290(arg8);
            func_150C2424(arg8);
        } else if (EV->unk2 == 0x31) {
            func_150DAE70(arg5, 0, arg8, 0);
        } else if (EV->unk2 == 0x33) {
            u8 type;
            Ev3958 *pos;
            s32 i4, i5, i6;
            f32 f7, f9, f10;
            s16 n;

            if (EV->unk4 >= 0x10) {
                type = 0xF;
            } else {
                type = EV->unk4;
            }
            pos = POS(arg4, arg1);
            i4 = pos->unk20 * 0.05000000075f;
            i5 = (pos->unk10 + 180.0f) * 0.7111111283f;
            i6 = pos->unkC * 0.7111111283f;
            f7 = (pos->unk38 * 0.1000000015f) + 0.5f;
            f9 = (pos->unk18 * 0.1000000015f) + 1.0f;
            f10 = pos->unk1C * 0.1000000015f;
            n = (EV->unk5 != 0) ? EV->unk5 : 0x64;
            func_15148F1C(type, arg7[0], arg7[1], arg7[2], i4, i5, i6, f7, n, f9, f10, arg8);
        } else if (EV->unk2 == 0x35) {
            s32 type;

            type = (EV->unk4 >= 2) ? 1 : EV->unk4;
            func_150DB9E0(arg5, type, arg8);
        } else if (EV->unk2 == 0x26) {
            Elem15023DE0 *found;
            s32 t;

            if (arg5 == NULL) {
                return 1;
            }
            t = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4);
            if (EV->unk4 == 0) {
                arg5->unk276 = 1;
                arg5->unk282 = t - arg3;
            }
        } else if (EV->unk2 == 0x27) {
            f32 speed;
            f32 dist;
            s32 n;

            speed = 1.0f;
            dist = 30.0f;
            n = 1;
            if (EV->unk4 != 0) {
                n = EV->unk4;
            }
            if (EV->unk5 != 0) {
                speed = EV->unk5 * 0.1000000015f;
            }
            if (EV->unk6 >= 0) {
                dist = EV->unk6;
            }
            func_1518B264(arg7, dist, speed, n, arg8);
        } else if (EV->unk2 == 2) {
            f32 speed;
            f32 dist;
            s32 n;

            speed = 1.0f;
            dist = 30.0f;
            n = 1;
            if (EV->unk4 != 0) {
                n = EV->unk4;
            }
            if (EV->unk5 != 0) {
                speed = EV->unk5 * 0.1000000015f;
            }
            if (EV->unk6 >= 0) {
                dist = EV->unk6;
            }
            func_1518B2A8(arg7, dist, speed, n, 0x1E, 0x64, arg8);
        } else if (EV->unk2 == 0x28) {
            if (arg6 != NULL) {
                func_15171600(arg6, arg8, 0);
            } else if (arg5 != NULL) {
                func_15171BF4(arg5, arg8);
            }
        } else if (EV->unk2 == 0x29) {
            if (arg6 != NULL) {
                func_15170EC4(arg6, arg8, 0);
            } else if (arg5 != NULL) {
                func_151711C4(arg5);
            }
        } else if (EV->unk2 == 0x2A) {
        } else if (EV->unk2 == 0x2B) {
            func_1518B6B0(arg7[0], arg7[1], arg7[2], arg8, 0);
        } else if (EV->unk2 == 0x2C) {
            Elem15023DE0 *found;
            s32 pad[7];

            func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4);
            if (EV->unk4 == 0) {
            }
        } else if (EV->unk2 == 0x2F) {
            s32 delay;
            Elem15023DE0 *found;

            if (arg5 == NULL) {
                return 1;
            }
            if (EV->unk4 == 0) {
                delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
                func_1516ED68(arg5 - D_800CC2D0, delay, arg8, 0);
            }
        } else if (EV->unk2 == 0x39) {
            s32 delay;
            Elem15023DE0 *found;

            if (EV->unk4 == 0) {
                delay = func_15023BB0(EV, EV->unk0, arg1, &found, 1, EV->unk2, 0, 0, 0, 0, arg4) - arg3;
                func_150C2804(arg7, arg7 + 1, arg7 + 2, delay, arg8, 0);
            }
        } else if (EV->unk2 == 0x3A) {
            f32 p[3];
            s32 count;

            p[0] = arg7[0];
            p[1] = arg7[1];
            p[2] = arg7[2];
            count = (EV->unk4 != 0) ? EV->unk4 : 0xA;
            func_151DBAA8(p, count, 6, arg8, 0);
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_50D80/func_15029BB8.s")

void func_1502A8A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
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
        pos = &(&D_800C3958)[arg4][arg1].unk0;
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
