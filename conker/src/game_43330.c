#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// structs.h's struct104 has no field at 0x1C (it is inside pad1C[0xD]); file-local view.
typedef struct {
    u8  pad0[0x1C];
    u16 unk1C;
} Struct43330;

extern void *allocate_memory(s32, s32, s32, s32);
extern s32 D_800BEBA4;

#define SND ((Struct43330 *)D_800B0DF0)

void func_15015E80(void) {
    if (SND->unk1C == 0) {
        SND->unk1C = 8000;
    }
    SND->unk1C += 400;
    D_800BE9C8[0] = allocate_memory(SND->unk1C * sizeof(Gfx), 255, 2, 0);
    D_800BE9C8[1] = allocate_memory(SND->unk1C * sizeof(Gfx), 255, 2, 0);
    D_800BEBA4 = (D_800BE9C8[1] - D_800BE9C8[0]) - 400;
}
