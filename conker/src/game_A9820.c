#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_1507C3E0(struct127 *obj, s16 *height, s16 *width, s16 *depth);

/* Refreshes the cached height/width/depth (struct126 unk114-unk118) of every object that has one. */
void func_1507C370(void) {
    s32 i;
    struct127 *obj = D_800CC2D0;

    for (i = 0; i < D_8008FD8C; i++, obj++) {
        struct126 *ext = obj->unk31C;
        if (ext != NULL) {
            func_1507C3E0(obj, (s16 *)&ext->unk114, (s16 *)&ext->unk116, (s16 *)&ext->unk118);
        }
    }
}

/* func_1507C3E0: the original has compiled-out debug code after the size selection (ra and
 * $f20 are saved and a1-a3 homed although no call survives). The dbg* names below are
 * stand-ins that are never referenced by the emitted code; the dead blocks only steer
 * uopt's live-range splitting (a: split, in $f18 across the selection; b: $f20; c: stack). */
void dbg_func0(void); void dbgf(f32); void dbgp(f32 *); void dbgi(s32); void dbgu(); s32 dbgr(void); f32 dbgrf(void); f32 dbgff(f32); void dbg3(f32,f32,f32); void dbgv(); extern f32 dbgx[];
void func_1507C3E0(struct127 *arg0, s16 *arg1, s16 *arg2, s16 *arg3) {
    f32 a;
    f32 c;
    f32 b;
    u8 *p;
    s32 flag;
    s32 pb[9];
    f32 zero;
    f32 x1;
    f32 x2;
    f32 x3;
    f32 x4;
    f32 x5;
    s32 dbg;
    s32 pa[8];

    dbg = 0;
    flag = 1;
    zero = 0.0f;
    if (arg0->interaction_state == 0) {
        c = b = 60.0f;
        a = 180.0f;
    } else if (arg0->interaction_state == 0x2D) {
        a = *(f32 *)((u8 *)arg0->camera + 0x960);
        c = b = *(f32 *)((u8 *)arg0->camera + 0x95C);
    } else if (arg0->interaction_state == 4) {
        a = arg0->unkE6 * 6.0f;
        c = b = arg0->unkE4 * 3.0f;
    } else if (arg0->interaction_state == 0x2C) {
        c = b = 80.0f;
        a = 160.0f;
    } else if (arg0->unk5 == 5) {
        a = arg0->unkE6 * 2.0f;
        c = b = arg0->unkE4;
    } else {
        switch (arg0->id) {
        case 0: case 1: case 2: case 3: case 4:
        case 59:
        case 117: case 128: case 130: case 136: case 144: case 150: case 152:
        case 156: case 157: case 159: case 160: case 176: case 177: case 178: case 180:
            p = (u8 *)arg0->unk31C;
            if (p != NULL && *(s32 *)(p + 4) != 0) {
                c = b = 60.0f;
                a = 80.0f;
            } else if (p != NULL && *(u8 *)(p + 0x17) != 0) {
                c = b = 70.0f;
                a = 300.0f;
            } else if (arg0->in_water) {
                a = 180.0f;
                c = 90.0; b = 90.0f;
            } else {
                c = 60.0f;
                b = 60.0f;
                a = 180.0f;
                if (p != NULL) {
                    if ((arg0->camera != NULL && *(s16 *)((u8 *)arg0->camera + 0x73C) != 0) ||
                        *(u8 *)(p + 0x197) != 0 || *(u8 *)(p + 0x1B3) != 0) {
                        c *= 2.0f; b *= 2.0f;
                    }
                    a += (u32)*(u16 *)(p + 0x1A6);
                }
            }
            if (p != NULL && (*(u8 *)(p + 0x4E) & 0xF) == 1 && *(u8 *)(p + 0x4F) == 0 &&
                arg0->xz_velocity < 40.0f) {
                c = b = 110.0f;
                a = 500.0f;
            }
            break;
        case 37: case 46:
            a = 70.0f;
            c = 90.0; b = 90.0f;
            break;
        case 54:
            a = 257.0f;
            c = 90.0; b = 90.0f;
            break;
        case 154:
            a = 60.0f;
            c = b = 45.0f;
            flag = 0;
            break;
        case 172:
            c = b = 30.0f;
            a = 90.0f;
            flag = 0;
            break;
        case 137: case 186:
            a = 60.0f;
            c = b = 30.0f;
            flag = 0;
            break;
        case 83:
            if (D_800BE616 != 0) {
                c = b = 150.0f;
                a = 300.0f;
            } else {
                a = 250.0f;
                c = 90.0; b = 90.0f;
            }
            break;
        default:
            flag = 0;
            a = arg0->unkE6 * 2.0f;
            c = b = arg0->unkE4;
            break;
        }
        if (arg0->unk13C > 100 && D_800CC2D0[arg0->unk13C - 100].id == 0x13) {
            b += 120.0f * arg0->xz_scale;
            a += 100.0f * arg0->y_scale;
        }
        if (flag) {
            f32 sx;
            sx = arg0->xz_scale;
            a *= arg0->y_scale;
            c *= sx;
            b *= sx;
            goto end;
        }
    }
end:
    if (!dbg) { } else {
        dbg3(a, b, b);
        x1 = dbgx[0x10]; x2 = dbgx[0x11]; x3 = dbgx[0x12]; x4 = dbgx[0x13]; x5 = dbgx[0x14];
        dbgx[0x15] = x1 * x2 * x3 * x4 * x5; dbgx[0x16] = x1 + x2 + x3 + x4 + x5;
        dbgp(&zero); dbgp(&c); dbgu(); dbgu(); 
    }
    if (arg1 != NULL) {
        *arg1 = a;
    }
    if (arg2 != NULL) {
        *arg2 = c;
    }
    if (arg3 != NULL) {
        *arg3 = b;
    }
    if (dbg) { dbgu(); dbgu(); dbgu(); dbgu(); dbgu(); dbgu(); dbgu(); dbgu();  }
}
