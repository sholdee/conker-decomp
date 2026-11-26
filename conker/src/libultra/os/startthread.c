#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PRinternal/osint.h"

void osStartThread(OSThread* t) {
    register u32 saveMask = __osDisableInt();

    switch (t->state) {
        case OS_STATE_WAITING:
            t->state = OS_STATE_RUNNABLE;
            func_100079D8(&__osRunQueue, t);
            break;
        case OS_STATE_STOPPED:
            if (t->queue == NULL || t->queue == &__osRunQueue) {
                t->state = OS_STATE_RUNNABLE;
                func_100079D8(&__osRunQueue, t);
            } else {
                t->state = OS_STATE_WAITING;
                func_100079D8(t->queue, t);
                func_100079D8(&__osRunQueue, func_10007A24(t->queue));
            }
            break;
#ifdef _DEBUG
        default:
            __osError(ERR_OSSTARTTHREAD, 0);
            __osRestoreInt(saveMask);
            return;
#endif
    }

    if (__osRunningThread == NULL) {
        func_10007A38();
    } else if (__osRunningThread->priority < __osRunQueue->priority) {
        __osRunningThread->state = OS_STATE_RUNNABLE;
        func_100078B4(&__osRunQueue);
    }

    __osRestoreInt(saveMask);
}
