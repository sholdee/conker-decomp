#include <ultra64.h>
#define func_151422C0 func_151422C0_header
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_1513D2F0 func_1513D2F0_hdr
#define func_1513D524 func_1513D524_hdr
#include "functions.h"
#undef func_151422C0
#undef func_150ADA20
#undef func_1513D2F0
#undef func_1513D524
u32 func_150ADA20(void);
void *func_1513D2F0(void *arg0, void *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8, s32 arg9, u8 argA, s32 argB);
#define D_800DBFF0 D_800DBFF0_hdr
#define D_800BE628 D_800BE628_hdr
#include "variables.h"
#undef D_800DBFF0
#undef D_800BE628

void *memcpy(void *, const void *, u32);

void func_1519CDB0(s32, f32, s32);
extern void *allocate_memory(s32, s32, s32, s32);

s32 func_15195DD4(s32, s32, s32, s32, s32, s32, s32);
s32 func_151422C0();
struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, u8, u8, s32, u8, s32);

extern char D_800A8A40[];
extern char D_800A8A48[];
extern f32 D_800A8AA8;
extern f32 D_800A8AF8;

extern s32 D_800E08E0;
extern s32 D_800E08E4;
extern s32 D_800E08E8;
extern s32 D_800E08EC;
extern s32 D_800E08F0;

typedef struct Struct151957B0 {
    struct Struct151957B0 *prev;
    struct Struct151957B0 *next;
} Struct151957B0;

void *func_151957B0(s32 size, void *head, void *tail) {
    Struct151957B0 *node;
    Struct151957B0 *prev;

    node = allocate_memory(size, 1, 0, 0);
    if (node != 0) {
        node->next = 0;
        prev = *(Struct151957B0 **)tail;
        if (prev != 0) {
            node->prev = prev;
            (*(Struct151957B0 **)tail)->next = node;
            *(Struct151957B0 **)tail = node;
        } else {
            node->prev = 0;
            *(Struct151957B0 **)tail = node;
            *(Struct151957B0 **)head = node;
        }
    }
    return node;
}

void trailing_151957B0(void) {
}

void func_1519582C(void) {
    D_800E08E0 = D_800E08E4 = 0;
    D_800E08E8 = D_800E08EC = 0;
    D_800E08F0 = -2;
}

typedef struct {
    s8 type;
    char pad[3];
    s32 val;
} Struct15195868;

extern s32 D_800B0E58[];

#define TYPE(n) (*(s8 *)(arg0 + ((n) << 3)))
#define VAL(n) (((Struct15195868 *)arg0)[n].val)

s32 func_15195868(s32 arg0, s32 arg1, s32 arg2, s32 *arg3) {
    s32 i;
    s32 c;
    s8 d;
    s8 e;

    *arg3 = 0;
    i = -1;
loop:
    i++;
    while (TYPE(i) != -3 && TYPE(i) != -0x21) {
        i++;
    }
    if (TYPE(i) == -0x21) {
        return -1;
    }
    if ((VAL(i) == D_800B0E58[arg1] || arg1 == 0) && arg2-- < 1) {
        goto found;
    }
    goto loop;
found:
    d = TYPE(i);
    while (d != -0xE) {
        i++;
        d = TYPE(i);
    }
    c = i;
    do {
        (*arg3)++;
        d = TYPE(++c);
        e = TYPE(++c);
    } while (d == -0xB && e == -0xE);
    return i;
}

extern void *func_151957B0(s32, void *, void *);
extern s32 D_800E08E0;
extern s32 D_800E08E4;
extern s32 D_800E08E8;
extern s32 D_800E08EC;

typedef struct Struct15195984 {
    struct Struct15195984 *prev;
    struct Struct15195984 *next;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    char pad15[0x18 - 0x15];
    s32 unk18;
    s16 unk1C[5];
    s16 unk26[5];
    u8 unk30[5];
    u8 unk35[5];
} Struct15195984;

void *func_15195984(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Struct15195984 *v0;
    s32 i;
    s32 hi;
    s32 lo;

    v0 = func_151957B0(0x3C, &D_800E08E8, &D_800E08EC);
    if (v0 != 0) {
        v0->unk18 = 0;
        for (i = 0; i < 5; i++) {
            v0->unk1C[i] = -1;
        }
        for (i = 0; i < 5; i++) {
            v0->unk26[i] = -1;
        }
        v0->unk8 = arg0;
        hi = ((arg1 >> 12) & 0xFFF) + 2;
        lo = (arg1 & 0xFFF) + 2;
        if (arg3 != 0) {
            hi <<= 1;
            lo <<= 1;
        }
        v0->unkE = (arg2 >> 12) & 0xFFF;
        v0->unk10 = arg2 & 0xFFF;
        v0->unkA = hi;
        v0->unkC = lo;
        v0->unkE = (u16)v0->unkE << 3;
        v0->unk10 = (u16)v0->unk10 << 3;
        v0->unk14 = 1;
    }
    return v0;
}

s16 func_15195A84(s16 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return arg0;
}

typedef struct {
    s32 pos;
    s32 size;
} Struct15195DD4Cmd;

extern s32 func_15195868(s32, s32, s32, s32 *);

void *func_15195AA8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 idx;
    s32 key;
    s32 size;
    s32 depth;
    s32 pos;
    s32 i = arg3;
    Struct15195984 *node = 0;

    if (arg0 == 0) {
        return 0;
    }
    if (arg3 == -1) {
        i = 0;
    }
    do {
        idx = func_15195868(arg0, arg1, i, &depth);
        if (idx == -1) {
            arg3 = 0;
        } else {
            if (arg1 == 0) {
                arg1 = D_800E08F0;
            }
            size = ((Struct15195DD4Cmd *) arg0 + idx)->size;
            pos = ((Struct15195DD4Cmd *) arg0 + idx)->pos;
            node = (Struct15195984 *) D_800E08E8;
            if (arg4 != 0) {
                key = -1;
            } else {
                key = arg1;
            }
            while ((node != 0) && (node->unk8 != 0)) {
                node = node->next;
            }
            if (node == 0) {
                node = func_15195984(arg1, size, pos, arg2);
                if (node != 0) {
                    arg4 = 0;
                    node->unk12 = arg6;
                    node->unk13 = arg7;
                } else {
                    return 0;
                }
            } else {
                idx = func_15195A84(idx, (s32) node, size, arg2);
                arg4 = 0;
            }
            if (node->unk14 < depth) {
                node->unk14 = depth;
            }
            if (arg5 == 0) {
                key = 0;
                while (key < 5 && node->unk1C[key] != -1) {
                    key++;
                }
                if (key < 5) {
                    node->unk1C[key] = idx;
                    node->unk30[key] = depth;
                }
            } else {
                key = 0;
                while (key < 5 && node->unk26[key] != -1) {
                    key++;
                }
                if (key < 5) {
                    node->unk26[key] = idx;
                    node->unk35[key] = depth;
                }
            }
            i++;
        }
    } while (arg3 == -1);
    if (arg1 == D_800E08F0) {
        D_800E08F0--;
    }
    return node;
}

typedef struct Struct15195D00 {
    char pad0[0x8];
    s32 unk8;
    u16 unkC;
    u8 unkE;
    u8 unkF;
    struct Struct15195D00 *unk10;
} Struct15195D00;

void func_15195D00(Struct15195984 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    Struct15195D00 *temp_v1;
    Struct15195D00 *temp_v0;
    s32 temp_arg3;

    temp_v0 = (Struct15195D00 *) arg0->unk18;
    if (temp_v0 != 0) {
        temp_v1 = temp_v0;
        do {
            if ((arg1 == temp_v1->unk8) && (arg2 == temp_v1->unkC)) {
                return;
            }
            temp_v0 = temp_v1->unk10;
            if (temp_v0 != 0) {
                temp_v1 = temp_v0;
                temp_v0 = temp_v0->unk10;
            }
        } while (temp_v0 != 0);
        temp_v0 = func_151957B0(0x14, &D_800E08E0, &D_800E08E4);
        temp_v1->unk10 = temp_v0;
    } else {
        temp_v0 = func_151957B0(0x14, &D_800E08E0, &D_800E08E4);
        arg0->unk18 = (s32) temp_v0;
    }
    temp_v0->unk8 = arg1;
    temp_v0->unkC = arg2;
    temp_arg3 = arg3;
    temp_v0->unkE = temp_arg3;
    temp_v0->unk10 = 0;
}

extern s32 func_15195868(s32, s32, s32, s32 *);

s32 func_15195DD4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 idx;
    s32 key;
    s32 size;
    s32 depth;
    s32 pos;
    s32 i = arg3;
    Struct15195984 *node;

    if (arg0 == 0) {
        return 0;
    }
    if (D_800BEAC0 != 0) {
        return 0;
    }
    if (arg3 == -1) {
        i = 0;
    }
    do {
        idx = func_15195868(arg0, arg1, i, &depth);
        if (idx == -1) {
            arg3 = 0;
        } else {
            if (arg1 == 0) {
                arg1 = D_800E08F0;
            }
            if (arg4 != 0) {
                key = -1;
            } else {
                key = arg1;
            }
            node = (Struct15195984 *) D_800E08E8;
            while ((node != 0) && (key != node->unk8)) {
                node = node->next;
            }
            size = ((Struct15195DD4Cmd *) arg0 + idx)->size;
            pos = ((Struct15195DD4Cmd *) arg0 + idx)->pos;
            if (node == 0) {
                node = func_15195984(arg1, size, pos, arg2);
                if (node != 0) {
                    arg4 = 0;
                    node->unk12 = arg5;
                    node->unk13 = arg6;
                } else {
                    return 0;
                }
            } else {
                idx = func_15195A84(idx, (s32) node, size, arg2);
                arg4 = 0;
            }
            if (node->unk14 < depth) {
                node->unk14 = depth;
            }
            func_15195D00(node, arg0, idx, depth);
            i++;
        }
    } while (arg3 == -1);
    if (arg1 == D_800E08F0) {
        D_800E08F0--;
    }
    return (s32) node;
}

typedef struct {
    char pad_0x0[0x1C];
    s32 field_0x1C;
} ResourceEntry;

s32 func_15195FB0(ResourceEntry *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    return func_15195DD4(arg0->field_0x1C, arg1, arg2, arg3, arg4, arg5, arg6);
}

typedef struct {
    /* 0x0 */ u32 w0;
    /* 0x4 */ u32 w1;
} Cmd15195FF0;

typedef struct Sub15195FF0 {
    /* 0x00 */ char pad00[0x8];
    /* 0x08 */ Cmd15195FF0 *unk08;
    /* 0x0C */ u16 unk0C;
    /* 0x0E */ u8 unk0E;
    /* 0x0F */ u8 unk0F;
    /* 0x10 */ struct Sub15195FF0 *unk10;
} Sub15195FF0;

typedef struct Struct15195FF0 {
    /* 0x00 */ struct Struct15195FF0 *prev;
    /* 0x04 */ struct Struct15195FF0 *next;
    /* 0x08 */ s16 unk08;
    /* 0x0A */ s16 unk0A;
    /* 0x0C */ s16 unk0C;
    /* 0x0E */ u16 unk0E;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ s8 unk12;
    /* 0x13 */ s8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ char pad15[0x3];
    /* 0x18 */ Sub15195FF0 *unk18;
    /* 0x1C */ s16 unk1C[5];
    /* 0x26 */ s16 unk26[5];
    /* 0x30 */ u8 unk30[5];
    /* 0x35 */ u8 unk35[5];
} Struct15195FF0;

void func_15195FF0(Cmd15195FF0 *arg0, Cmd15195FF0 *arg1) {
    Struct15195FF0 *node;
    u32 sp5C[8];
    Sub15195FF0 *sub;
    s32 uls;
    s32 ult;
    s32 w;
    s32 h;
    s32 x;
    s32 y;
    s32 i;
    s32 k;
    s32 idx;

    node = (Struct15195FF0 *)D_800E08E8;
    while (node != NULL) {
        w = node->unk0A;
        h = node->unk0C;
        x = node->unk0E;
        y = node->unk10;
        uls = x / 8 + 2;
        ult = y / 8 + 2;
        if (uls >= w) {
            uls = uls - w;
        }
        if (ult >= h) {
            ult = ult - h;
        }
        for (i = 0; i < 8 && i < node->unk14; i++) {
            sp5C[i] = 0xF2000000 | ((uls & 0xFFF) << 12) | (ult & 0xFFF);
            uls /= 2;
            ult /= 2;
        }
        w *= 8;
        h *= 8;
        if (D_800BEAC0 == 0) {
            x += node->unk12 * D_800BE9E4;
            y += node->unk13 * D_800BE9E4;
        }
        if (x < 0) {
            x += w;
        } else if (x >= w) {
            x -= w;
        }
        if (y < 0) {
            y += h;
        } else if (y >= h) {
            y -= h;
        }
        node->unk0E = x;
        node->unk10 = y;
        for (k = 0; k < 5 && node->unk1C[k] != -1; k++) {
            idx = node->unk1C[k];
            for (i = 0; i < node->unk30[k]; i++) {
                arg0[idx].w0 = sp5C[i];
                idx += 2;
            }
        }
        for (k = 0; k < 5 && node->unk26[k] != -1; k++) {
            idx = node->unk26[k];
            for (i = 0; i < node->unk35[k]; i++) {
                arg1[idx].w0 = sp5C[i];
                idx += 2;
            }
        }
        sub = node->unk18;
        while (sub != NULL) {
            idx = sub->unk0C;
            for (i = 0; i < sub->unk0E; i++, idx += 2) {
                sub->unk08[idx].w0 = sp5C[i];
            }
            if (sub->unk10 != NULL) {
                sub = sub->unk10;
            } else {
                sub = NULL;
            }
        }
        node = node->next;
    }
}

void func_15196318(u8 *arg0, s32 arg1, s32 arg2) {
    if (arg0 != 0) {
        arg0[0x12] = arg1;
        arg0[0x13] = arg2;
    }
}

extern void (*D_8008F898[])(void *);
extern void (*D_8008F88C[])(void *);
void func_151478F4(void *);
void func_15147928(void *);

void func_15196330(void *arg0) {
    s8 *temp;

    temp = *(s8 **)((u8 *)arg0 + 0x98);
    if (temp[0x65] != -1) {
        D_8008F898[temp[0x65]](arg0);
    }
    if (temp[0x62] != -1) {
        D_8008F88C[temp[0x62]](arg0);
    }
    func_151478F4(arg0);
}

void func_151963B4(void *arg0) {
    s8 *temp;

    temp = *(s8 **)((u8 *)arg0 + 0x98);
    if (temp[0x65] != -1) {
        D_8008F898[temp[0x65]](arg0);
    }
    if (temp[0x62] != -1) {
        D_8008F88C[temp[0x62]](arg0);
    }
    func_15147928(arg0);
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Vec15196438;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f15196438;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x18 */ Vec15196438 unk18;
    /* 0x24 */ Vec15196438 unk24;
    /* 0x30 */ s8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s8 unk3C;
    /* 0x3D */ s8 unk3D;
    /* 0x3E */ s8 unk3E;
    /* 0x3F */ s8 unk3F;
    /* 0x40 */ s8 unk40;
    /* 0x41 */ s8 unk41;
    /* 0x42 */ s8 unk42;
    /* 0x44 */ u16 unk44;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ u8 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ u8 unk5C;
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u8 unk5E;
    /* 0x5F */ u8 unk5F;
} Entry15196438;

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ s8 unk6;
    /* 0x07 */ s8 unk7;
    /* 0x08 */ Vec15196438 unk8;
    /* 0x14 */ Vec15196438 unk14;
    /* 0x20 */ Vec3f15196438 unk20;
    /* 0x2C */ Vec3f15196438 unk2C;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ f32 unk5C;
    /* 0x60 */ s8 unk60;
    /* 0x61 */ s8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ s8 unk65;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s8 unk68;
    /* 0x69 */ u8 unk69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ f32 unk70;
    /* 0x74 */ u8 unk74;
    /* 0x78 */ f32 unk78;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ u8 unk80;
    /* 0x84 */ f32 unk84;
    /* 0x88 */ f32 unk88;
    /* 0x8C */ u8 unk8C;
    /* 0x8D */ u8 unk8D;
    /* 0x8E */ u8 unk8E;
    /* 0x8F */ u8 unk8F;
} Payload15196438;

typedef struct {
    /* 0x00 */ Vec3f15196438 unk0;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u16 unkE;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 unk18;
} Header15196438;

extern Entry15196438 D_800A87A0[];
extern s16 (*D_8008F890[])(void);
extern f32 (*D_8008F870[])(void *, s32);
extern void (*D_8008F87C[])(void *);
void func_15143134(void *, Vec3f15196438 *, s32);
void *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void *func_15196438(void *arg0, s32 arg1, u8 arg2, s32 arg3)
{
  Header15196438 header;
  Payload15196438 payload;
  void *obj;
  s32 base;
  Entry15196438 *entry;
  Payload15196438 *p;
  if (arg0 == 0)
  {
    return 0;
  }
  if ((*((s32 *) (((u8 *) arg0) + 0x1D4))) == 0)
  {
    return 0;
  }
  entry = &D_800A87A0[arg1];
  payload.unk0 = arg0;
  payload.unk4 = *(((u8 *) arg0) + 0x3B);
  payload.unk6 = 2;
  payload.unk50 = 0;
  payload.unk4C = 0.0f;
  payload.unk70 = 0.0f;
  payload.unk7C = 0.0f;
  payload.unk88 = 0.0f;
  payload.unk60 = entry->unk3C;
  payload.unk61 = entry->unk3D;
  payload.unk62 = entry->unk3E;
  header.unkC = entry->unk0;
  payload.unk40 = entry->unk4;
  payload.unk48 = entry->unk8;
  payload.unk51 = entry->unkC;
  header.unk15 = entry->unk10;
  payload.unk5 = entry->unk14;
  payload.unk8 = entry->unk18;
  payload.unk14 = entry->unk24;
  payload.unk7 = entry->unk30;
  payload.unk52 = entry->unk31;
  payload.unk53 = entry->unk32;
  payload.unk54 = entry->unk33;
  header.unkE = entry->unk44;
  payload.unk6A = entry->unk47;
  payload.unk74 = entry->unk4C;
  payload.unk80 = entry->unk54;
  payload.unk63 = entry->unk3F;
  payload.unk58 = entry->unk34;
  payload.unk5C = entry->unk38;
  payload.unk6C = entry->unk48;
  payload.unk78 = entry->unk50;
  payload.unk84 = entry->unk58;
  payload.unk64 = entry->unk40;
  payload.unk65 = entry->unk41;
  payload.unk68 = entry->unk42;
  payload.unk69 = entry->unk46;
  payload.unk8C = entry->unk5C;
  payload.unk8D = entry->unk5D;
  payload.unk8E = entry->unk5E;
  payload.unk8F = entry->unk5F;
  if (payload.unk63 != (-1))
  {
    payload.unk66 = D_8008F890[payload.unk63]();
  }
  else
  {
    payload.unk66 = 0;
  }
  base = (*((s32 *) (((u8 *) payload.unk0) + 0x1D4))) + (payload.unk5 << 6);
  func_15143134(&payload.unk8, &payload.unk20, base);
  func_15143134(&payload.unk14, &payload.unk2C, base);
  header.unk10 = 3;
  header.unk0.x = payload.unk20.x;
  header.unk0.y = payload.unk20.y;
  header.unk0.z = payload.unk20.z;
  payload.unk3C = 0.0f;
  obj = func_15147A80(&header, 0x90, 0x24, 2, 2, 2, 0, 0, 0, arg2, arg3);
  if (obj != 0)
  {
    p = *((Payload15196438 **) (((u8 *) obj) + 0x98));
    memcpy(*((Payload15196438 **) (((u8 *) obj) + 0x98)), &payload, 0x90);
    if (p->unk7 != (-1))
    {
      p->unk38 = D_8008F870[p->unk7](obj, p->unk7);
    }
    else
    {
      p->unk38 = 0.0f;
    }
    if (payload.unk60 != (-1))
    {
      D_8008F87C[payload.unk60](obj);
    }
  }
  return obj;
}

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
} Vec15196748;

typedef struct {
    /* 0x00 */ Vec15196748 pos;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 pad22[0x2];
} Particle15196748;

typedef struct {
    /* 0x00 */ u8 pad0[0x6];
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 pad7[0x40 - 0x7];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ u8 pad44[0x52 - 0x44];
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 pad53[0x58 - 0x53];
    /* 0x58 */ f32 unk58;
    /* 0x5C */ u8 pad5C[0x61 - 0x5C];
    /* 0x61 */ s8 unk61;
    /* 0x62 */ u8 pad62[0x64 - 0x62];
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 pad65[0x68 - 0x65];
    /* 0x68 */ s8 unk68;
    /* 0x69 */ u8 pad69[0x6A - 0x69];
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B[0x8C - 0x6B];
    /* 0x8C */ u8 unk8C;
    /* 0x8D */ u8 unk8D;
    /* 0x8E */ u8 unk8E;
    /* 0x8F */ u8 unk8F;
} Desc15196748;

typedef struct {
    /* 0x00 */ u8 pad0[0x25];
    /* 0x25 */ u8 unk25;
    /* 0x26 */ u8 pad26[0x2C - 0x26];
    /* 0x2C */ s8 unk2C;
    /* 0x2D */ s8 unk2D;
    /* 0x2E */ s8 unk2E;
    /* 0x2F */ u8 pad2F[0x54 - 0x2F];
    /* 0x54 */ Vec15196748 unk54;
    /* 0x60 */ u8 pad60[0x94 - 0x60];
    /* 0x94 */ Particle15196748 *unk94;
    /* 0x98 */ Desc15196748 *unk98;
} Emitter15196748;

extern void (*D_8008F894[])(void *);
extern void (*D_8008F89C[])(void *);
extern s32 (*D_8008F880[])(void *);
void func_15198110();

s32 func_15196748(Emitter15196748 *arg0) {
    Desc15196748 *desc;
    Particle15196748 *arr;
    s32 i;
    s16 n;
    u8 scale;
    s8 life;
    u8 flags;
    u8 descFlags;

    desc = arg0->unk98;
    arr = arg0->unk94;
    if (arg0->unk2C < 2 && (desc->unk6 & 1)) {
        return 0;
    }
    i = arg0->unk2E;
    while (i != arg0->unk2D) {
        i--;
        if (i < 0) {
            i = arg0->unk25 - 1;
        }
        arr[i].unk10 += desc->unk40 * D_800BE9A4;
        arr[i].pos.x += arr[i].unkC * D_800BE9A4;
        arr[i].pos.y += arr[i].unk10 * D_800BE9A4;
        arr[i].pos.z += arr[i].unk14 * D_800BE9A4;
        arr[i].unk1F = 255;
        life = arr[i].unk1E;
        if (life > 0) {
            arr[i].unk1E = life - D_800BE9E4;
        } else {
            arr[i].unk1C -= desc->unk52 * D_800BE9E4;
        }
        if (arr[i].unk1C < desc->unk6A) {
            if (arr[i].unk21 == 0) {
                func_15198110(arg0, i);
            }
        }
        arr[i].unk18 += desc->unk58 * D_800BE9A4;
        if (arr[i].unk1C < 0) {
            desc->unk6 &= ~2;
            while (i != arg0->unk2D) {
                arg0->unk2D = arg0->unk2D + 1;
                if (arg0->unk2D == arg0->unk25) {
                    arg0->unk2D = 0;
                }
                arg0->unk2C = arg0->unk2C - 1;
            }
            arr[arg0->unk2D].unk1C = 0;
        }
    }
    i = arg0->unk2D;
    descFlags = desc->unk6;
    flags = descFlags;
    if (flags & 2) {
        flags = 0;
        n = desc->unk8C;
        do {
            arr[i].unk1F = flags; i++; flags += desc->unk8D; if (i == arg0->unk25) { i = 0; } n--;
        } while (n != 0 && i != arg0->unk2E);
        flags = desc->unk6;
    }
    if (flags & 4) {
        scale = 0;
        n = desc->unk8E;
        i = arg0->unk2E - 1;
        if (i < 0) {
            i = arg0->unk25 - 1;
        }
        do {
            arr[i].unk1F = (arr[i].unk1F * scale) >> 8; i--; scale += desc->unk8F; if (i < 0) { i = arg0->unk25 - 1; } n--;
        } while (n != 0 && i != arg0->unk2E);
    }
    if (desc->unk64 != -1) {
        D_8008F894[desc->unk64](arg0);
    }
    if (desc->unk68 != -1) {
        D_8008F89C[desc->unk68](arg0);
    }
    if (desc->unk61 != -1) {
        if (D_8008F880[desc->unk61](arg0) == 0) {
            return 0;
        }
    }
    if (arg0->unk2C > 0) {
        arg0->unk54 = arr[arg0->unk2D].pos;
    } else {
        arg0->unk54.x = 0.0f;
        arg0->unk54.y = 0.0f;
        arg0->unk54.z = 0.0f;
    }
    return 1;
}

typedef struct {
    /* 0x00 */ Vec3f15196438 pos;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 pad22[0x2];
} Particle15196B4C;

typedef struct {
    /* 0x00 */ u8 *unk0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ s8 unk6;
    /* 0x07 */ s8 unk7;
    /* 0x08 */ Vec15196438 unk8;
    /* 0x14 */ Vec15196438 unk14;
    /* 0x20 */ Vec3f15196438 unk20;
    /* 0x2C */ Vec3f15196438 unk2C;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ f32 unk5C;
    /* 0x60 */ u8 pad60[0x78 - 0x60];
    /* 0x78 */ f32 unk78;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ u8 unk80;
    /* 0x84 */ f32 unk84;
    /* 0x88 */ f32 unk88;
} Desc15196B4C;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Vec3f15196438 pos;
    /* 0x1C */ u8 pad1C[0x25 - 0x1C];
    /* 0x25 */ u8 unk25;
    /* 0x26 */ u8 pad26[0x2C - 0x26];
    /* 0x2C */ s8 unk2C;
    /* 0x2D */ s8 unk2D;
    /* 0x2E */ s8 unk2E;
    /* 0x2F */ u8 pad2F[0x94 - 0x2F];
    /* 0x94 */ Particle15196B4C *unk94;
    /* 0x98 */ Desc15196B4C *unk98;
} Emitter15196B4C;

void func_15198D88();
void func_151990AC();

s32 func_15196B4C(Emitter15196B4C *arg0, s32 arg1)
{
  Desc15196B4C *desc;
  Particle15196B4C *arr;
  Vec3f15196438 pos;
  s32 base;
  s32 mtx;
  f32 inv;
  f32 tstep;
  f32 dz;
  f32 dy;
  f32 dx;
  f32 dz2;
  f32 dy2;
  f32 dx2;
  f32 dscale;
  f32 dalpha;
  Vec3f15196438 cur;
  Vec3f15196438 cur2;
  f32 t;
  f32 alpha;
  f32 scale;
  f32 fq;
  s32 head;
  s32 tail;
  f32 pad4;
  f32 pad5;
  f32 pad6;
  f32 pad7;
  desc = arg0->unk98;
  arr = arg0->unk94;
  if ((*((s32 *) (desc->unk0 + 0x1D4))) == 0)
  {
    return 0;
  }
  if (desc->unk7 != (-1))
  {
    desc->unk44 = D_8008F870[desc->unk7](arg0, arg1);
  }
  mtx = (*((s32 *) (desc->unk0 + 0x1D4))) + (desc->unk5 << 6);
  func_15143134(&desc->unk8, &arg0->pos, mtx);
  func_15143134(&desc->unk14, &pos, mtx);
  desc->unk4C += desc->unk48 * D_800BE9A4;
  if (desc->unk4C > 1.0f)
  {
    do
    {
      inv = 1.0f / desc->unk4C;
      t = desc->unk3C + D_800BE9A4;
      fq = t;
      cur = desc->unk20;
      cur2 = desc->unk2C;
      tstep = t * inv;
      scale = desc->unk5C + (desc->unk58 * t);
      alpha = desc->unk38;
      dx = arg0->pos.x - desc->unk20.x;
      dy = arg0->pos.y - desc->unk20.y;
      dz = arg0->pos.z - desc->unk20.z;
      dx2 = pos.x - desc->unk2C.x;
      dy2 = pos.y - desc->unk2C.y;
      dz2 = pos.z - desc->unk2C.z;
      dscale = desc->unk5C - scale;
      dalpha = desc->unk44 - alpha;
    }
    while (0);
    t = fq;
    do
    {
      desc->unk7C += desc->unk78;
      if (desc->unk7C > 1.0f)
      {
        func_15198D88(arg0, &pos);
      }
      desc->unk88 += desc->unk84;
      if (desc->unk88 > 1.0f)
      {
        func_151990AC(arg0, &pos);
      }
      arr[arg0->unk2E].unk21 = 0;
      arr[arg0->unk2E].pos.x = cur.x;
      arr[arg0->unk2E].pos.y = cur.y;
      arr[arg0->unk2E].pos.z = cur.z;
      arr[arg0->unk2E].unkC = (cur2.x - cur.x) * alpha;
      arr[arg0->unk2E].unk10 = (cur2.y - cur.y) * alpha;
      arr[arg0->unk2E].unk14 = (cur2.z - cur.z) * alpha;
      arr[arg0->unk2E].unk18 = scale;
      arr[arg0->unk2E].unk1C = desc->unk53;
      arr[arg0->unk2E].unk1E = desc->unk54;
      arr[arg0->unk2E].unk1F = 0xFF;
      arr[arg0->unk2E].unk20 = desc->unk50;
      desc->unk50 += (func_150ADA20() % 6) + 3;
      arr[arg0->unk2E].unk10 += desc->unk40 * t;
      arr[arg0->unk2E].pos.x += arr[arg0->unk2E].unkC * t;
      arr[arg0->unk2E].pos.y += arr[arg0->unk2E].unk10 * t;
      arr[arg0->unk2E].pos.z += arr[arg0->unk2E].unk14 * t;
      t -= tstep;
      head = ++arg0->unk2E;
      if ((head & 0xFFFFFFFF) == arg0->unk25)
      {
        head = (arg0->unk2E = 0);
      }
      arg0->unk2C++;
      tail = arg0->unk2D;
      if (tail == head)
      {
        arg0->unk2D = tail + 1;
        if (arg0->unk2D == arg0->unk25)
        {
          arg0->unk2D = 0;
        }
        arg0->unk2C--;
      }
      cur.x += dx * inv;
      cur.y += dy * inv;
      cur.z += dz * inv;
      cur2.x += dx2 * inv;
      cur2.y += dy2 * inv;
      cur2.z += dz2 * inv;
      fq = dscale * inv;
      alpha += dalpha * inv;
      scale += fq;
      desc->unk4C -= 1.0f;
    }
    while (desc->unk4C > 1.0f);
    desc->unk20 = cur;
    desc->unk2C = cur2;
    desc->unk38 = alpha;
    desc->unk3C = t;
  }
  return 1;
}

typedef struct Vec3f_15197C10 {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_15197C10;

typedef struct CharacterFlamethrowerBeamCamera {
    u8 unknown0[0x2F8];
    Vec3f_15197C10 position;
    u8 unknown304[0x69C];
} CharacterFlamethrowerBeamCamera;

void func_151D5D60(void *, s16, s32, void **, u8 *);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern CharacterFlamethrowerBeamCamera *D_800DBFF0;

typedef struct {
    Vec3f_15197C10 pos;
    u8 pad0C[0xC];
    f32 size;
    s16 life;
    u8 pad1E;
    u8 alpha;
    u8 level;
    u8 pad21[3];
} Point15197148;

typedef struct {
    u8 pad0[0x51];
    u8 color;
    u8 pad52[0xA];
    f32 width;
} Style15197148;

typedef struct {
    u8 pad0[0x10];
    Vec3f_15197C10 pos;
    u8 pad1C[2];
    u16 flags1E;
    u8 pad20[5];
    u8 capacity;
    u8 pad26[6];
    s8 count;
    s8 tail;
    s8 head;
    u8 pad2F[0x84 - 0x2F];
    u8 buffers[0x10];
    Point15197148 *points;
    Style15197148 *style;
} Owner15197148;

typedef struct {
    s32 id;
    u16 width;
    u16 height;
    u8 unk8;
    u8 unk9;
    u8 fmt;
} TexSrc15197148;

typedef struct {
    s32 id;
    s32 *idp;
    u8 fmt;
    u16 width;
    u16 height;
    u8 unkE;
    u8 unkF;
} TexDesc15197148;

extern TexSrc15197148 D_800915B0;
extern s32 D_800D2C9C;
s32 func_15142E24(s32, s32, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);

Gfx *func_15197148(Owner15197148 *arg0, Gfx *gfx, s16 arg2) {
    Vtx_t *vtx;
    Vec3f_15197C10 *eye;
    Vec3f_15197C10 next;
    Vec3f_15197C10 cur;
    Point15197148 *points;
    Style15197148 *style;
    s32 i;
    s32 j;
    Vec3f_15197C10 toCam;
    Vec3f_15197C10 delta;
    Vec3f_15197C10 n;
    f32 len;
    u8 prev;
    u8 level;
    u8 flag;
    u8 alpha;
    f32 width;
    s32 old;

    if (arg0->count >= 2) {
        func_151D5D60(arg0->buffers, arg2, (arg0->capacity * 2 + 10) * sizeof(Vtx), (void **)&vtx, NULL);
        if (vtx != NULL) {
            eye = &D_800DBFF0[arg2].position;
            style = arg0->style;
            points = arg0->points;
            flag = 1;
            {
                TexDesc15197148 tex;

                tex.id = D_800915B0.id;
                tex.idp = &tex.id;
                tex.fmt = D_800915B0.fmt;
                tex.width = D_800915B0.width;
                tex.height = D_800915B0.height;
                tex.unkE = D_800915B0.unk8;
                tex.unkF = D_800915B0.unk9;
                gfx = (Gfx *)func_15142E24((s32)gfx, (s32)&tex.idp, 0, 0, 0x100, 0x100, 0, 4, 0, &flag, 3);
                gfx = func_15142C10(gfx, style->color, style->color, style->color, 0xFF, &flag);
                gfx = func_15142B7C(gfx, 1, 0x160600);
                gfx = func_1513F4E4(gfx, 0xB, &flag);
                gfx = func_15142FBC(gfx, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, 0x5049D8, &flag);
                if (arg0->flags1E & 2) {
                    j = arg0->head - 1;
                    if (j < 0) {
                        j = arg0->capacity - 1;
                    }
                    cur = arg0->pos;
                    next = points[j].pos;
                    level = points[j].level;
                    prev = points[j].level;
                    width = style->width;
                } else {
                    i = arg0->head - 1;
                    if (i < 0) {
                        i = arg0->capacity - 1;
                    }
                    j = i - 1;
                    if (j < 0) {
                        j = arg0->capacity - 1;
                    }
                    cur = points[i].pos;
                    next = points[j].pos;
                    prev = points[i].level;
                    level = points[j].level;
                    width = points[j].size;
                }
                delta.x = cur.x - next.x;
                delta.y = cur.y - next.y;
                delta.z = cur.z - next.z;
                toCam.x = cur.x - eye->x;
                toCam.y = cur.y - eye->y;
                toCam.z = cur.z - eye->z;
                n.x = delta.y * toCam.z - toCam.y * delta.z;
                n.y = delta.z * toCam.x - toCam.z * delta.x;
                n.z = delta.x * toCam.y - toCam.x * delta.y;
                len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
                if (len == 0.0f) {
                    n.x = 0.0f;
                    n.y = 0.0f;
                    n.z = 0.0f;
                } else {
                    f32 scale;

                    scale = width / len;
                    n.x *= scale;
                    n.y *= scale;
                    n.z *= scale;
                }
                alpha = (points[j].life * points[j].alpha) >> 8;
                vtx->ob[0] = cur.x + n.x;
                vtx->ob[1] = cur.y + n.y;
                vtx->ob[2] = cur.z + n.z;
                vtx->tc[0] = (prev + 0x100) << 6;
                vtx->tc[1] = 0x47C0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = alpha;
                vtx->flag = 0;
                vtx++;
                vtx->ob[0] = cur.x - n.x;
                vtx->ob[1] = cur.y - n.y;
                vtx->ob[2] = cur.z - n.z;
                vtx->tc[0] = (prev + 0x100) << 6;
                vtx->tc[1] = 0x4000;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = alpha;
                vtx->flag = 0;
                vtx++;
                do {
                    delta.x = cur.x - next.x;
                    delta.y = cur.y - next.y;
                    delta.z = cur.z - next.z;
                    toCam.x = next.x - eye->x;
                    toCam.y = next.y - eye->y;
                    toCam.z = next.z - eye->z;
                    n.x = delta.y * toCam.z - toCam.y * delta.z;
                    n.y = delta.z * toCam.x - toCam.z * delta.x;
                    n.z = delta.x * toCam.y - toCam.x * delta.y;
                    len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
                    if (len == 0.0f) {
                        n.x = 0.0f;
                    n.y = 0.0f;
                    n.z = 0.0f;
                    } else {
                        f32 scale;

                        scale = points[j].size / len;
                        n.x *= scale;
                        n.y *= scale;
                        n.z *= scale;
                    }
                    alpha = (points[j].life * points[j].alpha) >> 8;
                    vtx->ob[0] = next.x + n.x;
                    vtx->ob[1] = next.y + n.y;
                    vtx->ob[2] = next.z + n.z;
                    vtx->tc[0] = (level + 0x100) << 6;
                    vtx->tc[1] = 0x47C0;
                    vtx->cn[0] = 0xFF;
                    vtx->cn[1] = 0xFF;
                    vtx->cn[2] = 0xFF;
                    vtx->cn[3] = alpha;
                    vtx->flag = 0;
                    vtx++;
                    vtx->ob[0] = next.x - n.x;
                    vtx->ob[1] = next.y - n.y;
                    vtx->ob[2] = next.z - n.z;
                    vtx->tc[0] = (level + 0x100) << 6;
                    vtx->tc[1] = 0x4000;
                    vtx->cn[0] = 0xFF;
                    vtx->cn[1] = 0xFF;
                    vtx->cn[2] = 0xFF;
                    vtx->cn[3] = alpha;
                    vtx->flag = 0;
                    vtx++;
                    gSPVertex(gfx++, vtx - 4, 4, 0);
                    gSP1Triangle(gfx++, 0, 1, 2, 0);
                    gSP1Triangle(gfx++, 1, 3, 2, 0);
                    old = prev < level;
                    prev = level;
                    if (old) {
                        memcpy(vtx, vtx - 2, 0x20);
                        vtx[-2].tc[0] -= 0x4000;
                        vtx++;
                        vtx[-2].tc[0] -= 0x4000;
                        vtx++;
                    }
                    i = j;
                    j--;
                    if (j < 0) {
                        j = arg0->capacity - 1;
                    }
                    cur = points[i].pos;
                    next = points[j].pos;
                    level = points[j].level;
                } while (i != arg0->tail);
            }
        }
    }
    return gfx;
}


extern f32 D_800A8AA4;

f32 func_151979F8(s32 arg0) {
    return D_800A8AA4;
}

f32 func_15197A0C(s32 arg0) {
    return (f32)func_151422C0(0xA, (s32) D_800A8A40, 1, 0x1F4, (s32) D_800A8A48, 0x8CC) * D_800A8AA8;
}

extern f32 D_800A8AAC;

f32 func_15197A68(s32 arg0) {
    return D_800A8AAC;
}

struct ConkerStruct15197A7C {
    s32 unk0;
    u8 unk4;
};

extern void func_15147D64(struct ConkerStruct15197A7C *, s32);

void func_15197A7C(u8 *arg0) {
    struct ConkerStruct15197A7C sp18;

    if (arg0 != 0) {
        sp18.unk0 = (s32) arg0;
        sp18.unk4 = arg0[0x3B];
        func_15147D64(&sp18, 8);
    }
}

s32 func_15197AB4(f32 *arg0)
{
  int dummy;
  arg0[0x50] = arg0[0x57] * arg0[0x50];
  arg0[0x51] += arg0[0x56] * D_800BE9A4;
  dummy = (arg0 != 0);
  arg0[0x52] = arg0[0x57] * arg0[0x52];
  arg0[0x53] = arg0[0x58] * arg0[0x53];
  arg0[0x54] += arg0[0x56] * D_800BE9A4;
  arg0[0x55] = arg0[0x58] * arg0[0x55];
  arg0[0xD] += arg0[0x50] * D_800BE9A4;
  arg0[0xE] += arg0[0x51] * D_800BE9A4;
  arg0[0xF] += arg0[0x52] * D_800BE9A4;
  arg0[0x10] += arg0[0x53] * D_800BE9A4;
  arg0[0x11] += arg0[0x54] * D_800BE9A4;
  arg0[0x12] += arg0[0x55] * D_800BE9A4;
  return 1;
}

typedef struct {
    char pad0[0x1C];
    s16 unk1C;
    char pad1E[0x2C - 0x1E];
    f32 unk2C;
    f32 unk30;
    char pad34[0x5C - 0x34];
    u8 unk5C;
    char pad5D[0x164 - 0x5D];
    f32 unk164;
    s16 unk168;
    s16 unk16A;
} Struct15197BBC;

s32 func_15197BBC(Struct15197BBC *arg0) {
    s16 temp;

    temp = arg0->unk1C;
    arg0->unk30 = arg0->unk30 - (arg0->unk30 * arg0->unk164);
    arg0->unk2C = arg0->unk2C - (arg0->unk2C * arg0->unk164);
    if (temp < arg0->unk168) {
        arg0->unk5C = temp * arg0->unk16A;
    }
    return 1;
}

typedef struct CharacterFlamethrowerBeamVertex {
    s16 x, y, z, flag;
    u8 other[8];
} CharacterFlamethrowerBeamVertex;

typedef struct CharacterFlamethrowerBeam {
    u8 unknown0[0x2C];
    f32 width;
    f32 progress;
    Vec3f_15197C10 start;
    Vec3f_15197C10 end;
    u8 unknown4C[0x74];
    u8 vertexTemplate[0x40];
    u8 *buffers[1];
} CharacterFlamethrowerBeam;


void *func_15197C10(CharacterFlamethrowerBeam *arg0, s16 arg1) {
    CharacterFlamethrowerBeamVertex *vertices;
    void *result;
    f32 progress;
    Vec3f_15197C10 tip;
    f32 dx, dy, dz;
    f32 vx, vy, vz;
    f32 nx, ny, nz;
    f32 lengthSquared;
    u8 fresh;
    Vec3f_15197C10 *eye;
    f32 scale;

    func_151D5D60(arg0->buffers, arg1, 0x40, (void **)&vertices, &fresh);
    arg1; /* no-op: shifts uopt state so stores via vertices[1] reload it (file-context dependent) */
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(arg0->buffers[arg1], arg0->vertexTemplate, 0x40);
            memcpy(arg0->buffers[arg1] + 0x40, arg0->vertexTemplate, 0x40);
        }
    } else {
        return NULL;
    }
    progress = arg0->progress;
    dx = arg0->end.x - arg0->start.x;
    dy = arg0->end.y - arg0->start.y;
    dz = arg0->end.z - arg0->start.z;
    tip.x = arg0->start.x + dx * progress;
    tip.y = arg0->start.y + dy * progress;
    tip.z = arg0->start.z + dz * progress;
    eye = &D_800DBFF0[arg1].position;
    dx = tip.x - arg0->start.x;
    dy = tip.y - arg0->start.y;
    dz = tip.z - arg0->start.z;
    vx = arg0->start.x - eye->x;
    vy = arg0->start.y - eye->y;
    vz = arg0->start.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = arg0->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = arg0->start.x + nx;
    vertices->y = arg0->start.y + ny;
    vertices->z = arg0->start.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = arg0->start.x - nx;
    vertices->y = arg0->start.y - ny;
    vertices->z = arg0->start.z - nz;
    vertices->flag = 0;
    vertices++;
    dx = -dx;
    dy = -dy;
    dz = -dz;
    vx = tip.x - eye->x;
    vy = tip.y - eye->y;
    vz = tip.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = arg0->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = tip.x + nx;
    vertices->y = tip.y + ny;
    vertices->z = tip.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = tip.x - nx;
    vertices->y = tip.y - ny;
    vertices->z = tip.z - nz;
    vertices->flag = 0;
    return result;
}

typedef struct {
    s32 unk0;
    u8 unk4;
} SubA_15198054;

typedef struct {
    s32 unk0;
    union {
        u8 b4;
        s32 w4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB_15198054;

void func_151993B4();

void func_15198054(void *arg0, s32 arg1, u8 arg2) {
    SubA_15198054 *temp_v1 = *(SubA_15198054 **)((u8 *)arg0 + 0x98);
    ArgB_15198054 *b = (ArgB_15198054 *)arg1;

    if (arg2 == 0 || arg2 == 2 || arg2 == 8) {
        if ((b->unk0 == temp_v1->unk0) || (b->u4.b4 == temp_v1->unk4)) {
            func_151993B4(arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v1->unk0 == b->unk0) {
            temp_v1->unk0 = b->u4.w4;
            temp_v1->unk4 = b->unk9;
        } else if (temp_v1->unk0 == b->u4.w4) {
            temp_v1->unk0 = b->unk0;
            temp_v1->unk4 = b->unk8;
        }
    }
}

typedef struct {
    /* 0x00 */ Vec15196748 pos;
    /* 0x0C */ Vec15196748 vel;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 pad22[0x2];
} Particle15198110;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ u8 pad44[0x69 - 0x44];
    /* 0x69 */ u8 unk69;
    /* 0x6A */ u8 pad6A[0x6C - 0x6A];
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ f32 unk70;
} Desc15198110;

typedef struct {
    /* 0x00 */ u8 pad0[0x25];
    /* 0x25 */ u8 unk25;
    /* 0x26 */ u8 pad26[0x2E - 0x26];
    /* 0x2E */ s8 unk2E;
    /* 0x2F */ u8 pad2F[0x94 - 0x2F];
    /* 0x94 */ Particle15198110 *unk94;
    /* 0x98 */ Desc15198110 *unk98;
} Emitter15198110;

typedef struct {
    f32 unk0;
    f32 unk4;
    s16 unk8;
    s16 unkA;
} Entry15198110;

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ Vec15196748 unk1C;
    /* 0x28 */ Vec15196748 unk28;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
} Spawn15198110;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x30 */ Vec15196748 unk30;
    /* 0x3C */ Vec15196748 unk3C;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s16 unk58;
    /* 0x5A */ s16 unk5A;
} Extra15198110;

extern Entry15198110 D_800A8710[];
extern void *func_1513D524(void *, u8, u8, u8, u8, u8, s32, u8, s32);

void func_15198110(Emitter15198110 *arg0, s32 arg1) {
    Desc15198110 *desc;
    Particle15198110 *arr;
    Particle15198110 *cur;
    Particle15198110 *next;
    Entry15198110 *entry;
    s32 i;
    void *ret;
    u8 a;
    u8 b;
    Spawn15198110 spawn;
    Extra15198110 extra;
    f32 sinA;
    f32 cosA;
    f32 sinB;
    f32 cosB;
    f32 rad;
    f32 rc;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 flag;

    desc = arg0->unk98;
    arr = arg0->unk94;
    entry = &D_800A8710[desc->unk69];
    cur = (Particle15198110 *)((u8 *)arr + arg1 * 0x24);
    desc->unk70 += desc->unk6C;
    cur->unk21 = 1;
    if ((i = arg1 + 1) == arg0->unk25) {
        i = 0;
    }
    if (desc->unk70 > 1.0f && i != arg0->unk2E) {
        s32 pad[2];

        spawn.unk8 = 0;
        extra.unk20 = 25137.0f;
        extra.unk24 = 6733.0f;
        extra.unk2C = 0;
        spawn.unkC = (extra.unk20 + extra.unk24) * 0.5f;
        extra.unk48 = desc->unk40;
        extra.unk58 = 8;
        extra.unk5A = 0x20;
        extra.unk4C = 0.8280000091f;
        extra.unk50 = 0.753000021f;
        extra.unk54 = -0.0120000001f;
        extra.unk0 = entry->unk0 * 30.09054375f;
        extra.unk4 = entry->unk0 * 18.35523224f;
        extra.unk1C = 0;
        spawn.unk14 = (extra.unk0 + extra.unk4) * 0.5f;
        extra.unk8 = spawn.unk14;
        extra.unkC = entry->unk4 * 3.961740017f;
        extra.unk10 = entry->unk4 * 2.919177055f;
        extra.unk14 = entry->unk4 * 8.116548538f;
        extra.unk1E = 0;
        spawn.unk34 = 1.0f;
        spawn.unk3C = 1.0f;
        spawn.unk40 = 0x40000001;
        spawn.unk44 = 0xFF;
        spawn.unk45 = 0xFF;
        spawn.unk18 = (extra.unkC + extra.unk10) * 0.5f;
        extra.unk18 = spawn.unk18;
        spawn.unk38 = 0.0f;
        if (func_150ADA20() & 1) {
            spawn.unk0 = 0x13;
        } else {
            spawn.unk0 = 0x14;
        }
        spawn.unk10 = 0;
        spawn.unk11 = 0;
        spawn.unk12 = 0;
        spawn.unk13 = 0x68;
        spawn.unk2 = 0x301;
        next = (Particle15198110 *)((u8 *)arr + i * 0x24);
        extra.unk30 = next->vel;
        extra.unk3C = cur->vel;
        do {
            a = func_150ADA20() & 0xFF;
            b = func_150ADA20() & 0xFF;
            sinA = func_151423D8((u8)(a - 0x40));
            cosA = func_151423D8(a);
            sinB = func_151423D8((u8)(b - 0x40));
            cosB = func_151423D8(b);
            rad = next->unk18 * 0.8000000119f;
            spawn.unk4 = (func_150ADA20() % (u32)(entry->unkA + 1)) + entry->unk8;
            rc = rad * cosB;
            dx = rc * sinA;
            dy = -rad * sinB;
            dz = rc * cosA;
            spawn.unk1C.x = next->pos.x + dx;
            spawn.unk1C.y = next->pos.y + dy;
            spawn.unk1C.z = next->pos.z + dz;
            spawn.unk28.x = cur->pos.x + dx;
            spawn.unk28.y = cur->pos.y + dy;
            spawn.unk28.z = cur->pos.z + dz;
            if (func_150ADA20() & 1) {
                flag = 1;
            } else {
                flag = 0;
            }
            ret = func_1513D524(&spawn, 0xA, 0xB, 0, 9, (u8)(flag | 2), 0x5C, 0xFF, 0);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0x110, &extra, sizeof(extra));
            }
            desc->unk70 -= 1.0f;
        } while (desc->unk70 > 1.0f);
    }
}

extern void func_1505D024(struct127 *, s32, u16, s32);
extern s32 func_1518D1C0(struct127 *, s32, s32, s32, s32, s32, s32 *);
extern s32 D_800A8A84;

s32 func_15198570(Emitter15196748 *arg0) {
    Desc15196748 *desc;
    Particle15196748 *arr;
    struct127 *obj;
    s32 i;
    Vec15196748 pos;
    f32 r;
    f32 h;
    f32 d;
    f32 dx;
    f32 dz;
    f32 dy;
    f32 p1;

    desc = arg0->unk98;
    arr = arg0->unk94;
    obj = &D_800CC2D0[0];
    i = arg0->unk2E;
    while (i != arg0->unk2D) {
        i--;
        if (i < 0) {
            i = arg0->unk25 - 1;
        }
        pos = arr[i].pos;
        r = obj->unkE4;
        h = obj->unkE6;
        dx = pos.x - obj->x_position;
        dz = pos.z - obj->z_position;
        dy = pos.y - (obj->y_position + (s16)obj->unkE8);
        dy *= (1.0f + r / h) * 0.5f;
        d = dx * dx + dy * dy + dz * dz;
        if (d < r * r) {
            func_1505D024(obj, 0x60006, obj->unk7A, ((s32)*(struct127 **)desc - (s32)D_800CC2D0) / 0x32C);
            func_1518D1C0(obj, 0xB, 0, 1, ((u8 *)arg0)[0xC], ((u8 *)arg0)[1], &D_800A8A84);
            while (i != arg0->unk2D) {
                arg0->unk2D = arg0->unk2D + 1;
                if (arg0->unk2D == arg0->unk25) {
                    arg0->unk2D = 0;
                }
                arg0->unk2C = arg0->unk2C - 1;
            }
        }
    }
    return 1;
}

s32 func_151987CC(Emitter15196748 *arg0) {
    Desc15196748 *desc;
    Particle15196748 *arr;
    struct127 *obj;
    s32 i;
    s32 j;
    Vec15196748 pos;
    f32 r;
    f32 h;
    f32 d;
    f32 dx;
    f32 dz;
    f32 dy;
    f32 p1;
    f32 p3;

    desc = arg0->unk98;
    arr = arg0->unk94;
    for (j = 0; j < 25; j++) {
        obj = &D_800CC2D0[j];
        if (obj->interaction_state == 0 || obj->unk65 != 0 ||
            obj->unique_id == (*(struct127 **)desc)->unique_id) {
            continue;
        }
        {
            i = arg0->unk2E;
            while (i != arg0->unk2D) {
                i--;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                pos = arr[i].pos;
                r = obj->unkE4;
                h = obj->unkE6;
                dx = pos.x - obj->x_position;
                dz = pos.z - obj->z_position;
                dy = pos.y - (obj->y_position + (s16)obj->unkE8);
                dy *= (1.0f + r / h) * 0.5f;
                if (dx * dx + dy * dy + dz * dz < r * r + 2500.0f) {
                    while (i != arg0->unk2D) {
                        arg0->unk2D = arg0->unk2D + 1;
                        if (arg0->unk2D == arg0->unk25) {
                            arg0->unk2D = 0;
                        }
                        arg0->unk2C = arg0->unk2C - 1;
                    }
                }
            }
        }
    }
    return 1;
}

typedef struct Node1519897C {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ struct Node1519897C *unk8;
    /* 0x0C */ u8 padC[0x91 - 0xC];
    /* 0x91 */ u8 unk91;
    /* 0x92 */ s16 unk92;
    /* 0x94 */ s32 unk94;
    /* 0x98 */ f32 unk98;
    /* 0x9C */ f32 unk9C;
    /* 0xA0 */ f32 unkA0;
    /* 0xA4 */ u8 padA4[0x4];
    /* 0xA8 */ f32 unkA8;
    /* 0xAC */ u8 padAC[0x8];
    /* 0xB4 */ f32 unkB4;
    /* 0xB8 */ f32 unkB8;
    /* 0xBC */ u8 padBC[0xC];
    /* 0xC8 */ f32 unkC8;
    /* 0xCC */ u8 padCC[0xED - 0xCC];
    /* 0xED */ u8 unkED;
} Node1519897C;

extern u8 D_800DCE50[];
void func_15183ACC(s32);
s32 func_1510F8D8(s32, s32, s32, s32);

s32 func_1519897C(Emitter15196748 *arg0) {
    Desc15196748 *desc = arg0->unk98;
    Particle15196748 *arr = arg0->unk94;
    struct127 *obj;
    void *sub;
    Node1519897C *head;
    Node1519897C *node;
    s32 i;
    s32 j;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 r;
    u8 list[3] = { 0x15, 0x16, 0x17 };
    Vec15196748 pos;

    obj = *(struct127 **)desc;
    sub = obj->unk31C;
    if (sub == NULL) {
        return 0;
    }
    if (*(u8 *)((u8 *)sub + 0x58) != 1) {
        obj->unk2F8 &= ~0x100;
        return 0;
    }
    if (&arr) {
    }
    for (i = 0; i < 3; i++) {
        head = ((Node1519897C **)D_800DCE50)[list[i]];
        if (head != NULL) {
            node = head;
            j = arg0->unk2E;
            D_800DD190++;
            while (node != NULL) {
                ((Node1519897C **)D_800DD198)[D_800DD190] = node->unk8;
                nx = node->unk98;
                ny = node->unk9C;
                nz = node->unkA0;
                while (j != arg0->unk2D) {
                    j--;
                    if (j < 0) {
                        j = arg0->unk25 - 1;
                    }
                    pos = arr[j].pos;
                    r = arr[j].unk18;
                    dx = pos.x - nx;
                    dy = pos.y - ny;
                    dz = pos.z - nz;
                    if (dx * dx + dy * dy + dz * dz < r * r + 900.0f) {
                        func_15183ACC(5);
                        func_15168B10((s32)node, 0x18);
                        node->unkED = 5;
                        node->unk94 = *(s32 *)((u8 *)D_800DDE80 + 0x6C);
                        node->unk92 = 9;
                        node->unkB8 = 40.0f;
                        node->unkA8 = 40.0f;
                        node->unkB4 = 0.0f;
                        node->unkC8 = func_1510F8D8(node->unk98, node->unk9C, node->unkA0, 0);
                        node->unk91 = 4;
                    }
                }
                node = ((Node1519897C **)D_800DD198)[D_800DD190];
            }
            D_800DD190--;
        }
    }
    return 1;
}

void func_15198C60(void) {
    func_10010F30(0x1AA, 0x7FFF, 0x40, 0, 0);
}

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    char padC[0x24 - 0xC];
} Struct15198C90Vec;

typedef struct {
    char pad_0x0[0x66];
    u16 field_0x66;
} Struct15198C90Sound;

typedef struct {
    char pad_0x0[0x2C];
    s8 field_0x2C;
    s8 field_0x2D;
    char pad_0x2E[0x94 - 0x2E];
    Struct15198C90Vec *field_0x94;
    Struct15198C90Sound *field_0x98;
} Struct15198C90;

void func_15198C90(Struct15198C90 *arg0) {
    Struct15198C90Vec *temp_v1;
    Struct15198C90Sound *temp_v2;
    u16 temp_a0;
    s32 temp_t6;
    s32 temp_t7;

    temp_v2 = arg0->field_0x98;
    temp_v1 = arg0->field_0x94;

    if (arg0->field_0x2C != 0) {
        temp_a0 = temp_v2->field_0x66;
        if (temp_a0 != 0) {
            temp_t6 = 0x1388;
            temp_t7 = 0x1F4;
            func_1000F91C(temp_a0, 0x7FFF, 0, 0, 0,
                          ((Struct15198C90Vec *)((u8 *)temp_v1 + (arg0->field_0x2D * 0x24)))->unk0,
                          ((Struct15198C90Vec *)((u8 *)temp_v1 + (arg0->field_0x2D * 0x24)))->unk4,
                          ((Struct15198C90Vec *)((u8 *)temp_v1 + (arg0->field_0x2D * 0x24)))->unk8,
                          temp_t7, temp_t6);
        }
    }
}

void func_15198D40(Struct15198C90 *arg0) {
    Struct15198C90Sound *temp;
    u16 val;

    temp = arg0->field_0x98;
    if (arg0->field_0x2C != 0) {
        val = temp->field_0x66;
        if (val != 0) {
            func_100111C8(val);
        }
    }
}

void func_15198D7C(s32 arg0) {
}

typedef struct {
    f32 x, y, z;
} Vec3f_15198D88;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} Entry15198D88;

typedef struct {
    u8 pad0[0x74];
    u8 unk74;
    u8 pad75[0x7];
    f32 unk7C;
} State15198D88;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    Vec3f_15198D88 pos;
    u8 pad1C[0x7C];
    State15198D88 *unk98;
} Obj15198D88;

typedef struct {
    Vec3f_15198D88 pos;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} Spawn15198D88;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    u8 pad14[4];
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 pad1C[4];
} Motion15198D88;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
} Style15198D88;

extern Entry15198D88 D_800A8728[];
extern s32 func_15147DA0(Spawn15198D88 *, Motion15198D88 *, s32, s32, s32, s32, s32, s32, s32, s32, s32, Style15198D88 *, s32, s32, s32);

void func_15198D88(Obj15198D88 *arg0, Vec3f_15198D88 *arg1) {
    Style15198D88 style;
    State15198D88 *state;
    Entry15198D88 *entry;
    Spawn15198D88 spawn;
    Motion15198D88 motion;
    f32 px;
    f32 py;
    f32 pz;
    f32 speed;
    u8 a;
    u8 b;
    f32 sinA;
    f32 cosA;
    f32 sinB;
    f32 cosB;
    f32 rad;

    state = arg0->unk98;
    entry = &D_800A8728[state->unk74];
    spawn.unkE = 1;
    motion.unk19 = 0xA;
    motion.unk1A = 0xFF;
    motion.unk18 = 0x28;
    motion.unk1B = 0x82;
    spawn.pos = arg0->pos;
    style.unk0 = 0;
    style.unk4 = 1;
    style.unk8 = 0x160600;
    style.unkC = 3;
    style.unk10 = 0x10;
    style.unk14 = 0x80;
    style.unk18 = 0x20;
    style.unk1C = 0;
    style.unk1D = 9;
    do {
        speed = (func_150ADA68() * entry->unk8) + entry->unk4;
        a = func_150ADA20() & 0xFF;
        b = func_150ADA20() & 0xFF;
        sinA = func_151423D8((u8)(a - 0x40));
        cosA = func_151423D8(a);
        sinB = func_151423D8((u8)(b - 0x40));
        cosB = func_151423D8(b);
        rad = entry->unk0;
        px = arg1->x + (rad * cosB * sinA);
        py = arg1->y - (rad * sinB);
        pz = arg1->z + (rad * cosB * cosA);
        spawn.unk15 = (func_150ADA20() % (u32)(entry->unk22 + 1)) + entry->unk20;
        spawn.unkC = (func_150ADA20() % (u32)(entry->unk1E + 1)) + entry->unk1C;
        motion.unk0 = (func_150ADA68() * entry->unk10) + entry->unkC;
        motion.unk10 = (func_150ADA68() * entry->unk18) + entry->unk14;
        motion.unk4 = (px - arg0->pos.x) * speed;
        motion.unk8 = (py - arg0->pos.y) * speed;
        motion.unkC = (pz - arg0->pos.z) * speed;
        func_15147DA0(&spawn, &motion, 0, 1, 0, 0, 0, 0, 0, 0, 0, &style, 0, arg0->unkC, arg0->unk1);
        state->unk7C -= 1.0f;
    } while (state->unk7C > 1.0f);
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_151990AC;

typedef struct {
    char pad0[0x10];
    f32 position[3];
    char pad1C[0x98 - 0x1C];
    void *state;
} Actor151990AC;

typedef struct {
    char pad0[0x80];
    u8 unk80;
    f32 unk84;
    f32 unk88;
} Jet151990AC;

typedef struct {
    f32 radius;
    f32 speed;
    f32 speed_range;
    s16 acceleration;
    s16 acceleration_range;
    s16 fade;
    s16 fade_range;
    s16 size;
    s16 size_range;
} EmissionPreset151990AC;

typedef struct {
    void *texture;
    char pad4[8];
    s16 frame;
    s16 frame_rate;
    s16 position[3];
    s16 velocity_x;
    s16 velocity_z;
    char pad1A[2];
    u8 fraction_x;
    u8 fraction_y;
    u8 fraction_z;
    s8 callback;
    s16 velocity_y;
    s16 acceleration;
    s16 width;
    s16 height;
    s16 lifetime;
    s8 fade;
    char pad2B;
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    u8 environment[3];
    char pad33;
    u16 flags;
    char pad36[2];
} Emission151990AC;

void *func_15167D84(void *, s32, s32, s8, u8, s32);
extern u8 D_80090514[];
extern EmissionPreset151990AC D_800A8770[];

void func_151990AC(Actor151990AC *arg0, Vec3f_151990AC *arg1) {
    f32 speed;
    Emission151990AC packet;
    f32 cosine1;
    f32 sine1;
    f32 cosine2;
    f32 sine2;
    f32 x;
    f32 y;
    f32 z;
    u8 angle1;
    u8 angle2;
    u32 rnd;
    EmissionPreset151990AC *preset;
    Jet151990AC *state;

    state = (Jet151990AC *)arg0->state;
    preset = &D_800A8770[state->unk80];
    packet.position[0] = arg0->position[0];
    packet.position[1] = arg0->position[1];
    packet.position[2] = arg0->position[2];
    packet.fraction_x = 0;
    packet.fraction_z = 0;
    packet.fraction_y = 0;
    packet.red = 0xFF;
    packet.green = 0xFF;
    packet.blue = 0xFF;
    packet.environment[0] = 0;
    packet.environment[1] = 0;
    packet.environment[2] = 0;
    packet.alpha = 0xFF;
    packet.callback = -1;
    packet.flags = 0x12;
    packet.frame = 0;
    packet.frame_rate = 0;
    packet.texture = D_80090514;
    packet.lifetime = 300;
    do {
        speed = func_150ADA68() * preset->speed_range + preset->speed;
        rnd = func_150ADA20();
        angle1 = rnd & 255;
        rnd = func_150ADA20();
        angle2 = rnd & 255;
        cosine1 = func_151423D8(angle1 - 64);
        sine1 = func_151423D8(angle1);
        cosine2 = func_151423D8(angle2 - 64);
        sine2 = func_151423D8(angle2);
        x = arg1->x + preset->radius * sine2 * cosine1;
        y = arg1->y - preset->radius * cosine2;
        z = arg1->z + preset->radius * sine2 * sine1;
        packet.velocity_x = (x - arg0->position[0]) * speed;
        packet.velocity_y = (y - arg0->position[1]) * speed;
        packet.velocity_z = (z - arg0->position[2]) * speed;
        packet.acceleration = func_150ADA20() % (u32)(preset->acceleration_range + 1) + preset->acceleration;
        packet.fade = func_150ADA20() % (u32)(preset->fade_range + 1) + preset->fade;
        packet.width = packet.height = func_150ADA20() % (u32)(preset->size_range + 1) + preset->size;
        func_15167D84(&packet, 0, 0, -1, *((u8 *)arg0 + 0xC), *((u8 *)arg0 + 1));
        state->unk88 -= 1.0f;
    } while (state->unk88 > 1.0f);
}

struct ConkerStruct151993B4 {
    char pad0[0x1E];
    u16 unk1E;
    char pad20[0x10];
    u8 unk30;
    char pad31[0x67];
    u8 *unk98;
};

void func_151993B4(struct ConkerStruct151993B4 *arg0) {
    u8 *p;
    p = arg0->unk98;
    arg0->unk30 = 0;
    arg0->unk1E &= 0xFFFD;
    p[6] |= 1;
    p[6] |= 4;
}

extern u8 D_800A8A9C[];

void func_151993E4(u8 *arg0) {
    u8 *q;
    u8 *p;
    s32 i;
    s32 found;

    q = *(u8 **)(arg0 + 0x98);
    p = *(u8 **)q;
    i = 0;
    found = 0;
    do {
        if (p[0x3B] == D_800A8A9C[i]) {
            found = 1;
        } else {
            i++;
        }
    } while (!found && i < 6);

    if (found) {
        ((u8 *)D_800E0900[i])[0x14] = 0;
    }
}

void func_1519944C(u8 *arg0) {
    u8 *q;
    u8 *p;
    s32 i;
    s32 found;

    q = *(u8 **)(arg0 + 0x98);
    p = *(u8 **)q;
    i = 0;
    found = 0;
    do {
        if (p[0x3B] == D_800A8A9C[i]) {
            found = 1;
        } else {
            i++;
        }
    } while (!found && i < 6);

    if (found) {
        ((u8 *)D_800E0900[i])[0x14] = 1;
    }
}

typedef struct { f32 x, y, z; } Vec3f_151994B8;
typedef struct CharacterFlamethrowerIdentity_151994B8 {
    char pad0[0x3B];
    u8 unk3B;
} CharacterFlamethrowerIdentity_151994B8;
typedef struct {
    char pad0[0x130];
    u16 unk130;
    char pad132[0x16];
    void *unk148;
} CharacterFlamethrowerState_151994B8;
typedef struct {
    char pad0[0x98];
    CharacterFlamethrowerState_151994B8 *state;
} CharacterFlamethrowerActor_151994B8;
typedef struct {
    CharacterFlamethrowerIdentity_151994B8 *owner;
    u8 generation, joint, flags, pad07;
    Vec3f_151994B8 first, second;
    u8 pad20[0x14];
    s8 initialize, field35, field36, sound;
    u8 field38, field39, size;
    u8 pad3B[0x11];
} CharacterFlamethrowerSeed;
typedef struct { s32 words[17]; } CharacterFlamethrowerRecord44;
typedef struct { s32 words[15]; } CharacterFlamethrowerRecord3C;
typedef struct {
    s32 kind;
    CharacterFlamethrowerSeed seed;
    u8 red, green, blue, alpha, flags;
    u8 pad55[3];
    CharacterFlamethrowerRecord44 first, second;
    CharacterFlamethrowerRecord3C third;
} CharacterFlamethrowerSetup;
typedef struct {
    CharacterFlamethrowerSeed seed;
    CharacterFlamethrowerRecord44 first, second;
    CharacterFlamethrowerRecord3C third;
    Vec3f_151994B8 firstPosition, secondPosition;
    f32 field128, field12C;
    u8 pad130[4];
    f32 field134, field138, field13C, size, minimum;
    void *light;
    f32 field14C;
    u8 flags, field151;
    s16 field152;
} CharacterFlamethrowerInitialState;
typedef struct {
    union {
        Vec3f_151994B8 vector;
        f32 values[3];
    } position;
    s16 lifetime;
    u16 kind;
    s32 flags;
    u8 mode, id;
    u8 pad16[6];
} CharacterFlamethrowerFullSpawn;

void func_15145EA4(s32 *, s32 *, s32, s32);
struct225 *func_1519C09C(s32 arg0, f32 *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7);
extern void (*D_8008F8A4[])(void *);
extern s16 (*D_8008F8B8[])(void);

void *func_151994B8(s32 arg0, CharacterFlamethrowerSetup *arg1, u8 arg2, s32 arg3) {
    CharacterFlamethrowerActor_151994B8 *result;
    CharacterFlamethrowerFullSpawn spawn;
    CharacterFlamethrowerInitialState initial;
    CharacterFlamethrowerIdentity_151994B8 *owner;
    Vec3f_151994B8 *inputs[2];
    Vec3f_151994B8 *outputs[2];
    CharacterFlamethrowerState_151994B8 *state;

    owner = arg1->seed.owner;
    if (*(s32 *)((u8 *)owner + 0x1D4) == 0) {
        return NULL;
    }
    spawn.id = arg1->kind;
    spawn.lifetime = 0x12C;
    spawn.kind = 0x12;
    spawn.flags = 0xE;
    spawn.mode = 0;
    initial.seed = arg1->seed;
    inputs[0] = &initial.seed.first;
    inputs[1] = &initial.seed.second;
    outputs[0] = &initial.firstPosition;
    outputs[1] = &initial.secondPosition;
    func_15145EA4((s32 *)inputs, (s32 *)outputs,
                   *(s32 *)((u8 *)owner + 0x1D4) + (initial.seed.joint << 6), 2);
    {
        s32 flag8;
        s32 flag4;
        s32 flag2;
        s32 flag32;
        s32 flags;

        spawn.position.vector = initial.firstPosition;
        initial.field128 = 0.0f;
        initial.field12C = 0.0f;
        initial.size = (f32)(u32)initial.seed.size;
        initial.minimum = -16384.0f;
        initial.first = arg1->first;
        initial.field134 = 0.0f;
        initial.second = arg1->second;
        initial.field138 = 0.0f;
        initial.third = arg1->third;
        initial.field13C = 0.0f;
        initial.field14C = 0.0f;
        flags = arg1->flags;
        flag8 = (flags & 8) ? 8 : 0;
        flag4 = (flags & 4) ? 4 : 0;
        flag2 = (flags & 2) ? 2 : 0;
        flag32 = (flags & 0x10) ? 0x20 : 0;
        initial.flags = flag32 | flag2 | flag4 | flag8;
        initial.field152 = 0;
        initial.field151 = 0;
        result = func_15147A80(&spawn, (s32)(arg0 + 0x158), 0x28,
                               0xC, 0xC, 0xC, 0, 0, 0, arg2, arg3);
        if (result != NULL) {
            state = result->state;
            memcpy(state, &initial, 0x154);
            if (initial.seed.sound != -1) {
                state->unk130 = D_8008F8B8[initial.seed.sound]();
            } else {
                state->unk130 = 0;
            }
            if (initial.seed.initialize != -1) {
                D_8008F8A4[initial.seed.initialize](result);
            }
            if (arg1->flags & 1) {
                state->unk148 = (void *)func_1519C09C((s32)result, spawn.position.values,
                                   arg1->red, arg1->green, arg1->blue, arg1->alpha,
                                   arg2, arg3);
            } else {
                state->unk148 = NULL;
            }
        }
    }
    return result;
}

void func_15199834(u8 *arg0) {
    struct ConkerStruct15197A7C sp18;

    if (arg0 != 0) {
        sp18.unk0 = (s32) arg0;
        sp18.unk4 = arg0[0x3B];
        func_15147D64(&sp18, 0x26);
    }
}

void func_1516972C();

void func_1519986C(void *arg0, s32 arg1, u8 arg2) {
    SubA_15198054 *temp_v1 = *(SubA_15198054 **)((u8 *)arg0 + 0x98);
    ArgB_15198054 *b = (ArgB_15198054 *)arg1;

    if (arg2 == 0 || arg2 == 2 || arg2 == 0x26) {
        if ((b->unk0 == temp_v1->unk0) || (b->u4.b4 == temp_v1->unk4)) {
            func_1516972C(arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v1->unk0 == b->unk0) {
            temp_v1->unk0 = b->u4.w4;
            temp_v1->unk4 = b->unk9;
        } else if (temp_v1->unk0 == b->u4.w4) {
            temp_v1->unk0 = b->unk0;
            temp_v1->unk4 = b->unk8;
        }
    }
}

void func_15199980(void *);
void func_151478F4(void *);

void func_15199928(void *arg0) {
    func_15199980(arg0);
    func_151478F4(arg0);
}

void func_15147928(void *);

void func_15199954(void *arg0) {
    func_15199980(arg0);
    func_15147928(arg0);
}

extern void (*D_8008F8C0[])(void *);
extern void (*D_8008F8B4[])(void *);

void func_15199980(void *arg0) {
    s8 *temp;
    struct102 *p;

    temp = *(s8 **)((u8 *)arg0 + 0x98);
    if (temp[0x39] != -1) {
        D_8008F8C0[temp[0x39]](arg0);
    }
    if (temp[0x36] != -1) {
        D_8008F8B4[temp[0x36]](arg0);
    }
    p = *(struct102 **)(temp + 0x148);
    if (p != 0) {
        func_1516972C(p);
    }
}

extern s32 (*D_8008F8C4[])(void *);
extern void (*D_8008F8BC[])(void *);

void func_1519BE1C(struct17 *arg0, struct17 *arg1, f32 arg2, f32 arg3);

typedef struct {
    struct17 unk0;
    struct17 unkC;
    f32 unk18;
    char pad1C[0x28 - 0x1C];
} Struct15199A10Point;

typedef struct {
    char pad0[0xE];
    s16 unkE;
    s16 unk10;
    s16 unk12;
} Struct15199A10Out;

typedef struct {
    char pad0[0x14];
    Struct15199A10Out *unk14;
} Struct15199A10Node;

typedef struct {
    char pad0[0x20];
    f32 unk20;
    char pad24[0x30 - 0x24];
    f32 unk30;
    char pad34[0x38 - 0x34];
    s8 unk38;
    char pad39[0x48 - 0x39];
    s8 unk48;
    char pad49[0x148 - 0x49];
    Struct15199A10Node *unk148;
    char pad14C[0x150 - 0x14C];
    u8 unk150;
} Struct15199A10Data;

typedef struct {
    char pad0[0x1E];
    u16 unk1E;
    char pad20[0x25 - 0x20];
    u8 unk25;
    char pad26[0x2C - 0x26];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    char pad2F[0x54 - 0x2F];
    struct17 unk54;
    char pad60[0x94 - 0x60];
    Struct15199A10Point *unk94;
    Struct15199A10Data *unk98;
} Struct15199A10;

s32 func_15199A10(Struct15199A10 *arg0) {
    Struct15199A10Data *data;
    Struct15199A10Point *pts;
    s32 i;

    data = arg0->unk98;
    pts = arg0->unk94;
    data->unk150 &= ~0x10;
    if (arg0->unk2C < 2) {
        if (arg0->unk1E & 8) {
            return 0;
        }
    }
    if (data->unk48 != -1) {
        if (D_8008F8C4[data->unk48](arg0) == 0) {
            return 0;
        }
    }
    i = arg0->unk2E;
    while (i != arg0->unk2D) {
        Struct15199A10Point *pt;

        i--;
        if (i < 0) {
            i = arg0->unk25 - 1;
        }
        pt = &pts[i];
        func_1519BE1C(&pt->unk0, &pt->unkC, data->unk20, D_800BE9A4);
        pt->unk18 += data->unk30 * D_800BE9A4;
    }
    {
        Struct15199A10Point *head;

        if (data->unk148 != 0 && arg0->unk2C >= 2) {
            Struct15199A10Node *node = data->unk148;
            Struct15199A10Out *out = node->unk14;

            head = &pts[arg0->unk2D];
            out->unkE = head->unk0.unk0;
            out->unk10 = head->unk0.unk4;
            out->unk12 = head->unk0.unk8;
        }
    }
    if (data->unk38 != -1) {
        D_8008F8BC[data->unk38](arg0);
    }
    data->unk150 &= ~0x1;
    if (arg0->unk2C > 0) {
        arg0->unk54 = pts[arg0->unk2D].unk0;
    } else {
        arg0->unk54.unk0 = 0.0f;
        arg0->unk54.unk4 = 0.0f;
        arg0->unk54.unk8 = 0.0f;
    }
    return 1;
}

typedef struct {
    struct17 unk0;
    struct17 unkC;
    f32 unk18;
    f32 unk1C;
    u8 unk20;
    char pad21[3];
    f32 unk24;
} Struct15199C34Point;

typedef struct {
    char pad0[0x1AA];
    s16 unk1AA;
} Struct15199C34Sub31C;

typedef struct {
    char pad0[0x23D];
    u8 unk23D;
} Struct15199C34Sub318;

typedef struct {
    s32 unk0;
    char pad4[0x14 - 0x4];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    char pad20[0x3B - 0x20];
    u8 unk3B;
    char pad3C[0x74 - 0x3C];
    u8 unk74;
    char pad75[0x1D4 - 0x75];
    s32 unk1D4;
    char pad1D8[0x318 - 0x1D8];
    Struct15199C34Sub318 *unk318;
    Struct15199C34Sub31C *unk31C;
} Struct15199C34Obj;

typedef struct {
    Struct15199C34Obj *unk0;
    u8 unk4;
    u8 unk5;
    char pad6[2];
    struct17 unk8;
    struct17 unk14;
    f32 unk20;
    f32 unk24;
    char pad28[4];
    f32 unk2C;
    f32 unk30;
    char pad34[0x3A - 0x34];
    u8 unk3A;
    char pad3B[0x90 - 0x3B];
    struct17 unk90;
    f32 unk9C;
    f32 unkA0;
    s32 unkA4;
    s32 unkA8;
    s16 unkAC;
    s16 unkAE;
    f32 unkB0;
    f32 unkB4;
    f32 unkB8;
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    f32 unkC8;
    s16 unkCC;
    s16 unkCE;
    s16 unkD0;
    s16 unkD2;
    struct17 unkD4;
    f32 unkE0;
    f32 unkE4;
    s16 unkE8;
    s16 unkEA;
    s16 unkEC;
    s16 unkEE;
    f32 unkF0;
    f32 unkF4;
    f32 unkF8;
    f32 unkFC;
    f32 unk100;
    f32 unk104;
    f32 unk108;
    s16 unk10C;
    s16 unk10E;
    struct17 unk110;
    struct17 unk11C;
    f32 unk128;
    f32 unk12C;
    char pad130[0x138 - 0x130];
    f32 unk138;
    f32 unk13C;
    char pad140[0x144 - 0x140];
    f32 unk144;
    char pad148[0x150 - 0x148];
    u8 unk150;
    s8 unk151;
    s16 unk152;
} Struct15199C34Data;

typedef struct {
    char pad0;
    u8 unk1;
    char pad2[0xC - 0x2];
    u8 unkC;
    char padD[0x10 - 0xD];
    struct17 unk10;
    char pad1C[0x25 - 0x1C];
    u8 unk25;
    char pad26[0x2C - 0x26];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    char pad2F[0x94 - 0x2F];
    Struct15199C34Point *unk94;
    Struct15199C34Data *unk98;
} Struct15199C34;

typedef struct {
    struct17 pos;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    s8 unk14;
    u8 unk15;
    u8 pad16[2];
} Struct15199C34Spawn;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    s16 unk1E;
} Struct15199C34Motion;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
} Struct15199C34Style;

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Struct15199C34Particle;

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    u8 pad7;
} Struct15199C34Spark;

void func_15145740(void *, void *, void *, void *, f32);
s32 func_15146078(struct17 *, struct17 *, struct17 *);
void func_15143874(s16, f32, f32 *, f32 *);
void *func_15130280(void *, u8, s32, s32, u8, s32);
void func_151D8868(void *, s32, s32, s32);

s32 func_15199C34(Struct15199C34 *arg0) {
    Struct15199C34Data *data;
    Struct15199C34Point *pts;
    Struct15199C34Obj *obj;
    struct17 pos0;
    struct17 pos1;
    struct17 pos2;
    struct17 pos3;

    data = arg0->unk98;
    pts = arg0->unk94;
    obj = data->unk0;
    if (obj->unk0 == 0 || obj->unk3B != data->unk4) {
        return 0;
    }
    if ((obj->unk74 & 0xF) == 0xF) {
        data->unk150 |= 0x10;
    } else {
        data->unk150 &= ~0x10;
    }
    if (obj->unk1D4 != 0) {
        struct17 *inputs[4];
        struct17 *outputs[4];

        inputs[0] = &data->unk8;
        inputs[1] = &data->unk14;
        inputs[2] = &data->unk90;
        inputs[3] = &data->unkD4;
        outputs[0] = &pos0;
        outputs[1] = &pos1;
        outputs[2] = &pos2;
        outputs[3] = &pos3;
        func_15145EA4((s32 *)inputs, (s32 *)outputs, obj->unk1D4 + (data->unk5 << 6), 4);
        data->unk150 &= ~1;
    } else {
        struct17 dir;
        struct17 up;

        func_15145740(obj, &dir, &up, 0, 0.0f);
        pos0.unk0 = obj->unk14;
        pos0.unk4 = obj->unk18 + 33.0f;
        pos0.unk8 = obj->unk1C;
        pos1.unk0 = dir.unk0 * 0.03200000152f + pos0.unk0;
        pos1.unk4 = dir.unk4 * 0.03200000152f + pos0.unk4;
        pos1.unk8 = dir.unk8 * 0.03200000152f + pos0.unk8;
        data->unk150 |= 1;
    }
    arg0->unk10 = pos0;
    data->unk12C += data->unk24 * D_800BE9A4;
    if (data->unk12C > 1.0f) {
        f32 inv;
        f32 t;
        f32 step;
        struct17 d0;
        struct17 d1;
        struct17 cur0;
        struct17 cur1;
        f32 ix0;
        f32 iy0;
        f32 iz0;
        f32 ix1;
        f32 iy1;
        f32 iz1;

        inv = 1.0f / data->unk12C;
        t = data->unk128 + D_800BE9A4;
        step = t * inv;
        cur0 = data->unk110;
        cur1 = data->unk11C;
        d0.unk0 = pos0.unk0 - data->unk110.unk0;
        d0.unk4 = pos0.unk4 - data->unk110.unk4;
        d0.unk8 = pos0.unk8 - data->unk110.unk8;
        d1.unk0 = pos1.unk0 - data->unk11C.unk0;
        d1.unk4 = pos1.unk4 - data->unk11C.unk4;
        d1.unk8 = pos1.unk8 - data->unk11C.unk8;
        ix0 = d0.unk0 * inv;
        iy0 = d0.unk4 * inv;
        iz0 = d0.unk8 * inv;
        ix1 = d1.unk0 * inv;
        iy1 = d1.unk4 * inv;
        iz1 = d1.unk8 * inv;
        do {
            Struct15199C34Point *p;

            p = &pts[arg0->unk2E];
            p->unk0 = cur0;
            p->unkC.unk0 = cur1.unk0 - cur0.unk0;
            p->unkC.unk4 = cur1.unk4 - cur0.unk4;
            p->unkC.unk8 = cur1.unk8 - cur0.unk8;
            p->unk18 = data->unk2C;
            p->unk1C = 0.0f;
            p->unk20 = data->unk3A;
            p->unk24 = data->unk144;
            func_1519BE1C(&p->unk0, &p->unkC, data->unk20, t);
            p->unk18 += data->unk30 * t;
            arg0->unk2E++;
            if (arg0->unk2E == arg0->unk25) {
                arg0->unk2E = 0;
            }
            arg0->unk2C++;
            if (arg0->unk2E == arg0->unk2D) {
                arg0->unk2D++;
                if (arg0->unk2D == arg0->unk25) {
                    arg0->unk2D = 0;
                }
                arg0->unk2C--;
            }
            cur0.unk0 += ix0;
            cur0.unk4 += iy0;
            cur0.unk8 += iz0;
            cur1.unk0 += ix1;
            cur1.unk4 += iy1;
            cur1.unk8 += iz1;
            t -= step;
            data->unk12C -= 1.0f;
        } while (data->unk12C > 1.0f);
        data->unk110 = cur0;
        data->unk11C = cur1;
        data->unk128 = t;
    }
    if (obj->unk1D4 != 0 && (obj->unk74 & 0xF) != 0xF) {
        data->unk138 += (data->unk9C + func_150ADA68() * data->unkA0) * D_800BE9A4;
        if (data->unk138 > 1.0f) {
            s32 pad;
            Struct15199C34Spawn spawn;
            Struct15199C34Motion motion;
            Struct15199C34Style style;
            struct17 delta;
            struct17 axisA;
            struct17 axisB;
            f32 c;
            f32 s;
            f32 speed;

            style.unk4 = 0x200005;
            style.unk8 = 0x1F0600;
            style.unkC = 3;
            style.unk10 = 0x22;
            style.unk0 = 0;
            style.unk14 = 0x80;
            style.unk18 = 0x20;
            style.unk1C = 0;
            style.unk1D = 7;
            spawn.unkE = 0x15;
            spawn.unk10 = 1;
            spawn.unk14 = -1;
            motion.unk18 = 0x68;
            motion.unk19 = 0xA;
            motion.unk1A = 0xFF;
            motion.unk14 = -10000.0f;
            motion.unk1C = data->unkD0;
            motion.unk1E = data->unkD2;
            do {
                {
                    typedef s32 T15199C34a;

                spawn.unk15 = (func_150ADA20() % (u32)(data->unkA8 + 1)) + data->unkA4;
                spawn.unkC = (func_150ADA20() % (u32)(data->unkAE + 1)) + data->unkAC;
                    motion.unk0 = func_150ADA68() * data->unkB4 + data->unkB0;
                }
                spawn.pos = pos0;
                delta = pos2;
                delta.unk0 -= spawn.pos.unk0;
                delta.unk4 -= spawn.pos.unk4;
                delta.unk8 -= spawn.pos.unk8;
                if (func_15146078(&delta, &axisA, &axisB) != 0) {
                    speed = func_150ADA68() * data->unkBC + data->unkB8;
                    func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * data->unkC0, &s, &c);
                    motion.unk4 = (axisA.unk0 * s + axisB.unk0 * c + delta.unk0) * speed;
                    motion.unk8 = (axisA.unk4 * s + axisB.unk4 * c + delta.unk4) * speed;
                    motion.unkC = (axisA.unk8 * s + axisB.unk8 * c + delta.unk8) * speed;
                    motion.unk10 = func_150ADA68() * data->unkC8 + data->unkC4;
                    motion.unk1B = func_150ADA68() * data->unkCE + data->unkCC;
                    func_15147DA0((Spawn15198D88 *)&spawn, (Motion15198D88 *)&motion, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                                  (Style15198D88 *)&style, 0, arg0->unkC, arg0->unk1);
                }
                data->unk138 -= 1.0f;
            } while (data->unk138 > 1.0f);
        }
    }
    if (obj->unk1D4 != 0 && (obj->unk74 & 0xF) != 0xF) {
        data->unk13C += (data->unkE0 + func_150ADA68() * data->unkE4) * D_800BE9A4;
        if (data->unk13C > 1.0f) {
            Struct15199C34Particle part;
            struct17 delta;
            struct17 axisA;
            struct17 axisB;
            f32 c;
            f32 s;
            f32 speed;

            part.unk1D = 0x28;
            part.unk8 = 0x2203;
            part.unk0 = 0x200005;
            part.unk4 = 0x1F0600;
            part.unkC = 0;
            part.unk10 = 0;
            part.unk14 = 0xFF;
            part.unk15 = 0xFF;
            part.unk16 = 0xFF;
            part.unk17 = 0xFF;
            part.unk18 = 0xFF;
            part.unk19 = 0xFF;
            part.unk1A = 0xFF;
            part.pos = pos0;
            part.unk3C.unk0 = 0.0f;
            part.unk3C.unk4 = 0.0f;
            part.unk3C.unk8 = 0.0f;
            part.unk1E = data->unk10C;
            part.unk20 = data->unk10E;
            part.unk22 = 1;
            part.unk24 = 1.0f;
            part.unk58 = 0xC207;
            part.unk60 = 6;
            part.unk61 = 8;
            part.unk62 = -1;
            part.unk63 = -1;
            part.unk64 = -1;
            part.unk65 = 0;
            do {
                {
                    typedef s32 T15199C34b;

                part.unkA = (func_150ADA20() % (u32)(data->unkEA + 1)) + data->unkE8;
                part.unk1B = (func_150ADA20() % (u32)(data->unkEE + 1)) + data->unkEC;
                    part.unk28 = part.unk2C = func_150ADA68() * data->unkF4 + data->unkF0;
                }
                delta = pos3;
                delta.unk0 -= part.pos.unk0;
                delta.unk4 -= part.pos.unk4;
                delta.unk8 -= part.pos.unk8;
                if (func_15146078(&delta, &axisA, &axisB) != 0) {
                    speed = func_150ADA68() * data->unkFC + data->unkF8;
                    func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * data->unk100, &s, &c);
                    part.vel.unk0 = (axisA.unk0 * s + axisB.unk0 * c + delta.unk0) * speed;
                    part.vel.unk4 = (axisA.unk4 * s + axisB.unk4 * c + delta.unk4) * speed;
                    part.vel.unk8 = (axisA.unk8 * s + axisB.unk8 * c + delta.unk8) * speed;
                    part.unk54 = func_150ADA68() * data->unk108 + data->unk104;
                    func_15130280(&part, 1, 0, 0, arg0->unkC, arg0->unk1);
                }
                data->unk13C -= 1.0f;
            } while (data->unk13C > 1.0f);
        }
    }
    data->unk151 += D_800BE9E4;
    if (data->unk151 >= 0x3D) {
        if (data->unk0 != NULL && data->unk0->unk31C != NULL) {
            data->unk0->unk31C->unk1AA++;
        }
        data->unk151 = 0;
    }
    if ((data->unk150 & 8) && obj->unk318 != NULL) {
        data->unk152 -= D_800BE9E4;
        if (data->unk152 < 0) {
            s32 pad2;
            Struct15199C34Spark spark;

            spark.unk0 = 1;
            spark.unk2 = func_150ADA20() % 21U + 0x14;
            spark.unk5 = 1 << obj->unk318->unk23D;
            spark.unk4 = func_150ADA20() % 7U + 2;
            spark.unk6 = -1;
            func_151D8868(&spark, 0, arg0->unkC, arg0->unk1);
            data->unk152 = func_150ADA20() % 21U + 0x78;
        }
    }
    return 1;
}

typedef struct {
    struct17 unk0;
    struct17 unkC;
    f32 unk18;
    f32 unk1C;
    u8 unk20;
    char pad21[3];
    f32 unk24;
} Struct1519A9A4Point;

typedef struct {
    char pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} Struct1519A9A4Obj;

typedef struct {
    Struct1519A9A4Obj *unk0;
    char pad4[0x28 - 0x4];
    f32 unk28;
    char pad2C[0x35 - 0x2C];
    s8 unk35;
    char pad36[0x3A - 0x36];
    u8 unk3A;
    char pad3B;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    char pad48[4];
    f32 unk4C;
    f32 unk50;
    s16 unk54;
    s16 unk56;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    s16 unk80;
    s16 unk82;
    f32 unk84;
    u8 unk88;
    u8 unk89;
    char pad8A[2];
    f32 unk8C;
    char pad90[0x134 - 0x90];
    f32 unk134;
    char pad138[0x140 - 0x138];
    f32 unk140;
    f32 unk144;
    char pad148[0x14C - 0x148];
    f32 unk14C;
    u8 unk150;
} Struct1519A9A4Data;

typedef struct {
    char pad0;
    u8 unk1;
    char pad2[0xC - 0x2];
    u8 unkC;
    char padD[0x10 - 0xD];
    struct17 unk10;
    char pad1C[0x1E - 0x1C];
    u16 unk1E;
    char pad20[0x25 - 0x20];
    u8 unk25;
    char pad26[0x2C - 0x26];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    char pad2F[0x94 - 0x2F];
    Struct1519A9A4Point *unk94;
    Struct1519A9A4Data *unk98;
} Struct1519A9A4;

typedef struct {
    struct17 unk0;
    struct17 unkC;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    s16 unk2A;
} Struct1519A9A4Extra;

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    struct17 unk1C;
    struct17 unk28;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
} Struct1519A9A4Spawn;

extern void (*D_8008F8A8[])(void *);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, s32, f32 *, f32 *, f32 *, f32 *, s32, s32, f32);
f32 func_15143E64(struct17 *);
f32 func_15144528(f32, f32, f32);
void func_15143794(s16, s16, f32, f32 *);

s32 func_1519A9A4(Struct1519A9A4 *arg0) {
    Struct1519A9A4Data *data;
    Struct1519A9A4Point *pts;
    f32 maxLen;

    data = arg0->unk98;
    pts = arg0->unk94;
    maxLen = data->unk28;
    if ((data->unk150 & 4) && arg0->unk2C >= 2) {
        Struct1519A9A4Obj *obj;
        struct17 *end;
        s32 i;
        struct17 hitPos;
        f32 x;
        f32 y;
        f32 z;
        f32 dx;
        f32 dy;
        f32 hit;
        f32 dz;

        obj = data->unk0;
        if (arg0->unk1E & 2) {
            end = &arg0->unk10;
        } else {
            i = arg0->unk2E;
            i--;
            if (i < 0) {
                i = arg0->unk25 - 1;
            }
            end = &pts[i].unk0;
        }
        x = obj->unk14;
        y = obj->unk18 + 48.0f;
        z = obj->unk1C;
        dx = end->unk0 - x;
        dy = end->unk4 - y;
        dz = end->unk8 - z;
        if (func_150AC9C0(x, y, z, dx, dy, dz, 0, 0, &hitPos.unk0, &hitPos.unk4,
                          &hitPos.unk8, &hit, 0, 0, 0.0f) != 0 &&
            hit < maxLen) {
            maxLen = hit;
        }
    }
    if (arg0->unk2C >= 2) {
        struct17 *prev;
        s32 i;
        struct17 diff;

        i = arg0->unk2E;
        data->unk14C = 0.0f;
        if (arg0->unk1E & 2) {
            prev = &arg0->unk10;
        } else {
            i--;
            if (i < 0) {
                i = arg0->unk25 - 1;
            }
            prev = &pts[i].unk0;
        }
        do {
            Struct1519A9A4Point *p;

            i--;
            if (i < 0) {
                i = arg0->unk25 - 1;
            }
            p = &pts[i];
            diff.unk0 = p->unk0.unk0 - prev->unk0;
            diff.unk4 = p->unk0.unk4 - prev->unk4;
            diff.unk8 = p->unk0.unk8 - prev->unk8;
            p->unk1C = func_15143E64(&diff);
            data->unk14C += p->unk1C;
            if (maxLen < data->unk14C) {
                if (p->unk1C) {
                    f32 t = (data->unk14C - maxLen) / p->unk1C;

                    p->unk0.unk0 -= diff.unk0 * t;
                    p->unk0.unk4 -= diff.unk4 * t;
                    p->unk0.unk8 -= diff.unk8 * t;
                    p->unk1C = p->unk1C * (1.0f - t);
                }
                while (i != arg0->unk2D) {
                    arg0->unk2D++;
                    if (arg0->unk2D == arg0->unk25) {
                        arg0->unk2D = 0;
                    }
                    arg0->unk2C--;
                }
                data->unk14C = maxLen;
            }
            prev = &p->unk0;
        } while (i != arg0->unk2D);
    }
    if (arg0->unk2C >= 2) {
        f32 sum;
        f32 fadeStart;
        f32 fadeLen;
        f32 inv;
        s32 i;

        sum = 0.0f;
        i = arg0->unk2E;
        fadeStart = data->unk14C * data->unk3C;
        fadeLen = data->unk14C - fadeStart;
        inv = 1.0f / fadeLen;
        do {
            Struct1519A9A4Point *p;

            i--;
            if (i < 0) {
                i = arg0->unk25 - 1;
            }
            p = &pts[i];
            sum += p->unk1C;
            if (fadeStart < sum) {
                s32 pad;

                p->unk20 = data->unk140 * ((fadeLen - (sum - fadeStart)) * inv);
            } else {
                p->unk20 = data->unk3A;
            }
        } while (i != arg0->unk2D);
    }
    if (arg0->unk2C >= 2) {
        s32 i;
        f32 sum;

        data->unk144 += data->unk40 * D_800BE9A4;
        data->unk144 = func_15144528(data->unk144, 16383.0f, -16384.0f);
        i = arg0->unk2E;
        sum = 0.0f;
        do {
            Struct1519A9A4Point *p;

            i--;
            if (i < 0) {
                i = arg0->unk25 - 1;
            }
            p = &pts[i];
            sum += p->unk1C;
            p->unk24 = func_15144528(data->unk144 + sum * data->unk44, 16383.0f, -16384.0f);
        } while (i != arg0->unk2D);
    }
    if (arg0->unk2C >= 3 && (!(data->unk150 & 0x20) || !(data->unk150 & 0x10)) && maxLen <= data->unk14C) {
        data->unk134 += (data->unk4C + func_150ADA68() * data->unk50) * pts[arg0->unk2D].unk18 * D_800BE9A4;
        if (data->unk134 > 1.0f) {
            s32 head;
            s32 next;
            s32 i;
            Struct1519A9A4Spawn spawn;
            Struct1519A9A4Extra extra;
            s32 found;
            Struct1519A9A4Point *last;
            Struct1519A9A4Point *a;
            Struct1519A9A4Point *b;
            struct17 pos;
            f32 alpha;
            struct17 dir;
            f32 sum;
            f32 target;
            void *ret;
            s32 pad4;

            head = arg0->unk2D;
            alpha = 0.0f;
            next = head + 1;
            if (next == arg0->unk25) {
                next = 0;
            }
            sum = 0.0f;
            found = 0;
            last = NULL;
            target = data->unk14C * data->unk84;
            i = arg0->unk2E;
            do {
                Struct1519A9A4Point *p;

                i--;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                p = &pts[i];
                sum += p->unk1C;
                if (target <= sum) {
                    found = 1;
                    if (last != NULL && p->unk1C) {
                        f32 t = (sum - target) / p->unk1C;

                        pos.unk0 = p->unk0.unk0 + (last->unk0.unk0 - p->unk0.unk0) * t;
                        pos.unk4 = p->unk0.unk4 + (last->unk0.unk4 - p->unk0.unk4) * t;
                        pos.unk8 = p->unk0.unk8 + (last->unk0.unk8 - p->unk0.unk8) * t;
                        alpha = p->unk18 + (last->unk18 - p->unk18) * t;
                        dir.unk0 = p->unk0.unk0 - last->unk0.unk0;
                        dir.unk4 = p->unk0.unk4 - last->unk0.unk4;
                        dir.unk8 = p->unk0.unk8 - last->unk0.unk8;
                    } else {
                        pos = p->unk0;
                        alpha = p->unk18;
                        dir.unk0 = 0.0f;
                        dir.unk4 = 0.0f;
                        dir.unk8 = 0.0f;
                    }
                }
                last = p;
            } while (i != arg0->unk2D && found == 0);
            spawn.unk8 = 0;
            spawn.unkC = 0;
            extra.unk1C = data->unk78;
            extra.unk20 = data->unk7C;
            extra.unk24 = data->unk8C;
            extra.unk28 = data->unk80;
            extra.unk2A = data->unk82;
            spawn.unk1 = 0;
            spawn.unk2 = 0x2203;
            spawn.unk10 = 0xFF;
            spawn.unk11 = 0xFF;
            spawn.unk12 = 0xFF;
            spawn.unk13 = 0xFF;
            spawn.unk34 = 1.0f;
            spawn.unk38 = 0.0f;
            spawn.unk3C = 1.0f;
            spawn.unk40 = 0x40CC05E1;
            spawn.unk45 = 0xFF;
            spawn.unk46 = 0;
            spawn.unk47 = 7;
            spawn.unk48 = 0;
            a = &pts[head];
            b = &pts[next];
            do {
                extern s32 D_800A4AA0;
                f32 r1;
                f32 r2;
                f32 r3;
                s32 r;
                struct17 vel;

                r1 = func_150ADA68() * data->unk60 + data->unk58;
                r2 = func_150ADA68() * data->unk64 + data->unk5C;
                r3 = func_150ADA68() * data->unk6C + data->unk68;
                spawn.unk14 = 24.22288895f * r1;
                spawn.unk18 = 3.440458536f * r2;
                extra.unk0.unk0 = b->unkC.unk0 * r3;
                extra.unk0.unk4 = b->unkC.unk4 * r3;
                extra.unk0.unk8 = b->unkC.unk8 * r3;
                extra.unkC.unk0 = a->unkC.unk0 * r3;
                extra.unkC.unk4 = a->unkC.unk4 * r3;
                extra.unkC.unk8 = a->unkC.unk8 * r3;
                extra.unk18 = func_150ADA68() * data->unk74 + data->unk70;
                if (func_150ADA20() & 1) {
                    spawn.unk0 = 0x13;
                } else {
                    spawn.unk0 = 0x14;
                }
                spawn.unk4 = func_150ADA20() % (data->unk56 + 1) + data->unk54;
                func_15143794((s16)(func_150ADA20() & 0xFF), (s16)((func_150ADA20() & 0x7F) - 0x3F), alpha, &vel.unk0);
                spawn.unk1C.unk0 = pos.unk0 + vel.unk0;
                spawn.unk1C.unk4 = pos.unk4 + vel.unk4;
                spawn.unk1C.unk8 = pos.unk8 + vel.unk8;
                spawn.unk28.unk0 = spawn.unk1C.unk0 + dir.unk0;
                spawn.unk28.unk4 = spawn.unk1C.unk4 + dir.unk4;
                spawn.unk28.unk8 = spawn.unk1C.unk8 + dir.unk8;
                spawn.unk44 = func_150ADA20() % (data->unk89 + 1) + data->unk88;
                ret = func_1513D2F0(&spawn, &D_800A4AA0, 0x1F, 0x22, 0, 0x1A, ((func_150ADA20() & 1) ? 1 : 0) | 2, 0, 0, 0x2C,
                                    arg0->unkC, arg0->unk1);
                if (ret != NULL) {
                    memcpy((u8 *)ret + 0x110, &extra, sizeof(extra));
                }
                data->unk134 -= 1.0f;
            } while (data->unk134 > 1.0f);
        }
    }
    if (data->unk35 != -1) {
        D_8008F8A8[data->unk35](arg0);
    }
    if ((data->unk150 & 2) && arg0->unk2C >= 2) {
        struct17 *end;
        Struct1519A9A4Point *h;
        s32 i;
        s32 pad9;

        if (arg0->unk1E & 2) {
            end = &arg0->unk10;
        } else {
            i = arg0->unk2E - 1;
            if (i < 0) {
                i = arg0->unk25 - 1;
            }
            end = &pts[i].unk0;
        }
        h = &pts[arg0->unk2D];
        func_1508B20C((h->unk0.unk0 + end->unk0) * 0.5f, (h->unk0.unk4 + end->unk4) * 0.5f,
                      (h->unk0.unk8 + end->unk8) * 0.5f, data->unk14C * 0.5f);
    }
    return 1;
}

typedef struct Vec3f_1519B4B8 {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_1519B4B8;

typedef struct FlameState_1519B4B8 {
    u8 pad0[0x2C];
    f32 width;
    u8 pad30[0xA];
    u8 alpha3A;
    u8 pad3B[0x109];
    f32 texture144;
    u8 pad148[8];
    u8 flags150;
} FlameState_1519B4B8;

typedef struct FlameActor_1519B4B8 {
    char pad0[0x10];
    f32 position[3];
    char pad1C[0x2];
    u16 flags1E;
    char pad20[0x5];
    u8 point_capacity;
    char pad26[0x6];
    s8 unk2C;
    s8 point_index;
    s8 point_end;
    char pad2F[0x65];
    void *points;
    FlameState_1519B4B8 *state;
} FlameActor_1519B4B8;

typedef struct FlamePoint_1519B4B8 {
    Vec3f_1519B4B8 position;
    Vec3f_1519B4B8 velocity;
    f32 size;
    u8 pad1C[4];
    u8 alpha;
    u8 pad21[3];
    f32 texture;
} FlamePoint_1519B4B8;

typedef struct RenderView_1519B4B8 {
    u8 pad[0x180];
} RenderView_1519B4B8;

s32 func_15144B34(s32);
s32 func_150A6360(void *, void *, f32, f32, f32, f32, f32, f32);
s32 func_15142E24(s32, s32, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
extern u8 D_800D9C10[];
extern u8 D_80091064[];
extern s32 D_800A4AC8[][2];
extern s32 D_800D2C9C;
extern RenderView_1519B4B8 *D_800BE628;

Gfx *func_1519B4B8(FlameActor_1519B4B8 *arg0, Gfx *arg1, s16 arg2) {
    FlameState_1519B4B8 *state;
    Vec3f_1519B4B8 next;
    Vec3f_1519B4B8 cur;
    s32 index;
    FlamePoint_1519B4B8 *records;
    Vec3f_1519B4B8 *camera;
    Vec3f_1519B4B8 toCamera;
    Vec3f_1519B4B8 delta;
    Vec3f_1519B4B8 normal;
    s32 previous;
    f32 lengthSquared;
    u8 flag;
    f32 nextWidth;
    f32 curWidth;
    u8 nextAlpha;
    f32 nextTexture;
    f32 curTexture;
    Vtx_t *vtx;
    u8 curAlpha;

    if (arg0->unk2C < 2) {
        return arg1;
    }
    state = arg0->state;
    records = arg0->points;
    if ((state->flags150 & 0x20) && (D_800BE616 == 0) && (state->flags150 & 0x10)) {
        return arg1;
    }
    if (state->flags150 & 1) {
        s32 tail;
        FlamePoint_1519B4B8 *first;
        FlamePoint_1519B4B8 *last;
        f32 dx;
        f32 dy;
        f32 dz;

        tail = arg0->point_end;
        tail--;
        index = arg0->point_index;
        if (tail < 0) {
            tail = arg0->point_capacity - 1;
        }
        last = &records[index];
        first = &records[tail];
        dx = first->position.x - last->position.x;
        dy = first->position.y - last->position.y;
        dz = first->position.z - last->position.z;
        if (func_150A6360(&D_800BE628[arg2], D_800D9C10 + arg2 * 0x40,
                          (last->position.x + first->position.x) * 0.5f,
                          (last->position.y + first->position.y) * 0.5f,
                          (last->position.z + first->position.z) * 0.5f,
                          sqrtf(dx * dx + dz * dz), fabsf(dy) + (state->width + state->width), 20000.0f) == 0) {
            return arg1;
        }
    }
    func_151D5D60((u8 *)arg0 + 0x84, arg2, (arg0->point_capacity * 2 + 10) * sizeof(Vtx_t), (void **)&vtx, NULL);
    if (vtx == NULL) {
        return arg1;
    }
    camera = (Vec3f_1519B4B8 *)func_15144B34(arg2);
    flag = 1;
    arg1 = (Gfx *)func_15142E24((s32)arg1, (s32)D_80091064, 0, 0, 0, 0, 0x6B, 0, 0, &flag, 3);
    arg1 = func_15142B7C(arg1, 0x200005, 0x1F0600);
    arg1 = func_1513F4E4(arg1, 0x4C, &flag);
    arg1 = func_15142FBC(arg1, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[3][0] | D_800A4AC8[3][1], &flag);
    if (arg0->flags1E & 2) {
        index = arg0->point_end;
        index--;
        if (index < 0) {
            index = arg0->point_capacity - 1;
        }
        cur = *(Vec3f_1519B4B8 *)arg0->position;
        curWidth = state->width;
        curAlpha = state->alpha3A;
        curTexture = state->texture144;
    } else {
        previous = arg0->point_end;
        previous--;
        if (previous < 0) {
            previous = arg0->point_capacity - 1;
        }
        index = previous;
        index--;
        if (index < 0) {
            index = arg0->point_capacity - 1;
        }
        cur = records[previous].position;
        curWidth = records[previous].size;
        curAlpha = records[previous].alpha;
        curTexture = records[previous].texture;
    }
    next = records[index].position;
    nextWidth = records[index].size;
    nextAlpha = records[index].alpha;
    nextTexture = records[index].texture;
    delta.x = cur.x - next.x;
    delta.y = cur.y - next.y;
    delta.z = cur.z - next.z;
    toCamera.x = cur.x - camera->x;
    toCamera.y = cur.y - camera->y;
    toCamera.z = cur.z - camera->z;
    normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
    normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
    normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
    lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
    if (lengthSquared != 0.0f) {
        f32 scale = curWidth / sqrtf(lengthSquared);

        normal.x *= scale;
        normal.y *= scale;
        normal.z *= scale;
    } else {
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
    }
    vtx->ob[0] = cur.x + normal.x;
    vtx->ob[1] = cur.y + normal.y;
    vtx->ob[2] = cur.z + normal.z;
    vtx->tc[0] = curTexture;
    vtx->tc[1] = 0x47C0;
    vtx->cn[0] = 0xFF;
    vtx->cn[1] = 0xFF;
    vtx->cn[2] = 0xFF;
    vtx->cn[3] = curAlpha;
    vtx->flag = 0;
    vtx++;
    vtx->ob[0] = cur.x - normal.x;
    vtx->ob[1] = cur.y - normal.y;
    vtx->ob[2] = cur.z - normal.z;
    vtx->tc[0] = curTexture;
    vtx->tc[1] = 0x4000;
    vtx->cn[0] = 0xFF;
    vtx->cn[1] = 0xFF;
    vtx->cn[2] = 0xFF;
    vtx->cn[3] = curAlpha;
    vtx->flag = 0;
    vtx++;
    do {
        delta.x = cur.x - next.x;
        delta.y = cur.y - next.y;
        delta.z = cur.z - next.z;
        toCamera.x = next.x - camera->x;
        toCamera.y = next.y - camera->y;
        toCamera.z = next.z - camera->z;
        normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
        normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
        normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
        lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
        if (lengthSquared != 0.0f) {
            f32 scale = nextWidth / sqrtf(lengthSquared);

            normal.x *= scale;
            normal.y *= scale;
            normal.z *= scale;
        } else {
            normal.x = 0.0f;
            normal.y = 0.0f;
            normal.z = 0.0f;
        }
        vtx->ob[0] = next.x + normal.x;
        vtx->ob[1] = next.y + normal.y;
        vtx->ob[2] = next.z + normal.z;
        vtx->tc[0] = nextTexture;
        vtx->tc[1] = 0x47C0;
        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0xFF;
        vtx->cn[2] = 0xFF;
        vtx->cn[3] = nextAlpha;
        vtx->flag = 0;
        vtx++;
        vtx->ob[0] = next.x - normal.x;
        vtx->ob[1] = next.y - normal.y;
        vtx->ob[2] = next.z - normal.z;
        vtx->tc[0] = nextTexture;
        vtx->tc[1] = 0x4000;
        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0xFF;
        vtx->cn[2] = 0xFF;
        vtx->cn[3] = nextAlpha;
        vtx->flag = 0;
        vtx++;
        gSPVertex(arg1++, vtx - 4, 4, 0);
        gSP1Triangle(arg1++, 0, 1, 2, 0);
        gSP1Triangle(arg1++, 1, 3, 2, 0);
        if (nextTexture < curTexture) {
            memcpy(vtx, vtx - 2, 0x20);
            vtx[-2].tc[0] -= 0x8000;
            vtx++;
            vtx[-2].tc[0] -= 0x8000;
            vtx++;
        }
        previous = index;
        index--;
        if (index < 0) {
            index = arg0->point_capacity - 1;
        }
        cur = records[previous].position;
        next = records[index].position;
        nextWidth = records[index].size;
        nextAlpha = records[index].alpha;
        curTexture = records[previous].texture;
        nextTexture = records[index].texture;
    } while (previous != arg0->point_index);
    return arg1;
}

void func_1519BE1C(struct17 *arg0, struct17 *arg1, f32 arg2, f32 arg3) {
    struct17 sp4;

    sp4 = *arg1;
    arg1->unk4 += arg2 * arg3;
    arg0->unk0 += sp4.unk0 * arg3;
    arg0->unk4 += (sp4.unk4 * arg3) + (0.5f * arg2 * arg3 * arg3);
    arg0->unk8 += sp4.unk8 * arg3;
}

void func_1519BEB8(u8 *arg0) {
    u8 *q;
    u8 *p;
    s32 i;
    s32 found;

    q = *(u8 **)(arg0 + 0x98);
    p = *(u8 **)q;
    i = 0;
    found = 0;
    do {
        if (p[0x3B] == D_800A8A9C[i]) {
            found = 1;
        } else {
            i++;
        }
    } while (!found && i < 6);

    if (found) {
        ((u8 *)D_800E0900[i])[0x14] = 0;
    }
}

void func_1519BF20(u8 *arg0) {
    u8 *q;
    u8 *p;
    s32 i;
    s32 found;

    q = *(u8 **)(arg0 + 0x98);
    p = *(u8 **)q;
    i = 0;
    found = 0;
    do {
        if (p[0x3B] == D_800A8A9C[i]) {
            found = 1;
        } else {
            i++;
        }
    } while (!found && i < 6);

    if (found) {
        ((u8 *)D_800E0900[i])[0x14] = 1;
    }
}

void func_1519BF8C(void) {
    func_10010F30(0x1AA, 0x7FFF, 0x40, 0, 0);
}

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    char padC[0x28 - 0xC];
} Struct1519BFBCVec;

typedef struct {
    char pad_0x0[0x130];
    u16 field_0x130;
} Struct1519BFBCSound;

typedef struct {
    char pad0[0x2C];
    s8 unk2C;
    s8 unk2D;
    char pad2E[0x94 - 0x2E];
    Struct1519BFBCVec *unk94;
    Struct1519BFBCSound *field_0x98;
} Struct1519BFBC;

void func_1519BFBC(Struct1519BFBC *arg0) {
    Struct1519BFBCVec *temp_v1;
    Struct1519BFBCSound *temp_v2;
    u16 temp_a0;
    s32 temp_t6;
    s32 temp_t7;

    temp_v2 = arg0->field_0x98;
    temp_v1 = arg0->unk94;

    if (arg0->unk2C != 0) {
        temp_a0 = temp_v2->field_0x130;
        if (temp_a0 != 0) {
            temp_t6 = 0x1388;
            temp_t7 = 0x1F4;
            func_1000F91C(temp_a0, 0x7FFF, 0, 0, 0,
                          ((Struct1519BFBCVec *)((u8 *)temp_v1 + (arg0->unk2D * 0x28)))->unk0,
                          ((Struct1519BFBCVec *)((u8 *)temp_v1 + (arg0->unk2D * 0x28)))->unk4,
                          ((Struct1519BFBCVec *)((u8 *)temp_v1 + (arg0->unk2D * 0x28)))->unk8,
                          temp_t7, temp_t6);
        }
    }
}

void func_1519C06C(Struct1519BFBC *arg0) {
    Struct1519BFBCSound *temp = arg0->field_0x98;
    u16 val = temp->field_0x130;
    if (val != 0) {
        func_100111C8(val);
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
} Struct1519C09CData;

struct225 *func_1519C09C(s32 arg0, f32 *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7) {
    struct225 *temp_v0;
    Header header;
    Header2 header2;
    s32 sp5C;
    Struct1519C09CData data;

    data.unk0 = 35.0f;
    data.unk4 = 5.0f;
    sp5C = arg0;
    header.unk0 = 2;
    header.unk1 = 2;
    header.unk2 = 0x12C;
    header.unk4 = 0x22;
    data.unk8 = 110.0f;
    data.unkC = 1.0f;
    data.unk10 = 0.0f;
    data.unk14 = 7.0f;
    data.unk18 = D_800A8AF8;
    data.unk1C = 127.0f;
    header2.unk0 = arg1[0];
    header2.unk4 = arg1[1];
    header2.unk8 = arg1[2];

    temp_v0 = func_151602C0(&header, &header2, arg2, arg3, arg4, arg5, 0xFF, 0, 0x24, arg6, arg7);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x18, &data, sizeof(data));
        memcpy((u8 *)temp_v0 + 0x38, &sp5C, sizeof(sp5C));
    }
    return temp_v0;
}

void func_1519C258(s32);
void func_151617C4(s32);

void func_1519C200(s32 arg0) {
    func_1519C258(arg0);
    func_151617C4(arg0);
}

void func_151617E4(s32);

void func_1519C22C(s32 arg0) {
    func_1519C258(arg0);
    func_151617E4(arg0);
}

void func_1519C258(s32 arg0) {
    s32 *t = *(s32 **)(*(u32 *)(arg0 + 0x38) + 0x98);

    t[0x52] = 0;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f26C;

typedef struct {
    /* 0x00 */ Vec3f26C vel;
    /* 0x0C */ Vec3f26C avel;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
} Struct1519C26CSub;

typedef struct {
    char pad0[0x34];
    /* 0x34 */ Vec3f26C pos;
    /* 0x40 */ Vec3f26C rot;
} Struct1519C26C;

s32 func_1519C26C(Struct1519C26C *arg0) {
    f32 dtSq;
    Vec3f26C vel;
    Vec3f26C avel;
    Vec3f26C acc;
    Vec3f26C aacc;
    Struct1519C26CSub *p = (Struct1519C26CSub *)((char *)arg0 + 0x110);

    dtSq = D_800BE9A4 * D_800BE9A4;
    vel = p->vel;
    avel = p->avel;
    p->vel.y += p->unk18 * D_800BE9A4;
    p->avel.y += p->unk18 * D_800BE9A4;
    {
        s32 i;

        for (i = D_800BE9E4; i > 0; i--) {
            p->vel.x *= p->unk1C;
            p->vel.y *= p->unk1C;
            p->vel.z *= p->unk1C;
            p->avel.x *= p->unk20;
            p->avel.y *= p->unk20;
            p->avel.z *= p->unk20;
        }
    }
    acc.x = (p->vel.x - vel.x) * D_800BE9A8;
    acc.y = (p->vel.y - vel.y) * D_800BE9A8;
    acc.z = (p->vel.z - vel.z) * D_800BE9A8;
    aacc.x = (p->avel.x - avel.x) * D_800BE9A8;
    aacc.y = (p->avel.y - avel.y) * D_800BE9A8;
    aacc.z = (p->avel.z - avel.z) * D_800BE9A8;
    arg0->pos.x += vel.x * D_800BE9A4 + 0.5f * acc.x * dtSq;
    arg0->pos.y += vel.y * D_800BE9A4 + 0.5f * acc.y * dtSq;
    arg0->pos.z += vel.z * D_800BE9A4 + 0.5f * acc.z * dtSq;
    arg0->rot.x += avel.x * D_800BE9A4 + 0.5f * aacc.x * dtSq;
    arg0->rot.y += avel.y * D_800BE9A4 + 0.5f * aacc.y * dtSq;
    arg0->rot.z += avel.z * D_800BE9A4 + 0.5f * aacc.z * dtSq;
    return 1;
}

typedef struct {
    char pad0[0x24];
    f32 unk24;
    s16 unk28;
    s16 unk2A;
} C4E4Sub;

typedef struct {
    char pad0[0x1C];
    s16 unk1C;
    char pad1E[0x2C - 0x1E];
    f32 unk2C;
    f32 unk30;
    char pad34[0x5C - 0x34];
    u8 unk5C;
} C4E4Struct;

s32 func_1519C4E4(C4E4Struct *arg0) {
    s32 i;
    C4E4Sub *p = (C4E4Sub *)((char *)arg0 + 0x110);

    for (i = D_800BE9E4; i > 0; i--) {
        arg0->unk2C = arg0->unk2C - arg0->unk2C * p->unk24;
        arg0->unk30 = arg0->unk30 - arg0->unk30 * p->unk24;
    }
    if (arg0->unk1C < p->unk28) {
        s32 a1 = arg0->unk1C * p->unk2A;
        if (a1 < arg0->unk5C) {
            arg0->unk5C = a1;
        }
    }
    return 1;
}

typedef struct {
    u8 pad0[0x2C];
    f32 width;
    f32 t;
    Vec3f_15197C10 start;
    Vec3f_15197C10 end;
    u8 pad4C[0x74];
    u8 tmpl[0x40];
    u8 *buffers[4];
} Obj_1519C56C;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 flag;
    u8 pad8[8];
} Vtx_1519C56C;

void func_15145548(Vec3f_15197C10 *arg0, Vec3f_15197C10 *arg1, Vec3f_15197C10 *arg2, Vec3f_15197C10 *arg3, f32 *arg4);

void *func_1519C56C(Obj_1519C56C *arg0, s16 arg1) {
    Vtx_1519C56C *vertices;
    void *result;
    Vec3f_15197C10 *pos;
    Vec3f_15197C10 dir;
    Vec3f_15197C10 mid;
    f32 dx, dy, dz;
    f32 nx, ny, nz;
    Vec3f_15197C10 closest;
    f32 t;
    f32 lengthSquared;
    u8 fresh;
    f32 scale;

    func_151D5D60(arg0->buffers, arg1, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(arg0->buffers[arg1], arg0->tmpl, 0x40);
            memcpy(arg0->buffers[arg1] + 0x40, arg0->tmpl, 0x40);
        }
    } else {
        return NULL;
    }
    pos = (Vec3f_15197C10 *)func_15144B34(arg1);
    dir.x = arg0->end.x - arg0->start.x;
    dir.y = arg0->end.y - arg0->start.y;
    dir.z = arg0->end.z - arg0->start.z;
    mid.x = arg0->start.x + dir.x * arg0->t;
    mid.y = arg0->start.y + dir.y * arg0->t;
    mid.z = arg0->start.z + dir.z * arg0->t;
    dir.x = mid.x - arg0->start.x;
    dir.y = mid.y - arg0->start.y;
    dir.z = mid.z - arg0->start.z;
    func_15145548(&arg0->start, &dir, pos, &closest, &t);
    dx = closest.x - pos->x;
    dy = closest.y - pos->y;
    dz = closest.z - pos->z;
    nx = dir.y * dz - dy * dir.z;
    ny = dir.z * dx - dz * dir.x;
    nz = dir.x * dy - dx * dir.y;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared != 0.0f) {
        scale = arg0->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    lbl: ;
    } else {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    }
    vertices->x = arg0->start.x + nx;
    vertices->y = arg0->start.y + ny;
    vertices->z = arg0->start.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = arg0->start.x - nx;
    vertices->y = arg0->start.y - ny;
    vertices->z = arg0->start.z - nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = mid.x - nx;
    vertices->y = mid.y - ny;
    vertices->z = mid.z - nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = mid.x + nx;
    vertices->y = mid.y + ny;
    vertices->z = mid.z + nz;
    vertices->flag = 0;
    return result;
}

struct ConkerStruct1519C910_inner {
    char pad0[0x84];
    u16 unk84;
};

struct ConkerStruct1519C910 {
    struct ConkerStruct1519C910_inner *unk0;
    char pad4[0x154];
    s32 unk158;
    s32 unk15C;
};

struct ConkerStruct1519C910Outer {
    char pad0[0x98];
    struct ConkerStruct1519C910 *unk98;
};

s32 func_1519C910(struct ConkerStruct1519C910Outer *arg0) {
    struct ConkerStruct1519C910 *p = arg0->unk98;
    struct ConkerStruct1519C910_inner *q = p->unk0;
    u16 v = q->unk84;

    if (v != p->unk158) {
        if (v != p->unk15C) {
            return 0;
        }
    }
    return 1;
}

s32 func_1519C948(s32 arg0) {
    func_1519CDB0(arg0, 0.25f, 0x1C);
    return 0;
}

s32 func_1519C970(s32 arg0) {
    func_1519CDB0(arg0, 0.5f, 0x1D);
    return 0;
}

s32 func_1519C998(s32 arg0) {
    func_1519CDB0(arg0, 0.2f, 0x1D);
    return 0;
}

struct Desc1519C9C4 {
    s32 unk00;
    void *unk04;
    u8 unk08;
    s8 unk09;
    u8 pad0A[2];
    f32 unk0C;
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
    s8 unk38;
    s8 unk39;
    s8 unk3A;
    s8 unk3B;
    s8 unk3C;
    s8 unk3D;
    u8 unk3E;
    u8 pad3F[1];
    f32 unk40;
    f32 unk44;
    f32 unk48;
    s8 unk4C;
    u8 pad4D[3];
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
    s8 unk54;
    u8 pad55[3];
    f32 unk58;
    f32 unk5C;
    s16 unk60;
    s16 unk62;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    s16 unk8C;
    s16 unk8E;
    f32 unk90;
    u8 unk94;
    u8 unk95;
    u8 pad96[2];
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    f32 unkAC;
    s32 unkB0;
    s32 unkB4;
    s16 unkB8;
    s16 unkBA;
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    f32 unkC8;
    f32 unkCC;
    f32 unkD0;
    f32 unkD4;
    s16 unkD8;
    s16 unkDA;
    s16 unkDC;
    s16 unkDE;
    f32 unkE0;
    f32 unkE4;
    f32 unkE8;
    f32 unkEC;
    f32 unkF0;
    s16 unkF4;
    s16 unkF6;
    s16 unkF8;
    s16 unkFA;
    f32 unkFC;
    f32 unk100;
    f32 unk104;
    f32 unk108;
    f32 unk10C;
    f32 unk110;
    f32 unk114;
    s16 unk118;
    s16 unk11A;
};


void func_1519C9C4(u8 *arg0) {
    u8 *ret;
    struct Desc1519C9C4 d;
    u8 *obj;
    s32 pad;
    u8 *sub;

    d.unk04 = obj = *(u8 **)(arg0 + 0x28);
    d.unk08 = obj[0x3B];
    d.unk09 = 9;
    d.unk0C = 7.0f;
    d.unk10 = -147.0f;
    d.unk14 = 54.0f;
    d.unk18 = 7.0f;
    d.unk1C = -228.0f;
    d.unk20 = 80.0f;
    d.unk24 = 0.8700000048f;
    d.unk28 = 0.6970000267f;
    d.unk2C = 703.0f;
    d.unk40 = 0.5600000024f;
    d.unk30 = 8.400000572f;
    d.unk34 = 6.599999905f;
    d.unk3E = 200;
    d.unk38 = -1;
    d.unk39 = 0;
    d.unk3A = -1;
    d.unk3B = 0;
    d.unk3C = 0;
    d.unk3D = 0;
    d.unk44 = -234.0f;
    d.unk48 = 9.579999924f;
    d.unk4C = 0;
    d.unk00 = 15;
    d.unk54 = 15;
    d.unk50 = 60;
    d.unk51 = 255;
    d.unk52 = 235;
    d.unk53 = 82;
    d.unk58 = 0.001949999947f;
    d.unk5C = 0.00699999975f;
    d.unk60 = 20;
    d.unk62 = 13;
    d.unk64 = 0.503000021f;
    d.unk6C = 0.5120000243f;
    d.unk68 = 0.2990000248f;
    d.unk70 = 0.3920000196f;
    d.unk74 = 0.5550000072f;
    d.unk78 = 0.7080000043f;
    d.unk7C = 0.04200000316f;
    d.unk80 = 0.3410000205f;
    d.unk84 = 0.973477006f;
    d.unk88 = 0.9759860039f;
    d.unk8C = 12;
    d.unk8E = 21;
    d.unk90 = 0.4360000193f;
    d.unk94 = 100;
    d.unk95 = 80;
    d.unk98 = 0.006000000052f;
    d.unk9C = 7.0f;
    d.unkA0 = -218.0f;
    d.unkA4 = 150.0f;
    d.unkA8 = 0.2040000111f;
    d.unkAC = 0.4030000269f;
    d.unkB0 = 4;
    d.unkB4 = 3;
    d.unkB8 = 50;
    d.unkBA = 21;
    d.unkBC = 4.0f;
    d.unkC0 = 6.0f;
    d.unkC4 = 0.1000000015f;
    d.unkC8 = 0.1000000015f;
    d.unkCC = 50.0f;
    d.unkD0 = 0.3110000193f;
    d.unkD4 = 0.3080000281f;
    d.unkD8 = 80;
    d.unkDA = 175;
    d.unkDC = 22;
    d.unkDE = 11;
    d.unkE0 = 7.0f;
    d.unkE4 = -218.0f;
    d.unkE8 = 150.0f;
    d.unkEC = 0.05000000075f;
    d.unkF0 = 0.351000011f;
    d.unkF4 = 60;
    d.unkF6 = 20;
    d.unkF8 = 155;
    d.unkFA = 100;
    d.unkFC = 134.0f;
    d.unk100 = 120.0f;
    d.unk104 = 0.05000000075f;
    d.unk108 = 0.06000000238f;
    d.unk10C = 43.0f;
    d.unk110 = -0.25f;
    d.unk114 = 0.1780000031f;
    d.unk118 = 23;
    d.unk11A = 11;
    ret = func_151994B8(8, (CharacterFlamethrowerSetup *)&d, arg0[0xC], arg0[1]);
    if (ret != NULL) {
        sub = *(u8 **)(ret + 0x98);
        memcpy(sub + 0x158, arg0 + 0x30, 8);
    }
}

struct Sub1519CD64 {
    s32 unk0;
    u8 unk4;
    u8 pad5[0x36];
    u8 unk3B;
};

struct Obj1519CD64 {
    u8 padD[0xD];
    u8 unkD;
    s16 unkE;
    u8 pad10[0x18];
    struct Sub1519CD64 *unk28;
    u8 unk2C;
};

void func_1519CD64(struct Obj1519CD64 *a0) {
    struct Sub1519CD64 *v0 = a0->unk28;

    if (v0->unk0 != 0 && v0->unk4 != 0xFF && v0->unk3B == a0->unk2C) {
        return;
    }
    a0->unkE = -1;
    a0->unkD |= 1;
}

typedef struct CFIdentity1519CDB0 {
    char pad0[0x76];
    u16 unk76;
} CFIdentity1519CDB0;

typedef struct CFState1519CDB0 {
    CFIdentity1519CDB0 *identity;
    char pad4[0x148];
    f32 spacing;
} CFState1519CDB0;

typedef struct CFActor1519CDB0 {
    char pad0[0x10];
    f32 position[3];
    char pad1C[0x9];
    u8 point_capacity;
    char pad26[0x6];
    s8 unk2C;
    s8 point_index;
    s8 point_end;
    char pad2F[0x65];
    void *points;
    CFState1519CDB0 *state;
} CFActor1519CDB0;

typedef struct CFTrailPoint1519CDB0 {
    f32 position[3];
    u8 padC[0x10];
    f32 distance;
    u8 pad20[0x8];
} CFTrailPoint1519CDB0;

s32 func_1505D1C4(f32, f32, f32, s32, s32, s32, s32, s32);

void func_1519CDB0(s32 arg0_, f32 arg1, s32 arg2) {
#define arg0 ((CFActor1519CDB0 *)arg0_)
    f32 spacing;
    f32 factor;
    f32 total;
    f32 next;
    u8 *points;
    s32 index;
    CFState1519CDB0 *state;
    CFTrailPoint1519CDB0 *point;
    CFIdentity1519CDB0 *identity;
    f32 *previous;

    state = arg0->state;
    points = arg0->points;
    previous = arg0->position;
    if (arg0->unk2C >= 2) {
        f32 interpolated_z;
        total = 0.0f;
        spacing = state->spacing * arg1;
        index = arg0->point_end;
        next = 0.0f;
        do {
            f32 distance;
            index--;
            if (index < 0) {
                index = arg0->point_capacity - 1;
            }
            point = (CFTrailPoint1519CDB0 *)(index * 0x28 + points);
            distance = point->distance;
            total += distance;
            if (next <= total) {
                identity = state->identity;
                factor = (total - next) / distance;
                func_1505D1C4(((previous[0] - point->position[0]) * factor) + point->position[0],
                    ((previous[1] - point->position[1]) * factor) + point->position[1],
                    (interpolated_z = ((previous[2] - point->position[2]) * factor) + point->position[2]), arg2,
                    ((s32)identity - (s32)D_800CC2D0) / 0x32C,
                    identity->unk76, 0, 0);
                next += spacing;
            }
            previous = point->position;
        } while (index != arg0->point_index);
    }
#undef arg0
}
