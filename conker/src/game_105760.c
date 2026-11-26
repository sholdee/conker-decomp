#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_150E7FEC (game_113D60.c) is declared in each file that calls it: game_1E73B0.c and game_1F4650.c were
// compiled with a different view of it.
void func_150E7FEC(f32 size, u8 arg1, CollisionTri *surface, Vec3f *position, s16 duration, u8 arg5, u8 arg6, u8 arg7, u8 arg8,
                   u8 arg9, u8 owner, s32 kind);


void func_150D82B0(s32 arg0) {
}


/* First byte of a Gfx command: its opcode, read as a signed byte. */
#define GFX_OPCODE(gfx) (*(s8 *)(gfx))


/* Model attachment hook 1 (D_800841C0[1]). While the actor runs action 13 with anim->unk8 in 23..40,
 * scrolls the texture of the first G_SETTILESIZE command in the display list horizontally. */
s32 func_150D82BC(ModelAttachment *att, Actor *actor) {
    AnimInstance *anim = actor->unk2D0;
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
        if (23.0f <= anim->frame && anim->frame <= 40.0f) {
            att->seg6Id = D_80090298[7];
            t = anim->frame - 23.0f;
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

/* Model attachment hook 19 (D_800841C0[19]). Scrolls the first G_SETTILESIZE command's texture
 * vertically by a progress value (unk3C, percent; unk38 is the phase) that rises to 100% once action
 * 0x18D reaches anim->unk8 46, then falls to 0 once action 0x18E reaches 34. */
s32 func_150D83D8(ModelAttachment *att, Actor *actor) {
    Gfx *list;
    s32 i;
    f32 t;

    switch (att->unk38) {
        case 0:
            att->unk3C = 50;
            att->seg6Id = D_80090298[27];
            if (actor->animseq.uh == 0x18D && 46.0f <= actor->unk2D0->frame) {
                att->unk38 = 1;
                att->seg6Id = D_80090298[28];
            }
            break;
        case 1:
            att->unk3C += D_800BE9E4;
            if (att->unk3C >= 100) {
                att->unk3C = 100;
                att->unk38 = 2;
            }
            break;
        case 2:
            if (actor->animseq.uh == 0x18E && 34.0f <= actor->unk2D0->frame) {
                att->unk38 = 3;
            }
            break;
        case 3:
            att->unk3C -= D_800BE9E4 * 4;
            if (att->unk3C <= 0) {
                att->unk3C = 0;
                att->unk38 = 4;
            }
            break;
    }
    t = att->unk3C * 0.01f;
    list = *att->dlists;
    for (i = 0; GFX_OPCODE(&list[i]) != (s8)G_SETTILESIZE; i++) {
    }
    list[i].words.w0 = _SHIFTL(G_SETTILESIZE, 24, 8) | _SHIFTL(2, 12, 12) | _SHIFTL((s32)(120.0f * t + 2.0f), 0, 12);
    return 0;
}

/* Writes the two-halfword command { 0x42, 0 } and returns the position after it. */
s16 *func_150D8590(s16 *cmd, s32 arg1) {
    cmd[0] = 0x42;
    cmd[1] = 0;
    return cmd + 2;
}


/* Spawns impact effects at the actor's feet: a ground effect if the ground below is flagged,
 * then func_150E83AC's effect and a particle burst. */
void func_150D85AC(Actor *actor) {
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
        func_1504715C(&ground, actor);
        {
            SpriteBurst burst;

            {
                union { u32 value; f64 align; } random; // matching: keeps the value in a stack slot, not a temp
                f32 randomScale;

                if (func_15046C80(&probe, 0, pos.y - 200.0f, &ground) != 0 && (ground.flags & 1)) {
                    pos.x = probe.x;
                    pos.y = ground.height;
                    pos.z = probe.z;
                    randomScale = func_150ADA68();
                    random.value = func_150ADA20();
                    func_150E7FEC(randomScale * 109.0f + 140.0f, (u8)((random.value % 86U) + 170),
                                  &ground.tri, &pos, (func_150ADA20() % 251U) + 500, 0, 1, 0, 0, 0, 255, 0);
                }
                func_150E83AC(&pos, (func_150ADA20() % 62U) + 120, 255, 1);
            }
            burst.countBase = 26;
            burst.countRange = 7;
            burst.texture = 0x6C;
            burst.renderMode = 0x5103;
            burst.unk8 = 0x200005;
            burst.unkC = 0;
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
            func_15153634(&burst, 255, 255, 1);
        }
    }
}

struct S150D88AC {
    u8 pad0[0x14];
    u8 *unk14;
    u8 *unk18;
};

/* Sets unk14[9] to whether unk18[0x6F] is clear. */
s32 func_150D88AC(struct S150D88AC *arg0) {
    if (arg0->unk18[0x6F] != 0) {
        arg0->unk14[9] = 0;
    } else {
        arg0->unk14[9] = 1;
    }
    return 1;
}
