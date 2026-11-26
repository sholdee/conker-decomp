#include <ultra64.h>

/* An object's texture and palette addresses, bound to RSP segments when it is drawn. */
typedef struct {
    /* 0x0 */ void *object;
    /* 0x4 */ s32 texture;
    /* 0x8 */ s32 palette; // 0 when there is none
    /* 0xC */ u8 textureSegment;
    /* 0xD */ u8 paletteSegment;
} TextureBinding; // size 0x10

/* This file's .bss, defined before the headers declare it (see init_50A0.c). */
u8 gTexBindingCount; // number of bindings in use
TextureBinding gTexBindings[8];
s32 gTexAgeFirst;      // lowest texture id that texture_updateAging must age
s32 gTexAgeLast;      // highest texture id that texture_updateAging must age (-1: none)
u8 gTexAgeThrottle;       // set: texture_updateAging ages textures only while gTexLoadCooldown counts down
u8 gTexUseCounts[7762]; // per texture: use count; D_800BC448 holds each texture's lifetime (see game/Texture/TextureLoad/textureload.c)
u8 gTexLoadCooldown;       // set to 5 when texture_getAddress loads a texture; texture_updateAging counts it down
s32 gTexLastFreed;      // last texture id freed by texture_updateAging (-1 while it runs, -2 after)

#include "functions.h"
#include "variables.h"


/*
 * Drops one use of texture `index`. When the last use goes, the texture is queued for
 * aging (the gTexAgeFirst..gTexAgeLast range) with lifetime 2.
 */
void texture_dropUse(s32 index) {
    if (D_800BC448[index] != 0) {
        if (gTexUseCounts[index] != 0) {
            if (--gTexUseCounts[index] == 0) {
                if (index < gTexAgeFirst) {
                    gTexAgeFirst = index;
                }
                if (gTexAgeLast < index) {
                    gTexAgeLast = index;
                }
                texture_setLifetime(index, 2);
            }
        }
    }
}

/*
 * Drops one use of texture `index` and frees it right away when the last use goes
 * (including its packed buffer when flagged 0x40).
 */
void texture_dropUseAndFree(u32 index) {
    s8 *lifetime;

    lifetime = &D_800BC448[index];
    if (*lifetime != 0) {
        if (gTexUseCounts[index] != 0) {
            if (--gTexUseCounts[index] == 0) {
                if (*lifetime & 0x40) {
                    func_10004074(*(s16 **)D_800B0E58[index]);
                }
                func_10004074((s16 *)D_800B0E58[index]);
                D_800B0E58[index] = -1;
                *lifetime = 0;
            }
        }
    }
}

/* Clears all texture bindings. */
void texture_clearBindings(void) {
    gTexBindingCount = 0;
}

/* Adds a texture binding for `object`, if there is room. */
void texture_addBinding(void *object, s32 texture, s32 palette, s32 textureSegment, s32 paletteSegment) {
    TextureBinding *binding;
    u8 count = gTexBindingCount;

    if (gTexBindingCount < 8) {
        binding = &gTexBindings[count];
        binding->object = object;
        binding->texture = texture;
        binding->palette = palette;
        binding->textureSegment = textureSegment;
        binding->paletteSegment = paletteSegment;
        gTexBindingCount = count + 1;
    }
}

/* Emits the segment commands for every texture binding of `object`. */
Gfx *texture_emitBindings(Gfx *gfx, void *object) {
    s32 i;

    for (i = 0; i < gTexBindingCount; i++) {
        if (object == gTexBindings[i].object) {
            TextureBinding *binding = &gTexBindings[i];

            gSPSegment(gfx++, binding->textureSegment, binding->texture);
            if (binding->palette != 0) {
                gSPSegment(gfx++, binding->paletteSegment, binding->palette);
            }
        }
    }
    return gfx;
}
