#include <ultra64.h>
#include "conker/weapon.h"

/*
 * A data-only unit between game/Player/FlyControl/flycontrol and game/Particle/Spiral/spiral in the link: its .data (0x22D5D0) follows
 * game_11EEB0's padded .data and its .rodata (0x2464A0) follows game/Player/FlyControl/flycontrol's padded .rodata. Each holds one
 * object and the unit's padding to 16 bytes.
 *
 * The second gun's muzzle point and the bone it is on, like game/Weapon/GunShot/gunshot's gGunMuzzleStart and gGunMuzzleBone: game/Particle/HitImpact/hitimpact's
 * tables gGunMuzzlePoints and gGunMuzzleBones point to them; no code reads them directly.
 */
const Vec3f gSecondGunMuzzle = { 6.0f, 0.0f, 59.0f };

u8 gSecondGunMuzzleBone = 9;
