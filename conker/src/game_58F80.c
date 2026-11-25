#include <ultra64.h>
/* functions.h / variables.h type these with GameObj, which has no fields at many of the
   offsets this file uses; shadow them and redeclare them with the GameObj view below. */
#define func_1505E650 func_1505E650_hdr
#define func_1506160C func_1506160C_hdr
#define func_15072208 func_15072208_hdr
#include "functions.h"
#undef func_1505E650
#undef func_1506160C
#undef func_15072208
#define D_800CC2D0 D_800CC2D0_hdr
#define D_800C3E80 D_800C3E80_hdr
#include "variables.h"
#undef D_800CC2D0
#undef D_800C3E80

/* Streamed per-frame animation data (obj->anim). */
typedef struct ObjAnim {
    /* 0x000 */ u8 pad0[0x8];
    /* 0x008 */ f32 frame[2];
    /* 0x010 */ f32 unk10;
    /* 0x014 */ u8 pad14[0x4];
    /* 0x018 */ f32 frameCount[2];
    /* 0x020 */ u8 pad20[0x8];
    /* 0x028 */ u8 *frameData[2];
    /* 0x030 */ u8 *buffer[2];
    /* 0x038 */ u8 bufferOffset[2];
    /* 0x03A */ u8 pad3A[0x2];
    /* 0x03C */ s16 unk3C;
    /* 0x03E */ u8 pad3E[0x7];
    /* 0x045 */ u8 frameSize;
    /* 0x046 */ u8 pad46[0x2];
    /* 0x048 */ u8 unk48;
    /* 0x049 */ u8 pad49[0x1D0 - 0x49];
} ObjAnim; // size 0x1D0

/* struct126 (obj->unk31C) with the byte at 0x197 that struct126 still calls pad196[1]. */
typedef struct GameObjExt {
    /* 0x000 */ u8 pad0[0x197];
    /* 0x197 */ u8 unk197;
} GameObjExt;

/* File-local view of GameObj (the 0x32C-byte game objects in D_800CC2D0); field names follow
   include/structs.h where it has them. Central pass: fold into GameObj. */
typedef struct GameObj {
    /* 0x000 */ s32 interaction_state;
    /* 0x004 */ u8 id;
    /* 0x005 */ u8 unk5; // render kind: 2 sprite, 3 none, 5 matrix only, 7 skipped by LOD
    /* 0x006 */ u8 pad6[0x1];
    /* 0x007 */ u8 unk7;
    /* 0x008 */ u8 unk8;
    /* 0x009 */ u8 unk9;
    /* 0x00A */ u8 unkA;
    /* 0x00B */ u8 unkB[4]; // one per player
    /* 0x00F */ u8 unkF;
    /* 0x010 */ u16 unk10;
    /* 0x012 */ u8 pad12[0x2];
    /* 0x014 */ f32 x_position;
    /* 0x018 */ f32 y_position;
    /* 0x01C */ f32 z_position;
    /* 0x020 */ f32 y_velocity;
    /* 0x024 */ u8 pad24[0x18];
    /* 0x03C */ f32 xz_velocity;
    /* 0x040 */ f32 unk40;
    /* 0x044 */ u8 pad44[0x18];
    /* 0x05C */ s32 unk5C; // bone of the parent this object rides on
    /* 0x060 */ s16 unk60;
    /* 0x062 */ s16 unk62;
    /* 0x064 */ u8 pad64[0x1];
    /* 0x065 */ u8 unk65; // parent object index + 1
    /* 0x066 */ u8 unk66; // flags
    /* 0x067 */ u8 pad67[0x1];
    /* 0x068 */ u8 unk68;
    /* 0x069 */ u8 unk69;
    /* 0x06A */ u8 unk6A[2];
    /* 0x06C */ u8 unk6C[2];
    /* 0x06E */ u8 pad6E[0x1];
    /* 0x06F */ u8 unk6F;
    /* 0x070 */ u8 pad70[0x4];
    /* 0x074 */ u8 unk74; // per-player culled mask
    /* 0x075 */ u8 pad75[0x1];
    /* 0x076 */ u16 unk76;
    /* 0x078 */ u16 unk78;
    /* 0x07A */ u16 unk7A;
    /* 0x07C */ u16 unk7C;
    /* 0x07E */ u8 pad7E[0x6];
    /* 0x084 */ u16 unk84; // animation index, 0xFFFF = none
    /* 0x086 */ u8 pad86[0x2];
    /* 0x088 */ u8 unk88;
    /* 0x089 */ u8 pad89[0xB];
    /* 0x094 */ s32 unk94; // hidden display-list mask
    /* 0x098 */ s32 unk98; // blacked-out display-list mask
    /* 0x09C */ u8 pad9C[0x4];
    /* 0x0A0 */ u32 fogColor; // packed ARGB; in mode 1 a pointer to an f32 percentage
    /* 0x0A4 */ u8 fogMode;
    /* 0x0A5 */ u8 fogLevel; // fade level, or pulse phase in modes 4/5
    /* 0x0A6 */ u8 fogRate;
    /* 0x0A7 */ u8 fogFade; // pulse fade-out, 0xFF = full
    /* 0x0A8 */ u8 padA8[0x4];
    /* 0x0AC */ u8 unkAC;
    /* 0x0AD */ u8 padAD[0xB];
    /* 0x0B8 */ f32 unkB8;
    /* 0x0BC */ u8 padBC[0x8];
    /* 0x0C4 */ f32 unkC4;
    /* 0x0C8 */ f32 unkC8;
    /* 0x0CC */ s16 unkCC;
    /* 0x0CE */ u8 padCE[0x16];
    /* 0x0E4 */ s16 unkE4;
    /* 0x0E6 */ u8 padE6[0x12];
    /* 0x0F8 */ s32 unkF8;
    /* 0x0FC */ u8 padFC[0x5];
    /* 0x101 */ u8 unk101;
    /* 0x102 */ u8 pad102[0x20];
    /* 0x122 */ u8 unk122;
    /* 0x123 */ u8 unk123;
    /* 0x124 */ u8 pad124[0x3];
    /* 0x127 */ u8 unk127;
    /* 0x128 */ u8 pad128[0xC];
    /* 0x134 */ u8 unk134;
    /* 0x135 */ u8 unk135;
    /* 0x136 */ u8 unk136; // sprite texture
    /* 0x137 */ u8 pad137[0xD];
    /* 0x144 */ struct129 *unk144;
    /* 0x148 */ u8 pad148[0x4];
    /* 0x14C */ f32 xz_scale;
    /* 0x150 */ f32 y_scale;
    /* 0x154 */ f32 unk154;
    /* 0x158 */ f32 unk158;
    /* 0x15C */ u8 pad15C[0x4];
    /* 0x160 */ u16 unk160;
    /* 0x162 */ u8 pad162[0x1E];
    /* 0x180 */ f32 unk180;
    /* 0x184 */ u32 unk184;
    /* 0x188 */ u8 pad188[0x16];
    /* 0x19E */ u16 unk19E;
    /* 0x1A0 */ u8 pad1A0[0x28];
    /* 0x1C8 */ u8 lod; // current level of detail
    /* 0x1C9 */ u8 lodRequest; // requested LOD + 1, 0xFF = reset to 0
    /* 0x1CA */ u8 pad1CA[0xA];
    /* 0x1D4 */ f32 (*bones)[4][4]; // this frame's bone matrices
    /* 0x1D8 */ f32 (*prevBones)[4][4];
    /* 0x1DC */ u8 pad1DC[0x1];
    /* 0x1DD */ u8 unk1DD[3];
    /* 0x1E0 */ u8 unk1E0[3];
    /* 0x1E3 */ u8 pad1E3[0x11];
    /* 0x1F4 */ f32 unk1F4;
    /* 0x1F8 */ f32 unk1F8;
    /* 0x1FC */ u8 unk1FC;
    /* 0x1FD */ u8 pad1FD[0x49];
    /* 0x246 */ u8 unk246;
    /* 0x247 */ u8 pad247[0x19];
    /* 0x260 */ s32 unk260;
    /* 0x264 */ s32 unk264;
    /* 0x268 */ u8 pad268[0x8];
    /* 0x270 */ f32 unk270;
    /* 0x274 */ u8 unk274;
    /* 0x275 */ u8 pad275[0x5];
    /* 0x27A */ s16 unk27A[2];
    /* 0x27E */ s16 unk27E[2];
    /* 0x282 */ u8 pad282[0xA];
    /* 0x28C */ s32 unk28C[7][2]; // per LOD, per frame buffer
    /* 0x2C4 */ u8 *lodModels;
    /* 0x2C8 */ u8 lodCount;
    /* 0x2C9 */ u8 unk2C9;
    /* 0x2CA */ u8 pad2CA[0x6];
    /* 0x2D0 */ ObjAnim *anim;
    /* 0x2D4 */ struct102 *unk2D4;
    /* 0x2D8 */ u8 pad2D8[0x24];
    /* 0x2FC */ u8 unk2FC;
    /* 0x2FD */ u8 pad2FD[0x1];
    /* 0x2FE */ u8 unk2FE;
    /* 0x2FF */ u8 pad2FF[0x2];
    /* 0x301 */ u8 unk301;
    /* 0x302 */ u8 unk302[2];
    /* 0x304 */ s32 unk304[4];
    /* 0x314 */ s32 unk314;
    /* 0x318 */ struct108 *camera;
    /* 0x31C */ GameObjExt *unk31C;
    /* 0x320 */ u8 pad320[0xC];
} GameObj; // size 0x32C


extern GameObj D_800CC2D0[];
extern s32 D_800C3E80[2]; // bone-matrix pool per frame buffer

/* struct104 (D_800B0DF0) with the s16 at 0x3E that struct104 still covers with pad2A; nonzero
   means level fog: objects take the level's fog colour and a distance-based fog alpha. */
typedef struct Struct104Ext {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ s16 unk3E;
} Struct104Ext;
void func_1505E650(GameObj *obj, u16 anim, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6);
void func_1506160C(GameObj *obj, s32 arg1, u8 arg2, u8 arg3, s32 player);
GameObj *func_15072208(GameObj *obj, s32 arg1);


extern u16 D_800C4ED0[];
typedef struct {
    u8 unk0;
    u8 pad1[0xF];
} D_800C666FEntry;

typedef struct {
    u8 pad_0[0x4];
    u8 field_0x4;
    u8 pad_0x5[0x9B];
    u32 field_0xA0;
    u8 field_0xA4;
    u8 field_0xA5;
    u8 field_0xA6;
    u8 field_0xA7;
    u8 pad_0xA8[0x50];
    u32 field_0xF8;
    u8 pad_0xFC[0xD8];
    void *field_0x1D4;
    void *field_0x1D8;
    u8 pad_0x1DC[0x88];
    void *field_0x264;
} ActorInteractionFields;

extern D_800C666FEntry D_800C666F[];
extern void *allocate_memory(s32, s32, s32, s32);
extern void func_1502C608(s32);
extern void func_1502DF38(s32, s32);
extern void func_1502E4C4(s32);
extern void func_1502EAFC(GameObj *);
extern void func_1502EC34(GameObj *, s32 *, s32 *, s32 *, s32 *);
extern void func_1502EEF4(s32);
extern void func_1502F264(s32);
extern void func_1502FBE8(GameObj *);
extern void func_150345E4(s32);
extern void func_1503A08C(GameObj *);
extern void func_1503A830(GameObj *);
extern void func_1503DF48(s32);
extern void func_150A4B04(GameObj *);
extern void func_1517AD00(s32, s32, s32);

extern u8 D_80084160[];
extern u8 D_80084190[];
extern s32 func_1506196C(GameObj *, s32);
Gfx *func_1502C408(Gfx *, s32);
Gfx *func_1502C974(Gfx *gdl, s32 index, s32 player, s32 mode, s32 arg4);
Gfx *func_150368C4(Gfx *gdl, s32 index, s32 player);
Gfx *func_15030E08(Gfx *gdl, s32 player, s32 arg2);

/* Emits the display list of every object drawn in one render pass (mode) for one player. */
Gfx *func_1502BAD0(Gfx *gdl, s32 mode, s16 player) {
    GameObj *obj;
    s32 i;
    s32 state;

    gSPDisplayList(gdl++, D_80084160);
    obj = D_800CC2D0;
    for (i = 0; i != 25; i++, obj++) {
        D_8003C8E0 = (i & 0xFFFFFF) | 0x01000000;
        state = obj->interaction_state;
        if (state == 0 || obj->unk5 == 3 || obj->unk5 == 5) {
            continue;
        }
        if (obj->unk5 == 2) {
            if (mode != 2) {
                continue;
            }
        } else if (obj->id == 0xFF) {
            continue;
        }
        if (mode == 6) {
            if (obj->unk5 != 7) {
                continue;
            }
        } else if (mode == 0) {
            if (!((obj->unk184 >> 9) & 1)) {
                continue;
            }
        } else if (mode == 1) {
            if (obj->unk5 == 7 || obj->unk5 == 1) {
                continue;
            }
            if (obj->unk5 == 0 && func_1506196C(obj, player) < 0xFF) {
                continue;
            }
        } else if (mode == 2 && obj->unk5 != 2) {
            if (obj->unk5 == 7 || (obj->unk5 != 0 && obj->unk5 != 1)) {
                continue;
            }
            if (obj->unk5 == 0 && func_1506196C(obj, player) == 0xFF) {
                continue;
            }
        }
        if (obj->unk5 == 2) {
            gdl = func_1502C408(gdl, i);
        } else {
            gdl = func_1502C974(gdl, i, player, mode, 0);
            if (i == 0) {
                gdl = func_150368C4(gdl, i, player);
            }
        }
    }
    D_8003C8E0 = 0x01FFFFFF;
    if (mode == 1) {
        gdl = func_15030E08(gdl, player, 0);
    } else if (mode == 2) {
        gdl = func_15030E08(gdl, player, 1);
    } else if (mode == 6) {
        gdl = func_15030E08(gdl, player, 2);
    }
    gSPDisplayList(gdl++, D_80084190);
    D_8003C8E0 = 0;
    return gdl;
}

/* Per-frame update of one object: LOD, culling, bone matrices, fog effect and attachments. */
void func_1502BD84(GameObj *obj, s32 index) {
    obj->bones = NULL;
    if (obj->unk5 == 5) {
        func_1502DF38(index, TRUE);
        return;
    }
    if (obj->id == 0xFF) {
        return;
    }
    if (obj->unk5 == 3) {
        return;
    }
    if (obj->unk5 == 2) {
        func_1502C608(index);
        return;
    }
    if (obj->lodRequest != 0) {
        func_1502FBE8(obj);
    }
    func_1502E4C4(index);
    func_1502DF38(index, FALSE);
    func_1503A08C(obj);
    if (obj->bones == NULL) {
        obj->unk1FC = 2;
    } else {
        func_150345E4(index);
        func_1503A830(obj);
    }
    if (D_800C666F[index].unk0 != 0) {
        func_1503DF48(index);
    }
    if (obj->interaction_state != 0) {
        func_1502EEF4(index);
        func_1502F264(index);
        if (obj->fogMode != 0) {
            func_1502EAFC(obj);
        }
        if (obj->unkF8 & 0x4000) {
            func_150A4B04(obj);
        }
        if (obj->unk260 != 0) {
            func_1517AD00(obj->unk134, obj->unk135, index);
        }
    }
}

extern void func_1503F964(void);
extern void func_1502F3C8(void);
extern void func_15030468(void);
extern void func_1507C22C(s32);
extern void func_1502F948(GameObj *);
extern u8 D_800C3E90;
extern s32 D_800C3E74;
extern u8 D_800C3E70;
extern s32 D_800D121C;

/* Updates every object, parents before the objects attached to them (by attachment depth). */
void func_1502BEE4(void) {
    GameObj *obj;
    GameObj *parent;
    s32 i;
    s32 j;
    s32 state;
    u8 order[25];
    u8 depth[25];
    s32 count;
    s32 maxDepth;

    func_1503F964();
    maxDepth = 0;
    D_800C3E90 = 0;
    D_800C3E74 = 0;
    for (i = 0; i < 25; i++) {
        if (D_800CC2D0[i].unk274 != 0) {
            D_800C3E74 |= 1 << (D_800CC2D0[i].unk274 - 1);
        }
    }
    bzero(depth, 25);
    for (i = 0; i < 25; i++) {
        obj = &D_800CC2D0[i];
        state = obj->interaction_state;
        if (state != 0) {
            if (obj->unk65 != 0) {
                parent = obj;
                depth[i] = 0;
                while (parent->unk65 != 0) {
                    parent = &D_800CC2D0[parent->unk65 - 1];
                    depth[i]++;
                }
                if (maxDepth < depth[i]) {
                    maxDepth = depth[i];
                }
            } else {
                func_1502BD84(obj, i);
            }
        }
    }
    count = 0;
    if (maxDepth != 0) {
        for (i = 1; i <= maxDepth; i++) {
            for (j = 0; j < 25; j++) {
                if (i == depth[j]) {
                    order[count++] = j;
                }
            }
        }
        for (i = 0; i < count; i++) {
            func_1502BD84(&D_800CC2D0[order[i]], order[i]);
        }
    }
    func_1502F3C8();
    for (i = 0; i < 25; i++) {
        if (D_800CC2D0[i].interaction_state != 0) {
            func_1502F948(&D_800CC2D0[i]);
        }
    }
    func_15030468();
    if (D_800BEAC0 == 0) {
        func_1507C22C(0);
    }
    D_800C3E70 = 0;
}

void func_1516972C(struct102 *arg0);
extern s32 D_80082FA0;
extern u8 D_800C3638;
extern s32 D_800D121C;
extern s32 func_150229E4(GameObj *);
extern s32 func_1506196C(GameObj *, s32);
extern struct102 *func_1510D970(s32, GameObj *, s32, s32, s32);
extern u8 D_800C3656;
/* Creates the unk2D4 effect for objects visible to some player and frees it for the rest. */
void func_1502C1A4(void) {
    GameObj *obj;
    s32 visible;
    s32 i;
    s32 arg2;
    s32 arg4;
    s32 id;
    s32 state;
    struct102 *effect;

    // matching: -g3 line numbers; the init shares the loop's first line (preheader schedule)
    obj = D_800CC2D0; do {
        state = obj->interaction_state;
        if (state != 0) {
            id = obj->id;
            visible = FALSE;
            if (obj->unk5 != 3 && id != 0xFF && obj->unk123 == 0 && (obj->unk66 & 0x10) != 0x10 &&
                obj->unk160 != 0 && obj->unk65 == 0 &&
                (D_800C3638 == 0 || D_800C3656 != 0 || func_150229E4(obj) != 0) &&
                (obj->unk88 == 0 || obj->unkCC >= -150)) {
                for (i = 0; i <= D_80082FA0; i++) {
                    if (func_1506196C(obj, i) != 0) {
                        visible = TRUE;
                    }
                }
            }
            if (visible) {
                if (obj->unk2D4 == NULL) {
                    arg2 = 0;
                    if (obj->id == 0x4D) {
                        arg2 = 2;
                        arg4 = 1;
                    } else {
                        arg4 = 0;
                    }
                    obj->unk2D4 = func_1510D970(0, obj, arg2, 0, arg4);
                    // matching: the label ends the block so as1 can't move the store into the branch delay slot
                created:;
                }
            } else {
                effect = obj->unk2D4;
                if (effect != NULL) {
                    func_1516972C(effect);
                    obj->unk2D4 = NULL;
                }
            }
        }
        obj++;
        // matching: the end is its own symbol (D_800D121C == &D_800CC2D0[25]), not CSE'd with the start
    } while (obj != (GameObj *)&D_800D121C);
}


extern s32 D_800C3E88;
extern s32 D_800C3E8C;
extern u16 D_800C3E7A;

/* Resets this frame buffer's bone-matrix pool. */
void func_1502C380(void) {
    D_800C3E8C = D_800C3E88 = D_800C3E80[D_800BE9C0];
    D_800C3E7A = 0;
}

/* Texture index of a sprite object (out-of-range values fall back to 11). */
s32 func_1502C3BC(s32 index) {
    s32 tex = D_800CC2D0[index].unk136;

    if (tex >= 70) {
        tex = 11;
    }
    return tex;
}

typedef struct SpriteParams {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ s16 w;
    /* 0x8 */ s16 h;
    /* 0xA */ u8 alpha;
    /* 0xB */ u8 unkB;
    /* 0xC */ u8 unkC;
    /* 0xD */ u8 flip;
    /* 0xE */ u8 padE[2];
} SpriteParams;

extern u8 *D_8008CA4C[];
extern Gfx D_800873D0[];
extern Gfx D_80087408[];
Gfx *func_15094F70(Gfx *, u8 *, s32, SpriteParams *, s32, s32, s32, s32, s32);
Gfx *func_15095760(Gfx *, SpriteParams *);

#define ACTOR ((Actor1502C408 *)&D_800CC2D0[arg1])
/* Draws a kind-2 object as a textured sprite. */
Gfx *func_1502C408(Gfx *gdl, s32 index) {
    s32 pad; // matching: unused stack slots (pad, pad2) fix the frame layout
    SpriteParams sprite;
    s32 w;
    s32 h;
    s32 tex;
    s16 pad2;
    s16 scroll;
    s32 alpha;
    u8 size;

    if (D_800BEAC0 != 0) {
        return gdl;
    }
    alpha = D_800CC2D0[index].unk7;
    size = D_800CC2D0[index].unk122;
    scroll = D_800CC2D0[index].unk60;
    if (alpha == 0) {
        return gdl;
    }
    h = w = size * 19;
    if (D_800CC2D0[index].interaction_state == 3) {
        h = w = size * 15;
    }
    tex = func_1502C3BC(index);
    gSPDisplayList(gdl++, D_800873D0);
    gDPPipeSync(gdl++);
    if (gdl != NULL) { // matching: the empty test changes the register choice of the spilled temps
    }
    gdl = func_15094F70(gdl, D_8008CA4C[tex], scroll, &sprite, 0, 0, 0, 2, 3);
    sprite.flip = 0;
    sprite.alpha = alpha;
    sprite.w = w;
    sprite.h = h;
    sprite.x = D_800CC2D0[index].x_position;
    sprite.y = D_800CC2D0[index].y_position + (f32)(size / 2);
    sprite.z = D_800CC2D0[index].z_position;
    gdl = func_15095760(gdl, &sprite);
    gSPDisplayList(gdl++, D_80087408);
    return gdl;
}
/* Advances a sprite object's texture scroll, wrapping at the texture's period. */
void func_1502C608(s32 index) {
    s32 period;
    s16 scroll;
    s16 speed;

    if (D_800BEAC0 == 0) {
        scroll = D_800CC2D0[index].unk60;
        speed = D_800CC2D0[index].unk62;
        if (D_800CC2D0[index].unk7 != 0) {
            period = D_8008CA4C[func_1502C3BC(index)][4] << 8;
            scroll += speed;
            if (scroll >= period) {
                scroll -= period;
            } else if (scroll < 0) {
                scroll += period;
            }
            D_800CC2D0[index].unk60 = scroll;
        }
    }
}


extern s32 func_150849A0(GameObj *);
extern void func_150837D4(s32, s32, s32);

/* Switches the object to the LOD model for its distance from the player's camera. */
void func_1502C6E8(s32 index, s16 player, s32 mode) {
    struct108 *cam;
    GameObj *obj;
    s32 special;
    s32 lod;
    s32 maxLod;
    s32 model;
    f32 range[4];
    f32 diff;
    f32 distSq;

    cam = &D_800DBFF0[player];
    obj = &D_800CC2D0[index];
    if (obj->lodRequest != 0) {
        return;
    }
    if (obj->unk5 == 7) {
        return;
    }
    range[0] = 500.0f;
    range[1] = 730.0f;
    range[2] = 1500.0f;
    range[3] = 2000.0f;
    maxLod = obj->lodCount - 1;
    if (maxLod == -1) {
        return;
    }
    special = func_150849A0(obj);
    diff = cam->unk2F8 - obj->x_position;
    distSq = diff * diff;
    diff = cam->unk2FC - obj->y_position;
    diff = diff * diff;
    distSq += diff;
    diff = cam->unk300 - obj->z_position;
    diff = diff * diff;
    distSq += diff;
    if (distSq < range[0] * range[0]) {
        lod = 0;
    } else if (distSq < range[1] * range[1]) {
        lod = 1;
    } else if (distSq < range[2] * range[2]) {
        lod = 2;
    } else if (distSq < range[3] * range[3]) {
        lod = 3;
    } else {
        lod = 4;
    }
    if (lod >= 2 && obj->xz_velocity < 3.0f) {
        lod--;
    }
    if (special) {
        if (special == 90) {
            if (lod == 0) {
                lod = 1;
            }
        }
    } else if (D_800BE616 != 0) {
        lod = 1;
    }
    if (D_800C35EA == 1) {
        lod = 0;
    }
    if (maxLod < lod) {
        lod = maxLod;
        if (lod < 0) {
            lod = 0;
        }
    }
    if (lod == -1) {
        return;
    }
    if (lod == obj->lod) {
        return;
    }
    model = obj->lodModels[lod];
    func_150837D4(index, model, 0);
    obj->lod = lod;
}

extern Gfx *D_800BE9C8[];
extern s32 D_800BEBA4;
extern void *D_800DF7C0;
extern u8 D_800DF7C4;
extern s32 func_150849CC(GameObj *, s32 *);
extern void func_1502D630(GameObj *, s32 *, s32);
extern Gfx *func_1502CCFC(Gfx *, s32, s32, void *, s32, s32 *, s32, s32);
void func_1502D54C(s32, s32 *);

/* Draws one object's model for one player; drops the output if the display list would overflow. */
Gfx *func_1502C974(Gfx *gdl, s32 index, s32 player, s32 mode, s32 arg4) {
    s32 lod;
    s32 alpha;
    s32 colour[4];
    s32 overflow;
    void *model;
    Gfx *start;
    GameObj *obj;

    start = gdl;
    if (D_800C3638 != 0 && D_800C3656 == 0 && func_150229E4(&D_800CC2D0[index]) == 0) {
        return gdl;
    }
    obj = &D_800CC2D0[index];
    if ((obj->unk74 & (1 << player)) == (1 << player)) {
        return gdl;
    }
    if (func_1506196C(obj, player) == 0) {
        return gdl;
    }
    if (obj->bones == NULL) {
        return gdl;
    }
    if (mode != 4 && mode != 5 && mode != 3) {
        func_1502C6E8(index, player, mode);
        lod = obj->lod;
    } else {
        func_150849CC(obj, &lod);
    }
    func_1502D54C(index, colour);
    if (((Struct104Ext *)D_800B0DF0)->unk3E != 0) {
        func_1502D630(obj, colour, player);
    } else {
        colour[3] = 0xFF;
    }
    alpha = func_1506196C(obj, player);
    if (alpha < 0xFF) {
        if (obj->camera != NULL && obj->camera->unk2C == 0x100 && player != obj->unk127) {
            alpha = 0xFF;
        }
    }
    if (mode == 4) {
        alpha = (D_800DF7C4 * alpha) >> 8;
        model = D_800DF7C0;
    } else {
        model = obj->bones;
    }
    gdl = func_1502CCFC(gdl, index, player, model, alpha, colour, mode, arg4);
    // matching: IDO materialises the comparison as a 0/1 value only through this flag
    if (gdl - D_800BE9C8[D_800BE9C0] > D_800BEBA4) {
        overflow = TRUE;
    } else {
        overflow = FALSE;
    }
    if (overflow) {
        return start;
    }
    return gdl;
}

/* Gathers the colours an object is drawn with: the player's prim/env colours and the object's fog. */
void func_1502CC34(GameObj *obj, s32 player, s32 alpha, s32 *colour, s32 *primR, s32 *primG, s32 *primB,
                   s32 *envR, s32 *envG, s32 *envB, s32 *fogR, s32 *fogG, s32 *fogB, s32 *fogA) {
    u8 *prim;
    u8 *env;

    prim = D_800D9B68[player];
    *primR = prim[0];
    env = D_800D9B78[player];
    *primG = prim[1];
    *primB = prim[2];
    *envR = env[0];
    *envG = env[1];
    *envB = env[2];
    *fogR = 0;
    *fogG = 0;
    *fogB = 0;
    *fogA = colour[3];
    if (obj->fogMode != 0) {
        func_1502EC34(obj, fogR, fogG, fogB, fogA);
    }
}

extern s32 func_1503DA9C(GameObj *, s32, s32, s32);
Gfx *func_1502F01C(Gfx *, s32);
extern Gfx *func_1502F9FC(Gfx *, s32);
extern void func_1502FD70(GameObj *);
extern Gfx *func_15030F94(Gfx *, GameObj *, s32, s32);
extern Gfx *func_1515D914(Gfx *, s32, s32, s32, s32, s32, s32, s32, s32, u8 *, s32, u8 *, s32, void *);
extern void func_151EFE88(void *);
extern u16 D_800C4778[];
extern Gfx **D_800C48F0[];
extern Gfx **D_800C4488[];
extern u8 D_800DD2E4[];
extern u8 D_800D9900;
extern u8 D_800D9E28[];
extern u8 D_800C3E98[];
extern u8 D_C3E98[];
extern u8 D_80084160[];
extern u8 D_800832C0[];
extern u8 D_80082FC0[];
extern u8 D_80083140[];
extern u8 D_80084190[];

/* Emits an object's model: segments, geometry mode, lighting, colours and each part's display list. */
Gfx *func_1502CCFC(Gfx *gdl, s32 index, s32 player, void *model, s32 alpha, s32 *colour, s32 mode, s32 wrap) {
    GameObj *obj;
    s32 i;
    s32 lod;
    s32 id;
    s32 colourSet;
    s32 fb;
    s32 lightFlags;
    s32 levelFog;
    s32 lit;
    s32 primR;
    s32 primG;
    s32 primB;
    s32 envR;
    s32 envG;
    s32 envB;
    s32 fogR;
    s32 fogG;
    s32 fogB;
    s32 fogA;
    s32 pad; // matching: unused stack slot

    obj = &D_800CC2D0[index];
    id = obj->id;
    if (mode == 3 || mode == 4 || mode == 5) {
        id = func_150849CC(obj, &lod);
    } else {
        lod = obj->lod;
    }
    if (func_1503DA9C(obj, id, lod, (mode == 3) ? 0 : 1) != 0) {
        return gdl;
    }
    fb = D_800BE9C0;
    if (obj->unk28C[lod][fb] == 0) {
        return gdl;
    }
    if (wrap) {
        gSPDisplayList(gdl++, D_80084160);
    } else {
        gDPPipeSync(gdl++);
    }
    gdl = func_1502F01C(gdl, index);
    gdl = func_1502F9FC(gdl, index);
    gSPSegment(gdl++, 3, model);
    gSPSegment(gdl++, 1, obj->unk28C[lod][fb] - 0x38);
    func_1502CC34(obj, player, alpha, colour, &primR, &primG, &primB, &envR, &envG, &envB, &fogR, &fogG, &fogB,
                  &fogA);
    if (mode == 5) {
        gSPSegment(gdl++, 8, D_800832C0);
    } else if (mode == 3) {
        gSPSegment(gdl++, 8, D_800832C0);
    } else if (alpha < 0xFF) {
        gSPSegment(gdl++, 8, D_80082FC0);
    } else {
        gSPSegment(gdl++, 8, D_80083140);
    }
    gSPClearGeometryMode(gdl++, G_CULL_BOTH);
    if (mode == 0) {
        gSPSetGeometryMode(gdl++, G_CULL_FRONT);
    } else {
        gSPSetGeometryMode(gdl++, G_CULL_BACK);
    }
    lit = TRUE;
    levelFog = ((Struct104Ext *)D_800B0DF0)->unk3E;
    if ((obj->unk66 & 0xC) == 0xC) {
        gSPSetGeometryMode(gdl++, G_LIGHTING);
        gSPClearGeometryMode(gdl++, 0x400000);
    } else if ((obj->unk66 & 0xC) != 8) {
        gSPClearGeometryMode(gdl++, G_LIGHTING | 0x400000);
        lit = FALSE;
    } else if (D_800DCD7C != 0) {
        gSPSetGeometryMode(gdl++, G_LIGHTING | 0x400000);
    } else {
        gSPClearGeometryMode(gdl++, G_LIGHTING | 0x400000);
        lit = FALSE;
    }
    if (mode == 3) {
        gSPClearGeometryMode(gdl++, G_LIGHTING | 0x400000);
        lit = FALSE;
    }
    if (lit) {
        lightFlags = (obj->unk2FE == 0) ? 0xA : 2;
        if (D_800D9900 != 0) {
            lightFlags |= 8;
        }
        gdl = func_1515D914(gdl, player, (s32)obj->x_position, (s32)obj->y_position, (s32)obj->z_position,
                            obj->unk184, obj->unk304[player], obj->unk301, obj->unk314 + D_800BE9C0 * 8,
                            obj->unk302, 2, obj->unk1DD, lightFlags, D_800D9E28);
    }
    colourSet = -1;
    for (i = 0; i < D_800C4778[id]; i++) {
        if (!(obj->unk94 & (1 << i))) {
            if (!(obj->unk98 & (1 << i))) {
                if (colourSet != 0) {
                    gDPPipeSync(gdl++);
                    colourSet = 0;
                    if (levelFog == 0) {
                        gDPSetFogColor(gdl++, fogR, fogG, fogB, fogA);
                    } else {
                        gDPSetFogColor(gdl++, D_800DD2E4[0], D_800DD2E4[1], D_800DD2E4[2], fogA);
                    }
                    gDPSetPrimColor(gdl++, 0xF2, 0, primR, primG, primB, 0);
                    gDPSetEnvColor(gdl++, envR, envG, envB, alpha);
                }
            } else if (colourSet != 1) {
                gDPPipeSync(gdl++);
                gDPSetFogColor(gdl++, 0, 0, 0, 0xFF);
                gDPSetPrimColor(gdl++, 0xF2, 0, 0, 0, 0, 0);
                gDPSetEnvColor(gdl++, 0, 0, 0, 0xFF);
                colourSet = 1;
            }
            if (mode == 3) {
                if (D_800C48F0[id] != NULL) {
                    gSPDisplayList(gdl++, D_800C48F0[id][i]);
                    continue;
                }
            } else {
                func_151EFE88(D_800C3E98);
                gSPMatrix(gdl++, D_C3E98, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(gdl++, D_800C4488[id][i]);
            }
        }
    }
    func_1502FD70(obj);
    obj->unk2FE = 1;
    if (mode == 0) {
        gdl = func_15030F94(gdl, obj, player, 1);
    }
    if (wrap) {
        gSPDisplayList(gdl++, D_80084190);
    }
    if (mode == 0) {
        gSPGeometryMode(gdl++, G_CULL_FRONT, G_CULL_BACK);
    }
    return gdl;
}

void func_1502D54C(s32 arg0, s32 *arg1) {
    u8 *temp_v0;

    temp_v0 = (u8 *)&D_800CC2D0[arg0];
    if ((temp_v0[0x66] & 0xC) == 4) {
        arg1[0] = (temp_v0[0x1E0] + temp_v0[0x1DD]) / 2;
        arg1[1] = (temp_v0[0x1E1] + temp_v0[0x1DE]) / 2;
        arg1[2] = (temp_v0[0x1E2] + temp_v0[0x1DF]) / 2;
        arg1[0] = 0xFF - arg1[0];
        arg1[1] = 0xFF - arg1[1];
        arg1[2] = 0xFF - arg1[2];
        return;
    }
    arg1[2] = 0xFF;
    arg1[1] = 0xFF;
    arg1[0] = 0xFF;
}

void func_150A7A00(void *, f32, f32, f32, f32 *, f32 *, f32 *, f32 *);
extern f32 D_80096DE8;
extern f32 D_80096DEC;
extern f32 D_80096DF0;
extern f32 D_80096DF4;
extern u8 D_800D2CA8[];
extern u16 D_800DD2E8;
extern u16 D_800DD2EC;

void func_1502D630(GameObj *arg0, s32 *arg1, s32 arg2) {
    f32 far;
    f32 near;
    f32 step;
    f32 first;
    f32 last;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    f32 amount;

    if (D_800D9B1C != 0.0f) {
        first = 1.0f / D_800D9B1C;
    } else {
        first = D_80096DE8;
    }
    if (D_800D9B20 != 0.0f) {
        last = 1.0f / D_800D9B20;
    } else {
        last = D_80096DEC;
    }
    step = (first - last) * D_80096DF0;
    near = (f32)(u32)D_800DD2E8 * step + last;
    far = (f32)(u32)D_800DD2EC * step + last;
    func_150A7A00(D_800D2CA8 + (arg2 << 6), arg0->x_position,
                 arg0->y_position, arg0->z_position, &x, &y, &z, &w);
    w = (w != 0.0f) ? 1.0f / w : D_80096DF4;
    if (w < 0) {
        amount = 0;
    } else if (w >= far) {
        amount = 0;
    } else if (w <= near) {
        amount = 1.0f;
    } else {
        amount = (w - far) / (near - far);
    }
    arg1[3] = (1.0f - amount) * 255.0f;
}

extern void func_1505DFDC(void *);

typedef struct {
    u8 pad0[0x8];
    f32 pos[2];
    u8 pad10[0x8];
    f32 len[2];
    u8 pad20[0x8];
    u8 *src[2];
    u8 *dst[2];
    u8 off[2];
    u8 pad3A[2];
    s16 unk3C;
    u8 pad3E[7];
    u8 unk45;
    u8 pad46[2];
    u8 unk48;
    u8 pad49[0x1D0 - 0x49];
} Struct1502D824;

void func_1502D824(Struct1502D824 *arg0, u8 *arg1, u8 *arg2) {
    s32 n;
    u32 size;
    u8 *src;
    s32 twice;
    s32 frame;
    f32 pos;
    f32 len;
    s32 count;
    s32 frames;
    s32 i;
    u8 off;
    u8 offa;

    count = 1;
    offa = 0;
    if (arg1 != NULL) {
        if (*(u16 *)(arg1 + 0x84) == 0xFFFF) {
            func_1505DFDC(arg1);
        }
        arg0 = *(Struct1502D824 **)(arg1 + 0x2D0);
    }
    if (arg0 == NULL) {
        return;
    }
    if (arg0->unk3C > 0) {
        count = 2;
    }
    for (i = 0; i < count; i++) {
        n = arg0[i].unk45;
        twice = 0;
        if (!n) {
            arg0->off[i] = 0;
            continue;
        }
        if (arg0->src[i] == NULL) {
            continue;
        }
        pos = arg0->pos[i];
        len = arg0->len[i];
        size = n * 2;
        frame = pos;
        if (len <= frame) {
            frame = len - 1.0f;
        }
        frames = arg0[i].unk48 + 1;
        if (frames >= 2) {
            if (len - 1.0f <= frame) {
                frame = (len - 1.0f + (frames - 1)) / frames;
            } else {
                frame = frame / frames;
            }
        }
        src = arg0->src[i] + n * frame;
        offa = (u32)src & 3;
        src -= offa;
        if (len <= pos + 1) {
            twice = 1;
            size = n;
        } else {
            arg0->off[i] = n;
        }
        arg2 = (u8 *)((((u32)arg2 + 0xF) >> 4) << 4);
        size += offa;
        arg0->dst[i] = (u8 *)(offa + (u32)arg2);
        off = (u8)(s32)offa;
        if (size < 0xC8) {
            func_1000480C((u32)src, arg2, size);
        } else {
            func_10004514((s32)src, arg2, (size + 0xF) & ~0xF, 1);
        }
        arg2 += size + (off * 0);
        if (twice) {
            src = arg0->src[i];
            arg2 = (u8 *)((((u32)arg2 + 0xF) >> 4) << 4);
            off = (u32)src & 1;
            src -= off;
            size = off + n;
            if (size < 0xC8) {
                func_1000480C((u32)src, arg2, size);
            } else {
                func_10004514((s32)src, arg2, (size + 0xF) & ~0xF, 1);
            }
            arg0->off[i] = (arg2 + off) - arg0->dst[i];
            arg2 += size;
        }
    }
}

extern u16 D_800C4ED0[];

s32 func_1502DB20(s32 arg0) {
    switch (arg0) {
    case 0x3B:
    case 0x75:
    case 0x82:
    case 0x88:
    case 0x90:
    case 0x96:
    case 0x98:
    case 0x9C:
    case 0x9D:
    case 0x9F:
    case 0xA0:
    case 0xB1:
    case 0xB2:
    case 0xB4:
        return D_800C4ED0[arg0] - 4;
    }
    return D_800C4ED0[arg0];
}

extern s32 D_800C4BE0[];
extern u8 *D_8008422C[];
extern s32 D_80088554;
extern s32 D_80088558;
extern s32 D_8008857C;
extern u8 D_800C3FFA;
extern s32 func_1503CF20(s32, s32, void *, s32, s32);
extern s32 func_150838EC(GameObj *arg0, s32 arg1, s32 arg2, f32 arg3);
extern void func_15033FE0(void *, s32);
extern void func_1502D824();
extern void func_150A81D0(s32 *, s32, struct197 *, s32, s32, void *, u8 *, s32);
extern void bzero(void *, int);

void func_1502DB84(f32 arg0[4][4], s32 arg1) {
    s16 cmd[42];
    s32 id;
    u8 *ptr;
    GameObj *obj;
    s32 count;
    s32 v;
    u8 buf98[0x320];
    u8 buf54[0x44];
    struct129 *u144;

    obj = &D_800CC2D0[arg1];
    id = obj->id;
    ptr = NULL;
    if ((&D_800D19A0)[id] == 0) {
        u144 = obj->unk144;
        if (func_1503CF20(id, 0, u144, 1, obj->unkAC) != 0) {
            obj->bones = NULL;
            return;
        }
        if ((&D_800D19A0)[id] == 0) {
            obj->bones = NULL;
            return;
        }
    }
    if (obj->anim != NULL) goto ok;
    L2:
    if (func_150838EC(obj, 0, 0, 1.0f) == 0) {
    ok:
        if (D_800C4ED0[id] < 2 || obj->anim != NULL) {
            count = func_1502DB20(id);
            if ((u32)(D_800C3E7C << 6) < (u32)(D_800C3E88 + (count << 6) - D_800C3E8C)) {
                obj->bones = NULL;
                return;
            }
            func_1502FD70(obj);
            obj->bones = (struct255 *)D_800C3E88;
            {
            s32 k;
            if (D_800D1C90[id]->data.i.unk1 != 0) {
                u8 *p;

                D_800C3FFA = 0;
                bzero(buf54, D_800C4ED0[id]);
                p = D_8008422C[D_800D1C90[id]->data.i.unk1];
                while (*p != 0xFF) {
                    k = p[0];
                    buf54[k] = p[1];
                    p += 2;
                }
                ptr = buf54;
            }
            }
            func_15033FE0(cmd, arg1);
            func_1502D824(0, obj, buf98);
            {
                s16 scale;

                if (D_800C35EA != 1) {
                    scale = obj->y_scale * obj->unkCC;
                } else {
                    scale = 0;
                }
                func_150A81D0(&D_800C3E88, (s32)arg0, obj->anim, D_800C4BE0[id], count, cmd, ptr, scale);
            }
            obj->unk7C = D_8008857C;
            v = D_80088558;
            if (v != 0) {
                f32 f0;

                f0 = D_80088554 / 32768.0f;
                if (obj->unk1FC & 4) {
                    obj->unk1F8 -= v * 0.03125f;
                }
                obj->unk1F4 = f0 - obj->unk1F8;
                obj->unk1F8 = f0;
                if (obj->unk1FC & 2) {
                    obj->unk1FC &= 0xFFFC;
                    obj->unk1F4 = obj->xz_velocity;
                } else {
                    obj->unk1FC |= 1;
                }
                if (obj->unk246 & 0x40) {
                    f0 = v * 0.03125f;
                    if (1) {}
                    f0 = obj->xz_scale * f0;
                    f0 = f0 / obj->anim->frameCount[0];
                    f0 = *(volatile f32 *)&obj->xz_velocity / f0;
                    obj->anim->unk10 = f0 * 0.5f;
                }
            }
            D_800C3E7A += count;
        }
    }
}

extern void func_1502DB84(f32 mtx[4][4], s32 arg1);
typedef struct {
    u8 pad0[0xC8];
    f32 unkC8;
} Obj58F80C8;


void func_1502DF38(s32 arg0, s32 arg1) {
    f32 mtx[4][4];
    GameObj *obj;
    GameObj *other;
    s32 visible;
    s32 i;
    s32 j;
    s32 max;
    s32 v;
    f32 (*src)[4];
    f32 scale;
    void *ptr;
    s32 idx;

    obj = &D_800CC2D0[arg0];
    if (obj->xz_scale == 0.0f) {
        obj->xz_scale = 0.1000000015f;
        obj->unk154 = 0.1000000015f;
    }
    if (obj->y_scale == 0.0f) {
        obj->y_scale = 0.1000000015f;
        obj->unk158 = 0.1000000015f;
    }
    if ((obj->unk66 & 0x20) == 0x20) {
        visible = 0;
    } else {
        visible = 1;
    }
    if (obj->unk31C != NULL && obj->unk31C->unk197 != 0) {
        visible = 0;
    }
    if ((1 << arg0) & D_800C3E74) {
        visible = 0;
    }
    if (obj->id == 0x8A) {
        other = func_15072208(obj, 0);
        if (other != NULL && other->unk31C != NULL && other->unk31C->unk197 != 0) {
            visible = 0;
        }
    }
    if (visible) {
        if ((obj->unk74 & 0xF) == 0xF) {
            return;
        }
        max = 0;
        for (i = 0; i <= D_80082FA4; i++) {
            v = func_1506196C(obj, i);
            if (max < v) {
                max = v;
            }
        }
        if (max == 0) {
            return;
        }
    }
    D_800C3E78 = arg0;
    if (obj->unk65 && !(obj->unk101 & 0x10)) {
        idx = obj->unk65 - 1;
        src = (f32 (*)[4])D_800CC2D0[idx].bones;
        if (src == NULL) {
            return;
        }
        src = (f32 (*)[4])((u8 *)src + (obj->unk5C << 6));
        if (!(obj->unk101 & 4)) {
            for (i = 0; i < 3; i++) {
                if (i == 1) {
                    scale = obj->y_scale;
                } else {
                    scale = obj->xz_scale;
                }
                for (j = 0; j < 3; j++) {
                    mtx[i][j] = src[i][j] * scale;
                }
                mtx[i][3] = 0.0f;
                mtx[3][i] = src[3][i];
            }
            mtx[3][3] = 1.0f;
        } else {
            f32 len;
            for (i = 0; i < 3; i++) {
                len = sqrtf(src[i][0] * src[i][0] + src[i][1] * src[i][1] + src[i][2] * src[i][2]);
                if (i == 1) {
                    scale = obj->y_scale / len;
                } else {
                    scale = obj->xz_scale / len;
                }
                for (j = 0; j < 3; j++) {
                    mtx[i][j] = src[i][j] * scale;
                }
                mtx[i][3] = 0.0f;
                mtx[3][i] = src[3][i];
            }
            mtx[3][3] = 1.0f;
        }
    } else {
        func_150A9B0C(mtx, obj->unkB8, obj->unk40, obj->unkC4, obj->xz_scale, obj->y_scale, obj->xz_scale);
        if (obj->unk101 & 0x10) {
            other = &D_800CC2D0[obj->unk65 - 1];
            src = (f32 (*)[4])D_800CC2D0[obj->unk65 - 1].bones;
            if (src == NULL) {
                mtx[3][0] = 0;
                mtx[3][1] = 0;
                mtx[3][2] = 0;
            } else {
                src = (f32 (*)[4])((u8 *)src + (obj->unk5C << 6));
                mtx[3][0] = src[3][0];
                mtx[3][1] = src[3][1];
                mtx[3][2] = src[3][2];
            }
        } else {
            other = obj;
            mtx[3][0] = obj->x_position;
            mtx[3][1] = obj->y_position + obj->y_scale * ((Obj58F80C8 *)obj)->unkC8;
            mtx[3][2] = obj->z_position;
        }
        if (other->anim != NULL && *(s32 *)((u8 *)other->anim + 0x28) == 0) {
            mtx[3][1] += other->unkCC;
        }
    }
    if (arg1) {
        ptr = (void *)D_800C3E88;
        obj->bones = ptr;
        bcopy(mtx, ptr, 0x40);
        D_800C3E88 += 0x40;
        D_800C3E7A++;
        return;
    }
    func_1502DB84(mtx, arg0);
}

extern u8 D_800C3E90;
extern void func_150A9984(s32, s32);

void func_1502E474(void) {
    if (D_800C3E7A != 0) {
        func_150A9984(D_800C3E80[D_800BE9C0], D_800C3E7A);
    }
    D_800C3E90 = 1;
}

typedef struct {
    u8 pad0[0xE];
    u16 unkE;
    s16 unk10;
    u8 pad12[0x2C];
    u16 unk3E;
} Ent1502E4C4;

extern u8 D_800C3660;
extern u8 D_800D9C10[][0x40];
extern s32 func_150A6360(s32, void *, f32, f32, f32, f32, f32, f32);
extern s32 func_1503F904(GameObj *, s32, s32);
void func_1502E9FC(s32 arg0, s32 arg1);

void func_1502E4C4(s32 arg0) {
    GameObj *obj;
    u8 id;
    u8 *tbl;
    s32 base;
    f32 width;
    f32 height;
    f32 yoff;
    f32 thresh;
    s32 maxv;
    s32 i;
    s32 mask;
    s32 flag;
    s32 lim0;
    s32 lim;
    s32 old;
    s32 cmd;

    mask = 1;
    obj = &D_800CC2D0[arg0];
    id = obj->id;
    if (obj->unk5 == 7) {
        obj->unk74 &= 0xFFF0;
        return;
    }
    old = obj->unk74;
    obj->unk74 |= 0xF;
    if ((D_800C3638 != 0) && (D_800C3656 == 0) && (func_150229E4(obj) == 0)) {
        return;
    }
    if ((obj->unk127 != 0xFF) && !((u16)D_800D2340 & (1 << obj->unk127))) {
        return;
    }
    if ((obj->unk101 & 1) == 1) {
        return;
    }
    if (obj->unk7 == 0) {
        return;
    }
    if (obj->unk65 != 0) {
        obj->unk74 = D_800CC2D0[obj->unk65 - 1].unk74;
        return;
    }
    width = (u32)((Ent1502E4C4 *)D_800D1C90[id])->unkE * obj->xz_scale;
    height = (u32)((Ent1502E4C4 *)D_800D1C90[id])->unkE * obj->y_scale;
    yoff = (((Ent1502E4C4 *)D_800D1C90[id])->unk10 + obj->unkCC) * obj->y_scale;
    *(f32 *)((u8 *)obj + 0x270) = width;
    lim0 = *(u16 *)&obj->unk10;
    if ((D_800C35EA == 1) && (D_800C3660 == 0)) {
        lim0 = 0;
    }
    maxv = 0;
    if (obj->unk101 & 0x80) {
        thresh = 0.0f;
    } else {
        if (((Ent1502E4C4 *)D_800D1C90[id])->unk3E == 0) {
            thresh = obj->unkE4;
        } else if (((Ent1502E4C4 *)D_800D1C90[id])->unk3E == 1) {
            thresh = 0.0f;
        } else {
            thresh = (u32)((Ent1502E4C4 *)D_800D1C90[id])->unk3E * obj->xz_scale;
        }
    }
    for (i = 0; i <= D_80082FA0; i++) {
        s32 r;

        base = i * 0x180 + D_800BE628;
        tbl = D_800D9C10[i];
        flag = 0;
        if (D_800DBFF0[i].unk3D4->unk197 != 0) {
            lim = 100000;
        } else {
            lim = lim0;
        }
        r = func_150A6360(base, tbl, obj->x_position, obj->y_position + yoff,
                          obj->z_position, width, height, 20000.0f);
        if (r == 0) {
            flag = 1;
        } else {
            if (r < 0) {
                r = -r;
            }
            if (maxv < r) {
                maxv = r;
            }
            if (((obj->unk66 & 0x80) != 0x80) && (r <= thresh) && (D_800C35EA != 1)) {
                flag = 1;
            }
        }
        if (flag == 0) {
            if ((lim != 0) && (i != arg0)) {
                if (lim < r) {
                    cmd = 8;
                } else {
                    cmd = 9;
                }
                func_1506160C(obj, cmd & 0xFF, 0, 0, i);
            } else {
                obj->unkF |= (1 << i) << 4;
                func_1506160C(obj, 9, 0, 0, i);
                cmd = -1;
            }
            if ((obj->unkF8 & 0x800000) && (cmd != 8) && (D_800C35EA != 1) && (func_1503F904(obj, i, old) != 0)) {
                flag = 1;
            }
        } else {
            cmd = 10;
        }
        if (cmd != -1) {
            func_1506160C(obj, cmd & 0xFF, 1, 0, i);
        }
        if (flag == 0) {
            obj->unk74 &= ~mask;
        }
        if (func_1506196C(obj, i) == 0) {
            obj->unk74 |= mask;
        }
        mask <<= 1;
    }
    func_1502E9FC((s32)obj, maxv);
    if ((obj->camera != NULL) && ((obj->camera->unk2C == 0x10000) || (obj->unk31C->unk197 != 0))) {
        obj->unk74 |= 1 << arg0;
    }
    if (D_800C35EA != 1) {
        if (obj->unk2FC != 0) {
            obj->unk74 |= obj->unk2FC;
        }
    }
}

void func_1502E9FC(s32 arg0, s32 arg1) {
}

void func_1502EA0C(ActorInteractionFields *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    arg0->field_0xA4 = 4;
    arg0->field_0xA5 = 0;
    arg0->field_0xA6 = arg5;
    arg0->field_0xA0 = (arg4 << 24) | (arg1 << 16) | (arg2 << 8) | arg3;
    arg0->field_0xA7 = 0xFF;
}

typedef struct {
    u8 pad0[0xA4];
    /* 0xA4 */ u8 unkA4;
} S1502EA50;

void func_1502EA50(S1502EA50 *arg0) {
    arg0->unkA4 = 0x5;
}

void func_1502EA60(u8 *arg0, s32 arg1) {
    arg0[0xA4] = 2;
    arg0[0xA5] = 0xFF;
    arg0[0xA6] = arg1;
}

void func_1502EA7C(u8 *arg0, s32 arg1) {
    arg0[0xA4] = 3;
    arg0[0xA5] = 0xFF;
    arg0[0xA6] = arg1;
}

void func_1502EA98(ActorInteractionFields *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    if (arg0->field_0xA4 != 6 && arg0->field_0xA4 != 7) {
        if (arg5 != 0) {
            arg0->field_0xA5 = 0xFF;
        } else {
            arg0->field_0xA5 = 0;
        }
        arg0->field_0xA0 = (arg4 << 24) | (arg1 << 16) | (arg2 << 8) | arg3;
        arg0->field_0xA6 = arg6;
    }
    arg0->field_0xA4 = 6;
}

void func_1502EAFC(GameObj *arg0) {
    switch (*(u8 *)((u8 *)arg0 + 0xA4)) {
    case 2:
    case 3: {
        s32 lo = *(u8 *)((u8 *)arg0 + 0xA6) * D_800BE9E4;
        u8 v1 = *(u8 *)((u8 *)arg0 + 0xA5);

        if (lo < (s32) v1) {
            *(u8 *)((u8 *)arg0 + 0xA5) = (u8) (v1 - lo);
        } else {
            *(u8 *)((u8 *)arg0 + 0xA5) = 0U;
        }
    }
        break;
    case 5: {
        s32 t1 = D_800BE9E4 * 0xA;
        u8 v1 = *(u8 *)((u8 *)arg0 + 0xA7);

        if (t1 < (s32) v1) {
            *(u8 *)((u8 *)arg0 + 0xA7) = (u8) (v1 - t1);
        } else {
            *(u8 *)((u8 *)arg0 + 0xA4) = 0U;
        }
    }
        /* fallthrough */
    case 4:
        *(u8 *)((u8 *)arg0 + 0xA5) += *(u8 *)((u8 *)arg0 + 0xA6) * D_800BE9E4;
        break;
    case 6: {
        s32 lo = *(u8 *)((u8 *)arg0 + 0xA6) * D_800BE9E4;
        u8 v1 = *(u8 *)((u8 *)arg0 + 0xA5);

        if ((s32) v1 < (0xFF - lo)) {
            *(u8 *)((u8 *)arg0 + 0xA5) = (u8) (v1 + lo);
        } else {
            *(u8 *)((u8 *)arg0 + 0xA5) = 0xFFU;
        }
        *(u8 *)((u8 *)arg0 + 0xA4) = 7U;
    }
        break;
    case 7: {
        s32 lo = *(u8 *)((u8 *)arg0 + 0xA6) * D_800BE9E4;
        u8 v1 = *(u8 *)((u8 *)arg0 + 0xA5);

        if (lo < (s32) v1) {
            *(u8 *)((u8 *)arg0 + 0xA5) = (u8) (v1 - lo);
        } else {
            *(u8 *)((u8 *)arg0 + 0xA4) = 0U;
        }
    }
        break;
    }
}

/* Fog colour (r, g, b, a) of the object's current fog effect (see func_1502EAFC for the modes). */
void func_1502EC34(GameObj *arg0, s32 *r, s32 *g, s32 *b, s32 *a) {
    GameObj *obj = arg0; // matching: the parameter copy gets its own stack slot
    f32 level;

    switch (obj->fogMode) {
    case 1:
        *r = obj->fogLevel;
        *g = obj->fogRate;
        *b = obj->fogFade;
        if (obj->fogColor >= 0x100) {
            level = *(f32 *)obj->fogColor;
            level = (level < 0.0f) ? 0.0f : ((level > 255.0f) ? 255.0f : level);
            *a = level * 2.55f;
        } else {
            *a = obj->fogColor;
        }
        break;
    case 2:
    case 3:
        *r = 0;
        *g = 0;
        *b = 0;
        *a = obj->fogLevel;
        if (obj->fogMode == 3) {
            *a = 0xFF - *a;
        }
        break;
    case 4:
    case 5: {
        s32 alpha;
        f32 pulse;

        alpha = ((s32)obj->fogColor >> 24) & 0xFF;
        *r = ((s32)obj->fogColor >> 16) & 0xFF;
        *g = ((s32)obj->fogColor >> 8) & 0xFF;
        *b = obj->fogColor & 0xFF;
        pulse = (cosf(obj->fogLevel * 0.02454369329f) + 1.0f) * 0.5f;
        *a = alpha + 64.0f * pulse;
        *a += ((0xFF - *a) * (0xFF - obj->fogFade)) >> 8;
    }
        break; // matching: outside the block, so the store stays out of the branch delay slot
    case 6:
    case 7: {
        s32 alpha;

        alpha = ((s32)obj->fogColor >> 24) & 0xFF;
        *r = ((s32)obj->fogColor >> 16) & 0xFF;
        *g = ((s32)obj->fogColor >> 8) & 0xFF;
        *b = obj->fogColor & 0xFF;
        *a = 0xFF - ((obj->fogLevel * alpha) >> 8);
        break;
    }
    }
}

/* The two u8 slots at actor record +0x6A (GameObj still calls this word pad68). */
typedef struct Act1502EE8C {
    char pad0[0x6A];
    /* 0x6A */ u8 unk6A[2];
    char pad6C[0x32C - 0x6C];
} Act1502EE8C;

s32 func_1502EE8C(s32 arg0, s32 arg1) {
    s32 v;
    s32 r;

    v = ((Act1502EE8C *)D_800CC2D0)[arg0].unk6A[arg1];
    if (v >= 2) {
        if (v >= 4) {
            r = 2;
        } else {
            r = v - 2;
        }
    } else {
        r = v;
    }
    return r;
}


/* The two u8 counters at actor record +0x6C (GameObj still calls this pad6C). */
typedef struct Act1502EEF4 {
    char pad0[0x6C];
    /* 0x6C */ u8 unk6C[2];
    char pad6E[0x32C - 0x6E];
} Act1502EEF4;

extern void func_1507E3C0(GameObj *);

void func_1502EEF4(s32 arg0) {
    s32 i;
    u8 r;

    for (i = 0; i < 2; i++) {
        if (((Act1502EEF4 *)D_800CC2D0)[arg0].unk6C[i] >= 10) {
            continue;
        }
        r = func_1502EE8C(arg0, i);
        if (r == 0) {
            if (((Act1502EEF4 *)D_800CC2D0)[arg0].unk6C[i] > 0) {
                ((Act1502EEF4 *)D_800CC2D0)[arg0].unk6C[i]--;
            }
        } else if (r == 1) {
            if (((Act1502EEF4 *)D_800CC2D0)[arg0].unk6C[i] < 2) {
                ((Act1502EEF4 *)D_800CC2D0)[arg0].unk6C[i]++;
            }
        } else {
            ((Act1502EEF4 *)D_800CC2D0)[arg0].unk6C[i] = 1;
        }
    }
    func_1507E3C0(&D_800CC2D0[arg0]);
}

u8 *func_1507E908(void *, s32);
typedef struct {
    u8 pad0[8];
    u8 e[1][3];
} Fn1502F01CTbl;
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Fn1502F01CTex;
extern Fn1502F01CTex *D_800C5338[];

Gfx *func_1502F01C(Gfx *gdl, s32 arg1) {
    u8 *obj;
    u8 sp80[2];
    s32 id;
    u8 *res;
    s32 i;
    Fn1502F01CTex *tex;

    obj = (u8 *)&D_800CC2D0[arg1];
    id = obj[4];
    res = NULL;
    for (i = 0; i < 2; i++) {
        if (obj[0x6C + i] >= 10) {
            sp80[i] = obj[0x6C + i] - 10;
        } else {
            sp80[i] = ((Fn1502F01CTbl *)D_800D1C90[id])->e[i][obj[0x6C + i]];
            if (obj[0x6F] && !res) {
                res = func_1507E908(obj, obj[0x6F]);
            }
            if (res != NULL) {
                s32 sp68[2];
                sp68[0] = res[0];
                sp68[1] = res[1];
                if (res != NULL) {
                    if (sp68[i] != ((Fn1502F01CTbl *)D_800D1C90[id])->e[i][2]) {
                        if (obj[0x6C + i] == 0) {
                            sp80[i] = sp68[i];
                        }
                    } else {
                        sp80[i] = sp68[i];
                    }
                }
            }
        }
    }
    tex = D_800C5338[obj[4]];
    if (tex != NULL) {
        gSPSegment(gdl++, 6, tex[sp80[0]].unk0);
        gSPSegment(gdl++, 7, tex[sp80[1]].unk0);
        gSPSegment(gdl++, 10, tex[obj[0x68]].unk0);
        gSPSegment(gdl++, 11, tex[obj[0x69]].unk0);
    }
    return gdl;
}

void func_1502F264(s32 arg0) {
    GameObj *obj;
    GameObj *parent;
    f32 (*bone)[4][4];
    s32 i;

    obj = &D_800CC2D0[arg0];
    if (obj->unk65 == 0) {
        return;
    }
    parent = &D_800CC2D0[obj->unk65 - 1];
    if ((obj->unk101 & 2) == 2) {
        return;
    }
    bone = (f32 (*)[4][4])parent->bones;
    if (bone == NULL) {
        obj->x_position = parent->x_position;
        obj->y_position = parent->y_position;
        obj->z_position = parent->z_position;
    } else {
        bone += obj->unk5C;
        obj->x_position = (*bone)[3][0];
        obj->y_position = (*bone)[3][1];
        obj->z_position = (*bone)[3][2];
    }
    obj->unk180 = parent->unk180;
    obj->y_velocity = -4.0f;
    if (!(obj->unk101 & 0x20)) {
        obj->unk76 = parent->unk76;
        obj->xz_velocity = parent->xz_velocity;
        obj->unk40 = parent->unk40;
        obj->unk76 = parent->unk76; /* copied twice in the original */
        obj->unk7A = parent->unk7A;
        obj->unk78 = parent->unk78;
    }
    if (!(obj->unk101 & 0x40)) {
        obj->unk7 = parent->unk7;
        obj->unk8 = parent->unk8;
        obj->unk9 = parent->unk9;
        obj->unkA = parent->unkA;
        obj->unkF = parent->unkF;
        for (i = 0; i <= D_80082FA0; i++) {
            obj->unkB[i] = parent->unkB[i];
        }
    }

}

typedef struct {
    u32 start;
    s32 count;
    s32 id;
} Seg1502F490;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x1D4 - 5];
    u8 *unk1D4;
    u8 *unk1D8;
} Obj1502F490;

typedef struct {
    /* 0x000 */ s32 active;
    /* 0x004 */ u8 pad4[0x10];
    /* 0x014 */ f32 field_14;
    /* 0x018 */ f32 field_18;
    /* 0x01C */ f32 field_1C;
    /* 0x020 */ u8 pad20[0x180 - 0x20];
    /* 0x180 */ f32 field_180;
    /* 0x184 */ u8 pad184[0x19E - 0x184];
    /* 0x19E */ u16 field_19E;
    /* 0x1A0 */ u8 pad1A0[0x274 - 0x1A0];
    /* 0x274 */ u8 field_274;
    /* 0x275 */ u8 pad275[0x32C - 0x275];
} Actor1502F3C8;

#define ACTORS_1502F3C8 (*(Actor1502F3C8 (*)[25])&D_800CC2D0)

void func_1502F490(Obj1502F490 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, s32 arg4);

void func_1502F3C8(void) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if (ACTORS_1502F3C8[i].active == 0) {
            continue;
        }
        if (ACTORS_1502F3C8[i].field_274 == 0) {
            continue;
        }
        ACTORS_1502F3C8[i].field_18 = ACTORS_1502F3C8[i].field_180;
        func_1502F490((Obj1502F490 *)&ACTORS_1502F3C8[ACTORS_1502F3C8[i].field_274 - 1], &ACTORS_1502F3C8[i].field_14, &ACTORS_1502F3C8[i].field_18, &ACTORS_1502F3C8[i].field_1C, ACTORS_1502F3C8[i].field_19E);
        if (ACTORS_1502F3C8[i].field_18 < ACTORS_1502F3C8[i].field_180) {
            ACTORS_1502F3C8[i].field_18 = ACTORS_1502F3C8[i].field_180;
        } else {
            ACTORS_1502F3C8[i].field_180 = ACTORS_1502F3C8[i].field_18;
        }
    }
}


extern u16 D_800C5EF8[];
void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

void func_1502F490(Obj1502F490 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, s32 arg4) {
    f32 diff[3];
    f32 res[3];
    s32 idx[3];
    s32 i;
    s32 j;
    s32 mask;
    s32 seg;
    u8 id;
    u32 p[3];
    f32 v[2][3][3];
    f32 e1[2][3];
    f32 e2[2][3];
    u8 *mtx;
    f32 s;
    f32 t;

    id = arg0->unk4;
    if (D_800C6070[id] == 0) {
        return;
    }
    for (i = 0; i < 3; i++) {
        p[i] = *(s32 *)(D_800C6070[id] + arg4 * 12 + i * 4) + (&D_800D19A0)[id];
    }
    seg = 0;
    mask = 0;
    do {
        if (seg == D_800C5EF8[id]) {
            return;
        }
        for (i = 0; i < 3; i++) {
            if (!(mask & (1 << i))) {
                if (p[i] >= ((Seg1502F490 *)D_800C5C08[id])[seg].start && p[i] < ((Seg1502F490 *)D_800C5C08[id])[seg].start + ((Seg1502F490 *)D_800C5C08[id])[seg].count * 16) {
                    mask = (1 << i) | mask;
                    idx[i] = ((Seg1502F490 *)D_800C5C08[id])[seg].id;
                }
            }
        }
        seg++;
    } while (mask != 7);

    if (arg0->unk1D8 == NULL || arg0->unk1D4 == NULL) {
        return;
    }
    mtx = arg0->unk1D8;
    for (i = 0; i < 2; i++) {
        if (i == 1) {
            mtx = arg0->unk1D4;
        }
        for (j = 0; j < 3; j++) {
            func_150A7960((f32 *)(mtx + (idx[j] << 6)), ((s16 *)p[j])[0], ((s16 *)p[j])[1], ((s16 *)p[j])[2],
                          &v[i][j][0], &v[i][j][1], &v[i][j][2]);
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            e1[i][j] = v[i][1][j] - v[i][0][j];
            e2[i][j] = v[i][2][j] - v[i][0][j];
        }
    }
    diff[0] = *arg1 - v[0][0][0];
    diff[1] = *arg2 - v[0][0][1];
    diff[2] = *arg3 - v[0][0][2];
    s = e1[0][0] * e2[0][2] - e1[0][2] * e2[0][0];
    if (s != 0.0f) {
        s = (diff[0] * e2[0][2] - diff[2] * e2[0][0]) / s;
    } else {
        s = -100.0f;
    }
    if (s < 0.0f || s > 1.0f) {
        return;
    }
    if (e2[0][2] != 0.0f) {
        t = (diff[2] - s * e1[0][2]) / e2[0][2];
    } else {
        t = -100.0f;
    }
    if (t < 0.0f || t > 1.0f) {
        return;
    }
    for (i = 0; i < 3; i++) {
        res[i] = s * e1[1][i] + e2[1][i] * t;
    }
    diff[1] = s * e1[0][1] + e2[0][1] * t;
    *arg1 += res[0] + v[1][0][0] - diff[0] - v[0][0][0];
    *arg2 += res[1] + v[1][0][1] - diff[1] - v[0][0][1];
    *arg3 += res[2] + v[1][0][2] - diff[2] - v[0][0][2];
}

/* Keeps a copy of the object's bone matrices in prevBones (allocated on first use). */
void func_1502F948(GameObj *obj) {
    s32 id;
    void *copy;

    if ((obj->unkF8 & 0x4000) && obj->unk264 != 0 && obj->bones != NULL) {
        id = obj->id;
        if (obj->prevBones == NULL) {
            copy = allocate_memory(D_800C4ED0[id] * sizeof(*obj->bones), 1, 1, 2);
            obj->prevBones = copy;
            if (copy == NULL) {
                return;
            }
        }
        bcopy(obj->bones, obj->prevBones, D_800C4ED0[id] * sizeof(*obj->bones));
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} Fn1502F9FCTex;
Gfx *func_150C3160(Gfx *, void *);

Gfx *func_1502F9FC(Gfx *gdl, s32 arg1) {
    s32 id;
    s32 idx;
    s32 i;
    Fn1502F9FCTex *tex;
    s32 uls;
    s32 ult;
    s32 lrs;
    s32 lrt;
    s32 tile;

    id = D_800CC2D0[arg1].id;
    if (id == 0x89 || id == 0xBA) {
        return func_150C3160(gdl, &D_800CC2D0[arg1]);
    }
    if (id == 0) {
        idx = 14;
    } else if (id == 0x96) {
        idx = 7;
    } else if (id == 0x28) {
        idx = 4;
    } else if (id == 1 || id == 2 || id == 3 || id == 4) {
        idx = 5;
    } else {
        idx = 0;
    }
    if (D_800C5338[id] != NULL) {
        for (i = 0; i < 2; i++) {
            tex = &((Fn1502F9FCTex *)D_800C5338[id])[idx];
            uls = (u16)(((s16 *)((u8 *)&D_800CC2D0[arg1] + 0x27A))[i] + 2);
            ult = (u16)(((s16 *)((u8 *)&D_800CC2D0[arg1] + 0x27E))[i] + 2);
            lrs = (u16)(tex->unk8 * 4 - 2);
            lrt = (u16)(tex->unkA * 4 - 2);
            tile = 5 - i;
            gDPSetTileSize(gdl++, tile, (u16)uls, (u16)ult, (u16)lrs, (u16)lrt);
        }
    }
    return gdl;
}

extern void func_1507EABC(void *);

#define actor ((u8 *)arg0)
void func_1502FBE8(GameObj *arg0) {
    s32 next;
    void *old;
    s32 selection;

    if (actor[0x1C9] == 0) {
        return;
    }
    old = ((void **)D_800D1588)[actor[4]];
    if (actor[0x1C9] == 0xFF) {
        actor[0x1C9] = 0;
        next = 0;
    } else {
        next = actor[0x1C9] - 1;
        if (next >= actor[0x2C9] || next == actor[0x1C8]) {
            return;
        }
    }
    selection = (*(u8 **)(actor + 0x2C4))[next];
    func_150837D4((actor - (u8 *)D_800CC2D0) / 0x32C, selection, 0);
    actor[0x1C8] = next;
    if (D_800C35EA == 1) {
        D_800C3638 = 0;
    }
    if (old != ((void **)D_800D1588)[selection]) {
        if (*(u16 *)(actor + 0x84) >= ((u16 *)D_800C5A90)[selection]) {
            *(u16 *)(actor + 0x84) = 0xFFFF;
            func_1505E650(arg0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
        } else {
            s32 anim = *(u16 *)(actor + 0x84);

            *(u16 *)(actor + 0x84) = 0xFFFF;
            func_1505E650(arg0, anim, 1.0f, 0.0f, 0.0f, 0.0f, 0);
        }
    }
    if (D_800C35EA == 1) {
        D_800C3638 = 1;
    }
    func_1507EABC(actor);
}
#undef actor

typedef struct {
    u8 pad0[0x2E];
    u8 timer;
} Sub_1502FD70;

typedef struct {
    u8 pad0[4];
    u8 id;
    u8 pad5[0x144 - 5];
    Sub_1502FD70 *sub;
} Actor_1502FD70;

void func_1502FD70(GameObj *arg0) {
    Actor_1502FD70 *actor = (Actor_1502FD70 *)arg0;
    u8 id = actor->id;
    u8 *slot;

    if ((D_80038080 == 0) && (D_800BE9F0 == 0x1D)) {
        ((u8 *)D_800D2040)[id] = 2;
        return;
    }
    slot = &((u8 *)D_800D2040)[id];
    if (*slot != 0xFF) {
        if (actor->sub != NULL) {
            if (actor->sub->timer != 0xFF) {
                if (actor->sub->timer != 0) {
                    if (*slot < actor->sub->timer * 30) {
                        *slot = actor->sub->timer * 30;
                    }
                } else {
                    goto set3;
                }
            }
        } else {
set3:
            *slot = 3;
        }
    }
}
