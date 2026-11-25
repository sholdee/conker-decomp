#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A scripted actor move: actor `actor` is sent to pose `poseIndex` for `timer` frames. */
typedef struct {
    /* 0x00 */ u8 state; // 0 = idle, 1 = start pending, 2 = running
    /* 0x01 */ u8 actor; // index into D_800DBFF0
    /* 0x02 */ u8 poseIndex; // index into the D_800DC020 pose table
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 timer;
    /* 0x06 */ u8 pad6[6];
    /* 0x0C */ s32 flags;
    /* 0x10 */ f32 scale; // copied to D_800D2E30 while running
    /* 0x14 */ u8 pad14[0xC];
    /* 0x20 */ f32 unk20; // actor->unk37C at setup, written to actor->pad3A4 when done
} C3E20Command; // size 0x24

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ u8 pad6[6];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} C3E20Pose; // size 0x18

extern C3E20Command D_800D2DC0[3];
extern s32 D_800D2E30[];
extern f32 D_800D2DB8;

void func_1512D560(struct108 *, s32, struct168 *);

/* Clears all command slots. */
void func_15096970(void) {
    bzero(D_800D2DC0, sizeof(D_800D2DC0));
    D_800D2DB4 = 0;
}

/* Returns TRUE if any of the first `count` command slots is active. */
s32 func_150969A0(s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        if (D_800D2DC0[i].state != 0) {
            return TRUE;
        }
    }

    return FALSE;
}

s32 func_15096A68(s32 arg0) {
    u8 idx;
    C3E20Pose *pos;
    struct108 *obj;
    struct108 *obj2;

    switch (D_800D2DC0[arg0].state) {
    case 1:
        idx = D_800D2DC0[arg0].actor;
        obj = &D_800DBFF0[idx];
        pos = *(C3E20Pose **)&D_800DC020;
        pos = &pos[D_800D2DC0[arg0].poseIndex & 0xFF];
        D_800D2DB8 = obj->unk2C0;
        D_800D2DB4 = 1;
        D_800DBFF4[idx] = 3;
        ((struct168 *)&D_800C3600)->unk0 = 1;
        ((struct168 *)&D_800C3600)->unk4 = 0;
        ((struct168 *)&D_800C3600)->unk14 = pos->x;
        ((struct168 *)&D_800C3600)->unk18 = pos->y;
        ((struct168 *)&D_800C3600)->unk1C = pos->z;
        ((struct168 *)&D_800C3600)->unk20 = pos->unk14;
        ((struct168 *)&D_800C3600)->unk24 = pos->unkC;
        ((struct168 *)&D_800C3600)->unk28 = pos->unk10;
        ((struct168 *)&D_800C3600)->unk8 = 0.0f;
        func_1512D560(obj, 5, 0);
        func_1512D560(obj, 7, (struct168 *)&D_800C3600);
        if (!(D_800D2DC0[arg0].flags & 1)) {
            obj->unk3D0->unk7 = 0;
            obj->unk3D0->unk25C |= 0x200;
        }
        D_800D2E30[arg0] = D_800D2DC0[arg0].scale;
        D_800D2DC0[arg0].state = 2;
    lbl1:
        break;
    case 2:
        obj2 = &D_800DBFF0[D_800D2DC0[arg0].actor];
        D_800D2DC0[arg0].timer -= D_800BE9A0;
        if (obj2->unk3D0->health == 0) {
            D_800D2DC0[arg0].timer = 0;
        }
        if (D_800D2DC0[arg0].timer <= 0) {
            obj2->pad3A4 = D_800D2DC0[arg0].unk20;
            if (!(D_800D2DC0[arg0].flags & 2)) {
                func_1512D560(obj2, 6, 0);
            }
            D_800D2DB4 = 0;
            if (!(D_800D2DC0[arg0].flags & 1)) {
                obj2->unk3D0->unk25C &= ~0x200;
                obj2->unk3D0->unk7 = 0xFF;
            }
            D_800D2DC0[arg0].timer = 0;
            D_800D2DC0[arg0].state = 0;
            obj2->unk2C0 = D_800D2DB8;
            return 0;
        }
        break;
    }
    return 1;
}

/* Runs the active command slots until one of them is still busy. */
void func_15096D08(void) {
    s32 i;

    if (D_800C35EA != 1) {
        for (i = 0; i < 3; i++) {
            if (D_800D2DC0[i].state != 0 && func_15096A68(i) != 0) {
                break;
            }
        }
    }
}

extern s32 D_800D3840;
extern s32 D_800DC310[][0x1A];
extern s8 D_800C365C;
s32 func_15084000(s32);
void func_151254F4(struct108 *arg0, s32 arg1);
void func_1512A390(struct108 *arg0);

#define OBJF(o, off) (*(f32 *)((u8 *)(o) + (off)))

s32 func_15096D78(s32 arg0, s32 arg1, s32 *arg2) {
    struct108 *obj;

    obj = &D_800DBFF0[arg0];
    if (D_80082FA0 < arg0) {
        return 0;
    }
    switch (arg1) {
    case 12:
        D_800D2DC0[arg2[2]].unk20 = obj->unk37C;
        D_800D2DC0[arg2[2]].actor = arg0;
        D_800D2DC0[arg2[2]].poseIndex = arg2[3];
        D_800D2DC0[arg2[2]].timer = arg2[4];
        D_800D2DC0[arg2[2]].state = 1;
        if (D_800D3840 >= 6) {
            D_800D2DC0[arg2[2]].flags = arg2[5];
        } else {
            D_800D2DC0[arg2[2]].flags = 0;
        }
        if (D_800D3840 >= 7) {
            D_800D2DC0[arg2[2]].scale = arg2[6] * 1.52587890625e-05f;
        } else {
            D_800D2DC0[arg2[2]].scale = 1.0f;
        }
        if (!func_150969A0(arg2[2])) {
            func_15096A68(arg2[2]);
        }
        break;
    case 4:
        func_15123934(obj, arg2[2], arg2[3], arg2[4], 0x11);
        return 1;
    case 5:
        func_151239CC(obj, 0x11);
        return 1;
    case 6:
        if (arg2[2] != 0) {
            obj->unk84 |= arg2[3];
        } else {
            obj->unk84 &= ~arg2[3];
        }
        return 1;
    case 8:
        if (obj->unk3D4->unk120 == 0) {
            obj->unk374 = arg2[2];
            obj->unk34C = arg2[3];
            obj->unk348 = arg2[3];
        }
        return 1;
    case 7:
        obj->unk1E0 = arg2[2];
        obj->unk1B4 = arg2[3];
        return 1;
    case 9:
        obj->unk134 = arg2[2];
        return 1;
    case 10:
      {
        C3E20Pose *pos;
        pos = *(C3E20Pose **)&D_800DC020;
        pos = &pos[arg2[2] & 0xFFF];
        D_800D2DB4 = 1;
        D_800DBFF4[arg0] = 3;
        ((struct168 *)&D_800C3600)->unk0 = 1;
        ((struct168 *)&D_800C3600)->unk4 = 0;
        ((struct168 *)&D_800C3600)->unk14 = pos->x;
        ((struct168 *)&D_800C3600)->unk18 = pos->y;
        ((struct168 *)&D_800C3600)->unk1C = pos->z;
        ((struct168 *)&D_800C3600)->unk20 = pos->unk14;
        ((struct168 *)&D_800C3600)->unk24 = pos->unkC;
        ((struct168 *)&D_800C3600)->unk28 = pos->unk10;
        ((struct168 *)&D_800C3600)->unk8 = 0.0f;
        D_800C365C = 0;
        func_1512D560(obj, 5, 0);
        func_1512D560(obj, 7, (struct168 *)&D_800C3600);
        D_800CC2D0[0].unk25C |= 0x200;
      }
        break;
    case 11:
        func_1512D560(obj, 6, 0);
        D_800D2DB4 = 0;
        D_800CC2D0[0].unk25C &= ~0x200;
        break;
    case 14:
        obj->unk2A4 = arg2[2];
        obj->unk2A8 = arg2[3];
        OBJF(obj, 0x2AC) = arg2[4];
        obj->unk2BC = arg2[2];
        obj->unk2C0 = arg2[3];
        obj->unk2C4 = arg2[4];
        obj->unk2F8 = arg2[5];
        obj->unk2FC = arg2[6];
        obj->unk300 = arg2[7];
        obj->unk358 = obj->unk2FC;
        func_1512A390(obj);
        break;
    case 2:
        obj->unk2F8 = arg2[2];
        obj->unk2FC = arg2[3];
        obj->unk300 = arg2[4];
        obj->unk358 = obj->unk2FC;
        func_1512A390(obj);
        break;
    case 3:
        obj->unk2A4 = arg2[2];
        obj->unk2A8 = arg2[3];
        OBJF(obj, 0x2AC) = arg2[4];
        break;
    case 15:
        obj->unk198 = arg2[2];
        if (D_800D3840 == 4) {
            obj->unk194 = arg2[3];
        }
        break;
    case 16:
        obj->unk190 = arg2[2];
        if (D_800D3840 == 4) {
            obj->unk18C = arg2[3];
        }
        break;
    case 17:
        if (D_800D3840 == 5 && arg2[4] != 0) {
            func_1512D748(obj, arg2[2], arg2[3]);
            return 1;
        }
        if (D_800DC310[arg0][0] == 5) {
            func_1512D748(obj, arg2[2], arg2[3]);
            return 1;
        }
        return 0;
    case 18:
        func_151254F4(obj, func_15084000(arg2[2] & 0xFFF));
        break;
    case 19:
        func_15124770(obj, func_15084000(arg2[2] & 0xFFF));
        break;
    case 20:
        obj->unk1A4 = arg2[2] * 1.52587890625e-05f;
        obj->unk1A8 = arg2[3] * 1.52587890625e-05f;
        if (D_800D3840 == 5 && arg2[4] != 0) {
            obj->unk19C = obj->unk1A4;
            obj->unk1A0 = obj->unk1A8;
        }
        break;
    case 21:
      {
        C3E20Pose *pos;
        pos = *(C3E20Pose **)&D_800DC020;
        pos = &pos[arg2[2] & 0xFFF];
        pos->x = arg2[3];
        pos->y = arg2[4];
        pos->z = arg2[5];
      }
        break;
    case 22:
        obj->unk23C = 1;
        break;
    case 23:
        OBJF(obj, 0x7C0) = arg2[2] * 1.52587890625e-05f;
        break;
    case 24:
        if (D_800D3840 == 5) {
            *((u8 *)obj + 0x78C) = 1;
            OBJF(obj, 0x794) = arg2[2];
            OBJF(obj, 0x790) = arg2[3];
            OBJF(obj, 0x798) = arg2[4];
        } else {
            *((u8 *)obj + 0x78C) = 0;
        }
        break;
    case 25:
        obj->pad3A4 = arg2[2] * 1.52587890625e-05f;
        D_800D2DB0 = obj->pad3A4;
        break;
    case 26:
        func_1512868C(obj);
        break;
    case 27:
        D_80089520 = arg2[2] - 1;
        break;
    case 13:
        break;
    default:
        return 0;
    }
    return 0;
}


extern void *func_1505EEF4(s32);

s32 func_1509759C(s32 arg0, s32 arg1, s32 *arg2) {
    struct108 *obj;
    s32 ret;
    struct17 dir;
    struct17 diff;
    f32 angle;
    u8 *ent;
    C3E20Pose *pos;
    f32 dx;
    f32 dz;

    obj = &D_800DBFF0[arg0];
    if (D_80082FA0 < arg0) {
        return 0;
    }
    switch (arg1) {
    case 0x1F:
        return obj->unk390 * 65536.0f;
    case 0x1E:
        return obj->unk37C * 65536.0f;
    case 0x1D:
        return D_800D2DC0[arg2[2]].state;
    case 0x20:
        pos = *(C3E20Pose **)&D_800DC020;
        pos = &pos[arg2[2] & 0xFFF];
        dx = obj->unk2F8 - pos->x;
        dz = obj->unk300 - pos->z;
        return sqrtf(dx * dx + dz * dz);
    case 0x21:
        ent = func_1505EEF4(arg2[2] & 0xFFF);
        if (ent == NULL) {
            return -1;
        }
        angle = obj->unk39C + 1.570796371f;
        dir.unk0 = sinf(angle);
        dir.unk4 = 0.0f;
        dir.unk8 = cosf(angle);
        func_15048F90((struct17 *)&obj->unk2F8, (struct17 *)(ent + 0x14), &diff);
        diff.unk4 = 0.0f;
        if (func_150AD900((f32 *)&dir, (f32 *)&diff) >= 0.0f) {
            ret = -1;
        } else {
            ret = 1;
        }
        return ret;
    default:
        goto block_default;
    }
block_default:
    return 0;
}

void func_15097798(s32 arg0) {
    struct108 *temp_v0;
    struct17 sp20;
    s32 *temp_v1;

    temp_v0 = &D_800DBFF0[arg0];
    if (D_800D2DB4 != 0) {
        func_15048F90((struct17 *)&temp_v0->unk2A4, (struct17 *)&temp_v0->unk2F8, &sp20);
        func_1504917C(&sp20, &sp20);

        temp_v1 = &D_800D2E30[arg0];
        ((struct17 *)&temp_v0->unk2A4)->unk0 += (sp20.unk0 * -2.5f) * (f32)*temp_v1;
        ((struct17 *)&temp_v0->unk2A4)->unk4 += (sp20.unk4 * -2.0f) * (f32)*temp_v1;
        ((struct17 *)&temp_v0->unk2A4)->unk8 += (sp20.unk8 * -2.5f) * (f32)*temp_v1;
        ((struct17 *)&temp_v0->unk2F8)->unk0 += (sp20.unk0 * -2.5f) * (f32)*temp_v1;
        ((struct17 *)&temp_v0->unk2F8)->unk4 += (sp20.unk4 * -2.0f) * (f32)*temp_v1;
        ((struct17 *)&temp_v0->unk2F8)->unk8 += (sp20.unk8 * -2.5f) * (f32)*temp_v1;
    }
}
