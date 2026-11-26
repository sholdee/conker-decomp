#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Rare's MPEG audio layer III decoder (the streamed-audio codec).  One global
 * instance, D_800E1880.  Field names follow ISO 11172-3 / 13818-3 where the
 * header and side-info bit widths read in func_151F8088 / func_151F6FD0 pin
 * them down.
 *
 * The ROM's address arithmetic makes several per-granule / per-channel arrays
 * overlap their neighbours, so their bounds below are the largest that do not
 * collide: the side-info arrays are [2][1] because [gr][ch] both stride one
 * element; scfsi[1] would run into part23Length, huffVal[1] starts at huffSign,
 * nonZero[1] is zeroTail[0], and xr[1] is xrSb.  Only gr 0 of a mono stream is
 * safe from the overlaps. */

typedef s32 (*Mp3DecodeFn)(Mp3Decoder *dec);

typedef struct {
    /* 0x00 */ s32 l[23];    /* long-block scalefactors, per band */
    /* 0x5C */ s32 s[3][13]; /* short-block scalefactors, [window][band] */
} Mp3ScaleFac; /* size 0xF8 */

struct Mp3Decoder {
    /* 0x0000 */ s32 handle;            /* opaque, passed back to read() */
    /* 0x0004 */ Mp3ReadFn read;
    /* 0x0008 */ s32 unk0008;
    /* 0x000C */ s32 unk000C;
    /* 0x0010 */ s32 unk0010;
    /* 0x0014 */ s32 unk0014;
    /* 0x0018 */ s32 streamPos;         /* bytes consumed from the stream */
    /* 0x001C */ u8  mainData[0x2000];  /* bit reservoir */
    /* 0x201C */ s32 mainDataLen;       /* bytes filled */
    /* 0x2020 */ s32 mainDataBit;       /* bit cursor */
    /* 0x2024 */ u8  headerBuf[0x40];   /* header + CRC + side info bytes */
    /* 0x2064 */ s32 headerBit;         /* bit cursor into headerBuf */
    /* 0x2068 */ s32 headerLen;         /* 4, or 6 with CRC */
    /* 0x206C */ s32 sideInfoLen;
    /* 0x2070 */ u8  outBuf[6][0x488];  /* ring of decoded output blocks */
    /* 0x3BA0 */ s32 outIndex;
    /* 0x3BA4 */ s32 id;                /* 1 = MPEG-1, 0 = MPEG-2 LSF */
    /* 0x3BA8 */ s32 layer;             /* raw 2-bit field: 1 = layer III */
    /* 0x3BAC */ s32 protectionBit;     /* 0 = CRC follows the header */
    /* 0x3BB0 */ s32 bitrateIndex;
    /* 0x3BB4 */ s32 sampleRateIndex;
    /* 0x3BB8 */ s32 paddingBit;
    /* 0x3BBC */ s32 privateBit;
    /* 0x3BC0 */ s32 mode;              /* 3 = mono */
    /* 0x3BC4 */ s32 modeExtension;
    /* 0x3BC8 */ s32 copyright;
    /* 0x3BCC */ s32 original;
    /* 0x3BD0 */ s32 emphasis;
    /* 0x3BD4 */ s32 haveFirstHeader;
    /* 0x3BD8 */ s32 firstId;           /* first frame's header, to detect a stream change */
    /* 0x3BDC */ s32 firstLayer;
    /* 0x3BE0 */ s32 firstProtectionBit;
    /* 0x3BE4 */ s32 firstSampleRateIndex;
    /* 0x3BE8 */ s32 firstMode;
    /* 0x3BEC */ s32 firstCopyright;
    /* 0x3BF0 */ s32 firstOriginal;
    /* 0x3BF4 */ s32 mainDataBegin;
    /* 0x3BF8 */ s32 scfsi[1][32];      /* [ch][scfsi band] */
    /* 0x3C78 */ s32 part23Length[2][1];
    /* 0x3C80 */ s32 bigValues[2][1];
    /* 0x3C88 */ s32 globalGain[2][1];
    /* 0x3C90 */ s32 scalefacCompress[2][1];
    /* 0x3C98 */ s32 windowSwitching[2][1];
    /* 0x3CA0 */ s32 blockType[2][1];
    /* 0x3CA8 */ s32 mixedBlock[2][1];
    /* 0x3CB0 */ s32 tableSelect[2][1][3];
    /* 0x3CC8 */ s32 subblockGain[2][1][3];
    /* 0x3CE0 */ s32 region0Count[2][1];
    /* 0x3CE8 */ s32 region1Count[2][1];
    /* 0x3CF0 */ s32 preflag[2][1];
    /* 0x3CF8 */ s32 scalefacScale[2][1];
    /* 0x3D00 */ s32 count1TableSelect[2][1];
    /* 0x3D08 */ Mp3ScaleFac scalefac[2][1];
    /* 0x3EF8 */ s32 intensityScale;    /* MPEG-2 LSF intensity stereo */
    /* 0x3EFC */ s32 unk3EFC[6];        /* written past its end, into unk3F14 */
    /* 0x3F14 */ s32 unk3F14[16];
    /* 0x3F54 */ u8  pad3F54[0x3F7C - 0x3F54];
    /* 0x3F7C */ s32 bitrate;
    /* 0x3F80 */ s32 sampleRate;
    /* 0x3F84 */ s32 frameSize;
    /* 0x3F88 */ s32 mainDataSize;      /* main-data bytes in this frame */
    /* 0x3F8C */ s32 nch;
    /* 0x3F90 */ s32 ngr;
    /* 0x3F94 */ s16 huffVal[1][578];   /* Huffman-decoded magnitudes, per ch */
    /* 0x4418 */ u8  huffSign[1][578];  /* their sign bits */
    /* 0x465A */ u8  pad465A[2];
    /* 0x465C */ s32 nonZero[1];        /* decoded lines, per ch */
    /* 0x4660 */ s32 zeroTail[1];       /* 576 - nonZero, per ch */
    /* 0x4664 */ f32 xr[1][576];        /* dequantised lines, per ch */
    /* 0x4F64 */ f32 xrSb[2][32][18];   /* reordered lines, [ch][subband][line] */
    /* 0x6164 */ u8  pad6164[0x6A64 - 0x6164];
    /* 0x6A64 */ f32 overlap[2][32][18]; /* IMDCT overlap-add state */
    /* 0x7C64 */ u8  pad7C64[0x8474 - 0x7C64];
    /* 0x8474 */ s32 seekPos;           /* next sync search start, -1 = continue */
    /* 0x8478 */ Mp3DecodeFn decodeFrame;
    /* 0x847C */ Mp3DecodeFn readSideInfo;
}; /* size 0x8480 */

/* func_151F3DE0's constants and the tables it builds */
extern f32 D_800B0C2C;
extern f32 D_800B0C30;
extern f32 D_800B0C34;
extern f32 D_800B0C38;
extern f32 D_800B0C3C;
extern f32 D_800B0C40;
extern f32 D_800B0C44;
extern f32 D_800B0C48;
/* IMDCT window per block type (0 normal, 1 start, 2 short, 3 stop) */
extern f32 D_800E0E38[4][36];
extern f32 *D_800E1078;          /* pow43: i^(4/3) */
extern f32 D_800E1080[256];      /* scalefactor gain powers */
extern f32 D_800E1480[256];      /* 0.25^i: subblock gain powers */

/* The pretab (preemphasis) table; a struct so it can be copied by value. */
typedef struct {
    /* 0x00 */ s32 band[22];
} Mp3Pretab; /* size 0x58 */

/* nr_of_sfb_block for MPEG-2 LSF scalefactors: [isRight][slen class][block kind][part] */
typedef struct {
    /* 0x000 */ s32 nrSfb[2][3][3][4];
} Mp3SfbCounts; /* size 0x120 */

extern u8  D_800AEB54[8];        /* scfsi band group boundaries: 0 6 11 16 21 | 0 6 12 */
extern u8  D_800AEB5C[2][16];    /* the two MPEG-1 scalefactor length (slen) tables */
extern s16 D_800AE840[][3][22];  /* long-block band ends, [id][sampleRateIndex] */
extern u8  D_800AE948[][3][13];  /* short-block band ends, [id][sampleRateIndex] */
extern s32 D_800AE7B8[34];       /* Huffman linbits, per table */
extern Mp3Pretab D_800B0AB4;
/* Scalefactor-band boundaries, one row per id * 3 + sampleRateIndex. */
typedef struct {
    /* 0x00 */ s16 l[23]; /* long-block band boundaries, l[0] = 0 */
    /* 0x2E */ s16 s[14]; /* short-block band boundaries (per window) */
} Mp3BandBounds; /* size 0x4A */

extern Mp3BandBounds D_800AE998[6];
extern Mp3SfbCounts D_800B0B0C;
extern s16 D_800AEB7C[2][3][576]; /* short-block reorder index, [id][sampleRateIndex] */
extern f32 D_800B067C[8];        /* alias-reduction ca */
extern f32 D_800B069C[8];        /* alias-reduction cs */
extern Mp3Decoder D_800E1880;
extern s32 D_800B06BC[2][15];    /* bitrate table, indexed [id][bitrateIndex] */
extern s32 D_800B0734[2][4];     /* sample-rate table, indexed [id][sampleRateIndex] */

/* func_151F8960(buf, &bitPos, n) reads n bits from buf at bitPos. */
#define GET_HEADER_BITS(dec, n) func_151F8960((dec)->headerBuf, &(dec)->headerBit, (n))
/* n bits of main data, a zero width reading as 0 */
#define GET_MAIN_BITS(dec, n) \
    (((n) != 0) ? func_151F8960((dec)->mainData, &(dec)->mainDataBit, (n)) : 0)

/* Builds the decoder's tables: the four IMDCT windows, the relocated Huffman
 * table pointers, x^(4/3) for the 8192 quantised magnitudes (Newton iteration
 * on x^3 = i^4) and the two gain-power tables. Returns 0 without a pow43 buffer. */
s32 func_151F3DE0(void) {
    s32 i;
    f32 x;
    f32 xSq;
    f32 step;
    f32 target;
    f32 tolerance;
    f32 gain;
    f32 subblockGain;

    for (i = 0; i < 36; i++) {
        D_800E0E38[0][i] = sinf((i + 0.5f) * D_800B0C2C);
    }
    for (i = 0; i < 18; i++) {
        D_800E0E38[1][i] = sinf((i + 0.5f) * D_800B0C30);
    }
    for (i = 18; i < 24; i++) {
        D_800E0E38[1][i] = 1.0f;
    }
    for (i = 24; i < 30; i++) {
        D_800E0E38[1][i] = sinf((i + 0.5f - 18.0f) * D_800B0C34);
    }
    for (i = 30; i < 36; i++) {
        D_800E0E38[1][i] = 0.0f;
    }
    for (i = 0; i < 6; i++) {
        D_800E0E38[3][i] = 0.0f;
    }
    for (i = 6; i < 12; i++) {
        D_800E0E38[3][i] = sinf((i + 0.5f - 6.0f) * D_800B0C38);
    }
    for (i = 12; i < 18; i++) {
        D_800E0E38[3][i] = 1.0f;
    }
    for (i = 18; i < 36; i++) {
        D_800E0E38[3][i] = sinf((i + 0.5f) * D_800B0C3C);
    }
    for (i = 1; i < 34; i++) {
        D_800E0E20[i] += D_800E0E28;
    }
    D_800E1078 = D_800E0E30;
    if (D_800E1078 == NULL) {
        return 0;
    }
    x = 1.0f;
    D_800E1078[0] = 0.0f;
    for (i = 1; i < 8192; i++) {
        target = i;
        target *= target;
        tolerance = target * D_800B0C40;
        target *= target;
        do {
            xSq = x * x;
            step = (xSq * x - target) / (2.0f * xSq);
            x -= step;
        } while (step > tolerance || step < -tolerance);
        D_800E1078[i] = x;
    }
    gain = D_800B0C44;
    subblockGain = 0.25f;
    D_800E1080[0] = 1.0f;
    D_800E1480[0] = 1.0f;
    for (i = 1; i < 256; i++) {
        D_800E1080[i] = gain;
        D_800E1480[i] = subblockGain;
        subblockGain *= 0.25f;
        gain *= D_800B0C48;
    }
    return 1;
}

/* Dequantisation: xr = sign * |val|^(4/3) * 2^((globalGain - 210) / 4) * the
 * band's scalefactor (and, for short windows, subblock-gain) power, walking
 * the long / short band boundaries; lines past nonZero are cleared. */
s32 func_151F42E8(Mp3Decoder *dec, s32 gr, s32 ch) {
    s32 bandTable;
    s32 bandEnd;
    s32 windowStart;
    s32 windowWidth;
    Mp3Pretab pretab = D_800B0AB4;
    f32 longGain[22];
    f32 shortGain[3][13];
    f32 globalGain;
    s32 sfb;
    s32 window;
    s32 scalefacScale;
    s32 subblockGain;
    s32 exponent;
    s32 line;
    s32 lineCount;
    f32 *xr;
    s16 *val;
    u8 *sign;
    s32 pureShort;
    s32 mixedShort;
    s32 windowSwitching;
    s32 windowEnd;
    f32 *gains;

    bandTable = dec->sampleRateIndex + dec->id * 3;
    if (dec->windowSwitching[gr][ch] != 0 && dec->blockType[gr][ch] == 2) {
        if (dec->mixedBlock[gr][ch] != 0) {
            bandEnd = D_800AE998[bandTable].l[1];
        } else {
            bandEnd = D_800AE998[bandTable].s[1] * 3;
            windowWidth = D_800AE998[bandTable].s[1];
            windowStart = 0;
        }
    } else {
        bandEnd = D_800AE998[bandTable].l[1];
    }
    globalGain = func_1504A400(2.0f, (dec->globalGain[gr][ch] - 210.0f) * 0.25f);
    scalefacScale = dec->scalefacScale[gr][ch];
    for (sfb = 0; sfb < 22; sfb++) {
        exponent = (dec->scalefac[gr][ch].l[sfb] +
                dec->preflag[gr][ch] * pretab.band[sfb]) * (scalefacScale + 1);
        longGain[sfb] = D_800E1080[exponent] * globalGain;
    }
    for (window = 0; window < 3; window++) {
        for (sfb = 0; sfb < 13; sfb++) {
            subblockGain = dec->subblockGain[gr][ch][window];
            exponent = dec->scalefac[gr][ch].s[window][sfb] * (scalefacScale + 1);
            shortGain[window][sfb] = globalGain * D_800E1480[subblockGain] * D_800E1080[exponent];
        }
    }
    sfb = 0;
    line = 0;
    lineCount = dec->nonZero[ch];
    xr = dec->xr[ch];
    val = dec->huffVal[ch];
    sign = dec->huffSign[ch];
    pureShort = (dec->blockType[gr][ch] == 2) && (dec->mixedBlock[gr][ch] == 0);
    mixedShort = (dec->blockType[gr][ch] == 2) && (dec->mixedBlock[gr][ch] != 0);
    windowSwitching = dec->windowSwitching[gr][ch];
    while (line < lineCount) {
        if (bandEnd > lineCount) {
            bandEnd = lineCount;
        }
        if (windowSwitching && (pureShort || (mixedShort && line >= 36))) {
            window = (line - windowStart) / windowWidth;
            windowEnd = windowStart + windowWidth;
        }
        while (line < bandEnd) {
            if (windowSwitching && (pureShort || (mixedShort && line >= 36))) {
                if (line >= windowEnd) {
                    windowEnd += windowWidth;
                    window++;
                }
                gains = shortGain[window];
                if (*sign++) {
                    *xr++ = -(D_800E1078[*val++] * gains[sfb]);
                } else {
                    *xr++ = D_800E1078[*val++] * gains[sfb];
                }
            } else {
                if (*sign++) {
                    *xr++ = -(D_800E1078[*val++] * longGain[sfb]);
                } else {
                    *xr++ = D_800E1078[*val++] * longGain[sfb];
                }
            }
            line++;
        }
        if (dec->windowSwitching[gr][ch] != 0 && dec->blockType[gr][ch] == 2) {
            if (dec->mixedBlock[gr][ch] != 0) {
                if (D_800AE998[bandTable].l[8] == line) {
                    bandEnd = D_800AE998[bandTable].s[4] * 3;
                    sfb = 3;
                    windowWidth = D_800AE998[bandTable].s[sfb + 1] - D_800AE998[bandTable].s[sfb];
                    windowStart = D_800AE998[bandTable].s[sfb] * 3;
                } else if (line < D_800AE998[bandTable].l[8]) {
                    bandEnd = D_800AE998[bandTable].l[++sfb + 1];
                } else {
                    bandEnd = D_800AE998[bandTable].s[++sfb + 1] * 3;
                    windowWidth = D_800AE998[bandTable].s[sfb + 1] - D_800AE998[bandTable].s[sfb];
                    windowStart = D_800AE998[bandTable].s[sfb] * 3;
                }
            } else {
                bandEnd = D_800AE998[bandTable].s[++sfb + 1] * 3;
                windowWidth = D_800AE998[bandTable].s[sfb + 1] - D_800AE998[bandTable].s[sfb];
                windowStart = D_800AE998[bandTable].s[sfb] * 3;
            }
        } else {
            bandEnd = D_800AE998[bandTable].l[++sfb + 1];
        }
    }
    if (line < 576) {
        bzero(xr, dec->zeroTail[ch] * sizeof(*xr));
        // matching: -g emits a branch per return; the ROM has this extra one
        return 1;
    }
    return 1;
}

/* MPEG-1 scalefactors: short / mixed blocks read slen1 for the low bands and
 * slen2 above; long blocks read the four scfsi groups, reusing granule 0's
 * values for a group whose scfsi bit is set. */
s32 func_151F4F38(Mp3Decoder *dec, s32 gr, s32 ch) {
    s32 part;
    s32 sfb;
    s32 window;

    if (dec->windowSwitching[gr][ch] != 0 && dec->blockType[gr][ch] == 2) {
        if (dec->mixedBlock[gr][ch] != 0) {
            for (sfb = 0; sfb < 8; sfb++) {
                dec->scalefac[gr][ch].l[sfb] =
                    GET_MAIN_BITS(dec, D_800AEB5C[0][dec->scalefacCompress[gr][ch]]);
            }
            for (sfb = 3; sfb < 6; sfb++) {
                for (window = 0; window < 3; window++) {
                    dec->scalefac[gr][ch].s[window][sfb] =
                        GET_MAIN_BITS(dec, D_800AEB5C[0][dec->scalefacCompress[gr][ch]]);
                }
            }
            for (sfb = 6; sfb < 12; sfb++) {
                for (window = 0; window < 3; window++) {
                    dec->scalefac[gr][ch].s[window][sfb] =
                        GET_MAIN_BITS(dec, D_800AEB5C[1][dec->scalefacCompress[gr][ch]]);
                }
            }
        } else {
            for (part = 0; part < 2; part++) {
                for (sfb = D_800AEB54[part + 5]; sfb < D_800AEB54[part + 6]; sfb++) {
                    for (window = 0; window < 3; window++) {
                        dec->scalefac[gr][ch].s[window][sfb] =
                            GET_MAIN_BITS(dec, D_800AEB5C[part][dec->scalefacCompress[gr][ch]]);
                    }
                }
            }
        }
        for (window = 0; window < 3; window++) {
            dec->scalefac[gr][ch].s[window][12] = 0;
        }
    } else {
        for (part = 0; part < 4; part++) {
            if (dec->scfsi[ch][part] == 0 || gr == 0) {
                for (sfb = D_800AEB54[part]; sfb < D_800AEB54[part + 1]; sfb++) {
                    dec->scalefac[gr][ch].l[sfb] = GET_MAIN_BITS(
                        dec, D_800AEB5C[(part < 2) ? 0 : 1][dec->scalefacCompress[gr][ch]]);
                }
            } else {
                for (sfb = D_800AEB54[part]; sfb < D_800AEB54[part + 1]; sfb++) {
                    dec->scalefac[1][ch].l[sfb] = dec->scalefac[0][ch].l[sfb];
                }
            }
        }
        dec->scalefac[gr][ch].l[22] = 0;
    }
    return 1;
}

/* MPEG-2 LSF scalefactors (ISO 13818-3): scalefacCompress selects the slen of
 * each of four band groups and, through D_800B0B0C, how many bands each holds.
 * The intensity-stereo right channel uses its own split and records each
 * band's illegal intensity position in unk3EFC / unk3F14. Only granule 0
 * exists in LSF, so gr is unused. */
s32 func_151F578C(Mp3Decoder *dec, s32 gr, s32 ch) {
    Mp3SfbCounts counts = D_800B0B0C;
    s32 slen[4];
    s32 sfCompress = dec->scalefacCompress[0][ch];
    s32 *preflag = &dec->preflag[0][ch];
    s32 mixedBlock = dec->mixedBlock[0][ch];
    s32 blockType = dec->blockType[0][ch];
    s32 windowSwitching = dec->windowSwitching[0][ch];
    s32 isRight;
    s32 slenClass;
    s32 part;
    s32 i;
    s32 isCompress;
    s32 nrSfb[4];
    s32 sfb;
    s32 nbits;
    s32 maxPos;

    if ((dec->modeExtension != 1 && dec->modeExtension != 3) || ch != 1) {
        isRight = 0;
        if (sfCompress < 400) {
            slen[0] = (sfCompress >> 4) / 5;
            slen[1] = (sfCompress >> 4) % 5;
            slen[2] = (sfCompress % 16) >> 2;
            slen[3] = sfCompress % 4;
            *preflag = 0;
            slenClass = 0;
        } else if (sfCompress >= 400 && sfCompress < 500) {
            slen[0] = ((sfCompress - 400) >> 2) / 5;
            slen[1] = ((sfCompress - 400) >> 2) % 5;
            slen[2] = (sfCompress - 400) % 4;
            slen[3] = 0;
            *preflag = 0;
            slenClass = 1;
        } else if (sfCompress >= 500 && sfCompress < 512) {
            slen[0] = (sfCompress - 500) / 3;
            slen[1] = (sfCompress - 500) % 3;
            slen[2] = 0;
            slen[3] = 0;
            *preflag = 1;
            slenClass = 2;
        }
    }
    if ((dec->modeExtension == 1 || dec->modeExtension == 3) && ch == 1) {
        dec->intensityScale = sfCompress % 2;
        isCompress = sfCompress >> 1;
        isRight = 1;
        if (isCompress < 180) {
            slen[0] = isCompress / 36;
            slen[1] = (isCompress % 36) / 6;
            slen[2] = (isCompress % 36) % 6;
            slen[3] = 0;
            *preflag = 0;
            slenClass = 0;
        } else if (isCompress >= 180 && isCompress < 244) {
            slen[0] = ((isCompress - 180) % 64) >> 4;
            slen[1] = ((isCompress - 180) % 16) >> 2;
            slen[2] = (isCompress - 180) % 4;
            slen[3] = 0;
            *preflag = 0;
            slenClass = 1;
        } else if (isCompress >= 244 && isCompress < 255) {
            slen[0] = (isCompress - 244) / 3;
            slen[1] = (isCompress - 244) % 3;
            slen[2] = 0;
            slen[3] = 0;
            *preflag = 0;
            slenClass = 2;
        }
    }
    if (windowSwitching != 0 && blockType == 2) {
        bcopy(counts.nrSfb[isRight][slenClass][mixedBlock + 1], nrSfb, sizeof(nrSfb));
    } else {
        bcopy(counts.nrSfb[isRight][slenClass][0], nrSfb, sizeof(nrSfb));
    }
    sfb = 0;
    if (windowSwitching == 0 || (windowSwitching != 0 && blockType != 2)) {
        for (part = 0; part < 4; part++) {
            nbits = slen[part];
            maxPos = (1 << nbits) - 1;
            for (i = 0; i < nrSfb[part]; i++) {
                dec->scalefac[0][ch].l[sfb] = GET_MAIN_BITS(dec, nbits);
                if (ch != 0) {
                    dec->unk3EFC[sfb] = maxPos;
                }
                sfb++;
            }
        }
    } else if (blockType == 2) {
        if (mixedBlock == 0) {
            for (part = 0; part < 4; part++) {
                nbits = slen[part];
                maxPos = (1 << nbits) - 1;
                for (i = 0; i < nrSfb[part]; i += 3) {
                    dec->scalefac[0][ch].s[0][sfb] = GET_MAIN_BITS(dec, nbits);
                    dec->scalefac[0][ch].s[1][sfb] = GET_MAIN_BITS(dec, nbits);
                    dec->scalefac[0][ch].s[2][sfb] = GET_MAIN_BITS(dec, nbits);
                    if (ch != 0) {
                        dec->unk3F14[sfb] = maxPos;
                    }
                    sfb++;
                }
            }
        } else {
            nbits = slen[0];
            maxPos = (1 << nbits) - 1;
            for (i = 0; i < 6; i++) {
                dec->scalefac[0][ch].l[sfb] = GET_MAIN_BITS(dec, nbits);
                if (ch != 0) {
                    dec->unk3EFC[sfb] = maxPos;
                }
                sfb++;
            }
            nrSfb[0] -= 6;
            sfb = 3;
            for (part = 0; part < 4; part++) {
                nbits = slen[part];
                maxPos = (1 << nbits) - 1;
                for (i = 0; i < nrSfb[part]; i += 3) {
                    dec->scalefac[0][ch].s[0][sfb] = GET_MAIN_BITS(dec, nbits);
                    dec->scalefac[0][ch].s[1][sfb] = GET_MAIN_BITS(dec, nbits);
                    dec->scalefac[0][ch].s[2][sfb] = GET_MAIN_BITS(dec, nbits);
                    if (ch != 0) {
                        dec->unk3F14[sfb] = maxPos;
                    }
                    sfb++;
                }
            }
        }
    }
    return 1;
}

/* Reads the granule's scalefactors, then Huffman-decodes its big-value regions
 * (zero-filling regions whose table is empty) and count1 region into huffVal /
 * huffSign, padding the rest of the 576 lines with zeros. */
s32 func_151F63C4(Mp3Decoder *dec, s32 gr, s32 ch) {
    s32 part2Start;
    s32 bigValueEnd;
    s32 regionEnd[3];
    s16 *longBands;
    u8 *shortBands;
    s32 line;
    s16 *val;
    u8 *sign;
    s32 region;
    s32 table;
    s32 linbits;
    s32 end;
    s32 count;
    s32 part23End;

    part2Start = dec->mainDataBit;
    if (dec->id != 0) {
        func_151F4F38(dec, gr, ch);
    } else {
        func_151F578C(dec, gr, ch);
    }
    bigValueEnd = dec->bigValues[gr][ch] * 2;
    longBands = D_800AE840[dec->id][dec->sampleRateIndex];
    shortBands = D_800AE948[dec->id][dec->sampleRateIndex];
    if (dec->windowSwitching[gr][ch] == 0 && dec->blockType[gr][ch] == 0) {
        regionEnd[0] = longBands[dec->region0Count[gr][ch]] + 1;
        if (regionEnd[0] > bigValueEnd) {
            regionEnd[0] = regionEnd[1] = bigValueEnd;
        } else {
            regionEnd[1] = longBands[dec->region0Count[gr][ch] + dec->region1Count[gr][ch] + 1] + 1;
            if (regionEnd[1] > bigValueEnd) {
                regionEnd[1] = bigValueEnd;
            }
        }
    } else {
        if (dec->blockType[gr][ch] == 2 && dec->mixedBlock[gr][ch] == 0) {
            regionEnd[0] = shortBands[2] * 3 + 3;
        } else {
            regionEnd[0] = longBands[7] + 1;
        }
        if (regionEnd[0] > bigValueEnd) {
            regionEnd[0] = bigValueEnd;
        }
        regionEnd[1] = bigValueEnd;
    }
    regionEnd[2] = bigValueEnd;
    line = 0;
    val = dec->huffVal[ch];
    sign = dec->huffSign[ch];
    for (region = 0; region < 3; region++) {
        table = dec->tableSelect[gr][ch][region];
        linbits = D_800AE7B8[table];
        end = regionEnd[region];
        if (D_800E0E20[table] == 0) {
            count = end - line;
            bzero(val, count * sizeof(*val));
            val += count;
            sign += count;
            line = end;
        } else {
            line = func_151F8994(dec->mainData, &dec->mainDataBit, table, line, linbits, end, &val,
                                 &sign);
        }
    }
    table = dec->count1TableSelect[gr][ch] + 32; /* count1 tables A/B are 32, 33 */
    part23End = dec->part23Length[gr][ch] + part2Start;
    line = func_151F8B4C(dec->mainData, &dec->mainDataBit, table, line, part23End, &val, &sign);
    dec->mainDataBit = part23End;
    if (line > 576) {
        dec->nonZero[ch] = 576;
    } else {
        dec->nonZero[ch] = line;
    }
    if (line < 576) {
        dec->zeroTail[ch] = 576 - line;
        bzero(val, dec->zeroTail[ch] * sizeof(*val));
    } else {
        dec->zeroTail[ch] = 0;
    }
    return 1;
}

/* Short-block reordering: xr is copied into xrSb, short blocks through the
 * D_800AEB7C index table (mixed blocks keep their first 36 long lines). */
s32 func_151F6970(Mp3Decoder *dec, s32 gr) {
    s16 *reorder;
    f32 *dst;
    f32 *src;
    s32 i;

    reorder = D_800AEB7C[dec->id][dec->sampleRateIndex];
    dst = dec->xrSb[0][0];
    src = dec->xr[0];
    i = 0;
    if (dec->windowSwitching[gr][0] != 0 && dec->blockType[gr][0] == 2) {
        if (dec->mixedBlock[gr][0] != 0) {
            while (i++ < 36) {
                *dst++ = *src++;
            }
        }
        while (i < 576) {
            dst[reorder[i++]] = *src++;
        }
    } else {
        while (i++ < 576) {
            *dst++ = *src++;
        }
    }
    return 1;
}

/* Alias reduction: the eight butterflies between each pair of adjacent long-block
 * subbands (skipped for short blocks). Written out in full; a loop compiles
 * differently. */
s32 func_151F6B28(Mp3Decoder *dec, s32 gr, s32 ch) {
    s32 sb;
    f32 *xr;
    f32 bd;
    f32 bu;

    if (dec->windowSwitching[gr][ch] != 0) {
        if (dec->blockType[gr][ch] == 2) {
            return 1;
        }
    }

    for (sb = 1; sb < 32; sb++) {
        xr = dec->xrSb[ch][sb];
        bd = xr[0];
        bu = xr[-1];
        xr[-1] = bu * D_800B069C[0] - D_800B067C[0] * bd;
        xr[0] = bd * D_800B069C[0] + D_800B067C[0] * bu;

        bd = xr[1];
        bu = xr[-2];
        xr[-2] = bu * D_800B069C[1] - D_800B067C[1] * bd;
        xr[1] = bd * D_800B069C[1] + D_800B067C[1] * bu;

        bd = xr[2];
        bu = xr[-3];
        xr[-3] = bu * D_800B069C[2] - D_800B067C[2] * bd;
        xr[2] = bd * D_800B069C[2] + D_800B067C[2] * bu;

        bd = xr[3];
        bu = xr[-4];
        xr[-4] = bu * D_800B069C[3] - D_800B067C[3] * bd;
        xr[3] = bd * D_800B069C[3] + D_800B067C[3] * bu;

        bd = xr[4];
        bu = xr[-5];
        xr[-5] = bu * D_800B069C[4] - D_800B067C[4] * bd;
        xr[4] = bd * D_800B069C[4] + D_800B067C[4] * bu;

        bd = xr[5];
        bu = xr[-6];
        xr[-6] = bu * D_800B069C[5] - D_800B067C[5] * bd;
        xr[5] = bd * D_800B069C[5] + D_800B067C[5] * bu;

        bd = xr[6];
        bu = xr[-7];
        xr[-7] = bu * D_800B069C[6] - D_800B067C[6] * bd;
        xr[6] = bd * D_800B069C[6] + D_800B067C[6] * bu;

        bd = xr[7];
        bu = xr[-8];
        xr[-8] = bu * D_800B069C[7] - D_800B067C[7] * bd;
        xr[7] = bd * D_800B069C[7] + D_800B067C[7] * bu;
    }

    return 1;
}

/* Reads the side info that follows the frame header and derives the frame's
 * main-data size. Returns 0 on a short read. */
s32 func_151F6FD0(Mp3Decoder *dec) {
    s32 got;
    s32 ch;
    s32 band;
    s32 scalefacBits;
    s32 gr;
    s32 region;
    s32 window;

    if (dec->id != 0) {
        dec->sideInfoLen = (dec->mode == 3) ? 17 : 32;
    } else {
        dec->sideInfoLen = (dec->mode == 3) ? 9 : 17;
    }
    got = dec->read(dec->handle, &dec->headerBuf[dec->headerLen], dec->sideInfoLen, -1);
    if (dec->sideInfoLen != got) {
        return 0;
    }
    dec->streamPos += dec->sideInfoLen;
    dec->nch = (dec->mode == 3) ? 1 : 2;
    dec->ngr = (dec->id != 0) ? 2 : 1;
    if (dec->id != 0) {
        dec->mainDataBegin = GET_HEADER_BITS(dec, 9);
        // matching: skips the private bits as a ternary statement; an if/else differs
        (dec->nch == 1) ? GET_HEADER_BITS(dec, 5) : GET_HEADER_BITS(dec, 3);
    } else {
        dec->mainDataBegin = GET_HEADER_BITS(dec, 8);
        // matching: skips the private bits as a ternary statement; an if/else differs
        (dec->nch == 1) ? GET_HEADER_BITS(dec, 1) : GET_HEADER_BITS(dec, 2);
    }
    if (dec->id != 0) {
        for (ch = 0; ch < dec->nch; ch++) {
            for (band = 0; band < 4; band++) {
                dec->scfsi[ch][band] = GET_HEADER_BITS(dec, 1);
            }
        }
    }
    if (dec->id != 0) {
        scalefacBits = 4;
    } else {
        scalefacBits = 9;
    }
    for (gr = 0; gr < dec->ngr; gr++) {
        for (ch = 0; ch < dec->nch; ch++) {
            dec->part23Length[gr][ch] = GET_HEADER_BITS(dec, 12);
            dec->bigValues[gr][ch] = GET_HEADER_BITS(dec, 9);
            dec->globalGain[gr][ch] = GET_HEADER_BITS(dec, 8);
            dec->scalefacCompress[gr][ch] =
                (scalefacBits != 0) ? GET_HEADER_BITS(dec, scalefacBits) : 0;
            dec->windowSwitching[gr][ch] = GET_HEADER_BITS(dec, 1);
            if (dec->windowSwitching[gr][ch] != 0) {
                dec->blockType[gr][ch] = GET_HEADER_BITS(dec, 2);
                dec->mixedBlock[gr][ch] = GET_HEADER_BITS(dec, 1);
                for (region = 0; region < 2; region++) {
                    dec->tableSelect[gr][ch][region] = GET_HEADER_BITS(dec, 5);
                }
                dec->tableSelect[gr][ch][2] = 0;
                for (window = 0; window < 3; window++) {
                    dec->subblockGain[gr][ch][window] = GET_HEADER_BITS(dec, 3);
                }
            } else {
                dec->blockType[gr][ch] = 0;
                dec->mixedBlock[gr][ch] = 0;
                for (region = 0; region < 3; region++) {
                    dec->tableSelect[gr][ch][region] = GET_HEADER_BITS(dec, 5);
                }
                dec->region0Count[gr][ch] = GET_HEADER_BITS(dec, 4);
                dec->region1Count[gr][ch] = GET_HEADER_BITS(dec, 3);
            }
            if (dec->id != 0) {
                dec->preflag[gr][ch] = GET_HEADER_BITS(dec, 1);
            }
            dec->scalefacScale[gr][ch] = GET_HEADER_BITS(dec, 1);
            dec->count1TableSelect[gr][ch] = GET_HEADER_BITS(dec, 1);
        }
    }
    dec->bitrate = D_800B06BC[dec->id][dec->bitrateIndex];
    dec->sampleRate = D_800B0734[dec->id][dec->sampleRateIndex];
    if (dec->id != 0) {
        dec->frameSize = dec->bitrate * 144 / dec->sampleRate;
    } else {
        dec->frameSize = dec->bitrate * 72 / dec->sampleRate;
    }
    dec->mainDataSize = dec->frameSize + dec->paddingBit - (dec->headerLen + dec->sideInfoLen);
    return 1;
}

/* Layer III frame decode: reads the main data, then per channel Huffman-decodes,
 * dequantises, reorders, alias-reduces and runs the IMDCT, and writes a block of
 * s16 subband samples (after a 2-word scale header) to the current outBuf. */
s32 func_151F78B4(Mp3Decoder *dec) {
    s32 mainDataStart;
    s32 gr;
    s32 ch;
    s32 sbLimit;
    s32 sbUsed;
    s32 blockType;
    s32 sb;
    s32 ss;
    s16 *out;
    f32 hybrid[32][18];
    f32 scale;
    f32 sampleScale;
    f32 gain;
    s32 sample;
    s32 fixedScale;

    gr = 0;
    out = (s16 *)dec->outBuf[dec->outIndex];
    mainDataStart = func_151F7F60(dec);
    if (mainDataStart == 0) {
        return 0;
    }
    dec->mainDataBit = (mainDataStart - dec->mainDataBegin) * 8;
    if (dec->mainDataBit < 0) {
        return 1;
    }
    for (ch = 0; ch < dec->nch; ch++) {
        func_151F63C4(dec, gr, ch);
        func_151F42E8(dec, gr, ch);
    }
    func_151F6970(dec, gr);
    if (dec->windowSwitching[gr][0] != 0 && dec->blockType[gr][0] == 2) {
        sbLimit = 32;
    } else {
        sbUsed = (dec->nonZero[0] - 1) / 18 + 1;
        sbLimit = sbUsed;
    }
    for (ch = 0; ch < dec->nch; ch++) {
        func_151F6B28(dec, gr, ch);
        if (dec->windowSwitching[gr][ch] != 0 && dec->blockType[gr][ch] == 2 &&
            dec->mixedBlock[gr][ch] != 0) {
            blockType = 0;
        } else if (dec->windowSwitching[gr][ch] == 0) {
            blockType = 0;
        } else {
            blockType = dec->blockType[gr][ch];
        }
        if (blockType == 2) {
            for (sb = 0; sb < 2; sb++) {
                func_151F9BF0(dec->xrSb[ch][sb], sb, hybrid[sb], dec->overlap[ch][sb]);
            }
        } else {
            for (sb = 0; sb < 2; sb++) {
                func_151F8CF0(dec->xrSb[ch][sb], sb, hybrid[sb],
                              dec->overlap[ch][sb], D_800E0E38[blockType]);
            }
        }
        if (dec->windowSwitching[gr][ch] != 0 && dec->blockType[gr][ch] == 2 &&
            dec->mixedBlock[gr][ch] != 0) {
            blockType = 2;
        }
        if (blockType == 2) {
            for (sb = 2; sb < sbLimit; sb++) {
                func_151F9BF0(dec->xrSb[ch][sb], sb, hybrid[sb], dec->overlap[ch][sb]);
            }
        } else {
            for (sb = 2; sb < sbLimit; sb++) {
                func_151F8CF0(dec->xrSb[ch][sb], sb, hybrid[sb],
                              dec->overlap[ch][sb], D_800E0E38[blockType]);
            }
        }
        while (sb < 32) {
            bcopy(dec->overlap[ch][sb], hybrid[sb], sizeof(hybrid[sb]));
            bzero(dec->overlap[ch][sb], sizeof(dec->overlap[ch][sb]));
            sb++;
        }
        scale = 65536.0f;
        gain = 1.0f;
        fixedScale = scale * gain * 16.0f;
        *out++ = fixedScale >> 16;
        *out++ = fixedScale & 0xFFFF;
        fixedScale = -fixedScale;
        *out++ = fixedScale >> 16;
        *out++ = fixedScale & 0xFFFF;
        sampleScale = 2048.0f / gain;
        for (ss = 0; ss < 18; ss++) {
            for (sb = 0; sb < 32; sb++) {
                sample = hybrid[sb][ss] * sampleScale;
                *out++ = sample;
            }
        }
    }
    return 1;
}

/* Appends this frame's main data to the bit reservoir, first sliding the
 * buffer down by half when it would overflow. Returns the byte offset of the
 * new data. */
s32 func_151F7F60(Mp3Decoder *dec) {
    s32 half;
    s32 got;

    half = 0x1000;
    if (dec->mainDataLen + dec->mainDataSize >= 0x1FFC) {
        bcopy(&dec->mainData[half], dec->mainData, half);
        dec->mainDataLen -= half;
        dec->mainDataBit -= half * 8;
    }
    got = dec->read(dec->handle, &dec->mainData[dec->mainDataLen], dec->mainDataSize, -1);
    if (got < dec->mainDataSize) {
        /* zero-fills from mainData[got], not from the read position */
        bzero(&dec->mainData[got], dec->mainDataSize - got);
    }
    dec->streamPos += dec->mainDataSize;
    dec->mainDataLen += dec->mainDataSize;
    return dec->mainDataLen - dec->mainDataSize;
}

/* Finds the next frame sync (optionally seeking to seekPos first, -1 to
 * continue), parses the 4-byte header and CRC, skips frames whose header is
 * invalid or differs from the first one, selects the layer's decoder and reads
 * the side info. Returns 0 at end of stream or on an unsupported layer. */
s32 func_151F8088(Mp3Decoder *dec, s32 seekPos) {
    s32 pos;
    s32 syncLen;
    s32 got;
    u8 mask;

    if (seekPos != -1) {
        dec->streamPos = seekPos;
    }
    pos = seekPos;
    syncLen = 0;
    mask = 0xFF;
    while (1) {
        got = dec->read(dec->handle, &dec->headerBuf[syncLen], 1, pos);
        if (got <= 0) {
            return 0;
        }
        pos = -1;
        dec->streamPos++;
        if (dec->headerBuf[syncLen] != 0xFF && dec->headerBuf[syncLen] != 0xF3) {
            return 0;
        }
        if ((dec->headerBuf[syncLen] & mask) != mask) {
            mask = 0xFF;
            syncLen = 0;
            continue;
        }
        syncLen++;
        if (mask == 0xF0) {
            break;
        }
        mask = 0xF0;
    }
    got = dec->read(dec->handle, &dec->headerBuf[2], 2, -1);
    if (got <= 0) {
        return 0;
    }
    dec->streamPos += 2;
    dec->headerBit = 12; /* past the sync word */
    dec->id = GET_HEADER_BITS(dec, 1);
    dec->layer = GET_HEADER_BITS(dec, 2);
    dec->protectionBit = GET_HEADER_BITS(dec, 1);
    dec->bitrateIndex = GET_HEADER_BITS(dec, 4);
    dec->sampleRateIndex = GET_HEADER_BITS(dec, 2);
    dec->paddingBit = GET_HEADER_BITS(dec, 1);
    dec->privateBit = GET_HEADER_BITS(dec, 1);
    dec->mode = GET_HEADER_BITS(dec, 2);
    dec->modeExtension = GET_HEADER_BITS(dec, 2);
    dec->copyright = GET_HEADER_BITS(dec, 1);
    dec->original = GET_HEADER_BITS(dec, 1);
    dec->emphasis = GET_HEADER_BITS(dec, 2);
    if (dec->bitrateIndex == 15 || dec->sampleRateIndex == 3) {
        return func_151F8088(dec, -1);
    }
    if (dec->haveFirstHeader == 0) {
        dec->haveFirstHeader = 1;
        dec->firstId = dec->id;
        dec->firstLayer = dec->layer;
        dec->firstProtectionBit = dec->protectionBit;
        dec->firstSampleRateIndex = dec->sampleRateIndex;
        dec->firstMode = dec->mode;
        dec->firstCopyright = dec->copyright;
        dec->firstOriginal = dec->original;
    } else if (dec->id != dec->firstId || dec->layer != dec->firstLayer ||
               dec->protectionBit != dec->firstProtectionBit ||
               dec->sampleRateIndex != dec->firstSampleRateIndex ||
               dec->mode != dec->firstMode || dec->original != dec->firstOriginal) {
        return func_151F8088(dec, -1);
    }
    dec->headerLen = 4;
    if (dec->protectionBit == 0) {
        got = dec->read(dec->handle, &dec->headerBuf[4], 2, -1);
        if (got <= 0) {
            return 0;
        }
        dec->streamPos += 2;
        dec->headerBit += 16;
        dec->headerLen = 6;
    }
    if (dec->layer == 1) {
        dec->decodeFrame = func_151F78B4;
        dec->readSideInfo = func_151F6FD0;
    } else if (dec->layer == 2) {
        return 0;
    } else if (dec->layer == 3) {
        return 0;
    }
    if (dec->readSideInfo(dec) == 0) {
        return 0;
    }
    return 1;
}

/* Opens the (single, static) decoder on a stream: sync to the first frame and
 * clear the IMDCT overlap state. Returns NULL if no frame is found. */
Mp3Decoder *func_151F85C4(s32 handle, Mp3ReadFn read, s32 arg2) {
    Mp3Decoder *dec;

    dec = &D_800E1880;
    if (dec == NULL) {
        return NULL;
    }
    dec->unk000C = -1;
    dec->unk0010 = -1;
    dec->unk0014 = -1;
    dec->handle = handle;
    dec->read = read;
    dec->unk0008 = arg2;
    dec->mainDataLen = 0;
    dec->mainDataBit = 0;
    dec->outIndex = 0;
    if (func_151F8088(dec, 0) == 0) {
        return NULL;
    }
    dec->seekPos = 0;
    bzero(dec->overlap, sizeof(dec->overlap[0]));
    return dec;
}

/* Decodes the next frame into the next outBuf slot and returns it through
 * outBlock / outChannels. A frame with the copyright bit set is followed by a
 * NUL-terminated tag string, handed to the D_800E0E00 callback. */
s32 func_151F86B0(Mp3Decoder *decoder, u8 **outBlock, s32 *outChannels) {
    Mp3Decoder *dec;
    s32 ret;
    u8 tag[0x100];
    s32 i;

    // matching: the parameter is copied into a local
    dec = decoder;
    dec->outIndex++;
    if (dec->outIndex >= 6) {
        dec->outIndex = 0;
    }
    if (func_151F8088(dec, dec->seekPos) == 0) {
        D_800E0E04 = 3;
        return 0;
    }
    dec->seekPos = -1;
    ret = dec->decodeFrame(dec);
    if (ret == 0) {
        // matching: a goto to the shared return; an if-block or early return is a jump shorter
        goto end;
    }
    *outBlock = dec->outBuf[dec->outIndex];
    *outChannels = dec->nch;
    if (dec->copyright != 0) {
        i = 0;
        do {
            if (dec->read(dec->handle, &tag[i], 1, -1) == 0) {
                break;
            }
        } while (tag[i++]);
        if (D_800E0E00 != NULL) {
            D_800E0E00(0, tag, strlen(tag) + 1);
        }
    }
end:
    return ret;
}
