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

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_151C4B0C;

struct Game1F1F60Hit;
void *memcpy(void *, const void *, u32);
s32 func_15145128(f32 *, f32 *, f32 *, f32 *);
s32 func_151D5B6C(s32 arg0, s32 arg1, struct127 *arg2, s8 arg3, s32 arg4);
s32 func_151C4B0C(struct Game1F1F60Hit *arg0, struct Game1F1F60Hit *arg1, Vec3f_151C4B0C *arg2, Vec3f_151C4B0C *arg3, f32 *arg4, f32 arg5,
                  u8 arg6, Vec3f_151C4B0C *arg7, Vec3f_151C4B0C *arg8, u8 *arg9, u8 arg10, f32 arg11, void *arg12, s8 arg13,
                  u8 arg14, u8 arg15, s32 arg16, u8 arg17);

typedef struct Game1EF500TrailExt {
    u8 flags;
    u8 pad1[3];
    f32 velocity[3];
    f32 accel[3];
    f32 drift[3];
    u8 path[0x38];
    f32 direction[3];
    u8 pad6C[0x20];
    u8 extra[4];
    u8 *owner;
    u8 ownerId;
    u8 pad95[3];
    f32 scale;
    s8 field9C;
    u8 pad9D;
    s16 field9E;
    u8 chance;
    u8 fieldA1;
    u8 padA2[2];
    s32 fieldA4;
    s8 fieldA8;
    u8 padA9[3];
    s32 handle;
} Game1EF500TrailExt;

typedef struct Game1EF500TrailSpawn {
    u8 field00;
    u8 field01;
    s16 field02;
    s16 field04;
    u8 pad06[2];
    s32 field08;
    s32 field0C;
    u8 field10;
    u8 field11;
    u8 field12;
    u8 field13;
    f32 field14;
    f32 field18;
    Vec3f_151C4B0C position;
    f32 field28[6];
    s32 flags;
    u8 field44;
    u8 field45;
    u8 field46;
    u8 field47;
    u8 pad48[4];
    u8 field4C;
    u8 pad4D[0xB];
} Game1EF500TrailSpawn;

void *func_151C229C(Vec3f_151C4B0C *arg0, Vec3f_151C4B0C *arg1, s32 arg2, Vec3f_151C4B0C *arg3, u8 arg4, s32 arg5,
                    f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, u8 arg11, u8 *arg12,
                    u8 arg13, u8 arg14, u8 arg15, u8 arg16, u8 arg17, u8 arg18, s8 arg19,
                    s32 arg20, f32 arg21, u8 arg22, s8 arg23, s32 arg24, u8 arg25, s32 arg26) {
    void *result;
    Game1EF500TrailSpawn spawn;
    Game1EF500TrailExt ext;
    Vec3f_151C4B0C sourceDir;
    u8 useSource;
    Vec3f_151C4B0C targetDir;
    u8 useTarget;
    s32 unused; /* extra named home in the original frame */
    void *from;
    Vec3f_151C4B0C *dir;

    useSource = 0;
    useTarget = arg4 && arg3 != NULL;
    if (useTarget) {
        targetDir = *arg3;
    }
    ext.flags = (arg17 ? 1 : 0) | (arg18 ? 2 : 0) | (arg14 ? 4 : 0) | (arg15 ? 8 : 0);
    ext.handle = 0;
    ext.fieldA8 = arg23;
    ext.field9C = arg19;
    ext.fieldA4 = arg20;
    ext.chance = (func_150ADA20() & 0xFFU) < arg16 ? 1 : 0;
    ext.field9E = 0;
    ext.fieldA1 = 0;
    if (arg2 != 0 && arg3 != NULL) {
        if (!useTarget) {
            if (func_15145128(&arg3->x, &targetDir.x, NULL, NULL) != 0) {
                useTarget = 1;
            } else {
                return NULL;
            }
        }
        from = (void *)arg2;
        dir = &targetDir;
    } else {
        if (arg1 != NULL) {
            if (func_15145128(&arg1->x, &sourceDir.x, NULL, NULL) != 0) {
                useSource = 1;
            } else {
                return NULL;
            }
        }
        from = arg0;
        dir = &sourceDir;
    }
    ext.handle = arg14 ? func_151D5B6C((s32)from, (s32)dir, (struct127 *)arg12, arg23, 0) : -1;
    if (func_151C4B0C((struct Game1F1F60Hit *)arg5, (struct Game1F1F60Hit *)ext.path, arg0,
                      useSource ? &sourceDir : NULL, (f32 *)ext.extra, arg7, arg13, (Vec3f_151C4B0C *)arg2,
                      useTarget ? &targetDir : NULL, &ext.flags, 3, arg10, arg12, arg23, arg14, arg15,
                      ext.handle, 0) == 0) {
        return NULL;
    }
    ext.velocity[0] = ext.direction[0] * arg6;
    ext.velocity[1] = ext.direction[1] * arg6;
    ext.velocity[2] = ext.direction[2] * arg6;
    ext.accel[0] = 0.0f;
    ext.accel[1] = 0.0f;
    ext.accel[2] = 0.0f;
    ext.drift[0] = ext.direction[0] * arg9;
    ext.drift[1] = ext.direction[1] * arg9;
    ext.drift[2] = ext.direction[2] * arg9;
    ext.owner = arg12;
    ext.ownerId = (arg12 != NULL) ? arg12[0x3B] : 0x63;
    spawn.field00 = 0x41;
    ext.scale = arg6;
    spawn.field01 = 4;
    spawn.field02 = 0x4503;
    spawn.field04 = 0x12C;
    spawn.field08 = 0;
    spawn.field0C = 0;
    spawn.field10 = 0;
    spawn.field11 = 0;
    spawn.field12 = 0;
    spawn.field13 = 0xFF;
    spawn.field44 = arg11;
    spawn.field45 = 0xFF;
    spawn.field18 = arg9;
    spawn.field14 = arg8;
    spawn.position = *arg0;
    spawn.field28[0] = 0.0f;
    spawn.field28[1] = 0.0f;
    spawn.field28[2] = 0.0f;
    spawn.field28[3] = 0.0f;
    spawn.field28[4] = 0.0f;
    spawn.field28[5] = 0.0f;
    spawn.flags = 0xC04C0008;
    spawn.field46 = 0;
    spawn.field47 = 0;
    spawn.field4C = arg22;
    result = func_1513D2F0(&spawn, (s32)&D_800A4AA0, 0x16, 0, 0, 0x14, 0, 0, 0, arg24 + 0xB0, arg25, arg26);
    if (result != NULL) {
        u8 failed;

        failed = 0;
        memcpy((u8 *)result + 0x110, &ext, 0xB0);
        func_151C436C((s32)result, (f32 *)((u8 *)result + 0x110), (s32)arg21);
        if (func_151C43E0(result, (Mover151C43E0 *)((u8 *)result + 0x110), arg21) == 0) {
            failed = 1;
        }
        func_151C4510(result, (GameStruct151C4510b *)((u8 *)result + 0x110), arg21);
        if (func_151C455C((s32)result, (GameStruct151C455C *)((u8 *)result + 0x110), arg21) == 0) {
            failed = 1;
        }
        if (failed) {
            func_1516972C(result);
            return NULL;
        }
    }
    return result;
}

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

typedef struct {
    s32 field0;
    s32 field4;
    struct17 position8;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    s16 field2C;
    s16 field2E;
    s16 field30;
    s16 field32;
    s32 field34;
    s32 field38;
    s16 field3C;
    s16 field3E;
    s16 field40;
    u8 fields42[23];
    s32 field5C;
    s32 field60;
    s16 field64;
    s16 field66;
    s16 field68;
    u8 field6A;
    f32 field6C;
    s8 fields70[4];
} GameEmitterParticle151C329C;

typedef struct {
    s32 field0;
    s32 field4;
    s16 field8;
    s16 fieldA;
    s32 fieldC;
    s32 field10;
    u8 colors14[9];
    u8 field1D;
    s16 field1E;
    s16 field20;
    s16 field22;
    f32 field24;
    f32 field28;
    f32 field2C;
    struct17 position30;
    f32 field3C;
    f32 field40;
    f32 field44;
    f32 field48;
    f32 field4C;
    f32 field50;
    f32 field54;
    s32 field58;
    s32 field5C;
    s8 fields60[6];
    u8 fields66[0xA];
} GameEmitterSpawn151C329C;

typedef struct {
    u8 field0;
    s8 field1;
    s16 field2;
    u8 field4;
} GameEmitterLight151C329C;

extern f32 D_800AAA44;
extern f32 D_800AAA48;
extern f32 D_800AAA4C;
extern f32 D_800AAA50;
extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_151C329C(struct17 *arg0, u8 arg1, s32 arg2) {
    GameEmitterParticle151C329C descriptor;
    GameEmitterSpawn151C329C spawn;
    GameEmitterLight151C329C light;
    Header2 coordinates;

    descriptor.field0 = 5;
    descriptor.field4 = 7;
    descriptor.position8 = *arg0;
    descriptor.field14 = D_800AAA44;
    descriptor.field18 = D_800AAA48;
    descriptor.field1C = D_800AAA4C;
    descriptor.field20 = D_800AAA50;
    descriptor.field24 = 15.0f;
    descriptor.field28 = 30.0f;
    descriptor.field2C = 0;
    descriptor.field2E = 0xFF;
    descriptor.field30 = -0x40;
    descriptor.field32 = 0x50;
    descriptor.field34 = 3;
    descriptor.field38 = 1;
    descriptor.field3C = 0x14;
    descriptor.field3E = 0xF;
    descriptor.field40 = 1;
    descriptor.fields42[0] = 4;
    descriptor.fields42[1] = 2;
    descriptor.fields42[2] = 3;
    descriptor.fields42[3] = 0xFF;
    descriptor.fields42[4] = 0xFF;
    descriptor.fields42[5] = 0xFF;
    descriptor.fields42[6] = 0xFF;
    descriptor.fields42[7] = 0;
    descriptor.fields42[8] = 0;
    descriptor.fields42[9] = 0;
    descriptor.fields42[10] = 0;
    descriptor.fields42[11] = 0xFF;
    descriptor.fields42[12] = 0xFF;
    descriptor.fields42[13] = 0xFF;
    descriptor.fields42[14] = 0xFF;
    descriptor.fields42[15] = 0;
    descriptor.fields42[16] = 0;
    descriptor.fields42[17] = 0;
    descriptor.fields42[18] = 0;
    descriptor.fields42[19] = 0xFF;
    descriptor.fields42[20] = 0;
    descriptor.fields42[21] = 3;
    descriptor.fields42[22] = 0x24;
    descriptor.field5C = 0x200005;
    descriptor.field60 = 0x60600;
    descriptor.field64 = 8;
    descriptor.field66 = 0x1F;
    descriptor.field68 = 1;
    descriptor.field6A = 0;
    descriptor.field6C = 1.0f;
    descriptor.fields70[0] = -1;
    descriptor.fields70[1] = 0;
    descriptor.fields70[2] = -1;
    descriptor.fields70[3] = -1;
    func_15152B38(&descriptor, arg1, arg2);
    spawn.field1D = 0x2B;
    spawn.field8 = 0x4403;
    spawn.field0 = 0x200005;
    spawn.field4 = 0x20000;
    spawn.fieldA = (func_150ADA20() % 7U) + 6;
    spawn.fieldC = 0;
    spawn.field10 = 0;
    spawn.colors14[0] = 0xFF;
    spawn.colors14[1] = 0xFF;
    spawn.colors14[2] = 0xFF;
    spawn.colors14[3] = 0xFF;
    spawn.colors14[4] = 0xFF;
    spawn.colors14[5] = 0xFF;
    spawn.colors14[6] = 0xFF;
    spawn.colors14[7] = 0xFF;
    spawn.colors14[8] = 0xFF;
    spawn.field28 = spawn.field2C = func_150ADA68() * 196.0f + 106.0f;
    spawn.position30 = *arg0;
    spawn.field1E = 3;
    spawn.field20 = 0x55;
    spawn.field22 = 1;
    spawn.field3C = 0.0f;
    spawn.field40 = 0.0f;
    spawn.field44 = 0.0f;
    spawn.field48 = 0.0f;
    spawn.field4C = 0.0f;
    spawn.field50 = 0.0f;
    spawn.field54 = 0.0f;
    spawn.field24 = 1.0f;
    spawn.field58 = 1 | ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x8000 | 0x4000 | 0x200;
    spawn.fields60[0] = 6;
    spawn.fields60[1] = 6;
    spawn.fields60[2] = -1;
    spawn.fields60[3] = -1;
    spawn.fields60[4] = -1;
    spawn.fields60[5] = 4;
    func_15130280(&spawn, 1, 0, 0, arg1, arg2);
    light.field0 = 3;
    light.field1 = -1;
    light.field2 = (func_150ADA20() % 7U) + 6;
    light.field4 = 0;
    coordinates.unk0 = (s32)arg0->unk0;
    coordinates.unk4 = (s32)arg0->unk4;
    coordinates.unk8 = (s32)arg0->unk8;
    func_151602C0((Header *)&light, &coordinates, (func_150ADA20() & 1) + 5,
                 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg1, arg2);
}

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

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ struct17 pos;
    /* 0x3C */ u8 pad3C[0x60 - 0x3C];
    /* 0x60 */ struct17 prev;
    /* 0x6C */ u8 unk6C[0x88 - 0x6C];
} GameSub151C3B0C;

typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0x110 - 0xD];
    /* 0x110 */ GameSub151C3B0C unk110;
    /* 0x198 */ s32 unk198;
} GameObj151C3B0C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
} GameDebris151C3B0C; /* 0x28 */

typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ u8 pad06[2];
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ struct17 pos;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 pad4D[7];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} GameSpark151C3B0C; /* 0x58 */

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 unk22;
    /* 0x23 */ u8 unk23;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 unk25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ struct17 unk38;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s8 unk60;
    /* 0x61 */ s8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 pad65[3];
    /* 0x68 */ f32 unk68;
} GameSmoke151C3B0C; /* 0x6C */

void func_15153634(void *, u8, u8, s32);

void func_151C3B0C(GameObj151C3B0C *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u8 arg5, u8 arg6, u8 arg7) {
    s32 kind;
    GameSub151C3B0C *sub;
    s32 type;

    kind = arg0->unk198 & 0x1F;
    if ((D_800BE9F0 == 0x2B) || (D_800BE9F0 == 0x30)) {
        switch (kind) {
        case 1:
            type = 3;
            break;
        case 0:
        case 5:
        default:
            type = 4;
            break;
        }
    } else if ((kind == 0xB) && (D_800BE9F0 == 4)) {
        type = 1;
    } else if (((kind == 1) || (kind == 0xA)) && (D_800BE9F0 == 0xE)) {
        type = 2;
    } else if (D_800BE9F0 == 0x34) {
        type = 3;
    } else if (D_800BE9F0 == 0x27) {
        type = 5;
    } else {
        type = 0;
        arg3 = arg2 = arg1 = 0.0f;
    }
    if (func_150ADA68() < arg1) {
        GameDebris151C3B0C debris;
        f32 scale;
        const s32 shade[6] = { 0x6F, 0x6F, 0x70, 0x70, 0x4F, 0x76 };
        u8 big;

        scale = (func_150ADA68() * 15.0f) + 15.0f;
        debris.unk6 = shade[type];
        debris.unk7 = 0;
        debris.unk0 = (D_800BE616 ? 0x20000 : 0) | 0xBF01;
        debris.unk4 = (func_150ADA20() % 61U) + 0x3C;
        debris.unk8 = 0;
        debris.unkC = 0;
        debris.unk10 = (func_150ADA20() % 56U) + 0xC8;
        debris.unk11 = 0xFF;
        debris.unk12 = 0;
        debris.unk13 = 0;
        debris.unk14 = 0;
        debris.unk15 = 0xFF;
        big = (type == 4);
        debris.unk18 = ((big ? 0x48 : 0x3B) << 16) + 2;
        debris.unk16 = 0;
        debris.unk17 = big ? 6 : 7;
        debris.unk20 = arg5;
        debris.unk22 = 0x20;
        debris.unk24 = 7;
        sub = &arg0->unk110;
        func_1513C650((s32)&debris, 0, 0, (s32)sub->unk6C, sub->pos.unk0, sub->pos.unk4, sub->pos.unk8,
                      scale, scale, func_150ADA20() & 0xFF, 0, 0, 0, 0, arg0->unkC, arg0->unk1);
    }
    if (func_150ADA68() < arg2) {
        GameSpark151C3B0C spark;
        const s32 shade[6] = { 0x6D, 0x6D, 0x6E, 0x6E, 0x6D, 0x6D };

        sub = &arg0->unk110;
        spark.unk00 = shade[type];
        spark.unk01 = 0;
        spark.unk02 = 0x3B03;
        spark.unk04 = (func_150ADA20() % 6U) + 0xD;
        spark.unk08 = 0;
        spark.unk0C = 0;
        spark.unk10 = 0xFF;
        spark.unk11 = 0xFF;
        spark.unk12 = 0xFF;
        spark.unk13 = 0xFF;
        spark.unk14 = (func_150ADA68() * 5.0f) + 7.0f;
        spark.unk18 = (func_150ADA68() * 40.0f) + 10.0f;
        spark.pos = sub->pos;
        spark.unk28 = sub->pos.unk0 - sub->prev.unk0;
        spark.unk2C = sub->pos.unk4 - sub->prev.unk4;
        spark.unk30 = sub->pos.unk8 - sub->prev.unk8;
        spark.unk34 = 1.269999981f;
        spark.unk38 = 1.0f;
        spark.unk3C = 56.81999969f;
        spark.flags = (D_800BE616 ? 0x40000000 : 0) | 0x08DC0009;
        spark.unk44 = (func_150ADA20() % 76U) + 0xB4;
        spark.unk45 = 0xFF;
        spark.unk46 = 0;
        spark.unk47 = 7;
        spark.unk48 = 0;
        spark.unk4C = arg7;
        spark.unk54 = 8;
        spark.unk56 = 0x1F;
        func_1513D2F0(&spark, (s32)&D_800A4AA0, 0x22, 0, 0, 0x1C, 2, 0, 0, 0, arg0->unkC, arg0->unk1);
    }
    if (func_150ADA68() < arg3) {
        GameSmoke151C3B0C smoke;
        const u8 colorA[6][3] = { { 0xB7, 0x77, 0x34 }, { 0xB7, 0x77, 0x34 }, { 0xAA, 0x52, 0x1E },
                            { 0xAA, 0x52, 0x1E }, { 0x98, 0x92, 0x79 }, { 0xFF, 0xC0, 0x00 } };
        const u8 colorB[6][3] = { { 0x5C, 0x30, 0x00 }, { 0x5C, 0x30, 0x00 }, { 0x00, 0x00, 0x00 },
                            { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x8D }, { 0x8D, 0x00, 0x00 } };

        smoke.unk0 = 5;
        smoke.unk2 = 0xF;
        smoke.unk4 = 0x6C;
        smoke.unk6 = 0x5103;
        smoke.unk8 = 0x200005;
        smoke.unkC = 0;
        smoke.unk10 = 0x14;
        smoke.unk12 = 0xF;
        smoke.unk14 = 0;
        smoke.unk18 = 0;
        smoke.unk1F = 0xFF;
        smoke.unk1C = colorA[type][0];
        smoke.unk1D = colorA[type][1];
        smoke.unk1E = colorA[type][2];
        smoke.unk20 = colorB[type][0];
        smoke.unk21 = colorB[type][1];
        smoke.unk22 = colorB[type][2];
        smoke.unk23 = 0xE6;
        smoke.unk24 = 0x14;
        smoke.unk25 = 0xFF;
        smoke.unk26 = 0x19;
        smoke.unk28 = 0xA;
        smoke.unk2A = 0x1E;
        smoke.unk2C = 1.049999952f;
        smoke.unk30 = 60.0f;
        smoke.unk34 = 65.0f;
        smoke.unk38 = arg0->unk110.pos;
        smoke.unk44 = 0;
        smoke.unk46 = -0x28;
        smoke.unk48 = 0xFF;
        smoke.unk4A = 0x28;
        smoke.unk4C = 9.0f;
        smoke.unk50 = 17.0f;
        smoke.unk54 = 0.00800000038f;
        smoke.unk58 = 0.2610000074f;
        smoke.unk5C = (D_800BE616 ? 0x800000 : 0) | 0x40E07;
        smoke.unk60 = 0x10;
        smoke.unk61 = -1;
        smoke.unk62 = 8;
        smoke.unk63 = 6;
        smoke.unk64 = 1;
        smoke.unk68 = 0.8702070117f;
        func_15153634(&smoke, arg6, arg0->unkC, arg0->unk1);
    }
    if (func_150ADA68() < arg4) {
        Header header;
        Header2 header2;
        const u8 color[2][3] = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xA7, 0xA7 } };
        u8 sel;

        header.unk0 = 3;
        header.unk1 = -1;
        sub = &arg0->unk110;
        header.unk2 = (func_150ADA20() % 6U) + 3;
        header.unk4 = 0;
        header2.unk0 = (s32)sub->pos.unk0;
        header2.unk4 = (s32)sub->pos.unk4;
        header2.unk8 = (s32)sub->pos.unk8;
        sel = func_150ADA20() & 1;
        func_151602C0(&header, &header2, (func_150ADA20() % 6U) + 0xA, color[sel][0], color[sel][1], color[sel][2], 0xFF, 0, 0, arg0->unkC, arg0->unk1);
    }
}

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

typedef struct Game1F1CD0Emission {
    f32 x, y;
    f32 amplitudeX, amplitudeY;
    f32 phaseX, phaseY;
    f32 speedX, speedY;
    f32 time, rate;
    f32 remaining;
    Vec3f_151C4B0C position;
    f32 scaleX, scaleY, parameter;
} Game1F1CD0Emission;
typedef struct Game1F1CD0Actor {
    u8 pad0;
    u8 kind;
    u8 pad2[0xA];
    u8 owner;
    u8 padD[0x1B];
    Game1F1CD0Emission emission;
} Game1F1CD0Actor;

void func_151436B4(f32, f32, f32, f32 *);
void func_151C4820(Game1F1CD0Actor *arg0);

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
void func_151C4AB0(void *a0, GameStruct151C4AB0a *a1, u8 a2);


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
s32 func_151C4B0C(Game1F1F60Hit *arg0, Game1F1F60Hit *arg1, Vec3f_151C4B0C *arg2, Vec3f_151C4B0C *arg3, f32 *arg4, f32 arg5, u8 arg6, Vec3f_151C4B0C *arg7, Vec3f_151C4B0C *arg8, u8 *arg9, u8 arg10, f32 arg11, void *arg12, s8 arg13, u8 arg14, u8 arg15, s32 arg16, u8 arg17);
