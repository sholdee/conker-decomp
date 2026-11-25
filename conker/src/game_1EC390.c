#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* The spawned particle; only its two matrices (Particle.mtx in game_15F680.c) are touched here. */
typedef struct {
    /* 0x00 */ u8  pad0[0x90];
    /* 0x90 */ Mtx mtx[2];
} SpawnedParticle;

typedef struct { u8 ids[3]; } ParticleVariants;
extern ParticleVariants D_8008FBC0;
extern SpawnedParticle *func_15132A4C(ParticleSpawn *spawn, s32 arg1, s32 arg2, s32 extraSize, u8 arg4, s32 arg5);
extern void func_15133760(Mtx *mtx, SpawnedParticle *particle);

/* Spawns one of three particle variants at pos, turned rotY degrees about the vertical axis. */
void *func_151BEEE0(f32 rotY, Vec3f *pos, s32 arg2, u8 arg3, u8 variant, u8 arg5, s32 arg6, u8 arg7, s32 arg8) {
    SpawnedParticle *particle;
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
        if (D_800BE9F0 == 7) {
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

/* Burst config for func_15150F90 (same layout as in game_17CAF0.c). */
typedef struct {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f position;
    /* 0x14 */ f32 speedBase;
    /* 0x18 */ f32 speedRange;
    /* 0x1C */ s16 yawBase;
    /* 0x1E */ s16 yawRange;
    /* 0x20 */ s16 pitchBase;
    /* 0x22 */ s16 pitchRange;
    /* 0x24 */ f32 directionBase;
    /* 0x28 */ f32 directionRange;
    /* 0x2C */ f32 jitter;
    /* 0x30 */ f32 sizeBase;
    /* 0x34 */ f32 sizeRange;
    /* 0x38 */ f32 scale;
    /* 0x3C */ u8  kind;
    /* 0x3D */ u8  pad3D;
    /* 0x3E */ s16 lifeBase;
    /* 0x40 */ s16 lifeRange;
} Game150F90Config;

/* Burst config for func_15153634 (struct Local15153634Arg in game_17CAF0.c). */
typedef struct {
    /* 0x00 */ s16 countBase;
    /* 0x02 */ s16 countRange;
    /* 0x04 */ u8  unk4;
    /* 0x05 */ u8  pad5;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ u8  unk24;
    /* 0x25 */ u8  unk25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ Vec3f position;
    /* 0x44 */ s16 yawBase;
    /* 0x46 */ s16 pitchBase;
    /* 0x48 */ s16 yawRange;
    /* 0x4A */ s16 pitchRange;
    /* 0x4C */ f32 speedBase;
    /* 0x50 */ f32 speedRange;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8  unk60;
    /* 0x61 */ s8  unk61;
    /* 0x62 */ u8  unk62;
    /* 0x63 */ u8  unk63;
    /* 0x64 */ u8  unk64;
    /* 0x65 */ u8  pad65[3];
    /* 0x68 */ f32 extra;
} Config15153634;

void func_15150F90(Game150F90Config *config, u8 arg1, u8 arg2, s32 arg3);
void func_15153634(Config15153634 *config, u8 arg1, u8 arg2, s32 arg3);

/* Spawns two particle bursts at pos: one through func_15150F90, one through func_15153634. */
void func_151BF0C8(Vec3f *pos) {
    Game150F90Config first;
    Config15153634 second;

    first.countBase = 24;
    first.countRange = 10;
    first.position = *pos;
    first.speedBase = 0.06260000169f;
    first.speedRange = 0.05810000002f;
    first.yawBase = 0;
    first.yawRange = 255;
    first.pitchBase = -50;
    first.pitchRange = 60;
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
    second.unk4 = 0x6C;
    second.unk6 = 0x5103;
    second.unk8 = 0x200005;
    second.unkC = 0;
    second.unk10 = 40;
    second.unk12 = 40;
    second.unk14 = 0;
    second.unk18 = 0;
    second.unk1F = 0xFF;
    second.unk1C = 0xFF;
    second.unk1D = 0x91;
    second.unk1E = 6;
    second.unk20 = 0xFF;
    second.unk21 = 0xFF;
    second.unk22 = 0;
    second.unk23 = 20;
    second.unk24 = 200;
    second.unk25 = 0xFF;
    second.unk26 = 50;
    second.unk28 = 5;
    second.unk2A = 50;
    second.unk2C = 1.017730951f;
    second.unk30 = 175.0f;
    second.unk34 = 160.0f;
    second.position = *pos;
    second.yawBase = 0;
    second.pitchBase = -51;
    second.yawRange = 255;
    second.pitchRange = 80;
    second.speedBase = 4.0f;
    second.speedRange = 13.0f;
    second.unk54 = -0.007000000216f;
    second.unk58 = -0.006000000052f;
    second.unk5C = 0x840E07;
    second.unk60 = 16;
    second.unk61 = -1;
    second.unk62 = 8;
    second.unk63 = 6;
    second.unk64 = 1;
    second.extra = 0.9411770105f;
    func_15153634(&second, 0xFF, 0xFF, 1);
}

/* First argument of func_15157010. */
typedef struct {
    /* 0x00 */ u8  unk00;
    /* 0x01 */ s8  unk01;
    /* 0x02 */ u8  unk02;
    /* 0x03 */ s8  unk03;
    /* 0x04 */ s8  unk04;
    /* 0x05 */ u8  pad05;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8  unk2C;
    /* 0x2D */ u8  unk2D;
    /* 0x2E */ u8  pad2E[2];
    /* 0x30 */ u8  unk30;
    /* 0x31 */ u8  unk31;
    /* 0x32 */ u8  unk32;
    /* 0x33 */ u8  unk33;
    /* 0x34 */ u8  unk34;
    /* 0x35 */ u8  unk35;
    /* 0x36 */ u8  unk36;
    /* 0x37 */ u8  unk37;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ u8  pad39[3];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ Vec3f position;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} Config15157010;

/* The block copied to the panel's +0x120. */
typedef struct {
    /* 0x00 */ Vec3f unk0;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
} PanelExtra;

/* The object func_15157010 returns: the counter's background panel. */
typedef struct {
    /* 0x000 */ u8  pad0[0x54];
    /* 0x054 */ f32 x;
    /* 0x058 */ f32 y;
    /* 0x05C */ u8  pad5C[0x120 - 0x5C];
    /* 0x120 */ PanelExtra extra;
} CounterPanel;

/* First argument of func_1515548C (Struct1515548CArg in game_113D60.c). */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 sizeX;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ u8  unk10; /* sprite id */
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u16 flags;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
} Struct1515548CArg;

/* The object func_1515548C returns: one digit of the counter. */
typedef struct {
    /* 0x00 */ u8  pad0[0x10];
    /* 0x10 */ f32 x;
    /* 0x14 */ f32 y;
} CounterDigit;

/* Effect data of the two-digit counter that slides onto the screen (the 0x18 bytes at effect+0x28). */
typedef struct {
    /* 0x00 */ CounterPanel *panel;
    /* 0x04 */ CounterDigit *digits[2]; /* digits[1] is NULL for a one-digit count */
    /* 0x0C */ f32 timer;               /* counts down from 110 */
    /* 0x10 */ f32 plateau;             /* the slide curve's value while fully shown */
    /* 0x14 */ u8  flags;               /* bit 0: the slide has started */
} CounterDisplay;

typedef struct {
    /* 0x00 */ struct260 header;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ CounterDisplay display;
} CounterEffect;

extern CounterPanel *func_15157010(Config15157010 *config, s32 arg1, f32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 owner,
                                   s32 kind);
extern CounterDigit *func_1515548C(Struct1515548CArg *arg, s32 arg1, s32 *ids, s32 idCount, s32 arg4, u8 owner,
                                   s32 kind);

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
            Config15157010 panelConfig;
            PanelExtra panelExtra;
            Struct1515548CArg digitArg;

            panelConfig.unk38 = 0;
            panelConfig.unk00 = 0xC0;
            panelConfig.unk01 = -1;
            panelConfig.unk02 = 5;
            panelConfig.unk03 = 3;
            panelConfig.unk04 = 0;
            panelConfig.unk06 = 300;
            panelConfig.unk08 = 0xA3;
            panelConfig.unk0C = 8;
            panelConfig.unk10 = 0x100000;
            panelConfig.unk14 = 0x200004;
            panelConfig.unk18 = 0x60601;
            panelConfig.unk2D = 8;
            panelConfig.unk1C = 1;
            panelConfig.unk20 = 0x38;
            panelConfig.unk2C = 0;
            panelConfig.unk24 = 0x80;
            panelConfig.unk28 = 0x20;
            panelConfig.unk30 = 0xFF;
            panelConfig.unk31 = 0xFF;
            panelConfig.unk32 = 0xFF;
            panelConfig.unk33 = 0xFF;
            panelConfig.unk34 = 0xFF;
            panelConfig.unk35 = 0xFF;
            panelConfig.unk36 = 0xFF;
            panelConfig.unk37 = 0xFF;
            panelConfig.unk3C = 0;
            panelConfig.unk40 = 0;
            panelConfig.unk41 = 1;
            panelConfig.unk54 = 1;
            panelConfig.unk56 = 0xFF;
            panelConfig.position.x = 300.0f;
            if (D_800D2458) {
                panelConfig.position.y = 32.0f;
            } else {
                panelConfig.position.y = 79.0f;
            }
            panelConfig.position.z = 0.0f;
            panelExtra.unk0 = D_800A5480; /* variables.h declares D_800A5480 as a single f32 */
            panelExtra.unkC = 0.3010000288f;
            panelExtra.unk10 = 0.3010000288f;
            display->panel = func_15157010(&panelConfig, 0, 1.0f, 0, 0, 0x14, owner, kind);
            if (display->panel != NULL) {
                memcpy(&display->panel->extra, &panelExtra, sizeof(panelExtra));
            }
            {
                const s32 digitSprites[10] = { 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD };
                u8 i;

                digitArg.x = -500.0f;
                if (D_800D2458) {
                    digitArg.y = -15.0f;
                } else {
                    digitArg.y = -62.0f;
                }
                digitArg.unk12 = 300;
                digitArg.flags = 0x20;
                digitArg.unk16 = 1;
                digitArg.unk18 = 0xFF;
                digitArg.unk1A = 0;
                digitArg.unk1B = 0xFF;
                digitArg.unk1C = 0xFF;
                digitArg.unk1D = 0xFF;
                digitArg.unk1E = 0xFF;
                digitArg.unk1F = 0xFF;
                digitArg.unk20 = 0xFF;
                digitArg.unk21 = 0xFF;
                digitArg.unk22 = 0xFF;
                digitArg.unk23 = 0xFF;
                digitArg.unk24 = 0;
                digitArg.unk28 = 0x200004;
                digitArg.unk2C = 0x9F0601;
                digitArg.unk40 = 0;
                digitArg.unk41 = 0;
                digitArg.unk30 = 0x19;
                digitArg.unk34 = 0x13;
                digitArg.unk38 = 0x80;
                digitArg.unk3C = 0x20;
                digitArg.unk44 = 0;
                digitArg.unk50 = 0.0f;
                digitArg.unk54 = 0.0f;
                digitArg.sizeY = 16.0f;
                digitArg.sizeX = 16.0f;
                digitArg.unk48 = 1.f; // matching: spelling it 1.0f swaps the FP registers of 0.0f and 1.0f
                digitArg.unk4C = 1.f;
                if (digits[0] == 0) {
                    digitArg.unk10 = digitSprites[digits[1]];
                    display->digits[0] = func_1515548C(&digitArg, 0, NULL, 0, 0, owner, kind);
                    display->digits[1] = NULL;
                } else {
                    for (i = 0; i < 2; i++) {
                        digitArg.unk10 = digitSprites[digits[i]];
                        display->digits[i] = func_1515548C(&digitArg, 0, NULL, 0, 0, owner, kind);
                    }
                }
            }
        }
    }
}

s32 func_1517EF00(s32);
s32 func_15181CC8(s32);

/* Update of func_151BF340's counter. It waits until D_800C35EA != 1, func_15181CC8(0) != 0, func_1517EF00(0) == 0
 * and D_800BE9B4 == 0; then, as its 110-unit timer runs down by D_800BE9A4, it slides in over the first 20.57
 * units, holds, slides out over the last 20.57 and ends the effect. */
void func_151BF81C(CounterEffect *effect) {
    f32 slide;
    CounterDisplay *display;

    display = &effect->display;
    if (D_800BEAC1 != 0 || D_800BEAC0 != 0) {
        effect->header.unkE = -1;
        effect->header.unkD |= 1;
        return;
    }
    if (!(display->flags & 1)) {
        if (D_800C35EA == 1) {
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
        display->panel->x = slide * -85.0f + 150.0f;
        if (D_800D2458 != 0) {
            display->panel->y = 32.0f;
        } else {
            display->panel->y = 79.0f;
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
        effect->header.unkE = -1;
        effect->header.unkD |= 1;
    }
}

/* Frees the counter's panel and digits. */
void func_151BFB2C(CounterEffect *effect) {
    u8 i;
    CounterDisplay *display = &effect->display;

    if (display->panel != NULL) {
        func_1516972C((struct102 *)display->panel);
    }
    for (i = 0; i < 2; i++) {
        if (display->digits[i] != NULL) {
            func_1516972C((struct102 *)display->digits[i]);
        }
    }
}

/* Destructor of func_151BF340's counter effect. */
void func_151BFBA4(CounterEffect *effect) {
    func_151BFB2C(effect);
    func_1514933C(&effect->header);
}
