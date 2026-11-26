#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 compressed_ROM_START[];



/* Steps slot's tint channels toward their targets and applies a func_150489B0 wave to the prim/env colors. */
void func_1510CB10(s32 slot) {
    f32 wave;
    s32 step;
    s32 target;
    s32 diff;
    s32 override;
    s32 cur;
    s32 amount;
    s32 i;

    if (camera_global_Cameras[slot].SpecialFlags & 1) {
        override = D_800D9EB8[slot][0];
        step = D_800BE9E4 * 2;

        for (i = 0; i < 3; i++) {
            cur = D_800D9EA8[slot][i];
            if (override) {
                target = D_800D9EB8[slot][i];
            } else {
                target = D_800D9EB4[i];
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
                D_800D9EA8[slot][i] = cur;
            }

            wave = D_800D9E98[slot][i] * func_150489B0(D_800D9E70[slot][i] >> 4);
            amount = (s32)wave + cur - 127;
            if (amount >= 0) {
                D_800D9B68[slot][i] += amount;
            } else {
                D_800D9B78[slot][i] -= amount;
            }

            if (D_800D9B68[slot][i] >= 128) {
                D_800D9B68[slot][i] = 127;
            }
            if (D_800D9B78[slot][i] >= 128) {
                D_800D9B78[slot][i] = 127;
            }

            D_800D9E70[slot][i] += D_800D9E88[slot][i];
            D_800D9E70[slot][i] &= 0xFFF;
        }

        D_800D9EB8[slot][0] = 0;
    }
}

/* Emits the prim and env colors for slot's current tint. */
Gfx *func_1510CDB8(Gfx *gfx, s32 primAlpha, s32 envAlpha, s32 slot) {
    gDPSetPrimColor(gfx++, 0xF2, 0, D_800D9B68[slot][0], D_800D9B68[slot][1], D_800D9B68[slot][2], primAlpha);
    gDPSetEnvColor(gfx++, D_800D9B78[slot][0], D_800D9B78[slot][1], D_800D9B78[slot][2], envAlpha);
    return gfx;
}


#define GFX_OPCODE(gfx) (*(s8 *)(gfx))

/*
 * Resolves the texture references in a display list: each G_SETTIMG whose image address is a texture
 * index (segment 0) is replaced with the loaded texture's address via func_1510D0EC, offset to its
 * palette and given a segment when the flag bits in the address ask for it. With strict set, only
 * commands whose address has no flag bits are touched. When usedOut is non-NULL it receives a newly
 * allocated list (count, then the indices) of the distinct textures referenced.
 * Returns whether every texture loaded.
 */
s32 func_1510CE60(Gfx *dl, s32 strict, u32 countUse, s32 lifetime, s16 **usedOut) {
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
            if ((strict == 0 || !(dram & 0xFF000000)) && (u32)(dram & 0xFF000000) < 0x06000000) {
                flags = dram >> 22;
                seg = dram & 0x0F000000;
                dram &= 0xF03FFFFF;
                if (seg == 0) {
                    texAddr = func_1510D0EC(dram, &texSize, lifetime, countUse);
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

extern u8 D_800DBDBA;
// Last texture id freed by func_1510D404. A common (not a static): it sits in the bss right after
// D_800DBDBA, which no 16-byte aligned object .bss can start at.
extern s32 D_800DBDBC;

/* Per-texture lifetime in frames (1-3 count down; 4+ stays loaded) plus the 0x40 re-decompress flag. */

/*
 * Returns the address of texture `index`, loading it on first use: its compressed bytes (size from
 * D_80091D20, located by func_1510D374) are read from ROM and decompressed into a new buffer of the
 * size given by D_800B87A0. Returns 0x80000000 for an invalid index or a failed load. Also widens the
 * D_800D9F58..D_800D9F5C range of indices to scan, raises the texture's lifetime (see func_1510D404)
 * to at least `lifetime` and, when countUse is set, bumps its use count. sizeOut (optional) receives
 * the decompressed size.
 */
s32 func_1510D0EC(s32 index, s32 *sizeOut, s32 lifetime, s32 countUse) {
    s32 packedSize;
    s32 romAddr;
    s32 texture;
    s32 odd;
    s32 packed;
    s32 len;

    if (index < D_800D9F58) {
        D_800D9F58 = index;
    }
    if (D_800D9F5C < index) {
        D_800D9F5C = index;
    }
    if (index >= 0x1E52 || index < 0) {
        return 0x80000000;
    }
    packedSize = D_80091D20[index];
    if (packedSize == 0) {
        D_800B0E58[index] = 0x80000000;
    } else if (D_800B0E58[index] == -1) {
        D_800DBDBA = 5;
        if (lifetime == 0x3F) {
            lifetime = 0x3E;
        }
        romAddr = func_1510D374(index);
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
        packed = func_10003C6C((len + odd + 0xF) & ~0xF, 1, 2, 1, 2);
        if (packed == 0) {
            return 0x80000000;
        }
        func_10004514(romAddr, (void *)packed, (len + odd + 0xF) & ~0xF, 1);
        len = D_800B87A0[index];
        texture = func_10003C6C(len, 1, 1, 0, 2);
        if (texture == 0) {
            func_10004074((s16 *)packed);
            return 0x80000000;
        }
        func_10006240((u8 *)(packed + odd), (Entry1510D404 *)texture, D_8003809C);
        func_10004074((s16 *)packed);
        D_800B0E58[index] = texture;
        D_800D9F68[index] = 0;
    }
    if (sizeOut != NULL) {
        len = D_800B87A0[index];
        *sizeOut = len;
    }
    len = D_800B0E58[index];
    if (D_800BC448[index] < lifetime) {
        D_800BC448[index] = lifetime;
    }
    if (countUse != 0) {
        if (D_800D9F68[index] < 255) {
            D_800D9F68[index]++;
        }
    }
    // matching: taking texture's address keeps it in memory, as in the original
    if (0) { func_10004074((s16 *)&texture); }
    return len;
}

/* Returns the ROM address of texture `index`'s compressed data: the textures are packed back to back. */
s32 func_1510D374(s32 index) {
    s32 romAddr;
    s32 i;

    romAddr = (s32)compressed_ROM_START;
    for (i = 0; i < index; i++) {
        romAddr += D_80091D20[i];
    }

    return romAddr;
}

/*
 * Ages the textures in the D_800D9F58..D_800D9F5C range, freeing those whose lifetime
 * runs out and re-decompressing those flagged 0x40, and narrows the range to the ones still pending.
 */
void func_1510D404(void) {
    s32 start;
    s32 end;
    s32 i;
    u8 *data;
    s32 size;

    if (D_800D9F5C == -1) {
        return;
    }
    if (D_800DBDBA == 0 && D_800D9F60 != 0) {
        return;
    }
    if (D_800DBDBA != 0) {
        D_800DBDBA -= 1;
    }
    start = D_800D9F58;
    end = D_800D9F5C;
    D_800D9F58 = 0xFFFF;
    D_800D9F5C = -1;
    if (start < 0 || end >= 0x1E53) {
        D_8003C8E0 = 0x0C000046;
        func_150AD770();
    }
    D_800DBDBC = -1;
    for (i = start; i <= end; i++) {
        if (D_800BC448[i] != 0) {
            if (D_800BC448[i] < 4) {
                D_800BC448[i]--;
                if (D_800BC448[i] == 0) {
                    D_800DBDBC = i;
                    // matching: the empty compare counts as a loop use of -1, so IDO keeps -1 in a register
                    func_10004074((s16 *)D_800B0E58[i]);
                    if (i == -1) {
                    }
                    D_800B0E58[i] = -1;
                    continue; // matching: the extra loop block lowers the loop-limit temp's register priority
                } else {
                    if (i < D_800D9F58) {
                        D_800D9F58 = i;
                    }
                    if (D_800D9F5C < i) {
                        D_800D9F5C = i;
                    }
                }
            } else if (D_800BC448[i] & 0x40) {
                data = ((Entry1510D404 *)D_800B0E58[i])->data;
                size = ((Entry1510D404 *)D_800B0E58[i])->size;
                func_10006240(data + size, (Entry1510D404 *)D_800B0E58[i], D_8003809C);
                func_10004074((s16 *)data);
                D_800BC448[i] &= ~0x40;
                if (i < D_800D9F58) {
                    D_800D9F58 = i;
                }
                if (D_800D9F5C < i) {
                    D_800D9F5C = i;
                }
            }
        }
    }
    D_800DBDBC = -2;
}

/* Sets a loaded texture's lifetime, keeping its 0x40 flag; unloaded textures (lifetime 0) are left alone. */
void func_1510D608(s32 index, s32 lifetime) {
    s8 *state = &D_800BC448[index];
    s8 prev = *state;

    if (prev != 0) {
        *state = (prev & 0x40) | lifetime;
    }
}
