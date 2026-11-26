#include <ultra64.h>

#include "functions.h"
#include "variables.h"


/*
 * Game entrypoint: initialises the subsystems, then runs the main loop forever,
 * stepping the frame according to the game state in D_800BE615.
 */
void func_15007830(void) {
    game_clearBss();
    D_800D2C28[0] = 0;
    osCreateMesgQueue(&gGameMesgQueue, gGameMesgBuf, 16);
    gTaskDoneMsgs[0].type = OS_SC_DONE_MSG;
    gTaskDoneMsgs[1].type = OS_SC_DONE_MSG;
    D_800BE617 = 0;
    func_100050A0(&gGameMesgQueue);
    func_15003570();
    D_800BEAAB = 0;
    func_10008180();
    music_init();
    func_15016588();
    func_151EEFF0();
    D_800BEAA8 = 0;
    func_150061B0();
    func_15006234();
    save_resetSettings();
    text_loadFont(0);
    screen_resetEffects();
    text_clearQueue();
    D_800BE615 = 5;
    D_800BEA04 = 0;
    D_800BEA00 = 1;
    D_800BEAAA = 1;
    script_reallocTaskDoneBits();

    // matching: `while (1)`, not `do {} while (1)`; only this form 32-byte-aligns the dead epilogue
    while (1) {
        switch (D_800BE615) {
            case 1:
            case 5:
                save_initMenu();
                /* fallthrough */
            case 2:
                player_clearMatch();
                if (gFrontEndState == 2) {
                    func_150ADACC(0x81280783);
                }
                func_15007A70(D_800BEA04, D_800BEA00, gNextLevel);
                /* fallthrough */
            case 3:
                game_loadLevel();
                D_800BE615 = 0;
                break;
            case 4:
                break;
        }
        func_100051E8();
        D_800BE9E8 = 0;
        game_mainLoop();
    }
}
