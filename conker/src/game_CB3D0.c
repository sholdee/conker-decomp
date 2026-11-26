#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Five spring-eased script values (func_150495B0 / func_15049688). */
static f32 D_800D2FC0[5]; // velocity
static f32 D_800D2FD8[5]; // value
static u8 D_800D2FEC[5];  // set by func_1509DF20

// On a type-1 record (only while D_800D3840 == 3), sets both float slots of entry `index` to the
// 16.16 fixed-point `value` and flags the entry.
void func_1509DF20(s32 arg0, ScriptCmd *record) {
    if (record->arg.w == 1 && D_800D3840 == 3) {
        D_800D2FC0[record->a[0].w] = record->a[1].w * (1.0f / 65536.0f);
        D_800D2FD8[record->a[0].w] = record->a[1].w * (1.0f / 65536.0f);
        D_800D2FEC[record->a[0].w] = 1;
    }
}

// Empty handler.
void func_1509DFB4(s32 arg0, s32 arg1) {
}

// Runs command `cmd` with up to four integer params; most commands forward to a helper.
s32 func_1509DFC4(s32 arg0, s32 cmd, ScriptCmd *args) {
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
        func_15042D94(150, 86, 1, "%d", (args->a[0].w + 59) / 60);
        D_8008FDBC |= 0x80;
        break;
    case 2:
        D_800D2450 = args->a[0].w;
        break;
    case 3:
        func_15093818(args->a[0].w);
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
        return func_1508C194(args->a[0].w);
    case 9:
        return func_1508CAD8(args->a[0].w, args->a[1].w, args->a[2].w, args->a[3].w, 0, 0);
    case 10:
        return func_1508DA1C();
    case 11:
        return func_1508DC24();
    case 12:
        return func_1508DAEC(args->a[0].w, args->a[1].w);
    case 13:
        return func_1508B9BC();
    case 14:
        return func_1508D850(args->a[0].w);
    case 15:
        return func_1508E89C(args->a[0].w, args->a[1].w);
    case 16:
        id = func_15086D48(args->a[0].w);
        if (id != 255) {
            func_15086CBC(id, &x, &y, &z);
            func_1508B20C(x, y, z, args->a[1].w);
            return 0;
        }
        break;
    case 17:
        func_150870D0(args->a[0].w, args->a[1].w, args->a[2].w);
        return 0;
    case 18:
        func_15087350(args->a[0].w, args->a[1].w, 0.1f);
        return 0;
    case 19:
        func_15087DCC(args->a[0].w, args->a[1].w);
        return 0;
    case 20:
        func_15087FEC(args->a[0].w, args->a[1].w);
        return 0;
    case 21:
        func_15087FC4(args->a[0].w, args->a[1].w);
        return 0;
    case 22:
        return func_150880F8(args->a[0].w, args->a[1].w);
    case 23:
        func_1508B1D4(args->a[0].w);
        return 0;
    case 25:
        // matching: a function-scope scale plus a nested block makes IDO hoist the constant load
        scale = 1.0f / 65536.0f;
        {
        Camera *cam = camera_global_Cameras;
        func_150495B0(&D_800D2FD8[args->a[0].w], args->a[1].w * scale, &D_800D2FC0[args->a[0].w],
                      args->a[2].w * scale, args->a[3].w * scale, cam->SpringTime);
        return D_800D2FD8[args->a[0].w] * 65536.0f;
        }
    case 26:
        // matching: a function-scope scale plus a nested block makes IDO hoist the constant load
        scale = 1.0f / 65536.0f;
        {
        Camera *cam = camera_global_Cameras;
        func_15049688(&D_800D2FD8[args->a[0].w], args->a[1].w * scale, &D_800D2FC0[args->a[0].w],
                      args->a[2].w * scale, args->a[3].w * scale, cam->SpringTime);
        return D_800D2FD8[args->a[0].w] * 65536.0f;
        }
    case 24:
        obj = func_15083E90(args->a[0].w);
        if (obj != NULL) {
            func_15060F28(obj, 0);
        }
        break;
    }
    return 0;
}


// Answers query `cmd` with up to four integer params and returns the result as an s32.
s32 func_1509E3DC(s32 arg0, s32 cmd, ScriptCmd *args) {
    Actor *obj;
    // matching: cases 1 and 18 must share these two locals for IDO's register allocation
    s32 x;
    s32 z;

    switch (cmd) {
    case 0:
        return func_150CF040(args->a[0].w, args->a[1].w);
    case 1:
        x = args->a[0].w - args->a[2].w;
        z = args->a[1].w - args->a[3].w;
        return sqrtf(x * x + z * z);
    case 2:
        return func_1508CA88();
    case 3:
        return func_1508C9CC();
    case 4:
        return func_1508C5B8(args->a[0].w, args->a[1].w);
    case 5:
        return D_800E0BD0;
    case 6:
        return func_15089F9C(args->a[0].w);
    case 7:
        return func_1508E6D0(args->a[0].w);
    case 8:
        return 0;
    case 9:
        return func_150881CC(args->a[0].w);
    case 10:
        return func_15088218(args->a[0].w);
    case 11:
        return func_15088270(args->a[0].w);
    case 12:
        return func_150882B0(args->a[0].w);
    case 13:
        return func_150882E4(args->a[0].w, args->a[1].w);
    case 14:
        return func_150887F8();
    case 15:
        return D_8008FDBC & 0x40;
    case 16:
        return D_800E0BE0[args->a[0].w];
    case 17:
        return func_1508B194(args->a[0].w);
    case 20:
    case 21:
        return D_800D2FD8[args->a[0].w] * 65536.0f;
    case 18:
        obj = func_15083E90(args->a[0].w);
        if (obj != NULL) {
            x = obj->xpos;
            z = obj->zpos;
            return (x << 16) | (z & 0xFFFF);
        }
        return 0;
    case 19:
        obj = func_15083E90(args->a[0].w);
        if (obj != NULL) {
            return (s16)obj->ypos;
        }
        return 0;
    }
// matching: this label stops IDO folding the default return into the bounds-check delay slot
end:
    return 0;
}
