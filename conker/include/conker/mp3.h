#ifndef CONKER_MP3_H
#define CONKER_MP3_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "mp3": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init/Audio/Mp3Stream/mp3stream.c
 *   game/Audio/Mp3Voice/mp3voice.c
 *   game/Audio/Mp3Player/mp3player.c
 *   game/Audio/Mp3Decoder/mp3decoder.c
 *   game/Audio/Mp3Frame/mp3frame.c
 *   game/Audio/Mp3Decoder/mp3huffman.s
 *   game/Audio/Mp3Decoder/mp3imdctlong.s
 *   game/Audio/Mp3Decoder/mp3imdctshort.s
 */

/* Types that these files define; the declarations below use them. */
typedef struct Mp3Decoder Mp3Decoder;
typedef s32 (*Mp3ReadFn)(s32 handle, void *dst, s32 size, s32 seekPos);
typedef s32 (*Mp3DecodeFn)(Mp3Decoder *dec);

typedef struct {
    /* 0x00 */ s32 l[23];    /* long-block scalefactors, per band */
    /* 0x5C */ s32 s[3][13]; /* short-block scalefactors, [window][band] */
} Mp3ScaleFac; /* size 0xF8 */

/* The decoder state of Rare's MPEG audio layer III decoder (the streamed-audio codec). One global
 * instance, gMp3Decoder (game/Audio/Mp3Frame/mp3frame.c). Field names follow ISO 11172-3 / 13818-3 where
 * the header and side-info bit widths read in mp3_syncFrame / mp3_readSideInfo pin them down.
 *
 * The ROM's address arithmetic makes several per-granule / per-channel arrays
 * overlap their neighbours, so their bounds below are the largest that do not
 * collide: the side-info arrays are [2][1] because [gr][ch] both stride one
 * element; scfsi[1] would run into part23Length, huffVal[1] starts at huffSign,
 * nonZero[1] is zeroTail[0], and xr[1] is xrSb.  Only gr 0 of a mono stream is
 * safe from the overlaps. */
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

/* n bits of the frame header and side info (headerBuf), through mp3_getBits */
#define GET_HEADER_BITS(dec, n) mp3_getBits((dec)->headerBuf, &(dec)->headerBit, (n))

/* init/Audio/Mp3Stream/mp3stream.c */
void mp3_pushMessage(s32 unused, void *message, s32 size);
void mp3_initStream(ALHeap *heap);
void mp3_stopForLevel(s32 levelId);
void mp3_startTrack(s32 id, s32 volume, s32 pan);
s32 mp3_popMessage(u8 *dst, s32 dstSize);
s32 mp3_startTrackAt(u16 id, Actor *obj, s32 volume, s16 nearDistance, u16 farDistance);
s32 mp3_isPlaying(void);
extern s32 D_800427F0;
extern u16 gMp3LastTrack; /* the track last started */

/* game/Audio/Mp3Voice/mp3voice.c */
void mp3_initVoice(ALHeap *arg0);

/* game/Audio/Mp3Player/mp3player.c: the streaming music player (StreamVoice D_800E0D80) */
extern StreamVoice D_800E0D80; /* the streaming music player's voice */
extern s32 *D_800E0E20;        /* Huffman table pointers, relocated by D_800E0E28 */
extern void *D_800E0E28;       /* the Huffman table data */
void mp3_playStream(s32 data, s32 size);
void mp3_stop(void);
void mp3_pause(void);
void mp3_resume(void);
s32 mp3_getPlayState(void);
void mp3_setVolume(s32 volume, s32 arg1);
void mp3_setPan(s32 pan, s32 now);
void mp3_setLowPass(s32 arg0, s32 arg1);
s32 mp3_pullFrame(s32 nSamples, Acmd **cmdList);
void mp3_updateVolumeRamp(StreamVoice *v);
void mp3_setTagCallback(void (*callback)(s32, void *, s32));
/* (Mp3ReadFn read): sets the stream's read callback; declared without a prototype: mp3_initVoice (game/Audio/Mp3Voice/mp3voice.c)
 * passes it with the volume and the pan */
void mp3_setReadFn();
s32 mp3_readStream(s32 arg0, void *dst, s32 len, s32 offset);
void mp3_prefetchStream(void);

/* game/Audio/Mp3Decoder/mp3decoder.c */
s32 mp3_initTables(void);
s32 mp3_dequantize(Mp3Decoder *dec, s32 gr, s32 ch);
s32 mp3_readScalefactors(Mp3Decoder *dec, s32 gr, s32 ch);
s32 mp3_readLsfScalefactors(Mp3Decoder *dec, s32 gr, s32 ch);
s32 mp3_decodeHuffman(Mp3Decoder *dec, s32 gr, s32 ch);
s32 mp3_reorder(Mp3Decoder *dec, s32 gr);
s32 mp3_antialias(Mp3Decoder *dec, s32 gr, s32 ch);
s32 mp3_readSideInfo(Mp3Decoder *dec);
s32 mp3_decodeLayer3(Mp3Decoder *dec);
extern f32 *gMp3Pow43Buffer; /* pow43 table storage (0x8000 bytes) */

/* game/Audio/Mp3Frame/mp3frame.c */
s32 mp3_appendReservoir(Mp3Decoder *dec);
s32 mp3_syncFrame(Mp3Decoder *dec, s32 seekPos);
Mp3Decoder *mp3_open(s32 handle, Mp3ReadFn read, s32 arg2);
s32 mp3_decodeFrame(Mp3Decoder *decoder, u8 **outBlock, s32 *outChannels);

/* game/Audio/Mp3Decoder/mp3huffman.s: the bit reader (mp3_getBits(buf, &bitPos, n) reads n bits from buf at bitPos) and the
 * Huffman decoders */
s32 mp3_getBits(void *buf, s32 *bitPos, s32 n);
s32 mp3_decodeBigValues(void *buf, s32 *bitPos, s32 table, s32 line, s32 linbits, s32 end, s16 **val, u8 **sign);
s32 mp3_decodeCount1(void *buf, s32 *bitPos, s32 table, s32 line, s32 end, s16 **val, u8 **sign);

/* game/Audio/Mp3Decoder/mp3imdctlong.s */
void mp3_imdctLong(f32 *xr, s32 sb, f32 *out, f32 *overlap, f32 *window);

/* game/Audio/Mp3Decoder/mp3imdctshort.s */
void mp3_imdctShort(f32 *xr, s32 sb, f32 *out, f32 *overlap);

#endif
