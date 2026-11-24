#define func_1502B7F0 func_1502B7F0__proto
#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#undef func_1502B7F0


typedef struct { s32 a, b, c, d; } V16_150039E0;

typedef struct {
    s16 x0; s16 x2; s16 x4; s16 x6; s16 x8; s16 xA;
    s32 xC;
    s32 x10;
    u32 x14;
    s32 x18;
    u8 pad1C[4];
    f32 x20; f32 x24; f32 x28;
    u8 x2C; u8 x2D; u16 x2E; u16 x30; u8 x32; u8 x33;
    u8 x34;
    u8 pad35[3];
    s32 x38;
    u8 x3C;
    u8 pad3D;
    u16 x3E;
    u16 x40;
    u8 pad42[2];
} Src150039E0;

typedef struct {
    f32 u0; f32 u4; f32 u8;
    void *uC;
    s16 u10; s16 u12; s16 u14; u16 u16;
    f32 u18;
    void *u1C;
    V16_150039E0 *u20; V16_150039E0 *u24; V16_150039E0 *u28;
    f32 u2C; f32 u30; f32 u34;
    s32 u38; s32 u3C; s32 u40; s32 u44;
    u8 pad48[4];
    u8 u4C; u8 u4D; u8 u4E; u8 u4F;
    s16 u50; s16 u52; s16 u54;
    s16 u56; s16 u58;
    s16 u5A; s16 u5C; s16 u5E;
    f32 u60; f32 u64; f32 u68;
    u16 u6C; u8 u6E; u8 u6F; u8 u70; u8 u71; u8 u72; u8 u73;
    s16 u74; u8 pad76[2];
    s32 u78; s32 u7C; s32 u80; s32 u84;
    s16 u88; u8 u8A; u8 u8B; u8 u8C; u8 u8D; u8 u8E; u8 pad8F;
    u16 u90; s8 u92; u8 pad93;
    s32 u94; s32 u98; s32 u9C;
} Ent150039E0;

typedef struct {
    V16_150039E0 *end;
    s32 x4;
    s32 x8;
    s32 xC;
    s32 x10;
    u8 pad14[0x14];
    V16_150039E0 data[1];
} Mdl150039E0;

typedef struct { s32 a; s32 b; s32 c; } Tbl88C90;
typedef struct { u8 b0; u8 b1; s16 h2; s16 h4; u16 h6; u8 b8; u8 b9; u16 hA; } TblA26C0;
typedef struct { Mdl150039E0 *mdl; s32 x4; } TblB0E50;
extern s32 D_800DBF98;
extern TblB0E50 *D_800B0E50;
extern Tbl88C90 D_80088C90[];
extern TblA26C0 D_800A26C0[];
extern void *D_800DBED0[4];
extern s32 D_800DBEE0;
extern s32 D_800DBEE4;
extern s32 *D_800DBF94;
s32 func_1502B7F0();
void *allocate_memory(s32, u32, s32, s32);
Mdl150039E0 *func_1502B6BC(s32 *, s32, s32, s32, s32, s32);
void func_1510CE60(void *, s32, s32, s32, s32);
extern u16 D_800BE2A0;
extern u16 D_800BE2A2;
extern u16 D_800BE2A4;
void func_15004AAC(void *, s32);
s32 func_15004BF0(s32);
void func_15004CE0();

#define ENTS ((Ent150039E0 *)D_800DBEF4)
#define src ((Src150039E0 *)((s32)base + i * 0x44))

void func_150039E0(s32 arg0, s32 arg1) {
    s32 i;
    s32 found;
    s32 id;
    s32 spA8;
    s32 j;
    Src150039E0 *spA0;
    Src150039E0 *sp9C;
    Mdl150039E0 *mdl;
    s32 type;
    s32 off;
    Ent150039E0 *ent;
    s8 flag;
    s32 hasTbl;
    Mdl150039E0 *mdl2;
    Src150039E0 *base;

    D_800DBF88 = 0xFF;
    spA0 = NULL;
    sp9C = NULL;
    flag = 0;
    D_800BE2A0 = D_800BE2A2 = D_800BE2A4 = 0;
    i = func_1502B7F0(&spA0, 2, 0xB, arg0);
    D_800DBF00 = i / 68U;
    D_800DBEF0 = D_800DBF00;
    D_800DBF98 = func_1502B7F0(&sp9C, 3, 0xC, arg0, 2) / 68U;
    D_800DBEF0 += D_800DBF98;
    D_800DBEF4 = allocate_memory(D_800DBEF0 * sizeof(Ent150039E0), 1, 2, 0);
    if (D_800DBEF4 == NULL) {
        D_800DBEF0 = 0;
        D_800DBF98 = 0;
        D_800DBF00 = 0;
        return;
    }
    base = spA0;
    for (i = 0; i < D_800DBEF0; i++) {
        if (i == D_800DBF00) {
            base = &sp9C[-i];
        }
        found = 0;
        hasTbl = 0;
        ent = i + ENTS;
        ent->u10 = src->x0;
        ent->u12 = src->x2;
        ent->u14 = src->x4;
        ent->u0 = src->x6;
        ent->u4 = src->x8;
        ent->u8 = src->xA;
        ent->u2C = src->x20;
        ent->u30 = src->x24;
        ent->u34 = src->x28;
        ent->u1C = (void *)src->xC;
        ent->u70 = src->x3C & 0xFFF7;
        ent->u88 = 0;
        ent->u6E = src->x34;
        ent->u3C = src->x18;
        type = *(s32 *)&src->x14;
        if (src->x14 < 0xFFFF) {
            if ((s32)src->x14 == 0x28 || (s32)src->x14 == 0x42) {
                ent->u70 |= 4;
                flag = 1;
            }
            ent->u38 = D_80088C90[src->x14].a;
            ent->u78 = D_80088C90[src->x14].b;
            ent->u92 = D_80088C90[src->x14].c;
            if (src->x14 == 0x16) {
                hasTbl = 1;
            }
        } else {
            ent->u38 = type;
            ent->u78 = 0;
        }
        ent->u40 = src->x38;
        ent->u7C = 0;
        ent->u80 = 0;
        ent->u84 = 0;
        ent->u74 = 0;
        ent->u4E = 0;
        ent->u6C = 0;
        ent->u73 = 4;
        ent->u60 = ent->u64 = ent->u68 = 0.0f;
        ent->u5A = ent->u5C = ent->u5E = ent->u68;
        ent->u18 = 0;
        ent->u8A = 0xFF;
        ent->u8B = ent->u8C = ent->u8D = ent->u8E = 0xFF;
        ent->u71 = i;
        ent->u94 = 0;
        ent->u98 = 0;
        if (ent->u1C == NULL) {
            id = src->x10;
            for (j = D_800DBF00; j < i; j++) {
                if (src->xC == (base + j)->xC && src->x10 == (base + j)->x10 && ENTS[j].u1C != NULL) {
                    found = j + 1;
                    break;
                }
            }
            if (found == 0) {
                mdl = func_1502B6BC(&spA8, 0, 0, 2, 3, id);
                ent->uC = mdl;
            }
            if (found != 0) {
                ent->u28 = ENTS[found - 1].u28;
                ent->u1C = ENTS[found - 1].u1C;
                ent->u9C = ENTS[found - 1].u9C;
                ent->u16 = ENTS[found - 1].u16;
                ent->u44 = ENTS[found - 1].u44;
            } else {
                ent->u28 = mdl->data;
                ent->u1C = mdl->end;
                ent->u9C = mdl->x8;
                ent->u16 = (V16_150039E0 *)ent->u1C - ent->u28;
                ent->u44 = mdl->x10;
            }
            if (i >= D_800DBF00) {
                ent->u4C = D_800A26C0[id].b0;
                ent->u4D = D_800A26C0[id].b1;
                ent->u50 = D_800A26C0[id].h4;
                ent->u52 = D_800A26C0[id].h2;
                ent->u4F = D_800A26C0[id].b8;
                ent->u70 = D_800A26C0[id].b9 & ~8;
                ent->u6C = D_800A26C0[id].hA;
                if (flag) {
                    ent->u70 |= 4;
                }
                ent->u90 = D_800A26C0[id].h6;
                ent->u72 = src->x33;
            } else {
                ent->u50 = src->x2E;
                ent->u52 = src->x30;
                ent->u4F = src->x32;
                ent->u4C = src->x2C;
                ent->u4D = src->x2D;
                ent->u90 = src->x40;
                if (src->x33 != 0) {
                    ent->u72 = src->x33;
                } else {
                    ent->u72 = func_15004BF0(1);
                }
            }
            if (id == 0x71) {
                ent->u40 = 0xE;
            }
            if (ent->u38 != 0 || hasTbl != 0) {
                if (ent->u6E == 0) {
                    ent->u6F = 0xF;
                } else {
                    ent->u6F = 0;
                }
            } else {
                ent->u6F = 0x80;
            }
            ent->u54 = id;
        } else {
            ent->u54 = src->x10 | 0x8000;
            if ((s32)ent->u1C == 1 || (s32)ent->u1C == 2) {
                for (j = 0; j < i; j++) {
                    if (src->xC == (base + j)->xC && src->x10 == (base + j)->x10 && ENTS[j].u1C != NULL) {
                        found = j + 1;
                        break;
                    }
                }
                if (found == 0) {
                    id = src->x10;
                    mdl2 = D_800B0E50[id].mdl;
                    ent->u16 = ((u32)mdl2->end - (u32)mdl2->data) >> 4;
                    ent->u28 = mdl2->data;
                    ent->u44 = mdl2->x10;
                    ent->u9C = mdl2->x8;
                } else {
                    ent->u16 = ENTS[found - 1].u16;
                    ent->u28 = ENTS[found - 1].u28;
                    ent->u44 = ENTS[found - 1].u44;
                    ent->u9C = ENTS[found - 1].u9C;
                }
                if ((s32)ent->u1C == 2) {
                    ent->u6F = 0;
                } else {
                    ent->u6F = 0x80;
                }
                if (found == 0) {
                    ent->u1C = mdl2->end;
                } else {
                    ent->u1C = ENTS[found - 1].u1C;
                }
                ent->u6C = src->x3E;
                ent->uC = NULL;
            }
            ent->u50 = src->x2E;
            ent->u52 = src->x30;
            ent->u4F = src->x32;
            ent->u4C = src->x2C;
            ent->u4D = src->x2D;
            ent->u90 = src->x40;
            if (src->x33 != 0) {
                ent->u72 = src->x33;
            } else {
                ent->u72 = func_15004BF0(1);
            }
        }
        func_15004AAC(ent, (s32)(base + i));
        if (ent->u38 == 0 && hasTbl == 0) {
            ent->u6F |= 0x20;
        }
        if (found == 0) {
            func_1510CE60(ent->u1C, 0, 1, 0x3F, 0);
            func_15004CE0(ent->u1C, (u32)mdl);
        }
    }
    for (i = 0; i < D_800DBEF0; i++) {
        ent = i + ENTS;
        if (ent->u28 == NULL) continue;
        {
            if ((ent->u70 & 1) == 1U) {
                ent->u20 = allocate_memory(ent->u16 * sizeof(V16_150039E0), 1, 2, 0);
                ent->u24 = allocate_memory(ent->u16 * sizeof(V16_150039E0), 1, 2, 0);
            } else {
                ent->u20 = ent->u24 = ent->u28;
            }
            if (ent->u20 != ent->u28) {
                for (j = 0; j < ent->u16; j++) {
                    ent->u20[j] = ent->u28[j];
                }
                if (ent->u24 != ent->u20) {
                    for (j = 0; j < ent->u16; j++) {
                        ent->u24[j] = ent->u28[j];
                    }
                }
            }
        }
    }
    D_800DBEE0 = (s32)allocate_memory(D_800DBEF0 << 6, 1, 2, 0);
    D_800DBEE4 = (s32)allocate_memory(D_800DBEF0 << 6, 1, 2, 0);
    *(s32 *)D_800DBEF8 = (s32)allocate_memory(D_800DBEF0 * 4, 1, 0, 0);
    *(s32 *)D_800DBEFC = (s32)allocate_memory(D_800DBEF0, 1, 0, 0);
    for (i = 0; i < 4; i++) {
        D_800DBED0[i] = allocate_memory(D_800DBEF0 * 4, 1, 0, 0);
    }
    D_800DBF94 = allocate_memory(D_800DBEF0 * 4, 1, 0, 0);
    for (i = 0; i < D_800DBEF0; i++) {
        D_800DBF94[i] = 0;
    }
    for (i = D_800DBF00; i < D_800DBEF0; i++) {
        if (ENTS[i].u72 == 0) {
            ENTS[i].u72 = func_15004BF0(0);
        }
    }
}

// FIXME: matches but something isnt right
void func_15004574(void) {
    if (D_800DBF88 != 0xFF) {
        D_800DBF8C = D_800DBEF4[D_800DBF88].unk1C;
        D_800DBF90 = D_800DBEF4[D_800DBF88].unk28;
    }
}

void func_150045BC(void) {
}

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u8 pad6[0xA];
} Vec16_150045C4;

typedef struct {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u16 unk16;
    u8 pad18[0x8];
    Vec16_150045C4 *unk20;
    Vec16_150045C4 *unk24;
    Vec16_150045C4 *unk28;
    u8 pad2C[0xC];
    void *unk38;
    u8 pad3C[0x12];
    u8 unk4E;
    u8 unk4F;
    u8 pad50[0x1E];
    u8 unk6E;
    u8 unk6F;
    u8 unk70;
    u8 pad71[0xF];
    s32 unk80;
    f32 unk84;
    u8 pad88[0x18];
} Obj_150045C4;

extern Mtx **D_80089250[];
extern s16 D_800D37E0;
extern f32 D_800DBF08[4][4];
extern f32 D_800DBF48[4][4];
extern void func_151150B0(void);
extern void func_15116058(void);
extern void func_15116110(void);
void func_150A44F0(s32, s16 *, s32);
void func_150A7960(f32 mf[4][4], f32, f32, f32, f32 *, f32 *, f32 *);
void func_1510E7A4(s32 *, s32, f32 *, s32, s32 *, s32, f32, f32, f32, f32, s32, s32, f32, f32);
void func_1510F800(s32);
void func_1511490C(f32 mf[4][4], Obj_150045C4 *);
void func_15004A4C(void);

void func_150045C4(s32 arg0) {
    s32 i;
    Obj_150045C4 *p;
    f32 mtx[4][4];
    s32 j;
    s32 sp90;
    f32 sp8C;
    s32 sp88;
    f32 x;
    f32 y;
    f32 z;

    func_15004A4C();
    guMtxIdentF(D_800DBF08);
    guMtxIdentF(D_800DBF48);
    for (i = 0; i < D_800DBEF0; i++) {
        p = (Obj_150045C4 *)&D_800DBEF4[i];
        if (p->unk38 == NULL || p->unk38 == (void *)func_15116058 || p->unk38 == (void *)func_15116110) {
            p->unk4E = 3;
        }
        if (!(p->unk70 & 1)) {
            func_1511490C(mtx, p);
            guMtxF2L(mtx, &(*D_80089250[D_800BE9C0])[i]);
            bcopy(&(*D_80089250[D_800BE9C0])[i], &(*D_80089250[!D_800BE9C0])[i], sizeof(Mtx));
        }
        if (p->unk38 == (void *)func_151150B0) {
            p->unk38 = NULL;
        }
        if ((p->unk70 & 1) == 1) {
            func_1511490C(mtx, p);
            for (j = 0; j < p->unk16; j++) {
                func_150A7960(mtx, p->unk28[j].x, p->unk28[j].y, p->unk28[j].z, &x, &y, &z);
                p->unk20[j].x = x;
                p->unk20[j].y = y;
                p->unk20[j].z = z;
            }
            bcopy(p->unk20, p->unk24, p->unk16 * sizeof(Vec16_150045C4));
        }
        func_1510F800(2);
        if ((p->unk6F & 0x80) == 0x80 && (p->unk4F & 0x60) != 0x20) {
            D_800D37E0 = i;
            func_150A44F0(1, &D_800D37E0, 1);
        }
    }
    for (i = 0; i < D_800DBEF0; i++) {
        p = (Obj_150045C4 *)&D_800DBEF4[i];
        if (p->unk6E == 1) {
            continue;
        }
        if ((p->unk6F & 0x20) == 0x20) {
            func_1510E7A4(&sp88, 0, &sp8C, 0, &sp90, 0, p->unk10, p->unk12, p->unk14, p->unk12, i + 1, 0, -10000.0f, 20000.0f);
            p->unk80 = sp88;
            p->unk84 = sp8C;
        }
    }
}
typedef struct {
    s8 cmd;
    u8 b1;
    u8 b2;
    u8 b3;
    s32 w1;
} Struct_150049A4;

void func_150049A4(Struct_150049A4 *gfx, s32 arg1, s32 arg2) {
    s32 i;
    Struct_150049A4 *p;
    s8 cmd;

    i = 0;
    if ((s8)*(u8*)&gfx[i] != -0x21) {
        p = &gfx[i];
        cmd = p->cmd;
        do {
            i++;
            switch (cmd) {
                case 0xDE:
                    p->w1 += arg1;
                    break;
                case 1:
                    p->w1 += arg1;
                    break;
                case -0x24:
                    if (p->b3 == 0xE) {
                        p->w1 += arg2;
                    }
                    break;
            }
            p = &gfx[i];
            cmd = p->cmd;
        } while (-0x21 - cmd);
    }
}

void func_15004A4C(void) {
    s32 i;
    for (i = 0; i < D_800DBEF0; i++) {
        (*(s32 **)D_800DBEF8)[i] = 0;
        (*(s8 **)D_800DBEFC)[i] = 0;
    }
}

extern u16 D_800BE2A0;
extern u16 D_800BE2A2;
extern u16 D_800BE2A4;
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void func_15004AAC(void *arg0, s32 arg1) {
    f32 sz;
    f32 sx;
    f32 sy;
    f32 x;
    f32 y;
    f32 z;
    f32 root;
    f32 new_var;
    s32 maxv = 0;
    s32 off;
    s32 len;
    s32 val;
    s16 *p;
    u16 new_var2;
    u16 cur;
    u16 count;

    count = *((u16 *)(((s8 *)arg0) + 0x16));
    new_var2 = count;
    if (0 < ((s32)new_var2)) {
        if ((arg0 && arg0) && arg0) {
        }
        off = 0;
        p = *((s16 **)(((s8 *)arg0) + 0x28));
        sz = *((f32 *)(((s8 *)arg0) + 0x34));
        z = ((f32)p[2]) * sz;
        sx = *((f32 *)(((s8 *)arg0) + 0x2C));
        x = ((f32)p[0]) * sx;
        sy = *((f32 *)(((s8 *)arg0) + 0x30));
        if (!arg0) {
        }
        len = ((s32)new_var2) * 0x10;
        do {
            off += 0x10;
            z = ((f32)p[2]) * sz;
            x = ((f32)p[0]) * sx;
            new_var = sy;
            y = ((f32)p[1]) * new_var;
            val = (s32)((z * z) + ((x * x) + (y * y)));
            if (maxv < val) {
                maxv = val;
            }
            p = p + 8;
        } while (off < len);
    }
    if (maxv != 0) {
        cur = *((u16 *)(((s8 *)arg0) + 0x50));
        root = sqrtf((f32)maxv);
        if ((*((u16 *)(((s8 *)arg0) + 0x50))) == 0) {
            val = (s32)root;
            *((u16 *)(((s8 *)arg0) + 0x50)) = (u16)val;
            *((u16 *)(((s8 *)arg0) + 0x52)) = (u16)val;
            D_800BE2A2 += 1;
            return;
        }
        val = (s32)root;
        if ((((s32)cur) < val) || (((s32)(*((u16 *)(((s8 *)arg0) + 0x52)))) < val)) {
            D_800BE2A0 += 1;
            return;
        }
        D_800BE2A4 += 1;
    }
}
s32 func_15004BF0(s32 arg0) {
    typedef struct { u8 before[0x72]; u8 objectId; u8 after[0x2D]; } Entry;
    s32 objectIndex;
    s32 candidateObjectId;

    if (arg0 == 0) {
        candidateObjectId = 1;
        for (objectIndex = (arg0 = D_800DBF00); objectIndex < D_800DBEF0; objectIndex++) {
            if (candidateObjectId == ((Entry *)D_800DBEF4)[objectIndex].objectId) {
                candidateObjectId++;
                objectIndex = arg0 - 1;
            }
            if (candidateObjectId >= 0x100) {
                candidateObjectId = 0xFF;
                break;
            }
        }
    } else {
        arg0 = 0xFF - D_800DBF00;
        candidateObjectId = arg0;
        for (objectIndex = 0; objectIndex < D_800DBEF0; objectIndex++) {
            if (candidateObjectId == ((Entry *)D_800DBEF4)[objectIndex].objectId) {
                candidateObjectId--;
                objectIndex = -1;
            }
            if (candidateObjectId <= 0) {
                candidateObjectId = arg0;
                break;
            }
        }
    }
    return candidateObjectId;
}
typedef struct { s8 unk0; s8 unk1; s8 unk2; u8 unk3; u32 unk4; } Struct15004CE0;

void func_15004CE0(Struct15004CE0 *arg0, u32 arg1) {
    s32 i = 0;
    Struct15004CE0 *p = arg0;
    s8 tag;
    if (arg0->unk0 == (-0x21)) {
        return;
    }
    p = &arg0[i];
    tag = p->unk0;
    do {
        i++;
        if (p->unk0 == (-0x24)) {
            if (p->unk3 == 0xE) {
                if (p->unk4 < 0x80000000) {
                    p->unk4 += arg1;
                }
            }
        }
        p = &arg0[i];
        do { tag = p->unk0; } while (0);
    } while (p->unk0 != (-0x21));
}
