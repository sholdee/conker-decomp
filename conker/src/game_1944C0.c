#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/*
 * Object type descriptors: D_8008B4A8[type], 0x34 bytes each (struct115 in structs.h only names unk0/unk18).
 * D_8008B4D0 / D_8008B4D4 in variables.h are this table's +0x28 / +0x2C fields seen as struct115.
 */
typedef struct {
    /* 0x00 */ void (*unk0)(struct102 *);
    /* 0x04 */ void (*unk4)(struct102 *);
    /* 0x08 */ Gfx *(*draw)(Gfx *gfx, struct102 *obj, s16 arg2);
    /* 0x0C */ void (*unkC)(struct102 *);
    /* 0x10 */ Gfx *drawBeginDL;
    /* 0x14 */ Gfx *(*drawBegin)(Gfx *gfx, s32 type, s32 list, s16 arg3);
    /* 0x18 */ void (*unk18)(void);
    /* 0x1C */ void (*handleEvent)(struct102 *obj, s32 event, u8 eventType);
    /* 0x20 */ Gfx *(*drawEnd)(Gfx *gfx, s32 type, s16 arg2);
    /* 0x24 */ Gfx *drawEndDL;
    /* 0x28 */ void (*destroy)(struct102 *);
    /* 0x2C */ void (*free)(struct102 *);
    /* 0x30 */ s8 drawMode;
    /* 0x31 */ u8 pad31[3];
} ObjType;

#define OBJ_TYPE_COUNT 104
#define OBJ_TYPES ((ObjType *)D_8008B4A8)

/* Object lists: D_800DCE50[list][type] is the head of a doubly linked list of struct102 nodes. */
extern struct102 *D_800DCE50[2][OBJ_TYPE_COUNT];

/*
 * While a list is being walked, the next node is saved in D_800DD198[depth] (depth D_800DD190) so that
 * callbacks may destroy the current node; func_151696DC repairs saved entries that point at a dying node.
 */
#define OBJ_ITER_NEXT ((struct102 **)D_800DD198)

/* Link fields shared by every object (struct102 has only u8 fields at 0x4). */
typedef struct ObjLink {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 list;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ struct ObjLink *prev;
    /* 0x8 */ struct ObjLink *next;
    /* 0xC */ u8 owner;
} ObjLink;

extern void (*D_8008CA20[])(struct102 *);
extern void (*D_8008C9C8[])(struct102 *);
extern void (*D_8008CB64)(void);
extern void (*D_8008CB70)(void);

extern s16 D_800DD1C0;
extern s16 D_800DD1C2;
extern s16 D_800DD1C4;
extern s16 D_800DD1C6;

typedef struct {
    u8 pad0[0x10];
    Mtx mtx[2];
    f32 unk90;
    f32 unk94;
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    f32 unkA4;
    u8 padA8[0x18];
    f32 unkC0;
    f32 unkC4;
    f32 unkC8;
    u8 padCC[0x4];
    Gfx *display_list;
    u8 padD4[0x11];
    u8 prim_alpha;
    u8 tile_ult;
    u8 padE7[0x5];
    u8 render_mode;
    u8 combine_mode;
} Struct15168C4C;

Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

#define WGFX15168C4C(pkt, a, b)     \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

#define WGSPMATRIX15168C4C(pkt, mtx)        WGFX15168C4C(pkt, 0xDA380003, mtx)
#define WGSPDISPLAYLIST15168C4C(pkt, dl)    WGFX15168C4C(pkt, 0xDE000000, dl)

struct Some15171F04 {
    u8 pad0[4];
    u8 unk4;
    u8 pad5;
    u16 unk6;
    u16 unk8;
};

typedef struct {
    char pad_0[0x98];
    s16 field_0x98;
    s16 field_0x9A;
    char pad_0x9C[0xC];
    u16 field_0xA8;
    char pad_0xAA[0x6];
    u8 field_0xB0;
    char pad_0xB1[0x1];
    s8 field_0xB2;
} GameAnimObject;

typedef struct {
    char pad_0[0x1];
    u8 field_0x1;
    char pad_0x2[0xA];
    u8 field_0xC;
    char pad_0xD[0x3];
    u8 field_0x10;
    char pad_0x11[0x12];
    u8 field_0x23;
    char pad_0x24[0x4];
    char pad_0x28[0x20];
    u8 field_0x48;
} GameObjectHeader;

extern struct Some15171F04 *D_8008CA4C[];

void func_15169070(s32, s32, s32, u8);

/* Calls every object type's per-frame global hook (D_8008B4A8[i].unk18). */
void func_15167010(void) {
    struct115 *entry;
    struct115 *end;
    void (*func)(void);
    s32 unused;

    entry = D_8008B4A8;
    end = (struct115 *)((u32)entry + 101 * sizeof(struct115));
    unused = 0;
    // matching: a goto loop plus the dead `unused` (saved s1) give the target's base-relative end
    // and sltu exit; for/while/do forms fold `end` to a constant address
loop:
    func = (void (*)(void))entry->unk18;
    if (func != NULL) {
        func();
    }
    entry++;
    unused ^= 0;
    if ((u32)entry < (u32)end) {
        goto loop;
    }
}

void func_1516706C(void) {
    void (**funcp)(void);
    void (**end)(void);
    void (*func)(void);

    funcp = &D_8008CB64; end = &D_8008CB70;
    do {
        func = *funcp;
        if (func != NULL) {
            func();
        }
        funcp++;
    } while (funcp != end);
}
/* Runs every type's unk0 callback on each object of both lists. */
void func_151670C0(void) {
    s32 list;
    s32 type;
    struct102 *node;

    for (list = 0; list != 2; list++) {
        for (type = 0; type != 101; type++) {
            if (OBJ_TYPES[type].unk0 != NULL) {
                node = D_800DCE50[list][type];
                D_800DD190++;
                while (node != NULL) {
                    OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                    OBJ_TYPES[type].unk0(node);
                    node = OBJ_ITER_NEXT[D_800DD190];
                }
                D_800DD190--;
            }
        }
    }
}

/* Runs every type's unk4 callback on each object of both lists. */
void func_151671E8(void) {
    s32 list;
    s32 type;
    struct102 *node;

    for (list = 0; list != 2; list++) {
        for (type = 0; type != 101; type++) {
            if (OBJ_TYPES[type].unk4 != NULL) {
                node = D_800DCE50[list][type];
                D_800DD190++;
                while (node != NULL) {
                    OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                    OBJ_TYPES[type].unk4(node);
                    node = OBJ_ITER_NEXT[D_800DD190];
                }
                D_800DD190--;
            }
        }
    }
}

extern f32 D_800DD1D0[];

/* Caches sin/cos of each player's unk3A0 angle, then runs every type's unkC callback on its objects. */
void func_15167310(void) {
    s32 k;
    f32 angle;

    for (k = 0; k < D_80082FA0 + 1; k++) {
        angle = D_800DBFF0[k].unk3A0;
        D_800DD1D8[k] = sinf(angle);
        D_800DD1E8[k] = cosf(angle);
    }
    angle = D_800DBFF0->unk3A0;
    D_800DD1D0[0] = sinf(angle);
    D_800DD1D0[1] = cosf(angle);
    {
        s32 list;
        s32 type;
        struct102 *node;

        for (type = 0; type != OBJ_TYPE_COUNT; type++) {
            for (list = 0; list != 2; list++) {
                if (D_800DCE50[list][type] != NULL && OBJ_TYPES[type].unkC != NULL) {
                    node = D_800DCE50[list][type];
                    D_800DD190++;
                    while (node != NULL) {
                        OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                        OBJ_TYPES[type].unkC(node);
                        node = OBJ_ITER_NEXT[D_800DD190];
                    }
                    D_800DD190--;
                }
            }
        }
    }
}

typedef struct {
    u8 pad0[0x8];
    Gfx *(*unk8)(Gfx *, struct102 *, s16);
    u8 padC[0x4];
    Gfx *unk10;
    Gfx *(*unk14)(Gfx *, s32, s32, s16);
    u8 pad18[0x8];
    Gfx *(*unk20)(Gfx *, s32, s16);
    Gfx *unk24;
    u8 pad28[0x8];
    s8 unk30;
    u8 pad31[0x3];
} Struct151674F8;

extern Mtx D_80089470;
extern s32 D_800BE9C8[];
extern s32 D_800BEBA4;
extern u32 D_800DD1B0;
extern s32 D_800DD1B4;
extern s32 D_800DD1B8;
extern s16 D_800DD1BC;
extern s16 D_800DD1BE;
extern s16 D_800DD1C8;
extern s16 D_800DD1CA;
extern s16 D_800DD1CC;
extern s16 D_800DD1CE;
extern s16 D_800DD1F8;
extern s32 D_800DD1FC;
extern s32 D_800DD200;
extern s16 D_800DD204;
extern s16 D_800DD206;
extern s32 D_800DD208;
extern s32 D_800DD20C;
extern s32 D_800DD210;
extern s32 D_800DD214;
extern s32 D_800DD218;
extern s32 D_800DD21C;
extern s32 D_800DD220;
extern s32 D_800DD224;
extern s32 D_800DD228;
void func_15168F84(s32 arg0, s32 *arg1, s32 *arg2);

Gfx *func_151674F8(Gfx *gfx, s32 arg1, s16 arg2, s32 arg3) {
    s32 i;
    s32 start;
    s32 end;
    struct102 **slot;
    struct102 *node;
    s32 overflow;
    Gfx *orig0;
    Gfx *prev;
    Gfx *orig1;
    s32 cond;

    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    func_15168F84(arg1, &start, &end);
    for (i = start; i < end; i++) {

        if (((Struct151674F8 *)D_8008B4A8)[i].unk8 != NULL) {
            slot = &D_800DCE50[arg3][i];
            if (*slot != NULL) {
                D_8003C8E0 = (i & 0xFFFFFF) | 0x7000000;
                overflow = 0;
                if (((Struct151674F8 *)D_8008B4A8)[i].unk30 == 0) {
                    orig0 = gfx;
                }
                if (((Struct151674F8 *)D_8008B4A8)[i].unk30 == 1) {
                    orig1 = gfx;
                }
                D_800D2DAB = 0;
                D_800DD228 = 0;
                D_800DD220 = 0;
                D_800DD224 = 2;
                D_800DD1B0 = -1;
                D_800DD1B4 = -1;
                D_800DD1B8 = -1;
                D_800DD1BC = 0;
                D_800DD1BE = 0;
                D_800DD1C0 = D_800DD1C2 = D_800DD1C4 = D_800DD1C6 = -1;
                D_800DD1C8 = D_800DD1CA = D_800DD1CC = D_800DD1CE = -1;
                D_800DD1F8 = 0;
                D_800DD1FC = 0;
                D_800DD200 = 0;
                D_800DD204 = -1;
                D_800DD206 = -1;
                D_800DD208 = -1;
                D_800DD20C = -1;
                D_800DD210 = -1;
                D_800DD214 = 0;
                D_800DD218 = 0;
                D_800DD21C = 0;
                if (((Struct151674F8 *)D_8008B4A8)[i].unk10 != NULL) {
                    gSPDisplayList(gfx++, ((Struct151674F8 *)D_8008B4A8)[i].unk10);
                }
                if (((Struct151674F8 *)D_8008B4A8)[i].unk14 != NULL) {
                    gfx = ((Struct151674F8 *)D_8008B4A8)[i].unk14(gfx, i, arg3, arg2);
                }
                gSPClearGeometryMode(gfx++, G_LIGHTING);
                node = *slot;
                if (((Struct151674F8 *)D_8008B4A8)[i].unk30 == 1) {
                    D_800DD190++;
                    if (node != NULL) {
                        do {
                            prev = gfx;
                            ((struct102 **)D_800DD198)[D_800DD190] = (struct102 *)node->unk8;
                            gfx = ((Struct151674F8 *)D_8008B4A8)[i].unk8(gfx, node, arg2);
                            if ((((s32)gfx - D_800BE9C8[D_800BE9C0]) >> 3) > D_800BEBA4) {
                                cond = 1;
                            } else {
                                cond = 0;
                            }
                            if (cond) {
                                overflow = 1;
                                gfx = prev;
                            }
                            node = ((struct102 **)D_800DD198)[D_800DD190];
                        } while (node != NULL);
                    }
                    D_800DD190--;
                } else {
                    D_800DD190++;
                    if (node != NULL) {
                        do {
                            ((struct102 **)D_800DD198)[D_800DD190] = (struct102 *)node->unk8;
                            gfx = ((Struct151674F8 *)D_8008B4A8)[i].unk8(gfx, node, arg2);
                            node = ((struct102 **)D_800DD198)[D_800DD190];
                        } while (node != NULL);
                    }
                    D_800DD190--;
                }
                if (((Struct151674F8 *)D_8008B4A8)[i].unk24 != NULL) {
                    gSPDisplayList(gfx++, ((Struct151674F8 *)D_8008B4A8)[i].unk24);
                }
                if (((Struct151674F8 *)D_8008B4A8)[i].unk20 != NULL) {
                    gfx = ((Struct151674F8 *)D_8008B4A8)[i].unk20(gfx, i, arg2);
                }
                if (overflow) {
                    if ((((s32)gfx - D_800BE9C8[D_800BE9C0]) >> 3) > D_800BEBA4) {
                        cond = 1;
                    } else {
                        cond = 0;
                    }
                    if (cond) {
                        gSPSetGeometryMode(gfx, G_ZBUFFER);
                        return orig1;
                    }
                }
                if (((Struct151674F8 *)D_8008B4A8)[i].unk30 == 0) {
                    if ((((s32)gfx - D_800BE9C8[D_800BE9C0]) >> 3) > D_800BEBA4) {
                        cond = 1;
                    } else {
                        cond = 0;
                    }
                    if (cond) {
                        gSPSetGeometryMode(gfx, G_ZBUFFER);
                        return orig0;
                    }
                }
                D_8003C8E0 = 0;
            }
        }
    }
    gSPSetGeometryMode(gfx++, G_ZBUFFER);
    return gfx;
}

void func_15168A4C(ObjLink *obj, s32 type);

s32 func_15167A68(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    GameObjectHeader *temp;

    temp = (GameObjectHeader *)func_10003C6C(arg2, 1, arg3, 0, (u8)arg5);
    if (temp != 0) {
        temp->field_0x1 = arg1;
        func_15168A4C((s32 *)temp, arg0);
        temp->field_0xC = *((u8 *)&arg4 + 3);
    }
    return (s32)temp;
}

void func_15167AD8(s32 arg0, s32 arg1, s32 arg2) {
    GameObjectHeader *temp;

    temp = (GameObjectHeader *)func_15167A68(3, arg2, 0x28, 0, (u8)arg1, 1);
    if (temp != 0) {
        bcopy(arg0, &temp->field_0x10, 0x18);
        temp->field_0x23 = 0xFF;
    }
}
void func_15167B44(struct102 *arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a1;
    s32 temp_a2;

    temp_v0 = *(u8 *)((u8 *)arg0 + 0x24);
    if (temp_v0 != 0) {
        D_8008CA20[temp_v0](arg0);
    }

    temp_v1 = *(s8 *)((u8 *)arg0 + 0x22);
    if (temp_v1 > 0) {
        temp_v0 = *(u8 *)((u8 *)arg0 + 0x23);
        if (temp_v1 < temp_v0) {
            *(u8 *)((u8 *)arg0 + 0x23) = temp_v0 - temp_v1;
        } else {
            *(s16 *)((u8 *)arg0 + 0x14) = *(u8 *)(*(s32 *)((u8 *)arg0 + 0x10) + 4) << 8;
        }
    } else if (temp_v1 < 0) {
        temp_a1 = -temp_v1;
        temp_a2 = *(s16 *)((u8 *)arg0 + 0x14);
        if ((temp_a2 / 0x100) >= (*(u8 *)(*(s32 *)((u8 *)arg0 + 0x10) + 4) - 1)) {
            temp_v0 = *(u8 *)((u8 *)arg0 + 0x23);
            if (temp_a1 < temp_v0) {
                *(u8 *)((u8 *)arg0 + 0x23) = temp_v0 - temp_a1;
                *(s16 *)((u8 *)arg0 + 0x14) = temp_a2 - *(s16 *)((u8 *)arg0 + 0x16);
            }
        }
    }

    *(s16 *)((u8 *)arg0 + 0x14) += *(s16 *)((u8 *)arg0 + 0x16);
    if ((*(s16 *)((u8 *)arg0 + 0x14) / 0x100) >= *(u8 *)(*(s32 *)((u8 *)arg0 + 0x10) + 4)) {
        func_1516972C(arg0);
    }
}
// PERMUTER CANDIDATE / near-miss (best ~1090): full reconstruction below; stack frame (0x70)
// and locals sp50@0x50, sp5C@0x5C match. Blocker: the `mode` value (3/9) is MEMORY-resident at
// 0x48 in the target (spilled, double beq+bne, sw/lw 0x48(sp)); a clean `s32 mode` stays in a
// register (single branch). Forcing memory via the func_15168C4C idiom `*(s32*)((u8*)&mode - 4)`
// gets memory-residence but lands the anchor at 0x6C (slot 0x68, CSE'd base) not 0x4C/0x48 absolute;
// register renames cascade from the misaligned mode block. Needs the permuter (or exact anchor
// placement). ObjC58: unk10=s32** (arg1 of E24 + arg6=(*unk10)[unk14>>8]); unk14 s16; unk18/1A/1C/
// 1E/20 s16; unk22 s8; unk23 u8. sp5C is {s16 x,y,z,unk6,unk8; u8 unkA; u8 unkD} copied from those.
// Gfx *func_15167C58(Gfx *arg0, ObjC58 *arg1, s32 arg2) {
//     s32 mode; StructC58a sp5C; StructC58b sp50; Gfx *gfx; s32 **p = arg1->unk10;
//     sp50.unk0 = 1;
//     gfx = func_15142E24(arg0, (s32)p, arg1->unk14 << 8, 2, 0x100, 0x100,
//                         (*p)[arg1->unk14 >> 8], 5, (s32)&sp5C, (u8 *)&sp50, 3);
//     sp5C.unk0 = arg1->unk18; sp5C.unk2 = arg1->unk1A; sp5C.unk4 = arg1->unk1C;
//     sp5C.unk6 = arg1->unk1E; sp5C.unk8 = arg1->unk20; sp5C.unkA = arg1->unk23; sp5C.unkD = 0;
//     if (0xFF != arg1->unk22) { *(s32*)((u8*)&mode - 4) = 3; } else { *(s32*)((u8*)&mode - 4) = 9; }
//     gfx = func_15142FBC(gfx, 0x2C00,
//         (D_800A4AC8[*(s32*)((u8*)&mode-4)].unk4 | D_800A4AC8[*(s32*)((u8*)&mode-4)].unk0) | 4, (u8*)&sp50);
//     return (Gfx *)func_15095760(gfx, &sp5C);
// }
typedef struct {
    s32 unk0;
    s32 unk4;
} Mode15168118;
extern Mode15168118 D_800A4AC8[];
typedef struct {
    u8 pad0[0x10];
    s32 **unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s8 unk22;
    u8 unk23;
} ObjC58;
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 unkD;
} StructC58a;
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} StructC58b;
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
s32 func_15095760(Gfx *arg0, void *arg1);
Gfx *func_15167C58(Gfx *arg0, ObjC58 *arg1, u8 arg2) {
    Gfx *gfx;
    StructC58a sp5C;
    StructC58b sp50;
    s32 **tbl;
    s32 mode;
    s16 idx;

    if (arg2) {}
    sp50.unk0 = 1;
    idx = arg1->unk14;
    tbl = arg1->unk10;
    gfx = func_15142E24(arg0, (s32)tbl, idx << 8, 2, 0x100, 0x100,
                        (*tbl)[idx >> 8], 5, (s32)&sp5C, (u8 *)&sp50, 3);
    sp5C.unk0 = arg1->unk18;
    sp5C.unk2 = arg1->unk1A;
    sp5C.unk4 = arg1->unk1C;
    sp5C.unk6 = arg1->unk1E;
    sp5C.unk8 = arg1->unk20;
    sp5C.unkA = arg1->unk23;
    sp5C.unkD = 0;
    if (arg1->unk22 != 0xFF) {
        mode = 3;
    } else if (arg1->unk22 == 0xFF) {
        mode = 9;
    }
    gfx = func_15142FBC(gfx, 0x2C00, D_800A4AC8[mode].unk0 | D_800A4AC8[mode].unk4 | 4, (u8 *)&sp50);
    return (Gfx *)func_15095760(gfx, &sp5C);
}
s32 func_15167D84(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    GameObjectHeader *temp;

    temp = (GameObjectHeader *)func_15167A68((arg1 == 0) ? 5 : 0x42, arg5, arg2 + 0x50, 0, *((u8 *)&arg4 + 3), 1);
    if (temp == 0) {
        return (s32)temp;
    }
    bcopy(arg0, &temp->field_0x10, 0x38);
    temp->field_0x48 = *((s8 *)&arg3 + 3);
    return (s32)temp;
}
// PERMUTER CANDIDATE / JUSTREG (best 885): structure is byte-exact -- same instruction
// count, same addresses, same branch forms (beql/bgezl/blezl/bnel/bltzl), frame 0x20 and
// the `finished` spill at 0x1b(sp) all match. Residual = 73 register-only renames plus one
// displaced `lbu 0x2d` load. Cause: the three 24.8 accumulate steps need a named local for
// the sum (it is used twice: >>8 into the s16 and truncated into the u8), and a named local
// takes v0/v1/a1 out of IDO's round-robin temp pool; the target keeps that sum in the pool
// (t2/t8/t5) with v0/v1/a1 holding the packed value and the mflo. Dropping the local (writing
// the expression twice and letting CSE fold it) fails: the `sh 0x22` store invalidates the
// `arg0->unk30` load and IDO emits a second multu (2825). Splitting packed/product/sum into
// separate locals just moves which value owns v0/v1 (1095/1285/1495). The tail t3..t9 diff is
// a single +4 rotation of the temp counter cascading from that one choice.
// Best reconstruction (score 885):
// typedef struct {
//     char pad_0[0x10];
//     struct Some15171F04 *unk10;
//     char pad_14[0x8];
//     s16 unk1C;
//     s16 unk1E;
//     s16 unk20;
//     s16 unk22;
//     s16 unk24;
//     s16 unk26;
//     s16 unk28;
//     char pad_2A[0x2];
//     u8 unk2C;
//     u8 unk2D;
//     u8 unk2E;
//     s8 unk2F;
//     s16 unk30;
//     s16 unk32;
//     char pad_34[0x4];
//     s16 unk38;
//     s8 unk3A;
//     char pad_3B[0x4];
//     u8 unk3F;
//     char pad_40[0x4];
//     u16 unk44;
//     char pad_46[0x2];
//     s8 unk48;
// } GameSpriteObject;
//
// void func_15167E0C(GameSpriteObject *arg0) {
//     void (*func)(struct102 *);
//     u8 finished;
//     s32 limit;
//
//     finished = 0;
//     if (arg0->unk38 != 0x7FFF) {
//         arg0->unk38 -= D_800BE9E4;
//         if (arg0->unk38 < 0) {
//             arg0->unk38 = 0;
//             finished = 1;
//         }
//     }
//
//     if (arg0->unk3A > 0) {
//         if (arg0->unk3A < arg0->unk3F) {
//             arg0->unk3F -= arg0->unk3A;
//         } else {
//             arg0->unk38 = 0;
//             finished = 1;
//         }
//     }
//
//     if (arg0->unk2F != -1) {
//         func = D_8008C9C8[arg0->unk2F];
//         if (func != NULL) {
//             func((struct102 *)arg0);
//         }
//     }
//
//     if (arg0->unk38 != 0) {
//         s32 subY;
//         s32 subX;
//         s32 subZ;
//
//         arg0->unk30 += arg0->unk32;
//
//         subY = ((arg0->unk22 << 8) | arg0->unk2E) + arg0->unk30 * D_800BE9E4;
//         arg0->unk22 = subY >> 8;
//         arg0->unk2E = subY;
//
//         subX = ((arg0->unk20 << 8) | arg0->unk2C) + arg0->unk26 * D_800BE9E4;
//         arg0->unk20 = subX >> 8;
//         arg0->unk2C = subX;
//
//         subZ = ((arg0->unk24 << 8) | arg0->unk2D) + arg0->unk28 * D_800BE9E4;
//         arg0->unk24 = subZ >> 8;
//         arg0->unk2D = subZ;
//
//         if (arg0->unk1E != 0) {
//             limit = (arg0->unk10->unk4 << 8) - 1;
//             arg0->unk1C += arg0->unk1E * D_800BE9E4;
//             if (limit < arg0->unk1C) {
//                 if (arg0->unk44 & 0x20) {
//                     arg0->unk38 = 0;
//                     finished = 1;
//                 } else {
//                     arg0->unk44 |= 0x80;
//                     if (arg0->unk44 & 2) {
//                         arg0->unk1C = limit - (arg0->unk1C % limit);
//                         arg0->unk1E = -arg0->unk1E;
//                     } else {
//                         do {
//                             arg0->unk1C -= limit;
//                         } while (limit < arg0->unk1C);
//                     }
//                 }
//             } else if (arg0->unk1C < 0) {
//                 if (arg0->unk44 & 0x40) {
//                     arg0->unk38 = 0;
//                     finished = 1;
//                 } else if (arg0->unk44 & 2) {
//                     arg0->unk1C = -arg0->unk1C % limit;
//                     arg0->unk1E = -arg0->unk1E;
//                 } else {
//                     do {
//                         arg0->unk1C += limit;
//                     } while (arg0->unk1C < 0);
//                 }
//             }
//         }
//     } else {
//         finished = 1;
//     }
//
//     if (finished != 0) {
//         if (arg0->unk48 != -1) {
//             D_8008CA20[arg0->unk48]((struct102 *)arg0);
//         }
//         func_1516972C((struct102 *)arg0);
//     }
// }
typedef struct {
    char pad_0[0x10];
    struct Some15171F04 *unk10;
    char pad_14[0x8];
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    char pad_2A[0x2];
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    s8 unk2F;
    s16 unk30;
    s16 unk32;
    char pad_34[0x4];
    s16 unk38;
    s8 unk3A;
    char pad_3B[0x4];
    u8 unk3F;
    char pad_40[0x4];
    u16 unk44;
    char pad_46[0x2];
    s8 unk48;
} GameSpriteObject;

void func_15167E0C(GameSpriteObject *arg0) {
    void (*func)(struct102 *);
    u8 finished;
    s32 limit;

    finished = 0;
    if (arg0->unk38 != 0x7FFF) {
        arg0->unk38 -= D_800BE9E4;
        if (arg0->unk38 < 0) {
            arg0->unk38 = 0;
            finished = 1;
        }
    }

    if (arg0->unk3A > 0) {
        if (arg0->unk3A < arg0->unk3F) {
            arg0->unk3F -= arg0->unk3A;
        } else {
            arg0->unk38 = 0;
            finished = 1;
        }
    }

    if (arg0->unk2F != -1) {
        func = D_8008C9C8[arg0->unk2F];
        if (func != NULL) {
            func((struct102 *)arg0);
        }
    }

    if (arg0->unk38 != 0) {
        s32 p1;
        s32 p2;
        s32 p3;
        s32 prod;

        arg0->unk30 += arg0->unk32;

        p1 = arg0->unk22;
        p1 = (p1 << 8) | arg0->unk2E;
        prod = arg0->unk30 * D_800BE9E4;
        p1 += prod;
        arg0->unk22 = p1 >> 8;
        p1 = (u8)p1;
        arg0->unk2E = p1;
        p2 = arg0->unk20;
        p2 = (p2 << 8) | arg0->unk2C;
        prod = arg0->unk26 * D_800BE9E4;
        p2 += prod;
        arg0->unk20 = p2 >> 8;
        p2 = (u8)p2;
        arg0->unk2C = p2;
        p3 = arg0->unk24;
        p3 = (p3 << 8) | arg0->unk2D;
        prod = arg0->unk28 * D_800BE9E4;
        p3 += prod;
        arg0->unk24 = p3 >> 8;
        p3 = (u8)p3;
        arg0->unk2D = p3;
        if (arg0->unk1E != 0) {
            limit = (arg0->unk10->unk4 << 8) - 1;
            arg0->unk1C += arg0->unk1E * D_800BE9E4;
            if (limit < arg0->unk1C) {
                if (arg0->unk44 & 0x20) {
                    arg0->unk38 = 0;
                    finished = 1;
                } else {
                    arg0->unk44 |= 0x80;
                    if (arg0->unk44 & 2) {
                        arg0->unk1C = limit - (arg0->unk1C % limit);
                        arg0->unk1E = -arg0->unk1E;
                    } else {
                        do {
                            arg0->unk1C -= limit;
                        } while (limit < arg0->unk1C);
                    }
                }
            } else if (arg0->unk1C < 0) {
                if (arg0->unk44 & 0x40) {
                    arg0->unk38 = 0;
                    finished = 1;
                } else if (arg0->unk44 & 2) {
                    arg0->unk1C = -arg0->unk1C % limit;
                    arg0->unk1E = -arg0->unk1E;
                } else {
                    do {
                        arg0->unk1C += limit;
                    } while (arg0->unk1C < 0);
                }
            }
        }
    } else {
        finished = 1;
    }

    if (finished != 0) {
        if (arg0->unk48 != -1) {
            D_8008CA20[arg0->unk48]((struct102 *)arg0);
        }
        func_1516972C((struct102 *)arg0);
    }
}
typedef struct {
    s32 unk0;
    u8 pad4[0x6];
    u8 unkA;
} Tex15168118;

typedef struct {
    u8 pad0[0x10];
    Tex15168118 *unk10;
    s32 unk14;
    s32 unk18;
    s16 unk1C;
    s16 pad1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 pad26[0xE];
    s16 unk34;
    s16 unk36;
    u8 pad38[0x4];
    u8 unk3C;
    u8 unk3D;
    u8 unk3E;
    u8 unk3F;
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 pad43;
    u16 unk44;
} Obj15168118;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 unk6;
    s16 unk8;
    u8 unkA;
    u8 padB[2];
    u8 unkD;
} Pos15168118;

extern Tex15168118 D_800907FC;
extern Tex15168118 D_8009053C;
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern s32 D_800D2C9C;
s32 func_15095760(Gfx *arg0, void *arg1);

s32 func_15168118(Gfx *arg0, Obj15168118 *arg1, s16 arg2) {
    s32 idx;
    Pos15168118 pos;
    s32 pad1[2];
    u8 update;
    s32 pad2[3];
    f32 angle;
    s32 v;
    s32 base;
    s32 r, g, b;
    Tex15168118 *t;
    Tex15168118 *e;

    pos.unkD = 0;
    if (arg1->unk44 & 4) {
        pos.unkD = 1;
    }
    if (arg1->unk44 & 8) {
        pos.unkD |= 2;
    }
    if ((u32)arg1->unk10->unk0 < 0x10000000) {
    }
    if ((u32)arg1->unk10->unk0 < 0x10000000) {
    }
    update = 1;
    {
    s32 tex;
    t = arg1->unk10;
    if ((u32)t->unk0 < 0x10000000) {
        tex = t->unk0;
    } else {
        tex = ((s32 *)t->unk0)[arg1->unk1C >> 8];
    }
    arg0 = func_15142E24(arg0, (s32)t, arg1->unk1C << 8, 2, 0x100, 0x100, tex, 5, (s32)&pos, &update, 3);
    }
    idx = arg2;
    pos.x = arg1->unk20;
    pos.y = arg1->unk22;
    pos.z = arg1->unk24;
    v = arg1->unk34;
    if (v < 0) {
        angle = fabsf(D_800DBFF0[idx].unk380 + (arg1->unk18 & 0xFF) * 1.40625f);
        while (angle >= 360.0f) {
            angle -= 360.0f;
        }
        if (angle >= 180.0f) {
            angle -= 180.0f;
        }
        if (angle >= 90.0f) {
            angle = 180.0f - angle;
        }
        v = v * angle * -0.01111111138f;
        if (v < 0x400) {
            v = 0x400;
        }
    }
    pos.unk6 = v;
    v = arg1->unk36;
    if (v < 0) {
        idx = (arg1->unk34 < 0) ? 0 : arg1->unk34;
        v = fabsf(D_800DBFF0[arg2].unk388) * (idx + v) * 0.01111111138f - v;
    }
    pos.unk8 = v;
    if (arg1->unk44 & 0x100) {
        u8 mode;
        u8 combine;
        s32 pad3[2];

        mode = arg1->unk14 & 0xFF;
        combine = (arg1->unk14 & 0xFFFF0000) >> 16;
        arg0 = func_15142B7C(arg0, 5, 0);
        arg0 = func_1513F4E4(arg0, combine, &update);
        arg0 = func_15142FBC(arg0, G_PM_NPRIMITIVE | ((arg1->unk44 & 0x200) ? G_CYC_2CYCLE : G_CYC_1CYCLE) | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[mode].unk0 | D_800A4AC8[mode].unk4 | 4, &update);
    } else if (arg1->unk10->unkA != 5) {
        arg0 = func_1513F4E4(arg0, 0xD, &update);
        arg0 = func_15142FBC(arg0, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4 | 4, &update);
    } else if (arg1->unk10->unkA == 3) {
        arg0 = func_1513F4E4(arg0, 0xE, &update);
        arg0 = func_15142FBC(arg0, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4 | 4, &update);
    } else if (arg1->unk44 & 0x10) {
        arg0 = func_1513F4E4(arg0, 0xD, &update);
        arg0 = func_15142FBC(arg0, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[0x12].unk0 | D_800A4AC8[0x12].unk4 | 4, &update);
    } else {
        arg0 = func_1513F4E4(arg0, 0xF, &update);
        arg0 = func_15142FBC(arg0, G_PM_NPRIMITIVE | G_CYC_2CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[0x11].unk0 | D_800A4AC8[0x11].unk4 | 4, &update);
    }
    r = arg1->unk3D;
    g = arg1->unk3E;
    b = arg1->unk3F;
    arg0 = func_15142CF0(arg0, 1, 0, arg1->unk3C, r, g, b, &update);
    if (arg1->unk44 & 1) {
        r = arg1->unk40;
        g = arg1->unk41;
        b = arg1->unk42;
    } else {
        t = arg1->unk10;
        e = &D_8009053C;
        if (arg1->unk10 == &D_800907FC) { r = 0x80; g = 0x80; b = 0xFF; } else if (e == t) { r = 0xFF; g = 0xFF; b = 0xFF; } else { r = 0; g = 0; b = 0; }
    }
    arg0 = func_15142C10(arg0, r, g, b, 0, &update);
    return func_15095760(arg0, (void *)&pos);
}
typedef struct {
    Vtx vtx[8];
    u8 pad80[8];
    s16 unk88;
    u8 pad8A[0xE];
    u16 unk98;
    u8 pad9A[2];
    s16 unk9C;
    s16 unk9E;
    u8 unkA0;
} Struct1516865C;

void func_1516865C(Struct1516865C *arg0, u8 r, u8 g, u8 b, u8 a) {
    s16 s0;
    s16 s1;
    s16 t0;
    s16 t1;
    s32 i;

    s1 = 0x2000;
    s0 = (D_8008CA4C[arg0->unkA0]->unk8 + 0x100) << 5;
    t1 = 0x2000;
    t0 = (D_8008CA4C[arg0->unkA0]->unk6 + 0x100) << 5;
    for (i = 0; i < 2; i++) {
        arg0->vtx[i * 4 + 1].v.flag = 0;
        arg0->vtx[i * 4 + 1].v.cn[0] = r;
        arg0->vtx[i * 4 + 1].v.cn[1] = g;
        arg0->vtx[i * 4 + 1].v.cn[2] = b;
        arg0->vtx[i * 4 + 1].v.cn[3] = a;
        arg0->vtx[i * 4 + 2].v.flag = 0;
        arg0->vtx[i * 4 + 2].v.cn[0] = r;
        arg0->vtx[i * 4 + 2].v.cn[1] = g;
        arg0->vtx[i * 4 + 2].v.cn[2] = b;
        arg0->vtx[i * 4 + 2].v.cn[3] = a;
        arg0->vtx[i * 4 + 3].v.flag = 0;
        arg0->vtx[i * 4 + 3].v.cn[0] = r;
        arg0->vtx[i * 4 + 3].v.cn[1] = g;
        arg0->vtx[i * 4 + 3].v.cn[2] = b;
        arg0->vtx[i * 4 + 3].v.cn[3] = a;
        arg0->vtx[i * 4 + 0].v.flag = 0;
        arg0->vtx[i * 4 + 0].v.cn[0] = r;
        arg0->vtx[i * 4 + 0].v.cn[1] = g;
        arg0->vtx[i * 4 + 0].v.cn[2] = b;
        arg0->vtx[i * 4 + 0].v.cn[3] = a;
    }
    if (arg0->unk98 & 0x80) {
        s0 = 0x2000;
        s1 = (D_8008CA4C[arg0->unkA0]->unk8 + 0x100) << 5;
    }
    if (arg0->unk98 & 0x100) {
        t0 = 0x2000;
        t1 = (D_8008CA4C[arg0->unkA0]->unk6 + 0x100) << 5;
    }
    arg0->vtx[1].v.tc[0] = arg0->vtx[5].v.tc[0] = arg0->vtx[2].v.tc[0] = arg0->vtx[6].v.tc[0] = t1;
    arg0->vtx[2].v.tc[1] = arg0->vtx[6].v.tc[1] = arg0->vtx[3].v.tc[1] = arg0->vtx[7].v.tc[1] = s1;
    arg0->vtx[0].v.tc[0] = arg0->vtx[4].v.tc[0] = arg0->vtx[3].v.tc[0] = arg0->vtx[7].v.tc[0] = t0;
    arg0->vtx[0].v.tc[1] = arg0->vtx[4].v.tc[1] = arg0->vtx[1].v.tc[1] = arg0->vtx[5].v.tc[1] = s0;
    arg0->unk88 = 0;
    arg0->unk9C = 0;
    arg0->unk9E = 0;
}
// PERMUTER CANDIDATE (best 3095): leaf func; logic decoded fully (strided 8x0x10 init loop +
// two conditional value-swaps keyed on obj->0x98 flags 0x80/0x100 + field mirroring). Target
// homes a1/a2/a3 to caller arg slots (0xc/0x10/0x14) in a leaf while keeping them register-
// resident, an IDO arg-home idiom not reproducible from C; register allocation also diverges.
// Best reconstruction:
// void func_1516865C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
//     u8 *obj = (u8 *)arg0; s32 v0_val, v1_val, t1_val, t2_val, flags, i; s16 tmp; u8 *q; u8 t0b;
//     arg1 &= 0xFF; arg2 &= 0xFF; arg3 &= 0xFF;
//     v0_val = 0x2000; t1_val = 0x2000;
//     v1_val = ((s32)((D_8008CA4C[obj[0xA0]]->unk8 + 0x100) << 21)) >> 16;
//     t2_val = ((s32)((D_8008CA4C[obj[0xA0]]->unk6 + 0x100) << 21)) >> 16;
//     t0b = (u8)arg4; q = obj; i = 0;
//     do { i++; q += 0x40;
//         *(s16*)(q-0x2A)=0; q[-0x24]=arg1; q[-0x23]=arg2; q[-0x22]=arg3; q[-0x21]=t0b;
//         *(s16*)(q-0x1A)=0; q[-0x14]=arg1; q[-0x13]=arg2; q[-0x12]=arg3; q[-0x11]=t0b;
//         *(s16*)(q-0x0A)=0; q[-0x04]=arg1; q[-0x03]=arg2; q[-0x02]=arg3; q[-0x01]=t0b;
//         *(s16*)(q-0x3A)=0; q[-0x34]=arg1; q[-0x33]=arg2; q[-0x32]=arg3; q[-0x31]=t0b;
//     } while (i != 2);
//     flags = *(u16*)(obj+0x98);
//     if (flags & 0x80)  { v0_val = ((s32)((D_8008CA4C[obj[0xA0]]->unk8 + 0x100) << 21)) >> 16; v1_val = 0x2000; }
//     if (flags & 0x100) { t1_val = ((s32)((D_8008CA4C[obj[0xA0]]->unk6 + 0x100) << 21)) >> 16; t2_val = 0x2000; }
//     *(s16*)(obj+0x68)=t1_val; tmp=*(s16*)(obj+0x68);
//     *(s16*)(obj+0x7A)=v0_val; *(s16*)(obj+0x78)=t2_val;
//     *(s16*)(obj+0x28)=tmp; *(s16*)(obj+0x58)=tmp; *(s16*)(obj+0x18)=tmp;
//     tmp=*(s16*)(obj+0x7A); *(s16*)(obj+0x5A)=v1_val; *(s16*)(obj+0x88)=0;
//     *(s16*)(obj+0x3A)=tmp; *(s16*)(obj+0x6A)=tmp; *(s16*)(obj+0x2A)=tmp;
//     tmp=*(s16*)(obj+0x78); *(s16*)(obj+0x9C)=0; *(s16*)(obj+0x9E)=0;
//     *(s16*)(obj+0x38)=tmp; *(s16*)(obj+0x48)=tmp; *(s16*)(obj+0x8)=tmp;
//     tmp=*(s16*)(obj+0x5A); *(s16*)(obj+0x1A)=tmp; *(s16*)(obj+0x4A)=tmp; *(s16*)(obj+0xA)=tmp;
// }
s32 func_15168800(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp;

    temp = func_15167A68(0xE, arg2, 0xB8, 1, *((u8 *)&arg1 + 3), 1);
    if (temp == 0) {
        return 0;
    }
    bcopy(arg0, (void *)(temp + 0x10), 0xA8);
    return temp;
}
void func_15168870(GameAnimObject *arg0) {
    s32 temp_v0;
    s32 temp_v1;
    u8 temp_a1;
    s16 temp_a2;
    void (*func)(struct102 *);

    temp_a2 = arg0->field_0x9A;
    if (temp_a2 != 0) {
        temp_a1 = D_8008CA4C[arg0->field_0xB0]->unk4;
        temp_v1 = temp_a1;
        temp_v1 <<= 8;
        arg0->field_0x98 += temp_a2 * D_800BE9E4;
        temp_v0 = arg0->field_0x98;
        temp_v1 -= 1;
        if (temp_v1 < temp_v0) {
            if (arg0->field_0xA8 & 0x40) {
                arg0->field_0x98 = temp_v1 - (temp_v0 % temp_v1);
                arg0->field_0x9A = -temp_a2;
            } else if (arg0->field_0xA8 & 4) {
                arg0->field_0x98 = -1;
            } else {
                do {
                    arg0->field_0x98 = temp_v0 - temp_v1;
                    temp_v0 = arg0->field_0x98;
                } while (temp_v1 < temp_v0);
            }
        } else if (temp_v0 < 0) {
            if (arg0->field_0xA8 & 0x40) {
                arg0->field_0x98 = -temp_v0 % temp_v1;
                arg0->field_0x9A = -temp_a2;
            } else if (arg0->field_0xA8 & 4) {
                arg0->field_0x98 = -1;
            } else {
                do {
                    arg0->field_0x98 = temp_v0 + temp_v1;
                    temp_v0 = arg0->field_0x98;
                } while (temp_v0 < 0);
            }
        }
    }

    temp_v0 = arg0->field_0xB2;
    if (temp_v0 != -1) {
        func = D_8008C9C8[temp_v0];
        if (func != NULL) {
            func((struct102 *)arg0);
        }
    }

    if (arg0->field_0x98 == -1) {
        func_1516972C((struct102 *)arg0);
    }
}
void func_15168A2C(s32 arg0) {
    func_15168B10(arg0, 0);
}
/* Pushes obj onto the front of list D_800DCE50[obj->list][type] and sets its type. */
void func_15168A4C(ObjLink *obj, s32 type) {
    s32 list;
    ObjLink **head;

    list = obj->list;
    head = (ObjLink **)&D_800DCE50[list][type];
    obj->next = *head;
    if (obj->next != NULL) {
        obj->next->prev = obj;
    }
    obj->type = type;
    obj->prev = NULL;
    *head = obj;
}

/* Unlinks obj from its current list. */
void func_15168A9C(ObjLink *obj) {
    s32 list = obj->list;
    s32 type = obj->type;
    ObjLink **head = (ObjLink **)&D_800DCE50[list][type];

    if (obj == *head) {
        *head = obj->next;
    }
    if (obj->next != NULL) {
        obj->next->prev = obj->prev;
    }
    if (obj->prev != NULL) {
        obj->prev->next = obj->next;
    }
}

/* Moves obj to the list for another type. */
void func_15168B10(s32 obj, s32 type) {
    func_15168A9C((ObjLink *)obj);
    func_15168A4C((ObjLink *)obj, type);
}

// PERMUTER CANDIDATE / JUSTREG (best 1057): structure fully matches (both 0x14 stores kept via
// RMW forwarding). Remainder is a v0<->t6 register rotation on `lo`/`masked` + constant-hoist
// scheduling (li 0x1E vs lui order) that no C form pinned. Best reconstruction:
// permuter NO ZERO, best 305 (improved from 1057)
typedef struct Local15168B44 { u8 pad0[0x14]; s32 unk14; u8 pad18[0x20]; s16 unk38; u8 pad3A[5]; u8 unk3F; } Local15168B44;

void func_15168B44(void *arg0)
{
  Local15168B44 *a;
  s32 v1;
  s32 new_var2;
  u16 t6;
  u16 hi;
  u8 a2;
  s32 new_var;
  s32 c0;
  s32 z0;
  s32 z1;
  s32 z2;
  s32 t9;
  s32 z3;
  s32 z4;
  s32 z5;
  s32 keep;
  a = arg0;
  hi = 0x1E;
  v1 = a->unk14;
  new_var2 = v1;
  t6 = ((u16) new_var2) ^ 0;
  new_var = t6 - (1 & 0xFFFFFFFFu);
  if (t6 != 0)
  {
    c0 = v1;
    z0 = 0;
    z1 = 0;
    z2 = 0;
    z3 = 0;
    z4 = 0;
    z5 = 0;
    keep = new_var & 0xFFFF;
    new_var = keep * 0;
    if (a)
    {
    }
    hi++;
    hi--;
    t9 = (v1 & 0xFFFF0000) | new_var;
    v1 = 0;
    a->unk14 = t9;
    a->unk38 = hi;
    if (keep)
    {
      ;
    }
    *((s32 *) (((s8 *) arg0) + 0x14)) = ((t9 | keep) | (((((((c0 * 0) | (z0 * 0)) | (z1 * 0)) | (z2 * 0)) | (z3 * 0)) | (z4 * 0)) | (z4 = z5 * 0))) & 0xFFFFFFFFu;
    return;
  }
  hi = (u16) (v1 >> 16);
  v1 = a->unk3F;
  if (0)
  {
  }
  a2 = v1;
  if (hi < a2)
  {
    a->unk3F = a2 - hi;
    if (1)
    {
    }
    a->unk38 = 0x1E;
  }
  else
  {
    a2 = 0;
    a->unk38 = (a2, 0);
  }
}
// void func_15168B44(struct102 *arg0) {
//     s32 lo = *(s32 *)((u8 *)arg0 + 0x14) & 0xFFFF;
//     s32 hi;
//     s32 max;
//     if (lo != 0) {
//         *(s32 *)((u8 *)arg0 + 0x14) &= 0xFFFF0000;
//         *(s16 *)((u8 *)arg0 + 0x38) = 0x1E;
//         *(s32 *)((u8 *)arg0 + 0x14) |= (u16)(lo - 1);
//     } else {
//         max = *(u8 *)((u8 *)arg0 + 0x3F);
//         hi = (*(s32 *)((u8 *)arg0 + 0x14) >> 16) & 0xFFFF;
//         if (hi < max) {
//             *(u8 *)((u8 *)arg0 + 0x3F) = max - hi;
//             *(s16 *)((u8 *)arg0 + 0x38) = 0x1E;
//         } else {
//             *(s16 *)((u8 *)arg0 + 0x38) = 0;
//         }
//     }
// }
void func_15168BAC(struct102 *arg0) {
    u8 temp_v0;

    temp_v0 = *(u8 *)((u8 *)arg0 + 0xE4);
    if (temp_v0 != 0) {
        D_8008CA20[temp_v0](arg0);
    }
}
s32 func_15167A68(s32, s32, s32, s32, s32, s32);

void func_15168BE4(s32 *arg0, s32 arg1, s32 arg2) {
    s32 temp;

    if (arg0[0x10] != 0) {
        temp = func_15167A68(0x10, arg2, 0xF0, 1, (u8)arg1, 1);
        if (temp != 0) {
            bcopy(arg0, (void *)(temp + 0x90), 0x60);
        }
    }
}
Gfx *func_15168C4C(Gfx *gfx, Struct15168C4C *obj, s32 arg2) {
    s32 mode1;
    u8 update;
    s16 *zero_ptr;
    s16 *copy_ptr;

    func_15043D90((Mtx *)((u8 *)obj + (D_800BE9C0 << 6) + 0x10), obj->unk9C, obj->unkA0, obj->unkA4,
        obj->unkC0, obj->unkC4, obj->unkC8, obj->unk90, obj->unk94, obj->unk98);

    zero_ptr = &D_800DD1C4;
    copy_ptr = &D_800DD1C2;

    gDPPipeSync(gfx++);
    gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, obj->prim_alpha);
    D_800DD1C6 = obj->prim_alpha;
    *zero_ptr = 0;
    *copy_ptr = *zero_ptr;
    D_800DD1C0 = *copy_ptr;
    gDPSetEnvColor(gfx++, 0, 0, 0, 0);
    gDPSetTileSize(gfx++, 0, 2, obj->tile_ult, 0x7E, 0xFE);

    switch (obj->render_mode) {
    case 1:
        *(s32 *)((u8 *)&mode1 - 4) = 0x552230;
        break;
    case 2:
        *(s32 *)((u8 *)&mode1 - 4) = 0x504A50;
        break;
    }

    *(u8 *)((u8 *)&update - 4) = 0;
    gfx = func_15142FBC(gfx, 0x8ACA0, *(s32 *)((u8 *)&mode1 - 4), (u8 *)((u8 *)&update - 4));

    if (obj->combine_mode == 1) {
        gDPSetCombine(gfx++, 0x123824, 0xFF73FFFF);
    }

    WGSPMATRIX15168C4C(gfx++, (u8 *)obj + (D_800BE9C0 << 6) + 0x10);
    WGSPDISPLAYLIST15168C4C(gfx++, obj->display_list);

    return gfx;
}
void func_15168E34(s32 *arg0, s32 arg1) {
    s32 val = *arg0;
    if ((val & 0x0F000000) == 0) {
        *arg0 = val + arg1;
    }
}
typedef struct { s8 unk0; s8 unk1; s8 unk2; u8 unk3; u32 unk4; } Struct15168F08;

// PERMUTER CANDIDATE / JUSTREG (best 717): mirrors matched sibling func_15168F08 but calls
// func_15168E34; the call forces saved-reg promotion and a delay-slot reschedule (target puts
// a0=&p->unk4 in the first beq slot + distributes i++ into the bnel slots) that drives a whole-
// function saved-register rotation (base s1 vs s4). Not steerable from C. Best reconstruction:
// permuter NO ZERO, best 360 (improved from 717)
void func_15168E54(s8 *arg0, s32 arg1) {
    s32 i;
    s8 *p;

    for (i = 0; arg0[i << 3] != -0x21; i++) {
        p = arg0 + (i << 3);
        if ((*p == 1) || ((*p == -0x24) && (*(u8 *)(p + 3) == 0xE))) {
            func_15168E34((s32 *)(p + 4), arg1);
        }
    }
}
// void func_15168E54(Struct15168F08 *arg0, s32 arg1) {
//     s32 i; Struct15168F08 *p; s8 tag;
//     i = 0; p = arg0;
//     if (arg0->unk0 == -0x21) { return; }
//     p = &arg0[i]; tag = p->unk0;
//     do {
//         i++;
//         if ((p->unk0 == 1) || ((p->unk0 == -0x24) && (p->unk3 == 0xE))) {
//             func_15168E34((s32 *)&p->unk4, arg1);
//         }
//         p = &arg0[i];
//         do { tag = p->unk0; } while (0);
//     } while (p->unk0 != -0x21);
// }

void func_15168F08(Struct15168F08 *arg0, s32 arg1) {
    s32 i;
    Struct15168F08 *p;
    s8 tag;

    i = 0;
    p = arg0;
    if (arg0->unk0 == -0x21) {
        return;
    }
    p = &arg0[i];
    tag = p->unk0;
    do {
        i++;
        if ((p->unk0 == 1) || ((p->unk0 == -0x24) && (p->unk3 == 0xE))) {
            p->unk4 = p->unk4 & 0xFFFFFF;
            p->unk4 += arg1;
        }
        p = &arg0[i];
        do {
            tag = p->unk0;
        } while (0);
    } while (p->unk0 != -0x21);
}
void func_15168F84(s32 arg0, s32 *arg1, s32 *arg2) {
    if (arg0 == 0) {
        *arg1 = 0x1;
        *arg2 = 0x41;
    } else if (arg0 == 1) {
        *arg1 = 0x42;
        *arg2 = 0x4F;
    } else if (arg0 == 2) {
        *arg1 = 0x50;
        *arg2 = 0x58;
    } else if (arg0 == 3) {
        *arg1 = 0x59;
        *arg2 = 0x5C;
    } else if (arg0 == 5) {
        *arg1 = 0x61;
        *arg2 = 0x63;
    } else if (arg0 == 6) {
        *arg1 = 0x64;
        *arg2 = 0x65;
    } else {
        *arg1 = 0x5D;
        *arg2 = 0x60;
    }
}
void func_15169040(s32 arg0, u8 arg1) {
    func_15169070(0, 0x68, arg0, arg1);
}
void func_15143D18(s32 *, s32 *, s32, s32);
void func_1516968C(struct102 *arg0, u8 *arg1, u8 arg2);

/* Sends an event to every object of types [first, last) (clamped by func_15143D18). */
void func_15169070(s32 first, s32 last, s32 event, u8 eventType) {
    s32 type;
    struct102 *node;

    func_15143D18(&first, &last, 2, OBJ_TYPE_COUNT);
    for (type = first; type < last; type++) {
        s32 list;
        for (list = 0; list != 2; list++) {
            if (OBJ_TYPES[type].handleEvent != NULL) {
                node = D_800DCE50[list][type];
                D_800DD190++;
                while (node != NULL) {
                    OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                    func_1516968C(node, (u8 *)event, eventType);
                    OBJ_TYPES[type].handleEvent(node, event, eventType);
                    node = OBJ_ITER_NEXT[D_800DD190];
                }
                D_800DD190--;
            } else {
                node = D_800DCE50[list][type];
                D_800DD190++;
                while (node != NULL) {
                    OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                    func_1516968C(node, (u8 *)event, eventType);
                    node = OBJ_ITER_NEXT[D_800DD190];
                }
                D_800DD190--;
            }
        }
    }
}

/* Sends an event to every object of the `count` types listed in `types`. */
void func_15169260(s32 types, s32 count, s32 event, u8 eventType) {
    s32 *type;
    s32 i;
    s32 list;
    struct102 *node;

    for (i = 0, type = (s32 *)types; i < count; i++) {
        for (list = 0; list != 2; list++) {
            if (OBJ_TYPES[*type].handleEvent != NULL) {
                node = D_800DCE50[list][*type];
                D_800DD190++;
                while (node != NULL) {
                    OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                    func_1516968C(node, (u8 *)event, eventType);
                    OBJ_TYPES[*type].handleEvent(node, event, eventType);
                    node = OBJ_ITER_NEXT[D_800DD190];
                }
                D_800DD190--;
            } else {
                node = D_800DCE50[list][*type];
                D_800DD190++;
                while (node != NULL) {
                    OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                    func_1516968C(node, (u8 *)event, eventType);
                    node = OBJ_ITER_NEXT[D_800DD190];
                }
                D_800DD190--;
            }
        }
        type++;
    }
}

/* Sends an event to every object of one type (clamped to the last type). */
void func_1516944C(s32 type, s32 event, u8 eventType) {
    s32 list;
    struct102 *node;

    if (type >= OBJ_TYPE_COUNT) {
        type = OBJ_TYPE_COUNT - 1;
    }
    for (list = 0; list != 2; list++) {
        if (OBJ_TYPES[type].handleEvent != NULL) {
            node = D_800DCE50[list][type];
            D_800DD190++;
            while (node != NULL) {
                OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                func_1516968C(node, (u8 *)event, eventType);
                OBJ_TYPES[type].handleEvent(node, event, eventType);
                node = OBJ_ITER_NEXT[D_800DD190];
            }
            D_800DD190--;
        } else {
            node = D_800DCE50[list][type];
            D_800DD190++;
            while (node != NULL) {
                OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                func_1516968C(node, (u8 *)event, eventType);
                node = OBJ_ITER_NEXT[D_800DD190];
            }
            D_800DD190--;
        }
    }
}
void func_151695F0(struct127 *arg0, u8 arg1) {
    struct {
        s32 unk0;
        u8 unk4;
    } sp18;

    sp18.unk0 = (s32)arg0;
    sp18.unk4 = arg0->unique_id;
    func_15169040((s32)&sp18, arg1);
}
void func_1516962C(s32 arg0, struct127 *arg1, u8 arg2) {
    struct {
        s32 unk0;
        u8 unk4;
    } sp18;
    sp18.unk0 = (s32)arg1;
    sp18.unk4 = arg1->unique_id;
    func_1516944C(arg0, (s32)&sp18, arg2);
}
s32 func_15169668(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    D_800D2DAB = 1;
    return arg0;
}
void func_1516968C(struct102 *arg0, u8 *arg1, u8 arg2) {
    if (arg2 == 0xF || arg2 == 0x10) {
        if (arg1[0] == ((u8 *)arg0)[0xC]) {
            func_1516972C(arg0);
        }
    }
}

void func_151696DC(struct102 *arg0) {
    s8 i;

    for (i = 0; i < D_800DD190; i++) {
        if ((s32)arg0 == ((s32 *)D_800DD198)[i]) {
            ((s32 *)D_800DD198)[i] = arg0->unk8;
        }
    }
}

void func_1516972C(struct102 *arg0) {
    void (*func)(struct102 *arg0);
    func_151696DC(arg0);

    if (arg0->unk0 >= 2) {
        func = D_8008B4D0[arg0->unk0].unk0;
        if (func != NULL) {
            func(arg0);
            return;
        }
        func_15169804(arg0);
    }
}

void func_1516979C(struct102 *arg0) {
    void (*func)(struct102 *arg0);

    func_151696DC(arg0);
    func = D_8008B4D4[arg0->unk0].unk0;
    if (func != NULL) {
        func(arg0);
        return;
    }
    func_15169824(arg0);
}

void func_15169804(struct102 *arg0) {
    func_15168B10(arg0, 1);
}

void func_15169824(struct102 *arg0) {
    func_15168A9C(arg0);
    func_10004074(arg0);
}

void func_15169850(s32 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4) {
    if (arg1 == 0) {
        if (*(s32 *)arg0 == *(s32 *)arg2 || *(u8 *)(arg0 + 4) == *(u8 *)arg3) {
            func_1516972C((struct102 *)arg4);
        }
    } else if (arg1 == 0x2D) {
        s32 v = *(s32 *)arg0;
        if (v == *(s32 *)arg2) {
            *(s32 *)arg2 = *(s32 *)(arg0 + 4);
            *(u8 *)arg3 = *(u8 *)(arg0 + 9);
        } else if (*(s32 *)(arg0 + 4) == *(s32 *)arg2) {
            *(s32 *)arg2 = v;
            *(u8 *)arg3 = *(u8 *)(arg0 + 8);
        }
    }
}
