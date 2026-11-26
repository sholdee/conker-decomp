#include <ultra64.h>
#include "functions.h"
#include "variables.h"

u8 D_80084060[4] = { 0, 1, 2, 3 }; // the controller channel of each player; 4 or more: none
u8 D_80084064 = 1;                 // a controller read has finished: func_1501BB20 arms the timer for the next
static u8 D_80084068 = 3;          // the EEPROM probe result (2: a 16K EEPROM); 3 until the first probe

extern OSTimer D_800BE6E0;


/* After each controller read (func_1501BBB8 sets D_80084064), arms a one-shot timer of
   25 * osClockRate / 1000000 ticks that posts message 6 to D_8003B218. */
void func_1501BB20(void) {
    if (D_80084064) {
        osSetTimer(&D_800BE6E0, 25ULL * osClockRate / 1000000ULL, 0, &D_8003B218, (OSMesg)6);
        D_80084064 = 0;
    }
}

extern u16 D_800BE700[];
extern u16 D_800BE708[];
extern u16 D_800BE718[];


/* Reads the controllers into the per-player pads (D_80084060 maps player slots to channels),
   records newly pressed buttons, applies the stick dead zone and returns the pad pointer table. */
OSContPad **func_1501BBB8(void) {
    s32 pad; // matching: unused stack slot in the original frame
    s32 player;
    s32 replaying;
    s32 numPlayers;
    OSContPad pads[4];
    s32 i;
    s32 x;
    s32 y; // also carries the button word

    for (i = 0; i < 4; i++) {
        D_800BE718[i] = D_800BE748[i].button;
    }
    osRecvMesg(&D_800BE900, &D_800BE990, 1);
    osContGetReadData(pads);
    func_1501C57C();
    D_80084064 = 1;
    D_800BE740 = 0;
    for (i = 0; i < 4; i++) {
        if (pads[i].errno != 0) {
            pads[i].button = 0;
            pads[i].stick_x = 0;
            pads[i].stick_y = 0;
        } else {
            D_800BE740 |= 1 << i;
        }
    }
    if (level != 0x1D) {
        numPlayers = D_8008FD90;
    } else {
        numPlayers = 4;
    }
    for (player = 0; player < 4; player++) {
        i = D_80084060[player];
        if (i < 4 && player < numPlayers) {
            D_800BE748[player].button = pads[i].button;
            D_800BE748[player].stick_x = pads[i].stick_x;
            D_800BE748[player].stick_y = pads[i].stick_y;
            D_800BE748[player].button |= D_800BE720[i];
            D_800BE720[i] = 0;
        } else {
            D_800BE748[player].button = 0;
            D_800BE748[player].stick_x = 0;
            D_800BE748[player].stick_y = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        y = D_800BE748[i].button;
        D_800BE930[i] = y & ~D_800BE708[i];
        D_800BE918[i].button = y;
        D_800BE708[i] = y;
        x = D_800BE748[i].stick_x;
        if (((x < 0) ? -x : x) >= 8) {
            D_800BE918[i].stick_x = x - ((x < 0) ? -1 : 1) * 7;
        } else {
            D_800BE918[i].stick_x = 0;
        }
        y = D_800BE748[i].stick_y;
        if (((y < 0) ? -y : y) >= 8) {
            D_800BE918[i].stick_y = y - ((y < 0) ? -1 : 1) * 7;
        } else {
            D_800BE918[i].stick_y = 0;
        }
    }
    if (D_800E0B94 == 2 || D_800E0B95 == 2) {
        replaying = 1;
        func_151DD8C0();
    } else {
        replaying = 0;
    }
    for (i = 0; i < 4; i++) {
        y = D_800BE748[i].button;
        D_800BE710[i] = y & ~D_800BE700[i];
        D_800BE700[i] = y;
        if (!replaying) {
            x = D_800BE748[i].stick_x;
            if (((x < 0) ? -x : x) >= 8) {
                D_800BE748[i].stick_x = x - ((x < 0) ? -1 : 1) * 7;
            } else {
                D_800BE748[i].stick_x = 0;
            }
            y = D_800BE748[i].stick_y;
            if (((y < 0) ? -y : y) >= 8) {
                D_800BE748[i].stick_y = y - ((y < 0) ? -1 : 1) * 7;
            } else {
                D_800BE748[i].stick_y = 0;
            }
        }
    }
    return D_800BE730;
}


/* Starts rumble on a player's pak with an on/off pattern derived from strength (0-8). */
void func_1501C010(u8 player, u8 strength) {
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
    func_1501C0F0(player, onTime, offTime);
}

/* Starts a player's rumble cycle: motor on for onTime, then off for offTime, repeating. */
void func_1501C0F0(u8 player, f32 onTime, f32 offTime) {
    u8 channel = D_80084060[player];

    if (channel < 4 && D_800BE944[channel]) {
        D_800BE93C[channel] = 1;
        D_800BE950[channel] = 0.0f;
        D_800BE960[channel] = onTime;
        D_800BE970[channel] = offTime;
        D_800BE980[channel] = onTime + offTime;
    }
}

/* Stops a player's rumble cycle. */
void func_1501C17C(u8 player) {
    u8 channel = D_80084060[player];

    if (channel < 4) {
        D_800BE93C[channel] = 0;
    }
}


/* Per-frame rumble update: (re)detects rumble paks when D_800BE938 is set, forces motors off
   while D_800BEAC3 / D_800E0A00 are set, and otherwise drives each motor's on/off cycle. */
void func_1501C1B0(void) {
    s32 channel;
    s32 motorOn;

    for (channel = 0; channel < 4; channel++) {
        if (D_800BE938) {
            if (_MakeMotorData(&D_800BE900, &D_800BE760[channel], channel) == 0) {
                if (D_800BE944[channel] == 0) {
                    D_800BE944[channel] = 1;
                    D_800BE93C[channel] = 0;
                    D_800BE940[channel] = 0;
                    D_800BE948[channel] = 0;
                    D_800BE950[channel] = 0.0f;
                    D_800BE960[channel] = 1.0f;
                    D_800BE970[channel] = 1.0f;
                    D_800BE980[channel] = 2.0f;
                    osMotorStop(&D_800BE760[channel]);
                }
            } else {
                D_800BE944[channel] = 0;
            }
        }
        if (D_800BE944[channel]) {
            if (D_800BEAC3 || D_800E0A00) {
                _MakeMotorData(&D_800BE900, &D_800BE760[channel], channel);
                osMotorStop(&D_800BE760[channel]);
                D_800BE948[channel] = 0;
                D_800BE93C[channel] = 0;
            }
            if (!D_800BEAC1 && !D_800BEAC0 && !D_800BEAC2 && !D_800BEAC3 && !D_800E0A00) {
                switch (D_800BE93C[channel]) {
                case 0:
                    if (D_800BE940[channel] == 1) {
                        _MakeMotorData(&D_800BE900, &D_800BE760[channel], channel);
                        osMotorStop(&D_800BE760[channel]);
                        D_800BE948[channel] = 0;
                    }
                    break;
                case 1:
                    D_800BE950[channel] += D_800BE9A4;
                    while (D_800BE950[channel] > D_800BE980[channel]) {
                        D_800BE950[channel] -= D_800BE980[channel];
                    }
                    motorOn = (D_800BE960[channel] <= D_800BE950[channel]) ? 0 : 1;
                    if (motorOn) {
                        if (D_800BE948[channel] == 0) {
                            osMotorStart(&D_800BE760[channel]);
                        }
                    } else if (D_800BE948[channel] == 1) {
                        osMotorStop(&D_800BE760[channel]);
                    }
                    D_800BE948[channel] = motorOn;
                    break;
                }
            }
            D_800BE940[channel] = D_800BE93C[channel];
        }
    }
    D_800BE938 = 0;
    D_800BEAC3 = 0;
    D_800E0A00 = 0;
}

/* Stops rumble for every player. */
void func_1501C53C(void) {
    u8 player;

    for (player = 0; player < 4; player++) {
        func_1501C17C(player);
    }
}

/* Runs the pending EEPROM request. Without a 16K EEPROM (probe result D_80084068 != 2), load
   requests fall back to defaults and the others are dropped. */
void func_1501C57C(void) {
    s32 i;

    func_151DCFD8(0);
    if (D_80082BB4 != 0) {
        if (D_80084068 != 2) {
            D_80084068 = func_151DD460(&D_800BE900);
        }
        if (D_80084068 != 2) {
            // matching: empty switch emits the original's 7-entry jump table
            switch (D_80082BB4) {
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
            if (D_80082BB4 == 1) {
                func_150064E0();
            } else if (D_80082BB4 == 4) {
                func_15007168();
            }
            D_80082BB4 = 0;
        // matching: an unused label ends the block, so as1 keeps the store out of the branch delay slot
        end:;
        } else {
            switch (D_80082BB4) {
                case 1:
                    func_15006590(D_800BE3EC);
                    break;
                case 2:
                    func_15006BEC(D_800BE3EC);
                    break;
                case 3:
                    func_1500707C(D_800BE3EC);
                    break;
                case 4:
                    func_1500727C();
                    break;
                case 5:
                    func_15007360();
                    break;
                case 6:
                    func_15007440();
                    break;
                case 7:
                    func_15007558();
                    break;
            }
            D_80082BB4 = 0;
            if (D_800BE3F0 != NULL) {
                func_10004074(D_800BE3F0 - 8);
                D_800BE3F0 = NULL;
            }
        }
    }
}
