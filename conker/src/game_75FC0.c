#include <ultra64.h>

#include "functions.h"
#include "variables.h"

#if 0
f32 D_80099020[] = { 0.0, 3.619550943374634, -6.466859817504883, 3.5107080936431885, -0.5643919706344604, 0.005872000008821487 };
f32 D_80099038[] = { 21.717304229736328, -48.572906494140625, 37.104923248291016, -11.031848907470703, 1.0 };
f32 D_8009904C[] = { 0.0, -3.1845738887786865, 3.210297107696533, -0.6077240109443665 };
f32 D_8009905C[] = { -19.107446670532227, 27.860130310058594, -11.065337181091309, 1.0, 0.0 };

f32 D_80099070 = 57.2957763671875;
f32 D_80099074 = 57.2957763671875;
#endif

extern f32 D_80099020[];
extern f32 D_80099038[];
extern f32 D_8009904C[];
extern f32 D_8009905C[];
extern f32 D_80099070;
extern f32 D_80099074;


void func_1503E5F8(f32 mtx[4][4], f32 *x, f32 *y, f32 *z, f32 *rotX, f32 *rotY, f32 *rotZ,
                   f32 *scaleX, f32 *scaleY, f32 *scaleZ);
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 out[4][4]);

/* Builds the inverse of the rigid part (translation and rotation, no scale) of a transform. */
void func_15048B10(s32 mtx, s32 out) {
    f32 x;
    f32 y;
    f32 z;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    f32 rotXMtx[4][4];
    f32 rotYMtx[4][4];
    f32 rotZMtx[4][4];
    f32 translate[4][4];
    f32 tmp1[4][4];
    f32 tmp2[4][4];

    func_1503E5F8((f32 (*)[4])mtx, &x, &y, &z, &rotX, &rotY, &rotZ, &scaleX, &scaleY, &scaleZ);
    guMtxIdentF(translate);
    translate[3][0] = -x;
    translate[3][1] = -y;
    translate[3][2] = -z;

    func_150A8050(rotXMtx, -rotX, 0.0f, 0.0f);
    func_150A8050(rotYMtx, 0.0f, -rotY, 0.0f);
    func_150A8050(rotZMtx, 0.0f, 0.0f, -rotZ);

    func_150A7A48(translate, rotZMtx, tmp1);
    func_150A7A48(tmp1, rotYMtx, tmp2);
    func_150A7A48(tmp2, rotXMtx, (f32 (*)[4])out);
}

/* asinf: rational approximation, with range reduction for |x| >= 0.5 */
f32 func_15048C30(f32 x) {
    f32 root;
    f32 num;
    f32 den;
    f32 reduced;
    f32 sq;
    f32 absX;
    f32 result;
    s32 bits;
    s32 exponent;

    bits = *(s32 *)&x;
    exponent = (bits >> 23) & 0xFF;
    if (exponent < 126) {
        /* |x| < 0.5; tiny values return x itself */
        if (exponent >= 99) {
            sq = x * x;
            num = (((((((D_80099020[5] * sq) + D_80099020[4]) * sq) + D_80099020[3]) * sq) + D_80099020[2]) * sq) + D_80099020[1];
            den = ((((((sq + D_80099038[3]) * sq) + D_80099038[2]) * sq) + D_80099038[1]) * sq) + D_80099038[0];
            return ((sq * num * x) / den) + x;
        }
        return x;
    }
    if (exponent < 127) {
        /* 0.5 <= |x| < 1 */
        absX = fabsf(x);
        if (absX < D_80085FE8) {
            /* asin(x) = +-pi/4 + asin(2x^2 - 1) / 2 */
            sq = x * x;
            reduced = (sq + sq) - D_80085FD0;
            sq = reduced * reduced;
            num = (((((((D_80099020[5] * sq) + D_80099020[4]) * sq) + D_80099020[3]) * sq) + D_80099020[2]) * sq) + D_80099020[1];
            den = ((((((sq + D_80099038[3]) * sq) + D_80099038[2]) * sq) + D_80099038[1]) * sq) + D_80099038[0];
            if (bits > 0) {
                result = ((sq * num * reduced) / den) + reduced;
                return (0.5f * result) + D_80085FD8;
            } else {
                result = ((sq * num * reduced) / den) + reduced;
                return D_80085FE0 - (0.5f * result);
            }
        }
        /* asin(x) = +-(pi/2 - 2 asin(sqrt((1 - |x|) / 2))) */
        reduced = (D_80085FD0 - absX) * 0.5f;
        root = sqrtf(reduced);
        num = (((D_8009904C[3] * reduced) + D_8009904C[2]) * reduced) + D_8009904C[1];
        den = ((((reduced + D_8009905C[2]) * reduced) + D_8009905C[1]) * reduced) + D_8009905C[0];
        result = ((reduced * num * root) / den) + root;
        if (bits > 0) {
            return D_80085FDC - (result + result);
        } else {
            return result + result + D_80085FE4;
        }
    }
    if (x != x) {
        /* NaN */
        return 0.0f;
    }
    if (x == D_80085FD0) {
        return D_80085FDC;
    }
    if (x == D_80085FD4) {
        return D_80085FE4;
    }
    return 0.0f;
}

void func_15048F20(struct17 *arg0, struct17 *arg1, struct17 *arg2) {
    arg2->unk0 = arg0->unk0 + arg1->unk0;
    arg2->unk4 = arg0->unk4 + arg1->unk4;
    arg2->unk8 = arg0->unk8 + arg1->unk8;
}

void func_15048F58(struct17 *arg0, struct17 *arg1, struct17 *arg2) {
    arg2->unk0 = arg0->unk0 - arg1->unk0;
    arg2->unk4 = arg0->unk4 - arg1->unk4;
    arg2->unk8 = arg0->unk8 - arg1->unk8;
}

void func_15048F90(struct17 *arg0, struct17 *arg1, struct17 *arg2) {
    arg2->unk0 = arg1->unk0 - arg0->unk0;
    arg2->unk4 = arg1->unk4 - arg0->unk4;
    arg2->unk8 = arg1->unk8 - arg0->unk8;
}

f32 func_15048FC8(struct17 *arg0) {
    f32 tmp1;
    f32 tmp2;
    f32 tmp3;

    tmp1 = sqrtf((arg0->unk0 * arg0->unk0) + (arg0->unk8 * arg0->unk8));
    if (tmp1 == 0.0f) {
        return 0.0f;
    }

    tmp2 = -arg0->unk0 / tmp1;
    tmp2 = func_15048C30(tmp2);

    if (arg0->unk8 > 0.0f) {
        tmp2 = 270.0f - (tmp2 * D_80099070);
    } else {
        tmp2 = 90.0f + (tmp2 * D_80099074);
    }

    tmp2 -= 90.0f;

    if (tmp2 < 0.0f) {
        tmp2 += 360.0f;
    }
    return tmp2;
}

s32 func_150490A8(struct17 *arg0) {
    f32 temp_f12;
    s32 phi_v1;

    temp_f12 = sqrtf((arg0->unk0 * arg0->unk0) + (arg0->unk8 * arg0->unk8));

    if (temp_f12 == 0.0f) {
        return 0;
    }

    temp_f12 = arg0->unk0 / temp_f12;
    phi_v1 = func_15048864(temp_f12);

    if (0.0f < arg0->unk8) {
        if (phi_v1 < 64) {
            phi_v1 = 128 - phi_v1;
        } else {
            phi_v1 = 384 - phi_v1;
        }
    }
    return phi_v1;
}

void func_15049148(struct17 *arg0, f32 arg1, struct17 *arg2) {
    arg2->unk0 = arg0->unk0 * arg1;
    arg2->unk4 = arg0->unk4 * arg1;
    arg2->unk8 = arg0->unk8 * arg1;
}

void func_1504917C(struct17 *arg0, struct17 *arg1) {
    f32 tmp = func_150AD930(arg0);
    if (tmp != 0.0f) {
        tmp = 1.0f / tmp;
    }
    arg1->unk0 = arg0->unk0 * tmp;
    arg1->unk4 = arg0->unk4 * tmp;
    arg1->unk8 = arg0->unk8 * tmp;
}

void func_150491EC(struct17 *arg0, struct17 *arg1, struct17 *arg2) {
    arg2->unk0 = arg1->unk0 - arg0->unk0;
    arg2->unk4 = arg1->unk4 - arg0->unk4;
    arg2->unk8 = arg1->unk8 - arg0->unk8;
    func_1504917C(arg2, arg2);
}
