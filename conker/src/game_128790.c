#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Layout of ScreenSprite (game_182140.c), as far as func_150FB2E0 uses it. */
typedef struct {
    /* 0x00 */ u8 pad0[0x22];
    /* 0x22 */ s16 timer;     /* ScreenSpriteInfo.timer, counts down */
    /* 0x24 */ u8 pad24[2];
    /* 0x26 */ s16 fadeStart; /* ScreenSpriteInfo.fadeStart */
    /* 0x28 */ s16 fadeRate;  /* ScreenSpriteInfo.fadeRate */
    /* 0x2A */ u8 pad2A[4];
    /* 0x2E */ u8 alpha;      /* ScreenSpriteInfo.color1.alpha */
    /* 0x2F */ u8 pad2F[0x41];
    /* 0x70 */ s16 duration;  /* the timer's starting value */
} FadeInSprite;

void func_150FB240(u8 *alpha, s16 timer, s16 duration, s16 fadeLength, s16 fadeRate);
s32 func_15145EA4(struct17 **src, struct17 **dst, void *mtx, s32 count);
void func_150FEC28(Actor *actor, u8 bone, struct17 *arg2, struct17 *arg3, struct17 *position, u8 arg5, s32 arg6);
void func_151D4408(struct17 *pos, struct17 *dir, void *mtx, Actor *owner, f32 scale, u8 arg5, s32 arg6);

/* D_8008ACC8 sprite callback: fades the sprite's alpha in over its first fadeStart ticks. */
s32 func_150FB2E0(FadeInSprite *sprite) {
    func_150FB240(&sprite->alpha, sprite->timer, sprite->duration, sprite->fadeStart, sprite->fadeRate);
    return 1;
}

/* Spawns the func_150FEC28 / func_151D4408 effects on bone 14 of `actor`: point0 goes to `position`,
 * point2 to `origin` and point3 to `origin + delta`, all in world space. */
void func_150FB324(Actor *actor, u8 arg1, s32 arg2) {
    struct17 position;
    struct17 origin;
    struct17 delta;
    void *mtx;
    s32 pad; // matching: unused stack slot below mtx (removing it shifts the frame)
    struct17 point0 = { -108.0f, -9.0f, 85.0f };
    struct17 point1 = { -150.0f, -9.0f, 111.0f };
    struct17 point2 = { -32.0f, 4.0f, 40.0f };
    struct17 point3 = { -21.0f, 15.0f, 40.0f };
    struct17 *sources[3];
    struct17 *destinations[3];

    if (actor->unk1D4 == NULL) {
        return;
    }
    sources[0] = &point0;
    sources[1] = &point2;
    sources[2] = &point3;
    destinations[0] = &position;
    destinations[1] = &origin;
    destinations[2] = &delta;
    mtx = (Mtx *)actor->unk1D4 + 14;
    func_15145EA4(sources, destinations, mtx, 3);
    delta.unk0 -= origin.unk0;
    delta.unk4 -= origin.unk4;
    delta.unk8 -= origin.unk8;
    {
        u8 bone = 14;

        func_150FEC28(actor, bone, &point0, &point1, &position, arg1, arg2);
    }
    func_151D4408(&origin, &delta, mtx, actor, 1.0f, arg1, arg2);
}
