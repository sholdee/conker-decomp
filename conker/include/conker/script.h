#ifndef CONKER_SCRIPT_H
#define CONKER_SCRIPT_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "script": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Script/ObjScript/objscript.c
 *   game/Script/CameraCmd/cameracmd.c
 *   game/Script/ActorCmd/actorcmd.c
 *   game/Script/TaskSet/taskset.c
 *   game/Script/CutsceneCmd/cutscenecmd.c
 *   game/Script/EffectCmd/effectcmd.c
 *   game/Script/SwarmCmd/swarmcmd.c
 *   game/Script/MiscCmd/misccmd.c
 *   game/Script/MusicCmd/musiccmd.c
 *   game/Script/ObjectCmd/objectcmd.c
 *   game/Script/TaskCmd/taskcmd.c
 *   game/Script/TimerCmd/timercmd.c
 *   game/Script/ZoneCmd/zonecmd.c
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

/* A script command record: a command word, then its argument words. The interpreter (game/Script/ScriptInterp/scriptinterp.s) passes
 * the record to the start/end handlers of gScriptStartHandlers / gScriptEndHandlers (script_startEffect, script_endEffect) as
 * (id, cmd), and to the handlers of gScriptQueryHandlers / gScriptCommandHandlers as (id, arg, cmd); the dispatchers
 * script_query / script_command build one in gScriptCmdBuffer. Arguments that are speeds, scales or distances are
 * 16.16 fixed point (hence the many `* 1.52587890625e-05f`, i.e. 1/65536); angles in binary units convert
 * to degrees with 0.0054931640625f (360/65536) and back with 182.04444885f. */
typedef struct ScriptCmd {
    /* 0x00 */ s32 id;     /* bits 12-15: handler table + 1, bits 0-11: command or object id */
    /* 0x04 */ CmdArg arg; /* the handler's opcode, phase or first argument */
    /* 0x08 */ CmdArg a[7]; /* the parameters (gScriptCmdWordCount - 2 of them) */
} ScriptCmd;

/* Object id arguments carry flags above the low 12 bits. */
#define CMD_OBJECT_ID(arg) ((arg).w & 0xFFF)

/* Types that these files define; the declarations below use them. */
typedef struct C4DC0_arg1 C4DC0_arg1;
typedef struct CommandArgs CommandArgs;
typedef struct QueryArgs QueryArgs;
typedef struct Src150A0264 Src150A0264;
typedef struct Msg_CD860 Msg_CD860;

/* game/Script/ObjScript/objscript.c */
void script_jump(s32 target);
void script_applyMoveFlags(void);
void script_startMove(void);
void script_pickWaypoint(void);
void script_startTimedMove(void);
void script_jumpIfWaypointCompare(void);
void script_jumpIfWaypoint(void);
void script_startMoveUnlessNear(void);
void func_15075B60(void);
void func_15075B8C(void);
void func_15075BB8(void);
void func_15075BE8(void);
void script_setYSpeedAndGravity(void);
void func_15075CA0(void);
void func_15075CCC(void);
void func_15075D0C(void);
void func_15075D38(void);
void func_15075D64(void);
void func_15075D9C(void);
void func_15075DC8(void);
void script_switchScript(void);
void func_15075E6C(void);
void func_15075E98(void);
void script_jumpRandom(void);
void func_15075F00(void);
void func_15075F40(void);
void func_15075F6C(void);
void script_leapToWaypoint(void);
void func_150761C8(void);
void func_150761F4(void);
void func_15076220(void);
void script_playPlayerSound(void);
void script_fadeFirstTwoChannels(void);
void script_spawnPuff(void);
void script_jumpIfNotHit(void);
void func_15076394(void);
void func_150763B0(void);
void func_150764C8(void);
void script_setHitRoute(void);
void func_15076500(void);
void script_setImmune(void);
void func_1507659C(void);
void script_emptyCommand61(void);
void func_15076608(void);
void script_setNoshoveFlags(void);
void script_clearNoshoveFlags(void);
void func_150766D0(void);
void script_emptyCommand70(void);
void func_15076768(void);
void script_jumpIfBehindTarget(void);
void script_findNearestInView(void);
void script_targetFoundObject(void);
void script_setDrawType(void);
void script_moveToLastOfType(void);
void func_15076C7C(void);
void func_15076CB4(void);
void func_15076CCC(void);
void script_setSpeed(void);
void script_setScale(void);
void script_setType(void);
void func_15076E10(void);
void script_emptyCommand113(void);
void script_startRandomJump(void);
void func_15076F40(void);
void func_15076F78(void);
void func_15076FA8(void);
void script_jumpIfTargetSlow(void);
void func_15077174(void);
void script_playUntrackedSound(void);
void script_runFadeCommand(void);
void script_setEnergy(void);
void script_breakIntoParts(void);
void func_15077318(void);
void script_doNothing(void);
void script_setAnimSpeedAndAccel(void);
void func_15077404(void);
void script_jumpIfAtDestination(void);
void script_stepPathGraph(void);
void script_advanceWaypoint(void);
void func_150779A8(void);
void script_jumpIfWithinRange(void);
void script_setHitRouteAlt(void);
void func_15077ABC(void);
void func_15077B14(void);
void script_playSoundOnSelf(void);
void script_setTaskAttrib(void);
void script_restartObjectOfType(void);
void script_clearTaskAttrib(void);
void func_15077C1C(void);
void script_findTarget(void);
void script_setWaypoint(void);
void script_warpToWaypoint(void);
void script_startTrackAt(void);
void func_15077EEC(void);
void func_15077F08(void);
void func_15077F34(void);
void script_turnToWaypoint(void);
void script_jumpIfAnyObjectNear(void);
void script_jumpIfTargetInactive(void);
void func_150781A4(void);
void script_jumpIfNearWaypoint(void);
void func_150782CC(void);
void script_jumpIfUniqueId(void);
void script_targetUniqueId(void);
void script_jumpIfTargetDistance(void);
void script_goto(void);
void script_placeWaypointNearTarget(void);
void script_jumpIfTargetAnimFrame(void);
void func_15078874(void);
void func_15078890(void);
void func_15078900(void);
void script_copyTargetAnimFrame(void);
s32 script_testProperty(s32 index);
void script_jumpIfPlayerProperty(void);
void script_switchIfPlayerProperty(void);
void script_jumpIfProperty(void);
void script_switchIfProperty(void);
void script_jumpIfCoordBelow(void);
void script_setForcedLod(void);
void func_15079228(void);
void script_setIntelWalk(void);
void func_150792FC(void);
void script_restartFoundObject(void);
void script_sendAttachmentMessage(void);
void script_placeWaypointNearPlayer(void);
void script_leapToTarget(void);
void script_restartById(void);
void func_150796CC(void);
void func_15079790(void);
void script_startMoveAtPlayerSpeed(void);
void script_jumpIfAtWaypoint(void);
void script_requestExpression(void);
void script_setDefaultExpression(void);
void script_playRandomSoundOnSelf(void);
void func_15079A28(void);
void script_setSpawnDelay(void);
u16 script_getAngleToPathStart(s32 arg0);
void script_changeObjectProperty(void);
void script_setSpeedup(void);
void func_15079F50(void);
void func_15079F6C(void);
void script_steerByPlayerStick(void);
void script_setWaypointCoord(void);
void script_setSpeedFromPlayer(void);
void func_1507A210(void);
void script_breakIntoPartsVariant(void);
void func_1507A2A4(void);
void script_targetRandomOfType(void);
void func_1507A3B4(void);
void func_1507A3CC(void);
s32 script_getArgWord(void);
void func_1507A428(void);
void func_1507A47C(void);
void func_1507A4D4(void);
void script_changeWaypointDirection(void);
void script_jumpIfAngleInRange(void);
s32 script_getPadButtons(s32 arg0);
void script_jumpIfButtonsHeld(void);
void script_jumpIfLevel(void);
void script_setNorun(void);
void script_removePartSet(void);
void func_1507A808(void);
void script_jumpIfEnergy(void);
void script_startViewEnvelope(void);
void func_1507A8A8(void);
void script_jumpIfTargetType(void);
void script_attachToObject(void);
void script_setFacing(void);
void script_setWaypointRange(void);
void func_1507ACE0(void);
void func_1507AD30(void);
void script_setFamily(void);
void script_setMvmtControl(void);
void script_clearMvmtControl(void);
void func_1507ADE0(void);
void func_1507AE78(void);
void func_1507AE94(void);
void func_1507AF3C(void);
void func_1507AF98(void);
void func_1507B040(void);
void script_commandOwnedObject(void);
void script_setNojump(void);
void script_setBodySize(void);
void script_playSound1Range1800(void);
void script_playSound1Range3000(void);
void script_playSound2Range1800(void);
void script_playSound2Range3000(void);
void script_playSound1Range4000(void);
void script_playSound2Range4000(void);
void script_playSound1Range8000(void);
void script_playSound1Range1300(void);
void script_playSound2Range1300(void);
void script_playSound1Range800(void);
void script_playSound2Range800(void);
void func_1507B578(void);
void script_jumpIfContext(void);
void func_1507B630(void);
void func_1507B6E0(void);
void script_pressButtons(void);
void func_1507B7BC(void);
void script_jumpIfTargetStunned(void);
void script_callHandler(void);
void func_1507B8F4(void);
void func_1507B958(void);
void script_jumpIfWaypointParam(void);
void script_playRandomUntrackedSound(void);
void func_1507BAD0(void);
void func_1507BAF8(void);
void script_emptyCommand108(void);
const ObjScriptCmd *script_getScript(s32 arg0, s32 scriptId);
void script_runActor(Actor *obj);
extern u8 gScriptArg0; // argument bytes of the current script command (arg0)
extern f32 gScriptSpeedups[2];

/* game/Script/CameraCmd/cameracmd.c */
extern f32 gPendingCutbackAngle;
extern s32 gPoseCommandRunning; // a command is running
void script_clearPoseCommands(void);
s32 script_isPoseCommandActive(s32 count);
s32 script_updatePoseCommand(s32 slot);
void script_updatePoseCommands(void);
s32 script_cameraCommand(s32 actorIndex, s32 command, s32 *args);
s32 script_cameraQuery(s32 actorIndex, s32 query, s32 *args);
void script_dollyPoseCamera(s32 actorIndex);

/* game/Script/ActorCmd/actorcmd.c */
s32 script_spawnActor(s32 scriptId, s32 unused);
s32 script_removeActor(s32 objId, C4DC0_arg1 *arg1);
s32 script_actorCommand(s32 objId, s32 opcode, ScriptCmd *cmd);
s32 script_actorQuery(s32 objId, s32 opcode, ScriptCmd *cmd);

/* game/Script/TaskSet/taskset.c */
typedef struct {
    u8 unk0;
    u8 unk1;
} Struct_80087410;

extern u8 D_800D2E40;
extern u8 D_800D2E41;
extern u8 D_800D2E42;
extern u8 gAutoSavePending;
extern u8 gAutoSaveDisabled;
extern u8 D_800D2E45;
extern s32 gCurrentTaskId;
extern u8 *gTaskDoneBits; // 0x1B-byte bit array of per-id flags (saved with the game)
extern s32 D_800D2E50[4];
extern u8 gSeenTextBits[9]; // bit array of seen text ids (saved with the game); [8] also holds flags
extern u8 gSeenTextChanged;
extern u8 gTaskStates[0xCC]; // one state byte per resource id
extern u8 gLevelTaskCount;
extern u16 *gLevelTaskIds;
extern IdRecordList gTaskWorkingSet;
extern s32 gLevelTasksDirty;
extern Struct_80087410 D_80087410;
void script_loadLevelTasks(s32 unused, s32 reload);
void *script_getTaskBss(s32 id);
IdRecord *script_getTask(s32 id, s32 reload);
IdRecord *script_findTask(s16 id);
void script_freeTask(IdRecord *node);
void script_insertTask(IdRecord *node);
IdRecord *script_loadTask(s16 id);
IdRecord *script_allocTaskBss(IdRecord *node);
void script_purgeTasks(s32 unloadAll);
void script_runTasks(s32 mode);
s32 script_query(s32 count, s32 cmdWord, s32 arg, ...);
s32 script_command(s32 count, s32 cmdWord, s32 arg, ...);
void script_reallocTaskDoneBits(void);
void func_1509C228(void);
s32 script_canAutoSave(void);
void script_autoSave(void);
s32 func_1509C414(s32 index);
extern s32 gScriptCmdBuffer[16];

/* game/Script/CutsceneCmd/cutscenecmd.c */
s32 script_cutsceneCommand(s32 id, s32 event, ScriptCmd *params);
s32 script_cutsceneQuery(s32 id, s32 event, s32 unused);
void script_setAnchorAtObject(s32 ref, s32 alt);

/* The resource manifest D_80087430 (game/Script/CmdTables/cmdtables.c). Its dependency lists and names
 * (.rodata 0x242F30) stay data extracted from the ROM: the names are game text, which stays out of the
 * repository. */
/* One 0x14-byte record per resource/task; D_80087430 holds RESOURCE_COUNT of them. */
typedef struct ResourceDescriptor {
    /* 0x00 */ const char *name;                 // NULL for unused slots
    /* 0x04 */ s32 packedId;                     // low 29 bits: id - 1
    /* 0x08 */ u16 levelId;
    /* 0x0A */ u16 packedSpawnPoint;             // low 12 bits: spawn point
    /* 0x0C */ const u16 *dependencies;          // resource indices, RESOURCE_LIST_END-terminated
    /* 0x10 */ const u16 *relatedDependencies;   // resource indices, RESOURCE_LIST_END-terminated
} ResourceDescriptor; // size = 0x14

#define RESOURCE_COUNT 204
#define RESOURCE_LIST_END 0xFFFF

extern ResourceDescriptor D_80087430[RESOURCE_COUNT];

/* game/Script/CmdTables/cmdtables.c: the script command handlers by class (bits 12-15 of the command word, minus one): start,
 * end, query and command. Declared without a prototype: the handlers take different parameter types. */
extern s32 (*gScriptStartHandlers[15])();
extern s32 (*gScriptEndHandlers[15])();
extern s32 (*gScriptQueryHandlers[15])();
extern s32 (*gScriptCommandHandlers[15])();

/* game/Script/EffectCmd/effectcmd.c */
const char *script_getTaskName(s32 index);
u16 script_getTaskLevel(s32 index);
s32 script_getTaskSpawnPoint(s32 index);
s32 script_getTaskPackedId(s32 index);
s32 script_findTaskIndex(s32 id);
s32 script_countTasks(void);
s32 script_areTaskDepsDone(s32 index);
void func_1509CC94(s32 index);
void script_markTaskDepsDone(s32 index);
s32 script_markRelatedTasksDone(s32 index);
void script_propagateTasksDone(s32 index);
void script_markTasksDone(const u16 *list, void (*callback)(s32));
s32 script_loadResourceTable(s32 arg0, s32 *countOut);
void script_freeResourceTable(void);
s32 script_areResourceDepsLoaded(s32 unused, s32 index, s32 *firstDepOut, s32 *unk0Out);
s32 script_startEffect(s32 cmdId, ScriptCmd *cmd);
s32 script_endEffect(s32 cmdId, ScriptCmd *cmd);
s32 script_effectCommand(s32 cmdId, s32 phase, ScriptCmd *cmd);
s32 script_effectQuery(s32 cmdId, s32 phase, s32 arg2);

/* game/Script/SwarmCmd/swarmcmd.c */
s32 script_alwaysTrue0(s32 arg0);
s32 script_alwaysTrue1(s32 arg0);
s32 script_alwaysTrue2(s32 arg0);
s32 script_alwaysTrue13(s32 arg0);
s32 script_spawnSwarm(s32 arg0, SwarmSpawnCmd *cmd);
s32 script_destroySwarm(s32 tag, ScriptCmd *cmd);
s32 script_swarmCommand(s32 tag, s32 op, SwarmTargetCmd *cmd);
s32 script_swarmQuery(s32 tag, s32 op, SwarmTargetCmd *cmd);

/* game/Script/MiscCmd/misccmd.c */
s32 script_initSpringValue(s32 arg0, ScriptCmd *record);
s32 script_emptyMiscEnd(s32 arg0, ScriptCmd *record);
s32 script_miscCommand(s32 arg0, s32 cmd, ScriptCmd *args);
s32 script_miscQuery(s32 arg0, s32 cmd, ScriptCmd *args);

/* game/Script/MusicCmd/musiccmd.c */
s32 script_streamCommand(s32 id, s32 opcode, ScriptCmd *cmd);
s32 script_streamQuery(s32 id, s32 opcode, ScriptCmd *cmd);
s32 script_musicCommand(s32 id, u32 opcode, ScriptCmd *cmd);
s32 script_musicQuery(s32 id, s32 opcode, ScriptCmd *cmd);

/* game/Script/ObjectCmd/objectcmd.c */
s32 script_worldObjectCommand(s32 id, s32 op, s32 *cmd);
s32 script_worldObjectQuery(s32 id, s32 op, s32 *cmd);
s32 script_playSound(s32 sound, u32 *cmd);
s32 script_stopSound(s32 arg0, u32 *cmd);
s32 script_actorSoundCommand(s32 arg0, s32 op, u32 *cmd);
s32 script_soundQuery(s32 arg0, s32 arg1, s32 arg2);
s32 script_playRandomActorSound(); /* (s32 first, s32 last, s32 unplayed, u32 volume, s32 path, s16 nearDistance, s32 farDistance); K&R definition, declared without a prototype */
void script_playRandomObjectSound(); /* (s32 first, s32 last, s32 volume, s32 objId, s16 nearDistance, s32 farDistance); K&R definition, declared without a prototype */
s32 script_playRandomZoneSound(s32 first, s32 last, s32 volume, s32 index, u32 nearDistance);
s32 script_playRandomSound(s32 first, s32 last, s32 volume);
void script_playActorSound(); /* (u16 sound, s32 volume, s32 path, s16 nearDistance, s32 farDistance, s32 force); K&R definition, declared without a prototype */
void script_stopActorSound(s32 path, s32 secondVoice);
s32 script_playPlainSound(s32 sound, s32 volume, s32 cents, s32 pan);
s32 script_playSoundAtActor(s32 sound, s32 path, s32 volume, s32 nearDistance, s32 farDistance);
void script_moveSoundToActor(s32 handle, s32 path, s32 cents, s32 nearDistance, s32 farDistance);
s32 script_emptyQuery12(s32 arg0, s32 arg1, s32 arg2);
s32 script_trapCommand12(s32 arg0, s32 arg1, s32 arg2);

/* game/Script/TaskCmd/taskcmd.c */
s32 script_taskCommand(s32 id, s32 cmd, CommandArgs *args);
s32 script_taskQuery(s32 id, s32 query, QueryArgs *args);

/* game/Script/TimerCmd/timercmd.c */
void script_resetTimers(void);
void script_updateTimers(void);
s32 script_startTimer(s32 arg0, Src150A0264 *arg1);
s32 script_timerCommand(s32 arg0, s32 arg1, Src150A0264 *arg2);
s32 script_timerQuery(s32 arg0, s32 arg1, s32 arg2);

/* game/Script/ZoneCmd/zonecmd.c */
s32 script_zoneCommand(s32 slot, u32 command, Msg_CD860 *msg);
s32 script_zoneQuery(s32 slot, s32 query, Msg_CD860 *msg);

/* game/Script/ScriptInterp/scriptinterp.s */
extern s32 gScriptCmdWordCount;
extern s32 gScriptSavedSp;

#endif
