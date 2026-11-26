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
void func_10012560(s32 unused, void *message, s32 size);
void func_10012588(ALHeap *heap);
void func_100125CC(s32 levelId);
void func_1001263C(s32 id, s32 volume, s32 pan);
s32 func_100126E8(u8 *dst, s32 dstSize);
s32 func_10012718(u16 id, Actor *obj, s32 volume, s16 nearDistance, u16 farDistance);
s32 func_100127D0(void);
extern s32 D_800427F0;
extern u16 D_800427F4; /* the track last started */

/* game_43620.c */
void func_15016170(ALHeap *arg0);

/* libultra/audio/game_21FC90.c: the streaming music player (StreamVoice D_800E0D80) */
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
/* (Mp3ReadFn read): sets the stream's read callback; declared without a prototype: func_15016170 (game_43620.c)
 * passes it with the volume and the pan */
void func_151F3C34();
s32 func_151F3C4C(s32 arg0, void *dst, s32 len, s32 offset);
void func_151F3D78(void);

/* game_221290.c */
s32 func_151F3DE0(void);
s32 func_151F42E8(Mp3Decoder *dec, s32 gr, s32 ch);
s32 func_151F4F38(Mp3Decoder *dec, s32 gr, s32 ch);
s32 func_151F578C(Mp3Decoder *dec, s32 gr, s32 ch);
s32 func_151F63C4(Mp3Decoder *dec, s32 gr, s32 ch);
s32 func_151F6970(Mp3Decoder *dec, s32 gr);
s32 func_151F6B28(Mp3Decoder *dec, s32 gr, s32 ch);
s32 func_151F6FD0(Mp3Decoder *dec);
s32 func_151F78B4(Mp3Decoder *dec);
s32 func_151F7F60(Mp3Decoder *dec);
s32 func_151F8088(Mp3Decoder *dec, s32 seekPos);
Mp3Decoder *func_151F85C4(s32 handle, Mp3ReadFn read, s32 arg2);
s32 func_151F86B0(Mp3Decoder *decoder, u8 **outBlock, s32 *outChannels);

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
