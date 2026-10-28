#include <ultra64.h>

#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_150039E0.s")

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
#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_150049A4.s")
void func_15004A4C(void) {
    s32 i;
    for (i = 0; i < D_800DBEF0; i++) {
        (*(s32 **)D_800DBEF8)[i] = 0;
        (*(s8 **)D_800DBEFC)[i] = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_15004AAC.s")
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
