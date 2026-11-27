#include <ultra64.h>

#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/level.h"


void level_queueObjectDrop04(s32 x, s32 y, s32 z, s32 floorY, s32 markOnLanding) {
    ObjectEmitter *emitter = effect_newTimedEffectNoDraw((math_random() & 0x7F) + 0xA, 1, -1, 1, 0, 0xA, 0xFF, 0);
    if (emitter != NULL) {
        emitter->pos.x = x;
        emitter->pos.y = y;
        emitter->pos.z = z;
        emitter->pos.floorY = floorY;
        emitter->pos.markOnLanding = markOnLanding;
    }
}

void level_setup04(void) {
    level_queueObjectDrop04(386,  571,  -1576,  422, 0);
    level_queueObjectDrop04(-23,  797,  -2235, -145, 0);
    level_queueObjectDrop04(113,  745,  -2246, -190, 0);
    level_queueObjectDrop04(388,  1124, -1741,  481, 0);
    level_queueObjectDrop04(388,  1124, -1560,  643, 0);
    level_queueObjectDrop04(388,  1124, -1104,  916, 0);
    level_queueObjectDrop04(1464, 1095, -2654, -492, 1);
    level_queueObjectDrop04(1448, 1095, -2431,  502, 0);
    level_queueObjectDrop04(1409, 1116, -1993, -379, 0);
    level_queueObjectDrop04(1403, 1120, -569,  -404, 0);
    level_queueObjectDrop04(1540, 1371, -1265, -525, 0);
    level_queueObjectDrop04(892,  2181, -1759, 1736, 0);
    level_queueObjectDrop04(763,  2181, -1225, 1654, 0);
    level_queueObjectDrop04(1209, 2181, -899,  1376, 0);
    level_queueObjectDrop04(1719, 2181, -781,  -565, 0);
    level_queueObjectDrop04(1162, 2181, -644,  1437, 0);
    level_queueObjectDrop04(1932, 2181, -1266, -623, 1);
}
