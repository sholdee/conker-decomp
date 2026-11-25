#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* This build's libultra exports osMotorStart/osMotorStop/_MakeMotorData as real
   functions instead of the __osMotorAccess macro that os_motor.h defines. */
#undef osMotorStart
#undef osMotorStop
extern s32 osMotorStart(OSPfs *pfs);
extern s32 osMotorStop(OSPfs *pfs);
extern s32 _MakeMotorData(OSMesgQueue *mq, OSPfs *pfs, s32 channel);

extern u8 D_80084060[];
extern OSTimer D_800BE6E0;
extern u8 D_800BEAC2;
extern u8 D_800BEAC3;
extern u8 D_800E0A00;

void func_1501C0F0(u8 arg0, f32 arg1, f32 arg2);

/* Arms the controller-read timer (25 * osClockRate / 1000000 ticks, i.e. 25 us) once per
   frame, after func_1501BBB8 has consumed the previous read. */
void func_1501BB20(void) {
    if (D_80084064 != 0) {
        osSetTimer(&D_800BE6E0, 25ULL * D_8002BD10 / 1000000ULL, 0, &D_8003B218, (OSMesg)6);
        D_80084064 = 0;
    }
}

extern u16 D_800BE700[];
extern u16 D_800BE708[];
extern u16 D_800BE718[];
extern u8 D_800BE740;
extern OSContPad D_800BE918[];
extern u16 D_800BE930[];
extern u8 D_800E0B95;
extern OSMesg D_800BE990;
void func_1501C57C(void);
void func_151DD8C0(void);

#define PADS ((OSContPad *)D_800BE748)

/* Reads the controllers into the per-player pads (D_80084060 maps player slots to channels),
   records newly pressed buttons, applies the stick dead zone and returns the pad pointer table. */
struct14 *func_1501BBB8(void) {
    s32 pad; // matching: unused stack slot in the original frame
    s32 player;
    s32 replaying;
    s32 numPlayers;
    OSContPad pads[4];
    s32 i;
    s32 x;
    s32 y; // also carries the button word

    for (i = 0; i < 4; i++) {
        D_800BE718[i] = PADS[i].button;
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
    if (D_800BE9F0 != 0x1D) {
        numPlayers = D_8008FD90;
    } else {
        numPlayers = 4;
    }
    for (player = 0; player < 4; player++) {
        i = D_80084060[player];
        if (i < 4 && player < numPlayers) {
            PADS[player].button = pads[i].button;
            PADS[player].stick_x = pads[i].stick_x;
            PADS[player].stick_y = pads[i].stick_y;
            PADS[player].button |= D_800BE720[i];
            D_800BE720[i] = 0;
        } else {
            PADS[player].button = 0;
            PADS[player].stick_x = 0;
            PADS[player].stick_y = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        y = PADS[i].button;
        D_800BE930[i] = y & ~D_800BE708[i];
        D_800BE918[i].button = y;
        D_800BE708[i] = y;
        x = PADS[i].stick_x;
        if (((x < 0) ? -x : x) >= 8) {
            D_800BE918[i].stick_x = x - ((x < 0) ? -1 : 1) * 7;
        } else {
            D_800BE918[i].stick_x = 0;
        }
        y = PADS[i].stick_y;
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
        y = PADS[i].button;
        D_800BE710[i] = y & ~D_800BE700[i];
        D_800BE700[i] = y;
        if (!replaying) {
            x = PADS[i].stick_x;
            if (((x < 0) ? -x : x) >= 8) {
                PADS[i].stick_x = x - ((x < 0) ? -1 : 1) * 7;
            } else {
                PADS[i].stick_x = 0;
            }
            y = PADS[i].stick_y;
            if (((y < 0) ? -y : y) >= 8) {
                PADS[i].stick_y = y - ((y < 0) ? -1 : 1) * 7;
            } else {
                PADS[i].stick_y = 0;
            }
        }
    }
    return &D_800BE730;
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
        scaled = ((f32)strength - 1.0f) * (1.0f / 6.0f) * 5.0f;
        onTime = scaled + 2.0f;
        offTime = 7.0f - scaled;
    }
    func_1501C0F0(player, onTime, offTime);
}

void func_1501C0F0(u8 arg0, f32 arg1, f32 arg2) {
    u8 temp = D_80084060[arg0];

    if (temp < 4) {
        if (D_800BE944[temp] != 0) {
            D_800BE93C[temp] = 1;
            D_800BE950[temp] = 0.0f;
            D_800BE960[temp] = arg1;
            D_800BE970[temp] = arg2;
            D_800BE980[temp] = arg1 + arg2;
        }
    }
}

void func_1501C17C(u8 arg0) {
    u8 temp = D_80084060[arg0];
    if (temp < 4) {
        D_800BE93C[temp] = 0;
    }
}

void func_1501C1B0(void) {
    s32 i;
    s32 on;

    for (i = 0; i < 4; i++) {
        if (D_800BE938 != 0) {
            if (_MakeMotorData(&D_800BE900, (OSPfs *)&D_800BE760[i], i) == 0) {
                if (D_800BE944[i] == 0) {
                    D_800BE944[i] = 1;
                    D_800BE93C[i] = 0;
                    D_800BE940[i] = 0;
                    D_800BE948[i] = 0;
                    D_800BE950[i] = 0.0f;
                    D_800BE960[i] = 1.0f;
                    D_800BE970[i] = 1.0f;
                    D_800BE980[i] = 2.0f;
                    osMotorStop((OSPfs *)&D_800BE760[i]);
                }
            } else {
                D_800BE944[i] = 0;
            }
        }
        if (D_800BE944[i] != 0) {
            if ((D_800BEAC3 != 0) || (D_800E0A00 != 0)) {
                _MakeMotorData(&D_800BE900, (OSPfs *)&D_800BE760[i], i);
                osMotorStop((OSPfs *)&D_800BE760[i]);
                D_800BE948[i] = 0;
                D_800BE93C[i] = 0;
            }
            if ((D_800BEAC1 == 0) && (D_800BEAC0 == 0) && (D_800BEAC2 == 0) && (D_800BEAC3 == 0) && (D_800E0A00 == 0)) {
                switch (D_800BE93C[i]) {
                case 0:
                    if (D_800BE940[i] == 1) {
                        _MakeMotorData(&D_800BE900, (OSPfs *)&D_800BE760[i], i);
                        osMotorStop((OSPfs *)&D_800BE760[i]);
                        D_800BE948[i] = 0;
                    }
                    break;
                case 1:
                    D_800BE950[i] += D_800BE9A4;
                    while (D_800BE950[i] > D_800BE980[i]) {
                        D_800BE950[i] -= D_800BE980[i];
                    }
                    if (D_800BE960[i] <= D_800BE950[i]) {
                        on = 0;
                    } else {
                        on = 1;
                    }
                    if (on != 0) {
                        if (D_800BE948[i] == 0) {
                            osMotorStart((OSPfs *)&D_800BE760[i]);
                        }
                    } else {
                        if (D_800BE948[i] == 1) {
                            osMotorStop((OSPfs *)&D_800BE760[i]);
                        }
                    }
                    D_800BE948[i] = on;
                    break;
                }
            }
            D_800BE940[i] = D_800BE93C[i];
        }
    }
    D_800BE938 = 0;
    D_800BEAC3 = 0;
    D_800E0A00 = 0;
}

void func_1501C53C(void) {
    u8 i;
    for (i = 0; i < 4; i++) {
        func_1501C17C(i);
    }
}

extern u8 D_80084068;
extern u8 *D_800BE3F0;
void func_151DCFD8(s32);
u8 func_151DD460(OSMesgQueue *);
void func_15007168(void);
void func_15006590(s32);
void func_15006BEC(s32);
void func_1500707C(s32);
void func_1500727C(void);
void func_15007360(void);
void func_15007440(void);
void func_15007558(void);
void func_10004074(void *);

void func_1501C57C(void) {
    s32 i;

    func_151DCFD8(0);
    if (*(s8 *)&D_80082BB4 != 0) {
        if (D_80084068 != 2) {
            D_80084068 = func_151DD460(&D_800BE900);
        }
        if (D_80084068 != 2) {
            switch (*(s8 *)&D_80082BB4) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    break;
            }
            i = 19;
            loop:
            if (i > 0) {
                i--;
                goto loop;
            }
            if (*(s8 *)&D_80082BB4 == 1) {
                func_150064E0();
            } else if (*(s8 *)&D_80082BB4 == 4) {
                func_15007168();
            }
            D_80082BB4 = 0;
        lbl:;
        } else {
            switch (*(s8 *)&D_80082BB4) {
                case 1:
                    func_15006590(*(s8 *)&D_800BE3EC);
                    break;
                case 2:
                    func_15006BEC(*(s8 *)&D_800BE3EC);
                    break;
                case 3:
                    func_1500707C(*(s8 *)&D_800BE3EC);
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
