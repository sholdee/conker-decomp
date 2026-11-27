#include <ultra64.h>

/* Owner unknown: the .bss between game/Collision/Probe/probe and game/Math/TriBox/tribox (the units between them are
 * libultra's cosf, sinf, guPerspectiveF and guRotateF and seven of game/Math, and none of them uses it).
 * probe does not define it: probe's .bss ends with the 4-byte gTriggerNodes at 0x800CBE00, and an array of u64
 * after that would start at 0x800CBE08. The debug and ECTS builds have the same order. */
u64 gRspDramStack[SP_DRAM_STACK_SIZE64]; /* RSP dram stack of the graphics tasks (game/Cutscene/Sequence/sequence.c) */
