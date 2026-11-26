#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A data-only unit after game/Collision/Spawn/spawn in the link: its .rodata (0x241890) holds front-end data after the
 * float literals and jump tables of game/Collision/Spawn/spawn (0x2416F0-0x24188C, then its padding), so it is not part
 * of that unit; its .data (0x22B750) starts on a 16-byte unit boundary after game/Collision/HitSphere/hitsphere's. It holds the
 * model tables of the actor code: the object filters, the sub-model lists, the part sets, and the
 * expression actions of model 0 (game_AB760).
 */

/* Sub-model lists (gSubModelLists): model ids, ModelDef.subModelCount LODs then the extra models. */
const u8 D_8009CDD0[7] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x82, 0x96 };
const u8 D_8009CDD8[6] = { 0x96, 0x96, 0x96, 0x96, 0x96, 0x82 };
const u8 D_8009CDE0[6] = { 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3D };
const u8 D_8009CDE8[2] = { 0x54, 0x68 };
const u8 D_8009CDEC[3] = { 0x5A, 0x5A, 0x5A };

/* The parts of each part set (gPartSets). */
static const PartDef D_8009CDF0[1] = {
    { 0x0C, 15, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CE00[1] = {
    { 0x0C, 10, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CE10[1] = {
    { 0x0C, 15, 0, 1, 0, 0, 0, 0, { 0x000F, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CE20[1] = {
    { 0x30, 11, 18, 0, 1, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CE30[1] = {
    { 0x0E, 6, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CE40[1] = {
    { 0x0D, 16, 0, 1, 0, 0, 0, 1, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CE50[1] = {
    { 0x84, 12, 6, 2, 0, 0, 1, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CE60[1] = {
    { 0x0F, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CE70[1] = {
    { 0x10, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CE80[1] = {
    { 0x11, 4, 14, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CE90[1] = {
    { 0x13, 21, 1, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CEA0[1] = {
    { 0x14, 18, 0, 1, 0, 0, 0, 1, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CEB0[1] = {
    { 0x12, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CEC0[1] = {
    { 0x15, 9, 0, 1, 0, 0, 0, 0, { 0x0044, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CED0[2] = {
    { 0x84, 12, 6, 2, 0, 0, 1, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
    { 0x12, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CEF0[2] = {
    { 0x17, 4, 0, 1, 0, 0, 0, 0, { 0x0045, 0x0000, 0x0000 }, 0, 0 },
    { 0x16, 9, 0, 1, 0, 0, 0, 0, { 0x0045, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CF10[1] = {
    { 0x40, 9, 5, 0, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CF20[1] = {
    { 0x18, 9, 0, 1, 0, 0, 0, 0, { 0x0046, 0x01DC, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CF30[1] = {
    { 0x19, 9, 2, 1, 0, 0, 0, 0, { 0x000D, 0x019F, 0x000E }, 0, 0 },
};
static const PartDef D_8009CF40[1] = {
    { 0x1A, 4, 23, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CF50[1] = {
    { 0x1B, 9, 23, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CF60[1] = {
    { 0x1A, 4, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 64, 1 },
};
static const PartDef D_8009CF70[1] = {
    { 0x1B, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009CF80[1] = {
    { 0x6C, 23, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CF90[1] = {
    { 0x23, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CFA0[1] = {
    { 0x6B, 23, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CFB0[1] = {
    { 0x41, 9, 16, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CFC0[1] = {
    { 0x20, 9, 24, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CFD0[1] = {
    { 0x20, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009CFE0[1] = {
    { 0x20, 14, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009CFF0[1] = {
    { 0x4C, 4, 18, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D000[1] = {
    { 0x89, 0, 5, 2, 0, 0, 9, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D010[1] = {
    { 0x1C, 9, 3, 1, 0, 0, 0, 0, { 0x0055, 0x0057, 0x0056 }, 0, 0 },
};
static const PartDef D_8009D020[1] = {
    { 0x24, 0, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D030[1] = {
    { 0x8C, 0, 9, 2, 0, 0, 12, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D040[1] = {
    { 0x8E, 10, 5, 2, 0, 0, 6, 1, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D050[1] = {
    { 0x8D, 9, 5, 2, 0, 0, 7, 1, { 0x009B, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D060[1] = {
    { 0x8D, 9, 5, 2, 0, 0, 7, 1, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D070[1] = {
    { 0x8F, 9, 5, 2, 0, 0, 13, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D080[1] = {
    { 0x90, 4, 5, 2, 0, 0, 14, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D090[1] = {
    { 0x91, 0, 5, 2, 0, 0, 15, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D0A0[1] = {
    { 0x32, 0, 10, 1, 0, 4, 0, 24, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D0B0[1] = {
    { 0x32, 0, 0, 1, 0, 4, 0, 8, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D0C0[1] = {
    { 0x32, 0, 0, 1, 0, 4, 0, 16, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D0D0[1] = {
    { 0x33, 0, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D0E0[1] = {
    { 0x34, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D0F0[1] = {
    { 0x30, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D100[1] = {
    { 0x36, 51, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D110[1] = {
    { 0x97, 21, 12, 2, 0, 0, 20, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D120[1] = {
    { 0x31, 9, 4, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D130[1] = {
    { 0x92, 18, 5, 2, 0, 0, 16, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D140[1] = {
    { 0x93, 14, 5, 2, 0, 0, 16, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D150[1] = {
    { 0x94, 14, 5, 2, 0, 0, 16, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D160[1] = {
    { 0x95, 2, 0, 2, 0, 0, 17, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D170[1] = {
    { 0x8B, 9, 0, 2, 0, 0, 11, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D180[1] = {
    { 0x1F, 9, 0, 1, 0, 0, 0, 0, { 0x0068, 0x0091, 0x0094 }, 0, 0 },
};
static const PartDef D_8009D190[1] = {
    { 0x21, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D1A0[1] = {
    { 0x7B, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D1B0[1] = {
    { 0x22, 4, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D1C0[1] = {
    { 0x23, 0, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D1D0[1] = {
    { 0x7C, 0, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D1E0[1] = {
    { 0x85, 12, 5, 2, 0, 0, 2, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D1F0[1] = {
    { 0x1D, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D200[1] = {
    { 0x1E, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D210[1] = {
    { 0x50, 9, 19, 1, 0, 0, 0, 2, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D220[1] = {
    { 0x2D, 23, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D230[1] = {
    { 0x2E, 16, 0, 1, 0, 0, 0, 4, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D240[1] = {
    { 0x96, 18, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D250[1] = {
    { 0x2F, 23, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D260[1] = {
    { 0x41, 0, 28, 0, 2, 4, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D270[1] = {
    { 0x87, 9, 5, 2, 0, 0, 4, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D280[1] = {
    { 0x83, 19, 5, 2, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D290[1] = {
    { 0x88, 9, 5, 2, 0, 0, 5, 0, { 0x0011, 0x0026, 0x0029 }, 0, 0 },
};
static const PartDef D_8009D2A0[1] = {
    { 0x98, 9, 5, 2, 0, 0, 5, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D2B0[1] = {
    { 0x98, 9, 5, 2, 0, 0, 5, 0, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D2C0[1] = {
    { 0x9F, 0, 5, 2, 0, 0, 28, 1, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D2D0[1] = {
    { 0x9F, 0, 5, 2, 0, 0, 28, 1, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D2E0[1] = {
    { 0x81, 0, 0, 1, 0, 0, 0, 1, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D2F0[1] = {
    { 0x99, 18, 15, 2, 0, 0, 22, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D300[1] = {
    { 0x9A, 18, 5, 2, 0, 0, 24, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D310[1] = {
    { 0x9B, 9, 5, 2, 0, 0, 25, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D320[1] = {
    { 0x9C, 12, 5, 2, 0, 0, 25, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D330[1] = {
    { 0x9B, 9, 5, 2, 0, 0, 25, 0, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D340[1] = {
    { 0x9D, 9, 5, 2, 0, 0, 26, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D350[1] = {
    { 0x9D, 9, 5, 2, 0, 0, 26, 0, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D360[1] = {
    { 0x51, 19, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D370[1] = {
    { 0x4F, 0, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D380[1] = {
    { 0x4E, 0, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D390[1] = {
    { 0x1D, 9, 28, 0, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D3A0[1] = {
    { 0x6E, 9, 28, 0, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D3B0[1] = {
    { 0x2F, 9, 28, 0, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D3C0[1] = {
    { 0x27, 2, 11, 1, 0, 0, 0, 12, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D3D0[1] = {
    { 0x35, 2, 11, 1, 0, 0, 0, 12, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D3E0[1] = {
    { 0x28, 12, 8, 1, 0, 0, 0, 2, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D3F0[1] = {
    { 0x78, 9, 0, 1, 0, 0, 0, 2, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D400[1] = {
    { 0x2B, 12, 0, 1, 0, 0, 0, 1, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D410[1] = {
    { 0x2C, 4, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D420[1] = {
    { 0x29, 9, 11, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D430[1] = {
    { 0x2A, 0, 11, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D440[1] = {
    { 0x65, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D450[2] = {
    { 0x26, 2, 7, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
    { 0x25, 7, 7, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D470[1] = {
    { 0x12, 0, 2, 0, 0, 52, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D480[1] = {
    { 0x89, 9, 38, 0, 0, 4, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D490[1] = {
    { 0xBA, 9, 38, 0, 0, 4, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D4A0[1] = {
    { 0x37, 9, 17, 1, 0, 0, 0, 5, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D4B0[1] = {
    { 0x38, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D4C0[1] = {
    { 0x39, 14, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D4D0[1] = {
    { 0x7A, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D4E0[1] = {
    { 0x3A, 9, 0, 1, 0, 0, 0, 2, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D4F0[1] = {
    { 0x3A, 9, 0, 1, 0, 0, 0, 2, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D500[1] = {
    { 0x40, 4, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D510[1] = {
    { 0x3C, 3, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D520[1] = {
    { 0x3D, 1, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D530[1] = {
    { 0x3F, 2, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D540[1] = {
    { 0x3E, 2, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D550[1] = {
    { 0x3B, 40, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D560[1] = {
    { 0x3B, 0, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D570[1] = {
    { 0x42, 12, 22, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D580[1] = {
    { 0x43, 9, 22, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D590[1] = {
    { 0x43, 9, 22, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D5A0[1] = {
    { 0x46, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D5B0[1] = {
    { 0x47, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D5C0[1] = {
    { 0x44, 2, 0, 1, 0, 0, 0, 2, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D5D0[1] = {
    { 0x45, 2, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D5E0[1] = {
    { 0x48, 17, 21, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D5F0[1] = {
    { 0x49, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D600[1] = {
    { 0x4A, 19, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D610[1] = {
    { 0x52, 14, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D620[1] = {
    { 0x53, 0, 0, 1, 0, 0, 0, 8, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D630[1] = {
    { 0x54, 14, 0, 1, 0, 0, 0, 8, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D640[1] = {
    { 0x55, 0, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D650[1] = {
    { 0x56, 4, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D660[1] = {
    { 0x5A, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D670[1] = {
    { 0x5B, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D680[1] = {
    { 0x5C, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D690[1] = {
    { 0x5D, 23, 20, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D6A0[1] = {
    { 0x5F, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D6B0[1] = {
    { 0x7F, 14, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D6C0[1] = {
    { 0x5E, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D6D0[1] = {
    { 0x5D, 0, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D6E0[1] = {
    { 0x57, 14, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D6F0[1] = {
    { 0x58, 1, 0, 1, 0, 0, 0, 4, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D700[1] = {
    { 0x59, 0, 0, 1, 0, 0, 0, 4, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D710[1] = {
    { 0x4B, 4, 18, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D720[1] = {
    { 0x4D, 9, 16, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D730[1] = {
    { 0x60, 23, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D740[1] = {
    { 0x61, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D750[1] = {
    { 0x63, 4, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D760[1] = {
    { 0x62, 44, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D770[1] = {
    { 0x64, 23, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D780[1] = {
    { 0x66, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D790[1] = {
    { 0x67, 9, 16, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D7A0[1] = {
    { 0x69, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D7B0[1] = {
    { 0x6A, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D7C0[1] = {
    { 0x6A, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D7D0[1] = {
    { 0x6D, 4, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D7E0[1] = {
    { 0x6E, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D7F0[1] = {
    { 0x6F, 4, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 64, 1 },
};
static const PartDef D_8009D800[1] = {
    { 0x6F, 0, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D810[1] = {
    { 0x70, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 65, 1 },
};
static const PartDef D_8009D820[1] = {
    { 0x70, 0, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D830[1] = {
    { 0x71, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D840[1] = {
    { 0x75, 0, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D850[1] = {
    { 0x77, 4, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D860[1] = {
    { 0x76, 14, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D870[1] = {
    { 0x72, 4, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D880[1] = {
    { 0x73, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D890[1] = {
    { 0x74, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D8A0[1] = {
    { 0x9E, 9, 5, 2, 0, 0, 27, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D8B0[1] = {
    { 0x68, 9, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D8C0[1] = {
    { 0x79, 12, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D8D0[1] = {
    { 0x7D, 15, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D8E0[1] = {
    { 0x7E, 11, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D8F0[1] = {
    { 0x80, 2, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};
static const PartDef D_8009D900[1] = {
    { 0x82, 14, 0, 1, 0, 0, 0, 0, { 0x0000, 0x0000, 0x0000 }, 0, 0 },
};

/* The expression actions of model 0 (face_getActionTable). */
const u8 gModel0ExpressionActions[5] = { 5, 6, 7, 10, 11 };

/*
 * This file's .data. The object filters of the ray and range searches (game/Collision/RayCast/raycast.c, game/Particle/HitImpact/hitimpact.c):
 * declared without a prototype, as the filters differ in their second and third parameters.
 */
s32 (*gObjectFilters[7])() = {
    func_150FC930, func_150FF6B4, func_151C6290, weapon_filterBulletTarget, func_150EF784, NULL, NULL,
};

/* Per ModelDef.subModelList: the model's sub-model list. */
const u8 *gSubModelLists[6] = {
    NULL, D_8009CDD0, D_8009CDE0, D_8009CDD8, D_8009CDE8, D_8009CDEC,
};

/* The part sets of ModelDef.partSet (1-based). */
PartSet gPartSets[175] = {
    { D_8009CDF0, 1 },
    { D_8009CE20, 1 },
    { D_8009CE30, 1 },
    { D_8009CE40, 1 },
    { D_8009CE50, 1 },
    { D_8009CE60, 1 },
    { D_8009CE70, 1 },
    { D_8009CE80, 1 },
    { D_8009CE90, 1 },
    { D_8009CEB0, 1 },
    { D_8009CED0, 2 },
    { D_8009CEA0, 1 },
    { D_8009CEC0, 1 },
    { D_8009CEF0, 2 },
    { D_8009CF10, 1 },
    { D_8009CF20, 1 },
    { D_8009D260, 1 },
    { D_8009CE10, 1 },
    { D_8009D270, 1 },
    { D_8009D540, 1 },
    { D_8009CF30, 1 },
    { D_8009CFB0, 1 },
    { D_8009D570, 1 },
    { D_8009D530, 1 },
    { D_8009D500, 1 },
    { D_8009D2F0, 1 },
    { D_8009D5C0, 1 },
    { D_8009D290, 1 },
    { D_8009CF40, 1 },
    { D_8009CF50, 1 },
    { D_8009D000, 1 },
    { D_8009D010, 1 },
    { D_8009D4D0, 1 },
    { D_8009D1D0, 1 },
    { D_8009D1E0, 1 },
    { D_8009CFC0, 1 },
    { D_8009D5A0, 1 },
    { D_8009D5B0, 1 },
    { D_8009D180, 1 },
    { D_8009D190, 1 },
    { D_8009D1B0, 1 },
    { D_8009D1C0, 1 },
    { D_8009D280, 1 },
    { D_8009CFF0, 1 },
    { D_8009D590, 1 },
    { D_8009D3B0, 1 },
    { D_8009D1A0, 1 },
    { D_8009D8C0, 1 },
    { D_8009D370, 1 },
    { D_8009D380, 1 },
    { D_8009D170, 1 },
    { D_8009D5D0, 1 },
    { D_8009D020, 1 },
    { D_8009D470, 1 },
    { D_8009D030, 1 },
    { D_8009D450, 2 },
    { D_8009D3C0, 1 },
    { D_8009D040, 1 },
    { D_8009D050, 1 },
    { D_8009D3E0, 1 },
    { D_8009D420, 1 },
    { D_8009D400, 1 },
    { D_8009D410, 1 },
    { D_8009D080, 1 },
    { D_8009D070, 1 },
    { D_8009D090, 1 },
    { D_8009D130, 1 },
    { D_8009D1F0, 1 },
    { D_8009D220, 1 },
    { D_8009D140, 1 },
    { D_8009D160, 1 },
    { D_8009D230, 1 },
    { D_8009D240, 1 },
    { D_8009D250, 1 },
    { D_8009D0F0, 1 },
    { D_8009D120, 1 },
    { D_8009D0A0, 1 },
    { D_8009D0C0, 1 },
    { D_8009D390, 1 },
    { D_8009D0D0, 1 },
    { D_8009D3D0, 1 },
    { D_8009D0E0, 1 },
    { D_8009D100, 1 },
    { D_8009D110, 1 },
    { D_8009D3A0, 1 },
    { D_8009D4A0, 1 },
    { D_8009D4B0, 1 },
    { D_8009D4E0, 1 },
    { D_8009D510, 1 },
    { D_8009D550, 1 },
    { D_8009D2A0, 1 },
    { D_8009D520, 1 },
    { D_8009D560, 1 },
    { D_8009D150, 1 },
    { D_8009D5E0, 1 },
    { D_8009D5F0, 1 },
    { D_8009D580, 1 },
    { D_8009D600, 1 },
    { D_8009D720, 1 },
    { D_8009D710, 1 },
    { D_8009D200, 1 },
    { D_8009D210, 1 },
    { D_8009D320, 1 },
    { D_8009D300, 1 },
    { D_8009D360, 1 },
    { D_8009D610, 1 },
    { D_8009D620, 1 },
    { D_8009D630, 1 },
    { D_8009D640, 1 },
    { D_8009D8D0, 1 },
    { D_8009D8E0, 1 },
    { D_8009D650, 1 },
    { D_8009D6E0, 1 },
    { D_8009D6F0, 1 },
    { D_8009D700, 1 },
    { D_8009D310, 1 },
    { D_8009D340, 1 },
    { D_8009D660, 1 },
    { D_8009D670, 1 },
    { D_8009D680, 1 },
    { D_8009D690, 1 },
    { D_8009D6D0, 1 },
    { D_8009D6A0, 1 },
    { D_8009D6C0, 1 },
    { D_8009D730, 1 },
    { D_8009D740, 1 },
    { D_8009D750, 1 },
    { D_8009D760, 1 },
    { D_8009D770, 1 },
    { D_8009D440, 1 },
    { D_8009CE00, 1 },
    { D_8009D8A0, 1 },
    { D_8009D780, 1 },
    { D_8009D480, 1 },
    { D_8009D8B0, 1 },
    { D_8009D0B0, 1 },
    { D_8009D790, 1 },
    { D_8009D7A0, 1 },
    { D_8009D7B0, 1 },
    { D_8009CFA0, 1 },
    { D_8009CF80, 1 },
    { D_8009D7D0, 1 },
    { D_8009D330, 1 },
    { D_8009D350, 1 },
    { D_8009CF90, 1 },
    { D_8009CFE0, 1 },
    { D_8009D430, 1 },
    { D_8009D7E0, 1 },
    { D_8009D7F0, 1 },
    { D_8009D810, 1 },
    { D_8009D830, 1 },
    { D_8009D840, 1 },
    { D_8009D860, 1 },
    { D_8009D870, 1 },
    { D_8009D880, 1 },
    { D_8009D890, 1 },
    { D_8009D850, 1 },
    { D_8009D3F0, 1 },
    { D_8009D490, 1 },
    { D_8009D4C0, 1 },
    { D_8009D6B0, 1 },
    { D_8009D800, 1 },
    { D_8009D820, 1 },
    { D_8009CF60, 1 },
    { D_8009CF70, 1 },
    { D_8009D4F0, 1 },
    { D_8009D8F0, 1 },
    { D_8009D2B0, 1 },
    { D_8009D2D0, 1 },
    { D_8009D2E0, 1 },
    { D_8009D2C0, 1 },
    { D_8009D060, 1 },
    { D_8009D7C0, 1 },
    { D_8009CFD0, 1 },
    { D_8009D900, 1 },
};

