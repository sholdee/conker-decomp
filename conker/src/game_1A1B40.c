#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s16 unk0C;
    s16 unk0E;
    s16 x;
    s16 y;
    s16 z;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    u8 pad1C[3];
    s8 unk1F;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s8 unk2A;
    s8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 pad30[4];
    s16 unk34;
} Packet15174690;

extern s32 D_8008CA4C;
extern s32 func_15167D84(void *, s32, s32, s32, s32, s32);

void func_15174690(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    f32 angle;
    f32 x;
    f32 y;
    f32 z;
    s32 rand;
    struct127 *obj;
    Packet15174690 packet;

    obj = &D_800CC2D0[arg0];
    if ((obj->in_water == 0) && (-10000.0f == obj->unk118)) {
        y = obj->y_position + 5.0f;
        angle = obj->unk40 * 0.01745329238f;
        x = obj->x_position;
        z = obj->z_position;
        x -= 10.0f * sinf(angle);
        z -= 10.0f * cosf(angle);
        if (arg1 != 0) {
            rand = func_150ADA20();
            x += (f32)(rand % arg1);
            rand = func_150ADA20() + rand * 0;
            z += (f32)(rand % arg1);
        }
        if (rand) {}
        if (arg3 != 0) {
            arg2 += func_150ADA20() % (u32)arg3;
        }
        packet.unk00 = D_8008CA4C;
        packet.unk04 = arg2;
        packet.unk08 = arg5;
        packet.unk0C = 0;
        packet.unk0E = 0x100;
        packet.x = (s32)x;
        packet.y = (s32)y;
        packet.z = (s32)z;
        packet.unk16 = 0;
        packet.unk18 = 0;
        packet.unk1A = 0;
        packet.unk1F = 5;
        packet.unk20 = arg6;
        packet.unk22 = 0;
        packet.unk24 = arg4;
        packet.unk26 = arg4;
        packet.unk28 = 0x200;
        packet.unk2A = 0;
        packet.unk2B = 0;
        packet.unk2C = 0xFF;
        packet.unk2D = 0xFF;
        packet.unk2E = 0xFF;
        packet.unk2F = 0xFF;
        packet.unk34 = 0;
        func_15167D84(&packet, 0, 0, -1, (u8)arg7, arg8);
    }
}

void func_15174920(void *arg0) {
    s32 v0 = *(u8 *)((u8 *)arg0 + 0x3F);

    if (v0 >= 0xC9) {
        v0 = 0xC8;
    }
    v0 -= D_800BE9E4 * *(s32 *)((u8 *)arg0 + 0x18);
    if (v0 < 0) {
        *(s16 *)((u8 *)arg0 + 0x38) = 0;
        return;
    }
    *(u8 *)((u8 *)arg0 + 0x3F) = v0;
    *(s16 *)((u8 *)arg0 + 0x34) += *(s32 *)((u8 *)arg0 + 0x14);
    *(s16 *)((u8 *)arg0 + 0x36) += (*(s32 *)((u8 *)arg0 + 0x14) << 3) / 7;
}
