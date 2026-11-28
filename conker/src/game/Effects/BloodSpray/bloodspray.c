#include <ultra64.h>
#include "conker/blood.h"
#include "conker/collision.h"
#include "conker/effect.h"

/* One entry per spray type: a bone and two points in its local space. */
typedef struct {
    /* 0x00 */ u8 bone;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ Vec3f from;
    /* 0x10 */ Vec3f to;
} BloodBonePoint;

static const BloodBonePoint gBloodBonePoints[] = {
    { 8, { 0 }, { 0.0f, 11.0f, -25.0f }, { 0.0f, 11.0f, -60.0f } },
    { 8, { 0 }, { 37.0f, 0.0f, 0.0f }, { 70.0f, 0.0f, 0.0f } },
    { 8, { 0 }, { -37.0f, 0.0f, 0.0f }, { -70.0f, 0.0f, 0.0f } },
    { 8, { 0 }, { 0.0f, 34.0f, 0.0f }, { 0.0f, 77.0f, 0.0f } },
    { 8, { 0 }, { 0.0f, 29.0f, 20.0f }, { 0.0f, 62.0f, 44.0f } },
    { 1, { 0 }, { -23.0f, 1.0f, -7.0f }, { -37.0f, 1.0f, -23.0f } },
    { 1, { 0 }, { 23.0f, 1.0f, -7.0f }, { 37.0f, 1.0f, -23.0f } },
    { 1, { 0 }, { 0.0f, -9.0f, 18.0f }, { 0.0f, -17.0f, 43.0f } },
    { 1, { 0 }, { 0.0f, 1.0f, 22.0f }, { 0.0f, 1.0f, 50.0f } },
    { 1, { 0 }, { 0.0f, 15.0f, 20.0f }, { 0.0f, 22.0f, 44.0f } },
    { 2, { 0 }, { 0.0f, 22.0f, 13.0f }, { 0.0f, 36.0f, 34.0f } },
    { 1, { 0 }, { 19.0f, -7.0f, -2.0f }, { 39.0f, -7.0f, -23.0f } },
    { 1, { 0 }, { 10.0f, 13.0f, -4.0f }, { 2.0f, 23.0f, -20.0f } },
    { 1, { 0 }, { 4.0f, 21.0f, -6.0f }, { 12.0f, 27.0f, -30.0f } },
    { 2, { 0 }, { -5.0f, 26.0f, -4.0f }, { -13.0f, 37.0f, -30.0f } },
    { 17, { 0 }, { 0.0f, 0.0f, 0.0f }, { 23.0f, 17.0f, -2.0f } },
    { 22, { 0 }, { 0.0f, 0.0f, 0.0f }, { -7.0f, 19.0f, 13.0f } },
    { 18, { 0 }, { -16.0f, 9.0f, -4.0f }, { -16.0f, 47.0f, -4.0f } },
    { 23, { 0 }, { 26.0f, 0.0f, 0.0f }, { 55.0f, 0.0f, 0.0f } },
    { 27, { 0 }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 40.0f } },
    { 27, { 0 }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -42.0f } },
    { 31, { 0 }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 40.0f } },
    { 31, { 0 }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -42.0f } },
    { 34, { 0 }, { 0.0f, -4.0f, -37.0f }, { 0.0f, -21.0f, -76.0f } },
    { 35, { 0 }, { 0.0f, 6.0f, -45.0f }, { 0.0f, -22.0f, -79.0f } },
    { 36, { 0 }, { 0.0f, 24.0f, -46.0f }, { 0.0f, 24.0f, -91.0f } },
    { 37, { 0 }, { 0.0f, 47.0f, -46.0f }, { 0.0f, 47.0f, -73.0f } },
};

/* Blood spray of kind `type` between two points attached to one of obj's bones (skipped when cullMask&0xF is 0xF). */
void blood_sprayFromBone(obj, type, sceneTag, list)
    Actor *obj;
    u8 type;
    u8 sceneTag;
    s32 list;
{
    Vec3f from;
    Vec3f to;
    MtxF *boneMtx;
    const BloodBonePoint *entry;

    if (obj != NULL && obj->ob_matrix_ptr != NULL && (obj->cullMask & 0xF) != 0xF) {
        entry = &gBloodBonePoints[type];
        boneMtx = &obj->ob_matrix_ptr[entry->bone];
        math_transformOffset(&entry->from, &from, boneMtx);
        math_transformOffset(&entry->to, &to, boneMtx);
        blood_spray(&from, &to, ((math_randomFloat() * 170.0f) + 71.0f) * 0.01f, sceneTag, list);
    }
}
