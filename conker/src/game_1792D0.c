#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Particle instance; only the fields func_1514BE20 touches (same layout as Particle in game_179400.c). */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ u8 pad110[0x4C];
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
    above.x = p->head.def.pos.x;
    above.y = p->head.def.pos.y + 100.0f;
    above.z = p->head.def.pos.z;
    eased.x = p->head.def.rot.x + (above.x - p->head.def.rot.x) * p->easeFactor;
    eased.y = p->head.def.rot.y + (above.y - p->head.def.rot.y) * p->easeFactor;
    eased.z = p->head.def.rot.z + (above.z - p->head.def.rot.z) * p->easeFactor;
    {
        s32 pad; // matching: stack slot needed for the original frame layout

        dir.x = eased.x - p->head.def.pos.x;
        dir.y = eased.y - p->head.def.pos.y;
        dir.z = eased.z - p->head.def.pos.z;
        if (func_15145128(&dir, &dir, &length, &invLength)) {
            p->head.def.rot.x = p->head.def.pos.x + dir.x * 100.0f;
            p->head.def.rot.y = p->head.def.pos.y + dir.y * 100.0f;
            p->head.def.rot.z = p->head.def.pos.z + dir.z * 100.0f;
        } else {
            p->head.def.rot = p->head.def.pos;
        }
    }
}
