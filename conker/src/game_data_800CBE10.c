#include <ultra64.h>

/* Owner unknown: the .bss between game/Collision/Probe/probe and game/Math/TriBox/tribox (the units between them are
 * libultra's cosf, sinf, guPerspectiveF and guRotateF and seven of game/Math, and none of them uses it). */
u64 D_800CBE10[SP_DRAM_STACK_SIZE64]; /* RSP dram stack of the graphics tasks (game/Cutscene/Sequence/sequence.c) */
