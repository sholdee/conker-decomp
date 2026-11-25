#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A scripted actor move: actor `actor` is sent to pose `poseIndex` until `timer` runs out. */
typedef struct {
    /* 0x00 */ u8 state; // 0 = idle, 1 = start pending, 2 = running
    /* 0x01 */ u8 actor; // index into D_800DBFF0
    /* 0x02 */ u8 poseIndex; // index into the D_800DC020 pose table
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 timer;
    /* 0x06 */ u8 pad6[6];
    /* 0x0C */ s32 flags;
    /* 0x10 */ f32 scale; // copied (truncated) to D_800D2E30 when the command starts
    /* 0x14 */ u8 pad14[0xC];
    /* 0x20 */ f32 unk20; // actor->unk37C at setup, written to actor->unk3A4 when done
} C3E20Command; // size 0x24

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ u8 pad6[6];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} C3E20Pose; // size 0x18

extern C3E20Command D_800D2DC0[3];
extern s32 D_800D2E30[];
extern f32 D_800D2DB8; // actor->unk2C0 saved while a command runs

// variables.h declares D_800C3600 as `struct168 *` and D_800DC020 as `s32`, but D_800C3600 is the
// struct168 itself and D_800DC020 holds a C3E20Pose table pointer; hence the casts below.

void func_1512D560(Camera *, s32, struct168 *);

/* Clears all command slots. */
void func_15096970(void) {
    bzero(D_800D2DC0, sizeof(D_800D2DC0));
    D_800D2DB4 = 0;
}

/* Returns TRUE if any of the first `count` command slots is active. */
s32 func_150969A0(s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        if (D_800D2DC0[i].state != 0) {
            return TRUE;
        }
    }

    return FALSE;
}

/*
 * Advances command slot `slot`: starts the actor's move to its pose, then counts the timer down and
 * restores the actor when it runs out. Returns FALSE once the command has finished.
 */
s32 func_15096A68(s32 slot) {
    switch (D_800D2DC0[slot].state) {
    case 1: {
        u8 actorIndex;
        C3E20Pose *pose;
        Camera *actor;

        actorIndex = D_800D2DC0[slot].actor;
        actor = &D_800DBFF0[actorIndex];
        pose = (C3E20Pose *)D_800DC020;
        // matching: the redundant mask on the u8 index steers IDO's register allocation
        pose = &pose[D_800D2DC0[slot].poseIndex & 0xFF];
        D_800D2DB8 = actor->target.y;
        D_800D2DB4 = 1;
        D_800DBFF4[actorIndex] = 3;
        ((struct168 *)&D_800C3600)->unk0 = 1;
        ((struct168 *)&D_800C3600)->unk4 = 0;
        ((struct168 *)&D_800C3600)->unk14 = pose->x;
        ((struct168 *)&D_800C3600)->unk18 = pose->y;
        ((struct168 *)&D_800C3600)->unk1C = pose->z;
        ((struct168 *)&D_800C3600)->unk20 = pose->unk14;
        ((struct168 *)&D_800C3600)->unk24 = pose->unkC;
        ((struct168 *)&D_800C3600)->unk28 = pose->unk10;
        ((struct168 *)&D_800C3600)->unk8 = 0.0f;
        func_1512D560(actor, 5, NULL);
        func_1512D560(actor, 7, (struct168 *)&D_800C3600);
        if (!(D_800D2DC0[slot].flags & 1)) {
            actor->player->unk7 = 0;
            actor->player->unk25C |= 0x200;
        }
        D_800D2E30[slot] = D_800D2DC0[slot].scale;
        D_800D2DC0[slot].state = 2;
    }
        break;
    case 2: {
        Camera *actor;

        actor = &D_800DBFF0[D_800D2DC0[slot].actor];
        D_800D2DC0[slot].timer -= D_800BE9A0;
        if (actor->player->health == 0) {
            D_800D2DC0[slot].timer = 0;
        }
        if (D_800D2DC0[slot].timer <= 0) {
            actor->unk3A4 = D_800D2DC0[slot].unk20;
            if (!(D_800D2DC0[slot].flags & 2)) {
                func_1512D560(actor, 6, NULL);
            }
            D_800D2DB4 = 0;
            if (!(D_800D2DC0[slot].flags & 1)) {
                actor->player->unk25C &= ~0x200;
                actor->player->unk7 = 0xFF;
            }
            D_800D2DC0[slot].timer = 0;
            D_800D2DC0[slot].state = 0;
            actor->target.y = D_800D2DB8;
            return FALSE;
        }
    }
        break;
    }
    return TRUE;
}

/* Runs the active command slots until one of them is still busy. */
void func_15096D08(void) {
    s32 i;

    if (D_800C35EA != 1) {
        for (i = 0; i < 3; i++) {
            if (D_800D2DC0[i].state != 0 && func_15096A68(i) != 0) {
                break;
            }
        }
    }
}

extern s32 D_800D3840;
extern s32 D_800DC310[][0x1A];
extern s8 D_800C365C;
s32 func_15084000(s32);
void func_151254F4(Camera *actor, s32 arg1);
void func_1512A390(Camera *actor);

/*
 * Applies script command `command` with arguments args[2..] to actor `actorIndex`.
 * Fixed-point arguments are 16.16. Returns 1 for some commands, else 0.
 */
s32 func_15096D78(s32 actorIndex, s32 command, s32 *args) {
    Camera *actor;

    actor = &D_800DBFF0[actorIndex];
    if (actorIndex > D_80082FA0) {
        return 0;
    }
    switch (command) {
    case 12:
        D_800D2DC0[args[2]].unk20 = actor->yaw;
        D_800D2DC0[args[2]].actor = actorIndex;
        D_800D2DC0[args[2]].poseIndex = args[3];
        D_800D2DC0[args[2]].timer = args[4];
        D_800D2DC0[args[2]].state = 1;
        if (D_800D3840 >= 6) {
            D_800D2DC0[args[2]].flags = args[5];
        } else {
            D_800D2DC0[args[2]].flags = 0;
        }
        if (D_800D3840 >= 7) {
            D_800D2DC0[args[2]].scale = args[6] * (1.0f / 65536.0f);
        } else {
            D_800D2DC0[args[2]].scale = 1.0f;
        }
        if (!func_150969A0(args[2])) {
            func_15096A68(args[2]);
        }
        break;
    case 4:
        func_15123934(actor, args[2], args[3], args[4], 0x11);
        return 1;
    case 5:
        func_151239CC(actor, 0x11);
        return 1;
    case 6:
        if (args[2] != 0) {
            actor->flags |= args[3];
        } else {
            actor->flags &= ~args[3];
        }
        return 1;
    case 8:
        if (actor->playerExt->unk120 == 0) {
            actor->targetDistance = args[2];
            actor->unk34C = args[3];
            actor->height = args[3];
        }
        return 1;
    case 7:
        actor->presetMask = args[2];
        actor->preset = args[3];
        return 1;
    case 9:
        actor->unk134 = args[2];
        return 1;
    case 10: {
        C3E20Pose *pose;

        pose = (C3E20Pose *)D_800DC020;
        pose = &pose[args[2] & 0xFFF];
        D_800D2DB4 = 1;
        D_800DBFF4[actorIndex] = 3;
        ((struct168 *)&D_800C3600)->unk0 = 1;
        ((struct168 *)&D_800C3600)->unk4 = 0;
        ((struct168 *)&D_800C3600)->unk14 = pose->x;
        ((struct168 *)&D_800C3600)->unk18 = pose->y;
        ((struct168 *)&D_800C3600)->unk1C = pose->z;
        ((struct168 *)&D_800C3600)->unk20 = pose->unk14;
        ((struct168 *)&D_800C3600)->unk24 = pose->unkC;
        ((struct168 *)&D_800C3600)->unk28 = pose->unk10;
        ((struct168 *)&D_800C3600)->unk8 = 0.0f;
        D_800C365C = 0;
        func_1512D560(actor, 5, NULL);
        func_1512D560(actor, 7, (struct168 *)&D_800C3600);
        D_800CC2D0[0].unk25C |= 0x200;
    }
        break;
    case 11:
        func_1512D560(actor, 6, NULL);
        D_800D2DB4 = 0;
        D_800CC2D0[0].unk25C &= ~0x200;
        break;
    case 14:
        actor->focus.x = args[2];
        actor->focus.y = args[3];
        *(f32 *)&actor->focus.z = args[4];
        actor->target.x = args[2];
        actor->target.y = args[3];
        actor->target.z = args[4];
        actor->eye.x = args[5];
        actor->eye.y = args[6];
        actor->eye.z = args[7];
        actor->unk358 = actor->eye.y;
        func_1512A390(actor);
        break;
    case 2:
        actor->eye.x = args[2];
        actor->eye.y = args[3];
        actor->eye.z = args[4];
        actor->unk358 = actor->eye.y;
        func_1512A390(actor);
        break;
    case 3:
        actor->focus.x = args[2];
        actor->focus.y = args[3];
        *(f32 *)&actor->focus.z = args[4];
        break;
    case 15:
        actor->sideTarget = args[2];
        if (D_800D3840 == 4) {
            actor->side = args[3];
        }
        break;
    case 16:
        actor->liftTarget = args[2];
        if (D_800D3840 == 4) {
            actor->lift = args[3];
        }
        break;
    case 17:
        if (D_800D3840 == 5 && args[4] != 0) {
            func_1512D748(actor, args[2], args[3]);
            return 1;
        }
        if (D_800DC310[actorIndex][0] == 5) {
            func_1512D748(actor, args[2], args[3]);
            return 1;
        }
        return 0;
    case 18:
        func_151254F4(actor, func_15084000(args[2] & 0xFFF));
        break;
    case 19:
        func_15124770(actor, func_15084000(args[2] & 0xFFF));
        break;
    case 20:
        actor->unk1A4 = args[2] * (1.0f / 65536.0f);
        actor->unk1A8 = args[3] * (1.0f / 65536.0f);
        if (D_800D3840 == 5 && args[4] != 0) {
            actor->unk19C = actor->unk1A4;
            actor->unk1A0 = actor->unk1A8;
        }
        break;
    case 21: {
        C3E20Pose *pose;

        pose = (C3E20Pose *)D_800DC020;
        pose = &pose[args[2] & 0xFFF];
        pose->x = args[3];
        pose->y = args[4];
        pose->z = args[5];
    }
        break;
    case 22:
        actor->snap = 1;
        break;
    case 23:
        actor->unk7C0 = args[2] * (1.0f / 65536.0f);
        break;
    case 24:
        if (D_800D3840 == 5) {
            actor->aim.useOverride = 1;
            actor->aim.overrideYawOffset = args[2];
            actor->aim.overridePitch = args[3];
            actor->aim.overrideDistance = args[4];
        } else {
            actor->aim.useOverride = 0;
        }
        break;
    case 25:
        actor->unk3A4 = args[2] * (1.0f / 65536.0f);
        D_800D2DB0 = actor->unk3A4;
        break;
    case 26:
        func_1512868C(actor);
        break;
    case 27:
        D_80089520 = args[2] - 1;
        break;
    case 13:
        break;
    default:
        return 0;
    }
    return 0;
}


Actor *func_1505EEF4(s32);

/*
 * Answers script query `query` about actor `actorIndex` (arguments in args[2..]); fixed-point results
 * are 16.16.
 */
s32 func_1509759C(s32 actorIndex, s32 query, s32 *args) {
    Camera *actor;
    s32 side;
    struct17 facing;
    struct17 toTarget;
    f32 angle;
    Actor *target;
    C3E20Pose *pose;
    f32 dx;
    f32 dz;

    actor = &D_800DBFF0[actorIndex];
    if (actorIndex > D_80082FA0) {
        return 0;
    }
    switch (query) {
    case 31:
        return actor->unk390 * 65536.0f;
    case 30:
        return actor->yaw * 65536.0f;
    case 29:
        return D_800D2DC0[args[2]].state;
    case 32:
        // Horizontal distance from the actor to a pose.
        pose = (C3E20Pose *)D_800DC020;
        pose = &pose[args[2] & 0xFFF];
        dx = actor->eye.x - pose->x;
        dz = actor->eye.z - pose->z;
        return sqrtf(dx * dx + dz * dz);
    case 33:
        // Which side of the actor's facing direction a path object is on: -1 or 1 (-1 if not found).
        target = func_1505EEF4(args[2] & 0xFFF);
        if (target == NULL) {
            return -1;
        }
        angle = actor->yawRad + 1.570796371f;
        facing.unk0 = sinf(angle);
        facing.unk4 = 0.0f;
        facing.unk8 = cosf(angle);
        func_15048F90((struct17 *)&actor->eye.x, (struct17 *)&target->x_position, &toTarget);
        toTarget.unk4 = 0.0f;
        if (func_150AD900((f32 *)&facing, (f32 *)&toTarget) >= 0.0f) {
            side = -1;
        } else {
            side = 1;
        }
        return side;
    default:
        // matching: a plain `return 0` lets as1 fill the jump-table range check's delay slot
        goto unknown_query;
    }
unknown_query:
    return 0;
}

/* While a command runs, moves both actor points (unk2A4, unk2F8) along the unit vector between them, scaled by D_800D2E30. */
void func_15097798(s32 actorIndex) {
    Camera *actor;
    struct17 dir;

    actor = &D_800DBFF0[actorIndex];
    if (D_800D2DB4 != 0) {
        func_15048F90((struct17 *)&actor->focus.x, (struct17 *)&actor->eye.x, &dir);
        func_1504917C(&dir, &dir);

        actor->focus.x += dir.unk0 * -2.5f * D_800D2E30[actorIndex];
        actor->focus.y += dir.unk4 * -2.0f * D_800D2E30[actorIndex];
        *(f32 *)&actor->focus.z += dir.unk8 * -2.5f * D_800D2E30[actorIndex];
        actor->eye.x += dir.unk0 * -2.5f * D_800D2E30[actorIndex];
        actor->eye.y += dir.unk4 * -2.0f * D_800D2E30[actorIndex];
        actor->eye.z += dir.unk8 * -2.5f * D_800D2E30[actorIndex];
    }
}
