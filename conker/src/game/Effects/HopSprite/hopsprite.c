#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/sprite.h"
#include "conker/trail.h"
#include "conker/view.h"
#include "libc/string.h"

/* This file's .data: the texture scroll offsets; >> 7 gives the 10.2 fixed-point tile coordinate. */
s32 gTile0ScrollS = 0;
s32 gTile0ScrollT = 0;
s32 gTile1ScrollS = 0;
s32 gTile1ScrollT = 0;

void effect_spawnGroundFxNearPlayer(TimedEffect *object) {
    trail_spawnNearCamera(&movement_obj[0], movement_obj[0].ypos + 300.0f, -10000.0f);
}

/* Advances the texture scroll offsets by the frames elapsed. */
void effect_scrollTileTextures(s32 unused) {
    gTile0ScrollS += gFrameTicks * 26;
    gTile0ScrollT -= gFrameTicks * 28;
    gTile1ScrollS -= gFrameTicks * 28;
}

/* Sets the scrolled tile sizes of tiles 0 and 1. */
Gfx *effect_setScrolledTileSizes(Gfx *gfx) {
    gDPSetTileSize(gfx++, 0, gTile0ScrollS >> 7, gTile0ScrollT >> 7, 0x47E, 0x47E);
    gDPSetTileSize(gfx++, 1, gTile1ScrollS >> 7, gTile1ScrollT >> 7, 0x47E, 0x47E);
    return gfx;
}

/* Particle motion: while flags & 1, hop from startX to startX + distance along an arc. */
typedef struct {
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ f32 phase; // 0 to 1 over the hop
    /* 0x08 */ f32 startX;
    /* 0x0C */ f32 distance;
} HopMotion;

/* Particle motion: fall under gravity and bounce on a floor line (y grows downwards). */
typedef struct {
    /* 0x00 */ Vec2f vel;
    /* 0x08 */ f32 gravity;
    /* 0x0C */ f32 floorY;
} BounceMotion;

/* A screen sprite made by sprite_newScreenSprite (ScreenSprite in game/Sprite/ScreenSprite/screensprite.c, 0x70 bytes) with its motion data. */
struct HopSprite {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ Vec2f pos;
    /* 0x18 */ u8 pad18[0x70 - 0x18];
    /* 0x70 */ union {
        HopMotion hop;
        BounceMotion bounce;
    } motion;
};

/* Spawns the hopping particle, waiting off screen at (-300, 72). */
void effect_spawnHopSprite(s16 unk12, u8 unkC, s32 unk1) {
    ScreenSpriteInfo spawn;
    HopMotion hop;
    HopSprite *particle;

    spawn.y = 72.0f;
    spawn.texture = 0xC1;
    spawn.timer = unk12;
    spawn.flags = 0x31;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    spawn.kind = 8;
    spawn.color1.red = 0xFF;
    spawn.color1.green = 0xFF;
    spawn.color1.blue = 0xFF;
    spawn.color1.alpha = 0xFF;
    spawn.color2.red = 0xFF;
    spawn.color2.green = 0xFF;
    spawn.color2.blue = 0xFF;
    spawn.color2.alpha = 0xFF;
    spawn.unk23 = 0xFF;
    spawn.unk28 = 0x200004;

    hop.flags = 0;
    hop.phase = 0.0f;
    spawn.x = -300.0f;
    hop.startX = -300.0f;
    hop.distance = 0.0f;
    spawn.halfHeight = 7.5f;
    spawn.halfWidth = 7.5f;
    spawn.otherModeH = 0;
    spawn.unk2C = 0x9F0601;
    spawn.unk40 = 0;
    spawn.unk41 = 10;
    spawn.combineMode = 0x17;
    spawn.unk34 = 0x44;
    spawn.unk38 = 0x80;
    spawn.unk3C = 0x20;
    spawn.scaleY = 1.0f;
    spawn.scaleX = 1.0f;
    spawn.offsetT = 0.0f;
    spawn.offsetS = 0.0f;
    spawn.viewport = gScreenLayout;

    particle = sprite_newScreenSprite(&spawn, 12, 0, 0, 0x10, unkC, unk1);
    if (particle != NULL) {
        memcpy(&particle->motion.hop, &hop, sizeof(hop));
    }
}

/* Hopping particle update: follows the hop arc and, on landing, kicks up 2 to 5 bouncing
   dust particles. */
s32 effect_updateHopSprite(HopSprite *hopper) {
    f32 phase;
    ScreenSpriteInfo spawn;
    BounceMotion dust;
    s8 count;
    s32 rand;
    s32 bit2;
    HopSprite *particle;
    HopMotion *hop;

    hop = &hopper->motion.hop;
    if (hop->flags & 1) {
        hop->phase += 0.19000001f * gFrameDelta;
        phase = hop->phase;
        if (phase >= 1.0f) {
            hop->phase = 1.0f;
            hop->flags &= ~1;
            count = (math_random() & 3) + 2;
            dust.floorY = hopper->pos.y + 15.0f;
            // matching: x and y are copied as one Vec2f
            *(Vec2f *)&spawn.x = hopper->pos;
            spawn.fadeStart = 12;
            spawn.fadeRate = 0x15;
            spawn.kind = 0;
            spawn.color1.red = 0xFF;
            spawn.color1.green = 0xFF;
            spawn.color1.blue = 0xFF;
            spawn.color2.red = 0xFF;
            spawn.color2.green = 0xFF;
            spawn.color2.blue = 0xFF;
            spawn.color2.alpha = 0xFF;
            spawn.unk23 = 0xFF;
            spawn.otherModeH = 0;
            spawn.unk28 = 0x200004;
            spawn.unk2C = 0x9F0601;
            spawn.unk40 = 0;
            spawn.unk41 = 10;
            spawn.combineMode = 0x17;
            spawn.unk34 = 0x44;
            spawn.unk38 = 0x80;
            spawn.unk3C = 0x20;
            spawn.scaleX = 1.0f;
            spawn.scaleY = 1.0f;
            spawn.offsetT = 0.0f;
            spawn.offsetS = 0.0f;
            spawn.viewport = gScreenLayout;
            for (; count > 0; count--) {
                spawn.color1.alpha = (math_random() % 101U) + 155;
                spawn.texture = gKindParticleTexturePickers[2]();
                rand = math_random();
                phase = (math_randomFloat() * 2) + 2.5f; // matching: the hop phase's register holds the dust speed
                math_dirFromBinAngleXZ((rand % 65U) - 160, phase, &dust.vel.x, &dust.vel.y);
                dust.gravity = (math_randomFloat() * 0.333f) + 0.404f;
                spawn.halfWidth = spawn.halfHeight = (math_randomFloat() * 1.79f) + 1.6999999f;
                spawn.timer = (math_random() % 26U) + 30;
                if (math_random() & 1) {
                    bit2 = 2;
                } else {
                    bit2 = 0;
                }
                if (math_random() & 1) {
                    rand = 4; // matching: reusing rand's register for this flag bit
                } else {
                    rand = 0;
                }
                spawn.flags = rand | 1 | bit2 | 8 | 0x10 | 0x20; // matching: folding the constants into 0x39 changes the code
                particle = sprite_newScreenSprite(&spawn, 13, 0, 0, 0x10, hopper->unkC, hopper->unk1);
                if (particle != NULL) {
                    memcpy(&particle->motion.bounce, &dust, sizeof(dust));
                }
            }
            phase = hop->phase;
        }
        hopper->pos.x = hop->startX + (phase * hop->distance);
        hopper->pos.y = (sinf(hop->phase * 3.1415927f) * -17.5f) + 72.0f;
    }
    return 1;
}

/* Hopping particle message handler: message 0x52 starts a hop to the x given in the payload. */
void effect_hopSpriteMessage(HopSprite *hopper, f32 *targetX, u8 type) {
    HopMotion *hop = &hopper->motion.hop;

    if (type == 0x52) {
        hop->flags |= 1;
        hop->phase = 0.0f;
        hop->startX = hopper->pos.x;
        hop->distance = targetX[0] - hopper->pos.x;
    }
}

/* Sends message 0x52: hop to targetX. */
void effect_sendHopMessage(f32 targetX) {
    f32 payload = targetX;

    func_1515572C((s32)&payload, 0x52);
}

/* Bouncing particle update: integrates the motion and bounces off the floor, damped. */
s32 effect_updateBounceSprite(HopSprite *particle) {
    BounceMotion *m = &particle->motion.bounce;

    particle->pos.x += m->vel.x * gFrameDelta;
    particle->pos.y += (m->vel.y * gFrameDelta) + (((0.5f * m->gravity) * gFrameDelta) * gFrameDelta);
    m->vel.y += m->gravity * gFrameDelta;
    if (m->floorY < particle->pos.y) {
        particle->pos.y = m->floorY;
        m->vel.x *= 0.3f;
        m->vel.y = fabsf(m->vel.y) * -0.15f;
    }
    return 1;
}
