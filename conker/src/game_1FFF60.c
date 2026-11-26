#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/effect.h describes it. */
u8 D_800E0950[20];                 /* reference count per shared anchor resource (D_800AB140) */
u16 D_800E0968[20];                /* anchor count of each loaded set */
struct BoneAnchor *D_800E0990[20]; /* the loaded anchor set of each resource (type in game_1BC650) */

#include "functions.h"
#include "variables.h"

/* The ROM table ids of the shared anchor resources. */
static const u16 D_800AB140[20] = {
    0x1B0, 0x1B2, 0x1B3, 0x1B4, 0x1B5, 0x1B6, 0x1B7, 0x1B8, 0x1B9, 0x1BA,
    0x1BB, 0x1BC, 0x1BE, 0x1BD, 0x1C0, 0x1C1, 0x1C2, 0x1C3, 0x1C4, 0x1B1,
};

/* Object type 0x3D: a point on one bone of an actor, re-transformed every frame. */
struct BoneFollower {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Actor *actor;
    /* 0x14 */ u8 actorUid;         // actor->unique_id when spawned
    /* 0x15 */ u8 pad15[3];
    /* 0x18 */ Vec3f offset;        // point in bone space
    /* 0x24 */ u8 bone;             // index into actor->ob_matrix_ptr
    /* 0x25 */ u8 pad25;
    /* 0x26 */ s16 timer;
    /* 0x28 */ u8 flags;            // bit 0: timer counts down
    /* 0x29 */ s8 updateFn;         // index into D_8008FC40, -1 for none
    /* 0x2A */ s8 expireFn;         // index into D_8008FC48, -1 for none
    /* 0x2B */ u8 pad2B;
    /* 0x2C */ s32 attached;        // EffectHeader * destroyed with this one; matching: s32, a pointer changes regalloc
    /* 0x30 */ ActorFollowerGroup *group;
    /* 0x34 */ Vec3f position;      // offset transformed by the bone matrix
    /* 0x40 */ BoneFollower *next;
    /* 0x44 */ BoneFollower *prev;
}; // size 0x48

/* Object type 0x3E: watches an actor and owns a list of BoneFollowers. */
struct ActorFollowerGroup {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Actor *actor;
    /* 0x14 */ u8 actorUid;         // actor->unique_id when spawned
    /* 0x15 */ u8 pad15;
    /* 0x16 */ s16 timer;
    /* 0x18 */ u8 flags;            // bit 0: timer counts down; bit 1: actor is trackable (has bone matrices, (unk74 & 0xF) != 0xF)
    /* 0x19 */ s8 updateFn;         // index into D_8008FC4C, -1 for none
    /* 0x1A */ s8 trackFn;          // index into D_8008FC50, called when bit 1 of flags sets
    /* 0x1B */ s8 untrackFn;        // index into D_8008FC54, called when bit 1 of flags clears
    /* 0x1C */ s8 expireFn;         // index into D_8008FC58, -1 for none
    /* 0x1D */ u8 kind;             // index into D_8008FC5C and D_8008FC64
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s16 count;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ BoneFollower *head;
    /* 0x28 */ BoneFollower *tail;
    /* 0x2C */ u8 pad2C[4];
}; // size 0x30

/*
 * The follower and group callbacks, indexed by the fields named above. They are declared without a
 * prototype: each entry takes the follower or group as its own, longer view (EmitterParticle,
 * AnchorEmitter in game_1BC650.c). The message table keeps the prototype of its call, which passes the
 * type as a u8; its entry still takes the AnchorEmitter view (warning 709).
 */
static s32 (*D_8008FC40[])() = { func_1518FC04, func_1518FC44 }; /* BoneFollower.updateFn */
static void (*D_8008FC48[])() = { func_1518FC84 };               /* BoneFollower.expireFn */
static s32 (*D_8008FC4C[])() = { func_1518F7C4 };                /* ActorFollowerGroup.updateFn */
static void (*D_8008FC50[])() = { func_1518F89C };               /* ActorFollowerGroup.trackFn */
static void (*D_8008FC54[])() = { func_151D33B8 };               /* ActorFollowerGroup.untrackFn */
static void (*D_8008FC58[])() = { func_1518F858 };               /* ActorFollowerGroup.expireFn */
static void (*D_8008FC5C[])() = { NULL, func_151904BC };         /* per kind, before the followers go */
static void (*D_8008FC64[])(ActorFollowerGroup *, ActorMsg *, u8) = { NULL, func_15190550 }; /* per kind, after a message */


/* Takes a reference on shared resource `index`, loading it on the first reference. */
void func_151D2AB0(s32 index) {
    u32 loadInfo;

    D_800E0950[index]++;
    if (D_800E0950[index] < 2) {
        D_800E0990[index] = func_1502B5C8(&loadInfo, 2, 9, D_800AB140[index]);
        D_800E0968[index] = loadInfo >> 4;
    }
}

/* Drops a reference on shared resource `index`, freeing it with the last reference. */
void func_151D2B4C(s32 index) {
    if (D_800E0950[index] != 0) {
        D_800E0950[index]--;
        if (D_800E0950[index] == 0) {
            func_100043B4(D_800E0990[index], 4);
        }
    }
}

/* Spawns a BoneFollower from `params` (0x24 bytes from +0x10) and links it into its group. */
BoneFollower *func_151D2BA4(void *params, Vec3f *position, s32 extraSize, u8 arg3, s32 arg4) {
    BoneFollower *follower = func_15167A68(0x3D, arg4, extraSize + sizeof(BoneFollower), 1, arg3, 1);

    if (follower == NULL) {
        return NULL;
    }
    memcpy(&follower->actor, params, 0x24);
    follower->position = *position;
    func_151D3308(follower);
    return follower;
}

/* Per-frame update: tracks the bone, runs the timer and the update callback. */
void func_151D2C40(BoneFollower *follower) {
    Actor *actor;
    Vec3f position;

    actor = follower->actor;
    if (actor->type == 0 || actor->character_type == 0xFF || actor->unique_id != follower->actorUid ||
        actor->ob_matrix_ptr == NULL || (actor->unk74 & 0xF) == 0xF) {
        func_1516972C(&follower->header);
        return;
    }

    func_15143134(&follower->offset, &position, &actor->ob_matrix_ptr[follower->bone]);

    if (follower->flags & 1) {
        follower->timer -= D_800BE9E4;
        if (follower->timer < 0) {
            if (follower->expireFn != -1) {
                D_8008FC48[follower->expireFn](follower, &position);
            }
            follower->attached = 0;
            func_1516972C(&follower->header);
            return;
        }
    }

    if (follower->updateFn != -1 && D_8008FC40[follower->updateFn](follower, &position) == 0) {
        func_1516972C(&follower->header);
        return;
    }

    follower->position = position;
}

void func_151D2DAC(BoneFollower *follower) {
    func_151D3354(follower);
}

void func_151D2DCC(BoneFollower *arg0) {
    BoneFollower *follower = arg0; // matching: IDO keeps the pointer in a1 only via a local copy

    func_151D2DAC(follower);
    if (follower->attached != 0) {
        func_1516972C(follower->attached);
    }
    func_15169804(&follower->header);
}

void func_151D2E14(BoneFollower *arg0) {
    BoneFollower *follower = arg0; // matching: IDO keeps the pointer in a1 only via a local copy

    func_151D2DAC(follower);
    if (follower->attached != 0) {
        func_1516979C(follower->attached);
    }
    func_15169824(&follower->header);
}

/* Message handler: 0 destroys followers of the given actor; 0x2D swaps two actors. */
void func_151D2E5C(BoneFollower *follower, ActorMsg *msg, u8 type) {
    Actor *actor;
    Actor *otherActor;
    s32 actorUid;
    s32 msgUid;

    if (type == 0) {
        actor = follower->actor;
        otherActor = msg->ref.actor;
        actorUid = follower->actorUid;
        msgUid = msg->ref.uniqueId;
        if (actor == otherActor || msgUid == actorUid) {
            func_1516972C(&follower->header);
        }
    } else if (type == 0x2D) {
        actor = msg->swap.actorA;
        if (actor == follower->actor) {
            follower->actor = msg->swap.actorB;
            follower->actorUid = msg->swap.idB;
        } else if (follower->actor == msg->swap.actorB) {
            follower->actor = actor;
            follower->actorUid = msg->swap.idA;
        }
    }
}

/* Spawns an ActorFollowerGroup from `params` (0x10 bytes from +0x10) with an empty list. */
ActorFollowerGroup *func_151D2F00(void *params, s32 extraSize, u8 arg2, s32 arg3) {
    ActorFollowerGroup *group = func_15167A68(0x3E, arg3, extraSize + sizeof(ActorFollowerGroup), 1, arg2, 1);

    if (group == NULL) {
        return NULL;
    }
    memcpy(&group->actor, params, 0x10);
    group->count = 0;
    group->head = NULL;
    group->tail = NULL;
    group->flags &= ~2;
    return group;
}

/* Per-frame update: refreshes the trackable flag, runs the timer and the callbacks. */
void func_151D2F90(ActorFollowerGroup *group) {
    Actor *actor;
    u8 wasTrackable;
    u8 isTrackable;

    actor = group->actor;
    wasTrackable = group->flags & 2;
    if (actor->type == 0 || actor->character_type == 0xFF || actor->unique_id != group->actorUid) {
        func_1516972C(&group->header);
        return;
    }
    if (actor->ob_matrix_ptr != NULL && (actor->unk74 & 0xF) != 0xF) {
        group->flags |= 2;
    } else {
        group->flags &= ~2;
    }
    if (group->flags & 1) {
        group->timer -= D_800BE9E4;
        if (group->timer < 0) {
            if (group->expireFn != -1) {
                D_8008FC58[group->expireFn](group);
            }
            func_1516972C(&group->header);
            return;
        }
    }
    isTrackable = group->flags & 2;
    if (wasTrackable != isTrackable) {
        if (isTrackable) {
            D_8008FC50[group->trackFn](group);
        } else {
            D_8008FC54[group->untrackFn](group);
        }
    }
    if (group->updateFn != -1 && D_8008FC4C[group->updateFn](group) == 0) {
        func_1516972C(&group->header);
    }
}

/* Destroys every follower in the group (running their expire callbacks) and releases the actor. */
void func_151D3130(ActorFollowerGroup *group) {
    void (*onDestroy)(ActorFollowerGroup *);
    BoneFollower *follower;
    BoneFollower *next;

    onDestroy = D_8008FC5C[group->kind];
    if (onDestroy != NULL) {
        onDestroy(group);
    }
    follower = group->head;
    if (follower != NULL) {
        do {
            next = follower->next;
            if (follower->expireFn != -1) {
                D_8008FC48[follower->expireFn](follower, &follower->position);
            }
            follower->attached = 0;
            func_1516972C(&follower->header);
            follower = next;
        latch: // matching: the label ends the block so as1 fills the branch delay slot from the loop top
            ;
        } while (next != NULL);
    }
    func_1514EDF0(group, group->actor);
}

void func_151D31F4(ActorFollowerGroup *group) {
    func_151D3130(group);
    func_15169804(&group->header);
}

void func_151D3220(ActorFollowerGroup *group) {
    func_151D3130(group);
    func_15169824(&group->header);
}

/* Message handler: like func_151D2E5C, then forwards the message to the group kind's handler. */
void func_151D324C(ActorFollowerGroup *group, ActorMsg *msg, u8 type) {
    void (*onMessage)(ActorFollowerGroup *, ActorMsg *, u8);

    Actor *otherActor;

    if (type == 0) {
        func_151D33FC(group, msg);
    done: // matching: the label splits the block so the reload of type stays out of the delay slot
        ;
    } else if (type == 0x2D) {
        otherActor = msg->swap.actorA;
        if (group->actor == otherActor) {
            group->actor = msg->swap.actorB;
            group->actorUid = msg->swap.idB;
        } else if (group->actor == msg->swap.actorB) {
            group->actor = otherActor;
            group->actorUid = msg->swap.idA;
        }
    }

    onMessage = D_8008FC64[group->kind];
    if (onMessage != NULL) {
        onMessage(group, msg, type);
    }
}

/* Pushes the follower onto the front of its group's list. */
void func_151D3308(BoneFollower *follower) {
    ActorFollowerGroup *group = follower->group;
    BoneFollower *head;

    if (group != NULL) {
        follower->next = group->head;
        head = group->head;
        if (head != NULL) {
            head->prev = follower;
        } else {
            group->tail = follower;
        }
        follower->prev = NULL;
        group->head = follower;
        group->count++;
        follower->group = group;
    }
}

/* Unlinks the follower from its group's list. */
void func_151D3354(BoneFollower *follower) {
    ActorFollowerGroup *group;

    if (follower->group != NULL) {
        group = follower->group;
        if (follower->prev != NULL) {
            follower->prev->next = follower->next;
        } else {
            group->head = follower->next;
        }
        if (follower->next != NULL) {
            follower->next->prev = follower->prev;
        } else {
            group->tail = follower->prev;
        }
        group->count--;
    }
}

/* Destroys every follower in the group. */
void func_151D33B8(ActorFollowerGroup *group) {
    while (group->count > 0) {
        func_1516972C(&group->head->header);
    }
}

/* Destroys the group when message type 0 names its actor. */
void func_151D33FC(ActorFollowerGroup *group, ActorMsg *msg) {
    Actor *actor;
    s32 actorUid;
    s32 msgUid;
    Actor *otherActor;

    actor = group->actor;
    actorUid = group->actorUid;
    msgUid = msg->ref.uniqueId;
    otherActor = msg->ref.actor;
    if (actor == otherActor || msgUid == actorUid) {
        func_1516972C(&group->header);
    }
}

/* The effect-object kinds that func_151D343C sends its event to. */
static const IdList1 D_800AB168 = { { 0x3E } };

void func_151D343C(s32 arg0, u8 arg1) {
    IdList1 args;

    args = D_800AB168;
    func_15169260(args.ids, 1, arg0, arg1);
}
