#include <ultra64.h>

/* Owner unknown: the .bss between game/Math/MathUtil/mathutil and game/Particle/StreakParticle/streakparticle (15 units
 * lie between them, from game/Effects/ActorDamageFx/actordamagefx to game/Effects/PlayerMarker/playermarker; none uses it). */
u8 gLayoutDetailShift;  /* effect detail shift for the screen layout (0, 1 or 2); game/Level/LevelSetupReset/levelsetupreset.c sets the four */
f32 gLayoutDetailScale; /* its scale, 1 / 2^shift */
u8 D_800DCA28;  /* the same pair for the number of players */
f32 D_800DCA2C;
