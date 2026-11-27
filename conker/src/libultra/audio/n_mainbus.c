#include <ultra64.h>
#include <n_libaudio.h>
#include "n_abi.h"
#include "structs.h"
#include "conker/audio.h"
#include "conker/mp3.h"

typedef Acmd *(*MainBusHandler)(s32 sampleOffset, Acmd *p, s32 bus);

/* n_libaudio.h leaves the bus structs opaque; Conker's main-bus handler takes the
 * aux bus index as a third argument, and each aux bus (stride 0x44) carries its
 * low-pass filter at 0x40. */
struct N_ALMainBus_s {
    /* 0x0 */ void          *source;
    /* 0x4 */ MainBusHandler handler;
};

struct N_ALAuxBus_s {
    /* 0x00 */ u8       pad0[0x40];
    /* 0x40 */ LowPass *lowPass;
};

/* Append one frame's bus mix-down to the audio command list: clear the main and aux
 * buffers, then pull every aux bus (starting after the last one with an active
 * low-pass), mix it into the L/R main outs and run its pole filter on them. */
Acmd *n_alMainBusPull(s32 sampleOffset, Acmd *cmdList) {
    Acmd    *p;
    s32      i;
    s32      bus;

    p = cmdList;
    if ((D_800E0D80.unk84 == 0) || (mp3_pullFrame(0xB8, &p) == 0)) {
        aClearBuffer(p++, 0x4E0, 0x2E0);
        aClearBuffer(p++, 0x7C0, 0x2E0);
    }
    bus = 0;
    for (i = 1; i < n_syn->maxAuxBusses; i++) {
        if (n_syn->auxBus[i].lowPass->fgain > 0) {
            bus = i;
        }
    }
    for (i = 0; i < n_syn->maxAuxBusses; i++, bus++) {
        if (bus >= n_syn->maxAuxBusses) {
            bus = 0;
        }
        if (i != 0) {
            aClearBuffer(p++, 0x7C0, 0x2E0);
        }
        p = n_syn->mainBus->handler(sampleOffset, p, bus);
        if (D_800428C4[bus] != 0) {
            if (D_800428C6[bus] != 0) {
                aMix(p++, 0, 0x8000, 0x7C0, 0x4E0);
            } else {
                aMix(p++, 0, 0x7FFF, 0x7C0, 0x650);
            }
        } else {
            if (D_800428C6[bus] != 0) {
                aMix(p++, 0, 0x8000, 0x7C0, 0x650);
            } else {
                aMix(p++, 0, 0x7FFF, 0x7C0, 0x650);
            }
            aMix(p++, 0, 0x7FFF, 0x7C0, 0x4E0);
        }
        if (n_syn->auxBus[bus].lowPass->fgain > 0) {
            LowPass *lp = n_syn->auxBus[bus].lowPass;

            if (lp->first != 0) {
                audio_updateLowPassCoefs(lp, n_syn->outputRate);
            }
            aLoadADPCM(p++, 0x20, osVirtualToPhysical(&lp->fcvec));
            // matching: the ROM masks the state address twice (once here, once in the macro)
            n_aPoleFilter(p++, 0, 0x4E0, 0, osVirtualToPhysical(lp->fstate[0]) & 0xFFFFFF);
            n_aPoleFilter(p++, 0, 0x650, 0, osVirtualToPhysical(lp->fstate[1]) & 0xFFFFFF);
            lp->first = 0;
        }
    }
    return p;
}
