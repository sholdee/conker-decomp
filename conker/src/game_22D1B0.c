#include <ultra64.h>

/* Owner unknown: the .data between game/Effects/AttachedBurst/attachedburst and game/Effects/PipeExplode/pipeexplode
 * (8 units lie between them, from game/Effects/Trail/TrailSpray/trailspray to game/Particle/YawStreak/yawstreak; none uses it).
 * light_setDaylight (game/Effects/Light/light.c) reads the colours; in the ECTS build the cutscene events read them.
 * Neither neighbour defines them: attachedburst's .data is 8 bytes, padded to 16, and pipeexplode's first object
 * would follow these 12 bytes without padding. The ECTS build has the unit in the same place, with a fourth colour
 * after these three. game/Effects/PerimeterEmitter and greyscale.s are not in that build, so the owner is one of
 * the other six units, or a file with no code. */
u8 gDaylightOnAmbient[3] = { 0xFF, 0xFF, 0xFF }; /* gAmbientColour with the daylight on */
u8 gDaylightOnBackground[3] = { 0xFF, 0xFF, 0xFF }; /* the colour in the level settings with the daylight on */
u8 gDaylightOffAmbient[3] = { 0x30, 0x38, 0x5F }; /* gAmbientColour with the daylight off */
