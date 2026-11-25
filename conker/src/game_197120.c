#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#define func_150ADA20 func_150ADA20_hdr
#include "functions.h"
#undef func_15048A40
#undef func_150ADA20
#include "variables.h"


/* Partial view of a type 0x10 ModelFx object (game_1944C0.c): ObjLink, matrices, then its data. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 list;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[0x83];
    /* 0x90 */ f32 x;
    /* 0x94 */ f32 y;
    /* 0x98 */ f32 z;
    /* 0x9C */ f32 rotX;
    /* 0xA0 */ f32 rotY;
    /* 0xA4 */ f32 rotZ;
    /* 0xA8 */ f32 velX;
    /* 0xAC */ f32 velY;
    /* 0xB0 */ f32 velZ;
    /* 0xB4 */ f32 spinX;
    /* 0xB8 */ f32 spinY;
    /* 0xBC */ f32 spinZ;
    /* 0xC0 */ u8 padC0[4];
    /* 0xC4 */ f32 scale;
    /* 0xC8 */ u8 padC8[4];
    /* 0xCC */ s32 timer;
    /* 0xD0 */ u8 padD0[4];
    /* 0xD4 */ f32 baseScale;
    /* 0xD8 */ f32 friction;
    /* 0xDC */ f32 bounce;
    /* 0xE0 */ f32 spinDamp;
    /* 0xE4 */ u8 padE4;
    /* 0xE5 */ u8 alpha;
    /* 0xE6 */ u8 padE6;
    /* 0xE7 */ u8 phase;
    /* 0xE8 */ u8 flags;
    /* 0xE9 */ u8 floorOffset;
    /* 0xEA */ u8 sound;
    /* 0xEB */ u8 kind;
    /* 0xEC */ u8 state;
} Debris;

extern f32 func_15048A40(u8);
extern s32 func_150ADA20(void);
extern s32 func_1510F800(s32);
extern s32 func_1510F8D8(s32, s32, s32, s32);
void func_15171200(s32, s32, f32, f32, f32, f32, f32, f32, f32, s32, s32, f32, s32, s32);
void func_151709B4(s32, s32, s32, s32, f32, s32, s32);
void func_151718F0(f32, f32, f32, f32, s32, f32, s32, s32, s32, s32);
void func_1518CA04(s32);

/* Per-frame update of a bouncing debris particle: integrate motion, land/bounce on the floor, fade out. */
void func_15169C70(Debris *debris) {
    f32 dt;
    s32 floorHeight;
    s32 timer;
    s32 alpha;

    dt = D_800BE9E4;
    debris->x += debris->velX * dt;
    debris->z += debris->velZ * dt;
    debris->rotX += debris->spinX * dt;
    debris->rotY += debris->spinY * dt;
    debris->rotZ += debris->spinZ * dt;
    debris->velY += -0.5f * dt;
    debris->y += debris->velY * dt;
    if (debris->flags & 1) {
        debris->phase += D_800BE9E4 * 8;
        debris->scale = (func_15048A40(debris->phase) * 0.400000006f + 1.0f) * debris->baseScale;
    }
    if (debris->flags & 2) {
        debris->rotY = func_150484A0(debris->velX, debris->velZ) * 57.29577637f;
        debris->rotX = func_150484A0(-debris->velY, sqrtf(debris->velX * debris->velX + debris->velZ * debris->velZ)) * 57.29577637f;
    }
    func_1510F800(0);
    floorHeight = func_1510F8D8(debris->x, debris->y, debris->z, 0) + debris->floorOffset;
    if (debris->y < floorHeight && debris->velY < 0.0f) {
        debris->y = floorHeight;
        if (debris->kind == 1) {
            func_15171200(4, 0xB, debris->x, debris->y, debris->z, debris->velY * debris->bounce,
                          0.5f, 1.0f, 60.0f, 2, 255, -0.400000006f, debris->owner, debris->list);
            // matching: the (u32) step selects IDO's unsigned int-to-float sequence
            func_151718F0(debris->x, (debris->y - (f32)(u32)debris->floorOffset) + 30.0f,
                          debris->z, 50.0f, 40, 0.0030517578125f, 0, 20, debris->owner,
                          debris->list);
            func_151718F0(debris->x, (debris->y - (f32)(u32)debris->floorOffset) + 15.0f,
                          debris->z, 30.0f, 40, 0.0030517578125f, 0, 10, debris->owner,
                          debris->list);
            func_1518CA04(debris->sound);
            func_1516972C((struct102 *)debris);
            return;
        }
        if (debris->kind == 4) {
            func_15171200(3, 0xB, debris->x, debris->y, debris->z, debris->velY * debris->bounce,
                          0.5f, 0.6999999881f, 40.0f, 2, 60, -0.5f, debris->owner, debris->list);
            func_1518CA04(debris->sound);
            func_1516972C((struct102 *)debris);
            return;
        }
        if (debris->kind == 3) {
            func_15171200(2, 0xCB, debris->x, debris->y, debris->z, 6.0f, 0.5f, 1.0f, 60.0f, 5, 60,
                          -0.6999999881f, debris->owner, debris->list);
            func_151709B4(debris->x, debris->y + 10.0f, debris->z, 4, debris->velY * 200.0f,
                          debris->owner, debris->list);
            func_1518CA04(debris->sound);
            func_1516972C((struct102 *)debris);
            return;
        }
        if (debris->kind == 6) {
            debris->velZ = 0.0f;
            debris->velY = 0.0f;
            debris->velX = 0.0f;
            debris->spinZ = 0.0f;
            debris->spinY = 0.0f;
            debris->spinX = 0.0f;
            debris->bounce = 0.0f;
            debris->kind = 0;
            debris->timer = 60;
            return;
        }
        if (debris->kind == 8) {
            func_1518CA04(debris->sound);
            func_1516972C((struct102 *)debris);
            return;
        }
        debris->velX *= debris->friction;
        debris->velZ *= debris->friction;
        debris->velY *= debris->bounce;
        if (debris->velY < 2.0f) {
            // matching: integer 0 (not 0.0f) makes IDO store $zero directly
            debris->velX = 0;
            debris->velY = 0;
            debris->velZ = 0;
            debris->spinX = 0;
            debris->spinY = 0;
            debris->spinZ = 0;
        } else {
            func_151718F0(debris->x, (debris->y - (f32)(u32)debris->floorOffset) + 15.0f,
                          debris->z, 20.0f, 50, 0.000457763671875f, 0, 4, debris->owner,
                          debris->list);
            if (debris->kind == 2) {
                if (func_150ADA20() & 0x8000) {
                    debris->spinX = (f32)(func_150ADA20() >> 16) * 0.000152587890625f + 40.0f;
                }
            } else if (debris->kind == 5) {
                func_151709B4(debris->x, debris->y + 10.0f, debris->z, 4, debris->velY * 200.0f,
                              debris->owner, debris->list);
            }
        }
        debris->spinX *= debris->spinDamp;
        debris->spinZ *= debris->spinDamp;
    }
    timer = debris->timer;
    if (timer != 0) {
        timer -= D_800BE9E4;
        if (timer <= 0) {
            debris->state = 2;
            alpha = debris->alpha;
            alpha -= D_800BE9E4 * 4;
            if (alpha <= 0) {
                func_1518CA04(debris->sound);
                func_1516972C((struct102 *)debris);
                return;
            }
            debris->alpha = alpha;
            timer = 1;
        }
        debris->timer = timer;
    }
}

/* Partial view of a type 5 billboard Particle (game_1944C0.c) as its falling-drop callback uses it. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 list;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[7];
    /* 0x14 */ s32 floorY;      /* floor height last found under the drop */
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ s16 x;
    /* 0x22 */ s16 y;
    /* 0x24 */ s16 z;
    /* 0x26 */ s16 velX;
    /* 0x28 */ s16 velZ;
    /* 0x2A */ s16 unk2A;       /* added to y for the floor test */
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ s16 life;
    /* 0x3A */ u8 pad3A;
    /* 0x3B */ u8 unk3B;
} DropParticle;

/* The 0x18-byte template func_15167AD8 copies into a new SpriteFx from its anim field on. */
typedef struct {
    /* 0x00 */ void *anim;
    /* 0x04 */ s16 frame;
    /* 0x06 */ s16 frameStep;
    /* 0x08 */ s16 x;
    /* 0x0A */ s16 y;
    /* 0x0C */ s16 z;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s8 fadeStep;
    /* 0x13 */ u8 pad13;        /* alpha, set by func_15167AD8 */
    /* 0x14 */ s8 callback;     /* index into D_8008CA20 */
    /* 0x15 */ u8 unk15;
} SpriteFxTemplate;

extern void func_15167AD8(SpriteFxTemplate *, s32, s32);
extern void *D_8008CADC;

/* Drop callback: once it reaches the floor it dies and leaves a splash sprite; in the air, x/z drag. */
void func_1516A3F4(DropParticle *drop) {
    s32 level;
    s32 bottom;
    s32 floorY;
    SpriteFxTemplate splash;
    s32 dim;

    bottom = drop->y + drop->unk2A;
    floorY = drop->floorY;
    if (floorY >= bottom) {
        func_1510F800(0);
        floorY = drop->floorY = func_1510F8D8(drop->x, drop->y, drop->z, 0);
    }
    if (floorY >= bottom) {
        drop->life = 0;
        if (bottom - floorY < 50) {
            splash.anim = D_8008CADC;
            splash.frame = 0;
            splash.frameStep = 0x100;
            splash.x = drop->x;
            splash.y = drop->floorY + 10;
            splash.z = drop->z;
            level = drop->unk3B;
            splash.fadeStep = 0;
            dim = (level << 12) / 5100 + 204;
            splash.unkE = dim;
            splash.unk10 = dim;
            splash.unk15 = level;
            splash.callback = 3;
            func_15167AD8(&splash, drop->owner, drop->list);
        }
    } else {
        drop->velX *= 0.9800000191f;
        drop->velZ *= 0.9800000191f;
    }
}

struct Struct1516A538 {
    u8 pad0;
    u8 unk1;
    u8 padb[0xA];
    u8 unkC;
    u8 padd[0x7];
    s16 unk14;
    u8 pad16[0x2];
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    u8 pad1e[0x7];
    u8 unk25;
};

extern void func_15171D4C(f32, f32, f32, s32, s32, s32, f32, s32, s32, s32, s32, s32, s32, s32);
extern void func_1516F864(s32);
extern void func_1516F94C(s32, s32);

void func_1516A538(struct Struct1516A538 *arg0) {
    u32 b;
    u32 a;
    s32 c;

    if (arg0->unk14 >= 0x300) {
        a = (u32)func_150ADA20() % 3;
        c = arg0->unk25;
        b = (u32)func_150ADA20() % 0x168;
        func_15171D4C(
            (f32)(s32)arg0->unk18,
            (f32)((s32)arg0->unk1A - 0xA),
            (f32)(s32)arg0->unk1C,
            0x64,
            0,
            a + 0x20,
            (f32)(u32)b,
            0,
            (c * 25) / 255 + 0x19,
            0,
            0x100,
            0,
            arg0->unkC,
            arg0->unk1);
    }
}

typedef struct {
    char pad_0[0xE];
    s16 field_0xE;
    s16 field_0x10;
    s16 field_0x12;
    s16 field_0x14;
    s16 field_0x16;
    s16 field_0x18;
    char pad_0x1A[0x5];
    u8 field_0x1F;
    char pad_0x20[0x4];
    u8 field_0x24;
    char pad_0x25[0x7];
    u8 field_0x2C;
    u8 field_0x2D;
} Obj1516A648;

s32 func_1516A648(Obj1516A648 *arg0) {
    s32 temp_a1;
    s32 temp_v0;
    s32 temp_v1;

    temp_a1 = arg0->field_0x24;
    temp_v0 = arg0->field_0x1F;
    if (temp_a1 != 0) {
        if (temp_v0 != 0xFF) {
            temp_v0 += D_800BE9E4 << 4;
            if (temp_v0 >= 0x100) {
                temp_v0 = 0xFF;
            }
            arg0->field_0x1F = temp_v0;
            temp_a1 = ((volatile Obj1516A648 *)arg0)->field_0x24;
        }
    } else {
        if (temp_v0 != 0) {
            temp_v0 -= D_800BE9E4 << 2;
            if (temp_v0 < 0) {
                temp_v0 = 0;
            }
            arg0->field_0x1F = temp_v0;
            temp_a1 = ((volatile Obj1516A648 *)arg0)->field_0x24;
        }
    }

    if ((temp_a1 == 0) && (temp_v0 == 0)) {
        return 1;
    }

    arg0->field_0x18 = arg0->field_0x18 - 0x12C;
    func_1516F864((s32)arg0);
    temp_v1 = func_1510F8D8(arg0->field_0xE, arg0->field_0x10, arg0->field_0x12, 0) + 0xA;
    arg0->field_0x2C = temp_v1 >> 8;
    arg0->field_0x2D = temp_v1 & 0xFF;
    if (arg0->field_0x10 < temp_v1) {
        arg0->field_0x10 = temp_v1;
        arg0->field_0x18 = (s16)(s32)((f32)(s32)arg0->field_0x18 * -0.5f);
        func_1516F94C((s32)arg0, 0xE6);
    }
    return 0;
}
