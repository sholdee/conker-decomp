#include <ultra64.h>

/* This file's .bss, defined before the headers: IDO lays out .bss in the order it first sees each name. */
static s16 D_80035500; // reset by osSyncPrintf; nothing reads it in this build

#include "libc/stdarg.h"
#include "functions.h"
#include "variables.h"


/* The output callback of func_10002088: discards the text; any non-NULL return lets the formatting go on. */
u8 *func_10002070(u8 *arg, u8 *buf, u32 count) {
    return (u8 *)1;
}

/* osSyncPrintf: formats and discards the text (debug output is compiled out). */
void func_10002088(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    D_80035500 = 0;
    // the formatter reads unsigned chars (libultra's char is unsigned; this build uses -signed)
    func_100020D0(func_10002070, NULL, (u8 *)fmt, args);
}

static u8 D_8002AAF0[] = "                                "; // spaces
static u8 D_8002AB14[] = "00000000000000000000000000000000"; // zeroes
char *strchr(const char *str, s32 ch);

#define ATOI(dst, src)                         \
    for (dst = 0; *src >= '0' && *src <= '9'; ++src) {  \
        if (dst < 999)                                  \
            dst = dst * 10 + *src - '0';                \
    }
#define PUT(s, n)                              \
    if (0 < (n)) {                                      \
        if ((arg = (*pfn)(arg, s, n)) != NULL)    \
            x.nchar += (n);                             \
        else                                            \
            return x.nchar;                             \
    }
#define PAD(s, n)                                      \
    if (0 < (n)) {                                              \
        int i, j = (n);                                         \
        for (; 0 < j; j -= i) {                                 \
            i = 32 < (unsigned int)j ? 32 : j;                  \
            PUT(s, i);                                 \
        }                                                       \
    }

int func_100020D0(u8 *(*pfn)(u8 *, u8 *, u32), u8 *arg, u8 *fmt, va_list args) {
    _Pft x;

    x.nchar = 0;
    while (1) {
        u8 *s;
        u8 c;
        char *t;
        static const char D_8002BF84[] = " +-#0"; // the flag characters
        static const u32 D_8002BF8C[] = { 1, 2, 4, 8, 16, 0 }; // their bits in _Pft.flags
        u8 ac[32];

        s = fmt;
        while ((c = *s++) > 0) {
            if (c == '%') {
                s--;
                break;
            }
        }
        PUT(fmt, s - fmt);
        if (c == 0) {
            return x.nchar;
        }
        fmt = ++s;
        for (x.flags = 0; (t = strchr(D_8002BF84, *s)) != NULL; s++) {
            x.flags |= D_8002BF8C[t - D_8002BF84];
        }
        if (*s == '*') {
            x.width = va_arg(args, int);
            if (x.width < 0) {
                x.width = -x.width;
                x.flags |= 4;
            }
            s++;
        } else {
            ATOI(x.width, s);
        }
        if (*s != '.') {
            x.prec = -1;
        } else if (*++s == '*') {
            x.prec = va_arg(args, int);
            ++s;
        } else {
            ATOI(x.prec, s);
        }
        x.qual = strchr("hlL", *s) ? *s++ : '\0';
        if (x.qual == 'l' && *s == 'l') {
            x.qual = 'L';
            ++s;
        }
        func_10002718(&x, &args, *s, ac);
        x.width -= x.n0 + x.nz0 + x.n1 + x.nz1 + x.n2 + x.nz2;
        if (!(x.flags & 4)) {
            PAD(D_8002AAF0, x.width);
        }
        PUT(ac, x.n0);
        PAD(D_8002AB14, x.nz0);
        PUT(x.s, x.n1);
        PAD(D_8002AB14, x.nz1);
        PUT(x.s + x.n1, x.n2);
        PAD(D_8002AB14, x.nz2);
        if (x.flags & 4) {
            PAD(D_8002AAF0, x.width);
        }
        fmt = s + 1;
    }
    return 0;
}


#define FLAGS_SPACE 1
#define FLAGS_PLUS 2
#define FLAGS_MINUS 4
#define FLAGS_HASH 8
#define FLAGS_ZERO 16

#define LDSIGN(x) (((unsigned short *)&(x))[0] & 0x8000)

void _Litob(_Pft *px, unsigned char code);
unsigned int strlen(const char *s);

void func_10002718(_Pft *px, va_list *pap, unsigned char code, unsigned char *ac) {
    px->n0 = px->nz0 = px->n1 = px->nz1 = px->n2 = px->nz2 = 0;
    switch (code) {
        case 'c':
            ac[px->n0++] = va_arg(*pap, unsigned int);
            break;
        case 'd':
        case 'i':
            if (px->qual == 'l') {
                px->v.ll = va_arg(*pap, long);
            } else if (px->qual == 'L') {
                px->v.ll = va_arg(*pap, long long);
            } else {
                px->v.ll = va_arg(*pap, int);
            }
            if (px->qual == 'h') {
                px->v.ll = (short)px->v.ll;
            }
            if (px->v.ll < 0) {
                ac[px->n0++] = '-';
            } else if (px->flags & FLAGS_PLUS) {
                ac[px->n0++] = '+';
            } else if (px->flags & FLAGS_SPACE) {
                ac[px->n0++] = ' ';
            }
            px->s = &ac[px->n0];
            _Litob(px, code);
            break;
        case 'x':
        case 'X':
        case 'u':
        case 'o':
            if (px->qual == 'l') {
                px->v.ll = va_arg(*pap, long);
            } else if (px->qual == 'L') {
                px->v.ll = va_arg(*pap, long long);
            } else {
                px->v.ll = va_arg(*pap, int);
            }
            if (px->qual == 'h') {
                px->v.ll = (unsigned short)px->v.ll;
            } else if (px->qual == 0) {
                px->v.ll = (unsigned int)px->v.ll;
            }
            if (px->flags & FLAGS_HASH) {
                ac[px->n0++] = '0';
                if (code == 'x' || code == 'X') {
                    ac[px->n0++] = code;
                }
            }
            px->s = &ac[px->n0];
            _Litob(px, code);
            break;
        case 'e':
        case 'f':
        case 'g':
        case 'E':
        case 'G':
            px->v.ld = px->qual == 'L' ? va_arg(*pap, long double) : va_arg(*pap, double);
            if (LDSIGN(px->v.ld)) {
                ac[px->n0++] = '-';
            } else if (px->flags & FLAGS_PLUS) {
                ac[px->n0++] = '+';
            } else if (px->flags & FLAGS_SPACE) {
                ac[px->n0++] = ' ';
            }
            px->s = &ac[px->n0];
            func_10001550(px, code);
            break;
        case 'n':
            if (px->qual == 'h') {
                *(va_arg(*pap, unsigned short *)) = px->nchar;
            } else if (px->qual == 'l') {
                *va_arg(*pap, unsigned long *) = px->nchar;
            } else if (px->qual == 'L') {
                *va_arg(*pap, unsigned long long *) = px->nchar;
            } else {
                *va_arg(*pap, unsigned int *) = px->nchar;
            }
            break;
        case 'p':
            px->v.ll = (long)va_arg(*pap, void *);
            px->s = &ac[px->n0];
            _Litob(px, 'x');
            break;
        case 's':
            px->s = va_arg(*pap, unsigned char *);
            px->n1 = strlen((char *)px->s);
            if (px->prec >= 0 && px->n1 > px->prec) {
                px->n1 = px->prec;
            }
            break;
        case '%':
            ac[px->n0++] = '%';
            break;
        default:
            ac[px->n0++] = code;
            break;
    }
}
