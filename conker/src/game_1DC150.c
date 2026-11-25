#include <ultra64.h>
/* functions.h declares func_150ADA20 as returning u8; its asm returns a full word
 * (dsra32 v0), and func_151AF388 spills the result as a word. File-local prototype. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_151D9B8C func_151D9B8C_s32_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
#undef func_151D9B8C
void func_151D9B8C(u8, f32, u8, void *, struct17 *, s16, u8, u8, u8, u8, s32);
u32 func_150ADA20(void);
#include "variables.h"

void func_151DAB58(u8, f32, u8, f32 *, s32, u8, s32);

typedef struct {
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
} Packet_151AECA0;

typedef struct {
    f32 y;
    u8 unk4[0x19];
    u8 unk1D;
    u8 pad1E[6];
} Floor_151AECA0;

void func_15153F18(s16 *, void *, s32, s32, s32);
void func_1504715C(void *, void *);
s32 func_15046C80(f32 *, s32, f32, void *);
void func_15143874(s16, f32, f32 *, f32 *);
void func_15136698(f32, f32, u8, u8, s32, void *, f32 *, s32, s32, s32, s32);

void func_151AECA0(f32 *arg0, u8 arg1, s32 arg2) {
    Floor_151AECA0 floor;
    Packet_151AECA0 packet;
    s32 count;
    f32 probe[3];
    f32 pos[3];
    f32 pos2[3];
    f32 probe2[3];
    f32 unused0;
    f32 unusedA;
    f32 unusedB;
    f32 unused1;
    f32 unused2;
    f32 unused3;
    f32 unused4;
    f32 unused5;
    f32 unused6;
    f32 unused7;
    f32 unused8;
    f32 unused9;
    f32 unused10;
    f32 unused11;
    f32 height;
    u32 r1;
    u32 r2;
    f32 g1;
    f32 g2;
    volatile u32 angle;

    if (arg0 == NULL) {
        return;
    }
    func_1504715C(&floor, arg0);
lbD:
    packet.position[0] = arg0[5];
    packet.position[1] = arg0[6];
    packet.position[2] = arg0[7];
    packet.field14 = 5.079999924f;
    packet.field2C = 0xA;
    packet.field2E = 0;
    packet.field02 = 0xFF;
    packet.field04 = -0x3F;
    packet.field00 = 0;
    packet.field06 = 0x2D;
    packet.field30 = 3;
    packet.field32 = 2;
    packet.field34 = 0x28;
    packet.field36 = 0x14;
    packet.field38 = 0x9B;
    packet.field3A = 0x64;
    packet.field44 = 0x10;
    packet.field46 = 0xF;
    packet.field48 = 0;
    packet.field3C = 0;
    packet.field18 = 3.019999981f;
    packet.field1C = 0.807000041f;
    packet.field20 = 0.7010000348f;
    packet.field24 = 18.0f;
    packet.field28 = 11.80000019f;
    packet.field40 = 0.5f;
    func_15153F18(&packet.field00, packet.position, (s32)&floor, 0xFF, 1);
lbA:
    count = (func_150ADA20() % 6) + 7;
    probe[1] = arg0[6] + 100;
    while (count != 0) {
        angle = func_150ADA20();
        func_15143874((u8)angle, (func_150ADA68() * 59.0f) + 90.0f, &pos[0], &pos[2]);
lbC:
        pos[0] += arg0[5];
        pos[2] += arg0[7];
        probe[0] = pos[0];
        probe[2] = pos[2];
        if (func_15046C80(probe, 0, arg0[6] - 500.0f, &floor) == 0 || floor.unk1D == 3) {
            return;
        }
        pos[1] = floor.y;
        pos[1] += 10.0f;
        if (func_150ADA20() & 1) {
            func_151D9B8C(0, (func_150ADA68() * 4.5f) + 15.0f, (func_150ADA68() * 100.0f) + 155.0f,
                          floor.unk4, (struct17 *)pos, 100, 0, 1, 0, arg1, arg2);
        } else {
            func_151DAB58(0, (func_150ADA68() * 5.900000095f) + 6.099999905f, (func_150ADA68() * 100) + 155,
                          pos, 1, arg1, arg2);
        }
        count--;
    }
lbB:
    pos2[0] = arg0[5];
    pos2[1] = arg0[6];
    pos2[2] = arg0[7];
    probe2[0] = pos2[0];
    probe2[1] = pos2[1] + 100.0f;
    probe2[2] = pos2[2];
    if (func_15046C80(probe2, 0, pos2[1] - 500.0f, &floor) == 0 || floor.unk1D == 3) {
        return;
    }
    pos2[1] = floor.y;
    pos2[1] += 10.0f;
    g1 = func_150ADA68();
    g2 = func_150ADA68();
    r1 = func_150ADA20();
    r2 = func_150ADA20();
    func_15136698(((g1 * 30.0f) + 90.0f) * 0.001000000047f,
                  ((g2 * 103.0f) + 103.0f) * 9.999999747e-06f,
                  (r1 % 101) + 0x9B, (r2 % 101) + 0x64,
                  (func_150ADA20() % 56) + 0x46, floor.unk4, pos2, 0, 1, arg1, arg2);
lbl:;
}
