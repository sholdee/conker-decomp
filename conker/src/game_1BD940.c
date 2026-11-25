#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Emitter layout, as in game_1BC650.c (func_1518F5D0 creates these). */
typedef struct {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 rateRange;
    /* 0x08 */ f32 burstBase;
    /* 0x0C */ f32 burstRange;
    /* 0x10 */ f32 sizeX;
    /* 0x14 */ f32 sizeY;
    /* 0x18 */ f32 sizeXRange;
    /* 0x1C */ f32 sizeYRange;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21Range;
    /* 0x22 */ s16 lifeMin;
    /* 0x24 */ s16 lifeRange;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ s32 maxCount;
    /* 0x2C */ f32 velocityScale;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34Range;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ s16 unk42Range;
    /* 0x44 */ s32 anchorSet;
    /* 0x48 */ u8 flags;
    /* 0x49 */ u8 pad49[3];
    /* 0x4C */ f32 unk4C;
} EmitterTemplate; /* 0x50 */

typedef struct {
    /* 0x00 */ f32 accum;
    /* 0x04 */ EmitterTemplate tmpl;
    /* 0x54 */ struct225 *child;
    /* 0x58 */ s8 updateFunc;
    /* 0x59 */ s8 unk59Func;
    /* 0x5A */ u8 unk5A;
    /* 0x5B */ u8 pad5B;
} EmitterParams; /* 0x5C */

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ struct127 *owner;
    /* 0x14 */ u8 pad14[0xC];
    /* 0x20 */ s16 count;
    /* 0x22 */ u8 pad22[0xE];
    /* 0x30 */ EmitterParams params;
} Emitter;

/* Event payload naming an actor, as in game_1944C0.c. */
typedef struct {
    /* 0x0 */ struct127 *actor;
    /* 0x4 */ u8 id; /* the actor's unique_id */
} ActorEvent;

/* Object handled by func_15190454 (game_1BD8B0.c). */
typedef struct Game1BD8B0Object Game1BD8B0Object;

void func_15190454(Game1BD8B0Object *obj);
void func_151617E4(Game1BD8B0Object *obj);
s32 func_1518E298();
void func_1000FD38(void *, void *, s32);
void func_151D2B4C(s32);

void func_15190490(Game1BD8B0Object *obj) {
    func_15190454(obj);
    func_151617E4(obj);
}

/* Emitter destroy: frees its child effect, unregisters it and releases its anchor set. */
void func_151904BC(Emitter *emitter) {
    EmitterParams *params = &emitter->params;

    if (params->child != NULL) {
        func_1516972C((struct102 *)params->child);
    }
    func_1000FD38(func_1518E298, emitter, (s32)emitter->owner);
    func_151D2B4C(params->tmpl.anchorSet);
}

/* Sends event 0x2A about `actor` to every object of type 0x3E. */
void func_15190518(struct127 *actor) {
    ActorEvent event;

    event.actor = actor;
    event.id = actor->unique_id;
    func_1516944C(0x3E, (s32)&event, 0x2A);
}
