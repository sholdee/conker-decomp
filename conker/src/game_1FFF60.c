#include <ultra64.h>

#include "functions.h"
#include "variables.h"

typedef struct BoneFollower BoneFollower;
typedef struct ActorFollowerGroup ActorFollowerGroup;

/* Object type 0x3D: a point on one bone of an actor, re-transformed every frame. */
struct BoneFollower {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Actor *actor;
    /* 0x14 */ u8 actorUid;         // actor->unique_id when spawned
    /* 0x15 */ u8 pad15[3];
    /* 0x18 */ Vec3f offset;        // point in bone space
    /* 0x24 */ u8 bone;             // index into actor->unk1D4
    /* 0x25 */ u8 pad25;
    /* 0x26 */ s16 timer;
    /* 0x28 */ u8 flags;            // bit 0: timer counts down
    /* 0x29 */ s8 updateFn;         // index into D_8008FC40, -1 for none
    /* 0x2A */ s8 expireFn;         // index into D_8008FC48, -1 for none
    /* 0x2B */ u8 pad2B;
    /* 0x2C */ s32 attached;        // struct102 * destroyed with this one; matching: s32, a pointer changes regalloc
    /* 0x30 */ ActorFollowerGroup *group;
    /* 0x34 */ Vec3f position;      // offset transformed by the bone matrix
    /* 0x40 */ BoneFollower *next;
    /* 0x44 */ BoneFollower *prev;
}; // size 0x48

/* Object type 0x3E: watches an actor and owns a list of BoneFollowers. */
struct ActorFollowerGroup {
    /* 0x00 */ u8 pad0[0x10];
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

void *func_15167A68(s32, s32, s32, s32, s32, s32);
void func_15143134(Vec3f *in, Vec3f *out, MtxF *mtx);
void func_1514EDF0(ActorFollowerGroup *, Actor *);

void func_151D3308(BoneFollower *follower);
void func_151D3354(BoneFollower *follower);
void func_151D33FC(ActorFollowerGroup *group, struct223 *msg);

extern s32 (*D_8008FC40[])(BoneFollower *, Vec3f *);
extern void (*D_8008FC48[])(BoneFollower *, Vec3f *);
extern s32 (*D_8008FC4C[])(ActorFollowerGroup *);
extern void (*D_8008FC50[])(ActorFollowerGroup *);
extern void (*D_8008FC54[])(ActorFollowerGroup *);
extern void (*D_8008FC58[])(ActorFollowerGroup *);
extern void (*D_8008FC5C[])(ActorFollowerGroup *);
extern void (*D_8008FC64[])(ActorFollowerGroup *, struct223 *, u8);

// D_800E0950 is a u8 array of reference counts; variables.h declares it as a scalar.

/* Takes a reference on shared resource `index`, loading it on the first reference. */
void func_151D2AB0(s32 index) {
    u32 loadInfo;

    (&D_800E0950)[index]++;
    if ((&D_800E0950)[index] < 2) {
        D_800E0990[index] = func_1502B5C8(&loadInfo, 2, 9, D_800AB140[index]);
        D_800E0968[index] = loadInfo >> 4;
    }
}

/* Drops a reference on shared resource `index`, freeing it with the last reference. */
void func_151D2B4C(s32 index) {
    if ((&D_800E0950)[index] != 0) {
        (&D_800E0950)[index]--;
        if ((&D_800E0950)[index] == 0) {
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
    if (actor->interaction_state == 0 || actor->id == 0xFF || actor->unique_id != follower->actorUid ||
        actor->unk1D4 == NULL || (actor->unk74 & 0xF) == 0xF) {
        func_1516972C((struct102 *)follower);
        return;
    }

    func_15143134(&follower->offset, &position, &((MtxF *)actor->unk1D4)[follower->bone]);

    if (follower->flags & 1) {
        follower->timer -= D_800BE9E4;
        if (follower->timer < 0) {
            if (follower->expireFn != -1) {
                D_8008FC48[follower->expireFn](follower, &position);
            }
            follower->attached = 0;
            func_1516972C((struct102 *)follower);
            return;
        }
    }

    if (follower->updateFn != -1 && D_8008FC40[follower->updateFn](follower, &position) == 0) {
        func_1516972C((struct102 *)follower);
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
        func_1516972C((struct102 *)follower->attached);
    }
    func_15169804((struct102 *)follower);
}

void func_151D2E14(BoneFollower *arg0) {
    BoneFollower *follower = arg0; // matching: IDO keeps the pointer in a1 only via a local copy

    func_151D2DAC(follower);
    if (follower->attached != 0) {
        func_1516979C((struct102 *)follower->attached);
    }
    func_15169824((struct102 *)follower);
}

/* Message handler: 0 destroys followers of the given actor; 0x2D swaps two actors. */
void func_151D2E5C(BoneFollower *follower, struct223 *msg, u8 type) {
    s32 actor;
    s32 otherActor;
    s32 actorUid;
    s32 msgUid;

    if (type == 0) {
        actor = (s32)follower->actor;
        otherActor = msg->unk0;
        actorUid = follower->actorUid;
        msgUid = msg->unk4.ub;
        if (actor == otherActor || msgUid == actorUid) {
            func_1516972C((struct102 *)follower);
        }
    } else if (type == 0x2D) {
        actor = msg->unk0;
        if (actor == (s32)follower->actor) {
            follower->actor = (Actor *)msg->unk4.w;
            follower->actorUid = msg->unk9;
        } else if ((s32)follower->actor == msg->unk4.w) {
            follower->actor = (Actor *)actor;
            follower->actorUid = msg->unk8;
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
    if (actor->interaction_state == 0 || actor->id == 0xFF || actor->unique_id != group->actorUid) {
        func_1516972C((struct102 *)group);
        return;
    }
    if (actor->unk1D4 != NULL && (actor->unk74 & 0xF) != 0xF) {
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
            func_1516972C((struct102 *)group);
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
        func_1516972C((struct102 *)group);
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
            func_1516972C((struct102 *)follower);
            follower = next;
        latch: // matching: the label ends the block so as1 fills the branch delay slot from the loop top
            ;
        } while (next != NULL);
    }
    func_1514EDF0(group, group->actor);
}

void func_151D31F4(ActorFollowerGroup *group) {
    func_151D3130(group);
    func_15169804((struct102 *)group);
}

void func_151D3220(ActorFollowerGroup *group) {
    func_151D3130(group);
    func_15169824((struct102 *)group);
}

/* Message handler: like func_151D2E5C, then forwards the message to the group kind's handler. */
void func_151D324C(ActorFollowerGroup *group, struct223 *msg, u8 type) {
    void (*onMessage)(ActorFollowerGroup *, struct223 *, u8);

    s32 otherActor;

    if (type == 0) {
        func_151D33FC(group, msg);
    done: // matching: the label splits the block so the reload of type stays out of the delay slot
        ;
    } else if (type == 0x2D) {
        otherActor = msg->unk0;
        if ((s32)group->actor == otherActor) {
            group->actor = (Actor *)msg->unk4.w;
            group->actorUid = msg->unk9;
        } else if ((s32)group->actor == msg->unk4.w) {
            group->actor = (Actor *)otherActor;
            group->actorUid = msg->unk8;
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
        func_1516972C((struct102 *)group->head);
    }
}

/* Destroys the group when message type 0 names its actor. */
void func_151D33FC(ActorFollowerGroup *group, struct223 *msg) {
    s32 actor;
    s32 actorUid;
    s32 msgUid;
    s32 otherActor;

    actor = (s32)group->actor;
    actorUid = group->actorUid;
    msgUid = msg->unk4.ub;
    otherActor = msg->unk0;
    if (actor == otherActor || msgUid == actorUid) {
        func_1516972C((struct102 *)group);
    }
}

typedef struct {
    s32 unk0;
} Func151D343CArgs;

void func_151D343C(s32 arg0, u8 arg1) {
    Func151D343CArgs args;

    args = *(Func151D343CArgs *)D_800AB168; // matching: a struct copy, not an s32 load
    func_15169260(&args.unk0, 1, arg0, arg1);
}
