#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Script operand value meaning "leave this field unchanged". */
#define ARG_KEEP 0x7FFFFFFF

/* A word that holds either a float or the raw value a script stored there. */
typedef union {
    f32 f;
    s32 i;
} FloatOrWord;

/* Script-set scratch words of an object (its WorldObject.touch), each a float or a raw value. */
#define WORK(obj) ((FloatOrWord *)&(obj)->touch)

/* Script "set" handler for the object with id `id`: applies operation `op` with operands cmd[2..]. */
s32 script_worldObjectCommand(s32 id, s32 op, s32 *cmd) {
    WorldObject *obj;
    f32 x;
    f32 y;
    f32 z;
    s32 index;
    s32 pad[2]; // matching: unused stack slots (target frame is 0x40)

    obj = gfx_findObjectById(id);
    if (obj == NULL) {
        return 0;
    }
    switch (op) {
    case 0:
        obj->inactive = 0;
        if (obj->moveProc != NULL) {
            obj->viewFlags |= 0xF;
        }
        return 1;
    case 1:
        obj->inactive = 1;
        return 1;
    case 2:
        if (cmd[2] != ARG_KEEP) {
            obj->x = cmd[2];
        }
        if (cmd[3] != ARG_KEEP) {
            obj->y = cmd[3];
        }
        if (cmd[4] != ARG_KEEP) {
            obj->z = cmd[4];
        }
        return 1;
    case 12:
        index = mp_findNodeById(cmd[2]);
        if (index != -1) {
            mp_getNodePos(index, &x, &y, &z);
            obj->x = x;
            obj->y = cmd[3] + (s16)y;
            obj->z = z;
        }
        return 1;
    case 3:
        /* 16.16 fixed-point operands; 0x78000000 means "unchanged" */
        if (cmd[2] != 2013265920.0f) {
        unused_label: // matching: a label ends the block, so as1 won't pull the 65536.0f load into a bc1tl slot
            WORK(obj)[0].f = cmd[2] / 65536.0f;
        }
        if (cmd[3] != 2013265920.0f) {
            WORK(obj)[1].f = cmd[3] / 65536.0f;
        }
        if (cmd[4] != 2013265920.0f) {
            WORK(obj)[2].f = cmd[4] / 65536.0f;
        }
        return 1;
    case 4:
        if (cmd[2] != ARG_KEEP) {
            WORK(obj)[0].i = cmd[2];
        }
        if (cmd[3] != ARG_KEEP) {
            WORK(obj)[1].i = cmd[3];
        }
        if (cmd[4] != ARG_KEEP) {
            WORK(obj)[2].i = cmd[4];
        }
        return 1;
    case 5:
        obj->moveData.word = cmd[2];
        return 1;
    case 6:
        obj->task_flag &= ~TASK_FLAG_STATE;
        obj->task_flag |= cmd[2] & TASK_FLAG_STATE;
        return 1;
    case 7:
        if (cmd[2] != 0) {
            obj->task_flag |= TASK_FLAG_LOCKED;
        } else {
            obj->task_flag &= ~TASK_FLAG_LOCKED;
        }
        return 1;
    case 8:
        obj->moveData.word &= 0xFFFF;
        obj->moveData.word |= cmd[2] << 16;
        WORK(obj)[0].i = cmd[3];
        return 1;
    case 9:
        effect_spawnGroundImpactsUnder(obj);
        break;
    case 10:
        obj->moveProc = gObjectBehaviors[cmd[2]].moveProc;
        obj->onContact = gObjectBehaviors[cmd[2]].onContact;
        obj->contactNonPlayers = gObjectBehaviors[cmd[2]].contactNonPlayers;
        break;
    case 11:
        env_fadePropVtxColours(cmd[2], cmd[3], obj->unique_id);
        break;
    case 13:
        obj->object_status = (obj->object_status & ~cmd[3]) | cmd[2];
        break;
    case 14:
        if (cmd[2] != ARG_KEEP) {
            obj->moveX = cmd[2];
        }
        if (cmd[3] != ARG_KEEP) {
            obj->moveY = cmd[3];
        }
        if (cmd[4] != ARG_KEEP) {
            obj->moveZ = cmd[4];
        }
        break;
    case 15:
        if (cmd[2] != ARG_KEEP) {
            obj->spinX = cmd[2];
        }
        if (cmd[3] != ARG_KEEP) {
            obj->spinY = cmd[3];
        }
        if (cmd[4] != ARG_KEEP) {
            obj->spinZ = cmd[4];
        }
        break;
    case 16:
        if (cmd[2] != ARG_KEEP) {
            obj->t = cmd[2];
        }
        if (cmd[3] != ARG_KEEP) {
            obj->u = cmd[3];
        }
        if (cmd[4] != ARG_KEEP) {
            obj->v = cmd[4];
        }
        break;
    case 17:
        obj->fadeDistance = cmd[2];
        break;
    case 18:
        obj->alpha = cmd[2];
        break;
    }
    return 0;
}


/* Script "query" handler for the object with id `id`: returns the value that operation `op` asks for. */
s32 script_worldObjectQuery(s32 id, s32 op, s32 *cmd) {
    WorldObject *obj;
    s32 i;
    s32 result;

    obj = gfx_findObjectById(id);
    if (obj == NULL) {
        return 0;
    }
    switch (op) {
    case 19:
        return gfx_isObjectTouchedBy(obj, -1);
    case 20:
        return gfx_isObjectTouchedBy(obj, collision_findSlotBySpawnIndex(cmd[2] & 0xFFFF0FFF));
    case 21:
        for (i = 0; i < 25; i++) {
            if (movement_obj[i].type == cmd[2] && gfx_isObjectTouchedBy(obj, i)) {
                return 1;
            }
        }
        return 0;
    case 22:
        if ((obj->object_status & 4) == 4) {
            return 1;
        }
        return 0;
    case 23:
        result = gfx_getTouchingActor(obj);
        if (result < 0) {
            return 0;
        }
        return result | 0x2000;
    case 24:
        return obj->object_status;
    case 25:
        if (WORK(obj)[2].i == WORK(obj)[0].i) {
            return 1;
        }
        return 0;
    case 26:
        return obj->moveData.word;
    case 27:
        return obj->task_flag & TASK_FLAG_STATE;
    case 28:
        return obj->inactive == 0;
    case 29:
        return obj->x;
    case 30:
        return obj->y;
    case 31:
        return obj->z;
    case 32:
        return obj->v;
    case 33:
        if (cmd[2] == 0) {
            return WORK(obj)[0].i;
        }
        if (cmd[2] == 1) {
            return WORK(obj)[1].i;
        }
        if (cmd[2] == 2) {
            return WORK(obj)[2].i;
        }
        break;
    case 34:
        if (cmd[2] == 0) {
            return WORK(obj)[0].f;
        }
        if (cmd[2] == 1) {
            return WORK(obj)[1].f;
        }
        if (cmd[2] == 2) {
            return WORK(obj)[2].f;
        }
        break;
    case 35:
        return (obj->task_flag & TASK_FLAG_LOCKED) != 0;
    }
    return 0;
}

/* Looks like the script format version: later versions carry more operands. */

/* K&R definitions below (they narrow their short params from the stack); these are their promoted prototypes. */

/* Script "play sound" handler: plays `sound` the way cmd[1] selects (plain, ranged/positioned,
 * on an actor, at an actor's position...). Returns (handle << 16) | 0x7000, or 0 if none plays. */
s32 script_playSound(s32 sound, u32 *cmd) {
    s32 ret;
    s32 target;
    s32 cents;
    s32 pan;
    s32 force;

    if (gScriptCmdWordCount == 1) {
        ret = script_playPlainSound(sound, 0x7FFF, 0, 0x40);
    } else {
        ret = cmd[1]; // matching: the kind must share a register with the result
        switch (ret) {
        case 1:
            pan = 0x40;
            if (gScriptCmdWordCount >= 4) {
                cents = cmd[3];
                if (gScriptCmdWordCount == 5) {
                    pan = cmd[4];
                }
            } else {
                cents = 0;
            }
            ret = script_playPlainSound(sound, cmd[2], cents, pan);
            break;
        case 0:
            /* a random sound from sound..cmd[2], placed by cmd[5]: 0xTnnn = kind T, target nnn */
            target = cmd[5];
            switch (target & 0xF000) {
            case 0x2000:
                return script_playRandomActorSound(sound, cmd[2] & 0xFFF, cmd[3], cmd[4], target & 0xFFF, cmd[6], cmd[7]);
            case 0x3000:
                script_playRandomObjectSound(sound, cmd[2] & 0xFFF, cmd[4], target & 0xFFF, cmd[6], cmd[7]);
                return 0;
            case 0x4000:
                return script_playRandomZoneSound(sound, cmd[2] & 0xFFF, cmd[4], target & 0xFFF, cmd[6]);
            default:
                ret = script_playRandomSound(sound, cmd[2] & 0xFFF, cmd[4]);
                break;
            }
            break;
        case 2:
            force = FALSE;
            if (gScriptCmdWordCount >= 7 && cmd[6] != 0) {
                force = TRUE;
            }
            script_playActorSound(sound & 0xFFF, cmd[2], cmd[3] & 0xFFF, cmd[4], cmd[5], force);
            ret = 0;
            break;
        case 3:
            ret = script_playSoundAtActor(sound, cmd[3] & 0xFFF, cmd[2], cmd[4], cmd[5]);
            break;
        case 4:
            sound_playWithCooldown(sound, cmd[3] & 0xFFF, cmd[2], cmd[4], cmd[5]);
            return 0x7000;
        default:
            return 0;
        }
    }
    if (ret != 0) {
        ret = (ret << 16) | 0x7000;
    }
    return ret;
}

/* Script "stop sound" handler: stops the voice whose handle is in the command's top halfword. */
s32 script_stopSound(s32 arg0, u32 *cmd) {
    if (arg0 != 0) {
        sound_stop((cmd[0] & 0xFFFF0000) >> 16);
    }
    return 1;
}

/* Script actor-sound handler: stops (op 0/1) or moves (op 2) the sound of the actor on path cmd[2]. */
s32 script_actorSoundCommand(s32 arg0, s32 op, u32 *cmd) {
    switch (op) {
    case 1:
        script_stopActorSound(cmd[2] & 0xFFF, FALSE);
        return 1;
    case 0:
        script_stopActorSound(cmd[2] & 0xFFF, TRUE);
        return 1;
    case 2:
        script_moveSoundToActor((cmd[0] & 0xFFFF0000) >> 16, cmd[2] & 0xFFF, cmd[3], cmd[4], cmd[5]);
        return 1;
    }
    return 0;
}

s32 script_soundQuery(s32 arg0, s32 arg1, s32 arg2) {
    return 0;
}

/* Plays a random sound from first..last on the actor following `path`, picking only sounds whose
 * bit is still set in `unplayed` (all of them when it is 0). Returns the mask minus the one played. */
s32 script_playRandomActorSound(first, last, unplayed, volume, path, nearDistance, farDistance)
    s32 first; s32 last; s32 unplayed; u32 volume; s32 path; s16 nearDistance; s32 farDistance;
{
    Actor *actor;
    s32 count;
    u8 pick;

    actor = actor_findOnPath(path);
    if (volume >= 0x8000) {
        volume = 0x7FFF;
    }
    if (actor == NULL) {
        return unplayed;
    }
    if (!sound_isActorSound1Playing(actor)) {
        if (unplayed == 0) {
            unplayed = 0xFFFFFFFF >> (32 - (u8)(last - first + 1));
        }
        count = (u8)(last - first + 1);
        do {
            pick = math_random() % (u32)count;
        } while (!((1 << pick) & unplayed));

        if (sound_playActorSound1(pick + first, actor, volume, nearDistance, farDistance)) {
            unplayed ^= 1 << pick;
        }
    }
    return unplayed;
}

/* Plays a random sound from first..last on the object with id `objId`. */
void script_playRandomObjectSound(first, last, volume, objId, nearDistance, farDistance)
    s32 first; s32 last; s32 volume; s32 objId; s16 nearDistance; s32 farDistance;
{
    WorldObject *obj;

    obj = gfx_findObjectById(objId);
    gfx_playObjectSound(obj, sound_pickVariant(first, (u8)(last - first + 1)), volume, nearDistance, farDistance, 0);
}

/* Plays a random sound from first..last at point `index` of the zones table, whose halfword at
 * 0x6 is the far distance (nearDistance is kept below it). */
s32 script_playRandomZoneSound(s32 first, s32 last, s32 volume, s32 index, u32 nearDistance) {
    TriggerZone *point;
    s32 farDistance;

    point = &zones[index];
    farDistance = point->xSize;
    if (nearDistance >= farDistance) {
        nearDistance = farDistance - 1;
    }
    return sound_playAt(sound_pickVariant(first, (u8)(last - first + 1)), volume, 0, 0, 0, point->x, point->y,
                         point->z, nearDistance, farDistance);
}

/* Plays a random sound from first..last, unpositioned. */
s32 script_playRandomSound(s32 first, s32 last, s32 volume) {
    return sound_play(sound_pickVariant(first, (u8)(last - first + 1)), volume, 0x40, 0, 0);
}

/* Plays `sound` on the actor following `path`, unless the actor is already playing one and `force` is clear. */
void script_playActorSound(sound, volume, path, nearDistance, farDistance, force)
    u16 sound; s32 volume; s32 path; s16 nearDistance; s32 farDistance; s32 force;
{
    Actor *actor;

    actor = actor_findOnPath(path);
    if (actor != NULL) {
        if (force || !sound_isActorSound1Playing(actor)) {
            sound_playActorSound1(sound, actor, volume, nearDistance, farDistance);
        }
    }
}

/* Stops the sound of the actor following `path`: its 0x8C voice if `secondVoice`, else its 0x8E voice. */
void script_stopActorSound(s32 path, s32 secondVoice) {
    Actor *actor;

    actor = actor_findOnPath(path);
    if (actor != NULL) {
        if (secondVoice) {
            sound_stopActorSound1(actor);
        } else {
            sound_stopActorSound2(actor);
        }
    }
}

/* Plays `sound` unpositioned. */
s32 script_playPlainSound(s32 sound, s32 volume, s32 cents, s32 pan) {
    return sound_play(sound, volume, pan, cents, 0);
}

/* Plays `sound` at the position of the actor following `path`. */
s32 script_playSoundAtActor(s32 sound, s32 path, s32 volume, s32 nearDistance, s32 farDistance) {
    Actor *actor;
    s32 x;
    s32 y;
    s32 z;

    actor = actor_findOnPath(path);
    if (actor == NULL) {
        return 0;
    }
    x = actor->xpos;
    y = actor->ypos;
    z = actor->zpos;
    return sound_playAt(sound, volume, 0, 0, 0, x, y, z, nearDistance, farDistance);
}

/* Moves the voice `handle` to the position of the actor following `path`. */
void script_moveSoundToActor(s32 handle, s32 path, s32 cents, s32 nearDistance, s32 farDistance) {
    Actor *actor;
    s32 x;
    s32 y;
    s32 z;

    actor = actor_findOnPath(path);
    x = actor->xpos;
    y = actor->ypos;
    z = actor->zpos;
    sound_setVoiceSpatial(handle, 0x7FFF, cents, 0, 0, x, y, z, nearDistance, farDistance);
}

s32 script_emptyQuery12(s32 arg0, s32 arg1, s32 arg2) {
    return 0;
}

s32 script_trapCommand12(s32 arg0, s32 arg1, s32 arg2) {
    debug_fatalTrap();
    return 0;
}
