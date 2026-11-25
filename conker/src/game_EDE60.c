#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

/* Live particle spawned by an emitter (see func_15130280). */
typedef struct Particle Particle;

/* One entry of an emitter's slot pool; slots are chained by index, -1 ends a chain. */
typedef struct {
    /* 0x00 */ Particle *particle;
    /* 0x04 */ s16 next;
    /* 0x06 */ s16 pad6;
} EmitterSlot;

/* Describes an emitter; func_150C0AC0 copies it into the emitter. */
typedef struct {
    /* 0x00 */ vertex *source;     /* followed when flags & 2 */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ struct127 *target;  /* followed otherwise */
    /* 0x0C */ u8 targetId;        /* target->unique_id; a mismatch means the object was reused */
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ f32 rate;           /* particles per frame step while the target moves */
    /* 0x14 */ s16 slotCount;
    /* 0x16 */ s16 timer;          /* counts down when flags & 1 */
    /* 0x18 */ u8 flags;
    /* 0x19 */ u8 pad19[3];
} EmitterDesc; /* size 0x1C */

/* Emitter object that keeps spawning particles behind a moving object. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ EmitterDesc desc;
    /* 0x34 */ f32 lastX;
    /* 0x38 */ f32 lastZ;
    /* 0x3C */ f32 spawnAccum;
    /* 0x40 */ EmitterSlot *slots; /* desc.slotCount slots, stored right after the emitter */
    /* 0x44 */ s16 activeHead;     /* chain of slots holding a live particle */
    /* 0x46 */ s16 freeHead;       /* chain of unused slots */
} Emitter; /* size 0x48 */

struct Particle {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ vertex anchor;
    /* 0x4C */ u8 pad4C[0xA8 - 0x4C];
    /* 0xA8 */ Emitter *owner;
};

/* Effect callback: hands func_15169850 the emitter's target reference. */
void func_150C09B0(Emitter *emitter, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, (s32)&emitter->desc.target, (s32)&emitter->desc.targetId, (s32)emitter);
}

void func_150C0A48(Emitter *emitter);

/* Emitter destructors: free the live particles, then the emitter. */
void func_150C09F0(Emitter *emitter) {
    func_150C0A48(emitter);
    func_15169804((struct102 *)emitter);
}

extern void func_15169824(struct102 *);

void func_150C0A1C(Emitter *emitter) {
    func_150C0A48(emitter);
    func_15169824((struct102 *)emitter);
}

/* Frees every live particle of the emitter. */
void func_150C0A48(Emitter *emitter) {
    s16 i;

    for (i = emitter->activeHead; i != -1; i = emitter->slots[i].next) {
        func_1516972C((struct102 *)emitter->slots[i].particle);
    }
}

/* Allocates an emitter from desc with desc->slotCount particle slots, all initially free. */
Emitter *func_150C0AC0(EmitterDesc *desc, s32 arg1, s32 arg2) {
    Emitter *emitter;
    s32 size;
    s16 i;

    size = desc->slotCount * sizeof(EmitterSlot) + sizeof(Emitter);
    if (!(desc->flags & 2)) {
        if (desc->target == NULL) {
            return NULL;
        }
        if (desc->target->interaction_state == 0) {
            return NULL;
        }
    }

    emitter = func_15167A68(0x25, arg2, size, 1, (u8)arg1, 1);
    if (emitter == NULL) {
        return NULL;
    }

    memcpy(&emitter->desc, desc, sizeof(EmitterDesc));
    emitter->slots = (EmitterSlot *)(emitter + 1);
    if (desc->flags & 2) {
        emitter->lastX = emitter->desc.source->x;
        emitter->lastZ = emitter->desc.source->z;
    } else {
        emitter->lastX = emitter->desc.target->x_position;
        emitter->lastZ = emitter->desc.target->z_position;
    }
    emitter->spawnAccum = 0.0f;
    emitter->activeHead = -1;
    emitter->freeHead = 0;
    for (i = 0; i < desc->slotCount - 1; i++) {
        emitter->slots[i].next = i + 1;
    }
    emitter->slots[desc->slotCount - 1].next = -1;
    emitter->unk10 = 1;
    emitter->unk14 = 0;
    return emitter;
}

struct s150C0C38_obj {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    u8 pad18[4];
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
    f32 unk3C;
    vertex unk40;
    u8 pad4C[0x38];
    u16 unk84;
    u8 pad86[0xFA];
    f32 unk180;
};

struct s150C0C38_ent {
    struct s150C0C38_obj *unk0;
    s16 unk4;
    s16 unk6;
};

struct s150C0C38 {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0xB];
    vertex *unk18;
    u8 pad1C[4];
    struct s150C0C38_obj *unk20;
    u8 unk24;
    u8 pad25[3];
    f32 unk28;
    u8 pad2C[2];
    s16 unk2E;
    u8 unk30;
    u8 pad31[3];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    struct s150C0C38_ent *unk40;
    s16 unk44;
    s16 unk46;
};

struct s150C0C38_spawn {
    s32 unk00;
    s32 unk04;
    u16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    vertex unk30;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    s8 unk65;
    u8 pad66[0x2];
};

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_150C0C38(struct s150C0C38 *arg0) {
    struct s150C0C38 *self[1];
    volatile u8 kill;
    vertex pos;
    struct s150C0C38_obj *obj;
    s16 i;
    s32 bit;
    struct s150C0C38_spawn sp80;
    struct s150C0C38 *sp7C;

    self[0] = arg0;
    kill = 0;
    if (!(arg0->unk30 & 2)) {
        obj = arg0->unk20;
        if (obj->unk0 == 0) {
            kill = 1;
        }
        if (obj->unk3B != arg0->unk24) {
            kill = 1;
        }
        pos.x = obj->unk14;
        pos.y = arg0->unk20->unk180;
        pos.z = arg0->unk20->unk1C;
    } else {
        memcpy(&pos, arg0->unk18, 0xC);
        pos.y -= 65.0f;
    }
    if (kill == 0) {
        s32 ok;

        ok = 1;
        for (i = arg0->unk44; i != -1; i = arg0->unk40[i].unk4) {
            arg0->unk40[i].unk0->unk40 = pos;
        }
        if (arg0->unk30 & 4) {
            if ((arg0->unk20->unk84 != 0x1D) && (arg0->unk20->unk84 != 0x1E)) {
                ok = 0;
            }
        }
        if (arg0->unk20->unk3C < 3.0f) {
            ok = 0;
        }
        if (((pos.x != arg0->unk34) || (pos.z != arg0->unk38)) && ok) {
            arg0->unk3C += arg0->unk28 * D_800BE9A4;
            if (arg0->unk3C > 1.0f) {
                s16 idx;
                u8 angle;
                f32 sinA;
                f32 cosA;
                f32 speed;

                sp80.unk1D = 0x6C;
                sp80.unk08 = 0x5103;
                sp80.unk00 = 0x200005;
                sp80.unk04 = 0x1F0600;
                sp80.unk0C = 0;
                sp80.unk10 = 0;
                sp80.unk17 = 0xFF;
                sp80.unk1C = 0xFF;
                sp80.unk48 = 0.0f;
                sp80.unk4C = 0.0f;
                sp80.unk50 = 0.0f;
                sp80.unk54 = 0.0f;
                sp80.unk1E = 0x1E;
                sp80.unk20 = 8;
                sp80.unk58 = 0x80DE01;
                sp80.unk60 = 8;
                sp80.unk61 = 6;
                sp80.unk62 = -1;
                sp80.unk63 = 0;
                sp80.unk64 = -1;
                sp80.unk65 = 0;
                sp7C = arg0;
                if ((arg0->unk3C > 1.0f) && (arg0->unk46 != -1)) {
                    do {
                        idx = arg0->unk46;
                        angle = func_150ADA20() & 0xFFFF;
                        sinA = func_151423D8(angle - 0x40);
                        cosA = func_151423D8(angle);
                        speed = (func_150ADA68() * 33.0f) + 40.0f;
                        sp80.unk18 = 0x52;
                        sp80.unk19 = 0x6B;
                        sp80.unk1A = 0x97;
                        sp80.unk14 = 0x87;
                        sp80.unk15 = 0x6B;
                        sp80.unk16 = 0x97;
                        sp80.unk1B = (func_150ADA20() % 121U) + 0x50;
                        sp80.unk28 = sp80.unk2C = (func_150ADA68() * 17.0f) + 67.0f;
                        sp80.unk30 = pos;
                        sp80.unk3C = speed * sinA;
                        sp80.unk40 = 0.0f;
                        sp80.unk44 = speed * cosA;
                        sp80.unk58 &= ~0xC0;
                        if (func_150ADA20() & 1) {
                            bit = 0x80;
                        } else {
                            bit = 0;
                        }
                        sp80.unk58 |= (u32)(((func_150ADA20() & 1) ? 0x40 : 0) | bit);
                        sp80.unk0A = sp80.unk22 = (func_150ADA20() % 22U) + 0x46;
                        sp80.unk24 = 1.035226941f;
                        arg0->unk40[idx].unk0 = func_15130280(&sp80, 1, 0, 4, arg0->unkC, arg0->unk1);
                        if (arg0->unk40[idx].unk0 != NULL) {
                            memcpy((u8 *)arg0->unk40[idx].unk0 + 0xA8, &sp7C, 4);
                            arg0->unk46 = arg0->unk40[idx].unk4;
                            arg0->unk40[idx].unk4 = arg0->unk44;
                            arg0->unk44 = idx;
                        }
                        arg0->unk3C -= 1.0f;
                    } while ((arg0->unk3C > 1.0f) && (self[0]->unk46 != -1));
                }
                while (arg0->unk3C > 1.0f) {
                    arg0->unk3C -= 1.0f;
                }
            }
        }
        arg0->unk34 = pos.x;
        arg0->unk38 = pos.z;
    }
    if (arg0->unk30 & 1) {
        arg0->unk2E -= D_800BE9E4;
        if (arg0->unk2E < 0) {
            kill = 1;
        }
    }
    if (kill != 0) {
        func_1516972C((struct102 *)arg0);
    }
}

/* Particle destructor hook: moves the particle's slot from its emitter's active chain to the free chain. */
void func_150C1198(Particle *particle) {
    Emitter *emitter;
    s32 found;
    s16 prev;
    s16 i;

    emitter = particle->owner;
    found = FALSE;
    prev = -1;
    i = emitter->activeHead;
    while (i != -1 && !found) {
        if (particle == emitter->slots[i].particle) {
            found = TRUE;
        } else {
            prev = i;
            i = emitter->slots[i].next;
        }
    }

    if (found) {
        if (i == emitter->activeHead) {
            emitter->activeHead = emitter->slots[i].next;
        } else {
            emitter->slots[prev].next = emitter->slots[i].next;
        }
        emitter->slots[i].next = emitter->freeHead;
        emitter->freeHead = i;
    }
}
