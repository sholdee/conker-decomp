#ifndef CONKER_COLLISION_H
#define CONKER_COLLISION_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "collision": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_2DF70.c
 *   game_43880.c
 *   game_71820.c
 *   game_AE1D0.c
 *   game_AEB40.c
 *   game_AFA70.c
 *   game_D0960.c
 *   game_D0F20.s .. game_DB730.s (hand-written asm)
 *   game_DB750.c
 *   game_10EA20.c
 *   game_131A90.c
 *   game_13BB20.c
 */

/* Types that these files define; the declarations below use them. */
typedef struct QuadNode QuadNode;
typedef struct TriggerVolume TriggerVolume;
struct Foo150AE280;
struct ShadowVtx;
struct Foo150AE36C;

/* game_2DF70.c */
void func_15000AC0(void);
void func_15000AD0(s32 arg0);
void func_15001460(Gfx *dl);
void func_15001970(void);
void func_15001A08(void);
void func_15001B08(void);
QuadNode *func_15001B10(void);
void func_15001B5C(u8 value);
void func_15001B8C(u16 value);
void func_15001BC8(s32 map, s16 *nodeCount, s16 *leafCount);
void func_15001CEC(s32 map);
QuadNode *func_15001DE0(s32 x, s32 z, s32 halfX, s32 halfZ, s32 arg4, u16 *triList);
void func_15002008(s32 minX, s32 maxX, s32 minZ, s32 maxZ, u16 *triList);
int func_15002248(s32 minX, s32 maxX, s32 minZ, s32 maxZ, s32 *minY, s32 *maxY, u16 *triList, u16 **subList);
void func_15002560(QuadNode *node, QuadNode *parentNext);
void func_150025FC(void);
void func_150026C4(s32 map);
void func_150026E8(s32 map);
void func_15002724(s32 dl);
void func_15002754(void);
s32 func_150027F8(Gfx *dl);
s32 func_15002878(void);
s32 func_150028BC(s32 map);
void func_15002F40(s32 map);
s32 func_15002FA0(s32 value);
void func_15002FB4(s32 map);
void func_1500310C(void);

/* game_43880.c */
void func_150163D0(s32 arg0);

/* game_71820.c */
void func_15044370(void);
s32 func_15044380(f32 x, f32 y, f32 z, Actor *obj, s32 arg4, s32 secondPass);
void func_1504452C(s16 **vertices, Vec3f *out, f32 sinY, f32 cosY, f32 ox, f32 oy, f32 oz, s32 offset);
void func_15044658(void);
void func_15044660(Actor *obj, f32 x, f32 y, f32 z);
TriggerVolume *func_150448D0(s32 lifetime, s32 delay, s32 action, s32 depth, s32 width, s32 height, s32 angle,
                             s16 *position, s16 *size);
TriggerNode *func_15044964(s32 size, s32 check, s32 lifetime, s32 delay, s32 action, s32 x, s32 y, s32 z);
void func_15044A28(void);
s32 func_15044B78(TriggerVolume *volume);
s32 func_15044CE4(TriggerVolume *volume);
s32 func_15044D40(TriggerVolume *volume);
void func_15044DA0(void);
void func_15044DE8(void);
void func_15044E40(void);
void func_15044E88(void);
s32 func_15044ED0(Vec3f *pos, f32 top, CollisionHit *hit);
s32 func_150450CC(Vec3f *pos, f32 bottom, CollisionHit *hit);
s32 func_1504530C(Vec3f *pos, f32 top, CollisionHit *hit);
s32 func_15045384(Vec3f *pos, f32 top, CollisionHit *hit);
s32 func_1504554C(Vec3f *pos, f32 bottom, CollisionHit *hit);
void func_15045714(Vec3f *pos, u16 mask, s32 *setA, s32 *setB);
s32 func_15045780(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 func_15045800(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 func_15045880(Vec3f *pos, f32 top, s32 *set, CollisionHit *hit);
s32 func_15045AE4(Vec3f *pos, f32 bottom, s32 *set, CollisionHit *hit);
s32 func_15045D48(Vec3f *pos, f32 top, s32 *set, CollisionHit *hit);
s32 func_15045F8C(Vec3f *pos, f32 bottom, s32 *set, CollisionHit *hit);
s32 func_150461D0(Vec3f *pos, u16 mask, f32 top, CollisionHit *hit);
s32 func_15046460(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 func_150466F8(Vec3f *pos, u16 mask, f32 top, CollisionHit *hit);
s32 func_1504697C(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 func_15046C00(Vec3f *pos, u16 mask, f32 top, CollisionHit *hit);
s32 func_15046C80(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 func_15046D00(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 func_15046F84(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 func_15047004(Vec3f *pos, f32 bottom, CollisionHit *hit);
s32 func_150470B0(Vec3f *pos, f32 top, CollisionHit *hit);
void func_1504715C(CollisionHit *hit, Actor *obj);
void func_150472C0(CollisionHit *hit, RayHit *src);
void func_15047390(MtxF mf, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp);
void func_15047688(Mtx *m, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp);
void func_15047700(MtxF mf, LookAt *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp,
                   f32 zUp);
void func_15047B80(Mtx *m, LookAt *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp,
                   f32 zUp);

/* game_AE1D0.c */
s32 func_15080D20(Actor *self, Actor *obj, s32 type, f32 dist, f32 radius, s32 arg5);
s32 func_1508108C(Actor *obj);
s32 func_1508114C(Actor *obj, Vec3f *rayOrigin, Vec3f *rayDir, Vec3f *outNear, Vec3f *outFar, Vec3f *outCentre,
                  f32 *outDist, s32 *outIndex, s32 *outBone, s32 *outPart, s16 *outOffset);
s32 func_15081574(s32 self, f32 dist, f32 radius, s32 outTarget, s32 arg4, s32 model);

/* game_AEB40.c */
void func_15081690(Actor *self, f32 originX, f32 originY, f32 originZ, f32 dirX, f32 dirY, f32 dirZ, RayHit *out,
                   f32 maxDist, s32 skipWorld, s32 skipObjs, s32 skipModels, s8 filter, s32 filterArg, s32 ignoreMask);
s32 func_15081C20(Vec3f *origin, Vec3f *dir, Actor *obj, Vec3f *outNear, Vec3f *outFar, f32 *outNearDist,
                  f32 *outFarDist);
void func_15081E0C(Actor *obj, u16 part, u8 skip);
s32 func_15081E78(Actor *self, RayHit *hit, s32 kind);

/* game_AFA70.c */
s32 func_150825C0(s32 spawnIdx, s32 arg1);
void func_1508295C(s32 arg0, s32 arg1, s32 firstRange);
void func_150829D8(Actor *obj);
s32 func_15082A44(SpawnRecord *spawn, s32 spawnIdx, s32 arg2, s32 arg3, s32 slot);
void func_150832AC(Actor *obj);
void func_15083384(Actor *obj, s32 arg1);
void *func_15083568(Actor *parent, s32 setId, f32 arg2, s32 arg3);
void func_150836CC(Actor *parent, s32 setId);
void func_150837D4(s32 objIdx, s32 modelId, s32 arg2);
s32 func_150838EC(Actor *obj, s32 arg1, s32 arg2, f32 arg3);
void func_150839B8(Actor *obj, s32 modelId, SpawnRecord *spawn);
Actor *func_15083AC8(Actor *parent, s32 arg1, s32 modelId, s32 arg3, s32 state, s32 arg5, s32 inheritScale, s32 arg7,
                     f32 arg8);
s32 func_15083DA0(void);
s32 func_15083E0C(u8 id);
Actor *func_15083E90(u8 id);
s32 func_15083FB0(u8 id);
s32 func_15084000(s32 id);
s32 func_15084044(Actor *obj, s32 model);
void func_1508434C(Actor *obj);
void func_150843AC(Actor *obj, s32 model);
void func_15084488(SpawnRecord *spawn, s32 arg1, s32 arg2);
void func_15084558(Actor *self);
s32 func_150849A0(Actor *obj);
s32 func_150849CC(Actor *obj, s32 *outIndex);
void func_15084A18(void);
void func_15084C30(Actor *obj);
s32 func_15084CB0(s32 modelId);

/* game_D0960.c */
s32 func_150A34B0(TriggerZone *cyl, Vec3f *p0, Vec3f *p1, Vec3f *hit);
s32 func_150A3504(TriggerZone *cyl, Vec3f *p0, Vec3f *p1, Vec3f *hit);

/* game_D0F20.s: the ground probe. func_150A3A70 gathers the candidate hits under (x, z) into
 * D_800D3300 and returns their count; func_150A3FC4 gives the height of a triangle at (x, z), from
 * its three vertex pointers or, when triVerts is NULL, from the vertices in tri. */
s32 func_150A3A70(s32 x, s32 z);
s32 func_150A3FC4(f32 x, f32 z, Vec3s **triVerts, Vec3s *tri, f32 *y);

/* game_D1890.s */
s32 func_150A43E0(s32 x, s32 z, s32 count, u16 *props);

/* game_D19A0.s */
void func_150A44F0(s32 count, u16 *props, s32 arg2);
/* (s32 map); declared without a prototype: func_1510F800 (game_13BB20.c) passes its own argument on */
void func_150A49F4();

/* game_D1FB4.s */
void func_150A4B04(Actor *actor);

/* game_D2450.s */
s32 func_150A4FA0(s32 x, s32 z);

/* game_D2570.s */
Gfx *func_150A50C0(Gfx *dl, Gfx *gfx, MtxF viewMtx, s32 arg3, s32 arg4, s32 viewMask);

/* game_D260C.s */
Gfx *func_150A5378(Gfx *dl, Gfx *gfx, MtxF viewMtx, s32 arg3, s32 arg4, s32 viewMask);

/* game_D3040.s */
struct ShadowVtx *func_150A5B90(u32 *tris, struct ShadowVtx *out, f32 *mtx, struct ShadowVtx *end);
struct ShadowVtx *func_150A5E44(u16 *props, struct ShadowVtx *out, f32 *mtx, struct ShadowVtx *end);

/* game_D36C0.s */
s32 func_150A6210(MtxF mtx, f32 x, f32 y, f32 z, f32 width, f32 height, f32 arg6);

/* game_D3810.s: func_150A6360 tests a cylinder against a view's frustum and returns its distance */
s32 func_150A6360(RenderView *view, MtxF viewMtx, f32 x, f32 y, f32 z, f32 radius, f32 height, f32 farDist);
void func_150A64C8(s16 x, s16 z, s32 *groups, s32 y);

/* game_D39B0.s */
s32 func_150A6500(s16 x, s16 z, s32 *groups, u16 mask);

/* game_D3A18.s */
s32 func_150A6568(s16 x, s16 z, s32 *groups, u16 group, s32 ix, s32 iz, s32 minY, s32 maxY);
void func_150A6760(s32 index);

/* game_D4450.s: wave deformers of vertex lists */
void func_150A6FA0(Vtx *dst, u16 count, WaveParams *params, s8 *phase, f32 *weight, Vtx *src);
void func_150A70C0(Vtx *dst, u16 count, WaveParams *params, s8 *phase, Vtx *src);
void func_150A71C8(Vtx *dst, u16 count, WaveParams *params, u8 *unk, s8 *phase, Vtx *src);
void func_150A7360(Vtx *dst, u16 count, WaveParams *params, u8 *unk, s8 *phase, f32 *weight, Vtx *src);
void func_150A751C(Vtx *dst, u16 count, WaveParams *params, u8 *angle, s8 *phase, f32 *weight, Vtx *src);

/* game_D4E10.s: transforms the point (x, y, z) by mtx */
void func_150A7960(MtxF mtx, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/* game_D4EB0.s: the same with the w of the result */
void func_150A7A00(MtxF mtx, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ, f32 *outW);

/* game_D4EF8.s: out = a * b */
void func_150A7A48(MtxF a, MtxF b, MtxF out);

/* game_D5030.s: an identity Mtx */
void func_150A7B80(Mtx *m);

/* game_D51B0.s: a translation Mtx */
void func_150A7D00(Mtx *m, f32 x, f32 y, f32 z);

/* game_D5250.s */
void func_150A7DA0(MtxF mtx, f32 x, f32 y, f32 z);

/* game_D52A0.s: a rotation Mtx, angles in degrees */
void func_150A7DF0(Mtx *m, f32 rx, f32 ry, f32 rz);

/* game_D5500.s */
void func_150A8050(MtxF mtx, f32 rx, f32 ry, f32 rz);

/* game_D5680.s */
void func_150A81D0(Mtx **mtx, MtxF transform, AnimInstance *anim, s32 arg3, s32 numParts, s16 *arg5, u8 *remap, s32 scale);

/* game_D6E34.s: converts count float matrices in place into Mtx */
void func_150A9984(Mtx *mtx, s32 count);

/* game_D6FBC.s */
void func_150A9B0C(MtxF mtx, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);

/* game_D8248.s */
s32 func_150AAD98(Tri3f tri);

/* game_D86A0.s */
s32 func_150AB1F0(f32 x, f32 y, f32 z, Actor *actor, s32 arg4);

/* game_D9894.s */
void func_150AC3E4(f32 x, f32 y, f32 z, Actor *actor, s32 arg4);

/* game_D9E70.s: casts the ray from (x, y, z) along (dx, dy, dz), up to maxDist when it is not 0. Each
 * output pointer can be NULL. */
s32 func_150AC9C0(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, void *outTriVerts, CollisionTri *outTri, f32 *hitX,
                  f32 *hitY, f32 *hitZ, f32 *outDist, s32 *outObject, s32 *outSurface, f32 maxDist);

/* game_DAC20.s */
void func_150AD770(void);

/* game_DAC30.s */
f32 func_150AD780(f32 x);

/* game_DAC3C.s */
f32 func_150AD78C(f32 x);

/* game_DAD60.s: out = a x b */
void func_150AD8B0(f32 *a, f32 *b, f32 *out);

/* game_DADB0.s */
f32 func_150AD900(f32 *a, f32 *b);
f32 func_150AD930(f32 *v);

/* game_DAE10.s */
s32 func_150AD960(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* game_DAE50.s */
u32 func_150ADA20(void);
/* The same random function at the same address, under a u8 return type. 14 files were compiled with
 * that view: they use the result as a byte without masking it. */
u8 func_150ADA20_u8(void);
f32 func_150ADA68(void);
void func_150ADACC(s32 seed);

/* game_DAFA0.s */
void func_150ADAF0(IdRecord *node, s32 mode);

/* game_DB730.s */
void func_150AE280(void);

/* game_DB750.c */
s32 func_150AE2A0(struct Foo150AE280 *arg0);
s32 func_150AE35C(s32 arg0);
s16 *func_150AE36C(struct Foo150AE36C *arg0, s16 arg1);

/* game_10EA20.c */
s32 func_150E1570(Actor *source, f32 fromX, f32 fromY, f32 fromZ, f32 toX, f32 toY, f32 toZ, s32 skipIndex, s32 effect);

/* game_131A90.c */
void func_151045E0(void *arg0, s32 arg1, f32 arg2);
s32 func_151045F4(s32 arg0, s32 arg1);
void func_15104608(f32 x, f32 y, f32 z, s32 arg3);
s32 func_15104620(s32 arg0, s32 arg1);
s32 func_15104634(f32 x, f32 z, f32 y, f32 *out);

/* game_13BB20.c */
s32 func_1510E670(s32 index);
void func_1510E7A4(void **outTri, Vec3s *outVerts, f32 *outFloorY, f32 *outPropFloorY, u32 *outFlags, s16 *outType,
                   f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *obj, f32 minY, f32 maxY);
void func_1510E82C(void **outTri, Vec3s *outVerts, f32 *outFloorY, f32 *outPropFloorY, u32 *outFlags, s16 *outType,
                   f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *obj);
void func_1510E8BC(void **outTri, Vec3s *outVerts, f32 *outFloorY, f32 *outPropFloorY, u32 *outFlags, s16 *outType,
                   f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *obj, f32 unused12, f32 unused13, s32 noFallback);
void func_1510E950(void **outTri, Vec3s *outVerts, s16 (*outPropVerts)[3], f32 *outFloorY, f32 *outPropFloorY,
                   u32 *outFlags, s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *object, f32 minY,
                   f32 maxY, s32 noFallback);
f32 func_1510F648(f32 x, f32 y, f32 z);
s32 func_1510F720(s32 x, s32 z, s32 radius, ZoneNode **outLeaves);
void func_1510F800();
void func_1510F820(void *tri, void *verts, f32 *outDirX, f32 *outDirZ, f32 *unused, f32 *outSlopeX, f32 *outSlopeZ);
s32 func_1510F8CC(s32 flags);
s32 func_1510F8D8(s32 x, s32 y, s32 z, s32 *outTri);
void func_1510FC34(s32 index);
s32 func_1510FD20(s32 x, s32 z);
s32 func_1510FE30(s32 zone);

#endif
