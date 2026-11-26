#ifndef CONKER_CONT_H
#define CONKER_CONT_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "cont": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init_5570.c
 *   init_57E0.c
 *   game_33660.c
 *   game_48FD0.c
 *   game_205C90.c
 */

/* This build's libultra has osMotorStart and osMotorStop as functions (init_5570.c), not the
 * __osMotorAccess macros of PR/os_motor.h. */
#undef osMotorStart
#undef osMotorStop

/* A controller pak command block in the PIF RAM (init_5570.c, init_57E0.c). */
typedef struct PakReply {
    u8 dummy;
    u8 txSize;
    u8 rxSize;
    u8 command;
    u16 address;
    u8 data[32];
    u8 dataCrc;
} PakReply;

/* Rumble effect objects (game_205C90.c): a timed rumble on the controllers of the players in a mask. */

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

/* init_5570.c */
s32 osMotorStop(OSPfs *device);
s32 osMotorStart(OSPfs *device);
extern u8 D_8003BC30[MAXCONTROLLERS][64]; // per channel: the PIF command block that stops the motor
extern u8 D_8003BD30[MAXCONTROLLERS][64]; // per channel: the PIF command block that starts the motor

/* init_57E0.c. Its osMotorInit has another signature than the one in PR/os_motor.h, so only
 * init_57E0.c declares it. */
s32 _MakeMotorData(OSMesgQueue *queue, OSPfs *device, s32 channel);

/* game_33660.c */
void func_150061B0(void);
void func_15006234(void);

/* game_48FD0.c */
void func_1501BB20(void);
OSContPad **func_1501BBB8(void);
void func_1501C010(u8 player, u8 strength);
void func_1501C0F0(u8 player, f32 onTime, f32 offTime);
void func_1501C17C(u8 player);
void func_1501C1B0(void);
void func_1501C53C(void);
void func_1501C57C(void);
extern u8 D_80084060[4]; // the controller channel of each player; 4 or more: none
extern u8 D_80084064;    // a controller read has finished

/* game_205C90.c */
s32 func_151D87E0(u8 playerMask);
RumbleObj *func_151D8868(RumbleParams *params, s32 dataSize, u8 arg2, s32 arg3);
void func_151D8A24(RumbleObj *obj);
void func_151D8B24(RumbleObj *obj);
void func_151D8B88(RumbleObj *obj);
void func_151D8BB4(RumbleObj *obj);
void func_151D8BE0(RumbleFalloffObj *obj);
void func_151D8C00(RumbleObj *obj, RumbleFalloff *falloff);
void func_151D8D5C(RumbleObj *obj, s32 data, u8 message);
void func_151D8DB4(s32 data, u8 message);
void func_151D8DE8(void);

#endif
