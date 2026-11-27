#include <ultra64.h>
#include "conker/actor.h"
#include "conker/cutscene.h"
#include "conker/game.h"


/* Cutscene start callback 1 (gCutsceneStartCallbacks): when LEVEL_W1_COG_LAND starts cutscene 2, track 5 targets the
   actor of path `actorPath` (its unique id). */
void cutscene_startCallback1(s32 map, s32 cutscene, s32 scene, s32 actorPath) {
    Actor *actor;

    if (map == LEVEL_W1_COG_LAND && cutscene == 2) {
        actor = actor_findOnPath(actorPath);
        if (actor != NULL) {
            gSeqTrackInfos[scene][5].targetId = actor->unique_id;
        }
    }
}
