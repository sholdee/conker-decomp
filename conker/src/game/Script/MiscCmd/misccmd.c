#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/hud.h"
#include "conker/math.h"
#include "conker/mp.h"
#include "conker/save.h"
#include "conker/screen.h"
#include "conker/script.h"
#include "conker/text.h"

/* Five spring-eased script values (math_springTo / math_springAngleTo). */
static f32 gScriptSpringVelocities[5]; // velocity
static f32 gScriptSpringValues[5]; // value
static u8 gScriptSpringInitFlags[5];  // set by script_initSpringValue

// On a type-1 record (only while gScriptCmdWordCount == 3), sets both float slots of entry `index` to the
// 16.16 fixed-point `value` and flags the entry. A start handler of gScriptStartHandlers: it has the table's s32
// type but no return statement.
s32 script_initSpringValue(s32 cmdId, ScriptCmd *cmd) {
    if (cmd->arg.w == 1 && gScriptCmdWordCount == 3) {
        gScriptSpringVelocities[cmd->a[0].w] = cmd->a[1].w * (1.0f / 65536.0f);
        gScriptSpringValues[cmd->a[0].w] = cmd->a[1].w * (1.0f / 65536.0f);
        gScriptSpringInitFlags[cmd->a[0].w] = 1;
    }
}

// Empty end handler of gScriptEndHandlers: it has the table's s32 type but no return statement.
s32 script_emptyMiscEnd(s32 cmdId, ScriptCmd *cmd) {
}

// Runs command `opcode` with up to four integer params; most commands forward to a helper.
s32 script_miscCommand(s32 cmdId, s32 opcode, ScriptCmd *cmd) {
    s32 node;
    f32 x;
    f32 y;
    f32 z;
    Actor *obj;
    f32 scale;

    switch (opcode) {
    case 0:
        screen_startOpening(0);
        return 0;
    case 1:
        text_setColor(255, 255, 255, 255);
        text_printAt(150, 86, 1, "%d", (cmd->a[0].w + 59) / 60);
        gMatchFlags |= 0x80;
        break;
    case 2:
        gHudClockTime = cmd->a[0].w;
        break;
    case 3:
        hud_setClockEnabled(cmd->a[0].w);
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
        return func_1508C194(cmd->a[0].w);
    case 9:
        return mp_aimBot(cmd->a[0].w, cmd->a[1].w, cmd->a[2].w, cmd->a[3].w, 0, 0);
    case 10:
        return mp_resetSlots();
    case 11:
        return mp_updatePickups();
    case 12:
        return mp_releaseSlot(cmd->a[0].w, cmd->a[1].w);
    case 13:
        return mp_pickOpponentByPath();
    case 14:
        return mp_updateBotAttack(cmd->a[0].w);
    case 15:
        return mp_takeOverPlayer(cmd->a[0].w, cmd->a[1].w);
    case 16:
        node = mp_findNodeById(cmd->a[0].w);
        if (node != 255) {
            mp_getNodePos(node, &x, &y, &z);
            mp_addZone(x, y, z, cmd->a[1].w);
            return 0;
        }
        break;
    case 17:
        mp_initRacers(cmd->a[0].w, cmd->a[1].w, cmd->a[2].w);
        return 0;
    case 18:
        mp_updateRacer(cmd->a[0].w, cmd->a[1].w, 0.1f);
        return 0;
    case 19:
        mp_setRacerRoute(cmd->a[0].w, cmd->a[1].w);
        return 0;
    case 20:
        mp_setRacerLateral(cmd->a[0].w, cmd->a[1].w);
        return 0;
    case 21:
        mp_bindRacer(cmd->a[0].w, cmd->a[1].w);
        return 0;
    case 22:
        return mp_crossFinishLine(cmd->a[0].w, cmd->a[1].w);
    case 23:
        mp_clearTeamKills(cmd->a[0].w);
        return 0;
    case 25:
        // matching: a function-scope scale plus a nested block makes IDO hoist the constant load
        scale = 1.0f / 65536.0f;
        {
        Camera *cam = camera_global_Cameras;
        math_springTo(&gScriptSpringValues[cmd->a[0].w], cmd->a[1].w * scale, &gScriptSpringVelocities[cmd->a[0].w],
                      cmd->a[2].w * scale, cmd->a[3].w * scale, cam->SpringTime);
        return gScriptSpringValues[cmd->a[0].w] * 65536.0f;
        }
    case 26:
        // matching: a function-scope scale plus a nested block makes IDO hoist the constant load
        scale = 1.0f / 65536.0f;
        {
        Camera *cam = camera_global_Cameras;
        math_springAngleTo(&gScriptSpringValues[cmd->a[0].w], cmd->a[1].w * scale, &gScriptSpringVelocities[cmd->a[0].w],
                      cmd->a[2].w * scale, cmd->a[3].w * scale, cam->SpringTime);
        return gScriptSpringValues[cmd->a[0].w] * 65536.0f;
        }
    case 24:
        obj = collision_findObjectById(cmd->a[0].w);
        if (obj != NULL) {
            actor_remove(obj, 0);
        }
        break;
    }
    return 0;
}


// Answers query `opcode` with up to four integer params and returns the result as an s32.
s32 script_miscQuery(s32 cmdId, s32 opcode, ScriptCmd *cmd) {
    Actor *obj;
    // matching: cases 1 and 18 must share these two locals for IDO's register allocation
    s32 x;
    s32 z;

    switch (opcode) {
    case 0:
        return effect_getAngleAroundRotatingZonePivot(cmd->a[0].w, cmd->a[1].w);
    case 1:
        x = cmd->a[0].w - cmd->a[2].w;
        z = cmd->a[1].w - cmd->a[3].w;
        return sqrtf(x * x + z * z);
    case 2:
        return mp_nextHuman();
    case 3:
        return mp_nextBot();
    case 4:
        return mp_updateLineOfSight(cmd->a[0].w, cmd->a[1].w);
    case 5:
        return gBotDifficulty;
    case 6:
        return mp_pickOpponentInRange(cmd->a[0].w);
    case 7:
        return func_1508E6D0(cmd->a[0].w);
    case 8:
        return 0;
    case 9:
        return mp_getRacerLateral(cmd->a[0].w);
    case 10:
        return mp_getRacerDistance(cmd->a[0].w);
    case 11:
        return mp_getRacerSpeed(cmd->a[0].w);
    case 12:
        return mp_getRacerLap(cmd->a[0].w);
    case 13:
        return mp_findRacerByRank(cmd->a[0].w, cmd->a[1].w);
    case 14:
        return mp_isFlyThroughEnding();
    case 15:
        return gMatchFlags & 0x40;
    case 16:
        return gSettings[cmd->a[0].w];
    case 17:
        return mp_getTeamKills(cmd->a[0].w);
    case 20:
    case 21:
        return gScriptSpringValues[cmd->a[0].w] * 65536.0f;
    case 18:
        obj = collision_findObjectById(cmd->a[0].w);
        if (obj != NULL) {
            x = obj->xpos;
            z = obj->zpos;
            return (x << 16) | (z & 0xFFFF);
        }
        return 0;
    case 19:
        obj = collision_findObjectById(cmd->a[0].w);
        if (obj != NULL) {
            return (s16)obj->ypos;
        }
        return 0;
    }
// matching: this label stops IDO folding the default return into the bounds-check delay slot
end:
    return 0;
}
