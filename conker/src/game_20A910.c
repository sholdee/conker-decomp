#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void __osSiGetAccess(void);
void __osSiRelAccess(void);

/* func_151DD460 = osEepromProbe: returns EEPROM_TYPE_4K / EEPROM_TYPE_16K, or 0 if none. */
s32 func_151DD460(OSMesgQueue *mq) {
    s32 ret = 0;
    u16 type;
    OSContStatus sdata;

    __osSiGetAccess();
    ret = func_151DD710(mq, &sdata);
    if (ret != 0) {
        ret = 0;
    } else {
        type = sdata.type & (CONT_EEPROM | CONT_EEP16K);
        switch (type) {
            case CONT_EEPROM:
                ret = EEPROM_TYPE_4K;
                break;
            case CONT_EEPROM | CONT_EEP16K:
                ret = EEPROM_TYPE_16K;
                break;
            default:
                ret = 0;
                break;
        }
    }
    __osSiRelAccess();
    return ret;
}
