#include <ultra64.h>

#include "conker/os.h"
#include "conker/rom.h"

/* libultra code (with Rare additions). */

/* __osDevMgrMain, with Rare's PI-thread interlock added to the DMAREAD case. */
void __osDevMgrMain(void *arg) {
    OSIoMesg *mb;
    OSMesg em;
    OSMesg dummy;
    OSDevMgr *dm;
    s32 ret;

    mb = NULL;
    dm = (OSDevMgr *)arg;

    while (1) {
        osRecvMesg(dm->cmdQueue, (OSMesg *)&mb, OS_MESG_BLOCK);
        switch (mb->hdr.type) {
            case OS_MESG_TYPE_DMAREAD:
                if (gRomCopyActive != 0) {
                    gPiThreadStopped = 1;
                    osStopThread(&D_80035910);
                    gPiThreadStopped = 0;
                }
                gPiDmaReadBusy = 1;
                osRecvMesg(dm->acsQueue, &dummy, OS_MESG_BLOCK);
                ret = dm->dma(OS_READ, mb->devAddr, mb->dramAddr, mb->size);
                break;
            case OS_MESG_TYPE_DMAWRITE:
                osRecvMesg(dm->acsQueue, &dummy, OS_MESG_BLOCK);
                ret = dm->dma(OS_WRITE, mb->devAddr, mb->dramAddr, mb->size);
                break;
            case OS_MESG_TYPE_EDMAREAD:
                osRecvMesg(dm->acsQueue, &dummy, OS_MESG_BLOCK);
                ret = dm->edma(mb->piHandle, OS_READ, mb->devAddr, mb->dramAddr, mb->size);
                break;
            case OS_MESG_TYPE_EDMAWRITE:
                osRecvMesg(dm->acsQueue, &dummy, OS_MESG_BLOCK);
                ret = dm->edma(mb->piHandle, OS_WRITE, mb->devAddr, mb->dramAddr, mb->size);
                break;
            case OS_MESG_TYPE_LOOPBACK:
                osSendMesg(mb->hdr.retQueue, mb, OS_MESG_NOBLOCK);
                ret = -1;
                break;
            default:
                ret = -1;
                break;
        }
        if (ret == 0) {
            osRecvMesg(dm->evtQueue, &em, OS_MESG_BLOCK);
            osSendMesg(mb->hdr.retQueue, mb, OS_MESG_NOBLOCK);
            osSendMesg(dm->acsQueue, NULL, OS_MESG_NOBLOCK);
            if (mb->hdr.type == OS_MESG_TYPE_DMAREAD) {
                gPiDmaReadBusy = 0;
            }
        }
    }
}
