#include <ultra64.h>

#include "functions.h"
#include "variables.h"


/*
 * Game entrypoint: initialises the subsystems, then runs the main loop forever,
 * stepping the frame according to the game state in D_800BE615.
 */
void func_15007830(void) {
    func_15007A20();
    D_800D2C28[0] = 0;
    osCreateMesgQueue(&D_800BEA10, D_800BEA28, 16);
    D_800BEA68[0].type = OS_SC_DONE_MSG;
    D_800BEA68[1].type = OS_SC_DONE_MSG;
    D_800BE617 = 0;
    func_100050A0(&D_800BEA10);
    func_15003570();
    D_800BEAAB = 0;
    func_10008180();
    func_15000000();
    func_15016588();
    func_151EEFF0();
    D_800BEAA8 = 0;
    func_150061B0();
    func_15006234();
    func_151DD970();
    func_15015920(0);
    func_15008A60();
    func_15042D50();
    D_800BE615 = 5;
    D_800BEA04 = 0;
    D_800BEA00 = 1;
    D_800BEAAA = 1;
    func_1509C120();

    // matching: `while (1)`, not `do {} while (1)`; only this form 32-byte-aligns the dead epilogue
    while (1) {
        switch (D_800BE615) {
            case 1:
            case 5:
                func_151E50C8();
                /* fallthrough */
            case 2:
                func_15017498();
                if (D_800E0B94 == 2) {
                    func_150ADACC(0x81280783);
                }
                func_15007A70(D_800BEA04, D_800BEA00, D_800BE9F4);
                /* fallthrough */
            case 3:
                func_15007B3C();
                D_800BE615 = 0;
                break;
            case 4:
                break;
        }
        func_100051E8();
        D_800BE9E8 = 0;
        func_150186D0();
    }
}
