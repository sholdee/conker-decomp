#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct225 *func_151602C0(Header *header, Header2 *header2, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7, s32 offset, u8 arg9, s32 argA);

void func_15145740(struct127 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 arg4);
s32 func_1514654C(struct127 *, struct126 *, s32, vertex **, f32 **, s32);
s32 func_151C229C(f32 *arg0, s32 arg1, f32 *arg2, f32 *arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, struct127 *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26);
void func_151C5588(struct127 *obj, f32 *pos, s16 arg2, u8 arg3, s32 arg4);
void func_151C56A4(f32 *pos, u8 arg1, s32 arg2);

/* Model-space points (model part 0x46) that func_151C5280 transforms to world space. */
extern vertex D_800AAA90;
extern vertex D_800AAAA8;

/* Spawns a func_151C229C effect along a segment in front of `obj`; with a model (unk1D4), from model
 * part 0x46 plus extra func_151C56A4 / func_151C5588 effects at its start. */
void func_151C5280(struct127 *obj, u8 arg1, s32 arg2) {
    f32 start[3];
    f32 end[3];
    f32 dir[3];
    f32 unkB0[3];
    struct126 *part;
    vertex *modelPoints[2];
    f32 *worldPoints[2];
    s32 arg16;
    s32 arg23;
    s32 randInt;
    f32 randB;
    f32 randA;

    if (obj != NULL) {
        func_15145740(obj, dir, unkB0, NULL, 0.0f);
        dir[0] *= 0.001f;
        dir[1] *= 0.001f;
        dir[2] *= 0.001f;
        if (obj->unk1D4 != NULL) {
            modelPoints[0] = &D_800AAA90;
            modelPoints[1] = &D_800AAAA8;
            worldPoints[0] = start;
            worldPoints[1] = end;
            part = func_1503195C(obj, 0x46, 0);
            if (part == NULL) {
                return;
            }
            if (func_1514654C(obj, part, 0, modelPoints, worldPoints, 2) == 0) {
                return;
            }
            func_151C56A4(start, arg1, arg2);
            if (func_150ADA20() & 1) {
                func_151C5588(obj, start, 0x46, arg1, arg2);
            }
        } else {
            start[0] = obj->x_position;
            start[1] = obj->y_position + 106.0f;
            start[2] = obj->z_position;
            end[0] = start[0] + (dir[0] * -82.0f);
            end[1] = start[1] + (dir[1] * -82.0f);
            end[2] = start[2] + (dir[2] * -82.0f);
            start[0] -= dir[0] * -120.0f;
            start[1] -= dir[1] * -120.0f;
            start[2] -= dir[2] * -120.0f;
        }
        randA = func_150ADA68();
        randB = func_150ADA68();
        randInt = func_150ADA20();
        if (obj->unk1D4 != NULL) {
            arg16 = 255;
        } else {
            arg16 = 128;
        }
        if (D_800BE616 != 0) {
            arg23 = -1;
        } else {
            arg23 = 2;
        }
        func_151C229C(start, 0, end, dir, 1, 0, 300.0f, 1.0f / 300.0f, (randA * 10.0f) + 25.0f,
                      (randB * 150.0f) + 400.0f, 80.0f, (randInt % 56U) + 200, obj, 1, 1, 0, arg16, 0, 1, 0,
                      0x1A, 0.0f, 0xFF, arg23, 0, arg1, arg2);
    }
}

extern void func_15102B38(struct127 *, u8, vertex *, vertex *, f32 *, s32, s32, f32, f32 *, s32, s32, s32, u8, s32);
extern vertex D_800AAA9C;

/* Spawns a randomly sized and rotated func_15102B38 effect at `pos` on `obj`. */
void func_151C5588(struct127 *obj, f32 *pos, s16 arg2, u8 arg3, s32 arg4) {
    f32 scale[2];
    f32 angle;
    s32 rand[2]; /* rand[1] is drawn but never used */

    scale[1] = (func_150ADA68() * 1001.0f + 604.0f) * 0.01f;
    scale[0] = (func_150ADA68() * 59.0f + 141.0f) * 0.1f;
    rand[0] = func_150ADA20();
    rand[1] = func_150ADA20();
    angle = func_150ADA68();
    func_15102B38(obj, 0, &D_800AAA90, &D_800AAA9C, scale, (rand[0] & 3) + 6, 0xFF,
                  angle * 270.0f + 359.0f, pos, 0xFF, 0, arg2, arg3, arg4);
}

/* Spawns a func_151602C0 effect at `pos`, converted to integer coordinates. */
void func_151C56A4(f32 *pos, u8 arg1, s32 arg2) {
    Header header;
    Header2 intPos;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() % 3U) + 3;
    header.unk4 = 0;

    intPos.unk0 = pos[0];
    intPos.unk4 = pos[1];
    intPos.unk8 = pos[2];

    func_151602C0(&header, &intPos, (func_150ADA20() & 1) + 6, 0xFF, 0xE8, 0xAB, 0xFF, 0, 0, arg1, arg2);
}

extern s32 func_15137C64(f32 *, f32 *, f32 *, f32 *, f32 *, s32, s32, s32);
extern void func_151DC034(f32 *, f32, s32, s16, s32, u8, s32);
extern void func_151DA6F8(f32 *, f32 *, f32, s16, s32, f32, s32, s32, f32, f32, s32, s32, s32, s32, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, f32 *);
extern void func_15143874(s16, f32, f32 *, f32 *);
extern void *func_15130280(void *, u8, s32, s32, u8, s32);
void func_151D9014(f32 *a0, f32 *a1, s32 a2, f32 a3, s32 sp10, s32 sp14, f32 sp18, s32 sp1C, f32 sp20, f32 sp24, s32 sp28, s32 sp2C, s32 sp30, s32 sp34, u8 sp38, s32 sp3C);

typedef struct {
    u32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 unk30;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48[3];
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Spawn151C577C;

void func_151C577C(s32 arg0, s32 arg1, s32 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7) {
    f32 v0[3];
    struct17 v1;
    f32 v2[3];
    f32 v3[3];
    f32 v4[3];
    s16 cnt;
    f32 sinv;
    f32 cosv;
    f32 vel[3];
    s32 n;
    s32 i;
    f32 vel2[3];
    s32 j;
    Spawn151C577C sp;
    f32 scale;
    f32 fa;
    f32 fc;
    s32 ra;
    s32 rb;
    void *ret;

    if (func_15137C64(v0, &v1.unk0, v2, v3, v4, arg0, arg1, arg2) != 0) {
        fc = func_150ADA68();
        ra = func_150ADA20();
        func_151DC034(v0, fc * 24.0f + 168.0f, (ra % 101U + 0x9B) & 0xFF, (func_150ADA20() & 7) + 0xF, 1, arg6, arg7);
        if (arg4 != 0) {
            cnt = func_150ADA20() % 3U + 5;
            if (cnt != 0) {
                do {
                    fa = (func_150ADA68() * 33.0f + 60.0f) * 0.1000000015f;
                    func_15143874(func_150ADA20() & 0xFF, fa, &cosv, &sinv);
                    vel[0] = v4[0] * sinv + v3[0] * cosv;
                    vel[1] = v4[1] * sinv + v3[1] * cosv;
                    vel[2] = v4[2] * sinv + v3[2] * cosv;
                    fa = func_150ADA68();
                    rb = func_150ADA20();
                    ra = func_150ADA20();
                    fc = func_150ADA68();
                    func_151DA6F8(v0, vel, fa * 0.1620000005f + 0.1930000037f, rb % 9U + 0x12, ra % 156U + 0x64, fc * 4.0f + 4.0f, 3, func_150ADA20() & 0, 1.0f, 1.0f, 1, 1, 0, 0x10, 0xF, 0, arg6, arg7);
                    cnt--;
                } while (cnt != 0);
            }
        }
        if (arg3 != 0) {
            j = func_150ADA20() % 6U + 5;
            if (j != 0) {
                do {
                    fa = (func_150ADA68() * 150.0f + 200.0f) * 0.1000000015f;
                    vel2[0] = v2[0] * fa;
                    vel2[1] = v2[1] * fa;
                    vel2[2] = v2[2] * fa;
                    fa = func_150ADA68();
                    ra = func_150ADA20();
                    n = func_150ADA20();
                    func_151D9014(&v1.unk0, vel2, 1, fa * 0.682000041f + -0.4120000303f, ra % 11U + 5, n % 156U + 0x64, func_150ADA68() * 60.0f + 80.0f, 0, 1.0f, 1.0f, 1, 0, 1, 0, arg6, arg7);
                    j--;
                } while (j != 0);
            }
        }
        if (arg5 != 0) {
            j = func_150ADA20() % 7U + 3;
            scale = 0.9508649707f;
            sp.unk1D = 0x6C;
            sp.unk08 = 0x5103;
            sp.unk00 = 0x200005;
            sp.unk1E = 0x14;
            sp.unk20 = 0xC;
            sp.unk58 = 0x80D207;
            sp.unk60 = 8;
            sp.unk61 = 6;
            sp.unk62 = 0x10;
            sp.unk04 = 0;
            sp.unk0C = 0;
            sp.unk10 = 0;
            sp.unk63 = -1;
            sp.unk64 = -1;
            sp.unk65 = 0;
            sp.unk22 = 1;
            sp.unk24 = 1.0f;
            sp.unk14 = 0xE2;
            sp.unk15 = 0xB2;
            sp.unk16 = 0x60;
            sp.unk17 = 0xFF;
            sp.unk18 = 0x39;
            sp.unk19 = 0xF;
            sp.unk1A = 0;
            sp.unk1C = 0xFF;
            sp.unk30 = v1;
            sp.unk3C = 0.0f;
            sp.unk40 = 0.0f;
            sp.unk44 = 0.0f;
            for (; j > 0; j--) {
                ra = func_150ADA20();
                rb = func_150ADA20();
                func_15143794(ra & 0xFF, -(rb & 0x3F), func_150ADA68() * 7.0f + 2.0f, sp.unk48);
                fa = func_150ADA68() * 117.0f;
                sp.unk58 &= ~0xC0;
                sp.unk54 = (fa + -197.0f) * 0.001000000047f;
                i = (func_150ADA20() & 1) ? 0x80 : 0;
                n = (func_150ADA20() & 1) ? 0x40 : 0;
                sp.unk58 |= n | i;
                sp.unk1B = func_150ADA20() % 66U + 0xBE;
                sp.unk0A = func_150ADA20() % 45U + 0x2C;
                fc = func_150ADA68() * 122.0f + 77.0f;
                sp.unk28 = fc;
                sp.unk2C = fc;
                ret = func_15130280(&sp, 1, 0, 4, arg6, arg7);
                if (ret != NULL) {
                    memcpy((u8 *)ret + 0xA8, &scale, 4);
                }
            }
        }
    }
}

void func_15137F30(f32 *a0, f32 *a1, f32 *a2, f32 *a3, f32 arg4, void *arg5, f32 *out6C, f32 *out60, f32 *out54, f32 *out50, s16 *out4E, u8 *out4D, f32 *out48);
void func_151D9014(f32 *a0, f32 *a1, s32 a2, f32 a3, s32 sp10, s32 sp14, f32 sp18, s32 sp1C, f32 sp20, f32 sp24, s32 sp28, s32 sp2C, s32 sp30, s32 sp34, u8 sp38, s32 sp3C);

void func_151C5E74(f32 *a0, f32 *a1, f32 *a2, f32 *a3, f32 arg4, u8 *arg5) {
    f32 sp6C[3];
    f32 sp60[3];
    f32 sp54[3];
    f32 sp50;
    s16 sp4E;
    u8 sp4D;
    f32 sp48;

    func_15137F30(a0, a1, a2, a3, arg4, arg5, sp6C, sp60, sp54, &sp50, &sp4E, &sp4D, &sp48);
    func_151D9014(sp6C, sp54, 1, sp50, sp4E, sp4D, sp48, 0, 1.0f, 1.0f, 1, 0, 1, 0, arg5[0xC], arg5[1]);
}

typedef struct {
    u32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 unk30;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    struct17 unk48;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Params151C5F44; /* 0x70 */

typedef struct {
    u8 unk0;
    u8 unk1;
    f32 unk4;
} Extra151C5F44;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void *func_151C5F44(struct17 *arg0, struct17 *arg1, f32 arg2, f32 arg3, u8 arg4, s16 arg5, f32 arg6, u8 arg7,
                    s32 arg8, u8 arg9, s32 argA) {
    Params151C5F44 params;
    f32 scale;
    void *ret;
    Extra151C5F44 extra;
    s32 r2;
    s32 flag;
    s32 r1;

    scale = arg3;
    params.unk1D = 0x6C;
    params.unk08 = 0x5103;
    params.unk00 = 0x200005;
    params.unk04 = 0;
    params.unk0C = 0;
    params.unk10 = 0;
    params.unk1E = 0x14;
    params.unk20 = 0xC;
    r1 = ((func_150ADA20() & 1) ? 0x40 : 0);
    flag = (arg5 == -1) ? 0 : 1;
    r2 = (func_150ADA20() & 1) ? 0x80 : 0;
    params.unk58 = r2 | flag | 2 | 4 | r1 | 0x4000 | 0x8000 | 0x200 | 0x800000;
    params.unk60 = 8;
    params.unk61 = 6;
    if (arg7) {
        params.unk62 = 0x1D;
    } else {
        params.unk62 = 0xA;
    }
    params.unk63 = -1;
    params.unk64 = -1;
    params.unk22 = 1;
    params.unk65 = 0;
    extra.unk0 = 1;
    extra.unk1 = 0;
    params.unk14 = 0xE2;
    params.unk15 = 0xB2;
    params.unk16 = 0x60;
    params.unk17 = 0xFF;
    params.unk18 = 0x39;
    params.unk19 = 0xF;
    params.unk1A = 0;
    params.unk1C = 0xFF;
    params.unk24 = 1.0f;
    extra.unk4 = 0.7030000091f;
    params.unk30 = *arg0;
    params.unk3C = 0.0f;
    params.unk40 = 0.0f;
    params.unk44 = 0.0f;
    params.unk48 = *arg1;
    params.unk54 = arg2;
    params.unk1B = arg4;
    if (arg5 == -1) {
        params.unk0A = 0x12C;
    } else {
        params.unk0A = arg5;
    }
    params.unk2C = arg6;
    params.unk28 = arg6;
    ret = func_15130280(&params, 1, arg8, 0x10, arg9, argA);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0xA8, &scale, 4);
        memcpy((u8 *)ret + 0xB0, &extra, 8);
    }
    return ret;
}

struct conker151C61A0 {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 unk08;
    /* 0x0A */ s16 unk0A;
    /* 0x0C */ s16 unk0C;
    /* 0x0E */ s16 unk0E;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ s8 unk25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ s32 unk38;
};

void func_15154684(struct conker151C61A0 *arg0, u8 arg1, s32 arg2);

extern f32 D_800AAAF4;
extern f32 D_800AAAF8;
extern f32 D_800AAAFC;

void func_151C61A0(s32 a0, s32 a1, u8 a2, s32 a3) {
    struct conker151C61A0 sp1C;

    sp1C.unk00 = 0xA;
    sp1C.unk02 = 4;
    sp1C.unk04 = a0;
    sp1C.unk08 = 0;
    sp1C.unk0C = 0xFF;
    sp1C.unk0A = -0x18;
    sp1C.unk0E = 0x19;
    sp1C.unk10 = 5.0f;
    sp1C.unk14 = 7.0f;
    sp1C.unk18 = D_800AAAF4;
    sp1C.unk1C = D_800AAAF8;
    sp1C.unk20 = D_800AAAFC;
    sp1C.unk24 = 0xBE;
    sp1C.unk25 = 0x41;
    sp1C.unk26 = 0xC8;
    sp1C.unk28 = 0x96;
    sp1C.unk2C = 96.0f;
    sp1C.unk30 = 109.0f;
    sp1C.unk34 = 1.0f;
    sp1C.unk38 = a1;
    func_15154684(&sp1C, a2, a3);
}

s32 func_151C6290(u8 *arg0, s32 arg1, s32 arg2) {
    if (arg0[4] == 0x5A || arg0[4] == 0x74 || arg0[4] == 0x7A) {
        return 0;
    }
    return 1;
}
