#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A data-only unit between game_120950 and game/Particle/Spiral/spiral in the link: its .data (0x22D5D0) follows
 * game_11EEB0's padded .data and its .rodata (0x2464A0) follows game_120950's padded .rodata. Each holds one
 * object and the unit's padding to 16 bytes.
 *
 * The second gun's muzzle point and the bone it is on, like game/Weapon/GunShot/gunshot's gGunMuzzleStart and gGunMuzzleBone: game/Particle/HitImpact/hitimpact's
 * tables gGunMuzzlePoints and gGunMuzzleBones point to them; no code reads them directly.
 */
const Vec3f D_800A19E0 = { 6.0f, 0.0f, 59.0f };

u8 D_80088B10 = 9;
