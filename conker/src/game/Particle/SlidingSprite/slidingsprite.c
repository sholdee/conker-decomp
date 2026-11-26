#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Data copied into a effect_newTimedEffectNoDraw particle at offset 0x28 by func_1515AF90. */
typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 upperFloorY; // highest floor between the camera's eye and y = 19500
} GroundParticle;


/*
 * With a 1-in-16 chance, spawns a particle (effect_newTimedEffectNoDraw) 107-2010 units from the player's camera,
 * at an angle within 20 units of its yaw (unk380, converted from degrees). The spot needs a floor
 * above the eye; the particle sits on the floor at most 250 units below the eye (else at that depth).
 */
void func_1515AF90(s16 player) {
    if (!(func_150ADA20() & 0xF)) {
        void *particle;
        GroundParticle effect;
        Vec3f *eye;
        s32 yaw;
        u8 valid;
        s16 angle;
        f32 distance;
        f32 cosine;
        f32 sine;
        CollisionHit hit;
        Vec3f probe;
        CollisionHit lowerHit;
        Vec3f lowerProbe;
        f32 bottom;

        valid = TRUE;
        eye = &camera_global_Cameras[player].eye;
        yaw = camera_global_Cameras[player].TrueCameraAngle * 0.7111111283f;
        {
            u32 roll = func_150ADA20();

            angle = (roll % 41U) + yaw - 20;
            distance = func_150ADA68() * 1903.0f + 107.0f;
            cosine = math_cosBinAngleMasked(angle);
            sine = math_cosBinAngleMasked(angle - 0x40);
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
        if (collision_probeFloorSurfABSet0(&probe, 0, eye->y, &hit)) {
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
            if (collision_probeFloorSurfABSet0(&lowerProbe, 0, bottom, &lowerHit)) {
                effect.position.y = lowerHit.height;
            } else {
                effect.position.y = bottom;
            }
        }
        effect.unkC = (func_150ADA68() * 0.0f + 195.0f) * 0.001000000047f;
        effect.unk10 = 0.0f;
        if (valid) {
            particle = effect_newTimedEffectNoDraw((func_150ADA20() % 61U) + 5, -1, 12, 1, 7, 0x18, 0xFF, 0);
            if (particle != NULL) {
                memcpy((u8 *)particle + 0x28, &effect, sizeof(effect));
            }
        }
    }
}

/* Effect update: accumulates rate * gFrameDelta and spawns one particle_newSpriteParticle particle per whole unit. */
void func_1515B21C(EmitterObject *obj) {
    RiseEmitterState *emitter;
    void *particle;
    ParticleDesc6C desc;
    RiseMotion extra;

    emitter = &obj->emitter;
    emitter->accumulator += emitter->rate * gFrameDelta;
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
        extra.widthPhase = 0;
        extra.heightPhase = 0;
        extra.swayPhaseX = 0;
        extra.swayPhaseZ = 0;
        desc.vel.x = 0.0f;
        desc.vel.z = 0.0f;
        extra.maxRiseSpeed = 1.200000048f;
        extra.swayPerSpeed = 0.8333333135f;
        extra.ceiling = emitter->unk14;

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
            extra.baseSize = size;
            desc.height = size;
            extra.widthPhaseStep = (func_150ADA20() % 5U) + 4;
            extra.heightPhaseStep = (func_150ADA20() % 5U) + 4;
            extra.widthWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.baseSize;
            extra.heightWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.baseSize;
            extra.swayPhaseStepX = (func_150ADA20() & 3) + 2;
            extra.swayPhaseStepZ = (func_150ADA20() & 3) + 2;
            extra.swayX = ((func_150ADA68() * 0.0700000003f) + 0.05999999866f) * extra.baseSize;
            extra.swayZ = ((func_150ADA68() * 0.0700000003f) + 0.05999999866f) * extra.baseSize;
            desc.gravity = ((func_150ADA68() * 1005.0f) + 600.0f) * 0.001000000047f;

            particle = particle_newSpriteParticle(&desc, 2, 0x28);
            if (particle != NULL) {
                memcpy((u8 *)particle + 0xA8, &extra, sizeof(extra));
            }
            emitter->accumulator -= 1.0f;
        } while (emitter->accumulator > 1.0f);
    }
}

#define MSG_REMOVE_PLAYER_SPRITES 0xB

/* Broadcasts message 0xB to the screen sprites; func_1515B62C removes the player's sliding sprites. */
void func_1515B5F4(s16 player) {
    s16 payload[2]; // matching: only payload[0] is sent; the array keeps the stack slot

    payload[0] = player;
    func_1515572C((s32)payload, MSG_REMOVE_PLAYER_SPRITES);
}

/* Message handler of sprite kind 1: on message 0xB, removes the sprite if it belongs to that player. */
void func_1515B62C(SlidingSprite *sprite, s16 *payload, u8 type) {
    if (type == MSG_REMOVE_PLAYER_SPRITES) {
        if (sprite->data.player == payload[0]) {
            effect_destroyObject(&sprite->header);
        }
    }
}


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
    info.color1.red = 255;
    info.color1.green = 255;
    info.color1.blue = 255;
    info.color2.red = 255;
    data.player = player;
    info.viewport = player;
    info.color2.green = 255;
    info.color2.blue = 255;
    info.color2.alpha = 255;
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
        info.texture = D_8008FD04[particle_getContextEffectKind()]();
        info.timer = (func_150ADA20() % 51U) + 50;
        info.color1.alpha = (func_150ADA20() % 101U) + 100;
        if (func_150ADA20() & 1) {
            info.flags |= 2;
        }
        if (func_150ADA20() & 1) {
            info.flags |= 4;
        }
        sprite = sprite_newScreenSprite(&info, 2, 0, 0, sizeof(SlidingSpriteData), 255, 1);
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
    sprite->info.y += previous * gFrameDelta + 0.5f * sprite->data.acceleration * gFrameDelta;
    sprite->data.velocity = sprite->data.velocity + sprite->data.acceleration * gFrameDelta;
    sprite->info.halfHeight =
        sprite->data.baseHalfHeight + sprite->data.stretch * (sprite->data.velocity + previous) * 0.5f;
    return 1;
}

/*
 * Camera hooks, called each frame by game/Camera/Camera/camera.c with the camera's player index, depending on
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
