#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* ROM extent of one font's packed glyph data. */
typedef struct {
    s32 romStart;
    s32 romEnd;
} FontRomRange;

extern FontRomRange D_80082F80[];

extern u8 D_800CBD4C;

void func_15015910(void) {
    D_800CBD4C = 0;
}

/*
 * Loads font `font` from ROM into a scratch buffer and unpacks each glyph record. A record starts with
 * the 4-byte glyph metrics, then its own byte size as a big-endian u32, then the run-length encoded rows.
 */
void func_15015920(s16 font) {
    u8 *buffer;
    u8 *record;
    s32 glyph;

    buffer = func_10003C6C(0x2800, 1, 2, 1, 0);
    func_10004514(D_80082F80[font].romStart, buffer, D_80082F80[font].romEnd - D_80082F80[font].romStart, 1);
    record = buffer;
    glyph = 0;
    while (record + 15 < buffer + D_80082F80[font].romEnd - D_80082F80[font].romStart) {
        func_15015A38(record, glyph, font);
        record += (record[4] << 24) + (record[5] << 16) + (record[6] << 8) + record[7];
        glyph++;
    }
    func_10004074(buffer);
}


/*
 * Unpacks one glyph record into glyph `glyph` of font `font`: stores its metrics (the record holds
 * w - 1 and h - 1), then expands the run-length encoded rows into the glyph bitmap. Each data byte is
 * a run: low nibble = length - 1, high nibble = intensity. Rows are padded to a multiple of 8 texels,
 * odd rows are word-swapped (x ^ 4) for TMEM, and the bitmap gets a blank border row above and below
 * and a blank first column.
 */
void func_15015A38(u8 *record, s32 glyph, s32 font) {
    s32 x;
    s32 i;
    s32 y;
    s32 offset;
    s32 stride;
    s32 swizzle;
    s32 run;
    s32 shade;

    D_80085994[font][glyph].w = record[0] + 1;
    D_80085994[font][glyph].h = record[1] + 1;
    D_80085994[font][glyph].x = record[2];
    D_80085994[font][glyph].y = record[3];

    stride = (D_80085994[font][glyph].w + 7) & 0xFFF8;
    offset = 8;

    for (x = 0; x < D_80085994[font][glyph].w; x++) {
        D_80085990[font][glyph][x] = 0;
    }

    for (y = 1; y < D_80085994[font][glyph].h + 1; y++) {
        if ((y & 1) == 0) {
            swizzle = 0;
        } else {
            swizzle = 4;
        }
        for (x = 0; x < D_80085994[font][glyph].w; x++) {
            if (x == 0) {
                D_80085990[font][glyph][(x ^ swizzle) + y * stride] = 0;
            } else {
                run = (record[offset] & 0xF) + 1;
                shade = record[offset] & 0xF0;
                offset++;
                for (i = 0; i < run; i++) {
                    D_80085990[font][glyph][((x + i) ^ swizzle) + y * stride] = shade;
                }
                x += run - 1;
            }
        }
        for (; x < stride; x++) {
            D_80085990[font][glyph][(x ^ swizzle) + y * stride] = 0;
        }
    }

    for (x = 0; x < stride; x++) {
        D_80085990[font][glyph][x + y * stride] = 0;
    }
}
