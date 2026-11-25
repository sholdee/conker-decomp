#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Data copied into a func_151491F4 particle at offset 0x28 by func_1515AF90. */
typedef struct {
    /* 0x00 */ vertex position;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 upperFloorY; // highest floor between the camera's eye and y = 19500
} GroundParticle;

/* Result of a floor probe (CollisionHit in game_71820.c). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ s16 tri[9];
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 material;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 source;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ void *surface;
} GroundHit;

s32 func_1504697C(vertex *pos, u16 mask, f32 bottom, GroundHit *hit);

/*
 * With a 1-in-16 chance, spawns a particle (func_151491F4) 107-2010 units from the player's camera,
 * at an angle within 20 units of its yaw (unk380, converted from degrees). The spot needs a floor
 * above the eye; the particle sits on the floor at most 250 units below the eye (else at that depth).
 */
void func_1515AF90(s16 player) {
    if (!(func_150ADA20() & 0xF)) {
        void *particle;
        GroundParticle effect;
        vertex *eye;
        s32 yaw;
        u8 valid;
        s16 angle;
        f32 distance;
        f32 cosine;
        f32 sine;
        GroundHit hit;
        vertex probe;
        GroundHit lowerHit;
        vertex lowerProbe;
        f32 bottom;

        valid = TRUE;
        eye = &D_800DBFF0[player].eye;
        yaw = D_800DBFF0[player].unk380 * 0.7111111283f;
        {
            u32 roll = func_150ADA20();

            angle = (roll % 41U) + yaw - 20;
            distance = func_150ADA68() * 1903.0f + 107.0f;
            cosine = func_151423D8(angle);
            sine = func_151423D8(angle - 0x40);
            effect.position.x = eye->x - distance * sine;
            effect.position.z = eye->z - distance * cosine;
        }
        probe.x = effect.position.x;
        hit.material = 0;
        hit.flags = 0;
        hit.source = 0;
        hit.surface = NULL;
        probe.y = 19500.0f;
        probe.z = effect.position.z;
        hit.height = -10000.0f;
        if (func_1504697C(&probe, 0, eye->y, &hit)) {
            effect.upperFloorY = hit.height;
        } else {
            valid = FALSE;
        }
        if (valid) {
            bottom = eye->y + -250.0f;
            lowerHit.surface = NULL;
            lowerHit.source = 0;
            lowerHit.flags = 0;
            lowerHit.material = 0;
            lowerHit.height = -10000.0f;
            lowerProbe.x = effect.position.x;
            lowerProbe.y = eye->y;
            lowerProbe.z = effect.position.z;
            if (func_1504697C(&lowerProbe, 0, bottom, &lowerHit)) {
                effect.position.y = lowerHit.height;
            } else {
                effect.position.y = bottom;
            }
        }
        effect.unkC = (func_150ADA68() * 0.0f + 195.0f) * 0.001000000047f;
        effect.unk10 = 0.0f;
        if (valid) {
            particle = func_151491F4((func_150ADA20() % 61U) + 5, -1, 12, 1, 7, 0x18, 0xFF, 0);
            if (particle != NULL) {
                memcpy((u8 *)particle + 0x28, &effect, sizeof(effect));
            }
        }
    }
}

/* matching: the first 0x6C bytes of ParticleDesc; a full ParticleDesc on the stack changes the frame. */
typedef struct ParticleDesc6C {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode; /* low byte: D_800A4AC8 index, high byte: func_1513F4E4 arg */
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;      /* 16.16 animation frame */
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;     /* D_80090B60 index */
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ vertex pos;
    /* 0x3C */ vertex offset;
    /* 0x48 */ vertex vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ u32 flags;      /* PARTICLE_* */
    /* 0x5C */ struct ParticleViewMask *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* D_80089760 index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* D_80089800 index, -1 for none */
    /* 0x64 */ s8 drawFunc;    /* D_80089804 index, -1 for none */
    /* 0x65 */ u8 handlerSet;  /* D_80089814/D_80089844/D_80089878 index */
    /* 0x66 */ u8 views;       /* bit per view the particle is drawn in */
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 minSize;
    /* 0x6A */ s16 unk6A;
} ParticleDesc6C;

/* Extra data copied into a func_151303BC particle at +0xA8 (ParticleExtra in game_117490.c). */
typedef struct {
    /* 0x00 */ f32 size;
    /* 0x04 */ u8 unk4[4];
    /* 0x08 */ f32 unk8[2];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 unk18[4];
    /* 0x1C */ f32 unk1C[2];
    /* 0x24 */ f32 unk24;
} ParticleExtra;

/* Emitter state stored in an effect object at +0x28. */
typedef struct {
    /* 0x00 */ vertex position;
    /* 0x0C */ f32 rate;        // particles per unit of D_800BE9A4
    /* 0x10 */ f32 accumulator; // pending particles; one is spawned per whole unit
    /* 0x14 */ f32 unk14;
} ParticleEmitter;

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ ParticleEmitter emitter;
} EmitterObject;

void *func_151303BC(ParticleDesc6C *, s32, s32);

/* Effect update: accumulates rate * D_800BE9A4 and spawns one func_151303BC particle per whole unit. */
void func_1515B21C(EmitterObject *obj) {
    ParticleEmitter *emitter;
    void *particle;
    ParticleDesc6C desc;
    ParticleExtra extra;

    emitter = &obj->emitter;
    emitter->accumulator += emitter->rate * D_800BE9A4;
    if (emitter->accumulator > 1.0f) {
        f32 size;

        desc.texture = 47;
        desc.renderMode = 0xC01;
        desc.unk0 = 0x200005;
        desc.unk4 = 0;
        desc.b2 = 0;
        desc.g2 = 0;
        desc.r2 = 0;
        desc.a = 0;
        desc.b = 0;
        desc.g = 0;
        desc.r = 0;
        desc.frameRate = 0;
        desc.frame = 0;
        desc.brightness = 255;
        desc.scaleStart = 0;
        desc.scaleRate = 0.0f;
        desc.flags = 0x3207;
        desc.primColorMode = 5;
        desc.envColorMode = 5;
        desc.updateFunc = 4;
        desc.deathFunc = -1;
        desc.pos = emitter->position;
        desc.offset.x = 0.0f;
        desc.offset.y = 0.0f;
        desc.offset.z = 0.0f;
        desc.fadeStart = 50;
        desc.fadeRate = 5;
        extra.unk4[0] = 0;
        extra.unk4[1] = 0;
        extra.unk18[0] = 0;
        extra.unk18[1] = 0;
        desc.vel.x = 0.0f;
        desc.vel.z = 0.0f;
        extra.unk10 = 1.200000048f;
        extra.unk14 = 0.8333333135f;
        extra.unk24 = emitter->unk14;

        if (func_150ADA20() & 1) {
            desc.flags |= 0x40;
        }
        if (func_150ADA20() & 1) {
            desc.flags |= 0x80;
        }

        do {
            desc.vel.y = func_150ADA68() * 12.0f * 0.1000000015f;
            desc.timer = (func_150ADA20() % 152U) + 150;
            desc.alpha = (func_150ADA20() % 151U) + 55;
            size = (func_150ADA68() * 200.0f) + 100.0f;
            desc.width = size;
            extra.size = size;
            desc.height = size;
            extra.unk4[2] = (func_150ADA20() % 5U) + 4;
            extra.unk4[3] = (func_150ADA20() % 5U) + 4;
            extra.unk8[0] = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.size;
            extra.unk8[1] = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.size;
            extra.unk18[2] = (func_150ADA20() & 3) + 2;
            extra.unk18[3] = (func_150ADA20() & 3) + 2;
            extra.unk1C[0] = ((func_150ADA68() * 0.0700000003f) + 0.05999999866f) * extra.size;
            extra.unk1C[1] = ((func_150ADA68() * 0.0700000003f) + 0.05999999866f) * extra.size;
            desc.gravity = ((func_150ADA68() * 1005.0f) + 600.0f) * 0.001000000047f;

            particle = func_151303BC(&desc, 2, 0x28);
            if (particle != NULL) {
                memcpy((u8 *)particle + 0xA8, &extra, sizeof(extra));
            }
            emitter->accumulator -= 1.0f;
        } while (emitter->accumulator > 1.0f);
    }
}

/* Description a screen sprite is created from (ScreenSpriteInfo in game_182140.c). */
typedef struct {
    /* 0x00 */ f32 x; // centre, in viewport units
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 halfWidth;
    /* 0x0C */ f32 halfHeight;
    /* 0x10 */ u8 texture;
    /* 0x11 */ u8 pad11;
    /* 0x12 */ s16 timer;
    /* 0x14 */ u16 flags;
    /* 0x16 */ s16 fadeStart;
    /* 0x18 */ s16 fadeRate;
    /* 0x1A */ u8 kind; // index into D_8008AD04 (message handlers)
    /* 0x1B */ u8 color1[4];
    /* 0x1F */ u8 color2[4];
    /* 0x23 */ u8 unk23;
    /* 0x24 */ s32 otherModeH;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 combineMode;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ u8 viewport; // the only viewport drawn in when (flags & 0x10)
    /* 0x45 */ u8 pad45[3];
    /* 0x48 */ f32 unk48[4];
} ScreenSpriteInfo;

/* Per-sprite data func_1515B674 appends to its sprites (at ScreenSprite +0x70). */
typedef struct {
    /* 0x00 */ s16 player;
    /* 0x04 */ f32 acceleration;
    /* 0x08 */ f32 velocity;
    /* 0x0C */ f32 baseHalfHeight;
    /* 0x10 */ f32 stretch;
    /* 0x14 */ f32 unk14;
} SlidingSpriteData;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ ScreenSpriteInfo info;
    /* 0x68 */ u8 pad68[8];
    /* 0x70 */ SlidingSpriteData data;
} SlidingSprite;

#define MSG_REMOVE_PLAYER_SPRITES 0xB

extern void func_1515572C(s16 *payload, s32 type);

/* Broadcasts message 0xB to the screen sprites; func_1515B62C removes the player's sliding sprites. */
void func_1515B5F4(s16 player) {
    s16 payload[2]; // matching: only payload[0] is sent; the array keeps the stack slot

    payload[0] = player;
    func_1515572C(payload, MSG_REMOVE_PLAYER_SPRITES);
}

/* Message handler of sprite kind 1: on message 0xB, removes the sprite if it belongs to that player. */
void func_1515B62C(SlidingSprite *sprite, s16 *payload, u8 type) {
    if (type == MSG_REMOVE_PLAYER_SPRITES) {
        if (sprite->data.player == payload[0]) {
            func_1516972C((struct102 *)sprite);
        }
    }
}

extern u8 (*D_8008FD04[])(void);
extern s32 func_151D8E20(void);
extern SlidingSprite *func_1515548C(ScreenSpriteInfo *, s32, s32, s32, s32, u8, s32);

/*
 * Spawns 10 white sprites (kind 1, update callback 2 = func_1515B994) at random spots of the player's
 * viewport; they slide along y and stretch as they speed up. Called when the camera rises back above
 * its unk360 height.
 */
void func_1515B674(s16 player) {
    s32 pad0; // matching: unused stack slots around info
    ScreenSpriteInfo info;
    s32 pad1;
    SlidingSpriteData data;
    SlidingSprite *sprite;
    s16 count;

    count = (func_150ADA20() % 1U) + 10;
    info.flags = 0x19;
    info.kind = 1;
    info.fadeStart = 20;
    info.fadeRate = 12;
    info.color1[0] = 255;
    info.color1[1] = 255;
    info.color1[2] = 255;
    info.color2[0] = 255;
    data.player = player;
    info.viewport = player;
    info.color2[1] = 255;
    info.color2[2] = 255;
    info.color2[3] = 255;
    info.unk23 = 255;
    info.otherModeH = 0;
    info.unk28 = 0;
    info.unk2C = 0;
    info.unk40 = 0;
    info.unk41 = 9;
    info.combineMode = 7;
    info.unk34 = 60;
    info.unk38 = 0x80;
    info.unk3C = 0x20;
    while (count--) {
        data.acceleration = ((func_150ADA68() * 105.0f) + 105.0f) * 0.001000000047f;
        data.velocity = func_150ADA68() * 3008.0f * 0.001000000047f;
        info.halfHeight = info.halfWidth = data.baseHalfHeight =
            ((func_150ADA68() * 1579.0f) + 489.0f) * 0.009999999776f;
        data.stretch = ((func_150ADA68() * 301.0f) + 94.0f) * 0.009999999776f;
        info.x = (func_150ADA68() * 200.0f) + -100.0f;
        info.y = (func_150ADA68() * 148.0f) + -100.0f;
        info.texture = D_8008FD04[func_151D8E20()]();
        info.timer = (func_150ADA20() % 51U) + 50;
        info.color1[3] = (func_150ADA20() % 101U) + 100;
        if (func_150ADA20() & 1) {
            info.flags |= 2;
        }
        if (func_150ADA20() & 1) {
            info.flags |= 4;
        }
        sprite = func_1515548C(&info, 2, 0, 0, sizeof(SlidingSpriteData), 255, 1);
        if (sprite != NULL) {
            memcpy(&sprite->data, &data, sizeof(SlidingSpriteData));
        }
    }
}

/*
 * Sprite update callback 2: moves the sprite along y with constant acceleration and stretches its
 * half height with the average speed. Always returns 1 (keep alive).
 */
s32 func_1515B994(SlidingSprite *sprite) {
    f32 previous;

    previous = sprite->data.velocity;
    sprite->info.y += previous * D_800BE9A4 + 0.5f * sprite->data.acceleration * D_800BE9A4;
    sprite->data.velocity = sprite->data.velocity + sprite->data.acceleration * D_800BE9A4;
    sprite->info.halfHeight =
        sprite->data.baseHalfHeight + sprite->data.stretch * (sprite->data.velocity + previous) * 0.5f;
    return 1;
}

/*
 * Camera hooks, called each frame by game_14FF90.c with the camera's player index, depending on
 * whether its eye is at or below its unk360 height ("below") and was so last frame. "Special" means
 * camera status bit 4 or mode bit 0x40000 is set.
 */

/* Stays above. */
void func_1515BA10(s16 player) {
}

/* Stays below. */
void func_1515BA1C(s16 player) {
    func_1515AF90(player);
}

/* Snaps above, or rises above while special. */
void func_1515BA48(s16 player) {
}

/* Rises above (not special). */
void func_1515BA54(s16 player) {
    func_1515B674(player);
}

/* Snaps below, or goes below while special. */
void func_1515BA80(s16 player) {
    func_1515B5F4(player);
}

/* Goes below (not special). */
void func_1515BAAC(s16 player) {
    func_1515B5F4(player);
}
