#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/aim.h"
#include "conker/anim.h"
#include "conker/attach.h"
#include "conker/boot.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/effect.h"
#include "conker/event.h"
#include "conker/face.h"
#include "conker/game.h"
#include "conker/hud.h"
#include "conker/math.h"
#include "conker/model.h"
#include "conker/mp.h"
#include "conker/particle.h"
#include "conker/path.h"
#include "conker/player.h"
#include "conker/save.h"
#include "conker/screen.h"
#include "conker/script.h"
#include "conker/text.h"
#include "conker/trail.h"
#include "conker/view.h"
#include "conker/zone.h"

/* matching: no header declares actor_seatRider (movement.c): tintedobject.c was compiled with an s32 return */
void actor_seatRider(Actor *mount, f32 height, s32 baseMtx, s32 attachFlags, s32 hideRider, s32 mountWalk, s32 immune,
                   s32 hitRoute, s32 force, s32 soundCmd);

struct C4DC0_arg1 {
    u8 pad0[4];
    s32 unk4;
};


/* Returns the object id for script id `scriptId` (| 0x2000 when nonzero), spawning it from its slot if needed. */
s32 script_spawnActor(s32 scriptId, s32 cmd) {
    s32 ret;
    Actor *obj;

    ret = collision_findSpawnById(scriptId);
    if (ret != -1) {
        obj = collision_findObjectById(scriptId);
        if (obj == NULL) {
            gSpawnRecords[ret].unk2 = 0;
            if (collision_spawnObject(ret + gSpawnRecords, ret, 0, 0, 0) == 0) { // matching: index + base operand order
                return -1;
            }
            ret = collision_findSpawnById(scriptId);
        } else {
            ret = obj->listpos;
        }
    }
    if (ret != 0) {
        ret |= 0x2000;
    }
    return ret;
}

/* Removes object `objId` (flagging its spawn slot); returns 999999 on success, 0 if there is no such object. */
s32 script_removeActor(s32 objId, C4DC0_arg1 *cmd) {
    Actor *obj;

    obj = actor_findOnPath(objId);
    if (obj == NULL) {
        return 0;
    }
    if (objId < gSpawnRecordCount) {
        gSpawnRecords[objId].unk2 = 1;
    }
    if (gScriptCmdWordCount == 2) {
        if (cmd->unk4 == 0) {
            actor_remove(obj, 0);
        } else {
            actor_handleDeath(obj);
        }
    } else {
        actor_remove(obj, 0);
    }
    return 999999;
}


/* Raw field access for bytes of Actor that structs.h declares only as padding or with
 * another width (the shared headers are not edited in this pass). */
/* Index of an object in the movement_obj object table. */
#define OBJ_INDEX(o) ((o) - movement_obj)


/* Script "set" commands: applies command `opcode` with arguments `cmd->a` to object `objId`.
 * Returns 0 for an unknown command or missing object, 1 otherwise.
 * matching: the unused `pad` locals reserve stack slots of the original's block locals; each one is needed. */
s32 script_actorCommand(s32 objId, s32 opcode, ScriptCmd *cmd) {
    Actor *obj;
    s32 i;
    s32 node;
    s32 pad;

    obj = actor_findOnPath(objId);
    if (obj == NULL) {
        return 0;
    }
    switch (opcode) {
    case 1:
        if (cmd->a[0].w != 0) {
            obj->task_attrib |= cmd->a[1].w;
        } else {
            obj->task_attrib &= ~cmd->a[1].w;
        }
        return 1;
    case 0x52:
    case 0x5E:
        break;
    case 7:
        gCurrentActor = obj;
        actor_dropLiftedObject(gCurrentActor);
        break;
    case 2:
        obj->task_attrib |= 8;
        return 1;
    case 3:
        obj->task_attrib &= ~8;
        return 1;
    case 4:
        obj->task_attrib |= 0x10;
        return 1;
    case 0xA:
        obj->task_attrib &= ~4;
        break;
    case 6:
        obj->task_attrib |= 4;
        obj->task_attrib &= ~1;
        return 1;
    case 5:
        obj->task_attrib |= 1;
        return 1;
    case 8:
        if (cmd->a[0].w != 0x7FFFFFFF) {
            obj->xpos = cmd->a[0].w;
        }
        if (cmd->a[1].w != 0x7FFFFFFF) {
            obj->ypos = cmd->a[1].w;
        }
        if (cmd->a[2].w != 0x7FFFFFFF) {
            obj->zpos = cmd->a[2].w;
        }
        break;
    case 9: {
        s32 node;

        node = mp_findNodeById(cmd->a[0].w);
        if (node != -1) {
            mp_getNodePos(node, &obj->xpos, &obj->ypos, &obj->zpos);
        }
        return 1;
    }
    case 0xC:
    case 0xD:
        obj->speed = cmd->a[0].w * 1.52587890625e-05f;
        break;
    case 0xE:
        obj->yspd = cmd->a[0].w * 1.52587890625e-05f;
        break;
    case 0xF:
        obj->ygrav = cmd->a[0].w * 1.52587890625e-05f;
        break;
    case 0x10:
        obj->angle = obj->langle = (u16)(cmd->a[0].w * 1.52587890625e-05f * 182.04444885f) - 0x4000;
        break;
    case 0x11:
        obj->angle = obj->langle = cmd->a[0].w * 0.00390625f;
        break;
    case 0x12:
        obj->fangle = cmd->a[0].w * 0.00390625f;
        break;
    case 0x13:
        obj->imune = cmd->a[0].w;
        break;
    case 0x14:
        obj->intelwalk = cmd->a[0].w;
        gCurrentActor = obj;
        dc_glbl_CharNum = OBJ_INDEX(gCurrentActor);
        obj->scriptCmd = script_getScript(0, obj->intelwalk);
        obj->scriptTimer = 0;
        break;
    case 0x61: {
        s32 buttons;

        buttons = cmd->a[0].w;
        if (obj->mv != NULL) {
            if (buttons == -1) {
                buttons = 0x8000;
            }
            obj->mv->pressedButtons = buttons;
            obj->mv->pressedButtonsTimer = 0;
            switch (cmd->a[0].w) {
            case -1:
            case 4:
            case 0x2000:
                obj->mv->buttons = buttons;
                obj->mv->buttonsTimer = 2;
                break;
            }
        }
    }
        break;
    case 0x66:
        text_setAnchor(obj, cmd->a[0].w);
        break;
    case 0x67: {
        s32 quadrant;

        quadrant = cmd->a[0].w;
        text_setFlags(0);
        switch (gScreenLayout) {
        case 2:
        case 3:
            text_setPosition((quadrant & 1) * 150 + 10, (quadrant >> 1) * 115);
            break;
        }
        break;
    }
    case 0x62: {
        s32 pad;

        if (OBJ_INDEX(obj) >= 0 && OBJ_INDEX(obj) < gPlayerCount) {
            player_setStat(OBJ_INDEX(obj), 10, cmd->a[0].w);
        }
        D_800BE3E4 = player_getStat(OBJ_INDEX(obj), 6);
    }
        break;
    case 0x63: {
        s32 pad;

        if (OBJ_INDEX(obj) >= 0 && OBJ_INDEX(obj) < gPlayerCount) {
            player_setStat(OBJ_INDEX(obj), 9, cmd->a[0].w);
        }
        D_800BE3E4 = player_getStat(OBJ_INDEX(obj), 6);
    }
        break;
    case 0x64: {
        s32 value;

        value = cmd->a[0].w;
        if (gIsMultiplayer == 0 && (D_800E9D00 & 0x200)) {
            value = 0x33;
        }
        if (OBJ_INDEX(obj) >= 0 && OBJ_INDEX(obj) < gPlayerCount) {
            player_setStat(OBJ_INDEX(obj), 2, value);
        }
        break;
    }
    case 0x65:
        if (OBJ_INDEX(obj) >= 0 && OBJ_INDEX(obj) < gPlayerCount) {
            player_setStat(OBJ_INDEX(obj), 3, cmd->a[0].w);
        }
        break;
    case 0x5F:
        if (cmd->a[0].w < obj->stickspeed) {
            obj->stickspeed = cmd->a[0].w;
        }
        break;
    case 0x60:
        if (cmd->a[0].w < 0) {
            obj->pathGoal = 0xFF;
        } else {
            obj->pathGoal = mp_findNodeById(cmd->a[0].w);
        }
        obj->pathNode = 0xFF;
        obj->task_attrib &= ~0x800;
        obj->intelwalk = cmd->a[1].w;
        gCurrentActor = obj;
        dc_glbl_CharNum = OBJ_INDEX(gCurrentActor);
        obj->scriptCmd = script_getScript(0, obj->intelwalk);
        obj->scriptTimer = 0;
        break;
    case 0x5B:
        if (cmd->a[0].w < 0) {
            obj->pathGoal = 0xFF;
        } else {
            obj->pathGoal = mp_findNodeById(cmd->a[0].w);
        }
        break;
    case 0x5C:
        if (cmd->a[0].w < 0) {
            obj->pathNode = 0xFF;
        } else {
            obj->pathNode = mp_findNodeById(cmd->a[0].w);
        }
        break;
    case 0x5D:
        if (cmd->a[0].w < 0) {
            obj->pathAvoid = 0xFF;
        } else {
            obj->pathAvoid = mp_findNodeById(cmd->a[0].w);
        }
        break;
    case 0xB:
        obj->carried = cmd->a[0].w;
        break;
    case 0x15:
        obj->vtilt = cmd->a[0].w * 1.52587890625e-05f;
        break;
    case 0x16:
        obj->speedup = cmd->a[0].w * 1.52587890625e-05f;
        break;
    case 0x17:
        if (cmd->a[0].w != 0) {
            trail_getActorEmitter(obj, 0xFF, 0);
        } else {
            trail_destroyAllEmitters(obj);
        }
        break;
    case 0x72:
        if (obj->mv != NULL) {
            obj->mv->unk129 = cmd->a[0].w;
        } else {
            obj->targetIndex = cmd->a[0].w;
        }
        break;
    case 0x18:
        obj->intel_accel = cmd->a[0].w;
        break;
    case 0x19:
        if (cmd->a[0].w < 0) {
            obj->targetIndex = cmd->a[0].w;
        } else if (collision_findSlotBySpawnIndex(cmd->a[0].w & 0xFFFF0FFF) != -1) {
            obj->targetIndex = collision_findSlotBySpawnIndex(cmd->a[0].w & 0xFFFF0FFF);
        }
        break;
    case 0x1A:
        obj->unk20A = cmd->a[0].w;
        break;
    case 0x1B:
        if (cmd->a[0].w == 1) {
            obj->hiddenBones |= cmd->a[1].w;
        } else {
            obj->hiddenBones &= ~cmd->a[1].w;
        }
        break;
    case 0x1C:
        obj->targetScaleXz = cmd->a[0].w * 1.52587890625e-05f;
        obj->targetScaleY = cmd->a[1].w * 1.52587890625e-05f;
        obj->scaleStep = cmd->a[2].w * 1.52587890625e-05f;
        break;
    case 0x1E:
        if (cmd->a[0].w != 0) {
            obj->activeFlags |= 0x80;
        } else {
            obj->activeFlags &= ~0x80;
        }
        break;
    case 0x1F:
        player_setSpawnIndex(obj, cmd->a[0].w & 0xFFF);
        break;
    case 0x5A:
        player_respawnAt(obj, -1);
        break;
    case 0x20:
        obj->energy = cmd->a[0].w;
        if (gScriptCmdWordCount == 4) {
            gHudBlockCount = obj->energy;
            gHudBlockTarget = gHudBlockCount;
        }
        break;
    case 0x1D:
        if (cmd->a[0].w != 0) {
            obj->norun = 0xFF;
            obj->nojump = 0xFF;
            obj->count5 = 0xFF;
        } else {
            obj->norun = 0;
            obj->nojump = 0;
            obj->count5 = 0;
        }
        break;
    case 0x53:
        obj->norun = cmd->a[0].w;
        break;
    case 0x21:
        player_setStat(objId, cmd->a[0].w, cmd->a[1].w);
        break;
    case 0x22: {
        s32 j;

        for (j = 0; j < gScriptCmdWordCount - 2; j++) {
            if (cmd->a[j].w != 0x78000000) {
                obj->unk2D8[j] = cmd->a[j].w * 1.52587890625e-05f;
            }
        }
        break;
    }
    case 0x23: {
        s32 j;

        for (j = 0; j < gScriptCmdWordCount - 2; j++) {
            if (cmd->a[j].w != 0x7FFFFFFF) {
                (&obj->taskint[0])[j] = cmd->a[j].w;
            }
        }
        break;
    }
    case 0x25:
        obj->mv->pissed_up = cmd->a[0].w;
        obj->mv->pissed_walk = 0xA6;
        break;
    case 0x24:
        if (cmd->a[0].w == 0) {
            face_resetDefaultExpression(obj);
            obj->mv->pissed_up = 0;
            obj->mv->pissed_hangover = 0;
        } else {
            obj->mv->pissed_up = 1;
            if (cmd->a[0].w == 1) {
                face_setDefaultExpression(obj, 0x15);
            } else if (cmd->a[0].w == 2) {
                face_setDefaultExpression(obj, 0x2A);
                obj->mv->pissed_hangover = 0xFF;
            }
        }
        break;
    case 0x26:
        text_lockObjectText(objId, 0, cmd->a[0].w, 0);
        break;
    case 0x27:
        text_lockObjectText(objId, 1, cmd->a[0].w, 0);
        if (gScriptCmdWordCount == 3 && cmd->a[0].w == 0) {
            obj->mv->rt_context_b = 0;
        }
        break;
    case 0x70:
        if (cmd->a[1].w == 0) {
            obj->mv->unk128 &= ~cmd->a[0].w;
        } else {
            obj->mv->unk128 |= cmd->a[0].w;
        }
        break;
    case 0x28:
        obj->mv->context_z = cmd->a[0].w;
        break;
    case 0x29:
        obj->unk251 = cmd->a[0].w;
        break;
    case 0x2A:
        actor_hitScripted(obj, cmd->a[0].w, cmd->a[1].w, -1);
        break;
    case 0x2B:
        actor_hitScripted(obj, cmd->a[0].w, cmd->a[1].w, cmd->a[2].w);
        break;
    case 0x2C:
        actor_runAttackAt(cmd->a[1].w, cmd->a[2].w, cmd->a[3].w, cmd->a[0].w, cmd->a[4].w, cmd->a[5].w, 0, 0);
        break;
    case 0x2D:
        obj->waypoint = cmd->a[0].w;
        break;
    case 0x2E:
        gButtonsPressed[cmd->a[0].w] = cmd->a[1].w;
        gPadTable[cmd->a[0].w]->button = cmd->a[1].w;
        break;
    case 0x56:
        obj->attach_char = cmd->a[0].w;
        break;
    case 0x2F: {
        Actor *other;

        other = event_getOwnedObject(obj, cmd->a[0].w);
        if (other != NULL) {
            switch (cmd->a[1].w) {
            case 0:
                actor_setScale(obj, cmd->a[2].w * 1.52587890625e-05f, cmd->a[3].w * 1.52587890625e-05f);
                break;
            case 1:
                actor_playAnim(other, cmd->a[2].w, cmd->a[3].w * 1.52587890625e-05f, cmd->a[4].w * 1.52587890625e-05f, 0.0f, 0.0f, 0);
                break;
            case 2:
                other->scriptCmd = NULL;
                other->intelwalk = cmd->a[2].w;
                break;
            }
        }
    }
        break;
    case 0x30:
        obj->stunned = cmd->a[0].w;
        break;
    case 0x31:
        obj->intelhitroute = cmd->a[0].w;
        break;
    case 0x32:
        obj->lastAttacker = cmd->a[0].w;
        break;
    case 0x33:
        gScriptSpeedups[cmd->a[0].w] = cmd->a[1].w * 1.52587890625e-05f;
        break;
    case 0x34: {
        s32 x;
        s32 z;
        f32 y;

        x = camera_global_Cameras->eye.x;
        z = camera_global_Cameras->eye.z;
        x = x - sinf(camera_global_Cameras->TrueCameraAngleRad) * 1800.0f;
        z = z - cosf(camera_global_Cameras->TrueCameraAngleRad) * 1800.0f;
        x += math_random() % 800U - 400;
        z += math_random() % 800U - 400;
        if (x < cmd->a[0].w) {
            x = cmd->a[0].w + (cmd->a[0].w - x);
        } else if (x > cmd->a[1].w) {
            x = cmd->a[1].w + (cmd->a[1].w - x);
        }
        if (z < cmd->a[2].w) {
            z = cmd->a[2].w + (cmd->a[2].w - z);
        } else if (z > cmd->a[3].w) {
            z = cmd->a[3].w + (cmd->a[3].w - z);
        }
        y = camera_global_Cameras->eye.y;
        collision_findFloorInRange(0, 0, &y, 0, 0, 0, x, y, z, y, 0, 0, -10000.0f, 20000.0f);
        obj->xpos = x;
        obj->ypos = cmd->a[4].w + y;
        obj->zpos = z;
    }
        break;
    case 0x35:
        gCurrentActor = obj;
        dc_glbl_CharNum = OBJ_INDEX(gCurrentActor);
        obj->scriptCmd = script_getScript(0, cmd->a[0].w);
        obj->scriptTimer = 0;
        break;
    case 0x36:
        if (cmd->a[0].w != 0) {
            obj->mv->water_attribs |= cmd->a[1].w;
        } else {
            obj->mv->water_attribs &= ~cmd->a[1].w;
        }
        break;
    case 0x37:
        obj->family = cmd->a[0].w;
        break;
    case 0x38:
        obj->intelwalk = cmd->a[0].w;
        break;
    case 0x39:
        obj->waypointStep = cmd->a[0].w;
        break;
    case 0x3A: {
        f32 scale;

        scale = cmd->a[0].w * 1.52587890625e-05f;
        actor_setScale(obj, scale, scale);
        break;
    }
    case 0x3B:
        obj->mv->walk_attrib |= cmd->a[0].w;
        if (gScriptCmdWordCount == 4) {
            obj->mv->unk34 = cmd->a[1].w;
        }
        break;
    case 0x3C:
        if (cmd->a[0].w != 0) {
            obj->mv->injured = 1;
        } else {
            obj->mv->injured = 0;
        }
        break;
    case 0x3D:
        if (cmd->a[0].w == 0) {
            obj->hiddenParts = ~(cmd->a[1].w | 1);
        } else if (cmd->a[0].w == 1) {
            obj->hiddenParts &= ~cmd->a[1].w;
        } else {
            obj->hiddenParts |= cmd->a[1].w;
        }
        break;
    case 0x3E:
        model_breakIntoParts(obj, cmd->a[0].w, cmd->a[1].w);
        break;
    case 0x3F:
        obj->nojump = cmd->a[0].w;
        break;
    case 0x40:
        if (cmd->a[0].w != 0) {
            obj->noshove |= cmd->a[1].w;
        } else {
            obj->noshove &= ~cmd->a[1].w;
        }
        break;
    case 0x41:
        obj->fallStartY = cmd->a[0].w;
        break;
    case 0x42:
        obj->intel_animspd = cmd->a[0].w >> 8;
        obj->animSpeedFrac = cmd->a[0].w & 0xFF;
        break;
    case 0x43:
        gSpawnPaths[obj->listpos][cmd->a[0].w + 1].x = cmd->a[1].w;
        gSpawnPaths[obj->listpos][cmd->a[0].w + 1].y = cmd->a[2].w;
        gSpawnPaths[obj->listpos][cmd->a[0].w + 1].z = cmd->a[3].w;
        break;
    case 0x44: {
        f32 px;
        f32 py;
        f32 pz;

        node = mp_findNodeById(cmd->a[1].w);
        if (node != -1) {
            mp_getNodePos(node, &px, &py, &pz);
            gSpawnPaths[obj->listpos][cmd->a[0].w + 1].x = px;
            gSpawnPaths[obj->listpos][cmd->a[0].w + 1].y = py;
            gSpawnPaths[obj->listpos][cmd->a[0].w + 1].z = pz;
        }
        break;
    }
    case 0x45:
        obj->mv->inst_context = cmd->a[0].w;
        break;
    case 0x46:
        obj->nearScript = cmd->a[0].w;
        obj->nearRange = cmd->a[1].w;
        obj->nearCone = cmd->a[2].w;
        break;
    case 0x47:
        obj->mv->ind_cnt = cmd->a[0].w;
        break;
    case 0x48:
        obj->doing_hit = cmd->a[0].w;
        obj->doingHitTimer = cmd->a[1].w;
        break;
    case 0x49:
        obj->forced_lod = cmd->a[0].w;
        break;
    case 0x4A:
        face_setDefaultExpression(obj, cmd->a[0].w);
        break;
    case 0x4B: {
        s32 setId;

        setId = cmd->a[0].w;
        collision_createPartSet(obj, setId, 0.0f, 0);
        break;
    }
    case 0x4C:
    {
        s32 setId;

        setId = cmd->a[0].w;
        collision_removePartSet(obj, setId);
        break;
    }
    case 0x4D: {
        ModelAttachment *attachment;
        s32 setId;
        s32 index;

        setId = cmd->a[0].w;
        index = cmd->a[1].w;
        attachment = attach_find(obj, setId, index);
        if (attachment != NULL) {
            attachment->hookState = cmd->a[2].w;
        }
    }
        break;
    case 0x4E: {
        Actor *other;
        s32 pad;

        other = collision_findObjectById(cmd->a[0].w & 0xFF);
        if (other != NULL) {
            if (cmd->a[2].w == 0) {
                obj->unk101 |= 4;
            }
            obj->attach_char = OBJ_INDEX(other) + 1;
            obj->basemtx = cmd->a[1].w;
        }
        break;
    }
    case 0x4F:
        obj->unique_id = cmd->a[0].w;
        break;
    case 0x50:
        obj->cullMask = (obj->cullMask & ~cmd->a[1].w) | cmd->a[0].w;
        break;
    case 0x51:
        obj->mv->chased = cmd->a[0].w;
        break;
    case 0x54: {
        s32 fadeCmd;
        s32 step;
        s32 value;

        value = cmd->a[0].w;
        if (gScriptCmdWordCount == 3) {
            fadeCmd = 1;
            step = 0;
        } else {
            fadeCmd = 2;
            step = cmd->a[1].w;
        }
        actor_fadeCommand(obj, fadeCmd, value, step, 0);
        break;
    }
    case 0x57:
        obj->scriptTimer = cmd->a[0].w;
        break;
    case 0x58:
        obj->unk140 = cmd->a[0].w;
        break;
    case 0x55: {
        s32 pad;
        s32 idx;

        idx = OBJ_INDEX(obj);
        if (idx < 4) {
            gPadTable[idx]->button |= B_BUTTON;
            gButtonsPressed[idx] |= 0x4000;
        }
    }
        break;
    case 0x59:
        obj->visibleWith[cmd->a[1].w] = cmd->a[0].w;
        break;
    case 0x68:
        obj->cullDistance = cmd->a[0].w;
        break;
    case 0x69:
        gIdleRequestFlags |= cmd->a[0].w;
        break;
    case 0x6C:
        if (cmd->a[0].w == 9) {
            func_15063404(obj);
        } else if (cmd->a[0].w == 0x3B) {
            func_15063570(obj);
        }
        break;
    case 0x6D:
        if (cmd->a[0].w == 9) {
            func_150634E4(obj);
        }
        break;
    case 0x6A:
        if (cmd->a[0].w != 0) {
            obj->unk101 |= 1;
        } else {
            obj->unk101 &= ~1;
        }
        break;
    case 0x6B:
        if (cmd->a[0].w == 1) {
            obj->unk101 |= cmd->a[1].w;
        } else {
            obj->unk101 &= ~cmd->a[1].w;
        }
        break;
    case 0x6E: {
        s32 idx;
        s32 val;

        idx = cmd->a[0].w;
        val = cmd->a[1].w;
        obj->eyeCode[idx] = val + 10;
    }
        break;
    case 0x6F: {
        s32 idx;
        s32 val;

        idx = cmd->a[0].w;
        val = cmd->a[1].w;
        obj->swapTexture[idx] = val;
    }
        break;
    case 0x71: {
        s32 angle;
        s32 dist;
        s32 lo;
        s32 hi;
        s32 pad1;
        s32 pad2;
        s32 pad3;
        s32 pad4;
        s32 pad5;

        /* Place the object at a random distance in [a[2], a[3]) from (a[0], a[1]), at a random angle (degrees)
         * between the bounds a[4] / a[5], which are XOR-swapped into order first. */
        if (cmd->a[4].w >= cmd->a[5].w) {
            lo = cmd->a[5].w ^ cmd->a[4].w;
            hi = cmd->a[5].w ^ lo;
            cmd->a[4].w = lo ^ hi;
            cmd->a[5].w = hi;
        }
        angle = math_angleDiffInt((f32)cmd->a[4].w, (f32)cmd->a[5].w);
        angle = cmd->a[4].w + math_random() % angle;
        dist = cmd->a[2].w + math_random() % (cmd->a[3].w - cmd->a[2].w);
        obj->xpos = cmd->a[0].w + sinf(angle * 0.01745329238f) * -(f32)dist;
        obj->zpos = cmd->a[1].w + cosf(angle * 0.01745329238f) * -(f32)dist;
    }
        break;
    case 0x73:
        obj->type = cmd->a[0].w;
        break;
    case 0x74:
        obj->mv->prop = cmd->a[0].w;
        break;
    case 0x75:
        if (cmd->a[0].w != 0) {
            obj->unk2F8 |= cmd->a[1].w;
        } else {
            obj->unk2F8 &= ~cmd->a[1].w;
        }
        break;
    case 0x76:
        if (obj->mv != NULL) {
            obj->mv->ammo_count = cmd->a[0].w;
        }
        break;
    case 0x77:
        gAimFixedPoint[0] = cmd->a[0].w;
        gAimFixedPoint[1] = cmd->a[1].w;
        gAimFixedPoint[2] = cmd->a[2].w;
        break;
    case 0x78:
        switch (cmd->a[0].w) {
        case 0: {
            s32 setId;

            setId = 0x79;
            collision_createPartSet(obj, setId, 0.0f, 0);
            if (OBJ_INDEX(obj) < gHumanCount) {
                obj->mv->transport = 2;
                actor_playAnim(obj, 5, 1.0f, 0.0f, 0.0f, 0.0f, 0);
            }
        }
            break;
        case 1:
            func_1514DE94(obj);
            break;
        case 2:
            obj->mv->env_suit = 1;
            obj->forced_lod = 6;
            break;
        case 3: {
            Actor *other;

            other = actor_findOnPath(cmd->a[1].w & 0xFFFF0FFF);
            if (other != NULL) {
                obj->mv->transport = 1;
                actor_seatRider(other, 50.0f, 0x17, 4, 0, 1, 0, 0, 1, 0);
            }
            break;
        }
        case 4:
            particle_spawnBoneModelBurst(obj, 0xFF, 1);
            particle_attachBloodStrip(obj, 0xFF, 1);
            text_spawnAmount(obj, 0xFF, 1);
            break;
        case 5:
            actor_playAnim(obj, 0x16, 1.0f, 0.0f, 0.0f, 0.0f, 0);
            break;
        }
        break;
    case 0x79:
        if (cmd->a[0].w != 0) {
            obj->mv->matrix_physics = 1;
        } else {
            obj->mv->matrix_physics = 0;
        }
        break;
    case 0x7A: {
        Actor *other;
        s32 pad;

        other = collision_findObjectById(cmd->a[0].w & 0xFF);
        if (other != NULL) {
            obj->lifting = OBJ_INDEX(other) + 100;
            obj->speed = 0.0f;
            actor_attachToHost(other, OBJ_INDEX(obj));
        }
        break;
    }
    case 0x7B:
        obj->mv->unk1AF = cmd->a[0].w;
        break;
    case 0x7C:
        obj->arriveRange = cmd->a[0].w;
        break;
    case 0x7D:
        obj->radarColor = cmd->a[0].w + 1;
        break;
    case 0x7E:
        obj->mv->unk7C = cmd->a[0].w;
        break;
    case 0x7F:
        obj->listpos_ptr->flags = cmd->a[0].w;
        break;
    case 0x80:
        {
            f32 scale;

            scale = cmd->a[2].w * 0.01f;
            actor_respawnInPlace(obj, cmd->a[0].w, cmd->a[1].w, scale);
        }
        break;
    case 0x81:
        obj->touching_char = cmd->a[0].w;
        break;
    case 0x82: {
        u16 saved;

        saved = gMatchFlags;
        gMatchFlags = 0;
        actor_respawn(obj);
        gMatchFlags = saved;
    }
        break;
    case 0x83:
        obj->stunEndScript = cmd->a[0].w;
        break;
    case 0x84:
        obj->mv->transport = cmd->a[0].w;
        break;
    case 0x85:
        actor_finishDeath(OBJ_INDEX(obj));
        break;
    case 0x86:
        obj->sightRange = cmd->a[0].w;
        break;
    case 0x87:
        actor_kill(obj, 0xF);
        break;
    case 0x88:
        actor_kill(obj, 9);
        break;
    case 0x89:
        obj->alertCooldown = cmd->a[0].w;
        break;
    case 0x8A:
        obj->fogMode = cmd->a[0].w;
        obj->fogLevel = cmd->a[1].w;
        obj->fogRate = cmd->a[2].w;
        obj->fogColor = cmd->a[3].w;
        obj->fogFade = cmd->a[4].w;
        break;
    case 0x8C:
        obj->mv->weapon_status = cmd->a[0].w;
        break;
    case 0x8B:
        if (OBJ_INDEX(obj) <= gScreenLayout) {
            screen_startViewFade(cmd->a[0].w, cmd->a[1].w, cmd->a[2].w, cmd->a[3].w, cmd->a[4].w, OBJ_INDEX(obj));
        }
        break;
    case 0x8D:
        for (i = 0; i < 25; i++) {
            if (movement_obj[i].type != 0 && OBJ_INDEX(obj) + 1 == movement_obj[i].owner &&
                movement_obj[i].drawtype == 5) {
                actor_remove(&movement_obj[i], 0);
            }
        }
        break;
    case 0x8E:
        obj->floorY = cmd->a[0].w;
        break;
    default:
        return 0;
    }
    return 1;
}



/* Script "query" commands: returns the value of query `opcode` (arguments `cmd->a`) for object `objId`;
 * 0 for an unknown query or a missing object (except the existence queries 0xB0 / 0xB1 / 0xB7).
 * matching: the unused `pad` locals reserve stack slots of the original's block locals; each one is needed. */
s32 script_actorQuery(s32 objId, s32 opcode, ScriptCmd *cmd) {
    Actor *obj;
    Actor *other;
    s32 ret;
    s32 ix;
    s32 iz;
    f32 dx;
    f32 dz;

    if (opcode == 0xB7) {
        return collision_findSpawnById(objId);
    }
    obj = actor_findOnPath(objId);
    if (opcode == 0xB0) {
        if (obj == NULL) {
            return 1;
        }
        return 0;
    }
    if (opcode == 0xB1) {
        if (obj == NULL) {
            return 1;
        }
        if (obj->type == 8 || obj->type == 0x27) {
            return 1;
        }
        return 0;
    }
    if (obj == NULL) {
        return 0;
    }
    switch (opcode) {
    case 0xA5:
        return player_getStat(objId, cmd->a[0].h.lo);
    case 0x91:
        return obj->intelwalk;
    case 0x92:
        return obj->scriptTimer;
    case 0x93:
        return obj->energy;
    case 0xA8:
        if (obj->stunned != 0) {
            return 1;
        }
        break;
    case 0x94:
        if (obj->carried != 0) {
            return 1;
        }
        break;
    case 0x95:
        if (obj->lifting != 0 && collision_findSlotBySpawnIndex(cmd->a[0].w & 0xFFFF0FFF) + 100 == obj->lifting) {
            return 1;
        }
        break;
    case 0x96:
        if (obj->inwater == 1) {
            return 1;
        }
        break;
    case 0x97:
        ix = obj->xpos;
        iz = obj->zpos;
        return (ix << 16) | (iz & 0xFFFF);
    case 0x98:
        return (s16)obj->ypos;
    case 0x99:
        other = actor_findOnPath(cmd->a[0].w & 0xFFFF0FFF);
        dx = obj->xpos - other->xpos;
        dz = obj->zpos - other->zpos;
        if (dx * dx + dz * dz < cmd->a[1].w * cmd->a[1].w) {
            return 1;
        }
        return 0;
    case 0x9A: {
        s32 pad1;
        s32 pad2;
        other = actor_findOnPath(cmd->a[0].w & 0xFFFF0FFF);
        if (other == NULL) {
            return 32000;
        }
        dx = obj->xpos - other->xpos;
        dz = obj->zpos - other->zpos;
        return sqrtf(dx * dx + dz * dz);
    }
    case 0x9B: {
        f32 dx;
        f32 dz;
        dz = obj->zpos - cmd->a[1].w;
        dx = obj->xpos - cmd->a[0].w;
        return sqrtf(dz * dz + dx * dx);
    }
    case 0x9C:
        other = actor_findOnPath(cmd->a[0].w & 0xFFFF0FFF);
        return (actor_getDirectionAngle(other->xpos - obj->xpos, obj->zpos - other->zpos, 0) + 0x4000) * 0.0054931640625f * 65536.0f;
    case 0x9D:
        return (u32)(u16)(actor_getDirectionAngle(cmd->a[0].w - obj->xpos, obj->zpos - cmd->a[1].w, 0) - 0x4000) * 0.0054931640625f * 65536.0f;
    case 0x9E: {
        s32 radius;
        s32 dx;
        s32 dz;
        radius = cmd->a[2].w;
        dx = (s32)obj->xpos - cmd->a[0].w;
        dz = (s32)obj->zpos - cmd->a[1].w;
        if (dx * dx + dz * dz < radius * radius) {
            return 1;
        }
        return 0;
    }
    case 0x9F:
        return obj->speedup;
    case 0xA0:
        return obj->moveMode;
    case 0xA1:
        return obj->nearScript;
    case 0xB3:
        return obj->theta * 65536.0f;
    case 0xA4:
        return obj->langle;
    case 0xA2:
        ret = 0;
        if (obj->mv->unk57 != 0) {
            ret = 1;
        }
        return ret;
    case 0xA3:
        return obj->task_attrib & 0x40;
    case 0xA6:
        return (obj->fallStartY - obj->ypos) > 150.0f;
    case 0xA7:
        return player_getSpawnIndex(obj) | 0xD000;
    case 0xA9:
        return obj->mv->rt_context_a;
    case 0xAA:
        return obj->mv->rt_context_b;
    case 0xAB:
        return obj->mv->rt_context_z;
    case 0xAC:
        ret = zone_findActorZone(OBJ_INDEX(obj), gScriptCmdWordCount - 2, &cmd->a[0].w);
        if (ret != -1) {
            return ret | 0x4000;
        }
        return 0;
    case 0xAD:
        return obj->unk251;
    case 0xAE:
        return obj->speed;
    case 0xAF:
        if (obj->mv->pissed_hangover != 0) {
            return 2;
        }
        if (obj->mv->pissed_up != 0) {
            return 1;
        }
        return 0;
    case 0xB2:
        return (&obj->taskint[0])[cmd->a[0].w];
    case 0xB4:
        return obj->lastAttacker;
    case 0xB5: {
        u8 attacker;
        attacker = obj->lastAttacker;
        if (attacker & 0x80) {
            return movement_obj[attacker & ~0x80].type;
        }
        return 0xFFFF;
    }
    case 0xB6: {
        u8 *p;
        s32 attacker;
        p = &D_800E0BB8[OBJ_INDEX(obj)];
        attacker = *p;
        *p &= 0x7F;
        return attacker;
    }
    case 0xB8:
        return obj->lookYaw;
    case 0xB9: {
        s32 x;
        s32 z;
        x = gSpawnPaths[obj->listpos][cmd->a[0].w].x;
        z = gSpawnPaths[obj->listpos][cmd->a[0].w].z;
        return (x << 16) | (z & 0xFFFF);
    }
    case 0xBA:
        return gSpawnPaths[obj->listpos][cmd->a[0].w].y;
    case 0xBB:
        if (obj->attach_char != 0) {
            return movement_obj[obj->attach_char - 1].listpos;
        }
        return -1;
    case 0xBC:
        return obj->lifting;
    case 0xBE:
        return obj->yspd;
    case 0xBD: {
        s32 pad1;
        s32 pad2;
        other = actor_findOnPath(cmd->a[0].w & 0xFFFF0FFF);
        dx = obj->xpos - other->xpos;
        dz = obj->zpos - other->zpos;
        return sqrtf(dx * dx + dz * dz);
    }
    case 0xBF:
        return (s32)obj->shad * 65536.0f;
    case 0xC0:
        return obj->mv->ind_cnt;
    case 0xC1:
        return obj->mv->transport;
    case 0xC2:
        return obj->unique_id;
    case 0xC3:
        return obj->mvmt_control;
    case 0xC4:
        return obj->task_attrib;
    case 0xC5:
        return obj->norun;
    case 0xC7:
        if (obj->pathGoal == 0xFF) {
            return 1;
        }
        return obj->task_attrib & 0x800;
    case 0xDB:
        if (cmd->a[0].w == 1) {
            if (obj->pathNode == 0xFF) {
                return 0xFF;
            }
            return gPathNodes[obj->pathNode].id;
        }
        if (obj->pathGoal == 0xFF) {
            return 0xFF;
        }
        return gPathNodes[obj->pathGoal].id;
    case 0xC8: {
        f32 x;
        f32 y;
        f32 z;
        s32 dist2;
        s32 dist;
        s32 found;
        if (obj->pathNode == 0xFF || obj->pathGoal == 0xFF) {
            return 0;
        }
        found = mp_getNodePos(obj->pathNode, &x, &y, &z);
        dist = -1;
        if (found != 0) {
            x -= obj->xpos;
            y -= obj->ypos;
            z -= obj->zpos;
            dist = obj->pathDistance + (s32)sqrtf(x * x + y * y + z * z);
            if (obj->pathNode != obj->pathGoal) {
                if (mp_getNodePos(obj->pathGoal, &x, &y, &z) != 0) {
                    x -= obj->xpos;
                    y -= obj->ypos;
                    z -= obj->zpos;
                    dist2 = sqrtf(x * x + y * y + z * z);
                    if (dist < dist2) {
                        return dist2 | 0;
                    }
                }
            }
        }
        return dist;
    }
    case 0xC9:
        return path_getLayerAt(obj->ypos);
    case 0xCA: {
        u8 empty;
        empty = 0xFF;
        if (obj->pathNode != 0xFF) {
            return gPathNodes[obj->pathNode].id;
        }
        gPathNearerThanNode = obj->lastNearNode;
        ret = path_getNextNode(obj->xpos, obj->ypos, obj->zpos, 0, 0, &empty, &empty);
        obj->lastNearNode = ret;
        if (ret != 0xFF) {
            return gPathNodes[ret].id;
        }
        return 0xFF;
    }
    case 0xCB: {
        f32 dx;
        f32 dy;
        f32 dz;
        f32 hx;
        f32 hy;
        f32 hz;
        f32 hd;
        f32 dist;
        other = actor_findOnPath(cmd->a[0].w & 0xFFFF0FFF);
        if (other == NULL) {
            return 0;
        }
        dx = other->xpos - obj->xpos;
        dy = other->ypos - obj->ypos;
        dz = other->zpos - obj->zpos;
        dist = sqrtf(dx * dx + dy * dy + dz * dz);
        if (collision_castRayWorld(obj->xpos, obj->ypos + 100.0f, obj->zpos, dx, dy, dz, 0, 0, &hx, &hy, &hz, &hd, 0, 0, 0.0f) == 0) {
            return dist;
        }
        if (dist < hd) {
            if (mp_castWallRay(obj->xpos, obj->ypos, obj->zpos, dx, dz) > 0.0f) {
                return 0;
            }
            return dist;
        }
        return 0;
    }
    case 0xCC: {
        TriggerZone *zone;
        zone = zone_findAt(3, 11, obj->xpos, obj->ypos, obj->zpos);
        if (zone == NULL) {
            return -1;
        }
        ret = zone - zones;
        return ret;
    }
    case 0xCF: {
        s32 found;
        u8 foundNode;
        s32 i;
        s32 ret;
        s8 count;
        s8 nAvoid; // matching: s8 (a u8 count changes the code); &nAvoid is also the empty avoid list
        u8 nodes[18];

        /* Resolve the cmd->a[0].w node ids in cmd->a[1..].w (dropping unknown ones), then pick one from the
         * object position with path_getNextNode / path_search. */
        nAvoid = 0;
        ret = cmd->a[0].w;
        count = 0;
        for (i = 0; i < ret; i++) {
            nodes[count] = mp_findNodeById(cmd->a[1 + i].w);
            if (nodes[count] != 0xFF) {
                count++;
            }
        }
        ret = -1;
        if (count != 0) {
            found = path_getNextNode(obj->xpos, obj->ypos, obj->zpos, count, nAvoid, nodes, (u8 *)&nAvoid);
            foundNode = found;
            gPathGoalReached = 0xFF;
            if ((u8)found != 0xFF) {
                path_search(foundNode, count, nAvoid, nodes, (u8 *)&nAvoid);
            }
            if (gPathGoalReached != 0xFF) {
                ret = gPathNodes[gPathGoalReached].id;
            }
        }
        return ret;
    }
    case 0xD0: {
        s32 id;
        f32 x;
        f32 y;
        f32 z;
        id = mp_findNodeById(cmd->a[0].w);
        if (mp_getNodePos(id, &x, &y, &z) != 0) {
            x -= obj->xpos;
            z -= obj->zpos;
            ret = sqrtf(x * x + z * z);
        } else {
            ret = 30000;
        }
        return ret;
    }
    case 0xCD: {
        s32 angle;
        s32 pad1;
        s32 pad2;
        f32 bearing;
        f32 facing;
        f32 delta;
        other = actor_findOnPath(cmd->a[0].w & 0xFFFF0FFF);
        angle = actor_getDirectionAngle(other->xpos - obj->xpos, obj->zpos - other->zpos, 0) + 0x4000;
        bearing = angle * 0.0054931640625f;
        facing = obj->theta;
        bearing = math_wrapDegrees(bearing);
        delta = math_wrapDegrees(math_wrapDegrees(facing) - bearing);
        if (delta > 180.0f) {
            delta = -360.0f + delta;
        }
        if (cmd->a[1].w == 1 && delta < 0.0f) {
            delta = -delta;
        }
        return delta;
    }
    case 0xCE: {
        f32 dx;
        f32 dz;
        f32 pitch;
        other = actor_findOnPath(cmd->a[0].w & 0xFFFF0FFF);
        dx = other->xpos - obj->xpos;
        dz = other->zpos - obj->zpos;
        pitch = (u32)actor_getDirectionAngle(sqrtf(dx * dx + dz * dz), other->ypos - obj->ypos, 0) * 0.0054931640625f;
        pitch = math_wrapDegrees(pitch);
        if (pitch > 180.0f) {
            pitch = -360.0f + pitch;
        }
        if (cmd->a[1].w == 1 && pitch < 0.0f) {
            pitch = -pitch;
        }
        return pitch;
    }
    case 0xD1:
        ret = obj->unk22C;
        obj->unk22C = ret & ~0x100;
        return ret & 0x100;
    case 0xD2:
        ret = obj->unk22C;
        obj->unk22C = ret & ~0x200;
        return ret & 0x200;
    case 0xD4: {
        ActorCtl *mv;
        s32 context;
        mv = obj->mv;
        context = 0;
        if (mv != NULL) {
            context = mv->rt_context_b;
        }
        return context;
    }
    case 0xD3: {
        ActorCtl *mv;
        s32 result;
        mv = obj->mv;
        result = 0;
        if (mv != NULL) {
            switch (mv->context_b & 0x7F) {
            case 3:
            case 9:
            case 21:
                return 2;
            case 33:
            case 34:
                return 3;
            case 20:
            case 22:
            case 24:
            case 35:
            case 36:
            case 55:
            case 56:
            case 57:
            case 63:
            case 65:
                result = 1;
                break;
            }
        }
        return result;
    }
    case 0xD5:
        return save_isFreeForAll();
    case 0xD6:
        return save_checkMatchOver();
    case 0xD7:
        if ((obj->fallStartY - obj->ypos) > 600.0f && obj->waterHeight == -10000.0f) {
            return 1;
        }
        return 0;
    case 0xD8:
        return obj->inwater;
    case 0xD9:
        if (obj->unkA9 == 2) {
            return 1;
        }
        return 0;
    case 0xDA:
        return obj->floorY;
    case 0xDC:
        if (obj->mv->being_sniped != 0) {
            return obj->mv->being_s_char | 0x80;
        }
        return 0;
    case 0xDD:
        if (obj->mv != NULL) {
            return obj->mv->door_walkf_time;
        }
        return 0;
    case 0xDE:
        return OBJ_INDEX(obj);
    case 0xC6:
        ret = OBJ_INDEX(obj);
        if (ret >= 0 && ret < gPlayerCount) {
            return player_getStat(OBJ_INDEX(obj), 6);
        }
        break;
    case 0xDF:
        if (obj->mv != NULL) {
            return obj->mv->ammo_count;
        }
        return 0;
    case 0xE0:
        return obj->scale_y * 65536.0f;
    case 0xE1:
        return obj->imune;
    case 0xE2:
        return obj->mv->matrix_physics;
    case 0xE3:
        return gLastDeathKind;
    case 0xE4:
        return gRespawnPending;
    case 0xE5:
        return obj->touching_char;
    case 0xE6: {
        u8 v;
        Actor *toucher;
        if (obj->touching_char != 0) {
            toucher = &movement_obj[obj->touching_char - 1];
            obj->touching_char = 0;
            if (toucher->character_type == cmd->a[0].w) {
                return 1;
            }
            return 0;
        }
        return 0;
    }
    case 0xE7: {
        u8 idx;
        PathPoint *pts;
        s32 n;
        s32 best;
        s32 x;
        s32 z;
        s32 maxd;
        s32 mind;
        s32 i;
        s32 dx;
        s32 dz;
        idx = obj->listpos;
        pts = gSpawnPaths[idx];
        n = gSpawnPathCounts[idx];
        best = -1;
        if (pts != NULL && n != 0) {
            other = actor_findOnPath(cmd->a[0].w & 0xFFFF0FFF);
            x = other->xpos;
            z = other->zpos;
            maxd = 100000000;
            mind = -1;
            if (gScriptCmdWordCount >= 4) {
                ret = cmd->a[1].w;
                mind = ret * ret;
                if (gScriptCmdWordCount >= 5) {
                    ret = cmd->a[2].w;
                    maxd = ret * ret;
                }
            }
            for (i = 0; i < n; i++, pts++) {
                dx = pts->x - x;
                dz = pts->z - z;
                dx = dz * dz + dx * dx;
                if (dx < maxd && mind < dx) {
                    best = i;
                    maxd = dx;
                }
            }
        }
        return best;
    }
    case 0xE8:
        return obj->animseq.uh;
    case 0xE9:
        return obj->mv->land_floor;
    case 0xEA: {
        ModelAttachment *attachment;
        s32 setId;
        s32 index;
        setId = cmd->a[0].w;
        index = cmd->a[1].w;
        attachment = attach_find(obj, setId, index);
        if (attachment != NULL) {
            return attachment->hookState;
        }
        return 0;
    }
    case 0xEB: {
        f32 radius;
        f32 scale;
        s32 pad2;
        s32 pad3;
        s32 pad4;
        s32 pad5;
        f32 *pp; // matching: dead address-of stores keep dz/dy/dx/dist in stack slots
        f32 dz;
        f32 dy;
        f32 dx;
        f32 dist;
        pp = &dz;
        pp = &dy;
        pp = &dx;
        pp = &dist;
        ix = cmd->a[0].w;
        iz = cmd->a[1].w;
        radius = cmd->a[2].w;
        dx = obj->xpos - ix;
        dy = 0.0f;
        dz = obj->zpos - iz;
        dist = dz * dz + dx * dx;
        if (dist <= radius * radius) {
            ret = obj->xpos;
            iz = obj->zpos;
        } else {
            scale = radius / sqrtf(dist);
            ret = (dx *= scale) + ix;
            iz = (dz *= scale) + iz;
        }
        return (ret << 16) | (iz & 0xFFFF);
    }
    case 0xEC:
        return obj->mv->ladder;
    case 0xED:
        return gButtonsPressed[objId];
    case 0xEE: {
        s16 px;
        f32 x;
        f32 y;
        f32 z;

        /* Offset cmd->a[0].w along the reverse of facing angle unk7A, packed as (x << 16) | (z & 0xFFFF). */
        actor_splitVelocity(obj->langle + 0x8000, cmd->a[0].w, 0.0f, &x, &z, &y);
        px = (s16)(s32)obj->xpos + x;
        iz = (s16)((s16)(s32)obj->zpos + z);
        return (px << 16) | (iz & 0xFFFF);
    }
    case 0xEF:
        return obj->type;
    case 0xF0:
        return gPlayerRanks[obj->team];
    case 0xF1:
        return (obj->cullMask & 0xF) == 0xF;
    case 0xF2:
        if (obj->lifting != 0) {
            return movement_obj[obj->lifting].listpos | 0x2000;
        }
        return 0;
    case 0xF3:
        if (obj->unkA9 == 2) {
            return 1;
        }
        return 0;
    default:
        return 0;
    }
    return 0;
}
