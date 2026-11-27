#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/hud.h"
#include "conker/particle.h"
#include "conker/player.h"
#include "conker/screen.h"
#include "conker/script.h"
#include "conker/sound.h"
#include "conker/sprite.h"

/* Extension block copied into the spawned effect at 0x120. */
typedef struct {
    /* 0x00 */ f32 angle;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 count;
} Extra151BE850;


/*
 * Spawns an effect (variant 0-2) at `pos`, facing `angle` degrees. In one mode (gTaskDoneBits[0x19] bit 2,
 * or variant 2) it is a model effect 39 units ahead of and 40 below `pos`; otherwise func_151BEEE0 makes it.
 * The callers also pass the usual effect owner and list, which it does not use.
 */
void *func_151BE850(Vec3f *pos, f32 angle, u8 count, u8 arg3, u8 variant, u8 owner, s32 list) {
    void *fx;
    ZoneNode *zone;
    EffectParams params;
    Extra151BE850 extra;

    collision_selectMap(0);
    zone = collision_findZoneLeaf(pos->x, pos->z);
    if (variant >= 3) {
        return NULL;
    }
    if ((gTaskDoneBits[0x19] & 4) || (variant == 2)) {
        const u8 variantIds[3] = { 3, 1, 2 };
        Vec3f offset;
        s32 flag;

        extra.angle = angle;
        params.owner = NULL;
        params.ownerId = 0;
        params.lightMode = 0;
        params.pos = *pos;
        extra.count = count;
        {
            s32 pad; // matching: stack slot

            math_dirFromAngles(angle * 0.01745329238f, 0.0f, 39.0f, &offset);
            extra.pos.x = pos->x + offset.x;
            extra.pos.y = (pos->y + offset.y) - 40.0f;
            extra.pos.z = pos->z + offset.z;
        }
        params.unk38 = variantIds[variant];
        // matching: the (u8) narrowing of a separate local steers IDO's temp-register order
        flag = arg3 ? 0x10 : 0;
        params.flags = (u8)flag | 0xE;
        params.updateFunc = 1;
        if (level == LEVEL_W1_DOG_FISH) {
            params.updateFunc2 = 6;
        } else {
            params.updateFunc2 = 2;
        }
        params.preDrawFunc = 0;
        params.postDrawFunc = -1;
        params.lifetime = 300;
        params.model = 0xA2;
        params.bank = 8;
        params.render.otherModeH = 0;
        params.render.setGeometryMode = 0x620405;
        params.render.clearGeometryMode = 0x40200;
        params.render.renderMode = 20;
        params.render.combiner = 0x37;
        params.render.colorDither = 0x80;
        params.render.alphaDither = 0x20;
        params.render.primMode = 0;
        params.render.envMode = 8;
        params.color0.red = 0xFF;
        params.color0.green = 0xFF;
        params.color0.blue = 0xFF;
        params.color0.alpha = 0xFF;
        params.color1.red = 0xFF;
        params.color1.green = 0xFF;
        params.color1.blue = 0xFF;
        params.color1.alpha = 0xFF;
        params.mask = zone;
        fx = effect_newModelEffect(&params, 0, 1.0f, 3, 0xFF, 0x18, 0xFF, 1);
        if (fx != NULL) {
            // copies the whole 0x18-byte extension (extraSize above); a 0x18-byte Extra151BE850 grows the frame
            memcpy((u8 *)fx + 0x120, &extra, 0x18);
        }
    } else {
        fx = func_151BEEE0(angle, pos, 3, 0xFF, variant, arg3, zone, 0xFF, 1);
    }
    return fx;
}

struct S151BEB20 {
    u8 pad0[0x54];
    Vec3f pos;
    u8 pad60[0x7C - 0x60];
    MtxF mtx[2]; /* double-buffered, indexed by gDrawBufferIndex */
    u8 padFC[0x120 - 0xFC];
    f32 rotY;
};

/* Rebuilds this frame's matrix: rotation by `rotY`, translation to `pos`, axes scaled by 0.923. */
s32 effect_buildStatPickupMatrix(struct S151BEB20 *fx) {
    f32 scale;

    math_mtxRotateRPYF(fx->mtx[gDrawBufferIndex], 0.0f, fx->rotY, 0.0f);

    scale = 0.9230000377f;
    fx->mtx[gDrawBufferIndex][3][0] = fx->pos.x;
    fx->mtx[gDrawBufferIndex][3][1] = fx->pos.y;
    fx->mtx[gDrawBufferIndex][3][2] = fx->pos.z;

    fx->mtx[gDrawBufferIndex][0][0] *= scale;
    fx->mtx[gDrawBufferIndex][0][1] *= scale;
    fx->mtx[gDrawBufferIndex][0][2] *= scale;
    fx->mtx[gDrawBufferIndex][1][0] *= scale;
    fx->mtx[gDrawBufferIndex][1][1] *= scale;
    fx->mtx[gDrawBufferIndex][1][2] *= scale;
    fx->mtx[gDrawBufferIndex][2][0] *= scale;
    fx->mtx[gDrawBufferIndex][2][1] *= scale;
    fx->mtx[gDrawBufferIndex][2][2] *= scale;

    return 1;
}

/* Pre-draw callback (gModelEffectPreDrawFuncs[0]): adds nothing and lets the model draw. */
Gfx *effect_preDrawModelEffectNone(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync, u8 *proceed) {
    *proceed = TRUE;
    return gfx;
}

typedef struct {
    /* 0x00 */ f32 angle;   /* func_151BEEE0's angle */
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 count;    /* the stat value given to the player (player_setStat stat 3) */
} S151BECB8Params;

struct S151BECB8 {
    /* 0x000 */ EffectHeader header;
    /* 0x010 */ u8 flags;       /* EffectParams.flags */
    /* 0x011 */ u8 pad11[0x37];
    /* 0x048 */ u8 unk48;
    /* 0x049 */ u8 pad49[0xB];
    /* 0x054 */ Vec3f pos;       /* EffectParams.pos */
    /* 0x060 */ ZoneNode *zone; /* EffectParams.mask: collision_findZoneLeaf at the spawn position */
    /* 0x064 */ u8 pad64[0x98];
    /* 0x0FC */ s32 unkFC;
    /* 0x100 */ u8 pad100[0x1B];
    /* 0x11B */ u8 unk11B;
    /* 0x11C */ u8 pad11C[4];
    /* 0x120 */ S151BECB8Params params;
};


/*
 * Proximity check: for every active object whose bounding radius comes within 43 units of
 * params.pos, plays sound 0x511 and runs effect_showStatCounter/effect_spawnStatPickupBurst/func_151BEEE0.
 * Returns 0 if any object was in range, else 1.
 */
s32 effect_updateStatPickup(S151BECB8 *fx) {
    s32 untouched = 1;

    {
        Actor *obj;
        S151BECB8Params *params;
        f32 range = 43.0f; // matching: a local keeps 43.0f in $f20 across the loop
        f32 radius;
        f32 height;
        Vec3f objPos;
        Vec3f diff;

        obj = movement_obj;
        do {
            if (effect_isPlayerActor(obj) != 0) {
                f32 dist;

                params = &fx->params;
                sprite_getActorBounds(obj, &objPos, &radius, &height);
                diff.x = objPos.x - params->pos.x;
                diff.y = objPos.y - params->pos.y;
                diff.z = objPos.z - params->pos.z;
                dist = math_vec3Length(&diff);
                if (dist - radius < range) {
                    s32 mode;
                    s32 kind; // matching: switching on fx->unk48 directly changes the frame size

                    player_setStat(obj->playernumber, 3, params->count);
                    untouched = 0;
                    sound_play(0x511, 0x7D00, 0x40, 0, 0);
                    effect_showStatCounter(fx->header.owner, fx->header.kind);
                    effect_spawnStatPickupBurst(&params->pos);
                    kind = fx->unk48;
                    switch (kind) {
                        case 1:
                            mode = 1;
                            break;
                        case 2:
                            mode = 2;
                            break;
                        case 0:
                        case 3:
                        default:
                            mode = 0;
                            break;
                    }
                    func_151BEEE0(params->angle, &fx->pos, fx->unkFC, fx->unk11B, mode, fx->flags & 0x10,
                                  fx->zone, fx->header.owner, fx->header.kind);
                }
            }
            obj++;
        } while (obj != (Actor *)&gActorFrameCount); // matching: the end of movement_obj[26] by its own symbol; &movement_obj[26] changes the code
    }
    return untouched;
}

/* An object counts as active when it has a player slot, an interaction state and a valid id. */
s32 effect_isPlayerActor(Actor *obj) {
    if (obj->playernumber == 0xFF) {
        return 0;
    }
    if (obj->type == 0) {
        return 0;
    }
    if (obj->character_type == 0xFF) {
        return 0;
    }
    return 1;
}


/* Spawns one of three particle variants at pos, turned rotY degrees about the vertical axis. */
void *func_151BEEE0(f32 rotY, Vec3f *pos, s32 arg2, u8 arg3, u8 variant, u8 arg5, ZoneNode *zone, u8 owner, s32 list) {
    ModelParticle *particle;
    u8 variantIds[3] = { 4, 2, 3 };

    if (variant >= 3) {
        return NULL;
    }
    {
        ParticleSpawn spawn;
        s32 i;

        spawn.radius = 1.0f;
        spawn.elasticity = 1.0f;
        spawn.sizeY = 0.9230000377f;
        spawn.sizeXZ = 0.9230000377f;
        spawn.rot.x = 0.0f;
        spawn.rot.y = rotY;
        spawn.rot.z = 0.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.pos = *pos;
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
        spawn.rotVel.x = 0.0f;
        spawn.rotVel.y = 0.0f;
        spawn.rotVel.z = 0.0f;
        spawn.gravity = 0.0f;
        spawn.flags = (arg5 ? 0x4000 : 0) | 0x1D00 | 0x80000 | 0x40000;
        spawn.lifetime = 300;
        spawn.model = 0x26;
        spawn.variant = variantIds[variant];
        spawn.zone = zone;
        spawn.alpha = 0xFF;
        if (level == LEVEL_W1_DOG_FISH) {
            spawn.updateFn = 0x18;
        } else {
            spawn.updateFn = 0;
        }
        spawn.update2Fn = 0;
        spawn.floorHitFn = 0;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 2;
        spawn.lightMode = 0;
        spawn.owner = NULL;
        spawn.ownerId = 0;
        spawn.fadeStart = 1;
        spawn.fadeRate = 0xFF;
        particle = particle_newModelParticle(&spawn, arg2, arg3, 0, owner, list);
        if (particle != NULL) {
            for (i = 0; i < 2; i++) {
                particle_buildUniformMatrix(&particle->mtx[i], particle);
            }
        }
        return particle;
    }
}

/* Spawns two particle bursts at pos: one through particle_spawnPieceSpray, one through particle_spawnSpriteBurst. */
void effect_spawnStatPickupBurst(Vec3f *pos) {
    PieceSpray first;
    SpriteBurst second;

    first.countBase = 24;
    first.countRange = 10;
    first.pos = *pos;
    first.speedBase = 0.06260000169f;
    first.speedRange = 0.05810000002f;
    first.angles.yawBase = 0;
    first.angles.yawRange = 255;
    first.angles.pitchBase = -50;
    first.angles.pitchRange = 60;
    first.directionBase = 13.0f;
    first.directionRange = 13.0f;
    first.jitter = 10.3760004f;
    first.sizeBase = -1.048000097f;
    first.sizeRange = 0.9950000644f;
    first.scale = 0.9424110055f;
    first.kind = 3;
    first.lifeBase = 65;
    first.lifeRange = 30;
    particle_spawnPieceSpray(&first, 1, 0xFF, 1);

    second.countBase = 20;
    second.countRange = 8;
    second.texture = 0x6C;
    second.renderMode = 0x5103;
    second.setGeometryMode = 0x200005;
    second.clearGeometryMode = 0;
    second.timerBase = 40;
    second.timerRange = 40;
    second.frameRateBase = 0;
    second.frameRateRange = 0;
    second.a = 0xFF;
    second.r = 0xFF;
    second.g = 0x91;
    second.b = 6;
    second.r2 = 0xFF;
    second.g2 = 0xFF;
    second.b2 = 0;
    second.alphaBase = 20;
    second.alphaRange = 200;
    second.brightness = 0xFF;
    second.fadeStart = 50;
    second.fadeRate = 5;
    second.scaleStart = 50;
    second.scaleRate = 1.017730951f;
    second.sizeBase = 175.0f;
    second.sizeRange = 160.0f;
    second.pos = *pos;
    second.yawBase = 0;
    second.pitchBase = -51;
    second.yawRange = 255;
    second.pitchRange = 80;
    second.speedBase = 4.0f;
    second.speedRange = 13.0f;
    second.gravityBase = -0.007000000216f;
    second.gravityRange = -0.006000000052f;
    second.flags = 0x840E07;
    second.updateFunc = 16;
    second.deathFunc = -1;
    second.primColorMode = 8;
    second.envColorMode = 6;
    second.kind = 1;
    second.extra = 0.9411770105f;
    particle_spawnSpriteBurst(&second, 0xFF, 0xFF, 1);
}

/* Shows the value of player_getStat(0, 3) (less one, capped at 99) as a two-digit counter that slides in. */
void effect_showStatCounter(u8 owner, s32 kind) {
    CounterDisplay init;
    CounterEffect *effect;
    s32 count;
    s32 digits[2];
    CounterDisplay *display;

    effect_sendEventToTimedEffects(0, 0x57);
    count = player_getStat(0, 3);
    if (count != 0) {
        count--;
    }
    if (count >= 100) {
        count = 99;
    }
    digits[0] = count / 10;
    digits[1] = count % 10;
    init.panel = NULL;
    bzero(init.digits, sizeof(init.digits));
    init.timer = 110.0f;
    init.plateau = sinf(1.905000091f);
    init.flags = 0;
    effect = effect_newTimedEffect(300, -1, 0x60, -1, 0, 0x49, sizeof(CounterDisplay), owner, kind);
    if (effect != NULL) {
        display = &effect->display;
        memcpy(display, &init, sizeof(init));
        {
            EffectParams panelConfig;
            PanelExtra panelExtra;
            s32 pad; // matching: unused stack slot
            ScreenSpriteInfo digitArg;

            panelConfig.unk38 = 0;
            panelConfig.flags = 0xC0;
            panelConfig.updateFunc = -1;
            panelConfig.updateFunc2 = 5;
            panelConfig.preDrawFunc = 3;
            panelConfig.postDrawFunc = 0;
            panelConfig.lifetime = 300;
            panelConfig.model = 0xA3;
            panelConfig.bank = 8;
            panelConfig.render.otherModeH = 0x100000;
            panelConfig.render.setGeometryMode = 0x200004;
            panelConfig.render.clearGeometryMode = 0x60601;
            panelConfig.render.envMode = 8;
            panelConfig.render.renderMode = 1;
            panelConfig.render.combiner = 0x38;
            panelConfig.render.primMode = 0;
            panelConfig.render.colorDither = 0x80;
            panelConfig.render.alphaDither = 0x20;
            panelConfig.color0.red = 0xFF;
            panelConfig.color0.green = 0xFF;
            panelConfig.color0.blue = 0xFF;
            panelConfig.color0.alpha = 0xFF;
            panelConfig.color1.red = 0xFF;
            panelConfig.color1.green = 0xFF;
            panelConfig.color1.blue = 0xFF;
            panelConfig.color1.alpha = 0xFF;
            panelConfig.owner = 0;
            panelConfig.ownerId = 0;
            panelConfig.lightMode = 1;
            panelConfig.fadeStart = 1;
            panelConfig.fadeRate = 0xFF;
            panelConfig.pos.x = 300.0f;
            if (gHudClockShown) {
                panelConfig.pos.y = 32.0f;
            } else {
                panelConfig.pos.y = 79.0f;
            }
            panelConfig.pos.z = 0.0f;
            panelExtra.rot = gZeroVec3f;
            panelExtra.scaleXZ = 0.3010000288f;
            panelExtra.scaleY = 0.3010000288f;
            display->panel = effect_newModelEffect(&panelConfig, 0, 1.0f, 0, 0, 0x14, owner, kind);
            if (display->panel != NULL) {
                memcpy(&display->panel->extra, &panelExtra, sizeof(panelExtra));
            }
            {
                const s32 digitSprites[10] = { 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD };
                u8 i;

                digitArg.x = -500.0f;
                if (gHudClockShown) {
                    digitArg.y = -15.0f;
                } else {
                    digitArg.y = -62.0f;
                }
                digitArg.timer = 300;
                digitArg.flags = 0x20;
                digitArg.fadeStart = 1;
                digitArg.fadeRate = 0xFF;
                digitArg.kind = 0;
                digitArg.color1.red = 0xFF;
                digitArg.color1.green = 0xFF;
                digitArg.color1.blue = 0xFF;
                digitArg.color1.alpha = 0xFF;
                digitArg.color2.red = 0xFF;
                digitArg.color2.green = 0xFF;
                digitArg.color2.blue = 0xFF;
                digitArg.color2.alpha = 0xFF;
                digitArg.colorLevel = 0xFF;
                digitArg.render.otherModeH = 0;
                digitArg.render.setGeometryMode = 0x200004;
                digitArg.render.clearGeometryMode = 0x9F0601;
                digitArg.render.primMode = 0;
                digitArg.render.envMode = 0;
                digitArg.render.renderMode = 0x19;
                digitArg.render.combiner = 0x13;
                digitArg.render.colorDither = 0x80;
                digitArg.render.alphaDither = 0x20;
                digitArg.viewport = 0;
                digitArg.offsetS = 0.0f;
                digitArg.offsetT = 0.0f;
                digitArg.halfHeight = 16.0f;
                digitArg.halfWidth = 16.0f;
                digitArg.scaleX = 1.f; // matching: spelling it 1.0f swaps the FP registers of 0.0f and 1.0f
                digitArg.scaleY = 1.f;
                if (digits[0] == 0) {
                    digitArg.texture = digitSprites[digits[1]];
                    display->digits[0] = sprite_newScreenSprite(&digitArg, 0, NULL, 0, 0, owner, kind);
                    display->digits[1] = NULL;
                } else {
                    for (i = 0; i < 2; i++) {
                        digitArg.texture = digitSprites[digits[i]];
                        display->digits[i] = sprite_newScreenSprite(&digitArg, 0, NULL, 0, 0, owner, kind);
                    }
                }
            }
        }
    }
}

/* Update of effect_showStatCounter's counter. It waits until gCutsceneStates != 1, screen_isIrisOpen(0) != 0, screen_getViewFadeAlpha(0) == 0
 * and gLevelLoadState == 0; then, as its 110-unit timer runs down by gFrameDelta, it slides in over the first 20.57
 * units, holds, slides out over the last 20.57 and ends the effect. */
void effect_updateStatCounter(CounterEffect *effect) {
    f32 slide;
    CounterDisplay *display;

    display = &effect->display;
    if (gPauseRequested != 0 || gPaused != 0) {
        effect->base.header.state = -1;
        effect->base.header.flags |= 1;
        return;
    }
    if (!(display->flags & 1)) {
        if (gCutsceneStates[0] == 1) {
            return;
        }
        if (screen_isIrisOpen(0) == 0) {
            return;
        }
        if (screen_getViewFadeAlpha(0) != 0 || gLevelLoadState != 0) {
            return;
        }
        display->flags |= 1;
    }
    if (display->timer < 20.5700016f) {
        slide = sinf(display->timer * 0.04861448333f * 1.905000091f);
    } else if (display->timer > 89.42999268f) {
        slide = sinf((110.0f - display->timer) * 0.04861448333f * 1.905000091f);
    } else {
        slide = display->plateau;
    }
    if (display->panel != NULL) {
        display->panel->params.pos.x = slide * -85.0f + 150.0f;
        if (gHudClockShown != 0) {
            display->panel->params.pos.y = 32.0f;
        } else {
            display->panel->params.pos.y = 79.0f;
        }
    }
    if (display->digits[0] != NULL) {
        // matching: the no-op / 1.0f is needed; without it IDO emits shorter code
        display->digits[0]->x = ((slide * -85.0f + 150.0f) / 1.0f + 48.0f) - 12.0f;
        if (gHudClockShown != 0) {
            display->digits[0]->y = -15.0f;
        } else {
            display->digits[0]->y = -62.0f;
        }
    }
    if (display->digits[1] != NULL) {
        display->digits[1]->x = ((slide * -85.0f + 150.0f) / 1.0f + 48.0f) + 12.0f;
        if (gHudClockShown != 0) {
            display->digits[1]->y = -15.0f;
        } else {
            display->digits[1]->y = -62.0f;
        }
    }
    display->timer -= gFrameDelta;
    if (display->timer < 0.0f) {
        effect->base.header.state = -1;
        effect->base.header.flags |= 1;
    }
}

/* Frees the counter's panel and digits. */
void effect_freeStatCounterParts(CounterEffect *effect) {
    u8 i;
    CounterDisplay *display = &effect->display;

    if (display->panel != NULL) {
        effect_destroyObject(&display->panel->header);
    }
    for (i = 0; i < 2; i++) {
        if (display->digits[i] != NULL) {
            effect_destroyObject(&display->digits[i]->header);
        }
    }
}

/* Destructor of effect_showStatCounter's counter effect. */
void effect_destroyStatCounter(CounterEffect *effect) {
    effect_freeStatCounterParts(effect);
    effect_defaultDestroyTimedEffect(&effect->base);
}
