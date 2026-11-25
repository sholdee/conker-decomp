#include <ultra64.h>
#include "functions.h"
/* variables.h types D_800D212C as struct187 *, which lacks the signed link fields used here;
   shadow the declaration file-locally and use SpawnPoint instead. */
#define D_800D212C D_800D212C_struct187_decl_in_variables_h
#include "variables.h"
#undef D_800D212C


extern u8 D_8009D954[];
extern u8 *D_80087240[];

/* Looks obj->id up in the 7 rows of D_80087240 (row lengths in D_8009D954) and returns
 * the first entry of the row that contains it, or the id itself when no row does.
 * Same search as func_1503D5F0 (game_6A3D0). */
s32 func_15084D00(struct127 *obj) {
    s32 id = obj->id;
    s32 i;
    s32 j;

    for (i = 0; i < 7; i++) {
        for (j = 0; j < D_8009D954[i]; j++) {
            if (id == D_80087240[i][j]) {
                return D_80087240[i][0];
            }
        }
    }
    return id;
}

/* A spawn point (0x18 bytes) in the D_800D212C table, D_800D2128 entries long. */
typedef struct {
    /* 0x00 */ s16 pos[3];
    /* 0x06 */ u8 unk6;   /* angle, 256 units per turn */
    /* 0x07 */ u8 facing; /* angle, 256 units per turn */
    /* 0x08 */ f32 unk8[3];
    /* 0x14 */ u8 unk14;
    /* 0x15 */ s8 unk15;
    /* 0x16 */ s8 link;   /* index of the spawn point to use instead; 0 = none, -1 = invalid */
    /* 0x17 */ u8 pad17;
} SpawnPoint;

typedef struct {
    /* 0x00 */ u8 pad0[0x42];
    /* 0x42 */ s8 unk42;
} F15084D70_obj;

extern SpawnPoint *D_800D212C;
extern F15084D70_obj *D_8008FDD4;
extern s8 D_800E0BE3;

/* Copies spawn point `spawnIndex` (or, when it is -1, the player's stored one) into the out
 * parameters. In multiplayer (D_800BE616) with D_800D18A8 set, it instead picks the team's spawn
 * point farthest from the other team's players, plus a random term. When `useLink` is set, the
 * point's link is followed. Remembers the chosen index in the player's state; returns 1 when
 * there is no spawn table, else 0. */
s32 func_15084D70(s32 playerIndex, s32 spawnIndex, s32 useLink, s16 *outPos, f32 *outUnk8, u8 *outUnk6,
                  u8 *outFacing, s32 *outUnk14, s32 *outUnk15, s32 unused, s32 *outLinkFlag) {
    s32 remember;
    struct126 *state;
    s32 mask; // matching: also the player loop counter and link index; separate locals change regalloc
    s32 team;
    s32 best;
    s32 i;
    f32 nearestDistSq;
    f32 bestDistSq;
    f32 dx;
    f32 dz;
    s32 count;
    s32 lastSpawn;
    s32 j; // spawn loop counter, then the chosen spawn index

    remember = TRUE;
    if (D_800BE9F0 == 0x2B) {
        remember = FALSE;
        D_800D18A8 = 0;
    }
    if (D_800BE616 && D_8008FDD4->unk42 == 0 && D_800E0BE3 != 0) {
        remember = FALSE;
    }
    if (playerIndex != 0xFF) {
        if (spawnIndex == -1) {
            spawnIndex = D_800CC2D0[playerIndex].unk31C->pad11B[0];
        }
        if (remember) {
            D_800CC2D0[playerIndex].unk31C->pad11B[0] = 0;
        }
    }
    spawnIndex &= 0xFFF;
    if (spawnIndex >= D_800D2128 || spawnIndex < 0) {
        spawnIndex = 0;
    }
    if (D_800D212C != NULL) {
        if (D_800BE616 && D_800D18A8) {
            u8 candidates[32];

            /* the byte at 0x128 (inside struct127's pad128) is the player's team */
            team = *(u8 *)&D_800CC2D0[playerIndex].pad128;
            state = D_800CC2D0[playerIndex].unk31C;
            count = 0;
            best = 0;
            if (team == 0) {
                mask = D_800D2120;
            } else {
                mask = D_800D2124;
            }
            for (j = 0; j < D_800D2128; j++) {
                if ((1 << j) & mask) {
                    candidates[count] = j;
                    count++;
                }
            }
            bestDistSq = 0.0f;
            /* state byte 0x1B0 (past the end of struct126) is the last multiplayer spawn index */
            lastSpawn = *(s8 *)((u8 *)state + 0x1B0);
            for (i = 0; i < count; i++) {
                if (lastSpawn != candidates[i]) {
                    nearestDistSq = 100000000.0f;
                    for (mask = 0; mask < D_8008FD8C; mask++) {
                        if (team != *(u8 *)&D_800CC2D0[mask].pad128) {
                            dx = D_800CC2D0[mask].x_position - D_800D212C[candidates[i]].pos[0];
                            dz = D_800CC2D0[mask].z_position - D_800D212C[candidates[i]].pos[2];
                            dx = dx * dx + dz * dz;
                            if (dx < nearestDistSq) {
                                nearestDistSq = dx;
                            }
                        }
                    }
                    dz = func_150ADA68() * 500.0f;
                    nearestDistSq += dz * dz;
                    if (bestDistSq < nearestDistSq) {
                        best = i;
                        bestDistSq = nearestDistSq;
                    }
                }
            }
            if (D_8008FDD4->unk42 != 0 || D_800E0BE3 == 0) {
                spawnIndex = candidates[best];
            }
            state = D_800CC2D0[playerIndex].unk31C;
            *(s8 *)((u8 *)state + 0x1B0) = spawnIndex;
            useLink = FALSE;
        }
        j = spawnIndex;
        if (useLink && (mask = D_800D212C[spawnIndex].link) != 0) {
            j = mask;
            if (mask == -1 || mask >= D_800D2128) {
                j = spawnIndex;
            }
            if (outLinkFlag != NULL) {
                mask = D_800D212C[j].unk15;
                if (mask >= 0) {
                    *outLinkFlag = 0;
                } else if (mask == -1) {
                    *outLinkFlag = 0;
                } else {
                    *outLinkFlag = 1;
                }
            }
        } else if (useLink && D_800D212C[spawnIndex].link == 0) {
            D_800DDE38 = 2;
        }
        outUnk8[0] = D_800D212C[j].unk8[0];
        outUnk8[1] = D_800D212C[j].unk8[1];
        outUnk8[2] = D_800D212C[j].unk8[2];
        outPos[0] = D_800D212C[j].pos[0];
        outPos[1] = D_800D212C[j].pos[1];
        outPos[2] = D_800D212C[j].pos[2];
        *outUnk6 = D_800D212C[j].unk6;
        *outFacing = D_800D212C[j].facing;
        *outUnk14 = D_800D212C[j].unk14;
        *outUnk15 = D_800D212C[j].unk15;
        if (D_800BE616 && D_800D18A8 == 0) {
            f32 angle;
            f32 offset;
            angle = *outFacing * 3.1415927410f * (1.0f / 128);
            offset = func_150ADA68() * 80.0f - 40.0f;
            outPos[0] += sinf(angle) * offset;
            outPos[2] += cosf(angle) * offset;
        }
        if (playerIndex != 0xFF && remember) {
            D_800CC2D0[playerIndex].unk31C->pad11B[0] = spawnIndex;
        }
        return 0;
    }
    return 1;
}

void func_15085410(struct127 *arg0, s32 arg1) {
    arg0->unk31C->pad11B[0] = arg1;
}

u8 func_15085420(struct127 *arg0) {
    return arg0->unk31C->pad11B[0];
}

extern s32  func_15084D70(s32, s32, s32, s16*, f32*, u8*, u8*, s32*, s32*, s32, s32*);
extern f32  func_1506AD30(struct127 *, f32, s32);
extern void func_1507C3E0(struct127 *, s16 *, s16 *, s16 *);
extern void func_1505D6F0(struct127 *, s32);
extern void func_15005818(struct108 *, struct127 *, f32 *);
extern void func_15169040(s32, u8);

void func_15085430(struct127 *arg0, s32 arg1, s32 arg2) {
    s16 sp78[3];
    f32 sp6C[3];
    u8 sp6B;
    u8 sp6A;
    s32 sp64;
    s32 sp60;
    struct127 *oldObject;
    s32 oldIndex;
    struct { s32 unk0; u8 unk4; } sp50;

    func_15084D70(arg0->unk127, arg1, D_800D18A8, sp78, sp6C, &sp6B, &sp6A, &sp60, &sp64, arg2, 0);
    oldObject = D_800D154C;
    oldIndex = D_800C3E78;
    D_800D154C = arg0;
    D_800C3E78 = arg0 - D_800CC2D0;
    func_1506AD30(arg0, 800.0f, 1);
    D_800D154C = oldObject;
    D_800C3E78 = oldIndex;
    arg0->x_position = sp78[0];
    arg0->y_position = sp78[1];
    arg0->z_position = sp78[2];
    arg0->old_x_position = arg0->x_position;
    arg0->old_y_position = *(volatile f32 *)&arg0->y_position;
    arg0->old_z_position = arg0->z_position;
    arg0->unk76 = arg0->unk78 = arg0->unk7A = sp6A << 8;
    arg0->unk40 = (s16)(arg0->unk7A + 0x4000) * 0.0054931640625f;
    arg0->unkB8 = (sp6B * 360) >> 8;
    while (arg0->unkB8 > 180.0f) {
        arg0->unkB8 -= 360.0f;
    }
    arg0->unk28 = 0.0f;
    arg0->unk180 = arg0->y_position;
    if (arg0->unk31C != NULL) {
        func_1507C3E0(arg0, (s16 *)&arg0->unk31C->unk114, (s16 *)&arg0->unk31C->unk116, (s16 *)&arg0->unk31C->unk118);
    }
    arg0->unk1CC = arg0->y_position;
    arg0->unk2F8 &= ~0x10;
    arg0->unk22C |= 0x200;
    arg0->unk164 = 0.0f;
    arg0->unk168 = 0.0f;
    arg0->unk16C = 0.0f;
    arg0->unk170 = 0.0f;
    if (arg0->unk127 != 0xFF) {
        *((u8 *)arg0->unk31C + 0x23) = sp64;
        if (D_800BE616 && D_800D18A8) {
            func_1505D6F0(arg0, arg0 - D_800CC2D0);
        }
        if (arg0->unk31C->unk78 == 0 && arg0->unk31C->unk19B != 4 && arg0->unk31C->unk19B != 7) {
            arg0->unk83 = 0;
        }
    } else {
        arg0->unk83 = 0;
    }
    if (arg0->camera != NULL) {
        func_15005818(arg0->camera, arg0, sp6C);
    }
    sp50.unk0 = (s32)arg0;
    sp50.unk4 = arg0->unique_id;
    func_15169040((s32)&sp50, 4);
}

typedef struct GameB21B0PlayerRecordA {
    s32 field_0;
    s32 field_4;
    s8 field_8;
    s8 field_9;
    u8 field_A;
    u8 padB;
    s32 field_C;
    u8 pad10[0xC];
} GameB21B0PlayerRecordA;

typedef struct GameB21B0PlayerTable {
    s32 unk0;
    GameB21B0PlayerRecordA recs[1];
} GameB21B0PlayerTable;

#define PREC(i) (((GameB21B0PlayerRecordA *)(&D_800D2138 + 1))[i])
#define PPTR(i) ((GameB21B0PlayerRecordA *)((s32)&D_800D2138 + ((i) * 28 + 4)))

void func_15085710(s16 arg0, s16 arg1, s32 arg2) {
    GameB21B0PlayerRecordA *p;

    if (arg0 >= 0xFF) {
        return;
    }
    switch (arg1) {
    case 0:
        PREC(arg0).field_0 = arg2;
        break;
    case 1:
        PREC(arg0).field_4 = arg2;
        break;
    case 2:
        if (D_8008FDBC & 1) {
            arg2 = 6;
        }
        PREC(arg0).field_8 = arg2;
        break;
    case 3:
        p = PPTR(arg0);
        p->field_8 += arg2;
        p->field_8 = (p->field_8 > 100) ? 100 : p->field_8;
        break;
    case 4:
        if (!(D_8008FDBC & 1)) {
            p = PPTR(arg0);
            p->field_8 -= arg2;
        }
        break;
    case 5:
        PREC(arg0).field_9 = arg2;
        break;
    case 6:
        p = PPTR(arg0);
        p->field_9 += arg2;
        p->field_9 = (p->field_9 > 99) ? 99 : p->field_9;
        break;
    case 7:
        p = PPTR(arg0);
        p->field_9 -= arg2;
        p->field_9 = (p->field_9 < 0) ? 0 : p->field_9;
        break;
    case 8:
        PREC(arg0).field_A = arg2;
        break;
    case 9:
        PREC(arg0).field_C = arg2;
        break;
    case 10:
        if (!D_800BE616 || !*(s8 *)&D_800E0C20) {
            GameB21B0PlayerRecordA *q = PPTR(arg0);
            q->field_C += arg2;
        }
        break;
    case 11:
        p = PPTR(arg0);
        p->field_C -= arg2;
        p->field_C = (p->field_C < 0) ? 0 : p->field_C;
        break;
    }
}

/* Fields recovered from the shared 0x1C-byte player records. */
typedef struct GameB21B0PlayerRecord {
    s32 field_0;
    s32 field_4;
    s8 field_8;
    s8 field_9;
    u8 field_A;
    u8 padB;
    s32 field_C;
    u8 pad10[0xC];
} GameB21B0PlayerRecord;

extern GameB21B0PlayerRecord D_800D213C[];

s32 func_150859AC(s16 arg0, s16 arg1) {
    if (arg0 >= 0xFF) {
        return 0;
    }
    switch (arg1) {
    case 0:
        return *(u16 *)&D_800D2340 & (1 << arg0);
    case 1:
        return D_800D213C[arg0].field_0;
    case 2:
        return D_800D213C[arg0].field_4;
    case 3:
        return D_800D213C[arg0].field_8;
    case 4:
        return D_800D213C[arg0].field_9;
    case 5:
        return D_800D213C[arg0].field_A;
    case 6:
        return D_800D213C[arg0].field_C;
    }
}

void func_15085ABC(s16 arg0) {
    s32 i;

    i = 0;
    if (D_800D18A8 == 0) {
        for (i = 0; i != 4; i++) {
            if ((1 << i) & arg0) {
                func_15085710((s16)i, 5, D_800CC2D0[i].health);
            }
        }
    }
}
