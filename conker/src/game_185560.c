#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

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
s32 func_15159370();

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
    s32 pad;
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
    f32 mf[4][4];

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

typedef struct {
    f32 m[4][4];
} MtxF;

extern MtxF D_8008AE18;
extern f32 D_800A6070;

/* Draw callback: scales the fixed matrix D_8008AE18 by data.size and translates to the actor's position. */
s32 func_15158920(Mtx *mtx, EffectActor *actor) {
    f32 scale;
    s32 pad; // matching: IDO's frame has an unused slot here
    MtxF mf;

    mf = D_8008AE18;
    scale = actor->data.size * D_800A6070;
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
    /* 0x00 */ struct127 *first;
    /* 0x04 */ union {
        struct127 *ptr;
        u8 uniqueId; /* msgId 0: unique id of `first` */
    } second;
    /* 0x08 */ u8 firstId;
    /* 0x09 */ u8 secondId;
} EffectMessage;

/* Message handler: follows owner swaps (0x2D) and drops the owner when it goes away (0). */
void func_15158B3C(EffectActor *actor, EffectMessage *msg, u8 msgId) {
    struct127 *first;

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
    /* 0x18 */ struct127 *character;
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

s32 func_15159370(struct127 *chr, u8 *mode);
void func_1514EDF0(CharacterWatcher *watcher, struct127 *chr);

/* Creates a watcher for chr; returns NULL when chr is NULL or has no watchable mode. */
CharacterWatcher *func_15158BD0(struct127 *chr, u8 enableActions, s32 extraSize) {
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

s32 func_15159084(struct127 *chr, u8 mode);
s32 func_15159120(struct127 *chr, u8 mode);
s32 func_15159184(struct127 *chr, u8 mode);
s32 func_15159230(struct127 *chr, Vec3f *lastPosition, u8 prevState);
s32 func_151592B8(struct127 *chr, u8 mode);

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
    struct127 *first;

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

s32 func_15159084(struct127 *chr, u8 mode) {
    s32 ret;
    s32 flags;

    flags = chr->unk184 & 0x1F;
    if ((mode == 2) || (mode == 3)) {
        ret = 0;
    } else {
        if ((-10000.0f == chr->unk118) && ((flags & 0xA) == 0)) {
            ret = 1;
        } else if ((chr->unk118 < chr->y_position) || (((u8 *)&chr->pad134)[3] != 0)) { /* byte 0x137 */
            ret = 1;
        } else {
            ret = 0;
        }
    }
    return ret;
}

s32 func_15159120(struct127 *chr, u8 mode) {
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

s32 func_15159184(struct127 *chr, u8 mode) {
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
s32 func_15159230(struct127 *chr, Vec3f *lastPosition, u8 prevState) {
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
s32 func_151592B8(struct127 *chr, u8 mode) {
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
s32 func_15159370(struct127 *chr, u8 *mode) {
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
 * chr+0x2F4 (unnamed in struct127) is its component list.
 */
u8 func_1515942C(struct127 *chr, s16 state0, s16 state1, s16 state2, s16 moveState, s16 surfaceState) {
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
    if (func_1514ECE0(*(void **)&chr->pad2F0[4], 0x13, &node) == 0) {
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

typedef struct {
    f32 unk0;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    s8 unk7;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
} Sub151596BC;

typedef struct {
    u8 pad0[0x38];
    f32 unk38;
    f32 unk3C;
    u8 pad40[0x4];
    f32 unk44;
    u8 pad48[0x4];
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    u8 pad58[0x4];
    f32 unk5C;
    u8 pad60[0x8];
    u32 unk68;
    u8 pad6C[0x3C];
    Sub151596BC sub;
} Struct151596BC;

s32 func_151596BC(Struct151596BC *arg0, s32 arg1);
u8 func_151D8E20(void);
s32 func_15046C00(f32 *, u16, f32, u8 *);
s32 func_1514672C(f32 *);
extern f32 D_800A63A4;

u8 func_15159594(u8 *arg0, s32 arg1) {
    u8 result;
    s32 found;

    result = 1;
    if (func_151596BC((Struct151596BC *)arg0, arg1) != 0) {
        if (*(f32 *)(arg0 + 0x5C) > 0.0f) {
            f32 position[3];

            position[0] = *(f32 *)(arg0 + 0x4C) + *(f32 *)(arg0 + 0x40);
            position[1] = *(f32 *)((u8 *)arg1 + 4);
            position[2] = *(f32 *)(arg0 + 0x54) + *(f32 *)(arg0 + 0x48);
            if (func_1514672C(position) == 0) {
                return 0;
            }
            found = func_15046C00(position, 0, *(f32 *)(arg0 + 0x44), arg0 + 0x80);
            if (found != 0) {
                position[1] = *(f32 *)(arg0 + 0x80);
                if (*(u8 *)(arg0 + 0x9D) == 3) {
                    func_151DBCBC(func_151D8E20(), *(f32 *)(arg0 + 0xA8) * D_800A63A4,
                                  0xFF, (s32)(arg0 + 0x84), position,
                                  *(u8 *)(arg0 + 0xC), *(u8 *)(arg0 + 1));
                }
                result = 0;
            }
        }
    } else {
        result = 0;
    }
    return result;
}


s32 func_151596BC(Struct151596BC *arg0, s32 arg1) {
    Sub151596BC *sub;
    f32 temp0;
    f32 temp1;
    u8 ret;
    f32 temp2;

    sub = &arg0->sub;
    ret = 1;
    arg0->sub.unk4 += arg0->sub.unk6 * D_800BE9E4;
    arg0->sub.unk5 += arg0->sub.unk7 * D_800BE9E4;
    temp0 = func_151423D8(arg0->sub.unk4 - 0x40);
    temp1 = func_151423D8(sub->unk5 - 0x40);
    arg0->unk38 = sub->unk0 + (sub->unk8 * temp0);
    arg0->unk3C = sub->unk0 + (sub->unkC * temp1);

    if (0.0f < arg0->unk5C) {
        if (sub->unk10 < arg0->unk5C) {
            arg0->unk5C = sub->unk10;
        }
        temp2 = arg0->unk5C * sub->unk14;
        sub->unk18 += sub->unk1A * D_800BE9E4;
        sub->unk19 += sub->unk1B * D_800BE9E4;
        temp0 = func_151423D8(sub->unk18 - 0x40);
        temp1 = func_151423D8(sub->unk19 - 0x40);
        arg0->unk4C = (sub->unk1C * temp0) * temp2;
        arg0->unk54 = (sub->unk20 * temp1) * temp2;
    }

    if (arg0->unk68 & 0x2000) {
        if (sub->unk24 < (arg0->unk50 + arg0->unk44)) {
            ret = 0;
        }
    }
    return ret;
}

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
} Vec15159890;

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 unk08;
    /* 0x0A */ s16 unk0A;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ Vec15159890 unk30;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ Vec15159890 unk48;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ u8 pad5C[0x4];
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ u8 pad64[0x8];
} Params15159890;

extern f32 D_800A63A8;
extern f32 D_800A63AC;
extern f32 D_800A63B0;
extern f32 D_800A63B4;
extern f32 D_800A63B8;
extern f32 D_800A63BC;
extern f32 D_800A63C0;

void *func_151303BC(Params15159890 *, s32, s32);

void *func_15159890(f32 *arg0, f32 *arg1, s32 arg2, s32 arg3) {
    f32 temp_f2;
    Params15159890 params;
    void *temp_v0;
    Sub151596BC extra;

    params.unk1D = 0x2F;
    params.unk08 = 0xC01;
    params.unk00 = 0x200005;
    params.unk04 = 0;
    params.unk1A = 0;
    params.unk19 = 0;
    params.unk18 = 0;
    params.unk17 = 0;
    params.unk16 = 0;
    params.unk15 = 0;
    params.unk14 = 0;
    params.unk10 = 0;
    params.unk0C = 0;
    params.unk1C = 0xFF;
    params.unk22 = 0;
    params.unk24 = 0.0f;
    params.unk58 = 0x3207;
    params.unk60 = 5;
    params.unk61 = 5;
    params.unk62 = 4;
    params.unk63 = -1;
    params.unk30 = *(Vec15159890 *)arg0;
    params.unk3C = 0.0f;
    params.unk40 = 0.0f;
    params.unk44 = 0.0f;
    extra.unk4 = 0;
    extra.unk5 = 0;
    extra.unk18 = 0;
    extra.unk19 = 0;
    extra.unk24 = 1000.0f;
    extra.unk10 = 4.0f;
    extra.unk14 = 0.25f;
    params.unk48 = *(Vec15159890 *)arg1;

    if (func_150ADA20() & 1) {
        params.unk58 |= 0x40;
    }
    if (func_150ADA20() & 1) {
        params.unk58 |= 0x80;
    }

    params.unk0A = (func_150ADA20() % 25U) + 0x1E;
    params.unk1E = 0x1E;
    params.unk20 = 8;
    params.unk1B = (func_150ADA20() % 101U) + 0x64;
    temp_f2 = (func_150ADA68() * 20.0f) + 49.0f;
    params.unk28 = temp_f2;
    extra.unk0 = temp_f2;
    params.unk2C = temp_f2;
    extra.unk6 = (func_150ADA20() % 5U) + 4;
    extra.unk7 = (func_150ADA20() % 5U) + 4;
    extra.unk8 = ((func_150ADA68() * 0.25f) + D_800A63A8) * extra.unk0;
    extra.unkC = ((func_150ADA68() * 0.25f) + D_800A63AC) * extra.unk0;
    extra.unk1A = (func_150ADA20() % 5U) + 4;
    extra.unk1B = (func_150ADA20() % 5U) + 4;
    extra.unk1C = ((func_150ADA68() * D_800A63B0) + D_800A63B4) * extra.unk0;
    extra.unk20 = ((func_150ADA68() * D_800A63B8) + D_800A63BC) * extra.unk0;
    params.unk54 = ((func_150ADA68() * 254.0f) + 108.0f) * D_800A63C0;

    temp_v0 = func_151303BC(&params, 2, 0x28);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0xA8, &extra, 0x28);
    }
    return temp_v0;
}

void *func_15159890(f32 *, f32 *, s32, s32);

void func_15159BB0(f32 arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 *arg6) {
    f32 sp24[3];
    f32 sp18[3];

    sp24[0] = arg0;
    sp24[1] = arg1;
    sp18[0] = 0.0f;
    sp18[1] = 0.0f;
    sp18[2] = 0.0f;
    sp24[2] = arg2;
    func_15159890(sp24, sp18, arg6[0xC], arg6[0x1]);
}

typedef struct {
    /* 0x00 */ Vec15159890 pos;
    /* 0x0C */ Vec15159890 scale;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
} Sub15159C08;

typedef struct {
    f32 unk0;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    s8 unk7;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
} Extra15159C08;

extern u8 D_800BE9EB;
extern float sinf(float);
extern float cosf(float);
void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

void func_15159C08(u8 *arg0) {
    Sub15159C08 *s0;
    void *temp_v0;

    s0 = (Sub15159C08 *)(arg0 + 0x28);
    if (func_151454BC(D_800BE9EB, 3000.0f, (struct17 *)s0)) {
        s0->unk28 += (s0->unk2C + (func_150ADA68() * s0->unk30)) * D_800BE9A4;
        if (s0->unk28 > 1.0f) {
    Params15159890 params;
    Extra15159C08 extra;
    f32 angle;
    f32 radius;
    f32 x;
    f32 z;
    f32 scale;
    f32 inv;
    f32 mtx[4][4];
    f32 dist;
    f32 size;
    f32 temp_f2;
            params.unk1D = 0x2F;
            params.unk08 = 0xC01;
            params.unk00 = 0x200005;
            params.unk04 = 0;
            params.unk1A = 0;
            params.unk19 = 0;
            params.unk18 = 0;
            params.unk17 = 0;
            params.unk16 = 0;
            params.unk15 = 0;
            params.unk14 = 0;
            params.unk10 = 0;
            params.unk0C = 0;
            params.unk1C = 0xFF;
            params.unk22 = 0;
            params.unk24 = 0.0f;
            params.unk58 = 0x1205;
            params.unk60 = 5;
            params.unk61 = 5;
            params.unk62 = 0xC;
            params.unk63 = -1;
            params.unk30 = s0->pos;
            params.unk3C = 0.0f;
            params.unk40 = 0.0f;
            params.unk44 = 0.0f;
            extra.unk4 = 0;
            extra.unk5 = 0;
            params.unk54 = 0.0f;
            do {
                angle = func_150ADA68() * 6.283185482f;
                radius = func_150ADA68() * s0->unk1C;
                x = sinf(angle) * radius;
                z = cosf(angle) * radius;
                func_150A8050(mtx, s0->unk20, s0->unk24, 0.0f);
                func_150A7960((f32 *)mtx, x, 0.0f, z, &extra.unk10, &extra.unk14, &extra.unk18);
                extra.unk1C = func_150ADA68() * 6.283185482f;
                extra.unk20 = ((func_150ADA68() * 320.0f) + -160.0f) * 0.001000000047f;
                {
                s16 isize;

                scale = ((func_150ADA68() * 161.0f) + 446.0f) * 0.009999999776f;
                inv = 1.0f / scale;
                dist = s0->unk18 * inv;
                size = ((func_150ADA68() * 6596.0f) + 2402.0f) * 0.1000000015f * inv;
                params.unk48.x = s0->scale.x * scale;
                params.unk48.y = s0->scale.y * scale;
                params.unk48.z = s0->scale.z * scale;
                if (dist > 500.0f) {
                    params.unk0A = 500;
                } else {
                    params.unk0A = (size * 0.5f) + dist;
                }
                params.unk1E = size;
                params.unk20 = 0xFF / params.unk1E;
                params.unk58 &= ~0xC0;
                if (func_150ADA20() & 1) {
                    params.unk58 |= 0x40;
                }
                if (func_150ADA20() & 1) {
                    params.unk58 |= 0x80;
                }
                params.unk1B = (func_150ADA20() % 51U) + 0x7C;
                temp_f2 = (func_150ADA68() * 100.0f) + 240.0f;
                params.unk28 = temp_f2;
                extra.unk0 = temp_f2;
                params.unk2C = temp_f2;
                extra.unk6 = (func_150ADA20() % 5U) + 4;
                extra.unk7 = (func_150ADA20() % 5U) + 4;
                extra.unk8 = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.unk0;
                extra.unkC = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.unk0;
                temp_v0 = func_151303BC(&params, 2, 0x24);
                if (temp_v0 != NULL) {
                    memcpy((u8 *)temp_v0 + 0xA8, &extra, 0x24);
                }
                s0->unk28 -= 1.0f;
                }
            } while (s0->unk28 > 1.0f);
        }
    }
}

extern float sinf(float angle);
f32 func_151423D8(u8);
f32 func_15144B68(f32 arg0);
extern f32 D_800BE9A4;
extern s32 D_800BE9E4;
typedef struct 
{
  f32 unkA8;
  u8 unkAC;
  u8 unkAD;
  s8 unkAE;
  s8 unkAF;
  f32 unkB0;
  f32 unkB4;
  f32 unkB8;
  f32 unkBC;
  f32 unkC0;
  f32 unkC4;
  f32 unkC8;
} Sub1515A11C;
typedef struct 
{
  u8 pad0[0x38];
  f32 unk38;
  f32 unk3C;
  u8 pad40[0xC];
  f32 unk4C;
  f32 unk50;
  f32 unk54;
  u8 pad58[0x50];
  Sub1515A11C sub;
} Struct1515A11C;
extern f32 sinf(f32);
s32 func_1515A11C(Struct1515A11C *arg0, s32 arg1)
{
  Sub1515A11C *s0;
  f32 temp0;
  f32 temp1;
  s0 = &arg0->sub;
  arg0->sub.unkAC = arg0->sub.unkAC + (arg0->sub.unkAE * D_800BE9E4);
  arg0->sub.unkAD = arg0->sub.unkAD + (arg0->sub.unkAF * D_800BE9E4);
  temp0 = func_151423D8(arg0->sub.unkAC - 0x40);
  temp1 = func_151423D8(s0->unkAD - 0x40);
  arg0->unk38 = s0->unkA8 + (s0->unkB0 * temp0);
  arg0->unk3C = s0->unkA8 + (s0->unkB4 * temp1);
  if ((s0 && s0) && s0)
  {
  }
  s0->unkC4 = s0->unkC4 + (s0->unkC8 * D_800BE9A4);
  s0->unkC4 = func_15144B68(s0->unkC4);
  temp0 = sinf(s0->unkC4);
  arg0->unk4C = s0->unkB8 * temp0;
  arg0->unk50 = s0->unkBC * temp0;
  arg0->unk54 = s0->unkC0 * temp0;
  return 1;
}


typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 unk08;
    /* 0x0A */ s16 unk0A;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ Vec15159890 unk30;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ Vec15159890 unk48;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ u8 pad5C[0x4];
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 pad66[0xA];
} Params1515A238;

typedef struct {
    f32 unk0;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    s8 unk7;
    f32 unk8;
    f32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
} Extra1515A238;

typedef struct {
    f32 unk0;
    u8 pad4[0x18];
    u8 unk1C;
    u8 pad1D[0x7];
} Buf1515A238;

void *func_15130280(void *, u8, void *, s32, u8, s32);

void *func_1515A238(f32 *arg0, Vec15159890 *arg1, f32 arg2, f32 arg3, Buf1515A238 *arg4, f32 arg5,
                    s16 arg6, u8 arg7, u8 arg8, u8 arg9, s32 argA) {
    void *ret;
    Params1515A238 params;
    Extra1515A238 extra;
    Buf1515A238 buf;
    s32 r2;
    s32 r1;

    params.unk1D = 0x2F;
    params.unk08 = 0x2203;
    params.unk00 = 0x200005;
    params.unk04 = 0;
    params.unk1A = 0;
    params.unk19 = 0;
    params.unk18 = 0;
    params.unk17 = 0;
    params.unk16 = 0;
    params.unk15 = 0;
    params.unk14 = 0;
    params.unk10 = 0;
    params.unk0C = 0;
    params.unk1C = 0xFF;
    params.unk22 = 0;
    params.unk24 = 0.0f;
    r1 = (func_150ADA20() & 1) ? 0x40 : 0;
    r2 = (func_150ADA20() & 1) ? 0x80 : 0;
    params.unk58 = r2 | 0x1007 | r1 | 0x8000 | 0x4000;
    params.unk60 = 6;
    params.unk61 = 8;
    if (arg8) {
        params.unk62 = 0x15;
    } else {
        params.unk62 = 0x14;
    }
    params.unk63 = -1;
    params.unk64 = -1;
    params.unk30 = *(Vec15159890 *)arg0;
    params.unk3C = 0.0f;
    params.unk40 = 0.0f;
    params.unk44 = 0.0f;
    params.unk1E = 1;
    params.unk20 = 0xFF;
    extra.unk0 = arg5;
    params.unk28 = arg5;
    params.unk2C = arg5;
    extra.unk4 = func_150ADA20() & 0xFF;
    extra.unk5 = func_150ADA20() & 0xFF;
    extra.unk6 = (func_150ADA20() % 5U) + 4;
    extra.unk7 = (func_150ADA20() % 5U) + 4;
    extra.unk8 = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.unk0;
    extra.unkC = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.unk0;
    extra.unk10 = 0;
    extra.unk11 = 0;
    extra.unk12 = (func_150ADA20() % 5U) + 4;
    extra.unk13 = (func_150ADA20() % 5U) + 4;
    extra.unk14 = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * extra.unk0;
    extra.unk18 = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * extra.unk0;
    params.unk48 = *arg1;
    params.unk65 = 0;
    params.unk0A = arg6;
    params.unk1B = arg7;
    params.unk54 = arg2;
    extra.unk1C = arg3;
    if (arg1->x != 0 || arg1->z != 0) {
        extra.unk20 = -99999.8984375f;
    } else {
        if (arg4 != NULL) {
            buf = *arg4;
        } else {
            buf.unk1C = 0;
        }
        if (func_1514672C(arg0)) {
            if (func_15046C00(arg0, 0, 19500.0f, (u8 *)&buf)) {
                arg4 = &buf;
                extra.unk20 = buf.unk0;
            } else {
                extra.unk20 = 10000.0f;
            }
        } else {
            extra.unk20 = 10000.0f;
        }
    }
    ret = func_15130280(&params, 2, arg4, 0x24, arg9, argA);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0xA8, &extra, 0x24);
    }
    return ret;
}

typedef struct {
    f32 unk0;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    s8 unk7;
    f32 unk8;
    f32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} Sub1515A60C;

typedef struct {
    u8 pad0[0x38];
    f32 unk38;
    f32 unk3C;
    u8 pad40[0xC];
    f32 unk4C;
    u8 pad50[0x4];
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    u8 pad64[0x44];
    Sub1515A60C sub;
} Struct1515A60C;

s32 func_1515A60C(Struct1515A60C *arg0, s32 arg1) {
    Sub1515A60C *sub;
    s32 i;

    sub = &arg0->sub;
    arg0->unk38 = (func_151423D8(arg0->sub.unk4 - 0x40) * sub->unk8) + sub->unk0;
    arg0->unk3C = (func_151423D8(sub->unk5 - 0x40) * sub->unkC) + sub->unk0;
    arg0->unk4C = func_151423D8(sub->unk10 - 0x40) * sub->unk14;
    arg0->unk54 = func_151423D8(sub->unk11 - 0x40) * sub->unk18;

    sub->unk4 += sub->unk6 * D_800BE9E4;
    sub->unk5 += sub->unk7 * D_800BE9E4;
    sub->unk10 += sub->unk12 * D_800BE9E4;
    sub->unk11 += sub->unk13 * D_800BE9E4;

    for (i = D_800BE9E4; i > 0; i--) {
        arg0->unk58 *= sub->unk1C;
        arg0->unk5C *= sub->unk1C;
        arg0->unk60 *= sub->unk1C;
    }
    return 1;
}

typedef struct {
    f32 unk0;
    u8 pad4[0x1C];
    f32 unk20;
} Sub1515A78C;

u8 func_1515A78C(Struct1515A60C *arg0, s32 arg1) {
    Sub1515A78C *sub;
    u8 result;
    f32 position[3];

    result = 1;
    sub = (Sub1515A78C *)&arg0->sub;
    func_1515A60C(arg0, arg1);
    if (arg0->unk5C < 0.0f) {
        return 1;
    }
    position[0] = *(f32 *)((u8 *)arg0 + 0x40) + arg0->unk4C;
    position[1] = *(f32 *)((u8 *)arg1 + 4);
    position[2] = *(f32 *)((u8 *)arg0 + 0x48) + arg0->unk54;
    if (sub->unk20 == -99999.8984375f) {
        if (func_1514672C(position) == 0) {
            return 0;
        }
        if (func_15046C00(position, 0, *(f32 *)((u8 *)arg0 + 0x44), (u8 *)arg0 + 0x80) != 0) {
            position[1] = *(f32 *)((u8 *)arg0 + 0x80);
            result = 0;
        }
    } else if (sub->unk20 <= *(f32 *)((u8 *)arg0 + 0x44)) {
        position[1] = sub->unk20;
        result = 0;
    }
    if (result == 0) {
        if (func_150ADA68() < 0.200000003f) {
            if (*((u8 *)arg0 + 0x9D) == 3) {
                func_151DBCBC(func_151D8E20(), sub->unk0 * 0.200000003f, *((u8 *)arg0 + 0x2B),
                              (s32)((u8 *)arg0 + 0x84), position, *((u8 *)arg0 + 0xC), *((u8 *)arg0 + 1));
            }
        }
    }
    return result;
}

s32 func_1514ECE0(void *, s16, void *);

s32 func_1515A920(struct102 *arg0, s32 *arg1) {
    s32 *temp;
    s32 v;

    if (func_1514ECE0(*(void **)((u8 *)arg0 + 0x2F4), 0x13, &temp) == 0) {
        return 0;
    }
    v = *(s32 *)((u8 *)temp + 0x10);
    *arg1 = v + 0x34;
    return 1;
}

typedef struct {
    struct WORD words[10];
} Struct1515A974PayloadWords;

typedef struct {
    struct UBYTES4 first;
    Struct1515A974PayloadWords rest;
} Struct1515A974Payload;

typedef struct {
    f32 unk0;
    f32 unk4;
    Struct1515A974Payload payload;
} Struct1515A974Row;

extern Struct1515A974Row D_800A636C[];

typedef struct {
    Struct1515A974Payload payload;
    s32 pad2C[2];
    struct183 tmp;
} Struct1515A974Locals;


void func_1515A974(struct102 *volatile arg0, s32 arg1) {
    Struct1515A974Locals local;
    void *ret;
    s32 idx;

    idx = ((u8 *)arg0)[0x1F];
    if (idx <= 0) {
        local.tmp.unk0 = 0xF;
        local.tmp.unk4 = (s32)arg0;
        local.tmp.unk8 = D_800A636C[idx].unk0;
        local.tmp.unkC = D_800A636C[idx].unk4;
        local.tmp.unk10 = 0.0f;
        local.tmp.unk14 = 0.0f;
        local.tmp.unk18 = 0.0f;
        local.tmp.unk1C = 0.0f;
        local.tmp.unk20 = 0.0f;
        local.tmp.unk24 = 0;
        local.tmp.unk25 = -1;
        local.tmp.unk26 = 2;
        local.payload.first = D_800A636C[idx].payload.first;
        local.payload.rest = D_800A636C[idx].payload.rest;
        ret = func_151A8B20(&local.tmp, -1, 0x2C, 0xFF, 0);
        if (ret != NULL) {
            memcpy((u8 *)ret + 0x80, &local.payload, 0x2C);
        }
    }
}

typedef struct {
    s32 words[10];
} Payload1515AA84;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec1515AA84;

typedef struct {
    f32 unk0;
    Vec1515AA84 unk4;
    f32 unk10;
    Payload1515AA84 unk14;
} Locals1515AA84;

typedef struct {
    s16 base;
    s16 range;
    Payload1515AA84 payload;
} Spawn1515AA84;

typedef struct {
    u8 pad0[0x1];
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x73];
    Spawn1515AA84 unk80;
} Struct1515AA84;

void func_1515AA84(Struct1515AA84 *arg0, Vec1515AA84 *arg1, f32 arg2, s32 arg3) {
    Locals1515AA84 local;
    Spawn1515AA84 *spawn;
    struct260 *ret;

    spawn = &arg0->unk80;
    local.unk0 = 0.0f;
    local.unk4 = *arg1;
    local.unk10 = arg2;
    local.unk14 = spawn->payload;
    ret = func_15149130((u32)func_150ADA20() % (spawn->range + 1) + spawn->base, -1, 0x3F, -1, 1, 0, (struct37 *)0x3C, arg0->unkC, arg0->unk1);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0x28, &local, 0x3C);
    }
}

typedef struct {
    f32 unk0;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
} Extra1515AB88;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ Vec15159890 unk4;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
} Emit1515AB88;

typedef struct {
    u8 pad0[0x1];
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Emit1515AB88 unk28;
} Struct1515AB88;

void func_1515AB88(Struct1515AB88 *arg0) {
    Emit1515AB88 *s0;

    s0 = &arg0->unk28;
    s0->unk0 += (s0->unk14 + (func_150ADA68() * s0->unk18)) * D_800BE9A4;
    if (s0->unk0 > 1.0f) {
        Params1515A238 params;
        Extra1515AB88 extra;
        void *ret;
        s32 r2;
        s32 r1;
        extra.unk4 = 0;
        extra.unk5 = 0;
        extra.unk18 = 0;
        extra.unk19 = 0;
        extra.unk10 = s0->unk2C;
        extra.unk14 = s0->unk30;
        extra.unk24 = s0->unk10;
        params.unk1D = 0x2F;
        params.unk08 = 0x4403;
        params.unk00 = 0x200005;
        params.unk04 = 0x9F0600;
        params.unk0A = 0x12C;
        params.unk0C = 0;
        params.unk10 = 0;
        params.unk18 = 0xFF;
        params.unk19 = 0xFF;
        params.unk1A = 0xFF;
        params.unk17 = 0xFF;
        params.unk14 = s0->unk36;
        params.unk15 = s0->unk37;
        params.unk16 = s0->unk38;
        params.unk1C = 0xFF;
        params.unk30 = s0->unk4;
        params.unk48.x = 0.0f;
        params.unk48.y = 0.0f;
        params.unk48.z = 0.0f;
        params.unk1E = 1;
        params.unk20 = 0xFF;
        params.unk22 = 1;
        params.unk24 = 1.0f;
        params.unk60 = 6;
        params.unk61 = 9;
        params.unk62 = 4;
        params.unk63 = -1;
        params.unk64 = -1;
        params.unk65 = 0;
        params.unk3C = 0.0f;
        params.unk40 = 0.0f;
        params.unk44 = 0.0f;
        do {
            extra.unk6 = (func_150ADA20() % 5U) + 4;
            extra.unk7 = (func_150ADA20() % 5U) + 4;
            extra.unk0 = params.unk28 = params.unk2C = (func_150ADA68() * s0->unk20) + s0->unk1C;
            extra.unk8 = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.unk0;
            extra.unkC = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.unk0;
            extra.unk1A = (func_150ADA20() % 5U) + 4;
            extra.unk1B = (func_150ADA20() % 5U) + 4;
            extra.unk1C = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * extra.unk0;
            extra.unk20 = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * extra.unk0;
            params.unk54 = (func_150ADA68() * s0->unk28) + s0->unk24;
            params.unk1B = ((u32)func_150ADA20() % (s0->unk35 + 1)) + s0->unk34;
            r1 = (func_150ADA20() & 1) ? 0x40 : 0;
            r2 = (func_150ADA20() & 1) ? 0x80 : 0;
            params.unk58 = r2 | 6 | r1 | 0x8000 | 0x4000 | 0x2000 | 0x1000;
            ret = func_15130280(&params, 2, NULL, 0x28, arg0->unkC, arg0->unk1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0xA8, &extra, 0x28);
            }
            s0->unk0 -= 1.0f;
        } while (s0->unk0 > 1.0f);
    }
}
