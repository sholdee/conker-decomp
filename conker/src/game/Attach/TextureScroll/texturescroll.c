#include <ultra64.h>
#include "effects.h"
#include "conker/attach.h"
#include "conker/collision.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/texture.h"

/* matching: no header declares particle_spawnImpactMark (emissionsource.c): followeffects.c and sightbeam.c were compiled with other views */
void particle_spawnImpactMark(f32 size, u8 alpha, CollisionTri *surface, Vec3f *position, s16 duration, u8 lit, u8 arg6, u8 r, u8 g,
                   u8 b, u8 owner, s32 list);


void attach_emptyLevelUpdateHook1(s32 arg0) {
}


/* First byte of a Gfx command: its opcode, read as a signed byte. */
#define GFX_OPCODE(gfx) (*(s8 *)(gfx))


/* Model attachment hook 1 (gAttachHooks[1]). While the actor runs action 13 with anim->unk8 in 23..40,
 * scrolls the texture of the first G_SETTILESIZE command in the display list horizontally. */
s32 attach_hookScrollTexH(ModelAttachment *att, Actor *actor) {
    AnimInstance *anim = actor->anim;
    Gfx *list;
    s32 i;
    s32 uls;
    s32 w0;
    f32 t;

    if (anim == NULL) {
        return 0;
    }
    att->seg6Id = D_80090298[6];
    if (actor->animseq.uh == 13) {
        if (23.0f <= anim->frame[0] && anim->frame[0] <= 40.0f) {
            att->seg6Id = D_80090298[7];
            t = anim->frame[0] - 23.0f;
            t *= 1.0f / 17.0f;
            list = *att->dlists;
            for (i = 0; GFX_OPCODE(&list[i]) != (s8)G_SETTILESIZE; i++) {
            }
            w0 = list[i].words.w0;
            uls = 70.0f * t + 2.0f;
            if (uls < 0) {
                uls += (w0 >> 12) & 0xFFF;
            }
            /* rewrite only w0 (uls, ult); the tile and lrs/lrt in w1 stay */
            list[i].words.w0 = _SHIFTL(G_SETTILESIZE, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(2, 0, 12);
        }
    }
    return 0;
}

/* Model attachment hook 19 (gAttachHooks[19]). Scrolls the first G_SETTILESIZE command's texture
 * vertically by a progress value (hookState2, percent; hookState is the phase) that rises to 100% once action
 * 0x18D reaches anim->unk8 46, then falls to 0 once action 0x18E reaches 34. */
s32 attach_hookScrollTexV(ModelAttachment *att, Actor *actor) {
    Gfx *list;
    s32 i;
    f32 t;

    switch (att->hookState) {
        case 0:
            att->hookState2 = 50;
            att->seg6Id = D_80090298[27];
            if (actor->animseq.uh == 0x18D && 46.0f <= actor->anim->frame[0]) {
                att->hookState = 1;
                att->seg6Id = D_80090298[28];
            }
            break;
        case 1:
            att->hookState2 += gFrameTicks;
            if (att->hookState2 >= 100) {
                att->hookState2 = 100;
                att->hookState = 2;
            }
            break;
        case 2:
            if (actor->animseq.uh == 0x18E && 34.0f <= actor->anim->frame[0]) {
                att->hookState = 3;
            }
            break;
        case 3:
            att->hookState2 -= gFrameTicks * 4;
            if (att->hookState2 <= 0) {
                att->hookState2 = 0;
                att->hookState = 4;
            }
            break;
    }
    t = att->hookState2 * 0.01f;
    list = *att->dlists;
    for (i = 0; GFX_OPCODE(&list[i]) != (s8)G_SETTILESIZE; i++) {
    }
    list[i].words.w0 = _SHIFTL(G_SETTILESIZE, 24, 8) | _SHIFTL(2, 12, 12) | _SHIFTL((s32)(120.0f * t + 2.0f), 0, 12);
    return 0;
}

/* Writes the two-halfword command { 0x42, 0 } and returns the position after it. */
GameCommand *func_150D8590(GameCommand *cmd, s32 arg1) {
    cmd->opcode = 0x42;
    cmd->value = 0;
    return cmd + 1;
}


/* Spawns impact effects at the actor's feet: a ground effect if the ground below is flagged,
 * then particle_startRandomEmitter's effect and a particle burst. */
void attach_spawnFootImpact(Actor *actor) {
    Vec3f pos;

    pos.x = actor->xpos;
    pos.y = actor->floorY;
    pos.z = actor->zpos;
    {
        Vec3f probe;
        CollisionHit ground;

        probe.x = pos.x;
        probe.y = pos.y + 100.0f;
        probe.z = pos.z;
        collision_initHitFromObject(&ground, actor);
        {
            SpriteBurst burst;

            {
                union { u32 value; f64 align; } random; // matching: keeps the value in a stack slot, not a temp
                f32 randomScale;

                if (collision_probeFloorSurfABSet0Cached(&probe, 0, pos.y - 200.0f, &ground) != 0 && (ground.flags & 1)) {
                    pos.x = probe.x;
                    pos.y = ground.height;
                    pos.z = probe.z;
                    randomScale = math_randomFloat();
                    random.value = math_random();
                    particle_spawnImpactMark(randomScale * 109.0f + 140.0f, ((random.value % 86U) + 170),
                                  &ground.tri, &pos, (math_random() % 251U) + 500, 0, 1, 0, 0, 0, 255, 0);
                }
                particle_startRandomEmitter(&pos, (math_random() % 62U) + 120, 255, 1);
            }
            burst.countBase = 26;
            burst.countRange = 7;
            burst.texture = 0x6C;
            burst.renderMode = 0x5103;
            burst.setGeometryMode = 0x200005;
            burst.clearGeometryMode = 0;
            burst.timerBase = 30;
            burst.timerRange = 10;
            burst.frameRateBase = 0;
            burst.frameRateRange = 0;
            burst.a = 255;
            burst.r = 0xB9;
            burst.g = 0xC7;
            burst.b = 0xC4;
            burst.r2 = 0x95;
            burst.g2 = 0x91;
            burst.b2 = 0x7E;
            burst.alphaBase = 100;
            burst.alphaRange = 0x9B;
            burst.brightness = 255;
            burst.fadeStart = 30;
            burst.fadeRate = 8;
            burst.scaleStart = 30;
            burst.scaleRate = 1.022876024f;
            burst.sizeBase = 150.0f;
            burst.sizeRange = 298.0f;
            burst.pos.x = pos.x;
            burst.pos.y = pos.y + 35.0f;
            burst.pos.z = pos.z;
            burst.yawBase = 0;
            burst.pitchBase = -16;
            burst.yawRange = 255;
            burst.pitchRange = 20;
            burst.speedBase = 18.0f;
            burst.speedRange = 18.0f;
            burst.gravityBase = 0.09300000221f;
            burst.gravityRange = 0.302000016f;
            burst.flags = 0x840E07;
            burst.updateFunc = 16;
            burst.deathFunc = -1;
            burst.primColorMode = 8;
            burst.envColorMode = 6;
            burst.unk64 = 1;
            burst.extra = 0.9054819942f;
            particle_spawnSpriteBurst(&burst, 255, 255, 1);
        }
    }
}

struct S150D88AC {
    u8 pad0[0x14];
    u8 *unk14;
    u8 *unk18;
};

/* Sets unk14[9] to whether unk18[0x6F] is clear. */
s32 attach_disableLightWithoutExpression(struct S150D88AC *arg0) {
    if (arg0->unk18[0x6F] != 0) {
        arg0->unk14[9] = 0;
    } else {
        arg0->unk14[9] = 1;
    }
    return 1;
}
