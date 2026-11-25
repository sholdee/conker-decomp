#include <ultra64.h>
#include "functions.h"
/* variables.h declares D_800B0E30 and D_800B0E34 as arrays, but the code loads them as pointers to
 * tables. Hide those declarations and declare the real ones below. */
#define D_800B0E30 D_800B0E30_array_decl_in_variables_h
#define D_800B0E34 D_800B0E34_array_decl_in_variables_h
#include "variables.h"
#undef D_800B0E30
#undef D_800B0E34

typedef struct {
    u8 r, g, b;
} Color24;

/* A group of vertices whose colors fade together towards their own target colors. */
typedef struct {
    /* 0x0 */ Color24 *colors; // target color of each vertex
    /* 0x4 */ u16 *vertexIndices;
    /* 0x8 */ s32 count;
} VtxColorGroup; // size 0xC

/* Vertex-buffer fields of the 0xA0-byte level props at D_800DBEF4. */
typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Vtx *vertices[2]; // double-buffered by D_800BE9C0
    /* 0x28 */ u8 pad28[0x2C];
    /* 0x54 */ u16 kind;         // bit 15 is a flag; the rest selects the color set
    /* 0x56 */ u8 pad56[0x4A];
} PropVtxView; // size 0xA0

#define sProps ((PropVtxView *)D_800DBEF4)
// Byte 0x21 of *D_800B0DF0: bit 0 enables fades on the level mesh, bit 1 on props, and bit 2 enables
// both and takes the base colors from D_800BE520 instead of D_800BE510.
#define sVtxColorFlags (D_800B0DF0->pad1C[5])

extern Vtx *D_800B0E10;                 // level mesh vertices
extern VtxColorGroup **D_800B0E30;      // groups of each color set
extern u8 *D_800B0E34;                  // group count of each color set
extern u8 (*D_800BE510)[3];             // base RGB of each vertex
extern u8 (***D_800BE520)[3];           // [set][group][i]: base RGB of each group vertex
extern u16 *D_800BE524;                 // first D_800BE510 entry of each prop
extern PropVtxView *func_151149AC(u8);

/* Fades the vertices of color set colorSet (group group, or every group when group is 0xFF) from their
 * base colors towards the groups' target colors by blend/256. isProp selects the vertices of prop
 * propIndex instead of the level mesh. */
void func_151739B0(s32 colorSet, s32 isProp, s32 blend, s32 group, s32 propIndex) {
    s32 i;
    s32 j;
    s32 start;
    s32 end;
    Vtx *vertices;
    VtxColorGroup *groups;
    Color24 *targets;
    u16 *indices;
    u8 *base; // RGB
    u16 index;
    Vtx *v;

    if (D_800B0E34[colorSet] == 0) {
        return;
    }
    if (!(sVtxColorFlags & 4) &&
        ((isProp == 0 && !(sVtxColorFlags & 1)) || (isProp == 1 && !(sVtxColorFlags & 2)))) {
        return;
    }
    if (group == 0xFF) {
        start = 0;
        end = D_800B0E34[colorSet];
    } else {
        start = group;
        if (group >= D_800B0E34[colorSet]) {
            return;
        }
        end = group + 1;
    }
    if (isProp == 0) {
        vertices = D_800B0E10;
    } else {
        vertices = sProps[propIndex].vertices[D_800BE9C0];
    }
    groups = D_800B0E30[colorSet];
    for (i = start; i < end; i++) {
        indices = groups[i].vertexIndices;
        targets = groups[i].colors;
        for (j = 0; j < groups[i].count; j++) {
            index = indices[j];
            if (!(sVtxColorFlags & 4)) {
                // matching: the byte offset index * 3 from the start row; IDO multiplies it (multu)
                // where D_800BE510[start + index] would be shift-scaled
                if (isProp == 0) {
                    base = &D_800BE510[0][index * 3];
                } else {
                    base = &D_800BE510[D_800BE524[propIndex]][index * 3];
                }
            } else {
                base = D_800BE520[colorSet][i][j];
            }
            v = &vertices[index];
            v->v.cn[0] = base[0] + (((targets[j].r - base[0]) * blend) >> 8);
            v->v.cn[1] = base[1] + (((targets[j].g - base[1]) * blend) >> 8);
            v->v.cn[2] = base[2] + (((targets[j].b - base[2]) * blend) >> 8);
        }
    }
}

/* Fades the level mesh's color set 0 (see func_151739B0). */
void func_15173C60(s32 blend, s32 group) {
    func_151739B0(0, 0, blend, group, 0);
}

/* Fades the vertices of prop propId (see func_151739B0); its kind selects the color set. */
void func_15173C90(s32 blend, s32 group, s32 propId) {
    PropVtxView *prop;

    prop = func_151149AC(propId);
    if (prop != NULL) {
        s32 colorSet = prop->kind & ~0x8000;

        func_151739B0(colorSet, 1, blend, group, prop - sProps);
    }
}
