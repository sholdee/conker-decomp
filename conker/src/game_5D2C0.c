#include <ultra64.h>
#define func_15083568 func_15083568_orig
#include "functions.h"
#undef func_15083568
#include "variables.h"

extern f32 D_80097B68;
extern f32 D_800970DC;
extern u8 D_800BEA0C;
extern s32 D_800902BC[];
void func_1000FD38(void *, void *, s32);
s32 func_15033BDC();

typedef struct Game5D2C0RenderState {
    u8 pad_0[0x2];
    u8 field_0x02;
    u8 pad_0x03[0x13];
    u8 field_0x16;
    u8 pad_0x17[0x29];
    f32 field_0x40;
} Game5D2C0RenderState;

Game5D2C0RenderState *func_15083568(struct127 *, s32, f32, s32);
extern void func_150A7A48(f32 arg0[4][4], f32 arg1[4][4], f32 arg2[4][4]);
extern void func_1503E5F8();
extern void *allocate_memory(s32 size, s32 arg1, s32 arg2, s32 arg3);
extern void func_10004074(void *arg0);
void func_1518CA04(s32 arg0);
s32 func_1503F62C(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4, void *arg5, void *arg6);

typedef struct Game5D2C0EffectState {
    u8 pad_0[0x1];
    u8 field_0x01;
    u8 field_0x02;
    u8 pad_0x03[0x1F];
    s16 field_0x22;
    u8 pad_0x24[0x14];
    s32 field_0x38;
    s32 field_0x3C;
} Game5D2C0EffectState;

typedef struct Game5D2C0EventState {
    u8 pad_0[0x5];
    u8 field_0x05;
} Game5D2C0EventState;

typedef struct Game5D2C0FlagState {
    u8 pad_0[0x3];
    u8 field_0x03;
} Game5D2C0FlagState;

typedef struct Game5D2C0ActorState {
    u8 pad_0[0x11A];
    u8 field_0x11A;
    u8 pad_0x11B[0x81];
    u16 field_0x19C;
} Game5D2C0ActorState;

typedef struct Game5D2C0PositionState {
    s32 field_0x00;
    u8 pad_0x04[0x10];
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    u8 pad_0x20[0x64];
    u16 field_0x84;
    u8 pad_0x86[0x27];
    u8 field_0xAD;
    u8 pad_0xAE[0x6A];
    f32 field_0x118;
    u8 pad_0x11C[0x64];
    f32 field_0x180;
    u8 pad_0x184[0x198];
    Game5D2C0ActorState *field_0x31C;
} Game5D2C0PositionState;

typedef struct Game5D2C0AnimationState {
    u8 pad_0[0x4];
    u16 field_0x04;
    u8 pad_0x06[0x2];
    f32 field_0x08;
    u8 pad_0x0C[0xC];
    f32 field_0x18;
} Game5D2C0AnimationState;

typedef struct Game5D2C0AnimationOwner {
    u8 pad_0[0x48];
    struct197 *field_0x48;
} Game5D2C0AnimationOwner;

/* File header of an attachment model, as returned by func_1502B6BC. */
typedef struct ModelHeader {
    /* 0x00 */ Gfx **dlists;    // display list table (entries are relocated on first load)
    /* 0x04 */ u32 dlistsSize;  // size of the table in bytes
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 data[1];      // vertex data, up to the display list table
} ModelHeader;

extern u8 D_800DF7D0[];            // per-model load reference counts
extern s32 D_800DF9B8[];
extern ModelHeader *D_800E0148[];  // per-model loaded headers
ModelHeader *func_1502B6BC(s32, s32, s32, s32, s32, s32);
void func_1510CE60(Gfx *, s32, s32, s32, s32 *);
void func_15168E54(Gfx *, ModelHeader *);

/* Loads (or reuses) attachment model `model` and returns its display lists and vertex data. */
s32 func_1502FE10(s32 model, Gfx ***dlists, u8 *numDLists, u8 **data, s32 *arg4, s32 *arg5, s32 *arg6) {
    ModelHeader *header;
    s32 i;

    if (D_800DF7D0[model] == 0) {
        header = func_1502B6BC(0, 0, 0, 2, 9, model);
        if (header == NULL) {
            return 1;
        }
        *dlists = header->dlists;
        *numDLists = header->dlistsSize >> 2;
        for (i = 0; i < *numDLists; i++) {
            (*dlists)[i] = (Gfx *)((u8 *)(*dlists)[i] + (s32)header);
            func_1510CE60((*dlists)[i], 0, 1, 0x3E, &D_800DF9B8[model]);
            func_15168E54((*dlists)[i], header);
        }
        D_800E0148[model] = header;
    } else {
        header = D_800E0148[model];
        *dlists = header->dlists;
        *numDLists = header->dlistsSize >> 2;
    }
    D_800DF7D0[model]++;
    *data = header->data;
    if (arg4 != NULL) {
        *arg4 = header->unk10;
    }
    if (arg5 != NULL) {
        *arg5 = header->unk8;
        if (arg6 != NULL) {
            *arg6 = header->unkC >> 4;
        }
    }
    return 0;
}

/* Position and rotation offset from the attachment's bone (func_15030D54). */
typedef struct AttachmentOffset {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 rotX;
    /* 0x10 */ f32 rotY;
    /* 0x14 */ f32 rotZ;
} AttachmentOffset;

/* Animated-model instance driven by func_1503F5B8 (struct197 is a prefix of it). */
typedef struct AnimInstance {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ u16 anim;        // current animation; bit 15 is a flag
    /* 0x006 */ u8 pad6[0x2];
    /* 0x008 */ f32 frame;
    /* 0x00C */ u8 padC[0xC];
    /* 0x018 */ f32 numFrames;
    /* 0x01C */ u8 pad1C[0x1D];
    /* 0x039 */ s8 unk39;
    /* 0x03A */ s16 unk3A;
    /* 0x03C */ s16 unk3C;
    /* 0x03E */ u8 pad3E[0x215 - 0x3E];
    /* 0x215 */ u8 unk215;
    /* 0x216 */ u8 pad216[0x3E0 - 0x216];
    /* 0x3E0 */ u8 *unk3E0[2];   // per-frame matrices (indexed by D_800BE9C0)
    /* 0x3E8 */ u8 *unk3E8[2];
    /* 0x3F0 */ u8 pad3F0[6];
    /* 0x3F6 */ u8 unk3F6;
} AnimInstance;

/* A model attached to a bone of a game object. Allocated by func_15030AF4 and kept in the
 * doubly linked list headed by D_800C3EE0. */
typedef struct Attachment {
    /* 0x00 */ u8 ownerId;            // unique_id of the owning struct127
    /* 0x01 */ u8 model;              // model id; also selects the per-model hooks
    /* 0x02 */ u8 bone;               // owner joint matrix the model follows
    /* 0x03 */ u8 alpha;
    /* 0x04 */ u8 flags;
    /* 0x05 */ u8 updated;            // set by the update pass; not drawn before that
    /* 0x06 */ u8 setId;              // attachment set id (see func_15083568)
    /* 0x07 */ u8 handler;            // index into the D_800841C0 per-frame callbacks
    /* 0x08 */ u8 ownsVtx;            // vtx[] are private copies that must be freed
    /* 0x09 */ u8 loaded;
    /* 0x0A */ u16 ownerStates[3];    // if set, kept only while owner->unk84 is one of these
    /* 0x10 */ u8 pad10[3];
    /* 0x13 */ u8 hiddenDLists;       // bit i set: display list i is not drawn
    /* 0x14 */ u8 numDLists;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ s8 unk17;              // -1, or the animation set passed to func_1503F62C
    /* 0x18 */ u16 seg6Id;            // resolved by func_1510D0EC into segment 6
    /* 0x1A */ u16 seg7Id;            // resolved by func_1510D0EC into segment 7
    /* 0x1C */ u16 timer;             // frames left to live; 0xFFFF = forever
    /* 0x1E */ u16 parentSet;         // nonzero: follows a matrix of another attachment set
    /* 0x20 */ u16 parentMtx;
    /* 0x22 */ s16 rotZ;
    /* 0x24 */ Gfx **dlists;
    /* 0x28 */ void *vtx[2];          // segment 1 data, one per frame buffer
    /* 0x30 */ s32 unk30;
    /* 0x34 */ Mtx *mtx;              // two matrices, one per frame buffer
    /* 0x38 */ s32 unk38;             // per-model hook state
    /* 0x3C */ s32 unk3C;             // per-model hook state
    /* 0x40 */ f32 scale;
    /* 0x44 */ AttachmentOffset *offset;
    /* 0x48 */ AnimInstance *anim;
    /* 0x4C */ void *unk4C;
    /* 0x50 */ void *unk50;
    /* 0x54 */ struct Attachment *next;
    /* 0x58 */ struct Attachment *prev;
} Attachment; // size 0x5C

void func_15031C14(Attachment *att);
void func_1503F5B8(AnimInstance *anim, s32 arg1, s32 animId, f32 arg3, f32 arg4, s32 arg5);
void func_1503F7B8(AnimInstance *anim);

typedef struct Func15030158Arg0 {
    u8 unk0;
    u8 unk1;
    u8 pad2[6];
    u8 unk8;
    u8 unk9;
    u8 padA[0x1A];
    s32 unk24;
    void *unk28;
    void *unk2C;
    s32 unk30;
    void *unk34;
    u8 pad38[0xC];
    void *unk44;
    void *unk48;
    void *unk4C;
    void *unk50;
    struct Func15030158Arg0 *unk54;
    struct Func15030158Arg0 *unk58;
} Func15030158Arg0;

/* Loads the attachment's model, making private per-frame vertex copies when the owner needs them. */
s32 func_1502FFD8(Attachment *att, struct127 *owner) {
    u8 *data;
    s32 mode;
    s32 size;

    if (att->dlists == NULL) {
        if (att->unk17 == -1) {
            if (func_1502FE10(att->model, &att->dlists, &att->numDLists, &data, &att->unk30, NULL, NULL) != 0) {
                return 1;
            }
        } else {
            if (func_1503F62C(att->model, att->unk17, &att->dlists, &att->numDLists, &data, &att->unk30, &att->anim) != 0) {
                return 1;
            }
        }

        mode = owner->pad66 & 0xC;
        size = (u8 *)att->dlists - data;
        if (!(att->flags & 0x10) && (mode == 8) && (att->unk30 != 0)) {
            att->vtx[0] = allocate_memory(size, 1, 2, 2);
            if (att->vtx[0] == NULL) {
                return 1;
            }

            att->vtx[1] = allocate_memory(size, 1, 2, 2);
            if (att->vtx[1] == NULL) {
                func_10004074(att->vtx[0]);
                att->vtx[0] = NULL;
                return 1;
            }

            bcopy(data, att->vtx[0], size);
            bcopy(data, att->vtx[1], size);
            att->ownsVtx = 1;
        } else {
            att->vtx[0] = data;
            att->vtx[1] = data;
        }
        att->loaded = 1;
    }
    return 0;
}

/* Unlinks and frees an attachment; `immediate` frees its buffers now instead of deferring it. */
void func_15030158(Attachment *att, s32 immediate) {
    if (!immediate) {
        func_15031C14(att);
    }
    if (att->loaded) {
        if (att->ownsVtx) {
            if (immediate) {
                func_10004074(att->vtx[0]);
                func_10004074(att->vtx[1]);
            } else {
                func_100043B4(att->vtx[0], 4);
                func_100043B4(att->vtx[1], 4);
            }
        }
        if (att->anim != NULL) {
            func_1503F7B8(att->anim);
        }
        func_1518CA04(att->model);
    }
    if (att->mtx != NULL) {
        if (immediate) {
            func_10004074(att->mtx);
        } else {
            func_100043B4((s32 *)att->mtx, 4);
        }
    }
    if (att->offset != NULL) {
        AttachmentOffset *offset = att->offset; // matching: IDO loads it into v0 only via a local

        func_10004074(offset);
    }
    if (att->unk4C != NULL) {
        if (immediate) {
            func_10004074(att->unk4C);
        } else {
            func_100043B4(att->unk4C, 4);
        }
    }
    if (att->unk50 != NULL) {
        if (immediate) {
            func_10004074(att->unk50);
        } else {
            func_100043B4(att->unk50, 4);
        }
    }

    if (att == (Attachment *)D_800C3EE0) {
        D_800C3EE0 = (s32)att->next;
    }
    if (att->prev != NULL) {
        att->prev->next = att->next;
    }
    if (att->next != NULL) {
        att->next->prev = att->prev;
    }
    func_10004074(att);
}

s32 func_15030310(struct127 *owner, s32 setId, s32 index);

/* Removes every attachment of set `setId` from `owner`. */
s32 func_150302F0(struct127 *owner, s32 setId) {
    return func_15030310(owner, setId, 0xFF);
}

/* Removes attachment number `index` of set `setId` from `owner` (all of them when index is 0xFF). */
s32 func_15030310(struct127 *owner, s32 setId, s32 index) {
    Attachment *att;
    s32 removed;

    if (owner->unique_id == 0) {
        return 0;
    }
    removed = 0;
    att = (Attachment *)D_800C3EE0;
    while (att != NULL) {
        Attachment *next = att->next;

        if ((att->ownerId == owner->unique_id) && (att->setId == setId)) {
            if ((index != 0xFF) && (index != 0)) {
                index--;
                att = next;
                continue;
            } else {
                removed = 1;
                func_15030158(att, 0);
                if (index == 0) {
                    break;
                }
            }
        }
        att = next;
    }
    return removed;
}

/* Removes every attachment of `owner`. */
s32 func_150303E4(struct127 *owner) {
    Attachment *att;
    Attachment *next;
    s32 removed;

    if (owner->unique_id == 0) {
        return 0;
    }

    att = (Attachment *)D_800C3EE0;
    removed = 0;
    while (att != NULL) {
        next = att->next;
        if (att->ownerId == owner->unique_id) {
            removed = 1;
            func_15030158(att, 0);
        }
        att = next;
    }
    return removed;
}

typedef struct S15031070Sub {
    u8 pad0[0x3E0];
    u8 *unk3E0[2];
    u8 *unk3E8[2];
    u8 pad3F0[6];
    u8 unk3F6;
} S15031070Sub;

typedef struct S15031070 {
    u8 pad0[2];
    u8 unk2;
    u8 pad3[0x1B];
    u16 unk1E;
    u16 unk20;
    u8 pad22[0x12];
    u8 *unk34;
    u8 pad38[0x10];
    S15031070Sub *unk48;
} S15031070;

typedef struct S15030468Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} S15030468Sub;

typedef struct S15030468 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
    u16 unkA[3];
    u8 pad10[6];
    u8 unk16;
    u8 unk17;
    s16 unk18;
    s16 unk1A;
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    s16 unk22;
    u8 pad24[0x10];
    u8 *unk34;
    u8 pad38[8];
    f32 unk40;
    S15030468Sub *unk44;
    S15031070Sub *unk48;
    u8 pad4C[8];
    struct S15030468 *unk54;
} S15030468;

typedef s32 (*S15030468Func)(S15030468 *, struct127 *);
extern S15030468Func D_800841C0[];
s32 func_15031070(S15031070 *arg0, struct127 *arg1, u8 **arg2, u8 **arg3);
void func_1503F4B0(S15031070Sub *);

void func_15030468(void) {
    S15030468 *var;
    S15030468 *next;
    struct127 *obj;
    u16 t;
    s32 ret;
    s32 i;
    s32 j;
    f32 tmp[4][4];
    f32 m[4][4];
    f32 res[4][4];
    f32 (*mf)[4];
    Mtx *mtx;
    s32 kill;
    f32 mfbuf[4][4];
    s32 pass;
    s32 flag;
    f32 inv;
    u8 *sp198;
    u8 *sp194;
    f32 m2[4][4];
    f32 res2[4][4];
    f32 m3[4][4];
    f32 res3[4][4];

    flag = 0;
    for (pass = 0; pass < 2; pass++) {
        if (pass == 1 && flag == 0) {
            break;
        }
        var = (S15030468 *)D_800C3EE0;
        while (var != NULL) {
            next = var->unk54;
            ret = 0;
            if (pass == 0) {
                if (var->unk1E != 0) {
                    flag = 1;
                    var = next;
                    continue;
                }
            } else if (var->unk1E == 0) {
                var = next;
                continue;
            }
            obj = func_15083E90(var->unk0);
            var->unk5 = 1;
            if (obj == NULL) {
                func_15030158((Func15030158Arg0 *)var, 0);
                var = next;
                continue;
            }
            if (var->unk7 != 0) {
                ret = D_800841C0[var->unk7](var, obj);
            }
            if (ret != 0) {
                func_15030158((Func15030158Arg0 *)var, 0);
                var = next;
                continue;
            }
            if (var->unk34 != NULL || var->unk48 != NULL) {
                if (obj->unk1D4 != NULL) {
                    i = var->unk1E;
                    if (var->unk1E != 0) {
                        ret = (s32)func_1503195C(obj, i, 0);
                        if (ret == 0) {
                            func_15030158((Func15030158Arg0 *)var, 0);
                            var = next;
                            continue;
                        }
                        if (func_15031070((S15031070 *)ret, obj, &sp198, &sp194) == 0) {
                            func_15030158((Func15030158Arg0 *)var, 0);
                            var = next;
                            continue;
                        }
                        mf = mfbuf;
                        if (((S15031070 *)ret)->unk48 != NULL) {
                            sp198 += var->unk20 << 6;
                            guMtxL2F(mf, (Mtx *)sp198);
                            mtx = (Mtx *)mf;
                        } else {
                            guMtxL2F(mf, (Mtx *)sp198);
                        }
                    } else {
                        mf = (f32 (*)[4])((u8 *)obj->unk1D4 + (var->unk2 << 6));
                    }
                    if (var->unk34 != NULL) {
                        mtx = (Mtx *)var->unk34;
                        mtx += D_800BE9C0;
                        if (var->unk44 != NULL) {
                            for (i = 0; i < 3; i++) {
                                inv = 1.0f / sqrtf(mf[i][0] * mf[i][0] + mf[i][1] * mf[i][1] + mf[i][2] * mf[i][2]);
                                for (j = 0; j < 3; j++) {
                                    tmp[i][j] = mf[i][j] * inv;
                                }
                                tmp[i][3] = 0.0f;
                                tmp[3][i] = mf[3][i];
                            }
                            tmp[3][3] = 1.0f;
                            func_150A9B0C(m, var->unk44->unkC, var->unk44->unk10, var->unk44->unk14, var->unk40, var->unk40, var->unk40);
                            m[3][0] = var->unk44->unk0;
                            m[3][1] = var->unk44->unk4;
                            m[3][2] = var->unk44->unk8;
                            m[0][3] = 0.0f;
                            m[1][3] = 0.0f;
                            m[2][3] = 0.0f;
                            m[3][3] = 1.0f;
                            func_150A7A48(m, tmp, res);
                            guMtxF2L(res, mtx);
                        } else if (var->unk16 == 4) {
                            for (i = 0; i < 3; i++) {
                                inv = sqrtf(mf[i][0] * mf[i][0] + mf[i][1] * mf[i][1] + mf[i][2] * mf[i][2]);
                                if (i == 1) {
                                    inv = var->unk40 / inv;
                                } else {
                                    inv = var->unk40 / inv;
                                }
                                for (j = 0; j < 3; j++) {
                                    tmp[i][j] = mf[i][j] * inv;
                                }
                                tmp[i][3] = 0.0f;
                                tmp[3][i] = mf[3][i];
                            }
                            tmp[3][3] = 1.0f;
                            if (var->unk22 != 0) {
                                func_150A8050(m2, 0.0f, 0.0f, var->unk22 * 0.0054931640625f);
                                func_150A7A48(m2, tmp, res2);
                                guMtxF2L(res2, mtx);
                            } else {
                                guMtxF2L(tmp, mtx);
                            }
                        } else {
                            mf[0][3] = 0.0f;
                            mf[1][3] = 0.0f;
                            mf[2][3] = 0.0f;
                            mf[3][3] = 1.0f;
                            if (var->unk22 != 0) {
                                func_150A8050(m3, 0.0f, 0.0f, var->unk22 * 0.0054931640625f);
                                func_150A7A48(m3, mf, res3);
                                guMtxF2L(res3, mtx);
                            } else {
                                guMtxF2L(mf, mtx);
                            }
                        }
                    }
                } else {
                    mf = NULL;
                    mtx = NULL;
                }
            }
            if (var->unk48 != NULL) {
                if (var->unk34 != NULL) {
                    var->unk48->unk3E0[D_800BE9C0] = (u8 *)mtx;
                } else {
                    var->unk48->unk3E0[D_800BE9C0] = (u8 *)mf;
                }
                func_1503F4B0(var->unk48);
            }
            if (var->unkA[0] != 0) {
                kill = 1;
                for (i = 0; i < 3; i++) {
                    if (var->unkA[i] != 0 && obj->unk84.uh == var->unkA[i]) {
                        kill = 0;
                        break;
                    }
                }
                if (kill) {
                    func_15030158((Func15030158Arg0 *)var, 0);
                    var = next;
                    continue;
                }
            }
            if ((t = var->unk1C) != 0xFFFF) {
                if (t != 0) {
                    if (t > D_800BE9E4) {
                        var->unk1C = t - D_800BE9E4;
                    } else {
                        var->unk1C = 0;
                    }
                } else {
                    func_15030158((Func15030158Arg0 *)var, 0);
                }
            }
            var = next;
        }
    }
}

void func_15031A50(Attachment *att, struct127 *owner);

/* Attaches `model` to bone `bone` of `owner` as part of set `setId`. Returns NULL on failure, or
 * (unless flags & 8) when that model of that set is already attached. */
Attachment *func_15030AF4(struct127 *owner, s32 model, s32 bone, s32 flags, s32 arg4, s32 arg5, s32 lifetime,
                          s32 setId, s32 handler, u16 *ownerStates, s32 arg10, s32 parentSet, s32 parentMtx) {
    Attachment *att;
    Attachment *next;
    s32 err;

    if (owner->unique_id == 0) {
        return NULL;
    }
    if (!(flags & 8)) {
        att = (Attachment *)D_800C3EE0;
        if (att != NULL) {
            do {
                next = att->next;
                if ((att->ownerId == owner->unique_id) && (att->setId == setId) && (att->model == model)) {
                    return NULL;
                }
                att = next;
            } while (next != NULL);
        }
    }
    att = allocate_memory(sizeof(Attachment), 1, 0, 2);
    if (att == NULL) {
        return NULL;
    }
    err = 0;
    if ((Attachment *)D_800C3EE0 != NULL) {
        ((Attachment *)D_800C3EE0)->prev = att;
    }
    att->next = (Attachment *)D_800C3EE0;
    att->prev = NULL;
    D_800C3EE0 = (s32)att;
    att->ownerId = owner->unique_id;
    att->model = model;
    att->bone = bone;
    att->dlists = NULL;
    att->vtx[0] = NULL;
    att->vtx[1] = NULL;
    att->unk30 = 0;
    att->alpha = 0xFF;
    att->offset = NULL;
    att->flags = flags;
    att->setId = setId;
    att->scale = 1.0f;
    att->handler = handler;
    att->unk38 = 0;
    att->unk3C = 0;
    att->ownsVtx = 0;
    att->loaded = 0;
    att->seg6Id = 0;
    att->seg7Id = 0;
    att->ownerStates[0] = ownerStates[0];
    att->ownerStates[1] = ownerStates[1];
    att->ownerStates[2] = ownerStates[2];
    att->unk17 = arg10;
    att->anim = NULL;
    att->unk15 = arg4;
    att->unk16 = arg5;
    att->parentSet = parentSet;
    att->parentMtx = parentMtx;
    att->hiddenDLists = 0;
    att->unk4C = NULL;
    att->unk50 = NULL;
    att->updated = 0;
    att->rotZ = 0;
    if ((arg5 == 4) || (flags & 4)) {
        att->mtx = allocate_memory(2 * sizeof(Mtx), 1, 2, 2);
        if (att->mtx == NULL) {
            err = 1;
        }
    } else {
        att->mtx = NULL;
    }
    if (lifetime == 0) {
        att->timer = 0xFFFF;
    } else {
        att->timer = lifetime;
    }
    if (!err) {
        err |= func_1502FFD8(att, owner);
    }
    if (err) {
        func_15030158(att, 1);
        return NULL;
    }
    func_15031A50(att, owner);
    return att;
}

extern void *allocate_memory(s32 size, s32 arg1, s32 arg2, s32 arg3);
void func_15030D54(void *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6)
{
  f32 *temp;
  temp = allocate_memory(0x18, 1, 0, 2);
  *((f32 **) (((u8 *) arg0) + 0x44)) = temp;
  if (temp)
  {
    temp[((0, (*((s32 *) (((u8 *) arg0) + 0x34))) == 0)) * 0] = arg1;
    *((f32 *) ((*((s32 *) (((u8 *) arg0) + 0x44))) + 0x4)) = arg2;
    *((f32 *) ((*((s32 *) (((u8 *) arg0) + 0x44))) + 0x8)) = arg3;
    *((f32 *) ((*((s32 *) (((u8 *) arg0) + 0x44))) + 0xC)) = arg4;
    *((f32 *) ((*((s32 *) (((u8 *) arg0) + 0x44))) + 0x10)) = arg5;
    temp = ((u8 *) arg0) + 0x44;
    *((f32 *) ((*((s32 *) temp)) + 0x14)) = arg6;
    if (0, (*((s32 *) (((u8 *) arg0) + 0x34))) == 0)
    {
      *((void **) (((u8 *) arg0) + 0x34)) = allocate_memory(0x80, 1, 2, 2);
    }
  }
}


struct S15030F94 {
    u8 unk0;
    u8 pad1[2];
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 pad6[0xD];
    u8 unk13;
    u8 unk14;
    u8 pad15[3];
    u16 unk18;
    u16 unk1A;
    u8 pad1C[8];
    Gfx **unk24;
    Gfx *unk28[2];
    u8 pad30[0x24];
    struct S15030F94 *unk54;
};

s32 func_1506196C(struct127 *, s32);
Gfx *func_150311C4(Gfx *, struct S15030F94 *, struct127 *, s32, s32);

s32 func_15030E08(s32 arg0, s32 arg1, s32 arg2) {
    struct S15030F94 *var;
    struct S15030F94 *next;
    struct127 *obj;
    s32 v;

    var = (struct S15030F94 *)D_800C3EE0;
    while (var != NULL) {
        next = var->unk54;
        if (var->unk3 == 0) {
            var = next;
            continue;
        }
        obj = func_15083E90(var->unk0);
        if ((obj == NULL) || (obj->unk1D4 == NULL) || ((obj->unk74 & (1 << arg1)) == (1 << arg1))) {
            var = next;
            continue;
        }
        if (arg2 == 2) {
            if (obj->unk5 != 7) {
                var = next;
                continue;
            }
        } else {
            v = func_1506196C(obj, arg1);
            if (arg2 == 1) {
                if (!(var->unk4 & 2) && (v == 0xFF) && (obj->unk5 != 1)) {
                    var = next;
                    continue;
                }
            } else if ((var->unk4 & 2) || (v < 0xFF) || (obj->unk5 == 1)) {
                var = next;
                continue;
            }
        }
        arg0 = (s32)func_150311C4((Gfx *)arg0, var, obj, arg1, 0);
        var = next;
    }
    return arg0;

}


s32 func_15030F94(s32 arg0, struct127 *arg1, s32 arg2, s32 arg3) {
    struct S15030F94 *var;

    if ((arg1 == NULL) || (arg1->unk1D4 == NULL) || (arg1->unique_id == 0)) {
        return arg0;
    }
    var = (struct S15030F94 *)D_800C3EE0;
    while (var != NULL) {
        struct S15030F94 *next = var->unk54;

        if ((var->unk0 != arg1->unique_id) || (var->unk3 == 0)) {
            var = next;
            continue;
        }
        arg0 = (s32)func_150311C4((Gfx *)arg0, var, arg1, arg2, arg3);
        var = next;
    }
    return arg0;
}


s32 func_15031070(S15031070 *arg0, struct127 *arg1, u8 **arg2, u8 **arg3) {
    S15031070 *other;

    if (arg0->unk48 != NULL) {
        if (arg0->unk48->unk3F6 == 0) {
            return 0;
        }
        *arg2 = arg0->unk48->unk3E8[D_800BE9C0];
        *arg3 = arg0->unk48->unk3E0[D_800BE9C0];
    } else if (arg0->unk34 != NULL) {
        *arg2 = arg0->unk34 + (D_800BE9C0 << 6);
        *arg3 = (u8 *)arg1->unk1D4 + (arg0->unk2 << 6);
    } else if (arg0->unk1E) {
        other = (S15031070 *)func_1503195C(arg1, arg0->unk1E, 0);
        if (other == NULL) {
            return 0;
        }
        if (func_15031070(other, arg1, arg2, arg3) == 0) {
            return 0;
        }
        {
            s32 pad;
            *arg2 += arg0->unk20 << 6;
        }
        return 1;
    } else {
        *arg2 = (u8 *)arg1->unk1D4;
        *arg2 += arg0->unk2 << 6;
        *arg3 = *arg2;
    }
    return 1;
}

extern u8 D_80082FC0[];
extern u8 D_80083140[];
extern u8 D_800D9E28[];
extern u8 D_800DD2E4[];
extern Gfx *D_800BE9C8[];
extern s32 D_800BEBA4;
extern void func_1502D54C(s32, s32 *);
extern void func_1502D630(struct127 *, s32 *, s32, s32);
extern void func_1502CC34(struct127 *, s32, s32, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *);
extern void func_150832AC(struct127 *);
extern s32 func_1510D0EC(void *, s32 *, s32, s32);
extern Gfx *func_1515D914(Gfx *, s32, s32, s32, s32, s32, s32, s32, void *, void *, s32, void *, s32, void *);
extern Gfx *func_1515E544(Gfx *, s32, s32, s32, void *);
extern Gfx *func_15035D6C(Gfx *, struct S15030F94 *, s32, s32, s32);

#define OBJ_U8(o, off) (*(u8 *)((u8 *)(o) + (off)))
#define OBJ_S32(o, off) (*(s32 *)((u8 *)(o) + (off)))

Gfx *func_150311C4(Gfx *gdl, struct S15030F94 *arg1, struct127 *arg2, s32 arg3, s32 arg4) {
    u8 *sp114;
    u8 *sp110;
    s32 sp104[3];
    s32 sp100;
    s32 spFC;
    s32 spF8;
    s32 spF4;
    s32 spF0;
    s32 spEC;
    s32 spE8;
    s32 spE4;
    s32 spE0;
    s32 spDC;
    s32 spD8;
    s32 spD4;
    s32 spD0;
    s32 spBC[5];
    u16 t;
    Gfx *orig;
    s32 v;
    s32 doit;
    s32 called;

    orig = gdl;
    if (arg1->unk5 == 0) {
        return gdl;
    }
    gDPPipeSync(gdl++);
    spD0 = arg1->unk3;
    v = func_1506196C(arg2, arg3);
    if (v != 0xFF) {
        spD0 = (s32)((u32)v * spD0) >> 8;
    }
    if (spD0 < 0xFF) {
        gSPSegment(gdl++, 8, D_80082FC0);
    } else {
        gSPSegment(gdl++, 8, D_80083140);
    }
    if (func_15031070((S15031070 *)arg1, arg2, &sp114, &sp110) == 0) {
        return gdl;
    }
    func_1502D54C(arg2 - D_800CC2D0, spBC);
    if (((s16 *)D_800B0DF0)[0x3E / 2] != 0) {
        func_1502D630(&D_800CC2D0[arg2 - D_800CC2D0], spBC, arg3, arg2 - D_800CC2D0);
    } else {
        spBC[3] = 0xFF;
    }
    func_1502CC34(arg2, arg3, spD0, spBC, &spF8, &spF4, &spF0, &spEC, &spE8, &spE4, &spE0, &spDC, &spD8, &spD4);
    doit = 1;
    sp100 = ((s16 *)D_800B0DF0)[0x3E / 2];
    if ((OBJ_U8(arg2, 0x66) & 0xC) == 0xC) {
        gSPSetGeometryMode(gdl++, 0x20000);
        gSPClearGeometryMode(gdl++, 0x400000);
    } else if (((OBJ_U8(arg2, 0x66) & 0xC) != 8) && !(arg1->unk4 & 1)) {
        gSPClearGeometryMode(gdl++, 0x420000);
        doit = 0;
    } else if (D_800DCD7C != 0) {
        gSPSetGeometryMode(gdl++, 0x420000);
    } else {
        gSPClearGeometryMode(gdl++, 0x420000);
        doit = 0;
    }
    if (doit != 0) {
        if (OBJ_S32(arg2, arg3 * 4 + 0x304) == 0) {
            func_150832AC(arg2);
            called = 1;
        } else {
            called = 0;
        }
        if (OBJ_S32(arg2, arg3 * 4 + 0x304) != 0) {
            if ((OBJ_U8(arg2, 0x2FD) != 0) || (called != 0)) {
                v = 0xA;
            } else {
                v = 2;
            }
            if ((arg2->unk5 != 5) && (OBJ_U8(arg2, 0x302) != 0) && (OBJ_U8(arg2, 0x66) & 0xC) && (called == 0)) {
                gdl = func_1515E544(gdl, OBJ_S32(arg2, arg3 * 4 + 0x304), OBJ_U8(arg2, 0x301), OBJ_U8(arg2, 0x302),
                                    (u8 *)OBJ_S32(arg2, 0x314) + D_800BE9C0 * 8);
            } else {
                gdl = func_1515D914(gdl, arg3, (s32)arg2->x_position, (s32)arg2->y_position, (s32)arg2->z_position,
                                    OBJ_S32(arg2, 0x184), OBJ_S32(arg2, arg3 * 4 + 0x304), OBJ_U8(arg2, 0x301),
                                    (u8 *)OBJ_S32(arg2, 0x314) + D_800BE9C0 * 8, (u8 *)arg2 + 0x302, 2,
                                    (u8 *)arg2 + 0x1DD, v, D_800D9E28);
            }
        }
    }
    if (sp100 == 0) {
        gDPSetFogColor(gdl++, spE0, spDC, spD8, spD4);
    } else {
        gDPSetFogColor(gdl++, D_800DD2E4[0], D_800DD2E4[1], D_800DD2E4[2], spD4);
    }
    gDPSetFogColor(gdl++, spE0, spDC, spD8, spD4);
    gDPSetPrimColor(gdl++, 0xF2, 0, spF8, spF4, spF0, 0);
    gDPSetEnvColor(gdl++, spEC, spE8, spE4, spD0);
    gSPSegment(gdl++, 3, sp114);
    gSPSegment(gdl++, 1, arg1->unk28[D_800BE9C0]);
    t = arg1->unk18;
    if (t != 0) {
        v = func_1510D0EC(t, sp104, 0x3E, 0);
        gSPSegment(gdl++, 6, v);
    }
    if (arg1->unk1A != 0) {
        v = func_1510D0EC(arg1->unk1A, sp104, 0x3E, 0);
        gSPSegment(gdl++, 7, v);
    }
    if (arg4 == 0) {
        gSPSetGeometryMode(gdl++, 0x400);
        for (called = 0; called < arg1->unk14; called++) {
            if (!(arg1->unk13 & (1 << called))) {
                gSPDisplayList(gdl++, arg1->unk24[called]);
            }
        }
    } else {
        gSPSetGeometryMode(gdl++, 0x200);
        gdl = func_15035D6C(gdl, arg1, spEC, spE8, spE4);
    }
    if (D_800BEBA4 < (gdl - D_800BE9C8[D_800BE9C0])) {
        v = 1;
    } else {
        v = 0;
    }
    if (v != 0) {
        return orig;
    }
    return gdl;
}

/* Sets the alpha of attachment number `index` of set `setId` on `owner`. */
void func_1503192C(struct127 *owner, s32 setId, s32 alpha, s32 index) {
    Attachment *att;

    att = (Attachment *)func_1503195C(owner, setId, index);
    if (att != NULL) {
        att->alpha = alpha;
    }
}

/* Returns attachment number `index` of set `setId` on `owner`, or NULL. */
struct126 *func_1503195C(struct127 *owner, s32 setId, s32 index) {
    s32 ownerId;
    Attachment *att;
    Attachment *next;

    ownerId = owner->unique_id;
    if (ownerId == 0) {
        return NULL;
    }

    att = (Attachment *)D_800C3EE0;
    while (att != NULL) {
        next = att->next;
        if ((ownerId == att->ownerId) && (setId == att->setId)) {
            if (index != 0) {
                index--;
                att = next;
            } else {
                return (struct126 *)att;
            }
        } else {
            att = next;
        }
    }
    return NULL;
}

/* Returns the first attachment of set `setId`, preferring one owned by `owner`. */
Attachment *func_150319CC(s32 setId, struct127 *owner) {
    Attachment *att;
    Attachment *next;
    u8 ownerId;

    if (owner != NULL) {
        att = (Attachment *)D_800C3EE0;
        if (att != NULL) {
            ownerId = owner->unique_id;
            do {
                next = att->next;
                if ((att->ownerId == ownerId) && (att->setId == setId)) {
                    return att;
                }
                att = next;
            } while (att != NULL);
        }
    }
    att = (Attachment *)D_800C3EE0;
    while (att != NULL) {
        next = att->next;
        if (att->setId == setId) {
            return att;
        }
        att = next;
    }
    return NULL;
}

void func_150D5440(void *, u8, s32);
void func_150D3360(void *, u8, s32);
void func_151001B4();
void func_15163BE8(void *, u8, s32);
void func_151BD828(void *, u8, s32);
void func_151D74B0(void *, u8, s8, u8, s32);
extern s32 D_80090228;
extern s32 D_8009022C;

/* Applies a model's side effects to its owner when it is attached (undone by func_15031C14). */
void func_15031A50(Attachment *att, struct127 *owner) {
    s32 model = att->model;
    struct126 *unk;

    switch (model) {
    case 0x37:
        func_151001B4(owner);
        break;
    case 0x5A:
        unk = owner->unk31C;
        if (unk != NULL) {
            *(u16 *)((u8 *)unk + 0x1A6) += 0xAA; // not covered by struct126 yet
        }
        break;
    case 0x90:
        owner->unk9C |= 0x70;
        break;
    case 0x8F:
        owner->unk9C |= 0xE00;
        break;
    case 0x49:
        func_15163BE8(owner, 0xFF, 1);
        break;
    case 0x5D:
        func_150D3360(owner, 0xFF, 1);
        func_150D5440(owner, 0xFF, 1);
        break;
    case 0x3D:
        func_151BD828(owner, 0xFF, 1);
        break;
    case 0x1D:
        func_151D74B0(owner, 0, 2, 0xFF, 1);
        break;
    case 0x5E:
    case 0x85:
        owner->unk9C |= 0x6000;
        break;
    case 0x8D:
        if (func_150859AC(0, 6) < 100) {
            att->seg6Id = D_80090228;
        } else {
            att->seg6Id = D_8009022C;
        }
        break;
    case 0x82:
        func_151D74B0(owner, 6, -1, 0xFF, 1);
        break;
    }
}

/* An object pointer plus the unique_id it had, passed to event handlers. */
typedef struct ObjectRef {
    struct127 *obj;
    u8 uniqueId;
} ObjectRef;

void func_15100180(struct127 *);
void func_151616D0(u8, u8, ObjectRef *);
void func_15147D64(ObjectRef *, s32);
void func_151BD7F4(struct127 *);
void func_151D4668(struct127 *);
void func_151D747C(struct127 *);
void func_151027E8(struct127 *);

/* Undoes func_15031A50's side effects on the owner when an attachment is removed. */
void func_15031C14(Attachment *att) {
    struct127 *owner;
    ObjectRef ref49;
    ObjectRef ref5D;
    struct126 *unk;
    ObjectRef *volatile ref; // matching: IDO spills the pointer across the call only when volatile

    owner = func_15083E90(att->ownerId);
    if (owner != NULL) {
        switch (att->model) {
        case 0x5A:
            unk = owner->unk31C;
            if (unk != NULL) {
                *(u16 *)((u8 *)unk + 0x1A6) -= 0xAA; // not covered by struct126 yet
            }
            break;
        case 0x90:
            owner->unk9C &= ~0x70;
            break;
        case 0x8F:
            owner->unk9C &= ~0xE00;
            break;
        case 0x37:
        case 0x4B:
        case 0x4C:
            func_1000FD38(func_15033BDC, att, (s32)owner);
            if (att->model == 0x37) {
                func_15100180(owner);
            }
            break;
        case 0x49:
            ref49.obj = owner;
            ref49.uniqueId = owner->unique_id;
            func_151616D0(0x10, 0x29, &ref49);
            break;
        case 0x5D:
            ref5D.obj = owner;
            ref5D.uniqueId = owner->unique_id;
            func_15147D64(ref = &ref5D, 0x2E);
            func_151494E0((s32)ref, 0x2F);
            break;
        case 0x3D:
            func_151BD7F4(owner);
            break;
        case 0x1A:
        case 0x1B:
        case 0x5F:
        case 0x65:
        case 0x66:
            func_151D4668(owner);
            break;
        case 0x1D:
        case 0x82:
            func_151D747C(owner);
            break;
        case 0x5E:
        case 0x85:
            owner->unk9C &= ~0x6000;
            break;
        }
        switch (att->setId) {
        case 0x16:
        case 0x63:
        case 0x89:
            func_151027E8(owner);
            func_151D4668(owner);
            break;
        }
    }
}

/* Hook: cycles seg6Id back and forth through the three D_800902BC entries, one step per call. */
s32 func_15031E2C(Attachment *att, struct127 *owner) {
    s32 step;

    step = att->unk38;
    if (step >= 3) {
        step = 5 - step;
    }
    att->seg6Id = D_800902BC[step];
    att->unk38++;
    if (att->unk38 >= 6) {
        att->unk38 = 0;
    }
    return 0;
}

/* First byte of a display list command, read signed as the game does. */
#define GFX_CMD(gfx) (*(s8 *)(gfx))

/* Hook: sets the tile t of the first display list's 4th G_SETTILESIZE from the owner's
 * animation frame (0..120 maps to 27..2). */
s32 func_15031E7C(Attachment *att, struct127 *owner) {
    struct197 *anim;
    Gfx *gfx;
    s32 count;
    s32 i;
    f32 t;

    anim = owner->unk2D0;
    if (anim == NULL) {
        return 0;
    }

    gfx = att->dlists[0];
    if (gfx == NULL) {
        return 0;
    }

    if (owner->unk84.uh == 0x55) {
        t = 1.0f;
    } else if (owner->unk84.uh == 0x56) {
        t = 0.0f;
    } else if ((0.0f <= anim->unk8) && (anim->unk8 <= 120.0f)) {
        t = anim->unk8 * 0.008333333768f;
        t = 1.0f - t;
    } else {
        t = 0.0f;
    }

    i = 0;
    count = 4;
    do {
        count--;
        while (GFX_CMD(&gfx[i]) != (s8)G_SETTILESIZE) {
            i++;
        }
        if (count != 0) {
            i++;
        }
    } while (count != 0);

    gfx[i].words.w0 = (G_SETTILESIZE << 24) | (2 << 12) | ((s32)((25.0f * t) + 2.0f) & 0xFFF);
    return 0;
}

typedef struct F15031FC8Anim {
    u8 pad0[0x4];
    u16 unk4;
    u8 pad6[0x2];
    f32 unk8;
    u8 padC[0xC];
    f32 unk18;
    u8 pad1C[0x1D];
    s8 unk39;
    s16 unk3A;
    s16 unk3C;
    u8 pad3E[0x215 - 0x3E];
    u8 unk215;
} F15031FC8Anim;

typedef struct F15031FC8Arg0 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3[0x15];
    s16 unk18;
    u8 pad1A[0x4];
    s16 unk1E;
    s16 unk20;
    u8 pad22[0x26];
    F15031FC8Anim *unk48;
} F15031FC8Arg0;

typedef struct F15031FC8Arg1 {
    u8 pad0[0x4];
    u8 unk4;
    u8 unk5;
    u8 pad6[0x1E];
    f32 unk24;
    u8 pad28[0x14];
    f32 unk3C;
    u8 pad40[0x44];
    u16 unk84;
    u8 pad86[0x2D0 - 0x86];
    F15031FC8Anim *unk2D0;
    u8 pad2D4[0x14];
    s32 unk2E8;
} F15031FC8Arg1;

extern s32 D_800902D0;
extern s32 D_800902D4;
extern void func_1505E060(F15031FC8Anim *arg0);

s32 func_15031FC8(F15031FC8Arg0 *arg0, F15031FC8Arg1 *arg1) {
    F15031FC8Anim *sub;
    s32 anim;
    u8 pad1;
    s32 id;
    F15031FC8Anim *a;
    s32 cur;
    f32 lim;
    s32 pad;
    s32 flag;

    sub = arg1->unk2D0;
    anim = -1;
    a = arg0->unk48;
    if (a == NULL) {
        return 0;
    }
    cur = a->unk4 & ~0x8000;
    id = arg1->unk84;
    flag = 1;
    switch (arg1->unk4) {
    case 0x58:
        if (arg0->unk1 == 0x9B) {
            arg0->unk2 = 0x12;
        }
        switch (id) {
        case 39:
            anim = 3;
            break;
        case 40:
            anim = 4;
            break;
        case 2:
            anim = 1;
            break;
        case 3:
            anim = 0;
            break;
        default:
            anim = 0;
            if (arg0->unk1 == 0x9B) {
                anim = 3;
            }
            break;
        }
        break;
    case 0x5B:
        anim = 0;
        break;
    case 0x5A:
    case 0x74:
    case 0x7A:
        if (arg1->unk2E8 != 0) {
            arg0->unk18 = D_800902D4;
        } else {
            arg0->unk18 = D_800902D0;
        }
        switch (id) {
        case 6:
            anim = 7;
            break;
        case 38:
        case 39:
            anim = 0xF;
            break;
        case 1:
        case 42:
            anim = 3;
            break;
        case 0:
            anim = 5;
            break;
        case 3:
            anim = 6;
            break;
        case 7:
            anim = 2;
            break;
        case 10:
            anim = 4;
            break;
        case 15:
        case 30:
            anim = 8;
            break;
        case 19:
            anim = 0xD;
            break;
        case 4:
            anim = 9;
            break;
        case 5:
            anim = 0xA;
            break;
        case 16:
            anim = 0xB;
            break;
        case 8:
            anim = 0x10;
            break;
        case 20:
            anim = 0xE;
            break;
        case 21:
            anim = 0xE;
            break;
        case 0x50:
            anim = 0x13;
            break;
        case 0x51:
            anim = 0x14;
            break;
        default:
            anim = 6;
            break;
        }
        break;
    default:
        switch (arg0->unk1) {
        case 0x9E:
            arg0->unk2 = 9;
            arg0->unk1E = 0x41;
            arg0->unk20 = 1;
            switch (id) {
            case 0x1D6:
                anim = 1;
                break;
            case 0x1D7:
                anim = 2;
                arg0->unk2 = 0x13;
                break;
            case 0x257:
                anim = 4;
                arg0->unk2 = 0x13;
                break;
            case 0x275:
                anim = 5;
                arg0->unk2 = 0x13;
                break;
            case 0x1D8:
                anim = 3;
                break;
            }
            flag = 0;
            if (arg0->unk2 == 0x13) {
                arg0->unk1E = 0;
            }
            break;
        case 0x9B:
            anim = 0;
            if (arg1->unk5 == 5) {
                arg0->unk2 = 0;
            } else if (arg1->unk4 == 0x8B) {
                arg0->unk2 = 6;
                switch (id) {
                case 12:
                    anim = 6;
                    break;
                case 19:
                    anim = 7;
                    break;
                }
            } else {
                arg0->unk2 = 9;
                switch (id) {
                case 0x22E:
                    anim = 5;
                    break;
                case 0x1B7:
                    anim = 1;
                    break;
                case 0x1B8:
                    anim = 2;
                    break;
                }
            }
            break;
        case 0x9F:
            anim = 0;
            if (arg1->unk5 == 5) {
                arg0->unk2 = 0;
            } else if (arg1->unk4 == 0xB5) {
                arg0->unk2 = 0x12;
                switch (id) {
                case 0x34:
                    anim = 1;
                    break;
                case 0x35:
                    anim = 2;
                    break;
                case 0x36:
                    anim = 3;
                    break;
                case 0x37:
                    anim = 4;
                    break;
                }
            } else {
                arg0->unk2 = 9;
            }
            break;
        case 0x9C:
            anim = 8;
            break;
        case 0x9D:
            anim = 0;
            if (arg1->unk5 == 5) {
                arg0->unk2 = 0;
            } else if (arg1->unk4 == 0x8B) {
                arg0->unk2 = 6;
                switch (id) {
                case 19:
                case 32:
                    anim = 4;
                    break;
                case 34:
                    anim = 3;
                    break;
                }
            } else {
                arg0->unk2 = 9;
                switch (id) {
                case 0x21C:
                    anim = 1;
                    break;
                case 0x241:
                    anim = 2;
                    break;
                }
            }
            break;
        case 0x9A:
            switch (arg1->unk4) {
            case 0x87:
                arg0->unk2 = 0x12;
                switch (id) {
                case 1:
                    anim = 1;
                    break;
                case 5:
                    anim = 2;
                    break;
                default:
                    anim = 0;
                    break;
                }
                break;
            case 0x99:
                arg0->unk2 = 0x15;
                switch (id) {
                case 20:
                    anim = 3;
                    break;
                case 21:
                    anim = 4;
                    break;
                case 22:
                    anim = 5;
                    break;
                case 23:
                    anim = 6;
                    break;
                case 24:
                    anim = 7;
                    break;
                case 26:
                    anim = 8;
                    break;
                case 27:
                    anim = 9;
                    break;
                case 28:
                    anim = 0xA;
                    break;
                case 29:
                    anim = 0xB;
                    break;
                case 30:
                    anim = 0xC;
                    break;
                case 31:
                    anim = 0xD;
                    break;
                case 32:
                    anim = 0xE;
                    break;
                case 33:
                    anim = 0xF;
                    break;
                case 34:
                    anim = 0x10;
                    break;
                case 35:
                    anim = 0x11;
                    break;
                default:
                    anim = 0;
                    break;
                }
                break;
            }
            break;
        case 0x8D:
            switch (id) {
            case 0x323:
                anim = 1;
                break;
            case 0x324:
                anim = 2;
                break;
            default:
                anim = 0;
                break;
            }
            break;
        case 0x8E:
            switch (id) {
            case 0xB4:
                anim = 0;
                break;
            case 0xB6:
                anim = 1;
                break;
            case 0xB5:
                anim = 2;
                break;
            case 0xDD:
                anim = 3;
                break;
            case 0xDE:
                anim = 3;
                break;
            default:
                return 1;
            }
            break;
        case 0x8F:
        case 0x90:
            switch (id) {
            case 0xB9:
                anim = 1;
                break;
            case 0xAE:
                anim = 2;
                break;
            case 0xDA:
                anim = 3;
                break;
            case 0x104:
                anim = 4;
                break;
            case 0x107:
                anim = 5;
                break;
            case 0x10B:
                anim = 6;
                break;
            case 0x11E:
                anim = 7;
                break;
            case 0x11F:
                anim = 8;
                break;
            case 0x134:
                anim = 9;
                break;
            case 0x135:
                anim = 0xA;
                break;
            case 0x136:
                anim = 0xB;
                break;
            case 0x13B:
                anim = 0xD;
                break;
            case 0x13C:
                anim = 0xE;
                break;
            case 0x145:
                anim = 0xF;
                break;
            case 0x146:
                anim = 0x10;
                break;
            case 0x141:
                anim = 0x11;
                break;
            case 0x147:
                anim = 0x12;
                break;
            case 0x14B:
                anim = 0x16;
                break;
            case 0x14A:
                anim = 0x15;
                break;
            case 0x149:
                anim = 0x14;
                break;
            case 0x148:
                anim = 0x13;
                break;
            case 0x151:
                anim = 0x18;
                break;
            case 0x17F:
                anim = 0x1A;
                break;
            case 0x180:
                anim = 0x1B;
                break;
            case 0x181:
                anim = 0x1C;
                break;
            case 0x182:
                anim = 0x1D;
                break;
            case 0x186:
                anim = 0x1E;
                break;
            case 0x187:
                anim = 0x1F;
                break;
            case 0x189:
                anim = 0x20;
                break;
            case 0x18C:
                anim = 0x21;
                break;
            case 0x18D:
                anim = 0x22;
                break;
            case 0x18E:
                anim = 0x23;
                break;
            case 0x1A0:
                anim = 0x24;
                break;
            case 0x1A3:
                anim = 0x25;
                break;
            case 0x1B3:
                anim = 0x26;
                break;
            case 0x1B4:
                anim = 0x27;
                break;
            case 0x1B5:
                anim = 0x28;
                break;
            case 0x1B6:
                anim = 0x29;
                break;
            case 0x1B7:
                anim = 0x2A;
                break;
            case 0x1B8:
                anim = 0x2B;
                break;
            case 0x1BD:
                anim = 0x2C;
                break;
            case 0x1BE:
                anim = 0x2D;
                break;
            case 0x1BF:
                anim = 0x2E;
                break;
            case 0x1C0:
                anim = 0x2F;
                break;
            case 0x1C1:
                anim = 0x30;
                break;
            case 0x1C2:
                anim = 0x31;
                break;
            case 0x1C3:
                anim = 0x32;
                break;
            case 0x1C4:
                anim = 0x33;
                break;
            case 0x1C5:
                anim = 0x34;
                break;
            case 0x1C6:
                anim = 0x35;
                break;
            case 0x1C7:
                anim = 0x36;
                break;
            case 0x1C8:
                anim = 0x37;
                break;
            case 0x1C9:
                anim = 0x38;
                break;
            case 0x1CA:
                anim = 0x39;
                break;
            case 0x1D1:
                anim = 0x3A;
                break;
            case 0x1D2:
                anim = 0x3B;
                break;
            case 0x1D3:
                anim = 0x3C;
                break;
            case 0x1D5:
                anim = 0x3D;
                break;
            case 0x1D9:
                anim = 0x3E;
                break;
            case 0x1DA:
                anim = 0x3F;
                break;
            case 0x1DB:
                anim = 0x40;
                break;
            case 0x1E3:
                anim = 0x41;
                break;
            case 0x1E4:
                anim = 0x42;
                break;
            case 0x1E5:
                anim = 0x43;
                break;
            case 0x1E6:
                anim = 0x44;
                break;
            case 0x1E7:
                anim = 0x45;
                break;
            case 0x1E8:
                anim = 0x47;
                break;
            case 0x1E9:
                anim = 0x44;
                break;
            case 0x1F0:
                anim = 0x48;
                break;
            case 0x1F1:
                anim = 0x49;
                break;
            case 0x1F2:
                anim = 0x4A;
                break;
            case 0x201:
                anim = 0x4B;
                break;
            case 0x202:
                anim = 0x4D;
                break;
            case 0x203:
                anim = 0x4E;
                break;
            case 0x204:
                anim = 0x4F;
                break;
            case 0x200:
                anim = 0x50;
                break;
            case 0x205:
                anim = 0x51;
                break;
            case 0x208:
                anim = 0x52;
                break;
            case 0x209:
                anim = 0x53;
                break;
            case 0x20A:
                anim = 0x54;
                break;
            case 0x20B:
                anim = 0x55;
                break;
            case 0x20C:
                anim = 0x56;
                break;
            case 0x211:
                anim = 0x57;
                break;
            case 0x212:
                anim = 0x58;
                break;
            case 0x216:
                anim = 0x59;
                break;
            case 0x217:
                anim = 0x5A;
                break;
            case 0x218:
                anim = 0x5B;
                break;
            case 0x219:
                anim = 0x5C;
                break;
            case 0x21A:
                anim = 0x5D;
                break;
            case 0x21B:
                anim = 0x5E;
                break;
            case 0xA8:
                anim = 0x5F;
                break;
            case 0x1FD:
                anim = 0x4C;
                break;
            case 0x228:
                anim = 0x62;
                break;
            case 0x22E:
                anim = 0x63;
                break;
            case 0x22F:
                anim = 0x64;
                break;
            case 0x232:
                anim = 0x61;
                break;
            case 0x220:
                anim = 0x60;
                break;
            case 0x23A:
                anim = 0x65;
                break;
            case 0x23B:
                anim = 0x66;
                break;
            case 0x24D:
                anim = 0x6E;
                break;
            case 0x21C:
                anim = 0x6F;
                break;
            case 0x241:
                anim = 0x70;
                break;
            case 0x23C:
                anim = 0x67;
                break;
            case 0x24B:
                anim = 0x6D;
                break;
            case 0x24A:
                anim = 0x6C;
                break;
            case 0x249:
                anim = 0x6B;
                break;
            case 0x248:
                anim = 0x6A;
                break;
            case 0x246:
                anim = 0x69;
                break;
            case 0x245:
                anim = 0x68;
                break;
            case 0x25C:
                anim = 0x71;
                break;
            case 0x25D:
                anim = 0x72;
                break;
            case 0x25E:
                anim = 0x73;
                break;
            case 0x260:
                anim = 0x74;
                break;
            case 0x266:
                anim = 0x77;
                break;
            case 0x267:
                anim = 0x78;
                break;
            case 0x268:
                anim = 0x79;
                break;
            case 0x26A:
                anim = 0x7A;
                break;
            case 0x26B:
                anim = 0x7B;
                break;
            case 0x26C:
                anim = 0x7C;
                break;
            case 0x26D:
                anim = 0x7D;
                break;
            case 0x275:
                anim = 0x7E;
                break;
            case 0x1D6:
                anim = 0x7F;
                break;
            case 0x1D7:
                anim = 0x86;
                break;
            case 0x1D8:
                anim = 0x85;
                break;
            case 0x276:
                anim = 0x80;
                break;
            case 0x277:
                anim = 0x81;
                break;
            case 0x278:
                anim = 0x82;
                break;
            case 0x265:
                anim = 0x76;
                break;
            case 0x27F:
                anim = 0x83;
                break;
            case 0x280:
                anim = 0x84;
                break;
            case 0x281:
                anim = 0x87;
                break;
            case 0x282:
                anim = 0x88;
                break;
            case 0x290:
                anim = 0x89;
                break;
            case 0x291:
                anim = 0x8A;
                break;
            case 0x292:
                anim = 0x8B;
                break;
            case 0x293:
                anim = 0x8C;
                break;
            case 0x294:
                anim = 0x8D;
                break;
            case 0x295:
                anim = 0x8E;
                break;
            case 0x298:
                anim = 0x8F;
                break;
            case 0x29A:
                anim = 0x90;
                break;
            case 0x29B:
                anim = 0x91;
                break;
            case 0x29C:
                anim = 0x92;
                break;
            case 0x29D:
                anim = 0x93;
                break;
            case 0x2A9:
                anim = 0x94;
                break;
            case 0x2AA:
                anim = 0x95;
                break;
            case 0x2AC:
                anim = 0x96;
                break;
            case 0x2AD:
                anim = 0x97;
                break;
            case 0x2AE:
                anim = 0x98;
                break;
            case 0x2AF:
                anim = 0x99;
                break;
            case 0x2B0:
                anim = 0x9A;
                break;
            case 0x2B2:
                anim = 0x9B;
                break;
            case 0x2B3:
                anim = 0x9C;
                break;
            case 0x2B4:
                anim = 0x9D;
                break;
            case 0x2B5:
                anim = 0x9E;
                break;
            case 0x2B6:
                anim = 0x9F;
                break;
            case 0x2B8:
                anim = 0xA0;
                break;
            case 0x2B9:
                anim = 0xA1;
                break;
            case 0x2BA:
                anim = 0xA2;
                break;
            case 0x2BB:
                anim = 0xA3;
                break;
            case 0x2BC:
                anim = 0xA4;
                break;
            case 0x2BD:
                anim = 0xA5;
                break;
            case 0x2BE:
                anim = 0xA6;
                break;
            case 0x2BF:
                anim = 0xA7;
                break;
            case 0x2C0:
                anim = 0xA8;
                break;
            case 0x2C1:
                anim = 0xA9;
                break;
            case 0x2C4:
                anim = 0xAA;
                break;
            case 0x2CF:
                anim = 0xAB;
                break;
            case 0x2DA:
                anim = 0xAC;
                break;
            case 0x2DB:
                anim = 0xAD;
                break;
            case 0x2DD:
                anim = 0xAE;
                break;
            case 0x2DC:
                anim = 0xAF;
                break;
            case 0x2D9:
                anim = 0xB0;
                break;
            case 0x2DF:
                anim = 0xB1;
                break;
            case 0x2E0:
                anim = 0xB2;
                break;
            case 0x2E1:
                anim = 0xB3;
                break;
            case 0x2E2:
                anim = 0xB4;
                break;
            case 0x2E3:
                anim = 0xB5;
                break;
            case 0x2EC:
                anim = 0xB6;
                break;
            case 0x2ED:
                anim = 0xB7;
                break;
            case 0x2EE:
                anim = 0xB8;
                break;
            case 0x2EF:
                anim = 0xB9;
                break;
            case 0x2F0:
                anim = 0xBA;
                break;
            case 0x2F1:
                anim = 0xBB;
                break;
            case 0x2F8:
                anim = 0xBC;
                break;
            case 0x2F9:
                anim = 0xBD;
                break;
            case 0x301:
                anim = 0xBE;
                break;
            case 0x304:
                anim = 0xBF;
                break;
            case 0x305:
                anim = 0xC0;
                break;
            case 0x306:
                anim = 0xC1;
                break;
            case 0x307:
                anim = 0xC2;
                break;
            case 0x308:
                anim = 0xC3;
                break;
            case 0x309:
                anim = 0xC4;
                break;
            case 0x30A:
                anim = 0xC5;
                break;
            case 0x30B:
                anim = 0xC6;
                break;
            case 0x30C:
                anim = 0xC7;
                break;
            case 0x30D:
                anim = 0xC8;
                break;
            case 0x30E:
                anim = 0xC9;
                break;
            case 0x30F:
                anim = 0xCA;
                break;
            case 0x314:
                anim = 0xCB;
                break;
            case 0x315:
                anim = 0xCC;
                break;
            case 0x316:
                anim = 0xCD;
                break;
            case 0x317:
                anim = 0xCE;
                break;
            case 0x318:
                anim = 0xCF;
                break;
            case 0x319:
                anim = 0xD0;
                break;
            case 0x31C:
                anim = 0xD1;
                break;
            case 0x31D:
                anim = 0xD2;
                break;
            case 0x31E:
                anim = 0xD3;
                break;
            case 0x320:
                anim = 0xD4;
                break;
            case 0x31F:
                anim = 0xD5;
                break;
            case 0x321:
                anim = 0xD6;
                break;
            case 0x322:
                anim = 0xD7;
                break;
            case 0x323:
                anim = 0xD8;
                break;
            case 0x324:
                anim = 0xD9;
                break;
            case 0x326:
                anim = 0xDA;
                break;
            case 0x327:
                anim = 0xDB;
                break;
            case 0x328:
                anim = 0xDC;
                break;
            case 0x32B:
                anim = 0xDD;
                break;
            case 0x32C:
                anim = 0xDE;
                break;
            case 0x32D:
                anim = 0xDF;
                break;
            case 0x32E:
                anim = 0xE0;
                break;
            case 0x32F:
                anim = 0xE1;
                break;
            case 0x330:
                anim = 0xE2;
                break;
            case 0x331:
                anim = 0xE3;
                break;
            case 0x332:
                anim = 0xE4;
                break;
            case 0x333:
                anim = 0xE5;
                break;
            case 0x334:
                anim = 0xE6;
                break;
            case 0x335:
                anim = 0xE7;
                break;
            case 0x345:
                anim = 0xE8;
                break;
            case 0x347:
                anim = 0xEA;
                break;
            case 0x348:
                anim = 0xEB;
                break;
            case 0x349:
                anim = 0xEC;
                break;
            case 0x34A:
                anim = 0xED;
                break;
            case 0x34B:
                anim = 0xEE;
                break;
            case 0x34C:
                anim = 0xEF;
                break;
            case 0x34D:
                anim = 0xF0;
                break;
            case 0x240:
                anim = 0xE9;
                break;
            default:
                anim = 0;
                break;
            }
            break;
        case 0x83:
            switch (id) {
            case 0x5E:
                anim = 0;
                break;
            case 0x5F:
            case 0xFA:
                anim = 1;
                break;
            case 0x60:
                anim = 2;
                break;
            case 0xB3:
                anim = 3;
                break;
            default:
                return 1;
            }
            break;
        case 0x85:
            if (id == 0x13E) {
                anim = 5;
            } else if (id == 0x13F) {
                anim = 4;
            } else if (id == 0x7C) {
                anim = 2;
            } else if (arg1->unk3C > 0.0f) {
                anim = 1;
            } else {
                anim = 0;
            }
            break;
        case 0x88:
            switch (id) {
            default:
                anim = 2;
                break;
            case 0x11:
                anim = 0;
                break;
            case 0x26:
                anim = 1;
                break;
            }
            break;
        case 0x98:
            if (arg1->unk4 == 0x8B) {
                arg0->unk2 = 6;
                anim = 5;
                break;
            }
            switch (id) {
            case 0x12C:
                anim = 0;
                break;
            case 0x12D:
                anim = 3;
                break;
            case 0x142:
                anim = 4;
                break;
            case 0x12E:
            case 0x358:
                anim = 2;
                break;
            case 0x2AA:
                anim = 6;
                break;
            default:
                return 1;
            }
            break;
        case 0x87:
            switch (id) {
            case 0x172:
                anim = 0;
                break;
            case 0x174:
                anim = 2;
                break;
            case 0x17A:
                anim = 3;
                break;
            case 0x171:
            case 0x173:
                anim = 1;
                break;
            default:
                return 1;
            }
            break;
        case 0x89:
            if (arg1->unk5 == 5) {
                anim = 3;
                arg0->unk2 = 0;
                break;
            }
            switch (id) {
            case 0x49:
                anim = 0;
                arg0->unk2 = 9;
                break;
            case 0x47:
                anim = 1;
                arg0->unk2 = 9;
                break;
            case 0x81:
                anim = 2;
                arg0->unk2 = 0x13;
                break;
            default:
                return 1;
            }
            break;
        case 0x91:
            if (arg1->unk5 == 5) {
                arg0->unk2 = 0;
                if (arg1->unk24 == 0.0f) {
                    anim = 3;
                } else {
                    anim = 0;
                }
            } else {
                arg0->unk2 = 0xE;
                switch (id) {
                default:
                    anim = 4;
                    break;
                case 0x12:
                    anim = 2;
                    break;
                case 0x10:
                    anim = 1;
                    break;
                }
            }
            break;
        }
        break;
    }
    if (anim != -1) {
        func_1503F5B8((AnimInstance *)arg0->unk48, 0, anim, 0.0f, 0.0f, 1);
    }
    if (sub != NULL) {
        a = arg0->unk48;
        if (anim != cur) {
            goto skip1;
        }
        if (sub->unk3C != 0x3FF) {
            goto skip1;
        }
        func_1505E060(a);
        a = arg0->unk48;
    skip1:
        a->unk8 = sub->unk8;
        a = arg0->unk48;
        if (flag && (a->unk39 != 0 || a->unk215 == 0)) {
            a->unk3A = sub->unk3A;
            arg0->unk48->unk3C = sub->unk3C;
            a = arg0->unk48;
        }
        lim = a->unk18 - 1.0f;
        if (lim <= a->unk8) {
            a->unk8 = lim;
        }
    }
    return 0;
}

/* D_800841C0 (.data) holds this function's address: restores as1's indirect-jump-target view. */
static s32 (*const D_800841D4_entry[])(F15031FC8Arg0 *, F15031FC8Arg1 *) = { func_15031FC8 };


/* Hook: plays the owner's unk2E4 animation on the attachment and keeps it in step with the owner's frame. */
s32 func_150331B8(Attachment *att, struct127 *owner) {
    struct197 *ownerAnim;
    AnimInstance *anim;

    ownerAnim = owner->unk2D0;
    anim = att->anim;
    if (anim == NULL) {
        return 0;
    }
    if ((owner->unk2E4 & 0xFF) != 0xFF) {
        func_1503F5B8(anim, 0, owner->unk2E4 & 0xFF, 1.0f, 0.0f, 1);
    }
    if (ownerAnim != NULL) {
        att->anim->frame = ownerAnim->unk8;
        anim = att->anim;
        if (anim->numFrames <= anim->frame) {
            anim->frame = anim->numFrames - 1.0f;
        }
    }
    return 0;
}

/* Hook: plays animation 0 on the attachment once; removes the attachment when it has finished. */
s32 func_1503327C(Attachment *att, struct127 *owner) {
    AnimInstance *anim;

    anim = att->anim;
    if (anim == NULL) {
        return 0;
    }
    if ((anim->anim & 0x8000) != 0x8000) {
        func_1503F5B8(anim, 0, 0, 1.0f, 0.0f, 1);
        anim = att->anim;
    }
    if (anim->numFrames - 1.0f <= anim->frame) {
        return 1;
    }
    return 0;
}

s32 func_15033328(Attachment *att, struct127 *owner) {
    s32 delta;

    if (D_800C35EA == 1) {
        return 0;
    }
    if (owner->in_water == 0) {
        if (owner->unk118 < owner->unk180) {
            delta = D_800BE9E4;
            if (delta < att->unk38) {
                att->unk38 -= delta;
                return 0;
            } else {
                return 1;
            }
        }
    }
    att->unk38 = 30;
    return 0;
}

/* Hook: hides the attachment while the owner is below unk118 + 300; removes it in water. */
s32 func_150333A8(Attachment *att, struct127 *owner) {
    struct126 *unk;

    if (D_800C35EA == 1) {
        return 0;
    }
    if (owner->in_water != 0) {
        unk = owner->unk31C;
        if (unk != NULL) {
            unk->unk11A = 0;
        }
        return 1;
    }
    if ((owner->unk118 != -10000.0f) && (owner->y_position < owner->unk118 + 300.0f)) {
        att->alpha = 0;
    } else {
        att->alpha = 0xFF;
    }
    return 0;
}

/* Hook: while the owner's unk5 is 5, moves models 0x27/0x35/0x29 to bone 0 (spinning 0x27/0x35). */
s32 func_15033440(Attachment *att, struct127 *owner) {
    switch (att->model) {
    case 0x27:
    case 0x35:
        if (owner->unk5 == 5) {
            att->bone = 0;
            att->rotZ += D_800BE9E4 * 0xAAA;
        }
        break;
    case 0x29:
        if (owner->unk5 == 5) {
            att->bone = 0;
        }
        break;
    }
    return 0;
}

s32 func_150334B8(Attachment *att, struct127 *owner) {
    Gfx *gfx;
    s32 count;
    s32 i;
    s32 w0;
    s32 w1;
    s32 width;
    s32 s;
    s32 t;
    s32 height;
    s32 offset;

    gfx = NULL;
    count = 0;
    offset = 0;
    if (att->model == 0x37) {
        gfx = att->dlists[0];
        count = 4;
        offset = -100;
    }
    if (gfx != NULL) {
        i = 0;
        while (count != 0) {
            count--;
            while (GFX_CMD(&gfx[i]) != (s8)G_SETTILESIZE) {
                i++;
            }
            if (count != 0) {
                i++;
            }
        }

        w0 = gfx[i].words.w0;
        w1 = gfx[i].words.w1;
        width = ((w1 >> 12) & 0xFFF) + 2;
        s = ((w0 >> 12) & 0xFFF) + offset;
        t = w0 & 0xFFF;
        if (s >= width) {
            s -= width;
        }
        if (s < 0) {
            s += width;
        }
        height = (w1 & 0xFFF) + 2;
        if (t >= height) {
            t -= height;
        }
        if (t < 0) {
            t += height;
        }
        gfx[i].words.w0 = (G_SETTILESIZE << 24) | ((s & 0xFFF) << 12) | (t & 0xFFF);
    }
    return 0;
}

void *func_150335C8(struct127 *arg0, struct127 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    Game5D2C0RenderState *temp_v0;
    f32 sp124[4][4];
    f32 spE4[4][4];
    f32 spA4[4][4];
    f32 sp64[4][4];
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 scale;

    if (arg1->unk1D4 == 0) {
        return 0;
    }

    temp_v0 = func_15083568(arg1, arg2, 1.0f, 0);
    if (temp_v0 != 0) {
        temp_v0->field_0x02 = arg3;
        temp_v0->field_0x40 = arg0->xz_scale;

        if (arg4 != 0) {
            temp_v0->field_0x16 = temp_v0->field_0x16 & ~4;
        } else {
            temp_v0->field_0x16 = temp_v0->field_0x16 | 4;
        }

        guMtxL2F(sp124, (Mtx *)((u8 *)arg1->unk1D4 + (arg3 << 6)));
        func_15048B10((s32)sp124, (s32)spE4);

        scale = arg0->xz_scale;
        func_150A9B0C(spA4, arg0->unkB8, arg0->unk40, arg0->unkC4, scale, arg0->y_scale, scale);

        spA4[3][0] = arg0->x_position;
        spA4[3][1] = arg0->y_position;
        spA4[3][2] = arg0->z_position;
        spA4[0][3] = 0.0f;
        spA4[1][3] = 0.0f;
        spA4[2][3] = 0.0f;
        spA4[3][3] = 1.0f;

        func_150A7A48(spA4, spE4, sp64);
        func_1503E5F8(sp64, &sp60, &sp5C, &sp58, &sp48, &sp44, &sp40, &sp54, &sp50, &sp4C);

        if (arg5 != 0) {
            sp58 = 0.0f;
            sp5C = 0.0f;
            sp60 = 0.0f;
        }

        func_15030D54(temp_v0, sp60, sp5C, sp58, sp48, sp44, sp40);
        return temp_v0;
    }

    return 0;
}

/* Hook: model 0x11 is removed unless its owner is in one of a few states. */
s32 func_1503378C(Attachment *att, struct127 *owner) {
    u16 state = owner->unk84.uh;

    if (att->model == 0x11) {
        if ((state == 0x3E) || (state == 0x3D) || (state == 0x41) || (state == 0xD9) || (state == 0x138) ||
            (state == 0x139)) {
            return 0;
        }
    }
    return 1;
}

extern s32 D_800902FC[];

/* Hook: toggles seg6Id between the two D_800902FC entries every 16 ticks. */
s32 func_150337E4(Attachment *att, struct127 *owner) {
    att->unk38 += D_800BE9E4;
    if (att->unk38 >= 16) {
        att->unk38 = 0;
        att->unk3C ^= 1;
    }
    att->seg6Id = D_800902FC[att->unk3C];
    return 0;
}

void func_151026BC(struct127 *, s32, s32, s32, s32, s32);

/* Hook: starts func_151026BC effect `kind` while the owner is in the set's trigger state (and
 * unk31C allows it), and stops it with func_151027E8 when that no longer holds. */
s32 func_15033838(Attachment *att, struct127 *owner) {
    s32 kind;
    s32 triggered;
    struct126 *unk;

    triggered = 0;
    if (att->setId == 0x16) {
        kind = 4;
        if (owner->unk84.uh == 0x165) {
            triggered = 1;
        }
    } else if (att->setId == 0x89) {
        kind = 6;
        switch (owner->unk84.uh) {
        case 0x221:
        case 0x223:
        case 0x31B:
            if (owner->unk31C->unk198 == 2) {
                triggered = 1;
            }
            break;
        }
    } else {
        kind = 5;
        if (owner->unk84.uh == 0x157) {
            triggered = 1;
        }
    }
    if (att->unk38 == 0) {
        if (triggered) {
            unk = owner->unk31C;
            if ((unk->pad196[1] != 0) || (kind == 6)) {
                if (((u16)unk->unk8A & 0x2000) == 0x2000) {
                    if (unk->unk19E == 0) {
                        att->unk38 = 1;
                        func_151026BC(owner, -1, kind, 1, 0xFF, 1);
                    }
                }
            }
        }
    } else if (!triggered || (unk = owner->unk31C, ((u16)unk->unk8A & 0x2000) == 0) ||
               ((unk->pad196[1] == 0) && (kind != 6))) {
        att->unk38 = 0;
        func_151027E8(owner);
    }
    return 0;
}

/* Hook (model 0x37): scrolls the texture and keeps looping sound 0x448 playing at the owner
 * unless D_800BEA0C is set. */
s32 func_150339C8(Attachment *att, struct127 *owner) {
    func_150334B8(att, owner);
    if (D_800C35EA != 1) {
        if (D_800BEA0C != 0) {
            if (att->unk3C != 0) {
                func_1000FD38(func_15033BDC, att, (s32)owner);
            }
            att->unk3C = 0;
        } else if (att->unk3C == 0) {
            att->unk3C = func_1000FA64(0x448, owner->x_position, owner->y_position, owner->z_position, 32000, 1000,
                                       500, (s32)func_15033BDC, att, (s32)owner, 0, 0) |
                         0x80000000;
        }
    }
    return 0;
}

/* Hook: after a 30-tick delay, starts sound 0x513 at the owner (func_15033BDC follows it up). */
s32 func_15033AD8(Attachment *att, struct127 *owner) {
    if (D_800BE616 != 0) {
        func_1508B20C(owner->x_position, owner->y_position, owner->z_position, 900.0f);
    }
    if (att->unk38 == 0) {
        if (att->unk3C < 30) {
            att->unk3C += D_800BE9E4;
        } else {
            att->unk3C = func_1000FA64(0x513, owner->x_position, owner->y_position, owner->z_position, 32000, 1000,
                                       500, (s32)func_15033BDC, att, (s32)owner, 0, 0);
            att->unk38 = 0x513;
        }
    }
    return 0;
}

/* Sound started by the attachment hooks (func_1000FA64); only the fields used here. */
typedef struct AttachmentSound {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[0x10];
    /* 0x18 */ Attachment *att;
    /* 0x1C */ struct127 *owner;
    /* 0x20 */ u8 pad20[0x4];
    /* 0x24 */ u16 unk24;
} AttachmentSound;

void func_10010FFC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, struct127 *arg5);

/* Sound callback: tracks the owner's position and plays the model's follow-up sounds. Returns 1
 * when the sound should stop. */
s32 func_15033BDC(AttachmentSound *sound, s32 arg1, s32 *arg2, s32 arg3, s32 arg4, s32 arg5, s16 *arg6) {
    Attachment *att;
    struct127 *owner;
    struct126 *unk;

    att = sound->att;
    owner = sound->owner;
    if ((att != NULL) && (owner != NULL) && (owner->interaction_state != 0)) {
        sound->x = owner->x_position;
        sound->y = owner->y_position;
        sound->z = owner->z_position;

        if (*arg2 != 0) {
            if (att->model == 0x37) {
                s32 lastState = att->unk38;
                s32 sfxId = -1;

                if (owner->unk84.uh != (lastState & 0xFFFF)) {
                    if (owner->unk84.uh == 0x15F) {
                        sfxId = (func_150ADA20() & 3) + 0x444;
                    }
                }
                if (sfxId != -1) {
                    func_10010FFC(0, sfxId, 24000, 0, 0, owner);
                }
                att->unk38 = owner->unk84.uh;
            } else {
                unk = owner->unk31C;
                if (unk != NULL) {
                    if (*(u16 *)&unk->pad19C < 120) {
                        if (att->unk38 == 0x513) {
                            att->unk38 = 0x3A1;
                            att->unk3C = func_1000FA64(0x3A1, owner->x_position, owner->y_position,
                                                       owner->z_position, 32000, 1000, 500, (s32)func_15033BDC, att,
                                                       (s32)owner, 0, 0);
                            return 1;
                        }
                    }
                }
            }
            return 0;
        }

        if (att->model == 0x37) {
            if (sound->unk24 != 0) {
                func_100111C8(sound->unk24);
                sound->unk24 = 0;
            }
            *arg6 = 0;
            return 0;
        }
    }
    return 1;
}

s32 func_15033E00(Attachment *att, struct127 *owner) {
    if (owner->unk5 == 3) {
        return 1;
    }
    return 0;
}

s32 func_15033E28(struct127 *owner, Attachment **out) {
    Attachment *head;
    Attachment *att;
    Attachment *next;
    s32 count;

    head = (Attachment *)D_800C3EE0;
    count = 0;
    if (head == NULL) {
        return 0;
    }
    att = head; // matching: the second pointer moves the walk from v0 into a0
    while (att != NULL) {
        next = att->next;
        if (att->ownerId == owner->unique_id) {
            out[count] = att;
            count++;
        }
        att = next;
    }
    return count;
}

/* Returns the first attachment of `owner`, or NULL. */
Attachment *func_15033E84(struct127 *owner) {
    Attachment *att;
    Attachment *next;
    u8 ownerId;

    att = (Attachment *)D_800C3EE0;
    if (att != NULL) {
        ownerId = owner->unique_id;
        do {
            next = att->next;
            if (att->ownerId == ownerId) {
                return att;
            }
            att = next;
        } while (att != NULL);
    }
    return NULL;
}

/* Swaps the owners of all attachments of owner ids `a` and `b`. */
void func_15033EC4(s32 a, s32 b) {
    Attachment *att;
    Attachment *next;
    s32 idA;
    s32 ownerId;

    idA = a; // matching: IDO keeps the copy in a2
    att = (Attachment *)D_800C3EE0;
    while (att != NULL) {
        next = att->next;
        ownerId = att->ownerId;
        if (idA == ownerId) {
            att->ownerId = b;
        } else if (b == ownerId) {
            att->ownerId = idA;
        }
        att = next;
    }
}

/* Hook: removes the attachment (clearing unk31C->unk11A) unless unk31C->unk78 is 9 or unk11A is 3. */
s32 func_15033F0C(Attachment *att, struct127 *owner) {
    struct126 *unk;

    if (D_800C35EA == 1) {
        return 0;
    }
    unk = owner->unk31C;
    if (unk != NULL) {
        if (unk->unk78 != 9) {
            if (unk->unk11A != 3) {
                unk->unk11A = 0;
                return 1;
            }
        }
    }
    return 0;
}

/* Hook: like func_15033F0C, but keeps the attachment while unk31C->unk78 is 0xC or 0x16. */
s32 func_15033F70(Attachment *att, struct127 *owner) {
    struct126 *unk;

    if (D_800C35EA == 1) {
        return 0;
    }
    unk = owner->unk31C;
    if (unk != NULL) {
        if ((unk->unk78 != 0xC) && (unk->unk78 != 0x16)) {
            if (unk->unk11A != 3) {
                unk->unk11A = 0;
                return 1;
            }
        }
    }
    return 0;
}
