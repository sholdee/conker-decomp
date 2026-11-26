#ifndef CONKER_ACTOR_H
#define CONKER_ACTOR_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "actor": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_323E0.c
 *   game/done/game_43760.c
 *   game/done/game_43820.c
 *   game_58F80.c
 *   game_77BE0.c
 *   game_7F710.c
 *   game_7FA40.c
 *   game_7FC10.c
 *   game_803A0.c
 *   game_808E0.c
 *   game_80B80.c
 *   game_83300.c
 *   game_90840.c
 *   game_97AA0.c
 *   game_A9820.c
 *   game_A9D90.c
 *   game_DE5A0.c
 *   game_DF260.s
 *   game_EB020.c
 */


// 3-byte entries of D_80099140
typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
} Struct80099140;

typedef struct {
    s32 unk0;
    s32 unk4;
} Pair1506AA48;

typedef struct {
    /* 0x00 */ Actor *unk0;
    /* 0x04 */ Vec3f unk4;
    /* 0x10 */ s32 unk10;
} Arg1506AB7C;

/* game/done/game_323E0.c */
void func_15004F30(void);

/* game/done/game_43760.c */
void func_150162B0(void);

/* game/done/game_43820.c */
void func_15016370(void);

/* game_58F80.c */
Gfx *func_1502BAD0(Gfx *gdl, s32 mode, s16 player);
void func_1502BD84(Actor *obj, s32 index);
void func_1502BEE4(void);
void func_1502C1A4(void);
void func_1502C380(void);
s32 func_1502C3BC(s32 index);
Gfx *func_1502C408(Gfx *gdl, s32 index);
void func_1502C608(s32 index);
void func_1502C6E8(s32 index, s16 player, s32 mode);
Gfx *func_1502C974(Gfx *gdl, s32 index, s32 player, s32 mode, s32 wrap);
void func_1502CC34(Actor *obj, s32 player, s32 alpha, s32 *colour, s32 *primR, s32 *primG, s32 *primB, s32 *envR,
                   s32 *envG, s32 *envB, s32 *fogR, s32 *fogG, s32 *fogB, s32 *fogA);
Gfx *func_1502CCFC(Gfx *gdl, s32 index, s32 player, void *model, s32 alpha, s32 *colour, s32 mode, s32 wrap);
void func_1502D54C(s32 index, s32 *colour);
/* (Actor *obj, s32 *colour, s32 player); declared without a prototype: game_5D2C0.c passes 4 arguments */
void func_1502D630();
/* anim: an AnimInstance seen as struct ObjAnim (game_58F80.c), which indexes its current/previous pairs */
struct ObjAnim;
void func_1502D824(struct ObjAnim *anim, Actor *obj, u8 *buf);
s32 func_1502DB20(s32 id);
void func_1502DB84(MtxF mtx, s32 index);
void func_1502DF38(s32 index, s32 rootOnly);
void func_1502E474(void);
void func_1502E4C4(s32 index);
void func_1502E9FC(Actor *obj, s32 dist);
void func_1502EA0C(Actor *obj, s32 r, s32 g, s32 b, s32 a, s32 rate);
void func_1502EA50(Actor *obj);
void func_1502EA60(Actor *obj, s32 rate);
void func_1502EA7C(Actor *obj, s32 rate);
void func_1502EA98(Actor *obj, s32 r, s32 g, s32 b, s32 a, s32 instant, s32 rate);
void func_1502EAFC(Actor *obj);
void func_1502EC34(Actor *arg0, s32 *r, s32 *g, s32 *b, s32 *a);
s32 func_1502EE8C(s32 index, s32 slot);
void func_1502EEF4(s32 index);
Gfx *func_1502F01C(Gfx *gdl, s32 index);
void func_1502F264(s32 index);
void func_1502F3C8(void);
void func_1502F490(Actor *platform, f32 *x, f32 *y, f32 *z, s32 tri);
void func_1502F948(Actor *obj);
Gfx *func_1502F9FC(Gfx *gdl, s32 index);
void func_1502FBE8(Actor *obj);
void func_1502FD70(Actor *arg0);

/* game_77BE0.c */
void func_1504A730(void);
void func_1504ADD0(void);
s32 func_1504AEF4(s32 arg0, s32 arg1);
void func_1504AF10(Actor *obj, s32 force, s32 release);
f32 func_1504B0FC(Actor *obj, f32 arg1);
void func_1504BA38(Actor *obj);
void func_1504BAF0(Actor *obj);
void func_1504BB88(Actor *obj);
void func_1504BC38(Actor *obj);
void func_1504BE2C(Actor *obj, u16 *arg1, f32 *arg2, s8 *arg3);
s32 func_1504C078(void);
s32 func_1504C0B8(void);
void func_1504C0E8(Actor *obj, s32 arg1, f32 arg2);
void func_1504C854(Actor *obj);
s32 func_1504C8BC(Actor *obj);
void func_1504C9E4(Actor *obj, s8 target, u8 mode);
extern const f32 D_800990C0[8];
extern const f32 D_800990E0[8];
extern const f32 D_80099100[8];
extern const f32 D_80099120[8];
extern const Struct80099140 D_80099140[20];
extern const u16 D_8009917C[16];
extern const u16 D_8009919C[6];

/* game_7F710.c */
void func_15052260(Actor *actor);
void func_15052408(Actor *actor);
void func_15052458(s32 unused);
void func_15052464(Actor *actor);
void func_15052490(Actor *actor, u16 angle, f32 amount, f32 scale);
void func_1505250C(Actor *actor, s32 unused);

/* game_7FA40.c */
void func_15052590(Actor *obj);

/* game_7FC10.c */
void func_15052760(Actor *obj, s16 prevAngle);
void func_150528C8(Actor *obj);

/* game_803A0.c */
void func_15052EF0(Actor *obj);
Actor *func_15052F58(s32 index, s32 arg1);
/* void func_15052F9C(Actor *, f32 height, ...) is declared in the files that call it:
   game_1048D0.c was compiled with an s32 return. */
void func_1505327C(Actor *arg0, f32 yVelocity, f32 gravity, s32 arg3, s32 mtxIndex);

/* game_808E0.c */
s32 func_15053430(Actor *obj);
s32 func_150535F4(Actor *obj);
void func_15053694(Actor *obj);

/* game_80B80.c */
void func_150536D0(Actor *obj);
void func_150536E8(Actor *obj);
void func_1505371C(Actor *obj);
void func_15053750(Actor *obj);
void func_15053894(Actor *obj);
void func_150538CC(Actor *obj);
void func_150548D8(s32 arg0);
void func_150548E4(Actor *obj);
void func_15054A0C(Actor *obj);
void func_15054A5C(Actor *obj, Actor *other);
void func_15054A94(Actor *obj);
void func_15054F74(Actor *obj);
void func_15055260(Actor *obj, Actor *target, s32 hitIndex);
void func_1505528C(Actor *obj);
void func_15055A2C(s32 arg0, f32 x, f32 y, f32 z, s32 arg4);
void func_15055B0C(Actor *obj, s32 duration);
void func_15055B64(Actor *obj);
void func_15055BF8(Actor *obj);
void func_15055C88(Actor *obj);
void func_15055D48(Actor *obj);

/* game_83300.c */
void func_15055E50(Actor *actor, s32 zoneSetId);
void func_15056150(Actor *actor);
void func_15056258(Actor *actor);
void func_150562FC(Actor *actor);
s32 func_1505693C(Actor *actor, s32 targetIdx);
void func_15056A00(Actor *actor, u8 turn, u8 row);
void func_15056B08(Actor *actor);
void func_1505841C(Actor *actor, f32 idleRate);
void func_150585F0(Actor *actor, f32 decel);
void func_15058898(Actor *actor, f32 prevY);
void func_15058EA4(Actor *actor, f32 maxY, f32 gravityAbove, f32 minY, f32 gravityBelow, f32 maxVelY, f32 minVelY);
void func_15058F24(Actor *actor, f32 depth, f32 arg2);
void func_15059140(Actor *actor);
void func_150593C4(Actor *actor, u16 angle, f32 speed, f32 rate);
void func_15059444(Actor *actor);
void func_1505959C(Actor *actor, s32 hostIdx);
void func_150597FC(Actor *actor);
u8 func_150599C8(Actor *actor, u8 turnRate, u16 targetAngle);
u8 func_15059B54(Actor *actor, u16 maxStep);
void func_15059C84(Actor *actor);
void func_1505A184(u16 angle, f32 speed, f32 pitch, f32 *outX, f32 *outZ, f32 *outY);
void func_1505A250(f32 targetX, f32 targetZ, f32 rate, f32 *x, f32 *z);
void func_1505A3A8(f32 targetSpeed, Actor *actor, f32 accel, f32 decel, u8 turnDiff);
f32 func_1505A5CC(OSContPad *pad);
u16 func_1505A630(f32 x, f32 z, s32 unused);
f32 func_1505A6F8(Actor *a, Actor *b);
f32 func_1505A72C(Actor *a, Actor *b);
void func_1505A770(Actor *actor);
void func_1505A9AC(Actor *actor, f32 floorY);
void func_1505B5F8(Actor *actor, f32 floorY);
void func_1505B9C4(Actor *attacker, Actor *victim, HitReaction *base, HitReaction *hit, u8 index, s32 victimSlot,
                   u8 damageMask);
s16 func_1505C140(Actor *attacker, Actor *victim);
HitReaction *func_1505C1A4(Actor *actor);
s32 func_1505C1E4(Actor *attacker, Actor *victim, HitReaction *table, s32 index, s32 victimSlot, s32 hitMask,
                  s32 damageMask);
s32 func_1505C7D8(Actor *attacker, s32 selfSlot);
s32 func_1505D024(Actor *victim, s32 flags, u16 angle, s32 sourceSlot);
s32 func_1505D1C4(f32 x, f32 y, f32 z, s32 attackId, s32 sourceSlot, u16 angle, s32 losSkipSlot, s32 losOrigin);
void func_1505D2B8(Actor *actor, u8 entryIdx);
f32 func_1505D34C(f32 angleDeg, f32 a, f32 b, f32 scale, f32 *outZ);
void func_1505D408(Actor *actor, Actor *other, f32 nx, f32 ny, f32 nz, f32 distSq, f32 minDist, s32 otherSlot);
void func_1505D5D0(Actor *actor, Actor *other, f32 x, f32 y, f32 z, f32 ratio, f32 radius, s32 otherSlot,
                   f32 extraRadius, f32 extraHeight, f32 extraY, f32 offsetA, f32 offsetB);
void func_1505D6F0(Actor *actor, s32 selfIdx);
u8 func_1505DADC(Actor *actor, u16 *outAngle, u8 pitch, u8 skipIdx, u8 halfCone);
s8 func_1505DDA8(Actor *actor, s32 unused, u16 cone, f32 time);
f32 func_1505DF10(Actor *actor, u8 otherIdx, u16 *outAngle, f32 *outDistSqXZ, f32 *outDy, s32 unused);
void func_1505DFDC(Actor *actor);
void func_1505E060(AnimInstance *state);
void func_1505E0C4(Actor *actor, u16 *animEntry, AnimInstance *state, s32 arg3, s32 restart, s32 anim,
                   s32 bank, f32 speed, f32 blendFrames, f32 arg9, f32 arg10, s32 flag8000);
void func_1505E650(Actor *actor, u16 anim, f32 speed, f32 blend, f32 arg4, f32 arg5, s32 flag8000);
u32 func_1505E7CC(s32 kind, Actor *actor);
void func_1505E874(s32 sourceIdx, Actor *actor);
Actor *func_1505ED34(void);
Actor *func_1505EEB0(s32 state, s32 *outIdx);
Actor *func_1505EEF4(s32 pathIdx);
Actor *func_1505EFD0(s32 key);
Actor *func_1505F0AC(s32 id);
void func_1505F188(Actor *actor);
void func_1505F298(Actor *actor, OSContPad *pad);
void func_1506045C(Actor *actor, s32 buttons, s32 pressed, f32 height, s32 nested);
void func_15060778(s32 sound, Actor *actor, u16 rate, s32 volume, s32 minDist, s32 maxDist, s32 flags);
void func_15060A30(s32 sound, Actor *actor);
void func_15060A9C(s32 sound, Actor *actor);
void func_15060B04(s32 sound, Actor *actor, s32 rate);
void func_15060B70(s32 sound, void *actor);
s32 func_15060BA4(Actor *actor, s32 amount);
void func_15060BE0(Actor *actor);
void func_15060D54(Actor *actor);
void func_15060F28(Actor *actor, s32 mode);
void func_150611E8(Actor *actor, s32 pathIdx);
void func_150615DC(Actor *actor);
void func_1506160C(Actor *actor, u8 cmd, u8 value, u8 step, s32 player);
void func_150617BC(Actor *actor);
s32 func_1506196C(Actor *actor, s32 player);
void func_150619A8(void);
void func_15061B4C(void);
s32 func_150623F4(Actor *actor);
void func_150626EC(Actor *actor, s32 mode);
void func_150627D4(Actor *actor);
void func_15062800(Actor *a, Actor *b);
void func_15062AC4(Actor *actor);
void func_15062B1C(Actor *actor, f32 radius);
void func_15062B50(Actor *actor, f32 height);
void func_15062B84(Actor *actor);
void func_15062BDC(Actor *actor, f32 xzScale, f32 yScale);
void func_15062D10(s32 type, s32 tile, s32 ds, s32 dt, s32 set, s32 absolute);
void func_15062E24(s32 type, s32 firstVtx, s32 arg2, s32 arg3, s32 arg4, s32 set, s32 doubleSize);
void func_15062FC0(Actor *actor, s32 group, s32 arg2, s32 wrapS, s32 wrapT, s32 count, s32 ds, s32 dt);
void func_15063168(Actor *actor);
void func_15063254(Actor *actor, s32 type, s32 interaction_state, f32 scale);
extern const u16 D_80099420[4];
extern const u16 D_80099428[2];
extern const s32 D_8009942C[3];

/* game_90840.c */
s16 func_15063390(Actor *arg0);
void func_15063404(Actor *arg0);
void func_150634E4(Actor *arg0);
void func_15063570(Actor *arg0);
void func_15063628(Actor *arg0, f32 arg1);
void func_150636A4(Actor *arg0);
void func_150636F0(void);
s32 func_150639BC(Actor *arg0);
s32 func_15063A38(Actor *arg0, s32 buttons, s32 mask);
void func_15063B64(Actor *arg0);
Actor *func_15063C60(Actor *arg0, s32 id);
s32 func_15063E84(Actor *arg0, u16 anim, u16 flags, s32 action, s32 buttons);
void func_15063FA0(Actor *arg0, s32 kind);
u16 func_150641D8(Actor *arg0, u16 animA, u16 animB, u16 animC);
void func_150642AC(Actor *arg0, s32 buttons, f32 arg2, s32 noPlay, s32 lockB);
void func_150649A0(s32 srcIndex, s32 dstIndex);
void func_15064A14(Actor *arg0);
s32 func_15064B94(Actor *arg0, f32 arg1, f32 arg2, s32 overrideAnim, f32 arg4);
s32 func_15065A5C(Actor *arg0, s32 heldButtons, s32 pressedButtons, f32 arg3, s32 arg4, s32 act, s32 arg6, s32 arg7,
                  s32 arg8);

/* game_97AA0.c */
void func_1506A5F0(Actor *obj, s32 arg1);
void func_1506A6FC(Actor *obj, s32 arg1);
void func_1506A724(Actor *obj, s32 arg1);
void func_1506A760(Actor *obj, s32 arg1);
void func_1506A79C(Actor *obj, s32 arg1);
void func_1506A83C(Actor *obj, s32 arg1);
void func_1506A864(Actor *obj, s32 arg1);
void func_1506A968(Actor *obj, s32 arg1);
void func_1506AA08(Actor *obj, s32 arg1);
void func_1506AA48(Actor *obj, Pair1506AA48 *arg);
void func_1506AAE0(Actor *obj, s32 arg1);
void func_1506AB7C(Actor *obj, Arg1506AB7C *arg);
void func_1506ABC4(Actor *obj, Arg1506AB7C *arg);
void func_1506AC0C(Actor *obj, s32 arg1);
void func_1506AC58(Actor *obj, s32 arg1);
void func_1506AC8C(Actor *obj, s32 type, s32 arg);

/* game_A9820.c */
void func_1507C370(void);
void func_1507C3E0(Actor *obj, s16 *outHeight, s16 *outWidth, s16 *outDepth);

/* game_A9D90.c */
void func_1507C8E0(Actor *obj, s32 timer);
void func_1507C8FC(void);
void func_1507CD0C(Actor *obj);
void func_1507CD64(Actor *obj, s32 cause);
void func_1507D158(s32 slot, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_1507D1D8(Actor *obj);
void func_1507D4F8(s32 slot);
s32 func_1507D754(Actor *obj);
void func_1507DB34(void);
void func_1507DB3C(void);
void func_1507DB44(Actor *obj, s32 arg1);
void func_1507DB64(void);
Gfx *func_1507DB6C(Gfx *gfx, s32 player);
void func_1507DE4C(Actor *obj);
void func_1507DF10(Actor *obj, s32 part);
void func_1507DFE4(s32 resource, s32 arg1);
s32 func_1507E114(s32 arg0);
void func_1507E1D0(Actor *obj, f32 *x, f32 *y, f32 *z);

/* game_DE5A0.c */
void func_150B10F0(Actor *obj);
void func_150B12FC(Actor *obj);
void func_150B1484(Actor *obj);
void func_150B17DC(Actor *obj);
void func_150B19E0(Actor *obj);

/* game_DF260.s: rearranges the 64-bit words from start up to end */
void func_150B1DB0(void *start, void *end);

/* game_EB020.c */
void func_150BDB70(Actor *self);

#endif
