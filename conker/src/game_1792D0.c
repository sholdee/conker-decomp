#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Particle instance; only the fields func_1514BE20 touches (same layout as Particle in game_179400.c). */
typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ Vec3f pos;
    /* 0x040 */ Vec3f unk40;
    /* 0x04C */ u8 pad4C[0x110];
    /* 0x15C */ f32 easeFactor;
} Particle;

extern s32 func_15145128(Vec3f *out, Vec3f *in, f32 *length, f32 *invLength);

/*
 * Eases unk40 toward the point 100 units above the particle's position by easeFactor,
 * then pins it back to exactly 100 units from the position along that direction.
 */
void func_1514BE20(void *arg) {
    Vec3f above;
    Vec3f eased;
    Particle *p;
    Vec3f dir;
    f32 length;
    f32 invLength;

    // matching: the void * parameter copied into a typed local hoists &dir's address to entry
    p = arg;
    above.x = p->pos.x;
    above.y = p->pos.y + 100.0f;
    above.z = p->pos.z;
    eased.x = p->unk40.x + (above.x - p->unk40.x) * p->easeFactor;
    eased.y = p->unk40.y + (above.y - p->unk40.y) * p->easeFactor;
    eased.z = p->unk40.z + (above.z - p->unk40.z) * p->easeFactor;
    {
        s32 pad; // matching: stack slot needed for the original frame layout

        dir.x = eased.x - p->pos.x;
        dir.y = eased.y - p->pos.y;
        dir.z = eased.z - p->pos.z;
        if (func_15145128(&dir, &dir, &length, &invLength)) {
            p->unk40.x = p->pos.x + dir.x * 100.0f;
            p->unk40.y = p->pos.y + dir.y * 100.0f;
            p->unk40.z = p->pos.z + dir.z * 100.0f;
        } else {
            p->unk40 = p->pos;
        }
    }
}
