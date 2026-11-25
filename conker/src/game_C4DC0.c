#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    u8 pad0[4];
    s32 unk4;
} C4DC0_arg1;

extern s32 func_15082A44(void *, s32, s32, s32, s32);
extern Actor *func_1505EEF4(s32);
extern s32 func_15053430(Actor *);

/* Returns the object id for script id `scriptId` (| 0x2000 when nonzero), spawning it from its slot if needed. */
s32 func_15097910(s32 scriptId, s32 unused) {
    s32 ret;
    Actor *obj;

    ret = func_15083E0C((u8)scriptId);
    if (ret != -1) {
        obj = func_15083E90((u8)scriptId);
        if (obj == NULL) {
            D_800D20FC[ret].unk2 = 0;
            if (func_15082A44(ret + D_800D20FC, ret, 0, 0, 0) == 0) { // matching: index + base operand order
                return -1;
            }
            ret = func_15083E0C((u8)scriptId);
        } else {
            ret = obj->unk13F;
        }
    }
    if (ret != 0) {
        ret |= 0x2000;
    }
    return ret;
}

/* Removes object `objId` (flagging its spawn slot); returns 999999 on success, 0 if there is no such object. */
s32 func_150979CC(s32 objId, C4DC0_arg1 *arg1) {
    Actor *obj;

    obj = func_1505EEF4(objId);
    if (obj == NULL) {
        return 0;
    }
    if (objId < D_800D2100) {
        D_800D20FC[objId].unk2 = 1;
    }
    if (D_800D3840 == 2) {
        if (arg1->unk4 == 0) {
            func_15060F28(obj, 0);
        } else {
            func_15053430(obj);
        }
    } else {
        func_15060F28(obj, 0);
    }
    return 999999;
}


/* A script command as passed to the object command handlers below. Arguments that are speeds, scales or
 * distances are 16.16 fixed point (hence the many `* 1.52587890625e-05f`, i.e. 1/65536); angles in
 * binary units convert to degrees with 0.0054931640625f (360/65536) and back with 182.04444885f. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 a[6];
} ScriptCmd;

/* Raw field access for bytes of Actor that structs.h declares only as padding or with
 * another width (the shared headers are not edited in this pass). */
/* Index of an object in the D_800CC2D0 object table. */
#define OBJ_INDEX(o) ((o) - D_800CC2D0)


extern void func_1503DE70(Actor *, s32, s32);
extern void func_1504BAF0(Actor *);
extern void func_15052F9C(Actor *, f32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_1505D024(Actor *, s32, u16, s32);
extern void func_1505D1C4(f32, f32, f32, s32, s32, s32, s32, s32);
extern void func_15063254(Actor *, s32, s32, f32);
extern void func_15063404(Actor *);
extern void func_150634E4(Actor *);
extern void func_15063570(Actor *);
extern ObjScriptCmd *func_1507BB28(s32, s32);
extern void func_1507CD64(Actor *, s32);
extern void func_1507D1D8(Actor *);
extern void func_1507D4F8(s32);
extern void func_1507EB2C(Actor *);
extern void func_1507EB4C(Actor *, s32);
extern void func_15080620(s32, s32, s32, s32);
extern void func_150836CC(Actor *, s32);
extern s32 func_15084000(s32);
extern void func_15085410(Actor *, s32);
extern s32 func_15086CBC(s32, f32 *, f32 *, f32 *);
extern s32 func_15086D48(s32);
extern void func_1510E7A4(s32, s32, f32 *, s32, s32, s32, f32, f32, f32, f32, s32, s32, f32, f32);
extern void func_1514DE94(Actor *);
extern void func_15172D28(Actor *, s32);
extern void func_1517EE40(s32, s32, s32, s32, s32, s32);
extern void func_1518BA90(Actor *, s32, s32);
extern void func_1518BBF4(Actor *);
extern void func_15193660(Actor *, s32, s32);
extern void func_151937F4(Actor *, s32, s32);
extern void func_151D6BFC(Actor *, s32, s32);
extern f32 sinf(f32);
extern f32 cosf(f32);
extern u8 func_15085420(Actor *);
extern u8 func_15085DA8(f32);
extern s32 func_15086098(f32, f32, f32, s32, s32, s8 *, s8 *);
extern void func_15086364(s32, s8, s8, s8 *, s8 *);
extern f32 func_15086D94(f32, f32, f32, f32, f32);
extern s32 func_150A2AEC(u32, s32, s32 *);
extern TriggerZone *func_150A3194(s32, s32, s32, s32, s32);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, s32, f32 *, f32 *, f32 *, f32 *, s32, s32, f32);
extern f32 func_15144BC8(f32);
extern s8 func_151E564C(void);
extern s8 func_151E565C(void);

/* Script "set" commands: applies command `opcode` with arguments `cmd->a` to object `objId`.
 * Returns 0 for an unknown command or missing object, 1 otherwise.
 * matching: the unused `pad` locals reserve stack slots of the original's block locals; each one is needed. */
s32 func_15097A8C(s32 objId, s32 opcode, ScriptCmd *cmd) {
    Actor *obj;
    s32 i;
    s32 node;
    s32 pad;

    obj = func_1505EEF4(objId);
    if (obj == NULL) {
        return 0;
    }
    switch (opcode) {
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
        D_800C3E78 = OBJ_INDEX(D_800D154C);
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
                obj->unk31C->buttons = v;
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

        if (OBJ_INDEX(obj) >= 0 && OBJ_INDEX(obj) < D_8008FD8C) {
            func_15085710(OBJ_INDEX(obj), 10, cmd->a[0]);
        }
        D_800BE3E4 = func_150859AC(OBJ_INDEX(obj), 6);
    }
        break;
    case 0x63: {
        s32 pad;

        if (OBJ_INDEX(obj) >= 0 && OBJ_INDEX(obj) < D_8008FD8C) {
            func_15085710(OBJ_INDEX(obj), 9, cmd->a[0]);
        }
        D_800BE3E4 = func_150859AC(OBJ_INDEX(obj), 6);
    }
        break;
    case 0x64: {
        s32 v;

        v = cmd->a[0];
        if (D_800BE616 == 0 && (D_800E9D00 & 0x200)) {
            v = 0x33;
        }
        if (OBJ_INDEX(obj) >= 0 && OBJ_INDEX(obj) < D_8008FD8C) {
            func_15085710(OBJ_INDEX(obj), 2, v);
        }
        break;
    }
    case 0x65:
        if (OBJ_INDEX(obj) >= 0 && OBJ_INDEX(obj) < D_8008FD8C) {
            func_15085710(OBJ_INDEX(obj), 3, cmd->a[0]);
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
        D_800C3E78 = OBJ_INDEX(D_800D154C);
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
        if (cmd->a[0] != 0) {
            func_1518BA90(obj, 0xFF, 0);
        } else {
            func_1518BBF4(obj);
        }
        break;
    case 0x72:
        if (obj->unk31C != NULL) {
            obj->unk31C->unk129 = cmd->a[0];
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
        obj->unk20A = cmd->a[0];
        break;
    case 0x1B:
        if (cmd->a[0] == 1) {
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
        func_15085710(objId, cmd->a[0], cmd->a[1]);
        break;
    case 0x22: {
        s32 j;

        for (j = 0; j < D_800D3840 - 2; j++) {
            if (cmd->a[j] != 0x78000000) {
                obj->unk2D8[j] = cmd->a[j] * 1.52587890625e-05f;
            }
        }
        break;
    }
    case 0x23: {
        s32 j;

        for (j = 0; j < D_800D3840 - 2; j++) {
            if (cmd->a[j] != 0x7FFFFFFF) {
                (&obj->unk2E4)[j] = cmd->a[j];
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
            obj->unk31C->unk8 = 1;
            if (cmd->a[0] == 1) {
                func_1507EB4C(obj, 0x15);
            } else if (cmd->a[0] == 2) {
                func_1507EB4C(obj, 0x2A);
                obj->unk31C->unk16 = 0xFF;
            }
        }
        break;
    case 0x26:
        func_15080620(objId, 0, cmd->a[0], 0);
        break;
    case 0x27:
        func_15080620(objId, 1, cmd->a[0], 0);
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
        obj->unk31C->unk76 = cmd->a[0];
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
        D_800BE728[cmd->a[0]]->button = cmd->a[1];
        break;
    case 0x56:
        obj->unk65 = cmd->a[0];
        break;
    case 0x2F: {
        Actor *other;

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
        obj->unk10A = cmd->a[0];
        break;
    case 0x33:
        D_800D1898[cmd->a[0]] = cmd->a[1] * 1.52587890625e-05f;
        break;
    case 0x34: {
        s32 x;
        s32 z;
        f32 y;

        x = D_800DBFF0->eye.x;
        z = D_800DBFF0->eye.z;
        x = x - sinf(D_800DBFF0->unk3A0) * 1800.0f;
        z = z - cosf(D_800DBFF0->unk3A0) * 1800.0f;
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
        y = D_800DBFF0->eye.y;
        func_1510E7A4(0, 0, &y, 0, 0, 0, x, y, z, y, 0, 0, -10000.0f, 20000.0f);
        obj->x_position = x;
        obj->y_position = cmd->a[4] + y;
        obj->z_position = z;
    }
        break;
    case 0x35:
        D_800D154C = obj;
        D_800C3E78 = OBJ_INDEX(D_800D154C);
        obj->unk218 = func_1507BB28(0, cmd->a[0]);
        obj->unk21C = 0;
        break;
    case 0x36:
        if (cmd->a[0] != 0) {
            obj->unk31C->unk20 |= cmd->a[1];
        } else {
            obj->unk31C->unk20 &= ~cmd->a[1];
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
        break;
    }
    case 0x3B:
        obj->unk31C->unk32 |= cmd->a[0];
        if (D_800D3840 == 4) {
            obj->unk31C->unk34 = cmd->a[1];
        }
        break;
    case 0x3C:
        if (cmd->a[0] != 0) {
            obj->unk31C->unk1A = 1;
        } else {
            obj->unk31C->unk1A = 0;
        }
        break;
    case 0x3D:
        if (cmd->a[0] == 0) {
            obj->unk94 = ~(cmd->a[1] | 1);
        } else if (cmd->a[0] == 1) {
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
        D_800D2104[obj->unk13F][cmd->a[0] + 1].x = cmd->a[1];
        D_800D2104[obj->unk13F][cmd->a[0] + 1].y = cmd->a[2];
        D_800D2104[obj->unk13F][cmd->a[0] + 1].z = cmd->a[3];
        break;
    case 0x44: {
        f32 px;
        f32 py;
        f32 pz;

        node = func_15086D48(cmd->a[1]);
        if (node != -1) {
            func_15086CBC(node, &px, &py, &pz);
            D_800D2104[obj->unk13F][cmd->a[0] + 1].x = px;
            D_800D2104[obj->unk13F][cmd->a[0] + 1].y = py;
            D_800D2104[obj->unk13F][cmd->a[0] + 1].z = pz;
        }
        break;
    }
    case 0x45:
        obj->unk31C->unk7B = cmd->a[0];
        break;
    case 0x46:
        obj->unk234 = cmd->a[0];
        obj->unk236 = cmd->a[1];
        obj->unk237 = cmd->a[2];
        break;
    case 0x47:
        obj->unk31C->unk1B = cmd->a[0];
        break;
    case 0x48:
        obj->unkD0 = cmd->a[0];
        obj->unk114 = cmd->a[1];
        break;
    case 0x49:
        obj->lodRequest = cmd->a[0];
        break;
    case 0x4A:
        func_1507EB4C(obj, cmd->a[0]);
        break;
    case 0x4B: {
        s32 v;

        v = cmd->a[0];
        func_15083568(obj, v, 0.0f, 0);
        break;
    }
    case 0x4C:
    {
        s32 v;

        v = cmd->a[0];
        func_150836CC(obj, v);
        break;
    }
    case 0x4D: {
        ModelAttachment *p;
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
        Actor *other;
        s32 pad;

        other = func_15083E90(cmd->a[0] & 0xFF);
        if (other != NULL) {
            if (cmd->a[2] == 0) {
                obj->unk101 |= 4;
            }
            obj->unk65 = OBJ_INDEX(other) + 1;
            obj->unk5C = cmd->a[1];
        }
        break;
    }
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
        break;
    }
    case 0x57:
        obj->unk21C = cmd->a[0];
        break;
    case 0x58:
        obj->unk140 = cmd->a[0];
        break;
    case 0x55: {
        s32 pad;
        s32 idx;

        idx = OBJ_INDEX(obj);
        if (idx < 4) {
            D_800BE728[idx]->button |= B_BUTTON;
            D_800BE710[idx] |= 0x4000;
        }
    }
        break;
    case 0x59:
        obj->unk20B[cmd->a[1]] = cmd->a[0];
        break;
    case 0x68:
        obj->unk10 = cmd->a[0];
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
        if (cmd->a[0] == 1) {
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
        obj->eyeCode[idx] = val + 10;
    }
        break;
    case 0x6F: {
        s32 idx;
        s32 val;

        idx = cmd->a[0];
        val = cmd->a[1];
        obj->unk68[idx] = val;
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

        /* Place the object at a random distance in [a[2], a[3]) from (a[0], a[1]), at a random angle (degrees)
         * between the bounds a[4] / a[5], which are XOR-swapped into order first. */
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
            obj->unk31C->unk1AE = cmd->a[0];
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
            if (OBJ_INDEX(obj) < D_8008FD90) {
                obj->unk31C->unk4E = 2;
                func_1505E650(obj, 5, 1.0f, 0.0f, 0.0f, 0.0f, 0);
            }
        }
            break;
        case 1:
            func_1514DE94(obj);
            break;
        case 2:
            obj->unk31C->unk17 = 1;
            obj->lodRequest = 6;
            break;
        case 3: {
            Actor *other;

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
        Actor *other;
        s32 pad;

        other = func_15083E90(cmd->a[0] & 0xFF);
        if (other != NULL) {
            obj->unk13C = OBJ_INDEX(other) + 100;
            obj->xz_velocity = 0.0f;
            func_1505959C(other, OBJ_INDEX(obj));
        }
        break;
    }
    case 0x7B:
        obj->unk31C->unk1AF = cmd->a[0];
        break;
    case 0x7C:
        obj->unk23E = cmd->a[0];
        break;
    case 0x7D:
        obj->unk328 = cmd->a[0] + 1;
        break;
    case 0x7E:
        obj->unk31C->unk7C = cmd->a[0];
        break;
    case 0x7F:
        obj->spawn->flags = cmd->a[0];
        break;
    case 0x80:
        {
            f32 f;

            f = cmd->a[2] * 0.01f;
            func_15063254(obj, cmd->a[0], cmd->a[1], f);
        }
        break;
    case 0x81:
        obj->unk275 = cmd->a[0];
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
        func_1507D4F8(OBJ_INDEX(obj));
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
        obj->fogMode = cmd->a[0];
        obj->fogLevel = cmd->a[1];
        obj->fogRate = cmd->a[2];
        obj->fogColor = cmd->a[3];
        obj->fogFade = cmd->a[4];
        break;
    case 0x8C:
        obj->unk31C->unk11A = cmd->a[0];
        break;
    case 0x8B:
        if (OBJ_INDEX(obj) <= D_80082FA0) {
            func_1517EE40(cmd->a[0], cmd->a[1], cmd->a[2], cmd->a[3], cmd->a[4], OBJ_INDEX(obj));
        }
        break;
    case 0x8D:
        for (i = 0; i < 25; i++) {
            if (D_800CC2D0[i].interaction_state != 0 && OBJ_INDEX(obj) + 1 == D_800CC2D0[i].unk124 &&
                D_800CC2D0[i].unk5 == 5) {
                func_15060F28(&D_800CC2D0[i], 0);
            }
        }
        break;
    case 0x8E:
        obj->floorY = cmd->a[0];
        break;
    default:
        return 0;
    }
    return 1;
}



/* Script "query" commands: returns the value of query `opcode` (arguments `cmd->a`) for object `objId`;
 * 0 for an unknown query or a missing object (except the existence queries 0xB0 / 0xB1 / 0xB7).
 * matching: the unused `pad` locals reserve stack slots of the original's block locals; each one is needed. */
s32 func_15099C14(s32 objId, s32 opcode, ScriptCmd *cmd) {
    Actor *obj;
    Actor *other;
    s32 ret;
    s32 ix;
    s32 iz;
    f32 dx;
    f32 dz;

    if (opcode == 0xB7) {
        return func_15083E0C(objId);
    }
    obj = func_1505EEF4(objId);
    if (opcode == 0xB0) {
        if (obj == NULL) {
            return 1;
        }
        return 0;
    }
    if (opcode == 0xB1) {
        if (obj == NULL) {
            return 1;
        }
        if (obj->interaction_state == 8 || obj->interaction_state == 0x27) {
            return 1;
        }
        return 0;
    }
    if (obj == NULL) {
        return 0;
    }
    switch (opcode) {
    case 0xA5:
        return func_150859AC(objId, ((s16 *)&cmd->a[0])[1]);
    case 0x91:
        return obj->unk232;
    case 0x92:
        return obj->unk21C;
    case 0x93:
        return obj->health;
    case 0xA8:
        if (obj->stunned != 0) {
            return 1;
        }
        break;
    case 0x94:
        if (obj->unk13D != 0) {
            return 1;
        }
        break;
    case 0x95:
        if (obj->unk13C != 0 && func_15084000(cmd->a[0] & 0xFFFF0FFF) + 100 == obj->unk13C) {
            return 1;
        }
        break;
    case 0x96:
        if (obj->in_water == 1) {
            return 1;
        }
        break;
    case 0x97:
        ix = obj->x_position;
        iz = obj->z_position;
        return (ix << 16) | (iz & 0xFFFF);
    case 0x98:
        return (s16)(s32)obj->y_position;
    case 0x99:
        other = func_1505EEF4(cmd->a[0] & 0xFFFF0FFF);
        dx = obj->x_position - other->x_position;
        dz = obj->z_position - other->z_position;
        if (dx * dx + dz * dz < (f32)(cmd->a[1] * cmd->a[1])) {
            return 1;
        }
        return 0;
    case 0x9A: {
        s32 pad1;
        s32 pad2;
        other = func_1505EEF4(cmd->a[0] & 0xFFFF0FFF);
        if (other == NULL) {
            return 32000;
        }
        dx = obj->x_position - other->x_position;
        dz = obj->z_position - other->z_position;
        return sqrtf(dx * dx + dz * dz);
    }
    case 0x9B: {
        f32 dx;
        f32 dz;
        dz = obj->z_position - (f32)cmd->a[1];
        dx = obj->x_position - (f32)cmd->a[0];
        return sqrtf(dz * dz + dx * dx);
    }
    case 0x9C:
        other = func_1505EEF4(cmd->a[0] & 0xFFFF0FFF);
        return (f32)(func_1505A630(other->x_position - obj->x_position, obj->z_position - other->z_position, 0) + 0x4000) * 0.0054931640625f * 65536.0f;
    case 0x9D:
        return (f32)(u32)(u16)(func_1505A630((f32)cmd->a[0] - obj->x_position, obj->z_position - (f32)cmd->a[1], 0) - 0x4000) * 0.0054931640625f * 65536.0f;
    case 0x9E: {
        s32 r;
        s32 dx;
        s32 dz;
        r = cmd->a[2];
        dx = (s32)obj->x_position - cmd->a[0];
        dz = (s32)obj->z_position - cmd->a[1];
        if (dx * dx + dz * dz < r * r) {
            return 1;
        }
        return 0;
    }
    case 0x9F:
        return obj->unk48;
    case 0xA0:
        return obj->unk223;
    case 0xA1:
        return obj->unk234;
    case 0xB3:
        return obj->unk40 * 65536.0f;
    case 0xA4:
        return obj->unk7A;
    case 0xA2:
        ret = 0;
        if (obj->unk31C->unk57 != 0) {
            ret = 1;
        }
        return ret;
    case 0xA3:
        return obj->unk25C & 0x40;
    case 0xA6:
        return (obj->unk1CC - obj->y_position) > 150.0f;
    case 0xA7:
        return func_15085420(obj) | 0xD000;
    case 0xA9:
        return obj->unk31C->unk77;
    case 0xAA:
        return obj->unk31C->unk78;
    case 0xAB:
        return obj->unk31C->unk79;
    case 0xAC:
        ret = func_150A2AEC(OBJ_INDEX(obj), D_800D3840 - 2, cmd->a);
        if (ret != -1) {
            return ret | 0x4000;
        }
        return 0;
    case 0xAD:
        return obj->unk251;
    case 0xAE:
        return obj->xz_velocity;
    case 0xAF:
        if (obj->unk31C->unk16 != 0) {
            return 2;
        }
        if (obj->unk31C->unk8 != 0) {
            return 1;
        }
        return 0;
    case 0xB2:
        return (&obj->unk2E4)[cmd->a[0]];
    case 0xB4:
        return obj->unk10A;
    case 0xB5: {
        u8 v;
        v = obj->unk10A;
        if (v & 0x80) {
            return D_800CC2D0[v & ~0x80].interaction_state;
        }
        return 0xFFFF;
    }
    case 0xB6: {
        u8 *p;
        s32 v;
        p = &D_800E0BB8[OBJ_INDEX(obj)];
        v = *p;
        *p &= 0x7F;
        return v;
    }
    case 0xB8:
        return obj->unk2CA;
    case 0xB9: {
        s32 x;
        s32 z;
        x = D_800D2104[obj->unk13F][cmd->a[0]].x;
        z = D_800D2104[obj->unk13F][cmd->a[0]].z;
        return (x << 16) | (z & 0xFFFF);
    }
    case 0xBA:
        return D_800D2104[obj->unk13F][cmd->a[0]].y;
    case 0xBB:
        if (obj->unk65 != 0) {
            return D_800CC2D0[obj->unk65 - 1].unk13F;
        }
        return -1;
    case 0xBC:
        return obj->unk13C;
    case 0xBE:
        return obj->y_velocity;
    case 0xBD: {
        s32 pad1;
        s32 pad2;
        other = func_1505EEF4(cmd->a[0] & 0xFFFF0FFF);
        dx = obj->x_position - other->x_position;
        dz = obj->z_position - other->z_position;
        return sqrtf(dx * dx + dz * dz);
    }
    case 0xBF:
        return (f32)(s32)obj->unk28 * 65536.0f;
    case 0xC0:
        return obj->unk31C->unk1B;
    case 0xC1:
        return obj->unk31C->unk4E;
    case 0xC2:
        return obj->unique_id;
    case 0xC3:
        return obj->unkF4;
    case 0xC4:
        return obj->unk25C;
    case 0xC5:
        return obj->disable_run;
    case 0xC7:
        if (obj->unk211 == 0xFF) {
            return 1;
        }
        return obj->unk25C & 0x800;
    case 0xDB:
        if (cmd->a[0] == 1) {
            if (obj->unk20F == 0xFF) {
                return 0xFF;
            }
            return D_800D2350[obj->unk20F].unk7;
        }
        if (obj->unk211 == 0xFF) {
            return 0xFF;
        }
        return D_800D2350[obj->unk211].unk7;
    case 0xC8: {
        f32 x;
        f32 y;
        f32 z;
        s32 dist2;
        s32 dist;
        s32 found;
        if (obj->unk20F == 0xFF || obj->unk211 == 0xFF) {
            return 0;
        }
        found = func_15086CBC(obj->unk20F, &x, &y, &z);
        dist = -1;
        if (found != 0) {
            x -= obj->x_position;
            y -= obj->y_position;
            z -= obj->z_position;
            dist = obj->unk216 + (s32)sqrtf(x * x + y * y + z * z);
            if (obj->unk20F != obj->unk211) {
                if (func_15086CBC(obj->unk211, &x, &y, &z) != 0) {
                    x -= obj->x_position;
                    y -= obj->y_position;
                    z -= obj->z_position;
                    dist2 = sqrtf(x * x + y * y + z * z);
                    if (dist < dist2) {
                        return dist2 | 0;
                    }
                }
            }
        }
        return dist;
    }
    case 0xC9:
        return func_15085DA8(obj->y_position);
    case 0xCA: {
        u8 b;
        b = 0xFF;
        if (obj->unk20F != 0xFF) {
            return D_800D2350[obj->unk20F].unk7;
        }
        D_8008729C = obj->unk212[1];
        ret = func_15086098(obj->x_position, obj->y_position, obj->z_position, 0, 0, &b, &b);
        obj->unk212[1] = ret;
        if (ret != 0xFF) {
            return D_800D2350[ret].unk7;
        }
        return 0xFF;
    }
    case 0xCB: {
        f32 dx;
        f32 dy;
        f32 dz;
        f32 hx;
        f32 hy;
        f32 hz;
        f32 hd;
        f32 dist;
        other = func_1505EEF4(cmd->a[0] & 0xFFFF0FFF);
        if (other == NULL) {
            return 0;
        }
        dx = other->x_position - obj->x_position;
        dy = other->y_position - obj->y_position;
        dz = other->z_position - obj->z_position;
        dist = sqrtf(dx * dx + dy * dy + dz * dz);
        if (func_150AC9C0(obj->x_position, obj->y_position + 100.0f, obj->z_position, dx, dy, dz, 0, 0, &hx, &hy, &hz, &hd, 0, 0, 0.0f) == 0) {
            return dist;
        }
        if (dist < hd) {
            if (func_15086D94(obj->x_position, obj->y_position, obj->z_position, dx, dz) > 0.0f) {
                return 0;
            }
            return dist;
        }
        return 0;
    }
    case 0xCC: {
        TriggerZone *p;
        p = func_150A3194(3, 11, obj->x_position, obj->y_position, obj->z_position);
        if (p == NULL) {
            return -1;
        }
        ret = p - D_800D3098;
        return ret;
    }
    case 0xCF: {
        s32 found;
        u8 foundNode;
        s32 i;
        s32 ret;
        s8 count;
        s8 n2;
        s8 nodes[18];

        /* Resolve the cmd->a[0] node ids in cmd->a[1..] (dropping unknown ones), then pick one from the
         * object position with func_15086098 / func_15086364. */
        n2 = 0;
        ret = cmd->a[0];
        count = 0;
        for (i = 0; i < ret; i++) {
            nodes[count] = func_15086D48(cmd->a[1 + i]);
            if ((u8)nodes[count] != 0xFF) {
                count++;
            }
        }
        ret = -1;
        if (count != 0) {
            found = func_15086098(obj->x_position, obj->y_position, obj->z_position, count, n2, nodes, &n2);
            foundNode = found;
            D_80087298 = 0xFF;
            if ((u8)found != 0xFF) {
                func_15086364(foundNode, count, n2, nodes, &n2);
            }
            if (D_80087298 != 0xFF) {
                ret = D_800D2350[D_80087298].unk7;
            }
        }
        return ret;
    }
    case 0xD0: {
        s32 id;
        f32 x;
        f32 y;
        f32 z;
        id = func_15086D48(cmd->a[0]);
        if (func_15086CBC(id, &x, &y, &z) != 0) {
            x -= obj->x_position;
            z -= obj->z_position;
            ret = sqrtf(x * x + z * z);
        } else {
            ret = 30000;
        }
        return ret;
    }
    case 0xCD: {
        s32 temp;
        s32 pad1;
        s32 pad2;
        f32 a;
        f32 b;
        f32 c;
        other = func_1505EEF4(cmd->a[0] & 0xFFFF0FFF);
        temp = func_1505A630(other->x_position - obj->x_position, obj->z_position - other->z_position, 0) + 0x4000;
        a = (f32)temp * 0.0054931640625f;
        b = obj->unk40;
        a = func_15144BC8(a);
        c = func_15144BC8(func_15144BC8(b) - a);
        if (c > 180.0f) {
            c = -360.0f + c;
        }
        if (cmd->a[1] == 1 && c < 0.0f) {
            c = -c;
        }
        return c;
    }
    case 0xCE: {
        f32 dx;
        f32 dz;
        f32 c;
        other = func_1505EEF4(cmd->a[0] & 0xFFFF0FFF);
        dx = other->x_position - obj->x_position;
        dz = other->z_position - obj->z_position;
        c = (u32)func_1505A630(sqrtf(dx * dx + dz * dz), other->y_position - obj->y_position, 0) * 0.0054931640625f;
        c = func_15144BC8(c);
        if (c > 180.0f) {
            c = -360.0f + c;
        }
        if (cmd->a[1] == 1 && c < 0.0f) {
            c = -c;
        }
        return c;
    }
    case 0xD1:
        ret = obj->unk22C;
        obj->unk22C = ret & ~0x100;
        return ret & 0x100;
    case 0xD2:
        ret = obj->unk22C;
        obj->unk22C = ret & ~0x200;
        return ret & 0x200;
    case 0xD4: {
        ActorCtl *p;
        s32 v;
        p = obj->unk31C;
        v = 0;
        if (p != NULL) {
            v = p->unk78;
        }
        return v;
    }
    case 0xD3: {
        ActorCtl *p;
        s32 v;
        p = obj->unk31C;
        v = 0;
        if (p != NULL) {
            switch (p->unk75 & 0x7F) {
            case 3:
            case 9:
            case 21:
                return 2;
            case 33:
            case 34:
                return 3;
            case 20:
            case 22:
            case 24:
            case 35:
            case 36:
            case 55:
            case 56:
            case 57:
            case 63:
            case 65:
                v = 1;
                break;
            }
        }
        return v;
    }
    case 0xD5:
        return func_151E564C();
    case 0xD6:
        return func_151E565C();
    case 0xD7:
        if ((obj->unk1CC - obj->y_position) > 600.0f && obj->unk118 == -10000.0f) {
            return 1;
        }
        return 0;
    case 0xD8:
        return obj->in_water;
    case 0xD9:
        if (obj->unkA9 == 2) {
            return 1;
        }
        return 0;
    case 0xDA:
        return obj->floorY;
    case 0xDC:
        if (obj->unk31C->unk195 != 0) {
            return obj->unk31C->unk196 | 0x80;
        }
        return 0;
    case 0xDD:
        if (obj->unk31C != NULL) {
            return obj->unk31C->unk23;
        }
        return 0;
    case 0xDE:
        return OBJ_INDEX(obj);
    case 0xC6:
        ret = OBJ_INDEX(obj);
        if (ret >= 0 && ret < D_8008FD8C) {
            return func_150859AC(OBJ_INDEX(obj), 6);
        }
        break;
    case 0xDF:
        if (obj->unk31C != NULL) {
            return obj->unk31C->unk1AE;
        }
        return 0;
    case 0xE0:
        return obj->y_scale * 65536.0f;
    case 0xE1:
        return obj->immune;
    case 0xE2:
        return obj->unk31C->matrix_physics;
    case 0xE3:
        return D_800D18A9;
    case 0xE4:
        return D_800D18A8;
    case 0xE5:
        return obj->unk275;
    case 0xE6: {
        u8 v;
        Actor *p;
        if (obj->unk275 != 0) {
            p = &D_800CC2D0[obj->unk275 - 1];
            obj->unk275 = 0;
            if (p->id == cmd->a[0]) {
                return 1;
            }
            return 0;
        }
        return 0;
    }
    case 0xE7: {
        u8 idx;
        PathPoint *pts;
        s32 n;
        s32 best;
        s32 x;
        s32 z;
        s32 maxd;
        s32 mind;
        s32 i;
        s32 dx;
        s32 dz;
        idx = obj->unk13F;
        pts = D_800D2104[idx];
        n = D_800D2108[idx];
        best = -1;
        if (pts != NULL && n != 0) {
            other = func_1505EEF4(cmd->a[0] & 0xFFFF0FFF);
            x = other->x_position;
            z = other->z_position;
            maxd = 100000000;
            mind = -1;
            if (D_800D3840 >= 4) {
                ret = cmd->a[1];
                mind = ret * ret;
                if (D_800D3840 >= 5) {
                    ret = cmd->a[2];
                    maxd = ret * ret;
                }
            }
            for (i = 0; i < n; i++, pts++) {
                dx = pts->x - x;
                dz = pts->z - z;
                dx = dz * dz + dx * dx;
                if (dx < maxd && mind < dx) {
                    best = i;
                    maxd = dx;
                }
            }
        }
        return best;
    }
    case 0xE8:
        return obj->unk84.uh;
    case 0xE9:
        return obj->unk31C->unk51;
    case 0xEA: {
        ModelAttachment *p;
        s32 a;
        s32 b;
        a = cmd->a[0];
        b = cmd->a[1];
        p = func_1503195C(obj, a, b);
        if (p != NULL) {
            return p->unk38;
        }
        return 0;
    }
    case 0xEB: {
        f32 r;
        f32 s;
        s32 pad2;
        s32 pad3;
        s32 pad4;
        s32 pad5;
        f32 *pp; // matching: dead address-of stores keep dz/dy/dx/dist in stack slots
        f32 dz;
        f32 dy;
        f32 dx;
        f32 dist;
        pp = &dz;
        pp = &dy;
        pp = &dx;
        pp = &dist;
        ix = cmd->a[0];
        iz = cmd->a[1];
        r = cmd->a[2];
        dx = obj->x_position - (f32)ix;
        dy = 0.0f;
        dz = obj->z_position - (f32)iz;
        dist = dz * dz + dx * dx;
        if (dist <= r * r) {
            ret = obj->x_position;
            iz = obj->z_position;
        } else {
            s = r / sqrtf(dist);
            ret = (dx *= s) + (f32)ix;
            iz = (dz *= s) + (f32)iz;
        }
        return (ret << 16) | (iz & 0xFFFF);
    }
    case 0xEC:
        return obj->unk31C->unk95;
    case 0xED:
        return D_800BE710[objId];
    case 0xEE: {
        s16 px;
        f32 x;
        f32 y;
        f32 z;

        /* Offset cmd->a[0] along the reverse of facing angle unk7A, packed as (x << 16) | (z & 0xFFFF). */
        func_1505A184(obj->unk7A + 0x8000, cmd->a[0], 0.0f, &x, &z, &y);
        px = (s16)(s32)obj->x_position + x;
        iz = (s16)((s16)(s32)obj->z_position + z);
        return (px << 16) | (iz & 0xFFFF);
    }
    case 0xEF:
        return obj->interaction_state;
    case 0xF0:
        return D_800E0AC0[obj->team];
    case 0xF1:
        return (obj->unk74 & 0xF) == 0xF;
    case 0xF2:
        if (obj->unk13C != 0) {
            return D_800CC2D0[obj->unk13C].unk13F | 0x2000;
        }
        return 0;
    case 0xF3:
        if (obj->unkA9 == 2) {
            return 1;
        }
        return 0;
    default:
        return 0;
    }
    return 0;
}
