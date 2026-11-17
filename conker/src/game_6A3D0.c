#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 allocate_memory(s32, s32, s32, s32);
extern s32 *D_800C4020[];

struct Src1503D804 {
    u8 pad0[8];
    s16 unk8;
    s16 unkA;
    u8 padC[4];
};

struct Dst1503D804 {
    s16 unk0;
    s16 unk2;
};

struct D800D19A0Header {
    u8 pad_0[0x30];
    u8 *field_0x30;
    u32 field_0x34;
};

struct Hdr1503CF20 {
    s32 *unk0;
    u32 unk4;
    Gfx **unk8;
    u32 unkC;
    s32 unk10;
    u32 unk14;
    s32 unk18;
    u32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u32 unk2C;
    s32 unk30;
    s32 unk34;
};

struct E1503DC3C;
struct124 *func_1502B6BC(s32, s32, s32, s32, s32, s32);
extern Gfx **D_800C4488[];
extern s32 D_800C48F0[];
extern s32 D_800C4BE0[];
extern struct E1503DC3C *D_800C5338[];
extern s32 D_800C5048[];
extern u16 D_800C4310[];
extern u16 D_800C57A0[];
extern u16 D_800C4778[];
extern u16 D_800C5628[];
extern u16 D_800C4ED0[];
extern void func_1503D438(s32 *arg0, s32 arg1);
extern void func_1503D368(s8 *, s32);
extern s32 func_1503DC3C(s32);
extern void func_1510CE60(s32, s32, s32, s32, s32 *);
extern void func_1503D984(s32 arg0);
extern s32 func_1503D804(s32 arg0);
extern s32 func_150028BC(s32);
extern s32 func_1503D774(s32 arg0, s32 arg1);
extern s32 func_1503D660(s32 arg0, s32 arg1);
extern void func_1503DD1C(s32 arg0);
extern void func_10004074(u32);

s32 func_1503CF20(s32 arg0, s32 arg1, void *arg2, s32 arg3, s32 arg4) {
    s32 ret;
    struct Hdr1503CF20 *hdr;
    s32 size;
    u32 n;
    s32 i;

    if (arg0 == 0xFF) {
        return 0;
    }
    if (arg0 >= 0xBB) {
        return 0;
    }
    ret = 0;
    if ((&D_800D19A0)[arg0] != 0) {
        return 0;
    }
    hdr = (struct Hdr1503CF20 *)func_1502B6BC((s32)&size, 7, 0, 2, 1, arg0);
    if (hdr == NULL) {
        return 1;
    }
    (&D_800D19A0)[arg0] = (s32)hdr + 0x38;
    D_800C4020[arg0] = hdr->unk0;
    D_800C4488[arg0] = hdr->unk8;
    D_800C48F0[arg0] = hdr->unk28;
    D_800C4BE0[arg0] = hdr->unk10;
    D_800C5338[arg0] = (struct E1503DC3C *)hdr->unk18;
    D_800C5048[arg0] = hdr->unk20;
    D_800C4310[arg0] = hdr->unk4 >> 2;
    if (D_800C4020[arg0] != NULL) {
        n = (u32)D_800C4020[arg0];
    } else {
        n = (u32)D_800C4488[arg0];
    }
    D_800C57A0[arg0] = (n - (u32)hdr - 0x38) >> 4;
    D_800C4778[arg0] = hdr->unkC >> 2;
    for (i = 0; i < D_800C4778[arg0]; i++) {
        func_1503D438((s32 *)&D_800C4488[arg0][i], (s32)hdr);
        func_1503D368((s8 *)D_800C4488[arg0][i], (s32)hdr);
    }
    n = hdr->unk2C >> 2;
    for (i = 0; i < n; i++) {
        func_1503D438(&((s32 **)D_800C48F0)[arg0][i], (s32)hdr);
        func_1503D368((s8 *)((s32 **)D_800C48F0)[arg0][i], (s32)hdr);
    }
    D_800C5628[arg0] = hdr->unk1C / 12;
    D_800C4ED0[arg0] = hdr->unk14 >> 4;
    if (arg3 != 0) {
        ret |= func_1503DC3C(arg0);
        if (ret == 0) {
            for (i = 0; i < D_800C4778[arg0]; i++) {
                func_1510CE60((s32)D_800C4488[arg0][i], 0, 0, 0x3E, NULL);
            }
        }
    }
    if (ret == 0) {
        func_1503D984(arg0);
        ret |= func_1503D804(arg0);
    }
    if (ret == 0 && ((arg2 != NULL && (*(s32 *)((u8 *)arg2 + 0x18) & 0x4000)) || (arg4 & 1))) {
        if (func_150028BC(arg0) != 0) {
            ret |= 8;
        }
    }
    if (ret == 0) {
        ret |= func_1503D774(arg0, 0);
        if (ret == 0) {
            ret |= func_1503D660(arg0, 0);
        }
    }
    if (ret != 0) {
        if (((u32 *)D_800C6360)[arg0] != 0) {
            func_10004074(((u32 *)D_800C6360)[arg0]);
        }
        func_1503DD1C(arg0);
        if (D_800C5C08[arg0] != 0) {
            func_10004074(D_800C5C08[arg0]);
            D_800C5C08[arg0] = 0;
        }
        if (D_800C6070[arg0] != 0) {
            func_10004074(D_800C6070[arg0]);
            D_800C6070[arg0] = 0;
        }
        if ((&D_800D19A0)[arg0] != 0) {
            func_10004074((&D_800D19A0)[arg0] - 0x38);
            (&D_800D19A0)[arg0] = 0;
        }
    }
    return ret;
}

void func_1503D368(s8 *arg0, s32 arg1) {
    s32 i;
    s8 *entry;

    if (arg0 != NULL) {
        for (i = 0; arg0[i << 3] != -0x21; i++) {
            entry = &arg0[i << 3];
            switch (entry[0]) {
            case 1:
                func_1503D438((s32 *)&entry[4], arg1);
                break;
            case -0x24:
                if ((u8)entry[3] == 0xE) {
                    func_1503D438((s32 *)&entry[4], arg1);
                }
                break;
            }
        }
    }
}

void func_1503D438(s32 *arg0, s32 arg1) {
    s32 temp = *arg0;
    if (temp != 0 && (temp & 0x0F000000) == 0) {
        *arg0 = temp + arg1;
    }
}

void func_1503D45C(s32 *arg0, s32 arg1) {
    s32 temp = *arg0;
    while (temp != 0) {
        *arg0 = temp + arg1;
        temp = arg0[2];
        arg0 += 2;
    }
}

struct Elem1503D484 {
    u16 unk0;
    s32 unk4;
};

void func_1503D484(struct Elem1503D484 *arg0, s32 arg1) {
    struct Elem1503D484 *p = arg0;

    while (arg0->unk0 != 0x3E7) {
        if (arg0->unk4 != 0) {
            func_1503D438(&arg0->unk4, (s32)p);
        }
        arg0++;
    }

    D_800C5A90[arg1] = arg0 - p;
}

extern u8 D_80098888[];
extern u8 *D_80084410[];

void func_1503D510(s32 arg0) {
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < D_80098888[i]; j++) {
            if (D_80084410[i][j] == arg0) {
                for (k = 0; k < D_80098888[i]; k++) {
                    D_800D1588[D_80084410[i][k]] = D_800D1588[arg0];
                    ((u16 *)D_800C5A90)[D_80084410[i][k]] = ((u16 *)D_800C5A90)[arg0];
                }
                return;
            }
        }
    }
}


extern u8 D_80098888[];
extern u8 *D_80084410[];

/* Looks arg0 up in the 5 rows of D_80084410 (row lengths in D_80098888) and returns the
 * first entry of the row that contains it, or arg0 itself when no row does.
 * Same search as func_15084D00 (game_B21B0). */
s32 func_1503D5F0(s32 arg0) {
    s32 i;
    s32 j;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < D_80098888[i]; j++) {
            if (D_80084410[i][j] == arg0) {
                return D_80084410[i][0];
            }
        }
    }
    return arg0;
}

typedef struct Hdr1503D660 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Hdr1503D660;

Hdr1503D660 *func_1502B5C8(s32 *, s32, s32, s32);

/* D_800D1588[] holds one loaded header block per id; the golden re-reads the slot on every use. */
#define SLOT (((Hdr1503D660 **)D_800D1588)[arg0])

s32 func_1503D660(s32 arg0, s32 arg1) {
    s32 size;
    Hdr1503D660 *p;

    if (SLOT != NULL) {
        return 0;
    }
    p = func_1502B5C8(&size, 2, 15, func_1503D5F0(arg0));
    if (p == NULL) {
        SLOT = NULL;
        D_800C5A90[arg0] = 0;
        func_1503D510(arg0);
        return 4;
    }
    SLOT = p;
    if ((SLOT)->unk0 != 0) {
        (SLOT)->unk0 += (s32)SLOT;
        func_1503D45C((s32 *)(SLOT)->unk0, (s32)(SLOT + 1));
    }
    if ((SLOT)->unk4 != 0) {
        (SLOT)->unk4 += (s32)SLOT;
    }
    if ((SLOT)->unk8 != 0) {
        (SLOT)->unk8 += (s32)SLOT;
    }
    SLOT = SLOT + 1;
    func_1503D484((struct Elem1503D484 *)SLOT, arg0);
    func_1503D510(arg0);
    return 0;
}

#undef SLOT

extern struct124 *D_800D1C90[];
struct124 *func_1502B6BC(s32, s32, s32, s32, s32, s32);

s32 func_1503D774(s32 arg0, s32 arg1) {
    s32 sp2C;
    struct124 *r;

    if (D_800D1C90[arg0] != 0) {
        return 0;
    }
    r = func_1502B6BC((s32)&sp2C, 2, 0, 2, 17, arg0);
    if (r == 0) {
        D_800D1C90[arg0] = 0;
        return 2;
    } else {
        D_800D1C90[arg0] = r;
        D_800D1C90[arg0] = r->unk0;
    }
    return 0;
}

s32 func_1503D804(s32 arg0) {
    s32 count;
    s32 temp_v1;
    struct Src1503D804 *src;
    s32 *slot;

    if (arg0 == 0x24) {
        count = 0x5E;
        temp_v1 = 0;
    } else {
        temp_v1 = -1;
        count = 0;
    }

    if (count == 0) {
        return 0;
    }

    slot = &D_800C6360[arg0];
    if (*slot != 0) {
        return 0;
    }

    src = (struct Src1503D804 *)(&D_800D19A0)[arg0];
    if (temp_v1 != -1) {
        src += D_800C4020[arg0][temp_v1];
    }

    {
        s32 i;

        *slot = allocate_memory(count * sizeof(struct Dst1503D804), 1, 0, 2);
        if (*slot == 0) {
            return 1;
        }

        slot = (s32 *)*slot;
        for (i = 0; i < count; i++) {
            ((struct Dst1503D804 *)slot)[i].unk0 = src[i].unk8;
            ((struct Dst1503D804 *)slot)[i].unk2 = src[i].unkA;
        }
    }

    return 0;
}

extern Gfx **D_800C4488[];
extern s16 D_800C5918[];

/* sums primitive weights over the display list up to G_ENDDL: G_TRI1 1, G_TRI2 2, ops 0x10-0x1F 4 */
void func_1503D984(s32 arg0) {
    s32 count = 0;
    s32 i;
    s32 j;
    Gfx *dl;
    s8 op;

    for (i = 0; i < 1; i++) {
        dl = D_800C4488[arg0][i];
        j = 0;
        op = dl[j].words.w0 >> 24;
        while (op != (s8)G_ENDDL) {
            if (op == G_TRI1) {
                count += 1;
            } else if (op == G_TRI2) {
                count += 2;
            } else if ((op >> 4) == 1) {
                count += 4;
            }
            j++;
            op = dl[j].words.w0 >> 24;
        }
    }
    D_800C5918[arg0] = count;
}


s32 func_1503DA3C(s32 arg0, s32 arg1) {
    u8 *v0 = (u8 *)(&D_800D19A0)[arg0];
    struct D800D19A0Header *v1 = (struct D800D19A0Header *)(v0 - 0x38);
    u8 *temp;
    s32 ret;

    if (v0 == 0) {
        return 0xFF;
    }
    if (v1->field_0x34 < (u32)(arg1 + 1)) {
        return 0xFF;
    }
    temp = v1->field_0x30;
    ret = temp ? temp[arg1] : 0xFF;
    return ret;
}

extern s32 func_1503CF20(s32, s32, void *, s32, s32);
extern s32 func_150028BC(s32);
extern s32 func_15084044(void *, s32);
extern void func_1510CE60(s32, s32, s32, s32, s32 *);
extern s32 func_1503DC3C(s32);
extern u16 D_800C4778[];
extern u16 D_800C5628[];
extern struct E1503DC3C *D_800C5338[];

s32 func_1503DA9C(u8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *obj;
    s32 pad2;
    s32 ret;
    s32 i;

    obj = *(u8 **)(arg0 + 0x144);
    if ((&D_800D19A0)[arg1] == 0) {
        ret = func_1503CF20(arg1, 0, obj, arg3, arg0[0xAC]);
    } else {
        ret = 0;
        if (((obj != NULL) && (*(s32 *)(obj + 0x18) & 0x4000)) || (arg0[0xAC] & 1)) {
            if (D_800C5C08[arg1] == 0) {
                func_150028BC(arg1);
            }
        }
        if (arg3 != 0 && D_800C5628[arg1] != 0 && *(u32 *)D_800C5338[arg1] < 0x10000000U) {
            ret = func_1503DC3C(arg1);
            for (i = 0; i < D_800C4778[arg1]; i++) {
                func_1510CE60(((s32 **)D_800C4488)[arg1][i], 0, 0, 0x3E, 0);
            }
        }
    }
    if (ret == 0) {
        ret = func_15084044(arg0, arg2);
    }
    return ret;
}

extern u16 D_800C5628[];
extern struct E1503DC3C *D_800C5338[];
extern s32 func_1510D0EC(s32, s32, s32, s32);

struct E1503DC3C {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

s32 func_1503DC3C(s32 arg0) {
    s32 ret = 0;
    s32 i = 0;

    if ((s32)D_800C5628[arg0] > 0) {
        struct E1503DC3C *ptr = D_800C5338[arg0];
        s32 j = 0;
        do {
            *(s32 *)((s32)D_800C5338[arg0] + j) = func_1510D0EC(ptr->unk4, 0, 0x3E, 1);
            ptr = (struct E1503DC3C *)((s32)D_800C5338[arg0] + j);
            if (ptr->unk0 == (s32)0x80000000) {
                ret |= 0x10;
            }
            i++;
            j += 0xC;
            ptr++;
        } while (i < (s32)D_800C5628[arg0]);
    }
    return ret;
}

/* Real definition (game_139FC0) takes s32; golden's `or a0,a1` copy in the jal delay slot
   is reproduced by the callee being declared with an unsigned parameter here. */
extern void func_1510D7AC(u32);

void func_1503DD1C(s32 arg0) {
    s32 i;

    for (i = 0; i < D_800C5628[arg0]; i++) {
        struct E1503DC3C *e = &D_800C5338[arg0][i];

        if ((e->unk0 != (s32)0x80000000) && (e->unk4 != e->unk0)) {
            func_1510D7AC(e->unk4);
        }
    }
}
