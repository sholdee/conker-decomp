#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    u8 pad0[0x1D4];
    s32 unk1D4;
} Struct150EBEC0;

extern f32 D_800A1500[3];
extern f32 D_800A150C[3];
extern f32 D_800A1518[][3];
extern f32 D_800A1530[][3];
extern u8 D_800A1548[];
extern void func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, s32, f32 *, f32 *, f32 *, s32, s32, s32, f32);
extern void func_15143874(s16, f32, f32 *, f32 *);
extern void func_15102B38(Struct150EBEC0 *, u8, f32 *, f32 *, f32 *, s32, s32, f32, struct17 *, s32, s32, s32, u8, s32);
extern s32 func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);
s32 func_151C229C(struct17 *arg0, struct17 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, Struct150EBEC0 *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26);

s32 func_150EBEC0(Struct150EBEC0 *arg0, u8 arg1, u8 arg2, s32 arg3) {
    s32 ret;
    struct17 pos;
    struct17 dir;
    struct17 pos2;
    struct17 *target;
    struct17 newdir;
    s32 srcs[2];
    s32 dsts[2];
    s32 srcs1[1];
    s32 dsts1[1];
    struct17 hit;
    f32 off[2];
    Header hdr;
    Header2 hdr2;
    f32 sp90[2];
    f32 rand1;
    s32 sp88;
    f32 rand0;
    s32 sp80;

    if (arg0 == NULL) {
        return 0;
    }
    if (arg1 >= 2) {
        return 0;
    }
    if (arg0->unk1D4 == 0) {
        return 0;
    }
    srcs[0] = (s32)D_800A1500;
    srcs[1] = (s32)D_800A150C;
    dsts[0] = (s32)&pos;
    dsts[1] = (s32)&dir;
    func_15145EA4(srcs, dsts, arg0->unk1D4 + 0x80, 2);
    dir.unk0 -= pos.unk0;
    dir.unk4 -= pos.unk4;
    dir.unk8 -= pos.unk8;
    {
        void func_15145EA4(s32 *, s32 *, s32, s32);

        srcs1[0] = (s32)D_800A1518[arg1];
        dsts1[0] = (s32)&pos2;
        func_15145EA4(srcs1, dsts1, (D_800A1548[arg1] << 6) + arg0->unk1D4, 1);
    }
    if (func_150AC9C0(pos.unk0, pos.unk4, pos.unk8, dir.unk0, dir.unk4, dir.unk8, 0, 0, &hit.unk0, &hit.unk4, &hit.unk8, 0, 0, 0, 0.0f) != 0) {
        sp88 = func_150ADA20();
        rand0 = func_150ADA68() * 80.0f;
        func_15143874((s16)(sp88 & 0xFF), rand0, &off[0], &off[1]);
        {
            void func_15143874(s16, f32, f32 *, f32 *);

            hit.unk0 += off[0];
            hit.unk8 += off[1];
            target = &newdir;
            newdir.unk0 = hit.unk0 - pos2.unk0;
            newdir.unk4 = hit.unk4 - pos2.unk4;
            newdir.unk8 = hit.unk8 - pos2.unk8;
        }
    } else {
        target = &dir;
    }
    {
        f32 func_150ADA68(void);

        rand0 = func_150ADA68();
        rand1 = func_150ADA68();
        func_150ADA20();
        ret = func_151C229C(&pos2, target, 0, 0, 0, 0, 130.0f, 0.007692307699f, rand0 * 18.0f + 29.0f,
                            rand1 * 400.0f + 262.0f, 100.0f, 0xFF, arg0, 1, 1, 0, 0, 0, 1, 3, 0x1A, 0.0f,
                            0xFF, -1, 0, arg2, arg3);
    }
    hdr.unk0 = 3;
    hdr.unk1 = -1;
    hdr.unk2 = (func_150ADA20() % 3U) + 3;
    hdr.unk4 = 0;
    hdr2.unk0 = pos2.unk0;
    hdr2.unk4 = pos2.unk4;
    hdr2.unk8 = pos2.unk8;
    sp88 = (func_150ADA20() % 13U) + 0x5A;
    func_151602C0(&hdr, &hdr2, sp88, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,
                  arg2, arg3);
    sp90[1] = (func_150ADA68() * 2586.0f + 1758.0f) * 0.001000000047f;
    sp90[0] = (func_150ADA68() * 1357.0f + 968.0f) * 0.1000000015f;
    sp80 = func_150ADA20();
    sp88 = func_150ADA20();
    rand0 = func_150ADA68() * 1077.0f + 4000.0f;
    func_15102B38(arg0, D_800A1548[arg1], D_800A1518[arg1], D_800A1530[arg1], sp90,
                  (sp80 % 6U) + 6, (sp88 % 52U) + 0xC1, rand0, &pos2, 0xFF, 0, -1,
                  arg2, arg3);
    return ret;
}

typedef struct {
    s32 unk0;
    u8 unk4;
} Struct150EC3D4;

s32 func_150EC3D4(Struct150EC3D4 *arg0, Struct150EC3D4 *arg1) {
    s32 temp;

    if (arg0 == arg1) {
        return 0;
    }
    if (arg0->unk0 == 0) {
        return 0;
    }
    temp = arg0->unk4;
    if (temp == 0xFF) {
        return 0;
    }
    if ((temp == 0) || (temp == 1) || (temp == 2) || (temp == 3) || (temp == 4) || (temp == 0x28) || (temp == 0x77)) {
        return 1;
    }
    return 0;
}

void func_151C3B0C(s32, f32, f32, f32, f32, s32, s32, s32);

void func_150EC45C(s32 arg0) {
    func_151C3B0C(arg0, 1.0f, 1.0f, 1.0f, 0.0f, 0xFF, 0xFF, 0xFF);
}
