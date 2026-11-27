#include <ultra64.h>
#include "conker/actor.h"
#include "conker/cutscene.h"
#include "conker/save.h"
#include "conker/script.h"
#include "conker/sight.h"
#include "conker/zone.h"

/* Applies command `opcode` with the parameters in `cmd` to zone `zone` (an index into zones).
 * Returns 1 for commands 0-2 and 0 otherwise. */
s32 script_zoneCommand(s32 zone, u32 opcode, ScriptCmd *cmd) {
    s32 mode;

    switch (opcode) {
    case 0:
        zone_setDisabled(zone, 0);
        return 1;
    case 1:
        zone_setDisabled(zone, 1);
        return 1;
    case 2:
        zone_setData1(zone, cmd->a[0].w, cmd->a[1].w);
        return 1;
    case 3:
        zone_setPosition(zone, cmd->a[0].w, cmd->a[1].w, cmd->a[2].w);
        break;
    case 4:
        zone_setParams(zone, cmd->a[0].w, cmd->a[1].w, cmd->a[2].w, cmd->a[3].w, cmd->a[4].w);
        break;
    case 5:
        // matching: `== 0` would compile to sltiu; the inverted test keeps the branchy form
        mode = (cmd->a[0].w == 999) ? 2 : (cmd->a[0].w != 0) ? 0 : 1;
        sight_sendBeamCommand(mode, &zones[zone]);
        break;
    }
    return 0;
}

/* Answers query `opcode` about zone `zone`; `cmd` carries the query's parameters.
 * Queries 6 and 13 take an id whose bits 12-15 select its kind: 0x2000 is an object in
 * movement_obj (looked up by actor_findOnPath), 0x9000 a zones record. */
s32 script_zoneQuery(s32 zone, s32 opcode, ScriptCmd *cmd) {
    switch (opcode) {
    case 6:
        {
            s32 pad; // matching: unused stack slot keeps `result` at the original sp offset
            s32 id;
            s32 result;
            Actor *obj;
            s32 ret = 0;

            id = cmd->a[0].w & 0xFFFF0FFF;
            switch (cmd->a[0].w & 0xF000) {
            case 0x2000:
                obj = actor_findOnPath(id);
                if (obj == NULL) {
                    return 0;
                }
                result = zone_isActorOutsideId(obj - movement_obj, zone);
            lbl1: // matching: block boundary stops IDO moving the store into the branch delay slot
                break;
            case 0x9000:
                result = zone_isCameraOutsideId(id, zone);
                break;
            }
            return (result == 0) ? 1 : ret;
        }
    case 8:
        {
            s32 subTypeData;
            s32 data1;
            s32 data2;
            s32 data3;

            zone_getParams(zone, &subTypeData, &data1, &data2, &data3);
            if (data2 == 0) {
                break;
            }
            if (gCutsceneStates[0] == 0) {
                return 1;
            }
            break;
        }
    case 9:
        {
            s32 subTypeData;
            s32 data1;
            s32 data2;
            s32 data3;

            zone_getParams(zone, &subTypeData, &data1, &data2, &data3);
            if (data2 == 0) {
                break;
            }
            if (gCutsceneStates[0] == 1) {
                return 1;
            }
            break;
        }
    case 10:
        {
            TriggerZone *rec;
            s32 x;
            s32 z;

            // matching: the no-op cast changes IDO's register numbering
            rec = (TriggerZone *)zones + zone; // matching: the (no-op) cast orders the addu
            x = rec->x;
            z = rec->z;
            return (x << 16) | (z & 0xFFFF);
        }
    case 11:
    case 12:
        {
            s32 i;

            for (i = 0; i < 25; i++) {
                if (movement_obj[i].type == cmd->a[0].w && zone_isActorOutsideId(i, zone) == 0) {
                    if (opcode == 11) {
                        return movement_obj[i].listpos | 0x2000;
                    }
                    return movement_obj[i].unique_id;
                }
            }
            return 0;
        }
    case 7:
        return zones[zone].inactive;
    case 13:
        {
            s32 pad; // matching: unused stack slot; removing it shifts the frame layout
            s32 id;
            s32 result;
            Actor *obj;

            id = cmd->a[0].w & 0xFFFF0FFF;
            switch (cmd->a[0].w & 0xF000) {
            case 0x2000:
                obj = actor_findOnPath(id);
                result = zone_isActorBehindGate(obj - movement_obj, zone);
                break;
            case 0x9000:
                result = zone_isCameraBehindGate(id, zone);
                break;
            }
            return result;
        }
    case 14:
        return zones[zone].y;
    case 15:
        if (zone_findActorInZone(zone) == -1) {
            return 0;
        }
        return 1;
    case 16:
        if (zone_findActorInZone(zone) == -1) {
            s32 i;

            for (i = 0; i < gHumanCount; i++) {
                if (zone_isCameraOutsideId(i, zone) == 0) {
                    if (gScriptCmdWordCount == 2) {
                        return 1;
                    }
                    if (cmd->a[1].w != 0) {
                        if (movement_obj[i].mv->rt_context_b != 3 && zone_isCameraOutsideId(i, cmd->a[0].w & 0xFFFF0FFF) == 0) {
                            return 1;
                        }
                    } else {
                        return 1;
                    }
                }
            }
            return 0;
        }
        return 1;
    case 17:
        {
            s32 which;

            which = cmd->a[0].w;
            if (which == 0) {
                return zones[zone].subTypeData;
            } else if (which == 1) {
                return zones[zone].data1;
            } else if (which == 2) {
                return zones[zone].data2;
            } else if (which == 3) {
                return zones[zone].data3;
            }
            break;
        }
    case 18:
        return zones[zone].yaw;
    }
    return 0;
}
