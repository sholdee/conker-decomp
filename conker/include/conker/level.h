#ifndef CONKER_LEVEL_H
#define CONKER_LEVEL_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "level": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Level/LevelTints/leveltints.c
 *   game/Level/LevelModels/levelmodels.c
 *   game/Level/LevelObjects/levelobjects.c
 *   game/Level/ZoneSpray/zonespray.c
 *   game/Level/LevelSetupReset/levelsetupreset.c
 *   game/Level/LevelSetupBarnInside/levelsetupbarninside.c
 *   game/Level/LevelSetupBarnOutside/levelsetupbarnoutside.c
 *   game/Level/LevelSetupBarnArena/levelsetupbarnarena.c
 *   game/Level/LevelSetupBrontoCavern/levelsetupbrontocavern.c
 *   game/Level/LevelSetupBrontoThroat/levelsetupbrontothroat.c
 *   game/Level/LevelSetupDogFish/levelsetupdogfish.c
 *   game/Level/LevelSetupCogLand/levelsetupcogland.c
 *   game/Level/LevelSetupDinoArena/levelsetupdinoarena.c
 *   game/Level/LevelSetupPooCabin/levelsetuppoocabin.c
 *   game/Level/LevelSetupW1VolcanoRace/levelsetupw1volcanorace.c
 *   game/Level/LevelSetupRockSolid/levelsetuprocksolid.c
 *   game/Level/LevelSetup30/levelsetup30.c
 *   game/Level/LevelSetupHarbour/levelsetupharbour.c
 *   game/Level/LevelSetupBoilerRoom/levelsetupboilerroom.c
 *   game/Level/LevelSetupOutsideMolehole/levelsetupoutsidemolehole.c
 *   game/Level/LevelSetup07/levelsetup07.c
 *   game/Level/LevelSetupTankFactory/levelsetuptankfactory.c
 *   game/Level/ZoneShapeEmitter/zoneshapeemitter.c
 *   game/Level/LevelSetup18/levelsetup18.c
 *   game/Level/LevelSetupLab/levelsetuplab.c
 *   game/Level/LevelSetupW3HauntedHouse/levelsetupw3hauntedhouse.c
 *   game/Level/LevelSetupHauntedOuthouse/levelsetuphauntedouthouse.c
 *   game/Level/LevelSetupHorrorIntro/levelsetuphorrorintro.c
 *   game/Level/LevelSetupLiftTunnel/levelsetuplifttunnel.c
 *   game/Level/LevelSetupBankEntrance/levelsetupbankentrance.c
 *   game/Level/LevelSetupThroneRoom/levelsetupthroneroom.c
 *   game/Level/LevelSetupFrontEnd/levelsetupfrontend.c
 *   game/Level/LevelSetupHeist/levelsetupheist.c
 *   game/Level/LevelSetup25/levelsetup25.c
 *   game/Level/LevelSetupMainWar/levelsetupmainwar.c
 *   game/Level/AmbientAction3/ambientaction3.c
 *   game/Level/AmbientAction5/ambientaction5.c
 *   game/Level/ZoneAreaEmitter/zoneareaemitter.c
 *   game/Level/AmbientColourCycle/ambientcolourcycle.c
 *   game/Level/GroundParticles/groundparticles.c
 *   game/Level/EffectKind/effectkind.c
 *   game/Level/SetupResetStub/setupresetstub.c
 *   game/Level/WaveReset/wavereset.c
 *   game/Level/ZoneSpawn/zonespawn.c
 *   game/Level/SpawnTable/spawntable.c
 *   game/Level/LevelHookUBend/levelhookubend.c
 *   game/Level/ObjectTables/objecttables.c (data only, linked after game/Prop/Clock/clock)
 *   game_data_800D9920.c (data only, owner unknown; linked after game/Effects/TintedObject/tintedobject)
 *   game/Level/LevelHooks/levelhooks.c
 *   game/Level/HeightCheck/heightcheck.c
 */

/* A display list command, viewed as its opcode, the G_MOVEMEM index byte and the address word. */
typedef struct {
    /* 0x0 */ s8 opcode;
    /* 0x1 */ u8 pad1[2];
    /* 0x3 */ u8 index;
    /* 0x4 */ u32 addr;
} DlCommand;

/* game/Level/LevelObjects/levelobjects.c */
typedef void (*ObjectFunc)();

/* Defaults for objects of the second (grouped) list, indexed by model id (gGroupedObjectDefaults). */
typedef struct {
    /* 0x0 */ u8 hasShadow;
    /* 0x1 */ u8 shadowSize;
    /* 0x2 */ s16 cullRadiusY;
    /* 0x4 */ s16 cullRadiusXZ;
    /* 0x6 */ u16 drawDistance;
    /* 0x8 */ u8 flags;
    /* 0x9 */ u8 state;
    /* 0xA */ u16 effectFlags;
} GroupedObjectDefaults;

/* A level object placement as stored in the level's object lists (0x44 bytes each). */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 rotX;
    /* 0x08 */ s16 rotY;
    /* 0x0A */ s16 rotZ;
    /* 0x0C */ Gfx *displayList; // NULL: load model `modelId`; 1 or 2: use level model `modelId`
    /* 0x10 */ s32 modelId;
    /* 0x14 */ union {
        u32 index;         // into gObjectBehaviors when below 0xFFFF
        ObjectFunc func;   // otherwise the update function itself
    } behavior;
    /* 0x18 */ void *data;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ f32 scaleX;
    /* 0x24 */ f32 scaleY;
    /* 0x28 */ f32 scaleZ;
    /* 0x2C */ u8 hasShadow;
    /* 0x2D */ u8 shadowSize;
    /* 0x2E */ u16 cullRadiusXZ;
    /* 0x30 */ u16 cullRadiusY;
    /* 0x32 */ u8 flags;
    /* 0x33 */ u8 id;            // 0: assign a free one
    /* 0x34 */ u8 inactive;
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ s32 material;
    /* 0x3C */ u8 state;
    /* 0x3D */ u8 pad3D;
    /* 0x3E */ u16 effectFlags;
    /* 0x40 */ u16 drawDistance;
    /* 0x42 */ u8 pad42[2];
} ObjectSpawn; // size 0x44

/* game/Level/LevelTints/leveltints.c */
/* (s32 arg0); declared without a prototype: game/Audio/LevelMusic/levelmusic.c calls it without the (unused) argument */
void level_initTintWaves();

/* game/Level/LevelModels/levelmodels.c */
extern s32 *gLevelAssetLists[]; // per level (LevelHeader.assetList): zero-terminated asset id list. The segment
                          // starts here; init/Boot/BootMain/bootmain first DMAs its 16-byte header in (word 0: compressed data offset).
extern const u16 gTextureRomSizes[7762]; // per texture: compressed size
extern Gfx *gLevelDisplayLists[4]; // level display lists
extern Vtx *gLevelVertices[4]; // level mesh vertices of the four level models (each display list follows its vertices)
extern VtxColorGroup **gVtxColorGroups; // groups of each vertex color set
extern u8 *gVtxColorGroupCounts; // group count of each vertex color set
extern u8 gVtxColorSetCount;
extern s32 gLevelVertexCounts[4];
extern LevelModelEntry *gLevelModels; // the current level's models
extern s32 gTextureAddrs[7762]; // per texture: its address once loaded, or -1
extern u16 gTextureSizes[7762]; // per texture: size
extern s8 gTextureLifetimes[7762]; // per texture: frames left before it is freed (0 = not loaded)
void level_rebaseVtxColorSet(s32 index, s32 base, s32 list);
void level_loadModels(s32 levelId);
s32 level_resetTextureTable(void);
void level_readTextureSizes(void);
void level_buildCollisionTris(s32 unused);
void level_buildCollisionQuadTrees(void);
s32 level_findTextureByAddr(s32 addr);
void func_150039B0(s32 levelId);
void level_setMeshTriMaterials(s32 addr);

/* game/Level/LevelObjects/levelobjects.c */
void level_spawnObjects(s32 levelId, s32 arg1);
void func_15004574(void);
void func_150045BC(void);
void level_prepareObjects(s32 levelId);
void level_relocateDl(DlCommand *dl, s32 offset, s32 matrixOffset);
void level_clearObjectBlocks(void);
void level_setObjectCullRadius(WorldObject *obj, ObjectSpawn *spawn);
s32 level_findFreeObjectId(s32 fromTop);
void level_rebaseDlMatrices(DlCommand *dl, u32 base);

/* game/Level/ZoneSpray/zonespray.c */
s32 level_spawnZoneSpray(TriggerZone *zone);

/* game/Level/LevelSetupReset/levelsetupreset.c */
void level_resetSetupState(void);
void func_1500E70C(s32 id);
void level_setEffectDetail(void);
void level_setupTraining(void);
void level_setup01(void);
void level_setupBeeCavern(void);

/* game/Level/LevelSetupBarnInside/levelsetupbarninside.c */
void level_emptySetupBarnInside(void);

/* game/Level/LevelSetupBarnOutside/levelsetupbarnoutside.c */
void level_queueObjectDropBarnOutside(s32 x, s32 y, s32 z, s32 floorY, s32 markOnLanding);
void level_setupBarnOutside(void);

/* game/Level/LevelSetupBarnArena/levelsetupbarnarena.c */
void level_setupBarnArena(void);
void level_spawnObjBurst(Actor *obj, s32 owner, s32 list);
void level_spawnBoltRingZone(TriggerZone *zone);

/* game/Level/LevelSetupBrontoCavern/levelsetupbrontocavern.c */
void level_setupBrontoCavern(void);

/* game/Level/LevelSetupBrontoThroat/levelsetupbrontothroat.c */
void level_setupBrontoThroat(void);

/* game/Level/LevelSetupDogFish/levelsetupdogfish.c */
void *level_spawnFlatSprite(f32 x, f32 y, f32 z);
void level_queueObjectDropDogFish(s32 x, s32 y, s32 z, s32 floorY);
void level_setupDogFish(void);

/* game/Level/LevelSetupCogLand/levelsetupcogland.c */
void level_queueObjectDropCogLand(s32 x, s32 y, s32 z, s32 floorY, s32 markOnLanding);
void level_setupCogLand(void);

/* game/Level/LevelSetupDinoArena/levelsetupdinoarena.c */
extern BounceRecord *gBounceRecords; /* ten bouncing cursors (level_initBounceRecords) */
void level_initBounceRecords(void);
void level_addUvWobbleDinoArena(void);
void level_setupDinoArena(void);

/* game/Level/LevelSetupPooCabin/levelsetuppoocabin.c */
void level_setupPooCabin(void);

/* game/Level/LevelSetupW1VolcanoRace/levelsetupw1volcanorace.c */
void level_setupW1VolcanoRace(void);

/* game/Level/LevelSetupRockSolid/levelsetuprocksolid.c */
void level_emptySetupRockSolid(void);

/* game/Level/LevelSetup30/levelsetup30.c */
void level_setupPooBoss(void);
void level_setupUBend(void);

/* game/Level/LevelSetupHarbour/levelsetupharbour.c */
void level_setupHarbour(void);
void level_spawnBoltEmitter(Actor *obj);

/* game/Level/LevelSetupBoilerRoom/levelsetupboilerroom.c */
void level_setupBoilerRoom(void);

/* game/Level/LevelSetupOutsideMolehole/levelsetupoutsidemolehole.c */
void level_setupOutsideMolehole(void);

/* game/Level/LevelSetup07/levelsetup07.c */
void level_emptySetup07(void);

/* game/Level/LevelSetupTankFactory/levelsetuptankfactory.c */
void level_setupTankFactory(void);

/* game/Level/ZoneShapeEmitter/zoneshapeemitter.c */
s32 level_spawnShapeEmitterZone(TriggerZone *zone);

/* game/Level/LevelSetup18/levelsetup18.c */
void level_setupWarboss(void);
void level_setupCanyon(void);
void level_setupMachineGunNest(void);

/* game/Level/LevelSetupLab/levelsetuplab.c */
void level_setupLab(void);

/* game/Level/LevelSetupW3HauntedHouse/levelsetupw3hauntedhouse.c */
extern struct TexScroll **gSetup33TexScrolls; /* four texture scrolls (level_setupW3HauntedHouse) */
void level_setupW3HauntedHouse(void);

/* game/Level/LevelSetupHauntedOuthouse/levelsetuphauntedouthouse.c */
void level_setupHauntedOuthouse(void);

/* game/Level/LevelSetupHorrorIntro/levelsetuphorrorintro.c */
void level_setupHorrorIntro(void);

/* game/Level/LevelSetupLiftTunnel/levelsetuplifttunnel.c */
void level_setupLiftTunnel(void);

/* game/Level/LevelSetupBankEntrance/levelsetupbankentrance.c */
void level_setupBankEntrance(void);

/* game/Level/LevelSetupThroneRoom/levelsetupthroneroom.c */
void level_setupThroneRoom(void);
void level_spawnModelBeam(Actor *owner, u8 fxOwner, s32 list);
void level_spawnFlashBurst(Actor *owner, u8 fxOwner, s32 list);

/* game/Level/LevelSetupFrontEnd/levelsetupfrontend.c */
void level_queueObjectDropFrontEnd(s32 x, s32 y, s32 z, s32 floorY);
void level_setupFrontEnd(void);

/* game/Level/LevelSetupHeist/levelsetupheist.c */
extern u8 gFlagLightStates[5]; /* five level flags, cleared by level_setupHeist */
void level_setupHeist(void);

/* game/Level/LevelSetup25/levelsetup25.c */
void level_setup28(void);
void level_setup29(void);
void level_setupWmpVolcanoRace(void);

/* game/Level/LevelSetupMainWar/levelsetupmainwar.c */
void level_setupMainWar(void);
s32 level_spawnStageZone(TriggerZone *zone);

/* game/Level/AmbientAction3/ambientaction3.c */
void level_startAmbientRedDrift(s32 unused);

/* game/Level/AmbientAction5/ambientaction5.c */
void level_startAmbientPulse(s32 unused);

/* game/Level/ZoneAreaEmitter/zoneareaemitter.c */
void level_spawnZoneAreaEmitter(s32 zone, s32 kind);

/* game/Level/AmbientColourCycle/ambientcolourcycle.c */
void level_startAmbientColourCycle(s32 unused);
void level_attachDripEmitter(Actor *obj);

/* game/Level/GroundParticles/groundparticles.c */
void level_spawnLightAlphaModel(LightEffect *effect);

/* game/Level/EffectKind/effectkind.c */
u8 level_getEffectKind(void);

/* game/Level/SetupResetStub/setupresetstub.c */
void func_15012770(void);

/* game/Level/WaveReset/wavereset.c */
void level_resetWaveDeformers(void);

/* game/Level/ZoneSpawn/zonespawn.c */
void level_resetScreenCopy(void);
void level_spawnZones(void);
s32 level_spawnPointGlowZone(TriggerZone *obj);
s32 level_spawnEffectZone(TriggerZone *obj);
s32 level_spawnTrailZone(TriggerZone *obj);
s32 level_spawnZoneBeam(TriggerZone *obj);
s32 level_spawnNeutralZone(TriggerZone *obj);
s32 level_spawnZoneAreaEffect(TriggerZone *obj, s32 index);
s32 level_spawnAreaParticlesZone(TriggerZone *obj);
s32 func_15013D38(TriggerZone *obj);
s32 level_spawnChannelParticlesZone(TriggerZone *obj);
s32 level_spawnStreamZone(TriggerZone *obj);
s32 level_spawnHandlerZone(TriggerZone *obj);
s32 level_registerFlamethrowerZone(TriggerZone *obj);
s32 level_registerEmissionSourceZone(TriggerZone *obj);
s32 func_1501407C(TriggerZone *obj);
s32 level_spawnRandomPointZone(TriggerZone *obj);
s32 level_spawnFireZone(TriggerZone *obj);
s32 level_spawnAreaEmitterZone(TriggerZone *obj);
s32 level_registerSprayAreaZone(TriggerZone *obj);
s32 level_spawnFlackZone(TriggerZone *obj);
s32 level_spawnBulletZone(TriggerZone *obj);
s32 level_spawnMissileZone(TriggerZone *obj);
s32 level_spawnDiscoBallZone(TriggerZone *obj);
s32 level_spawnLaserTubeZone(TriggerZone *obj);
s32 level_addDustRegionZone(void);
s32 level_spawnZoneEffect(TriggerZone *obj);
s32 level_spawnBoltRainZone(TriggerZone *obj);
s32 level_spawnBoltZone(TriggerZone *obj);
void level_spawnBoltArcZone(TriggerZone *obj);
s32 level_spawnSparkRegionZone(TriggerZone *obj);
s32 level_spawnRopeZone(TriggerZone *obj);
extern const s32 gPointGlowTextures[5]; // defined after level_spawnPointGlowZone, which reads them: their .rodata follows its const local
extern const s32 gPointGlowRenderModes[5];
extern const s32 gPointGlowEmitterTypes[8];
extern u16 *gScreenCopy; // RGBA16 screen copy, gScreenWidth x gScreenHeight (game/Screen/FrameImage/frameimage)
extern u8 gScreenCopyAlpha;
extern u8 gFrameEchoPass;

/* game/Level/SpawnTable/spawntable.c */
void level_collectSpawnModelTypes(void);
void level_loadSpawnTable(s32 levelId);
void level_allocSpawnPaths(s32 levelId);
extern u8 gSpawnModelTypeCount;      // number of distinct spawn model types in gSpawnModelTypes
extern u16 gSpawnModelTypes[8];  // the distinct spawn model types (< 0xBB) of the level
extern s32 *gSpawnModelData[8]; // per spawn model type: data loaded by rom_loadPathTo

/* game/Level/LevelHookUBend/levelhookubend.c */
void level_hookUBend(Camera *camera);
void level_updateHazardProps(s32 unused);
void level_spawnBloodStrip(Actor *obj, u8 mirrored);
s32 level_sampleBloodStripPower(BloodStrip *strip);
void level_emitBloodStripParticle(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip);

/* game/Level/ObjectTables/objecttables.c (data only, linked after game/Prop/Clock/clock) */
extern ObjectBehavior gObjectBehaviors[89];               // level-object behaviour handlers
extern const GroupedObjectDefaults gGroupedObjectDefaults[114];

/* game_data_800D9920.c (data only, owner unknown; linked after game/Effects/TintedObject/tintedobject) */
extern u8 D_800D9920;
extern u8 D_800D9921;
extern u8 D_800D9928[14];
extern u8 D_800D9938[14];

/* game/Level/LevelHooks/levelhooks.c */
void level_runHook(void *camera, s32 levelId);

/* game/Level/HeightCheck/heightcheck.c */
s32 level_isCameraAboveLimit(s32 levelId);

/* game/Level/LevelHeader/levelheader.c */
extern LevelHeader *gLevelHeader; // game/Level/LevelHeader/levelheader.c

#endif
