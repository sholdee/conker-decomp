#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// Spawns one of several effects at the point D_800CC2C0, picked by obj's id.
void func_15193CA0(Actor *obj, u8 arg1, u8 arg2, s32 arg3) {
    Vec3f pos;
    CollisionHit ground;
    s32 type;

    collision_initHitFromObject(&ground, obj);
    pos.x = D_800CC2C0;
    pos.y = D_800CC2C4;
    pos.z = D_800CC2C8;
    {
        // Anti-tamper: checksum a code range and corrupt memory if it was modified.
        u32 addr;
        u32 sum;
        u32 end;
        s32 pad; // matching: IDO needs the extra stack slot

        addr = (u32)&zone_updateActor;
        end = (u32)&zone_isOutside;
        sum = 0;
        for (; addr < end; addr += 4) {
            sum ^= *(u32 *)addr;
        }
        if (sum != 0xA3C35D52) {
            addr = (u32)&D_80043B40[(osGetCount() & 0xFF) * 8]; // matching: reuses addr's register
            *(s32 *)addr = osGetCount() & 0x3FFFF7;
        }
    }
    if (obj->character_type == 6 || obj->character_type == 6 || obj->character_type == 9 || obj->character_type == 0xC || obj->character_type == 0xF || obj->character_type == 0x10 || obj->character_type == 0x12 || obj->character_type == 0x17 || obj->character_type == 0x1B || obj->character_type == 0x1E || obj->character_type == 0x28 || obj->character_type == 0x29 || obj->character_type == 0x2A || obj->character_type == 0x2B || obj->character_type == 0x2C || obj->character_type == 0x2D || obj->character_type == 0x2E || obj->character_type == 0x30 || obj->character_type == 0x34 || obj->character_type == 0x38 || obj->character_type == 0x3A || obj->character_type == 0x41 || obj->character_type == 0x42 || obj->character_type == 0x4B || obj->character_type == 0x46 || obj->character_type == 0x47 || obj->character_type == 0x49 || obj->character_type == 0x4A || obj->character_type == 0x4C || obj->character_type == 0x4D || obj->character_type == 0x4E || obj->character_type == 0x4F || obj->character_type == 0x51 || obj->character_type == 0x52 || obj->character_type == 0x55 || obj->character_type == 0x56 || obj->character_type == 0x5D || obj->character_type == 0x60 || obj->character_type == 0x61 || obj->character_type == 0x62 || obj->character_type == 0x63 || obj->character_type == 0x66 || obj->character_type == 0x67 || obj->character_type == 0x6F || obj->character_type == 0x70 || obj->character_type == 0x73 || obj->character_type == 0x77 || obj->character_type == 0x7B || obj->character_type == 0x7D || obj->character_type == 0x7E || obj->character_type == 0x7F || obj->character_type == 0x82 || obj->character_type == 0x83 || obj->character_type == 0x84 || obj->character_type == 0x89 || obj->character_type == 0x8C || obj->character_type == 0x8E || obj->character_type == 0x8F || obj->character_type == 0x91 || obj->character_type == 0x94 || obj->character_type == 0x9E || obj->character_type == 0xA1 || obj->character_type == 0xA6 || obj->character_type == 0xAB || obj->character_type == 0xAC || obj->character_type == 0xB2 || obj->character_type == 0xB4 || obj->character_type == 0xB7 || obj->character_type == 0xB8 || obj->character_type == 0xB9) { // matching: the repeated first id test is in the original
        weapon_spawnImpactBurst(&pos, arg2, arg3);
        return;
    }
    if (obj->character_type == 5 || obj->character_type == 5 || obj->character_type == 7 || obj->character_type == 0x13 || obj->character_type == 0x20 || obj->character_type == 0x23 || obj->character_type == 0x44 || obj->character_type == 0x5A || obj->character_type == 0x5C || obj->character_type == 0x5E || obj->character_type == 0x5F || obj->character_type == 0x6D || obj->character_type == 0x74 || obj->character_type == 0x75 || obj->character_type == 0x78 || obj->character_type == 0x7A || obj->character_type == 0x8A || obj->character_type == 0x8D || obj->character_type == 0xAD || obj->character_type == 0xAE || obj->character_type == 0xAF || obj->character_type == 0xB1) { // matching: repeated first test, as above
        type = 1;
    } else if (obj->character_type == 0 || obj->character_type == 1 || obj->character_type == 2 || obj->character_type == 3 || obj->character_type == 4 || obj->character_type == 0x96 || obj->character_type == 8 || obj->character_type == 0xA || obj->character_type == 0x11 || obj->character_type == 0x14 || obj->character_type == 0x16 || obj->character_type == 0x1A || obj->character_type == 0x21 || obj->character_type == 0x22 || obj->character_type == 0x26 || obj->character_type == 0x27 || obj->character_type == 0x31 || obj->character_type == 0x36 || obj->character_type == 0x3B || obj->character_type == 0x3E || obj->character_type == 0x50 || obj->character_type == 0x53 || obj->character_type == 0x54 || obj->character_type == 0x58 || obj->character_type == 0x5B || obj->character_type == 0x64 || obj->character_type == 0x69 || obj->character_type == 0x71 || obj->character_type == 0x6B || obj->character_type == 0x72 || obj->character_type == 0x76 || obj->character_type == 0x79 || obj->character_type == 0x80 || obj->character_type == 0x86 || obj->character_type == 0x87 || obj->character_type == 0x88 || obj->character_type == 0x90 || obj->character_type == 0x97 || obj->character_type == 0x98 || obj->character_type == 0x9A || obj->character_type == 0x9B || obj->character_type == 0x9C || obj->character_type == 0x9D || obj->character_type == 0x9F || obj->character_type == 0xA0 || obj->character_type == 0xA2 || obj->character_type == 0xA4 || obj->character_type == 0xA5 || obj->character_type == 0xA7 || obj->character_type == 0xB0 || obj->character_type == 0xB3 || obj->character_type == 0xB5 || obj->character_type == 0xB6) {
        type = 0;
    } else if (obj->character_type == 0x25 || obj->character_type == 0x25 || obj->character_type == 0x92 || obj->character_type == 0x95) {
        type = 2;
    } else if (obj->character_type == 0x3F) {
        type = 6;
    } else if (obj->character_type == 0x93) {
        type = 5;
    } else {
        return;
    }
    if (arg1) {
        func_151DC484(&pos, &ground, type, arg2, arg3);
        return;
    }
    func_151DC260(&pos, &ground, type, arg2, arg3);
}

void func_151942B0(Actor *self, Actor *other, u8 *arg2) {
    func_15193CA0(other, FALSE, 0xFF, 1);
}

void func_151942E8(self, other, arg2)
    Actor *self;
    Actor *other;
    u8 *arg2;
{
    func_15193CA0(other, TRUE, 0xFF, 1);
}

void func_15194320(Actor *self, Actor *other, u8 *arg2) {
    switch (other->character_type) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        func_1518D1C0(other, 10, 0, 1, 0xFF, 1, "effects_impact_lavaboulder");
        break;
    }
}

void func_15194394(Actor *self, Actor *other, u8 *arg2) {
    switch (other->character_type) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        func_1518D1C0(other, 12, 0, 1, 0xFF, 1, "effects_impact_fireimp");
        break;
    }
}


// Spawns a burst of effects from other when its id is 0-4.
void func_15194408(Actor *self, Actor *other) {
    SplashSpraySpec burst;
    CollisionHit ground;

    switch (other->character_type) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        break;
    default:
        return;
    }

    func_151AE7B0(other, 0.6f, 90, 0xFF, 1);
    collision_initHitFromObject(&ground, other);
    burst.spray.pos.x = other->xpos;
    burst.spray.pos.y = other->ypos + 50.0f;
    burst.spray.pos.z = other->zpos;
    burst.spray.scaleBase = 6.0f;
    burst.spray.countBase = 12;
    burst.spray.countRange = 5;
    burst.angles.yawRange = 0xFF;
    burst.angles.pitchBase = -0x3F;
    burst.angles.yawBase = 0;
    burst.angles.pitchRange = 0x23;
    burst.spray.itemCountBase = 3;
    burst.spray.itemCountRange = 2;
    burst.spray.lifeBase = 30;
    burst.spray.lifeRange = 30;
    burst.spray.unk30 = 100;
    burst.spray.unk32 = 100;
    burst.spray.unk3C = 16;
    burst.spray.unk3E = 15;
    burst.spray.unk40 = 0;
    burst.spray.kind = 0;
    burst.spray.scaleRange = 3.0f;
    burst.spray.sizeBase = 0.21f;
    burst.spray.sizeRange = 0.406f;
    burst.spray.speedBase = 13.0f;
    burst.spray.speedRange = 8.0f;
    burst.spray.unk38 = 0.3f;
    func_15153F18(&burst.angles, &burst.spray, &ground, 0xFF, 1);
}

void func_15194588(Actor *self, Actor *other, u8 *arg2) {
    func_150C7020(self, 500, 0xFF, 1);
    func_15194408(self, other);
}

// Spawns a burst of effects above other, aimed by self's facing angle, unless other's kind forbids it.
void func_151945CC(Actor *self, Actor *other, u8 *arg2) {
    Vec3f pos;
    s16 facingYaw;
    SplashSpraySpec burst;
    CollisionHit ground;

    facingYaw = (self->langle >> 8) - 0x40; // unk7A is a u16 angle; >> 8 makes it a byte angle
    pos.x = other->xpos;
    pos.y = other->ypos + 270.0;
    pos.z = other->zpos;
    {
        s32 kind;
        s32 profile;

        collision_initHitFromObject(&ground, other);
        kind = func_15134070(other);
        if (kind != 0x63) {
            profile = D_800A3FD8[kind].unkE;
            if (profile != 2) {
                burst.spray.pos = pos;
                burst.spray.scaleBase = 6.699999809f;
                burst.spray.scaleRange = 2.329999924f;
                burst.spray.sizeBase = 0.6800000072f;
                burst.spray.sizeRange = 0.0f;
                burst.spray.speedBase = 4.400000095f;
                burst.spray.countBase = 15;
                burst.spray.countRange = 12;
                burst.angles.yawBase = facingYaw - 0x40;
                burst.angles.yawRange = 0x80;
                burst.angles.pitchBase = -0xA0;
                burst.angles.pitchRange = 0x20;
                burst.spray.itemCountBase = 3;
                burst.spray.itemCountRange = 1;
                burst.spray.lifeBase = 31;
                burst.spray.lifeRange = 19;
                burst.spray.unk30 = 155;
                burst.spray.unk32 = 100;
                burst.spray.unk3C = 16;
                burst.spray.unk3E = 15;
                burst.spray.unk40 = 0;
                burst.spray.speedRange = 5.300000191f;
                burst.spray.unk38 = 0.5f;
                if (profile == 1) {
                    burst.spray.kind = 1;
                } else {
                    burst.spray.kind = 0;
                }
                func_15153F18(&burst.angles, &burst.spray, &ground, 0xFF, 1);
                func_151B4CD0(other, 0xFF, 1);
            }
        }
    }
}

void func_15194794(Actor *self, Actor *other, u8 *arg2) {
    func_151B01B8(self, other);
    func_151B09BC(self, other, 1000, 0xFF, 0);
    switch (other->character_type) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        func_151AF270(other, 0xFF, 1);
        break;
    }
}

// For ids 0-4 and 0x53, spawns a burst of effects above other and calls func_151D8868.
void func_15194810(Actor *self, Actor *other, u8 *arg2) {
    Vec3f pos;
    SplashSpraySpec burst;
    CollisionHit ground;
    RumbleParams params;

    switch (other->character_type) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 0x53:
        pos.x = other->xpos;
        pos.y = other->ypos + 95.0f;
        pos.z = other->zpos;
    fence: // matching: this basic-block boundary keeps as1 from scheduling across it
        collision_initHitFromObject(&ground, other);
        burst.spray.pos = pos;
        burst.spray.scaleBase = 10.0f;
        burst.spray.countBase = 15;
        burst.spray.countRange = 10;
        burst.angles.yawRange = 0xFF;
        burst.angles.pitchBase = -0x3F;
        burst.angles.yawBase = 0;
        burst.angles.pitchRange = 0x39;
        burst.spray.itemCountBase = 3;
        burst.spray.itemCountRange = 2;
        burst.spray.lifeBase = 70;
        burst.spray.lifeRange = 22;
        burst.spray.unk30 = 155;
        burst.spray.unk32 = 100;
        burst.spray.unk3C = 16;
        burst.spray.unk3E = 15;
        burst.spray.unk40 = 0;
        burst.spray.kind = 0;
        burst.spray.scaleRange = 5.0f;
        burst.spray.sizeBase = 0.6750000119f;
        burst.spray.sizeRange = 0.2310000062f;
        burst.spray.speedBase = 5.0f;
        burst.spray.speedRange = 25.10000038f;
        burst.spray.unk38 = 0.5f;
        func_15153F18(&burst.angles, &burst.spray, &ground, 0xFF, 1);
        params.flags = 1;
        params.duration = (func_150ADA20() % 11U) + 20;
        params.strength = 8;
        params.updateKind = -1;
        params.playerMask = 1;
        func_151D8868(&params, 0, 0xFF, 0);
        break;
    }
}

void func_151949E0(Actor *self, Actor *other, u8 *arg2) {
}

// Calls screen_spawnFade when D_8008FD90 == 1 and self's mv->unk1A0 cooldown has run out, then restarts it.
void func_151949F4(s32 arg0, Actor *self, Actor *other) {
    if (D_8008FD90 == 1 && self->playernumber == 0 && (D_800E9D00 & 8) && self->mv->unk1A0 == 0) {
        screen_spawnFade(arg0, self, other);
        self->mv->unk1A0 = 1200;
    }
}

void func_15194A68(Actor *self, Actor *other, u8 *arg2) {
    func_1507DE4C(other);
    func_15138BC0(other, 0xFF, 1);
    func_151949F4(0, self, other);
}

// Sets obj's unk9C flags and starts an id-specific animation for ids 0x75 and 0x80.
void func_15194AB4(Actor *obj) {
    s32 anim;

    obj->unk9C |= 0xFFFE;
    anim = -1;
    switch (obj->character_type) {
    case 0x75:
        anim = 0x73;
        break;
    case 0x80:
        anim = 0x72;
        break;
    }
    if (anim != -1) {
        collision_createPartSet(obj, anim, 1.0f, 0);
    }
}

void func_15194B1C(Actor *obj) {
    s32 kind;

    kind = func_15134070(obj);
    if (kind != 0x63) {
        func_15138120(obj, kind, 0);
    }
    func_15136C3C(obj, 1, 1, 1, 1, 1, 0xFF, 1);
    func_15194AB4(obj);
}

void func_15194B94(Actor *self, Actor *other, u8 *arg2) {
    if (other->character_type == 0x53) {
        func_151942B0(self, other, arg2);
        return;
    }
    func_15194B1C(other);
    func_151949F4(1, self, other);
}

void func_15194BF0(Actor *self, Actor *other, u8 *arg2) {
    switch (other->character_type) {
    case 0x16:
        func_15136C3C(other, 1, 1, 1, 1, 0, 0xFF, 1);
        func_15145A50(other);
        /* fallthrough */
    case 0x7C:
        if (other->attach_char != 0) {
            s32 slot = other->attach_char - 1;

            movement_obj[slot].intelwalk = 10;
            movement_obj[slot].scriptCmd = NULL;
            other->attach_char = 0;
        }
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 0x11:
    case 0x14:
    case 0x3B:
    case 0x58:
    case 0x5A:
    case 0x5B:
    case 0x5F:
    case 0x74:
    case 0x75:
    case 0x7A:
    case 0x80:
    case 0x87:
    case 0x88:
    case 0x90:
    case 0x96:
    case 0x98:
    case 0x9C:
    case 0x9D:
    case 0x9F:
    case 0xA0:
    case 0xB0:
    case 0xB1:
    case 0xB2:
    case 0xB4:
        func_15136C3C(other, 1, 1, 1, 1, 0, 0xFF, 1);
        func_15145A50(other);
        actor_kill(other, 6);
        if (other->mv != NULL && other->mv->GrenadeThrowState != 0) {
            actor_throwGrenade(other);
        }
        break;
    }
}

void func_15194D80(Actor *self, Actor *other, u8 *arg2) {
    func_15194FF4(self, other, FALSE);
}

void func_15194DA4(Actor *self, Actor *other, u8 *arg2) {
    func_15194FF4(self, other, TRUE);
}

void func_15194DC8(Actor *self, Actor *other, u8 *arg2) {
    CollisionHit ground;

    if (gIsMultiplayer != 0) {
        func_15194DA4(self, other, arg2);
    } else {
        func_1518D1C0(other, 11, 0, 1, 0xFF, 1, "effects_impact_lava");
    }
    collision_initHitFromObject(&ground, other);
    func_1514B364((Vec3f *)&other->xpos, &ground, 0xFF, 1); // xpos, ypos, zpos as one Vec3f
}

void func_15194E54(Actor *self, Actor *other, u8 *arg2) {
    if (other->character_type == 0x3E || other->character_type == 0xA6) {
        func_150B06B0(other, self, 0xFF, 1);
    }
}

void func_15194EA8(Actor *self, Actor *other, u8 *arg2) {
    u8 result;
    u32 rand1;
    u32 rand0;

    result = 0;
    if (other->CameraPtr != NULL) {
        rand0 = func_150ADA20();
        rand1 = func_150ADA20();
        screen_spawnFlash(0xFF, 0xFF, 0xFF, 0xFF, rand1 % 11 + 35, other->CameraPtr->CNumber, 0xFF, 1);
        if (rand0) { } // matching: keeps the unused first random value live across the second call
    }
    func_15194BF0(self, other, &result);
}

void func_15194F48(Actor *self, Actor *other, u8 *arg2) {
    Vec3f pos;

    pos.x = other->xpos;
    pos.y = other->ypos + 50.0f;
    pos.z = other->zpos;
    weapon_spawnObjectHit(&pos, other, 1, 0xFF, 1);
    func_15136C3C(other, 0, 0, 1, 0, 0, 0xFF, 1);
    func_15145A50(other);
    actor_kill(other, 6);
}

void func_15194FF4(self, other, arg2)
Actor *self;
Actor *other;
u8 arg2;
{
    PeriodicObject *spawned;
    PeriodicTrigger trigger;

    func_15190518(other);
    switch (other->character_type) {
    case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x11: case 0x14: case 0x16:
    case 0x3B: case 0x50: case 0x53: case 0x58: case 0x5A: case 0x5F: case 0x70: case 0x74:
    case 0x75: case 0x7A: case 0x80: case 0x87: case 0x88: case 0x90: case 0x96: case 0x98:
    case 0x99: case 0x9C: case 0x9D: case 0x9F: case 0xA0: case 0xB0: case 0xB1: case 0xB2:
    case 0xB4:
        if (gIsMultiplayer != 0) {
            trigger.interval = 0x7D;
            trigger.elapsed = 5;
            if (self->character_type == 0xFF && self->owner != 0) { // an id-0xFF object names its owner by slot + 1
                trigger.target = &movement_obj[self->owner - 1];
            } else {
                trigger.target = self;
            }
            spawned = func_1518F51C(other, 0, 250, arg2 ? 0 : -1, arg2 ? 0 : -1, 8, arg2 ? 1 : 0, arg2 ? 12 : 0, 0xFF, 1);
            if (spawned != NULL && arg2 != 0) {
                memcpy(&spawned->trigger, &trigger, sizeof(trigger));
                {
                    ActorCtl *ext = other->mv;

                    if (ext != NULL) {
                        ext->on_fire = 1;
                    }
                }
            }
        } else {
            func_1518F51C(other, 0, 300, -1, -1, 8, 0, 0, 0xFF, 1);
        }
        break;
    }
}


// Kills obj and spawns a copy of it; the two are thrown apart with random velocities.
void func_151951E0(Actor *obj) {
    Actor *copy;

    obj->energy = 0;
    actor_kill(obj, 5);
    func_15194AB4(obj);
    obj->type = 0x28;
    obj->speed = -3.0f;
    obj->unkBC = -10.0f - (func_150ADA20() % 5U);
    obj->unk148 = (func_150ADA20() % 10U) - 5.0f;
    obj->yspd = 0.0f;
    obj->ygrav = 0.0f;
    copy = actor_alloc();
    copy->type = 0x28;
    copy->character_type = obj->character_type;
    copy->imune = 0xFF;
    copy->stunned = 0xFF;
    copy->xpos = obj->xpos;
    copy->ypos = obj->ypos;
    copy->zpos = obj->zpos;
    copy->theta = obj->theta;
    copy->angle = obj->angle;
    copy->langle = obj->langle;
    copy->vtilt = obj->vtilt;
    copy->scale_xz = obj->scale_xz;
    copy->scale_y = obj->scale_y;
    copy->speed = 10.0f;
    copy->unkBC = (func_150ADA20() % 5U) + 10.0f;
    copy->unk148 = (func_150ADA20() % 10U) - 5.0f;
    copy->unk68[0] = obj->unk68[0];
    copy->unk68[1] = obj->unk68[1];
    copy->eyeClosed[0] = obj->eyeClosed[0];
    copy->eyeClosed[1] = obj->eyeClosed[1];
    copy->eyeCode[0] = obj->eyeCode[0];
    copy->eyeCode[1] = obj->eyeCode[1];
    copy->angle += (func_150ADA20() % 10922U) - 65536.0f / 12;
    copy->theta = (obj->angle + 0x4000) * (360.0f / 65536);
    copy->noshove = obj->noshove;
    copy->unk9C |= 0x0FFF0000;
    collision_allocAnimState(copy, obj->animseq.uh, 0, obj->animsp);
    func_150D10E4(obj, 0);
    func_150D10E4(copy, 1);
}

// Calls func_15137610 on a 40-unit segment through other, at right angles to self's (or other's, or a random) heading.
void func_15195528(Actor *self, Actor *other, u8 *arg2) {
    f32 angle;
    Vec3f from;
    Vec3f to;
    f32 dir[3]; // matching: an f32[3] (a Vec3f changes IDO's schedule); passed as a Vec3f below
    f32 dx;
    f32 dz;

    if (self != NULL) {
        angle = self->theta - 90.0f;
    } else if (other != NULL) {
        angle = other->theta - 90.0f;
    } else {
        angle = func_150ADA68() * 360.0f;
    }
    func_1514373C(angle * 0.01745329238f, 1.0f, &dir[0], &dir[2]);
    dir[1] = 0.0f;
    dx = dir[0] * 20.0f;
    from.x = other->xpos + dx;
    from.y = other->ypos + 50.0f;
    dz = dir[2] * 20.0f;
    from.z = other->zpos + dz;
    to.x = other->xpos - dx;
    to.y = other->ypos + 50.0f;
    to.z = other->zpos - dz;
    func_15137610(other, &from, &to, (Vec3f *)dir, 0xFF, 1);
}

void func_15195650(Actor *self, Actor *other, u8 *arg2) {
    func_151B01B8(self, other);
    func_151B09BC(self, other, 1000, 0xFF, 0);
}

void func_15195698(Actor *self, Actor *other, u8 *arg2) {
    if (other->energy == 0 && other->playernumber != 0xFF) {
        func_15136C3C(other, 1, 1, 1, 1, 0, 0xFF, 1);
        func_15145A50(other);
        actor_kill(other, 6);
    }
    if (self->character_type == 0x47) {
        self->energy = 0;
    }
}

void func_15195738(Actor *self, Actor *other, u8 *arg2) {
    RumbleParams params;

    params.flags = 1;
    params.duration = (func_150ADA20() % 11U) + 20;
    params.strength = 8;
    params.updateKind = -1;
    params.playerMask = 1;
    func_151D8868(&params, 0, 0xFF, 0);
}
