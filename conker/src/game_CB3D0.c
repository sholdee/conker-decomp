#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct CB3D0_arg1 {
    s32 unk0;
    s32 type;
    s32 index;
    s32 value; // 16.16 fixed point
};

extern f32 D_800D2FC0[];
extern f32 D_800D2FD8[];
extern u8 D_800D2FEC[];

// On a type-1 record (only while D_800D3840 == 3), sets both float slots of entry `index` to the
// 16.16 fixed-point `value` and flags the entry.
void func_1509DF20(s32 arg0, struct CB3D0_arg1 *record) {
    if (record->type == 1 && D_800D3840 == 3) {
        D_800D2FC0[record->index] = record->value * (1.0f / 65536.0f);
        D_800D2FD8[record->index] = record->value * (1.0f / 65536.0f);
        D_800D2FEC[record->index] = 1;
    }
}

// Empty handler.
void func_1509DFB4(s32 arg0, s32 arg1) {
}

struct CB3D0_arg2 {
    s32 unk0;
    s32 unk4;
    s32 params[4];
};

// Runs command `cmd` with up to four integer params; most commands forward to a helper.
s32 func_1509DFC4(s32 arg0, s32 cmd, struct CB3D0_arg2 *args) {
    s32 id;
    f32 x;
    f32 y;
    f32 z;
    Actor *obj;
    f32 scale;

    switch (cmd) {
    case 0:
        func_15181E18(0);
        return 0;
    case 1:
        func_1504332C(255, 255, 255, 255);
        func_15042D94(150, 86, 1, "%d", (args->params[0] + 59) / 60);
        D_8008FDBC |= 0x80;
        break;
    case 2:
        D_800D2450 = args->params[0];
        break;
    case 3:
        func_15093818(args->params[0]);
        return 0;
    case 4:
        return func_15089BC0();
    case 5:
        return func_1508A1BC();
    case 6:
        return func_1508A6FC();
    case 7:
        return func_1508BF14();
    case 8:
        return func_1508C194(args->params[0]);
    case 9:
        return func_1508CAD8(args->params[0], args->params[1], args->params[2], args->params[3], 0, 0);
    case 10:
        return func_1508DA1C();
    case 11:
        return func_1508DC24();
    case 12:
        return func_1508DAEC(args->params[0], args->params[1]);
    case 13:
        return func_1508B9BC();
    case 14:
        return func_1508D850(args->params[0]);
    case 15:
        return func_1508E89C(args->params[0], args->params[1]);
    case 16:
        id = func_15086D48(args->params[0]);
        if (id != 255) {
            func_15086CBC(id, &x, &y, &z);
            func_1508B20C(x, y, z, args->params[1]);
            return 0;
        }
        break;
    case 17:
        func_150870D0(args->params[0], args->params[1], args->params[2]);
        return 0;
    case 18:
        func_15087350(args->params[0], args->params[1], 0.1f);
        return 0;
    case 19:
        func_15087DCC(args->params[0], args->params[1]);
        return 0;
    case 20:
        func_15087FEC(args->params[0], args->params[1]);
        return 0;
    case 21:
        func_15087FC4(args->params[0], args->params[1]);
        return 0;
    case 22:
        return func_150880F8(args->params[0], args->params[1]);
    case 23:
        func_1508B1D4(args->params[0]);
        return 0;
    case 25:
        // matching: a function-scope scale plus a nested block makes IDO hoist the constant load
        scale = 1.0f / 65536.0f;
        {
        Camera *cam = camera_global_Cameras;
        func_150495B0(&D_800D2FD8[args->params[0]], args->params[1] * scale, &D_800D2FC0[args->params[0]],
                      args->params[2] * scale, args->params[3] * scale, cam->SpringTime);
        return D_800D2FD8[args->params[0]] * 65536.0f;
        }
    case 26:
        // matching: a function-scope scale plus a nested block makes IDO hoist the constant load
        scale = 1.0f / 65536.0f;
        {
        Camera *cam = camera_global_Cameras;
        func_15049688(&D_800D2FD8[args->params[0]], args->params[1] * scale, &D_800D2FC0[args->params[0]],
                      args->params[2] * scale, args->params[3] * scale, cam->SpringTime);
        return D_800D2FD8[args->params[0]] * 65536.0f;
        }
    case 24:
        obj = func_15083E90((u8)args->params[0]);
        if (obj != NULL) {
            func_15060F28(obj, 0);
        }
        break;
    }
    return 0;
}


// Answers query `cmd` with up to four integer params and returns the result as an s32.
s32 func_1509E3DC(s32 arg0, s32 cmd, struct CB3D0_arg2 *args) {
    Actor *obj;
    // matching: cases 1 and 18 must share these two locals for IDO's register allocation
    s32 x;
    s32 z;

    switch (cmd) {
    case 0:
        return func_150CF040(args->params[0], args->params[1]);
    case 1:
        x = args->params[0] - args->params[2];
        z = args->params[1] - args->params[3];
        return sqrtf(x * x + z * z);
    case 2:
        return func_1508CA88();
    case 3:
        return func_1508C9CC();
    case 4:
        return func_1508C5B8(args->params[0], args->params[1]);
    case 5:
        return D_800E0BD0;
    case 6:
        return func_15089F9C(args->params[0]);
    case 7:
        return func_1508E6D0(args->params[0]);
    case 8:
        return 0;
    case 9:
        return func_150881CC(args->params[0]);
    case 10:
        return func_15088218(args->params[0]);
    case 11:
        return func_15088270(args->params[0]);
    case 12:
        return func_150882B0(args->params[0]);
    case 13:
        return func_150882E4(args->params[0], args->params[1]);
    case 14:
        return func_150887F8();
    case 15:
        return D_8008FDBC & 0x40;
    case 16:
        return D_800E0BE0[args->params[0]];
    case 17:
        return func_1508B194(args->params[0]);
    case 20:
    case 21:
        return D_800D2FD8[args->params[0]] * 65536.0f;
    case 18:
        obj = func_15083E90((u8)args->params[0]);
        if (obj != NULL) {
            x = obj->xpos;
            z = obj->zpos;
            return (x << 16) | (z & 0xFFFF);
        }
        return 0;
    case 19:
        obj = func_15083E90((u8)args->params[0]);
        if (obj != NULL) {
            return (s16)obj->ypos;
        }
        return 0;
    }
// matching: this label stops IDO folding the default return into the bounds-check delay slot
end:
    return 0;
}
