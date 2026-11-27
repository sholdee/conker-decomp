#include <ultra64.h>
#include "conker/actor.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/sound.h"
#include "conker/weapon.h"


void effect_levelUpdateCutsceneTreads(s32 arg0) {
    s32 i;

    sound_updateCooldowns();
    if (gCutsceneStates[0] == 1) {
        for (i = 0; i < 25; i++) {
            if (movement_obj[i].character_type == 0x28) {
                weapon_scrollTreads(&movement_obj[i], 0x14, 0x14);
            }
        }
    }
}
