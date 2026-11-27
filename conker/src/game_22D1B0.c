#include <ultra64.h>

/* Owner unknown: the .data between game/Effects/AttachedBurst/attachedburst and game/Effects/PipeExplode/pipeexplode
 * (8 units lie between them, from game/Trail/TrailSpray/trailspray to game/Particle/YawStreak/yawstreak; none uses it).
 * light_setDaylight (game/Effects/Light/light.c) reads the colours; in the ECTS build the cutscene events read them. */
u8 D_800886F0[3] = { 0xFF, 0xFF, 0xFF }; /* gAmbientColour with the daylight on */
u8 D_800886F4[3] = { 0xFF, 0xFF, 0xFF }; /* the colour in the level settings with the daylight on */
u8 D_800886F8[3] = { 0x30, 0x38, 0x5F }; /* gAmbientColour with the daylight off */
