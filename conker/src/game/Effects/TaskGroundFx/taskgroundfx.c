#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/sound.h"

/* ActorTask callback (see game/Effects/ActorTasks/actortasks.c; only the task's CharacterWatcher is read here): ground effect 5
 * under the task's actor, with sound 0x5208. */
void func_151AE6B0(CharacterWatcher *task) {
    Actor *actor = task->character;

    {
        Vec3f pos;
        CollisionHit *hit;

        pos.x = actor->xpos;
        if (-9500.0f < actor->waterHeight) {
            pos.y = actor->waterHeight + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        hit = &task->hit;
        if (collision_probeFloorSurfBCached(&pos, 0, pos.y - 300.0f, hit) != 0) {
            pos.y = task->hit.height;
            effect_spawnImpact(&pos, hit, 5, task->header.owner, task->header.kind);
            sound_playOnActor(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
}
