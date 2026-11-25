#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Particle instance; only the fields func_1514BE20 touches (same layout as Particle in game_179400.c). */
typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ struct17 pos;
    /* 0x040 */ struct17 unk40;
    /* 0x04C */ u8 pad4C[0x110];
    /* 0x15C */ f32 easeFactor;
} Particle;

extern s32 func_15145128(struct17 *out, struct17 *in, f32 *length, f32 *invLength);

/*
 * Eases unk40 toward the point 100 units above the particle's position by easeFactor,
 * then pins it back to exactly 100 units from the position along that direction.
 */
void func_1514BE20(void *arg) {
    struct17 above;
    struct17 eased;
    Particle *p;
    struct17 dir;
    f32 length;
    f32 invLength;

    // matching: the void * parameter copied into a typed local hoists &dir's address to entry
    p = arg;
    above.unk0 = p->pos.unk0;
    above.unk4 = p->pos.unk4 + 100.0f;
    above.unk8 = p->pos.unk8;
    eased.unk0 = p->unk40.unk0 + (above.unk0 - p->unk40.unk0) * p->easeFactor;
    eased.unk4 = p->unk40.unk4 + (above.unk4 - p->unk40.unk4) * p->easeFactor;
    eased.unk8 = p->unk40.unk8 + (above.unk8 - p->unk40.unk8) * p->easeFactor;
    {
        s32 pad; // matching: stack slot needed for the original frame layout

        dir.unk0 = eased.unk0 - p->pos.unk0;
        dir.unk4 = eased.unk4 - p->pos.unk4;
        dir.unk8 = eased.unk8 - p->pos.unk8;
        if (func_15145128(&dir, &dir, &length, &invLength)) {
            p->unk40.unk0 = p->pos.unk0 + dir.unk0 * 100.0f;
            p->unk40.unk4 = p->pos.unk4 + dir.unk4 * 100.0f;
            p->unk40.unk8 = p->pos.unk8 + dir.unk8 * 100.0f;
        } else {
            p->unk40 = p->pos;
        }
    }
}
