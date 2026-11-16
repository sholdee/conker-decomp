#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_proto
#define func_10010F30 func_10010F30_void
#include "functions.h"
#undef func_150ADA20
#undef func_10010F30
#include "variables.h"

typedef void (*GameFunc151C455CDispatch)(s32);
typedef void (*GameFunc151C455CCallback)(struct17 *, s32);

extern GameFunc151C455CDispatch D_8008FBD0[];
extern s32 func_150ADA20(void);
extern s32 func_10010F30(s32, u16, u8, s16, u8);
extern s32 func_1000B060(f32, f32, u32);
extern void func_1000E7A0(u32, s32);
extern s32 func_1510F8CC(s32);
extern f32 D_800AAA30;
extern f32 D_800AAA34;
extern f32 D_800AAA7C;
extern f32 D_800AAA80;
extern f32 D_800AAA84;

typedef struct {
    u8 pad0[0x2C];
    s16 unk2C;
} GameStruct151C2050State;

void func_151C2050(struct127 *arg0, f32 *arg1, f32 *arg2, s32 arg3, f32 arg4) {
    s32 sp4C;
    s32 sp48;
    struct108 *camera;
    s32 sp40;
    s16 temp_v0;
    f32 sp34;
    f32 temp_f0;

    if (arg0 != 0) {
        if ((D_800BE616 != 0) || ((camera = arg0->camera) == 0)) {
            temp_v0 = ((GameStruct151C2050State *)D_800B0DF0)->unk2C;
            if ((temp_v0 == 0x24) || (temp_v0 == 0x13) || (temp_v0 == 0x4D) || (temp_v0 == 0x85) || (temp_v0 == 0x93)) {
                func_100114D0((s32)arg1[0], (s32)arg1[1], (s32)arg1[2], 0x7FFF, 0x3E8, 0x64, &sp4C, &sp48, 0);
                if ((u32)sp48 >= 0x1001U) {
                    sp48 = (u32)sp48 >> 7;
                    func_1000E7A0(8, (func_1510F8CC(arg3) + 1) | (sp48 << 8) | (sp4C << 16));
                }
            }
        } else {
            temp_v0 = ((GameStruct151C2050State *)D_800B0DF0)->unk2C;
            if ((temp_v0 == 0x24) || (temp_v0 == 0x13)) {
                temp_f0 = 255.0f - (arg4 * D_800AAA30);
                if (temp_f0 > 16.0f) {
                    sp34 = temp_f0;
                    sp40 = func_1000B060(arg2[0], arg2[2], (u32)(camera->unk3A0 * D_800AAA34));
                    func_1000E7A0(8, (func_1510F8CC(arg3) + 1) | ((s32)sp34 << 8) | (sp40 << 16));
                }
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1EF500/func_151C229C.s")

typedef struct {
    u8 unk0;
    u8 pad1[0x5F];
    f32 unk60;
    f32 unk64;
    f32 unk68;
    u8 pad6C[0x20];
    f32 unk8C;
    s32 unk90;
    u8 pad94[4];
    f32 unk98;
    u8 pad9C[0xC];
    s8 unkA8;
    u8 padA9[3];
    s32 unkAC;
} Mover151C43E0;

typedef struct {
    u8 pad0[0x34];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
} GameStruct151C4510a;

typedef struct {
    u8 pad0[0x4];
    f32 unk4;
    f32 unk8;
    f32 unkC;
} GameStruct151C4510b;

typedef struct {
    u8 unk0;
    u8 pad1[0x83];
    struct131 *unk84;
    u8 pad88[0x4];
    f32 unk8C;
    u8 pad90[0xC];
    s8 unk9C;
} GameStruct151C455C;

typedef struct {
    u16 unk0;
    u16 unk2;
} GameStruct151C2734Sfx;

typedef struct {
    u8 pad0[0x60];
    f32 unk60;
    f32 unk64;
    f32 unk68;
    u8 pad6C[0x20];
    f32 unk8C;
    u8 pad90[0x8];
    f32 unk98;
    u8 pad9C[0x2];
    u16 unk9E;
    u8 unkA0;
    u8 unkA1;
} GameStruct151C2734Sub;

typedef struct {
    u8 pad0[0x34];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u8 pad40[0xD0];
    GameStruct151C2734Sub unk110;
} GameStruct151C2734;

extern GameStruct151C2734Sfx D_8008FBE8[];
extern s32 func_1000A420(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32 *, s32 *, s32);
void func_151C436C(s32 a0, f32 *a1, s32 a2);
u8 func_151C43E0(struct108 *arg0, Mover151C43E0 *arg1, f32 arg2);
void func_151C4510(GameStruct151C4510a *a0, GameStruct151C4510b *a1, f32 a2);
s32 func_151C455C(s32 arg0, GameStruct151C455C *arg1, f32 arg2);

s8 func_151C2734(GameStruct151C2734 *arg0) {
    s8 ret;
    GameStruct151C2734Sub *sub;
    struct108 *found;
    s32 sound;
    s32 r;
    s32 i;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dot;
    f32 dist;
    s32 vol;
    s32 pan;

    sub = &arg0->unk110;
    ret = 1;
    func_151C436C((s32)arg0, (f32 *)sub, D_800BE9E4);
    if (func_151C43E0((struct108 *)arg0, (Mover151C43E0 *)sub, D_800BE9A4) == 0) {
        ret = 0;
    }
    func_151C4510((GameStruct151C4510a *)arg0, (GameStruct151C4510b *)sub, D_800BE9A4);
    if (func_151C455C((s32)arg0, (GameStruct151C455C *)sub, D_800BE9A4) == 0) {
        ret = 0;
    }
    if (sub->unkA0 != 0 && ret == 1) {
        sound = 0;
        found = NULL;
        if (sub->unk9E == 0) {
            r = func_150ADA20() & 7;
            for (i = 0; i <= D_80082FA0; i++) {
                found = &D_800DBFF0[i];
                dx = found->unk2F8 - arg0->unk34;
                dy = found->unk2FC - arg0->unk38;
                dz = found->unk300 - arg0->unk3C;
                dot = sub->unk60 * dx + dy * sub->unk64 + dz * sub->unk68;
                if (dot < (u32)D_8008FBE8[r].unk2 * sub->unk98 && (D_8008FBE8[r].unk2 - 10) * sub->unk98 < dot) {
                    dist = sqrtf(dx * dx + dy * dy + dz * dz);
                    if (dist != 0.0f && dist < sub->unk8C * sub->unk98) {
                        dot /= dist;
                        if (0.8999999762f < dot) {
                            sound = D_8008FBE8[r].unk0;
                            sub->unkA1 = i;
                            break;
                        }
                    }
                }
                found = NULL;
            }
        } else if (func_1000F3D0(sub->unk9E)) {
            found = &D_800DBFF0[sub->unkA1];
        } else {
            sub->unkA0 = 0;
        }
        if (found != NULL) {
            vol = 0x40;
            func_1000A420(arg0->unk34 - found->unk2F8, arg0->unk38 - found->unk2FC, arg0->unk3C - found->unk300,
                          *(s32 *)&found->unk380, arg0->unk34 - found->unk2F8, arg0->unk38 - found->unk2FC,
                          arg0->unk3C - found->unk300, 10000, 9999, &vol, &pan, 0);
            if (sound != 0) {
                sub->unk9E = func_10010F30(sound, 0x6590, vol & 0x7F, 0, vol & 0x80);
            } else {
                func_1000F85C(sub->unk9E, 4, vol & 0x7F);
                func_1000F85C(sub->unk9E, 0x100, vol & 0x80);
            }
        }
    }
    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1EF500/func_151C2AD0.s")

typedef struct {
    s32 unk0;
    u8 unk4;
} GameStruct151C2E4C;

s32 func_151C2E4C(GameStruct151C2E4C *a0, GameStruct151C2E4C *a1) {
    if (a0 == a1) {
        return 0;
    }
    if (a0->unk0 == 0) {
        return 0;
    }
    if (a0->unk4 == 0xFF) {
        return 0;
    }
    return 1;
}

typedef struct {
    s32 unk0;
    u8 unk4;
    u8 pad[0x122];
    u8 unk127;
} GameStruct151C2E94;

s32 func_151C2E94(GameStruct151C2E94 *a0, GameStruct151C2E94 *a1) {
    if (a0 == a1) {
        return 0;
    }
    if (a0->unk0 == 0) {
        return 0;
    }
    if (a0->unk4 == 0xFF) {
        return 0;
    }
    if (a0->unk127 == 0xFF) {
        return 0;
    }
    return 1;
}

s32 func_151D4DAC(s32, s32, s32, s32, s32, s32, void *, s32, s32);

s32 func_151C2EF0(s32 a0, s32 a1, struct108 *a2, s32 a3, s32 arg4, s32 arg5) {
    return func_151D4DAC(a0, a1, a3, arg4, arg5, *(s32 *)((u8 *)a2 + 0x1B4),
                         (u8 *)a2 + 0x170, *((u8 *)a2 + 0xC), *((u8 *)a2 + 0x1));
}

extern f32 D_800AAA3C;
extern f32 D_800AAA40;
extern u8 D_800DCA20;
extern f32 D_800DCA24;
void func_15102920(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32);
void func_15152B38(void *, u8, s32);
s32 func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);

typedef struct {
    /* 0x0 */ f32 a;
    /* 0x4 */ f32 b;
    /* 0x8 */ f32 c;
} GameVec151C2F48;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ GameVec151C2F48 unk30;
    /* 0x3C */ u8 pad3C[0x6C - 0x3C];
    /* 0x6C */ u8 unk6C[0x88 - 0x6C];
    /* 0x88 */ s32 unk88;
} GameSub151C2F48;

typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0x110 - 0xD];
    /* 0x110 */ GameSub151C2F48 unk110;
} GameObj151C2F48;

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ GameVec151C2F48 unk08;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
    /* 0x4A */ u8 unk4A;
    /* 0x4B */ u8 unk4B;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ u8 unk4E;
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ s8 unk70;
    /* 0x71 */ s8 unk71;
    /* 0x72 */ s8 unk72;
    /* 0x73 */ s8 unk73;
} GameSpawn151C2F48;

void func_151C2F48(GameObj151C2F48 *arg0) {
    GameSub151C2F48 *sub;
    Header header;
    Header2 header2;

    sub = &arg0->unk110;
    func_15102920((func_150ADA68() * 30.0f) + 8.0f, 0xFF, sub->unk6C, &sub->unk30, (func_150ADA20() % 46U) + 0x19, 1, 1, sub->unk88, arg0->unkC, arg0->unk1);
    if (func_150ADA68() < (D_800AAA3C * D_800DCA24)) {
        header.unk0 = 3;
        header.unk1 = -1;
        header.unk2 = (func_150ADA20() % 7U) + 4;
        header.unk4 = 0;
        header2.unk0 = (s32) sub->unk30.a;
        header2.unk4 = (s32) sub->unk30.b;
        header2.unk8 = (s32) sub->unk30.c;
        func_151602C0(&header, &header2, (func_150ADA20() % 9U) + 0xC, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg0->unkC, arg0->unk1);

        {
            GameSpawn151C2F48 sp54;

            sp54.unk00 = 5 >> D_800DCA20;
            sp54.unk04 = 4 >> D_800DCA20;
            sp54.unk08 = sub->unk30;
            sp54.unk14 = 8.25f;
            sp54.unk18 = D_800AAA40;
            sp54.unk1C = 0.0f;
            sp54.unk20 = 0.0f;
            sp54.unk24 = 7.0f;
            sp54.unk28 = 16.0f;
            sp54.unk2C = 0;
            sp54.unk2E = 0xFF;
            sp54.unk30 = -0x3D;
            sp54.unk32 = 0x50;
            sp54.unk34 = 3;
            sp54.unk38 = 1;
            sp54.unk3C = 0xC;
            sp54.unk3E = 0xA;
            sp54.unk40 = 1;
            sp54.unk42 = 4;
            sp54.unk43 = 2;
            sp54.unk44 = 3;
            sp54.unk45 = 0xFF;
            sp54.unk46 = 0xFF;
            sp54.unk47 = 0xFF;
            sp54.unk48 = 0xFF;
            sp54.unk49 = 0;
            sp54.unk4A = 0;
            sp54.unk4B = 0;
            sp54.unk4C = 0;
            sp54.unk4D = 0xFF;
            sp54.unk4E = 0xFF;
            sp54.unk4F = 0xFF;
            sp54.unk50 = 0xFF;
            sp54.unk51 = 0;
            sp54.unk52 = 0;
            sp54.unk53 = 0;
            sp54.unk54 = 0;
            sp54.unk55 = 0xFF;
            sp54.unk56 = 0;
            sp54.unk57 = 3;
            sp54.unk58 = 0x24;
            sp54.unk5C = 0x200005;
            sp54.unk60 = 0x60600;
            sp54.unk64 = 0xA;
            sp54.unk66 = 0x19;
            sp54.unk68 = 1;
            sp54.unk6A = 0;
            sp54.unk6C = 1.0f;
            sp54.unk70 = -1;
            sp54.unk71 = 0;
            sp54.unk72 = -1;
            sp54.unk73 = -1;
            func_15152B38(&sp54, arg0->unkC, arg0->unk1);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1EF500/func_151C329C.s")

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ struct17 pos;
    /* 0x3C */ struct17 unk3C;
    /* 0x48 */ struct17 vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ f32 unk6C;
} GameParticle151C36D8; /* 0x70 */

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_151C36D8(struct17 *arg0, u8 arg1, s32 arg2) {
    GameSpawn151C2F48 spawn;
    GameParticle151C36D8 particle;
    Header header;
    Header2 header2;

    spawn.unk00 = 8;
    spawn.unk04 = 6;
    spawn.unk08 = *(GameVec151C2F48 *)arg0;
    spawn.unk14 = 2.599999905f;
    spawn.unk18 = 2.169999838f;
    spawn.unk1C = -0.9690000415f;
    spawn.unk20 = 0.7180000544f;
    spawn.unk24 = 10.0f;
    spawn.unk28 = 8.0f;
    spawn.unk2C = 0;
    spawn.unk2E = 0xFF;
    spawn.unk30 = -0x40;
    spawn.unk32 = 0x50;
    spawn.unk34 = 3;
    spawn.unk38 = 0;
    spawn.unk3C = 0xF;
    spawn.unk3E = 0xA;
    spawn.unk40 = 1;
    spawn.unk42 = 4;
    spawn.unk43 = 2;
    spawn.unk44 = 3;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0xFF;
    spawn.unk47 = 0xFF;
    spawn.unk48 = 0xFF;
    spawn.unk49 = 0;
    spawn.unk4A = 0;
    spawn.unk4B = 0;
    spawn.unk4C = 0;
    spawn.unk4D = 0xFF;
    spawn.unk4E = 0xFF;
    spawn.unk4F = 0xFF;
    spawn.unk50 = 0xFF;
    spawn.unk51 = 0;
    spawn.unk52 = 0;
    spawn.unk53 = 0;
    spawn.unk54 = 0;
    spawn.unk55 = 0xFF;
    spawn.unk56 = 0;
    spawn.unk57 = 3;
    spawn.unk58 = 0x24;
    spawn.unk5C = 0x200005;
    spawn.unk60 = 0x60600;
    spawn.unk64 = 8;
    spawn.unk66 = 0x1F;
    spawn.unk68 = 1;
    spawn.unk6A = 0;
    spawn.unk6C = 1.0f;
    spawn.unk70 = -1;
    spawn.unk71 = 0;
    spawn.unk72 = -1;
    spawn.unk73 = -1;
    func_15152B38(&spawn, arg1, arg2);

    particle.unk1D = 0x2B;
    particle.unk8 = 0x4403;
    particle.unk0 = 0x200005;
    particle.unk4 = 0x20000;
    particle.unkA = (func_150ADA20() % 5U) + 4;
    particle.unkC = 0;
    particle.unk10 = 0;
    particle.unk14 = 0xFF;
    particle.unk15 = 0xFF;
    particle.unk16 = 0xFF;
    particle.unk17 = 0xFF;
    particle.unk18 = 0xFF;
    particle.unk19 = 0xFF;
    particle.unk1A = 0xFF;
    particle.unk1B = 0xFF;
    particle.unk1C = 0xFF;
    particle.unk28 = particle.unk2C = (func_150ADA68() * 60.0f) + 80.0f;
    particle.pos = *arg0;
    particle.unk1E = 2;
    particle.unk20 = 0x7F;
    particle.unk22 = 1;
    particle.unk3C.unk0 = 0.0f;
    particle.unk3C.unk4 = 0.0f;
    particle.unk3C.unk8 = 0.0f;
    particle.vel.unk0 = 0.0f;
    particle.vel.unk4 = 0.0f;
    particle.vel.unk8 = 0.0f;
    particle.unk54 = 0.0f;
    particle.unk24 = 1.0f;
    particle.unk58 = ((func_150ADA20() & 1) ? 0x40 : 0) | (((func_150ADA20() & 1) ? 0x80 : 0) | 1) | 0x8000 | 0x4000 | 0x200;
    particle.unk60 = 6;
    particle.unk61 = 6;
    particle.unk62 = -1;
    particle.unk63 = -1;
    particle.unk64 = -1;
    particle.unk65 = 4;
    func_15130280(&particle, 1, 0, 0, arg1, arg2);

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() % 7U) + 6;
    header.unk4 = 0;
    header2.unk0 = (s32) arg0->unk0;
    header2.unk4 = (s32) arg0->unk4;
    header2.unk8 = (s32) arg0->unk8;
    func_151602C0(&header, &header2, (func_150ADA20() & 1) + 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg1, arg2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1EF500/func_151C3B0C.s")

void func_151C436C(s32 a0, f32 *a1, s32 a2) {
    f32 x;
    f32 tx;
    f32 y;
    f32 ty;
    f32 z;
    f32 tz;
    f32 factor;

    if (a2 > 0) {
        x = a1[4];
        tx = a1[7];
        y = a1[5];
        ty = a1[8];
        z = a1[6];
        tz = a1[9];
        factor = D_800AAA7C;
        do {
            a1[4] += (tx - a1[4]) * factor;
            a1[5] += (ty - a1[5]) * factor;
            a1[6] += (tz - a1[6]) * factor;
            a2--;
        } while (a2 > 0);
    }
}

typedef struct {
    s32 unk00;
    f32 unk04;
    struct17 unk08;
    struct17 unk14;
    struct17 unk20;
    struct17 unk2C;
    struct17 unk38;
    u8 unk44[0x12];
    s16 unk56;
    u8 unk58;
    u8 unk59;
    u8 unk5A;
    u8 pad5B;
    s32 unk5C;
    s32 unk60;
    u8 pad64[0xC];
} Ray151C43E0;


typedef struct {
    u8 pad0[0x34];
    struct17 pos;
} Pos151C43E0;

void func_15081690(s32, f32, f32, f32, f32, f32, f32, Ray151C43E0 *, f32, s32, s32, s32, s32, s32, s32);

u8 func_151C43E0(struct108 *arg0, Mover151C43E0 *arg1, f32 arg2) {
    u8 result;
    s32 obj;
    f32 scale;
    Ray151C43E0 ray;
    s32 solid;

    obj = arg1->unk90;
    result = 1;
    if (arg1->unk0 & 4) {
        if (arg1->unk8C < arg2) {
            scale = arg1->unk98 * arg1->unk8C;
        } else {
            scale = arg1->unk98 * arg2;
        }
        if (scale != 0.0f) {
            struct17 *from = &((Pos151C43E0 *)arg0)->pos;

            solid = !(arg1->unk0 & 8);
            func_15081690(obj, from->unk0, from->unk4, from->unk8, arg1->unk60, arg1->unk64, arg1->unk68, &ray, scale, 1, 0,
                          solid, arg1->unkA8, 0, arg1->unkAC);
            if (ray.unk59 >= 2) {
                func_151C2EF0(ray.unk00, obj, arg0, (s32)&ray.unk08, (s32)&ray.unk20, (s32)&ray);
                result = 0;
            }
        }
    }
    return result;
}


void func_151C4510(GameStruct151C4510a *a0, GameStruct151C4510b *a1, f32 a2) {
    a0->unk34 += a1->unk4 * a2;
    a0->unk38 += a1->unk8 * a2;
    a0->unk3C += a1->unkC * a2;
}


s32 func_151C455C(s32 arg0, GameStruct151C455C *arg1, f32 arg2) {
    s32 ret;
    GameFunc151C455CCallback callback;
    struct131 *temp_v1;
    s8 index;

    arg1->unk8C -= arg2;
    ret = 1;
    if (arg1->unk8C <= 0.0f) {
        callback = (GameFunc151C455CCallback)D_800E0940;
        if ((callback != 0) && (arg1->unk0 & 1)) {
            temp_v1 = arg1->unk84;
            if (temp_v1 != 0) {
                callback((struct17 *)((u8 *)arg1 + 0x30), temp_v1 - D_800DBEF4);
            }
        }
        if (arg1->unk0 & 2) {
            index = arg1->unk9C;
            if (index != -1) {
                D_8008FBD0[index](arg0);
            }
        }
        ret = 0;
    }
    return ret;
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} GameStruct151C4644Vec;

typedef struct {
    GameStruct151C4644Vec *unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
} GameStruct151C4644Sub;

typedef struct {
    u8 pad0[1];
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    GameStruct151C4644Sub unk28;
} GameStruct151C4644;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
} GameStruct151C4644Payload;

void func_151C4644(GameStruct151C4644 *arg0) {
    GameStruct151C4644Sub *sub;
    GameStruct151C4644Payload sp48;
    struct260 *temp_v0;

    sub = &arg0->unk28;
    sub->unk4 -= D_800BE9E4;
    if (sub->unk4 < 0) {
        sp48.unk0 = sub->unk1C;
        sp48.unk4 = sub->unk20;
        sp48.unk8 = func_150ADA68() * sub->unk24;
        sp48.unkC = func_150ADA68() * sub->unk24;
        sp48.unk10 = func_150ADA68() * D_800AAA80;
        sp48.unk14 = func_150ADA68() * D_800AAA84;
        sp48.unk18 = sub->unk28;
        sp48.unk1C = sub->unk28;
        sp48.unk20 = 0.0f;
        sp48.unk24 = (func_150ADA68() * sub->unk18) + sub->unk14;
        sp48.unk28 = 0.0f;
        sp48.unk2C = sub->unk0->unk0;
        sp48.unk30 = sub->unk0->unk2;
        sp48.unk34 = sub->unk0->unk4;
        sp48.unk38 = sub->unk2C;
        sp48.unk3C = sub->unk30;
        sp48.unk40 = sub->unk34;

        temp_v0 = func_15149130((s16)((func_150ADA20() % (u32)(sub->unk12 + 1)) + sub->unk10),
                                -1, 0x2B, -1, 1, 0, (struct37 *)0x44, arg0->unkC, arg0->unk1);
        if (temp_v0 != NULL) {
            memcpy((u8 *)temp_v0 + 0x28, &sp48, 0x44);
        }

        sub->unk4 = (func_150ADA20() % (u32)(sub->unkC + 1)) + sub->unk8;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1EF500/func_151C4820.s")

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
} GameStruct151C4AB0a;

typedef struct {
    u8 pad0[0x90];
    s32 unk90;
    u8 unk94;
} GameStruct151C4AB0b;

void func_151C4AB0(void *a0, GameStruct151C4AB0a *a1, u8 a2) {
    GameStruct151C4AB0b *v0 = (GameStruct151C4AB0b *)((u8 *)a0 + 0x110);
    if (a2 == 0x2D) {
        if (v0->unk90 == a1->unk0) {
            v0->unk90 = a1->unk4;
            v0->unk94 = a1->unk9;
        } else if (v0->unk90 == a1->unk4) {
            v0->unk90 = a1->unk0;
            v0->unk94 = a1->unk8;
        }
    }
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_151C4B0C;

typedef struct Game1F1F60Hit {
    void *actor;
    f32 dist;
    Vec3f_151C4B0C hit;
    Vec3f_151C4B0C normal;
    Vec3f_151C4B0C point;
    Vec3f_151C4B0C start;
    Vec3f_151C4B0C end;
    s16 surface[9];
    s16 field_56;
    s8 field_58;
    u8 type;
    s8 field_5A;
    u8 pad5B;
    u8 *field_5C;
    s32 field_60;
} Game1F1F60Hit;

s32 func_15145128(f32 *, f32 *, f32 *, f32 *);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, s16 *, f32 *, f32 *, f32 *, f32 *, s32 *, void *, f32);

s32 func_151C4B0C(Game1F1F60Hit *arg0, Game1F1F60Hit *arg1, Vec3f_151C4B0C *arg2, Vec3f_151C4B0C *arg3, f32 *arg4, f32 arg5,
                  u8 arg6, Vec3f_151C4B0C *arg7, Vec3f_151C4B0C *arg8, u8 *arg9, u8 arg10, f32 arg11, void *arg12, s8 arg13,
                  u8 arg14, u8 arg15, s32 arg16, u8 arg17) {
    Game1F1F60Hit hit;

    if (arg0 != NULL) {
        *arg1 = *arg0;
        if (arg2->x != arg1->start.x || arg2->y != arg1->start.y || arg2->z != arg1->start.z) {
            arg1->end.x = arg1->hit.x - arg2->x;
            arg1->end.y = arg1->hit.y - arg2->y;
            arg1->end.z = arg1->hit.z - arg2->z;
            arg1->start = *arg2;
            if (func_15145128(&arg1->end.x, &arg1->end.x, &arg1->dist, NULL) == 0) {
                return 0;
            }
        }
        if (arg17 != 0 && (arg1->type == 2 || arg1->type == 3)) {
            func_15081690((s32)arg12, arg1->start.x, arg1->start.y, arg1->start.z, arg1->end.x, arg1->end.y,
                          arg1->end.z, (Ray151C43E0 *)&hit, 0.0f, 0, 1, 1, -1, 0, arg16);
            if (hit.type != 0) {
                *arg1 = hit;
            }
        }
        *arg4 = arg1->dist * arg5;
    } else if (arg6 != 0) {
        u8 found;
        s32 index;

        index = -1;
        if (arg7 != NULL && arg8 != NULL) {
            arg1->start = *arg7;
            arg1->end = *arg8;
        } else {
            arg1->start = *arg2;
            arg1->end = *arg3;
        }
        func_15081690((s32)arg12, arg1->start.x, arg1->start.y, arg1->start.z, arg1->end.x, arg1->end.y,
                      arg1->end.z, (Ray151C43E0 *)arg1, 0.0f, 0, arg14 ? 0 : 1, (arg15 != 0 && arg14 != 0) ? 0 : 1, arg13,
                      0, arg16);
        if (arg1->field_5C == NULL) {
            index = -1;
        } else {
            index = (arg1->field_5C - (u8 *)D_800DBEF4) / 160;
        }
        if (arg1->type == 0) {
            found = 0;
        } else if (arg1->type == 1) {
            found = 1;
        } else if (arg7 != NULL && arg8 != NULL) {
            f32 dist;
            Game1F1F60Hit saved;

            saved = *arg1;
            arg1->start = *arg2;
            arg1->end.x = arg1->hit.x - arg2->x;
            arg1->end.y = arg1->hit.y - arg2->y;
            arg1->end.z = arg1->hit.z - arg2->z;
            if (func_15145128(&arg1->end.x, &arg1->end.x, &arg1->dist, NULL) == 0) {
                return 0;
            }
            dist = arg1->dist;
            found = func_150AC9C0(arg1->hit.x, arg1->hit.y, arg1->hit.z, arg1->end.x, arg1->end.y, arg1->end.z,
                                  NULL, arg1->surface, &arg1->hit.x, &arg1->hit.y, &arg1->hit.z, &arg1->dist,
                                  &index, &arg1->field_60, 0.0f);
            if (found) {
                arg1->dist += dist;
                arg1->start = *arg2;
            }
        } else {
            found = func_150AC9C0(arg2->x, arg2->y, arg2->z, arg1->end.x, arg1->end.y, arg1->end.z, NULL,
                                  arg1->surface, &arg1->hit.x, &arg1->hit.y, &arg1->hit.z, &arg1->dist, &index,
                                  &arg1->field_60, 0.0f);
        }
        if (found) {
            *arg4 = arg1->dist * arg5;
            if (arg2->x != arg1->start.x || arg2->y != arg1->start.y || arg2->z != arg1->start.z) {
                arg1->end.x = arg1->hit.x - arg2->x;
                arg1->end.y = arg1->hit.y - arg2->y;
                arg1->end.z = arg1->hit.z - arg2->z;
                arg1->start = *arg2;
                if (func_15145128(&arg1->end.x, &arg1->end.x, &arg1->dist, NULL) == 0) {
                    return 0;
                }
            }
            func_151C2050((struct127 *)arg12, &arg1->hit.x, &arg1->end.x, arg1->field_60, arg1->dist);
            if (index == -1) {
                arg1->field_5C = NULL;
            } else {
                arg1->field_5C = (u8 *)(D_800DBEF4 + index);
            }
        } else {
            *arg9 &= ~arg10;
            *arg4 = arg11;
            if (arg3 == NULL && arg8 == NULL) {
                return 0;
            }
        }
    } else {
        *arg9 &= ~arg10;
        *arg4 = arg11;
        arg1->start = *arg2;
        if (arg3 != NULL) {
            arg1->end = *arg3;
        } else if (arg8 != NULL) {
            arg1->end = *arg8;
        } else {
            return 0;
        }
    }
    return 1;
}
