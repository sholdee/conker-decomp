#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/view.h"
#include "libc/string.h"

/*
 * The front-end .rodata of the unit that begins with this file (0x24BF20). The ROM shows that this file,
 * actoremitterhandlers.c, game/Beam/BeamMotion/beammotion.c, game/Particle/AnchorEmitter/anchoremitter.c
 * and the files up to game/Effects/BurnDamage/burndamage.c were one unit: their .rodata slices follow each
 * other without unit padding. The strings and tables of the later files come before this file's literals,
 * so they are defined here, in the ROM order, until the files are merged; the files that use them see the
 * declarations in conker/particle.h. In one file with the others, beam_fillMotionVerts does not match yet
 * (IDO keeps vtx in a register where the ROM loads it again).
 */
/* The effect names that particle_chainAllOverFireToSmoke and particle_chainTailFireToSmoke pass on. */
const char gAllOverFireToSmokeName[] = "effects_characterfire_conker_all_over_fire_to_smoke_death";
const char gTailFireToSmokeName[] = "effects_characterfire_conker_tail_fire_to_smoke_death";
/* The template of particle_broadcastToActorEmitters's local. */
const IdList1 gActorEmitterTypes = { { 0x21 } };
/*
 * The anchor emitter templates, one array per kind (gAnchorEmitterTemplates): { rate, rateRange, burstBase,
 * burstRange, sizeX, sizeY, sizeXRange, sizeYRange, unk20, unk21Range, lifeMin, lifeRange, pad, maxCount,
 * velocityScale, unk30, unk34Range, unk38, unk3C, unk3E, unk40, unk42Range, anchorSet, flags, pad, unk4C }.
 */
const AnchorEmitterTemplate D_800A74D8[] = {
    { 0.504f, 0.517f, 20.0f, 10.0f, 0.46100003f, 0.574f, 0.404f, 0.41200003f, 80, 100, 20, 60, { 0 }, 30,
      0.271f, 0.21f, 0.347f, 0.934411f, 16, 15, 20, 40, 0, 0x00, { 0 }, 0.085f },
};
const AnchorEmitterTemplate D_800A7528[] = {
    { 0.30100003f, 1.108f, 10.0f, 10.0f, 0.628f, 1.1010001f, 1.8320001f, 1.342f, 80, 120, 20, 100, { 0 }, 40,
      0.4f, 0.123f, 0.27f, 1.0f, 18, 14, 50, 50, 5, 0x01, { 0 }, 1.0f },
};
const AnchorEmitterTemplate D_800A7578[] = {
    { 0.34500003f, 0.38300002f, 35.0f, 0.0f, 0.52900004f, 0.66300005f, 0.45200002f, 0.577f, 80, 80, 16, 64, { 0 }, 35,
      0.428f, 0.31500003f, 0.36200002f, 0.91916f, 16, 15, 18, 20, 2, 0x00, { 0 }, 0.083000004f },
};
const AnchorEmitterTemplate D_800A75C8[] = {
    { 1.5660001f, 1.595f, 10.0f, 5.0f, 0.50200003f, 0.453f, 0.24300002f, 0.34800002f, 80, 80, 10, 30, { 0 }, 15,
      0.304f, 0.149f, 0.259f, 0.918552f, 15, 17, 45, 25, 1, 0x00, { 0 }, 0.060000002f },
};
const AnchorEmitterTemplate D_800A7618[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 4, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A7668[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 3, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A76B8[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 7, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A7708[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 8, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A7758[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 9, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A77A8[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 10, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A77F8[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 11, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A7848[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 13, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A7898[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 12, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A78E8[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 14, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A7938[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 15, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A7988[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 16, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A79D8[] = {
    { 0.698f, 0.60800004f, 15.0f, 10.0f, 0.312f, 0.354f, 0.404f, 0.30400002f, 80, 80, 12, 48, { 0 }, 25,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 17, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A7A28[] = {
    { 1.0f, 0.1f, 30.0f, 20.0f, 0.6f, 0.6f, 0.8f, 0.6f, 80, 80, 12, 48, { 0 }, 70,
      0.393f, 0.19800001f, 0.30400002f, 0.922707f, 12, 21, 15, 10, 18, 0x00, { 0 }, 0.064f },
};
const AnchorEmitterTemplate D_800A7A78[] = {
    { 0.504f, 0.517f, 20.0f, 10.0f, 0.46100003f, 0.574f, 0.404f, 0.41200003f, 80, 100, 20, 60, { 0 }, 30,
      0.271f, 0.21f, 0.347f, 0.934411f, 16, 15, 20, 40, 19, 0x00, { 0 }, 0.085f },
};
/* The per-object-id emitter data blocks of particle_getObjEmitterData (all empty in this build). */
const u8 D_800A7AC8[4] = { 0 };
const u8 D_800A7ACC[4] = { 0 };
const u8 D_800A7AD0[4] = { 0 };
const u8 D_800A7AD4[4] = { 0 };
const u8 D_800A7AD8[4] = { 0 };
const u8 D_800A7ADC[4] = { 0 };
const u8 D_800A7AE0[4] = { 0 };
const u8 D_800A7AE4[4] = { 0 };
const u8 D_800A7AE8[4] = { 0 };
const u8 D_800A7AEC[4] = { 0 };
const u8 D_800A7AF0[4] = { 0 };
const u8 D_800A7AF4[4] = { 0 };
const u8 D_800A7AF8[4] = { 0 };
const u8 D_800A7AFC[4] = { 0 };
const u8 D_800A7B00[4] = { 0 };
const u8 D_800A7B04[4] = { 0 };
const u8 D_800A7B08[4] = { 0 };
const u8 D_800A7B0C[4] = { 0 };
const u8 D_800A7B10[4] = { 0 };

/*
 * This file's .data. The emitter tables by ActorEmitterParams.table; the tables are in game/Particle/ActorEmitterData/actoremitterdata.c.
 */
static EmitterTable *gActorEmitterTables[] = {
    &D_8008F780, &gAllOverFireEmitterTable, &D_8008F7A4, &D_8008F7B0, &D_8008F7D4, &D_8008F7BC, &D_8008F7C8, &gTailFireEmitterTable,
    &gAllOverSmokeEmitterTable, &gTailSmokeEmitterTable, &gLavaBoulderImpactEmitterTable, &D_8008F804, &gFireImpImpactEmitterTable, &D_8008F81C, &D_8008F828, &D_8008F834,
};
/* Particle spawn callbacks, by ParticleTemplate.spawnCb (game/Particle/ActorEmitterHandlers/actoremitterhandlers.c). */
static void (*gActorEmitterSpawnFuncs[])(u8 *, u8 *, u8 *, u8 *, u8 *, u8 *, u16 *) = { particle_pickBlackTexture, particle_pickPaletteTexture };
/* Lifetime callbacks, by ActorEmitterParams.expireCb (game/Particle/ActorEmitterHandlers/actoremitterhandlers.c). */
static s32 (*gActorEmitterExpireFuncs[])(ActorEmitterProcess *) = { particle_chainAllOverFireToSmoke, particle_chainTailFireToSmoke };

/* Creates an emitter of the given kind on owner, optionally killing the owner's existing emitters first. */
ActorEmitterProcess *particle_startActorEmitter(Actor *owner, s16 kind, s16 lifetime, u8 killExisting, u8 fxOwner, u8 fxList,
                       const char *callerName) {
    ActorEmitterParams params;
    s32 i;

    if (owner == NULL) {
        return NULL;
    }
    if (killExisting) {
        for (i = 0; i < 2; i++) {
            ActorEmitterProcess *emitter;

            gObjectWalkDepth++;
            emitter = (ActorEmitterProcess *)gObjectLists[i][0x21];
            while (emitter != NULL) {
                gObjectWalkNext[gObjectWalkDepth] = emitter->header.next;
                if (owner == emitter->params.owner || emitter->params.ownerId == owner->unique_id) {
                    effect_destroyObject(&emitter->header);
                }
                emitter = gObjectWalkNext[gObjectWalkDepth];
            }
            gObjectWalkDepth--;
        }
    }
    params.owner = owner;
    params.ownerId = owner->unique_id;
    switch (kind) {
    case 10:
        params.lifetime = 75;
        params.flags = owner->energy ? 1 : 0;
        params.expireCb = 1;
        params.table = 7;
        return particle_newActorEmitter(&params, fxOwner);
    case 11:
        params.lifetime = 75;
        params.flags = owner->energy ? 1 : 0;
        params.expireCb = 0;
        params.table = 1;
        return particle_newActorEmitter(&params, fxOwner);
    case 12:
        params.lifetime = 75;
        params.flags = owner->energy ? 1 : 0;
        params.expireCb = 1;
        params.table = 7;
        return particle_newActorEmitter(&params, fxOwner);
    case 1:
        params.lifetime = 500;
        params.flags = 1;
        params.expireCb = 0;
        params.table = 1;
        return particle_newActorEmitter(&params, fxOwner);
    case 2:
        params.lifetime = 3000;
        params.flags = 1;
        params.expireCb = 1;
        params.table = 7;
        return particle_newActorEmitter(&params, fxOwner);
    case 3:
        params.lifetime = 200;
        params.flags = 1;
        params.expireCb = -1;
        params.table = 8;
        return particle_newActorEmitter(&params, fxOwner);
    case 4:
        params.lifetime = 200;
        params.flags = 1;
        params.expireCb = -1;
        params.table = 9;
        return particle_newActorEmitter(&params, fxOwner);
    case 5:
        params.lifetime = 3000;
        params.flags = 0;
        params.expireCb = -1;
        params.table = 2;
        return particle_newActorEmitter(&params, fxOwner);
    case 6:
        params.lifetime = 3000;
        params.flags = 0;
        params.expireCb = -1;
        params.table = 3;
        return particle_newActorEmitter(&params, fxOwner);
    case 7:
        params.lifetime = 3000;
        params.flags = 0;
        params.expireCb = -1;
        params.table = 4;
        return particle_newActorEmitter(&params, fxOwner);
    case 8:
        params.lifetime = 3000;
        params.flags = 0;
        params.expireCb = -1;
        params.table = 5;
        return particle_newActorEmitter(&params, fxOwner);
    case 9:
        params.lifetime = 3000;
        params.flags = 0;
        params.expireCb = -1;
        params.table = 6;
        return particle_newActorEmitter(&params, fxOwner);
    case 13:
        params.lifetime = lifetime;
        params.flags = (lifetime == 999) ? 0 : 1;
        params.expireCb = -1;
        params.table = 10;
        return particle_newActorEmitter(&params, fxOwner);
    case 14:
        params.lifetime = lifetime;
        params.flags = 1;
        params.expireCb = -1;
        params.table = 11;
        return particle_newActorEmitter(&params, fxOwner);
    case 15:
        params.lifetime = lifetime;
        params.flags = (lifetime == 999) ? 0 : 1;
        params.expireCb = -1;
        params.table = 12;
        return particle_newActorEmitter(&params, fxOwner);
    case 16:
        params.lifetime = lifetime;
        params.flags = (lifetime == 999) ? 0 : 1;
        params.expireCb = -1;
        params.table = 13;
        return particle_newActorEmitter(&params, fxOwner);
    case 17:
        params.lifetime = 300;
        params.flags = 1;
        params.expireCb = -1;
        params.table = 14;
        return particle_newActorEmitter(&params, fxOwner);
    case 18:
        params.lifetime = lifetime;
        params.flags = 1;
        params.expireCb = -1;
        params.table = 15;
        return particle_newActorEmitter(&params, fxOwner);
    }
    return NULL;
}

/*
 * Per-frame emitter update: follows the owner (or its bones), releases particles whose slot timer ran out
 * and spawns new ones at the info's rate. Dies with its owner or when its lifetime callback says so.
 */
void particle_updateActorEmitter(ActorEmitterProcess *emitter) {
    ActorEmitterProcess *self;
    s8 done;

    self = emitter; // matching: IDO allocates the param copy differently
    done = 0;
    if (!(self->flags & 1)) {
        if (self->count != 0) {
            particle_freeActorEmitterParticles(self);
            self->state = 1;
        }
        return;
    }
    if (self->params.owner != NULL && self->params.owner->type != 0 &&
        self->params.owner->unique_id == self->params.ownerId) {
        u8 attach;
        s16 i;
        s16 j;
        EmitterTable *table;
        EmitterInfo **row;
        EmitterInfo *info;
        s32 found;
        u8 reset;
        ActorEmitterSlot *slot;
        s32 k;
        s32 numFree;
        ActorEmitterSlot *prevFree;
        ActorEmitterSlot *freeSlot;

        if (self->scaleSrc != NULL) {
            self->scale = *self->scaleSrc->unk18 * 0.009999999776f;
        }
        if (self->params.flags & 1) {
            self->params.lifetime -= gFrameTicks;
            if (self->params.lifetime < 0) {
                if (self->params.expireCb != -1) {
                    done = gActorEmitterExpireFuncs[self->params.expireCb](self);
                } else {
                    done = 1;
                }
            }
        }
        freeSlot = NULL;
        if (self->params.owner->cullMask & (1 << gCurrentView)) {
            if (!(self->state & 1)) {
                particle_freeActorEmitterParticles(self);
                self->state |= 1;
            }
        } else {
            reset = 0;
            numFree = 0;
            prevFree = NULL;
            table = gActorEmitterTables[self->params.table];
            if (self->params.owner->ob_matrix_ptr != NULL && (self->params.owner->cullMask & 0xF) != 0xF) {
                attach = 4;
                row = &table->infos[self->params.owner->current_lod * (table->n + 2)];
                i = 0;
                found = 0;
                info = row[table->n];
                if (i < table->n) {
                    do {
                        for (j = 0; !found && j < table->idLists[i].count; j++) {
                            if (self->params.owner->animseq.uh == table->idLists[i].ids[j]) {
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
                info = table->infos[self->params.owner->current_lod * (table->n + 2) + table->n + 1];
                attach = 2;
            }
            if (!(self->state & 8) && (self->state != attach || info->mode != self->infoMode)) {
                particle_freeActorEmitterParticles(self);
                reset = 1;
                self->accum = info->maxCount;
            }
            if (attach == 2) {
                self->pos.x = self->params.owner->xpos;
                self->pos.y = self->params.owner->ypos + info->yOffset;
                self->pos.z = self->params.owner->zpos;
            } else if (info->mode == 2) {
                math_transformPoint(self->params.owner->ob_matrix_ptr[info->bone], info->x, info->y, info->z,
                              &self->pos.x, &self->pos.y, &self->pos.z);
            }
            slot = self->slots;
            for (k = 0; k < info->numSlots; k++, slot++) {
                u8 isFree;

                isFree = 1;
                if (slot->particle) {
                    slot->particle->head.def.rot = slot->particle->head.def.pos;
                    if (info->mode == 2 || attach == 2) {
                        slot->particle->head.def.pos.x = self->pos.x + slot->offset.x;
                        slot->particle->head.def.pos.y = self->pos.y + slot->offset.y;
                        slot->particle->head.def.pos.z = self->pos.z + slot->offset.z;
                    } else {
                        math_transformOffset(&info->points[k].pos, &slot->particle->head.def.pos,
                                      self->params.owner->ob_matrix_ptr + info->points[k].bone);
                    }
                    slot->timer -= gFrameTicks;
                    if (slot->timer < 0) {
                        ActorEmitterParticleData *data;

                        data = &slot->particle->data;
                        data->vel.x = (slot->particle->head.def.pos.x - slot->particle->head.def.rot.x) * gFrameDeltaInv;
                        if (data->vel.x > 10.0f) {
                            data->vel.x = 10.0f;
                        } else if (data->vel.x < -10.0f) {
                            data->vel.x = -10.0f;
                        }
                        data->vel.z = (slot->particle->head.def.pos.z - slot->particle->head.def.rot.z) * gFrameDeltaInv;
                        if (data->vel.z > 10.0f) {
                            data->vel.z = 10.0f;
                        } else if (data->vel.z < -10.0f) {
                            data->vel.z = -10.0f;
                        }
                        data->vel.y = 0.0f;
                        particle_setQuadParticleFuncs(&slot->particle->head, data->tmpl->unk3B, data->tmpl->unk3C, data->tmpl->unk3D,
                                      data->tmpl->unk3E);
                        slot->particle->head.def.lifetime =
                            math_random() % (data->tmpl->lifeRange + 1) + data->tmpl->lifeMin;
                        slot->particle->head.def.flags |= 1;
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
                    ActorEmitterParticleData data;
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
                    data.stretch = tmpl->unk30;
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
                    spawn.alpha = 0xFF;
                    spawn.colorLevel = 0xFF;
                    while (ok && numFree != 0 && !(self->accum < 1.0f) && self->count < info->maxCount) {
                        s32 skip;

                        skip = math_random() % (numFree + 1);
                        while (skip--) {
                            prevFree = freeSlot;
                            freeSlot = freeSlot->next;
                        }
                        gActorEmitterSpawnFuncs[tmpl->spawnCb](&spawn.texture, &spawnFlag, &spawn.rgba[0], &spawn.rgba[1],
                                                  &spawn.rgba[2], &spawn.rgba[3], &spawn.renderModes);
                        freeSlot->particle = particle_newQuadParticleWhite(&spawn, tmpl->unk38, tmpl->unk39, 0, tmpl->unk3A,
                                                           spawnFlag, sizeof(ActorEmitterParticleData), 0xFF, 0);
                        if (freeSlot->particle != NULL) {
                            memcpy(&freeSlot->particle->data, &data, sizeof(ActorEmitterParticleData));
                            freeSlot->particle->head.def.flags |= 0x40000000;
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

                                    yaw = math_random() & 0xFF;
                                    pitch = math_random() & 0xFF;
                                    speed = math_randomFloat() * info->speed;
                                    c1 = math_cosBinAngleMasked(pitch - 0x40);
                                    c2 = math_cosBinAngleMasked(pitch);
                                    c3 = math_cosBinAngleMasked(yaw - 0x40);
                                    c4 = math_cosBinAngleMasked(yaw);
                                    freeSlot->offset.x = speed * c2 * c3;
                                    freeSlot->offset.y = -speed * c1;
                                    freeSlot->offset.z = speed * c2 * c4;
                                }
                                    break;
                                case 2: {
                                    u8 pitch;
                                    f32 speed;

                                    pitch = math_random() & 0xFF;
                                    speed = math_randomFloat() * info->speed;
                                    freeSlot->offset.x = math_cosBinAngleMasked(pitch) * speed;
                                    freeSlot->offset.y = math_randomFloat() * info->speedY;
                                    freeSlot->offset.z = -math_cosBinAngleMasked(pitch - 0x40) * speed;
                                }
                                    break;
                                }
                                freeSlot->particle->head.def.pos.x = self->pos.x + freeSlot->offset.x;
                                freeSlot->particle->head.def.pos.y = self->pos.y + freeSlot->offset.y;
                                freeSlot->particle->head.def.pos.z = self->pos.z + freeSlot->offset.z;
                            } else {
                                s32 idx;

                                idx = freeSlot - self->slots;
                                math_transformOffset(&info->points[idx].pos, &freeSlot->particle->head.def.pos,
                                              self->params.owner->ob_matrix_ptr + info->points[idx].bone);
                            }
                            freeSlot->particle->head.def.rot = freeSlot->particle->head.def.pos;
                            freeSlot->timer = math_random() % (info->lifeRange + 1) + info->lifeMin;
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
        effect_destroyObject(&self->header);
    }
}

/* Sound record callback: moves the sound to its object (callback_param2); stops it when the object is gone. */
s32 particle_followEmitterSound(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound) {
    Actor *target;

    target = record->callback_param2.obj;
    if (target != NULL && target->type != 0) {
        record->x = target->xpos;
        record->y = target->ypos;
        record->z = target->zpos;
        return 0;
    }

    return 1;
}

/* Frees every particle the emitter holds and clears its slots. */
void particle_freeActorEmitterParticles(ActorEmitterProcess *emitter) {
    ActorEmitterSlot *slot;
    s32 i;

    slot = emitter->slots;
    emitter->count = 0;
    emitter->accum = 0.0f;
    for (i = 0; i < 100; i++, slot++) {
        if (slot->particle != NULL) {
            effect_destroyObject(&slot->particle->head.header);
        }
    }
    bzero(emitter->slots, sizeof(emitter->slots));
}

/* Switches the emitter to another EmitterTable, dropping its particles. */
void particle_setActorEmitterTable(ActorEmitterProcess *emitter, u8 table) {
    if (emitter->params.table != table) {
        particle_freeActorEmitterParticles(emitter);
        emitter->params.table = table;
    }
}

/* Starts an emitter process with the given params. */
ActorEmitterProcess *particle_newActorEmitter(ActorEmitterParams *params, u8 owner) {
    ActorEmitterProcess *emitter;

    if (params->owner->character_type == 150) {
        return NULL;
    }
    emitter = effect_newObject(0x21, 1, 0x9EC, 1, owner, 1);
    if (emitter == NULL) {
        return NULL;
    }
    memcpy(&emitter->params, params, sizeof(ActorEmitterParams));
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
