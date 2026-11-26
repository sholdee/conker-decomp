#include "n_synthInternals.h"

/*
 * Shadow of N_PVoice covering the low-pass (a LowPass, n_libaudio.h) block Rare bolted on at
 * 0xA0: fc / fgain / fcvec[16] / state pointer, plus the tap count at 0x99.
 */
typedef struct {
    u8  pad0[0x99];
    /* 0x99 */ u8   count;
    u8  pad9A[0x6];
    /* 0xA0 */ s16  fc;
    /* 0xA2 */ s16  fgain;
    u8  padA4[0x4];
    /* 0xA8 */ s16  fcvec;
    u8  padAA[0x1E];
    /* 0xC8 */ s32  flags;
    /* 0xCC */ void *fstate;
} Voice;

f32 sqrtf(f32);

Acmd *func_10022040(Voice *voice, s16 *outp, s32 arg2, Acmd *p) {
    Acmd *ptr;
    f32 gain;
    Acmd *mixCmd;
    Acmd *loadCmd;
    Acmd *poleCmd;

    ptr = p;
    ptr = func_10022460((N_PVoice *) voice, outp, p);

    if ((voice->count != 0) && (voice->count < 64)) {
        if (voice->count >= 6) {
            gain = 26755.0f / sqrtf((f32) voice->count + 1.0f);
        } else {
            gain = 65536.0f / ((f32) voice->count + 1.0f);
        }
        if (gain < 7723.0f) {
            gain = 7723.0f;
        }
        mixCmd = ptr++;
        mixCmd->words.w0 = outp[0] & 0xFFFF;
        mixCmd->words.w1 = (((u32) gain & 0xFFFF) << 16) | ((voice->count + 1) & 0xFFFF);
    }
    if (voice->fgain > 0) {
        if (voice->flags != 0) {
            func_1001CF38((LowPass *)&voice->fc, 22050.0f);
        }
        /* aLoadADPCM(32, fcvec): DMA the 16 filter coefficients into DMEM */
        loadCmd = ptr++;
        loadCmd->words.w0 = 0xB000020;
        loadCmd->words.w1 = osVirtualToPhysical(&voice->fcvec);
        if (voice->flags == 2) {
            voice->flags = 0;
        }
        /* aPoleFilter(flags, outp, fstate) */
        poleCmd = ptr++;
        poleCmd->words.w0 = (outp[0] & 0xFFFF) | (((voice->flags & 0xFF) << 16) | 0xE000000);
        poleCmd->words.w1 = osVirtualToPhysical(voice->fstate) & 0xFFFFFF & 0xFFFFFF;
        voice->flags = 0;
    }
    return ptr;
}

s32 n_alLoadParam(N_PVoice *filter, s32 paramID, void *param) {
    f32 *paramAsFloat = (f32 *)&param;

    switch (paramID) {
        case 4:
            filter->unkA2 = 0;
            func_10021C40(filter, 4, param);
            break;
        case 18:
            filter->unkA2 = (s32)param;
            filter->unkC8 |= 2;
            break;
        case 19:
            filter->unkA0 = *paramAsFloat;
            filter->unkC8 |= 2;
            break;
        case 17:
            filter->unk99 = (s32)param;
            break;
        default:
            func_10021C40(filter, paramID, param);
            break;
    }

    return 0;
}
