#ifndef CONKER_SCRIPT_H
#define CONKER_SCRIPT_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "script": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_A28B0.c
 *   game_C3E20.c
 *   game_C4DC0.c
 *   game_C8950.c
 *   game_C98F0.c
 *   game_C9EC0.c
 *   game_CB1C0.c
 *   game_CB3D0.c
 *   game_CBAF0.c
 *   game_CBDB0.c
 *   game_CCD00.c
 *   game_CD5A0.c
 *   game_CD860.c
 */

/* One argument word of a script command; handlers also read just its low halfword or low byte. */
typedef union CmdArg {
    s32 w;
    struct {
        s16 hi;
        s16 lo;
    } h;
    struct {
        u8 b0;
        u8 b1;
        u8 b2;
        u8 lo;
    } b;
} CmdArg;

/* A script command record: a command word, then its argument words. The interpreter (game_DAFA0.s) passes
 * the record to the start/end handlers of D_80088420 / D_8008845C (func_1509D180, func_1509D780) as
 * (id, cmd), and to the handlers of D_80088498 / D_800884D4 as (id, arg, cmd); the dispatchers
 * func_1509BE40 / func_1509BFB0 build one in D_800D2F60. Arguments that are speeds, scales or distances are
 * 16.16 fixed point (hence the many `* 1.52587890625e-05f`, i.e. 1/65536); angles in binary units convert
 * to degrees with 0.0054931640625f (360/65536) and back with 182.04444885f. */
typedef struct ScriptCmd {
    /* 0x00 */ s32 id;     /* bits 12-15: handler table + 1, bits 0-11: command or object id */
    /* 0x04 */ CmdArg arg; /* the handler's opcode, phase or first argument */
    /* 0x08 */ CmdArg a[7]; /* the parameters (D_800D3840 - 2 of them) */
} ScriptCmd;

/* Object id arguments carry flags above the low 12 bits. */
#define CMD_OBJECT_ID(arg) ((arg).w & 0xFFF)

/* Types that these files define; the declarations below use them. */
typedef struct C4DC0_arg1 C4DC0_arg1;
typedef struct Struct_1509C440 Struct_1509C440;
typedef struct CommandArgs CommandArgs;
typedef struct QueryArgs QueryArgs;
typedef struct Src150A0264 Src150A0264;
typedef struct Msg_CD860 Msg_CD860;
struct CB3D0_arg1;
struct CB3D0_arg2;

/* game_A28B0.c */
void func_15075400(s32 target);
void func_15075498(void);
void func_15075548(void);
void func_15075650(void);
void func_15075884(void);
void func_15075938(void);
void func_15075A50(void);
void func_15075AAC(void);
void func_15075B60(void);
void func_15075B8C(void);
void func_15075BB8(void);
void func_15075BE8(void);
void func_15075C24(void);
void func_15075CA0(void);
void func_15075CCC(void);
void func_15075D0C(void);
void func_15075D38(void);
void func_15075D64(void);
void func_15075D9C(void);
void func_15075DC8(void);
void func_15075DE8(void);
void func_15075E6C(void);
void func_15075E98(void);
void func_15075EB4(void);
void func_15075F00(void);
void func_15075F40(void);
void func_15075F6C(void);
void func_1507602C(void);
void func_150761C8(void);
void func_150761F4(void);
void func_15076220(void);
void func_15076250(void);
void func_150762B0(void);
void func_150762D4(void);
void func_15076340(void);
void func_15076394(void);
void func_150763B0(void);
void func_150764C8(void);
void func_150764E4(void);
void func_15076500(void);
void func_1507652C(void);
void func_1507659C(void);
void func_15076600(void);
void func_15076608(void);
void func_15076624(void);
void func_15076678(void);
void func_150766D0(void);
void func_15076760(void);
void func_15076768(void);
void func_150767F4(void);
void func_150768DC(void);
void func_15076B5C(void);
void func_15076B78(void);
void func_15076B94(void);
void func_15076C7C(void);
void func_15076CB4(void);
void func_15076CCC(void);
void func_15076D04(void);
void func_15076D3C(void);
void func_15076DF4(void);
void func_15076E10(void);
void func_15076E48(void);
void func_15076E50(void);
void func_15076F40(void);
void func_15076F78(void);
void func_15076FA8(void);
void func_150770E4(void);
void func_15077174(void);
void func_15077190(void);
void func_150771F0(void);
void func_15077294(void);
void func_150772E8(void);
void func_15077318(void);
void func_1507735C(void);
void func_15077364(void);
void func_15077404(void);
void func_150774B4(void);
void func_15077508(void);
void func_150778F0(void);
void func_150779A8(void);
void func_150779D4(void);
void func_15077AA0(void);
void func_15077ABC(void);
void func_15077B14(void);
void func_15077B44(void);
void func_15077B80(void);
void func_15077BB4(void);
void func_15077BE4(void);
void func_15077C1C(void);
void func_15077C38(void);
void func_15077DA0(void);
void func_15077DBC(void);
void func_15077E9C(void);
void func_15077EEC(void);
void func_15077F08(void);
void func_15077F34(void);
void func_15077F64(void);
void func_15078074(void);
void func_1507813C(void);
void func_150781A4(void);
void func_150781F4(void);
void func_150782CC(void);
void func_150782E8(void);
void func_15078358(void);
void func_1507839C(void);
void func_15078520(void);
void func_15078544(void);
void func_1507879C(void);
void func_15078874(void);
void func_15078890(void);
void func_15078900(void);
void func_15078A08(void);
s32 func_15078A60(s32 index);
void func_1507900C(void);
void func_1507903C(void);
void func_15079090(void);
void func_150790C4(void);
void func_1507911C(void);
void func_150791F0(void);
void func_15079228(void);
void func_150792E0(void);
void func_150792FC(void);
void func_15079334(void);
void func_15079390(void);
void func_150793D8(void);
void func_15079570(void);
void func_1507965C(void);
void func_150796CC(void);
void func_15079790(void);
void func_15079880(void);
void func_150798F8(void);
void func_15079928(void);
void func_15079988(void);
void func_150799B4(void);
void func_15079A28(void);
void func_15079A58(void);
u16 func_15079A98(s32 arg0);
void func_15079B30(void);
void func_15079F24(void);
void func_15079F50(void);
void func_15079F6C(void);
void func_15079FBC(void);
void func_1507A100(void);
void func_1507A164(void);
void func_1507A210(void);
void func_1507A270(void);
void func_1507A2A4(void);
void func_1507A2F8(void);
void func_1507A3B4(void);
void func_1507A3CC(void);
s32 func_1507A3E8(void);
void func_1507A428(void);
void func_1507A47C(void);
void func_1507A4D4(void);
void func_1507A528(void);
void func_1507A620(void);
s32 func_1507A6FC(s32 arg0);
void func_1507A71C(void);
void func_1507A774(void);
void func_1507A7C0(void);
void func_1507A7DC(void);
void func_1507A808(void);
void func_1507A838(void);
void func_1507A878(void);
void func_1507A8A8(void);
void func_1507A8EC(void);
void func_1507A984(void);
void func_1507AA48(void);
void func_1507ACB0(void);
void func_1507ACE0(void);
void func_1507AD30(void);
void func_1507AD60(void);
void func_1507AD7C(void);
void func_1507ADAC(void);
void func_1507ADE0(void);
void func_1507AE78(void);
void func_1507AE94(void);
void func_1507AF3C(void);
void func_1507AF98(void);
void func_1507B040(void);
void func_1507B058(void);
void func_1507B15C(void);
void func_1507B178(void);
void func_1507B234(void);
void func_1507B280(void);
void func_1507B2CC(void);
void func_1507B318(void);
void func_1507B364(void);
void func_1507B3B0(void);
void func_1507B3FC(void);
void func_1507B448(void);
void func_1507B494(void);
void func_1507B4E0(void);
void func_1507B52C(void);
void func_1507B578(void);
void func_1507B5C4(void);
void func_1507B630(void);
void func_1507B6E0(void);
void func_1507B734(void);
void func_1507B7BC(void);
void func_1507B7E8(void);
void func_1507B884(void);
void func_1507B8F4(void);
void func_1507B958(void);
void func_1507B974(void);
void func_1507BA48(void);
void func_1507BAD0(void);
void func_1507BAF8(void);
void func_1507BB20(void);
const ObjScriptCmd *func_1507BB28(s32 arg0, s32 scriptId);
void func_1507BC14(Actor *obj);
extern u8 D_800D1890; // argument bytes of the current script command (arg0)
extern f32 D_800D1898[2];

/* game_C3E20.c */
extern f32 D_800D2DB0;
extern s32 D_800D2DB4; // a command is running
void func_15096970(void);
s32 func_150969A0(s32 count);
s32 func_15096A68(s32 slot);
void func_15096D08(void);
s32 func_15096D78(s32 actorIndex, s32 command, s32 *args);
s32 func_1509759C(s32 actorIndex, s32 query, s32 *args);
void func_15097798(s32 actorIndex);

/* game_C4DC0.c */
s32 func_15097910(s32 scriptId, s32 unused);
s32 func_150979CC(s32 objId, C4DC0_arg1 *arg1);
s32 func_15097A8C(s32 objId, s32 opcode, ScriptCmd *cmd);
s32 func_15099C14(s32 objId, s32 opcode, ScriptCmd *cmd);

/* game_C8950.c */
void func_1509B4A0(s32 unused, s32 reload);
void *func_1509B570(s32 id);
IdRecord *func_1509B5AC(s32 id, s32 reload);
IdRecord *func_1509B704(s16 id);
void func_1509B764(IdRecord *node);
void func_1509B810(IdRecord *node);
IdRecord *func_1509B8FC(s16 id);
IdRecord *func_1509B950(IdRecord *node);
void func_1509BA04(s32 unloadAll);
void func_1509BBA0(s32 mode);
s32 func_1509BE40(s32 count, s32 cmdWord, s32 arg, ...);
s32 func_1509BFB0(s32 count, s32 cmdWord, s32 arg, ...);
void func_1509C120(void);
void func_1509C228(void);
s32 func_1509C2A4(void);
void func_1509C3A0(void);
s32 func_1509C414(s32 index);
extern s32 D_800D2F60[16];

/* game_C98F0.c */
s32 func_1509C440(s32 id, s32 event, Struct_1509C440 *params);
s32 func_1509C7C8(s32 id, s32 event, s32 unused);
void func_1509C8FC(s32 ref, s32 alt);

/* game_C9EC0.c */
const char *func_1509CA10(s32 index);
u16 func_1509CA30(s32 index);
s32 func_1509CA50(s32 index);
s32 func_1509CA78(s32 index);
s32 func_1509CA98(s32 id);
s32 func_1509CB68(void);
s32 func_1509CBD4(s32 index);
void func_1509CC94(s32 index);
void func_1509CCB4(s32 index);
s32 func_1509CCF4(s32 index);
void func_1509CDDC(s32 index);
void func_1509CE64(u16 *list, void (*callback)(s32));
s32 func_1509CF28(s32 arg0, s32 *countOut);
void func_1509D054(void);
s32 func_1509D08C(s32 unused, s32 index, s32 *firstDepOut, s32 *unk0Out);
s32 func_1509D180(s32 cmdId, ScriptCmd *cmd);
s32 func_1509D780(s32 cmdId, ScriptCmd *cmd);
s32 func_1509D8FC(s32 cmdId, s32 phase, ScriptCmd *cmd);
s32 func_1509DBBC(s32 cmdId, s32 phase, s32 arg2);

/* game_CB1C0.c */
s32 func_1509DD10(s32 arg0);
s32 func_1509DD20(s32 arg0);
s32 func_1509DD30(s32 arg0);
s32 func_1509DD40(s32 arg0);
s32 func_1509DD50(s32 arg0, SwarmSpawnCmd *cmd);
s32 func_1509DDC4(s32 tag, u16 arg1);
s32 func_1509DDFC(s32 tag, s32 op, SwarmTargetCmd *cmd);
s32 func_1509DEC4(s32 tag, s32 op, SwarmTargetCmd *cmd);

/* game_CB3D0.c */
void func_1509DF20(s32 arg0, struct CB3D0_arg1 *record);
void func_1509DFB4(s32 arg0, s32 arg1);
s32 func_1509DFC4(s32 arg0, s32 cmd, struct CB3D0_arg2 *args);
s32 func_1509E3DC(s32 arg0, s32 cmd, struct CB3D0_arg2 *args);

/* game_CBAF0.c */
s32 func_1509E640(s32 id, s32 opcode, ScriptCmd *cmd);
s32 func_1509E6F0(s32 id, s32 opcode, ScriptCmd *cmd);
s32 func_1509E730(s32 id, u32 opcode, ScriptCmd *cmd);
s32 func_1509E8A0(s32 id, s32 opcode, ScriptCmd *cmd);

/* game_CBDB0.c */
s32 func_1509E900(s32 id, s32 op, s32 *cmd);
s32 func_1509ED74(s32 id, s32 op, s32 *cmd);
s32 func_1509EFF0(s32 sound, u32 *cmd);
s32 func_1509F248(s32 arg0, u32 *cmd);
s32 func_1509F284(s32 arg0, s32 op, u32 *cmd);
s32 func_1509F33C(s32 arg0, s32 arg1, s32 arg2);
s32 func_1509F354(s32 first, s32 last, s32 unplayed, u32 volume, s32 path, s32 nearDistance, s32 farDistance);
void func_1509F47C(s32 first, s32 last, s32 volume, s32 objId, s32 nearDistance, s32 farDistance);
s32 func_1509F4E8(s32 first, s32 last, s32 volume, s32 index, u32 nearDistance);
s32 func_1509F5A8(s32 first, s32 last, s32 volume);
void func_1509F5F4(s32 sound, s32 volume, s32 path, s32 nearDistance, s32 farDistance, s32 force);
void func_1509F660(s32 path, s32 secondVoice);
s32 func_1509F6B0(s32 sound, s32 volume, s32 cents, s32 pan);
s32 func_1509F6E8(s32 sound, s32 path, s32 volume, s32 nearDistance, s32 farDistance);
void func_1509F77C(s32 handle, s32 path, s32 cents, s32 nearDistance, s32 farDistance);
s32 func_1509F800(s32 arg0, s32 arg1, s32 arg2);
s32 func_1509F818(s32 arg0, s32 arg1, s32 arg2);

/* game_CCD00.c */
s32 func_1509F850(s32 id, s32 cmd, CommandArgs *args);
s32 func_1509FE0C(s32 id, s32 query, QueryArgs *args);

/* game_CD5A0.c */
void func_150A00F0(void);
void func_150A019C(void);
s32 func_150A0264(s32 arg0, Src150A0264 *arg1);
s32 func_150A02D0(s32 arg0, s32 arg1, Src150A0264 *arg2);
s32 func_150A0374(s32 arg0, s32 arg1, s32 arg2);

/* game_CD860.c */
s32 func_150A03B0(s32 slot, u32 command, Msg_CD860 *msg);
s32 func_150A04D8(s32 slot, s32 query, Msg_CD860 *msg);

#endif
