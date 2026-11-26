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
u8 D_800D9ED0; // number of bindings in use
TextureBinding D_800D9ED8[8];
s32 D_800D9F58;      // lowest texture id that func_1510D404 must age
s32 D_800D9F5C;      // highest texture id that func_1510D404 must age (-1: none)
u8 D_800D9F60;       // set: func_1510D404 ages textures only while D_800DBDBA counts down
u8 D_800D9F68[7762]; // per texture: use count; D_800BC448 holds each texture's lifetime (see game_139FC0.c)
u8 D_800DBDBA;       // set to 5 when func_1510D0EC loads a texture; func_1510D404 counts it down
s32 D_800DBDBC;      // last texture id freed by func_1510D404 (-1 while it runs, -2 after)

#include "functions.h"
#include "variables.h"


/*
 * Drops one use of texture `index`. When the last use goes, the texture is queued for
 * aging (the D_800D9F58..D_800D9F5C range) with lifetime 2.
 */
void func_1510D720(s32 index) {
    if (D_800BC448[index] != 0) {
        if (D_800D9F68[index] != 0) {
            if (--D_800D9F68[index] == 0) {
                if (index < D_800D9F58) {
                    D_800D9F58 = index;
                }
                if (D_800D9F5C < index) {
                    D_800D9F5C = index;
                }
                func_1510D608(index, 2);
            }
        }
    }
}

/*
 * Drops one use of texture `index` and frees it right away when the last use goes
 * (including its packed buffer when flagged 0x40).
 */
void func_1510D7AC(u32 index) {
    s8 *lifetime;

    lifetime = &D_800BC448[index];
    if (*lifetime != 0) {
        if (D_800D9F68[index] != 0) {
            if (--D_800D9F68[index] == 0) {
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
void func_1510D864(void) {
    D_800D9ED0 = 0;
}

/* Adds a texture binding for `object`, if there is room. */
void func_1510D874(void *object, s32 texture, s32 palette, s32 textureSegment, s32 paletteSegment) {
    TextureBinding *binding;
    u8 count = D_800D9ED0;

    if (D_800D9ED0 < 8) {
        binding = &D_800D9ED8[count];
        binding->object = object;
        binding->texture = texture;
        binding->palette = palette;
        binding->textureSegment = textureSegment;
        binding->paletteSegment = paletteSegment;
        D_800D9ED0 = count + 1;
    }
}

/* Emits the segment commands for every texture binding of `object`. */
Gfx *func_1510D8C0(Gfx *gfx, void *object) {
    s32 i;

    for (i = 0; i < D_800D9ED0; i++) {
        if (object == D_800D9ED8[i].object) {
            TextureBinding *binding = &D_800D9ED8[i];

            gSPSegment(gfx++, binding->textureSegment, binding->texture);
            if (binding->palette != 0) {
                gSPSegment(gfx++, binding->paletteSegment, binding->palette);
            }
        }
    }
    return gfx;
}
