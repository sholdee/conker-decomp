#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Orientation, scale and simple ballistic motion stored after an effect actor's header. */
typedef struct {
    /* 0x00 */ Vec3f angle;
    /* 0x0C */ f32 scale;
    /* 0x10 */ Vec3f velocity;
    /* 0x1C */ Vec3f angularVelocity;
    /* 0x28 */ f32 gravity;
    /* 0x2C */ f32 damping;
} EffectMotion;

/*
 * Effect actor allocated by func_15167A68. Bytes 0x10..0x53 are copied from a 0x44-byte
 * template; type-specific data follows the 0xF8-byte header.
 */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u8 flags; /* 1: lifetime counts down, 2: attached to owner, 4: attach option */
    /* 0x11 */ u8 pad11;
    /* 0x12 */ s8 updateCallback; /* index into D_8008AE00, -1 for none */
    /* 0x13 */ s8 drawCallback;   /* index into D_8008AE0C, -1 for none */
    /* 0x14 */ s16 lifetime;
    /* 0x16 */ u8 displayList;    /* index into D_8008AFB8 */
    /* 0x17 */ u8 pad17;
    /* 0x18 */ s32 otherModeH;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 renderPreset;  /* index into D_800A4AC8 */
    /* 0x28 */ u8 pad28[3];
    /* 0x2B */ u8 combineMode;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s8 primColorMode;
    /* 0x35 */ s8 envColorMode;
    /* 0x36 */ u8 pad36[2];
    /* 0x38 */ u8 color0[4]; /* [3] is alpha */
    /* 0x3C */ u8 color1[4];
    /* 0x40 */ void *owner;
    /* 0x44 */ u8 ownerId;
    /* 0x45 */ u8 attachMode;
    /* 0x46 */ u8 pad46[2];
    /* 0x48 */ Vec3f position;
    /* 0x54 */ u8 pad54[4];
    /* 0x58 */ Mtx mtx[2]; /* double-buffered by D_800BE9C0 */
    /* 0xD8 */ s32 attachment; /* start of the block func_151462C8 reads */
    /* 0xDC */ u8 unkDC;
    /* 0xDD */ u8 padDD[3];
    /* 0xE0 */ s32 *handles[4]; /* one per D_80082FA0 + 1 */
    /* 0xF0 */ s32 *sharedHandle;
    /* 0xF4 */ s32 unkF4;
    /* 0xF8 */ union {
        EffectMotion motion;
        f32 size;
        struct {
            s16 duration;
            s32 alphaPerTick;
        } fade;
    } data;
} EffectActor;

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern s32 *func_1515D480(s32);
extern s32 *func_1515D440(void);

/* Allocates an effect actor of extraSize type-specific bytes from a 0x44-byte template. */
EffectActor *func_151580B0(void *template, s32 attachment, s32 arg2, u8 large, s32 extraSize, u8 arg5,
                           s32 arg6) {
    EffectActor *actor;
    s32 i;

    actor = func_15167A68(large ? 0x55 : 0x37, arg6, extraSize + 0xF8, 1, arg5, 1);
    if (actor == NULL) {
        return NULL;
    }
    memcpy(&actor->flags, template, 0x44);
    actor->attachment = attachment;
    actor->unkF4 = arg2;
    actor->unkDC = 0;
    for (i = 0; i < 4; i++) {
        actor->handles[i] = NULL;
    }

    actor->sharedHandle = NULL;
    if (attachment) {
        for (i = 0; i <= D_80082FA0; i++) {
            actor->handles[i] = func_1515D480(attachment);
        }
        actor->sharedHandle = func_1515D440();
    }
    return actor;
}

EffectActor *func_151581D8(void *template, u8 large, s32 extraSize, u8 arg3, s32 arg4) {
    return func_151580B0(template, 0, 0, large, extraSize, arg3, arg4);
}

extern s32 (*D_8008AE00[])(EffectActor *);

/* Per-frame update: counts down the lifetime, runs the update callback, frees the actor when done. */
void func_15158224(EffectActor *actor) {
    s32 callback;
    u8 expired;

    expired = 0;
    if (actor->flags & 1) {
        actor->lifetime -= D_800BE9E4;
        if (actor->lifetime < 0) {
            expired = 1;
        }
    }
    if (expired == 0) {
        callback = actor->updateCallback;
        if (callback != -1) {
            if (D_8008AE00[callback](actor) == 0) {
                expired = 1;
            }
        }
    }
    if (expired) {
        func_1516972C((struct102 *)actor);
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
} RenderPreset;

extern s32 (*D_8008AE0C[])(Mtx *, EffectActor *);
extern Gfx *D_8008AFB8[];
extern RenderPreset D_800A4AC8[];

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 r, s32 g, s32 b, s32 a, u8 *update);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 r, s32 g, s32 b, s32 a, u8 *update);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *update);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *update);
void func_151441A4(s16 *r, s16 *g, s16 *b, s16 *a, u8 arg4, u8 arg5, u8 arg6, s32 arg7, u8 arg8, u8 arg9,
                   u8 arg10, u8 arg11, u8 arg12, u8 arg13);
void func_151442FC(s16 *r, s16 *g, s16 *b, s16 *a, u8 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9,
                   u8 arg10, u8 arg11, u8 arg12, u8 arg13);
Gfx *func_151462C8(Gfx *gfx, void *arg1, u8 arg2, void *arg3, u8 arg4, s16 arg5, Vec3f *arg6, u8 arg7, s32 arg8);

/* Draws an effect actor: sets its render state and colors, then its matrix and display list. */
Gfx *func_151582C8(Gfx *gfx, EffectActor *actor, s16 arg2) {
    s32 pad; // matching: IDO's frame has an unused slot here
    s16 primR, primG, primB, primA;
    s16 envR, envG, envB, envA;
    u8 update;

    update = 1;
    if (actor->drawCallback != -1) {
        if (D_8008AE0C[actor->drawCallback](&actor->mtx[D_800BE9C0], actor) == 0) {
            return gfx;
        }
    }
    gfx = func_15142B7C(gfx, actor->unk1C, actor->unk20);
    func_151441A4(&primR, &primG, &primB, &primA, actor->color1[0], actor->color1[1], actor->color1[2], actor->color1[3],
                  actor->color0[0], actor->color0[1], actor->color0[2], actor->color0[3], 255, actor->primColorMode);
    func_151442FC(&envR, &envG, &envB, &envA, actor->color1[0], actor->color1[1], actor->color1[2], actor->color1[3],
                  actor->color0[0], actor->color0[1], actor->color0[2], actor->color0[3], 255, actor->envColorMode);
    gfx = func_15142C10(gfx, envR, envG, envB, envA, &update);
    gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &update);
    gfx = func_1513F4E4(gfx, actor->combineMode, &update);
    gfx = func_15142FBC(gfx, actor->otherModeH | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | actor->unk2C | actor->unk30,
                        D_800A4AC8[actor->renderPreset].unk0 | D_800A4AC8[actor->renderPreset].unk4, &update);
    if (actor->flags & 2) {
        gfx = func_151462C8(gfx, &actor->attachment, actor->attachMode, actor->owner, actor->ownerId, arg2,
                            &actor->position, (actor->flags & 4) ? 1 : 0, 0);
    }
    gSPMatrix(gfx++, &actor->mtx[D_800BE9C0], G_MTX_MODELVIEW | G_MTX_LOAD);
    gSPDisplayList(gfx++, D_8008AFB8[actor->displayList]);
    return gfx;
}

/* Draw callback: rotates by data.motion.angle, scales uniformly and translates to the actor's position. */
s32 func_1515858C(Mtx *mtx, EffectActor *actor) {
    EffectMotion *motion = &actor->data.motion;
    MtxF mf;

    func_150A8050(mf, motion->angle.x, motion->angle.y, motion->angle.z);
    mf[3][0] = actor->position.x;
    mf[3][1] = actor->position.y;
    mf[3][2] = actor->position.z;
    mf[0][0] *= motion->scale;
    mf[0][1] *= motion->scale;
    mf[0][2] *= motion->scale;
    mf[1][0] *= motion->scale;
    mf[1][1] *= motion->scale;
    mf[1][2] *= motion->scale;
    mf[2][0] *= motion->scale;
    mf[2][1] *= motion->scale;
    mf[2][2] *= motion->scale;
    guMtxF2L(mf, mtx);
    return 1;
}

/* Update callback: damps horizontal velocity, applies gravity and integrates position and angle. */
s32 func_15158684(EffectActor *actor) {
    EffectMotion *motion;
    Vec3f prevVelocity;
    s32 i;

    motion = &actor->data.motion;
    prevVelocity = motion->velocity;
    for (i = D_800BE9E4; i != 0; i--) {
        motion->velocity.x *= motion->damping;
        motion->velocity.z *= motion->damping;
    }
    {
        f32 accelX;
        f32 accelZ;
        f32 accelY;

        motion->velocity.y += motion->gravity * D_800BE9A4;
        accelX = (motion->velocity.x - prevVelocity.x) * D_800BE9A8;
        accelY = motion->gravity;
        accelZ = (motion->velocity.z - prevVelocity.z) * D_800BE9A8;
        actor->position.x += (prevVelocity.x + 0.5f * accelX * D_800BE9A4) * D_800BE9A4;
        actor->position.y += (prevVelocity.y + 0.5f * accelY * D_800BE9A4) * D_800BE9A4;
        actor->position.z += (prevVelocity.z + 0.5f * accelZ * D_800BE9A4) * D_800BE9A4;
    }
    motion->angle.x += motion->angularVelocity.x * D_800BE9A4;
    motion->angle.y += motion->angularVelocity.y * D_800BE9A4;
    motion->angle.z += motion->angularVelocity.z * D_800BE9A4;
    return 1;
}

/* A matrix wrapped in a struct so that it can be copied by assignment. */
typedef struct {
    MtxF m;
} MtxFBox;

extern MtxFBox D_8008AE18;

/* Draw callback: scales the fixed matrix D_8008AE18 by data.size and translates to the actor's position. */
s32 func_15158920(Mtx *mtx, EffectActor *actor) {
    f32 scale;
    s32 pad; // matching: IDO's frame has an unused slot here
    MtxFBox mf;

    mf = D_8008AE18;
    scale = actor->data.size * 0.003921568859f;
    mf.m[3][0] = actor->position.x;
    mf.m[3][1] = actor->position.y;
    mf.m[3][2] = actor->position.z;
    mf.m[0][0] *= scale;
    mf.m[0][1] *= scale;
    mf.m[0][2] *= scale;
    mf.m[1][0] *= scale;
    mf.m[1][1] *= scale;
    mf.m[1][2] *= scale;
    mf.m[2][0] *= scale;
    mf.m[2][1] *= scale;
    mf.m[2][2] *= scale;
    guMtxF2L(mf.m, mtx);
    return 1;
}

/* Releases the attachment handles allocated by func_151580B0. */
void func_15158A20(EffectActor *actor) {
    s32 i;

    s32 *handle; // matching: copied after each NULL test so IDO keeps the load in v0

    for (i = 0; i <= D_80082FA0; i++) {
        if (actor->handles[i] != NULL) {
            handle = actor->handles[i];
            func_100043B4(handle, 4);
        }
    }
    if (actor->sharedHandle != NULL) {
        handle = actor->sharedHandle;
        func_100043B4(handle, 4);
    }
}

void func_15158AA4(EffectActor *actor) {
    func_15158A20(actor);
    func_15169804((struct102 *)actor);
}

void func_15158AD0(EffectActor *actor) {
    func_15158A20(actor);
    func_15169824((struct102 *)actor);
}

/* Update callback: once the lifetime drops below data.fade.duration, fades alpha out with it. */
s32 func_15158AFC(EffectActor *actor) {
    s32 alpha;

    if (actor->lifetime < actor->data.fade.duration) {
        alpha = actor->lifetime * actor->data.fade.alphaPerTick;
        if (alpha < actor->color0[3]) {
            actor->color0[3] = alpha;
        }
    }
    return 1;
}

/* Message passed to the effect message handlers; msgId 0x2D swaps owners first <-> second. */
typedef struct {
    /* 0x00 */ Actor *first;
    /* 0x04 */ union {
        Actor *ptr;
        u8 uniqueId; /* msgId 0: unique id of `first` */
    } second;
    /* 0x08 */ u8 firstId;
    /* 0x09 */ u8 secondId;
} EffectMessage;

/* Message handler: follows owner swaps (0x2D) and drops the owner when it goes away (0). */
void func_15158B3C(EffectActor *actor, EffectMessage *msg, u8 msgId) {
    Actor *first;

    if (msgId == 0x2D) {
        first = msg->first;
        if (actor->owner == first) {
            actor->owner = msg->second.ptr;
            actor->ownerId = msg->secondId;
        } else {
            if (actor->owner == msg->second.ptr) {
                actor->owner = first;
                actor->ownerId = msg->firstId;
            }
        dummy_label_356969: ; // matching: the empty labelled statement splits IDO's basic block
        }
    } else if (msgId == 0) {
        first = msg->first;
        if (actor->owner != first) {
            if (msg->second.uniqueId != actor->ownerId) {
                return;
            }
        }
        actor->owner = NULL;
        actor->ownerId = 0;
    }
}

/*
 * Tracks a character's state (five small fields packed by STATE_BITS) and runs the
 * actions of D_8008B02C[mode] when it changes as described by D_800A6200.
 */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ Actor *character;
    /* 0x1C */ u8 characterId;
    /* 0x1D */ u8 mode; /* from func_15159370 */
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ Vec3f lastPosition;
    /* 0x2C */ u8 state0; /* func_15159084 */
    /* 0x2D */ u8 state1; /* func_15159120 */
    /* 0x2E */ u8 state2; /* func_15159184 */
    /* 0x2F */ u8 moveState; /* func_15159230 */
    /* 0x30 */ u8 surfaceState; /* func_151592B8 */
    /* 0x31 */ u8 flags; /* 1: run state-change actions */
    /* 0x32 */ u8 pad32[2];
    /* 0x34 */ u8 unk34[0x1C];
    /* 0x50 */ u8 unk50;
} CharacterWatcher;

s32 func_15159370(Actor *chr, u8 *mode);
void func_1514EDF0(CharacterWatcher *watcher, Actor *chr);

/* Creates a watcher for chr; returns NULL when chr is NULL or has no watchable mode. */
CharacterWatcher *func_15158BD0(Actor *chr, u8 enableActions, s32 extraSize) {
    CharacterWatcher *watcher;

    if (chr == NULL) {
        return NULL;
    }
    watcher = func_15167A68(0x2E, 0, extraSize + 0x58, 1, 0xFF, 1);
    if (watcher == NULL) {
        return NULL;
    }
    watcher->character = chr;
    watcher->characterId = chr->unique_id;
    watcher->state0 = 2;
    watcher->state1 = 2;
    watcher->state2 = 2;
    watcher->moveState = 3;
    watcher->surfaceState = 0;
    watcher->lastPosition.x = chr->x_position;
    watcher->lastPosition.y = chr->y_position;
    watcher->lastPosition.z = chr->z_position;
    watcher->flags = 0;
    if (enableActions != 0) {
        watcher->flags |= 1;
    }
    watcher->unk10 = 1;
    watcher->unk14 = 0;
    if (func_15159370(chr, &watcher->mode) == 0) {
        func_1516979C((struct102 *)watcher);
        return NULL;
    }
    watcher->unk50 = 0;
    return watcher;
}

void func_15158CD4(CharacterWatcher *watcher) {
    func_1514EDF0(watcher, watcher->character);
    func_15169804((struct102 *)watcher);
}

void func_15158D00(CharacterWatcher *watcher) {
    func_1514EDF0(watcher, watcher->character);
    func_15169824((struct102 *)watcher);
}

s32 func_15159084(Actor *chr, u8 mode);
s32 func_15159120(Actor *chr, u8 mode);
s32 func_15159184(Actor *chr, u8 mode);
s32 func_15159230(Actor *chr, Vec3f *lastPosition, u8 prevState);
s32 func_151592B8(Actor *chr, u8 mode);

/* A state-change rule: both masks are unions of STATE_BITS values. */
typedef struct {
    s32 from;
    s32 to;
} StateRule;

/* Packs the five state fields into one bitfield, three bits per field. */
#define STATE_BITS(a, b, c, d, e)                                              \
    ((1 << (a)) | (1 << ((b) + 3)) | (1 << ((c) + 6)) | (1 << ((d) + 9)) |     \
     (1 << ((e) + 13)))

extern void (*D_8008AFD0[])(CharacterWatcher *);
extern s8 *D_8008B02C[];
extern StateRule D_800A6200[];

/* Update: recomputes the character's state and fires every rule matching the old -> new transition. */
void func_15158D2C(CharacterWatcher *watcher) {
    s32 oldBits;
    s8 action;
    u8 oldState0;
    u8 oldState1;
    u8 oldState2;
    u8 oldMoveState;
    u8 oldSurfaceState;
    s32 newBits;
    s32 i;

    oldState0 = watcher->state0;
    oldState1 = watcher->state1;
    oldState2 = watcher->state2;
    oldMoveState = watcher->moveState;
    oldSurfaceState = watcher->surfaceState;

    watcher->state0 = func_15159084(watcher->character, watcher->mode);
    if (watcher->state0 == 0) {
        watcher->state1 = func_15159120(watcher->character, watcher->mode);
    } else {
        watcher->state1 = 2;
    }

    if (watcher->state0 == 0) {
        if (watcher->state1 == 0) {
            watcher->state2 = 1;
        } else {
            watcher->state2 = func_15159184(watcher->character, watcher->mode);
        }
    } else {
        watcher->state2 = 0;
    }

    watcher->moveState = func_15159230(watcher->character, &watcher->lastPosition, watcher->moveState);
    watcher->lastPosition.x = watcher->character->x_position;
    watcher->lastPosition.y = watcher->character->y_position;
    watcher->lastPosition.z = watcher->character->z_position;
    watcher->surfaceState = func_151592B8(watcher->character, watcher->mode);

    if (watcher->flags & 1) {
        newBits = STATE_BITS(watcher->state0, watcher->state1, watcher->state2, watcher->moveState,
                             watcher->surfaceState);
        for (i = 0; i < 28; i++) {
            action = D_8008B02C[watcher->mode][i];
            if (action != -1) {
                oldBits = STATE_BITS(oldState0, oldState1, oldState2, oldMoveState, oldSurfaceState);
                if ((D_800A6200[i].from | oldBits) == D_800A6200[i].from) {
                    if ((D_800A6200[i].to | newBits) == D_800A6200[i].to) {
                        D_8008AFD0[action](watcher);
                    }
                }
            }
        }
    }
}

void func_1519F400(void *);

/* Message handler: frees the watcher when its character goes away (0), follows swaps (0x2D). */
void func_15158FA4(void *self, EffectMessage *msg, u8 msgId) {
    CharacterWatcher *watcher = self;
    Actor *first;

    if (msgId == 0) {
        if ((msg->first == watcher->character) || (msg->second.uniqueId == watcher->characterId)) {
            func_1516972C(self);
        }
    } else if (msgId == 0x2D) {
        first = msg->first;
        if (watcher->character == first) {
            watcher->character = msg->second.ptr;
            watcher->characterId = msg->secondId;
        } else {
            if (watcher->character == msg->second.ptr) {
                watcher->character = first;
                watcher->characterId = msg->firstId;
            }
        dummy_label_15158FA4: ; // matching: the empty labelled statement splits IDO's basic block
        }
    } else if (msgId == 4) {
        if ((msg->first == watcher->character) || (msg->second.uniqueId == watcher->characterId)) {
            func_1519F400(self);
        }
    }
}

s32 func_15159084(Actor *chr, u8 mode) {
    s32 ret;
    s32 flags;

    flags = chr->unk184 & 0x1F;
    if ((mode == 2) || (mode == 3)) {
        ret = 0;
    } else {
        if ((-10000.0f == chr->unk118) && ((flags & 0xA) == 0)) {
            ret = 1;
        } else if ((chr->unk118 < chr->y_position) || (chr->unk137 != 0)) { /* byte 0x137 */
            ret = 1;
        } else {
            ret = 0;
        }
    }
    return ret;
}

s32 func_15159120(Actor *chr, u8 mode) {
    s32 ret;

    if ((mode == 2) || (mode == 3)) {
        ret = 1;
    } else if (chr->unk180 < (chr->unk118 - 35.0f)) {
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

s32 func_15159184(Actor *chr, u8 mode) {
    s32 ret;

    if ((mode == 2) || (mode == 3)) {
        if (D_800C35EA != 1) {
            ret = 0;
        } else if ((chr->unk118 - 75.0f) < chr->y_position) {
            ret = 1;
        } else {
            ret = 0;
        }
    } else if ((chr->unk118 - 75.0f) < chr->y_position) {
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

/* Returns 0 if chr has not moved since lastPosition, 1 if it started moving, 2 if it kept moving. */
s32 func_15159230(Actor *chr, Vec3f *lastPosition, u8 prevState) {
    s32 ret;

    if ((chr->x_position != lastPosition->x) || (chr->y_position != lastPosition->y) ||
        (chr->z_position != lastPosition->z)) {
        if ((prevState == 1) || (prevState == 2)) {
            ret = 2;
        } else {
            ret = 1;
        }
    } else {
        ret = 0;
    }
    return ret;
}

extern u16 D_8008B040; /* two u8 ids, 0x3B and 0x3C */

/* Returns 3 on one of the D_8008B040 ids (mode 0 only), 2 in water, 1 if unk28 is set, else 4. */
s32 func_151592B8(Actor *chr, u8 mode) {
    s32 i;
    s32 found;
    union {
        u16 value;
        u8 bytes[2];
    } ids; // matching: the u8[2] table is copied with one lhu

    if (mode == 0) {
        found = 0;
        ids.value = D_8008B040;
        i = 1;
        do {
            if (chr->unk84.uh == ids.bytes[i--]) {
                found = 1;
            }
        } while ((found == 0) && (i >= 0));
    } else {
        found = 0;
    }
    if (found != 0) {
        i = 3;
    } else if (chr->in_water != 0) {
        i = 2;
    } else if (chr->unk28 != 0.0f) {
        i = 1;
    } else {
        i = 4;
    }
    return i;
}

/* Picks the watcher mode for chr; returns 0 if nothing applies. */
s32 func_15159370(Actor *chr, u8 *mode) {
    s32 ret;

    ret = 0;
    if (D_800BE616 != 0) {
        *mode = 0;
        ret = 1;
    }
    if (chr->unique_id == 1) {
        *mode = 0;
        ret = 1;
    }
    if ((chr->unk5 == 5) || (chr->unk5 == 2)) {
        *mode = 1;
        ret = 1;
    }
    switch (chr->id) {
    case 8:
        *mode = 2;
        ret = 1;
        break;
    case 10:
        *mode = 3;
        ret = 1;
        break;
    case 0x29:
    case 0x2A:
        *mode = 4;
        ret = 1;
        break;
    }
    return ret;
}

/* Node returned by func_1514ECE0; component 0x13 holds the character's watcher. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ CharacterWatcher *watcher;
} WatcherNode;

s32 func_1514ECE0(void *, s16, void *);

/*
 * Returns whether chr's watcher state matches the given field values (-1 matches anything).
 * chr+0x2F4 (unnamed in Actor) is its component list.
 */
u8 func_1515942C(Actor *chr, s16 state0, s16 state1, s16 state2, s16 moveState, s16 surfaceState) {
    CharacterWatcher *watcher;
    WatcherNode *node;
    u8 ret;
    s32 mask0;
    s32 mask1;
    s32 mask2;
    s32 maskMove;
    s32 maskSurface;
    s32 want;
    s32 have;

    ret = 0;
    if (func_1514ECE0(chr->attachments, 0x13, &node) == 0) {
        return 0;
    }
    watcher = node->watcher;
    if (moveState == -1) {
        maskMove = 0x1E00;
    } else {
        maskMove = 1 << (moveState + 9);
    }
    if (state2 == -1) {
        mask2 = 0x1C0;
    } else {
        mask2 = 1 << (state2 + 6);
    }
    if (state1 == -1) {
        mask1 = 0x38;
    } else {
        mask1 = 1 << (state1 + 3);
    }
    if (state0 == -1) {
        mask0 = 7;
    } else {
        mask0 = 1 << state0;
    }
    if (surfaceState == -1) {
        maskSurface = 0x3E000;
    } else {
        maskSurface = 1 << (surfaceState + 13);
    }
    have = STATE_BITS(watcher->state0, watcher->state1, watcher->state2, watcher->moveState,
                      watcher->surfaceState);
    want = maskSurface | mask0 | mask1 | mask2 | maskMove;
    if (want == (want | have)) {
        ret = 1;
    }
    return ret;
}

/* Ground query result written by func_15046C00. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 unk4[0x18];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 surface; /* 3 spawns a splash in the particle callbacks */
    /* 0x1E */ u8 pad1E[6];
} GroundHit;

/* Particle data at 0xA8 for the rising-and-swaying particles (func_151596BC, func_15159594). */
typedef struct {
    /* 0x00 */ f32 baseSize;
    /* 0x04 */ u8 widthPhase;
    /* 0x05 */ u8 heightPhase;
    /* 0x06 */ s8 widthPhaseStep;
    /* 0x07 */ s8 heightPhaseStep;
    /* 0x08 */ f32 widthWobble;
    /* 0x0C */ f32 heightWobble;
    /* 0x10 */ f32 maxRiseSpeed;
    /* 0x14 */ f32 swayPerSpeed;
    /* 0x18 */ u8 swayPhaseX;
    /* 0x19 */ u8 swayPhaseZ;
    /* 0x1A */ u8 swayPhaseStepX;
    /* 0x1B */ u8 swayPhaseStepZ;
    /* 0x1C */ f32 swayX;
    /* 0x20 */ f32 swayZ;
    /* 0x24 */ f32 ceiling;
} RiseMotion;

/* Particle data at 0xA8 for particles oscillating along a direction (func_1515A11C). */
typedef struct {
    /* 0x00 */ f32 baseSize;
    /* 0x04 */ u8 widthPhase;
    /* 0x05 */ u8 heightPhase;
    /* 0x06 */ s8 widthPhaseStep;
    /* 0x07 */ s8 heightPhaseStep;
    /* 0x08 */ f32 widthWobble;
    /* 0x0C */ f32 heightWobble;
    /* 0x10 */ Vec3f direction;
    /* 0x1C */ f32 angle;
    /* 0x20 */ f32 angleSpeed;
} OscillateMotion;

/* Particle data at 0xA8 for drifting particles that may land (func_1515A60C, func_1515A78C). */
typedef struct {
    /* 0x00 */ f32 baseSize;
    /* 0x04 */ u8 widthPhase;
    /* 0x05 */ u8 heightPhase;
    /* 0x06 */ s8 widthPhaseStep;
    /* 0x07 */ s8 heightPhaseStep;
    /* 0x08 */ f32 widthWobble;
    /* 0x0C */ f32 heightWobble;
    /* 0x10 */ u8 swayPhaseX;
    /* 0x11 */ u8 swayPhaseZ;
    /* 0x12 */ u8 swayPhaseStepX;
    /* 0x13 */ u8 swayPhaseStepZ;
    /* 0x14 */ f32 swayX;
    /* 0x18 */ f32 swayZ;
    /* 0x1C */ f32 damping;
    /* 0x20 */ f32 groundHeight; /* GROUND_UNKNOWN: query func_15046C00 each frame */
} DriftMotion;

#define GROUND_UNKNOWN -99999.8984375f

/* Particle object created by func_151303BC / func_15130280 from a ParticleDesc6C. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1E];
    /* 0x2B */ u8 alpha;
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ f32 width;
    /* 0x3C */ f32 height;
    /* 0x40 */ Vec3f position;
    /* 0x4C */ Vec3f offset;
    /* 0x58 */ Vec3f velocity;
    /* 0x64 */ u8 pad64[4];
    /* 0x68 */ u32 flags; /* 0x2000: dies above RiseMotion.ceiling */
    /* 0x6C */ u8 pad6C[0x14];
    /* 0x80 */ GroundHit ground;
    /* 0xA4 */ u8 padA4[4];
    /* 0xA8 */ union {
        RiseMotion rise;
        OscillateMotion oscillate;
        DriftMotion drift;
    } motion;
} Particle;

/* Second argument of the particle update callbacks; only unk4 is read here. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ f32 unk4;
} ParticleUpdateArg;

s32 func_151596BC(Particle *particle, ParticleUpdateArg *arg1);
u8 func_151D8E20(void);
s32 func_15046C00(Vec3f *pos, u16 arg1, f32 maxHeight, GroundHit *hit);
s32 func_1514672C(Vec3f *pos);

/* Update callback: rises and sways; dies on reaching the ground hit above it (splashing on surface 3). */
u8 func_15159594(Particle *particle, ParticleUpdateArg *arg1) {
    u8 alive;
    s32 found;

    alive = 1;
    if (func_151596BC(particle, arg1) != 0) {
        if (particle->velocity.y > 0.0f) {
            Vec3f pos;

            pos.x = particle->position.x + particle->offset.x;
            pos.y = arg1->unk4;
            pos.z = particle->position.z + particle->offset.z;
            if (func_1514672C(&pos) == 0) {
                return 0;
            }
            found = func_15046C00(&pos, 0, particle->position.y, &particle->ground);
            if (found != 0) {
                pos.y = particle->ground.height;
                if (particle->ground.surface == 3) {
                    func_151DBCBC(func_151D8E20(), particle->motion.rise.baseSize * 0.200000003f, 0xFF,
                                  (s32)particle->ground.unk4, &pos, particle->unkC, particle->unk1);
                }
                alive = 0;
            }
        }
    } else {
        alive = 0;
    }
    return alive;
}

/* Update callback: wobbles the size, caps the rise speed and sways with it; dies above the ceiling. */
s32 func_151596BC(Particle *particle, ParticleUpdateArg *arg1) {
    RiseMotion *rise;
    f32 sin0;
    f32 sin1;
    u8 alive;
    f32 sway;

    rise = &particle->motion.rise;
    alive = 1;
    rise->widthPhase += rise->widthPhaseStep * D_800BE9E4;
    rise->heightPhase += rise->heightPhaseStep * D_800BE9E4;
    sin0 = func_151423D8(rise->widthPhase - 0x40);
    sin1 = func_151423D8(rise->heightPhase - 0x40);
    particle->width = rise->baseSize + (rise->widthWobble * sin0);
    particle->height = rise->baseSize + (rise->heightWobble * sin1);

    if (0.0f < particle->velocity.y) {
        if (rise->maxRiseSpeed < particle->velocity.y) {
            particle->velocity.y = rise->maxRiseSpeed;
        }
        sway = particle->velocity.y * rise->swayPerSpeed;
        rise->swayPhaseX += rise->swayPhaseStepX * D_800BE9E4;
        rise->swayPhaseZ += rise->swayPhaseStepZ * D_800BE9E4;
        sin0 = func_151423D8(rise->swayPhaseX - 0x40);
        sin1 = func_151423D8(rise->swayPhaseZ - 0x40);
        particle->offset.x = (rise->swayX * sin0) * sway;
        particle->offset.z = (rise->swayZ * sin1) * sway;
    }

    if (particle->flags & 0x2000) {
        if (rise->ceiling < (particle->offset.y + particle->position.y)) {
            alive = 0;
        }
    }
    return alive;
}

/* matching: the first 0x6C bytes of ParticleDesc; a full ParticleDesc on the stack changes the frame. */
typedef struct ParticleDesc6C {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode; /* low byte: D_800A4AC8 index, high byte: func_1513F4E4 arg */
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;      /* 16.16 animation frame */
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;     /* D_80090B60 index */
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ Vec3f pos;
    /* 0x3C */ Vec3f offset;
    /* 0x48 */ Vec3f vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ u32 flags;      /* PARTICLE_* */
    /* 0x5C */ struct ParticleViewMask *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* D_80089760 index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* D_80089800 index, -1 for none */
    /* 0x64 */ s8 drawFunc;    /* D_80089804 index, -1 for none */
    /* 0x65 */ u8 handlerSet;  /* D_80089814/D_80089844/D_80089878 index */
    /* 0x66 */ u8 views;       /* bit per view the particle is drawn in */
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 minSize;
    /* 0x6A */ s16 unk6A;
} ParticleDesc6C;

Particle *func_151303BC(ParticleDesc6C *spawn, s32 arg1, s32 motionSize);

/* Spawns one rising particle (func_151596BC family) at pos with the given velocity. */
Particle *func_15159890(Vec3f *pos, Vec3f *velocity, s32 arg2, s32 arg3) {
    f32 size;
    ParticleDesc6C spawn;
    Particle *particle;
    RiseMotion rise;

    spawn.texture = 0x2F;
    spawn.renderMode = 0xC01;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.b2 = 0;
    spawn.g2 = 0;
    spawn.r2 = 0;
    spawn.a = 0;
    spawn.b = 0;
    spawn.g = 0;
    spawn.r = 0;
    spawn.frameRate = 0;
    spawn.frame = 0;
    spawn.brightness = 0xFF;
    spawn.scaleStart = 0;
    spawn.scaleRate = 0.0f;
    spawn.flags = 0x3207;
    spawn.primColorMode = 5;
    spawn.envColorMode = 5;
    spawn.updateFunc = 4;
    spawn.deathFunc = -1;
    spawn.pos = *pos;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    rise.widthPhase = 0;
    rise.heightPhase = 0;
    rise.swayPhaseX = 0;
    rise.swayPhaseZ = 0;
    rise.ceiling = 1000.0f;
    rise.maxRiseSpeed = 4.0f;
    rise.swayPerSpeed = 0.25f;
    spawn.vel = *velocity;

    if (func_150ADA20() & 1) {
        spawn.flags |= 0x40;
    }
    if (func_150ADA20() & 1) {
        spawn.flags |= 0x80;
    }

    spawn.timer = (func_150ADA20() % 25U) + 30;
    spawn.fadeStart = 30;
    spawn.fadeRate = 8;
    spawn.alpha = (func_150ADA20() % 101U) + 100;
    size = (func_150ADA68() * 20.0f) + 49.0f;
    spawn.width = size;
    rise.baseSize = size;
    spawn.height = size;
    rise.widthPhaseStep = (func_150ADA20() % 5U) + 4;
    rise.heightPhaseStep = (func_150ADA20() % 5U) + 4;
    rise.widthWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * rise.baseSize;
    rise.heightWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * rise.baseSize;
    rise.swayPhaseStepX = (func_150ADA20() % 5U) + 4;
    rise.swayPhaseStepZ = (func_150ADA20() % 5U) + 4;
    rise.swayX = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * rise.baseSize;
    rise.swayZ = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * rise.baseSize;
    spawn.gravity = ((func_150ADA68() * 254.0f) + 108.0f) * 0.001000000047f;

    particle = func_151303BC(&spawn, 2, sizeof(RiseMotion));
    if (particle != NULL) {
        memcpy(&particle->motion, &rise, sizeof(RiseMotion));
    }
    return particle;
}

/* Spawns one stationary rising particle at (x, y, z). */
void func_15159BB0(f32 x, f32 y, f32 z, s32 arg3, s32 arg4, s32 arg5, EffectHeader *obj) {
    Vec3f pos;
    Vec3f velocity;

    pos.x = x;
    pos.y = y;
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    velocity.z = 0.0f;
    pos.z = z;
    func_15159890(&pos, &velocity, obj->owner, obj->kind);
}

/* Emitter data at 0x28 of the func_15159C08 emitter. */
typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ Vec3f velocity;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 radius;
    /* 0x20 */ f32 angleX;
    /* 0x24 */ f32 angleY;
    /* 0x28 */ f32 pending; /* particles owed; one is spawned per whole unit */
    /* 0x2C */ f32 rate;
    /* 0x30 */ f32 rateRandom;
} DiskEmitter;

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ DiskEmitter emitter;
} DiskEmitterObject;

extern u8 D_800BE9EB;
extern float sinf(float);
extern float cosf(float);
void func_150A7960(MtxF mtx, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/* Emitter update: spawns oscillating particles at random points of a rotated disk. */
void func_15159C08(DiskEmitterObject *obj) {
    DiskEmitter *emitter;
    Particle *particle;

    emitter = &obj->emitter;
    if (func_151454BC(D_800BE9EB, 3000.0f, (Vec3f *)emitter)) {
        emitter->pending += (emitter->rate + (func_150ADA68() * emitter->rateRandom)) * D_800BE9A4;
        if (emitter->pending > 1.0f) {
            ParticleDesc6C spawn;
            OscillateMotion oscillate;
            f32 angle;
            f32 radius;
            f32 x;
            f32 z;
            f32 scale;
            f32 invScale;
            MtxF mtx;
            f32 dist;
            f32 fadeTime;
            f32 size;

            spawn.texture = 0x2F;
            spawn.renderMode = 0xC01;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0;
            spawn.b2 = 0;
            spawn.g2 = 0;
            spawn.r2 = 0;
            spawn.a = 0;
            spawn.b = 0;
            spawn.g = 0;
            spawn.r = 0;
            spawn.frameRate = 0;
            spawn.frame = 0;
            spawn.brightness = 0xFF;
            spawn.scaleStart = 0;
            spawn.scaleRate = 0.0f;
            spawn.flags = 0x1205;
            spawn.primColorMode = 5;
            spawn.envColorMode = 5;
            spawn.updateFunc = 12;
            spawn.deathFunc = -1;
            spawn.pos = emitter->position;
            spawn.offset.x = 0.0f;
            spawn.offset.y = 0.0f;
            spawn.offset.z = 0.0f;
            oscillate.widthPhase = 0;
            oscillate.heightPhase = 0;
            spawn.gravity = 0.0f;
            do {
                angle = func_150ADA68() * 6.283185482f;
                radius = func_150ADA68() * emitter->radius;
                x = sinf(angle) * radius;
                z = cosf(angle) * radius;
                func_150A8050(mtx, emitter->angleX, emitter->angleY, 0.0f);
                func_150A7960(mtx, x, 0.0f, z, &oscillate.direction.x, &oscillate.direction.y,
                              &oscillate.direction.z);
                oscillate.angle = func_150ADA68() * 6.283185482f;
                oscillate.angleSpeed = ((func_150ADA68() * 320.0f) + -160.0f) * 0.001000000047f;
                {
                    s16 pad; // matching: IDO's frame has an unused slot here

                    scale = ((func_150ADA68() * 161.0f) + 446.0f) * 0.009999999776f;
                    invScale = 1.0f / scale;
                    dist = emitter->unk18 * invScale;
                    fadeTime = ((func_150ADA68() * 6596.0f) + 2402.0f) * 0.1000000015f * invScale;
                    spawn.vel.x = emitter->velocity.x * scale;
                    spawn.vel.y = emitter->velocity.y * scale;
                    spawn.vel.z = emitter->velocity.z * scale;
                    if (dist > 500.0f) {
                        spawn.timer = 500;
                    } else {
                        spawn.timer = (fadeTime * 0.5f) + dist;
                    }
                    spawn.fadeStart = fadeTime;
                    spawn.fadeRate = 0xFF / spawn.fadeStart;
                    spawn.flags &= ~0xC0;
                    if (func_150ADA20() & 1) {
                        spawn.flags |= 0x40;
                    }
                    if (func_150ADA20() & 1) {
                        spawn.flags |= 0x80;
                    }
                    spawn.alpha = (func_150ADA20() % 51U) + 124;
                    size = (func_150ADA68() * 100.0f) + 240.0f;
                    spawn.width = size;
                    oscillate.baseSize = size;
                    spawn.height = size;
                    oscillate.widthPhaseStep = (func_150ADA20() % 5U) + 4;
                    oscillate.heightPhaseStep = (func_150ADA20() % 5U) + 4;
                    oscillate.widthWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * oscillate.baseSize;
                    oscillate.heightWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * oscillate.baseSize;
                    particle = func_151303BC(&spawn, 2, sizeof(OscillateMotion));
                    if (particle != NULL) {
                        memcpy(&particle->motion, &oscillate, sizeof(OscillateMotion));
                    }
                    emitter->pending -= 1.0f;
                }
            } while (emitter->pending > 1.0f);
        }
    }
}

f32 func_15144B68(f32 angle);

/* Update callback: wobbles the size and moves the offset along direction by sin(angle). */
s32 func_1515A11C(Particle *particle, ParticleUpdateArg *arg1) {
    OscillateMotion *osc;
    f32 sin0;
    f32 sin1;

    osc = &particle->motion.oscillate;
    osc->widthPhase += osc->widthPhaseStep * D_800BE9E4;
    osc->heightPhase += osc->heightPhaseStep * D_800BE9E4;
    sin0 = func_151423D8(osc->widthPhase - 0x40);
    sin1 = func_151423D8(osc->heightPhase - 0x40);
    particle->width = osc->baseSize + (osc->widthWobble * sin0);
    particle->height = osc->baseSize + (osc->heightWobble * sin1);
    if ((osc && osc) && osc) { // matching: these dead tests of osc steer IDO's schedule of the angle update
    }
    osc->angle += osc->angleSpeed * D_800BE9A4;
    osc->angle = func_15144B68(osc->angle);
    sin0 = sinf(osc->angle);
    particle->offset.x = osc->direction.x * sin0;
    particle->offset.y = osc->direction.y * sin0;
    particle->offset.z = osc->direction.z * sin0;
    return 1;
}

void *func_15130280(ParticleDesc6C *spawn, u8 arg1, GroundHit *ground, s32 motionSize, u8 arg4, s32 arg5);

/* Spawns one drifting particle; with no horizontal velocity it lands on the ground below pos. */
Particle *func_1515A238(Vec3f *pos, Vec3f *velocity, f32 arg2, f32 damping, GroundHit *ground, f32 size,
                        s16 lifetime, u8 alpha, u8 arg8, u8 arg9, s32 argA) {
    Particle *particle;
    s32 pad; // matching: 4 unused frame bytes sit above spawn
    ParticleDesc6C spawn;
    DriftMotion drift;
    GroundHit hit;
    s32 flipY;
    s32 flipX;

    spawn.texture = 0x2F;
    spawn.renderMode = 0x2203;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.b2 = 0;
    spawn.g2 = 0;
    spawn.r2 = 0;
    spawn.a = 0;
    spawn.b = 0;
    spawn.g = 0;
    spawn.r = 0;
    spawn.frameRate = 0;
    spawn.frame = 0;
    spawn.brightness = 0xFF;
    spawn.scaleStart = 0;
    spawn.scaleRate = 0.0f;
    flipX = (func_150ADA20() & 1) ? 0x40 : 0;
    flipY = (func_150ADA20() & 1) ? 0x80 : 0;
    spawn.flags = flipY | 0x1007 | flipX | 0x8000 | 0x4000;
    spawn.primColorMode = 6;
    spawn.envColorMode = 8;
    if (arg8) {
        spawn.updateFunc = 0x15;
    } else {
        spawn.updateFunc = 0x14;
    }
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.pos = *pos;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    drift.baseSize = size;
    spawn.width = size;
    spawn.height = size;
    drift.widthPhase = func_150ADA20() & 0xFF;
    drift.heightPhase = func_150ADA20() & 0xFF;
    drift.widthPhaseStep = (func_150ADA20() % 5U) + 4;
    drift.heightPhaseStep = (func_150ADA20() % 5U) + 4;
    drift.widthWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * drift.baseSize;
    drift.heightWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * drift.baseSize;
    drift.swayPhaseX = 0;
    drift.swayPhaseZ = 0;
    drift.swayPhaseStepX = (func_150ADA20() % 5U) + 4;
    drift.swayPhaseStepZ = (func_150ADA20() % 5U) + 4;
    drift.swayX = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * drift.baseSize;
    drift.swayZ = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * drift.baseSize;
    spawn.vel = *velocity;
    spawn.handlerSet = 0;
    spawn.timer = lifetime;
    spawn.alpha = alpha;
    spawn.gravity = arg2;
    drift.damping = damping;
    if (velocity->x != 0 || velocity->z != 0) {
        drift.groundHeight = GROUND_UNKNOWN;
    } else {
        if (ground != NULL) {
            hit = *ground;
        } else {
            hit.unk1C = 0;
        }
        if (func_1514672C(pos)) {
            if (func_15046C00(pos, 0, 19500.0f, &hit)) {
                ground = &hit;
                drift.groundHeight = hit.height;
            } else {
                drift.groundHeight = 10000.0f;
            }
        } else {
            drift.groundHeight = 10000.0f;
        }
    }
    particle = func_15130280(&spawn, 2, ground, sizeof(DriftMotion), arg9, argA);
    if (particle != NULL) {
        memcpy(&particle->motion, &drift, sizeof(DriftMotion));
    }
    return particle;
}

/* Update callback: wobbles the size, sways the offset and damps the velocity. */
s32 func_1515A60C(Particle *particle, ParticleUpdateArg *arg1) {
    DriftMotion *drift;
    s32 i;

    drift = &particle->motion.drift;
    particle->width = (func_151423D8(drift->widthPhase - 0x40) * drift->widthWobble) + drift->baseSize;
    particle->height = (func_151423D8(drift->heightPhase - 0x40) * drift->heightWobble) + drift->baseSize;
    particle->offset.x = func_151423D8(drift->swayPhaseX - 0x40) * drift->swayX;
    particle->offset.z = func_151423D8(drift->swayPhaseZ - 0x40) * drift->swayZ;

    drift->widthPhase += drift->widthPhaseStep * D_800BE9E4;
    drift->heightPhase += drift->heightPhaseStep * D_800BE9E4;
    drift->swayPhaseX += drift->swayPhaseStepX * D_800BE9E4;
    drift->swayPhaseZ += drift->swayPhaseStepZ * D_800BE9E4;

    for (i = D_800BE9E4; i > 0; i--) {
        particle->velocity.x *= drift->damping;
        particle->velocity.y *= drift->damping;
        particle->velocity.z *= drift->damping;
    }
    return 1;
}

/* Update callback: drifts, then dies on reaching the ground (sometimes splashing on surface 3). */
u8 func_1515A78C(Particle *particle, ParticleUpdateArg *arg1) {
    DriftMotion *drift;
    u8 alive;
    Vec3f pos;

    alive = 1;
    drift = &particle->motion.drift;
    func_1515A60C(particle, arg1);
    if (particle->velocity.y < 0.0f) {
        return 1;
    }
    pos.x = particle->position.x + particle->offset.x;
    pos.y = arg1->unk4;
    pos.z = particle->position.z + particle->offset.z;
    if (drift->groundHeight == GROUND_UNKNOWN) {
        if (func_1514672C(&pos) == 0) {
            return 0;
        }
        if (func_15046C00(&pos, 0, particle->position.y, &particle->ground) != 0) {
            pos.y = particle->ground.height;
            alive = 0;
        }
    } else if (drift->groundHeight <= particle->position.y) {
        pos.y = drift->groundHeight;
        alive = 0;
    }
    if (alive == 0) {
        if (func_150ADA68() < 0.200000003f) {
            if (particle->ground.surface == 3) {
                func_151DBCBC(func_151D8E20(), drift->baseSize * 0.200000003f, particle->alpha,
                              (s32)particle->ground.unk4, &pos, particle->unkC, particle->unk1);
            }
        }
    }
    return alive;
}

/* Returns the address of chr's watcher unk34 block through *out; 0 if chr has no watcher. */
s32 func_1515A920(Actor *chr, u8 **out) {
    WatcherNode *node;
    CharacterWatcher *watcher;

    if (func_1514ECE0(chr->attachments, 0x13, &node) == 0) {
        return 0;
    }
    watcher = node->watcher;
    *out = watcher->unk34;
    return 1;
}

/* Settings an emitter (func_1515AB88) spawns its rising particles with. */
typedef struct {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 rateRandom;
    /* 0x08 */ f32 sizeBase;
    /* 0x0C */ f32 sizeRandom;
    /* 0x10 */ f32 unk54Base;
    /* 0x14 */ f32 unk54Random;
    /* 0x18 */ f32 maxRiseSpeed;
    /* 0x1C */ f32 swayPerSpeed;
    /* 0x20 */ u8 alphaBase;
    /* 0x21 */ u8 alphaRange;
    /* 0x22 */ u8 color[3];
    /* 0x25 */ u8 pad25[3];
} EmitterSettings;

typedef struct {
    s16 base;
    s16 range;
} TypeRange;

/* Data at 0x80 of a spawner object: emitters get a random type in [types.base, types.base + types.range]. */
typedef struct {
    /* 0x00 */ TypeRange types;
    /* 0x04 */ EmitterSettings settings;
} SpawnerData;

/* Spawner object created by func_151A8B20 (func_1515A974). */
typedef struct {
    /* 0x00 */ u8 pad0[0x1];
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x73];
    /* 0x80 */ SpawnerData data;
} Spawner;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ SpawnerData data;
} SpawnerTemplate;

extern SpawnerTemplate D_800A636C[];

typedef struct {
    /* 0x00 */ u8 pad0[0x1F];
    /* 0x1F */ u8 variant; /* index into D_800A636C */
} SpawnerSource;

Spawner *func_151A8B20(struct183 *info, s16 arg1, s32 dataSize, u8 arg3, s32 arg4);

/* Creates a spawner object for src from template D_800A636C[src->variant] (variant 0 only). */
// matching: volatile makes IDO reload src from its stack slot
void func_1515A974(SpawnerSource *volatile src, s32 arg1) {
    struct183 info;
    s32 pad[2]; // matching: 8 unused frame bytes between data and info
    SpawnerData data;
    Spawner *spawner;
    s32 variant;

    variant = src->variant;
    if (variant <= 0) {
        info.unk0 = 0xF;
        info.unk4 = (s32)src;
        info.unk8 = D_800A636C[variant].unk0;
        info.unkC = D_800A636C[variant].unk4;
        info.unk10 = 0.0f;
        info.unk14 = 0.0f;
        info.unk18 = 0.0f;
        info.unk1C = 0.0f;
        info.unk20 = 0.0f;
        info.unk24 = 0;
        info.unk25 = -1;
        info.unk26 = 2;
        data.types = D_800A636C[variant].data.types;
        data.settings = D_800A636C[variant].data.settings;
        spawner = func_151A8B20(&info, -1, sizeof(SpawnerData), 0xFF, 0);
        if (spawner != NULL) {
            memcpy(&spawner->data, &data, sizeof(SpawnerData));
        }
    }
}

/* Emitter data at 0x28 of the func_1515AB88 emitter. */
typedef struct {
    /* 0x00 */ f32 pending; /* particles owed; one is spawned per whole unit */
    /* 0x04 */ Vec3f position;
    /* 0x10 */ f32 ceiling;
    /* 0x14 */ EmitterSettings settings;
} RiseEmitter;

typedef struct {
    /* 0x00 */ u8 pad0[0x1];
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ RiseEmitter emitter;
} RiseEmitterObject;

/* Spawner callback: creates an emitter of a random type at pos that spawns particles up to ceiling. */
void func_1515AA84(Spawner *spawner, Vec3f *pos, f32 ceiling, s32 arg3) {
    RiseEmitter emitter;
    SpawnerData *data;
    RiseEmitterObject *obj;

    data = &spawner->data;
    emitter.pending = 0.0f;
    emitter.position = *pos;
    emitter.ceiling = ceiling;
    emitter.settings = data->settings;
    obj = (RiseEmitterObject *)func_15149130(func_150ADA20() % (data->types.range + 1) + data->types.base,
                                             -1, 0x3F, -1, 1, 0, sizeof(RiseEmitter), spawner->unkC,
                                             spawner->unk1);
    if (obj != NULL) {
        memcpy(&obj->emitter, &emitter, sizeof(RiseEmitter));
    }
}

/* Emitter update: spawns rising particles (func_151596BC family) at the emitter's position. */
void func_1515AB88(RiseEmitterObject *obj) {
    RiseEmitter *emitter;

    emitter = &obj->emitter;
    emitter->pending += (emitter->settings.rate + (func_150ADA68() * emitter->settings.rateRandom)) * D_800BE9A4;
    if (emitter->pending > 1.0f) {
        s32 pad; // matching: 4 unused frame bytes sit above spawn
        ParticleDesc6C spawn;
        RiseMotion rise;
        Particle *particle;
        s32 flipY;
        s32 flipX;

        rise.widthPhase = 0;
        rise.heightPhase = 0;
        rise.swayPhaseX = 0;
        rise.swayPhaseZ = 0;
        rise.maxRiseSpeed = emitter->settings.maxRiseSpeed;
        rise.swayPerSpeed = emitter->settings.swayPerSpeed;
        rise.ceiling = emitter->ceiling;
        spawn.texture = 0x2F;
        spawn.renderMode = 0x4403;
        spawn.unk0 = 0x200005;
        spawn.unk4 = 0x9F0600;
        spawn.timer = 300;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.r2 = 0xFF;
        spawn.g2 = 0xFF;
        spawn.b2 = 0xFF;
        spawn.a = 0xFF;
        spawn.r = emitter->settings.color[0];
        spawn.g = emitter->settings.color[1];
        spawn.b = emitter->settings.color[2];
        spawn.brightness = 0xFF;
        spawn.pos = emitter->position;
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
        spawn.fadeStart = 1;
        spawn.fadeRate = 0xFF;
        spawn.scaleStart = 1;
        spawn.scaleRate = 1.0f;
        spawn.primColorMode = 6;
        spawn.envColorMode = 9;
        spawn.updateFunc = 4;
        spawn.deathFunc = -1;
        spawn.drawFunc = -1;
        spawn.handlerSet = 0;
        spawn.offset.x = 0.0f;
        spawn.offset.y = 0.0f;
        spawn.offset.z = 0.0f;
        do {
            rise.widthPhaseStep = (func_150ADA20() % 5U) + 4;
            rise.heightPhaseStep = (func_150ADA20() % 5U) + 4;
            rise.baseSize = spawn.width = spawn.height =
                (func_150ADA68() * emitter->settings.sizeRandom) + emitter->settings.sizeBase;
            rise.widthWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * rise.baseSize;
            rise.heightWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * rise.baseSize;
            rise.swayPhaseStepX = (func_150ADA20() % 5U) + 4;
            rise.swayPhaseStepZ = (func_150ADA20() % 5U) + 4;
            rise.swayX = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * rise.baseSize;
            rise.swayZ = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * rise.baseSize;
            spawn.gravity = (func_150ADA68() * emitter->settings.unk54Random) + emitter->settings.unk54Base;
            spawn.alpha = (func_150ADA20() % (emitter->settings.alphaRange + 1)) + emitter->settings.alphaBase;
            flipX = (func_150ADA20() & 1) ? 0x40 : 0;
            flipY = (func_150ADA20() & 1) ? 0x80 : 0;
            spawn.flags = flipY | 6 | flipX | 0x8000 | 0x4000 | 0x2000 | 0x1000;
            particle = func_15130280(&spawn, 2, NULL, sizeof(RiseMotion), obj->unkC, obj->unk1);
            if (particle != NULL) {
                memcpy(&particle->motion, &rise, sizeof(RiseMotion));
            }
            emitter->pending -= 1.0f;
        } while (emitter->pending > 1.0f);
    }
}
