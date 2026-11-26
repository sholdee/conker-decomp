#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/effect.h describes it. */
u8 gAnchorRefCounts[20];                 /* reference count per shared anchor resource (gAnchorResourceIds) */
u16 gAnchorCounts[20];                /* anchor count of each loaded set */
struct BoneAnchor *gAnchorSets[20]; /* the loaded anchor set of each resource (type in game_1BC650) */

#include "functions.h"
#include "variables.h"

/* The ROM table ids of the shared anchor resources. */
static const u16 gAnchorResourceIds[20] = {
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
    /* 0x29 */ s8 updateFn;         // index into gBoneFollowerUpdateFuncs, -1 for none
    /* 0x2A */ s8 expireFn;         // index into gBoneFollowerExpireFuncs, -1 for none
    /* 0x2B */ u8 pad2B;
    /* 0x2C */ s32 attached;        // EffectHeader * destroyed with this one; matching: s32, a pointer changes regalloc
    /* 0x30 */ ActorFollowerGroup *group;
    /* 0x34 */ Vec3f position;      // offset transformed by the bone matrix
    /* 0x40 */ BoneFollower *next;
    /* 0x44 */ BoneFollower *prev;
}; // size 0x48

/*
 * The follower and group callbacks, indexed by the fields named above. They are declared without a
 * prototype: each entry takes the follower or group as its own, longer view (EmitterParticle,
 * AnchorEmitter in game_1BC650.c). The message table keeps the prototype of its call, which passes the
 * type as a u8; its entry still takes the AnchorEmitter view (warning 709).
 */
static s32 (*gBoneFollowerUpdateFuncs[])() = { particle_followAnchorEased, particle_followAnchor }; /* BoneFollower.updateFn */
static void (*gBoneFollowerExpireFuncs[])() = { particle_launchAnchorParticle };               /* BoneFollower.expireFn */
static s32 (*gFollowerGroupUpdateFuncs[])() = { particle_updateAnchorEmitter };                /* ActorFollowerGroup.updateFn */
static void (*gFollowerGroupTrackFuncs[])() = { particle_emitAnchorBurst };               /* ActorFollowerGroup.trackFn */
static void (*gFollowerGroupUntrackFuncs[])() = { effect_clearGroupFollowers };               /* ActorFollowerGroup.untrackFn */
static void (*gFollowerGroupExpireFuncs[])() = { particle_runAnchorEmitterExpire };               /* ActorFollowerGroup.expireFn */
static void (*gFollowerGroupDestroyFuncs[])() = { NULL, particle_destroyAnchorEmitter };         /* per kind, before the followers go */
static void (*gFollowerGroupMessageFuncs[])(ActorFollowerGroup *, ActorMsg *, u8) = { NULL, effect_anchorEmitterMessage }; /* per kind, after a message */


/* Takes a reference on shared resource `index`, loading it on the first reference. */
void effect_acquireAnchorSet(s32 index) {
    u32 loadInfo;

    gAnchorRefCounts[index]++;
    if (gAnchorRefCounts[index] < 2) {
        gAnchorSets[index] = rom_loadPath(&loadInfo, 2, 9, gAnchorResourceIds[index]);
        gAnchorCounts[index] = loadInfo >> 4;
    }
}

/* Drops a reference on shared resource `index`, freeing it with the last reference. */
void effect_releaseAnchorSet(s32 index) {
    if (gAnchorRefCounts[index] != 0) {
        gAnchorRefCounts[index]--;
        if (gAnchorRefCounts[index] == 0) {
            heap_setTag(gAnchorSets[index], 4);
        }
    }
}

/* Spawns a BoneFollower from `params` (0x24 bytes from +0x10) and links it into its group. */
BoneFollower *effect_spawnBoneFollower(void *params, Vec3f *position, s32 extraSize, u8 arg3, s32 arg4) {
    BoneFollower *follower = effect_newObject(0x3D, arg4, extraSize + sizeof(BoneFollower), 1, arg3, 1);

    if (follower == NULL) {
        return NULL;
    }
    memcpy(&follower->actor, params, 0x24);
    follower->position = *position;
    effect_linkBoneFollower(follower);
    return follower;
}

/* Per-frame update: tracks the bone, runs the timer and the update callback. */
void effect_updateBoneFollower(BoneFollower *follower) {
    Actor *actor;
    Vec3f position;

    actor = follower->actor;
    if (actor->type == 0 || actor->character_type == 0xFF || actor->unique_id != follower->actorUid ||
        actor->ob_matrix_ptr == NULL || (actor->unk74 & 0xF) == 0xF) {
        effect_destroyObject(&follower->header);
        return;
    }

    math_transformOffset(&follower->offset, &position, &actor->ob_matrix_ptr[follower->bone]);

    if (follower->flags & 1) {
        follower->timer -= D_800BE9E4;
        if (follower->timer < 0) {
            if (follower->expireFn != -1) {
                gBoneFollowerExpireFuncs[follower->expireFn](follower, &position);
            }
            follower->attached = 0;
            effect_destroyObject(&follower->header);
            return;
        }
    }

    if (follower->updateFn != -1 && gBoneFollowerUpdateFuncs[follower->updateFn](follower, &position) == 0) {
        effect_destroyObject(&follower->header);
        return;
    }

    follower->position = position;
}

void func_151D2DAC(BoneFollower *follower) {
    effect_unlinkBoneFollower(follower);
}

void effect_destroyBoneFollower(BoneFollower *arg0) {
    BoneFollower *follower = arg0; // matching: IDO keeps the pointer in a1 only via a local copy

    func_151D2DAC(follower);
    if (follower->attached != 0) {
        effect_destroyObject(follower->attached);
    }
    effect_defaultDestroy(&follower->header);
}

void effect_freeBoneFollower(BoneFollower *arg0) {
    BoneFollower *follower = arg0; // matching: IDO keeps the pointer in a1 only via a local copy

    func_151D2DAC(follower);
    if (follower->attached != 0) {
        effect_freeObject(follower->attached);
    }
    effect_defaultFree(&follower->header);
}

/* Message handler: 0 destroys followers of the given actor; 0x2D swaps two actors. */
void effect_boneFollowerMessage(BoneFollower *follower, ActorMsg *msg, u8 type) {
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
            effect_destroyObject(&follower->header);
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

/* Spawns an ActorFollowerGroup from `params` (0x10 bytes from +0x10) with an empty list, followed by extraSize
 * bytes of the caller's data; the caller keeps the object in its own view (AnchorEmitter). */
void *effect_newFollowerGroup(void *params, s32 extraSize, u8 arg2, s32 arg3) {
    ActorFollowerGroup *group = effect_newObject(0x3E, arg3, extraSize + sizeof(ActorFollowerGroup), 1, arg2, 1);

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
void effect_updateFollowerGroup(ActorFollowerGroup *group) {
    Actor *actor;
    u8 wasTrackable;
    u8 isTrackable;

    actor = group->actor;
    wasTrackable = group->flags & 2;
    if (actor->type == 0 || actor->character_type == 0xFF || actor->unique_id != group->actorUid) {
        effect_destroyObject(&group->header);
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
                gFollowerGroupExpireFuncs[group->expireFn](group);
            }
            effect_destroyObject(&group->header);
            return;
        }
    }
    isTrackable = group->flags & 2;
    if (wasTrackable != isTrackable) {
        if (isTrackable) {
            gFollowerGroupTrackFuncs[group->trackFn](group);
        } else {
            gFollowerGroupUntrackFuncs[group->untrackFn](group);
        }
    }
    if (group->updateFn != -1 && gFollowerGroupUpdateFuncs[group->updateFn](group) == 0) {
        effect_destroyObject(&group->header);
    }
}

/* Destroys every follower in the group (running their expire callbacks) and releases the actor. */
void effect_destroyGroupFollowers(ActorFollowerGroup *group) {
    void (*onDestroy)(ActorFollowerGroup *);
    BoneFollower *follower;
    BoneFollower *next;

    onDestroy = gFollowerGroupDestroyFuncs[group->kind];
    if (onDestroy != NULL) {
        onDestroy(group);
    }
    follower = group->head;
    if (follower != NULL) {
        do {
            next = follower->next;
            if (follower->expireFn != -1) {
                gBoneFollowerExpireFuncs[follower->expireFn](follower, &follower->position);
            }
            follower->attached = 0;
            effect_destroyObject(&follower->header);
            follower = next;
        latch: // matching: the label ends the block so as1 fills the branch delay slot from the loop top
            ;
        } while (next != NULL);
    }
    effect_detachHandle(group, group->actor);
}

void effect_destroyFollowerGroup(ActorFollowerGroup *group) {
    effect_destroyGroupFollowers(group);
    effect_defaultDestroy(&group->header);
}

void effect_freeFollowerGroup(ActorFollowerGroup *group) {
    effect_destroyGroupFollowers(group);
    effect_defaultFree(&group->header);
}

/* Message handler: like effect_boneFollowerMessage, then forwards the message to the group kind's handler. */
void effect_followerGroupMessage(ActorFollowerGroup *group, ActorMsg *msg, u8 type) {
    void (*onMessage)(ActorFollowerGroup *, ActorMsg *, u8);

    Actor *otherActor;

    if (type == 0) {
        effect_handleFollowerGroupActorGone(group, msg);
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

    onMessage = gFollowerGroupMessageFuncs[group->kind];
    if (onMessage != NULL) {
        onMessage(group, msg, type);
    }
}

/* Pushes the follower onto the front of its group's list. */
void effect_linkBoneFollower(BoneFollower *follower) {
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
void effect_unlinkBoneFollower(BoneFollower *follower) {
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
void effect_clearGroupFollowers(ActorFollowerGroup *group) {
    while (group->count > 0) {
        effect_destroyObject(&group->head->header);
    }
}

/* Destroys the group when message type 0 names its actor. */
void effect_handleFollowerGroupActorGone(ActorFollowerGroup *group, ActorMsg *msg) {
    Actor *actor;
    s32 actorUid;
    s32 msgUid;
    Actor *otherActor;

    actor = group->actor;
    actorUid = group->actorUid;
    msgUid = msg->ref.uniqueId;
    otherActor = msg->ref.actor;
    if (actor == otherActor || msgUid == actorUid) {
        effect_destroyObject(&group->header);
    }
}

/* The effect-object kinds that effect_sendEventToFollowerGroups sends its event to. */
static const IdList1 gFollowerGroupTypes = { { 0x3E } };

void effect_sendEventToFollowerGroups(s32 arg0, u8 arg1) {
    IdList1 args;

    args = gFollowerGroupTypes;
    effect_sendEventToTypes(args.ids, 1, arg0, arg1);
}
