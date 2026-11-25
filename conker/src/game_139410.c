#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The opcode byte of a display-list command, read signed (G_ENDDL is -0x21). */
#define GFX_OP(g) (*(s8 *)(g))

/*
 * Finds the vertices that `dl` draws with texture `tex` (its `skip`-th G_SETTIMG of D_800B0E58[tex]):
 * stores the first one in *outVtx and returns how many there are (0 if the texture is never loaded).
 * Opcodes 0x10-0x1F are Conker's four-triangle command (12 five-bit indices).
 */
s32 func_1510BF60(Gfx *dl, s32 tex, Vtx **outVtx, s32 skip) {
    s32 count;
    s32 unused;
    s32 state;
    s32 maxIdx;
    Vtx *base;
    s32 depth;
    s32 rebase;
    s32 pad; // matching: keeps the stack slot that `found` had at function scope
    s32 idx[12];
    s32 op;
    s32 i;
    s32 n;
    s32 minIdx;

    func_1510D0EC(tex, &unused, 3, 0);
    *outVtx = NULL;
    state = 0;
    maxIdx = 0;
    depth = 0;
    rebase = 0;
    for (;;) {
        op = GFX_OP(dl);
        if (state == 0) {
            if (op == (s8)G_ENDDL) {
                return 0;
            }
            if (op == (s8)G_SETTIMG && dl->words.w1 == D_800B0E58[tex] && skip-- <= 0) {
                state++;
                dl++;
                continue;
            }
        } else if (state == 1) {
            /* Find the G_VTX for this texture: forward, or back from its first triangle. */
            s32 found; // matching: declared in this block for its -g3 scope marker
            found = 0;
            while (op != G_TRI1 && op != G_TRI2 && (op >> 4) != 1) {
                if (op == G_VTX) {
                    found = 1;
                    break;
                }
                op = GFX_OP(++dl);
            }
            state++;
            if (found == 0) {
                rebase = 1;
                while (op != G_VTX) {
                    if (op == (s8)G_SETTIMG) {
                        depth--;
                    }
                    op = GFX_OP(--dl);
                }
            }
            dl--;
            continue;
        } else if (state == 2) {
            if (op == (s8)G_ENDDL) {
                break;
            }
            if (op == (s8)G_SETTIMG) {
                depth++;
                if (depth > 0) {
                    break;
                }
            } else if (op == G_VTX) {
                maxIdx = 0;
                base = (Vtx *)dl->words.w1;
                if (*outVtx == NULL || base < *outVtx) {
                    *outVtx = base;
                    dl++;
                    continue;
                }
            } else if (op == G_TRI1 || op == G_TRI2 || (op >> 4) == 1) {
                /* Collect the triangle's vertex indices. */
                if (op == G_TRI1) {
                    n = 3;
                    idx[0] = (dl->words.w0 >> 16) & 0xFF;
                    idx[1] = (dl->words.w0 >> 8) & 0xFF;
                    idx[2] = dl->words.w0 & 0xFF;
                    for (i = 0; i < n; i++) {
                        idx[i] >>= 1;
                    }
                } else if (op == G_TRI2) {
                    n = 6;
                    idx[0] = (dl->words.w0 >> 16) & 0xFF;
                    idx[1] = (dl->words.w0 >> 8) & 0xFF;
                    idx[2] = dl->words.w0 & 0xFF;
                    idx[3] = (dl->words.w1 >> 16) & 0xFF;
                    idx[4] = (dl->words.w1 >> 8) & 0xFF;
                    idx[5] = dl->words.w1 & 0xFF;
                    for (i = 0; i < n; i++) {
                        idx[i] >>= 1;
                    }
                } else {
                    n = 12;
                    idx[0] = (dl->words.w1 >> 25) & 0x1F;
                    idx[1] = (dl->words.w1 >> 20) & 0x1F;
                    idx[2] = (dl->words.w1 >> 15) & 0x1F;
                    idx[3] = (dl->words.w1 >> 10) & 0x1F;
                    idx[4] = (dl->words.w1 >> 5) & 0x1F;
                    idx[5] = dl->words.w1 & 0x1F;
                    idx[6] = (dl->words.w0 >> 10) & 0x1F;
                    idx[7] = (dl->words.w0 >> 5) & 0x1F;
                    idx[8] = dl->words.w0 & 0x1F;
                    idx[9] = (dl->words.w0 >> 23) & 0x1F;
                    idx[10] = (dl->words.w0 >> 18) & 0x1F;
                    idx[11] = ((dl->words.w1 >> 30) & 3) + (((dl->words.w0 >> 15) & 7) << 2);
                }
                for (i = 0; i < n; i++) {
                    if (maxIdx < idx[i]) {
                        maxIdx = idx[i];
                    }
                }
                if (depth == 0) {
                    minIdx = idx[0];
                    if (rebase) {
                        for (i = 1; i < n; i++) {
                            if (idx[i] < minIdx) {
                                minIdx = idx[i];
                            }
                        }
                        rebase = 0;
                        minIdx &= 0xFF; // matching: puts the andi before the load of *outVtx
                        *outVtx += minIdx;
                    }
                }
                { // matching: this empty block's -g3 scope marker steers as1 scheduling
                    s32 pad;
                }
            }
        }
        dl++;
    }
    count = ((maxIdx + 1) + base) - *outVtx; // matching: ugen evaluates the index first
    return count;
}

extern UvWobble *D_800D9E60[];

/* Registers a UV wobble for mesh 0 vertices drawn with texture `tex` and mesh 1 ones drawn with D_800917B8. */
UvWobble *func_1510C4AC(s32 tex, s32 disabled, s32 rate, s32 amplitude) {
    Vtx *vtx;
    s32 count;
    s32 i;
    UvWobble *wob;

    if (D_800D9E64 > 0) { /* only one wobble fits */
        return NULL;
    }

    wob = allocate_memory(sizeof(UvWobble), 1, 0, 0);
    if (wob == NULL) {
        return NULL;
    }

    D_800D9E60[D_800D9E64++] = wob;
    wob->disabled = disabled;
    wob->primary = NULL;
    wob->secondary = NULL;
    wob->phase = 0;
    wob->rate = rate;
    wob->amplitude = amplitude;

    count = func_1510BF60(D_800B0E00[0], tex, &vtx, 0);
    if (count != 0) {
        wob->primaryCount = count;
        wob->primaryIndex = vtx - D_800B0E10[0];
        wob->primary = allocate_memory(count * sizeof(TexCoord), 1, 0, 0);
        for (i = 0; i < count; i++) {
            wob->primary[i].s = vtx[i].v.tc[0];
            wob->primary[i].t = vtx[i].v.tc[1];
        }
    }

    count = func_1510BF60(D_800B0E00[1], D_800917B8, &vtx, 0);
    if (count != 0) {
        wob->secondaryCount = count;
        wob->secondaryIndex = vtx - D_800B0E10[1];
        wob->secondary = allocate_memory(count * sizeof(TexCoord), 1, 0, 0);
        for (i = 0; i < count; i++) {
            wob->secondary[i].s = vtx[i].v.tc[0];
            wob->secondary[i].t = vtx[i].v.tc[1];
        }
    }

    return wob;
}

/* Advances every enabled UV wobble and rewrites its vertices' texture coordinates. */
void func_1510C8A8(void) {
    s32 i;
    s32 j;
    UvWobble *wob;
    Vtx *vtx;
    f32 angle;
    s16 offS;
    s16 offT;

    for (i = 0; i < D_800D9E64; i++) {
        wob = D_800D9E60[i];
        if (wob->disabled == 0) {
            if (wob->primary != NULL) {
                f32 sn;
                f32 cs;

                wob->phase += wob->rate * D_800BE9E4;
                vtx = D_800B0E10[0]; // matching: &D_800B0E10[0][index] schedules differently
                vtx += wob->primaryIndex;
                angle = wob->phase * 9.587380191e-05f; /* 2pi / 0x10000 */
                sn = sinf(angle);
                cs = cosf(angle);
                offS = wob->amplitude * sn;
                offT = wob->amplitude * cs;
                for (j = 0; j < wob->primaryCount; j++) {
                    vtx[j].v.tc[0] = wob->primary[j].s + offS;
                    vtx[j].v.tc[1] = wob->primary[j].t + offT;
                }
            }
            /* Mesh 1 reuses mesh 0's offsets (unset if it has no mesh-0 vertices). */
            if (wob->secondary != NULL) {
                vtx = D_800B0E10[1];
                vtx += wob->secondaryIndex;
                for (j = 0; j < wob->secondaryCount; j++) {
                    vtx[j].v.tc[0] = wob->secondary[j].s + offS;
                    vtx[j].v.tc[1] = wob->secondary[j].t + offT;
                }
            }
        }
    }
}
