#include <ultra64.h>

#include "conker/gfx.h"
#include "conker/math.h"
#include "conker/player.h"
#include "conker/rom.h"


void player_offsetLinkGroupObjects(s32 offset) {
    s32 i;
    s32 j;

    for (i = 0; i < (s32)gLinkGroupCount; i++) {
        for (j = 0; j < gLinkGroups[i].count; j++) {
            if ((gLinkGroups[i].members[j] >> 12) == 2) {
                gLinkGroups[i].members[j] += offset;
            }
        }
    }
}

void player_loadLinkGroups(s32 levelId) {
    u32 size = 0;
    gLinkGroups = rom_loadPath(&size, 3, 12, levelId, 4);
    gLinkGroupCount = size / 24;
    player_offsetLinkGroupObjects(gUngroupedObjectCount);
}
