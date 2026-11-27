#include <ultra64.h>
#include <n_libaudio.h>
#include <os_internal.h>
#include <ultraerror.h>

void alSeqFileNew(ALSeqFile *arg0, u8 *base) {
    s32 offset = (s32)base;
    s32 i;

    for (i = 0; i < arg0->seqCount; i++)
    {
        arg0->seqArray[i].offset += offset;
    }
}

/* Rare's alBnkfNew takes a third argument. */
void alBnkfNew(ALBankFile *file, u8 *table, s32 arg2) {
    s32 offset = (s32)file;
    s32 woffset = (s32)table;
    s32 sp1C = arg2;

    s32 i;

    ALFailIf(file->revision != AL_BANK_VERSION, ERR_ALBNKFNEW);

    for (i = 0; i < file->bankCount; i++) {
        file->bankArray[i] = (ALBank *)((u8 *)file->bankArray[i] + offset);
        if (file->bankArray[i] != 0) {
            _bnkfPatchBank(file->bankArray[i], offset, woffset, sp1C);
        }
    }
}

/* Conker's _bnkfPatchBank: relocates the percussion and instrument 0 by the bank file offset, the other
 * instruments by the wave table offset. */
void _bnkfPatchBank(ALBank *bank, s32 offset, s32 table, s32 arg3) {
    s32 i;
    s32 tmp;

    if (bank->flags != 0) {
        return;
    }

    bank->flags = 1;

    if (bank->percussion != NULL) {
        bank->percussion = (ALInstrument *)((u8 *)bank->percussion + offset);
        func_10012B84(bank->percussion, arg3);
    }

    for (i = 0; i < bank->instCount; i ++) {
        if (bank->instArray[i] != NULL) {
            if (i == 0) {
                bank->instArray[i] = (ALInstrument *)((u8 *)bank->instArray[i] + offset);
                func_10012BD0(bank->instArray[i], arg3);
            } else {
                tmp = (arg3 & -8) << 5;
                bank->instArray[i] = (ALInstrument *)((u8 *)bank->instArray[i] + tmp);
            }
        }
    }
}

/* Relocates the first sound of a percussion instrument. */
void func_10012B84(ALInstrument *inst, s32 arg1) {
    if (inst->flags) {
        return;
    }
    inst->flags = 1;
    arg1 = (0xFFFFFFF8 & arg1) << 5;
    inst->soundArray[0] = (ALSound *)((u8 *)inst->soundArray[0] + arg1);
}

/* Relocates every sound of an instrument. */
void func_10012BD0(ALInstrument *inst, s32 arg1) {
    s32 i;

    if (inst->flags != 0) {
        return;
    }
    inst->flags = 1;

    arg1 = (arg1 & -8) << 5;

    for (i = 0; i < inst->soundCount; i++)
    {
        inst->soundArray[i] = (ALSound *)((u8 *)inst->soundArray[i] + arg1);
    }
}

/* _bnkfPatchSound: relocates a sound's envelope, key map and wave table, then the wave table itself. */
void _bnkfPatchSound(ALSound *sound, s32 offset, s32 table) {
    if (sound->flags) {
        return;
    }
    sound->flags = 1;
    sound->envelope = (ALEnvelope *)((u8 *)sound->envelope + offset);
    sound->keyMap = (ALKeyMap *)((u8 *)sound->keyMap + offset);
    sound->wavetable = (ALWaveTable *)((u8 *)sound->wavetable + offset);
    _bnkfPatchWaveTable((WaveTable *)sound->wavetable, offset, table);
}

/* _bnkfPatchWaveTable: relocates the wave data by `table` and the loop and ADPCM book by `offset`. */
void _bnkfPatchWaveTable(WaveTable *wave, s32 offset, s32 table) {
    wave->flags = 1;
    wave->base += table;
    if (wave->type == AL_ADPCM_WAVE) {
        wave->book = (ALADPCMBook *)((u8 *)wave->book + offset);
        if (wave->loop != NULL) {
            wave->loop = (u8 *)wave->loop + offset;
        }
        wave->unk14 = 0;
    } else if ((wave->type == AL_RAW16_WAVE) && (wave->loop != NULL)) {
        wave->loop = (u8 *)wave->loop + offset;
    }
}
