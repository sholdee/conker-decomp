#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

extern void func_1504715C(CollisionHit *ground, Actor *obj);

// Parameters for func_151D8868.
typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ u8 unk7;
} Struct15195738Small;

extern void func_151D8868(Struct15195738Small *params, s32 arg1, u8 arg2, s32 arg3);
extern void func_151C0098(f32 *pos, Actor *obj, s32 arg2, u8 arg3, s32 arg4);
extern void func_1507CD64(Actor *obj, s32 arg1);
extern s32 func_1518D1C0(Actor *obj, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5, u8 *arg6);
extern u8 D_800A8244[];

extern void func_151C329C(f32 *pos, u8 arg1, s32 arg2);
extern void func_151DC260(f32 *pos, CollisionHit *ground, s32 type, u8 arg3, s32 arg4);
extern void func_151DC484(f32 *pos, CollisionHit *ground, s32 type, u8 arg3, s32 arg4);
extern s32 func_150A11C4;
extern s32 func_150A1DA0;

// Spawns one of several effects at the point D_800CC2C0, picked by obj's id.
void func_15193CA0(Actor *obj, u8 arg1, u8 arg2, s32 arg3) {
    f32 pos[3];
    CollisionHit ground;
    s32 type;

    func_1504715C(&ground, obj);
    pos[0] = D_800CC2C0;
    pos[1] = D_800CC2C4;
    pos[2] = D_800CC2C8;
    {
        // Anti-tamper: checksum a code range and corrupt memory if it was modified.
        u32 addr;
        u32 sum;
        u32 end;
        s32 pad; // matching: IDO needs the extra stack slot

        addr = (u32)&func_150A11C4;
        end = (u32)&func_150A1DA0;
        sum = 0;
        for (; addr < end; addr += 4) {
            sum ^= *(u32 *)addr;
        }
        if (sum != 0xA3C35D52) {
            addr = (u32)&D_80043B40[(osGetCount() & 0xFF) * 8]; // matching: reuses addr's register
            *(s32 *)addr = osGetCount() & 0x3FFFF7;
        }
    }
    if (obj->id == 6 || obj->id == 6 || obj->id == 9 || obj->id == 0xC || obj->id == 0xF || obj->id == 0x10 || obj->id == 0x12 || obj->id == 0x17 || obj->id == 0x1B || obj->id == 0x1E || obj->id == 0x28 || obj->id == 0x29 || obj->id == 0x2A || obj->id == 0x2B || obj->id == 0x2C || obj->id == 0x2D || obj->id == 0x2E || obj->id == 0x30 || obj->id == 0x34 || obj->id == 0x38 || obj->id == 0x3A || obj->id == 0x41 || obj->id == 0x42 || obj->id == 0x4B || obj->id == 0x46 || obj->id == 0x47 || obj->id == 0x49 || obj->id == 0x4A || obj->id == 0x4C || obj->id == 0x4D || obj->id == 0x4E || obj->id == 0x4F || obj->id == 0x51 || obj->id == 0x52 || obj->id == 0x55 || obj->id == 0x56 || obj->id == 0x5D || obj->id == 0x60 || obj->id == 0x61 || obj->id == 0x62 || obj->id == 0x63 || obj->id == 0x66 || obj->id == 0x67 || obj->id == 0x6F || obj->id == 0x70 || obj->id == 0x73 || obj->id == 0x77 || obj->id == 0x7B || obj->id == 0x7D || obj->id == 0x7E || obj->id == 0x7F || obj->id == 0x82 || obj->id == 0x83 || obj->id == 0x84 || obj->id == 0x89 || obj->id == 0x8C || obj->id == 0x8E || obj->id == 0x8F || obj->id == 0x91 || obj->id == 0x94 || obj->id == 0x9E || obj->id == 0xA1 || obj->id == 0xA6 || obj->id == 0xAB || obj->id == 0xAC || obj->id == 0xB2 || obj->id == 0xB4 || obj->id == 0xB7 || obj->id == 0xB8 || obj->id == 0xB9) { // matching: the repeated first id test is in the original
        func_151C329C(pos, arg2, arg3);
        return;
    }
    if (obj->id == 5 || obj->id == 5 || obj->id == 7 || obj->id == 0x13 || obj->id == 0x20 || obj->id == 0x23 || obj->id == 0x44 || obj->id == 0x5A || obj->id == 0x5C || obj->id == 0x5E || obj->id == 0x5F || obj->id == 0x6D || obj->id == 0x74 || obj->id == 0x75 || obj->id == 0x78 || obj->id == 0x7A || obj->id == 0x8A || obj->id == 0x8D || obj->id == 0xAD || obj->id == 0xAE || obj->id == 0xAF || obj->id == 0xB1) { // matching: repeated first test, as above
        type = 1;
    } else if (obj->id == 0 || obj->id == 1 || obj->id == 2 || obj->id == 3 || obj->id == 4 || obj->id == 0x96 || obj->id == 8 || obj->id == 0xA || obj->id == 0x11 || obj->id == 0x14 || obj->id == 0x16 || obj->id == 0x1A || obj->id == 0x21 || obj->id == 0x22 || obj->id == 0x26 || obj->id == 0x27 || obj->id == 0x31 || obj->id == 0x36 || obj->id == 0x3B || obj->id == 0x3E || obj->id == 0x50 || obj->id == 0x53 || obj->id == 0x54 || obj->id == 0x58 || obj->id == 0x5B || obj->id == 0x64 || obj->id == 0x69 || obj->id == 0x71 || obj->id == 0x6B || obj->id == 0x72 || obj->id == 0x76 || obj->id == 0x79 || obj->id == 0x80 || obj->id == 0x86 || obj->id == 0x87 || obj->id == 0x88 || obj->id == 0x90 || obj->id == 0x97 || obj->id == 0x98 || obj->id == 0x9A || obj->id == 0x9B || obj->id == 0x9C || obj->id == 0x9D || obj->id == 0x9F || obj->id == 0xA0 || obj->id == 0xA2 || obj->id == 0xA4 || obj->id == 0xA5 || obj->id == 0xA7 || obj->id == 0xB0 || obj->id == 0xB3 || obj->id == 0xB5 || obj->id == 0xB6) {
        type = 0;
    } else if (obj->id == 0x25 || obj->id == 0x25 || obj->id == 0x92 || obj->id == 0x95) {
        type = 2;
    } else if (obj->id == 0x3F) {
        type = 6;
    } else if (obj->id == 0x93) {
        type = 5;
    } else {
        return;
    }
    if (arg1) {
        func_151DC484(pos, &ground, type, arg2, arg3);
        return;
    }
    func_151DC260(pos, &ground, type, arg2, arg3);
}

void func_151942B0(Actor *self, Actor *other, u8 *arg2) {
    func_15193CA0(other, FALSE, 0xFF, 1);
}

void func_151942E8(Actor *self, Actor *other, u8 *arg2) {
    func_15193CA0(other, TRUE, 0xFF, 1);
}

extern u8 D_800A8210;

void func_15194320(Actor *self, Actor *other, u8 *arg2) {
    switch (other->id) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        func_1518D1C0(other, 10, 0, 1, 0xFF, 1, &D_800A8210);
        break;
    }
}

extern u8 D_800A822C;

void func_15194394(Actor *self, Actor *other, u8 *arg2) {
    switch (other->id) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        func_1518D1C0(other, 12, 0, 1, 0xFF, 1, &D_800A822C);
        break;
    }
}

extern void func_15153F18(AngleRange *angles, SplashSpray *spray, CollisionHit *ground, u8 arg3, s32 arg4);
extern void *func_151AE7B0(Actor *obj, f32 arg1, s16 arg2, u8 arg3, s32 arg4);

// Spawns a burst of effects from other when its id is 0-4.
void func_15194408(Actor *self, Actor *other) {
    SplashSpraySpec burst;
    CollisionHit ground;

    switch (other->id) {
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
    func_1504715C(&ground, other);
    burst.spray.pos.x = other->x_position;
    burst.spray.pos.y = other->y_position + 50.0f;
    burst.spray.pos.z = other->z_position;
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

extern void func_150C7020(Actor *obj, s32 arg1, u8 arg2, s32 arg3);

void func_15194588(Actor *self, Actor *other, u8 *arg2) {
    func_150C7020(self, 500, 0xFF, 1);
    func_15194408(self, other);
}

extern u8 D_800A3FE6[];
extern s32 func_15134070(Actor *);
extern void func_151B4CD0(Actor *obj, u8 arg1, s32 arg2);

// Spawns a burst of effects above other, aimed by self's facing angle, unless other's kind forbids it.
void func_151945CC(Actor *self, Actor *other, u8 *arg2) {
    Vec3f pos;
    s16 facingYaw;
    SplashSpraySpec burst;
    CollisionHit ground;

    facingYaw = (self->unk7A >> 8) - 0x40; // unk7A is a u16 angle; >> 8 makes it a byte angle
    pos.x = other->x_position;
    pos.y = other->y_position + 270.0;
    pos.z = other->z_position;
    {
        s32 kind;
        s32 profile;

        func_1504715C(&ground, other);
        kind = func_15134070(other);
        if (kind != 0x63) {
            profile = D_800A3FE6[kind * 16]; // 16-byte records
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

void func_151B01B8(Actor *self, Actor *other);
void func_151B09BC(Actor *self, Actor *other, s16 arg2, u8 arg3, s32 arg4);
void *func_151AF270(Actor *obj, u8 arg1, s32 arg2);

void func_15194794(Actor *self, Actor *other, u8 *arg2) {
    func_151B01B8(self, other);
    func_151B09BC(self, other, 1000, 0xFF, 0);
    switch (other->id) {
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
    Struct15195738Small params;

    switch (other->id) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 0x53:
        pos.x = other->x_position;
        pos.y = other->y_position + 95.0f;
        pos.z = other->z_position;
    fence: // matching: this basic-block boundary keeps as1 from scheduling across it
        func_1504715C(&ground, other);
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
        params.unk0 = 1;
        params.unk2 = (func_150ADA20() % 11U) + 20;
        params.unk4 = 8;
        params.unk6 = -1;
        params.unk5 = 1;
        func_151D8868(&params, 0, 0xFF, 0);
        break;
    }
}

void func_151949E0(Actor *self, Actor *other, u8 *arg2) {
}

extern void func_15104170(s32 arg0, Actor *self, Actor *other);
extern void func_1507DE4C(Actor *obj);
extern void func_15138BC0(Actor *obj, u8 arg1, s32 arg2);

// Calls func_15104170 when D_8008FD90 == 1 and self's unk31C->unk1A0 cooldown has run out, then restarts it.
void func_151949F4(s32 arg0, Actor *self, Actor *other) {
    if (D_8008FD90 == 1 && self->unk127 == 0 && (D_800E9D00 & 8) && self->unk31C->unk1A0 == 0) {
        func_15104170(arg0, self, other);
        self->unk31C->unk1A0 = 1200;
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
    switch (obj->id) {
    case 0x75:
        anim = 0x73;
        break;
    case 0x80:
        anim = 0x72;
        break;
    }
    if (anim != -1) {
        func_15083568(obj, anim, 1.0f, 0);
    }
}

extern void func_15138120(Actor *obj, s32 kind, s32 arg2);
extern void func_15136C3C(Actor *obj, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, s32 arg7);

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
    if (other->id == 0x53) {
        func_151942B0(self, other, arg2);
        return;
    }
    func_15194B1C(other);
    func_151949F4(1, self, other);
}

void func_15194BF0(Actor *self, Actor *other, u8 *arg2) {
    switch (other->id) {
    case 0x16:
        func_15136C3C(other, 1, 1, 1, 1, 0, 0xFF, 1);
        func_15145A50(other);
        /* fallthrough */
    case 0x7C:
        if (other->unk65 != 0) {
            s32 slot = other->unk65 - 1;

            D_800CC2D0[slot].unk232 = 10;
            D_800CC2D0[slot].unk218 = NULL;
            other->unk65 = 0;
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
        func_1507CD64(other, 6);
        if (other->unk31C != NULL && other->unk31C->unk19B != 0) {
            func_15063B64(other);
        }
        break;
    }
}

void func_15194FF4(Actor *self, Actor *other, s32 arg2);

void func_15194D80(Actor *self, Actor *other, u8 *arg2) {
    func_15194FF4(self, other, FALSE);
}

void func_15194DA4(Actor *self, Actor *other, u8 *arg2) {
    func_15194FF4(self, other, TRUE);
}

extern void func_1514B364(f32 *pos, CollisionHit *ground, u8 arg2, s32 arg3);

void func_15194DC8(Actor *self, Actor *other, u8 *arg2) {
    CollisionHit ground;

    if (D_800BE616 != 0) {
        func_15194DA4(self, other, arg2);
    } else {
        func_1518D1C0(other, 11, 0, 1, 0xFF, 1, D_800A8244);
    }
    func_1504715C(&ground, other);
    func_1514B364(&other->x_position, &ground, 0xFF, 1);
}

extern void func_150B06B0(Actor *obj, Actor *other, u8 arg2, s32 arg3);

void func_15194E54(Actor *self, Actor *other, u8 *arg2) {
    if (other->id == 0x3E || other->id == 0xA6) {
        func_150B06B0(other, self, 0xFF, 1);
    }
}

extern void func_15182670(u8 arg0, u8 arg1, u8 arg2, u8 arg3, s16 arg4, u8 arg5, u8 arg6, s32 arg7);

void func_15194EA8(Actor *self, Actor *other, u8 *arg2) {
    u8 result;
    u32 rand1;
    u32 rand0;

    result = 0;
    if (other->camera != NULL) {
        rand0 = func_150ADA20();
        rand1 = func_150ADA20();
        func_15182670(0xFF, 0xFF, 0xFF, 0xFF, rand1 % 11 + 35, other->camera->playerIndex, 0xFF, 1);
        if (rand0) { } // matching: keeps the unused first random value live across the second call
    }
    func_15194BF0(self, other, &result);
}

void func_15194F48(Actor *self, Actor *other, u8 *arg2) {
    f32 pos[3];

    pos[0] = other->x_position;
    pos[1] = other->y_position + 50.0f;
    pos[2] = other->z_position;
    func_151C0098(pos, other, 1, 0xFF, 1);
    func_15136C3C(other, 0, 0, 1, 0, 0, 0xFF, 1);
    func_15145A50(other);
    func_1507CD64(other, 6);
}

typedef struct {
    s32 unk0;
    s32 unk4;
    Actor *source;
} Struct15194FF4;

extern u8 *func_1518F51C(Actor *obj, u8 arg1, s16 arg2, s8 arg3, s8 arg4, s8 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9);
extern void func_15190518(Actor *obj);

void func_15194FF4(self, other, arg2)
Actor *self;
Actor *other;
u8 arg2;
{
    u8 *spawned;
    Struct15194FF4 info;

    func_15190518(other);
    switch (other->id) {
    case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x11: case 0x14: case 0x16:
    case 0x3B: case 0x50: case 0x53: case 0x58: case 0x5A: case 0x5F: case 0x70: case 0x74:
    case 0x75: case 0x7A: case 0x80: case 0x87: case 0x88: case 0x90: case 0x96: case 0x98:
    case 0x99: case 0x9C: case 0x9D: case 0x9F: case 0xA0: case 0xB0: case 0xB1: case 0xB2:
    case 0xB4:
        if (D_800BE616 != 0) {
            info.unk0 = 0x7D;
            info.unk4 = 5;
            if (self->id == 0xFF && self->unk124 != 0) { // an id-0xFF object names its owner by slot + 1
                info.source = &D_800CC2D0[self->unk124 - 1];
            } else {
                info.source = self;
            }
            spawned = func_1518F51C(other, 0, 250, arg2 ? 0 : -1, arg2 ? 0 : -1, 8, arg2 ? 1 : 0, arg2 ? 12 : 0, 0xFF, 1);
            if (spawned != NULL && arg2 != 0) {
                memcpy(spawned + 0x90, &info, sizeof(info));
                {
                    ActorCtl *ext = other->unk31C;

                    if (ext != NULL) {
                        ext->unk1AC = 1;
                    }
                }
            }
        } else {
            func_1518F51C(other, 0, 300, -1, -1, 8, 0, 0, 0xFF, 1);
        }
        break;
    }
}

extern Actor *func_1505ED34(void);
extern s32 func_150838EC(Actor *obj, s32 anim, s32 arg2, f32 speed);
extern void func_150D10E4(Actor *obj, u8 arg1);

// Kills obj and spawns a copy of it; the two are thrown apart with random velocities.
void func_151951E0(Actor *obj) {
    Actor *copy;

    obj->health = 0;
    func_1507CD64(obj, 5);
    func_15194AB4(obj);
    obj->interaction_state = 0x28;
    obj->xz_velocity = -3.0f;
    obj->unkBC = -10.0f - (func_150ADA20() % 5U);
    obj->unk148 = (func_150ADA20() % 10U) - 5.0f;
    obj->y_velocity = 0.0f;
    obj->gravity = 0.0f;
    copy = func_1505ED34();
    copy->interaction_state = 0x28;
    copy->id = obj->id;
    copy->immune = 0xFF;
    copy->stunned = 0xFF;
    copy->x_position = obj->x_position;
    copy->y_position = obj->y_position;
    copy->z_position = obj->z_position;
    copy->unk40 = obj->unk40;
    copy->unk76 = obj->unk76;
    copy->unk7A = obj->unk7A;
    copy->unkB8 = obj->unkB8;
    copy->xz_scale = obj->xz_scale;
    copy->y_scale = obj->y_scale;
    copy->xz_velocity = 10.0f;
    copy->unkBC = (func_150ADA20() % 5U) + 10.0f;
    copy->unk148 = (func_150ADA20() % 10U) - 5.0f;
    copy->unk68[0] = obj->unk68[0];
    copy->unk68[1] = obj->unk68[1];
    copy->eyeClosed[0] = obj->eyeClosed[0];
    copy->eyeClosed[1] = obj->eyeClosed[1];
    copy->eyeCode[0] = obj->eyeCode[0];
    copy->eyeCode[1] = obj->eyeCode[1];
    copy->unk76 += (func_150ADA20() % 10922U) - 65536.0f / 12;
    copy->unk40 = (obj->unk76 + 0x4000) * (360.0f / 65536);
    copy->unkF8 = obj->unkF8;
    copy->unk9C |= 0x0FFF0000;
    func_150838EC(copy, obj->unk84.uh, 0, obj->animation_speed);
    func_150D10E4(obj, 0);
    func_150D10E4(copy, 1);
}

extern void func_1514373C(f32 angle, f32 length, f32 *x, f32 *z);
extern void func_15137610(Actor *obj, f32 *from, f32 *to, f32 *dir, u8 arg4, s32 arg5);

// Calls func_15137610 on a 40-unit segment through other, at right angles to self's (or other's, or a random) heading.
void func_15195528(Actor *self, Actor *other, u8 *arg2) {
    f32 angle;
    f32 from[3];
    f32 to[3];
    f32 dir[3];
    f32 dx;
    f32 dz;

    if (self != NULL) {
        angle = self->unk40 - 90.0f;
    } else if (other != NULL) {
        angle = other->unk40 - 90.0f;
    } else {
        angle = func_150ADA68() * 360.0f;
    }
    func_1514373C(angle * 0.01745329238f, 1.0f, &dir[0], &dir[2]);
    dir[1] = 0.0f;
    dx = dir[0] * 20.0f;
    from[0] = other->x_position + dx;
    from[1] = other->y_position + 50.0f;
    dz = dir[2] * 20.0f;
    from[2] = other->z_position + dz;
    to[0] = other->x_position - dx;
    to[1] = other->y_position + 50.0f;
    to[2] = other->z_position - dz;
    func_15137610(other, from, to, dir, 0xFF, 1);
}

void func_15195650(Actor *self, Actor *other, u8 *arg2) {
    func_151B01B8(self, other);
    func_151B09BC(self, other, 1000, 0xFF, 0);
}

void func_15195698(Actor *self, Actor *other, u8 *arg2) {
    if (other->health == 0 && other->unk127 != 0xFF) {
        func_15136C3C(other, 1, 1, 1, 1, 0, 0xFF, 1);
        func_15145A50(other);
        func_1507CD64(other, 6);
    }
    if (self->id == 0x47) {
        self->health = 0;
    }
}

void func_15195738(Actor *self, Actor *other, u8 *arg2) {
    Struct15195738Small params;

    params.unk0 = 1;
    params.unk2 = (func_150ADA20() % 11U) + 20;
    params.unk4 = 8;
    params.unk6 = -1;
    params.unk5 = 1;
    func_151D8868(&params, 0, 0xFF, 0);
}
