#ifndef CONKER_MP3_H
#define CONKER_MP3_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "mp3": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init_12560.c
 *   game_43620.c
 *   libultra/audio/game_21FC90.c
 *   game_221290.c
 *   game_225D20.s
 *   game_2261A0.s
 *   game_2270A0.s
 */

/* Types that these files define; the declarations below use them. */
typedef struct Mp3Decoder Mp3Decoder;
typedef s32 (*Mp3ReadFn)(s32 handle, void *dst, s32 size, s32 seekPos);

/* init_12560.c */
void mp3_pushMessage(s32 unused, void *message, s32 size);
void mp3_initStream(ALHeap *heap);
void mp3_stopForLevel(s32 levelId);
void mp3_startTrack(s32 id, s32 volume, s32 pan);
s32 mp3_popMessage(u8 *dst, s32 dstSize);
s32 mp3_startTrackAt(u16 id, Actor *obj, s32 volume, s16 nearDistance, u16 farDistance);
s32 mp3_isPlaying(void);
extern s32 D_800427F0;
extern u16 gMp3LastTrack; /* the track last started */

/* game_43620.c */
void mp3_initVoice(ALHeap *arg0);

/* libultra/audio/game_21FC90.c: the streaming music player (StreamVoice D_800E0D80) */
extern StreamVoice D_800E0D80; /* the streaming music player's voice */
extern s32 *D_800E0E20;        /* Huffman table pointers, relocated by D_800E0E28 */
extern void *D_800E0E28;       /* the Huffman table data */
void func_151F2960(s32 data, s32 size);
void func_151F2BA8(void);
void func_151F2BE8(void);
void func_151F2C4C(void);
s32 func_151F2CDC(void);
void func_151F2D6C(s32 volume, s32 arg1);
void func_151F2DFC(s32 pan, s32 now);
void func_151F2E4C(s32 arg0, s32 arg1);
s32 func_151F2E88(s32 nSamples, Acmd **cmdList);
void func_151F39E4(StreamVoice *v);
void func_151F3C1C(void (*callback)(s32, void *, s32));
/* (Mp3ReadFn read): sets the stream's read callback; declared without a prototype: mp3_initVoice (game_43620.c)
 * passes it with the volume and the pan */
void func_151F3C34();
s32 func_151F3C4C(s32 arg0, void *dst, s32 len, s32 offset);
void func_151F3D78(void);

/* game_221290.c */
s32 mp3_initTables(void);
s32 mp3_dequantize(Mp3Decoder *dec, s32 gr, s32 ch);
s32 mp3_readScalefactors(Mp3Decoder *dec, s32 gr, s32 ch);
s32 mp3_readLsfScalefactors(Mp3Decoder *dec, s32 gr, s32 ch);
s32 mp3_decodeHuffman(Mp3Decoder *dec, s32 gr, s32 ch);
s32 mp3_reorder(Mp3Decoder *dec, s32 gr);
s32 mp3_antialias(Mp3Decoder *dec, s32 gr, s32 ch);
s32 mp3_readSideInfo(Mp3Decoder *dec);
s32 mp3_decodeLayer3(Mp3Decoder *dec);
s32 mp3_appendReservoir(Mp3Decoder *dec);
s32 mp3_syncFrame(Mp3Decoder *dec, s32 seekPos);
Mp3Decoder *mp3_open(s32 handle, Mp3ReadFn read, s32 arg2);
s32 mp3_decodeFrame(Mp3Decoder *decoder, u8 **outBlock, s32 *outChannels);
extern f32 *gMp3Pow43Buffer; /* pow43 table storage (0x8000 bytes) */

/* game_225D20.s: the bit reader (func_151F8960(buf, &bitPos, n) reads n bits from buf at bitPos) and the
 * Huffman decoders */
s32 func_151F8960(void *buf, s32 *bitPos, s32 n);
s32 func_151F8994(void *buf, s32 *bitPos, s32 table, s32 line, s32 linbits, s32 end, s16 **val, u8 **sign);
s32 func_151F8B4C(void *buf, s32 *bitPos, s32 table, s32 line, s32 end, s16 **val, u8 **sign);

/* game_2261A0.s */
void func_151F8CF0(f32 *xr, s32 sb, f32 *out, f32 *overlap, f32 *window);

/* game_2270A0.s */
void func_151F9BF0(f32 *xr, s32 sb, f32 *out, f32 *overlap);

#endif
