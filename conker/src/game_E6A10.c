#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
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
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
} Spawn150B89E0;

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
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    u8 unk7C;
    u8 unk7D;
    u8 pad7E[2];
    s16 unk80;
    s16 unk82;
    s32 unk84;
} Burst150B89E0;

void *memcpy(void *, const void *, u32);
void func_15151A38(Burst150B89E0 *, u8, s32);

/* Particle state shared by the update callbacks below. */
typedef struct {
    u8 pad0[0x1C];
    s16 unk1C;
    u8 pad1E[0xA];
    u8 unk28;
    u8 pad29[3];
    f32 unk2C;
    f32 unk30;
    u8 pad34[4];
    f32 unk38;
    u8 pad3C[0x20];
    u8 unk5C;
    u8 pad5D[0xB3];
    f32 unk110;
} Particle150B9560;

/* Before t=80: integrates unk110 as a speed and moves unk38 by it. From t=115 scales unk2C/unk30 by 1.35; before t=65 grows unk30. */
s32 func_150B9560(Particle150B9560 *particle) {
    f32 *speed = &particle->unk110;
    f32 scale;

    if (particle->unk1C < 80) {
        *speed += 0.3f * D_800BE9A4;
        particle->unk38 -= *speed;
    }

    if (particle->unk1C >= 115) {
        scale = 1.35f;
        particle->unk2C *= scale;
        particle->unk30 *= scale;
    } else if (particle->unk1C < 65) {
        particle->unk30 += 1.2f;
    }

    return 1;
}

/* Ramps unk5C up by 8 per step of unk1C while unk1C < 32. */
s32 func_150B95FC(Particle150B9560 *particle) {
    s16 t = particle->unk1C;

    if (t < 32) {
        particle->unk5C = t * 8;
    }
    return 1;
}

/* Ramps unk28 up by 4 per step of unk1C while unk1C < 64. */
s32 func_150B961C(Particle150B9560 *particle) {
    s16 t = particle->unk1C;

    if (t < 64) {
        particle->unk28 = t * 4;
    }
    return 1;
}

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    f32 unk10;
    u8 pad14[0xC];
    f32 unk20;
    f32 unk24;
    f32 unk28;
    u8 pad2C[0xC];
    vertex unk38;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    u8 pad50[0x10];
    s32 unk60;
    s16 unk64;
    u8 pad66[0xB];
    u8 unk71;
    u8 pad72;
    u8 unk73;
    u8 pad74;
    u8 unk75;
    u8 pad76[2];
    u8 unk78;
    u8 pad79[0x97];
    f32 unk110;
    s32 unk114;
    u8 pad118[0x10];
    s32 unk128;
    u8 pad12C[0x20];
    s32 unk14C;
    u8 pad150[0x18];
    s32 unk168;
} Obj150B963C;

s32 func_15046C80(vertex *, s32, f32, f32 *);
extern s32 D_80088740;

s32 func_150B963C(Obj150B963C *arg0) {
    f32 dist;
    vertex pp;
    f32 sp16C;
    f32 sp168;
    vertex pos;

    if ((D_80088730 == NULL) || (D_80088730->interaction_state == 0) || (D_80088730->unique_id != 1)) {
        D_80088730 = func_15083E90(1);
    }
    if (D_80088730 != NULL) {
        pp.x = D_80088730->x_position;
        pp.y = D_80088730->y_position;
        pp.z = D_80088730->z_position;
        dist = ((arg0->unk38.x - pp.x) * (arg0->unk38.x - pp.x)) + ((arg0->unk38.y - pp.y) * (arg0->unk38.y - pp.y)) + ((arg0->unk38.z - pp.z) * (arg0->unk38.z - pp.z));
        if (dist < ((arg0->unk10 * arg0->unk10) + 2500.0f)) {
            if ((D_80088730->xz_velocity > 15.0f) && (func_150ADA20() & 1)) {
                sp16C = func_151423D8(D_80088730->unk76 >> 8);
                sp168 = func_151423D8((D_80088730->unk76 >> 8) + 0x40);
                arg0->unk71 = 1;
                arg0->unk60 |= 0x7D;
                arg0->unk73 = 1;
                arg0->unk75 = 1;
                arg0->unk78 = 0;
                arg0->unk44 = D_80088730->xz_velocity * sp16C * 1.299999952f;
                arg0->unk48 = (func_150ADA68() * 15.0f) + 10.0f;
                arg0->unk4C = D_80088730->xz_velocity * sp168 * 1.299999952f;
                arg0->unk64 = 0x12C;
                arg0->unk60 |= 0x80;
                arg0->unk20 = 0.0f;
                arg0->unk24 = 0.0f;
                arg0->unk28 = 0.0f;
                return 1;
            }
            pos.x = arg0->unk38.x;
            pos.y = arg0->unk38.y + 100.0f;
            pos.z = arg0->unk38.z;
            if (func_15046C80(&pos, 0, arg0->unk38.y - 2000.0f, &arg0->unk110) != 0) {
            lbl_spawn:
                pos.y = arg0->unk110;
                {
                Spawn150B89E0 spawn;
                s32 sp_ret;
                f32 extra;
                spawn.unk54 = 0x19;
                spawn.unk56 = 0xA;
                spawn.unk0 = 4;
                spawn.unk1 = 0;
                spawn.unk2 = 0x3B02;
                spawn.unk4 = (func_150ADA20() % 31U) + 0x28;
                spawn.unk8 = 0;
                spawn.unkC = 0;
                spawn.unk10 = 0xFF;
                spawn.unk11 = 0xFF;
                spawn.unk12 = 0xFF;
                spawn.unk13 = 0xFF;
                spawn.unk14 = spawn.unk18 = ((func_150ADA68() * 25.0f) + 25.0f) * 0.001000000047f;
                *(vertex *)&spawn.unk1C = pos;
                spawn.unk40 = 0x0C5C0001;
                spawn.unk44 = 0xFF;
                spawn.unk45 = 0xFF;
                spawn.unk46 = 0;
                spawn.unk47 = 7;
                spawn.unk28 = 0.0f;
                spawn.unk2C = 0.0f;
                spawn.unk30 = 0.0f;
                spawn.unk34 = 0.0f;
                spawn.unk38 = 0.0f;
                spawn.unk3C = 0.0f;
                extra = ((func_150ADA68() * 80.0f) + 50.0f) * 9.999999747e-06f;
                spawn.unk50 = arg0->unk128;
                sp_ret = func_1513D594((s32)&spawn, (s32)&D_800A4AA0, 0x18, 0, 0,
                                    (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1), func_150ADA20() & 0xFF,
                                    500.0f, 500.0f, 0, (s32)&arg0->unk114, arg0->unk14C, arg0->unk168, 0, 8, arg0->unkC, arg0->unk1);
                if (sp_ret != 0) {
                    memcpy((u8 *)sp_ret + 0x128, &extra, 8);
                }
                }
            }
            {
            Burst150B89E0 burst;
            *(vertex *)&burst.unk0 = arg0->unk38;
            burst.unkC = 3.0f;
            burst.unk10 = 3.100000143f;
            burst.unk24 = 6;
            burst.unk26 = 6;
            burst.unk2A = 0xFF;
            burst.unk2C = -0x40;
            burst.unk2E = 0x1E;
            burst.unk28 = 0;
            burst.unk30 = 3;
            burst.unk32 = 3;
            burst.unk34 = 0x1E;
            burst.unk36 = 0x14;
            burst.unk38 = 1;
            burst.unk39 = 0x21;
            burst.unk3A = 0x48;
            burst.unk3C = 1;
            burst.unk40 = 0;
            burst.unk44 = 0;
            burst.unk48 = 0;
            burst.unk4C = 0;
            burst.unk50 = 0;
            burst.unk54 = 0;
            burst.unk55 = 0x9A;
            burst.unk56 = 0x65;
            burst.unk14 = 0.7050000429f;
            burst.unk18 = 0.3100000024f;
            burst.unk1C = 5.0f;
            burst.unk20 = 7.0f;
            burst.unk58 = arg0->unk14C;
            burst.unk5C = arg0->unk168;
            burst.unk60 = 0;
            burst.unk64 = 0x220005;
            burst.unk68 = 0x1D0600;
            burst.unk6C = 1;
            burst.unk70 = 0x3B;
            burst.unk74 = 0x80;
            burst.unk78 = 0x20;
            burst.unk7C = 0;
            burst.unk7D = 7;
            burst.unk80 = 0xA;
            burst.unk82 = 0x19;
            burst.unk84 = arg0->unk128;
            func_15151A38(&burst, arg0->unkC, arg0->unk1);
            }
            func_10010F88(D_80088740, 0x61A8, 0, 0, 0, arg0->unk38.x, arg0->unk38.y, arg0->unk38.z, 1000, 4000);
            return 0;
        }
    }
    if ((D_80088720 == NULL) || (D_80088720->interaction_state == 0) || (D_80088720->unique_id != 1)) {
        D_80088720 = func_15083E90(1);
    }
    if (D_80088720 != NULL) {
        D_80088724.x = D_80088720->x_position;
        D_80088724.y = D_80088720->y_position + 50.0f;
        D_80088724.z = D_80088720->z_position;
    }
    arg0->unk20 = arg0->unk20 + (0.200000003f * (D_80088724.x - arg0->unk20));
    arg0->unk24 = arg0->unk24 + (0.200000003f * (D_80088724.y - arg0->unk24));
    arg0->unk28 = arg0->unk28 + (0.200000003f * (D_80088724.z - arg0->unk28));
    return 1;
}


struct Struct150B9D14 {
    char pad0[0x18];
    s32 unk18;
    s32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
};

extern void func_15142600(s32, s32, s32, s32, f32, f32, f32, f32, f32, f32, f32, f32);

s32 func_150B9D14(s32 arg0, struct Struct150B9D14 *arg1) {
    func_15142600(arg0, arg1->unk18, arg1->unk1C, arg1->unk2C,
                  arg1->unk30, arg1->unk34, arg1->unk38, arg1->unk3C,
                  arg1->unk40, arg1->unk20, arg1->unk24, arg1->unk28);
    return 1;
}

struct Struct150B9D8C {
    char pad0[0x1];
    u8  unk1;
    char pad2[0xA];
    u8  unkC;
    char padD[0x83];
    s32 unk90;
    char pad94[0x4];
    s16 unk98;
    char pad9A[0x2];
    s16 unk9C;
    char pad9E[0x2];
    s16 unkA0;
    s16 unkA2;
    char padA4[0x2];
    s16 unkA6;
};

struct Struct150B9D8CTmp {
    s32 unk0;
    s16 unk4;
    s8  unk6;
    char pad7[0x1];
    s32 unk8;
    s32 unkC;
    u8  unk10;
    u8  unk11;
    u8  unk12;
    u8  unk13;
    u8  unk14;
    u8  unk15;
    char pad16[0x2];
    s32 unk18;
    char pad1C[0x10];
};

void func_150B9D8C(struct Struct150B9D8C *arg0) {
    struct Struct150B9D8CTmp tmp;
    s32 c;
    s32 b;
    s32 a;

    if (arg0->unk98 == -1) {
        tmp.unk6 = 4;
        tmp.unk8 = 0;
        tmp.unkC = 0;
        tmp.unk0 = 0x11;
        tmp.unk4 = 0x64;
        tmp.unk10 = 0xFF;
        tmp.unk11 = arg0->unk90;
        tmp.unk12 = 0;
        tmp.unk13 = 0;
        tmp.unk14 = 0;
        tmp.unk15 = 0xFF;
        tmp.unk18 = 0x30001;

        a = func_150ADA20();
        b = func_150ADA20();
        c = func_150ADA20();

        func_1513C4EC((s32)&tmp, 0, 2, 0,
                      (f32)arg0->unk9C,
                      (f32)(arg0->unkA6 + 2),
                      (f32)arg0->unkA0,
                      (f32)arg0->unkA2,
                      (f32)arg0->unkA2,
                      a & 0xFF,
                      ((c & 1) << 1) + (b & 1),
                      3, 0xFF, 0,
                      arg0->unkC,
                      arg0->unk1);
    }
}
