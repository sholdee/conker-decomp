#include <ultra64.h>
#include <n_libaudio.h>
#include "variables.h"

extern s32  D_800E0E08;
extern s32  D_800E0E10;
extern s16  D_800E0E14;
extern s16  D_800E0E16;
extern u8   D_800E0E18;
extern u8   D_800E0E2C;
extern void *D_800E0E24;
extern s16  D_800E0DB0;
extern s16  D_800E0DB2;
extern s32  D_800E0DD8;
extern s32  D_800E0DE0;
extern s32  D_800E0DE4;
extern Mp3ReadFn D_800E0DFC;

extern Mp3Decoder *D_800E0D84;
extern void *D_800E0D88; /* D_800E0D80.unk8 */
extern s16  D_800E0D8C;
extern s16  D_800E0D8E;
extern s16  D_800E0D90;
extern s16  D_800E0D92;
extern s16  D_800E0D94;
extern s16  D_800E0D96;
extern u16  D_800E0D98;
extern s16  D_800E0D9A;
extern s16  D_800E0D9C;
extern u16  D_800E0D9E;
extern s16  D_800E0DA0;
extern s16  D_800E0DA2;
extern s16  D_800E0DA4;
extern s32  D_800E0DA8;
extern s32  D_800E0DAC;
extern void *D_800E0DDC; /* D_800E0D80.unk5C */
extern u8  *D_800E0DE8;
extern s32  D_800E0DEC;
extern s32  D_800E0DF0;
extern void *D_800E0DF4[];
extern s32  D_800E0DF8;
extern u8   D_800E0E19;

extern s32  _getRate(f32 ivol, f32 tvol, s32 count, u16 *ratel);

/*
 * Local shadow of N_ALSynth.  n_libaudio.h calls 0x24..0x2B `u8 pad24[8]` and puts the one
 * DMA-callback it does name (`dma`) at 0x2C -- which is right for n_synfreevoice.c, and that
 * TU matches.  The streaming reads below go through a SECOND ALDMANew-shaped callback that
 * lives at 0x24 (the offset the stock SDK ALSynth uses).  Shadow it here rather than editing
 * the shared header, which would re-score every audio TU.
 */
typedef struct {
    /* 0x00 */ u8       pad0[0x24];
    /* 0x24 */ ALDMANew dmaNew;
} StreamSynth;

extern s16 _getVol(s16 ivol, s32 samples, s16 ratem, u16 ratel);

void func_151F2960(s32 arg0, s32 arg1) {
    if (D_800E0DFC == 0) {
        return;
    }
    D_800E0E04 = 4;
    if (D_800E0E2C == 0) {
      D_800E0E2C = 1;
        D_800E0E30 = allocate_memory(0x8000, 0xFF, 2, 1);
        if (D_800E0E30 == 0) {
            D_800E0E2C = 0;
            return;
        }
        D_800E0E20 = func_1502B5C8(0, 2, 0x17, 4);
        if (D_800E0E20 != 0) {
            func_100043B4(D_800E0E20, 0xFF);
        }
        D_800E0E24 = func_1502B5C8(0, 2, 0x17, 5);
        if (D_800E0E24 != 0) {
            func_100043B4(D_800E0E24, 0xFF);
        }
        D_800E0E28 = func_1502B5C8(0, 2, 0x17, 6);
        if (D_800E0E28 != 0) {
            func_100043B4(D_800E0E28, 0xFF);
        }
        if ((D_800E0E20 == 0) || (D_800E0E24 == 0) || (D_800E0E28 == 0)) {
            if (D_800E0E20 != 0) {
                func_10004074(D_800E0E20);
            }
            if (D_800E0E24 != 0) {
                func_10004074(D_800E0E24);
            }
            if (D_800E0E28 != 0) {
                func_10004074(D_800E0E28);
            }
            func_10004074(D_800E0E30);
            D_800E0E30 = NULL;
            D_800E0E2C = 0;
            return;
        }
        func_151F3DE0();
    }
    D_800E0D80.romAddr = arg0;
    D_800E0DE0 = arg1;
    D_800E0DE4 = 0;
    D_800E0E10 = 0;
    D_800E0E18 = 5;
    D_800E0E04 = 5;
}

void func_151F2BA8(void) {
    u32 mask = osSetIntMask(1);
    D_800E0E04 = 3;
    osSetIntMask(mask);
}

void func_151F2BE8(void) {
    u32 mask = osSetIntMask(1);
    if (D_800E0E04 == 5) {
        D_800E0E04 = 6;
    } else {
        D_800E0E04 = 2;
    }
    osSetIntMask(mask);
}

void func_151F2C4C(void) {
    u32 mask = osSetIntMask(1);
    if (D_800E0E04 == 2) {
        D_800E0E18 = 5;
        D_800E0E04 = 7;
    } else if (D_800E0E04 == 6) {
        D_800E0E18 = 5;
        D_800E0E04 = 5;
    }
    osSetIntMask(mask);
}

s32 func_151F2CDC(void) {
    s32 ret = 0;
    u32 mask = osSetIntMask(1);

    if ((D_800E0E04 == 1) || (D_800E0E04 == 5) || (D_800E0E04 == 6) || (D_800E0E04 == 7) || (D_800E0E04 == 2)) {
        ret = D_800E0E04;
    }
    osSetIntMask(mask);
    return ret;
}

void func_151F2D6C(s32 arg0, s32 arg1) {
    u32 mask = osSetIntMask(1);
    if (arg0 < 0) {
        D_800E0E08 = 0;
    } else if (arg0 >= 0x8000) {
        D_800E0E08 = 0x7FFF;
    } else {
        D_800E0E08 = arg0;
    }
    D_800E0E10 = arg1;
    osSetIntMask(mask);
}

void func_151F2DFC(s32 arg0, s32 arg1) {
    if (arg0 >= 0x80) {
        arg0 = 0x7F;
    } else {
        if (arg0 < 0) {
            arg0 = 0;
        }
    }
    D_800E0E16 = arg0;
    if (arg1 != 0) {
        D_800E0E14 =  D_800E0E16;
    }
}

void func_151F2E4C(s32 arg0, s32 arg1) {
    D_800E0DB2 = arg0;
    D_800E0DB0 = arg1;
    if (D_800E0DD8 == 0) {
        D_800E0DD8 = 2;
    }
}

/*
 * One audio frame of the streaming player: ramp the master volume towards its target, service
 * the state machine on D_800E0E04 (5 = opening / 7 = restarting / 1 = running / 3 = shutting
 * down), and, while running, append the acmds that DMA the decoded stream into DMEM, clear the
 * two output halves, program the envmixer and finally mix.  `nSamples` is how many samples the
 * caller wants this frame; `cmdList` is the audio command pointer, advanced in place.
 * Returns 1 while the stream is still running, 0 otherwise.
 */
s32 func_151F2E88(s32 nSamples, Acmd **cmdList) {
    s32 i;
    s32 diff;
    s32 count = 0;
    u8 *ptr;
    u8 *buf = NULL;
    s32 len;
    s32 dmemL = 0x4E0;
    s32 dmemR = 0x650;

    /* master volume slews towards D_800E0E16 two units at a time */
    if (D_800E0E14 != D_800E0E16) {
        diff = D_800E0E16 - D_800E0E14;
        if ((diff >= -2) && (diff < 3)) {
            D_800E0E14 = D_800E0E14 + diff;
        } else if (diff >= 3) {
            D_800E0E14 = D_800E0E14 + 2;
        } else {
            D_800E0E14 = D_800E0E14 - 2;
        }
    }

    if (D_800E0E04 == 5) {
        func_151F3D78();
        if (D_800E0E18 == 0) {
            D_800E0D84 = func_151F85C4(0, D_800E0DFC, D_800E0DE0);
            if (D_800E0D84 == 0) {
                D_800E0DE4 = 0;
                D_800E0E10 = 0;
                D_800E0E18 = 5;
                func_151F3D78();
                D_800E0E04 = 5;
                return 0;
            } else {
                D_800E0E04 = 1;
                D_800E0DA4 = 1;
                D_800E0DE8 = 0;
                D_800E0DEC = 0;
                D_800E0DF0 = 0;
                D_800E0DF8 = 0;
                bzero(((StreamVoice *)&D_800E0D80)->unk74, 0x440); // matching: a no-op cast keeps the address in its own register
            }
        } else {
            D_800E0E18 = D_800E0E18 - 1;
        }
    }

    if (D_800E0E04 == 7) {
        func_151F3D78();
        if (D_800E0E18 == 0) {
            D_800E0E04 = 1;
        } else {
            D_800E0E18 = D_800E0E18 - 1;
        }
    }

    if (D_800E0E04 == 1) {
        /* out of decoded samples: pull the next block of decoded buffers from the decoder */
        if (nSamples + D_800E0DF0 > D_800E0DEC) {
            D_800E0DEC = D_800E0DEC - D_800E0DF0;
            if ((D_800E0DEC != 0) && (D_800E0DE8 != 0)) {
                buf = D_800E0DF0 * 2 + D_800E0DE8;
                len = D_800E0DEC;
            }
            D_800E0DF0 = 0;
            D_800E0DEC = 0;
            if (func_151F86B0(D_800E0D84, &ptr, &count) != 0) {
                D_800E0DE8 = ptr;
                for (i = 0; i < count; i++) {
                    {
                        Acmd *_a = (*cmdList)++;
                        _a->words.w0 = 0x08000000;
                        _a->words.w1 = osVirtualToPhysical(D_800E0DF4[i]);
                    }
                    {
                        Acmd *_a = (*cmdList)++;
                        _a->words.w0 = 0x07000000 | (D_800E0DF8 & 0xFFFF);
                        _a->words.w1 = osVirtualToPhysical(ptr);
                    }
                    ptr += 0x488;
                }
                D_800E0DF8 = (D_800E0DF8 - 0x24) & 0x1E;
                D_800E0DEC = 0x240;
                D_800E0E19 = (count == 2);
            } else {
                /*
                 * Empty on purpose.  IDO emits the `b` at 0x151F326C (ROM 0x22071C) only for a
                 * then-block that has a matching else, and without this clause the function is
                 * exactly one instruction short.  Whatever stood here in Rare's source (a debug
                 * print, a #if 0) produced no code; the else itself is load-bearing.
                 */
            }
        }
        func_151F3D78();
        if (D_800E0E19 == 0) {
            dmemL = 0;
        }
        /* leftovers from the previous block, if any */
        if (buf != NULL) {
            {
                Acmd *_a = (*cmdList)++;
                _a->words.w0 = 0x04000000 | (((len + len) & 0xFFF) << 12) | (dmemL & 0xFFF);
                _a->words.w1 = osVirtualToPhysical(buf);
            }
            if (D_800E0E19 != 0) {
                buf += 0x488;
                {
                    Acmd *_a = (*cmdList)++;
                    _a->words.w0 = 0x04000000 | (((len + len) & 0xFFF) << 12) | (dmemR & 0xFFF);
                    _a->words.w1 = osVirtualToPhysical(buf);
                }
            }
            dmemL += len + len;
            dmemR += len + len;
            nSamples = nSamples - len;
        }
        if ((nSamples > 0) && (D_800E0DE8 != 0)) {
            buf = D_800E0DF0 * 2 + D_800E0DE8;
            {
                Acmd *_a = (*cmdList)++;
                _a->words.w0 = 0x04000000 | (((nSamples + nSamples) & 0xFFF) << 12) | (dmemL & 0xFFF);
                _a->words.w1 = osVirtualToPhysical(buf);
            }
            if (D_800E0E19 != 0) {
                buf += 0x488;
                {
                    Acmd *_a = (*cmdList)++;
                    _a->words.w0 = 0x04000000 | (((nSamples + nSamples) & 0xFFF) << 12) | (dmemR & 0xFFF);
                    _a->words.w1 = osVirtualToPhysical(buf);
                }
            }
            D_800E0DF0 = D_800E0DF0 + nSamples;
        }
        func_151F39E4(&D_800E0D80);
        if (D_800E0E19 == 0) {
            {
                Acmd *_a = (*cmdList)++;
                _a->words.w0 = 0x020004E0;
                _a->words.w1 = 0x2E0;
            }
            {
                Acmd *_a = (*cmdList)++;
                _a->words.w0 = 0x020007C0;
                _a->words.w1 = 0x2E0;
            }
            if (D_800E0DB2 > 0) {
                if (D_800E0DD8 != 0) {
                    func_1001CF38((LowPass *)&D_800E0D80.unk30, 22020.0f);
                }
                {
                    Acmd *_a = (*cmdList)++;
                    _a->words.w0 = 0x0B000020;
                    _a->words.w1 = osVirtualToPhysical(D_800E0D80.unk38) & 0xFFFFFF;
                }
                if (D_800E0DD8 == 2) {
                    D_800E0DD8 = 0;
                }
                {
                    Acmd *_a = (*cmdList)++;
                    _a->words.w0 = 0x0E000000 | ((D_800E0DD8 & 0xFF) << 16);
                    /* the outer mask is redundant but is in the ROM: two `and`s at 0x151F3620 */
                    _a->words.w1 = (osVirtualToPhysical(D_800E0DDC) & 0xFFFFFF) & 0xFFFFFF;
                }
                D_800E0DD8 = 0;
            }
            if (D_800E0DA4 != 0) {
                D_800E0DA4 = 0;
                D_800E0D9C = (D_8002BC10[D_800E0D8C] * D_800E0D8E) >> 15;
                D_800E0D9A = _getRate(D_800E0D90, D_800E0D9C, D_800E0DAC,
                                      &D_800E0D80.em_lratl);
                D_800E0DA2 = (D_8002BC10[127 - D_800E0D8C] * D_800E0D8E) >> 15;
                D_800E0DA0 = _getRate(D_800E0D92, D_800E0DA2, D_800E0DAC,
                                      &D_800E0D80.em_rratl);
                {
                    Acmd *_a = (*cmdList)++;
                    _a->words.w0 = 0x09060000 | (D_800E0D90 & 0xFFFF);
                    _a->words.w1 = ((D_800E0D94 & 0xFFFF) << 16) | (D_800E0D96 & 0xFFFF);
                }
                {
                    Acmd *_a = (*cmdList)++;
                    _a->words.w0 = 0x09040000 | (D_800E0DA2 & 0xFFFF);
                    _a->words.w1 = ((D_800E0DA0 & 0xFFFF) << 16) | (D_800E0D9E & 0xFFFF);
                }
                {
                    Acmd *_a = (*cmdList)++;
                    _a->words.w0 = 0x09000000 | (D_800E0D9C & 0xFFFF);
                    _a->words.w1 = ((D_800E0D9A & 0xFFFF) << 16) | (D_800E0D98 & 0xFFFF);
                }
                {
                    Acmd *_a = (*cmdList)++;
                    _a->words.w0 = 0x03010000 | (D_800E0D92 & 0xFFFF);
                    _a->words.w1 = osVirtualToPhysical(D_800E0D88);
                }
            } else {
                {
                    Acmd *_a = (*cmdList)++;
                    _a->words.w0 = 0x03000000;
                    _a->words.w1 = osVirtualToPhysical(D_800E0D88);
                }
            }
            D_800E0DA8 = D_800E0DA8 + 0xB8;
            if (D_800E0DA8 > D_800E0DAC) {
                D_800E0DA8 = D_800E0DAC;
            }
        }
    } else if ((D_800E0E04 == 3) && (level != 0x35)) {
        if (D_800E0E2C != 0) {
            func_10004074(D_800E0E20);
            func_10004074(D_800E0E24);
            func_10004074(D_800E0E28);
            D_800E0E2C = 0;
        }
        if (D_800E0E30 != 0) {
            func_10004074(D_800E0E30);
            D_800E0E30 = NULL;
        }
        D_800E0E04 = 0;
        return 0;
    } else {
        return 0;
    }
    return 1;
}

/*
 * Re-arm the streaming voice's pan/volume ramp whenever the requested volume or pan has
 * changed: finish (or snap) the ramp that is running, force a non-zero current volume so the
 * envmixer keeps mixing, latch the new volume/pan -- remapping the pan for the mono and
 * headphone output modes -- and start a fresh segment rounded up to a whole 184-sample frame.
 */
void func_151F39E4(StreamVoice *v) {
    if ((v->em_volume != v->unk88) || (v->em_pan != v->unk94)) {
        if (v->em_delta >= v->em_segEnd) {
            v->em_ltgt  = (D_8002BC10[v->em_pan] * v->em_volume) >> 15;
            v->em_rtgt  = (D_8002BC10[127 - v->em_pan] * v->em_volume) >> 15;
            v->em_delta = v->em_segEnd;
            v->em_cvolL = v->em_ltgt;
            v->em_cvolR = v->em_rtgt;
        } else {
            v->em_cvolL = _getVol(v->em_cvolL, v->em_delta, v->em_lratm, v->em_lratl);
            v->em_cvolR = _getVol(v->em_cvolR, v->em_delta, v->em_rratm, v->em_rratl);
        }
        if (v->em_cvolL == 0) {
            v->em_cvolL = 1;
        }
        if (v->em_cvolR == 0) {
            v->em_cvolR = 1;
        }
        v->em_volume = v->unk88;
        if ((v->em_volume == 0) && (v->unk90 != 0)) {
            func_151F2BA8();
        }
        if (v->em_pan != v->unk94) {
            if (D_800428C2 != 0) {
                v->em_pan = (v->unk94 >> 1) + 0x20;
            } else if (D_800428C1 != 0) {
                v->em_pan = 0x40;
            } else {
                v->em_pan = v->unk94;
            }
        }
        v->em_delta  = 0;
        v->em_segEnd = ((v->unk90 + 0xB7) / 0xB8) * 0xB8;
        v->unk24     = 1;
    }
}

void func_151F3C1C(void (*callback)(s32, void *, s32)) {
    D_800E0E00 = callback;
}

void func_151F3C34(read)
    Mp3ReadFn read;
{
    D_800E0DFC = read;
}

/*
 * Copy `len` bytes of the stream out of ROM into `dst`, starting at byte `offset` of the
 * stream (offset == -1 means "carry on from where the last read stopped").  Clamps the
 * request to what is left, returns the number of bytes actually copied, 0 if the DMA failed.
 */
s32 func_151F3C4C(s32 arg0, void *dst, s32 len, s32 offset) {
    s32       addr;
    ALDMAproc dma;

    if (offset != -1) {
        D_800E0DE4 = offset;
    }
    if (D_800E0DE4 + len > D_800E0DE0) {
        len = D_800E0DE0 - D_800E0DE4;
    }
    dma  = ((StreamSynth *) n_syn)->dmaNew(&addr);
    addr = dma(D_800E0D80.romAddr + D_800E0DE4, len, 0);
    if (addr == 0) {
        return 0;
    }
    addr += 0x80000000;
    osInvalDCache((void *) addr, len);
    bcopy((void *) addr, dst, len);
    D_800E0DE4 += len;
    return len;
}

/* Prefetch the next 0x810 bytes of the stream; the data is not copied anywhere, the point is
   the DMA into the audio cache. */
void func_151F3D78(void) {
    s32       addr;
    ALDMAproc dma;

    dma = ((StreamSynth *) n_syn)->dmaNew(&addr);
    dma(D_800E0D80.romAddr + D_800E0DE4, 0x810, 0);
}
