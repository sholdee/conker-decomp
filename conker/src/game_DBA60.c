#include <ultra64.h>

#define func_150ADA20 func_150ADA20_u8_unused
#include "functions.h"
#undef func_150ADA20
#include "variables.h"

/* Per-frame input handling for a player-controlled struct108 (button queries via func_1509BE40). */
void func_150AE5B0(struct108 *arg0) {
    if (!(D_800D2E4C->unk4 & 0x80)) {
        if (func_1509BE40(0, 0x2000, 0xBB) != -1) {
            if (func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 8) != 0) {
                arg0->unk84 |= 0x1000000;
                func_151254F4(arg0, D_800CC335 - 1);
            }
        } else if (func_151239CC(arg0, 8) != 0) {
            func_151254F4(arg0, 0);
        }
    }
    arg0->unk84 &= ~0x4000;
    if (!(D_800D2E4C->unk1 & 4)) {
        if (func_1509BE40(1, 0x2000, 0x95, func_1509BE40(0, 0x2014, 0xB7) | 0x2000) != 0) {
            arg0->unk84 |= 0x1000000;
            if ((arg0->unk2C & 1) && func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 0) != 0) {
                arg0->unk1B4 = 3;
                arg0->unk84 &= ~4;
                func_15124B18(arg0);
            }
        } else if (func_151239CC(arg0, 0) != 0) {
            func_15124B18(arg0);
            arg0->unk84 &= ~0x1000000;
        }
    }
    if (func_1509BE40(1, 0x4082, 6, 0x9000) != 0) {
        arg0->unk84 |= 0x10000;
    } else {
        arg0->unk84 &= ~0x10000;
    }
}


/* Spawn parameters for func_15147DA0. */
typedef struct {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ s16 lifetime;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 pad16[2];
} Spawn150AE790;

/* Motion parameters for func_15147DA0. */
typedef struct {
    /* 0x00 */ f32 size;
    /* 0x04 */ f32 velX;
    /* 0x08 */ f32 velY;
    /* 0x0C */ f32 velZ;
    /* 0x10 */ f32 scale;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 pad1C[4];
} Motion150AE790;

/* Render options for func_15147DA0. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
} Style150AE790;

/* Emission request for func_15167D84 (field names as in game_109220.c's Game150DC558Emission). */
typedef struct {
    /* 0x00 */ s32 texture;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s16 frame;
    /* 0x0E */ s16 frame_rate;
    /* 0x10 */ s16 position[3];
    /* 0x16 */ s16 velocity_x;
    /* 0x18 */ s16 velocity_z;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u8 fraction_x;
    /* 0x1D */ u8 fraction_y;
    /* 0x1E */ u8 fraction_z;
    /* 0x1F */ s8 callback;
    /* 0x20 */ s16 velocity_y;
    /* 0x22 */ s16 acceleration;
    /* 0x24 */ s16 width;
    /* 0x26 */ s16 height;
    /* 0x28 */ s16 lifetime;
    /* 0x2A */ s8 fade;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 red;
    /* 0x2D */ u8 green;
    /* 0x2E */ u8 blue;
    /* 0x2F */ u8 alpha;
    /* 0x30 */ s8 environment[3];
    /* 0x34 */ s16 flags;
} Emission150AE790;

/* Link from an effect to the actor it is attached to. */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id when the link was made */
    /* 0x05 */ u8 bone;
} OwnerLink;

/* Effect object attached to an actor (created by func_150AEB9C). */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ OwnerLink link;
} AttachedEffect;

extern s32 D_8008CB14;
extern s32 func_150ADA20(void);
void func_15142314(struct255 *bones, s32 bone, f32 *out);
extern s32 func_15147DA0(Spawn150AE790 *, Motion150AE790 *, s32, s32, s32, s32, s32, s32, s32, s32, s32, Style150AE790 *, s32, s32, s32);
s32 func_15167D84(void *, s32, s32, s32, s32, s32);

/* Bursts 12-20 particles plus one emission from the owner's bone, when the owner is still the same actor. */
void func_150AE790(AttachedEffect *effect) {
    OwnerLink *link;
    struct127 *owner;
    Spawn150AE790 spawn;
    Motion150AE790 motion;
    s32 pad; // matching: unused stack slot
    Style150AE790 style;
    f32 sinYaw;
    f32 cosYaw;
    f32 speed;
    f32 horizontal;
    f32 sinPitch;
    f32 cosPitch;
    Emission150AE790 emission;
    s32 count;
    u8 yaw;
    u8 pitch;
    struct255 *bones;
    f32 duration;
    f32 distance;

    link = &effect->link;
    if (effect->link.owner == NULL) {
        return;
    }
    owner = link->owner;
    if (link->owner->interaction_state == 0) {
        return;
    }
    bones = owner->unk1D4;
    if (bones != NULL && link->owner->unique_id == link->ownerId) {
        func_15142314(link->owner->unk1D4, link->bone, spawn.pos);
        spawn.unk10 = 1;
        spawn.unkE = 1;
        motion.unk19 = 0;
        motion.unk1A = 0xFF - (((u32)link->owner->unk184 >> 5) & 3) * 64;
        motion.unk14 = link->owner->unk180;
        motion.unk18 = 8;
        motion.alpha = 0xFF;
        count = func_150ADA20() % 9U + 12;
        style.unk0 = 0;
        style.unk4 = 1;
        style.unk8 = 0x160600;
        style.unkC = 3;
        style.unk10 = 0x10;
        style.unk14 = 0x80;
        style.unk18 = 0x20;
        style.unk1C = 0;
        style.unk1D = 9;
        for (; count != 0; count--) {
            yaw = func_150ADA20() & 0xFF;
            pitch = func_150ADA20() % 51U;
            sinYaw = func_151423D8(yaw);
            cosYaw = func_151423D8(yaw - 0x40);
            sinPitch = func_151423D8(pitch);
            cosPitch = func_151423D8(pitch - 0x40);
            speed = func_150ADA68() * 0.5f + 18.79999924f;
            spawn.unk15 = 4;
            spawn.lifetime = (func_150ADA20() & 0xF) + 35;
            motion.size = func_150ADA68() * 2.299999952f + 2.900000095f;
            motion.scale = func_150ADA68() * 0.1000000015f + 0.6999999881f;
            horizontal = speed * sinPitch;
            motion.velX = horizontal * cosYaw;
            motion.velY = -speed * cosPitch;
            motion.velZ = horizontal * sinYaw;
            func_15147DA0(&spawn, &motion, 0, 1, 2, 0, 0, 0, 0, 0, 0, &style, 0, effect->unkC, effect->unk1);
        }

        duration = func_150ADA68() + 2.5f;
        distance = func_150ADA68() * 200.0f + 600.0f;
        emission.unk1A = distance;
        emission.unk04 = distance / duration;
        emission.height = 0;
        emission.width = 0;
        emission.red = 0xFF;
        emission.environment[0] = 0;
        emission.environment[1] = 0;
        emission.environment[2] = 0;
        emission.green = 0xFF;
        emission.blue = 0xFF;
        emission.lifetime = 100;
        emission.alpha = 0xFF;
        emission.fade = 0;
        emission.frame = 0;
        emission.frame_rate = 0;
        emission.fraction_x = 0;
        emission.fraction_z = 0;
        emission.fraction_y = 0;
        emission.velocity_x = 0;
        emission.velocity_z = 0;
        emission.acceleration = 0;
        emission.velocity_y = 0;
        emission.texture = D_8008CB14;
        emission.position[0] = spawn.pos[0];
        emission.position[1] = spawn.pos[1];
        emission.position[2] = spawn.pos[2];
        emission.callback = 0xB;
        emission.flags = 0;
        emission.unk2B = 0;
        emission.unk08 = 0;
        func_15167D84(&emission, 0, 0, -1, effect->unkC, effect->unk1);
    }
}
// some funky xor going on
typedef struct {
    u8 unk0;
    u8 pad1;
    s16 unk2;
} Ent150AEB9C;
typedef struct {
    s32 count;
    Ent150AEB9C *ents;
} EntList150AEB9C;
typedef struct {
    u8 pad0[0x28];
    void *unk28;
    u8 unk2C;
    u8 unk2D;
} Spawn150AEB9C;
extern EntList150AEB9C D_800886E0[];
extern Ent150AEB9C *D_800886E4;
extern u32 D_15001B08[];
extern u8 D_800DCE50[];
void func_15001A08(void);

void func_150AEB9C(u8 *arg0) {
    u32 *p;
    u32 sum;
    s32 i;
    s32 idx;
    u32 end;

    idx = 0;
    if (arg0 != NULL) {
        sum = 0;
        end = (u32)D_15001B08;
        for (p = (u32 *)func_15001A08; (u32)p < end; p++) {
            sum ^= *p;
            sum <<= 1;
        }
        if (sum != 0xB4E42D60) {
            *(s32 *)(D_800DCE50 + 0x8C) = 0;
            *(s32 *)(D_800DCE50 + 0x22C) = 0;
        }
        for (i = 0; i < D_800886E0[idx].count; i++) {
            Spawn150AEB9C *obj;
            obj = (Spawn150AEB9C *)func_151491F4(D_800886E0[idx].ents[i].unk2, 0, -1, 1, 0x27, 8, 0xFF, 0);
            if (obj == NULL) {
                break;
            } else {
                EntList150AEB9C *list;
                list = &D_800886E0[idx];
                obj->unk28 = arg0;
                obj->unk2C = arg0[0x3B];
                obj->unk2D = list->ents[i].unk0;
            }
        }
    }
}

void func_150AECCC(struct42 *arg0) {
    arg0->unk96 = arg0->unk96 + (arg0->unk94 * D_800BE9E4);
    if (arg0->unk96 >= 0x1401) {
        arg0->unk96 = 0x1400;
    }
    arg0->unk9E = arg0->unk9E - ((s32) arg0->unk96 >> 8);
    arg0->unkA4 = arg0->unkA4 + D_800BE9E4;
    if (arg0->unkA4 >= 0x1A) {
        arg0->unkA4 = 0x19;
    }
}

void func_150AED4C(struct114 *arg0) {
    arg0->unk34 += arg0->unk14 * D_800BE9E4;
    if (arg0->unk2A < arg0->unk34) {
        arg0->unk3A = 70;
        arg0->unk34 = arg0->unk2A;
    }
    arg0->unk36 = arg0->unk34;
}

s32 func_150AED9C(void *arg0) {
    void *temp_v0 = *(void **)((s32)arg0 + 0x98);
    s32 temp_v1 = *(s16 *)((s32)arg0 + 0x1C) << 3;
    s32 temp_t7;

    if (temp_v1 >= 0x100) {
        temp_v1 = 0xFF;
    }
    temp_t7 = temp_v1 & 0xFF;
    *(u8 *)((s32)temp_v0 + 0x1B) = temp_v1;
    if (temp_t7 < 0) {
        return 0;
    }
    return 1;
}

s32 func_150AEDD8(struct202 *arg0) {
    if (arg0->unk1C < 0x20) {
        arg0->unk28 = arg0->unk1C * 8;
    }
    return 1;
}

typedef struct {
    s32 unk0;
    u8 unk4;
} SubA_150AEDF8;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8 b4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB_150AEDF8;

void func_150AEDF8(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_150AEDF8 *temp_v0 = (SubA_150AEDF8 *)((u8 *)arg0 + 0x28);
    ArgB_150AEDF8 *b = (ArgB_150AEDF8 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else {
            if (temp_v0->unk0 == b->u4.w4) {
                temp_v0->unk0 = b->unk0;
                ((volatile SubA_150AEDF8 *)temp_v0)->unk4 = b->unk8;
            }
trailing_label_150AEDF8:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}
