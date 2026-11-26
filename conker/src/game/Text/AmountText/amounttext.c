#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Style 3 text box state: an amount shown as "$ %d" that fades out with its owner. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; // owner->unique_id when the box was made
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ s32 amount;
    /* 0x0C */ s16 fadeStart; // alpha fades once the lifetime drops below this
    /* 0x0E */ s16 fadeRate;
} AmountTextStyle;

/* Called when `character` touches a pulse object (game/Prop/PropUpdates/propupdates.c): spawns particle_spawnFollowDrift's particles on it
   while level is 0x32 or 0x33. */
void func_151D6970(Actor *character, void *obj) {
    if ((level == 0x32) || (level == 0x33)) {
        particle_spawnFollowDrift(character, 0xFF, 1);
    }
}


/* Spawns the particle_spawnSplashSprayAtAngles and particle_spawnDropSprayAtAngles bursts at `pickup`, on the ground under `player`. */
void func_151D69B4(WorldObject *pickup, Actor *player) {
    CollisionHit ground;
    SplashSpraySpec first;

    collision_initHitFromObject(&ground, player);
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
    particle_spawnSplashSprayAtAngles(&first.angles, &first.spray, &ground, 0xFF, 1);
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
        particle_spawnDropSprayAtAngles(&second.angles, &second.spray, &ground, 0xFF, 1);
    }
}


/*
 * Spawns model effect 0xA5 on `owner` (updated by func_151D6E60, drawn with func_151D710C) and a text box
 * showing a random amount as "$ %d" (style 3: text_updateAmount / text_handleAmountMessage).
 */
void text_spawnAmount(Actor *owner, u8 arg1, s32 arg2) {
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
    params.pos = gZeroVec3f;
    params.fadeStart = 8;
    params.fadeRate = 0x1F;
    effect_newModelEffect(&params, 0, 1.0f, 0, 0, 0, arg1, arg2);
    {
        AmountTextStyle style;
        u8 str[4] = "500";
        s32 scale;
        TextObj *textObj;

        style.owner = owner;
        style.ownerId = owner->unique_id;
        style.fadeStart = 8;
        style.fadeRate = 0x1F;
        style.amount = player_getStat(0, 6);
        entry = script_getTaskBss(0x83);
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
        textObj = effect_newTextBox(0x63, str, 150, sizeof(AmountTextStyle), 3, 1, arg1, arg2);
        if (textObj != NULL) {
            memcpy(textObj->text.style, &style, sizeof(AmountTextStyle));
        }
    }
}

/*
 * Effect update (gModelEffectUpdate2Funcs[3]): this frame's matrix is func_150A8050's for (0, math_getCameraYaw(0) + 25, 0),
 * placed 120 units above the owner and scaled by 0.551.
 */
s32 func_151D6E60(ModelEffect *fx) {
    f32 scale;

    func_150A8050(fx->mtx[D_800BE9C0], 0.0f, math_getCameraYaw(0) + 25.0f, 0.0f);

    scale = 0.5509999990f;
    fx->mtx[D_800BE9C0][3][0] = fx->params.owner->xpos;
    fx->mtx[D_800BE9C0][3][1] = fx->params.owner->ypos + 120.0f;
    fx->mtx[D_800BE9C0][3][2] = fx->params.owner->zpos;

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
 * Text box update for style 3 (gTextBoxUpdateFuncs[3]): removes the box (returns 0) once its owner is gone or
 * replaced, otherwise prints the amount, fading out over the last fadeStart frames.
 */
s32 text_updateAmount(TextObj *obj) {
    Actor *owner;
    AmountTextStyle *style;
    s16 timeLeft;
    s16 pad; // matching: unused, but alpha's register allocation depends on it
    s16 alpha;

    style = obj->text.style;
    owner = style->owner;
    if ((owner->type == 0) || (owner->unique_id != style->ownerId)) {
        return 0;
    }

    timeLeft = obj->base.header.state;
    if (timeLeft < style->fadeStart) {
        alpha = timeLeft * style->fadeRate;
        if (alpha < 0) {
            alpha = 0;
        }
    } else {
        alpha = 0xFF;
    }

    text_setAnchor(owner, 0x109);
    text_setColor(0x32, 0x7D, 0x1C, alpha);
    text_setFlags(0x81);
    text_print("$ %d", style->amount);
    // matching: no return value here, so the caller gets what the void text_print left in v0
}

/* Text box message handler for style 3 (gTextBoxMessageFuncs[1]): keeps the owner reference up to date. */
void text_handleAmountMessage(TextObj *obj, u8 *payload, u8 type) {
    AmountTextStyle *style = obj->text.style;

    effect_handleOwnerActorEvent((s32)payload, type, &style->owner, &style->ownerId, obj);
}

/*
 * Effect pre-draw (gModelEffectPreDrawFuncs[1]): binds segment 8 to D_80083740 while the effect's alpha is below 0xFF
 * (else D_800838C0), then texture 0xC3 to segments 6 and 7.
 */
Gfx *func_151D710C(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync, u8 *proceed) {
    if (fx->params.color0.alpha < 0xFF) {
        gSPSegment(gfx++, 8, D_80083740);
    } else {
        gSPSegment(gfx++, 8, D_800838C0);
    }

    gfx = particle_bindSegmentTexture(particle_bindSegmentTexture(gfx, 0xC3, 6, 3), 0xC3, 7, 3);
    *proceed = 1;
    return gfx;
}
