#include <ultra64.h>

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c): per camera slot, the tint
 * channels texture_updateTint steps and waves (set up by level_initTintWaves from the tables below). */
u16 gTintWavePhases[4][3]; // wave phase (12 bits after >> 4)
u8 gTintWaveSteps[4][3];  // wave phase step
u8 gTintWaveAmps[4][3];  // wave amplitude
u8 gTintLevels[4][3];  // current level
u8 gTintDefaultTargets[3];     // default target level
u8 gTintTargetOverrides[4][3];  // override target level ([0] != 0: use it for this frame)

#include "functions.h"
#include "variables.h"

/* Tint presets, chosen per level by level_initTintWaves: an entry's three indices pick the phase step,
 * amplitude and target level rows. */
const u8 gTintPresets[7][3] = {
    { 0, 0, 0 }, { 0, 1, 1 }, { 0, 2, 2 }, { 0, 3, 3 }, { 0, 4, 4 }, { 0, 5, 5 }, { 0, 6, 6 },
};
const u8 gTintPresetSteps[1][3] = {
    { 11, 23, 34 },
};
const u8 gTintPresetAmps[7][3] = {
    { 20, 40, 20 }, { 20, 20, 40 }, { 0, 16, 40 }, { 20, 20, 20 }, { 10, 10, 10 }, { 0, 0, 0 }, { 20, 20, 10 },
};
const u8 gTintPresetTargets[7][3] = {
    { 80, 127, 235 }, { 80, 235, 127 }, { 0, 71, 70 }, { 59, 102, 12 }, { 70, 85, 45 }, { 128, 128, 128 }, { 90, 67, 31 },
};

extern u8 compressed_ROM_START[];



/* Steps slot's tint channels toward their targets and applies a math_cosBinAngle wave to the prim/env colors. */
void texture_updateTint(s32 slot) {
    f32 wave;
    s32 step;
    s32 target;
    s32 diff;
    s32 override;
    s32 cur;
    s32 amount;
    s32 i;

    if (camera_global_Cameras[slot].SpecialFlags & 1) {
        override = gTintTargetOverrides[slot][0];
        step = D_800BE9E4 * 2;

        for (i = 0; i < 3; i++) {
            cur = gTintLevels[slot][i];
            if (override) {
                target = gTintTargetOverrides[slot][i];
            } else {
                target = gTintDefaultTargets[i];
            }

            diff = target - cur;
            if (diff != 0) {
                amount = (diff < 0) ? -diff : diff;
                if (amount < step) {
                    cur = target;
                } else if (diff < 0) {
                    cur -= step;
                } else {
                    cur += step;
                }
                gTintLevels[slot][i] = cur;
            }

            wave = gTintWaveAmps[slot][i] * math_cosBinAngle(gTintWavePhases[slot][i] >> 4);
            amount = (s32)wave + cur - 127;
            if (amount >= 0) {
                gTintPrimColors[slot][i] += amount;
            } else {
                gTintEnvColors[slot][i] -= amount;
            }

            if (gTintPrimColors[slot][i] >= 128) {
                gTintPrimColors[slot][i] = 127;
            }
            if (gTintEnvColors[slot][i] >= 128) {
                gTintEnvColors[slot][i] = 127;
            }

            gTintWavePhases[slot][i] += gTintWaveSteps[slot][i];
            gTintWavePhases[slot][i] &= 0xFFF;
        }

        gTintTargetOverrides[slot][0] = 0;
    }
}

/* Emits the prim and env colors for slot's current tint. */
Gfx *texture_setTintColors(Gfx *gfx, s32 primAlpha, s32 envAlpha, s32 slot) {
    gDPSetPrimColor(gfx++, 0xF2, 0, gTintPrimColors[slot][0], gTintPrimColors[slot][1], gTintPrimColors[slot][2], primAlpha);
    gDPSetEnvColor(gfx++, gTintEnvColors[slot][0], gTintEnvColors[slot][1], gTintEnvColors[slot][2], envAlpha);
    return gfx;
}


#define GFX_OPCODE(gfx) (*(s8 *)(gfx))

/*
 * Resolves the texture references in a display list: each G_SETTIMG whose image address is a texture
 * index (segment 0) is replaced with the loaded texture's address via texture_getAddress, offset to its
 * palette and given a segment when the flag bits in the address ask for it. With strict set, only
 * commands whose address has no flag bits are touched. When usedOut is non-NULL it receives a newly
 * allocated list (count, then the indices) of the distinct textures referenced.
 * Returns whether every texture loaded.
 */
s32 texture_resolveDl(Gfx *dl, s32 strict, u32 countUse, s32 lifetime, s16 **usedOut) {
    s32 seg;
    s32 i;
    s32 texSize;
    s32 flags;
    s32 dram;
    s32 failed;
    s32 texAddr;
    s32 value; // matching: one local for both the bit mask and the texture index
    s32 count;
    s16 *list;
    s32 bit;
    s32 j;
    Gfx *gfx;
    u8 used[971];

    // matching: these dead statements lower the register priority of dl/usedOut so IDO keeps them on the stack
    if (1) {}
    if (1) {}
    if (&usedOut == NULL) {}
    failed = 0;
    if (usedOut != NULL) {
        for (i = 0; i < 971; i++) {
            used[i] = 0;
        }
        count = 0;
    }
    for (i = 0; GFX_OPCODE(&dl[i]) != (s8)G_ENDDL; i++) {
        gfx = &dl[i];
        if (GFX_OPCODE(&dl[i]) == (s8)G_SETTIMG) {
            dram = gfx->words.w1;
            if ((strict == 0 || !(dram & 0xFF000000)) && (dram & 0xFF000000) < 0x06000000) {
                flags = dram >> 22;
                seg = dram & 0x0F000000;
                dram &= 0xF03FFFFF;
                if (seg == 0) {
                    texAddr = texture_getAddress(dram, &texSize, lifetime, countUse);
                    gfx->words.w1 = texAddr;
                    if (usedOut != NULL) {
                        value = 1 << (dram & 7);
                        if (!(used[dram >> 3] & value)) {
                            used[dram >> 3] |= value;
                            count++;
                        }
                    }
                } else {
                    flags = 0;
                }
                if (texAddr == 0x80000000) {
                    failed = TRUE;
                }
                if (flags != 0) {
                    if (flags & 1) {
                        gfx->words.w1 += texSize - 0x200;
                    } else if (flags & 2) {
                        gfx->words.w1 += texSize - 0x20;
                    }
                    gfx->words.w1 |= (flags & 0x3C) << 22;
                }
            }
        }
    }
    if (usedOut != NULL) {
        list = allocate_memory((count + 1) << 1, 1, 0, 2);
        *usedOut = list;
        if (list != NULL) {
            *list++ = count;
            bit = 1;
            i = 0;
            value = 0;
            j = 0;
            while (i != count) {
                if (used[j] & bit) {
                    list[i] = value;
                    i++;
                }
                if (bit != 0x80) {
                    bit <<= 1;
                } else {
                    bit = 1;
                    j++;
                }
                value++;
            }
        }
    }
    return failed == 0;
}

typedef struct {
    u8 *data;
    s32 size;
} Entry1510D404;

/*
 * Returns the address of texture `index`, loading it on first use: its compressed bytes (size from
 * gTextureRomSizes, located by texture_getRomAddress) are read from ROM and decompressed into a new buffer of the
 * size given by gTextureSizes. Returns 0x80000000 for an invalid index or a failed load. Also widens the
 * gTexAgeFirst..gTexAgeLast range of indices to scan, raises the texture's lifetime (see texture_updateAging)
 * to at least `lifetime` and, when countUse is set, bumps its use count. sizeOut (optional) receives
 * the decompressed size.
 */
s32 texture_getAddress(s32 index, s32 *sizeOut, s32 lifetime, s32 countUse) {
    s32 packedSize;
    s32 romAddr;
    s32 texture;
    s32 odd;
    u8 *packed;
    s32 len;

    if (index < gTexAgeFirst) {
        gTexAgeFirst = index;
    }
    if (gTexAgeLast < index) {
        gTexAgeLast = index;
    }
    if (index >= 0x1E52 || index < 0) {
        return 0x80000000;
    }
    packedSize = gTextureRomSizes[index];
    if (packedSize == 0) {
        gTextureAddrs[index] = 0x80000000;
    } else if (gTextureAddrs[index] == -1) {
        gTexLoadCooldown = 5;
        if (lifetime == 0x3F) {
            lifetime = 0x3E;
        }
        romAddr = texture_getRomAddress(index);
        len = packedSize;
        if (romAddr & 1) {
            romAddr--;
            odd = 1;
        } else {
            odd = 0;
        }
        packedSize = len + odd;
        if (packedSize & 1) {
            len++;
        }
        packed = heap_alloc((len + odd + 0xF) & ~0xF, 1, 2, 1, 2);
        if (packed == NULL) {
            return 0x80000000;
        }
        rom_read(romAddr, packed, (len + odd + 0xF) & ~0xF, 1);
        len = gTextureSizes[index];
        texture = (s32)heap_alloc(len, 1, 1, 0, 2); // the texture cache gTextureAddrs holds addresses as s32
        if (texture == 0) {
            heap_free(packed);
            return 0x80000000;
        }
        func_10006240(packed + odd, texture, D_8003809C);
        heap_free(packed);
        gTextureAddrs[index] = texture;
        gTexUseCounts[index] = 0;
    }
    if (sizeOut != NULL) {
        len = gTextureSizes[index];
        *sizeOut = len;
    }
    len = gTextureAddrs[index];
    if (gTextureLifetimes[index] < lifetime) {
        gTextureLifetimes[index] = lifetime;
    }
    if (countUse != 0) {
        if (gTexUseCounts[index] < 255) {
            gTexUseCounts[index]++;
        }
    }
    // matching: taking texture's address keeps it in memory, as in the original
    if (0) { heap_free(&texture); }
    return len;
}

/* Returns the ROM address of texture `index`'s compressed data: the textures are packed back to back. */
s32 texture_getRomAddress(s32 index) {
    s32 romAddr;
    s32 i;

    romAddr = (s32)compressed_ROM_START;
    for (i = 0; i < index; i++) {
        romAddr += gTextureRomSizes[i];
    }

    return romAddr;
}

/*
 * Ages the textures in the gTexAgeFirst..gTexAgeLast range, freeing those whose lifetime
 * runs out and re-decompressing those flagged 0x40, and narrows the range to the ones still pending.
 */
void texture_updateAging(void) {
    s32 start;
    s32 end;
    s32 i;
    u8 *data;
    s32 size;

    if (gTexAgeLast == -1) {
        return;
    }
    if (gTexLoadCooldown == 0 && gTexAgeThrottle != 0) {
        return;
    }
    if (gTexLoadCooldown != 0) {
        gTexLoadCooldown -= 1;
    }
    start = gTexAgeFirst;
    end = gTexAgeLast;
    gTexAgeFirst = 0xFFFF;
    gTexAgeLast = -1;
    if (start < 0 || end >= 0x1E53) {
        D_8003C8E0 = 0x0C000046;
        debug_fatalTrap();
    }
    gTexLastFreed = -1;
    for (i = start; i <= end; i++) {
        if (gTextureLifetimes[i] != 0) {
            if (gTextureLifetimes[i] < 4) {
                gTextureLifetimes[i]--;
                if (gTextureLifetimes[i] == 0) {
                    gTexLastFreed = i;
                    // matching: the empty compare counts as a loop use of -1, so IDO keeps -1 in a register
                    heap_free((s16 *)gTextureAddrs[i]);
                    if (i == -1) {
                    }
                    gTextureAddrs[i] = -1;
                    continue; // matching: the extra loop block lowers the loop-limit temp's register priority
                } else {
                    if (i < gTexAgeFirst) {
                        gTexAgeFirst = i;
                    }
                    if (gTexAgeLast < i) {
                        gTexAgeLast = i;
                    }
                }
            } else if (gTextureLifetimes[i] & 0x40) {
                data = ((Entry1510D404 *)gTextureAddrs[i])->data;
                size = ((Entry1510D404 *)gTextureAddrs[i])->size;
                func_10006240(data + size, (Entry1510D404 *)gTextureAddrs[i], D_8003809C);
                heap_free(data);
                gTextureLifetimes[i] &= ~0x40;
                if (i < gTexAgeFirst) {
                    gTexAgeFirst = i;
                }
                if (gTexAgeLast < i) {
                    gTexAgeLast = i;
                }
            }
        }
    }
    gTexLastFreed = -2;
}

/* Sets a loaded texture's lifetime, keeping its 0x40 flag; unloaded textures (lifetime 0) are left alone. */
void texture_setLifetime(s32 index, s32 lifetime) {
    s8 *state = &gTextureLifetimes[index];
    s8 prev = *state;

    if (prev != 0) {
        *state = (prev & 0x40) | lifetime;
    }
}
