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

/* cosf: cos(x) = sin(|x| + pi/2), so the range reduction rounds to the nearest
   HALF integer (the +0.5f / -0.5f pair around the rounding step) and the same
   odd polynomial is then evaluated on the reduced argument.
   The test uses the biased exponent field of x (bits 22..30); >= 0x136 means
   |x| is too large to reduce meaningfully. */
f32 cosf(f32 x)
{
    f32 xa;
    f32 t;
    f32 y;
    f32 ysq;
    f32 result;
    s32 n;

    n = *((s32 *) &x);
    if (((n >> 22) & 0x1FF) < 0x136) {
        if (0.0f < x) {
            xa = x;
        } else {
            xa = -x;
        }
        y = xa;
        t = (xa * rpi) + 0.5f;
        /* NOTE: this zero is spelled `0.f` while the one above is `0.0f`.
           IDO pools identical float literals within a function into a single
           value, which would keep one register tied up across the whole block
           and shift the allocation; two distinct spellings keep them separate,
           as the shipped code has them. Do not normalise the spelling. */
        if (0.f <= t) {
            n = (s32) (t + 0.5f);
        } else {
            n = (s32) (t - 0.5f);
        }
        t = ((f32) n) - 0.5f;
        y = y - (t * pihi);
        y = y - (t * pilo);
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
