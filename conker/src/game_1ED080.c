#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_151BFB2C(struct260 *effect);

/* Destructor of a counter effect: frees its panel and digits, then the effect itself. */
void func_151BFBD0(struct260 *effect) {
    func_151BFB2C(effect);
    func_15149368(effect);
}

/* Message handler: frees the object on message 0x57. */
void func_151BFBFC(struct102 *obj, s32 arg1, u8 msg) {
    if (msg == 0x57) {
        func_1516972C(obj);
    }
}
