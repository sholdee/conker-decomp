#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Five spring-eased script values (math_springTo / math_springAngleTo). */
static f32 gScriptSpringVelocities[5]; // velocity
static f32 gScriptSpringValues[5]; // value
static u8 gScriptSpringInitFlags[5];  // set by script_initSpringValue

// On a type-1 record (only while gScriptCmdWordCount == 3), sets both float slots of entry `index` to the
// 16.16 fixed-point `value` and flags the entry. A start handler of gScriptStartHandlers: it has the table's s32
// type but no return statement.
s32 script_initSpringValue(s32 arg0, ScriptCmd *record) {
    if (record->arg.w == 1 && gScriptCmdWordCount == 3) {
        gScriptSpringVelocities[record->a[0].w] = record->a[1].w * (1.0f / 65536.0f);
        gScriptSpringValues[record->a[0].w] = record->a[1].w * (1.0f / 65536.0f);
        gScriptSpringInitFlags[record->a[0].w] = 1;
    }
}

// Empty end handler of gScriptEndHandlers: it has the table's s32 type but no return statement.
s32 func_1509DFB4(s32 arg0, ScriptCmd *record) {
}

// Runs command `cmd` with up to four integer params; most commands forward to a helper.
s32 script_miscCommand(s32 arg0, s32 cmd, ScriptCmd *args) {
    s32 id;
    f32 x;
    f32 y;
    f32 z;
    Actor *obj;
    f32 scale;

    switch (cmd) {
    case 0:
        screen_startOpening(0);
        return 0;
    case 1:
        text_setColor(255, 255, 255, 255);
        text_printAt(150, 86, 1, "%d", (args->a[0].w + 59) / 60);
        D_8008FDBC |= 0x80;
        break;
    case 2:
        gHudClockTime = args->a[0].w;
        break;
    case 3:
        hud_setClockEnabled(args->a[0].w);
        return 0;
    case 4:
        return mp_loadGame();
    case 5:
        return mp_updateTargetList();
    case 6:
        return mp_update();
    case 7:
        return mp_pickTeamOpponents();
    case 8:
        return func_1508C194(args->a[0].w);
    case 9:
        return mp_aimBot(args->a[0].w, args->a[1].w, args->a[2].w, args->a[3].w, 0, 0);
    case 10:
        return mp_resetSlots();
    case 11:
        return mp_updatePickups();
    case 12:
        return mp_releaseSlot(args->a[0].w, args->a[1].w);
    case 13:
        return mp_pickOpponentByPath();
    case 14:
        return mp_updateBotAttack(args->a[0].w);
    case 15:
        return mp_takeOverPlayer(args->a[0].w, args->a[1].w);
    case 16:
        id = mp_findNodeById(args->a[0].w);
        if (id != 255) {
            mp_getNodePos(id, &x, &y, &z);
            mp_addZone(x, y, z, args->a[1].w);
            return 0;
        }
        break;
    case 17:
        mp_initRacers(args->a[0].w, args->a[1].w, args->a[2].w);
        return 0;
    case 18:
        mp_updateRacer(args->a[0].w, args->a[1].w, 0.1f);
        return 0;
    case 19:
        mp_setRacerRoute(args->a[0].w, args->a[1].w);
        return 0;
    case 20:
        mp_setRacerLateral(args->a[0].w, args->a[1].w);
        return 0;
    case 21:
        mp_bindRacer(args->a[0].w, args->a[1].w);
        return 0;
    case 22:
        return mp_crossFinishLine(args->a[0].w, args->a[1].w);
    case 23:
        func_1508B1D4(args->a[0].w);
        return 0;
    case 25:
        // matching: a function-scope scale plus a nested block makes IDO hoist the constant load
        scale = 1.0f / 65536.0f;
        {
        Camera *cam = camera_global_Cameras;
        math_springTo(&gScriptSpringValues[args->a[0].w], args->a[1].w * scale, &gScriptSpringVelocities[args->a[0].w],
                      args->a[2].w * scale, args->a[3].w * scale, cam->SpringTime);
        return gScriptSpringValues[args->a[0].w] * 65536.0f;
        }
    case 26:
        // matching: a function-scope scale plus a nested block makes IDO hoist the constant load
        scale = 1.0f / 65536.0f;
        {
        Camera *cam = camera_global_Cameras;
        math_springAngleTo(&gScriptSpringValues[args->a[0].w], args->a[1].w * scale, &gScriptSpringVelocities[args->a[0].w],
                      args->a[2].w * scale, args->a[3].w * scale, cam->SpringTime);
        return gScriptSpringValues[args->a[0].w] * 65536.0f;
        }
    case 24:
        obj = collision_findObjectById(args->a[0].w);
        if (obj != NULL) {
            actor_remove(obj, 0);
        }
        break;
    }
    return 0;
}


// Answers query `cmd` with up to four integer params and returns the result as an s32.
s32 script_miscQuery(s32 arg0, s32 cmd, ScriptCmd *args) {
    Actor *obj;
    // matching: cases 1 and 18 must share these two locals for IDO's register allocation
    s32 x;
    s32 z;

    switch (cmd) {
    case 0:
        return func_150CF040(args->a[0].w, args->a[1].w);
    case 1:
        x = args->a[0].w - args->a[2].w;
        z = args->a[1].w - args->a[3].w;
        return sqrtf(x * x + z * z);
    case 2:
        return mp_nextHuman();
    case 3:
        return mp_nextBot();
    case 4:
        return mp_updateLineOfSight(args->a[0].w, args->a[1].w);
    case 5:
        return D_800E0BD0;
    case 6:
        return mp_pickOpponentInRange(args->a[0].w);
    case 7:
        return func_1508E6D0(args->a[0].w);
    case 8:
        return 0;
    case 9:
        return mp_getRacerLateral(args->a[0].w);
    case 10:
        return mp_getRacerDistance(args->a[0].w);
    case 11:
        return mp_getRacerSpeed(args->a[0].w);
    case 12:
        return mp_getRacerLap(args->a[0].w);
    case 13:
        return mp_findRacerByRank(args->a[0].w, args->a[1].w);
    case 14:
        return func_150887F8();
    case 15:
        return D_8008FDBC & 0x40;
    case 16:
        return gSettings[args->a[0].w];
    case 17:
        return func_1508B194(args->a[0].w);
    case 20:
    case 21:
        return gScriptSpringValues[args->a[0].w] * 65536.0f;
    case 18:
        obj = collision_findObjectById(args->a[0].w);
        if (obj != NULL) {
            x = obj->xpos;
            z = obj->zpos;
            return (x << 16) | (z & 0xFFFF);
        }
        return 0;
    case 19:
        obj = collision_findObjectById(args->a[0].w);
        if (obj != NULL) {
            return (s16)obj->ypos;
        }
        return 0;
    }
// matching: this label stops IDO folding the default return into the bounds-check delay slot
end:
    return 0;
}
