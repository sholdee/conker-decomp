#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/**
 * Scans a display list up to its G_ENDDL for the first two G_SETTIMG commands.
 * Returns the index of the first one and stores the index of the second in *second.
 */
s32 prop_findSetTimg(Gfx *dl, s32 *second) {
    s32 count;
    s32 found[2];
    s32 i;

    found[0] = 0;
    found[1] = 0;
    count = 0;
    for (i = 0; *(s8 *)&dl[i] != (s8)G_ENDDL && count < 2; i++) {
        // matching: IDO emits the target branch layout only for a one-case switch, not an if
        switch (*(s8 *)&dl[i]) {
        case (s8)G_SETTIMG:
            found[count++] = i;
            break;
        }
    }
    *second = found[1];
    return found[0];
}

/* Behaviour scratch state (WorldObject.touch) of an object whose display list textures prop_animateTexture animates. */
typedef struct {
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 texIndices;
    /* 0x84 */ s32 animState;
} TexAnimWork;

#define WORK(obj) ((TexAnimWork *)&(obj)->touch)

/* One texture animation: a sequence of texture assets, optionally restricted to one level (levelId, -1 for every level). */
typedef struct {
    s32 *frames;
    u8 frameCount;
    u8 kind;
    s8 levelId;
    u8 ticksPerFrame;
    u8 pingPong;
    u8 pad9[3];
} TexAnim;

/* Texture asset index lists (extracted data). */

/* The texture animations of the world objects (.data, after game/Prop/PropMover/propmover.c's). */
static TexAnim gTexAnims[24] = {
    { D_8009177C, 15, 11, LEVEL_W1_TRAINING, 1, 1 },
    { D_8009177C, 15, 10, LEVEL_W1_TRAINING, 1, 1 },
    { D_8009177C, 15, 12, LEVEL_W1_TRAINING, 1, 1 },
    { D_8009177C, 15, 7, LEVEL_THE_WINDMILL, 1, 1 },
    { D_8009177C, 15, 9, LEVEL_THE_WINDMILL, 1, 1 },
    { D_8009177C, 15, 12, LEVEL_THE_WINDMILL, 1, 1 },
    { D_8009177C, 15, 13, LEVEL_THE_WINDMILL, 1, 1 },
    { D_8009177C, 15, 7, LEVEL_W1_WINDMILL_EXPLODED, 1, 1 },
    { D_8009177C, 15, 9, LEVEL_W1_WINDMILL_EXPLODED, 1, 1 },
    { D_8009177C, 15, 11, LEVEL_W1_WINDMILL_EXPLODED, 1, 1 },
    { D_8009177C, 15, 7, LEVEL_W1_DOG_FISH, 1, 1 },
    { D_8009177C, 15, 4, LEVEL_W1_DOG_FISH, 1, 1 },
    { D_8009177C, 15, 5, LEVEL_W1_DOG_FISH, 1, 1 },
    { D_8009177C, 15, 6, LEVEL_W1_DOG_FISH, 1, 1 },
    { D_8009177C, 15, 9, LEVEL_W1_DOG_FISH, 2, 1 },
    { D_8009177C, 15, 10, LEVEL_W1_DOG_FISH, 1, 1 },
    { D_800917D8, 3, 9, LEVEL_W1_DOG_FISH_NURSERY_PART1, 6, 0 },
    { D_800917D8, 3, 9, LEVEL_W1_INSIDE_SAFE, 6, 0 },
    { D_8009177C, 15, 9, LEVEL_W3_HORROR_INTRO, 1, 1 },
    { D_8009177C, 15, 10, LEVEL_W3_HORROR_INTRO, 1, 1 },
    { D_8009177C, 15, 11, LEVEL_W3_HORROR_INTRO, 1, 1 },
    { D_8009177C, 15, 12, LEVEL_W3_HORROR_INTRO, 1, 1 },
    { D_8009177C, 15, 13, LEVEL_W3_HORROR_INTRO, 1, 1 },
    { D_80090338, 3, 255, LEVEL_W1_DOG_FISH_NURSERY_PART1, 3, 0 },
};

/**
 * Advances the texture animation that matches obj's kind in the current level and loads the
 * current frame's texture into obj's display list.
 * texIndices caches the G_SETTIMG indices found by prop_findSetTimg (second << 16 | first);
 * animState packs the play direction and tick counter (direction << 16 | tick).
 */
void prop_animateTexture(WorldObject *obj, s32 *texIndices, s32 *animState) {
    s32 pad; // matching: unused stack slot
    s32 size;
    s32 i;
    TexAnim *anim;
    s32 kind;
    s32 tick;
    s32 direction;
    s32 second;
    s32 flags;
    s32 addr;
    s32 paletteAddr;
    s32 value;

    anim = NULL;
    for (i = 0; i < 24; i++) {
        kind = gTexAnims[i].kind;
        if (gTexAnims[i].levelId != -1) {
            if (level != gTexAnims[i].levelId) {
                continue;
            }
            if (kind != 0xFF) {
                kind |= 0x8000;
            }
        }
        if (kind == 0xFF || kind == obj->modelId) {
            anim = &gTexAnims[i];
            break;
        }
    }
    if (anim != NULL) {
        if (*texIndices == 0) {
            value = prop_findSetTimg(obj->displayList, &second);
            *texIndices = (second << 16) | value;
        } else {
            second = (u16)(*texIndices >> 16);
        }
        direction = (s16)((*animState >> 16) & 0xFFFF);
        tick = *animState & 0xFFFF;
        if (direction == 0) {
            direction = 1;
        }
        tick += direction;
        if (anim->pingPong) {
            if (tick < 0) {
                direction = 1;
                tick = anim->ticksPerFrame;
            } else if (tick >= anim->frameCount * anim->ticksPerFrame) {
                direction = -1;
                tick = (anim->frameCount - 1) * anim->ticksPerFrame;
            }
        } else if (tick >= anim->frameCount * anim->ticksPerFrame) {
            tick -= anim->frameCount * anim->ticksPerFrame;
            direction = 1;
        }
        value = anim->frames[tick / anim->ticksPerFrame];
        paletteAddr = 0;
        if (level == LEVEL_THE_WINDMILL || level == LEVEL_W1_WINDMILL_EXPLODED) {
            flags = 3;
        } else {
            flags = 0x3E;
        }
        addr = texture_getAddress(value, &size, flags, 0);
        if (second != 0) {
            paletteAddr = size + addr - 0x20;
        }
        texture_addBinding(obj, addr, paletteAddr, 4, 5);
        *animState = (direction << 16) | tick;
    }
}

/* Runs prop_registerTextureScroll while unk3C is set, then advances the texture animation. */
void prop_scrollAndAnimateTexture(WorldObject *obj) {
    if (obj->moveData.word != 0) {
        prop_registerTextureScroll(obj);
    }
    prop_animateTexture(obj, &WORK(obj)->texIndices, &WORK(obj)->animState);
}

/* When gCutsceneStates is 1 and cutscene_isActorIndexBound accepts this object's index, clears unk7C and unk18;
 * then runs prop_bob and advances the texture animation. */
void prop_bobAndAnimateTexture(WorldObject *obj) {
    if (gCutsceneStates[0] == 1 && cutscene_isActorIndexBound(obj - db_Object_array, 0) != 0) {
        WORK(obj)->unk7C = 0;
        obj->yOffset = 0.0f;
    }
    prop_bob(obj);
    prop_animateTexture(obj, &WORK(obj)->texIndices, &WORK(obj)->animState);
}
