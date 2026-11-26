#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined in address order before functions.h and variables.h declare the globals: IDO
 * lays out .bss in the order it first sees each name, and that order also depends on how many types and
 * variables are declared before it (see init/Sched/SchedInit/schedinit.c). */
OSTimer gContReadTimer;             // the timer that starts the next controller read
u16 gButtonsHeld[4];              // per player: the buttons held at the last read
u16 gMenuButtonsHeld[4];              // per player: the buttons held at the last read, for gMenuButtonsPressed
u16 gButtonsPressed[4];              // per player: the newly pressed buttons
u16 gButtonsPrevious[4];              // per player: the buttons held before this read
u16 gPendingButtons[4];              // per channel: buttons to add at the next read
OSContPad **gPadTable;         // the per-player pads (the table that cont_readPads returns)
s32 gNumControllers;                 // the number of connected controllers (game/Cont/PadInit/padinit.c)
OSContPad *gPadPtrs[4];       // per player: a pointer into gPads
u8 gConnectedPads;                  // a bit for each controller that answered the last read
OSContPad gPads[4];        // the per-player pads
OSPfs gRumblePaks[MAXCONTROLLERS]; // the rumble pak of each channel
OSMesgQueue gSiMesgQueue;         // the SI message queue
OSContPad gMenuPads[4];        // the per-player pads with the stick dead zone (menu reads)
u16 gMenuButtonsPressed[4];              // per player: the newly pressed buttons (menu reads)
u8 gRumbleRedetect;                  // look for rumble paks at the next update
u8 gRumbleOn[MAXCONTROLLERS];  // per channel: the rumble is on
u8 gRumbleWasOn[MAXCONTROLLERS];  // per channel: the rumble was on at the last update
u8 gRumblePakPresent[MAXCONTROLLERS];  // per channel: a rumble pak is present
u8 gMotorRunning[MAXCONTROLLERS];  // per channel: the motor is running
f32 gRumbleTime[MAXCONTROLLERS]; // per channel: the time in the rumble cycle
f32 gRumbleOnTime[MAXCONTROLLERS]; // per channel: the on time of the rumble cycle
f32 gRumbleOffTime[MAXCONTROLLERS]; // per channel: the off time of the rumble cycle
f32 gRumblePeriod[MAXCONTROLLERS]; // per channel: the length of the rumble cycle
OSMesg gSiMesg;              // the message received from gSiMesgQueue

#include "functions.h"
#include "variables.h"

u8 gPlayerChannels[4] = { 0, 1, 2, 3 }; // the controller channel of each player; 4 or more: none
u8 gContReadDone = 1;                 // a controller read has finished: cont_startReadTimer arms the timer for the next
static u8 gEepromProbeResult = 3;          // the EEPROM probe result (2: a 16K EEPROM); 3 until the first probe

/* After each controller read (cont_readPads sets gContReadDone), arms a one-shot timer of
   25 * osClockRate / 1000000 ticks that posts message 6 to gSchedInterruptQueue. */
void cont_startReadTimer(void) {
    if (gContReadDone) {
        osSetTimer(&gContReadTimer, 25ULL * osClockRate / 1000000ULL, 0, &gSchedInterruptQueue, (OSMesg)6);
        gContReadDone = 0;
    }
}

/* Reads the controllers into the per-player pads (gPlayerChannels maps player slots to channels),
   records newly pressed buttons, applies the stick dead zone and returns the pad pointer table. */
OSContPad **cont_readPads(void) {
    s32 pad; // matching: unused stack slot in the original frame
    s32 player;
    s32 replaying;
    s32 numPlayers;
    OSContPad pads[4];
    s32 i;
    s32 x;
    s32 y; // also carries the button word

    for (i = 0; i < 4; i++) {
        gButtonsPrevious[i] = gPads[i].button;
    }
    osRecvMesg(&gSiMesgQueue, &gSiMesg, 1);
    osContGetReadData(pads);
    cont_runEepromRequest();
    gContReadDone = 1;
    gConnectedPads = 0;
    for (i = 0; i < 4; i++) {
        if (pads[i].errno != 0) {
            pads[i].button = 0;
            pads[i].stick_x = 0;
            pads[i].stick_y = 0;
        } else {
            gConnectedPads |= 1 << i;
        }
    }
    if (level != 0x1D) {
        numPlayers = D_8008FD90;
    } else {
        numPlayers = 4;
    }
    for (player = 0; player < 4; player++) {
        i = gPlayerChannels[player];
        if (i < 4 && player < numPlayers) {
            gPads[player].button = pads[i].button;
            gPads[player].stick_x = pads[i].stick_x;
            gPads[player].stick_y = pads[i].stick_y;
            gPads[player].button |= gPendingButtons[i];
            gPendingButtons[i] = 0;
        } else {
            gPads[player].button = 0;
            gPads[player].stick_x = 0;
            gPads[player].stick_y = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        y = gPads[i].button;
        gMenuButtonsPressed[i] = y & ~gMenuButtonsHeld[i];
        gMenuPads[i].button = y;
        gMenuButtonsHeld[i] = y;
        x = gPads[i].stick_x;
        if (((x < 0) ? -x : x) >= 8) {
            gMenuPads[i].stick_x = x - ((x < 0) ? -1 : 1) * 7;
        } else {
            gMenuPads[i].stick_x = 0;
        }
        y = gPads[i].stick_y;
        if (((y < 0) ? -y : y) >= 8) {
            gMenuPads[i].stick_y = y - ((y < 0) ? -1 : 1) * 7;
        } else {
            gMenuPads[i].stick_y = 0;
        }
    }
    if (gFrontEndState == 2 || gPrevFrontEndState == 2) {
        replaying = 1;
        os_playDemoInput();
    } else {
        replaying = 0;
    }
    for (i = 0; i < 4; i++) {
        y = gPads[i].button;
        gButtonsPressed[i] = y & ~gButtonsHeld[i];
        gButtonsHeld[i] = y;
        if (!replaying) {
            x = gPads[i].stick_x;
            if (((x < 0) ? -x : x) >= 8) {
                gPads[i].stick_x = x - ((x < 0) ? -1 : 1) * 7;
            } else {
                gPads[i].stick_x = 0;
            }
            y = gPads[i].stick_y;
            if (((y < 0) ? -y : y) >= 8) {
                gPads[i].stick_y = y - ((y < 0) ? -1 : 1) * 7;
            } else {
                gPads[i].stick_y = 0;
            }
        }
    }
    return gPadPtrs;
}


/* Starts rumble on a player's pak with an on/off pattern derived from strength (0-8). */
void cont_startRumble(u8 player, u8 strength) {
    f32 scaled;
    f32 onTime;
    f32 offTime;

    if (strength > 8) {
        strength = 8;
    }
    if (strength == 8) {
        onTime = 20.0f;
        offTime = 0.0f;
    } else if (strength == 0) {
        onTime = 0.0f;
        offTime = 99.0f;
    } else {
        scaled = (strength - 1.0f) * (1.0f / 6.0f) * 5.0f;
        onTime = scaled + 2.0f;
        offTime = 7.0f - scaled;
    }
    cont_startRumbleCycle(player, onTime, offTime);
}

/* Starts a player's rumble cycle: motor on for onTime, then off for offTime, repeating. */
void cont_startRumbleCycle(u8 player, f32 onTime, f32 offTime) {
    u8 channel = gPlayerChannels[player];

    if (channel < 4 && gRumblePakPresent[channel]) {
        gRumbleOn[channel] = 1;
        gRumbleTime[channel] = 0.0f;
        gRumbleOnTime[channel] = onTime;
        gRumbleOffTime[channel] = offTime;
        gRumblePeriod[channel] = onTime + offTime;
    }
}

/* Stops a player's rumble cycle. */
void cont_stopRumble(u8 player) {
    u8 channel = gPlayerChannels[player];

    if (channel < 4) {
        gRumbleOn[channel] = 0;
    }
}


/* Per-frame rumble update: (re)detects rumble paks when gRumbleRedetect is set, forces motors off
   while D_800BEAC3 / gRumbleSuppressed are set, and otherwise drives each motor's on/off cycle. */
void cont_updateRumble(void) {
    s32 channel;
    s32 motorOn;

    for (channel = 0; channel < 4; channel++) {
        if (gRumbleRedetect) {
            if (osMotorInit(&gSiMesgQueue, &gRumblePaks[channel], channel) == 0) {
                if (gRumblePakPresent[channel] == 0) {
                    gRumblePakPresent[channel] = 1;
                    gRumbleOn[channel] = 0;
                    gRumbleWasOn[channel] = 0;
                    gMotorRunning[channel] = 0;
                    gRumbleTime[channel] = 0.0f;
                    gRumbleOnTime[channel] = 1.0f;
                    gRumbleOffTime[channel] = 1.0f;
                    gRumblePeriod[channel] = 2.0f;
                    osMotorStop(&gRumblePaks[channel]);
                }
            } else {
                gRumblePakPresent[channel] = 0;
            }
        }
        if (gRumblePakPresent[channel]) {
            if (D_800BEAC3 || gRumbleSuppressed) {
                osMotorInit(&gSiMesgQueue, &gRumblePaks[channel], channel);
                osMotorStop(&gRumblePaks[channel]);
                gMotorRunning[channel] = 0;
                gRumbleOn[channel] = 0;
            }
            if (!gPauseRequested && !gPaused && !gPauseCooldown && !D_800BEAC3 && !gRumbleSuppressed) {
                switch (gRumbleOn[channel]) {
                case 0:
                    if (gRumbleWasOn[channel] == 1) {
                        osMotorInit(&gSiMesgQueue, &gRumblePaks[channel], channel);
                        osMotorStop(&gRumblePaks[channel]);
                        gMotorRunning[channel] = 0;
                    }
                    break;
                case 1:
                    gRumbleTime[channel] += gFrameDelta;
                    while (gRumbleTime[channel] > gRumblePeriod[channel]) {
                        gRumbleTime[channel] -= gRumblePeriod[channel];
                    }
                    motorOn = (gRumbleOnTime[channel] <= gRumbleTime[channel]) ? 0 : 1;
                    if (motorOn) {
                        if (gMotorRunning[channel] == 0) {
                            osMotorStart(&gRumblePaks[channel]);
                        }
                    } else if (gMotorRunning[channel] == 1) {
                        osMotorStop(&gRumblePaks[channel]);
                    }
                    gMotorRunning[channel] = motorOn;
                    break;
                }
            }
            gRumbleWasOn[channel] = gRumbleOn[channel];
        }
    }
    gRumbleRedetect = 0;
    D_800BEAC3 = 0;
    gRumbleSuppressed = 0;
}

/* Stops rumble for every player. */
void cont_stopAllRumble(void) {
    u8 player;

    for (player = 0; player < 4; player++) {
        cont_stopRumble(player);
    }
}

/* Runs the pending EEPROM request. Without a 16K EEPROM (probe result gEepromProbeResult != 2), load
   requests fall back to defaults and the others are dropped. */
void cont_runEepromRequest(void) {
    s32 i;

    save_pollEepromWrite(0);
    if (gEepromRequest != 0) {
        if (gEepromProbeResult != 2) {
            gEepromProbeResult = osEepromProbe(&gSiMesgQueue);
        }
        if (gEepromProbeResult != 2) {
            // matching: empty switch emits the original's 7-entry jump table
            switch (gEepromRequest) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    break;
            }
            // matching: a goto loop; IDO unrolls the equivalent for/while delay loop
            i = 19;
        delay:
            if (i > 0) {
                i--;
                goto delay;
            }
            if (gEepromRequest == 1) {
                save_resetGameState();
            } else if (gEepromRequest == 4) {
                save_resetModeSettings();
            }
            gEepromRequest = 0;
        // matching: an unused label ends the block, so as1 keeps the store out of the branch delay slot
        end:;
        } else {
            switch (gEepromRequest) {
                case 1:
                    save_readFile(gEepromRequestArg);
                    break;
                case 2:
                    save_writeSlot(gEepromRequestArg);
                    break;
                case 3:
                    save_eraseSlot(gEepromRequestArg);
                    break;
                case 4:
                    save_readModeSettings();
                    break;
                case 5:
                    save_writeModeSettings();
                    break;
                case 6:
                    save_readSettings();
                    break;
                case 7:
                    save_writeSettings();
                    break;
            }
            gEepromRequest = 0;
            if (gSaveEntryBits != NULL) {
                heap_free(gSaveEntryBits - 8);
                gSaveEntryBits = NULL;
            }
        }
    }
}
