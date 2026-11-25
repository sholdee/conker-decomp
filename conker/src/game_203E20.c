#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_150D6730(struct127 *obj, u8 arg1, s32 arg2);
extern s32 func_15042E3C(char *format, ...);
extern Gfx *func_15133EEC(Gfx *gfx, u16 index, u8 segment, s32 arg3);
extern u32 D_80083740[];
extern u32 D_800838C0[];

/* Header shared by the objects func_15149130 creates (same layout as in game_FC5F0.c). */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE; // set to -1 to remove the object
    /* 0x10 */ u8 pad10[0x28 - 0x10];
} ObjHeader;

/* Text box data (same layout as TextBox in game_FC5F0.c). */
typedef struct {
    /* 0x00 */ u8 id;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ u8 *str;
    /* 0x08 */ u8 flags;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ u8 *line;
    /* 0x10 */ u8 *lineEnd;
    /* 0x14 */ u8 lineLen;
    /* 0x15 */ u8 page;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ u8 *lines[2];
    /* 0x20 */ void *style; // the display style's own state
    /* 0x24 */ s8 updateFn; // index into D_800888A0, or -1
    /* 0x25 */ u8 msgFn; // index into D_800888B0
    /* 0x26 */ u8 pad26[2];
} TextBox;

/* Text box object created by func_150CFF10 (same layout as TextObj in game_FC5F0.c). */
typedef struct {
    /* 0x00 */ ObjHeader header;
    /* 0x28 */ TextBox text;
} TextObj;

/* Style 3 text box state: an amount shown as "$ %d" that fades out with its owner. */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId; // owner->unique_id when the box was made
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ s32 amount;
    /* 0x0C */ s16 fadeStart; // alpha fades once the lifetime drops below this
    /* 0x0E */ s16 fadeRate;
} AmountTextStyle;

/* Called when `character` touches a pulse object (game_1483E0.c): spawns func_150D6730's particles on it
   while D_800BE9F0 is 0x32 or 0x33. */
void func_151D6970(struct127 *character, void *obj) {
    if ((D_800BE9F0 == 0x32) || (D_800BE9F0 == 0x33)) {
        func_150D6730(character, 0xFF, 1);
    }
}

/* Parameter block for func_15153F18 (Params15153F18 in game_16EE20.c). */
typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    f32 pos[3];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    u8 unk3C;
    u8 pad3D[3];
    f32 unk40;
    s16 unk44;
    s16 unk46;
    s32 unk48;
} Params15153F18;

/* Parameter block for func_15150178. */
typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    f32 pos[3];
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    u8 unk2C;
    u8 unk2D;
    u8 pad2E[2];
    f32 unk30;
    f32 unk34;
    u8 unk38;
    u8 unk39;
    u8 pad3A[2];
    f32 unk3C;
    u8 unk40;
    u8 pad41[3];
    f32 unk44;
} Params15150178;

/* Pickup object (Obj15119FC0 in game_147470.c; only the fields used here). */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s16 position[3];
} Pickup;

extern void func_1504715C(void *ground, struct127 *actor);
extern void func_15153F18(s16 *params, f32 *pos, void *ground, s32 arg3, s32 arg4);
extern void func_15150178(s16 *params, f32 *pos, void *ground, s32 arg3, s32 arg4);

/* Spawns the func_15153F18 and func_15150178 bursts at `pickup`, on the ground under `player`. */
void func_151D69B4(Pickup *pickup, struct127 *player) {
    u8 ground[0x24]; // ground probe result (Ground in game_1228D0.c)
    Params15153F18 first;

    func_1504715C(ground, player);
    first.pos[0] = pickup->position[0];
    first.pos[1] = pickup->position[1];
    first.pos[2] = pickup->position[2];
    first.unk14 = 2.49000001f;
    first.unk2C = 3;
    first.unk2E = 3;
    first.unk02 = 0xFF;
    first.unk04 = -0x2B;
    first.unk00 = 0;
    first.unk06 = 0x20;
    first.unk30 = 3;
    first.unk32 = 2;
    first.unk34 = 0x28;
    first.unk36 = 0x14;
    first.unk38 = 0x9B;
    first.unk3A = 0x64;
    first.unk44 = 0x10;
    first.unk46 = 0xF;
    first.unk48 = 0;
    first.unk3C = 9;
    first.unk18 = 2.029999971f;
    first.unk1C = 0.6080000401f;
    first.unk20 = 0.2569999993f;
    first.unk24 = 17.0f;
    first.unk28 = 3.5f;
    first.unk40 = 0.400000006f;
    func_15153F18(&first.unk00, first.pos, ground, 0xFF, 1);
    {
    Params15150178 second;

    second.pos[0] = pickup->position[0];
    second.pos[1] = pickup->position[1];
    second.pos[2] = pickup->position[2];
    second.unk14 = 0xF;
    second.unk16 = 6;
    second.unk00 = 0;
    second.unk02 = 0xFF;
    second.unk04 = -0x40;
    second.unk06 = 0xC;
    second.unk20 = 0x3C;
    second.unk22 = 0x2D;
    second.unk2C = 0xC8;
    second.unk2D = 0x37;
    second.unk38 = 1;
    second.unk39 = 9;
    second.unk3C = 1.0f;
    second.unk40 = 0;
    second.unk44 = 1.0f;
    second.unk18 = 10.0f;
    second.unk1C = 6.0f;
    second.unk24 = -1.506000042f;
    second.unk28 = 0.8140000105f;
    second.unk30 = 44.0f;
    second.unk34 = 88.0f;
    func_15150178(&second.unk00, second.pos, ground, 0xFF, 1);
    }
}

/* Effect spawn parameters (same layout as EffectParams in game_1844C0.c). */
typedef struct {
    /* 0x00 */ u8 flags;
    /* 0x01 */ s8 updateFunc;   // index into D_8008AD90, -1 = none
    /* 0x02 */ s8 updateFunc2;  // index into D_8008ADA0, -1 = none
    /* 0x03 */ s8 preDrawFunc;  // index into D_8008ADBC, -1 = none
    /* 0x04 */ s8 postDrawFunc; // index into D_8008ADCC, -1 = none
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 lifetime;
    /* 0x08 */ s32 model;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 otherModeH;
    /* 0x14 */ s32 geometrySet;
    /* 0x18 */ s32 geometryClear;
    /* 0x1C */ s32 renderMode;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 otherModeH2;
    /* 0x28 */ s32 otherModeH3;
    /* 0x2C */ s8 primMode;
    /* 0x2D */ s8 envMode;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ u8 color0[4];
    /* 0x34 */ u8 color1[4];
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ struct127 *owner;
    /* 0x40 */ u8 ownerId;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ struct17 pos;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 fadeStart; // alpha fades once lifetime drops below this
    /* 0x56 */ s16 fadeRate;
} EffectParams; // size 0x58

/* Model effect object (Effect in game_1844C0.c; only the fields used here). */
typedef struct {
    /* 0x00 */ u8 header[0x10];
    /* 0x10 */ EffectParams params;
    /* 0x68 */ u8 pad68[0x7C - 0x68];
    /* 0x7C */ f32 mtx[2][4][4]; // per frame
} ModelEffect;

typedef struct {
    /* 0x00 */ u8 pad0[0x64];
    /* 0x64 */ s32 unk64;
} Struct1509B570;

extern void *func_15157010(EffectParams *params, s32 arg1, f32 arg2, s32 arg3, s32 arg4, s32 extraSize, u8 owner,
                           s32 list);
extern TextObj *func_150CFF10(u8 id, u8 *str, s16 duration, s32 styleSize, s8 updateFn, u8 msgFn, u8 arg6, s32 arg7);
extern Struct1509B570 *func_1509B570(s32);

/*
 * Spawns model effect 0xA5 on `owner` (updated by func_151D6E60, drawn with func_151D710C) and a text box
 * showing a random amount as "$ %d" (style 3: func_151D7000 / func_151D70CC).
 */
void func_151D6BFC(struct127 *owner, u8 arg1, s32 arg2) {
    EffectParams params;
    Struct1509B570 *entry;

    params.unk38 = 0;
    params.flags = 0x27;
    params.updateFunc = -1;
    params.updateFunc2 = 3;
    params.preDrawFunc = 1;
    params.postDrawFunc = -1;
    params.lifetime = 150;
    params.model = 0xA5;
    params.unkC = 0x17;
    params.otherModeH = 0;
    params.geometrySet = 0x220405;
    params.geometryClear = 0x40200;
    params.envMode = 8;
    params.renderMode = 1;
    params.unk20 = 0x38;
    params.primMode = 0;
    params.otherModeH2 = 0x80;
    params.otherModeH3 = 0x20;
    params.color0[0] = 0xFF;
    params.color0[1] = 0xFF;
    params.color0[2] = 0xFF;
    params.color0[3] = 0xFF;
    params.color1[0] = 0xFF;
    params.color1[1] = 0xFF;
    params.color1[2] = 0xFF;
    params.color1[3] = 0xFF;
    params.owner = owner;
    params.ownerId = owner->unique_id;
    params.unk41 = 1;
    params.pos = *(struct17 *)&D_800A5480;
    params.fadeStart = 8;
    params.fadeRate = 0x1F;
    func_15157010(&params, 0, 1.0f, 0, 0, 0, arg1, arg2);
    {
    AmountTextStyle style;
    char str[4] = "500";
    s32 scale;
    TextObj *textObj;
    style.owner = owner;
    style.ownerId = owner->unique_id;
    style.fadeStart = 8;
    style.fadeRate = 0x1F;
    style.amount = func_150859AC(0, 6);
    entry = func_1509B570(0x83);
    if (entry != NULL) {
        scale = entry->unk64 + 1;
        if (scale == 0xFF) {
            style.amount += 100;
        } else if (scale == 0) {
            style.amount = 0;
        } else if (scale < 3) {
            s32 scaled = style.amount * scale; // matching: the product needs its own local

            style.amount = scaled / 3;
        }
    } else {
        style.amount = 0;
    }
    textObj = func_150CFF10(0x63, str, 150, sizeof(AmountTextStyle), 3, 1, arg1, arg2);
    if (textObj != NULL) {
        memcpy(textObj->text.style, &style, sizeof(AmountTextStyle));
    }
    }
}

extern f32 func_15144AA8(s32);

/*
 * Effect update (D_8008ADA0[3]): this frame's matrix is func_150A8050's for (0, func_15144AA8(0) + 25, 0),
 * placed 120 units above the owner and scaled by 0.551.
 */
s32 func_151D6E60(ModelEffect *fx) {
    f32 scale;

    func_150A8050(fx->mtx[D_800BE9C0], 0.0f, func_15144AA8(0) + 25.0f, 0.0f);

    scale = 0.5509999990f;
    fx->mtx[D_800BE9C0][3][0] = fx->params.owner->x_position;
    fx->mtx[D_800BE9C0][3][1] = fx->params.owner->y_position + 120.0f;
    fx->mtx[D_800BE9C0][3][2] = fx->params.owner->z_position;

    fx->mtx[D_800BE9C0][0][0] *= scale;
    fx->mtx[D_800BE9C0][0][1] *= scale;
    fx->mtx[D_800BE9C0][0][2] *= scale;
    fx->mtx[D_800BE9C0][1][0] *= scale;
    fx->mtx[D_800BE9C0][1][1] *= scale;
    fx->mtx[D_800BE9C0][1][2] *= scale;
    fx->mtx[D_800BE9C0][2][0] *= scale;
    fx->mtx[D_800BE9C0][2][1] *= scale;
    fx->mtx[D_800BE9C0][2][2] *= scale;

    return 1;
}

/*
 * Text box update for style 3 (D_800888A0[3]): removes the box (returns 0) once its owner is gone or
 * replaced, otherwise prints the amount, fading out over the last fadeStart frames.
 */
s32 func_151D7000(TextObj *obj) {
    struct127 *owner;
    AmountTextStyle *style;
    s16 timeLeft;
    s16 pad; // matching: unused, but alpha's register allocation depends on it
    s16 alpha;

    style = obj->text.style;
    owner = style->owner;
    if ((owner->interaction_state == 0) || (owner->unique_id != style->ownerId)) {
        return 0;
    }

    timeLeft = obj->header.unkE;
    if (timeLeft < style->fadeStart) {
        alpha = timeLeft * style->fadeRate;
        if (alpha < 0) {
            alpha = 0;
        }
    } else {
        alpha = 0xFF;
    }

    func_150432CC((s32)owner, 0x109);
    func_1504332C(0x32, 0x7D, 0x1C, alpha);
    func_15042D78(0x81);
    return func_15042E3C("$ %d", style->amount);
}

/* Text box message handler for style 3 (D_800888B0[1]): keeps the owner reference up to date. */
void func_151D70CC(TextObj *obj, u8 *payload, u8 type) {
    AmountTextStyle *style = obj->text.style;

    func_15169850((s32)payload, type, (s32)&style->owner, (s32)&style->ownerId, (s32)obj);
}

/*
 * Effect pre-draw (D_8008ADBC[1]): binds segment 8 to D_80083740 while the effect's alpha is below 0xFF
 * (else D_800838C0), then texture 0xC3 to segments 6 and 7.
 */
Gfx *func_151D710C(Gfx *gfx, ModelEffect *fx, s16 view, u8 *needSync, u8 *proceed) {
    if (fx->params.color0[3] < 0xFF) {
        gSPSegment(gfx++, 8, D_80083740);
    } else {
        gSPSegment(gfx++, 8, D_800838C0);
    }

    gfx = func_15133EEC(func_15133EEC(gfx, 0xC3, 6, 3), 0xC3, 7, 3);
    *proceed = 1;
    return gfx;
}
