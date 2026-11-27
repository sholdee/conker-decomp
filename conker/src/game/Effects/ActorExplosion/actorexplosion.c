#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/beam.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/sound.h"
#include "conker/weapon.h"

static const s32 gExplosionSprayModels[3] = { 0xBD, 0xBE, 0xBF };
static const f32 gExplosionSprayScales[3] = { 1.0f, 1.0f, 1.0f };
static const s32 gExplosionBurstModels[2] = { 0x2D, 0x2E };
static const f32 gExplosionBurstScales[2] = { 1.0f, 1.0f };

/* Pulses the scale of bone matrix 62 of `actor`, faster as bit pairs of actor->taskint[0] are set. */
void func_150F1A00(Actor *actor) {
    f32 (*mtx)[4];
    u16 rates[3] = { 720, 2000, 3000 };
    s32 idx;
    TexAnimState *rec;
    u16 rate;
    f32 scale;
    s32 i;
    s32 j;

    idx = 0;
    if ((actor->taskint[0] & 3) == 3) {
        idx = 1;
    }
    if ((actor->taskint[0] & 0xC) == 0xC) {
        idx = 2;
    }
    rate = rates[idx];
    rec = &D_800CC298;
    rec->pulse += rate * gFrameTicks;
    scale = (((cosf(rec->pulse * 9.587380191e-05f) + 1.0f) * 0.5f) * 0.3500000238f) + 0.8999999762f;
    mtx = actor->ob_matrix_ptr[62];
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            mtx[i][j] *= scale;
        }
    }
}

/* Eases D_800CC298's tilt toward its lower limit while actor `index` is in state 0x21 and toward
 * its upper limit otherwise, then emits it as commands 0xB2, 0xB8, ... 0xD0 at `cmd`. */
GameCommand *func_150F1B48(GameCommand *cmd, s32 index) {
    s32 down;
    s32 step;
    s32 tilt;
    s32 i;

    down = (movement_obj[index].animseq.uh == 0x21) ? 1 : 0;
    step = gFrameTicks * 0x222;
    if (down) {
        tilt = D_800CC298.tilt - step;
        if (-6553.600098f < tilt) {
            D_800CC298.tilt = tilt;
        } else {
            D_800CC298.tilt = -0x1999;
        }
    } else {
        tilt = D_800CC298.tilt + step;
        if (tilt < 5461.333496f) {
            D_800CC298.tilt = tilt;
        } else {
            D_800CC298.tilt = 0x1555;
        }
    }
    for (i = 0; i < 6; i++) {
        cmd->opcode = 0xB2 + i * 6;
        cmd->value = D_800CC298.tilt;
        cmd++;
    }
    return cmd;
}

/* Picks actor->swapTexture[0] by state (0x14 or not) and actor->swapTexture[1] by the bit pairs of actor->taskint[0]. */
void func_150F1CB0(Actor *actor) {
    if (actor->animseq.uh == 0x14) {
        actor->swapTexture[0] = 0x1B;
    } else {
        actor->swapTexture[0] = 0xC;
    }
    actor->swapTexture[1] = 0x13;
    if ((actor->taskint[0] & 3) == 3) {
        actor->swapTexture[1] = 0x14;
    }
    if ((actor->taskint[0] & 0xC) == 0xC) {
        actor->swapTexture[1] = 0x17;
    }
}

/* Burst of effects at `actor` (particle_startPlayerRumbles and friends, sound 0x2B6/0x2BE, two particle
 * sprays scaled by the actor's size). */
void beam_spawnActorExplosion(Actor *actor, u8 arg1, s32 arg2) {
    Vec3f pos;
    f32 scale;

    if (actor == NULL) {
        return;
    }
    pos.x = actor->xpos;
    pos.y = actor->ypos + 20.0f;
    pos.z = actor->zpos;
    scale = (actor->scale_xz + actor->scale_y) * 0.5f;
    particle_startPlayerRumbles(&pos, 506.0f, 1013.0f, 0.0009871669f, 15, 20, arg1, arg2);
    particle_startPlayerCameraWobbles(&pos, 506.0f, 1013.0f, 0.0009871669f, 5, arg1, arg2);
    particle_spawnBlastSpray(&pos, arg1, arg2);
    sound_playAt((math_random() & 8) + 0x2B6, 0x7FFF, 0, 0, 0, pos.x, pos.y, pos.z, 4000, 6000);
    particle_spawnLightFlash(&pos, arg1, arg2);
    particle_spawnGrowFade(&pos, math_randomFloat() * 4.0f + 12.0f, 1.6409999f,
                  math_random() % 56U + 200, 0.0f, arg1, arg2);
    {
        DebrisSpray a;
        DebrisBurst b;

        a.countBase = 3;
        a.countRange = 0;
        a.pos = pos;
        a.speedBase = 30.0f;
        a.speedRange = 20.0f;
        a.gravityBase = -1.855000138f;
        a.gravityRange = 0.6980000138f;
        a.angles.yawBase = 0;
        a.angles.yawRange = 0xFF;
        a.angles.pitchBase = -64;
        a.angles.pitchRange = 44;
        a.lifeBase = 60;
        a.lifeRange = 20;
        a.scaleBase = 1 * scale; // matching: IDO emits both multiplies; plain scale / 0.0f differ
        a.scaleRange = 0 * scale;
        a.spin = 8.990000725f;
        a.floorHitChance = 0.0f;
        a.floorHitFn = 12;
        particle_spawnDebrisSpray(&a, gExplosionSprayModels, gExplosionSprayScales, 3, 0, 0, 0, 1, -1, 0, 0.0f, 0, 0, arg1, arg2);
        b.countBase = 5;
        b.countRange = 3;
        b.pos = pos;
        b.speedBase = 25.0f;
        b.speedRange = 15.0f;
        b.angles.yawBase = 0;
        b.angles.yawRange = 0xFF;
        b.angles.pitchBase = -64;
        b.angles.pitchRange = 30;
        b.lifeBase = 70;
        b.lifeRange = 20;
        b.gravityBase = -1.239000082f;
        b.gravityRange = 0.3760000169f;
        b.scaleBase = 0.503000021f;
        b.scaleRange = 0.2560000122f;
        b.spin = 13.93300056f;
        particle_spawnDebrisBurst(&b, gExplosionBurstModels, gExplosionBurstScales, 2, 0.0f, 0, arg1, arg2);
    }
}

/* Random sound 0x6C-0x6E for actor `index` once it is far enough from the origin, at most every
 * 128-255 ticks (gRandomSoundTimer counts down); sound_getPanVolumeAt derives the pan from its height. */
void beam_levelUpdateRandomSounds(s32 index) {
    f32 x;
    f32 z;
    s32 pan;
    s32 out2;
    s32 out3;
    Actor *actor;

    actor = &movement_obj[index];
    x = actor->xpos;
    z = actor->zpos;
    x *= x;
    z *= z;
    /* squares the already-squared terms: the test is on x^4 + z^4 */
    x = (x * x) + (z * z);
    if (gRandomSoundTimer != 0) {
        if (gFrameTicks < gRandomSoundTimer) {
            gRandomSoundTimer -= gFrameTicks;
        } else {
            gRandomSoundTimer = 0;
        }
    } else if (x > 6760000.0f) {
        // matching: indexes movement_obj again rather than reading through `actor`
        sound_getPanVolumeAt(0, movement_obj[index].ypos, 0, 0x7FFF, 20000, 19000, &pan, &out2, &out3);
        pan += 0x80;
        gRandomSoundTimer = (math_random() & 0x7F) + 0x80;
        sound_play((math_random() % 3U) + 0x6C, 24000, pan, 0, 0);
    }
}

/* Spawns weapon_spawnImpactBurst's effect at a random point 100 units around bone 19 of `actor`
 * (unless actor->cullMask's low nibble is 0xF). */
void beam_spawnImpactBurstNearBone(Actor *actor, s32 arg1, s32 arg2) {
    Vec3f offset;
    Vec3f pos;

    if ((actor->ob_matrix_ptr != NULL) && ((actor->cullMask & 0xF) != 0xF)) {
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 0.0f;
        math_dirFromBinAngleXZ(math_random() & 0xFF, 100.0f, &offset.x, &offset.z);
        math_transformOffset(&offset, &pos, &actor->ob_matrix_ptr[19]);
        weapon_spawnImpactBurst(&pos, arg1, arg2);
    }
}
