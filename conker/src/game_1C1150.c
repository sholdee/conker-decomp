#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ u8 unk7;
} Struct15195738Small;

extern void func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_151C0098(f32 *arg0, struct127 *arg1, s32 arg2, s32 arg3, s32 arg4);
extern void func_1507CD64(struct127 *arg0, s32 arg1);
extern void func_1504715C(f32 *arg0, struct127 *arg1);
extern void func_1514B364(f32 *arg0, f32 *arg1, s32 arg2, s32 arg3);
extern s32 func_1518D1C0(struct127 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 *arg6);
extern u8 D_800A8244[];


#pragma GLOBAL_ASM("asm/nonmatchings/game_1C1150/func_15193CA0.s")

extern void func_15193CA0(s32, s32, s32, s32);

void func_151942B0(s32 arg0, s32 arg1, s32 arg2) {
    func_15193CA0(arg1, 0, 0xFF, 1);
}

void func_151942E8(s32 arg0, s32 arg1, s32 arg2) {
    func_15193CA0(arg1, 1, 0xFF, 1);
}

typedef struct Game1C1150Event {
    u8 pad0[4];
    u8 type;
} Game1C1150Event;

extern u8 D_800A8210;

void func_15194320(void *arg0, Game1C1150Event *arg1, s32 arg2) {
    switch (arg1->type) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            func_1518D1C0((struct127 *)arg1, 0xA, 0, 1, 0xFF, 1, &D_800A8210);
            break;
    }
}

extern u8 D_800A822C;

void func_15194394(void *arg0, Game1C1150Event *arg1, s32 arg2) {
    switch (arg1->type) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            func_1518D1C0((struct127 *)arg1, 0xC, 0, 1, 0xFF, 1, &D_800A822C);
            break;
    }
}

typedef struct Game194408Packet {
    s16 field00;
    s16 field02;
    s16 field04;
    s16 field06;
    f32 position[3];
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
    s16 field34;
    s16 field36;
    s16 field38;
    s16 field3A;
    u8 field3C;
    u8 pad3D[3];
    f32 field40;
    s16 field44;
    s16 field46;
    s32 field48;
} Game194408Packet;

extern void func_15153F18(s16 *, void *, s32, s32, s32);
extern void *func_151AE7B0(void *, f32, s16, u8, s32);

void func_15194408(s32 arg0, Game1C1150Event *arg1) {
    struct {
        u8 scratch[0x24];
        Game194408Packet packet;
    } locals;

    switch (arg1->type) {
    case 0:
        goto spawn;
    case 1:
        goto spawn;
    case 2:
        goto spawn;
    case 3:
        goto spawn;
    case 4:
        goto spawn;
    }
    return;

spawn:
    func_151AE7B0(arg1, 0.6f, 0x5A, 0xFF, 1);
    func_1504715C((f32 *)locals.scratch, (struct127 *)arg1);
    locals.packet.position[0] = *(f32 *)((u8 *)arg1 + 0x14);
    locals.packet.position[1] = *(f32 *)((u8 *)arg1 + 0x18) + 50.0f;
    locals.packet.position[2] = *(f32 *)((u8 *)arg1 + 0x1C);
    locals.packet.field14 = 6.0f;
    locals.packet.field2C = 0xC;
    locals.packet.field2E = 5;
    locals.packet.field02 = 0xFF;
    locals.packet.field04 = -0x3F;
    locals.packet.field00 = 0;
    locals.packet.field06 = 0x23;
    locals.packet.field30 = 3;
    locals.packet.field32 = 2;
    locals.packet.field34 = 0x1E;
    locals.packet.field36 = 0x1E;
    locals.packet.field38 = 0x64;
    locals.packet.field3A = 0x64;
    locals.packet.field44 = 0x10;
    locals.packet.field46 = 0xF;
    locals.packet.field48 = 0;
    locals.packet.field3C = 0;
    locals.packet.field18 = 3.0f;
    locals.packet.field1C = 0.21f;
    locals.packet.field20 = 0.406f;
    locals.packet.field24 = 13.0f;
    locals.packet.field28 = 8.0f;
    locals.packet.field40 = 0.3f;
    func_15153F18(&locals.packet.field00, &locals.packet.position,
                   (s32)locals.scratch, 0xFF, 1);
}

extern void func_150C7020(s32, s32, s32, s32);
extern void func_15194408(s32, Game1C1150Event *);

void func_15194588(s32 arg0, s32 arg1, s32 arg2) {
    func_150C7020(arg0, 0x1F4, 0xFF, 1);
    func_15194408(arg0, (Game1C1150Event *)arg1);
}

typedef struct Game1945CCVec {
    f32 x;
    f32 y;
    f32 z;
} Game1945CCVec;

typedef struct Game1945CCPacket {
    s16 field00;
    s16 field02;
    s16 field04;
    s16 field06;
    Game1945CCVec position;
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
    s16 field34;
    s16 field36;
    s16 field38;
    s16 field3A;
    u8 field3C;
    u8 pad3D[3];
    f32 field40;
    s16 field44;
    s16 field46;
    s32 field48;
} Game1945CCPacket;

extern u8 D_800A3FE6[];
extern s32 func_15134070(struct127 *);
extern void func_151B4CD0(void *, s32, s32);

void func_151945CC(void *arg0, void *arg1, s32 arg2) {
    Game1945CCVec pos;
    s16 shade;
    Game1945CCPacket packet;
    u8 scratch[0x24];

    shade = ((s32) *(u16 *)((u8 *)arg0 + 0x7A) >> 8) - 0x40;
    pos.x = *(f32 *)((u8 *)arg1 + 0x14);
    pos.y = *(f32 *)((u8 *)arg1 + 0x18) + 270.0;
    pos.z = *(f32 *)((u8 *)arg1 + 0x1C);
    {
    s32 kind;
    s32 profile;

    func_1504715C((f32 *)scratch, (struct127 *)arg1);
    kind = func_15134070((struct127 *)arg1);
    if (kind != 0x63) {
        profile = D_800A3FE6[kind << 4];
        if (profile != 2) {
            packet.position = pos;
            packet.field14 = 6.699999809f;
            packet.field18 = 2.329999924f;
            packet.field1C = 0.6800000072f;
            packet.field20 = 0.0f;
            packet.field24 = 4.400000095f;
            packet.field2C = 0xF;
            packet.field2E = 0xC;
            packet.field00 = shade - 0x40;
            packet.field02 = 0x80;
            packet.field04 = -0xA0;
            packet.field06 = 0x20;
            packet.field30 = 3;
            packet.field32 = 1;
            packet.field34 = 0x1F;
            packet.field36 = 0x13;
            packet.field38 = 0x9B;
            packet.field3A = 0x64;
            packet.field44 = 0x10;
            packet.field46 = 0xF;
            packet.field48 = 0;
            packet.field28 = 5.300000191f;
            packet.field40 = 0.5f;
            if (profile == 1) {
                packet.field3C = 1;
            } else {
                packet.field3C = 0;
            }
            func_15153F18(&packet.field00, &packet.position, (s32)scratch, 0xFF, 1);
            func_151B4CD0(arg1, 0xFF, 1);
        }
    }
    }
}

void func_151B01B8(s32, s32);
void func_151B09BC(s32, s32, s32, s32, s32);
void *func_151AF270();

void func_15194794(s32 arg0, Game1C1150Event *arg1, s8 *arg2) {
    func_151B01B8(arg0, (s32)arg1);
    func_151B09BC(arg0, (s32)arg1, 0x3E8, 0xFF, 0);
    switch (arg1->type) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            func_151AF270(arg1, 0xFF, 1);
            break;
    }
}

typedef struct Vec3_15194810 {
    f32 x, y, z;
} Vec3_15194810;

typedef struct Game194810Packet {
    s16 field00;
    s16 field02;
    s16 field04;
    s16 field06;
    Vec3_15194810 position;
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
    s16 field34;
    s16 field36;
    s16 field38;
    s16 field3A;
    u8 field3C;
    u8 pad3D[3];
    f32 field40;
    s16 field44;
    s16 field46;
    s32 field48;
} Game194810Packet;

void func_15194810(s32 arg0, Game1C1150Event *arg1, s32 arg2) {
    Vec3_15194810 pos;
    Game194810Packet packet;
    u8 scratch[0x24];
    Struct15195738Small small;

    switch (arg1->type) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 0x53:
        pos.x = *(f32 *)((u8 *)arg1 + 0x14);
        pos.y = *(f32 *)((u8 *)arg1 + 0x18) + 95.0f;
        pos.z = *(f32 *)((u8 *)arg1 + 0x1C);
    fence:
        func_1504715C((f32 *)scratch, (struct127 *)arg1);
        packet.position = pos;
        packet.field14 = 10.0f;
        packet.field2C = 0xF;
        packet.field2E = 0xA;
        packet.field02 = 0xFF;
        packet.field04 = -0x3F;
        packet.field00 = 0;
        packet.field06 = 0x39;
        packet.field30 = 3;
        packet.field32 = 2;
        packet.field34 = 0x46;
        packet.field36 = 0x16;
        packet.field38 = 0x9B;
        packet.field3A = 0x64;
        packet.field44 = 0x10;
        packet.field46 = 0xF;
        packet.field48 = 0;
        packet.field3C = 0;
        packet.field18 = 5.0f;
        packet.field1C = 0.6750000119f;
        packet.field20 = 0.2310000062f;
        packet.field24 = 5.0f;
        packet.field28 = 25.10000038f;
        packet.field40 = 0.5f;
        func_15153F18(&packet.field00, &packet.position, (s32)scratch, 0xFF, 1);
        small.unk0 = 1;
        small.unk2 = (func_150ADA20() % 0xBU) + 0x14;
        small.unk4 = 8;
        small.unk6 = -1;
        small.unk5 = 1;
        func_151D8868(&small, 0, 0xFF, 0);
        break;
    }
}

void func_151949E0(s32 arg0, s32 arg1, s32 arg2) {
}

extern void func_15104170(s32);

void func_151949F4(arg0, arg1) s32 arg0; struct127 *arg1; {
    if (D_8008FD90 == 1) {
        if (arg1->unk127 == 0) {
            if (D_800E9D00 & 0x8) {
                if (arg1->unk31C->unk1A0 == 0) {
                    func_15104170(arg0);
                    arg1->unk31C->unk1A0 = 0x4B0;
                }
            }
        }
    }
}

extern void func_1507DE4C(s32);
extern void func_15138BC0(s32, s32, s32);

void func_15194A68(s32 arg0, s32 arg1, s32 arg2) {
    func_1507DE4C(arg1);
    func_15138BC0(arg1, 0xFF, 1);
    func_151949F4(0, arg0, arg1);
}

void func_15194AB4(struct127* arg0) {
    s32 sp_a1;

    arg0->unk9C |= 0xFFFE;
    sp_a1 = -1;
    switch (arg0->id) {
    case 0x75:
        sp_a1 = 0x73;
        break;
    case 0x80:
        sp_a1 = 0x72;
        break;
    }
    if (sp_a1 != -1) {
        func_15083568(arg0, sp_a1, 1.0f, 0);
    }
}

extern s32 func_15134070(struct127*);
extern void func_15138120(struct127*, s32, s32);
extern void func_15136C3C(struct127*, s32, s32, s32, s32, s32, s32, s32);

void func_15194B1C(struct127* arg0) {
    s32 temp;

    temp = func_15134070(arg0);
    if (temp != 0x63) {
        func_15138120(arg0, temp, 0);
    }
    func_15136C3C(arg0, 1, 1, 1, 1, 1, 0xFF, 1);
    func_15194AB4(arg0);
}

void func_15194B94(s32 arg0, struct127* arg1, s32 arg2) {
    struct127* temp;

    temp = arg1;
    if (temp->id == 0x53) {
        func_151942B0(arg0, (s32)temp, arg2);
        return;
    }
    func_15194B1C(temp);
    func_151949F4(1, arg0, temp);
}

void func_15194BF0(s32 arg0, struct127 *arg1, u8 *arg2) {
    s32 id;
    struct126 *temp_v0;
    struct127 *temp_t6;
    s32 temp_v1;

    id = arg1->id;
    if (id >= 0x3C) {
        switch (id) {
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
            goto full_body;
        case 0x7C:
            goto cleanup_body;
        }
    } else {
        if (id >= 0x17) {
            if (id != 0x3B) {
                return;
            }
            goto full_body;
        } else {
            switch (id) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 0x11:
            case 0x14:
                goto full_body;
            case 0x16:
                goto stop_then_cleanup;
            }
        }
    }
    return;

stop_then_cleanup:
    func_15136C3C(arg1, 1, 1, 1, 1, 0, 0xFF, 1);
    func_15145A50(arg1);

cleanup_body:
    if (arg1->unk65 != 0) {
        temp_v1 = arg1->unk65 - 1;
        temp_t6 = arg1;
        D_800CC2D0[temp_v1].unk232 = 0xA;
        D_800CC2D0[temp_v1].unk218 = NULL;
        temp_t6->unk65 = 0;
after_store_unk65:
        return;
    }
    return;

full_body:
    func_15136C3C(arg1, 1, 1, 1, 1, 0, 0xFF, 1);
    func_15145A50(arg1);
    func_1507CD64(arg1, 6);
    temp_v0 = arg1->unk31C;
    if (temp_v0 != NULL) {
        if (((u8 *) temp_v0)[0x19B] != 0) {
            func_15063B64(arg1);
        }
    }
}

void func_15194FF4();

s32 func_15194D80(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_15194FF4(arg0, arg1, 0, arg3);
}

s32 func_15194DA4(arg0, arg1, arg2, arg3)
s32 arg0;
s32 arg1;
s32 arg2;
s32 arg3;
{
    func_15194FF4(arg0, arg1, 1, arg3);
}

void func_15194DC8(s32 arg0, struct127 *arg1, s32 arg2) {
    f32 sp2C[9];

    if (D_800BE616 != 0) {
        func_15194DA4(arg0, (s32)arg1, arg2);
    } else {
        func_1518D1C0(arg1, 0xB, 0, 1, 0xFF, 1, D_800A8244);
    }
    func_1504715C(sp2C, arg1);
    func_1514B364(&arg1->x_position, sp2C, 0xFF, 1);
}

extern void func_150B06B0(s32, s32, s32, s32);

void func_15194E54(s32 arg0, s32 arg1, s32 arg2) {
    if ((*(u8 *)(arg1 + 4) == 0x3E) || (*(u8 *)(arg1 + 4) == 0xA6)) {
        func_150B06B0(arg1, arg0, 0xFF, 1);
    }
}

extern void func_15182670(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_15194BF0(s32, struct127*, u8*);

void func_15194EA8(s32 arg0, struct127* arg1, s32 arg2) {
    u8 sp37;
    u32 temp1;
    u32 temp0;

    sp37 = 0;
    if (arg1->camera != 0) {
        temp0 = func_150ADA20();
        temp1 = func_150ADA20();
        func_15182670(0xFF, 0xFF, 0xFF, 0xFF, temp1 % 0xB + 0x23, arg1->camera->unk23D, 0xFF, 1);
        if (temp0) { }
    }
    func_15194BF0(arg0, arg1, &sp37);
}

void func_15194F48(s32 arg0, struct127* arg1, s32 arg2) {
    f32 sp34[3];

    sp34[0] = arg1->x_position;
    sp34[1] = arg1->y_position + 50.0f;
    sp34[2] = arg1->z_position;
    func_151C0098(sp34, arg1, 1, 0xFF, 1);
    func_15136C3C(arg1, 0, 0, 1, 0, 0, 0xFF, 1);
    func_15145A50(arg1);
    func_1507CD64(arg1, 6);
}

typedef struct {
    s32 unk0;
    s32 unk4;
    struct127 *unk8;
} Struct15194FF4;

extern s32 func_1518F51C(void *arg0, u8 arg1, s32 arg2, s8 arg3, s8 arg4, s8 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9);
extern void func_15190518(void *arg0);

void func_15194FF4(arg0, arg1, arg2)
struct127 *arg0;
struct127 *arg1;
u8 arg2;
{
    u8 *ret;
    Struct15194FF4 sp48;

    func_15190518(arg1);
    switch (arg1->id) {
    case 0x0: case 0x1: case 0x2: case 0x3: case 0x4: case 0x11: case 0x14: case 0x16:
    case 0x3b: case 0x50: case 0x53: case 0x58: case 0x5a: case 0x5f: case 0x70: case 0x74:
    case 0x75: case 0x7a: case 0x80: case 0x87: case 0x88: case 0x90: case 0x96: case 0x98:
    case 0x99: case 0x9c: case 0x9d: case 0x9f: case 0xa0: case 0xb0: case 0xb1: case 0xb2:
    case 0xb4:
        if (D_800BE616 != 0) {
            sp48.unk0 = 0x7D;
            sp48.unk4 = 5;
            if ((arg0->id == 0xFF) && (arg0->unk124 != 0)) {
                sp48.unk8 = &D_800CC2D0[arg0->unk124 - 1];
            } else {
                sp48.unk8 = arg0;
            }
            ret = (u8 *)func_1518F51C(arg1, 0, 250, arg2 ? 0 : -1, arg2 ? 0 : -1, 8, arg2 ? 1 : 0, arg2 ? 0xC : 0, 0xFF, 1);
            if ((ret != 0) && (arg2 != 0)) {
                memcpy(ret + 0x90, &sp48, 0xC);
                {
                    u8 *temp = (u8 *)arg1->unk31C;
                    if (temp != NULL) {
                        temp[0x1AC] = 1;
                    }
                }
            }
        } else {
            func_1518F51C(arg1, 0, 300, -1, -1, 8, 0, 0, 0xFF, 1);
        }
        break;
    }
}

extern struct127 *func_1505ED34(void);
extern s32 func_150838EC(struct127 *arg0, s32 arg1, s32 arg2, f32 arg3);
extern void func_150D10E4(void *arg0, u8 arg1);

#define U8AT(p, off) (*(u8 *)((u8 *)(p) + (off)))
#define F32AT(p, off) (*(f32 *)((u8 *)(p) + (off)))

void func_151951E0(struct127 *arg0) {
    struct127 *obj;

    arg0->health = 0;
    func_1507CD64(arg0, 5);
    func_15194AB4(arg0);
    arg0->interaction_state = 0x28;
    arg0->xz_velocity = -3.0f;
    F32AT(arg0, 0xBC) = -10.0f - (func_150ADA20() % 5U);
    arg0->unk148 = (func_150ADA20() % 10U) - 5.0f;
    arg0->y_velocity = 0.0f;
    arg0->gravity = 0.0f;
    obj = func_1505ED34();
    obj->interaction_state = 0x28;
    obj->id = arg0->id;
    obj->immune = 0xFF;
    obj->stunned = 0xFF;
    obj->x_position = arg0->x_position;
    obj->y_position = arg0->y_position;
    obj->z_position = arg0->z_position;
    obj->unk40 = arg0->unk40;
    obj->unk76 = arg0->unk76;
    obj->unk7A = arg0->unk7A;
    obj->unkB8 = arg0->unkB8;
    obj->xz_scale = arg0->xz_scale;
    obj->y_scale = arg0->y_scale;
    obj->xz_velocity = 10.0f;
    F32AT(obj, 0xBC) = (func_150ADA20() % 5U) + 10.0f;
    obj->unk148 = (func_150ADA20() % 10U) - 5.0f;
    U8AT(obj, 0x68) = U8AT(arg0, 0x68);
    U8AT(obj, 0x69) = U8AT(arg0, 0x69);
    U8AT(obj, 0x6A) = U8AT(arg0, 0x6A);
    U8AT(obj, 0x6B) = U8AT(arg0, 0x6B);
    U8AT(obj, 0x6C) = U8AT(arg0, 0x6C);
    U8AT(obj, 0x6D) = U8AT(arg0, 0x6D);
    obj->unk76 += (func_150ADA20() % 10922U) - 5461.33349609375f;
    obj->unk40 = (arg0->unk76 + 0x4000) * 0.0054931640625f;
    obj->unkF8 = arg0->unkF8;
    obj->unk9C |= 0x0FFF0000;
    func_150838EC(obj, arg0->unk84.uh, 0, arg0->animation_speed);
    func_150D10E4(arg0, 0);
    func_150D10E4(obj, 1);
}

extern f32 D_800A8708;
extern void func_1514373C(f32, f32, f32 *, f32 *);
extern void func_15137610(struct127 *, f32 *, f32 *, f32 *, u8, s32);

void func_15195528(struct127 *arg0, struct127 *arg1, s32 arg2) {
    f32 angle;
    f32 from[3];
    f32 to[3];
    f32 dir[3];
    f32 dx;
    f32 dz;

    if (arg0 != NULL) {
        angle = arg0->unk40 - 90.0f;
    } else if (arg1 != NULL) {
        angle = arg1->unk40 - 90.0f;
    } else {
        angle = func_150ADA68() * 360.0f;
    }
    func_1514373C(angle * D_800A8708, 1.0f, &dir[0], &dir[2]);
    dir[1] = 0.0f;
    dx = dir[0] * 20.0f;
    from[0] = arg1->x_position + dx;
    from[1] = arg1->y_position + 50.0f;
    dz = dir[2] * 20.0f;
    from[2] = arg1->z_position + dz;
    to[0] = arg1->x_position - dx;
    to[1] = arg1->y_position + 50.0f;
    to[2] = arg1->z_position - dz;
    func_15137610(arg1, from, to, dir, 0xFF, 1);
}

void func_151B01B8(s32, s32);
void func_151B09BC(s32, s32, s32, s32, s32);

void func_15195650(s32 arg0, s32 arg1, s32 arg2) {
    func_151B01B8(arg0, arg1);
    func_151B09BC(arg0, arg1, 0x3E8, 0xFF, 0);
}

extern void func_15145A50(struct127 *arg0);
extern void func_1507CD64(struct127 *arg0, s32 arg1);

void func_15195698(struct127 *arg0, struct127 *arg1, s32 arg2) {
    if (arg1->health == 0) {
        if (arg1->unk127 != 0xFF) {
            func_15136C3C(arg1, 1, 1, 1, 1, 0, 0xFF, 1);
            func_15145A50(arg1);
            func_1507CD64(arg1, 6);
        }
    }
    if (arg0->id == 0x47) {
        arg0->health = 0;
    }
}

void func_15195738(s32 arg0, s32 arg1, s32 arg2) {
    Struct15195738Small sp18;

    sp18.unk0 = 1;
    sp18.unk2 = (func_150ADA20() % 0xBU) + 0x14;
    sp18.unk4 = 8;
    sp18.unk6 = -1;
    sp18.unk5 = 1;
    func_151D8868(&sp18, 0, 0xFF, 0);
}
