#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#include "libc/stdlib.h"
#include "libc/string.h"

/* Zeroes the 0xFE0-byte block at D_80043B40, one word at a time. */
void func_10001420(void) {
    // matching: the four zero terms and the if (1) block steer IDO's allocator to a1/a0
    // (plain loop is byte-identical but uses v0/v1); no idiomatic form found
    s32 zero0 = 0;
    s32 zero1 = 0;
    s32 size = 0xFE0;
    s32 zero2;
    s32 zero3;
    s32 *p = D_80043B40;

    if (1) {
        zero2 = 0;
        zero3 = 0;
        do {
            *p++ = 0;
        } while ((u32)p < (u32)D_80043B40 + size + zero0 + zero1 + zero2 + zero3);
    }
}

/* With interrupts disabled: clears D_80043B40 and invalidates the I-cache from
 * D_1002AAD0 up to the end of 4MB RDRAM. */
void func_10001444(void) {
    u32 saveMask = __osDisableInt();
    func_100061F8(2, 31);
    func_10001420();
    func_10005BE0();
    osInvalICache(&D_1002AAD0, 0x80400000 - (u32)&D_1002AAD0);
    __osRestoreInt(saveMask);
}

/* Stops thread gMainThread. */
void func_100014A0(void) {
    osStopThread(&gMainThread);
}

/* With interrupts disabled: releases the D_8003BE74 and D_8003BE70 blocks (as KSEG0
 * pointers) through heap_free, then hands arg0 to func_10005B04 and clears D_80043B40. */
void func_100014C4(s32 arg0) {
    u32 saveMask = __osDisableInt();
    func_100061F8(2, 31);

    if (D_8003BE74) {
        heap_free((void *)(D_8003BE74 | 0x80000000));
    }
    if (D_8003BE70) {
        heap_free((void *)(D_8003BE70 | 0x80000000));
    }
    func_10005B04(arg0);
    func_10001420();
    func_10005BE0();
    __osRestoreInt(saveMask);
}

#define FLAGS_MINUS 4
#define FLAGS_HASH 8
#define FLAGS_ZERO 16

/* 10^(2^i): scales the value by powers of ten one exponent bit at a time. */
static const double sPowersOfTen[] = { 1e1, 1e2, 1e4, 1e8, 1e16, 1e32, 1e64, 1e128, 1e256 };

/* _Ldtob (libultra xldtob.c). Unlike the SDK, this copy builds its 0.0/1.0 constants as
 * float locals converted to double at runtime (golden: mtc1/cvt.d.s, with 0.0 kept in a
 * double stack slot) -- IDO folds every literal spelling. The -g3 frame also holds one
 * extra pointer local, here the typed copy of arg0. Formats the double in *arg0 for
 * the %e/%E/%f/%g/%G conversion code. */
void _Ldtob(void *arg0, unsigned char code) {
    unsigned char buff[32];
    unsigned char *p;
    double ldval;
    double zero;
    float fz = 0.0f;
    float fo = 1.0f;
    _Pft *px = arg0;
    short err;
    short nsig;
    short xexp;

    p = buff;
    ldval = px->v.ld;
    zero = fz;

    if (px->prec < 0) {
        px->prec = 6;
    } else if (px->prec == 0 && (code == 'g' || code == 'G')) {
        px->prec = 1;
    }

    err = _Ldunscale(&xexp, px);
    if (err > 0) {
        memcpy(px->s, err == 2 ? "NaN" : "Inf", px->n1 = 3);
        return;
    } else if (err == 0) {
        nsig = 0;
        xexp = 0;
    } else {
        {
            int i;
            int n;

            if (ldval < zero) {
                ldval = -ldval;
            }

            /* convert the binary exponent to a decimal one (30103 / 100000 ~= log10(2)) */
            if ((xexp = xexp * 30103 / 100000 - 4) < 0) {
                n = (3 - xexp) & ~3, xexp = -n;

                for (i = 0; n > 0; n >>= 1, i++) {
                    if (n & 1) {
                        ldval *= sPowersOfTen[i];
                    }
                }
            } else if (xexp > 0) {
                double factor = fo;

                xexp &= ~3;

                for (n = xexp, i = 0; n > 0; n >>= 1, i++) {
                    if (n & 1) {
                        factor *= sPowersOfTen[i];
                    }
                }

                ldval /= factor;
            }
        }
        {
            int gen = px->prec + ((code == 'f') ? 10 + xexp : 6);

            if (gen > 19) {
                gen = 19;
            }

            for (*p++ = '0'; gen > 0 && zero < ldval; p += 8) {
                int j;
                long lo = ldval;

                if ((gen -= 8) > 0) {
                    ldval = (ldval - lo) * 1e8;
                }

                for (p += 8, j = 8; lo > 0 && --j >= 0;) {
                    ldiv_t qr;
                    qr = ldiv(lo, 10);
                    *--p = qr.rem + '0', lo = qr.quot;
                }

                while (--j >= 0) {
                    *--p = '0';
                }
            }

            gen = p - &buff[1];

            for (p = &buff[1], xexp += 7; *p == '0'; p++) {
                --gen, --xexp;
            }

            nsig = px->prec + ((code == 'f') ? xexp + 1 : ((code == 'e' || code == 'E') ? 1 : 0));

            if (gen < nsig) {
                nsig = gen;
            }

            if (nsig > 0) {
                const unsigned char drop = nsig < gen && '5' <= p[nsig] ? '9' : '0';
                int n;

                for (n = nsig; p[--n] == drop;) {
                    --nsig;
                }

                if (drop == '9') {
                    ++p[n];
                }

                if (n < 0) {
                    --p, ++nsig, ++xexp;
                }
            }
        }
    }

    _Genld(px, code, p, nsig, xexp);
}


/* _Ldunscale (libultra xldtob.c): splits the double in px into its binary exponent
 * (stored to *pex) and a fraction in [0.5, 1). Returns 2 for NaN, 1 for infinity,
 * -1 for a finite nonzero value and 0 for zero. */
s16 _Ldunscale(s16 *pex, _Pft *px) {
    u16 *ps = (u16 *)px;
    s16 xchar = (ps[0] & 0x7FF0) >> 4;

    if (xchar == 0x7FF) {
        *pex = 0;
        return (ps[0] & 0xF) || ps[1] || ps[2] || ps[3] ? 2 : 1;
    } else if (xchar > 0) {
        ps[0] = (ps[0] & 0x800F) | 0x3FF0;
        *pex = xchar - 0x3FE;
        return -1;
    } else if (xchar < 0) {
        return 2;
    } else {
        *pex = 0;
        return 0;
    }
}

/* _Genld (libultra xldtob.c): lays out the nsig digits at p with decimal exponent xexp
 * as %f or %e text in px->s, setting the n1/nz1/n2/nz2 field widths. */
void _Genld(_Pft *px, unsigned char code, unsigned char *p, short nsig, short xexp) {
    if (nsig <= 0) {
        p = (unsigned char *)"0";
        nsig = 1;
    }

    if (code == 'f' || ((code == 'g' || code == 'G') && (-4 <= xexp) && (xexp < px->prec))) {
        xexp++;
        if (code != 'f') {
            if (!(px->flags & FLAGS_HASH) && nsig < px->prec) {
                px->prec = nsig;
            }
            if ((px->prec -= xexp) < 0) {
                px->prec = 0;
            }
        }
        if (xexp <= 0) {
            px->s[px->n1++] = '0';
            if (0 < px->prec || px->flags & FLAGS_HASH) {
                px->s[px->n1++] = '.';
            }
            if (px->prec < -xexp) {
                xexp = -px->prec;
            }
            px->nz1 = -xexp;
            px->prec += xexp;
            if (px->prec < nsig) {
                nsig = px->prec;
            }
            memcpy(&px->s[px->n1], p, px->n2 = nsig);
            px->nz2 = px->prec - nsig;
        } else if (nsig < xexp) {
            memcpy(&px->s[px->n1], p, nsig);
            px->n1 += nsig;
            px->nz1 = xexp - nsig;
            if (0 < px->prec || px->flags & FLAGS_HASH) {
                px->s[px->n1] = '.', ++px->n2;
            }
            px->nz2 = px->prec;
        } else {
            memcpy(&px->s[px->n1], p, xexp);
            px->n1 += xexp;
            nsig -= xexp;
            if (0 < px->prec || px->flags & FLAGS_HASH) {
                px->s[px->n1++] = '.';
            }
            if (px->prec < nsig) {
                nsig = px->prec;
            }
            memcpy(&px->s[px->n1], p + xexp, nsig);
            px->n1 += nsig;
            px->nz1 = px->prec - nsig;
        }
    } else {
        if (code == 'g' || code == 'G') {
            if (nsig < px->prec) {
                px->prec = nsig;
            }
            if (--px->prec < 0) {
                px->prec = 0;
            }
            code = code == 'g' ? 'e' : 'E';
        }
        px->s[px->n1++] = *p++;
        if (0 < px->prec || px->flags & FLAGS_HASH) {
            px->s[px->n1++] = '.';
        }
        if (0 < px->prec) {
            if (px->prec < --nsig) {
                nsig = px->prec;
            }
            memcpy(&px->s[px->n1], p, nsig);
            px->n1 += nsig;
            px->nz1 = px->prec - nsig;
        }
        p = &px->s[px->n1];
        *p++ = code;
        if (0 <= xexp) {
            *p++ = '+';
        } else {
            *p++ = '-';
            xexp = -xexp;
        }
        if (100 <= xexp) {
            if (1000 <= xexp) {
                *p++ = xexp / 1000 + '0', xexp %= 1000;
            }
            *p++ = xexp / 100 + '0', xexp %= 100;
        }
        *p++ = xexp / 10 + '0', xexp %= 10;
        *p++ = xexp + '0';
        px->n2 = p - &px->s[px->n1];
    }
    if ((px->flags & (FLAGS_ZERO | FLAGS_MINUS)) == FLAGS_ZERO) {
        int n = px->n0 + px->n1 + px->nz1 + px->n2 + px->nz2;

        if (n < px->width) {
            px->nz0 = px->width - n;
        }
    }
}
