#ifndef CONKER_LEVEL_H
#define CONKER_LEVEL_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "level": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Level/LevelTints/leveltints.c
 *   game/Level/LevelModels/levelmodels.c
 *   game/Level/LevelObjects/levelobjects.c
 *   game/Level/ZoneSpray/zonespray.c
 *   game/Level/LevelSetupReset/levelsetupreset.c
 *   game/Level/LevelSetup03/levelsetup03.c
 *   game/Level/LevelSetup04/levelsetup04.c
 *   game/Level/LevelSetup09/levelsetup09.c
 *   game/Level/LevelSetup05/levelsetup05.c
 *   game/Level/LevelSetup34/levelsetup34.c
 *   game/Level/LevelSetup08/levelsetup08.c
 *   game/Level/LevelSetup11/levelsetup11.c
 *   game/Level/LevelSetup12/levelsetup12.c
 *   game/Level/LevelSetup19/levelsetup19.c
 *   game/Level/LevelSetup24/levelsetup24.c
 *   game/Level/LevelSetup27/levelsetup27.c
 *   game/Level/LevelSetup30/levelsetup30.c
 *   game/Level/LevelSetup02/levelsetup02.c
 *   game/Level/LevelSetup06/levelsetup06.c
 *   game/Level/LevelSetup07/levelsetup07.c
 *   game/Level/LevelSetup10/levelsetup10.c
 *   game/Level/ZoneShapeEmitter/zoneshapeemitter.c
 *   game/Level/LevelSetup18/levelsetup18.c
 *   game/Level/LevelSetup23/levelsetup23.c
 *   game/Level/LevelSetup33/levelsetup33.c
 *   game/Level/LevelSetup35/levelsetup35.c
 *   game/Level/LevelSetup37/levelsetup37.c
 *   game/Level/LevelSetup16/levelsetup16.c
 *   game/Level/LevelSetup31/levelsetup31.c
 *   game/Level/LevelSetup13/levelsetup13.c
 *   game/Level/LevelSetup17/levelsetup17.c
 *   game/Level/LevelSetup14/levelsetup14.c
 *   game/Level/LevelSetup25/levelsetup25.c
 *   game/Level/LevelSetup26/levelsetup26.c
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
 *   game/Level/LevelHook30/levelhook30.c
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
    /* 0x0 */ u8 unk4C;
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
    /* 0x2C */ u8 unk4C;
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
void func_150039B0(s32 arg0);
void level_setMeshTriMaterials(s32 addr);

/* game/Level/LevelObjects/levelobjects.c */
void level_spawnObjects(s32 levelId, s32 arg1);
void func_15004574(void);
void func_150045BC(void);
void level_prepareObjects(s32 arg0);
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
void level_setup21(void);
void level_setup01(void);
void level_setup36(void);

/* game_3BFD0.c */
void level_emptySetup03(void);

/* game_3BFE0.c */
void level_queueObjectDrop04(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void level_setup04(void);

/* game_3C230.c */
void level_setup09(void);
void level_spawnObjBurst(Actor *arg0, s32 arg1, s32 arg2);
void level_spawnBoltRingZone(TriggerZone *zone);

/* game_3C3D0.c */
void level_setup05(void);

/* game_3C6C0.c */
void level_setup34(void);

/* game/Level/LevelSetup08/levelsetup08.c */
void *level_spawnFlatSprite(f32 x, f32 y, f32 z);
void level_queueObjectDrop08(s32 x, s32 y, s32 z, s32 floorY);
void level_setup08(void);

/* game/Level/LevelSetup11/levelsetup11.c */
void level_queueObjectDrop11(s32 x, s32 y, s32 z, s32 floorY, s32 arg4);
void level_setup11(void);

/* game/Level/LevelSetup12/levelsetup12.c */
extern BounceRecord *gBounceRecords; /* ten bouncing cursors (level_initBounceRecords) */
void level_initBounceRecords(void);
void func_1500FF9C(void);
void level_setup12(void);

/* game/Level/LevelSetup19/levelsetup19.c */
void level_setup19(void);

/* game_3D6F0.c */
void level_setup24(void);

/* game_3D770.c */
void level_emptySetup27(void);

/* game_3D780.c */
void level_setup30(void);
void level_setup32(void);

/* game/Level/LevelSetup02/levelsetup02.c */
void level_setup15(void);
void level_spawnBoltEmitter(Actor *arg0);
void level_setup02(void);

/* game_3DB30.c */
void level_setup06(void);

/* game_3DB50.c */
void level_emptySetup07(void);

/* game_3DB60.c */
void level_setup10(void);

/* game_3DB80.c */
s32 level_spawnShapeEmitterZone(TriggerZone *zone);

/* game/Level/LevelSetup18/levelsetup18.c */
void level_setup18(void);
void level_setup20(void);
void level_setup22(void);

/* game/Level/LevelSetup23/levelsetup23.c */
void level_setup23(void);

/* game_3E460.c */
extern struct TexScroll **gSetup33TexScrolls; /* four texture scrolls (level_setup33) */
void level_setup33(void);

/* game_3E5A0.c */
void level_setup35(void);

/* game_3E620.c */
void level_setup37(void);

/* game_3E7E0.c */
void level_setup16(void);

/* game_3E810.c */
void level_setup31(void);

/* game/Level/LevelSetup13/levelsetup13.c */
void level_setup13(void);
void func_1501175C(Actor *owner, u8 arg1, s32 arg2);
void level_spawnFlashBurst(Actor *owner, u8 arg1, s32 arg2);

/* game_3EFB0.c */
void level_queueObjectDrop17(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void level_setup17(void);

/* game_3F0F0.c */
extern u8 gFlagLightStates[5]; /* five level flags, cleared by level_setup14 */
void level_setup14(void);

/* game_3F120.c */
void level_setup28(void);
void level_setup29(void);
void level_setup25(void);

/* game/Level/LevelSetup26/levelsetup26.c */
void level_setup26(void);
s32 level_spawnStageZone(TriggerZone *zone);

/* game_3F3D0.c */
void level_startAmbientRedDrift(s32 arg0);

/* game_3F450.c */
void level_startAmbientPulse(s32 arg0);

/* game_3F4D0.c */
void level_spawnZoneAreaEmitter(s32 arg0, s32 arg1);

/* game_3F560.c */
void level_startAmbientColourCycle(s32 arg0);
void level_attachDripEmitter(Actor *arg0);

/* game/Level/GroundParticles/groundparticles.c */
void func_150121C0(LightEffect *effect);

/* game_3FBD0.c */
u8 level_getEffectKind(void);

/* game_3FC20.c */
void func_15012770(void);

/* game_3FC30.c */
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
s32 func_15015644(TriggerZone *obj);
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

/* game/Level/LevelHook30/levelhook30.c */
void level_hook30(Camera *camera);
void level_updateHazardProps(s32 arg0);
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
void level_runHook(void *arg0, s32 arg1);

/* game_1A6300.c */
s32 level_isCameraAboveLimit(s32 arg0);

#endif
