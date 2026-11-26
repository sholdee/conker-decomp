#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct { u8 ids[3]; } ParticleVariants;
extern ParticleVariants D_8008FBC0;

/* Spawns one of three particle variants at pos, turned rotY degrees about the vertical axis. */
void *func_151BEEE0(f32 rotY, Vec3f *pos, s32 arg2, u8 arg3, u8 variant, u8 arg5, s32 arg6, u8 arg7, s32 arg8) {
    ModelParticle *particle;
    ParticleVariants variants = D_8008FBC0;

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
        spawn.variant = variants.ids[variant];
        spawn.unk5C = arg6;
        spawn.alpha = 0xFF;
        if (level == 7) {
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
        spawn.unk6A = 0;
        spawn.owner = NULL;
        spawn.ownerId = 0;
        spawn.fadeStart = 1;
        spawn.fadeRate = 0xFF;
        particle = func_15132A4C(&spawn, arg2, arg3, 0, arg7, arg8);
        if (particle != NULL) {
            for (i = 0; i < 2; i++) {
                func_15133760(&particle->mtx[i], particle);
            }
        }
        return particle;
    }
}

/* Spawns two particle bursts at pos: one through func_15150F90, one through func_15153634. */
void func_151BF0C8(Vec3f *pos) {
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
    func_15150F90(&first, 1, 0xFF, 1);

    second.countBase = 20;
    second.countRange = 8;
    second.texture = 0x6C;
    second.renderMode = 0x5103;
    second.unk8 = 0x200005;
    second.unkC = 0;
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
    second.unk64 = 1;
    second.extra = 0.9411770105f;
    func_15153634(&second, 0xFF, 0xFF, 1);
}

/* Shows the value of func_150859AC(0, 3) (less one, capped at 99) as a two-digit counter that slides in. */
void func_151BF340(u8 owner, s32 kind) {
    CounterDisplay init;
    CounterEffect *effect;
    s32 count;
    s32 digits[2];
    CounterDisplay *display;

    func_151494E0(0, 0x57);
    count = func_150859AC(0, 3);
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
    effect = func_15149130(300, -1, 0x60, -1, 0, 0x49, sizeof(CounterDisplay), owner, kind);
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
            panelConfig.unkC = 8;
            panelConfig.render.unk0 = 0x100000;
            panelConfig.render.setGeometryMode = 0x200004;
            panelConfig.render.clearGeometryMode = 0x60601;
            panelConfig.render.envMode = 8;
            panelConfig.render.renderMode = 1;
            panelConfig.render.unk10 = 0x38;
            panelConfig.render.primMode = 0;
            panelConfig.render.unk14 = 0x80;
            panelConfig.render.unk18 = 0x20;
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
            panelConfig.unk41 = 1;
            panelConfig.fadeStart = 1;
            panelConfig.fadeRate = 0xFF;
            panelConfig.pos.x = 300.0f;
            if (D_800D2458) {
                panelConfig.pos.y = 32.0f;
            } else {
                panelConfig.pos.y = 79.0f;
            }
            panelConfig.pos.z = 0.0f;
            panelExtra.unk0 = D_800A5480;
            panelExtra.unkC = 0.3010000288f;
            panelExtra.unk10 = 0.3010000288f;
            display->panel = func_15157010(&panelConfig, 0, 1.0f, 0, 0, 0x14, owner, kind);
            if (display->panel != NULL) {
                memcpy(&display->panel->extra, &panelExtra, sizeof(panelExtra));
            }
            {
                // not const: the original unit (game_1EBD00..game_1EC390) keeps its aggregate initializers ahead of the literals (ROM 0x24F374)
                s32 digitSprites[10] = { 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD };
                u8 i;

                digitArg.x = -500.0f;
                if (D_800D2458) {
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
                digitArg.unk23 = 0xFF;
                digitArg.otherModeH = 0;
                digitArg.unk28 = 0x200004;
                digitArg.unk2C = 0x9F0601;
                digitArg.unk40 = 0;
                digitArg.unk41 = 0;
                digitArg.combineMode = 0x19;
                digitArg.unk34 = 0x13;
                digitArg.unk38 = 0x80;
                digitArg.unk3C = 0x20;
                digitArg.viewport = 0;
                digitArg.offsetS = 0.0f;
                digitArg.offsetT = 0.0f;
                digitArg.halfHeight = 16.0f;
                digitArg.halfWidth = 16.0f;
                digitArg.scaleX = 1.f; // matching: spelling it 1.0f swaps the FP registers of 0.0f and 1.0f
                digitArg.scaleY = 1.f;
                if (digits[0] == 0) {
                    digitArg.texture = digitSprites[digits[1]];
                    display->digits[0] = func_1515548C(&digitArg, 0, NULL, 0, 0, owner, kind);
                    display->digits[1] = NULL;
                } else {
                    for (i = 0; i < 2; i++) {
                        digitArg.texture = digitSprites[digits[i]];
                        display->digits[i] = func_1515548C(&digitArg, 0, NULL, 0, 0, owner, kind);
                    }
                }
            }
        }
    }
}

/* Update of func_151BF340's counter. It waits until D_800C35EA != 1, func_15181CC8(0) != 0, func_1517EF00(0) == 0
 * and D_800BE9B4 == 0; then, as its 110-unit timer runs down by D_800BE9A4, it slides in over the first 20.57
 * units, holds, slides out over the last 20.57 and ends the effect. */
void func_151BF81C(CounterEffect *effect) {
    f32 slide;
    CounterDisplay *display;

    display = &effect->display;
    if (D_800BEAC1 != 0 || D_800BEAC0 != 0) {
        effect->base.header.state = -1;
        effect->base.header.flags |= 1;
        return;
    }
    if (!(display->flags & 1)) {
        if (D_800C35EA[0] == 1) {
            return;
        }
        if (func_15181CC8(0) == 0) {
            return;
        }
        if (func_1517EF00(0) != 0 || D_800BE9B4 != 0) {
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
        if (D_800D2458 != 0) {
            display->panel->params.pos.y = 32.0f;
        } else {
            display->panel->params.pos.y = 79.0f;
        }
    }
    if (display->digits[0] != NULL) {
        // matching: the no-op / 1.0f is needed; without it IDO emits shorter code
        display->digits[0]->x = ((slide * -85.0f + 150.0f) / 1.0f + 48.0f) - 12.0f;
        if (D_800D2458 != 0) {
            display->digits[0]->y = -15.0f;
        } else {
            display->digits[0]->y = -62.0f;
        }
    }
    if (display->digits[1] != NULL) {
        display->digits[1]->x = ((slide * -85.0f + 150.0f) / 1.0f + 48.0f) + 12.0f;
        if (D_800D2458 != 0) {
            display->digits[1]->y = -15.0f;
        } else {
            display->digits[1]->y = -62.0f;
        }
    }
    display->timer -= D_800BE9A4;
    if (display->timer < 0.0f) {
        effect->base.header.state = -1;
        effect->base.header.flags |= 1;
    }
}

/* Frees the counter's panel and digits. */
void func_151BFB2C(CounterEffect *effect) {
    u8 i;
    CounterDisplay *display = &effect->display;

    if (display->panel != NULL) {
        func_1516972C(&display->panel->header);
    }
    for (i = 0; i < 2; i++) {
        if (display->digits[i] != NULL) {
            func_1516972C(&display->digits[i]->header);
        }
    }
}

/* Destructor of func_151BF340's counter effect. */
void func_151BFBA4(CounterEffect *effect) {
    func_151BFB2C(effect);
    func_1514933C(&effect->base);
}
