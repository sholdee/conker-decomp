#include <ultra64.h>
/* functions.h declares func_150ADA20 as returning u8, but it returns a full word and the
 * callers here use all of it (divu on v0), so this file declares it locally. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

/* Surface hit record from a ground/collision probe (cf. GroundHit in game_1D6E80.c). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 unk4[0x18];
    /* 0x1C */ u8 flags;
} FxSurfaceHit;

/* Per-impact-type effect selection: indices into the tables below, -1 for none. */
typedef struct {
    /* 0x00 */ s8 first;      /* D_800A9040 index */
    /* 0x01 */ s8 firstStyle; /* D_800A9068 index (read by func_151AC078) */
    /* 0x02 */ s8 second;     /* D_800A9158 index */
    /* 0x03 */ s8 unk3;
    /* 0x04 */ s8 impact;     /* D_800A925C index */
} FxImpactSet; // size 0x5

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ s16 countBase;
    /* 0x06 */ u16 countRange;
} FxSpawnRange; // size 0x8

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x08 */ f32 unk8;
} FxImpactEntry; // size 0xC

/* Passed through func_1514C678 to the spawn callbacks (e.g. func_151AC078). */
typedef struct {
    /* 0x00 */ FxSurfaceHit *hit;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 kind;
} FxSpawnPayload;

extern FxImpactSet D_800A9020[];
extern FxSpawnRange D_800A9040[];
extern FxSpawnRange D_800A9158[];
extern FxImpactEntry D_800A925C[];

u8 func_151D8E20(void);
void func_1514C678(f32 x, f32 y, f32 z, f32, s32, s32, s32 count, s32, s32, f32, FxSpawnPayload *, s32);
void func_151DBE80(u8, f32, f32, s16, vertex *, void *, s32, s32, s32, s32);

/* Spawns the effects listed for impact type `type` at `pos` on surface `hit`. */
void func_151ABE40(vertex *pos, FxSurfaceHit *hit, s32 type, s32 arg3, s32 arg4) {
    FxImpactSet *set;
    u8 kind;

    kind = func_151D8E20();
    if (hit == NULL) {
        return;
    }
    set = &D_800A9020[type];
    if (set->first != -1) {
        FxSpawnPayload payload;

        payload.hit = hit;
        payload.unk4 = arg4;
        payload.kind = kind;
        {
            FxSpawnRange *range = &D_800A9040[set->first];

            func_1514C678(pos->x, pos->y, pos->z, range->unk0, 0, 255,
                          func_150ADA20() % (range->countRange + 1) + range->countBase, 11, type, 0.0f,
                          &payload, (u8)arg3);
        }
    }
    if (set->second != -1) {
        FxSpawnPayload payload;

        payload.hit = hit;
        payload.unk4 = arg4;
        payload.kind = kind;
        {
            FxSpawnRange *range = &D_800A9158[set->second];

            // matching: a plain (u8)arg3 is CSE'd with the first call's and spilled
            func_1514C678(pos->x, pos->y, pos->z, range->unk0, 0, 255,
                          func_150ADA20() % (range->countRange + 1) + range->countBase, 12, type, 0.0f,
                          &payload, (u8)(u16)arg3);
        }
    }
    if (set->impact != -1 && (hit->flags & 1)) {
        FxImpactEntry *impact = &D_800A925C[set->impact];

        func_151DBE80(kind, impact->unk0, impact->unk8, impact->unk4, pos, hit->unk4, 150, 0, (u8)arg3, arg4);
    }
}

/* Random ranges for one spray particle: each value is base + rand() % (range + 1) or
 * base + randf() * range. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 pitchBase;
    /* 0x04 */ u16 pitchRange;
    /* 0x08 */ f32 speedBase;
    /* 0x0C */ f32 speedRange;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 chance;
} FxSprayParams; // size 0x30

typedef struct {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 pad16[2];
} FxSpraySpawn;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 vel[3];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 flags;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 pad1C[4];
} FxSprayPart;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 pad1E[2];
} FxSprayStyle;

extern FxSprayParams D_800A9068[];
extern u8 D_800AB3F4[];
extern u8 D_800AB404[];
s32 func_15147DA0(FxSpraySpawn *, FxSprayPart *, s32, s32, s32, s32, s32, s32, s32, s32, s32, FxSprayStyle *,
                  FxSurfaceHit *, s32, s32);

/* Spawn callback: launches one spray particle at (x, y, z) heading along `yaw`, with random
 * pitch and speed from the impact type's spray parameters. */
s32 func_151AC078(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw, s32 arg9,
                  s32 argA, s32 type, s32 argC, FxSpawnPayload *payload, u8 argE) {
    FxSprayParams *params;
    FxSpraySpawn spawn;
    FxSprayPart part;
    f32 roll;
    FxSprayStyle style;
    f32 sinYaw;
    f32 cosYaw;
    f32 speed;
    f32 horizontal;
    f32 vertical;
    s16 pitch;
    f32 sinPitch;
    f32 cosPitch;
    u32 random;
    s32 firstFlag;
    s32 secondFlag;

    {
        s32 colorFlag;

        params = &D_800A9068[D_800A9020[type].firstStyle];
        sinYaw = func_151423D8(yaw);
        cosYaw = func_151423D8(yaw - 0x40);
        style.unk0 = 0;
        style.unk4 = 1;
        style.unk8 = 0x160600;
        style.unkC = 1;
        style.unk10 = 16;
        style.unk14 = 128;
        style.unk18 = 32;
        style.unk1C = 0;
        style.unk1D = 9;
        spawn.unk10 = 1;
        spawn.unkE = 1;
        spawn.pos[1] = y;
        part.unk14 = y;
        spawn.pos[0] = x;
        spawn.pos[2] = z;
        part.unk19 = D_800AB3F4[payload->kind];
        part.unk1A = 255;
        colorFlag = D_800AB404[payload->kind] ? 0x20 : 0;
        part.flags = colorFlag | 8;
    }
    roll = func_150ADA68();
    if (roll < params->chance) {
        part.flags |= 3;
        firstFlag = 6;
        secondFlag = 2;
    } else {
        firstFlag = 0;
        secondFlag = 0;
        part.flags &= ~3;
    }
    random = func_150ADA20();
    part.unk1B = random % (params->unk1 + 1) + params->unk0;
    random = func_150ADA20();
    pitch = random % (params->pitchRange + 1) + params->pitchBase;
    sinPitch = func_151423D8(pitch);
    cosPitch = func_151423D8(pitch - 0x40);
    speed = func_150ADA68() * params->speedRange + params->speedBase;
    horizontal = speed * sinPitch;
    part.vel[0] = horizontal * cosYaw;
    vertical = -speed * cosPitch;
    part.vel[1] = vertical;
    part.vel[2] = horizontal * sinYaw;
    random = func_150ADA20();
    spawn.unk15 = random % (params->unk14 + 1) + params->unk10;
    random = func_150ADA20();
    spawn.unkC = random % (params->unk1A + 1) + params->unk18;
    part.unk0 = func_150ADA68() * params->unk20 + params->unk1C;
    part.unk10 = func_150ADA68() * params->unk28 + params->unk24;
    func_15147DA0(&spawn, &part, 0, 1, 8, 0, firstFlag, secondFlag, 0, 0, 0, &style, payload->hit, argE, payload->unk4);
    return 1;
}

s32 func_151AC3CC(s16 *arg0) {
    u8 *ptr = *(u8 **)((u8 *)arg0 + 0x98);
    s32 val = arg0[0x1C / 2] << 3;
    if (val >= 0x100) {
        val = 0xFF;
    }
    if (val < ptr[0x1B]) {
        ptr[0x1B] = val;
    }
    return 1;
}

void func_151DAB58(u8, f32, u8, f32 *, s32, u8, s32);
typedef struct {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    f32 field_0x0C;
    f32 field_0x10;
} ObjectEffectSample;

typedef struct {
    f32 field_0x00;
    char pad_0x04[0x17];
    u8 field_0x1B;
    char pad_0x1C[0x4];
    u8 field_0x20;
} ObjectEffectPayloadData;

typedef struct {
    char pad_0[0x1];
    u8 field_0x01;
    char pad_0x02[0xA];
    u8 field_0x0C;
    char pad_0x0D[0x20];
    s8 field_0x2D;
    char pad_0x2E[0x66];
    ObjectEffectSample *field_0x94;
    ObjectEffectPayloadData *field_0x98;
} ObjectEffectState;

s32 func_151AC408(ObjectEffectState *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    ObjectEffectPayloadData *ptr = arg0->field_0x98;
    ObjectEffectSample *arr = arg0->field_0x94;
    f32 sp4C[3];
    u8 color;
    s32 life;

    sp4C[0] = arr[arg0->field_0x2D].field_0x00;
    sp4C[1] = arg4;
    sp4C[2] = arr[arg0->field_0x2D].field_0x08;
    color = func_151D8E20();
    if (func_150ADA20() & 1) {
        life = (func_150ADA20() % 41) + 80;
        func_151D9B8C(color, ptr->field_0x00 * 6.0f, ptr->field_0x1B, (void *)arg5, (struct17 *)sp4C, life, 1, 1, 0, arg0->field_0x0C, arg0->field_0x01);
    } else {
        func_151DAB58(color, ptr->field_0x00 * 1.5f, ptr->field_0x1B, sp4C, 1, arg0->field_0x0C, arg0->field_0x01);
    }
    ptr->field_0x20 = 4;
    return 1;
}
