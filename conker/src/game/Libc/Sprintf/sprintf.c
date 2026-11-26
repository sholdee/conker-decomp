#include <ultra64.h>

#include "libc/stdarg.h"
#include "libc/string.h"
#include "functions.h"
#include "variables.h"

/** sprintf output callback (libultra proutSprintf): append `count` bytes to `dst`, return the new end. */
u8 *func_151EFF70(u8 *dst, u8 *src, u32 count) {
    return (u8 *)memcpy(dst, src, count) + count;
}

/** sprintf: format into `buf`, NUL-terminate, and return the length (negative on error). */
s32 func_151EFF94(u8 *buf, u8 *fmt, ...) {
    va_list args;
    s32 length;

    va_start(args, fmt);
    length = func_100020D0(func_151EFF70, buf, fmt, args);
    if (length >= 0) {
        buf[length] = '\0';
    }
    return length;
}
