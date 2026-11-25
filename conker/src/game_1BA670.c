#include <ultra64.h>
/* functions.h declares func_1513D524 as returning void; this file uses the particle it returns. */
#define func_1513D524 func_1513D524_void_proto
#include "functions.h"
#undef func_1513D524
#include "variables.h"
#include "effects.h"

/* Settings an emitter is created with; copied verbatim into Emitter.params. */
typedef struct EmitterParams {
    /* 0x0 */ struct127 *owner;
    /* 0x4 */ u8 ownerId;   // owner->unique_id at creation; the emitter dies when it changes
    /* 0x6 */ s16 lifetime; // counts down when (flags & 1)
    /* 0x8 */ u8 flags;
    /* 0x9 */ s8 expireCb;  // index into D_8008D628 run when lifetime runs out, -1: just die
    /* 0xA */ u8 table;     // index into D_8008D5E0
} EmitterParams; /* 0xC */

/* Particle template, referenced by an EmitterInfo. */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s16 lifeMin;   // lifetime given to a particle when its slot releases it
    /* 0x36 */ s16 lifeRange;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 unk3A;
    /* 0x3B */ u8 unk3B;
    /* 0x3C */ u8 unk3C;
    /* 0x3D */ u8 unk3D;
    /* 0x3E */ u8 unk3E;
    /* 0x3F */ u8 spawnCb; // index into D_8008D620
} ParticleTemplate; /* 0x40 */

/* Per-particle data the emitter copies into Particle.data. */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ vertex vel;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ ParticleTemplate *tmpl;
} ParticleData; /* 0x58 */

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 lifetime;
    /* 0x01E */ u8 pad1E[0x16];
    /* 0x034 */ vertex pos;
    /* 0x040 */ vertex prevPos;
    /* 0x04C */ u8 pad4C[0xC];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 pad5C[0xB4];
    /* 0x110 */ ParticleData data;
} Particle;

/* An emission point attached to one of the owner's bones. */
typedef struct {
    /* 0x0 */ vertex pos;
    /* 0xC */ s16 bone;
} EmitterPoint; /* 0x10 */

typedef struct {
    /* 0x00 */ ParticleTemplate *tmpl;
    /* 0x04 */ s32 numSlots;
    /* 0x08 */ EmitterPoint *points; // one per slot (attached mode)
    /* 0x0C */ f32 rate;             // particles added per frame
    /* 0x10 */ s32 maxCount;
    /* 0x14 */ s16 lifeMin;          // frames a slot holds its particle
    /* 0x16 */ s16 lifeRange;
    /* 0x18 */ u8 spread;            // 1: sphere, 2: horizontal disc (world mode)
    /* 0x19 */ u8 mode;              // 2: emit around one point instead of per-bone points
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ f32 speed;
    /* 0x20 */ f32 speedY;
    /* 0x24 */ f32 yOffset;
    /* 0x28 */ f32 x;
    /* 0x2C */ f32 y;
    /* 0x30 */ f32 z;
    /* 0x34 */ u8 bone;
} EmitterInfo;

/* IDs (struct127.unk84) that select one row of an EmitterTable. */
typedef struct {
    /* 0x0 */ s16 count;
    /* 0x4 */ u8 *ids;
} EmitterIdList;

/*
 * infos holds (n + 2) entries per owner variant (struct127.pad1C8): one per id list,
 * then the attached-mode default, then the unattached one.
 */
typedef struct {
    /* 0x0 */ EmitterInfo **infos;
    /* 0x4 */ s16 n;
    /* 0x8 */ EmitterIdList *idLists;
} EmitterTable;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 *unk18; // emitter scale in percent
} EmitterScaleSource;

typedef struct EmitterSlot {
    /* 0x00 */ Particle *particle;
    /* 0x04 */ s32 timer;
    /* 0x08 */ vertex offset;
    /* 0x14 */ struct EmitterSlot *next; // ring of free slots while emitting
} EmitterSlot; /* 0x18 */

/* Process of type 0x21: emits particles around (or from the bones of) its owner. */
typedef struct Emitter {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ struct Emitter *next;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s32 flags;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ EmitterParams params;
    /* 0x24 */ f32 accum; // fractional particles owed
    /* 0x28 */ s32 count; // live particles
    /* 0x2C */ u8 state;  // 1: hidden, 2: world mode, 4: attached mode, 8: new
    /* 0x2D */ u8 infoMode;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ EmitterScaleSource *scaleSrc;
    /* 0x34 */ f32 scale;
    /* 0x38 */ vertex pos;
    /* 0x44 */ u8 pad44[4];
    /* 0x48 */ EmitterSlot slots[100];
} Emitter;

/* Process lists, by priority and process type. */
extern Emitter *D_800DCE50[2][104];
extern EmitterTable *D_8008D5E0[];
extern void (*D_8008D620[])(u8 *, u8 *, u8 *, u8 *, u8 *, u8 *, s16 *);
extern s32 (*D_8008D628[])(Emitter *);

void *memcpy(void *, const void *, u32);
extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern Particle *func_1513D524(ParticleDef *, u8, u8, u8, u8, u8, s32, u8, s32);
extern void func_1513F680();
extern void func_15143134(void *, void *, void *);
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
void func_1518E308(Emitter *emitter);
Emitter *func_1518E3C4(EmitterParams *params, u8 arg1);

/* Creates an emitter of the given kind on owner, optionally killing the owner's existing emitters first. */
Emitter *func_1518D1C0(struct127 *owner, s16 kind, s16 lifetime, u8 killExisting, u8 arg4, u8 arg5,
                       void *arg6) {
    EmitterParams params;
    s32 i;
    Emitter *emitter;

    if (owner == NULL) {
        return NULL;
    }
    if (killExisting) {
        for (i = 0; i < 2; i++) {
            D_800DD190++;
            emitter = D_800DCE50[i][0x21];
            while (emitter != NULL) {
                ((Emitter **)D_800DD198)[D_800DD190] = emitter->next;
                if (owner == emitter->params.owner || emitter->params.ownerId == owner->unique_id) {
                    func_1516972C((struct102 *)emitter);
                }
                emitter = ((Emitter **)D_800DD198)[D_800DD190];
            }
            {
                // matching: the block-scope redeclaration makes IDO rematerialize &D_800DD190
                extern s8 D_800DD190;

                D_800DD190--;
            }
        }
    }
    params.owner = owner;
    params.ownerId = owner->unique_id;
    switch (kind) {
    case 10:
        params.lifetime = 75;
        params.flags = owner->health ? 1 : 0;
        params.expireCb = 1;
        params.table = 7;
        return func_1518E3C4(&params, arg4);
    case 11:
        params.lifetime = 75;
        params.flags = owner->health ? 1 : 0;
        params.expireCb = 0;
        params.table = 1;
        return func_1518E3C4(&params, arg4);
    case 12:
        params.lifetime = 75;
        params.flags = owner->health ? 1 : 0;
        params.expireCb = 1;
        params.table = 7;
        return func_1518E3C4(&params, arg4);
    case 1:
        params.lifetime = 500;
        params.flags = 1;
        params.expireCb = 0;
        params.table = 1;
        return func_1518E3C4(&params, arg4);
    case 2:
        params.lifetime = 3000;
        params.flags = 1;
        params.expireCb = 1;
        params.table = 7;
        return func_1518E3C4(&params, arg4);
    case 3:
        params.lifetime = 200;
        params.flags = 1;
        params.expireCb = -1;
        params.table = 8;
        return func_1518E3C4(&params, arg4);
    case 4:
        params.lifetime = 200;
        params.flags = 1;
        params.expireCb = -1;
        params.table = 9;
        return func_1518E3C4(&params, arg4);
    case 5:
        params.lifetime = 3000;
        params.flags = 0;
        params.expireCb = -1;
        params.table = 2;
        return func_1518E3C4(&params, arg4);
    case 6:
        params.lifetime = 3000;
        params.flags = 0;
        params.expireCb = -1;
        params.table = 3;
        return func_1518E3C4(&params, arg4);
    case 7:
        params.lifetime = 3000;
        params.flags = 0;
        params.expireCb = -1;
        params.table = 4;
        return func_1518E3C4(&params, arg4);
    case 8:
        params.lifetime = 3000;
        params.flags = 0;
        params.expireCb = -1;
        params.table = 5;
        return func_1518E3C4(&params, arg4);
    case 9:
        params.lifetime = 3000;
        params.flags = 0;
        params.expireCb = -1;
        params.table = 6;
        return func_1518E3C4(&params, arg4);
    case 13:
        params.lifetime = lifetime;
        params.flags = (lifetime == 999) ? 0 : 1;
        params.expireCb = -1;
        params.table = 10;
        return func_1518E3C4(&params, arg4);
    case 14:
        params.lifetime = lifetime;
        params.flags = 1;
        params.expireCb = -1;
        params.table = 11;
        return func_1518E3C4(&params, arg4);
    case 15:
        params.lifetime = lifetime;
        params.flags = (lifetime == 999) ? 0 : 1;
        params.expireCb = -1;
        params.table = 12;
        return func_1518E3C4(&params, arg4);
    case 16:
        params.lifetime = lifetime;
        params.flags = (lifetime == 999) ? 0 : 1;
        params.expireCb = -1;
        params.table = 13;
        return func_1518E3C4(&params, arg4);
    case 17:
        params.lifetime = 300;
        params.flags = 1;
        params.expireCb = -1;
        params.table = 14;
        return func_1518E3C4(&params, arg4);
    case 18:
        params.lifetime = lifetime;
        params.flags = 1;
        params.expireCb = -1;
        params.table = 15;
        return func_1518E3C4(&params, arg4);
    }
    return NULL;
}

/*
 * Per-frame emitter update: follows the owner (or its bones), releases particles whose slot timer ran out
 * and spawns new ones at the info's rate. Dies with its owner or when its lifetime callback says so.
 */
void func_1518D6E0(Emitter *emitter) {
    Emitter *self;
    s8 done;

    self = emitter; // matching: IDO allocates the param copy differently
    done = 0;
    if (!(self->flags & 1)) {
        if (self->count != 0) {
            func_1518E308(self);
            self->state = 1;
        }
        return;
    }
    if (self->params.owner != NULL && self->params.owner->interaction_state != 0 &&
        self->params.owner->unique_id == self->params.ownerId) {
        u8 attach;
        s16 i;
        s16 j;
        EmitterTable *table;
        EmitterInfo **row;
        EmitterInfo *info;
        s32 found;
        u8 reset;
        EmitterSlot *slot;
        s32 k;
        s32 numFree;
        EmitterSlot *prevFree;
        EmitterSlot *freeSlot;

        if (self->scaleSrc != NULL) {
            self->scale = *self->scaleSrc->unk18 * 0.009999999776f;
        }
        if (self->params.flags & 1) {
            self->params.lifetime -= D_800BE9E4;
            if (self->params.lifetime < 0) {
                if (self->params.expireCb != -1) {
                    done = D_8008D628[self->params.expireCb](self);
                } else {
                    done = 1;
                }
            }
        }
        freeSlot = NULL;
        if (self->params.owner->unk74 & (1 << D_80082FA4)) {
            if (!(self->state & 1)) {
                func_1518E308(self);
                self->state |= 1;
            }
        } else {
            reset = 0;
            numFree = 0;
            prevFree = NULL;
            table = D_8008D5E0[self->params.table];
            if (self->params.owner->unk1D4 != NULL && (self->params.owner->unk74 & 0xF) != 0xF) {
                attach = 4;
                row = &table->infos[self->params.owner->pad1C8 * (table->n + 2)];
                i = 0;
                found = 0;
                info = row[table->n];
                if (i < table->n) {
                    do {
                        for (j = 0; !found && j < table->idLists[i].count; j++) {
                            if (self->params.owner->unk84.uh == table->idLists[i].ids[j]) {
                                found = 1;
                                info = row[i];
                            }
                        }
                        i++;
                        goto cond; // matching: block boundary lets IDO keep i's s16 increment in its register
                    cond:;
                    } while (!found && i < table->n);
                }
            } else {
                info = table->infos[self->params.owner->pad1C8 * (table->n + 2) + table->n + 1];
                attach = 2;
            }
            if (!(self->state & 8) && (self->state != attach || info->mode != self->infoMode)) {
                func_1518E308(self);
                reset = 1;
                self->accum = info->maxCount;
            }
            if (attach == 2) {
                self->pos.x = self->params.owner->x_position;
                self->pos.y = self->params.owner->y_position + info->yOffset;
                self->pos.z = self->params.owner->z_position;
            } else if (info->mode == 2) {
                func_150A7960((f32 *)((Mtx *)self->params.owner->unk1D4 + info->bone), info->x, info->y, info->z,
                              &self->pos.x, &self->pos.y, &self->pos.z);
            }
            slot = self->slots;
            for (k = 0; k < info->numSlots; k++, slot++) {
                u8 isFree;

                isFree = 1;
                if (slot->particle) {
                    slot->particle->prevPos = slot->particle->pos;
                    if (info->mode == 2 || attach == 2) {
                        slot->particle->pos.x = self->pos.x + slot->offset.x;
                        slot->particle->pos.y = self->pos.y + slot->offset.y;
                        slot->particle->pos.z = self->pos.z + slot->offset.z;
                    } else {
                        func_15143134(&info->points[k], &slot->particle->pos,
                                      (Mtx *)self->params.owner->unk1D4 + info->points[k].bone);
                    }
                    slot->timer -= D_800BE9E4;
                    if (slot->timer < 0) {
                        ParticleData *data;

                        data = &slot->particle->data;
                        data->vel.x = (slot->particle->pos.x - slot->particle->prevPos.x) * D_800BE9A8;
                        if (data->vel.x > 10.0f) {
                            data->vel.x = 10.0f;
                        } else if (data->vel.x < -10.0f) {
                            data->vel.x = -10.0f;
                        }
                        data->vel.z = (slot->particle->pos.z - slot->particle->prevPos.z) * D_800BE9A8;
                        if (data->vel.z > 10.0f) {
                            data->vel.z = 10.0f;
                        } else if (data->vel.z < -10.0f) {
                            data->vel.z = -10.0f;
                        }
                        data->vel.y = 0.0f;
                        func_1513F680(slot->particle, data->tmpl->unk3B, data->tmpl->unk3C, data->tmpl->unk3D,
                                      data->tmpl->unk3E);
                        slot->particle->lifetime =
                            func_150ADA20() % (data->tmpl->lifeRange + 1) + data->tmpl->lifeMin;
                        slot->particle->flags |= 1;
                        slot->particle = NULL;
                        self->count--;
                    } else {
                        isFree = 0;
                    }
                }
                if (isFree) {
                    numFree++;
                    if (prevFree == NULL) {
                        prevFree = slot;
                    }
                    slot->next = freeSlot;
                    freeSlot = slot;
                }
            }
            if (numFree != 0) {
                self->accum += info->rate;
                if (self->accum > 1.0f) {
                    ParticleDef spawn;
                    ParticleData data;
                    ParticleTemplate *tmpl;
                    u8 ok;
                    u8 spawnFlag;

                    tmpl = info->tmpl;
                    ok = 1;
                    prevFree->next = freeSlot;
                    spawn.frame = 0;
                    data.unk20 = tmpl->unk14;
                    data.unk24 = tmpl->unk18;
                    data.unk2C = 0;
                    spawn.frameSpeed = (tmpl->unk14 + tmpl->unk18) * 0.5f;
                    data.unk3C = tmpl->unk1C;
                    data.unk40 = tmpl->unk20;
                    data.unk44 = tmpl->unk24;
                    data.unk4C = tmpl->unk2C;
                    data.unk50 = tmpl->unk30;
                    data.unk48 = tmpl->unk28;
                    data.unk4A = tmpl->unk2A;
                    data.unk0 = tmpl->unk0 * self->scale;
                    data.unk4 = tmpl->unk4 * self->scale;
                    data.unk1C = 0;
                    data.unk8 = spawn.width = reset ? (data.unk0 + data.unk4) * 0.5f : 0.0f;
                    data.unkC = tmpl->unk8 * self->scale;
                    data.unk10 = tmpl->unkC * self->scale;
                    data.unk14 = tmpl->unk10 * self->scale;
                    data.unk1E = 0;
                    data.unk18 = spawn.height = reset ? (data.unkC + data.unk10) * 0.5f : 0.0f;
                    data.tmpl = tmpl;
                    spawn.scale.x = 1.0f;
                    spawn.scale.y = 0.0f;
                    spawn.scale.z = 1.0f;
                    spawn.flags = 0x40000000;
                    spawn.unk44 = 0xFF;
                    spawn.unk45 = 0xFF;
                    while (ok && numFree != 0 && !(self->accum < 1.0f) && self->count < info->maxCount) {
                        s32 skip;

                        skip = func_150ADA20() % (numFree + 1);
                        while (skip--) {
                            prevFree = freeSlot;
                            freeSlot = freeSlot->next;
                        }
                        D_8008D620[tmpl->spawnCb](&spawn.texture, &spawnFlag, &spawn.rgba[0], &spawn.rgba[1],
                                                  &spawn.rgba[2], &spawn.rgba[3], &spawn.renderModes);
                        freeSlot->particle = func_1513D524(&spawn, tmpl->unk38, tmpl->unk39, 0, tmpl->unk3A,
                                                           spawnFlag, sizeof(ParticleData), 0xFF, 0);
                        if (freeSlot->particle != NULL) {
                            memcpy(&freeSlot->particle->data, &data, sizeof(ParticleData));
                            freeSlot->particle->flags |= 0x40000000;
                            if (info->mode == 2 || attach == 2) {
                                switch (info->spread) {
                                case 1: {
                                    u8 yaw;
                                    u8 pitch;
                                    f32 speed;
                                    f32 c1;
                                    f32 c2;
                                    f32 c3;
                                    f32 c4;

                                    yaw = func_150ADA20() & 0xFF;
                                    pitch = func_150ADA20() & 0xFF;
                                    speed = func_150ADA68() * info->speed;
                                    c1 = func_151423D8(pitch - 0x40);
                                    c2 = func_151423D8(pitch);
                                    c3 = func_151423D8(yaw - 0x40);
                                    c4 = func_151423D8(yaw);
                                    freeSlot->offset.x = speed * c2 * c3;
                                    freeSlot->offset.y = -speed * c1;
                                    freeSlot->offset.z = speed * c2 * c4;
                                }
                                    break;
                                case 2: {
                                    u8 pitch;
                                    f32 speed;

                                    pitch = func_150ADA20() & 0xFF;
                                    speed = func_150ADA68() * info->speed;
                                    freeSlot->offset.x = func_151423D8(pitch) * speed;
                                    freeSlot->offset.y = func_150ADA68() * info->speedY;
                                    freeSlot->offset.z = -func_151423D8(pitch - 0x40) * speed;
                                }
                                    break;
                                }
                                freeSlot->particle->pos.x = self->pos.x + freeSlot->offset.x;
                                freeSlot->particle->pos.y = self->pos.y + freeSlot->offset.y;
                                freeSlot->particle->pos.z = self->pos.z + freeSlot->offset.z;
                            } else {
                                s32 idx;

                                idx = freeSlot - self->slots;
                                func_15143134(&info->points[idx], &freeSlot->particle->pos,
                                              (Mtx *)self->params.owner->unk1D4 + info->points[idx].bone);
                            }
                            freeSlot->particle->prevPos = freeSlot->particle->pos;
                            freeSlot->timer = func_150ADA20() % (info->lifeRange + 1) + info->lifeMin;
                            freeSlot = freeSlot->next;
                            prevFree->next = freeSlot;
                            self->count++;
                        } else {
                            ok = 0;
                        }
                        numFree--;
                        self->accum -= 1.0f;
                    }
                    while (self->accum > 1.0f) {
                        self->accum -= 1.0f;
                    }
                }
            }
            self->state = attach;
            self->infoMode = info->mode;
        }
    } else {
        done = 1;
    }
    if (done) {
        func_1516972C(self);
    }
}

typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[0x14];
    /* 0x1C */ struct127 *target;
} Struct1518E298;

/* Copies the target's position into the tracker (as s16); returns 1 when there is no live target. */
s32 func_1518E298(Struct1518E298 *tracker, s32 arg1, s32 arg2, s32 arg3) {
    struct127 *target;

    target = tracker->target;
    if (target != NULL && target->interaction_state != 0) {
        tracker->x = target->x_position;
        tracker->y = target->y_position;
        tracker->z = target->z_position;
        return 0;
    }

    return 1;
}

/* Frees every particle the emitter holds and clears its slots. */
void func_1518E308(Emitter *emitter) {
    EmitterSlot *slot;
    s32 i;

    slot = emitter->slots;
    emitter->count = 0;
    emitter->accum = 0.0f;
    for (i = 0; i < 100; i++, slot++) {
        if (slot->particle != NULL) {
            func_1516972C((struct102 *)slot->particle);
        }
    }
    bzero(emitter->slots, sizeof(emitter->slots));
}

/* Switches the emitter to another EmitterTable, dropping its particles. */
void func_1518E37C(Emitter *emitter, u8 table) {
    if (emitter->params.table != table) {
        func_1518E308(emitter);
        emitter->params.table = table;
    }
}

/* Starts an emitter process with the given params. */
Emitter *func_1518E3C4(EmitterParams *params, u8 arg1) {
    Emitter *emitter;

    if (params->owner->id == 150) {
        return NULL;
    }
    emitter = func_15167A68(0x21, 1, 0x9EC, 1, arg1, 1);
    if (emitter == NULL) {
        return NULL;
    }
    memcpy(&emitter->params, params, sizeof(EmitterParams));
    emitter->count = 0;
    emitter->state = 8;
    emitter->infoMode = 4;
    emitter->scaleSrc = NULL;
    emitter->flags = 1;
    emitter->unk14 = 0;
    emitter->accum = 0.0f;
    emitter->pos.x = 0.0f;
    emitter->pos.y = 0.0f;
    emitter->pos.z = 0.0f;
    emitter->scale = 1.0f;
    bzero(emitter->slots, sizeof(emitter->slots));
    return emitter;
}
