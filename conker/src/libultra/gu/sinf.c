#include <ultra64.h>

typedef union {
    unsigned int i;
    float f;
} fu;

/* coefficients for polynomial approximation of sin on +/- pi/2 */
static const fu P[] = {
    { 0x3F800000 },
    { 0xBE2AAAA6 },
    { 0x3C088769 },
    { 0xB94FB7FD },
    { 0x362EDEFA },
};

/* Scalar constants: IDO places these in .data (the P table above is .rodata). */
static const f32 rpi = 0.3183099031f;    /* 1 / pi */
static const f32 pihi = 3.141592979f;    /* pi, high part */
static const f32 pilo = 3.178650942e-08f; /* pi, low part */
static const f32 zero = 0.0f;

/* sinf: SDK-style range reduction to +/-pi/2 followed by a degree-9 odd
   polynomial. `m` is the biased exponent field of |x| (bits 22..30), used to
   pick the path: tiny (return x), small enough for the bare polynomial, in
   range for reduction, or too large / NaN. */
f32 sinf(f32 x)
{
    f32 xsq;
    f32 y;
    f32 ysq;
    f32 result;
    f32 fn;
    s32 bits;
    s32 n;
    s32 m;

    bits = *((s32 *) &x);
    m = (bits >> 22) & 0x1FF;

    if (m < 0xFF) {
        if (m >= 0xE6) {
            y = x;
            xsq = y * y;
            result = (((((P[4].f * xsq) + P[3].f) * xsq) + P[2].f) * xsq) + P[1].f;
            return ((y * xsq) * result) + y;
        }
        return x;
    }
    if (m < 0x136) {
        y = x;
        fn = y * rpi;
        if (0.0f <= fn) {
            n = fn + 0.5f;
        } else {
            n = fn - 0.5f;
        }
        fn = n;
        y = y - (fn * pihi);
        y = y - (fn * pilo);
        ysq = y * y;
        result = (((((P[4].f * ysq) + P[3].f) * ysq) + P[2].f) * ysq) + P[1].f;
        if ((n & 1) == 0) {
            return ((y * ysq) * result) + y;
        }
        return -(((y * ysq) * result) + y);
    }
    if (x != x) {
        return __libm_qnan_f;
    }
    return zero;
}
