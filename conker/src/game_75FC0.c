#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* asinf rational approximation coefficients: numerator P[1..5], denominator Q[0..4] (Q[4] = 1) */
static const f32 D_80099020[] = { 0.0f, 3.619550943f, -6.466859818f, 3.510708094f, -0.5643919706f, 0.005872000009f };
static const f32 D_80099038[] = { 21.71730423f, -48.57290649f, 37.10492325f, -11.03184891f, 1.0f };
/* asinf coefficients for the sqrt-reduced range: P[1..3], Q[0..3] */
static const f32 D_8009904C[] = { 0.0f, -3.184573889f, 3.210297108f, -0.6077240109f };
static const f32 D_8009905C[] = { -19.10744667f, 27.86013031f, -11.06533718f, 1.0f, 0.0f };

/* asinf scalar constants (const scalars: IDO places them in .data) */
static const f32 D_80085FD0 = 1.0f;
static const f32 D_80085FD4 = -1.0f;
static const f32 D_80085FD8 = 0.7853980064f;  /* pi/4 */
static const f32 D_80085FDC = 1.570796013f;   /* pi/2 */
static const f32 D_80085FE0 = -0.7853980064f; /* -pi/4 */
static const f32 D_80085FE4 = -1.570796013f;  /* -pi/2 */
static const f32 D_80085FE8 = 0.866024971f;   /* cos(pi/6) */

/* Builds the inverse of the rigid part (translation and rotation, no scale) of a transform. */
void func_15048B10(MtxF mtx, MtxF out) {
    f32 x;
    f32 y;
    f32 z;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    MtxF rotXMtx;
    MtxF rotYMtx;
    MtxF rotZMtx;
    MtxF translate;
    MtxF tmp1;
    MtxF tmp2;

    func_1503E5F8(mtx, &x, &y, &z, &rotX, &rotY, &rotZ, &scaleX, &scaleY, &scaleZ);
    guMtxIdentF(translate);
    translate[3][0] = -x;
    translate[3][1] = -y;
    translate[3][2] = -z;

    func_150A8050(rotXMtx, -rotX, 0.0f, 0.0f);
    func_150A8050(rotYMtx, 0.0f, -rotY, 0.0f);
    func_150A8050(rotZMtx, 0.0f, 0.0f, -rotZ);

    func_150A7A48(translate, rotZMtx, tmp1);
    func_150A7A48(tmp1, rotYMtx, tmp2);
    func_150A7A48(tmp2, rotXMtx, out);
}

/* asinf: rational approximation, with range reduction for |x| >= 0.5. */
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
            result = ((sq * num * reduced) / den) + reduced;
            if (bits > 0) {
                return (0.5f * result) + D_80085FD8;
            } else {
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

/* out = a + b */
void func_15048F20(Vec3f *a, Vec3f *b, Vec3f *out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
}

/* out = a - b */
void func_15048F58(Vec3f *a, Vec3f *b, Vec3f *out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
}

/* out = to - from */
void func_15048F90(Vec3f *from, Vec3f *to, Vec3f *out) {
    out->x = to->x - from->x;
    out->y = to->y - from->y;
    out->z = to->z - from->z;
}

/* Heading of a vector in the XZ plane, in degrees [0, 360). */
f32 func_15048FC8(Vec3f *vec) {
    f32 len;
    f32 angle;

    len = sqrtf((vec->x * vec->x) + (vec->z * vec->z));
    if (len == 0.0f) {
        return 0.0f;
    }

    angle = -vec->x / len; // matching: the sine goes through `angle` before the asinf call
    angle = func_15048C30(angle);

    if (vec->z > 0.0f) {
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
s32 func_150490A8(Vec3f *vec) {
    f32 len;
    s32 angle;

    len = sqrtf((vec->x * vec->x) + (vec->z * vec->z));

    if (len == 0.0f) {
        return 0;
    }

    len = vec->x / len; // matching: IDO needs the ratio in the same f32 local as the length
    angle = func_15048864(len);

    if (vec->z > 0.0f) {
        if (angle < 64) {
            angle = 128 - angle;
        } else {
            angle = 384 - angle;
        }
    }
    return angle;
}

/* out = vec * scale */
void func_15049148(Vec3f *vec, f32 scale, Vec3f *out) {
    out->x = vec->x * scale;
    out->y = vec->y * scale;
    out->z = vec->z * scale;
}

/* out = vec normalized (a zero vector stays zero) */
void func_1504917C(Vec3f *vec, Vec3f *out) {
    f32 scale = func_150AD930(&vec->x);

    if (scale != 0.0f) {
        scale = 1.0f / scale;
    }
    out->x = vec->x * scale;
    out->y = vec->y * scale;
    out->z = vec->z * scale;
}

/* out = unit vector pointing from `from` to `to` */
void func_150491EC(Vec3f *from, Vec3f *to, Vec3f *out) {
    out->x = to->x - from->x;
    out->y = to->y - from->y;
    out->z = to->z - from->z;
    func_1504917C(out, out);
}
