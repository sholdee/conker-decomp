#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3F;

/* Directions a func_15150400/func_15152190 burst scatters in (256-step binary angles). */
typedef struct {
    /* 0x0 */ s16 yawBase;
    /* 0x2 */ s16 yawRange;
    /* 0x4 */ s16 pitchBase;
    /* 0x6 */ s16 pitchRange;
} AngleRange;

/* Config of func_15150400 (game_17CAF0.c): spawns countBase + rand(countRange) pieces around origin,
 * each with a random entry of the variant tables and a copy of extraData. */
typedef struct {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3F origin;
    /* 0x14 */ f32 speedBase;
    /* 0x18 */ f32 speedRange;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ f32 scaleBase;
    /* 0x2C */ f32 scaleRange;
    /* 0x30 */ f32 spin;
    /* 0x34 */ void *unk34;
    /* 0x38 */ f32 unk38; /* probability */
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ s32 *variantIds;
    /* 0x4C */ const f32 *variantScales;
    /* 0x50 */ u32 variantCount;
    /* 0x54 */ f32 radius;
    /* 0x58 */ u8 flags;
    /* 0x59 */ u8 unk59;
    /* 0x5A */ u8 pad5A[2];
    /* 0x5C */ s32 extraSize;
    /* 0x60 */ void *extraData; /* copied into each piece at +0x170 */
} Arg15150400;

/* Config of func_15152190 (game_17CAF0.c). */
typedef struct {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3F origin;
    /* 0x14 */ AngleRange angles;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 scaleBase;
    /* 0x34 */ f32 scaleRange;
    /* 0x38 */ f32 spin;
} Arg15152190;

/* Config of func_15153634 (game_17CAF0.c): spawns countBase + rand(countRange) particles at position. */
typedef struct {
    /* 0x00 */ s16 countBase;
    /* 0x02 */ s16 countRange;
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
    /* 0x38 */ Vec3F position;
    /* 0x44 */ s16 yawBase;
    /* 0x46 */ s16 pitchBase;
    /* 0x48 */ s16 yawRange;
    /* 0x4A */ s16 pitchRange;
    /* 0x4C */ f32 speedBase;
    /* 0x50 */ f32 speedRange;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s8 unk60;
    /* 0x61 */ s8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 pad65[3];
    /* 0x68 */ f32 extra; /* copied into each particle at +0xA8 */
} Arg15153634;

/* A piece spawned by func_15150400, as the handlers below see it. */
typedef struct {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0xB];
    /* 0x018 */ f32 unk18;
    /* 0x01C */ u8 pad1C[0x1C];
    /* 0x038 */ Vec3F position;
    /* 0x044 */ u8 pad44[0x1C];
    /* 0x060 */ s32 unk60;
    /* 0x064 */ u8 pad64[0x2];
    /* 0x066 */ u16 unk66;
    /* 0x068 */ u8 pad68[0xA8];
    /* 0x110 */ u8 unk110[0x60];
    /* 0x170 */ f32 burstChance; /* func_150C04C0's extraData */
} Piece;


extern void func_15150400(Arg15150400 *, AngleRange *, u8, s32);
extern void func_15152190(Arg15152190 *, const s32 *, const f32 *, u32, f32, s32, u8, s32);
extern void func_15153634(Arg15153634 *, u8, u8, s32);

/* An entry whose level ramps toward maxLevel while `rising` is set and back to 0 otherwise. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[0x7];
    /* 0x1F */ u8 level;
    /* 0x20 */ u8 pad20[0x4];
    /* 0x24 */ u8 rising;
    /* 0x25 */ u8 pad25;
    /* 0x26 */ u8 maxLevel;
    /* 0x27 */ u8 riseRate;
    /* 0x28 */ u8 pad28[0x4];
    /* 0x2C */ u8 age;
    /* 0x2D */ s8 unk14Rate;
    /* 0x2E */ s8 unk16Rate;
    /* 0x2F */ u8 fallRate;
} Struct150BF0F4;


void func_150BEF70(s32 arg0) {
}

/* K&R definition in game_19A8B0.c; callers pass all 49 arguments as ints. */
extern void func_1516D99C();

/* Starts two mirrored particle emitters at (0x1FDB, 0x25, +/-0x17C) whose timer is the low half of arg0. */
void func_150BEF7C(s32 arg0) {
    s32 i;
    s32 z;
    s32 timer;

    timer = arg0 & 0xFFFF;
    z = 0x17C;
    for (i = 0; i < 2; i++) {
        func_1516D99C(0x1FDB, 0x25, (s16)z, 0xD,
            0, 0x50, 0x8F, 0, 0, 0, 0, 0,
            8, 0xC8, 0xA, 0, 0, 0, 0, 0,
            0x28, 0x28, 4, 0, 0, 0, 0, 0x555,
            0x555, 0x555, 0x555, timer, 0x32, 0, 0xFF, 0x14,
            0xFA0, 0x7D0, 1, 6, 0, 1, 0, 0,
            0, 0, 3, 0xFF, 0);
        z = -z;
    }
}

/* Steps the entry by D_800BE9E4 frames; returns 1 once it has faded out or shrunk to nothing. */
s32 func_150BF0F4(Struct150BF0F4 *entry) {
    s32 level;
    s32 limit;
    s32 delta;
    s32 size;
    s32 age;

    level = entry->level;
    limit = entry->maxLevel;
    if (entry->rising) {
        if (level != limit) {
            level += D_800BE9E4 * entry->riseRate;
            if (level > limit) {
                level = limit;
            }
            entry->level = level;
        }
    } else if (level != 0) {
        level -= D_800BE9E4 * entry->fallRate;
        if (level < 0) {
            level = 0;
        }
        entry->level = level;
    }

    if (!entry->rising && level == 0) {
        return 1;
    }

    delta = entry->unk14Rate;
    delta *= D_800BE9E4;
    entry->unk14 += delta;
    size = entry->unk14;
    delta = entry->unk16Rate;
    delta *= D_800BE9E4;
    entry->unk16 += delta;
    if (size <= 0 || size <= 0) { /* sic: unk16 is never tested */
        entry->unk16 = 0;
        entry->unk14 = entry->unk16;
        return 1;
    }

    age = entry->age;
    age += D_800BE9E4;
    if (age >= 0x80) {
        age = 0x7F;
    }
    entry->age = age;
    return 0;
}

extern u8 D_800C35E8;

/* A vertex of the object's mesh: 16 bytes, position first. */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u8 pad6[0xA];
} Vtx150BF21C;

/* An object whose mesh is translated toward a target height and bobs while it is at rest. */
typedef struct {
    /* 0x000 */ f32 x;
    /* 0x004 */ f32 y;
    /* 0x008 */ f32 z;
    /* 0x00C */ u8 padC[0x4];
    /* 0x010 */ s16 rx;
    /* 0x012 */ s16 ry;
    /* 0x014 */ s16 rz;
    /* 0x016 */ u16 vtxCount;
    /* 0x018 */ u8 pad18[0x8];
    /* 0x020 */ Vtx150BF21C *dstVtx[2]; /* double-buffered, indexed by D_800BE9C0 */
    /* 0x028 */ Vtx150BF21C *srcVtx;
    /* 0x02C */ f32 sx;
    /* 0x030 */ f32 sy;
    /* 0x034 */ f32 sz;
    /* 0x038 */ u8 pad38[0x4];
    /* 0x03C */ s32 targetHeight;
    /* 0x040 */ u8 pad40[0x33];
    /* 0x073 */ u8 flags; /* bits 0-1: movement state */
    /* 0x074 */ u16 unk74;
    /* 0x076 */ u8 pad76[0x6];
    /* 0x07C */ f32 height;
    /* 0x080 */ f32 bobPhase;
    /* 0x084 */ f32 speed;
    /* 0x088 */ u8 pad88[0x54];
    /* 0x0DC */ s32 unkDC;
    /* 0x0E0 */ u8 padE0[0x44];
    /* 0x124 */ f32 unk124;
} Struct150BF21C;

extern void func_15114D24(Struct150BF21C *, s32, s32, s16, s16, s32);
extern void func_15114F04(Struct150BF21C *, s32, s32);
extern void func_150A7DA0(f32 [4][4], f32, f32, f32);
extern void func_15043F6C(f32 [4][4], f32, f32, f32, f32, f32, f32, f32, f32, f32);
extern void func_150A7A48(f32 [4][4], f32 [4][4], f32 [4][4]);
extern void func_150A7960(f32 [4][4], f32, f32, f32, f32 *, f32 *, f32 *);

/* Moves the object toward targetHeight (accelerating, with sounds), bobs it while at rest, then
 * writes its transformed mesh into the current vertex buffer. */
void func_150BF21C(Struct150BF21C *obj) {
    Vtx150BF21C *dst;
    Vtx150BF21C *src;
    f32 diff;
    f32 x;
    f32 height;
    f32 z;
    f32 speed;
    s32 paused;
    f32 bob;
    f32 translate[4][4];
    f32 model[4][4];
    f32 rate;
    s32 i;
    f32 bobXZ;

    height = obj->height;
    bob = 0.0f;
    rate = 0.05f;
    if (D_800C35EA == 0) {
        paused = FALSE;
    } else if (D_800C35E8 == 1) {
        paused = FALSE;
    } else {
        paused = TRUE;
    }
    if (D_800BE9B4 != 0) {
        height = obj->targetHeight;
        obj->height = height;
    }
    if (!paused) {
        diff = obj->targetHeight - height;
        if (diff == 0.0f) {
            obj->speed = 0.0f;
            obj->bobPhase += 0.05f;
            bob = ((height / 1500.0f) + 1.0f) * D_800BE9A4;
            if (bob < 0.0f) {
                bob = 0.0f;
            }
            func_15114D24(obj, -1, 0x7D00, 500, 2000, 0);
        } else {
            rate = 0.2f;
            obj->bobPhase = 0.0f;
            speed = obj->speed;
            if (diff < 0.0f) {
                if (speed > -60.0f) {
                    speed -= 5.0f;
                }
            } else {
                if (speed < 6.0f) {
                    speed += 0.5f;
                }
                func_15114D24(obj, 0xB9, 0x7D00, 500, 2000, 1);
            }
            obj->speed = speed;
            if (fabsf(speed) < fabsf(diff)) {
                height += speed * D_800BE9A4;
                if (speed < -25.0f) {
                    obj->unk124 = -10.0f;
                } else {
                    obj->unk124 = speed * 0.4000000060f;
                }
                obj->unkDC = 0x7D00;
                obj->flags &= ~3;
                obj->flags |= 2;
                if (obj->unk74 == 0) {
                    func_15114D24(obj, 0xB9, 0x3E80, 1000, 4000, 0);
                }
                func_15114F04(obj, 0x3E80, speed * 500.0f);
            } else {
                height = obj->targetHeight;
                if (speed < 0.0f) {
                    obj->unkDC = 0x80;
                }
                obj->flags &= ~3;
                obj->flags |= 3;
                obj->unk124 = 0.0f;
                func_15114D24(obj, -1, 0, 0, 0, 0);
            }
            obj->height = height;
            obj->speed = speed;
        }
        bobXZ = bob * 3.0f;
        obj->x += ((bobXZ * sinf(obj->bobPhase)) - obj->x) * rate;
        obj->y += (((bob * 4.0f) * sinf(obj->bobPhase * 1.100000024f)) - obj->y) * rate;
        obj->z += ((bobXZ * cosf(obj->bobPhase)) - obj->z) * rate;
    } else {
        height = 0.0f;
    }
    func_150A7DA0(translate, 0.0f, height, 0.0f);
    func_15043F6C(model, obj->x, obj->y, obj->z, obj->sx, obj->sy, obj->sz, obj->rx, obj->ry, obj->rz);
    func_150A7A48(translate, model, translate);
    dst = obj->dstVtx[D_800BE9C0];
    src = obj->srcVtx;
    for (i = 0; i < obj->vtxCount; i++) {
        func_150A7960(translate, src->x, src->y, src->z, &x, &height, &z);
        dst->x = x;
        dst->y = height;
        dst->z = z;
        src++;
        dst++;
    }
}

typedef struct {
    /* 0x000 */ u8 pad0[0x73];
    /* 0x073 */ u8 flags; /* bits 0-1: mode */
    /* 0x074 */ u8 pad74[0x34];
    /* 0x0A8 */ f32 unkA8;
    /* 0x0AC */ u8 padAC[0x78];
    /* 0x124 */ void *unk124;
} Struct150BF760;

/* 0x30-byte entries of D_800D20FC (structs.h types it as struct258 *). */
typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 pad3[0x2D];
} SpawnSlot;

void func_1511B51C(Struct150BF760 *);
void *func_15195FB0(Struct150BF760 *, s32, s32, s32, s32, s32, s32);
void func_150825C0(s32, s32);

/* Drives object 0x14 according to the mode in obj->flags, spawning it first when a mode needs it,
 * and sets obj->unkA8 to a sine of the object's animation progress. */
void func_150BF760(Struct150BF760 *obj) {
    struct127 *actor;
    struct197 *anim;
    s32 mode;
    s32 slot;
    f32 progress = 0.0f;

    if (D_800C35EA != 1 || D_800C35E8 == 1) {
        func_1511B51C(obj);
        if (obj->unk124 == NULL) {
            obj->unk124 = func_15195FB0(obj, D_800902B8, 1, -1, 0, 0, -8);
        }
        actor = func_15083E90(0x14);
        mode = obj->flags & 3;
        if (actor != NULL) {
            anim = actor->unk2D0;
            progress = anim->unk8 / (anim->unk18 - 2.0f);
        }
        obj->unkA8 = 0.0f;
        if (mode == 2) {
            if (progress <= 0.75f) {
                obj->unkA8 = -sinf(progress * 3.141592741f * 1.333333254f) * 10.0f;
            }
            if (actor == NULL) {
                slot = func_15083E0C(0x14);
                ((SpawnSlot *)D_800D20FC)[slot].unk2 = 0;
                func_150825C0(slot, 0);
                actor = func_15083E90(0x14);
            }
            if (actor->unk232 != 2) {
                actor->unk232 = 2;
                actor->unk218 = NULL;
            }
        } else if (mode == 3) {
            if (actor != NULL && progress >= 1.0f) {
                func_15060F28(actor, 0);
            }
        } else if (mode == 0) {
            if (progress >= 0.25f) {
                obj->unkA8 = sinf(progress * 4.188789368f - 4.188789368f) * 10.0f;
            }
            if (actor != NULL && progress >= 1.0f) {
                func_15060F28(actor, 0);
            }
        } else if (mode == 1) {
            if (progress >= 0.25f) {
                obj->unkA8 = sinf(progress * 4.188789368f - 4.188789368f) * 10.0f;
            }
            if (actor == NULL) {
                slot = func_15083E0C(0x14);
                ((SpawnSlot *)D_800D20FC)[slot].unk2 = 0;
                func_150825C0(slot, 0);
                actor = func_15083E90(0x14);
            }
            if (actor == NULL && actor->unk232 != 3) { /* sic: the original tests actor == NULL */
                actor->unk232 = 3;
                actor->unk218 = NULL;
            }
        }
    }
}

/* A 0xA0-byte entry of D_800DBEF4 (structs.h types it as struct131): one of a pair of platforms that
 * shakes and sinks. The partner entry follows it. */
typedef struct {
    /* 0x00 */ f32 shakeX;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 shakeZ;
    /* 0x0C */ u8 padC[0x6];
    /* 0x12 */ s16 height;
    /* 0x14 */ u8 pad14[0x28];
    /* 0x3C */ s32 unk3C; /* high half: kind */
    /* 0x40 */ u8 pad40[0xF];
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 pad50[0x1E];
    /* 0x6E */ u8 unk6E;
    /* 0x6F */ u8 pad6F[0x4];
    /* 0x73 */ u8 flags; /* bits 0-1: state, bit 2: held down */
    /* 0x74 */ u8 pad74[0x8];
    /* 0x7C */ s32 home; /* low half: rest height; bit 31: initialised */
    /* 0x80 */ s32 unk80; /* sinking timer; on the partner, its offset from the lowered height */
    /* 0x84 */ s32 unk84;
    /* 0x88 */ u8 pad88[0x18];
} Platform150BFA7C; /* size 0xA0 */

typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ s8 unk6;
} Fx151D8868;

extern u8 D_800BE9EB;
extern void func_1508EE0C(s32, s32);
extern void func_15164F0C(u8, u8, void *, u8, s32);
extern void func_1518B6B0(f32, f32, f32, s32, s32);
extern void func_1518CD20(Platform150BFA7C *, s32, s32);
extern void *func_151D8868(Fx151D8868 *, s32, s32, s32);
extern s32 func_151EF610(void);

/* Platform update. State 0 rests at the home height, 2 shakes then sinks by `travel`, 3 stays down
 * and 1 rises back. Kind 1 platforms just follow their partner's offset. */
void func_150BFA7C(Platform150BFA7C *platform) {
    s16 lowered;
    s16 top;
    s16 bottom;
    s32 pad; // matching: unused stack slot
    s32 travel;
    s32 step;
    s32 diff;
    s32 state;
    s32 kind;
    Fx151D8868 fx;

    step = 0x10;
    kind = (platform->unk3C >> 16) & 0xFFFF;
    state = platform->flags & 3;
    travel = 0;
    if (kind == 0) {
        travel = 0x82;
    } else if (kind == 2) {
        travel = 0x622;
        step = 0x30;
    }
    if (platform->home == 0) {
        platform->home = platform->height | 0x80000000;
    }
    top = (s16)(platform->home & 0xFFFF);
    lowered = top - 0x50;
    bottom = lowered - travel;
    if (kind == 1) {
        diff = top - platform->height;
        if (diff != platform->unk80) {
            platform->height = top + platform->unk80;
        }
        return;
    }
    if ((platform->unk4F & 4) && D_800CC2D0[0].unk31C->unk57 == 1) {
        func_1518B6B0(D_800CC2D0[0].x_position, D_800CC2D0[0].y_position, D_800CC2D0[0].z_position, 0xFF, 0);
        if (state == 0) {
            func_15164F0C(3, D_800BE9EB, NULL, 0xFF, 0);
            fx.unk0 = 1;
            fx.unk2 = 60;
            fx.unk5 = 1;
            fx.unk4 = 8;
            fx.unk6 = -1;
            func_151D8868(&fx, 0, 0xFF, 0);
        }
    }
    if (state == 3) {
        if (lowered != platform->height) {
            platform->height = lowered;
        }
        if (!(platform->unk4F & 4) && !(platform->flags & 4)) {
            state = 1;
        }
    } else if (state == 0) {
        if (top != platform->height) {
            platform->height = top;
        }
    } else if (state == 2) {
        platform->unk80 += D_800BE9E4;
        if (platform->unk80 < 0) {
            platform->shakeX = ((f32)(func_151EF610() % 5) - 2.0f) * 0.125f;
            platform->shakeZ = ((f32)(func_151EF610() % 5) - 2.0f) * 0.125f;
        } else {
            if (platform->unk80 < 40) {
                platform->shakeX = ((f32)(func_151EF610() % 5) - 2.0f) / 12.0f;
                platform->shakeZ = ((f32)(func_151EF610() % 5) - 2.0f) / 12.0f;
            } else {
                platform->shakeZ = 0.0f;
                platform->shakeX = 0.0f;
            }
            if (platform->height < lowered) {
                if (kind == 0) {
                    step = ((platform->height - bottom + 0x10) * step) / travel;
                    if (platform->unk84 == 0) {
                        func_1518CD20(platform, 0xFF, 0);
                        platform->unk84 = 1;
                    }
                }
                platform->height -= step;
                platform[1].unk80 = platform->height - lowered;
            } else {
                platform->height -= step;
                platform->unk84 = 0;
            }
            if (platform->height <= bottom || step == 0) {
                platform->height = bottom;
                diff = bottom - lowered;
                top += diff;
                platform->home += diff;
                platform->home |= 0x80000000;
                platform->unk80 = 0;
                platform[1].home += diff;
                platform[1].home |= 0x80000000;
                platform[1].unk80 = 0;
                if (kind == 4 && state != 3) {
                    func_1518CD20(platform, 0xFF, 0);
                }
                state = 3;
                platform->shakeZ = 0.0f;
                platform->shakeX = 0.0f;
                if (kind == 2) {
                    platform->unk6E = 1;
                    platform[1].unk6E = 1;
                    func_1508EE0C(2, (u16)(platform - (Platform150BFA7C *)D_800DBEF4));
                    func_1508EE0C(2, (u16)(&platform[1] - (Platform150BFA7C *)D_800DBEF4));
                }
            }
        }
    } else if (state == 1) {
        state = 1; // matching: redundant store steers register allocation
        platform->height += 3;
        if (platform->height >= top) {
            platform->height = top;
            state = 0;
        }
    }
    platform->flags &= ~3;
    platform->flags |= state;
}

void func_15136C3C(struct127 *, s32, s32, s32, s32, s32, s32, s32);

/* func_15136C3C on the object, then a burst of particles at its x/z, unk180 + 105 high. */
void func_150BFFE0(struct127 *obj) {
    Vec3F pos;
    Arg15153634 spawn;

    pos.x = obj->x_position;
    pos.y = obj->unk180 + 70.0f;
    pos.z = obj->z_position;
    func_15136C3C(obj, 0, 0, 1, 0, 0, 0xFF, 1);
    spawn.countBase = 0x11;
    spawn.countRange = 7;
    spawn.unk4 = 0x6C;
    spawn.unk6 = 0x5103;
    spawn.unk8 = 0x200005;
    spawn.unkC = 0;
    spawn.unk10 = 0x28;
    spawn.unk12 = 0x14;
    spawn.unk14 = 0;
    spawn.unk18 = 0;
    spawn.unk1F = 0xFF;
    spawn.unk1C = 0x4E;
    spawn.unk1D = 0x54;
    spawn.unk1E = 0x7B;
    spawn.unk20 = 0xA4;
    spawn.unk21 = 0xA1;
    spawn.unk22 = 0xC8;
    spawn.unk23 = 0x9B;
    spawn.unk24 = 0x64;
    spawn.unk25 = 0xFF;
    spawn.unk26 = 0x1E;
    spawn.unk28 = 8;
    spawn.unk2A = 0x1E;
    spawn.unk2C = 1.019999981f;
    spawn.unk30 = 451.0f;
    spawn.unk34 = 501.0f;
    spawn.position.x = pos.x;
    spawn.position.y = pos.y + 35.0f;
    spawn.position.z = pos.z;
    spawn.yawBase = 0;
    spawn.pitchBase = -0x14;
    spawn.yawRange = 0xFF;
    spawn.pitchRange = 0x1E;
    spawn.speedBase = 15.0f;
    spawn.speedRange = 39.0f;
    spawn.unk54 = 0.3540000021f;
    spawn.unk58 = 0.6260000467f;
    spawn.unk5C = 0x40040E07;
    spawn.unk60 = 0x10;
    spawn.unk61 = -1;
    spawn.unk62 = 8;
    spawn.unk63 = 6;
    spawn.unk64 = 1;
    spawn.extra = 0.8775510192f;
    func_15153634(&spawn, 0xFF, 0xFF, 1);
}

/* Piece handler: two particle bursts 20 units above `y` at the piece's position. */
s32 func_150C01DC(Piece *piece, s32 arg1, s32 arg2, s32 arg3, f32 y) {
    Vec3F pos;
    Arg15152190 burst;

    pos.x = piece->position.x;
    pos.y = y + 20.0f;
    pos.z = piece->position.z;
    {
    const s32 ids[1] = { 5 };
    const f32 scales[1] = { 1.0f };

    burst.countBase = 5;
    burst.countRange = 4;
    burst.origin = pos;
    burst.angles.yawBase = 0;
    burst.angles.yawRange = 0xFF;
    burst.angles.pitchBase = -0x40;
    burst.angles.pitchRange = 0x22;
    burst.speedBase = 12.0f;
    burst.speedRange = 8.0f;
    burst.unk24 = -1.697000027f;
    burst.unk28 = 0.4990000129f;
    burst.unk2C = 0x1E;
    burst.unk2E = 0xF;
    burst.scaleBase = 0.8000000119f;
    burst.scaleRange = 1.200000048f;
    burst.spin = 15.29900074f;
    func_15152190(&burst, ids, scales, 1, 0.0f, 1, piece->unkC, piece->unk1);
    }
    {
    Arg15153634 spawn;

    spawn.countBase = 5;
    spawn.countRange = 8;
    spawn.unk4 = 0x6C;
    spawn.unk6 = 0x5103;
    spawn.unk8 = 0x200005;
    spawn.unkC = 0;
    spawn.unk10 = 0x1E;
    spawn.unk12 = 0xF;
    spawn.unk14 = 0;
    spawn.unk18 = 0;
    spawn.unk1F = 0xFF;
    spawn.unk1C = 0x4E;
    spawn.unk1D = 0x54;
    spawn.unk1E = 0x7B;
    spawn.unk20 = 0xA4;
    spawn.unk21 = 0xA1;
    spawn.unk22 = 0xC8;
    spawn.unk23 = 0x64;
    spawn.unk24 = 0x9B;
    spawn.unk25 = 0xFF;
    spawn.unk26 = 0xE;
    spawn.unk28 = 0x12;
    spawn.unk2A = 0xE;
    spawn.unk2C = 1.046154022f;
    spawn.unk30 = 218.0f;
    spawn.unk34 = 305.0f;
    spawn.position = pos;
    spawn.yawBase = 0;
    spawn.pitchBase = -0x28;
    spawn.yawRange = 0xFF;
    spawn.pitchRange = 0x28;
    spawn.speedBase = 19.0f;
    spawn.speedRange = 8.0f;
    spawn.unk54 = 0.2620000243f;
    spawn.unk58 = 0.2620000243f;
    spawn.unk5C = 0x840E07;
    spawn.unk60 = 0x10;
    spawn.unk61 = -1;
    spawn.unk62 = 8;
    spawn.unk63 = 6;
    spawn.unk64 = 1;
    spawn.extra = 0.8977670074f;
    func_15153634(&spawn, 0xFF, piece->unkC, piece->unk1);
    }
    return 0;
}

/* Scatters 9-13 pieces of variant `id` from origin; each piece carries a 0.65 burst chance. */
void func_150C04C0(Vec3F *origin, void *arg1, s32 id, u8 arg3, u8 arg4, s32 arg5) {
    Arg15150400 config;
    AngleRange angles;
    s32 ids[1];
    const f32 scales[1] = { 1.0f };
    f32 burstChance;
    u8 flags;

    ids[0] = id;
    config.countBase = 9;
    config.countRange = 4;
    burstChance = 0.6500000358f;
    config.origin = *origin;
    config.speedBase = 4.0f;
    config.speedRange = 7.0f;
    config.unk1C = -1.280000091f;
    config.unk20 = 0.3360000253f;
    config.scaleBase = 0.6000000238f;
    config.scaleRange = 0.4500000179f;
    config.unk24 = 0x50;
    config.unk26 = 0x3C;
    config.unk34 = arg1;
    config.variantIds = ids;
    config.variantScales = scales;
    config.variantCount = 1;
    config.spin = 17.07700157f;
    config.unk38 = 0.7520000339f;
    config.unk3C = 0.2040000111f;
    config.unk40 = 0.1640000045f;
    config.unk44 = 10.0f;
    config.radius = 78.0f;
    if (arg3 != 0) {
        flags = 1;
    } else {
        flags = 0;
    }
    config.flags = flags;
    config.unk59 = 0xE;
    config.extraSize = 4;
    config.extraData = &burstChance;
    angles.yawBase = 0;
    angles.yawRange = 0xFF;
    angles.pitchBase = -0x32;
    angles.pitchRange = 0x23;
    func_15150400(&config, &angles, arg4, arg5);
}

extern s32 func_15133B98(Piece *, s32, s32, s32, f32, s32);

/* Piece handler: with probability burstChance, bursts into smaller pieces and particles 20 units above
 * `y`; otherwise falls back to func_15133B98. */
s32 func_150C0648(Piece *piece, s32 arg1, s32 arg2, s32 arg3, f32 y, s32 arg5) {
    u8 flags;

    if (func_150ADA68() < piece->burstChance) {
        {
            Arg15150400 config;
            AngleRange angles;
            s32 ids[1];
            const f32 scales[1] = { 1.0f };

            ids[0] = piece->unk66;
            config.countBase = 2;
            config.countRange = 0;
            config.origin.x = piece->position.x;
            config.origin.y = y + 20.0f;
            config.origin.z = piece->position.z;
            config.speedBase = 5.0f;
            config.speedRange = 4.0f;
            config.unk1C = -1.210000038f;
            config.unk20 = 0.4720000327f;
            config.unk24 = 0x28;
            config.unk26 = 0x1E;
            config.scaleBase = piece->unk18 * 0.3600000143f;
            config.scaleRange = piece->unk18 * 0.4130000174f;
            config.spin = 31.52700233f;
            config.unk34 = piece->unk110;
            config.unk38 = 1.0f;
            config.unk3C = 0; // matching: integer zeros here; 0.0f changes the code
            config.unk40 = 0;
            config.unk44 = 10.0f;
            config.variantIds = ids;
            config.variantScales = scales;
            config.variantCount = 1;
            config.radius = 0;
            if (piece->unk60 & 0x800) {
                flags = 1;
            } else {
                flags = 0;
            }
            config.flags = flags;
            config.unk59 = 0xA;
            config.extraSize = 0;
            config.extraData = NULL;
            angles.yawBase = 0;
            angles.yawRange = 0xFF;
            angles.pitchBase = -0x2A;
            angles.pitchRange = 0x19;
            func_15150400(&config, &angles, piece->unkC, piece->unk1);
            {
            Arg15153634 spawn;
            f32 spawnY;

            spawnY = y + 20.0f;
            spawn.countBase = 1;
            spawn.countRange = 2;
            spawn.unk4 = 0x6C;
            spawn.unk6 = 0x5103;
            spawn.unk8 = 0x200005;
            spawn.unkC = 0;
            spawn.unk10 = 0x1E;
            spawn.unk12 = 0x10;
            spawn.unk14 = 0;
            spawn.unk18 = 0;
            spawn.unk1F = 0xFF;
            spawn.unk1C = 0x4E;
            spawn.unk1D = 0x54;
            spawn.unk1E = 0x7B;
            spawn.unk20 = 0xA4;
            spawn.unk21 = 0xA1;
            spawn.unk22 = 0xC8;
            spawn.unk23 = 0xC8;
            spawn.unk24 = 0x37;
            spawn.unk25 = 0xFF;
            spawn.unk26 = 0x14;
            spawn.unk28 = 0xC;
            spawn.unk2A = 0x14;
            spawn.unk2C = 1.024461985f;
            spawn.unk30 = 178.0f;
            spawn.unk34 = 91.0f;
            spawn.position.x = piece->position.x;
            spawn.position.y = spawnY;
            spawn.position.z = piece->position.z;
            spawn.yawBase = 0;
            spawn.pitchBase = -0x12;
            spawn.yawRange = 0xFF;
            spawn.pitchRange = 0xB;
            spawn.speedBase = 4.0f;
            spawn.speedRange = 6.0f;
            spawn.unk54 = 0.006000000052f;
            spawn.unk58 = 0.0f;
            spawn.unk5C = 0x40040E07;
            spawn.unk60 = 0x10;
            spawn.unk61 = -1;
            spawn.unk62 = 8;
            spawn.unk63 = 6;
            spawn.unk64 = 1;
            spawn.extra = 0.9535999894f;
            func_15153634(&spawn, 0xFF, piece->unkC, piece->unk1);
            }
        }
        return 0;
    }
    return func_15133B98(piece, arg1, arg2, arg3, y, arg5);
}
