#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// Byte 0x21 of *D_800B0DF0: bit 0 enables fades on the level mesh, bit 1 on props, and bit 2 enables
// both and takes the base colors from D_800BE520 instead of D_800BE510.
#define sVtxColorFlags (D_800B0DF0->vtxColorFlags)

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
        vertices = D_800B0E10[0];
    } else {
        vertices = db_Object_array[propIndex].vertices[D_800BE9C0];
    }
    groups = D_800B0E30[colorSet];
    for (i = start; i < end; i++) {
        indices = groups[i].vertexIndices;
        targets = groups[i].colors;
        for (j = 0; j < groups[i].count; j++) {
            index = indices[j];
            if (!(sVtxColorFlags & 4)) {
                // matching: the byte offset index * 3 from the start row; IDO multiplies it (multu)
                // where D_800BE510[0][start + index] would be shift-scaled
                if (isProp == 0) {
                    base = &D_800BE510[0][0][index * 3];
                } else {
                    base = &D_800BE510[0][D_800BE524[propIndex]][index * 3];
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
    WorldObject *prop;

    prop = func_151149AC(propId);
    if (prop != NULL) {
        s32 colorSet = prop->modelId & ~0x8000;

        func_151739B0(colorSet, 1, blend, group, prop - db_Object_array);
    }
}
