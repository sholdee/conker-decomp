#include <ultra64.h>

/* Owner unknown: the .bss between game/Effects/TintedObject/tintedobject and
 * game/Effects/MaterialScroll/materialscroll (12 units lie between them, from game/Attach/TextureScroll/texturescroll
 * to game/Camera/LevelHookHarbour/levelhookharbour; none uses it). Script command 2 of game/Script/EffectCmd/effectcmd.c sets
 * D_800D9920, game/Level/LevelSetupReset/levelsetupreset.c and game/Level/LevelSetupBoilerRoom/levelsetupboilerroom.c clear all four,
 * and nothing reads them.
 * Of the per-level setups, only that of LEVEL_W2_BOILERROOM (level_setupBoilerRoom, from the level headers in the ROM) clears
 * them, and that level's update hook is in texturescroll, the first of the 12 units. materialscroll does not define the block: a gap of 10 bytes
 * separates D_800D9938 from materialscroll's gMaterialGlowLevels. In the debug and ECTS builds 16 more bytes that
 * nothing uses lie in that gap. */
u8 D_800D9920;
u8 D_800D9921;
u8 D_800D9928[14];
u8 D_800D9938[14];
