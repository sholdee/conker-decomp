#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Fixed-point 16.16 to float scale. */
#define FIX16_TO_F32 (1.0f / 65536.0f)

/*
 * Event handler (address-taken, see the table at 800884D4). `event` selects the action; for
 * gScriptCmdWordCount mode 6 the params hold a position (a[0..2]) and a 16.16 angle (a[3]).
 */
s32 script_cutsceneCommand(s32 id, s32 event, ScriptCmd *params) {
    s32 value;
    s32 savedValue;
    s32 value2;
    s32 extra0;
    s32 extra1;

    switch (event) {
    case 1:
        if (gCutsceneStates[0] == 0) {
            value = level;
            if (gScriptCmdWordCount == 6) {
                func_15022190(params->a[0].w, params->a[1].w, params->a[2].w, params->a[3].w * FIX16_TO_F32);
            } else if (gScriptCmdWordCount == 3) {
                // Type 2 in bits 12-15: the low 12 bits are an object id.
                if ((params->a[0].w & 0xF000) == 0x2000) {
                    func_1509C8FC(params->a[0].w, 0);
                } else {
                    value = params->a[0].w;
                }
            }
            cutscene_start(value, id, 0, 0, 0);
            return 1;
        }
        return 0;
    case 4:
        if (gCutsceneStates[0] == 0) {
            if (gScriptCmdWordCount == 6) {
                func_15022190(params->a[0].w, params->a[1].w, params->a[2].w, params->a[3].w * FIX16_TO_F32);
            }
            savedValue = level;
            level = 0x25;
            // matching: passing 0x25 directly reschedules the switch
            cutscene_start(level, id, 0, 0, 0);
            level = savedValue;
            return 1;
        }
        return 0;
    case 3:
        if (gCutsceneStates[0] == 0) {
            extra0 = params->a[0].w;
            extra1 = params->a[1].w;
            value2 = level;
            if (gScriptCmdWordCount == 8) {
                func_15022190(params->a[2].w, params->a[3].w, params->a[4].w, params->a[5].w * FIX16_TO_F32);
            } else if (gScriptCmdWordCount == 5) {
                value2 = params->a[2].w;
            }
            cutscene_start(value2, id, 0, extra0, extra1);
            return 1;
        }
        return 0;
    case 2: {
        s32 current = level;

        D_800C35C2 = id + 1;
        D_800C35C3 = current;
        D_800C3C99 = 1;
        if (gScriptCmdWordCount == 6) {
            func_150221E8(params->a[0].w, params->a[1].w, params->a[2].w, params->a[3].w * FIX16_TO_F32);
        } else if (gScriptCmdWordCount == 3) {
            if ((params->a[0].w & 0xF000) == 0x2000) {
                func_1509C8FC(params->a[0].w, 1);
            } else {
                D_800C35C3 = params->a[0].w;
            }
        }
    } break; // matching: the block must close before the break (line steers as1)
    case 5:
        D_800C35C2 = id + 1;
        D_800C35C3 = 0x25;
        if (gScriptCmdWordCount == 6) {
            func_150221E8(params->a[0].w, params->a[1].w, params->a[2].w, params->a[3].w * FIX16_TO_F32);
        }
        break;
    case 6:
        D_800C3C9C = 1;
        break;
    }
    // Events 2, 5 and 6 (and unknown ones) fall off the end with no return value.
}


/* Condition check per event; its table entry (80088498) sits near script_cutsceneCommand's (800884D4). */
s32 script_cutsceneQuery(s32 id, s32 event, s32 unused) {
    switch (event) {
    case 1:
        if (D_800C35C4 != 0) {
            return 0;
        }
        if (gCutsceneStates[0] == 0) {
            return 1;
        }
        if (id == D_800C3C98) {
            return 0;
        }
        return 1;
    case 2:
        if ((gCutsceneStates[0] == 1) && (id == D_800C3C98)) {
            return 1;
        }
        return 0;
    case 3:
        if ((gCutsceneStates[0] == 1) && (id == D_800C35E8[0])) {
            return 1;
        }
        return 0;
    case 4:
        return gCutsceneStates[0] != 1;
    case 5:
        return D_800C3672;
    case 6:
        if ((gCutsceneStates[0] == 1) && (id == D_800C35E8[0])) {
            return 1;
        }
        return 0;
    }
}

/*
 * Looks up the object whose id is the low 12 bits of `ref` and passes its position, with the
 * player's (camera_global_Cameras) angle turned by 180 degrees, to func_150221E8 if `alt` else func_15022190.
 */
void func_1509C8FC(s32 ref, s32 alt) {
    Actor *obj;
    Camera *player;
    f32 angle;

    obj = actor_findOnPath(ref & 0xFFF);
    if (obj != NULL) {
        player = camera_global_Cameras;
        if (player->Mode & 0x100) {
            angle = player->aim.savedYaw;
        } else {
            angle = player->CameraAngle;
        }

        if (alt) {
            func_150221E8(obj->xpos, obj->ypos, obj->zpos, angle + 180.0f);
        } else {
            func_15022190(obj->xpos, obj->ypos, obj->zpos, angle + 180.0f);
        }
    }
}
