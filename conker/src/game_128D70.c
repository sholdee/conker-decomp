#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

/* State block that func_150FC438 copies into its spawned object at offset 0x28. */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerUniqueId;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 spawnTimer;
    /* 0x0C */ u8 side;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ struct260 *unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s8 unk16; // owner camera's unk23D, or -1
    /* 0x17 */ u8 pad17;
    /* 0x18 */ struct127 *other;
    /* 0x1C */ u8 otherUniqueId;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 pad1E[2];
} EmitterState; // size 0x20

/* The object spawned by func_150FC438. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ EmitterState state;
} EmitterObj;


/* Effect descriptor passed to func_15164780. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ s8 unk1;
    /* 0x02 */ s8 unk2;
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 pad7;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
} EmitterEffect; // size 0x38

extern struct260 *func_15164780(EmitterEffect *, s32, u8, s32);


typedef struct {
    u8 pad0[0x130];
    f32 unk130;
    f32 unk134;
    f32 unk138;
    f32 unk13C[3];
    u8 pad148[0x4F];
    u8 unk197;
} S31C_150FB8C0;

/* First argument of func_1515548C (same layout as game_113D60.c's Struct1515548CArg). */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 sizeX;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ s8 pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 flags;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 unk22;
    /* 0x23 */ u8 unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s8 unk40;
    /* 0x41 */ s8 unk41;
    /* 0x42 */ s8 pad42[2];
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 pad45[3];
    /* 0x48 */ u8 pad48[0x10];
} EffectArg_150FB8C0; // size 0x58

/* First argument of func_151602C0. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ s8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s8 unk4;
} SparkArg_150FB8C0;

/* First argument of func_1515572C. */
typedef struct {
    /* 0x00 */ u8 side;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ struct108 *camera;
} CameraArg_150FB8C0;

extern u8 D_800A1DD0[], D_800A1E34[];
extern f32 D_800A1DD4[][3], D_800A1DEC[][3], D_800A1E04[][3], D_800A1E1C[][3], D_800A1E38[][3], D_800A1E50[][3], D_800A1E68[][3], D_800A1E80[][3];
extern f32 D_800A1E98, D_800A1E9C, D_800A1EA0, D_800A1EA4, D_800A1EA8, D_800A1EAC;
extern struct127 *func_1507515C(struct127 *);
extern void func_15145EA4(void *, void *, s32, s32);
extern void func_151450B4(struct17 *, struct17 *, struct17 *);
extern s32 func_151C229C(f32 *, f32 *, f32 *, f32 *, s32, s32, f32, f32, f32, f32, f32, s32, struct127 *, s32, s32, s32, s32, s32, s32, s32, s32, f32, s32, s32, s32, s32, s32);
extern s32 func_151602C0(void *, void *, s32, s32, s32, s32, s32, s32, s32, u8, s32);
extern void func_15102B38(struct127 *, u8, void *, void *, f32 *, s32, s32, f32, f32 *, s32, s32, s32, u8, s32);
extern void func_1515548C(void *, s32, s32, s32, s32, s32, s32);
extern void func_1515572C(void *, s32);

void func_150FB8C0(struct127 *arg0, u8 arg1, f32 arg2, u8 arg3, u8 arg4, s32 arg5) {
    struct127 *sp1C4;
    u8 sp1C3;
    f32 sp1B4[3];
    f32 sp1A8[3];
    f32 sp19C[3];
    f32 sp190[3];
    f32 sp184[3];
    f32 sp178[3];
    void *sp16C[3];
    void *sp160[3];
    s32 sp15C;
    f32 sp150[3];
    struct17 sp144;
    f32 t;
    f32 d0;
    f32 d1;
    f32 d2;
    f32 *sp130;
    u8 sp12F;
    s8 sp12E;
    SparkArg_150FB8C0 sp128;
    s32 sp11C[3];
    f32 sp114[2];
    u8 sp113;
    EffectArg_150FB8C0 spB8;
    CameraArg_150FB8C0 spB0;

    if (arg0 == NULL || arg1 < 0 || arg1 >= 2) {
        return;
    }
    if (arg3 != 0) {
        sp1C4 = NULL;
        sp1C3 = 0;
    } else {
        sp1C4 = func_1507515C(arg0);
        if (sp1C4 == NULL) {
            return;
        }
        if (((S31C_150FB8C0 *)sp1C4->unk31C) == NULL) {
            return;
        }
        if (((S31C_150FB8C0 *)sp1C4->unk31C)->unk197 != 0) {
            sp1C3 = 1;
        } else {
            sp1C3 = 0;
        }
    }
    if (arg0->unk1D4 == NULL) {
        return;
    }
    if (arg0->id == 0x23) {
        sp16C[0] = D_800A1E38[arg1];
        if (sp1C3) {
            sp16C[1] = D_800A1E68[arg1];
        } else {
            sp16C[1] = D_800A1E50[arg1];
        }
        sp16C[2] = D_800A1E80[arg1];
        sp15C = (D_800A1E34[arg1] << 6) + (s32)arg0->unk1D4;
    } else {
        sp150[0] = (13.0f - (1.0f + func_150ADA68() * 6.0f)) * (arg1 ? -1 : 1);
        sp150[1] = D_800A1DEC[arg1][1];
        sp150[2] = D_800A1DEC[arg1][2];
        sp16C[0] = D_800A1DD4[arg1];
        sp16C[1] = sp1C3 ? D_800A1E04[arg1] : sp150;
        sp16C[2] = D_800A1E1C[arg1];
        sp15C = (D_800A1DD0[arg1] << 6) + (s32)arg0->unk1D4;
    }
    sp160[0] = sp1B4;
    if (sp1C3) {
        sp160[1] = sp184;
    } else {
        sp160[1] = sp1A8;
    }
    sp160[2] = sp178;
    func_15145EA4(sp16C, sp160, sp15C, sp1C3 ? 3 : 2);
    if (sp1C3) {
        extern void func_151450B4(struct17 *, struct17 *, struct17 *);

        sp130 = ((S31C_150FB8C0 *)sp1C4->unk31C)->unk13C;
        sp184[0] -= sp1B4[0];
        sp184[1] -= sp1B4[1];
        sp184[2] -= sp1B4[2];
        sp178[0] -= sp1B4[0];
        sp178[1] -= sp1B4[1];
        sp178[2] -= sp1B4[2];
                func_151450B4((struct17 *)sp184, (struct17 *)sp178, &sp144);
        d0 = sp1B4[0] - sp130[0];
        d1 = sp1B4[1] - sp130[1];
        d2 = sp1B4[2] - sp130[2];
        t = ((sp144.unk0 * d0) + (sp144.unk4 * d1) + (sp144.unk8 * d2)) /
            ((sp144.unk0 * ((S31C_150FB8C0 *)sp1C4->unk31C)->unk130) + (sp144.unk4 * ((S31C_150FB8C0 *)sp1C4->unk31C)->unk134) + (sp144.unk8 * ((S31C_150FB8C0 *)sp1C4->unk31C)->unk138));
        sp19C[0] = sp130[0] + (t * ((S31C_150FB8C0 *)sp1C4->unk31C)->unk130);
        sp19C[1] = sp130[1] + (t * ((S31C_150FB8C0 *)sp1C4->unk31C)->unk134);
        sp19C[2] = sp130[2] + (t * ((S31C_150FB8C0 *)sp1C4->unk31C)->unk138);
        sp190[0] = ((S31C_150FB8C0 *)sp1C4->unk31C)->unk130;
        sp190[1] = ((S31C_150FB8C0 *)sp1C4->unk31C)->unk134;
        sp190[2] = ((S31C_150FB8C0 *)sp1C4->unk31C)->unk138;
    } else {
        sp1A8[0] -= sp1B4[0];
        sp1A8[1] -= sp1B4[1];
        sp1A8[2] -= sp1B4[2];
    }
    if (!sp1C3 || sp1C4->camera == NULL) {
        sp12F = 0xFF;
    } else {
        sp12F = ~(1 << sp1C4->camera->unk23D) & 0xFF;
    }
    if (D_800BE9F0 == 0x34 || D_800BE9F0 == 0x26) {
        sp12E = 5;
    } else {
        sp12E = 4;
    }
    func_151C229C(sp1B4, (sp1C3) ? NULL : sp1A8, (sp1C3) ? sp19C : NULL, (sp1C3) ? sp190 : NULL, (sp1C3) ? 1 : 0, 0,
                  252.0f, 0.003968254197f, (func_150ADA68() * 20.0f) + 75.0f, (func_150ADA68() * 250.0f) + 502.0f, 50.0f,
                  (func_150ADA20() % 31U) + 0xE1, (sp1C4 != NULL) ? sp1C4 : arg0, 1, 1, (D_800BE616 != 0) ? 0 : 1, 0x32, 0, 1, sp12E, 0x2C, arg2,
                  sp12F, 0, 0, arg4, arg5);
    if ((arg0->unk74 & 0xF) != 0xF) {
        if (!(func_150ADA20() & 7)) {
            sp128.unk0 = 3;
            sp128.unk1 = -1;
            sp128.unk2 = (func_150ADA20() % 5U) + 4;
            sp128.unk4 = 0;
            sp11C[0] = (s32)sp1B4[0];
            sp11C[1] = (s32)sp1B4[1];
            sp11C[2] = (s32)sp1B4[2];
            func_151602C0(&sp128, sp11C, (func_150ADA20() % 31U) + 0x46, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg4, arg5);
        }
        if (!(func_150ADA20() & 1)) {
            sp114[1] = (func_150ADA68() * 2.435000181f) + 3.534000158f;
            sp114[0] = (func_150ADA68() * 43.40000153f) + 48.40000153f;
            if (!sp1C3 || sp1C4->camera == NULL) {
                sp113 = 0xFF;
            } else {
                sp113 = ~(1 << sp1C4->camera->unk23D) & 0xFF;
            }
            func_15102B38(arg0, (arg0->id == 0x23) ? D_800A1E34[arg1] : D_800A1DD0[arg1], (arg0->id == 0x23) ? (void *)D_800A1E38[arg1] : (void *)D_800A1DD4[arg1],
                          (arg0->id == 0x23) ? (void *)D_800A1E50[arg1] : (void *)D_800A1DEC[arg1], sp114,
                          (func_150ADA20() % 3U) + 4, (func_150ADA20() % 101U) + 0x9B, (func_150ADA68() * 1012.0f) + 1497.0f, sp1B4,
                          sp113, 0, -1, arg4, arg5);
        }
    }
    if (sp1C3 && sp1C4->camera != NULL) {
        if (arg1) {
            spB8.x = -59.0f;
        } else {
            spB8.x = 59.0f;
        }
        spB8.y = 68.0f;
        spB8.sizeX = (func_150ADA68() * 20.0f) + 40.0f;
        spB8.sizeY = (func_150ADA68() * 20.0f) + 80.0f;
        spB8.unk10 = 0x80;
        spB8.unk12 = (func_150ADA20() & 3) + 5;
        spB8.flags = (s16)((arg1 ? 2 : 0) | 0x39) & 0xFFFF;
        spB8.unk16 = 3;
        spB8.unk18 = 0x55;
        spB8.unk1A = 0;
        spB8.unk1B = 0xFF;
        spB8.unk1C = 0xFF;
        spB8.unk1D = 0xFF;
        spB8.unk1E = (func_150ADA20() % 56U) + 0xC8;
        spB8.unk1F = 0xFF;
        spB8.unk20 = 0xFF;
        spB8.unk21 = 0xFF;
        spB8.unk22 = 0xFF;
        spB8.unk23 = 0xFF;
        spB8.unk28 = 0x200004;
        spB8.unk24 = 0;
        spB8.unk2C = 0x1F0601;
        spB8.unk30 = 3;
        spB8.unk34 = 0x22;
        spB8.unk38 = 0x80;
        spB8.unk3C = 0x20;
        spB8.unk40 = 0;
        spB8.unk41 = 7;
        spB8.unk44 = sp1C4->camera->unk23D;
        func_1515548C(&spB8, 0, 0, 0, 0, 0, arg5);
        spB0.side = arg1;
        spB0.camera = sp1C4->camera;
        func_1515572C(&spB0, 0x37);
    }
}


void func_151C3B0C(void *, f32, f32, f32, f32, s32, s32, s32);

typedef struct {
    /* 0x000 */ u8 pad0[0x1A0];
    /* 0x1A0 */ struct127 *owner;
    /* 0x1A4 */ u8 ownerUniqueId;
} Particle_150FC368;

/* Calls func_151C3B0C for a particle. The last argument is a mask of every camera except the owner's,
 * or 0xFF when the owner is gone, has no camera or fails one of the other checks. */
void func_150FC368(Particle_150FC368 *particle) {
    u8 cameraMask;
    struct127 *owner;

    owner = particle->owner;
    if ((owner == NULL) || (owner->interaction_state == 0) || (owner->id == 0xFF) ||
        (owner->unique_id != particle->ownerUniqueId) || (owner->unk31C == NULL) ||
        (owner->unk31C->unk84 != 0) || (owner->unk127 == 0xFF) || (owner->camera == NULL)) {
        cameraMask = 0xFF;
    } else {
        cameraMask = ~(1U << owner->camera->unk23D) & 0xFF;
    }
    func_151C3B0C(particle, 1.0f, 1.0f, 0.6f, 0.0f, 0xFF, 0xFF, cameraMask);
}

/* Spawns an emitter object that follows `owner` (see func_150FC614); when `other` has a camera, also
 * creates an effect for that camera. */
void func_150FC438(struct127 *owner, struct127 *other, u8 arg2, u8 arg3) {
    EmitterState state;
    EmitterObj *obj;
    EmitterState *dest;
    EmitterEffect effect;

    state.owner = owner;
    state.ownerUniqueId = owner->unique_id;
    state.spawnTimer = 0.0f;
    state.side = 0;
    state.unkD = arg2;
    state.unk10 = NULL;
    state.unk14 = 0;
    state.unk1D = arg3;
    if (other != NULL && other->camera != NULL) {
        state.unk16 = other->camera->unk23D;
    } else {
        state.unk16 = -1;
    }
    state.other = other;
    if (other != NULL) {
        state.otherUniqueId = other->unique_id;
    } else {
        state.otherUniqueId = 0xFF;
    }

    obj = (EmitterObj *)func_15149130(300, -1, 0x27, -1, 0, 0x25, (struct37 *)0x20, 0xFF, 1);
    if (obj != NULL) {
        dest = &obj->state;
        memcpy(dest, &state, sizeof(state));
        if (other != NULL && other->camera != NULL) {
            effect.unk0 = 4;
            effect.unk1 = -1;
            effect.unk2 = -1;
            effect.unk4 = 300;
            effect.unk6 = other->camera->unk23D;
            effect.unk8 = 12.0f;
            effect.unkC = 20.0f;
            effect.unk10 = 19.0f;
            effect.unk14 = 0.1976999938f;
            effect.unk18 = 0.2345999926f;
            effect.unk1C = 0.1586000025f;
            effect.unk20 = 0.5690000057f;
            effect.unk24 = 0.7200000286f;
            effect.unk28 = 1.149000049f;
            effect.unk2C = 1.886599898f;
            effect.unk30 = 1.99119997f;
            effect.unk34 = 1.254899979f;
            dest->unk10 = func_15164780(&effect, 0, 0xFF, 1);
        }
    }
}

typedef struct {
    u8 unk0;
    u8 pad1;
    s16 unk2;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    u8 unk7;
} Spark_150FC614;

extern void func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* Per-frame update of an emitter: destroys it once its owner is gone or changed, otherwise fires
 * func_150FB8C0 from alternating sides whenever the spawn timer runs out. */
void func_150FC614(EmitterObj *obj) {
    EmitterState *state;
    struct127 *owner;
    Spark_150FC614 spark;

    state = &obj->state;
    owner = state->owner;
    if ((owner->interaction_state == 0) || (owner->id == 0xFF) || (owner->unique_id != state->ownerUniqueId) || (D_800C35EA == 1)) {
        obj->unkE = -1;
        return;
    }
    if (owner->unk84.uh != state->unk1D) {
        obj->unkE = -1;
        return;
    }

    state->spawnTimer -= D_800BE9A4;
    while (state->spawnTimer < 0.0f) {
        func_150FB8C0(owner, state->side, -state->spawnTimer, state->unkD, obj->unkC, obj->unk1);
        state->side ^= 1;
        if (state->other != NULL) {
            if (state->other->unk31C != NULL) {
                state->other->unk31C->unk1AA++;
            }
        }
        state->spawnTimer += 4.0f + (func_150ADA68() * 4.0f);
    }

    if (state->unk16 == -1) {
        return;
    }
    state->unk14 -= D_800BE9E4;
    if (state->unk14 < 0) {
        spark.unk0 = 1;
        func_150ADA20();
        spark.unk2 = 30;
        spark.unk5 = 1 << state->unk16;
        spark.unk4 = (func_150ADA20() % 6U) + 3;
        spark.unk6 = -1;
        func_151D8868(&spark, 0, obj->unkC, obj->unk1);
        func_150ADA20();
        state->unk14 = 250;
    }
}

/* Message payloads handled by func_150FC818, selected by its msgType argument. */
typedef union {
    struct {
        struct127 *obj;
        u8 uniqueId;
    } type0;
    struct {
        struct127 *objA;
        struct127 *objB;
        u8 uniqueIdA;
        u8 uniqueIdB;
    } type2D;
} EmitterMsg;

/* Message handler: type 0 destroys the emitter when it names the owner or the other object, type 0x2D
 * swaps two objects, retargeting the emitter's owner or other object. */
void func_150FC818(EmitterObj *obj, EmitterMsg *msg, u8 msgType) {
    EmitterState *state = &obj->state;
    struct127 *msgObj;
    struct127 *msgObjA;
    struct127 *owner;

    if (msgType == 0) {
        msgObj = msg->type0.obj;
        owner = state->owner;
        if ((owner == msgObj) || (state->ownerUniqueId == msg->type0.uniqueId)) {
            func_1516972C((struct102 *)obj);
        } else if (state->other != NULL) {
            if ((msgObj == state->other) || (state->otherUniqueId == msg->type0.uniqueId)) {
                func_1516972C((struct102 *)obj);
            }
        }
    } else if (msgType == 0x2D) {
        msgObjA = msg->type2D.objA;
        if (msgObjA == state->owner) {
            state->owner = msg->type2D.objB;
            state->ownerUniqueId = msg->type2D.uniqueIdB;
        } else if (state->owner == msg->type2D.objB) {
            state->owner = msgObjA;
            state->ownerUniqueId = msg->type2D.uniqueIdA;
        } else if (state->other != NULL) {
            if (msgObjA == state->other) {
                state->other = msg->type2D.objB;
                state->otherUniqueId = msg->type2D.uniqueIdB;
            } else if (state->other == msg->type2D.objB) {
                state->other = msgObjA;
                state->otherUniqueId = msg->type2D.uniqueIdA;
            }
        }
    }
}
s32 func_150FC930(struct260 *arg0, s32 arg1, s32 arg2);
void func_150FC974(struct260 *arg0);

extern void func_150FC974(struct260 *);
void func_150FC9A4(struct260 *arg0);

extern void func_15149368(struct260 *);
void func_150FC9D0(struct260 *arg0);
