#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 func_1510D0EC(s32, s32 *, s32, s32);

#define OP(p) (*(s8 *)(p))
#define W0(p) (*(u32 *)(p))
#define W1(p) (*(u32 *)((p) + 4))

s32 func_1510BF60(s32 dl, s32 segAddr, Vtx **outVtx, s32 skip) {
    s32 count;
    s32 out;
    s32 state;
    s32 maxIdx;
    Vtx *base;
    s32 depth;
    s32 first;
    s32 pad;
    s32 idx[12];
    s32 op;
    s32 i;
    s32 n;
    s32 minIdx;


    func_1510D0EC(segAddr, &out, 3, 0);
    *outVtx = NULL;
    state = 0;
    maxIdx = 0;
    depth = 0;
    first = 0;
    while (1) {
        op = OP(dl);
        if (state == 0) {
            if (op == -0x21) {
                return 0;
            }
            if (op == -3 && W1(dl) == D_800B0E58[segAddr] && skip-- <= 0) {
                state++;
                dl += 8;
                continue;
            }
        } else if (state == 1) {
            s32 found;
            found = 0;
            while (op != 5 && op != 6 && (op >> 4) != 1) {
                if (op == 1) {
                    found = 1;
                    break;
                }
                op = OP(dl + 8);
                dl += 8;
            }
            state++;
            if (found == 0) {
                first = 1;
                while (op != 1) {
                    if (op == -3) {
                        depth--;
                    }
                    op = OP(dl - 8);
                    dl -= 8;
                }
            }
            dl -= 8;
            continue;
        } else if (state == 2) {
            if (op == -0x21) {
                break;
            }
            if (op == -3) {
                depth++;
                if (depth > 0) {
                    break;
                }
            } else if (op == 1) {
                maxIdx = 0;
                base = (Vtx *)W1(dl);
                if (*outVtx == NULL || base < *outVtx) {
                    *outVtx = base;
                    dl += 8;
                    continue;
                }
            } else if (op == 5 || op == 6 || (op >> 4) == 1) {
                if (op == 5) {
                    n = 3;
                    idx[0] = (W0(dl) >> 16) & 0xFF;
                    idx[1] = (W0(dl) >> 8) & 0xFF;
                    idx[2] = W0(dl) & 0xFF;
                    for (i = 0; i < n; i++) {
                        idx[i] >>= 1;
                    }
                } else if (op == 6) {
                    n = 6;
                    idx[0] = (W0(dl) >> 16) & 0xFF;
                    idx[1] = (W0(dl) >> 8) & 0xFF;
                    idx[2] = W0(dl) & 0xFF;
                    idx[3] = (W1(dl) >> 16) & 0xFF;
                    idx[4] = (W1(dl) >> 8) & 0xFF;
                    idx[5] = W1(dl) & 0xFF;
                    for (i = 0; i < n; i++) {
                        idx[i] >>= 1;
                    }
                } else {
                    n = 12;
                    idx[0] = (W1(dl) >> 25) & 0x1F;
                    idx[1] = (W1(dl) >> 20) & 0x1F;
                    idx[2] = (W1(dl) >> 15) & 0x1F;
                    idx[3] = (W1(dl) >> 10) & 0x1F;
                    idx[4] = (W1(dl) >> 5) & 0x1F;
                    idx[5] = W1(dl) & 0x1F;
                    idx[6] = (W0(dl) >> 10) & 0x1F;
                    idx[7] = (W0(dl) >> 5) & 0x1F;
                    idx[8] = W0(dl) & 0x1F;
                    idx[9] = (W0(dl) >> 23) & 0x1F;
                    idx[10] = (W0(dl) >> 18) & 0x1F;
                    idx[11] = (((W0(dl) >> 15) & 7) << 2) + ((W1(dl) >> 30) & 3);
                }
                for (i = 0; i < n; i++) {
                    if (maxIdx < idx[i]) {
                        maxIdx = idx[i];
                    }
                }
                if (depth == 0) {
                    minIdx = idx[0];
                    if (first != 0) {
                        for (i = 1; i < n; i++) {
                            if (idx[i] < minIdx) {
                                minIdx = idx[i];
                            }
                        }
                        first = 0;
                        minIdx &= 0xFF;
                        *outVtx += minIdx;
                    }
                }
                {
                    s32 pad;
                }
            }
        }
        dl += 8;
    }
    count = ((maxIdx + 1) + base) - *outVtx;
    return count;
}

/* Saved texture coordinates of one vertex. */
typedef struct {
    /* 0x00 */ s16 s;
    /* 0x02 */ s16 t;
} TexCoord;

/* Scrolls the texture coordinates of one mesh's vertices along a circle. */
typedef struct {
    /* 0x00 */ TexCoord *primary;   /* original UVs of the vertices in mesh 0 */
    /* 0x04 */ TexCoord *secondary; /* original UVs of the vertices in mesh 1 */
    /* 0x08 */ u8 primaryCount;
    /* 0x09 */ u8 secondaryCount;
    /* 0x0A */ u8 primaryIndex;     /* first vertex in D_800B0E10[0] */
    /* 0x0B */ u8 secondaryIndex;   /* first vertex in D_800B0E10[1] */
    /* 0x0C */ u8 disabled;
    /* 0x0D */ u8 pad0D;
    /* 0x0E */ u16 phase;
    /* 0x10 */ u16 rate;
    /* 0x12 */ u16 amplitude;
} UvWobble;

extern UvWobble *D_800D9E60[];
extern Vtx *D_800B0E10[];
extern s32 D_800917B8;
extern void *allocate_memory(s32, s32, s32, s32);

/* Registers a UV wobble for the vertices drawn by segment `seg` in both level meshes. */
UvWobble *func_1510C4AC(s32 seg, s32 disabled, s32 rate, s32 amplitude) {
    Vtx *vtx;
    s32 count;
    s32 i;
    UvWobble *wob;

    if (D_800D9E64 > 0) {
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

    count = func_1510BF60(D_800B0E00[0], seg, &vtx, 0);
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
                vtx = D_800B0E10[0];
                vtx += wob->primaryIndex;
                angle = wob->phase * 9.587380191e-05f;
                sn = sinf(angle);
                cs = cosf(angle);
                offS = wob->amplitude * sn;
                offT = wob->amplitude * cs;
                for (j = 0; j < wob->primaryCount; j++) {
                    vtx[j].v.tc[0] = wob->primary[j].s + offS;
                    vtx[j].v.tc[1] = wob->primary[j].t + offT;
                }
            }
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
