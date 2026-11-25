#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

extern void func_150D6730(Actor *obj, u8 arg1, s32 arg2);
extern s32 func_15042E3C(char *format, ...);
extern Gfx *func_15133EEC(Gfx *gfx, u16 index, u8 segment, s32 arg3);
extern u32 D_80083740[];
extern u32 D_800838C0[];

/* Text box data (same layout as TextLines in game_FC5F0.c). */
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
} TextLines;

/* Text box object created by func_150CFF10 (same layout as TextObj in game_FC5F0.c). */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ TextLines text;
} TextObj;

/* Style 3 text box state: an amount shown as "$ %d" that fades out with its owner. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; // owner->unique_id when the box was made
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ s32 amount;
    /* 0x0C */ s16 fadeStart; // alpha fades once the lifetime drops below this
    /* 0x0E */ s16 fadeRate;
} AmountTextStyle;

/* Called when `character` touches a pulse object (game_1483E0.c): spawns func_150D6730's particles on it
   while D_800BE9F0 is 0x32 or 0x33. */
void func_151D6970(Actor *character, void *obj) {
    if ((D_800BE9F0 == 0x32) || (D_800BE9F0 == 0x33)) {
        func_150D6730(character, 0xFF, 1);
    }
}

extern void func_1504715C(void *ground, Actor *actor);
extern void func_15153F18(AngleRange *angles, SplashSpray *spray, void *ground, s32 arg3, s32 arg4);
extern void func_15150178(AngleRange *angles, DropSpray *spray, void *ground, s32 arg3, s32 arg4);

/* Spawns the func_15153F18 and func_15150178 bursts at `pickup`, on the ground under `player`. */
void func_151D69B4(WorldObject *pickup, Actor *player) {
    u8 ground[0x24]; // ground probe result (Ground in game_1228D0.c)
    SplashSpraySpec first;

    func_1504715C(ground, player);
    first.spray.pos.x = pickup->x;
    first.spray.pos.y = pickup->y;
    first.spray.pos.z = pickup->z;
    first.spray.scaleBase = 2.49000001f;
    first.spray.countBase = 3;
    first.spray.countRange = 3;
    first.angles.yawRange = 0xFF;
    first.angles.pitchBase = -0x2B;
    first.angles.yawBase = 0;
    first.angles.pitchRange = 0x20;
    first.spray.itemCountBase = 3;
    first.spray.itemCountRange = 2;
    first.spray.lifeBase = 0x28;
    first.spray.lifeRange = 0x14;
    first.spray.unk30 = 0x9B;
    first.spray.unk32 = 0x64;
    first.spray.unk3C = 0x10;
    first.spray.unk3E = 0xF;
    first.spray.unk40 = 0;
    first.spray.kind = 9;
    first.spray.scaleRange = 2.029999971f;
    first.spray.sizeBase = 0.6080000401f;
    first.spray.sizeRange = 0.2569999993f;
    first.spray.speedBase = 17.0f;
    first.spray.speedRange = 3.5f;
    first.spray.unk38 = 0.400000006f;
    func_15153F18(&first.angles, &first.spray, ground, 0xFF, 1);
    {
        DropSpraySpec second;

        second.spray.pos.x = pickup->x;
        second.spray.pos.y = pickup->y;
        second.spray.pos.z = pickup->z;
        second.spray.countBase = 0xF;
        second.spray.countRange = 6;
        second.angles.yawBase = 0;
        second.angles.yawRange = 0xFF;
        second.angles.pitchBase = -0x40;
        second.angles.pitchRange = 0xC;
        second.spray.lifeBase = 0x3C;
        second.spray.lifeRange = 0x2D;
        second.spray.unk24 = 0xC8;
        second.spray.unk25 = 0x37;
        second.spray.unk30 = 1;
        second.spray.kind = 9;
        second.spray.unk34 = 1.0f;
        second.spray.wobbles = 0;
        second.spray.bounceChance = 1.0f;
        second.spray.speedBase = 10.0f;
        second.spray.speedRange = 6.0f;
        second.spray.gravityBase = -1.506000042f;
        second.spray.gravityRange = 0.8140000105f;
        second.spray.sizeBase = 44.0f;
        second.spray.sizeRange = 88.0f;
        func_15150178(&second.angles, &second.spray, ground, 0xFF, 1);
    }
}

/* Model effect object (Effect in game_1844C0.c; only the fields used here). */
typedef struct {
    /* 0x00 */ u8 header[0x10];
    /* 0x10 */ EffectParams params;
    /* 0x68 */ u8 pad68[0x7C - 0x68];
    /* 0x7C */ MtxF mtx[2]; // per frame
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
void func_151D6BFC(Actor *owner, u8 arg1, s32 arg2) {
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
    params.render.unk0 = 0;
    params.render.setGeometryMode = 0x220405;
    params.render.clearGeometryMode = 0x40200;
    params.render.envMode = 8;
    params.render.renderMode = 1;
    params.render.unk10 = 0x38;
    params.render.primMode = 0;
    params.render.unk14 = 0x80;
    params.render.unk18 = 0x20;
    params.color0.red = 0xFF;
    params.color0.green = 0xFF;
    params.color0.blue = 0xFF;
    params.color0.alpha = 0xFF;
    params.color1.red = 0xFF;
    params.color1.green = 0xFF;
    params.color1.blue = 0xFF;
    params.color1.alpha = 0xFF;
    params.owner = owner;
    params.ownerId = owner->unique_id;
    params.unk41 = 1;
    params.pos = D_800A5480;
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
    Actor *owner;
    AmountTextStyle *style;
    s16 timeLeft;
    s16 pad; // matching: unused, but alpha's register allocation depends on it
    s16 alpha;

    style = obj->text.style;
    owner = style->owner;
    if ((owner->interaction_state == 0) || (owner->unique_id != style->ownerId)) {
        return 0;
    }

    timeLeft = obj->header.state;
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

    func_15169850((s32)payload, type, &style->owner, &style->ownerId, obj);
}

/*
 * Effect pre-draw (D_8008ADBC[1]): binds segment 8 to D_80083740 while the effect's alpha is below 0xFF
 * (else D_800838C0), then texture 0xC3 to segments 6 and 7.
 */
Gfx *func_151D710C(Gfx *gfx, ModelEffect *fx, s16 view, u8 *needSync, u8 *proceed) {
    if (fx->params.color0.alpha < 0xFF) {
        gSPSegment(gfx++, 8, D_80083740);
    } else {
        gSPSegment(gfx++, 8, D_800838C0);
    }

    gfx = func_15133EEC(func_15133EEC(gfx, 0xC3, 6, 3), 0xC3, 7, 3);
    *proceed = 1;
    return gfx;
}
