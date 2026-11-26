#ifndef CONKER_CONT_H
#define CONKER_CONT_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "cont": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init/Cont/Motor/motor.c
 *   init/Cont/MotorInit/motorinit.c
 *   game/Cont/PadInit/padinit.c
 *   game/Cont/PadRead/padread.c
 *   game/Cont/Rumble/rumble.c
 */

/* This build's libultra has osMotorStart and osMotorStop as functions (init/Cont/Motor/motor.c), not the
 * __osMotorAccess macros of PR/os_motor.h. */
#undef osMotorStart
#undef osMotorStop

/* A controller pak command block in the PIF RAM (init/Cont/Motor/motor.c, init/Cont/MotorInit/motorinit.c). */
typedef struct PakReply {
    u8 dummy;
    u8 txSize;
    u8 rxSize;
    u8 command;
    u16 address;
    u8 data[32];
    u8 dataCrc;
} PakReply;

/* Rumble effect objects (game/Cont/Rumble/rumble.c): a timed rumble on the controllers of the players in a mask. */

/* Distance falloff data of a RumbleFalloffObj. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 innerRadius;  /* full strength inside this distance */
    /* 0x10 */ f32 falloffRange; /* zero strength beyond innerRadius + falloffRange */
    /* 0x14 */ f32 falloffScale; /* strength lost per unit beyond innerRadius */
    /* 0x18 */ u8 view;
} RumbleFalloff;

typedef struct {
    /* 0x00 */ u8 pad0[0xE];    /* object header */
    /* 0x0E */ RumbleParams params;
    /* 0x16 */ u8 appliedStrength;
    /* 0x17 */ u8 pad17;
} RumbleObj; /* followed by the spawner's extra data */

/* A rumble whose strength falls off with distance from the camera. */
typedef struct {
    /* 0x00 */ RumbleObj base;
    /* 0x18 */ RumbleFalloff falloff;
} RumbleFalloffObj;

/* init/Cont/Motor/motor.c */
s32 osMotorStop(OSPfs *device);
s32 osMotorStart(OSPfs *device);
extern u8 _MotorStopData[MAXCONTROLLERS][64]; // per channel: the PIF command block that stops the motor
extern u8 _MotorStartData[MAXCONTROLLERS][64]; // per channel: the PIF command block that starts the motor

/* init/Cont/MotorInit/motorinit.c: osMotorInit is declared by PR/os_motor.h. */

/* game/Cont/PadInit/padinit.c */
void cont_initSiQueue(void);
void cont_init(void);

/* game/Cont/PadRead/padread.c */
void cont_startReadTimer(void);
OSContPad **cont_readPads(void);
void cont_startRumble(u8 player, u8 strength);
void cont_startRumbleCycle(u8 player, f32 onTime, f32 offTime);
void cont_stopRumble(u8 player);
void cont_updateRumble(void);
void cont_stopAllRumble(void);
void cont_runEepromRequest(void);
extern u8 gPlayerChannels[4]; // the controller channel of each player; 4 or more: none
extern u8 gContReadDone;    // a controller read has finished
extern OSTimer gContReadTimer; // the timer that starts the next controller read
extern u16 gButtonsHeld[4]; // per player: the buttons held at the last read
extern u16 gMenuButtonsHeld[4]; // per player: the buttons held at the last read, for gMenuButtonsPressed
extern u16 gButtonsPressed[4]; // per player: the newly pressed buttons
extern u16 gButtonsPrevious[4]; // per player: the buttons held before this read
extern u16 gPendingButtons[4]; // per channel: buttons to add at the next read
extern OSContPad **gPadTable; // the per-player pads (the table that cont_readPads returns)
extern s32 gNumControllers; // the number of connected controllers
extern OSContPad *gPadPtrs[4]; // per player: a pointer into gPads
extern u8 gConnectedPads; // a bit for each controller that answered the last read
extern OSContPad gPads[4]; // the per-player pads
extern OSPfs gRumblePaks[MAXCONTROLLERS]; // the rumble pak of each channel
extern OSMesgQueue gSiMesgQueue; // the SI message queue
extern OSContPad gMenuPads[4]; // the per-player pads with the stick dead zone (menu reads)
extern u16 gMenuButtonsPressed[4]; // per player: the newly pressed buttons (menu reads)
extern u8 gRumbleRedetect; // look for rumble paks at the next update
extern u8 gRumbleOn[MAXCONTROLLERS]; // per channel: the rumble is on
extern u8 gRumbleWasOn[MAXCONTROLLERS]; // per channel: the rumble was on at the last update
extern u8 gRumblePakPresent[MAXCONTROLLERS]; // per channel: a rumble pak is present
extern u8 gMotorRunning[MAXCONTROLLERS]; // per channel: the motor is running
extern f32 gRumbleTime[MAXCONTROLLERS]; // per channel: the time in the rumble cycle
extern f32 gRumbleOnTime[MAXCONTROLLERS]; // per channel: the on time of the rumble cycle
extern f32 gRumbleOffTime[MAXCONTROLLERS]; // per channel: the off time of the rumble cycle
extern f32 gRumblePeriod[MAXCONTROLLERS]; // per channel: the length of the rumble cycle
extern OSMesg gSiMesg; // the message received from gSiMesgQueue

/* game/Cont/Rumble/rumble.c */
extern u8 gRumbleSuppressed; /* set by cont_killAllRumble (all rumble off); game/Cont/PadRead/padread keeps the motors off while set */
s32 cont_hasRumblePak(u8 playerMask);
RumbleObj *cont_spawnRumble(RumbleParams *params, s32 dataSize, u8 arg2, s32 arg3);
void cont_updateRumbleObj(RumbleObj *obj);
void cont_stopRumbleObj(RumbleObj *obj);
void cont_destroyRumbleObj(RumbleObj *obj);
void cont_freeRumbleObj(RumbleObj *obj);
void cont_updateRumbleFalloff(RumbleFalloffObj *obj);
void cont_applyRumbleFalloff(RumbleObj *obj, RumbleFalloff *falloff);
void cont_handleRumbleMessage(RumbleObj *obj, s32 data, u8 message);
void cont_sendRumbleMessage(s32 data, u8 message);
void cont_killAllRumble(void);

#endif
