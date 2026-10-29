#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// bit of a beast
typedef struct Struct357F0 {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
} Struct357F0;

f32 sinf(f32);
f32 cosf(f32);

void func_15008340(Struct357F0 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 *arg4) {
    switch (arg0->unk15 & 3) {
    default:
        {
            *arg1 = arg0->x;
            *arg2 = arg0->z;
            *arg3 = (f32)arg0->y + (f32)arg0->unk8;
            *arg4 = (f32)arg0->y - (f32)arg0->unk8;
        }
        break;
    case 2:
        {
            f32 angle;
            f32 s;
            f32 c;
            f32 dx;
            f32 dz;

            angle = arg0->unk10 * 0.01745329238f;
            s = sinf(angle);
            c = cosf(angle);
            dx = (f32)((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * 1.5259021893e-05f * ((f32)arg0->unk6 + (f32)arg0->unk6) + (f32)-arg0->unk6;
            dz = (f32)((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * 1.5259021893e-05f * ((f32)arg0->unkA + (f32)arg0->unkA) + (f32)-arg0->unkA;
            *arg1 = arg0->x + (dx * c + dz * s);
            *arg2 = arg0->z + (dz * c - dx * s);
            *arg3 = (f32)arg0->y + (f32)arg0->unk8;
            *arg4 = arg0->y;
        }
        break;
    case 0:
        {
            f32 angle;
            f32 s;
            f32 c;
            f32 d;

            angle = (f32)((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * 1.5259021893e-05f;
            angle = (angle + angle) * 3.141592741f;
            s = sinf(angle);
            c = cosf(angle);
            d = (f32)((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * 1.5259021893e-05f * arg0->unk6;
            *arg1 = arg0->x + d * c;
            *arg2 = arg0->z - d * s;
            *arg3 = (f32)arg0->y + (f32)arg0->unk8;
            *arg4 = arg0->y;
        }
        break;
    case 1:
        {
            f32 angle;
            f32 s;
            f32 c;
            f32 d;

            angle = (f32)((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * 1.5259021893e-05f;
            angle = (angle + angle) * 3.141592741f;
            s = sinf(angle);
            c = cosf(angle);
            d = (f32)((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * 1.5259021893e-05f * arg0->unk6;
            *arg1 = arg0->x + d * c;
            *arg2 = arg0->z - d * s;
            *arg3 = (f32)arg0->y + (f32)arg0->unk6;
            *arg4 = (f32)arg0->y - (f32)arg0->unk6;
        }
        break;
    }
}
