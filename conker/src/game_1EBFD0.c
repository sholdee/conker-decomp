#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct S151BEB20 {
    u8 pad0[0x54];
    Vec3f pos;
    u8 pad60[0x7C - 0x60];
    MtxF mtx[2]; /* double-buffered, indexed by D_800BE9C0 */
    u8 padFC[0x120 - 0xFC];
    f32 rotY;
};

/* Rebuilds this frame's matrix: rotation by `rotY`, translation to `pos`, axes scaled by 0.923. */
s32 func_151BEB20(struct S151BEB20 *arg0) {
    f32 scale;

    func_150A8050(arg0->mtx[D_800BE9C0], 0.0f, arg0->rotY, 0.0f);

    scale = 0.9230000377f;
    arg0->mtx[D_800BE9C0][3][0] = arg0->pos.x;
    arg0->mtx[D_800BE9C0][3][1] = arg0->pos.y;
    arg0->mtx[D_800BE9C0][3][2] = arg0->pos.z;

    arg0->mtx[D_800BE9C0][0][0] *= scale;
    arg0->mtx[D_800BE9C0][0][1] *= scale;
    arg0->mtx[D_800BE9C0][0][2] *= scale;
    arg0->mtx[D_800BE9C0][1][0] *= scale;
    arg0->mtx[D_800BE9C0][1][1] *= scale;
    arg0->mtx[D_800BE9C0][1][2] *= scale;
    arg0->mtx[D_800BE9C0][2][0] *= scale;
    arg0->mtx[D_800BE9C0][2][1] *= scale;
    arg0->mtx[D_800BE9C0][2][2] *= scale;

    return 1;
}

s32 func_151BEC94(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    *arg4 = 1;
    return arg0;
}

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 unk10;
} S151BECB8Params;

struct S151BECB8 {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[3];
    /* 0x010 */ u8 unk10;
    /* 0x011 */ u8 pad11[0x37];
    /* 0x048 */ u8 unk48;
    /* 0x049 */ u8 pad49[0xB];
    /* 0x054 */ Vec3f unk54;
    /* 0x060 */ ZoneNode *zone; /* EffectParams.mask: func_1510FD20 at the spawn position */
    /* 0x064 */ u8 pad64[0x98];
    /* 0x0FC */ s32 unkFC;
    /* 0x100 */ u8 pad100[0x1B];
    /* 0x11B */ u8 unk11B;
    /* 0x11C */ u8 pad11C[4];
    /* 0x120 */ S151BECB8Params params;
};


/*
 * Proximity check: for every active object whose bounding radius comes within 43 units of
 * params.pos, plays sound 0x511 and runs func_151BF340/func_151BF0C8/func_151BEEE0.
 * Returns 0 if any object was in range, else 1.
 */
s32 func_151BECB8(S151BECB8 *arg0) {
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
            if (func_151BEE94(obj) != 0) {
                f32 dist;

                params = &arg0->params;
                func_1515C1A0(obj, &objPos, &radius, &height);
                diff.x = objPos.x - params->pos.x;
                diff.y = objPos.y - params->pos.y;
                diff.z = objPos.z - params->pos.z;
                dist = func_15143E64(&diff);
                if (dist - radius < range) {
                    s32 mode;
                    s32 kind; // matching: switching on arg0->unk48 directly changes the frame size

                    func_15085710(obj->playernumber, 3, params->unk10);
                    untouched = 0;
                    func_10010F30(0x511, 0x7D00, 0x40, 0, 0);
                    func_151BF340(arg0->unkC, arg0->unk1);
                    func_151BF0C8(&params->pos);
                    kind = arg0->unk48;
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
                    func_151BEEE0(params->unk0, &arg0->unk54, arg0->unkFC, arg0->unk11B, mode, arg0->unk10 & 0x10,
                                  arg0->zone, arg0->unkC, arg0->unk1);
                }
            }
            obj++;
        } while (obj != (Actor *)&D_800D1548); /* end of movement_obj[26] */
    }
    return untouched;
}

/* An object counts as active when it has a player slot, an interaction state and a valid id. */
s32 func_151BEE94(Actor *arg0) {
    if (arg0->playernumber == 0xFF) {
        return 0;
    }
    if (arg0->type == 0) {
        return 0;
    }
    if (arg0->character_type == 0xFF) {
        return 0;
    }
    return 1;
}
