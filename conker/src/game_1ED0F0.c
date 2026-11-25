/* functions.h declares func_150ADA20 as returning u8; it returns a full u32 random number. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#undef func_150ADA20
u32 func_150ADA20(void);

void func_151541B8(void *, f32, s32, f32, f32, u8, s32);

/* Particle parameters for a bullet impact, filled in by func_151BFC40. */
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ vertex pos;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s8  unk50;
} ImpactFxConfig; /* size 0x54 */

/* Collision surface (0x12 bytes, 2-byte aligned) as stored in a ray-cast result. */
typedef struct {
    /* 0x00 */ s16 unk00[9];
} HitSurface;

/* Argument block for func_1514FB98: an impact with no surface. */
typedef struct {
    /* 0x00 */ vertex vel;
    /* 0x0C */ f32 scale;
    /* 0x10 */ ImpactFxConfig config;
} ImpactFx; /* size 0x64 */

/* Argument block for func_1514FBFC: an impact against a collision surface. */
typedef struct {
    /* 0x00 */ vertex vel;
    /* 0x0C */ HitSurface surface;
    /* 0x20 */ f32 scale;
    /* 0x24 */ ImpactFxConfig config;
} SurfaceImpactFx; /* size 0x78 */

/* Argument block for func_1514FCE8. */
typedef struct {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ vertex pos;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s8  unk58;
} DebrisFx;

/* Fills in the impact particle parameters; fewer particles on busy frames. */
void func_151BFC40(ImpactFxConfig *config, f32 *scale) {
    config->unk00 = 3;
    config->unk04 = 4 >> ((D_80082FA0 >= 2) ? 1 : 0);
    *scale = 494.0f;
    config->unk14 = 0.06089999899f;
    config->unk18 = 0.02520000003f;
    config->unk1C = 45.0f;
    config->unk20 = 53.0f;
    config->unk24 = 203.0f;
    config->unk28 = 414.0f;
    config->unk2C = 7;
    config->unk30 = 3 >> ((D_80082FA0 >= 2 || D_8008FD8C >= 8) ? 1 : 0);
    config->unk34 = 15.0f;
    config->unk38 = 0.8800000548f;
    config->unk3C = -0.3980000317f;
    config->unk40 = 0.3910000324f;
    config->unk44 = 25;
    config->unk46 = 15;
    config->unk48 = 100;
    config->unk4A = 100;
    config->unk4C = 12;
    config->unk4E = 20;
    config->unk50 = (D_80082FA0 >= 2 || D_8008FD8C >= 8) ? -1 : 0;
}

void func_151C0418(vertex *pos, u8 arg1, s32 arg2);
void func_151C04F8(vertex *pos, u8 arg1, s32 arg2);
void func_151C05A4(vertex *pos, u8 arg1, s32 arg2);
void func_151C05F0(vertex *pos, u8 arg1, s32 arg2);
void func_1514FB98(ImpactFx *, u8, s32);
void func_1514FBFC(SurfaceImpactFx *, u8, s32);
void func_1514FCE8(DebrisFx *, u8, s32);
void func_1504715C(f32 *, s32);
s32 func_15046C80(vertex *, s32, f32, f32 *);
void func_151C0360(void *surface, vertex *pos, u8 arg2, s32 arg3);
void func_151C0644(vertex *pos, u8 arg1, s32 arg2);
s32 func_15144E80(HitSurface *, vertex *, vertex *, vertex *);

/* Bullet hit with no surface: sparks and smoke at pos, plus impact particles thrown back along -dir. */
void func_151BFDA0(vertex *pos, vertex *dir, u8 withParticles, u8 arg3, s32 arg4) {
    ImpactFx fx;

    func_151C0418(pos, arg3, arg4);
    func_151C04F8(pos, arg3, arg4);
    func_151C05A4(pos, arg3, arg4);
    func_151C05F0(pos, arg3, arg4);
    if (withParticles) {
        fx.vel.x = -dir->x;
        fx.vel.y = -dir->y;
        fx.vel.z = -dir->z;
        func_151BFC40(&fx.config, &fx.scale);
        fx.config.pos = *pos;
        func_1514FB98(&fx, arg3, arg4);
    }
}

/* Bullet hit against a collision surface (or none). */
void func_151BFE84(HitSurface *surface, vertex *pos, vertex *dir, u8 withParticles, u8 arg4, s32 arg5) {
    vertex v0;
    vertex v1;
    vertex v2;

    // matching: taking the addresses keeps arg4/arg5 in their stack slots (reloaded at each use)
    func_151C0418(pos, *(u8 *)&arg4, *(s32 *)&arg5);
    func_151C04F8(pos, arg4, arg5);
    func_151C05A4(pos, arg4, arg5);
    func_151C05F0(pos, arg4, arg5);
    if (surface != NULL && withParticles) {
        if (func_15144E80(surface, &v0, &v1, &v2) != 0 && v2.y < 0.0f) {
            func_151C0644(pos, arg4, arg5);
        }
    }
    if (surface != NULL && withParticles) {
        func_151C0360(surface, pos, arg4, arg5);
    }
    if (withParticles) {
        if (surface != NULL) {
            SurfaceImpactFx fx;

            fx.vel.x = -dir->x;
            fx.vel.y = -dir->y;
            fx.vel.z = -dir->z;
            fx.surface = *surface;
            fx.config.pos = *pos;
            func_151BFC40(&fx.config, &fx.scale);
            func_1514FBFC(&fx, arg4, arg5);
        } else {
            ImpactFx fx;

            fx.vel.x = -dir->x;
            fx.vel.y = -dir->y;
            fx.vel.z = -dir->z;
            func_151BFC40(&fx.config, &fx.scale);
            fx.config.pos = *pos;
            func_1514FB98(&fx, arg4, arg5);
        }
    }
}

/* Bullet hit on an object: sparks and debris at pos, plus a ground mark below it if hitObj is set. */
void func_151C0098(vertex *pos, s32 hitObj, u8 withParticles, u8 arg3, s32 arg4) {
    DebrisFx fx;
    vertex probe;
    f32 floor[9];
    vertex markPos;

    func_151C0418(pos, arg3, arg4);
    func_151C04F8(pos, arg3, arg4);
    func_151C05A4(pos, arg3, arg4);
    func_151C05F0(pos, arg3, arg4);

    // matching: taking the address keeps withParticles in its stack slot (reloaded at each use)
    if (*(u8 *)&withParticles) {
        fx.unk00 = 0;
        fx.unk02 = 255;
        fx.unk04 = -64;
        fx.unk06 = 71;
        fx.unk08 = 6;
        fx.unk0C = 4;
        fx.pos = *pos;
        fx.unk1C = 23.0f;
        fx.unk20 = 30.0f;
        fx.unk24 = 45.0f;
        fx.unk28 = 53.0f;
        fx.unk2C = 203.0f;
        fx.unk30 = 414.0f;
        fx.unk34 = 7;
        fx.unk38 = 3;
        fx.unk3C = 15.0f;
        fx.unk40 = 0.8720000386f;
        fx.unk44 = -0.3980000317f;
        fx.unk48 = 0.3640000224f;
        fx.unk4C = 25;
        fx.unk4E = 15;
        fx.unk50 = 100;
        fx.unk52 = 100;
        fx.unk54 = 12;
        fx.unk56 = 20;
        fx.unk58 = 0;
        func_1514FCE8(&fx, arg3, arg4);
    }

    if (withParticles && hitObj != 0) {
        probe.x = pos->x;
        probe.y = pos->y + 100.0f;
        probe.z = pos->z;
        func_1504715C(floor, hitObj);
        if (func_15046C80(&probe, 0, pos->y - 1000.0f, floor) != 0) {
            markPos.x = probe.x;
            markPos.y = floor[0];
            markPos.z = probe.z;
            func_151C0360(&floor[1], &markPos, arg3, arg4);
            func_151C0644(pos, arg3, arg4);
        }
    }
}

/* Whether obj is a valid target: not in the ignore list, not self, active, and with a valid id. */
s32 func_151C02E4(struct127 *obj, struct127 *self, s32 ignoreCount, struct127 **ignore) {
    u8 i;

    for (i = 0; i < ignoreCount; i++) {
        if (obj == ignore[i]) {
            return FALSE;
        }
    }
    if (obj == self) {
        return FALSE;
    }
    if (obj->interaction_state == 0) {
        return FALSE;
    }
    if (obj->id == 0xFF) {
        return FALSE;
    }
    return TRUE;
}

void func_150E7FEC(f32, s32, void *, vertex *, s32, s32, s32, s32, s32, s32, s32, s32);

void func_151C0360(void *surface, vertex *pos, u8 arg2, s32 arg3) {
    // matching: the f64 member gives the random value an 8-byte aligned stack slot
    union { u32 w; f64 align; } shade;
    f32 rand;
    u32 duration;

    rand = func_150ADA68();
    shade.w = func_150ADA20();
    duration = func_150ADA20();
    func_150E7FEC(rand * 75.0f + 75.0f, (u8)(shade.w % 56 + 200), surface, pos,
                  duration % 205 + 299, 1, 1, 0, 0, 0, arg2, 0);
}

s32 func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);

void func_151C0418(vertex *pos, u8 arg1, s32 arg2) {
    Header header;
    Header2 header2;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = func_150ADA20() % 11 + 20;
    header.unk4 = 0;

    header2.unk0 = pos->x;
    header2.unk4 = pos->y;
    header2.unk8 = pos->z;

    func_151602C0(&header, &header2, func_150ADA20() % 121 + 60, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg1, arg2);
}

void func_151C04F8(vertex *pos, u8 arg1, s32 arg2) {
    // matching: the f64 member gives the random value an 8-byte aligned stack slot
    union { f32 f; f64 align; } rand;
    u32 shade;

    rand.f = func_150ADA68();
    shade = func_150ADA20();
    // 0x3F974EB9 is 1.18209f, passed in a GPR as its bit pattern
    func_151541B8(pos, rand.f * 4.0f + 14.0f, 0x3F974EB9, (f32)(shade % 56 + 200), 0.0f, arg1, arg2);
}

void func_151D5334(vertex *, f32, f32, f32, s32, u8, s32);

void func_151C05A4(vertex *pos, u8 arg1, s32 arg2) {
    func_151D5334(pos, 800.0f, 2000.0f, 0.0005f, 5, arg1, arg2);
}

void func_151C05F0(vertex *pos, u8 arg1, s32 arg2) {
    func_151D5404((struct17 *)pos, 800.0f, 2000.0f, 0.0005f, 15, 20, arg1, arg2);
}

void func_150E83AC(vertex *, s16, u8, s32);

void func_151C0644(vertex *pos, u8 arg1, s32 arg2) {
    func_150E83AC(pos, func_150ADA20() % 62 + 120, arg1, arg2);
}

typedef struct {
    /* 0x00 */ u8 pad00[0x4F];
    /* 0x4F */ u8 unk4F;
} Obj151C1D5C;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ HitSurface unk04;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s8  unk1C;
    /* 0x1D */ s8  unk1D;
    /* 0x20 */ Obj151C1D5C *unk20;
} Res151C1D5C;

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ vertex unk08;
    /* 0x14 */ vertex unk14;
    /* 0x20 */ vertex unk20;
    /* 0x2C */ vertex unk2C;
    /* 0x38 */ vertex unk38;
    /* 0x44 */ HitSurface unk44;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8  unk58;
    /* 0x59 */ u8  unk59;
    /* 0x5A */ u8  unk5A;
    /* 0x5B */ u8  pad5B;
    /* 0x5C */ Obj151C1D5C *unk5C;
    /* 0x60 */ s32 unk60;
} Ray151C196C;

typedef struct {
    /* 0x000 */ u8 pad0[0x130];
    /* 0x130 */ vertex unk130;
    /* 0x13C */ vertex unk13C;
    /* 0x148 */ u8 pad148[0x4F];
    /* 0x197 */ u8 unk197;
} Cam151C0698;

typedef struct {
    /* 0x000 */ u8 pad0[0x23D];
    /* 0x23D */ u8 unk23D;
} Ctl151C0698;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ u8 unk04;
    /* 0x005 */ u8 pad5[0xF];
    /* 0x014 */ f32 unk14;
    /* 0x018 */ f32 unk18;
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ u8 pad20[0x45];
    /* 0x065 */ u8 unk65;
    /* 0x066 */ u8 pad66[0xE];
    /* 0x074 */ u8 unk74;
    /* 0x075 */ u8 pad75[0x15F];
    /* 0x1D4 */ s32 unk1D4;
    /* 0x1D8 */ u8 pad1D8[0x140];
    /* 0x318 */ Ctl151C0698 *unk318;
    /* 0x31C */ Cam151C0698 *unk31C;
} Actor151C0698;

typedef struct {
    s32 w[4];
} Blk151C0698;

typedef struct {
    /* 0x000 */ Ray151C196C ray;
    /* 0x064 */ f32 unk64;
    /* 0x068 */ f32 unk68;
    /* 0x06C */ Actor151C0698 *unk6C;
    /* 0x070 */ Blk151C0698 unk70;
    /* 0x080 */ struct102 *unk80;
    /* 0x084 */ struct102 *unk84;
    /* 0x088 */ s32 unk88;
    /* 0x08C */ s32 unk8C;
    /* 0x090 */ u16 unk90;
    /* 0x092 */ u8 unk92;
    /* 0x093 */ u8 pad93;
    /* 0x094 */ f32 unk94;
    /* 0x098 */ vertex unk98;
    /* 0x0A4 */ Res151C1D5C unkA4;
    /* 0x0C8 */ u8 unkC8;
    /* 0x0C9 */ u8 padC9[3];
    /* 0x0CC */ s32 unkCC;
} Spawn151C0698;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex unk40;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ u8 pad76[6];
} Desc151C0698;

f32 func_15144AA8(u8, void *);
void func_15145740(Actor151C0698 *, vertex *, void *, s32, f32);
s32 func_151D5B6C(vertex *, vertex *, Actor151C0698 *, s32, Blk151C0698 *);
void *func_15132A4C(Desc151C0698 *, s32, s32, s32, u8, s32);
void func_15145974(vertex *, f32 *, f32 *);
s32 func_151B8908(void *);
s32 func_151B8400(void *);
s32 func_151B8668(void *, u8, s32);
void func_15145EA4(vertex **, vertex **, s32, s32);
void func_15102B38(Actor151C0698 *, s32, vertex *, vertex *, f32 *, s32, s32, f32,
                   vertex *, s32, s32, s32, u8, s32);
void func_15103254(s16, u8, f32, vertex *, s32, u8, s32);
void func_151C1798(struct127 *arg0, s32 arg1);
void func_151C1860(f32 *arg0, u8 arg1, s32 arg2);
s32 func_151C196C(Ray151C196C *arg0, vertex *arg1, vertex *arg2, vertex *arg3,
                  vertex *arg4, s32 arg5, s32 arg6, vertex *arg7, f32 *arg8,
                  Res151C1D5C *arg9, s32 arg10);
s32 func_151C110C();
#define RAND() func_150ADA20()

void func_151C0698(Actor151C0698 *arg0, Actor151C0698 *arg1, Blk151C0698 *arg2, u8 arg3, u8 arg4, s32 arg5) {
    Ray151C196C ray;
    f32 depth;
    vertex hitPos;
    Res151C1D5C res;
    u8 useCam;
    vertex pos;
    vertex dir;
    vertex camPos;
    vertex camDir;
    s32 mode;
    vertex *src;
    Actor151C0698 *who;
    f32 angle;
    u8 sp218[0x14];
    Spawn151C0698 spawn;
    Desc151C0698 desc;
    void *obj;
    Spawn151C0698 *p;
    Header header;
    Header2 header2;

    depth = 0.0f;
    if (D_800BE616 != 0) {
        if (arg0->unk1D4 != 0) {
            func_151C1798((struct127 *) arg0, (s32) &pos);
        } else {
            pos.x = arg0->unk14;
            pos.y = arg0->unk18 + 65.0f;
            pos.z = arg0->unk1C;
        }
    } else {
        src = &arg0->unk31C->unk13C;
        obj = arg0->unk318;
        if (obj != NULL) {
            if (arg0->unk65 != 0) {
lbl1:
                pos.x = src->x;
                pos.y = src->y + -13.0f;
                pos.z = src->z;
            } else if (arg0->unk31C->unk197 != 0) {
                extern u8 D_800BE616;

                angle = func_15144AA8(((Ctl151C0698 *) obj)->unk23D, obj) * 0.01745329238f;
                pos.x = cosf(angle) * 25.0f + src->x;
                pos.y = src->y;
                pos.z = src->z - sinf(angle) * 25.0f;
            } else if (arg0->unk1D4 != 0) {
                func_151C1798((struct127 *) arg0, (s32) &pos);
            } else {
                pos.x = arg0->unk14;
                pos.y = arg0->unk18 + 65.0f;
                pos.z = arg0->unk1C;
            }
        } else if (arg0->unk1D4 != 0) {
            func_151C1798((struct127 *) arg0, (s32) &pos);
        } else {
            pos.x = arg0->unk14;
            pos.y = arg0->unk18 + 65.0f;
            pos.z = arg0->unk1C;
        }
    }

    if ((arg0->unk31C != NULL && arg0->unk31C->unk197 != 0) || arg1 != NULL) {
        extern u8 D_800BE616;

        who = (arg1 != NULL) ? arg1 : arg0;

        useCam = 1;
        src = &who->unk31C->unk13C;
        camPos = *src;
        camDir.x = who->unk31C->unk130.x;
        camDir.y = who->unk31C->unk130.y;
        camDir.z = who->unk31C->unk130.z;
    } else {
        func_15145740(arg0, &dir, sp218, 0, 0.0f);
        useCam = 0;
        dir.x *= 0.001000000047f;
        dir.y *= 0.001000000047f;
        dir.z *= 0.001000000047f;
    }

    if (D_800BE9F0 == 0x24) {
        mode = 0;
    } else {
        mode = func_151D5B6C(useCam ? &camPos : &pos, useCam ? &camDir : &dir, arg0, 3, arg2);
    }

    if (func_151C196C(&ray, &pos, useCam ? NULL : &dir, useCam ? &camPos : NULL,
                      useCam ? &camDir : NULL, (s32) arg0, (s32) arg2, &hitPos, &depth, &res,
                      mode) != 0) {
        extern u8 D_800BE616;

        spawn.unkCC = mode;
        spawn.unk80 = NULL;
        spawn.ray = ray;
        spawn.unk84 = NULL;
        desc.unk00 = 1.0f;
        desc.unk04 = 1.0f;
        desc.unk08 = 0.04700000212f;
        desc.unk0C = 0.04700000212f;
        desc.unk10 = 0.0f;
        desc.unk14 = 0.0f;
        desc.unk18 = 0.0f;
        desc.unk1C = 1.0f;
        desc.unk20 = 1.0f;
        desc.unk24 = 1.0f;
        desc.pos = pos;
        desc.unk4C = 0.0f;
        desc.unk50 = 0x500920;
        desc.unk54 = 0x12C;
        desc.unk56 = 0x2F;
        desc.unk58 = 1;
        spawn.unk6C = arg0;
        spawn.unk70 = *arg2;
        spawn.unk94 = depth * 0.004999999888f;
        spawn.unk98 = hitPos;
        spawn.unkC8 = (s8)((depth != 0) ? 1 : 0);
        spawn.unkA4 = res;
        spawn.unk68 = 200.0f;
        desc.vel.x = spawn.ray.unk38.x * spawn.unk68;
        desc.vel.y = spawn.ray.unk38.y * spawn.unk68;
        desc.vel.z = spawn.ray.unk38.z * spawn.unk68;
        func_15145974(&desc.vel, &desc.unk14, &desc.unk10);
        desc.unk18 = 0.0f;
        desc.unk40.x = 0.0f;
        desc.unk40.y = 0.0f;
        desc.unk40.z = 0.0f;
        spawn.unk64 = spawn.ray.unk04 * 0.004999999888f;
        spawn.unk92 = arg3;
        desc.unk5C = 0;
        desc.unk60 = 0xFF;
        desc.unk61 = 0xA;
        desc.unk62 = 0;
        desc.unk63 = 0;
        desc.unk64 = 0;
        desc.unk65 = 0;
        desc.unk66 = 0;
        desc.unk67 = 0;
        desc.unk68 = 2;
        desc.unk6A = 2;
        desc.unk6C = 0;
        desc.unk70 = 0;
        desc.unk72 = 1;
        desc.unk74 = 0xFF;
        obj = func_15132A4C(&desc, 3, 0xFF, 0xD0, arg4, arg5);
        if (obj != NULL) {
            p = (Spawn151C0698 *) ((u8 *) obj + 0x170);
            memcpy(p, &spawn, 0xD0);
            header.unk0 = 2;
            header.unk1 = -1;
            header.unk2 = 0x12C;
            header.unk4 = 0;
            header2.unk0 = (s32) pos.x;
            header2.unk4 = (s32) pos.y;
            header2.unk8 = (s32) pos.z;
            p->unk80 = (struct102 *) func_151602C0(&header, &header2, 0x28, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg4, arg5);
            p->unk84 = (struct102 *) func_151B8908(obj);
            p->unk90 = func_1000FA64((RAND() % 3U) + 0x2EE, pos.x, pos.y, pos.z, 0x7530, 0x3E8, 0x1F4, (s32) func_151C110C, obj, 0, 4, 0);
            p->unk88 = func_151B8400(obj);
            if (D_80082FA0 < 2 && D_8008FD8C < 4) {
                p->unk8C = func_151B8668(obj, arg4, arg5);
            } else {
                extern u8 D_800BE616;

                p->unk8C = 0;
            }
        } else {
            if (spawn.unk80 != NULL) {
                func_1516972C(spawn.unk80);
            }
            if (spawn.unk84 != NULL) {
                func_1516972C(spawn.unk84);
            }
        }
        if (arg0->unk1D4 != 0 && (arg0->unk74 & 0xF) != 0xF) {
            if (arg0->unk04 != 0x77 && arg0->unk04 != 0x28) {
                const vertex a = { 3.0f, 125.0f, 35.0f };
                const vertex b = { 6.0f, -50.0f, 35.0f };
                const vertex c = { 3.0f, 200.0f, 35.0f };
                f32 scl[2];
                vertex outA;
                vertex outB;
                vertex *in[2];
                vertex *out[2];

                scl[0] = func_150ADA68() * 10.0f + 25.0f;
                scl[1] = func_150ADA68() * 1.009000063f + 2.013000011f;
                in[0] = &a;
                in[1] = &b;
                out[0] = &outA;
                out[1] = &outB;
                func_15145EA4(in, out, arg0->unk1D4 + 0x240, 2);
                func_15102B38(arg0, 9, &a, &c, scl, (RAND() % 3U) + 8,
                              (RAND() % 56U) + 200, func_150ADA68() * 204.0f + 601.0f,
                              &outA, 0xFF, 0, -1, arg4, arg5);
                func_15103254((RAND() % 3U) + 8, (RAND() % 56U) + 200,
                              func_150ADA68() * 204.0f + 601.0f, &outB, 0xFF, arg4, arg5);
                func_151C1860((f32 *) &outB, arg4, arg5);
            }
        }
    }
}

typedef struct {
    u8 pad0[0x38];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
} Struct151C110CInner;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u8 pad8[0x10];
    Struct151C110CInner *unk18;
} Struct151C110C;

s32 func_151C110C(Struct151C110C *arg0, s32 arg1, void **arg2, s32 arg3, s32 arg4, s32 arg5, s16 *arg6) {
    Struct151C110CInner *temp_v1;
    s16 *temp_t3;

    temp_v1 = arg0->unk18;
    temp_t3 = arg6;
    if (temp_v1 != NULL) {
        if (*arg2 != NULL) {
            arg0->unk2 = temp_v1->unk38;
            arg0->unk4 = temp_v1->unk3C;
            arg0->unk6 = temp_v1->unk40;
            return 0;
        }
    }

    *temp_t3 = 0;
    return 0;
}

typedef struct Model151C1180 {
    u8 pad0[0x14];
    s16 *unk14;
} Model151C1180;

typedef struct Fx151C1180 {
    u8 pad0[0x40];
    vertex unk40;
} Fx151C1180;

typedef struct {
    /* 0x000 */ Ray151C196C ray;
    /* 0x064 */ f32 unk64;
    /* 0x068 */ f32 unk68;
    /* 0x06C */ s32 unk6C;
    /* 0x070 */ u8 unk70[0x10];
    /* 0x080 */ struct Model151C1180 *unk80;
    /* 0x084 */ struct Fx151C1180 *unk84;
    /* 0x088 */ u8 pad88[0xA];
    /* 0x092 */ u8 unk92;
    /* 0x093 */ u8 pad93;
    /* 0x094 */ f32 unk94;
    /* 0x098 */ u8 unk98[0xC];
    /* 0x0A4 */ u8 unkA4[0x24];
    /* 0x0C8 */ u8 unkC8;
    /* 0x0C9 */ u8 padC9[3];
    /* 0x0CC */ s32 unkCC;
} Sub151C1180;

typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0x2B];
    /* 0x038 */ vertex pos;
    /* 0x044 */ vertex vel;
    /* 0x050 */ u8 pad50[0x120];
    /* 0x170 */ Ray151C196C ray;
} Obj151C1180;


void func_15055A2C(s32, f32, f32, f32, s32);
void func_151C1654(vertex *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_15081690(s32, f32, f32, f32, f32, f32, f32, Ray151C196C *, f32, s32, s32, s32, s32,
                   s32, s32);
s32 func_151ABE40(void *, void *, s32, s32, s32);

s32 func_151C1180(Obj151C1180 *arg0)
{
  Sub151C1180 *sub;
  f32 scale;
  vertex spD4;
  vertex spC8;
  Ray151C196C sp64;
  Model151C1180 *model;
  sub = (Sub151C1180 *) (&arg0->ray);
  if (sub->unk64 < D_800BE9A4)
  {
    scale = sub->unk68 * sub->unk64;
  }
  else
  {
    scale = sub->unk68 * D_800BE9A4;
  }
  spD4.x = sub->ray.unk38.x * scale;
  spD4.y = sub->ray.unk38.y * scale;
  spD4.z = sub->ray.unk38.z * scale;
  if (D_800E0930 != 0)
  {
    if (((s32 (*)(vertex *, vertex *, vertex *)) D_800E0930)(&arg0->pos, &spD4, &spC8) != 0)
    {
      func_15055A2C(0, spC8.x, spC8.y, spC8.z, 0);
      func_151C1654((vertex *) (&spC8), (s32) (&sub->ray.unk38), sub->unk6C, sub->unk92, 0);
      func_151BFDA0((vertex *) (&spC8), (vertex *) (&sub->ray.unk38), !(sub->unkC8 & 1), arg0->unkC, arg0->unk1);
      return 0;
    }
  }
  if (scale)
  {
    fence:
    func_15081690(sub->unk6C, arg0->pos.x, arg0->pos.y, arg0->pos.z, spD4.x, spD4.y, spD4.z, &sp64, scale, 1, 0, 1, 3, (s32) sub->unk70, sub->unkCC);

    if (sp64.unk59 >= 2)
    {
      func_15055A2C(1, sp64.unk08.x, sp64.unk08.y, sp64.unk08.z, 0);
      func_151C1654((vertex *) (&sp64.unk08), (s32) (&sub->ray.unk38), sub->unk6C, sub->unk92, 0);
      func_151C0098((vertex *) (&sp64.unk08), sp64.unk00, !(sub->unkC8 & 1), arg0->unkC, arg0->unk1);
      return 0;
    }
  }
  arg0->pos.x += arg0->vel.x * D_800BE9A4;
  arg0->pos.y += arg0->vel.y * D_800BE9A4;
  arg0->pos.z += arg0->vel.z * D_800BE9A4;
  if (sub->unkC8 & 1)
  {
    if (sub->unk94 > 0.0f)
    {
      sub->unk94 -= D_800BE9A4;
      if (sub->unk94 <= 0.0f)
      {
        func_151ABE40(sub->unk98, sub->unkA4, 2, arg0->unkC, arg0->unk1);
      }
    }
  }
  sub->unk64 -= D_800BE9A4;
  if (sub->unk64 <= 0.0f)
  {
    if (sub->ray.unk59 == 1)
    {
      func_15055A2C(0, sub->ray.unk08.x, sub->ray.unk08.y, sub->ray.unk08.z, 0);
      func_151C1654((vertex *) (&sub->ray.unk08), (s32) (&sub->ray.unk38), sub->unk6C, sub->unk92, (s32) (&sub->ray.unk44));
      func_151BFE84((HitSurface *) (&sub->ray.unk44), (vertex *) (&sub->ray.unk08), (vertex *) (&arg0->vel), !(sub->unkC8 & 1), arg0->unkC, arg0->unk1);
    }
    return 0;
  }
  model = sub->unk80;
  if (model != 0)
  {
    model->unk14[7] = arg0->pos.x;
    if ((arg0->unk1 && arg0->unk1) && arg0->unk1)
    {
    }
    model->unk14[8] = arg0->pos.y;
    model->unk14[9] = arg0->pos.z;
  }
  if (sub->unk84 != 0)
  {
    sub->unk84->unk40 = arg0->pos;
  }
  return 1;
}

typedef struct Sub151C1570 {
    char pad0[0x80];
    /* 0x80 */ void *unk80;
    /* 0x84 */ void *unk84;
    /* 0x88 */ u8 *unk88;
    /* 0x8C */ void *unk8C;
} Sub151C1570;

void func_151C1570(void *arg0) {
    Sub151C1570 *p;

    p = (Sub151C1570 *)((u8 *)arg0 + 0x170);
    if (p->unk80 != NULL) {
        func_1516972C(p->unk80);
    }
    if (p->unk84 != NULL) {
        func_1516972C(p->unk84);
    }
    if (p->unk88 != NULL) {
        s32 *q;

        q = (s32 *)(p->unk88 + 0x110);
        *q = 0;
    }
    if (p->unk8C != NULL) {
        func_1516972C(p->unk8C);
    }
    func_1000FD38(func_151C110C, arg0, 0);
}

void func_151C1570(void *);
void func_15132570(void *);

void func_151C15FC(void *arg0) {
    func_151C1570(arg0);
    func_15132570(arg0);
}

void func_1513259C(void *);

void func_151C1628(void *arg0) {
    func_151C1570(arg0);
    func_1513259C(arg0);
}

void func_151D40D4(vertex *, s32, s32, s32, s32, s32, s32, s32);
void func_151D42E8(vertex *, s32, s32, s32, s32);

void func_151C1654(vertex *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    void (*callback)(s32, s32, s32);
    u8 temp;

    callback = (void (*)(s32, s32, s32)) D_800E0934;
    if (callback != NULL) {
        callback((s32)arg0->x, (s32)arg0->y, (s32)arg0->z);
    }

    temp = (u8)arg3;
    if (temp != 0) {
        switch (temp) {
        case 1:
            func_151D42E8(arg0, arg1, arg2, arg4, 0x24);
            return;
        case 2:
            func_151D42E8(arg0, arg1, arg2, arg4, 0x25);
            return;
        case 3:
            func_151D40D4(arg0, arg1, arg2, 0, arg4, 0x16, 0x26, 0);
            return;
        }
    }

    func_151D40D4(arg0, arg1, arg2, 0, arg4, 0x16, 0x15, 0);
}

void func_15143134(void *, s32, s32);
struct S800AA958 { s32 unk0; s32 unk4; s32 unk8; };
extern u8 D_800AA954[];
extern struct S800AA958 D_800AA958[];

void func_151C1798(struct127 *arg0, s32 arg1) {
    s32 index;

    switch (arg0->id) {
    case 0x77:
        index = 1;
        break;
    case 0x28:
        index = 2;
        break;
    default:
        index = 0;
        break;
    }
    func_15143134(&D_800AA958[index], arg1,
                  (s32)arg0->unk1D4 + (D_800AA954[index] << 6));
}

void func_151C1814(s32 *arg0, s32 *arg1, u8 arg2) {
    s32 *v0 = (s32 *)((u8 *)arg0 + 0x170);
    if (arg2 == 0x2D) {
        if (arg1[0] == v0[0x1B]) {
            v0[0x1B] = arg1[1];
        } else if (arg1[1] == v0[0x1B]) {
            v0[0x1B] = arg1[0];
        }
    }
}

void func_151C1860(f32 *arg0, u8 arg1, s32 arg2) {
    Header header;
    Header2 header2;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() % 5U) + 5;
    header.unk4 = 0;

    header2.unk0 = (s32) arg0[0];
    header2.unk4 = (s32) arg0[1];
    header2.unk8 = (s32) arg0[2];

    func_151602C0(&header, &header2, (func_150ADA20() % 0xBU) + 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg1, arg2);
}

typedef struct {
    /* 0x00 */ s32 count;
    /* 0x04 */ struct127 *objs[1];
} IgnoreList;

/* Target filter callback: func_151C02E4 with the ignore list packed in a struct. */
s32 func_151C1940(struct127 *obj, struct127 *self, IgnoreList *ignore) {
    return func_151C02E4(obj, self, ignore->count, ignore->objs);
}


void func_151C1D5C(s32, vertex *, vertex *, Ray151C196C *, f32, s32, s32, s32, s8,
                   s32, vertex *, f32 *, Res151C1D5C *, s32);
s32 func_15145128(vertex *, vertex *, f32 *, f32 *);

s32 func_151C196C(Ray151C196C *arg0, vertex *arg1, vertex *arg2, vertex *arg3,
                  vertex *arg4, s32 arg5, s32 arg6, vertex *arg7, f32 *arg8,
                  Res151C1D5C *arg9, s32 arg10) {
    u8 trace;
    u8 hit;
    vertex *src;

    trace = 1;
    hit = 0;

    if ((arg3 != NULL) && (arg4 != NULL)) {
        arg0->unk2C = *arg3;
        arg0->unk38 = *arg4;
        func_151C1D5C(arg5, &arg0->unk2C, &arg0->unk38, arg0, 0.0f, 0, 0, 1, 3, arg6, arg7,
                      arg8, arg9, arg10);

        if (arg0->unk59 == 0) {
            if ((arg1->x != arg0->unk2C.x) || (arg1->y != arg0->unk2C.y) ||
                (arg1->z != arg0->unk2C.z)) {
                arg0->unk2C = *arg1;
                arg0->unk38 = *arg4;
            } else {
                trace = 0;
            }
        } else {
            if ((arg1->x != arg0->unk2C.x) || (arg1->y != arg0->unk2C.y) ||
                (arg1->z != arg0->unk2C.z)) {
                arg0->unk2C = *arg1;
                arg0->unk38.x = arg0->unk08.x - arg1->x;
                arg0->unk38.y = arg0->unk08.y - arg1->y;
                arg0->unk38.z = arg0->unk08.z - arg1->z;
                if (func_15145128(&arg0->unk38, &arg0->unk38, &arg0->unk04, NULL) == 0) {
                    return 0;
                }
            }
        }
    } else {
        arg0->unk2C = *arg1;
        arg0->unk38 = *arg2;
    }

    if (trace != 0) {
        func_151C1D5C(arg5, &arg0->unk2C, &arg0->unk38, arg0, 0.0f, 0, 1, 1, 3, arg6, arg7,
                      arg8, arg9, arg10);
        if (arg0->unk59 != 0) {
            hit = 1;
        }
    }

    if (hit == 0) {
        arg0->unk00 = 0;
        arg0->unk04 = 5000.0f;
        arg0->unk2C = *arg1;
        if (arg4 != NULL) {
            src = arg4;
        } else {
            src = arg2;
        }
        arg0->unk38 = *src;
        arg0->unk08.x = arg0->unk2C.x + arg0->unk38.x * 5000.0f;
        arg0->unk08.y = arg0->unk2C.y + arg0->unk38.y * 5000.0f;
        arg0->unk08.z = arg0->unk2C.z + arg0->unk38.z * 5000.0f;
        arg0->unk14 = arg0->unk08;
        arg0->unk20 = arg0->unk08;
        arg0->unk56 = 0;
        arg0->unk58 = 0;
        arg0->unk59 = 0;
        arg0->unk5A = 0;
        arg0->unk5C = NULL;
        arg0->unk60 = 0;
    }
    return 1;
}

void func_15081690(s32, f32, f32, f32, f32, f32, f32, Ray151C196C *, f32, s32, s32, s32, s32,
                   s32, s32);

void func_151C1D5C(s32 arg0, vertex *arg1, vertex *arg2, Ray151C196C *arg3, f32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s8 arg8, s32 arg9, vertex *arg10,
                   f32 *arg11, Res151C1D5C *arg12, s32 arg13) {
    Obj151C1D5C *obj;
    vertex pos;
    f32 dist;
    s32 more;

    dist = 0.0f;
    *arg11 = 0.0f;
    pos = *arg1;

    do {
        func_15081690(arg0, pos.x, pos.y, pos.z, arg2->x, arg2->y, arg2->z, arg3, arg4, arg5,
                      arg6, arg7, arg8, arg9, arg13);
        dist += arg3->unk04;
        if (arg3->unk5C != NULL) {
            Obj151C1D5C *p = arg3->unk5C;

            if ((p->unk4F & 0x60) == 0x40) {
                pos.x = arg3->unk08.x + arg3->unk38.x * 0.1f;
                obj = p;
                pos.y = arg3->unk08.y + arg3->unk38.y * 0.1f;
                pos.z = arg3->unk08.z + arg3->unk38.z * 0.1f;
                *arg10 = arg3->unk08;
                *arg11 = dist;
                dist += 0.1f;
                arg12->unk00 = arg3->unk08.y;
                arg12->unk04 = arg3->unk44;
                arg12->unk18 = arg3->unk60;
                arg12->unk1C = 7;
                arg12->unk1D = 3;
                arg12->unk20 = obj;
                more = 1;
            } else {
                more = 0;
            }
        } else {
            more = 0;
        }
    } while (more);

    arg3->unk2C = *arg1;
    arg3->unk04 = dist;
}

struct sp18_151C1FB8 {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ u8 unk7;
};

extern void func_151D8868(void *, s32, s32, s32);

void func_151C1FB8(struct127 *arg0) {
    struct sp18_151C1FB8 sp18;

    if (arg0->camera != 0) {
        sp18.unk0 = 1;
        sp18.unk2 = (func_150ADA20() & 7) + 0xD;
        sp18.unk5 = 1 << arg0->camera->unk23D;
        sp18.unk4 = (func_150ADA20() % 3U) + 6;
        sp18.unk6 = -1;
        func_151D8868(&sp18, 0, 0xFF, 1);
    }
}
