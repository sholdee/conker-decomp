#include <ultra64.h>
#include "conker/camera.h"
#include "conker/game.h"
#include "conker/script.h"



/* Sets flag bits in actor->flags: 0x01000000 while in water, and level-specific bits
 * from script_query queries for the levels LEVEL_WMP_TUNNELS, LEVEL_WMP_BEACH_WAR and LEVEL_WMP_MAIN_WAR. */
void camera_levelHook15(Camera *actor) {
    s32 queryFlags;

    queryFlags = actor->CNumber | 0x9000;
    if (actor->CharPtr->inwater == 1) {
        actor->AvailableModes |= 0x01000000;
    } else {
        actor->AvailableModes &= ~0x01000000;
    }

    if (level == LEVEL_WMP_TUNNELS) {
        if (script_query(1, 0x406D, 6, queryFlags) != 0) {
            actor->AvailableModes |= 0x200;
        } else {
            actor->AvailableModes &= ~0x200;
        }
    } else if (level == LEVEL_WMP_BEACH_WAR) {
        if (script_query(1, 0x403C, 6, queryFlags) != 0 ||
            script_query(1, 0x403D, 6, queryFlags) != 0) {
            actor->AvailableModes |= 0x200;
        } else {
            actor->AvailableModes &= ~0x200;
        }
    } else if (level == LEVEL_WMP_MAIN_WAR) {
        if (script_query(1, 0x4053, 6, queryFlags) != 0 ||
            script_query(1, 0x4054, 6, queryFlags) != 0 ||
            script_query(1, 0x4056, 6, queryFlags) != 0 ||
            script_query(1, 0x4057, 6, queryFlags) != 0) {
            actor->AvailableModes |= 0x20000000;
        } else {
            actor->AvailableModes &= ~0x20000000;
        }

        if (script_query(1, 0x4058, 6, 0x9000) != 0 ||
            script_query(1, 0x4059, 6, 0x9000) != 0) {
            actor->AvailableModes |= 0x80000000;
        } else {
            actor->AvailableModes &= ~0x80000000;
        }
    } else if (level == LEVEL_WMP_TUNNELS) {
        // Unreachable (0x34 is handled above), but the original code has this check.
        if (script_query(1, 0x406E, 6, queryFlags) != 0) {
            actor->AvailableModes |= 0x20000000;
        } else {
            actor->AvailableModes &= ~0x20000000;
        }
    }
}
