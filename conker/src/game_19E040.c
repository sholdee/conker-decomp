#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_proto
#define func_15048A40 func_15048A40_void_proto
#include "functions.h"
#undef func_150ADA20
#undef func_15048A40
#include "variables.h"

extern f32 D_800A6FC0;
extern f32 D_800A6FC4;
void func_151717FC(struct127 *, f32, f32, f32, f32, f32, s32, s32, s32, s32, f32, s32, s32);

typedef struct Game70F4CPacket {
    f32 position[3];
    f32 rotation[3];
    f32 velocity[3];
    f32 random[3];
    f32 scale[3];
    s32 life;
    s32 texture;
    u32 pad44;
    f32 color[3];
    u8 flags[10];
    u8 pad5E[2];
} Game70F4CPacket;

typedef struct {
    u8 pad0[0x10];
    s16 x;
    s16 y;
    s16 z;
    u8 pad16[0x6E - 0x16];
    u8 unk6E;
} Obj15170B90;

extern f32 D_800A6E90[6][3];
s32 func_1518C900(s32);
void func_15168BE4(void *, u8, s32);
u32 func_150ADA20();
void func_151718F0(f32, f32, f32, f32, s32, f32, s32, s32, s32, s32);

void func_15170B90(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 i;
    Game70F4CPacket packet;
    f32 x;
    f32 y;
    f32 z;
    s32 id;
    s32 texture;
    s32 r;

    x = ((Obj15170B90 *)arg0)->x;
    y = ((Obj15170B90 *)arg0)->y;
    z = ((Obj15170B90 *)arg0)->z;
    packet.rotation[0] = 0.0f;
    packet.rotation[1] = 0.0f;
    packet.rotation[2] = 0.0f;
    packet.scale[0] = 1.0f;
    packet.scale[1] = 1.0f;
    packet.scale[2] = 1.0f;
    packet.life = 0x3C;
    packet.color[0] = 0.3000000119f;
    packet.color[1] = -0.6999999881f;
    packet.flags[0] = 2;
    packet.color[2] = -0.200000003f;
    packet.flags[1] = 0xFF;
    packet.flags[2] = 0;
    packet.flags[3] = 0;
    packet.flags[4] = 0;
    packet.flags[5] = 0x3C;
    packet.flags[7] = arg2;
    packet.flags[8] = 0;
    packet.flags[9] = 0;
    for (i = 0; i < 6; i++) {
        if (arg3 & 2) {
            const u8 ids[6] = { 7, 2, 3, 8, 9, 10 };
            id = ids[i];
        } else if (arg3 & 1) {
            id = arg1 + i;
        } else {
            id = arg1;
        }
        texture = func_1518C900(id);
        packet.position[0] = D_800A6E90[i][0] + x;
        packet.position[1] = D_800A6E90[i][1] + y;
        packet.position[2] = D_800A6E90[i][2] + z;
        packet.flags[6] = id;
        packet.velocity[0] = D_800A6E90[i][0] * 0.07999999821f;
        packet.velocity[1] = fabsf(D_800A6E90[i][1]) * 0.07999999821f + 7.0f;
        packet.velocity[2] = D_800A6E90[i][2] * 0.07999999821f;
        r = func_150ADA20();
        packet.velocity[0] += (f32)(r >> 16) * 0.00006103515625f;
        r = func_150ADA20();
        packet.velocity[1] += (f32)(r >> 16) * 0.00006103515625f;
        r = func_150ADA20();
        packet.velocity[2] += (f32)(r >> 16) * 0.00006103515625f;
        packet.random[0] = (f32)((s32)func_150ADA20() >> 16) * 0.0000762939453125f;
        packet.random[1] = (f32)((s32)func_150ADA20() >> 16) * 0.0000762939453125f;
        packet.random[2] = (f32)((s32)func_150ADA20() >> 16) * 0.0000762939453125f;
        packet.texture = texture;
        func_15168BE4(&packet, arg4, arg5);
    }
    func_151718F0(x, y - 80.0f, z, 70.0f, 500, 0.00457763671875f, 0, 10, arg4, arg5);
    ((Obj15170B90 *)arg0)->unk6E = 1;
}

extern s32 D_800BE9F0;
void func_15170B90(s32, s32, s32, s32, u8, s32);

void func_15170EC4(s32 arg0, u8 arg1, s32 arg2) {
    switch (D_800BE9F0) {
    case 2:
        func_15170B90(arg0, 1, 1, 1, arg1, arg2);
        break;
    case 0x10:
        func_15170B90(arg0, 0xA9, 8, 0, arg1, arg2);
        break;
    }
}

typedef struct {
    u16 id;
    f32 pos[3];
    f32 rot[3];
} Entry15170F4C;

extern Entry15170F4C D_800A6ED8[6];
extern f32 func_150ADA68(void);

void func_15170F4C(s32 arg0, u8 arg1, s32 arg2) {
    s32 i;
    Game70F4CPacket packet;
    Entry15170F4C *e;
    f32 x;
    f32 y;
    f32 z;
    s32 count;
    s32 texture;

    count = 6;
    e = D_800A6ED8;
    packet.flags[8] = 2;
    packet.flags[9] = 1;
    packet.flags[7] = 6;
    packet.flags[5] = 0;
    x = ((Obj15170B90 *)arg0)->x;
    y = ((Obj15170B90 *)arg0)->y;
    z = ((Obj15170B90 *)arg0)->z;
    packet.rotation[0] = 0.0f;
    packet.rotation[1] = 0.0f;
    packet.rotation[2] = 0.0f;
    packet.scale[0] = 2.0f;
    packet.scale[1] = 2.0f;
    packet.scale[2] = 2.0f;
    packet.life = 0x3C;
    packet.flags[0] = 2;
    packet.flags[1] = 0xFF;
    packet.flags[2] = 0;
    packet.flags[3] = 0;
    packet.flags[4] = 0;
    packet.color[0] = 0.3000000119f;
    packet.color[1] = -0.6999999881f;
    packet.color[2] = -0.200000003f;
    for (i = 0; i < count; i++) {
        texture = func_1518C900(e->id);
        packet.flags[6] = e->id;
        packet.position[0] = e->pos[0] + x;
        packet.position[1] = e->pos[1] + y;
        packet.position[2] = e->pos[2] + z;
        packet.velocity[0] = e->pos[0] * 0.09000000358f;
        packet.velocity[1] = fabsf(e->pos[1]) * 0.2300000042f;
        packet.velocity[2] = e->pos[2] * 0.09000000358f;
        packet.rotation[0] = e->rot[0];
        packet.rotation[1] = e->rot[1];
        packet.rotation[2] = e->rot[2];
        packet.random[0] = packet.rotation[0] * 0.1000000015f;
        packet.random[1] = func_150ADA68() * 4.0f - 2.0f;
        packet.random[2] = func_150ADA68() * 10.0f - 5.0f;
        packet.texture = texture;
        func_15168BE4(&packet, arg1, arg2);
        e++;
    }
    ((Obj15170B90 *)arg0)->unk6E = 1;
}

extern void func_150C3D5C(struct127 *);

void func_151711C4(struct127 *arg0) {
    if (((u8 *)arg0)[4] == 0x33) {
        func_150C3D5C(arg0);
    }
    func_15060F28(arg0, 1);
}


s32 func_1518C900(s32);
void func_15168BE4(void *, u8, s32);
u32 func_150ADA20();

void func_15171200(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7,
                   f32 arg8, s32 arg9, s32 arg10, f32 arg11, u8 arg12, s32 arg13) {
    s32 i;
    Game70F4CPacket packet;
    f32 range;
    s32 texture;
    f32 size;
    f32 driftX;
    f32 lift;
    f32 driftZ;
    f32 spread;

    range = arg7 - arg6;
    packet.life = arg10;
    packet.rotation[0] = 0.0f;
    packet.rotation[1] = 0.0f;
    packet.rotation[2] = 0.0f;
    packet.color[0] = arg11 * -0.75f;
    packet.color[1] = arg11;
    packet.flags[0] = 2;
    packet.flags[1] = 255;
    packet.flags[2] = 0;
    packet.flags[3] = 0;
    packet.flags[4] = 0;
    packet.flags[6] = arg1;
    packet.flags[7] = arg9;
    packet.flags[8] = 1;
    packet.flags[9] = 1;
    arg8 *= 1.5258789e-05f;
    packet.color[2] = -0.2f;
    spread = arg5 * 6.1035157e-06f;
    arg5 *= 0.8f;
    for (i = 0; i < arg0; i++) {
        packet.position[0] = (f32)((s32)func_150ADA20() >> 16) * arg8 + arg2;
        packet.position[1] = (f32)((s32)func_150ADA20() >> 16) * arg8 + arg3;
        packet.position[2] = (f32)((s32)func_150ADA20() >> 16) * arg8 + arg4;
        texture = func_1518C900(arg1);
        packet.texture = texture;
        driftX = (f32)((s32)func_150ADA20() >> 16) * 0.00022888184f;
        packet.velocity[0] = driftX;
        lift = (f32)(func_150ADA20() & 0xFFFF) * spread + arg5;
        packet.velocity[1] = lift;
        driftZ = (f32)((s32)func_150ADA20() >> 16) * 0.00022888184f;
        packet.velocity[2] = driftZ;
        packet.random[0] = (f32)((s32)func_150ADA20() >> 16) * 0.00015258789f;
        packet.random[1] = (f32)((s32)func_150ADA20() >> 16) * 0.00015258789f;
        packet.random[2] = (f32)((s32)func_150ADA20() >> 16) * 0.00015258789f;
        packet.scale[0] = (f32)(func_150ADA20() & 0xFFFF) * 1.5258789e-05f * range + arg6;
        packet.scale[1] = (f32)(func_150ADA20() & 0xFFFF) * 1.5258789e-05f * range + arg6;
        size = (f32)(func_150ADA20() & 0xFFFF) * 1.5258789e-05f * range + arg6;
        packet.flags[5] = (packet.scale[1] + packet.scale[0] + size) * 6.666667f + 20.0f;
        packet.scale[2] = size;
        func_15168BE4(&packet, arg12, arg13);
    }
}

void func_15171600(struct127 *arg0, s32 arg1, s32 arg2) {
    u8 temp;

    switch (((u8 *)arg0)[0x72]) {
    case 0xEB:
        temp = arg1;
        func_151717FC(arg0, 400.0f, 500.0f, 0.0f, D_800A6FC0, 0.0f, 2, 5, 0x14, 0x14, 1.0f, temp, arg2);
        break;
    case 0xF6:
        temp = arg1;
        func_151717FC(arg0, 200.0f, 400.0f, -1800.0f, -1000.0f, -1200.0f, 2, 5, 4, 0xA, 1.0f, temp, arg2);
        break;
    case 0xF8:
        temp = arg1;
        func_151717FC(arg0, 100.0f, 160.0f, -2500.0f, D_800A6FC4, 0.0f, 2, 5, 4, 4, 0.5f, temp, arg2);
        break;
    case 0xF9:
        temp = arg1;
        func_151717FC(arg0, 200.0f, 100.0f, 100.0f, -300.0f, 0.0f, 1, 5, 4, 4, 1.0f, temp, arg2);
        break;
    }
}

typedef struct {
    u8 pad0[0x10];
    s16 x;
    s16 y;
    s16 z;
} Pos151717FC;

void func_151700D8(f32 *, f32, f32, s32, f32, s32, s32, s32, s32, s32);

void func_151717FC(struct127 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7,
                   s32 arg8, s32 arg9, f32 argA, s32 argB, s32 argC) {
    f32 verts[4][3];
    f32 cx;
    f32 cy;
    f32 cz;

    cx = ((Pos151717FC *)arg0)->x + arg3;
    cy = ((Pos151717FC *)arg0)->y + arg4;
    cz = ((Pos151717FC *)arg0)->z + arg5;
    verts[0][0] = arg1 + cx;
    verts[0][1] = cy;
    verts[0][2] = arg2 + cz;
    verts[1][0] = cx - arg1;
    verts[1][1] = cy;
    verts[1][2] = arg2 + cz;
    verts[2][0] = cx - arg1;
    verts[2][1] = cy;
    verts[2][2] = cz - arg2;
    verts[3][0] = arg1 + cx;
    verts[3][1] = cy;
    verts[3][2] = cz - arg2;
    func_151700D8((f32 *)verts, cx, cz, arg7, argA, arg6, arg8, arg9, (u8)argB, argC);
}

f32 func_15048A40(u8);
f32 func_150489B0(u8);
typedef struct {
    u8 v[3];
} Color151718F0;
extern Color151718F0 D_8008CC20;
void func_1516D4E8(s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_151718F0(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4, f32 arg5, s32 arg6, s32 arg7,
                   s32 arg8, s32 arg9) {
    s32 i;
    s32 angle;
    u8 a;
    f32 sn;
    f32 cs;
    f32 z;
    s32 c;
    u8 chi;
    u8 clo;
    f32 scale;
    s32 spd;
    s32 s;
    u32 r;
    f32 x;
    u8 shi;
    u8 slo;
    s32 step;
    u32 r1;
    u32 r2;
    s32 pad;
    const u8 colors[1][3] = { { 0x45, 0x53, 0x68 } };

    if (arg7 != 0) {
        angle = 0;
        step = 0xFFFF / arg7;
        for (i = 0; i < arg7; i++) {
            a = angle >> 8;
            angle |= 0;
            sn = func_15048A40(a);
            a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0;
            cs = func_150489B0(a);
            z = arg3 * cs + arg2;
            c = 200.0f * cs;
            chi = c >> 8;
            clo = c;
            r1 = func_150ADA20();
            scale = (f32)(r1 & 0xFFFF) * 0.0000152587890625f + 100.0f;
            r2 = func_150ADA20();
            spd = (f32)(r2 & 0xFFFF) * arg5 + (f32)arg4;
            r = func_150ADA20();
            s = 200.0f * sn;
            x = arg3 * sn + arg0;
            shi = s >> 8;
            slo = s;
            func_1516D4E8(x, arg1, z, 0xD, 0, colors[arg6][0], colors[arg6][1],
                          colors[arg6][2], 0, 0, 0, 0, 2, shi, slo, chi, clo, 0, 0, 5, 0, 0,
                          0, (s16)spd, (s16)spd, (r & 0xF) + 0xA, scale, 0, 0, (u8)arg8, arg9);
            angle += step;
        }
    }
}

extern void func_150AEEB0(void *);
extern void func_150BD740(void *, u8, s32);
extern void func_150CDBB0(void *, u8, s32);

void func_15171BF4(void *arg0, u8 arg1) {
    switch (((u8 *)arg0)[4]) {
    case 0x13:
    case 0x23:
        func_150AEEB0(arg0);
        func_10010154(0x69, arg0, 0x7D00, 0xC8, 0x7D0);
        break;
    case 0x1E:
        func_150BD740(arg0, arg1, 1);
        break;
    case 0x54:
        func_150CDBB0(arg0, arg1, 1);
        break;
    }
}
