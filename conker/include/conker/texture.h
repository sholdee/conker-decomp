#ifndef CONKER_TEXTURE_H
#define CONKER_TEXTURE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "texture": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Texture/UvWobble/uvwobble.c
 *   game/Texture/TextureLoad/textureload.c
 *   game/Texture/TextureRelease/texturerelease.c
 *   game/Texture/TextureUse/textureuse.c
 *   game/Texture/TextureTables/texturetables.c
 */

/* game/Texture/UvWobble/uvwobble.c */
/* Scrolls the texture coordinates of one mesh's vertices along a circle. */
typedef struct UvWobble {
    /* 0x00 */ TexCoord *primary;   /* original UVs of the vertices in mesh 0 */
    /* 0x04 */ TexCoord *secondary; /* original UVs of the vertices in mesh 1 */
    /* 0x08 */ u8 primaryCount;
    /* 0x09 */ u8 secondaryCount;
    /* 0x0A */ u8 primaryIndex;     /* first vertex in gLevelVertices[0] */
    /* 0x0B */ u8 secondaryIndex;   /* first vertex in gLevelVertices[1] */
    /* 0x0C */ u8 disabled;
    /* 0x0D */ u8 pad0D;
    /* 0x0E */ u16 phase;
    /* 0x10 */ u16 rate;
    /* 0x12 */ u16 amplitude;
} UvWobble;

extern u8 gUvWobbleCount;

s32 texture_findTexVerts(Gfx *dl, s32 tex, Vtx **outVtx, s32 skip);
UvWobble *texture_addUvWobble(s32 tex, s32 disabled, s32 rate, s32 amplitude);
void texture_updateUvWobbles(void);

/* game/Texture/TextureLoad/textureload.c */
extern u16 gTintWavePhases[4][3];
extern u8 gTintWaveSteps[4][3];
extern u8 gTintWaveAmps[4][3];
extern u8 gTintLevels[4][3];
extern u8 gTintDefaultTargets[3];
extern u8 gTintTargetOverrides[4][3];
extern const u8 gTintPresets[7][3];
extern const u8 gTintPresetSteps[1][3];
extern const u8 gTintPresetAmps[7][3];
extern const u8 gTintPresetTargets[7][3];
void texture_updateTint(s32 slot);
Gfx *texture_setTintColors(Gfx *gfx, s32 primAlpha, s32 envAlpha, s32 slot);
s32 texture_resolveDl(Gfx *dl, s32 strict, u32 countUse, s32 lifetime, s16 **usedOut);
s32 texture_getAddress(s32 index, s32 *sizeOut, s32 lifetime, s32 countUse); /* texture address of a texture id */
s32 texture_getRomAddress(s32 index);
void texture_updateAging(void);
void texture_setLifetime(s32 index, s32 lifetime);

/* game/Texture/TextureRelease/texturerelease.c */
/* list: a count-prefixed array of s16 texture ids (built by texture_resolveDl), freed after it is released. */
void texture_releaseList(s16 *list);
void texture_releaseId(s32 id);

/* game/Texture/TextureUse/textureuse.c */
extern s32 gTexAgeFirst;
extern s32 gTexAgeLast;
extern u8 gTexAgeThrottle;
extern u8 gTexUseCounts[7762];
extern u8 gTexLoadCooldown;
extern s32 gTexLastFreed;
void texture_dropUse(s32 index);
void texture_dropUseAndFree(u32 index);
void texture_clearBindings(void);
void texture_addBinding(void *object, s32 texture, s32 palette, s32 textureSegment, s32 paletteSegment);
Gfx *texture_emitBindings(Gfx *gfx, void *object);

/* game/Texture/TextureTables/texturetables.c: the shared texture tables (a data-only unit). The TextureDesc entries are the ones the
 * texture table gEffectAnimTextures (game/Effects/ObjectList/objectlist.c) and the code use by name; D_80090150, D_8009016C, D_80090184
 * and D_800901C4 are the asset id lists of gLevelAssetLists (game/Level/LevelModels/levelmodels.c); D_80090230 and D_80090244 are the
 * two frame tables of gBounceRecordFrameTables and gTexAnimFrameTables. */
extern s32 D_80090150[7];
extern s32 D_8009016C[6];
extern s32 D_80090184[16];
extern s32 D_800901C4[11];
extern s32 D_80090204[2][2];
extern s32 D_80090214[5];
extern s32 D_80090228;
extern s32 D_8009022C;
extern s32 D_80090230[5];
extern s32 D_80090244[5];
extern s32 D_80090258;
extern s32 D_80090260;
extern s32 D_80090264;
extern s32 D_8009026C;
extern s32 D_80090270;
extern s32 D_80090274[9];
extern s32 D_80090298[8]; /* texture ids */
extern s32 D_800902B8;
extern s32 D_800902BC[3];
extern s32 gBarTopEdgeTexId; /* texture id: top bar edge */
extern s32 gBarBottomEdgeTexId; /* texture id: bottom bar edge */
extern s32 D_800902D0;
extern s32 D_800902D4;
extern s32 D_800902D8;
extern s32 D_800902DC;
extern s32 D_800902E4;
extern s32 D_800902E8;
extern s32 D_800902EC;
extern s32 D_800902FC[5];
extern s32 D_80090310;
extern s32 D_80090314;
extern s32 D_80090318;
extern s32 D_8009031C;
extern s32 D_80090320;
extern s32 D_80090324[5];
extern s32 D_80090338[8];
extern TextureDesc D_80090388;
extern Texture gFlareTexture; /* flare sprite texture */
extern TextureDesc D_800903BC;
extern TextureDesc D_800903C8;
extern TextureDesc D_800903D4;
extern TextureDesc D_800903F4;
extern TextureDesc D_80090414;
extern TextureDesc D_80090420;
extern TextureDesc D_8009042C;
extern TextureDesc D_80090438;
extern TextureDesc D_80090444;
extern TextureDesc D_80090450;
extern TextureDesc D_8009045C;
extern TextureDesc D_80090468;
extern TextureDesc D_80090474;
extern TextureDesc D_80090494;
extern TextureDesc D_800904B4;
extern TextureDesc D_800904C0;
extern TextureDesc D_800904CC;
extern TextureDesc D_800904D8;
extern TextureDesc D_800904E4;
extern TextureDesc D_800904F0;
extern TextureDesc D_800904FC;
extern TextureDesc D_80090508;
extern TextureDesc D_80090514;
extern TextureDesc D_80090520;
extern TextureDesc D_8009052C;
extern TextureDesc D_8009053C;
extern TextureDesc gPuffQuadTexture;
extern TextureDesc gTextBoxTexture;
extern TextureDesc gTextBubbleTexture;
extern TextureDesc gTextBoxTailTexture;
extern TextureDesc gTextBubbleCircleTexture;
extern TextureDesc gTextBoxAltTailTexture;
extern TextureDesc D_800905A8;
extern TextureDesc D_800905B4;
extern TextureDesc D_800905C0;
extern TextureDesc D_800905CC;
extern TextureDesc D_800905D8;
extern TextureDesc D_800905E4;
extern TextureDesc D_800905F0;
extern TextureDesc D_80090600;
extern TextureDesc gDebrisTexture;
extern TextureDesc D_80090634;
extern TextureDesc D_80090660;
extern TextureDesc D_8009066C;
extern TextureDesc D_80090678;
extern TextureDesc D_80090684;
extern TextureDesc D_80090690;
extern TextureDesc D_800906C4;
extern TextureDesc D_800906F8;
extern TextureDesc D_80090704;
extern TextureDesc D_80090710;
extern TextureDesc D_80090730;
extern TextureDesc D_8009075C;
extern TextureDesc D_8009077C;
extern TextureDesc D_8009078C;
extern TextureDesc D_80090798;
extern TextureDesc D_800907EC;
extern TextureDesc D_800907FC;
extern TextureDesc D_8009080C;
extern TextureDesc gEffectTextures[207]; /* the effect texture table (the `texture` index of the effect descriptors) */
extern Texture D_80091514;
extern s32 D_80091520[1];
extern s32 D_80091524[1];
extern s32 D_80091528[1];
extern s32 D_8009152C[1];
extern s32 D_80091530[1];
extern s32 D_80091534[1];
extern s32 D_80091538[1];
extern s32 D_8009153C[5];
extern s32 D_80091550[5];
extern s32 D_80091564[16];
extern Texture gTrailTexture; /* addr is set to the trail texture before each draw */
extern Texture D_800915B0;
extern s32 D_800915BC[5];
extern s32 D_800915D0[1];
extern s32 D_800915D4[1];
extern TextureDesc D_800915DC;
extern TextureDesc D_800915EC;
extern TextureDesc D_800915FC;
extern TextureDesc D_8009160C;
extern TextureDesc D_8009161C;
extern TextureDesc D_8009162C;
extern TextureDesc D_8009163C;
extern TextureDesc D_8009164C;
extern TextureDesc D_8009165C;
extern TextureDesc D_80091690;
extern TextureDesc D_800916AC;
extern TextureDesc D_800916C8;
extern TextureDesc D_800916E4;
extern TextureDesc D_80091700;
extern TextureDesc D_80091710;
extern TextureDesc D_80091720;
extern TextureDesc D_80091730;
extern TextureDesc D_80091740;
extern TextureDesc D_80091750;
extern TextureDesc D_80091764;
extern s32 gBlobShadowTextures[3];
extern s32 D_8009177C[15];
extern s32 D_800917B8;
extern s32 D_800917D8[3];
extern s32 D_800917E4[1];
extern s32 D_800917E8[1];
extern HudSprite D_800917EC;
extern HudSprite D_800917F8;
extern HudSprite D_80091804;
extern HudSprite D_80091810;
extern HudSprite D_8009181C;
extern HudSprite D_80091828;
extern TextureDesc D_80091834;
extern TextureDesc D_80091840;
extern TextureDesc D_8009184C;
extern TextureDesc D_80091858;
extern TextureDesc D_80091864;
extern TextureDesc D_80091870;
extern TextureDesc D_8009187C;
extern TextureDesc D_80091888;
extern TextureDesc D_80091894;
extern TextureDesc D_800918A0;
extern TextureDesc D_800918AC;
extern TextureDesc D_800918B8;
extern TextureDesc D_800918C4;
extern TextureDesc D_800918D0;
extern TextureDesc D_800918DC;
extern TextureDesc D_800918E8;
extern TextureDesc D_800918F4;
extern TextureDesc D_80091900;
extern TextureDesc D_8009190C;
extern TextureDesc D_80091918;
extern TextureDesc D_80091924;
extern TextureDesc D_80091930;
extern TextureDesc D_8009193C;
extern TextureDesc D_80091948;

#endif
