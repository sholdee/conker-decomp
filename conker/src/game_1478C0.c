#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/**
 * Scans a display list up to its G_ENDDL for the first two G_SETTIMG commands.
 * Returns the index of the first one and stores the index of the second in *second.
 */
s32 func_1511A410(Gfx *dl, s32 *second) {
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

/* Behaviour scratch state (WorldObject.touch) of an object whose display list textures func_1511A494 animates. */
typedef struct {
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 texIndices;
    /* 0x84 */ s32 animState;
} TexAnimWork;

#define WORK(obj) ((TexAnimWork *)&(obj)->touch)

/* One texture animation: a sequence of texture assets, optionally restricted to one world. */
typedef struct {
    s32 *frames;
    u8 frameCount;
    u8 kind;
    s8 world;
    u8 ticksPerFrame;
    u8 pingPong;
    u8 pad9[3];
} TexAnim;

/* Texture asset index lists (extracted data). */

/* The texture animations of the world objects (.data, after game_143DE0.c's). */
static TexAnim D_80089324[24] = {
    { D_8009177C, 15, 11, 41, 1, 1 },
    { D_8009177C, 15, 10, 41, 1, 1 },
    { D_8009177C, 15, 12, 41, 1, 1 },
    { D_8009177C, 15, 7, 6, 1, 1 },
    { D_8009177C, 15, 9, 6, 1, 1 },
    { D_8009177C, 15, 12, 6, 1, 1 },
    { D_8009177C, 15, 13, 6, 1, 1 },
    { D_8009177C, 15, 7, 59, 1, 1 },
    { D_8009177C, 15, 9, 59, 1, 1 },
    { D_8009177C, 15, 11, 59, 1, 1 },
    { D_8009177C, 15, 7, 7, 1, 1 },
    { D_8009177C, 15, 4, 7, 1, 1 },
    { D_8009177C, 15, 5, 7, 1, 1 },
    { D_8009177C, 15, 6, 7, 1, 1 },
    { D_8009177C, 15, 9, 7, 2, 1 },
    { D_8009177C, 15, 10, 7, 1, 1 },
    { D_800917D8, 3, 9, 23, 6, 0 },
    { D_800917D8, 3, 9, 18, 6, 0 },
    { D_8009177C, 15, 9, 65, 1, 1 },
    { D_8009177C, 15, 10, 65, 1, 1 },
    { D_8009177C, 15, 11, 65, 1, 1 },
    { D_8009177C, 15, 12, 65, 1, 1 },
    { D_8009177C, 15, 13, 65, 1, 1 },
    { D_80090338, 3, 255, 23, 3, 0 },
};

/**
 * Advances the texture animation that matches obj's kind in the current world and loads the
 * current frame's texture into obj's display list.
 * texIndices caches the G_SETTIMG indices found by func_1511A410 (second << 16 | first);
 * animState packs the play direction and tick counter (direction << 16 | tick).
 */
void func_1511A494(WorldObject *obj, s32 *texIndices, s32 *animState) {
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
        kind = D_80089324[i].kind;
        if (D_80089324[i].world != -1) {
            if (level != D_80089324[i].world) {
                continue;
            }
            if (kind != 0xFF) {
                kind |= 0x8000;
            }
        }
        if (kind == 0xFF || kind == obj->modelId) {
            anim = &D_80089324[i];
            break;
        }
    }
    if (anim != NULL) {
        if (*texIndices == 0) {
            value = func_1511A410(obj->displayList, &second);
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
        if (level == 6 || level == 0x3B) {
            flags = 3;
        } else {
            flags = 0x3E;
        }
        addr = func_1510D0EC(value, &size, flags, 0);
        if (second != 0) {
            paletteAddr = size + addr - 0x20;
        }
        func_1510D874(obj, addr, paletteAddr, 4, 5);
        *animState = (direction << 16) | tick;
    }
}

/* Runs func_15116110 while unk3C is set, then advances the texture animation. */
void func_1511A6FC(WorldObject *obj) {
    if (obj->moveData.word != 0) {
        func_15116110(obj);
    }
    func_1511A494(obj, &WORK(obj)->texIndices, &WORK(obj)->animState);
}

/* When D_800C35EA is 1 and func_15022B08 accepts this object's index, clears unk7C and unk18;
 * then runs func_151162D4 and advances the texture animation. */
void func_1511A738(WorldObject *obj) {
    if (D_800C35EA[0] == 1 && func_15022B08(obj - db_Object_array, 0) != 0) {
        WORK(obj)->unk7C = 0;
        obj->yOffset = 0.0f;
    }
    func_151162D4(obj);
    func_1511A494(obj, &WORK(obj)->texIndices, &WORK(obj)->animState);
}
