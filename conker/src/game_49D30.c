#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u16 *D_800C35C8[];
extern u16 *D_800C35D8[];
extern u8 *D_800C35F0[];
extern u8 D_800C363A[];
extern void func_1507E7E4(void *, s32, s32, s32, s32);
extern s8 D_8008FD84[];
extern u8 D_800C35E8;
extern s32 D_800C35B0[];
extern s32 D_800C3640[];
extern u8 D_800C3C99;
extern u8 D_800C3C9C;
extern u8 D_800D2E40;
extern s32 func_1501D2C4(s32, s32);
extern f32 D_800969D8;
extern f32 D_800969DC;
extern u8 D_800C35AA;
extern f32 D_80096A1C;
extern void func_1507F640(void);
extern void func_1508F060(void);
extern void func_150242F8(s32, s32);
extern void func_1501EC38(s32);
extern void func_15020EC4(s32);
extern void func_1502178C(struct127 *, s32, s32);
extern void func_1501E81C(s32, s32);
extern s32 func_150229E4(struct127 *);
extern s32 func_1502B6BC(s32 *, s32, s32 *, s32, s32, s32, s32);
extern u8 *D_800C3688[][30];
extern s32 D_800C3668[];
extern s32 func_15023BB0(void *, s32, s32, s16 **, s32, s32, s32, s32, s32, s32, s32);
extern void func_15022640(s32, s32);
extern struct131 *func_151149AC(u8);
extern struct127 *func_1505EEF4(s32);
extern void func_15082A44(void *, s32, s32, s32, s32);
extern u8 D_800D2100;

typedef struct {
    f32 unk0;
    f32 *unk4;
    u8 pad8[0x34];
    u8 unk3C;
    u8 pad3D[3];
} struct_game49D30_0;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u16 unk6;
} struct_game49D30_1;

typedef struct {
    s16 unk0;
    u8 pad2[0x32];
    s32 unk34;
} struct_game49D30_2;

extern struct_game49D30_0 *D_800C3A50[];
extern struct_game49D30_1 *D_800C3868[][30];
extern struct_game49D30_2 D_800C3D48;
extern s32 func_1501F72C(s32, f32, f32 *, s32, s32 *);
extern void func_1501FFE8(f32 *, struct_game49D30_1 *, s32, s32);
extern void func_1516D2E0(s32);
extern void func_1516D328(s32);

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
} SchedTask1501C880; /* 0x60 */

typedef struct {
    u8 unk0[0x20];
} SchedMsg1501C880;

extern u8 D_800BE9C0;
extern s32 D_800BE9C8[];
extern SchedTask1501C880 *D_800BEAD4;
extern SchedTask1501C880 D_800BEAD8[];
extern u8 D_800BEBA0;
extern u64 *D_800BEB9C;
extern u64 *D_800BEAD0;
extern u64 D_800BEBB0[];
extern u64 D_800C1060[];
extern u64 D_800CBE10[];
extern u64 D_8003A5E8[];
extern s32 D_100291A0;
extern void func_1501A39C(void);
extern void func_1501CC3C(void);

/* Append a parameterless GBI command (gDPNoParam) at the display-list write head D_800BE9D0. */
#define GFX_APPEND_NOPARAM(cmd)                 \
{                                               \
    Gfx *gfx = (Gfx *)D_800BE9D0;               \
    D_800BE9D0 += sizeof(Gfx);                  \
    gfx->words.w0 = _SHIFTL(cmd, 24, 8);        \
    gfx->words.w1 = 0;                          \
}

/* Finish this frame's display list and send it to the scheduler as an RSP graphics task. */
void func_1501C880(s32 arg0, s32 swapBuffers) {
    s32 capacity;
    s32 *dlBase;
    s32 used;

    dlBase = D_800BE9C8;
    capacity = (dlBase[1] - dlBase[0]) >> 3;
    D_800BEAD4 = &D_800BEAD8[D_800BE9C0];
    D_800BEAD4->next = 0;
    used = (D_800BE9D0 - dlBase[D_800BE9C0]) >> 3;
    if ((used < 0) || (used > capacity)) {
        func_1501A39C();
        D_800BE9D0 = D_800BE9D8[D_800BE9C0];
        GFX_APPEND_NOPARAM(G_RDPFULLSYNC);
        GFX_APPEND_NOPARAM(G_ENDDL);
        used = (D_800BE9D0 - dlBase[D_800BE9C0]) >> 3;
    }

    if ((used < 0) || (used > (capacity - 400))) {
        D_800BE9D0 = dlBase[D_800BE9C0] + ((capacity - 400) << 3);
        GFX_APPEND_NOPARAM(G_RDPFULLSYNC);
        GFX_APPEND_NOPARAM(G_ENDDL);
    }

    D_800BEAD4->list.data_ptr = (u64 *)dlBase[D_800BE9C0];
    D_800BEAD4->list.data_size = ((D_800BE9D0 - dlBase[D_800BE9C0]) >> 3) << 3;
    func_1501CC3C();

    D_800BEAD4->list.type = M_GFXTASK;
    D_800BEAD4->list.flags = OS_TASK_LOADABLE;
    D_800BEAD4->list.ucode_boot = (u64 *)&D_100290D0;
    D_800BEAD4->list.ucode_boot_size = (s32)&D_100291A0 - (s32)&D_100290D0;
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
    D_800BEAD4->msg = &((SchedMsg1501C880 *)&D_800BEA68)[D_800BE9C0];
    D_800BEAD4->framebuffer = D_8002AAE8[D_800BE9C0];
    osWritebackDCacheAll();
    osSendMesg(&D_8003B1E8, D_800BEAD4, 1);
    if (swapBuffers != 0) {
        D_800BE9C0 ^= 1;
    }
    // matching: the frame counter is loaded as s16 (lh); include/variables.h declares it u16
    *(s16 *)&D_800BEAA8 = *(s16 *)&D_800BEAA8 + 1;
}


typedef struct {
    s32 textStart;
    s32 textEnd;
    s32 dataStart;
    s32 dataEnd;
} struct_1501CC3C;

extern u8 D_2D4B0[];
extern s8 D_80084070;
extern s8 D_80084074;
extern struct_1501CC3C D_80084078[];
extern s32 D_800BEB98;

void func_1501CC3C(void) {
    s32 idx;
    s32 ofs;
    struct_1501CC3C *e;

    idx = D_800B0DF0->pad2A[1];
    if (idx >= 2) {
        idx -= 2;
        ofs = 0;
    } else {
        ofs = 4;
    }
    if (D_800BEB98 == 1) {
        idx += 2;
    }
    if ((idx != D_80084070) || (ofs != D_80084074)) {
        func_10004514(D_80084078[idx].textStart + (s32)D_2D4B0, D_800C1060, D_80084078[idx].textEnd - D_80084078[idx].textStart, 1);
        D_800BEB9C = (u64 *)((D_80084078[idx].textEnd - D_80084078[idx].textStart) + (u8 *)D_800C1060);
        func_10004514(D_80084078[idx].dataStart + (s32)D_2D4B0, D_800BEB9C, D_80084078[idx].dataEnd - D_80084078[idx].dataStart, 1);
        D_80084070 = idx;
        D_80084074 = ofs;
        e = D_80084078 + (idx + ofs);
        func_10004514(e->textStart + (s32)D_2D4B0, D_800BEBB0, e->textEnd - e->textStart, 1);
        D_800BEAD0 = (u64 *)((e->textEnd - e->textStart) + (u8 *)D_800BEBB0);
        func_10004514(e->dataStart + (s32)D_2D4B0, D_800BEAD0, e->dataEnd - e->dataStart, 1);
    }
}

extern s32 D_800C3960[];

void func_1501CDC0(s32 arg0) {
    s32 i;
    s32 j;

    for (i = 0; i < D_800C363A[arg0]; i++) {
        for (j = 0; j < 16; j++) {
            ((u8 *(*)[30])D_800C3960)[arg0][i][j] = 0xFF;
        }
    }
}

typedef struct {
    u8 pad0[0xA];
    s16 unkA;
    u8 padC[0x6];
    s16 unk12;
} struct_1501CE54_0;

typedef struct {
    u8 pad0[6];
    s16 unk6;
} struct_1501CE54_1;

extern s32 D_800C35D0;
extern struct_1501CE54_1 *D_800C3778[][30];

void func_1501CE54(s32 arg0) {
    s32 found;
    s32 i;
    s16 val;
    u16 idx;

    D_800C3640[arg0] = 0;
    if (D_800C363A[arg0] != 0) {
        if (*D_800C35D8[arg0] >= 4) {
            val = ((struct_1501CE54_0 *)D_800C3688[arg0][0])->unk12;
            if (val != -9999) {
                D_800C3640[arg0] = val;
                return;
            }
        }
        found = 0;
        for (i = 0; i < D_800C363A[arg0]; i++) {
            if (D_800C35C8[arg0][i] != 0 && D_800C35D8[arg0][i] != 0) {
                if (D_800C3640[arg0] < ((struct_1501CE54_0 *)D_800C3688[arg0][i])->unkA) {
                    D_800C3640[arg0] = ((struct_1501CE54_0 *)D_800C3688[arg0][i])->unkA;
                    found = 1;
                }
            }
        }
        if (found == 0) {
            for (i = 0; i < D_800C363A[arg0]; i++) {
                if (D_800C35C8[arg0][i] != 0) {
                    idx = ((u16 **)&D_800C35D0)[arg0][i];
                    if (idx) {
                        if (D_800C3640[arg0] < D_800C3778[arg0][i][idx - 1].unk6) {
                            D_800C3640[arg0] = D_800C3778[arg0][i][idx - 1].unk6;
                        }
                    }
                }
            }
        }
    }
}

s32 func_1501CFF8(s32 arg0) {
    s32 sum;
    s32 i;
    s32 count;
    s32 limit;
    u16 *ptr;

    sum = 0;
    i = 0;
    if ((count = D_800C363A[arg0]) > 0) {
        limit = count;
        ptr = D_800C35D8[arg0];
loop:
        sum += *ptr;
        i++;
        ptr++;
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
extern s32 D_800C3D50;
void func_150233BC(void);

void func_1501D044(s32 arg0) {
    f32 zero = 0.0f;
    f32 initial_value = 999.0f;

    D_800C35B8[arg0] = -0x64;
    D_800C35B0[arg0] = 0;
    D_800C35C0[arg0] = 0;
    D_800C3C8E = 0;
    D_800C363A[arg0] = 0;
    (&D_800C35EA)[arg0] = 0;
    D_800C3D50 = 0;
    D_800C35F8 = 0xFF;
    D_800C3648 = zero;
    D_800C364C = zero;
    D_800C3650 = zero;
    ((u8 *)&D_800C3654)[arg0] = 0;
    D_800C3656[arg0] = 0;
    D_800C3658[arg0] = 1;
    D_800C365A[arg0] = 0;
    D_800C365C[arg0] = 0;
    D_800C365E[arg0] = 0;
    D_800C3660[arg0] = 0;
    D_800C3638 = 0;
    D_800C3662 = 0;
    D_800C3C8C = 0;
    D_800C3C8D = 0;
    D_800C35C2 = 0;
    D_800C3A58 = 0;
    D_800C35A4 = zero;
    D_800C3C8A = -1;
    D_800C3C8B = 0;
    D_800C3674 = initial_value;
    D_800C3678 = initial_value;
    D_800C367C = 0;
    D_800C3680 = 0;
    D_800C3672 = 0;
    D_800C3C9D = 0x64;
    D_800C3C9E = -1;
    func_150233BC();
    D_8008FD84[0] = 0;
    D_800C3670 = 0;
}

s32 func_1501D1D4(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp34;
    s32 sp30;
    s32 temp_v0;

    temp_v0 = func_1502B6BC(&sp30, 0, &sp34, 3, 6, arg0, arg1);
    if (temp_v0 != 0) {
        D_800C3668[arg2] = temp_v0;
    } else {
        D_800C3668[arg2] = 0;
    }

    return temp_v0;
}

void func_1501D258(s32 arg0, s32 arg1) {
    if (!D_800C3670) {
        D_800C3A60[arg0] |= (1LL << arg1);
    }
}

s32 func_1501D2C4(s32 arg0, s32 arg1) {
    if (D_800C3670 != 0) {
        return 1;
    }
    if ((D_800C3A60[arg0] & (1ULL << arg1)) != 0) {
        return 1;
    }
    return 0;
}

typedef struct {
    u16 n0;
    u16 n1;
    u16 n2;
    u16 n3;
} Hdr1501D348;

typedef struct {
    s32 a;
    u32 b;
} Ent1501D348;

typedef struct {
    u8 pad0[4];
    void *unk4;
    u8 pad8[0x30];
    void *unk38;
    u8 pad3C[4];
} Anim1501D348;

extern void *allocate_memory(s32, s32, s32, s32);
extern void func_1501C730(s32, s32, s32, s32, s32);
extern void func_15020388(s32, s32);
extern void func_15022BA4(s32);
extern void func_150238D0(void);
extern void func_15022234(s32);
extern void func_15022248(s32);
extern void func_15022848(s32);
extern void func_15022754(s32);
extern void func_15022528(s32);
extern void func_1501DAAC(s32);
extern void (*D_800840FC[])(s32, s32, s32, s32);
extern u8 D_800C35A8;
extern u8 D_800C3C98;
extern u16 D_800C3C9A;
extern s32 D_800C35E0[];

void func_1501D348(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Hdr1501D348 *hdr;
    Ent1501D348 *data;
    s32 state;
    s32 i;
    s32 k;
    s32 size;

    state = (&D_800C35EA)[arg2];
    if (state != 1 && D_800C35C4 == 0) {
        D_800C3C90 = arg3;
        D_800C3C94 = arg4;
        if (arg0 != D_800BE9F0) {
            D_800C35C4 = arg1 + 1;
            func_1501C730(1, arg0, 0, 0, 0);
            if ((&D_800C35EA)[arg2] != 0) {
                D_800C35C5 = 0;
                return;
            }
            D_800C35C5 = 1;
            return;
        }
        (&D_800C35E8)[arg2] = arg1;
        if (state != 2) {
            D_800C35A8 = D_800BE9F0;
            D_800C3C98 = arg1;
        }
        func_1501D044(arg2);
        if (func_1501D1D4(arg0, arg1, arg2) != 0) {
            data = (Ent1501D348 *)D_800C3668[arg2];
            hdr = *(Hdr1501D348 **)data;
            D_800C35C8[arg2] = allocate_memory(hdr->n0 * 2, 1, 0, 0);
            k = 1;
            for (i = 0; i < hdr->n0; i++) {
                D_800C35C8[arg2][i] = data[k].b >> 3;
                k++;
            }
            size = hdr->n1;
            if (size < hdr->n0) {
                size = hdr->n0;
            }
            ((u16 **)&D_800C35D0)[arg2] = allocate_memory(size * 2, 1, 0, 0);
            bzero(((u16 **)&D_800C35D0)[arg2], size * 2);
            size = hdr->n2;
            if (size < hdr->n0) {
                size = hdr->n0;
            }
            D_800C35D8[arg2] = allocate_memory(size * 2, 1, 0, 0);
            bzero(D_800C35D8[arg2], size * 2);
            D_800C35E0[arg2] = (s32)allocate_memory(hdr->n3 * 4, 1, 0, 0);
            D_800C3A50[arg2] = allocate_memory(hdr->n0 << 6, 1, 0, 0);
            for (i = 0; i < hdr->n0; i++) {
                if (D_800C35C8[arg2][i] != 0) {
                    ((Anim1501D348 *)D_800C3A50[arg2])[i].unk4 = allocate_memory(D_800C35C8[arg2][i] * 4, 1, 0, 0);
                    ((Anim1501D348 *)D_800C3A50[arg2])[i].unk38 = allocate_memory(0x194, 1, 0, 0);
                }
            }
            ((void **)&D_800C3958)[arg2] = allocate_memory(hdr->n0 * 0x44, 1, 0, 0);
            for (i = 0; i < hdr->n0; i++) {
                ((void *(*)[30])D_800C3960)[arg2][i] = allocate_memory(0x10, 1, 0, 0);
            }
            for (i = 0; i < hdr->n0; i++) {
                D_800C35D8[arg2][i] = 0;
                ((u16 **)&D_800C35D0)[arg2][i] = 0;
                D_800C35C8[arg2][i] = 0;
            }
            D_800C363A[arg2] = hdr->n0;
            k = 1;
            for (i = 0; i < hdr->n0; i++) {
                D_800C35C8[arg2][i] = data[k].b >> 3;
                D_800C3868[arg2][i] = (struct_game49D30_1 *)data[k].a;
                k++;
            }
            D_800C35F0[arg2] = (u8 *)data[k].a;
            k += hdr->n0;
            for (i = 0; i < hdr->n1; i++) {
                ((u16 **)&D_800C35D0)[arg2][i] = data[k].b >> 3;
                D_800C3778[arg2][i] = (struct_1501CE54_1 *)data[k].a;
                k++;
            }
            for (i = 0; i < hdr->n2; i++) {
                D_800C35D8[arg2][i] = data[k].b >> 3;
                D_800C3688[arg2][i] = (u8 *)data[k].a;
                k++;
            }
            for (i = 0; i < hdr->n3; i++) {
                ((s32 **)D_800C35E0)[arg2][i] = data[k].a;
                k++;
            }
            if (arg3 != 0) {
                D_800840FC[arg3](arg0, arg1, arg2, arg4);
            }
            D_800C3C9A = func_1501CFF8(arg2);
            D_800C3D50 = (s32)allocate_memory(D_800C3C9A * 12, 1, 0, 0);
            D_800C35B0[arg2] = 0;
            func_1501CDC0(arg2);
            func_1501CE54(arg2);
            func_15022BA4(arg2);
            for (i = 0; i < D_800C363A[arg2]; i++) {
                func_15020388(i, arg2);
            }
            func_150238D0();
            (&D_800C35EA)[arg2] = 1;
            D_800C3638 = 1;
            func_15022234(arg2);
            func_15022248(arg2);
            if (D_800C363A[arg2] == 0) {
                func_1501E81C(0, arg2);
            }
            func_150242F8(1, arg2);
            func_1501EC38(arg2);
            func_15022848(arg2);
            func_15022754(arg2);
            func_15022528(arg2);
            func_15020EC4(arg2);
            D_800DBFF4[0] = 4;
            func_1501DAAC(arg2);
        }
    }
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s8 unk4;
    s8 unk5;
    s16 unk6;
} struct_game49D30_3;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} struct_game49D30_4;

extern u8 D_800C35A9;
extern u16 D_800C3C8E;
extern struct_game49D30_4 D_80084110[];
extern void func_1517EE40(s32, s32, s32, s32, s32, s32);

void func_1501DAAC(s32 arg0) {
    s32 i;
    s32 best;
    struct_game49D30_3 *found;
    s32 score;
    u8 *ptr;

    D_800C3C99 = 0;
    D_800C35A9 = 0;
    D_800C3C9C = 0;
    D_800C35AA = 0;
    D_800C3C8E = 0;

    best = -1;
    i = 0;
    if (D_800C363A[arg0] > 0) {
        do {
            ptr = D_800C3688[arg0][i];
            if (ptr != 0) {
                found = 0;
                func_15023BB0(ptr + 0x18, 5, i, (s16 **)&found, 1, 0x1E, 0, 0, 0, 0, arg0);
                if (found != 0) {
                    D_800C3C99 = 1;
                }

                found = 0;
                func_15023BB0(ptr + 0x18, 5, i, (s16 **)&found, 1, 0xE, 0, 0, 0, 0, arg0);
                if (found != 0) {
                    D_800C3C9C = 1;
                }

                found = 0;
                func_15023BB0(ptr + 0x18, 5, i, (s16 **)&found, 1, 0x68, 0, 0, 0, 0, arg0);
                if (found != 0) {
                    D_800C35AA = 1;
                }

                found = 0;
                func_15023BB0(ptr + 0x18, 5, i, (s16 **)&found, 1, 0x44, 0, 0, 0, 0, arg0);
                if (found != 0) {
                    D_800C35A9 = 1;
                }

                found = 0;
                score = func_15023BB0(ptr + 0x18, 5, i, (s16 **)&found, 1, 0x77, 0, 0, 0, 0, arg0);
                if (found != 0) {
                    D_800C3C8E = score;
                }

                found = 0;
                score = func_15023BB0(ptr + 0x18, 5, i, (s16 **)&found, 1, 1, 1, 0, 0, 0, arg0);
                if (found != 0) {
                    if ((best == -1) || (score < best)) {
                        best = score;
                        if (found->unk6 == 1) {
                            func_1517EE40(D_80084110[found->unk5].unk0, D_80084110[found->unk5].unk1, D_80084110[found->unk5].unk2, 0, 1, 0);
                        } else {
                            func_1517EE40(D_80084110[found->unk5].unk0, D_80084110[found->unk5].unk1, D_80084110[found->unk5].unk2, 0, 0, 0);
                        }
                    }
                }
            }
            i++;
        } while (i < D_800C363A[arg0]);
    }

    D_800C35B0[arg0] = D_800C3C8E;
}

void func_1501DE18(s32 arg0) {
    s32 i;
    s32 limit;
    struct127 *obj;

    D_800C35B0[arg0] = D_800C3640[arg0] + D_800BEA08;
    func_150242F8(1, 0);
    func_1501EC38(0);
    func_150242F8(0, 0);
    func_15020EC4(0);

    for (obj = D_800CC2D0, i = 0, limit = 25; i != limit; i++, obj = (struct127 *)((u8 *)obj + 0x32C)) {
        if (obj->interaction_state) {
            D_800C3E78 = i;
            if (func_150229E4(obj)) {
                func_1502178C(obj, arg0, -1);
            }
        }
    }

    func_1501E81C(1, arg0);
}

s32 func_100127D0(void);
void func_15060F28(struct127 *arg0, s32 arg1);
struct127 *func_15072208(struct127 *, s32);
extern u8 D_800C35C4;
extern u8 D_800C3663;
extern struct127 D_800CC2D0[26];
extern s32 D_800D121C;
extern u8 D_800DBFF4[];
extern u8 D_800C3C99;
extern u8 D_800C35A9;
extern s8 D_800C3C8A;
extern u8 D_800C3C8D;
extern u8 D_800C3672;
extern void func_1000E17C(void);
extern void func_1517EE40(s32, s32, s32, s32, s32, s32);
extern void func_151F2BA8(void);
void func_1501DE18(s32 arg0);
void func_1501DF04(s32 arg0)
{
  struct127 *obj;
  u8 temp_s0;
  s32 new_var;
  if (func_100127D0() != 0)
  {
    func_151F2BA8();
  }
  obj = func_15072208(D_800CC2D0, 0);
  if (obj != 0)
  {
    func_15060F28(obj, 0);
  }
  temp_s0 = D_800C3C99;
  func_1501DE18(arg0);
  if (temp_s0 != 0)
  {
    if (D_800C35C4 == 0)
    {
      do
      {
        temp_s0 = D_800C3C99;
        func_1501DE18(new_var = arg0);
      }
      while ((temp_s0 != 0) && (D_800C35C4 == 0));
    }
  }
 if (((D_800C35A9 == 0) && (D_800C3C8A != 1)) && (D_800C3C8D == 0)) { func_1517EE40(0, 0, 0, 0x1E, 0, 0); } { u8 *temp_v0 = (u8 *) D_800CC2D0; u8 *end = (u8 *) (&D_800D121C); do { if ((*((s32 *) temp_v0)) != 0) {
        temp_v0[0xA4] = 0;
      }
      temp_v0 += sizeof(struct127);
    }
    while (temp_v0 != end);
  }
  D_800C3663 = 0;
  func_1000E17C();
  D_800DBFF4[0] = 4;
  D_800C3672 = 1;
}

extern u8 D_800C3663;


s32 func_1501E05C(s32 arg0) {
    s32 temp_a2;
    s32 temp_v1;

    temp_a2 = D_800BE9F0;
    temp_v1 = (s32) D_8008FD84;

    if (temp_a2 == 0x1D) {
        if (*(s8 *)temp_v1 == 0) {
            goto check_mode_1d;
        }
        *(s8 *)temp_v1 = 0;
        return 1;
check_mode_1d:
        if (D_800C35E8 != 5) {
            return 0;
        }
    }

    if (temp_a2 == 0x21) {
        if ((D_800BE710[0] & 0x1000) && (D_8000030C == 1) && (D_800C35B0[0] >= 0x12D)) {
            return 1;
        }
        return 0;
    }

    if (D_800BE710[0] & 0x20) {
        if (D_800C3C9C == 0) {
            if (D_800D2E40 != 0) {
                return 1;
            }
            if (func_1501D2C4(temp_a2, (&D_800C35E8)[arg0]) != 0) {
                if ((D_800C3C99 != 0) || ((D_800C35B0[arg0] + 0x1E) < D_800C3640[arg0])) {
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

void func_1501E1B4(s32 arg0) {
    if (D_800C3C88 == 0) {
        if ((&D_800C35EA)[arg0] != 1) {
            return;
        }
        if (func_1501E05C(arg0) == 0) {
            return;
        }
        D_8008FD84[0] = 0;
        if (D_800C3640[arg0] - D_800C35B0[arg0] < 0x5B) {
            return;
        }
        D_800C3C88 = 1;
        D_800C3C89 = 0x1E;
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
        func_1501DF04(arg0);
        D_800C3C88 = 0;
    }
}

void func_1501E400(s32 arg0);

void func_1501E2F8(s32 arg0) {
    if (((&D_800C35EA)[arg0] == 1) && (D_800C35B0[arg0] >= D_800C3640[arg0])) {
        if (D_800C3C8C == 0) {
            func_1501E81C(0, arg0);
            if ((&D_800C35EA)[arg0] == 1) {
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

void func_1501E400(s32 arg0) {
    if (D_800C3A58 != 0) {
        D_800C3A58 -= 1;
    }
    D_800C35B8[arg0] = D_800C35B0[arg0];
    if ((&D_800C35EA)[arg0] == 1) {
        func_1501E1B4(arg0);
        if ((arg0 != 0) || (D_800BEAC0 == 0)) {
            D_800C35B0[arg0] += D_800BEA08;
        }
        if ((D_800C35C0[arg0] == 0) &&
            (D_800C3C8E == D_800C35B8[arg0]) &&
            (D_800C3C8E != D_800C35B0[arg0])) {
            D_800C35B8[arg0] = -1;
        }
        if (func_151F2CDC() == 1) {
            func_15080228();
        }
        if (D_800C35B0[arg0] >= D_800C3640[arg0]) {
            D_800C35B0[arg0] = D_800C3640[arg0];
        }
    }
}

void func_10004074(s32);
extern s32 D_800C35E0[];
extern s32 D_800C35D0;
extern s32 D_800C3D50;
extern s32 D_800C3960[];

void func_1501E540(s32 arg0) {
    s32 index;
    u16 *count;

    if (D_800C3668[arg0] != 0) {
        count = *(u16 **)D_800C3668[arg0];
        index = 0;
        if ((s32)*count > 0) {
            do {
                if (((u16 *)D_800C35C8[arg0])[index] != 0) {
                    func_10004074(*(s32 *)((u8 *)D_800C3A50[arg0] + (index << 6) + 4));
                    func_10004074(*(s32 *)((u8 *)D_800C3A50[arg0] + (index << 6) + 0x38));
                }
                index++;
            } while (index < (s32)*count);
            index = 0;
        }
        func_10004074((s32)D_800C3A50[arg0]);
        D_800C3A50[arg0] = 0;
        func_10004074((s32)((u8 **)&D_800C3958)[arg0]);
        ((u8 **)&D_800C3958)[arg0] = 0;
        if ((s32)*count > 0) {
            s32 *cursor;

            cursor = (s32 *)((u8 *)D_800C3960 + arg0 * 0x78);
            do {
                func_10004074(*cursor);
                *cursor = 0;
                index++;
                cursor++;
            } while (index < (s32)*count);
        }
        func_10004074((s32)D_800C35C8[arg0]);
        func_10004074(*(s32 *)((u8 *)&D_800C35D0 + arg0 * 4));
        func_10004074((s32)D_800C35D8[arg0]);
        func_10004074(D_800C35E0[arg0]);
        D_800C35C8[arg0] = 0;
        *(s32 *)((u8 *)&D_800C35D0 + arg0 * 4) = 0;
        D_800C35D8[arg0] = 0;
        D_800C35E0[arg0] = 0;
        func_10004074(D_800C3D50);
        D_800C3D50 = 0;
        func_10004074(D_800C3668[arg0]);
        D_800C3668[arg0] = 0;
    }
}

typedef struct {
    u8 pad0[0x6A];
    u8 unk6A;
    u8 unk6B;
    u8 unk6C;
    u8 unk6D;
} Obj1501E73C;

void func_1507EABC(void *);

void func_1501E73C(s32 arg0) {
    s32 i;
    Obj1501E73C *obj;

    for (i = 0; i < D_800C363A[arg0]; i++) {
        if (*(u16 *)&D_800C35F0[arg0][i * 8] == 2) {
            obj = (Obj1501E73C *)func_15083E90(D_800C35F0[arg0][i * 8 + 2]);
            if (obj == NULL) {
                continue;
            }
            if (obj->unk6C >= 10) {
                obj->unk6C = 0;
                obj->unk6A = 0;
            }
            if (obj->unk6D >= 10) {
                obj->unk6D = 0;
                obj->unk6B = 0;
            }
            func_1507EABC(obj);
        }
    }
}

extern void func_15169040(u8 *, u8);
extern u8 D_800C35C3;
void func_1501EA18(s32 arg0);
void func_150233E4(void);
s32 func_150235DC(s32 arg0);

void func_1501E81C(s32 arg0, s32 arg1) {
    struct108 *sp44;

    sp44 = D_800DBFF0;
    if ((&D_800C35EA)[arg1] != 0) {
        {
            struct { u8 unk0; } sp40;
            s32 var_v0;

            sp40.unk0 = (&D_800C35E8)[arg1];
            var_v0 = arg0 ? 0xF : 0x10;
            func_15169040((u8 *)&sp40, var_v0);
        }
        D_800C35B8[arg1] = -100;
        D_800C35B0[arg1] = D_800C3C8E;
        func_1501D258(D_800BE9F0, (&D_800C35E8)[arg1]);
        (&D_800C35EA)[arg1] = 0;
        D_800C3638 = 0;
        func_1501E73C(arg1);
        func_150233E4();
        D_800C3662 = 0;
        D_800C3683 = 0;
        func_150235DC(arg1);
        func_1501EA18(arg1);
        func_1501E540(arg1);
        D_800C3648 = 0.0f;
        D_800C364C = 0.0f;
        D_800C3650 = 0.0f;
        func_1510B32C(0, sp44->unk19C, sp44->unk1A0, 1.0f);
        if (D_800C35C2 != 0) {
            s32 sp38;
            s32 var_a0;

            (&D_800C35EA)[arg1] = 2;
            var_a0 = D_800C35C3;
            if (0x25 == D_800C35C3) {
            sp38 = D_800BE9F0;
            D_800BE9F0 = 0x25;
            var_a0 = 0x25;
            }
            D_800C3671 = 0; func_1501D348(var_a0, D_800C35C2 - 1, arg1, D_800C3C90, D_800C3C94);
            if (0x25 == D_800C35C3) {
                D_800BE9F0 = sp38;
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

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    f32 unkC;
    s32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
} Msg1501EA18;
typedef struct {
    s16 x, y, z;
} Vec3s1501EA18;
extern void func_1512D560(void *, s32, void *);
void func_150228E4(s32 arg0);
void func_150227BC(s32 arg0);
extern s16 D_800C358C[];
extern s16 D_800C3598[];
extern f32 D_800C3594;
extern f32 D_800C35A0;

#define MSG (*(Msg1501EA18 *)&D_800C3600)
void func_1501EA18(s32 arg0) {
    struct108 *player;
    struct127 *obj;
    s32 i;

    player = D_800DBFF0;
    MSG.unk0 = 1;
    MSG.unk4 = arg0;
    MSG.unk14 = player->unk2F8;
    MSG.unk18 = player->unk2FC;
    MSG.unk1C = player->unk300;
    MSG.unk20 = 0.0f;
    MSG.unk24 = player->unk388;
    MSG.unk28 = player->unk37C;
    if (D_800C3671 == 0) {
        func_1512D560(player, 7, &MSG);
        if (D_800C3674 != 999.0f) {
            MSG.unkC = D_800C3674;
            func_1512D560(player, 8, &MSG);
        }
        if (D_800C3678 != 999.0f) {
            MSG.unkC = D_800C3678;
            MSG.unk10 = D_800C367C;
            func_1512D560(player, 10, &MSG);
        }
    }
    if (D_800C35C2 == 0 && D_800C3671 == 0) {
        if (D_800C3680 == 0) {
            func_1512D560(player, 6, (void *)arg0);
        } else {
            D_800C3680 = 0;
        }
    }
    if (D_800C365A[arg0] == 0) {
        func_150228E4(arg0);
    }
    func_150227BC(arg0);
    for (i = 0; i != 25; i++) {
        if (D_800CC2D0[i].interaction_state != 0 && func_150229E4(&D_800CC2D0[i]) != 0) {
            *(u8 *)&D_800CC2D0[i].padA4 = 0;
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
} Out1501EC38;

typedef struct {
    f32 value;
    u8 interp;
    u8 channel;
    s16 time;
} Key1501EC38;
extern f32 func_150497E0(f32 *, s32, f32);
void func_1501FC8C(s32 arg0, f32 arg1, f32 *arg2, f32 *arg3, f32 *arg4, s32 arg5);
void func_1501FE68(s32 arg0, f32 arg1, f32 *arg2, s32 arg3);

void func_1501EC38(s32 arg0) {
    s32 lo[15];
    s32 hi[15];
    s32 idx[15];
    s32 flag[15];
    f32 first[15];
    f32 last[15];
    s32 cnt[15];
    f32 r[15];
    s32 j;
    s32 k;
    s32 i;
    f32 t;
    s32 mask;
    s32 found;
    s32 n;
    f32 pts[4];
    s32 start;
    s32 remaining;
    s32 m;
    s32 ch;
    s32 q;
    s32 next;
    Key1501EC38 *key;

    if ((&D_800C35EA)[arg0] == 1) {
        D_800C3638 = 0;
        for (i = 0; i < D_800C363A[arg0]; i++) {
            if (D_800C35C8[arg0][i] != 0) {
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk0 = D_800C3868[arg0][i]->unk0;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk4 = D_800C3868[arg0][i]->unk2;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk8 = D_800C3868[arg0][i]->unk4;
            } else {
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk0 = 0.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk4 = 0.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk8 = 0.0f;
            }
            if (*(u16 *)&D_800C35F0[arg0][i * 8] == 4) {
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unkC = 64.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk10 = 0.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk14 = 20.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk18 = 255.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk1C = 62.5f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk20 = 50.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk24 = 0.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk28 = 0.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk2C = 0.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk34 = 0.0f;
            } else {
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unkC = 0.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk10 = D_800C35A4;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk14 = 0.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk18 = -9999.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk1C = -9999.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk20 = -9999.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk24 = 0.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk28 = 0.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk2C = 0.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk3C = -9999.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk40 = -9999.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk30 = -9999.0f;
            }
            ((Out1501EC38 **)&D_800C3958)[arg0][i].unk38 = 0.0f;
            if (*(u16 *)&D_800C35F0[arg0][i * 8] == 1) {
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk34 = -9999.0f;
            } else if (*(u16 *)&D_800C35F0[arg0][i * 8] == 2) {
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk34 = 255.0f;
            } else if (*(u16 *)&D_800C35F0[arg0][i * 8] == 5) {
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk18 = 32.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk1C = 32.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk34 = 1.0f;
            } else if (*(u16 *)&D_800C35F0[arg0][i * 8] == 3) {
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk18 = -9999.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk1C = -9999.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk20 = -9999.0f;
                ((Out1501EC38 **)&D_800C3958)[arg0][i].unk34 = 255.0f;
            }
            if (((Key1501EC38 *(*)[30])D_800C3778)[arg0][i] != NULL) {
                for (j = 0; j < 15; j++) {
                    hi[j] = -9999;
                    lo[j] = -9999;
                    idx[j] = -9999;
                    flag[j] = 1;
                    cnt[j] = 0;
                    first[j] = -9999.0f;
                    last[j] = -9999.0f;
                }
                for (j = 0; j < ((u16 **)&D_800C35D0)[arg0][i]; j++) {
                    key = &((Key1501EC38 *(*)[30])D_800C3778)[arg0][i][j];
                    ch = key->channel;
                    if (idx[ch] == -9999) {
                        if (key->time >= D_800C35B0[arg0]) {
                            idx[ch] = cnt[ch];
                            hi[ch] = key->time;
                        } else {
                            flag[ch] = key->interp;
                            lo[ch] = key->time;
                        }
                    }
                    last[ch] = key->value;
                    if (first[ch] == -9999.0f) {
                        first[ch] = key->value;
                    }
                    cnt[ch]++;
                }
                for (j = 0; j < 15; j++) {
                    r[j] = -9999.0f;
                    if (lo[j] == -9999 && hi[j] == -9999) {
                        continue;
                    }
                    if (lo[j] == -9999) {
                        r[j] = first[j];
                    } else if (hi[j] == -9999) {
                        r[j] = last[j];
                    } else {
                        t = (f32)(D_800C35B0[arg0] - lo[j]) / (f32)(hi[j] - lo[j]);
                        mask = 6;
                        found = -1;
                        if (flag[j] == 0) {
                            s32 skip;
                            if (idx[j] - 2 >= 0) {
                                mask = 7;
                                start = idx[j] - 2;
                                remaining = 3;
                                q = 0;
                            } else {
                                start = idx[j] - 1;
                                remaining = 2;
                                q = 1;
                            }
                            if (idx[j] + 1 < cnt[j]) {
                                mask |= 8;
                                remaining++;
                            }
                            skip = 0;
                            for (m = 0; m < ((u16 **)&D_800C35D0)[arg0][i]; m++) {
                                if (remaining == 0) {
                                    break;
                                }
                                key = &((Key1501EC38 *(*)[30])D_800C3778)[arg0][i][m];
                                if (j == key->channel) {
                                    if (skip == start) {
                                        pts[q++] = key->value;
                                        remaining--;
                                    } else {
                                        skip++;
                                    }
                                }
                            }
                            if (!(mask & 1)) {
                                pts[0] = pts[1];
                            }
                            if (!(mask & 8)) {
                                pts[3] = pts[2];
                            }
                            r[j] = func_150497E0(pts, 0, t);
                        } else {
                            n = 0;
                            k = 0;
                            for (; k < ((u16 **)&D_800C35D0)[arg0][i]; k++) {
                                key = &((Key1501EC38 *(*)[30])D_800C3778)[arg0][i][k];
                                if (j == key->channel) {
                                    if (found == -1) {
                                        if (n == idx[j] - 1) {
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
                            r[j] = ((Key1501EC38 *(*)[30])D_800C3778)[arg0][i][next].value - ((Key1501EC38 *(*)[30])D_800C3778)[arg0][i][found].value;
                            r[j] *= t;
                            r[j] += ((Key1501EC38 *(*)[30])D_800C3778)[arg0][i][found].value;
                        }
                    }
                }
                if (r[10] == -9999.0f) {
                    r[10] = 0.0f;
                }
                if (*((u8 *(*)[30])D_800C3960)[arg0][i] != 0xFF) {
                    func_1501FE68(i, r[10], &((Out1501EC38 **)&D_800C3958)[arg0][i].unk10, arg0);
                } else if (r[7] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk10 = r[7];
                }
                if (r[8] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unkC = r[8];
                }
                if (r[6] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk14 = r[6];
                }
                if (r[5] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk18 = r[5];
                }
                if (r[4] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk1C = r[4];
                }
                if (r[3] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk20 = r[3];
                }
                if (r[1] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk24 = r[1];
                }
                if (r[2] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk28 = r[2];
                }
                if (r[0] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk2C = r[0];
                }
                if (r[14] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk30 = r[14];
                }
                if (r[9] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk34 = r[9];
                }
                if (r[11] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk38 = r[11];
                }
                if (r[13] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk3C = r[13];
                }
                if (r[12] != -9999.0f) {
                    ((Out1501EC38 **)&D_800C3958)[arg0][i].unk40 = r[12];
                }
                func_1501FC8C(i, r[10], &((Out1501EC38 **)&D_800C3958)[arg0][i].unk0,
                              &((Out1501EC38 **)&D_800C3958)[arg0][i].unk4,
                              &((Out1501EC38 **)&D_800C3958)[arg0][i].unk8, arg0);
            }
        }
        D_800C3638 = 1;
    }
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_1501F72C;
void func_15020878(s32, s32, s32);

s32 func_1501F72C(s32 arg0, f32 arg1, f32 *arg2, s32 arg3, s32 *arg4) {
    s32 key;
    s32 lo;
    s32 hi;
    s32 sample;
    Vec3f_1501F72C points[4];
    f32 *times;
    s32 i;
    u8 j;
    s32 interp;
    f32 t;

    lo = 0;
    hi = *(u16 *)((u8 *)D_800C35C8[arg3] + arg0 * 2) - 1;
    if (hi >= 0) {
        times = *(f32 **)((u8 *)D_800C3A50[arg3] + (arg0 << 6) + 4);
        do {
            key = (lo + hi) / 2;
            if (arg1 < times[key]) {
                hi = key - 1;
            } else {
                lo = key + 1;
            }
            if (times[key] <= arg1 && arg1 <= times[key + 1]) {
                break;
            }
        } while (hi >= lo);
    }
    *arg4 = key;
    if (key != *((u8 *)D_800C3A50[arg3] + (arg0 << 6) + 0x3C)) {
        func_15020878(arg0, key, arg3);
    }
    interp = *(u16 *)(*(u8 **)((u8 *)D_800C3868 + arg3 * 0x78 + arg0 * 4) + key * 8 + 6);
    func_1501FFE8((f32 *)points, *(void **)((u8 *)D_800C3868 + arg3 * 0x78 + arg0 * 4), key,
                  *(u16 *)((u8 *)D_800C35C8[arg3] + arg0 * 2));
    if (interp == 0) {
        hi = 100;
        lo = 0;
        times = *(f32 **)((u8 *)D_800C3A50[arg3] + (arg0 << 6) + 0x38);
        do {
            sample = (lo + hi) / 2;
            if (arg1 < times[sample]) {
                hi = sample - 1;
            } else {
                lo = sample + 1;
            }
            if (times[sample] <= arg1 && arg1 <= times[sample + 1]) {
                break;
            }
        } while (hi >= lo);
        j = 0;
        for (i = 0; i < 2; i++) {
            t = (f32)(sample + i) * 0.01f;
            ((Vec3f_1501F72C *)arg2)[j].x = ((((f32 *)((u8 *)D_800C3A50[arg3] + (arg0 << 6)))[11] * t + ((f32 *)((u8 *)D_800C3A50[arg3] + (arg0 << 6)))[8]) * t + ((f32 *)((u8 *)D_800C3A50[arg3] + (arg0 << 6)))[5]) * t + ((f32 *)((u8 *)D_800C3A50[arg3] + (arg0 << 6)))[2];
            ((Vec3f_1501F72C *)arg2)[j].y = ((((f32 *)((u8 *)D_800C3A50[arg3] + (arg0 << 6)))[12] * t + ((f32 *)((u8 *)D_800C3A50[arg3] + (arg0 << 6)))[9]) * t + ((f32 *)((u8 *)D_800C3A50[arg3] + (arg0 << 6)))[6]) * t + ((f32 *)((u8 *)D_800C3A50[arg3] + (arg0 << 6)))[3];
            ((Vec3f_1501F72C *)arg2)[j].z = ((((f32 *)((u8 *)D_800C3A50[arg3] + (arg0 << 6)))[13] * t + ((f32 *)((u8 *)D_800C3A50[arg3] + (arg0 << 6)))[10]) * t + ((f32 *)((u8 *)D_800C3A50[arg3] + (arg0 << 6)))[7]) * t + ((f32 *)((u8 *)D_800C3A50[arg3] + (arg0 << 6)))[4];
            j ^= 1;
        }
    } else {
        t = (*(f32 **)((u8 *)D_800C3A50[arg3] + (arg0 << 6) + 4))[key + 1] - (*(f32 **)((u8 *)D_800C3A50[arg3] + (arg0 << 6) + 4))[key];
        arg1 = arg1 - (*(f32 **)((u8 *)D_800C3A50[arg3] + (arg0 << 6) + 4))[key];
        if (t != 0) {
            arg1 = arg1 / t;
        } else {
            arg1 = 0.0f;
        }
        j = 0;
        for (i = 0; i < 2; i++) {
            ((Vec3f_1501F72C *)arg2)[j].x = (points[2].x - points[1].x) * arg1 + points[1].x;
            ((Vec3f_1501F72C *)arg2)[j].y = (points[2].y - points[1].y) * arg1 + points[1].y;
            ((Vec3f_1501F72C *)arg2)[j].z = (points[2].z - points[1].z) * arg1 + points[1].z;
            j ^= 1;
        }
    }
    return sample;
}

extern f32 D_800969D4;

void func_1501FC8C(s32 arg0, f32 arg1, f32 *arg2, f32 *arg3, f32 *arg4, s32 arg5) {
    f32 time;
    s32 key_index;
    s32 sample_index;
    f32 positions[6];
    f32 *times;
    f32 numerator;
    f32 ratio;

    if (*(u16 *)((u8 *)D_800C35C8[arg5] + (u32)arg0 * 2U) >= 2) {
        if (arg1 > 100.0f) {
            arg1 = 100.0f;
        }
        if (arg1 < 0.0f) {
            arg1 = 0.0f;
        }
        time = *(f32 *)((u8 *)D_800C3A50[arg5] + ((u32)arg0 << 6)) * arg1 * D_800969D4;
        sample_index = func_1501F72C(arg0, time, positions, arg5, &key_index);
        if (*(u16 *)(*(u8 **)((u8 *)D_800C3868 + (u32)arg5 * 0x78U + (u32)arg0 * 4U) + (u32)key_index * 8U + 6) == 0) {
            times = *(f32 **)((u8 *)D_800C3A50[arg5] + ((u32)arg0 << 6) + 0x38) + sample_index;
            arg1 = times[1] - times[0];
            numerator = time - times[0];
            if (arg1 != 0) {
                ratio = numerator / arg1;
            } else {
                ratio = 0.0f;
            }
            *arg2 = positions[0] + (positions[3] - positions[0]) * ratio;
            *arg3 = positions[1] + (positions[4] - positions[1]) * ratio;
            *arg4 = positions[2] + (positions[5] - positions[2]) * ratio;
            return;
        }
        *arg2 = positions[0];
        *arg3 = positions[1];
        *arg4 = positions[2];
    }
}

void func_1501FE68(s32 arg0, f32 arg1, f32 *arg2, s32 arg3) {
    f32 temp_f12;
    f32 sp6C[6];
    s32 sp60[3];
    f32 sp30[10];
    f32 temp_f14;
    register f32 temp_f0;

    if (D_800C35C8[arg3][arg0] >= 2) {
        if (arg1 > 100.0f) {
            arg1 = 100.0f;
        }
        if (arg1 < 0.0f) {
            arg1 = 0.0f;
        }

        temp_f0 = D_800C3A50[arg3][arg0].unk0 * arg1 * D_800969D8;
        func_1501F72C(arg0, temp_f0, sp6C, arg3, sp60);

        if (D_800C3868[arg3][arg0][sp60[0]].unk6 == 0) {
            temp_f12 = sp6C[0] - sp6C[3];
            temp_f14 = sp6C[2] - sp6C[5];
        } else {
            func_1501FFE8(sp30 - 2, D_800C3868[arg3][arg0], sp60[0], D_800C35C8[arg3][arg0]);
            temp_f12 = sp30[4] - sp30[1];
            temp_f14 = sp30[6] - sp30[3];
        }
        *arg2 = func_150484A0(temp_f12, temp_f14) * D_800969DC;
    }
}

void func_1501FFE8(f32 *arg0, struct_game49D30_1 *arg1, s32 arg2, s32 arg3) {
    s32 flags;
    s32 start;
    s32 count;
    s32 outIdx;
    s32 i;
    s32 outOffset;

    flags = 6;
    if ((arg2 - 1) >= 0) {
        flags = 7;
        start = arg2 - 1;
        count = 3;
        outIdx = 0;
    } else {
        start = arg2;
        count = 2;
        outIdx = 1;
    }

    if ((arg2 + 2) < arg3) {
        flags |= 8;
        count++;
    }

    for (i = start; i < start + count; outIdx++, i++) {
        outOffset = outIdx * 12;
        *(f32 *)((u8 *)arg0 + outOffset + 0) = (f32) arg1[i].unk0;
        *(f32 *)((u8 *)arg0 + outOffset + 4) = (f32) arg1[i].unk2;
        *(f32 *)((u8 *)arg0 + outOffset + 8) = (f32) arg1[i].unk4;
    }

    if (!(flags & 1)) {
        arg0[0] = arg0[3];
        arg0[1] = arg0[4];
        arg0[2] = arg0[5];
    }
    if (!(flags & 8)) {
        arg0[9] = arg0[6];
        arg0[10] = arg0[7];
        arg0[11] = arg0[8];
    }
}

void func_15020388(s32 arg0, s32 arg1) {
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
    u16 flag;

    if (D_800C35C8[arg1][arg0] >= 2) {
        D_800C3A50[arg1][arg0].unk3C = 0xFF;
        len = 0.0f;
        cur = 0;
        for (i = 0; i < D_800C35C8[arg1][arg0] - 1; i++) {
            flag = D_800C3868[arg1][arg0][i].unk6;
            func_1501FFE8(&pts[0][0], D_800C3868[arg1][arg0], i, D_800C35C8[arg1][arg0]);
            D_800C3A50[arg1][arg0].unk4[i] = len;
            if (flag == 0) {
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
        D_800C3A50[arg1][arg0].unk4[i] = len;
        D_800C3A50[arg1][arg0].unk0 = len;
        D_800C3A50[arg1][arg0].unk3C = 0xFF;
    }
}

typedef struct {
    f32 unk0;
    f32 *unk4;
    f32 unk8[3];
    f32 unk14[3];
    f32 unk20[3];
    f32 unk2C[3];
    f32 *unk38;
    u8 unk3C;
} Spline15020878;

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern f32 D_800969EC;

void func_15020878(s32 arg0, s32 arg1, s32 arg2) {
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
    s32 interp;

    if (arg1 >= D_800C35C8[arg2][arg0] - 1) {
        return;
    }
    {
        j = 0;
        total = ((Spline15020878 *)D_800C3A50[arg2])[arg0].unk4[arg1];
        interp = D_800C3868[arg2][arg0][arg1].unk6;
        func_1501FFE8(points, D_800C3868[arg2][arg0], arg1, D_800C35C8[arg2][arg0]);
        if (interp == 0) {
            for (i = 0; i < 3; i++) {
                a[i] = -0.5f * points[i] + 1.5f * points[i + 3] + -1.5f * points[i + 6] + points[i + 9] * 0.5f;
                b[i] = points[i] + -2.5f * points[i + 3] + (points[i + 6] + points[i + 6]) + points[i + 9] * -0.5f;
                c[i] = -0.5f * points[i] + points[i + 6] * 0.5f;
                d[i] = points[i + 3];
                ((Spline15020878 *)D_800C3A50[arg2])[arg0].unk8[i] = d[i];
                ((Spline15020878 *)D_800C3A50[arg2])[arg0].unk14[i] = c[i];
                ((Spline15020878 *)D_800C3A50[arg2])[arg0].unk20[i] = b[i];
                ((Spline15020878 *)D_800C3A50[arg2])[arg0].unk2C[i] = a[i];
            }
            ((Spline15020878 *)D_800C3A50[arg2])[arg0].unk38[0] = total;
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
                ((Spline15020878 *)D_800C3A50[arg2])[arg0].unk38[i] = total;
                i++;
                j ^= 1;
            }
        } else {
            total = ((Spline15020878 *)D_800C3A50[arg2])[arg0].unk4[arg1 + 1] - total;
            for (k = 0; k < 101; k++) {
                ((Spline15020878 *)D_800C3A50[arg2])[arg0].unk38[k] = (f32)k * total / 101.0f;
            }
        }
        ((Spline15020878 *)D_800C3A50[arg2])[arg0].unk3C = arg1;
    }
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
    u8 pad24[0x10];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
} Elem15020EC4; // size 0x44

typedef struct {
    u16 type;
    u8 kind;
    u8 idx;
    s16 yoff;
    s16 pad6;
} Ent15020EC4; // size 0x8

typedef struct {
    s32 unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
    s32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
} Msg15020EC4;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[4];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x16];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    u8 pad38[0x16];
    u8 unk4E;
    u8 pad4F[0x20];
    u8 unk6F;
    u8 pad70[0x1A];
    u8 unk8A;
} Obj15020EC4;

typedef struct { f32 x, y, z; } V3f15020EC4;
V3f15020EC4 D_800C3620;
extern void func_15128CB0(struct108 *);
extern void func_1517D5FC(s16, s16, s16, s32, s32, s32);
s32 func_15023264(s32 arg0, s32 arg1);

#define MSG2 (*(Msg15020EC4 *)&D_800C3600)
#define ENT(a, i) ((Ent15020EC4 *)(D_800C35F0[a] + (i) * 8))
#define ELEM(a, i) (&((Elem15020EC4 **)&D_800C3958)[a][i])

void func_15020EC4(s32 arg0) {
    s32 i;
    s32 j;
    struct108 *player;
    Elem15020EC4 *e;
    Obj15020EC4 *o;
    f32 f22;
    f32 f20;

    if ((&D_800C35EA)[arg0] == 1) {
        D_800C3638 = 0;
        if (D_800C35F8 == 0xFF) {
            D_800C35F8 = func_15023264(arg0, D_800C35B0[arg0]);
        }
        for (j = 0; j != 2; j++) {
            for (i = 0; i < D_800C363A[arg0]; i++) {
                if (((u16 **)&D_800C35D0)[arg0][i] == 0) {
                    continue;
                }
                if (D_800C35C8[arg0][i] == 0) {
                    continue;
                }
                if (j == 0) {
                    if (ENT(arg0, i)->type == 1) {
                        continue;
                    }
                } else if (ENT(arg0, i)->type != 1) {
                    continue;
                }
                if (ENT(arg0, i)->type == 1) {
                    player = D_800DBFF0;
                    if (i != D_800C35F8) {
                        continue;
                    }
                    if (!(player->unk2C & 0x40000)) {
                        if (D_800C3671 == 0) {
                            func_1512D560(player, 5, (void *)arg0);
                        }
                        func_15128CB0(player);
                    }
                    e = ELEM(arg0, i);
                    if (e->unk34 == -9999.0f) {
                        MSG2.unk8 = 0.0f;
                    } else {
                        e->unk34 = (e->unk34 < -20.0f) ? -20.0f : (e->unk34 > 40.0f) ? 40.0f : e->unk34;
                        e = ELEM(arg0, i);
                        MSG2.unk8 = e->unk34;
                    }
                    if (e->unk3C == -9999.0f) {
                        f22 = 0.0f;
                    } else {
                        e->unk3C = (e->unk3C < -20.0f) ? -20.0f : (e->unk3C > 40.0f) ? 40.0f : e->unk3C;
                        e = ELEM(arg0, i);
                        f22 = e->unk3C;
                    }
                    if (e->unk40 == -9999.0f) {
                        f20 = 0.0f;
                        while (0) { }
                        MSG2.unk4 = arg0;
                    } else {
                        e->unk40 = (e->unk40 < -20.0f) ? -20.0f : (e->unk40 > 40.0f) ? 40.0f : e->unk40;
                        if (ENT(arg0, i)) { }
                        e = ELEM(arg0, i);
                        f20 = e->unk40;
                        MSG2.unk4 = arg0;
                    }
                    MSG2.unk14 = e->unk0;
                    MSG2.unk18 = e->unk4;
                    MSG2.unk1C = e->unk8;
                                        if (ENT(arg0, i)->kind == 1) {
                        MSG2.unk0 = 0;
                        MSG2.unk20 = ELEM(arg0, (s8)ENT(arg0, i)->idx)->unk0;
                        MSG2.unk24 = ELEM(arg0, (s8)ENT(arg0, i)->idx)->unk4 + ENT(arg0, i)->yoff + (e = ELEM(arg0, i))->unk38;
                        MSG2.unk28 = ELEM(arg0, (s8)ENT(arg0, i)->idx)->unk8;
                        MSG2.unk2C = -e->unk14;
                        MSG2.unk30 = f22;
                        MSG2.unk34 = f20;
                    } else if (ENT(arg0, i)->kind == 2) {
{
    struct127 *ob;

                        ob = func_15083E90(ENT(arg0, i)->idx);
                        if (ob == NULL) {
                            continue;
                        }
                        MSG2.unk0 = 0;
                        D_800C3620.x = ob->x_position;
                        D_800C3620.y = ob->y_position + ENT(arg0, i)->yoff + (e = ELEM(arg0, i))->unk38;
                        D_800C3620.z = ob->z_position;
                        MSG2.unk2C = -e->unk14;
                        MSG2.unk30 = f22;
                        MSG2.unk34 = f20;
}
                    } else if (ENT(arg0, i)->kind == 3) {
                        Obj15020EC4 *ob3;

                        ob3 = (Obj15020EC4 *)func_151149AC(ENT(arg0, i)->idx);
                        if (ob3 == NULL) {
                            continue;
                        }
                        MSG2.unk0 = 0;
                        D_800C3620.x = ob3->unk10;
                        D_800C3620.y = (ob3->unk12 + ENT(arg0, i)->yoff) + (e = ELEM(arg0, i))->unk38;
                        D_800C3620.z = ob3->unk14;
                        MSG2.unk2C = -e->unk14;
                        MSG2.unk30 = f22;
                        MSG2.unk34 = f20;
                    } else if (ENT(arg0, i)->kind == 0) {
                        MSG2.unk0 = 1;
                        MSG2.unk20 = -e->unk14;
                        MSG2.unk24 = e->unkC;
                        MSG2.unk28 = e->unk10;
                        MSG2.unk18 = MSG2.unk18 + e->unk38;
                        MSG2.unk2C = f22;
                        MSG2.unk30 = f20;
                    }
                    if (D_800C3671 == 0) {
                        func_1512D560(player, 7, &MSG2);
                    }
                } else if (ENT(arg0, i)->type == 3) {
                    o = (Obj15020EC4 *)func_151149AC(ENT(arg0, i)->kind);
                    if (o == NULL) {
                        continue;
                    }
                    o->unk4E = 0;
                    o->unk6F &= ~0x80;
                    o->unk6F = o->unk6F;
                    o->unk10 = ELEM(arg0, i)->unk0;
                    o->unk12 = ELEM(arg0, i)->unk4 + ELEM(arg0, i)->unk38;
                    o->unk14 = ELEM(arg0, i)->unk8;
                    o->unk0 = ELEM(arg0, i)->unkC;
                    o->unk4 = ELEM(arg0, i)->unk10;
                    o->unk8 = ELEM(arg0, i)->unk14;
                    if (ELEM(arg0, i)->unk18 != -9999.0f) {
                        o->unk2C = ELEM(arg0, i)->unk18 * 0.01f;
                    }
                    if (ELEM(arg0, i)->unk1C != -9999.0f) {
                        o->unk30 = ELEM(arg0, i)->unk1C * 0.01f;
                    }
                    if (ELEM(arg0, i)->unk20 != -9999.0f) {
                        o->unk34 = ELEM(arg0, i)->unk20 * 0.01f;
                    }
                    if ((s32)ELEM(arg0, i)->unk34 >= 0x100) {
                        o->unk8A = 0xFF;
                    } else if ((s32)ELEM(arg0, i)->unk34 < 0) {
                        o->unk8A = 0;
                    } else {
                        o->unk8A = (s32)ELEM(arg0, i)->unk34;
                    }
                } else if (ENT(arg0, i)->type == 2) {
                    struct127 *ob2;

                    ob2 = func_15083E90(ENT(arg0, i)->kind);
                    func_1502178C(ob2, arg0, i);
                } else if (ENT(arg0, i)->type == 4) {
                    s32 arr[3];

                    arr[0] = ENT(arg0, i)->kind;
                    arr[1] = ENT(arg0, i)->kind;
                    arr[2] = ENT(arg0, i)->kind;
                } else if (ENT(arg0, i)->type == 5) {
                    e = ELEM(arg0, i);
                    if (e->unk34 >= 1.0f) {
                        func_1517D5FC(e->unk0, e->unk4, e->unk8, 0, e->unk18, e->unk1C);
                    }
                }
            }
        }
        D_800C3638 = 1;
    }
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
    u8 pad20[0x14];
    f32 unk34;
    f32 unk38;
    u8 pad3C[0x8];
} Ent1502178C;

typedef struct {
    u16 kind;
    u8 actor;
    u8 pad3[5];
} Sel1502178C;

extern void func_1510E7A4(void *, void *, f32 *, void *, void *, void *, f32, f32, f32, f32, u16, struct127 *, f32, f32);

extern f32 D_800D35E0; /* ((u8 *)&D_800D35E0)[-4] == D_800D35DC; a distinct base keeps IDO from hoisting the address */

void func_1502178C(struct127 *arg0, s32 arg1, s32 arg2) {
    s32 i;
    f32 floorY;
    struct127 *obj;
    s32 end;
    Sel1502178C *sel;
    f32 angle;
    f32 old[3];
    s32 start;
    s32 changed;

    if ((&D_800C35EA)[arg1] == 1) {
        D_800C3638 = 0;
        if (arg2 != -1) {
            start = arg2;
            end = arg2 + 1;
        } else {
            start = 0;
            end = D_800C363A[arg1];
        }
        for (i = start; i < end; i++) {
            if (D_800C35C8[arg1][i] == 0) continue; {
                sel = &((Sel1502178C *)D_800C35F0[arg1])[i];
                if (sel->kind == 2) {
                    obj = func_15083E90(sel->actor);
                    if (obj != NULL && obj == arg0) {
                        old[0] = obj->x_position;
                        old[1] = obj->y_position;
                        old[2] = obj->z_position;
                        obj->x_position = ((Ent1502178C **)&D_800C3958)[arg1][i].unk0;
                        obj->y_position = ((Ent1502178C **)&D_800C3958)[arg1][i].unk4 + ((Ent1502178C **)&D_800C3958)[arg1][i].unk38;
                        obj->z_position = ((Ent1502178C **)&D_800C3958)[arg1][i].unk8;
                        obj->unk1CC = obj->y_position;
                        if (D_800C3C88 == 0) {
                            obj->xz_velocity = (obj->x_position - old[0]) * (obj->x_position - old[0]) + (obj->z_position - old[2]) * (obj->z_position - old[2]);
                            if (obj->xz_velocity) {
                                obj->xz_velocity = sqrtf(obj->xz_velocity) * 2.0f;
                            }
                        } else {
                            obj->xz_velocity = 0.0f;
                        }
                        if (obj->unk31C != NULL) {
                            *(f32 *)obj->unk31C = obj->xz_velocity * 2.0f;
                        }
                        *(f32 *)&obj->pad54 = 0.0f;
                        obj->unkCC = 0;
                        changed = 0;
                        *(u8 *)&D_800D35DC = (arg0 - D_800CC2D0) + 1;
                        func_1510E7A4(&obj->pad188, &obj->unk18C, &floorY, &obj->unk118, &obj->unk184, (u8 *)obj + 0x1A2,
                                      obj->x_position, obj->y_position, obj->z_position, obj->y_position, 0, obj,
                                      -10000.0f, arg0->y_position);
                        ((u8 *)&D_800D35E0)[-4] = 0;
                        obj->unk180 = floorY;
                        obj->unkD0 = 0;
                        obj->unk28 = obj->y_position - obj->unk180;
                        if (obj->interaction_state == 1) {
                            if (obj->unk118 != -10000.0f && obj->y_position < obj->unk118) {
                                obj->in_water = 1;
                            } else {
                                obj->in_water = 0;
                                obj->unkB2 = 0;
                            }
                        }
                        if (((Ent1502178C **)&D_800C3958)[arg1][i].unk18 != -9999.0f) {
                            changed = 1;
                            obj->xz_scale = ((Ent1502178C **)&D_800C3958)[arg1][i].unk18 * 0.009999999776f;
                            obj->unk154 = ((Ent1502178C **)&D_800C3958)[arg1][i].unk18 * 0.009999999776f;
                        }
                        if (((Ent1502178C **)&D_800C3958)[arg1][i].unk1C != -9999.0f) {
                            changed = 1;
                            obj->y_scale = ((Ent1502178C **)&D_800C3958)[arg1][i].unk1C * 0.009999999776f;
                            obj->unk158 = ((Ent1502178C **)&D_800C3958)[arg1][i].unk1C * 0.009999999776f;
                        }
                        if (changed) {
                            func_15062BDC(obj, obj->xz_scale, obj->y_scale);
                        }
                        obj->unkB8 = ((Ent1502178C **)&D_800C3958)[arg1][i].unkC;
                        angle = ((Ent1502178C **)&D_800C3958)[arg1][i].unk10 + 180.0f;
                        while (angle >= 360.0f) {
                            angle -= 360.0f;
                        }
                        while (angle < 0.0f) {
                            angle += 360.0f;
                        }
                        obj->unk76 = obj->unk7A = obj->unk78 = (s16)((u32)(angle * 182.0444489f) - 0x4000);
                        obj->unk40 = angle;
                        obj->unkC4 = ((Ent1502178C **)&D_800C3958)[arg1][i].unk14;
                        ((Ent1502178C **)&D_800C3958)[arg1][i].unk34 = (((Ent1502178C **)&D_800C3958)[arg1][i].unk34 < 0.0f) ? 0.0f : (((Ent1502178C **)&D_800C3958)[arg1][i].unk34 > 255.0f) ? 255.0f : ((Ent1502178C **)&D_800C3958)[arg1][i].unk34;
                        func_1506160C(obj, 1, ((Ent1502178C **)&D_800C3958)[arg1][i].unk34, 0, 0);
                        obj->old_x_position = obj->x_position;
                        obj->old_y_position = obj->y_position;
                        obj->old_z_position = obj->z_position;
                    }
                }
            }
        }
        D_800C3638 = 1;
    }
}


typedef struct {
    u16 kind;
    u8 actor, pad3[2];
    s8 index;
    s16 height;
} Game49D30Selection;

s32 func_15021DB8(f32 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, s32 arg4, s32 arg5) {
    s32 index;
    u8 *choices;
    s32 matrixOffset;
    u8 *camera;
    f32 sentinel;

    if ((&D_800C35EA)[arg5] != 1) {
        return 0;
    }
    for (index = 0; index < D_800C363A[arg5]; index++) {
        if (((u16 *)D_800C35C8[arg5])[index] == 0) {
            continue;
        }
        if (((Game49D30Selection *)D_800C35F0[arg5])[index].kind != 2 ||
            ((u8 (*)[0x32C])&D_800CC2D0)[arg4][0x3B] != ((Game49D30Selection *)D_800C35F0[arg5])[index].actor) {
            continue;
        }
        choices = ((u8 *(*)[30])D_800C3960)[arg5][index];
        if (choices[2] != 0xFF) {
            Game49D30Selection *selection;
            s32 slot;

            selection = (Game49D30Selection *)(((u8 *(*)[30])D_800C3688)[arg5][index] + choices[2] * 8);
            slot = selection->index;
            matrixOffset = slot * 0x44;
            arg3[0] = *(f32 *)(((u8 **)&D_800C3958)[arg5] + matrixOffset);
            arg3[1] = *(f32 *)(((u8 **)&D_800C3958)[arg5] + matrixOffset + 4) + (f32)selection->height;
            arg3[2] = *(f32 *)(((u8 **)&D_800C3958)[arg5] + matrixOffset + 8);
            return 0;
        }
        if (choices[1] != 0xFF) {
            Game49D30Selection *selection;

            selection = (Game49D30Selection *)(((u8 *(*)[30])D_800C3688)[arg5][index] + choices[1] * 8);
            if (selection->index == 0) {
                matrixOffset = index * 0x44;
                *arg0 = *(f32 *)(((u8 **)&D_800C3958)[arg5] + matrixOffset + 0x28);
                *arg1 = *(f32 *)(((u8 **)&D_800C3958)[arg5] + matrixOffset + 0x24);
                *arg2 = *(f32 *)(((u8 **)&D_800C3958)[arg5] + matrixOffset + 0x2C);
                return 1;
            }
            camera = (u8 *)D_800DBFF0;
            arg3[0] = *(f32 *)(camera + 0x2F8);
            arg3[1] = *(f32 *)(camera + 0x2FC) + (f32)selection->height;
            arg3[2] = *(f32 *)(camera + 0x300);
            return 0;
        }
        sentinel = -99999.0f;
        arg3[0] = sentinel;
        arg3[1] = sentinel;
        arg3[2] = sentinel;
        return 0;
    }
    return 0;
}

typedef struct {
    u16 unk0;
    u8 unk2;
    u8 pad3[5];
} Elem15022024;

typedef struct {
    u8 pad0[0x3C];
    f32 unk3C;
    f32 unk40;
} Ent15022024;


s32 func_15022024(f32 *arg0, f32 *arg1, s32 arg2, s32 arg3) {
    s32 i;

    if ((&D_800C35EA)[arg3] != 1) {
        return 0;
    }
    for (i = 0; i < D_800C363A[arg3]; i++) {
        if (D_800C35C8[arg3][i] != 0) {
            if (((Elem15022024 *)D_800C35F0[arg3])[i].unk0 == 2 && D_800CC2D0[arg2].unique_id == ((Elem15022024 *)D_800C35F0[arg3])[i].unk2) {
                arg0[1] = 0.0f;
                arg1[1] = 0.0f;
                arg0[0] = 0.0f;
                arg1[0] = 0.0f;
                if (((Ent15022024 **)&D_800C3958)[arg3][i].unk3C != -9999.0f) {
                    arg1[0] = ((Ent15022024 **)&D_800C3958)[arg3][i].unk3C;
                    arg1[1] = arg1[0];
                }
                if (((Ent15022024 **)&D_800C3958)[arg3][i].unk40 != -9999.0f) {
                    arg0[0] = ((Ent15022024 **)&D_800C3958)[arg3][i].unk40;
                    arg0[1] = arg0[0];
                }
                return 1;
            }
        }
    }
    return 0;
}

extern s16 D_800C358C[];
extern f32 D_800C3594;
extern u8 D_800C3663;

void func_15022190(s16 arg0, s16 arg1, s16 arg2, f32 arg3) {
    D_800C3594 = arg3;
    D_800C358C[0] = arg0;
    D_800C358C[1] = arg1;
    D_800C358C[2] = arg2;
    D_800C3663 = 1;
}

extern s16 D_800C3598[];
extern f32 D_800C35A0;
extern u8 D_800C3510[];
extern u8 D_800C3518[][25];
extern u8 D_800C354A[];
extern u8 D_800C3550[][30];

void func_150221E8(s16 arg0, s16 arg1, s16 arg2, f32 arg3) {
    D_800C35A0 = arg3;
    D_800C3598[0] = arg0;
    D_800C3598[1] = arg1;
    D_800C3598[2] = arg2;
}

void func_15022234(s32 arg0) {
    D_800C3510[arg0] = 0;
}

void func_15022248(s32 arg0) {
    D_800C354A[arg0] = 0;
}

void func_1502225C(s32 arg0, s32 arg1) {
    s32 i;
    s32 count;
    s32 limit;
    u8 *count_ptr;

    i = 0;
    count_ptr = &D_800C3510[arg1];
    count = *count_ptr;
    limit = count;
    if (count > 0) {
        do {
            if (arg0 == D_800C3518[arg1][i]) {
                return;
            }
            i++;
        } while (i < limit);
    }

    D_800C3518[arg1][count] = arg0;
    *count_ptr = count + 1;
}

void func_150222E0(s32 arg0, s32 arg1) {
    struct127 *temp_v0;
    u8 *temp_v1;

    if (D_800C35C8[arg1][arg0] != 0) {
        temp_v1 = &D_800C35F0[arg1][arg0 * 8];
        if (*(u16 *)temp_v1 == 2) {
            temp_v0 = func_15083E90(temp_v1[2]);
            if (temp_v0 != 0) {
                *(u8 *)((u8 *)temp_v0 + 0x6C) = 0;
                *(u8 *)((u8 *)temp_v0 + 0x6D) = 0;
                *(u8 *)((u8 *)temp_v0 + 0x6A) = 0;
                *(u8 *)((u8 *)temp_v0 + 0x6B) = 0;
                *(u16 *)((u8 *)temp_v0 + 0x282) = 0;
                *(u16 *)((u8 *)temp_v0 + 0x27A) = 0;
                *(u16 *)((u8 *)temp_v0 + 0x27C) = 0;
                *(u16 *)((u8 *)temp_v0 + 0x27E) = 0;
                *(u16 *)((u8 *)temp_v0 + 0x280) = 0;
                func_1507E7E4(temp_v0, 0, 3, 0xFFFF, 0xA);
                *(u8 *)((u8 *)temp_v0 + 0x71) = 0;
            }
        }
    }
}

void func_15022398(s32 arg0, s32 arg1) {
    struct127 *temp_v0;
    s32 temp_a1;
    s32 temp_id;

    if (D_800C35C8[arg1][arg0] != 0) {
        if (*(u16 *)&D_800C35F0[arg1][arg0 * 8] == 2) {
            temp_id = D_800C35F0[arg1][(arg0 * 8) + 2];
            temp_a1 = func_15083E0C(temp_id);
            if (temp_a1 != -1) {
                temp_v0 = func_1505EEF4(temp_a1);
                if (temp_v0 != 0) {
                    if (temp_v0->unk5 == 3) {
                        if (D_800C35F0 != &D_800C35F0[arg1]) {
                            temp_v0->unk5 = 4;
                        } else {
                            temp_v0->unk5 = 0;
                        }
                    }
                    if (temp_id == 1) {
                        temp_v0->unkF4 &= -0x2B;
                    }
                } else {
                    if (temp_a1 < D_800D2100) {
                        func_15082A44((struct_game49D30_2 *)((temp_a1 * 0x30) + (s32)D_800D20FC), temp_a1, 0, arg1, 0);
                        temp_v0 = func_1505EEF4(temp_a1);
                        if (temp_v0 != 0) {
                            func_1505E650(temp_v0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 1);
                            func_1502225C(((s32)temp_v0 - (s32)D_800CC2D0) / (s32)sizeof(struct127), arg1);
                        }
                    }
                }
            }
        }
    }
}

void func_15022528(s32 arg0) {
    s32 i;
    struct127 *obj;

    for (i = 0; i < D_800C363A[arg0]; i++) {
        if (((u16 **)&D_800C35D0)[arg0][i] == 0) {
            continue;
        }
        if (D_800C35C8[arg0][i] == 0) {
            continue;
        }
        if (*(u16 *)&D_800C35F0[arg0][i * 8] == 2) {
            obj = func_15083E90(D_800C35F0[arg0][i * 8 + 2]);
            if (obj != NULL) {
                func_1502178C(obj, arg0, i);
            }
        }
    }
}

void func_15022640(s32 arg0, s32 arg1) {
    s32 i;
    s32 count;
    s32 limit;
    u8 *count_ptr;

    i = 0;
    count_ptr = &D_800C354A[arg1];
    count = *count_ptr;
    limit = count;
    if (count > 0) {
        do {
            if (arg0 == D_800C3550[arg1][i]) {
                return;
            }
            i++;
        } while (i < limit);
    }

    D_800C3550[arg1][count] = arg0;
    *count_ptr = count + 1;
}

void func_150226BC(s32 arg0, s32 arg1) {
    struct131 *temp_v0;
    u8 *temp_v1;

    if (D_800C35C8[arg1][arg0] != 0) {
        temp_v1 = &D_800C35F0[arg1][arg0 * 8];
        if (*(u16 *)temp_v1 == 3) {
            temp_v0 = func_151149AC(temp_v1[2]);
            if (temp_v0 != 0) {
                if (*(u8 *)((u8 *)temp_v0 + 0x6E) == 1) {
                    func_15022640(*(u8 *)((u8 *)temp_v0 + 0x72), arg1);
                    *(u8 *)((u8 *)temp_v0 + 0x6E) = 0;
                }
            }
        }
    }
}

extern u8 D_800C363A[];
extern void func_150226BC(s32, s32);

void func_15022754(s32 arg0) {
    s32 i;

    for (i = 0; i < D_800C363A[arg0]; i++) {
        func_150226BC(i, arg0);
    }
}

void func_150227BC(s32 arg0) {
    s32 i;
    u8 *count;

    count = &D_800C354A[arg0];
    for (i = 0; i < *count; i++) {
        *(s8 *)((u8 *)func_151149AC(D_800C3550[arg0][i]) + 0x6E) = 1;
    }
}

extern void func_15022398(s32, s32);
extern void func_150222E0(s32, s32);

void func_15022848(s32 arg0) {
    s32 i;

    if ((&D_800C35EA)[arg0] == 1) {
        D_800C3638 = 0;
        for (i = 0; i < D_800C363A[arg0]; i++) {
            func_15022398(i, arg0);
            func_150222E0(i, arg0);
        }
        D_800C3638 = 1;
    }
}

void func_150228E4(s32 arg0) {
    s32 i;
    s32 count;
    u8 *actor;
    u8 *countp;

    countp = &D_800C3510[arg0];
    count = *countp;
    for (i = 0; i < count; i++) {
        actor = ((u8 (*)[0x32C])D_800CC2D0)[*(arg0 * 0x19 + (u8 *)D_800C3518 + i)];
        if (*(s32 *)actor != 0) {
            func_15060F28((struct127 *)actor, 0);
            count = *countp;
        }
    }
}

void func_15022998(s32 *arg0) {
    if ((*arg0 == 0x1B) || (*arg0 == 4)) {
        if (D_80086014[*arg0] != 0) {
            D_80086014[*arg0](arg0);
        }
    }
}

s32 func_150229E4(struct127 *arg0)
{
  s32 i;
  s32 idx;
  s32 slot;
  s32 id;
  idx = 0;
  idx = 0;
  if (arg0 == 0)
  {
    return 0;
  }
  if (arg0->interaction_state == 0)
  {
    return 0;
  }
  if (D_800C35C8[idx] == 0)
  {
    return 0;
  }
  if (arg0->unk5 == 4)
  {
    idx = 1;
  }
  slot = arg0->unk65;
  if (slot != 0)
  {
    id = D_800CC2D0[slot - 1].unique_id;
  }
  else
  {
    id = arg0->unique_id;
  }
  for (i = 0; i < D_800C363A[idx]; i++)
  {
    if (D_800C35C8[idx][i] != 0)
    {
      if (((*((u16 *) (&D_800C35F0[idx][i * 8]))) == 2) && (id == D_800C35F0[idx][(i * 8) + 2]))
      {
        if (slot == 0)
        {
          return 1;
        }
        return 2;
      }
    }
  }

  return 0;
}

s32 func_15022B08(s32 arg0, s32 arg1) {
    s32 i;

    for (i = 0; i < D_800C363A[arg1]; i++) {
        if (D_800C35C8[arg1][i] != 0) {
            if (D_800C35F0[arg1][(i * 8) + 2] == ((u8 *)*(&D_800DBEF4) + (arg0 * 0xA0))[0x72]) {
                return 1;
            }
        }
    }

    return 0;
}

typedef struct {
    f32 unk0;
    u8 pad4;
    u8 unk5;
    u8 pad6[2];
} struct_15022BA4_0;

void func_15022BA4(s32 arg0) {
    s32 i;
    s32 j;
    s32 k;
    s32 flag;
    f32 y;
    f32 x;
    f32 z;
    f32 dx;
    f32 dy;
    f32 dz;
    struct_game49D30_1 *p;

    for (i = 0; i < D_800C363A[arg0]; i++) {
        if (D_800C35C8[arg0][i] == 0) {
            continue;
        }
        if (*(u16 *)&D_800C35F0[arg0][i * 8] == 6) {
            if (D_800C3663 == 0) {
                p = D_800C3868[arg0][i];
                func_15022190(p->unk0, p->unk2, p->unk4, 0.0f);
            }
            if (D_800C35F0[arg0][i * 8 + 2] == 0) {
                struct127 *obj;
                obj = func_15083E90(D_800C35F0[arg0][i * 8 + 3]);
                if (obj == NULL) {
                    return;
                }
                x = obj->x_position;
                y = obj->y_position;
                z = obj->z_position;
            } else if (D_800C35F0[arg0][i * 8 + 2] == 1) {
                struct131 *obj;
                obj = (struct131 *)(s8)D_800C35F0[arg0][i * 8 + 3];
                obj = func_151149AC((s32)obj);
                if (obj == NULL) {
                    return;
                }
                x = obj->unk10;
                y = obj->unk12;
                z = obj->unk14;
            } else {
                return;
            }
            p = D_800C3868[arg0][i];
            dx = x - p->unk0;
            dy = y - p->unk2;
            dz = z - p->unk4;
            for (j = 0; j < D_800C363A[arg0]; j++) {
                for (k = 0; k < D_800C35C8[arg0][j]; k++) {
                    D_800C3868[arg0][j][k].unk0 = D_800C3868[arg0][j][k].unk0 + dx;
                    D_800C3868[arg0][j][k].unk2 = D_800C3868[arg0][j][k].unk2 + dy;
                    D_800C3868[arg0][j][k].unk4 = D_800C3868[arg0][j][k].unk4 + dz;
                }
            }
            return;
        } else if (*(u16 *)&D_800C35F0[arg0][i * 8] == 7) {
            f32 c;
            f32 s;
            f32 t;
            f32 ang;
            f32 rx;
            f32 rz;
            f32 nx;
            f32 f;
            struct_15022BA4_0 *e;

            f = 0.0f;
            for (j = 0; j < ((u16 **)&D_800C35D0)[arg0][i]; j++) {
                e = &((struct_15022BA4_0 *)D_800C3778[arg0][i])[j];
                if (e->unk5 == 7) {
                    f = e->unk0;
                    break;
                }
            }
            if (D_800C3663 == 0) {
                p = D_800C3868[arg0][i];
                func_15022190(p->unk0, p->unk2, p->unk4, f);
            }
            t = D_800C3594 - f;
            D_800C35A4 = t;
            p = D_800C3868[arg0][i];
            ang = t * 0.01745329238f;
            x = p->unk0;
            y = p->unk2;
            z = p->unk4;
            c = cosf(ang);
            s = sinf(ang);
            dy = D_800C358C[1] - y;
            for (j = 0; j < D_800C363A[arg0]; j++) {
                for (k = 0, flag = 0; k < D_800C35C8[arg0][j]; k++) {
                    rx = D_800C3868[arg0][j][k].unk0 - x;
                    rz = D_800C3868[arg0][j][k].unk4 - z;
                    nx = rx * c + rz * s;
                    rz = -rx * s + rz * c;
                    D_800C3868[arg0][j][k].unk0 = D_800C358C[0] + nx;
                    D_800C3868[arg0][j][k].unk2 = D_800C3868[arg0][j][k].unk2 + dy;
                    D_800C3868[arg0][j][k].unk4 = D_800C358C[2] + rz;
                }
                if (D_800C35D8[arg0][j] != 0 && ((s8 *)D_800C3688[arg0][j])[0x20] == 7) {
                    flag = 1;
                }
                if (flag == 0) {
                    for (k = 0; k < ((u16 **)&D_800C35D0)[arg0][j]; k++) {
                        e = &((struct_15022BA4_0 *)D_800C3778[arg0][j])[k];
                        if (e->unk5 == 7) {
                            e->unk0 = e->unk0 + t;
                        }
                    }
                }
            }
            return;
        }
    }
}

s32 func_15023264(s32 arg0, s32 arg1) {
    s32 i;
    s32 best;
    s32 result;
    s32 score;
    s16 *found;
    u8 **entry;
    u8 *ptr;
    u8 *records;

    result = -1;
    best = 0x1869F;

    i = 0;
    if (D_800C363A[arg0] > 0) {
        entry = D_800C3688[arg0];
        do {
            ptr = *entry;
            if (ptr != 0) {
                found = 0;
                score = func_15023BB0(ptr + 0x18, 3, i, &found, 0, 0, 0, 0, 0, 0, arg0);
                if (found != 0) {
                    if (score < best) {
                        best = score;
                        result = found[1];
                    }
                }
            }
            i++;
            entry++;
        } while (i < D_800C363A[arg0]);
    }

    if (result == -1) {
        result = 0;
        i = 0;
        if (i < D_800C363A[arg0]) {
            records = D_800C35F0[arg0];
            do {
                if (*(u16 *)records == 1) {
                    result = i;
                    break;
                }
                i++;
                records += 8;
            } while (i < D_800C363A[arg0]);
        }
    }

    return result;
}

void func_150233BC(void) {
    bzero(D_800C3CA0, 0xA8);
}

void func_150233E4(void) {
    s32 i;
    int new_var;
    struct_game49D30_2 *entry;

    i = 0;
    do {
        entry = &((struct_game49D30_2 *) D_800C3CA0)[i];
        if (entry->unk0 != 0) {
            func_1516D2E0(entry->unk34);
            entry->unk34 = 0;
            new_var = 0;
            entry->unk0 = new_var;
        }
        i++;
    } while (&D_800C3D48 != &((struct_game49D30_2 *) D_800C3CA0)[i]);
}

void func_15023440(struct_game49D30_2 *arg0, s32 arg1) {
    struct_game49D30_2 *entry;

    entry = arg0;
    if (arg1 != 0) {
        func_1516D2E0(entry->unk34);
        entry->unk34 = 0;
    } else if (*(u8 *)((u8 *)entry + 0xC) != 0) {
        func_1516D328(entry->unk34);
    } else {
        entry->unk34 = 0;
    }
    entry->unk0 = 0;
}

typedef struct Game49D30Record {
    s16 active;
    u8 pad2[0xA];
    u8 flagC;
    u8 padD[0x27];
    s32 resource34;
} Game49D30Record;

Game49D30Record *func_150234A4(s32 arg0, s32 arg1) {
    s32 i;
    Game49D30Record *found;
    Game49D30Record *slot;

    arg0++;
    found = NULL;
    slot = NULL;
    for (i = 0; i < 3; i++) {
        if ((((Game49D30Record *)&D_800C3CA0) + i)->active == arg0) {
            if (arg1 != 0 && (((Game49D30Record *)&D_800C3CA0) + i)->flagC != 0) {
                slot = (Game49D30Record *)&D_800C3CA0;
                func_15023440((struct_game49D30_2 *)slot, 1);
            } else {
                found = ((Game49D30Record *)&D_800C3CA0) + i;
            }
            break;
        }
        if ((((Game49D30Record *)&D_800C3CA0) + i)->active == 0) {
            slot = ((Game49D30Record *)&D_800C3CA0) + i;
            if (arg1 != 0 && slot->flagC != 0) {
                func_15023440((struct_game49D30_2 *)slot, 1);
            }
            break;
        }
    }
    if (i == 3) {
        slot = (Game49D30Record *)&D_800C3CA0;
        func_15023440((struct_game49D30_2 *)slot, 1);
    }
    if (found != NULL) {
        return found;
    }
    if (slot != NULL) {
        slot->active = arg0;
    }
    return slot;
}

s32 func_150235DC(s32 arg0) {
    struct127 *temp_v0;
    struct127 *temp_v1;
    void *temp_a0;
    s8 temp_v0_2;

    D_800CC2D0[0].unk25C &= ~0x200;
    if (D_800C35AA == 0) {
        temp_v0 = func_15083E90(1);
        if (temp_v0 != 0) {
            temp_v1 = temp_v0;
            D_800C3E78 = temp_v0 - D_800CC2D0;
            temp_a0 = temp_v0->unk31C;
            temp_v0->xz_velocity = 0.0f;
            if (temp_a0 != 0) {
                *(f32 *)temp_a0 = 0.0f;
            }
            *(f32 *)((u8 *)temp_v0 + 0x54) = 0.0f;
            temp_v0->unkCC = 0;
            temp_v0->unkD0 = 0;

            if ((D_80096A1C != temp_v0->unk118) && (temp_v0->y_position < temp_v0->unk118)) {
                temp_v0->in_water = 1;
                temp_v0->y_velocity = 0.0f;
                temp_v0->gravity = 0.0f;
                if (((temp_v0->unk118 - 50.0f) < temp_v0->unk1A6) || ((((u8 *)temp_v0->unk31C)[0x20] & 1) != 0)) {
                    temp_v1->unkB2 = 0;
                    func_1508F060();
                }
            } else {
                temp_v0->in_water = 0;
                temp_v0->unkB2 = 0;
                temp_v0->y_velocity = -4.0f;
                temp_v0->y_position = temp_v0->unk180;
                temp_v0->unk1CC = temp_v0->unk180;
                temp_v0->gravity = 5.0f;
            }

            temp_v1->unk81 = 0;
            temp_v1->unk83 = 0;
            temp_v1->disable_run = 0;
            temp_v1->disable_jump = 0;
            temp_v1->stunned = 0;
            temp_v1->unk28 = temp_v1->y_position - temp_v1->unk180;
            D_800D154C = temp_v1;
            func_1507F640();
            ((u8 *)temp_v1->unk31C)[0x23] = 0;
            temp_v1->unk287 = 0;
            temp_v0_2 = temp_v1->unk287;
            temp_v1->unk276 = 0;
            temp_v1->unk282 = 0;
            temp_v1->unk278 = 0;
            temp_v1->unk286 = temp_v0_2;
            temp_v1->unk285 = temp_v0_2;
            temp_v1->unk284 = temp_v0_2;
            return temp_v0_2;
        }
    }
}

void func_150627D4(struct127 *arg0);
extern s32 D_800C3654;
extern u8 D_800C3E78;
extern struct127 D_800CC2D0[26];
extern void func_1504A730(void);
extern void func_1507E73C(struct127 *);
extern void func_1507FEA0(struct127 *);
extern void func_15061B4C(void);
extern void func_1502178C(struct127 *, s32, s32);
extern s32 func_150229E4(struct127 *);
void func_15022998(s32 *arg0);
void func_1502378C(void)
{
  s32 new_var;
  s32 *new_var2;
  s32 i;
  s32 limit;
  s32 active;
  struct127 *obj;
  obj = D_800CC2D0;
 i = 0; if ((*((u8 *) (&D_800C3654))) != 0) { func_1504A730(); return; } for (active = 1, limit = 0x19; i != (limit ^ 0); obj++) { if (obj->interaction_state != 0) { D_800C3E78 = i;
      func_15022998(new_var2 = (s32 *) obj);
      func_1507E73C(obj);
      new_var = func_150229E4(obj);
      if (new_var != 0)
      {
 do { } while (0);
        func_1502178C(obj, 0, -1);
        func_150627D4(obj);
        func_1507FEA0(obj);
        obj->unk2FF = active & 0xFFFFFFFF;
      }
    }
    i++;
  }

  func_15061B4C();
}
