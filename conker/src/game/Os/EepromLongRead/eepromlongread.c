#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Rare's copy of the SDK's EEPROM long read (2.0I and later: no address check and no delay between blocks). It
 * first finishes the pending timed EEPROM write (save_pollEepromWrite), which the SDK's does not do. */
s32 func_151DD3A0(OSMesgQueue *mq, u8 address, u8 *buffer, int length) {
    s32 ret = 0;

    while (save_pollEepromWrite(1) != 0) {};

    while (length > 0) {
        ret = osEepromRead(mq, address, buffer);
        if (ret != 0) {
            return ret;
        }
        length -= EEPROM_BLOCK_SIZE;
        address++;
        buffer += EEPROM_BLOCK_SIZE;
    }

    return ret;
}
