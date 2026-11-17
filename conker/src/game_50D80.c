#include <ultra64.h>
#define func_1514D3B0 func_1514D3B0_hdr
#include "functions.h"
#undef func_1514D3B0
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

#pragma GLOBAL_ASM("asm/nonmatchings/game_50D80/func_150242F8.s")

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
    s32 unk10;
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
extern s32 func_1518F51C(void *, u8, s32, s8, s8, s8, u8, s32, u8, s32);
extern u8 *func_1519003C(struct127 *, s32 *);
extern void func_15190518(void *);
extern void func_15191B8C(ObjRef1502460C *, u8);
extern void func_151937F4(struct127 *, s32, s32);
extern s32 func_1519D030(void *, s32, s32, s32, s32, s32);
extern s32 func_151A4FD0(s32, s32, s32, s32, s32, s32, u8, s32);
extern void func_151A9390(s32, s32, s32, f32 *, f32, f32, s32, s32, s32);
extern s32 func_151BE850(f32 *, s32, u8, u8, u8, u8, u8);
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

#pragma GLOBAL_ASM("asm/nonmatchings/game_50D80/func_150265CC.s")

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
