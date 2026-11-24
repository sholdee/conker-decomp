#include <ultra64.h>
#define func_15083E90 func_15083E90_s32_decl_in_functions_h
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_15083E90
#undef func_150ADA20
struct127 *func_15083E90(u8);
#include "variables.h"


struct C4DC0_elem30 {
    u8 pad0[2];
    u8 unk2;
    u8 pad3[0x2D];
};
extern s32 func_15082A44(void *, s32, s32, s32, s32);

s32 func_15097910(s32 arg0, s32 arg1) {
    s32 ret;
    struct127 *obj;

    ret = func_15083E0C((u8)arg0);
    if (ret != -1) {
        obj = func_15083E90((u8)arg0);
        if (obj == NULL) {
            ((struct C4DC0_elem30 *)D_800D20FC)[ret].unk2 = 0;
            if (func_15082A44(ret + (struct C4DC0_elem30 *)D_800D20FC, ret, 0, 0, 0) == 0) {
                return -1;
            }
            ret = func_15083E0C((u8)arg0);
        } else {
            ret = ((u8 *)obj)[0x13F];
        }
    }
    if (ret != 0) {
        ret |= 0x2000;
    }
    return ret;
}


struct C4DC0_arg1 {
    u8 pad0[4];
    s32 unk4;
};
struct C4DC0_elem {
    u8 pad0[2];
    u8 unk2;
    u8 pad3[0x2D];
};
extern struct127 *func_1505EEF4(s32);
extern s32 func_15053430(struct127 *);
extern u8 D_800D2100;
extern s32 D_800D3840;

s32 func_150979CC(s32 arg0, struct C4DC0_arg1 *arg1) {
    struct127 *temp;

    temp = func_1505EEF4(arg0);
    if (temp == 0) {
        return 0;
    }
    if (arg0 < D_800D2100) {
        ((struct C4DC0_elem *)D_800D20FC)[arg0].unk2 = 1;
    }
    if (D_800D3840 == 2) {
        if (arg1->unk4 == 0) {
            func_15060F28(temp, 0);
        } else {
            func_15053430(temp);
        }
    } else {
        func_15060F28(temp, 0);
    }
    return 999999;
}
u32 func_150ADA20(void);

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 a[6];
} ScriptCmd;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} PathPoint;

typedef struct {
    u8 pad0[8];
    PathPoint pts[1];
} PathData;

#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern s32 D_800BE3E4;
extern s16 D_800C3FF4[3];
extern void func_1503DE70(struct127 *, s32, s32);
extern void func_1504BAF0(struct127 *);
extern void func_15052F9C(struct127 *, f32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_1505D024(struct127 *, s32, u16, s32);
extern void func_1505D1C4(f32, f32, f32, s32, s32, s32, s32, s32);
extern void func_15063254(struct127 *, s32, s32, f32);
extern void func_15063404(struct127 *);
extern void func_150634E4(struct127 *);
extern void func_15063570(struct127 *);
extern struct216 *func_1507BB28(s32, s32);
extern void func_1507CD64(struct127 *, s32);
extern void func_1507D1D8(struct127 *);
extern void func_1507D4F8(s32);
extern void func_1507EB2C(struct127 *);
extern void func_1507EB4C(struct127 *, s32);
extern void func_15080620(s32, s32, s32, s32);
extern void func_150836CC(struct127 *, s32);
extern s32 func_15084000(s32);
extern void func_15085410(struct127 *, s32);
extern s32 func_15086CBC(s32, f32 *, f32 *, f32 *);
extern s32 func_15086D48(s32);
extern void func_1510E7A4(s32, s32, f32 *, s32, s32, s32, f32, f32, f32, f32, s32, s32, f32, f32);
extern void func_1514DE94(struct127 *);
extern void func_15172D28(struct127 *, s32);
extern void func_1517EE40(s32, s32, s32, s32, s32, s32);
extern void func_1518BA90(struct127 *, s32, s32);
extern void func_1518BBF4(struct127 *);
extern void func_15193660(struct127 *, s32, s32);
extern void func_151937F4(struct127 *, s32, s32);
extern void func_151D6BFC(struct127 *, s32, s32);
extern f32 sinf(f32);
extern f32 cosf(f32);

s32 func_15097A8C(s32 arg0, s32 arg1, ScriptCmd *cmd) {
    struct127 *obj;
    s32 i;
    s32 node;
    s32 pad3;

    obj = func_1505EEF4(arg0);
    if (obj == NULL) {
        return 0;
    }
    switch (arg1) {
    case 1:
        if (cmd->a[0] != 0) {
            obj->unk25C |= cmd->a[1];
        } else {
            obj->unk25C &= ~cmd->a[1];
        }
        return 1;
    case 0x52:
    case 0x5E:
        break;
    case 7:
        D_800D154C = obj;
        func_1504BAF0(D_800D154C);
        break;
    case 2:
        obj->unk25C |= 8;
        return 1;
    case 3:
        obj->unk25C &= ~8;
        return 1;
    case 4:
        obj->unk25C |= 0x10;
        return 1;
    case 0xA:
        obj->unk25C &= ~4;
        break;
    case 6:
        obj->unk25C |= 4;
        obj->unk25C &= ~1;
        return 1;
    case 5:
        obj->unk25C |= 1;
        return 1;
    case 8:
        if (cmd->a[0] != 0x7FFFFFFF) {
            obj->x_position = cmd->a[0];
        }
        if (cmd->a[1] != 0x7FFFFFFF) {
            obj->y_position = cmd->a[1];
        }
        if (cmd->a[2] != 0x7FFFFFFF) {
            obj->z_position = cmd->a[2];
        }
        break;
    case 9: {
        s32 node;

        node = func_15086D48(cmd->a[0]);
        if (node != -1) {
            func_15086CBC(node, &obj->x_position, &obj->y_position, &obj->z_position);
        }
        return 1;
    }
    case 0xC:
    case 0xD:
        obj->xz_velocity = cmd->a[0] * 1.52587890625e-05f;
        break;
    case 0xE:
        obj->y_velocity = cmd->a[0] * 1.52587890625e-05f;
        break;
    case 0xF:
        obj->gravity = cmd->a[0] * 1.52587890625e-05f;
        break;
    case 0x10:
        obj->unk76 = obj->unk7A = (u16)(cmd->a[0] * 1.52587890625e-05f * 182.04444885f) - 0x4000;
        break;
    case 0x11:
        obj->unk76 = obj->unk7A = cmd->a[0] * 0.00390625f;
        break;
    case 0x12:
        obj->unk78 = cmd->a[0] * 0.00390625f;
        break;
    case 0x13:
        obj->immune = cmd->a[0];
        break;
    case 0x14:
        obj->unk232 = cmd->a[0];
        D_800D154C = obj;
        D_800C3E78 = D_800D154C - D_800CC2D0;
        obj->unk218 = func_1507BB28(0, obj->unk232);
        obj->unk21C = 0;
        break;
    case 0x61: {
        s32 v;

        v = cmd->a[0];
        if (obj->unk31C != NULL) {
            if (v == -1) {
                v = 0x8000;
            }
            obj->unk31C->unk8C = v;
            obj->unk31C->unk8F = 0;
            switch (cmd->a[0]) {
            case -1:
            case 4:
            case 0x2000:
                obj->unk31C->unk8A = v;
                obj->unk31C->unk8E = 2;
                break;
            }
        }
    }
        break;
    case 0x66:
        func_150432CC((s32)obj, cmd->a[0]);
        break;
    case 0x67: {
        s32 v;

        v = cmd->a[0];
        func_15042D78(0);
        switch (D_80082FA0) {
        case 2:
        case 3:
            func_150432FC((v & 1) * 150 + 10, (v >> 1) * 115);
            break;
        }
        break;
    }
    case 0x62: {
        s32 pad;

        if ((obj - D_800CC2D0) >= 0 && (obj - D_800CC2D0) < D_8008FD8C) {
            func_15085710(obj - D_800CC2D0, 10, cmd->a[0]);
        }
        D_800BE3E4 = func_150859AC(obj - D_800CC2D0, 6);
    }
        break;
    case 0x63: {
        s32 pad;

        if ((obj - D_800CC2D0) >= 0 && (obj - D_800CC2D0) < D_8008FD8C) {
            func_15085710(obj - D_800CC2D0, 9, cmd->a[0]);
        }
        D_800BE3E4 = func_150859AC(obj - D_800CC2D0, 6);
    }
        break;
    case 0x64: {
        s32 v;

        v = cmd->a[0];
        if (D_800BE616 == 0 && (D_800E9D00 & 0x200)) {
            v = 0x33;
        }
        if ((obj - D_800CC2D0) >= 0 && (obj - D_800CC2D0) < D_8008FD8C) {
            func_15085710(obj - D_800CC2D0, 2, v);
        }
        break;
    }
    case 0x65:
        if ((obj - D_800CC2D0) >= 0 && (obj - D_800CC2D0) < D_8008FD8C) {
            func_15085710(obj - D_800CC2D0, 3, cmd->a[0]);
        }
        break;
    case 0x5F:
        if (cmd->a[0] < obj->unk44) {
            obj->unk44 = cmd->a[0];
        }
        break;
    case 0x60:
        if (cmd->a[0] < 0) {
            obj->unk211 = 0xFF;
        } else {
            obj->unk211 = func_15086D48(cmd->a[0]);
        }
        obj->unk20F = 0xFF;
        obj->unk25C &= ~0x800;
        obj->unk232 = cmd->a[1];
        D_800D154C = obj;
        D_800C3E78 = D_800D154C - D_800CC2D0;
        obj->unk218 = func_1507BB28(0, obj->unk232);
        obj->unk21C = 0;
        break;
    case 0x5B:
        if (cmd->a[0] < 0) {
            obj->unk211 = 0xFF;
        } else {
            obj->unk211 = func_15086D48(cmd->a[0]);
        }
        break;
    case 0x5C:
        if (cmd->a[0] < 0) {
            obj->unk20F = 0xFF;
        } else {
            obj->unk20F = func_15086D48(cmd->a[0]);
        }
        break;
    case 0x5D:
        if (cmd->a[0] < 0) {
            obj->unk212[0] = 0xFF;
        } else {
            obj->unk212[0] = func_15086D48(cmd->a[0]);
        }
        break;
    case 0xB:
        obj->unk13D = cmd->a[0];
        break;
    case 0x15:
        obj->unkB8 = cmd->a[0] * 1.52587890625e-05f;
        break;
    case 0x16:
        obj->unk48 = cmd->a[0] * 1.52587890625e-05f;
        break;
    case 0x17:
        cmd->a[0] ? func_1518BA90(obj, 0xFF, 0) : func_1518BBF4(obj);
        break;
    case 0x72:
        if (obj->unk31C != NULL) {
            FLD(obj->unk31C, 0x129, u8) = cmd->a[0];
        } else {
            obj->unk222 = cmd->a[0];
        }
        break;
    case 0x18:
        obj->unk247 = cmd->a[0];
        break;
    case 0x19:
        if (cmd->a[0] < 0) {
            obj->unk222 = cmd->a[0];
        } else if (func_15084000(cmd->a[0] & 0xFFFF0FFF) != -1) {
            obj->unk222 = func_15084000(cmd->a[0] & 0xFFFF0FFF);
        }
        break;
    case 0x1A:
        FLD(obj, 0x20A, u8) = cmd->a[0];
        break;
    case 0x1B:
        if (1 == cmd->a[0]) {
            obj->unk9C |= cmd->a[1];
        } else {
            obj->unk9C &= ~cmd->a[1];
        }
        break;
    case 0x1C:
        obj->unk154 = cmd->a[0] * 1.52587890625e-05f;
        obj->unk158 = cmd->a[1] * 1.52587890625e-05f;
        obj->unk15C = cmd->a[2] * 1.52587890625e-05f;
        break;
    case 0x1E:
        if (cmd->a[0] != 0) {
            obj->unk2FA |= 0x80;
        } else {
            obj->unk2FA &= ~0x80;
        }
        break;
    case 0x1F:
        func_15085410(obj, cmd->a[0] & 0xFFF);
        break;
    case 0x5A:
        func_15172D28(obj, -1);
        break;
    case 0x20:
        obj->health = cmd->a[0];
        if (D_800D3840 == 4) {
            D_800D2456 = obj->health;
            D_800D2457 = D_800D2456;
        }
        break;
    case 0x1D:
        if (cmd->a[0] != 0) {
            obj->disable_run = 0xFF;
            obj->disable_jump = 0xFF;
            obj->unk83 = 0xFF;
        } else {
            obj->disable_run = 0;
            obj->disable_jump = 0;
            obj->unk83 = 0;
        }
        break;
    case 0x53:
        obj->disable_run = cmd->a[0];
        break;
    case 0x21:
        func_15085710(arg0, cmd->a[0], cmd->a[1]);
        break;
    case 0x22: {
        s32 j;

        for (j = 0; j < D_800D3840 - 2; j++) {
            if (cmd->a[j] != 0x78000000) {
                FLD(obj, 0x2D8 + j * 4, f32) = cmd->a[j] * 1.52587890625e-05f;
            }
        }
        break;
    }
    case 0x23: {
        s32 j;

        for (j = 0; j < D_800D3840 - 2; j++) {
            if (cmd->a[j] != 0x7FFFFFFF) {
                FLD(obj, 0x2E4 + j * 4, s32) = cmd->a[j];
            }
        }
        break;
    }
    case 0x25:
        obj->unk31C->unk8 = cmd->a[0];
        obj->unk31C->unkE = 0xA6;
        break;
    case 0x24:
        if (cmd->a[0] == 0) {
            func_1507EB2C(obj);
            obj->unk31C->unk8 = 0;
            obj->unk31C->unk16 = 0;
        } else {
            FLD(obj->unk31C, 0x8, s16) = 1;
            if (cmd->a[0] == 1) {
                func_1507EB4C(obj, 0x15);
            } else if (cmd->a[0] == 2) {
                func_1507EB4C(obj, 0x2A);
                obj->unk31C->unk16 = 0xFF;
            }
        }
        break;
    case 0x26:
        func_15080620(arg0, 0, cmd->a[0], 0);
        break;
    case 0x27:
        func_15080620(arg0, 1, cmd->a[0], 0);
        if (D_800D3840 == 3 && cmd->a[0] == 0) {
            obj->unk31C->unk78 = 0;
        }
        break;
    case 0x70:
        if (cmd->a[1] == 0) {
            obj->unk31C->unk128 &= ~cmd->a[0];
        } else {
            obj->unk31C->unk128 |= cmd->a[0];
        }
        break;
    case 0x28:
        FLD(obj->unk31C, 0x76, u8) = cmd->a[0];
        break;
    case 0x29:
        obj->unk251 = cmd->a[0];
        break;
    case 0x2A:
        func_1505D024(obj, cmd->a[0], cmd->a[1], -1);
        break;
    case 0x2B:
        func_1505D024(obj, cmd->a[0], cmd->a[1], cmd->a[2]);
        break;
    case 0x2C:
        func_1505D1C4(cmd->a[1], cmd->a[2], cmd->a[3], cmd->a[0], cmd->a[4], cmd->a[5], 0, 0);
        break;
    case 0x2D:
        obj->unk21E = cmd->a[0];
        break;
    case 0x2E:
        D_800BE710[cmd->a[0]] = cmd->a[1];
        *((u16 **)D_800BE728[0])[cmd->a[0]] = cmd->a[1];
        break;
    case 0x56:
        obj->unk65 = cmd->a[0];
        break;
    case 0x2F: {
        struct127 *other;

        other = func_15072208(obj, cmd->a[0]);
        if (other != NULL) {
            switch (cmd->a[1]) {
            case 0:
                func_15062BDC(obj, cmd->a[2] * 1.52587890625e-05f, cmd->a[3] * 1.52587890625e-05f);
                break;
            case 1:
                func_1505E650(other, cmd->a[2], cmd->a[3] * 1.52587890625e-05f, cmd->a[4] * 1.52587890625e-05f, 0.0f, 0.0f, 0);
                break;
            case 2:
                other->unk218 = NULL;
                other->unk232 = cmd->a[2];
                break;
            }
        }
    }
        break;
    case 0x30:
        obj->stunned = cmd->a[0];
        break;
    case 0x31:
        obj->unk239 = cmd->a[0];
        break;
    case 0x32:
        obj->pad10A = cmd->a[0];
        break;
    case 0x33:
        D_800D1898[cmd->a[0]] = cmd->a[1] * 1.52587890625e-05f;
        break;
    case 0x34: {
        s32 x;
        s32 z;
        f32 y;

        x = FLD(D_800DBFF0, 0x2F8, f32);
        z = FLD(D_800DBFF0, 0x300, f32);
        x = x - sinf(FLD(D_800DBFF0, 0x3A0, f32)) * 1800.0f;
        z = z - cosf(FLD(D_800DBFF0, 0x3A0, f32)) * 1800.0f;
        x += func_150ADA20() % 800U - 400;
        z += func_150ADA20() % 800U - 400;
        if (x < cmd->a[0]) {
            x = cmd->a[0] + (cmd->a[0] - x);
        } else if (x > cmd->a[1]) {
            x = cmd->a[1] + (cmd->a[1] - x);
        }
        if (z < cmd->a[2]) {
            z = cmd->a[2] + (cmd->a[2] - z);
        } else if (z > cmd->a[3]) {
            z = cmd->a[3] + (cmd->a[3] - z);
        }
        y = FLD(D_800DBFF0, 0x2FC, f32);
        func_1510E7A4(0, 0, &y, 0, 0, 0, x, y, z, y, 0, 0, -10000.0f, 20000.0f);
        obj->x_position = x;
        obj->y_position = cmd->a[4] + y;
        obj->z_position = z;
    }
        break;
    case 0x35:
        D_800D154C = obj;
        D_800C3E78 = D_800D154C - D_800CC2D0;
        obj->unk218 = func_1507BB28(0, cmd->a[0]);
        obj->unk21C = 0;
        break;
    case 0x36:
        if (cmd->a[0] != 0) {
            FLD(obj->unk31C, 0x20, u8) |= cmd->a[1];
        } else {
            FLD(obj->unk31C, 0x20, u8) &= ~cmd->a[1];
        }
        break;
    case 0x37:
        obj->unk1E4 = cmd->a[0];
        break;
    case 0x38:
        obj->unk232 = cmd->a[0];
        break;
    case 0x39:
        obj->unk221 = cmd->a[0];
        break;
    case 0x3A: {
        f32 scale;

        scale = cmd->a[0] * 1.52587890625e-05f;
        func_15062BDC(obj, scale, scale);
    }
        break;
    case 0x3B:
        FLD(obj->unk31C, 0x32, u16) |= cmd->a[0];
        if (D_800D3840 == 4) {
            FLD(obj->unk31C, 0x34, u16) = cmd->a[1];
        }
        break;
    case 0x3C:
        if (cmd->a[0] != 0) {
            FLD(obj->unk31C, 0x1A, u8) = 1;
        } else {
            FLD(obj->unk31C, 0x1A, u8) = 0;
        }
        break;
    case 0x3D:
        if (cmd->a[0] == 0) {
            obj->unk94 = ~(cmd->a[1] | 1);
        } else if (1 == cmd->a[0]) {
            obj->unk94 &= ~cmd->a[1];
        } else {
            obj->unk94 |= cmd->a[1];
        }
        break;
    case 0x3E:
        func_1503DE70(obj, cmd->a[0], cmd->a[1]);
        break;
    case 0x3F:
        obj->disable_jump = cmd->a[0];
        break;
    case 0x40:
        if (cmd->a[0] != 0) {
            obj->unkF8 |= cmd->a[1];
        } else {
            obj->unkF8 &= ~cmd->a[1];
        }
        break;
    case 0x41:
        obj->unk1CC = cmd->a[0];
        break;
    case 0x42:
        obj->unk246 = cmd->a[0] >> 8;
        obj->unk249 = cmd->a[0] & 0xFF;
        break;
    case 0x43:
        ((PathData *)D_800D2104[obj->unk13F])->pts[cmd->a[0]].x = cmd->a[1];
        ((PathData *)D_800D2104[obj->unk13F])->pts[cmd->a[0]].y = cmd->a[2];
        ((PathData *)D_800D2104[obj->unk13F])->pts[cmd->a[0]].z = cmd->a[3];
        break;
    case 0x44: {
        f32 px;
        f32 py;
        f32 pz;

        node = func_15086D48(cmd->a[1]);
        if (node != -1) {
            func_15086CBC(node, &px, &py, &pz);
            ((PathData *)D_800D2104[obj->unk13F])->pts[cmd->a[0]].x = px;
            ((PathData *)D_800D2104[obj->unk13F])->pts[cmd->a[0]].y = py;
            ((PathData *)D_800D2104[obj->unk13F])->pts[cmd->a[0]].z = pz;
        }
        break;
    }
    case 0x45:
        FLD(obj->unk31C, 0x7B, u8) = cmd->a[0];
        break;
    case 0x46:
        obj->unk234 = cmd->a[0];
        obj->unk236 = cmd->a[1];
        obj->unk237 = cmd->a[2];
        break;
    case 0x47:
        FLD(obj->unk31C, 0x1B, u8) = cmd->a[0];
        break;
    case 0x48:
        obj->unkD0 = cmd->a[0];
        obj->unk114 = cmd->a[1];
        break;
    case 0x49:
        obj->unk1C9 = cmd->a[0];
        break;
    case 0x4A:
        func_1507EB4C(obj, cmd->a[0]);
        break;
    case 0x4B: {
        s32 v;

        v = cmd->a[0];
        func_15083568(obj, v, 0.0f, 0);
    }
        break;
    case 0x4C:
    {
        s32 v;

        v = cmd->a[0];
        func_150836CC(obj, v);
    }
        break;
    case 0x4D: {
        struct126 *p;
        s32 v1;
        s32 v2;

        v1 = cmd->a[0];
        v2 = cmd->a[1];
        p = func_1503195C(obj, v1, v2);
        if (p != NULL) {
            p->unk38 = cmd->a[2];
        }
    }
        break;
    case 0x4E: {
        struct127 *other;
        s32 id;

        other = func_15083E90(cmd->a[0] & 0xFF);
        if (other != NULL) {
            if (cmd->a[2] == 0) {
                obj->unk101 |= 4;
            }
            obj->unk65 = (other - D_800CC2D0) + 1;
            obj->unk5C = cmd->a[1];
        }
    }
        break;
    case 0x4F:
        obj->unique_id = cmd->a[0];
        break;
    case 0x50:
        obj->unk74 = (obj->unk74 & ~cmd->a[1]) | cmd->a[0];
        break;
    case 0x51:
        obj->unk31C->chased = cmd->a[0];
        break;
    case 0x54: {
        s32 a;
        s32 b;
        s32 c;

        c = cmd->a[0];
        if (D_800D3840 == 3) {
            a = 1;
            b = 0;
        } else {
            a = 2;
            b = cmd->a[1];
        }
        func_1506160C(obj, (u8)a, c, b, 0);
    }
        break;
    case 0x57:
        obj->unk21C = cmd->a[0];
        break;
    case 0x58:
        FLD(obj, 0x140, u8) = cmd->a[0];
        break;
    case 0x55: {
        s32 pad;
        s32 idx;

        idx = obj - D_800CC2D0;
        if (idx < 4) {
            *((u16 **)D_800BE728[0])[idx] |= 0x4000;
            D_800BE710[idx] |= 0x4000;
        }
    }
        break;
    case 0x59:
        ((u8 *)obj + 0x20B)[cmd->a[1]] = cmd->a[0];
        break;
    case 0x68:
        FLD(obj, 0x10, s16) = cmd->a[0];
        break;
    case 0x69:
        D_800D18C0 |= cmd->a[0];
        break;
    case 0x6C:
        if (cmd->a[0] == 9) {
            func_15063404(obj);
        } else if (cmd->a[0] == 0x3B) {
            func_15063570(obj);
        }
        break;
    case 0x6D:
        if (cmd->a[0] == 9) {
            func_150634E4(obj);
        }
        break;
    case 0x6A:
        if (cmd->a[0] != 0) {
            obj->unk101 |= 1;
        } else {
            obj->unk101 &= ~1;
        }
        break;
    case 0x6B:
        if (1 == cmd->a[0]) {
            obj->unk101 |= cmd->a[1];
        } else {
            obj->unk101 &= ~cmd->a[1];
        }
        break;
    case 0x6E: {
        s32 idx;
        s32 val;

        idx = cmd->a[0];
        val = cmd->a[1];
        ((u8 *)obj + 0x6C)[idx] = val + 10;
    }
        break;
    case 0x6F: {
        s32 idx;
        s32 val;

        idx = cmd->a[0];
        val = cmd->a[1];
        ((u8 *)obj + 0x68)[idx] = val;
    }
        break;
    case 0x71: {
        s32 angle;
        s32 dist;
        s32 lo;
        s32 hi;
        s32 pad1;
        s32 pad2;
        s32 pad3;
        s32 pad4;
        s32 pad5;

        if (cmd->a[4] >= cmd->a[5]) {
            lo = cmd->a[5] ^ cmd->a[4];
            hi = cmd->a[5] ^ lo;
            cmd->a[4] = lo ^ hi;
            cmd->a[5] = hi;
        }
        angle = func_15048AD0((f32)cmd->a[4], (f32)cmd->a[5]);
        angle = cmd->a[4] + func_150ADA20() % angle;
        dist = cmd->a[2] + func_150ADA20() % (cmd->a[3] - cmd->a[2]);
        obj->x_position = cmd->a[0] + sinf(angle * 0.01745329238f) * -(f32)dist;
        obj->z_position = cmd->a[1] + cosf(angle * 0.01745329238f) * -(f32)dist;
    }
        break;
    case 0x73:
        obj->interaction_state = cmd->a[0];
        break;
    case 0x74:
        obj->unk31C->unk31 = cmd->a[0];
        break;
    case 0x75:
        if (cmd->a[0] != 0) {
            obj->unk2F8 |= cmd->a[1];
        } else {
            obj->unk2F8 &= ~cmd->a[1];
        }
        break;
    case 0x76:
        if (obj->unk31C != NULL) {
            FLD(obj->unk31C, 0x1AE, u8) = cmd->a[0];
        }
        break;
    case 0x77:
        D_800C3FF4[0] = cmd->a[0];
        D_800C3FF4[1] = cmd->a[1];
        D_800C3FF4[2] = cmd->a[2];
        break;
    case 0x78:
        switch (cmd->a[0]) {
        case 0: {
            s32 v;

            v = 0x79;
            func_15083568(obj, v, 0.0f, 0);
            if ((obj - D_800CC2D0) < D_8008FD90) {
                obj->unk31C->unk4E = 2;
                func_1505E650(obj, 5, 1.0f, 0.0f, 0.0f, 0.0f, 0);
            }
        }
            break;
        case 1:
            func_1514DE94(obj);
            break;
        case 2:
            FLD(obj->unk31C, 0x17, u8) = 1;
            obj->unk1C9 = 6;
            break;
        case 3: {
            struct127 *other;

            other = func_1505EEF4(cmd->a[1] & 0xFFFF0FFF);
            if (other != NULL) {
                obj->unk31C->unk4E = 1;
                func_15052F9C(other, 50.0f, 0x17, 4, 0, 1, 0, 0, 1, 0);
            }
            break;
        }
        case 4:
            func_15193660(obj, 0xFF, 1);
            func_151937F4(obj, 0xFF, 1);
            func_151D6BFC(obj, 0xFF, 1);
            break;
        case 5:
            func_1505E650(obj, 0x16, 1.0f, 0.0f, 0.0f, 0.0f, 0);
            break;
        }
        break;
    case 0x79:
        if (cmd->a[0] != 0) {
            obj->unk31C->matrix_physics = 1;
        } else {
            obj->unk31C->matrix_physics = 0;
        }
        break;
    case 0x7A: {
        struct127 *other;
        s32 id;

        other = func_15083E90(cmd->a[0] & 0xFF);
        if (other != NULL) {
            obj->unk13C = (other - D_800CC2D0) + 100;
            obj->xz_velocity = 0.0f;
            func_1505959C(other, obj - D_800CC2D0);
        }
        break;
    }
    case 0x7B:
        FLD(obj->unk31C, 0x1AF, u8) = cmd->a[0];
        break;
    case 0x7C:
        obj->unk23E = cmd->a[0];
        break;
    case 0x7D:
        FLD(obj, 0x328, u8) = cmd->a[0] + 1;
        break;
    case 0x7E:
        FLD(obj->unk31C, 0x7C, u8) = cmd->a[0];
        break;
    case 0x7F:
        obj->unk144->unk0 = cmd->a[0];
        break;
    case 0x80:
        {
            f32 f;

            f = cmd->a[2] * 0.01f;
            func_15063254(obj, cmd->a[0], cmd->a[1], f);
        }
        break;
    case 0x81:
        obj->pad275 = cmd->a[0];
        break;
    case 0x82: {
        u16 saved;

        saved = D_8008FDBC;
        D_8008FDBC = 0;
        func_1507D1D8(obj);
        D_8008FDBC = saved;
    }
        break;
    case 0x83:
        obj->unk238 = cmd->a[0];
        break;
    case 0x84:
        obj->unk31C->unk4E = cmd->a[0];
        break;
    case 0x85:
        func_1507D4F8(obj - D_800CC2D0);
        break;
    case 0x86:
        obj->unk23D = cmd->a[0];
        break;
    case 0x87:
        func_1507CD64(obj, 0xF);
        break;
    case 0x88:
        func_1507CD64(obj, 9);
        break;
    case 0x89:
        obj->unk22E = cmd->a[0];
        break;
    case 0x8A:
        FLD(obj, 0xA4, u8) = cmd->a[0];
        FLD(obj, 0xA5, u8) = cmd->a[1];
        FLD(obj, 0xA6, u8) = cmd->a[2];
        obj->padA0 = cmd->a[3];
        FLD(obj, 0xA7, u8) = cmd->a[4];
        break;
    case 0x8C:
        obj->unk31C->unk11A = cmd->a[0];
        break;
    case 0x8B:
        if ((obj - D_800CC2D0) <= D_80082FA0) {
            func_1517EE40(cmd->a[0], cmd->a[1], cmd->a[2], cmd->a[3], cmd->a[4], obj - D_800CC2D0);
        }
        break;
    case 0x8D:
        for (i = 0; i < 25; i++) {
            if (D_800CC2D0[i].interaction_state != 0 && (obj - D_800CC2D0) + 1 == D_800CC2D0[i].unk124 &&
                D_800CC2D0[i].unk5 == 5) {
                func_15060F28(&D_800CC2D0[i], 0);
            }
        }
        break;
    case 0x8E:
        obj->unk180 = cmd->a[0];
        break;
    default:
        return 0;
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_C4DC0/func_15099C14.s")
