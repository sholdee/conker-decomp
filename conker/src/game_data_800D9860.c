#include <ultra64.h>
#include "structs.h"

/* Owner unknown: the .bss between game/Script/ScriptInterp/scriptinterp and game/Sprite/SpriteRoutines/spriteroutines
 * (17 units lie between them, from game/Script/TaskExit/taskexit to game/Camera/LevelHook07/levelhook07; none uses it).
 * The debug and ECTS builds have it in the same place. One of the 17 units, game/Particle/SmokeEmitter/smokeemitter.c,
 * calls trail_spawnNearCamera, the function that fills it; that does not show which unit defines it. */
CollisionHit D_800D9860; /* the floor under the last spawn point of game/Effects/Trail/LevelStream/levelstream.c */
