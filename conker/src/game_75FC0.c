#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* asinf rational approximation coefficients: numerator P[1..5], denominator Q[0..4] (Q[4] = 1) */
static const f32 D_80099020[] = { 0.0f, 3.619550943f, -6.466859818f, 3.510708094f, -0.5643919706f, 0.005872000009f };
static const f32 D_80099038[] = { 21.71730423f, -48.57290649f, 37.10492325f, -11.03184891f, 1.0f };
/* asinf coefficients for the sqrt-reduced range: P[1..3], Q[0..3] */
static const f32 D_8009904C[] = { 0.0f, -3.184573889f, 3.210297108f, -0.6077240109f };
static const f32 D_8009905C[] = { -19.10744667f, 27.86013031f, -11.06533718f, 1.0f, 0.0f };

void func_1503E5F8(f32 mtx[4][4], f32 *x, f32 *y, f32 *z, f32 *rotX, f32 *rotY, f32 *rotZ,
                   f32 *scaleX, f32 *scaleY, f32 *scaleZ);
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 out[4][4]);

/*
 * Builds the inverse of the rigid part (translation and rotation, no scale) of a transform.
 * mtx and out are f32[4][4] matrices; functions.h still declares them as s32.
 */
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

/*
 * asinf: rational approximation, with range reduction for |x| >= 0.5.
 * D_80085FD0 / D_80085FD4 are 1.0f / -1.0f; matching: they are loaded from rodata, a literal would use lui/mtc1.
 */
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
        if (absX < 0.866024971f) {
            /* asin(x) = +-pi/4 + asin(2x^2 - 1) / 2 */
            sq = x * x;
            reduced = (sq + sq) - D_80085FD0;
            sq = reduced * reduced;
            num = (((((((D_80099020[5] * sq) + D_80099020[4]) * sq) + D_80099020[3]) * sq) + D_80099020[2]) * sq) + D_80099020[1];
            den = ((((((sq + D_80099038[3]) * sq) + D_80099038[2]) * sq) + D_80099038[1]) * sq) + D_80099038[0];
            result = ((sq * num * reduced) / den) + reduced;
            if (bits > 0) {
                return (0.5f * result) + 0.785398006f;
            } else {
                return -0.785398006f - (0.5f * result);
            }
        }
        /* asin(x) = +-(pi/2 - 2 asin(sqrt((1 - |x|) / 2))) */
        reduced = (D_80085FD0 - absX) * 0.5f;
        root = sqrtf(reduced);
        num = (((D_8009904C[3] * reduced) + D_8009904C[2]) * reduced) + D_8009904C[1];
        den = ((((reduced + D_8009905C[2]) * reduced) + D_8009905C[1]) * reduced) + D_8009905C[0];
        result = ((reduced * num * root) / den) + root;
        if (bits > 0) {
            return 1.57079601f - (result + result);
        } else {
            return result + result + -1.57079601f;
        }
    }
    if (x != x) {
        /* NaN */
        return 0.0f;
    }
    if (x == D_80085FD0) {
        return 1.57079601f;
    }
    if (x == D_80085FD4) {
        return -1.57079601f;
    }
    return 0.0f;
}

/* out = a + b */
void func_15048F20(struct17 *a, struct17 *b, struct17 *out) {
    out->unk0 = a->unk0 + b->unk0;
    out->unk4 = a->unk4 + b->unk4;
    out->unk8 = a->unk8 + b->unk8;
}

/* out = a - b */
void func_15048F58(struct17 *a, struct17 *b, struct17 *out) {
    out->unk0 = a->unk0 - b->unk0;
    out->unk4 = a->unk4 - b->unk4;
    out->unk8 = a->unk8 - b->unk8;
}

/* out = to - from */
void func_15048F90(struct17 *from, struct17 *to, struct17 *out) {
    out->unk0 = to->unk0 - from->unk0;
    out->unk4 = to->unk4 - from->unk4;
    out->unk8 = to->unk8 - from->unk8;
}

/* Heading of a vector in the XZ plane, in degrees [0, 360). */
f32 func_15048FC8(struct17 *vec) {
    f32 len;
    f32 angle;

    len = sqrtf((vec->unk0 * vec->unk0) + (vec->unk8 * vec->unk8));
    if (len == 0.0f) {
        return 0.0f;
    }

    angle = -vec->unk0 / len; // matching: the sine goes through `angle` before the asinf call
    angle = func_15048C30(angle);

    if (vec->unk8 > 0.0f) {
        angle = 270.0f - (angle * 57.29577637f);
    } else {
        angle = 90.0f + (angle * 57.29577637f);
    }

    angle -= 90.0f;

    if (angle < 0.0f) {
        angle += 360.0f;
    }
    return angle;
}

/* Heading of a vector in the XZ plane as an integer angle, from the func_15048864 lookup table. */
s32 func_150490A8(struct17 *vec) {
    f32 len;
    s32 angle;

    len = sqrtf((vec->unk0 * vec->unk0) + (vec->unk8 * vec->unk8));

    if (len == 0.0f) {
        return 0;
    }

    len = vec->unk0 / len; // matching: IDO needs the ratio in the same f32 local as the length
    angle = func_15048864(len);

    if (vec->unk8 > 0.0f) {
        if (angle < 64) {
            angle = 128 - angle;
        } else {
            angle = 384 - angle;
        }
    }
    return angle;
}

/* out = vec * scale */
void func_15049148(struct17 *vec, f32 scale, struct17 *out) {
    out->unk0 = vec->unk0 * scale;
    out->unk4 = vec->unk4 * scale;
    out->unk8 = vec->unk8 * scale;
}

/* out = vec normalized (a zero vector stays zero) */
void func_1504917C(struct17 *vec, struct17 *out) {
    f32 scale = func_150AD930(&vec->unk0);

    if (scale != 0.0f) {
        scale = 1.0f / scale;
    }
    out->unk0 = vec->unk0 * scale;
    out->unk4 = vec->unk4 * scale;
    out->unk8 = vec->unk8 * scale;
}

/* out = unit vector pointing from `from` to `to` */
void func_150491EC(struct17 *from, struct17 *to, struct17 *out) {
    out->unk0 = to->unk0 - from->unk0;
    out->unk4 = to->unk4 - from->unk4;
    out->unk8 = to->unk8 - from->unk8;
    func_1504917C(out, out);
}
