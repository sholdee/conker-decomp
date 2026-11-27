#include <ultra64.h>

/* Owner unknown: the .bss between game/Effects/TintedObject/tintedobject and game_10CD70 (12 units lie between them,
 * from game/Attach/TextureScroll/texturescroll to game/Camera/LevelHook09/levelhook09; none uses it). Script command 2
 * of game/Script/EffectCmd/effectcmd.c sets D_800D9920, game/Level/LevelSetupReset/levelsetupreset.c and
 * game/Level/LevelSetup02/levelsetup02.c clear all four, and nothing reads them. */
u8 D_800D9920;
u8 D_800D9921;
u8 D_800D9928[14];
u8 D_800D9938[14];
